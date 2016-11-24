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
#ifndef GUARD_DEEMON_PROCECSS_C
#define GUARD_DEEMON_PROCECSS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS

// /include/*
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/list.h>
#include <deemon/gc.h>
#include <deemon/fs_api.h>
#include <deemon/float.h>
#include <deemon/time.h>
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/extern.h>
#include <deemon/mp/pipe.h>
#include <deemon/mp/thread.h>
#include <deemon/mp/process.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/tuple.h>
#include <deemon/sys/sysfs.h>
#include <deemon/file/file.fd.h>
#include "unicode/char_traits.inl"
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include "virtual_fs.h.inl"
#endif
#include DEE_INCLUDE_MEMORY_API()

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <deemon/optional/std/string.h>
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Winternl.h>
DEE_COMPILER_MSVC_WARNING_POP
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PROCESS_H
#include <process.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H
#include <dirent.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H
#include <sys/time.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()


// /src/*
#include <deemon/__xconf.inl>
#include <deemon/posix_features.inl>


// */ (nano...)

#ifdef DEE_PLATFORM_UNIX
#define POSIX_PROCESS_PRIORITY_MIN (-20)
#define POSIX_PROCESS_PRIORITY_MAX   19
#define POSIX_PROCESS_PRIORITY_TO_FLOAT(x) (((double)(x)-POSIX_PROCESS_PRIORITY_MIN)/(POSIX_PROCESS_PRIORITY_MAX-POSIX_PROCESS_PRIORITY_MIN))
#define POSIX_PROCESS_FLOAT_TO_PRIORITY(x) (POSIX_PROCESS_PRIORITY_MIN+(int)((x)*(POSIX_PROCESS_PRIORITY_MAX-POSIX_PROCESS_PRIORITY_MIN)))
#endif

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) _DeeProcess_NewFromHandle(DEE_A_IN DeeProcessHandle handle);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_Utf8SplitCommandLine(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *cmd, DEE_A_OUT_OBJECT(DeeUtf8StringObject) const **exe, DEE_A_OUT_OBJECT(DeeUtf8StringObject) const **args);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_WideSplitCommandLine(DEE_A_IN_OBJECT(DeeWideStringObject) const *cmd, DEE_A_OUT_OBJECT(DeeWideStringObject) const **exe, DEE_A_OUT_OBJECT(DeeWideStringObject) const **args);

#ifdef DEE_PLATFORM_WINDOWS
#ifndef PROCESS_NAME_NATIVE
#define PROCESS_NAME_NATIVE     0x00000001
#endif
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED DWORD) _DeeProcess_Win32FloatToPriority(DEE_A_IN double prio);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeProcess_Utf8Win32GenerateEnvironmentString(DEE_A_IN DeeListObject const *env);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeProcess_WideWin32GenerateEnvironmentString(DEE_A_IN DeeListObject const *env);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) HANDLE) _DeeProcess_Win32AcquireRight(DEE_A_IN HANDLE hprocess, DEE_A_IN DWORD new_rights);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeProcess_Utf8Win32Exe(DEE_A_IN HANDLE hprocess);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeProcess_WideWin32Exe(DEE_A_IN HANDLE hprocess);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeProcess_Utf8Win32Cwd(DEE_A_IN HANDLE process_handle);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeProcess_WideWin32Cwd(DEE_A_IN HANDLE process_handle);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) _DeeProcess_Utf8Win32Cmd(DEE_A_IN HANDLE process_handle, DEE_A_IN int exe_on_fail);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) _DeeProcess_WideWin32Cmd(DEE_A_IN HANDLE process_handle, DEE_A_IN int exe_on_fail);
#endif

#ifdef DEE_PLATFORM_UNIX
static int _DeeProcess_PosixCreate(
 DEE_A_IN_Z char const *exec_proc, char *const *argv, char *const *envv, DEE_A_IN_Z_OPT char const *cwd,
 DEE_A_IN int child_stdin, DEE_A_IN int child_stdout, DEE_A_IN int child_stderr, DEE_A_OUT DeeProcessHandle *handle);
DEE_STATIC_INLINE(void) _DeeProcess_PosixFreeStringList(DEE_A_IN char **listv);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_PosixSplitArgv(DEE_A_IN_Z char const *exe, DEE_A_IN_Z char const *args, DEE_A_OUT char ***argv);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_PosixSplitEnv(DEE_A_IN DeeListObject *env, DEE_A_OUT char ***envv);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) _DeeProcess_PosixGetCmd(DEE_A_IN DeeProcessHandle pid);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_PosixSetPriority(DEE_A_IN pid_t self, DEE_A_IN double priority);
#endif






DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *)
_DeeProcess_NewFromHandle(DEE_A_IN DeeProcessHandle handle) {
 DeeProcessObject *result;
 if ((result = (DeeProcessObject *)DeeObject_Mallocf(
  DeeProcess_SIZEOF_REF,"process: %p",(void *)(Dee_uintptr_t)handle)) != NULL) {
  DeeObject_INIT(result,&DeeProcess_Type);
  result->p_flags = DEE_PROCESS_FLAG_NONE;
  DeeProcess_INIT_REF(result,handle);
 }
 return (DeeObject *)result;
}

#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED DWORD) _DeeProcess_Win32FloatToPriority(DEE_A_IN double priority) {
 // 0.00 -- 0.20 IDLE_PRIORITY_CLASS
 // 0.20 -- 0.40 BELOW_NORMAL_PRIORITY_CLASS
 // 0.40 -- 0.60 NORMAL_PRIORITY_CLASS
 // 0.60 -- 0.80 ABOVE_NORMAL_PRIORITY_CLASS
 // 0.80 -- 1.00 HIGH_PRIORITY_CLASS
 if (priority < 0.2) return IDLE_PRIORITY_CLASS;
 if (priority < 0.4) return BELOW_NORMAL_PRIORITY_CLASS;
 if (priority < 0.6) return NORMAL_PRIORITY_CLASS;
 if (priority < 0.8) return ABOVE_NORMAL_PRIORITY_CLASS;
 return HIGH_PRIORITY_CLASS;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) HANDLE)
_DeeProcess_Win32AcquireRight(DEE_A_IN HANDLE hprocess, DEE_A_IN DWORD new_rights) {
 HANDLE result; DWORD pid = GetProcessId(hprocess);
 DEE_LVERBOSE2("Trying to acquire rights %x for process %p\n",new_rights,hprocess);
 if ((result = OpenProcess(new_rights,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(%lu) : %K",
                      pid,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
 }
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
_DeeProcess_Utf8Win32Exe(DEE_A_IN HANDLE hprocess) {
 DeeObject *result; DWORD result_size; DWORD error;
 typedef BOOL (WINAPI *LPQUERYFULLPROCESSIMAGENAMEA)(HANDLE hProcess, DWORD dwFlags, LPSTR lpExeName, PDWORD lpdwSize);
 static LPQUERYFULLPROCESSIMAGENAMEA p_QueryFullProcessImageNameA = NULL;
 DEE_ATOMIC_ONCE({
  HMODULE hKernel32 = GetModuleHandle(TEXT("KERNEL32"));
  p_QueryFullProcessImageNameA = hKernel32 ? (LPQUERYFULLPROCESSIMAGENAMEA)
   GetProcAddress(hKernel32,"QueryFullProcessImageNameA") : NULL;
 });
 if (!p_QueryFullProcessImageNameA) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,"Not implemented: QueryFullProcessImageNameA");
  return NULL;
 }
 if ((result = DeeUtf8String_NewSized(result_size = 256)) == NULL) return NULL;
again:
 if (!(*p_QueryFullProcessImageNameA)(hprocess,0,
  DeeUtf8String_STR(result),&result_size)) {
  switch (error = DeeSystemError_Win32Consume()) {

   case ERROR_GEN_FAILURE:
    // Generated for Zombie Processes
    Dee_DECREF(result);
    DeeReturn_EmptyUtf8String;

   case ERROR_INSUFFICIENT_BUFFER:
    // Must increase buffer size
    result_size = (DWORD)(DeeUtf8String_SIZE(result)*2);
    if (DeeUtf8String_Resize(&result,(Dee_size_t)result_size) != 0) {
err_r: Dee_DECREF(result); return NULL;
    }
    goto again;

   default:
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "QueryFullProcessImageNameA(" DEE_TYPES_IUPRINTF(
                        DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE) ") : %K",
                        hprocess,error,DeeSystemError_Win32ToString(error));
    goto err_r;
  }
 }
 if (result_size != DeeUtf8String_SIZE(result) &&
     DeeUtf8String_Resize(&result,(Dee_size_t)result_size) != 0)  {
  Dee_DECREF(result);
  return NULL;
 }
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
_DeeProcess_WideWin32Exe(DEE_A_IN HANDLE hprocess) {
 DeeObject *result; DWORD result_size; DWORD error;
 typedef BOOL (WINAPI *LPQUERYFULLPROCESSIMAGENAMEW)(HANDLE hProcess, DWORD dwFlags, LPWSTR lpExeName, PDWORD lpdwSize);
 static LPQUERYFULLPROCESSIMAGENAMEW p_QueryFullProcessImageNameW = NULL;
 DEE_ATOMIC_ONCE({
  HMODULE hKernel32 = GetModuleHandle(TEXT("KERNEL32"));
  p_QueryFullProcessImageNameW = hKernel32 ? (LPQUERYFULLPROCESSIMAGENAMEW)
   GetProcAddress(hKernel32,"QueryFullProcessImageNameW") : NULL;
 });
 if (!p_QueryFullProcessImageNameW) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,"Not implemented: QueryFullProcessImageNameW");
  return NULL;
 }
 if ((result = DeeWideString_NewSized(result_size = 256)) == NULL) return NULL;
again:
 if (!(*p_QueryFullProcessImageNameW)(hprocess,0,
  DeeWideString_STR(result),&result_size)) {
  switch ((error = DeeSystemError_Win32Consume())) {

   case ERROR_GEN_FAILURE:
    // Generated for Zombie Processes
    Dee_DECREF(result);
    DeeReturn_EmptyWideString;

   case ERROR_INSUFFICIENT_BUFFER:
    // Must increase buffer size
    result_size = (DWORD)(DeeWideString_SIZE(result)*2);
    if (DeeWideString_Resize(&result,(Dee_size_t)result_size) != 0) {
err_r: Dee_DECREF(result); return NULL;
    }
    goto again;

   default:
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "QueryFullProcessImageNameA(" DEE_TYPES_IUPRINTF(
                        DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE) ") : %K",
                        hprocess,DeeSystemError_Win32ToString(error));
    goto err_r;
  }
 }
 if (result_size != DeeWideString_SIZE(result) &&
     DeeWideString_Resize(&result,(Dee_size_t)result_size) != 0)  {
  Dee_DECREF(result);
  return NULL;
 }
 return result;
}
DEE_STATIC_INLINE(void) _deewin32_acquire_se_debug_privilege(void) {
 DEE_ATOMIC_ONCE({
  static Dee_WideChar const debug_privilege[] = {
   'S','e','D','e','b','u','g','P','r','i','v','i','l','e','g','e','\0'};
  // Acquire the debug privilege that will allow us to open all the processes (at least if we're admin)
  DeeWin32Sys_AcquirePrivilege(debug_privilege);
 });
}


