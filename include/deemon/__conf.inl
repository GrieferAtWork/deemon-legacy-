/* Copyright (c) 2016 - deemon by Griefer@Work                                    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 */
#ifndef GUARD_DEEMON___CONF_INL
#define GUARD_DEEMON___CONF_INL 1

#if 1
//#define DEE_WITHOUT_THREADS
#endif

#ifndef DEE_DEBUG
#if defined(DEBUG) || defined(_DEBUG) || defined(__DEEMON_DEBUG__)
# define DEE_DEBUG 1
#endif
#endif /* !DEE_DEBUG */

#include "__features.inl"

#ifdef DEE_WITHOUT_IO
#define DEE_PLATFORM_HAVE_IO 0
#endif

//////////////////////////////////////////////////////////////////////////
// Enable the c++ APIs for deemon
#ifndef DEE_HAVE_CXX_API
#if /*defined(DEE_LIMITED_API) || */!defined(__cplusplus)
#define DEE_HAVE_CXX_API 0
#else
#define DEE_HAVE_CXX_API 1
#endif
#endif

#ifndef DEE_LIMITED_DEX
#if defined(DEE_LIMITED_API) || defined(DEE_EXTENSION)
#define DEE_LIMITED_DEX 1
#endif
#endif

#if defined(DEE_NO_DEPRECATED_API) || \
    defined(DEE_LIMITED_API)
// Disable deprecated APIs in limited/internal headers
#undef DEE_DEPRECATED_API
#endif

//////////////////////////////////////////////////////////////////////////
// Define 'DEE_DEPRECATED_API' as the API version to enable backwards compatibility for
// Defaults to be last API version (e.g.: v101 defaults to '100')
// >> define 'DEE_NO_DEPRECATED_API' to disable all deprecated APIs
// NOTE: Even with this defined, some old parts of the API might still be gone
// 
// - 'support_begin' describes the API version in which the component was originally added
// - 'depr_begin' describes the first API version since when component is deprecated
// - 'support_end' describes the first API version that will
//   definitely no longer include support for this function.
//   Note though, that support may end before that time in
//   cases where name collisions could occur.
#ifdef DEE_DEPRECATED_API
// Special Version dependent deprecated API support
// Enable if it's currently supported and either not deprecated or
// if the deprecated API level lies within the frame of support.
#define DEE_DEPRECATED_API_VERSION(support_begin,deprecation_begin,support_end) \
((((DEE_DEPRECATED_API) >= (support_begin) && \
   (DEE_DEPRECATED_API) < (support_end)) || \
  (deprecation_begin) < DEE_VERSION_API) && \
 (support_begin) <= DEE_VERSION_API && \
 (support_end) > DEE_VERSION_API)
#else
// Enable if it's currently supported and not deprecated
#define DEE_DEPRECATED_API_VERSION(support_begin,deprecation_begin,support_end) \
 ((deprecation_begin) > DEE_VERSION_API &&\
  (support_begin) <= DEE_VERSION_API &&\
  (support_end) > DEE_VERSION_API)
#endif

#ifndef DEE_COMPILER_STRUCTURE_PACKED_BEGIN
#if DEE_COMPILER_HAVE_PRAGMA_PACK
#define DEE_COMPILER_STRUCTURE_PACKED_BEGIN DEE_COMPILER_PRAGMA(pack(push,1))
#define DEE_COMPILER_STRUCTURE_PACKED_END   DEE_COMPILER_PRAGMA(pack(pop))
#else
#define DEE_COMPILER_STRUCTURE_PACKED_BEGIN /* nothing */
#define DEE_COMPILER_STRUCTURE_PACKED_END   /* nothing */
#endif
#endif /* !DEE_COMPILER_STRUCTURE_PACKED_BEGIN */

#ifndef DEE_COMPILER_MSVC_WARNING_PUSH
#ifdef _MSC_VER
#define DEE_COMPILER_MSVC_WARNING_PUSH(w) __pragma(warning(push)) __pragma(warning(disable: w))
#define DEE_COMPILER_MSVC_WARNING_POP     __pragma(warning(pop))
#else
#define DEE_COMPILER_MSVC_WARNING_PUSH(w) /* nothing */
#define DEE_COMPILER_MSVC_WARNING_POP     /* nothing */
#endif
#endif /* !DEE_COMPILER_MSVC_WARNING_PUSH */

#ifndef DEE_COMPILER_PREFAST_WARNING_PUSH
#ifdef _PREFAST_
#define DEE_COMPILER_PREFAST_WARNING_PUSH    DEE_COMPILER_MSVC_WARNING_PUSH
#define DEE_COMPILER_PREFAST_WARNING_POP     DEE_COMPILER_MSVC_WARNING_POP
#else
#define DEE_COMPILER_PREFAST_WARNING_PUSH(w) /* nothing */
#define DEE_COMPILER_PREFAST_WARNING_POP     /* nothing */
#endif
#endif /* !DEE_COMPILER_PREFAST_WARNING_PUSH */

#if defined(__cplusplus)
#define DEE_PRIVATE_DECL_CXX_BEGIN extern "C" {
#define DEE_PRIVATE_DECL_CXX_END   }
#else
#define DEE_PRIVATE_DECL_CXX_BEGIN /* nothing */
#define DEE_PRIVATE_DECL_CXX_END   /* nothing */
#endif

