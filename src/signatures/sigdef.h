//
// Created by exozg on 28/01/2025.
//

#ifndef SIGDEF_H
#define SIGDEF_H

#include "signatures.h"
#include "../hooks/Hooks.h"
#include "../util/Logger.h"
#include "lua.h"

#include <sigmatch/sigmatch.hpp>
using namespace sigmatch_literals;

#ifdef SIG_DEFINE

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset, true);

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset, true);

#else

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#endif

class SigSearchBase {
public:
    virtual ~SigSearchBase() = default;
    virtual void Search() const = 0;
};

template <typename OriginalFuncPtr>
class SigSearch: public SigSearchBase {
public:
    const char* name;
    sigmatch::signature signature;
    void*& address;
    OriginalFuncPtr Original = nullptr;
    uint32_t offset;
    bool first = false;

    SigSearch(const char* name, const sigmatch::signature &sig, void*& address, const int offset, bool first): name(name), signature(sig), address(address), offset(offset), first(first) {
        Signatures::GetInstance().Append(this);
    }

    void Search() const override {
        if (!Signatures::GetInstance().Search(signature, address, offset, first)) {
            MSML_LOG_INFO("Failed to find an address for %s", name);
        } else {
            MSML_LOG_INFO("Found %s 0x%x", name, address);
        }
    }

    void Install(void *detour) {
        Hooks::GetInstance().Install(address, detour, reinterpret_cast<void**>(&Original));
    }
};

CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(operator_new, void*, "56 57 8b 7c 24 2c 57 e8 ?? ?? ?? ?? 8b 4c 24 20 8b 54 24 18 83 c4 04 51 8b 4c 24 14 52 8b 54 24 14 51 52 8b c8", 0, uint32_t, uint32_t, uint32_t, const char*, uint32_t, uint32_t, const char*, uint32_t, uint32_t);

// exe specific
CREATE_NORMAL_CALLABLE_SIGNATURE(Rvl_Malloc, void*, "56 57 8b 7c 24 24 57 e8 3a 86 de ff 8b 4c 24 18 8b 54 24 10 83 c4 04 51 52 8b c8 e8 7f 9b de ff 8b f0 6a 01 56 e8 c5 19 de ff 83 c4 08 85 f6 75 09 57 e8 4b 26 df ff 83 c4 04 5f 8b c6 5e c3", 0, uint32_t, const char*, int32_t, uint32_t, const char*, int32_t, uint32_t);

namespace Revo::ResourceSystem {
    CREATE_MEMBER_CALLABLE_SIGNATURE(Init,void,"81 EC 20 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 1C 02 00 00 56 8B F1 ?? ?? ?? ?? ?? 84 C0 ?? ?? 32", 0);
}

namespace EA::ResourceMan {
    struct Key {
        uint64_t instance;
        uint32_t type;
        uint32_t group;
    };

    namespace Database {
        struct Database{};
    }

    namespace DatabasePackedFile {
        struct DatabasePackedFile {};
    };

    namespace Manager {
        struct Manager {};

        CREATE_NORMAL_CALLABLE_SIGNATURE(GetManager, Manager*, "a1 ?? ?? ?? ?? c3 cc cc cc cc cc cc cc cc cc cc 8b 4c 24 04 a1 c4 5b 09 01 89 0d c4 5b 09 01", 0);
        CREATE_MEMBER_CALLABLE_SIGNATURE(RegisterDatabase, void, "83 EC 10 53 55 56 57 8B F9 8D 4F 48 68 2C 95 E7 00 89 4C 24 18 ?? ?? ?? ?? ?? 80 7C", 0, bool, const Database::Database*, uint32_t);
    };

    namespace DatabaseDirectoryFiles {
        struct DatabaseDirectoryFiles{};
        CREATE_MEMBER_CALLABLE_SIGNATURE(ctor, DatabaseDirectoryFiles*, "53 56 8B F1 57 C7 06 94 4A E5 00 33 C0 8D 4E 04 87 01 C7 06", 0, const wchar_t*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(AddFile, void, "83 EC 68 8B 44 24 70 56 6A 02 50 8B F1 ?? ?? ?? ?? ?? 8B 8C 24 80 00 00 00 83 C4 08 52 50 51", 0, const Key*, const wchar_t*, const wchar_t*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(Attach, void, "53 55 56 33 DB 38 5C 24 10 57 8B F1 ?? ?? ?? ?? ?? ?? 8B 46 5C 3B C3 8B 7C 24 18", 0, bool, const Manager::Manager*, bool);
    }

    namespace PFRecordRead {
        struct PFRecordRead {};
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_1, void*, "56 8B F1 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 10 C7", 0, uint32_t, uint32_t, Key*, DatabasePackedFile::DatabasePackedFile*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_2, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 18 8B", 0, void*, uint32_t, bool, Key*, DatabasePackedFile::DatabasePackedFile*);
    }
}

#pragma region lua functions

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushvalue, void, "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89 11 8B 40 04 83", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getfield, int, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 51 8d 54 24 1c", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushcclosure, void, "56 8B 74 24 08 8B 46 10 8B 48 44 3B 48 40 57 ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8B 46 14 3B 46 28 ?? ?? 8B 46 44", 0, lua_State *L, lua_CFunction fn, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setfield, void, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 83 E9 08 51", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settop, void, "8B 4C 24 08 85 C9 8B 44 24 04 ?? ?? 8B 50 0C 03 C9 03 C9 03 C9 03 D1 39 50", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettop, int, "8b 4c 24 04 8b 41 08 2b 41 0c c1 f8 03 c3", 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tolstring, const char*, "56 8B 74 24 08 57 8B 7C 24 10 57 56 ?? ?? ?? ?? ?? 83 C4 08 83 78 04 04 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4", 0, lua_State *L, int index, size_t *len);

#pragma endregion

#endif //SIGDEF_H
