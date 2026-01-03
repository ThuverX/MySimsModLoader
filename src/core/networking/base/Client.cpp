//
// Created by exozg on 22/12/2025.
//

#include "Client.h"

#include "Packet.h"
#include "../packets/packets.h"

namespace Msml::Core::Networking
{
    Client::Client() noexcept
    {
        mClient = {};
        mServerPeer = {};
        mClient = enet_host_create(NULL, 1, 2, 0, 0);
    }

    Client::~Client()
    {
        enet_host_destroy(mClient);
    }

    bool Client::Connect(const std::string& acEndpoint, uint16_t aPort) noexcept
    {
        ENetAddress address = {};
        enet_address_set_host(&address, acEndpoint.c_str());
        address.port = aPort;

        mServerPeer = enet_host_connect(mClient, &address, 2, 0);

        ENetEvent event = {};

        if (enet_host_service(mClient, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            return true;
        }

        enet_peer_reset(mServerPeer);
        return false;
    }

    void Client::Close() noexcept
    {
        if (mServerPeer == nullptr) { return; }
        OnDisconnected(EDisconnectReason::kAborted);
        enet_peer_disconnect(mServerPeer, 0);
    }

    void Client::Update() noexcept
    {
        mClock.Update();

        ENetEvent event = {};
        while (enet_host_service(mClient, &event, 3000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                {
                    auto* packet = new Packet(event.packet->data, event.packet->dataLength);

                    OnConsume(packet);

                    if (packet->GetType() == static_cast<uint32_t>(PacketType::kServerTime))
                    {
                        uint64_t cServerTime = 0;
                        const auto kCWasSynchronized = GetClock().IsSynchronized();

                        mClock.Synchronize(cServerTime, event.peer->roundTripTime);

                        if (!kCWasSynchronized)
                        {
                            OnConnected();
                        }
                    }

                    enet_packet_destroy(event.packet);
                    break;
                }
            case ENET_EVENT_TYPE_DISCONNECT:
                OnDisconnected(EDisconnectReason::kAborted);
                break;
            case ENET_EVENT_TYPE_NONE:
                break;
            case ENET_EVENT_TYPE_CONNECT:
                break;
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                OnDisconnected(EDisconnectReason::kTimeout);
                break;
            }
        }

        OnUpdate();
    }

    void Client::Send(const Packet* apPacket) const noexcept
    {
        auto* packet = enet_packet_create(apPacket->Build(), apPacket->GetPacketSize(), ENET_PACKET_FLAG_RELIABLE);

        enet_peer_send(mServerPeer, 0, packet);
    }

    bool Client::IsConnected() const noexcept
    {
        return mServerPeer != nullptr;
    }

    const SynchronizedClock& Client::GetClock() const noexcept
    {
        return mClock;
    }
}
