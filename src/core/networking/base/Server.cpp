//
// Created by exozg on 22/12/2025.
//

#include "Server.h"

#include <thread>

#include "Packet.h"
#include "../../system/Logger.h"
#include "../packets/packets.h"

using namespace std::chrono;

#define MAX_CLIENTS 32

namespace Msml::Core::Networking
{
    Server::Server() noexcept
        : mTickRate(10)
        , m_lastClockSyncTime(0ns)
        , m_lastUpdateTime(0ns)
        , m_timeBetweenUpdates(100ms)
    {
        mServer = {};
    }

    Server::~Server()
    {
        enet_host_destroy(mServer);
    }

    bool Server::Host(const uint16_t aPort, uint32_t aTickRate) noexcept
    {
        Close();

        ENetAddress address = {};

        address.host = ENET_HOST_ANY;
        address.port = aPort;

        mServer = enet_host_create(&address, MAX_CLIENTS, 2, 0, 0);

        if (mServer == nullptr)
        {
            return false;
        }

        if (mTickRate == 0 && aTickRate == 0)
        {
            aTickRate = 10;
        }
        // If we pass 0, reuse the previously used tick rate
        else if (aTickRate == 0)
        {
            aTickRate = mTickRate;
        }

        mTickRate = aTickRate;

        // update time in MS
        m_timeBetweenUpdates = 1000ms / mTickRate;
        return IsListening();
    }

    void Server::Close() noexcept
    {
        if (IsListening())
        {
            enet_host_destroy(mServer);
        }
    }

    void Server::Update() noexcept
    {
        m_currentTick = high_resolution_clock::now();

        if (IsListening())
        {
            ENetEvent event;

            while (enet_host_service(mServer, &event, mTickRate) > 0)
            {
                switch (event.type)
                {
                    case ENET_EVENT_TYPE_RECEIVE:
                        {
                            auto* packet = new Packet(event.packet->data, event.packet->dataLength);
                            OnConsume(packet, event.peer);
                            enet_packet_destroy (event.packet);
                            break;
                        }
                    case ENET_EVENT_TYPE_NONE:
                        break;
                    case ENET_EVENT_TYPE_CONNECT:
                        OnConnection(event.peer);
                        break;
                    case ENET_EVENT_TYPE_DISCONNECT:
                        OnDisconnection(event.peer, EDisconnectReason::kQuit);
                        break;
                    case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                        OnDisconnection(event.peer, EDisconnectReason::kTimedOut);
                        break;
                }
            }
        }

        // Sync clocks every 10 seconds
        if (m_currentTick - m_lastClockSyncTime >= 10s)
        {
            m_lastClockSyncTime = m_currentTick;
            SynchronizeClientClocks();
        }

        if (m_currentTick - m_lastUpdateTime >= m_timeBetweenUpdates)
        {
            m_lastUpdateTime = m_currentTick;
            OnUpdate();
        }

        std::this_thread::sleep_for(2ms);
    }

    void Server::SendToAll(const Packet* apPacket) const noexcept
    {
        auto* packet = enet_packet_create(apPacket->Build(), apPacket->GetPacketSize(), ENET_PACKET_FLAG_RELIABLE);

        enet_host_broadcast(mServer, 0, packet);
    }

    void Server::Send(ENetPeer* peer, const Packet* apPacket) const noexcept
    {
        auto* packet = enet_packet_create(apPacket->Build(), apPacket->GetPacketSize(), ENET_PACKET_FLAG_RELIABLE);

        enet_peer_send(peer, 0, packet);
    }

    void Server::Kick(ENetPeer* peer) noexcept
    {
        OnDisconnection(peer, EDisconnectReason::kKicked);
        enet_peer_disconnect_now(peer, 0);
    }

    uint16_t Server::GetPort() const noexcept
    {
        return mServer->address.port;
    }

    bool Server::IsListening() const noexcept
    {
        return mServer != nullptr && mServer->socket != INVALID_SOCKET;
    }

    uint32_t Server::GetClientCount() const noexcept
    {
        return mServer->peerCount;
    }

    uint32_t Server::GetTickRate() const noexcept
    {
        return mTickRate;
    }

    uint64_t Server::GetTick() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_currentTick.time_since_epoch()).count();
    }

    bool Server::IsAlive(ENetPeer* peer) const noexcept
    {
        return peer->state != ENET_PEER_STATE_DISCONNECTED;
    }

    void Server::SynchronizeClientClocks(ENetPeer* aSpecificConnection) const noexcept
    {
        const auto kTime = GetTick();

        auto* packet = new Packet(static_cast<uint32_t>(PacketType::kServerTime), 8);
        std::memcpy(packet->GetData(), &kTime, 8);

        if(aSpecificConnection != nullptr)
        {
            Send(aSpecificConnection, packet);
        }
        else
        {
            SendToAll(packet);
        }
    }
}
