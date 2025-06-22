//
// Created by exozg on 12/03/2025.
//

#include "./IdResolver.h"

#include <fstream>

#include "../assets/Asset.h"
#include "../hash/FNV.h"
#include "../system/Logger.h"

namespace Msml::Core::Resource {
    IdResolver &IdResolver::GetInstance() {
        static IdResolver instance;
        return instance;
    }

    std::string IdResolver::GetHash64(const uint64_t kHash) {
        if (mHashes64.contains(kHash)) {
            return mHashes64.at(kHash);
        }

        return std::format("{:016x}", kHash);
    }

    std::string IdResolver::GetHash32(const uint32_t kHash) {
        if (mHashes32.contains(kHash)) {
            return mHashes32.at(kHash);
        }

        return std::format("{:08x}", kHash);
    }

    void IdResolver::Load(const std::string &path) {
        auto *const kStream = new EA::IO::FileStream(path);
        kStream->AddRef();

        mHashes32.clear();
        mHashes64.clear();

        if (kStream->GetState() != EA::IO::FileError::kSuccess || kStream->GetAccessFlags() == EA::IO::AccessFlags::kNone) {
            MSML_LOG_ERROR("Failed to load Hashes from %s", path.c_str());
            kStream->Close();
            kStream->Release();
            return;
        }

        uint64_t count = 0;
        READ(kStream, count);

        for (uint64_t i = 0; i < count; i++) {
            std::string text;
            READ_CSTRING(kStream, text);
            uint32_t fnv32 = 0;
            READ(kStream, fnv32);
            uint64_t fnv64 = 0;
            READ(kStream, fnv64);

            mHashes32.insert(std::pair(fnv32, text));
            mHashes64.insert(std::pair(fnv64, text));
        }

        kStream->Close();
        kStream->Release();

        MSML_LOG_INFO("Loaded %d Hashes from %s", mHashes32.size(), path.c_str());
    }

    void IdResolver::Add(const std::string &kStr) {
        uint64_t fnv64 = Hash::FNV::FromString64(kStr);
        uint32_t fnv32 = Hash::FNV::FromString32(kStr);

        mHashes32.insert(std::pair(fnv32, kStr));
        mHashes64.insert(std::pair(fnv64, kStr));
    }

    std::string IdResolver::ToHumanReadable(const EA::ResourceMan::Key &key) {
        const std::string kInstance = GetInstance().GetHash64(key.mInstance);
        const std::string kType = Asset::GetTypeName(key.mType);
        const std::string kGroup = GetInstance().GetHash32(key.mGroup);

        if (key.mGroup == 0) {
            return kInstance + "." + kType;
        }

        return kGroup + "!" + kInstance + "." + kType;
    }

    std::string IdResolver::ToFilename(const EA::ResourceMan::Key &key) {
        const std::string kInstance = std::format("0x{:016x}", key.mInstance);
        const std::string kType = Asset::GetTypeName(key.mType);
        const std::string kGroup = std::format("0x{:08x}", key.mGroup);

        return kGroup + "!" + kInstance + "." + kType;
    }
}
