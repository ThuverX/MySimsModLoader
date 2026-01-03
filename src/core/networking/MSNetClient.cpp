//
// Created by exozg on 22/12/2025.
//

#include "MSNetClient.h"

#include "../system/Logger.h"

namespace Msml::Core::Networking
{
    void MSNetClient::OnConsume(Packet* packet)
    {
    }

    void MSNetClient::OnConnected()
    {
        MSML_LOG_INFO("MSNetClient OnConnected");
    }

    void MSNetClient::OnDisconnected(EDisconnectReason aReason)
    {
        MSML_LOG_INFO("MSNetClient OnDisconnected");
    }

    void MSNetClient::OnUpdate()
    {

    }
}
