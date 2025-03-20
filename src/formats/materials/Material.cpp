#include "Material.h"

#include <cstdint>
#include "../BinaryReader.h"
#include <iostream>

void Material::Read(Material &instance, void *data, size_t size) {
    BinaryReader reader(data, size);

    reader.Read<uint32_t>(); // 1
    reader.Read<uint32_t>(); // 1
    reader.Read<uint32_t>(); // 0
    reader.Read<uint32_t>(); // 0
    reader.Read<uint32_t>(); // 1

    // TODO: Give ResourceKey its self-contained Read method
    // This requires the current offset to be passed to the Read method
    instance.self.instance = reader.Read<uint64_t>();
    instance.self.type = reader.Read<uint32_t>();
    instance.self.group = reader.Read<uint32_t>();

    instance.headerSize = reader.Read<uint32_t>();
    instance.totalSize = reader.Read<uint32_t>();

    reader.Read(nullptr, 4); // MATD
    reader.Read<uint32_t>(); // MATD version 

    instance.materialHash = reader.Read<uint32_t>();
    instance.shaderHash = reader.Read<uint32_t>();

    instance.mtrlSize = reader.Read<uint32_t>();

    size_t mtrlOffset = reader.GetOffset();

    reader.Read(nullptr, 4); // MTRL
    reader.Read<uint32_t>(); // 0

    instance.dataSize = reader.Read<uint32_t>();
    instance.numParams = reader.Read<uint32_t>();

    for (uint32_t i = 0; i < instance.numParams; i++) {
        MaterialParameter parameter;
        // TODO: Read material parameter

    }
}
