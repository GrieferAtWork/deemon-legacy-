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

#ifdef __DEEMON__
#include <__cdefault.dee>
#endif

//////////////////////////////////////////////////////////////////////////
// Preprocessor helper macros
#ifndef DEE_PP_CAT_2
# define DEE_PRIVATE_PP_CAT_2(a,b)       a##b
# define DEE_PRIVATE_PP_CAT_3(a,b,c)     a##b##c
# define DEE_PRIVATE_PP_CAT_4(a,b,c,d)   a##b##c##d
# define DEE_PRIVATE_PP_CAT_5(a,b,c,d,e) a##b##c##d##e
# define DEE_PP_CAT_2(a,b)       DEE_PRIVATE_PP_CAT_2(a,b)
# define DEE_PP_CAT_3(a,b,c)     DEE_PRIVATE_PP_CAT_3(a,b,c)
# define DEE_PP_CAT_4(a,b,c,d)   DEE_PRIVATE_PP_CAT_4(a,b,c,d)
# define DEE_PP_CAT_5(a,b,c,d,e) DEE_PRIVATE_PP_CAT_5(a,b,c,d,e)
#endif

#ifndef DEE_PP_STR
# define DEE_PRIVATE_PP_STR(x) #x
# define DEE_PP_STR(x) DEE_PRIVATE_PP_STR(x)
#endif

#ifndef DEE_PP_MUL8
#ifdef __TPP_EVAL
# define DEE_PP_MUL8(x) __TPP_EVAL(x*8)
#else
# define DEE_PRIVATE_PP_MUL8_1   8
# define DEE_PRIVATE_PP_MUL8_2   16
# define DEE_PRIVATE_PP_MUL8_4   32
# define DEE_PRIVATE_PP_MUL8_8   64
# define DEE_PRIVATE_PP_MUL8_16  128
# define DEE_PRIVATE_PP_MUL8_32  256
# define DEE_PRIVATE_PP_MUL8_64  512
# define DEE_PRIVATE_PP_MUL8_128 1024
# define DEE_PRIVATE_PP_MUL8(x) DEE_PRIVATE_PP_MUL8_##x
# define DEE_PP_MUL8(x) DEE_PRIVATE_PP_MUL8(x)
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Compiler specific features
#ifndef DEE_COMPILER_HAVE_MSVC_SEH
#if defined(_MSC_VER)
# define DEE_COMPILER_HAVE_MSVC_SEH 1
#else /* "__try" */
# define DEE_COMPILER_HAVE_MSVC_SEH 0
#endif
#endif


#ifndef DEE_PRIVATE_CODEGEAR_0X_SUPPORT
#if defined(__BORLANDC__) && __BORLANDC__ >= 0x599
#pragma defineonoption DEE_PRIVATE_CODEGEAR_0X_SUPPORT -Ax
#endif
#endif

#ifndef DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#if defined(_MSC_VER) || (defined(__DEEMON__) && __DEEMON__ >= 101)
# define DEE_COMPILER_HAVE_WIDESTRING_PREFIX 1
#else /* L"..." / L'?' */
# define DEE_COMPILER_HAVE_WIDESTRING_PREFIX 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_UTF16STRING_PREFIX
#if defined(__DEEMON__) && __DEEMON__ >= 101
# define DEE_COMPILER_HAVE_UTF16STRING_PREFIX 1
#else /* u"..." / u'?' */
# define DEE_COMPILER_HAVE_UTF16STRING_PREFIX 0
#endif
#endif

#ifndef DEE_COMPILER_HAVE_UTF32STRING_PREFIX
#if defined(__DEEMON__) && __DEEMON__ >= 101
# define DEE_COMPILER_HAVE_UTF32STRING_PREFIX 1
#else /* U"..." / U'?' */
# define DEE_COMPILER_HAVE_UTF32STRING_PREFIX 0
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
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */

#ifndef DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
#if __has_extension(__tpp_msvc_integer_suffix__) || \
   (defined(_MSC_VER) && !defined(__ANDROID__))
# define DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX 1
#else
# define DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX 0
#endif
#endif /* !DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX */

#ifndef DEE_COMPILER_HAVE_UNNAMED_UNION
#if (defined(__DEEMON__)\
 || (defined(_MSC_VER) /* && Apparently supported on all versions? */))\
 || (defined(__STDC__) && __STDC__ >= 201103L && !defined(__cplusplus))\
 || (defined(__GNUC__) && (__GNUC__ >= 2 && (__GNUC__ != 2 ||\
    (__GNUC_MINOR__ >= 95 && (__GNUC_MINOR__ != 95 || defined(__cplusplus))))))\
 && (!defined(__SUNPRO_C) && !defined(__SUNPRO_CC))
# define DEE_COMPILER_HAVE_UNNAMED_UNION  1
#else
# define DEE_COMPILER_HAVE_UNNAMED_UNION  0
#endif
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
#ifndef DEE_COMPILER_HAVE_UNNAMED_STRUCT
#if (defined(__DEEMON__)\
 || (defined(_MSC_VER) /* && Apparently supported on all versions? */))\
 || (defined(__GNUC__) && (__GNUC__ >= 2 && (__GNUC__ != 2 || (!defined(__cplusplus) && __GNUC_MINOR__ >= 96))))\
 && (!defined(__SUNPRO_C) && !defined(__SUNPRO_CC))
# define DEE_COMPILER_HAVE_UNNAMED_STRUCT 1
#else
# define DEE_COMPILER_HAVE_UNNAMED_STRUCT 0
#endif
#endif /* !DEE_COMPILER_HAVE_UNNAMED_STRUCT */

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
# include <debug_new_disable.inl>
# include "AvailabilityMacros.h"
# include "TargetConditionals.h"
#else
# include "AvailabilityMacros.h"
# include "TargetConditionals.h"
# include <debug_new_enable.inl>
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
    defined(__MINGW64__) || defined(__CYGWIN__) ||\
    defined(__CYGWIN32__)
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
#ifdef _MSC_VER
# define DEE_COMPILER_PRAGMA    __pragma
#else
# define DEE_COMPILER_PRAGMA(x) _Pragma(#x)
#endif
#endif /* !DEE_COMPILER_PRAGMA */

#ifndef DEE_COMPILER_PACK_PUSH
#if defined(_MSC_VER) || defined(__GNUC__) || \
    defined(__TINYC__) || defined(__DEEMON__)
# define DEE_COMPILER_PACK_PUSH(packed) DEE_COMPILER_PRAGMA(pack(push,packed))
# define DEE_COMPILER_PACK_POP          DEE_COMPILER_PRAGMA(pack(pop))
#else
# define DEE_COMPILER_PACK_PUSH(packed) /* nothing */
# define DEE_COMPILER_PACK_POP          /* nothing */
#endif
#endif /* !DEE_COMPILER_PACK_PUSH */

#ifndef DEE_COMPILER_MSVC_WARNING_PUSH
#ifdef _MSC_VER
# define DEE_COMPILER_MSVC_WARNING_PUSH(w) __pragma(warning(push)) __pragma(warning(disable: w))
# define DEE_COMPILER_MSVC_WARNING_POP     __pragma(warning(pop))
#else
# define DEE_COMPILER_MSVC_WARNING_PUSH(w) /* nothing */
# define DEE_COMPILER_MSVC_WARNING_POP     /* nothing */
#endif
#endif /* !DEE_COMPILER_MSVC_WARNING_PUSH */