//////////////////////////////////////////////////////////////////////////
// Returns the commandline / current working directory of a given process
#define DEE_PROCESS_PEB_FLAG_NONE          0x00000000
#define DEE_PROCESS_PEB_FLAG_CORRECTRIGHTS 0x00000001
#define _DeeProcess_Win32GetPEBInfo(process_handle,cmd_,cwd_) \
 _DeeProcess_Win32GetPEBInfoEx(process_handle,DEE_PROCESS_PEB_FLAG_NONE,cmd_,cwd_)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_Win32GetPEBInfoEx(
 DEE_A_IN HANDLE process_handle, DEE_A_IN Dee_uint32_t flags, 
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(DeeWideStringObject) **cmd_,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(DeeWideStringObject) **cwd_) {
 // based on: http://stackoverflow.com/questions/7446887/get-command-line-string-of-64-bit-process-from-32-bit-process
 typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
  PVOID Reserved1[2];
  Dee_uint64_t PebBaseAddress;
  PVOID Reserved2[4];
  ULONG_PTR UniqueProcessId[2];
  PVOID Reserved3[2];
 } PROCESS_BASIC_INFORMATION_WOW64;
 typedef struct _UNICODE_STRING_WOW64 {
  USHORT Length;
  USHORT MaximumLength;
  Dee_uint32_t padding;
  Dee_uint64_t Buffer;
 } UNICODE_STRING_WOW64;
 typedef NTSTATUS (NTAPI *LPNTQUERYINFORMATIONPROCESS)(
  HANDLE ProcessHandle, ULONG ProcessInformationClass,
  PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
 typedef LPNTQUERYINFORMATIONPROCESS LPNTWOW64QUERYINFORMATIONPROCESS64;
 typedef NTSTATUS (NTAPI *LPNTWOW64READVIRTUALMEMORY64)(
  HANDLE ProcessHandle, Dee_uint64_t BaseAddress,
  PVOID Buffer, ULONG64 Size, PULONG64 NumberOfBytesRead);
 static struct OFFSETS_T {
  // Offsets in RTL_USER_PROCESS_PARAMETERS
  DWORD ProcessParametersOffset;
  DWORD CommandLineOffset;
  DWORD CurrentWorkingDirectoryOffset;
 } const known_offsets[2] = {
  {16,  64, 36/*,20,24,28*/}, // default
  {32,  112,56/*,32,40,48*/}, // PROCESSOR_ARCHITECTURE_AMD64
 };
 DeeAnyStringObject *system_cmd;
#ifndef DEE_PLATFORM_64_BIT
 BOOL wow;
#endif
 struct OFFSETS_T const *offsets;
 int result; SYSTEM_INFO si;
 DWORD error,pebSize,ppSize;
 PBYTE peb,pp;
 _deewin32_acquire_se_debug_privilege();
 GetNativeSystemInfo(&si);
 offsets = (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
  ? &known_offsets[1] : &known_offsets[0];
 pebSize = offsets->ProcessParametersOffset + 8;
 while DEE_UNLIKELY((peb = (PBYTE)malloc_nz(pebSize)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 ppSize = offsets->CommandLineOffset + 16;
 while DEE_UNLIKELY((pp = (PBYTE)malloc_nz(ppSize)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  free_nn(peb);
  DeeError_NoMemory();
  return -1;
 }
#ifndef DEE_PLATFORM_64_BIT
 if DEE_UNLIKELY(!IsWow64Process(GetCurrentProcess(),&wow)) {
  DeeError_SystemError("IsWow64Process");
  goto err;
 }
 if (wow) {
  // we're running as a 32-bit process in a 64-bit OS
  PROCESS_BASIC_INFORMATION_WOW64 pbi;
  Dee_uint64_t parameters;
  UNICODE_STRING_WOW64 *pUnicodeString;
  static LPNTWOW64QUERYINFORMATIONPROCESS64 p_NtWow64QueryInformationProcess64 = NULL;
  static LPNTWOW64READVIRTUALMEMORY64 p_NtWow64ReadVirtualMemory64 = NULL;
  DEE_ATOMIC_ONCE({
   HMODULE hNTDll = GetModuleHandle(TEXT("ntdll.dll"));
   p_NtWow64QueryInformationProcess64 = (LPNTWOW64QUERYINFORMATIONPROCESS64)
    GetProcAddress(hNTDll,"NtWow64QueryInformationProcess64");
   p_NtWow64ReadVirtualMemory64 = (LPNTWOW64READVIRTUALMEMORY64)
    GetProcAddress(hNTDll,"NtWow64ReadVirtualMemory64");
  });
  if DEE_UNLIKELY(!p_NtWow64QueryInformationProcess64) { DeeError_NotImplemented_str("NtWow64QueryInformationProcess64"); goto err; }
  if DEE_UNLIKELY(!p_NtWow64ReadVirtualMemory64) { DeeError_NotImplemented_str("NtWow64ReadVirtualMemory64"); goto err; }
  // get process information from 64-bit world
  error = (DWORD)(*p_NtWow64QueryInformationProcess64)(process_handle,0,&pbi,sizeof(pbi),NULL);
  if DEE_UNLIKELY(error != 0) {
   DeeError_SystemErrorExplicit("NtWow64QueryInformationProcess64",error);
   goto err;
  }
  // read PEB from 64-bit address space
  error = (DWORD)(*p_NtWow64ReadVirtualMemory64)(process_handle,pbi.PebBaseAddress,peb,pebSize,NULL);
  if DEE_UNLIKELY(error != 0) {
err_read_wow64:
   if ((flags&DEE_PROCESS_PEB_FLAG_CORRECTRIGHTS)==0) {
    // Retry with the correct rights
    if DEE_UNLIKELY((process_handle = _DeeProcess_Win32AcquireRight(
     process_handle,PROCESS_QUERY_INFORMATION|PROCESS_VM_READ)
     ) == NULL) return -1;
    result = _DeeProcess_Win32GetPEBInfoEx(
     process_handle,flags|DEE_PROCESS_PEB_FLAG_CORRECTRIGHTS,cmd_,cwd_);
    if DEE_UNLIKELY(!CloseHandle(process_handle)) SetLastError(0);
    goto end;
   }
   DeeError_SystemErrorExplicit("NtWow64ReadVirtualMemory64",error);
   goto err;
  }
  // read ProcessParameters from 64-bit address space
  parameters = *(Dee_uint64_t*)(peb+offsets->ProcessParametersOffset); // address in remote process adress space
  error = (DWORD)(*p_NtWow64ReadVirtualMemory64)(process_handle,parameters,pp,ppSize,NULL);
  if DEE_UNLIKELY(error != 0) goto err_read_wow64;
  if (cmd_) { // read CommandLine
   pUnicodeString = (UNICODE_STRING_WOW64*)(pp+offsets->CommandLineOffset);
   if DEE_UNLIKELY((system_cmd = (DeeAnyStringObject *)DeeWideString_NewSized(
    pUnicodeString->Length/sizeof(Dee_WideChar))) == NULL) goto err;
   error = (DWORD)(*p_NtWow64ReadVirtualMemory64)(process_handle,pUnicodeString->Buffer,
                                                  DeeWideString_STR(system_cmd),
                                                  pUnicodeString->Length,NULL);
   if DEE_UNLIKELY(error != 0) { Dee_DECREF(system_cmd); goto err_read_wow64; }
   while (DeeWideString_SIZE(system_cmd) &&
          DEE_CH_IS_SPACE(DeeWideString_STR(system_cmd)[DeeWideString_SIZE(system_cmd)-1])
          ) --system_cmd->as_len;
   Dee_INHERIT_REF(*cmd_,*(DeeObject **)&system_cmd);
  }
  if (cwd_) { // read Current Working directory
   pUnicodeString = (UNICODE_STRING_WOW64*)(pp+offsets->CurrentWorkingDirectoryOffset);
   if DEE_UNLIKELY((*cwd_ = DeeWideString_NewSized(pUnicodeString->Length/sizeof(Dee_WideChar))) == NULL) goto err2;
   error = (DWORD)(*p_NtWow64ReadVirtualMemory64)(process_handle,pUnicodeString->Buffer,DeeWideString_STR(*cwd_),pUnicodeString->Length,NULL);
   if DEE_UNLIKELY(error != 0) { Dee_DECREF(*cwd_); if (cmd_) Dee_DECREF(*cmd_); goto err_read_wow64; }
  }
 } else
#endif
 {
  // we're running as a 32-bit process in a 32-bit OS, or as a 64-bit process in a 64-bit OS
  PROCESS_BASIC_INFORMATION pbi; PBYTE *parameters;
  UNICODE_STRING *pCommandLine;
  static LPNTQUERYINFORMATIONPROCESS p_NtQueryInformationProcess = NULL;
  // get process information
  DEE_ATOMIC_ONCE({
   p_NtQueryInformationProcess = (LPNTQUERYINFORMATIONPROCESS)GetProcAddress(
    GetModuleHandle(TEXT("ntdll.dll")),"NtQueryInformationProcess");
  });
  if DEE_UNLIKELY(!p_NtQueryInformationProcess) { DeeError_NotImplemented_str("NtQueryInformationProcess"); goto err; }
  error = (DWORD)(*p_NtQueryInformationProcess)(process_handle,0,&pbi,sizeof(pbi),NULL);
  if DEE_UNLIKELY(error != 0) { DeeError_SystemError("NtQueryInformationProcess"); goto err; }
  // read PEB
  if DEE_UNLIKELY(!ReadProcessMemory(process_handle,pbi.PebBaseAddress,peb,pebSize,NULL)) {
err_read:
   if ((flags&DEE_PROCESS_PEB_FLAG_CORRECTRIGHTS)==0) {
    // Retry with the correct rights
    if DEE_UNLIKELY((process_handle = _DeeProcess_Win32AcquireRight(
     process_handle,PROCESS_QUERY_INFORMATION|PROCESS_VM_READ)
     ) == NULL) return -1;
    result = _DeeProcess_Win32GetPEBInfoEx(
     process_handle,flags|DEE_PROCESS_PEB_FLAG_CORRECTRIGHTS,cmd_,cwd_);
    if DEE_UNLIKELY(!CloseHandle(process_handle)) SetLastError(0);
    goto end;
   }
   DeeError_SystemError("ReadProcessMemory");
   goto err;
  }
  // read ProcessParameters
  parameters = (PBYTE*)*(LPVOID*)(peb+offsets->ProcessParametersOffset); // address in remote process adress space
  if DEE_UNLIKELY(!ReadProcessMemory(process_handle,parameters,pp,ppSize,NULL)) goto err_read;
  if (cmd_) { // read CommandLine
   pCommandLine = (UNICODE_STRING*)(pp+offsets->CommandLineOffset);
   if DEE_UNLIKELY((system_cmd = (DeeAnyStringObject *)DeeWideString_NewSized(
    pCommandLine->Length/sizeof(Dee_WideChar))) == NULL) goto err;
   if DEE_UNLIKELY(!ReadProcessMemory(process_handle,pCommandLine->Buffer,DeeWideString_STR(
    system_cmd),pCommandLine->Length,NULL)) { Dee_DECREF(system_cmd); goto err_read; }
   while (DeeWideString_SIZE(system_cmd) &&
          DEE_CH_IS_SPACE(DeeWideString_STR(system_cmd)[DeeWideString_SIZE(system_cmd)-1])
          ) --system_cmd->as_len;
   Dee_INHERIT_REF(*cmd_,*(DeeObject **)&system_cmd);
  }
  if (cwd_) { // read Current Working directory
   pCommandLine = (UNICODE_STRING*)(pp+offsets->CurrentWorkingDirectoryOffset);
   if DEE_UNLIKELY((*cwd_ = DeeWideString_NewSized(pCommandLine->Length/sizeof(Dee_WideChar))) == NULL) goto err2;
   if DEE_UNLIKELY(!ReadProcessMemory(process_handle,pCommandLine->Buffer,DeeWideString_STR(
    *cwd_),pCommandLine->Length,NULL)) { Dee_DECREF(*cwd_); if (cmd_) Dee_DECREF(*cmd_); goto err_read; }
  }
 }
 result = 0;
end: free_nn(peb),free_nn(pp);
 return result;
err2: if (cmd_) Dee_DECREF(*cmd_);
err: result = -1; goto end;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
_DeeProcess_WideWin32Cmd(DEE_A_IN HANDLE process_handle, DEE_A_IN int exe_on_fail) {
 DeeObject *result;
 if DEE_UNLIKELY(_DeeProcess_Win32GetPEBInfo(process_handle,&result,NULL) != 0) {
  if (!exe_on_fail) return NULL;
  result = _DeeProcess_WideWin32Exe(process_handle);
  DeeError_Handled(); // Handle another error, or the first
 }
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
_DeeProcess_Utf8Win32Cmd(DEE_A_IN HANDLE process_handle, DEE_A_IN int exe_on_fail) {
 DeeObject *result;
 if DEE_UNLIKELY((result = _DeeProcess_WideWin32Cmd(process_handle,exe_on_fail)) != NULL)
  if DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&result) != 0)
   Dee_CLEAR(result);
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
_DeeProcess_WideWin32Cwd(DEE_A_IN HANDLE process_handle) {
 DeeObject *result;
 if DEE_UNLIKELY(_DeeProcess_Win32GetPEBInfo(process_handle,NULL,&result) != 0) return NULL;
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
_DeeProcess_Utf8Win32Cwd(DEE_A_IN HANDLE process_handle) {
 DeeObject *result;
 if DEE_UNLIKELY((result = _DeeProcess_WideWin32Cwd(process_handle)) != NULL)
  if DEE_UNLIKELY(DeeUtf8String_InplaceCast((DeeObject const **)&result) != 0)
   Dee_CLEAR(result);
 return result;
}
#endif



#ifdef DEE_PLATFORM_UNIX

#if !DEE_HAVE_EXECVPE
extern int execvpe(const char *file, char *const argv[], char *const envp[]);
#endif

static int _DeeProcess_PosixCreate(
 DEE_A_IN_Z char const *exec_proc,
 char *const argv[], char *const envv[],
 DEE_A_IN_Z_OPT char const *cwd, DEE_A_IN int child_stdin,
 DEE_A_IN int child_stdout, DEE_A_IN int child_stderr,
 DEE_A_OUT DeeProcessHandle *handle) {
 DeeProcessHandle pid;
 if ((pid = fork()) == 0) {
  // Child process
  if (cwd) chdir(cwd);
  if (child_stdin != -1) dup2(child_stdin,0);
  if (child_stdout != -1) dup2(child_stdout,1);
  if (child_stderr != -1) dup2(child_stderr,2);
  if (envv) {
   execvpe(exec_proc,argv,envv);
  } else {
   execvp(exec_proc,argv);
  }
  exit(1);
 } else if (pid < 0) {
  // Error
  DeeError_SystemError("fork");
  return -1;
 } else *handle = pid;
 return 0;
}

DEE_STATIC_INLINE(void)
_DeeProcess_PosixFreeStringList(char **listv) {
 char **iter = listv;
 while (*iter) free_nn(*iter++);
 free_nn(listv);
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_PosixSplitArgv(
 DEE_A_IN_Z char const *exe, DEE_A_IN_Z char const *args, DEE_A_OUT char ***argv) {
 char const *begin;
 char *arg,**argv_iter,**new_argv,**result_argv;
 Dee_size_t arg_size,result_argc = 1;
 DEE_ASSERT(args && argv);
 DEE_LVERBOSE3("Parsing argv string for %q: %q\n",exe,args);
 while ((result_argv = (
  char **)malloc_nz(2*sizeof(char *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem2:
  DeeError_NoMemory();
  return -1;
 }
 while ((result_argv[0] = (*exe ? strdup_nz(exe)
  : (char *)calloc_nz(1,sizeof(char)))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  free_nn(result_argv);
  goto no_mem2;
 }
 if (*args) do {
  begin = args;
  while (*args) {
   if (*args == '"') { while (*++args != '"') if (!*args) goto done; }
   else if (*args == '\\' && args[1]) args += 2;
   else if (DEE_CH_IS_SPACE(*args)) break;
   else ++args;
  }
done:
  arg_size = (Dee_size_t)(args-begin);
  while ((arg = (char *)malloc_nz((arg_size+1)*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
   argv_iter = result_argv+result_argc;
   while (argv_iter != result_argv) free_nn(*--argv_iter);
   free(result_argv);
   goto no_mem2;
  }
  memcpy(arg,begin,arg_size*sizeof(char));
  arg[arg_size] = '\0';
  DEE_LVERBOSE3("Found process argument: %q\n",arg);
  while ((new_argv = (char **)realloc_nz(result_argv,(
   result_argc+2)*sizeof(char *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   free_nn(arg);
   goto no_mem;
  }
  new_argv[result_argc++] = arg;
  result_argv = new_argv;
  while (DEE_CH_IS_SPACE(*args)) ++args;
 } while (*args);
 DEE_LVERBOSE3("Found %Iu arguments; stored in %p\n",result_argc,result_argv);
 result_argv[result_argc] = NULL;
 *argv = result_argv;
 return 0;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_PosixSplitEnv(
 DEE_A_IN DeeListObject *env, DEE_A_OUT char ***envv) {
 char **env_list,**dst_iter,*line; DeeObject **iter,**end,*key,*value;
 if ((env = (DeeListObject *)DeeList_Copy((DeeObject *)env)) == NULL) return -1;
 DEE_LVERBOSE2("Splitting environment list: %r\n",env);
 if (DeeList_SIZE(env) == 0) {
  *envv = NULL;
  goto end;
 }
 end = (iter = DeeList_ELEM(env))+DeeList_SIZE(env);
 while ((env_list = (char **)malloc_nz(
  (DeeList_SIZE(env)+1)*sizeof(char *))
  ) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
  DeeError_NoMemory();
  Dee_DECREF(env);
  return -1;
 }
 dst_iter = env_list;
 while (iter != end) {
  Dee_size_t line_size;
  DEE_ASSERT(DeeObject_Check(*iter) && DeeTuple_Check(*iter));
  DEE_ASSERT(DeeTuple_SIZE(*iter) == 2);
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,0)));
  DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,1)));
  if ((key = DeeString_Cast(DeeTuple_GET(*iter,0))) == NULL) goto err_line;
  if ((value = DeeString_Cast(DeeTuple_GET(*iter,1))) == NULL) { Dee_DECREF(key); goto err_line; }
  line_size = DeeString_SIZE(key)+DeeString_SIZE(value)+2;
  while ((line = (char *)malloc_nz(line_size*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   Dee_DECREF(value);
   Dee_DECREF(key);
err_line:
   while (dst_iter != env_list) free_nn(*--dst_iter);
   free_nn(env_list);
   goto no_mem;
  }
  memcpy(line,DeeString_STR(key),DeeString_SIZE(key));
  line[DeeString_SIZE(key)] = '=';
  memcpy(line+DeeString_SIZE(key)+1,DeeString_STR(value),DeeString_SIZE(value));
  Dee_DECREF(value);
  Dee_DECREF(key);
  line[line_size-1] = '\0';
  *dst_iter++ = line,++iter;
 }
 *envv = env_list;
end: Dee_DECREF(env);
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *)
_DeeProcess_PosixGetCmd(DEE_A_IN DeeProcessHandle pid) {
 DeeObject *file,*filename,*data,*result;
 char *iter,*end,*dst;
 if ((filename = DeeString_Newf("/proc/%d/cmdline",pid)) == NULL) return NULL;
 file = DeeFile_OpenObject(filename,DEE_OPENMODE('r',0));
 Dee_DECREF(filename);
 if (!file) return NULL;
 data = DeeFile_ReadData(file,(Dee_size_t)-1);
 Dee_DECREF(file);
 DEE_LVERBOSE2("Read /proc/%d/cmdline data: %r\n",pid,data);
 if (!data) return NULL;
 if (!DeeString_SIZE(data)) {
  Dee_DECREF(data);
  DeeReturn_EmptyString;
 }
 if ((result = DeeString_NewSized(DeeString_SIZE(data)-1)) == NULL) {
  Dee_DECREF(data);
  return NULL;
 }
 end = (iter = DeeString_STR(data))+(DeeString_SIZE(data)-1);
 dst = DeeString_STR(result);
 while (iter != end) { *dst++ = *iter ? *iter : ' '; ++iter; }
 Dee_DECREF(data);
 return result;
}
#endif





static struct {
 DEE_OBJECT_HEAD
 DeeProcessHandle      p_handle;
 struct DeeAtomicMutex p_lock;
 Dee_uint8_t           p_flags;
} _DeeProcess_Self = {
 DEE_OBJECT_HEAD_INIT(&DeeProcess_Type),
 DEE_PROCESS_INVALID_HANDLE,
 DeeAtomicMutex_INIT(),
 DEE_PROCESS_FLAG_NONE
};

#define DeeProcess_IS_SELF(ob) \
((ob)==(DeeObject*)&_DeeProcess_Self||\
 DeeProcess_ID(ob)==DeeProcess_SelfID())

DEE_A_RET_OBJECT_NOEXCEPT(DeeProcessObject) *DeeProcess_Self(void) {
 return (DeeObject *)&_DeeProcess_Self;
}


DEE_A_RET_EXCEPT(-1) int DeeProcess_Initialize(void) {
#ifdef DEE_PLATFORM_WINDOWS
 _DeeProcess_Self.p_handle = (DeeProcessHandle)GetCurrentProcess();
#elif defined(DEE_PLATFORM_UNIX)
 _DeeProcess_Self.p_handle = getpid();
#endif
 return 0;
}
void DeeProcess_Finalize(void) {
 // Nothing (yet)
}





#ifdef DEE_PLATFORM_UNIX
DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *
DeeProcess_PosixNewFromPid(DEE_A_IN DeeProcessHandle handle) {
 return _DeeProcess_NewFromHandle(handle);
}
#endif

#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *
DeeProcess_Win32NewFromID(DEE_A_IN DeeProcessID id) {
 HANDLE process_handle; // Open the process as a handle (use the lowest possible access right)
 DeeObject *result;
 if (id == GetCurrentProcessId()) { Dee_INCREF(&_DeeProcess_Self); return (DeeObject *)&_DeeProcess_Self; }
 if ((process_handle = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,id)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(%lu) : %K",id,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 if ((result = _DeeProcess_NewFromHandle(process_handle)) == NULL) {
  CloseHandle(process_handle);
 }
 return result;
}
#endif







DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *
DeeProcess_NewObject(DEE_A_IN_OBJECT(DeeStringObject) const *cmd) {
 DeeObject *exe,*args,*result; int temp;
 if (DeeWideString_Check(cmd)) {
  temp = _DeeProcess_WideSplitCommandLine(
   cmd,(DeeObject const **)&exe,(DeeObject const **)&args);
 } else {
  if ((cmd = DeeUtf8String_Cast(cmd)) == NULL) return NULL;
  temp = _DeeProcess_Utf8SplitCommandLine(
   cmd,(DeeObject const **)&exe,(DeeObject const **)&args);
  Dee_DECREF(cmd);
 }
 if (temp != 0) return NULL;
 result = DeeProcess_NewExObject(exe,args);
 Dee_DECREF(args);
 Dee_DECREF(exe);
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *DeeProcess_NewExObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *exe, DEE_A_IN_OBJECT(DeeAnyStringObject) const *args) {
 DeeProcessObject *result;
 DEE_ASSERT(DeeObject_Check(exe));
 DEE_ASSERT(DeeObject_Check(args));
 if ((exe = DeeFS_PathExpandObject(exe)) == NULL) return NULL;
 if (DeeWideString_Check(args)) Dee_INCREF(args);
 else if ((args = DeeWideString_Cast(args)) == NULL) { result = NULL; goto err_exe; }
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 {
  DeeObject *temp;
  if (DeeWideString_Check(exe)) {
   if (DeeVFS_WideIsVirtualPath(DeeWideString_STR(exe))) {
    if ((temp = DeeVFS_WideReadReFsLink(DeeWideString_STR(exe))) == NULL) goto err_args_r;
    Dee_DECREF(exe);
    Dee_INHERIT_REF(exe,temp);
   }
  } else {
   if (DeeUtf8String_InplaceCast((DeeObject const **)&exe) != 0) {
err_args_r: result = NULL; goto err_args;
   }
   if (DeeVFS_Utf8IsVirtualPath(DeeUtf8String_STR(exe))) {
    if ((temp = DeeVFS_Utf8ReadReFsLink(DeeUtf8String_STR(exe))) == NULL) goto err_args_r;
    Dee_DECREF(exe);
    Dee_INHERIT_REF(exe,temp);
   }
  }
 }
#endif
 if ((result = DeeObject_MALLOCF(DeeProcessObject,"Process")) != NULL) {
  DeeObject_INIT(result,&DeeProcess_Type);
  result->p_flags = DEE_PROCESS_FLAG_NOREF;
  DeeProcess_INIT_NONREF(result);
  Dee_INHERIT_REF(result->p_exe,*(DeeAnyStringObject **)&exe);
  Dee_INHERIT_REF(result->p_args,*(DeeAnyStringObject **)&args);
 } else {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
err_args:
#endif
  Dee_DECREF(args);
err_exe: Dee_DECREF(exe);
 }
 return (DeeObject *)result;
}





#define _self ((DeeProcessObject *)self)
static void _deeprocess_error_is_ref(DEE_A_INOUT_OBJECT(DeeProcessObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_ASSERT(DeeProcess_IS_REF(self));
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Invalid operator in process reference %k",self);
}
static void _deeprocess_error_cant_change_running(DeeProcessObject *self, char const *name) {
 DeeError_SetStringf(&DeeErrorType_ValueError,"Can't change <%s> of running %k",name,self);
}



#ifdef DEE_PLATFORM_WINDOWS
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_Start(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN int detached) {
 STARTUPINFOW info;
 DEE_STATIC_ASSERT(sizeof(STARTUPINFOA) == sizeof(STARTUPINFOW));
 DeeObject *usedcwd,*cmdargs,*appname,*cmdline,*env_strings;
 PROCESS_INFORMATION pinfo; BOOL temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  return 1; /* Already started */
 }
 Dee_INCREF(appname = (DeeObject *)_self->p_exe);
 Dee_INCREF(cmdargs = (DeeObject *)_self->p_args);
 Dee_XINCREF(usedcwd = (DeeObject *)_self->p_cwd);
 DeeProcess_RELEASE(self);
 // Generate the command line for windows
 if (DeeWideString_Check(appname)) {
  Dee_WideChar *iter;
  if (DeeWideString_InplaceCast((DeeObject const **)&cmdargs) != 0 ||
     (usedcwd && DeeWideString_InplaceCast((DeeObject const **)&usedcwd) != 0)) {
err_cmdlinegen:
   Dee_XDECREF(usedcwd);
   Dee_DECREF(appname);
   return -1;
  }
  if ((cmdline = DeeWideString_NewSized(
       DeeWideString_SIZE(appname)+3+
       DeeWideString_SIZE(cmdargs))
       ) == NULL) goto err_cmdlinegen;
  iter = DeeWideString_STR(cmdline);
  *iter++ = '"';
  memcpy(iter,DeeWideString_STR(appname),DeeWideString_SIZE(appname)*sizeof(Dee_WideChar));
  iter += DeeWideString_SIZE(appname);
  *iter++ = '"';
  *iter++ = ' ';
  memcpy(iter,DeeWideString_STR(cmdargs),DeeWideString_SIZE(cmdargs)*sizeof(Dee_WideChar));
 } else {
  Dee_Utf8Char *iter;
  if (DeeUtf8String_InplaceCast((DeeObject const **)&appname) != 0 ||
      DeeUtf8String_InplaceCast((DeeObject const **)&cmdargs) != 0 ||
     (usedcwd && DeeUtf8String_InplaceCast((DeeObject const **)&usedcwd) != 0) ||
      (cmdline = DeeUtf8String_NewSized(
       DeeUtf8String_SIZE(appname)+3+
       DeeUtf8String_SIZE(cmdargs))) == NULL
      ) goto err_cmdlinegen;
  iter = DeeUtf8String_STR(cmdline);
  *iter++ = '"';
  memcpy(iter,DeeUtf8String_STR(appname),DeeUtf8String_SIZE(appname)*sizeof(Dee_Utf8Char));
  iter += DeeUtf8String_SIZE(appname);
  *iter++ = '"';
  *iter++ = ' ';
  memcpy(iter,DeeUtf8String_STR(cmdargs),DeeUtf8String_SIZE(cmdargs)*sizeof(Dee_Utf8Char));
 }
 Dee_DECREF(cmdargs);

 // Begin blocking the process for real!
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  Dee_XDECREF(usedcwd);
  Dee_DECREF(cmdline);
  Dee_DECREF(appname);
  return 1; // Already started
 }
 memset(&info,0,sizeof(STARTUPINFOA));
 info.cb = sizeof(STARTUPINFOA);
 info.dwFlags = STARTF_USESTDHANDLES;
#define IMPORT_HANDLE(dst,src,default)\
do{\
 /* TODO: Throw an error if the source fd was closed. */\
 if ((src) && DeeFileFD_Win32AcquireHandle((DeeObject *)(src),&(dst)) == 0) {\
  if (!SetHandleInformation(dst,HANDLE_FLAG_INHERIT,1)) goto err_hinfo;\
  /* TODO: We should probably keep the tickets until the process has started. */\
  DeeFileFD_Win32ReleaseHandle((DeeObject *)(src));\
 } else {\
  if (!DuplicateHandle(\
   GetCurrentProcess(),GetStdHandle(default),\
   GetCurrentProcess(),&(dst),0,TRUE,DUPLICATE_SAME_ACCESS)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
    "DuplicateHandle(GetStdHandle(" #default ") = %p) : %K",\
    GetStdHandle(default),DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   /* TODO: Close handles already opened. */\
   goto err_ulock_files;\
  }\
 }\
}while(0)
 IMPORT_HANDLE(info.hStdInput,_self->p_stdin,STD_INPUT_HANDLE);
 IMPORT_HANDLE(info.hStdOutput,_self->p_stdout,STD_OUTPUT_HANDLE);
 IMPORT_HANDLE(info.hStdError,_self->p_stderr,STD_ERROR_HANDLE);
#undef IMPORT_HANDLE
 if (_self->p_environ) {
  if (DeeWideString_Check(appname)) {
   if ((env_strings = _DeeProcess_WideWin32GenerateEnvironmentString(
    (DeeListObject *)_self->p_environ)) == NULL) goto err_ulock_files;
  } else {
   if ((env_strings = _DeeProcess_Utf8Win32GenerateEnvironmentString(
    (DeeListObject *)_self->p_environ)) == NULL) goto err_ulock_files;
  }
 } else env_strings = NULL;
 DEE_LVERBOSE1("Starting process: %r - %r\n",_self->p_exe,_self->p_args);
 if (DeeWideString_Check(appname)) {
  DEE_ASSERT(DeeWideString_Check(cmdline));
  DEE_ASSERT(!env_strings || DeeWideString_Check(env_strings));
  DEE_ASSERT(!usedcwd || DeeWideString_Check(usedcwd));
  temp = CreateProcessW(
   NULL/*DeeWideString_STR(appname)*/,DeeWideString_STR(cmdline),NULL,NULL,TRUE,
   _DeeProcess_Win32FloatToPriority(_self->p_priority),
   env_strings ? DeeWideString_STR(env_strings) : NULL,
   usedcwd ? DeeWideString_STR(usedcwd) : NULL,&info,&pinfo);
 } else {
  DEE_ASSERT(DeeUtf8String_Check(cmdline));
  DEE_ASSERT(!env_strings || DeeUtf8String_Check(env_strings));
  DEE_ASSERT(!usedcwd || DeeUtf8String_Check(usedcwd));
  temp = CreateProcessA(
   NULL/*DeeUtf8String_STR(appname)*/,DeeUtf8String_STR(cmdline),NULL,NULL,TRUE,
   _DeeProcess_Win32FloatToPriority(_self->p_priority),
   env_strings ? DeeUtf8String_STR(env_strings) : NULL,
   usedcwd ? DeeUtf8String_STR(usedcwd) : NULL,
   (LPSTARTUPINFOA)&info,&pinfo);
 }
 Dee_XDECREF(env_strings);
 if (!temp) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateProcess%s(" /*%r*/ "NULL,%r,NULL,NULL,%d,%lu,%K,%K) : %K",
                      DeeUtf8String_Check(appname) ? "A" : "W",
                      /*appname,*/cmdline,(info.dwFlags&STARTF_USESTDHANDLES)!=0,
                      _DeeProcess_Win32FloatToPriority(_self->p_priority),
                      env_strings ? DeeString_Repr(env_strings) : DeeString_NEW("NULL"),
                      usedcwd ? DeeString_Repr(usedcwd) : DeeString_NEW("NULL"),
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  goto err_ulock_files;
 }
 _self ->p_handle = detached ? DEE_PROCESS_INVALID_HANDLE : pinfo.hProcess;
 DeeProcess_RELEASE(self);

 if (detached && !CloseHandle(pinfo.hProcess)) SetLastError(0);
 if (!CloseHandle(pinfo.hThread)) SetLastError(0);
 // Only decref temporaries after unlocking the process
 Dee_XDECREF(usedcwd);
 Dee_DECREF(cmdline);
 Dee_DECREF(appname);
 return 0;
err_hinfo:
 DeeError_SystemError("SetHandleInformation");
err_ulock_files:
 DeeProcess_RELEASE(self);
 Dee_XDECREF(usedcwd);
 Dee_DECREF(cmdline);
 Dee_DECREF(appname);
 return -1;
}
#elif defined(DEE_PLATFORM_UNIX)
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_Start(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN int detached) {
 DeeObject *used_exe,*used_args; char **argv,**envv; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) { DeeProcess_RELEASE(self); return 1; /* Already started */ }
 Dee_INCREF(used_exe = (DeeObject *)_self->p_exe);
 Dee_INCREF(used_args = (DeeObject *)_self->p_args);
 DeeProcess_RELEASE(self);
 if (DeeString_InplaceCast((DeeObject const **)&used_exe) != 0 ||
     DeeString_InplaceCast((DeeObject const **)&used_args) != 0
     ) { result = -1; goto end; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) { result = 1; goto end2; /* Already started */ }
 if (_DeeProcess_PosixSplitArgv(DeeString_STR(used_exe),
  DeeString_STR(used_args),&argv) != 0) { result = -1; goto end2; }
 if (_self->p_environ) {
  if (_DeeProcess_PosixSplitEnv(_self->p_environ,&envv) != 0) {
   _DeeProcess_PosixFreeStringList(argv);
   result = -1;
   goto end2;
  }
 } else envv = NULL;
 DEE_LVERBOSE1("Starting process: %q - %q\n",DeeString_STR(used_exe),DeeString_STR(used_args));
#ifdef DEE_DEBUG
 {char **argv_iter = argv;
  while (*argv_iter) {
   DEE_LVERBOSE1("ARGV[%Iu] %q\n",(argv_iter-argv),*argv_iter);
   ++argv_iter;
  }
 }
#endif
 // Lock all files to gain exclusive access to their handles
 if (_self->p_stdin)  DeeFileFD_ACQUIRE_SHARED(_self->p_stdin,{ DeeError_SetStringf(&DeeErrorType_IOError,"process.stdin was closed"); result = -1; goto end_afterstdin; });
 if (_self->p_stdout) DeeFileFD_ACQUIRE_SHARED(_self->p_stdout,{ DeeError_SetStringf(&DeeErrorType_IOError,"process.stdout was closed"); result = -1; goto end_afterstdout; });
 if (_self->p_stderr) DeeFileFD_ACQUIRE_SHARED(_self->p_stderr,{ DeeError_SetStringf(&DeeErrorType_IOError,"process.stderr was closed"); result = -1; goto end_afterstderr; });
 // TODO: Have another pipe that the child process can use to
 //       talk back if starting the process caused an error.
 result = _DeeProcess_PosixCreate(DeeString_STR(used_exe),argv,envv,
                                  _self->p_cwd ? DeeString_STR(_self->p_cwd) : NULL,
                                  _self->p_stdin ? ((DeeFileFDObject *)_self->p_stdin)->fd_descr.unx_fd : -1,
                                  _self->p_stdout ? ((DeeFileFDObject *)_self->p_stdout)->fd_descr.unx_fd : -1,
                                  _self->p_stderr ? ((DeeFileFDObject *)_self->p_stderr)->fd_descr.unx_fd : -1,
                                  &_self->p_handle);
 if (_self->p_stderr) DeeFileFD_RELEASE_SHARED(_self->p_stderr);
end_afterstderr:
 if (_self->p_stdout) DeeFileFD_RELEASE_SHARED(_self->p_stdout);
end_afterstdout:
 if (_self->p_stdin) DeeFileFD_RELEASE_SHARED(_self->p_stdin);
end_afterstdin:
 if (envv) _DeeProcess_PosixFreeStringList(envv);
 _DeeProcess_PosixFreeStringList(argv);
end2: DeeProcess_RELEASE(self);
end:
 Dee_DECREF(used_args);
 Dee_DECREF(used_exe);
 return result;
}
#endif


DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_Detach(
 DEE_A_IN_OBJECT(DeeProcessObject) *self) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (!DeeProcess_HANDLE(self)) return 1;
 if (!CloseHandle(DeeProcess_HANDLE(self))) {
  DeeError_SystemError("CloseHandle");
  return -1;
 }
 ((DeeProcessObject *)self)->p_handle = NULL;
 return 0;
#elif defined(DEE_PLATFORM_UNIX)
 (void)self;
 DeeError_TODONotImplemented(); // TODO
 return -1;
#else
 (void)self;
 DeeError_NotImplemented();
 return -1;
#endif
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_Terminate(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self,
 DEE_A_IN DeeProcessReturn exit_code) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_LVERBOSE1("Terminating process: %k\n",self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#if !DEE_CONFIG_RUNTIME_HAVE_EXIT
 if (DeeProcess_IS_SELF(self)) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "Can't terminate current process");
  return -1;
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DeeProcess_ACQUIRE(self);
  if (!TerminateProcess(DeeProcess_HANDLE(self),exit_code)) {
   DWORD error; DWORD pid; HANDLE new_handle;
   switch ((error = DeeSystemError_Win32Consume())) {
    case ERROR_ACCESS_DENIED: {
     // re-try after acquiring the missing right
     pid = GetProcessId(DeeProcess_HANDLE(self));
     new_handle = OpenProcess(PROCESS_TERMINATE,FALSE,pid);
     if (new_handle == NULL) {
      DeeProcess_RELEASE(self);
      DeeError_SetStringf(&DeeErrorType_SystemError,
                          "OpenProcess(%lu) : %K",pid,
                          DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
      return -1;
     }
     if (!TerminateProcess(new_handle,exit_code)) {
      error = DeeSystemError_Win32Consume();
      if (!CloseHandle(new_handle)) SetLastError(0);
      if (error == ERROR_ACCESS_DENIED) {
       DWORD exit_code;
       if (!GetExitCodeProcess(self,&exit_code)) {
        DeeProcess_RELEASE(self);
        DeeError_SetStringf(&DeeErrorType_SystemError,"GetExitCodeProcess(%Iu) : %K",
                            self,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
        return -1;
       }
       if (exit_code != STILL_ACTIVE) {
        // If the process was terminated, return 1
        DeeProcess_RELEASE(self);
        return 1;
       }
      }
      goto err;
     } else {
      if (!CloseHandle(new_handle)) SetLastError(0);
     }
    } break;

    default:err:
     DeeProcess_RELEASE(self);
     DeeError_SetStringf(&DeeErrorType_SystemError,
                         "TerminateProcess(%k) : %K",self,
                         DeeSystemError_Win32ToString(error));
     return -1;
   }
  }
  DeeProcess_RELEASE(self);
  return 0;
#elif defined(DEE_PLATFORM_UNIX)
  (void)self;
  DeeError_TODONotImplemented(); // TODO
  return -1;
#else
  (void)self;
  DeeError_NotImplemented();
  return -1;
#endif
 }
}



DEE_A_RET_EXCEPT(-1) int DeeProcess_DelCwd(
 DEE_A_IN_OBJECT(DeeProcessObject) *self) {
 DeeAnyStringObject *old_cwd;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFS_ChDir("");
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"cwd");
  return -1;
 }
 old_cwd = _self->p_cwd;
 _self->p_cwd = NULL;
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_cwd);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetCwd(
 DEE_A_IN_OBJECT(DeeProcessObject) *self,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFS_ChdirObject(cwd);
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't change cwd of running %k to %k",
                      self,cwd);
  return -1;
 }
 DeeProcess_RELEASE(self);
 if ((cwd = DeeFS_PathExpandObject(cwd)) == NULL) return -1;
 Dee_XDECREF(_self->p_cwd);
 Dee_INHERIT_REF(_self->p_cwd,*(DeeAnyStringObject **)&cwd);
 return 0;
}




DEE_A_RET_WUNUSED int DeeProcess_Started(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DeeProcess_ACQUIRE(self);
 result = DeeProcess_STARTED(self);
 DeeProcess_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeProcess_Terminated(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
#ifdef DEE_PLATFORM_WINDOWS
 DWORD exit_code; BOOL success;
 DeeProcess_ACQUIRE(self);
 success = GetExitCodeProcess(DeeProcess_HANDLE(self),&exit_code);
 DeeProcess_RELEASE(self);
 if (!success) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetExitCodeProcess(%k) : %K",
                      self,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return exit_code != STILL_ACTIVE;
#elif defined(DEE_PLATFORM_UNIX)
 (void)self;
 DeeError_TODONotImplemented(); // TODO
 return -1;
#else
 (void)self;
 DeeError_NotImplemented();
 return -1;
#endif
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6103)
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeProcess_Join(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_OUT_OPT DeeProcessReturn *retval) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeProcess_JoinTimed(self,INFINITE,retval);
#elif defined(DEE_PLATFORM_UNIX)
 int temp; pid_t error;
 DEE_LVERBOSE1("Joining process: %k\n",self);
#ifdef WIFCONTINUED
again:
#endif /* WIFCONTINUED */
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 error = waitpid(DeeProcess_HANDLE(self),&temp,0);
 DeeProcess_RELEASE(self);
 if (error == (pid_t)-1) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waitpid(%k) : %K",self,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 } else if (WIFEXITED(temp)) {
  if (retval) *retval = WEXITSTATUS(temp);
 } else if (WIFSIGNALED(temp)) {
  if (retval) *retval = WCOREDUMP(temp) ? 0 : 1;
 } else if (WIFSTOPPED(temp)) {
  if (retval) *retval = 0;
#ifdef WIFCONTINUED
 } else if (WIFCONTINUED(temp)) {
  goto again;
#endif /* WIFCONTINUED */
 }
 return 0;
#else
 (void)self;
 DeeError_NotImplemented();
 return -1;
#endif
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeProcess_JoinTimedImpl(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN unsigned int msecs,
 DEE_A_OUT_OPT DeeProcessReturn *retval) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 {
#ifdef DEE_PLATFORM_WINDOWS
  DEE_LVERBOSE1("Joining process (timed: %u): %k\n",msecs,self);
  DeeProcess_ACQUIRE(self);
  switch (WaitForSingleObject(DeeProcess_HANDLE(self),msecs)) {
   case WAIT_ABANDONED:
   case WAIT_OBJECT_0:
    if (retval && !GetExitCodeProcess(DeeProcess_HANDLE(self),(LPDWORD)retval)) {
     DeeProcess_RELEASE(self);
     DeeError_SystemError("GetExitCodeProcess");
     return -1;
    }
    if (!CloseHandle(DeeProcess_HANDLE(self))) SetLastError(0);
    _self->p_handle = NULL;
    DeeProcess_RELEASE(self);
    return 0;
   case WAIT_TIMEOUT: 
    DeeProcess_RELEASE(self);
    return 1;
   default: break;
  }
  DeeProcess_RELEASE(self);
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "WaitForSingleObject(%k,%u) : %K",self,msecs,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
#elif defined(DEE_PLATFORM_UNIX)
  int temp; pid_t error;
  DEE_LVERBOSE1("Joining process (timed: %u): %k\n",msecs,self);
#if DEE_HAVE_UALARM
  ualarm(msecs,0);
#else
  alarm(msecs/1000);
#endif
  DeeProcess_ACQUIRE(self);
  error = waitpid(DeeProcess_HANDLE(self),&temp,0);
  DeeProcess_RELEASE(self);
  if (error == (pid_t)-1) {
   temp = DeeSystemError_Consume();
#ifdef SIGALRM
   if (temp == SIGALRM) return 1; // timeout
#endif
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "waitpid(%k) : %K",self,
                       DeeSystemError_ToString(temp));
   return -1;
  } else if (WIFEXITED(temp)) {
   if (retval) *retval = WEXITSTATUS(temp);
  } else if (WIFSIGNALED(temp)) {
   if (retval) *retval = WCOREDUMP(temp) ? 0 : 1;
  } else if (WIFSTOPPED(temp)) {
   if (retval) *retval = 0;
  } else if (WIFCONTINUED(temp)) {
   return 1; // not really a timeout, but close enough...
  }
  return 0;
#else
  (void)self,msecs;
  DeeError_NotImplemented();
  return -1;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_TryJoin(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_OUT_OPT DeeProcessReturn *retval) {
 return _DeeProcess_JoinTimedImpl(self,0,retval);
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeProcess_JoinTimed(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN unsigned int msecs,
 DEE_A_OUT_OPT DeeProcessReturn *retval) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 return _DeeProcess_JoinTimedImpl(self,msecs,retval);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeProcess_GetStdin(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFile_Std(DEE_STDIN);
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return NULL; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self) || !_self->p_stdin) {
  DeeProcess_RELEASE(self);
  DeeError_SET_STRING(&DeeErrorType_AttributeError,"Missing attribute: stdin");
  return NULL;
 }
 Dee_INCREF(result = (DeeObject *)_self->p_stdin);
 DeeProcess_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeProcess_GetStdout(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFile_Std(DEE_STDOUT);
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return NULL; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self) || !_self->p_stdout) {
  DeeProcess_RELEASE(self);
  DeeError_SET_STRING(&DeeErrorType_AttributeError,"Missing attribute: stdout");
  return NULL;
 }
 Dee_INCREF(result = (DeeObject *)_self->p_stdout);
 DeeProcess_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeProcess_GetStderr(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFile_Std(DEE_STDERR);
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return NULL; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self) || !_self->p_stderr) {
  DeeProcess_RELEASE(self);
  DeeError_SET_STRING(&DeeErrorType_AttributeError,"Missing attribute: stderr");
  return NULL;
 }
 Dee_INCREF(result = (DeeObject *)_self->p_stderr);
 DeeProcess_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetStdin(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *fp) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_ASSERT(DeeObject_Check(fp));
 if (DeeProcess_IS_SELF(self)) { if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return -1; DeeFile_SetStd(DEE_STDIN,fp); return 0; }
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFileFD_Type) != 0) return -1;
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Setting process handle : stdin : %k -> %k\n",self,fp);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stdin");
  return -1;
 }
 old_file = _self->p_stdin;
 Dee_INCREF(_self->p_stdin = (DeeFileFDObject *)fp);
 Dee_XDECREF(old_file);
 DeeProcess_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetStdout(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *fp) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_ASSERT(DeeObject_Check(fp));
 if (DeeProcess_IS_SELF(self)) { if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return -1; DeeFile_SetStd(DEE_STDOUT,fp); return 0; }
 if (!DeeFileIO_Check(fp) && DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeePipe_Type) != 0) return -1;
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Setting process handle : stdout : %k -> %k\n",self,fp);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stdout");
  return -1;
 }
 old_file = _self->p_stdout;
 Dee_INCREF(_self->p_stdout = (DeeFileFDObject *)fp);
 Dee_XDECREF(old_file);
 DeeProcess_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetStderr(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *fp) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_ASSERT(DeeObject_Check(fp));
 if (DeeProcess_IS_SELF(self)) { if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return -1; DeeFile_SetStd(DEE_STDERR,fp); return 0; }
 if (!DeeFileIO_Check(fp) && DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeePipe_Type) != 0) return -1;
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Setting process handle : stderr : %k -> %k\n",self,fp);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stderr");
  return -1;
 }
 old_file = _self->p_stderr;
 Dee_INCREF(_self->p_stderr = (DeeFileFDObject *)fp);
 Dee_XDECREF(old_file);
 DeeProcess_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_DelStdin(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) { DeeFile_DelStd(DEE_STDIN); return 0; }
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Deleting process handle : stdin : %k -> NULL\n",self);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stdin");
  return -1;
 }
 old_file = _self->p_stdin;
 _self->p_stdin = NULL;
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_file);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_DelStdout(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) { DeeFile_DelStd(DEE_STDOUT); return 0; }
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Deleting process handle : stdout : %k -> NULL\n",self);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stdout");
  return -1;
 }
 old_file = _self->p_stdout;
 _self->p_stdout = NULL;
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_file);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_DelStderr(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self) {
 DeeFileFDObject *old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) { DeeFile_DelStd(DEE_STDERR); return 0; }
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DEE_LVERBOSE2("Deleting process handle : stderr : %k -> NULL\n",self);
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"stderr");
  return -1;
 }
 old_file = _self->p_stderr;
 _self->p_stderr = NULL;
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_file);
 return 0;
}

