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
#ifndef GUARD_DEEMON___FEATURES_INL
#define GUARD_DEEMON___FEATURES_INL 1

#ifndef GUARD_DEEMON___CONF_INL
#error "Don't #include this file directly."
#endif

#ifndef __is_identifier
#define __is_identifier(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#ifndef __has_extension
#define __has_extension   __has_feature
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x) 0
#endif
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif
#ifndef __has_include
#define __has_include(x) 0
#endif
#ifndef __has_include_next
#define __has_include_next(x) 0
#endif

#if defined(__DEEMON__)
#include <__cdefault.dee>
#endif

//////////////////////////////////////////////////////////////////////////
// Preprocessor helper macros
#ifndef DEE_PP_CAT_2
#define DEE_PRIVATE_PP_CAT_2(a,b)       a##b
#define DEE_PRIVATE_PP_CAT_3(a,b,c)     a##b##c
#define DEE_PRIVATE_PP_CAT_4(a,b,c,d)   a##b##c##d
#define DEE_PRIVATE_PP_CAT_5(a,b,c,d,e) a##b##c##d##e
#define DEE_PP_CAT_2(a,b)       DEE_PRIVATE_PP_CAT_2(a,b)
#define DEE_PP_CAT_3(a,b,c)     DEE_PRIVATE_PP_CAT_3(a,b,c)
#define DEE_PP_CAT_4(a,b,c,d)   DEE_PRIVATE_PP_CAT_4(a,b,c,d)
#define DEE_PP_CAT_5(a,b,c,d,e) DEE_PRIVATE_PP_CAT_5(a,b,c,d,e)
#endif

#ifndef DEE_PP_STR
#define DEE_PRIVATE_PP_STR(x) #x
#define DEE_PP_STR(x) DEE_PRIVATE_PP_STR(x)
#endif

#ifndef DEE_PP_MUL8
#ifdef __TPP_EVAL
#define DEE_PP_MUL8(x) __TPP_EVAL(x*8)
#else
#define DEE_PRIVATE_PP_MUL8_1   8
#define DEE_PRIVATE_PP_MUL8_2   16
#define DEE_PRIVATE_PP_MUL8_4   32
#define DEE_PRIVATE_PP_MUL8_8   64
#define DEE_PRIVATE_PP_MUL8_16  128
#define DEE_PRIVATE_PP_MUL8_32  256
#define DEE_PRIVATE_PP_MUL8_64  512
#define DEE_PRIVATE_PP_MUL8_128 1024
#define DEE_PRIVATE_PP_MUL8(x) DEE_PRIVATE_PP_MUL8_##x
#define DEE_PP_MUL8(x) DEE_PRIVATE_PP_MUL8(x)
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Compiler specific features
#ifndef DEE_COMPILER_HAVE_PRAGMA_PACK
#if defined(_MSC_VER) || defined(__GNUC__) || \
    defined(__TINYC__) || defined(__DEEMON__)
#define DEE_COMPILER_HAVE_PRAGMA_PACK 1
#else /* #pragma pack(push,x) */
#define DEE_COMPILER_HAVE_PRAGMA_PACK 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_LONG_LONG
#if (defined(_MSC_VER) && (defined(_MSC_EXTENSIONS) || _MSC_VER >= 1400)) || \
    (defined(__clang__)) || \
    (defined(__GNUC__) && !defined(__DARWIN_NO_LONG_LONG)) || \
    (defined(__BORLANDC__) && __BORLANDC__ >= 0x561 && !defined(__NO_LONG_LONG)) ||\
    (defined(__DEEMON__))
#define DEE_COMPILER_HAVE_LONG_LONG 1
#else /* "long long" */
#define DEE_COMPILER_HAVE_LONG_LONG 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_FLOAT
#if 1
#define DEE_COMPILER_HAVE_FLOAT 1
#else /* "float" */
#define DEE_COMPILER_HAVE_FLOAT 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_DOUBLE
#if 1
#define DEE_COMPILER_HAVE_DOUBLE 1
#else /* "double" */
#define DEE_COMPILER_HAVE_DOUBLE 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_LDOUBLE
#if !defined(__NO_LONG_DOUBLE_MATH)
#define DEE_COMPILER_HAVE_LDOUBLE 1
#else /* "long double" */
#define DEE_COMPILER_HAVE_LDOUBLE 0
#endif
#endif


#ifndef DEE_COMPILER_HAVE_CXX_INLINE
#if defined(__cplusplus)
#define DEE_COMPILER_HAVE_CXX_INLINE 1
#else /* "inline" */
#define DEE_COMPILER_HAVE_CXX_INLINE 0
#endif
#endif

#ifndef DEE_ATTRIBUTE_ALWAYS_INLINE
#if __has_attribute(always_inline) || \
   (defined(__GNUC__) && __GNUC__*100+__GNUC_MINOR__ >= 301)
#define DEE_ATTRIBUTE_ALWAYS_INLINE __attribute__((__always_inline__))
#else
#define DEE_ATTRIBUTE_ALWAYS_INLINE /* nothing */
#endif
#endif

#if __has_attribute(malloc) || \
   (defined(__GNUC__) && __GNUC__ >= 3)
#define DEE_ATTRIBUTE_MALLOC __attribute__((__malloc__))
#elif __has_declspec_attribute(restrict) || \
     (defined(_MSC_VER) && _MSC_VER >= 1400)
#define DEE_ATTRIBUTE_MALLOC __declspec(restrict)
#else
#define DEE_ATTRIBUTE_MALLOC /* nothing */
#endif

#ifndef DEE_COMPILER_HAVE_MSVC_INLINE
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#define DEE_COMPILER_HAVE_MSVC_INLINE 1
#else /* "__inline" */
#define DEE_COMPILER_HAVE_MSVC_INLINE 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_GCC_INLINE
#if defined(__GNUC__)
#define DEE_COMPILER_HAVE_GCC_INLINE 1
#else /* "__inline__" */
#define DEE_COMPILER_HAVE_GCC_INLINE 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_MSVC_COUNTER
#if defined(__COUNTER__)
#define DEE_COMPILER_HAVE_MSVC_COUNTER 1
#else /* "__COUNTER__" */
#define DEE_COMPILER_HAVE_MSVC_COUNTER 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_MSVC_SEH
#if defined(_MSC_VER)
#define DEE_COMPILER_HAVE_MSVC_SEH 1
#else /* "__try" */
#define DEE_COMPILER_HAVE_MSVC_SEH 0
#endif
#endif


#ifndef DEE_PRIVATE_CODEGEAR_0X_SUPPORT
#if defined(__BORLANDC__) && __BORLANDC__ >= 0x599
#pragma defineonoption DEE_PRIVATE_CODEGEAR_0X_SUPPORT -Ax
#endif
#endif

#ifndef DEE_COMPILER_HAVE_CXX11_STATIC_ASSERT
#if __has_feature(cxx_static_assert) || \
   (defined(__IBMCPP_STATIC_ASSERT) && __IBMCPP_STATIC_ASSERT) || (defined(__cplusplus) && (\
   (defined(_MSC_VER) && _MSC_VER >= 1600) || \
   (defined(__GNUC__) && __GNUC__*100+__GNUC_MINOR__ >= 403 && \
    (defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L))) || \
   (defined(__BORLANDC__) && defined(DEE_PRIVATE_CODEGEAR_0X_SUPPORT) && __BORLANDC__ >= 0x610)))
#define DEE_COMPILER_HAVE_CXX11_STATIC_ASSERT 1
#else /* "static_assert" */
#define DEE_COMPILER_HAVE_CXX11_STATIC_ASSERT 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#if defined(_MSC_VER)
#define DEE_COMPILER_HAVE_WIDESTRING_PREFIX 1
#else /* L"..." / L'?' */
#define DEE_COMPILER_HAVE_WIDESTRING_PREFIX 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_UTF16STRING_PREFIX
#if defined(__DEEMON__) && __DEEMON__ >= 101
#define DEE_COMPILER_HAVE_UTF16STRING_PREFIX 1
#else /* u"..." / u'?' */
#define DEE_COMPILER_HAVE_UTF16STRING_PREFIX 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_UTF32STRING_PREFIX
#if defined(__DEEMON__) && __DEEMON__ >= 101
#define DEE_COMPILER_HAVE_UTF32STRING_PREFIX 1
#else /* U"..." / U'?' */
#define DEE_COMPILER_HAVE_UTF32STRING_PREFIX 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_BUILTIN_EXPECT
#if __has_builtin(__builtin_expect) || \
  (!defined(__clang__) && defined(__GNUC__))
