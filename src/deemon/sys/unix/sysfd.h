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
#ifndef GUARD_DEEMON_SYS_POSIX_SYSFD_H
#define GUARD_DEEMON_SYS_POSIX_SYSFD_H 1

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
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H
#include <sys/stat.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif /* ... */
#include <deemon/time.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#define DEE_UNIX_SYSFD_HEAD \
 int unx_fd; /*< File descriptor. */

#define DeeUnixSysFD DeeUnixSysFD
struct DeeUnixSysFD { DEE_UNIX_SYSFD_HEAD };

#ifndef DEE_UNIX_SYSFD_SEEK_SET
#ifdef SEEK_SET
# define DEE_UNIX_SYSFD_SEEK_SET  SEEK_SET
#else
# define DEE_UNIX_SYSFD_SEEK_SET  0
#endif
#endif /* !DEE_UNIX_SYSFD_SEEK_SET */
#ifndef DEE_UNIX_SYSFD_SEEK_CUR
#ifdef SEEK_CUR
# define DEE_UNIX_SYSFD_SEEK_CUR  SEEK_CUR
#else
# define DEE_UNIX_SYSFD_SEEK_CUR  1
#endif
#endif /* !DEE_UNIX_SYSFD_SEEK_CUR */
#ifndef DEE_UNIX_SYSFD_SEEK_END
#ifdef SEEK_END
# define DEE_UNIX_SYSFD_SEEK_END  SEEK_END
#else
# define DEE_UNIX_SYSFD_SEEK_END  2
#endif
#endif /* !DEE_UNIX_SYSFD_SEEK_END */

#if DEE_HAVE_CLOSE
#define DeeUnixSys_FDQuit(fd) (void)close(fd)
#endif /* DEE_HAVE_CLOSE */

#ifdef DeeUnixSys_FDQuit
#define DeeUnixSysFD_Quit(self) DeeUnixSys_FDQuit((self)->unx_fd)
#endif

