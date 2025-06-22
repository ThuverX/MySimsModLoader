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

namespace Msml::Core::Resource {
    class CustomDatabase : public EA::ResourceMan::IDatabase {
        EA::ResourceMan::Manager::Manager *mManager = nullptr;

    public:
        std::unordered_map<EA::ResourceMan::Key, Asset *> mAssets;

        ~CustomDatabase() override = default;

        IDatabase *AsInterface(uint32_t typeID) override;

        bool Init() override;

        bool Shutdown() override;

        uint32_t GetDatabaseType() override;

        void Lock(bool toLock) override;

        void Open(EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, bool, bool) override;

        void Close() override;

        EA::IO::AccessFlags GetAccessFlags() override;

        void Flush() override;

        const wchar_t *GetLocation() override;

        void SetLocation(wchar_t *location) override;

        size_t GetKeyList(eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> &,
                          EA::ResourceMan::IKeyFilter *) override;

        size_t GetKeyListSortedByPosition(eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> &,
                                          EA::ResourceMan::IKeyFilter *) override;

        void GetRecordInfoList(
            eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, eastl::dummy_allocator> *
            outputList, const eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> *inKeys) override;

        void GetRecordInfoListSortedByPosition(
            eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, eastl::dummy_allocator> *
            outputList, const eastl::vector<EA::ResourceMan::Key, eastl::dummy_allocator> *inKeys) override;

        bool OpenRecord1(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **pDstRecord,
                         EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int unknown,
                         EA::ResourceMan::RecordInfo *) override;

        bool OpenRecord2(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **pDstRecord,
                         EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int unknown,
                         EA::ResourceMan::RecordInfo *) override;

        uint32_t GetOpenCount() override;

        void CloseRecord(EA::ResourceMan::IRecord *record) override;

        void DeleteRecord(EA::ResourceMan::IRecord *record) override;

        uint64_t GetModificationTime() override;

        bool Attach(bool, EA::ResourceMan::Manager::Manager *pResourceMan, bool) override;

        void AddAsset(Asset *pAsset);
    };
}

#endif //CUSTOMDATABASE_H
