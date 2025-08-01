//
// Created by exozg on 24/01/2025.
//

#ifndef MODLOADER_H
#define MODLOADER_H

#include <filesystem>

#include "../system/Console.h"

namespace Msml::Core {
    class ModLoader {
        ModLoader();

        System::Console mConsole;
    public:
        static ModLoader &GetInstance();

        void Initialize();

        // Root game folder path
        std::filesystem::path mModulePath;

        static void Message(const std::string& message);
        static void MessageAndExit(const std::string& message);
    };
}

#endif //MODLOADER_H
