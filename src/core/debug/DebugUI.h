//
// Created by exozg on 11/06/2025.
//

#ifndef DEBUGUI_H
#define DEBUGUI_H

#include <unordered_map>
#include <string>

class DebugWindow {
public:
    virtual ~DebugWindow() = default;

    virtual void draw() = 0;
    virtual void drawMenu() {}
    virtual const std::string& getName() const = 0;

    bool mIsVisible = true;
};

namespace Msml::Core {
    class DebugUI {
        std::unordered_map<std::string, DebugWindow*> mWindows;

    public:
        static DebugUI &GetInstance();

        bool mIsVisible = false;

        template<typename T>
        T* GetWindow(const std::string& name);

        void Init();

        void Draw();
    };
}


#endif //DEBUGUI_H
