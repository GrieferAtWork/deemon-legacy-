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
#ifndef GUARD_DEEMON_SYS_WIN32_W32_HANDLE_NAME_H
#define GUARD_DEEMON_SYS_WIN32_W32_HANDLE_NAME_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/deemonrun.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/sys/win32/w32_ntdll.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <winternl.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

// This macro assures that INVALID_HANDLE_VALUE (0xFFFFFFFF) returns FALSE
#define IsConsoleHandle(h) (((((ULONG_PTR)h) & 0x10000003) == 0x3) ? TRUE : FALSE)



//////////////////////////////////////////////////////////////////////////
// Based on this:
// >> http://stackoverflow.com/questions/65170/how-to-get-name-associated-with-open-handle/
static DEE_ATTRIBUTE_UNUSED DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeWin32_WideGetNtPathFromHandle(DEE_A_IN HANDLE handle) {
 DeeWideStringObject *result,*newresult; UNICODE_STRING *buffer;
 DWORD dwBufferSize,dwNewBufferSize,dwUsedBufferSize; NTSTATUS error;
 DEE_STATIC_ASSERT(sizeof(UNICODE_STRING) <= Dee_OFFSETOF(DeeWideStringObject,s_str));
 static Dee_WideChar const _console_fmt[] = {
  '\\','D','e','v','i','c','e',
  '\\','C','o','n','s','o','l','e','%','0','4','l','u','X',0};
 if (IsConsoleHandle(handle)) return DeeWideString_Newf(_console_fmt,(DWORD)(Dee_uintptr_t)handle);
 DeeWin32Load_NtQueryObject();
 if (!DeeWin32_NtQueryObject) {
  DeeError_SetString(&DeeErrorType_NotImplemented,DeeWin32Name_NtQueryObject);
  return NULL;
 }
 result = (DeeWideStringObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeWideStringObject,s_str)+
  (DEE_XCONFIG_FSBUFSIZE_WIN32NTQUERYOBJECT*sizeof(Dee_WideChar)));
 if DEE_UNLIKELY(!result) return NULL;
 dwBufferSize = (DEE_XCONFIG_FSBUFSIZE_WIN32NTQUERYOBJECT*sizeof(Dee_WideChar));
again:
 buffer = ((UNICODE_STRING *)result->s_str)-1;
 buffer->Buffer = NULL;
 buffer->Length = 0;
 buffer->MaximumLength = 0;
 dwUsedBufferSize = 0;
 error = (*DeeWin32_NtQueryObject)(handle,DeeWin32_ObjectNameInformation,
                                   (PVOID)buffer,dwBufferSize,&dwUsedBufferSize);
 // TODO: Resize buffer when it's too small
 // TODO: Start out with a smaller buffer
 if (!buffer->Buffer || !buffer->Length) {
  if (error == DEE_WIN32_STATUS_INFO_LENGTH_MISMATCH) {
   dwNewBufferSize = dwBufferSize*2;
   if (dwNewBufferSize > dwBufferSize) {
    newresult = (DeeWideStringObject *)DeeObject_Realloc(
     result,Dee_OFFSETOF(DeeWideStringObject,s_str)+
     (dwBufferSize = dwNewBufferSize));
    if DEE_UNLIKELY(!newresult) goto err_freer;
    result = newresult;
    goto again;
   }
  }
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "%s(%p) : %K",
                      DeeWin32Name_NtQueryObject,handle,
                      DeeSystemError_Win32ToString(ERROR_FILE_NOT_FOUND));
err_freer:
  DeeObject_Free(result);
  return NULL;
 }
 result->s_len = buffer->Length/sizeof(Dee_WideChar);
 dwNewBufferSize = buffer->Length+sizeof(Dee_WideChar);
 if (dwNewBufferSize != dwBufferSize) {
  // Save up on some memory
  newresult = (DeeWideStringObject *)DeeObject_Realloc(
   result,Dee_OFFSETOF(DeeWideStringObject,s_str)+dwNewBufferSize);
  if DEE_UNLIKELY(!newresult) goto err_freer;
  result = newresult;
 }
 DeeObject_INIT(result,&DeeWideString_Type);
 return (DeeObject *)result;
}

DEE_STATIC_INLINE(int) _deew32_wsicmpit(Dee_WideChar const *a, Dee_WideChar const *b, Dee_size_t s) {
 Dee_WideChar const *aiter,*aend,*biter; Dee_WideChar ch;
 aend = (aiter = a)+s,biter = b;
 while (aiter != aend) {
  ch = *aiter++;
  if (ch >= 'a' && ch <= 'z') ch -= ('a'-'A');
  if (ch != *biter++) return 0;
 }
 return 1;
}
DEE_STATIC_INLINE(int) _deew32_wsicmp2(Dee_WideChar const *a, Dee_WideChar const *b, Dee_size_t s) {
 Dee_WideChar const *aiter,*aend,*biter; Dee_WideChar cha,chb;
 aend = (aiter = a)+s,biter = b;
 while (aiter != aend) {
  cha = *aiter++; if (cha >= 'a' && cha <= 'z') cha -= ('a'-'A');
  chb = *biter++; if (chb >= 'a' && chb <= 'z') chb -= ('a'-'A');
  if (cha != chb) return 0;
 }
 return 1;
}


