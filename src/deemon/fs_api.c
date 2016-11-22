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
#ifndef GUARD_DEEMON_FS_API_C
#define GUARD_DEEMON_FS_API_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/fs_api.h>
#include <deemon/generic_vtable.h>
#include <deemon/list.h>
#include <deemon/memberdef.h>
#include <deemon/mp/process.h>
#include <deemon/mp/thread.h>
#include <deemon/random.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime_flags.h>
#include <deemon/string.h>
#include <deemon/time.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/__alloca.inl>
#include <deemon/__xconf.inl>
#include <deemon/posix_features.inl>
#include <deemon/sys/sysfs.h>
#include <deemon/sys/sysfd.h>
#include <deemon/unicode/char_traits.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/virtual_fs.h.inl>
#endif

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Userenv.h>
#include <Lmcons.h>
#include <accctrl.h>
#include <Aclapi.h>
#if DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
DEE_COMPILER_MSVC_WARNING_POP
#endif

#if defined(DEE_PLATFORM_UNIX)
#ifdef _MSC_VER
#pragma warning(push,0)
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#include <features.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H
#include <stdlib.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#include <time.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H
#include <pwd.h>
#endif
#include <deemon/optional/std/string.h>
#if DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H
#include <dirent.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H
#include <utime.h>
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#if DEE_CONFIG_RUNTIME_HAVE_VFS
int DeeFlag_NoVFS = 0;

extern void _DeeFlag_SetNoVFSString(char const *value);
void _DeeFlag_SetNoVFSString(char const *value) {
 int new_flag;
 if (!*value) new_flag = 0;
 else if (DeeString_ToNumber(int,value,&new_flag) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_NOVFS
                 "' (defaulting to '0')\n",1);
  new_flag = 0;
 } else new_flag = !!new_flag;
 DEE_LVERBOSE1("Setting: 'DeeFlag_NoVFS = %d;'\n",new_flag);
 DeeFlag_NoVFS = new_flag;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */


#ifdef DEE_PLATFORM_WINDOWS
#define CMP_PATH_C(a,b) (DEE_CH_TO_UPPER(a)==DEE_CH_TO_UPPER(b))
#else
#define CMP_PATH_C(a,b) ((a)==(b))
#endif
#define EXT_SEP   DEE_FS_EXT_SEP
#define SEP       DEE_FS_SEP
#define ALT_SEP   DEE_FS_ALT_SEP
#ifdef DEE_FS_DRIVE_SEP
#define DRIVE_SEP DEE_FS_DRIVE_SEP
#endif

#ifdef DRIVE_SEP
#define IS_DRIVE_SEP(x) ((x)==DRIVE_SEP)
#endif

#define IS_SEP        DEE_FS_ISSEP
#define IS_EXT_SEP(x) ((x)==EXT_SEP)

#ifdef DEE_PLATFORM_WINDOWS
#define DEE_HAVE_WIDEAPI 1
#else
#define DEE_HAVE_WIDEAPI 0
#endif

#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV

#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
extern void _Dee_SetStackLimitString(char const *value);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
extern void _DeeFlag_SetNoSignalHandlersString(char const *value);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
extern void _DeeFlag_SetNoVFSString(char const *value);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
extern void _DeeSocket_SetMaxUDPPacketSizeString(char const *value);
extern void _DeeSocket_SetDefaultMaxBacklogString(char const *value);
#endif
#ifdef DEE_DEBUG
extern void _DeeFlag_SetNoAssertString(char const *value);
extern void _DeeFlag_SetVerboseString(char const *value);
#endif

static void DeeFS_Utf8ReconfigureEnv(
 DEE_A_IN_Z Dee_Utf8Char const *name,
 DEE_A_IN_Z Dee_Utf8Char const *value) {
#ifdef DEE_PLATFORM_WINDOWS
# define DEE_FS_UTF8_PLATCMP Dee_Utf8StrCaseCmp
#else
# define DEE_FS_UTF8_PLATCMP Dee_Utf8StrCmp
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT) == 0) _Dee_SetStackLimitString(value); else
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS) == 0) _DeeFlag_SetNoSignalHandlersString(value); else
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_NOVFS) == 0) _DeeFlag_SetNoVFSString(value); else
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE) == 0) _DeeSocket_SetMaxUDPPacketSizeString(value); else
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG) == 0) _DeeSocket_SetDefaultMaxBacklogString(value); else
#endif
#ifdef DEE_DEBUG
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT) == 0) _DeeFlag_SetNoAssertString(value); else
 if (DEE_FS_UTF8_PLATCMP(name,DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE) == 0) _DeeFlag_SetVerboseString(value); else
#endif
 (void)name,value;
#undef DEE_FS_UTF8_PLATCMP
}

#endif /* DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV */

// Forward inline APIs
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathHeadObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathHeadObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathTailObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathTailObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExtObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExtObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathIncTrailObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathIncTrailObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExcTrailObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExcTrailObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *_DeeFS_Utf8PathAbsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *cwd);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *_DeeFS_WidePathAbsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OBJECT(DeeWideStringObject) const *cwd);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *_DeeFS_Utf8PathRelObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *cwd);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *_DeeFS_WidePathRelObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OBJECT(DeeWideStringObject) const *cwd);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathJoinObject(DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) DeeObject const *const *pathv);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathJoinObject(DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) DeeObject const *const *pathv);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpandUserObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpandUserObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpandVarsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpandVarsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsAbsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsAbsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
#define _DeeFS_Utf8IsAbsObject(path) _DeeFS_Utf8IsAbs(DeeUtf8String_STR(path))
#define _DeeFS_WideIsAbsObject(path) _DeeFS_WideIsAbs(DeeWideString_STR(path))
#ifdef DEE_PLATFORM_WINDOWS
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *_DeeFS_Utf8Win32ReadLinkHandle(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN HANDLE hfile);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *_DeeFS_WideWin32ReadLinkHandle(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN HANDLE hfile);
static void _deefs_win32_enable_symlink(void);
#ifndef SYMBOLIC_LINK_FLAG_DIRECTORY
#define SYMBOLIC_LINK_FLAG_DIRECTORY            (0x1)
#endif
#endif

#ifdef DEE_PLATFORM_UNIX
static int _dee_posix_check_getcwd_error(void);
static int _deefs_posix_try_stat(char const *path, struct stat *attrib);
static int _deefs_posix_stat(char const *path, struct stat *attrib);
static int _deefs_posix_fstat(int fd, DeeObject *path, struct stat *attrib);
#if DEE_HAVE_LSTAT
static int _deefs_posix_try_lstat(char const *path, struct stat *attrib);
#endif
#endif

#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
// Fixes slashes and prepends the '\\?\' prefix for extended path names
// >> Called automatically if functions fail because path names are too long
// NOTE: These functions require their arguments to not already be UNC paths!
// NOTE: Also note how both versions return a widestring.
//       This is because extended path names only work with the unicode-apis
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_Utf8Win32PathUNC(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32PathUNC(DEE_A_IN_Z Dee_WideChar const *path);
#define DeeFS_Utf8Win32IsPathUNC(path) ((path)[0]=='\\'&&(path)[1]=='\\'&&(path)[2]=='?'&&(path)[3]=='\\')
#define DeeFS_WideWin32IsPathUNC(path) ((path)[0]=='\\'&&(path)[1]=='\\'&&(path)[2]=='?'&&(path)[3]=='\\')
#define DEE_FS_WIN32_IS_UNC_ERROR(err)\
((err)==ERROR_FILE_NOT_FOUND||(err)==ERROR_PATH_NOT_FOUND||\
 (err)==ERROR_BAD_PATHNAME||(err)==ERROR_INVALID_NAME||\
 (err)==ERROR_BAD_NETPATH||(err)==ERROR_BAD_NET_NAME)
#endif
#define WIN32_IS_FILE_NOT_FOUND_ERROR(err)\
((err)==ERROR_FILE_NOT_FOUND||(err)==ERROR_PATH_NOT_FOUND||\
 (err)==ERROR_BAD_PATHNAME||(err)==ERROR_INVALID_NAME||\
 (err)==ERROR_BAD_NETPATH||(err)==ERROR_NOT_READY||\
 (err)==ERROR_BAD_NET_NAME)
#endif

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _dee_fs_utf8_gen_initial_tmpname(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix, DEE_A_OUT Dee_Utf8Char **digits);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _dee_fs_wide_gen_initial_tmpname(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix, DEE_A_OUT Dee_WideChar **digits);
DEE_STATIC_INLINE(int) _deefs_utf8_inc_digit(DEE_A_INOUT Dee_Utf8Char *digit);
DEE_STATIC_INLINE(int) _deefs_wide_inc_digit(DEE_A_INOUT Dee_WideChar *digit);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _deefs_utf8_inc_tmpname(Dee_Utf8Char *digits);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _deefs_wide_inc_tmpname(Dee_WideChar *digits);

#ifdef DEE_PLATFORM_WINDOWS
#ifndef GetEnvironmentStringsA
#undef GetEnvironmentStrings
#define GetEnvironmentStringsA GetEnvironmentStrings
#endif
#endif

#if !defined(environ) && \
    !defined(DEE_PLATFORM_WINDOWS)
extern char **environ;
#endif

#if !defined(S_ISDIR) && defined(_S_IFDIR)
#define S_ISDIR(x) (((x)&_S_IFDIR)!=0)
#endif
#if !defined(S_ISREG) && defined(_S_IFREG)
#define S_ISREG(x) (((x)&_S_IFREG)!=0)
#endif
#if !defined(S_ISLNK) && defined(_S_IFLNK)
#define S_ISLNK(x) (((x)&_S_IFLNK)!=0)
#endif


#ifndef __INTELLISENSE__
#define WIDE
#include "fs_api.unicode_impl.inl"
#include "fs_api.unicode_impl.inl"

#include "fs_api.win32_sid.inl"
#endif


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT(-1) int DeeFS_Win32HardLinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
 if DEE_UNLIKELY((dst = DeeFS_PathExpandObject(dst)) == NULL) return -1;
 if DEE_UNLIKELY((src = DeeFS_PathExpandObject(src)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(dst)) {
  if DEE_UNLIKELY(DeeWideString_InplaceCast(&src) != 0) { result = -1; goto end_1; }
  result = _DeeFS_WideWin32HardLink(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if (DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&dst) != 0)
   || DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&src) != 0)) { result = -1; goto end_1; }
  result = _DeeFS_Utf8Win32HardLink(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
 }
