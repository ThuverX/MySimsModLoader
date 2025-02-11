//
// Created by exozg on 24/01/2025.
//

#include "Assets.h"

#include <iostream>
#include <utility>

#include "../modloader/ModLoader.h"
#include "../mods/Mods.h"
#include "../util/File.h"

Assets &Assets::GetInstance() {
    static Assets instance;
    return instance;
}

void * __fastcall AddFileHooked(void *this_ptr, void *_ECX, const EA::ResourceMan::Key *key, const wchar_t *path,
                                const wchar_t *name) {

    for (const auto asset : Assets::GetInstance().GetReplacersByPath(path)) {
        asset->selfKey = {
            key->instance,
            key->type,
            key->group,
        };
    }

    return EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Original(this_ptr, key, path, name);
}

void * __fastcall GetResourceSystemResourceHooked(void *this_ptr, void *_ECX, const EA::ResourceMan::Key &key,
                                                  void **outResource, void *b, void *database, void *factory,
                                                  const EA::ResourceMan::Key *key2, uint32_t f, uint32_t g, uint32_t h,
                                                  uint32_t i) {
    EA::ResourceMan::Key modKey = key;

    if (const Asset *asset = Assets::GetInstance().GetReplacerByKey(key.instance, key.group, key.type)) {
        modKey = {
            asset->selfKey.instance,
            asset->selfKey.type,
            asset->selfKey.group,
        };
    }

    return Revo::ResourceSystem::GetResourceHook.Original(this_ptr, modKey, outResource, b, database, factory, key2, f,
                                                          g, h, i);
}

void Assets::Install() {
    EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Install(&AddFileHooked);

    Revo::ResourceSystem::GetResourceHook.Install(&GetResourceSystemResourceHooked);
}

void Assets::RegisterReplacer(const std::string &path, const EA::ResourceMan::Key *key) {
    auto *asset = new Asset;
    asset->key = *key;
    asset->path = path;
    replacers.emplace_back(asset);
}

void Assets::CreateDatabase(void *manager) {
    for (const auto &mod: Mods::GetInstance().mods) {
        if (!mod->assetsPath.empty()) {
            std::filesystem::path mod_path(mod->path);
            std::filesystem::path assets_path(mod->assetsPath);
            void *databaseStruct = Rvl_Malloc(0x98, "ResourceHelper", 0, 0, nullptr, 0, 0x10);
            void *database = EA::ResourceMan::DatabaseDirectoryFiles::ctor(
                databaseStruct, (mod_path / assets_path).wstring().c_str());

            MSML_LOG_DEBUG("Creating database at %s", (mod_path / assets_path).string().c_str());

            EA::ResourceMan::Manager::RegisterDatabase(manager, true, database, 1000);
            EA::ResourceMan::DatabaseDirectoryFiles::Init(database);
            EA::ResourceMan::DatabaseDirectoryFiles::Open(database, IO::AccessFlags::Read, IO::CD::LoadAllFiles, false,
                                                          false);
        }
    }
}

Asset *Assets::GetReplacerByKey(const uint64_t instance, const uint32_t group, const uint32_t type) const {
    for (auto &asset: replacers) {
        if (asset->key.instance == instance &&
            asset->key.group == group &&
            asset->key.type == type) {
            return asset;
        }
    }
    return nullptr;
}

std::vector<Asset *>
Assets::GetReplacersByKey(const uint64_t instance, const uint32_t group, const uint32_t type) const {
    auto vec = std::vector<Asset *>();

    for (auto &asset: replacers) {
        if (asset->key.instance == instance &&
            asset->key.group == group &&
            asset->key.type == type) {
            vec.emplace_back(asset);
        }
    }

    return vec;
}

Asset *Assets::GetReplacerByPath(const std::wstring &path) const {
    const std::filesystem::path p(path);
    for (auto &asset: replacers) {
        if (asset->path == p) {
            return asset;
        }
    }
    return nullptr;
}

std::vector<Asset *> Assets::GetReplacersByPath(const std::wstring &path) const {
    auto vec = std::vector<Asset *>();
    const std::filesystem::path p(path);
    for (auto &asset: replacers) {
        if (asset->path == p) {
            vec.emplace_back(asset);
        }
    }
    return vec;
}
