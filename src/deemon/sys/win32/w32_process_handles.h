/* Copyright (c) 2016 - deemon by Griefer@Work                                    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the"Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED"AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 */
#ifndef GUARD_DEEMON_SYS_WIN32_W32_PROCESS_HANDLES_H
#define GUARD_DEEMON_SYS_WIN32_W32_PROCESS_HANDLES_H 1

#include <deemon/__conf.inl>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/optional/atomic_once.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Based on source code referred here:
// >> http://stackoverflow.com/questions/733384/how-to-enumerate-process-handles
//////////////////////////////////////////////////////////////////////////
// The code & stackoverflow page referred to this disclaimer
// >> Though this being C and not C++, my version is a complete rewrite...
//////////////////////////////////////////////////////////////////////////////////////
// Written by Zoltan Csizmadia, zoltan_csizmadia@yahoo.com
// For companies(Austin,TX): If you would like to get my resume, send an email.
//
// The source is free, but if you want to use it, mention my name and e-mail address
//
//////////////////////////////////////////////////////////////////////////////////////

#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN          0
#define DEE_WIN32_HANDLETYPE_TYPE_TYPE             1
#define DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY        2
#define DEE_WIN32_HANDLETYPE_TYPE_SYMBOLIC_LINK    3
#define DEE_WIN32_HANDLETYPE_TYPE_TOKEN            4
#define DEE_WIN32_HANDLETYPE_TYPE_PROCESS          5
#define DEE_WIN32_HANDLETYPE_TYPE_THREAD           6
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_7        7
#define DEE_WIN32_HANDLETYPE_TYPE_EVENT            8
#define DEE_WIN32_HANDLETYPE_TYPE_EVENT_PAIR       9
#define DEE_WIN32_HANDLETYPE_TYPE_MUTANT           10
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_11       11
#define DEE_WIN32_HANDLETYPE_TYPE_SEMAPHORE        12
#define DEE_WIN32_HANDLETYPE_TYPE_TIMER            13
#define DEE_WIN32_HANDLETYPE_TYPE_PROFILE          14
#define DEE_WIN32_HANDLETYPE_TYPE_WINDOW_STATION   15
#define DEE_WIN32_HANDLETYPE_TYPE_DESKTOP          16
#define DEE_WIN32_HANDLETYPE_TYPE_SECTION          17
#define DEE_WIN32_HANDLETYPE_TYPE_KEY              18
#define DEE_WIN32_HANDLETYPE_TYPE_PORT             19
#define DEE_WIN32_HANDLETYPE_TYPE_WAITABLE_PORT    20
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_21       21
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_22       22
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_23       23
#define DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN_24       24
//#define DEE_WIN32_HANDLETYPE_OB_TYPE_CONTROLLER  ??
//#define DEE_WIN32_HANDLETYPE_OB_TYPE_DEVICE      ??
//#define DEE_WIN32_HANDLETYPE_OB_TYPE_DRIVER      ??
#define DEE_WIN32_HANDLETYPE_TYPE_IO_COMPLETION    25
#define DEE_WIN32_HANDLETYPE_TYPE_FILE             26


typedef struct _SYSTEM_HANDLE {
 DWORD	ProcessID;
 WORD	 HandleType;
 WORD	 HandleNumber;  // Can be used as a HANDLE (yes, it sounds weird, but it works)
 DWORD	KernelAddress;
 DWORD	Flags;
} SYSTEM_HANDLE;
typedef struct _SYSTEM_HANDLE_INFORMATION {
 DWORD			      Count;
 SYSTEM_HANDLE	Handles[1];
} SYSTEM_HANDLE_INFORMATION;


