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

#ifdef __INTELLISENSE__
#include <deemon/sys/_win32.sysfd.filename.c.inl>
#endif

#include <deemon/__xconf.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/runtime/extern.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifndef DEE_PRIVATE_WIN32_GETFINALPATHNAMEBYHANDLEA_PROTOTYPES_DEFINED
#define DEE_PRIVATE_WIN32_GETFINALPATHNAMEBYHANDLEA_PROTOTYPES_DEFINED 1
typedef DWORD (WINAPI *LPGETFINALPATHNAMEBYHANDLEA)(HANDLE hFile, LPSTR lpszFilePath, DWORD cchFilePath, DWORD dwFlags);
typedef DWORD (WINAPI *LPGETFINALPATHNAMEBYHANDLEW)(HANDLE hFile, LPWSTR lpszFilePath, DWORD cchFilePath, DWORD dwFlags);
typedef DWORD (WINAPI *LPGETMAPPEDFILENAMEA)(HANDLE hProcess, LPVOID lpv, LPSTR lpFilename, DWORD nSize);
typedef DWORD (WINAPI *LPGETMAPPEDFILENAMEW)(HANDLE hProcess, LPVOID lpv, LPWSTR lpFilename, DWORD nSize);
static LPGETFINALPATHNAMEBYHANDLEW pGetFinalPathNameByHandleW = NULL;
static LPGETFINALPATHNAMEBYHANDLEA pGetFinalPathNameByHandleA = NULL;
static LPGETMAPPEDFILENAMEA pGetMappedFileNameA = NULL;
static LPGETMAPPEDFILENAMEW pGetMappedFileNameW = NULL;
static char const *sGetFinalPathNameByHandleA = "GetFinalPathNameByHandleA";;
static char const *sGetFinalPathNameByHandleW = "GetFinalPathNameByHandleW";;
static char const *sGetMappedFileNameA        = "GetMappedFileNameA";
static char const *sGetMappedFileNameW        = "GetMappedFileNameW";
static char const *sK32GetMappedFileNameA     = "K32GetMappedFileNameA";
static char const *sK32GetMappedFileNameW     = "K32GetMappedFileNameW";
static Dee_WideChar const ws_KERNAL32[] = {'K','E','R','N','E','L','3','2',0};
static Dee_WideChar const ws_Psapi_dll[] = {'P','s','a','p','i','.','d','l','l',0};
#endif

#ifdef WIDE
#define pGetFinalPathNameByHandle pGetFinalPathNameByHandleW
#define sGetFinalPathNameByHandle sGetFinalPathNameByHandleW
#define sGetMappedFileName        sGetMappedFileNameW
#define sK32GetMappedFileName     sK32GetMappedFileNameW
#define pGetMappedFileName        pGetMappedFileNameW
#define DeeString_F(x)            DeeWideString_##x
#define DEE_CHAR                  Dee_WideChar
#else
#define pGetFinalPathNameByHandle pGetFinalPathNameByHandleA
#define sGetFinalPathNameByHandle sGetFinalPathNameByHandleA
#define sGetMappedFileName        sGetMappedFileNameA
#define sK32GetMappedFileName     sK32GetMappedFileNameW
#define pGetMappedFileName        pGetMappedFileNameA
#define DeeString_F(x)            DeeUtf8String_##x
#define DEE_CHAR                  Dee_Utf8Char
#endif