#ifndef DEE_LIMITED_DEX
#define DEE_PRIVATE_DECL_WARN_NONLIMITED\
 __pragma(warning(disable: 4127)) /*< Conditional expression is constant */
#else
#define DEE_PRIVATE_DECL_WARN_NONLIMITED
#endif

#if defined(_MSC_VER)
#define DEE_PRIVATE_DECL_WARN_BEGIN __pragma(warning(push)) DEE_PRIVATE_DECL_WARN_NONLIMITED\
 __pragma(warning(disable: 4820)) /*< Padding added to type. */\
 __pragma(warning(disable: 4191)) /*< Unsafe type cast (the cast is always intended...). */\
 __pragma(warning(error:   4047)) /*< This (normally) warning is raised if we link a type method with invalid params / return type(s). */
#define DEE_PRIVATE_DECL_WARN_END   __pragma(warning(pop))
#elif defined(__DEEMON__)
#define DEE_PRIVATE_DECL_WARN_BEGIN \
 _Pragma("warning(push,disable: 213 455)")
#define DEE_PRIVATE_DECL_WARN_END   _Pragma("warning(pop)")
#else
#define DEE_PRIVATE_DECL_WARN_BEGIN /* nothing */
#define DEE_PRIVATE_DECL_WARN_END   /* nothing */
#endif


//////////////////////////////////////////////////////////////////////////
// Ensure that all types are aligned for pointers (by default)
#if defined(_MSC_VER) && defined(DEE_PLATFORM_64_BIT)
#define DEE_PRIVATE_DECL_PACK_BEGIN __pragma(pack(push,8))
#define DEE_PRIVATE_DECL_PACK_END   __pragma(pack(pop))
#elif defined(_MSC_VER) && !defined(DEE_PLATFORM_64_BIT)
#define DEE_PRIVATE_DECL_PACK_BEGIN __pragma(pack(push,4))
#define DEE_PRIVATE_DECL_PACK_END   __pragma(pack(pop))
#elif 1 && defined(DEE_PLATFORM_64_BIT)
#define DEE_PRIVATE_DECL_PACK_BEGIN _Pragma("pack(push,8)")
#define DEE_PRIVATE_DECL_PACK_END   _Pragma("pack(pop)")
#elif 1 && !defined(DEE_PLATFORM_64_BIT)
#define DEE_PRIVATE_DECL_PACK_BEGIN _Pragma("pack(push,4)")
#define DEE_PRIVATE_DECL_PACK_END   _Pragma("pack(pop)")
#else
#define DEE_PRIVATE_DECL_PACK_BEGIN /* nothing */
#define DEE_PRIVATE_DECL_PACK_END   /* nothing */
#endif

#define DEE_DECL_CXX_BEGIN DEE_PRIVATE_DECL_WARN_BEGIN DEE_PRIVATE_DECL_PACK_BEGIN
#define DEE_DECL_CXX_END   DEE_PRIVATE_DECL_PACK_END DEE_PRIVATE_DECL_WARN_END
#define DEE_DECL_BEGIN     DEE_PRIVATE_DECL_CXX_BEGIN DEE_DECL_CXX_BEGIN
#define DEE_DECL_END       DEE_DECL_CXX_END DEE_PRIVATE_DECL_CXX_END

#ifdef DEE_EXTENSION
#ifndef DEE_LINK_DLL_IMPORT
#define DEE_LINK_DLL_IMPORT 1
#endif
#endif

#if !defined(DEE_NO_AUTO_CONFIG) && \
    !defined(DEE_LINK_DLL_IMPORT) && \
    !defined(DEE_LINK_DLL_EXPORT) && \
    !defined(DEE_LIMITED_API)
// Default to dllimport by assuming that the user
// #included this header as part of an API import
#define DEE_LINK_DLL_IMPORT 1
#endif


#ifndef DEE_ATTRIBUTE_CDECL
#if !defined(DEE_PLATFORM_UNIX) ||\
    !defined(DEE_PLATFORM_64_BIT)
#if defined(_MSC_VER)
# define DEE_ATTRIBUTE_CDECL __cdecl
#elif defined(__GNUC__)
# define DEE_ATTRIBUTE_CDECL __attribute__((__cdecl__))
#elif 0
# define DEE_ATTRIBUTE_CDECL _Pragma("cdecl")
#endif
#endif /* ... */
#endif /* !DEE_ATTRIBUTE_CDECL */

#ifndef DEE_CALL
#ifdef DEE_ATTRIBUTE_CDECL
# define DEE_CALL DEE_ATTRIBUTE_CDECL
#else
# define DEE_CALL /* nothing */
#endif
#endif

#ifndef __DEEMON__
# define DEE_EXPORT_DATA_DECL(T) extern DEE_ATTRIBUTE_DLLEXPORT T
# define DEE_EXPORT_FUNC_DECL(T) extern DEE_ATTRIBUTE_DLLEXPORT T DEE_CALL
#if defined(DEE_EXTENSION) && !defined(_MSC_VER)
# define DEE_IMPORT_DATA_DECL(T) extern T
# define DEE_IMPORT_FUNC_DECL(T) extern DEE_ATTRIBUTE_DLLIMPORT T DEE_CALL
#else
# define DEE_IMPORT_DATA_DECL(T) extern DEE_ATTRIBUTE_DLLIMPORT T
# define DEE_IMPORT_FUNC_DECL(T) extern DEE_ATTRIBUTE_DLLIMPORT T DEE_CALL
#endif
#endif