#ifndef DEE_COMPILER_PREFAST_WARNING_PUSH
#ifdef _PREFAST_
# define DEE_COMPILER_PREFAST_WARNING_PUSH    DEE_COMPILER_MSVC_WARNING_PUSH
# define DEE_COMPILER_PREFAST_WARNING_POP     DEE_COMPILER_MSVC_WARNING_POP
#else
# define DEE_COMPILER_PREFAST_WARNING_PUSH(w) /* nothing */
# define DEE_COMPILER_PREFAST_WARNING_POP     /* nothing */
#endif
#endif /* !DEE_COMPILER_PREFAST_WARNING_PUSH */

#ifndef DEE_ATTRIBUTE_ALWAYS_INLINE
#if __has_attribute(always_inline) || \
   (defined(__GNUC__) && __GNUC__*100+__GNUC_MINOR__ >= 301)
# define DEE_ATTRIBUTE_ALWAYS_INLINE __attribute__((__always_inline__))
#else
# define DEE_ATTRIBUTE_ALWAYS_INLINE /* nothing */
#endif
#endif

#if __has_attribute(malloc) || \
   (defined(__GNUC__) && __GNUC__ >= 3)
# define DEE_ATTRIBUTE_MALLOC __attribute__((__malloc__))
#elif __has_declspec_attribute(restrict) || \
     (defined(_MSC_VER) && _MSC_VER >= 1400)
# define DEE_ATTRIBUTE_MALLOC __declspec(restrict)
#else
# define DEE_ATTRIBUTE_MALLOC /* nothing */
#endif

#ifndef DEE_ATTRIBUTE_DLLIMPORT
#if defined(DEE_PLATFORM_WINDOWS) || defined(__CYGWIN__)
#if __has_declspec_attribute(dllimport) || defined(_MSC_VER)
# define DEE_ATTRIBUTE_DLLIMPORT __declspec(dllimport)
# define DEE_ATTRIBUTE_DLLEXPORT __declspec(dllexport)
#elif __has_attribute(dllimport) || defined(__TINYC__) || \
     (defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__) >= 205)
# define DEE_ATTRIBUTE_DLLIMPORT __attribute__((__dllimport__))
# define DEE_ATTRIBUTE_DLLEXPORT __attribute__((__dllexport__))
#endif
#else
#if __has_attribute(visibility) || \
   (defined(__GNUC__) && __GNUC__ >= 4)
# define DEE_ATTRIBUTE_DLLIMPORT __attribute__((__visibility__("default")))
# define DEE_ATTRIBUTE_DLLEXPORT __attribute__((__visibility__("default")))
#endif
#endif
#ifndef DEE_ATTRIBUTE_DLLIMPORT
# define DEE_ATTRIBUTE_DLLIMPORT /* nothing */
# define DEE_ATTRIBUTE_DLLEXPORT /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_DLLIMPORT */

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
#endif /* !DEE_ATTRIBUTE_NORETURN */

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
#endif /* !DEE_ATTRIBUTE_NOINLINE */


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
#endif /* !DEE_ATTRIBUTE_CONST */

#ifndef DEE_ATTRIBUTE_NONNULL
#if __has_attribute(nonnull) || \
   (defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)))
# define DEE_ATTRIBUTE_NONNULL(ids) __attribute__((__nonnull__ ids))
#elif defined(__nonnull)
# define DEE_ATTRIBUTE_NONNULL      __nonnull
#elif defined(__INTELLISENSE__) || (defined(_MSC_VER) && 1)
// enforce usage of double parenthesis
# define DEE_PRIVATE_ATTRIBUTE_NONNULL(...) /* nothing */
# define DEE_ATTRIBUTE_NONNULL(ids) DEE_PRIVATE_ATTRIBUTE_NONNULL ids
#else
# define DEE_ATTRIBUTE_NONNULL(ids) /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_NONNULL */

#ifndef DEE_ATTRIBUTE_FALLTHROUGH
#if __has_cpp_attribute(fallthrough)
# define DEE_ATTRIBUTE_FALLTHROUGH [[fallthrough]];
#else
# define DEE_ATTRIBUTE_FALLTHROUGH /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_FALLTHROUGH */

#ifndef DEE_ATTRIBUTE_SENTINAL
#if __has_attribute(sentinel) || \
   (defined(__GNUC__)  && __GNUC__ >= 4)
# define DEE_ATTRIBUTE_SENTINAL __attribute__((__sentinel__))
#else
# define DEE_ATTRIBUTE_SENTINAL /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_SENTINAL */

#ifndef DEE_ATTRIBUTE_UNUSED
#if __has_attribute(unused) || \
   (defined(__GNUC__)  && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)))
# define DEE_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
# define DEE_ATTRIBUTE_UNUSED /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_UNUSED */

#ifndef DEE_ATTRIBUTE_CDECL
#if !defined(DEE_PLATFORM_UNIX) ||\
    !defined(DEE_PLATFORM_64_BIT)
#if defined(_MSC_VER) || defined(__DEEMON__)
# define DEE_ATTRIBUTE_CDECL __cdecl
#elif defined(__GNUC__) || __has_attribute(__cdecl__)
# define DEE_ATTRIBUTE_CDECL __attribute__((__cdecl__))
#elif 0
# define DEE_ATTRIBUTE_CDECL _Pragma("cdecl")
#endif
#endif /* ... */
#endif /* !DEE_ATTRIBUTE_CDECL */

// Indicates that a reference should be added to the returned deemon object
// Requires that the return type be of pointer-to struct with uuid == 'DEE_UUID_DeeObject'
#ifndef DEE_ATTRIBUTE_ADD_RESULT_REFERENCE
#if defined(__DEEMON__) && __has_attribute(add_result_reference)
# define DEE_ATTRIBUTE_ADD_RESULT_REFERENCE  __attribute__((__add_result_reference__))
#else
# define DEE_ATTRIBUTE_ADD_RESULT_REFERENCE  /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_ADD_RESULT_REFERENCE */

#ifndef DEE_ATTRIBUTE_DEPRECATED
#if __has_declspec_attribute(deprecated) || \
   (defined(_MSC_VER) && _MSC_VER >= 1200)
# define DEE_ATTRIBUTE_DEPRECATED(reason) __declspec(deprecated(reason))
#elif __has_attribute(deprecated) || \
     (defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)))
#if (defined(__GNUC__) && (__GNUC__ > 4 ||\
    (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))) || defined(__DEEMON__)
# define DEE_ATTRIBUTE_DEPRECATED(reason) __attribute__((__deprecated__(reason)))
#else
# define DEE_ATTRIBUTE_DEPRECATED(reason) __attribute__((__deprecated__))
#endif
#elif (__has_cpp_attribute(deprecated) >= 201309) || \
      (defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER >= 1900)
