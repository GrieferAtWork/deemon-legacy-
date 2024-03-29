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
#include <deemon/string.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/optional/fs_api.fsapimode.h>
#include <deemon/optional/fs_api.fileproperty.h>
#include <deemon/optional/fs_api.modechange.h>
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
#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
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
#ifdef DEE_PRIVATE_DECL_DEE_UID_T
DEE_PRIVATE_DECL_DEE_UID_T
#undef DEE_PRIVATE_DECL_DEE_UID_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_GID_T
DEE_PRIVATE_DECL_DEE_GID_T
#undef DEE_PRIVATE_DECL_DEE_GID_T
#endif


DEE_OBJECT_DEF(DeeFSDirObject);
DEE_OBJECT_DEF(DeeFSDirIteratorObject);
DEE_OBJECT_DEF(DeeFSQueryObject);
DEE_OBJECT_DEF(DeeFSQueryIteratorObject);
#ifdef DEE_PLATFORM_WINDOWS
DEE_OBJECT_DEF(DeeFSWin32SIDObject);
#endif

struct DeeListObject;
struct DeeTimeObject;

#define DEE_PRIVATE_CALL_UTF8WIDE_ARG0(T,err,uniarg,u8func,wfunc)\
 T result; DeeObject *u8arg;\
 if (DeeWideString_Check(uniarg)) return wfunc(uniarg);\
 if DEE_UNLIKELY((u8arg = DeeUtf8String_Cast(uniarg)) == NULL) return err;\
 result = u8func(u8arg);\
 Dee_DECREF(u8arg);\
 return result;
#define DEE_PRIVATE_CALL_UTF8WIDE_ARGN(T,err,uniarg,u8func,wfunc,...)\
 T result; DeeObject *u8arg;\
 if (DeeWideString_Check(uniarg)) return wfunc(uniarg,__VA_ARGS__);\
 if DEE_UNLIKELY((u8arg = DeeUtf8String_Cast(uniarg)) == NULL) return err;\
 result = u8func(u8arg,__VA_ARGS__);\
 Dee_DECREF(u8arg);\
 return result;


typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETCWD)(void);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETCWD)(void);
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHDIR)(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHDIR)(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHDIROBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHDIROBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Current working directory get/set
// NOTE: 'path' may be relative, allowing you to navigate from the current cwd
#define DeeFS_Utf8GetCwd            DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETCWD,0)
#define DeeFS_WideGetCwd            DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETCWD,1)
#define DeeFS_Utf8Chdir             DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHDIR,2)
#define DeeFS_WideChdir             DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHDIR,3)
#define DeeFS_Utf8ChdirObject       DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHDIROBJECT,4)
#define DeeFS_WideChdirObject       DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHDIROBJECT,5)
#define _DeeFS_Utf8Chdir            DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHDIR,6)
#define _DeeFS_WideChdir            DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHDIR,7)
#define _DeeFS_Utf8ChdirObject      DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHDIROBJECT,8)
#define _DeeFS_WideChdirObject      DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHDIROBJECT,9)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_ChdirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,DeeFS_Utf8ChdirObject,DeeFS_WideChdirObject)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
_DeeFS_ChdirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,_DeeFS_Utf8ChdirObject,_DeeFS_WideChdirObject)
}
#define DeeFS_GetCwd DeeFS_Utf8GetCwd

#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFS_Utf8ChDir    DEE_DEPRECATED_MACRO(DeeFS_Utf8Chdir)
#define DeeFS_WideChDir    DEE_DEPRECATED_MACRO(DeeFS_WideChdir)
#define DeeFS_ChDirObject  DEE_DEPRECATED_MACRO(DeeFS_ChdirObject)
#define _DeeFS_Utf8ChDir   DEE_DEPRECATED_MACRO(_DeeFS_Utf8Chdir)
#define _DeeFS_WideChDir   DEE_DEPRECATED_MACRO(_DeeFS_WideChdir)
#define _DeeFS_ChDirObject DEE_DEPRECATED_MACRO(_DeeFS_ChdirObject)
#endif


typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETHOME)(void);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETHOME)(void);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETUSERHOME)(DEE_A_IN_Z Dee_Utf8Char const *username);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETUSERHOME)(DEE_A_IN_Z Dee_WideChar const *username);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETUSERHOMEOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *username);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETUSERHOMEOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *username);

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
#define DeeFS_Utf8GetHome           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETHOME,          10)
#define DeeFS_WideGetHome           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETHOME,          11)
#define DeeFS_Utf8GetUserHome       DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETUSERHOME,      12)
#define DeeFS_WideGetUserHome       DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETUSERHOME,      13)
#define DeeFS_Utf8GetUserHomeObject DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETUSERHOMEOBJECT,14)
#define DeeFS_WideGetUserHomeObject DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETUSERHOMEOBJECT,15)

#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFS_Utf8GetHomeUser       DEE_DEPRECATED_MACRO(DeeFS_Utf8GetUserHome)
#define DeeFS_WideGetHomeUser       DEE_DEPRECATED_MACRO(DeeFS_WideGetUserHome)
#define DeeFS_GetHomeUser           DEE_DEPRECATED_MACRO(DeeFS_GetUserHome)
#define DeeFS_GetHomeUserObject     DEE_DEPRECATED_MACRO(DeeFS_GetUserHomeObject)
#endif

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
DeeFS_GetUserHomeObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *username) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(DeeObject *,NULL,username,DeeFS_Utf8GetUserHomeObject,DeeFS_WideGetUserHomeObject)
}
#define DeeFS_GetHome DeeFS_Utf8GetHome

typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETTMP)(void);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETTMP)(void);

