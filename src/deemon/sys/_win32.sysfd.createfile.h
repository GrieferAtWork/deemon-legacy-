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
#include <deemon/sys/_win32.sysfd.h>
DEE_DECL_BEGIN
#endif

#define DEE_WIN32_DO_CREATEFILEW(filename,dwDesiredAccess,dwShareMode,dwCreationDisposition,dwFlagsAndAttributes)\
 CreateFileW(filename,dwDesiredAccess,dwShareMode,NULL,dwCreationDisposition,dwFlagsAndAttributes,NULL)


#ifndef TRY_CREATE
static DEE_ATTRIBUTE_UNUSED void _DeeWin32Sys_ThrowCreateFileError(
 DWORD error, Dee_WideChar const *filename, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeTypeObject *used_error_type;
 switch (error) {
  case ERROR_FILE_NOT_FOUND:
  case ERROR_PATH_NOT_FOUND:
  case ERROR_BAD_NETPATH: used_error_type = &DeeErrorType_FileNotFound; break;
  default:                used_error_type = &DeeErrorType_IOError; break;
 }
 DeeError_SetStringf(used_error_type,
                     "CreateFileW(%lq,%lu,%lu,NULL,%lu,%lu,NULL) : %K",
                     filename,dwDesiredAccess,dwShareMode,dwCreationDisposition,dwFlagsAndAttributes,
                     DeeSystemError_Win32ToString(error));
}
#endif

