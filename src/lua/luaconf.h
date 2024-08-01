#ifndef __LUACONF_H_
#define __LUACONF_H_

#include <limits.h>
#include <stddef.h>

/**
 * General Configuration File for Lua
 *
 * Some definitions here can be changed externally, through the compiler
 * (e.g., with '-D' options): They are commented out or protected
 * by '#if !defined' guards. However, several other definitions
 * should be changed directly here, either because they affect the
 * Lua ABI (by making the changes here, you ensure that all software
 * connected to Lua, such as C libraries, will be compiled with the same
 * configuration); or because they are seldom changed.
 */

#define LUA_USE_POSIX
#define LUA_USE_DLOPEN /* MacOS does not need -ldl */

/**
 * @param LUA_PATH_SEP -> the character that separates templates in a path.
 * @param LUA_PATH_MARK -> the string that marks the substitution points in a
 * template.
 * @param LUA_EXEC_DIR in a Windows path is replaced by the executable's
 * directory.
 */
#define LUA_PATH_SEP  ";"
#define LUA_PATH_MARK "?"
#define LUA_EXEC_DIR  "!"

/**
 * @param LUA_PATH_DEFAULT -> default path to look for Lua libraries.
 * @param LUA_CPATH_DEFAULT -> default path to look for C libraries.
 */
#define LUA_VDIR LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#define LUA_ROOT "/usr/local/"
#define LUA_LDIR LUA_ROOT "share/lua/" LUA_VDIR "/"
#define LUA_CDIR LUA_ROOT "lib/lua/" LUA_VDIR "/"

#define LUA_PATH_DEFAULT                                              \
  LUA_LDIR "?.lua;" LUA_LDIR "?/init.lua;" LUA_CDIR "?.lua;" LUA_CDIR \
           "?/init.lua;"                                              \
           "./?.lua;"                                                 \
           "./?/init.lua"

#define LUA_CPATH_DEFAULT                 \
  LUA_CDIR "?.so;" LUA_CDIR "loadall.so;" \
           "./?.so"

/** LUA_DIRSEP is the directory separator (for submodules). */
#define LUA_DIRSEP "/"

/** LUA_IGMARK is a mark to ignore all after it when building the module name
 * (e.g., used to build the luaopen_ function name). Typically, the suffix after
 * the mark is the module version, as in "mod-v1.2.so". */
#define LUA_IGMARK "-"

#define LUA_API    extern
#define LUALIB_API LUA_API
#define LUAMOD_API LUA_API
#define LUAI_FUNC  extern

/**
 * @param LUA_INTEGER_FRMLEN -> the length modifier for reading/writing
 * integers.
 * @param LUA_INTEGER_FMT -> the format for writing integers.
 * @param LUAI_UACINT -> the result of a 'default argument promotion' over a
 * LUA_INTEGER. use it to avoid problems with promotions (which can turn a
 * comparison between unsigneds into a signed comparison)
 * @param lua_integer2str -> converts an integer to a string.
 * @param LUA_MAXINTEGER -> the maximum value for a LUA_INTEGER.
 * @param LUA_MININTEGER -> the minimum value for a LUA_INTEGER.
 * @param LUA_MAXUNSIGNED -> the maximum value for a LUA_UNSIGNED.
 */
#define LUA_INTEGER        long
#define LUA_INTEGER_FRMLEN "l"
#define LUA_INTEGER_FMT    "%" LUA_INTEGER_FRMLEN "d"

#define LUAI_UACINT  LUA_INTEGER
#define LUA_UNSIGNED unsigned LUAI_UACINT

#define lua_integer2str(s, sz, n) \
  snprintf((s), sz, LUA_INTEGER_FMT, (LUAI_UACINT)(n))

#define LUA_MAXINTEGER  LONG_MAX
#define LUA_MININTEGER  LONG_MIN
#define LUA_MAXUNSIGNED ULONG_MAX

/** lua_pointer2str converts a pointer to a readable string in a non-specified
 * way. */
#define lua_pointer2str(buff, sz, p) snprintf(buff, sz, "%p", p)

/** lua_number2strx converts a float to a hexadecimal numeral. In C99, 'sprintf'
 * (with format specifiers '%a'/'%A') does that. Otherwise, you can leave
 * 'lua_number2strx' undefined and Lua will provide its own implementation. */