#define DEE_COMPILER_HAVE_BUILTIN_EXPECT 1
#else /* "__builtin_expect" */
#define DEE_COMPILER_HAVE_BUILTIN_EXPECT 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_MSVC_FIXED_LENGTH_INT
#if __has_extension(tpp_msvc_integer_suffix) || \
   (defined(_MSC_VER) && !defined(__ANDROID__))
#define DEE_COMPILER_HAVE_MSVC_FIXED_LENGTH_INT 1
#else
#define DEE_COMPILER_HAVE_MSVC_FIXED_LENGTH_INT 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_MSVC_ASSUME
#if defined(_MSC_VER)
#define DEE_COMPILER_HAVE_MSVC_ASSUME 1
#else /* "__assume" */
#define DEE_COMPILER_HAVE_MSVC_ASSUME 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_C_STRUCT_NAMESPACE
#if !defined(__cplusplus)
#define DEE_COMPILER_HAVE_C_STRUCT_NAMESPACE 1
#else
#define DEE_COMPILER_HAVE_C_STRUCT_NAMESPACE 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_CXX_UNNAMED_ARGUMENTS
#if defined(__cplusplus) || defined(__DEEMON__)
# define DEE_COMPILER_HAVE_CXX_UNNAMED_ARGUMENTS 1
#else
# define DEE_COMPILER_HAVE_CXX_UNNAMED_ARGUMENTS 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_BUILTIN_UNREACHABLE
#if __has_builtin(__builtin_unreachable) || \
   (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)))
# define DEE_COMPILER_HAVE_BUILTIN_UNREACHABLE 1
#else
# define DEE_COMPILER_HAVE_BUILTIN_UNREACHABLE 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_BUILTIN_CHOOSE_EXPR
#if __has_builtin(__builtin_choose_expr) || defined(__GNUC__)
# define DEE_COMPILER_HAVE_BUILTIN_CHOOSE_EXPR 1
#else
# define DEE_COMPILER_HAVE_BUILTIN_CHOOSE_EXPR 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P
#if __has_builtin(__builtin_constant_p) || defined(__GNUC__)
# define DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P 1
#else
#define DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P 0
# endif
#endif

// Early evaluation of __builtin_constant_p,
// allowing it to be used in __builtin_choose_expr
#ifndef DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P_EARLY_EVALUATE
#if DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P && defined(__DEEMON__)
# define DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P_EARLY_EVALUATE 1
#else
# define DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P_EARLY_EVALUATE 0
#endif
#endif

#ifndef DEE_GCC_EXTENSION
#if defined(__GNUC__)
# define DEE_GCC_EXTENSION __extension__
#else
# define DEE_GCC_EXTENSION /* nothing */
#endif
#endif

#ifndef DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#if defined(__GNUC__) || __has_feature(__expr_statement__)
# define DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS 1
#else
# define DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS 0
#endif
#endif

#ifndef DEE_BUILTIN_LATE_CHOOSE_EXPR
#if DEE_COMPILER_HAVE_BUILTIN_CONSTANT_P_EARLY_EVALUATE
# define DEE_BUILTIN_LATE_CHOOSE_EXPR          __builtin_choose_expr
#else
# define DEE_BUILTIN_LATE_CHOOSE_EXPR(x,tt,ff) ((x)?(tt):(ff))
#endif
#endif

#ifndef DEE_COMPILER_HAVE_CXX11_NOEXCEPT
#if defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS) && \
   (__has_feature(cxx_noexcept) || \
   (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46) || \
   (defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 190023026)))
# define DEE_COMPILER_HAVE_CXX11_NOEXCEPT 1
#else
# define DEE_COMPILER_HAVE_CXX11_NOEXCEPT 0
#endif
#endif /* !DEE_COMPILER_HAVE_CXX11_NOEXCEPT */


//////////////////////////////////////////////////////////////////////////
// debug_new config
#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_DEBUG_NEW_H
#if __has_include(<debug_new.h>) || \
   (defined(DEE_USE_DEBUG_NEW) && DEE_USE_DEBUG_NEW) || \
    defined(HAVE_DEBUG_NEW_H) || \
    defined(HAVE_INCLUDE_DEBUG_NEW_H) || \
    defined(GUARD_DEBUG_NEW_H)
# define DEE_ENVIRONMENT_HAVE_INCLUDE_DEBUG_NEW_H 1
#else
# define DEE_ENVIRONMENT_HAVE_INCLUDE_DEBUG_NEW_H 0
#endif
#endif
#ifndef DEE_USE_DEBUG_NEW
#if DEE_ENVIRONMENT_HAVE_INCLUDE_DEBUG_NEW_H
# define DEE_USE_DEBUG_NEW 1
#else
# define DEE_USE_DEBUG_NEW 0
#endif
#endif /* !DEE_USE_DEBUG_NEW */




//////////////////////////////////////////////////////////////////////////
// Platform config

#ifdef __APPLE__
#if DEE_USE_DEBUG_NEW
#include <debug_new_disable.inl>
#endif
#include "AvailabilityMacros.h"
#include "TargetConditionals.h"
#if DEE_USE_DEBUG_NEW
#include <debug_new_enable.inl>
#endif
#endif /* __APPLE__ */

#ifndef DEE_PLATFORM_APPLE
#if defined(__APPLE__) || defined(__MACOSX__) || defined(TARGET_OS_IPHONE)
#define DEE_PLATFORM_APPLE 1
#endif
#endif

#if !defined(_WIN32) && !defined(__DEEMON__) && (\
     defined(__unix__) || defined(__unix) || \
    (defined(__APPLE__) && defined(__MACH__)))
// To grab '_POSIX_VERSION'
#if DEE_USE_DEBUG_NEW
# include <debug_new_disable.inl>
# include <unistd.h>
# include <debug_new_enable.inl>
#else
# include <unistd.h>
#endif
#endif

#ifdef _POSIX_VERSION
#if _POSIX_VERSION >= 198808L
# define DEE_PLATFORM_POSIX_1_1988     1
#if _POSIX_VERSION >= 199009L
# define DEE_PLATFORM_POSIX_1_1990     1
#if _POSIX_VERSION >= 199506L
# define DEE_PLATFORM_ISO_POSIX_1_1996 1
#if _POSIX_VERSION >= 200112L
# define DEE_PLATFORM_ISO_POSIX_1_2001 1
#if _POSIX_VERSION >= 200809L
# define DEE_PLATFORM_ISO_POSIX_1_2008 1
#endif
#endif
#endif
#endif
#endif
#endif


#ifndef DEE_PLATFORM_UNIX
#if (defined(linux) || defined(__linux) || defined(__linux__) || \
     defined(__unix__) || defined(__unix) || defined(_POSIX_) || defined(__MACOSX__) || \
    (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE /*&& defined(__LP64__)*/) || \
     defined(__ANDROID__) || defined(__ANDROID) || defined(_ANDROID) || \
     defined(__android__) || defined(__android) || defined(_android)) && (\
    !defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__CYGWIN__))
# define DEE_PLATFORM_UNIX 1
#endif
#endif



#ifndef DEE_PLATFORM_WINDOWS
#if defined(_WIN32) || defined(WIN32) || \
    defined(_WIN64) || defined(WIN64) || \
    defined(__WINDOWS__) || defined(__MINGW32__) || \
    defined(__MINGW64__) || defined(__CYGWIN__)
#define DEE_PLATFORM_WINDOWS 1
#endif
#endif

#ifndef DEE_PLATFORM_ARM
#if defined(_ARM_) || defined(_M_ARM) || defined(__arm__) || \
    defined(__arm) || defined(arm)
#define DEE_PLATFORM_ARM 1
#endif
#endif


#if defined(DEE_PLATFORM_UNIX) && defined(DEE_PLATFORM_WINDOWS)
#error "Deemon: Windows __AND__ Posix platform? - We can't have both as target!"
#endif

#ifndef DEE_PLATFORM_64_BIT
#if defined(_WIN64) || defined(WIN64) || defined(_X64_) || defined(_M_X64) || \
   (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE && defined(__LP64__)) || \
   (defined(CONFIG_64BIT)) || (defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8)
#define DEE_PLATFORM_64_BIT 1
#endif
#endif

#if defined(DEE_PLATFORM_WINDOWS) || \
    defined(DEE_PLATFORM_UNIX)
