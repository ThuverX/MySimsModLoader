//
// Created by exozg on 24/01/2025.
//

#ifndef MODLOADER_H
#define MODLOADER_H

#include <filesystem>

#include "../system/Console.h"

#define MODS_PATH "mods/"
#define W_MODS_PATH L"mods/"

#ifdef _WIN64
#define DATA_PATH "data"
#define W_DATA_PATH L"data"
#else
#define DATA_PATH "../SimsRevData"
#define W_DATA_PATH L"../SimsRevData"
#endif

namespace Msml::Core {
    class ModLoader {
        ModLoader();

        System::Console mConsole;
    public:
        static ModLoader &GetInstance();

        void Initialize();

        // Root game folder path
        std::filesystem::path mModulePath;

        static void SoftCrash();
    };
}

#endif //MODLOADER_H
