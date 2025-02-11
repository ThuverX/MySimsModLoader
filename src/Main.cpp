//
// Created by exozg on 24/01/2025.
//

#include <windows.h>
#include "dsound.h"
#include "modloader/modloader.h"

#define SIG_DEFINE
#include "signatures/sigdef.h"
#undef SIG_DEFINE


// TODO: This would be nice if it worked
LONG WINAPI CrashHandler(EXCEPTION_POINTERS* exceptionInfo) {
    MSML_LOG_ERROR("MySims Crashed!");

    return EXCEPTION_EXECUTE_HANDLER;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {

        SetUnhandledExceptionFilter(CrashHandler);

        char bufd[200];
        GetSystemDirectoryA(bufd, 200);
        strcat_s(bufd, "\\dsound.dll");

        ModLoader::GetInstance().Initialize();

        HMODULE hL = LoadLibraryA(bufd);
        if (!hL)
            return false;

        dsound(hL);
    }

    return TRUE;
}
