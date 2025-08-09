//
// Created by exozg on 05/05/2025.
//

#include "CustomDatabase.h"

#include <ranges>

#include "CustomRecord.h"
#include "IdResolver.h"
#include "../../EA/IO/FileStream.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../EA/IO/SubFileStream.h"
#include "../../include/refpack.h"
#include "../../tweakers/Tweaker.h"
#include "../assets/Assets.h"
#include "../hash/FNV.h"
#include "../hooks/LuaHook.h"
#include "../modloader/ModLoader.h"


namespace Msml::Core::Resource {
    EA::ResourceMan::IDatabase *CustomDatabase::AsInterface(const uint32_t typeID) {
        if (typeID == 0x7fa120bb) {
            return this;
        }

        return nullptr;
    }

    bool CustomDatabase::Init() {
        mIsReady = true;
        return true;
    }

    bool CustomDatabase::Shutdown() {
        mIsReady = false;
        return true;
    }

    uint32_t CustomDatabase::GetDatabaseType() {
        return 0x7fa120bb;
    }

    bool CustomDatabase::OpenRecord2(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **pDstRecord,
                                     EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int,
                                     EA::ResourceMan::RecordInfo *pRecordInfo) {
        // The order should be Asset, DDF, DBPF

        CustomRecord *record = nullptr;
        EA::ResourceMan::RecordInfo recordInfo = {};

        if (mAssets.contains(key)) {
            auto *const kAsset = mAssets[key];
            record = new CustomRecord(key, kAsset->GetStream(), this);

            recordInfo.mCompressedSize = record->mStream->GetSize();
            recordInfo.mMemorySize = record->mStream->GetSize();
        } else if (Assets::GetInstance().mDDFPaths.contains(key)) {
            const auto kPath = Assets::GetInstance().mDDFPaths[key];
            record = new CustomRecord(key, new EA::IO::FileStream(kPath), this);

            recordInfo.mCompressedSize = record->mStream->GetSize();
            recordInfo.mMemorySize = record->mStream->GetSize();
        } else if (Assets::GetInstance().mDBPFItems.contains(key)) {
            const auto kItem = Assets::GetInstance().mDBPFItems[key];

            recordInfo.mFlags = kItem.mRecord.mFlags;
            recordInfo.mChunkOffset = kItem.mRecord.mChunkOffset;
            recordInfo.mCompressedSize = kItem.mRecord.mCompressedSize;
            recordInfo.mMemorySize = kItem.mRecord.mMemorySize;
            recordInfo.mIsSaved = kItem.mRecord.mIsSaved;

            if (kItem.mRecord.mFlags == static_cast<uint16_t>(-1)) {
                return false;
            }

            record = new CustomRecord(
                    key, new EA::IO::SubFileStream(kItem.mPath, kItem.mRecord.mChunkOffset,
                                                   kItem.mRecord.mCompressedSize), this);
        } else {
            uint64_t instance = Hash::FNV::FromString64("fallback");
            if (key.mType == static_cast<uint32_t>(FileType::DDS)) {
                instance = Hash::FNV::FromString32("fallback");
            }

            const EA::ResourceMan::Key kFallbackKey = {
                .mInstance = instance,
                .mType = key.mType,
                .mGroup = 0
            };

            if (mAssets.contains(kFallbackKey) && key != kFallbackKey) {
                auto *const kAsset = mAssets[kFallbackKey];
                record = new CustomRecord(key, kAsset->GetStream(), this);

                recordInfo.mCompressedSize = record->mStream->GetSize();
                recordInfo.mMemorySize = record->mStream->GetSize();
            }
        }

        if (record != nullptr) {
            if (pRecordInfo != nullptr) {
                *pRecordInfo = recordInfo;
            }

            if (pDstRecord != nullptr) {
                *pDstRecord = record;
                Tweaker::RegistryOnLoad(record);
            }

            return true;
        }

        return false;
    }

    size_t CustomDatabase::GetKeyList(eastl::vector<EA::ResourceMan::Key, EASTLDummyAllocatorType> &pDst,
                                      EA::ResourceMan::IKeyFilter *pFilter) {
        for (const auto &key: mAssets | std::views::keys) {
            if (pFilter->IsValid(key)) {
                pDst.push_back(key);
            }
        }

        return pDst.size();
    }

    size_t CustomDatabase::GetKeyListSortedByPosition(
        eastl::vector<EA::ResourceMan::Key, EASTLDummyAllocatorType> &pDst,
        EA::ResourceMan::IKeyFilter *pFilter) {
        for (const auto &key: mAssets | std::views::keys) {
            if (pFilter->IsValid(key)) {
                pDst.push_back(key);
            }
        }

        return pDst.size();
    }

    void CustomDatabase::GetRecordInfoList(
        eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, EASTLDummyAllocatorType> *
        pOutputList,
        const eastl::vector<EA::ResourceMan::Key, EASTLDummyAllocatorType> *pInKeys) {
    }

