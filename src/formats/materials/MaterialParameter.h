#ifndef MATERIALPARAMETER_H
#define MATERIALPARAMETER_H
#include "../../EA/IO/IStream.h"
#include "../../EA/ResourceMan/ResourceKey.h"
#include "../primitives/Vector4.h"

enum ValueType: uint32_t {
    Color = 1,
    Value = 2,
    Key = 4
};

struct MaterialParameter {
    uint32_t name;
    uint32_t type;
    uint32_t valueFieldCount;
    uint32_t offset;

    Vector4 colorValue;
    uint32_t intValue;
    EA::ResourceMan::Key keyValue;

    static void Read(MaterialParameter &instance, size_t base_offset, EA::IO::IStream* stream);
    void WriteHeader(EA::IO::IStream* stream) const;
    void WriteBody(EA::IO::IStream* stream) const;
    uint32_t GetBodySize() const;
};

#endif // MATERIALPARAMETER_H
