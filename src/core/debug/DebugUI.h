//
// Created by exozg on 11/06/2025.
//

#ifndef DEBUGUI_H
#define DEBUGUI_H

#include <vector>
#include "windows/AssetWindow.h"

using window_func = void (*)();

namespace Msml::Core {
    class DebugUI {
        std::vector<window_func> mWindows = {
            AssetWindow,
        };

    public:
        static DebugUI &GetInstance();

        bool mIsVisible = false;

        void Init();

        void Draw();
    };
}


#endif //DEBUGUI_H
