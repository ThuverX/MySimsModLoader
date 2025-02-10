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

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, full_name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#full_name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, full_name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#full_name, signature ## _sig, (void*&)name, offset, true);

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, full_name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#full_name, signature ## _sig, (void*&)name, offset, false);

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, full_name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void* this_ptr, __VA_ARGS__); \
    name ## ptr name = nullptr; \
    SigSearch<name ## ptr> name ## Hook(#full_name, signature ## _sig, (void*&)name, offset, true);

#else

#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, full_name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(name, full_name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, full_name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#define CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(name, full_name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void* this_ptr, __VA_ARGS__); \
    extern name ## ptr name; \
    extern SigSearch<name ## ptr> name ## Hook;

#endif

class SigSearchBase {
public:
    virtual ~SigSearchBase() = default;
    virtual void Search() const = 0;
    virtual std::string GetName() const = 0;
    virtual void* GetAddress() const = 0;
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

    SigSearch(std::string name, const sigmatch::signature &sig, void*& address, const int offset, bool first): name(name), signature(sig), address(address), offset(offset), first(first) {
        Signatures::GetInstance().Append(name, this);
    }

    void Search() const override {
        if (!Signatures::GetInstance().Search(signature, address, offset, first)) {
            MSML_LOG_INFO("Failed to find an address for %s", name.c_str());
        } else {
            MSML_LOG_INFO("Found %s 0x%x", name.c_str(), address);
        }
    }

    std::string GetName() const override {
        return name;
    }

    void* GetAddress() const override {
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
        LoadAllFiles = 7, // Specific to MySims
    };
}

CREATE_NORMAL_CALLABLE_SIGNATURE_FIRST(operator_new, operator_new, void*, "56 57 8b 7c 24 2c 57 e8 ?? ?? ?? ?? 8b 4c 24 20 8b 54 24 18 83 c4 04 51 8b 4c 24 14 52 8b 54 24 14 51 52 8b c8", 0, uint32_t, uint32_t, uint32_t, const char*, uint32_t, uint32_t, const char*, uint32_t, uint32_t);

// exe specific
CREATE_NORMAL_CALLABLE_SIGNATURE(Rvl_Malloc, Rvl_Malloc, void*, "56 57 8b 7c 24 24 57 e8 3a 86 de ff 8b 4c 24 18 8b 54 24 10 83 c4 04 51 52 8b c8 e8 7f 9b de ff 8b f0 6a 01 56 e8 c5 19 de ff 83 c4 08 85 f6 75 09 57 e8 4b 26 df ff 83 c4 04 5f 8b c6 5e c3", 0, uint32_t, const char*, int32_t, uint32_t, const char*, int32_t, uint32_t);

namespace Revo::ResourceSystem {

    // "81 EC 74 06 00 00 A1 C0 44 02 01 33 C4 89 84 24 70 06 00 00 A1 D8 52 E5" LoadAllPackages
    CREATE_MEMBER_CALLABLE_SIGNATURE(GetResource, Revo::ResourceSystem::GetResource, void*,"53 8B 5C 24 1C 85 DB 56 57 8B F1 8B C3 ?? ?? 8B 44 24 10 8B 16", 0, const EA::ResourceMan::Key& key, void** ,void*,void* database,void* factory,const EA::ResourceMan::Key*,uint32_t,uint32_t,uint32_t,uint32_t);
    CREATE_MEMBER_CALLABLE_SIGNATURE(LoadPackage, Revo::ResourceSystem::LoadPackage, void, "83 EC 24 53 55 56 57 8B 7C 24 38 6A 01 8B E9 68 C5 9D 1C 81", 0, const wchar_t* name, const wchar_t* path);
    CREATE_MEMBER_CALLABLE_SIGNATURE(Init, Revo::ResourceSystem::Init,void,"81 EC 20 02 00 00 A1 C0 44 02 01 33 C4 89 84 24 1C 02 00 00 56 8B F1 ?? ?? ?? ?? ?? 84 C0 ?? ?? 32", 0);
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