//////////////////////////////////////////////////////////////////////////
// Returns the DOS path of a given NT path, but simply returns 'path' if it fails to do so
static DEE_ATTRIBUTE_UNUSED DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeWin32_WideGetDosPathFromNtPath(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 Dee_WideChar const *spath; LSTATUS error;
 WCHAR drive_strings[300]; WCHAR *drive_iter;
 static Dee_WideChar const text_Device_Serial[] = {
  '\\','D','E','V','I','C','E','\\','S','E','R','I','A','L',0};
 static Dee_WideChar const text_Device_UsbSer[] = {
  '\\','D','E','V','I','C','E','\\','U','S','B','S','E','R',0};
 static Dee_WideChar const text_Hardware_DeviceMap_SerialComm[] = {
       'H','a','r','d','w','a','r','e',
  '\\','D','e','v','i','c','e','M','a','p',
  '\\','S','e','r','i','a','l','C','o','m','m',0};
 static Dee_WideChar const text_Device_LanmanRedirector[] = {
  '\\','D','E','V','I','C','E',
  '\\','L','A','N','M','A','N','R','E','D','I','R','E','C','T','O','R','\\',0};
 static Dee_WideChar const text_Device_Mup[] = {
  '\\','D','E','V','I','C','E',
  '\\','M','U','P','\\',0};
 static Dee_WideChar const fmt_netpath[] = {
  '\\','\\','%','l','$','s',0};
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 spath = DeeWideString_STR(path);
 if (DeeWideString_SIZE(path) >= 14 && (
     _deew32_wsicmpit(spath,text_Device_Serial,14)
  || _deew32_wsicmpit(spath,text_Device_UsbSer,14))) {
  HKEY h_Key; WCHAR comport[50]; DWORD u32_Type,u32_Size;
  if DEE_UNLIKELY((error = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
   text_Hardware_DeviceMap_SerialComm,0,KEY_QUERY_VALUE,&h_Key)) != ERROR_SUCCESS) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "RegOpenKeyExW(HKEY_LOCAL_MACHINE,%lq,0,KEY_QUERY_VALUE,...) : %K",
                       text_Hardware_DeviceMap_SerialComm,
                       DeeSystemError_Win32ToString((DWORD)error));
   return NULL;
  }
  u32_Size = sizeof(comport);
  if DEE_UNLIKELY((error = RegQueryValueExW(h_Key,spath,0,
   &u32_Type,(BYTE*)comport,&u32_Size)) != ERROR_SUCCESS) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "RegOpenKeyExW(%p,%lq,0,...) : %K",
                       h_Key,spath,
                       DeeSystemError_Win32ToString((DWORD)error));
   RegCloseKey(h_Key);
   return NULL;
  }
  RegCloseKey(h_Key);
  return DeeWideString_New(comport);
 }
 if (DeeWideString_SIZE(path) >= 12 && // Windows 7+
     _deew32_wsicmpit(spath,text_Device_Mup,12)
     ) return DeeWideString_Newf(fmt_netpath,(Dee_size_t)(DeeWideString_SIZE(path)-12),spath+12);
 if (DeeWideString_SIZE(path) >= 25 && // Windows XP
     _deew32_wsicmpit(spath,text_Device_LanmanRedirector,25)
     ) return DeeWideString_Newf(fmt_netpath,(Dee_size_t)(DeeWideString_SIZE(path)-25),spath+25);
 if (!GetLogicalDriveStringsW(sizeof(drive_strings)/sizeof(WCHAR),drive_strings)) goto retpath;
 drive_iter = drive_strings;
 while (drive_iter[0]) {
  WCHAR ntvolume[1000]; WCHAR *drive_next; Dee_size_t volume_size;
  drive_next = drive_iter+(Dee_WideStrLen(drive_iter)+1);
  drive_iter[2] = 0; // the backslash is not allowed for QueryDosDevice()
  ntvolume[0] = 0;
  // may return multiple strings!
  // returns very weird strings for network shares
  if (!QueryDosDeviceW(drive_iter,ntvolume,sizeof(ntvolume)/2)) goto retpath;
  volume_size = Dee_WideStrLen(ntvolume);
  if (volume_size <= DeeWideString_SIZE(path) &&
      _deew32_wsicmp2(spath,ntvolume,volume_size)) {
   static Dee_WideChar const _fmtdrive[] = {'%','l','s','%','l','$','s',0};
   return DeeWideString_Newf(_fmtdrive,drive_iter,
                             (Dee_size_t)(DeeWideString_SIZE(path)-volume_size),
                             spath+volume_size);
  }
  drive_iter = drive_next;
 }
retpath:
 DeeReturn_NEWREF(path);
}

static DEE_ATTRIBUTE_UNUSED DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeWin32_WideGetDosPathFromHandle(DEE_A_IN HANDLE handle) {
 DeeObject *result,*newresult;
 if DEE_UNLIKELY((result = DeeWin32_WideGetNtPathFromHandle(handle)) == NULL) return NULL;
 newresult = DeeWin32_WideGetDosPathFromNtPath(result);
 Dee_DECREF(result);
 return newresult;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_W32_HANDLE_NAME_H */
