#ifndef MATERIALPARAMETER_H
#define MATERIALPARAMETER_H

#include "MaterialParameterType.h"

struct MaterialParameter {
    MaterialParameterType type;
    // TODO: Add ExtraParamInfo
    uint32_t valueType;
    uint32_t valueFieldCount;
    uint32_t offset;

    static void Read(MaterialParameter &instance, void *data, size_t size);
};

#endif // MATERIALPARAMETER_H
