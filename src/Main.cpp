//
// Created by exozg on 24/01/2025.
//

#include <windows.h>


#define SIG_DEFINE
#include "core/signatures/sigdef.h"
#undef SIG_DEFINE

#include "core/modloader/ModLoader.h"
#include "core/system/CrashHandler.h"
#include "EASTL/allocator.h"
#include "EASTL/internal/config.h"
#include "tweakers/Tweakers.h" // registers tweakers

#ifdef _WIN64
#include "platform/w64/wsock.h"
#else
#include "platform/w32/dsound.h"
#endif

// unused but required to build eastl
namespace eastl {
    typedef void (*EASTL_AssertionFailureFunction)(const char *pExpression, void *pContext);

    EASTL_API void SetAssertionFailureFunction(EASTL_AssertionFailureFunction pFunction, void *pContext) {
    }

    EASTL_API void AssertionFailure(const char *pExpression) {
    }

    EASTL_API void AssertionFailureFunctionDefault(const char *pExpression, void *pContext) {
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        AddVectoredExceptionHandler(1, msml::core::system::VectoredCrashHandler);

        char bufd[200];
        GetSystemDirectoryA(bufd, 200);
#ifdef _WIN64
        strcat_s(bufd, "\\WSOCK32.dll");
#else
        strcat_s(bufd, "\\dsound.dll");
#endif

        msml::core::ModLoader::GetInstance().Initialize();

        const HMODULE hL = LoadLibraryA(bufd);
        if (!hL)
            return false;

        exports(hL);
    }

    return TRUE;
}
