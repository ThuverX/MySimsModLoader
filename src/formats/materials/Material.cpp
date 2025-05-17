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
