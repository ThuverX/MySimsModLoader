//
// Created by exozg on 07/08/2025.
//

#ifndef SIGDEF_H
#define SIGDEF_H

#include "../../EA/IO/Constants.h"
#include "../../EA/ResourceMan/IResource.h"
#include "../../EA/ResourceMan/RecordInfo.h"
#include "../../EA/ResourceMan/ResourceKey.h"

#include <sigmatch/sigmatch.hpp>

#include "../../include/lua.h"
#include "signatures_macros.h"
#include "EASTL/hash_map.h"

#ifdef VERSION_MYSIMS_COZYBUNDLE
#include "version/signatures_mysims64.h"
#endif

#ifdef VERSION_MYSIMS_ORIGINAL
#include "version/signatures_mysims32.h"
#endif

#ifdef VERSION_MYSIMSKINGDOM_COZYBUNDLE
#include "version/signatures_kingdom64.h"
#endif

namespace Revo {
    namespace ResourceSystem {
        struct ResourceSystem {
        };

        VIRTUAL(void, ResourceSystem, Init);
    }

    namespace StateMachine {
        struct StateMachine {};

        // VIRTUAL(void, StateMachine, RequestState, int, bool);
    }

#ifdef VERSION_MYSIMS_COZYBUNDLE
    GLOBAL(void*, load_body,
           void *_a, char *pDynamicSkinName, void *_c,
           EA::ResourceMan::IResource *pMaterialResource,
           EA::ResourceMan::IResource *pTextureResource,
           EA::ResourceMan::IResource *pMaskResource);
#endif
    namespace App {
        struct TinyXmlInstance {
            void *mTinyXmlImplementation;
            void *mTiXmlElement;
        };

#if defined(VERSION_MYSIMS_COZYBUNDLE) || defined(VERSION_MYSIMS_ORIGINAL)

        STATIC(size_t, App, GetCorrectLoadFolder, wchar_t *, bool, const wchar_t *, const wchar_t *);

        STATIC(TinyXmlInstance*, App, ReadXMLFromPath, TinyXmlInstance*, const char*, void*, const char*, bool, double*,
               const char*);

        STATIC(TinyXmlInstance*, App, ReadXMLFromStream, const char*&, EA::IO::IStream*, void*, const char*, double*);
#endif
    }
}


namespace EA {
    namespace ArgScript {
        struct ArgScript {
        };

#if defined(VERSION_MYSIMS_COZYBUNDLE) || defined(VERSION_MYSIMS_ORIGINAL)
        VIRTUAL(void, ArgScript, Output, void*, const char* pFormat, ...)

        VIRTUAL(void, ArgScript, ErrorOutput, const char* pFormat, ...)
#endif
    }

    namespace ResourceMan {
        class IRecord;
        class IDatabase;
        class IResource;

        namespace PFIndexModifiable {
            struct PFIndexModifiable {
                void **mVTable;
                eastl::hash_map<Key, RecordInfo> mItemMap;
            };
        }

        namespace DatabasePackedFile {
            struct DatabasePackedFile {
                void **mVTable;
            };

            VIRTUAL(bool, DatabasePackedFile, OpenRecord, const Key& key, IRecord** pDstRecord,
                    IO::AccessFlags accessFlags, IO::CD creationDisposition, int,
                    EA::ResourceMan::RecordInfo* pRecordInfo);

            VIRTUAL(bool, DatabasePackedFile, Open, EA::IO::AccessFlags accessFlags, EA::IO::CD creationDisposition,
                    bool,
                    bool);

            VIRTUAL(PFIndexModifiable::PFIndexModifiable*, DatabasePackedFile, GetIndex);

            VIRTUAL(wchar_t*, DatabasePackedFile, GetLocation);
        }

        namespace Manager {
            struct Manager {
            };

            STATIC(Manager*, Manager, GetManager);
            VIRTUAL(void*, Manager, GetResource, const Key& key, IResource** ppResource, void*, IDatabase* pDatabase,
                    void* pFactory,
                    const Key* kpKey, uint32_t, uint32_t, uint32_t, uint32_t);

            VIRTUAL(bool, Manager, SetTypename, uint32_t nTypeID, const wchar_t* pExtension);

            VIRTUAL(void, Manager, RegisterDatabase, bool bAdd, IDatabase* pDatabase, uint32_t priority);
        }

        namespace DatabaseDirectoryFiles {
            struct DatabaseDirectoryFiles {
                void **mVtable;
                size_t _padding[2];
                wchar_t *mLocation;
            };

            VIRTUAL(bool, DatabaseDirectoryFiles, OpenRecord, const Key& key, IRecord** ppDstRecord,
                    IO::AccessFlags accessFlags, IO::CD creationDisposition, int,
                    EA::ResourceMan::RecordInfo* pRecordInfo);

            VIRTUAL(void*, DatabaseDirectoryFiles, AddFile, const Key&, const wchar_t* pPath, const wchar_t* pName);
        }
    }

