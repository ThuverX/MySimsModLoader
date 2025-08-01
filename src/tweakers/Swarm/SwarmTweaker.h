//
// Created by exozg on 13/03/2025.
//

#ifndef SWARMTWEAKER_H
#define SWARMTWEAKER_H

#include "../Tweaker.h"

// Allows for adding new effects to the game.
class SwarmTweaker final : public Tweaker {
    std::vector<Msml::Core::Asset *> mSwarmsToAdd;

public:
    bool OnLoad(Msml::Core::Resource::CustomRecord &asset) override;

    bool OnRegister(Msml::Core::Asset &asset) override;
};


#endif //SWARMTWEAKER_H
