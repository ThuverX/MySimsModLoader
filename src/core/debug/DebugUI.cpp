//
// Created by exozg on 11/06/2025.
//

#include "DebugUI.h"

#include <ranges>

#include "imgui.h"
#include "../../EA/ResourceMan/KeyFilter.h"
#include "../signatures/sigdef.h"
#include "windows/AssetWindow.h"
#include "windows/HexWindow.h"
#include "windows/LuaWindow.h"

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
    }

    template<typename T>
    T* DebugUI::GetWindow(const std::string& name) {
        if (mWindows.contains(name)) {
            return dynamic_cast<T*>(mWindows[name]);
        }
        return nullptr;
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

        SetCursorLock(mIsVisible);
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

    // // Should this be here? We really don't have another place for the forge functions
    // void writeLogHooked(uint32_t level, const char *filename, int line_number, const char *message, ...) {
    //     va_list args;
    //     va_start(args, message);
    //
    //     std::vector<char> buffer(1024);
    //     int required = vsnprintf(buffer.data(), buffer.size(), message, args);
    //
    //     va_end(args);
    //
    //     if (required < 0) {
    //         return;
    //     }
    //
    //     // Resize if needed
    //     if (static_cast<size_t>(required) >= buffer.size()) {
    //         buffer.resize(required + 1);
    //         va_start(args, message);
    //         vsnprintf(buffer.data(), buffer.size(), message, args);
    //         va_end(args);
    //     }
    //
    //     auto local_level = system::Logger::LogLevel::INFO;
    //
    //     switch (level) {
    //         case 0:
    //         case 1:
    //             return;
    //         case 2: local_level = system::Logger::LogLevel::DEBUG;
    //             break;
    //         case 4: local_level = system::Logger::LogLevel::INFO;
    //             break;
    //         case 8: local_level = system::Logger::LogLevel::WARNING;
    //             break;
    //         case 16: local_level = system::Logger::LogLevel::ERR;
    //             break;
    //         default: local_level = system::Logger::LogLevel::INFO;
    //     }
    //
    //     system::Logger::Log(local_level, false, filename, line_number,
    //                         "%s",
    //                         buffer.data());
    // }
}
