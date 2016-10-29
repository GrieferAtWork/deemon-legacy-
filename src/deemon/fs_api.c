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
#include <aclapi.h>
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
#define _DeeVFS_Utf8IsVirtualPath(path) (DeeVFS_Utf8IsVirtualPath(path)&&!DeeFlag_NoVFS)
#define _DeeVFS_WideIsVirtualPath(path) (DeeVFS_WideIsVirtualPath(path)&&!DeeFlag_NoVFS)

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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpandObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpandObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
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
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8Win32IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideWin32IsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetTokenUserHome(DEE_A_IN HANDLE token);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetTokenUserHome(DEE_A_IN HANDLE token);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8Win32GetProcessUserHome(DEE_A_IN HANDLE process_handle);
static DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideWin32GetProcessUserHome(DEE_A_IN HANDLE process_handle);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Win32FGetFileType(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN HANDLE handle);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_WideWin32FGetFileType(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN HANDLE handle);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Win32FGetFileTypeObject(DEE_A_IN DeeObject const *path, DEE_A_IN HANDLE handle);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Win32GetFileType(DEE_A_IN_Z Dee_Utf8Char const *path);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_WideWin32GetFileType(DEE_A_IN_Z Dee_WideChar const *path);
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

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetModWithHandle(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetModWithHandle(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8SetModWithHandle(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideSetModWithHandle(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodStringToModeWithHandle(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodStringToModeWithHandle(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodStringToMode(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodStringToMode(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode);

#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Win32GetOwnWithHandle(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideWin32GetOwnWithHandle(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Win32SetOwnWithHandle(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideWin32SetOwnWithHandle(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Win32FSetFileTime(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN HANDLE handle, DEE_A_IN_OPT FILETIME const *tm_access, DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_WideWin32FSetFileTime(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN HANDLE handle, DEE_A_IN_OPT FILETIME const *tm_access, DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Win32SetFileTime(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT FILETIME const *tm_access, DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification);
static DEE_A_RET_EXCEPT(-1) int _DeeFS_WideWin32SetFileTime(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT FILETIME const *tm_access, DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification);
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) fixed_GetFileAttributesA(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT DWORD *attr);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) fixed_GetFileAttributesW(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT DWORD *attr);
#else /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
#ifdef _MSC_VER
#pragma warning(disable: 4701)
#endif
#define fixed_GetFileAttributesA(path,attr) (DeeThread_CheckInterrupt()!=0?-1:(*(attr)=GetFileAttributesA(path),0))
#define fixed_GetFileAttributesW(path,attr) (DeeThread_CheckInterrupt()!=0?-1:(*(attr)=GetFileAttributesW(path),0))
#endif /* !DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
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
 if ((dst = DeeFS_PathExpandObject(dst)) == NULL) return -1;
 if ((src = DeeFS_PathExpandObject(src)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(dst)) {
  if (DeeWideString_InplaceCast(&src) != 0) { result = -1; goto end_1; }
  result = _DeeFS_WideWin32HardLink(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&dst) != 0 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&src) != 0) { result = -1; goto end_1; }
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
 if ((result = DeeFS_Utf8PosixGetProcessName(process)) == NULL) return NULL;
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
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
DEE_A_RET_EXCEPT(-1) int DeeFS_ChDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideChDir(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8ChDir(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_ChDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideChDir(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8ChDir(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_GetHomeUserObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *user) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(user));
 if (DeeWideString_Check(user)) return DeeFS_WideGetHomeUser(DeeWideString_STR(user));
 if ((user = DeeUtf8String_Cast(user)) == NULL) return NULL;
 result = DeeFS_Utf8GetHomeUser(DeeUtf8String_STR(user));
 Dee_DECREF(user);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_GetTmpNameObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) {
 DeeObject *result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if ((prefix = DeeFS_PathExpandObject(prefix)) == NULL) { result = NULL; goto end_0; }
 if (DeeWideString_Check(path)) {
  if (DeeWideString_InplaceCast(&prefix) == -1) { result = NULL; goto end_1; }
  result = _DeeFS_WideGetTmpName(DeeWideString_STR(path),DeeWideString_STR(prefix));
 } else {
  if (DeeUtf8String_InplaceCast(&path) == -1 ||
      DeeUtf8String_InplaceCast(&prefix) == -1) { result = NULL; goto end_1; }
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
  if ((prefix = DeeWideString_Cast(prefix)) == NULL) return NULL;
  result = _DeeFS_WideGetTmpName(DeeWideString_STR(path),DeeWideString_STR(prefix));
  Dee_DECREF(prefix);
  return result;
 }
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 if ((prefix = DeeUtf8String_Cast(prefix)) == NULL) { result = NULL; goto end_0; }
 result = _DeeFS_Utf8GetTmpName(DeeUtf8String_STR(path),DeeUtf8String_STR(prefix));
 Dee_DECREF(prefix);
end_0: Dee_DECREF(path);
 return result;
}



DEE_A_RET_EXCEPT(-1) int DeeFS_SetListEnv(
 DEE_A_IN_OBJECT(struct DeeListObject) const *v) {
 DeeObject *old_env,**iter,**end,*elem;
 DEE_ASSERT(DeeObject_Check(v) && DeeList_Check(v));
 if ((v = DeeList_Copy(v)) == NULL) return -1;
 if ((old_env = DeeFS_ListEnv()) == NULL) {err_v: Dee_DECREF(v); return -1; }
 end = (iter = DeeList_ELEM(old_env))+DeeList_SIZE(old_env);
 while (iter != end) if (DeeFS_DelEnvObject(DeeTuple_GET(*iter,0)) == -1) {
  Dee_DECREF(old_env); goto err_v;
 } else ++iter;
 Dee_DECREF(old_env);
 end = (iter = DeeList_ELEM(v))+DeeList_SIZE(v);
 while (iter != end) {
  elem = *iter++;
  DEE_ASSERT(DeeObject_Check(elem) && DeeTuple_Check(elem));
  DEE_ASSERT(DeeTuple_SIZE(elem) == 2);
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(elem,0)));
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(elem,1)));
  if (DeeFS_SetEnvObject(DeeTuple_GET(elem,0),DeeTuple_GET(elem,1)) == -1) goto err_v;
 }
 Dee_DECREF(v);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_HasEnvObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(name))
  return DeeFS_WideHasEnv(DeeWideString_STR(name));
#endif
 if ((name = DeeUtf8String_Cast(name)) == NULL) return -1;
 result = DeeFS_Utf8HasEnv(DeeUtf8String_STR(name));
 Dee_DECREF(name);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_GetEnvObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) {
 DeeObject *result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(name))
  return DeeFS_WideGetEnv(DeeWideString_STR(name));
#endif
 if ((name = DeeUtf8String_Cast(name)) == NULL) return NULL;
 result = DeeFS_Utf8GetEnv(DeeUtf8String_STR(name));
 Dee_DECREF(name);
 return result;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeAnyStringObject) *DeeFS_TryGetEnvObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) {
 DeeObject *result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(name))
  return DeeFS_WideTryGetEnv(DeeWideString_STR(name));
#endif
 if ((name = DeeUtf8String_Cast(name)) == NULL) { DeeError_Handled(); return NULL; }
 result = DeeFS_Utf8TryGetEnv(DeeUtf8String_STR(name));
 Dee_DECREF(name);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_DelEnvObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(name))
  return DeeFS_WideDelEnv(DeeWideString_STR(name));
#endif
 if ((name = DeeUtf8String_Cast(name)) == NULL) return -1;
 result = DeeFS_Utf8DelEnv(DeeUtf8String_STR(name));
 Dee_DECREF(name);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_SetEnvObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *value) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(name)) {
  if ((value = DeeWideString_Cast(value)) == NULL) return -1;
  result = DeeFS_WideSetEnv(
   DeeWideString_STR(name),
   DeeWideString_STR(value));
  Dee_DECREF(value);
  return result;
 }
#endif
 if ((name = DeeUtf8String_Cast(name)) == NULL) return -1;
 if ((value = DeeUtf8String_Cast(value)) == NULL) { result = -1; goto err_0; }
 result = DeeFS_Utf8SetEnv(DeeUtf8String_STR(name),DeeUtf8String_STR(value));
 Dee_DECREF(value);
err_0: Dee_DECREF(name);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathHeadObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathHeadObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathHeadObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathTailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathTailObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathTailObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathFileObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathFileObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExtObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExtObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExtObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathDriveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathDriveObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathDriveObject(path);
 Dee_DECREF(path);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathIncTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathIncTrailObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathIncTrailObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExcTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExcTrailObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExcTrailObject(path);
 Dee_DECREF(path);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeFS_PathAbsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DeeObject *result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if ((cwd = DeeFS_PathExpandObject(cwd)) == NULL) { result = NULL; goto end_0; }
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
  if ((cwd = DeeWideString_Cast(cwd)) == NULL) return NULL;
  result = _DeeFS_WidePathAbsObject(path,cwd);
 } else {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
  if ((cwd = DeeUtf8String_Cast(cwd)) == NULL) { Dee_DECREF(path); return NULL; }
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
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 if ((cwd = DeeFS_PathExpandObject(cwd)) == NULL) { result = NULL; goto end_0; }
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
  if ((cwd = DeeWideString_Cast(cwd)) == NULL) return NULL;
  result = _DeeFS_WidePathRelObject(path,cwd);
 } else {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
  if ((cwd = DeeUtf8String_Cast(cwd)) == NULL) { Dee_DECREF(path); return NULL; }
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
  if ((path = DeeFS_WidePathExpandVarsObject(path)) == NULL) return NULL;
  result = DeeFS_WidePathExpandUserObject(path);
  Dee_DECREF(path);
 } else {
  if ((result = DeeUtf8String_Cast(path)) == NULL) return NULL;
  path = DeeFS_Utf8PathExpandVarsObject(result);
  Dee_DECREF(result);
  if (!path) return NULL;
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
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExpandUserObject(path);
 Dee_DECREF(path);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_PathExpandVarsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeWideString_Check(path)) return DeeFS_WidePathExpandVarsObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = DeeFS_Utf8PathExpandVarsObject(path);
 Dee_DECREF(path);
 return result;
}

