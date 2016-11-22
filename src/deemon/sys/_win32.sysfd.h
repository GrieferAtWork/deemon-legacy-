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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSFD_H
#define GUARD_DEEMON_SYS__WIN32_SYSFD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/deemonrun.h>
#include <deemon/fs_api.h>
#include <deemon/string.h>
#include <deemon/file.h>
#include <deemon/type.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Aclapi.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include DEE_INCLUDE_MEMORY_API()


#ifndef SIZEOF_DWORD
#define SIZEOF_DWORD 4
#endif
#ifndef DWORD_MAX
#define DWORD_MAX 0xFFFFFFFF
#endif


// v not what you think it is...
#undef FILE_TYPE_DISK

DEE_DECL_BEGIN


#define DEE_WIN32_SYSFD_HEAD  HANDLE w32_handle; /*< [1..1] File handle. */
struct DeeWin32SysFD { DEE_WIN32_SYSFD_HEAD };

#define DEE_WIN32_SYSFD_SEEK_SET  FILE_BEGIN
#define DEE_WIN32_SYSFD_SEEK_CUR  FILE_CURRENT
#define DEE_WIN32_SYSFD_SEEK_END  FILE_END

#define DeeWin32SysFD_Quit(self) \
 (DEE_LIKELY(CloseHandle((self)->w32_handle)) ? (void)0 : SetLastError(0))

#define DeeWin32SysFD_GET_STDIN(self)  (void)((self)->w32_handle = GetStdHandle(STD_INPUT_HANDLE))
#define DeeWin32SysFD_GET_STDOUT(self) (void)((self)->w32_handle = GetStdHandle(STD_OUTPUT_HANDLE))
#define DeeWin32SysFD_GET_STDERR(self) (void)((self)->w32_handle = GetStdHandle(STD_ERROR_HANDLE))

#define DeeWin32SysFD_TryInitCopy(self,right) \
 DuplicateHandle(GetCurrentProcess(),(right)->w32_handle,\
                 GetCurrentProcess(),&(self)->w32_handle,\
                 0,TRUE,DUPLICATE_SAME_ACCESS)
#define DeeWin32SysFD_InitCopy(self,right,...) \
do{\
 if DEE_UNLIKELY(!DeeWin32SysFD_TryInitCopy(self,right)) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      "DuplicateHandle(GetCurrentProcess(),%p,GetCurrentProcess(),...,0,TRUE,DUPLICATE_SAME_ACCESS) : %K",\
                      (right)->w32_handle,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
static BOOL DeeWin32SysFD_LargeRead64(HANDLE hFile, void *p, Dee_uint64_t s, Dee_uint64_t *rs) {
 DWORD temp; DEE_ASSERTF(s > DWORD_MAX,"Not a large read");
 if DEE_UNLIKELY(!ReadFile(hFile,p,DWORD_MAX,(LPDWORD)&temp,NULL)) return FALSE;
 *rs = temp;
 while (s > temp) {
  *(uintptr_t *)&p += temp; s -= temp;
  if DEE_UNLIKELY(!ReadFile(hFile,p,s >= DWORD_MAX
    ? (DWORD)DWORD_MAX : (DWORD)s,(LPDWORD)&temp,NULL)) return FALSE;
  *rs += temp;
 }
 return TRUE;
}
static BOOL DeeWin32SysFD_LargeWrite64(HANDLE hFile, void const *p, Dee_uint64_t s, Dee_uint64_t *ws) {
 DWORD temp; DEE_ASSERTF(s > DWORD_MAX,"Not a large read");
 if DEE_UNLIKELY(!WriteFile(hFile,p,DWORD_MAX,(LPDWORD)&temp,NULL)) return FALSE;
 *ws = temp;
 while (s > temp) {
  *(uintptr_t *)&p += temp; s -= temp;
  if DEE_UNLIKELY(!WriteFile(hFile,p,s >= DWORD_MAX
    ? (DWORD)DWORD_MAX : (DWORD)s,(LPDWORD)&temp,NULL)) return FALSE;
  *ws += temp;
 }
 return TRUE;
}
#if DEE_PLATFORM_ENDIAN == 1234
#define DEE_WIN32_SYSFD_64LS32(x) ((Dee_uint32_t)(x)+1)
#define DEE_WIN32_SYSFD_64MS32(x) ((Dee_uint32_t)(x))
#else
#define DEE_WIN32_SYSFD_64LS32(x) ((Dee_uint32_t)(x))
#define DEE_WIN32_SYSFD_64MS32(x) ((Dee_uint32_t)(x)+1)
#endif

#define DeeWin32SysFD_DoTryRead(self,p,s,rs)\
 ((s) > DWORD_MAX ? DeeWin32SysFD_LargeRead64((self)->w32_handle,p,s,rs)\
  : (*DEE_WIN32_SYSFD_64MS32(rs) = 0,ReadFile((self)->w32_handle,p,(DWORD)(s),(LPDWORD)DEE_WIN32_SYSFD_64LS32(rs),NULL)))
#define DeeWin32SysFD_DoTryWrite(self,p,s,ws)\
 ((s) > DWORD_MAX ? DeeWin32SysFD_LargeWrite64((self)->w32_handle,p,s,ws)\
  : (*DEE_WIN32_SYSFD_64MS32(ws) = 0,WriteFile((self)->w32_handle,p,(DWORD)(s),(LPDWORD)DEE_WIN32_SYSFD_64LS32(ws),NULL)))
#else
#define DeeWin32SysFD_DoTryRead(self,p,s,rs)\
 ReadFile((self)->w32_handle,p,s,(LPDWORD)(rs),NULL)
#define DeeWin32SysFD_DoTryWrite(self,p,s,ws)\
 WriteFile((self)->w32_handle,p,s,(LPDWORD)(ws),NULL)
#endif

#define DeeWin32SysFD_TryRead(self,p,s,rs) \
 (DeeWin32SysFD_DoTryRead(self,p,s,rs) ? TRUE :\
 (GetLastError() == ERROR_BROKEN_PIPE) ? (*(rs) = 0,TRUE) : FALSE)
#define DeeWin32SysFD_TryWrite(self,p,s,rs) \
 (DeeWin32SysFD_DoTryWrite(self,p,s,rs) ? TRUE :\
 (GetLastError() == ERROR_BROKEN_PIPE) ? (*(rs) = 0,TRUE) : FALSE)

#define DeeWin32SysFD_TrySeek(self,off,whence,newpos)\
 DeeWin32SysFD_TrySeekHandle((self)->w32_handle,off,whence,newpos)

static BOOL DeeWin32SysFD_TrySeekHandle(
 HANDLE hFile, Dee_int64_t off, DWORD whence, Dee_uint64_t *newpos) {
 union{ Dee_int64_t off64; Dee_uint64_t uoff64; DWORD offd[2]; LONG offl[2]; } used_offset; used_offset.off64 = off;
 if DEE_UNLIKELY((used_offset.offd[0] = SetFilePointer(hFile,
  used_offset.offl[0],&used_offset.offl[1],
  (DWORD)whence)) == INVALID_SET_FILE_POINTER) {
  if (GetLastError() != 0) return FALSE;
 }
 if (newpos) *newpos = used_offset.uoff64;
 return TRUE;
}

#define DeeWin32SysFD_Seek(self,off,whence,newpos,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysFD_SeekHandle(\
  (self)->w32_handle,off,(DWORD)(whence),newpos)) {\
  {__VA_ARGS__;}\
 }\
}while(0)

