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
#include <deemon/sys/sysinfo.h>
#include <deemon/sys/sysfs.h>
#include <deemon/sys/sysfd.h>
#include <deemon/unicode/char_traits.inl>

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
 return _DeeFS_Utf8Readlink(proc_temp);
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


#if defined(DeeSysInfo_Utf8GetHostname)\
 || defined(DeeSysInfo_WideGetHostname)
#define DEE_FS_HAVE_GETHOSTNAME
#endif

#ifndef DEE_FS_HAVE_GETHOSTNAME
DeeError_NEW_STATIC(_dee_notimplemented_gethostname,&DeeErrorType_NotImplemented,"gethostname");
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetHostname(void) {
#ifdef DeeSysInfo_Utf8GetHostname
 DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_Utf8GetHostname(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 return result;
#elif defined(DeeSysInfo_WideGetHostname)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_WideGetHostname(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_gethostname);
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideGetHostname(void) {
#ifdef DeeSysInfo_WideGetHostname
 DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_WideGetHostname(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 return result;
#elif defined(DeeSysInfo_Utf8GetHostname)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_Utf8GetHostname(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_gethostname);
 return NULL;
#endif
}


#if defined(DeeSysInfo_Utf8GetUsername)\
 || defined(DeeSysInfo_WideGetUsername)
#define DEE_FS_HAVE_GETUSERNAME
#endif

#ifndef DEE_FS_HAVE_GETUSERNAME
DeeError_NEW_STATIC(_dee_notimplemented_getusername,&DeeErrorType_NotImplemented,"getusername");
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetUsername(void) {
#ifdef DeeSysInfo_Utf8GetUsername
 DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_Utf8GetUsername(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 return result;
#elif defined(DeeSysInfo_WideGetUsername)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_WideGetUsername(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getusername);
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideGetUsername(void) {
#ifdef DeeSysInfo_WideGetUsername
 DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_WideGetUsername(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 return result;
#elif defined(DeeSysInfo_Utf8GetUsername)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeSysInfo_Utf8GetUsername(&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getusername);
 return NULL;
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



DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_API_C */
