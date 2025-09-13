//
// Created by exozg on 10/08/2025.
//

#ifndef LUA_IMGUI_H
#define LUA_IMGUI_H

#include "../../include/lua.h"
#include "../signatures/sigdef.h"
#include "imgui.h"

static bool sHasOpenedBegin = false;

inline int lua_imgui_Begin(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs < 1 || !lua_isstring(L, 1)) {
        return luaL_error(L, "imgui.Begin expects a window name string");
    }
    const char* windowName = lua_tostring(L, 1);

    bool opened = ImGui::Begin(windowName);
    lua_pushboolean(L, opened);
    sHasOpenedBegin = true;
    return 1;
}

inline int lua_imgui_End(lua_State* L) {
    ImGui::End();
    sHasOpenedBegin = false;
    return 0;
}

inline void ensure_imgui_closure() {
    if (sHasOpenedBegin) {
        ImGui::End();
    }
}

inline int lua_imgui_Text(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs < 1 || !lua_isstring(L, 1)) {
        return luaL_error(L, "imgui.Text expects a string");
    }

    const char* str = lua_tostring(L, 1);
    ImGui::TextUnformatted(str);
    return 0;
}

inline int lua_imgui_Button(lua_State* L) {
    int nargs = lua_gettop(L);
    if (nargs < 1 || !lua_isstring(L, 1)) {
        return luaL_error(L, "imgui.Button expects a string");
    }

    const char* str = lua_tostring(L, 1);
    bool clicked = ImGui::Button(str);
    lua_pushboolean(L, clicked);
    return 1;
}

inline int lua_imgui_intinput(lua_State* L) {
    int nargs = lua_gettop(L);

    if (nargs < 1 || !lua_isstring(L, 1)) {
        return luaL_error(L, "imgui.IntInput expects a number");
    }

    if (nargs < 2 || !lua_isnumber(L, 2)) {
        return luaL_error(L, "imgui.IntInput expects a number");
    }

    const char* str = lua_tostring(L, 1);

    int value = lua_tointeger(L, 2);
    bool edited = ImGui::InputInt(str, &value);
    lua_pushboolean(L, edited);
    lua_pushinteger(L, value);
    return 2;
}

#endif //LUA_IMGUI_H
