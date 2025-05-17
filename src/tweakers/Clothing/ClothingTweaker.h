//
// Created by exozg on 17/05/2025.
//

#ifndef CLOTHINGTWEAKER_H
#define CLOTHINGTWEAKER_H
#include "../Tweaker.h"


// Allows for easily adding new clothing to the game, wip
class ClothingTweaker final : public Tweaker {
public:
    bool OnLoad(msml::core::resource::CustomRecord &asset) override;
    bool OnRegister(msml::core::assets::Asset &asset) override;
};



#endif //CLOTHINGTWEAKER_H
