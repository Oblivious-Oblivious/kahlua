#ifndef __LUAXLIB_H_
#define __LUAXLIB_H_

#include "lua.h"
#include "luaconf.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define lua_assert(c) assert(c)

/** @brief global table */
#define LUA_GNAME "_G"

typedef struct luaL_Buffer luaL_Buffer;

/** @brief extra error code for 'luaL_loadfilex' */
#define LUA_ERRFILE (LUA_ERRERR + 1)

/** @brief key, in the registry, for table of loaded modules */
#define LUA_LOADED_TABLE "_LOADED"

/** @brief key, in the registry, for table of preloaded loaders */
#define LUA_PRELOAD_TABLE "_PRELOAD"

typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;

#define LUAL_NUMSIZES (sizeof(lua_Integer) * 16 + sizeof(lua_Number))

extern void(luaL_checkversion_)(lua_State *L, lua_Number ver, size_t sz);
#define luaL_checkversion(L) \
  luaL_checkversion_(L, LUA_VERSION_NUM, LUAL_NUMSIZES)

extern int(luaL_getmetafield)(lua_State *L, int obj, const char *e);
extern int(luaL_callmeta)(lua_State *L, int obj, const char *e);
extern const char *(luaL_tolstring)(lua_State *L, int idx, size_t *len);
extern int(luaL_argerror)(lua_State *L, int arg, const char *extramsg);
extern int(luaL_typeerror)(lua_State *L, int arg, const char *tname);
extern const char *(luaL_checklstring)(lua_State *L, int arg, size_t *l);
extern const char *(luaL_optlstring)(lua_State *L,
                                     int arg,
                                     const char *def,
                                     size_t *l);
extern lua_Number(luaL_checknumber)(lua_State *L, int arg);
extern lua_Number(luaL_optnumber)(lua_State *L, int arg, lua_Number def);
extern lua_Integer(luaL_checkinteger)(lua_State *L, int arg);
extern lua_Integer(luaL_optinteger)(lua_State *L, int arg, lua_Integer def);
extern void(luaL_checkstack)(lua_State *L, int sz, const char *msg);
extern void(luaL_checktype)(lua_State *L, int arg, int t);
extern void(luaL_checkany)(lua_State *L, int arg);
extern int(luaL_newmetatable)(lua_State *L, const char *tname);
extern void(luaL_setmetatable)(lua_State *L, const char *tname);
extern void *(luaL_testudata)(lua_State *L, int ud, const char *tname);
extern void *(luaL_checkudata)(lua_State *L, int ud, const char *tname);
extern void(luaL_where)(lua_State *L, int lvl);
extern int(luaL_error)(lua_State *L, const char *fmt, ...);
extern int(luaL_checkoption)(
  lua_State *L, int arg, const char *def, const char *const lst[]
);
extern int(luaL_fileresult)(lua_State *L, int stat, const char *fname);
extern int(luaL_execresult)(lua_State *L, int stat);

/* predefined references */
#define LUA_NOREF  (-2)
#define LUA_REFNIL (-1)

extern int(luaL_ref)(lua_State *L, int t);
extern void(luaL_unref)(lua_State *L, int t, int ref);
extern int(luaL_loadfilex)(
  lua_State *L, const char *filename, const char *mode
);
#define luaL_loadfile(L, f) luaL_loadfilex(L, f, NULL)
extern int(luaL_loadbufferx)(
  lua_State *L, const char *buff, size_t sz, const char *name, const char *mode
);
extern int(luaL_loadstring)(lua_State *L, const char *s);
extern lua_State *(luaL_newstate)(void);
extern lua_Integer(luaL_len)(lua_State *L, int idx);
extern void(luaL_addgsub)(
  luaL_Buffer *b, const char *s, const char *p, const char *r
);
extern const char *(luaL_gsub)(lua_State *L,
                               const char *s,
                               const char *p,
                               const char *r);
extern void(luaL_setfuncs)(lua_State *L, const luaL_Reg *l, int nup);
extern int(luaL_getsubtable)(lua_State *L, int idx, const char *fname);
extern void(luaL_traceback)(
  lua_State *L, lua_State *L1, const char *msg, int level
);
extern void(luaL_requiref)(
  lua_State *L, const char *modname, lua_CFunction openf, int glb
);

