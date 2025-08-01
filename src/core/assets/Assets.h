//
// Created by exozg on 24/01/2025.
//

#ifndef ASSETS_H
#define ASSETS_H

#include <filesystem>
#include <unordered_map>

#include "../../EA/ResourceMan/ResourceKey.h"
#include "../resource/CustomDatabase.h"

namespace Msml::Core {
    struct DBPFRecordInfo {
        std::filesystem::path mPath;
        EA::ResourceMan::RecordInfo mRecord;
    };

    class Assets {
    public:
        Resource::CustomDatabase* mDatabase = nullptr;
        std::unordered_map<EA::ResourceMan::Key, std::filesystem::path> mDDFPaths;
        std::unordered_map<EA::ResourceMan::Key, DBPFRecordInfo> mDBPFItems;

        static Assets& GetInstance();

        static void Install();
        void CreateDatabase();
        void RegisterAsset(const std::filesystem::path &path) const;
        void RegisterAsset(Asset* pAsset) const;
        static bool GetAsset(const EA::ResourceMan::Key& key, EA::ResourceMan::IRecord ** ppRecord);
        static bool GetAsset(const std::string& name, EA::ResourceMan::IRecord ** ppRecord);
        void CreateDatabaseEntries(const std::filesystem::path &path) const;
    };
}

#endif //ASSETS_H