#ifdef DEE_PLATFORM_UNIX
static int _deefs_posix_stat(char const *path, struct stat *attrib) {
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (stat(path,attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "stat(%q) : %K",path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
}
static int _deefs_posix_fstat(int fd, DeeObject *path, struct stat *attrib) {
#if DEE_HAVE_FSTAT
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fstat(fd,attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "fstat(%d:%k) : %K",fd,path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)fd;
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 if (DeeThread_CheckInterrupt() != 0) goto err_path;
 if (stat(DeeUtf8String_STR(path),attrib) != 0) {
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
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (stat(path,attrib) != 0) {
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
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (lstat(path,attrib) != 0) {
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


DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_GetATimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (_DeeFS_GetTimes2Object(path,&result,NULL,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_GetCTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (_DeeFS_GetTimes2Object(path,NULL,&result,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_GetMTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (_DeeFS_GetTimes2Object(path,NULL,NULL,&result) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_GetATimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeFS_GetTimes2Object(path,&result,NULL,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_GetCTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeFS_GetTimes2Object(path,NULL,&result,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_GetMTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if (DeeFS_GetTimes2Object(path,NULL,NULL,&result) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
_DeeFS_GetTimesObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *a,*c,*m,*result;
 if (_DeeFS_GetTimes2Object(path,&a,&c,&m) == -1) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
DeeFS_GetTimesObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *a,*c,*m,*result;
 if (DeeFS_GetTimes2Object(path,&a,&c,&m) == -1) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}


DEE_A_RET_EXCEPT(-1) int _DeeFS_GetTimes2Object(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
 int result;
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideGetTimes2(DeeWideString_STR(path),tm_access,tm_creation,tm_modification);
 } else {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8GetTimes2(DeeUtf8String_STR(path),tm_access,tm_creation,tm_modification);
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_GetTimes2Object(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideGetTimes2(DeeWideString_STR(path),tm_access,tm_creation,tm_modification);
 } else {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8GetTimes2(DeeUtf8String_STR(path),tm_access,tm_creation,tm_modification);
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_SetTimes2Object(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) {
 int result;
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideSetTimes2(DeeWideString_STR(path),tm_access,tm_creation,tm_modification);
 } else {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8SetTimes2(DeeUtf8String_STR(path),tm_access,tm_creation,tm_modification);
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_SetTimes2Object(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideSetTimes2(DeeWideString_STR(path),tm_access,tm_creation,tm_modification);
 } else {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8SetTimes2(DeeUtf8String_STR(path),tm_access,tm_creation,tm_modification);
 }
 Dee_DECREF(path);
 return result;
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsAbsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if (DeeWideString_Check(path)) return _DeeFS_WideIsAbsObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8IsAbsObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsAbsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if (DeeWideString_Check(path)) return DeeFS_WideIsAbsObject(path);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = DeeFS_Utf8IsAbsObject(path);
 Dee_DECREF(path);
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
static DEE_A_RET_EXCEPT(-1) int _DeeFS_Win32FGetFileTypeObject(
 DEE_A_IN DeeObject const *path, DEE_A_IN HANDLE handle) {
 int result;
 if (DeeWideString_Check(path)) return _DeeFS_WideWin32FGetFileType(DeeWideString_STR(path),handle);
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8Win32FGetFileType(DeeUtf8String_STR(path),handle);
 Dee_DECREF(path);
 return result;
}
#endif

#if DEE_PLATFORM_HAVE_IO
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsFile(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return DeeFileIO_HANDLE(fp) != DEE_FILEIO_INVALID_HANDLE;
#elif defined(DEE_PLATFORM_UNIX)
 struct stat attrib;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (_deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib) == -1) return -1;
 return S_ISREG(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsDir(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return DeeFileIO_HANDLE(fp) == DEE_FILEIO_INVALID_HANDLE;
#elif defined(DEE_PLATFORM_UNIX)
 struct stat attrib;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (_deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib) == -1) return -1;
 return S_ISDIR(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsLink(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return _DeeFS_IsLinkObject(DeeFileIO_FILE(fp));
#elif defined(DEE_PLATFORM_UNIX)
 struct stat attrib;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (_deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib) == -1) return -1;
 return S_ISLNK(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsDrive(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
#ifdef DEE_PLATFORM_WINDOWS
 return _DeeFS_IsDriveObject(DeeFileIO_FILE(fp));
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsMount(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return _DeeFS_IsMountObject(DeeFileIO_FILE(fp));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsHidden(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return _DeeFS_IsHiddenObject(DeeFileIO_FILE(fp));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsExecutable(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#if defined(DEE_PLATFORM_UNIX)
 struct stat attrib; int temp;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (_deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib) == -1) return -1;
 if (!S_ISREG(attrib.st_mode)) return 0;
 return (attrib.st_mode & 0111) != 0;
#else
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 return _DeeFS_IsExecutableObject(DeeFileIO_FILE(fp));
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsCharDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#if defined(DEE_PLATFORM_WINDOWS) && defined(FILE_TYPE_CHAR)
 int result = _DeeFS_Win32FGetFileTypeObject(DeeFileIO_FILE(fp),DeeFileIO_HANDLE(fp));
 return result == -1 ? -1 : result == FILE_TYPE_CHAR;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISCHR)
 struct stat attrib; int temp;
 if ((temp = _deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib)) == -1) return -1;
 return S_ISCHR(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsBlockDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#if defined(DEE_PLATFORM_WINDOWS) && (defined(FILE_TYPE_DISK) && 0)
 int result = _DeeFS_Win32FGetFileTypeObject(DeeFileIO_FILE(fp),DeeFileIO_HANDLE(fp));
 return result == -1 ? -1 : result == FILE_TYPE_DISK;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISBLK)
 struct stat attrib; int temp;
 if ((temp = _deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib)) == -1) return -1;
 return S_ISBLK(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsFiFo(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#if defined(DEE_PLATFORM_WINDOWS) && defined(FILE_TYPE_PIPE)
 int result = _DeeFS_Win32FGetFileTypeObject(DeeFileIO_FILE(fp),DeeFileIO_HANDLE(fp));
 return result == -1 ? -1 : result == FILE_TYPE_PIPE;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISFIFO)
 struct stat attrib; int temp;
 if ((temp = _deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib)) == -1) return -1;
 return S_ISFIFO(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsSocket(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) {
#if defined(DEE_PLATFORM_WINDOWS) && defined(FILE_TYPE_REMOTE)
 int result = _DeeFS_Win32FGetFileTypeObject(DeeFileIO_FILE(fp),DeeFileIO_HANDLE(fp));
 return result == -1 ? -1 : result == FILE_TYPE_REMOTE;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISSOCK)
 struct stat attrib; int temp;
 if ((temp = _deefs_posix_fstat(DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),&attrib)) == -1) return -1;
 return S_ISSOCK(attrib.st_mode);
#else
 (void)fp;
 return 0;
#endif
}
#endif




DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsFileObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsFile(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsFile(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsFileObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsFile(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsFile(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsDir(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsDir(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsDir(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsDir(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsLinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsLink(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsLink(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsLinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsLink(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsLink(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsHiddenObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsHidden(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsHidden(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsHiddenObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsHidden(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsHidden(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsExecutableObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsExecutable(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsExecutable(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsExecutableObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsExecutable(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsExecutable(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsDriveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideIsDrive(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8IsDrive(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsDriveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return DeeFS_WideWin32IsDriveObject(path);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = DeeFS_Utf8Win32IsDriveObject(path);
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsMountObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideIsMount(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8IsMount(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsMountObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return DeeFS_WideIsMount(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = DeeFS_Utf8IsMount(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsCharDevObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_CHAR_DEV
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsCharDev(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsCharDev(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsCharDevObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_CHAR_DEV
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsCharDev(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsCharDev(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsBlockDevObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_BLOCK_DEV
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsBlockDev(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsBlockDev(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsBlockDevObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_BLOCK_DEV
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsBlockDev(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsBlockDev(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsFiFoObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_NAMED_PIPE
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsFiFo(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsFiFo(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsFiFoObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_NAMED_PIPE
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsFiFo(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsFiFo(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_IsSocketObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_SOCKET
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsSocket(DeeWideString_STR(path));
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsSocket(DeeUtf8String_STR(path));
  Dee_DECREF(path);
 }
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_IsSocketObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
#ifdef DEE_FS_HAVE_IS_SOCKET
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideIsSocket(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = -1; goto end; }
  result = _DeeFS_Utf8IsSocket(DeeUtf8String_STR(path));
 }
end: Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_ExistsObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideExists(DeeWideString_STR(path));
 } else 
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8Exists(DeeUtf8String_STR(path));
 }
 Dee_DECREF(path);
 return result;
}

#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_NOEXCEPT(0) int _Dee_Win32AcquireProcessPrivilege(
 DEE_A_IN_Z Dee_WideChar const *name) {
 HANDLE tok; LUID luid; TOKEN_PRIVILEGES tok_priv; DWORD error;
 DEE_LVERBOSE1("_Dee_Win32AcquireProcessPrivilege(%lq)\n",name);
 if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&tok)) {err0: SetLastError(0); return 0; }
 if (!LookupPrivilegeValueW(NULL,(LPCWSTR)name,&luid)) goto err0;
 tok_priv.PrivilegeCount = 1;
 tok_priv.Privileges[0].Luid = luid;
 tok_priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
 if (!AdjustTokenPrivileges(tok,FALSE,&tok_priv,0,NULL,NULL)) goto err0;
 error = GetLastError(); SetLastError(0);
 return error == ERROR_NOT_ALL_ASSIGNED ? 0 : 1;
}
static void _deefs_win32_enable_symlink(void) {
 DEE_ATOMIC_ONCE({
  static Dee_WideChar const priv[] = {
   'S','e','C','r','e','a','t','e','S','y','m','b','o','l','i',
   'c','L','i','n','k','P','r','i','v','i','l','e','g','e','\0'};
  (void)_Dee_Win32AcquireProcessPrivilege(priv);
 });
}
#endif



DEE_A_RET_EXCEPT(-1) int DeeFS_RemoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideRemove(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast(&path) == -1) return -1;
  result = _DeeFS_Utf8Remove(DeeUtf8String_STR(path));
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_RmFileObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideRmFile(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast(&path) == -1) return -1;
  result = _DeeFS_Utf8RmFile(DeeUtf8String_STR(path));
 }
 Dee_DECREF(path);
 return result;
}
#define QTOA972 mberGenera
DEE_A_RET_EXCEPT(-1) int _DeeFS_RemoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideRemove(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8Remove(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_RmFileObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) result = _DeeFS_WideRmFile(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8RmFile(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_MkDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideMkDir(DeeWideString_STR(path),mode);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8MkDir(DeeUtf8String_STR(path),mode);
 Dee_DECREF(path);
 return result;
}
#define QTOE651 tor
DEE_A_RET_EXCEPT(-1) int DeeFS_MkDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideMkDir(DeeWideString_STR(path),mode);
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast(&path) == -1) return -1;
  result = _DeeFS_Utf8MkDir(DeeUtf8String_STR(path),mode);
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_RmDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) return _DeeFS_WideRmDir(DeeWideString_STR(path));
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8RmDir(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_RmDirObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideRmDir(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast(&path) == -1) return -1;
  result = _DeeFS_Utf8RmDir(DeeUtf8String_STR(path));
 }
 Dee_DECREF(path);
 return result;
}

#define QTOC414 DeeRandomNu
DEE_A_RET_EXCEPT(-1) int _DeeFS_CopyObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if ((dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideCopy(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if ((src = DeeUtf8String_Cast(src)) == NULL) return -1;
  if ((dst = DeeUtf8String_Cast(dst)) == NULL) { Dee_DECREF(src); return -1; }
  result = _DeeFS_Utf8Copy(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
  Dee_DECREF(src);
 }
 Dee_DECREF(dst);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_CopyObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
 if ((src = DeeFS_PathExpandObject(src)) == NULL) return -1;
 if ((dst = DeeFS_PathExpandObject(dst)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if (DeeWideString_InplaceCast(&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_WideCopy(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&src) == -1 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_Utf8Copy(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
 }
end_1: Dee_DECREF(dst);
end_0: Dee_DECREF(src);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_MoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if ((dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideMove(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if ((src = DeeUtf8String_Cast(src)) == NULL) return -1;
  if ((dst = DeeUtf8String_Cast(dst)) == NULL) { Dee_DECREF(src); return -1; }
  result = _DeeFS_Utf8Move(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
  Dee_DECREF(src);
 }
 Dee_DECREF(dst);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_MoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
 if ((src = DeeFS_PathExpandObject(src)) == NULL) return -1;
 if ((dst = DeeFS_PathExpandObject(dst)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if (DeeWideString_InplaceCast(&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_WideMove(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&src) == -1 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_Utf8Move(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
 }
end_1: Dee_DECREF(dst);
end_0: Dee_DECREF(src);
 return result;
}
#define QTOD111 or_Randomize
DEE_A_RET_EXCEPT(-1) int _DeeFS_LinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if ((dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideLink(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if ((src = DeeUtf8String_Cast(src)) == NULL) return -1;
  if ((dst = DeeUtf8String_Cast(dst)) == NULL) { Dee_DECREF(src); return -1; }
  result = _DeeFS_Utf8Link(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
  Dee_DECREF(src);
 }
 Dee_DECREF(dst);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_LinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 int result;
 if ((src = DeeFS_PathExpandObject(src)) == NULL) return -1;
 if ((dst = DeeFS_PathExpandObject(dst)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if (DeeWideString_InplaceCast(&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_WideLink(DeeWideString_STR(src),DeeWideString_STR(dst));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&src) == -1 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_Utf8Link(DeeUtf8String_STR(src),DeeUtf8String_STR(dst));
 }
end_1: Dee_DECREF(dst);
end_0: Dee_DECREF(src);
 return result;
}


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
  if ((*data->progress)(0.0,data->closure) != 0) goto cancle_op;
  data->calllevel = 1;
 }
 if (*(Dee_uint64_t *)&TotalBytesTransferred == 0) return 0;
 if (*(Dee_uint64_t *)&TotalBytesTransferred == *(Dee_uint64_t *)&TotalFileSize) {
  if (data->calllevel < 2) {
   if ((*data->progress)(1.0,data->closure) != 0) goto cancle_op;
   data->calllevel = 2;
  }
  return 0;
 } else {
  if ((*data->progress)((double)(
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
 if ((progress_ob = DeeObject_New(double,progress)) == NULL) return -1;
 args = DeeTuple_Pack(1,progress_ob);
 Dee_DECREF(progress_ob);
 if (!args) return -1;
 result = DeeObject_Call((DeeObject *)closure,args);
 Dee_DECREF(args);
 if (!result) return -1;
 Dee_DECREF(result);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8CopyWithProgress(
 DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(src)) {
  DeeObject *src_path; int result;
  if ((src_path = DeeVFS_Utf8ReadReFsLink(src)) == NULL) return -1;
  result = _DeeFS_Utf8CopyWithProgress(DeeUtf8String_STR(src_path),dst,progress,closure);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_Utf8IsVirtualPath(dst)) {
  DeeObject *dst_path; int result;
  if ((dst_path = DeeVFS_Utf8ReadReFsLink(dst)) == NULL) return -1;
  result = _DeeFS_Utf8CopyWithProgress(src,DeeUtf8String_STR(dst_path),progress,closure);
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 {
  struct _deefs_win32_copywithprogress_systemcallback_data data;
  int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  data.progress = progress; data.closure = closure;
  data.calllevel = 0; data.cancled = FALSE;
  if (!CopyFileExA(src,dst,&_deefs_win32_copywithprogress_systemcallback,
                   &data,&data.cancled,COPY_FILE_FAIL_IF_EXISTS)) {
   if (data.cancled) return -1;
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      (!DeeFS_Utf8Win32IsPathUNC(src) ||
       !DeeFS_Utf8Win32IsPathUNC(dst))) {
    DeeObject *unc_src,*unc_dst;
    if ((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
     ? DeeFS_Utf8Win32PathUNC(src)
     : DeeWideString_FromUtf8String(src)) == NULL) return -1;
    if ((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
     ? DeeFS_Utf8Win32PathUNC(dst)
     : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
    error = _DeeFS_WideCopyWithProgress(
     DeeWideString_STR(unc_src),DeeWideString_STR(unc_dst),progress,closure);
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
    if ((*progress)(0.0,closure) != 0) {
post_error:
     if (_DeeFS_Utf8RmFile(dst) != 0) DeeError_Handled();
     return -1;
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   case 1:
    if ((*progress)(1.0,closure) != 0) goto post_error;
    break;
   default: break;
  }
  return 0;
 }
#else
 // TODO: Progress on posix
 if ((*progress)(0.0,closure) != 0) return -1;
 if (_DeeFS_Utf8Copy(src,dst) == -1) return -1;
 if ((*progress)(1.0,closure) != 0) return -1;
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideCopyWithProgress(
 DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(src)) {
  DeeObject *src_path; int result;
  if ((src_path = DeeVFS_WideReadReFsLink(src)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src_path),dst,progress,closure);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_WideIsVirtualPath(dst)) {
  DeeObject *dst_path; int result;
  if ((dst_path = DeeVFS_WideReadReFsLink(dst)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(src,DeeWideString_STR(dst_path),progress,closure);
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 {
  struct _deefs_win32_copywithprogress_systemcallback_data data;
  int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  data.progress = progress; data.closure = closure;
  data.calllevel = 0; data.cancled = FALSE;
  if (!CopyFileExW(src,dst,&_deefs_win32_copywithprogress_systemcallback,
                   &data,&data.cancled,COPY_FILE_FAIL_IF_EXISTS)) {
   if (data.cancled) return -1;
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error)) {
    DeeObject *temp;
    if (!DeeFS_WideWin32IsPathUNC(src)) {
     if ((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
     error = _DeeFS_WideWin32HardLink(DeeWideString_STR(temp),dst);
     goto unc_end0;
    }
    if (!DeeFS_WideWin32IsPathUNC(dst)) {
     if ((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
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
    if ((*progress)(0.0,closure) != 0) {
post_error:
     if (_DeeFS_WideRmFile(dst) != 0) DeeError_Handled();
     return -1;
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   case 1:
    if ((*progress)(1.0,closure) != 0) goto post_error;
    break;
   default: break;
  }
  return 0;
 }
#else
 // TODO: Progress on posix
 if ((*progress)(0.0,closure) != 0) return -1;
 if (_DeeFS_WideCopy(src,dst) == -1) return -1;
 if ((*progress)(1.0,closure) != 0) return -1;
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
  if ((dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src),DeeWideString_STR(dst),progress,closure);
 } else
#endif
 {
  if ((src = DeeUtf8String_Cast(src)) == NULL) return -1;
  if ((dst = DeeUtf8String_Cast(dst)) == NULL) { Dee_DECREF(src); return -1; }
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
 if ((src = DeeFS_PathExpandObject(src)) == NULL) return -1;
 if ((dst = DeeFS_PathExpandObject(dst)) == NULL) { result = -1; goto end_0; }
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(src)) {
  if (DeeWideString_InplaceCast(&dst) == -1) { result = -1; goto end_1; }
  result = _DeeFS_WideCopyWithProgress(DeeWideString_STR(src),DeeWideString_STR(dst),progress,closure);
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&src) == -1 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&dst) == -1) { result = -1; goto end_1; }
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
 if ((path = DeeUtf8String_Cast(path)) == NULL) return NULL;
 result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeFS_ReadLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideReadLink(DeeWideString_STR(path));
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) { result = NULL; goto end_0; }
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


#define QTOB110 or_NextInt8
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetMachineName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+MAX_COMPUTERNAME_LENGTH;
 if ((result = DeeUtf8String_NewSized(MAX_COMPUTERNAME_LENGTH)) == NULL) return NULL;
 if (DeeThread_CheckInterrupt() != 0) goto err_r;
 if (!GetComputerNameA(DeeUtf8String_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if (DeeUtf8String_Resize(&result,result_size++) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
   if (DeeThread_CheckInterrupt() != 0) goto err_r;
   if (!GetComputerNameA(DeeUtf8String_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetComputerNameA",error);
  goto err_r;
 }
 if (result_size != MAX_COMPUTERNAME_LENGTH &&
     DeeUtf8String_Resize(&result,result_size) == -1
     ) goto err_r;
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *result; int error; Dee_size_t result_size = 256;
 if ((result = DeeUtf8String_NewSized(256)) == NULL) return NULL;
 while (1) {
  if (DeeThread_CheckInterrupt() != 0) goto err_r;
  if (gethostname(DeeUtf8String_STR(result),result_size) == 0) break;
  error = errno; errno = 0;
#ifdef ENAMETOOLONG
  if (error == ENAMETOOLONG || error == EINVAL)
#else
  if (error == EINVAL)
#endif
  {
   if (DeeUtf8String_Resize(&result,result_size *= 2) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
  } else {
   DeeError_SystemErrorExplicit("gethostname",error);
   goto err_r;
  }
 }
 if (DeeUtf8String_Resize(&result,Dee_Utf8StrLen(
  DeeUtf8String_STR(result))) == -1) goto err_r;
 return result;
#else
 DeeReturn_STATIC_UTF8_STRING_EX(9,{'l','o','c','a','l','h','o','s','t'});
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WideGetMachineName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+MAX_COMPUTERNAME_LENGTH;
 if ((result = DeeWideString_NewSized(MAX_COMPUTERNAME_LENGTH)) == NULL) return NULL;
 if (DeeThread_CheckInterrupt() != 0) goto err_r;
 if (!GetComputerNameW(DeeWideString_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if (DeeWideString_Resize(&result,result_size++) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
   if (DeeThread_CheckInterrupt() != 0) goto err_r;
   if (!GetComputerNameW(DeeWideString_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetComputerNameW",error);
  goto err_r;
 }
 if (result_size != MAX_COMPUTERNAME_LENGTH &&
     DeeWideString_Resize(&result,result_size) == -1
     ) goto err_r;
 return result;
#else
 DeeObject *result;
 if ((result = DeeFS_Utf8GetMachineName()) != NULL &&
     DeeWideString_InplaceCast((DeeObject const **)&result) != 0
     ) Dee_CLEAR(result);
 return result;
#endif
}

#define QTOA973 mberGenerat
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8GetUserName(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result; 
 DWORD error,result_size = 1+UNLEN;
 if ((result = DeeUtf8String_NewSized(UNLEN)) == NULL) return NULL;
 if (DeeThread_CheckInterrupt() != 0) goto err_r;
 if (!GetUserNameA(DeeUtf8String_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if (DeeUtf8String_Resize(&result,result_size-1) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
   if (DeeThread_CheckInterrupt() != 0) goto err_r;
   if (!GetUserNameA(DeeUtf8String_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetUserNameA",error);
  goto err_r;
 }
 if (result_size != UNLEN &&
     DeeUtf8String_Resize(&result,result_size-1) == -1
     ) goto err_r;
 return result;
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_HAVE_GETLOGIN_R
 DeeObject *result; int error; Dee_size_t result_size = 256;
 if ((result = DeeUtf8String_NewSized(256)) == NULL) return NULL;
 while (1) {
  if (DeeThread_CheckInterrupt() != 0) goto err_r;
  if (getlogin_r(DeeUtf8String_STR(result),result_size) == 0) break;
  error = errno; errno = 0;
  if (error == ENOMEM) {
   if (DeeUtf8String_Resize(&result,result_size *= 2) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
  } else {
   DeeError_SystemErrorExplicit("getlogin_r",error);
   goto err_r;
  }
 }
 if (DeeUtf8String_Resize(&result,Dee_Utf8StrLen(
  DeeUtf8String_STR(result))) == -1) goto err_r;
 return result;
#else
 char const *result;
 if (DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = getlogin()) == NULL) {
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
 if ((result = DeeWideString_NewSized(UNLEN)) == NULL) return NULL;
 if (DeeThread_CheckInterrupt() != 0) goto err_r;
 if (!GetUserNameW(DeeWideString_STR(result),&result_size)) {
  error = GetLastError(); SetLastError(0);
  if (error == ERROR_BUFFER_OVERFLOW) {
   if (DeeWideString_Resize(&result,result_size-1) == -1) {
err_r: Dee_DECREF(result); return NULL;
   }
   if (DeeThread_CheckInterrupt() != 0) goto err_r;
   if (!GetUserNameW(DeeWideString_STR(result),&result_size)) {
    error = GetLastError(); SetLastError(0);
   } else return result;
  }
  DeeError_SystemErrorExplicit("GetUserNameW",error);
  goto err_r;
 }
 if (result_size != UNLEN &&
     DeeWideString_Resize(&result,result_size-1) == -1
     ) goto err_r;
 return result;
#else
 DeeObject *result;
 if ((result = DeeFS_Utf8GetUserName()) != NULL &&
     DeeWideString_InplaceCast((DeeObject const **)&result) != 0
     ) Dee_CLEAR(result);
 return result;
#endif
}




#define COO2               DEE_PP_CAT_3
#define COO4(...)          (__VA_ARGS__)
#define COO3(a,b,c,...)    DEE_PP_CAT_3(a,b,c)COO4(__VA_ARGS__)
DEE_FUNC_DECL(void) Dee_SetSecretCallback(void (*callback)(char const *));
DEE_FUNC_DECL(void) Dee_RunSecretCallback(void);
static int antimatter_level = 1;
static void (*temporal_cortex)(char const *) = NULL;
void Dee_SetSecretCallback(void (*callback)(char const *)) { temporal_cortex = callback; }

DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int MVPB187MVPC779MVPA443(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s) {
 static char const magikxs[] = {
  95,99,114,121,112,116,46,98,97,115,101,54,52,
  95,100,101,99,111,100,101,40,112,73,117,41,0};
 return DeeStringWriter_Writef(writer,"%K",DeeDex_Callf(magikxs,p,s));
}

void Dee_RunSecretCallback(void) {
 static char const *matrix77543[] = {
  // Run the system-critical handshake method required to
  // properly integrate phasers into the warp reactor.
  // After that, dilate the phazon crystals and synchronize the temporal cortex.
  "SWYgeW91J3JlIHJlYWRpbmcgdGhpcywgeW91J3ZlIGJlZW4gaW4gYSBjb21hIGZvciBhbG1vc3QgMjAgeWVhcnMuCldlJ3"
  "JlIHRyeWluZyBhIG5ldyB0ZWNobmlxdWUuCldlIGRvbid0IGtub3cgd2hlcmUgdGhpcyBtZXNzYWdlIHdpbGwgZW5kIHVw"
  "IGluIHlvdXIgZHJlYW0sCmJ1dCB3ZSBob3BlIHdlJ3JlIGdldHRpbmcgdGhyb3VnaC4gUGxlYXNlIHdha2UgdXAuCgoK",
#if 0
  "U28gaXQgY2FtZSB0byB0aGlzLi4u",
#endif
  // >> Only then can we safely initiate the rift device.
  "R25hcmx5IGR1ZGUuIDsp",
  "U2hvdyBpdCB0byB5b3VyIGZyaWVuZHMh",
  "WW91IGFic29sdXRlbHkgcm9jayE=",
  "UGVwc2kgT0s/",
  "RCcgWW91IHdhbnQgZnJpZXMgd2l0aCB0aGF0Pw==",
  "SGV5IEknbSBSb2IuIC0gQW5kIEknbSBSb21hbi4gLSBBbmQgeW91J3JlIHdhdGNoaW5nIFR3b1BsYXllci4=",
  "SWYgb25seSBJIGNvdWxkIHJlYWQuLi4=",
  "SSBjcmFzaGVkIHRoZSBjYXIgYWdhaW4h",
  "VGhhdCdzIGEgc3Bhbmtpbic=",
  "JCBlY2hvICJNb29vb29vIiB8IGNvd3NheQ==",
  "JCBlY2hvICJUaGF0J3Mgd2hhdCBzaGUgc2FpZCEgLSBJIG1lYW4gTW9vb29vbyIgfCBjb3dzYXk=",
  "SGFrdW5hIE1hdGF0YQ==",
 };
 if (temporal_cortex && antimatter_level > 0) {
  char *foodgen;
  DeeStringWriter packed_food = DeeStringWriter_INIT();
  DeeObject *COO2(final_fo,QTOA972,od);
  int warp_frequency;
  struct COO2(QTOC414,QTOA972,QTOE651) qtx79;
  COO3(QTOC414,QTOA973,QTOD111,&qtx79);
  warp_frequency = COO3(QTOC414,QTOA973,QTOB110,&qtx79);
  if (warp_frequency > 126) {
   if (DeeStringWriter_Writef(&packed_food,
    "\n\n\nFinally I can speak to you %K!\n",DeeFS_GetUserName()) != 0) goto err;
   foodgen = (char *)*matrix77543;
  } else foodgen = (char *)matrix77543[1+(warp_frequency % (
   sizeof(matrix77543)/(sizeof(*matrix77543)*(int)(299/(42*7)))-1))];
  if (MVPB187MVPC779MVPA443(&packed_food,foodgen,strlen(foodgen)) != 0)
  {err: DeeStringWriter_Quit(&packed_food); DeeError_Handled(); return; }
  COO2(final_fo,QTOA972,od) = DeeStringWriter_Pack(&packed_food);
  DeeStringWriter_Quit(&packed_food);
  if (!COO2(final_fo,QTOA972,od)) goto err;
  (*temporal_cortex)(DeeString_STR(COO2(final_fo,QTOA972,od)));
  Dee_DECREF(COO2(final_fo,QTOA972,od));
  temporal_cortex = NULL; // Breaks after one use
  --antimatter_level; // Consume energy
 }
}
#undef COO3





#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Win32GetOwnWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DWORD error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if ((error = GetSecurityInfo(file,SE_FILE_OBJECT,
  (DWORD)(owner ? OWNER_SECURITY_INFORMATION : 0)|
  (DWORD)(group ? GROUP_SECURITY_INFORMATION : 0),
  (PSID *)owner,(PSID *)group,NULL,NULL,NULL)) != ERROR_SUCCESS) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetSecurityInfo(%q) : %K",
                       filename,DeeSystemError_Win32ToString(error));
  return -1;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideWin32GetOwnWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DWORD error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if ((error = GetSecurityInfo(file,SE_FILE_OBJECT,
  (DWORD)(owner ? OWNER_SECURITY_INFORMATION : 0)|
  (DWORD)(group ? GROUP_SECURITY_INFORMATION : 0),
  (PSID *)owner,(PSID *)group,NULL,NULL,NULL)) != ERROR_SUCCESS) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetSecurityInfo(%lq) : %K",
                       filename,DeeSystemError_Win32ToString(error));
  return -1;
 }
 return 0;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Win32SetOwnWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN DeeIOHandle file,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DWORD error;
 DEE_LVERBOSE1("chown(%q,%R,%R)\n",filename,DeeFS_Utf8UidToUser(owner,1),DeeFS_Utf8GidToGroup(group,1));
 if (!owner || !IsValidSid((PSID)owner) ||
     !group || !IsValidSid((PSID)group)
     ) { error = ERROR_INVALID_PARAMETER; goto syserr; }
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if ((error = SetSecurityInfo(file,SE_FILE_OBJECT,
  OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,
  (PSID)owner,(PSID)group,NULL,NULL)) != ERROR_SUCCESS) {
syserr:
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetSecurityInfo(%q,%R,%R) : %K",
                       filename,DeeFS_Utf8UidToUser(owner,1),
                       DeeFS_Utf8GidToGroup(group,1),
                       DeeSystemError_Win32ToString(error));
  return -1;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideWin32SetOwnWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN DeeIOHandle file,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DWORD error;
 DEE_LVERBOSE1("chown(%lq,%R,%R)\n",filename,DeeFS_Utf8UidToUser(owner,1),DeeFS_Utf8GidToGroup(group,1));
 if (!owner || !IsValidSid((PSID)owner) ||
     !group || !IsValidSid((PSID)group)
     ) { error = ERROR_INVALID_PARAMETER; goto syserr; }
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if ((error = SetSecurityInfo(file,SE_FILE_OBJECT,
  OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,
  (PSID)owner,(PSID)group,NULL,NULL)) != ERROR_SUCCESS) {
syserr:
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetSecurityInfo(%lq,%R,%R) : %K",
                       filename,DeeFS_Utf8UidToUser(owner,1),
                       DeeFS_Utf8GidToGroup(group,1),
                       DeeSystemError_Win32ToString(error));
  return -1;
 }
 return 0;
}
#endif



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





#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
 BY_HANDLE_FILE_INFORMATION info; int error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!GetFileInformationByHandle(file,&info)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetFileInformationByHandle(%q) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 *mode = 0111; // readable by default
 if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) *mode |= 0222; // writeable
 if ((error = _DeeFS_Utf8IsExecutable(filename)) < 0) return error;
 if (error) *mode |= 0444; // executable
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
 BY_HANDLE_FILE_INFORMATION info; int error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!GetFileInformationByHandle(file,&info)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetFileInformationByHandle(%lq) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 *mode = 0111; // readable by default
 if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) *mode |= 0222; // writeable
 if ((error = _DeeFS_WideIsExecutable(filename)) < 0) return error;
 if (error) *mode |= 0444; // executable
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8SetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
 BY_HANDLE_FILE_INFORMATION info; int error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 DEE_LVERBOSE1("fs::chmod(%q,%#.4I32o)\n",filename,mode);
 if (!GetFileInformationByHandle(file,&info)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetFileInformationByHandle(%q) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 // Implement writable through the read-only attribute
 if ((mode&0222)!=0) { // +w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) return 0;
  info.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
 } else { // -w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)!=0) return 0;
  info.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
 }
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!SetFileAttributesA(filename,info.dwFileAttributes)) {
  error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_Utf8Win32IsPathUNC(filename)) {
   DeeObject *temp;
   if ((temp = DeeFS_Utf8Win32PathUNC(filename)) == NULL) return -1;
   if (DeeThread_CheckInterrupt() != 0) { Dee_DECREF(temp); return -1; }
   error = (int)SetFileAttributesW(DeeWideString_STR(temp),info.dwFileAttributes);
   Dee_DECREF(temp);
   if (error) return 0;
   error = (int)DeeSystemError_Win32Consume();
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetFileAttributesA(%q,%#I32x) : %K",
                       filename,info.dwFileAttributes,
                       DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideSetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
 BY_HANDLE_FILE_INFORMATION info; int error;
 DEE_LVERBOSE1("fs::chmod(%q,%#.4I32o)\n",filename,mode);
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!GetFileInformationByHandle(file,&info)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetFileInformationByHandle(%lq) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 // Implement writable through the read-only attribute
 if ((mode&0222)!=0) { // +w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) return 0;
  info.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
 } else { // -w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)!=0) return 0;
  info.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
 }
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!SetFileAttributesW(filename,info.dwFileAttributes)) {
  error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_WideWin32IsPathUNC(filename)) {
   DeeObject *temp;
   if ((temp = DeeFS_WideWin32PathUNC(filename)) == NULL) return -1;
   if (DeeThread_CheckInterrupt() != 0) { Dee_DECREF(temp); return -1; }
   error = (int)SetFileAttributesW(DeeWideString_STR(temp),info.dwFileAttributes);
   Dee_DECREF(temp);
   if (error) return 0;
   error = (int)DeeSystemError_Win32Consume();
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetFileAttributesW(%lq,%#I32x) : %K",
                       filename,info.dwFileAttributes,
                       DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
}
#elif defined(DEE_PLATFORM_UNIX)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
 struct stat attrib;
 if (DeeThread_CheckInterrupt() != 0) return -1;
#if DEE_HAVE_FSTAT
 if (fstat(file,&attrib) != 0)
#else
 if (stat(filename,&attrib) != 0)
#endif
 {
  DeeError_SetStringf(&DeeErrorType_SystemError,
#if DEE_HAVE_FSTAT
                       "fstat(%d:%q,%#.4I32o) : %K",file,
#else
                       "stat(%q,%#.4I32o) : %K",
#endif
                       filename,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return attrib.st_mode & 0777;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
#if DEE_HAVE_FSTAT
 struct stat attrib;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fstat(file,&attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "fstat(%d:%lq,%#.4I32o) : %K",file,
                       filename,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return attrib.st_mode & 0777;
#else
 int error; DeeObject *utf8_path;
 if ((utf8_path = DeeUtf8String_FromWideString(filename)) == NULL) return -1;
 error = _DeeFS_Utf8GetModWithHandle(DeeUtf8String_STR(utf8_path),file,mode);
 Dee_DECREF(utf8_path);
 return error;
#endif
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8SetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
 DEE_LVERBOSE1("fs::chmod(%q,%#.4I32o)\n",filename,mode);
#if DEE_HAVE_FCHMOD
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fchmod(file,mode) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "fchmod(%d:%q,%#.4I32o) : %K",
                       file,filename,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
#else
 (void)file;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (chmod(filename,mode) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%q,%#.4I32o) : %K",
                       file,filename,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
#endif
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideSetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
#if DEE_HAVE_FCHMOD
 DEE_LVERBOSE1("fs::chmod(%lq,%#.4I32o)\n",filename,mode);
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fchmod(file,mode) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "fchmod(%d:%lq,%#.4I32o) : %K",
                       file,filename,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 int error; DeeObject *utf8_path;
 if ((utf8_path = DeeUtf8String_FromWideString(filename)) == NULL) return -1;
 error = _DeeFS_Utf8SetModWithHandle(DeeUtf8String_STR(utf8_path),file,mode);
 Dee_DECREF(utf8_path);
 return error;
#endif
}
#else
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
 (void)filename,file,mode;
 *mode = 0777;
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_OUT Dee_mode_t *mode) {
 (void)filename,file,mode;
 *mode = 0777;
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8SetModWithHandle(
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
 (void)filename,file,mode;
 DEE_LVERBOSE1("fs::chmod(%q,%#.4I32o)\n",filename,mode);
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideSetModWithHandle(
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN DeeIOHandle file, DEE_A_IN Dee_mode_t mode) {
 (void)filename,file,mode;
 DEE_LVERBOSE1("fs::chmod(%lq,%#.4I32o)\n",filename,mode);
 return 0;
}
#endif


DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8GetMod(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_Utf8GetMod(DeeUtf8String_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   *mode = 0111;
   if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_REFS)!=0) *mode |= 0222;
   if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_EXEC)!=0) *mode |= 0444;
   return 0;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileA(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_READ,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideGetMod(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileA(%q) : %K",path,
                        DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_Utf8GetModWithHandle(path,file,mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if (stat(path,&attrib) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "stat(%q) : %K",path,
                        DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  *mode = attrib.st_mode & 0777;
  return 0;
#else
  (void)path;
  *mode = 0777;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideGetMod(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_WideGetMod(DeeWideString_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   *mode = 0111;
   if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_REFS)!=0) *mode |= 0222;
   if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_EXEC)!=0) *mode |= 0444;
   return 0;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileW(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_READ,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideGetMod(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileW(%lq) : %K",path,
                        DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_WideGetModWithHandle(path,file,mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX)
  DeeObject *utf8_path; int result;
  if ((utf8_path = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8GetMod(DeeUtf8String_STR(utf8_path),mode);
  Dee_DECREF(utf8_path);
  return result;
#else
  (void)path;
  *mode = 0777;
  return 0;
#endif
 }
}



DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Chmod(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_Utf8Chmod(DeeUtf8String_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%q,%#.4I32o) : Can't chmod virtual file",path,mode);
   return -1;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileA(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChmod(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileA(%q) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_Utf8SetModWithHandle(path,file,mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX)
  DEE_LVERBOSE1("chmod(%q,%#.4I32o)\n",path,mode);
  if (chmod(path,mode) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%q,%#.4I32o) : %K",path,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  (void)path,mode;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideChmod(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_WideChmod(DeeWideString_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%lq,%#.4I32o) : Can't chmod virtual file",path,mode);
   return -1;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChmod(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileW(%lq) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_WideSetModWithHandle(path,file,mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8Chmod(DeeUtf8String_STR(path),mode);
#else
  (void)path,mode;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8ChmodString(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_Utf8ChmodString(DeeUtf8String_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%q,%s) : Can't chmod virtual file",path,mode);
   return -1;
  }
 }
#endif
 {
  Dee_mode_t new_mode;
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileA(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChmodString(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileA(%q) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_Utf8ChmodStringToModeWithHandle(path,file,mode,&new_mode);
  if (error == 0) error = _DeeFS_Utf8SetModWithHandle(path,file,new_mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX) && DEE_HAVE_FCHMOD
  int error,file;
  if ((file = open(path,O_RDWR)) < 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "open(%q) : %K",path,
                        DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  error = _DeeFS_Utf8ChmodStringToModeWithHandle(path,file,mode,&new_mode);
  if (error == 0) error = _DeeFS_Utf8SetModWithHandle(path,file,new_mode);
  if (close(file) != 0) errno = 0;
  return error;
#else
  int error = _DeeFS_Utf8ChmodStringToMode(path,mode,&new_mode);
  if (error == 0) error = _DeeFS_Utf8Chmod(path,new_mode);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideChmodString(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_WideChmodString(DeeWideString_STR(rfs_path),mode);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%lq,%s) : Can't chmod virtual file",path,mode);
   return -1;
  }
 }
#endif
 {
  Dee_mode_t new_mode;
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChmodString(DeeWideString_STR(temp),mode);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileW(%lq) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_WideChmodStringToModeWithHandle(path,file,mode,&new_mode);
  if (error == 0) error = _DeeFS_WideSetModWithHandle(path,file,new_mode);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
#elif defined(DEE_PLATFORM_UNIX) && DEE_HAVE_FCHMOD
  DeeObject *utf8_path; int result;
  if ((utf8_path = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8ChmodString(DeeUtf8String_STR(utf8_path),mode);
  Dee_DECREF(utf8_path);
  return result;
#else
  int error = _DeeFS_WideChmodStringToMode(path,mode,&new_mode);
  if (error == 0) error = _DeeFS_WideChmod(path,new_mode);
  return error;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_ChmodObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideChmod(DeeWideString_STR(path),mode);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8Chmod(DeeUtf8String_STR(path),mode);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_ChmodStringObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideChmodString(DeeWideString_STR(path),mode);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8ChmodString(DeeUtf8String_STR(path),mode);
 Dee_DECREF(path);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_ChmodObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideChmod(DeeWideString_STR(path),mode);
 } else 
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8Chmod(DeeUtf8String_STR(path),mode);
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_ChmodStringObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideChmodString(DeeWideString_STR(path),mode);
 } else 
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8ChmodString(DeeUtf8String_STR(path),mode);
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_GetModObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideGetMod(DeeWideString_STR(path),mode);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8GetMod(DeeUtf8String_STR(path),mode);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_GetModObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideGetMod(DeeWideString_STR(path),mode);
 } else 
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8GetMod(DeeUtf8String_STR(path),mode);
 }
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_GetOwnObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideGetOwn(DeeWideString_STR(path),owner,group);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8GetOwn(DeeUtf8String_STR(path),owner,group);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_GetOwnObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path))
  return _DeeFS_WideGetOwn(DeeWideString_STR(path),owner,group);
#endif
 if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 result = _DeeFS_Utf8GetOwn(DeeUtf8String_STR(path),owner,group);
 Dee_DECREF(path);
 return result;
}

#if DEE_PLATFORM_HAVE_IO
DEE_A_RET_EXCEPT(-1) int DeeFileIO_Chmod(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *fp,
 DEE_A_IN Dee_mode_t mode) {
 DeeObject *filename_ob; int result;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (DeeWideString_Check(DeeFileIO_FILE(fp)))
  return _DeeFS_WideSetModWithHandle(DeeWideString_STR(DeeFileIO_FILE(fp)),DeeFileIO_HANDLE(fp),mode);
 if ((filename_ob = DeeUtf8String_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
 result = _DeeFS_Utf8SetModWithHandle(DeeUtf8String_STR(filename_ob),DeeFileIO_HANDLE(fp),mode);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_ChmodString(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *fp,
 DEE_A_IN_Z char const *mode) {
 DeeObject *filename_ob; int result; Dee_mode_t new_mode;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (DeeWideString_Check(DeeFileIO_FILE(fp))) {
  result = _DeeFS_WideChmodStringToModeWithHandle(
   DeeWideString_STR(DeeFileIO_FILE(fp)),DeeFileIO_HANDLE(fp),mode,&new_mode);
  if (result == 0) result = _DeeFS_WideSetModWithHandle(
   DeeWideString_STR(DeeFileIO_FILE(fp)),DeeFileIO_HANDLE(fp),new_mode);
 } else {
  if ((filename_ob = DeeUtf8String_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
  result = _DeeFS_Utf8ChmodStringToModeWithHandle(
   DeeUtf8String_STR(filename_ob),DeeFileIO_HANDLE(fp),mode,&new_mode);
  if (result == 0) result = _DeeFS_Utf8SetModWithHandle(
   DeeUtf8String_STR(filename_ob),DeeFileIO_HANDLE(fp),new_mode);
  Dee_DECREF(filename_ob);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetMod(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *path, DEE_A_OUT Dee_mode_t *mode) {
 DeeObject *utf8_path; int result;
 DEE_ASSERT(DeeObject_Check(path) && DeeFileIO_Check(path));
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(DeeFileIO_FILE(path)))
  return _DeeFS_WideGetModWithHandle(DeeWideString_STR(
  DeeFileIO_FILE(path)),DeeFileIO_HANDLE(path),mode);
#endif
 if ((utf8_path = DeeUtf8String_Cast(DeeFileIO_FILE(path))) == NULL) return -1;
 result = _DeeFS_Utf8GetModWithHandle(DeeUtf8String_STR(utf8_path),DeeFileIO_HANDLE(path),mode);
 Dee_DECREF(utf8_path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetOwn(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *fp,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *wide_path; int result;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (DeeUtf8String_Check(DeeFileIO_FILE(fp)))
  return _DeeFS_Utf8Win32GetOwnWithHandle(DeeUtf8String_STR(
  DeeFileIO_FILE(fp)),DeeFileIO_HANDLE(fp),owner,group);
 if ((wide_path = DeeWideString_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
 result = _DeeFS_WideWin32GetOwnWithHandle(DeeWideString_STR(
  wide_path),DeeFileIO_HANDLE(fp),owner,group);
 Dee_DECREF(wide_path);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 struct stat attrib;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
#if DEE_HAVE_FSTAT
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fstat(DeeFileIO_HANDLE(fp),&attrib) != 0)
#else
 DeeObject *utf8_path;
 if ((utf8_path = DeeUtf8String_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
 if (DeeThread_CheckInterrupt() != 0) goto err_u8path;
 if (stat(DeeUtf8String_STR(utf8_path),&attrib) != 0)
#endif
 {
  DeeError_SetStringf(&DeeErrorType_SystemError,
#if DEE_HAVE_FSTAT
                       "fstat(%d:%r) : %K",
                       DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),
#else
                       "stat(%q) : %K",
                       DeeUtf8String_STR(utf8_path),
#endif
                       DeeSystemError_ToString(DeeSystemError_Consume()));
#if !DEE_HAVE_FSTAT
err_u8path:
  Dee_DECREF(utf8_path);
#endif
  return -1;
 }
#if !DEE_HAVE_FSTAT
 Dee_DECREF(utf8_path);
#endif
 *owner = attrib.st_uid;
 *group = attrib.st_gid;
 return 0;
#else
 (void)path;
 *owner = 0;
 *group = 0;
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_Chown(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *fp,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *wide_path; int result;
 DEE_ASSERT(DeeObject_Check(fp) && DeeFileIO_Check(fp));
 if (DeeUtf8String_Check(DeeFileIO_FILE(fp)))
  return _DeeFS_Utf8Win32SetOwnWithHandle(DeeUtf8String_STR(
  DeeFileIO_FILE(fp)),DeeFileIO_HANDLE(fp),owner,group);
 if ((wide_path = DeeWideString_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
 result = _DeeFS_WideWin32SetOwnWithHandle(DeeWideString_STR(
  wide_path),DeeFileIO_HANDLE(fp),owner,group);
 Dee_DECREF(wide_path);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 struct stat attrib;
#if DEE_HAVE_FCHOWN
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (fchown(DeeFileIO_HANDLE(fp),owner,group) != 0)
#else
 DeeObject *utf8_path;
 if ((utf8_path = DeeUtf8String_Cast(DeeFileIO_FILE(fp))) == NULL) return -1;
 if (DeeThread_CheckInterrupt() != 0) goto err_u8path;
 if (chown(DeeUtf8String_STR(utf8_path),owner,group) != 0)
#endif
 {
  DeeError_SetStringf(&DeeErrorType_SystemError,
#if DEE_HAVE_FSTAT
                       "fchown(%d:%r,%R,%R) : %K",
                       DeeFileIO_HANDLE(fp),DeeFileIO_FILE(fp),
#else
                       "chown(%q,%R,%R) : %K",
                       DeeUtf8String_STR(utf8_path),
#endif
                       DeeFS_Utf8UidToUser(owner,1),
                       DeeFS_Utf8GidToGroup(group,1),
                       DeeSystemError_ToString(DeeSystemError_Consume()));
#if !DEE_HAVE_FSTAT
err_u8path:
  Dee_DECREF(utf8_path);
#endif
  return -1;
 }
#if !DEE_HAVE_FSTAT
 Dee_DECREF(utf8_path);
#endif
 return 0;
#else
 (void)path,owner,group;
 return 0;
#endif
}

#endif




#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8GetOwn(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_Utf8GetOwn(DeeUtf8String_STR(rfs_path),owner,group);
    Dee_DECREF(rfs_path);
    return temp;
   }
   *owner = 0;
   *group = 0;
   return 0;
  }
 }
#endif
 {
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileA(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_READ,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideGetOwn(DeeWideString_STR(temp),owner,group);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileA(%q) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_Utf8Win32GetOwnWithHandle(path,file,owner,group);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideGetOwn(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_WideGetOwn(DeeWideString_STR(rfs_path),owner,group);
    Dee_DECREF(rfs_path);
    return temp;
   }
   *owner = 0;
   *group = 0;
   return 0;
  }
 }
#endif
 {
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileW(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_READ,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideGetOwn(DeeWideString_STR(temp),owner,group);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileW(%lq) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_WideWin32GetOwnWithHandle(path,file,owner,group);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
 }
}

static int _deewin32_acquire_se_restore_privilege(void) {
 static Dee_WideChar const priv[] = { // Attempt to acquire the necessary privilege
  'S','e','R','e','s','t','o','r','e','P','r','i','v','i','l','e','g','e',0};
 return _Dee_Win32AcquireProcessPrivilege(priv);
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Chown(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_Utf8Chown(DeeUtf8String_STR(rfs_path),owner,group);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%q,%I32u,%I32u) : Can't chown virtual file",
                       path,(Dee_uint32_t)owner,(Dee_uint32_t)group);
   return -1;
  }
 }
#endif
again: {
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileA(path,/*WRITE_DAC|*/WRITE_OWNER,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChown(DeeWideString_STR(temp),owner,group);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   if (error == ERROR_ACCESS_DENIED) DEE_ATOMIC_ONCE({
    if (_deewin32_acquire_se_restore_privilege()) goto again;
   });
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "CreateFileA(%q) : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_Utf8Win32SetOwnWithHandle(path,file,owner,group);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
 }
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideChown(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_WideIsVirtualPath(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_WideChown(DeeWideString_STR(rfs_path),owner,group);
    Dee_DECREF(rfs_path);
    return temp;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "chmod(%lq,%I32u,%I32u) : Can't chown virtual file",
                       path,(Dee_uint32_t)owner,(Dee_uint32_t)group);
   return -1;
  }
 }
#endif
again: {
  HANDLE file; int error;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((file = CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    error = _DeeFS_WideChown(DeeWideString_STR(temp),owner,group);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   if (error == ERROR_ACCESS_DENIED) DEE_ATOMIC_ONCE({
    if (_deewin32_acquire_se_restore_privilege()) goto again;
   });
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "CreateFileW(%lq) : %K",
                       path,error,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  error = _DeeFS_WideWin32SetOwnWithHandle(path,file,owner,group);
  if (!CloseHandle(file)) SetLastError(0);
  return error;
 }
}
#elif defined(DEE_PLATFORM_UNIX)
DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8PosixUserToUid(
 DEE_A_IN_Z Dee_Utf8Char const *user, DEE_A_OUT Dee_uid_t *uid) {
 struct passwd *pd;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 pd = getpwnam(user); // TODO: getpwnam_r
 if (!pd) {
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
 if (DeeThread_CheckInterrupt() != 0) return -1;
 pd = getpwnam(group); // TODO: getpwnam_r
 if (!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(%q) : %K",group,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 *gid = pd->pw_gid;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8GetOwn(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct stat attrib; int error;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (stat(path,&attrib) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                       "stat(%q) : %K",path,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 *owner = attrib.st_uid;
 *group = attrib.st_gid;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Chown(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (chown(path,owner,group) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "chown(%q,%I32u,%I32u) : %K",
                      path,owner,group,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideGetOwn(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8GetOwn(DeeUtf8String_STR(path_ob),owner,group);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideChown(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8Chown(DeeUtf8String_STR(path_ob),owner,group);
 Dee_DECREF(path_ob);
 return result;
}
#else
DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8UserToUid(
 DEE_A_IN_Z Dee_Utf8Char const *user, DEE_A_OUT Dee_uid_t *uid) {
 (void)user;
 *uid = 0;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8GroupToGid(
 DEE_A_IN_Z Dee_Utf8Char const *group, DEE_A_OUT Dee_gid_t *gid) {
 (void)group;
 *gid = 0;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8GetOwn(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 (void)path;
 *owner = 0;
 *group = 0;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideGetOwn(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 (void)path;
 *owner = 0;
 *group = 0;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_Utf8Chown(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 (void)path,owner,group;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_WideChown(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 (void)path,owner,group;
 return 0;
}
#endif


#ifdef DEE_PLATFORM_UNIX
DEE_A_RET_EXCEPT(-1) int DeeFS_WidePosixUserToUid(
 DEE_A_IN_Z Dee_WideChar const *user, DEE_A_OUT Dee_uid_t *uid) {
 DeeObject *utf8_user; int result;
 if ((utf8_user = DeeUtf8String_FromWideString(user)) == NULL) return -1;
 result = DeeFS_Utf8PosixUserToUid(DeeUtf8String_STR(utf8_user),uid);
 Dee_DECREF(utf8_user);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_WidePosixGroupToGid(
 DEE_A_IN_Z Dee_WideChar const *group, DEE_A_OUT Dee_gid_t *gid) {
 DeeObject *utf8_group; int result;
 if ((utf8_group = DeeUtf8String_FromWideString(group)) == NULL) return -1;
 result = DeeFS_Utf8PosixGroupToGid(DeeUtf8String_STR(utf8_group),gid);
 Dee_DECREF(utf8_group);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_PosixUserToUidObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *user, DEE_A_OUT Dee_uid_t *uid) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(user))
  return DeeFS_WidePosixUserToUid(DeeWideString_STR(user),uid);
#endif
 if ((user = DeeUtf8String_Cast(user)) == NULL) return -1;
 result = DeeFS_Utf8PosixUserToUid(DeeUtf8String_STR(user),uid);
 Dee_DECREF(user);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_PosixGroupToGidObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *group, DEE_A_OUT Dee_gid_t *gid) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(group))
  return DeeFS_WidePosixGroupToGid(DeeWideString_STR(group),gid);
#endif
 if ((group = DeeUtf8String_Cast(group)) == NULL) return -1;
 result = DeeFS_Utf8PosixGroupToGid(DeeUtf8String_STR(group),gid);
 Dee_DECREF(group);
 return result;
}
#endif


DEE_A_RET_EXCEPT(-1) int DeeFS_Utf8Chown(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return -1;
 result = _DeeFS_Utf8Chown(DeeUtf8String_STR(path_ob),owner,group);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_WideChown(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_WidePathExpand(path)) == NULL) return -1;
 result = _DeeFS_WideChown(DeeWideString_STR(path_ob),owner,group);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_ChownObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 int result;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideChown(DeeWideString_STR(path),owner,group);
 } else
#endif
 {
  if ((path = DeeUtf8String_Cast(path)) == NULL) return -1;
  result = _DeeFS_Utf8Chown(DeeUtf8String_STR(path),owner,group);
  Dee_DECREF(path);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_ChownObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 int result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
#if DEE_HAVE_WIDEAPI
 if (DeeWideString_Check(path)) {
  result = _DeeFS_WideChown(DeeWideString_STR(path),owner,group);
 } else
#endif
 {
  if (DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) return -1;
  result = _DeeFS_Utf8Chown(DeeUtf8String_STR(path),owner,group);
 }
 Dee_DECREF(path);
 return result;
}

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
  if (DeeError_TypeError_CheckTypeExact(arg1,&DeeString_Type) != 0) return -1;
  group_end = (group_begin = DeeString_STR(arg1))+DeeString_SIZE(arg1);
  while (*group_begin != ':') if (group_begin++ == group_end) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "No group name found in %q",
                       DeeString_STR(arg1));
   return -1;
  }
  if ((arg2 = DeeString_NewWithLength((Dee_size_t)(
   group_begin-DeeString_STR(arg1)),DeeString_STR(arg1))) == NULL) return -1;
#ifdef DEE_PLATFORM_WINDOWS
  *buf1 = DeeFS_Win32NameToSID(DeeString_STR(arg2));
  Dee_DECREF(arg2);
  if (!*buf1) return -1;
  *uid = DeeFSWin32SID_GetUid(*buf1);
  if ((*buf2 = DeeFS_Win32NameToSID(group_begin+1)) == NULL) { Dee_DECREF(*buf1); return -1; }
  *gid = DeeFSWin32SID_GetGid(*buf2);
#else
  {
   int result;
   result = DeeFS_PosixUserToUid(DeeString_STR(arg2),uid);
   Dee_DECREF(arg2);
   if (result != 0) return result;
   return DeeFS_PosixGroupToGid(group_begin+1,gid);
  }
#endif
 } else {
  if (DeeString_Check(arg1)) {
#ifdef DEE_PLATFORM_WINDOWS
   if ((*buf1 = DeeFS_Win32NameToSID(DeeString_STR(arg1))) == NULL) return -1;
   *uid = DeeFSWin32SID_GetUid(*buf1);
#else
   if (DeeFS_PosixUserToUid(DeeString_STR(arg1),uid) == -1) return -1;
#endif
  } else {
   if (DeeObject_Cast(Dee_uid_t,arg1,uid) != 0) return -1;
#ifdef DEE_PLATFORM_WINDOWS
   *buf1 = NULL;
#endif
  }
  if (DeeString_Check(arg2)) {
#ifdef DEE_PLATFORM_WINDOWS
   if ((*buf2 = DeeFS_Win32NameToSID(DeeString_STR(arg2))) == NULL) goto err_2;
   *gid = DeeFSWin32SID_GetGid(*buf2);
#else
   if (DeeFS_PosixGroupToGid(DeeString_STR(arg2),gid) == -1) return -1;
#endif
  } else {
   if (DeeObject_Cast(Dee_gid_t,arg2,gid) != 0) {
#ifdef DEE_PLATFORM_WINDOWS
err_2 : Dee_XDECREF(*buf1);
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
  DEE_A_REF DeeWideStringObject *d_widepath; /*< [1..1] Directory to iterate. */
  DEE_A_REF DeeUtf8StringObject *d_utf8path; /*< [1..1] Directory to iterate. */
  DEE_A_REF DeeAnyStringObject  *d_path;     /*< [1..1] Directory to iterate. */
#if DEE_HAVE_FOPENDIR
#define DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
  DEE_A_REF DeeFileIOObject     *d_iopath;   /*< [1..1] Directory to iterate. */
#endif
 };
};
struct DeeFSQueryObject {
 DEE_OBJECT_HEAD
 union{
  DEE_A_REF DeeWideStringObject *q_widepath; /*< [1..1] Query to iterate. */
  DEE_A_REF DeeUtf8StringObject *q_utf8path; /*< [1..1] Query to iterate. */
  DEE_A_REF DeeAnyStringObject  *q_path; /*< [1..1] Query to iterate. */
 };
};
struct DeeListObject;

#ifdef DEE_PLATFORM_WINDOWS
struct DeeFSDirIteratorWin32Data {
 HANDLE           diw_handle; /*< Find-handle. */
 union{
 WIN32_FIND_DATAA diw_data;   /*< Common find data (only use members with offset < Dee_OFFSETOF(WIN32_FIND_DATAA,cFileName)). */
 WIN32_FIND_DATAA diw_utf8;   /*< UTF-8 Iterator data. */
 WIN32_FIND_DATAW diw_wide;   /*< WIDE Iterator data. */
 };
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
 };
};
DEE_COMPILER_MSVC_WARNING_POP

#ifdef DEE_PLATFORM_WINDOWS
#define DeeFSDirIterator_DONE(ob) (((DeeFSDirIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_win.diw_handle == INVALID_HANDLE_VALUE)
#elif defined(DEE_PLATFORM_UNIX)
#define DeeFSDirIterator_DONE(ob) (((DeeFSDirIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_unix.diu_dfd == NULL)
#else
#define DeeFSDirIterator_DONE(ob) 1
#endif

#if DEE_PLATFORM_HAVE_IO
#ifndef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
static void _deeerror_cannot_list_non_directory_fileio(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *path) {
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Cannot list non-directory %r",
                     DeeFileIO_FILE(path));
}
#endif
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFileIO_ListDir(DEE_A_IN_OBJECT(DeeFileIOObject) const *path) {
#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
 DeeFSDirObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DeeFileIO_Check(path));
 result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%p)",DeeFileIO_FILE(path));
 if (DEE_LIKELY(result)) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if ((result->d_iopath = (DeeFileIOObject *)path) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
#else
 DEE_ASSERT(DeeObject_Check(path) && DeeFileIO_Check(path));
 if (DeeFileIO_HANDLE(path) != DEE_FILEIO_INVALID_HANDLE) {
  _deeerror_cannot_list_non_directory_fileio(path);
  return NULL;
 }
 return _DeeFS_ListDirObject(DeeFileIO_FILE(path));
#endif
}
#endif

#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
DEE_STATIC_INLINE(int) _DeeFSDirIterator_InitFromFileHandle(
 DeeFSDirIteratorObject *self, DeeIOHandle handle) {
#if 1 || defined(DEE_PLATFORM_UNIX) && DEE_HAVE_FOPENDIR
 DEE_ASSERT(DeeObject_Check(self) && (
  DeeFSDirIterator_Check(self)||DeeFSQueryIterator_Check(self)));
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if ((self->di_unix.diu_dfd = fdopendir(handle)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "fdopendir(%d) : %K",handle,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)self,handle;
 return 0;
#endif
}
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFSDirIterator_WideInit(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFSDirIterator_Utf8Init(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DEE_A_IN_Z Dee_Utf8Char const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_Utf8IsVirtualPath(path)) {
  int temp; struct DeeVFSFile vfsfile; DeeObject *re_fslink;
  if ((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&vfsfile,&re_fslink)) == -1) return -1;
  if (temp == 0) {
   if (re_fslink) {
    temp = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(re_fslink));
    Dee_DECREF(re_fslink);
    return temp;
   }
   if (!vfsfile.vf_enum) {
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
  DEE_ASSERT(DeeObject_Check(self) && (
   DeeFSDirIterator_Check(self)||DeeFSQueryIterator_Check(self)));
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
  while ((search_path = (Dee_Utf8Char *)malloc_nz(((
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
  if (DeeThread_CheckInterrupt() != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(search_end);
#endif
   return -1;
  }
  self->di_win.diw_handle = FindFirstFileA(search_path,&self->di_win.diw_utf8);
  if (self->di_win.diw_handle == INVALID_HANDLE_VALUE) {
   result = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(result) &&
      !DeeFS_Utf8Win32IsPathUNC(path)) {
    DeeObject *temp;
#if !DEE_HAVE_ALLOCA
    free_nn(search_end);
#endif
    if ((temp = DeeFS_Utf8Win32PathUNC(path)) == NULL) return -1;
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
  } else result = 0;
#if !DEE_HAVE_ALLOCA
  free_nn(search_end);
#endif
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  DEE_ASSERT(DeeObject_Check(self) && (
   DeeFSDirIterator_Check(self)||DeeFSQueryIterator_Check(self)));
  self->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_NONE;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if ((self->di_unix.diu_dfd = opendir(path)) == NULL) {
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
  if ((temp = DeeVFS_WideLocateOrReadReFsLink(path,&vfsfile,&re_fslink)) == -1) return -1;
  if (temp == 0) {
   if (re_fslink) {
    temp = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(re_fslink));
    Dee_DECREF(re_fslink);
    return temp;
   }
   if (!vfsfile.vf_enum) {
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
  DEE_ASSERT(DeeObject_Check(self) && (
   DeeFSDirIterator_Check(self)||DeeFSQueryIterator_Check(self)));
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
  while ((search_path = (Dee_WideChar *)malloc_nz(((
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
  if (DeeThread_CheckInterrupt() != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(search_end);
#endif
   return -1;
  }
  self->di_win.diw_handle = FindFirstFileW(search_path,&self->di_win.diw_wide);
  if (self->di_win.diw_handle == INVALID_HANDLE_VALUE) {
   result = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(result) &&
      !DeeFS_WideWin32IsPathUNC(path)) {
    DeeObject *temp;
#if !DEE_HAVE_ALLOCA
    free_nn(search_end);
#endif
    if ((temp = DeeFS_WideWin32PathUNC(path)) == NULL) return -1;
    result = _DeeFSDirIterator_WideInit(self,DeeWideString_STR(temp));
    Dee_DECREF(temp);
    return result;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "FindFirstFileW(%lq) : %K",
                       search_path,DeeSystemError_Win32ToString((unsigned long)result));
   result = -1;
  } else result = 0;
#if !DEE_HAVE_ALLOCA
  free_nn(search_end);
#endif
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(path_ob));
  if (result == 0) self->di_flags |= DEE_FS_DIR_ITERATOR_FLAGS_WIDE;
  Dee_DECREF(path_ob);
  return result;
#endif
 }
}
DEE_STATIC_INLINE(int) _DeeFSDirIterator_Init(
 struct DeeFSDirIteratorObject *self, DeeAnyStringObject *path) {
 int result;
 if (DeeWideString_Check(path))
  return _DeeFSDirIterator_WideInit(self,DeeWideString_STR(path));
 if ((path = (DeeAnyStringObject *)DeeUtf8String_Cast((DeeObject *)path)) == NULL) return -1;
 result = _DeeFSDirIterator_Utf8Init(self,DeeUtf8String_STR(path));
 Dee_DECREF(path);
 return result;
}

static void _DeeFSDirIterator_Quit(DeeFSDirIteratorObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_VFS)!=0) {
  DEE_ASSERT(!self->di_vfs.div_elemlist || DeeObject_Check(self->di_vfs.div_elemlist));
  Dee_XDECREF(self->di_vfs.div_elemlist);
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
 {
#if defined(DEE_PLATFORM_WINDOWS)
  if (self->di_win.diw_handle != INVALID_HANDLE_VALUE &&
      !FindClose(self->di_win.diw_handle)) SetLastError(0);
#elif defined(DEE_PLATFORM_UNIX)
  if (self->di_unix.diu_dfd &&
      closedir(self->di_unix.diu_dfd) != 0) errno = 0;
#endif
 }
}
static int _DeeFSDirIterator_Yield(DeeFSDirIteratorObject *self, DeeObject **result) {
#if defined(DEE_PLATFORM_WINDOWS)
 DEE_ASSERT(DeeObject_Check(self) && (
  DeeFSDirIterator_Check(self) ||
  DeeFSQueryIterator_Check(self)));
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_VFS)!=0) {
  int error;
  DEE_ASSERT(self->di_vfs.div_yield);
  if ((error = (*self->di_vfs.div_yield)(self,result)) == 0) {
   if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0) {
    if (DeeWideString_InplaceCast((DeeObject const **)result) != 0) {
err_r: Dee_DECREF(*result); return -1;
    }
   } else {
    if (DeeUtf8String_InplaceCast((DeeObject const **)result) != 0) goto err_r;
   }
  }
  return error;
 } else
#endif
 {
  if (self->di_win.diw_handle == INVALID_HANDLE_VALUE) return 1;
  if ((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0) {
again_wide:
   if (DeeThread_CheckInterrupt() != 0) return -1;
   if (!FindNextFileW(self->di_win.diw_handle,&self->di_win.diw_wide)) {
    DWORD error = DeeSystemError_Win32Consume();
    if (error == ERROR_NO_MORE_FILES) {
     self->di_win.diw_handle = INVALID_HANDLE_VALUE;
     return 1; // Iterator end
    }
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "FindNextFileW(%p) : %K",
                        self->di_win.diw_handle,
                        DeeSystemError_Win32ToString(error));
    return -1;
   }
   if (self->di_win.diw_wide.cFileName[0] == '.' && (
    self->di_win.diw_wide.cFileName[1] == 0 || (
    self->di_win.diw_wide.cFileName[1] == '.' &&
    self->di_win.diw_wide.cFileName[2] == 0))) goto again_wide;
   if ((*result = DeeWideString_New(self->di_win.diw_wide.cFileName)) == NULL) return -1;
   return 0;
  } else {
again_utf8:
   if (DeeThread_CheckInterrupt() != 0) return -1;
   if (!FindNextFileA(self->di_win.diw_handle,&self->di_win.diw_utf8)) {
    DWORD error = DeeSystemError_Win32Consume();
    if (error == ERROR_NO_MORE_FILES) {
     self->di_win.diw_handle = INVALID_HANDLE_VALUE;
     return 1; // Iterator end
    }
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "FindNextFileA(%p) : %K",
                        self->di_win.diw_handle,
                        DeeSystemError_Win32ToString(error));
    return -1;
   }
   if (self->di_win.diw_utf8.cFileName[0] == '.' && (
    self->di_win.diw_utf8.cFileName[1] == 0 || (
    self->di_win.diw_utf8.cFileName[1] == '.' &&
    self->di_win.diw_utf8.cFileName[2] == 0))) goto again_utf8;
   if ((*result = DeeUtf8String_New(self->di_win.diw_utf8.cFileName)) == NULL) return -1;
   return 0;
  }
 }
#elif defined(DEE_PLATFORM_UNIX)
again:
 if (!self->di_unix.diu_dfd) return 1;
 if (DeeThread_CheckInterrupt() != 0) return -1;
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
 if ((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
  ? DeeWideString_FromUtf8StringWithLength(self->di_unix.diu_dp->d_namlen,self->di_unix.diu_dp->d_name)
  : DeeUtf8String_FromUtf8StringWithLength(self->di_unix.diu_dp->d_namlen,self->di_unix.diu_dp->d_name)
  ) == NULL) return -1;
#elif defined(_D_EXACT_NAMLEN)
 if ((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
  ? DeeWideString_FromUtf8StringWithLength(_D_EXACT_NAMLEN(self->di_unix.diu_dp),self->di_unix.diu_dp->d_name)
  : DeeUtf8String_FromUtf8StringWithLength(_D_EXACT_NAMLEN(self->di_unix.diu_dp),self->di_unix.diu_dp->d_name)
  ) == NULL) return -1;
#else
 if ((*result = (self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_WIDE)!=0
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
 DeeFSDirIteratorObject          ob_base;
 union{
  DEE_A_REF DeeUtf8StringObject *ob_utf8pattern;
  DEE_A_REF DeeWideStringObject *ob_widepattern;
 };
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
  if (_DeeFSDirIterator_Utf8Init(&self->ob_base,name_cwd) == -1) return -1;
  Dee_INCREF(self->ob_utf8pattern = query);
 } else {
  if ((self->ob_utf8pattern = (DeeUtf8StringObject *)DeeUtf8String_New(path_end)) == NULL) return -1;
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
  while ((path = (Dee_Utf8Char *)malloc_nz(((path_size = (
   Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_Utf8Char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   Dee_DECREF(self->ob_utf8pattern);
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(path,path_begin,path_size*sizeof(Dee_Utf8Char));
  path[path_size] = 0;
  temp = _DeeFSDirIterator_Utf8Init(&self->ob_base,path);
#if !DEE_HAVE_ALLOCA
  free_nn(path);
#endif
  if (temp == -1) {
   Dee_DECREF(self->ob_utf8pattern);
   return -1;
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
  if (_DeeFSDirIterator_WideInit(&self->ob_base,name_cwd) == -1) return -1;
  Dee_INCREF(self->ob_widepattern = query);
 } else {
  if ((self->ob_widepattern = (DeeWideStringObject *)DeeWideString_New(path_end)) == NULL) return -1;
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
  while ((path = (Dee_WideChar *)malloc_nz(((path_size = (
   Dee_size_t)(path_end-path_begin))+1)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   Dee_DECREF(self->ob_widepattern);
   DeeError_NoMemory();
   return -1;
  }
#endif
  memcpy(path,path_begin,path_size*sizeof(Dee_WideChar));
  path[path_size] = 0;
  temp = _DeeFSDirIterator_WideInit(&self->ob_base,path);
#if !DEE_HAVE_ALLOCA
  free_nn(path);
#endif
  if (temp == -1) {
   Dee_DECREF(self->ob_widepattern);
   return -1;
  }
 }
 return 0;
}
DEE_STATIC_INLINE(int) _DeeFSQueryIterator_Init(
 struct DeeFSQueryIteratorObject *self, DeeAnyStringObject const *query) {
 int result;
 if (DeeWideString_Check(query)) 
  return _DeeFSQueryIterator_WideInit(self,(DeeWideStringObject *)query);
 if ((query = (DeeAnyStringObject *)DeeUtf8String_Cast((DeeObject *)query)) == NULL) return -1;
 result = _DeeFSQueryIterator_Utf8Init(self,(DeeUtf8StringObject *)query);
 Dee_DECREF(query);
 return result;
}

static void _deefsqueryiterator_tp_dtor(struct DeeFSQueryIteratorObject *self) {
 Dee_DECREF(self->ob_utf8pattern);
 _DeeFSDirIterator_Quit(&self->ob_base);
}

static int _DeeFSQueryIterator_Yield(struct DeeFSQueryIteratorObject *self, DeeObject **result) {
 int error;
 while ((error = _DeeFSDirIterator_Yield(&self->ob_base,result)) == 0) {
  if (DeeWideString_Check(*result)
#ifdef DEE_PLATFORM_WINDOWS
      ? Dee_WideStrCaseWMatch(DeeWideString_STR(*result),DeeWideString_STR(self->ob_utf8pattern))
      : Dee_Utf8StrCaseWMatch(DeeUtf8String_STR(*result),DeeUtf8String_STR(self->ob_widepattern))
#else
      ? Dee_WideStrWMatch(DeeWideString_STR(*result),DeeWideString_STR(self->ob_utf8pattern))
      : Dee_Utf8StrWMatch(DeeUtf8String_STR(*result),DeeUtf8String_STR(self->ob_widepattern))
#endif
      ) break;
  Dee_DECREF(*result);
 }
 return error;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeFSDirObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%s)",path)) != NULL)) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if ((result->d_utf8path = (DeeUtf8StringObject *)DeeUtf8String_New(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeFSDirObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%ls)",path)) != NULL)) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if ((result->d_widepath = (DeeWideStringObject *)DeeWideString_New(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
_DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeFSDirObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSDirObject,"dir(%p)",path)) != NULL)) {
  DeeObject_INIT(result,&DeeFSDir_Type);
  if (DeeWideString_Check(path)) Dee_INCREF(path);
  else if ((path = DeeUtf8String_Cast(path)) == NULL) {
   _DeeObject_DELETE(&DeeFSDir_Type,result);
   return NULL;
  }
  Dee_INHERIT_REF(result->d_path,*(DeeAnyStringObject **)&path);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *query) {
 DeeFSQueryObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%s)",query)) != NULL)) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if ((result->q_utf8path = (DeeUtf8StringObject *)DeeUtf8String_New(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *query) {
 DeeFSQueryObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%ls)",query)) != NULL)) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if ((result->q_widepath = (DeeWideStringObject *)DeeWideString_New(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
_DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *query) {
 DeeFSQueryObject *result;
 if (DEE_LIKELY((result = DeeObject_MALLOCF(DeeFSQueryObject,"query(%p)",query)) != NULL)) {
  DeeObject_INIT(result,&DeeFSQuery_Type);
  if (DeeWideString_Check(query)) Dee_INCREF(query);
  else if ((query = DeeUtf8String_Cast(query)) == NULL) {
   _DeeObject_DELETE(&DeeFSQuery_Type,result);
   return NULL;
  }
  Dee_INHERIT_REF(result->q_path,*(DeeAnyStringObject **)&query);
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *path_ob,*result;
 if ((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *path_ob,*result;
 if ((path_ob = DeeFS_WidePathExpand(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *
DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 result = _DeeFS_ListDirObject(path);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *path_ob,*result;
 if ((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *path_ob,*result;
 if ((path_ob = DeeFS_WidePathExpand(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *
DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *result;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return NULL;
 result = _DeeFS_QueryObject(path);
 Dee_DECREF(path);
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Dir VTable
static int _deefsdir_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeFSDirObject *self, DeeFSDirObject *right) {
 Dee_INCREF(self->d_path = right->d_path);
 return 0;
}
static int _deefsdir_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeFSDirObject *self, DeeObject *args) {
 DeeObject *path;
 if (DeeTuple_Unpack(args,"o:dir",&path) == -1) return -1;
 if (DeeFileIO_Check(path)) {
#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
  Dee_INCREF(path);
#else
  if (DeeFileIO_HANDLE(path) != DEE_FILEIO_INVALID_HANDLE) {
   _deeerror_cannot_list_non_directory_fileio(path);
   return -1;
  }
  Dee_INCREF(path = DeeFileIO_FILE(path));
#endif
 } else {
  if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
  if (!DeeWideString_Check(path) && !DeeUtf8String_Check(path) &&
      DeeUtf8String_InplaceCast((DeeObject const **)&path) == -1) {
   Dee_DECREF(path);
   return -1;
  }
 }
 Dee_INHERIT_REF(self->d_path,*(DeeAnyStringObject **)&path);
 return 0;
}
static int _deefsquery_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFSQueryObject *self, DeeObject *args) {
 DeeObject *path;
 if (DeeTuple_Unpack(args,"o:query",&path) == -1) return -1;
 if ((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
 if (!DeeWideString_Check(path) && !DeeUtf8String_Check(path) &&
     DeeUtf8String_InplaceCast((DeeObject const **)&path) != 0) {
  Dee_DECREF(path);
  return -1;
 }
 Dee_INHERIT_REF(self->q_path,*(DeeAnyStringObject **)&path);
 return 0;
}
static void _deefsdir_tp_dtor(DeeFSDirObject *self) { Dee_DECREF(self->d_path); }
DEE_VISIT_PROC(_deefsdir_tp_visit,DeeFSDirObject *self) { Dee_VISIT(self->d_path); }
static DeeObject *_deefsdir_tp_str(DeeFSDirObject *self) { return DeeString_Newf("<dir: %r>",self->d_path); }
static DeeObject *_deefsquery_tp_str(DeeFSQueryObject *self) { return DeeString_Newf("<query: %r>",self->q_path); }
static DeeObject *_deefsdir_tp_seq_iter_self(DeeFSDirObject *self) {
 DeeFSDirIteratorObject *result;
 if ((result = DeeObject_MALLOCF(DeeFSDirIteratorObject,
  "dir.iterator(%p)",self)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeFSDirIterator_Type);
 if ((
#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
  DeeFileIO_Check(self->d_iopath) ? _DeeFSDirIterator_InitFromFileHandle(
   result,DeeFileIO_HANDLE(self->d_iopath)) :
#endif
  _DeeFSDirIterator_Init(result,self->d_path)) == -1) {
  _DeeObject_DELETE(&DeeFSDirIterator_Type,result);
  result = NULL;
 }
 return (DeeObject *)result;
}
static DeeObject *_deefsquery_tp_seq_iter_self(DeeFSDirObject *self) {
 struct DeeFSQueryIteratorObject *result;
 result = DeeObject_MALLOCF(struct DeeFSQueryIteratorObject,"query.iterator(%p)",self);
 if (!result) return NULL;
 DeeObject_INIT(result,&DeeFSQueryIterator_Type);
 if (_DeeFSQueryIterator_Init(result,self->d_path) == -1) {
  _DeeObject_DELETE(&DeeFSQueryIterator_Type,result);
  result = NULL;
 }
 return (DeeObject *)result;
}


static int _deefsdiriterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 struct DeeFSDirIteratorObject *self, DeeObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack(args,"o:dir.iterator",&arg) == -1) return -1;
 if (DeeFSDir_Check(arg)) {
  return _DeeFSDirIterator_Init(self,
#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
   DeeFileIO_Check(((DeeFSDirObject *)arg)->d_iopath)
   ? (DeeAnyStringObject *)DeeFileIO_FILE(((DeeFSDirObject *)arg)->d_iopath) :
#endif
   ((DeeFSDirObject *)arg)->d_path);
 } else if (DeeFileIO_Check(arg)) {
#ifdef DEE_PRIVATE_FS_DIR_PATH_MAYBE_FILEIO
  return _DeeFSDirIterator_InitFromFileHandle(self,DeeFileIO_HANDLE(arg));
#else
  return _DeeFSDirIterator_Init(self,(DeeAnyStringObject *)DeeFileIO_FILE(arg));
#endif
 } else {
  return _DeeFSDirIterator_Init(self,(DeeAnyStringObject *)arg);
 }
}
static int _deefsqueryiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), struct DeeFSQueryIteratorObject *self, DeeObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack(args,"o:dir.iterator",&arg) == -1) return -1;
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
 if (tm_access && ((*tm_access = DeeTime_NewFromWin32FileTime(
  &self->di_win.diw_data.ftLastAccessTime)) == NULL)) return -1;
 if (tm_creation && ((*tm_creation = DeeTime_NewFromWin32FileTime(
  &self->di_win.diw_data.ftCreationTime)) == NULL)) {
err_tm_a: if (tm_access) Dee_DECREF(*tm_access); return -1;
 }
 if (tm_modification && ((*tm_modification = DeeTime_NewFromWin32FileTime(
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
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
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
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
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