DEE_COMPILER_MSVC_WARNING_PUSH(4996)
DEE_STATIC_INLINE(DWORD) GetNTMajorVersion(void) {
 OSVERSIONINFOEX osvi;
 BOOL bOsVersionInfoEx;
 ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
 osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
 bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
 if (bOsVersionInfoEx == 0) {
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (!GetVersionEx((OSVERSIONINFO *)&osvi)) return FALSE;
 }
 return osvi.dwMajorVersion;
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_STATIC_INLINE(BOOL) DeeWin32_CapturedHandleIsSupported(SYSTEM_HANDLE const *handle) {
 static DWORD dwNTMajorVersion;
 DEE_ATOMIC_ONCE({ dwNTMajorVersion = GetNTMajorVersion(); });
 if (dwNTMajorVersion >= 5) return TRUE; // Windows 2000 supports everything
 //NT4 System process doesn't like if we bother his internal security
 if (handle->ProcessID == 2 && handle->HandleType == 16) return FALSE;
 return TRUE;
}

typedef DWORD (WINAPI *LPNTQUERYSYSTEMINFORMATION)(DWORD,VOID*,DWORD,ULONG*);

DEE_STATIC_INLINE(DEE_A_RET_Z_OPT char const *)
DeeWin32_GetSystemHandleTypeName(WORD ty) {
 switch (ty) {
  case DEE_WIN32_HANDLETYPE_TYPE_TYPE           : return "TYPE";
  case DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY      : return "DIRECTORY";
  case DEE_WIN32_HANDLETYPE_TYPE_SYMBOLIC_LINK  : return "SYMBOLIC_LINK";
  case DEE_WIN32_HANDLETYPE_TYPE_TOKEN          : return "TOKEN";
  case DEE_WIN32_HANDLETYPE_TYPE_PROCESS        : return "PROCESS";
  case DEE_WIN32_HANDLETYPE_TYPE_THREAD         : return "THREAD";
  case DEE_WIN32_HANDLETYPE_TYPE_EVENT          : return "EVENT";
  case DEE_WIN32_HANDLETYPE_TYPE_EVENT_PAIR     : return "EVENT_PAIR";
  case DEE_WIN32_HANDLETYPE_TYPE_MUTANT         : return "MUTANT";
  case DEE_WIN32_HANDLETYPE_TYPE_SEMAPHORE      : return "SEMAPHORE";
  case DEE_WIN32_HANDLETYPE_TYPE_TIMER          : return "TIMER";
  case DEE_WIN32_HANDLETYPE_TYPE_PROFILE        : return "PROFILE";
  case DEE_WIN32_HANDLETYPE_TYPE_WINDOW_STATION : return "WINDOW_STATION";
  case DEE_WIN32_HANDLETYPE_TYPE_DESKTOP        : return "DESKTOP";
  case DEE_WIN32_HANDLETYPE_TYPE_SECTION        : return "SECTION";
  case DEE_WIN32_HANDLETYPE_TYPE_KEY            : return "KEY";
  case DEE_WIN32_HANDLETYPE_TYPE_PORT           : return "PORT";
  case DEE_WIN32_HANDLETYPE_TYPE_WAITABLE_PORT  : return "WAITABLE_PORT";
  case DEE_WIN32_HANDLETYPE_TYPE_IO_COMPLETION  : return "IO_COMPLETION";
  case DEE_WIN32_HANDLETYPE_TYPE_FILE           : return "FILE";
  default: break;
 }
 return NULL;
}

#define DeeWin32_FreeSystemHandleInformation(info) free_nn(info)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) SYSTEM_HANDLE_INFORMATION *)
DeeWin32_CaptureSystemHandleInformation(void) {
 SYSTEM_HANDLE_INFORMATION *result,*newresult;
 DWORD size = 0x2000,needed;
 static LPNTQUERYSYSTEMINFORMATION pNtQuerySystemInformation = NULL;
 DEE_ATOMIC_ONCE({
  *(FARPROC *)&pNtQuerySystemInformation = GetProcAddress(
   GetModuleHandleA("ntdll.dll"),"NtQuerySystemInformation");
 });
 if (!pNtQuerySystemInformation) {
  DeeError_NotImplemented_str("NtQuerySystemInformation");
  return NULL;
 }
 while ((result = (SYSTEM_HANDLE_INFORMATION*)malloc_nz(size)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 // Query the needed buffer size for the objects (system wide)
 while ((*pNtQuerySystemInformation)(16,result,size,&needed) != 0) {
  if DEE_UNLIKELY(needed == 0) { // No handles?
   // TODO: This is probably meant for errors, right? (GetLastError()...)
   free_nn(result);
   while ((result = (SYSTEM_HANDLE_INFORMATION*)malloc_nz(
    Dee_OFFSETOF(SYSTEM_HANDLE_INFORMATION,Handles))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
   }
   result->Count = 0;
   return result;
  }
  VirtualFree(result,0,MEM_RELEASE);
  size = needed+256;
  newresult = (SYSTEM_HANDLE_INFORMATION*)realloc_nnz(result,size);
  if DEE_UNLIKELY(!newresult) {/*err_r:*/ free_nn(result); return FALSE; }
  result = newresult;
 }
 return result;
}







DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_W32_PROCESS_HANDLES_H */
