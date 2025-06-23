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
#include "../../core/system/Logger.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../formats/builders/MaterialBuilder.h"

struct XmlStringWriter : pugi::xml_writer {
    std::string mResult;

    void write(const void *pData, const size_t size) override {
        mResult.append(static_cast<const char *>(pData), size);
    }
};

const EA::ResourceMan::Key kMasterInventoryKey = {
    .mInstance = Msml::Hash::FNV::FromString64("CAS.inventory"),
    .mType = static_cast<uint32_t>(Msml::Core::FileType::XML),
    .mGroup = 0,
};

bool ClothingTweaker::OnLoad(Msml::Core::Resource::CustomRecord &asset) {
    if (asset.mKey == kMasterInventoryKey) {
        MSML_LOG_INFO("Patching Master Inventory");

        const std::string kXmlString = Msml::Core::Util::StreamUtil::ReadString(asset.GetStream());

        pugi::xml_document doc;

        if (const pugi::xml_parse_result kResult = doc.load_string(kXmlString.c_str()); !kResult) {
            return false;
        }

        const pugi::xml_node kInventoryNode = doc.child("Inventory");
        if (!kInventoryNode) {
            return false;
        }

        for (const auto &outfitToAdd: mOutfitsToAdd) {
            auto contextNode = kInventoryNode.find_child_by_attribute("name", outfitToAdd->mContext.c_str());
            if (!contextNode) {
                continue;
            }
            auto typeNode = contextNode.child(outfitToAdd->mType.c_str());
            if (!typeNode) {
                continue;
            }

            auto lastSetNode = typeNode.find_node([outfitToAdd](const pugi::xml_node kNode) -> bool {
                return strcmp(kNode.text().get(), outfitToAdd->mModel.c_str()) == 0;
            });

            auto modelNode = typeNode.append_child("Model");
            modelNode.append_attribute("design_mode") = outfitToAdd->mDesignMode.c_str();
            modelNode.append_attribute("gender") = outfitToAdd->mGender.c_str();
            modelNode.append_attribute("category") = outfitToAdd->mCategory.c_str();
            modelNode.text() = outfitToAdd->mModel.c_str();

            if (lastSetNode != nullptr) {
                typeNode.insert_move_after(modelNode, lastSetNode);
            }
        }

        auto *const kStrWriter = new XmlStringWriter;

        doc.save(*kStrWriter);

        auto *const kOutStream = new EA::IO::MemoryStream(kStrWriter->mResult.data(), kStrWriter->mResult.size());

        asset.SetStream(kOutStream);

        return true;
    }

    return false;
}

// TODO: does not yet work on x86, look into 004bb920, Revo::CharacterVisualComponent::BodyMaterialSetResourceAcquireCallback