#ifndef DEE_EXPORT_DATA_DECL
#define DEE_EXPORT_DATA_DECL(T) extern T
#endif
#ifndef DEE_IMPORT_DATA_DECL
#define DEE_IMPORT_DATA_DECL(T) extern T
#endif
#ifndef DEE_EXPORT_FUNC_DECL
#define DEE_EXPORT_FUNC_DECL(T) extern T DEE_CALL
#endif
#ifndef DEE_IMPORT_FUNC_DECL
#define DEE_IMPORT_FUNC_DECL(T) extern T DEE_CALL
#endif

#if defined(__DEEMON__)
#ifndef DEE_LINK_DLL_IMPORT
# error "Can only link in dll-import mode from deemon user code"
#endif
#ifdef DEE_PLATFORM_WINDOWS
# define DEE_LIBFILE_SELF "libdeemon.dll"
#elif 1
# define DEE_LIBFILE_SELF ""
#else
# define DEE_LIBFILE_SELF "libdeemon_" DEE_PP_STR(__TPP_EVAL(DEE_VERSION_FULL)) ".so"
#endif
# define DEE_DATA_DECL(T) extern DEE_LIBFILE_SELF: T
# define DEE_FUNC_DECL(T) extern DEE_LIBFILE_SELF: T DEE_CALL
#elif defined(DEE_LINK_DLL_EXPORT)
# define DEE_DATA_DECL    DEE_EXPORT_DATA_DECL
# define DEE_FUNC_DECL    DEE_EXPORT_FUNC_DECL
#elif defined(DEE_LINK_DLL_IMPORT)
# define DEE_DATA_DECL    DEE_IMPORT_DATA_DECL
# define DEE_FUNC_DECL    DEE_IMPORT_FUNC_DECL
#else
# define DEE_DATA_DECL(T) extern T
# define DEE_FUNC_DECL(T) extern T DEE_CALL
#endif

#if DEE_USE_DEBUG_NEW
# define DEE_INCLUDE_MEMORY_API()         <debug_new.h>
# define DEE_INCLUDE_MEMORY_API_DISABLE() <debug_new_disable.inl>
# define DEE_INCLUDE_MEMORY_API_ENABLE()  <debug_new_enable.inl>
#if defined(__FUNCTION__) && defined(_MSC_VER)
# define DEE_DEBUG_NEW_ACCESS(expr)       __FILE__ "\0" DEE_PP_STR(__LINE__) "\0" expr "\0" __FUNCTION__
#else
# define DEE_DEBUG_NEW_ACCESS(expr)       __FILE__ "\0" DEE_PP_STR(__LINE__) "\0" expr
#endif
#ifndef DEBUG_NEW_CONFIG_LINK_DLLEXPORT
#ifdef GUARD_DEBUG_NEW_CONF_INL
# error "Too late to configure debug_new now..."
#endif
#ifdef DEE_LINK_DLL_IMPORT
# define DEBUG_NEW_CONFIG_LINK_DLLIMPORT 1
#endif
#endif
#else /* DEE_USE_DEBUG_NEW */
# define DEE_INCLUDE_MEMORY_API()         <stdlib.h>
# define DEE_INCLUDE_MEMORY_API_DISABLE() <deemon/__nullfile>
# define DEE_INCLUDE_MEMORY_API_ENABLE()  <deemon/__nullfile>
# define DEE_DEBUG_NEW_ACCESS(expr)       ((char const *)0)
#endif /* !DEE_USE_DEBUG_NEW */

#if __has_builtin(__builtin_breakpoint)
# define DEE_BUILTIN_BREAKPOINT __builtin_breakpoint
#elif defined(DEBUG_NEW_BREAKPOINT)
# define DEE_BUILTIN_BREAKPOINT DEBUG_NEW_BREAKPOINT
#elif defined(_MSC_VER)
DEE_DECL_BEGIN
void __cdecl __debugbreak(void);
DEE_DECL_END
# define DEE_BUILTIN_BREAKPOINT __debugbreak
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H
# include <signal.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H */
#ifdef SIGTRAP
#  define DEE_BUILTIN_BREAKPOINT() raise(SIGTRAP)
#endif
#endif


#ifndef DEE_COMPILER_ASSUME
#if DEE_COMPILER_HAVE_MSVC_ASSUME
#define DEE_COMPILER_ASSUME       __assume
#else
#define DEE_COMPILER_ASSUME(expr) (void)0
#endif
#endif

#ifndef DEE_BUILTIN_UNREACHABLE_
#if DEE_COMPILER_HAVE_BUILTIN_UNREACHABLE
#define DEE_BUILTIN_UNREACHABLE_   __builtin_unreachable
#elif DEE_COMPILER_HAVE_MSVC_ASSUME
#define DEE_BUILTIN_UNREACHABLE_() __assume(0)
#else
#define DEE_BUILTIN_UNREACHABLE_() (void)0
#endif
#endif

