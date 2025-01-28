//
// Created by exozg on 24/01/2025.
//

#include "Mods.h"
#include <filesystem>

#include "../modloader/ModLoader.h"
#include "../util/Logger.h"

Mods & Mods::GetInstance() {
    static Mods instance(MODS_PATH);
    return instance;
}

void Mods::Find() {
    MSML_LOG_INFO("Searching for mods in %s", rootPath.c_str());
    mods.clear();
    try {
        if (std::filesystem::exists(rootPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(rootPath)) {
                if (std::filesystem::is_directory(entry.path())) {
                    std::string modXmlPath = entry.path().string() + "/mod.xml";

                    if (std::filesystem::exists(modXmlPath)) {
                        Mod* mod = Mod::fromXML(entry.path().string());
                        if (mod) {
                            mods.emplace_back(mod);
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        MSML_LOG_ERROR("Failed to find mods directory: %s", e.what());
    }
}

void Mods::LoadAssets() {
    for (const auto& mod : mods) {
        mod->LoadAssets();
    }
}

void Mods::LoadHooks() {
}

Mods::Mods(std::string rootPath) {
    this->rootPath = rootPath;

}