//////////////////////////////////////////////////////////////////////////
// Returns the temp path, as indicated by the OS
#define DeeFS_Utf8GetTmp DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETTMP,16)
#define DeeFS_WideGetTmp DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETTMP,17)
#define DeeFS_GetTmp DeeFS_Utf8GetTmp
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Utf8GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_WideGetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetTmpName(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z Dee_Utf8Char const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetTmpName(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z Dee_WideChar const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFS_GetTmpNameObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) _DeeFS_GetTmpNameObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OBJECT(DeeAnyStringObject) const *prefix) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_PLATFORM_WINDOWS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetDllDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetDllDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSystemDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSystemDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSystemWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSystemWindowsDirectory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8Win32GetSysWow64Directory(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideWin32GetSysWow64Directory(void);
#define DeeFS_Win32GetDllDirectory           DeeFS_Utf8Win32GetDllDirectory
#define DeeFS_Win32GetSystemDirectory        DeeFS_Utf8Win32GetSystemDirectory
#define DeeFS_Win32GetWindowsDirectory       DeeFS_Utf8Win32GetWindowsDirectory
#define DeeFS_Win32GetSystemWindowsDirectory DeeFS_Utf8Win32GetSystemWindowsDirectory
#define DeeFS_Win32GetSysWow64Directory      DeeFS_Utf8Win32GetSysWow64Directory
#endif


typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_UTF8ENUMENV)(void);
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_WIDEENUMENV)(void);
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETENV)(DEE_A_IN_Z Dee_Utf8Char const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETENV)(DEE_A_IN_Z Dee_WideChar const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8GETENVOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEGETENVOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_UTF8HASENV)(DEE_A_IN_Z Dee_Utf8Char const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_WIDEHASENV)(DEE_A_IN_Z Dee_WideChar const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_UTF8HASENVOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_WIDEHASENVOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8DELENV)(DEE_A_IN_Z Dee_Utf8Char const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEDELENV)(DEE_A_IN_Z Dee_WideChar const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8DELENVOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEDELENVOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8SETENV)(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDESETENV)(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8SETENVOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDESETENVOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8TRYGETENV)(DEE_A_IN_Z Dee_Utf8Char const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDETRYGETENV)(DEE_A_IN_Z Dee_WideChar const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8TRYGETENVOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDETRYGETENVOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// List/Get/Del/Set environment vars
// NOTE: 'DeeFS_ListEnv' returns a list of 2-elem string tuples,
//        where the first is the env name and the second its value.
#define DeeFS_WideEnumEnv DeeFS_GETFUNCTION_v102(PDEEFS_UTF8ENUMENV,18)
#define DeeFS_Utf8EnumEnv DeeFS_GETFUNCTION_v102(PDEEFS_WIDEENUMENV,19)
#define DeeFS_EnumEnv   DeeFS_Utf8EnumEnv
#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFS_WideListEnv DEE_DEPRECATED_MACRO(DeeFS_WideEnumEnv)
#define DeeFS_Utf8ListEnv DEE_DEPRECATED_MACRO(DeeFS_Utf8EnumEnv)
#define DeeFS_ListEnv     DEE_DEPRECATED_MACRO(DeeFS_EnumEnv)
#endif

#define DeeFS_Utf8GetEnv          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETENV,20)
#define DeeFS_WideGetEnv          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETENV,21)
#define DeeFS_Utf8GetEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETENVOBJECT,22)
#define DeeFS_WideGetEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETENVOBJECT,23)
#define DeeFS_Utf8HasEnv          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASENV,24)
#define DeeFS_WideHasEnv          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASENV,25)
#define DeeFS_Utf8HasEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASENVOBJECT,26)
#define DeeFS_WideHasEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASENVOBJECT,27)
#define DeeFS_Utf8DelEnv          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8DELENV,28)
#define DeeFS_WideDelEnv          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEDELENV,29)
#define DeeFS_Utf8DelEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8DELENVOBJECT,30)
#define DeeFS_WideDelEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEDELENVOBJECT,31)
#define DeeFS_Utf8SetEnv          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETENV,32)
#define DeeFS_WideSetEnv          DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETENV,33)
#define DeeFS_Utf8SetEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETENVOBJECT,34)
#define DeeFS_WideSetEnvObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETENVOBJECT,35)
#define DeeFS_Utf8TryGetEnv       DeeFS_GETFUNCTION_v102(PDEEFS_UTF8TRYGETENV,36)
#define DeeFS_WideTryGetEnv       DeeFS_GETFUNCTION_v102(PDEEFS_WIDETRYGETENV,37)
#define DeeFS_Utf8TryGetEnvObject DeeFS_GETFUNCTION_v102(PDEEFS_UTF8TRYGETENVOBJECT,38)
#define DeeFS_WideTryGetEnvObject DeeFS_GETFUNCTION_v102(PDEEFS_WIDETRYGETENVOBJECT,39)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int)
DeeFS_HasEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *envname) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,envname,DeeFS_Utf8HasEnvObject,DeeFS_WideHasEnvObject)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
DeeFS_GetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *envname) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(DeeObject *,NULL,envname,DeeFS_Utf8GetEnvObject,DeeFS_WideGetEnvObject)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
DeeFS_TryGetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *envname) {
 DeeObject *u8envname,*result;
 if (DeeWideString_Check(envname)) return DeeFS_WideTryGetEnvObject(envname);
 if DEE_UNLIKELY((u8envname = DeeUtf8String_Cast(envname)) == NULL) { DeeError_HandledOne(); return NULL; }
 result = DeeFS_Utf8TryGetEnvObject(u8envname);
 Dee_DECREF(u8envname);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_DelEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *envname) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,envname,DeeFS_Utf8DelEnvObject,DeeFS_WideDelEnvObject)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_SetEnvObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *envname, DEE_A_IN_OBJECT(DeeAnyStringObject) const *envvalue) {
 DeeObject *casted_envname,*casted_envvalue; int result;
 if (DeeWideString_Check(envname)) {
  if DEE_UNLIKELY((casted_envvalue = DeeWideString_Cast(envvalue)) == NULL) return -1;
  result = DeeFS_WideSetEnvObject(envname,casted_envvalue);
  Dee_DECREF(casted_envvalue);
  return result;
 }
 if DEE_UNLIKELY((casted_envname = DeeUtf8String_Cast(envname)) == NULL) return -1;
 if DEE_UNLIKELY((casted_envvalue = DeeUtf8String_Cast(envvalue)) == NULL) { result = -1; goto end_0; }
 result = DeeFS_Utf8SetEnvObject(casted_envname,casted_envvalue);
 Dee_DECREF(casted_envvalue);
end_0: Dee_DECREF(casted_envname);
 return result;
}

