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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSFD_C_INL
#define GUARD_DEEMON_SYS__WIN32_SYSFD_C_INL 1

#include <deemon/sys/_win32.sysfd.h>
#include <deemon/optional/atomic_once.h>

// NOTE: This file is part of the /src/deemon/file.c compilation unit

DEE_DECL_BEGIN

BOOL DeeWin32Sys_AcquirePrivilege(DEE_A_IN Dee_WideChar const *name) {
 HANDLE tok,hProcess; LUID luid; TOKEN_PRIVILEGES tok_priv; DWORD error;
 DEE_LVERBOSE1("DeeWin32Sys_AcquirePrivilege(%lq)\n",name);
 hProcess = GetCurrentProcess();
 if DEE_UNLIKELY(!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&tok)) return FALSE;
 if DEE_UNLIKELY(!LookupPrivilegeValueW(NULL,name,&luid)) return FALSE;
 tok_priv.PrivilegeCount = 1;
 tok_priv.Privileges[0].Luid = luid;
 tok_priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
 if DEE_UNLIKELY(!AdjustTokenPrivileges(tok,FALSE,&tok_priv,0,NULL,NULL)) return FALSE;
 error = GetLastError(); SetLastError(0);
 return DEE_UNLIKELY(error == ERROR_NOT_ALL_ASSIGNED) ? 0 : 1;
}
BOOL DeeWin32Sys_AcquireSeRestorePrivilege(void) {
 static Dee_WideChar const priv_name[] = {'S','e','R','e','s','t','o','r','e','P','r','i','v','i','l','e','g','e',0};
 static BOOL result = FALSE;
 DEE_ATOMIC_ONCE({ result = DeeWin32Sys_AcquirePrivilege(priv_name); });
 return result;
}



static Dee_WideChar const _deewin32sys_sKERNAL32[] = {'K','E','R','N','E','L','3','2',0};
typedef HANDLE (WINAPI *LPDEEWIN32REOPENFILE)(HANDLE hOriginalFile, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlags);
static char const *_deewin32sys_sReOpenFile = "ReOpenFile";
static LPDEEWIN32REOPENFILE _deewin32sys_pReOpenFile = NULL;
static struct DeeAtomicOnceFlag _deewin32sys_pReOpenFile_loaded = DeeAtomicOnceFlag_INIT();
#define _deewin32sys_pReOpenFile_LOAD()\
 DeeAtomicOnceFlag_RUN(&_deewin32sys_pReOpenFile_loaded,{\
  *(FARPROC *)&_deewin32sys_pReOpenFile = GetProcAddress(\
  GetModuleHandleW(_deewin32sys_sKERNAL32),_deewin32sys_sReOpenFile);\
 })



HANDLE DeeWin32Sys_TryReOpenFile(
 HANDLE hOriginalFile, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwFlagsAndAttributes) {
 DeeObject *filename; HANDLE result;
 _deewin32sys_pReOpenFile_LOAD();
 if (_deewin32sys_pReOpenFile) return (*_deewin32sys_pReOpenFile)(hOriginalFile,dwDesiredAccess,dwShareMode,dwFlagsAndAttributes);
 if ((filename = DeeWin32Sys_WideGetHandleFilename(hOriginalFile)) == NULL) { DeeError_HandledOne(); return INVALID_HANDLE_VALUE; }
 result = DeeWin32Sys_WideTryCreateFileObject(filename,dwDesiredAccess,dwShareMode,
                                              OPEN_EXISTING,dwFlagsAndAttributes);
 Dee_DECREF(filename);
 return result;
}
HANDLE DeeWin32Sys_ReOpenFile(
 HANDLE hOriginalFile, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwFlagsAndAttributes) {
 DeeObject *filename; HANDLE result;
 _deewin32sys_pReOpenFile_LOAD();
 if (_deewin32sys_pReOpenFile) {
  result = (*_deewin32sys_pReOpenFile)(hOriginalFile,dwDesiredAccess,dwShareMode,dwFlagsAndAttributes);
  if (result == INVALID_HANDLE_VALUE) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "ReOpenFile(%p,%lu,%lu,%lu) : %K",
                       hOriginalFile,dwDesiredAccess,dwShareMode,dwFlagsAndAttributes,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return INVALID_HANDLE_VALUE;
  }
 }
 if ((filename = DeeWin32Sys_WideGetHandleFilename(hOriginalFile)) == NULL) return INVALID_HANDLE_VALUE;
 result = DeeWin32Sys_WideCreateFileObject(filename,dwDesiredAccess,dwShareMode,
                                           OPEN_EXISTING,dwFlagsAndAttributes);
 Dee_DECREF(filename);
 return result;
}

BOOL DeeWin32Sys_CreateSymbolicLinkW(
 Dee_WideChar const *link_name, Dee_WideChar const *target_name) {
 typedef BOOLEAN (WINAPI *LPCREATESYMBOLICLINKW)(LPCWSTR lpSymlinkFileName, LPCWSTR lpTargetFileName, DWORD dwFlags);
 static LPCREATESYMBOLICLINKW pCreateSymbolicLinkW = NULL;
 DWORD target_attributes,link_flags;
 static char const *sCreateSymbolicLinkW = "CreateSymbolicLinkW";
 static Dee_WideChar const priv_name[] = {
  'S','e','C','r','e','a','t','e','S','y','m','b','o','l','i',
  'c','L','i','n','k','P','r','i','v','i','l','e','g','e',0};
 static BOOL has_privilege = FALSE;
 DEE_ATOMIC_ONCE({
  *(FARPROC *)&pCreateSymbolicLinkW = GetProcAddress(
  GetModuleHandleW(_deewin32sys_sKERNAL32),sCreateSymbolicLinkW);
 });
 if (!pCreateSymbolicLinkW) { SetLastError(ERROR_PROC_NOT_FOUND); return FALSE; }
 target_attributes = GetFileAttributesW(target_name);
 if (target_attributes == INVALID_FILE_ATTRIBUTES) return FALSE;
 link_flags = 0;
 if ((target_attributes&FILE_ATTRIBUTE_DIRECTORY)!=0)
  link_flags |= 0x1; // SYMBOLIC_LINK_FLAG_DIRECTORY
 if ((*pCreateSymbolicLinkW)(link_name,target_name,link_flags)) return TRUE;
 // Acquire the missing rights
 DEE_ATOMIC_ONCE({ has_privilege = DeeWin32Sys_AcquirePrivilege(priv_name); });
 if (!has_privilege) { SetLastError(ERROR_ACCESS_DENIED); return FALSE; }
 // Try again
 return (*pCreateSymbolicLinkW)(link_name,target_name,link_flags);
}



DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "_win32.sysfd.filename.impl.inl"
#include "_win32.sysfd.filename.impl.inl"
#endif

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFD_C_INL */