#define lua_number2strx(L, b, sz, f, n) \
  ((void)L, snprintf(b, sz, f, (LUAI_UACNUMBER)(n)))

/**
 * LUAI_UACNUMBER is the result of a 'default argument promotion'
 * over a floating number.
 * l_floatatt(x) corrects float attribute 'x' to the proper float type
 * by prefixing it with one of FLT/DBL/LDBL.
 * LUA_NUMBER_FRMLEN is the length modifier for writing floats.
 * LUA_NUMBER_FMT is the format for writing floats.
 * lua_number2str converts a float to a string.
 * l_mathop allows the addition of an 'l' or 'f' to all math operations.
 * l_floor takes the floor of a float.
 * strtod converts a decimal numeral to a number.
 */

/** lua_numbertointeger converts a float number with an integral value to an
 * integer, or returns 0 if float is not within the range of a lua_Integer. (The
 * range comparisons are tricky because of rounding. The tests here assume a
 * two-complement representation, where MININTEGER always has an exact
 * representation as a float; MAXINTEGER may not have one, and therefore its
 * conversion to float may have an ill-defined value.) */
#define LUA_NUMBER        double
#define LUA_NUMBER_FRMLEN ""
#define LUA_NUMBER_FMT    "%.14g"

#define l_floatatt(n) (DBL_##n)

#define LUAI_UACNUMBER double

#define lua_numbertointeger(n, p)         \
  ((n) >= (LUA_NUMBER)(LUA_MININTEGER) && \
   (n) < -(LUA_NUMBER)(LUA_MININTEGER) && (*(p) = (LUA_INTEGER)(n), 1))

#define l_mathop(op) op
#define l_floor(x)   (l_mathop(floor)(x))
#define lua_number2str(s, sz, n) \
  snprintf((s), sz, LUA_NUMBER_FMT, (LUAI_UACNUMBER)(n))

/** LUA_KCONTEXT is the type of the context ('ctx') for continuation functions.
 * It must be a numerical type; Lua will use 'intptr_t' if available, otherwise
 * it will use 'ptrdiff_t' (the nearest thing to 'intptr_t' in C89) */
#define LUA_KCONTEXT ptrdiff_t

/** lua_getlocaledecpoint gets the locale "radix character" (decimal point).
 * Change that if you do not want to use C locales. (Code using this macro must
 * include the header 'locale.h'.) */
#if !defined(lua_getlocaledecpoint)
  #define lua_getlocaledecpoint() (localeconv()->decimal_point[0])
#endif

/** macros to improve jump prediction, used mostly for error handling and debug
 * facilities. (Some macros in the Lua API use these macros. Define
 * LUA_NOBUILTIN if you do not want '__builtin_expect' in your code.) */
#if !defined(luai_likely)
  #if defined(__GNUC__) && !defined(LUA_NOBUILTIN)
    #define luai_likely(x)   (__builtin_expect(((x) != 0), 1))
    #define luai_unlikely(x) (__builtin_expect(((x) != 0), 0))
  #else
    #define luai_likely(x)   (x)
    #define luai_unlikely(x) (x)
  #endif
#endif

/** Macros that affect the API and must be stable (that is, must be the same
 * when you compile Lua and when you compile code that links to Lua). */

/** Limits the size of the Lua stack.  CHANGE it if you need a different limit.
 * This limit is arbitrary; its only purpose is to stop Lua from consuming
 * unlimited stack space (and to reserve some numbers for pseudo-indices). (It
 * must fit into max(size_t)/32 and max(int)/2.) */
#define LUAI_MAXSTACK 1000000

/** Defines the size of a raw memory area associated with a Lua state with very
 * fast access.  CHANGE it if you need a different size. */
#define LUA_EXTRASPACE (sizeof(void *))

/** The maximum size for the description of the source of a function in debug
 * information.  CHANGE it if you want a different size. */
#define LUA_IDSIZE 60

/** The initial buffer size used by the lauxlib buffer system. */
#define LUAL_BUFFERSIZE ((int)(16 * sizeof(void *) * sizeof(lua_Number)))

/** Defines fields that, when used in a union, ensure maximum alignment for the
 * other items in that union. */
#define LUAI_MAXALIGN \
  lua_Number n;       \
  double u;           \
  void *s;            \
  lua_Integer i;      \
  long l

#endif
