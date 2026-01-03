//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_SERVER_H
#define MYSIMSMODLOADER_SERVER_H
#include <chrono>

#include "enet.h"

// loosely based on https://github.com/tiltedphoques/TiltedConnect/tree/master

namespace Msml::Core::Networking
{
    class Packet;
    class Server
    {
    public:
        Server() noexcept;
        virtual ~Server();

        bool Host(uint16_t aPort, uint32_t aTickRate) noexcept;
        void Close() noexcept;

        void Update() noexcept;

        enum class EDisconnectReason : uint8_t
        {
            kUnknown,
            kQuit,
            kKicked,
            kBanned,
            kBadConnection,
            kTimedOut
        };

        virtual void OnUpdate() = 0;
        virtual void OnConsume(Packet* packet, ENetPeer* peer) = 0;
        virtual void OnConnection(ENetPeer* peer) = 0;
        virtual void OnDisconnection(ENetPeer* peer,
                                     EDisconnectReason aReason) = 0;

        void SendToAll(const Packet* apPacket) const noexcept;
        void Send(ENetPeer* peer, const Packet* apPacket) const noexcept;
        void Kick(ENetPeer* peer) noexcept;

        [[nodiscard]] uint16_t GetPort() const noexcept;
        [[nodiscard]] bool IsListening() const noexcept;
        [[nodiscard]] uint32_t GetClientCount() const noexcept;
        [[nodiscard]] uint32_t GetTickRate() const noexcept;
        [[nodiscard]] uint64_t GetTick() const noexcept;
        [[nodiscard]] bool IsAlive(ENetPeer* peer) const noexcept;

    private:

        void SynchronizeClientClocks(ENetPeer* aSpecificConnection = nullptr) const noexcept;

        ENetHost* mServer = {};
        uint32_t mTickRate;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastClockSyncTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdateTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_currentTick;
        std::chrono::milliseconds m_timeBetweenUpdates;
    };
}

#endif //MYSIMSMODLOADER_SERVER_H