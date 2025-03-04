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

namespace EA::ResourceMan {
    struct Key;
}

using namespace sigmatch_literals;
#ifdef SIG_DEFINE

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, ns, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, true);

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, ns, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#ns "::" #name, signature ## _sig, (void*&)name, offset, true);

#else

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, ns, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, ns, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, ns, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, ns, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(ns* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#endif

class SigSearchBase {
public:
    virtual ~SigSearchBase() = default;
    [[nodiscard]] virtual bool Search() const = 0;
    [[nodiscard]] virtual std::string GetName() const = 0;
    [[nodiscard]] virtual void* GetAddress() const = 0;
    virtual void ApplyAddress(void* addr) const = 0;
};

template <typename OriginalFuncPtr>
class SigSearch: public SigSearchBase {
public:
    std::string name;
    sigmatch::signature signature;
    void*& address;
    OriginalFuncPtr Original = nullptr;
    uint32_t offset;
    bool first = false;

    SigSearch(const std::string &name, const sigmatch::signature &sig, void*& address, const int offset, bool first): name(name), signature(sig), address(address), offset(offset), first(first) {
        Signatures::GetInstance().Append(name, this);
    }

    [[nodiscard]] bool Search() const override {
        if (!Signatures::GetInstance().Search(signature, address, offset, first)) {
            MSML_LOG_ERROR("Failed to find an address for %s", name.c_str());
            return false;
        }
        MSML_LOG_DEBUG("Found %s 0x%x", name.c_str(), address);
        return true;
    }

    [[nodiscard]] std::string GetName() const override {
        return name;
    }

    [[nodiscard]] void* GetAddress() const override {
        return address;
    }

    void ApplyAddress(void *addr) const override {
        address = addr;
    }

    void Install(void *detour) {
        Hooks::GetInstance().Install(address, detour, reinterpret_cast<void**>(&Original));
    }
};


namespace IO {
    enum class AccessFlags : int
    {
        /// No specified flags. Also used to indicate that a given IO stream is closed.
        None = 0x00,
        /// Used for identifying read access to an entity.
        Read = 0x01,
        /// Used for identifying write access to an entity.
        Write = 0x02,
        /// Used for identifying both read and write access to an entity.
        ReadWrite = 0x03
    };

    /// Creation disposition. Specifies aspects of how to create or not create a file during opening of it.
    enum class CD : int
    {
        /// Fails if file already exists.
        CreateNew = 1,
        /// Never fails, always opens or creates and truncates to 0.
        CreateAlways = 2,
        /// Fails if file doesn't exist, keeps contents.
        OpenExisting = 3,
        /// Never fails, creates if doesn't exist, keeps contents.
        OpenAlways = 4,
        /// Fails if file doesn't exist, but truncates to 0 if it does.
        TruncateExisting = 5,
        /// Default (implementation-specific) disposition
        Default = 6,
        // Specific to MySims
        LoadAllFiles = 7,
    };
}

namespace Revo::ResourceSystem {
    struct ResourceSystem{};
    CREATE_MEMBER_CALLABLE_SIGNATURE(GetResource, ResourceSystem, void*,"53 8B 5C 24 1C 85 DB 56 57 8B F1 8B C3 ?? ?? 8B 44 24 10 8B 16", 0, const EA::ResourceMan::Key& key, void** ,void*,void* database,void* factory,const EA::ResourceMan::Key*,uint32_t,uint32_t,uint32_t,uint32_t);
    CREATE_MEMBER_CALLABLE_SIGNATURE(LoadPackage, ResourceSystem, void, "83 EC 24 53 55 56 57 8B 7C 24 38 6A 01 8B E9 68 C5 9D 1C 81", 0, const wchar_t* name, const wchar_t* path);
    CREATE_MEMBER_CALLABLE_SIGNATURE(Init, ResourceSystem,void,"81 EC 20 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 1C 02 00 00 56 8B F1 ?? ?? ?? ?? ?? 84 C0 ?? ?? 32", 0);
}
namespace EA {
    namespace ResourceMan {
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
            CREATE_NORMAL_CALLABLE_SIGNATURE(GetManager, Manager, Manager*, "a1 ?? ?? ?? ?? c3 cc cc cc cc cc cc cc cc cc cc 8b 4c 24 04 a1 c4 5b 09 01 89 0d c4 5b 09 01", 0);
            CREATE_MEMBER_CALLABLE_SIGNATURE(GetResource, Manager, void*,"83 EC 10 53 56 32 DB 38 5C 24 1C 57 8B F1 ?? ?? 8B 7C 24 38 85 FF", 0, bool,const Key& key, void**, void*, void* database, void* factory, const Key*, uint32_t, uint32_t, uint32_t, uint32_t);
            CREATE_MEMBER_CALLABLE_SIGNATURE(RegisterDatabase, Manager, void, "83 EC 10 53 55 56 57 8B F9 8D 4F 48 68 ?? 95 E7 00 89 4C 24 18 ?? ?? ?? ?? ?? 80 7C 24 24", 0, bool add, void* pDatabase, uint32_t priority);
        }

        namespace DatabaseDirectoryFiles {
            struct DatabaseDirectoryFiles{};
            CREATE_MEMBER_CALLABLE_SIGNATURE(ctor, DatabaseDirectoryFiles, DatabaseDirectoryFiles*, "53 56 8B F1 57 C7 06 94 4A E5 00 33 C0 8D 4E 04 87 01 C7 06", 0, const wchar_t* path);
            CREATE_MEMBER_CALLABLE_SIGNATURE(AddRef, DatabaseDirectoryFiles, void, "83 c1 04 b8 01 00 00 00 f0 0f c1 01 40 c3 cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc b8 40 e2 b6 04", 0);
            CREATE_MEMBER_CALLABLE_SIGNATURE(Init, DatabaseDirectoryFiles, void, "80 79 08 00 b0 01 75 03 88 41 08 c3 cc cc cc cc 80 79 08 00 74 0b 8b 01", 0);
            CREATE_MEMBER_CALLABLE_SIGNATURE(Open, DatabaseDirectoryFiles, void, "53 56 8b f1 80 7e 08 00 57 74 55", 0, IO::AccessFlags access_flags, IO::CD creation_disposition, bool, bool);
            CREATE_MEMBER_CALLABLE_SIGNATURE(AddFile, DatabaseDirectoryFiles, void*, "83 EC 68 8B 44 24 70 56 6A 02 50 8B F1 ?? ?? ?? ?? ?? 8B 8C 24 80 00 00 00 83 C4 08 52 50 51", 0, const Key*, const wchar_t*, const wchar_t*);
            CREATE_MEMBER_CALLABLE_SIGNATURE(Attach, DatabaseDirectoryFiles, void, "53 55 56 33 DB 38 5C 24 10 57 8B F1 ?? ?? ?? ?? ?? ?? 8B 46 5C 3B C3 8B 7C 24 18", 0, bool, void* pResourceMan, bool);
            CREATE_MEMBER_CALLABLE_SIGNATURE(AddExtensionMapping, DatabaseDirectoryFiles, void, "83 EC 28 53 55 56 8B E9 57 8D 7D 68 68 ?? 8D E7 00 8B CF 89 7C 24 14 ?? ?? ?? ?? ?? 8B", 0, const wchar_t* extension, uint32_t type);

            CREATE_MEMBER_CALLABLE_SIGNATURE(UpdateRecordData, DatabaseDirectoryFiles, void,"83 ec 20 53 57 8b f9 33 db 38 5f 08 0f 84 a3 00 00 00 8b 47 10 50", 0, void* eastl_vector);
            CREATE_MEMBER_CALLABLE_SIGNATURE(UpdateRecordData2, DatabaseDirectoryFiles, void,"81 ec 44 08 00 00 a1 ?? ?? ?? ?? 33 c4 89 84 24 40 08 00 00 8b 84 24 4c 08 00 00 53 55 56 57", 0, const wchar_t* path, void* eastl_vector);

        }

        namespace PFRecordRead {
            struct PFRecordRead {};
            CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_1, PFRecordRead, void*, "56 8B F1 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 10 C7", 0, uint32_t, uint32_t, Key*, DatabasePackedFile::DatabasePackedFile*);
            CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_2, PFRecordRead, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 18 8B", 0, void*, uint32_t, bool, Key*, DatabasePackedFile::DatabasePackedFile*);
        }

        namespace DDFRecord {
            struct DDFRecord {};
            CREATE_MEMBER_CALLABLE_SIGNATURE(ctor, DDFRecord, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8D 7E", 0, void*, const Key*)
        }

        CREATE_NORMAL_CALLABLE_SIGNATURE(SetupDatabaseDirectoryFiles, ResourceMan,void,"81 ec e0 08 00 00 a1 ?? ?? ?? ?? 33 c4 89 84 24 dc 08 00 00 8b 84 24 e4 08 00 00 53 55 8b ac 24 fc 08 00 00 56 8b b4 24 fc 08 00 00 89 44 24 0c 8b 84 24 f4 08 00 00 57 33 db 33 ff 3b c3 89 44 24 2c 89 7c 24 14 75 11", 0, const wchar_t*, void* manager, IO::AccessFlags, bool (*)(const wchar_t*, void*), void*, int)
    }

    namespace ScriptOs::LuaScriptingSystem {
        struct LuaScriptingSystem {
            uint8_t padding[0x10];
            lua_State* state;
        };
        CREATE_MEMBER_CALLABLE_SIGNATURE(Startup, LuaScriptingSystem, void, "83 ec 30 53 55 56 57 8b f1 c7 44 24 10 1c 00 00 00 c7 44 24 14 c8 32 00 00 c7 44 24 18 20 00 00 00 c7 44 24 1c 50 46 00 00 c7 44 24 20 28 00 00 00 c7 44 24 24 52 1c 00 00 c7 44 24 28 50 00 00 00 c7 44 24 2c b6 35 00 00 c7 44 24 30 a0 00 00 00 c7 44 24 34 b2 0c 00 00 c7 44 24 38 40 01 00 00 c7 44 24 3c dc 05 00 00 33 db", 0);
    }
}

