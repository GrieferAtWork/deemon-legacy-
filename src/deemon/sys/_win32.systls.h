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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSTLS_H
#define GUARD_DEEMON_SYS__WIN32_SYSTLS_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

struct DeeWin32SysTLS { DWORD st_handle; };
#define DeeWin32SysTLS_TryInit(ob) (((ob)->st_handle = TlsAlloc()) != TLS_OUT_OF_INDEXES)
#define DeeWin32SysTLS_Init(ob,...) \
do{\
 if DEE_UNLIKELY(!DeeWin32SysTLS_TryInit(ob)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "TlsAlloc() : %K",\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32SysTLS_Quit(ob)                (void)TlsFree((ob)->st_handle)
#define DeeWin32SysTLS_TryGetNofail(ob,result) (void)(*(void **)&(result)=TlsGetValue((ob)->st_handle))
#define DeeWin32SysTLS_TryGet(ob,result)       ((*(void **)&(result)=TlsGetValue((ob)->st_handle)) != NULL || GetLastError() == 0)
#define DeeWin32SysTLS_TrySet(ob,value)        TlsSetValue((ob)->st_handle,(LPVOID)(value))
#define DeeWin32SysTLS_Get(ob,result,...) \
do{\
 if DEE_UNLIKELY((*(void **)&(result) = TlsGetValue((ob)->st_handle)) == NULL) {\
  DWORD error = GetLastError();\
  if DEE_UNLIKELY(error != 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "TlsGetValue(%lu) : %K",(ob)->st_handle,\
                       DeeSystemError_Win32ToString(error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeWin32SysTLS_Set(ob,value,...) \
do{\
 if DEE_UNLIKELY(!DeeWin32SysTLS_TrySet(ob,value)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "TlsSetValue(%lu,%p) : %K",(ob)->st_handle,value,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeSysTLS              DeeWin32SysTLS
#define DeeSysTLS_TryInit      DeeWin32SysTLS_TryInit
#define DeeSysTLS_Init         DeeWin32SysTLS_Init
#define DeeSysTLS_Quit         DeeWin32SysTLS_Quit
#define DeeSysTLS_TryGetNofail DeeWin32SysTLS_TryGetNofail
#define DeeSysTLS_TryGet       DeeWin32SysTLS_TryGet
#define DeeSysTLS_TrySet       DeeWin32SysTLS_TrySet
#define DeeSysTLS_Get          DeeWin32SysTLS_Get
#define DeeSysTLS_Set          DeeWin32SysTLS_Set

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSTLS_H */
