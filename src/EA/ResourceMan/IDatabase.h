//
// Created by exozg on 11/05/2025.
//

#ifndef IDATABASE_H
#define IDATABASE_H

#include <cstdint>
#include <EASTL/algorithm.h>
#include <EASTL/vector.h>

#include "IRecord.h"
#include "KeyFilter.h"
#include "RecordInfo.h"
#include "ResourceKey.h"
#include "../../core/signatures/sigdef.h"

namespace EA::ResourceMan {
    class IDatabase {
        uint32_t mRefCount = 0;

    public:
        bool mIsReady = false;

        virtual uint32_t AddRef() {
            return ++mRefCount;
        }

        virtual uint32_t Release() {
            const uint32_t kNewCount = --mRefCount;

            if (kNewCount == 0) {
                delete this;
            }

            return kNewCount;
        }

        virtual ~IDatabase() = default;

        virtual IDatabase *AsInterface(uint32_t typeID) = 0;

        virtual bool Init() = 0;

        virtual bool Shutdown() = 0;

        virtual uint32_t GetDatabaseType() = 0;

        virtual uint32_t GetRefCount() {
            return mRefCount;
        };

        virtual void Lock(bool toLock) = 0;

        virtual void Open(IO::AccessFlags accessFlags, IO::CD creationDisposition, bool, bool) = 0;

        virtual void Close() = 0;

        virtual IO::AccessFlags GetAccessFlags() = 0;

        virtual void Flush() = 0;

        virtual const wchar_t *GetLocation() = 0;

        virtual void SetLocation(wchar_t *) = 0;

        virtual size_t GetKeyList(eastl::vector<Key, EASTLDummyAllocatorType> &pDst, IKeyFilter* pFilter) = 0;

        virtual size_t GetKeyListSortedByPosition(eastl::vector<Key, EASTLDummyAllocatorType> &pDst, IKeyFilter* pFilter) = 0;

        virtual void GetRecordInfoList(
            eastl::vector<eastl::pair<Key, RecordInfo>, EASTLDummyAllocatorType> *pOutputList,
            const eastl::vector<Key, EASTLDummyAllocatorType> *pInKeys) = 0;

        virtual void GetRecordInfoListSortedByPosition(
            eastl::vector<eastl::pair<Key, RecordInfo>, EASTLDummyAllocatorType> *pOutputList,
            const eastl::vector<Key, EASTLDummyAllocatorType> *pInKeys) = 0;

        virtual bool OpenRecord1(const Key &key, IRecord **ppDstRecord, IO::AccessFlags accessFlags, IO::CD creationDisposition, int,
                                 RecordInfo * pRecordInfo) = 0;

        virtual bool OpenRecord2(const Key &key, IRecord **ppDstRecord, IO::AccessFlags accessFlags, IO::CD creationDisposition, int,
                                 RecordInfo * pRecordInfo) = 0;

        virtual uint32_t GetOpenCount() = 0;

        virtual void CloseRecord(IRecord *) = 0;

        virtual void DeleteRecord(IRecord *) = 0;

        virtual uint64_t GetModificationTime() = 0;

        virtual bool Attach(bool, Manager::Manager *pResourceMan, bool) = 0;
    };
}

#endif //IDATABASE_H