# define DEE_ATTRIBUTE_DEPRECATED(reason) [[deprecated(reason)]]
#else
# define DEE_ATTRIBUTE_DEPRECATED(reason) /* nothing */
#endif
#endif /* !DEE_ATTRIBUTE_DEPRECATED */

#ifndef DEE_MACRO_DEPRECATED
#if defined(_MSC_VER) || defined(__DEEMON__)
# define DEE_MACRO_DEPRECATED(new)\
  DEE_COMPILER_PRAGMA(message(__FILE__ "(" DEE_PP_STR(__LINE__) ") : Warning : Deprecated macro. Use " DEE_PP_STR(#new) " instead!")) new
#elif defined(__GNUC__)
# define DEE_MACRO_DEPRECATED(new)\
  DEE_COMPILER_PRAGMA(message __FILE__ ":" DEE_PP_STR(__LINE__) ": Warning : Deprecated macro. Use " DEE_PP_STR(#new) " instead!")) new
#else
# define DEE_MACRO_DEPRECATED(new)
#endif
#endif


#ifndef DEE_BUILTIN_BREAKPOINT
#if __has_builtin(__builtin_breakpoint)
# define DEE_BUILTIN_BREAKPOINT __builtin_breakpoint
#elif defined(DEBUG_NEW_BREAKPOINT)
# define DEE_BUILTIN_BREAKPOINT DEBUG_NEW_BREAKPOINT
#elif defined(_MSC_VER)
# define DEE_BUILTIN_BREAKPOINT __debugbreak
#ifdef __cplusplus
extern "C" { void __cdecl __debugbreak(void); }
#else
void __cdecl __debugbreak(void);
#endif
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H
# include <signal.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H */
#ifdef SIGTRAP
# define DEE_BUILTIN_BREAKPOINT() raise(SIGTRAP)
#endif
#endif
#endif /* !DEE_BUILTIN_BREAKPOINT */

#ifndef DEE_BUILTIN_UNREACHABLE_
#if __has_builtin(__builtin_unreachable) || \
   (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)))
# define DEE_BUILTIN_UNREACHABLE_   __builtin_unreachable
#elif defined(_MSC_VER)
# define DEE_BUILTIN_UNREACHABLE_() __assume(0)
#else
# define DEE_BUILTIN_UNREACHABLE_() (void)0
#endif
#endif

#ifndef DEE_COMPILER_ASSUME
#ifdef _MSC_VER
# define DEE_COMPILER_ASSUME       __assume
#elif __has_builtin(__builtin_assume)
# define DEE_COMPILER_ASSUME       __builtin_assume
#else
# define DEE_COMPILER_ASSUME(expr) (void)0
#endif
#endif

#ifndef DEE_LIKELY
#if __has_builtin(__builtin_expect) || \
  (!defined(__clang__) && defined(__GNUC__))
#if defined(__cplusplus) || defined(__DEEMON__)
# define DEE_LIKELY(x)   (__builtin_expect(!!(x),true))
# define DEE_UNLIKELY(x) (__builtin_expect(!!(x),false))
#else
# define DEE_LIKELY(x)   (__builtin_expect(!!(x),1))
# define DEE_UNLIKELY(x) (__builtin_expect(!!(x),0))
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
# define DEE_PLATFORM_LIL_ENDIAN 1
#elif DEE_PLATFORM_ENDIAN == 4321
# define DEE_PLATFORM_BIG_ENDIAN 1
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Sizes of different types
// >> These are used everywhere, so changing them
//    should fix errors from invalid type sizes
// >> 'DEE_TYPES_SIZEOF_LLONG' is optional, but all others are mandatory
#ifndef DEE_TYPES_SIZEOF_CHAR
#if defined(__SIZEOF_CHAR__)
# define DEE_TYPES_SIZEOF_CHAR  __SIZEOF_CHAR__
#elif 1
# define DEE_TYPES_SIZEOF_CHAR  1
#endif
#endif /* !DEE_TYPES_SIZEOF_CHAR */

#ifndef DEE_TYPES_SIZEOF_SHORT
#if defined(__SIZEOF_SHORT__)
# define DEE_TYPES_SIZEOF_SHORT __SIZEOF_SHORT__
#elif 1
# define DEE_TYPES_SIZEOF_SHORT 2
#endif
#endif /* !DEE_TYPES_SIZEOF_SHORT */

#ifndef DEE_TYPES_SIZEOF_INT
#if defined(__SIZEOF_INT__)
# define DEE_TYPES_SIZEOF_INT   __SIZEOF_INT__
#elif 1
# define DEE_TYPES_SIZEOF_INT   4
#endif
#endif /* !DEE_TYPES_SIZEOF_INT */

#ifndef DEE_TYPES_SIZEOF_LONG
#if defined(__SIZEOF_LONG__)
# define DEE_TYPES_SIZEOF_LONG  __SIZEOF_LONG__
#elif defined(DEE_PLATFORM_64_BIT) && \
     !defined(DEE_PLATFORM_WINDOWS)
# define DEE_TYPES_SIZEOF_LONG  8
#elif 1
# define DEE_TYPES_SIZEOF_LONG  4
#endif
#endif /* !DEE_TYPES_SIZEOF_LONG */

#ifndef DEE_TYPES_SIZEOF_LLONG
#if defined(__SIZEOF_LLONG__)
# define DEE_TYPES_SIZEOF_LLONG __SIZEOF_LLONG__
#elif defined(__SIZEOF_LONG_LONG__)
# define DEE_TYPES_SIZEOF_LLONG __SIZEOF_LONG_LONG__
#elif (defined(_MSC_VER) && (defined(_MSC_EXTENSIONS) || _MSC_VER >= 1400))\
   ||  defined(__clang__) || defined(__DEEMON__)\
   || (defined(__GNUC__) && !defined(__DARWIN_NO_LONG_LONG))\
   || (defined(__BORLANDC__) && __BORLANDC__ >= 0x561 && !defined(__NO_LONG_LONG))\
   || (defined(__SIZEOF_LLONG__) || defined(__SIZEOF_LONG_LONG__))
# define DEE_TYPES_SIZEOF_LLONG 8
#endif
#endif /* !DEE_TYPES_SIZEOF_LLONG */

#ifndef DEE_TYPES_SIZEOF_FLOAT
#if defined(__SIZEOF_FLOAT__)
# define DEE_TYPES_SIZEOF_FLOAT __SIZEOF_FLOAT__
#elif 1
# define DEE_TYPES_SIZEOF_FLOAT 4
#endif
#endif /* !DEE_TYPES_SIZEOF_FLOAT */

#ifndef DEE_TYPES_SIZEOF_DOUBLE
#if defined(__SIZEOF_DOUBLE__)
# define DEE_TYPES_SIZEOF_DOUBLE __SIZEOF_DOUBLE__
#elif 1
# define DEE_TYPES_SIZEOF_DOUBLE 8
#endif
#endif /* !DEE_TYPES_SIZEOF_DOUBLE */

#ifndef DEE_TYPES_SIZEOF_LDOUBLE
#if defined(__SIZEOF_LONG_DOUBLE__)
# define DEE_TYPES_SIZEOF_LDOUBLE __SIZEOF_LONG_DOUBLE__
#elif defined(__SIZEOF_LDOUBLE__)
# define DEE_TYPES_SIZEOF_LDOUBLE __SIZEOF_LDOUBLE__
#elif !defined(__NO_LONG_DOUBLE_MATH)
#if defined(__C67__) || defined(__i386__)
# define DEE_TYPES_SIZEOF_LDOUBLE 12
#elif defined(__x86_64__)
# define DEE_TYPES_SIZEOF_LDOUBLE 16
#elif defined(DEE_PLATFORM_ARM)
# define DEE_TYPES_SIZEOF_LDOUBLE 8
#else
# define DEE_TYPES_SIZEOF_LDOUBLE 8
#endif
#endif /* ... */
#endif /* !DEE_TYPES_SIZEOF_LDOUBLE */


#if defined(DEE_TYPES_SIZEOF_CHAR) && DEE_TYPES_SIZEOF_CHAR == 1
# define DEE_PRIVATE_TYPES_INT_1  signed char
# define DEE_PRIVATE_TYPES_UINT_1 unsigned char
#elif defined(DEE_TYPES_SIZEOF_SHORT) && DEE_TYPES_SIZEOF_SHORT == 1
# define DEE_PRIVATE_TYPES_INT_1  short
# define DEE_PRIVATE_TYPES_UINT_1 unsigned short
#elif defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 1
# define DEE_PRIVATE_TYPES_INT_1  int
# define DEE_PRIVATE_TYPES_UINT_1 unsigned int
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 1
# define DEE_PRIVATE_TYPES_INT_1  long
# define DEE_PRIVATE_TYPES_UINT_1 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 1
# define DEE_PRIVATE_TYPES_INT_1  long long
# define DEE_PRIVATE_TYPES_UINT_1 unsigned long long
#elif defined(DEE_REQUIRE_ALL_INTEGRAL_TYPES)
# error No type suitable for int8_t / uint8_t found
#endif
#if defined(DEE_TYPES_SIZEOF_SHORT) && DEE_TYPES_SIZEOF_SHORT == 2
# define DEE_PRIVATE_TYPES_INT_2  short
# define DEE_PRIVATE_TYPES_UINT_2 unsigned short
#elif defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 2
# define DEE_PRIVATE_TYPES_INT_2  int
# define DEE_PRIVATE_TYPES_UINT_2 unsigned int
#elif defined(DEE_TYPES_SIZEOF_CHAR) && DEE_TYPES_SIZEOF_CHAR == 2
# define DEE_PRIVATE_TYPES_INT_2  signed char
# define DEE_PRIVATE_TYPES_UINT_2 unsigned char
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 2
# define DEE_PRIVATE_TYPES_INT_2  long
# define DEE_PRIVATE_TYPES_UINT_2 unsigned long
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 2
# define DEE_PRIVATE_TYPES_INT_2  long long
# define DEE_PRIVATE_TYPES_UINT_2 unsigned long long
#elif defined(DEE_REQUIRE_ALL_INTEGRAL_TYPES)
# error No type suitable for int16_t / uint16_t found
#endif
#if defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 4
# define DEE_PRIVATE_TYPES_INT_4  int
# define DEE_PRIVATE_TYPES_UINT_4 unsigned int
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 4
# define DEE_PRIVATE_TYPES_INT_4  long
# define DEE_PRIVATE_TYPES_UINT_4 unsigned long
#elif defined(DEE_TYPES_SIZEOF_SHORT) && DEE_TYPES_SIZEOF_SHORT == 4
# define DEE_PRIVATE_TYPES_INT_4  short
# define DEE_PRIVATE_TYPES_UINT_4 unsigned short
#elif defined(DEE_TYPES_SIZEOF_CHAR) && DEE_TYPES_SIZEOF_CHAR == 4
# define DEE_PRIVATE_TYPES_INT_4  signed char
# define DEE_PRIVATE_TYPES_UINT_4 unsigned char
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 4
# define DEE_PRIVATE_TYPES_INT_4  long long
# define DEE_PRIVATE_TYPES_UINT_4 unsigned long long
#elif defined(DEE_REQUIRE_ALL_INTEGRAL_TYPES)
# error No type suitable for int32_t / uint32_t found
#endif
#if defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
# define DEE_PRIVATE_TYPES_INT_8  long long
# define DEE_PRIVATE_TYPES_UINT_8 unsigned long long
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 8
# define DEE_PRIVATE_TYPES_INT_8  long
# define DEE_PRIVATE_TYPES_UINT_8 unsigned long
#elif defined(DEE_TYPES_SIZEOF_CHAR) && DEE_TYPES_SIZEOF_CHAR == 8
# define DEE_PRIVATE_TYPES_INT_8  signed char
# define DEE_PRIVATE_TYPES_UINT_8 unsigned char
#elif defined(DEE_TYPES_SIZEOF_SHORT) && DEE_TYPES_SIZEOF_SHORT == 8
# define DEE_PRIVATE_TYPES_INT_8  short
# define DEE_PRIVATE_TYPES_UINT_8 unsigned short
#elif defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 8
# define DEE_PRIVATE_TYPES_INT_8  int
# define DEE_PRIVATE_TYPES_UINT_8 unsigned int
#elif defined(DEE_REQUIRE_ALL_INTEGRAL_TYPES)
# error No type suitable for int64_t / uint64_t found
#endif

#define DEE_PRIVATE_TYPES_XINT2_0(sizeof) DEE_PRIVATE_TYPES_UINT_##sizeof
#define DEE_PRIVATE_TYPES_XINT2_1(sizeof) DEE_PRIVATE_TYPES_INT_##sizeof
#define DEE_PRIVATE_TYPES_XINT_0(sizeof) DEE_PRIVATE_TYPES_XINT2_0(sizeof)
#define DEE_PRIVATE_TYPES_XINT_1(sizeof) DEE_PRIVATE_TYPES_XINT2_1(sizeof)

#ifdef DEE_PRIVATE_TYPES_UINT_1
#define DEE_PRIVATE_TYPES_HAVE_1 1
#define DEE_TYPES_INT8_T   DEE_PRIVATE_TYPES_INT_1
#define DEE_TYPES_UINT8_T  DEE_PRIVATE_TYPES_UINT_1
#else
#define DEE_PRIVATE_TYPES_HAVE_1 0
#endif
#ifdef DEE_PRIVATE_TYPES_UINT_2
#define DEE_PRIVATE_TYPES_HAVE_2 1
#define DEE_TYPES_INT16_T  DEE_PRIVATE_TYPES_INT_2
#define DEE_TYPES_UINT16_T DEE_PRIVATE_TYPES_UINT_2
#else
#define DEE_PRIVATE_TYPES_HAVE_2 0
#endif
#ifdef DEE_PRIVATE_TYPES_UINT_4
#define DEE_PRIVATE_TYPES_HAVE_4 1
#define DEE_TYPES_INT32_T  DEE_PRIVATE_TYPES_INT_4
#define DEE_TYPES_UINT32_T DEE_PRIVATE_TYPES_UINT_4
#else
#define DEE_PRIVATE_TYPES_HAVE_4 0
#endif
#ifdef DEE_PRIVATE_TYPES_UINT_8
#define DEE_PRIVATE_TYPES_HAVE_8 1
#define DEE_TYPES_INT64_T  DEE_PRIVATE_TYPES_INT_8
#define DEE_TYPES_UINT64_T DEE_PRIVATE_TYPES_UINT_8
#else
#define DEE_PRIVATE_TYPES_HAVE_8 0
#endif

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

//////////////////////////////////////////////////////////////////////////
// Type generators (Return a type matching the given type/sign-ness)
#define DEE_TYPES_INT(sizeof)         DEE_PRIVATE_TYPES_INT(sizeof)
#define DEE_TYPES_UINT(sizeof)        DEE_PRIVATE_TYPES_UINT(sizeof)
#define DEE_TYPES_XINT(signed,sizeof) DEE_PRIVATE_TYPES_XINT(signed,sizeof)
#define DEE_TYPES_HAVE(sizeof)        DEE_PRIVATE_TYPES_HAVE(sizeof)
#define DEE_TYPES_FLOAT(id)           DEE_PRIVATE_TYPES_FLOAT(id)
#define DEE_PRIVATE_TYPES_INT(sizeof)         DEE_PRIVATE_TYPES_INT_##sizeof
#define DEE_PRIVATE_TYPES_UINT(sizeof)        DEE_PRIVATE_TYPES_UINT_##sizeof
#define DEE_PRIVATE_TYPES_XINT(signed,sizeof) DEE_PRIVATE_TYPES_XINT_##signed(sizeof)
#define DEE_PRIVATE_TYPES_HAVE(sizeof)        DEE_PRIVATE_TYPES_HAVE_##sizeof
#define DEE_PRIVATE_TYPES_FLOAT(id)           DEE_PRIVATE_TYPES_FLOAT_##id

#ifndef DEE_TYPES_SIZEOF_POINTER
#if defined(__SIZEOF_POINTER__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_POINTER__
#elif defined(__SIZEOF_PTRDIFF_T__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_PTRDIFF_T__
#elif defined(__SIZEOF_INTPTR_T__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_INTPTR_T__
#elif defined(__SIZEOF_UINTPTR_T__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_UINTPTR_T__
#elif defined(__SIZEOF_SIZE_T__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_SIZE_T__
#elif defined(__SIZEOF_SSIZE_T__)
# define DEE_TYPES_SIZEOF_POINTER __SIZEOF_SSIZE_T__
#elif defined(DEE_PLATFORM_64_BIT)
# define DEE_TYPES_SIZEOF_POINTER 8
#else
# define DEE_TYPES_SIZEOF_POINTER 4
#endif
#endif

#define DEE_TYPES_SIZEOF_PTRDIFF_T DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_INTPTR_T  DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_UINTPTR_T DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_SIZE_T    DEE_TYPES_SIZEOF_POINTER
#define DEE_TYPES_SIZEOF_SSIZE_T   DEE_TYPES_SIZEOF_POINTER

#ifndef DEE_TYPES_SIZEOF_WCHAR_T
#if defined(__SIZEOF_WCHAR__)
# define DEE_TYPES_SIZEOF_WCHAR_T   __SIZEOF_WCHAR__
#elif defined(DEE_PLATFORM_WINDOWS)
# define DEE_TYPES_SIZEOF_WCHAR_T   2
#else
# define DEE_TYPES_SIZEOF_WCHAR_T   4
#endif
#if !DEE_TYPES_HAVE(DEE_TYPES_SIZEOF_WCHAR_T)
#undef DEE_TYPES_SIZEOF_WCHAR_T
#endif
#endif /* !DEE_TYPES_SIZEOF_WCHAR_T */

#ifndef DEE_TYPES_SIZEOF_CHAR16_T
#if defined(__SIZEOF_CHAR16__)
# define DEE_TYPES_SIZEOF_CHAR16_T  __SIZEOF_CHAR16__
#else
# define DEE_TYPES_SIZEOF_CHAR16_T  2
#endif
#if !DEE_TYPES_HAVE(DEE_TYPES_SIZEOF_CHAR16_T)
#undef DEE_TYPES_SIZEOF_WCHAR_T
#endif
#endif /* !DEE_TYPES_SIZEOF_CHAR16_T */
#ifndef DEE_TYPES_SIZEOF_CHAR32_T
#if defined(__SIZEOF_CHAR32__)
# define DEE_TYPES_SIZEOF_CHAR32_T  __SIZEOF_CHAR32__
#else
# define DEE_TYPES_SIZEOF_CHAR32_T  4
#endif
#if !DEE_TYPES_HAVE(DEE_TYPES_SIZEOF_CHAR32_T)
#undef DEE_TYPES_SIZEOF_WCHAR_T
#endif
#endif /* !DEE_TYPES_SIZEOF_CHAR32_T */

#ifdef DEE_TYPES_SIZEOF_CHAR16_T
#define DEE_TYPES_CHAR16_T_SIGNED  0
#endif
#ifdef DEE_TYPES_SIZEOF_CHAR32_T
#define DEE_TYPES_CHAR32_T_SIGNED  0
#endif


#ifndef DEE_TYPES_CHAR_SIGNED
#ifdef DEE_TYPES_SIZEOF_CHAR
#if defined(__CHAR_SIGNED__) || defined(_CHAR_SIGNED)
# define DEE_TYPES_CHAR_SIGNED 1
#elif defined(__CHAR_UNSIGNED__) || defined(_CHAR_UNSIGNED)
# define DEE_TYPES_CHAR_SIGNED 0
#elif '\xFF' != 0xFF
# define DEE_TYPES_CHAR_SIGNED 1
#else
# define DEE_TYPES_CHAR_SIGNED 0
#endif
#endif /* DEE_TYPES_SIZEOF_CHAR */
#endif /* !DEE_TYPES_CHAR_SIGNED */

#ifndef DEE_TYPES_WCHAR_T_SIGNED
#if defined(__WCHAR_SIGNED__) || defined(__WCHAR_T_SIGNED__) || \
    defined(_WCHAR_SIGNED) || defined(_WCHAR_T_SIGNED)
# define DEE_TYPES_WCHAR_T_SIGNED 1
#elif defined(__WCHAR_UNSIGNED__) || defined(__WCHAR_T_UNSIGNED__) || \
      defined(_WCHAR_UNSIGNED) || defined(_WCHAR_T_UNSIGNED)
# define DEE_TYPES_WCHAR_T_SIGNED 0
#elif DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#if L'\xFFFF' != 0xFFFF
# define DEE_TYPES_WCHAR_T_SIGNED 1
#else
# define DEE_TYPES_WCHAR_T_SIGNED 0
#endif
#else
# define DEE_TYPES_WCHAR_T_SIGNED 0
#endif
#endif /* DEE_TYPES_WCHAR_T_SIGNED */

#ifndef DEE_TYPES_SIZEOF_INT_FAST8_T
#if defined(__SIZEOF_INT_FAST8__)
# define DEE_TYPES_SIZEOF_INT_FAST8_T __SIZEOF_INT_FAST8__
#elif defined(DEE_TYPES_INT8_T)
# define DEE_TYPES_SIZEOF_INT_FAST8_T 1
#elif defined(DEE_TYPES_INT16_T)
# define DEE_TYPES_SIZEOF_INT_FAST8_T 2
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_FAST8_T 4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_FAST8_T 8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_FAST8_T */

#ifndef DEE_TYPES_SIZEOF_INT_FAST16_T
#if defined(__SIZEOF_INT_FAST16__)
# define DEE_TYPES_SIZEOF_INT_FAST16_T  __SIZEOF_INT_FAST16__
#elif defined(_MSC_VER) && defined(DEE_PLATFORM_WINDOWS)
# define DEE_TYPES_SIZEOF_INT_FAST16_T  4
#elif defined(DEE_TYPES_INT16_T)
# define DEE_TYPES_SIZEOF_INT_FAST16_T  2
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_FAST16_T  4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_FAST16_T  8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_FAST16_T */

#ifndef DEE_TYPES_SIZEOF_INT_FAST32_T
#if defined(__SIZEOF_INT_FAST32__)
# define DEE_TYPES_SIZEOF_INT_FAST32_T  __SIZEOF_INT_FAST32__
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_FAST32_T  4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_FAST32_T  8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_FAST32_T */

#ifndef DEE_TYPES_SIZEOF_INT_FAST64_T
#if defined(__SIZEOF_INT_FAST64__)
# define DEE_TYPES_SIZEOF_INT_FAST64_T  __SIZEOF_INT_FAST64__
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_FAST64_T  8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_FAST64_T */

#ifndef DEE_TYPES_SIZEOF_INT_LEAST8_T
#if defined(__SIZEOF_INT_LEAST8__)
# define DEE_TYPES_SIZEOF_INT_LEAST8_T  __SIZEOF_INT_LEAST8__
#elif defined(DEE_TYPES_INT8_T)
# define DEE_TYPES_SIZEOF_INT_LEAST8_T  1
#elif defined(DEE_TYPES_INT16_T)
# define DEE_TYPES_SIZEOF_INT_LEAST8_T  2
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_LEAST8_T  4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_LEAST8_T  8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_LEAST8_T */

#ifndef DEE_TYPES_SIZEOF_INT_LEAST16_T
#if defined(__SIZEOF_INT_LEAST16__)
# define DEE_TYPES_SIZEOF_INT_LEAST16_T __SIZEOF_INT_LEAST16__
#elif defined(DEE_TYPES_INT16_T)
# define DEE_TYPES_SIZEOF_INT_LEAST16_T 2
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_LEAST16_T 4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_LEAST16_T 8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_LEAST16_T */

#ifndef DEE_TYPES_SIZEOF_INT_LEAST32_T
#if defined(__SIZEOF_INT_LEAST32__)
# define DEE_TYPES_SIZEOF_INT_LEAST32_T __SIZEOF_INT_LEAST32__
#elif defined(DEE_TYPES_INT32_T)
# define DEE_TYPES_SIZEOF_INT_LEAST32_T 4
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_LEAST32_T 8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_LEAST32_T */

#ifndef DEE_TYPES_SIZEOF_INT_LEAST64_T
#if defined(__SIZEOF_INT_LEAST64__)
# define DEE_TYPES_SIZEOF_INT_LEAST64_T __SIZEOF_INT_LEAST64__
#elif defined(DEE_TYPES_INT64_T)
# define DEE_TYPES_SIZEOF_INT_LEAST64_T 8
#endif
#endif /* !DEE_TYPES_SIZEOF_INT_LEAST64_T */


#ifndef DEE_TYPES_SIZEOF_TIME_T
#if defined(__SIZEOF_TIME_T__)
# define DEE_TYPES_SIZEOF_TIME_T    __SIZEOF_TIME_T__
#elif defined(_USE_32BIT_TIME_T)
# define DEE_TYPES_SIZEOF_TIME_T    4
#elif defined(_MSC_VER) && _MSC_VER < 1400
# define DEE_TYPES_SIZEOF_TIME_T    4
#else
# define DEE_TYPES_SIZEOF_TIME_T    8
#endif
#endif

#ifndef DEE_TYPES_TIME_T_SIGNED
#if defined(_MSC_VER)
# define DEE_TYPES_TIME_T_SIGNED    1
#else
# define DEE_TYPES_TIME_T_SIGNED    0
#endif
#endif

#define DEE_TYPES_SIZEOF_THREADID  4

#ifndef DEE_TYPES_SIZEOF_INTMAX_T
#if DEE_TYPES_HAVE(8)
# define DEE_TYPES_SIZEOF_INTMAX_T  8
#elif DEE_TYPES_HAVE(4)
# define DEE_TYPES_SIZEOF_INTMAX_T  4
#elif DEE_TYPES_HAVE(2)
# define DEE_TYPES_SIZEOF_INTMAX_T  2
#elif DEE_TYPES_HAVE(1)
# define DEE_TYPES_SIZEOF_INTMAX_T  1
#endif
#endif /* !DEE_TYPES_SIZEOF_INTMAX_T */

#ifdef DEE_TYPES_SIZEOF_INTMAX_T
#define DEE_TYPES_SIZEOF_UINTMAX_T DEE_TYPES_SIZEOF_INTMAX_T
#endif

#ifdef DEE_PLATFORM_UNIX
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#if DEE_USE_DEBUG_NEW || defined(GUARD_DEBUG_NEW_H)
# include <debug_new_disable.inl>
# include <features.h>
# include <debug_new_enable.inl>
#else
# include <features.h>
#endif
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H */
# define DEE_TYPES_SIZEOF_UID_T      4
# define DEE_TYPES_SIZEOF_GID_T      4
# define DEE_TYPES_SIZEOF_MODE_T     4
# define DEE_TYPES_SIZEOF_PID_T      4
# define DEE_TYPES_SIZEOF_SOCKLEN_T  4
# define DEE_TYPES_SIZEOF_USECONDS_T 4
#ifdef __USE_FILE_OFFSET64
# define DEE_TYPES_SIZEOF_OFF_T      8
#else
# define DEE_TYPES_SIZEOF_OFF_T      4
#endif
# define DEE_TYPES_SIZEOF_OFF64_T    8
#endif

#ifdef DEE_PLATFORM_WINDOWS
# define DEE_TYPES_SIZEOF_DEE_PROCESS_ID     4
# define DEE_TYPES_SIZEOF_DEE_PROCESS_RETURN 4
# define DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE DEE_TYPES_SIZEOF_POINTER
#elif defined(DEE_PLATFORM_UNIX)
# define DEE_TYPES_SIZEOF_DEE_PROCESS_ID     DEE_TYPES_SIZEOF_PID_T
# define DEE_TYPES_SIZEOF_DEE_PROCESS_RETURN 4
# define DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE DEE_TYPES_SIZEOF_PID_T
#endif



#ifdef DEE_TYPES_UINT8_T
#define DEE_PRIVATES_TYPES_IPRINTF_1  "%I8d"
#define DEE_PRIVATES_TYPES_IUPRINTF_1 "%I8u"
#endif
#ifdef DEE_TYPES_UINT16_T
#define DEE_PRIVATES_TYPES_IPRINTF_2  "%I16d"
#define DEE_PRIVATES_TYPES_IUPRINTF_2 "%I16u"
#endif
#ifdef DEE_TYPES_UINT32_T
#define DEE_PRIVATES_TYPES_IPRINTF_4  "%I32d"
#define DEE_PRIVATES_TYPES_IUPRINTF_4 "%I32u"
#endif
#ifdef DEE_TYPES_UINT64_T
#define DEE_PRIVATES_TYPES_IPRINTF_8  "%I64d"
#define DEE_PRIVATES_TYPES_IUPRINTF_8 "%I64u"
#endif

//////////////////////////////////////////////////////////////////////////
// Some optimizations, to get the printf string shorter
#ifdef DEE_TYPES_SIZEOF_SIZE_T
#if DEE_TYPES_SIZEOF_SIZE_T == 4
# undef DEE_PRIVATES_TYPES_IPRINTF_4
# undef DEE_PRIVATES_TYPES_IUPRINTF_4
# define DEE_PRIVATES_TYPES_IPRINTF_4  "%Id"
# define DEE_PRIVATES_TYPES_IUPRINTF_4 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 8
# undef DEE_PRIVATES_TYPES_IPRINTF_8
# undef DEE_PRIVATES_TYPES_IUPRINTF_8
# define DEE_PRIVATES_TYPES_IPRINTF_8  "%Id"
# define DEE_PRIVATES_TYPES_IUPRINTF_8 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 1
# undef DEE_PRIVATES_TYPES_IPRINTF_1
# undef DEE_PRIVATES_TYPES_IUPRINTF_1
# define DEE_PRIVATES_TYPES_IPRINTF_1  "%Id"
# define DEE_PRIVATES_TYPES_IUPRINTF_1 "%Iu"
#elif DEE_TYPES_SIZEOF_SIZE_T == 2
# undef DEE_PRIVATES_TYPES_IPRINTF_2
# undef DEE_PRIVATES_TYPES_IUPRINTF_2
# define DEE_PRIVATES_TYPES_IPRINTF_2  "%Id"
# define DEE_PRIVATES_TYPES_IUPRINTF_2 "%Iu"
#endif
#endif /* DEE_TYPES_SIZEOF_SIZE_T */
#ifdef DEE_TYPES_SIZEOF_INT
#if DEE_TYPES_SIZEOF_INT == 4
# undef DEE_PRIVATES_TYPES_IPRINTF_4
# undef DEE_PRIVATES_TYPES_IUPRINTF_4
# define DEE_PRIVATES_TYPES_IPRINTF_4  "%d"
# define DEE_PRIVATES_TYPES_IUPRINTF_4 "%u"
#elif DEE_TYPES_SIZEOF_INT == 2
# undef DEE_PRIVATES_TYPES_IPRINTF_2
# undef DEE_PRIVATES_TYPES_IUPRINTF_2
# define DEE_PRIVATES_TYPES_IPRINTF_2  "%d"
# define DEE_PRIVATES_TYPES_IUPRINTF_2 "%u"
#elif DEE_TYPES_SIZEOF_INT == 1
# undef DEE_PRIVATES_TYPES_IPRINTF_1
# undef DEE_PRIVATES_TYPES_IUPRINTF_1
# define DEE_PRIVATES_TYPES_IPRINTF_1  "%d"
# define DEE_PRIVATES_TYPES_IUPRINTF_1 "%u"
#elif DEE_TYPES_SIZEOF_INT == 8
# undef DEE_PRIVATES_TYPES_IPRINTF_8
# undef DEE_PRIVATES_TYPES_IUPRINTF_8
# define DEE_PRIVATES_TYPES_IPRINTF_8  "%d"
# define DEE_PRIVATES_TYPES_IUPRINTF_8 "%u"
#endif
#endif /* DEE_TYPES_SIZEOF_INT */
#define DEE_PRIVATES_TYPES_IXPRINTF_0 DEE_PRIVATE_TYPES_IUPRINTF
#define DEE_PRIVATES_TYPES_IXPRINTF_1 DEE_PRIVATE_TYPES_IPRINTF

//////////////////////////////////////////////////////////////////////////
// Printf helpers (NOTE: Only guarantied to work with deemon's own printf-style functions)
#define DEE_TYPES_IPRINTF(sizeof)                 DEE_PRIVATE_TYPES_IPRINTF(sizeof)
#define DEE_TYPES_IUPRINTF(sizeof)                DEE_PRIVATE_TYPES_IUPRINTF(sizeof)
#define DEE_TYPES_IXPRINTF(signed,sizeof)         DEE_PRIVATE_TYPES_IXPRINTF(signed,sizeof)
#define DEE_PRIVATE_TYPES_IPRINTF(sizeof)         DEE_PRIVATES_TYPES_IPRINTF_##sizeof
#define DEE_PRIVATE_TYPES_IUPRINTF(sizeof)        DEE_PRIVATES_TYPES_IUPRINTF_##sizeof
#define DEE_PRIVATE_TYPES_IXPRINTF(signed,sizeof) DEE_PRIVATES_TYPES_IXPRINTF_##signed(sizeof)


#ifndef DEE_STATIC_INLINE
#if defined(__cplusplus)
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE inline T
#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__) || defined(__SC__)\
   || defined(__WATCOMC__) || defined(__LCC__) || defined(__DECC)
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE __inline T
#elif defined(__GNUC__)
# define DEE_STATIC_INLINE(T)  static DEE_ATTRIBUTE_UNUSED DEE_ATTRIBUTE_ALWAYS_INLINE __inline__ T
#elif defined(__DEEMON__)
# define DEE_STATIC_INLINE(T)  DEE_ATTRIBUTE_UNUSED T
#else
# define DEE_STATIC_INLINE(T)  static DEE_ATTRIBUTE_UNUSED T
#endif
#endif /* !DEE_STATIC_INLINE */

#ifndef DEE_STATIC_ASSERT
#if __has_feature(cxx_static_assert) || \
   (defined(__IBMCPP_STATIC_ASSERT) && __IBMCPP_STATIC_ASSERT) || (defined(__cplusplus) && (\
   (defined(_MSC_VER) && _MSC_VER >= 1600) || \
   (defined(__GNUC__) && __GNUC__*100+__GNUC_MINOR__ >= 403 && \
    (defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L))) || \
   (defined(__BORLANDC__) && defined(DEE_PRIVATE_CODEGEAR_0X_SUPPORT) && __BORLANDC__ >= 0x610)))
# define DEE_STATIC_ASSERT(x) static_assert((x),#x)
#elif defined(__TPP_COUNTER)
# define DEE_STATIC_ASSERT(x) typedef int DEE_PP_CAT_2(_dee_static_assert_,__TPP_COUNTER(dee_static_assert))[!!(x)?1:0]
#elif defined(__COUNTER__)
# define DEE_STATIC_ASSERT(x) typedef int DEE_PP_CAT_2(_dee_static_assert_,__COUNTER__)[!!(x)?1:0]
#else
# define DEE_STATIC_ASSERT(x) typedef int DEE_PP_CAT_2(_dee_static_assert_,__LINE__)[!!(x)?1:0]
#endif
#endif /* !DEE_STATIC_ASSERT */

#ifndef DEE_STRUCT_DEF
#if defined(__cplusplus) || defined(__DEEMON__)
# define DEE_STRUCT_DEF(T) struct T
#else
# define DEE_STRUCT_DEF(T) struct T; typedef struct T T
#endif
#endif

#ifndef DEE_UNUSED
#if defined(__LCLINT__)
# define DEE_UNUSED(x) /*@unused@*/ x
#elif defined(__GNUC__)
# define DEE_UNUSED(x) x DEE_ATTRIBUTE_UNUSED
#elif defined(__cplusplus) || defined(__DEEMON__)
# define DEE_UNUSED(x) /* nothing */
#elif defined(_MSC_VER)
# define DEE_UNUSED(x) x
#ifdef DEE_LIMITED_DEX
# pragma warning(disable: 4100)
#endif
#else
# define DEE_UNUSED(x) DEE_ATTRIBUTE_UNUSED x
#endif
#endif /* !DEE_UNUSED */


#ifndef DEE_INT8_C
#ifdef DEE_TYPES_INT8_T
#if DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT8_C(x) x##i8
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST8_T) && DEE_TYPES_SIZEOF_INT_LEAST16_T == 1
#ifdef INT8_C
# define DEE_INT8_C    INT8_C
#elif defined(__INT8_C)
# define DEE_INT8_C    __INT8_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_INT8_T */
#ifndef DEE_INT8_C
# define DEE_INT8_C(x) x
#endif
#endif /* !DEE_INT8_C */

#ifndef DEE_UINT8_C
#ifdef DEE_TYPES_UINT8_T
#if DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT8_C(x) x##ui8
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST8_T) && DEE_TYPES_SIZEOF_INT_LEAST16_T == 1
#ifdef UINT8_C
# define DEE_UINT8_C    UINT8_C
#elif defined(__UINT8_C)
# define DEE_UINT8_C    __UINT8_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_UINT8_T */
#ifndef DEE_UINT8_C
# define DEE_UINT8_C(x) x
#endif
#endif /* !DEE_UINT8_C */

