#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <cstdint>

#include "../ResourceKey.h"
#include "MaterialParameter.h"

// TODO: Add shader IDs

struct Material {
    std::vector<MaterialParameter> parameters;
    ResourceKey self;
    uint32_t totalSize;
    uint32_t headerSize;
    uint32_t mtrlSize;
    uint32_t dataSize;

    uint32_t numParams;

    uint32_t materialHash;
    uint32_t shaderHash;

    // TODO: add an easy way to fetch the shader name

    static void Read(Material &instance, void *data, size_t size);
};

#endif // MATERIAL_H
