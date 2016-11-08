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
#include "fs_api.c"
#define WIDE
DEE_DECL_BEGIN
#endif
#ifdef WIDE
#define DeeFS_F(x)                 DeeFS_Wide##x
#define _DeeFS_F(x)                _DeeFS_Wide##x
#define DeeVFS_F(x)                DeeVFS_Wide##x
#define _DeeVFS_F(x)               _DeeVFS_Wide##x
#define WIN32_F(x)                 x##W
#define DeeString_F(x)             DeeWideString_##x
#define DEE_STRINGOBJECT           DeeWideStringObject
#define DEE_CHAR                   Dee_WideChar
#define DEE_STRLEN                 Dee_WideStrLen
#define DEE_STRINGWRITER           DeeWideStringWriter
#define DeeStringWriter_F(x)       DeeWideStringWriter_##x
#if DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#define DEE_PRIVATE_CHAR_C(x)      L##x
#define DEE_CHAR_C(x)              DEE_PRIVATE_CHAR_C(x)
#else
#define DEE_CHAR_C(x)              (Dee_WideChar)x
#endif
#define DEE_EMPTYSTRING            Dee_EmptyWideString
#define DEERETURN_EMPTYSTRING      DeeReturn_EmptyWideString
#define DEERETURN_STATIC_STRING_EX DeeReturn_STATIC_WIDE_STRING_EX
#define DEE_PRINTF_STR             "%ls"
#define DEE_PRINTF_STRQ            "L%lq"
#else
#define DeeFS_F(x)                 DeeFS_Utf8##x
#define _DeeFS_F(x)                _DeeFS_Utf8##x
#define DeeVFS_F(x)                DeeVFS_Utf8##x
#define _DeeVFS_F(x)               _DeeVFS_Utf8##x
#define WIN32_F(x)                 x##A
#define DeeString_F(x)             DeeUtf8String_##x
#define DEE_STRINGOBJECT           DeeUtf8StringObject
#define DEE_CHAR                   Dee_Utf8Char
#define DEE_STRLEN                 Dee_Utf8StrLen
#define DEE_CHAR_C(x)              x
#define DEE_STRINGWRITER           DeeUtf8StringWriter
#define DeeStringWriter_F(x)       DeeUtf8StringWriter_##x
#define DEE_EMPTYSTRING            Dee_EmptyUtf8String
#define DEERETURN_EMPTYSTRING      DeeReturn_EmptyUtf8String
#define DEERETURN_STATIC_STRING_EX DeeReturn_STATIC_UTF8_STRING_EX
#define DEE_PRINTF_STR             "%s"
#define DEE_PRINTF_STRQ            "%q"
#endif

#define DEE_STRINGWRITER_INIT    DeeStringWriter_F(INIT)
#define DEE_STRINGWRITER_Pack    DeeStringWriter_F(Pack)
#define DEE_STRINGWRITER_Quit    DeeStringWriter_F(Quit)
#define DEE_STRINGWRITER_SIZE    DeeStringWriter_F(SIZE)
#define DEE_STRINGWRITER_ELEM    DeeStringWriter_F(ELEM)
#define DEE_STRINGWRITER_STR     DeeStringWriter_F(STR)
#define DEE_STRING_Cast          DeeString_F(Cast)
#define DEE_STRING_Check         DeeString_F(Check)
#define DEE_STRING_InplaceCast   DeeString_F(InplaceCast)
#define DEE_STRING_New           DeeString_F(New)
#define DEE_STRING_NewSized      DeeString_F(NewSized)
#define DEE_STRING_NewWithLength DeeString_F(NewWithLength)
#define DEE_STRING_Resize        DeeString_F(Resize)
#define DEE_STRING_SIZE          DeeString_F(SIZE)
#define DEE_STRING_STR           DeeString_F(STR)




#ifndef DEE_FS_FILEIO_INITOBJECT_DEFINED
#define DEE_FS_FILEIO_INITOBJECT_DEFINED 1
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8InitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideInitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
#endif

#ifdef WIDE
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideInitObjectEx
#else
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8InitObjectEx
#endif
(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 // Parse the open mode
 DEE_ASSERT(DeeObject_Check(file) && DEE_STRING_Check(file));
 if (DeeFile_ParseMode(mode,&self->fio_mode) != 0) return -1;
 _DeeFile_Init(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(DEE_STRING_STR(file))) {
  struct DeeVFSFile vfs_file; int temp; DeeObject *rfs_path;
  if ((temp = DeeVFS_F(LocateOrReadReFsLink)(
   DEE_STRING_STR(file),&vfs_file,&rfs_path)) < 0) return -1;
  if (temp == 1) {
   DeeError_SetStringf(&DeeErrorType_FileNotFound,
                       "Could not find virtual file: %r:%q",
                       file,mode);
   return -1;
  }
  if (rfs_path) {
#ifdef WIDE
   temp = _DeeFileIO_WideInitObjectEx(self,rfs_path,mode,permissions);
#else
   temp = _DeeFileIO_Utf8InitObjectEx(self,rfs_path,mode,permissions);
#endif
   Dee_DECREF(rfs_path);
   return temp;
  }
  if (!vfs_file.vf_open) {
   DeeError_SetStringf(&DeeErrorType_IOError,
                       "Cannot open virtual file: %r:%q",
                       file,mode);
   return -1;
  }
#ifdef WIDE
  Dee_INCREF(self->fio_widefile = (DEE_STRINGOBJECT *)file);
#else
  Dee_INCREF(self->fio_utf8file = (DEE_STRINGOBJECT *)file);
#endif
  if ((*vfs_file.vf_open)(&vfs_file,self,self->fio_mode) != 0) {
   Dee_DECREF(self->fio_utf8file);
   return -1;
  }
  return 0;
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD disposition,access,attr;
  DEE_STATIC_ASSERT(DEE_FILEIO_INVALID_HANDLE == (void *)INVALID_HANDLE_VALUE);
  switch (self->fio_mode&DEE_FILE_MASK_BASIC_MODE) {
   case DEE_FILE_MODE_READ: disposition = OPEN_EXISTING; access = GENERIC_READ; break;
   case DEE_FILE_MODE_WRITE: disposition = CREATE_ALWAYS; access = GENERIC_WRITE; break;
   case DEE_FILE_MODE_APPEND: disposition = OPEN_ALWAYS; access = GENERIC_WRITE; break;
   case DEE_FILE_MODE_READ|DEE_FILE_FLAG_UPDATE: disposition = OPEN_EXISTING; access = GENERIC_READ|GENERIC_WRITE; break;
   case DEE_FILE_MODE_WRITE|DEE_FILE_FLAG_UPDATE: disposition = CREATE_ALWAYS; access = GENERIC_READ|GENERIC_WRITE; break;
   case DEE_FILE_MODE_APPEND|DEE_FILE_FLAG_UPDATE: disposition = OPEN_ALWAYS; access = GENERIC_READ|GENERIC_WRITE; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#if 0 /*< Not a good idea... (data leakage and such) */
  DEE_ATOMIC_ONCE({
   // Required for taking advantage of the performance boost gained by 'SetFileValidData'
   _Dee_Win32AcquireProcessPrivilege(L"SeManageVolumePrivilege");
  });
#endif
  attr = FILE_ATTRIBUTE_NORMAL;
  // Make it readonly if there are no write permissions
  if ((permissions&0444)==0) attr |= FILE_ATTRIBUTE_READONLY;
  self->fio_handle = WIN32_F(CreateFile)(
   DEE_STRING_STR(file),access,FILE_SHARE_READ,NULL,disposition,attr,NULL);
  if (self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   int temp,error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_F(Win32IsPathUNC)(DEE_STRING_STR(file))) {
    file = DeeFS_F(Win32PathUNC)(DEE_STRING_STR(file));
    error = _DeeFileIO_WideInitObjectEx(self,file,mode,permissions);
    Dee_DECREF(file);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   switch (error) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_BAD_NETPATH:not_found:
     DeeError_SetStringf(&DeeErrorType_FileNotFound,
                         DEE_PP_STR(WIN32_F(CreateFile))
                         "(%r:%q) : %K",file,mode,
                         DeeSystemError_Win32ToString((unsigned long)error));
     break;

    case ERROR_ACCESS_DENIED:
     // This might be caused by 'file' being a folder
     // Which is something that deemon actually allows
     if ((temp = _DeeFS_F(IsDir)(DEE_STRING_STR(file))) < 0) return -1;
     else if (temp) goto no_err;
     if (error == ERROR_PATH_NOT_FOUND) goto not_found;
     DEE_ATTRIBUTE_FALLTHROUGH

    default:
     DeeError_SetStringf(&DeeErrorType_IOError,
                         DEE_PP_STR(WIN32_F(CreateFile))
                         "(%r:%q) : %K",file,mode,
                         DeeSystemError_Win32ToString((unsigned long)error));
     break;
   }
   return -1;
  }
no_err:
#ifdef WIDE
  Dee_INCREF(self->fio_widefile = (DEE_STRINGOBJECT *)file);
#else
  Dee_INCREF(self->fio_utf8file = (DEE_STRINGOBJECT *)file);
#endif
#elif defined(WIDE)
 DeeObject *utf8file; int result;
 if ((utf8file = DeeUtf8String_Cast(file)) == NULL) return -1;
 result = _DeeFileIO_Utf8InitObjectEx(self,utf8file,mode,permissions);
 Dee_DECREF(utf8file);
 if (result == 0) {
  Dee_DECREF(self->fio_utf8file);
  Dee_INCREF(self->fio_widefile = (DeeWideStringObject *)file);
 }
 return result;
#elif defined(DEE_PLATFORM_UNIX)
  int open_mode;
  switch (self->fio_mode&DEE_FILE_MASK_BASIC_MODE) {
   case DEE_FILE_MODE_READ: open_mode = O_RDONLY; break;
   case DEE_FILE_MODE_WRITE: open_mode = O_WRONLY|O_CREAT|O_TRUNC; break;
   case DEE_FILE_MODE_APPEND: open_mode = O_WRONLY|O_CREAT|O_APPEND; break;
   case DEE_FILE_MODE_READ|DEE_FILE_FLAG_UPDATE: open_mode = O_RDWR; break;
   case DEE_FILE_MODE_WRITE|DEE_FILE_FLAG_UPDATE: open_mode = O_RDWR|O_CREAT|O_TRUNC; break;
   case DEE_FILE_MODE_APPEND|DEE_FILE_FLAG_UPDATE: open_mode = O_RDWR|O_CREAT|O_APPEND; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  self->fio_handle = open(DeeUtf8String_STR(file),open_mode,permissions);
  if (self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   int error;
   switch ((error = DeeSystemError_Consume())) {
    case EISDIR:
    case ENOTDIR:
    case ENOENT:
     DeeError_SetStringf(&DeeErrorType_FileNotFound,
                         "open(%r:%q) : %K",file,mode,
                         DeeSystemError_ToString(error));
     break;
    default:
     DeeError_SetStringf(&DeeErrorType_IOError,
                         "open(%r:%q) : %K",file,mode,
                         DeeSystemError_ToString(error));
     break;
   }
   return -1;
  }
  Dee_INCREF(self->fio_utf8file = (DeeUtf8StringObject *)file);
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  char const *open_mode;
  (void)permissions;
  switch (self->fio_mode&DEE_FILE_MASK_BASIC_MODE) {
   case DEE_FILE_MODE_READ: open_mode = "r"; break;
   case DEE_FILE_MODE_WRITE: open_mode = "w"; break;
   case DEE_FILE_MODE_APPEND: open_mode = "a"; break;
   case DEE_FILE_MODE_READ|DEE_FILE_FLAG_UPDATE: open_mode = "r+"; break;
   case DEE_FILE_MODE_WRITE|DEE_FILE_FLAG_UPDATE: open_mode = "w+"; break;
   case DEE_FILE_MODE_APPEND|DEE_FILE_FLAG_UPDATE: open_mode = "a+"; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  self->fio_handle = (void *)fopen(DeeUtf8String_STR(file),open_mode);
  if (self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   DeeError_SetStringf(&DeeErrorType_IOError,
                       "fopen(%r,%q) : %K",file,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  Dee_INCREF(self->ob_file = (DeeUtf8StringObject *)file);
#endif
 }
 return 0;
}


#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_F(Win32PathUNC)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef WIDE
 DeeObject *result; Dee_size_t path_size; DEE_CHAR *iter,*end;
 DEE_ASSERT(path && "Invalid path");
 DEE_ASSERT(!DeeFS_WideWin32IsPathUNC(path) && "Already a UNC path");
 path_size = Dee_WideStrLen(path);
 if ((result = DeeWideString_NewSized(4+path_size)) == NULL) return NULL;
 iter = DEE_STRING_STR(result);
 // prepend UNC prefix
 *iter++ = DEE_CHAR_C('\\');
 *iter++ = DEE_CHAR_C('\\');
 *iter++ = DEE_CHAR_C('?');
 *iter++ = DEE_CHAR_C('\\');
 memcpy(iter,path,path_size*sizeof(DEE_CHAR));
 end = iter+path_size;
 // Fix slashes
 while (iter != end) {
  if (*iter == '/') *iter = '\\';
  ++iter;
 }
 return result;
#else
 DeeObject *result,*widepath;
 if ((widepath = DeeWideString_FromUtf8String(path)) == NULL) return NULL;
 result = DeeFS_WideWin32PathUNC(DeeWideString_STR(widepath));
 Dee_DECREF(widepath);
 return result;
#endif
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(Win32GetModuleName)(DEE_A_IN_OPT /*HMODULE*/void *module) {
 DeeObject *result; DWORD temp; Dee_size_t bufsize;
 if ((result = DEE_STRING_NewSized((
  bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32GETMODULENAME))) == NULL) return NULL;
again:
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 temp = WIN32_F(GetModuleFileName)((
  HMODULE)module,DEE_STRING_STR(result),(DWORD)bufsize);
 if (temp == 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetModuleFileName)) "(%p) : %K",module,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_r:
  Dee_DECREF(result);
  return NULL;
 } else if (temp < bufsize) {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else if (temp == bufsize && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
  if (DEE_STRING_Resize(&result,(bufsize *= 2)) != 0) goto err_r;
  goto again;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32HardLink)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DEE_ASSERT(src && dst);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(CreateHardLink)(dst,src,NULL)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#ifdef WIDE
  if (DEE_FS_WIN32_IS_UNC_ERROR(error)) {
   DeeObject *temp;
   if (!DeeFS_WideWin32IsPathUNC(src)) {
    if DEE_UNLIKELY((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
    error = _DeeFS_WideWin32HardLink(DeeWideString_STR(temp),dst);
    goto unc_end0;
   }
   if (!DeeFS_WideWin32IsPathUNC(dst)) {
    if DEE_UNLIKELY((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
    error = _DeeFS_WideWin32HardLink(src,DeeWideString_STR(temp));
unc_end0: Dee_DECREF(temp);
    return error;
   }
  }
#else
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     (!DeeFS_Utf8Win32IsPathUNC(src) ||
      !DeeFS_Utf8Win32IsPathUNC(dst))) {
   DeeObject *unc_src,*unc_dst;
   if ((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
    ? DeeFS_Utf8Win32PathUNC(src)
    : DeeWideString_FromUtf8String(src)) == NULL) return -1;
   if ((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
    ? DeeFS_Utf8Win32PathUNC(dst)
    : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
   error = _DeeFS_WideWin32HardLink(
    DeeWideString_STR(unc_src),DeeWideString_STR(unc_dst));
   Dee_DECREF(unc_dst);
err_unc0: Dee_DECREF(unc_src);
   return error;
  }
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(CreateHardLink))
                      "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",
                      dst,src,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(Win32HardLink)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if ((dst_ob = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(Win32HardLink)(DEE_STRING_STR(src_ob),DEE_STRING_STR(dst_ob));
 Dee_DECREF(dst_ob);
err_0: Dee_DECREF(src_ob);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeFS_F(Win32Drives)(void) {
 DEE_CHAR *buffer,*new_buffer,*iter,*end;
 DeeObject *result,*entry;
 DWORD req_bufsize,used_bufsize; int error;
 while ((buffer = (DEE_CHAR *)malloc_nz(
  (DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES+1)*sizeof(DEE_CHAR))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
  DeeError_NoMemory();
  return NULL;
 }
 used_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES;
again:
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) { free_nn(buffer); return NULL; }
 req_bufsize = WIN32_F(GetLogicalDriveStrings)(used_bufsize,buffer);
 if (!req_bufsize) {
  free_nn(buffer);
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetLogicalDriveStrings)) "() : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 if (req_bufsize > DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES) {
  while ((new_buffer = (DEE_CHAR *)realloc_nnz(
   buffer,(req_bufsize+1)*sizeof(DEE_CHAR))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   free_nn(buffer);
   goto no_mem;
  }
  buffer = new_buffer;
  used_bufsize = req_bufsize;
  goto again;
 }
 buffer[req_bufsize] = DEE_CHAR_C('\0');
 end = (iter = buffer)+req_bufsize;
 if ((result = DeeList_New()) == NULL) goto end;
 while (iter < end) {
  if (!*iter) break;
  if ((entry = DEE_STRING_New(iter)) == NULL) {
err_r: Dee_CLEAR(result); goto end;
  }
  iter += DEE_STRLEN(iter)+1;
  error = DeeList_Append(result,entry);
  Dee_DECREF(entry);
  if (error != 0) goto err_r;
 }
end:
 free_nn(buffer);
 return result;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4055)
static DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(Win32GetTokenUserHome)(DEE_A_IN HANDLE token) {
 int error; DeeObject *result; DWORD req_size = 0;
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
 if (!WIN32_F(p_GetUserProfileDirectory)) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetUserProfileDirectory))
                      "(...) : Not implemented");
  return NULL;
 }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if (!(*WIN32_F(p_GetUserProfileDirectory))(token,NULL,&req_size)) {
  error = (int)DeeSystemError_Win32Consume();
  if (error != ERROR_INSUFFICIENT_BUFFER) {
err_api:
   DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetUserProfileDirectoryW)),(DWORD)error);
   return NULL;
  }
 }
 if ((result = DEE_STRING_NewSized(req_size-1)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) { Dee_DECREF(result); return NULL; }
 if (!(*WIN32_F(p_GetUserProfileDirectory))(
  token,DEE_STRING_STR(result),&req_size)) {
  Dee_DECREF(result);
  error = (int)DeeSystemError_Win32Consume();
  goto err_api;
 }
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP

static DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(Win32GetProcessUserHome)(DEE_A_IN HANDLE process_handle) {
 DeeObject *result; HANDLE process_token;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if (!OpenProcessToken(process_handle,TOKEN_QUERY,&process_token)) {
  DeeError_SystemError("OpenProcessToken");
  return NULL;
 }
 result = DeeFS_F(Win32GetTokenUserHome)(process_token);
 if (!CloseHandle(process_token)) SetLastError(0);
 return result;
}

static DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32FGetFileType)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN HANDLE handle) {
 DWORD error,ftype;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if ((ftype = GetFileType(handle)) == 0 && (error = DeeSystemError_Win32Consume()) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetFileType(%p:" DEE_PRINTF_STRQ ") : %K",
                      handle,path,DeeSystemError_Win32ToString(error));
  return -1;
 }
 return (int)ftype;
}
static DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32GetFileType)(
 DEE_A_IN_Z DEE_CHAR const *path) {
 DWORD error; int ftype; HANDLE handle;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if ((handle = WIN32_F(CreateFile)(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
  FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL)) != INVALID_HANDLE_VALUE) {
  ftype = _DeeFS_F(Win32FGetFileType)(path,handle);
  if (!CloseHandle(handle)) SetLastError(0);
  return ftype;
 }
 error = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
 if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
    !DeeFS_F(Win32IsPathUNC)(path)) {
  DeeObject *temp;
  if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
  ftype = _DeeFS_WideWin32GetFileType(DeeWideString_STR(temp));
  Dee_DECREF(temp);
  return ftype;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
 if (error == ERROR_ACCESS_DENIED && _DeeFS_F(IsDir)(path)) return 0;
 if (WIN32_IS_FILE_NOT_FOUND_ERROR(error)) return 0;
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     DEE_PP_STR(WIN32_F(CreateFile))
                     "(" DEE_PRINTF_STRQ ") : %K",
                     path,DeeSystemError_Win32ToString(error));
 return -1;
}
#endif


DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(GetHome)(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result,*homedrive,*homepath;
 static DEE_CHAR const var_HOME[] = {'H','O','M','E',0};
 static DEE_CHAR const var_USERPROFILE[] = {'U','S','E','R','P','R','O','F','I','L','E',0};
 static DEE_CHAR const var_HOMEDRIVE[] = {'H','O','M','E','D','R','I','V','E',0};
 static DEE_CHAR const var_HOMEPATH[] = {'H','O','M','E','P','A','T','H',0};
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DeeFS_F(TryGetEnv)(var_HOME)) != NULL) return result;
 if ((result = DeeFS_F(TryGetEnv)(var_USERPROFILE)) != NULL) return result;
 if ((homedrive = DeeFS_F(TryGetEnv)(var_HOMEDRIVE)) == NULL) goto win_home_api;
 if ((homepath = DeeFS_F(TryGetEnv)(var_HOMEPATH)) == NULL) { Dee_DECREF(homedrive); goto win_home_api; }
 result = DeeString_F(ConcatWithLength)(homedrive,DEE_STRING_SIZE(homepath),DEE_STRING_STR(homepath));
 Dee_DECREF(homepath);
 Dee_DECREF(homedrive);
 if (!result) return NULL;
 homepath = DeeFS_PathIncTrailObject(result); // Make sure we have a trailing backslash
 Dee_DECREF(result);
 return homepath;
win_home_api:
 return DeeFS_F(Win32GetProcessUserHome)(GetCurrentProcess());
#elif defined(WIDE)
 DeeObject *result;
 if ((result = DeeFS_Utf8GetHome()) == NULL) return NULL;
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H
 DeeObject *result;
 struct passwd *pw;
 if ((result = DeeFS_Utf8TryGetEnv("HOME")) != NULL) return result;
 if ((pw = getpwuid(getuid())) == NULL) {
  DeeError_SystemError("getpwuid");
  return NULL;
 }
 if (!pw->pw_dir) DeeReturn_EmptyUtf8String;
 return DeeUtf8String_New(pw->pw_dir);
#else
 return DeeFS_Utf8GetEnv("HOME");
#endif
#else
 DeeReturn_STATIC_UTF8_STRING("~"); // Of course!
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(GetHomeUser)(DEE_A_IN_Z DEE_CHAR const *user) {
#ifdef DEE_PLATFORM_WINDOWS
 (void)user; // TODO?
 return DeeFS_F(GetHome)();
#elif defined(WIDE)
 DeeObject *user_ob,*result;
 if ((user_ob = DeeUtf8String_FromWideString(user)) == NULL) return NULL;
 result = DeeFS_Utf8GetHomeUser(DeeUtf8String_STR(user_ob));
 Dee_DECREF(user_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 struct passwd *pd;
 pd = getpwnam(user); // TODO: getpwnam_r
 if (!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(" DEE_PRINTF_STRQ ") : %K",user,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return NULL;
 }
 if (!pd->pw_dir) DeeReturn_EmptyString; // Lets be careful...
 return DeeUtf8String_New(pd->pw_dir);
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(GetTmp)(void) {
 DeeObject *result;
 static DEE_CHAR const name_TMPDIR[] = {'T','M','P','D','I','R','\0'};
 static DEE_CHAR const name_TEMP[] = {'T','E','M','P','\0'};
 static DEE_CHAR const name_TMP[] = {'T','M','P','\0'};
 if ((result = DeeFS_F(TryGetEnv)(name_TMPDIR)) != NULL) return result;
 if ((result = DeeFS_F(TryGetEnv)(name_TEMP)) != NULL) return result;
 if ((result = DeeFS_F(TryGetEnv)(name_TMP)) != NULL) return result;
#ifdef DEE_PLATFORM_WINDOWS
 {DWORD temp;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
  if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP)) == NULL) return NULL;
  temp = WIN32_F(GetTempPath)(DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP+1,DEE_STRING_STR(result));
  if (!temp) {
   int error;
err:
   error = (int)DeeSystemError_Win32Consume();
   Dee_DECREF(result);
   if (!error) DEERETURN_EMPTYSTRING;
   DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetTempPathW)),(DWORD)error);
   return NULL;
  }
  if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP) {
again:
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
err_r: Dee_DECREF(result); return NULL;
   }
   temp = WIN32_F(GetTempPath)(temp+1,DEE_STRING_STR(result));
   if (!temp) goto err;
   if (temp > DEE_STRING_SIZE(result)) goto again;
   if (temp < DEE_STRING_SIZE(result))
    if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  } else {
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  }
 }
 return result;
#elif defined(WIDE) && defined(_wP_tmpdir)
 return DeeWideString_New(_wP_tmpdir);
#elif defined(WIDE)
 if ((result = DeeFS_Utf8GetTmp()) == NULL) return NULL;
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
#elif defined(P_tmpdir)
 // NOTE: According to this:
 // http://www.gnu.org/software/libc/manual/html_node/Temporary-Files.html#index-P_005ftmpdir-1608
 // 'P_tmpdir' is a 'char const *', meaning we can't 'sizeof()' its length
 // (And while we probably still could, lets better not count on that...)
 return DeeUtf8String_New(P_tmpdir);
#else
 DeeReturn_STATIC_UTF8_STRING("/tmp");
#endif
}

#ifdef WIDE
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *) _dee_fs_wide_gen_initial_tmpname
#else
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *) _dee_fs_utf8_gen_initial_tmpname
#endif
(DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix, DEE_A_OUT DEE_CHAR **digits) {
 static DEE_CHAR const name_0000temp[] = {'0','0','0','0','.','t','m','p',0};
 DeeObject *result; Dee_size_t digit_offset,path_len;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 DEE_ASSERT(path && prefix && digits);
 path_len = DEE_STRLEN(path);
 while (path_len && IS_SEP(path[path_len-1])) --path_len;
 if (DeeStringWriter_F(WriteMyStringWithLength)(&writer,path_len,path) != 0 ||
     DeeStringWriter_F(WriteMyChar)(&writer,SEP) != 0 ||
     DeeStringWriter_F(WriteMyString)(&writer,prefix) != 0 ||
     DeeStringWriter_F(WRITE_MY_STRING)(&writer,name_0000temp) != 0) goto err;
 digit_offset = DeeStringWriter_F(SIZE)(&writer)-8;
 result = DEE_STRINGWRITER_Pack(&writer);
 if (result) *digits = DEE_STRING_STR(result)+digit_offset;
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
#ifdef WIDE
DEE_STATIC_INLINE(int) _deefs_wide_inc_digit
#else
DEE_STATIC_INLINE(int) _deefs_utf8_inc_digit
#endif
(DEE_A_INOUT DEE_CHAR *digit) {
 DEE_ASSERT(digit);
 switch (*digit) {
  case '0': case '1': case '2': case '3':
  case '4': case '5': case '6': case '7':
  case '8': case 'A': case 'B': case 'C':
  case 'D': case 'E': ++*digit; break;
  case '9': *digit = 'A'; break;
  case 'F': *digit = '0'; return 1;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return 0;
}

#ifdef WIDE
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _deefs_wide_inc_tmpname
#else
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _deefs_utf8_inc_tmpname
#endif
(DEE_CHAR *digits) {
#ifdef WIDE
 if (_deefs_wide_inc_digit(digits+3)) if (_deefs_wide_inc_digit(digits+2))
 if (_deefs_wide_inc_digit(digits+1)) if (_deefs_wide_inc_digit(digits+0))
#else
 if (_deefs_utf8_inc_digit(digits+3)) if (_deefs_utf8_inc_digit(digits+2))
 if (_deefs_utf8_inc_digit(digits+1)) if (_deefs_utf8_inc_digit(digits+0))
#endif
 {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "tmpname(): Too many temporary files");
  return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *_DeeFS_F(GetTmpName)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix) {
 DeeObject *result; DEE_CHAR *digits;
#ifdef WIDE
 if ((result = _dee_fs_wide_gen_initial_tmpname(path,prefix,&digits)) == NULL) return NULL;
#else
 if ((result = _dee_fs_utf8_gen_initial_tmpname(path,prefix,&digits)) == NULL) return NULL;
#endif
 while (_DeeFS_F(IsFile)(DEE_STRING_STR(result))) {
#ifdef WIDE
  if (_deefs_wide_inc_tmpname(digits) != 0)
#else
  if (_deefs_utf8_inc_tmpname(digits) != 0)
#endif
  {
   Dee_DECREF(result);
   return NULL;
  }
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(GetTmpName)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix) {
 DeeObject *path_ob,*result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 result = _DeeFS_F(GetTmpName)(DEE_STRING_STR(path_ob),prefix);
 Dee_DECREF(path_ob);
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_PREFAST_WARNING_PUSH(6387)
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(Win32GetDllDirectory)(void) {
#ifdef WIDE
 typedef DWORD (WINAPI *LPGETDLLDIRECTORYW)(DWORD nBufferLength, LPWSTR lpBuffer);
 static LPGETDLLDIRECTORYW p_GetDllDirectoryW = NULL;
 static char const s_GetDllDirectoryW[] = "GetDllDirectoryW";
#else
 typedef DWORD (WINAPI *LPGETDLLDIRECTORYA)(DWORD nBufferLength, LPSTR lpBuffer);
 static LPGETDLLDIRECTORYA p_GetDllDirectoryA = NULL;
 static char const s_GetDllDirectoryA[] = "GetDllDirectoryA";
#endif
 DWORD temp; DeeObject *result;
 DEE_ATOMIC_ONCE({
  WIN32_F(p_GetDllDirectory) = (WIN32_F(LPGETDLLDIRECTORY))GetProcAddress(
   GetModuleHandle(TEXT("KERNEL32")),WIN32_F(s_GetDllDirectory));
 });
 if (!WIN32_F(p_GetDllDirectory)) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetDllDirectory))
                      "(...) : Not implemented");
  return NULL;
 }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY)) == NULL) return NULL;
 temp = (*WIN32_F(p_GetDllDirectory))(DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY+1,DEE_STRING_STR(result));
 if (!temp) {
  int error;
err:
  error = (int)DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetDllDirectoryW)),(DWORD)error);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = (*WIN32_F(p_GetDllDirectory))(temp+1,DEE_STRING_STR(result));
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(Win32GetSystemDirectory)(void) {
 DWORD temp; DeeObject *result; int error;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY)) == NULL) return NULL;
 if ((temp = WIN32_F(GetSystemDirectory)(
  DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY)) == 0) {
err: error = (int)DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetSystemDirectoryW)),(DWORD)error);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetSystemDirectory)(DEE_STRING_STR(result),temp);
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(Win32GetWindowsDirectory)(void) {
 DWORD temp; DeeObject *result; int error;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetWindowsDirectory)(DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY);
 if (!temp) {
err: error = (int)DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetWindowsDirectory)),(DWORD)error);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetWindowsDirectory)(DEE_STRING_STR(result),temp);
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(Win32GetSystemWindowsDirectory)(void) {
 DWORD temp; DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetSystemWindowsDirectory)(DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY);
 if (!temp) {
  int error;
err:
  error = (int)DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetSystemWindowsDirectoryW)),(DWORD)error);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
  temp = WIN32_F(GetSystemWindowsDirectory)(DEE_STRING_STR(result),temp);
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(Win32GetSysWow64Directory)(void) {
 DWORD temp; DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY)) == NULL) return NULL;
 temp = WIN32_F(GetSystemWow64Directory)(DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY);
 if (!temp) {
  int error;
err:
  error = (int)DeeSystemError_Win32Consume();
  Dee_DECREF(result);
  if (!error) DEERETURN_EMPTYSTRING;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetSystemWow64Directory)),(DWORD)error);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {err_r: Dee_DECREF(result); return NULL; }
  temp = WIN32_F(GetSystemWow64Directory)(DEE_STRING_STR(result),temp);
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
}

static DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32FSetFileTime)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN HANDLE handle, DEE_A_IN_OPT FILETIME const *tm_access,
 DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!SetFileTime(handle,tm_creation,tm_access,tm_modification)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetFileTime(%p:" DEE_PRINTF_STRQ ",%I64u,%I64u,%I64u) : %K",
                      handle,path,
                      tm_creation ? *(Dee_uint64_t*)tm_creation : (Dee_uint64_t)0,
                      tm_access ? *(Dee_uint64_t*)tm_access : (Dee_uint64_t)0,
                      tm_modification ? *(Dee_uint64_t*)tm_modification : (Dee_uint64_t)0,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32SetFileTime)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_OPT FILETIME const *tm_access,
 DEE_A_IN_OPT FILETIME const *tm_creation, DEE_A_IN_OPT FILETIME const *tm_modification) {
 DWORD error; int result;
 HANDLE handle;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if ((handle = WIN32_F(CreateFile)(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|
  FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_GENERIC_WRITE,NULL)) != INVALID_HANDLE_VALUE) {
  result = _DeeFS_F(Win32FSetFileTime)(path,handle,tm_access,tm_creation,tm_modification);
  if (!CloseHandle(handle)) SetLastError(0);
  return result;
 }
 error = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
 if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
    !DeeFS_F(Win32IsPathUNC)(path)) {
  DeeObject *temp;
  if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
  result = _DeeFS_WideWin32SetFileTime(
   DeeWideString_STR(temp),tm_access,tm_creation,tm_modification);
  Dee_DECREF(temp);
  return result;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     DEE_PP_STR(WIN32_F(CreateFile))
                     "(" DEE_PRINTF_STRQ ") : %K",
                     path,DeeSystemError_Win32ToString(error));
 return -1;
}
#endif






