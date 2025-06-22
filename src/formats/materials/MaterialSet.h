//
// Created by exozg on 23/05/2025.
//

#ifndef MATERIALSET_H
#define MATERIALSET_H

#include "../../EA/ResourceMan/ResourceKey.h"

struct MTST {
    uint32_t mName;
    uint32_t mIndex;
    std::vector<uint32_t> mIndices;
};

struct MaterialSet {
    EA::ResourceMan::Key mSelf;
    std::vector<EA::ResourceMan::Key> mMaterials;
    MTST mMtst;

    static void Read(MaterialSet &instance, EA::IO::IStream *pStream);

    void Write(EA::IO::IStream *pStream);
};


#endif //MATERIALSET_H
