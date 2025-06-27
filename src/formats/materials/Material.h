#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "MaterialParameter.h"
#include "../../EA/io/IStream.h"
#include "../../EA/ResourceMan/ResourceKey.h"

struct Material {
    std::vector<MaterialParameter> mParameters;
    EA::ResourceMan::Key mSelf;

    uint32_t mMaterialId;
    uint32_t mShaderId;

    static void Read(Material &instance, EA::IO::IStream *pStream);

    void Write(EA::IO::IStream *pStream);
};

#endif // MATERIAL_H
