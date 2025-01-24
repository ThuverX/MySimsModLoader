//
// Created by exozg on 24/01/2025.
//

#ifndef ASSETS_H
#define ASSETS_H

#include <vector>

#include "Asset.h"
#include "../signatures/Signatures.h"

class Assets {
public:
    static Assets& GetInstance();
    void Install();
    void RegisterAsset(std::string path, const EA::ResourceMan::Key* key);
    Asset* GetAsset(uint64_t instance, uint32_t group, uint32_t type) const;
private:
    std::vector<Asset*> assets;
};



#endif //ASSETS_H