    void CustomDatabase::GetRecordInfoListSortedByPosition(
        eastl::vector<eastl::pair<EA::ResourceMan::Key, EA::ResourceMan::RecordInfo>, EASTLDummyAllocatorType> *
        pOutputList,
        const eastl::vector<EA::ResourceMan::Key, EASTLDummyAllocatorType> *pInKeys) {
        // leaving this unimplemented for now
        // for (auto in_key : *in_keys) {
        //     for (const auto &asset: assets) {
        //         if (asset.first == in_key) {
        //             output_list->emplace_back(
        //             eastl::make_pair(in_key, EA::ResourceMan::RecordInfo{
        //                 .chunkOffset = 0,
        //                 .compressedSize = asset->stream->GetSize(),
        //                 .memorySize = asset->stream->GetSize(),
        //                 .flags = 0
        //             }));
        //             break;
        //         }
        //     }
        // }
    }


    bool CustomDatabase::Attach(bool a, EA::ResourceMan::Manager::Manager *pResourceMan, bool b) {
        if (pResourceMan == nullptr) {
            return false;
        }

        // just calls EA::ResourceMan::Manager::SetTypename to update the manager on which typenames we use.
        // and "attaches" itself to this instance (Sets this->manager to pResourceMan)

        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x01661233, L"model");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xf9e50586, L"revomodel");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xb359c791, L"windowsmodel");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x8eaf13de, L"rig");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x6b20c4f3, L"clip");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x0166038c, L"keynamemap");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x015a1849, L"geometry");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x01d0e75d, L"material");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x02019972, L"materialset");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x00b552ea, L"oldspeedtree");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x021d7e8c, L"speedtree");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x00b2d882, L"dds");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x8e342417, L"compositetexture");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x025ed6f4, L"simoutfit");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x585ee310, L"levelxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x58969018, L"levelbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xd5988020, L"physics");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x474999b4, L"luascript");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x50182640, L"lightsetxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x50002128, L"lightsetbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xdc37e964, L"xml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x2c81b60a, L"footprintset");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xc876c85e, L"objectconstructionxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xc08ec0ee, L"objectconstructionbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x4045d294, L"slotxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x487bf9e4, L"slotbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xcf60795e, L"swm");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x9752e396, L"swarmbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xe0d83029, L"xmlbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xa6856948, L"cabxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xc644f440, L"cabbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x5bca8c06, L"big");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xb6b5c271, L"bnk");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x474999b4, L"lua");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x2b8e2411, L"luo");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xb61215e9, L"lightboxxml");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xd6215201, L"lightboxbin");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x1e1e6516, L"xmb");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0xfd72d418, L"ttf");
        EA::ResourceMan::Manager::SetTypename(pResourceMan, 0x35ebb959, L"ttc");

        this->mManager = pResourceMan;

        return true;
    }

    void CustomDatabase::AddAsset(Asset *pAsset) {
        if (pAsset->mType == AssetType::kBuffer) {
            MSML_LOG_DEBUG("Registering %s: <buffer>", IdResolver::ToFilename(pAsset->mKey).c_str());
        } else if (pAsset->mType == AssetType::kPath) {
            MSML_LOG_DEBUG("Registering %s: \"%s\"", IdResolver::ToFilename(pAsset->mKey).c_str(),
                           pAsset->mPath.filename().string().c_str());
        } else if (pAsset->mType == AssetType::kRedirect) {
            MSML_LOG_DEBUG("Registering %s: -> %s", IdResolver::ToFilename(pAsset->mKey).c_str(),
                           IdResolver::ToFilename(pAsset->mKeyRedirect).c_str());
        }
        mAssets[pAsset->mKey] = pAsset;
    }

#pragma region Unused

    // unused
    void CustomDatabase::Lock(bool toLock) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::Lock");
    }

    // unused
    void CustomDatabase::Open(EA::IO::AccessFlags access_flags, EA::IO::CD creation_disposition, bool, bool) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::Open");
    }

    // unused
    void CustomDatabase::Close() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::Close");
    }

    // unused
    EA::IO::AccessFlags CustomDatabase::GetAccessFlags() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::GetAccessFlags");
        return EA::IO::AccessFlags::kRead;
    }

    // unused
    void CustomDatabase::Flush() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::Flush");
    }

    // unused
    const wchar_t *CustomDatabase::GetLocation() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::GetLocation");
        return L"";
    }

    // unused
    void CustomDatabase::SetLocation(wchar_t *) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::SetLocation");
    }

    // unused
    bool CustomDatabase::OpenRecord1(const EA::ResourceMan::Key &key, EA::ResourceMan::IRecord **ppDstRecord,
                                     EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition, int,
                                     EA::ResourceMan::RecordInfo *) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::OpenRecordUnused");
        return false;
    }

    // unused
    uint32_t CustomDatabase::GetOpenCount() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::GetOpenCount");
        return 0;
    }

    // unused
    void CustomDatabase::CloseRecord(EA::ResourceMan::IRecord *) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::CloseRecord");
    }

    // unused
    void CustomDatabase::DeleteRecord(EA::ResourceMan::IRecord *) {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::DeleteRecord");
    }

    // unused
    uint64_t CustomDatabase::GetModificationTime() {
        MSML_LOG_ERROR("UNUSED: CustomDatabase::GetModificationTime");
        return 0;
    }

#pragma endregion
}
