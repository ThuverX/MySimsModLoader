//
// Created by exozg on 24/01/2025.
//

#include "Asset.h"
#include <regex>

#include "Assets.h"
#include "../../EA/IO/MemoryStream.h"
#include "../hash/FNV.h"

namespace Msml::Core {
    EA::IO::IStream * Asset::GetStream() const {
        if (mType == AssetType::kPath) {
            return new EA::IO::FileStream(mPath);
        }

        if (mType == AssetType::kRedirect) {
            if (mKey == mKeyRedirect) {
                return nullptr; // failsafe
            }
            EA::ResourceMan::IRecord* record = {};

            if (Assets::GetInstance().mDatabase->OpenRecord2(mKeyRedirect, &record, EA::IO::AccessFlags::kRead, EA::IO::CD::kLoadAllFiles, 0, nullptr)) {
                return record->GetStream();
            }
            return nullptr;
        }

        if (mType == AssetType::kBuffer) {
            return new EA::IO::MemoryStream(mBuffer.data(), mBuffer.size());
        }

        return nullptr;
    }

    FileType Asset::GetFileType(const std::string &extension) {
        std::string ext = extension;

        std::ranges::transform(ext, ext.begin(), ::tolower);

        if (!extension.empty() && extension[0] == '.') {
            ext = ext.substr(1);
        }

        if (const auto kIt = kStringToEnum.find(ext); kIt != kStringToEnum.end()) {
            return kIt->second;
        }
        return FileType::UNKNOWN;
    }

    std::string Asset::GetTypeName(const uint32_t kValue) {
        for (const auto &[name, enumValue]: kStringToEnum) {
            if (static_cast<uint32_t>(enumValue) == kValue) {
                return name;
            }
        }

        return "unknown";
    }

    EA::ResourceMan::Key Asset::GetKey(const std::filesystem::path &path) {
        const auto kFilename = path.stem().string();
        const auto kExtension = path.extension().string();

        const auto kType = GetFileType(kExtension);

        if (const auto kSep = kFilename.find('!'); kSep != std::string::npos) {
            const auto kLeft = kFilename.substr(0, kSep);
            const auto kRight = kFilename.substr(kSep + 1);

            // Fix for weird file names on tb
            if (kLeft.find('_') != std::string::npos || kRight.find('_') != std::string::npos) {
                return {
                    .mInstance = 0,
                    .mType = 0,
                    .mGroup = 0
                };
            }

            const auto kGroup = static_cast<uint32_t>(std::stoul(kLeft, nullptr, 16));
            const auto kInstance = std::stoull(kRight, nullptr, 16);

            return {
                .mInstance = kInstance,
                .mType = static_cast<uint32_t>(kType),
                .mGroup = kGroup
            };
        }

        uint64_t instance = 0;

        if (kType == FileType::DDS) {
            instance = Hash::FNV::FromString32(kFilename);
        } else {
            instance = Hash::FNV::FromString64(kFilename);
        }

        return {
            .mInstance = instance,
            .mType = static_cast<uint32_t>(kType),
            .mGroup = 0,
        };
    }
}
