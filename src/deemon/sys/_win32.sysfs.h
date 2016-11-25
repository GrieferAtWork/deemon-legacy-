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
#include <deemon/sys/_win32.sysfd.h>

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

// TODO: The \\?\ prefix only works for absolute paths!
//    >> When prepending it, we have to ensure that the path is absolute.
//       And when it isn't, we need to make it be (using the real cwd)!

#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif

#define DeeWin32Sys_Utf8IsUNCPath(path) ((path)[0]=='\\'&&(path)[1]=='\\'&&(path)[2]=='?'&&(path)[3]=='\\')
#define DeeWin32Sys_WideIsUNCPath(path) ((path)[0]=='\\'&&(path)[1]=='\\'&&(path)[2]=='?'&&(path)[3]=='\\')
#define DeeWin32Sys_Utf8IsUNCPathObject(path) DeeWin32Sys_Utf8IsUNCPath(DeeUtf8String_STR(path))
#define DeeWin32Sys_WideIsUNCPathObject(path) DeeWin32Sys_WideIsUNCPath(DeeWideString_STR(path))
#define DeeWin32Sys_ISUNCPATHERROR(err)\
((err)==ERROR_FILE_NOT_FOUND||(err)==ERROR_PATH_NOT_FOUND||\
 (err)==ERROR_BAD_PATHNAME||(err)==ERROR_INVALID_NAME||\
 (err)==ERROR_BAD_NETPATH||(err)==ERROR_BAD_NET_NAME)
#define DeeWin32Sys_ISNOTFOUNDERROR(err)\
((err)==ERROR_FILE_NOT_FOUND||(err)==ERROR_PATH_NOT_FOUND||\
 (err)==ERROR_BAD_PATHNAME||(err)==ERROR_INVALID_NAME||\
 (err)==ERROR_BAD_NETPATH||(err)==ERROR_BAD_NET_NAME)


#define DeeWin32Sys_Utf8GetCwd(result,...) \
do{\
 DWORD _wc_temp,_wc_bufsize;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized((\
  _wc_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD))) == NULL) {__VA_ARGS__;}\
 while (1) {\
  _wc_temp = GetCurrentDirectoryA(_wc_bufsize+1,DeeUtf8String_STR(*(result)));\
  if DEE_UNLIKELY(!_wc_temp) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"GetCurrentDirectoryA() : %K",\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   Dee_DECREF(*(result));\
   {__VA_ARGS__;}\
  }\
  if DEE_UNLIKELY(DeeUtf8String_Resize(result,_wc_temp) != 0) { Dee_DECREF(*(result)); {__VA_ARGS__;}}\
  if (_wc_temp <= _wc_bufsize) break;\
 }\
}while(0)
#define DeeWin32Sys_WideGetCwd(result,...) \
do{\
 DWORD _wc_temp,_wc_bufsize;\
 if DEE_UNLIKELY((*(result) = DeeWideString_NewSized((\
  _wc_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD))) == NULL) {__VA_ARGS__;}\
 while (1) {\
  _wc_temp = GetCurrentDirectoryW(_wc_bufsize+1,DeeWideString_STR(*(result)));\
  if DEE_UNLIKELY(!_wc_temp) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"GetCurrentDirectoryW() : %K",\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   Dee_DECREF(*(result));\
   {__VA_ARGS__;}\
  }\
  if DEE_UNLIKELY(DeeWideString_Resize(result,_wc_temp) != 0) { Dee_DECREF(*(result)); {__VA_ARGS__;}}\
  if (_wc_temp <= _wc_bufsize) break;\
 }\
}while(0)
#define DeeWin32SysFS_Utf8GetCwd DeeWin32Sys_Utf8GetCwd
#define DeeWin32SysFS_WideGetCwd DeeWin32Sys_WideGetCwd

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
#define DeeWin32SysFS_Utf8HasEnv    DeeWin32Sys_Utf8HasEnv
#define DeeWin32SysFS_WideHasEnv    DeeWin32Sys_WideHasEnv
#define DeeWin32SysFS_Utf8DelEnv    DeeWin32Sys_Utf8DelEnv
#define DeeWin32SysFS_WideDelEnv    DeeWin32Sys_WideDelEnv
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


