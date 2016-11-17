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
#include <deemon/file.h>
#include <deemon/type.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include DEE_INCLUDE_MEMORY_API()


#ifndef SIZEOF_DWORD
#define SIZEOF_DWORD 4
#endif
#ifndef DWORD_MAX
#define DWORD_MAX 0xFFFFFFFF
#endif

DEE_DECL_BEGIN


#define DEE_WIN32_SYSFD_HEAD  HANDLE w32_handle; /*< [1..1] File handle. */
struct DeeWin32SysFD { DEE_WIN32_SYSFD_HEAD };

#define DEE_WIN32_SYSFD_SEEK_SET  FILE_BEGIN
#define DEE_WIN32_SYSFD_SEEK_CUR  FILE_CURRENT
#define DEE_WIN32_SYSFD_SEEK_END  FILE_END

#define DeeWin32SysFD_Quit(self) \
 (DEE_LIKELY(CloseHandle((self)->w32_handle)) ? (void)0 : SetLastError(0))

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
                       "WriteFile(%p,%p,%Iu) : %K",\
                       (self)->w32_handle,p,(Dee_size_t)(s),\
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

// >> [[optional
// >>   struct DeeSysFDTimes { ... };
// >>   [[optional]] #define DeeSysFDTimes_INIT(atime,ctime,mtime) { ... }
// >>                void DeeSysFDTimes_Init(DEE_A_OUT struct DeeSysFDTimes *self, Dee_timetick_t atime, Dee_timetick_t ctime, Dee_timetick_t mtime);
// >>                Dee_timetick_t DeeSysFDTimes_GetATime(DEE_A_IN struct DeeSysFDTimes const *self);
// >>                Dee_timetick_t DeeSysFDTimes_GetCTime(DEE_A_IN struct DeeSysFDTimes const *self);
// >>                Dee_timetick_t DeeSysFDTimes_GetMTime(DEE_A_IN struct DeeSysFDTimes const *self);
// >>                void DeeSysFDTimes_SetATime(DEE_A_INOUT struct DeeSysFDTimes *self, Dee_timetick_t );
// >>                void DeeSysFDTimes_SetCTime(DEE_A_INOUT struct DeeSysFDTimes *self, Dee_timetick_t );
// >>                void DeeSysFDTimes_SetMTime(DEE_A_INOUT struct DeeSysFDTimes *self, Dee_timetick_t );
// >>   [[optional]] bool DeeSysFD_TryGetTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT struct DeeSysFDTimes *times);
// >>   [[optional]] bool DeeSysFD_TryGetATime(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT Dee_timetick_t *atime);
// >>   [[optional]] bool DeeSysFD_TryGetCTime(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT Dee_timetick_t *ctime);
// >>   [[optional]] bool DeeSysFD_TryGetMTime(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT Dee_timetick_t *mtime);
// >>   [[optional]] void DeeSysFD_GetTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT struct DeeSysFDTimes *times, CODE on_error);
// >>   [[optional]] void DeeSysFD_SetTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN struct DeeSysFDTimes const *times, CODE on_error);
// >>   [[optional]] void DeeSysFD_SetACMTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN_OPT Dee_timetick_t *atime, DEE_A_IN_OPT Dee_timetick_t *ctime, DEE_A_IN_OPT Dee_timetick_t *mtime, CODE on_error);
// >> ]]




#define DeeWin32SysFD_GET_STDIN(self)  (void)((self)->w32_handle = GetStdHandle(STD_INPUT_HANDLE))
#define DeeWin32SysFD_GET_STDOUT(self) (void)((self)->w32_handle = GetStdHandle(STD_OUTPUT_HANDLE))
#define DeeWin32SysFD_GET_STDERR(self) (void)((self)->w32_handle = GetStdHandle(STD_ERROR_HANDLE))



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
       /*((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_APPEND ? */OPEN_ALWAYS)
