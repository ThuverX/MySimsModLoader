//
// Created by exozg on 22/12/2025.
//
#define ENET_IMPLEMENTATION
#include "Network.h"

#include "enet.h"
#include "MSNetOpen.h"
#include "../system/Logger.h"
#include <thread>
#include <queue>
#include <mutex>

#include "MSNetClient.h"

#define MSML_PORT 9642
#define MSML_TICK_RATE 16

namespace Msml::Core::Networking
{
    enum class ThreadMessageType: uint32_t
    {
        kStartServer,
        kConnectToServer,
        kDisconnect,
        kShutdown
    };

    struct ConnectToServerOption
    {
        std::string ip;
        uint16_t port;
    };

    struct ThreadMessage
    {
        ThreadMessageType type;
        void* data;
        size_t size;
    };

    std::queue<ThreadMessage> gMessageQueue;
    std::mutex gQueueMutex;

    void PushMessage(ThreadMessage msg)
    {
        std::scoped_lock lock(gQueueMutex);
        gMessageQueue.emplace(msg);
    }

    void ServerThread()
    {
        if (enet_initialize() != 0)
        {
            MSML_LOG_ERROR("Failed to init ENet");
            return;
        }

        MSNetOpen server;
        MSNetClient client;
        bool isRunning = true;

        while (isRunning)
        {
            if (server.IsListening())
            {
                server.Update();
            }

            if (client.IsConnected())
            {
                client.Update();
            }

            std::scoped_lock lock(gQueueMutex);
            while (!gMessageQueue.empty())
            {
                const ThreadMessage kMsg = gMessageQueue.front();
                gMessageQueue.pop();

                switch (kMsg.type)
                {
                case ThreadMessageType::kStartServer:
                    {
                        if (!server.Host(MSML_PORT, MSML_TICK_RATE))
                        {
                            MSML_LOG_ERROR("Failed to start server");
                            return;
                        }
                        MSML_LOG_INFO("Started server on %i", MSML_PORT);
                        break;
                    }
                case ThreadMessageType::kConnectToServer:
                    {
                        auto* option = static_cast<ConnectToServerOption*>(kMsg.data);
                        if (!client.Connect(option->ip, option->port))
                        {
                            MSML_LOG_ERROR("Failed to connect to server %s:%i", option->ip.c_str(), option->port);
                        }
                        MSML_LOG_INFO("Connected to server");
                        break;
                    }
                case ThreadMessageType::kDisconnect:
                    {
                        MSML_LOG_INFO("Disconnecting...");
                        server.Close();
                        client.Close();
                        break;
                    }
                case ThreadMessageType::kShutdown:
                    {
                        isRunning = false;
                        break;
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        enet_deinitialize();
    }

    void Network::StartServer()
    {
        PushMessage({
            .type = ThreadMessageType::kStartServer,
            .data = nullptr,
        });
    }

    void Network::Disconnect()
    {
        PushMessage({
            .type = ThreadMessageType::kDisconnect,
            .data = nullptr,
        });
    }

    void Network::ConnectToServer()
    {
        ConnectToServerOption option = {
            .ip = "127.0.0.1",
            .port = MSML_PORT,
        };

        PushMessage({
            .type = ThreadMessageType::kConnectToServer,
            .data = &option,
        });
    }

    void Network::Initialize()
    {
        std::thread worker(ServerThread);
        worker.detach();
    }

    void Network::Shutdown()
    {
        PushMessage({
            .type = ThreadMessageType::kShutdown,
            .data = nullptr
        });
    }
}
