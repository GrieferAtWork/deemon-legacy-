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

#define DEE_WIN32_DO_CREATEFILEW(filename,dwDesiredAccess,dwCreationDisposition,dwFlagsAndAttributes)\
 CreateFileW(filename,dwDesiredAccess,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwCreationDisposition,dwFlagsAndAttributes,NULL)


#ifndef TRY_CREATE
static DEE_ATTRIBUTE_UNUSED void _DeeWin32SysFileFD_ThrowCreateFileError(
 DWORD error, Dee_WideChar const *filename, DWORD dwDesiredAccess,
 DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeTypeObject *used_error_type;
 switch (error) {
  case ERROR_FILE_NOT_FOUND:
  case ERROR_PATH_NOT_FOUND:
  case ERROR_BAD_NETPATH: used_error_type = &DeeErrorType_FileNotFound; break;
  default:                used_error_type = &DeeErrorType_IOError; break;
 }
 DeeError_SetStringf(used_error_type,
                     "CreateFileW(%r,%lu,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,%lu,%lu,NULL) : %K",
                     filename,dwDesiredAccess,dwCreationDisposition,dwFlagsAndAttributes,
                     DeeSystemError_Win32ToString(error));
}
#endif

#ifdef TRY_CREATE
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideTryInitObject
#else
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideInitObject
#endif
(struct DeeWin32SysFileFD *self, DeeObject const *filename,
 DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 Dee_size_t filename_size; Dee_WideChar const *wfilename;
 DEE_ASSERT(DeeObject_Check(filename) && DeeWideString_Check(filename));
 wfilename = DeeWideString_STR(filename);
 self->w32_handle = DEE_WIN32_DO_CREATEFILEW(wfilename,dwDesiredAccess,
                                             dwCreationDisposition,dwFlagsAndAttributes);
 if (self->w32_handle != INVALID_HANDLE_VALUE) return TRUE;
 if (wfilename[0] != '\\' || wfilename[1] != '\\' ||
     wfilename[2] != '?' || wfilename[3] != '\\') {
  Dee_WideChar *uncwfilename,*dst_string;
  // Create UNC filename
  filename_size = DeeWideString_SIZE(filename);
  while DEE_UNLIKELY((uncwfilename = (Dee_WideChar *)
   malloc_nz((filename_size+5)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
#ifndef TRY_CREATE
   DeeError_NoMemory();
#endif
   return FALSE;
  }
  dst_string = uncwfilename;
  *dst_string++ = '\\';
  *dst_string++ = '\\';
  *dst_string++ = '?';
  *dst_string++ = '\\';
  memcpy(dst_string,filename,filename_size*sizeof(Dee_WideChar));
  dst_string[filename_size] = 0;
  self->w32_handle = DEE_WIN32_DO_CREATEFILEW(uncwfilename,dwDesiredAccess,
                                              dwCreationDisposition,dwFlagsAndAttributes);
#ifndef TRY_CREATE
  if (self->w32_handle == INVALID_HANDLE_VALUE) {
   _DeeWin32SysFileFD_ThrowCreateFileError(GetLastError(),uncwfilename,dwDesiredAccess,
                                           dwCreationDisposition,dwFlagsAndAttributes);
   free_nn(uncwfilename);
   return FALSE;
  }
#endif
  free_nn(uncwfilename);
 }
#ifdef TRY_CREATE
 return self->w32_handle != INVALID_HANDLE_VALUE;
#else
 else {
  _DeeWin32SysFileFD_ThrowCreateFileError(GetLastError(),DeeWideString_STR(filename),dwDesiredAccess,
                                          dwCreationDisposition,dwFlagsAndAttributes);
  return FALSE;
 }
 return TRUE;
#endif
}

#ifdef TRY_CREATE
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideTryInit
#else
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideInit
#endif
(struct DeeWin32SysFileFD *self, Dee_WideChar const *filename,
 DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 Dee_size_t filename_size;
 self->w32_handle = DEE_WIN32_DO_CREATEFILEW(filename,dwDesiredAccess,
                                             dwCreationDisposition,dwFlagsAndAttributes);
 if (self->w32_handle != INVALID_HANDLE_VALUE) return TRUE;
 if (filename && (filename[0] != '\\' || filename[1] != '\\' ||
                  filename[2] != '?' || filename[3] != '\\')) {
  Dee_WideChar *uncwfilename,*dst_string;
  // Create UNC filename
  filename_size = Dee_WideStrLen(filename);
  while DEE_UNLIKELY((uncwfilename = (Dee_WideChar *)
   malloc_nz((filename_size+5)*sizeof(Dee_WideChar))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
#ifndef TRY_CREATE
   DeeError_NoMemory();
#endif
   return FALSE;
  }
  dst_string = uncwfilename;
  *dst_string++ = '\\';
  *dst_string++ = '\\';
  *dst_string++ = '?';
  *dst_string++ = '\\';
  memcpy(dst_string,filename,filename_size*sizeof(Dee_WideChar));
  dst_string[filename_size] = 0;
  self->w32_handle = DEE_WIN32_DO_CREATEFILEW(uncwfilename,dwDesiredAccess,
                                              dwCreationDisposition,dwFlagsAndAttributes);
#ifndef TRY_CREATE
  if (self->w32_handle == INVALID_HANDLE_VALUE) {
   _DeeWin32SysFileFD_ThrowCreateFileError(GetLastError(),uncwfilename,dwDesiredAccess,
                                           dwCreationDisposition,dwFlagsAndAttributes);
   free_nn(uncwfilename);
   return FALSE;
  }
#endif
  free_nn(uncwfilename);
 }
#ifdef TRY_CREATE
 return self->w32_handle != INVALID_HANDLE_VALUE;
#else
 else {
  _DeeWin32SysFileFD_ThrowCreateFileError(GetLastError(),filename,dwDesiredAccess,
                                          dwCreationDisposition,dwFlagsAndAttributes);
  return FALSE;
 }
 return TRUE;
#endif
}


#ifdef TRY_CREATE
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8TryInitObject
#else
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8InitObject
#endif
(struct DeeWin32SysFileFD *self, DeeObject const *filename,
 DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeObject *wfilename; BOOL result;
 DEE_ASSERT(DeeObject_Check(filename) && DeeUtf8String_Check(filename));
 if DEE_UNLIKELY((wfilename = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(filename),DeeUtf8String_STR(filename))) == NULL) {
#ifndef TRY_CREATE
  DeeError_HandledOne();
#endif
  return FALSE;
 }
#ifdef TRY_CREATE
 result = DeeWin32SysFileFD_DoWideTryInitObject(self,wfilename,dwDesiredAccess,
                                                dwCreationDisposition,dwFlagsAndAttributes);
#else
 result = DeeWin32SysFileFD_DoWideInitObject(self,wfilename,dwDesiredAccess,
                                             dwCreationDisposition,dwFlagsAndAttributes);
#endif
 Dee_DECREF(wfilename);
 return result;
}
#ifdef TRY_CREATE
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8TryInit
#else
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8Init
#endif
(struct DeeWin32SysFileFD *self, Dee_Utf8Char const *filename,
 DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
 DeeObject *wfilename; BOOL result;
 if ((wfilename = DeeWideString_FromUtf8String(filename)) == NULL) {
#ifndef TRY_CREATE
  DeeError_HandledOne();
#endif
  return FALSE;
 }
#ifdef TRY_CREATE
 result = DeeWin32SysFileFD_DoWideTryInitObject(self,wfilename,dwDesiredAccess,
                                                dwCreationDisposition,dwFlagsAndAttributes);
#else
 result = DeeWin32SysFileFD_DoWideInitObject(self,wfilename,dwDesiredAccess,
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

