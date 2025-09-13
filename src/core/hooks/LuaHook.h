//
// Created by exozg on 12/02/2025.
//

#ifndef LUAHOOK_H
#define LUAHOOK_H
#include <string>
#include <vector>

#include "../../include/lua.h"


namespace EA::ScriptOs::LuaScriptingSystem {
    struct LuaScriptingSystem;
}

namespace Msml::Core::Hooks {
    struct LuaFunction {
        std::string mTable;
        std::string mName;
        lua_CFunction mFunction;
    };
    class LuaHook {
    public:
        static EA::ScriptOs::LuaScriptingSystem::LuaScriptingSystem* sLuaScriptingSystem;
        static lua_State* sGlobalState;
        static std::string sLuaError;
        static std::vector<LuaFunction> sFunctions;

        static void Require(const std::string& kPath);

        static bool RunString(const std::string &kCode);

        static bool RegisterFunction(const std::string& kTableName,const std::string& kFunctionName, lua_CFunction function);

        static void Install();
        static void Reload();

        static bool _RegisterFunction(const std::string& kTableName,const std::string& kFunctionName, lua_CFunction function);

    };
}

#endif //LUAHOOK_H
