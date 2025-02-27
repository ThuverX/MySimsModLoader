//
// Created by exozg on 24/01/2025.
//

#include "Mod.h"

#include "pugixml.hpp"
#include "../util/File.h"
#include <filesystem>
#include <regex>

#include "../assets/Asset.h"
#include "../assets/Assets.h"
#include "../hooks/LuaHook.h"
#include "../signatures/Signatures.h"
#include "../util/Logger.h"

Mod * Mod::fromXML(const std::string path) {
    const auto mod = new Mod;

    pugi::xml_document doc;

    if (pugi::xml_parse_result result = doc.load_file((path + "/mod.xml").c_str()); !result)
        return nullptr;

    pugi::xml_node modNode = doc.child("Mod");
    if (!modNode)
        return nullptr;

    std::string name = modNode.child("Name").text().as_string();
    std::string description = modNode.child("Description").text().as_string();
    std::string author = modNode.child("Author").text().as_string();

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

    if (!modNode.attribute("priority").empty())
        mod->priority = modNode.attribute("priority").as_int();

    mod->name = name;
    mod->description = description;
    mod->author = author;
    mod->path = path;

    pugi::xml_node assetsNode = modNode.child("Assets");

    uint32_t replacerCount = 0;

    if (assetsNode != nullptr) {
        if (assetsNode.attribute("path").empty()) {
            MSML_LOG_ERROR("Mod %s is missing <Assets path>", path);
            return nullptr;
        }

        mod->assetsPath = assetsNode.attribute("path").as_string();

        for (pugi::xml_node replacer: assetsNode.children("Replacer")) {
            std::string keyName = replacer.attribute("key").as_string();
            std::string typeName = replacer.attribute("type").as_string();
            std::string filePath = replacer.text().as_string();

            std::regex keyPattern(R"((0x[0-9A-Fa-f]+)[ !](0x[0-9A-Fa-f]+))");

            if (std::smatch matches; std::regex_match(keyName, matches, keyPattern) && matches.size() == 3) {
                uint32_t group = std::stoul(matches[1].str(), nullptr, 16);
                uint64_t instance = std::stoull(matches[2].str(), nullptr, 16);

                uint32_t type = Asset::GetFileType(typeName);

                EA::ResourceMan::Key key = {
                    instance, type, group
                };

                std::string fullpath = (std::filesystem::path(path) / std::filesystem::path(filePath)).string();

                Assets::GetInstance().RegisterReplacer(fullpath, &key);
                replacerCount++;
            }
        }
    }

    MSML_LOG_INFO("%s registered %d replacer(s)", mod->name.c_str(), replacerCount);

    uint32_t hookCount = 0;

    if (pugi::xml_node hooksNode = modNode.child("Hooks"); hooksNode != nullptr) {
        for (pugi::xml_node hook: hooksNode.children("Hook")) {
            std::string key = hook.attribute("key").as_string();
            std::string hookPath = hook.text().as_string();
            std::string fullpath = std::filesystem::absolute(std::filesystem::path(path) / std::filesystem::path(hookPath)).string();

            mod->postHooks.push_back({
                key, fullpath
            });
            hookCount++;
        }

        for (pugi::xml_node hook: hooksNode.children("Prehook")) {
            std::string key = hook.attribute("key").as_string();
            std::string hookPath = hook.text().as_string();
            std::string fullpath = std::filesystem::absolute(std::filesystem::path(path) / std::filesystem::path(hookPath)).string();

            mod->preHooks.push_back({
                key, fullpath
            });
            hookCount++;
        }
    }

    MSML_LOG_INFO("%s registered %d hook(s)", mod->name.c_str(), hookCount);

    return mod;
}

void Mod::RunPostHooks(const std::string& modulePath) const {
    for (const auto& hook: postHooks) {
        if (hook.nativeFile == modulePath || hook.nativeFile == "*") {
            LuaHook::GetInstance().Require(hook.hookFile);
        }
    }
}

void Mod::RunPreHooks(const std::string& modulePath) const {
    for (const auto& hook: preHooks) {
        if (hook.nativeFile == modulePath || hook.nativeFile == "*") {
            LuaHook::GetInstance().Require(hook.hookFile);
        }
    }
}
