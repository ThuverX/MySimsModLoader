//
// Created by exozg on 08/08/2025.
//

#ifndef SIGNATURES_MYSIMS64_H
#define SIGNATURES_MYSIMS64_H

#include "signatures_engine64.h"

// other
#define SIG_load_body "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 48 8B E9 4D 8B F1 48 8D 0D ?? ?? ?? ?? 49 8B F8 48 8B F2 E8 ?? ?? ?? ??"

// resource system
#define SIG_ResourceSystem_Init "40 55 57 48 8D AC 24 D8 FD FF FF 48 81 EC 28 03 00 00 48 8B 05 ?? ?? ?? ??"

// dbpf
#define SIG_DatabasePackedFile_OpenRecord "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 4C 89 4C 24 20 57 41 54 41 55 41 56 41 57 48 81 EC A0 00 00 00 4D 8B F8 4C 8B F2"
#define SIG_DatabasePackedFile_Open "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 30 48 8B D9 41 0F B6 E9 48 8D 0D ?? ?? ?? ?? 41 8B F0"
#define SIG_DatabasePackedFile_GetIndex "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 83 D0 02 00 00 48 83 C4 20 5B"
#define SIG_DatabasePackedFile_GetLocation "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 43 20 48 83 C4 20 5B C3 CC 48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20"

// app
#define SIG_App_GetCorrectLoadFolder "40 53 55 56 57 48 81 EC 98 06 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4"
#define SIG_App_ReadXMLFromPath "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 A8 F9 FF FF 48 81 EC 58 07 00 00 48 8B 05 ?? ?? ?? ??"
#define SIG_App_ReadXMLFromStream "48 89 5C 24 10 48 89 6C 24 20 4C 89 44 24 18 56 57 41 54 41 55 41 57 48 83 EC 70"

// argscript
#define SIG_ArgScript_Output "48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 56 57 48 83 EC 40 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ??"
#define SIG_ArgScript_ErrorOutput "48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 57 48 83 EC 28 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C7 43 18 05 00 00 00 48 8D 05 ?? ?? ?? ?? 48 89 03"

// lua scripting system
#define SIG_LuaScriptingSystem_Startup "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 81 EC C0 00 00 00 48 8B E9 48 8D 0D ?? ?? ?? ??"


#endif //SIGNATURES_MYSIMS64_H
