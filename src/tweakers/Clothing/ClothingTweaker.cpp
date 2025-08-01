//
// Created by exozg on 17/05/2025.
//

#include "ClothingTweaker.h"

#include "pugixml.hpp"
#include "SkintoneVariantGenerator.h"
#include "../Tweakers.h"
#include "../../core/assets/Assets.h"
#include "../../core/hash/FNV.h"
#include "../../core/modloader/Mod.h"
#include "../../core/modloader/Mods.h"
#include "../../core/resource/IdResolver.h"
#include "../../core/system/Logger.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../formats/builders/MaterialBuilder.h"
#include "../../formats/builders/MaterialSetBuilder.h"
#include "../../formats/materials/MaterialSet.h"

struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    void write(const void* data, size_t size) override {
        result.append(static_cast<const char*>(data), size);
    }
};

EA::ResourceMan::Key MasterInventoryKey = {
    .mInstance = Msml::Hash::FNV::FromString64("CAS.inventory"),
    .mType = static_cast<uint32_t>(Msml::Core::FileType::XML),
    .mGroup = 0,
};

bool ClothingTweaker::OnLoad(Msml::Core::Resource::CustomRecord &asset) {
    if (asset.mKey == MasterInventoryKey) {
        MSML_LOG_INFO("Patching Master Inventory");

        const std::string xml_string = Msml::Core::Util::StreamUtil::ReadString(asset.GetStream());

        pugi::xml_document doc;

        if (const pugi::xml_parse_result result = doc.load_string(xml_string.c_str()); !result)
            return false;

        const pugi::xml_node inventoryNode = doc.child("Inventory");
        if (!inventoryNode)
            return false;

        for (const auto & outfit_to_add : outfitsToAdd) {
            auto contextNode = inventoryNode.find_child_by_attribute("name", outfit_to_add->context.c_str());
            if (!contextNode) continue;
            auto typeNode = contextNode.child(outfit_to_add->type.c_str());
            if (!typeNode) continue;

            auto lastSetNode = typeNode.find_node([outfit_to_add] (const pugi::xml_node node) -> bool {
                return strcmp(node.text().get(), outfit_to_add->model.c_str()) == 0;
            });

            auto modelNode = typeNode.append_child("Model");
            modelNode.append_attribute("design_mode") = outfit_to_add->design_mode.c_str();
            modelNode.append_attribute("gender") = outfit_to_add->gender.c_str();
            modelNode.append_attribute("category") = outfit_to_add->category.c_str();
            modelNode.text() = outfit_to_add->model.c_str();

            if (lastSetNode != nullptr)
                typeNode.insert_move_after(modelNode, lastSetNode);
        }

        const auto str_writer = new xml_string_writer();

        doc.save(*str_writer);

        const auto out_stream = new EA::IO::MemoryStream(str_writer->result.data(), str_writer->result.size());

        asset.SetStream(out_stream);

        return true;
    }

    return false;
}

// TODO: does not yet work on x86, look into 004bb920, Revo::CharacterVisualComponent::BodyMaterialSetResourceAcquireCallback

