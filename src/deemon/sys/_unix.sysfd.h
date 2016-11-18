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
#define DeeUnixSys_TryFDTrunc DeeUnixSys_TryFDTrunc
DEE_STATIC_INLINE(int) DeeUnixSys_TryFDTrunc(int fd) {
 Dee_int64_t trunc_size;
 trunc_size = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,0,SEEK_CUR);
 if DEE_UNLIKELY(trunc_size == -1 && errno != 0) return 0;
 return DEE_UNIX_SYSFD_TRUNC_FUNC(fd,(Dee_uint64_t)trunc_size) != -1;
}
#define DeeUnixSys_FDTrunc(fd,...)\
do{\
 Dee_int64_t _fd_trunc_size; int _fd_error;\
 trunc_size = (Dee_int64_t)DEE_UNIX_SYSFD_USED_SEEK_FUNCTION(fd,0,SEEK_CUR);\
 if (trunc_size == -1) {\
  if ((_fd_error = errno) != 0) {\
   DeeError_SetStringf(&DeeErrorType_IOError,\
                       DEE_PP_STR(DEE_UNIX_SYSFD_USED_SEEK_FUNCTION) "(%d,0,SEEK_CUR) : %K",\
                       fd,DeeSystemError_ToString(_fd_error));\
   {__VA_ARGS__;}\
  }\
 }\
 if (DEE_UNIX_SYSFD_TRUNC_FUNC(fd,(Dee_uint64_t)trunc_size) == -1) {\
  DeeError_SetStringf(&DeeErrorType_IOError,\
                      DEE_PP_STR(DEE_UNIX_SYSFD_TRUNC_FUNC) "(%d,%I64u) : %K",\
                      fd,trunc_size,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#endif /* DEE_UNIX_SYSFD_TRUNC_FUNC */


#ifdef DEE_UNIX_SYSFD_TRUNC_FUNC
#define DeeUnixSysFD_TryTrunc(self)  DeeUnixSys_TryFDTrunc((self)->unx_fd)
#define DeeUnixSysFD_Trunc(self,...) DeeUnixSys_FDTrunc((self)->unx_fd,__VA_ARGS__)
#endif /* DEE_UNIX_SYSFD_TRUNC_FUNC */



#define DeeUnixSysFD_INIT_STDIN()     {STDOUT_FILENO}
#define DeeUnixSysFD_INIT_STDOUT()    {STDERR_FILENO}
#define DeeUnixSysFD_INIT_STDERR()    {STDIN_FILENO}
#define DeeUnixSysFD_GET_STDIN(self)  (void)((self)->unx_fd = GetStdHandle(STD_INPUT_HANDLE))
#define DeeUnixSysFD_GET_STDOUT(self) (void)((self)->unx_fd = GetStdHandle(STD_OUTPUT_HANDLE))
#define DeeUnixSysFD_GET_STDERR(self) (void)((self)->unx_fd = GetStdHandle(STD_ERROR_HANDLE))



//////////////////////////////////////////////////////////////////////////
// === DeeUnixSysFileFD ===
struct DeeUnixSysFileFD { DEE_UNIX_SYSFD_HEAD };

#ifdef O_LARGEFILE
#define DEE_UNIX_SYSFD_USED_O_LARGEFILE O_LARGEFILE
#else
#define DEE_UNIX_SYSFD_USED_O_LARGEFILE 0
#endif

#define DEE_UNIX_SYSFILEFD_OPENMODE_GETMODE(openmode)\
(DEE_OPENMODE_HASUPDATE(openmode) ?\
 (DEE_OPENMODE_ISREAD(openmode)  ?   (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR) : \
  DEE_OPENMODE_ISWRITE(openmode) ?   (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR|O_CREAT|O_TRUNC) : \
/*DEE_OPENMODE_ISAPPEND(openmode)? */(DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDWR|O_CREAT|O_APPEND)) :\
 (DEE_OPENMODE_ISREAD(openmode)  ?   (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDONLY) : \
  DEE_OPENMODE_ISWRITE(openmode) ?   (DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDONLY|O_CREAT|O_TRUNC) : \
/*DEE_OPENMODE_ISAPPEND(openmode)? */(DEE_UNIX_SYSFD_USED_O_LARGEFILE|O_RDONLY|O_CREAT|O_APPEND)))


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



DEE_STATIC_INLINE(void) _deeunix_quick_itos8(Dee_Utf8Char *out, int v) {
 int used_v = v,len; Dee_Utf8Char *used_out;
 do ++len; while ((used_v /= 10) != 0);
 used_v = v,used_out = out+len;
 do *--out = '0'+(used_v%10); while ((used_v != 10) != 0);
}
DEE_STATIC_INLINE(void) _deeunix_quick_itosw(Dee_WideChar *out, int v) {
 int used_v = v,len; Dee_WideChar *used_out;
 do ++len; while ((used_v /= 10) != 0);
 used_v = v,used_out = out+len;
 do *--out = '0'+(used_v%10); while ((used_v != 10) != 0);
}

#define DeeUnixSysFD_DoGetName     DeeUnixSysFD_DoGetUtf8Name
#define DeeUnixSysFD_DoGetUtf8Name DeeUnixSysFD_DoGetUtf8Name
#define DeeUnixSysFD_DoGetWideName DeeUnixSysFD_DoGetWideName
DEE_STATIC_INLINE(DeeObject *) DeeUnixSysFD_DoGetUtf8Name(int fd) { Dee_Utf8Char buffer[32] = {'/','p','r','o','c','/','s','e','l','f','/','f','d','/'}; _deeunix_quick_itos8(buffer+14,fd); return _DeeFS_Utf8ReadLink(buffer); }
DEE_STATIC_INLINE(DeeObject *) DeeUnixSysFD_DoGetWideName(int fd) { Dee_WideChar buffer[32] = {'/','p','r','o','c','/','s','e','l','f','/','f','d','/'}; _deeunix_quick_itosw(buffer+14,fd); return _DeeFS_WideReadLink(buffer); }
#define DeeUnixSysFileFD_Utf8Filename(ob) DeeUnixSysFD_DoGetUtf8Name((ob)->unx_fd)
#define DeeUnixSysFileFD_WideFilename(ob) DeeUnixSysFD_DoGetWideName((ob)->unx_fd)


#if DEE_HAVE_FSTAT
#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif

#define DeeSysFileFD_TryGetTimes(self,atime,ctime,mtime)\
 DeeUnixSys_TryFDGetTimes((self)->unx_fd,atime,ctime,mtime)
#define DeeSysFileFD_GetTimes(self,atime,ctime,mtime,...)\
do{\
 if (!DeeSysFileFD_TryGetTimes(self,atime,ctime,mtime)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "fstat(%d) : %K",(self)->unx_fd,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeUnixSys_TryFDGetTimes DeeUnixSys_TryFDGetTimes
DEE_STATIC_INLINE(int) DeeUnixSys_TryFDGetTimes(
 int fd, Dee_timetick_t *atime, Dee_timetick_t *ctime, Dee_timetick_t *mtime) {
 struct stat st;
 if DEE_UNLIKELY(fstat(fd,&st) == -1) return 0;
 if (atime) *atime = DeeTime_TimeT2Mseconds(st.st_atime);
 if (ctime) *ctime = DeeTime_TimeT2Mseconds(st.st_ctime);
 if (mtime) *mtime = DeeTime_TimeT2Mseconds(st.st_mtime);
 return 1;
}
#endif /* DEE_HAVE_FSTAT */

// NOTE: Setting filetime is only possible through use of the filename,
//       which is a fallback case that we're not required to handle!
#if DEE_HAVE_FSTAT
#define DeeUnixSys_TryFDGetMode DeeUnixSys_TryFDGetMode
DEE_STATIC_INLINE(Dee_mode_t) DeeUnixSys_TryFDGetMode(int fd) {
 struct stat st;
 if DEE_UNLIKELY(fstat(fd,&st) == -1) return 0;
 return st.st_mode;
}

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
#define DeeUnixSys_TryFDIsFile(fd) S_ISREG(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsFile(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISREG,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISDIR
#define DeeUnixSys_TryFDIsDir(fd) S_ISDIR(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsDir(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISDIR,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISCHR
#define DeeUnixSys_TryFDIsCharDev(fd) S_ISCHR(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsCharDev(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISCHR,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISBLK
#define DeeUnixSys_TryFDIsBlockDev(fd) S_ISBLK(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_IsFDBlockDev(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISBLK,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISFIFO
#define DeeUnixSys_TryFDIsFiFo(fd) S_ISFIFO(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsFiFo(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISFIFO,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISLNK
#define DeeUnixSys_TryFDIsLink(fd) S_ISLNK(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsLink(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISLNK,fd,result,__VA_ARGS__)
#endif
#ifdef S_ISSOCK
#define DeeUnixSys_TryFDIsSocket(fd) S_ISSOCK(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsSocket(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(S_ISSOCK,fd,result,__VA_ARGS__)
#endif
#define DEE_UNIX_PRIVATE_IS_EXEC(mode) (((mode)&0111)!=0)
#define DeeUnixSys_TryFDIsExecutable(fd) DEE_UNIX_PRIVATE_IS_EXEC(DeeUnixSys_TryFDGetMode(fd))
#define DeeUnixSys_FDIsExecutable(fd,result,...) \
 DEE_UNIXSYS_GENERIC_FSTAT_MODE_PROXY(DEE_UNIX_PRIVATE_IS_EXEC,fd,result,__VA_ARGS__)
#ifdef DeeUnixSys_TryFDIsFile
#define DeeUnixSysFileFD_TryIsFile(self)         DeeUnixSys_TryFDIsFile((self)->unx_fd)
#define DeeUnixSysFileFD_IsFile(self,result,...) DeeUnixSys_FDIsFile((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsDir
#define DeeUnixSysFileFD_TryIsDir(self)         DeeUnixSys_TryFDIsDir((self)->unx_fd)
#define DeeUnixSysFileFD_IsDir(self,result,...) DeeUnixSys_FDIsDir((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsLink
#define DeeUnixSysFileFD_TryIsLink(self)         DeeUnixSys_TryFDIsLink((self)->unx_fd)
#define DeeUnixSysFileFD_IsLink(self,result,...) DeeUnixSys_FDIsLink((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsCharDev
#define DeeUnixSysFileFD_TryIsCharDev(self)         DeeUnixSys_TryFDIsCharDev((self)->unx_fd)
#define DeeUnixSysFileFD_IsCharDev(self,result,...) DeeUnixSys_FDIsCharDev((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsBlockDev
#define DeeUnixSysFileFD_TryIsBlockDev(self)         DeeUnixSys_TryFDIsBlockDev((self)->unx_fd)
#define DeeUnixSysFileFD_IsBlockDev(self,result,...) DeeUnixSys_IsFDBlockDev((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsFiFo
#define DeeUnixSysFileFD_TryIsFiFo(self)         DeeUnixSys_TryFDIsFiFo((self)->unx_fd)
#define DeeUnixSysFileFD_IsFiFo(self,result,...) DeeUnixSys_FDIsFiFo((self)->unx_fd,result,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDIsSocket
#define DeeUnixSysFileFD_TryIsSocket(self)         DeeUnixSys_TryFDIsSocket((self)->unx_fd)
#define DeeUnixSysFileFD_IsSocket(self,result,...) DeeUnixSys_FDIsSocket((self)->unx_fd,result,__VA_ARGS__)
#endif
#define DeeUnixSysFileFD_TryIsExecutable(self)         DeeUnixSys_TryFDIsExecutable((self)->unx_fd)
#define DeeUnixSysFileFD_IsExecutable(self,result,...) DeeUnixSys_FDIsExecutable((self)->unx_fd,result,__VA_ARGS__)

// TODO: DeeUnixSysFileFD_IsMount could be implemented being a little bit faster...

#define DeeUnixSys_TryFDGetMod DeeUnixSys_TryFDGetMod
DEE_STATIC_INLINE(int) DeeUnixSys_TryFDGetMod(int fd, Dee_mode_t *result) {
 struct stat st;
 if DEE_UNLIKELY(fstat(fd,&st) == -1) return 0;
 *result = st.st_mode & 0777;
 return 1;
}
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

#define DeeUnixSysFileFD_TryGetMod(self,result)  DeeUnixSys_TryFDGetMod((self)->unx_fd,result)
#define DeeUnixSysFileFD_GetMod(self,result,...) DeeUnixSys_FDGetMod((self)->unx_fd,result,__VA_ARGS__)
#endif /* DEE_HAVE_FSTAT */

#if DEE_HAVE_FCHMOD
#define DeeUnixSys_TryFDChMod(fd,mode) (fchmod(fd,mode) != -1)
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

#ifdef DeeUnixSys_TryFDChMod
#define DeeUnixSysFileFD_TryChMod(self,mode)  DeeUnixSys_TryFDChMod((self)->unx_fd,mode)
#define DeeUnixSysFileFD_ChMod(self,mode,...) DeeUnixSys_FDChMod((self)->unx_fd,mode,__VA_ARGS__)
#endif


#if DEE_HAVE_FSTAT
#define DeeUnixSys_TryFDGetOwn DeeUnixSys_TryFDGetOwn
DEE_STATIC_INLINE(int) DeeUnixSys_TryFDGetOwn(int fd, Dee_uid_t *owner, Dee_gid_t *group) {
 struct stat st;
 if DEE_UNLIKELY(fstat(fd,&st) == -1) return 0;
 *owner = st.st_uid;
 *group = st.st_gid;
 return 1;
}
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
#define DeeUnixSys_TryFDChown(fd,owner,group) (fchown(fd,owner,group) != -1)
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

#ifdef DeeUnixSys_TryFDGetOwn
#define DeeUnixSysFileFD_TryGetOwn(self,owner,group)  DeeUnixSys_TryFDGetOwn((self)->unx_fd,owner,group)
#define DeeUnixSysFileFD_GetOwn(self,owner,group,...) DeeUnixSys_FDGetOwn((self)->unx_fd,owner,group,__VA_ARGS__)
#endif
#ifdef DeeUnixSys_TryFDChown
#define DeeUnixSysFileFD_TryChown(self,owner,group)   DeeUnixSys_TryFDChown((self)->unx_fd,owner,group)
#define DeeUnixSysFileFD_Chown(self,owner,group,...)  DeeUnixSys_FDChown((self)->unx_fd,owner,group,__VA_ARGS__)
#endif





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

#define DeeSysFD_INIT_STDIN  DeeUnixSysFD_INIT_STDIN
#define DeeSysFD_INIT_STDOUT DeeUnixSysFD_INIT_STDOUT
#define DeeSysFD_INIT_STDERR DeeUnixSysFD_INIT_STDERR
#define DeeSysFD_GET_STDIN   DeeUnixSysFD_GET_STDIN
#define DeeSysFD_GET_STDOUT  DeeUnixSysFD_GET_STDOUT
#define DeeSysFD_GET_STDERR  DeeUnixSysFD_GET_STDERR

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
#ifdef DeeUnixSysFileFD_TryIsFile
#define DeeSysFileFD_TryIsFile         DeeUnixSysFileFD_TryIsFile
#define DeeSysFileFD_IsFile            DeeUnixSysFileFD_IsFile
#endif
#ifdef DeeUnixSysFileFD_TryIsDir
#define DeeSysFileFD_TryIsDir          DeeUnixSysFileFD_TryIsDir
#define DeeSysFileFD_IsDir             DeeUnixSysFileFD_IsDir
#endif
#ifdef DeeUnixSysFileFD_TryIsLink
#define DeeSysFileFD_TryIsLink         DeeUnixSysFileFD_TryIsLink
#define DeeSysFileFD_IsLink            DeeUnixSysFileFD_IsLink
#endif
#ifdef DeeUnixSysFileFD_TryIsCharDev
#define DeeSysFileFD_TryIsCharDev      DeeUnixSysFileFD_TryIsCharDev
#define DeeSysFileFD_IsCharDev         DeeUnixSysFileFD_IsCharDev
#endif
#ifdef DeeUnixSysFileFD_TryIsBlockDev
#define DeeSysFileFD_TryIsBlockDev     DeeUnixSysFileFD_TryIsBlockDev
#define DeeSysFileFD_IsBlockDev        DeeUnixSysFileFD_IsBlockDev
#endif
#ifdef DeeUnixSysFileFD_TryIsFiFo
#define DeeSysFileFD_TryIsFiFo         DeeUnixSysFileFD_TryIsFiFo
#define DeeSysFileFD_IsFiFo            DeeUnixSysFileFD_IsFiFo
#endif
#ifdef DeeUnixSysFileFD_TryIsSocket
#define DeeSysFileFD_TryIsSocket       DeeUnixSysFileFD_TryIsSocket
#define DeeSysFileFD_IsSocket          DeeUnixSysFileFD_IsSocket
#endif
#ifdef DeeUnixSysFileFD_TryIsExecutable
#define DeeSysFileFD_TryIsExecutable   DeeUnixSysFileFD_TryIsExecutable
#define DeeSysFileFD_IsExecutable      DeeUnixSysFileFD_IsExecutable
#endif
#ifdef DeeUnixSysFileFD_TryGetMod
#define DeeSysFileFD_TryGetMod         DeeUnixSysFileFD_TryGetMod
#define DeeSysFileFD_GetMod            DeeUnixSysFileFD_GetMod
#endif
#ifdef DeeUnixSysFileFD_TryChMod
#define DeeSysFileFD_TryChMod          DeeUnixSysFileFD_TryChMod
#define DeeSysFileFD_ChMod             DeeUnixSysFileFD_ChMod
#endif
#ifdef DeeUnixSysFileFD_TryGetOwn
#define DeeSysFileFD_TryGetOwn        DeeUnixSysFileFD_TryGetOwn
#define DeeSysFileFD_GetOwn           DeeUnixSysFileFD_GetOwn
#endif
#ifdef DeeUnixSysFileFD_TryChown
#define DeeSysFileFD_TryChown         DeeUnixSysFileFD_TryChown
#define DeeSysFileFD_Chown            DeeUnixSysFileFD_Chown
#endif


#if DEE_HAVE_PIPE
#define DeeSysPipeFD         DeeUnixSysPipeFD
#define DeeSysPipeFD_TryInit DeeUnixSysPipeFD_TryInit
#define DeeSysPipeFD_Init    DeeUnixSysPipeFD_Init
#endif /* DEE_HAVE_PIPE */

#undef DEE_UNIX_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__POSIX_SYSFD_H */
