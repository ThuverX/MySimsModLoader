//
// Created by exozg on 24/01/2025.
//

#include "../../../libs/pugixml/src/pugixml.hpp"
#include <regex>

#include "Mod.h"

#include "../assets/Asset.h"
#include "../hooks/LuaHook.h"
#include "../system/Logger.h"

namespace msml::core::modloader {
    Mod * Mod::fromXML(const std::filesystem::path &path) {
        pugi::xml_document doc;

        if (const pugi::xml_parse_result result = doc.load_file((path / "mod.xml").c_str()); !result)
            return nullptr;

        const pugi::xml_node modNode = doc.child("Mod");
        if (!modNode)
            return nullptr;

        const std::string name = modNode.child("Name").text().as_string();
        const std::string description = modNode.child("Description").text().as_string();
        const std::string author = modNode.child("Author").text().as_string();

        if (name.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Name>", path);
            return nullptr;
        }

        if (description.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Description>", path);
            return nullptr;
        }

        if (author.empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Author>", path);
            return nullptr;
        }

        const auto mod = new Mod;

        if (!modNode.attribute("priority").empty())
            mod->priority = modNode.attribute("priority").as_int();

        mod->name = name;
        mod->description = description;
        mod->author = author;
        mod->path = path.string();

        uint32_t hookCount = 0;

        if (const pugi::xml_node hooksNode = modNode.child("Hooks"); hooksNode != nullptr) {
            for (pugi::xml_node hook: hooksNode.children("Hook")) {
                std::string hookPath = hook.text().as_string();
                std::string fullpath = absolute(path / hookPath).string();

                mod->postHooks.push_back(fullpath);
                hookCount++;
            }

            for (pugi::xml_node hook: hooksNode.children("Prehook")) {
                std::string hookPath = hook.text().as_string();
                std::string fullpath = std::filesystem::absolute(std::filesystem::path(path) / std::filesystem::path(hookPath)).string();

                mod->preHooks.push_back(fullpath);
                hookCount++;
            }
        }

        MSML_LOG_INFO("%s registered %d hook(s)", mod->name.c_str(), hookCount);

        return mod;
    }

    void Mod::RunPostHooks() const {
        for (const auto& hook: postHooks) {
            hooks::LuaHook::Require(hook);
        }
    }

    void Mod::RunPreHooks() const {
        for (const auto& hook: preHooks) {
            hooks::LuaHook::Require(hook);
        }
    }
}
