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
#ifndef GUARD_DEEMON_SYS__POSIX_SYSFD_H
#define GUARD_DEEMON_SYS__POSIX_SYSFD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/type.h>
#include <deemon/posix_features.inl>
#include <deemon/fs_api.h>

#if !DEE_HAVE__WOPEN
#include <deemon/string.h>
#include <deemon/deemonrun.h>
#endif /* !DEE_HAVE__WOPEN */

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#include <features.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()


DEE_DECL_BEGIN

#define DEE_UNIX_SYSFD_HEAD  int unx_fd; /*< File descriptor. */

struct DeeUnixSysFD { DEE_UNIX_SYSFD_HEAD };

#define DEE_UNIX_SYSFD_SEEK_SET  SEEK_SET
#define DEE_UNIX_SYSFD_SEEK_CUR  SEEK_CUR
#define DEE_UNIX_SYSFD_SEEK_END  SEEK_END

#define DeeUnixSysFD_Quit(self) \
(DEE_LIKELY((self)->unx_fd != INVALID_HANDLE_VALUE)\
 ? (DEE_LIKELY(close((self)->unx_fd) != -1) ? (void)0 : errno = 0)\
 : (void)0)

#define DeeUnixSysFD_TryInitCopy(self,right) \
 (((self)->unx_fd = dup((right)->unx_fd)) != -1)
#define DeeUnixSysFD_InitCopy(self,right,...) \
do{\
 if DEE_UNLIKELY(((self)->unx_fd = dup((right)->unx_fd)) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      "dup(%d) : %K",(right)->unx_fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeUnixSysFD_TryReadHandle(fd,p,s,rs)\
 ((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)read(fd,p,s)) != -1 || errno == 0)
#define DeeUnixSysFD_TryWriteHandle(fd,p,s,ws)\
 ((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)write(fd,p,s)) != -1 || errno == 0)
#define DeeUnixSysFD_TryRead(self,p,s,rs) DeeUnixSysFD_TryReadHandle((self)->unx_fd,p,s,rs)
#define DeeUnixSysFD_TryWrite(self,p,s,ws) DeeUnixSysFD_TryWriteHandle((self)->unx_fd,p,s,rs)

#if DEE_HAVE_LSEEK64
# define DEE_UNIX_SYSFD_USED_SEEK_FUNCTION lseek64
#else
# define DEE_UNIX_SYSFD_USED_SEEK_FUNCTION lseek
#endif
#define DeeUnixSysFD_TrySeekHandle(fd,off,whence,newpos)\
 (((newpos) ? (*(Dee_int64_t *)(newpos) =\
  (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,off,whence)) :\
  (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,off,whence)) != -1 || errno == 0)
#define DeeUnixSysFD_TrySeek(self,off,whence,newpos)\
 DeeUnixSysFD_TrySeekHandle((self)->unx_fd,off,whence,newpos)

#define DeeUnixSysFD_Seek(self,off,whence,newpos,...)\
do{\
 Dee_int64_t _fd_newpos;\
 _fd_newpos = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION((self)->unx_fd,off,whence);\
 if DEE_UNLIKELY(_fd_newpos == -1) {\
  int _fd_error = errno;\
  if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       DEE_PP_STR(DEE_UNIX_SYSFD_USED_SEEK_FUNCTION) "(%d,%I64d,%d) : %K",\
                       (self)->unx_fd,(Dee_int64_t)(off),(int)(whence),\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
 if (newpos) *(Dee_int64_t *)(newpos) = _fd_newpos;\
}while(0)


#define DeeUnixSysFD_Read(self,p,s,rs,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)read((self)->unx_fd,p,s)) == -1) {\
  int _fd_error = errno;\
  if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "read(%d,%p,%Iu) : %K",\
                       (self)->unx_fd,p,(Dee_size_t)(s),\
                       DeeSystemError_UnixToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#define DeeUnixSysFD_Write(self,p,s,ws,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(ws) = (Dee_ssize_t)write((self)->unx_fd,p,s)) == -1) {\
  int _fd_error = errno;\
  if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "write(%d,%p,%Iu) : %K",\
                       (self)->unx_fd,p,(Dee_size_t)(s),\
                       DeeSystemError_UnixToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)