end_1: Dee_DECREF(src);
end_0: Dee_DECREF(dst);
 return result;
}

#endif

#ifdef DEE_PLATFORM_UNIX
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8PosixGetProcessName(DEE_A_IN pid_t process) {
#if DEE_HAVE_READLINK
 char proc_temp[32];
 sprintf(proc_temp,"/proc/%d/exe",process);
 return _DeeFS_Utf8ReadLink(proc_temp);
#else
 return DeeUtf8String_Newf("/proc/%d/exe",process);
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WidePosixGetProcessName(DEE_A_IN pid_t process) {
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeFS_Utf8PosixGetProcessName(process)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
}

static int _dee_posix_check_getcwd_error(void) {
 int error = errno; errno = 0;
 switch (error) {
  case 0:
  case ERANGE:
   return 0;
  default:
   DeeError_SystemError("getcwd");
   return -1;
 }
}
#endif

//////////////////////////////////////////////////////////////////////////
// Current working directory get/set
// NOTE: 'path' may be relative, allowing you to navigate from the current cwd

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_GetTmpNameObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if DEE_UNLIKELY((prefix = DeeFS_PathExpandObject(prefix)) == NULL) { result = NULL; goto end_0; }
 if (DeeWideString_Check(path)) {
  if DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)&prefix) != 0) { result = NULL; goto end_1; }
  result = _DeeFS_WideGetTmpName(DeeWideString_STR(path),DeeWideString_STR(prefix));
 } else {
  if (DEE_UNLIKELY(DeeUtf8String_InplaceCast(&path) != 0)
   || DEE_UNLIKELY(DeeUtf8String_InplaceCast(&prefix) != 0)) { result = NULL; goto end_1; }
  result = _DeeFS_Utf8GetTmpName(DeeUtf8String_STR(path),DeeUtf8String_STR(prefix));
 }
end_1: Dee_DECREF(prefix);
end_0: Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *_DeeFS_GetTmpNameObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) {
 DeeObject *result;
 if (DeeWideString_Check(path)) {
  if DEE_UNLIKELY((prefix = DeeWideString_Cast(prefix)) == NULL) return NULL;
  result = _DeeFS_WideGetTmpName(DeeWideString_STR(path),DeeWideString_STR(prefix));
  Dee_DECREF(prefix);
  return result;
 }
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 if DEE_UNLIKELY((prefix = DeeUtf8String_Cast(prefix)) == NULL) { result = NULL; goto end_0; }
 result = _DeeFS_Utf8GetTmpName(DeeUtf8String_STR(path),DeeUtf8String_STR(prefix));
 Dee_DECREF(prefix);
end_0: Dee_DECREF(path);
 return result;
}



#if 1
DEE_A_RET_EXCEPT(-1) int DeeFS_SetListEnv(
 DEE_A_IN_OBJECT(struct DeeListObject) const *v) {
 DeeObject *old_env,**iter,**end,*elem;
 DEE_ASSERT(DeeObject_Check(v) && DeeList_Check(v));
 if DEE_UNLIKELY((v = DeeList_Copy(v)) == NULL) return -1;
 if DEE_UNLIKELY((old_env = DeeFS_EnumEnv()) == NULL) {err_v: Dee_DECREF(v); return -1; }
 end = (iter = DeeList_ELEM(old_env))+DeeList_SIZE(old_env);
 while (iter != end) {
  DEE_ASSERT(DeeObject_Check(*iter) && DeeTuple_Check(*iter));
  DEE_ASSERT(DeeTuple_SIZE(*iter) == 2);
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,0)));
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,1)));
  if (DeeFS_DelEnvObject(DeeTuple_GET(*iter,0)) != 0) {
   Dee_DECREF(old_env);
   goto err_v;
  }
  ++iter;
 }
 Dee_DECREF(old_env);
 end = (iter = DeeList_ELEM(v))+DeeList_SIZE(v);
 while (iter != end) {
  elem = *iter++;
  if DEE_UNLIKELY(DeeFS_SetEnvObject(
   DeeTuple_GET(elem,0),DeeTuple_GET(elem,1)) != 0) goto err_v;
 }
 Dee_DECREF(v);
 return 0;
}
#endif


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathHeadObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathHeadObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathHeadObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathTailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathTailObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathTailObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathFileObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathFileObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExtObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExtObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExtObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathDriveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathDriveObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathDriveObject(path);
 Dee_DECREF(path);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathIncTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathIncTrailObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathIncTrailObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExcTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExcTrailObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExcTrailObject(path);
 Dee_DECREF(path);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_PathAbsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd = DeeFS_PathExpandObject(cwd)) == NULL) { result = NULL; goto end_0; }
 result = _DeeFS_PathAbsObject(path,cwd);
 Dee_DECREF(cwd);
end_0: Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *_DeeFS_PathAbsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DeeObject *result;
 if (DeeWideString_Check(path)) {
  if DEE_UNLIKELY((cwd = DeeWideString_Cast(cwd)) == NULL) return NULL;
  result = _DeeFS_WidePathAbsObject(path,cwd);
 } else {
  if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
  if DEE_UNLIKELY((cwd = DeeUtf8String_Cast(cwd)) == NULL) { Dee_DECREF(path); return NULL; }
  result = _DeeFS_Utf8PathAbsObject(path,cwd);
  Dee_DECREF(path);
 }
 Dee_DECREF(cwd);
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_PathRelObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd = DeeFS_PathExpandObject(cwd)) == NULL) { result = NULL; goto end_0; }
 result = _DeeFS_PathRelObject(path,cwd);
 Dee_DECREF(cwd);