#define DeeWin32Sys_WideGetTimes(path,atime,ctime,mtime,...)\
do{\
 WIN32_FILE_ATTRIBUTE_DATA _tm_attr; Dee_timetick_t _tm_temp;\
 static char const *_tm_convert_error_message = "FileTimeToLocalFileTime(%I64u) : %K";\
 if DEE_UNLIKELY(!GetFileAttributesExW(path,GetFileExInfoStandard,&_tm_attr)) {\
  if (!DeeWin32Sys_WideIsUNCPath(path)) {\
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

#define DeeWin32SysFS_WideGetTimes    DeeWin32Sys_WideGetTimes

#define DeeWin32Sys_Utf8IsAbs(path,result,...) \
do{\
 if (!*(path)) *(result) = 0; else {\
  Dee_Utf8Char const *_ia_iter = (path);\
  while (_ia_iter[1] && _ia_iter[1] != ':') ++_ia_iter;\
  *(result) = _ia_iter[1] == ':';\
 }\
}while(0)
#define DeeWin32Sys_WideIsAbs(path,result,...) \
do{\
 if (!*(path)) *(result) = 0; else {\
  Dee_WideChar const *_ia_iter = (path);\
  while (_ia_iter[1] && _ia_iter[1] != ':') ++_ia_iter;\
  *(result) = _ia_iter[1] == ':';\
 }\
}while(0)
#define DeeWin32Sys_Utf8IsDrive(path,result,...) \
do{\
 if (!*(path)) *(result) = 0; else {\
  Dee_Utf8Char const *_id_iter = (path);\
  while (_id_iter[1] && _id_iter[1] != ':') ++_id_iter;\
  if (_id_iter[1]) while (_id_iter[2] == '//' || _id_iter[2] == '\\') ++_id_iter;\
  *(result) = _id_iter[1] && !_id_iter[2];\
 }\
}while(0)
#define DeeWin32Sys_WideIsDrive(path,result,...) \
do{\
 if (!*(path)) *(result) = 0; else {\
  Dee_WideChar const *_id_iter = (path);\
  while (_id_iter[1] && _id_iter[1] != ':') ++_id_iter;\
  if (_id_iter[1]) while (_id_iter[2] == '//' || _id_iter[2] == '\\') ++_id_iter;\
  *(result) = _id_iter[1] && !_id_iter[2];\
 }\
}while(0)



#define DeeWin32SysFS_Utf8IsAbs   DeeWin32Sys_Utf8IsAbs
#define DeeWin32SysFS_WideIsAbs   DeeWin32Sys_WideIsAbs
#define DeeWin32SysFS_Utf8IsMount DeeWin32Sys_Utf8IsDrive
#define DeeWin32SysFS_WideIsMount DeeWin32Sys_WideIsDrive
#define DeeWin32SysFS_Utf8IsDrive DeeWin32Sys_Utf8IsDrive
#define DeeWin32SysFS_WideIsDrive DeeWin32Sys_WideIsDrive

#define DeeWin32Sys_WideUnlinkWithLength(path,path_size,...) \
do{\
 if DEE_UNLIKELY(!DeleteFileW(path)) {\
  static char const *_ul_error_message = "DeleteFileW(%lq) : %K";\
  DeeObject *_ul_newpath; Dee_size_t _ul_path_size;\
  if (!DeeWin32Sys_WideIsUNCPath(path)) {\
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

#define DeeWin32Sys_WideRmDirWithLength(path,path_size,...) \
do{\
 if DEE_UNLIKELY(!RemoveDirectoryW(path)) {\
  static char const *_rd_error_message = "RemoveDirectoryW(%lq) : %K";\
  DeeObject *_rd_newpath; Dee_size_t _rd_path_size;\
  if (!DeeWin32Sys_WideIsUNCPath(path)) {\
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

#define DeeWin32Sys_WideMkDirWithLength(path,path_size,...) \
do{\
 if DEE_UNLIKELY(!CreateDirectoryW(path,NULL)) {\
  static char const *_md_error_message = "CreateDirectoryW(%lq) : %K";\
  DeeObject *_md_newpath; Dee_size_t _md_path_size;\
  if (!DeeWin32Sys_WideIsUNCPath(path)) {\
   _md_path_size = (path_size);\
   if DEE_UNLIKELY((_md_newpath = DeeWideString_NewSized(_md_path_size+4)) == NULL) { DeeError_HandledOne(); return FALSE; }\
   memcpy(DeeWideString_STR(_md_newpath)+4,path,_md_path_size*sizeof(Dee_WideChar));\
   DeeWideString_STR(_md_newpath)[0] = '\\';\
   DeeWideString_STR(_md_newpath)[1] = '\\';\
   DeeWideString_STR(_md_newpath)[2] = '?';\
   DeeWideString_STR(_md_newpath)[3] = '\\';\
   if DEE_UNLIKELY(!CreateDirectoryW(DeeWideString_STR(_md_newpath),NULL)) {\
    DeeError_SetStringf(&DeeErrorType_SystemError,_md_error_message,DeeWideString_STR(_md_newpath),\
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
    Dee_DECREF(_md_newpath);\
    {__VA_ARGS__;}\
   }\
   Dee_DECREF(_md_newpath);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_md_error_message,path,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeWin32Sys_WideMkDir(path,...) \
 DeeWin32Sys_WideMkDirWithLength(path,Dee_WideStrLen(path),__VA_ARGS__)
#define DeeWin32Sys_WideMkDirObject(path,...) \
 DeeWin32Sys_WideMkDirWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),__VA_ARGS__)



#define DeeWin32SysFS_WideUnlink                     DeeWin32Sys_WideUnlink
#define DeeWin32SysFS_WideUnlinkObject               DeeWin32Sys_WideUnlinkObject
#define DeeWin32SysFS_WideRmDir                      DeeWin32Sys_WideRmDir
#define DeeWin32SysFS_WideRmDirObject                DeeWin32Sys_WideRmDirObject
#define DeeWin32SysFS_WideMkDir(path,mode,...)       do{(void)(mode); DeeWin32Sys_WideMkDir(path,__VA_ARGS__); }while(0)
#define DeeWin32SysFS_WideMkDirObject(path,mode,...) do{(void)(mode); DeeWin32Sys_WideMkDirObject(path,__VA_ARGS__); }while(0)

#define DeeWin32Sys_WideGetFileAttributesWithLength(path,path_size,result,notfound_attr,...)\
do{\
 static char const *_fa_errorfmt = "GetFileAttributesW(%lq) : %K";\
 if ((*(result) = GetFileAttributesW(path)) == INVALID_FILE_ATTRIBUTES) {\
  DWORD _fa_error = GetLastError();\
  if DEE_LIKELY(!DeeWin32Sys_WideIsUNCPath(path) && DeeWin32Sys_ISUNCPATHERROR(_fa_error)) {\
   Dee_WideChar *uncpath; Dee_size_t _fa_path_size = (path_size);\
   while DEE_UNLIKELY((uncpath = (Dee_WideChar *)malloc_nz((\
    _fa_path_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(uncpath+4,path,_fa_path_size*sizeof(Dee_WideChar));\
   uncpath[0] = '\\',uncpath[1] = '\\';\
   uncpath[2] = '?', uncpath[3] = '\\';\
   uncpath[_fa_path_size+4] = 0;\
   if ((*(result) = GetFileAttributesW(uncpath)) == INVALID_FILE_ATTRIBUTES) {\
    _fa_error = GetLastError();\
    if (DeeWin32Sys_ISNOTFOUNDERROR(_fa_error)) {\
     *(result) = (notfound_attr);\
    } else {\
     DeeError_SetStringf(&DeeErrorType_SystemError,_fa_errorfmt,uncpath,\
                         DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
     free_nn(uncpath);\
     {__VA_ARGS__;}\
    }\
   }\
   free_nn(uncpath);\
  } else if (DeeWin32Sys_ISNOTFOUNDERROR(_fa_error)) {\
   *(result) = (notfound_attr);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_fa_errorfmt,path,\
                       DeeSystemError_Win32ToString(_fa_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeWin32Sys_WideGetFileAttributes(path,result,notfound_attr,...)\
 DeeWin32Sys_WideGetFileAttributesWithLength(path,Dee_WideStrLen(path),result,notfound_attr,__VA_ARGS__)
#define DeeWin32Sys_WideGetFileAttributesObject(path,result,notfound_attr,...)\
 DeeWin32Sys_WideGetFileAttributesWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),result,notfound_attr,__VA_ARGS__)

#define DeeWin32Sys_WideIsFileWithLength(path,path_size,result,...)\
do{\
 DWORD _if_attr;\
 DeeWin32Sys_WideGetFileAttributesWithLength(path,path_size,&_if_attr,0xFFFFFFFF,__VA_ARGS__);\
 *(result) = (_if_attr&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==0;\
}while(0)
#define DeeWin32Sys_WideIsFile(path,result,...)\
 DeeWin32Sys_WideIsFileWithLength(path,Dee_WideStrLen(path),result,__VA_ARGS__)
#define DeeWin32Sys_WideIsFileObject(path,result,...)\
 DeeWin32Sys_WideIsFileWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),result,__VA_ARGS__)

#define DeeWin32Sys_WideIsDirWithLength(path,path_size,result,...)\
do{\
 DWORD _if_attr;\
 DeeWin32Sys_WideGetFileAttributesWithLength(path,path_size,&_if_attr,0,__VA_ARGS__);\
 *(result) = (_if_attr&(FILE_ATTRIBUTE_DIRECTORY))!=0;\
}while(0)
#define DeeWin32Sys_WideIsDir(path,result,...)\
 DeeWin32Sys_WideIsDirWithLength(path,Dee_WideStrLen(path),result,__VA_ARGS__)
#define DeeWin32Sys_WideIsDirObject(path,result,...)\
 DeeWin32Sys_WideIsDirWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),result,__VA_ARGS__)

#define DeeWin32Sys_WideIsLinkWithLength(path,path_size,result,...)\
do{\
 DWORD _if_attr;\
 DeeWin32Sys_WideGetFileAttributesWithLength(path,path_size,&_if_attr,0,__VA_ARGS__);\
 *(result) = (_if_attr&(FILE_ATTRIBUTE_REPARSE_POINT))!=0;\
}while(0)
#define DeeWin32Sys_WideIsLink(path,result,...)\
 DeeWin32Sys_WideIsLinkWithLength(path,Dee_WideStrLen(path),result,__VA_ARGS__)
#define DeeWin32Sys_WideIsLinkObject(path,result,...)\
 DeeWin32Sys_WideIsLinkWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),result,__VA_ARGS__)

#define DeeWin32SysFS_WideIsFile           DeeWin32Sys_WideIsFile
#define DeeWin32SysFS_WideIsFileObject     DeeWin32Sys_WideIsFileObject
#define DeeWin32SysFS_WideIsDir            DeeWin32Sys_WideIsDir
#define DeeWin32SysFS_WideIsDirObject      DeeWin32Sys_WideIsDirObject
#define DeeWin32SysFS_WideIsLink           DeeWin32Sys_WideIsLink
#define DeeWin32SysFS_WideIsLinkObject     DeeWin32Sys_WideIsLinkObject



#define DeeWin32Sys_Utf8IsExecutableWithLength(path,path_size,result,...) \
do{\
 static Dee_Utf8Char const _ie_name_PATHEXT[] = {'P','A','T','H','E','X','T',0};\
 DeeObject *_ie_pathext;\
 if DEE_UNLIKELY((_ie_pathext = DeeWin32Sys_Utf8TryGetEnv(_ie_name_PATHEXT)) == NULL) *(result) = 0;\
 else {\
  Dee_Utf8Char *_ie_iter,*_ie_end,*_ie_extstart;\
  Dee_Utf8Char const *_ie_match_end; Dee_size_t _ie_path_size,_ie_extsize;\
  _ie_match_end = (path)+(_ie_path_size = (path_size));\
  _ie_end = (_ie_iter = DeeUtf8String_STR(_ie_pathext))+DeeUtf8String_SIZE(_ie_pathext);\
  while (1) {\
   _ie_extstart = _ie_iter;\
   while (_ie_iter != _ie_end && *_ie_iter != ';') ++_ie_iter;\
   if ((_ie_extsize = (Dee_size_t)(_ie_iter-_ie_extstart)) != 0 && _ie_extsize < _ie_path_size) {\
    if (memcmp(_ie_match_end-_ie_extsize,_ie_extstart,_ie_extsize*sizeof(Dee_Utf8Char)) == 0) {\
     *(result) = 1;\
     break;\
    }\
   }\
   if (_ie_iter == _ie_end) { *(result) = 0; break; }\
   ++_ie_iter; /* skip ';' */\
  }\
  Dee_DECREF(_ie_pathext);\
 }\
}while(0)
#define DeeWin32Sys_WideIsExecutableWithLength(path,path_size,result,...) \
do{\
 static Dee_WideChar const _ie_name_PATHEXT[] = {'P','A','T','H','E','X','T',0};\
 DeeObject *_ie_pathext;\
 if DEE_UNLIKELY((_ie_pathext = DeeWin32Sys_WideTryGetEnv(_ie_name_PATHEXT)) == NULL) *(result) = 0;\
 else {\
  Dee_WideChar *_ie_iter,*_ie_end,*_ie_extstart;\
  Dee_WideChar const *_ie_match_end; Dee_size_t _ie_path_size,_ie_extsize;\
  _ie_match_end = (path)+(_ie_path_size = (path_size));\
  _ie_end = (_ie_iter = DeeWideString_STR(_ie_pathext))+DeeWideString_SIZE(_ie_pathext);\
  while (1) {\
   _ie_extstart = _ie_iter;\
   while (_ie_iter != _ie_end && *_ie_iter != ';') ++_ie_iter;\
   if ((_ie_extsize = (Dee_size_t)(_ie_iter-_ie_extstart)) != 0 && _ie_extsize < _ie_path_size) {\
    if (memcmp(_ie_match_end-_ie_extsize,_ie_extstart,_ie_extsize*sizeof(Dee_WideChar)) == 0) {\
     *(result) = 1;\
     break;\
    }\
   }\
   if (_ie_iter == _ie_end) { *(result) = 0; break; }\
   ++_ie_iter; /* skip ';' */\
  }\
  Dee_DECREF(_ie_pathext);\
 }\
}while(0)
#define DeeWin32SysFS_Utf8IsExecutable(path,result,...) DeeWin32Sys_Utf8IsExecutableWithLength(path,Dee_Utf8StrLen(path),result,__VA_ARGS__)
#define DeeWin32SysFS_WideIsExecutable(path,result,...) DeeWin32Sys_WideIsExecutableWithLength(path,Dee_WideStrLen(path),result,__VA_ARGS__)
#define DeeWin32SysFS_Utf8IsExecutableObject(path,result,...) DeeWin32Sys_Utf8IsExecutableWithLength(DeeUtf8String_STR(path),DeeUtf8String_SIZE(path),result,__VA_ARGS__)
#define DeeWin32SysFS_WideIsExecutableObject(path,result,...) DeeWin32Sys_WideIsExecutableWithLength(DeeWideString_STR(path),DeeWideString_SIZE(path),result,__VA_ARGS__)

#define DeeWin32Sys_WideCopyEx(src,dst,fail_if_exists,...) \
 DeeWin32Sys_WideCopyWithLengthEx(src,Dee_WideStrLen(src),dst,Dee_WideStrLen(dst),fail_if_exists,__VA_ARGS__)
#define DeeWin32Sys_WideCopyExObject(src,dst,fail_if_exists,...) \
 DeeWin32Sys_WideCopyWithLengthEx(DeeWideString_STR(src),DeeWideString_SIZE(src),\
                                  DeeWideString_STR(dst),DeeWideString_SIZE(dst),\
                                  fail_if_exists,__VA_ARGS__)
#define DeeWin32Sys_WideCopyWithLengthEx(src,src_size,dst,dst_size,fail_if_exists,...)\
do{\
 static char const *_c_errorfmt = "CopyFileW(%lq,%lq,%b) : %K";\
 Dee_WideChar *_c_new_src,*_c_new_dst; Dee_size_t _c_size;\
 if DEE_UNLIKELY(!CopyFileW(src,dst,fail_if_exists)) {\
  if (DeeWin32Sys_WideIsUNCPath(src) && DeeWin32Sys_WideIsUNCPath(dst)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_c_errorfmt,src,dst,(int)(fail_if_exists),\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (DeeWin32Sys_WideIsUNCPath(src)) _c_new_src = (Dee_WideChar *)(src); else {\
   _c_size = (src_size);\
   while DEE_UNLIKELY((_c_new_src = (Dee_WideChar *)\
    malloc_nz((_c_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_c_new_src+4,src,_c_size*sizeof(Dee_WideChar));\
   _c_new_src[_c_size+4] = 0;\
   _c_new_src[0] = '\\',_c_new_src[1] = '\\';\
   _c_new_src[2] = '?', _c_new_src[3] = '\\';\
  }\
  if (DeeWin32Sys_WideIsUNCPath(dst)) _c_new_dst = (Dee_WideChar *)(dst); else {\
   _c_size = (dst_size);\
   while DEE_UNLIKELY((_c_new_dst = (Dee_WideChar *)\
    malloc_nz((_c_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    if (_c_new_src != (Dee_WideChar *)(src)) free_nn(_c_new_src);\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_c_new_dst+4,dst,_c_size*sizeof(Dee_WideChar));\
   _c_new_dst[_c_size+4] = 0;\
   _c_new_dst[0] = '\\',_c_new_dst[1] = '\\';\
   _c_new_dst[2] = '?', _c_new_dst[3] = '\\';\
  }\
  if (!CopyFileW(_c_new_src,_c_new_dst,fail_if_exists)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_c_errorfmt,_c_new_src,_c_new_dst,(int)(fail_if_exists),\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (_c_new_dst != (Dee_WideChar *)(dst)) free_nn(_c_new_dst);\
  if (_c_new_src != (Dee_WideChar *)(src)) free_nn(_c_new_src);\
 }\
}while(0)
#define DeeWin32SysFS_WideCopy(src,dst,...)       DeeWin32Sys_WideCopyEx(src,dst,TRUE,__VA_ARGS__)
#define DeeWin32SysFS_WideCopyObject(src,dst,...) DeeWin32Sys_WideCopyExObject(src,dst,TRUE,__VA_ARGS__)

#define DeeWin32Sys_WideMove(src,dst,...) \
 DeeWin32Sys_WideMoveWithLength(src,Dee_WideStrLen(src),dst,Dee_WideStrLen(dst),__VA_ARGS__)
#define DeeWin32Sys_WideMoveObject(src,dst,...) \
 DeeWin32Sys_WideMoveWithLength(DeeWideString_STR(src),DeeWideString_SIZE(src),\
                                DeeWideString_STR(dst),DeeWideString_SIZE(dst),__VA_ARGS__)
#define DeeWin32Sys_WideMoveWithLength(src,src_size,dst,dst_size,...)\
do{\
 static char const *_m_errorfmt = "MoveFileW(%lq,%lq) : %K";\
 Dee_WideChar *_m_new_src,*_m_new_dst; Dee_size_t _m_size;\
 if DEE_UNLIKELY(!MoveFileW(src,dst)) {\
  if (DeeWin32Sys_WideIsUNCPath(src) && DeeWin32Sys_WideIsUNCPath(dst)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_m_errorfmt,src,dst,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (DeeWin32Sys_WideIsUNCPath(src)) _m_new_src = (Dee_WideChar *)(src); else {\
   _m_size = (src_size);\
   while DEE_UNLIKELY((_m_new_src = (Dee_WideChar *)\
    malloc_nz((_m_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_m_new_src+4,src,_m_size*sizeof(Dee_WideChar));\
   _m_new_src[_m_size+4] = 0;\
   _m_new_src[0] = '\\',_m_new_src[1] = '\\';\
   _m_new_src[2] = '?', _m_new_src[3] = '\\';\
  }\
  if (DeeWin32Sys_WideIsUNCPath(dst)) _m_new_dst = (Dee_WideChar *)(dst); else {\
   _m_size = (dst_size);\
   while DEE_UNLIKELY((_m_new_dst = (Dee_WideChar *)\
    malloc_nz((_m_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    if (_m_new_src != (Dee_WideChar *)(src)) free_nn(_m_new_src);\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_m_new_dst+4,dst,_m_size*sizeof(Dee_WideChar));\
   _m_new_dst[_m_size+4] = 0;\
   _m_new_dst[0] = '\\',_m_new_dst[1] = '\\';\
   _m_new_dst[2] = '?', _m_new_dst[3] = '\\';\
  }\
  if (!MoveFileW(_m_new_src,_m_new_dst)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_m_errorfmt,_m_new_src,_m_new_dst,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (_m_new_dst != (Dee_WideChar *)(dst)) free_nn(_m_new_dst);\
  if (_m_new_src != (Dee_WideChar *)(src)) free_nn(_m_new_src);\
 }\
}while(0)
#define DeeWin32SysFS_WideMove(src,dst,...)       DeeWin32Sys_WideMove(src,dst,__VA_ARGS__)
#define DeeWin32SysFS_WideMoveObject(src,dst,...) DeeWin32Sys_WideMoveObject(src,dst,__VA_ARGS__)


// v implemented in "_win32.sysfd.c.inl"
extern BOOL DeeWin32Sys_CreateSymbolicLinkW(
 Dee_WideChar const *link_name, Dee_WideChar const *target_name);
#define DeeWin32Sys_WideLink(link_name,target_name,...) \
 DeeWin32Sys_WideLinkWithLength(link_name,Dee_WideStrLen(link_name),target_name,Dee_WideStrLen(target_name),__VA_ARGS__)
#define DeeWin32Sys_WideLinkObject(link_name,target_name,...) \
 DeeWin32Sys_WideLinkWithLength(DeeWideString_STR(link_name),DeeWideString_SIZE(link_name),\
                                DeeWideString_STR(target_name),DeeWideString_SIZE(target_name),__VA_ARGS__)
#define DeeWin32Sys_WideLinkWithLength(link_name,link_name_size,target_name,target_name_size,...)\
do{\
 static char const *_l_errorfmt = "CreateSymbolicLinkW(%lq,%lq) : %K";\
 Dee_WideChar *_l_new_link_name,*_l_new_target_name; Dee_size_t _l_size;\
 if DEE_UNLIKELY(!DeeWin32Sys_CreateSymbolicLinkW(link_name,target_name)) {\
  if (DeeWin32Sys_WideIsUNCPath(link_name) && DeeWin32Sys_WideIsUNCPath(target_name)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_l_errorfmt,link_name,target_name,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (DeeWin32Sys_WideIsUNCPath(link_name)) _l_new_link_name = (Dee_WideChar *)(link_name); else {\
   _l_size = (link_name_size);\
   while DEE_UNLIKELY((_l_new_link_name = (Dee_WideChar *)\
    malloc_nz((_l_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_l_new_link_name+4,link_name,_l_size*sizeof(Dee_WideChar));\
   _l_new_link_name[_l_size+4] = 0;\
   _l_new_link_name[0] = '\\',_l_new_link_name[1] = '\\';\
   _l_new_link_name[2] = '?', _l_new_link_name[3] = '\\';\
  }\
  if (DeeWin32Sys_WideIsUNCPath(target_name)) _l_new_target_name = (Dee_WideChar *)(target_name); else {\
   _l_size = (target_name_size);\
   while DEE_UNLIKELY((_l_new_target_name = (Dee_WideChar *)\
    malloc_nz((_l_size+5)*sizeof(Dee_WideChar))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    if (_l_new_link_name != (Dee_WideChar *)(link_name)) free_nn(_l_new_link_name);\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   memcpy(_l_new_target_name+4,target_name,_l_size*sizeof(Dee_WideChar));\
   _l_new_target_name[_l_size+4] = 0;\
   _l_new_target_name[0] = '\\',_l_new_target_name[1] = '\\';\
   _l_new_target_name[2] = '?', _l_new_target_name[3] = '\\';\
  }\
  if (!DeeWin32Sys_CreateSymbolicLinkW(_l_new_link_name,_l_new_target_name)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,_l_errorfmt,_l_new_link_name,_l_new_target_name,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
  if (_l_new_target_name != (Dee_WideChar *)(target_name)) free_nn(_l_new_target_name);\
  if (_l_new_link_name != (Dee_WideChar *)(link_name)) free_nn(_l_new_link_name);\
 }\
}while(0)
#define DeeWin32SysFS_WideLink(link_name,target_name,...)       DeeWin32Sys_WideLink(link_name,target_name,__VA_ARGS__)
#define DeeWin32SysFS_WideLinkObject(link_name,target_name,...) DeeWin32Sys_WideLinkObject(link_name,target_name,__VA_ARGS__)

/*
if ((hfile = WIN32_F(CreateFile)(
   path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,
   FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL)) == INVALID_HANDLE_VALUE)
*/

#ifndef DEE_PRIVATE_REPARSE_DATA_BUFFER_DEFINED
#define DEE_PRIVATE_REPARSE_DATA_BUFFER_DEFINED
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT ReparseDataLength;
  USHORT Reserved;
  union {
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      ULONG  Flags;
      WCHAR  PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      WCHAR  PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      UCHAR DataBuffer[1];
    } GenericReparseBuffer;
  }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define SymbolicLinkReparseBuffer _rd_data.SymbolicLinkReparseBuffer
#define MountPointReparseBuffer   _rd_data.MountPointReparseBuffer
#define GenericReparseBuffer      _rd_data.GenericReparseBuffer
 _rd_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#endif


#if DEE_XCONFIG_FSBUFSIZE_WIN32READLINK > MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#undef DEE_XCONFIG_FSBUFSIZE_WIN32READLINK
#define DEE_XCONFIG_FSBUFSIZE_WIN32READLINK MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#endif
#define DeeWin32Sys_WideReadlink(path,result,...) \
do{\
 HANDLE _rl_hFile; REPARSE_DATA_BUFFER *_rl_buffer,*_rl_newbuffer;\
 DWORD _rl_bufsize,_rl_bytesreturned,_rl_error;\
 if DEE_UNLIKELY((_rl_hFile = DeeWin32Sys_WideCreateFile(path,GENERIC_READ,\
  FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,OPEN_EXISTING,\
  FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT)) == INVALID_HANDLE_VALUE) {__VA_ARGS__;}\
 while DEE_UNLIKELY((_rl_buffer = (REPARSE_DATA_BUFFER *)malloc_nz(DEE_XCONFIG_FSBUFSIZE_WIN32READLINK)) == NULL) {\
  if DEE_LIKELY(Dee_CollectMemory()) continue;\
  CloseHandle(_rl_hFile);\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 _rl_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32READLINK;\
 while DEE_UNLIKELY(!DeviceIoControl(_rl_hFile,FSCTL_GET_REPARSE_POINT,\
   NULL,0,_rl_buffer,_rl_bufsize,&_rl_bytesreturned,NULL)) {\
  if ((_rl_error = GetLastError()) == ERROR_INSUFFICIENT_BUFFER) {\
   _rl_bufsize *= 2;\
   while DEE_UNLIKELY((_rl_newbuffer = (REPARSE_DATA_BUFFER *)\
    realloc_nnz(_rl_buffer,_rl_bufsize)) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    free_nn(_rl_buffer);\
    CloseHandle(_rl_hFile);\
    DeeError_NoMemory();\
    {__VA_ARGS__;}\
   }\
   _rl_buffer = _rl_newbuffer;\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "DeviceIoControl(%p:%lq,FSCTL_GET_REPARSE_POINT,NULL,0,...,%lu,...,NULL) : %K",\
                       _rl_hFile,path,(unsigned long)_rl_bufsize,\
                       DeeSystemError_Win32ToString(_rl_error));\
   free_nn(_rl_buffer);\
   CloseHandle(_rl_hFile);\
   {__VA_ARGS__;}\
  }\
 }\
 CloseHandle(_rl_hFile);\
 switch (_rl_buffer->ReparseTag) {\
  case IO_REPARSE_TAG_SYMLINK:\
   *(result) = DeeWideString_NewWithLength(\
    _rl_buffer->SymbolicLinkReparseBuffer.SubstituteNameLength/sizeof(Dee_WideChar),\
    _rl_buffer->SymbolicLinkReparseBuffer.PathBuffer+\
    _rl_buffer->SymbolicLinkReparseBuffer.SubstituteNameOffset/sizeof(Dee_WideChar));\
   break;\
  case IO_REPARSE_TAG_MOUNT_POINT: {\
   Dee_WideChar const *_rl_begin,*_rl_end;\
   _rl_end = (_rl_begin = _rl_buffer->MountPointReparseBuffer.PathBuffer+\
    (_rl_buffer->MountPointReparseBuffer.SubstituteNameOffset/sizeof(Dee_WideChar)))+\
    (_rl_buffer->MountPointReparseBuffer.SubstituteNameLength/sizeof(Dee_WideChar));\
   /* Get rid of that annoying '\??\' prefix */\
   if (_rl_begin+4 <= _rl_end && (_rl_begin[0] == '\\' || _rl_begin[0] == '/') && _rl_begin[1] == '?' &&\
       _rl_begin[2] == '?' && (_rl_begin[3] == '\\' || _rl_begin[3] == '/')) _rl_begin += 4;\
   *(result) = DeeWideString_NewWithLength((Dee_size_t)(_rl_end-_rl_begin),_rl_begin);\
  } break;\
  default:\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "DeviceIoControl(%p:%lq) : Unknown/Unsupported link type: %lu",\
                       _rl_hFile,path,(unsigned long)_rl_buffer->ReparseTag);\
   free_nn(_rl_buffer);\
   {__VA_ARGS__;}\
   break;\
 }\
 free_nn(_rl_buffer);\
 if DEE_UNLIKELY(!*(result)) {__VA_ARGS__;}\
}while(0)
#define DeeWin32SysFS_WideReadlink DeeWin32Sys_WideReadlink


#if 0 /* TODO: Permissions for CreateDirectoryW */
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
#endif



#define DeeSysFS_Utf8GetCwd                DeeWin32SysFS_Utf8GetCwd
#define DeeSysFS_WideGetCwd                DeeWin32SysFS_WideGetCwd
#define DeeSysFS_Utf8Chdir                 DeeWin32SysFS_Utf8Chdir
#define DeeSysFS_WideChdir                 DeeWin32SysFS_WideChdir
#define DeeSysFS_Utf8GetEnv                DeeWin32SysFS_Utf8GetEnv
#define DeeSysFS_WideGetEnv                DeeWin32SysFS_WideGetEnv
#define DeeSysFS_Utf8TryGetEnv             DeeWin32SysFS_Utf8TryGetEnv
#define DeeSysFS_WideTryGetEnv             DeeWin32SysFS_WideTryGetEnv
#define DeeSysFS_Utf8HasEnv                DeeWin32SysFS_Utf8HasEnv
#define DeeSysFS_WideHasEnv                DeeWin32SysFS_WideHasEnv
#define DeeSysFS_Utf8DelEnv                DeeWin32SysFS_Utf8DelEnv
#define DeeSysFS_WideDelEnv                DeeWin32SysFS_WideDelEnv
#define DeeSysFS_Utf8SetEnv                DeeWin32SysFS_Utf8SetEnv
#define DeeSysFS_WideSetEnv                DeeWin32SysFS_WideSetEnv
#define DeeSysFS_Utf8EnumEnv               DeeWin32SysFS_Utf8EnumEnv
#define DeeSysFS_WideEnumEnv               DeeWin32SysFS_WideEnumEnv
#define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DEE_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_Utf8GetHome               DeeWin32SysFS_Utf8GetHome
#define DeeSysFS_WideGetHome               DeeWin32SysFS_WideGetHome
#define DeeSysFS_Utf8GetTmp                DeeWin32SysFS_Utf8GetTmp
#define DeeSysFS_WideGetTmp                DeeWin32SysFS_WideGetTmp
#define DeeSysFS_WideGetTimes              DeeWin32SysFS_WideGetTimes
#define DeeSysFS_Utf8IsAbs                 DeeWin32SysFS_Utf8IsAbs
#define DeeSysFS_WideIsAbs                 DeeWin32SysFS_WideIsAbs
#define DeeSysFS_Utf8IsMount               DeeWin32SysFS_Utf8IsMount
#define DeeSysFS_WideIsMount               DeeWin32SysFS_WideIsMount
#define DeeSysFS_Utf8IsDrive               DeeWin32SysFS_Utf8IsDrive
#define DeeSysFS_WideIsDrive               DeeWin32SysFS_WideIsDrive
#define DeeSysFS_WideUnlink                DeeWin32SysFS_WideUnlink
#define DeeSysFS_WideUnlinkObject          DeeWin32SysFS_WideUnlinkObject
#define DeeSysFS_WideRmDir                 DeeWin32SysFS_WideRmDir
#define DeeSysFS_WideRmDirObject           DeeWin32SysFS_WideRmDirObject
#define DeeSysFS_WideMkDir                 DeeWin32SysFS_WideMkDir
#define DeeSysFS_WideMkDirObject           DeeWin32SysFS_WideMkDirObject
#define DeeSysFS_WideIsFile                DeeWin32SysFS_WideIsFile
#define DeeSysFS_WideIsFileObject          DeeWin32SysFS_WideIsFileObject
#define DeeSysFS_WideIsDir                 DeeWin32SysFS_WideIsDir
#define DeeSysFS_WideIsDirObject           DeeWin32SysFS_WideIsDirObject
#define DeeSysFS_WideIsLink                DeeWin32SysFS_WideIsLink
#define DeeSysFS_WideIsLinkObject          DeeWin32SysFS_WideIsLinkObject
#define DeeSysFS_Utf8IsExecutable          DeeWin32SysFS_Utf8IsExecutable
#define DeeSysFS_Utf8IsExecutableObject    DeeWin32SysFS_Utf8IsExecutableObject
#define DeeSysFS_WideIsExecutable          DeeWin32SysFS_WideIsExecutable
#define DeeSysFS_WideIsExecutableObject    DeeWin32SysFS_WideIsExecutableObject
#define DeeSysFS_WideCopy                  DeeWin32SysFS_WideCopy
#define DeeSysFS_WideCopyObject            DeeWin32SysFS_WideCopyObject
#define DeeSysFS_WideMove                  DeeWin32SysFS_WideMove
#define DeeSysFS_WideMoveObject            DeeWin32SysFS_WideMoveObject
#define DeeSysFS_WideLink                  DeeWin32SysFS_WideLink
#define DeeSysFS_WideLinkObject            DeeWin32SysFS_WideLinkObject
#define DeeSysFS_WideReadlink              DeeWin32SysFS_WideReadlink

DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "_win32.sysfs.unicode_impl.h"
#include "_win32.sysfs.unicode_impl.h"
#endif

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_H */
