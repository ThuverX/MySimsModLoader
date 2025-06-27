//
// Created by exozg on 24/01/2025.
//

#include "Mods.h"

#include <filesystem>
#include <utility>
#include <algorithm>

#include "../modloader/ModLoader.h"
#include "../system/Logger.h"

namespace Msml::Core {
    Mods &Mods::GetInstance() {
        auto absolutePath = std::filesystem::absolute(
            ModLoader::GetInstance().mModulePath / MODS_PATH).string();

        static Mods instance(absolutePath);
        return instance;
    }

    void Mods::Find() {
        MSML_LOG_INFO("Searching for mods in %s", mRootPath.c_str());
        mMods.clear();
        try {
            if (std::filesystem::exists(mRootPath)) {
                for (const auto &entry: std::filesystem::directory_iterator(mRootPath)) {
                    if (is_directory(entry.path()) && exists(entry.path() / "mod.xml")) {
                        if (Mod *mod = Mod::FromXML(entry.path())) {
                            mMods.emplace_back(mod);
                        }
                    }
                }
            }
        } catch (const std::exception &e) {
            MSML_LOG_ERROR("Failed to find mods directory: %s", e.what());
        }
    }

    std::vector<Mod *> Mods::GetModsSorted() const {
        std::vector<Mod *> sortedMods = mMods;
        std::ranges::sort(sortedMods, [](const Mod *kModA, const Mod *kModB) {
            return kModA->mPriority < kModB->mPriority;
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

    Mods::Mods(std::string rootPath) : mRootPath(std::move(rootPath)) {}
}