#ifndef DEE_LIKELY
#if DEE_COMPILER_HAVE_BUILTIN_EXPECT
#if defined(__cplusplus) || defined(__DEEMON__)
#define DEE_LIKELY(x)   (__builtin_expect(!!(x),true))
#define DEE_UNLIKELY(x) (__builtin_expect(!!(x),false))
#else
#define DEE_LIKELY(x)   (__builtin_expect(!!(x),1))
#define DEE_UNLIKELY(x) (__builtin_expect(!!(x),0))
#endif
#elif defined(__INTELLISENSE__) && defined(__cplusplus)
class __intellisense__BOOL_ONLY { public: operator bool (); };
static __intellisense__BOOL_ONLY __intellisense__DEE_LIKELY(bool x);
static __intellisense__BOOL_ONLY __intellisense__DEE_UNLIKELY(bool x);
#define DEE_LIKELY(x)   (__intellisense__DEE_LIKELY(x))
#define DEE_UNLIKELY(x) (__intellisense__DEE_UNLIKELY(x))
#else
#define DEE_LIKELY      /* nothing */
#define DEE_UNLIKELY    /* nothing */
#endif
#endif

#ifndef Dee_OFFSETOF
#if defined(__GNUC__) || __has_builtin(__builtin_offsetof)
# define Dee_OFFSETOF       __builtin_offsetof
#elif defined(offsetof)
# define Dee_OFFSETOF       offsetof
#else /* ... */
# define Dee_OFFSETOF(s,m) (DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T))(&((s*)0)->m)
#endif /* ... */
#endif /* !Dee_OFFSETOF */


#ifdef DEE_DEBUG
DEE_DECL_BEGIN
#ifndef DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED
#define DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED 1
DEE_DATA_DECL(int) _DeeFlag_NoAssert;
DEE_DATA_DECL(int) _DeeFlag_Verbose;
#endif /* !DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED */
DEE_FUNC_DECL(void) _Dee_DebugOut(char const *fmt, ...);
DEE_FUNC_DECL(void) _Dee_VerboseOut(char const *fmt, ...);
DEE_FUNC_DECL(DEE_ATTRIBUTE_NORETURN void) _Dee_AbnormalTermination(void);
#ifdef DEE_BUILTIN_BREAKPOINT
DEE_FUNC_DECL(void) _Dee_AssertionFailed(char const *expr_, char const *file_, int line_);
DEE_FUNC_DECL(void) _Dee_AssertionFailedf(char const *expr_, char const *file_, int line_, char const *fmt, ...);
#else /* DEE_BUILTIN_BREAKPOINT */
DEE_FUNC_DECL(DEE_ATTRIBUTE_NORETURN void) _Dee_AssertionFailed(char const *expr_, char const *file_, int line_);
DEE_FUNC_DECL(DEE_ATTRIBUTE_NORETURN void) _Dee_AssertionFailedf(char const *expr_, char const *file_, int line_, char const *fmt, ...);
#endif /* !DEE_BUILTIN_BREAKPOINT */
DEE_DECL_END
#ifdef DEE_BUILTIN_BREAKPOINT
# define DEE_ASSERT(expr)         (DEE_UNLIKELY(!(_DeeFlag_NoAssert||(expr)))?(_Dee_AssertionFailed(#expr,__FILE__,__LINE__),DEE_BUILTIN_BREAKPOINT()):(void)0)
# define DEE_ASSERTF(expr,...)    (DEE_UNLIKELY(!(_DeeFlag_NoAssert||(expr)))?(_Dee_AssertionFailedf(#expr,__FILE__,__LINE__,__VA_ARGS__),DEE_BUILTIN_BREAKPOINT()):(void)0)
#else /* DEE_BUILTIN_BREAKPOINT */
# define DEE_ASSERT(expr)         (DEE_UNLIKELY(!(_DeeFlag_NoAssert||(expr)))?_Dee_AssertionFailed(#expr,__FILE__,__LINE__):(void)0)
# define DEE_ASSERTF(expr,...)    (DEE_UNLIKELY(!(_DeeFlag_NoAssert||(expr)))?_Dee_AssertionFailedf(#expr,__FILE__,__LINE__,__VA_ARGS__):(void)0)
#endif /* !DEE_BUILTIN_BREAKPOINT */
#define DEE_LVERBOSE_LV_R(lv,...) (DEE_UNLIKELY(_DeeFlag_Verbose>=(lv))?_Dee_VerboseOut(__VA_ARGS__):(void)0)
#define DEE_LVERBOSE_LV(lv,...)   (DEE_UNLIKELY(_DeeFlag_Verbose>=(lv))?_Dee_VerboseOut(__FILE__ "(" DEE_PP_STR(__LINE__) ") : " __VA_ARGS__):(void)0)
#define DEE_LVERBOSE1R(...)        DEE_LVERBOSE_LV_R(1,__VA_ARGS__)
#define DEE_LVERBOSE1(...)         DEE_LVERBOSE_LV(1,__VA_ARGS__)
#define DEE_LVERBOSE2R(...)        DEE_LVERBOSE_LV_R(2,__VA_ARGS__)
#define DEE_LVERBOSE2(...)         DEE_LVERBOSE_LV(2,__VA_ARGS__)
#define DEE_LVERBOSE3R(...)        DEE_LVERBOSE_LV_R(3,__VA_ARGS__)
#define DEE_LVERBOSE3(...)         DEE_LVERBOSE_LV(3,__VA_ARGS__)
#define DEE_LVERBOSE4R(...)        DEE_LVERBOSE_LV_R(4,__VA_ARGS__)
#define DEE_LVERBOSE4(...)         DEE_LVERBOSE_LV(4,__VA_ARGS__)
#define DEE_LDEBUG                 _Dee_DebugOut
#define DEE_ABNORMAL_TERMINATION   _Dee_AbnormalTermination
#else /* DEE_DEBUG */
#define DEE_ASSERT				             DEE_COMPILER_ASSUME
#define DEE_ASSERTF(expr,...)				  DEE_COMPILER_ASSUME(expr)
#define DEE_ABNORMAL_TERMINATION   DEE_BUILTIN_UNREACHABLE
#if defined(_MSC_VER)
# define DEE_LDEBUG               (void)__noop
#else
# define DEE_LDEBUG(...)          (void)0
#endif
#endif /* !DEE_DEBUG */


