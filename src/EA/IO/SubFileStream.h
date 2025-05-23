//
// Created by exozg on 16/05/2025.
//

#ifndef SUBFILESTREAM_H
#define SUBFILESTREAM_H
#include "IStream.h"


#include <filesystem>
#include <windows.h>

namespace EA::IO {
    class SubFileStream final : public IStream {
        std::filesystem::path path;
        HANDLE hFile = INVALID_HANDLE_VALUE;

        size_t begin;
        size_t size;
        size_t end;

    public:
        explicit SubFileStream(const std::filesystem::path &path, size_t offset, size_t size);

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