#define DEE_PLATFORM_HAVE_PIPES 1
#else
#define DEE_PLATFORM_HAVE_PIPES 0
#endif


#ifndef DEE_PLATFORM_WINDOWS
#undef DEE_COMPILER_HAVE_MSVC_SEH
#define DEE_COMPILER_HAVE_MSVC_SEH 0
#endif



//////////////////////////////////////////////////////////////////////////
// have-include config
#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#if __has_include(<io.h>) || defined(_MSC_VER) || \
    defined(HAVE_IO_H) || defined(HAVE_INCLUDE_IO_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H
#if __has_include(<stddef.h>) || 1 || \
    defined(HAVE_STDDEF_H) || defined(HAVE_INCLUDE_STDDEF_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H
#if __has_include(<stdint.h>) || 1 || \
    defined(HAVE_STDINT_H) || defined(HAVE_INCLUDE_STDINT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#if __has_include(<stdarg.h>) || defined(_MSC_VER) || 1 || \
    defined(HAVE_STDARG_H) || defined(HAVE_INCLUDE_STDARG_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#if __has_include(<time.h>) || defined(_MSC_VER) || \
    defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_TIME_H) || defined(HAVE_INCLUDE_TIME_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_PROCESS_H
#if __has_include(<process.h>) || defined(_MSC_VER) || \
    defined(HAVE_PROCESS_H) || defined(HAVE_INCLUDE_PROCESS_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PROCESS_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PROCESS_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#if __has_include(<pthread.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_PTHREAD_H) || defined(HAVE_INCLUDE_PTHREAD_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H
#if __has_include(<sched.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SCHED_H) || defined(HAVE_INCLUDE_SCHED_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SEMAPHORE_H
#if __has_include(<semaphore.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SEMAPHORE_H) || defined(HAVE_INCLUDE_SEMAPHORE_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SEMAPHORE_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SEMAPHORE_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#if __has_include(<errno.h>) || defined(_MSC_VER) || \
    defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_ERRNO_H) || defined(HAVE_INCLUDE_ERRNO_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#if __has_include(<fcntl.h>) || defined(_MSC_VER) || \
    defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_FCNTL_H) || defined(HAVE_INCLUDE_FCNTL_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#if __has_include(<stdio.h>) || 1 || \
    defined(HAVE_STDIO_H) || defined(HAVE_INCLUDE_STDIO_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#if __has_include(<features.h>) || defined(DEE_PLATFORM_UNIX) ||\
   (defined(__GNUC__) && !defined(DEE_PLATFORM_WINDOWS)) ||\
    defined(HAVE_FEATURES_H)|| defined(HAVE_INCLUDE_FEATURES_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STRING_H
#if __has_include(<string.h>) || 1 ||\
    defined(HAVE_STRING_H)|| defined(HAVE_INCLUDE_STRING_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STRING_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STRING_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_LIMITS_H
#if __has_include(<limits.h>) || 1 ||\
    defined(HAVE_LIMITS_H)|| defined(HAVE_INCLUDE_LIMITS_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_LIMITS_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_LIMITS_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H
#if __has_include(<dirent.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_DIRENT_H) || defined(HAVE_INCLUDE_DIRENT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SETJMP_H
#if __has_include(<setjmp.h>) || 1 || \
    defined(HAVE_SETJMP_H) || defined(HAVE_INCLUDE_SETJMP_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SETJMP_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SETJMP_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H
#if __has_include(<utime.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_UTIME_H) || defined(HAVE_INCLUDE_UTIME_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#if __has_include(<unistd.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_UNISTD_H) || defined(HAVE_INCLUDE_UNISTD_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H
#if __has_include(<stdlib.h>) || 1 || \
    defined(HAVE_STDLIB_H)|| defined(HAVE_INCLUDE_STDLIB_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H
#if __has_include(<sys/sysinfo.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_SYSINFO_H) || defined(HAVE_INCLUDE_SYS_SYSINFO_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H
#if __has_include(<sys/sysctl.h>) || defined(DEE_PLATFORM_APPLE) || \
    defined(HAVE_SYS_SYSCTL_H) || defined(HAVE_INCLUDE_SYS_SYSCTL_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SOCKET_H
#if __has_include(<sys/socket.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_SOCKET_H) || defined(HAVE_INCLUDE_SYS_SOCKET_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SOCKET_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SOCKET_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H
#if __has_include(<sys/stat.h>) || defined(_MSC_VER) || \
    defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_STAT_H) || defined(HAVE_INCLUDE_SYS_STAT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#if __has_include(<sys/types.h>) || defined(_MSC_VER) || \
    defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_TYPES_H) || defined(HAVE_INCLUDE_SYS_TYPES_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_WAIT_H
#if __has_include(<sys/wait.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_WAIT_H) || defined(HAVE_INCLUDE_SYS_WAIT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_WAIT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_WAIT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H
#if __has_include(<sys/time.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_TIME_H) || defined(HAVE_INCLUDE_SYS_TIME_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_RESOURCE_H
#if __has_include(<sys/resource.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SYS_RESOURCE_H) || defined(HAVE_INCLUDE_SYS_RESOURCE_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_RESOURCE_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_RESOURCE_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H
#if __has_include(<sys/select.h>) || defined(DEE_PLATFORM_ISO_POSIX_1_2001) || \
    defined(HAVE_SYS_SELECT_H) || defined(HAVE_INCLUDE_SYS_SELECT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H
#if __has_include(<pwd.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_PWD_H) || defined(HAVE_INCLUDE_PWD_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_NETDB_H
#if __has_include(<netdb.h>) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_NETDB_H) || defined(HAVE_INCLUDE_NETDB_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_NETDB_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_NETDB_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H
#if __has_include(<signal.h>) || \
    defined(_MSC_VER) || defined(DEE_PLATFORM_UNIX) || \
    defined(HAVE_SIGNAL_H) || defined(HAVE_INCLUDE_SIGNAL_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_ALLOCA_H
#if __has_include(<alloca.h>) || defined(__GNUC__) ||\
    defined(DEE_PLATFORM_UNIX) ||\
    defined(HAVE_ALLOCA_H) || defined(HAVE_INCLUDE_ALLOCA_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ALLOCA_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ALLOCA_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_MALLOC_H
#if __has_include(<malloc.h>) || \
    defined(_MSC_VER) || defined(__GNUC__) || \
    defined(HAVE_MALLOC_H) || defined(HAVE_INCLUDE_MALLOC_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_MALLOC_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_MALLOC_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_MATH_H
#if __has_include(<math.h>) || 1 || \
    defined(HAVE_MATH_H)|| defined(HAVE_INCLUDE_MATH_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_MATH_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_MATH_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_FLOAT_H
#if __has_include(<float.h>) || defined(_MSC_VER) || \
    defined(HAVE_FLOAT_H) || defined(HAVE_INCLUDE_FLOAT_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FLOAT_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_FLOAT_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_BLUETOOTH_H
#if __has_include(<bluetooth/bluetooth.h>) || \
    defined(HAVE_BLUETOOTH_BLUETOOTH_H) || \
    defined(HAVE_INCLUDE_BLUETOOTH_BLUETOOTH_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_BLUETOOTH_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_BLUETOOTH_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_H
#if __has_include(<bluetooth.h>) || \
    defined(HAVE_BLUETOOTH_H) || \
    defined(HAVE_INCLUDE_BLUETOOTH_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_LINUX_NETLINK_H
#if __has_include(<linux/netlink.h>) || \
    defined(HAVE_LINUX_NETLINK_H) || \
    defined(HAVE_INCLUDE_LINUX_NETLINK_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_LINUX_NETLINK_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_LINUX_NETLINK_H 0
#endif
#endif

#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_ASM_TYPES_H
#if __has_include(<asm/types.h>) || \
    defined(HAVE_ASM_TYPES_H) || \
    defined(HAVE_INCLUDE_ASM_TYPES_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ASM_TYPES_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_ASM_TYPES_H 0
#endif
#endif


#ifndef DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_UN_H
#if __has_include(<sys/un.h>) || \
    defined(HAVE_SYS_UN_H) || \
    defined(HAVE_INCLUDE_SYS_UN_H)
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_UN_H 1
#else
#define DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_UN_H 0
#endif
#endif




#ifndef DEE_COMPILER_PRAGMA
#if defined(_MSC_VER)
#define DEE_COMPILER_PRAGMA    __pragma
#else
#define DEE_COMPILER_PRAGMA(x) _Pragma(#x)
#endif
#endif /* !DEE_COMPILER_PRAGMA */

#if defined(_MSC_VER) || __has_feature(__tpp_msvc_integer_suffix__)
# define DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX 1
#else
# define DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX 0
#endif

#ifndef DEE_ATTRIBUTE_DLLIMPORT
#if defined(DEE_PLATFORM_WINDOWS) || defined(__CYGWIN__)
#if __has_declspec_attribute(dllimport) || defined(_MSC_VER)
#define DEE_ATTRIBUTE_DLLIMPORT __declspec(dllimport)
#define DEE_ATTRIBUTE_DLLEXPORT __declspec(dllexport)
#elif __has_attribute(dllimport) || defined(__TINYC__) || \
     (defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__) >= 205)
#define DEE_ATTRIBUTE_DLLIMPORT __attribute__((__dllimport__))
#define DEE_ATTRIBUTE_DLLEXPORT __attribute__((__dllexport__))
#endif
#else
#if __has_attribute(visibility) || \
   (defined(__GNUC__) && __GNUC__ >= 4)
#define DEE_ATTRIBUTE_DLLIMPORT __attribute__((__visibility__("default")))
#define DEE_ATTRIBUTE_DLLEXPORT __attribute__((__visibility__("default")))
#endif
#endif
#ifndef DEE_ATTRIBUTE_DLLIMPORT
#define DEE_ATTRIBUTE_DLLIMPORT /* nothing */
#endif
#ifndef DEE_ATTRIBUTE_DLLEXPORT
#define DEE_ATTRIBUTE_DLLEXPORT /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_NORETURN
#if __has_declspec_attribute(noreturn) || \
   (defined(_MSC_VER) && _MSC_VER >= 1200)
# define DEE_ATTRIBUTE_NORETURN  __declspec(noreturn)
#elif __has_attribute(noreturn) || defined(__clang__) || \
     (defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__) >= 205)
# define DEE_ATTRIBUTE_NORETURN  __attribute__((__noreturn__))
#elif __has_cpp_attribute(noreturn) || \
     (defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER >= 1900)
# define DEE_ATTRIBUTE_NORETURN  [[noreturn]]
#elif __has_include(<stdnoreturn.h>)
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
# include <debug_new_disable.inl>
# include <stdnoreturn.h>
# include <debug_new_enable.inl>
#else
# include <stdnoreturn.h>
#endif
#ifdef noreturn
# define DEE_ATTRIBUTE_NORETURN  noreturn
#endif
#endif
#ifndef DEE_ATTRIBUTE_NORETURN
# define DEE_ATTRIBUTE_NORETURN  /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_NOINLINE
#if __has_declspec_attribute(noinline) || \
   (defined(_MSC_VER) && _MSC_VER >= 1300)
# define DEE_ATTRIBUTE_NOINLINE __declspec(noinline)
#elif __has_attribute(__noinline__) ||\
     (defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)))
#ifdef __CUDACC__
# define DEE_ATTRIBUTE_NOINLINE __attribute__((noinline))
#else
# define DEE_ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#endif
#else
# define DEE_ATTRIBUTE_NOINLINE /* nothing */
#endif
#endif


#ifndef DEE_ATTRIBUTE_CONST
#if __has_declspec_attribute(noalias) || \
   (defined(_MSC_VER) && _MSC_VER >= 1400)
# define DEE_ATTRIBUTE_CONST __declspec(noalias)
#elif __has_attribute(const) || \
     (defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)))
# define DEE_ATTRIBUTE_CONST __attribute__((__const__))
#else
# define DEE_ATTRIBUTE_CONST  /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_NONNULL
#if __has_attribute(nonnull) || \
   (defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)))
#define DEE_ATTRIBUTE_NONNULL(ids) __attribute__((__nonnull__ ids))
#elif defined(__nonnull)
#define DEE_ATTRIBUTE_NONNULL      __nonnull
#elif defined(__INTELLISENSE__) || (defined(_MSC_VER) && 1)
// enforce usage of double parenthesis
#define DEE_PRIVATE_ATTRIBUTE_NONNULL(...) /* nothing */
#define DEE_ATTRIBUTE_NONNULL(ids) DEE_PRIVATE_ATTRIBUTE_NONNULL ids
#else
#define DEE_ATTRIBUTE_NONNULL(ids) /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_FALLTHROUGH
#if __has_cpp_attribute(fallthrough)
#define DEE_ATTRIBUTE_FALLTHROUGH [[fallthrough]];
#else
#define DEE_ATTRIBUTE_FALLTHROUGH /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_SENTINAL
#if __has_attribute(sentinel) || \
   (defined(__GNUC__)  && __GNUC__ >= 4)
#define DEE_ATTRIBUTE_SENTINAL __attribute__((__sentinel__))
#else
#define DEE_ATTRIBUTE_SENTINAL /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_UNUSED
#if __has_attribute(unused) || \
   (defined(__GNUC__)  && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)))
#define DEE_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define DEE_ATTRIBUTE_UNUSED /* nothing */
#endif
#endif

// Indicates that a reference should be added to the returned deemon object
// Requires that the return type be of pointer-to struct with uuid == 'DEE_UUID_DeeObject'
#ifndef DEE_ATTRIBUTE_ADD_RESULT_REFERENCE
#if defined(__DEEMON__) && __has_attribute(add_result_reference)
#define DEE_ATTRIBUTE_ADD_RESULT_REFERENCE  __attribute__((__add_result_reference__))
#else
#define DEE_ATTRIBUTE_ADD_RESULT_REFERENCE  /* nothing */
#endif
#endif

#ifndef DEE_ATTRIBUTE_DEPRECATED
#if __has_declspec_attribute(deprecated) || \
   (defined(_MSC_VER) && _MSC_VER >= 1200)
#define DEE_ATTRIBUTE_DEPRECATED(reason) __declspec(deprecated(reason))
#elif __has_attribute(deprecated) || \
     (defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)))
#if (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))) || \
     defined(__DEEMON__)
#define DEE_ATTRIBUTE_DEPRECATED(reason) __attribute__((__deprecated__(reason)))
#else
#define DEE_ATTRIBUTE_DEPRECATED(reason) __attribute__((__deprecated__))
#endif
#elif (__has_cpp_attribute(deprecated) >= 201309) || \
      (defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER >= 1900)
#define DEE_ATTRIBUTE_DEPRECATED(reason) [[deprecated(reason)]]
#else
#define DEE_ATTRIBUTE_DEPRECATED(reason) /* nothing */
#endif
#endif


#ifndef DEE_CXX11_NOEXCEPT
#if defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS)
#if DEE_COMPILER_HAVE_CXX11_NOEXCEPT
# define DEE_CXX11_NOEXCEPT noexcept
#else
# define DEE_CXX11_NOEXCEPT throw()
#endif
#else
# define DEE_CXX11_NOEXCEPT /* nothing */
#endif
#endif /* !DEE_CXX11_NOEXCEPT */

#if __has_feature(cxx_constexpr) || \
   (defined(__cpp_constexpr) && __cpp_constexpr >= 200704) || (defined(__cplusplus) && \
   (defined(__IBMCPP__) && defined(__IBMCPP_CONSTEXPR) && __IBMCPP_CONSTEXPR) || \
   (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5130) || \
   (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && \
   (defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)) || \
   (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026))
# define DEE_STATIC_CONST(T,decl) static constexpr T decl
#elif defined(__cplusplus) && (\
   (!defined(__SUNPRO_CC) || __SUNPRO_CC > 0x520) &&\
   (!defined(__IBMCPP__) || __IBMCPP__ > 502) &&\
   (!defined(MPW_CPLUS) || MPW_CPLUS > 0x890))
# define DEE_STATIC_CONST(T,decl) static T const decl
#else
# define DEE_STATIC_CONST(T,decl) enum{decl}
#endif

#if __has_feature(deleted_functions) || (defined(__cplusplus) &&\
   (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180020827))
#define DEE_CXX_DELETE_CLASSDEFAULT(T)\
 T() = delete;\
 T(T const&) = delete;\
 T &operator = (T const&) = delete;
#else
#define DEE_CXX_DELETE_CLASSDEFAULT(T)\
private:\
 T();\
 T(T const&);\
 T &operator = (T const&);\
public:
#endif



//////////////////////////////////////////////////////////////////////////
// Platform endian
#ifndef DEE_PLATFORM_ENDIAN
#if defined(__BYTEORDER__)
#if __BYTEORDER__ == 1234
#define DEE_PLATFORM_ENDIAN 1234
#elif __BYTEORDER__ == 4321
#define DEE_PLATFORM_ENDIAN 4321
#endif
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
#include <debug_new_disable.inl>
#endif
#include <endian.h>
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
#include <debug_new_enable.inl>
#endif
#if defined(__BYTE_ORDER)
# define DEE_PRIVATE_PLATFORM_ENDIAN  __BYTE_ORDER
#elif defined(_BYTE_ORDER)
# define DEE_PRIVATE_PLATFORM_ENDIAN  _BYTE_ORDER
#elif defined(BYTE_ORDER)
# define DEE_PRIVATE_PLATFORM_ENDIAN  BYTE_ORDER
#endif
#ifdef DEE_PRIVATE_PLATFORM_ENDIAN
// Remove possible parenthesis around the number
// >> Required as we might need to DEE_PP_STR() this constant
#if DEE_PRIVATE_PLATFORM_ENDIAN == 1234
# define DEE_PLATFORM_ENDIAN 1234
#elif DEE_PRIVATE_PLATFORM_ENDIAN == 4321
# define DEE_PLATFORM_ENDIAN 4321
#elif DEE_PRIVATE_PLATFORM_ENDIAN == 3412
# define DEE_PLATFORM_ENDIAN 3412
#elif defined(__TPP_EVAL)
# define DEE_PLATFORM_ENDIAN __TPP_EVAL(DEE_PRIVATE_PLATFORM_ENDIAN)
#else
# define DEE_PLATFORM_ENDIAN DEE_PRIVATE_PLATFORM_ENDIAN
#endif
#endif
#endif
#ifndef DEE_PLATFORM_ENDIAN
#if defined(__hppa__) || defined(__m68k__) || \
    defined(_M_M68K) || defined(__M_M68K__) || \
   (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__powerpc__) || \
    defined(_M_PPC) || defined(__ARMEB__) || \
    defined(__sparc__)
# define DEE_PLATFORM_ENDIAN 4321
#else /* Big-Endian... */
# define DEE_PLATFORM_ENDIAN 1234
#endif /* Lil-Endian... */
#endif
#if DEE_PLATFORM_ENDIAN == 1234
#define DEE_PLATFORM_LIL_ENDIAN 1
#elif DEE_PLATFORM_ENDIAN == 4321
#define DEE_PLATFORM_BIG_ENDIAN 1
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Sizes of different types
// >> These are used everywhere, so changing them
//    should fix errors from invalid type sizes
// >> 'DEE_TYPES_SIZEOF_LLONG' is optional, but all others are mandatory
#if defined(__SIZEOF_CHAR__)
#define DEE_TYPES_SIZEOF_CHAR  __SIZEOF_CHAR__
#else
#define DEE_TYPES_SIZEOF_CHAR  1
#endif
#if defined(__SIZEOF_SHORT__)
#define DEE_TYPES_SIZEOF_SHORT __SIZEOF_SHORT__
#else
#define DEE_TYPES_SIZEOF_SHORT 2
#endif
#if defined(__SIZEOF_INT__)
#define DEE_TYPES_SIZEOF_INT   __SIZEOF_INT__
#else
#define DEE_TYPES_SIZEOF_INT   4
#endif
#if defined(__SIZEOF_LONG__)
#define DEE_TYPES_SIZEOF_LONG  __SIZEOF_LONG__
#elif defined(DEE_PLATFORM_64_BIT) && \
     !defined(DEE_PLATFORM_WINDOWS)
#define DEE_TYPES_SIZEOF_LONG  8
#else
#define DEE_TYPES_SIZEOF_LONG  4
#endif
#if defined(__SIZEOF_LLONG__)
#define DEE_TYPES_SIZEOF_LLONG __SIZEOF_LLONG__
#elif defined(__SIZEOF_LONG_LONG__)
#define DEE_TYPES_SIZEOF_LLONG __SIZEOF_LONG_LONG__
#elif DEE_COMPILER_HAVE_LONG_LONG
#define DEE_TYPES_SIZEOF_LLONG 8
#endif

#ifndef DEE_TYPES_SIZEOF_FLOAT
#if DEE_COMPILER_HAVE_FLOAT
#if defined(__SIZEOF_FLOAT__)
# define DEE_TYPES_SIZEOF_FLOAT __SIZEOF_FLOAT__
#else
# define DEE_TYPES_SIZEOF_FLOAT 4
#endif
#endif
#endif

#ifndef DEE_TYPES_SIZEOF_DOUBLE
#if DEE_COMPILER_HAVE_DOUBLE
#if defined(__SIZEOF_DOUBLE__)
# define DEE_TYPES_SIZEOF_DOUBLE __SIZEOF_DOUBLE__
#else
# define DEE_TYPES_SIZEOF_DOUBLE 8
#endif
#endif
#endif

#ifndef DEE_TYPES_SIZEOF_LDOUBLE
#if DEE_COMPILER_HAVE_LDOUBLE
#if defined(__SIZEOF_LONG_DOUBLE__)
# define DEE_TYPES_SIZEOF_LDOUBLE __SIZEOF_LONG_DOUBLE__
#elif defined(__SIZEOF_LDOUBLE__)
# define DEE_TYPES_SIZEOF_LDOUBLE __SIZEOF_LDOUBLE__
#elif defined(__C67__) || defined(__i386__)
# define DEE_TYPES_SIZEOF_LDOUBLE 12
#elif defined(__x86_64__)
# define DEE_TYPES_SIZEOF_LDOUBLE 16
#elif defined(DEE_PLATFORM_ARM)
# define DEE_TYPES_SIZEOF_LDOUBLE 8
#else
# define DEE_TYPES_SIZEOF_LDOUBLE 8
#endif
#endif
#endif

#if defined(__SIZEOF_POINTER__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_POINTER__
#elif defined(DEE_PLATFORM_64_BIT)
# define DEE_TYPES_SIZEOF_POINTER 8
#else
# define DEE_TYPES_SIZEOF_POINTER 4
#endif

#if defined(__CHAR_SIGNED__) || defined(_CHAR_SIGNED)
#define DEE_TYPES_CHAR_SIGNED 1
#elif defined(__CHAR_UNSIGNED__) || defined(_CHAR_UNSIGNED)
#define DEE_TYPES_CHAR_SIGNED 0
#elif '\xFF' != 0xFF
#define DEE_TYPES_CHAR_SIGNED 1
#else
#define DEE_TYPES_CHAR_SIGNED 0
#endif

#if defined(__WCHAR_SIGNED__) || defined(__WCHAR_T_SIGNED__) || \
    defined(_WCHAR_SIGNED) || defined(_WCHAR_T_SIGNED)
#define DEE_TYPES_WCHAR_T_SIGNED 1
#elif defined(__WCHAR_UNSIGNED__) || defined(__WCHAR_T_UNSIGNED__) || \
      defined(_WCHAR_UNSIGNED) || defined(_WCHAR_T_UNSIGNED)
#define DEE_TYPES_WCHAR_T_SIGNED 0
#elif DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#if L'\xFFFF' != 0xFFFF
#define DEE_TYPES_WCHAR_T_SIGNED 1
#else
#define DEE_TYPES_WCHAR_T_SIGNED 0
#endif
#else
#define DEE_TYPES_WCHAR_T_SIGNED 0
#endif

#if defined(__SIZEOF_INT_FAST8__)
#define DEE_TYPES_SIZEOF_INT_FAST8_T __SIZEOF_INT_FAST8__
#else
#define DEE_TYPES_SIZEOF_INT_FAST8_T 1
#endif
#if defined(__SIZEOF_INT_FAST16__)
#define DEE_TYPES_SIZEOF_INT_FAST16_T  __SIZEOF_INT_FAST16__
#elif defined(_MSC_VER) && defined(DEE_PLATFORM_WINDOWS)
#define DEE_TYPES_SIZEOF_INT_FAST16_T  4
#else
#define DEE_TYPES_SIZEOF_INT_FAST16_T  2
#endif
#if defined(__SIZEOF_INT_FAST32__)
#define DEE_TYPES_SIZEOF_INT_FAST32_T  __SIZEOF_INT_FAST32__
#else
#define DEE_TYPES_SIZEOF_INT_FAST32_T  4
#endif
#if defined(__SIZEOF_INT_FAST64__)
#define DEE_TYPES_SIZEOF_INT_FAST64_T  __SIZEOF_INT_FAST64__
#else
#define DEE_TYPES_SIZEOF_INT_FAST64_T  8
#endif
#if defined(__SIZEOF_INT_LEAST8__)
#define DEE_TYPES_SIZEOF_INT_LEAST8_T  __SIZEOF_INT_LEAST8__
#else
#define DEE_TYPES_SIZEOF_INT_LEAST8_T  1
#endif
#if defined(__SIZEOF_INT_LEAST16__)
#define DEE_TYPES_SIZEOF_INT_LEAST16_T __SIZEOF_INT_LEAST16__
#else
#define DEE_TYPES_SIZEOF_INT_LEAST16_T 2
#endif
#if defined(__SIZEOF_INT_LEAST32__)
#define DEE_TYPES_SIZEOF_INT_LEAST32_T __SIZEOF_INT_LEAST32__
#else
#define DEE_TYPES_SIZEOF_INT_LEAST32_T 4
#endif
#if defined(__SIZEOF_INT_LEAST64__)
#define DEE_TYPES_SIZEOF_INT_LEAST64_T __SIZEOF_INT_LEAST64__
#else
#define DEE_TYPES_SIZEOF_INT_LEAST64_T 8
#endif
#define DEE_TYPES_SIZEOF_PTRDIFF_T    DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_SIZE_T       DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_INTPTR_T     DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_SSIZE_T      DEE_TYPES_SIZEOF_POINTER
#if defined(__SIZEOF_WCHAR__)
#define DEE_TYPES_SIZEOF_WCHAR_T   __SIZEOF_WCHAR__
#elif defined(DEE_PLATFORM_WINDOWS)
#define DEE_TYPES_SIZEOF_WCHAR_T   2
#else
#define DEE_TYPES_SIZEOF_WCHAR_T   4
#endif
#if defined(__SIZEOF_CHAR16__)
#define DEE_TYPES_SIZEOF_CHAR16_T  __SIZEOF_CHAR16__
#else
#define DEE_TYPES_SIZEOF_CHAR16_T  2
#endif
#if defined(__SIZEOF_CHAR32__)
#define DEE_TYPES_SIZEOF_CHAR32_T  __SIZEOF_CHAR32__
#else
#define DEE_TYPES_SIZEOF_CHAR32_T  4
#endif
#define DEE_TYPES_CHAR16_T_SIGNED  0
#define DEE_TYPES_CHAR32_T_SIGNED  0

#ifndef DEE_TYPES_SIZEOF_TIME_T
#if defined(__SIZEOF_TIME_T__)
#define DEE_TYPES_SIZEOF_TIME_T    __SIZEOF_TIME_T__
#elif defined(_USE_32BIT_TIME_T)
#define DEE_TYPES_SIZEOF_TIME_T    4
#elif defined(_MSC_VER) && _MSC_VER < 1400
#define DEE_TYPES_SIZEOF_TIME_T    4
#else
#define DEE_TYPES_SIZEOF_TIME_T    8
#endif
#endif

#ifndef DEE_TYPES_TIME_T_SIGNED
#if defined(_MSC_VER)
#define DEE_TYPES_TIME_T_SIGNED    1
#else
#define DEE_TYPES_TIME_T_SIGNED    0
#endif
#endif

#define DEE_TYPES_SIZEOF_THREADID  4

// deemon requires this type to be at least 64 bits
// >> todo: If the compiler supports 128 bit integers, this probably has to be '16'
#define DEE_TYPES_SIZEOF_INTMAX_T  8
#define DEE_TYPES_SIZEOF_UINTMAX_T DEE_TYPES_SIZEOF_INTMAX_T

#ifdef DEE_PLATFORM_UNIX
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
#include <debug_new_disable.inl>
#endif
#include <features.h>
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
#include <debug_new_enable.inl>
#endif
#endif
#define DEE_TYPES_SIZEOF_UID_T      4
#define DEE_TYPES_SIZEOF_GID_T      4
#define DEE_TYPES_SIZEOF_MODE_T     4
#define DEE_TYPES_SIZEOF_PID_T      4
#define DEE_TYPES_SIZEOF_SOCKLEN_T  4
#define DEE_TYPES_SIZEOF_USECONDS_T 4
#ifdef __USE_FILE_OFFSET64
#define DEE_TYPES_SIZEOF_OFF_T      8
#else
#define DEE_TYPES_SIZEOF_OFF_T      4
#endif
#define DEE_TYPES_SIZEOF_OFF64_T    8
#endif

#ifdef DEE_PLATFORM_WINDOWS
#define DEE_TYPES_SIZEOF_DEE_PROCESS_ID     4
#define DEE_TYPES_SIZEOF_DEE_PROCESS_RETURN 4
#define DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE DEE_TYPES_SIZEOF_POINTER
#elif defined(DEE_PLATFORM_UNIX)
#define DEE_TYPES_SIZEOF_DEE_PROCESS_ID     DEE_TYPES_SIZEOF_PID_T
#define DEE_TYPES_SIZEOF_DEE_PROCESS_RETURN 4
#define DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE DEE_TYPES_SIZEOF_PID_T
#endif

#if DEE_TYPES_SIZEOF_CHAR == 1
#define DEE_PRIVATE_TYPES_INT_1  signed char
#define DEE_PRIVATE_TYPES_UINT_1 unsigned char
#elif DEE_TYPES_SIZEOF_SHORT == 1
#define DEE_PRIVATE_TYPES_INT_1  short
#define DEE_PRIVATE_TYPES_UINT_1 unsigned short
#elif DEE_TYPES_SIZEOF_INT == 1
#define DEE_PRIVATE_TYPES_INT_1  int
#define DEE_PRIVATE_TYPES_UINT_1 unsigned int
#elif DEE_TYPES_SIZEOF_LONG == 1
#define DEE_PRIVATE_TYPES_INT_1  long
#define DEE_PRIVATE_TYPES_UINT_1 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 1
#define DEE_PRIVATE_TYPES_INT_1  long long
#define DEE_PRIVATE_TYPES_UINT_1 unsigned long long
#else
#error No type suitable for Dee_int8_t / Dee_uint8_t found
#endif
#if DEE_TYPES_SIZEOF_CHAR == 2
#define DEE_PRIVATE_TYPES_INT_2  signed char
#define DEE_PRIVATE_TYPES_UINT_2 unsigned char
#elif DEE_TYPES_SIZEOF_SHORT == 2
#define DEE_PRIVATE_TYPES_INT_2  short
#define DEE_PRIVATE_TYPES_UINT_2 unsigned short
#elif DEE_TYPES_SIZEOF_INT == 2
#define DEE_PRIVATE_TYPES_INT_2  int
#define DEE_PRIVATE_TYPES_UINT_2 unsigned int
#elif DEE_TYPES_SIZEOF_LONG == 2
#define DEE_PRIVATE_TYPES_INT_2  long
#define DEE_PRIVATE_TYPES_UINT_2 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 2
#define DEE_PRIVATE_TYPES_INT_2  long long
#define DEE_PRIVATE_TYPES_UINT_2 unsigned long long
#else
#error No type suitable for Dee_int16_t / Dee_uint16_t found
#endif
#if DEE_TYPES_SIZEOF_CHAR == 4
#define DEE_PRIVATE_TYPES_INT_4  signed char
#define DEE_PRIVATE_TYPES_UINT_4 unsigned char
#elif DEE_TYPES_SIZEOF_SHORT == 4
#define DEE_PRIVATE_TYPES_INT_4  short
#define DEE_PRIVATE_TYPES_UINT_4 unsigned short
#elif DEE_TYPES_SIZEOF_INT == 4
#define DEE_PRIVATE_TYPES_INT_4  int
#define DEE_PRIVATE_TYPES_UINT_4 unsigned int
#elif DEE_TYPES_SIZEOF_LONG == 4
#define DEE_PRIVATE_TYPES_INT_4  long
#define DEE_PRIVATE_TYPES_UINT_4 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 4
#define DEE_PRIVATE_TYPES_INT_4  long long
#define DEE_PRIVATE_TYPES_UINT_4 unsigned long long
#else
#error No type suitable for Dee_int32_t / Dee_uint32_t found
#endif
#if DEE_TYPES_SIZEOF_CHAR == 8
#define DEE_PRIVATE_TYPES_INT_8  signed char
#define DEE_PRIVATE_TYPES_UINT_8 unsigned char
#elif DEE_TYPES_SIZEOF_SHORT == 8
#define DEE_PRIVATE_TYPES_INT_8  short
#define DEE_PRIVATE_TYPES_UINT_8 unsigned short
#elif DEE_TYPES_SIZEOF_INT == 8
#define DEE_PRIVATE_TYPES_INT_8  int
#define DEE_PRIVATE_TYPES_UINT_8 unsigned int
#elif DEE_TYPES_SIZEOF_LONG == 8
#define DEE_PRIVATE_TYPES_INT_8  long
#define DEE_PRIVATE_TYPES_UINT_8 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
#define DEE_PRIVATE_TYPES_INT_8  long long
#define DEE_PRIVATE_TYPES_UINT_8 unsigned long long
#else
#error No type suitable for Dee_int64_t / Dee_uint64_t found
#endif

#define DEE_PRIVATE_TYPES_XINT2_0(sizeof) DEE_PRIVATE_TYPES_UINT_##sizeof
#define DEE_PRIVATE_TYPES_XINT2_1(sizeof) DEE_PRIVATE_TYPES_INT_##sizeof
#define DEE_PRIVATE_TYPES_XINT_0(sizeof) DEE_PRIVATE_TYPES_XINT2_0(sizeof)
#define DEE_PRIVATE_TYPES_XINT_1(sizeof) DEE_PRIVATE_TYPES_XINT2_1(sizeof)

#define DEE_TYPES_INT8_T   DEE_PRIVATE_TYPES_INT_1
#define DEE_TYPES_INT16_T  DEE_PRIVATE_TYPES_INT_2
#define DEE_TYPES_INT32_T  DEE_PRIVATE_TYPES_INT_4
#define DEE_TYPES_INT64_T  DEE_PRIVATE_TYPES_INT_8
#define DEE_TYPES_UINT8_T  DEE_PRIVATE_TYPES_UINT_1
#define DEE_TYPES_UINT16_T DEE_PRIVATE_TYPES_UINT_2
#define DEE_TYPES_UINT32_T DEE_PRIVATE_TYPES_UINT_4
#define DEE_TYPES_UINT64_T DEE_PRIVATE_TYPES_UINT_8

#ifdef DEE_TYPES_SIZEOF_FLOAT
#define DEE_TYPES_FLOATID_FLOAT   1
#define DEE_PRIVATE_TYPES_FLOAT_1 float
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
#define DEE_TYPES_FLOATID_DOUBLE  2
#define DEE_PRIVATE_TYPES_FLOAT_2 double
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
#define DEE_TYPES_FLOATID_LDOUBLE 3
#define DEE_PRIVATE_TYPES_FLOAT_3 long double
#endif

#define DEE_PRIVATE_TYPES_INT(sizeof)         DEE_PRIVATE_TYPES_INT_##sizeof
#define DEE_PRIVATE_TYPES_UINT(sizeof)        DEE_PRIVATE_TYPES_UINT_##sizeof
#define DEE_PRIVATE_TYPES_XINT(signed,sizeof) DEE_PRIVATE_TYPES_XINT_##signed(sizeof)
#define DEE_PRIVATE_TYPES_FLOAT(id)           DEE_PRIVATE_TYPES_FLOAT_##id
#define DEE_TYPES_INT(sizeof)         DEE_PRIVATE_TYPES_INT(sizeof)
#define DEE_TYPES_UINT(sizeof)        DEE_PRIVATE_TYPES_UINT(sizeof)
#define DEE_TYPES_XINT(signed,sizeof) DEE_PRIVATE_TYPES_XINT(signed,sizeof)
#define DEE_TYPES_FLOAT(id)           DEE_PRIVATE_TYPES_FLOAT(id)


#define DEE_PRIVATES_TYPES_IPRINTF_1  "%I8d"
#define DEE_PRIVATES_TYPES_IUPRINTF_1 "%I8u"
#define DEE_PRIVATES_TYPES_IPRINTF_2  "%I16d"
#define DEE_PRIVATES_TYPES_IUPRINTF_2 "%I16u"
#define DEE_PRIVATES_TYPES_IPRINTF_4  "%I32d"
#define DEE_PRIVATES_TYPES_IUPRINTF_4 "%I32u"
#define DEE_PRIVATES_TYPES_IPRINTF_8  "%I64d"
#define DEE_PRIVATES_TYPES_IUPRINTF_8 "%I64u"

//////////////////////////////////////////////////////////////////////////
// Some optimizations, to get the printf string shorter
#if DEE_TYPES_SIZEOF_SIZE_T == 1
#undef DEE_PRIVATES_TYPES_IPRINTF_1
#undef DEE_PRIVATES_TYPES_IUPRINTF_1
#define DEE_PRIVATES_TYPES_IPRINTF_1  "%Id"
#define DEE_PRIVATES_TYPES_IUPRINTF_1 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 2
#undef DEE_PRIVATES_TYPES_IPRINTF_2
#undef DEE_PRIVATES_TYPES_IUPRINTF_2
#define DEE_PRIVATES_TYPES_IPRINTF_2  "%Id"
#define DEE_PRIVATES_TYPES_IUPRINTF_2 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 4
#undef DEE_PRIVATES_TYPES_IPRINTF_4
#undef DEE_PRIVATES_TYPES_IUPRINTF_4
#define DEE_PRIVATES_TYPES_IPRINTF_4  "%Id"
#define DEE_PRIVATES_TYPES_IUPRINTF_4 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 8
#undef DEE_PRIVATES_TYPES_IPRINTF_8
#undef DEE_PRIVATES_TYPES_IUPRINTF_8
#define DEE_PRIVATES_TYPES_IPRINTF_8  "%Id"
#define DEE_PRIVATES_TYPES_IUPRINTF_8 "%Iu"
#endif
#if DEE_TYPES_SIZEOF_INT == 1
#undef DEE_PRIVATES_TYPES_IPRINTF_1
#undef DEE_PRIVATES_TYPES_IUPRINTF_1
#define DEE_PRIVATES_TYPES_IPRINTF_1  "%d"
#define DEE_PRIVATES_TYPES_IUPRINTF_1 "%u"
#elif DEE_TYPES_SIZEOF_INT == 2
#undef DEE_PRIVATES_TYPES_IPRINTF_2
#undef DEE_PRIVATES_TYPES_IUPRINTF_2
#define DEE_PRIVATES_TYPES_IPRINTF_2  "%d"
#define DEE_PRIVATES_TYPES_IUPRINTF_2 "%u"
#elif DEE_TYPES_SIZEOF_INT == 4
#undef DEE_PRIVATES_TYPES_IPRINTF_4
#undef DEE_PRIVATES_TYPES_IUPRINTF_4
#define DEE_PRIVATES_TYPES_IPRINTF_4  "%d"
#define DEE_PRIVATES_TYPES_IUPRINTF_4 "%u"
#elif DEE_TYPES_SIZEOF_INT == 8
#undef DEE_PRIVATES_TYPES_IPRINTF_8
#undef DEE_PRIVATES_TYPES_IUPRINTF_8
#define DEE_PRIVATES_TYPES_IPRINTF_8  "%d"
#define DEE_PRIVATES_TYPES_IUPRINTF_8 "%u"
#endif
#define DEE_PRIVATES_TYPES_IXPRINTF_0 DEE_TYPES_IUPRINTF
#define DEE_PRIVATES_TYPES_IXPRINTF_1 DEE_TYPES_IPRINTF

//////////////////////////////////////////////////////////////////////////
// Printf helpers (NOTE: Only guarantied to work with deemon's own printf-style functions)
#define DEE_TYPES_IPRINTF(sizeof)         DEE_PRIVATE_TYPES_IPRINTF(sizeof)
#define DEE_TYPES_IUPRINTF(sizeof)        DEE_PRIVATE_TYPES_IUPRINTF(sizeof)
#define DEE_TYPES_IXPRINTF(signed,sizeof) DEE_PRIVATE_TYPES_IXPRINTF(signed,sizeof)
#define DEE_PRIVATE_TYPES_IPRINTF(sizeof)         DEE_PRIVATES_TYPES_IPRINTF_##sizeof
#define DEE_PRIVATE_TYPES_IUPRINTF(sizeof)        DEE_PRIVATES_TYPES_IUPRINTF_##sizeof
#define DEE_PRIVATE_TYPES_IXPRINTF(signed,sizeof) DEE_PRIVATES_TYPES_IXPRINTF_##signed(sizeof)


#ifndef DEE_STATIC_INLINE
#if DEE_COMPILER_HAVE_CXX_INLINE
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE inline T
#elif DEE_COMPILER_HAVE_MSVC_INLINE
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE __inline T
#elif DEE_COMPILER_HAVE_GCC_INLINE
# define DEE_STATIC_INLINE(T)  static DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE __inline__ T
#elif defined(__DEEMON__)
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED T
#else
# define DEE_STATIC_INLINE(T)  static DEE_ATTRIBUTE_UNUSED T
#endif
#endif /* !DEE_STATIC_INLINE */

#ifndef DEE_STATIC_ASSERT
#if DEE_COMPILER_HAVE_CXX11_STATIC_ASSERT
# define DEE_STATIC_ASSERT(x) static_assert((x),#x)
#elif DEE_COMPILER_HAVE_MSVC_COUNTER
# define DEE_STATIC_ASSERT(x) typedef int DEE_PP_CAT_2(_dee_static_assert_,__COUNTER__)[!!(x)?1:0]
#else
# define DEE_STATIC_ASSERT(x) typedef int DEE_PP_CAT_2(_dee_static_assert_,__LINE__)[!!(x)?1:0]
#endif
#endif /* !DEE_STATIC_ASSERT */

#ifndef DEE_STRUCT_DEF
#if DEE_COMPILER_HAVE_C_STRUCT_NAMESPACE
# define DEE_STRUCT_DEF(T) struct T; typedef struct T T
#else
# define DEE_STRUCT_DEF(T) struct T
#endif
#endif

#ifndef DEE_UNUSED
#if DEE_COMPILER_HAVE_CXX_UNNAMED_ARGUMENTS
# define DEE_UNUSED(x) /* nothing */
#else
# define DEE_UNUSED(x) x
#ifdef _MSC_VER
# pragma warning(disable: 4100) /* This is just getting too annoying... */
#endif
#endif
#endif /* !DEE_UNUSED */


#ifndef DEE_PLATFORM_VA_LIST_IS_ARRAY
// formerly: DEE_COMPILER_HAVE_VA_LIST_IS_ARRAY
#if defined(DEE_PLATFORM_UNIX) && defined(DEE_PLATFORM_64_BIT)
# define DEE_PLATFORM_VA_LIST_IS_ARRAY 1
#else
# define DEE_PLATFORM_VA_LIST_IS_ARRAY 0
#endif
#endif


#ifndef DEE_INT8_C
#if defined(__INT8_C)
# define DEE_INT8_C    __INT8_C
#elif defined(INT8_C)
# define DEE_INT8_C    INT8_C
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT8_C(x) x##i8
#else
# define DEE_INT8_C(x) x
#endif
#endif

#ifndef DEE_UINT8_C
#if defined(__UINT8_C)
# define DEE_UINT8_C    __UINT8_C
#elif defined(UINT8_C)
# define DEE_UINT8_C    UINT8_C
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT8_C(x) x##ui8
#else
# define DEE_UINT8_C(x) x
#endif
#endif

#ifndef DEE_INT16_C
#if defined(__INT16_C)
# define DEE_INT16_C    __INT16_C
#elif defined(INT16_C)
# define DEE_INT16_C    INT16_C
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT16_C(x) x##i16
#else
# define DEE_INT16_C(x) x
#endif
#endif

#ifndef DEE_UINT16_C
#if defined(__UINT16_C)
# define DEE_UINT16_C    __UINT16_C
#elif defined(UINT16_C)
# define DEE_UINT16_C    UINT16_C
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT16_C(x) x##ui16
#else
# define DEE_UINT16_C(x) x
#endif
#endif

#ifndef DEE_INT32_C
#if defined(__INT32_C)
# define DEE_INT32_C    __INT32_C
#elif defined(INT32_C)
# define DEE_INT32_C    INT32_C
#elif DEE_TYPES_SIZEOF_INT == 4
# define DEE_INT32_C(x) x
#elif DEE_TYPES_SIZEOF_LONG == 4
# define DEE_INT32_C(x) x##l
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT32_C(x) x##i32
#else
# define DEE_INT32_C(x) x
#endif
#endif

#ifndef DEE_UINT32_C
#if defined(__UINT32_C)
# define DEE_UINT32_C    __UINT32_C
#elif defined(UINT32_C)
# define DEE_UINT32_C    UINT32_C
#elif DEE_TYPES_SIZEOF_INT == 4
# define DEE_UINT32_C(x) x##u
#elif DEE_TYPES_SIZEOF_LONG == 4
# define DEE_UINT32_C(x) x##ul
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT32_C(x) x##ui32
#else
# define DEE_UINT32_C(x) x
#endif
#endif

#ifndef DEE_INT64_C
#if defined(__INT64_C)
# define DEE_INT64_C    __INT64_C
#elif defined(INT64_C)
# define DEE_INT64_C    INT64_C
#elif DEE_TYPES_SIZEOF_LONG == 8
# define DEE_INT64_C(x) x##l
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
# define DEE_INT64_C(x) x##ll
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT64_C(x) x##i64
#else
# define DEE_INT64_C(x) x
#endif
#endif

#ifndef DEE_UINT64_C
#if defined(__UINT64_C)
# define DEE_UINT64_C    __UINT64_C
#elif defined(UINT64_C)
# define DEE_UINT64_C    UINT64_C
#elif DEE_TYPES_SIZEOF_LONG == 8
# define DEE_UINT64_C(x) x##ul
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
# define DEE_UINT64_C(x) x##ull
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT64_C(x) x##ui64
#else
# define DEE_UINT64_C(x) x
#endif
#endif

#ifndef Dee_MIN
# define Dee_MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef Dee_MAX
# define Dee_MAX(a,b) ((a)<(b)?(b):(a))
#endif

#ifndef DEE_BUILTIN_BREAKPOINT
#if defined(_MSC_VER)
#define DEE_BUILTIN_BREAKPOINT __debugbreak
#ifdef __cplusplus
extern "C" { void __cdecl __debugbreak(void); }
#else
             void __cdecl __debugbreak(void);
#endif
#elif __has_builtin(__builtin_breakpoint)
#define DEE_BUILTIN_BREAKPOINT __builtin_breakpoint
#endif
#endif /* !DEE_BUILTIN_BREAKPOINT */



#ifdef DEE_PLATFORM_WINDOWS
#define DEE_TYPES_SIZEOF_UID_T  DEE_TYPES_SIZEOF_POINTER // sizeof(PSID)
#define DEE_TYPES_SIZEOF_GID_T  DEE_TYPES_SIZEOF_POINTER // sizeof(PSID)
#define DEE_TYPES_SIZEOF_MODE_T 4
#endif


#if defined(__INTELLISENSE__) && \
    defined(DEE_PLATFORM_WINDOWS) && \
    defined(DEE_LIMITED_API)
// Just for syntax highlighting
#define DEE_TYPES_SIZEOF_UID_T  4
#define DEE_TYPES_SIZEOF_GID_T  4
#define DEE_TYPES_SIZEOF_MODE_T 4
#define DEE_TYPES_SIZEOF_PID_T  4
typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_UID_T)  uid_t;
typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_GID_T)  gid_t;
typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_MODE_T) mode_t;
typedef DEE_TYPES_INT(DEE_TYPES_SIZEOF_PID_T)   pid_t;
#endif


#if defined(_MSC_VER)
#if defined(DEE_LIMITED_API) || defined(DEE_EXTENSION)
#undef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#undef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS 1
// NOTE: These warnings are only disabled in deemon source files
#pragma warning(disable: 4127) // Conditional expression is constant ('do{ ... }while(0)')
#pragma warning(disable: 4711) // Function selected for automatic inline expansion
#pragma warning(disable: 4514) // Unreferenced inline function has been removed
#pragma warning(disable: 4710) // Function not inlined (... Who cares?!)
#pragma warning(disable: 4738) // Storing 32-bit float result in memory, possible loss of performance
#pragma warning(disable: 4505) // Unreferenced local function has been removed
#ifdef __INTELLISENSE__
#ifdef __cplusplus
#undef NULL
// make 'return NULL' a highlighted error if the return type isn't a pointer
// (You wouldn't believe how often I've already had that happen)
// NOTE: Don't worry: Only intellisense for deemon source files sees this.
//       So this won't interfere with your own override for NULL. :)
#define NULL nullptr
#endif
#endif
#endif /* DEE_LIMITED_API || DEE_EXTENSION */
#endif

#endif /* !GUARD_DEEMON___FEATURES_INL */
