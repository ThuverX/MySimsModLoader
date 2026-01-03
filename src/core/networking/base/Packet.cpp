//
// Created by exozg on 22/12/2025.
//

#include "Packet.h"

#include <cstring>

namespace Msml::Core::Networking
{
    Packet::Packet(const uint32_t aType) noexcept
        : m_pData(nullptr)
          , m_size(0)
          , m_type(aType)
    {
    }

    Packet::Packet(const uint32_t aType, const uint32_t aSize) noexcept
        : m_pData(nullptr)
          , m_size(aSize)
          , m_type(aType)
    {
        m_pData = new char[m_size];
    }

    Packet::Packet(void* aData, const uint32_t aSize) noexcept
        : m_size(aSize)
    {
        m_pData = new char[aSize];
        std::memcpy(&m_pData[4], aData, aSize);
        std::memcpy(&m_type, aData, sizeof(m_type));
    }

    Packet::~Packet() noexcept
    {
        delete[] m_pData;
        m_pData = nullptr;
    }

    char* Packet::GetData() const noexcept
    {
        return m_pData;
    }

    char* Packet::Build() const noexcept
    {
        auto* const kData = new char[m_size + sizeof(m_type)];
        std::memcpy(kData, &m_type, sizeof(m_type));
        std::memcpy(&kData[4], m_pData, m_size);
        return kData;
    }

    uint32_t Packet::GetPacketSize() const noexcept
    {
        return m_size + 4;
    }

    uint32_t Packet::GetDataSize() const noexcept
    {
        return m_size;
    }

    bool Packet::IsValid() const noexcept
    {
        return m_pData != nullptr;
    }

    uint32_t Packet::GetType() const noexcept
    {
        return m_type;
    }
}
