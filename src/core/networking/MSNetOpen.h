//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_MSNETOPEN_H
#define MYSIMSMODLOADER_MSNETOPEN_H
#include "base/Server.h"

namespace Msml::Core::Networking
{
    class MSNetOpen : public Server
    {
    public:
        void OnUpdate() override;
        void OnConsume(Packet* packet, ENetPeer* peer) override;
        void OnConnection(ENetPeer* peer) override;
        void OnDisconnection(ENetPeer* peer, EDisconnectReason aReason) override;
    };
}


#endif //MYSIMSMODLOADER_MSNETOPEN_H