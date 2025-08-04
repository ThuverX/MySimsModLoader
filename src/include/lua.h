//
// Created by exozg on 25/01/2025.
//

#ifndef LUA_H
#define LUA_H


struct lua_State;

typedef const char * (*lua_Reader)(lua_State *L, void *ud, size_t *sz);

typedef int (*lua_CFunction)(lua_State *L);

typedef void * (*lua_Alloc)(void *ud, void *ptr, size_t osize, size_t nsize);

typedef struct luaL_Reg {
    const char *name;
    lua_CFunction func;
} luaL_Reg;

// From src/lua.h
// Pseudo-indices
#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))

// From src/lauxlib.h
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

// Thread status; 0 is OK

#define LUA_OK          0
#define LUA_YIELD		1
#define LUA_ERRRUN		2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM		4
#define LUA_ERRERR		5
// From src/lauxlib.h
// Extra error code for 'luaL_load'
#define LUA_ERRFILE     (LUA_ERRERR+1)

// From src/lua.h
// Option for multiple returns in 'lua_pcall' and 'lua_call'
#define LUA_MULTRET	(-1)
#define LUA_TNONE		(-1)
#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define lua_pop(L,n)		lua_settop(L, -(n)-1)
#define lua_newtable(L)		lua_createtable(L, 0, 0)
#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)
#define lua_getglobal(L,s)	lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define lua_setglobal(L,s)	lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)

#define luaL_getmetatable(L,n)		(lua_getfield(L, LUA_REGISTRYINDEX, (n)))

#define lua_isnumber(L,n)		(lua_type(L, (n)) == LUA_TNUMBER)

#define luaL_argcheck(L, cond,numarg,extramsg) ((void)((cond) || luaL_argerror(L, (numarg), (extramsg))))
#define luaL_checkstring(L,n)   (luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)   (luaL_optlstring(L, (n), (d), NULL))
#define luaL_checkint(L,n)      ((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)      ((int)luaL_optinteger(L, (n), (d)))
#define luaL_checklong(L,n)     ((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)     ((long)luaL_optinteger(L, (n), (d)))

#define luaL_opt(L,f,n,d)       (lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))
#define luaL_typename(L,i)      lua_typename(L, lua_type(L,(i)))
#define lua_absindex(L, i)         ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : \
    lua_gettop(L) + (i) + 1)

#endif //LUA_H
