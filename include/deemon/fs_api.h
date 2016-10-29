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
#ifndef GUARD_DEEMON_FS_API_H
#define GUARD_DEEMON_FS_API_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

#ifdef DEE_PLATFORM_UNIX
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_IO_HANDLE
DEE_PRIVATE_DECL_DEE_IO_HANDLE
#undef DEE_PRIVATE_DECL_DEE_IO_HANDLE
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#define DEE_FS_EXT_SEP     '.'
#define DEE_FS_EXT_SEP_S   "."
#ifdef DEE_PLATFORM_WINDOWS
#define DEE_FS_DELIM       ';'
#define DEE_FS_DELIM_S     ";"
#define DEE_FS_SEP         '\\'
#define DEE_FS_SEP_S       "\\"
#define DEE_FS_ALT_SEP     '/'
#define DEE_FS_ALT_SEP_S   "/"
#define DEE_FS_DRIVE_SEP   ':'
#define DEE_FS_DRIVE_SEP_S ":"
#else
#define DEE_FS_DELIM       ':'
#define DEE_FS_DELIM_S     ":"
#define DEE_FS_SEP         '/'
#define DEE_FS_SEP_S       "/"
#define DEE_FS_ALT_SEP     '\\'
#define DEE_FS_ALT_SEP_S   "\\"
#endif

#ifdef DEE_PLATFORM_WINDOWS
#define DEE_FS_ISSEP(ch) ((ch)=='/'||(ch)=='\\')
#else
#define DEE_FS_ISSEP(ch) ((ch)=='/')
#endif

#ifndef DEE_PLATFORM_WINDOWS
#ifdef DEE_TYPES_SIZEOF_UID_T
DEE_STATIC_ASSERT(sizeof(uid_t) == DEE_TYPES_SIZEOF_UID_T);
#endif
#ifdef DEE_TYPES_SIZEOF_GID_T
DEE_STATIC_ASSERT(sizeof(gid_t) == DEE_TYPES_SIZEOF_GID_T);
#endif
#ifdef DEE_TYPES_SIZEOF_MODE_T
DEE_STATIC_ASSERT(sizeof(mode_t) == DEE_TYPES_SIZEOF_MODE_T);
#endif
#endif

#ifdef DEE_PRIVATE_DECL_DEE_MODE_T
DEE_PRIVATE_DECL_DEE_MODE_T
#undef DEE_PRIVATE_DECL_DEE_MODE_T
#endif
typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_UID_T) Dee_uid_t;
typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_GID_T) Dee_gid_t;


DEE_OBJECT_DEF(DeeFSDirObject);
DEE_OBJECT_DEF(DeeFSDirIteratorObject);
DEE_OBJECT_DEF(DeeFSQueryObject);
DEE_OBJECT_DEF(DeeFSQueryIteratorObject);
#ifdef DEE_PLATFORM_WINDOWS
DEE_OBJECT_DEF(DeeFSWin32SIDObject);
#endif

struct DeeListObject;
struct DeeTimeObject;

