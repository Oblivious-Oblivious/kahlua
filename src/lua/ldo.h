/*
** $Id: ldo.h $
** Stack and Call structure of Lua
** See Copyright Notice in lua.h
*/

#ifndef ldo_h
#define ldo_h


#include "llimits.h"
#include "lobject.h"
#include "lstate.h"
#include "lzio.h"


/*
** Macro to check stack size and grow stack if needed.  Parameters
** 'pre'/'pos' allow the macro to preserve a pointer into the
** stack across reallocations, doing the work only when needed.
** It also allows the running of one GC step when the stack is
** reallocated.
** 'condmovestack' is used in heavy tests to force a stack reallocation
** at every check.
*/
#define luaD_checkstackaux(L, n, pre, pos)            \
  if(luai_unlikely(L->stack_last.p - L->top.p <= (n))) { \
    pre;                                              \
    luaD_growstack(L, n, 1);                          \
    pos;                                              \
  } else {                                            \
    condmovestack(L, pre, pos);                       \
  }

/* In general, 'pre'/'pos' are empty (nothing to save) */
#define luaD_checkstack(L, n) luaD_checkstackaux(L, n, (void)0, (void)0)



#define savestack(L, pt)   (cast_charp(pt) - cast_charp(L->stack.p))
#define restorestack(L, n) cast(StkId, cast_charp(L->stack.p) + (n))


/* macro to check stack size, preserving 'p' */
#define checkstackp(L, n, p)                        \
  luaD_checkstackaux(                               \
    L,                                              \
    n,                                              \
    ptrdiff_t t__ = savestack(L, p), /* save 'p' */ \
    p             = restorestack(L, t__)            \
  ) /* 'pos' part: restore 'p' */


/* macro to check stack size and GC, preserving 'p' */
#define checkstackGCp(L, n, p)                                              \
  luaD_checkstackaux(L, n, ptrdiff_t t__ = savestack(L, p); /* save 'p' */  \
                     luaC_checkGC(L),          /* stack grow uses memory */ \
                     p = restorestack(L, t__)) /* 'pos' part: restore 'p' */


/* macro to check stack size and GC */
#define checkstackGC(L, fsize) \
  luaD_checkstackaux(L, (fsize), luaC_checkGC(L), (void)0)


/* type of protected functions, to be ran by 'runprotected' */
typedef void (*Pfunc)(lua_State *L, void *ud);

extern void luaD_seterrorobj(lua_State *L, int errcode, StkId oldtop);
extern int
luaD_protectedparser(lua_State *L, ZIO *z, const char *name, const char *mode);
extern void
luaD_hook(lua_State *L, int event, int line, int fTransfer, int nTransfer);
extern void luaD_hookcall(lua_State *L, CallInfo *ci);
extern int
luaD_pretailcall(lua_State *L, CallInfo *ci, StkId func, int narg1, int delta);
extern CallInfo *luaD_precall(lua_State *L, StkId func, int nResults);
extern void luaD_call(lua_State *L, StkId func, int nResults);
extern void luaD_callnoyield(lua_State *L, StkId func, int nResults);
extern int luaD_closeprotected(lua_State *L, ptrdiff_t level, int status);
extern int
luaD_pcall(lua_State *L, Pfunc func, void *u, ptrdiff_t oldtop, ptrdiff_t ef);
extern void luaD_poscall(lua_State *L, CallInfo *ci, int nres);
extern int luaD_reallocstack(lua_State *L, int newsize, int raiseerror);
extern int luaD_growstack(lua_State *L, int n, int raiseerror);
extern void luaD_shrinkstack(lua_State *L);
extern void luaD_inctop(lua_State *L);

extern l_noret luaD_throw(lua_State *L, int errcode);
extern int luaD_rawrunprotected(lua_State *L, Pfunc f, void *ud);

#endif
