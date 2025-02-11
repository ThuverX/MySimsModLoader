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
    void RegisterReplacer(const std::string& path, const EA::ResourceMan::Key* key);
    void CreateDatabase(void* manager);

    [[nodiscard]] Asset* GetReplacerByKey(uint64_t instance, uint32_t group, uint32_t type) const;

    [[nodiscard]] std::vector<Asset *> GetReplacersByKey(uint64_t instance, uint32_t group, uint32_t type) const;

    [[nodiscard]] Asset* GetReplacerByPath(const std::wstring& path) const;

    std::vector<Asset *> GetReplacersByPath(const std::wstring &path) const;

    EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles* database;
private:
    std::vector<Asset*> replacers;
};



#endif //ASSETS_H