#ifndef DEE_INT16_C
#ifdef DEE_TYPES_INT16_T
#if DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT16_C(x) x##i16
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST16_T) && DEE_TYPES_SIZEOF_INT_LEAST16_T == 2
#ifdef INT16_C
# define DEE_INT16_C    INT16_C
#elif defined(__INT16_C)
# define DEE_INT16_C    __INT16_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_INT16_T */
#ifndef DEE_INT16_C
# define DEE_INT16_C(x) x
#endif
#endif /* !DEE_INT16_C */

#ifndef DEE_UINT16_C
#ifdef DEE_TYPES_UINT16_T
#if DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT16_C(x) x##ui16
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST16_T) && DEE_TYPES_SIZEOF_INT_LEAST16_T == 2
#ifdef UINT16_C
# define DEE_UINT16_C    UINT16_C
#elif defined(__UINT16_C)
# define DEE_UINT16_C    __UINT16_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_UINT16_T */
#ifndef DEE_UINT16_C
# define DEE_UINT16_C(x) x
#endif
#endif /* !DEE_UINT16_C */

#ifndef DEE_INT32_C
#ifdef DEE_TYPES_INT32_T
#if defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 4
# define DEE_INT32_C(x) x
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 4
# define DEE_INT32_C(x) x##l
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT32_C(x) x##i32
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST32_T) && DEE_TYPES_SIZEOF_INT_LEAST32_T == 4
#ifdef INT32_C
# define DEE_INT32_C    INT32_C
#elif defined(__INT32_C)
# define DEE_INT32_C    __INT32_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_INT32_T */
#ifndef DEE_INT32_C
# define DEE_INT32_C(x) x
#endif
#endif /* !DEE_INT32_C */

