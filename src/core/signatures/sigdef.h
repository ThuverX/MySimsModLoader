//
// Created by exozg on 28/01/2025.
//

#ifndef SIGDEF_H
#define SIGDEF_H

#include "../../include/lua.h"

#include <sigmatch/sigmatch.hpp>

#include "Signatures.h"
#include "../../EA/IO/Constants.h"
#include "../../EA/ResourceMan/RecordInfo.h"
#include "../../EA/ResourceMan/ResourceKey.h"
#include "../hooks/Hooks.h"
#include "../system/Logger.h"
#include "EASTL/hash_map.h"

using namespace sigmatch_literals;

#pragma region defs

#ifdef _WIN64
#ifdef SIG_DEFINE
// create definitions for win64

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature64 ## _sig, (void*&)name, offset, false); \

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature64 ## _sig, (void*&)name, offset, false); \

#define CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__, ...); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature64 ## _sig, (void*&)name, offset, false); \

#else
// usage for win64

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__, ...); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#endif
#endif

#ifndef _WIN64
#ifdef SIG_DEFINE
// create definitions for win32

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, false);
#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, false);
#define CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__cdecl *name ## ptr)(ns* this_ptr, __VA_ARGS__, ...); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, false);
#else
// usage for win32

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(name, ns, retn, signature, signature64, offset, ...) \
    typedef retn(__cdecl *name ## ptr)(ns* this_ptr, __VA_ARGS__, ...); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#endif
#endif

template<typename OriginalFuncPtr>
class SigSearch : public msml::core::SigSearchBase {
public:
    std::string name;
    sigmatch::signature signature;
    void *&address;
    OriginalFuncPtr Original = nullptr;
    uint32_t offset;
    bool first = false;

    SigSearch(const std::string &name, const sigmatch::signature &sig, void *&address, const int offset,
              bool first): name(name), signature(sig), address(address), offset(offset), first(first) {
        msml::core::Signatures::GetInstance().Append(name, this);
    }

    [[nodiscard]] bool Search() const override {
        if (!msml::core::Signatures::GetInstance().Search(signature, address, offset, first)) {
            MSML_LOG_ERROR("Failed to find an address for %s", name.c_str());
            return false;
        }
        MSML_LOG_DEBUG("Found %s %p", name.c_str(), address);
        return true;
    }

    [[nodiscard]] std::string GetName() const override {
        return name;
    }

    [[nodiscard]] void *GetAddress() const override {
        return address;
    }

    void ApplyAddress(void *addr) const override {
        address = addr;
    }

    void Install(void *detour) {
        msml::core::Hooks::Install(address, detour, reinterpret_cast<void **>(&Original));
    }
};

#pragma endregion

#pragma pack(push, 1)

typedef void (*VTable)();

namespace Revo {
    namespace ResourceSystem {
        struct ResourceSystem {
            VTable *vTable;
        };

        CREATE_MEMBER_CALLABLE_SIGNATURE(Init, ResourceSystem, void,
                                         "81 EC 20 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 1C 02 00 00 56 8B F1 ?? ?? ?? ?? ?? 84 C0 ?? ?? 32",
                                         "40 55 57 48 8D AC 24 D8 FD FF FF 48 81 EC 28 03 00 00 48 8B 05 ?? ?? ?? ??",
                                         0);
    }

    namespace App {
        struct App {
            void *tinyXmlImplementation; // TinyXmlImplementation::vftable
            void *tiXmlElement;
        };

        CREATE_NORMAL_CALLABLE_SIGNATURE(GetCorrectLoadFolder, App, size_t,
                                         "81 EC 24 04 00 00 A1 C0 44 02 01 33 C4 89 84 24 20 04 00 00 A1 84 7E F0 00 53 8B 9C 24 38 04 00 00",
                                         "40 53 55 56 57 48 81 EC 98 06 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4", 0,
                                         wchar_t*,
                                         bool, const wchar_t*, const wchar_t*)

        CREATE_MEMBER_CALLABLE_SIGNATURE(ReadXMLFromPath, App, App*,
                                         "81 EC 90 06 00 00 A1 C0 44 02 01 33 C4 89 84 24 8C 06 00 00 8B 84 24 9C 06 00 00",
                                         "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 A8 F9 FF FF 48 81 EC 58 07 00 00 48 8B 05 ?? ?? ?? ??",
                                         0, const char*, void*, const char*, bool, double*, const char*);

        CREATE_NORMAL_CALLABLE_SIGNATURE(ReadXMLFromStream, App, App*,
                                         "83 EC 14 53 55 56 57 E8 ?? ?? ?? ?? 80 7C 24 3C 00 8B 4C 24 2C",
                                         "48 89 5C 24 10 48 89 6C 24 20 4C 89 44 24 18 56 57 41 54 41 55 41 57 48 83 EC 70",
                                         0, const char**, EA::IO::IStream*, void*, const char*, double*);
    }

    namespace ObjectFolder {
        struct ObjectFolder {
            VTable *vTable;
        };

        struct ObjectDef {
            VTable *vtable;
            uint32_t values[63];
        };

        struct CharacterDef {
            VTable *vtable;
            uint32_t values[91];
        };

        // CREATE_MEMBER_CALLABLE_SIGNATURE(LoadAllObjectDefs, ObjectFolder, void, "",
        //                                  "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 41 54 41 55 41 56 41 57 48 8D AC 24 20 FF FF FF 48 81 EC E0 01 00 00 48 8B 05 ?? ?? ?? ??",
        //                                  0);
        //
        // CREATE_NORMAL_CALLABLE_SIGNATURE(AddCharacterDef, ObjectFolder, CharacterDef*, "",
        //                                  "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 60 0F B6 DA 48 8B F1 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 04 00 00 00 E8 ?? ?? ?? ?? 33 FF 48 89 BC 24 80 00 00 00 48 89 7C 24 30 48 89 7C 24 28 88 5C 24 20 4C 8B 0D 0E 96 81 00 4C 8D 84 24 80 00 00 00 48 8B D6",
        //                                  0, const char* path, bool);
        //
        // CREATE_NORMAL_CALLABLE_SIGNATURE(AddObjectDef, ObjectFolder, ObjectDef*, "",
        //                                  "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 60 0F B6 DA 48 8B F1 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 04 00 00 00 E8 ?? ?? ?? ?? 33 FF 48 89 BC 24 80 00 00 00 48 89 7C 24 30 48 89 7C 24 28 88 5C 24 20 4C 8B 0D 7E 67 80 00 4C 8D 84 24 80 00 00 00 48 8B D6",
        //                                  0, const char* path, bool);
    }
}

namespace EA {
    namespace ArgScript {
        struct ArgScript {
        };

        CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(
            Output, ArgScript,
            void,
            "8B 4C 24 04 83 EC 10 85 C9 0F 84 C4 00 00 00 8B 01",
            "48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 56 57 48 83 EC 40 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ??",
            0, void*, const char* fmt);

        CREATE_MEMBER_CALLABLE_SIGNATURE_VA_ARGS(
            ErrorOutput, ArgScript,
            void,
            "83 EC 14 8B 0D E4 FA 08 01 85 C9 B8 90 0E 09 01",
            "48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 57 48 83 EC 28 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C7 43 18 05 00 00 00 48 8D 05 ?? ?? ?? ?? 48 89 03",
            0, const char* fmt);
    }

    namespace ResourceMan {
        class IRecord;

        namespace PFIndexModifiable {
            struct PFIndexModifiable {
                VTable *vtable;
                eastl::hash_map<Key, RecordInfo> itemMap;
            };
        }

        namespace DatabasePackedFile {
            struct DatabasePackedFile {
                VTable *vtable;
            };

            CREATE_MEMBER_CALLABLE_SIGNATURE(OpenRecord, DatabasePackedFile, bool,
                                             "E9 FE E7 31 00 E9 49 A6 0F 00 E9 94 94 1F 00",
                                             "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 4C 89 4C 24 20 57 41 54 41 55 41 56 41 57 48 81 EC A0 00 00 00 4D 8B F8 4C 8B F2",
                                             0,
                                             const Key& key, IRecord** pDstRecord,
                                             IO::AccessFlags accessFlags, IO::CD cd, int, EA::ResourceMan::RecordInfo*);

            CREATE_MEMBER_CALLABLE_SIGNATURE(Open, DatabasePackedFile, bool,
                                             "56 8B F1 8A 4C 24 10 32 C0 84 C9",
                                             "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 30 48 8B D9 41 0F B6 E9 48 8D 0D ?? ?? ?? ?? 41 8B F0",
                                             0,
                                             EA::IO::AccessFlags access_flags, EA::IO::CD creation_disposition, bool,
                                             bool);

            CREATE_MEMBER_CALLABLE_SIGNATURE(GetIndex, DatabasePackedFile, PFIndexModifiable::PFIndexModifiable*,
                                             "E9 59 58 32 00 E9 14 14 28 00 E9 2F 08 22 00",
                                             "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 83 D0 02 00 00 48 83 C4 20 5B",
                                             0);

            CREATE_MEMBER_CALLABLE_SIGNATURE(GetLocation, DatabasePackedFile, wchar_t*,
                                             "E9 28 AE 14 00 E9 93 85 3C 00 E9 FE 15 33 00",
                                             "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 43 20 48 83 C4 20 5B C3 CC 48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20",
                                             0);
        }

        namespace Manager {
            struct Manager {
            };

            CREATE_MEMBER_CALLABLE_SIGNATURE(SetTypename, Manager, bool,
                                             "83 EC 08 56 8B F1 57 8D BE 38 01 00 00 68 2C 95 E7 00 8B CF E8 ?? ?? ?? ?? 8D 44 24 14 50 8D 4C 24 0C 81 C6 E8 00 00 00 51 8B CE E8 ?? ?? ?? ?? 8B 56 08 8B 4E 04 8B 44 24 08 3B 04 91",
                                             "48 89 5C 24 18 55 56 57 48 81 EC 80 00 00 00 49 8B F0 8B FA 48 8B E9", 0,
                                             uint32_t nTypeID, const wchar_t* pExtension);

            CREATE_MEMBER_CALLABLE_SIGNATURE(RegisterDatabase, Manager, void,
                                             "83 EC 10 53 55 56 57 8B F9 8D 4F 48 68 2C 95 E7 00 89 4C 24 18",
                                             "48 89 5C 24 10 48 89 6C 24 18 56 57 41 54 41 56 41 57 48 83 EC 40 41 8B E9 49 8B F0 0F B6 FA",
                                             0, bool add, void* pDatabase, uint32_t priority);
        }

        namespace DatabaseDirectoryFiles {
            struct DatabaseDirectoryFiles {
                VTable *vtable;
                size_t padding1[2];
                wchar_t *location;
            };

            CREATE_MEMBER_CALLABLE_SIGNATURE(OpenRecord, DatabaseDirectoryFiles, bool,
                                             "81 EC 18 04 00 00 A1 C0 44 02 01 33 C4",
                                             "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC A8 04 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 90 04 00 00 45 8B E1 4C 89 44 24 48 4C 8B EA",
                                             0,
                                             const Key& key, IRecord** pDstRecord,
                                             IO::AccessFlags accessFlags, IO::CD cd, int, EA::ResourceMan::RecordInfo*);
            CREATE_MEMBER_CALLABLE_SIGNATURE(AddFile, DatabaseDirectoryFiles, void*,
                                             "83 EC 68 8B 44 24 70 56 6A 02 50 8B F1 ?? ?? ?? ?? ?? 8B 8C 24 80 00 00 00 83 C4 08 52 50 51",
                                             "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 B9 48 81 EC C0 00 00 00 49 8B F9 49 8B D8",
                                             0, const Key&, const wchar_t* path, const wchar_t* name);
        }
    }

    namespace ScriptOs::LuaScriptingSystem {
        struct LuaScriptingSystem {
            VTable *vtable;
            size_t padding[3];
            lua_State *state;
        };

        CREATE_MEMBER_CALLABLE_SIGNATURE(Startup, LuaScriptingSystem, void,
                                         "83 ec 30 53 55 56 57 8b f1 c7 44 24 10 1c 00 00 00 c7 44 24 14 c8 32 00 00 c7 44 24 18 20 00 00 00 c7 44 24 1c 50 46 00 00 c7 44 24 20 28 00 00 00 c7 44 24 24 52 1c 00 00 c7 44 24 28 50 00 00 00 c7 44 24 2c b6 35 00 00 c7 44 24 30 a0 00 00 00 c7 44 24 34 b2 0c 00 00 c7 44 24 38 40 01 00 00 c7 44 24 3c dc 05 00 00 33 db",
                                         "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 81 EC C0 00 00 00 48 8B E9 48 8D 0D ?? ?? ?? ??",
                                         0);
    }
}

#pragma region lua functions

// These are correct, but very hard to find...
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushvalue, lua, void,
                                 "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89 11 8B 40 04 83",
                                 "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 4C 8B 47 10 48 8B 5C 24 30 48 8B 10 49 89 10",
                                 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getfield, lua, int,
                                 "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 51 8d 54 24 1c",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F1 49 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 48 8B D8 49 C7 C0 FF FF FF FF 66 0F 1F 84 00 00 00 00 00 49 FF C0 42 80 3C 07 00 75 F6 48 8B D7 48 8B CE E8 ?? ?? ?? ?? 4C 8B 4E 10 4C 8D 44 24 20 48 8B D3 48 89 44 24 20 48 8B CE",
                                 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushcclosure, lua, void,
                                 "56 8B 74 24 08 8B 46 10 8B 48 44 3B 48 40 57 ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8B 46 14 3B 46 28 ?? ?? 8B 46 44",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 49 63 F8 48 8D 0D ?? ?? ?? ?? 48 8B F2 E8 ?? ?? ?? ?? 4C 8B 4B 20 49 8B 41 70 49 39 41 78",
                                 0, lua_State *L, lua_CFunction fn, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setfield, lua, void,
                                 "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 83 E9 08 51",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B F1 49 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 48 8B D8 49 C7 C0 FF FF FF FF 66 0F 1F 84 00 00 00 00 00 49 FF C0 42 80 3C 07 00 75 F6 48 8B D7 48 8B CE E8 ?? ?? ?? ?? 4C 8B 4E 10 4C 8D 44 24 20 49 83 E9 10 48 89 44 24 20 48 8B D3",
                                 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settop, lua, void,
                                 "8B 4C 24 08 85 C9 8B 44 24 04 ?? ?? 8B 50 0C 03 C9 03 C9 03 C9 03 D1 39 50",
                                 "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 48 63 FA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 85 FF 78 50 48 8B 53 18 4C 8B C7",
                                 0,
                                 lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettop, lua, int, "8b 4c 24 04 8b 41 08 2b 41 0c c1 f8 03 c3",
                                 "40 53 48 83 EC 20 48 8B D9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 43 10 48 2B 43 18 48 C1 F8 04 48 83 C4 20",
                                 0,
                                 lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tolstring, lua, const char*,
                                 "56 8B 74 24 08 57 8B 7C 24 10 57 56 ?? ?? ?? ?? ?? 83 C4 08 83 78 04 04 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 49 8B D8 48 8D 0D ?? ?? ?? ?? 8B F2 E8 ?? ?? ?? ?? 8B D6 48 8B CF E8 ?? ?? ?? ??",
                                 0, lua_State *L, int index, size_t *len);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_newstate, lua, lua_State*,
                                 "53 55 8B 6C 24 0C 57 8B 7C 24 14 33 DB 53 68 4C 01 00 00 53 53 57 ?? ?? 83 C4 14 3B C3 ?? ?? 5F",
                                 "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 30 48 8B E9 48 8B F2 48 8D 0D ?? ?? ?? ??",
                                 0, lua_Alloc f, void* ud);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_ref, lua, int,
                                 "53 8B 5C 24 0C 8D 83 0F 27 00 00 3D 0F 27 00 00 56 8B 74 24 0C ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8D",
                                 "48 89 5C 24 10 57 48 83 EC 20 48 8B D9 8B FA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 87 0F 27 00 00 3D 0F 27 00 00 77 0C",
                                 0, lua_State *L, int t);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadfile, lua, int,
                                 "81 EC 0C 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 08 02 00 00 55 8B AC 24 18 02 00 00 56 8B B4 24 18 02 00 00 57 56 ?? ?? ?? ?? ?? 8B",
                                 "48 89 5C 24 20 55 56 57 48 81 EC 40 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4",
                                 0, lua_State *L, const char* filename);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_remove, lua, void,
                                 "8b 44 24 08 56 8b 74 24 08 50 56 e8 ?? ?? ?? ?? 83 c0 08 83 c4 08 3b 46 08 73 18 eb 03 8d 49 00",
                                 "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 48 8B 4F 10 48 83 C0 10 48 3B C1",
                                 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_insert, lua, void,
                                 "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 83 C4 08 3B C8 ?? ?? 8D 9B 00 00 00",
                                 "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B D3 48 8B CF E8 ?? ?? ?? ?? 48 8B 57 10 4C 8B C8 48 3B D0 76 19",
                                 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pcall, lua, int,
                                 "8B 44 24 10 83 EC 08 85 C0 56 8B 74 24 10 ?? ?? 33 C9 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4 08 2B",
                                 "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 40 48 8B D9 41 8B F9 48 8D 0D ?? ?? ?? ?? 41 8B F0 8B EA E8 ?? ?? ?? ??",
                                 0, lua_State *L, int nargs, int nresults, int errfunc);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawgeti, lua, void,
                                 "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4C 24 18 8B 10 51 52 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 41 8B F8 48 8D 0D ?? ?? ?? ?? 8B DA E8 ?? ?? ?? ?? 8B D3 48 8B CE E8 ?? ?? ?? ?? 8B D7 48 8B 08 E8 ?? ?? ?? ?? 48 8B 56 10",
                                 0, lua_State *L, int index, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_checklstring, lua, const char *,
                                 "8B 44 24 0C 53 56 8B 74 24 0C 57 8B 7C 24 14 50 57 56 ?? ?? ?? ?? ?? 8B D8 83 C4 0C 85 DB ?? ?? 55",
                                 "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 49 8B D8 48 8D 0D ?? ?? ?? ?? 8B FA E8 ?? ?? ?? ?? 8B D7 48 8B CE E8 ?? ?? ?? ??",
                                 0, lua_State *L, int narg, size_t *l);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushstring, lua, void,
                                 "55 8B 6C 24 0C 85 ED ?? ?? 8B 44 24 08 8B 48 08 89 69 04 83 40 08 08 5D",
                                 "48 89 6C 24 18 56 48 83 EC 20 48 8B F1 48 8B EA 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 85 ED 75 17 48 8B 46 10 89 68 08",
                                 0,
                                 lua_State *L, const char *s);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaB_loadstring, lua, int,
                                 "51 56 8B 74 24 0C 57 8D 44 24 08 50 6A 01 56 ?? ?? ?? ?? ?? 6A 00 8B F8 57 6A 02 56",
                                 "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 4C 8D 44 24 38 BA 01 00 00 00 48 8B CF",
                                 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadbuffer, void, int,
                                 "83 EC 08 8B 44 24 10 8B 54 24 18 8B 4C 24 14 52 89 44 24 04 8D 44 24 04 50 89 4C 24 0C 8B 4C 24 14",
                                 "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 48 8B E9 49 8B F1 48 8D 0D ?? ?? ?? ?? 49 8B F8 48 8B DA E8 ?? ?? ?? ??",
                                 0, lua_State * L, const char * buff, size_t sz, const char * name);

#pragma endregion

#pragma pack(pop)

#endif //SIGDEF_H