#ifdef WIDE
DeeObject *DeeWin32SysFileFD_DoGetWideFilename
#else
DeeObject *DeeWin32SysFileFD_DoGetUtf8Filename
#endif
(HANDLE hFile) {
 HANDLE hFileMap,hCurrProcess; DWORD error;
 DeeObject *result; void *pMem; DEE_CHAR *name;
 // NOTE: We prefer the GetFinalPathNameByHandle functions,
 //       because those work for empty files...
 DEE_ATOMIC_ONCE({
  *(FARPROC *)&pGetFinalPathNameByHandle = GetProcAddress(GetModuleHandleW(ws_KERNAL32),sGetFinalPathNameByHandle);
 });
 if (pGetFinalPathNameByHandle) {
  if DEE_UNLIKELY((result = DeeString_F(NewSized)(
   DEE_XCONFIG_FSBUFSIZE_WIN32_GETFINALPATHBYHANDLE)) == NULL) return NULL;
gfpbh_again:
  error = (*pGetFinalPathNameByHandle)(hFile,DeeString_F(STR)(result),
                                       (DWORD)DeeString_F(SIZE)(result),
                                       VOLUME_NAME_DOS);
  if DEE_UNLIKELY(!error) {
   if ((error = GetLastError()) == 0) {
#ifdef WIDE
    DeeReturn_EmptyWideString;
#else
    DeeReturn_EmptyUtf8String;
#endif
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "%s(%p,...,%Iu,VOLUME_NAME_DOS) : %K",sGetFinalPathNameByHandle,hFile,
                       DeeString_F(SIZE)(result),
                       DeeSystemError_Win32ToString(error));
   return NULL;
  }
  if ((Dee_size_t)error < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,(Dee_size_t)error) != 0) Dee_CLEAR(result);
  } else if ((Dee_size_t)error != DeeString_F(SIZE)(result)) {
   // Reallocate the buffer and try again
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,(Dee_size_t)(error-1)) != 0) Dee_CLEAR(result);
   goto gfpbh_again;
  }
  // Strip away a prefixed unc marker
  if (DeeString_F(SIZE)(result) >= 4) {
   name = DeeString_F(STR)(result);
   if (name[0] == '\\' && name[1] == '\\'
    && name[2] == '?' && name[3] == '\\') {
    Dee_size_t newsize = DeeString_F(SIZE)(result)-4;
    memmove(name,name+4,newsize*sizeof(DEE_CHAR));
    if DEE_UNLIKELY(DeeString_F(Resize)(&result,newsize) != 0) Dee_CLEAR(result);
   }
  }
  return result;
 }

 DEE_ATOMIC_ONCE({
/*if (!pGetMappedFileName)*/*(FARPROC *)&pGetMappedFileName = GetProcAddress(GetModuleHandleW(ws_KERNAL32),sK32GetMappedFileName);
  if (!pGetMappedFileName)  *(FARPROC *)&pGetMappedFileName = GetProcAddress(GetModuleHandleW(ws_KERNAL32),sGetMappedFileName);
  if (!pGetMappedFileName)  *(void   **)&pGetMappedFileName = DeeSharedLib_WidePoolTryImport(ws_Psapi_dll,sGetMappedFileName);
  if (!pGetMappedFileName)  *(void   **)&pGetMappedFileName = DeeSharedLib_WidePoolTryImport(ws_Psapi_dll,sK32GetMappedFileName);
 });
 if DEE_UNLIKELY(!pGetMappedFileName) {
  DeeError_SET_STRING(&DeeErrorType_NotImplemented,
                      "Impossible to retrieve filename from handle");
  return NULL;
 }
 //////////////////////////////////////////////////////////////////////////
 // The below code is derived from here:
 // >> https://msdn.microsoft.com/en-us/library/windows/desktop/aa366789(v=vs.85).aspx
 //////////////////////////////////////////////////////////////////////////


 // Create a file mapping object.
#ifdef WIDE
 if DEE_UNLIKELY((hFileMap = CreateFileMappingW(hFile,NULL,PAGE_READONLY,0,1,NULL)) == NULL)
#else
 if DEE_UNLIKELY((hFileMap = CreateFileMappingA(hFile,NULL,PAGE_READONLY,0,1,NULL)) == NULL)
