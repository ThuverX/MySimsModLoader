//
// Created by exozg on 13/03/2025.
//

#ifndef SWARMTWEAKER_H
#define SWARMTWEAKER_H

#include "../Tweaker.h"

// Allows for adding new effects to the game.
class SwarmTweaker final : public Tweaker {
public:
    std::vector<msml::core::assets::Asset*> swarmsToAdd;
    bool OnLoad(msml::core::resource::CustomRecord &asset) override;
    bool OnRegister(msml::core::assets::Asset &asset) override;
};



#endif //SWARMTWEAKER_H
