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
#define DEESTRINGOBJECT            DeeWideStringObject
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
#define DEESTRINGOBJECT            DeeUtf8StringObject
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



DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeFS_F(ChmodEx)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 Dee_mode_t newmode; DEE_ASSERT(mcc && mcv);
 if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET &&
     _DeeFS_F(GetMod)(path,&newmode) != 0) return -1;
 _DeeFS_ExecModeChange(&newmode,mcc,mcv);
 return _DeeFS_F(Chmod)(path,newmode);
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeFS_F(ChmodExObject)(
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 Dee_mode_t newmode; DEE_ASSERT(mcc && mcv);
 if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET &&
     _DeeFS_F(GetModObject)(path,&newmode) != 0) return -1;
 _DeeFS_ExecModeChange(&newmode,mcc,mcv);
 return _DeeFS_F(ChmodObject)(path,newmode);
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeFS_F(ChmodEx)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 Dee_mode_t newmode; DEE_ASSERT(mcc && mcv);
 if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET &&
     DeeFS_F(GetMod)(path,&newmode) != 0) return -1;
 _DeeFS_ExecModeChange(&newmode,mcc,mcv);
 return DeeFS_F(Chmod)(path,newmode);
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeFS_F(ChmodExObject)(
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 Dee_mode_t newmode; DEE_ASSERT(mcc && mcv);
 if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET &&
     DeeFS_F(GetModObject)(path,&newmode) != 0) return -1;
 _DeeFS_ExecModeChange(&newmode,mcc,mcv);
 return DeeFS_F(ChmodObject)(path,newmode);
}


#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_F(Win32PathUNC)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef WIDE
 DeeObject *result; Dee_size_t path_size; DEE_CHAR *iter,*end;
 DEE_ASSERTF(path,"Invalid path");
 DEE_ASSERTF(!DeeFS_WideWin32IsPathUNC(path),"Already a UNC path");
 path_size = Dee_WideStrLen(path);
 if DEE_UNLIKELY((result = DeeWideString_NewSized(4+path_size)) == NULL) return NULL;
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
 if DEE_UNLIKELY((widepath = DeeWideString_FromUtf8String(path)) == NULL) return NULL;
 result = DeeFS_WideWin32PathUNC(DeeWideString_STR(widepath));
 Dee_DECREF(widepath);
 return result;
