//
// Created by exozg on 24/01/2025.
//

#include "ModLoader.h"

#include <iostream>

#include "../util/Logger.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"
#include "../mods/Mods.h"

#include "../Version.h"


ModLoader & ModLoader::GetInstance() {
    static ModLoader instance;
    return instance;
}

int lua_print_hook(lua_State* L) {
    const int nargs = lua_gettop(L);
    std::string logMessage;

    for (int i = 1; i <= nargs; ++i) {
        const char* arg = lua_tostring(L, i);
        logMessage += (arg ? arg : "(nil)");

        if (i < nargs) {
            logMessage += " ";
        }
    }

    MSML_LOG_LUA("%s", logMessage.c_str());
    return 0;
}

void lua_pushvalueHooked(lua_State* L, int index) {

    lua_getfield(L, LUA_GLOBALSINDEX, "_G");
    lua_pushcclosure(L, lua_print_hook, 0);
    lua_setfield(L, -2, "print");
    lua_pop(L, 1);

    lua_pushvalueHook.Original(L, index);
}

void ModLoader::Initialize() {
    MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);
    Mods::GetInstance().Find();

    Hooks::GetInstance().Initialize();
    Assets::GetInstance().Install();
    InstallLuaHooks();
    Hooks::GetInstance().Enable();

    Mods::GetInstance().LoadAssets();
    Mods::GetInstance().LoadHooks();
}

void ModLoader::InstallLuaHooks() {
    lua_pushvalueHook.Install(&lua_pushvalueHooked);
}