void ClothingTweaker::CreateBodyMaterial(const uint32_t group, const std::string& type, const std::string &name, const std::string &texture, const int skin_id) {
    const std::string file_name_with_skin = name + "_s" + std::to_string(skin_id);

    const EA::ResourceMan::Key material_key = {
        .mInstance = Msml::Hash::FNV::FromString32(file_name_with_skin.c_str()),
        .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIAL),
        .mGroup = group,
    };

    const auto material_asset = new Msml::Core::Asset(material_key, Msml::Core::AssetType::kBuffer);
    const auto material_stream = new EA::IO::MemoryStream();
    material_stream->AddRef();

    auto material = MaterialBuilder()
        .WithShader(ShaderType::lambert)
        .WithKey(material_key)
        .WithColorParameter(0xDAA9532D, 0, 0, 0, 0)
        .WithColorParameter(0x3BD441A0,0,0,0,0)
        .WithColorParameter(0x988403F9,0,0,0,0)
        .WithColorParameter(0x5D22FD3,1,1,1,1)
        .WithValueParameter(0xF46B90AE, 0)
        .WithColorParameter(0x29BCDD1F,1,1,1,1)
        .WithColorParameter(0x73C9923E, 1,1,1,1)
        .WithColorParameter(0x4A5DAA3,0,0,0,0)
        .WithValueParameter(0x76F88689, 1)
        .WithColorParameter(0x7FEE2D1A,1,1,1,1)
        .WithKeyParameter(0x6CC0FD85, {
            .mInstance = Msml::Hash::FNV::FromString32(texture.c_str()),
            .mType = static_cast<uint32_t>(Msml::Core::FileType::DDS),
            .mGroup = 0,
        }).Build();

    material.Write(material_stream);
    material_asset->mBuffer = Msml::Core::Util::StreamUtil::ReadBytes(material_stream);
    material_stream->Close();
    material_stream->Release();

    const EA::ResourceMan::Key setKey {
        .mInstance = group,
        .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIALSET),
        .mGroup = Msml::Hash::FNV::FromString32(type.c_str()),
    };

    material_tweaker.AddMaterial(setKey, material_key);
    Msml::Core::Assets::GetInstance().RegisterAsset(material_asset);
}

/*
    idea for facial expression names
    _blink
    _annoyed
    _blush
    _serious
    _sad
    _glad
    _hurt
    _neutral
    _confused
 */

void ClothingTweaker::CreateEyesMaterials(const std::string& name, const std::string &base_texture) {
    const EA::ResourceMan::Key material_set_key = {
        .mInstance = Msml::Hash::FNV::FromString32(name.c_str()),
        .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIALSET),
        .mGroup = Msml::Hash::FNV::FromString32("Eyes")
    };

    auto material_set = MaterialSetBuilder()
        .WithKey(material_set_key)
        .WithMTSTName(Msml::Hash::FNV::FromString32(name.c_str()))
        .WithMTSTDefaultIndex();

    for (int i = 0; i <= 8; i++) {
        std::string key_name = name + std::to_string(i);
        std::string texture_name = base_texture + std::to_string(i);

        const EA::ResourceMan::Key material_key = {
            .mInstance = Msml::Hash::FNV::FromString32(key_name.c_str()),
            .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIAL),
            .mGroup = Msml::Hash::FNV::FromString32(name.c_str())
        };

        const auto material_asset = new Msml::Core::Asset(material_key, Msml::Core::AssetType::kBuffer);
        const auto material_stream = new EA::IO::MemoryStream();
        material_stream->AddRef();

        const EA::ResourceMan::Key texture_key = {
            .mInstance = Msml::Hash::FNV::FromString32(texture_name.c_str()),
            .mType = static_cast<uint32_t>(Msml::Core::FileType::DDS),
            .mGroup = 0,
        };

        auto material = MaterialBuilder()
            .WithShader(ShaderType::lambert)
            .WithKey(material_key)
            .WithColorParameter(0xDAA9532D, 0, 0, 0, 0)
            .WithColorParameter(0x3BD441A0,0,0,0,0)
            .WithColorParameter(0x5D22FD3,1,1,1,1)
            .WithValueParameter(0xF46B90AE, 0)
            .WithColorParameter(0x29BCDD1F,1,1,1,1)
            .WithKeyParameter(0xC3FAAC4F, texture_key)
            .WithColorParameter(0x73C9923E, 1,1,1,1)
            .WithColorParameter(0x4A5DAA3,0,0,0,0)
            .WithValueParameter(0x76F88689, 1)
            .WithColorParameter(0x7FEE2D1A,1,1,1,1)
            .WithKeyParameter(0x6CC0FD85, texture_key).Build();

        material_set.WithMaterial(material);

        material.Write(material_stream);
        material_asset->mBuffer = Msml::Core::Util::StreamUtil::ReadBytes(material_stream);
        material_stream->Close();
        material_stream->Release();

        Msml::Core::Assets::GetInstance().RegisterAsset(material_asset);
    }

    const auto material_set_asset = new Msml::Core::Asset(material_set_key, Msml::Core::AssetType::kBuffer);
    const auto material_set_stream = new EA::IO::MemoryStream();
    material_set_stream->AddRef();

    material_set.Build().Write(material_set_stream);
    material_set_asset->mBuffer = Msml::Core::Util::StreamUtil::ReadBytes(material_set_stream);
    material_set_stream->Close();
    material_set_stream->Release();

    Msml::Core::Assets::GetInstance().RegisterAsset(material_set_asset);
}

