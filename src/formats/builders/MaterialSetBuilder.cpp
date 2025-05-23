//
// Created by exozg on 23/05/2025.
//

#include "MaterialSetBuilder.h"

MaterialSetBuilder & MaterialSetBuilder::withMaterial(const Material &mat) {
    matSet.materials.push_back(mat.self);

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withMaterial(const EA::ResourceMan::Key key) {
    matSet.materials.push_back(key);

    return *this;
}

MaterialSet MaterialSetBuilder::build() {
    return matSet;
}