DEE_A_RET_EXCEPT(-1) double DeeProcess_GetPriority(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 double result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_LVERBOSE2("Getting process priority: %k\n",self);
 if (DeeProcess_IS_REF(self)) {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD prioclass;
  DeeProcess_ACQUIRE(self);
get_as_ref:
  prioclass = GetPriorityClass(DeeProcess_HANDLE(self));
  DeeProcess_RELEASE(self);
  switch (prioclass) {
  // 0.00 -- 0.20   IDLE_PRIORITY_CLASS
  // 0.20 -- 0.40   BELOW_NORMAL_PRIORITY_CLASS
  // 0.40 -- 0.60   NORMAL_PRIORITY_CLASS
  // 0.60 -- 0.80   ABOVE_NORMAL_PRIORITY_CLASS
  // 0.80 -- 1.00   HIGH_PRIORITY_CLASS
   case IDLE_PRIORITY_CLASS:         return 0.1;
   case BELOW_NORMAL_PRIORITY_CLASS: return 0.3;
   case NORMAL_PRIORITY_CLASS:       return 0.5;
   case ABOVE_NORMAL_PRIORITY_CLASS: return 0.7;
   case HIGH_PRIORITY_CLASS:         return 0.9;
   case REALTIME_PRIORITY_CLASS:     return 1.0;
   case 0:
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "GetPriorityClass(%k) : %K",self,
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
    return -1.0;
   default: break;
  }
  return 0.5;
#elif defined(DEE_PLATFORM_UNIX)
  int result;
  DeeProcess_ACQUIRE(self);
get_as_ref:
  errno = 0;
  result = getpriority(PRIO_PROCESS,DeeProcess_HANDLE(self));
  DeeProcess_RELEASE(self);
  if (result == -1) {
   result = DeeSystemError_Consume();
   if (result == 0) return 0.5;
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "getpriority(PRIO_PROCESS,%k) : %K",
                       self,DeeSystemError_ToString(result));
   return -1.0;
  }
  return POSIX_PROCESS_FLOAT_TO_PRIORITY(result);
#else
  if (0) {get_as_ref: DeeProcess_RELEASE(self); }
  return 0.5;
#endif
 }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) goto get_as_ref;
 result = _self->p_priority;
 DeeProcess_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetPriority(
 DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_IN double priority) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (priority < 0.0) priority = 0.0;
 else if (priority > 1.0) priority = 1.0;
 DEE_LVERBOSE2("Setting process priority: %k -> %f\n",self,priority);
 if (DeeProcess_IS_REF(self)) {
#ifdef DEE_PLATFORM_WINDOWS
  BOOL success;
  DeeProcess_ACQUIRE(self);
set_as_ref:
  success = SetPriorityClass(self,_DeeProcess_Win32FloatToPriority(priority));
  DeeProcess_RELEASE(self);
  if (!success) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetPriorityClass(%k,%lu) : %K",
                       self,_DeeProcess_Win32FloatToPriority(priority),
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
  return 0;
#elif defined(DEE_PLATFORM_UNIX)
  int error;
  DeeProcess_ACQUIRE(self);
set_as_ref:
  error = setpriority(PRIO_PROCESS,
                      DeeProcess_HANDLE(self),
                      POSIX_PROCESS_FLOAT_TO_PRIORITY(priority));
  DeeProcess_RELEASE(self);
  if (error == -1) {
   DeeError_SetStringf(&DeeErrorType_SystemError,"setpriority("
                       DEE_TYPES_IUPRINTF(DEE_TYPES_SIZEOF_PID_T) ",%d) : %K",
                       self,POSIX_PROCESS_FLOAT_TO_PRIORITY(priority),
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  if (0) {set_as_ref: DeeProcess_RELEASE(self); }
  DeeError_NotImplemented_str("DeeProcess_SetPriority()");
  return -1;
#endif
 }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) goto set_as_ref;
 _self->p_priority = priority;
 DeeProcess_RELEASE(self);
 return 0;
}





DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeProcess_GetEnviron(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) return DeeFS_EnumEnv();
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return NULL; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't read cwd of %k",self);
  return NULL;
 } 
 Dee_XINCREF(result = (DeeObject *)_self->p_environ);
 DeeProcess_RELEASE(self);
 return result ? result : DeeFS_EnumEnv();
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_SetEnviron(
 DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_INOUT_OBJECT(DeeListObject) *env) {
 DeeListObject *old_env;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DEE_ASSERT(DeeObject_Check(env) && DeeList_Check(env));
 if (DeeProcess_IS_SELF(self)) return DeeFS_SetListEnv(env);
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 if ((env = DeeList_Copy(env)) == NULL) return -1;
#ifdef DEE_DEBUG
 {DeeObject **iter,**end;
  for(end = (iter = DeeList_ELEM(env))+DeeList_SIZE(env); iter!=end; ++iter) {
   DEE_ASSERT(DeeObject_Check(*iter) && DeeTuple_Check(*iter));
   DEE_ASSERT(DeeTuple_SIZE(*iter) == 2);
   DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,0)));
   DEE_ASSERT(DeeObject_Check(DeeTuple_GET(*iter,1)));
  }
 }
#endif
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't set environ of %k to %k",
                      self,env);
  Dee_DECREF(env);
  return -1;
 }
 old_env = _self->p_environ;
 Dee_INHERIT_REF(_self->p_environ,*(DeeListObject **)&env);
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_env);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeProcess_DelEnviron(
 DEE_A_IN_OBJECT(DeeProcessObject) *self) {
 DeeListObject *old_environ; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_IS_SELF(self)) {
  if ((old_environ = (DeeListObject *)DeeList_NewEmpty()) == NULL) return -1;
  result = DeeFS_SetListEnv((DeeObject *)old_environ);
  Dee_DECREF(old_environ);
  return result;
 }
 if (DeeProcess_IS_REF(self)) { _deeprocess_error_is_ref(self); return -1; }
 DeeProcess_ACQUIRE(self);
 if (DeeProcess_STARTED(self)) {
  DeeProcess_RELEASE(self);
  _deeprocess_error_cant_change_running((DeeProcessObject *)self,"environ");
  return -1;
 }
 old_environ = _self->p_environ;
 _self->p_environ = NULL;
 DeeProcess_RELEASE(self);
 Dee_XDECREF(old_environ);
 return 0;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeProcess_Enumerate(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result,*proc,**dst; Dee_size_t noaccess;
 DWORD *ids,*ids_iter,*ids_end; DWORD nids,nids_max,error;
 typedef BOOL (WINAPI *LPENUMPROCESSES)(DWORD *lpidProcess, DWORD cb, LPDWORD lpcbNeeded);
 static LPENUMPROCESSES p_EnumProcesses = NULL;
 _deewin32_acquire_se_debug_privilege();
 DEE_ATOMIC_ONCE({
  HMODULE hKernel32 = GetModuleHandle(TEXT("KERNEL32"));
  p_EnumProcesses = hKernel32 ? (LPENUMPROCESSES)
   GetProcAddress(hKernel32,"K32EnumProcesses") : NULL;
DEE_COMPILER_MSVC_WARNING_PUSH(4055)
  if (!p_EnumProcesses) p_EnumProcesses = (LPENUMPROCESSES)
   DeeSharedLib_PoolTryImport("Psapi.dll","EnumProcesses");
DEE_COMPILER_MSVC_WARNING_POP
 });
 if (!p_EnumProcesses) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,"Not implemented: EnumProcesses");
  return NULL;
 }
 while ((ids = (DWORD *)malloc_nz((nids_max = 256)*sizeof(DWORD))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
  DeeError_NoMemory();
  return NULL;
 }
 while (1) {
  if (!(*p_EnumProcesses)(ids,nids_max*sizeof(DWORD),&nids)) { DeeError_SystemError("EnumProcesses"); free_nn(ids); return NULL; }
  if ((nids /= sizeof(DWORD)) != nids_max) break;
  while ((ids_iter = (DWORD *)realloc_nnz(
   ids,(nids_max *= 2)*sizeof(DWORD))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   free_nn(ids);
   goto no_mem;
  }
  ids = ids_iter;
 }
 if (!nids) {
  // No processes? ...OK?
  free_nn(ids);
  DeeReturn_EmptyList;
 }
 // Remove 0-entries
 ids_end = (ids_iter = ids)+(Dee_size_t)nids;
 while (ids_iter != ids_end) {
  if (!*ids_iter) memmove(ids_iter,ids_iter+1,(Dee_size_t)(
   (ids_end-ids_iter)-1)*sizeof(DWORD)),--nids;
  ++ids_iter;
 }
 // All process ids collected
 if ((result = _DeeList_NewUnsafe((Dee_size_t)nids)) == NULL) {err_0: free_nn(ids); return NULL; }
 ids_end = (ids_iter = ids)+(Dee_size_t)nids,dst = DeeList_ELEM(result),noaccess = 0;
 while (ids_iter != ids_end) {
  HANDLE process_handle; // Open the process as a handle (use the lowest possible access right)
  if ((process_handle = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,*ids_iter)) == NULL) {
   switch ((error = DeeSystemError_Win32Consume())) {
    case 0:
    case ERROR_ACCESS_DENIED:
     ++noaccess;
     goto ignore;
    default:
     DeeError_SetStringf(&DeeErrorType_SystemError,
                         "OpenProcess(%lu) : %K",
                         *ids_iter,DeeSystemError_Win32ToString(error));
     break;
   }
err_1: while (dst != DeeList_ELEM(result)) Dee_DECREF(*--dst);
   _DeeList_FreeUnsafe(result);
   goto err_0;
  }
  if DEE_UNLIKELY((proc = _DeeProcess_NewFromHandle(process_handle)) == NULL) {
   if DEE_UNLIKELY(!CloseHandle(process_handle)) SetLastError(0);
   goto err_1;
  }
  Dee_INHERIT_REF(*dst,proc);
  ++dst;
ignore:
  ++ids_iter;
 }
 free_nn(ids);
 // Remove entires for processes that we couldn't access
 if (noaccess && DEE_UNLIKELY(_DeeList_ResizeUnsafe(
  result,DeeList_SIZE(result)-noaccess) != 0)) goto err_1;
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 // >> #include <fs>
 // >> function process::enum() -> [[] {
 // >>   for (x: fs::dir("/proc/")) if (x.isdigit()) yield (pid_t)x;
 // >> }()...];
 DeeObject *result,*entry;
 DIR *dir; struct dirent *iter; int error;
 DEE_LVERBOSE1("Enumerating processes...\n");
 if ((dir = opendir("/proc/")) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "opendir(\"/proc/\") : %K",
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) goto end;
 while ((iter = readdir(dir)) != NULL) {
  char const *name = iter->d_name;
  DeeProcessHandle handle;
  if (!name || !*name) continue;
  handle = 0;
  while (DEE_CH_IS_DIGIT(*name)) handle = handle*10+((*name++)-'0');
  if (!*name) {
   DEE_LVERBOSE2("Found process: " DEE_TYPES_IUPRINTF(DEE_TYPES_SIZEOF_DEE_PROCESS_HANDLE) "\n",handle);
   if ((entry = _DeeProcess_NewFromHandle(handle)) == NULL) goto err_r;
   error = DeeList_Append(result,entry);
   Dee_DECREF(entry);
   if (error != 0) goto err_r;
  }
 }
 if ((error = errno) != 0) { // error 0 means end-of-directory
  errno = 0;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "readdir(opendir(\"/proc/\")) : %K",
                      DeeSystemError_ToString(error));
err_r: Dee_CLEAR(result);
 }
