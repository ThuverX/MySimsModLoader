//
// Created by exozg on 13/03/2025.
//

#ifndef TWEAKER_H
#define TWEAKER_H

#include "../core/assets/Asset.h"
#include "../core/resource/CustomRecord.h"


class Tweaker {
public:
    Tweaker() {
        GetRegistry().push_back(this);
    }

    virtual bool OnLoad(Msml::Core::Resource::CustomRecord& asset) = 0;
    virtual bool OnRegister(Msml::Core::Asset& asset) = 0;
    virtual ~Tweaker() = default;

    static std::vector<Tweaker*>& GetRegistry() {
        static std::vector<Tweaker*> registry;
        return registry;
    }

    static void RegistryOnLoad(Msml::Core::Resource::CustomRecord* pAsset) {
        for (const auto &tweaker: GetRegistry()) {
            tweaker->OnLoad(*pAsset);
        }
    }

    static void RegistryOnRegister(Msml::Core::Asset* pAsset) {
        for (const auto &tweaker: GetRegistry()) {
            tweaker->OnRegister(*pAsset);
        }
    }
};

#endif //TWEAKER_H