// TODO: Should we just remove this one?
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
 DeeFS_SetListEnv(DEE_A_IN_OBJECT(struct DeeListObject) const *v) DEE_ATTRIBUTE_NONNULL((1));

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
#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeFS_Utf8PathExpandObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeFS_WidePathExpandObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
#endif

typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETTIMES)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETTIMES)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETTIMESOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETTIMESOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8SETTIMES)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDESETTIMES)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8SETTIMESOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDESETTIMESOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the <last access>, <creation> and <last modification> time of a file/directory 'path'
#define DeeFS_Utf8GetTimes           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETTIMES,40)
#define DeeFS_WideGetTimes           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETTIMES,41)
#define DeeFS_Utf8GetTimesObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETTIMESOBJECT,42)
#define DeeFS_WideGetTimesObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETTIMESOBJECT,43)
#define _DeeFS_Utf8GetTimes          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETTIMES,44)
#define _DeeFS_WideGetTimes          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETTIMES,45)
#define _DeeFS_Utf8GetTimesObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETTIMESOBJECT,46)
#define _DeeFS_WideGetTimesObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETTIMESOBJECT,47)
#define DeeFS_Utf8SetTimes           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETTIMES,48)
#define DeeFS_WideSetTimes           DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETTIMES,49)
#define DeeFS_Utf8SetTimesObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETTIMESOBJECT,50)
#define DeeFS_WideSetTimesObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETTIMESOBJECT,51)
#define _DeeFS_Utf8SetTimes          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETTIMES,52)
#define _DeeFS_WideSetTimes          DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETTIMES,53)
#define _DeeFS_Utf8SetTimesObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8SETTIMESOBJECT,54)
#define _DeeFS_WideSetTimesObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDESETTIMESOBJECT,55)

DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_GetTimesObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8GetTimesObject,_DeeFS_WideGetTimesObject,atime,ctime,mtime)
}
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_GetTimesObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8GetTimesObject,DeeFS_WideGetTimesObject,atime,ctime,mtime)
}
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_SetTimesObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime,
 DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8SetTimesObject,_DeeFS_WideSetTimesObject,atime,ctime,mtime)
}
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_SetTimesObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime,
 DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8SetTimesObject,DeeFS_WideSetTimesObject,atime,ctime,mtime)
}


typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_UTF8HASPROPERTY)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_WIDEHASPROPERTY)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_UTF8HASPROPERTYOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int (DEE_CALL *PDEEFS_WIDEHASPROPERTYOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns '1' if certain conditions apply for a given 'path'; '0' otherwise

#define DeeFS_Utf8HasProperty           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASPROPERTY,56)
#define DeeFS_WideHasProperty           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASPROPERTY,57)
#define DeeFS_Utf8HasPropertyObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASPROPERTYOBJECT,58)
#define DeeFS_WideHasPropertyObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASPROPERTYOBJECT,59)
#define _DeeFS_Utf8HasProperty          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASPROPERTY,60)
#define _DeeFS_WideHasProperty          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASPROPERTY,61)
#define _DeeFS_Utf8HasPropertyObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8HASPROPERTYOBJECT,62)
#define _DeeFS_WideHasPropertyObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEHASPROPERTYOBJECT,63)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int)
DeeFS_HasPropertyObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8HasPropertyObject,DeeFS_WideHasPropertyObject,prop)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int)
_DeeFS_HasPropertyObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8HasPropertyObject,_DeeFS_WideHasPropertyObject,prop)
}