end: if (closedir(dir) != 0) errno = 0;
 return result;
#else
 DeeReturn_EmptyList;
#endif
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeProcess_Win32Times(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **creation_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **exit_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **kernel_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **user_time) {
 FILETIME tc,te,tk,tu; BOOL success;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 success = GetProcessTimes(DeeProcess_HANDLE(self),&tc,&te,&tk,&tu);
 DeeProcess_RELEASE(self);
 if (!success) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetProcessTimes(%k) : %K",self,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 if (creation_time && (*creation_time = DeeTime_NewFromWin32FileTime(&tc)) == NULL) return -1;
 if (exit_time && (*exit_time = DeeTime_NewFromWin32FileTime(&te)) == NULL) goto err_0;
 if (kernel_time && (*kernel_time = DeeTime_NewFromWin32FileTime0(&tk)) == NULL) goto err_1;
 if (user_time && (*user_time = DeeTime_NewFromWin32FileTime0(&tu)) == NULL) goto err_2;
 return 0;
err_2: if (kernel_time) Dee_DECREF(*kernel_time);
err_1: if (exit_time) Dee_DECREF(*exit_time);
err_0: if (creation_time) Dee_DECREF(*creation_time);
 return -1;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeProcess_Win32IsCritical(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 BOOL result,success;
 typedef BOOL (WINAPI *LPISPROCESSCRITICAL)(HANDLE hProcess, PBOOL Critical);
 static LPISPROCESSCRITICAL p_IsProcessCritical = NULL;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DEE_ATOMIC_ONCE({
  HMODULE hKernel32 = GetModuleHandle(TEXT("KERNEL32"));
  p_IsProcessCritical = hKernel32 ? (LPISPROCESSCRITICAL)
   GetProcAddress(hKernel32,"IsProcessCritical") : NULL;
 });
 if (!p_IsProcessCritical) return 0;
 DeeProcess_ACQUIRE(self);
 success = (*p_IsProcessCritical)(DeeProcess_HANDLE(self),&result);
 DeeProcess_RELEASE(self);
 if (!success) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"IsProcessCritical(%k) : %K",
                      self,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return result;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeProcess_Win32GetPriorityBoost(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 BOOL result,success;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (!DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 success = GetProcessPriorityBoost(DeeProcess_HANDLE(self),&result);
 DeeProcess_RELEASE(self);
 if (!success) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetProcessPriorityBoost(%k) : %K",
                      self,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return !result;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeProcess_Win32SetPriorityBoost(
 DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_IN int enabled) {
 BOOL success;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeProcess_ACQUIRE(self);
 success = SetProcessPriorityBoost(DeeProcess_HANDLE(self),enabled ? FALSE : TRUE);
 DeeProcess_RELEASE(self);
 if (!success) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"SetProcessPriorityBoost(%k,%s) : %K",
                      self,enabled ? "FALSE" : "TRUE",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
#endif

#undef _self







static DeeObject *DEE_CALL _deeprocess_tp_any_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args) {
 DeeObject *exe,*cmdargs = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:process",&exe,&cmdargs) != 0) return NULL;
 if (DeeNone_Check(cmdargs)) return DeeProcess_NewObject(exe);
 if (!DeeAnyString_Check(cmdargs)) {
  // Join the arguments: 'process("deemon",["-F","format_file.dee"])'
  DeeWideString_NEW_STATIC_EX(_space_wide,1,{' '});
  DeeUtf8String_NEW_STATIC_EX(_space_utf8,1,{' '});
  if ((cmdargs = (DeeWideString_Check(exe)
   ? DeeWideString_Join((DeeObject *)&_space_wide,cmdargs)
   : DeeUtf8String_Join((DeeObject *)&_space_utf8,cmdargs)
   )) == NULL) return NULL;
  exe = DeeProcess_NewExObject(exe,cmdargs);
  Dee_DECREF(cmdargs);
  return exe;
 }
 return DeeProcess_NewExObject(exe,cmdargs);
}
static void DEE_CALL _deeprocess_tp_dtor(DeeProcessObject *self) {
 if (!DeeProcess_IS_REF(self)) {
  Dee_DECREF(self->p_exe);
  Dee_DECREF(self->p_args);
  Dee_XDECREF(self->p_environ);
  Dee_XDECREF(self->p_stdin);
  Dee_XDECREF(self->p_stdout);
  Dee_XDECREF(self->p_stderr);
  Dee_XDECREF(self->p_cwd);
 }
#ifdef DEE_PLATFORM_WINDOWS
 if (DeeProcess_HANDLE(self) && !CloseHandle(DeeProcess_HANDLE(self)))
  SetLastError(0);
#endif
}
DEE_VISIT_PROC(_deeprocess_tp_visit,DeeProcessObject *self) {
 if (!DeeProcess_IS_REF(self)) {
  DeeProcess_ACQUIRE(self);
  Dee_VISIT(self->p_exe);
  Dee_VISIT(self->p_args);
  Dee_XVISIT(self->p_environ);
  Dee_XVISIT(self->p_stdin);
  Dee_XVISIT(self->p_stdout);
  Dee_XVISIT(self->p_stderr);
  Dee_XVISIT(self->p_cwd);
  DeeProcess_RELEASE(self);
 }
}
static DeeObject *DEE_CALL _deeprocess_tp_str(DeeProcessObject *self) {
 return DeeString_Newf("<process(%R) -> " DEE_TYPES_IUPRINTF(DEE_TYPES_SIZEOF_DEE_PROCESS_ID) ">",
                       DeeProcess_Cmd((DeeObject *)self),DeeProcess_ID((DeeObject *)self));
}
static DeeObject *DEE_CALL _deeprocess_tp_cmp_lo(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) <  DeeProcess_ID((DeeObject *)right)); }
static DeeObject *DEE_CALL _deeprocess_tp_cmp_le(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) <= DeeProcess_ID((DeeObject *)right)); }
static DeeObject *DEE_CALL _deeprocess_tp_cmp_eq(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) == DeeProcess_ID((DeeObject *)right)); }
static DeeObject *DEE_CALL _deeprocess_tp_cmp_ne(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) != DeeProcess_ID((DeeObject *)right)); }
static DeeObject *DEE_CALL _deeprocess_tp_cmp_gr(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) >  DeeProcess_ID((DeeObject *)right)); }
static DeeObject *DEE_CALL _deeprocess_tp_cmp_ge(DeeProcessObject *self, DeeProcessObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeProcess_Type) != 0) return NULL; DeeReturn_Bool(DeeProcess_ID((DeeObject *)self) >= DeeProcess_ID((DeeObject *)right)); }




