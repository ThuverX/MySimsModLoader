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

bool MaterialTweaker::OnLoad(msml::core::resource::CustomRecord &asset) {
    if (materialsToAddToSets.contains(asset.key)) {
        const uint32_t materialCount = materialsToAddToSets[asset.key].size();
        if (materialCount <= 0) return false;
        MSML_LOG_INFO("Patching %s with %d material(s)",
                      msml::core::resource::IdResolver::ToFilename(asset.key).c_str(),
                      materialCount);

        MaterialSet matSet;
        const auto stream = asset.GetStream();
        stream->AddRef();

        MaterialSet::Read(matSet, stream);

        stream->Release();

        for (const auto &value: materialsToAddToSets[asset.key]) {
            MSML_LOG_DEBUG("\tAdding %s to %s", msml::core::resource::IdResolver::ToFilename(value).c_str(),
                           msml::core::resource::IdResolver::ToFilename(asset.key).c_str());
            matSet.materials.emplace_back(value);
            matSet.mtst.indices.emplace_back(matSet.mtst.indices.size() + matSet.mtst.index + 1);
        }

        const auto out_stream = new EA::IO::MemoryStream;
        matSet.Write(out_stream);
        // yeah this is stupid, and not having this baked in,
        // is a mistake waiting to happen
        out_stream->SetPosition(0, EA::IO::PositionType::Begin);

        asset.SetStream(out_stream);

        return true;
    }

    return false;
}

bool MaterialTweaker::OnRegister(msml::core::assets::Asset &asset) {
    if (asset.path.string().ends_with(".material.xml")) {
        MSML_LOG_INFO("Loading material from %s", asset.path.filename().string().c_str());
        const auto filename = asset.path.stem();
        const EA::ResourceMan::Key key = msml::core::assets::Asset::GetKey(filename);
        auto materialBuilder = MaterialBuilder()
                .withKey(key);

        const std::string xml_string = msml::core::util::StreamUtil::ReadString(asset.GetStream());

        pugi::xml_document doc;

        if (const pugi::xml_parse_result result = doc.load_string(xml_string.c_str()); !result)
            return false;

        const pugi::xml_node materialNode = doc.child("Material");
        if (!materialNode)
            return false;

        std::string shader = materialNode.attribute("shader").as_string();
        materialBuilder.withShader(GetShaderType(shader));

        for (const auto &node: materialNode.children("Parameter")) {
            std::string type = node.attribute("type").as_string();
            std::string name = node.attribute("name").as_string();

            uint32_t name_id = 0;

            if (name.starts_with("0x")) {
                name_id = std::stoul(name, nullptr, 0);
            } else {
                name_id = msml::hash::fnv::FromString32(name.c_str());
            }

            if (type == "map") {
                EA::ResourceMan::Key mapKey = msml::core::assets::Asset::GetKey(node.text().as_string());
                materialBuilder.withKeyParameter(name_id, mapKey);
            } else if (type == "color") {
                float r = node.attribute("r").as_float();
                float g = node.attribute("g").as_float();
                float b = node.attribute("b").as_float();
                float a = node.attribute("a").as_float();
                materialBuilder.withColorParameter(name_id, r, g, b, a);
            } else if (type == "int") {
                uint32_t integer = std::stoul(node.text().as_string(), nullptr, 0);
                materialBuilder.withValueParameter(name_id, integer);
            }
        }

        auto material = materialBuilder.build();
        const auto out_asset = new msml::core::assets::Asset(key, msml::core::assets::BUFFER);
        const auto out_stream = new EA::IO::MemoryStream();
        out_stream->AddRef();

        material.Write(out_stream);

        out_asset->buffer = msml::core::util::StreamUtil::ReadBytes(out_stream);
        out_stream->Release();

        msml::core::Assets::GetInstance().RegisterAsset(out_asset);

        return true;
    }

    return false;
}

void MaterialTweaker::AddMaterial(const EA::ResourceMan::Key &setKey, const EA::ResourceMan::Key &materialKey) {
    if (!materialsToAddToSets.contains(setKey)) {
        materialsToAddToSets[setKey] = {};
    }

    materialsToAddToSets[setKey].push_back(materialKey);
}