#ifndef DEE_UINT32_C
#ifdef DEE_TYPES_UINT32_T
#if defined(DEE_TYPES_SIZEOF_INT) && DEE_TYPES_SIZEOF_INT == 4
# define DEE_UINT32_C(x) x##u
#elif defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 4
# define DEE_UINT32_C(x) x##ul
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT32_C(x) x##ui32
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST32_T) && DEE_TYPES_SIZEOF_INT_LEAST32_T == 4
#ifdef UINT32_C
# define DEE_UINT32_C    UINT32_C
#elif defined(__UINT32_C)
# define DEE_UINT32_C    __UINT32_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_UINT32_T */
#ifndef DEE_UINT32_C
#define DEE_UINT32_C(x) x
#endif
#endif /* !DEE_UINT32_C */

#ifndef DEE_INT64_C
#ifdef DEE_TYPES_INT64_T
#if defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 8
# define DEE_INT64_C(x) x##l
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
# define DEE_INT64_C(x) x##ll
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_INT64_C(x) x##i64
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST64_T) && DEE_TYPES_SIZEOF_INT_LEAST64_T == 8
#ifdef INT64_C
# define DEE_INT64_C    INT64_C
#elif defined(__INT64_C)
# define DEE_INT64_C    __INT64_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_INT64_T */
#ifndef DEE_INT64_C
# define DEE_INT64_C(x) x
#endif
#endif /* !DEE_INT64_C */

