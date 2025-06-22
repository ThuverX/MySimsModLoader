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

MaterialSet MaterialSetBuilder::Build() {
    return mMaterialSet;
}