#ifndef DEE_LVERBOSE1
#define DEE_LVERBOSE1  DEE_LDEBUG
#endif /* !DEE_LVERBOSE1 */
#ifndef DEE_LVERBOSE1R
#define DEE_LVERBOSE1R DEE_LDEBUG
#endif /* !DEE_LVERBOSE1R */
#ifndef DEE_LVERBOSE2
#define DEE_LVERBOSE2  DEE_LDEBUG
#endif /* !DEE_LVERBOSE2 */
#ifndef DEE_LVERBOSE2R
#define DEE_LVERBOSE2R DEE_LDEBUG
#endif /* !DEE_LVERBOSE2R */
#ifndef DEE_LVERBOSE3
#define DEE_LVERBOSE3  DEE_LDEBUG
#endif /* !DEE_LVERBOSE3 */
#ifndef DEE_LVERBOSE3R
#define DEE_LVERBOSE3R DEE_LDEBUG
#endif /* !DEE_LVERBOSE3R */
#ifndef DEE_LVERBOSE4
#define DEE_LVERBOSE4  DEE_LDEBUG
#endif /* !DEE_LVERBOSE4 */
#ifndef DEE_LVERBOSE4R
#define DEE_LVERBOSE4R DEE_LDEBUG
#endif /* !DEE_LVERBOSE4R */

#ifdef __INTELLISENSE__
#ifdef __cplusplus
#undef DEE_ASSERT
#undef DEE_ASSERTF
static void __intellisense__DEE_ASSERT(bool expr);
static void __intellisense__DEE_ASSERTF(bool expr, char const *fmt, ...);
#define DEE_ASSERT     __intellisense__DEE_ASSERT
#define DEE_ASSERTF    __intellisense__DEE_ASSERTF
#endif

