//
// Created by exozg on 17/05/2025.
//

#include "MaterialParameter.h"

void MaterialParameter::Read(MaterialParameter &instance, size_t base_offset, EA::IO::IStream *stream) {
    READ(stream, instance.name);
    READ(stream, instance.type);
    READ(stream, instance.valueFieldCount);
    READ(stream, instance.offset);

    const size_t jump = OFFSET(stream);

    SEEK(stream, jump + base_offset, EA::IO::PositionType::Begin);

    if (instance.type == Color) {
        if (instance.valueFieldCount <= 1) READ(stream, instance.colorValue.x);
        if (instance.valueFieldCount <= 2) READ(stream, instance.colorValue.x);
        if (instance.valueFieldCount <= 3) READ(stream, instance.colorValue.x);
        if (instance.valueFieldCount <= 4) READ(stream, instance.colorValue.x);
    } else if (instance.type == Value) {
        READ(stream, instance.intValue);
    } else if (instance.type == Key) {
        EA::ResourceMan::Key::Read(instance.keyValue, stream);
    }

    SEEK(stream, jump, EA::IO::PositionType::Begin);
}
