//
// Created by exozg on 16/05/2025.
//

#ifndef SUBFILESTREAM_H
#define SUBFILESTREAM_H
#include "IStream.h"


#include <filesystem>
#include <windows.h>

#include "FileStream.h"

namespace EA::IO {
    class SubFileStream final : public IStream {
        FileStream mFileStream;

        size_t mBegin;
        size_t mSize;
        size_t mEnd;

    public:
        static constexpr uint32_t kType = 0x1489eb2f;
        explicit SubFileStream(const std::filesystem::path &kPath, size_t kOffset, size_t kSize);

        [[nodiscard]] uint32_t GetType() const override;

        [[nodiscard]] AccessFlags GetAccessFlags() const override;

        [[nodiscard]] FileError GetState() const override;

        bool Close() override;

        [[nodiscard]] size_t GetSize() const override;

        bool SetSize(size_t size) override;

        [[nodiscard]] size_t GetPosition(PositionType positionType) const override;

        bool SetPosition(int distance, PositionType positionType) override;

        [[nodiscard]] size_t GetAvailable() const override;

        size_t Read(void *pData, size_t nSize) override;

        bool Flush() override;

        size_t Write(const void *pData, size_t nSize) override;
    };
}



#endif //SUBFILESTREAM_H
