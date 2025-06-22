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

namespace Msml::Core {
    Assets &Assets::GetInstance() {
        static Assets instance;
        return instance;
    }

    void * __fastcall AddFileHooked(EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *thisPtr CATCH_EDX,
                                    const EA::ResourceMan::Key &key, const wchar_t *path,
                                    const wchar_t *name) {
        const std::filesystem::path kAssetPath = ModLoader::GetInstance().mModulePath / DATA_PATH / path;
        const auto kFilename = kAssetPath.stem().string();

        Assets::GetInstance().mDDFPaths[key] = kAssetPath;

        Resource::IdResolver::GetInstance().Add(kFilename);

        return EA::ResourceMan::DatabaseDirectoryFiles::AddFileHook.Original(thisPtr, key, path, name);
    }

    void __fastcall ResourceSystemInitHooked(Revo::ResourceSystem::ResourceSystem *thisPtr CATCH_EDX) {
        EA::ResourceMan::Manager::RegisterDatabase(reinterpret_cast<EA::ResourceMan::Manager::Manager *>(thisPtr),
                                                   true,
                                                   Assets::GetInstance().mDatabase, 1000);

        Revo::ResourceSystem::InitHook.Original(thisPtr);
    }

    bool __fastcall DatabaseDirectoryFilesOpenRecordHooked(
        EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles *thisPtr CATCH_EDX,
        const EA::ResourceMan::Key &key,
        EA::ResourceMan::IRecord **ppDstRecord,
        EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int _a, EA::ResourceMan::RecordInfo * pRecordInfo) {
        return false;
    }

    bool __fastcall DatabasePackedFileOpenRecordHooked(
        EA::ResourceMan::DatabasePackedFile::DatabasePackedFile *thisPtr CATCH_EDX,
        const EA::ResourceMan::Key &key,
        EA::ResourceMan::IRecord **ppDstRecord,
        EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int _a, EA::ResourceMan::RecordInfo *pRecordInfo) {
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
        EA::ResourceMan::DatabasePackedFile::DatabasePackedFile *thisPtr CATCH_EDX, EA::IO::AccessFlags accessFlags,
        EA::IO::CD creationDisposition, bool _a, bool _b) {
        const auto kDidOpen = EA::ResourceMan::DatabasePackedFile::OpenHook.Original(
            thisPtr, accessFlags, creationDisposition, _a, _b);

        if (kDidOpen) {
            EA::ResourceMan::PFIndexModifiable::PFIndexModifiable *pfIndex =
                    EA::ResourceMan::DatabasePackedFile::GetIndex(thisPtr);

            const std::filesystem::path kDbPath =
                    ModLoader::GetInstance().mModulePath / DATA_PATH / EA::ResourceMan::DatabasePackedFile::GetLocation(
                        thisPtr);

            // TODO: we might lose these if the game unloads them...
            for (auto element: pfIndex->mItemMap) {
                Assets::GetInstance().mDBPFItems[element.first] = {
                    .mPath = kDbPath,
                    .mRecord = element.second,
                };
            }
        }

        return kDidOpen;
    }

    wchar_t *CharToWChar(const char *input) {
        if (input == nullptr) {
            return nullptr;
        }

        const size_t kLen = std::mbstowcs(nullptr, input, 0);
        if (kLen == static_cast<size_t>(-1)) {
            return nullptr;
        }

        auto *wstr = new wchar_t[kLen + 1];
        std::mbstowcs(wstr, input, kLen + 1);
        return wstr;
    }

