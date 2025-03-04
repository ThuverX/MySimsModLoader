//
// Created by exozg on 24/01/2025.
//

#include "Mods.h"
#include <filesystem>
#include <utility>
#include <algorithm>

#include "../modloader/ModLoader.h"
#include "../util/Logger.h"

Mods & Mods::GetInstance() {
    auto absolutePath = std::filesystem::absolute(
        ModLoader::GetInstance().modulePath + MODS_PATH).string();

    static Mods instance(absolutePath);
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
                        if (Mod* mod = Mod::fromXML(entry.path().string())) {
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

std::vector<Mod *> Mods::GetModsSorted() const {
    std::vector<Mod*> sortedMods = mods;
    std::ranges::sort(sortedMods, [](const Mod* a, const Mod* b) {
        return a->priority < b->priority;
    });
    return sortedMods;
}

void Mods::RunPostHooks() const {
    for (const auto& mod : GetModsSorted()) {
        mod->RunPostHooks();
    }
}

void Mods::RunPreHooks() const {
    for (const auto& mod : GetModsSorted()) {
        mod->RunPreHooks();
    }
}


Mods::Mods(std::string rootPath) {
    this->rootPath = std::move(rootPath);
}
