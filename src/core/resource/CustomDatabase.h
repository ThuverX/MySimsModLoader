//
// Created by exozg on 05/05/2025.
//

#ifndef CUSTOMDATABASE_H
#define CUSTOMDATABASE_H

#include <unordered_map>

#include "../../EA/ResourceMan/IDatabase.h"
#include "../assets/Asset.h"
#include "../signatures/sigdef.h"
#include "EASTL/vector.h"

namespace msml::core::resource {
    class CustomDatabase : public EA::ResourceMan::IDatabase {
        EA::ResourceMan::Manager::Manager *manager = nullptr;

    public:
        std::unordered_map<EA::ResourceMan::Key, assets::Asset *> assets;

        ~CustomDatabase() override = default;

        IDatabase *AsInterface(uint32_t id) override;

        bool Init() override;

        bool Shutdown() override;

        uint32_t GetDatabaseType() override;

        void Lock(bool toLock) override;

        void Open(EA::IO::AccessFlags access_flags, EA::IO::CD creation_disposition, bool, bool) override;

        void Close() override;

        EA::IO::AccessFlags GetAccessFlags() override;

        void Flush() override;

        const wchar_t *GetLocation() override;

        void SetLocation(wchar_t *) override;

        size_t GetKeyList(eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> &,
                          EA::ResourceMan::IKeyFilter *) override;

        size_t GetKeyListSortedByPosition(eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> &,
                                          EA::ResourceMan::IKeyFilter *) override;

        void GetRecordInfoList(
            eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, eastl::dummy_allocator> *
            output_list, const eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> *in_keys) override;

        void GetRecordInfoListSortedByPosition(
            eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, eastl::dummy_allocator> *
            output_list, const eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> *in_keys) override;

        bool OpenRecord1(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **pDstRecord,
                         EA::IO::AccessFlags accessFlags, EA::IO::CD cd, int, EA::ResourceMan::RecordInfo *) override;

        bool OpenRecord2(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **pDstRecord,
                         EA::IO::AccessFlags accessFlags, EA::IO::CD cd, int, EA::ResourceMan::RecordInfo *) override;

        uint32_t GetOpenCount() override;

        void CloseRecord(EA::ResourceMan::IRecord *) override;

        void DeleteRecord(EA::ResourceMan::IRecord *) override;

        uint64_t GetModificationTime() override;

        bool Attach(bool, EA::ResourceMan::Manager::Manager *pResourceMan, bool) override;

        void AddAsset(assets::Asset *pAsset);
        void GetKeys(std::vector<EA::ResourceMan::Key> &keys);
    };
}

#endif //CUSTOMDATABASE_H
