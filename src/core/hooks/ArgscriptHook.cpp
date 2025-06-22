//
// Created by exozg on 17/05/2025.
//

#include "ArgscriptHook.h"

#include "../hooks/Config.h"
#include "../signatures/sigdef.h"

namespace Msml::Core::Hooks {
    void __fastcall ArgScriptOutputHookedWin64(EA::ArgScript::ArgScript *thisPtr CATCH_EDX, void *, const char *fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);

        char temp[1024];
        vsnprintf(temp, sizeof(temp), fmt, args);

        MSML_LOG_INFO("ArgScript: %s", temp);

        va_end(args);
    }

    void __fastcall ArgScriptErrorOutputHookedWin64(EA::ArgScript::ArgScript *thisPtr CATCH_EDX, const char *fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);

        char temp[1024];
        vsnprintf(temp, sizeof(temp), fmt, args);

        MSML_LOG_ERROR("ArgScript: %s", temp);

        va_end(args);
    }

    void __fastcall ArgScriptOutputHookedWin32(const char *fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);

        char temp[1024];
        vsnprintf(temp, sizeof(temp), fmt, args);

        MSML_LOG_INFO("ArgScript: %s", temp);

        va_end(args);
    }

    void __fastcall ArgScriptErrorOutputHookedWin32(const char *fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);

        char temp[1024];
        vsnprintf(temp, sizeof(temp), fmt, args);

        MSML_LOG_ERROR("ArgScript: %s", temp);

        va_end(args);
    }

    void ArgscriptHook::Install() {
#ifdef _WIN64
        EA::ArgScript::ErrorOutputHook.Install(&ArgScriptErrorOutputHookedWin64);
        EA::ArgScript::OutputHook.Install(&ArgScriptOutputHookedWin64);
#else
        EA::ArgScript::ErrorOutputHook.Install(&ArgScriptErrorOutputHookedWin32);
        EA::ArgScript::OutputHook.Install(&ArgScriptOutputHookedWin32);
#endif
    }
}
