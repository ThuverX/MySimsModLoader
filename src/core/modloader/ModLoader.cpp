//
// Created by exozg on 24/01/2025.
//

#include <windows.h>

#include "ModLoader.h"

#include "Mods.h"
#include "../../Version.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"

#include "../hooks/LuaHook.h"
#include "../hooks/ArgscriptHook.h"
#include "../resource/IdResolver.h"
#include "../signatures/Signatures.h"
#include "../system/Logger.h"

namespace Msml::Core {
    ModLoader &ModLoader::GetInstance() {
        static ModLoader instance;
        return instance;
    }

    void ModLoader::Initialize() {
        System::Logger::sModule = mModulePath;

        bool consoleEnabled = false;

#ifndef NDEBUG
        consoleEnabled = true;
#endif

        if (IsDebuggerPresent() != 0) {
            consoleEnabled = false;
        }

        if (consoleEnabled) {
            mConsole.Enable();
        }

        MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);

        Hooks::Initialize();
        if (!Signatures::GetInstance().SearchAll()) {
            // If we failed to get any signatures, we quit
#ifndef NDEBUG
            MSML_LOG_ERROR("Failed to find addresses for some functions, did you write the signature correctly?");
            Hooks::Uninitialize();

            SoftCrash();
#else
            MSML_LOG_ERROR("Failed to find functions. Please report this on github and include your log file at");
            MSML_LOG_ERROR("https://github.com/ThuverX/MySimsModLoader/issues\n");
            MSML_LOG_INFO("NO MODS WILL BE LOADED");
            Hooks::Uninitialize();
            return;
#endif
        }

        Resource::IdResolver::GetInstance().Load("./hashes.bin");
        Modloader::Mods::GetInstance().Find();
        Assets::Install();
        Assets::GetInstance().CreateDatabase();
        Hooks::ArgscriptHook::Install();
        Hooks::LuaHook::Install();
        Hooks::Enable();
    }

    void ModLoader::SoftCrash() {
        MessageBox(nullptr, "The application is going to crash, check the logs.", "ModLoader::SoftCrash", 1);
        exit(1);
    }

    ModLoader::ModLoader() {
        mModulePath = absolute(std::filesystem::current_path());
    }
}