bool ClothingTweaker::OnRegister(Msml::Core::Asset &asset) {
    if (asset.mPath.string().ends_with(".clothing.xml")) {
        MSML_LOG_INFO("Registering clothing: %s", asset.mPath.string().c_str());
        pugi::xml_document doc;

        if (const pugi::xml_parse_result result = doc.load_file(asset.mPath.c_str()); !result)
            return false;

        const pugi::xml_node outfitNode = doc.child("Clothing");
        if (!outfitNode)
            return false;

        std::string name = outfitNode.attribute("name").as_string();
        std::string category = outfitNode.attribute("category").as_string();
        std::string gender = outfitNode.attribute("gender").as_string();
        std::string type = outfitNode.attribute("type").as_string();
        std::string model;
        std::string design_mode;

        std::vector<std::string> contexts;
        for (const auto & node : outfitNode.child("Contexts").children("Context")) {
            contexts.emplace_back(node.text().as_string());
        }

        if (type == "Body") {
            model = outfitNode.child("Model").text().as_string();

            const auto texture_node = outfitNode.child("Texture");
            std::string texture = texture_node.text().as_string();
            std::string mask = texture_node.attribute("mask").as_string();

            design_mode = "_" + name;

            std::string file_name = model + design_mode;
            std::string mask_name = file_name + "_mask";

            uint32_t model_instance = Msml::Hash::FNV::FromString32(model.c_str());

#ifdef VERSION_TACO_BELL
            MSML_LOG_ERROR("Clothing tweaking is not yet supported for your game version");
            return false;
            // EA::ResourceMan::IRecord* texture_record;
            // msml::core::Assets::GetAsset(texture + ".dds", &texture_record);
            // EA::ResourceMan::IRecord* mask_record;
            // msml::core::Assets::GetAsset(mask + ".dds", &mask_record);
            //
            // if (SkintoneVariantGenerator::Generate(file_name, texture_record, mask_record)){
            //     CreateMaterial(model_instance, type, file_name, file_name + "_s0", 0);
            //     CreateMaterial(model_instance, type, file_name, file_name + "_s1", 1);
            //     CreateMaterial(model_instance, type, file_name, file_name + "_s2", 2);
            // }
#endif

#ifdef VERSION_COZY_BUNDLE

            CreateBodyMaterial(model_instance, type, file_name, texture, 0);

            {
                const EA::ResourceMan::Key mask_key = {
                    .instance = msml::hash::fnv::FromString32(mask_name.c_str()),
                    .type = msml::core::assets::DDFFileType::DDS,
                    .group = 0,
                };

                auto mask_asset = new msml::core::assets::Asset(mask_key, msml::core::assets::REDIRECT);
                mask_asset->key_redirect = {
                    .instance = msml::hash::fnv::FromString32(mask.c_str()),
                    .type = msml::core::assets::DDFFileType::DDS,
                    .group = 0,
                };

                msml::core::Assets::GetInstance().RegisterAsset(mask_asset);
            }
#endif
        }
        else if (type == "Eyes" || type == "Mouth") {
            model = name;

            std::string texture = outfitNode.child("Texture").text().as_string();

            design_mode = "NA";

            CreateEyesMaterials(name, texture);
        }

        MSML_LOG_INFO("Created materials for %s", name.c_str());

        for (const std::string & context : contexts) {
            outfitsToAdd.push_back(new OutfitEntry{
                .context = context,
                .type = type,
                .design_mode = design_mode,
                .gender = gender,
                .category = category,
                .model = model
            });
        }

        return true;
    }

    return false;
}
