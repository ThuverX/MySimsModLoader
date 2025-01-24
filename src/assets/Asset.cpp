//
// Created by exozg on 24/01/2025.
//

#include "Asset.h"
#include <regex>

#include "../util/FNV.h"

EA::ResourceMan::Key* Asset::FromAssetName(const std::string path) {
    auto k = new EA::ResourceMan::Key;

    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
    std::string extension = base_filename.substr(base_filename.find_last_of(".") + 1);
    std::string file_without_extension = base_filename.substr(0, base_filename.find_last_of('.'));

    std::regex formatRegex(R"(0x[0-9A-Fa-f]+!0x[0-9A-Fa-f]+)");

    size_t exclamationPos = file_without_extension.find('!');
    if (std::smatch match; exclamationPos != std::string::npos &&std::regex_search(file_without_extension, match, formatRegex)) {

        std::string leftStr = file_without_extension.substr(0, exclamationPos);
        std::string rightStr = file_without_extension.substr(exclamationPos + 1);

        std::stringstream leftStream(leftStr);
        leftStream >> std::hex >> k->group;
        if (leftStream.fail()) {
            throw std::invalid_argument("Failed to parse group part as uint32_t");
        }

        std::stringstream rightStream(rightStr);
        rightStream >> std::hex >> k->instance;
        if (rightStream.fail()) {
            throw std::invalid_argument("Failed to parse instance part as uint64_t");
        }
    } else {
        // this file name is plain text
        k->instance = FNV::FromString64(file_without_extension.c_str());
        k->group = 0; // TODO: this should take the folder name its in and use that
    }

    k->type = GetFileType(extension);

    return k;
}

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
