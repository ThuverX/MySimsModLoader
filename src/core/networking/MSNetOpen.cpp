//
// Created by exozg on 22/12/2025.
//

#include "MSNetOpen.h"

#include "../system/Logger.h"

namespace Msml::Core::Networking
{
    void MSNetOpen::OnUpdate()
    {

    }

    void MSNetOpen::OnConsume(Packet* packet, ENetPeer* peer)
    {
    }

    void MSNetOpen::OnConnection(ENetPeer* peer)
    {
        MSML_LOG_INFO("MSNetOpen OnConnection");
    }

    void MSNetOpen::OnDisconnection(ENetPeer* peer, EDisconnectReason aReason)
    {
        MSML_LOG_INFO("MSNetOpen OnDisconnection");
    }
}
