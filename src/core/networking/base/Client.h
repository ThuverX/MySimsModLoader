//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_CLIENT_H
#define MYSIMSMODLOADER_CLIENT_H
#include <string>

#include "enet.h"
#include "SynchronizedClock.h"

namespace Msml::Core::Networking
{
    class Packet;
    class Client
    {
    public:
        enum class EDisconnectReason : uint8_t
        {
            kTimeout,
            kLocalProblem,
            kKicked,
            kCannotResolve,
            kAborted,
            kNormal
        };

        Client() noexcept;
        virtual ~Client();

        bool Connect(const std::string& acEndpoint, uint16_t aPort) noexcept;
        void Close() noexcept;

        void Update() noexcept;

        virtual void OnConsume(Packet* packet) = 0;
        virtual void OnConnected() = 0;
        virtual void OnDisconnected(EDisconnectReason aReason) = 0;
        virtual void OnUpdate() = 0;

        void Send(const Packet* apPacket) const noexcept;

        [[nodiscard]] bool IsConnected() const noexcept;
        [[nodiscard]] const SynchronizedClock& GetClock() const noexcept;

    private:
        ENetHost* mClient = {};
        ENetPeer* mServerPeer = {};
        SynchronizedClock mClock;
    };
}


#endif //MYSIMSMODLOADER_CLIENT_H