#define luaL_newlibtable(L, l) \
  lua_createtable(L, 0, sizeof(l) / sizeof((l)[0]) - 1)
#define luaL_newlib(L, l) \
  (luaL_checkversion(L), luaL_newlibtable(L, l), luaL_setfuncs(L, l, 0))
#define luaL_argcheck(L, cond, arg, extramsg) \
  ((void)(luai_likely(cond) || luaL_argerror(L, (arg), (extramsg))))
#define luaL_argexpected(L, cond, arg, tname) \
  ((void)(luai_likely(cond) || luaL_typeerror(L, (arg), (tname))))
#define luaL_checkstring(L, n)  (luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L, n, d) (luaL_optlstring(L, (n), (d), NULL))
#define luaL_typename(L, i)     lua_typename(L, lua_type(L, (i)))
#define luaL_dofile(L, fn) \
  (luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define luaL_dostring(L, s) \
  (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define luaL_getmetatable(L, n)      (lua_getfield(L, LUA_REGISTRYINDEX, (n)))
#define luaL_opt(L, f, n, d)         (lua_isnoneornil(L, (n)) ? (d) : f(L, (n)))
#define luaL_loadbuffer(L, s, sz, n) luaL_loadbufferx(L, s, sz, n, NULL)

/**
 * @brief Perform arithmetic operations on lua_Integer values with wrap-around
 * semantics, as the Lua core does.
 */
#define luaL_intop(op, v1, v2) \
  ((lua_Integer)((lua_Unsigned)(v1)op(lua_Unsigned)(v2)))

/** @brief push the value used to represent failure/error */
#define luaL_pushfail(L) lua_pushnil(L)

/**
 * @brief A buffer is a userdata with metatable 'LUA_BUFFER' and initial
 * @param b -> buffer address
 * @param size -> buffer size
 * @param n -> number of characters in buffer
 * @param init -> ensures maximum alignment for buffer
 */
struct luaL_Buffer {
  char *b;
  size_t size;
  size_t n;
  lua_State *L;
  union {
    LUAI_MAXALIGN;
    char b[LUAL_BUFFERSIZE];
  } init;
};

#define luaL_bufflen(bf)  ((bf)->n)
#define luaL_buffaddr(bf) ((bf)->b)
#define luaL_addchar(B, c)                                  \
  ((void)((B)->n < (B)->size || luaL_prepbuffsize((B), 1)), \
   ((B)->b[(B)->n++] = (c)))
#define luaL_addsize(B, s) ((B)->n += (s))
#define luaL_buffsub(B, s) ((B)->n -= (s))

extern void(luaL_buffinit)(lua_State *L, luaL_Buffer *B);
extern char *(luaL_prepbuffsize)(luaL_Buffer *B, size_t sz);
extern void(luaL_addlstring)(luaL_Buffer *B, const char *s, size_t l);
extern void(luaL_addstring)(luaL_Buffer *B, const char *s);
extern void(luaL_addvalue)(luaL_Buffer *B);
extern void(luaL_pushresult)(luaL_Buffer *B);
extern void(luaL_pushresultsize)(luaL_Buffer *B, size_t sz);
extern char *(luaL_buffinitsize)(lua_State *L, luaL_Buffer *B, size_t sz);

#define luaL_prepbuffer(B) luaL_prepbuffsize(B, LUAL_BUFFERSIZE)

/** File handles for IO library */

/**
 * @brief A file handle is a userdata with metatable 'LUA_FILEHANDLE' and
 * initial structure 'luaL_Stream' (it may contain other fields after that
 * initial structure).
 */
#define LUA_FILEHANDLE "FILE*"

/**
 * @brief Helper for file handling
 * @param f -> stream (NULL for incompletely created streams)
 * @param closef -> to close stream (NULL for closed streams)
 */
typedef struct luaL_Stream {
  FILE *f;
  lua_CFunction closef;
} luaL_Stream;

/** @brief Abstraction layer for basic report of messages and errors */
#define lua_writestring(s, l)      fwrite((s), sizeof(char), (l), stdout)
#define lua_writeline()            (lua_writestring("\n", 1), fflush(stdout))
#define lua_writestringerror(s, p) (fprintf(stderr, (s), (p)), fflush(stderr))

#endif
