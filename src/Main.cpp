//
// Created by exozg on 24/01/2025.
//

#include <windows.h>
#include "dsound.h"
#include "modloader/modloader.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {

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
