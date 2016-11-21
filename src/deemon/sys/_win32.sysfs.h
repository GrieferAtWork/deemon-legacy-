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
  Dee_Utf8Char *_env_value_begin,*_env_line_end;\
  _env_value_begin = _env_line;\
  /* Skip internal env vars (like per-drive cwd, and others) */\
  if (*_env_value_begin == '=') continue;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  /* No idea what this is about, but lets be safe */\
  if DEE_UNLIKELY(!*_env_value_begin) continue;\
  _env_line_end = _env_value_begin+Dee_Utf8StrLen(_env_line);\
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
  Dee_WideChar *_env_value_begin,*_env_line_end;\
  _env_value_begin = _env_line;\
  /* Skip internal env vars (like per-drive cwd, and others) */\
  if (*_env_value_begin == DEE_WIDECHAR_C('=')) continue;\
  while (*_env_value_begin && *_env_value_begin != DEE_WIDECHAR_C('=')) ++_env_value_begin;\
  /* No idea what this is about, but lets be safe */\
  if DEE_UNLIKELY(!*_env_value_begin) continue;\
  _env_line_end = _env_value_begin+Dee_WideStrLen(_env_line);\
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
 WIN32_FILE_ATTRIBUTE_DATA attr; Dee_timetick_t temp; BOOL success;
 if DEE_UNLIKELY(!GetFileAttributesExW(path,GetFileExInfoStandard,&attr)) {
  if (path[0] != '\\' || path[1] != '\\' || path[2] != '?' || path[3] != '\\') {
   DeeObject *unc_path; Dee_size_t path_size;
   // Retry after UNC formating
   path_size = Dee_WideStrLen(path);
   if DEE_UNLIKELY((unc_path = DeeWideString_NewSized(
    path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
   memcpy(DeeWideString_STR(unc_path)+4,path,path_size);
   DeeWideString_STR(unc_path)[0] = '\\';
   DeeWideString_STR(unc_path)[1] = '\\';
   DeeWideString_STR(unc_path)[2] = '?';
   DeeWideString_STR(unc_path)[3] = '\\';
   success = GetFileAttributesExW(DeeWideString_STR(unc_path),GetFileExInfoStandard,&attr);
   Dee_DECREF(unc_path);
  } else success = FALSE;
  if (!success) return FALSE;
 }
 if (atime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftLastAccessTime,(LPFILETIME)&temp)) return FALSE; *atime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (ctime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftCreationTime,  (LPFILETIME)&temp)) return FALSE; *ctime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (mtime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&attr.ftLastWriteTime, (LPFILETIME)&temp)) return FALSE; *mtime = DeeWin32Sys_FiletimeToTimetick(temp); }
 return TRUE;
}

