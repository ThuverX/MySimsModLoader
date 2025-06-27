//
// Created by exozg on 01/06/2025.
//

#ifndef SKINTONEVARIANTGENERATOR_H
#define SKINTONEVARIANTGENERATOR_H

#include "../../formats/primitives/Vector4.h"
#include <string>

#include "DirectXTexP.h"
#include "../../EA/ResourceMan/IRecord.h"
#include "../../EA/ResourceMan/IResource.h"

constexpr int kVariantCount = 3;
constexpr Vector4 kVariant1 = {
    .x=211, .y=163, .z=140, .w=255
};

constexpr Vector4 kVariant2 = {
    .x=214, .y=154, .z=107, .w=255
};

constexpr Vector4 kVariant3 = {
    .x=165, .y=111, .z=66, .w=255
};

inline constexpr Vector4 kVariants[kVariantCount] = {
    kVariant1,
    kVariant2,
    kVariant3
};

namespace SkintoneVariantGenerator {
    bool CreateImage(const std::vector<uint8_t>& kBuffer, DirectX::ScratchImage& image);
    bool Generate(const std::string &kName, EA::ResourceMan::IRecord *pBaseLayer, EA::ResourceMan::IRecord* pMaskLayer);
};



#endif //SKINTONEVARIANTGENERATOR_H