#pragma region lua functions

// These are correct, but very hard to find...
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushvalue, lua, void, "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89 11 8B 40 04 83", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getfield, lua, int, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 51 8d 54 24 1c", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushcclosure, lua, void, "56 8B 74 24 08 8B 46 10 8B 48 44 3B 48 40 57 ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8B 46 14 3B 46 28 ?? ?? 8B 46 44", 0, lua_State *L, lua_CFunction fn, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setfield, lua, void, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 83 E9 08 51", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settop, lua, void, "8B 4C 24 08 85 C9 8B 44 24 04 ?? ?? 8B 50 0C 03 C9 03 C9 03 C9 03 D1 39 50", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettop, lua, int, "8b 4c 24 04 8b 41 08 2b 41 0c c1 f8 03 c3", 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tolstring, lua, const char*, "56 8B 74 24 08 57 8B 7C 24 10 57 56 ?? ?? ?? ?? ?? 83 C4 08 83 78 04 04 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4", 0, lua_State *L, int index, size_t *len);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_newstate, lua, lua_State*, "53 55 8B 6C 24 0C 57 8B 7C 24 14 33 DB 53 68 4C 01 00 00 53 53 57 ?? ?? 83 C4 14 3B C3 ?? ?? 5F", 0, lua_Alloc f, void* ud);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_ref, lua, int, "53 8B 5C 24 0C 8D 83 0F 27 00 00 3D 0F 27 00 00 56 8B 74 24 0C ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8D", 0, lua_State *L, int t);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadfile, lua, int, "81 EC 0C 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 08 02 00 00 55 8B AC 24 18 02 00 00 56 8B B4 24 18 02 00 00 57 56 ?? ?? ?? ?? ?? 8B", 0, lua_State *L, const char* filename);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_remove, lua, void, "8b 44 24 08 56 8b 74 24 08 50 56 e8 ?? ?? ?? ?? 83 c0 08 83 c4 08 3b 46 08 73 18 eb 03 8d 49 00", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_insert, lua, void, "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 83 C4 08 3B C8 ?? ?? 8D 9B 00 00 00", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pcall, lua, int, "8B 44 24 10 83 EC 08 85 C0 56 8B 74 24 10 ?? ?? 33 C9 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4 08 2B", 0, lua_State *L, int nargs, int nresults, int errfunc);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_rawgeti, lua, void, "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4C 24 18 8B 10 51 52 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89", 0, lua_State *L, int index, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_checklstring, lua, const char *, "8B 44 24 0C 53 56 8B 74 24 0C 57 8B 7C 24 14 50 57 56 ?? ?? ?? ?? ?? 8B D8 83 C4 0C 85 DB ?? ?? 55", 0, lua_State *L, int narg, size_t *l);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushstring, lua, void, "55 8B 6C 24 0C 85 ED ?? ?? 8B 44 24 08 8B 48 08 89 69 04 83 40 08 08 5D", 0, lua_State *L, const char *s);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaB_loadstring, lua, int, "51 56 8B 74 24 0C 57 8D 44 24 08 50 6A 01 56 ?? ?? ?? ?? ?? 6A 00 8B F8 57 6A 02 56", 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaG_errormsg, void, int, "56 8B 74 24 08 8B 46 60 85 C0 ?? ?? 57 8B 7E 20 03 F8 83 7F 04 06", 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(luaL_loadbuffer, void, int, "83 EC 08 8B 44 24 10 8B 54 24 18 8B 4C 24 14 52 89 44 24 04 8D 44 24 04 50 89 4C 24 0C 8B 4C 24 14", 0, lua_State * L, const char * buff, size_t sz,const char * name);

#pragma endregion

#endif //SIGDEF_H
