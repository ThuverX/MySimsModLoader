//
// Created by exozg on 24/01/2025.
//

#include "Assets.h"

#include <iostream>
#include <utility>

#include "../modloader/ModLoader.h"
#include "../mods/Mods.h"

Assets &Assets::GetInstance() {
    static Assets instance;
    return instance;
}

void * __fastcall AddFileHooked(EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *this_ptr, void *_ECX, const EA::ResourceMan::Key *key, const wchar_t *path,
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

void * __fastcall GetResourceSystemResourceHooked(Revo::ResourceSystem::ResourceSystem *this_ptr, void *_ECX, const EA::ResourceMan::Key &key,
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

    // TODO: if a file is not found, respond with a debug version of the file. So the game doesn't crash
    // mostly needed for models, materials and textures

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

#pragma optimize("", off)
void Assets::CreateDatabase(EA::ResourceMan::Manager::Manager * manager) {
    for (const auto &mod: Mods::GetInstance().mods) {
        if (!mod->assetsPath.empty()) {
            std::filesystem::path mod_path(mod->path);
            std::filesystem::path assets_path(mod->assetsPath);

            auto *databaseStruct = static_cast<EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *>(Rvl_Malloc(
                0x98, "ResourceHelper", 0, 0, nullptr, 0, 0x10));
            auto *database = EA::ResourceMan::DatabaseDirectoryFiles::ctor(
                databaseStruct, (mod_path / assets_path).wstring().c_str());

            MSML_LOG_DEBUG("Creating database at %s", (mod_path / assets_path).string().c_str());

            EA::ResourceMan::Manager::RegisterDatabase(manager, true, database, 1000);
            EA::ResourceMan::DatabaseDirectoryFiles::Init(database);
            EA::ResourceMan::DatabaseDirectoryFiles::Open(database, IO::AccessFlags::Read, IO::CD::LoadAllFiles, false,
                                                          false);

            // Maybe list the keys out here? instead of doing the add file hook? This way we can also programmatically call upon files
        }
    }
}
#pragma optimize("", on)

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
