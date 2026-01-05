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
#define SIG_LuaScriptingSystem_LoadAllLuaScripts "48 89 4C 24 08 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 E1 48 81 EC E8 00 00 00 48 8B F9 48 8D 0D ?? ?? ?? ??"

// graphics
#define SIG_Graphics_GetDevice "48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC 48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? B8 1C 02 00 00 48 83 C4 28 C3"

// the forge
#define SIG_addFence "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F2 48 8B D9 BA 10 00 00 00 8D 4A 08 FF 15 A5 A9 8D 00 0F 57 C0 4C 8D 0D EB 5E 99 00"

#define SIG_StateMachine_RequestState "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 41 0F B6 F0 48 8D 0D ?? ?? ?? ?? 8B FA E8 ?? ?? ?? ?? 3B 7B 10 75 27 40 84 F6"

// ui
#define SIG_UI_SetCursorLock "40 53 48 83 EC 20 0F B6 D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 88 1D 2A 6B 6C 00 48 83 C4 20 5B"


// statemachine
#define SIG_StateMachine_GetWorld "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 63 43 10 83 F8 07 77 16 48 8B 4C C3 20 48 85 C9 74 0C 48 8B 01 48 83 C4 20 5B 48 FF 60 20 33 C0 48 83 C4 20"

// world
#define SIG_World_Constructor "48 89 5C 24 18 48 89 4C 24 08 55 56 57 48 83 EC 40 48 8B F9 48 8D 0D ?? ?? ?? ??"
#define SIG_World_AddObject "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 49 8B F8 48 8D 0D ?? ?? ?? ?? 48 8B DA E8 ?? ?? ?? ?? 48 85 DB 74 1D 4C 8B C7 48 8B D3"
#define SIG_World_ReadWorldFromXML "48 8B C4 48 89 58 08 48 89 70 18 48 89 78 20 48 89 50 10 55 41 54"

// blockworld
#define SIG_BlockWorld_GoToOps "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 40 48 8B F9 49 8B E8 48 8D 0D ?? ?? ?? ?? 48 8B F2"
#define SIG_BlockWorld_Constructor "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 48 89 4C 24 08 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B E9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ??"

// StateOPS
#define SIG_StateOPS_MaximumAllowedObjectsInLevel "48 89 5C 24 10 48 89 6C 24 18 56 41 56 41 57 48 83 EC 20 48 8B E9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ??"

// gameobject factory
#define SIG_GameObjectFactory_Spawn "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20 48 8B F1 49 8B F9 48 8D 0D ?? ?? ?? ?? 49 8B E8"
#define SIG_GameObjectFactory_Constructor "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 33 D2 C7 83 08 04 00 00 00 00 00 00 41 B8 00 04 00 00 48 8B CB"

// ObjectModule
#define SIG_ObjectModule_RegisterWorld "48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? B0 01 48 83 C4 28 C3"

// gameobject
#define SIG_GameObject_GetTransform "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 48 8B FA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F 10 43 58 0F 11 07 0F 10 4B 68"
#define SIG_GameObject_SetTransform "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 48 8B F2 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 53 58 48 8B CE E8 ?? ?? ?? ??"

// constants
#define CONST_gSimulator 0xbb65e8
#define CONST_gGameObjectFactory 0xbb66a0
#define CONST_gBlockWorld 0xbabb08
#define CONST_gWorld 0xbc7f18

#endif //SIGNATURES_MYSIMS64_H
