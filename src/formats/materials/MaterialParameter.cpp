//
// Created by exozg on 17/05/2025.
//

#include "MaterialParameter.h"

void MaterialParameter::Read(MaterialParameter &instance, const size_t kBaseOffset, EA::IO::IStream *pStream) {
    READ(pStream, instance.mName);
    READ(pStream, instance.mType);
    READ(pStream, instance.mValueFieldCount);
    READ(pStream, instance.mOffset);

    const size_t kJump = OFFSET(pStream);

    SEEK(pStream, instance.mOffset + kBaseOffset, EA::IO::PositionType::kBegin);

    if (instance.mType == ValueType::kColor) {
        if (instance.mValueFieldCount >= 1) { READ(pStream, instance.mColorValue.x); }
        if (instance.mValueFieldCount >= 2) { READ(pStream, instance.mColorValue.y); }
        if (instance.mValueFieldCount >= 3) { READ(pStream, instance.mColorValue.z); }
        if (instance.mValueFieldCount >= 4) { READ(pStream, instance.mColorValue.w); }
    } else if (instance.mType == ValueType::kValue) {
        READ(pStream, instance.mIntValue);
    } else if (instance.mType == ValueType::kKey) {
        EA::ResourceMan::Key::Read(instance.mKeyValue, pStream);
    }

    SEEK(pStream, kJump, EA::IO::PositionType::kBegin);
}

void MaterialParameter::WriteHeader(EA::IO::IStream *pStream) const {
    WRITE(pStream, mName);
    WRITE(pStream, mType);
    WRITE(pStream, mValueFieldCount);
    WRITE(pStream, mOffset);
}

void MaterialParameter::WriteBody(EA::IO::IStream *pStream) const {
    if (mType == ValueType::kColor) {
        if (mValueFieldCount >= 1) { WRITE(pStream, mColorValue.x); }
        if (mValueFieldCount >= 2) { WRITE(pStream, mColorValue.y); }
        if (mValueFieldCount >= 3) { WRITE(pStream, mColorValue.z); }
        if (mValueFieldCount >= 4) { WRITE(pStream, mColorValue.w); }
    } else if (mType == ValueType::kValue) {
        WRITE(pStream, mIntValue);
    } else if (mType == ValueType::kKey) {
        mKeyValue.Write(pStream);
    }
}

uint32_t MaterialParameter::GetBodySize() const {
    uint32_t size = 0;
    if (mType == ValueType::kColor) {
        if (mValueFieldCount >= 1) { size += sizeof(float); }
        if (mValueFieldCount >= 2) { size += sizeof(float); }
        if (mValueFieldCount >= 3) { size += sizeof(float); }
        if (mValueFieldCount >= 4) { size += sizeof(float); }
    } else if (mType == ValueType::kValue) {
        size += sizeof(uint32_t);
    } else if (mType == ValueType::kKey) {
        size += sizeof(EA::ResourceMan::Key);
    }
    return size;
}
