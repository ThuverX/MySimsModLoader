//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <sigmatch/sigmatch.hpp>
#include "../hooks/Hooks.h"
#include "../util/Logger.h"
#include "lua.h"

using namespace sigmatch_literals;

class Signatures {
public:
    static Signatures& GetInstance();
    bool Search(const sigmatch::signature &sig, void*& address, uint32_t offset);
private:
    Signatures();
    sigmatch::search_context context;
};
#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    static name ## ptr name = nullptr; \
    static SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset);

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    static name ## ptr name = nullptr; \
    static SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset);

template <typename OriginalFuncPtr>
class SigSearch {
public:
    const char* name;
    sigmatch::signature signature;
    void*& address;
    OriginalFuncPtr Original = nullptr;
    uint32_t offset;

    SigSearch(const char* name, const sigmatch::signature &sig, void*& address, const int offset): name(name), signature(sig), address(address), offset(offset) {
        Search();
    }

    void Search() const {
        if (!Signatures::GetInstance().Search(signature, address, offset)) {
            MSML_LOG_INFO("Failed to find an address for %s", name);
        }
    }

    void Install(void *detour) {
        if (address == nullptr)
            Search();
        Hooks::GetInstance().Install(address, detour, reinterpret_cast<void**>(&Original));
    }
};

namespace EA::ResourceMan {
    struct Key {
        uint64_t instance;
        uint32_t type;
        uint32_t group;
    };
    struct DatabasePackedFile {
    };
    namespace PFRecordRead {
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_1, void*, "56 8B F1 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 10 C7", 0, uint32_t, uint32_t, Key*, DatabasePackedFile*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_2, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 18 8B", 0, void*, uint32_t, bool, Key*, DatabasePackedFile*);
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

#endif //SIGNATURES_H