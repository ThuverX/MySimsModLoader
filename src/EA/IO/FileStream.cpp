//
// Created by exozg on 06/05/2025.
//

#include "FileStream.h"

namespace EA::IO {
    FileStream::FileStream(const std::filesystem::path &path): path(path) {
        hFile = CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
    }

    uint32_t FileStream::GetType() const {
        return Type;
    }

    AccessFlags FileStream::GetAccessFlags() const {
        return (hFile != INVALID_HANDLE_VALUE) ? AccessFlags::Read : AccessFlags::None;
    }

    FileError FileStream::GetState() const {
        return (hFile != INVALID_HANDLE_VALUE) ? FileError::Success : FileError::InvalidHandle;
    }

    bool FileStream::Close() {
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            return true;
        }
        return false;
    }

    size_t FileStream::GetSize() const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        LARGE_INTEGER size;
        if (GetFileSizeEx(hFile, &size))
            return static_cast<size_t>(size.QuadPart);

        return -2;
    }

    bool FileStream::SetSize(size_t size) {
        return false;
    }

    size_t FileStream::GetPosition(PositionType positionType) const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        constexpr LARGE_INTEGER zero = {};
        LARGE_INTEGER pos;

        if (positionType == PositionType::End) {
            if (SetFilePointerEx(hFile, zero, &pos, FILE_CURRENT))
                return GetSize() - static_cast<size_t>(pos.QuadPart);
        }

        if (SetFilePointerEx(hFile, zero, &pos, FILE_CURRENT))
            return static_cast<size_t>(pos.QuadPart);

        return -1;
    }

    bool FileStream::SetPosition(const int distance, PositionType positionType) {
        if (hFile == INVALID_HANDLE_VALUE) return false;

        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(distance);

        DWORD method = FILE_CURRENT;
        switch (positionType) {
            case PositionType::Begin: method = FILE_BEGIN; break;
            case PositionType::End: method = FILE_END; break;
            default: method = FILE_CURRENT; break;
        }

        return SetFilePointerEx(hFile, li, nullptr, method);
    }

    size_t FileStream::GetAvailable() const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        const size_t size = GetSize();
        const size_t pos = GetPosition(PositionType::Begin);
        return (pos <= size) ? (size - pos) : -1;
    }

    size_t FileStream::Read(void *pData, size_t nSize) {
        if (hFile == INVALID_HANDLE_VALUE || pData == nullptr) return -1;

        DWORD bytesRead = 0;
        if (ReadFile(hFile, pData, static_cast<DWORD>(nSize), &bytesRead, nullptr))
            return bytesRead;

        return -1;
    }

    bool FileStream::Flush() {
        if (hFile == INVALID_HANDLE_VALUE) return false;
        return FlushFileBuffers(hFile) != 0;
    }

    size_t FileStream::Write(const void *pData, size_t nSize) {
        return 0;
    }

    std::filesystem::path FileStream::GetPath() {
        return path;
    }
}
