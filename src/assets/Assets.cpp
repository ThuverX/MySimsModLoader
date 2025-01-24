//
// Created by exozg on 24/01/2025.
//

#include "Assets.h"

#include <utility>

#include "../signatures/Signatures.h"
#include "../util/File.h"

Assets & Assets::GetInstance() {
    static Assets instance;
    return instance;
}

void* __fastcall PFRecordRead_1Hook(void* this_ptr, void* _ECX, uint32_t a, uint32_t b, EA::ResourceMan::Key* key, EA::ResourceMan::DatabasePackedFile* db) {
    if (const Asset* asset = Assets::GetInstance().GetAsset(key->instance, key->group, key->type)) {
        auto bytes = File::ReadAsBytes(asset->path.c_str());

        void* data = bytes.data();
        const uint32_t size = bytes.size();

        return EA::ResourceMan::PFRecordRead::PFRecordRead_2Hook.Original(this_ptr, data, size, false, key, db);
    }

    return EA::ResourceMan::PFRecordRead::PFRecordRead_1Hook.Original(this_ptr, a, b, key, db);
}

void* __fastcall PFRecordRead_2Hook(void* this_ptr, void* _ECX, void* data, uint32_t size, bool ukn, EA::ResourceMan::Key* key, EA::ResourceMan::DatabasePackedFile* db) {
    if (const Asset* asset = Assets::GetInstance().GetAsset(key->instance, key->group, key->type)) {
        auto bytes = File::ReadAsBytes(asset->path.c_str());

        void* data = bytes.data();
        const uint32_t size = bytes.size();

        return EA::ResourceMan::PFRecordRead::PFRecordRead_2Hook.Original(this_ptr, data, size, false, key, db);
    }


    return EA::ResourceMan::PFRecordRead::PFRecordRead_2Hook.Original(this_ptr, data, size, ukn, key, db);
}

void Assets::Install() {
    EA::ResourceMan::PFRecordRead::PFRecordRead_1Hook.Install(&PFRecordRead_1Hook);
    EA::ResourceMan::PFRecordRead::PFRecordRead_2Hook.Install(&PFRecordRead_2Hook);
}

void Assets::RegisterAsset(std::string path, const EA::ResourceMan::Key *key) {
    auto* asset = new Asset;
    asset->key = *key;
    asset->path = std::move(path);
    assets.emplace_back(asset);
}

Asset * Assets::GetAsset(uint64_t instance, uint32_t group, uint32_t type) const {
    for (auto & asset : assets) {
        if (asset->key.instance == instance &&
            asset->key.group == group &&
            asset->key.type == type) {
            return asset;
        }
    }
    return nullptr;
}
