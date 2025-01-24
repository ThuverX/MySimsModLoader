//
// Created by exozg on 24/01/2025.
//

#include "ModLoader.h"

#include "../util/Logger.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"
#include "../mods/Mods.h"

#include "../Version.h"


ModLoader & ModLoader::GetInstance() {
    static ModLoader instance;
    return instance;
}

void ModLoader::Initialize() {
    MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);
    Mods::GetInstance().Find();

    Hooks::GetInstance().Initialize();
    Assets::GetInstance().Install();
    Hooks::GetInstance().Enable();

    Mods::GetInstance().LoadAssets();
    Mods::GetInstance().LoadHooks();
}

