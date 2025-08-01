//
// Created by exozg on 30/05/2025.
//

#ifndef MATERIALTWEAKER_H
#define MATERIALTWEAKER_H
#include "../Tweaker.h"


class MaterialTweaker : public Tweaker {
    std::unordered_map<EA::ResourceMan::Key, std::vector<EA::ResourceMan::Key> > mMaterialsToAddToSets;

public:
    bool OnLoad(Msml::Core::Resource::CustomRecord &asset) override;

    bool OnRegister(Msml::Core::Asset &asset) override;

    void AddMaterial(const EA::ResourceMan::Key &kSetKey, const EA::ResourceMan::Key &kMaterialKey);
};

#endif //MATERIALTWEAKER_H
