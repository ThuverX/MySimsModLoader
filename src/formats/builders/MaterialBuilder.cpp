//
// Created by exozg on 23/05/2025.
//

#include "MaterialBuilder.h"

MaterialBuilder & MaterialBuilder::withShader(ShaderType type) {
    mat.shaderId = static_cast<uint32_t>(type);

    return *this;
}

MaterialBuilder & MaterialBuilder::withKey(const EA::ResourceMan::Key &key) {
    mat.self.instance = key.instance;
    mat.self.type = key.type;
    mat.self.group = key.group;
    mat.materialId = static_cast<uint32_t>(key.instance & 0xFFFFFFFF);
    return *this;
}

MaterialBuilder & MaterialBuilder::withColorParameter(const uint32_t name, const float x, const float y, const float z, const float w) {
    MaterialParameter param{};
    param.name = name;
    param.type = Color;
    param.valueFieldCount = 4;
    param.colorValue = {x, y, z, w};
    mat.parameters.push_back(param);

    return *this;
}

MaterialBuilder & MaterialBuilder::withValueParameter(const uint32_t name, const int value) {
    MaterialParameter param{};
    param.name = name;
    param.type = Value;
    param.valueFieldCount = 1;
    param.intValue = value;
    mat.parameters.push_back(param);

    return *this;
}

MaterialBuilder & MaterialBuilder::withKeyParameter(const uint32_t name, const EA::ResourceMan::Key key) {
    MaterialParameter param{};
    param.name = name;
    param.type = Key;
    param.valueFieldCount = 4;
    param.keyValue = key;
    mat.parameters.push_back(param);

    return *this;
}

Material MaterialBuilder::build() {
    return mat;
}
