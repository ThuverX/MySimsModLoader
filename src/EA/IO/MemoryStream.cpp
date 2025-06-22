//
// Created by exozg on 08/05/2025.
//

#include "MemoryStream.h"

namespace EA::IO {
    MemoryStream::MemoryStream() = default;

    MemoryStream::MemoryStream(const void *pData, const size_t kSize) {
        mBuffer.resize(kSize);
        memcpy(mBuffer.data(), pData, kSize);
    }

    uint32_t MemoryStream::GetType() const {
        return kType;
    }

    AccessFlags MemoryStream::GetAccessFlags() const {
        return AccessFlags::kReadWrite;
    }

    FileError MemoryStream::GetState() const {
        return FileError::kSuccess;
    }

    bool MemoryStream::Close() {
        mBuffer.clear();
        return true;
    }

    size_t MemoryStream::GetSize() const {
        return mBuffer.size();
    }

    bool MemoryStream::SetSize(size_t size) {
        mBuffer.resize(size);
        return false;
    }

    size_t MemoryStream::GetPosition(PositionType positionType) const {
        if (positionType == PositionType::kBegin) {
            return mPosition;
        }

        if (positionType == PositionType::kEnd) {
            return mBuffer.size() - mPosition;
        }

        return 0;
    }

    bool MemoryStream::SetPosition(int distance, PositionType positionType) {
        if (distance > mBuffer.size()) {
            return false;
        }

        if (positionType == PositionType::kBegin) {
            mPosition = distance;
            return true;
        }

        if (positionType == PositionType::kCurrent) {
            mPosition += distance;
            return true;
        }

        if (positionType == PositionType::kEnd) {
            mPosition = mBuffer.size() - distance;
            return true;
        }

        return false;
    }

    size_t MemoryStream::GetAvailable() const {
        if (mPosition >= mBuffer.size()) {
            return 0;
        }
        return mBuffer.size() - mPosition;
    }

    size_t MemoryStream::Read(void *pData, size_t nSize) {
        if (pData == nullptr) {
            return 0;
        }
        const size_t kAvailable = mBuffer.size() - mPosition;
        const size_t kToCopy = (nSize < kAvailable) ? nSize : kAvailable;

        memcpy(pData, &mBuffer[mPosition], kToCopy);
        mPosition += kToCopy;

        return kToCopy;
    }

    bool MemoryStream::Flush() {
        mBuffer.clear();
        return true;
    }

    size_t MemoryStream::Write(const void *pData, size_t nSize) {
        if (pData == nullptr) {
            return 0;
        }

        if (mBuffer.size() < mPosition + nSize) {
            mBuffer.resize(mPosition + nSize);
        }

        memcpy(&mBuffer[mPosition], pData, nSize);
        mPosition += nSize;

        return nSize;
    }
}