#endif
 {
  DeeError_SetStringf(&DeeErrorType_SystemError,
#ifdef WIDE
                      "CreateFileMappingW(%p,NULL,PAGE_READONLY,0,1,NULL) : %K",
#else
                      "CreateFileMappingA(%p,NULL,PAGE_READONLY,0,1,NULL) : %K",
#endif
                      hFile,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 if DEE_UNLIKELY((pMem = MapViewOfFile(hFileMap,FILE_MAP_READ,0,0,1)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "MapViewOfFile(%p,FILE_MAP_READ,0,0,1) : %K",hFileMap,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_filemap:
  if DEE_UNLIKELY(!CloseHandle(hFileMap)) SetLastError(0);
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(
  DEE_XCONFIG_FSBUFSIZE_WIN32_GETMAPPEDFILENAME)) == NULL) goto err_filemap;
 hCurrProcess = GetCurrentProcess();
mapfilename_again:
 error = (*pGetMappedFileName)(hCurrProcess,pMem,DeeString_F(STR)(result),
                               (DWORD)(DeeString_F(SIZE)(result)+1));
 if DEE_UNLIKELY(error == 0) {
  error = GetLastError();
  Dee_DECREF(result);
  if (error == 0) {
#ifdef WIDE
   Dee_INCREF(result = Dee_EmptyWideString);
#else
   Dee_INCREF(result = Dee_EmptyUtf8String);
#endif
   if DEE_UNLIKELY(!UnmapViewOfFile(pMem)) SetLastError(0);
   if DEE_UNLIKELY(!CloseHandle(hFileMap)) SetLastError(0);
   return result;
  }
  if (error == ERROR_INSUFFICIENT_BUFFER) {
   // Increase the buffersize and try again
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,DeeString_F(SIZE)(result)*2) != 0) {
err_r_unmap_vof: Dee_DECREF(result); goto err_unmap_vof;
   }
   goto mapfilename_again;
  }
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "%s(%p,%p,...,%lu) : %K",
                      sGetMappedFileName,pMem,(DWORD)(DeeString_F(SIZE)(result)+1),
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_unmap_vof: if DEE_UNLIKELY(!UnmapViewOfFile(pMem)) SetLastError(0); goto err_filemap;
 }
 if DEE_LIKELY(error != DeeString_F(SIZE)(result)+1) {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,(Dee_size_t)error) != 0) goto err_r_unmap_vof;
 }
 // Close the mapping
 if DEE_UNLIKELY(!UnmapViewOfFile(pMem)) SetLastError(0);
 if DEE_UNLIKELY(!CloseHandle(hFileMap)) SetLastError(0);

#if 0
 // Translate path with device name to drive letters.
 TCHAR szTemp[BUFSIZE];
 szTemp[0] = '\0';

 if (GetLogicalDriveStrings(BUFSIZE-1, szTemp)) 
 {
   TCHAR szName[MAX_PATH];
   TCHAR szDrive[3] = TEXT(" :");
   BOOL bFound = FALSE;
   TCHAR* p = szTemp;

   do 
   {
     // Copy the drive letter to the template string
     *szDrive = *p;

     // Look up each device name
     if (QueryDosDevice(szDrive, szName, MAX_PATH))
     {
       size_t uNameLen = _tcslen(szName);

       if (uNameLen < MAX_PATH) 
       {
         bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
                  && *(pszFilename + uNameLen) == _T('\\');

         if (bFound) 
         {
           // Reconstruct pszFilename using szTempFile
           // Replace device path with DOS path
           TCHAR szTempFile[MAX_PATH];
           StringCchPrintf(szTempFile,
                     MAX_PATH,
                     TEXT("%s%s"),
                     szDrive,
                     pszFilename+uNameLen);
           StringCchCopyN(pszFilename, MAX_PATH+1, szTempFile, _tcslen(szTempFile));
         }
       }
     }

     // Go to the next NULL character.
     while (*p++);
   } while (!bFound && *p); // end of string
 }
#endif
 return result;
}

#undef pGetFinalPathNameByHandle
#undef sGetFinalPathNameByHandle
#undef sGetMappedFileName
#undef sK32GetMappedFileName
#undef pGetMappedFileName
#undef DeeString_F
#undef DEE_CHAR

#ifdef WIDE
#undef WIDE
#endif

DEE_DECL_END
