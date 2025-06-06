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

constexpr int VARIANT_COUNT = 3;
constexpr Vector4 variant1 = {
    211, 163, 140, 255
};

constexpr Vector4 variant2 = {
    214, 154, 107, 255
};

constexpr Vector4 variant3 = {
    165, 111, 66, 255
};

inline Vector4 variants[VARIANT_COUNT] = {
    variant1,
    variant2,
    variant3
};

namespace SkintoneVariantGenerator {
    bool CreateImage(const std::vector<uint8_t>& buffer, DirectX::ScratchImage& image);
    bool Generate(const std::string &name, EA::ResourceMan::IRecord *base_layer, EA::ResourceMan::IRecord* mask_layer);
};



#endif //SKINTONEVARIANTGENERATOR_H
