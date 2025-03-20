//
// Created by exozg on 24/01/2025.
//

#include "ModLoader.h"

#include "../util/Logger.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"
#include "../mods/Mods.h"

#include "../Version.h"
#include "../hashes/Hashes.h"
#include "../hooks/LuaHook.h"
#include "../signatures/sigdef.h"


ModLoader & ModLoader::GetInstance() {
    static ModLoader instance;
    return instance;
}

void ModLoader::Initialize() {
    MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);
    Hooks::GetInstance().Initialize();
    if (!Signatures::GetInstance().SearchAll()) {
        // If we failed to get any signatures, we quit
#ifndef NDEBUG
        MSML_LOG_ERROR("Failed to find addresses for some functions, did you write the signature correctly?");
        Hooks::GetInstance().Uninitialize();

        SoftCrash();
#else
        MSML_LOG_ERROR("Failed to find functions. Please report this on github and include your log file at");
        MSML_LOG_ERROR("https://github.com/ThuverX/MySimsModLoader/issues\n");
        MSML_LOG_INFO("NO MODS WILL BE LOADED");
        Hooks::GetInstance().Uninitialize();
        return;
#endif
    }
    Hashes::GetInstance().Load("./hashes.bin");
    Mods::GetInstance().Find();
    Assets::GetInstance().Install();
    LuaHook::GetInstance().Install();
    Hooks::GetInstance().Enable();
}

void ModLoader::SoftCrash() {
    MessageBox(nullptr, "The application is going to crash, check the logs.", "ModLoader::SoftCrash", 1);
    exit(0);
}

ModLoader::ModLoader() {
    modulePath = absolute(std::filesystem::current_path()).string() + "/";
}
