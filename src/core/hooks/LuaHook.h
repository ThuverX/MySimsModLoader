//
// Created by exozg on 12/02/2025.
//

#ifndef LUAHOOK_H
#define LUAHOOK_H
#include <string>

#include "../../include/lua.h"

namespace msml::core::hooks {
    class LuaHook {
    public:
        static void Require(const std::string& path);
        static void Install();
        static lua_State* GlobalState;
    };
}

#endif //LUAHOOK_H