//////////////////////////////////////////////////////////////////////////
// Current working directory get/set
// NOTE: 'path' may be relative, allowing you to navigate from the current cwd
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(GetCwd)(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DWORD temp; DeeObject *result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD)) == NULL) return NULL;
 temp = WIN32_F(GetCurrentDirectory)(DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD+1,DEE_STRING_STR(result));
 if (!temp) {
err: DeeError_SystemError(DEE_PP_STR(WIN32_F(GetCurrentDirectory)));
err_r: Dee_DECREF(result);
  return NULL;
 }
 if (temp > DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD) {
again:
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
  temp = WIN32_F(GetCurrentDirectory)(temp+1,DEE_STRING_STR(result));
  if (!temp) goto err;
  if (temp > DEE_STRING_SIZE(result)) goto again;
  if (temp < DEE_STRING_SIZE(result))
   if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else {
  if (DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 }
 return result;
#elif defined(WIDE)
 DeeObject *result;
 if ((result = DeeFS_Utf8GetCwd()) == NULL) return NULL;
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 Dee_size_t bufsize,last_size = 0;
 DeeObject *result;
 Dee_Utf8Char *cwd_start;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 if ((result = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) == NULL) return NULL;
 if (!getcwd(DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) {
  if (_dee_posix_check_getcwd_error() != 0) {err_r: Dee_DECREF(result); return NULL; }
  if (DeeUtf8String_Resize(&result,(bufsize = (last_size = DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)*2)) != 0) goto err_r;
  while (!getcwd(DeeUtf8String_STR(result),bufsize)) {
   Dee_size_t new_size;
   if (_dee_posix_check_getcwd_error() != 0) goto err_r;
   if (DeeUtf8String_Resize(&result,(new_size = bufsize*2)) != 0) goto err_r;
   last_size = bufsize,bufsize = new_size;
  }
 }
 cwd_start = DeeUtf8String_STR(result)+last_size;
 while (*cwd_start) ++cwd_start;
 last_size = (Dee_size_t)(cwd_start-DeeUtf8String_STR(result));
 bufsize = DeeUtf8String_SIZE(result);
 if (last_size != bufsize && DeeUtf8String_Resize(&result,last_size) != 0) goto err_r;
 return result;
#else
 DeeReturn_STATIC_STRING("."); // Yes!
#endif
}



DEE_A_RET_EXCEPT(-1) int _DeeFS_F(ChDir)(
 DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *rfs_path; int result;
  if ((rfs_path = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(ChDir)(DEE_STRING_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(SetCurrentDirectory)(path)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *temp;
   if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   error = _DeeFS_WideChDir(DeeWideString_STR(temp));
   Dee_DECREF(temp);
   return error;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(SetCurrentDirectory))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      path,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8ChDir(DeeUtf8String_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (chdir(path) == -1) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "chdir(" DEE_PRINTF_STRQ ") : %K",path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)path;
 return 0;
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeFS_F(ListEnv)(void) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeObject *result;
#ifdef WIDE
 LPWCH env_vars;
#else
 LPCH env_vars;
#endif
 DEE_CHAR const *line;
 if ((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) { Dee_DECREF(result); return NULL; }
 if ((env_vars = WIN32_F(GetEnvironmentStrings)()) == NULL) {
  Dee_DECREF(result);
  DeeError_SystemError(DEE_PP_STR(WIN32_F(GetEnvironmentStringsA)));
  return NULL;
 }
 for (line = env_vars; *line; line += (DEE_STRLEN(line)+1)) {
  DeeObject *name,*value,*entry;
  DEE_CHAR const *value_begin = line;
  if (*value_begin == DEE_CHAR_C('=')) continue; // Skip internal env vars (like per-drive cwd, and others)
  while (*value_begin && *value_begin != DEE_CHAR_C('=')) ++value_begin;
  if (!*value_begin) continue; // No idea what this is about, but lets be safe
  if ((name = DEE_STRING_NewWithLength((Dee_size_t)(value_begin-line),line)) == NULL) goto err;
  if ((value = DEE_STRING_New(value_begin+1)) == NULL) {err_n: Dee_DECREF(name); goto err; }
  if ((entry = DeeTuple_Pack(2,name,value)) == NULL) {err_nv: Dee_DECREF(value); goto err_n; }
  if (DeeList_Append(result,entry) == -1) goto err_nv;
  Dee_DECREF(entry);
  Dee_DECREF(value);
  Dee_DECREF(name);
 }
end: if (!WIN32_F(FreeEnvironmentStrings)(env_vars)) SetLastError(0);
 return result;
err: Dee_DECREF(result); result = NULL; goto end;
#elif defined(WIDE)
 DeeObject *result,**iter,**end;
 if ((result = DeeFS_Utf8ListEnv()) == NULL) return NULL;
 end = (iter = DeeList_ELEM(result))+DeeList_SIZE(result);
 while (iter != end) if (DeeWideString_InplaceCast((DeeObject const **)iter++) == -1) { Dee_CLEAR(result); break; }
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *result;
 char **env_iter,*env;
 if ((result = DeeList_New()) == NULL) return NULL;
 if (environ) for (env_iter = environ; (env = *env_iter) != NULL; ++env_iter) {
  DeeObject *name,*value,*entry;
  char *value_begin = env;
  while (*value_begin && *value_begin != '=') ++value_begin;
  if ((name = DeeUtf8String_NewWithLength((Dee_size_t)(value_begin-env),env)) == NULL) {err: Dee_DECREF(result); return NULL; }
  if ((value = DeeUtf8String_New(value_begin+1)) == NULL) {err_n: Dee_DECREF(name); goto err; }
  if ((entry = DeeTuple_Pack(2,name,value)) == NULL) {err_nv: Dee_DECREF(value); goto err_n; }
  if (DeeList_Append(result,entry) == -1) { Dee_DECREF(entry); goto err_nv; }
  Dee_DECREF(entry);
  Dee_DECREF(value);
  Dee_DECREF(name);
 }
 return result;
#else
 DeeReturn_EmptyList; // What else could it be?
#endif
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(HasEnv)(DEE_A_IN_Z DEE_CHAR const *name) {
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (WIN32_F(GetEnvironmentVariable)(name,NULL,0) == 0) {
  DWORD error = DeeSystemError_Win32Consume();
  if (error == ERROR_ENVVAR_NOT_FOUND) return 0;
  DeeError_SystemErrorExplicit(DEE_PP_STR(WIN32_F(GetEnvironmentVariable)),error);
 }
 return 1;
#elif defined(WIDE)
 DeeObject *name_ob; int result;
 if ((name_ob = DeeUtf8String_FromWideString(name)) == NULL) return -1;
 result = DeeFS_Utf8HasEnv(DeeUtf8String_STR(name_ob));
 Dee_DECREF(name_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 return getenv(name) ? 1 : (errno = 0,0);
#else
 (void)name;
 return 0;
#endif
}

DEE_A_RET_OBJECT_NOEXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(TryGetEnv)(DEE_A_IN_Z DEE_CHAR const *name) {
#ifdef DEE_PLATFORM_WINDOWS
 DWORD error; DeeObject *result;
 DEE_LVERBOSE1("Lookup env variable: " DEE_PRINTF_STRQ "\n",name);
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETENV)) == NULL) {err_h: DeeError_Handled(); return NULL; }
 error = WIN32_F(GetEnvironmentVariable)(name,DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETENV+1);
 if (error == 0) {not_found: SetLastError(0); Dee_DECREF(result); return NULL; }
 if (error > DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
again:
  if (DEE_STRING_Resize(&result,error-1) == -1) {err_r: Dee_DECREF(result); goto err_h; }
  error = WIN32_F(GetEnvironmentVariable)(name,DEE_STRING_STR(result),error);
  if (error == 0) goto not_found;
  // ** this could happen, if the variable changed between the 2 calls
  if (error > DEE_STRING_SIZE(result)) goto again;
 } else if (error < DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
  if (DEE_STRING_Resize(&result,error) == -1) goto err_r;
 }
 return result;
#elif defined(WIDE)
 DeeObject *name_ob,*result;
 if ((name_ob = DeeUtf8String_FromWideString(name)) == NULL) { DeeError_Handled(); return NULL; }
 result = DeeFS_Utf8TryGetEnv(DeeUtf8String_STR(name_ob));
 Dee_DECREF(name_ob);
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 char const *env_value;
 DEE_LVERBOSE1("Lookup env variable: " DEE_PRINTF_STRQ "\n",name);
 if ((env_value = getenv(name)) == NULL) return NULL;
 return DeeUtf8String_New(env_value);
#else
 (void)name;
 DeeReturn_EmptyString;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(GetEnv)(DEE_A_IN_Z DEE_CHAR const *name) {
#ifdef DEE_PLATFORM_WINDOWS
 DWORD error; DeeObject *result;
 if ((result = DEE_STRING_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32GETENV)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
 DEE_LVERBOSE1("Lookup env variable: " DEE_PRINTF_STRQ "\n",name);
 error = WIN32_F(GetEnvironmentVariable)(name,DEE_STRING_STR(result),DEE_XCONFIG_FSBUFSIZE_WIN32GETENV+1);
 if (error == 0) {
not_found: error = DeeSystemError_Win32Consume();
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetEnvironmentVariable))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      name,DeeSystemError_Win32ToString(error));
  goto err_r;
 }
 if (error > DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
again:
  if (DEE_STRING_Resize(&result,error-1) == -1) {err_r: Dee_DECREF(result); return NULL; }
  error = WIN32_F(GetEnvironmentVariable)(name,DEE_STRING_STR(result),error);
  if (error == 0) goto not_found;
  // ** this could happen, if the variable changed between the 2 calls
  if (error > DEE_STRING_SIZE(result)) goto again;
 } else if (error < DEE_XCONFIG_FSBUFSIZE_WIN32GETENV) {
  if (DEE_STRING_Resize(&result,error) == -1) goto err_r;
 }
 return result;
#elif defined(WIDE)
 DeeObject *result,*name_ob;
 if ((name_ob = DeeUtf8String_FromWideString(name)) == NULL) return NULL;
 result = DeeFS_Utf8GetEnv(DeeUtf8String_STR(name_ob));
 Dee_DECREF(name_ob);
 if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 Dee_Utf8Char const *env_value;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DEE_LVERBOSE1("[SYSCALL] getenv(" DEE_PRINTF_STRQ ")\n",name);
 if ((env_value = getenv(name)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getenv(" DEE_PRINTF_STRQ ") : Env var not found",
                      name);
  return NULL;
 }
 return DeeUtf8String_New(env_value);
#else
 (void)name;
 DeeReturn_EmptyUtf8String;
#endif
}

DEE_A_RET_EXCEPT(-1) int DeeFS_F(DelEnv)(DEE_A_IN_Z DEE_CHAR const *name) {
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DEE_LVERBOSE1("Deleting env variable: " DEE_PRINTF_STRQ "\n",name);
 if (!WIN32_F(SetEnvironmentVariable)(name,NULL)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(SetEnvironmentVariable))
                      "(" DEE_PRINTF_STRQ ",NULL) : %K",name,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 int result; DeeObject *name_ob;
 if ((name_ob = DeeUtf8String_FromWideString(name)) == NULL) return -1;
 result = DeeFS_Utf8DelEnv(DeeUtf8String_STR(name_ob));
 Dee_DECREF(name_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_HAVE_UNSETENV
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DEE_LVERBOSE1("Deleting env variable: " DEE_PRINTF_STRQ "\n",name);
 if (unsetenv(name) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "unsetenv(" DEE_PRINTF_STRQ ") : %K",name,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 // Nothing we can do here.
 // But let's not bicker about this not being implemented...
 (void)name;
 return 0;
#endif
#else
 (void)name;
 return 0;
#endif
}


DEE_A_RET_EXCEPT(-1) int DeeFS_F(SetEnv)(
 DEE_A_IN_Z DEE_CHAR const *name,
 DEE_A_IN_Z DEE_CHAR const *value) {
#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV && defined(WIDE)
 DeeObject *u8name,*u8value;
#endif
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV && defined(WIDE)
 if ((u8name = DeeUtf8String_FromWideString(name)) == NULL) return -1;
 if ((u8value = DeeUtf8String_FromWideString(value)) == NULL) { Dee_DECREF(u8name); return -1; }
#endif
 DEE_LVERBOSE1("Setting env variable: " DEE_PRINTF_STRQ "=" DEE_PRINTF_STRQ "\n",name,value);
 if (!WIN32_F(SetEnvironmentVariable)(name,value)) {
  DWORD error = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV && defined(WIDE)
  Dee_DECREF(u8name);
  Dee_DECREF(u8value);
#endif
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(SetEnvironmentVariable))
                      "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",
                      name,value,DeeSystemError_Win32ToString(error));
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
#ifdef WIDE
 DeeFS_Utf8ReconfigureEnv(DeeUtf8String_STR(u8name),DeeUtf8String_STR(u8value));
 Dee_DECREF(u8name);
 Dee_DECREF(u8value);
#else
 DeeFS_Utf8ReconfigureEnv(name,value);
#endif
#endif
 return 0;
#elif defined(WIDE)
 DeeObject *name_ob,*value_ob; int result;
 if ((name_ob = DeeUtf8String_FromWideString(name)) == NULL) return -1;
 if ((value_ob = DeeUtf8String_FromWideString(value)) == NULL) { result = -1; goto end_0; }
 result = DeeFS_Utf8SetEnv(DeeUtf8String_STR(name_ob),DeeUtf8String_STR(value_ob));
 Dee_DECREF(value_ob);
end_0: Dee_DECREF(name_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX) && DEE_HAVE_SETENV
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DEE_LVERBOSE1("Setting env variable: " DEE_PRINTF_STRQ "=" DEE_PRINTF_STRQ "\n",name,value);
 if (setenv(name,value,1) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "setenv(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",name,value,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
 DeeFS_Utf8ReconfigureEnv(name,value);
#endif
 return 0;
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *put_str;
 if ((put_str = DeeUtf8String_Newf("%s=%s",name,value)) == NULL) return -1;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_putstr;
 DEE_LVERBOSE1("Setting env variable: " DEE_PRINTF_STRQ "=" DEE_PRINTF_STRQ "\n",name,value);
 if (putenv(DeeUtf8String_STR(put_str)) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "putenv(" DEE_PRINTF_STRQ ") : %K",
                      DeeUtf8String_STR(put_str),
                      DeeSystemError_ToString(DeeSystemError_Consume()));
err_putstr:
  Dee_DECREF(put_str);
  return -1;
 }
 Dee_DECREF(put_str);
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
 DeeFS_Utf8ReconfigureEnv(name,value);
#endif
 return 0;
#else
 (void)name,value;
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
 DeeFS_Utf8ReconfigureEnv(name,value);
#endif
 return 0;
#endif
}


DEE_A_RET_EXCEPT(-1) int _DeeFS_F(SetTimes2)(
 DEE_A_IN_Z DEE_CHAR const *path,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) {
 DEE_ASSERT(path);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_F(LocateOrReadReFsLink)(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_F(SetTimes2)(DEE_STRING_STR(
     rfs_path),tm_access,tm_creation,tm_modification);
    Dee_DECREF(rfs_path);
    return temp;
   }
   return 0;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  FILETIME ftm_access,ftm_creation,ftm_modification;
  DEE_ASSERT(!tm_access || (DeeObject_Check(tm_access) && DeeTime_Check(tm_access)));
  DEE_ASSERT(!tm_creation || (DeeObject_Check(tm_creation) && DeeTime_Check(tm_creation)));
  DEE_ASSERT(!tm_modification || (DeeObject_Check(tm_modification) && DeeTime_Check(tm_modification)));
  if (tm_access && DeeTime_AsWin32FileTime(tm_access,&ftm_access) == -1) return -1;
  if (tm_creation && DeeTime_AsWin32FileTime(tm_creation,&ftm_creation) == -1) return -1;
  if (tm_modification && DeeTime_AsWin32FileTime(tm_modification,&ftm_modification) == -1) return -1;
  return _DeeFS_F(Win32SetFileTime)(path,
   tm_access ? &ftm_access : NULL,
   tm_creation ? &ftm_creation : NULL,
   tm_modification ? &ftm_modification : NULL);
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8SetTimes2(DeeUtf8String_STR(path_ob),tm_access,tm_creation,tm_modification);
  Dee_DECREF(path_ob);
  return result;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H
  // NOTE: Under linux we can't set the creation time
  struct utimbuf times;
  int error;
  if (!tm_access || !tm_modification) {
   DeeObject *atemp,*mtemp; int temp;
   if (!tm_access && !tm_modification) return 0; // no-op
   if (_DeeFS_GetTimes2(path,
    tm_access ? NULL : &atemp,NULL,
    tm_modification ? NULL : &mtemp
    ) == -1) return -1;
   temp = DeeTime_AsTimeT(tm_access ? tm_access : atemp,&times.actime);
   if (temp != -1) temp = DeeTime_AsTimeT(tm_modification ? tm_modification : mtemp,&times.actime);
   if (!tm_access) Dee_DECREF(atemp);
   if (!tm_modification) Dee_DECREF(mtemp);
   if (temp == -1) return -1;
  } else {
   if (DeeTime_AsTimeT(tm_access,&times.actime) == -1 ||
       DeeTime_AsTimeT(tm_modification,&times.modtime) == -1
       ) return -1;
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if (utime(path,&times) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "utime(" DEE_PRINTF_STRQ ",<%O>,<%O>) : %K",
                       path,tm_access,tm_modification,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  (void)path;
  (void)tm_access;
  (void)tm_creation;
  (void)tm_modification;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_F(GetTimes2)(
 DEE_A_IN_Z DEE_CHAR const *path,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  struct DeeVFSFile file; DeeObject *rfs_path; int temp;
  if ((temp = DeeVFS_F(LocateOrReadReFsLink)(path,&file,&rfs_path)) == -1) return -1;
  if (temp == 0) {
   if (rfs_path) {
    temp = _DeeFS_F(GetTimes2)(DEE_STRING_STR(
     rfs_path),tm_access,tm_creation,tm_modification);
    Dee_DECREF(rfs_path);
    return temp;
   }
   if (tm_access) if ((*tm_access = DeeTime_New(0)) == NULL) return -1;
   if (tm_creation) if ((*tm_creation = DeeTime_New(0)) == NULL) goto err_0;
   if (tm_modification) if ((*tm_modification = DeeTime_New(0)) == NULL) goto err_1;
   return 0;
  }
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE file_handle; int error;
  WIN32_FILE_ATTRIBUTE_DATA attribute_data;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if ((file_handle = WIN32_F(CreateFile)(path,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ|
   FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)) == INVALID_HANDLE_VALUE) {
   error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_F(Win32IsPathUNC)(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
    error = _DeeFS_WideGetTimes2(DeeWideString_STR(temp),tm_access,tm_creation,tm_modification);
    Dee_DECREF(temp);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   if (error == ERROR_ACCESS_DENIED) {
    // This can happen if 'path' is not a file, but a directory
    if (!WIN32_F(GetFileAttributesEx)(path,GetFileExInfoStandard,&attribute_data)) {
     SetLastError(0);
    } else goto put_data;
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       DEE_PP_STR(WIN32_F(CreateFile))
                       "(" DEE_PRINTF_STRQ ") : %K",
                       path,DeeSystemError_Win32ToString((unsigned long)error));
   return -1;
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_fh;
  if (!GetFileTime(file_handle,
   tm_access ? &attribute_data.ftLastAccessTime : NULL,
   tm_creation ? &attribute_data.ftCreationTime : NULL,
   tm_modification ? &attribute_data.ftLastWriteTime : NULL)) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetFileTime(%p:" DEE_PRINTF_STRQ ") : %K",
                       file_handle,path,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_fh: if (!CloseHandle(file_handle)) SetLastError(0);
   return -1;
  }
  if (!CloseHandle(file_handle)) SetLastError(0);
put_data:
  if (tm_access && (*tm_access = DeeTime_NewFromWin32FileTime(&attribute_data.ftLastAccessTime)) == NULL) return -1;
  if (tm_creation && (*tm_creation = DeeTime_NewFromWin32FileTime(&attribute_data.ftCreationTime)) == NULL) goto err_0;
  if (tm_modification && (*tm_modification = DeeTime_NewFromWin32FileTime(&attribute_data.ftLastWriteTime)) == NULL) goto err_1;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8GetTimes2(DeeUtf8String_STR(path_ob),tm_access,tm_creation,tm_modification);
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib;
  if (_deefs_posix_stat(path,&attrib) == -1) return -1;
  if (tm_access && (*tm_access = DeeTime_NewFromTimeT(attrib.st_atime)) == NULL) return -1;
  if (tm_creation && (*tm_creation = DeeTime_NewFromTimeT(attrib.st_ctime)) == NULL) goto err_0;
  if (tm_modification && (*tm_modification = DeeTime_NewFromTimeT(attrib.st_mtime)) == NULL) goto err_1;
#else
  (void)path;
  if (tm_access && (*tm_access = DeeTime_New(0)) == NULL) return -1;
  if (tm_creation && (*tm_creation = DeeTime_New(0)) == NULL) goto err_0;
  if (tm_modification && (*tm_modification = DeeTime_New(0)) == NULL) goto err_1;
#endif
  return 0;
err_1: if (tm_creation) Dee_DECREF(*tm_creation);
err_0: if (tm_access) Dee_DECREF(*tm_access);
  return -1;
 }
}

#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) WIN32_F(fixed_GetFileAttributes)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_OUT DWORD *attr) {
 DWORD error;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if ((*attr = WIN32_F(GetFileAttributes)(path)) == INVALID_FILE_ATTRIBUTES) {
  error = GetLastError();
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *unc_path;
   if ((unc_path = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   *attr = GetFileAttributesW(DeeWideString_STR(unc_path));
   error = GetLastError(); // preserve error across decref
   Dee_DECREF(unc_path);
   SetLastError(error);
  }
 }
 return 0;
}
#endif /* !DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
#endif


DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsFile)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsFile)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) != 0) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES)
   return (temp&FILE_ATTRIBUTE_DIRECTORY)==0;
  temp = DeeSystemError_Win32Consume();
  return temp != ERROR_FILE_NOT_FOUND &&
         temp != ERROR_PATH_NOT_FOUND &&
         temp != ERROR_INVALID_NAME;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsFile(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (temp == 1) return 0;
  return S_ISREG(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsDir)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsDir)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) == -1) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES) {
   if ((temp & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
    HANDLE handle;
    if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
    if ((handle = WIN32_F(CreateFile)(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|
     FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL)) != INVALID_HANDLE_VALUE) {
     CloseHandle(handle);
     goto ret_attr_dir;
    } else {
     temp = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
     if (DEE_FS_WIN32_IS_UNC_ERROR(temp) &&
        !DeeFS_F(Win32IsPathUNC)(path)) {
      DeeObject *unc_path; int result;
      if ((unc_path = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
      result = _DeeFS_WideIsDir(DeeWideString_STR(unc_path));
      Dee_DECREF(unc_path);
      return result;
     }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
     return 0;
    }
   } else {
ret_attr_dir:
    return (temp & FILE_ATTRIBUTE_DIRECTORY) != 0;
   }
  }
  temp = DeeSystemError_Win32Consume();
  return !WIN32_IS_FILE_NOT_FOUND_ERROR(temp);
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsDir(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (temp == 1) return 0;
  return S_ISDIR(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsLink)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsLink)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) == -1) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES)
   return (temp & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
  else SetLastError(0);
  return 0;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsLink(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX) && DEE_HAVE_LSTAT && defined(S_ISLNK)
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_lstat(path,&attrib)) == -1) return -1;
  if (temp == 1) return 0;
  return S_ISLNK(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsHidden)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsHidden)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) == -1) return -1;
  if (temp != INVALID_FILE_ATTRIBUTES) return (temp & FILE_ATTRIBUTE_HIDDEN) != 0;
  SetLastError(0);
  return 0;
#elif defined(DEE_PLATFORM_UNIX)
  DEE_CHAR const *path_end; DEE_ASSERT(path);
  path_end = path+DEE_STRLEN(path);
  while (path_end != path && !IS_SEP(path_end[-1])) --path_end;
  return *path_end == DEE_CHAR_C('.');
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsExecutable)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsExecutable)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  static DEE_CHAR const s_PATHEXT[] = {'P','A','T','H','E','X','T',0};
  DeeObject *pathext; DEE_CHAR *iter,*ext_begin,*end;
  DEE_CHAR const *path_end,*path_iter; Dee_size_t path_size,ext_size;
  DEE_ASSERT(path);
  if ((pathext = DeeFS_F(GetEnv)(s_PATHEXT)) == NULL) return -1;
  end = (iter = DEE_STRING_STR(pathext))+DEE_STRING_SIZE(pathext);
  path_end = path+(path_size = DEE_STRLEN(path));
  if (iter != end) while (1) {
   ext_begin = iter;
   while (iter != end && *iter != DEE_CHAR_C(';')) ++iter;
   ext_size = (Dee_size_t)(iter-ext_begin);
   if (ext_size <= path_size) {
    path_iter = path+(path_size-ext_size);
    while (path_iter != path_end) if (
     DEE_CH_TO_UPPER(*path_iter++) !=
     DEE_CH_TO_UPPER(*ext_begin++)) goto next;
    Dee_DECREF(pathext);
    // It has the right suffix >> Not make sure it exists as a file
    return _DeeFS_F(IsFile)(path);
   }
next:
   if (iter == end) break;
   ++iter; // skip ';'
  }
  Dee_DECREF(pathext);
  return 0;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsExecutable(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return (attrib.st_mode & 0111) != 0;
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsCharDev)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsCharDev)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
#ifdef FILE_TYPE_CHAR
#define DEE_FS_HAVE_IS_CHAR_DEV
  int result = _DeeFS_F(Win32GetFileType)(path);
  return result == -1 ? -1 : result == FILE_TYPE_CHAR;
#else
  (void)path;
  return 0;