    namespace ScriptOs::LuaScriptingSystem {
        struct LuaScriptingSystem {
            void **mVtable;
            size_t _padding[3];
            lua_State *mState;
        };

        VIRTUAL(void, LuaScriptingSystem, Startup);
        // VIRTUAL(bool, LuaScriptingSystem, LoadAllLuaScripts);
    }
}

namespace UI {
#ifdef VERSION_MYSIMS_COZYBUNDLE
    STATIC(void, UI, SetCursorLock, bool state);
#else
    inline void SetCursorLock(bool state){};
#endif
}

#if defined(VERSION_MYSIMS_COZYBUNDLE) || defined(VERSION_MYSIMSKINGDOM_COZYBUNDLE)

struct GraphicDevice;

namespace TSS::Graphics {
    STATIC(GraphicDevice*, Graphics, GetDevice);
}

#pragma region tf

struct Queue;
struct QueuePresentDesc;
struct Renderer;
struct QueueDesc;
struct CmdDesc;
struct Cmd;
struct CmdPoolDesc;
struct CmdPool;
struct BindRenderTargetsDesc;
struct QueueSubmitDesc;
struct Fence;

GLOBAL(void, addCmd, Renderer* pRenderer, const CmdDesc* pDesc, Cmd** ppCmd);
GLOBAL(void, addCmdPool, Renderer* pRenderer, const CmdPoolDesc* pDesc, CmdPool** ppCmdPool);
GLOBAL(void, resetCmdPool, Renderer* pRenderer, CmdPool* pCmdPool);
GLOBAL(void, beginCmd, Cmd* pCmd);
GLOBAL(void, cmdBindRenderTargets, Cmd* pCmd, const BindRenderTargetsDesc* pDesc);
GLOBAL(void, cmdSetViewport, Cmd* pCmd, float x, float y, float width, float height, float minDepth, float maxDepth);
GLOBAL(void, cmdSetScissor, Cmd* pCmd, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
GLOBAL(void, endCmd, Cmd* pCmd);
GLOBAL(void, queuePresent, Queue* pQueue, const QueuePresentDesc* pDesc);
GLOBAL(void, addQueue, Renderer* pRenderer, QueueDesc* pDesc, Queue** ppQueue);
GLOBAL(void, addFence, Renderer* pRenderer, Fence** ppFence);
GLOBAL(void, queueSubmit, Queue* pQueue, const QueueSubmitDesc* pDesc)
// GLOBAL(void, waitForFences, Renderer* pRenderer, uint32_t fenceCount, Fence** ppFences); // tiny function, hard to find, use the one below
GLOBAL(void, internal_waitForFences, uint32_t fenceCount, Fence** ppFences);
GLOBAL(void, tfWriteLog, uint32_t level, const char* filename, int line_number, const char* message, ...);

#pragma endregion tf
#endif

#pragma region lua

GLOBAL(void, lua_pushvalue, lua_State *L, int index);
GLOBAL(int, lua_getfield, lua_State *L, int index, const char *k);
GLOBAL(void, lua_pushcclosure, lua_State *L, lua_CFunction fn, int n);
GLOBAL(void, lua_setfield, lua_State *L, int index, const char *k);
GLOBAL(void, lua_settop, lua_State *L, int index);
GLOBAL(int, lua_gettop, lua_State *L);
GLOBAL(const char*, lua_tolstring, lua_State *L, int index, size_t *len);
GLOBAL(lua_State*, lua_newstate, lua_Alloc f, void* ud);
GLOBAL(int, luaL_ref, lua_State *L, int t);
GLOBAL(int, luaL_loadfile, lua_State *L, const char* filename);
GLOBAL(void, lua_remove, lua_State *L, int index);
GLOBAL(void, lua_insert, lua_State *L, int index);
GLOBAL(int, lua_pcall, lua_State *L, int nargs, int nresults, int errfunc);
GLOBAL(void, lua_rawgeti, lua_State *L, int index, int n);
GLOBAL(const char*, luaL_checklstring, lua_State *L, int narg, size_t *l);
GLOBAL(void, lua_pushstring, lua_State *L, const char *s);
GLOBAL(int, luaB_loadstring, lua_State *L);
GLOBAL(int, luaL_loadbuffer, lua_State * L, const char * buff, size_t sz, const char * name);
GLOBAL(int, lua_type, lua_State * L, int index);
GLOBAL(bool, lua_toboolean, lua_State *L, int index);
GLOBAL(double, lua_tonumber, lua_State *L, int index);
GLOBAL(void, lua_pushnil, lua_State *L);
GLOBAL(int, lua_next, lua_State * L, int index);
GLOBAL(void, lua_createtable, lua_State *L, int narray, int nrec);
GLOBAL(int, luaL_error, lua_State *L, const char *fmt, ...);
GLOBAL(void, lua_pushboolean, lua_State *L, int b);
GLOBAL(void, lua_pushinteger, lua_State *L, int i);
GLOBAL(int, lua_tointeger, lua_State *L, int idx);

#pragma endregion lua

#endif //SIGDEF_H
