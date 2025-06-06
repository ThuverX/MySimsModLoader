//
// Created by exozg on 24/01/2025.
//

#include "Assets.h"

#include <iostream>
#include <ranges>

#include "Asset.h"
#include "../../tweakers/Tweaker.h"
#include "../hash/FNV.h"
#include "../signatures/sigdef.h"
#include "../hooks/Config.h"
#include "../modloader/ModLoader.h"
#include "../modloader/Mods.h"
#include "../resource/IdResolver.h"

namespace msml::core {
    Assets &Assets::GetInstance() {
        static Assets instance;
        return instance;
    }

    void * __fastcall AddFileHooked(EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *this_ptr CATCH_EDX,
                                    const EA::ResourceMan::Key &key, const wchar_t *path,
                                    const wchar_t *name) {
        const std::filesystem::path asset_path = ModLoader::GetInstance().modulePath / DATA_PATH / path;
        const auto filename = asset_path.stem().string();

        Assets::GetInstance().ddf_paths[key] = asset_path;

        resource::IdResolver::GetInstance().Add(filename);

        return EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Original(this_ptr, key, path, name);
    }

    void __fastcall ResourceSystemInitHooked(Revo::ResourceSystem::ResourceSystem *this_ptr CATCH_EDX) {
        EA::ResourceMan::Manager::RegisterDatabase(reinterpret_cast<EA::ResourceMan::Manager::Manager *>(this_ptr),
                                                   true,
                                                   Assets::GetInstance().database, 1000);

        Revo::ResourceSystem::InitHook.Original(this_ptr);
    }

    bool __fastcall DatabaseDirectoryFilesOpenRecordHooked(
        EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *this_ptr CATCH_EDX,
        const EA::ResourceMan::Key &key,
        EA::ResourceMan::IRecord **pDstRecord,
        EA::IO::AccessFlags accessFlags, EA::IO::CD cd, int a, EA::ResourceMan::RecordInfo *record_info) {
        // bad ddf, dont care
        return false;
    }

    bool __fastcall DatabasePackedFileOpenRecordHooked(
        EA::ResourceMan::DatabasePackedFile::DatabasePackedFile *this_ptr CATCH_EDX,
        const EA::ResourceMan::Key &key,
        EA::ResourceMan::IRecord **pDstRecord,
        EA::IO::AccessFlags accessFlags, EA::IO::CD cd, int a, EA::ResourceMan::RecordInfo *record_info) {
        // bad dbpf, dont care
        return false;
    }

    // void __fastcall LoadAllObjectDefsHooked(Revo::ObjectFolder::ObjectFolder *this_ptr CATCH_ECX) {
    //     MSML_LOG_INFO("LoadAllObjectDefs");
    //
    //     Revo::ObjectFolder::LoadAllObjectDefsHook.Original(this_ptr);
    //
    //     char buffer[MAX_PATH];
    //     GetCurrentDirectoryA(MAX_PATH, buffer);
    //
    //     SetCurrentDirectoryA("C:/Program Files (x86)/Steam/steamapps/common/MySims/mods/testmod/assets/");
    //     Revo::ObjectFolder::AddCharacterDef("CharacterDefs/GuyDef.xml", true);
    //     SetCurrentDirectoryA(buffer);
    // }
    //
    // Revo::ObjectFolder::CharacterDef * __fastcall AddCharacterDef(char *name, bool a) {
    //     auto ret = Revo::ObjectFolder::AddCharacterDefHook.Original(name, a);
    //
    //     char buffer[MAX_PATH];
    //     GetCurrentDirectoryA(MAX_PATH, buffer);
    //     MSML_LOG_INFO("%s / %s %p", buffer, name, ret);
    //
    //     return ret;
    // }

    bool __fastcall DatabasePackedFileOpenHooked(
        EA::ResourceMan::DatabasePackedFile::DatabasePackedFile *this_ptr CATCH_EDX, EA::IO::AccessFlags access_flags,
        EA::IO::CD creation_disposition, bool a, bool b) {
        const auto didOpen = EA::ResourceMan::DatabasePackedFile::OpenHook.Original(
            this_ptr, access_flags, creation_disposition, a, b);

        if (didOpen) {
            EA::ResourceMan::PFIndexModifiable::PFIndexModifiable *pfIndex =
                    EA::ResourceMan::DatabasePackedFile::GetIndex(this_ptr);

            const std::filesystem::path db_path =
                    ModLoader::GetInstance().modulePath / DATA_PATH / EA::ResourceMan::DatabasePackedFile::GetLocation(
                        this_ptr);

            // TODO: we might lose these if the game unloads them...
            for (auto element: pfIndex->itemMap) {
                Assets::GetInstance().dbpf_items[element.first] = {
                    db_path,
                    element.second,
                };
            }
        }

        return didOpen;
    }

    wchar_t *CharToWChar(const char *input) {
        if (!input) return nullptr;

        size_t len = std::mbstowcs(nullptr, input, 0);
        if (len == static_cast<size_t>(-1)) return nullptr;

        auto *wstr = new wchar_t[len + 1];
        std::mbstowcs(wstr, input, len + 1);
        return wstr;
    }

