//
// Created by exozg on 24/01/2025.
//

#include <windows.h>

#include "ModLoader.h"

#include "Mods.h"
#include "../../Version.h"
#include "../../EA/IO/File.h"
#include "../assets/Assets.h"
#include "../debug/DebugUI.h"
#include "../debug/ImGuiHook.h"
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

        bool consoleEnabled = false;

#ifdef BUILD_DEBUG
        consoleEnabled = true;
#endif

        if (EA::IO::File::Exists(mModulePath / MODS_PATH / DEVELOPER_TXT)) {
            consoleEnabled = true;
        }

        if (IsDebuggerPresent() != 0) {
            consoleEnabled = false;
        }

        if (consoleEnabled) {
            mConsole.Enable();
        }

        System::Logger::Enable();

        MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);

        Hooks::Initialize();
        if (!Signatures::GetInstance().SearchAll()) {
            // If we failed to get any signatures, we quit
#ifdef BUILD_DEBUG
            MSML_LOG_ERROR("Failed to find addresses for some functions, did you write the signature correctly?");
            Hooks::Uninitialize();

            MessageAndExit("Failed to find addresses for some functions, did you write the signature correctly?");
#else
            MSML_LOG_ERROR("Failed to find functions. Please report this on github and include your log file at");
            MSML_LOG_ERROR("https://github.com/ThuverX/MySimsModLoader/issues\n");
            MSML_LOG_INFO("NO MODS WILL BE LOADED");
            Hooks::Uninitialize();
            return;
#endif
        }

        Resource::IdResolver::GetInstance().Load("./hashes.bin");
        Mods::GetInstance().Find();
        Assets::Install();
        Assets::GetInstance().CreateDatabase();
        DebugUI::Install();
        ImGuiHook::Install();
        Hooks::ArgscriptHook::Install();
        Hooks::LuaHook::Install();
        Hooks::Enable();
    }

    void ModLoader::Message(const std::string &message) {
        System::Logger::Flush();

        MessageBox(nullptr, message.c_str(), "MySims ModLoader", 0);
    }

    void ModLoader::MessageAndExit(const std::string &message) {
        System::Logger::Flush();

        MessageBox(nullptr, message.c_str(), "MySims ModLoader", 1);
        exit(1);
    }

    ModLoader::ModLoader() {
        mModulePath = absolute(std::filesystem::current_path());
    }
}
