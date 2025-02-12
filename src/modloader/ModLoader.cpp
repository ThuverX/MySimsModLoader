//
// Created by exozg on 24/01/2025.
//

#include "ModLoader.h"

#include "../util/Logger.h"
#include "../assets/Assets.h"
#include "../hooks/Hooks.h"
#include "../mods/Mods.h"

#include "../Version.h"
#include "../signatures/sigdef.h"


ModLoader & ModLoader::GetInstance() {
    static ModLoader instance;
    return instance;
}

int lua_print(lua_State* L) {
    const int nargs = lua_gettop(L);
    std::string logMessage;

    for (int i = 1; i <= nargs; ++i) {
        const char* arg = lua_tostring(L, i);
        logMessage += arg ? arg : "(nil)";

        if (i < nargs) {
            logMessage += " ";
        }
    }

    MSML_LOG_LUA("%s", logMessage.c_str());
    return 0;
}

void __fastcall ResourceSystemInitHooked(Revo::ResourceSystem::ResourceSystem* this_ptr, void* _ECX) {
    Revo::ResourceSystem::InitHook.Original(this_ptr);

    Assets::GetInstance().CreateDatabase(reinterpret_cast<EA::ResourceMan::Manager::Manager *>(this_ptr));
}

void __fastcall LuaScriptingSystemStartupHooked(EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem* this_ptr) {
    EA::ScriptOs::LuaScriptingSystem::StartupHook.Original(this_ptr);

    lua_State* L = this_ptr->state;

    lua_getfield(L, LUA_GLOBALSINDEX, "_G");
    lua_pushcclosure(L, lua_print, 0);
    lua_setfield(L, -2, "print");
    lua_pop(L, 1);
}


void ModLoader::Initialize() {
    MSML_LOG_INFO("MSML Version: %s", MSML_VERSION);
    Hooks::GetInstance().Initialize();

    Signatures::GetInstance().SearchAll();

    Mods::GetInstance().Find();

    Assets::GetInstance().Install();

    InstallLuaHooks();
    Revo::ResourceSystem::InitHook.Install(&ResourceSystemInitHooked);

    Hooks::GetInstance().Enable();

    Mods::GetInstance().LoadHooks();
}

void ModLoader::InstallLuaHooks() {
    EA::ScriptOs::LuaScriptingSystem::StartupHook.Install(&LuaScriptingSystemStartupHooked);
}

ModLoader::ModLoader() {
    modulePath = absolute(std::filesystem::current_path()).string() + "/";
}
