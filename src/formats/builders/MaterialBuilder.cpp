//
// Created by exozg on 23/05/2025.
//

#include "MaterialBuilder.h"

ShaderType GetShaderType(const std::string &type) {
    if (const auto kIterator = kStringToEnum.find(type); kIterator != kStringToEnum.end()) {
        return kIterator->second;
    }
    return ShaderType::lambert;
}

std::string GetShaderName(const uint32_t kValue) {
    for (const auto &[name, enumValue]: kStringToEnum) {
        if (static_cast<uint32_t>(enumValue) == kValue) {
            return name;
        }
    }

    return "unknown";
}

MaterialBuilder &MaterialBuilder::WithShader(ShaderType type) {
    mMaterial.mShaderId = static_cast<uint32_t>(type);

    return *this;
}

MaterialBuilder &MaterialBuilder::WithKey(const EA::ResourceMan::Key &key) {
    mMaterial.mSelf = key;
    mMaterial.mMaterialId = static_cast<uint32_t>(key.mInstance & 0xFFFFFFFF);
    return *this;
}

MaterialBuilder &MaterialBuilder::WithColorParameter(const uint32_t kName, const float x, const float y, const float z,
                                                     const float w) {
    MaterialParameter param{};
    param.mName = kName;
    param.mType = ValueType::kColor;
    param.mValueFieldCount = 4;
    param.mColorValue = {.x = x, .y = y, .z = z, .w = w};
    mMaterial.mParameters.push_back(param);

    return *this;
}

MaterialBuilder &MaterialBuilder::WithValueParameter(const uint32_t kName, const uint32_t kValue) {
    MaterialParameter param{};
    param.mName = kName;
    param.mType = ValueType::kValue;
    param.mValueFieldCount = 1;
    param.mIntValue = kValue;
    mMaterial.mParameters.push_back(param);

    return *this;
}

MaterialBuilder &MaterialBuilder::WithKeyParameter(const uint32_t kName, const EA::ResourceMan::Key kKey) {
    MaterialParameter param{};
    param.mName = kName;
    param.mType = ValueType::kKey;
    param.mValueFieldCount = 4;
    param.mKeyValue = kKey;
    mMaterial.mParameters.push_back(param);

    return *this;
}

Material MaterialBuilder::Build() {
    return mMaterial;
}
