//
// Created by exozg on 24/01/2025.
//

#include "Asset.h"
#include <regex>

#include "Assets.h"
#include "../../EA/IO/MemoryStream.h"
#include "../hash/FNV.h"

namespace msml::core::assets {
    EA::IO::IStream * Asset::GetStream() const {
        if (type == PATH) return new EA::IO::FileStream(path);
        if (type == REDIRECT) {
            if (key == key_redirect) return nullptr; // failsafe
            EA::ResourceMan::IRecord* record = {};

            if (Assets::GetInstance().database->OpenRecord2(key_redirect, &record, EA::IO::AccessFlags::Read, EA::IO::CD::LoadAllFiles, 0, nullptr))
                return record->GetStream();
            return nullptr;
        }
        if (type == BUFFER) return new EA::IO::MemoryStream(buffer.data(), buffer.size());

        return nullptr;
    }

    DDFFileType Asset::GetFileType(const std::string &extension) {
        std::string ext = extension;

        std::ranges::transform(ext, ext.begin(), ::tolower);

        if (!extension.empty() && extension[0] == '.') {
            ext = ext.substr(1);
        }

        if (const auto it = stringToEnum.find(ext); it != stringToEnum.end()) {
            return it->second;
        }
        return UNKNOWN;
    }

    std::string Asset::GetTypeName(const uint32_t value) {
        for (const auto &[name, enumValue]: stringToEnum) {
            if (static_cast<uint32_t>(enumValue) == value) {
                return name;
            }
        }

        return "unknown";
    }

    EA::ResourceMan::Key Asset::GetKey(const std::filesystem::path &path) {
        const auto filename = path.stem().string();
        const auto extension = path.extension().string();

        const uint32_t type = GetFileType(extension);

        if (const auto sep = filename.find('!'); sep != std::string::npos) {
            const auto left = filename.substr(0, sep);
            const auto right = filename.substr(sep + 1);

            // Fix for weird file names on tb
            if (left.find('_') != std::string::npos || right.find('_') != std::string::npos) {
                return {
                    .instance = 0,
                    .type = UNKNOWN,
                    .group = 0
                };
            }

            const auto group = static_cast<uint32_t>(std::stoul(left, nullptr, 16));
            const auto instance = std::stoull(right, nullptr, 16);

            return {
                .instance = instance,
                .type = type,
                .group = group
            };
        }

        uint64_t instance = 0;

        // TODO: This is prob not right...
        if (type == DDS) {
            instance = hash::fnv::FromString32(filename.c_str());
        } else {
            instance = hash::fnv::FromString64(filename.c_str());
        }

        return {
            .instance = instance,
            .type = type,
            .group = 0,
        };
    }
}