#undef DEE_LDEBUG
#undef DEE_LVERBOSE1
#undef DEE_LVERBOSE1R
#undef DEE_LVERBOSE2
#undef DEE_LVERBOSE2R
#undef DEE_LVERBOSE3
#undef DEE_LVERBOSE3R
#undef DEE_LVERBOSE4
#undef DEE_LVERBOSE4R
static void __intellisense__DEE_LDEBUG(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE1(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE1R(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE2(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE2R(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE3(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE3R(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE4(char const *fmt, ...);
static void __intellisense__DEE_LVERBOSE4R(char const *fmt, ...);
#define DEE_LDEBUG     __intellisense__DEE_LDEBUG
#define DEE_LVERBOSE1  __intellisense__DEE_LVERBOSE1
#define DEE_LVERBOSE1R __intellisense__DEE_LVERBOSE1R
#define DEE_LVERBOSE2  __intellisense__DEE_LVERBOSE2
#define DEE_LVERBOSE2R __intellisense__DEE_LVERBOSE2R
#define DEE_LVERBOSE3  __intellisense__DEE_LVERBOSE3
#define DEE_LVERBOSE3R __intellisense__DEE_LVERBOSE3R
#define DEE_LVERBOSE4  __intellisense__DEE_LVERBOSE4
#define DEE_LVERBOSE4R __intellisense__DEE_LVERBOSE4R
#endif

#ifdef _PREFAST_
#undef DEE_ASSERT
#define DEE_ASSERT __assume
#endif

#ifndef DEE_BUILTIN_UNREACHABLE
#ifdef DEE_DEBUG
#define DEE_BUILTIN_UNREACHABLE() (\
 DEE_ASSERT(!"DEE_BUILTIN_UNREACHABLE() reached"),\
 DEE_BUILTIN_UNREACHABLE_())
#else
#define DEE_BUILTIN_UNREACHABLE DEE_BUILTIN_UNREACHABLE_
#endif
#endif


#define DEE_CONFIG_SIZEOF_DEE_HASH_T  DEE_TYPES_SIZEOF_SIZE_T


//////////////////////////////////////////////////////////////////////////
// Sizes of runtime deemon types
// NOTE: None of these are guarantied to always have the same size
// For fixed length integers, use the builtin [__|u]intN[_t] types.
#define DEE_CONFIG_RT_CHAR_SIGNED          1
#define DEE_CONFIG_RT_SIZEOF_CHAR          1
#define DEE_CONFIG_RT_WCHAR_T_SIGNED       0
#ifdef DEE_PLATFORM_WINDOWS
#define DEE_CONFIG_RT_SIZEOF_WCHAR_T       2
#else
#define DEE_CONFIG_RT_SIZEOF_WCHAR_T       4
#endif
#define DEE_CONFIG_RT_CHAR16_T_SIGNED      0
#define DEE_CONFIG_RT_SIZEOF_CHAR16_T      2
#define DEE_CONFIG_RT_CHAR32_T_SIGNED      0
#define DEE_CONFIG_RT_SIZEOF_CHAR32_T      4
#define DEE_CONFIG_RT_SIZEOF_BOOL          1
#define DEE_CONFIG_RT_SIZEOF_SHORT         2
#define DEE_CONFIG_RT_SIZEOF_INT           4
#ifndef DEE_CONFIG_RT_SIZEOF_LONG
#if defined(DEE_PLATFORM_64_BIT) && \
   !defined(DEE_PLATFORM_WINDOWS)
#define DEE_CONFIG_RT_SIZEOF_LONG          8
#else
#define DEE_CONFIG_RT_SIZEOF_LONG          4
#endif
#endif
#define DEE_CONFIG_RT_SIZEOF_LLONG         8
#define DEE_CONFIG_RT_SIZEOF_INTMAX_T      8 // Must always be the greatest integer (aka. '8')
#define DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T  1
#define DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T 2
#define DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T 4
#define DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T 8
#define DEE_CONFIG_RT_SIZEOF_INT_FAST8_T   DEE_TYPES_SIZEOF_INT_FAST8_T
#define DEE_CONFIG_RT_SIZEOF_INT_FAST16_T  DEE_TYPES_SIZEOF_INT_FAST16_T
#define DEE_CONFIG_RT_SIZEOF_INT_FAST32_T  DEE_TYPES_SIZEOF_INT_FAST32_T
#define DEE_CONFIG_RT_SIZEOF_INT_FAST64_T  DEE_TYPES_SIZEOF_INT_FAST64_T
#define DEE_CONFIG_RT_FLOATID_FLOAT        DEE_TYPES_FLOATID_FLOAT
#define DEE_CONFIG_RT_FLOATID_DOUBLE       DEE_TYPES_FLOATID_DOUBLE
#ifdef DEE_TYPES_FLOATID_LDOUBLE
#define DEE_CONFIG_RT_FLOATID_LDOUBLE      DEE_TYPES_FLOATID_LDOUBLE
#else
#define DEE_CONFIG_RT_FLOATID_LDOUBLE      DEE_TYPES_FLOATID_DOUBLE
#endif

// The following must always be the same as on the current platform
#define DEE_CONFIG_RT_SIZEOF_SIZE_T    DEE_TYPES_SIZEOF_SIZE_T
#define DEE_CONFIG_RT_SIZEOF_POINTER   DEE_TYPES_SIZEOF_POINTER
#define DEE_CONFIG_RT_SIZEOF_PTRDIFF_T DEE_TYPES_SIZEOF_PTRDIFF_T

#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
#define DEE_CONFIG_RT_SIZEOF_FLOAT DEE_TYPES_SIZEOF_FLOAT
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
#define DEE_CONFIG_RT_SIZEOF_FLOAT DEE_TYPES_SIZEOF_DOUBLE
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_CONFIG_RT_SIZEOF_FLOAT DEE_TYPES_SIZEOF_LDOUBLE
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
#define DEE_CONFIG_RT_SIZEOF_DOUBLE DEE_TYPES_SIZEOF_FLOAT
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
#define DEE_CONFIG_RT_SIZEOF_DOUBLE DEE_TYPES_SIZEOF_DOUBLE
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_CONFIG_RT_SIZEOF_DOUBLE DEE_TYPES_SIZEOF_LDOUBLE
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
#define DEE_CONFIG_RT_SIZEOF_LDOUBLE DEE_TYPES_SIZEOF_FLOAT
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
#define DEE_CONFIG_RT_SIZEOF_LDOUBLE DEE_TYPES_SIZEOF_DOUBLE
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_CONFIG_RT_SIZEOF_LDOUBLE DEE_TYPES_SIZEOF_LDOUBLE
#endif

#include "__annotations.inl"

#ifndef GUARD_DEEMON_VERSION_H
#include "version.h"
#endif

#define DEE_OBJECT_DEF DEE_STRUCT_DEF
#define DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES\
 typedef DEE_TYPES_INT8_T Dee_int8_t;\
 typedef DEE_TYPES_INT16_T Dee_int16_t;\
 typedef DEE_TYPES_INT32_T Dee_int32_t;\
 typedef DEE_TYPES_INT64_T Dee_int64_t;\
 typedef DEE_TYPES_UINT8_T Dee_uint8_t;\
 typedef DEE_TYPES_UINT16_T Dee_uint16_t;\
 typedef DEE_TYPES_UINT32_T Dee_uint32_t;\
 typedef DEE_TYPES_UINT64_T Dee_uint64_t;
#define DEE_PRIVATE_DECL_DEE_SIZE_TYPES\
 typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T) Dee_size_t;\
 typedef DEE_TYPES_INT(DEE_TYPES_SIZEOF_SIZE_T) Dee_ssize_t;\
 typedef DEE_TYPES_INT(DEE_TYPES_SIZEOF_SIZE_T) Dee_ptrdiff_t;\
 typedef DEE_TYPES_INT(DEE_TYPES_SIZEOF_SIZE_T) Dee_intptr_t;\
 typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T) Dee_uintptr_t;
#define DEE_PRIVATE_DECL_DEE_HASH_T\
 typedef DEE_TYPES_UINT(DEE_CONFIG_SIZEOF_DEE_HASH_T) Dee_hash_t;
#define DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES\
 typedef DEE_TYPES_XINT(DEE_CONFIG_RT_CHAR_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR) Dee_rt_char;\
 typedef DEE_TYPES_XINT(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T) Dee_rt_wchar_t;\
 typedef DEE_TYPES_XINT(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T) Dee_rt_char16_t;\
 typedef DEE_TYPES_XINT(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T) Dee_rt_char32_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_BOOL) Dee_rt_bool;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_BOOL) Dee_rt_intbool;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_BOOL) Dee_rt_uintbool;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_SHORT) Dee_rt_short;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_SHORT) Dee_rt_ushort;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT) Dee_rt_int;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT) Dee_rt_uint;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_LONG) Dee_rt_long;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_LONG) Dee_rt_ulong;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_LLONG) Dee_rt_llong;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_LLONG) Dee_rt_ullong;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INTMAX_T) Dee_rt_intmax_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INTMAX_T) Dee_rt_uintmax_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_SIZE_T) Dee_rt_size_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T) Dee_rt_ptrdiff_t;\
 typedef DEE_TYPES_FLOAT(DEE_CONFIG_RT_FLOATID_FLOAT) Dee_rt_float;\
 typedef DEE_TYPES_FLOAT(DEE_CONFIG_RT_FLOATID_DOUBLE) Dee_rt_double;\
 typedef DEE_TYPES_FLOAT(DEE_CONFIG_RT_FLOATID_LDOUBLE) Dee_rt_ldouble;\
 typedef DEE_TYPES_INT8_T Dee_rt_int8_t;\
 typedef DEE_TYPES_INT16_T Dee_rt_int16_t;\
 typedef DEE_TYPES_INT32_T Dee_rt_int32_t;\
 typedef DEE_TYPES_INT64_T Dee_rt_int64_t;\
 typedef DEE_TYPES_UINT8_T Dee_rt_uint8_t;\
 typedef DEE_TYPES_UINT16_T Dee_rt_uint16_t;\
 typedef DEE_TYPES_UINT32_T Dee_rt_uint32_t;\
 typedef DEE_TYPES_UINT64_T Dee_rt_uint64_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T) Dee_rt_int_fast8_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T) Dee_rt_int_fast16_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T) Dee_rt_int_fast32_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T) Dee_rt_int_fast64_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T) Dee_rt_uint_fast8_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T) Dee_rt_uint_fast16_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T) Dee_rt_uint_fast32_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T) Dee_rt_uint_fast64_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T) Dee_rt_int_least8_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T) Dee_rt_int_least16_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T) Dee_rt_int_least32_t;\
 typedef DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T) Dee_rt_int_least64_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T) Dee_rt_uint_least8_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T) Dee_rt_uint_least16_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T) Dee_rt_uint_least32_t;\
 typedef DEE_TYPES_UINT(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T) Dee_rt_uint_least64_t;