static BOOL DeeWin32SysFD_SeekHandle(
 HANDLE hFile, Dee_int64_t off, DWORD whence, Dee_uint64_t *newpos) {
 union{ Dee_int64_t off64; Dee_uint64_t uoff64; DWORD offd[2]; LONG offl[2];
 } used_offset; used_offset.off64 = off;
 if DEE_UNLIKELY((used_offset.offd[0] = SetFilePointer(hFile,
  used_offset.offl[0],&used_offset.offl[1],
  (DWORD)whence)) == INVALID_SET_FILE_POINTER) {
  DWORD error = GetLastError();
  switch (error) {
   case 0: break;
   case ERROR_INVALID_FUNCTION:
   case ERROR_SEEK_ON_DEVICE:
    DeeError_NotImplemented_str("SetFilePointer");
    return FALSE;

#if 0
   case ERROR_INVALID_HANDLE:
    if DEE_UNLIKELY(hFile == DEE_FILEIO_INVALID_HANDLE) {
     // This can happen if we tried to open a directory
     if (newpos) *newpos = 0;
     return 0;
    }
    DEE_ATTRIBUTE_FALLTHROUGH
#endif

   default:
    DeeError_SetStringf(&DeeErrorType_IOError,
                        "SetFilePointer(%p,%I64u,%d) : %K",
                        hFile,off,whence,
                        DeeSystemError_Win32ToString(error));
    return -1;
  }
 }
 if (newpos) *newpos = used_offset.uoff64;
 return TRUE;
}


#define DeeWin32SysFD_Read(self,p,s,rs,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysFD_DoTryRead(self,p,s,rs)) {\
  DWORD _fd_error = GetLastError();\
  if (_fd_error == ERROR_BROKEN_PIPE) *(rs) = 0;\
  else {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "ReadFile(%p,%p,%Iu) : %K",\
                       (self)->w32_handle,p,(Dee_size_t)(s),\
                       DeeSystemError_Win32ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)

#define DeeWin32SysFD_Write(self,p,s,ws,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysFD_DoTryWrite(self,p,s,ws)) {\
  DWORD _fd_error = GetLastError();\
  if (_fd_error == ERROR_BROKEN_PIPE) *(ws) = 0;\
  else {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "WriteFile(%p,%p,%Iu:%.*q) : %K",\
                       (self)->w32_handle,p,(Dee_size_t)(s),(unsigned)(s),p,\
                       DeeSystemError_Win32ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)


#define DeeWin32SysFD_TryFlush(self) FlushFileBuffers((self)->w32_handle)
#define DeeWin32SysFD_Flush(self,...)\
do{\
 if (!DeeWin32SysFD_TryFlush(self)) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      "FlushFileBuffers(%p) : %K",(self)->w32_handle,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeWin32SysFD_TryTrunc(self) SetEndOfFile((self)->w32_handle)
#define DeeWin32SysFD_Trunc(self,...)\
do{\
 if (!DeeWin32SysFD_TryTrunc(self)) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      "SetEndOfFile(%p) : %K",(self)->w32_handle,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif


#ifndef DeeWin32Sys_FiletimeToTimetick // NOTE: Also implemented in "_win32.sysfd.h"
#define DeeWin32Sys_FiletimeToTimetick(ft) (((ft)/DEE_UINT64_C(10000))+DEE_UINT64_C(50522659200000))
#define DeeWin32Sys_TimetickToFiletime(tt) (((tt)-DEE_UINT64_C(50522659200000))*DEE_UINT64_C(10000))
#endif

DEE_STATIC_INLINE(BOOL) DeeWin32Sys_TryGetHandleTime(
 DEE_A_IN HANDLE hFile,
 DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime,
 DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 Dee_timetick_t temp;
 if DEE_UNLIKELY(!GetFileTime(hFile,(LPFILETIME)ctime,(LPFILETIME)atime,(LPFILETIME)mtime)) return FALSE;
 if (atime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)atime,(LPFILETIME)&temp)) return FALSE; *atime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (ctime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)ctime,(LPFILETIME)&temp)) return FALSE; *ctime = DeeWin32Sys_FiletimeToTimetick(temp); }
 if (mtime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)mtime,(LPFILETIME)&temp)) return FALSE; *mtime = DeeWin32Sys_FiletimeToTimetick(temp); }
 return TRUE;
}
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_TrySetHandleTime(
 DEE_A_IN HANDLE hFile,
 DEE_A_IN_OPT Dee_timetick_t const *atime,
 DEE_A_IN_OPT Dee_timetick_t const *ctime,
 DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 FILETIME fatime,fctime,fmtime,temp;
 if (atime) { *(Dee_timetick_t *)&temp = DeeWin32Sys_TimetickToFiletime(*atime); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&temp,&fatime)) return FALSE; }
 if (ctime) { *(Dee_timetick_t *)&temp = DeeWin32Sys_TimetickToFiletime(*ctime); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&temp,&fctime)) return FALSE; }
 if (mtime) { *(Dee_timetick_t *)&temp = DeeWin32Sys_TimetickToFiletime(*mtime); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&temp,&fmtime)) return FALSE; }
 return SetFileTime(hFile,
                    ctime ? &fctime : NULL,
                    atime ? &fatime : NULL,
                    mtime ? &fmtime : NULL);
}

