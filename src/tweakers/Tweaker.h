﻿//
// Created by exozg on 13/03/2025.
//

#ifndef TWEAKER_H
#define TWEAKER_H

#include "../core/assets/Asset.h"
#include "../core/resource/CustomRecord.h"


class Tweaker {
public:
    Tweaker() {
        getRegistry().push_back(this);
    }

    virtual bool OnLoad(msml::core::resource::CustomRecord& asset) = 0;
    virtual bool OnRegister(msml::core::assets::Asset& asset) = 0;
    virtual ~Tweaker() = default;

    static std::vector<Tweaker*>& getRegistry() {
        static std::vector<Tweaker*> registry;
        return registry;
    }

    static void RegistryOnLoad(msml::core::resource::CustomRecord* asset) {
        for (const auto &tweaker: getRegistry()) {
            if (tweaker->OnLoad(*asset)) {
                break;
            }
        }
    }

    static void RegistryOnRegister(msml::core::assets::Asset* asset) {
        for (const auto &tweaker: getRegistry()) {
            if (tweaker->OnRegister(*asset)) {
                break;
            }
        }
    }
};

#endif //TWEAKER_H
