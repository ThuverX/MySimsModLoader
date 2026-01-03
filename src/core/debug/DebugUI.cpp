//
// Created by exozg on 11/06/2025.
//

#include "DebugUI.h"

#include <ranges>

#include "lua_imgui.h"
#include "../../include/lua.h"
#include "windows/AssetWindow.h"
#include "windows/HexWindow.h"
#include "windows/LuaWindow.h"
#include "windows/LuaWindowInstance.h"
#include "windows/NetworkWindow.h"

namespace Msml::Core {
    DebugUI &DebugUI::GetInstance() {
        static DebugUI instance;
        return instance;
    }

    void DebugUI::Init() {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        mWindows["assets"] = new AssetWindow;
        mWindows["hexviewer"] = new HexWindow;
        mWindows["lua"] = new LuaWindow;
        mWindows["network"] = new NetworkWindow;
    }

    template<typename T>
    T* DebugUI::GetWindow(const std::string& name) {
        if (mWindows.contains(name)) {
            return dynamic_cast<T*>(mWindows[name]);
        }
        return nullptr;
    }

    void DebugUI::RegisterWindow(const std::string &name, DebugWindow *window) {
        mWindows[name] = window;
    }

    void SetupDockspace()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {0.0f, 0.0f});

        ImGui::Begin("DockSpace", nullptr, windowFlags);

        ImGui::PopStyleVar(5);

        ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }

    void DebugUI::Draw() {
        if ((GetAsyncKeyState(VK_F10) & 1) != 0) {
            mIsVisible = !mIsVisible;
        }

        // TODO: set input lock
        UI::SetCursorLock(mIsVisible);
        ImGui::GetIO().MouseDrawCursor = mIsVisible;

        if (!mIsVisible) {
            return;
        }

        SetupDockspace();

        if (ImGui::BeginMainMenuBar()) {
            for (DebugWindow *mWindow: mWindows | std::views::values) {
                mWindow->drawMenu();
            }

            if (ImGui::BeginMenu("Window")) {
                for (DebugWindow *mWindow: mWindows | std::views::values) {
                    if (ImGui::MenuItem(mWindow->getName().c_str())) {
                        mWindow->mIsVisible = !mWindow->mIsVisible;
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        for (DebugWindow *mWindow: mWindows | std::views::values) {
            mWindow->draw();
        }
    }

    int lua_register_window(lua_State* L) {
        int nargs = lua_gettop(L);
        if (nargs < 2) {
            luaL_error(L, "Expected 2 arguments: (string name, function callback)");
            return 0;
        }

        if (!lua_isstring(L, 1)) {
            luaL_error(L, "First argument must be a string");
            return 0;
        }

        const char* windowName = lua_tostring(L, 1);

        if (!lua_isfunction(L, 2)) {
            luaL_error(L, "Second argument must be a function");
            return 0;
        }

        lua_pushvalue(L, 2);
        int callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

        DebugUI::GetInstance().RegisterWindow(windowName, new LuaWindowInstance(windowName, callbackRef));

        return 0;
    }

    void DebugUI::Install() {
        Hooks::LuaHook::RegisterFunction("DebugUI", "RegisterWindow", lua_register_window);
        Hooks::LuaHook::RegisterFunction("ImGui", "Begin", lua_imgui_Begin);
        Hooks::LuaHook::RegisterFunction("ImGui", "End", lua_imgui_End);
        Hooks::LuaHook::RegisterFunction("ImGui", "Text", lua_imgui_Text);
        Hooks::LuaHook::RegisterFunction("ImGui", "Button", lua_imgui_Button);
        Hooks::LuaHook::RegisterFunction("ImGui", "IntInput", lua_imgui_intinput);
    }
}
