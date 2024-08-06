/*
** $Id: lfunc.h $
** Auxiliary functions to manipulate prototypes and closures
** See Copyright Notice in lua.h
*/

#ifndef lfunc_h
#define lfunc_h


#include "lobject.h"


#define sizeCclosure(n) \
  (cast_int(offsetof(CClosure, upvalue)) + cast_int(sizeof(TValue)) * (n))

#define sizeLclosure(n) \
  (cast_int(offsetof(LClosure, upvals)) + cast_int(sizeof(TValue *)) * (n))


/* test whether thread is in 'twups' list */
#define isintwups(L) (L->twups != L)


/*
** maximum number of upvalues in a closure (both C and Lua). (Value
** must fit in a VM register.)
*/
#define MAXUPVAL 255


#define upisopen(up) ((up)->v.p != &(up)->u.value)


#define uplevel(up) check_exp(upisopen(up), cast(StkId, (up)->v.p))


/*
** maximum number of misses before giving up the cache of closures
** in prototypes
*/
#define MAXMISS 10



/* special status to close upvalues preserving the top of the stack */
#define CLOSEKTOP (-1)


extern Proto *luaF_newproto(lua_State *L);
extern CClosure *luaF_newCclosure(lua_State *L, int nupvals);
extern LClosure *luaF_newLclosure(lua_State *L, int nupvals);
extern void luaF_initupvals(lua_State *L, LClosure *cl);
extern UpVal *luaF_findupval(lua_State *L, StkId level);
extern void luaF_newtbcupval(lua_State *L, StkId level);
extern void luaF_closeupval(lua_State *L, StkId level);
extern StkId luaF_close(lua_State *L, StkId level, int status, int yy);
extern void luaF_unlinkupval(UpVal *uv);
extern void luaF_freeproto(lua_State *L, Proto *f);
extern const char *
luaF_getlocalname(const Proto *func, int local_number, int pc);


#endif
