//
// Created by exozg on 23/05/2025.
//

#include "MaterialSet.h"

void ReadMTST(MTST &mtst, EA::IO::IStream *pStream) {
    SKIP(pStream, 4); // MTST
    SKIP(pStream, 4); // MTST version
    READ(pStream, mtst.mName);
    READ(pStream, mtst.mIndex);
    READ_VALUE(pStream, uint32_t, count);
    for (uint32_t i = 0; i < count; i++) {
        uint32_t index = 0;
        READ(pStream, index);

        mtst.mIndices.push_back(index);
    }
}

void WriteMTST(MTST &mtst, EA::IO::IStream *pStream) {
    WRITE_STRING(pStream, "MTST");
    WRITE_VALUE(pStream, uint32_t, 1);
    WRITE(pStream, mtst.mName);
    WRITE(pStream, mtst.mIndex);
    WRITE_VALUE(pStream, uint32_t, mtst.mIndices.size());
    for (auto &index: mtst.mIndices) {
        WRITE(pStream, index);
    }
}

void MaterialSet::Read(MaterialSet &instance, EA::IO::IStream *pStream) {
    SKIP(pStream, 4); // 1
    SKIP(pStream, 4); // 1
    SKIP(pStream, 4); // 0

    READ_VALUE(pStream, uint32_t, numMaterials);

    SKIP(pStream, 4); // 1

    EA::ResourceMan::Key::Read(instance.mSelf, pStream);

    for (uint32_t i = 0; i < numMaterials; i++) {
        EA::ResourceMan::Key key{};
        EA::ResourceMan::Key::Read(key, pStream);

        instance.mMaterials.push_back(key);
    }

    SKIP(pStream, 4); // Header size
    SKIP(pStream, 4); // MTST size
    ReadMTST(instance.mMtst, pStream);
}

void MaterialSet::Write(EA::IO::IStream *pStream) {
    WRITE_VALUE(pStream, uint32_t, 1);
    WRITE_VALUE(pStream, uint32_t, 1);
    WRITE_VALUE(pStream, uint32_t, 0);

    WRITE_VALUE(pStream, uint32_t, mMaterials.size());

    WRITE_VALUE(pStream, uint32_t, 1);

    mSelf.Write(pStream);

    for (auto &material: mMaterials) {
        material.Write(pStream);
    }

    WRITE_VALUE(pStream, uint32_t, OFFSET(pStream) + 8);
    CREATE_HOLE(pStream, mtstSize, uint32_t);

    const size_t kMtstOffset = OFFSET(pStream);

    WriteMTST(mMtst, pStream);

    const uint32_t kMtstSize = OFFSET(pStream) - kMtstOffset;

    FILL_HOLE(pStream, mtstSize, kMtstSize);
}