#if DEE_HAVE_DUP
#define DeeUnixSys_FDInitCopy(fd,pfd2,...) \
do{\
 if DEE_UNLIKELY((*(pfd2) = dup(fd)) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,"dup(%d) : %K",fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_HAVE_DUP */

#ifdef DeeUnixSys_FDInitCopy
#define DeeUnixSysFD_InitCopy(self,right,...) \
 DeeUnixSys_FDInitCopy((self)->unx_fd,&(right)->unx_fd,__VA_ARGS__)
#endif /* DeeUnixSys_FDInitCopy */

#ifndef DEE_UNIX_SYSFD_USED_SEEK_FUNCTION
#if DEE_HAVE_LSEEK64
# define DEE_UNIX_SYSFD_USED_SEEK_FUNCTION lseek64
#elif DEE_HAVE_LSEEK
# define DEE_UNIX_SYSFD_USED_SEEK_FUNCTION lseek
#endif /* ... */
#endif /* !DEE_UNIX_SYSFD_USED_SEEK_FUNCTION */

#ifdef DEE_UNIX_SYSFD_USED_SEEK_FUNCTION
#define DeeUnixSys_FDSeek(fd,off,whence,newpos,...)\
do{\
 Dee_int64_t _fd_newpos = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,off,whence);\
 if DEE_UNLIKELY(_fd_newpos == -1) {\
  int _fd_error = errno;\
  if (_fd_error != 0) {\
   if (_fd_error == ESPIPE || /*< can't seek | v can't seek to end of file */\
      (_fd_error == EINVAL && (off) == 0 && (whence) == SEEK_END)) {\
    DeeError_NotImplemented_str("seek");\
   } else {\
    DeeError_SetStringf(&DeeErrorType_IOError,\
                        DEE_PP_STR(DEE_UNIX_SYSFD_USED_SEEK_FUNCTION) "(%d,%I64d,%d) : %K",\
                        fd,(Dee_int64_t)(off),(int)(whence),\
                        DeeSystemError_ToString(_fd_error));\
   }\
   {__VA_ARGS__;}\
  }\
 }\
 if (newpos) *(Dee_int64_t *)(newpos) = _fd_newpos;\
}while(0)
#endif /* DEE_UNIX_SYSFD_USED_SEEK_FUNCTION */

#if DEE_HAVE_READ
#define DeeUnixSys_FDRead(fd,p,s,rs,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)read(fd,p,s)) == -1) {\
  int _fd_error = errno;\
  /* v Must be done to correctly handle reading from memory maps */\
  if (_fd_error == EIO) *(rs) = 0;\
  else if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "read(%d,%p,%Iu) : %K",\
                       fd,p,(Dee_size_t)(s),\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#endif /* DEE_HAVE_READ */

#if DEE_HAVE_WRITE
#define DeeUnixSys_FDWrite(fd,p,s,ws,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(ws) = (Dee_ssize_t)write(fd,p,s)) == -1) {\
  int _fd_error = errno;\
  /* v Must be done to correctly handle writing to memory maps */\
  if (_fd_error == EIO) *(ws) = 0;\
  else if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "write(%d,%p,%Iu:%.*q) : %K",\
                       fd,p,(Dee_size_t)(s),(unsigned)(s),p,\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#endif /* DEE_HAVE_WRITE */

#if DEE_HAVE_PREAD
#define DeeUnixSys_FDReadAt(fd,pos,p,s,rs,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)pread(fd,p,s,(off_t)(pos))) == -1) {\
  int _fd_error = errno;\
  /* v Must be done to correctly handle writing to memory maps */\
  if (_fd_error == EIO) *(ws) = 0;\
  else if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "pread(%d,%p,%Iu) : %K",\
                       fd,p,(Dee_size_t)(s),\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#endif /* DEE_HAVE_PREAD */

#if DEE_HAVE_PWRITE
#define DeeUnixSys_FDWriteAt(fd,pos,p,s,rs,...)\
do{\
 if DEE_UNLIKELY((*(Dee_ssize_t *)(rs) = (Dee_ssize_t)pwrite(fd,p,s,(off_t)(pos))) == -1) {\
  int _fd_error = errno;\
  /* v Must be done to correctly handle writing to memory maps */\
  if (_fd_error == EIO) *(ws) = 0;\
  else if (_fd_error != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       "pwrite(%d,%p,%Iu) : %K",\
                       fd,p,(Dee_size_t)(s),\
                       DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)
#endif /* DEE_HAVE_PWRITE */


#ifndef DEE_UNIX_SYSFD_FLUSH_FUNC
#if DEE_HAVE_FSYNC
# define DEE_UNIX_SYSFD_FLUSH_FUNC     fsync
#elif DEE_HAVE_FDATASYNC
# define DEE_UNIX_SYSFD_FLUSH_FUNC     fdatasync
#endif /* ... */
#endif /* !DEE_UNIX_SYSFD_FLUSH_FUNC */
#ifdef DEE_UNIX_SYSFD_FLUSH_FUNC
#define DeeUnixSys_FDFlush(fd,...)\
do{\
 if (DEE_UNIX_SYSFD_FLUSH_FUNC(fd) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      DEE_PP_STR(DEE_UNIX_SYSFD_FLUSH_FUNC) "(%d) : %K",\
                      fd,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_UNIX_SYSFD_FLUSH_FUNC */


#ifndef DEE_UNIX_SYSFD_TRUNC_FUNC
#if DEE_HAVE_FTRUNCATE
# define DEE_UNIX_SYSFD_TRUNC_FUNC     ftruncate
#endif /* DEE_HAVE_FTRUNCATE */
#endif /* !DEE_UNIX_SYSFD_TRUNC_FUNC */
#ifdef DEE_UNIX_SYSFD_TRUNC_FUNC
#define DeeUnixSys_FDTrunc(fd,...)\
do{\
 Dee_uint64_t _fd_trunc_size; int _fd_error;\
 DeeUnixSys_FDSeek(fd,0,SEEK_CUR,&_fd_trunc_size,__VA_ARGS__);\
 if DEE_UNLIKELY(DEE_UNIX_SYSFD_TRUNC_FUNC(fd,_fd_trunc_size) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      DEE_PP_STR(DEE_UNIX_SYSFD_TRUNC_FUNC) "(%d,%I64u) : %K",\
                      fd,_fd_trunc_size,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_UNIX_SYSFD_TRUNC_FUNC */


#ifdef DeeUnixSys_FDRead
#define DeeUnixSysFD_Read(self,p,s,rs,...) DeeUnixSys_FDRead((self)->unx_fd,p,s,rs,__VA_ARGS__)
#endif /* DeeUnixSys_FDRead */
#ifdef DeeUnixSys_FDWrite
#define DeeUnixSysFD_Write(self,p,s,ws,...) DeeUnixSys_FDWrite((self)->unx_fd,p,s,ws,__VA_ARGS__)
#endif /* DeeUnixSys_FDWrite */
#ifdef DeeUnixSys_FDSeek
#define DeeUnixSysFD_Seek(self,off,whence,newpos,...) DeeUnixSys_FDSeek((self)->unx_fd,off,whence,newpos,__VA_ARGS__)
#endif /* DeeUnixSys_FDSeek */
#ifdef DeeUnixSys_FDFlush
#define DeeUnixSysFD_Flush(self,...) DeeUnixSys_FDFlush((self)->unx_fd,__VA_ARGS__)
#endif /* DeeUnixSys_FDFlush */
#ifdef DeeUnixSys_FDTrunc
#define DeeUnixSysFD_Trunc(self,...) DeeUnixSys_FDTrunc((self)->unx_fd,__VA_ARGS__)
#endif /* DeeUnixSys_FDTrunc */



#ifdef STDIN_FILENO
#define DeeUnixSysFD_INIT_STDIN()     {STDIN_FILENO}
#define DeeUnixSysFD_GET_STDIN(self)  (void)((self)->unx_fd = STDIN_FILENO)
#endif
#ifdef STDOUT_FILENO
#define DeeUnixSysFD_INIT_STDOUT()    {STDOUT_FILENO}
#define DeeUnixSysFD_GET_STDOUT(self) (void)((self)->unx_fd = STDOUT_FILENO)
#endif
#ifdef STDERR_FILENO
#define DeeUnixSysFD_INIT_STDERR()    {STDERR_FILENO}
#define DeeUnixSysFD_GET_STDERR(self) (void)((self)->unx_fd = STDERR_FILENO)
#endif



//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysFileFD ===
#define DeeUnixSysFileFD DeeUnixSysFileFD
struct DeeUnixSysFileFD { DEE_UNIX_SYSFD_HEAD };

#ifdef O_LARGEFILE
#define DEE_UNIX_SYSFD_USED_O_LARGEFILE O_LARGEFILE
#else
#define DEE_UNIX_SYSFD_USED_O_LARGEFILE 0
#endif

#define DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(openmode)\
(DEE_OPENMODE_HASUPDATE(openmode) ?\
 (DEE_OPENMODE_ISREAD(openmode)   ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR) : \
  DEE_OPENMODE_ISWRITE(openmode)  ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR|O_CREAT|O_TRUNC) : \
  DEE_OPENMODE_ISCREATE(openmode) ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR|O_CREAT|O_EXCL) : \
/*DEE_OPENMODE_ISAPPEND(openmode) ?*/(DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR|O_CREAT|O_APPEND)) :\
 (DEE_OPENMODE_ISREAD(openmode)   ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDONLY) : \
  DEE_OPENMODE_ISWRITE(openmode)  ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_WRONLY|O_CREAT|O_TRUNC) : \
  DEE_OPENMODE_ISCREATE(openmode) ?  (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_WRONLY|O_CREAT|O_EXCL) : \
/*DEE_OPENMODE_ISAPPEND(openmode) ?*/(DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_WRONLY|O_CREAT|O_APPEND)))

#define DeeUnixSysFileFD_Utf8Init(self,filename,mode,perms,...)\
do{\
 int _fd_error;\
 if DEE_UNLIKELY(((self)->unx_fd = open(filename,\
  DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms))) == -1) {\
  DeeTypeObject *_fd_used_error;\
  switch ((_fd_error = errno)) {\
   case EISDIR: case ENOTDIR: case ENOENT: _fd_used_error = &DeeErrorType_FileNotFound; break;\
   default:                                _fd_used_error = &DeeErrorType_IOError; break;\
  }\
  DeeError_SetStringf(_fd_used_error,\
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
  DeeTypeObject *_fd_used_error;\
  switch ((_fd_error = errno)) {\
   case EISDIR: case ENOTDIR: case ENOENT: _fd_used_error = &DeeErrorType_FileNotFound; break;\
   default:                                _fd_used_error = &DeeErrorType_IOError; break;\
  }\
  DeeError_SetStringf(_fd_used_error,\
                      "_wopen(%lq,%d,%.4o) : %K",filename,\
                      DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(mode),(int)(perms),\
                      DeeSystemError_ToString(_fd_error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeUnixSysFileFD_WideInitObject(self,filename,mode,perms,...)\
 DeeUnixSysFileFD_WideInit(self,DeeWideString_STR(filename),mode,perms,__VA_ARGS__)
#endif



DEE_STATIC_INLINE(void) _deeunix_quick_itos(char *out, int v) {
 int used_v = v,len = 0; char *used_out;
 do ++len; while ((used_v /= 10) != 0);
 used_v = v,used_out = out+len,*used_out = 0;
 do *--out = '0'+(used_v%10); while ((used_v /= 10) != 0);
}

#ifndef DEE_PRIVATE_NFS_UTF8READLINK_DECLARED
#define DEE_PRIVATE_NFS_UTF8READLINK_DECLARED
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8Readlink(DEE_A_IN_Z Dee_Utf8Char const *path);
#endif
DEE_STATIC_INLINE(DeeObject *) DeeUnixSysFD_DoGetUtf8Name(int fd) {
 Dee_Utf8Char buffer[32] = {'/','p','r','o','c','/','s','e','l','f','/','f','d','/'};
 _deeunix_quick_itos(buffer+15,fd);
 return DeeNFS_Utf8Readlink(buffer);
}
#define DeeUnixSysFD_DoGetUtf8Name        DeeUnixSysFD_DoGetUtf8Name
#define DeeUnixSysFileFD_Utf8Filename(ob) DeeUnixSysFD_DoGetUtf8Name((ob)->unx_fd)


#if DEE_HAVE_FSTAT
#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif
#define DeeUnixSys_FDGetTimes(fd,atime,ctime,mtime,...)\
do{\
 struct stat _ut_st;\
 if DEE_UNLIKELY(fstat(fd,&_ut_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fstat(%d) : %K",fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 if (atime) *(atime) = DeeTime_TimeT2Mseconds(_ut_st.st_atime);\
 if (ctime) *(ctime) = DeeTime_TimeT2Mseconds(_ut_st.st_ctime);\
 if (mtime) *(mtime) = DeeTime_TimeT2Mseconds(_ut_st.st_mtime);\
}while(0)
#define DeeUnixSysFileFD_GetTimes(self,atime,ctime,mtime,...)\
 DeeUnixSys_FDGetTimes((self)->unx_fd,atime,ctime,mtime,__VA_ARGS__)
#endif /* DEE_HAVE_FSTAT */


#if DEE_HAVE_FUTIMENS
// int futimens(int fd, const struct timespec times[2]);
#define DeeUnixSys_FDSetTimes_impl(fd,atime,mtime,...) \
do{\
 struct timespec _ut_spec[2];\
 if ((atime) && (mtime)) {\
  _ut_spec[0].tv_sec = (Dee_time_t)DeeTime_TimeT2Mseconds(*(atime));\
  _ut_spec[0].tv_nsec = (long)(*(atime) % 1000)*1000000;\
  _ut_spec[1].tv_sec = (Dee_time_t)DeeTime_TimeT2Mseconds(*(mtime));\
  _ut_spec[1].tv_nsec = (long)(*(mtime) % 1000)*1000000;\
 } else {\
  struct stat _tm_st;\
  if DEE_UNLIKELY(fstat(fd,&_tm_st) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "fstat(%d) : %K",fd,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
   {__VA_ARGS__;}\
  }\
  if (atime) {\
   _ut_spec[0].tv_sec = (Dee_time_t)DeeTime_TimeT2Mseconds(*(atime));\
   _ut_spec[0].tv_nsec = (long)(*(atime) % 1000)*1000000;\
  } else _ut_spec[0].tv_sec = _tm_st.st_atime,_ut_spec[0].tv_nsec = 0;\
  if (mtime) {\
   _ut_spec[1].tv_sec = (Dee_time_t)DeeTime_TimeT2Mseconds(*(mtime));\
   _ut_spec[1].tv_nsec = (long)(*(mtime) % 1000)*1000000;\
  } else _ut_spec[1].tv_sec = _tm_st.st_mtime,_ut_spec[1].tv_nsec = 0;\
 }\
 if (futimens(fd,_ut_spec) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"futimens(%d,{"\
                      "{%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_TIME_T)) ",%ld},"\
                      "{%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_TIME_T)) ",%ld}}) : %K",fd,\
                      _ut_spec[0].tv_sec,_ut_spec[0].tv_nsec,\
                      _ut_spec[1].tv_sec,_ut_spec[1].tv_nsec,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#elif DEE_HAVE_FUTIMES
// int futimes(int fd, const struct timeval tv[2]);
#define DeeUnixSys_FDSetTimes_impl(fd,atime,mtime,...) \
do{\
 struct timeval _tm_val[2];\
 if ((atime) && (mtime)) {\
  _tm_val[0].tv_sec = (long)DeeTime_TimeT2Mseconds(*(atime));\
  _tm_val[0].tv_usec = (long)(*(atime) % 1000);\
  _tm_val[1].tv_sec = (long)DeeTime_TimeT2Mseconds(*(mtime));\
  _tm_val[1].tv_usec = (long)(*(mtime) % 1000);\
 } else {\
  struct stat _tm_st;\
  if DEE_UNLIKELY(fstat(fd,&_tm_st) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"fstat(%d) : %K",fd,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
   {__VA_ARGS__;}\
  }\
  if (atime) {\
   _tm_val[0].tv_sec = (long)DeeTime_TimeT2Mseconds(*(atime));\
   _tm_val[0].tv_usec = (long)(*(atime) % 1000);\
  } else _tm_val[0].tv_sec = _tm_st.st_atime,_tm_val[0].tv_usec = 0;\
  if (mtime) {\
   _tm_val[1].tv_sec = (long)DeeTime_TimeT2Mseconds(*(mtime));\
   _tm_val[1].tv_usec = (long)(*(mtime) % 1000);\
  } else _tm_val[1].tv_sec = _tm_st.st_mtime,_tm_val[1].tv_usec = 0;\
 }\
 if DEE_UNLIKELY(futimes(fd,_tm_val) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "futimes(%d,{{%ld,%ld},{%ld,%ld}}) : %K",fd,\
                      (long)_tm_val[0].tv_sec,(long)_tm_val[0].tv_usec,\
                      (long)_tm_val[1].tv_sec,(long)_tm_val[1].tv_usec,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#ifdef DeeUnixSys_FDSetTimes_impl
#define DeeUnixSys_FDSetTimes(fd,atime,ctime,mtime,...)\
 DeeUnixSys_FDSetTimes_impl(fd,atime,mtime,__VA_ARGS__)
#endif

#ifdef DeeUnixSys_FDSetTimes
#define DeeUnixSysFileFD_SetTimes(self,atime,ctime,mtime,...)\
 DeeUnixSys_FDSetTimes((self)->unx_fd,atime,ctime,mtime,__VA_ARGS__)
#endif



#if DEE_HAVE_FSTAT
#ifndef S_ISDIR
#ifdef S_IFDIR
# define S_ISDIR(x) (((x)&S_IFDIR)!=0)
#elif defined(_S_IFDIR)
# define S_ISDIR(x) (((x)&_S_IFDIR)!=0)
#endif
#endif
#ifndef S_ISREG
#ifdef S_IFREG
# define S_ISREG(x) (((x)&S_IFREG)!=0)
#elif defined(_S_IFREG)
# define S_ISREG(x) (((x)&_S_IFREG)!=0)
#endif
#endif
#ifndef S_ISLNK
#ifdef S_IFLNK
# define S_ISLNK(x) (((x)&S_IFLNK)!=0)
#elif defined(_S_IFLNK)
# define S_ISLNK(x) (((x)&_S_IFLNK)!=0)
#endif
#endif

#define DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(ismacro,fd,result,...)\
do{\
 struct stat _fd_st;\
 if DEE_UNLIKELY(fstat(fd,&_fd_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fstat(%d) : %K",fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = ismacro(_fd_st.st_mode);\
}while(0)

#ifdef S_ISREG
#define DeeUnixSys_FDIsFile(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISREG,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISDIR
#define DeeUnixSys_FDIsDir(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISDIR,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISCHR
#define DeeUnixSys_FDIsCharDev(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISCHR,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISBLK
#define DeeUnixSys_IsFDBlockDev(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISBLK,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISFIFO
#define DeeUnixSys_FDIsFiFo(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISFIFO,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISLNK
#define DeeUnixSys_FDIsLink(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISLNK,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISSOCK
#define DeeUnixSys_FDIsSocket(fd,result,...) DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISSOCK,fd,result,__VA_ARGS__)
#endif
#define DEE_UNIX_PRIVATE_IS_EXEC(mode) (((mode)&0111)!=0)
#define DeeUnixSys_FDIsExecutable(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(DEE_UNIX_PRIVATE_IS_EXEC,fd,result,__VA_ARGS__)
#ifdef DeeUnixSys_FDIsFile
#define DeeUnixSysFileFD_IsFile(self,result,...) DeeUnixSys_FDIsFile((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsDir
#define DeeUnixSysFileFD_IsDir(self,result,...) DeeUnixSys_FDIsDir((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsLink
#define DeeUnixSysFileFD_IsLink(self,result,...) DeeUnixSys_FDIsLink((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsCharDev
#define DeeUnixSysFileFD_IsCharDev(self,result,...) DeeUnixSys_FDIsCharDev((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsBlockDev
#define DeeUnixSysFileFD_IsBlockDev(self,result,...) DeeUnixSys_IsFDBlockDev((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsFiFo
#define DeeUnixSysFileFD_IsFiFo(self,result,...) DeeUnixSys_FDIsFiFo((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDIsSocket
#define DeeUnixSysFileFD_IsSocket(self,result,...) DeeUnixSys_FDIsSocket((self)->unx_fd,result,__VA_ARGS__)
#endif
#define DeeUnixSysFileFD_IsExecutable(self,result,...) DeeUnixSys_FDIsExecutable((self)->unx_fd,result,__VA_ARGS__)

// TODO: DeeUnixSysFileFD_IsMount could be implemented being a little bit faster...
#define DeeUnixSys_FDGetMod(fd,result,...)\
do{\
 struct stat _fd_st;\
 if DEE_UNLIKELY(fstat(fd,&_fd_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fstat(%d) : %K",fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 *(result) = _fd_st.st_mode & 0777;\
}while(0)
#define DeeUnixSysFileFD_GetMod(self,result,...) DeeUnixSys_FDGetMod((self)->unx_fd,result,__VA_ARGS__)
#endif /* DEE_HAVE_FSTAT */

#if DEE_HAVE_FCHMOD
#define DeeUnixSys_FDChMod(fd,mode,...)\
do{\
 if DEE_UNLIKELY(fchmod(fd,mode) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fchmod(%d,%.4o) : %K",fd,(int)(mode),\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#ifdef DeeUnixSys_FDChMod
#define DeeUnixSysFileFD_ChMod(self,mode,...) DeeUnixSys_FDChMod((self)->unx_fd,mode,__VA_ARGS__)
#endif


#if DEE_HAVE_FSTAT
#define DeeUnixSys_FDGetOwn(fd,owner,group,...)\
do{\
 struct stat _fd_st;\
 if DEE_UNLIKELY(fstat(fd,&_fd_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fstat(%d) : %K",fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 *(owner) = _fd_st.st_uid;\
 *(group) = _fd_st.st_gid;\
}while(0)

#endif /* DEE_HAVE_FSTAT */


#if DEE_HAVE_FCHOWN
#define DeeUnixSys_FDChown(fd,owner,group,...) \
do{\
 if DEE_UNLIKELY(fchown(fd,owner,group) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fchown(%d,%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_UID_T)) "d,"\
                                "%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_GID_T)) "d) : %K",fd,\
                      (Dee_uid_t)(owner),(Dee_gid_t)(group),\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_HAVE_FCHOWN */

#ifdef DeeUnixSys_FDGetOwn
#define DeeUnixSysFileFD_GetOwn(self,owner,group,...) DeeUnixSys_FDGetOwn((self)->unx_fd,owner,group,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_FDChown
#define DeeUnixSysFileFD_Chown(self,owner,group,...)  DeeUnixSys_FDChown((self)->unx_fd,owner,group,__VA_ARGS__)
#endif





//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysPipeFD ===
#if DEE_HAVE_PIPE
#define DeeUnixSysPipeFD DeeUnixSysPipeFD
struct DeeUnixSysPipeFD { DEE_UNIX_SYSFD_HEAD };
#define DeeUnixSysPipeFD_Init(reader,writer,...)\
do{\
 int _up_handles[2];\
 if DEE_UNLIKELY(pipe(_up_handles) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"pipe() : %K",\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 (reader)->unx_fd = _up_handles[0];\
 (writer)->unx_fd = _up_handles[1];\
}while(0)
#endif /* DEE_HAVE_PIPE */


#ifdef DEE_UNIX_SYSFD_SEEK_SET
#define DEE_SYSFD_SEEK_SET  DEE_UNIX_SYSFD_SEEK_SET
#endif
#ifdef DEE_UNIX_SYSFD_SEEK_CUR
#define DEE_SYSFD_SEEK_CUR  DEE_UNIX_SYSFD_SEEK_CUR
#endif
#ifdef DEE_UNIX_SYSFD_SEEK_END
#define DEE_SYSFD_SEEK_END  DEE_UNIX_SYSFD_SEEK_END
#endif

#ifdef DeeUnixSysFD
#define DeeSysFD            DeeUnixSysFD
#ifdef DeeUnixSysFD_Quit
#define DeeSysFD_Quit       DeeUnixSysFD_Quit
#endif
#ifdef DeeUnixSysFD_InitCopy
#define DeeSysFD_InitCopy   DeeUnixSysFD_InitCopy
#endif
#ifdef DeeUnixSysFD_Read
#define DeeSysFD_Read       DeeUnixSysFD_Read
#endif
#ifdef DeeUnixSysFD_Write
#define DeeSysFD_Write      DeeUnixSysFD_Write
#endif
#ifdef DeeUnixSysFD_ReadAt
#define DeeSysFD_ReadAt     DeeUnixSysFD_ReadAt
#endif
#ifdef DeeUnixSysFD_WriteAt
#define DeeSysFD_WriteAt    DeeUnixSysFD_WriteAt
#endif
#ifdef DeeUnixSysFD_Seek
#define DeeSysFD_Seek       DeeUnixSysFD_Seek
#endif
#ifdef DeeUnixSysFD_Flush
#define DeeSysFD_Flush      DeeUnixSysFD_Flush
#endif
#ifdef DeeUnixSysFD_Trunc
#define DeeSysFD_Trunc      DeeUnixSysFD_Trunc
#endif
#ifdef DeeUnixSysFD_INIT_STDIN
#define DeeSysFD_INIT_STDIN  DeeUnixSysFD_INIT_STDIN
#endif
#ifdef DeeUnixSysFD_INIT_STDOUT
#define DeeSysFD_INIT_STDOUT DeeUnixSysFD_INIT_STDOUT
#endif
#ifdef DeeUnixSysFD_INIT_STDERR
#define DeeSysFD_INIT_STDERR DeeUnixSysFD_INIT_STDERR
#endif
#ifdef DeeUnixSysFD_GET_STDIN
#define DeeSysFD_GET_STDIN   DeeUnixSysFD_GET_STDIN
#endif
#ifdef DeeUnixSysFD_GET_STDOUT
#define DeeSysFD_GET_STDOUT  DeeUnixSysFD_GET_STDOUT
#endif
#ifdef DeeUnixSysFD_GET_STDERR
#define DeeSysFD_GET_STDERR  DeeUnixSysFD_GET_STDERR
#endif
#endif /* DeeUnixSysFD */

#ifdef DeeUnixSysFileFD
#define DeeSysFileFD                   DeeUnixSysFileFD
#ifdef DeeUnixSysFileFD_Utf8Init
#define DeeSysFileFD_Utf8Init          DeeUnixSysFileFD_Utf8Init
#endif
#ifdef DeeUnixSysFileFD_WideInit
#define DeeSysFileFD_WideInit          DeeUnixSysFileFD_WideInit
#endif
#ifdef DeeUnixSysFileFD_Utf8InitObject
#define DeeSysFileFD_Utf8InitObject    DeeUnixSysFileFD_Utf8InitObject
#endif
#ifdef DeeUnixSysFileFD_WideInitObject
#define DeeSysFileFD_WideInitObject    DeeUnixSysFileFD_WideInitObject
#endif
#ifdef DeeUnixSysFileFD_Utf8Filename
#define DeeSysFileFD_Utf8Filename      DeeUnixSysFileFD_Utf8Filename
#endif
#ifdef DeeUnixSysFileFD_GetTimes
#define DeeSysFileFD_GetTimes          DeeUnixSysFileFD_GetTimes
#endif
#ifdef DeeUnixSysFileFD_SetTimes
#define DeeSysFileFD_SetTimes          DeeUnixSysFileFD_SetTimes
#endif
#ifdef DeeUnixSysFileFD_IsFile
#define DeeSysFileFD_IsFile            DeeUnixSysFileFD_IsFile
#endif
#ifdef DeeUnixSysFileFD_IsDir
#define DeeSysFileFD_IsDir             DeeUnixSysFileFD_IsDir
#endif
#ifdef DeeUnixSysFileFD_IsLink
#define DeeSysFileFD_IsLink            DeeUnixSysFileFD_IsLink
#endif
#ifdef DeeUnixSysFileFD_IsCharDev
#define DeeSysFileFD_IsCharDev         DeeUnixSysFileFD_IsCharDev
#endif
#ifdef DeeUnixSysFileFD_IsBlockDev
#define DeeSysFileFD_IsBlockDev        DeeUnixSysFileFD_IsBlockDev
#endif
#ifdef DeeUnixSysFileFD_IsFiFo
#define DeeSysFileFD_IsFiFo            DeeUnixSysFileFD_IsFiFo
#endif
#ifdef DeeUnixSysFileFD_IsSocket
#define DeeSysFileFD_IsSocket          DeeUnixSysFileFD_IsSocket
#endif
#ifdef DeeUnixSysFileFD_IsExecutable
#define DeeSysFileFD_IsExecutable      DeeUnixSysFileFD_IsExecutable
#endif
#ifdef DeeUnixSysFileFD_GetMod
#define DeeSysFileFD_GetMod            DeeUnixSysFileFD_GetMod
#endif
#ifdef DeeUnixSysFileFD_ChMod
#define DeeSysFileFD_ChMod             DeeUnixSysFileFD_ChMod
#endif
#ifdef DeeUnixSysFileFD_GetOwn
#define DeeSysFileFD_GetOwn           DeeUnixSysFileFD_GetOwn
#endif
#ifdef DeeUnixSysFileFD_Chown
#define DeeSysFileFD_Chown            DeeUnixSysFileFD_Chown
#endif
#endif /* DeeUnixSysFileFD */

#ifdef DeeUnixSysPipeFD
#define DeeSysPipeFD         DeeUnixSysPipeFD
#ifdef DeeUnixSysPipeFD_Init
#define DeeSysPipeFD_Init    DeeUnixSysPipeFD_Init
#endif
#endif /* DeeUnixSysPipeFD */
#undef DEE_UNIX_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS_POSIX_SYSFD_H */
