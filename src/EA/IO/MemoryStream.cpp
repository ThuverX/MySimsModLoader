//
// Created by exozg on 08/05/2025.
//

#include "MemoryStream.h"

namespace EA::IO {
    MemoryStream::MemoryStream() = default;

    MemoryStream::MemoryStream(const void *data, const size_t size) {
        buffer.resize(size);
        memcpy(buffer.data(), data, size);
    }

    uint32_t MemoryStream::GetType() const {
        return Type;
    }

    AccessFlags MemoryStream::GetAccessFlags() const {
        return AccessFlags::ReadWrite;
    }

    FileError MemoryStream::GetState() const {
        return FileError::Success;
    }

    bool MemoryStream::Close() {
        buffer.clear();
        return true;
    }

    size_t MemoryStream::GetSize() const {
        return buffer.size();
    }

    bool MemoryStream::SetSize(size_t size) {
        buffer.resize(size);
        return false;
    }

    size_t MemoryStream::GetPosition(PositionType positionType) const {
        if (positionType == PositionType::Begin) {
            return position;
        }

        if (positionType == PositionType::End) {
            return buffer.size() - position;
        }

        return 0;
    }

    bool MemoryStream::SetPosition(int distance, PositionType positionType) {
        if (distance > buffer.size()) {
            return false;
        }

        if (positionType == PositionType::Begin) {
            position = distance;
            return true;
        }

        if (positionType == PositionType::Current) {
            position += distance;
            return true;
        }

        if (positionType == PositionType::End) {
            position = buffer.size() - distance;
            return true;
        }

        return false;
    }

    size_t MemoryStream::GetAvailable() const {
        if (position >= buffer.size()) return 0;
        return buffer.size() - position;
    }

    size_t MemoryStream::Read(void *pData, size_t nSize) {
        if (pData == nullptr) return 0;
        const size_t available = buffer.size() - position;
        const size_t to_copy = (nSize < available) ? nSize : available;

        memcpy(pData, &buffer[position], to_copy);
        position += to_copy;

        return to_copy;
    }

    bool MemoryStream::Flush() {
        buffer.clear();
        return true;
    }

    size_t MemoryStream::Write(const void *pData, size_t nSize) {
        if (pData == nullptr) return 0;

        if (buffer.size() < position + nSize)
            buffer.resize(position + nSize);

        memcpy(&buffer[position], pData, nSize);
        position += nSize;

        return nSize;
    }
}