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

    MaterialSet Build();
};


#endif //MATERIALSETBUILDER_H
