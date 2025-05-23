//
// Created by exozg on 17/05/2025.
//

#ifndef CLOTHINGTWEAKER_H
#define CLOTHINGTWEAKER_H
#include "../Tweaker.h"

#include <unordered_map>

// Allows for easily adding new clothing to the game

struct OutfitEntry {
    std::string context;
    std::string type;
    std::string design_mode;
    std::string gender;
    std::string category;
    std::string model;
};

class ClothingTweaker final : public Tweaker {
public:
    std::vector<OutfitEntry*> outfitsToAdd;
    std::unordered_map<EA::ResourceMan::Key, std::vector<EA::ResourceMan::Key>> materialsToAdd;

    bool OnLoad(msml::core::resource::CustomRecord &asset) override;
    bool OnRegister(msml::core::assets::Asset &asset) override;

private:
    void CreateMaterial(uint32_t group, const std::string &name, const std::string &texture, const int skin_id);
};

#endif //CLOTHINGTWEAKER_H
