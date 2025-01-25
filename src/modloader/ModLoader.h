//
// Created by exozg on 24/01/2025.
//

#ifndef MODLOADER_H
#define MODLOADER_H
#include "../util/Console.h"


class ModLoader {
public:
    static ModLoader& GetInstance();
    void Initialize();
private:
    void InstallLuaHooks();
    Console console;
};



#endif //MODLOADER_H
