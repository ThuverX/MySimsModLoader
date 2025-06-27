//
// Created by exozg on 17/05/2025.
//

#ifndef CLOTHINGTWEAKER_H
#define CLOTHINGTWEAKER_H

#include "../Tweaker.h"

struct OutfitEntry {
    std::string mContext;
    std::string mType;
    std::string mDesignMode;
    std::string mGender;
    std::string mCategory;
    std::string mModel;
};

class ClothingTweaker final : public Tweaker {
    std::vector<OutfitEntry *> mOutfitsToAdd;

    static void CreateMaterial(uint32_t kGroup, const std::string &kType, const std::string &kName,
                               const std::string &kTexture, int kSkinId);

public:
    bool OnLoad(Msml::Core::Resource::CustomRecord &asset) override;

    bool OnRegister(Msml::Core::Asset &asset) override;
};

#endif //CLOTHINGTWEAKER_H
