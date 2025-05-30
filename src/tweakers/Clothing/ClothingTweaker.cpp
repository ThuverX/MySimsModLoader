//
// Created by exozg on 17/05/2025.
//

#include "ClothingTweaker.h"

#include "pugixml.hpp"
#include "../../core/assets/Assets.h"
#include "../../core/hash/FNV.h"
#include "../../core/resource/IdResolver.h"
#include "../../core/system/Logger.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../formats/builders/MaterialBuilder.h"
#include "../../formats/materials/MaterialSet.h"

struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    void write(const void* data, size_t size) override {
        result.append(static_cast<const char*>(data), size);
    }
};

EA::ResourceMan::Key MasterInventoryKey = {
    .instance = msml::hash::fnv::FromString64("CAS.inventory"),
    .type = msml::core::assets::DDFFileType::XML,
    .group = 0,
};

bool ClothingTweaker::OnLoad(msml::core::resource::CustomRecord &asset) {
    if (materialsToAdd.contains(asset.key)) {
        const uint32_t materialCount = materialsToAdd[asset.key].size();
        MSML_LOG_INFO("Patching %s with %d materials", msml::core::resource::IdResolver::ToFilename(asset.key).c_str(), materialCount);

        MaterialSet matSet;
        const auto stream = asset.GetStream();
        stream->AddRef();

        MaterialSet::Read(matSet, stream);

        stream->Release();

        for (const auto & value : materialsToAdd[asset.key]) {
            matSet.materials.push_back(value);
        }

        const auto out_stream = new EA::IO::MemoryStream();
        matSet.Write(out_stream);

        asset.SetStream(out_stream);

        return true;
    }

    if (asset.key == MasterInventoryKey) {
        MSML_LOG_INFO("Patching Master Inventory");

        const auto stream = asset.GetStream();
        stream->AddRef();
        const auto n_size = stream->GetSize();
        std::string xml_string(n_size, '\0');
        stream->Read(xml_string.data(), n_size);
        stream->Release();

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

void ClothingTweaker::CreateMaterial(const uint32_t group, const std::string &name, const std::string &texture, const int skin_id) {
    const std::string file_name_with_skin = name + "_s" + std::to_string(skin_id);

    const EA::ResourceMan::Key material_key = {
        .instance = msml::hash::fnv::FromString32(file_name_with_skin.c_str()),
        .type = msml::core::assets::DDFFileType::MATERIAL,
        .group = group,
    };

    const auto material_asset = new msml::core::assets::Asset(material_key, msml::core::assets::BUFFER);
    const auto material_stream = new EA::IO::MemoryStream();
    material_stream->AddRef();

    auto material = MaterialBuilder()
        .withShader(ShaderType::lambert)
        .withKey(material_key)
        .withColorParameter(0xDAA9532D, 0, 0, 0, 0)
        .withColorParameter(0x3BD441A0,0,0,0,0)
        .withColorParameter(0x988403F9,0,0,0,0)
        .withColorParameter(0x5D22FD3,1,1,1,1)
        .withValueParameter(0xF46B90AE, 0)
        .withColorParameter(0x29BCDD1F,1,1,1,1)
        .withColorParameter(0x73C9923E, 1,1,1,1)
        .withColorParameter(0x4A5DAA3,0,0,0,0)
        .withValueParameter(0x76F88689, 1)
        .withColorParameter(0x7FEE2D1A,1,1,1,1)
        .withKeyParameter(0x6CC0FD85, {
            .instance = msml::hash::fnv::FromString32(texture.c_str()),
            .type = msml::core::assets::DDFFileType::DDS,
            .group = 0,
        }).build();

    material.Write(material_stream);
    material_asset->buffer = material_stream->AsBuffer();
    material_stream->Close();
    material_stream->Release();

    const EA::ResourceMan::Key setKey {
        .instance = group,
        .type = msml::core::assets::DDFFileType::MATERIALSET,
        .group = 0
    };

    if (!materialsToAdd.contains(setKey)) {
        materialsToAdd[setKey] = {};
    }

    materialsToAdd[setKey].push_back(material_key);

    msml::core::Assets::GetInstance().RegisterAsset(material_asset);
}

bool ClothingTweaker::OnRegister(msml::core::assets::Asset &asset) {
    if (asset.path.string().ends_with(".outfit.xml")) {
        MSML_LOG_INFO("Registering clothing outfit: %s", asset.path.string().c_str());
        pugi::xml_document doc;

        if (const pugi::xml_parse_result result = doc.load_file(asset.path.c_str()); !result)
            return false;

        const pugi::xml_node outfitNode = doc.child("Outfit");
        if (!outfitNode)
            return false;

        std::string name = outfitNode.attribute("name").as_string();
        // maybe we can make this a list, because you might want npc's to also wear the outfit
        std::string context = outfitNode.attribute("context").as_string();
        std::string model = outfitNode.attribute("model").as_string();
        std::string category = outfitNode.attribute("category").as_string();
        std::string type = outfitNode.attribute("type").as_string();
        std::string gender = outfitNode.attribute("gender").as_string();
        std::string texture = outfitNode.attribute("texture").as_string();
        std::string mask = outfitNode.attribute("mask").as_string();

        std::string design_mode = "_" + name;

        std::string file_name = model + design_mode;
        std::string mask_name = file_name + "_mask";

        uint32_t model_instance = msml::hash::fnv::FromString32(model.c_str());

#ifdef VERSION_TACO_BELL
        {
            // requires a lot of work, won't be doing this right now
        }
#endif

#ifdef VERSION_COZY_BUNDLE

        CreateMaterial(model_instance, file_name, texture, 0);

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

        outfitsToAdd.push_back(new OutfitEntry{
            .context = context,
            .type = type,
            .design_mode = design_mode,
            .gender = gender,
            .category = category,
            .model = model
        });

        // Make sure to also add to kMaster
        outfitsToAdd.push_back(new OutfitEntry{
            .context = "kMaster",
            .type = type,
            .design_mode = design_mode,
            .gender = gender,
            .category = category,
            .model = model
        });

        // maybe have a flag for this, if it can only be unlocked later
        outfitsToAdd.push_back(new OutfitEntry{
            .context = "kInitial",
            .type = type,
            .design_mode = design_mode,
            .gender = gender,
            .category = category,
            .model = model
        });

        return true;
    }

    return false;
}