static DeeObject *DEE_CALL _deeprocess_start(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result,detached = 0;
 if (DeeTuple_Unpack(args,"|b:start",&detached) != 0) return NULL;
 if ((result = DeeProcess_Start((DeeObject *)self,detached)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeprocess_detach(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if (DeeTuple_Unpack(args,":detach") != 0) return NULL;
 if ((result = DeeProcess_Detach((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeprocess_join(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeProcessReturn result;
 if (DeeTuple_Unpack(args,":join") != 0) return NULL;
 if (DeeProcess_Join((DeeObject *)self,&result) != 0) return NULL;
 return DeeObject_New(DeeProcessReturn,result);
}
static DeeObject *DEE_CALL _deeprocess_try_join(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int success; DeeProcessReturn retval; DeeObject *retval_ob,*result;
 if (DeeTuple_Unpack(args,":try_join") != 0) return NULL;
 if ((success = DeeProcess_TryJoin((DeeObject *)self,&retval)) < 0) return NULL;
 if (success) retval_ob = Dee_None; else if ((retval_ob = DeeObject_New(DeeProcessReturn,retval)) == NULL) return NULL;
 // We return a 2-elemental tuple: (success,retval)
 result = DeeTuple_Pack(2,success == 0 ? Dee_True : Dee_False,retval_ob);
 if (!success) Dee_DECREF(retval_ob);
 return result;
}
static DeeObject *DEE_CALL _deeprocess_join_timed(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int msecs; int success; DeeProcessReturn retval; DeeObject *retval_ob,*result;
 if (DeeTuple_Unpack(args,"u:join_timed",&msecs) != 0) return NULL;
 if ((success = DeeProcess_JoinTimed((DeeObject *)self,msecs,&retval)) < 0) return NULL;
 if (success) retval_ob = Dee_None; else if ((retval_ob = DeeObject_New(DeeProcessReturn,retval)) == NULL) return NULL;
 // We return a 2-elemental tuple: (success,retval)
 result = DeeTuple_Pack(2,success == 0 ? Dee_True : Dee_False,retval_ob);
 if (!success) Dee_DECREF(retval_ob);
 return result;
}
static DeeObject *DEE_CALL _deeprocess_id(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":id") != 0) return NULL;
 return DeeObject_New(DeeProcessID,DeeProcess_ID((DeeObject *)self));
}
static DeeObject *DEE_CALL _deeprocess_terminate(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result; DeeProcessReturn exit_code = 0;
 if (DeeTuple_Unpack(args,"|I32u:terminate",&exit_code) != 0) return NULL;
 if ((result = DeeProcess_Terminate((DeeObject *)self,exit_code)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeprocess_started(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":started") != 0) return NULL;
 DeeReturn_Bool(DeeProcess_STARTED(self));
}
static DeeObject *DEE_CALL _deeprocess_detached(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":detached") != 0) return NULL;
 DeeReturn_Bool(DeeProcess_DETACHED(self));
}
static DeeObject *DEE_CALL _deeprocess_terminated(
 DeeProcessObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if (DeeTuple_Unpack(args,":terminated") != 0) return NULL;
 if ((result = DeeProcess_Terminated((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
#ifdef DEE_PLATFORM_WINDOWS
static DeeObject *DEE_CALL _deeprocess_win32_times(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *ct,*et,*kt,*ut,*result;
 if (DeeTuple_Unpack(args,":win32_times") != 0) return NULL;
 if (DeeProcess_Win32Times(self,&ct,&et,&kt,&ut) != 0) return NULL;
 result = DeeTuple_Pack(4,ct,et,kt,ut);
 Dee_DECREF(ct);
 Dee_DECREF(et);
 Dee_DECREF(kt);
 Dee_DECREF(ut);
 return result;
}
static DeeObject *DEE_CALL _deeprocess_win32_creation_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":win32_creation_time") != 0) return NULL;
 if (DeeProcess_Win32Times(self,&result,NULL,NULL,NULL) != 0) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deeprocess_win32_exit_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":win32_exit_time") != 0) return NULL;
 if (DeeProcess_Win32Times(self,NULL,&result,NULL,NULL) != 0) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deeprocess_win32_kernel_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":win32_kernel_time") != 0) return NULL;
 if (DeeProcess_Win32Times(self,NULL,NULL,&result,NULL) != 0) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deeprocess_win32_user_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":win32_user_time") != 0) return NULL;
 if (DeeProcess_Win32Times(self,NULL,NULL,NULL,&result) != 0) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deeprocess_win32_is_critical(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if (DeeTuple_Unpack(args,":win32_is_critical") != 0) return NULL;
 if ((result = DeeProcess_Win32IsCritical(self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
#endif
static DeeObject *DEE_CALL _deeprocess_exe_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_Utf8Exe((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_wexe_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_WideExe((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_cmd_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_Utf8Cmd((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_wcmd_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_WideCmd((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_args_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_Utf8Args((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_wargs_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_WideArgs((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_argv_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_Utf8Argv((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_wargv_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_WideArgv((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_cwd_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_Utf8Cwd((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_wcwd_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_WideCwd((DeeObject *)self); }
static int DEE_CALL _deeprocess_cwd_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_DelCwd((DeeObject *)self); }
static int DEE_CALL _deeprocess_cwd_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { return DeeProcess_SetCwd((DeeObject *)self,v); }
static DeeObject *DEE_CALL _deeprocess_stdin_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_GetStdin((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_stdout_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_GetStdout((DeeObject *)self); }
static DeeObject *DEE_CALL _deeprocess_stderr_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_GetStderr((DeeObject *)self); }
static int DEE_CALL _deeprocess_stdin_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_DelStdin((DeeObject *)self); }
static int DEE_CALL _deeprocess_stdout_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_DelStdout((DeeObject *)self); }
static int DEE_CALL _deeprocess_stderr_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_DelStderr((DeeObject *)self); }
static int DEE_CALL _deeprocess_stdin_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { return DeeProcess_SetStdin((DeeObject *)self,v); }
static int DEE_CALL _deeprocess_stdout_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { return DeeProcess_SetStdout((DeeObject *)self,v); }
static int DEE_CALL _deeprocess_stderr_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { return DeeProcess_SetStderr((DeeObject *)self,v); }
static DeeObject *DEE_CALL _deeprocess_priority_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { double result; if ((result = DeeProcess_GetPriority((DeeObject *)self)) == -1.0) return NULL; return DeeObject_New(double,result); }
static int DEE_CALL _deeprocess_priority_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_SetPriority((DeeObject *)self,0.5); }
static int DEE_CALL _deeprocess_priority_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { double val; if (DeeObject_Cast(double,v,&val) != 0) return -1; return DeeProcess_SetPriority((DeeObject *)self,val); }
static DeeObject *DEE_CALL _deeprocess_env_get(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_GetEnviron((DeeObject *)self); }
static int DEE_CALL _deeprocess_env_del(DeeProcessObject *self, void *DEE_UNUSED(closure)) { return DeeProcess_DelEnviron((DeeObject *)self); }
static int DEE_CALL _deeprocess_env_set(DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *elem;
 if (DeeObject_InplaceGetInstance(&v,&DeeList_Type) != 0) return -1;
 DeeList_ACQUIRE(v);
 for (i = 0; i < DeeList_SIZE(v); ++i) {
  Dee_INCREF(elem = DeeList_GET(v,i));
  DeeList_RELEASE(v);
  if (DeeError_TypeError_CheckTypeExact(elem,&DeeTuple_Type) != 0) return -1;
  if (DeeTuple_SIZE(elem) != 2) { DeeError_TypeError_TupleSizeExpected(2,DeeTuple_SIZE(elem)); return -1; }
  if (DeeError_TypeError_CheckTypeExact(DeeTuple_GET(elem,0),&DeeString_Type) != 0) return -1;
  if (DeeError_TypeError_CheckTypeExact(DeeTuple_GET(elem,1),&DeeString_Type) != 0) return -1;
  DeeList_ACQUIRE(v);
 }
 DeeList_RELEASE(v);
 return DeeProcess_SetEnviron((DeeObject *)self,v);
}
#ifdef DEE_PLATFORM_WINDOWS
static DeeObject *DEE_CALL _deeprocess_win32_priority_boost_get(
 DeeProcessObject *self, void *DEE_UNUSED(closure)) {
 int result;
 if ((result = DeeProcess_Win32GetPriorityBoost((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static int DEE_CALL _deeprocess_win32_priority_boost_del(
 DeeProcessObject *self, void *DEE_UNUSED(closure)) {
 return DeeProcess_Win32SetPriorityBoost((DeeObject *)self,1);
}
static int DEE_CALL _deeprocess_win32_priority_boost_set(
 DeeProcessObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int mode;
 if ((mode = DeeObject_Bool(v)) < 0) return -1;
 return DeeProcess_Win32SetPriorityBoost((DeeObject *)self,mode);
}
#endif

static DeeObject *DEE_CALL _deeprocess_self(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":self") != 0) return NULL;
 DeeReturn_NEWREF(DeeProcess_Self());
}
static DeeObject *DEE_CALL _deeprocess_enum(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":enum") != 0) return NULL;
 return DeeProcess_Enumerate();
}


#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deeprocess_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deeprocess_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__p_handle",DeeProcessObject,p_handle,DeeProcessHandle),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_lock",DeeProcessObject,p_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_flags",DeeProcessObject,p_flags,Dee_uint8_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_exe",DeeProcessObject,p_exe,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_args",DeeProcessObject,p_args,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_environ",DeeProcessObject,p_environ,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_stdin",DeeProcessObject,p_stdin,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_stdout",DeeProcessObject,p_stdout,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_stderr",DeeProcessObject,p_stderr,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_cwd",DeeProcessObject,p_cwd,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__p_priority",DeeProcessObject,p_priority,double),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMethodDef const _deeprocess_tp_methods[] = {
 DEE_METHODDEF_v100("start",member(&_deeprocess_start),"(bool detached = false) -> bool"),
 DEE_METHODDEF_v100("join",member(&_deeprocess_join),"() -> process_return"),
 DEE_METHODDEF_v100("try_join",member(&_deeprocess_try_join),"() -> (bool,process_return)"),
 DEE_METHODDEF_v100("join_timed",member(&_deeprocess_join_timed),"(unsigned int msecs) -> (bool,process_return)"),
 DEE_METHODDEF_v100("id",member(&_deeprocess_id),"() -> process_id"),
 DEE_METHODDEF_v100("started",member(&_deeprocess_started),"() -> bool"),
 DEE_METHODDEF_v100("detach",member(&_deeprocess_detach),"() -> bool"),
 DEE_METHODDEF_v100("detached",member(&_deeprocess_detached),"() -> bool"),
 DEE_METHODDEF_v100("terminate",member(&_deeprocess_terminate),"(process_return exit_code = 0) -> bool"),
 DEE_METHODDEF_v100("terminated",member(&_deeprocess_terminated),"() -> bool"),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_METHODDEF_v100("win32_times",member(&_deeprocess_win32_times),"() -> (time,time,time,time)"),
 DEE_METHODDEF_v100("win32_creation_times",member(&_deeprocess_win32_creation_time),"() -> time"),
 DEE_METHODDEF_v100("win32_exit_time",member(&_deeprocess_win32_exit_time),"() -> time"),
 DEE_METHODDEF_v100("win32_kernel_time",member(&_deeprocess_win32_kernel_time),"() -> time"),
 DEE_METHODDEF_v100("win32_user_time",member(&_deeprocess_win32_user_time),"() -> time"),
 DEE_METHODDEF_v100("win32_is_critical",member(&_deeprocess_win32_is_critical),"() -> bool"),
#endif
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deeprocess_tp_getsets[] = {
 DEE_GETSETDEF_v100("stdin",member(&_deeprocess_stdin_get),member(&_deeprocess_stdin_del),member(&_deeprocess_stdin_set),
  "-> file\n"
  "\tGet/Set the stdin file of a process before its launch.\n"
  "\tIf the process is 'process.self()', this is identical to overwriting 'file.io.stdin'"),
 DEE_GETSETDEF_v100("stdout",member(&_deeprocess_stdout_get),member(&_deeprocess_stdout_del),member(&_deeprocess_stdout_set),
  "-> file\n"
  "\tGet/Set the stdout file of a process before its launch.\n"
  "\tIf the process is 'process.self()', this is identical to overwriting 'file.io.stdout'"),
 DEE_GETSETDEF_v100("stderr",member(&_deeprocess_stderr_get),member(&_deeprocess_stderr_del),member(&_deeprocess_stderr_set),
  "-> file\n"
  "\tGet/Set the stderr file of a process before its launch.\n"
  "\tIf the process is 'process.self()', this is identical to overwriting 'file.io.stderr'"),
 DEE_GETSETDEF_v100("priority",member(&_deeprocess_priority_get),member(&_deeprocess_priority_del),member(&_deeprocess_priority_set),
  "-> double\n"
  "\tGet/Set prority of a process as a floating point number clamped between 0.0 and 1.0.\n"
  "\tThe default value is 0.5"),
 DEE_GETSETDEF_v100("cwd",member(&_deeprocess_cwd_get),member(&_deeprocess_cwd_del),member(&_deeprocess_cwd_set),
  "-> string\n"
  "\tGet/Set the initial working directory of a process before its launch.\n"
  "\tIf the process is 'process.self()', this is identical to calling 'fs.getcwd()' / 'fs.chdir()'"),
 DEE_GETSETDEF_v100("wcwd",member(&_deeprocess_wcwd_get),member(&_deeprocess_cwd_del),member(&_deeprocess_cwd_set),
  "-> string.wide\n\tSame as 'cwd', but using widestring encoding"),
 DEE_GETSETDEF_v100("cmd",member(&_deeprocess_cmd_get),null,null,
  "-> string\n\tReturns the commandline used to launch any given process.\n"
  "\tNote, that on windows a running process is allowed to modify this value."),
 DEE_GETSETDEF_v100("wcmd",member(&_deeprocess_wcmd_get),null,null,
  "-> string.wide\n\tSame as 'cmd', but using widestring encoding"),
 DEE_GETSETDEF_v100("exe",member(&_deeprocess_exe_get),null,null,
  "-> string\n\tReturns the filename of the executable a process is running from"),
 DEE_GETSETDEF_v100("wexe",member(&_deeprocess_wexe_get),null,null,
  "-> string.wide\n\tSame as 'exe', but using widestring encoding"),
 DEE_GETSETDEF_v100("args",member(&_deeprocess_args_get),null,null,
  "-> string\n\tReturns the argument portion of a process as a space-seperated string"),
 DEE_GETSETDEF_v100("wargs",member(&_deeprocess_wargs_get),null,null,
  "-> string.wide\n\tSame as 'args', but using widestring encoding"),
 DEE_GETSETDEF_v100("argv",member(&_deeprocess_argv_get),null,null,
  "-> interable\n\tReturns an iterable sequence of strings, yielding the different arguments used to launch the process"),
 DEE_GETSETDEF_v100("wargv",member(&_deeprocess_wargv_get),null,null,
  "-> interable\n\tSame as 'argv', but yielded string use the widestring encoding"),
 DEE_GETSETDEF_v100("env",member(&_deeprocess_env_get),member(&_deeprocess_env_del),member(&_deeprocess_env_set),
  "-> iterable\n"
  "\tGet/Set environment strings of a process before its launch.\n"
  "\tIterating the returned sequence will yield key-item pairs of strings"),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_GETSETDEF_v100("win32_priority_boost",
  member(&_deeprocess_win32_priority_boost_get),
  member(&_deeprocess_win32_priority_boost_del),
  member(&_deeprocess_win32_priority_boost_set),
  "win32_priority_boost -> bool\n"
  "\tConfigure the win32 process priority boost system hint."),
#endif
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deeprocess_tp_class_methods[] = {
 DEE_METHODDEF_v100("self",member(&_deeprocess_self),
  "() -> process\n"
  "@return: A process object describing the current process.\n"
  "\tThis process has special permissions, allowing for direct modification of deemon's current working directory and more."
  "\tThe returned process is also available in '\"/proc/self/\"'"),
 DEE_METHODDEF_v100("enum",member(&_deeprocess_enum),
  "() -> interable\n"
  "@return: An interable sequence of process objects, detailing all running processes that the caller has access to.\n"
  "\tNote, that only processes visible to deemon will returned.\n"
  "\tCalling this function lists the same processes as available in \"/proc/\""),
 DEE_METHODDEF_END_v100
};



DeeTypeObject DeeProcess_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("process"),member(
  "(string cmdline) -> process\n"
  "(string exe, string args) -> process\n"
  "(string exe, iterable argv) -> process\n"
  "\tCreates a new process constroller with the given commandline\n"),
  member(DEE_TYPE_FLAG_VAR_OBJECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,null,null,
  member(&_deeprocess_tp_any_new)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeprocess_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeprocess_tp_str),
  member(&_deeprocess_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deeprocess_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deeprocess_tp_cmp_lo),member(&_deeprocess_tp_cmp_le),
  member(&_deeprocess_tp_cmp_eq),member(&_deeprocess_tp_cmp_ne),
  member(&_deeprocess_tp_cmp_gr),member(&_deeprocess_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeprocess_tp_members),member(_deeprocess_tp_getsets),
  member(_deeprocess_tp_methods),null,null,member(_deeprocess_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};


#ifndef __INTELLISENSE__
#define WIDE
#include "process.unicode.c.inl"
#include "process.unicode.c.inl"
#endif

#undef DeeProcess_ID
DEE_A_RET_WUNUSED DeeProcessID DeeProcess_ID(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
#ifdef DEE_PLATFORM_WINDOWS
 return GetProcessId(DeeProcess_HANDLE(self));
#else
 return DeeProcess_HANDLE(self);
#endif
}

#undef DeeProcess_SelfID
DEE_A_RET_WUNUSED DeeProcessID DeeProcess_SelfID(void) {
#ifdef DEE_PLATFORM_WINDOWS
 return GetCurrentProcessId();
#elif defined(DEE_PLATFORM_UNIX)
 return getpid();
#else
 return DEE_PROCESS_INVALID_ID;
#endif
}

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_PROCESS */

#endif /* !GUARD_DEEMON_MP_PROCECSS_H */
