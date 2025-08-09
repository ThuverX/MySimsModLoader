//
// Created by exozg on 09/08/2025.
//

#ifndef SIGNATURES_ENGINE64_H
#define SIGNATURES_ENGINE64_H

// manager
#define SIG_Manager_GetManager "48 83 EC 28 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC 48 89 5C 24 10 48 89 6C 24 18 56 57 41 56"
#define SIG_Manager_GetResource "48 89 5C 24 10 48 89 6C 24 18 56 57 41 56 48 83 EC 60 49 8B F9 49 8B F0 48 8B EA"
#define SIG_Manager_SetTypename "48 89 5C 24 18 55 56 57 48 81 EC 80 00 00 00 49 8B F0 8B FA 48 8B E9"
#define SIG_Manager_RegisterDatabase "48 89 5C 24 10 48 89 6C 24 18 56 57 41 54 41 56 41 57 48 83 EC 40 41 8B E9 49 8B F0 0F B6 FA"

// ddf
#define SIG_DatabaseDirectoryFiles_OpenRecord "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC A8 04 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 90 04 00 00 45 8B"
#define SIG_DatabaseDirectoryFiles_AddFile "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 B9 48 81 EC C0 00 00 00 49 8B F9 49 8B D8"

// lua
#define SIG_lua_pushvalue "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 4C 8B 47 10 48 8B 5C 24 30 48 8B 10 49 89 10"
#define SIG_lua_getfield "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F1 49 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 48 8B D8 49 C7 C0 FF FF FF FF 66 0F 1F 84 00 00 00 00 00 49 FF C0 42 80 3C 07 00 75 F6 48 8B D7 48 8B CE E8 ?? ?? ?? ?? 4C 8B 4E 10 4C 8D 44 24 20 48 8B D3 48 89 44 24 20 48 8B CE"
#define SIG_lua_pushcclosure "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 49 63 F8 48 8D 0D ?? ?? ?? ?? 48 8B F2 E8 ?? ?? ?? ?? 4C 8B 4B 20 49 8B 41 70 49 39 41 78"
#define SIG_lua_setfield "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F1 49 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 48 8B D8 49 C7 C0 FF FF FF FF 66 0F 1F 84 00 00 00 00 00 49 FF C0 42 80 3C 07 00 75 F6 48 8B D7 48 8B CE E8 ?? ?? ?? ?? 4C 8B 4E 10 4C 8D 44 24 20 49 83 E9 10 48 89 44 24 20 48 8B D3"
#define SIG_lua_settop "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 48 63 FA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 85 FF 78 50 48 8B 53 18 4C 8B C7"
#define SIG_lua_gettop "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 43 10 48 2B 43 18 48 C1 F8 04 48 83 C4 20"
#define SIG_lua_tolstring "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 49 8B D8 48 8D 0D ?? ?? ?? ?? 8B F2 E8 ?? ?? ?? ?? 8B D6 48 8B CF E8 ?? ?? ?? ??"
#define SIG_lua_newstate "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 30 48 8B E9 48 8B F2 48 8D 0D ?? ?? ?? ??"
#define SIG_luaL_ref "48 89 5C 24 10 57 48 83 EC 20 48 8B D9 8B FA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 87 0F 27 00 00 3D 0F 27 00 00 77 0C"
#define SIG_luaL_loadfile "48 89 5C 24 20 55 56 57 48 81 EC 40 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4"
#define SIG_lua_remove "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 48 8B 4F 10 48 83 C0 10 48 3B C1"
#define SIG_lua_insert "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 48 8B 57 10 4C 8B C8 48 3B D0 76 19"
#define SIG_lua_pcall "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 40 48 8B D9 41 8B F9 48 8D 0D ?? ?? ?? ?? 41 8B F0 8B EA E8 ?? ?? ?? ??"
#define SIG_lua_rawgeti "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 41 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 8B D7 48 8B 08 E8 ?? ?? ?? ?? 48 8B 56 10"
#define SIG_luaL_checklstring "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 49 8B D8 48 8D 0D ?? ?? ?? ?? 8B FA E8 ?? ?? ?? ?? 8B D7 48 8B CE E8 ?? ?? ?? ??"
#define SIG_lua_pushstring "48 89 6C 24 18 56 48 83 EC 20 48 8B F1 48 8B EA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 85 ED 75 17 48 8B 46 10 89 68 08"
#define SIG_luaB_loadstring "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 4C 8D 44 24 38 BA 01 00 00 00 48 8B CF"
#define SIG_luaL_loadbuffer "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 48 8B E9 49 8B F1 48 8D 0D ?? ?? ?? ?? 49 8B F8 48 8B DA E8 ?? ?? ?? ?? 4C 8B"


#endif //SIGNATURES_ENGINE64_H
