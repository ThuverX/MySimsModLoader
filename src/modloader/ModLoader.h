//
// Created by exozg on 24/01/2025.
//

#ifndef MODLOADER_H
#define MODLOADER_H

#include <string>

#include "../util/Console.h"

#define MODS_PATH "mods/"
#define W_MODS_PATH L"mods/"

class ModLoader {
public:
    static ModLoader& GetInstance();
    void Initialize();
    std::string modulePath;
    void SoftCrash();
private:
    ModLoader();
    Console console;
};



#endif //MODLOADER_H
