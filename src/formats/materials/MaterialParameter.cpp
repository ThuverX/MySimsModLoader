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

    SEEK(stream, instance.offset + base_offset, EA::IO::PositionType::Begin);

    if (instance.type == Color) {
        if (instance.valueFieldCount >= 1) READ(stream, instance.colorValue.x);
        if (instance.valueFieldCount >= 2) READ(stream, instance.colorValue.y);
        if (instance.valueFieldCount >= 3) READ(stream, instance.colorValue.z);
        if (instance.valueFieldCount >= 4) READ(stream, instance.colorValue.w);
    } else if (instance.type == Value) {
        READ(stream, instance.intValue);
    } else if (instance.type == Key) {
        EA::ResourceMan::Key::Read(instance.keyValue, stream);
    }

    SEEK(stream, jump, EA::IO::PositionType::Begin);
}

void MaterialParameter::WriteHeader(EA::IO::IStream *stream) const {
    WRITE(stream, name);
    WRITE(stream, type);
    WRITE(stream, valueFieldCount);
    WRITE(stream, offset);
}

void MaterialParameter::WriteBody(EA::IO::IStream *stream) const {
    if (type == Color) {
        if (valueFieldCount >= 1) WRITE(stream, colorValue.x);
        if (valueFieldCount >= 2) WRITE(stream, colorValue.y);
        if (valueFieldCount >= 3) WRITE(stream, colorValue.z);
        if (valueFieldCount >= 4) WRITE(stream, colorValue.w);
    } else if (type == Value) {
        WRITE(stream, intValue);
    } else if (type == Key) {
        keyValue.Write(stream);
    }
}

uint32_t MaterialParameter::GetBodySize() const {
    uint32_t size = 0;
    if (type == Color) {
        if (valueFieldCount >= 1) size += sizeof(float);
        if (valueFieldCount >= 2) size += sizeof(float);
        if (valueFieldCount >= 3) size += sizeof(float);
        if (valueFieldCount >= 4) size += sizeof(float);
    } else if (type == Value) {
        size += sizeof(uint32_t);
    } else if (type == Key) {
        size += sizeof(EA::ResourceMan::Key);
    }
    return size;
}
