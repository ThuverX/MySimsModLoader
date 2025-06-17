//
// Created by exozg on 06/05/2025.
//

#include "FileStream.h"

#include "../../core/system/Logger.h"

namespace EA::IO {

    FileError MapWindowsErrorToFileError(DWORD errorCode) {
        switch (errorCode) {
            case ERROR_SUCCESS:
                return FileError::Success;
            case ERROR_INVALID_HANDLE:
                return FileError::InvalidHandle;
            case ERROR_OUTOFMEMORY:
            case ERROR_NOT_ENOUGH_MEMORY:
                return FileError::OutOfMemory;
            case ERROR_FILE_NOT_FOUND:
                return FileError::FileNotFound;
            case ERROR_PATH_NOT_FOUND:
            case ERROR_INVALID_DRIVE:
                return FileError::PathNotFound;
            case ERROR_ACCESS_DENIED:
                return FileError::AccessDenied;
            case ERROR_WRITE_PROTECT:
                return FileError::WriteProtect;
            case ERROR_CURRENT_DIRECTORY:
                return FileError::CurrentDirectory;
            case ERROR_NOT_READY:
                return FileError::NotReady;
            case ERROR_CRC:
                return FileError::CRC;
            default:
                return FileError::Other;
        }
    }

    FileStream::FileStream(const std::filesystem::path &path, const AccessFlags access_flags, const CD cd): path(path), cd(cd), accessFlags(access_flags) {
        DWORD wCd = 0;
        DWORD wAccess = 0;
        DWORD wFileShare = 0;

        switch (cd) {
            case CD::CreateNew:
                wCd = CREATE_NEW;
                break;
            case CD::CreateAlways:
                wCd = CREATE_ALWAYS;
                break;
            case CD::OpenExisting:
                wCd = OPEN_EXISTING;
                break;
            case CD::OpenAlways:
                wCd = OPEN_ALWAYS;
                break;
            case CD::TruncateExisting:
                wCd = TRUNCATE_EXISTING;
                break;
            default:
            case CD::LoadAllFiles:
            case CD::Default:
                wCd = OPEN_EXISTING;
                break;
        }

        switch (access_flags) {
            case AccessFlags::Read:
                wAccess = GENERIC_READ;
                wFileShare = FILE_SHARE_READ;
                break;
            case AccessFlags::Write:
                wAccess = GENERIC_WRITE;
                wFileShare = FILE_SHARE_WRITE;
                break;
            case AccessFlags::ReadWrite:
                wAccess = GENERIC_READ | GENERIC_WRITE;
                wFileShare = FILE_SHARE_READ | FILE_SHARE_WRITE;
                break;
            default:
            case AccessFlags::None:
                wAccess = GENERIC_READ;
                wFileShare = FILE_SHARE_READ;
                break;
        }

        hFile = CreateFileW(
            path.c_str(),
            wAccess,
            wFileShare,
            nullptr,
            wCd,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        fileError = MapWindowsErrorToFileError(GetLastError());

        if (hFile == INVALID_HANDLE_VALUE) {
            MSML_LOG_ERROR("Failed to open file %s", path.string().c_str());
        }
    }

    FileStream::FileStream(const std::filesystem::path &path): FileStream(path, AccessFlags::Read, CD::Default) {
    }

    uint32_t FileStream::GetType() const {
        return Type;
    }

    AccessFlags FileStream::GetAccessFlags() const {
        return (hFile != INVALID_HANDLE_VALUE) ? accessFlags : AccessFlags::None;
    }

    FileError FileStream::GetState() const {
        return fileError;
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

        fileError = MapWindowsErrorToFileError(GetLastError());

        return -1;
    }

    bool FileStream::Flush() {
        if (hFile == INVALID_HANDLE_VALUE) return false;
        return FlushFileBuffers(hFile) != 0;
    }

    size_t FileStream::Write(const void *pData, size_t nSize) {
        if (hFile == INVALID_HANDLE_VALUE || pData == nullptr) return -1;

        DWORD bytesWritten = 0;
        if (WriteFile(hFile, pData, static_cast<DWORD>(nSize), &bytesWritten, nullptr))
            return bytesWritten;

        fileError = MapWindowsErrorToFileError(GetLastError());

        return -1;
    }

    std::filesystem::path FileStream::GetPath() {
        return path;
    }
}