#endif
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsCharDev(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISCHR)
#define DEE_FS_HAVE_IS_CHAR_DEV
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return S_ISCHR(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsBlockDev)(DEE_A_IN_Z DEE_CHAR const *path) {
 (void)path;
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsBlockDev)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
#if 1
  return 0;
#else
#define DEE_FS_HAVE_IS_BLOCK_DEV
  int result = _DeeFS_F(Win32GetFileType)(path);
  return result == -1 ? -1 : result == FILE_TYPE_DISK;
#endif
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsBlockDev(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISBLK)
#define DEE_FS_HAVE_IS_BLOCK_DEV
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return S_ISBLK(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsFiFo)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsFiFo)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
#ifdef FILE_TYPE_PIPE
#define DEE_FS_HAVE_IS_NAMED_PIPE
  int result = _DeeFS_F(Win32GetFileType)(path);
  return result == -1 ? -1 : result == FILE_TYPE_PIPE;
#else
  (void)path;
  return 0;
#endif
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsFiFo(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISFIFO)
#define DEE_FS_HAVE_IS_NAMED_PIPE
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return S_ISFIFO(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsSocket)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsSocket)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
#ifdef FILE_TYPE_REMOTE
#define DEE_FS_HAVE_IS_SOCKET
  int result = _DeeFS_F(Win32GetFileType)(path);
  return result == -1 ? -1 : result == FILE_TYPE_REMOTE;
#else
  (void)path;
  return 0;
