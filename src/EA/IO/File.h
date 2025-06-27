//
// Created by exozg on 27/06/2025.
//

#ifndef FILE_H
#define FILE_H

#include <filesystem>

namespace EA::IO::File {
    bool Exists(const std::filesystem::path& path);
    bool Remove(const std::filesystem::path& path);
};



#endif //FILE_H
