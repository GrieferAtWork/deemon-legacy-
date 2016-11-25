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
#ifndef GUARD_DEEMON_POSIX_FEATURES_INL
#define GUARD_DEEMON_POSIX_FEATURES_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H\
 || DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#include <features.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h> // _POSIX_PRIORITY_SCHEDULING, _POSIX_TIMERS, _POSIX_SYNCHRONIZED_IO
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* ... */

#ifndef DEE_HAVE_PTHREAD_GETUNIQUE_NP
// TODO: IBM (may) have something to do with this...
#define DEE_HAVE_PTHREAD_GETUNIQUE_NP 0
#endif

#ifndef DEE_HAVE_PTHREAD_TRYJOIN_NP
#if defined(_GNU_SOURCE)
# define DEE_HAVE_PTHREAD_TRYJOIN_NP   1
#else
# define DEE_HAVE_PTHREAD_TRYJOIN_NP   0
#endif
#endif

#ifndef DEE_HAVE_PTHREAD_TIMEDJOIN_NP
#if defined(_GNU_SOURCE)
# define DEE_HAVE_PTHREAD_TIMEDJOIN_NP 1
#else
# define DEE_HAVE_PTHREAD_TIMEDJOIN_NP 0
#endif
#endif


#ifndef DEE_HAVE_PTHREAD_SETNAME_NP
#if defined(_GNU_SOURCE)
# define DEE_HAVE_PTHREAD_SETNAME_NP 1
#else
# define DEE_HAVE_PTHREAD_SETNAME_NP 0
#endif
#endif

#ifndef DEE_HAVE_NANOSLEEP
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L)
# define DEE_HAVE_NANOSLEEP 1
#else
# define DEE_HAVE_NANOSLEEP 0
#endif
#endif

#ifndef DEE_HAVE_CLOCK_NANOSLEEP
#if defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600\
 || defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L 
# define DEE_HAVE_CLOCK_NANOSLEEP 1
#else
# define DEE_HAVE_CLOCK_NANOSLEEP 0
#endif
#endif

#ifndef DEE_HAVE_USLEEP
#if ((defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) \
 && !(defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L))\
 || (defined(_DEFAULT_SOURCE) && _DEFAULT_SOURCE)\
 || (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)
# define DEE_HAVE_USLEEP 1
#else
# define DEE_HAVE_USLEEP 0
#endif
#endif

#ifndef DEE_HAVE_SCHED_YIELD
#ifdef _POSIX_PRIORITY_SCHEDULING 
# define DEE_HAVE_SCHED_YIELD 1
#else
# define DEE_HAVE_SCHED_YIELD 0
#endif
#endif

#ifndef DEE_HAVE_SELECT
#if defined(DEE_PLATFORM_WINDOWS)\
 || (DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H)\
 || (DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H\
  && DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H\
  && DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H)
# define DEE_HAVE_SELECT 1
#else
# define DEE_HAVE_SELECT 0
#endif
#endif

#ifndef DEE_HAVE_CLOCK_GETTIME
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
# define DEE_HAVE_CLOCK_GETTIME 1
#else
# define DEE_HAVE_CLOCK_GETTIME 0
#endif
#endif
#ifndef DEE_HAVE_CLOCK_MONOTONIC
#if defined(_POSIX_MONOTONIC_CLOCK)
# define DEE_HAVE_CLOCK_MONOTONIC 1
#else
# define DEE_HAVE_CLOCK_MONOTONIC 0
#endif
#endif
#ifndef DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID
#if defined(_POSIX_CPUTIME)
# define DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID 1
#else
# define DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID 0
#endif
#endif
#ifndef DEE_HAVE_CLOCK_THREAD_CPUTIME_ID
#if defined(_POSIX_THREAD_CPUTIME)
# define DEE_HAVE_CLOCK_THREAD_CPUTIME_ID 1
#else
# define DEE_HAVE_CLOCK_THREAD_CPUTIME_ID 0
#endif
#endif

#ifndef DEE_HAVE_LOCALTIME_R
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE)\
 || (defined(_BSD_SOURCE) && _BSD_SOURCE) || \
    (defined(_SVID_SOURCE) && _SVID_SOURCE) || \
    (defined(_POSIX_SOURCE) && _POSIX_SOURCE)
# define DEE_HAVE_LOCALTIME_R 1
#else
# define DEE_HAVE_LOCALTIME_R 0
#endif
#endif

#ifndef DEE_HAVE_SETENV
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)\
 || (defined(_BSD_SOURCE) && _BSD_SOURCE)
# define DEE_HAVE_SETENV 1
#else
# define DEE_HAVE_SETENV 0
#endif
#endif

#ifndef DEE_HAVE__WSETENV
#define DEE_HAVE__WSETENV 0
#endif

#ifndef DEE_HAVE_PUTENV
#if (defined(_SVID_SOURCE) && _SVID_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE)\
 || (defined(_MSC_VER))
# define DEE_HAVE_PUTENV 1
#else
# define DEE_HAVE_PUTENV 0
#endif
#endif

