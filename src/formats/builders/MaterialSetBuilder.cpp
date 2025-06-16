//
// Created by exozg on 23/05/2025.
//

#include "MaterialSetBuilder.h"

MaterialSetBuilder & MaterialSetBuilder::withMaterial(const Material &mat) {
    matSet.mtst.indices.emplace_back(matSet.mtst.index + matSet.mtst.indices.size());

    matSet.materials.emplace_back(mat.self);

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withMTSTName(uint32_t name) {
    matSet.mtst.name = name;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withMTSTIndex(uint32_t index) {
    matSet.mtst.index = index;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withMTSTDefaultIndex() {
    matSet.mtst.index = 805306369;

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withMaterial(const EA::ResourceMan::Key key) {
    matSet.materials.push_back(key);

    return *this;
}

MaterialSetBuilder & MaterialSetBuilder::withKey(EA::ResourceMan::Key key) {
    matSet.self = key;

    return *this;
}

MaterialSet MaterialSetBuilder::build() {
    return matSet;
}