//////////////////////////////////////////////////////////////////////////
// File properties
#define _DeeFS_Utf8IsFile(path)           _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define _DeeFS_WideIsFile(path)           _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define _DeeFS_IsFile(path)                   _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define _DeeFS_IsFileObject(path)       _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISFILE)
#define  DeeFS_Utf8IsFile(path)            DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define  DeeFS_WideIsFile(path)            DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define  DeeFS_IsFile(path)                    DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define  DeeFS_IsFileObject(path)        DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISFILE)
#define _DeeFS_Utf8IsDir(path)            _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define _DeeFS_WideIsDir(path)            _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define _DeeFS_IsDir(path)                    _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define _DeeFS_IsDirObject(path)        _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISDIR)
#define  DeeFS_Utf8IsDir(path)             DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define  DeeFS_WideIsDir(path)             DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define  DeeFS_IsDir(path)                     DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define  DeeFS_IsDirObject(path)         DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISDIR)
#define _DeeFS_Utf8IsLink(path)           _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define _DeeFS_WideIsLink(path)           _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define _DeeFS_IsLink(path)                   _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define _DeeFS_IsLinkObject(path)       _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISLINK)
#define  DeeFS_Utf8IsLink(path)            DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define  DeeFS_WideIsLink(path)            DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define  DeeFS_IsLink(path)                    DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define  DeeFS_IsLinkObject(path)        DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISLINK)
#define _DeeFS_Utf8IsDrive(path)          _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define _DeeFS_WideIsDrive(path)          _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define _DeeFS_IsDrive(path)                  _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define _DeeFS_IsDriveObject(path)      _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISDRIVE)
#define  DeeFS_Utf8IsDrive(path)           DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define  DeeFS_WideIsDrive(path)           DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define  DeeFS_IsDrive(path)                   DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define  DeeFS_IsDriveObject(path)       DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISDRIVE)
#define _DeeFS_Utf8IsMount(path)          _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define _DeeFS_WideIsMount(path)          _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define _DeeFS_IsMount(path)                  _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define _DeeFS_IsMountObject(path)      _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISMOUNT)
#define  DeeFS_Utf8IsMount(path)           DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define  DeeFS_WideIsMount(path)           DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define  DeeFS_IsMount(path)                   DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define  DeeFS_IsMountObject(path)       DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISMOUNT)
#define _DeeFS_Utf8IsHidden(path)         _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define _DeeFS_WideIsHidden(path)         _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define _DeeFS_IsHidden(path)                 _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define _DeeFS_IsHiddenObject(path)     _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISHIDDEN)
#define  DeeFS_Utf8IsHidden(path)          DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define  DeeFS_WideIsHidden(path)          DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define  DeeFS_IsHidden(path)                  DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define  DeeFS_IsHiddenObject(path)      DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISHIDDEN)
#define _DeeFS_Utf8IsExecutable(path)     _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define _DeeFS_WideIsExecutable(path)     _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define _DeeFS_IsExecutable(path)             _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define _DeeFS_IsExecutableObject(path) _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define  DeeFS_Utf8IsExecutable(path)      DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define  DeeFS_WideIsExecutable(path)      DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define  DeeFS_IsExecutable(path)              DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define  DeeFS_IsExecutableObject(path)  DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define _DeeFS_Utf8IsCharDev(path)        _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define _DeeFS_WideIsCharDev(path)        _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define _DeeFS_IsCharDev(path)                _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define _DeeFS_IsCharDevObject(path)    _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISCHARDEV)
#define  DeeFS_Utf8IsCharDev(path)         DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define  DeeFS_WideIsCharDev(path)         DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define  DeeFS_IsCharDev(path)                 DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define  DeeFS_IsCharDevObject(path)     DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISCHARDEV)
#define _DeeFS_Utf8IsBlockDev(path)       _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define _DeeFS_WideIsBlockDev(path)       _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define _DeeFS_IsBlockDev(path)               _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define _DeeFS_IsBlockDevObject(path)   _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define  DeeFS_Utf8IsBlockDev(path)        DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define  DeeFS_WideIsBlockDev(path)        DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define  DeeFS_IsBlockDev(path)                DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define  DeeFS_IsBlockDevObject(path)    DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define _DeeFS_Utf8IsFiFo(path)           _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define _DeeFS_WideIsFiFo(path)           _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define _DeeFS_IsFiFo(path)                   _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define _DeeFS_IsFiFoObject(path)       _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISFIFO)
#define  DeeFS_Utf8IsFiFo(path)            DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define  DeeFS_WideIsFiFo(path)            DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define  DeeFS_IsFiFo(path)                    DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define  DeeFS_IsFiFoObject(path)        DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISFIFO)
#define _DeeFS_Utf8IsSocket(path)         _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define _DeeFS_WideIsSocket(path)         _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define _DeeFS_IsSocket(path)                 _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define _DeeFS_IsSocketObject(path)     _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISSOCKET)
#define  DeeFS_Utf8IsSocket(path)          DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define  DeeFS_WideIsSocket(path)          DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define  DeeFS_IsSocket(path)                  DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define  DeeFS_IsSocketObject(path)      DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISSOCKET)
#define _DeeFS_Utf8IsAbs(path)            _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISABS)
#define _DeeFS_WideIsAbs(path)            _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISABS)
#define _DeeFS_IsAbs(path)                    _DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISABS)
#define _DeeFS_IsAbsObject(path)        _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISABS)
#define  DeeFS_Utf8IsAbs(path)             DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISABS)
#define  DeeFS_WideIsAbs(path)             DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_ISABS)
#define  DeeFS_IsAbs(path)                     DeeFS_HasProperty(path,DEE_FILEPROPERTY_ISABS)
#define  DeeFS_IsAbsObject(path)         DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_ISABS)
#define _DeeFS_Utf8Exists(path)           _DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define _DeeFS_WideExists(path)           _DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define _DeeFS_Exists(path)                   _DeeFS_HasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define _DeeFS_ExistsObject(path)       _DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_EXISTS)
#define  DeeFS_Utf8Exists(path)            DeeFS_Utf8HasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define  DeeFS_WideExists(path)            DeeFS_WideHasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define  DeeFS_Exists(path)                    DeeFS_HasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define  DeeFS_ExistsObject(path)        DeeFS_HasPropertyObject(path,DEE_FILEPROPERTY_EXISTS)


typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8PATHFUN)(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEPATHFUN)(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8PATHFUNOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEPATHFUNOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8MKDIR)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEMKDIR)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8MKDIROBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEMKDIROBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Remove anything / a file
#define DeeFS_Utf8Remove           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,64)
#define DeeFS_WideRemove           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,65)
#define DeeFS_Utf8RemoveObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,66)
#define DeeFS_WideRemoveObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,67)
#define _DeeFS_Utf8Remove          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,68)
#define _DeeFS_WideRemove          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,69)
#define _DeeFS_Utf8RemoveObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,70)
#define _DeeFS_WideRemoveObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,71)
#define DeeFS_Utf8Unlink           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,72)
#define DeeFS_WideUnlink           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,73)
#define DeeFS_Utf8UnlinkObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,74)
#define DeeFS_WideUnlinkObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,75)
#define _DeeFS_Utf8Unlink          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,76)
#define _DeeFS_WideUnlink          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,77)
#define _DeeFS_Utf8UnlinkObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,78)
#define _DeeFS_WideUnlinkObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,79)


//////////////////////////////////////////////////////////////////////////
// Create / Remove a directory
#define DeeFS_Utf8RmDir           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,80)
#define DeeFS_WideRmDir           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,81)
#define DeeFS_Utf8RmDirObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,82)
#define DeeFS_WideRmDirObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,83)
#define _DeeFS_Utf8RmDir          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUN,84)
#define _DeeFS_WideRmDir          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUN,85)
#define _DeeFS_Utf8RmDirObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8PATHFUNOBJECT,86)
#define _DeeFS_WideRmDirObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEPATHFUNOBJECT,87)
#define DeeFS_Utf8MkDir           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8MKDIR,88)
#define DeeFS_WideMkDir           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEMKDIR,89)
#define DeeFS_Utf8MkDirObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8MKDIROBJECT,90)
#define DeeFS_WideMkDirObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEMKDIROBJECT,91)
#define _DeeFS_Utf8MkDir          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8MKDIR,92)
#define _DeeFS_WideMkDir          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEMKDIR,93)
#define _DeeFS_Utf8MkDirObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8MKDIROBJECT,94)
#define _DeeFS_WideMkDirObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEMKDIROBJECT,95)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_RemoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,DeeFS_Utf8RemoveObject,DeeFS_WideRemoveObject) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_UnlinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,DeeFS_Utf8UnlinkObject,DeeFS_WideUnlinkObject) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_MkDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) { DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8MkDirObject,DeeFS_WideMkDirObject,mode) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_RmDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,DeeFS_Utf8RmDirObject,DeeFS_WideRmDirObject) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_RemoveObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,_DeeFS_Utf8RemoveObject,_DeeFS_WideRemoveObject) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_UnlinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,_DeeFS_Utf8UnlinkObject,_DeeFS_WideUnlinkObject) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_MkDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) { DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8MkDirObject,_DeeFS_WideMkDirObject,mode) }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_RmDirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) { DEE_PRIVATE_CALL_UTF8WIDE_ARG0(int,-1,path,_DeeFS_Utf8RmDirObject,_DeeFS_WideRmDirObject) }


