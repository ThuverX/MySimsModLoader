//
// Created by exozg on 06/05/2025.
//

#ifndef CUSTOMSTREAM_H
#define CUSTOMSTREAM_H

#include <windows.h>
#include <cstdint>
#include <filesystem>

#include "Constants.h"
#include "IStream.h"

namespace EA::IO {
    class FileStream final : public IStream {
        std::filesystem::path path;
        HANDLE hFile = INVALID_HANDLE_VALUE;

    public:
        static constexpr uint32_t Type = 0xb7faadfe;
        explicit FileStream(const std::filesystem::path &path);

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

        std::filesystem::path GetPath();
    };
}

#endif //CUSTOMSTREAM_H