#ifndef DEE_HAVE__WPUTENV
#if defined(_MSC_VER)
# define DEE_HAVE__WPUTENV 1
#else
# define DEE_HAVE__WPUTENV 0
#endif
#endif

#ifndef DEE_HAVE_UNSETENV
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)\
 || (defined(_BSD_SOURCE) && _BSD_SOURCE)
# define DEE_HAVE_UNSETENV 1
#else
# define DEE_HAVE_UNSETENV 0
#endif
#endif

#ifndef DEE_HAVE__WUNSETENV
#define DEE_HAVE__WUNSETENV 0
#endif

#ifndef DEE_HAVE_READLINK
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE && defined(_XOPEN_SOURCE_EXTENDED) && _XOPEN_SOURCE_EXTENDED)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
# define DEE_HAVE_READLINK 1
#else
# define DEE_HAVE_READLINK 0
#endif
#endif

#ifndef DEE_HAVE_FOPENDIR
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)\
 || (defined(_GNU_SOURCE) && _GNU_SOURCE)
# define DEE_HAVE_FOPENDIR 1
#else
# define DEE_HAVE_FOPENDIR 0
#endif
#endif

#ifndef DEE_HAVE_GETLOGIN_R
#if (defined(_REENTRANT) && _REENTRANT)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199506L)
# define DEE_HAVE_GETLOGIN_R 1
#else
# define DEE_HAVE_GETLOGIN_R 0
#endif
#endif

#ifndef DEE_HAVE_CHOWN
#if defined(DEE_PLATFORM_UNIX)
# define DEE_HAVE_CHOWN 1
#else
# define DEE_HAVE_CHOWN 0
#endif
#endif

#ifndef DEE_HAVE_FCHOWN
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE\
  && defined(_XOPEN_SOURCE_EXTENDED) && _XOPEN_SOURCE_EXTENDED)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
# define DEE_HAVE_FCHOWN 1
#else
# define DEE_HAVE_FCHOWN 0
#endif
#endif

#ifndef DEE_HAVE_CHMOD
#if defined(DEE_PLATFORM_UNIX)\
 || defined(_MSC_VER)
# define DEE_HAVE_CHMOD 1
#else
# define DEE_HAVE_CHMOD 0
#endif
#endif

#ifndef DEE_HAVE_FCHMOD
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE\
  && defined(_XOPEN_SOURCE_EXTENDED) && _XOPEN_SOURCE_EXTENDED)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
# define DEE_HAVE_FCHMOD 1
#else
# define DEE_HAVE_FCHMOD 0
#endif
#endif

#ifndef DEE_HAVE_STAT
#if defined(DEE_PLATFORM_UNIX)\
 || defined(_MSC_VER)
# define DEE_HAVE_STAT 1
#else
# define DEE_HAVE_STAT 0
#endif
#endif

#ifndef DEE_HAVE_FSTAT
#if defined(DEE_PLATFORM_UNIX)\
 || defined(_MSC_VER)
# define DEE_HAVE_FSTAT  1
#else
# define DEE_HAVE_FSTAT  0
#endif
#endif

#ifndef DEE_HAVE_LSTAT
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_DEFAULT_SOURCE) && _DEFAULT_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
# define DEE_HAVE_LSTAT 1
#else
# define DEE_HAVE_LSTAT 0
#endif
#endif

#ifndef DEE_HAVE_EXECVPE
#if defined(_GNU_SOURCE)
# define DEE_HAVE_EXECVPE 1
#else
# define DEE_HAVE_EXECVPE 0
#endif
#endif

#ifndef DEE_HAVE_UALARM
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500 || _XOPEN_SOURCE\
  && defined(_XOPEN_SOURCE_EXTENDED) && _XOPEN_SOURCE_EXTENDED)\
  && !((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)\
    || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700)))
# define DEE_HAVE_UALARM 1
#else
# define DEE_HAVE_UALARM 0
#endif
#endif

#ifndef DEE_HAVE_SEM_TIMEDWAIT
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
# define DEE_HAVE_SEM_TIMEDWAIT 1
#else
# define DEE_HAVE_SEM_TIMEDWAIT 0
#endif
#endif

#if defined(_MSC_VER)
#define fseek64 _fseeki64
#define ftell64 _ftelli64
#define lseek64 _lseeki64
#endif

#ifndef DEE_HAVE_FSEEK64
#if defined(_MSC_VER)
# define DEE_HAVE_FSEEK64 1
#else
# define DEE_HAVE_FSEEK64 0
#endif
#endif

#ifndef DEE_HAVE_FTELL64
#if defined(_MSC_VER)
# define DEE_HAVE_FTELL64 1
#else
# define DEE_HAVE_FTELL64 0
#endif
#endif

#ifndef DEE_HAVE_FSEEKO
#if (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
# define DEE_HAVE_FSEEKO 1
#else
# define DEE_HAVE_FSEEKO 0
#endif
#endif

#ifndef DEE_HAVE_FTELLO
#if (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
# define DEE_HAVE_FTELLO 1
#else
# define DEE_HAVE_FTELLO 0
#endif
#endif

#ifndef DEE_HAVE_LSEEK64
#if defined(_LARGEFILE64_SOURCE)\
 || defined(_MSC_VER)
