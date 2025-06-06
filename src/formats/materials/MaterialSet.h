//
// Created by exozg on 23/05/2025.
//

#ifndef MATERIALSET_H
#define MATERIALSET_H

#include "../../EA/ResourceMan/ResourceKey.h"

struct MTST {
    uint32_t name;
    uint32_t index;
    uint32_t count;
    std::vector<uint32_t> indices;
};

struct MaterialSet {
    uint32_t numMaterials;
    EA::ResourceMan::Key self;
    std::vector<EA::ResourceMan::Key> materials;
    uint32_t headerSize;
    uint32_t mtstSize;
    MTST mtst;

    static void Read(MaterialSet &instance, EA::IO::IStream* stream);
    void Write(EA::IO::IStream* stream);
};



#endif //MATERIALSET_H