#define DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(openmode)\
 (DWORD)(((openmode)&DEE_OPENMODE_FLAG_UPDATE) ? (GENERIC_READ|GENERIC_WRITE) : \
         ((openmode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_READ ? GENERIC_READ : GENERIC_WRITE)
#define DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)\
 (DWORD)((FILE_FLAG_BACKUP_SEMANTICS|FILE_ATTRIBUTE_NORMAL)|(((perms)&0444)==0 ? FILE_ATTRIBUTE_READONLY : 0))


DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8Init(struct DeeWin32SysFileFD *self, Dee_Utf8Char const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideInit(struct DeeWin32SysFileFD *self, Dee_WideChar const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8TryInit(struct DeeWin32SysFileFD *self, Dee_Utf8Char const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideTryInit(struct DeeWin32SysFileFD *self, Dee_WideChar const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8InitObject(struct DeeWin32SysFileFD *self, DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideInitObject(struct DeeWin32SysFileFD *self, DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoUtf8TryInitObject(struct DeeWin32SysFileFD *self, DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);
DEE_STATIC_INLINE(BOOL) DeeWin32SysFileFD_DoWideTryInitObject(struct DeeWin32SysFileFD *self, DeeObject const *filename, DWORD dwDesiredAccess, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);

#ifndef __INTELLISENSE__
#define TRY_CREATE
#include "_win32.sysfd.createfile.h"
#include "_win32.sysfd.createfile.h"
#endif

#define DeeWin32SysFileFD_Utf8TryInitObject(self,filename,mode,perms) \
((self)->w32_openmode = mode,DeeWin32SysFileFD_DoUtf8TryInitObject(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)))
#define DeeWin32SysFileFD_WideTryInitObject(self,filename,mode,perms) \
((self)->w32_openmode = mode,DeeWin32SysFileFD_DoWideTryInitObject(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)))
#define DeeWin32SysFileFD_Utf8TryInit(self,filename,mode,perms) \
((self)->w32_openmode = mode,DeeWin32SysFileFD_DoUtf8TryInit(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)))
#define DeeWin32SysFileFD_WideTryInit(self,filename,mode,perms) \
((self)->w32_openmode = mode,DeeWin32SysFileFD_DoWideTryInit(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms)))
#define DeeWin32SysFileFD_Utf8InitObject(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(DeeWin32SysFileFD_DoUtf8InitObject(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),\
  DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_WideInitObject(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(DeeWin32SysFileFD_DoWideInitObject(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),\
  DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_Utf8Init(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(DeeWin32SysFileFD_DoUtf8TryInit(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),\
  DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)
#define DeeWin32SysFileFD_WideInit(self,filename,mode,perms,...) \
do{\
 if DEE_LIKELY(DeeWin32SysFileFD_DoWideTryInit(self,filename,\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETACCESS(mode),\
  DEE_WIN32_SYSFILEFD_OPENMODE_GETDISPOSITION(mode),\
  DEE_WIN32_SYSFILEFD_PERMISSIONS_GETATTR(perms))\
 ) (self)->w32_openmode = mode; else {__VA_ARGS__;} \
}while(0)

extern DeeObject *DeeWin32SysFileFD_DoGetUtf8Filename(HANDLE hFile);
extern DeeObject *DeeWin32SysFileFD_DoGetWideFilename(HANDLE hFile);
#define DeeWin32SysFileFD_Utf8Filename(ob) DeeWin32SysFileFD_DoGetUtf8Filename((ob)->w32_handle)
#define DeeWin32SysFileFD_WideFilename(ob) DeeWin32SysFileFD_DoGetWideFilename((ob)->w32_handle)


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

#define DeeSysPipeFD           DeeWin32SysPipeFD
#define DeeSysPipeFD_TryInitEx DeeWin32SysPipeFD_TryInitEx
#define DeeSysPipeFD_InitEx    DeeWin32SysPipeFD_InitEx
#undef DEE_WIN32_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFD_H */
