//
// Created by exozg on 27/06/2025.
//

#include "File.h"

#include <windows.h>

namespace EA::IO::File {
    bool Exists(const std::filesystem::path &path) {
        const DWORD dwAttrib = GetFileAttributesW(path.c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
               !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    bool Remove(const std::filesystem::path &path) {
        return DeleteFileW(path.c_str()) != FALSE;
    }
}