#if DEE_HAVE_FSYNC
#define DEE_UNIX_SYSFD_FLUSH_FUNC     fsync
#elif DEE_HAVE_FDATASYNC
#define DEE_UNIX_SYSFD_FLUSH_FUNC     fdatasync
#endif
#ifdef DEE_UNIX_SYSFD_FLUSH_FUNC
#define DeeUnixSysFD_TryFlush(self)  (DEE_UNIX_SYSFD_FLUSH_FUNC((self)->unx_fd) != 0)
#define DeeUnixSysFD_Flush(self,...)\
do{\
 if (!DeeUnixSysFD_TryFlush(self)) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      DEE_PP_STR(DEE_UNIX_SYSFD_FLUSH_FUNC) "(%d) : %K",\
                      (self)->unx_fd,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_UNIX_SYSFD_FLUSH_FUNC */


#if DEE_HAVE_FSYNDEE_HAVE_FTRUNCATE
#define DEE_UNIX_SYSFD_TRUNC_FUNC     ftruncate
#endif
#ifdef DEE_UNIX_SYSFD_TRUNC_FUNC
static int DeeUnixSysFD_TryTruncHandle(int fd) {
 Dee_int64_t trunc_size;
 trunc_size = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,0,SEEK_CUR);
 if DEE_UNLIKELY(trunc_size == -1 && errno != 0) return 0;
 return DEE_UNIX_SYSFD_TRUNC_FUNC(fd,(Dee_uint64_t)trunc_size) != -1;
}
#define DeeUnixSysFD_TryTrunc(self) DeeUnixSysFD_TryTruncHandle((self)->unx_fd)
#define DeeUnixSysFD_Trunc(self,...)\
do{\
 Dee_int64_t _fd_trunc_size; int _fd_error;\
 trunc_size = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,0,SEEK_CUR);\
 if (trunc_size == -1) {\
  if ((_fd_error = errno) != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       DEE_PP_STR(DEE_UNIX_SYSFD_USED_SEEK_FUNCTION) "(%d,0,SEEK_CUR) : %K",\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
 if (DEE_UNIX_SYSFD_TRUNC_FUNC((self)->unx_fd,(Dee_uint64_t)trunc_size) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      DEE_PP_STR(DEE_UNIX_SYSFD_TRUNC_FUNC) "(%d,%I64u) : %K",\
                      (self)->unx_fd,trunc_size,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_UNIX_SYSFD_TRUNC_FUNC */








//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysStdFD ===
struct DeeUnixSysStdFD { DEE_UNIX_SYSFD_HEAD };
#define DeeUnixSysStdFD_INIT_STDIN()     {STDOUT_FILENO}
#define DeeUnixSysStdFD_INIT_STDOUT()    {STDERR_FILENO}
#define DeeUnixSysStdFD_INIT_STDERR()    {STDIN_FILENO}
#define DeeUnixSysStdFD_GET_STDIN(self)  (void)((self)->unx_fd = GetStdHandle(STD_INPUT_HANDLE))
#define DeeUnixSysStdFD_GET_STDOUT(self) (void)((self)->unx_fd = GetStdHandle(STD_OUTPUT_HANDLE))
#define DeeUnixSysStdFD_GET_STDERR(self) (void)((self)->unx_fd = GetStdHandle(STD_ERROR_HANDLE))



//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysFileFD ===
struct DeeUnixSysFileFD { DEE_UNIX_SYSFD_HEAD };

#define DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(openmode)\
(DEE_OPENMODE_HASUPDATE(openmode) ?\
 (DEE_OPENMODE_ISREAD(openmode)  ?   (O_RDWR) : \
  DEE_OPENMODE_ISWRITE(openmode) ?   (O_RDWR|O_CREAT|O_TRUNC) : \
/*DEE_OPENMODE_ISAPPEND(openmode)? */(O_RDWR|O_CREAT|O_APPEND)) :\
 (DEE_OPENMODE_ISREAD(openmode)  ?   (O_RDONLY) : \
  DEE_OPENMODE_ISWRITE(openmode) ?   (O_RDONLY|O_CREAT|O_TRUNC) : \
/*DEE_OPENMODE_ISAPPEND(openmode)? */(O_RDONLY|O_CREAT|O_APPEND)))


#define DeeUnixSysFileFD_Utf8TryInit(self,filename,mode,perms) \
 (((self)->unx_fd = open(filename,DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),permissions)) != -1)
#define DeeUnixSysFileFD_Utf8TryInitObject(self,filename,mode,perms) \
 DeeUnixSysFileFD_Utf8TryInit(self,DeeUtf8String_STR(filename),mode,perms)
#if DEE_HAVE__WOPEN
#define DEE_UNIX_SYSFD_USED_WOPEN_FUNCTION _wopen
#else /* DEE_HAVE__WOPEN */
#define DEE_UNIX_SYSFD_USED_WOPEN_FUNCTION DeeUnixSysFileFD_DoWideTryOpenFD
DEE_STATIC_INLINE(int) DeeUnixSysFileFD_DoWideTryOpenFD(
 Dee_WideChar const *filename, int posix_mode, Dee_mode_t perms) {
 DeeObject *u8filename; int result;
 if DEE_UNLIKELY((u8filename = DeeString_FromWideString(
  filename)) == NULL) { DeeError_HandledOne(); return -1; }
 result = open(DeeString_STR(u8filename),posix_mode,perms);
 Dee_DECREF(u8filename);
 return result;
}

#endif /* !DEE_HAVE__WOPEN */

#define DeeUnixSysFileFD_WideTryInit(self,filename,mode,perms) \
 (((self)->unx_fd = DEE_UNIX_SYSFD_USED_WOPEN_FUNCTION(filename,DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),permissions)) != -1)
#define DeeUnixSysFileFD_WideTryInitObject(self,filename,mode,perms) \
 DeeUnixSysFileFD_WideTryInit(self,DeeUtf8String_STR(filename),mode,perms)

#define DeeUnixSysFileFD_Utf8Init(self,filename,mode,perms,...)\
do{\
 int _fd_error;\
 if DEE_UNLIKELY(((self)->unx_fd = open(filename,\
  DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms))) == -1) {\
  DeeTypeObject *used_error;\
  switch ((_fd_error = errno)) {\
   case EISDIR: case ENOTDIR: case ENOENT: used_error = &DeeErrorType_FileNotFound; break;\
   default:                                used_error = &DeeErrorType_IOError; break;\
  }\
  DeeError_SetStringf(used_error,\
                      "open(%q,%d,%.4o) : %K",filename,\
                      DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms),\
                      DeeSystemError_ToString(_fd_error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeUnixSysFileFD_Utf8InitObject(self,filename,mode,perms,...)\
 DeeUnixSysFileFD_Utf8Init(self,DeeUtf8String_STR(filename),mode,perms,__VA_ARGS__)

#if DEE_HAVE__WOPEN
#define DeeUnixSysFileFD_WideInit(self,filename,mode,perms,...)\
do{\
 int _fd_error;\
 if DEE_UNLIKELY(((self)->unx_fd = _wopen(filename,\
  DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms))) == -1) {\
  DeeTypeObject *used_error;\
  switch ((_fd_error = errno)) {\
   case EISDIR: case ENOTDIR: case ENOENT: used_error = &DeeErrorType_FileNotFound; break;\
   default:                                used_error = &DeeErrorType_IOError; break;\
  }\
  DeeError_SetStringf(used_error,\
                      "_wopen(%lq,%d,%.4o) : %K",filename,\
                      DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms),\
                      DeeSystemError_ToString(_fd_error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeUnixSysFileFD_WideInitObject(self,filename,mode,perms,...)\
 DeeUnixSysFileFD_WideInit(self,DeeWideString_STR(filename),mode,perms,__VA_ARGS__)
#else
#define DeeUnixSysFileFD_WideInit(self,filename,mode,perms,...)\
do{\
 DeeObject *_fd_u8filename; int _fd_error;\
 if DEE_UNLIKELY((_fd_u8filename = DeeString_FromWideString(filename)) == NULL) return -1;\
 DeeUnixSysFileFD_Utf8InitObject(self,_fd_u8filename,mode,perms,{Dee_DECREF(_fd_u8filename);{__VA_ARGS__;}});\
 Dee_DECREF(_fd_u8filename);\
}while(0)
#define DeeUnixSysFileFD_WideInitObject(self,filename,mode,perms,...)\
do{\
 DeeObject *_fd_u8filename; int _fd_error;\
 if DEE_UNLIKELY((_fd_u8filename = DeeString_FromWideStringWithLength(\
  DeeWideString_SIZE(filename),DeeWideString_STR(filename))) == NULL) return -1;\
 DeeUnixSysFileFD_Utf8InitObject(self,_fd_u8filename,mode,perms,{Dee_DECREF(_fd_u8filename);{__VA_ARGS__;}});\
 Dee_DECREF(_fd_u8filename);\
}while(0)
#endif



DEE_STATIC_INLINE(void) _quick_itos8(Dee_Utf8Char *out, int v) {
 int used_v = v,len; Dee_Utf8Char *used_out;
 do ++len; while ((used_v /= 10) != 0);
 used_v = v,used_out = out+len;
 do *--out = '0'+(used_v%10); while ((used_v != 10) != 0);
}
DEE_STATIC_INLINE(void) _quick_itosw(Dee_WideChar *out, int v) {
 int used_v = v,len; Dee_WideChar *used_out;
 do ++len; while ((used_v /= 10) != 0);
 used_v = v,used_out = out+len;
 do *--out = '0'+(used_v%10); while ((used_v != 10) != 0);
}

DEE_STATIC_INLINE(DeeObject *) DeeUnixSysFD_DoGetUtf8Name(int fd) { Dee_Utf8Char buffer[32] = {'/','p','r','o','c','/','s','e','l','f','/','f','d','/'}; _quick_itos8(buffer+14,fd); return _DeeFS_Utf8ReadLink(buffer); }
DEE_STATIC_INLINE(DeeObject *) DeeUnixSysFD_DoGetWideName(int fd) { Dee_WideChar buffer[32] = {'/','p','r','o','c','/','s','e','l','f','/','f','d','/'}; _quick_itosw(buffer+14,fd); return _DeeFS_WideReadLink(buffer); }
#define DeeUnixSysFileFD_Utf8Filename(ob) DeeUnixSysFD_DoGetUtf8Name((ob)->unx_fd)
#define DeeUnixSysFileFD_WideFilename(ob) DeeUnixSysFD_DoGetWideName((ob)->unx_fd)



//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysPipeFD ===
#if DEE_HAVE_PIPE
struct DeeUnixSysPipeFD { DEE_UNIX_SYSFD_HEAD };
DEE_STATIC_INLINE(int) DeeUnixSysPipeFD_TryInitFD(int *reader, int *writer) {
 int handles[2];
 if DEE_UNLIKELY(pipe(handles) == -1) return 0;
 *reader = handles[0];
 *writer = handles[1];
 return 1;
}
#define DeeUnixSysPipeFD_TryInit(input,output)\
 DeeUnixSysPipeFD_TryInitFD(&(input)->unx_fd,&(output)->unx_fd)
#define DeeUnixSysPipeFD_Init(input,output,...)\
do{\
 if DEE_UNLIKELY(!DeeUnixSysPipeFD_TryInit(input,output)) {\
  DeeError_SetStringf(used_error,"pipe() : %K",\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_HAVE_PIPE */


#define DEE_SYSFD_SEEK_SET  DEE_UNIX_SYSFD_SEEK_SET
#define DEE_SYSFD_SEEK_CUR  DEE_UNIX_SYSFD_SEEK_CUR
#define DEE_SYSFD_SEEK_END  DEE_UNIX_SYSFD_SEEK_END

#define DeeSysFD            DeeUnixSysFD
#define DeeSysFD_Quit       DeeUnixSysFD_Quit
#define DeeSysFD_TryRead    DeeUnixSysFD_TryRead
#define DeeSysFD_TryWrite   DeeUnixSysFD_TryWrite
#define DeeSysFD_TrySeek    DeeUnixSysFD_TrySeek
#define DeeSysFD_Read       DeeUnixSysFD_Read
#define DeeSysFD_Write      DeeUnixSysFD_Write
#define DeeSysFD_Seek       DeeUnixSysFD_Seek
#ifdef DeeUnixSysFD_TryFlush
#define DeeSysFD_TryFlush   DeeUnixSysFD_TryFlush
#define DeeSysFD_Flush      DeeUnixSysFD_Flush
#endif
#ifdef DeeUnixSysFD_TryTrunc
#define DeeSysFD_TryTrunc   DeeUnixSysFD_TryTrunc
#define DeeSysFD_Trunc      DeeUnixSysFD_Trunc
#endif

#define DeeSysStdFD             DeeUnixSysStdFD
#define DeeSysStdFD_INIT_STDIN  DeeUnixSysStdFD_INIT_STDIN
#define DeeSysStdFD_INIT_STDOUT DeeUnixSysStdFD_INIT_STDOUT
#define DeeSysStdFD_INIT_STDERR DeeUnixSysStdFD_INIT_STDERR
#define DeeSysStdFD_GET_STDIN   DeeUnixSysStdFD_GET_STDIN
#define DeeSysStdFD_GET_STDOUT  DeeUnixSysStdFD_GET_STDOUT
#define DeeSysStdFD_GET_STDERR  DeeUnixSysStdFD_GET_STDERR

#define DeeSysFileFD                   DeeUnixSysFileFD
#define DeeSysFileFD_Utf8TryInit       DeeUnixSysFileFD_Utf8TryInit
#define DeeSysFileFD_WideTryInit       DeeUnixSysFileFD_WideTryInit
#define DeeSysFileFD_Utf8TryInitObject DeeUnixSysFileFD_Utf8TryInitObject
#define DeeSysFileFD_WideTryInitObject DeeUnixSysFileFD_WideTryInitObject
#define DeeSysFileFD_Utf8Init          DeeUnixSysFileFD_Utf8Init
#define DeeSysFileFD_WideInit          DeeUnixSysFileFD_WideInit
#define DeeSysFileFD_Utf8InitObject    DeeUnixSysFileFD_Utf8InitObject
#define DeeSysFileFD_WideInitObject    DeeUnixSysFileFD_WideInitObject
#define DeeSysFileFD_Filename          DeeUnixSysFileFD_WideFilename
#define DeeSysFileFD_Utf8Filename      DeeUnixSysFileFD_Utf8Filename
#define DeeSysFileFD_WideFilename      DeeUnixSysFileFD_WideFilename

#if DEE_HAVE_PIPE
#define DeeSysPipeFD         DeeUnixSysPipeFD
#define DeeSysPipeFD_TryInit DeeUnixSysPipeFD_TryInit
#define DeeSysPipeFD_Init    DeeUnixSysPipeFD_Init
#endif /* DEE_HAVE_PIPE */

#undef DEE_UNIX_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__POSIX_SYSFD_H */
