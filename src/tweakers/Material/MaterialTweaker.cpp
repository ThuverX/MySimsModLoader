//
// Created by exozg on 30/05/2025.
//

#include "MaterialTweaker.h"

#include "pugixml.hpp"
#include "../../core/assets/Assets.h"
#include "../../core/hash/FNV.h"
#include "../../core/resource/IdResolver.h"
#include "../../core/system/Logger.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../formats/builders/MaterialBuilder.h"
#include "../../formats/materials/MaterialSet.h"

bool MaterialTweaker::OnLoad(Msml::Core::Resource::CustomRecord &asset) {
    if (mMaterialsToAddToSets.contains(asset.mKey)) {
        const uint32_t kMaterialCount = mMaterialsToAddToSets[asset.mKey].size();
        if (kMaterialCount <= 0) {
            return false;
        }
        MSML_LOG_INFO("Patching %s with %d material(s)",
                      Msml::Core::Resource::IdResolver::ToFilename(asset.mKey).c_str(),
                      kMaterialCount);

        MaterialSet matSet;
        auto *const kStream = asset.GetStream();
        kStream->AddRef();

        MaterialSet::Read(matSet, kStream);

        kStream->Release();

        for (const auto &value: mMaterialsToAddToSets[asset.mKey]) {
            MSML_LOG_DEBUG("\tAdding %s to %s", Msml::Core::Resource::IdResolver::ToFilename(value).c_str(),
                           Msml::Core::Resource::IdResolver::ToFilename(asset.mKey).c_str());
            matSet.mMaterials.emplace_back(value);
            matSet.mMtst.mIndices.emplace_back(matSet.mMtst.mIndices.size() + matSet.mMtst.mIndex + 1);
        }

        auto *const kOutStream = new EA::IO::MemoryStream;
        matSet.Write(kOutStream);
        // yeah this is stupid, and not having this baked in,
        // is a mistake waiting to happen
        kOutStream->SetPosition(0, EA::IO::PositionType::kBegin);

        asset.SetStream(kOutStream);

        return true;
    }

    return false;
}

bool MaterialTweaker::OnRegister(Msml::Core::Asset &asset) {
    if (asset.mPath.string().ends_with(".material.xml")) {
        MSML_LOG_INFO("Loading material from %s", asset.mPath.filename().string().c_str());
        const auto kFilename = asset.mPath.stem();
        const EA::ResourceMan::Key kKey = Msml::Core::Asset::GetKey(kFilename);
        auto materialBuilder = MaterialBuilder()
                .WithKey(kKey);

        const std::string kXmlString = Msml::Core::Util::StreamUtil::ReadString(asset.GetStream());

        pugi::xml_document doc;

        if (const pugi::xml_parse_result kResult = doc.load_string(kXmlString.c_str()); !kResult) {
            return false;
        }

        const pugi::xml_node kMaterialNode = doc.child("Material");
        if (!kMaterialNode) {
            return false;
        }

        const std::string kShader = kMaterialNode.attribute("shader").as_string();
        materialBuilder.WithShader(GetShaderType(kShader));

        for (const auto &node: kMaterialNode.children("Parameter")) {
            std::string type = node.attribute("type").as_string();
            std::string name = node.attribute("name").as_string();

            uint32_t nameId = 0;

            if (name.starts_with("0x")) {
                nameId = std::stoul(name, nullptr, 0);
            } else {
                nameId = Msml::Hash::FNV::FromString32(name);
            }

            if (type == "map") {
                const EA::ResourceMan::Key kMapKey = Msml::Core::Asset::GetKey(node.text().as_string());
                materialBuilder.WithKeyParameter(nameId, kMapKey);
            } else if (type == "color") {
                const float r = node.attribute("r").as_float();
                const float g = node.attribute("g").as_float();
                const float b = node.attribute("b").as_float();
                const float a = node.attribute("a").as_float();
                materialBuilder.WithColorParameter(nameId, r, g, b, a);
            } else if (type == "int") {
                const uint32_t kInteger = std::stoul(node.text().as_string(), nullptr, 0);
                materialBuilder.WithValueParameter(nameId, kInteger);
            }
        }

        auto material = materialBuilder.Build();
        auto *const kOutAsset = new Msml::Core::Asset(kKey, Msml::Core::AssetType::kBuffer);
        auto *const kOutStream = new EA::IO::MemoryStream();
        kOutStream->AddRef();

        material.Write(kOutStream);

        kOutAsset->mBuffer = Msml::Core::Util::StreamUtil::ReadBytes(kOutStream);
        kOutStream->Release();

        Msml::Core::Assets::GetInstance().RegisterAsset(kOutAsset);

        return true;
    }

    return false;
}

void MaterialTweaker::AddMaterial(const EA::ResourceMan::Key &kSetKey, const EA::ResourceMan::Key &kMaterialKey) {
    if (!mMaterialsToAddToSets.contains(kSetKey)) {
        mMaterialsToAddToSets[kSetKey] = {};
    }

    mMaterialsToAddToSets[kSetKey].push_back(kMaterialKey);
}
