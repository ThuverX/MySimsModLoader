//
// Created by exozg on 11/06/2025.
//

#ifndef DEBUGUI_H
#define DEBUGUI_H

#include "imgui_hex.h"
#include "../assets/Assets.h"

namespace Msml::Core {
    class DebugUI {
        // ImGuiHexEditorState hex_state = {};
        // std::vector<uint8_t> hex_buffer = {};
        // EA::ResourceMan::Key active_key = {};
        // bool use_hex_buffer = false;
        // Assets* assets = nullptr;
        // std::vector<EA::ResourceMan::Key> keys{};
        // std::vector<EA::ResourceMan::Key> filtered_keys{};
        // char search_string[256] = {};
        //
        // void DrawAssets();
        // void DrawDatabase();

    public:
        static DebugUI& GetInstance();

        void Init();
        void Draw();

        static void Install();
    };
}


#endif //DEBUGUI_H
