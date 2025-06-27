//
// Created by exozg on 24/01/2025.
//

#include "../../../libs/pugixml/src/pugixml.hpp"
#include <regex>

#include "Mod.h"

#include "../assets/Asset.h"
#include "../hooks/LuaHook.h"
#include "../system/Logger.h"

namespace Msml::Core {
    Mod * Mod::FromXML(const std::filesystem::path &path) {
        pugi::xml_document doc;

        if (const pugi::xml_parse_result kResult = doc.load_file((path / "mod.xml").c_str()); !kResult) {
            return nullptr;
        }

        const pugi::xml_node kModNode = doc.child("Mod");
        if (!kModNode) {
            return nullptr;
        }

        const std::string kName = kModNode.child("Name").text().as_string();
        const std::string kDescription = kModNode.child("Description").text().as_string();
        const std::string kAuthor = kModNode.child("Author").text().as_string();

        if (kName.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Name>", path);
            return nullptr;
        }

        if (kDescription.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Description>", path);
            return nullptr;
        }

        if (kAuthor.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Author>", path);
            return nullptr;
        }

        auto *const kMod = new Mod;

        if (!kModNode.attribute("priority").empty()) {
            kMod->mPriority = kModNode.attribute("priority").as_int();
        }

        kMod->mName = kName;
        kMod->mDescription = kDescription;
        kMod->mAuthor = kAuthor;
        kMod->mPath = path.string();

        uint32_t hookCount = 0;

        if (const pugi::xml_node kHooksNode = kModNode.child("Hooks"); kHooksNode != nullptr) {
            for (pugi::xml_node hook: kHooksNode.children("Hook")) {
                std::string hookPath = hook.text().as_string();
                std::string fullpath = absolute(path / hookPath).string();

                kMod->mPostHooks.push_back(fullpath);
                hookCount++;
            }

            for (pugi::xml_node hook: kHooksNode.children("Prehook")) {
                std::string hookPath = hook.text().as_string();
                std::string fullpath = std::filesystem::absolute(std::filesystem::path(path) / std::filesystem::path(hookPath)).string();

                kMod->mPreHooks.push_back(fullpath);
                hookCount++;
            }
        }

        MSML_LOG_INFO("%s registered %d hook(s)", kMod->mName.c_str(), hookCount);

        return kMod;
    }

    void Mod::RunPostHooks() const {
        for (const auto& hook: mPostHooks) {
            Hooks::LuaHook::Require(hook);
        }
    }

    void Mod::RunPreHooks() const {
        for (const auto& hook: mPreHooks) {
            Hooks::LuaHook::Require(hook);
        }
    }
}
