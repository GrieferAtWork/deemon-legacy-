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
#include <Winternl.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004
#endif
#ifndef SystemHandleInformation
#define SystemHandleInformation ((SYSTEM_INFORMATION_CLASS)16)
#endif
typedef NTSTATUS (NTAPI *LPNTQUERYSYSTEMINFORMATION)(
 SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation,
 ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS (NTAPI *LPNTQUERYOBJECT)(
 HANDLE Handle, OBJECT_INFORMATION_CLASS ObjectInformationClass,
 PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);

static LPNTQUERYSYSTEMINFORMATION pNtQuerySystemInformation = NULL;
static LPNTQUERYOBJECT pNtQueryObject = NULL;

typedef struct _SYSTEM_HANDLE {
 ULONG       ProcessId;
 BYTE        ObjectTypeNumber;
 BYTE        Flags;
 USHORT      Handle; // Can be used as a HANDLE (yes, it sounds weird, but it works)
 PVOID       Object;
 ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE;
typedef struct _SYSTEM_HANDLE_INFORMATION {
 DWORD			      Count;
 SYSTEM_HANDLE	Handles[1];
} SYSTEM_HANDLE_INFORMATION;

DEE_STATIC_INLINE(HANDLE)
DeeWin32_SystemHandle_Open(SYSTEM_HANDLE const *self) {
 HANDLE result,hProcess;
 hProcess = OpenProcess(PROCESS_DUP_HANDLE,FALSE,self->ProcessId);
 if DEE_UNLIKELY(!hProcess) return INVALID_HANDLE_VALUE;
 if (!DuplicateHandle(hProcess,(HANDLE)self->Handle,
  GetCurrentProcess(),&result,0,0,0)) result = INVALID_HANDLE_VALUE;
 CloseHandle(hProcess);
 return result;
}


typedef struct _OBJECT_TYPE_INFORMATION {
 UNICODE_STRING Name;
 ULONG TotalNumberOfObjects;
 ULONG TotalNumberOfHandles;
 ULONG TotalPagedPoolUsage;
 ULONG TotalNonPagedPoolUsage;
 ULONG TotalNamePoolUsage;
 ULONG TotalHandleTableUsage;
 ULONG HighWaterNumberOfObjects;
 ULONG HighWaterNumberOfHandles;
 ULONG HighWaterPagedPoolUsage;
 ULONG HighWaterNonPagedPoolUsage;
 ULONG HighWaterNamePoolUsage;
 ULONG HighWaterHandleTableUsage;
 ULONG InvalidAttributes;
 GENERIC_MAPPING GenericMapping;
 ULONG ValidAccess;
 BOOLEAN SecurityRequired;
 BOOLEAN MaintainHandleCount;
 USHORT MaintainTypeList;
 int PoolType;
 ULONG PagedPoolUsage;
 ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;



//////////////////////////////////////////////////////////////////////////
// Based on various sources all over the internet, like these:
// >> http://stackoverflow.com/questions/733384/how-to-enumerate-process-handles
// >> http://forum.sysinternals.com/howto-enumerate-handles_topic18892.html
// >> https://code.msdn.microsoft.com/windowsapps/CppFileHandle-03c8ea0b
//////////////////////////////////////////////////////////////////////////////////////

// NOTE: The ids marked with a 'Confirmed' text are confirmed to match
//       their internal counterparts for the listed versions of windows.
// >> If something goes wrong during type id discovery, deemon
//    will re-use these ids as mappings.
//    Essentially, the closer these are to the real thing, the more likely
//    everything is going to work out correctly if the discovery fails.

#define/*hidden*/DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN 0
#define DEE_WIN32_HANDLETYPE_TYPE_TYPE            1
#define DEE_WIN32_HANDLETYPE_TYPE_SYMBOLIC_LINK   2
#define DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY       3 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_TOKEN           5 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_PROCESS         7 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_THREAD          8 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_EVENT          12 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_EVENT_PAIR      9
#define DEE_WIN32_HANDLETYPE_TYPE_MUTANT         13 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_SEMAPHORE      15 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_TIMER          16 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_PROFILE        14
#define DEE_WIN32_HANDLETYPE_TYPE_WINDOW_STATION 20 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_DESKTOP        21 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_SECTION        35 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_KEY            37 /* Confirmed: w8.1 (x64) */
#define DEE_WIN32_HANDLETYPE_TYPE_PORT           19
#define DEE_WIN32_HANDLETYPE_TYPE_WAITABLE_PORT  22
#define DEE_WIN32_HANDLETYPE_TYPE_CONTROLLER     25
#define DEE_WIN32_HANDLETYPE_TYPE_DEVICE         26
#define DEE_WIN32_HANDLETYPE_TYPE_DRIVER         27
#define DEE_WIN32_HANDLETYPE_TYPE_IO_COMPLETION  28
#define DEE_WIN32_HANDLETYPE_TYPE_FILE           30 /* Confirmed: w8.1 (x64) */

/*
Unknown handle type name 0x11(17) "IRTimer"
Unknown handle type name 0x16(22) "Composition"
Unknown handle type name 0x17(23) "TpWorkerFactory"
Unknown handle type name 0x1d(29) "WaitCompletionPacket"
Unknown handle type name 0x26(38) "ALPC Port"
Unknown handle type name 0x28(40) "WmiGuid"
Unknown handle type name 0x0b(11) "DebugObject"
*/

//////////////////////////////////////////////////////////////////////////
// Smart, version-independent mapping of handle types to constant values above.

static BYTE DeeWin32_HandleTypeMap[256] = {
/*[[[deemon print " 0x00"+((" "+("0xFF,"*16)+"\n")*16).lsstrip(" 0xFF"),; ]]]*/
 0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//[[[end]]]
};
#define DeeWin32_GetHandleType(syshandle)\
  (DeeWin32_HandleTypeMap[syshandle->ObjectTypeNumber] != 0xFF ? DeeWin32_HandleTypeMap[syshandle->ObjectTypeNumber] \
 : DeeWin32_HandleTypeMap[syshandle->ObjectTypeNumber] = DeeWin32_GetEffectiveSystemHandleTypeID(syshandle))


// wide-string-insensitive-lower-compare-utf8
// >> Returns TRUE/FALSE if equal
static BOOL _dee_wstrilcmp8(PWSTR s, USHORT len, char const *match) {
 PWSTR iter = s,end = s+len; char a,b;
 char const *matchiter = match;
 while (iter != end) {
  b = (char)*iter++;
  while ((b == ' '  || b == '\n'
       || b == '\t' || b == '_'
       ) && iter != end) b = (char)*iter++;
  a = *matchiter++; if (!a) return iter == end;
  if (b >= 'A' && b <= 'Z') b += ('a'-'A');
  if (a != b) return FALSE;
 }
 return iter == end;
}

static BYTE DeeWin32_GetHandleTypeFromWindowsName(PWSTR s, USHORT len) {
/*[[[deemon
#include <file>
#include <util>
local found = [];
for (local line: file.open(__FILE__)) {
  local name;
  try name = line.scanf(" # define DEE_WIN32_HANDLETYPE_TYPE_%[^ ]")...;
  catch (e...) continue;
  found.append(name);
}
found2 = [(for (local x: found) x.replace("_","").lower())...];
local max_size = (for (x: found2) #repr x) > ...;
for (local a,b: util::zip(found2,found)) {
   local defname = "DEE_WIN32_HANDLETYPE_TYPE_"+b.upper();
   print "#ifdef "+defname;
   print " if (_dee_wstrilcmp8(s,len,"+repr(a).ljust(max_size)+")) return "+defname+";";
   print "#endif";
}
]]]*/
#ifdef DEE_WIN32_HANDLETYPE_TYPE_TYPE
 if (_dee_wstrilcmp8(s,len,"type"         )) return DEE_WIN32_HANDLETYPE_TYPE_TYPE;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_SYMBOLIC_LINK
 if (_dee_wstrilcmp8(s,len,"symboliclink" )) return DEE_WIN32_HANDLETYPE_TYPE_SYMBOLIC_LINK;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY
 if (_dee_wstrilcmp8(s,len,"directory"    )) return DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_TOKEN
 if (_dee_wstrilcmp8(s,len,"token"        )) return DEE_WIN32_HANDLETYPE_TYPE_TOKEN;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_PROCESS
 if (_dee_wstrilcmp8(s,len,"process"      )) return DEE_WIN32_HANDLETYPE_TYPE_PROCESS;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_THREAD
 if (_dee_wstrilcmp8(s,len,"thread"       )) return DEE_WIN32_HANDLETYPE_TYPE_THREAD;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_EVENT
 if (_dee_wstrilcmp8(s,len,"event"        )) return DEE_WIN32_HANDLETYPE_TYPE_EVENT;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_EVENT_PAIR
 if (_dee_wstrilcmp8(s,len,"eventpair"    )) return DEE_WIN32_HANDLETYPE_TYPE_EVENT_PAIR;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_MUTANT
 if (_dee_wstrilcmp8(s,len,"mutant"       )) return DEE_WIN32_HANDLETYPE_TYPE_MUTANT;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_SEMAPHORE
 if (_dee_wstrilcmp8(s,len,"semaphore"    )) return DEE_WIN32_HANDLETYPE_TYPE_SEMAPHORE;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_TIMER
 if (_dee_wstrilcmp8(s,len,"timer"        )) return DEE_WIN32_HANDLETYPE_TYPE_TIMER;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_PROFILE
 if (_dee_wstrilcmp8(s,len,"profile"      )) return DEE_WIN32_HANDLETYPE_TYPE_PROFILE;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_WINDOW_STATION
 if (_dee_wstrilcmp8(s,len,"windowstation")) return DEE_WIN32_HANDLETYPE_TYPE_WINDOW_STATION;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_DESKTOP
 if (_dee_wstrilcmp8(s,len,"desktop"      )) return DEE_WIN32_HANDLETYPE_TYPE_DESKTOP;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_SECTION
 if (_dee_wstrilcmp8(s,len,"section"      )) return DEE_WIN32_HANDLETYPE_TYPE_SECTION;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_KEY
 if (_dee_wstrilcmp8(s,len,"key"          )) return DEE_WIN32_HANDLETYPE_TYPE_KEY;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_PORT
 if (_dee_wstrilcmp8(s,len,"port"         )) return DEE_WIN32_HANDLETYPE_TYPE_PORT;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_WAITABLE_PORT
 if (_dee_wstrilcmp8(s,len,"waitableport" )) return DEE_WIN32_HANDLETYPE_TYPE_WAITABLE_PORT;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_CONTROLLER
 if (_dee_wstrilcmp8(s,len,"controller"   )) return DEE_WIN32_HANDLETYPE_TYPE_CONTROLLER;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_DEVICE
 if (_dee_wstrilcmp8(s,len,"device"       )) return DEE_WIN32_HANDLETYPE_TYPE_DEVICE;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_DRIVER
 if (_dee_wstrilcmp8(s,len,"driver"       )) return DEE_WIN32_HANDLETYPE_TYPE_DRIVER;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_IO_COMPLETION
 if (_dee_wstrilcmp8(s,len,"iocompletion" )) return DEE_WIN32_HANDLETYPE_TYPE_IO_COMPLETION;
#endif
#ifdef DEE_WIN32_HANDLETYPE_TYPE_FILE
 if (_dee_wstrilcmp8(s,len,"file"         )) return DEE_WIN32_HANDLETYPE_TYPE_FILE;
#endif
//[[[end]]]
 return DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
}

static BYTE DeeWin32_GetEffectiveHandleTypeID(HANDLE handle) {
 BYTE result; OBJECT_TYPE_INFORMATION *objectTypeInfo;
 DEE_ATOMIC_ONCE({
  *(FARPROC *)&pNtQueryObject = GetProcAddress(
   GetModuleHandleA("ntdll.dll"),"NtQueryObject");
 });
 if (!pNtQueryObject) return DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
 if ((objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc_nz(0x1000)) != NULL)  {
  // Retrieve information about the type name and match it against a list of known types
  if (NT_SUCCESS((*pNtQueryObject)(handle,ObjectTypeInformation,objectTypeInfo,0x1000,NULL))) {
   result = DeeWin32_GetHandleTypeFromWindowsName(
    objectTypeInfo->Name.Buffer,
    objectTypeInfo->Name.Length/sizeof(WCHAR));
#ifdef DEE_DEBUG
   if (result != DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN) {
    DEE_LVERBOSE3("Discovered handle type ID %#.2I8x(%I8u) for %l$q\n",
                  result,result,objectTypeInfo->Name.Length/sizeof(WCHAR),
                  objectTypeInfo->Name.Buffer);
   } else {
    DEE_LVERBOSE3("Unknown handle type name %l$q\n",
                  objectTypeInfo->Name.Length/sizeof(WCHAR),
                  objectTypeInfo->Name.Buffer);
   }
#endif
  } else result = DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
  free_nn(objectTypeInfo);
 } else result = DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
 return result;
}

static BYTE DeeWin32_GetEffectivePHIDTypeID(DWORD pid, WORD hid) {
 HANDLE my_handle,proc_handle; BYTE result;
 proc_handle = OpenProcess(PROCESS_DUP_HANDLE,FALSE,pid);
 if DEE_UNLIKELY(!proc_handle) return 0xFF;
 if (DuplicateHandle(proc_handle,(HANDLE)hid,GetCurrentProcess(),&my_handle,0,0,0)) {
  result = DeeWin32_GetEffectiveHandleTypeID(my_handle);
  CloseHandle(my_handle);
 } else if (GetLastError() == ERROR_NOT_SUPPORTED) {
  result = DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
 } else {
  result = 0xFF;
 }
 CloseHandle(proc_handle);
 return result;
}
static BYTE DeeWin32_GetEffectiveSystemHandleTypeID(SYSTEM_HANDLE const *handle) {
 HANDLE my_handle,proc_handle; BYTE result;
 proc_handle = OpenProcess(PROCESS_DUP_HANDLE,FALSE,handle->ProcessId);
 if DEE_UNLIKELY(!proc_handle) goto fallback;
 if (DuplicateHandle(proc_handle,(HANDLE)handle->Handle,GetCurrentProcess(),&my_handle,0,0,0)) {
  result = DeeWin32_GetEffectiveHandleTypeID(my_handle);
  CloseHandle(my_handle);
 } else {
  result = DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN;
 }
 CloseHandle(proc_handle);
 if (result != DEE_WIN32_HANDLETYPE_TYPE_UNKNOWN) return result;
fallback:
 if (handle->ObjectTypeNumber == 0xFF) return 0; // Shouldn't happen?
 return handle->ObjectTypeNumber;
}



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
 if (handle->ProcessId == 2 && handle->ObjectTypeNumber == 16) return FALSE;
 return TRUE;
}


#define DeeWin32_FreeSystemHandleInformation(info) free_nn(info)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) SYSTEM_HANDLE_INFORMATION *)
DeeWin32_CaptureSystemHandleInformation(void) {
 SYSTEM_HANDLE_INFORMATION *result,*newresult;
 ULONG size = 0x2000,needed; NTSTATUS error;
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
 while (1) {
  error = (*pNtQuerySystemInformation)(SystemHandleInformation,result,size,&needed);
  if (error == STATUS_INFO_LENGTH_MISMATCH) {
   size *= 2;
   newresult = (SYSTEM_HANDLE_INFORMATION*)realloc_nnz(result,size);
   if DEE_UNLIKELY(!newresult) {/*err_r:*/ free_nn(result); return FALSE; }
   result = newresult;
  } else if (error != 0) {
   free_nn(result);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "NtQuerySystemInformation(16,...) : %K",
                       DeeSystemError_Win32ToString((DWORD)error));
   return NULL;
  } else break;
 }
 return result;
}







DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_W32_PROCESS_HANDLES_H */
