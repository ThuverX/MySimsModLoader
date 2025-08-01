//
// Created by exozg on 23/05/2025.
//

#include "MaterialSetBuilder.h"

MaterialSetBuilder &MaterialSetBuilder::WithMaterial(const Material &kMaterial) {
    mMaterialSet.mMaterials.push_back(kMaterial.mSelf);

    return *this;
}

MaterialSetBuilder &MaterialSetBuilder::WithMaterial(const EA::ResourceMan::Key kKey) {
    mMaterialSet.mMaterials.push_back(kKey);

    return *this;
}

MaterialSetBuilder &MaterialSetBuilder::WithKey(const EA::ResourceMan::Key kKey) {
    mMaterialSet.mSelf = kKey;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::WithMTSTName(const uint32_t kName) {
    mMaterialSet.mMtst.mName = kName;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::WithMTSTIndex(const uint32_t kIndex) {
    mMaterialSet.mMtst.mIndex = kIndex;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::WithMTSTDefaultIndex() {
    mMaterialSet.mMtst.mIndex = 805306369;

    return *this;
}


MaterialSet MaterialSetBuilder::Build() {
    return mMaterialSet;
}
