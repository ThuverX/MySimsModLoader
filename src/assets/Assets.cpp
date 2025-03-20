//
// Created by exozg on 24/01/2025.
//

#include "Assets.h"

#include <iostream>
#include <utility>

#include "../hashes/Hashes.h"
#include "../mods/Mods.h"
#include "../Common.h"

Assets &Assets::GetInstance() {
    static Assets instance;
    return instance;
}

void * __fastcall AddFileHooked(EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *this_ptr CATCH_ECX,
                                const EA::ResourceMan::Key *key, const wchar_t *path,
                                const wchar_t *name) {
    for (const auto asset: Assets::GetInstance().GetReplacersByPath(path)) {
        asset->selfKey = {
            key->instance,
            key->type,
            key->group,
        };
    }

    // MSML_LOG_DEBUG_HIDDEN("Loading %s", Hashes::ToHumanReadable(*key).c_str());

    return EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Original(this_ptr, key, path, name);
}

void * __fastcall GetResourceSystemResourceHooked(Revo::ResourceSystem::ResourceSystem *this_ptr CATCH_ECX,
                                                  const EA::ResourceMan::Key &key,
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

    const auto ret = Revo::ResourceSystem::GetResourceHook.Original(this_ptr, modKey, outResource, b, database, factory, key2, f,
                                                          g, h, i);
    // if (ret != nullptr) {
    //     MSML_LOG_DEBUG_HIDDEN("Got: %s", Hashes::ToHumanReadable(modKey).c_str());
    // }

    return ret;
}

// uint32_t __fastcall PFRecordReadRead(EA::ResourceMan::PFRecordRead::PFRecordRead* this_ptr, void* _ECX, void* data, uint32_t size) {
//     auto ret = EA::ResourceMan::PFRecordRead::ReadHook.Original(this_ptr, data, size);
//     auto* basePtr = reinterpret_cast<EA::ResourceMan::PFRecordBase::PFRecordBase *>(reinterpret_cast<uint32_t>(this_ptr) - 48);
//
//     MSML_LOG_DEBUG_HIDDEN("Reading %s", Hashes::ToHumanReadable(basePtr->key).c_str());
//
//     // TODO: How do we change this data?...
//     //
//     // if (basePtr->key.instance == SwarmTweaker::RootSwarmKey.instance &&
//     //         basePtr->key.group == SwarmTweaker::RootSwarmKey.group &&
//     //         basePtr->key.type == SwarmTweaker::RootSwarmKey.type) {
//     //     const auto strData = std::string(static_cast<char*>(data), size);
//     //     MSML_LOG_DEBUG("%s", strData.c_str());
//     // }
//
//     return ret;
// }

void __fastcall ResourceSystemInitHooked(Revo::ResourceSystem::ResourceSystem *this_ptr CATCH_ECX) {
    Assets::GetInstance().CreateDatabase(reinterpret_cast<EA::ResourceMan::Manager::Manager *>(this_ptr));
    Revo::ResourceSystem::InitHook.Original(this_ptr);
}

void Assets::Install() {
    EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Install(&AddFileHooked);
    Revo::ResourceSystem::GetResourceHook.Install(&GetResourceSystemResourceHooked);
    Revo::ResourceSystem::InitHook.Install(&ResourceSystemInitHooked);

    // EA::ResourceMan::PFRecordRead::ReadHook.Install(&PFRecordReadRead);
    // Revo::SwarmResourceFactory::CreateResourceHook.Install(&SwarmResourceFactoryCreateResource);
}

void Assets::RegisterReplacer(const std::string &path, const EA::ResourceMan::Key *key) {
    auto *asset = new Asset;
    asset->key = *key;
    asset->path = path;
    replacers.emplace_back(asset);
}

#ifdef _WIN64
constexpr size_t DATABASE_DIRECTORY_FILES_SIZE = 0xe8;
#else
constexpr size_t DATABASE_DIRECTORY_FILES_SIZE = 0x98;
#endif

void CreateDatabaseInternal(EA::ResourceMan::Manager::Manager *manager, const std::filesystem::path& path) {

    auto *databaseStruct = static_cast<EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *>(malloc(DATABASE_DIRECTORY_FILES_SIZE));
    auto *database = EA::ResourceMan::DatabaseDirectoryFiles::ctor(
        databaseStruct, path.wstring().c_str());

    MSML_LOG_DEBUG("Creating database at %s", path.string().c_str());

    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"model", 0x01661233);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"revomodel", 0xf9e50586);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"windowsmodel", 0xb359c791);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"rig", 0x8eaf13de);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"clip", 0x6b20c4f3);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"keynamemap", 0x0166038c);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"geometry", 0x015a1849);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"material", 0x01d0e75d);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"materialset", 0x02019972);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"oldspeedtree", 0x00b552ea);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"speedtree", 0x021d7e8c);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"dds", 0x00b2d882);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"compositetexture", 0x8e342417);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"simoutfit", 0x025ed6f4);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"levelxml", 0x585ee310);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"levelbin", 0x58969018);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"physics", 0xd5988020);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"luascript", 0x474999b4);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"lightsetxml", 0x50182640);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"lightsetbin", 0x50002128);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"xml", 0xdc37e964);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"footprintset", 0x2c81b60a);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"objectconstructionxml", 0xc876c85e);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"objectconstructionbin", 0xc08ec0ee);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"slotxml", 0x4045d294);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"slotbin", 0x487bf9e4);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"swm", 0xcf60795e);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"swarmbin", 0x9752e396);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"xmlbin", 0xe0d83029);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"cabxml", 0xa6856948);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"cabbin", 0xc644f440);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"big", 0x5bca8c06);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"bnk", 0xb6b5c271);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"lua", 0x474999b4);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"luo", 0x2b8e2411);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"lightboxxml", 0xb61215e9);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"lightboxbin", 0xd6215201);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"xmb", 0x1e1e6516);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"ttf", 0xfd72d418);
    EA::ResourceMan::DatabaseDirectoryFiles::AddExtensionMapping(database, L"ttc", 0x35ebb959);

    EA::ResourceMan::Manager::RegisterDatabase(manager, true, database, 1000);
    EA::ResourceMan::DatabaseDirectoryFiles::Init(database);
    EA::ResourceMan::DatabaseDirectoryFiles::Open(database, EA::IO::AccessFlags::Read, EA::IO::CD::LoadAllFiles, false,
                                                  false);

    // Maybe list the keys out here? instead of doing the add file hook? This way we can also programmatically call upon files
}

#pragma optimize("", off)
void Assets::CreateDatabase(EA::ResourceMan::Manager::Manager *manager) {
    if (Mods::GetInstance().mods.empty()) return;
    for (const auto &mod: Mods::GetInstance().mods) {
        if (!mod->assetsPath.empty()) {
            std::filesystem::path mod_path(mod->path);
            std::filesystem::path assets_path(mod->assetsPath);

            CreateDatabaseInternal(manager, mod_path / assets_path);

            for (const auto& entry : std::filesystem::directory_iterator(mod_path / assets_path)) {
                if (std::filesystem::is_directory(entry.path())) {
                    CreateDatabaseInternal(manager, entry.path());
                }
            }

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