#endif
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsSocket(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX) && defined(S_ISSOCK)
#define DEE_FS_HAVE_IS_SOCKET
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  if (!S_ISREG(attrib.st_mode)) return 0;
  return S_ISSOCK(attrib.st_mode);
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsDrive)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_CHAR const *path_iter;
 DEE_ASSERT(path); path_iter = path;
 if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
 do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter)); // we allow longer drive names (coz who nose?)
 if (!IS_DRIVE_SEP(*path)) return 0; // no ':' after the drive name
 while (IS_SEP(path[1])) ++path; // skip seps after the ':'
 return !path[1];
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsMount)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsMount)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  return _DeeFS_F(IsDrive)(path);
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsMount(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  // As described here:
  // http://stackoverflow.com/questions/10410513/function-or-a-systemcall-similar-to-mountpoint-command-in-linux
  struct stat mp,mp_parent; int temp;
  char *path_parent,*path_end; Dee_size_t path_size;
  if ((temp = _deefs_posix_try_stat(path,&mp)) != 0)
   return temp == -1 ? -1 : 0;
  path_size = strlen(path);
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
  path_parent = (char *)alloca((path_size+4)*sizeof(char));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
  while ((path_parent = (char *)malloc_nz(
   (path_size+4)*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
#endif
  path_end = path_parent+path_size;
  if (path_size) {
   memcpy(path_parent,path,path_size*sizeof(char));
   if (!IS_SEP(path_end[-1])) *path_end++ = SEP;
  }
  *path_end++ = '.';
  *path_end++ = '.';
  *path_end++ = '\0';
  if ((temp = _deefs_posix_try_stat(path_parent,&mp_parent)) != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(path_parent);
#endif
   return temp == -1 ? -1 : 0;
  }
#if !DEE_HAVE_ALLOCA
  free_nn(path_parent);
#endif
  return mp.st_dev != mp_parent.st_dev;
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(Exists)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(Exists)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) == -1) return -1;
  if (temp == INVALID_FILE_ATTRIBUTES) { SetLastError(0); return 0; }
  return 1;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8Exists(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  struct stat attrib; int temp;
  if ((temp = _deefs_posix_try_stat(path,&attrib)) == -1) return -1;
  return temp == 0;
#else
  (void)path;
  return 0;
#endif
 }
}


DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Remove)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *path_ob; int result;
  if ((path_ob = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(Remove)(DEE_STRING_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  int error; DWORD temp;
  if (WIN32_F(fixed_GetFileAttributes)(path,&temp) == -1) return -1;
  if (temp == INVALID_FILE_ATTRIBUTES) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       DEE_PP_STR(WIN32_F(GetFileAttributes))
                       "(" DEE_PRINTF_STRQ ") : %K",path,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  } else if ((temp&(FILE_ATTRIBUTE_DIRECTORY|
   FILE_ATTRIBUTE_REPARSE_POINT)) == FILE_ATTRIBUTE_DIRECTORY) {
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   if (!WIN32_F(RemoveDirectory)(path)) {
    error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
    if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
       !DeeFS_F(Win32IsPathUNC)(path)) {
     DeeObject *temp;
try_unc:
     if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
     error = _DeeFS_WideRemove(DeeWideString_STR(temp));
     Dee_DECREF(temp);
     return error;
    }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        DEE_PP_STR(WIN32_F(RemoveDirectory))
                        "(" DEE_PRINTF_STRQ ") : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
    return -1;
   }
  } else {
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   if (!WIN32_F(DeleteFile)(path)) {
    error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
    if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
       !DeeFS_Utf8Win32IsPathUNC(path)) goto try_unc;
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        DEE_PP_STR(WIN32_F(DeleteFile))
                        "(" DEE_PRINTF_STRQ ") : %K",
                        path,DeeSystemError_Win32ToString((unsigned long)error));
    return -1;
   }
  }
  return 0;
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8Remove(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if (remove(path) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "remove(" DEE_PRINTF_STRQ ") : %K",path,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  (void)path;
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_F(RmFile)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *path_ob; int result;
  if ((path_ob = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(RmFile)(DEE_STRING_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(DeleteFile)(path)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *temp;
   if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   error = _DeeFS_WideRmFile(DeeWideString_STR(temp));
   Dee_DECREF(temp);
   return error;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(DeleteFile))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      path,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8RmFile(DeeUtf8String_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (unlink(path) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "unlink(" DEE_PRINTF_STRQ ") : %K",path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)path;
 return 0;
#endif
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_F(MkDir)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_mode_t mode) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *path_ob; int result;
  if ((path_ob = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(MkDir)(DEE_STRING_STR(path_ob),mode);
  Dee_DECREF(path_ob);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 (void)mode; // TODO: read-only flag?
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(CreateDirectory)(path,NULL)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *temp;
   if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   error = _DeeFS_WideMkDir(DeeWideString_STR(temp),mode);
   Dee_DECREF(temp);
   return error;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(CreateDirectory))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      path,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8MkDir(DeeUtf8String_STR(path_ob),mode);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (mkdir(path,mode) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "mkdir(" DEE_PRINTF_STRQ ",%#o) : %K",path,mode,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)path,mode;
 return 0;
#endif
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_F(RmDir)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) {
  DeeObject *path_ob; int result;
  if ((path_ob = DeeVFS_F(ReadReFsLink)(path)) == NULL) return -1;
  result = _DeeFS_F(RmDir)(DEE_STRING_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(RemoveDirectory)(path)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *temp;
   if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
   error = _DeeFS_WideRmDir(DeeWideString_STR(temp));
   Dee_DECREF(temp);
   return error;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(RemoveDirectory))
                      "(" DEE_PRINTF_STRQ ") : %K",
                      path,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *path_ob; int result;
 if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = _DeeFS_Utf8RmDir(DeeUtf8String_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (rmdir(path) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "rmdir(" DEE_PRINTF_STRQ ") : %K",path,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)path;
 return 0;
#endif
}

DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Copy)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(src)) {
  DeeObject *src_path; int result;
  if ((src_path = DeeVFS_F(ReadReFsLink)(src)) == NULL) return -1;
  result = _DeeFS_F(Copy)(DEE_STRING_STR(src_path),dst);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_F(IsVirtualPath)(dst)) {
  DeeObject *dst_path; int result;
  if ((dst_path = DeeVFS_F(ReadReFsLink)(dst)) == NULL) return -1;
  result = _DeeFS_F(Copy)(src,DEE_STRING_STR(dst_path));
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(CopyFile)(src,dst,TRUE)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#ifdef WIDE
  if (DEE_FS_WIN32_IS_UNC_ERROR(error)) {
   DeeObject *temp;
   if (!DeeFS_WideWin32IsPathUNC(src)) {
    if ((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
    error = _DeeFS_WideCopy(DeeWideString_STR(temp),dst);
    goto unc_end0;
   }
   if (!DeeFS_WideWin32IsPathUNC(dst)) {
    if ((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
    error = _DeeFS_WideCopy(src,DeeWideString_STR(temp));
unc_end0: Dee_DECREF(temp);
    return error;
   }
  }
#else
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     (!DeeFS_Utf8Win32IsPathUNC(src) ||
      !DeeFS_Utf8Win32IsPathUNC(dst))) {
   DeeObject *unc_src,*unc_dst;
   if ((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
    ? DeeFS_Utf8Win32PathUNC(src)
    : DeeWideString_FromUtf8String(src)) == NULL) return -1;
   if ((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
    ? DeeFS_Utf8Win32PathUNC(dst)
    : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
   error = _DeeFS_WideCopy(
    DeeWideString_STR(unc_src),DeeWideString_STR(unc_dst));
   Dee_DECREF(unc_dst);
err_unc0: Dee_DECREF(unc_src);
   return error;
  }
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(CopyFile))
                      "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",
                      src,dst,error,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeUtf8String_FromWideString(src)) == NULL) return -1;
 if ((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { result = -1; goto end_0; }
 result = _DeeFS_Utf8Copy(DeeUtf8String_STR(src_ob),DeeUtf8String_STR(dst_ob));
 Dee_DECREF(dst_ob);
end_0: Dee_DECREF(src_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 int fd_dst,fd_src;
 char buf[4096];
 ssize_t nread;
 int saved_errno;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 fd_src = open(src,O_RDONLY);
 if (fd_src < 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "open(" DEE_PRINTF_STRQ ") : copy:open(src) : %K",src,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_from;
 fd_dst = open(dst,O_WRONLY|O_CREAT|O_EXCL,DEE_FILEIO_DEFAULT_PERMISSIONS);
 if (fd_dst < 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "open(" DEE_PRINTF_STRQ ") : copy:open(dst) : %K",dst,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
err_from:
  if (close(fd_src) != 0) errno = 0;
  return -1;
 }
 while (1) {
  char *out_ptr;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_to;
  nread = read(fd_src,buf,sizeof(buf));
  if (nread <= 0) break;
  out_ptr = buf;
  ssize_t nwritten;
  do {
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_to;
   nwritten = write(fd_dst,out_ptr,nread);
   if (nwritten >= 0) {
    nread -= nwritten;
    out_ptr += nwritten;
   } else if (errno != EINTR) {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "copy.dst:write(%d: " DEE_PRINTF_STRQ ") : %K",fd_dst,dst,
                        DeeSystemError_ToString(DeeSystemError_Consume()));
err_to:
    if (close(fd_dst) != 0) errno = 0;
#if 1
    if (unlink(dst) != 0) errno = 0; // Erase partially copied fp
#endif
    goto err_from;
   }
  } while (nread > 0);
 }
 if (nread == 0) {
  if (close(fd_dst) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "copy.dst:close(%d: " DEE_PRINTF_STRQ ") : %K",fd_dst,dst,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   goto err_from;
  }
  if (close(fd_src) != 0) errno = 0;
  return 0; // success
 } else {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "copy.src:read(%d: " DEE_PRINTF_STRQ ") : %K",fd_src,src,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  goto err_to;
 }
#else
 (void)src,dst;
 return 0;
#endif
}



DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Move)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(src)) {
  DeeObject *src_path; int result;
  if ((src_path = DeeVFS_F(ReadReFsLink)(src)) == NULL) return -1;
  result = _DeeFS_F(Move)(DEE_STRING_STR(src_path),dst);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_F(IsVirtualPath)(dst)) {
  DeeObject *dst_path; int result;
  if ((dst_path = DeeVFS_F(ReadReFsLink)(dst)) == NULL) return -1;
  result = _DeeFS_F(Move)(src,DEE_STRING_STR(dst_path));
  Dee_DECREF(dst_path);
  return result;
 }
#endif
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (!WIN32_F(MoveFile)(src,dst)) {
  int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#ifdef WIDE
  if (DEE_FS_WIN32_IS_UNC_ERROR(error)) {
   DeeObject *temp;
   if (!DeeFS_WideWin32IsPathUNC(src)) {
    if ((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
    error = _DeeFS_WideMove(DeeWideString_STR(temp),dst);
    goto unc_end0;
   }
   if (!DeeFS_WideWin32IsPathUNC(dst)) {
    if ((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
    error = _DeeFS_WideMove(src,DeeWideString_STR(temp));
unc_end0: Dee_DECREF(temp);
    return error;
   }
  }
#else
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     (!DeeFS_Utf8Win32IsPathUNC(src) ||
      !DeeFS_Utf8Win32IsPathUNC(dst))) {
   DeeObject *unc_src,*unc_dst;
   if ((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
    ? DeeFS_Utf8Win32PathUNC(src)
    : DeeWideString_FromUtf8String(src)) == NULL) return -1;
   if ((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
    ? DeeFS_Utf8Win32PathUNC(dst)
    : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
   error = _DeeFS_WideMove(
    DeeWideString_STR(unc_src),DeeWideString_STR(unc_dst));
   Dee_DECREF(unc_dst);
err_unc0: Dee_DECREF(unc_src);
   return error;
  }
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(MoveFile))
                      "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",
                      src,dst,DeeSystemError_Win32ToString((unsigned long)error));
  return -1;
 }
 return 0;
#elif defined(WIDE)
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeUtf8String_FromWideString(src)) == NULL) return -1;
 if ((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { result = -1; goto end_0; }
 result = _DeeFS_Utf8Move(DeeUtf8String_STR(src_ob),DeeUtf8String_STR(dst_ob));
 Dee_DECREF(dst_ob);
end_0: Dee_DECREF(src_ob);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if (rename(src,dst) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "rename(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",src,dst,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#else
 (void)src,dst;
 return 0;
#endif
}



DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Link)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(src)) {
  DeeObject *src_path; int result;
  if ((src_path = DeeVFS_F(ReadReFsLink)(src)) == NULL) return -1;
  result = _DeeFS_F(Link)(DEE_STRING_STR(src_path),dst);
  Dee_DECREF(src_path);
  return result;
 }
 if (_DeeVFS_F(IsVirtualPath)(dst)) {
  DeeObject *dst_path; int result;
  if ((dst_path = DeeVFS_F(ReadReFsLink)(dst)) == NULL) return -1;
  result = _DeeFS_F(Link)(src,DEE_STRING_STR(dst_path));
  Dee_DECREF(dst_path);
  return result;
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD attr;
#ifdef WIDE
  typedef BOOLEAN (APIENTRY *LPCREATESYMBOLICLINKW)(
   LPCWSTR lpSymlinkFileName, LPCWSTR lpTargetFileName, DWORD dwFlags);
  static LPCREATESYMBOLICLINKW p_CreateSymbolicLinkW = NULL;
  static char const s_CreateSymbolicLinkW[] = "CreateSymbolicLinkW";
#else
  typedef BOOLEAN (APIENTRY *LPCREATESYMBOLICLINKA)(
   LPCSTR lpSymlinkFileName, LPCSTR lpTargetFileName, DWORD dwFlags);
  static LPCREATESYMBOLICLINKA p_CreateSymbolicLinkA = NULL;
  static char const s_CreateSymbolicLinkA[] = "CreateSymbolicLinkA";
#endif
  DEE_ATOMIC_ONCE({
   WIN32_F(p_CreateSymbolicLink) = (WIN32_F(LPCREATESYMBOLICLINK))GetProcAddress(
   GetModuleHandle(TEXT("KERNEL32")),WIN32_F(s_CreateSymbolicLink));
  });
  if (!WIN32_F(p_CreateSymbolicLink)) {
   DeeError_SET_STRING(&DeeErrorType_SystemError,
                       DEE_PP_STR(WIN32_F(CreateSymbolicLink))
                       "(...) : Not implemented");
   return -1;
  }
  // Try to enable the privilege
  // If this fails, we get a permissions error later
  _deefs_win32_enable_symlink();
  // TODO: In python3, the following...
  // >> import _winapi
  // >> print(_winapi.CreateJunction("include", "newinclude"))
  // ... manages to create a directory link without admin rights...
  if (WIN32_F(fixed_GetFileAttributes)(src,&attr) == -1) return -1;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if (!(*WIN32_F(p_CreateSymbolicLink))(dst,src,(DWORD)(
   ((attr&FILE_ATTRIBUTE_DIRECTORY)!=0)
   ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0))) {
   int error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#ifdef WIDE
   if (DEE_FS_WIN32_IS_UNC_ERROR(error)) {
    DeeObject *temp;
    if (!DeeFS_WideWin32IsPathUNC(src)) {
     if ((temp = DeeFS_WideWin32PathUNC(src)) == NULL) return -1;
     error = _DeeFS_WideLink(DeeWideString_STR(temp),dst);
     goto unc_end0;
    }
    if (!DeeFS_WideWin32IsPathUNC(dst)) {
     if ((temp = DeeFS_WideWin32PathUNC(dst)) == NULL) return -1;
     error = _DeeFS_WideLink(src,DeeWideString_STR(temp));
unc_end0: Dee_DECREF(temp);
     return error;
    }
   }
#else
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      (!DeeFS_Utf8Win32IsPathUNC(src) ||
       !DeeFS_Utf8Win32IsPathUNC(dst))) {
    DeeObject *unc_src,*unc_dst;
    if ((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
     ? DeeFS_Utf8Win32PathUNC(src)
     : DeeWideString_FromUtf8String(src)) == NULL) return -1;
    if ((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
     ? DeeFS_Utf8Win32PathUNC(dst)
     : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
    error = _DeeFS_WideLink(
     DeeWideString_STR(unc_src),DeeWideString_STR(unc_dst));
    Dee_DECREF(unc_dst);
err_unc0: Dee_DECREF(unc_src);
    return error;
   }
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
#if 0
   if (error == ERROR_PRIVILEGE_NOT_HELD) { // We can't create symbolic links...
    // TODO: Try out luck with 'mklink <dst> <src>'
    if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
    if (!WIN32_F(CreateHardLink)(dst,src,NULL)) {
     DeeError_SetStringf(&DeeErrorType_SystemError,
                         DEE_PP_STR(WIN32_F(CreateHardLink))
                         "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",dst,src,
                         DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
     return -1;
    }
   } else
#endif
   {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        DEE_PP_STR(WIN32_F(CreateSymbolicLink))
                        "(" DEE_PRINTF_STRQ "," DEE_PRINTF_STRQ ") : %K",
                        dst,src,DeeSystemError_Win32ToString((unsigned long)error));
    return -1;
   }
  }
  return 0;
#elif defined(WIDE)
  DeeObject *src_ob,*dst_ob; int result;
  if ((src_ob = DeeUtf8String_FromWideString(src)) == NULL) return -1;
  if ((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { result = -1; goto end_0; }
  result = _DeeFS_Utf8Link(DeeUtf8String_STR(src_ob),DeeUtf8String_STR(dst_ob));
  Dee_DECREF(dst_ob);
end_0: Dee_DECREF(src_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if (link(src,dst) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "link(%q,%q) : %K",src,dst,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  return 0;
#else
  (void)src,dst;
  return 0;
#endif
 }
}


#ifdef DEE_PLATFORM_WINDOWS
#ifndef DEE_PRIVATE_REPARSE_DATA_BUFFER_DEFINED
#define DEE_PRIVATE_REPARSE_DATA_BUFFER_DEFINED
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT ReparseDataLength;
  USHORT Reserved;
  union {
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      ULONG  Flags;
      WCHAR  PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      WCHAR  PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      UCHAR DataBuffer[1];
    } GenericReparseBuffer;
  }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define SymbolicLinkReparseBuffer _rd_data.SymbolicLinkReparseBuffer
#define MountPointReparseBuffer   _rd_data.MountPointReparseBuffer
#define GenericReparseBuffer      _rd_data.GenericReparseBuffer
 _rd_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
  ;
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
DEE_COMPILER_MSVC_WARNING_POP
#endif

static DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
_DeeFS_F(Win32ReadLinkHandle)(DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN HANDLE hfile) {
 DeeObject *result; DWORD bytesReturned; Dee_size_t bufsize;
 REPARSE_DATA_BUFFER *buffer;
again:
 if ((buffer = (REPARSE_DATA_BUFFER *)malloc_nz((bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32READLINK))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
no_mem: DeeError_NoMemory();
  return NULL;
 }
 while (bufsize < MAXIMUM_REPARSE_DATA_BUFFER_SIZE) {
  // Make up the control code - see CTL_CODE on ntddk.h
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_buffer;
  if (!DeviceIoControl(hfile,FSCTL_GET_REPARSE_POINT,
   NULL,0,buffer,(DWORD)bufsize,&bytesReturned,NULL)) {
   DWORD error = DeeSystemError_Win32Consume();
   if (error == ERROR_NOT_A_REPARSE_POINT) {
    free_nn(buffer); // Not a reparse point --> return the path itself
    return DEE_STRING_New(path);
   } else if (error == ERROR_INSUFFICIENT_BUFFER) {
    REPARSE_DATA_BUFFER *new_buffer;
    while ((new_buffer = (REPARSE_DATA_BUFFER *)
     realloc_nnz(buffer,bufsize *= 2)) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) goto again;
     free_nn(buffer); goto no_mem;
    }
    buffer = new_buffer;
   } else {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "DeviceIoControl(%#p: " DEE_PRINTF_STRQ ") : %K",
                        hfile,path,DeeSystemError_Win32ToString(error));
err_buffer:
    free_nn(buffer);
    return NULL;
   }
  } else break;
 }
 switch (buffer->ReparseTag) {
  case IO_REPARSE_TAG_SYMLINK:
   result = DeeString_F(FromWideStringWithLength)(
    buffer->SymbolicLinkReparseBuffer.SubstituteNameLength/sizeof(Dee_WideChar),
    buffer->SymbolicLinkReparseBuffer.PathBuffer+
    buffer->SymbolicLinkReparseBuffer.SubstituteNameOffset/sizeof(Dee_WideChar));
   break;
  case IO_REPARSE_TAG_MOUNT_POINT: {
   Dee_WideChar const *begin,*end;
   end = (begin = buffer->MountPointReparseBuffer.PathBuffer+
    (buffer->MountPointReparseBuffer.SubstituteNameOffset/sizeof(Dee_WideChar)))+
    (buffer->MountPointReparseBuffer.SubstituteNameLength/sizeof(Dee_WideChar));
   // Get rid of that annoying '\??\' prefix
   if (begin+4 <= end && IS_SEP(begin[0]) && begin[1] == '?' &&
       begin[2] == '?' && IS_SEP(begin[3])) begin += 4;
   result = DeeString_F(FromWideStringWithLength)((Dee_size_t)(end-begin),begin);
  } break;
  default:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "DeviceIoControl(%#p: " DEE_PRINTF_STRQ ") : Unknown/Unsupported link type: %lu",
                       hfile,path,buffer->ReparseTag);
   result = NULL;
   break;
 }
 free_nn(buffer);
 return result;
}
#endif


DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
_DeeFS_F(ReadLink)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(ReadLink)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DeeObject *result; HANDLE hfile;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
  if ((hfile = WIN32_F(CreateFile)(
   path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,
   FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL)) == INVALID_HANDLE_VALUE) {
   DWORD error = DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_F(Win32IsPathUNC)(path)) {
    DeeObject *temp;
    if ((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return NULL;
    result = _DeeFS_WideReadLink(DeeWideString_STR(temp));
    Dee_DECREF(temp);
    return result;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   if (_DeeFS_F(Exists)(path)) return DEE_STRING_New(path);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       DEE_PP_STR(WIN32_F(CreateFile))
                       "(" DEE_PRINTF_STRQ ") : %K",
                       path,DeeSystemError_Win32ToString(error));
   return NULL;
  }
  result = _DeeFS_F(Win32ReadLinkHandle)(path,hfile);
  if (!CloseHandle(hfile)) SetLastError(0);
  return result;
#elif defined(WIDE)
  DeeObject *result,*path_ob;
  if ((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
  result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  if (result && DeeWideString_InplaceCast((DeeObject const **)&result) != 0) Dee_CLEAR(result);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t bytes; DeeObject *result; int error;
  if DEE_UNLIKELY((result = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) == NULL) return NULL;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
  bytes = (Dee_ssize_t)readlink(path,DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);
again:
  if (DEE_UNLIKELY(bytes < 0)) {
err_sys:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "readlink(" DEE_PRINTF_STRQ ") : %K",path,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
err_r:
   Dee_DECREF(result);
   return NULL;
  } else if (DEE_UNLIKELY(bytes > DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) {
   Dee_ssize_t new_bytes;
   if (DeeUtf8String_Resize(&result,bytes) == -1) goto err_r;
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   new_bytes = (Dee_ssize_t)readlink(path,DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);
   if (new_bytes < 0) goto err_sys;
   if (DEE_UNLIKELY(new_bytes != bytes)) {
    bytes = new_bytes; // Link must have changed
    goto again;
   }
  } else if (DEE_LIKELY(bytes < DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) {
   if (DeeUtf8String_Resize(&result,bytes) == -1) goto err_r;
  }
  return result;
#endif
 }
}







//////////////////////////////////////////////////////////////////////////
// Joins an array of strings, to form a valid path
// - Slashes are stripped from all path elements
// - Empty paths are ignored
// - All paths are joined with the platform-specific slash
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathJoinObject)(DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) DeeObject const *const *pathv) {
 DeeObject *result; DeeObject const *const *iter,*const *end,*elem;
 DEE_CHAR const *path_begin,*path_end;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 end = (iter = pathv)+pathc;
 while (iter != end) {
  if ((elem = DEE_STRING_Cast(*iter)) == NULL) goto err;
  path_end = (path_begin = DEE_STRING_STR(elem))+DEE_STRING_SIZE(elem);
  if (iter != pathv) while (path_begin < path_end && IS_SEP(*path_begin)) ++path_begin;
  if (iter != end-1) while (path_end > path_begin && IS_SEP(path_end[-1])) --path_end;
  if (path_begin != path_end) {
   if (iter != pathv && DEE_STRINGWRITER_SIZE(&writer) != 0 &&
       !IS_SEP(DEE_STRINGWRITER_ELEM(&writer)[DEE_STRINGWRITER_SIZE(&writer)-1]) &&
       DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C(SEP)) == -1
       ) goto err_elem;
   if (DeeStringWriter_F(WriteMyStringWithLength)(
    &writer,(Dee_size_t)(path_end-path_begin),path_begin) == -1) goto err_elem;
  }
#if DEE_CONFIG_RUNTIME_HAVE_VFS
  else if (iter == pathv && IS_SEP(*DEE_STRING_STR(elem))) {
   if (DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C(SEP)) == -1) goto err_elem;
  }
#endif
  Dee_DECREF(elem);
  ++iter;
 }
 result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
err_elem: Dee_DECREF(elem);
err: result = NULL; goto end;
}


#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28020)
#endif
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathExpandVarsObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR ch,*iter,*end,*var_begin,*var_end,*var_name;
 DeeObject *var_value,*result; Dee_size_t varname_len; int temp;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 end = (iter = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while DEE_LIKELY(iter != end) if DEE_UNLIKELY(
  *iter == DEE_CHAR_C('$') || *iter == DEE_CHAR_C('%')
  ) goto begin; else ++iter;
 // If the path doesn't contain any characters indicating vars, don't do anything!
 DeeReturn_NEWREF(path);
begin: {
  DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
  iter = DEE_STRING_STR(path);
  while DEE_LIKELY(iter != end) {
   ch = *iter++;
   if DEE_UNLIKELY(ch == DEE_CHAR_C('$')) {
    if DEE_UNLIKELY(*iter == DEE_CHAR_C('{')) {
     unsigned int recursion = 1;
     var_begin = ++iter;
     while (iter != end) {
      if DEE_UNLIKELY(*iter == DEE_CHAR_C('}') && !--recursion) break; else
      if DEE_UNLIKELY(*iter == DEE_CHAR_C('{')) ++recursion;
      ++iter;
     }
     if DEE_LIKELY((var_end = iter) != end) ++iter;
    } else {
     var_begin = iter;
     while (/*iter != end && */DEE_CH_IS_ALNUM(*iter)) ++iter;
     var_end = iter;
    }
put_var:
    varname_len = (Dee_size_t)(var_end-var_begin);
    while DEE_UNLIKELY((var_name = (DEE_CHAR *)malloc_nz((
     varname_len+1)*sizeof(DEE_CHAR))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     goto err;
    }
    memcpy(var_name,var_begin,varname_len*sizeof(DEE_CHAR));
    var_name[varname_len] = DEE_CHAR_C('\0');
    var_value = DeeFS_F(TryGetEnv)(var_name);
    free_nn(var_name);
    if DEE_LIKELY(var_value) {
     temp = DeeStringWriter_F(WriteMyStringWithLength)(
      &writer,DEE_STRING_SIZE(var_value),DEE_STRING_STR(var_value));
     Dee_DECREF(var_value);
     if DEE_UNLIKELY(temp == -1) goto err;
    } else { // Unknown variable (ignore + don't expand)
     if (*--var_begin == DEE_CHAR_C('{')) --var_begin;
     if (DeeStringWriter_F(WriteMyStringWithLength)(
      &writer,(Dee_size_t)(iter-var_begin),var_begin) == -1
      ) goto err;
    }
   } else if DEE_UNLIKELY(ch == DEE_CHAR_C('%')) {
    var_begin = iter;
    while (iter != end && *iter != DEE_CHAR_C('%')) ++iter;
    if DEE_LIKELY((var_end = iter) != end) ++iter;
    goto put_var;
   } else if DEE_UNLIKELY(DeeStringWriter_F(WriteMyChar)(&writer,ch) == -1) {
    goto err;
   }
  }
  result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
  return result;
err: result = NULL; goto end;
 }
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif


DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathExpandUserObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DeeObject *result;
 DEE_CHAR *path_begin,*path_end,*user,*user_begin;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 if (path_begin == path_end) DEERETURN_EMPTYSTRING;
 if (*path_begin == DEE_CHAR_C('~')) {
  ++path_begin;
  if (DEE_CH_IS_ALNUM(*path_begin)) {
   user_begin = path_begin; // Custom user
   do ++path_begin; while (DEE_CH_IS_ALNUM(*path_begin));
#if DEE_HAVE_ALLOCA
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
   user = (DEE_CHAR *)alloca((((Dee_size_t)(path_begin-user_begin))+1)*sizeof(DEE_CHAR));
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
   while ((user = (DEE_CHAR *)malloc_nz((((
    Dee_size_t)(path_begin-user_begin))+1)*sizeof(DEE_CHAR))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
  }
#endif
   memcpy(user,user_begin,(Dee_size_t)(path_begin-user_begin)*sizeof(DEE_CHAR));
   user[path_begin-user_begin] = DEE_CHAR_C('\0');
   result = DeeFS_F(GetHomeUser)(user);
#if !DEE_HAVE_ALLOCA
   free_nn(user);
#endif
   if (!result) return NULL;
  } else {
   if ((result = DeeFS_F(GetHome)()) == NULL) return NULL;
  }
#if 0
  while (path_begin != path_end && IS_SEP(*path_begin)) ++path_begin;
#endif
  if (DeeString_F(AppendWithLength)(&result,(Dee_size_t)(path_end-path_begin),path_begin) == -1) { Dee_DECREF(result); result = NULL; }
  return result;
 } else DeeReturn_NEWREF(path);
}








static DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *_DeeFS_F(PathAbsObject)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *cwd) {
 DEE_CHAR const *path_begin,*path_end,*cwd_begin,*cwd_end;
 DeeObject *result,*real_cwd,*new_cwd; DEE_CHAR *result_iter;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 DEE_ASSERT(DeeObject_Check(cwd) && DEE_STRING_Check(cwd));
 if (_DeeFS_F(IsAbs)(DEE_STRING_STR(path))) DeeReturn_NEWREF(path);
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(cwd))) {
  // Make sure 'cwd' is absolute
  if ((real_cwd = DeeFS_F(GetCwd)()) == NULL) return NULL;
  new_cwd = _DeeFS_F(PathAbsObject)(cwd,real_cwd);
  Dee_DECREF(real_cwd);
  if (!new_cwd) return NULL;
  cwd = new_cwd;
 } else Dee_INCREF(cwd);
 path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 cwd_end = (cwd_begin = DEE_STRING_STR(cwd))+DEE_STRING_SIZE(cwd);
 while (path_begin != path_end) {
  if (path_begin[0] == DEE_CHAR_C('.') && path_begin[1] == DEE_CHAR_C('.') &&
     (path_begin+2 == path_end || IS_SEP(path_begin[2]))) {
   // Skip seps
#ifdef IS_DRIVE_SEP
   while (cwd_end != cwd_begin && IS_SEP(cwd_end[-1]) && !IS_DRIVE_SEP(cwd_end[-2])) --cwd_end;
   while (cwd_end != cwd_begin && !IS_SEP(cwd_end[-1]) && !IS_DRIVE_SEP(cwd_end[-2])) --cwd_end;
#else
   while (cwd_end != cwd_begin && IS_SEP(cwd_end[-1])) --cwd_end;
   while (cwd_end != cwd_begin && !IS_SEP(cwd_end[-1])) --cwd_end;
#endif
   if (path_begin+2 == path_end) path_begin = path_end;
   else path_begin += 3; // Prev directory reference
   goto skip_path_sep;
  } else if (path_begin[0] == DEE_CHAR_C('.') && IS_SEP(path_begin[1])) {
   path_begin += 2; // Current directory reference
   goto skip_path_sep;
  } else if (path_begin[0] == DEE_CHAR_C('.') && path_begin+1 == path_end) {
   ++path_begin; // Current directory reference
skip_path_sep:
   while (IS_SEP(*path_begin)) ++path_begin;
  } else break;
 }
 if (path_begin == path_end) {
  if (cwd_begin == cwd_end) return (DeeObject *)cwd; // inherit reference
  result = DEE_STRING_NewWithLength((Dee_size_t)(cwd_end-cwd_begin),cwd_begin);
 } else {
  while (cwd_end != cwd_begin && IS_SEP(cwd_end[-1])) --cwd_end;
  if ((result = DEE_STRING_NewSized(
   (Dee_size_t)(cwd_end-cwd_begin)+1+
   (Dee_size_t)(path_end-path_begin))) != NULL) {
   result_iter = DEE_STRING_STR(result);
   memcpy(result_iter,cwd_begin,(Dee_size_t)(cwd_end-cwd_begin)*sizeof(DEE_CHAR));
   result_iter += (Dee_size_t)(cwd_end-cwd_begin);
   *result_iter++ = DEE_CHAR_C(SEP);
   memcpy(result_iter,path_begin,(Dee_size_t)(path_end-path_begin)*sizeof(DEE_CHAR));
  }
 }
 Dee_DECREF(cwd);
 return result;
}



static DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *_DeeFS_F(PathRelObject)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *cwd) {
 DEE_CHAR const *path_begin,*path_end,*path_part,*cwd_begin,*cwd_end,*cwd_part;
 DEE_CHAR *result_iter; unsigned int up_refs; DeeObject *result,*real_cwd = NULL,*temp;
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(path))) {
  if ((real_cwd = DeeFS_F(GetCwd)()) == NULL) {err_p: Dee_DECREF(path); return NULL; }
  if ((temp = _DeeFS_F(PathAbsObject)(path,real_cwd)) == NULL) {
err_rc_p: Dee_DECREF(real_cwd); goto err_p;
  }
  Dee_INHERIT_REF(path,temp);
 } else Dee_INCREF(path);
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(cwd))) {
  if (!real_cwd && (real_cwd = DeeFS_F(GetCwd)()) == NULL) { Dee_DECREF(cwd); goto err_p; }
  temp = _DeeFS_F(PathAbsObject)(cwd,real_cwd);
  if (!temp) goto err_rc_p;
  Dee_INHERIT_REF(cwd,temp);
 } else Dee_INCREF(cwd);
 Dee_XCLEAR(real_cwd);
 path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 cwd_end = (cwd_begin = DEE_STRING_STR(cwd))+DEE_STRING_SIZE(cwd);
#ifdef IS_DRIVE_SEP
 while (1) {
#ifdef DEE_PLATFORM_WINDOWS
  if (DEE_CH_TO_UPPER(*cwd_begin++) != DEE_CH_TO_UPPER(*path_begin++))
#else
  if (*cwd_begin++ != *path_begin++)
#endif
  {
diff_drive:
   Dee_DECREF(cwd);
   return (DeeObject *)path; // Inherit reference
  }
  if (IS_DRIVE_SEP(*cwd_begin)) {
   if (IS_DRIVE_SEP(*path_begin)) goto same_drive;
   goto diff_drive;
  } else if (IS_DRIVE_SEP(*path_begin)) goto diff_drive;
 }
same_drive:
 // Skip the drive sep ':'
 ++path_begin;
 ++cwd_begin;
#endif
 // Skip everything the two paths have in common
 cwd_part = cwd_begin,path_part = path_begin;
 goto common_begin;
 while (cwd_part != cwd_end && path_part != path_end) {
  if (IS_SEP(*cwd_part)) {
   if (!IS_SEP(*path_part)) goto common_end;
   do ++cwd_part; while (IS_SEP(*cwd_part));
   do ++path_part; while (IS_SEP(*path_part));
common_begin:
   while (*cwd_part == DEE_CHAR_C('.') && IS_SEP(cwd_part[1])) cwd_part += 2;
   while (*path_part == DEE_CHAR_C('.') && IS_SEP(path_part[1])) path_part += 2;
   cwd_begin = cwd_part,path_begin = path_part;
  } else {
   if (!CMP_PATH_C(*cwd_part,*path_part)) goto common_end;
   ++cwd_part,++path_part;
  }
 }
 cwd_begin = cwd_part,path_begin = path_part;
common_end:
 while (IS_SEP(*cwd_begin)) ++cwd_begin;
 while (IS_SEP(*path_begin)) ++path_begin;
 // Count the amount of folders remaining in 'cwd'
 // >> Depending on it's about, we have to add additional "../" prefixes to the resulting path
 up_refs = 0;
 while (cwd_begin != cwd_end) {
  while (!IS_SEP(*cwd_begin)) if (++cwd_begin == cwd_end) break;
  ++up_refs;
  while (IS_SEP(*cwd_begin)) ++cwd_begin;
 }
 Dee_DECREF(cwd);
#if 1 // Small, optional memory-reuse optimization
 if (path_begin == DEE_STRING_STR(path) && !up_refs)
  return (DeeObject *)path; // inherit reference
#endif
 if (!up_refs && path_end == path_begin) {
#ifdef WIDE
  DeeWideString_NEW_STATIC_EX(_cwd,1,{'.'});
#else
  DeeUtf8String_NEW_STATIC_EX(_cwd,1,{'.'});
#endif
  Dee_INCREF(result = (DeeObject *)&_cwd);
 } else if ((result = DEE_STRING_NewSized((Dee_size_t)(
  up_refs*3)+(Dee_size_t)(path_end-path_begin))) != NULL) {
  result_iter = DEE_STRING_STR(result);
  while (up_refs--) {
   *result_iter++ = DEE_CHAR_C('.');
   *result_iter++ = DEE_CHAR_C('.');
   *result_iter++ = DEE_CHAR_C(SEP);
  }
  memcpy(result_iter,path_begin,(Dee_size_t)(path_end-path_begin)*sizeof(DEE_CHAR));
 }
 Dee_DECREF(path);
 return result;
}

// Returns the head of a filename (excluding trail). e.g.: "/foo/bar.txt" --> "/foo/"
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathHeadObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_end;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_end = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_end != path_begin && !IS_SEP(new_path_end[-1])) --new_path_end;
 while (new_path_end != path_begin && IS_SEP(new_path_end[-2])) --new_path_end; // Skip multiple ending seps
 if (path_end == new_path_end) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(new_path_end-path_begin),path_begin);
}