#ifndef DEE_UINT64_C
#ifdef DEE_TYPES_UINT64_T
#if defined(DEE_TYPES_SIZEOF_LONG) && DEE_TYPES_SIZEOF_LONG == 8
# define DEE_UINT64_C(x) x##ul
#elif defined(DEE_TYPES_SIZEOF_LLONG) && DEE_TYPES_SIZEOF_LLONG == 8
# define DEE_UINT64_C(x) x##ull
#elif DEE_COMPILER_HAVE_MSVC_INTEGER_SUFFIX
# define DEE_UINT64_C(x) x##ui64
#elif defined(DEE_TYPES_SIZEOF_INT_LEAST64_T) && DEE_TYPES_SIZEOF_INT_LEAST64_T == 8
#ifdef UINT64_C
# define DEE_UINT64_C    UINT64_C
#elif defined(__UINT64_C)
# define DEE_UINT64_C    __UINT64_C
#endif
#endif /* ... */
#endif /* DEE_TYPES_UINT64_T */
#ifndef DEE_UINT64_C
# define DEE_UINT64_C(x) x
#endif
#endif /* !DEE_UINT64_C */

#ifndef Dee_MIN
#define Dee_MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef Dee_MAX
#define Dee_MAX(a,b) ((a)<(b)?(b):(a))
#endif

#ifdef DEE_PLATFORM_WINDOWS
#define DEE_TYPES_SIZEOF_UID_T  DEE_TYPES_SIZEOF_POINTER // sizeof(PSID)
#define DEE_TYPES_SIZEOF_GID_T  DEE_TYPES_SIZEOF_POINTER // sizeof(PSID)
#define DEE_TYPES_SIZEOF_MODE_T 4
#endif


#if defined(__INTELLISENSE__) && \
    defined(DEE_PLATFORM_WINDOWS) && \
    defined(DEE_LIMITED_API)
// Just for syntax highlighting
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
#pragma warning(disable: 4619) // Invlaid warning number
#pragma warning(disable: 4987) // Non-standard extension used
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
