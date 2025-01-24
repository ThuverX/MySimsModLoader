//
// Created by exozg on 24/01/2025.
//

#include "Mod.h"

#include "pugixml.hpp"
#include "../util/File.h"
#include <filesystem>

#include "../assets/Asset.h"
#include "../assets/Assets.h"
#include "../signatures/Signatures.h"
#include "../util/Logger.h"

Mod * Mod::fromXML(const std::string path) {
    const auto mod = new Mod;

    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file((path + "/mod.xml").c_str());
    if (!result)
        return nullptr;

    pugi::xml_node child = doc.child("Mod");
    if (!child)
        return nullptr;

    const char* name = child.child("Name").text().as_string();
    const char* description = child.child("Description").text().as_string();
    const char* author = child.child("Author").text().as_string();

    if (!name) {
        MSML_LOG_ERROR("Mod %s is missing <Name>", path);
        return nullptr;
    }

    if (!description) {
        MSML_LOG_ERROR("Mod %s is missing <Description>", path);
        return nullptr;
    }

    if (!author) {
        MSML_LOG_ERROR("Mod %s is missing <Author>", path);
        return nullptr;
    }

    mod->name = name;
    mod->description = description;
    mod->author = author;
    mod->path = path.c_str();

    std::string assetsPath = path + "/assets/";
    uint32_t assetCount = 0;

    try {
        if (std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(assetsPath)) {
                if (std::filesystem::is_regular_file(entry)) {
                    const auto& filePath = entry.path().string();

                    mod->assets.emplace_back(filePath.c_str());
                    assetCount++;
                }
            }
        }
    } catch (const std::exception& e) {
        MSML_LOG_ERROR("Failed load assets from directory %s: %s", assetsPath.c_str(), e.what());
    }

    MSML_LOG_INFO("%s registered %d asset(s)", mod->name.c_str(), assetCount);

    return mod;
}

void Mod::LoadAssets() {
    for (const auto& entry : assets) {
        const EA::ResourceMan::Key* key = Asset::FromAssetName(entry);

        Assets::GetInstance().RegisterAsset(entry, key);
    }
}

void Mod::LoadHooks() {
}