end_0: Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *_DeeFS_PathRelObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DeeObject *result;
 if (DeeWideString_Check(path)) {
  if DEE_UNLIKELY((cwd = DeeWideString_Cast(cwd)) == NULL) return NULL;
  result = _DeeFS_WidePathRelObject(path,cwd);
 } else {
  if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
  if DEE_UNLIKELY((cwd = DeeUtf8String_Cast(cwd)) == NULL) { Dee_DECREF(path); return NULL; }
  result = _DeeFS_Utf8PathRelObject(path,cwd);
  Dee_DECREF(path);
 }
 Dee_DECREF(cwd);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_PathJoinObject(
 DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) DeeObject const *const *pathv) {
 if (!pathc) DeeReturn_EmptyUtf8String;
 return DeeWideString_Check(*pathv)
  ? DeeFS_WidePathJoinObject(pathc,pathv)
  : DeeFS_Utf8PathJoinObject(pathc,pathv);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExpandObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path));
 if (DeeWideString_Check(path)) {
  if DEE_UNLIKELY((path = DeeFS_WidePathExpandVarsObject(path)) == NULL) return NULL;
  result = DeeFS_WidePathExpandUserObject(path);
  Dee_DECREF(path);
 } else {
  if DEE_UNLIKELY((result = DeeUtf8String_Cast(path)) == NULL) return NULL;
  path = DeeFS_Utf8PathExpandVarsObject(result);
  Dee_DECREF(result);
  if DEE_UNLIKELY(!path) return NULL;
  result = DeeFS_Utf8PathExpandUserObject(path);
  Dee_DECREF(path);
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExpandUserObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path));
 if (DeeWideString_Check(path)) return DeeFS_WidePathExpandUserObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExpandUserObject(path);
 Dee_DECREF(path);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExpandVarsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExpandVarsObject(path);
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExpandVarsObject(path);
 Dee_DECREF(path);
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetDllDirectory(void) { return DeeWin32SysFS_Utf8GetDllDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetDllDirectory(void) { return DeeWin32SysFS_WideGetDllDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetSystemDirectory(void) { return DeeWin32SysFS_Utf8GetSystemDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetSystemDirectory(void) { return DeeWin32SysFS_WideGetSystemDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetWindowsDirectory(void) { return DeeWin32SysFS_Utf8GetWindowsDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetWindowsDirectory(void) { return DeeWin32SysFS_WideGetWindowsDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetSystemWindowsDirectory(void) { return DeeWin32SysFS_Utf8GetSystemWindowsDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetSystemWindowsDirectory(void) { return DeeWin32SysFS_WideGetSystemWindowsDirectory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetSysWow64Directory(void) { return DeeWin32SysFS_Utf8GetSysWow64Directory(); }
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetSysWow64Directory(void) { return DeeWin32SysFS_WideGetSysWow64Directory(); }
#endif


#ifdef DEE_PLATFORM_UNIX
static int _deefs_posix_stat(char const *path, struct stat *attrib) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(stat(path,attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "stat(%q) : %K",path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
}
static int _deefs_posix_fstat(int fd, DeeObject *path, struct stat *attrib) {
#if DEE_HAVE_FSTAT
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(fstat(fd,attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "fstat(%d:%k) : %K",fd,path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)fd;
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_path;
 if DEE_UNLIKELY(stat(DeeUtf8String_STR(path),attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "stat(%q) : %K",
                      DeeUtf8String_STR(path),
                      DeeSystemError_ToString(DeeSystemError_Consume()));
err_path:
  Dee_DECREF(path);
  return -1;
 }
 Dee_DECREF(path);
 return 0;
#endif
}
static int _deefs_posix_try_stat(
 char const *path, struct stat *attrib) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(stat(path,attrib) != 0) {
  int error = DeeSystemError_Consume();
  if (error == ENOTDIR || error == ENOENT) return 1; // Not found
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "stat(%q) : %K",
                      path,DeeSystemError_ToString(error));
  return -1;
 }
 return 0;
}
#if DEE_HAVE_LSTAT
static int _deefs_posix_try_lstat(char const *path, struct stat *attrib) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(lstat(path,attrib) != 0) {
  int error = DeeSystemError_Consume();
  if (error == ENOTDIR || error == ENOENT) return 1; // Not found
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "stat(%q) : %K",
                      path,DeeSystemError_ToString(error));
  return -1;
 }
 return 0;
}
#endif
#endif



#ifdef DEE_PLATFORM_WINDOWS
struct _deefs_win32_copywithprogress_systemcallback_data {
 DeeFS_ProcessFunc progress;
 void             *closure;
 BOOL              cancled;
 int               calllevel; // 0: nothing; 1: 0% called; 2: 100% called
};
static DWORD WINAPI _deefs_win32_copywithprogress_systemcallback(
 LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred,
 LARGE_INTEGER StreamSize, LARGE_INTEGER StreamBytesTransferred,
 DWORD dwStreamNumber, DWORD dwCallbackReason,
 HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData) {
 struct _deefs_win32_copywithprogress_systemcallback_data *data;
 data = (struct _deefs_win32_copywithprogress_systemcallback_data *)lpData;
 (void)StreamSize,StreamBytesTransferred,dwStreamNumber,
       dwCallbackReason,hSourceFile,hDestinationFile;
 if (data->cancled) return 0;
 if (data->calllevel < 1) {
  if DEE_UNLIKELY((*data->progress)(0.0,data->closure) != 0) goto cancle_op;
  data->calllevel = 1;
 }
 if (*(Dee_uint64_t *)&TotalBytesTransferred == 0) return 0;
 if (*(Dee_uint64_t *)&TotalBytesTransferred == *(Dee_uint64_t *)&TotalFileSize) {
  if (data->calllevel < 2) {
   if DEE_UNLIKELY((*data->progress)(1.0,data->closure) != 0) goto cancle_op;
   data->calllevel = 2;
  }
  return 0;
 } else {
  if DEE_UNLIKELY((*data->progress)((double)(
   (long double)*((Dee_uint64_t *)&TotalBytesTransferred)/
   (long double)*((Dee_uint64_t *)&TotalFileSize)
   ),data->closure) != 0) {
cancle_op:
   data->cancled = TRUE;
   return PROGRESS_CANCEL;
  }
 }
 return 0;
}
#endif

DEE_A_RET_EXCEPT(-1) int _DeeFS_ProgressCallObject(double progress, void *closure) {
 DeeObject *progress_ob,*args,*result;
 DEE_ASSERT(DeeObject_Check(closure));
 if DEE_UNLIKELY((progress_ob = DeeObject_New(double,progress)) == NULL) return -1;
 args = DeeTuple_Pack(1,progress_ob);
 Dee_DECREF(progress_ob);
 if DEE_UNLIKELY(!args) return -1;
 result = DeeObject_Call((DeeObject *)closure,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8CopyWithProgress(
 DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(src)) {
  DeeObject *src_path; int result;
  if DEE_UNLIKELY((src_path = DeeVFS_Utf8ReadReFsLink(src)) == NULL) return -1;
  result = _DeeFS_Utf8CopyWithProgress(DeeUtf8String_STR(src_path),dst,progress,closure);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_Utf8IsVirtualPath(dst)) {
  DeeObject *dst_path; int result;
  if DEE_UNLIKELY((dst_path = DeeVFS_Utf8ReadReFsLink(dst)) == NULL) return -1;
  result = _DeeFS_Utf8CopyWithProgress(src,DeeUtf8String_STR(dst_path),progress,closure);
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 {
  struct _deefs_win32_copywithprogress_systemcallback_data data;
  int error;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  data.progress = progress; data.closure = closure;
  data.calllevel = 0; data.cancled = FALSE;
  if DEE_UNLIKELY(!CopyFileExA(src,dst,&_deefs_win32_copywithprogress_systemcallback,
                              &data,&data.cancled,COPY_FILE_FAIL_IF_EXISTS)) {
   if DEE_UNLIKELY(data.cancled) return -1;
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      (!DeeFS_Utf8Win32IsPathUNC(src) ||
       !DeeFS_Utf8Win32IsPathUNC(dst))) {
    DeeObject *unc_src,*unc_dst;
    if DEE_UNLIKELY((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
     ? DeeFS_Utf8Win32PathUNC(src)
     : DeeWideString_FromUtf8String(src)) == NULL) return -1;
    if DEE_UNLIKELY((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
     ? DeeFS_Utf8Win32PathUNC(dst)
     : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
    error = _DeeFS_WideCopyWithProgress(DeeWideString_STR(unc_src),
                                        DeeWideString_STR(unc_dst),
                                        progress,closure);
    Dee_DECREF(unc_dst);
err_unc0: Dee_DECREF(unc_src);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "CopyFileExA(%q,%q) : %K",
                       src,dst,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  switch (data.calllevel) {
   case 0:
    if DEE_UNLIKELY((*progress)(0.0,closure) != 0) {
post_error:
     if DEE_UNLIKELY(_DeeFS_Utf8Unlink(dst) != 0) DeeError_Handled();
     return -1;
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   case 1:
    if DEE_UNLIKELY((*progress)(1.0,closure) != 0) goto post_error;
    break;
   default: break;
  }
  return 0;
 }
#else
 // TODO: Progress on posix
 if DEE_UNLIKELY((*progress)(0.0,closure) != 0) return -1;
 if DEE_UNLIKELY(_DeeFS_Utf8Copy(src,dst) != 0) return -1;
 if DEE_UNLIKELY((*progress)(1.0,closure) != 0) return -1;
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideCopyWithProgress(
 DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(src)) {
  DeeObject *src_path; int result;
  if DEE_UNLIKELY((src_path = DeeVFS_WideReadReFsLink(src)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src_path),dst,progress,closure);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_WideIsVirtualPath(dst)) {
  DeeObject *dst_path; int result;
  if DEE_UNLIKELY((dst_path = DeeVFS_WideReadReFsLink(dst)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(src,DeeWideString_STR(dst_path),progress,closure);
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 {
  struct _deefs_win32_copywithprogress_systemcallback_data data;
  int error;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  data.progress = progress; data.closure = closure;
  data.calllevel = 0; data.cancled = FALSE;
  if DEE_UNLIKELY(!CopyFileExW(src,dst,&_deefs_win32_copywithprogress_systemcallback,
                              &data,&data.cancled,COPY_FILE_FAIL_IF_EXISTS)) {
   if DEE_UNLIKELY(data.cancled) return -1;
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if DEE_LIKELY(DEE_FS_WIN32_IS_UNC_ERROR(error)) {
    DeeObject *temp;
    if (!DeeFS_WideWin32IsPathUNC(src)) {
     if DEE_UNLIKELY((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
     error = _DeeFS_WideWin32HardLink(DeeWideString_STR(temp),dst);
     goto unc_end0;
    }
    if (!DeeFS_WideWin32IsPathUNC(dst)) {
     if DEE_UNLIKELY((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
     error = _DeeFS_WideCopyWithProgress(src,DeeWideString_STR(temp),progress,closure);
unc_end0: Dee_DECREF(temp);
     return error;
    }
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "CopyFileExW(%lq,%lq) : %K",
                       src,dst,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  switch (data.calllevel) {
   case 0:
    if DEE_UNLIKELY((*progress)(0.0,closure) != 0) {
post_error:
     if DEE_UNLIKELY(_DeeFS_WideUnlink(dst) != 0) DeeError_Handled();
     return -1;
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   case 1:
    if DEE_UNLIKELY((*progress)(1.0,closure) != 0) goto post_error;
    break;
   default: break;
  }
  return 0;
 }
#else
 // TODO: Progress on posix
 if DEE_UNLIKELY((*progress)(0.0,closure) != 0) return -1;
 if DEE_UNLIKELY(_DeeFS_WideCopy(src,dst) != 0) return -1;
 if DEE_UNLIKELY((*progress)(1.0,closure) != 0) return -1;
 return 0;
#endif
}



DEE_A_RET_EXCEPT(-1) int _DeeFS_CopyObjectWithProgress(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY((dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src),DeeWideString_STR(dst),progress,closure);
 } else
#endif
 {
  if DEE_UNLIKELY((src = DeeUtf8String_Cast(src)) == NULL) return -1;
  if DEE_UNLIKELY((dst = DeeUtf8String_Cast(dst)) == NULL) { Dee_DECREF(src); return -1; }
  result = _DeeFS_Utf8CopyWithProgress(DeeUtf8String_STR(src),DeeUtf8String_STR(dst),progress,closure);
  Dee_DECREF(src);
 }
 Dee_DECREF(dst);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_CopyObjectWithProgress(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
 int result;
 if DEE_UNLIKELY((src = DeeFS_PathExpandObject(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst = DeeFS_PathExpandObject(dst)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY(DeeWideString_InplaceCast(&dst) != 0) { result = -1; goto end_1; }
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src),DeeWideString_STR(dst),progress,closure);
 } else
#endif
 {
  if (DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&src) != 0)
   || DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&dst) != 0)) { result = -1; goto end_1; }
  result = _DeeFS_Utf8CopyWithProgress(DeeUtf8String_STR(src),DeeUtf8String_STR(dst),progress,closure);
 }
end_1: Dee_DECREF(dst);
end_0: Dee_DECREF(src);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
_DeeFS_ReadLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideReadLink(DeeWideString_STR(path));
#endif
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_ReadLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideReadLink(DeeWideString_STR(path));
 } else
#endif
 {
  if DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&path) != 0) { result = NULL; goto end_0; }
  result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(path));
 }
end_0: Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8GetDeemonExecutable(void) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_Utf8Win32GetModuleName(NULL);
#elif defined(DEE_PLATFORM_UNIX)
 return DeeFS_Utf8PosixGetProcessName(getpid());
#else
 DeeReturn_STATIC_UTF8_STRING_EX(6,{'d','e','e','m','o','n'});
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WideGetDeemonExecutable(void) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_WideWin32GetModuleName(NULL);
#elif defined(DEE_PLATFORM_UNIX)
 return DeeFS_WidePosixGetProcessName(getpid());
#else
 DeeReturn_STATIC_WIDE_STRING_EX(6,{'d','e','e','m','o','n'});
#endif
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetMachineName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+MAX_COMPUTERNAME_LENGTH;
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized(MAX_COMPUTERNAME_LENGTH)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 if DEE_UNLIKELY(!GetComputerNameA(DeeUtf8String_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,result_size++) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   if DEE_UNLIKELY(!GetComputerNameA(DeeUtf8String_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetComputerNameA",error);
  goto err_r;
 }
 if (result_size != MAX_COMPUTERNAME_LENGTH &&
     DEE_UNLIKELY(DeeUtf8String_Resize(&result,result_size) != 0)
     ) goto err_r;
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *result; int error; Dee_size_t result_size = 256;
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized(256)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
  if DEE_UNLIKELY(gethostname(DeeUtf8String_STR(result),result_size) == 0) break;
  error = errno; errno = 0;
#ifdef ENAMETOOLONG
  if (error == ENAMETOOLONG || error == EINVAL)
#else
  if (error == EINVAL)
#endif
  {
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,(result_size *= 2)) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
  } else {
   DeeError_SystemErrorExplicit("gethostname",error);
   goto err_r;
  }
 }
 if DEE_UNLIKELY(DeeUtf8String_Resize(&result,
  Dee_Utf8StrLen(DeeUtf8String_STR(result))) != 0) goto err_r;
 return result;
#else
 DeeReturn_STATIC_UTF8_STRING_EX(9,{'l','o','c','a','l','h','o','s','t'});
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideGetMachineName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+MAX_COMPUTERNAME_LENGTH;
 if DEE_UNLIKELY((result = DeeWideString_NewSized(MAX_COMPUTERNAME_LENGTH)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 if DEE_UNLIKELY(!GetComputerNameW(DeeWideString_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if DEE_UNLIKELY(DeeWideString_Resize(&result,result_size++) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   if DEE_UNLIKELY(!GetComputerNameW(DeeWideString_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetComputerNameW",error);
  goto err_r;
 }
 if (result_size != MAX_COMPUTERNAME_LENGTH &&
     DEE_UNLIKELY(DeeWideString_Resize(&result,result_size) != 0)
     ) goto err_r;
 return result;
#else
 DeeObject *result;
 if (DEE_LIKELY((result = DeeFS_Utf8GetMachineName()) != NULL) &&
     DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)&result) != 0)
     ) Dee_CLEAR(result);
 return result;
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetUserName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+UNLEN;
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized(UNLEN)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 if DEE_UNLIKELY(!GetUserNameA(DeeUtf8String_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,result_size-1) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   if DEE_UNLIKELY(!GetUserNameA(DeeUtf8String_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetUserNameA",error);
  goto err_r;
 }
 if (result_size != UNLEN &&
     DEE_UNLIKELY(DeeUtf8String_Resize(&result,result_size-1) != 0)
     ) goto err_r;
 return result;
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_HAVE_GETLOGIN_R
 DeeObject *result; int error; Dee_size_t result_size = 256;
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized(256)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
  if DEE_UNLIKELY(getlogin_r(DeeUtf8String_STR(result),result_size) == 0) break;
  error = errno; errno = 0;
  if (error == ENOMEM) {
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,(result_size *= 2)) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
  } else {
   DeeError_SystemErrorExplicit("getlogin_r",error);
   goto err_r;
  }
 }
 if DEE_UNLIKELY(DeeUtf8String_Resize(&result,
  Dee_Utf8StrLen(DeeUtf8String_STR(result))) != 0) goto err_r;
 return result;
#else
 char const *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if DEE_UNLIKELY((result = getlogin()) == NULL) {
  DeeError_SystemError("getlogin");
  return NULL;
 }
 return DeeUtf8String_New(result);
#endif
#else
 DeeReturn_STATIC_UTF8_STRING_EX(4,{'U','S','E','R'});
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideGetUserName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+UNLEN;
 if DEE_UNLIKELY((result = DeeWideString_NewSized(UNLEN)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 if DEE_UNLIKELY(!GetUserNameW(DeeWideString_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if DEE_UNLIKELY(DeeWideString_Resize(&result,result_size-1) != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   if DEE_UNLIKELY(!GetUserNameW(DeeWideString_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetUserNameW",error);
  goto err_r;
 }
 if (result_size != UNLEN &&
     DEE_UNLIKELY(DeeWideString_Resize(&result,result_size-1) != 0)
     ) goto err_r;
 return result;
#else
 DeeObject *result;
 if (DEE_LIKELY((result = DeeFS_Utf8GetUserName()) != NULL) &&
     DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)&result) != 0)
     ) Dee_CLEAR(result);
 return result;
#endif
}




#ifndef DEE_PLATFORM_WINDOWS
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8UidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name) {
 (void)uid,full_name;
 DeeError_TODONotImplemented(); // TODO
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WideUidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name) {
 (void)uid,full_name;
 DeeError_TODONotImplemented(); // TODO
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8GidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name) {
 (void)gid,full_name;
 DeeError_TODONotImplemented(); // TODO
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WideGidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name) {
 (void)gid,full_name;
 DeeError_TODONotImplemented(); // TODO
 return NULL;
}
#endif


DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeFS_ParseModeChange(
 DEE_A_IN_Z char const *mode, DEE_A_OUT Dee_modechange_t *result) {
 char const *mode_iter; Dee_uint8_t opkind;
 Dee_uint16_t channels,changes;
 DEE_ASSERT(mode),DEE_ASSERT(result);
 mode_iter = mode,channels = 0,changes = 0;
 opkind = DEE_MODECHANGE_KIND_NONE;
 while (1) switch (*mode_iter++) {
  case 'u': case 'U': channels |= 0700; break;
  case 'g': case 'G': channels |= 0070; break;
  case 'o': case 'O': channels |= 0007; break;
  default: --mode_iter; goto p_opkind;
 }
p_opkind:
 while (1) switch (*mode_iter++) {
  case '+': opkind = DEE_MODECHANGE_KIND_ADD; break;
  case '-': opkind = DEE_MODECHANGE_KIND_DEL; break;
  case '=': opkind = DEE_MODECHANGE_KIND_SET; break;
  default: --mode_iter; goto p_opflags;
 }
p_opflags:
 while (1) switch (*mode_iter++) {
  case 'r': case 'R': changes |= 0444; break;
  case 'w': case 'W': changes |= 0222; break;
  case 'x': case 'X': changes |= 0111; break;
  default: --mode_iter; goto end;
 }
end:
 if DEE_UNLIKELY(*mode_iter) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "chmod(%q) : Invalid chmod format",mode);
  return -1;
 }
 // If no channels were specified, all are affected
 if (!channels) channels = 0777;
 // Compile the final modechange, and store it in '*result'
 *result = DEE_MODECHANGE(channels&changes,opkind);
 return 0;
}


void _DeeFS_ExecModeChange(
 DEE_A_INOUT Dee_mode_t *mode, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mmc) Dee_modechange_t const *mcv) {
 Dee_modechange_t const *iter,*end; Dee_modechange_t change;
 end = (iter = mcv)+mcc;
 do {
  change = *iter++;
  switch (DEE_MODECHANGE_KIND(change)) {
   case DEE_MODECHANGE_KIND_SET:    *mode  = (Dee_mode_t)  DEE_MODECHANGE_BITS(change); break;
   case DEE_MODECHANGE_KIND_ADD:    *mode |= (Dee_mode_t)  DEE_MODECHANGE_BITS(change); break;
   case DEE_MODECHANGE_KIND_DEL:    *mode &= (Dee_mode_t)~(DEE_MODECHANGE_BITS(change)); break;
   case DEE_MODECHANGE_KIND_TOGGLE: *mode ^= (Dee_mode_t)  DEE_MODECHANGE_BITS(change); break;
   default: break;
  }
 } while (iter != end);
}



#if defined(DEE_PLATFORM_UNIX)
DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8PosixUserToUid(
 DEE_A_IN_Z Dee_Utf8Char const *user, DEE_A_OUT Dee_uid_t *uid) {
 struct passwd *pd;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 pd = getpwnam(user); // TODO: getpwnam_r
 if DEE_UNLIKELY(!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(%q) : %K",user,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 *uid = pd->pw_uid;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8PosixGroupToGid(
 DEE_A_IN_Z Dee_Utf8Char const *group, DEE_A_OUT Dee_gid_t *gid) {
 struct passwd *pd;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 pd = getpwnam(group); // TODO: getpwnam_r
 if DEE_UNLIKELY(!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(%q) : %K",group,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 *gid = pd->pw_gid;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_WidePosixUserToUid(
 DEE_A_IN_Z Dee_WideChar const *user, DEE_A_OUT Dee_uid_t *uid) {
 DeeObject *utf8_user; int result;
 if DEE_UNLIKELY((utf8_user = DeeUtf8String_FromWideString(user)) == NULL) return -1;
 result = DeeFS_Utf8PosixUserToUid(DeeUtf8String_STR(utf8_user),uid);
 Dee_DECREF(utf8_user);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_WidePosixGroupToGid(
 DEE_A_IN_Z Dee_WideChar const *group, DEE_A_OUT Dee_gid_t *gid) {
 DeeObject *utf8_group; int result;
 if DEE_UNLIKELY((utf8_group = DeeUtf8String_FromWideString(group)) == NULL) return -1;
 result = DeeFS_Utf8PosixGroupToGid(DeeUtf8String_STR(utf8_group),gid);
 Dee_DECREF(utf8_group);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_PosixUserToUidObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *user, DEE_A_OUT Dee_uid_t *uid) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(user)) {
  result = DeeFS_WidePosixUserToUid(DeeWideString_STR(user),uid);
 } else
#endif
 {
  if DEE_UNLIKELY((user = DeeUtf8String_Cast(user)) == NULL) return -1;
  result = DeeFS_Utf8PosixUserToUid(DeeUtf8String_STR(user),uid);
  Dee_DECREF(user);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_PosixGroupToGidObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *group, DEE_A_OUT Dee_gid_t *gid) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(group)) {
  result = DeeFS_WidePosixGroupToGid(DeeWideString_STR(group),gid);
 } else
#endif
 {
  if DEE_UNLIKELY((group = DeeUtf8String_Cast(group)) == NULL) return -1;
  result = DeeFS_Utf8PosixGroupToGid(DeeUtf8String_STR(group),gid);
  Dee_DECREF(group);
 }
 return result;
}
#endif


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT(-1) int _DeeFS_AutoGetUserAndGroup(
 DEE_A_IN DeeObject *arg1, DEE_A_IN_OPT DeeObject *arg2,
 DEE_A_OUT Dee_uid_t *uid, DEE_A_OUT Dee_gid_t *gid,
 DEE_A_OUT DeeObject **buf1, DEE_A_OUT DeeObject **buf2)
#else
DEE_A_RET_EXCEPT(-1) int _DeeFS_AutoGetUserAndGroup(
 DEE_A_IN DeeObject *arg1, DEE_A_IN_OPT DeeObject *arg2,
 DEE_A_OUT Dee_uid_t *uid, DEE_A_OUT Dee_gid_t *gid)
#endif
{
 char const *group_begin,*group_end;
 if (!arg2) {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(arg1,&DeeString_Type) != 0) return -1;
  group_end = (group_begin = DeeString_STR(arg1))+DeeString_SIZE(arg1);
  while (*group_begin != ':') if (group_begin++ == group_end) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "No group name found in %q",
                       DeeString_STR(arg1));
   return -1;
  }
  if DEE_UNLIKELY((arg2 = DeeString_NewWithLength((Dee_size_t)(
   group_begin-DeeString_STR(arg1)),DeeString_STR(arg1))) == NULL) return -1;
#ifdef DEE_PLATFORM_WINDOWS
  *buf1 = DeeFS_Win32NameToSID(DeeString_STR(arg2));
  Dee_DECREF(arg2);
  if DEE_UNLIKELY(!*buf1) return -1;
  *uid = DeeFSWin32SID_GetUid(*buf1);
  if DEE_UNLIKELY((*buf2 = DeeFS_Win32NameToSID(group_begin+1)) == NULL) { Dee_DECREF(*buf1); return -1; }
  *gid = DeeFSWin32SID_GetGid(*buf2);
#else
  {
   int result;
   result = DeeFS_PosixUserToUid(DeeString_STR(arg2),uid);
   Dee_DECREF(arg2);
   if DEE_UNLIKELY(result != 0) return result;
   return DeeFS_PosixGroupToGid(group_begin+1,gid);
  }
#endif
 } else {
  if (DeeString_Check(arg1)) {
#ifdef DEE_PLATFORM_WINDOWS
   if DEE_UNLIKELY((*buf1 = DeeFS_Win32NameToSID(DeeString_STR(arg1))) == NULL) return -1;
   *uid = DeeFSWin32SID_GetUid(*buf1);
#else
   if DEE_UNLIKELY(DeeFS_PosixUserToUid(DeeString_STR(arg1),uid) != 0) return -1;
#endif
  } else {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uid_t,arg1,uid) != 0) return -1;
#ifdef DEE_PLATFORM_WINDOWS
   *buf1 = NULL;
#endif
  }
  if (DeeString_Check(arg2)) {
#ifdef DEE_PLATFORM_WINDOWS
   if DEE_UNLIKELY((*buf2 = DeeFS_Win32NameToSID(DeeString_STR(arg2))) == NULL) goto err_2;
   *gid = DeeFSWin32SID_GetGid(*buf2);
#else
   if DEE_UNLIKELY(DeeFS_PosixGroupToGid(DeeString_STR(arg2),gid) != 0) return -1;
#endif
  } else {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_gid_t,arg2,gid) != 0) {
#ifdef DEE_PLATFORM_WINDOWS
err_2: Dee_XDECREF(*buf1);
#endif
    return -1;
   }
#ifdef DEE_PLATFORM_WINDOWS
   *buf2 = NULL;
#endif
  }
 }
 return 0;
}










DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeFSDirObject {
 DEE_OBJECT_HEAD
 union{
  DEE_A_REF DeeWideStringObject *dir_widepath; /*< [1..1] Directory to iterate. */
  DEE_A_REF DeeUtf8StringObject *dir_utf8path; /*< [1..1] Directory to iterate. */
  DEE_A_REF DeeAnyStringObject  *dir_path;     /*< [1..1] Directory to iterate. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define dir_widepath _dir_data.dir_widepath
#define dir_utf8path _dir_data.dir_utf8path
#define dir_path     _dir_data.dir_path
 _dir_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
};
struct DeeFSQueryObject {
 DEE_OBJECT_HEAD
 union{
  DEE_A_REF DeeWideStringObject *qry_widepath; /*< [1..1] Query to iterate. */
  DEE_A_REF DeeUtf8StringObject *qry_utf8path; /*< [1..1] Query to iterate. */
  DEE_A_REF DeeAnyStringObject  *qry_path;     /*< [1..1] Query to iterate. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define qry_widepath _qry_data.qry_widepath
#define qry_utf8path _qry_data.qry_utf8path
#define qry_path     _qry_data.qry_path
 _qry_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
};
struct DeeListObject;

#ifdef DEE_PLATFORM_WINDOWS
struct DeeFSDirIteratorWin32Data {
 HANDLE            diw32_handle; /*< Find-handle. */
 union{
  WIN32_FIND_DATAA diw32_data;   /*< Common find data (only use members with offset < Dee_OFFSETOF(WIN32_FIND_DATAA,cFileName)). */
  WIN32_FIND_DATAA diw32_utf8;   /*< UTF-8 Iterator data. */
  WIN32_FIND_DATAW diw32_wide;   /*< WIDE Iterator data. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define diw32_data _diw32_data.diw32_data
#define diw32_utf8 _diw32_data.diw32_utf8
#define diw32_wide _diw32_data.diw32_wide
 _diw32_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
};
#endif
#ifdef DEE_PLATFORM_UNIX
struct DeeFSDirIteratorUnixData {
 struct dirent *diu_dp;
 DIR           *diu_dfd;
};
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
struct DeeFSDirIteratorVFSData {
 DEE_A_REF struct DeeObject *div_elemlist; /*< [0..1] Element list. */
 int (DEE_CALL *div_yield)(DeeFSDirIteratorObject *self, DeeObject **result); /*< [1..1] Yield object generator. */
 void                       *div_elemiter; /*< [?..?] Element iterator. */
 void                       *div_elemend;  /*< [?..?] list end. */
};
#endif


struct DeeFSDirIteratorObject {
 DEE_OBJECT_HEAD
 Dee_uint32_t          di_flags;
#define DEE_FS_DIR_ITERATOR_FLAGS_NONE DEE_UINT32_C(0x00000000)
#define DEE_FS_DIR_ITERATOR_FLAGS_WIDE DEE_UINT32_C(0x00000001)
#define DEE_FS_DIR_ITERATOR_FLAGS_CTX0 DEE_UINT32_C(0x40000000)
#define DEE_FS_DIR_ITERATOR_FLAGS_CTX1 DEE_UINT32_C(0x80000000)
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_FS_DIR_ITERATOR_FLAGS_VFS  DEE_UINT32_C(0x00000002)
#endif
 union{
#ifdef DEE_PLATFORM_WINDOWS
  struct DeeFSDirIteratorWin32Data     di_win;
#endif
#ifdef DEE_PLATFORM_UNIX
  struct DeeFSDirIteratorUnixData      di_unix;
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
  struct DeeFSDirIteratorVFSData       di_vfs;
#endif
  int                                  di_unused;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#ifdef DEE_PLATFORM_WINDOWS
#define di_win _di_data.di_win
#endif
#ifdef DEE_PLATFORM_UNIX
#define di_unix _di_data.di_unix
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define di_vfs _di_data.di_vfs
#endif
 _di_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
};
DEE_COMPILER_MSVC_WARNING_POP

#ifdef DEE_PLATFORM_WINDOWS
# define DeeFSDirIterator_DONE(ob) (((DeeFSDirIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_win.diw_handle == INVALID_HANDLE_VALUE)
#elif defined(DEE_PLATFORM_UNIX)
# define DeeFSDirIterator_DONE(ob) (((DeeFSDirIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_unix.diu_dfd == NULL)
#else
# define DeeFSDirIterator_DONE(ob) 1
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFileIO_ListDir(DEE_A_INOUT_OBJECT(DeeFileIOObject) *path) {
 DeeObject *pathname,*result;
 DEE_ASSERT(DeeObject_Check(path) && DeeFileIO_Check(path));
 // TODO: fopendir support
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY((pathname = DeeFileIO_WideFilename(path)) == NULL) return NULL;
#else
 if DEE_UNLIKELY((pathname = DeeFileIO_Utf8Filename(path)) == NULL) return NULL;
#endif
 result = _DeeFS_ListDirObject(pathname);
 Dee_DECREF(pathname);
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFSDirIterator_WideInit(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFSDirIterator_Utf8Init(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DEE_A_IN_Z Dee_Utf8Char const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  int temp; struct DeeVFSFile vfsfile; DeeObject *re_fslink;
  if DEE_UNLIKELY((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&vfsfile,&re_fslink)) < 0) return temp;
  if (temp == 0) {
   if (re_fslink) {
    temp = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(re_fslink));
    Dee_DECREF(re_fslink);
    return temp;
   }
   if DEE_UNLIKELY(!vfsfile.vf_enum) {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "Cannot enumerate virtual path: %q",path);
    return -1;
   }
   return (*vfsfile.vf_enum)(&vfsfile,self);
  }
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
  Dee_Utf8Char *search_path,*search_end; Dee_size_t path_len; int result;
  DEE_ASSERT(DeeObject_Check(self) && (DeeFSDirIterator_Check(self)
                                  || DeeFSQueryIterator_Check(self)));
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
  search_path = (Dee_Utf8Char *)alloca(((path_len = Dee_Utf8StrLen(path))+3)*sizeof(Dee_Utf8Char));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
  while DEE_UNLIKELY((search_path = (Dee_Utf8Char *)malloc_nz(((
   path_len = Dee_Utf8StrLen(path))+3)*sizeof(Dee_Utf8Char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(search_path,path,path_len*sizeof(Dee_Utf8Char));
  search_end = search_path+path_len;
  while (search_end != search_path && IS_SEP(search_end[-1])) --search_end;
  *search_end++ = SEP;
  *search_end++ = '*';
  *search_end++ = '\0';
  self->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_NONE;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(search_end);
#endif
   return -1;
  }
  self->di_win.diw32_handle = FindFirstFileA(search_path,&self->di_win.diw32_utf8);
  if DEE_UNLIKELY(self->di_win.diw32_handle == INVALID_HANDLE_VALUE) {
   result = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(result) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
#if !DEE_HAVE_ALLOCA
    free_nn(search_end);
#endif
    if DEE_UNLIKELY((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    // Sorry, but this one will (hopefully) yield widestrings...
    result = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(temp));
    Dee_DECREF(temp);
    return result;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "FindFirstFileA(%q) : %K",search_path,
                       DeeSystemError_Win32ToString((unsigned long)result));
   result = -1;
  } else {
   result = 0;
  }
#if !DEE_HAVE_ALLOCA
  free_nn(search_end);
#endif
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  DEE_ASSERT(DeeObject_Check(self) && (DeeFSDirIterator_Check(self)
                                  || DeeFSQueryIterator_Check(self)));
  self->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_NONE;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if DEE_UNLIKELY((self->di_unix.diu_dfd = opendir(path)) == NULL) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "opendir(%q) : %K",path,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  (void)self,path;
  return 0;
#endif
 }
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFSDirIterator_WideInit(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DEE_A_IN_Z Dee_WideChar const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  int temp; struct DeeVFSFile vfsfile; DeeObject *re_fslink;
  if DEE_UNLIKELY((temp = DeeVFS_WideLocateOrReadReFsLink(path,&vfsfile,&re_fslink)) < 0) return temp;
  if (temp == 0) {
   if (re_fslink) {
    temp = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(re_fslink));
    Dee_DECREF(re_fslink);
    return temp;
   }
   if DEE_UNLIKELY(!vfsfile.vf_enum) {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "Cannot enumerate virtual path: %lq",path);
    return -1;
   }
   return (*vfsfile.vf_enum)(&vfsfile,self);
  }
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
  Dee_WideChar *search_path,*search_end; Dee_size_t path_len; int result;
  DEE_ASSERT(DeeObject_Check(self) && (DeeFSDirIterator_Check(self)
                                  || DeeFSQueryIterator_Check(self)));
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
  search_path = (Dee_WideChar *)alloca(((path_len = Dee_WideStrLen(path))+3)*sizeof(Dee_WideChar));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
  while DEE_UNLIKELY((search_path = (Dee_WideChar *)malloc_nz(((
   path_len = Dee_WideStrLen(path))+3)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(search_path,path,path_len*sizeof(Dee_WideChar));
  search_end = search_path+path_len;
  while (search_end != search_path && IS_SEP(search_end[-1])) --search_end;
  *search_end++ = SEP;
  *search_end++ = '*';
  *search_end++ = '\0';
  self->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_WIDE;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(search_end);
#endif
   return -1;
  }
  self->di_win.diw32_handle = FindFirstFileW(search_path,&self->di_win.diw32_wide);
  if DEE_UNLIKELY(self->di_win.diw32_handle == INVALID_HANDLE_VALUE) {
   result = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(result)
   && !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
#if !DEE_HAVE_ALLOCA
    free_nn(search_end);
#endif
    if DEE_UNLIKELY((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    result = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(temp));
    Dee_DECREF(temp);
    return result;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "FindFirstFileW(%lq) : %K",
                       search_path,DeeSystemError_Win32ToString((unsigned long)result));
   result = -1;
  } else {
   result = 0;
  }
#if !DEE_HAVE_ALLOCA
  free_nn(search_end);
#endif
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(path_ob));
  if DEE_LIKELY(result == 0) self->di_flags |= DEE_FS_DIR_ITERATOR_FLAGS_WIDE;
  Dee_DECREF(path_ob);
  return result;
#endif
 }
}
DEE_STATIC_INLINE(int) _DeeFSDirIterator_Init(
 struct DeeFSDirIteratorObject *self, DeeAnyStringObject *path) {
 int result;
 if (DeeWideString_Check(path)) {
  result = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(path));
 } else {
  if DEE_UNLIKELY((path = (DeeAnyStringObject *)DeeUtf8String_Cast((DeeObject *)path)) == NULL) return -1;
  result = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define _DeeFSDirIterator_Visit(ob) \
do{if (((ob)->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_VFS)!=0) {\
 DEE_ASSERT(!(ob)->di_vfs.div_elemlist || DeeObject_Check((ob)->di_vfs.div_elemlist));\
 Dee_XVISIT((ob)->di_vfs.div_elemlist);\
}}while(0)
#else
#define _DeeFSDirIterator_Visit(ob) do{}while(0)
#endif

static void _DeeFSDirIterator_Quit(DeeFSDirIteratorObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_VFS)!=0) {
  DEE_ASSERT(!self->di_vfs.div_elemlist || DeeObject_Check(self->di_vfs.div_elemlist));
  Dee_XDECREF(self->di_vfs.div_elemlist);
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
 {
#if defined(DEE_PLATFORM_WINDOWS)
  if (self->di_win.diw32_handle != INVALID_HANDLE_VALUE &&
      DEE_UNLIKELY(!FindClose(self->di_win.diw32_handle))) SetLastError(0);
#elif defined(DEE_PLATFORM_UNIX)
  if (self->di_unix.diu_dfd &&
      DEE_UNLIKELY(closedir(self->di_unix.diu_dfd) != 0)) errno = 0;
#endif
 }
}
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define _pdeefsdiriterator_tp_visit &_deefsdiriterator_tp_visit
DEE_VISIT_PROC(_deefsdiriterator_tp_visit,DeeFSDirIteratorObject *self) {
 _DeeFSDirIterator_Visit(self);
}
#else
#define _pdeefsdiriterator_tp_visit DeeType_DEFAULT_SLOT(tp_visit)
#endif

static int _DeeFSDirIterator_Yield(DeeFSDirIteratorObject *self, DeeObject **result) {
#if defined(DEE_PLATFORM_WINDOWS)
 DEE_ASSERT(DeeObject_Check(self) && (DeeFSDirIterator_Check(self)
                                 || DeeFSQueryIterator_Check(self)));
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_VFS)!=0) {
  int error;
  DEE_ASSERT(self->di_vfs.div_yield);
  if DEE_LIKELY((error = (*self->di_vfs.div_yield)(self,result)) == 0) {
   if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0) {
    if DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)result) != 0) {
err_r: Dee_DECREF(*result); return -1;
    }
   } else {
    if DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)result) != 0) goto err_r;
   }
  }
  return error;
 } else
#endif
 {
  if (self->di_win.diw32_handle == INVALID_HANDLE_VALUE) return 1;
  if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0) {
again_wide:
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   if DEE_UNLIKELY(!FindNextFileW(self->di_win.diw32_handle,&self->di_win.diw32_wide)) {
    DWORD error = DeeSystemError_Win32Consume();
    if (error == ERROR_NO_MORE_FILES) {
     self->di_win.diw32_handle = INVALID_HANDLE_VALUE;
     return 1; // Iterator end
    }
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "FindNextFileW(%p) : %K",
                        self->di_win.diw32_handle,
                        DeeSystemError_Win32ToString(error));
    return -1;
   }
   if (self->di_win.diw32_wide.cFileName[0] == '.' && (
       self->di_win.diw32_wide.cFileName[1] == 0 || (
       self->di_win.diw32_wide.cFileName[1] == '.' &&
       self->di_win.diw32_wide.cFileName[2] == 0))) goto again_wide;
   if DEE_UNLIKELY((*result = DeeWideString_New(self->di_win.diw32_wide.cFileName)) == NULL) return -1;
   return 0;
  } else {
again_utf8:
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   if DEE_UNLIKELY(!FindNextFileA(self->di_win.diw32_handle,&self->di_win.diw32_utf8)) {
    DWORD error = DeeSystemError_Win32Consume();
    if (error == ERROR_NO_MORE_FILES) {
     self->di_win.diw32_handle = INVALID_HANDLE_VALUE;
     return 1; // Iterator end
    }
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "FindNextFileA(%p) : %K",
                        self->di_win.diw32_handle,
                        DeeSystemError_Win32ToString(error));
    return -1;
   }
   if (self->di_win.diw32_utf8.cFileName[0] == '.' && (
       self->di_win.diw32_utf8.cFileName[1] == 0 || (
       self->di_win.diw32_utf8.cFileName[1] == '.' &&
       self->di_win.diw32_utf8.cFileName[2] == 0))) goto again_utf8;
   if ((*result = DeeUtf8String_New(self->di_win.diw32_utf8.cFileName)) == NULL) return -1;
   return 0;
  }
 }
#elif defined(DEE_PLATFORM_UNIX)
again:
 if DEE_UNLIKELY(!self->di_unix.diu_dfd) return 1;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 errno = 0; // ... NULL is returned and errno is not changed ...
 if ((self->di_unix.diu_dp = readdir(self->di_unix.diu_dfd)) == NULL) {
  int error = errno;
  if (error != 0) { // error 0 means end-of-directory
   DeeError_SystemErrorExplicit("readdir",error);
   errno = 0;
   return -1;
  }
  self->di_unix.diu_dp = NULL;
  return 1;
 }
 if (!self->di_unix.diu_dp->d_name ||
     (self->di_unix.diu_dp->d_name[0] == '.' && (self->di_unix.diu_dp->d_name[1] == 0 || (
      self->di_unix.diu_dp->d_name[1] == '.' && self->di_unix.diu_dp->d_name[2] == 0
      )))) goto again; // Skip "." and ".."
#ifdef _DIRENT_HAVE_D_NAMLEN
 if DEE_UNLIKELY((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
  ? DeeWideString_FromUtf8StringWithLength(self->di_unix.diu_dp->d_namlen,self->di_unix.diu_dp->d_name)
  : DeeUtf8String_FromUtf8StringWithLength(self->di_unix.diu_dp->d_namlen,self->di_unix.diu_dp->d_name)
  ) == NULL) return -1;
#elif defined(_D_EXACT_NAMLEN)
 if DEE_UNLIKELY((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
  ? DeeWideString_FromUtf8StringWithLength(_D_EXACT_NAMLEN(self->di_unix.diu_dp),self->di_unix.diu_dp->d_name)
  : DeeUtf8String_FromUtf8StringWithLength(_D_EXACT_NAMLEN(self->di_unix.diu_dp),self->di_unix.diu_dp->d_name)
  ) == NULL) return -1;
#else
 if DEE_UNLIKELY((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
  ? DeeWideString_FromUtf8String(self->di_unix.diu_dp->d_name)
  : DeeUtf8String_FromUtf8String(self->di_unix.diu_dp->d_name)
  ) == NULL) return -1;
#endif
 return 0;
#else
 (void)self;
 return 1;
#endif
}