#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFS_Utf8RmFile    DEE_DEPRECATED_MACRO(DeeFS_Utf8Unlink)
#define DeeFS_WideRmFile    DEE_DEPRECATED_MACRO(DeeFS_WideUnlink)
#define DeeFS_RmFileObject  DEE_DEPRECATED_MACRO(DeeFS_UnlinkObject)
#define _DeeFS_Utf8RmFile   DEE_DEPRECATED_MACRO(_DeeFS_Utf8Unlink)
#define _DeeFS_WideRmFile   DEE_DEPRECATED_MACRO(_DeeFS_WideUnlink)
#define _DeeFS_RmFileObject DEE_DEPRECATED_MACRO(_DeeFS_UnlinkObject)
#endif




typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETMOD)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETMOD)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETMODOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETMODOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Get read/write/execute permissions for a given file.
// NOTE: On windows, this only checks the read-only flag of a given file.
//       To check if a file is executable, use 'DeeFS_IsExecutable'
#define DeeFS_Utf8GetMod           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETMOD,96)
#define DeeFS_WideGetMod           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETMOD,97)
#define DeeFS_Utf8GetModObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETMODOBJECT,98)
#define DeeFS_WideGetModObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETMODOBJECT,99)
#define _DeeFS_Utf8GetMod          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETMOD,100)
#define _DeeFS_WideGetMod          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETMOD,101)
#define _DeeFS_Utf8GetModObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETMODOBJECT,102)
#define _DeeFS_WideGetModObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETMODOBJECT,103)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_GetModObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8GetModObject,DeeFS_WideGetModObject,mode)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
_DeeFS_GetModObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_mode_t *mode) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8GetModObject,_DeeFS_WideGetModObject,mode)
}


typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHMOD)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHMOD)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHMODOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHMODOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Set read/write/execute permissions for a given file.
// NOTE: On windows, this only sets the read-only flag of a given file.
#define DeeFS_Utf8Chmod           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHMOD,104)
#define DeeFS_WideChmod           DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHMOD,105)
#define DeeFS_Utf8ChmodObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHMODOBJECT,106)
#define DeeFS_WideChmodObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHMODOBJECT,107)
#define _DeeFS_Utf8Chmod          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHMOD,108)
#define _DeeFS_WideChmod          DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHMOD,109)
#define _DeeFS_Utf8ChmodObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHMODOBJECT,110)
#define _DeeFS_WideChmodObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHMODOBJECT,111)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_ChmodObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8ChmodObject,DeeFS_WideChmodObject,mode)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
_DeeFS_ChmodObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_mode_t mode) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8ChmodObject,_DeeFS_WideChmodObject,mode)
}

//////////////////////////////////////////////////////////////////////////
// Extended mode changing functions
// >> All provided mode change operations will be
//    performed starting at 'mcv[0]' and ending with 'mcv[mcc-1]'.
// >> The resulting permission bits will then be assigned to the file.
// NOTE: At least one mode change must be provided in order to elude undefined behavior
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodEx(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodEx(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodExObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodExObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8ChmodEx(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChmodEx(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8ChmodExObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChmodExObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) DEE_ATTRIBUTE_NONNULL((1));

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodExObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8ChmodExObject,_DeeFS_WideChmodExObject,mcc,mcv)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodExObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8ChmodExObject,DeeFS_WideChmodExObject,mcc,mcv)
}

DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Utf8ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return _DeeFS_Utf8ChmodEx(path,1,&change); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_WideChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return _DeeFS_WideChmodEx(path,1,&change); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Utf8ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return DeeFS_Utf8ChmodEx(path,1,&change); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_WideChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return DeeFS_WideChmodEx(path,1,&change); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodStringObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return _DeeFS_ChmodExObject(path,1,&change); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodStringObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN_Z char const *mode) { Dee_modechange_t change; if (DeeFS_ParseModeChange(mode,&change) != 0) return -1; return DeeFS_ChmodExObject(path,1,&change); }



typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETOWN)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETOWN)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8GETOWNOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDEGETOWNOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Get owner/group for a given file.
#define DeeFS_Utf8GetOwn           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETOWN,112)
#define DeeFS_WideGetOwn           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETOWN,113)
#define DeeFS_Utf8GetOwnObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETOWNOBJECT,114)
#define DeeFS_WideGetOwnObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETOWNOBJECT,115)
#define _DeeFS_Utf8GetOwn          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETOWN,116)
#define _DeeFS_WideGetOwn          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETOWN,117)
#define _DeeFS_Utf8GetOwnObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8GETOWNOBJECT,118)
#define _DeeFS_WideGetOwnObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEGETOWNOBJECT,119)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_GetOwnObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8GetOwnObject,DeeFS_WideGetOwnObject,owner,group)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
_DeeFS_GetOwnObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8GetOwnObject,_DeeFS_WideGetOwnObject,owner,group)
}

typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHOWN)(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHOWN)(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8CHOWNOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECHOWNOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Get owner/group for a given file.
#define DeeFS_Utf8Chown           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHOWN,120)
#define DeeFS_WideChown           DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHOWN,121)
#define DeeFS_Utf8ChownObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHOWNOBJECT,122)
#define DeeFS_WideChownObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHOWNOBJECT,123)
#define _DeeFS_Utf8Chown          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHOWN,124)
#define _DeeFS_WideChown          DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHOWN,125)
#define _DeeFS_Utf8ChownObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8CHOWNOBJECT,126)
#define _DeeFS_WideChownObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDECHOWNOBJECT,127)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
DeeFS_ChownObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,DeeFS_Utf8ChownObject,DeeFS_WideChownObject,owner,group)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
_DeeFS_ChownObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARGN(int,-1,path,_DeeFS_Utf8ChownObject,_DeeFS_WideChownObject,owner,group)
}



