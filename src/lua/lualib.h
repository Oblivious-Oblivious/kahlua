/*
** $Id: lualib.h $
** Lua standard libraries
** See Copyright Notice in lua.h
*/


#ifndef lualib_h
#define lualib_h

#include "lua.h"


/* version suffix for environment variable names */
#define LUA_VERSUFFIX "_" LUA_VERSION_MAJOR "_" LUA_VERSION_MINOR


extern int(luaopen_base)(lua_State *L);

#define LUA_COLIBNAME "coroutine"
extern int(luaopen_coroutine)(lua_State *L);

#define LUA_TABLIBNAME "table"
extern int(luaopen_table)(lua_State *L);

#define LUA_IOLIBNAME "io"
extern int(luaopen_io)(lua_State *L);

#define LUA_OSLIBNAME "os"
extern int(luaopen_os)(lua_State *L);

#define LUA_STRLIBNAME "string"
extern int(luaopen_string)(lua_State *L);

#define LUA_UTF8LIBNAME "utf8"
extern int(luaopen_utf8)(lua_State *L);

#define LUA_MATHLIBNAME "math"
extern int(luaopen_math)(lua_State *L);

#define LUA_DBLIBNAME "debug"
extern int(luaopen_debug)(lua_State *L);

#define LUA_LOADLIBNAME "package"
extern int(luaopen_package)(lua_State *L);


/* open all previous libraries */
extern void(luaL_openlibs)(lua_State *L);


#endif