        CREATE_MEMBER_CALLABLE_SIGNATURE(GetResource, EA::ResourceMan::Manager::GetResource, void*,"83 EC 10 53 56 32 DB 38 5C 24 1C 57 8B F1 ?? ?? 8B 7C 24 38 85 FF", 0, bool,const Key& key, void**, void*, void* database, void* factory, const Key*, uint32_t, uint32_t, uint32_t, uint32_t);
        CREATE_NORMAL_CALLABLE_SIGNATURE(GetManager, EA::ResourceMan::Manager::GetManager, Manager*, "a1 ?? ?? ?? ?? c3 cc cc cc cc cc cc cc cc cc cc 8b 4c 24 04 a1 c4 5b 09 01 89 0d c4 5b 09 01", 0);
        CREATE_MEMBER_CALLABLE_SIGNATURE(RegisterDatabase, EA::ResourceMan::Manager::RegisterDatabase, void, "83 EC 10 53 55 56 57 8B F9 8D 4F 48 68 2C 95 E7 00 89 4C 24 18 ?? ?? ?? ?? ?? 80 7C", 0, bool add, void* pDatabase, uint32_t priority);
    };

    namespace DatabaseDirectoryFiles {
        struct DatabaseDirectoryFiles{};
        CREATE_MEMBER_CALLABLE_SIGNATURE(ctor, EA::ResourceMan::DatabaseDirectoryFiles::DatabaseDirectoryFiles, DatabaseDirectoryFiles*, "53 56 8B F1 57 C7 06 94 4A E5 00 33 C0 8D 4E 04 87 01 C7 06", 0, const wchar_t* path);
        CREATE_MEMBER_CALLABLE_SIGNATURE(AddRef, EA::ResourceMan::DatabaseDirectoryFiles::AddRef, void, "83 c1 04 b8 01 00 00 00 f0 0f c1 01 40 c3 cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc b8 40 e2 b6 04", 0);
        CREATE_MEMBER_CALLABLE_SIGNATURE(Init, EA::ResourceMan::DatabaseDirectoryFiles::Init, void, "80 79 08 00 b0 01 75 03 88 41 08 c3 cc cc cc cc 80 79 08 00 74 0b 8b 01", 0);
        CREATE_MEMBER_CALLABLE_SIGNATURE(Open, EA::ResourceMan::DatabaseDirectoryFiles::Open, void, "53 56 8b f1 80 7e 08 00 57 74 55", 0, IO::AccessFlags access_flags, IO::CD creation_disposition, bool, bool);
        CREATE_MEMBER_CALLABLE_SIGNATURE(AddFile, EA::ResourceMan::DatabaseDirectoryFiles::AddFile, void*, "83 EC 68 8B 44 24 70 56 6A 02 50 8B F1 ?? ?? ?? ?? ?? 8B 8C 24 80 00 00 00 83 C4 08 52 50 51", 0, const Key*, const wchar_t*, const wchar_t*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(Attach, EA::ResourceMan::DatabaseDirectoryFiles::Attach, void, "53 55 56 33 DB 38 5C 24 10 57 8B F1 ?? ?? ?? ?? ?? ?? 8B 46 5C 3B C3 8B 7C 24 18", 0, bool, void* pResourceMan, bool);

        CREATE_MEMBER_CALLABLE_SIGNATURE(UpdateRecordData, EA::ResourceMan::DatabaseDirectoryFiles::UpdateRecordData, void,"83 ec 20 53 57 8b f9 33 db 38 5f 08 0f 84 a3 00 00 00 8b 47 10 50", 0, void* eastl_vector);
        CREATE_MEMBER_CALLABLE_SIGNATURE(UpdateRecordData2, EA::ResourceMan::DatabaseDirectoryFiles::UpdateRecordData2, void,"81 ec 44 08 00 00 a1 ?? ?? ?? ?? 33 c4 89 84 24 40 08 00 00 8b 84 24 4c 08 00 00 53 55 56 57", 0, const wchar_t* path, void* eastl_vector);

    }

    namespace DatabaseDirectoryFilesAutoUpdate {
        // CREATE_MEMBER_CALLABLE_SIGNATURE_FIRST(AddDDF, EA::ResourceMan::DatabaseDirectoryFilesAutoUpdate::AddDDF, void,"53 55 8b 6c 24 10 56 8b f1 8b 46 0c 85 c0 57 8d 7e 04 b1 01 74 21 8b 55 00 8d a4 24 00 00 00 00 3b 50 10 8b f8 0f 92 c1 84 c9 74 05 8b 40 04 eb 02 8b 00 85 c0 75 e9 84 c9 8b df 74 10 3b 7e 08 74 2a 57", 0, void* database, bool);
    }

    namespace PFRecordRead {
        struct PFRecordRead {};
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_1, EA::ResourceMan::PFRecordRead::PFRecordRead_1, void*, "56 8B F1 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 10 C7", 0, uint32_t, uint32_t, Key*, DatabasePackedFile::DatabasePackedFile*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_2, EA::ResourceMan::PFRecordRead::PFRecordRead_2, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 18 8B", 0, void*, uint32_t, bool, Key*, DatabasePackedFile::DatabasePackedFile*);
    }

    namespace DDFRecord {
        struct DDFRecord {};
        CREATE_MEMBER_CALLABLE_SIGNATURE(ctor, EA::ResourceMan::DDFRecord::DDFRecord, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8D 7E", 0, void*, const Key*)
    }

    CREATE_NORMAL_CALLABLE_SIGNATURE(SetupDatabaseDirectoryFiles,EA::ResourceMan::SetupDatabaseDirectoryFiles,void,"81 ec e0 08 00 00 a1 ?? ?? ?? ?? 33 c4 89 84 24 dc 08 00 00 8b 84 24 e4 08 00 00 53 55 8b ac 24 fc 08 00 00 56 8b b4 24 fc 08 00 00 89 44 24 0c 8b 84 24 f4 08 00 00 57 33 db 33 ff 3b c3 89 44 24 2c 89 7c 24 14 75 11", 0, const wchar_t*, void* manager, uint32_t, bool (*)(const wchar_t*, void*), void*, int)
}

#pragma region lua functions

CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushvalue, lua_pushvalue, void, "8B 44 24 08 56 8B 74 24 08 50 56 ?? ?? ?? ?? ?? 8B 4E 08 8B 10 89 11 8B 40 04 83", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_getfield,lua_getfield, int, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 51 8d 54 24 1c", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_pushcclosure,lua_pushcclosure, void, "56 8B 74 24 08 8B 46 10 8B 48 44 3B 48 40 57 ?? ?? 56 ?? ?? ?? ?? ?? 83 C4 04 8B 46 14 3B 46 28 ?? ?? 8B 46 44", 0, lua_State *L, lua_CFunction fn, int n);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_setfield,lua_setfield, void, "8B 44 24 08 83 EC 08 53 56 8B 74 24 14 57 50 56 ?? ?? ?? ?? ?? 8B 54 24 28 8B F8 8B C2 83 C4 08 8D 58 01 8A 08 83 C0 01 84 C9 ?? ?? 2B C3 50 52 56 ?? ?? ?? ?? ?? 8B 4E 08 83 E9 08 51", 0, lua_State *L, int index, const char *k);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_settop,lua_settop, void, "8B 4C 24 08 85 C9 8B 44 24 04 ?? ?? 8B 50 0C 03 C9 03 C9 03 C9 03 D1 39 50", 0, lua_State *L, int index);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_gettop,lua_gettop, int, "8b 4c 24 04 8b 41 08 2b 41 0c c1 f8 03 c3", 0, lua_State *L);
CREATE_NORMAL_CALLABLE_SIGNATURE(lua_tolstring,lua_tolstring, const char*, "56 8B 74 24 08 57 8B 7C 24 10 57 56 ?? ?? ?? ?? ?? 83 C4 08 83 78 04 04 ?? ?? 50 56 ?? ?? ?? ?? ?? 83 C4", 0, lua_State *L, int index, size_t *len);

#pragma endregion

#endif //SIGDEF_H
