#ifndef MATERIALPARAMETER_H
#define MATERIALPARAMETER_H

#include "../../EA/IO/IStream.h"
#include "../../EA/ResourceMan/ResourceKey.h"
#include "../primitives/Vector4.h"

enum class ValueType: uint32_t {
    kColor = 1,
    kValue = 2,
    kKey = 4
};

struct MaterialParameter {
    uint32_t mName;
    ValueType mType;
    uint32_t mValueFieldCount;
    uint32_t mOffset;

    Vector4 mColorValue;
    uint32_t mIntValue;
    EA::ResourceMan::Key mKeyValue;

    static void Read(MaterialParameter &instance, size_t kBaseOffset, EA::IO::IStream *pStream);

    void WriteHeader(EA::IO::IStream *pStream) const;

    void WriteBody(EA::IO::IStream *pStream) const;

    uint32_t GetBodySize() const;
};

#endif // MATERIALPARAMETER_H