//////////////////////////////////////////////////////////////////////////
// Query
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeFSQueryIteratorObject {
 DeeFSDirIteratorObject          qit_base;
 union{
  DEE_A_REF DeeUtf8StringObject *qit_utf8pattern;
  DEE_A_REF DeeWideStringObject *qit_widepattern;
  DEE_A_REF DeeAnyStringObject  *qit_pattern;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define qit_utf8pattern _qit_data.qit_utf8pattern
#define qit_widepattern _qit_data.qit_widepattern
#define qit_pattern     _qit_data.qit_pattern
 _qit_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
};
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(int) _DeeFSQueryIterator_Utf8Init(
 struct DeeFSQueryIteratorObject *self, DeeUtf8StringObject *query) {
 Dee_Utf8Char *path_begin,*path_end,*path; Dee_size_t path_size; int temp;
 path_end = (path_begin = DeeUtf8String_STR(query))+DeeUtf8String_SIZE(query);
 while (path_end != path_begin && !IS_SEP(path_end[-1])) --path_end;
 if (path_end == path_begin) {
  // Query the current directory
  static Dee_Utf8Char name_cwd[] = {'.',0};
  if DEE_UNLIKELY(_DeeFSDirIterator_Utf8Init(&self->qit_base,name_cwd) != 0) return -1;
  Dee_INCREF(self->qit_utf8pattern = query);
 } else {
  if DEE_UNLIKELY((self->qit_utf8pattern = (DeeUtf8StringObject *)DeeUtf8String_New(path_end)) == NULL) return -1;
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
  path = (Dee_Utf8Char *)alloca(((path_size = (Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_Utf8Char));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
  while DEE_UNLIKELY((path = (Dee_Utf8Char *)malloc_nz(((path_size = (
   Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_Utf8Char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   Dee_DECREF(self->qit_utf8pattern);
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(path,path_begin,path_size*sizeof(Dee_Utf8Char));
  path[path_size] = 0;
  temp = _DeeFSDirIterator_Utf8Init(&self->qit_base,path);
#if !DEE_HAVE_ALLOCA
  free_nn(path);
#endif
  if DEE_UNLIKELY(temp != 0) {
   Dee_DECREF(self->qit_utf8pattern);
   return temp;
  }
 }
 return 0;
}
DEE_STATIC_INLINE(int) _DeeFSQueryIterator_WideInit(
 struct DeeFSQueryIteratorObject *self, DeeWideStringObject *query) {
 Dee_WideChar *path_begin,*path_end,*path; Dee_size_t path_size; int temp;
 path_end = (path_begin = DeeWideString_STR(query))+DeeWideString_SIZE(query);
 while (path_end != path_begin && !IS_SEP(path_end[-1])) --path_end;
 if (path_end == path_begin) {
  // Query the current directory
  static Dee_WideChar name_cwd[] = {'.',0};
  if DEE_UNLIKELY(_DeeFSDirIterator_WideInit(&self->qit_base,name_cwd) != 0) return -1;
  Dee_INCREF(self->qit_widepattern = query);
 } else {
  if ((self->qit_widepattern = (DeeWideStringObject *)DeeWideString_New(path_end)) == NULL) return -1;
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
  path = (Dee_WideChar *)alloca(((path_size = (Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_WideChar));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
  while DEE_UNLIKELY((path = (Dee_WideChar *)malloc_nz(((path_size = (
   Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   Dee_DECREF(self->qit_widepattern);
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(path,path_begin,path_size*sizeof(Dee_WideChar));
  path[path_size] = 0;
  temp = _DeeFSDirIterator_WideInit(&self->qit_base,path);
#if !DEE_HAVE_ALLOCA
  free_nn(path);
#endif
  if DEE_UNLIKELY(temp != 0) {
   Dee_DECREF(self->qit_widepattern);
   return temp;
  }
 }
 return 0;
}
DEE_STATIC_INLINE(int) _DeeFSQueryIterator_Init(
 struct DeeFSQueryIteratorObject *self, DeeAnyStringObject const *query) {
 int result;
 if (DeeWideString_Check(query)) {
  result = _DeeFSQueryIterator_WideInit(self,(DeeWideStringObject *)query);
 } else {
  if DEE_UNLIKELY((query = (DeeAnyStringObject *)DeeUtf8String_Cast((DeeObject *)query)) == NULL) return -1;
  result = _DeeFSQueryIterator_Utf8Init(self,(DeeUtf8StringObject *)query);
  Dee_DECREF(query);
 }
 return result;
}

static void _deefsqueryiterator_tp_dtor(struct DeeFSQueryIteratorObject *self) {
 Dee_DECREF(self->qit_pattern);
 _DeeFSDirIterator_Quit(&self->qit_base);
}
DEE_VISIT_PROC(_deefsqueryiterator_tp_visit,struct DeeFSQueryIteratorObject *self) {
 Dee_VISIT(self->qit_pattern);
 _DeeFSDirIterator_Visit(&self->qit_base);
}

static int _DeeFSQueryIterator_Yield(struct DeeFSQueryIteratorObject *self, DeeObject **result) {
 int error;
 while DEE_LIKELY((error = _DeeFSDirIterator_Yield(&self->qit_base,result)) == 0) {
  // Check if our query patter matches the underlying iterator's yield-name
  // NOTE: On windows we don't use window's builtin wild-match functionality,
  //       as that one doesn't work correctly/as expected in certain situations.
  if (DeeWideString_Check(*result)
#ifdef DEE_PLATFORM_WINDOWS
      ? Dee_WideStrCaseWMatch(DeeWideString_STR(*result),DeeWideString_STR(self->qit_utf8pattern))
      : Dee_Utf8StrCaseWMatch(DeeUtf8String_STR(*result),DeeUtf8String_STR(self->qit_widepattern))
#else
      ? Dee_WideStrWMatch(DeeWideString_STR(*result),DeeWideString_STR(self->qit_utf8pattern))
      : Dee_Utf8StrWMatch(DeeUtf8String_STR(*result),DeeUtf8String_STR(self->qit_widepattern))
#endif
      ) break;
  Dee_DECREF(*result);
 }
 return error;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeFSDirObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%s)",path)) != NULL) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if DEE_UNLIKELY((result->dir_utf8path = (DeeUtf8StringObject *)DeeUtf8String_New(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeFSDirObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%ls)",path)) != NULL) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if DEE_UNLIKELY((result->dir_widepath = (DeeWideStringObject *)DeeWideString_New(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeFSDirObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%p)",path)) != NULL) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if (DeeWideString_Check(path)) Dee_INCREF(path);
  else if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   return NULL;
  }
  Dee_INHERIT_REF(result->dir_path,*(DeeAnyStringObject **)&path);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *query) {
 DeeFSQueryObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%s)",query)) != NULL) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if DEE_UNLIKELY((result->qry_utf8path = (DeeUtf8StringObject *)DeeUtf8String_New(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *query) {
 DeeFSQueryObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%ls)",query)) != NULL) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if DEE_UNLIKELY((result->qry_widepath = (DeeWideStringObject *)DeeWideString_New(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *query) {
 DeeFSQueryObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%p)",query)) != NULL) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if (DeeWideString_Check(query)) Dee_INCREF(query);
  else if DEE_UNLIKELY((query = DeeUtf8String_Cast(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   return NULL;
  }
  Dee_INHERIT_REF(result->qry_path,*(DeeAnyStringObject **)&query);
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *path_ob,*result;
 if DEE_UNLIKELY((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *path_ob,*result;
 if DEE_UNLIKELY((path_ob = DeeFS_WidePathExpand(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *path_ob,*result;
 if DEE_UNLIKELY((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *path_ob,*result;
 if DEE_UNLIKELY((path_ob = DeeFS_WidePathExpand(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path);
 Dee_DECREF(path);
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Dir VTable
static int _deefsdir_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeFSDirObject *self, DeeFSDirObject *right) {
 Dee_INCREF(self->dir_path = right->dir_path);
 return 0;
}
static int _deefsdir_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeFSDirObject *self, DeeObject *args) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dir",&path) != 0) return -1;
 if (DeeFileIO_Check(path)) {
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY((path = DeeFileIO_WideFilename(path)) == NULL) return -1;
#else
  if DEE_UNLIKELY((path = DeeFileIO_Filename(path)) == NULL) return -1;
#endif
 } else {
  if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
  if (!DeeWideString_Check(path) && !DeeUtf8String_Check(path) &&
      DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&path) != 0)) {
   Dee_DECREF(path);
   return -1;
  }
 }
 Dee_INHERIT_REF(self->dir_path,*(DeeAnyStringObject **)&path);
 return 0;
}
static int _deefsquery_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFSQueryObject *self, DeeObject *args) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:query",&path) != 0) return -1;
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
 if (!DeeWideString_Check(path) && !DeeUtf8String_Check(path) &&
     DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&path) != 0)) {
  Dee_DECREF(path);
  return -1;
 }
 Dee_INHERIT_REF(self->qry_path,*(DeeAnyStringObject **)&path);
 return 0;
}
static void _deefsdir_tp_dtor(DeeFSDirObject *self) { Dee_DECREF(self->dir_path); }
DEE_VISIT_PROC(_deefsdir_tp_visit,DeeFSDirObject *self) { Dee_VISIT(self->dir_path); }
static DeeObject *_deefsdir_tp_str(DeeFSDirObject *self) { return DeeString_Newf("<dir: %r>",self->dir_path); }
static DeeObject *_deefsquery_tp_str(DeeFSQueryObject *self) { return DeeString_Newf("<query: %r>",self->qry_path); }
static DeeObject *_deefsdir_tp_seq_iter_self(DeeFSDirObject *self) {
 DeeFSDirIteratorObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOCF(DeeFSDirIteratorObject,
  "dir.iterator(%p)",self)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeFSDirIterator_Type);
 if DEE_UNLIKELY(_DeeFSDirIterator_Init(result,self->dir_path) != 0) {
  _DeeObject_DELETE(&DeeFSDirIterator_Type,result);
  result = NULL;
 }
 return (DeeObject *)result;
}
static DeeObject *_deefsquery_tp_seq_iter_self(DeeFSDirObject *self) {
 struct DeeFSQueryIteratorObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOCF(
  struct DeeFSQueryIteratorObject,"query.iterator(%p)",self)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeFSQueryIterator_Type);
 if DEE_UNLIKELY(_DeeFSQueryIterator_Init(result,self->dir_path) != 0) {
  _DeeObject_DELETE(&DeeFSQueryIterator_Type,result);
  result = NULL;
 }
 return (DeeObject *)result;
}


static int _deefsdiriterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 struct DeeFSDirIteratorObject *self, DeeObject *args) {
 DeeObject *arg; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dir.iterator",&arg) != 0) return -1;
 if (DeeFSDir_Check(arg)) {
  result = _DeeFSDirIterator_Init(self,((DeeFSDirObject *)arg)->dir_path);
 } else if (DeeFileIO_Check(arg)) {
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY((arg = DeeFileIO_WideFilename(arg)) == NULL) return -1;
#else
  if DEE_UNLIKELY((arg = DeeFileIO_Filename(arg)) == NULL) return -1;
#endif
  result = _DeeFSDirIterator_Init(self,(DeeAnyStringObject *)arg);
  Dee_DECREF(arg);
 } else {
  result = _DeeFSDirIterator_Init(self,(DeeAnyStringObject *)arg);
 }
 return result;
}
static int _deefsqueryiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), struct DeeFSQueryIteratorObject *self, DeeObject *args) {
 DeeObject *arg;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dir.iterator",&arg) != 0) return -1;
 return _DeeFSQueryIterator_Init(self,(DeeAnyStringObject *)arg);
}


static struct DeeMemberDef const _deefsdir_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeFSDirIterator_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deefsquery_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeFSQueryIterator_Type),
 DEE_MEMBERDEF_END_v100
};

#if 0
static DEE_A_RET_EXCEPT(-1) int _DeeFSDirIterator_GetTimes(
 DEE_A_IN DeeFSDirIteratorObject *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(DeeTimeObject) **tm_modification) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFSDirIterator_Check(self));
 if DEE_UNLIKELY(DeeFSDirIterator_DONE(self)) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Iterator doesn't point to any file/dir");
  return -1;
 }
#if defined(DEE_PLATFORM_WINDOWS)
 if (tm_access && DEE_UNLIKELY((*tm_access = DeeTime_NewFromWin32FileTime(
  &self->di_win.diw_data.ftLastAccessTime)) == NULL)) return -1;
 if (tm_creation && DEE_UNLIKELY((*tm_creation = DeeTime_NewFromWin32FileTime(
  &self->di_win.diw_data.ftCreationTime)) == NULL)) {
err_tm_a: if (tm_access) Dee_DECREF(*tm_access); return -1;
 }
 if (tm_modification && DEE_UNLIKELY((*tm_modification = DeeTime_NewFromWin32FileTime(
  &self->di_win.diw_data.ftLastWriteTime)) == NULL)) {
/*err_tm_c:*/ if (tm_creation) Dee_DECREF(*tm_creation); goto err_tm_a;
 }
 return 0;
#elif defined(DEE_PLATFORM_UNIX)

#else                   
 DeeError_NotImplemented_str("_DeeFSDirIterator_GetTimes");
 return -1;
#endif
}
// DEE_GETSETDEF_CONST_v100("times",member(&_deefsdiriterator_times_get),null,member(&_deefsdiriterator_times_set),"times -> (time,time,time)\n"),
// DEE_GETSETDEF_CONST_v100("atime",member(&_deefsdiriterator_atime_get),null,member(&_deefsdiriterator_atime_set),"atime -> time\n"),
// DEE_GETSETDEF_CONST_v100("ctime",member(&_deefsdiriterator_ctime_get),null,member(&_deefsdiriterator_ctime_set),"ctime -> time\n"),
// DEE_GETSETDEF_CONST_v100("mtime",member(&_deefsdiriterator_mtime_get),null,member(&_deefsdiriterator_mtime_set),"mtime -> time\n"),
#endif



DeeTypeObject DeeFSDir_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dir"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFSDirObject),null,null,
  member(&_deefsdir_tp_copy_ctor),null,
  member(&_deefsdir_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deefsdir_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deefsdir_tp_str),
  member(&_deefsdir_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deefsdir_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_deefsdir_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
  member(_deefsdir_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeFSDirIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dir.iterator"),null,
  member(DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFSDirIteratorObject),null,null,null,null,
  member(&_deefsdiriterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_DeeFSDirIterator_Quit)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(_pdeefsdiriterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_DeeFSDirIterator_Yield)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeTypeObject DeeFSQuery_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("query"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFSDirObject),null,null,
  member(&_deefsdir_tp_copy_ctor),null,
  member(&_deefsquery_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deefsdir_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deefsquery_tp_str),
  member(&_deefsquery_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deefsdir_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_deefsquery_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
  member(_deefsquery_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeFSQueryIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("query.iterator"),null,
  member(DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFSQueryIteratorObject),null,null,null,null,
  member(&_deefsqueryiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deefsqueryiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deefsqueryiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_DeeFSQueryIterator_Yield)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#ifndef __INTELLISENSE__
#include "virtual_fs.c.inl"
#endif
#endif


#endif /* !GUARD_DEEMON_FS_API_C */
