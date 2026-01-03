//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_MSNETCLIENT_H
#define MYSIMSMODLOADER_MSNETCLIENT_H
#include "base/Client.h"

namespace Msml::Core::Networking
{
    class MSNetClient : public Client
    {
    public:
        void OnConsume(Packet* packet) override;
        void OnConnected() override;
        void OnDisconnected(EDisconnectReason aReason) override;
        void OnUpdate() override;
    };
}


#endif //MYSIMSMODLOADER_MSNETCLIENT_H