//
// Created by exozg on 23/05/2025.
//

#ifndef MATERIALSETBUILDER_H
#define MATERIALSETBUILDER_H
#include "../materials/Material.h"
#include "../materials/MaterialSet.h"


class MaterialSetBuilder {
    MaterialSet mMaterialSet{};

public:
    MaterialSetBuilder &WithMaterial(const Material &kMaterial);

    MaterialSetBuilder &WithMaterial(EA::ResourceMan::Key kKey);

    MaterialSetBuilder &WithKey(EA::ResourceMan::Key kKey);

    MaterialSetBuilder &WithMTSTName(uint32_t kName);

    MaterialSetBuilder &WithMTSTDefaultIndex();

    MaterialSetBuilder &WithMTSTIndex(uint32_t index);

    MaterialSet Build();
};


#endif //MATERIALSETBUILDER_H
