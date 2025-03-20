//
// Created by exozg on 13/03/2025.
//

#ifndef SWARMTWEAKER_H
#define SWARMTWEAKER_H

#include "../Tweaker.h"
#include "../../signatures/sigdef.h"


class SwarmTweaker : public Tweaker {
public:
    static EA::ResourceMan::Key RootSwarmKey;
    std::string path;
    void Apply(const EA::ResourceMan::Key& key) override;
};



#endif //SWARMTWEAKER_H
