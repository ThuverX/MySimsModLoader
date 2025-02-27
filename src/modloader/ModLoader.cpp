//
// Created by exozg on 24/01/2025.
//

#include "ModLoader.h"

#include "../util/Logger.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"
#include "../mods/Mods.h"

#include "../Version.h"
#include "../hooks/LuaHook.h"
#include "../signatures/sigdef.h"


ModLoader & ModLoader::GetInstance() {
    static ModLoader instance;
    return instance;
}

void ModLoader::Initialize() {
    MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);
    Hooks::GetInstance().Initialize();
    Signatures::GetInstance().SearchAll();
    Mods::GetInstance().Find();
    Assets::GetInstance().Install();
    LuaHook::GetInstance().Initialize();
    Hooks::GetInstance().Enable();
}

ModLoader::ModLoader() {
    modulePath = absolute(std::filesystem::current_path()).string() + "/";
}
