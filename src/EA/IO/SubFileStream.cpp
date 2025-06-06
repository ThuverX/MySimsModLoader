//
// Created by exozg on 16/05/2025.
//

#include "SubFileStream.h"

namespace EA::IO {
    SubFileStream::SubFileStream(const std::filesystem::path &path, const size_t offset, const size_t size): path(path) {
        hFile = CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        begin = offset;
        this->size = size;
        end = begin + size;

        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(begin);

        SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN);
    }

    uint32_t SubFileStream::GetType() const {
        return 0x1489eb2f;
    }

    AccessFlags SubFileStream::GetAccessFlags() const {
        return (hFile != INVALID_HANDLE_VALUE) ? AccessFlags::Read : AccessFlags::None;
    }

    FileError SubFileStream::GetState() const {
        return (hFile != INVALID_HANDLE_VALUE) ? FileError::Success : FileError::InvalidHandle;
    }

    bool SubFileStream::Close() {
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            return true;
        }
        return false;
    }

    size_t SubFileStream::GetSize() const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        return size;
    }

    bool SubFileStream::SetSize(size_t size) {
        this->size = size;
        end = begin + size;
        return true;
    }

    size_t SubFileStream::GetPosition(PositionType positionType) const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        constexpr LARGE_INTEGER zero = {};
        LARGE_INTEGER pos;

        if (positionType == PositionType::End) {
            if (SetFilePointerEx(hFile, zero, &pos, FILE_CURRENT))
                return GetSize() - static_cast<size_t>(pos.QuadPart) - begin;
        }

        if (SetFilePointerEx(hFile, zero, &pos, FILE_CURRENT))
            return static_cast<size_t>(pos.QuadPart) - begin;

        return -1;
    }

    bool SubFileStream::SetPosition(int distance, PositionType positionType) {
        if (hFile == INVALID_HANDLE_VALUE) return false;

        DWORD method = FILE_CURRENT;
        switch (positionType) {
            case PositionType::Begin: {
                method = FILE_BEGIN;
                distance = begin + distance;
                break;
            }
            case PositionType::End: {
                method = FILE_BEGIN;
                distance = end - distance;
                break;
            }
            default: {
                method = FILE_CURRENT;
                break;
            }
        }

        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(distance);

        return SetFilePointerEx(hFile, li, nullptr, method);
    }

    size_t SubFileStream::GetAvailable() const {
        if (hFile == INVALID_HANDLE_VALUE) return -1;

        const size_t size = GetSize();
        const size_t pos = GetPosition(PositionType::Begin);
        return (pos <= size) ? (size - pos) : -1;
    }

    size_t SubFileStream::Read(void *pData, size_t nSize) {
        if (hFile == INVALID_HANDLE_VALUE || pData == nullptr) return -1;

        DWORD bytesRead = 0;
        if (ReadFile(hFile, pData, static_cast<DWORD>(nSize), &bytesRead, nullptr))
            return bytesRead;

        return -1;
    }

    bool SubFileStream::Flush() {
        if (hFile == INVALID_HANDLE_VALUE) return false;
        return FlushFileBuffers(hFile) != 0;
    }

    size_t SubFileStream::Write(const void *pData, size_t nSize) {
        return 0;
    }
}