    Revo::App::TinyXmlInstance * ReadXMLFromPathHooked(Revo::App::TinyXmlInstance *xml_instance, const char *filename, void *document,
                                            const char *rootType, bool isGameFile, double *e, const char *folder) {

        const auto key = assets::Asset::GetKey(filename);

        if (!isGameFile || key.type == assets::DDFFileType::UNKNOWN)
            return Revo::App::ReadXMLFromPathHook.Original(xml_instance, filename, document, rootType, isGameFile, e,
                                                           folder);

        const auto wide_filename = CharToWChar(filename);
        const auto wide_folder = CharToWChar(folder);

        wchar_t real_path[MAX_PATH];
        Revo::App::GetCorrectLoadFolder(real_path, isGameFile, wide_folder, wide_filename);

        delete[] wide_filename;
        delete[] wide_folder;

        const auto record = new resource::CustomRecord(key, new EA::IO::FileStream(real_path),
                                                       Assets::GetInstance().database);

        Tweaker::RegistryOnLoad(record);

        const auto ret = Revo::App::ReadXMLFromStream(filename, record->stream, document, rootType, e);

        xml_instance->tinyXmlImplementation = ret->tinyXmlImplementation;
        xml_instance->tiXmlElement = ret->tiXmlElement;

        return ret;
    }

#ifdef VERSION_COZY_BUNDLE
    void *load_body(void *a, char *dynamic_skin_name, void *c, EA::ResourceMan::IResource *material_resource,
                    EA::ResourceMan::IResource *texture_resource, EA::ResourceMan::IResource *mask_resource) {
        const auto manager = EA::ResourceMan::Manager::GetManager();

        if (material_resource == nullptr) {
            const EA::ResourceMan::Key key = {
                .instance = hash::fnv::FromString64("fallback"),
                .type = assets::MATERIAL,
                .group = 0
            };

            EA::ResourceMan::Manager::GetResource(manager, key, &material_resource, nullptr, nullptr, nullptr, nullptr,
                                                  0, 0, 0, 0);
        }

        if (texture_resource == nullptr) {
            const EA::ResourceMan::Key key = {
                .instance = hash::fnv::FromString32("fallback"),
                .type = assets::DDS,
                .group = 0
            };

            EA::ResourceMan::Manager::GetResource(manager, key, &texture_resource, nullptr, nullptr, nullptr, nullptr,
                                                  0, 0, 0, 0);
        }

        if (mask_resource == nullptr) {
            const EA::ResourceMan::Key key = {
                .instance = hash::fnv::FromString32("empty"),
                .type = assets::DDS,
                .group = 0
            };

            EA::ResourceMan::Manager::GetResource(manager, key, &mask_resource, nullptr, nullptr, nullptr, nullptr, 0,
                                                  0, 0, 0);
        }

        return Revo::load_bodyHook.Original(a, dynamic_skin_name, c, material_resource, texture_resource,
                                            mask_resource);
    }
#endif
    void Assets::Install() {
        EA::ResourceMan::DatabaseDirectoryFiles::OpenRecordHook.Install(&DatabaseDirectoryFilesOpenRecordHooked);
        EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Install(&AddFileHooked);
        EA::ResourceMan::DatabasePackedFile::OpenRecordHook.Install(&DatabasePackedFileOpenRecordHooked);
        EA::ResourceMan::DatabasePackedFile::OpenHook.Install(&DatabasePackedFileOpenHooked);

        Revo::ResourceSystem::InitHook.Install(&ResourceSystemInitHooked);
        Revo::App::ReadXMLFromPathHook.Install(&ReadXMLFromPathHooked);
#ifdef VERSION_COZY_BUNDLE
        Revo::load_bodyHook.Install(&load_body);
#endif
    }

    void Assets::CreateDatabaseEntries(const std::filesystem::path &path) const {
        for (const auto &entry: std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                RegisterAsset(entry.path());
            }
        }
    }

    void Assets::RegisterAsset(const std::filesystem::path &path) const {
        const EA::ResourceMan::Key key = assets::Asset::GetKey(path);

        // Lua files with the same name as game ones will override them. Causing the game not to load
        // we want to discourage doing this and using hooks instead.
        if (key.type == assets::DDFFileType::LUA) return;

        const auto filename = path.stem().string();

        resource::IdResolver::GetInstance().Add(filename);

        const auto asset = new assets::Asset(key, assets::PATH);
        asset->path = path;

        if (database != nullptr)
            database->AddAsset(asset);
    }

    void Assets::RegisterAsset(assets::Asset *asset) const {
        if (asset->key.type == assets::DDFFileType::LUA) return;

        const auto filename = asset->path.stem().string();

        resource::IdResolver::GetInstance().Add(filename);

        if (database != nullptr)
            database->AddAsset(asset);
    }

    bool Assets::GetAsset(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord ** record) {
        if (GetInstance().database == nullptr) return false;
        return GetInstance().database->OpenRecord2(key, record, EA::IO::AccessFlags::Read, EA::IO::CD::LoadAllFiles, 0, nullptr);
    }

    bool Assets::GetAsset(const std::string& name, EA::ResourceMan::IRecord ** record) {
        if (GetInstance().database == nullptr) return false;
        return GetInstance().database->OpenRecord2(assets::Asset::GetKey(name), record, EA::IO::AccessFlags::Read, EA::IO::CD::LoadAllFiles, 0, nullptr);
    }

    void Assets::CreateDatabase() {
        database = new resource::CustomDatabase;

        if (modloader::Mods::GetInstance().mods.empty()) return;
        for (const auto &mod: modloader::Mods::GetInstance().mods) {
            std::filesystem::path mod_path(mod->path);

            CreateDatabaseEntries(mod_path);

            for (const auto &entry: std::filesystem::recursive_directory_iterator(mod_path)) {
                if (is_directory(entry.path())) {
                    CreateDatabaseEntries(entry.path());
                }
            }
        }
        for (const auto &val: database->assets | std::views::values) {
            Tweaker::RegistryOnRegister(val);
        }

        MSML_LOG_INFO("Database created");
    }
}
