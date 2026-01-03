//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_NETWORK_H
#define MYSIMSMODLOADER_NETWORK_H

namespace Msml::Core::Networking
{
    class Network
    {
    public:
        static void StartServer();
        static void Disconnect();
        static void ConnectToServer();
        static void Initialize();
        static void Shutdown();
    };
}


#endif //MYSIMSMODLOADER_NETWORK_H