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
#include "_win32.sysfs.h"
#define WIDE
#endif

#ifdef WIDE
#define DEESTRINGOBJECT       DeeWideStringObject
#define DeeString_F(x)        DeeWideString_##x
#define DeeVFS_F(x)           DeeVFS_Wide##x
#define DeeWin32Sys_F(x)      DeeWin32Sys_Wide##x
#define WIN32_F(x)            x##W
#define DEE_CHAR              Dee_WideChar
#define DEE_PRINTF_STRQ       "%lq"
#define DEERETURN_EMPTYSTRING DeeReturn_EmptyWideString
#else
#define DEESTRINGOBJECT       DeeUtf8StringObject
#define DeeString_F(x)        DeeUtf8String_##x
#define DeeVFS_F(x)           DeeVFS_Utf8##x
#define DeeWin32Sys_F(x)      DeeWin32Sys_Utf8##x
#define WIN32_F(x)            x##A
#define DEE_CHAR              Dee_Utf8Char
#define DEE_PRINTF_STRQ       "%q"
#define DEERETURN_EMPTYSTRING DeeReturn_EmptyUtf8String
#endif

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *) DeeWin32Sys_F(GetCwd)(void) {
 DWORD temp; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD)) == NULL) return NULL;
 temp = WIN32_F(GetCurrentDirectory)(DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD+1,DeeString_F(STR)(result));
 if DEE_UNLIKELY(!temp) {
err: DeeError_SetStringf(&DeeErrorType_SystemError,
                         DEE_PP_STR(WIN32_F(GetCurrentDirectory)) "() : %K",
                         DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_r: Dee_DECREF(result);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  temp = WIN32_F(GetCurrentDirectory)(temp+1,DeeString_F(STR)(result));
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeWin32Sys_F(GetEnv)(DEE_A_IN_Z DEE_CHAR const *envname) {
 DWORD error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETENV)) == NULL) return NULL;
 error = WIN32_F(GetEnvironmentVariable)(envname,DeeString_F(STR)(result),
                                         DEE_XCONFIG_FSBUFSIZE_WIN32GETENV+1);
 if DEE_UNLIKELY(error == 0) {
not_found: error = DeeSystemError_Win32Consume();
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetEnvironmentVariable))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      envname,DeeSystemError_Win32ToString(error));
  goto err_r;
 }
 if DEE_UNLIKELY(error > DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,error-1) != 0)
  {err_r: Dee_DECREF(result); return NULL; }
  error = WIN32_F(GetEnvironmentVariable)(envname,DeeString_F(STR)(result),error);
  if DEE_UNLIKELY(error == 0) goto not_found;
  // ** this could happen, if the variable changed between the 2 calls
  if DEE_UNLIKELY(error > DeeString_F(SIZE)(result)) goto again;
 } else if (error < DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,error) != 0) goto err_r;
 }
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DEESTRINGOBJECT) *)
DeeWin32Sys_F(TryGetEnv)(DEE_A_IN_Z DEE_CHAR const *envname) {
 DWORD error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETENV)) == NULL) {err_h: DeeError_HandledOne(); return NULL; }
 error = WIN32_F(GetEnvironmentVariable)(envname,DeeString_F(STR)(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETENV+1);
 if (error == 0) {not_found: SetLastError(0); Dee_DECREF(result); return NULL; }
 if DEE_UNLIKELY(error > DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,error-1) != 0) {err_r: Dee_DECREF(result); goto err_h; }
  error = WIN32_F(GetEnvironmentVariable)(envname,DeeString_F(STR)(result),error);
  if (error == 0) goto not_found;
  // ** this could happen, if the variable changed between the 2 calls
  if (error > DeeString_F(SIZE)(result)) goto again;
 } else if (error < DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,error) != 0) goto err_r;
 }
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeWin32Sys_F(GetTokenUserHome)(DEE_A_IN HANDLE hToken) {
 DWORD error; DeeObject *result; DWORD req_size = 0;
#ifdef WIDE
 typedef BOOL (WINAPI *LPGETUSERPROFILEDIRECTORYW)(
  HANDLE hToken, LPWSTR lpProfileDir, LPDWORD lpcchSize);
 static LPGETUSERPROFILEDIRECTORYW p_GetUserProfileDirectoryW;
 static char const s_GetUserProfileDirectoryW[] = "GetUserProfileDirectoryW";
#else
 typedef BOOL (WINAPI *LPGETUSERPROFILEDIRECTORYA)(
  HANDLE hToken, LPSTR lpProfileDir, LPDWORD lpcchSize);
 static LPGETUSERPROFILEDIRECTORYA p_GetUserProfileDirectoryA;
 static char const s_GetUserProfileDirectoryA[] = "GetUserProfileDirectoryA";
#endif
 DEE_ATOMIC_ONCE({
  HMODULE hUserenv;
  WIN32_F(p_GetUserProfileDirectory) = (WIN32_F(LPGETUSERPROFILEDIRECTORY))(
   ((hUserenv = GetModuleHandle(TEXT("USERENV"))) != NULL)
   ? GetProcAddress(hUserenv,WIN32_F(s_GetUserProfileDirectory))
   : DeeSharedLib_PoolTryImport("userenv.dll",WIN32_F(s_GetUserProfileDirectory)));
 });
 if DEE_UNLIKELY(!WIN32_F(p_GetUserProfileDirectory)) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetUserProfileDirectory))
                      "(...) : Not implemented");
  return NULL;
 }
 if DEE_UNLIKELY(!(*WIN32_F(p_GetUserProfileDirectory))(hToken,NULL,&req_size)) {
  error = DeeSystemError_Win32Consume();
  if DEE_UNLIKELY(error != ERROR_INSUFFICIENT_BUFFER) {
err_api:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       DEE_PP_STR(WIN32_F(GetUserProfileDirectory)) "(%p,NULL,...) : %K",hToken,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return NULL;
  }
 }
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(req_size-1)) == NULL) return NULL;
 if DEE_UNLIKELY(!(*WIN32_F(p_GetUserProfileDirectory))
                 (hToken,DeeString_F(STR)(result),&req_size)) {
  Dee_DECREF(result);
  error = DeeSystemError_Win32Consume();
  goto err_api;
 }
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeWin32Sys_F(GetProcessUserHome)(DEE_A_IN HANDLE hProcess) {
 DeeObject *result; HANDLE hProcessToken;
 if DEE_UNLIKELY(!OpenProcessToken(hProcess,TOKEN_QUERY,&hProcessToken)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcessToken(%p,TOKEN_QUERY,...) : %K",hProcess,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 result = DeeWin32Sys_F(GetTokenUserHome)(hProcessToken);
 if DEE_UNLIKELY(!CloseHandle(hProcessToken)) SetLastError(0);
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *) DeeWin32Sys_F(GetHome)(void) {
 DeeObject *result,*homedrive,*homepath;
 static DEE_CHAR const backslash[] = {'\\'};
 static DEE_CHAR const var_HOME[] = {'H','O','M','E',0};
 static DEE_CHAR const var_USERPROFILE[] = {'U','S','E','R','P','R','O','F','I','L','E',0};
 static DEE_CHAR const var_HOMEDRIVE[] = {'H','O','M','E','D','R','I','V','E',0};
 static DEE_CHAR const var_HOMEPATH[] = {'H','O','M','E','P','A','T','H',0};
 if ((result = DeeWin32Sys_F(TryGetEnv)(var_HOME)) != NULL) return result;
 if ((result = DeeWin32Sys_F(TryGetEnv)(var_USERPROFILE)) != NULL) return result;
 if ((homedrive = DeeWin32Sys_F(TryGetEnv)(var_HOMEDRIVE)) == NULL) goto win_home_api;
 if ((homepath = DeeWin32Sys_F(TryGetEnv)(var_HOMEPATH)) == NULL) { Dee_DECREF(homedrive); goto win_home_api; }
 result = DeeString_F(ConcatWithLength)(homedrive,DeeString_F(SIZE)(homepath),DeeString_F(STR)(homepath));
 Dee_DECREF(homepath);
 Dee_DECREF(homedrive);
 if DEE_UNLIKELY(!result) return NULL;
 if (!DeeString_F(SIZE)(result) || ( // Make sure we have a trailing backslash
  DeeString_F(STR)(result)[DeeString_F(SIZE)(result)-1] != '\\' &&
  DeeString_F(STR)(result)[DeeString_F(SIZE)(result)-1] != '/') &&
  DeeString_F(AppendWithLength)(&result,1,backslash) != 0) Dee_CLEAR(result);
 return result;
win_home_api:
 return DeeWin32Sys_F(GetProcessUserHome)(GetCurrentProcess());
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *) DeeWin32Sys_F(GetTmp)(void) {
 DeeObject *result; DWORD temp; DWORD error;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP)) == NULL) return NULL;
 temp = WIN32_F(GetTempPath)(DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP+1,DeeString_F(STR)(result));
 if DEE_UNLIKELY(!temp) {
err: error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if DEE_UNLIKELY(!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetTempPath)) "(%lu,...) : %K",
                      (DWORD)(DeeString_F(SIZE)(result)+1),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetTempPath)(temp+1,DeeString_F(STR)(result));
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6387)
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeWin32Sys_F(GetDllDirectory)(void) {
#ifdef WIDE
 typedef DWORD (WINAPI *LPGETDLLDIRECTORYW)(DWORD nBufferLength, LPWSTR lpBuffer);
 static LPGETDLLDIRECTORYW p_GetDllDirectoryW = NULL;
 static char const s_GetDllDirectoryW[] = "GetDllDirectoryW";
#else
 typedef DWORD (WINAPI *LPGETDLLDIRECTORYA)(DWORD nBufferLength, LPSTR lpBuffer);
 static LPGETDLLDIRECTORYA p_GetDllDirectoryA = NULL;
 static char const s_GetDllDirectoryA[] = "GetDllDirectoryA";
#endif
 DWORD temp,error; DeeObject *result;
 DEE_ATOMIC_ONCE({
  WIN32_F(p_GetDllDirectory) = (WIN32_F(LPGETDLLDIRECTORY))GetProcAddress(
   GetModuleHandle(TEXT("KERNEL32")),WIN32_F(s_GetDllDirectory));
 });
 if DEE_UNLIKELY(!WIN32_F(p_GetDllDirectory)) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetDllDirectory))
                      "(...) : Not implemented");
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY)) == NULL) return NULL;
 temp = (*WIN32_F(p_GetDllDirectory))(DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY+1,DeeString_F(STR)(result));
 if DEE_UNLIKELY(!temp) {
err:
  error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if DEE_UNLIKELY(!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetDllDirectory)) "(%lu,...) : %K",
                      (DWORD)(DeeString_F(SIZE)(result)+1),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = (*WIN32_F(p_GetDllDirectory))(temp+1,DeeString_F(STR)(result));
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeWin32Sys_F(GetSystemDirectory)(void) {
 DWORD temp,error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY)) == NULL) return NULL;
 if DEE_UNLIKELY((temp = WIN32_F(GetSystemDirectory)(
  DeeString_F(STR)(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY)) == 0) {
err: error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if DEE_UNLIKELY(!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetSystemDirectory)) "(%lu,...) : %K",
                      (DWORD)DeeString_F(SIZE)(result),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetSystemDirectory)(DeeString_F(STR)(result),temp);
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeWin32Sys_F(GetWindowsDirectory)(void) {
 DWORD temp,error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetWindowsDirectory)(DeeString_F(STR)(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY);
 if DEE_UNLIKELY(!temp) {
err: error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if DEE_UNLIKELY(!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetWindowsDirectory)) "(%lu,...) : %K",
                      (DWORD)DeeString_F(SIZE)(result),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetWindowsDirectory)(DeeString_F(STR)(result),temp);
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeWin32Sys_F(GetSystemWindowsDirectory)(void) {
 DWORD temp,error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetSystemWindowsDirectory)(DeeString_F(STR)(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY);
 if DEE_UNLIKELY(!temp) {
err:
  error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if DEE_UNLIKELY(!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetSystemWindowsDirectory)) "(%lu,...) : %K",
                      (DWORD)DeeString_F(SIZE)(result),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetSystemWindowsDirectory)(DeeString_F(STR)(result),temp);
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeWin32Sys_F(GetSysWow64Directory)(void) {
 DWORD temp,error; DeeObject *result;
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetSystemWow64Directory)(DeeString_F(STR)(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY);
 if DEE_UNLIKELY(!temp) {
err:
  error = DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetSystemWow64Directory)) "(%lu,...) : %K",
                      (DWORD)DeeString_F(SIZE)(result),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 if DEE_UNLIKELY(temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY) {
again:
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetSystemWow64Directory)(DeeString_F(STR)(result),temp);
  if DEE_UNLIKELY(!temp) goto err;
  if DEE_UNLIKELY(temp > DeeString_F(SIZE)(result)) goto again;
  if (temp < DeeString_F(SIZE)(result)) {
   if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
  }
 } else {
  if DEE_UNLIKELY(DeeString_F(Resize)(&result,temp) != 0) goto err_r;
 }
 return result;
}

DEE_DECL_END

#undef DEESTRINGOBJECT
#undef DeeString_F
#undef DeeVFS_F
#undef DeeWin32Sys_F
#undef WIN32_F
#undef DEE_CHAR
#undef DEE_PRINTF_STRQ
#undef DEERETURN_EMPTYSTRING

#ifdef WIDE
#undef WIDE
#endif


