//
// Created by exozg on 23/05/2025.
//

#ifndef MATERIALSETBUILDER_H
#define MATERIALSETBUILDER_H
#include "../materials/Material.h"
#include "../materials/MaterialSet.h"


class MaterialSetBuilder {
    MaterialSet matSet{};
public:
    MaterialSetBuilder& withMaterial(const Material& mat);
    MaterialSetBuilder& withMTSTName(uint32_t name);
    MaterialSetBuilder& withMTSTIndex(uint32_t index);
    MaterialSetBuilder& withMTSTDefaultIndex();
    MaterialSetBuilder& withMaterial(EA::ResourceMan::Key key);
    MaterialSetBuilder& withKey(EA::ResourceMan::Key key);

    MaterialSet build();
};



#endif //MATERIALSETBUILDER_H