#ifdef DEE_PLATFORM_WINDOWS
DEE_OBJECT_DEF(DeeFSWin32SIDObject);
DEE_DATA_DECL(DeeTypeObject) DeeFSWin32SID_Type;
#define DeeFSWin32SID_GetUid                DeeFSWin32SID_GetID
#define DeeFSWin32SID_GetGid                DeeFSWin32SID_GetID
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeFSWin32SID_GetID(DEE_A_IN_OBJECT(DeeFSWin32SIDObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *) DeeFSWin32SID_NewFromSID(/*PSID*/Dee_uintptr_t sid);
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


typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8COPY)(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECOPY)(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8COPYOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDECOPYOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8LINK)(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDELINK)(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_UTF8LINKOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) DEE_ATTRIBUTE_NONNULL((1,2));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDEEFS_WIDELINKOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) DEE_ATTRIBUTE_NONNULL((1,2));



//////////////////////////////////////////////////////////////////////////
// Copy/Move/Link a file
#define DeeFS_Utf8Copy           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPY,128)
#define DeeFS_WideCopy           DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPY,129)
#define DeeFS_Utf8CopyObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPYOBJECT,130)
#define DeeFS_WideCopyObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPYOBJECT,131)
#define _DeeFS_Utf8Copy          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPY,132)
#define _DeeFS_WideCopy          DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPY,133)
#define _DeeFS_Utf8CopyObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPYOBJECT,134)
#define _DeeFS_WideCopyObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPYOBJECT,135)
#define DeeFS_Utf8Move           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPY,136)
#define DeeFS_WideMove           DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPY,137)
#define DeeFS_Utf8MoveObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPYOBJECT,138)
#define DeeFS_WideMoveObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPYOBJECT,139)
#define _DeeFS_Utf8Move          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPY,140)
#define _DeeFS_WideMove          DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPY,141)
#define _DeeFS_Utf8MoveObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8COPYOBJECT,142)
#define _DeeFS_WideMoveObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDECOPYOBJECT,143)
#define DeeFS_Utf8Link           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8LINK,144)
#define DeeFS_WideLink           DeeFS_GETFUNCTION_v102(PDEEFS_WIDELINK,145)
#define DeeFS_Utf8LinkObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8LINKOBJECT,146)
#define DeeFS_WideLinkObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDELINKOBJECT,147)
#define _DeeFS_Utf8Link          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8LINK,148)
#define _DeeFS_WideLink          DeeFS_GETFUNCTION_v102(PDEEFS_WIDELINK,149)
#define _DeeFS_Utf8LinkObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8LINKOBJECT,150)
#define _DeeFS_WideLinkObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDELINKOBJECT,151)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_CopyObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 DeeObject *casted_src,*casted_dst; int result;
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY((casted_dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideCopyObject(src,casted_dst);
  Dee_DECREF(casted_dst);
  return result;
 }
 if DEE_UNLIKELY((casted_src = DeeUtf8String_Cast(src)) == NULL) return -1;
 if DEE_UNLIKELY((casted_dst = DeeUtf8String_Cast(dst)) == NULL) { result = -1; goto end_0; }
 result = _DeeFS_Utf8CopyObject(casted_src,casted_dst);
 Dee_DECREF(casted_dst);
end_0: Dee_DECREF(casted_src);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_MoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 DeeObject *casted_src,*casted_dst; int result;
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY((casted_dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = _DeeFS_WideMoveObject(src,casted_dst);
  Dee_DECREF(casted_dst);
  return result;
 }
 if DEE_UNLIKELY((casted_src = DeeUtf8String_Cast(src)) == NULL) return -1;
 if DEE_UNLIKELY((casted_dst = DeeUtf8String_Cast(dst)) == NULL) { result = -1; goto end_0; }
 result = _DeeFS_Utf8MoveObject(casted_src,casted_dst);
 Dee_DECREF(casted_dst);
end_0: Dee_DECREF(casted_src);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFS_LinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *link_name, DEE_A_IN_OBJECT(DeeAnyStringObject) const *target_name) {
 DeeObject *casted_link_name,*casted_target_name; int result;
 if (DeeWideString_Check(link_name)) {
  if DEE_UNLIKELY((casted_target_name = DeeWideString_Cast(target_name)) == NULL) return -1;
  result = _DeeFS_WideLinkObject(link_name,casted_target_name);
  Dee_DECREF(casted_target_name);
  return result;
 }
 if DEE_UNLIKELY((casted_link_name = DeeUtf8String_Cast(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((casted_target_name = DeeUtf8String_Cast(target_name)) == NULL) { result = -1; goto end_0; }
 result = _DeeFS_Utf8LinkObject(casted_link_name,casted_target_name);
 Dee_DECREF(casted_target_name);
end_0: Dee_DECREF(casted_link_name);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_CopyObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 DeeObject *casted_src,*casted_dst; int result;
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY((casted_dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = DeeFS_WideCopyObject(src,casted_dst);
  Dee_DECREF(casted_dst);
  return result;
 }
 if DEE_UNLIKELY((casted_src = DeeUtf8String_Cast(src)) == NULL) return -1;
 if DEE_UNLIKELY((casted_dst = DeeUtf8String_Cast(dst)) == NULL) { result = -1; goto end_0; }
 result = DeeFS_Utf8CopyObject(casted_src,casted_dst);
 Dee_DECREF(casted_dst);
end_0: Dee_DECREF(casted_src);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_MoveObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *src, DEE_A_IN_OBJECT(DeeAnyStringObject) const *dst) {
 DeeObject *casted_src,*casted_dst; int result;
 if (DeeWideString_Check(src)) {
  if DEE_UNLIKELY((casted_dst = DeeWideString_Cast(dst)) == NULL) return -1;
  result = DeeFS_WideMoveObject(src,casted_dst);
  Dee_DECREF(casted_dst);
  return result;
 }
 if DEE_UNLIKELY((casted_src = DeeUtf8String_Cast(src)) == NULL) return -1;
 if DEE_UNLIKELY((casted_dst = DeeUtf8String_Cast(dst)) == NULL) { result = -1; goto end_0; }
 result = DeeFS_Utf8MoveObject(casted_src,casted_dst);
 Dee_DECREF(casted_dst);
end_0: Dee_DECREF(casted_src);
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_LinkObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *link_name, DEE_A_IN_OBJECT(DeeAnyStringObject) const *target_name) {
 DeeObject *casted_link_name,*casted_target_name; int result;
 if (DeeWideString_Check(link_name)) {
  if DEE_UNLIKELY((casted_target_name = DeeWideString_Cast(target_name)) == NULL) return -1;
  result = DeeFS_WideLinkObject(link_name,casted_target_name);
  Dee_DECREF(casted_target_name);
  return result;
 }
 if DEE_UNLIKELY((casted_link_name = DeeUtf8String_Cast(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((casted_target_name = DeeUtf8String_Cast(target_name)) == NULL) { result = -1; goto end_0; }
 result = DeeFS_Utf8LinkObject(casted_link_name,casted_target_name);
 Dee_DECREF(casted_target_name);
end_0: Dee_DECREF(casted_link_name);
 return result;
}

typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_UTF8OPENDIR)(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_WIDEOPENDIR)(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_UTF8OPENDIROBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *PDEEFS_WIDEOPENDIROBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

#define DeeFS_Utf8Opendir           DeeFS_GETFUNCTION_v102(PDEEFS_UTF8OPENDIR,152)
#define DeeFS_WideOpendir           DeeFS_GETFUNCTION_v102(PDEEFS_WIDEOPENDIR,154)
#define DeeFS_Utf8OpendirObject     DeeFS_GETFUNCTION_v102(PDEEFS_UTF8OPENDIROBJECT,154)
#define DeeFS_WideOpendirObject     DeeFS_GETFUNCTION_v102(PDEEFS_WIDEOPENDIROBJECT,155)
#define _DeeFS_Utf8Opendir          DeeFS_GETFUNCTION_v102(PDEEFS_UTF8OPENDIR,156)
#define _DeeFS_WideOpendir          DeeFS_GETFUNCTION_v102(PDEEFS_WIDEOPENDIR,157)
#define _DeeFS_Utf8OpendirObject    DeeFS_GETFUNCTION_v102(PDEEFS_UTF8OPENDIROBJECT,158)
#define _DeeFS_WideOpendirObject    DeeFS_GETFUNCTION_v102(PDEEFS_WIDEOPENDIROBJECT,159)

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *)
DeeFS_OpendirObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(DeeObject *,NULL,path,DeeFS_Utf8OpendirObject,DeeFS_WideOpendirObject)
}




typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8READLINK)(DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEREADLINK)(DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *(DEE_CALL *PDEEFS_UTF8READLINKOBJECT)(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));
typedef DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *(DEE_CALL *PDEEFS_WIDEREADLINKOBJECT)(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Reads a given file system link
#define DeeFS_Utf8Readlink        DeeFS_GETFUNCTION_v102(PDEEFS_UTF8READLINK,168)
#define DeeFS_WideReadlink        DeeFS_GETFUNCTION_v102(PDEEFS_WIDEREADLINK,169)
#define DeeFS_Utf8ReadlinkObject  DeeFS_GETFUNCTION_v102(PDEEFS_UTF8READLINKOBJECT,170)
#define DeeFS_WideReadlinkObject  DeeFS_GETFUNCTION_v102(PDEEFS_WIDEREADLINKOBJECT,171)
#define _DeeFS_Utf8Readlink       DeeFS_GETFUNCTION_v102(PDEEFS_UTF8READLINK,172)
#define _DeeFS_WideReadlink       DeeFS_GETFUNCTION_v102(PDEEFS_WIDEREADLINK,173)
#define _DeeFS_Utf8ReadlinkObject DeeFS_GETFUNCTION_v102(PDEEFS_UTF8READLINKOBJECT,174)
#define _DeeFS_WideReadlinkObject DeeFS_GETFUNCTION_v102(PDEEFS_WIDEREADLINKOBJECT,175)

#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define _DeeFS_Utf8ReadLink   DEE_DEPRECATED_MACRO(_DeeFS_Utf8Readlink)
#define _DeeFS_WideReadLink   DEE_DEPRECATED_MACRO(_DeeFS_WideReadlink)
#define _DeeFS_ReadLink       DEE_DEPRECATED_MACRO(_DeeFS_Readlink)
#define _DeeFS_ReadLinkObject DEE_DEPRECATED_MACRO(_DeeFS_ReadlinkObject)
#define DeeFS_Utf8ReadLink    DEE_DEPRECATED_MACRO(DeeFS_Utf8Readlink)
#define DeeFS_WideReadLink    DEE_DEPRECATED_MACRO(DeeFS_WideReadlink)
#define DeeFS_ReadLink        DEE_DEPRECATED_MACRO(DeeFS_Readlink)
#define DeeFS_ReadLinkObject  DEE_DEPRECATED_MACRO(DeeFS_ReadlinkObject)
#endif

DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
_DeeFS_ReadlinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(DeeObject *,NULL,path,_DeeFS_Utf8ReadlinkObject,_DeeFS_WideReadlinkObject)
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
DeeFS_ReadlinkObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DEE_PRIVATE_CALL_UTF8WIDE_ARG0(DeeObject *,NULL,path,DeeFS_Utf8ReadlinkObject,DeeFS_WideReadlinkObject)
}



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



#define _DeeFS_Utf8ListDir   _DeeFS_Utf8Opendir
#define _DeeFS_WideListDir   _DeeFS_WideOpendir
#define _DeeFS_ListDirObject _DeeFS_OpendirObject
#define DeeFS_Utf8ListDir    DeeFS_Utf8Opendir
#define DeeFS_WideListDir    DeeFS_WideOpendir
#define DeeFS_ListDirObject  DeeFS_OpendirObject

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
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetHostname(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetHostname(void);
#define DeeFS_GetHostname DeeFS_Utf8GetHostname

// Returns the name of the current user
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Utf8GetUsername(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_WideGetUsername(void);
#define DeeFS_GetUsername DeeFS_Utf8GetUsername

#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFS_Utf8GetMachineName DEE_DEPRECATED_MACRO(DeeFS_Utf8GetHostname)
#define DeeFS_WideGetMachineName DEE_DEPRECATED_MACRO(DeeFS_WideGetHostname)
#define DeeFS_GetMachineName     DEE_DEPRECATED_MACRO(DeeFS_GetHostname)
#define DeeFS_Utf8GetUserName    DEE_DEPRECATED_MACRO(DeeFS_Utf8GetUsername)
#define DeeFS_WideGetUserName    DEE_DEPRECATED_MACRO(DeeFS_WideGetUsername)
#define DeeFS_GetUserName        DEE_DEPRECATED_MACRO(DeeFS_GetUsername)
#endif


#ifdef DEE_PLATFORM_WINDOWS
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeFS_Utf8Win32Drives(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeFS_WideWin32Drives(void);
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

#undef DEE_PRIVATE_CALL_UTF8WIDE_ARG0
#undef DEE_PRIVATE_CALL_UTF8WIDE_ARGN

DEE_DECL_END

#ifdef __cplusplus
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Chdir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChDir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideChdir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChDir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Chdir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChDir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideChdir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_GetUserHome(DEE_A_IN_Z Dee_Utf8Char const *user) { return DeeFS_Utf8GetUserHome(user); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_GetUserHome(DEE_A_IN_Z Dee_WideChar const *user) { return DeeFS_WideGetUserHome(user); }
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
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_HasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop) { return _DeeFS_Utf8HasProperty(path,prop); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeFS_HasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop) { return _DeeFS_WideHasProperty(path,prop); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_HasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop) { return DeeFS_Utf8HasProperty(path,prop); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_HasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop) { return DeeFS_WideHasProperty(path,prop); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Remove(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Remove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Remove(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideRemove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Unlink(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Unlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_Unlink(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideUnlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Remove(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Remove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Remove(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideRemove(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Unlink(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Unlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Unlink(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideUnlink(path); }
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
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_Opendir(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Opendir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) _DeeFS_Opendir(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideOpendir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_Opendir(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Opendir(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSDirObject) *) DeeFS_Opendir(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideOpendir(path); }
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
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodEx(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) { return _DeeFS_Utf8ChmodEx(path,mcc,mcv); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodEx(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) { return _DeeFS_WideChmodEx(path,mcc,mcv); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodString(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_Z char const *mode) { return _DeeFS_Utf8ChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeFS_ChmodString(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_Z char const *mode) { return _DeeFS_WideChmodString(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_Utf8Chmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_Chmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode) { return DeeFS_WideChmod(path,mode); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodEx(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) { return DeeFS_Utf8ChmodEx(path,mcc,mcv); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_EXCEPT(-1) int) DeeFS_ChmodEx(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) { return DeeFS_WideChmodEx(path,mcc,mcv); }
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
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeFS_Readlink(DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeFS_Utf8Readlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeFS_Readlink(DEE_A_IN_Z Dee_WideChar const *path) { return _DeeFS_WideReadlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeFS_Readlink(DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeFS_Utf8Readlink(path); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeFS_Readlink(DEE_A_IN_Z Dee_WideChar const *path) { return DeeFS_WideReadlink(path); }
#else
#define /*DEE_A_EXEC*/ _DeeFS_ChDir            _DeeFS_Utf8Chdir
#define /*DEE_A_EXEC*/ DeeFS_ChDir             DeeFS_Utf8Chdir
#define /*DEE_A_EXEC*/ DeeFS_GetUserHome       DeeFS_Utf8GetUserHome
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
#define /*DEE_A_EXEC*/ _DeeFS_GetTimes         _DeeFS_Utf8GetTimes
#define /*DEE_A_EXEC*/ _DeeFS_SetTimes         _DeeFS_Utf8SetTimes
#define /*DEE_A_EXEC*/ DeeFS_GetTimes          DeeFS_Utf8GetTimes
#define /*DEE_A_EXEC*/ DeeFS_SetTimes          DeeFS_Utf8SetTimes
#define /*DEE_A_EXEC*/ _DeeFS_HasProperty      _DeeFS_Utf8HasProperty
#define /*DEE_A_EXEC*/ DeeFS_HasProperty       DeeFS_Utf8HasProperty
#define /*DEE_A_EXEC*/ _DeeFS_Remove           _DeeFS_Utf8Remove
#define /*DEE_A_EXEC*/ _DeeFS_RmFile           _DeeFS_Utf8Unlink
#define /*DEE_A_EXEC*/ DeeFS_Remove            DeeFS_Utf8Remove
#define /*DEE_A_EXEC*/ DeeFS_RmFile            DeeFS_Utf8Unlink
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
#define /*DEE_A_EXEC*/ _DeeFS_Opendir          _DeeFS_Utf8Opendir
#define /*DEE_A_EXEC*/ DeeFS_Opendir           DeeFS_Utf8Opendir
#ifdef DEE_PLATFORM_WINDOWS
#define /*DEE_A_EXEC*/ _DeeFS_Win32HardLink    _DeeFS_Utf8Win32HardLink
#define /*DEE_A_EXEC*/ DeeFS_Win32HardLink     DeeFS_Utf8Win32HardLink
#endif
#define /*DEE_A_EXEC*/ DeeFS_GetMod            DeeFS_Utf8GetMod
#define /*DEE_A_EXEC*/ _DeeFS_GetMod           _DeeFS_Utf8GetMod
#define /*DEE_A_EXEC*/ _DeeFS_Chmod            _DeeFS_Utf8Chmod
#define /*DEE_A_EXEC*/ _DeeFS_ChmodEx          _DeeFS_Utf8ChmodEx
#define /*DEE_A_EXEC*/ _DeeFS_ChmodString      _DeeFS_Utf8ChmodString
#define /*DEE_A_EXEC*/ DeeFS_Chmod             DeeFS_Utf8Chmod
#define /*DEE_A_EXEC*/ DeeFS_ChmodEx           DeeFS_Utf8ChmodEx
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
#define /*DEE_A_EXEC*/ _DeeFS_Readlink         _DeeFS_Utf8Readlink
#define /*DEE_A_EXEC*/ DeeFS_Readlink          DeeFS_Utf8Readlink
#endif


#endif /* !GUARD_DEEMON_FS_API_H */
