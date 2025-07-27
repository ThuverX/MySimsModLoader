//
// Created by exozg on 12/02/2025.
//

#include "LuaHook.h"

#include "../../Version.h"
#include "../../EA/IO/FileStream.h"
#include "../modloader/Mods.h"
#include "../signatures/sigdef.h"

namespace Msml::Core::Hooks {
    int LuaPrint(lua_State* L) {
        const int kNargs = lua_gettop(L);
        std::string logMessage;

        for (int i = 1; i <= kNargs; ++i) {
            const char* arg = lua_tostring(L, i);
            logMessage += (arg != nullptr) ? arg : "(nil)";

            if (i < kNargs) {
                logMessage += " ";
            }
        }

        MSML_LOG_LUA("%s", logMessage.c_str());
        return 0;
    }

    static int sOriginalRequireRef = LUA_REFNIL;

    int LuaRequire(lua_State* L) {
        Mods::GetInstance().RunPreHooks();

        lua_pushstring(L, MSML_VERSION);
        lua_setglobal(L, "MSML_VERSION");

        lua_pushcclosure(L, LuaPrint, 0);
        lua_setglobal(L, "print");

        lua_rawgeti(L, LUA_REGISTRYINDEX, sOriginalRequireRef);

        lua_pushvalue(L, 1);

        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
            lua_pop(L, 1);
            return 0;
        }

        Mods::GetInstance().RunPostHooks();

        return 1;
    }

    void __fastcall LuaScriptingSystemStartupHooked(EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem* pThisPtr) {
        EA::ScriptOs::LuaScriptingSystem::StartupHook.Original(pThisPtr);

        lua_State* L = pThisPtr->mState;

        lua_getglobal(L, "CodeRequire");
        sOriginalRequireRef = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_pushcclosure(L, LuaRequire, 0);
        lua_setglobal(L, "CodeRequire");

        LuaHook::sGlobalState = L;

        MSML_LOG_INFO("Lua hooked successfully!");
    }

    lua_State* LuaHook::sGlobalState = nullptr;

    void LuaHook::Require(const std::string& path) {
        lua_State* L = sGlobalState;

        auto *const kStream = new EA::IO::FileStream(path);
        kStream->AddRef();
        const auto kNSize = kStream->GetSize();
        std::string string(kNSize, '\0');
        kStream->Read(string.data(), kNSize);
        kStream->Close();
        kStream->Release();

        if (luaL_loadbuffer(L, string.data(), string.size(), "_chunk") != LUA_OK) {
            MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), lua_tostring(L, -1));
            lua_pop(L, 1);
            return;
        }

        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            MSML_LOG_ERROR("Lua Error at %s: %s", path.c_str(), lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    void _AptInternalUpdateHooked(uint32_t a, uint32_t b) {
        MSML_LOG_DEBUG_HIDDEN("_AptInternalUpdateHooked");
        // void* mv = GetAptMovieCharacter();

        void* apt_string = CreateString("TEST STRING");

        // sMethod_createTextField(apt_string, a);
        _AptInternalUpdateHook.Original(a, b);
    }

    void LuaHook::Install() {
        _AptInternalUpdateHook.Install(&_AptInternalUpdateHooked);
        EA::ScriptOs::LuaScriptingSystem::StartupHook.Install(&LuaScriptingSystemStartupHooked);
    }
}