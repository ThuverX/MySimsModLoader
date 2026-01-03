//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_PACKETS_H
#define MYSIMSMODLOADER_PACKETS_H
#include <cstdint>


namespace Msml::Core::Networking
{
    enum class PacketType: uint32_t
    {
        kServerTime
    };
}

#endif //MYSIMSMODLOADER_PACKETS_H
