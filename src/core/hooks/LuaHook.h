//
// Created by exozg on 12/02/2025.
//

#ifndef LUAHOOK_H
#define LUAHOOK_H
#include <string>

#include "../../include/lua.h"

namespace Msml::Core::Hooks {
    class LuaHook {
    public:
        static lua_State* sGlobalState;

        static void Require(const std::string& kPath);
        static bool RunString(const std::string& kCode);
        static void Install();
    };
}

#endif //LUAHOOK_H