#define DeeWin32Sys_GetHandleTime(hFile,atime,ctime,mtime,...) \
do{\
 Dee_timetick_t _ft_temp;\
 static char const *_ft_convert_error_message = "FileTimeToLocalFileTime(%I64u) : %K";\
 if DEE_UNLIKELY(!GetFileTime(hFile,(LPFILETIME)(ctime),\
                (LPFILETIME)(atime),(LPFILETIME)(mtime))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileTime(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 if (atime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)&_ft_temp,(LPFILETIME)(atime))) { DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(atime) = DeeWin32Sys_FiletimeToTimetick(_ft_temp); }\
 if (ctime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)&_ft_temp,(LPFILETIME)(ctime))) { DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(ctime) = DeeWin32Sys_FiletimeToTimetick(_ft_temp); }\
 if (mtime) { if DEE_UNLIKELY(!FileTimeToLocalFileTime((LPFILETIME)&_ft_temp,(LPFILETIME)(mtime))) { DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}} *(mtime) = DeeWin32Sys_FiletimeToTimetick(_ft_temp); }\
}while(0)
#define DeeWin32Sys_SetHandleTime(hFile,atime,ctime,mtime,...) \
do{\
 FILETIME _ft_atime,_ft_ctime,_ft_mtime,_ft_temp;\
 static char const *_ft_convert_error_message = "FileTimeToLocalFileTime(%I64u) : %K";\
 if (atime) { *(Dee_timetick_t *)&_ft_temp = DeeWin32Sys_TimetickToFiletime(*(atime)); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&_ft_temp,&_ft_atime)) DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,*(Dee_timetick_t *)&_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}}\
 if (ctime) { *(Dee_timetick_t *)&_ft_temp = DeeWin32Sys_TimetickToFiletime(*(ctime)); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&_ft_temp,&_ft_ctime)) DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,*(Dee_timetick_t *)&_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}}\
 if (mtime) { *(Dee_timetick_t *)&_ft_temp = DeeWin32Sys_TimetickToFiletime(*(mtime)); if DEE_UNLIKELY(!LocalFileTimeToFileTime(&_ft_temp,&_ft_mtime)) DeeError_SetStringf(&DeeErrorType_SystemError,_ft_convert_error_message,*(Dee_timetick_t *)&_ft_temp,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume())); {__VA_ARGS__;}}\
 if DEE_UNLIKELY(!SetFileTime(hFile,(ctime) ? &_ft_ctime : NULL,\
        (atime) ? &_ft_atime : NULL,(mtime) ? &_ft_mtime : NULL)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetFileTime(%p,%I64u,%I64u,%I64u) : %K",hFile,\
                      (atime) ? *(Dee_timetick_t *)&_ft_atime : (Dee_timetick_t)0,\
                      (ctime) ? *(Dee_timetick_t *)&_ft_ctime : (Dee_timetick_t)0,\
                      (mtime) ? *(Dee_timetick_t *)&_ft_mtime : (Dee_timetick_t)0,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32SysFileFD_TryGetTimes(self,atime,ctime,mtime)  DeeWin32Sys_TryGetHandleTime((self)->w32_handle,atime,ctime,mtime)
#define DeeWin32SysFileFD_TrySetTimes(self,atime,ctime,mtime)  DeeWin32Sys_TrySetHandleTime((self)->w32_handle,atime,ctime,mtime)
#define DeeWin32SysFileFD_GetTimes(self,atime,ctime,mtime,...) DeeWin32Sys_GetHandleTime((self)->w32_handle,atime,ctime,mtime,__VA_ARGS__)
#define DeeWin32SysFileFD_SetTimes(self,atime,ctime,mtime,...) DeeWin32Sys_SetHandleTime((self)->w32_handle,atime,ctime,mtime,__VA_ARGS__)

DEE_STATIC_INLINE(DWORD) DeeWin32Sys_TryGetFileAttributesFromHandle(HANDLE hFile) {
 BY_HANDLE_FILE_INFORMATION fileinfo;
 if (!GetFileInformationByHandle(hFile,&fileinfo)) return 0;
 return fileinfo.dwFileAttributes;
}
DEE_STATIC_INLINE(DWORD) DeeWin32Sys_GetFileAttributesFromHandle(HANDLE hFile) {
 BY_HANDLE_FILE_INFORMATION fileinfo;
 if (!GetFileInformationByHandle(hFile,&fileinfo)) return INVALID_FILE_ATTRIBUTES;
 // NOTE: If 0x80000000 even is a flag, it's one we're not looking for.
 //    >> So to make sure we're never, ever detecting an error where there
 //       is none, we ensure that 'dwFileAttributes' can't possibly be
 //       INVALID_FILE_ATTRIBUTES (with is 0xFFFFFFFF)
 return fileinfo.dwFileAttributes&0x7FFFFFFF;
}

extern DeeObject *DeeWin32Sys_Utf8GetHandleFilename(HANDLE hFile);
extern DeeObject *DeeWin32Sys_WideGetHandleFilename(HANDLE hFile);
#define DeeWin32SysFileFD_Utf8Filename(ob) DeeWin32Sys_Utf8GetHandleFilename((ob)->w32_handle)
#define DeeWin32SysFileFD_WideFilename(ob) DeeWin32Sys_WideGetHandleFilename((ob)->w32_handle)


#define DeeWin32Sys_TryHandleIsFile(hFile) \
((DeeWin32Sys_TryGetFileAttributesFromHandle(hFile)&\
 (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_REPARSE_POINT))==0)
#define DeeWin32Sys_TryHandleIsDir(hFile) \
 ((DeeWin32Sys_TryGetFileAttributesFromHandle(hFile)&(FILE_ATTRIBUTE_DIRECTORY))!=0)
#define DeeWin32Sys_TryHandleIsLink(hFile) \
 ((DeeWin32Sys_TryGetFileAttributesFromHandle(hFile)&(FILE_ATTRIBUTE_REPARSE_POINT))!=0)
#define DeeWin32Sys_TryHandleIsHidden(hFile) \
 ((DeeWin32Sys_TryGetFileAttributesFromHandle(hFile)&(FILE_ATTRIBUTE_HIDDEN))!=0)
#define DeeWin32Sys_TryHandleIsReadOnly(hFile) \
 ((DeeWin32Sys_TryGetFileAttributesFromHandle(hFile)&(FILE_ATTRIBUTE_READONLY))!=0)
DEE_STATIC_INLINE(int) DeeWin32Sys_TryHandleIsExecutable(HANDLE hFile) {
 DeeObject *filename; int result;
 if DEE_UNLIKELY((filename = DeeWin32Sys_WideGetHandleFilename(
  hFile)) == NULL) {herr: DeeError_HandledOne(); return 0; }
 result = _DeeFS_WideIsExecutable(DeeWideString_STR(filename));
 Dee_DECREF(filename);
 if (result < 0) goto herr;
 return result;
}
#define DeeWin32Sys_HandleIsExecutable(hFile,result,...)\
do{ if DEE_UNLIKELY((*(result) = DeeWin32Sys_HandleIsExecutable_impl(hFile)) < 0) {__VA_ARGS__;} }while(0)
DEE_STATIC_INLINE(int) DeeWin32Sys_HandleIsExecutable_impl(HANDLE hFile) {
 DeeObject *filename; int result;
 if DEE_UNLIKELY((filename = DeeWin32Sys_WideGetHandleFilename(hFile)) == NULL) return -1;
 result = _DeeFS_WideIsExecutable(DeeWideString_STR(filename));
 Dee_DECREF(filename);
 return result;
}

#ifdef FILE_TYPE_CHAR
#define DeeWin32Sys_TryHandleIsCharDev(hFile)  (GetFileType(hFile) == FILE_TYPE_CHAR)
#else
#define DeeWin32Sys_TryHandleIsCharDev(hFile)  0
#endif
#ifdef FILE_TYPE_DISK
#define DeeWin32Sys_TryHandleIsBlockDev(hFile) (GetFileType(hFile) == FILE_TYPE_DISK)
#else
#define DeeWin32Sys_TryHandleIsBlockDev(hFile) 0
#endif
#ifdef FILE_TYPE_PIPE
#define DeeWin32Sys_TryHandleIsFiFo(hFile)     (GetFileType(hFile) == FILE_TYPE_PIPE)
#else
#define DeeWin32Sys_TryHandleIsFiFo(hFile)     0
#endif
#ifdef FILE_TYPE_REMOTE
#define DeeWin32Sys_TryHandleIsSocket(hFile)   (GetFileType(hFile) == FILE_TYPE_REMOTE)
#else
#define DeeWin32Sys_TryHandleIsSocket(hFile)   0
#endif

#define DeeWin32Sys_HandleIsFile(hFile,result,...) \
do{\
 DWORD _fd_attr = DeeWin32Sys_GetFileAttributesFromHandle(hFile);\
 if (_fd_attr == INVALID_FILE_ATTRIBUTES) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = (_fd_attr&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_REPARSE_POINT))==0;\
}while(0)

