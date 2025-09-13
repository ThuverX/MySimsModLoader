//
// Created by exozg on 12/02/2025.
//

#include "LuaHook.h"

#include "../../include/lua.h"
#include "../../Version.h"
#include "../../EA/IO/FileStream.h"
#include "../modloader/Mods.h"
#include "../signatures/sigdef.h"

namespace Msml::Core::Hooks {
    static int sOriginalRequireRef = LUA_REFNIL;
    std::vector<LuaFunction> LuaHook::sFunctions;
    std::string LuaHook::sLuaError;

    int LuaPrint(lua_State *L) {
        const int kNargs = lua_gettop(L);
        std::string logMessage;

        for (int i = 1; i <= kNargs; ++i) {
            const char *arg = lua_tostring(L, i);
            logMessage += (arg != nullptr) ? arg : "(nil)";

            if (i < kNargs) {
                logMessage += " ";
            }
        }

        MSML_LOG_LUA("%s", logMessage.c_str());
        return 0;
    }

    int LuaRequire(lua_State *L) {
        Mods::GetInstance().RunPreHooks();

        lua_pushstring(L, MSML_VERSION);
        lua_setglobal(L, "MSML_VERSION");

        lua_pushcfunction(L, LuaPrint);
        lua_setglobal(L, "print");

        lua_rawgeti(L, LUA_REGISTRYINDEX, sOriginalRequireRef);

        lua_pushvalue(L, 1);

        if (lua_pcall(L, 1, 1, 0) != 0) {
            const char* err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Error calling original require: %s", err);
            LuaHook::sLuaError = err;
            lua_pop(L, 1);
            return 0;
        }

        Mods::GetInstance().RunPostHooks();

        return 1;
    }

    EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem* LuaHook::sLuaScriptingSystem = nullptr;

    void __fastcall LuaScriptingSystemStartupHooked(EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem *pThisPtr) {
        LuaHook::sLuaScriptingSystem = pThisPtr;
        EA::ScriptOs::LuaScriptingSystem::StartupHook.Original(pThisPtr);

        lua_State *L = pThisPtr->mState;

        lua_getglobal(L, "CodeRequire");
        sOriginalRequireRef = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_pushcfunction(L, LuaRequire);
        lua_setglobal(L, "CodeRequire");

        LuaHook::sGlobalState = L;

        for (auto &func: LuaHook::sFunctions) {
            LuaHook::_RegisterFunction(func.mTable, func.mName, func.mFunction);
        }

        LuaHook::sFunctions.clear();

        MSML_LOG_INFO("Lua hooked successfully!");
    }

    lua_State *LuaHook::sGlobalState = nullptr;

    void LuaHook::Require(const std::string &path) {
        lua_State *L = sGlobalState;

        auto *const kStream = new EA::IO::FileStream(path);
        kStream->AddRef();
        const auto kNSize = kStream->GetSize();
        std::string string(kNSize, '\0');
        kStream->Read(string.data(), kNSize);
        kStream->Close();
        kStream->Release();

        if (luaL_loadbuffer(L, string.data(), string.size(), "_chunk") != 0) {
            const char *err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), err);
            sLuaError = err;
            lua_pop(L, 1);
            return;
        }

        if (lua_pcall(L, 0, 0, 0) != 0) {
            const char *err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), err);
            sLuaError = err;
            lua_pop(L, 1);
        }
    }

    bool LuaHook::RunString(const std::string &kCode) {
        lua_State *L = sGlobalState;

        if (luaL_loadbuffer(L, kCode.data(), kCode.size(), "_chunk") != 0) {
            const char *err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Lua Error at _chunk: %s", err);
            sLuaError = err;
            lua_pop(L, 1);

            return false;
        }

        if (lua_pcall(L, 0, 1, 0) != 0) {
            const char *err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Lua Error at _chunk: %s", err);
            sLuaError = err;
            lua_pop(L, 1);

            return false;
        }
        return true;
    }



    bool LuaHook::_RegisterFunction(const std::string &kTableName, const std::string &kFunctionName,
                                    lua_CFunction function) {
        lua_State *L = sGlobalState;
        if (L == nullptr) { return false; }

        if (kTableName.empty() || kTableName == "global" || kTableName == "_G") {
            lua_pushcfunction(L, function);
            lua_setglobal(L, kFunctionName.c_str());
            return true;
        }

        lua_getglobal(L, kTableName.c_str());

        if (lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setglobal(L, kTableName.c_str());
        }

        lua_pushcfunction(L, function);

        lua_setfield(L, -2, kFunctionName.c_str());

        lua_pop(L, 1);

        return true;
    }

    bool LuaHook::RegisterFunction(const std::string &kTableName, const std::string &kFunctionName,
                                   lua_CFunction function) {
        if (sGlobalState == nullptr) {
            sFunctions.emplace_back(LuaFunction{
                .mTable = kTableName,
                .mName = kFunctionName,
                .mFunction = function,
            });
            return true;
        }

        return _RegisterFunction(kTableName, kFunctionName, function);
    }

    void LuaHook::Install() {
        EA::ScriptOs::LuaScriptingSystem::StartupHook.Install(&LuaScriptingSystemStartupHooked);
    }

    void LuaHook::Reload() {
        MSML_LOG_DEBUG("Reloading lua...");
        MSML_LOG_DEBUG("Not yet supported");
        // EA::ScriptOs::LuaScriptingSystem::LoadAllLuaScripts(sLuaScriptingSystem);
    }
}