#ifndef DEE_WITHOUT_THREADS
#define DEE_PRIVATE_DECL_DEE_THREAD_ID\
 typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_THREADID) DeeThreadID;
#endif
#define DEE_PRIVATE_DECL_DEE_MODE_T\
 typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_MODE_T) Dee_mode_t;
#define DEE_PRIVATE_DECL_DEE_OBJECT                DEE_OBJECT_DEF(DeeObject);
#define DEE_PRIVATE_DECL_DEE_TYPEOBJECT            DEE_OBJECT_DEF(DeeTypeObject);
#define DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT       DEE_OBJECT_DEF(DeeFileTypeObject);
#define DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT     DEE_OBJECT_DEF(DeeStructuredObject);
#define DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT DEE_OBJECT_DEF(DeeStructuredTypeObject);
#define DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND \
typedef int DeeExceptionHandlerKind; enum{\
 DeeExceptionHandleKind_FLAG_ALL   = 0x01,\
 DeeExceptionHandleKind_FLAG_TYPED = 0x02,\
 DeeExceptionHandleKind_FLAG_VAR   = 0x04,\
 DeeExceptionHandleKind_FINALLY    = 0x00,\
 DeeExceptionHandleKind_ALL        = DeeExceptionHandleKind_FLAG_ALL,\
 DeeExceptionHandleKind_ALL_VAR    = DeeExceptionHandleKind_FLAG_ALL|DeeExceptionHandleKind_FLAG_VAR,\
 DeeExceptionHandleKind_TYPED      = DeeExceptionHandleKind_FLAG_TYPED,\
 DeeExceptionHandleKind_TYPED_VAR  = DeeExceptionHandleKind_FLAG_TYPED|DeeExceptionHandleKind_FLAG_VAR,\
};
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS \
typedef DEE_TYPES_UINT16_T DeeFunctionFlags; enum{\
 DeeFunctionFlags_NONE     = 0x0000,\
 DeeFunctionFlags_DEFAULT  = 0x0000,\
 DeeFunctionFlags_SYSV     = 0x0001,\
 DeeFunctionFlags_STDCALL  = 0x0002,\
 DeeFunctionFlags_THISCALL = 0x0003,\
 DeeFunctionFlags_FASTCALL = 0x0004,\
 DeeFunctionFlags_MS_CDECL = 0x0005,\
 DeeFunctionFlags_PASCAL   = 0x0006,\
 DeeFunctionFlags_REGISTER = 0x0007,\
 DeeFunctionFlags_WIN64    = 0x0008,\
 DeeFunctionFlags_UNIX64   = 0x0009,\
 DeeFunctionFlags_CC_MASK  = 0x000F,\
 DeeFunctionFlags_ADD_RESULT_REFERENCE = 0x0010,\
 DeeFunctionFlags_VARARGS  = 0x0020,\
 DeeFunctionFlags_NOEXCEPT = 0x0040,\
 DeeFunctionFlags_NORETURN = 0x0080,\
};
#else
#define DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS \
typedef DEE_TYPES_UINT16_T DeeFunctionFlags; enum{\
 DeeFunctionFlags_NONE     = 0x0000,\
 DeeFunctionFlags_DEFAULT  = 0x0000,\
 DeeFunctionFlags_THISCALL = 0x0003,\
 DeeFunctionFlags_NOEXCEPT = 0x0040,\
 DeeFunctionFlags_NORETURN = 0x0080,\
};
#endif
#define DEE_PRIVATE_DECL_DEE_VISIT_PROC\
 struct DeeObject; typedef int (DEE_CALL *DeeVisitProc)(struct DeeObject*,void*);