    Revo::App::TinyXmlInstance *ReadXMLFromPathHooked(Revo::App::TinyXmlInstance *pXmlInstance, const char *pFilename,
                                                      void *document,
                                                      const char *pRootType, bool bIsGameFile, double *_e,
                                                      const char *pFolder) {
        const auto kKey = Asset::GetKey(pFilename);

        if (!bIsGameFile || kKey.mType == static_cast<uint32_t>(FileType::UNKNOWN)) {
            return Revo::App::ReadXMLFromPathHook.Original(pXmlInstance, pFilename, document, pRootType, bIsGameFile, _e,
                                                           pFolder);
        }

        const auto *const kWideFilename = CharToWChar(pFilename);
        const auto *const kWideFolder = CharToWChar(pFolder);

        wchar_t realPath[MAX_PATH];
        Revo::App::GetCorrectLoadFolder(realPath, bIsGameFile, kWideFolder, kWideFilename);

        delete[] kWideFilename;
        delete[] kWideFolder;

        auto *const kRecord = new Resource::CustomRecord(kKey, new EA::IO::FileStream(realPath),
                                                         Assets::GetInstance().mDatabase);

        Tweaker::RegistryOnLoad(kRecord);

        auto *const kRet = Revo::App::ReadXMLFromStream(pFilename, kRecord->mStream, document, pRootType, _e);

        pXmlInstance->tinyXmlImplementation = kRet->tinyXmlImplementation;
        pXmlInstance->tiXmlElement = kRet->tiXmlElement;

        return kRet;
    }

#ifdef VERSION_COZY_BUNDLE
    void *LoadBody(void * _a, char *dynamicSkinName, void * _c, EA::ResourceMan::IResource *materialResource,
                   EA::ResourceMan::IResource *textureResource, EA::ResourceMan::IResource *maskResource) {
        auto *const kManager = EA::ResourceMan::Manager::GetManager();

        if (materialResource == nullptr) {
            const EA::ResourceMan::Key kKey = {
                .mInstance = Hash::FNV::FromString64("fallback"),
                .mType = static_cast<uint32_t>(FileType::MATERIAL),
                .mGroup = 0
            };

            EA::ResourceMan::Manager::GetResource(kManager, kKey, &materialResource, nullptr, nullptr, nullptr, nullptr,
                                                  0, 0, 0, 0);
        }

        if (textureResource == nullptr) {
            const EA::ResourceMan::Key kKey = {
                .mInstance = Hash::FNV::FromString32("fallback"),
                .mType = static_cast<uint32_t>(FileType::DDS),
                .mGroup = 0
            };

            EA::ResourceMan::Manager::GetResource(kManager, kKey, &textureResource, nullptr, nullptr, nullptr, nullptr,
                                                  0, 0, 0, 0);
        }

        if (maskResource == nullptr) {
            const EA::ResourceMan::Key kKey = {
                .mInstance = Hash::FNV::FromString32("empty"),
                .mType = static_cast<uint32_t>(FileType::DDS),
                .mGroup = 0
            };

            EA::ResourceMan::Manager::GetResource(kManager, kKey, &maskResource, nullptr, nullptr, nullptr, nullptr, 0,
                                                  0, 0, 0);
        }

        return Revo::load_bodyHook.Original(_a, dynamicSkinName, _c, materialResource, textureResource,
                                            maskResource);
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
        Revo::load_bodyHook.Install(&LoadBody);
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
        const EA::ResourceMan::Key kKey = Asset::GetKey(path);

        // Lua files with the same name as game ones will override them. Causing the game not to load
        // we want to discourage doing this and using hooks instead.
        if (kKey.mType == static_cast<uint32_t>(FileType::LUA)) {
            return;
        }

        const auto kFilename = path.stem().string();

        Resource::IdResolver::GetInstance().Add(kFilename);

        auto *const kAsset = new Asset(kKey, AssetType::kPath);
        kAsset->mPath = path;

        if (mDatabase != nullptr) {
            mDatabase->AddAsset(kAsset);
        }
    }

    void Assets::RegisterAsset(Asset *pAsset) const {
        if (pAsset->mKey.mType == static_cast<uint32_t>(FileType::LUA)) {
            return;
        }

        const auto kFilename = pAsset->mPath.stem().string();

        Resource::IdResolver::GetInstance().Add(kFilename);

        if (mDatabase != nullptr) {
            mDatabase->AddAsset(pAsset);

        }
    }

    bool Assets::GetAsset(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **ppRecord) {
        if (GetInstance().mDatabase == nullptr) {
            return false;
        }
        return GetInstance().mDatabase->OpenRecord2(key, ppRecord, EA::IO::AccessFlags::kRead, EA::IO::CD::kLoadAllFiles, 0,
                                                   nullptr);
    }

    bool Assets::GetAsset(const std::string &name, EA::ResourceMan::IRecord **ppRecord) {
        if (GetInstance().mDatabase == nullptr) {
            return false;
        }
        return GetInstance().mDatabase->OpenRecord2(Asset::GetKey(name), ppRecord, EA::IO::AccessFlags::kRead,
                                                   EA::IO::CD::kLoadAllFiles, 0, nullptr);
    }

    void Assets::CreateDatabase() {
        mDatabase = new Resource::CustomDatabase;

        if (Modloader::Mods::GetInstance().mMods.empty()) {
            return;
        }

        for (const auto &mod: Modloader::Mods::GetInstance().mMods) {
            std::filesystem::path modPath(mod->mPath);

            CreateDatabaseEntries(modPath);

            for (const auto &entry: std::filesystem::recursive_directory_iterator(modPath)) {
                if (is_directory(entry.path())) {
                    CreateDatabaseEntries(entry.path());
                }
            }
        }
        for (const auto &val: mDatabase->mAssets | std::views::values) {
            Tweaker::RegistryOnRegister(val);
        }

        MSML_LOG_INFO("Database created");
    }
}
