//
// Created by exozg on 12/02/2025.
//

#ifndef LUAHOOK_H
#define LUAHOOK_H
#include <string>

#include "../signatures/lua.h"

class LuaHook {
public:
    static LuaHook &GetInstance();
    void Require(const std::string& path) const;
    void Install();
    lua_State* GlobalState;
};



#endif //LUAHOOK_H
