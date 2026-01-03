//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_NETWORKWINDOW_H
#define MYSIMSMODLOADER_NETWORKWINDOW_H
#include "../DebugUI.h"
#include "../../networking/Network.h"

#include <cmath>

class NetworkWindow final : public DebugWindow {
public:
    std::string mName = "Network";

    NetworkWindow() = default;

    //Revo::GameObject::GameObject* g = nullptr;
    double mT =0;

    void draw() override {
        mT+=0.01;
        if (ImGui::Begin((mName + "##window_networkwindow").c_str(), &mIsVisible)) {
            if (ImGui::Button("Host"))
            {
                Msml::Core::Networking::Network::StartServer();
            }

            if (ImGui::Button("Connect"))
            {
                Msml::Core::Networking::Network::ConnectToServer();
            }

            if (ImGui::Button("Disconnect"))
            {
                Msml::Core::Networking::Network::Disconnect();
            }

            // if (ImGui::Button("Test"))
            // {
            //
            //     auto blockWorld = Revo::BlockWorld::gBlockWorld;
            //
            //     auto world = Revo::StateMachine::GetWorld(&(*blockWorld)->mStateMachine);
            //     EA::Math::Vector3 pos = {
            //         134,
            //         0,
            //         90
            //     };
            //
            //     EA::Math::Vector3 rot = {
            //         0,
            //         0,
            //         0
            //     };
            //     g = Revo::GameObjectFactory::Spawn(Revo::GameObjectFactory::Instance(),0, "TEST__", "ObjectDefs/Activity_RocketLaunch_Def.xml", &pos, &rot, nullptr);
            //
            //     if (g != nullptr)
            //     {
            //         Revo::World::AddObject(static_cast<Revo::World::World*>(world), g);
            //     }
            // }
        }

        /*if (g != nullptr)
        {
            EA::Math::Transform transform  = {};

            Revo::GameObject::GetTransform(g, &transform);
            transform.mPosition.mX = 134 + (std::sin(mT) * 10);
            transform.mPosition.mZ = 90 + (std::cos(mT) * 10);
            Revo::GameObject::SetTransform(g, &transform);
        }*/

        ImGui::End();
    };

    const std::string & getName() const override {
        return mName;
    };
};


#endif //MYSIMSMODLOADER_NETWORKWINDOW_H