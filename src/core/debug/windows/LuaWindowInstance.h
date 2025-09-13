//
// Created by exozg on 10/08/2025.
//

#ifndef LUAWINDOWINSTANCE_H
#define LUAWINDOWINSTANCE_H
#include "../DebugUI.h"
#include "../../hooks/LuaHook.h"
#include "../../signatures/sigdef.h"

class LuaWindowInstance final : public DebugWindow {
public:
    std::string mName = "LuaWindow";
    int mCallbackRef = LUA_NOREF;

    LuaWindowInstance(const std::string &name, const int kCallbackRef) : mName(name), mCallbackRef(kCallbackRef) {
    }

    void draw() override {
        if (Msml::Core::Hooks::LuaHook::sGlobalState == nullptr || mCallbackRef == LUA_NOREF) { return; }
        lua_State* L = Msml::Core::Hooks::LuaHook::sGlobalState;
        lua_rawgeti(L, LUA_REGISTRYINDEX, mCallbackRef);
        if (!lua_isfunction(L, -1)) {
            lua_pop(L, 1);
            MSML_LOG_ERROR("Callback ref is not a function!");
            return;
        }

        if (lua_pcall(L, 0, 0, 0) != 0) {
            const char* err = lua_tostring(L, -1);
            MSML_LOG_ERROR("Error calling callback: %s", err);
            lua_pop(L, 1);
        }

        ensure_imgui_closure();
    };

    const std::string &getName() const override {
        return mName;
    };
};

#endif //LUAWINDOWINSTANCE_H