#define DeeWin32Sys_HandleIsDir(hFile,result,...) \
do{\
 DWORD _fd_attr = DeeWin32Sys_GetFileAttributesFromHandle(hFile);\
 if (_fd_attr == INVALID_FILE_ATTRIBUTES) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = (_fd_attr&(FILE_ATTRIBUTE_DIRECTORY))!=0;\
}while(0)
#define DeeWin32Sys_HandleIsLink(hFile,result,...) \
do{\
 DWORD _fd_attr = DeeWin32Sys_GetFileAttributesFromHandle(hFile);\
 if (_fd_attr == INVALID_FILE_ATTRIBUTES) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = (_fd_attr&(FILE_ATTRIBUTE_REPARSE_POINT))!=0;\
}while(0)
#define DeeWin32Sys_HandleIsHidden(hFile,result,...) \
do{\
 DWORD _fd_attr = DeeWin32Sys_GetFileAttributesFromHandle(hFile);\
 if (_fd_attr == INVALID_FILE_ATTRIBUTES) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = (_fd_attr&(FILE_ATTRIBUTE_HIDDEN))!=0;\
}while(0)
#define DeeWin32Sys_HandleIsReadOnly(hFile,result,...) \
do{\
 DWORD _fd_attr = DeeWin32Sys_GetFileAttributesFromHandle(hFile);\
 if (_fd_attr == INVALID_FILE_ATTRIBUTES) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = (_fd_attr&(FILE_ATTRIBUTE_READONLY))!=0;\
}while(0)
#ifdef FILE_TYPE_CHAR
#define DeeWin32Sys_HandleIsCharDev(hFile,result,...)\
do{\
 DWORD _fd_type;\
 if DEE_UNLIKELY((_fd_type = GetFileType(hFile)) == 0/*FILE_TYPE_UNKNOWN*/) {\
  if DEE_UNLIKELY((_fd_type = GetLastError()) != 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetFileType(%p) : %K",hFile,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
 *(result) = _fd_type == FILE_TYPE_CHAR;\
}while(0)
#endif

#ifdef FILE_TYPE_DISK
#define DeeWin32Sys_HandleIsBlockDev(hFile,result,...)\
do{\
 DWORD _fd_type;\
 if DEE_UNLIKELY((_fd_type = GetFileType(hFile)) == 0/*FILE_TYPE_UNKNOWN*/) {\
  if DEE_UNLIKELY((_fd_type = GetLastError()) != 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetFileType(%p) : %K",hFile,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
 *(result) = _fd_type == FILE_TYPE_DISK;\
}while(0)
#else
#define DeeWin32Sys_HandleIsBlockDev(hFile,result,...) do{ *(result) = 0; }while(0)
#endif
#ifdef FILE_TYPE_PIPE
#define DeeWin32Sys_HandleIsFiFo(hFile,result,...)\
do{\
 DWORD _fd_type;\
 if DEE_UNLIKELY((_fd_type = GetFileType(hFile)) == 0/*FILE_TYPE_UNKNOWN*/) {\
  if DEE_UNLIKELY((_fd_type = GetLastError()) != 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetFileType(%p) : %K",hFile,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
 *(result) = _fd_type == FILE_TYPE_PIPE;\
}while(0)
#else
#define DeeWin32Sys_HandleIsFiFo(hFile,result,...) do{ *(result) = 0; }while(0)
#endif
#ifdef FILE_TYPE_REMOTE
#define DeeWin32Sys_HandleIsSocket(hFile,result,...)\
do{\
 DWORD _fd_type;\
 if DEE_UNLIKELY((_fd_type = GetFileType(hFile)) == 0/*FILE_TYPE_UNKNOWN*/) {\
  if DEE_UNLIKELY((_fd_type = GetLastError()) != 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetFileType(%p) : %K",hFile,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   {__VA_ARGS__;}\
  }\
 }\
 *(result) = _fd_type == FILE_TYPE_REMOTE;\
}while(0)
#else
#define DeeWin32Sys_HandleIsSocket(hFile,result,...) do{ *(result) = 0; }while(0)
#endif

#define DeeWin32SysFileFD_TryIsFile(self)     DeeWin32Sys_TryHandleIsFile((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsDir(self)      DeeWin32Sys_TryHandleIsDir((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsLink(self)     DeeWin32Sys_TryHandleIsLink((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsHidden(self)   DeeWin32Sys_TryHandleIsHidden((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsCharDev(self)  DeeWin32Sys_TryHandleIsCharDev((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsBlockDev(self) DeeWin32Sys_TryHandleIsBlockDev((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsFiFo(self)     DeeWin32Sys_TryHandleIsFiFo((self)->w32_handle)
#define DeeWin32SysFileFD_TryIsSocket(self)   DeeWin32Sys_TryHandleIsSocket((self)->w32_handle)

#define DeeWin32SysFileFD_IsFile(self,result,...)     DeeWin32Sys_HandleIsFile((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsDir(self,result,...)      DeeWin32Sys_HandleIsDir((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsLink(self,result,...)     DeeWin32Sys_HandleIsLink((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsHidden(self,result,...)   DeeWin32Sys_HandleIsHidden((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsCharDev(self,result,...)  DeeWin32Sys_HandleIsCharDev((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsBlockDev(self,result,...) DeeWin32Sys_HandleIsBlockDev((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsFiFo(self,result,...)     DeeWin32Sys_HandleIsFiFo((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_IsSocket(self,result,...)   DeeWin32Sys_HandleIsSocket((self)->w32_handle,result,__VA_ARGS__)

#define DeeWin32Sys_TryHandleGetMod(hFile,result)\
 (*(result) = (Dee_mode_t)(0444|(DeeWin32Sys_TryHandleIsReadOnly(hFile)?0:0222)\
  |(DeeWin32Sys_TryHandleIsExecutable(hFile)?0111:0)),1)
#define DeeWin32Sys_HandleGetMod(hFile,result,...)\
do{\
 int _fp_temp;\
 DeeWin32Sys_HandleIsReadOnly(hFile,&_fp_temp,__VA_ARGS__);\
 *(result) = (Dee_mode_t)(0444|(_fp_temp ? 0 : 0222));\
 DeeWin32Sys_HandleIsExecutable(hFile,&_fp_temp,__VA_ARGS__);\
 if (_fp_temp) *(result) |= 0111;\
}while(0)


DEE_STATIC_INLINE(BOOL) DeeWin32Sys_TryHandleChmod(HANDLE hFile, Dee_mode_t mode) {
 BY_HANDLE_FILE_INFORMATION info; DeeObject *wfilename; BOOL result;
 // Only able to change the read-only bits
 if DEE_UNLIKELY(!GetFileInformationByHandle(hFile,&info)) return FALSE;
 if ((mode&0222)!=0) { // +w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) return TRUE;
  info.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
 } else { // -w
  if ((info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)!=0) return TRUE;
  info.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
 }
 if ((wfilename = DeeWin32Sys_WideGetHandleFilename(hFile)) == NULL)
 { DeeError_HandledOne(); return FALSE; }
 result = SetFileAttributesW(DeeWideString_STR(wfilename),info.dwFileAttributes);
 Dee_DECREF(wfilename);
 return result;
}
#define DeeWin32Sys_HandleChmod(hFile,mode,...)\
do{\
 BY_HANDLE_FILE_INFORMATION _fp_info; DeeObject *_fp_wfilename;\
 /* Only able to change the read-only bits */\
 if DEE_UNLIKELY(!GetFileInformationByHandle(hFile,&_fp_info)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetFileInformationByHandle(%p) : %K",hFile,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
 if ((((mode)&0222)!=0) != ((_fp_info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0)) {\
  if ((_fp_info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)==0) /* Toggle read-only */\
   _fp_info.dwFileAttributes &= ~(FILE_ATTRIBUTE_READONLY);\
  else _fp_info.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;\
  if DEE_UNLIKELY((_fp_wfilename = DeeWin32Sys_WideGetHandleFilename(hFile)) == NULL) {__VA_ARGS__;}\
  if DEE_UNLIKELY(!SetFileAttributesW(DeeWideString_STR(_fp_wfilename),_fp_info.dwFileAttributes)) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "SetFileAttributesW(%r,%lu) : %K",\
                       _fp_wfilename,_fp_info.dwFileAttributes,\
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
   Dee_DECREF(_fp_wfilename);\
   {__VA_ARGS__;}\
  }\
  Dee_DECREF(_fp_wfilename);\
 }\
}while(0)

#define DeeWin32SysFileFD_TryGetMod(self,result)  DeeWin32Sys_TryHandleGetMod((self)->w32_handle,result)
#define DeeWin32SysFileFD_GetMod(self,result,...) DeeWin32Sys_HandleGetMod((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysFileFD_TryChmod(self,mode)     DeeWin32Sys_TryHandleChmod((self)->w32_handle,mode)
#define DeeWin32SysFileFD_Chmod(self,mode,...)    DeeWin32Sys_HandleChmod((self)->w32_handle,mode,__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
// Reopens a given handle with the specified arguments.
// NOTE: Windows vista and up implement a function ReOpenFile,
//       and if that's available, we use it. But for xp we
//       need to go the long route and acquire the filename
//       to use CreateFile for opening it.
// >> implemented in "_win32.sysfd.c.inl"
extern HANDLE DeeWin32Sys_TryReOpenFile(HANDLE hOriginalFile, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlagsAndAttributes);
extern HANDLE DeeWin32Sys_ReOpenFile(HANDLE hOriginalFile, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlagsAndAttributes);

extern BOOL DeeWin32Sys_AcquirePrivilege(DEE_A_IN Dee_WideChar const *name);
extern BOOL DeeWin32Sys_AcquireSeRestorePrivilege(void);

//////////////////////////////////////////////////////////////////////////
// Changes the owner/group of a given file.
// NOTE: The caller is responsible for ensuring that the file was opened with 'WRITE_OWNER'
#define DeeWin32Sys_DoTryHandleGetOwn(hFile,owner,group)\
 (GetSecurityInfo(hFile,SE_FILE_OBJECT,\
  OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,\
  (PSID *)(owner),(PSID *)(group),NULL,NULL,NULL) == 0)
#define DeeWin32Sys_DoTryHandleChown(hFile,owner,group)\
 ((owner) && IsValidSid((PSID)(owner)) && (group) && IsValidSid((PSID)(group)) && \
  SetSecurityInfo(hFile,SE_FILE_OBJECT,\
  OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,\
  (PSID)(owner),(PSID)(group),NULL,NULL) == 0)

//////////////////////////////////////////////////////////////////////////
// Changes the owner/group of a given file.
// Also tries to acquire 'SeRestorePrivilege' and reopens the file if it wasn't opened with 'WRITE_OWNER'
// >> implemented in "_win32.sysfd.c.inl"
#define DeeWin32Sys_TryHandleChown(hFile,owner,group) \
 DeeWin32Sys_TryHandleChown_impl(hFile,(PSID)(owner),(PSID)(group))
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_TryHandleChown_impl(HANDLE hFile, PSID owner, PSID group) {
 BOOL result; HANDLE hNewFile;
 if (DeeWin32Sys_DoTryHandleChown(hFile,owner,group)) return TRUE;
 /* Retry with fixed permissions */
 hNewFile = DeeWin32Sys_ReOpenFile(hFile,WRITE_OWNER,FILE_SHARE_READ|FILE_SHARE_WRITE|
                                   FILE_SHARE_DELETE,FILE_GENERIC_WRITE);
 if (hNewFile == INVALID_HANDLE_VALUE) {
  if (!DeeWin32Sys_AcquireSeRestorePrivilege()) return FALSE;
  hNewFile = DeeWin32Sys_ReOpenFile(hFile,WRITE_OWNER,FILE_SHARE_READ|FILE_SHARE_WRITE|
                                    FILE_SHARE_DELETE,FILE_GENERIC_WRITE);
  if (hNewFile == INVALID_HANDLE_VALUE) return FALSE;
 }
 result = DeeWin32Sys_DoTryHandleChown(hFile,owner,group);
 if (!CloseHandle(hNewFile)) SetLastError(0);
 return result;
}
#define DeeWin32Sys_TryHandleGetOwn(hFile,owner,group) \
 DeeWin32Sys_TryHandleGetOwn_impl(hFile,(PSID *)(owner),(PSID *)(group))
DEE_STATIC_INLINE(BOOL) DeeWin32Sys_TryHandleGetOwn_impl(HANDLE hFile, PSID *owner, PSID *group) {
 BOOL result; HANDLE hNewFile;
 if (DeeWin32Sys_DoTryHandleGetOwn(hFile,owner,group)) return TRUE;
 /* Retry with fixed permissions */
 hNewFile = DeeWin32Sys_ReOpenFile(hFile,READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE|
                                   FILE_SHARE_DELETE,FILE_GENERIC_READ);
 if (hNewFile == INVALID_HANDLE_VALUE) {
  if (!DeeWin32Sys_AcquireSeRestorePrivilege()) return FALSE;
  hNewFile = DeeWin32Sys_ReOpenFile(hFile,READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE|
                                    FILE_SHARE_DELETE,FILE_GENERIC_READ);
  if (hNewFile == INVALID_HANDLE_VALUE) return FALSE;
 }
 result = DeeWin32Sys_DoTryHandleGetOwn(hFile,owner,group);
 if (!CloseHandle(hNewFile)) SetLastError(0);
 return result;
}


#define DeeWin32Sys_DoHandleGetOwn(hFile,owner,group,...)\
do{\
 DWORD _dhg_error;\
 if DEE_UNLIKELY((_dhg_error = GetSecurityInfo(hFile,SE_FILE_OBJECT,\
  OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,\
  (PSID *)(owner),(PSID *)(group),NULL,NULL,NULL)) != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "GetSecurityInfo(%p,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,...,NULL,NULL,NULL) : %K",\
                       (HANDLE)(hFile),DeeSystemError_Win32ToString(_dhg_error));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeWin32Sys_HandleGetOwn(hFile,owner,group,...)\
do{\
 HANDLE _hg_hNewFile;\
 if (!DeeWin32Sys_DoTryHandleGetOwn(hFile,owner,group)) {\
  /* Retry with fixed permissions */\
  _hg_hNewFile = DeeWin32Sys_TryReOpenFile(hFile,READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE|\
                                           FILE_SHARE_DELETE,FILE_GENERIC_READ);\
  if (_hg_hNewFile == INVALID_HANDLE_VALUE) {\
   if (DeeWin32Sys_AcquireSeRestorePrivilege()) {\
    _hg_hNewFile = DeeWin32Sys_ReOpenFile(hFile,READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE|\
                                          FILE_SHARE_DELETE,FILE_GENERIC_READ);\
    if (_hg_hNewFile == INVALID_HANDLE_VALUE) {__VA_ARGS__;}\
   } else {\
    DeeError_SET_STRING(&DeeErrorType_SystemError,\
                        "Failed to acquire \"SeRestorePrivilege\"");\
    {__VA_ARGS__;}\
   }\
  }\
  DeeWin32Sys_DoHandleGetOwn(hFile,owner,group,{CloseHandle(_hg_hNewFile); {__VA_ARGS__;}});\
  CloseHandle(_hg_hNewFile);\
 }\
}while(0)

#define DeeWin32Sys_DoHandleChown(hFile,owner,group,...)\
do{\
 DWORD _dhc_error;\
 if DEE_UNLIKELY((!(owner) || !IsValidSid((PSID)(owner)) ||\
                  !(group) || !IsValidSid((PSID)(group))\
 ) ? (_dhc_error = ERROR_INVALID_PARAMETER,1) : (\
     (_dhc_error = SetSecurityInfo(hFile,SE_FILE_OBJECT,\
      OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,\
      (PSID)(owner),(PSID)(group),NULL,NULL)) != 0)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "SetSecurityInfo(%p,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,%p,%p,NULL,NULL) : %K",\
                       (HANDLE)(hFile),(PSID)(owner),(PSID)(group),\
                       DeeSystemError_Win32ToString(_dhc_error));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeWin32Sys_HandleChown(hFile,owner,group,...)\
do{\
 HANDLE _hc_hNewFile;\
 if (!DeeWin32Sys_DoTryHandleChown(hFile,owner,group)) {\
  /* Retry with fixed permissions */\
  _hc_hNewFile = DeeWin32Sys_TryReOpenFile(hFile,WRITE_OWNER,FILE_SHARE_READ|FILE_SHARE_WRITE|\
                                           FILE_SHARE_DELETE,FILE_GENERIC_WRITE);\
  if (_hc_hNewFile == INVALID_HANDLE_VALUE) {\
   if (DeeWin32Sys_AcquireSeRestorePrivilege()) {\
    _hc_hNewFile = DeeWin32Sys_ReOpenFile(hFile,WRITE_OWNER,FILE_SHARE_READ|FILE_SHARE_WRITE|\
                                          FILE_SHARE_DELETE,FILE_GENERIC_WRITE);\
    if (_hc_hNewFile == INVALID_HANDLE_VALUE) {__VA_ARGS__;}\
   } else {\
    DeeError_SET_STRING(&DeeErrorType_SystemError,\
                        "Failed to acquire \"SeRestorePrivilege\"");\
    {__VA_ARGS__;}\
   }\
  }\
  DeeWin32Sys_DoHandleChown(hFile,owner,group,{CloseHandle(_hc_hNewFile); {__VA_ARGS__;}});\
  CloseHandle(_hc_hNewFile);\
 }\
}while(0)

#define DeeWin32SysFileFD_TryGetOwn(self,owner,group)  DeeWin32Sys_TryHandleGetOwn((self)->w32_handle,owner,group)
#define DeeWin32SysFileFD_GetOwn(self,owner,group,...) DeeWin32Sys_HandleGetOwn((self)->w32_handle,owner,group,__VA_ARGS__)
#define DeeWin32SysFileFD_TryChown(self,owner,group)   DeeWin32Sys_TryHandleChown((self)->w32_handle,owner,group)
#define DeeWin32SysFileFD_Chown(self,owner,group,...)  DeeWin32Sys_HandleChown((self)->w32_handle,owner,group,__VA_ARGS__)





//////////////////////////////////////////////////////////////////////////
// === DeeWin32SysFileFD ===
struct DeeWin32SysFileFD {
 DEE_WIN32_SYSFD_HEAD
 Dee_openmode_t w32_openmode; /*< The mode the file was opened under. */
};
#define DeeWin32SysFileFD_IS_APPENDMODE(self) DEE_OPENMODE_ISAPPEND((self)->w32_openmode)

#define DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(openmode)\
 (DWORD)(((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_READ   ?   OPEN_EXISTING : \
         ((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_WRITE  ?   CREATE_ALWAYS : \
         ((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_CREATE ?   CREATE_NEW : \
       /*((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_APPEND ? */OPEN_ALWAYS)
#define DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(openmode)\
 (DWORD)(((openmode)&DEE_OPENMODE_FLAG_UPDATE) ? (GENERIC_READ|GENERIC_WRITE) : \
         ((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_READ ? GENERIC_READ : GENERIC_WRITE)
#define DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
 (DWORD)((FILE_FLAG_BACKUP_SEMANTICS|FILE_ATTRIBUTE_NORMAL)|(((perms)&0444)==0 ? FILE_ATTRIBUTE_READONLY : 0))
#define DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(openmode)\
 (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE)


DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8CreateFile(Dee_Utf8Char const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideCreateFile(Dee_WideChar const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8TryCreateFile(Dee_Utf8Char const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideTryCreateFile(Dee_WideChar const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8CreateFileObject(DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideCreateFileObject(DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_Utf8TryCreateFileObject(DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(HANDLE) DeeWin32Sys_WideTryCreateFileObject(DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);

#ifndef __INTELLISENSE__
#define TRY_CREATE
#include "_win32.sysfd.createfile.h"
#include "_win32.sysfd.createfile.h"
#endif

#define DeeWin32SysFileFD_TryInitCopy(self,right)\
 ((self)->w32_openmode = (right)->w32_openmode,DeeWin32SysFD_TryInitCopy(self,right))
#define DeeWin32SysFileFD_InitCopy(self,right,...) \
do{\
 DeeWin32SysFD_InitCopy(self,right,__VA_ARGS__);\
 (self)->w32_openmode = (right)->w32_openmode;\
}while(0)

#define DeeWin32SysFileFD_Utf8TryInitObject(self,filename,mode,perms) \
((self)->w32_openmode = mode,((self)->w32_handle = DeeWin32Sys_Utf8TryCreateFileObject(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
)) != INVALID_HANDLE_VALUE)
#define DeeWin32SysFileFD_WideTryInitObject(self,filename,mode,perms) \
((self)->w32_openmode = mode,((self)->w32_handle = DeeWin32Sys_WideTryCreateFileObject(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
)) != INVALID_HANDLE_VALUE)
#define DeeWin32SysFileFD_Utf8TryInit(self,filename,mode,perms) \
((self)->w32_openmode = mode,((self)->w32_handle = DeeWin32Sys_Utf8TryCreateFile(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
)) != INVALID_HANDLE_VALUE)
#define DeeWin32SysFileFD_WideTryInit(self,filename,mode,perms) \
((self)->w32_openmode = mode,((self)->w32_handle = DeeWin32Sys_WideTryCreateFile(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
)) != INVALID_HANDLE_VALUE)
#define DeeWin32SysFileFD_Utf8InitObject(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(((self)->w32_handle = DeeWin32Sys_Utf8CreateFileObject(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) != INVALID_HANDLE_VALUE) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_WideInitObject(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(((self)->w32_handle = DeeWin32Sys_WideCreateFileObject(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) != INVALID_HANDLE_VALUE) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_Utf8Init(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(((self)->w32_handle = DeeWin32Sys_Utf8CreateFile(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) != INVALID_HANDLE_VALUE) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_WideInit(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(((self)->w32_handle = DeeWin32Sys_WideCreateFile(filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),DEE_WIN32_SYSFILEFD_OPENMODE_GETSHAREMODE(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) != INVALID_HANDLE_VALUE) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)


// Windows doesn't provide its own append filemode, so we need to improvise
#define DeeWin32SysFileFD_TryWrite(self,p,s,ws) \
((DeeWin32SysFileFD_IS_APPENDMODE(self)\
  ? DeeWin32SysFD_TrySeek(self,0,FILE_END,NULL)\
  : TRUE) && DeeWin32SysFD_TryWrite(self,p,s,ws))
#define DeeWin32SysFileFD_Write(self,p,s,ws,...) \
do{\
 if (DeeWin32SysFileFD_IS_APPENDMODE(self)) {\
  DeeWin32SysFD_Seek(self,0,FILE_END,NULL,__VA_ARGS__);\
 }\
 DeeWin32SysFD_Write(self,p,s,ws,__VA_ARGS__);\
}while(0)


#define DeeWin32SysFileFD_TryGetSize(self,result)\
 GetFileSizeEx((self)->w32_handle,(PLARGE_INTEGER)(result))
#define DeeWin32SysFileFD_GetSize(self,result,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysFileFD_TryGetSize(self,result)) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      "GetFileSizeEx(%p) : %K",(self)->w32_handle,\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
 }\
}while(0)






//////////////////////////////////////////////////////////////////////////
// === DeeWin32SysPipeFD ===
struct DeeWin32SysPipeFD { DEE_WIN32_SYSFD_HEAD };
#define DeeWin32SysPipeFD_TryInitEx(reader,writer,size_hint)\
 CreatePipe(&(reader)->w32_handle,&(writer)->w32_handle,NULL,(DWORD)(size_hint))
#define DeeWin32SysPipeFD_InitEx(reader,writer,size_hint,...)\
do{\
 if (!DeeWin32SysPipeFD_TryInitEx(reader,writer,size_hint)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "CreatePipe(...,NULL,%lu) : %K",(DWORD)(size_hint),\
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)










#define DEE_SYSFD_SEEK_SET  DEE_WIN32_SYSFD_SEEK_SET
#define DEE_SYSFD_SEEK_CUR  DEE_WIN32_SYSFD_SEEK_CUR
#define DEE_SYSFD_SEEK_END  DEE_WIN32_SYSFD_SEEK_END

#define DeeSysFD             DeeWin32SysFD
#define DeeSysFD_TryInitCopy DeeWin32SysFD_TryInitCopy
#define DeeSysFD_InitCopy    DeeWin32SysFD_InitCopy
#define DeeSysFD_Quit        DeeWin32SysFD_Quit
#define DeeSysFD_TryRead     DeeWin32SysFD_TryRead
#define DeeSysFD_TryWrite    DeeWin32SysFD_TryWrite
#define DeeSysFD_TrySeek     DeeWin32SysFD_TrySeek
#define DeeSysFD_Read        DeeWin32SysFD_Read
#define DeeSysFD_Write       DeeWin32SysFD_Write
#define DeeSysFD_Seek        DeeWin32SysFD_Seek
#define DeeSysFD_TryFlush    DeeWin32SysFD_TryFlush
#define DeeSysFD_Flush       DeeWin32SysFD_Flush
#define DeeSysFD_TryTrunc    DeeWin32SysFD_TryTrunc
#define DeeSysFD_Trunc       DeeWin32SysFD_Trunc
#define DeeSysFD_GET_STDIN   DeeWin32SysFD_GET_STDIN
#define DeeSysFD_GET_STDOUT  DeeWin32SysFD_GET_STDOUT
#define DeeSysFD_GET_STDERR  DeeWin32SysFD_GET_STDERR

#define DeeSysFileFD                   DeeWin32SysFileFD
#define DeeSysFileFD_TryInitCopy       DeeWin32SysFileFD_TryInitCopy
#define DeeSysFileFD_InitCopy          DeeWin32SysFileFD_InitCopy
#define DeeSysFileFD_Utf8TryInit       DeeWin32SysFileFD_Utf8TryInit
#define DeeSysFileFD_WideTryInit       DeeWin32SysFileFD_WideTryInit
#define DeeSysFileFD_Utf8TryInitObject DeeWin32SysFileFD_Utf8TryInitObject
#define DeeSysFileFD_WideTryInitObject DeeWin32SysFileFD_WideTryInitObject
#define DeeSysFileFD_Utf8Init          DeeWin32SysFileFD_Utf8Init
#define DeeSysFileFD_WideInit          DeeWin32SysFileFD_WideInit
#define DeeSysFileFD_Utf8InitObject    DeeWin32SysFileFD_Utf8InitObject
#define DeeSysFileFD_WideInitObject    DeeWin32SysFileFD_WideInitObject
#define DeeSysFileFD_Filename          DeeWin32SysFileFD_WideFilename
#define DeeSysFileFD_Utf8Filename      DeeWin32SysFileFD_Utf8Filename
#define DeeSysFileFD_WideFilename      DeeWin32SysFileFD_WideFilename
#define DeeSysFileFD_TryWrite          DeeWin32SysFileFD_TryWrite
#define DeeSysFileFD_Write             DeeWin32SysFileFD_Write
#define DeeSysFileFD_TryGetSize        DeeWin32SysFileFD_TryGetSize
#define DeeSysFileFD_GetSize           DeeWin32SysFileFD_GetSize
#define DeeSysFileFD_TryGetTimes       DeeWin32SysFileFD_TryGetTimes
#define DeeSysFileFD_TrySetTimes       DeeWin32SysFileFD_TrySetTimes
#define DeeSysFileFD_GetTimes          DeeWin32SysFileFD_GetTimes
#define DeeSysFileFD_SetTimes          DeeWin32SysFileFD_SetTimes
#define DeeSysFileFD_TryIsFile         DeeWin32SysFileFD_TryIsFile
#define DeeSysFileFD_TryIsDir          DeeWin32SysFileFD_TryIsDir
#define DeeSysFileFD_TryIsLink         DeeWin32SysFileFD_TryIsLink
#define DeeSysFileFD_TryIsHidden       DeeWin32SysFileFD_TryIsHidden
#define DeeSysFileFD_TryIsCharDev      DeeWin32SysFileFD_TryIsCharDev
#define DeeSysFileFD_TryIsBlockDev     DeeWin32SysFileFD_TryIsBlockDev
#define DeeSysFileFD_TryIsFiFo         DeeWin32SysFileFD_TryIsFiFo
#define DeeSysFileFD_TryIsSocket       DeeWin32SysFileFD_TryIsSocket
#define DeeSysFileFD_IsFile            DeeWin32SysFileFD_IsFile
#define DeeSysFileFD_IsDir             DeeWin32SysFileFD_IsDir
#define DeeSysFileFD_IsLink            DeeWin32SysFileFD_IsLink
#define DeeSysFileFD_IsHidden          DeeWin32SysFileFD_IsHidden
#define DeeSysFileFD_IsCharDev         DeeWin32SysFileFD_IsCharDev
#define DeeSysFileFD_IsBlockDev        DeeWin32SysFileFD_IsBlockDev
#define DeeSysFileFD_IsFiFo            DeeWin32SysFileFD_IsFiFo
#define DeeSysFileFD_IsSocket          DeeWin32SysFileFD_IsSocket
#define DeeSysFileFD_TryGetMod         DeeWin32SysFileFD_TryGetMod
#define DeeSysFileFD_GetMod            DeeWin32SysFileFD_GetMod
#define DeeSysFileFD_TryChmod          DeeWin32SysFileFD_TryChmod
#define DeeSysFileFD_Chmod             DeeWin32SysFileFD_Chmod
#define DeeSysFileFD_TryGetOwn         DeeWin32SysFileFD_TryGetOwn
#define DeeSysFileFD_GetOwn            DeeWin32SysFileFD_GetOwn
#define DeeSysFileFD_TryChown          DeeWin32SysFileFD_TryChown
#define DeeSysFileFD_Chown             DeeWin32SysFileFD_Chown

#define DeeSysPipeFD           DeeWin32SysPipeFD
#define DeeSysPipeFD_TryInitEx DeeWin32SysPipeFD_TryInitEx
#define DeeSysPipeFD_InitEx    DeeWin32SysPipeFD_InitEx
#undef DEE_WIN32_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFD_H */
