//
// Created by exozg on 16/05/2025.
//

#include "SubFileStream.h"

// TODO: we should really have tests for these and the other stream functions
namespace EA::IO {
    SubFileStream::SubFileStream(const std::filesystem::path &path, const size_t kOffset, const size_t kSize): mFileStream(path), mBegin(kOffset), mSize(kSize), mEnd(mBegin + kSize) {
        mFileStream.SetPosition(mBegin, PositionType::kBegin);
    }

    uint32_t SubFileStream::GetType() const {
        return kType;
    }

    AccessFlags SubFileStream::GetAccessFlags() const {
        return mFileStream.GetAccessFlags();
    }

    FileError SubFileStream::GetState() const {
        return mFileStream.GetState();
    }

    bool SubFileStream::Close() {
        return mFileStream.Close();
    }

    size_t SubFileStream::GetSize() const {
        return mSize;
    }

    bool SubFileStream::SetSize(size_t size) {
        mSize = size;
        mEnd = mBegin + size;
        return true;
    }

    size_t SubFileStream::GetPosition(PositionType positionType) const {
        if (positionType == PositionType::kEnd) {
            return GetSize() - mFileStream.GetPosition(PositionType::kBegin) - mBegin;
        }

        return mFileStream.GetPosition(PositionType::kBegin) - mBegin;
    }

    bool SubFileStream::SetPosition(int distance, PositionType positionType) {
        switch (positionType) {
            case PositionType::kBegin:
                return mFileStream.SetPosition(mBegin + distance, PositionType::kBegin);
            case PositionType::kEnd:
                return mFileStream.SetPosition(mEnd - distance, PositionType::kEnd);
            case PositionType::kCurrent:
                return mFileStream.SetPosition(distance, PositionType::kBegin);
        }

        return false;
    }

    size_t SubFileStream::GetAvailable() const {
        const size_t kSize = GetSize();
        const size_t kPos = GetPosition(PositionType::kBegin);
        return (kPos <= kSize) ? (kSize - kPos) : -1;
    }

    size_t SubFileStream::Read(void *pData, size_t nSize) {
        return mFileStream.Read(pData, nSize);
    }

    bool SubFileStream::Flush() {
        return mFileStream.Flush();

    }

    size_t SubFileStream::Write(const void *pData, size_t nSize) {
        return mFileStream.Write(pData, nSize);
    }
}
