//
// Created by exozg on 06/05/2025.
//

#include "FileStream.h"

#include "../../core/system/Logger.h"

namespace EA::IO {

    FileError MapWindowsErrorToFileError(const DWORD kErrorCode) {
        switch (kErrorCode) {
            case ERROR_SUCCESS:
                return FileError::kSuccess;
            case ERROR_INVALID_HANDLE:
                return FileError::kInvalidHandle;
            case ERROR_OUTOFMEMORY:
            case ERROR_NOT_ENOUGH_MEMORY:
                return FileError::kOutOfMemory;
            case ERROR_FILE_NOT_FOUND:
                return FileError::kFileNotFound;
            case ERROR_PATH_NOT_FOUND:
            case ERROR_INVALID_DRIVE:
                return FileError::kPathNotFound;
            case ERROR_ACCESS_DENIED:
                return FileError::kAccessDenied;
            case ERROR_WRITE_PROTECT:
                return FileError::kWriteProtect;
            case ERROR_CURRENT_DIRECTORY:
                return FileError::kCurrentDirectory;
            case ERROR_NOT_READY:
                return FileError::kNotReady;
            case ERROR_CRC:
                return FileError::kCrc;
            default:
                return FileError::kOther;
        }
    }

    FileStream::FileStream(const std::filesystem::path &path, const AccessFlags kAccessFlags, const CD kCreationDisposition): mPath(path), mCd(kCreationDisposition), mAccessFlags(kAccessFlags) {
        DWORD wCd = 0;
        DWORD wAccess = 0;
        DWORD wFileShare = 0;

        switch (kCreationDisposition) {
            case CD::kCreateNew:
                wCd = CREATE_NEW;
                break;
            case CD::kCreateAlways:
                wCd = CREATE_ALWAYS;
                break;
            case CD::kOpenExisting:
                wCd = OPEN_EXISTING;
                break;
            case CD::kOpenAlways:
                wCd = OPEN_ALWAYS;
                break;
            case CD::kTruncateExisting:
                wCd = TRUNCATE_EXISTING;
                break;
            default:
            case CD::kLoadAllFiles:
            case CD::kDefault:
                wCd = OPEN_EXISTING;
                break;
        }

        switch (kAccessFlags) {
            case AccessFlags::kRead:
                wAccess = GENERIC_READ;
                wFileShare = FILE_SHARE_READ;
                break;
            case AccessFlags::kWrite:
                wAccess = GENERIC_WRITE;
                wFileShare = FILE_SHARE_WRITE;
                break;
            case AccessFlags::kReadWrite:
                wAccess = GENERIC_READ | GENERIC_WRITE;
                wFileShare = FILE_SHARE_READ | FILE_SHARE_WRITE;
                break;
            default:
            case AccessFlags::kNone:
                wAccess = GENERIC_READ;
                wFileShare = FILE_SHARE_READ;
                break;
        }

        mFileHandle = CreateFileW(
            path.c_str(),
            wAccess,
            wFileShare,
            nullptr,
            wCd,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        mFileError = MapWindowsErrorToFileError(GetLastError());

        if (mFileHandle == INVALID_HANDLE_VALUE) {
            MSML_LOG_ERROR("Failed to open file %s", path.string().c_str());
        }
    }

    FileStream::FileStream(const std::filesystem::path &path): FileStream(path, AccessFlags::kRead, CD::kDefault) {
    }

    uint32_t FileStream::GetType() const {
        return kType;
    }

    AccessFlags FileStream::GetAccessFlags() const {
        return (mFileHandle != INVALID_HANDLE_VALUE) ? mAccessFlags : AccessFlags::kNone;
    }

    FileError FileStream::GetState() const {
        return mFileError;
    }

    bool FileStream::Close() {
        if (mFileHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(mFileHandle);
            mFileHandle = INVALID_HANDLE_VALUE;
            return true;
        }
        return false;
    }

    size_t FileStream::GetSize() const {
        if (mFileHandle == INVALID_HANDLE_VALUE) {
            return -1;
        }

        LARGE_INTEGER size;
        if (GetFileSizeEx(mFileHandle, &size) != 0) {
            return static_cast<size_t>(size.QuadPart);
        }

        return -2;
    }

    bool FileStream::SetSize(size_t size) {
        return false;
    }

    size_t FileStream::GetPosition(PositionType positionType) const {
        if (mFileHandle == INVALID_HANDLE_VALUE) {
            return -1;
        }

        constexpr LARGE_INTEGER kZero = {};
        LARGE_INTEGER pos;

        if (positionType == PositionType::kEnd) {
            if (SetFilePointerEx(mFileHandle, kZero, &pos, FILE_CURRENT) != 0) {
                return GetSize() - static_cast<size_t>(pos.QuadPart);
            }
        }

        if (SetFilePointerEx(mFileHandle, kZero, &pos, FILE_CURRENT) != 0) {
            return static_cast<size_t>(pos.QuadPart);
        }

        return -1;
    }

    bool FileStream::SetPosition(const int kDistance, PositionType positionType) {
        if (mFileHandle == INVALID_HANDLE_VALUE) {
            return false;
        }

        LARGE_INTEGER largeInteger = {};
        largeInteger.QuadPart = static_cast<LONGLONG>(kDistance);

        DWORD method = FILE_CURRENT;
        switch (positionType) {
            case PositionType::kBegin: method = FILE_BEGIN; break;
            case PositionType::kEnd: method = FILE_END; break;
            default: method = FILE_CURRENT; break;
        }

        return SetFilePointerEx(mFileHandle, largeInteger, nullptr, method) != 0;
    }

    size_t FileStream::GetAvailable() const {
        if (mFileHandle == INVALID_HANDLE_VALUE) {
            return -1;
        }

        const size_t kSize = GetSize();
        const size_t kPos = GetPosition(PositionType::kBegin);
        return (kPos <= kSize) ? (kSize - kPos) : -1;
    }

    size_t FileStream::Read(void *pData, size_t nSize) {
        if (mFileHandle == INVALID_HANDLE_VALUE || pData == nullptr) return -1;

        DWORD bytesRead = 0;
        if (ReadFile(mFileHandle, pData, static_cast<DWORD>(nSize), &bytesRead, nullptr) != 0) {
            return bytesRead;
        }

        mFileError = MapWindowsErrorToFileError(GetLastError());

        return -1;
    }

    bool FileStream::Flush() {
        if (mFileHandle == INVALID_HANDLE_VALUE) {
            return false;
        }
        return FlushFileBuffers(mFileHandle) != 0;
    }

    size_t FileStream::Write(const void *pData, size_t nSize) {
        if (mFileHandle == INVALID_HANDLE_VALUE || pData == nullptr) {
            return -1;
        }

        DWORD bytesWritten = 0;
        if (WriteFile(mFileHandle, pData, static_cast<DWORD>(nSize), &bytesWritten, nullptr) != 0) {
            return bytesWritten;
        }

        mFileError = MapWindowsErrorToFileError(GetLastError());

        return -1;
    }

    std::filesystem::path FileStream::GetPath() {
        return mPath;
    }
}
