//
// Created by exozg on 24/01/2025.
//

#include <windows.h>

#define SIG_DEFINE
#include "core/signatures/sigdef.h"
#undef SIG_DEFINE

#include "core/modloader/ModLoader.h"
#include "core/system/CrashHandler.h"
#include "EASTL/internal/config.h"


#ifdef VERSION_MYSIMS_ORIGINAL
#include "platform/w32/dsound.h"
#else
#include "platform/w64/wsock.h"
#endif

// unused but required to build eastl
namespace eastl {
    using EASTL_AssertionFailureFunction = void (*)(const char *pExpression, void *pContext);

    EASTL_API void SetAssertionFailureFunction(EASTL_AssertionFailureFunction pFunction, void *pContext) {
    }

    EASTL_API void AssertionFailure(const char *pExpression) {
    }

    EASTL_API void AssertionFailureFunctionDefault(const char *pExpression, void *pContext) {
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, const DWORD kReason, LPVOID lpReserved) {
    if (kReason == DLL_PROCESS_ATTACH) {
        AddVectoredExceptionHandler(1, Msml::Core::System::VectoredCrashHandler);

        char lpBuffer[MAX_PATH];
        GetSystemDirectoryA(lpBuffer, MAX_PATH);

#ifdef VERSION_MYSIMS_ORIGINAL
        strcat_s(lpBuffer, "\\dsound.dll");
#else
        strcat_s(lpBuffer, "\\WSOCK32.dll");
#endif

        Msml::Core::ModLoader::GetInstance().Initialize();

        const HMODULE kHL = LoadLibraryA(lpBuffer);
        if (kHL == nullptr) {
            return FALSE;
        }

        exports(kHL);
    } else if (kReason == DLL_PROCESS_DETACH) {
        Msml::Core::System::Logger::Flush();
    }

    return TRUE;
}
