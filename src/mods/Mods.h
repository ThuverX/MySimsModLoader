//
// Created by exozg on 24/01/2025.
//

#ifndef MODS_H
#define MODS_H

#include <vector>

#include "Mod.h"

class Mods {
public:
    static Mods& GetInstance();
    void Find();
    void LoadAssets();
    void LoadHooks();
private:
    explicit Mods(std::string rootPath);
    std::string rootPath;
    std::vector<Mod*> mods;
};



#endif //MODS_H
