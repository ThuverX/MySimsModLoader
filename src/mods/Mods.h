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
    void LoadHooks();
    std::vector<Mod*> mods;
private:
    explicit Mods(std::string rootPath);
    std::string rootPath;
};



#endif //MODS_H
