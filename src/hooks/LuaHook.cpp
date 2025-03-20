//
// Created by exozg on 12/02/2025.
//

#include "LuaHook.h"

#include "../Version.h"
#include "../mods/Mods.h"
#include "../signatures/lua.h"
#include "../signatures/sigdef.h"
#include "../util/File.h"

LuaHook & LuaHook::GetInstance() {
    static LuaHook instance;
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

static int original_require_ref = LUA_REFNIL;

int lua_require(lua_State* L) {
    Mods::GetInstance().RunPreHooks();

    lua_rawgeti(L, LUA_REGISTRYINDEX, original_require_ref);

    lua_pushvalue(L, 1);

    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        lua_pop(L, 1);
        return 0;
    }

    Mods::GetInstance().RunPostHooks();

    return 1;
}

void __fastcall LuaScriptingSystemStartupHooked(EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem* this_ptr) {
    EA::ScriptOs::LuaScriptingSystem::StartupHook.Original(this_ptr);

    lua_State* L = this_ptr->state;

    lua_pushstring(L, MSML_VERSION);
    lua_setglobal(L, "MSML_VERSION");

    lua_pushcclosure(L, lua_print, 0);
    lua_setglobal(L, "print");

    lua_getglobal(L, "CodeRequire");
    original_require_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushcclosure(L, lua_require, 0);
    lua_setglobal(L, "CodeRequire");

    LuaHook::GetInstance().GlobalState = L;

    MSML_LOG_INFO("Lua hooked successfully!");
}

void LuaHook::Require(const std::string& path) const {
    lua_State* L = GlobalState;

    if (const auto code = File::ReadAsString(path.c_str()); luaL_loadbuffer(L, code, strlen(code), "_chunk") != LUA_OK) {
        MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }

    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void LuaHook::Install() {
    EA::ScriptOs::LuaScriptingSystem::StartupHook.Install(&LuaScriptingSystemStartupHooked);
}