#endif
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(Win32GetModuleName)(DEE_A_IN_OPT /*HMODULE*/void *module) {
 DeeObject *result; DWORD temp; Dee_size_t bufsize;
 if DEE_UNLIKELY((result = DEE_STRING_NewSized((
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
  if DEE_UNLIKELY(DEE_STRING_Resize(&result,temp) != 0) goto err_r;
 } else if (temp == bufsize && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
  if DEE_UNLIKELY(DEE_STRING_Resize(&result,(bufsize *= 2)) != 0) goto err_r;
  goto again;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeFS_F(Win32HardLink)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst) {
 DEE_ASSERT(src && dst);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(!WIN32_F(CreateHardLink)(dst,src,NULL)) {
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
   if DEE_UNLIKELY((unc_src = DeeFS_Utf8Win32IsPathUNC(src)
    ? DeeFS_Utf8Win32PathUNC(src)
    : DeeWideString_FromUtf8String(src)) == NULL) return -1;
   if DEE_UNLIKELY((unc_dst = DeeFS_Utf8Win32IsPathUNC(dst)
    ? DeeFS_Utf8Win32PathUNC(dst)
    : DeeWideString_FromUtf8String(dst)) == NULL) { error = -1; goto err_unc0; }
   error = _DeeFS_WideWin32HardLink(DeeWideString_STR(unc_src),
                                    DeeWideString_STR(unc_dst));
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
 if DEE_UNLIKELY((src_ob = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(Win32HardLink)(DEE_STRING_STR(src_ob),DEE_STRING_STR(dst_ob));
 Dee_DECREF(dst_ob);
err_0: Dee_DECREF(src_ob);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeFS_F(Win32Drives)(void) {
 DEE_CHAR *buffer,*new_buffer,*iter,*end;
 DeeObject *result,*entry;
 DWORD req_bufsize,used_bufsize; int error;
 while DEE_UNLIKELY((buffer = (DEE_CHAR *)malloc_nz((
  DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES+1)*sizeof(DEE_CHAR))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
  DeeError_NoMemory();
  return NULL;
 }
 used_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES;
again:
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) { free_nn(buffer); return NULL; }
 if DEE_UNLIKELY((req_bufsize = WIN32_F(GetLogicalDriveStrings)(used_bufsize,buffer)) == 0) {
  free_nn(buffer);
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(WIN32_F(GetLogicalDriveStrings)) "() : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 if DEE_UNLIKELY(req_bufsize > DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES) {
  while DEE_UNLIKELY((new_buffer = (DEE_CHAR *)realloc_nnz(
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
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) goto end;
 while (iter < end) {
  if (!*iter) break;
  if DEE_UNLIKELY((entry = DEE_STRING_New(iter)) == NULL) {
err_r: Dee_CLEAR(result); goto end;
  }
  iter += DEE_STRLEN(iter)+1;
  error = DeeList_Append(result,entry);
  Dee_DECREF(entry);
  if DEE_UNLIKELY(error != 0) goto err_r;
 }
end:
 free_nn(buffer);
 return result;
}
#endif


#ifdef WIDE
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *) _dee_fs_wide_gen_initial_tmpname
#else
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *) _dee_fs_utf8_gen_initial_tmpname
#endif
(DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix, DEE_A_OUT DEE_CHAR **digits) {
 static DEE_CHAR const name_0000temp[] = {'0','0','0','0','.','t','m','p',0};
 DeeObject *result; Dee_size_t digit_offset,path_len;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 DEE_ASSERT(path && prefix && digits);
 path_len = DEE_STRLEN(path);
 while (path_len && IS_SEP(path[path_len-1])) --path_len;
 if (DEE_UNLIKELY(DeeStringWriter_F(WriteMyStringWithLength)(&writer,path_len,path) != 0)
  || DEE_UNLIKELY(DeeStringWriter_F(WriteMyChar)(&writer,SEP) != 0)
  || DEE_UNLIKELY(DeeStringWriter_F(WriteMyString)(&writer,prefix) != 0)
  || DEE_UNLIKELY(DeeStringWriter_F(WRITE_MY_STRING)(&writer,name_0000temp) != 0)) goto err;
 digit_offset = DeeStringWriter_F(SIZE)(&writer)-8;
 result = DEE_STRINGWRITER_Pack(&writer);
 if DEE_LIKELY(result) *digits = DEE_STRING_STR(result)+digit_offset;
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
 if DEE_UNLIKELY(_deefs_wide_inc_digit(digits+3)) if DEE_UNLIKELY(_deefs_wide_inc_digit(digits+2))
 if DEE_UNLIKELY(_deefs_wide_inc_digit(digits+1)) if DEE_UNLIKELY(_deefs_wide_inc_digit(digits+0))
#else
 if DEE_UNLIKELY(_deefs_utf8_inc_digit(digits+3)) if DEE_UNLIKELY(_deefs_utf8_inc_digit(digits+2))
 if DEE_UNLIKELY(_deefs_utf8_inc_digit(digits+1)) if DEE_UNLIKELY(_deefs_utf8_inc_digit(digits+0))
#endif
 {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "tmpname(): Too many temporary files");
  return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *_DeeFS_F(GetTmpName)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix) {
 DeeObject *result; DEE_CHAR *digits;
#ifdef WIDE
 if DEE_UNLIKELY((result = _dee_fs_wide_gen_initial_tmpname(path,prefix,&digits)) == NULL) return NULL;
#else
 if DEE_UNLIKELY((result = _dee_fs_utf8_gen_initial_tmpname(path,prefix,&digits)) == NULL) return NULL;
#endif
 while (_DeeFS_F(IsFile)(DEE_STRING_STR(result))) {
#ifdef WIDE
  if DEE_UNLIKELY(_deefs_wide_inc_tmpname(digits) != 0)
#else
  if DEE_UNLIKELY(_deefs_utf8_inc_tmpname(digits) != 0)
#endif
  {
   Dee_DECREF(result);
   return NULL;
  }
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeFS_F(GetTmpName)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *prefix) {
 DeeObject *path_ob,*result;
 if DEE_UNLIKELY((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 result = _DeeFS_F(GetTmpName)(DEE_STRING_STR(path_ob),prefix);
 Dee_DECREF(path_ob);
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) WIN32_F(fixed_GetFileAttributes)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_OUT DWORD *attr) {
 DWORD error;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY((*attr = WIN32_F(GetFileAttributes)(path)) == INVALID_FILE_ATTRIBUTES) {
  error = GetLastError();
  if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
     !DeeFS_F(Win32IsPathUNC)(path)) {
   DeeObject *unc_path;
   if DEE_UNLIKELY((unc_path = DeeFS_F(Win32PathUNC)(path)) == NULL) return -1;
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

static DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
_DeeFS_F(Win32ReadLinkHandle)(DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN HANDLE hfile) {
 DeeObject *result; DWORD bytesReturned; Dee_size_t bufsize;
 REPARSE_DATA_BUFFER *buffer;
again:
 if DEE_UNLIKELY((buffer = (REPARSE_DATA_BUFFER *)malloc_nz(
  (bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32READLINK))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
no_mem: DeeError_NoMemory();
  return NULL;
 }
 while (bufsize < MAXIMUM_REPARSE_DATA_BUFFER_SIZE) {
  // Make up the control code - see CTL_CODE on ntddk.h
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_buffer;
  if DEE_UNLIKELY(!DeviceIoControl(hfile,FSCTL_GET_REPARSE_POINT,
   NULL,0,buffer,(DWORD)bufsize,&bytesReturned,NULL)) {
   DWORD error = DeeSystemError_Win32Consume();
   if (error == ERROR_NOT_A_REPARSE_POINT) {
    free_nn(buffer); // Not a reparse point --> return the path itself
    return DEE_STRING_New(path);
   } else if (error == ERROR_INSUFFICIENT_BUFFER) {
    REPARSE_DATA_BUFFER *new_buffer;
    while DEE_UNLIKELY((new_buffer = (REPARSE_DATA_BUFFER *)
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


DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
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
   DWORD error = (DWORD)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_F(Win32IsPathUNC)(path)) {
    DeeObject *temp;
    if DEE_UNLIKELY((temp = DeeFS_F(Win32PathUNC)(path)) == NULL) return NULL;
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
  if DEE_UNLIKELY(!CloseHandle(hfile)) SetLastError(0);
  return result;
#elif defined(WIDE)
  DeeObject *result,*path_ob;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
  result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  if (result && DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)&result) != 0)) Dee_CLEAR(result);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t bytes; DeeObject *result; int error;
  if DEE_UNLIKELY((result = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) == NULL) return NULL;
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
  bytes = (Dee_ssize_t)readlink(path,DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);
again:
  if DEE_UNLIKELY(bytes < 0) {
err_sys:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "readlink(" DEE_PRINTF_STRQ ") : %K",path,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
err_r:
   Dee_DECREF(result);
   return NULL;
  } else if DEE_UNLIKELY(bytes > DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK) {
   Dee_ssize_t new_bytes;
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,bytes) != 0) goto err_r;
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_r;
   new_bytes = (Dee_ssize_t)readlink(
    path,DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);
   if (new_bytes < 0) goto err_sys;
   if DEE_UNLIKELY(new_bytes != bytes) {
    bytes = new_bytes; // Link must have changed
    goto again;
   }
  } else if (DEE_LIKELY(bytes < DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) {
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,bytes) != 0) goto err_r;
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathJoinObject)(DEE_A_IN Dee_size_t pathc, DEE_A_IN_R(pathc) DeeObject const *const *pathv) {
 DeeObject *result; DeeObject const *const *iter,*const *end,*elem;
 DEE_CHAR const *path_begin,*path_end;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 end = (iter = pathv)+pathc;
 while (iter != end) {
  if DEE_UNLIKELY((elem = DEE_STRING_Cast(*iter)) == NULL) goto err;
  path_end = (path_begin = DEE_STRING_STR(elem))+DEE_STRING_SIZE(elem);
  if (iter != pathv) while (path_begin < path_end && IS_SEP(*path_begin)) ++path_begin;
  if (iter != end-1) while (path_end > path_begin && IS_SEP(path_end[-1])) --path_end;
  if (path_begin != path_end) {
   if (iter != pathv && DEE_STRINGWRITER_SIZE(&writer) != 0 &&
       !IS_SEP(DEE_STRINGWRITER_ELEM(&writer)[DEE_STRINGWRITER_SIZE(&writer)-1]) &&
       DEE_UNLIKELY(DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C(SEP)) != 0)) goto err_elem;
   if DEE_UNLIKELY(DeeStringWriter_F(WriteMyStringWithLength)(
    &writer,(Dee_size_t)(path_end-path_begin),path_begin) != 0) goto err_elem;
  }
#if DEE_CONFIG_RUNTIME_HAVE_VFS
  else if (iter == pathv && IS_SEP(*DEE_STRING_STR(elem))) {
   if DEE_UNLIKELY(DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C(SEP)) != 0) goto err_elem;
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathExpandVarsObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DEE_CHAR ch,*iter,*end,*var_begin,*var_end,*var_name;
 DeeObject *var_value,*result; Dee_size_t varname_len; int temp;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 end = (iter = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY(*iter == DEE_CHAR_C('$') || *iter == DEE_CHAR_C('%')
                  ) goto begin;
  ++iter;
 }
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
     if DEE_UNLIKELY(temp != 0) goto err;
    } else { // Unknown variable (ignore + don't expand)
     if (*--var_begin == DEE_CHAR_C('{')) --var_begin;
     if DEE_UNLIKELY(DeeStringWriter_F(WriteMyStringWithLength)(
      &writer,(Dee_size_t)(iter-var_begin),var_begin) != 0) goto err;
    }
   } else if DEE_UNLIKELY(ch == DEE_CHAR_C('%')) {
    var_begin = iter;
    while (iter != end && *iter != DEE_CHAR_C('%')) ++iter;
    if DEE_LIKELY((var_end = iter) != end) ++iter;
    goto put_var;
   } else if DEE_UNLIKELY(DeeStringWriter_F(WriteMyChar)(&writer,ch) != 0) {
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


DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathExpandUserObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
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
   while DEE_UNLIKELY((user = (DEE_CHAR *)malloc_nz((((
    Dee_size_t)(path_begin-user_begin))+1)*sizeof(DEE_CHAR))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
  }
#endif
   memcpy(user,user_begin,(Dee_size_t)(path_begin-user_begin)*sizeof(DEE_CHAR));
   user[path_begin-user_begin] = DEE_CHAR_C('\0');
   result = DeeFS_F(GetUserHome)(user);
#if !DEE_HAVE_ALLOCA
   free_nn(user);
#endif
   if DEE_UNLIKELY(!result) return NULL;
  } else {
   if DEE_UNLIKELY((result = DeeFS_F(GetHome)()) == NULL) return NULL;
  }
#if 0
  while (path_begin != path_end && IS_SEP(*path_begin)) ++path_begin;
#endif
  if DEE_UNLIKELY(DeeString_F(AppendWithLength)(
   &result,(Dee_size_t)(path_end-path_begin),path_begin) != 0)
  { Dee_DECREF(result); result = NULL; }
  return result;
 } else DeeReturn_NEWREF(path);
}








static DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *_DeeFS_F(PathAbsObject)(
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *cwd) {
 DEE_CHAR const *path_begin,*path_end,*cwd_begin,*cwd_end;
 DeeObject *result,*real_cwd,*new_cwd; DEE_CHAR *result_iter;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 DEE_ASSERT(DeeObject_Check(cwd) && DEE_STRING_Check(cwd));
 if (_DeeFS_F(IsAbs)(DEE_STRING_STR(path))) DeeReturn_NEWREF(path);
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(cwd))) {
  // Make sure 'cwd' is absolute
  if DEE_UNLIKELY((real_cwd = DeeFS_F(GetCwd)()) == NULL) return NULL;
  new_cwd = _DeeFS_F(PathAbsObject)(cwd,real_cwd);
  Dee_DECREF(real_cwd);
  if DEE_UNLIKELY(!new_cwd) return NULL;
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
  if DEE_UNLIKELY((result = DEE_STRING_NewSized(
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



static DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *_DeeFS_F(PathRelObject)(
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *cwd) {
 DEE_CHAR const *path_begin,*path_end,*path_part,*cwd_begin,*cwd_end,*cwd_part;
 DEE_CHAR *result_iter; unsigned int up_refs; DeeObject *result,*real_cwd = NULL,*temp;
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(path))) {
  if DEE_UNLIKELY((real_cwd = DeeFS_F(GetCwd)()) == NULL) {err_p: Dee_DECREF(path); return NULL; }
  if DEE_UNLIKELY((temp = _DeeFS_F(PathAbsObject)(path,real_cwd)) == NULL) {
err_rc_p: Dee_DECREF(real_cwd); goto err_p;
  }
  Dee_INHERIT_REF(path,temp);
 } else Dee_INCREF(path);
 if (!_DeeFS_F(IsAbs)(DEE_STRING_STR(cwd))) {
  if (!real_cwd && DEE_UNLIKELY((real_cwd = DeeFS_F(GetCwd)()) == NULL)) { Dee_DECREF(cwd); goto err_p; }
  if DEE_UNLIKELY((temp = _DeeFS_F(PathAbsObject)(cwd,real_cwd)) == NULL) goto err_rc_p;
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
 } else if DEE_UNLIKELY((result = DEE_STRING_NewSized((Dee_size_t)(
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathHeadObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_end;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_end = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_end != path_begin && !IS_SEP(new_path_end[-1])) --new_path_end;
 while (new_path_end != path_begin && IS_SEP(new_path_end[-2])) --new_path_end; // Skip multiple ending seps
 if (path_end == new_path_end) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(new_path_end-path_begin),path_begin);
}

// Returns the tail of a filename. e.g.: "/foo/bar.txt" --> "bar.txt"
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathTailObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_begin;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_begin = path_end = (path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_begin != path_begin && !IS_SEP(new_path_begin[-1])) --new_path_begin;
 if (path_begin == new_path_begin) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(path_end-new_path_begin),new_path_begin);
}

// Returns the name of a filename. e.g.: "/foo/bar.txt" --> "bar"
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathFileObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathExtObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathDriveObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
#ifdef DRIVE_SEP
 DEE_CHAR *path_begin,*path_end,*new_path_end;
 Dee_size_t drive_len; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_end = (new_path_end = path_begin = DEE_STRING_STR(path))+DEE_STRING_SIZE(path);
 while (new_path_end != path_end) {
  if (IS_DRIVE_SEP(*new_path_end)) {
   drive_len = (Dee_size_t)(new_path_end-path_begin);
   if DEE_UNLIKELY((result = DEE_STRING_NewSized(drive_len+2)) != NULL) {
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
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *)
DeeFS_F(PathIncTrailObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end; Dee_size_t path_len; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 path_end = (path_begin = DEE_STRING_STR(path))+(path_len = DEE_STRING_SIZE(path));
 if (path_begin == path_end || !IS_SEP(path_end[-1])) {
  if DEE_UNLIKELY((result = DEE_STRING_NewSized(path_len+1)) != NULL) {
   memcpy(DEE_STRING_STR(result),path_begin,path_len*sizeof(DEE_CHAR));
   DEE_STRING_STR(result)[path_len] = DEE_CHAR_C(SEP);
  }
  return result;
 }
 DeeReturn_NEWREF(path);
}

// Exclude all trailing slashes from a given path. e.g.: "/foo/bar///" --> "/foo/bar"
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExcTrailObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DEE_CHAR *path_begin,*path_end,*new_path_end; Dee_size_t path_len;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 new_path_end = path_end = (path_begin = DEE_STRING_STR(path))+(path_len = DEE_STRING_SIZE(path));
 while (new_path_end != path_begin && IS_SEP(new_path_end[-1])) --new_path_end;
 if (path_end == new_path_end) DeeReturn_NEWREF(path);
 return DEE_STRING_NewWithLength((Dee_size_t)(new_path_end-path_begin),path_begin);
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
  if DEE_UNLIKELY((path = DeeFS_F(PathExpandObject)(path)) == NULL) return -1;
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

















DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathHead)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathHeadObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathTail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathTailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathFile)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathFileObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExt)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExtObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathDrive)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathDriveObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathIncTrail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathIncTrailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExcTrail)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExcTrailObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeFS_F(PathAbs)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if DEE_UNLIKELY((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd_ob = DeeFS_F(PathExpand)(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathAbsObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeFS_F(PathRel)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if DEE_UNLIKELY((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd_ob = DeeFS_F(PathExpand)(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathRelObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *_DeeFS_F(PathAbs)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd_ob = DEE_STRING_New(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathAbsObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *_DeeFS_F(PathRel)(
 DEE_A_IN_Z DEE_CHAR const *path, DEE_A_IN_Z DEE_CHAR const *cwd) {
 DeeObject *result,*path_ob,*cwd_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 if DEE_UNLIKELY((cwd_ob = DEE_STRING_New(cwd)) == NULL) { Dee_DECREF(path_ob); return NULL; }
 result = _DeeFS_F(PathRelObject)(path_ob,cwd_ob);
 Dee_DECREF(cwd_ob);
 Dee_DECREF(path_ob);
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExpand)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandVarsObject)(path_ob);
 Dee_DECREF(path_ob);
 if DEE_UNLIKELY(!result) return NULL;
 path_ob = DeeFS_F(PathExpandUserObject)(result);
 Dee_DECREF(result);
 return path_ob;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExpandObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DEE_STRING_Check(path));
 if DEE_UNLIKELY((path = DeeFS_F(PathExpandVarsObject)(path)) == NULL) return NULL;;
 result = DeeFS_F(PathExpandUserObject)(path);
 Dee_DECREF(path);
 return result;
}




DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExpandVars)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandVarsObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(PathExpandUser)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DEE_STRING_New(path)) == NULL) return NULL;
 result = DeeFS_F(PathExpandUserObject)(path_ob);
 Dee_DECREF(path_ob);
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeFS_F(CopyWithProgress)(
 DEE_A_IN_Z DEE_CHAR const *src, DEE_A_IN_Z DEE_CHAR const *dst,
 DEE_A_IN DeeFS_ProcessFunc progress, void *closure) {
 DeeObject *src_path,*dst_path; int result;
 if DEE_UNLIKELY((src_path = DeeFS_F(PathExpand)(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_path = DeeFS_F(PathExpand)(dst)) == NULL) { result = -1; goto err_0; }
 result = _DeeFS_F(CopyWithProgress)(
  DEE_STRING_STR(src_path),DEE_STRING_STR(dst_path),progress,closure);
 Dee_DECREF(dst_path);
err_0: Dee_DECREF(src_path);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeFS_F(ReadLink)(DEE_A_IN_Z DEE_CHAR const *path) {
 DeeObject *result,*path_ob;
 if DEE_UNLIKELY((path_ob = DeeFS_F(PathExpand)(path)) == NULL) return NULL;
 result = _DeeFS_F(ReadLink)(DEE_STRING_STR(path_ob));
 Dee_DECREF(path_ob);
 return result;
}

#undef DeeFS_F
#undef _DeeFS_F
#undef DeeVFS_F
#undef _DeeVFS_F
#undef WIN32_F
#undef DeeString_F
#undef DEESTRINGOBJECT
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