# define DEE_HAVE_LSEEK64 1
#else
# define DEE_HAVE_LSEEK64 0
#endif
#endif

#ifndef DEE_HAVE_FSYNC
#if (defined(_BSD_SOURCE) && _BSD_SOURCE)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
# define DEE_HAVE_FSYNC 1
#else
# define DEE_HAVE_FSYNC 0
#endif
#endif

#ifndef DEE_HAVE_FDATASYNC
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 && (defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0)
# define DEE_HAVE_FDATASYNC 1
#else
# define DEE_HAVE_FDATASYNC 0
#endif
#endif

#ifndef DEE_HAVE_FTRUNCATE
#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)\
 || (defined(_BSD_SOURCE) && _BSD_SOURCE)
# define DEE_HAVE_FTRUNCATE 1
#else
# define DEE_HAVE_FTRUNCATE 0
#endif
#endif

#ifndef DEE_HAVE_FILENO
#if (defined(_MSC_VER))\
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1)\
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE)\
 || (defined(_POSIX_SOURCE) && _POSIX_SOURCE)
# define DEE_HAVE_FILENO 1
#else
# define DEE_HAVE_FILENO 0
#endif
#endif

#ifndef DEE_HAVE_SYSCONF
#if defined(DEE_PLATFORM_UNIX)
# define DEE_HAVE_SYSCONF 1
#else
# define DEE_HAVE_SYSCONF 0
#endif
#endif

#ifndef DEE_HAVE_GETRANDOM
#define DEE_HAVE_GETRANDOM 0 // TODO
#endif

#ifndef DEE_HAVE_GETENTROPY
// TODO: Available since 'OpenBSD 5.6' (how can I detect this?)
#define DEE_HAVE_GETENTROPY 0
#endif

#ifndef DEE_HAVE_REMOVE
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
# define DEE_HAVE_REMOVE 1 /*< Should be part of the c-standard. */
#else
# define DEE_HAVE_REMOVE 0
#endif
#endif

#ifndef DEE_HAVE_UNLINK
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H \
 ||(DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H && defined(_MSC_VER))
# define DEE_HAVE_UNLINK 1
#else
# define DEE_HAVE_UNLINK 0
#endif
#endif

#ifndef DEE_HAVE_RMDIR
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H \
 ||(DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H && defined(_MSC_VER))
# define DEE_HAVE_RMDIR 1
#else
# define DEE_HAVE_RMDIR 0
#endif
#endif

#ifndef DEE_HAVE_RENAME
#if (DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H) \
 || (defined(DEE_PLATFORM_UNIX))
# define DEE_HAVE_RENAME 1
#else
# define DEE_HAVE_RENAME 0
#endif
#endif


#ifndef DEE_HAVE_PREAD
#if defined(DEE_HAVE_PWRITE)
# define DEE_HAVE_PREAD DEE_HAVE_PWRITE
#elif (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)\
   || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
# define DEE_HAVE_PREAD 1
#else
# define DEE_HAVE_PREAD 0
#endif
#endif
#ifndef DEE_HAVE_PWRITE
#define DEE_HAVE_PWRITE DEE_HAVE_PREAD
#endif

#ifndef DEE_HAVE__WOPEN
#if defined(_MSC_VER)
# define DEE_HAVE__WOPEN 1
#else
# define DEE_HAVE__WOPEN 0
#endif
#endif

#ifndef DEE_HAVE__WGETCWD
#if defined(_MSC_VER)
# define DEE_HAVE__WGETCWD 1
#else
# define DEE_HAVE__WGETCWD 0
#endif
#endif

#ifndef DEE_HAVE__WCHDIR
#if defined(_MSC_VER)
# define DEE_HAVE__WCHDIR 1
#else
# define DEE_HAVE__WCHDIR 0
#endif
#endif

#ifndef DEE_HAVE__WGETENV
#if defined(_MSC_VER)
# define DEE_HAVE__WGETENV 1
#else
# define DEE_HAVE__WGETENV 0
#endif
#endif

#ifndef DEE_HAVE__WENVIRON
#if defined(_MSC_VER)
# define DEE_HAVE__WENVIRON 1
#else
# define DEE_HAVE__WENVIRON 0
#endif
#endif

#ifndef DEE_HAVE__WSTAT
#if defined(_MSC_VER)
# define DEE_HAVE__WSTAT 1
#else
# define DEE_HAVE__WSTAT 0
#endif
#endif

#ifndef DEE_HAVE__WMKDIR
#if defined(_MSC_VER)
# define DEE_HAVE__WMKDIR 1
#else
# define DEE_HAVE__WMKDIR 0
#endif
#endif

#ifndef DEE_HAVE__WRMDIR
#if defined(_MSC_VER)
# define DEE_HAVE__WRMDIR 1
#else
# define DEE_HAVE__WRMDIR 0
#endif
#endif

#ifndef DEE_HAVE_PIPE
#if defined(DEE_PLATFORM_UNIX)
# define DEE_HAVE_PIPE 1
#else
# define DEE_HAVE_PIPE 0
#endif
#endif

#endif /* !GUARD_DEEMON_POSIX_FEATURES_INL */