void ClothingTweaker::CreateMaterial(const uint32_t kGroup, const std::string &kType, const std::string &kName,
                                     const std::string &kTexture, int kSkinId) {
    const std::string kFileNameWithSkin = kName + "_s" + std::to_string(kSkinId);

    const EA::ResourceMan::Key kMaterialKey = {
        .mInstance = Msml::Hash::FNV::FromString32(kFileNameWithSkin),
        .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIAL),
        .mGroup = kGroup,
    };

    auto *const kMaterialAsset = new Msml::Core::Asset(kMaterialKey, Msml::Core::AssetType::kBuffer);
    auto *const kMaterialStream = new EA::IO::MemoryStream();
    kMaterialStream->AddRef();

    auto material = MaterialBuilder()
            .WithShader(ShaderType::lambert)
            .WithKey(kMaterialKey)
            .WithColorParameter(0xDAA9532D, 0, 0, 0, 0)
            .WithColorParameter(0x3BD441A0, 0, 0, 0, 0)
            .WithColorParameter(0x988403F9, 0, 0, 0, 0)
            .WithColorParameter(0x5D22FD3, 1, 1, 1, 1)
            .WithValueParameter(0xF46B90AE, 0)
            .WithColorParameter(0x29BCDD1F, 1, 1, 1, 1)
            .WithColorParameter(0x73C9923E, 1, 1, 1, 1)
            .WithColorParameter(0x4A5DAA3, 0, 0, 0, 0)
            .WithValueParameter(0x76F88689, 1)
            .WithColorParameter(0x7FEE2D1A, 1, 1, 1, 1)
            .WithKeyParameter(0x6CC0FD85, {
                                  .mInstance = Msml::Hash::FNV::FromString32(kTexture),
                                  .mType = static_cast<uint32_t>(Msml::Core::FileType::DDS),
                                  .mGroup = 0,
                              }).Build();

    material.Write(kMaterialStream);
    kMaterialAsset->mBuffer = Msml::Core::Util::StreamUtil::ReadBytes(kMaterialStream);
    kMaterialStream->Close();
    kMaterialStream->Release();

    const EA::ResourceMan::Key kSetKey{
        .mInstance = kGroup,
        .mType = static_cast<uint32_t>(Msml::Core::FileType::MATERIALSET),
        .mGroup = Msml::Hash::FNV::FromString32(kType),
    };

    material_tweaker.AddMaterial(kSetKey, kMaterialKey);
    Msml::Core::Assets::GetInstance().RegisterAsset(kMaterialAsset);
}

bool ClothingTweaker::OnRegister(Msml::Core::Asset &asset) {
    if (asset.mPath.string().ends_with(".outfit.xml")) {
        MSML_LOG_INFO("Registering clothing outfit: %s", asset.mPath.string().c_str());
        pugi::xml_document doc;

        if (const pugi::xml_parse_result kResult = doc.load_file(asset.mPath.c_str()); !kResult) {
            return false;
        }

        const pugi::xml_node kOutfitNode = doc.child("Outfit");
        if (!kOutfitNode) {
            return false;
        }

        std::string name = kOutfitNode.child("Name").text().as_string();
        std::string model = kOutfitNode.child("Model").text().as_string();
        std::string category = kOutfitNode.attribute("category").as_string();
        std::string type = kOutfitNode.attribute("type").as_string();
        std::string gender = kOutfitNode.attribute("gender").as_string();
        std::string texture = kOutfitNode.attribute("texture").as_string();
        std::string mask = kOutfitNode.attribute("mask").as_string();
        std::vector<std::string> contexts;

        for (const auto &node: kOutfitNode.child("Contexts").children("Context")) {
            contexts.emplace_back(node.text().as_string());
        }

        std::string designMode = "_" + name;

        std::string fileName = model + designMode;
        std::string maskName = fileName + "_mask";

        uint32_t modelInstance = Msml::Hash::FNV::FromString32(model);

#ifdef PLATFORM_WIN32
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

#ifdef PLATFORM_WIN64

        CreateMaterial(modelInstance, type, fileName, texture, 0); {
            const EA::ResourceMan::Key kMaskKey = {
                .mInstance = Msml::Hash::FNV::FromString32(maskName),
                .mType = static_cast<uint32_t>(Msml::Core::FileType::DDS),
                .mGroup = 0,
            };

            auto *maskAsset = new Msml::Core::Asset(kMaskKey, Msml::Core::AssetType::kRedirect);
            maskAsset->mKeyRedirect = {
                .mInstance = Msml::Hash::FNV::FromString32(mask),
                .mType = static_cast<uint32_t>(Msml::Core::FileType::DDS),
                .mGroup = 0,
            };

            Msml::Core::Assets::GetInstance().RegisterAsset(maskAsset);
        }
#endif

        MSML_LOG_INFO("Created materials for %s", fileName.c_str());

        for (const std::string &context: contexts) {
            mOutfitsToAdd.push_back(new OutfitEntry{
                .mContext = context,
                .mType = type,
                .mDesignMode = designMode,
                .mGender = gender,
                .mCategory = category,
                .mModel = model
            });
        }

        return true;
    }

    return false;
}
