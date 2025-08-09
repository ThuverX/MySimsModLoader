//
// Created by exozg on 09/08/2025.
//

#ifndef SIGNATURES_MYSIMS32_H
#define SIGNATURES_MYSIMS32_H

// resource system
#define SIG_ResourceSystem_Init "81 EC 20 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 1C 02 00 00 56 8B F1 ?? ?? ?? ?? ?? 84 C0 ?? ?? 32"

// app
#define SIG_App_GetCorrectLoadFolder "81 EC 24 04 00 00 A1 C0 44 02 01 33 C4 89 84 24 20 04 00 00 A1 84 7E F0 00 53 8B 9C 24 38 04 00 00"
#define SIG_App_ReadXMLFromPath "81 EC 90 06 00 00 A1 C0 44 02 01 33 C4 89 84 24 8C 06 00 00 8B 84 24 9C 06 00 00"
#define SIG_App_ReadXMLFromStream "83 EC 14 53 55 56 57 E8 ?? ?? ?? ?? 80 7C 24 3C 00 8B 4C 24 2C"

// argscript
#define SIG_ArgScript_Output "8B 4C 24 04 83 EC 10 85 C9 0F 84 C4 00 00 00 8B 01"
#define SIG_ArgScript_ErrorOutput "83 EC 14 8B 0D E4 FA 08 01 85 C9 B8 90 0E 09 01"

// lua scripting system
#define SIG_LuaScriptingSystem_Startup "83 ec 30 53 55 56 57 8b f1 c7 44 24 10 1c 00 00 00 c7 44 24 14 c8 32 00 00 c7 44 24 18 20 00 00 00 c7 44 24 1c 50 46 00 00 c7 44 24 20 28 00 00 00 c7 44 24 24 52 1c 00 00 c7 44 24 28 50 00 00 00 c7 44 24 2c b6 35 00 00 c7 44 24 30 a0 00 00 00 c7 44 24 34 b2 0c 00 00 c7 44 24 38 40 01 00 00 c7 44 24 3c dc 05 00 00 33 db"

// manager
#define SIG_Manager_GetManager "a1 ?? ?? ?? ?? c3 cc cc cc cc cc cc cc cc cc cc 8b 4c 24 04 a1 c4 5b 09 01 89 0d c4 5b 09 01"
#define SIG_Manager_GetResource "83 EC 10 53 56 32 DB 38 5C 24 1C 57 8B F1 ?? ?? 8B 7C 24 38 85 FF"
#define SIG_Manager_SetTypename "83 EC 08 56 8B F1 57 8D BE 38 01 00 00 68 2C 95 E7 00 8B CF E8 ?? ?? ?? ?? 8D 44 24 14 50 8D 4C 24 0C 81 C6 E8 00 00 00 51 8B CE E8 ?? ?? ?? ?? 8B 56 08 8B 4E 04 8B 44 24 08 3B 04 91"
#define SIG_Manager_RegisterDatabase "83 EC 10 53 55 56 57 8B F9 8D 4F 48 68 2C 95 E7 00 89 4C 24 18"

// dbpf
#define SIG_DatabasePackedFile_OpenRecord "E9 FE E7 31 00 E9 49 A6 0F 00 E9 94 94 1F 00"
#define SIG_DatabasePackedFile_Open "56 8B F1 8A 4C 24 10 32 C0 84 C9"
#define SIG_DatabasePackedFile_GetIndex "E9 59 58 32 00 E9 14 14 28 00 E9 2F 08 22 00"
#define SIG_DatabasePackedFile_GetLocation "E9 28 AE 14 00 E9 93 85 3C 00 E9 FE 15 33 00"

// ddf
#define SIG_DatabaseDirectoryFiles_OpenRecord "81 EC 18 04 00 00 A1 C0 44 02 01 33 C4"
#define SIG_DatabaseDirectoryFiles_AddFile "83 EC 68 8B 44 24 70 56 6A 02 50 8B F1 ?? ?? ?? ?? ?? 8B 8C 24 80 00 00 00 83 C4 08 52 50 51"

// lua
#define SIG_lua_pushvalue "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89 11 8B 40 04 83"
#define SIG_lua_getfield "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 51 8d 54 24 1c"
#define SIG_lua_pushcclosure "56 8B 74 24 08 8B 46 10 8B 48 44 3B 48 40 57 ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8B 46 14 3B 46 28 ?? ?? 8B 46 44"
#define SIG_lua_setfield "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 83 E9 08 51"
#define SIG_lua_settop "8B 4C 24 08 85 C9 8B 44 24 04 ?? ?? 8B 50 0C 03 C9 03 C9 03 C9 03 D1 39 50"
#define SIG_lua_gettop "8b 4c 24 04 8b 41 08 2b 41 0c c1 f8 03 c3"
#define SIG_lua_tolstring "56 8B 74 24 08 57 8B 7C 24 10 57 56 ?? ?? ?? ?? ?? 83 C4 08 83 78 04 04 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4"
#define SIG_lua_newstate "53 55 8B 6C 24 0C 57 8B 7C 24 14 33 DB 53 68 4C 01 00 00 53 53 57 ?? ?? 83 C4 14 3B C3 ?? ?? 5F"
#define SIG_luaL_ref "53 8B 5C 24 0C 8D 83 0F 27 00 00 3D 0F 27 00 00 56 8B 74 24 0C ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8D"
#define SIG_luaL_loadfile "81 EC 0C 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 08 02 00 00 55 8B AC 24 18 02 00 00 56 8B B4 24 18 02 00 00 57 56 ?? ?? ?? ?? ?? 8B"
#define SIG_lua_remove "8b 44 24 08 56 8b 74 24 08 50 56 e8 ?? ?? ?? ?? 83 c0 08 83 c4 08 3b 46 08 73 18 eb 03 8d 49 00"
#define SIG_lua_insert "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 83 C4 08 3B C8 ?? ?? 8D 9B 00 00 00"
#define SIG_lua_pcall "8B 44 24 10 83 EC 08 85 C0 56 8B 74 24 10 ?? ?? 33 C9 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4 08 2B"
#define SIG_lua_rawgeti "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4C 24 18 8B 10 51 52 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89"
#define SIG_luaL_checklstring "8B 44 24 0C 53 56 8B 74 24 0C 57 8B 7C 24 14 50 57 56 ?? ?? ?? ?? ?? 8B D8 83 C4 0C 85 DB ?? ?? 55"
#define SIG_lua_pushstring "55 8B 6C 24 0C 85 ED ?? ?? 8B 44 24 08 8B 48 08 89 69 04 83 40 08 08 5D"
#define SIG_luaB_loadstring "51 56 8B 74 24 0C 57 8D 44 24 08 50 6A 01 56 ?? ?? ?? ?? ?? 6A 00 8B F8 57 6A 02 56"
#define SIG_luaL_loadbuffer "83 EC 08 8B 44 24 10 8B 54 24 18 8B 4C 24 14 52 89 44 24 04 8D 44 24 04 50 89 4C 24 0C 8B 4C 24 14"



#endif //SIGNATURES_MYSIMS32_H