//////////////////////////////////////////////////////////////////////////
// Current working directory get/set
// NOTE: 'path' may be relative, allowing you to navigate from the current cwd
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetCwd(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetCwd(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ChDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
#define DeeFS_GetCwd DeeFS_Utf8GetCwd

//////////////////////////////////////////////////////////////////////////
// Returns the home directory of the current user
// Always ends with a trailing backslash
// On Windows:
//   1. Tries to return $HOME
//   2. Tries to return $USERPROFILE
//   3. Tries to return $HOMEDRIVE+$HOMEPATH
//   4. fails with an exception (returns NULL)
// On Unix:
//   1. Tries to return $HOME
//   2. return the user's home dir from passwd (can cause an error)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetHome(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetHome(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetHomeUser(DEE_A_IN_Z Dee_Utf8Char const *user) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetHomeUser(DEE_A_IN_Z Dee_WideChar const *user) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_GetHomeUserObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *user) DEE_ATTRIBUTE_NONNULL((1));
#define DeeFS_GetHome DeeFS_Utf8GetHome

//////////////////////////////////////////////////////////////////////////
// Returns the temp path, as indicated by the OS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetTmp(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetTmp(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Utf8GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_WideGetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_GetTmpNameObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) _DeeFS_GetTmpNameObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeFS_GetTmp DeeFS_Utf8GetTmp

#ifdef DEE_PLATFORM_WINDOWS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetDllDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSystemDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSystemWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSysWow64Directory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetDllDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSystemDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSystemWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSysWow64Directory(void);
#define DeeFS_Win32GetDllDirectory           DeeFS_Utf8Win32GetDllDirectory
#define DeeFS_Win32GetSystemDirectory        DeeFS_Utf8Win32GetSystemDirectory
#define DeeFS_Win32GetWindowsDirectory       DeeFS_Utf8Win32GetWindowsDirectory
#define DeeFS_Win32GetSystemWindowsDirectory DeeFS_Utf8Win32GetSystemWindowsDirectory
#define DeeFS_Win32GetSysWow64Directory      DeeFS_Utf8Win32GetSysWow64Directory
#endif


//////////////////////////////////////////////////////////////////////////
// List/Get/Del/Set environment vars
// NOTE: 'DeeFS_ListEnv' returns a list of 2-elem string tuples,
//        where the first is the env name and the second its value.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) *) DeeFS_WideListEnv(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) *) DeeFS_Utf8ListEnv(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_SetListEnv(DEE_A_IN_OBJECT(struct DeeListObject) const *v) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *) DeeFS_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *name, DEE_A_IN_Z Dee_Utf8Char const *value) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *name, DEE_A_IN_Z Dee_WideChar const *value) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_HasEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_GetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeAnyStringObject) *) DeeFS_TryGetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_DelEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_SetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name, DEE_A_IN_OBJECT(DeeAnyStringObject) const *value) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeFS_ListEnv   DeeFS_Utf8ListEnv

// Returns the head of a filename (including trail). e.g.: "/foo/bar.txt" --> "/foo/"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathHead(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathHead(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathHeadObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Returns the tail of a filename. e.g.: "/foo/bar.txt" --> "bar.txt"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathTail(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathTail(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathTailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Returns the name of a filename. e.g.: "/foo/bar.txt" --> "bar"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathFile(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathFile(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Returns the extension of a filename. e.g.: "/foo/bar.txt" --> ".txt" (Return "" if no extension is found)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExt(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExt(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathExtObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Returns the drive of a filename (including trail). e.g.: "C:/foo/bar.txt" --> "C:/" (Always returns the root folder "/" on non-windows systems; return "" if the path is relative)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathDrive(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathDrive(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathDriveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Appends a missing trailing slash to the given path. e.g.: "/foo/bar" --> "/foo/bar/"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathIncTrail(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathIncTrail(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathIncTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
// Exclude all trailing slashes from a given path. e.g.: "/foo/bar///" --> "/foo/bar"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExcTrail(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExcTrail(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathExcTrailObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the absolute version of 'path', using 'cwd' as the current working directory
// NOTE: if 'cwd' is not absolute, if will be made absolute, using the real cwd
// NOTE: if 'path' is already absolute, it will be returned unmodified
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathAbs(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathAbs(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Utf8PathAbs(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_WidePathAbs(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathAbsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) _DeeFS_PathAbsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns a relative path starting in 'cwd' and leading to 'path'
// NOTE: if 'cwd' is not absolute, if will be made absolute, using the real cwd
// NOTE: if 'path' is already relative, it will be forced absolute using the real cwd ('path = fs.path.abs(path,fs.getcwd())')
// NOTE: On windows, return the absolute version of 'path' if it's located on a different drive than 'cwd'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathRel(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathRel(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Utf8PathRel(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_WidePathRel(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathRelObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) _DeeFS_PathRelObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Joins an array of strings, to form a valid path
// - Slashes are stripped from all path elements
// - Empty paths are ignored
// - All paths are joined with the platform-specific slash
// NOTE: This function throws an 'Error.TypeError' if any path entry isn't a string
// NOTE: Path arguments are not expanded
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathJoinObject(
 DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) /*DeeAnyStringObject*/DeeObject const *const *pathv);

//////////////////////////////////////////////////////////////////////////
// Expand the user folder in a path name
// Replaces a '~' prefix with the current user's home path
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpandUser(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpandUser(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathExpandUserObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Expands environment variables in the given path
// Recognized formats: '${ENV_VAR}', '$ENV_VAR', '%ENV_VAR%'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpandVars(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpandVars(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathExpandVarsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Expands vars + user in a given path
// NOTE: This function is used to format a given path and retry operations like
//       'file.io', '__builtin_fs_chdir', '__builtin_fs_remove'
// NOTE: This function is only used if the call would have otherwise failed.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PathExpand(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePathExpand(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_PathExpandObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Returns the <last access>, <creation> or <last modification> time of a file/directory 'path'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_Utf8GetATime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_Utf8GetCTime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_Utf8GetMTime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_Utf8GetATime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_Utf8GetCTime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_Utf8GetMTime(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_WideGetATime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_WideGetCTime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_WideGetMTime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_WideGetATime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_WideGetCTime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_WideGetMTime(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetATimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetCTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetMTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetATimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetCTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetMTimeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns a 3-elem tuple equivalent to
// '(DeeFS_GetATime(path),
//   DeeFS_GetCTime(path),
//   DeeFS_GetMTime(path))'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) _DeeFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) _DeeFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) _DeeFS_GetTimesObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFS_GetTimesObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Optionaly returns all file times, storing new DeeTimeObjects in the 'tm_*' pointers
// - Each tm_* argument can be NULL to ignore that specific time value.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8SetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8GetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8SetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideSetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideGetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideSetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetTimes2Object(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_SetTimes2Object(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_GetTimes2Object(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_SetTimes2Object(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) DEE_ATTRIBUTE_NONNULL((1));
#define /*DEE_A_EXEC*/ DeeFS_SetATimeObject(path,tm_access)       DeeFS_SetTimes2Object(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ DeeFS_SetCTimeObject(path,tm_creation)     DeeFS_SetTimes2Object(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ DeeFS_SetMTimeObject(path,tm_modification) DeeFS_SetTimes2Object(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ _DeeFS_Utf8SetATime(path,tm_access)        _DeeFS_Utf8SetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_Utf8SetCTime(path,tm_creation)      _DeeFS_Utf8SetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_Utf8SetMTime(path,tm_modification)  _DeeFS_Utf8SetTimes2(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ DeeFS_Utf8SetATime(path,tm_access)         DeeFS_Utf8SetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ DeeFS_Utf8SetCTime(path,tm_creation)       DeeFS_Utf8SetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ DeeFS_Utf8SetMTime(path,tm_modification)   DeeFS_Utf8SetTimes2(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ _DeeFS_WideSetATime(path,tm_access)        _DeeFS_WideSetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_WideSetCTime(path,tm_creation)      _DeeFS_WideSetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_WideSetMTime(path,tm_modification)  _DeeFS_WideSetTimes2(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ DeeFS_WideSetATime(path,tm_access)         DeeFS_WideSetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ DeeFS_WideSetCTime(path,tm_creation)       DeeFS_WideSetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ DeeFS_WideSetMTime(path,tm_modification)   DeeFS_WideSetTimes2(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ _DeeFS_SetATime(path,tm_access)            _DeeFS_SetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_SetCTime(path,tm_creation)          _DeeFS_SetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ _DeeFS_SetMTime(path,tm_modification)      _DeeFS_SetTimes2(path,NULL,NULL,tm_modification)
#define /*DEE_A_EXEC*/ DeeFS_SetATime(path,tm_access)             DeeFS_SetTimes2(path,tm_access,NULL,NULL)
#define /*DEE_A_EXEC*/ DeeFS_SetCTime(path,tm_creation)           DeeFS_SetTimes2(path,NULL,tm_creation,NULL)
#define /*DEE_A_EXEC*/ DeeFS_SetMTime(path,tm_modification)       DeeFS_SetTimes2(path,NULL,NULL,tm_modification)


//////////////////////////////////////////////////////////////////////////
// Returns '1' if certain conditions apply for a given 'path'; '0' otherwise
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsAbs(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsAbs(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsAbsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDriveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsMountObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsHiddenObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsExecutableObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsCharDevObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsBlockDevObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFiFoObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsSocketObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsAbsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDriveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsMountObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsHiddenObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsExecutableObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsCharDevObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsBlockDevObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFiFoObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsSocketObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsFile(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsDir(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsLink(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsDrive(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsMount(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsHidden(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsExecutable(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsCharDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsBlockDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsFiFo(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFileIO_IsSocket(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp) DEE_ATTRIBUTE_NONNULL((1));
#endif

//////////////////////////////////////////////////////////////////////////
// Returns '0' if a path describes an existing file / directory
// - The path may contain wildcards, like '*' or '?'
// Equivalent to '(#fs::query(path) != 0)'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_ExistsObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Remove anything / a file
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8RmFile(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideRmFile(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8RmFile(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideRmFile(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_RemoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_RmFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_RemoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmFileObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Create / Remove a directory
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8RmDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideRmDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8RmDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideRmDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_MkDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_RmDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_MkDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Copy/Move/Link a file
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_CopyObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_MoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_LinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_CopyObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_MoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_LinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));

typedef DEE_A_RET_EXCEPT(-1) int (*DeeFS_ProcessFunc)(double progress, void *closure);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ProgressCallObject(double progress, void *closure);

//////////////////////////////////////////////////////////////////////////
// Copy a file with a progress callback
// >> The progress callback is executed with 0.0 <= 'progress' <= 1.0 and at least twice for '0.0' and '1.0'
// >> If the progress callback returns non-zero, the copy operation is aborted and any data already copied is discarded and the function return -1 (while assuming that a deemon error was set)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8CopyWithProgress(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideCopyWithProgress(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8CopyWithProgress(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideCopyWithProgress(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_CopyObjectWithProgress(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_CopyObjectWithProgress(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));

// Call a deemon object 'callback((double)progress)' as progress callback
#define /*DEE_A_EXEC*/ _DeeFS_Utf8CopyWithProgressCallable(src,dst,callback)   _DeeFS_Utf8CopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/  DeeFS_Utf8CopyWithProgressCallable(src,dst,callback)    DeeFS_Utf8CopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/ _DeeFS_WideCopyWithProgressCallable(src,dst,callback)   _DeeFS_WideCopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/  DeeFS_WideCopyWithProgressCallable(src,dst,callback)    DeeFS_WideCopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/ _DeeFS_CopyWithProgressCallable(src,dst,callback)       _DeeFS_CopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/  DeeFS_CopyWithProgressCallable(src,dst,callback)        DeeFS_CopyWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/ _DeeFS_CopyObjectWithProgressCallable(src,dst,callback) _DeeFS_CopyObjectWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)
#define /*DEE_A_EXEC*/  DeeFS_CopyObjectWithProgressCallable(src,dst,callback)  DeeFS_CopyObjectWithProgress(src,dst,&_DeeFS_ProgressCallObject,callback)


DEE_DATA_DECL(DeeTypeObject) DeeFSDir_Type;
DEE_DATA_DECL(DeeTypeObject) DeeFSDirIterator_Type;
#define DeeFSDir_Check(ob)              DeeObject_InstanceOf(ob,&DeeFSDir_Type)
#define DeeFSDir_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeFSDir_Type)
#define DeeFSDirIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeFSDirIterator_Type)
#define DeeFSDirIterator_Check          DeeFSDirIterator_CheckExact

DEE_DATA_DECL(DeeTypeObject) DeeFSQuery_Type;
DEE_DATA_DECL(DeeTypeObject) DeeFSQueryIterator_Type;
#define DeeFSQuery_Check(ob)              DeeObject_InstanceOf(ob,&DeeFSQuery_Type)
#define DeeFSQuery_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeFSQuery_Type)
#define DeeFSQueryIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeFSQueryIterator_Type)
#define DeeFSQueryIterator_Check          DeeFSQueryIterator_CheckExact

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_Utf8ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_WideListDir(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_ListDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) _DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) _DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) _DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) DeeFS_Utf8Query(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) DeeFS_WideQuery(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) DeeFS_QueryObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *)
 DeeFileIO_ListDir(DEE_A_IN_OBJECT(DeeFileIOObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
#endif

#ifdef DEE_PLATFORM_WINDOWS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetModuleName(DEE_A_IN_OPT /*HMODULE*/void *module);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetModuleName(DEE_A_IN_OPT /*HMODULE*/void *module);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Win32HardLink(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideWin32HardLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Win32HardLink(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideWin32HardLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Win32HardLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeFS_Win32GetModuleName DeeFS_Utf8Win32GetModuleName
#endif

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_GetModObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetModObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_GetMod(DEE_A_IN_OBJECT(DeeFileIOObject) const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
#endif


DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodStringObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodStringObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));

#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Chmod(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_ChmodString(DEE_A_IN_OBJECT(DeeFileIOObject) const *fp, DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1,2));
#endif
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChownObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ChownObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_PLATFORM_WINDOWS
DEE_OBJECT_DEF(DeeFSWin32SIDObject);
DEE_DATA_DECL(DeeTypeObject) DeeFSWin32SID_Type;
#define DeeFSWin32SID_GetUid                DeeFSWin32SID_GetID
#define DeeFSWin32SID_GetGid                DeeFSWin32SID_GetID
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeFSWin32SID_GetID(DEE_A_IN_OBJECT(DeeFSWin32SIDObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFSWin32SID_NewFromSID(/*PSID*/Dee_uintptr_t sid) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFS_Utf8Win32NameToSID(DEE_A_IN_Z Dee_Utf8Char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFS_WideWin32NameToSID(DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFS_Win32NameToSIDObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
#endif


#ifdef DEE_PLATFORM_UNIX
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8PosixUserToUid(DEE_A_IN_Z Dee_Utf8Char const *user, DEE_A_OUT Dee_uid_t *uid) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WidePosixUserToUid(DEE_A_IN_Z Dee_WideChar const *user, DEE_A_OUT Dee_uid_t *uid) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8PosixGroupToGid(DEE_A_IN_Z Dee_Utf8Char const *group, DEE_A_OUT Dee_gid_t *gid) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WidePosixGroupToGid(DEE_A_IN_Z Dee_WideChar const *group, DEE_A_OUT Dee_gid_t *gid) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixUserToUidObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *user, DEE_A_OUT Dee_uid_t *uid) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixGroupToGidObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *group, DEE_A_OUT Dee_gid_t *gid) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the account name associated with a given uid / gid
// - By default this will simply return the login name, but if
//   'full_name' is non-zero, the name of the associated machine/domain will
//   be appended alongside an '@'.
//   e.g.: DeeFS_Utf8UidToUser(...,1) --> "cool@dudepc"
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8UidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideUidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name);
#define DeeFS_UidToUser  DeeFS_Utf8UidToUser
#define DeeFS_GidToGroup DeeFS_Utf8GidToGroup

#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Chown(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *fp,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
#endif


DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_GetOwnObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetOwnObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
#if DEE_PLATFORM_HAVE_IO
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_GetOwn(DEE_A_IN_OBJECT(DeeFileIOObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif


#ifdef DEE_PLATFORM_UNIX
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8PosixGetProcessName(DEE_A_IN pid_t process);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WidePosixGetProcessName(DEE_A_IN pid_t process);
#define DeeFS_PosixGetProcessName DeeFS_Utf8PosixGetProcessName
#endif


// Returns the absolute path to the deemon executable (e.g.: '/usr/bin/deemon')
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetDeemonExecutable(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetDeemonExecutable(void);
#define DeeFS_GetDeemonExecutable DeeFS_Utf8GetDeemonExecutable

// Returns the name of the computer
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetMachineName(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetMachineName(void);
#define DeeFS_GetMachineName DeeFS_Utf8GetMachineName

// Returns the name of the current user
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetUserName(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetUserName(void);
#define DeeFS_GetUserName DeeFS_Utf8GetUserName

// Reads a given file system link
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Utf8ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_WideReadLink(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideReadLink(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) _DeeFS_ReadLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_ReadLinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_PLATFORM_WINDOWS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeFS_Utf8Win32Drives(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeFS_WideWin32Drives(void);
#ifdef DEE_LIMITED_API
extern DEE_A_RET_NOEXCEPT(0) int _Dee_Win32AcquireProcessPrivilege(
 DEE_A_IN_Z Dee_WideChar const *name) DEE_ATTRIBUTE_NONNULL((1));
#endif
#endif

#ifdef DEE_LIMITED_API
#ifdef DEE_PLATFORM_WINDOWS
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeFS_AutoGetUserAndGroup(
 DEE_A_IN DeeObject *arg1, DEE_A_IN_OPT DeeObject *arg2,
 DEE_A_OUT Dee_uid_t *uid, DEE_A_OUT Dee_gid_t *gid,
 DEE_A_OUT DeeObject **buf1, DEE_A_OUT DeeObject **buf2);
#else
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeFS_AutoGetUserAndGroup(
 DEE_A_IN DeeObject *arg1, DEE_A_IN_OPT DeeObject *arg2,
 DEE_A_OUT Dee_uid_t *uid, DEE_A_OUT Dee_gid_t *gid);
#endif
#endif

DEE_DECL_END

#ifdef __cplusplus
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8ChDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChDir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideChDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8ChDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChDir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideChDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_GetHomeUser(DEE_A_IN_Z Dee_Utf8Char const *user) { return DeeFS_Utf8GetHomeUser(user); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_GetHomeUser(DEE_A_IN_Z Dee_WideChar const *user) { return DeeFS_WideGetHomeUser(user); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) { return _DeeFS_Utf8GetTmpName(path,prefix); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_GetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) { return _DeeFS_WideGetTmpName(path,prefix); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) { return DeeFS_Utf8GetTmpName(path,prefix); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_GetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) { return DeeFS_WideGetTmpName(path,prefix); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_HasEnv(DEE_A_IN_Z Dee_Utf8Char const *name) { return DeeFS_Utf8HasEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_HasEnv(DEE_A_IN_Z Dee_WideChar const *name) { return DeeFS_WideHasEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_GetEnv(DEE_A_IN_Z Dee_Utf8Char const *name) { return DeeFS_Utf8GetEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_GetEnv(DEE_A_IN_Z Dee_WideChar const *name) { return DeeFS_WideGetEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *) DeeFS_TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *name) { return DeeFS_Utf8TryGetEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *) DeeFS_TryGetEnv(DEE_A_IN_Z Dee_WideChar const *name) { return DeeFS_WideTryGetEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_DelEnv(DEE_A_IN_Z Dee_Utf8Char const *name) { return DeeFS_Utf8DelEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_DelEnv(DEE_A_IN_Z Dee_WideChar const *name) { return DeeFS_WideDelEnv(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_SetEnv(DEE_A_IN_Z Dee_Utf8Char const *name, DEE_A_IN_Z Dee_Utf8Char const *value) { return DeeFS_Utf8SetEnv(name,value); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_SetEnv(DEE_A_IN_Z Dee_WideChar const *name, DEE_A_IN_Z Dee_WideChar const *value) { return DeeFS_WideSetEnv(name,value); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathHead(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathHead(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathHead(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathHead(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathTail(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathTail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathTail(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathTail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathFile(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathFile(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathExt(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathExt(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathExt(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathExt(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathDrive(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathDrive(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathIncTrail(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathIncTrail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathIncTrail(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathIncTrail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathExcTrail(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathExcTrail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathExcTrail(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathExcTrail(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathAbs(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) { return DeeFS_Utf8PathAbs(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathAbs(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) { return DeeFS_WidePathAbs(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_PathAbs(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) { return _DeeFS_Utf8PathAbs(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_PathAbs(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) { return _DeeFS_WidePathAbs(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathRel(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) { return DeeFS_Utf8PathRel(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathRel(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) { return DeeFS_WidePathRel(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_PathRel(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *cwd) { return _DeeFS_Utf8PathRel(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_PathRel(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *cwd) { return _DeeFS_WidePathRel(path,cwd); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathExpandUser(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathExpandUser(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathExpandUser(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathExpandUser(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathExpandVars(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathExpandVars(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathExpandVars(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathExpandVars(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_PathExpand(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8PathExpand(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_PathExpand(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WidePathExpand(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetATime(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8GetATime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetATime(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideGetATime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetCTime(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8GetCTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetCTime(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideGetCTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetMTime(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8GetMTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) _DeeFS_GetMTime(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideGetMTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetATime(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8GetATime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetATime(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideGetATime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetCTime(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8GetCTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetCTime(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideGetCTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetMTime(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8GetMTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFS_GetMTime(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideGetMTime(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) _DeeFS_GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8GetTimes(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) _DeeFS_GetTimes(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideGetTimes(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFS_GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8GetTimes(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFS_GetTimes(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideGetTimes(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) { return _DeeFS_Utf8GetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) { return _DeeFS_WideGetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_SetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) { return _DeeFS_Utf8SetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_SetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) { return _DeeFS_WideSetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) { return DeeFS_Utf8GetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation, DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) { return DeeFS_WideGetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_SetTimes2(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) { return DeeFS_Utf8SetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_SetTimes2(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation, DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) { return DeeFS_WideSetTimes2(path,tm_access,tm_creation,tm_modification); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsAbs(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsAbs(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsAbs(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFile(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFile(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsLink(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsLink(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsDrive(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsMount(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsMount(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsMount(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsHidden(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsHidden(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsHidden(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsExecutable(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsExecutable(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsExecutable(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsCharDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsCharDev(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsCharDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsBlockDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsBlockDev(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsBlockDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsFiFo(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsFiFo(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsFiFo(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8IsSocket(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_IsSocket(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideIsSocket(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsAbs(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsAbs(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsAbs(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFile(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFile(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsLink(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsLink(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsDrive(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsDrive(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsMount(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsMount(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsMount(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsHidden(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsHidden(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsHidden(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsCharDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsCharDev(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsCharDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsBlockDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsBlockDev(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsBlockDev(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsFiFo(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsFiFo(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsFiFo(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsSocket(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsSocket(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsSocket(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8IsExecutable(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_IsExecutable(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideIsExecutable(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Exists(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Exists(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_Exists(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideExists(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Exists(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Exists(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_Exists(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideExists(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Remove(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Remove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Remove(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideRemove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmFile(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8RmFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmFile(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideRmFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Remove(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Remove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Remove(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideRemove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_RmFile(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8RmFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_RmFile(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideRmFile(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) { return _DeeFS_Utf8MkDir(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_MkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) { return _DeeFS_WideMkDir(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8RmDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmDir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideRmDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_Utf8MkDir(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_MkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_WideMkDir(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_RmDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8RmDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_RmDir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideRmDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return _DeeFS_Utf8Copy(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Copy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return _DeeFS_WideCopy(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return _DeeFS_Utf8Move(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Move(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return _DeeFS_WideMove(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Link(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return _DeeFS_Utf8Link(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Link(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return _DeeFS_WideLink(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return DeeFS_Utf8Copy(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Copy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return DeeFS_WideCopy(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return DeeFS_Utf8Move(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Move(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return DeeFS_WideMove(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Link(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return DeeFS_Utf8Link(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Link(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return DeeFS_WideLink(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_CopyWithProgress(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) { return _DeeFS_Utf8CopyWithProgress(src,dst,progress,closure); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_CopyWithProgress(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) { return _DeeFS_WideCopyWithProgress(src,dst,progress,closure); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeFS_CopyWithProgress(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) { return DeeFS_Utf8CopyWithProgress(src,dst,progress,closure); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeFS_CopyWithProgress(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, DEE_A_IN DeeFS_ProcessFunc progress, void *closure) { return DeeFS_WideCopyWithProgress(src,dst,progress,closure); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8ListDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_ListDir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideListDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_ListDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8ListDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_ListDir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideListDir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) _DeeFS_Query(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Query(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) _DeeFS_Query(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideQuery(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) DeeFS_Query(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Query(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSQueryObject) *) DeeFS_Query(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideQuery(path); }
#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Win32HardLink(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return _DeeFS_Utf8Win32HardLink(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Win32HardLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return _DeeFS_WideWin32HardLink(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Win32HardLink(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { return DeeFS_Utf8Win32HardLink(src,dst); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Win32HardLink(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { return DeeFS_WideWin32HardLink(src,dst); }
#endif
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) { return DeeFS_Utf8GetMod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) { return DeeFS_WideGetMod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) { return _DeeFS_Utf8GetMod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) { return _DeeFS_WideGetMod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) { return _DeeFS_Utf8Chmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Chmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) { return _DeeFS_WideChmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) { return _DeeFS_Utf8ChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) { return _DeeFS_WideChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_Utf8Chmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_WideChmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) { return DeeFS_Utf8ChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) { return DeeFS_WideChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) { return _DeeFS_Utf8Chown(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Chown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) { return _DeeFS_WideChown(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) { return DeeFS_Utf8Chown(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) { return DeeFS_WideChown(path,owner,group); }
#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFS_Win32NameToSID(DEE_A_IN_Z Dee_Utf8Char const *name) { return DeeFS_Utf8Win32NameToSID(name); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFS_Win32NameToSID(DEE_A_IN_Z Dee_WideChar const *name) { return DeeFS_WideWin32NameToSID(name); }
#endif
#ifdef DEE_PLATFORM_UNIX
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixUserToUid(DEE_A_IN_Z Dee_Utf8Char const *user, DEE_A_OUT Dee_uid_t *uid) { return DeeFS_Utf8PosixUserToUid(user,uid); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixUserToUid(DEE_A_IN_Z Dee_WideChar const *user, DEE_A_OUT Dee_uid_t *uid) { return DeeFS_WidePosixUserToUid(user,uid); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixGroupToGid(DEE_A_IN_Z Dee_Utf8Char const *group, DEE_A_OUT Dee_gid_t *gid) { return DeeFS_Utf8PosixGroupToGid(group,gid); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_PosixGroupToGid(DEE_A_IN_Z Dee_WideChar const *group, DEE_A_OUT Dee_gid_t *gid) { return DeeFS_WidePosixGroupToGid(group,gid); }
#endif
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) { return DeeFS_Utf8GetOwn(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeFS_GetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) { return DeeFS_WideGetOwn(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) { return _DeeFS_Utf8GetOwn(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) { return _DeeFS_WideGetOwn(path,owner,group); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8ReadLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_ReadLink(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideReadLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8ReadLink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_ReadLink(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideReadLink(path); }
#else
#define /*DEE_A_EXEC*/ _DeeFS_ChDir            _DeeFS_Utf8ChDir
#define /*DEE_A_EXEC*/ DeeFS_ChDir             DeeFS_Utf8ChDir
#define /*DEE_A_EXEC*/ DeeFS_GetHomeUser       DeeFS_Utf8GetHomeUser
#define /*DEE_A_EXEC*/ _DeeFS_GetTmpName       _DeeFS_Utf8GetTmpName
#define /*DEE_A_EXEC*/ DeeFS_GetTmpName        DeeFS_Utf8GetTmpName
#define /*DEE_A_EXEC*/ DeeFS_HasEnv            DeeFS_Utf8HasEnv
#define /*DEE_A_EXEC*/ DeeFS_GetEnv            DeeFS_Utf8GetEnv
#define /*DEE_A_EXEC*/ DeeFS_TryGetEnv         DeeFS_Utf8TryGetEnv
#define /*DEE_A_EXEC*/ DeeFS_DelEnv            DeeFS_Utf8DelEnv
#define /*DEE_A_EXEC*/ DeeFS_SetEnv            DeeFS_Utf8SetEnv
#define /*DEE_A_EXEC*/ DeeFS_PathHead          DeeFS_Utf8PathHead
#define /*DEE_A_EXEC*/ DeeFS_PathTail          DeeFS_Utf8PathTail
#define /*DEE_A_EXEC*/ DeeFS_PathFile          DeeFS_Utf8PathFile
#define /*DEE_A_EXEC*/ DeeFS_PathExt           DeeFS_Utf8PathExt
#define /*DEE_A_EXEC*/ DeeFS_PathDrive         DeeFS_Utf8PathDrive
#define /*DEE_A_EXEC*/ DeeFS_PathIncTrail      DeeFS_Utf8PathIncTrail
#define /*DEE_A_EXEC*/ DeeFS_PathExcTrail      DeeFS_Utf8PathExcTrail
#define /*DEE_A_EXEC*/ _DeeFS_PathAbs          _DeeFS_Utf8PathAbs
#define /*DEE_A_EXEC*/ DeeFS_PathAbs           DeeFS_Utf8PathAbs
#define /*DEE_A_EXEC*/ _DeeFS_PathRel          _DeeFS_Utf8PathRel
#define /*DEE_A_EXEC*/ DeeFS_PathRel           DeeFS_Utf8PathRel
#define /*DEE_A_EXEC*/ DeeFS_PathExpandUser    DeeFS_Utf8PathExpandUser
#define /*DEE_A_EXEC*/ DeeFS_PathExpandVars    DeeFS_Utf8PathExpandVars
#define /*DEE_A_EXEC*/ DeeFS_PathExpand        DeeFS_Utf8PathExpand
#define /*DEE_A_EXEC*/ _DeeFS_GetATime         _DeeFS_Utf8GetATime
#define /*DEE_A_EXEC*/ _DeeFS_GetCTime         _DeeFS_Utf8GetCTime
#define /*DEE_A_EXEC*/ _DeeFS_GetMTime         _DeeFS_Utf8GetMTime
#define /*DEE_A_EXEC*/ DeeFS_GetATime          DeeFS_Utf8GetATime
#define /*DEE_A_EXEC*/ DeeFS_GetCTime          DeeFS_Utf8GetCTime
#define /*DEE_A_EXEC*/ DeeFS_GetMTime          DeeFS_Utf8GetMTime
#define /*DEE_A_EXEC*/ _DeeFS_GetTimes         _DeeFS_Utf8GetTimes
#define /*DEE_A_EXEC*/ DeeFS_GetTimes          DeeFS_Utf8GetTimes
#define /*DEE_A_EXEC*/ _DeeFS_GetTimes2        _DeeFS_Utf8GetTimes2
#define /*DEE_A_EXEC*/ _DeeFS_SetTimes2        _DeeFS_Utf8SetTimes2
#define /*DEE_A_EXEC*/ DeeFS_GetTimes2         DeeFS_Utf8GetTimes2
#define /*DEE_A_EXEC*/ DeeFS_SetTimes2         DeeFS_Utf8SetTimes2
#define /*DEE_A_EXEC*/ _DeeFS_IsAbs            _DeeFS_Utf8IsAbs
#define /*DEE_A_EXEC*/ _DeeFS_IsFile           _DeeFS_Utf8IsFile
#define /*DEE_A_EXEC*/ _DeeFS_IsDir            _DeeFS_Utf8IsDir
#define /*DEE_A_EXEC*/ _DeeFS_IsLink           _DeeFS_Utf8IsLink
#define /*DEE_A_EXEC*/ _DeeFS_IsDrive          _DeeFS_Utf8IsDrive
#define /*DEE_A_EXEC*/ _DeeFS_IsMount          _DeeFS_Utf8IsMount
#define /*DEE_A_EXEC*/ _DeeFS_IsHidden         _DeeFS_Utf8IsHidden
#define /*DEE_A_EXEC*/ _DeeFS_IsExecutable     _DeeFS_Utf8IsExecutable
#define /*DEE_A_EXEC*/ _DeeFS_IsCharDev        _DeeFS_Utf8IsCharDev
#define /*DEE_A_EXEC*/ _DeeFS_IsBlockDev       _DeeFS_Utf8IsBlockDev
#define /*DEE_A_EXEC*/ _DeeFS_IsFiFo           _DeeFS_Utf8IsFiFo
#define /*DEE_A_EXEC*/ _DeeFS_IsSocket         _DeeFS_Utf8IsSocket
#define /*DEE_A_EXEC*/ DeeFS_IsAbs             DeeFS_Utf8IsAbs
#define /*DEE_A_EXEC*/ DeeFS_IsFile            DeeFS_Utf8IsFile
#define /*DEE_A_EXEC*/ DeeFS_IsDir             DeeFS_Utf8IsDir
#define /*DEE_A_EXEC*/ DeeFS_IsLink            DeeFS_Utf8IsLink
#define /*DEE_A_EXEC*/ DeeFS_IsDrive           DeeFS_Utf8IsDrive
#define /*DEE_A_EXEC*/ DeeFS_IsMount           DeeFS_Utf8IsMount
#define /*DEE_A_EXEC*/ DeeFS_IsHidden          DeeFS_Utf8IsHidden
#define /*DEE_A_EXEC*/ DeeFS_IsCharDev         DeeFS_Utf8IsCharDev
#define /*DEE_A_EXEC*/ DeeFS_IsBlockDev        DeeFS_Utf8IsBlockDev
#define /*DEE_A_EXEC*/ DeeFS_IsFiFo            DeeFS_Utf8IsFiFo
#define /*DEE_A_EXEC*/ DeeFS_IsSocket          DeeFS_Utf8IsSocket
#define /*DEE_A_EXEC*/ DeeFS_IsExecutable      DeeFS_Utf8IsExecutable
#define /*DEE_A_EXEC*/ _DeeFS_Exists           _DeeFS_Utf8Exists
#define /*DEE_A_EXEC*/ DeeFS_Exists            DeeFS_Utf8Exists
#define /*DEE_A_EXEC*/ _DeeFS_Remove           _DeeFS_Utf8Remove
#define /*DEE_A_EXEC*/ _DeeFS_RmFile           _DeeFS_Utf8RmFile
#define /*DEE_A_EXEC*/ DeeFS_Remove            DeeFS_Utf8Remove
#define /*DEE_A_EXEC*/ DeeFS_RmFile            DeeFS_Utf8RmFile
#define /*DEE_A_EXEC*/ _DeeFS_MkDir            _DeeFS_Utf8MkDir
#define /*DEE_A_EXEC*/ _DeeFS_RmDir            _DeeFS_Utf8RmDir
#define /*DEE_A_EXEC*/ DeeFS_MkDir             DeeFS_Utf8MkDir
#define /*DEE_A_EXEC*/ DeeFS_RmDir             DeeFS_Utf8RmDir
#define /*DEE_A_EXEC*/ _DeeFS_Copy             _DeeFS_Utf8Copy
#define /*DEE_A_EXEC*/ _DeeFS_Move             _DeeFS_Utf8Move
#define /*DEE_A_EXEC*/ _DeeFS_Link             _DeeFS_Utf8Link
#define /*DEE_A_EXEC*/ DeeFS_Copy              DeeFS_Utf8Copy
#define /*DEE_A_EXEC*/ DeeFS_Move              DeeFS_Utf8Move
#define /*DEE_A_EXEC*/ DeeFS_Link              DeeFS_Utf8Link
#define /*DEE_A_EXEC*/ _DeeFS_CopyWithProgress _DeeFS_Utf8CopyWithProgress
#define /*DEE_A_EXEC*/ DeeFS_CopyWithProgress  DeeFS_Utf8CopyWithProgress
#define /*DEE_A_EXEC*/ _DeeFS_ListDir          _DeeFS_Utf8ListDir
#define /*DEE_A_EXEC*/ DeeFS_ListDir           DeeFS_Utf8ListDir
#define /*DEE_A_EXEC*/ _DeeFS_Query            _DeeFS_Utf8Query
#define /*DEE_A_EXEC*/ DeeFS_Query             DeeFS_Utf8Query
#ifdef DEE_PLATFORM_WINDOWS
#define /*DEE_A_EXEC*/ _DeeFS_Win32HardLink    _DeeFS_Utf8Win32HardLink
#define /*DEE_A_EXEC*/ DeeFS_Win32HardLink     DeeFS_Utf8Win32HardLink
#endif
#define /*DEE_A_EXEC*/ DeeFS_GetMod            DeeFS_Utf8GetMod
#define /*DEE_A_EXEC*/ _DeeFS_GetMod           _DeeFS_Utf8GetMod
#define /*DEE_A_EXEC*/ _DeeFS_Chmod            _DeeFS_Utf8Chmod
#define /*DEE_A_EXEC*/ _DeeFS_ChmodString      _DeeFS_Utf8ChmodString
#define /*DEE_A_EXEC*/ DeeFS_Chmod             DeeFS_Utf8Chmod
#define /*DEE_A_EXEC*/ DeeFS_ChmodString       DeeFS_Utf8ChmodString
#define /*DEE_A_EXEC*/ _DeeFS_Chown            _DeeFS_Utf8Chown
#define /*DEE_A_EXEC*/ DeeFS_Chown             DeeFS_Utf8Chown
#ifdef DEE_PLATFORM_WINDOWS
#define /*DEE_A_EXEC*/ DeeFS_Win32NameToSID    DeeFS_Utf8Win32NameToSID
#endif
#ifdef DEE_PLATFORM_UNIX
#define /*DEE_A_EXEC*/ DeeFS_PosixUserToUid    DeeFS_Utf8PosixUserToUid
#define /*DEE_A_EXEC*/ DeeFS_PosixGroupToGid   DeeFS_Utf8PosixGroupToGid
#endif
#define /*DEE_A_EXEC*/ DeeFS_GetOwn            DeeFS_Utf8GetOwn
#define /*DEE_A_EXEC*/ _DeeFS_GetOwn           _DeeFS_Utf8GetOwn
#define /*DEE_A_EXEC*/ _DeeFS_ReadLink         _DeeFS_Utf8ReadLink
#define /*DEE_A_EXEC*/ DeeFS_ReadLink          DeeFS_Utf8ReadLink
#endif


#endif /* !GUARD_DEEMON_FS_API_H */
