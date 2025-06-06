//
// Created by exozg on 24/01/2025.
//

#ifndef ASSETS_H
#define ASSETS_H

#include <filesystem>
#include <unordered_map>

#include "../../EA/ResourceMan/ResourceKey.h"
#include "../resource/CustomDatabase.h"
#include "../resource/CustomRecord.h"

namespace msml::core {
    struct DBPFRecordInfo {
        std::filesystem::path path;
        EA::ResourceMan::RecordInfo record;
    };

    class Assets {
    public:
        static Assets& GetInstance();

        static void Install();
        void CreateDatabase();
        void RegisterAsset(const std::filesystem::path &path) const;
        void RegisterAsset(assets::Asset* asset) const;
        static bool GetAsset(const EA::ResourceMan::Key& key, EA::ResourceMan::IRecord ** record);
        static bool GetAsset(const std::string& name, EA::ResourceMan::IRecord ** record);
        void CreateDatabaseEntries(const std::filesystem::path &path) const;
        resource::CustomDatabase* database = nullptr;
        std::unordered_map<EA::ResourceMan::Key, std::filesystem::path> ddf_paths;
        std::unordered_map<EA::ResourceMan::Key, DBPFRecordInfo> dbpf_items;
    };
}

#endif //ASSETS_H