#define DEE_PRIVATE_DECL_DEE_CFUNCTION\
 struct DeeObject; typedef DEE_A_RET_EXCEPT_REF struct DeeObject *(DEE_CALL *DeeCFunction)(struct DEE_A_INOUT_OBJECT(DeeTupleObject) *args);
#define DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION\
 struct DeeObject; typedef DEE_A_RET_EXCEPT_REF struct DeeObject *(DEE_CALL *DeeMemberFunction)(\
  DEE_A_INOUT struct DeeObject *self, struct DEE_A_INOUT_OBJECT(DeeTupleObject) *args, void *closure);
#define DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION\
 struct DeeObject; struct DeeStructuredTypeObject;\
 typedef DEE_A_RET_EXCEPT_REF struct DeeObject *(DEE_CALL *DeeStructuredMemberFunction)(\
  DEE_A_INOUT struct DeeStructuredTypeObject *tp_self, DEE_A_INOUT void *self,\
  struct DEE_A_INOUT_OBJECT(DeeTupleObject) *args, void *closure);
#define DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC\
 struct DeeObject; typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeObjectIterateFunc)(\
  DEE_A_INOUT struct DeeObject *elem, DEE_A_IN_OPT void *closure);


#if defined(DEE_PLATFORM_HAVE_IO) && !DEE_PLATFORM_HAVE_IO
#define DEE_PLATFORM_HAVE_IO_HANDLE    0
#elif defined(DEE_PLATFORM_WINDOWS)
#define DEE_PLATFORM_HAVE_IO_HANDLE    1
#define DEE_PRIVATE_DECL_DEE_IO_HANDLE typedef /*HANDLE*/void *DeeIOHandle;
#define DEE_FILEIO_INVALID_HANDLE      ((void *)((Dee_size_t)-1)) // INVALID_HANDLE_VALUE
#elif defined(DEE_PLATFORM_UNIX)
#define DEE_PLATFORM_HAVE_IO_HANDLE    1
#define DEE_PRIVATE_DECL_DEE_IO_HANDLE typedef int DeeIOHandle;
#define DEE_FILEIO_INVALID_HANDLE      (-1)
#endif

#ifndef DEE_PLATFORM_HAVE_IO_HANDLE
#define DEE_PLATFORM_HAVE_IO_HANDLE 0
#endif

#ifndef DEE_PLATFORM_HAVE_IO
#if DEE_PLATFORM_HAVE_IO_HANDLE || \
    DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#define DEE_PLATFORM_HAVE_IO 1
#else
#define DEE_PLATFORM_HAVE_IO 0
#endif
#endif

#ifndef DEE_PLATFORM_HAVE_PROCESS
#if DEE_PLATFORM_HAVE_IO_HANDLE && \
   (defined(DEE_PLATFORM_WINDOWS) ||\
    defined(DEE_PLATFORM_UNIX))
#define DEE_PLATFORM_HAVE_PROCESS 1
#else
#define DEE_PLATFORM_HAVE_PROCESS 0
#endif
#endif



#if defined(DEE_LIMITED_API) && !defined(__INTELLISENSE__) && \
   !defined(DEE_DEBUG) && !defined(_PREFAST_)
// Only enabled if:
//  - Limited api is enabled        (A deemon source file included this header)
//  - Not the intellisense compiler (Would screw with syntax highlighting)
//  - Debug mode is disabled        (In debug mode, the non-macro versions add assertions)
//  - Not the Prefast compiler      (Non-macro versions have annotations)
// NOTE: The non-macro versions are always included for binary compatibility
#define DEE_LIMITED_OPTIMIZATIONS
#endif

#if defined(DEE_LIMITED_API) && \
    defined(DEE_PLATFORM_WINDOWS)
// Just a little helper, so we don't use anything above XP
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#endif

#endif /* !GUARD_DEEMON___CONF_INL */
