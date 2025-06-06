//
// Created by exozg on 23/05/2025.
//

#include "MaterialSet.h"

void ReadMTST(MTST &mtst, EA::IO::IStream *stream) {
    SKIP(stream, 4); // MTST
    SKIP(stream, 4); // MTST version
    READ(stream, mtst.name);
    READ(stream, mtst.index);
    READ(stream, mtst.count);
    for (uint32_t i = 0; i < mtst.count; i++) {
        uint32_t index;
        READ(stream, index);

        mtst.indices.push_back(index);
    }
}

void WriteMTST(MTST &mtst, EA::IO::IStream *stream) {
    WRITE_STRING(stream, "MTST");
    WRITE_VALUE(stream, uint32_t, 1);
    WRITE(stream, mtst.name);
    WRITE(stream, mtst.index);
    mtst.count = mtst.indices.size();
    WRITE(stream, mtst.count);
    for (auto & index : mtst.indices) {
        WRITE(stream, index);
    }
}

void MaterialSet::Read(MaterialSet &instance, EA::IO::IStream *stream) {
    SKIP(stream, 4); // 1
    SKIP(stream, 4); // 1
    SKIP(stream, 4); // 0

    READ(stream, instance.numMaterials);

    SKIP(stream, 4); // 1

    EA::ResourceMan::Key::Read(instance.self, stream);

    for (uint32_t i = 0; i < instance.numMaterials; i++) {
        EA::ResourceMan::Key key{};
        EA::ResourceMan::Key::Read(key, stream);

        instance.materials.push_back(key);
    }

    READ(stream, instance.headerSize);
    READ(stream, instance.mtstSize);
    ReadMTST(instance.mtst, stream);
}

void MaterialSet::Write(EA::IO::IStream *stream) {
    WRITE_VALUE(stream, uint32_t, 1);
    WRITE_VALUE(stream, uint32_t, 1);
    WRITE_VALUE(stream, uint32_t, 0);

    numMaterials = materials.size();
    WRITE_VALUE(stream, uint32_t, numMaterials);

    WRITE_VALUE(stream, uint32_t, 1);

    self.Write(stream);

    for (auto & material : materials) {
        material.Write(stream);
    }

    WRITE_VALUE(stream, uint32_t, OFFSET(stream) + 8);
    CREATE_HOLE(stream, mtstSize, uint32_t);

    const size_t mtstOffset = OFFSET(stream);

    WriteMTST(mtst, stream);

    const uint32_t mtstSize = OFFSET(stream) - mtstOffset;

    FILL_HOLE(stream, mtstSize, mtstSize);
}
