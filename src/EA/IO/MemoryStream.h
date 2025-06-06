//
// Created by exozg on 08/05/2025.
//

#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include "IStream.h"
#include <vector>

#include "Constants.h"

namespace EA::IO {
    class MemoryStream : public IStream {
        size_t position = 0;
        std::vector<uint8_t> buffer;

    public:
        static constexpr uint32_t Type = 0xe41cb215;

        MemoryStream();
        MemoryStream(const void* data, size_t size);

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


#endif //MEMORYSTREAM_H
