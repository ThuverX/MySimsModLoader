#include "Material.h"

void Material::Read(Material &instance, EA::IO::IStream* stream) {

    SKIP(stream, 4); // 1
    SKIP(stream, 4); // 1
    SKIP(stream, 4); // 0
    SKIP(stream, 4); // 0
    SKIP(stream, 4); // 1

    EA::ResourceMan::Key::Read(instance.self, stream);

    READ(stream, instance.headerSize);
    READ(stream, instance.totalSize);

    SKIP(stream, 4); // MATD
    SKIP(stream, 4); // MATD version

    READ(stream, instance.materialId);
    READ(stream, instance.shaderId);

    READ(stream, instance.mtrlSize);

    const size_t mtrlOffset = OFFSET(stream);

    SKIP(stream, 4); // MTRL
    SKIP(stream, 4); // MTRL version

    READ(stream, instance.dataSize);
    READ(stream, instance.numParams);

    for (uint32_t i = 0; i < instance.numParams; i++) {
        MaterialParameter parameter{};
        MaterialParameter::Read(parameter, mtrlOffset, stream);

        instance.parameters.push_back(parameter);
    }
}

void Material::Write(EA::IO::IStream *stream) {
    WRITE_VALUE(stream, uint32_t, 1);
    WRITE_VALUE(stream, uint32_t, 1);
    WRITE_VALUE(stream, uint32_t, 0);
    WRITE_VALUE(stream, uint32_t, 0);
    WRITE_VALUE(stream, uint32_t, 1);

    self.Write(stream);

    WRITE_VALUE(stream, uint32_t, OFFSET(stream) + 8);
    CREATE_HOLE(stream, totalSize, uint32_t);
    const size_t totalOffset = OFFSET(stream);

    WRITE_STRING(stream, "MATD");
    WRITE_VALUE(stream, uint32_t, 1);

    WRITE(stream, materialId);
    WRITE(stream, shaderId);

    CREATE_HOLE(stream, mtrlSize, uint32_t);
    const size_t mtrlOffset = OFFSET(stream);

    WRITE_STRING(stream, "MTRL");
    WRITE_VALUE(stream, uint32_t, 0);

    CREATE_HOLE(stream, dataSize, uint32_t);

    numParams = parameters.size();
    WRITE_VALUE(stream, uint32_t, numParams);

    uint32_t rolling_offset = OFFSET(stream) + 16 * numParams - mtrlOffset;

    for (auto & parameter : parameters) {
        parameter.offset = rolling_offset;
        rolling_offset += parameter.GetBodySize();
        parameter.WriteHeader(stream);
    }

    const size_t dataOffset = OFFSET(stream);

    for (auto & parameter : parameters) {
        parameter.WriteBody(stream);
    }

    const size_t endOffset = OFFSET(stream);

    FILL_HOLE(stream, dataSize, endOffset - dataOffset);
    FILL_HOLE(stream, totalSize, endOffset - totalOffset);
    FILL_HOLE(stream, mtrlSize, endOffset - mtrlOffset);
}
