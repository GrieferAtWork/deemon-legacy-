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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSFS_H
#define GUARD_DEEMON_SYS__WIN32_SYSFS_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/deemonrun.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/string.h>
#include <deemon/runtime/extern.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Aclapi.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include DEE_INCLUDE_MEMORY_API()

#ifdef DEE_PLATFORM_WINDOWS
#ifndef GetEnvironmentStringsA
#undef GetEnvironmentStrings
#define GetEnvironmentStringsA GetEnvironmentStrings
#endif
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetCwd(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetCwd(void);
#define DeeWin32SysFS_Utf8GetCwd DeeWin32Sys_Utf8GetCwd
#define DeeWin32SysFS_WideGetCwd DeeWin32Sys_WideGetCwd
#define DeeWin32SysFS_Utf8TryChdir(path) SetCurrentDirectoryA(path)
#define DeeWin32SysFS_WideTryChdir(path) SetCurrentDirectoryW(path)

#define DeeWin32SysFS_Utf8Chdir(path,...)\
do{\
 if DEE_UNLIKELY(!SetCurrentDirectoryA(path)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetCurrentDirectoryA(%q) : %K",path,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32SysFS_WideChdir(path,...)\
do{\
 if DEE_UNLIKELY(!SetCurrentDirectoryW(path)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetCurrentDirectoryW(%lq) : %K",path,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
#define DeeWin32Sys_Utf8TryHasEnv(envname) (GetEnvironmentVariableA(envname,NULL,0)!=0)
#define DeeWin32Sys_WideTryHasEnv(envname) (GetEnvironmentVariableW(envname,NULL,0)!=0)
#define DeeWin32Sys_Utf8HasEnv(envname,result,...) \
do{\
 if (GetEnvironmentVariableA(envname,NULL,0)==0) {\
  DWORD error = GetLastError();\
  if (error == ERROR_ENVVAR_NOT_FOUND) *(result) = 0;\
  else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetEnvironmentVariableA(%q,NULL,0) : %K",envname,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 } else *(result) = 1;\
}while(0)
#define DeeWin32Sys_WideHasEnv(envname,result,...) \
do{\
 if (GetEnvironmentVariableW(envname,NULL,0)==0) {\
  DWORD error = GetLastError();\
  if (error == ERROR_ENVVAR_NOT_FOUND) *(result) = 0;\
  else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetEnvironmentVariableW(%lq,NULL,0) : %K",envname,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 } else *(result) = 1;\
}while(0)
#define DeeWin32Sys_Utf8TryDelEnv(envname) SetEnvironmentVariableA(envname,NULL)
#define DeeWin32Sys_WideTryDelEnv(envname) SetEnvironmentVariableW(envname,NULL)
#define DeeWin32Sys_Utf8DelEnv(envname,...) \
do{\
 if (!SetEnvironmentVariableA(envname,NULL)) {\
  DWORD error = GetLastError();\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetEnvironmentVariableA(%q,NULL) : %K",envname,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_WideDelEnv(envname,...) \
do{\
 if (!SetEnvironmentVariableW(envname,NULL)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetEnvironmentVariableW(%lq,NULL,0) : %K",envname,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_Utf8TrySetEnv(envname,newvalue) SetEnvironmentVariableA(envname,newvalue)
#define DeeWin32Sys_WideTrySetEnv(envname,newvalue) SetEnvironmentVariableW(envname,newvalue)
#define DeeWin32Sys_Utf8SetEnv(envname,newvalue,...) \
do{\
 if (!SetEnvironmentVariableA(envname,newvalue)) {\
  DWORD error = GetLastError();\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetEnvironmentVariableA(%q,%q) : %K",envname,newvalue,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_WideSetEnv(envname,newvalue,...) \
do{\
 if (!SetEnvironmentVariableW(envname,newvalue)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetEnvironmentVariableW(%lq,%lq,0) : %K",envname,newvalue,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32SysFS_Utf8TryGetEnv DeeWin32Sys_Utf8TryGetEnv
#define DeeWin32SysFS_WideTryGetEnv DeeWin32Sys_WideTryGetEnv
#define DeeWin32SysFS_Utf8GetEnv    DeeWin32Sys_Utf8GetEnv
#define DeeWin32SysFS_WideGetEnv    DeeWin32Sys_WideGetEnv
#define DeeWin32SysFS_Utf8TryHasEnv DeeWin32Sys_Utf8TryHasEnv
#define DeeWin32SysFS_WideTryHasEnv DeeWin32Sys_WideTryHasEnv
#define DeeWin32SysFS_Utf8HasEnv    DeeWin32Sys_Utf8HasEnv
#define DeeWin32SysFS_WideHasEnv    DeeWin32Sys_WideHasEnv
#define DeeWin32SysFS_Utf8TryDelEnv DeeWin32Sys_Utf8TryDelEnv
#define DeeWin32SysFS_WideTryDelEnv DeeWin32Sys_WideTryDelEnv
#define DeeWin32SysFS_Utf8DelEnv    DeeWin32Sys_Utf8DelEnv
#define DeeWin32SysFS_WideDelEnv    DeeWin32Sys_WideDelEnv
#define DeeWin32SysFS_Utf8TrySetEnv DeeWin32Sys_Utf8TrySetEnv
#define DeeWin32SysFS_WideTrySetEnv DeeWin32Sys_WideTrySetEnv
#define DeeWin32SysFS_Utf8SetEnv    DeeWin32Sys_Utf8SetEnv
#define DeeWin32SysFS_WideSetEnv    DeeWin32Sys_WideSetEnv

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetTmp(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetTmp(void);
#define DeeWin32SysFS_Utf8GetTmp DeeWin32Sys_Utf8GetTmp
#define DeeWin32SysFS_WideGetTmp DeeWin32Sys_WideGetTmp

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetDllDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetDllDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetSystemDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetSystemDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetWindowsDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetWindowsDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetSystemWindowsDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetSystemWindowsDirectory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetSysWow64Directory(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetSysWow64Directory(void);
#define DeeWin32SysFS_Utf8GetDllDirectory           DeeWin32Sys_Utf8GetDllDirectory
#define DeeWin32SysFS_WideGetDllDirectory           DeeWin32Sys_WideGetDllDirectory
#define DeeWin32SysFS_Utf8GetSystemDirectory        DeeWin32Sys_Utf8GetSystemDirectory
#define DeeWin32SysFS_WideGetSystemDirectory        DeeWin32Sys_WideGetSystemDirectory
#define DeeWin32SysFS_Utf8GetWindowsDirectory       DeeWin32Sys_Utf8GetWindowsDirectory
#define DeeWin32SysFS_WideGetWindowsDirectory       DeeWin32Sys_WideGetWindowsDirectory
#define DeeWin32SysFS_Utf8GetSystemWindowsDirectory DeeWin32Sys_Utf8GetSystemWindowsDirectory
#define DeeWin32SysFS_WideGetSystemWindowsDirectory DeeWin32Sys_WideGetSystemWindowsDirectory
#define DeeWin32SysFS_Utf8GetSysWow64Directory      DeeWin32Sys_Utf8GetSysWow64Directory
#define DeeWin32SysFS_WideGetSysWow64Directory      DeeWin32Sys_WideGetSysWow64Directory

#define DeeWin32Sys_Utf8EnumEnv(enum,...)\
do{\
 LPCH _env_vars,_env_line;\
 if DEE_UNLIKELY((_env_vars = GetEnvironmentStringsA()) == NULL) {\
  Dee_DECREF(result);\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetEnvironmentStringsA() : %K",\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 for (_env_line = _env_vars; *_env_line;) {\
  Dee_Utf8Char const *_env_name,*_env_value_begin,*_env_line_end;\
  _env_value_begin = _env_line;\
  /* Skip internal env vars (like per-drive cwd, and others) */\
  if (*_env_value_begin == '=') continue;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  /* No idea what this is about, but lets be safe */\
  if DEE_UNLIKELY(!*_env_value_begin) continue;\
  _env_line_end += _env_value_begin+Dee_Utf8StrLen(_env_line);\
  enum((Dee_size_t)(_env_value_begin-_env_line),_env_line,\
       (Dee_size_t)(_env_line_end-(_env_value_begin+1)),_env_value_begin+1,\
       {if DEE_UNLIKELY(!FreeEnvironmentStringsA(_env_vars)) SetLastError(0); {__VA_ARGS__;}});\
  _env_line = _env_line_end+1;\
 }\
 if DEE_UNLIKELY(!FreeEnvironmentStringsA(_env_vars)) SetLastError(0);\
}while(0)
#define DeeWin32Sys_WideEnumEnv(enum,...)\
do{\
 LPWCH _env_vars,_env_line;\
 if DEE_UNLIKELY((_env_vars = GetEnvironmentStringsW()) == NULL) {\
  Dee_DECREF(result);\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetEnvironmentStringsW() : %K",\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 for (_env_line = _env_vars; *_env_line;) {\
  Dee_WideChar const *_env_name,*_env_value_begin,*_env_line_end;\
  _env_value_begin = _env_line;\
  /* Skip internal env vars (like per-drive cwd, and others) */\
  if (*_env_value_begin == DEE_WIDECHAR_C('=')) continue;\
  while (*_env_value_begin && *_env_value_begin != DEE_WIDECHAR_C('=')) ++_env_value_begin;\
  /* No idea what this is about, but lets be safe */\
  if DEE_UNLIKELY(!*_env_value_begin) continue;\
  _env_line_end += _env_value_begin+Dee_WideStrLen(_env_line);\
  enum((Dee_size_t)(_env_value_begin-_env_line),_env_line,\
       (Dee_size_t)(_env_line_end-(_env_value_begin+1)),_env_value_begin+1,\
       {if DEE_UNLIKELY(!FreeEnvironmentStringsW(_env_vars)) SetLastError(0); {__VA_ARGS__;}});\
  _env_line = _env_line_end+1;\
 }\
 if DEE_UNLIKELY(!FreeEnvironmentStringsW(_env_vars)) SetLastError(0);\
}while(0)
#define DEE_WIN32_SYS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DEE_WIN32_SYS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED

#define DeeWin32SysFS_Utf8EnumEnv DeeWin32Sys_Utf8EnumEnv
#define DeeWin32SysFS_WideEnumEnv DeeWin32Sys_WideEnumEnv
#define DEE_WIN32_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DEE_WIN32_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetTokenUserHome(DEE_A_IN HANDLE hToken);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetTokenUserHome(DEE_A_IN HANDLE hToken);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetProcessUserHome(DEE_A_IN HANDLE hProcess);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetProcessUserHome(DEE_A_IN HANDLE hProcess);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeWin32Sys_Utf8GetHome(void);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWin32Sys_WideGetHome(void);
#define DeeWin32SysFS_Utf8GetHome DeeWin32Sys_Utf8GetHome
#define DeeWin32SysFS_WideGetHome DeeWin32Sys_WideGetHome

#ifndef DeeWin32Sys_FiletimeToTimetick // NOTE: Also implemented in "_win32.sysfd.h"
#define DeeWin32Sys_FiletimeToTimetick(ft) (((ft)/DEE_UINT64_C(10000))+DEE_UINT64_C(50522659200000))
#define DeeWin32Sys_TimetickToFiletime(tt) (((tt)-DEE_UINT64_C(50522659200000))*DEE_UINT64_C(10000))
#endif


static BOOL DeeWin32Sys_WideTryGetTimes(
 Dee_WideChar const *path, Dee_timetick_t *atime, Dee_timetick_t *ctime, Dee_timetick_t *mtime) {
 WIN32_FILE_ATTRIBUTE_DATA attr; Dee_timetick_t temp;
 if DEE_UNLIKELY(!GetFileAttributesExW(path,GetFileExInfoStandard,&attr)) {
  if (path[0] != '\\' || path[1] != '\\' || path[2] != '?' || path[3] != '\\') {
   DeeObject *unc_path; Dee_size_t path_size; BOOL temp;
   // Retry after UNC formating
   path_size = Dee_WideStrLen(path);
   if DEE_UNLIKELY((unc_path = DeeWideString_NewSized(
    path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
   memcpy(DeeWideString_STR(unc_path)+4,path,path_size);
   DeeWideString_STR(unc_path)[0] = '\\';
   DeeWideString_STR(unc_path)[1] = '\\';
   DeeWideString_STR(unc_path)[2] = '?';
   DeeWideString_STR(unc_path)[3] = '\\';
   temp = GetFileAttributesExW(DeeWideString_STR(unc_path),GetFileExInfoStandard,&attr);
   Dee_DECREF(unc_path);
   if (!temp) return FALSE;
  } else return FALSE;
 }
 if (atime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftLastAccessTime,(LPFILETIME)&temp)) return FALSE; *atime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (ctime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftCreationTime,  (LPFILETIME)&temp)) return FALSE; *ctime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (mtime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftLastWriteTime, (LPFILETIME)&temp)) return FALSE; *mtime = DeeWin32Sys_FiletimeToTimetick(temp); }
 return TRUE;
}
static BOOL DeeWin32Sys_Utf8TryGetTimes(
 Dee_Utf8Char const *path, Dee_timetick_t *atime,
 Dee_timetick_t *ctime, Dee_timetick_t *mtime) {
 DeeObject *wpath; BOOL result;
 if DEE_UNLIKELY((wpath = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return FALSE; }
 result = DeeWin32Sys_WideTryGetTimes(DeeWideString_STR(wpath),atime,ctime,mtime);
 Dee_DECREF(wpath);
 return result;
}


#define DeeWin32SysFS_Utf8TryGetTimes DeeWin32Sys_Utf8TryGetTimes
#define DeeWin32SysFS_WideTryGetTimes DeeWin32Sys_WideTryGetTimes

// >> [[optional]] void DeeSysFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);




#define DeeSysFS_Utf8GetCwd      DeeWin32SysFS_Utf8GetCwd
#define DeeSysFS_WideGetCwd      DeeWin32SysFS_WideGetCwd
#define DeeSysFS_Utf8TryChdir    DeeWin32SysFS_Utf8TryChdir
#define DeeSysFS_WideTryChdir    DeeWin32SysFS_WideTryChdir
#define DeeSysFS_Utf8Chdir       DeeWin32SysFS_Utf8Chdir
#define DeeSysFS_WideChdir       DeeWin32SysFS_WideChdir
#define DeeSysFS_Utf8GetEnv      DeeWin32SysFS_Utf8GetEnv
#define DeeSysFS_WideGetEnv      DeeWin32SysFS_WideGetEnv
#define DeeSysFS_Utf8TryGetEnv   DeeWin32SysFS_Utf8TryGetEnv
#define DeeSysFS_WideTryGetEnv   DeeWin32SysFS_WideTryGetEnv
#define DeeSysFS_Utf8HasEnv      DeeWin32SysFS_Utf8HasEnv
#define DeeSysFS_WideHasEnv      DeeWin32SysFS_WideHasEnv
#define DeeSysFS_Utf8TryHasEnv   DeeWin32SysFS_Utf8TryHasEnv
#define DeeSysFS_WideTryHasEnv   DeeWin32SysFS_WideTryHasEnv
#define DeeSysFS_Utf8DelEnv      DeeWin32SysFS_Utf8DelEnv
#define DeeSysFS_WideDelEnv      DeeWin32SysFS_WideDelEnv
#define DeeSysFS_Utf8TryDelEnv   DeeWin32SysFS_Utf8TryDelEnv
#define DeeSysFS_WideTryDelEnv   DeeWin32SysFS_WideTryDelEnv
#define DeeSysFS_Utf8SetEnv      DeeWin32SysFS_Utf8SetEnv
#define DeeSysFS_WideSetEnv      DeeWin32SysFS_WideSetEnv
#define DeeSysFS_Utf8TrySetEnv   DeeWin32SysFS_Utf8TrySetEnv
#define DeeSysFS_WideTrySetEnv   DeeWin32SysFS_WideTrySetEnv
#define DeeSysFS_Utf8EnumEnv     DeeWin32SysFS_Utf8EnumEnv
#define DeeSysFS_WideEnumEnv     DeeWin32SysFS_WideEnumEnv
#define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DEE_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_Utf8GetHome     DeeWin32SysFS_Utf8GetHome
#define DeeSysFS_WideGetHome     DeeWin32SysFS_WideGetHome
#define DeeSysFS_Utf8GetTmp      DeeWin32SysFS_Utf8GetTmp
#define DeeSysFS_WideGetTmp      DeeWin32SysFS_WideGetTmp
#define DeeSysFS_Utf8TryGetTimes DeeWin32SysFS_Utf8TryGetTimes
#define DeeSysFS_WideTryGetTimes DeeWin32SysFS_WideTryGetTimes

DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "_win32.sysfs.unicode_impl.h"
#include "_win32.sysfs.unicode_impl.h"
#endif

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_H */