#define DeeWin32Sys_WideGetTimes(path,atime,ctime,mtime,...)\
do{\
 WIN32_FILE_ATTRIBUTE_DATA _tm_attr; Dee_timetick_t _tm_temp;\
 static char const *_tm_convert_error_message = "FileTimeToLocalFileTime(%I64u) : %K";\
 if DEE_UNLIKELY(!GetFileAttributesExW(path,GetFileExInfoStandard,&_tm_attr)) {\
  if ((path)[0] != '\\' || (path)[1] != '\\' || (path)[2] != '?' || (path)[3] != '\\') {\
   DeeObject *_tm_unc_path; Dee_size_t _tm_path_size;\
   /* Retry after UNC formating */\
   _tm_path_size = Dee_WideStrLen(path);\
   if DEE_UNLIKELY((_tm_unc_path = DeeWideString_NewSized(\
    _tm_path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }\
   memcpy(DeeWideString_STR(_tm_unc_path)+4,path,_tm_path_size);\
   DeeWideString_STR(_tm_unc_path)[0] = '\\';\
   DeeWideString_STR(_tm_unc_path)[1] = '\\';\
   DeeWideString_STR(_tm_unc_path)[2] = '?';\
   DeeWideString_STR(_tm_unc_path)[3] = '\\';\
   if (!GetFileAttributesExW(DeeWideString_STR(_tm_unc_path),GetFileExInfoStandard,&_tm_attr)) {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "GetFileAttributesExW(%lq,GetFileExInfoStandard,...) : %K",\
                        DeeWideString_STR(_tm_unc_path),\
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
    Dee_DECREF(_tm_unc_path);\
    {__VA_ARGS__;}\
   }\
   Dee_DECREF(_tm_unc_path);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetFileAttributesExW(%lq,GetFileExInfoStandard,...) : %K",\
                       path,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
 if (atime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&_tm_attr.ftLastAccessTime,(LPFILETIME)&_tm_temp)) { DeeError_SetStringf(&DeeErrorType_SystemError,_tm_convert_error_message,*(Dee_timetick_t *)&_tm_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(atime) = DeeWin32Sys_FiletimeToTimetick(_tm_temp); }\
 if (ctime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&_tm_attr.ftCreationTime,  (LPFILETIME)&_tm_temp)) { DeeError_SetStringf(&DeeErrorType_SystemError,_tm_convert_error_message,*(Dee_timetick_t *)&_tm_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(ctime) = DeeWin32Sys_FiletimeToTimetick(_tm_temp); }\
 if (mtime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime(&_tm_attr.ftLastWriteTime, (LPFILETIME)&_tm_temp)) { DeeError_SetStringf(&DeeErrorType_SystemError,_tm_convert_error_message,*(Dee_timetick_t *)&_tm_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(mtime) = DeeWin32Sys_FiletimeToTimetick(_tm_temp); }\
}while(0)

#define DeeWin32SysFS_WideTryGetTimes DeeWin32Sys_WideTryGetTimes
#define DeeWin32SysFS_WideGetTimes    DeeWin32Sys_WideGetTimes

DEE_STATIC_INLINE(BOOL) DeeWin32Sys_Utf8TryIsAbs(Dee_Utf8Char const *path);
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryIsAbs(Dee_WideChar const *path);
#define DeeWin32SysFS_Utf8TryIsAbs       DeeWin32Sys_Utf8TryIsAbs
#define DeeWin32SysFS_WideTryIsAbs       DeeWin32Sys_WideTryIsAbs

DEE_STATIC_INLINE(BOOL) DeeWin32Sys_Utf8TryIsDrive(Dee_Utf8Char const *path);
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryIsDrive(Dee_WideChar const *path);
#define DeeWin32SysFS_Utf8TryIsDrive       DeeWin32Sys_Utf8TryIsDrive
#define DeeWin32SysFS_WideTryIsDrive       DeeWin32Sys_WideTryIsDrive


DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryUnlink(Dee_WideChar const *path) {
 DeeObject *newpath; Dee_size_t path_size; BOOL result;
 if DEE_LIKELY(DeleteFileW(path)) return TRUE;
 if (path[0] != '\\' || path[1] != '\\' || path[2] != '?' || path[3] != '\\') {
  path_size = Dee_WideStrLen(path);
  if DEE_UNLIKELY((newpath = DeeWideString_NewSized(path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
  memcpy(DeeWideString_STR(newpath)+4,path,path_size*sizeof(Dee_WideChar));
  DeeWideString_STR(newpath)[0] = '\\';
  DeeWideString_STR(newpath)[1] = '\\';
  DeeWideString_STR(newpath)[2] = '?';
  DeeWideString_STR(newpath)[3] = '\\';
  result = DeleteFileW(DeeWideString_STR(newpath));
  Dee_DECREF(newpath);
  return result;
 }
 return FALSE;
}
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryUnlinkObject(DeeObject const *path) {
 DeeObject *newpath; BOOL result;
 if (DeleteFileW(DeeWideString_STR(path))) return TRUE;
 if (DeeWideString_STR(path)[0] != '\\' || DeeWideString_STR(path)[1] != '\\'
  || DeeWideString_STR(path)[2] != '?' || DeeWideString_STR(path)[3] != '\\') {
  if DEE_UNLIKELY((newpath = DeeWideString_NewSized(
   DeeWideString_SIZE(path)+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
  memcpy(DeeWideString_STR(newpath)+4,path,
         DeeWideString_SIZE(path)*sizeof(Dee_WideChar));
  DeeWideString_STR(newpath)[0] = '\\';
  DeeWideString_STR(newpath)[1] = '\\';
  DeeWideString_STR(newpath)[2] = '?';
  DeeWideString_STR(newpath)[3] = '\\';
  result = DeleteFileW(DeeWideString_STR(newpath));
  Dee_DECREF(newpath);
  return result;
 }
 return FALSE;
}

#define DeeWin32Sys_WideUnlinkWithLength(path,path_size,...) \
do{\
 if DEE_UNLIKELY(!DeleteFileW(path)) {\
  static char const *_ul_error_message = "DeleteFileW(%lq) : %K";\
  DeeObject *_ul_newpath; Dee_size_t _ul_path_size;\
  if ((path)[0] != '\\' || (path)[1] != '\\' || (path)[2] != '?' || (path)[3] != '\\') {\
   _ul_path_size = (path_size);\
   if DEE_UNLIKELY((_ul_newpath = DeeWideString_NewSized(_ul_path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }\
   memcpy(DeeWideString_STR(_ul_newpath)+4,path,_ul_path_size*sizeof(Dee_WideChar));\
   DeeWideString_STR(_ul_newpath)[0] = '\\';\
   DeeWideString_STR(_ul_newpath)[1] = '\\';\
   DeeWideString_STR(_ul_newpath)[2] = '?';\
   DeeWideString_STR(_ul_newpath)[3] = '\\';\
   if DEE_UNLIKELY(!DeleteFileW(DeeWideString_STR(_ul_newpath))) {\
    DeeError_SetStringf(&DeeErrorType_SystemError,_ul_error_message,DeeWideString_STR(_ul_newpath),\
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
    Dee_DECREF(_ul_newpath);\
    {__VA_ARGS__;}\
   }\
   Dee_DECREF(_ul_newpath);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_ul_error_message,path,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeWin32Sys_WideUnlink(path,...) \
 DeeWin32Sys_WideUnlinkWithLength(path,Dee_WideStrLen(path),__VA_ARGS__)
#define DeeWin32Sys_WideUnlinkObject(path,...) \
 DeeWin32Sys_WideUnlinkWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),__VA_ARGS__)


DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryRmDir(Dee_WideChar const *path) {
 DeeObject *newpath; Dee_size_t path_size; BOOL result;
 if (RemoveDirectoryW(path)) return TRUE;
 if (path[0] != '\\' || path[1] != '\\' || path[2] != '?' || path[3] != '\\') {
  path_size = Dee_WideStrLen(path);
  if DEE_UNLIKELY((newpath = DeeWideString_NewSized(path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
  memcpy(DeeWideString_STR(newpath)+4,path,path_size*sizeof(Dee_WideChar));
  DeeWideString_STR(newpath)[0] = '\\';
  DeeWideString_STR(newpath)[1] = '\\';
  DeeWideString_STR(newpath)[2] = '?';
  DeeWideString_STR(newpath)[3] = '\\';
  result = RemoveDirectoryW(DeeWideString_STR(newpath));
  Dee_DECREF(newpath);
  return result;
 }
 return FALSE;
}
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_WideTryRmDirObject(DeeObject const *path) {
 DeeObject *newpath; BOOL result;
 if (RemoveDirectoryW(DeeWideString_STR(path))) return TRUE;
 if (DeeWideString_STR(path)[0] != '\\' || DeeWideString_STR(path)[1] != '\\'
  || DeeWideString_STR(path)[2] != '?' || DeeWideString_STR(path)[3] != '\\') {
  if DEE_UNLIKELY((newpath = DeeWideString_NewSized(
   DeeWideString_SIZE(path)+4)) == NULL) { DeeError_HandledOne(); return FALSE; }
  memcpy(DeeWideString_STR(newpath)+4,path,
         DeeWideString_SIZE(path)*sizeof(Dee_WideChar));
  DeeWideString_STR(newpath)[0] = '\\';
  DeeWideString_STR(newpath)[1] = '\\';
  DeeWideString_STR(newpath)[2] = '?';
  DeeWideString_STR(newpath)[3] = '\\';
  result = RemoveDirectoryW(DeeWideString_STR(newpath));
  Dee_DECREF(newpath);
  return result;
 }
 return FALSE;
}
#define DeeWin32Sys_WideRmDirWithLength(path,path_size,...) \
do{\
 if DEE_UNLIKELY(!RemoveDirectoryW(path)) {\
  static char const *_rd_error_message = "RemoveDirectoryW(%lq) : %K";\
  DeeObject *_rd_newpath; Dee_size_t _rd_path_size;\
  if ((path)[0] != '\\' || (path)[1] != '\\' || (path)[2] != '?' || (path)[3] != '\\') {\
   _rd_path_size = (path_size);\
   if DEE_UNLIKELY((_rd_newpath = DeeWideString_NewSized(_rd_path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }\
   memcpy(DeeWideString_STR(_rd_newpath)+4,path,_rd_path_size*sizeof(Dee_WideChar));\
   DeeWideString_STR(_rd_newpath)[0] = '\\';\
   DeeWideString_STR(_rd_newpath)[1] = '\\';\
   DeeWideString_STR(_rd_newpath)[2] = '?';\
   DeeWideString_STR(_rd_newpath)[3] = '\\';\
   if DEE_UNLIKELY(!RemoveDirectoryW(DeeWideString_STR(_rd_newpath))) {\
    DeeError_SetStringf(&DeeErrorType_SystemError,_rd_error_message,DeeWideString_STR(_rd_newpath),\
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
    Dee_DECREF(_rd_newpath);\
    {__VA_ARGS__;}\
   }\
   Dee_DECREF(_rd_newpath);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_rd_error_message,path,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeWin32Sys_WideRmDir(path,...) \
 DeeWin32Sys_WideRmDirWithLength(path,Dee_WideStrLen(path),__VA_ARGS__)
#define DeeWin32Sys_WideRmDirObject(path,...) \
 DeeWin32Sys_WideRmDirWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),__VA_ARGS__)



#define DeeWin32SysFS_WideTryUnlink       DeeWin32Sys_WideTryUnlink
#define DeeWin32SysFS_WideTryUnlinkObject DeeWin32Sys_WideTryUnlinkObject
#define DeeWin32SysFS_WideUnlink          DeeWin32Sys_WideUnlink
#define DeeWin32SysFS_WideUnlinkObject    DeeWin32Sys_WideUnlinkObject
#define DeeWin32SysFS_WideTryRmDir        DeeWin32Sys_WideTryRmDir
#define DeeWin32SysFS_WideTryRmDirObject  DeeWin32Sys_WideTryRmDirObject
#define DeeWin32SysFS_WideRmDir           DeeWin32Sys_WideRmDir
#define DeeWin32SysFS_WideRmDirObject     DeeWin32Sys_WideRmDirObject



#if 0 /* TODO */
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsFile)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsFile)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if DEE_UNLIKELY(WIN32_F(fixed_GetFileAttributes)(path,&temp) != 0) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES) return (temp&FILE_ATTRIBUTE_DIRECTORY)==0;
  temp = DeeSystemError_Win32Consume();
  return temp != ERROR_FILE_NOT_FOUND &&
         temp != ERROR_PATH_NOT_FOUND &&
         temp != ERROR_INVALID_NAME;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsFile(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if DEE_UNLIKELY((temp = _deefs_posix_try_stat(path,&attrib)) != 0)
   return DEE_UNLIKELY(temp < 0) ? temp : 0;
  return S_ISREG(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsDir)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsDir)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if DEE_UNLIKELY(WIN32_F(fixed_GetFileAttributes)(path,&temp) != 0) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES) {
   if ((temp & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
    HANDLE handle;
    if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
    if DEE_UNLIKELY((handle = WIN32_F(CreateFile)(path,GENERIC_READ,FILE_SHARE_READ|
     FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL)) != INVALID_HANDLE_VALUE) {
     CloseHandle(handle);
     goto ret_attr_dir;
    }
    temp = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
    if (DEE_FS_WIN32_IS_UNC_ERROR(temp) &&
       !DeeFS_F(Win32IsPathUNC)(path)) {
     DeeObject *unc_path; int result;
     if DEE_UNLIKELY((unc_path = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
     result = _DeeFS_WideIsDir(DeeWideString_STR(unc_path));
     Dee_DECREF(unc_path);
     return result;
    }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
    return 0;
   } else {
ret_attr_dir:
    return (temp & FILE_ATTRIBUTE_DIRECTORY) != 0;
   }
  }
  temp = DeeSystemError_Win32Consume();
  return !WIN32_IS_FILE_NOT_FOUND_ERROR(temp);
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsDir(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if DEE_UNLIKELY((temp = _deefs_posix_try_stat(path,&attrib)) != 0)
   return DEE_UNLIKELY(temp < 0) ? temp : 0;
  return S_ISDIR(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsExecutable)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsExecutable)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  static DEE_CHAR const s_PATHEXT[] = {'P','A','T','H','E','X','T',0};
  DeeObject *pathext; DEE_CHAR *iter,*ext_begin,*end;
  DEE_CHAR const *path_end,*path_iter; Dee_size_t path_size,ext_size;
  DEE_ASSERT(path);
  if DEE_UNLIKELY((pathext = DeeFS_F(GetEnv)(s_PATHEXT)) == NULL) return -1;
  end = (iter = DEE_STRING_STR(pathext))+DEE_STRING_SIZE(pathext);
  path_end = path+(path_size = DEE_STRLEN(path));
  if (iter != end) while (1) {
   ext_begin = iter;
   while (iter != end && *iter != DEE_CHAR_C(';')) ++iter;
   ext_size = (Dee_size_t)(iter-ext_begin);
   if (ext_size <= path_size) {
    path_iter = path+(path_size-ext_size);
    while (path_iter != path_end) {
     if (DEE_CH_TO_UPPER(*path_iter++) !=
         DEE_CH_TO_UPPER(*ext_begin++)) goto next;
    }
    Dee_DECREF(pathext);
    // It has the right suffix >> Not make sure it exists as a file
    return _DeeFS_F(IsFile)(path);
   }
next:
   if (iter == end) break;
   ++iter; // skip ';'
  }
  Dee_DECREF(pathext);
  return 0;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsExecutable(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if DEE_UNLIKELY((temp = _deefs_posix_try_stat(path,&attrib)) != 0)
   return DEE_UNLIKELY(temp < 0) ? temp : 0;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return (attrib.st_mode & 0111) != 0;
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsDrive)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_CHAR const *path_iter;
 DEE_ASSERT(path); path_iter = path;
 if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
 do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter)); // we allow longer drive names (coz who nose?)
 if (!IS_DRIVE_SEP(*path)) return 0; // no ':' after the drive name
 while (IS_SEP(path[1])) ++path; // skip seps after the ':'
 return !path[1];
#else
 (void)path;
 return 0;
#endif
}
#define MODE2PERMS(mode) \
((((mode)&1)!=0 ? GENERIC_WRITE : 0)\
|(((mode)&2)!=0 ? GENERIC_READ : 0)\
|(((mode)&4)!=0 ? GENERIC_EXECUTE : 0))

#ifdef WIDE
static ULONG DeeFS_Win32InitializeAccessW(EXPLICIT_ACCESSW *access, Dee_mode_t mode)
#else
static ULONG DeeFS_Win32InitializeAccessA(EXPLICIT_ACCESSA *access, Dee_mode_t mode)
#endif
{
 PSID everyone_sid = NULL;
 SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_NT_AUTHORITY;
 // v LEAK
 //AllocateAndInitializeSid(&SIDAuthWorld,1,SECURITY_WORLD_RID,
 //                         0,0,0,0,0,0,0,&everyone_sid);
 AllocateAndInitializeSid(&SIDAuthWorld,2,SECURITY_BUILTIN_DOMAIN_RID,
                          DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&everyone_sid);
 ZeroMemory(access,3*sizeof(*access));
 access[0].grfAccessPermissions = MODE2PERMS(mode);
 access[0].grfAccessMode = SET_ACCESS;
 access[0].grfInheritance = NO_INHERITANCE;
 access[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
 access[0].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
 access[0].Trustee.ptstrName = (DEE_CHAR *)everyone_sid;
 return 1;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_F(MkDir)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_mode_t mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(MkDir)(DEE_STRING_STR(path_ob),mode);
  Dee_DECREF(path_ob);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 BOOL created;
 (void)mode; // TODO: at least the read-only flag?
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;

 // TODO
#if 1
 created = WIN32_F(CreateDirectory)(path,NULL);
#else
 {
  ULONG n; PACL acl = NULL;
#ifdef WIDE
  EXPLICIT_ACCESSW ea[3];
  n = DeeFS_Win32InitializeAccessW(ea,mode);
  SetEntriesInAclW(n,ea,NULL,&acl);
#else
  EXPLICIT_ACCESSA ea[3];
  n = DeeFS_Win32InitializeAccessA(ea,mode);
  SetEntriesInAclA(n,ea,NULL,&acl);
#endif
  PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
  InitializeSecurityDescriptor(sd,SECURITY_DESCRIPTOR_REVISION);
  SetSecurityDescriptorDacl(sd,TRUE,acl,FALSE);
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = sd;
  sa.bInheritHandle = FALSE;
  created = WIN32_F(CreateDirectory)(path,&sa);
//FreeSid(everyone_sid);
  LocalFree(sd);
  LocalFree(acl);
 }
#endif
 if (!created) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *temp;
   if DEE_UNLIKELY((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   error = _DeeFS_WideMkDir(DeeWideString_STR(temp),mode);
   Dee_DECREF(temp);
   return error;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(CreateDirectory))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      path,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8MkDir(DeeUtf8String_STR(path_ob),mode);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(mkdir(path,mode) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "mkdir(" DEE_PRINTF_STRQ ",%#o) : %K",path,mode,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)path,mode;
 return 0;
#endif
}

#endif



#define DeeSysFS_Utf8GetCwd          DeeWin32SysFS_Utf8GetCwd
#define DeeSysFS_WideGetCwd          DeeWin32SysFS_WideGetCwd
#define DeeSysFS_Utf8TryChdir        DeeWin32SysFS_Utf8TryChdir
#define DeeSysFS_WideTryChdir        DeeWin32SysFS_WideTryChdir
#define DeeSysFS_Utf8Chdir           DeeWin32SysFS_Utf8Chdir
#define DeeSysFS_WideChdir           DeeWin32SysFS_WideChdir
#define DeeSysFS_Utf8GetEnv          DeeWin32SysFS_Utf8GetEnv
#define DeeSysFS_WideGetEnv          DeeWin32SysFS_WideGetEnv
#define DeeSysFS_Utf8TryGetEnv       DeeWin32SysFS_Utf8TryGetEnv
#define DeeSysFS_WideTryGetEnv       DeeWin32SysFS_WideTryGetEnv
#define DeeSysFS_Utf8HasEnv          DeeWin32SysFS_Utf8HasEnv
#define DeeSysFS_WideHasEnv          DeeWin32SysFS_WideHasEnv
#define DeeSysFS_Utf8TryHasEnv       DeeWin32SysFS_Utf8TryHasEnv
#define DeeSysFS_WideTryHasEnv       DeeWin32SysFS_WideTryHasEnv
#define DeeSysFS_Utf8DelEnv          DeeWin32SysFS_Utf8DelEnv
#define DeeSysFS_WideDelEnv          DeeWin32SysFS_WideDelEnv
#define DeeSysFS_Utf8TryDelEnv       DeeWin32SysFS_Utf8TryDelEnv
#define DeeSysFS_WideTryDelEnv       DeeWin32SysFS_WideTryDelEnv
#define DeeSysFS_Utf8SetEnv          DeeWin32SysFS_Utf8SetEnv
#define DeeSysFS_WideSetEnv          DeeWin32SysFS_WideSetEnv
#define DeeSysFS_Utf8TrySetEnv       DeeWin32SysFS_Utf8TrySetEnv
#define DeeSysFS_WideTrySetEnv       DeeWin32SysFS_WideTrySetEnv
#define DeeSysFS_Utf8EnumEnv         DeeWin32SysFS_Utf8EnumEnv
#define DeeSysFS_WideEnumEnv         DeeWin32SysFS_WideEnumEnv
#define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DEE_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_Utf8GetHome         DeeWin32SysFS_Utf8GetHome
#define DeeSysFS_WideGetHome         DeeWin32SysFS_WideGetHome
#define DeeSysFS_Utf8GetTmp          DeeWin32SysFS_Utf8GetTmp
#define DeeSysFS_WideGetTmp          DeeWin32SysFS_WideGetTmp
#define DeeSysFS_WideGetTimes        DeeWin32SysFS_WideGetTimes
#define DeeSysFS_WideTryGetTimes     DeeWin32SysFS_WideTryGetTimes
#define DeeSysFS_Utf8TryIsAbs        DeeWin32SysFS_Utf8TryIsAbs
#define DeeSysFS_WideTryIsAbs        DeeWin32SysFS_WideTryIsAbs
#define DeeSysFS_Utf8TryIsMount      DeeWin32SysFS_Utf8TryIsDrive
#define DeeSysFS_WideTryIsMount      DeeWin32SysFS_WideTryIsDrive
#define DeeSysFS_Utf8TryIsDrive      DeeWin32SysFS_Utf8TryIsDrive
#define DeeSysFS_WideTryIsDrive      DeeWin32SysFS_WideTryIsDrive
#define DeeSysFS_WideTryUnlink       DeeWin32SysFS_WideTryUnlink
#define DeeSysFS_WideTryUnlinkObject DeeWin32SysFS_WideTryUnlinkObject
#define DeeSysFS_WideUnlink          DeeWin32SysFS_WideUnlink
#define DeeSysFS_WideUnlinkObject    DeeWin32SysFS_WideUnlinkObject
#define DeeSysFS_WideTryRmDir        DeeWin32SysFS_WideTryRmDir
#define DeeSysFS_WideTryRmDirObject  DeeWin32SysFS_WideTryRmDirObject
#define DeeSysFS_WideRmDir           DeeWin32SysFS_WideRmDir
#define DeeSysFS_WideRmDirObject     DeeWin32SysFS_WideRmDirObject

DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "_win32.sysfs.unicode_impl.h"
#include "_win32.sysfs.unicode_impl.h"
#endif

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_H */
