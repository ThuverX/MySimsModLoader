//
// Created by exozg on 24/01/2025.
//

#include "Mods.h"

#include <filesystem>
#include <utility>
#include <algorithm>

#include "../modloader/ModLoader.h"
#include "../system/Logger.h"

namespace msml::core::modloader {
    Mods &Mods::GetInstance() {
        auto absolutePath = absolute(
            ModLoader::GetInstance().modulePath / MODS_PATH).string();

        static Mods instance(absolutePath);
        return instance;
    }

    void Mods::Find() {
        MSML_LOG_INFO("Searching for mods in %s", rootPath.c_str());
        mods.clear();
        try {
            if (std::filesystem::exists(rootPath)) {
                for (const auto &entry: std::filesystem::directory_iterator(rootPath)) {
                    if (is_directory(entry.path()) && exists(entry.path() / "mod.xml")) {
                        if (Mod *mod = Mod::fromXML(entry.path())) {
                            mods.emplace_back(mod);
                        }
                    }
                }
            }
        } catch (const std::exception &e) {
            MSML_LOG_ERROR("Failed to find mods directory: %s", e.what());
        }
    }

    std::vector<Mod *> Mods::GetModsSorted() const {
        std::vector<Mod *> sortedMods = mods;
        std::ranges::sort(sortedMods, [](const Mod *a, const Mod *b) {
            return a->priority < b->priority;
        });
        return sortedMods;
    }

    void Mods::RunPostHooks() const {
        for (const auto &mod: GetModsSorted()) {
            mod->RunPostHooks();
        }
    }

    void Mods::RunPreHooks() const {
        for (const auto &mod: GetModsSorted()) {
            mod->RunPreHooks();
        }
    }

    Mods::Mods(std::string rootPath) {
        this->rootPath = std::move(rootPath);
    }
}