#ifdef TRY_CREATE
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideTryCreateFileObject
#else
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideCreateFileObject
#endif
(DeeObject const *filename, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 Dee_size_t filename_size; Dee_WideChar const *wfilename; HANDLE result;
 DEE_ASSERT(DeeObject_Check(filename) && DeeWideString_Check(filename));
 wfilename = DeeWideString_STR(filename);
 result = DEE_WIN32_DO_CREATEFILEW(wfilename,dwDesiredAccess,dwShareMode,
                                   dwCreationDisposition,dwFlagsAndAttributes);
 if DEE_LIKELY(result != INVALID_HANDLE_VALUE) return result;
 if ((wfilename[0] != '\\' || wfilename[1] != '\\'
   || wfilename[2] != '?' || wfilename[3] != '\\')
   && wfilename[0] && wfilename[1] == ':') {
  Dee_WideChar *uncwfilename,*dst_string;
  // Create UNC filename
  filename_size = DeeWideString_SIZE(filename);
  while DEE_UNLIKELY((uncwfilename = (Dee_WideChar *)
   malloc_nz((filename_size+5)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
#ifndef TRY_CREATE
   DeeError_NoMemory();
#endif
   return INVALID_HANDLE_VALUE;
  }
  dst_string = uncwfilename;
  *dst_string++ = '\\';
  *dst_string++ = '\\';
  *dst_string++ = '?';
  *dst_string++ = '\\';
  memcpy(dst_string,DeeWideString_STR(filename),filename_size*sizeof(Dee_WideChar));
  dst_string[filename_size] = 0;
  result = DEE_WIN32_DO_CREATEFILEW(uncwfilename,dwDesiredAccess,dwShareMode,
                                    dwCreationDisposition,dwFlagsAndAttributes);
#ifndef TRY_CREATE
  if (result == INVALID_HANDLE_VALUE) {
   _DeeWin32Sys_ThrowCreateFileError(GetLastError(),uncwfilename,dwDesiredAccess,
                                     dwShareMode,dwCreationDisposition,dwFlagsAndAttributes);
  }
#endif
  free_nn(uncwfilename);
 }
#ifndef TRY_CREATE
 else {
  _DeeWin32Sys_ThrowCreateFileError(GetLastError(),DeeWideString_STR(filename),dwDesiredAccess,
                                    dwShareMode,dwCreationDisposition,dwFlagsAndAttributes);
  return INVALID_HANDLE_VALUE;
 }
#endif
 return result;
}

#ifdef TRY_CREATE
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideTryCreateFile
#else
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideCreateFile
#endif
(Dee_WideChar const *filename, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 Dee_size_t filename_size; HANDLE result;
 result = DEE_WIN32_DO_CREATEFILEW(filename,dwDesiredAccess,dwShareMode,
                                   dwCreationDisposition,dwFlagsAndAttributes);
 if DEE_LIKELY(result != INVALID_HANDLE_VALUE) return result;
 if ((filename[0] != '\\' || filename[1] != '\\'
   || filename[2] != '?' || filename[3] != '\\')
   && filename[0] && filename[1] == ':') {
  Dee_WideChar *uncwfilename,*dst_string;
  // Create UNC filename
  filename_size = Dee_WideStrLen(filename);
  while DEE_UNLIKELY((uncwfilename = (Dee_WideChar *)
   malloc_nz((filename_size+5)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
#ifndef TRY_CREATE
   DeeError_NoMemory();
#endif
   return INVALID_HANDLE_VALUE;
  }
  dst_string = uncwfilename;
  *dst_string++ = '\\';
  *dst_string++ = '\\';
  *dst_string++ = '?';
  *dst_string++ = '\\';
  memcpy(dst_string,filename,filename_size*sizeof(Dee_WideChar));
  dst_string[filename_size] = 0;
  result = DEE_WIN32_DO_CREATEFILEW(uncwfilename,dwDesiredAccess,dwShareMode,
                                    dwCreationDisposition,dwFlagsAndAttributes);
#ifndef TRY_CREATE
  if (result == INVALID_HANDLE_VALUE) {
   _DeeWin32Sys_ThrowCreateFileError(GetLastError(),uncwfilename,dwDesiredAccess,
                                     dwShareMode,dwCreationDisposition,dwFlagsAndAttributes);
  }
#endif
  free_nn(uncwfilename);
 }
#ifndef TRY_CREATE
 else {
  _DeeWin32Sys_ThrowCreateFileError(GetLastError(),filename,dwDesiredAccess,
                                    dwShareMode,dwCreationDisposition,dwFlagsAndAttributes);
  return INVALID_HANDLE_VALUE;
 }
#endif
 return result;
}


#ifdef TRY_CREATE
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8TryCreateFileObject
#else
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8CreateFileObject
#endif
(DeeObject const *filename, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeObject *wfilename; HANDLE result;
 DEE_ASSERT(DeeObject_Check(filename) && DeeUtf8String_Check(filename));
 if DEE_UNLIKELY((wfilename = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(filename),DeeUtf8String_STR(filename))) == NULL) {
#ifndef TRY_CREATE
  DeeError_HandledOne();
#endif
  return INVALID_HANDLE_VALUE;
 }
#ifdef TRY_CREATE
 result = DeeWin32Sys_WideTryCreateFileObject(wfilename,dwDesiredAccess,dwShareMode,
                                              dwCreationDisposition,dwFlagsAndAttributes);
#else
 result = DeeWin32Sys_WideCreateFileObject(wfilename,dwDesiredAccess,dwShareMode,
                                           dwCreationDisposition,dwFlagsAndAttributes);
#endif
 Dee_DECREF(wfilename);
 return result;
}
#ifdef TRY_CREATE
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8TryCreateFile
#else
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8CreateFile
#endif
(Dee_Utf8Char const *filename, DWORD dwDesiredAccess,
 DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeObject *wfilename; HANDLE result;
 if ((wfilename = DeeWideString_FromUtf8String(filename)) == NULL) {
#ifndef TRY_CREATE
  DeeError_HandledOne();
#endif
  return INVALID_HANDLE_VALUE;
 }
#ifdef TRY_CREATE
 result = DeeWin32Sys_WideTryCreateFileObject(wfilename,dwDesiredAccess,dwShareMode,
                                              dwCreationDisposition,dwFlagsAndAttributes);
#else
 result = DeeWin32Sys_WideCreateFileObject(wfilename,dwDesiredAccess,dwShareMode,
                                           dwCreationDisposition,dwFlagsAndAttributes);
#endif
 Dee_DECREF(wfilename);
 return result;
}

#undef DEE_WIN32_DO_CREATEFILEW
#ifdef TRY_CREATE
#undef TRY_CREATE
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

