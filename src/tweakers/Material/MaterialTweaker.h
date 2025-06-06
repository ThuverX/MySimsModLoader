//
// Created by exozg on 30/05/2025.
//

#ifndef MATERIALTWEAKER_H
#define MATERIALTWEAKER_H
#include "../Tweaker.h"


class MaterialTweaker : public Tweaker {
public:
    std::unordered_map<EA::ResourceMan::Key, std::vector<EA::ResourceMan::Key>> materialsToAddToSets;

    bool OnLoad(msml::core::resource::CustomRecord &asset) override;
    bool OnRegister(msml::core::assets::Asset &asset) override;

    void AddMaterial(const EA::ResourceMan::Key& setKey, const EA::ResourceMan::Key& materialKey);

};




#endif //MATERIALTWEAKER_H
