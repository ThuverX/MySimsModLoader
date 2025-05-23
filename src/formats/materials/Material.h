#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "MaterialParameter.h"
#include "../../EA/io/IStream.h"
#include "../../EA/ResourceMan/ResourceKey.h"

struct Material {
    std::vector<MaterialParameter> parameters;
    EA::ResourceMan::Key self;
    uint32_t totalSize;
    uint32_t headerSize;
    uint32_t mtrlSize;
    uint32_t dataSize;

    uint32_t numParams;

    uint32_t materialId;
    uint32_t shaderId;

    static void Read(Material &instance, EA::IO::IStream* stream);
    void Write(EA::IO::IStream* stream);
};

#endif // MATERIAL_H
