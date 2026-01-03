//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_PACKET_H
#define MYSIMSMODLOADER_PACKET_H
#include <cstdint>

namespace Msml::Core::Networking
{
    class Packet
    {
    public:
        Packet(uint32_t m_type) noexcept;
        explicit Packet(uint32_t m_type, uint32_t aSize) noexcept;
        Packet(void* data, uint32_t aSize) noexcept;
        virtual ~Packet() noexcept;

        [[nodiscard]] char* GetData() const noexcept;
        [[nodiscard]] char* Build() const noexcept;
        [[nodiscard]] uint32_t GetPacketSize() const noexcept;

        [[nodiscard]] uint32_t GetDataSize() const noexcept;

        [[nodiscard]] bool IsValid() const noexcept;

        [[nodiscard]] uint32_t GetType() const noexcept;

    private:

        char* m_pData;
        uint32_t m_size;
        uint32_t m_type{};
    };
}


#endif //MYSIMSMODLOADER_PACKET_H