// Returns the tail of a filename. e.g.: "/foo/bar.txt" --> "bar.txt"
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathTailObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_begin;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_begin = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_begin != path_begin && !IS_SEP(new_path_begin[-1])) --new_path_begin;
 if (path_begin == new_path_begin) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(path_end-new_path_begin),new_path_begin);
}

// Returns the name of a filename. e.g.: "/foo/bar.txt" --> "bar"
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathFileObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_begin,*new_path_end;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_begin = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_begin != path_begin && !IS_SEP(new_path_begin[-1])) --new_path_begin;
 new_path_end = new_path_begin;
 while (new_path_end != path_end && !IS_EXT_SEP(*new_path_end)) ++new_path_end;
 if (path_begin == new_path_begin && new_path_end == path_end) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(new_path_end-new_path_begin),new_path_begin);
}

// Returns the extension of a filename. e.g.: "/foo/bar.txt" --> ".txt" (Return "" if no extension is found)
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathExtObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_begin;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_begin = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_begin != path_begin && !IS_EXT_SEP(*new_path_begin)) {
  if (IS_SEP(new_path_begin[-1])) DEERETURN_EMPTYSTRING;
  --new_path_begin;
 }
 if (path_begin == new_path_begin) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(path_end-new_path_begin),new_path_begin);
}

// Returns the drive of a filename (including trail). e.g.: "C:/foo/bar.txt" --> "C:/" (Always returns the root folder "/" on non-windows systems; return "" if the path is relative)
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathDriveObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
#ifdef DRIVE_SEP
 DEE_CHAR *path_begin,*path_end,*new_path_end;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_end = (new_path_end = path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_end != path_end) {
  if (IS_DRIVE_SEP(*new_path_end)) {
   Dee_size_t drive_len = (Dee_size_t)(new_path_end-path_begin);
   DeeObject *result = DEE_STRING_NewSized(drive_len+2);
   if (result) {
    memcpy(DEE_STRING_STR(result),path_begin,drive_len*sizeof(DEE_CHAR));
    DEE_STRING_STR(result)[drive_len] = DEE_CHAR_C(DRIVE_SEP);
    if (IS_SEP(new_path_end[1]))
     DEE_STRING_STR(result)[drive_len+1] = new_path_end[1];
    else DEE_STRING_STR(result)[drive_len+1] = DEE_CHAR_C(SEP);
   }
   return result;
  }
  ++new_path_end;
 }
 DEERETURN_EMPTYSTRING;
#else
 (void)path;
 DEERETURN_STATIC_STRING_EX(1,{DEE_CHAR_C(SEP)});
#endif
}

// Appends a missing trailing slash to the given path. e.g.: "/foo/bar" --> "/foo/bar/"
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathIncTrailObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end; Dee_size_t path_len;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_end = (path_begin = DEE_STRING_STR(path))+(path_len = DEE_STRING_SIZE(path));
 if (path_begin == path_end || !IS_SEP(path_end[-1])) {
  DeeObject *result = DEE_STRING_NewSized(path_len+1);
  if (result) {
   memcpy(DEE_STRING_STR(result),path_begin,path_len*sizeof(DEE_CHAR));
   DEE_STRING_STR(result)[path_len] = DEE_CHAR_C(SEP);
  }
  return result;
 }
 DeeReturn_NEWREF(path);
}

// Exclude all trailing slashes from a given path. e.g.: "/foo/bar///" --> "/foo/bar"
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExcTrailObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_end; Dee_size_t path_len;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_end = path_end = (path_begin = DEE_STRING_STR(path))+(path_len = DEE_STRING_SIZE(path));
 while (new_path_end != path_begin && IS_SEP(new_path_end[-1])) --new_path_end;
 if (path_end == new_path_end) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(new_path_end-path_begin),path_begin);
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsAbs)(
 DEE_A_IN_Z DEE_CHAR const *path) {
 DEE_ASSERT(path);
#if defined(DRIVE_SEP)
 if (!DEE_CH_IS_ALNUM(*path)) return IS_SEP(*path);
 do ++path; while (DEE_CH_IS_ALNUM(*path));
 return IS_DRIVE_SEP(*path);
#else
 return IS_SEP(*path);
#endif
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_F(IsAbsObject)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 int temp; DeeObject *path_ob; DEE_CHAR const *path_s,*path_iter;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_s = DEE_STRING_STR(path);
 if (*path_s == DEE_CHAR_C('~')) return 1;
#if defined(DRIVE_SEP)
 if (*path_s == DEE_CHAR_C('$')
  || *path_s == DEE_CHAR_C('%')) {
expand:
#if 1
  if ((path_ob = DeeFS_F(PathExpandVarsObject)(path)) == NULL) return -1;
#else
  if ((path_ob = DeeFS_F(PathExpandObject)(path)) == NULL) return -1;
#endif
  path_s = DEE_STRING_STR(path_ob);
  if (!DEE_CH_IS_ALNUM(*path_s)) temp = 0; else {
   do ++path_s; while (DEE_CH_IS_ALNUM(*path_s));
   temp = IS_DRIVE_SEP(*path_s);
  }
  Dee_DECREF(path_ob);
  return temp;
 } else {
  path_iter = path_s;
  if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
  do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter));
  if (*path_iter == DEE_CHAR_C('~')) return 1;
  if (*path_iter == DEE_CHAR_C('$')
   || *path_iter == DEE_CHAR_C('%')
   ) goto expand;
  return IS_DRIVE_SEP(*path_iter);
 }
