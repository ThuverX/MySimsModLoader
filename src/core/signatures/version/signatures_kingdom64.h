//
// Created by exozg on 09/08/2025.
//

#ifndef SIGNATURES_KINGDOM64_H
#define SIGNATURES_KINGDOM64_H

#include "signatures_engine64.h"


// resource system
#define SIG_ResourceSystem_Init "40 57 48 81 EC 70 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4"

// lua scripting system
#define SIG_LuaScriptingSystem_Startup "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 33 DB 48 8D 0D ?? ?? ?? ?? 33 D2"

// dbpf
#define SIG_DatabasePackedFile_OpenRecord "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 4C 89 4C 24 20 57 41 54 41 55 41 56 41 57 48 81 EC B0 00 00 00 4D 8B E0 48 8B EA"
#define SIG_DatabasePackedFile_Open "48 89 5C 24 18 55 57 41 57 48 83 EC 30 48 8B F9"
#define SIG_DatabasePackedFile_GetIndex "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 83 38 03 00 00 48 83 C4 20 5B"
#define SIG_DatabasePackedFile_GetLocation "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 43 20 48 83 C4 20 5B C3 CC 48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20"

// graphics
#define SIG_Graphics_GetDevice "48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC 48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? B8 E0 01 00 00 48 83 C4 28 C3"

// the forge
#define SIG_addFence "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F2 48 8B D9 BA 10 00 00 00 8D 4A 08 FF 15 35 A2 84 00 0F 57 C0 4C 8D 0D 5B 8F 8E 00"


#endif //SIGNATURES_KINGDOM64_H
