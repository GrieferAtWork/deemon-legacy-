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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSINFO_H
#define GUARD_DEEMON_SYS__WIN32_SYSINFO_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/error.h>
#include <deemon/string.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#define DeeWin32SysInfo_Utf8GetHostname DeeWin32Sys_Utf8GetHostname
#define DeeWin32Sys_Utf8GetHostname(result,...) \
do{\
 DWORD _hn_error,_hn_result_size = 1+DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME)) == NULL) {__VA_ARGS__;}\
 while (1) {\
  if DEE_UNLIKELY(!GetComputerNameA(DeeUtf8String_STR(*(result)),&_hn_result_size)) {\
   _hn_error = GetLastError();\
   if (_hn_error == ERROR_BUFFER_OVERFLOW\
    || _hn_error == ERROR_INSUFFICIENT_BUFFER\
    || _hn_error == ERROR_MORE_DATA) {\
    if DEE_LIKELY(DeeUtf8String_Resize(result,_hn_result_size++) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "GetComputerNameA(...) : %K",\
                        DeeSystemError_Win32ToString(_hn_error));\
   }\
  } else {\
   if DEE_LIKELY(DeeUtf8String_Resize(result,_hn_result_size) == 0) break;\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32SysInfo_WideGetHostname DeeWin32Sys_WideGetHostname
#define DeeWin32Sys_WideGetHostname(result,...) \
do{\
 DWORD _hn_error,_hn_result_size = 1+DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME;\
 if DEE_UNLIKELY((*(result) = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME)) == NULL) {__VA_ARGS__;}\
 while (1) {\
  if DEE_UNLIKELY(!GetComputerNameW(DeeWideString_STR(*(result)),&_hn_result_size)) {\
   _hn_error = GetLastError();\
   if (_hn_error == ERROR_BUFFER_OVERFLOW\
    || _hn_error == ERROR_INSUFFICIENT_BUFFER\
    || _hn_error == ERROR_MORE_DATA) {\
    if DEE_LIKELY(DeeWideString_Resize(result,_hn_result_size++) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "GetComputerNameW(...) : %K",\
                        DeeSystemError_Win32ToString(_hn_error));\
   }\
  } else {\
   if DEE_LIKELY(DeeWideString_Resize(result,_hn_result_size) == 0) break;\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeWin32SysInfo_Utf8GetUsername DeeWin32Sys_Utf8GetUsername
#define DeeWin32Sys_Utf8GetUsername(result,...) \
do{\
 DWORD _un_error,_un_result_size = 1+DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME)) == NULL) {__VA_ARGS__;}\
 while (1) {\
  if DEE_UNLIKELY(!GetUserNameA(DeeUtf8String_STR(*(result)),&_un_result_size)) {\
   _un_error = GetLastError();\
   if (_un_error == ERROR_BUFFER_OVERFLOW\
    || _un_error == ERROR_INSUFFICIENT_BUFFER\
    || _un_error == ERROR_MORE_DATA) {\
    if DEE_LIKELY(DeeUtf8String_Resize(result,_un_result_size-1) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "GetUserNameA(...) : %K",\
                        DeeSystemError_Win32ToString(_un_error));\
   }\
  } else {\
   if DEE_LIKELY(DeeUtf8String_Resize(result,_un_result_size-1) == 0) break;\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32SysInfo_WideGetUsername DeeWin32Sys_WideGetUsername
#define DeeWin32Sys_WideGetUsername(result,...) \
do{\
 DWORD _un_error,_un_result_size = 1+DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME;\
 if DEE_UNLIKELY((*(result) = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME)) == NULL) {__VA_ARGS__;}\
 while (1) {\
  if DEE_UNLIKELY(!GetUserNameW(DeeWideString_STR(*(result)),&_un_result_size)) {\
   _un_error = GetLastError();\
   if (_un_error == ERROR_BUFFER_OVERFLOW\
    || _un_error == ERROR_INSUFFICIENT_BUFFER\
    || _un_error == ERROR_MORE_DATA) {\
    if DEE_LIKELY(DeeWideString_Resize(result,_un_result_size-1) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "GetUserNameW(...) : %K",\
                        DeeSystemError_Win32ToString(_un_error));\
   }\
  } else {\
   if DEE_LIKELY(DeeWideString_Resize(result,_un_result_size-1) == 0) break;\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)



#define DeeSysInfo_Utf8GetHostname DeeWin32SysInfo_Utf8GetHostname
#define DeeSysInfo_WideGetHostname DeeWin32SysInfo_WideGetHostname
#define DeeSysInfo_Utf8GetUsername DeeWin32SysInfo_Utf8GetUsername
#define DeeSysInfo_WideGetUsername DeeWin32SysInfo_WideGetUsername

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSINFO_H */
