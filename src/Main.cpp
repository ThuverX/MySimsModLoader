//
// Created by exozg on 24/01/2025.
//

#include <windows.h>
#include "modloader/modloader.h"

#define SIG_DEFINE
#include "signatures/sigdef.h"
#undef SIG_DEFINE

#ifdef _WIN64
#include "platform/w64/wsock.h"
#else
#include "platform/w32/dsound.h"
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {

        char bufd[200];
        GetSystemDirectoryA(bufd, 200);
#ifdef _WIN64
        strcat_s(bufd, "\\WSOCK32.dll");
#else
        strcat_s(bufd, "\\dsound.dll");
#endif

        ModLoader::GetInstance().Initialize();

        HMODULE hL = LoadLibraryA(bufd);
        if (!hL)
            return false;

        exports(hL);
    }

    return TRUE;
}
