//
// Created by exozg on 13/03/2025.
//

#include "SwarmTweaker.h"

#include "../../assets/Asset.h"
#include "../../util/FNV.h"

EA::ResourceMan::Key SwarmTweaker::RootSwarmKey = {
    .instance = FNV::FromString64("MySims"),
    .type = static_cast<uint32_t>(SWM),
    .group = 0,
};

void SwarmTweaker::Apply(const EA::ResourceMan::Key &key) {
}
