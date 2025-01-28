//
// Created by exozg on 24/01/2025.
//

#ifndef ASSETS_H
#define ASSETS_H

#include <vector>

#include "Asset.h"

class Assets {
public:
    static Assets& GetInstance();
    void Install();
    void RegisterAsset(std::string path, const EA::ResourceMan::Key* key);
    void CreateDatabase();

    Asset* GetAsset(uint64_t instance, uint32_t group, uint32_t type) const;
private:
    std::vector<Asset*> assets;
    EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles* database;
};



#endif //ASSETS_H