#else
 if (*path_s == DEE_CHAR_C('$')
  || *path_s == DEE_CHAR_C('%')) {
  if ((path_ob = DeeFS_F(PathExpandVarsObject)(path)) == NULL) return -1;
  temp = IS_SEP(*DEE_STRING_STR(path_ob));
  Dee_DECREF(path_ob);
  return temp;
 } else {
  return IS_SEP(*path_s);
 }
#endif
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsAbs)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; DEE_CHAR const *path_iter;
 int temp; DEE_ASSERT(path);
 if (*path == DEE_CHAR_C('~')) return 1;
#if defined(DRIVE_SEP)
 if (*path == DEE_CHAR_C('$')
  || *path == DEE_CHAR_C('%')) {
expand:
  if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
  path = DEE_STRING_STR(path_ob);
  if (!DEE_CH_IS_ALNUM(*path)) temp = 0; else {
   do ++path; while (DEE_CH_IS_ALNUM(*path));
   temp = IS_DRIVE_SEP(*path);
  }
  Dee_DECREF(path_ob);
  return temp;
 } else {
  path_iter = path;
  if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
  do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter));
  if (*path_iter == DEE_CHAR_C('~')) return 1;
  if (*path_iter == DEE_CHAR_C('$')
   || *path_iter == DEE_CHAR_C('%')) goto expand;
  return IS_DRIVE_SEP(*path_iter);
 }
#else
 if (*path == DEE_CHAR_C('$')
  || *path == DEE_CHAR_C('%')) {
  if ((path_ob = DeeFS_F(PathExpandVars)(path)) == NULL) return -1;
  temp = IS_SEP(*DEE_STRING_STR(path_ob));
  Dee_DECREF(path_ob);
  return temp;
 } else {
  return IS_SEP(*path);
 }
#endif
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsDrive)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_CHAR const *path_iter;
 DeeObject *path_ob; int temp;
 if (*path == DEE_CHAR_C('$')
  || *path == DEE_CHAR_C('%')
  || *path == DEE_CHAR_C('~')) {
expand:
  if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
  path = DEE_STRING_STR(path_ob);
  if (!DEE_CH_IS_ALNUM(*path)) temp = 0; else {
   do ++path; while (DEE_CH_IS_ALNUM(*path));
   temp = IS_DRIVE_SEP(*path) && !path[IS_SEP(path[1]) ? 2 : 1];
  }
  Dee_DECREF(path_ob);
  return temp;
 } else {
  path_iter = path;
  if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
  do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter));
  if (*path_iter == DEE_CHAR_C('$')
   || *path_iter == DEE_CHAR_C('%')
   || *path_iter == DEE_CHAR_C('~')) goto expand;
  return IS_DRIVE_SEP(*path_iter) && !path_iter[IS_SEP(path_iter[1]) ? 2 : 1];
 }
#else
 (void)path;
 return 0;
#endif
}
#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeFS_F(Win32IsDriveObject)(
 DEE_A_IN_OBJECT(DEE_STRING_OBJECT) const *path) {
 DEE_CHAR const *path_s,*path_iter; int temp;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_s = DEE_STRING_STR(path);
 if (*path_s == DEE_CHAR_C('$')
  || *path_s == DEE_CHAR_C('%')
  || *path_s == DEE_CHAR_C('~')) {
expand:
  if ((path = DeeFS_F(PathExpandObject)(path)) == NULL) return -1;
  path_s = DEE_STRING_STR(path);
  if (!DEE_CH_IS_ALNUM(*path_s)) temp = 0; else {
   do ++path_s; while (DEE_CH_IS_ALNUM(*path_s));
   temp = IS_DRIVE_SEP(*path_s) && !path_s[IS_SEP(path_s[1]) ? 2 : 1];
  }
  Dee_DECREF(path);
  return temp;
 } else {
  path_iter = path_s;
  if (!DEE_CH_IS_ALNUM(*path_iter)) return 0;
  do ++path_iter; while (DEE_CH_IS_ALNUM(*path_iter));
  if (*path_iter == DEE_CHAR_C('$')
   || *path_iter == DEE_CHAR_C('%')
   || *path_iter == DEE_CHAR_C('~')) goto expand;
  return IS_DRIVE_SEP(*path_iter) && !path_iter[IS_SEP(path_iter[1]) ? 2 : 1];
 }
}
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_F(ChmodStringToModeWithHandle)(
 DEE_A_IN_Z DEE_CHAR const *filename, DEE_A_IN DeeIOHandle file,
 DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode) {
 int opkind = 0; char const *mode_iter = mode_str;
 Dee_uint32_t channels = 0;
 Dee_uint32_t op_flags = 0;
 while (1) switch (*mode_iter++) {
  case 'u': case 'U': channels |= 0700; break;
  case 'g': case 'G': channels |= 0070; break;
  case 'o': case 'O': channels |= 0007; break;
  default: --mode_iter; goto p_opkind;
 }
p_opkind:
 while (1) switch (*mode_iter++) {
  case '+': opkind = 1; break;
  case '-': opkind = 2; break;
  case '=': opkind = 3; break;
  default: --mode_iter; goto p_opflags;
 }
p_opflags:
 while (1) switch (*mode_iter++) {
  case 'r': case 'R': op_flags |= 0444; break;
  case 'w': case 'W': op_flags |= 0222; break;
  case 'x': case 'X': op_flags |= 0111; break;
  default: --mode_iter; goto end;
 }
end:
 if (*mode_iter) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "chmod(" DEE_PRINTF_STRQ ",%q) : Invalid chmod format",
                      filename,mode_str);
  return -1;
 }
 if (!channels) channels = 0777;
 if (opkind == 3) *mode = op_flags&channels; else {
  Dee_mode_t old_mode;
  if (_DeeFS_F(GetModWithHandle)(filename,file,&old_mode) == -1) return -1;
  *mode = old_mode&0777;
  if (opkind == 1) *mode |= op_flags&channels;
  else if (opkind == 2) *mode &= ~(op_flags&channels);
 }
 return 0;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeFS_F(ChmodStringToMode)(
 DEE_A_IN_Z DEE_CHAR const *filename,
 DEE_A_IN_Z char const *mode_str, DEE_A_OUT Dee_mode_t *mode) {
 int opkind = 0; char const *mode_iter = mode_str;
 Dee_uint32_t channels = 0;
 Dee_uint32_t op_flags = 0;
 while (1) switch (*mode_iter++) {
  case 'u': case 'U': channels |= 0700; break;
  case 'g': case 'G': channels |= 0070; break;
  case 'o': case 'O': channels |= 0007; break;
  default: --mode_iter; goto p_opkind;
 }
p_opkind:
 while (1) switch (*mode_iter++) {
  case '+': opkind = 1; break;
  case '-': opkind = 2; break;
  case '=': opkind = 3; break;
  default: --mode_iter; goto p_opflags;
 }
p_opflags:
 while (1) switch (*mode_iter++) {
  case 'r': case 'R': op_flags |= 0444; break;
  case 'w': case 'W': op_flags |= 0222; break;
  case 'x': case 'X': op_flags |= 0111; break;
  default: --mode_iter; goto end;
 }
end:
 if (*mode_iter) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "chmod(" DEE_PRINTF_STRQ ",%q) : Invalid chmod format",
                      filename,mode_str);
  return -1;
 }
 if (!channels) channels = 0777;
 if (opkind == 3) *mode = op_flags&channels; else {
  Dee_mode_t old_mode;
  if (_DeeFS_F(GetMod)(filename,&old_mode) == -1) return -1;
  *mode = old_mode&0777;
  if (opkind == 1) *mode |= op_flags&channels;
  else if (opkind == 2) *mode &= ~(op_flags&channels);
 }
 return 0;
}

















DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathHead)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathHeadObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathTail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathTailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathFile)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathFileObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExt)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExtObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathDrive)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathDriveObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathIncTrail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathIncTrailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExcTrail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExcTrailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(PathAbs)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 if ((cwd_ob = DeeFS_F(PathExpand)(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathAbsObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeFS_F(PathRel)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 if ((cwd_ob = DeeFS_F(PathExpand)(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathRelObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *_DeeFS_F(PathAbs)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 if ((cwd_ob = DEE_STRING_New(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathAbsObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *_DeeFS_F(PathRel)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 if ((cwd_ob = DEE_STRING_New(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathRelObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExpand)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandVarsObject)(path_ob);
 Dee_DECREF(path_ob);
 if (!result) return NULL;
 path_ob = DeeFS_F(PathExpandUserObject)(result);
 Dee_DECREF(result);
 return path_ob;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
DeeFS_F(PathExpandObject)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *path) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 if ((path = DeeFS_F(PathExpandVarsObject)(path)) == NULL) return NULL;;
 result = DeeFS_F(PathExpandUserObject)(path);
 Dee_DECREF(path);
 return result;
}




DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExpandVars)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandVarsObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(PathExpandUser)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandUserObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}









DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_F(GetATime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (_DeeFS_F(GetTimes2)(path,&result,NULL,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_F(GetCTime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (_DeeFS_F(GetTimes2)(path,NULL,&result,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
_DeeFS_F(GetMTime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (_DeeFS_F(GetTimes2)(path,NULL,NULL,&result) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_F(GetATime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (DeeFS_F(GetTimes2)(path,&result,NULL,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_F(GetCTime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (DeeFS_F(GetTimes2)(path,NULL,&result,NULL) == -1) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFS_F(GetMTime)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result;
 if (DeeFS_F(GetTimes2)(path,NULL,NULL,&result) == -1) return NULL;
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
_DeeFS_F(GetTimes)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *a,*c,*m,*result;
 if (_DeeFS_F(GetTimes2)(path,&a,&c,&m) == -1) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
DeeFS_F(GetTimes)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *a,*c,*m,*result;
 if (DeeFS_F(GetTimes2)(path,&a,&c,&m) == -1) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}




DEE_A_RET_EXCEPT(-1) int DeeFS_F(GetTimes2)(
 DEE_A_IN_Z DEE_CHAR const *path,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(GetTimes2)(DEE_STRING_STR(
  path_ob),tm_access,tm_creation,tm_modification);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(SetTimes2)(
 DEE_A_IN_Z DEE_CHAR const *path,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) const *tm_modification) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(SetTimes2)(DEE_STRING_STR(
  path_ob),tm_access,tm_creation,tm_modification);
 Dee_DECREF(path_ob);
 return result;
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsFile)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsFile)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsDir)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsDir)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsLink)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsLink)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsHidden)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsHidden)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsExecutable)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsExecutable)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsMount)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_F(IsDrive)(path);
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsMount)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
#else
 (void)path;
 return 0;
#endif
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsCharDev)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_FS_HAVE_IS_CHAR_DEV
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsCharDev)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsBlockDev)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_FS_HAVE_IS_BLOCK_DEV
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsBlockDev)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsFiFo)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_FS_HAVE_IS_NAMED_PIPE
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsFiFo)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(IsSocket)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef DEE_FS_HAVE_IS_SOCKET
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(IsSocket)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path);
 return result;
#else
 (void)path;
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFS_F(Exists)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(Exists)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_F(Remove)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(Remove)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(RmFile)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(RmFile)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(MkDir)(DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_uint32_t mode) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(MkDir)(DEE_STRING_STR(path_ob),mode);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(RmDir)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(RmDir)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(Copy)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if ((dst_ob = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(Copy)(DEE_STRING_STR(src_ob),DEE_STRING_STR(dst_ob));
 Dee_DECREF(dst_ob);
err_0: Dee_DECREF(src_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(Move)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if ((dst_ob = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(Move)(DEE_STRING_STR(src_ob),DEE_STRING_STR(dst_ob));
 Dee_DECREF(dst_ob);
err_0: Dee_DECREF(src_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(Link)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DeeObject *src_ob,*dst_ob; int result;
 if ((src_ob = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if ((dst_ob = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(Link)(DEE_STRING_STR(src_ob),DEE_STRING_STR(dst_ob));
 Dee_DECREF(dst_ob);
err_0: Dee_DECREF(src_ob);
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeFS_F(CopyWithProgress)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
 DeeObject *src_path,*dst_path; int result;
 if ((src_path = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if ((dst_path = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(CopyWithProgress)(
  DEE_STRING_STR(src_path),DEE_STRING_STR(dst_path),progress,closure);
 Dee_DECREF(dst_path);
err_0: Dee_DECREF(src_path);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeFS_F(ReadLink)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 result = _DeeFS_F(ReadLink)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_F(Chmod)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_mode_t mode) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(Chmod)(DEE_STRING_STR(path),mode);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(ChmodString)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z char const *mode) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(ChmodString)(DEE_STRING_STR(path),mode);
 Dee_DECREF(path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(GetMod)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_OUT Dee_mode_t *mode) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(GetMod)(DEE_STRING_STR(path_ob),mode);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFS_F(GetOwn)(
 DEE_A_IN_Z DEE_CHAR const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(GetOwn)(DEE_STRING_STR(path_ob),owner,group);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_F(ChDir)(
 DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return -1;
 result = _DeeFS_F(ChDir)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}


#undef DeeFS_F
#undef _DeeFS_F
#undef DeeVFS_F
#undef _DeeVFS_F
#undef WIN32_F
#undef DeeString_F
#undef DEE_STRINGOBJECT
#undef DEE_CHAR
#undef DEE_STRLEN
#undef DEE_PRINTF_STR
#undef DEE_PRINTF_STRQ
#ifdef DEE_PRIVATE_CHAR_C
#undef DEE_PRIVATE_CHAR_C
#endif
#undef DEE_CHAR_C
#undef DEERETURN_STATIC_STRING_EX
#undef DEE_EMPTYSTRING
#undef DEERETURN_EMPTYSTRING
#undef DEE_STRINGWRITER
#undef DeeStringWriter_F
#undef DEE_STRINGWRITER_INIT
#undef DEE_STRINGWRITER_Pack
#undef DEE_STRINGWRITER_Quit
#undef DEE_STRINGWRITER_SIZE
#undef DEE_STRINGWRITER_ELEM
#undef DEE_STRINGWRITER_STR
#undef DEE_STRING_Cast
#undef DEE_STRING_Check
#undef DEE_STRING_InplaceCast
#undef DEE_STRING_New
#undef DEE_STRING_NewSized
#undef DEE_STRING_NewWithLength
#undef DEE_STRING_Resize
#undef DEE_STRING_SIZE
#undef DEE_STRING_STR

#ifdef WIDE
#undef WIDE
#endif
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
