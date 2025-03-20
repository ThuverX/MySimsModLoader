//
// Created by exozg on 24/01/2025.
//

#include "Asset.h"
#include <regex>

DDFFileType Asset::GetFileType(const std::string& extension) {
    std::string ext = extension;

    if (!extension.empty() && extension[0] == '.') {
        ext = extension.substr(1);
    }

    if (const auto it = stringToEnum.find(ext); it != stringToEnum.end()) {
        return it->second;
    }
    return UNKNOWN;
}

std::string Asset::GetTypeName(const uint32_t value) {
    for (const auto& [name, enumValue] : stringToEnum) {
        if (static_cast<uint32_t>(enumValue) == value) {
            return name;
        }
    }
    return "unknown";
}