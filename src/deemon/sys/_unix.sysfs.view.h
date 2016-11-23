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
#ifndef GUARD_DEEMON_SYS__UNIX_SYSFS_VIEW_H
#define GUARD_DEEMON_SYS__UNIX_SYSFS_VIEW_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/posix_features.inl>
#include <deemon/optional/atomic_mutex.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_DIRENT_H
#include <dirent.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifdef __INTELLISENSE__ /* Syntax highlighting! */
#ifndef PATH_MAX
#define PATH_MAX 1024
#endif /* !PATH_MAX */

#define _DIRENT_HAVE_D_TYPE
#define _DIRENT_HAVE_D_NAMLEN
#define _D_EXACT_NAMLEN(p) ((p)->d_namlen)
#define DT_UNKNOWN  0
#define DT_REG      1
#define DT_DIR      2
#define DT_FIFO     3
#define DT_SOCK     4
#define DT_CHR      5
#define DT_BLK      6
#define DT_LNK      7
struct dirent {
 long           d_ino;            /* Always zero */
 unsigned short d_reclen;         /* Structure size */
 size_t         d_namlen;         /* Length of name without \0 */
 int            d_type;           /* File type */
 char           d_name[PATH_MAX]; /* File name */
};
typedef struct dirent dirent;

struct DIR { int placeholder; };
typedef struct DIR DIR;
static DIR *opendir (const char *dirname);
static struct dirent *readdir (DIR *dirp);
static int closedir (DIR *dirp);
static void rewinddir (DIR* dirp);
#endif

#define DeeUnixSysFSUtf8View DeeUnixSysFSUtf8View
struct DeeUnixSysFSUtf8View {
 struct dirent        *unx_dp;
 DIR                  *unx_dfd;
 struct DeeAtomicMutex unx_lock;
};

#define DeeUnixSysFSUtf8View_Acquire(self) DeeAtomicMutex_AcquireRelaxed(&(self)->unx_lock)
#define DeeUnixSysFSUtf8View_Release(self) DeeAtomicMutex_Release(&(self)->unx_lock)

#define DeeUnixSysFSUtf8View_Quit(self)\
do{ if ((self)->unx_dfd) closedir((self)->unx_dfd); }while(0)
#define DeeUnixSysFSUtf8View_Init(self,path,...)\
do{\
 if DEE_UNLIKELY(((self)->unx_dfd = opendir(path)) == NULL) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "opendir(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 while (1) {\
  char const *_uv_name; errno = 0;\
  if DEE_UNLIKELY(((self)->unx_dp = readdir((self)->unx_dfd)) == NULL) {\
   int _uv_error = errno;\
   closedir((self)->unx_dfd);\
   if (_uv_error == 0) { (self)->unx_dfd = NULL; break; }\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "readdir(%p:%q) : %K",(self)->unx_dfd,path,\
                       DeeSystemError_ToString(_uv_error));\
   {__VA_ARGS__;}\
  }\
  _uv_name = (self)->unx_dp->d_name;\
  if (!(_uv_name[0] == '.' && (!_uv_name[1] || (_uv_name[1] == '.' && !_uv_name[2])))) break;\
 }\
 DeeAtomicMutex_Init(&(self)->unx_lock);\
}while(0)
#define DeeUnixSysFSUtf8View_IsDone(self) ((self)->unx_dp == NULL)
#define DeeUnixSysFSUtf8View_AdvanceAndReleaseOnError(self,...) \
do{\
 errno = 0;\
 if DEE_UNLIKELY(((self)->unx_dp = readdir((self)->unx_dfd)) == NULL) {\
  int _uv_error = errno; DIR *_uv_dfd;\
  if (_uv_error == 0) {\
   closedir((self)->unx_dfd);\
   (self)->unx_dfd = NULL;\
  } else {\
   _uv_dfd = (self)->unx_dfd;\
   DeeUnixSysFSUtf8View_Release(self);\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "readdir(%p) : %K",_uv_dfd,\
                       DeeSystemError_ToString(_uv_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)

#define DEE_UNIXSYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
#define DEE_UNIXSYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
#define DeeUnixSysFSUtf8View_GetFilenameStrLocked(self) ((self)->unx_dfd->d_name)
#ifdef _D_EXACT_NAMLEN
#define DeeUnixSysFSUtf8View_GetFilenameSizeLocked(self) _D_EXACT_NAMLEN((self)->unx_dfd)
#elif defined(_DIRENT_HAVE_D_NAMLEN)
#define DeeUnixSysFSUtf8View_GetFilenameSizeLocked(self) ((self)->unx_dfd->d_namlen)
#endif

#define DeeUnixSysFSUtf8View_IsHiddenAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_name[0]=='.');}while(0)
#ifdef _DIRENT_HAVE_D_TYPE
#ifdef DT_BLK
#define DeeUnixSysFSUtf8View_IsBlockDevAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_BLK);}while(0)
#endif
#ifdef DT_CHR
#define DeeUnixSysFSUtf8View_IsCharDevAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_CHR);}while(0)
#endif
#ifdef DT_DIR
#define DeeUnixSysFSUtf8View_IsDirAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_DIR);}while(0)
#endif
#ifdef DT_FIFO
#define DeeUnixSysFSUtf8View_IsFiFoAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_FIFO);}while(0)
#endif
#ifdef DT_LNK
#define DeeUnixSysFSUtf8View_IsLinkAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_LNK);}while(0)
#endif
#ifdef DT_REG
#define DeeUnixSysFSUtf8View_IsFileAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_REG);}while(0)
#endif
#ifdef DT_SOCK
#define DeeUnixSysFSUtf8View_IsSocketAndReleaseOnError(self,result,...) do{*(result)=((self)->unx_dfd->d_type==DT_SOCK);}while(0)
#endif
#endif

#define DeeSysFSUtf8View                          DeeUnixSysFSUtf8View
#define DeeSysFSUtf8View_Quit                     DeeUnixSysFSUtf8View_Quit
#define DeeSysFSUtf8View_Init                     DeeUnixSysFSUtf8View_Init
#define DeeSysFSUtf8View_IsDone                   DeeUnixSysFSUtf8View_IsDone
#define DeeSysFSUtf8View_Acquire                  DeeUnixSysFSUtf8View_Acquire
#define DeeSysFSUtf8View_Release                  DeeUnixSysFSUtf8View_Release
#define DeeSysFSUtf8View_AdvanceAndReleaseOnError DeeUnixSysFSUtf8View_AdvanceAndReleaseOnError
#define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
#define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
#define DeeSysFSUtf8View_GetFilenameStrLocked     DeeUnixSysFSUtf8View_GetFilenameStrLocked
#ifdef DeeUnixSysFSUtf8View_GetFilenameSizeLocked
#define DeeSysFSUtf8View_GetFilenameSizeLocked    DeeUnixSysFSUtf8View_GetFilenameSizeLocked
#endif
#define DeeSysFSUtf8View_IsHiddenAndReleaseOnError DeeUnixSysFSUtf8View_IsHiddenAndReleaseOnError
#ifdef DeeUnixSysFSUtf8View_IsBlockDevAndReleaseOnError
#define DeeSysFSUtf8View_IsBlockDevAndReleaseOnError DeeUnixSysFSUtf8View_IsBlockDevAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsCharDevAndReleaseOnError
#define DeeSysFSUtf8View_IsCharDevAndReleaseOnError DeeUnixSysFSUtf8View_IsCharDevAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsDirAndReleaseOnError
#define DeeSysFSUtf8View_IsDirAndReleaseOnError DeeUnixSysFSUtf8View_IsDirAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsFiFoAndReleaseOnError
#define DeeSysFSUtf8View_IsFiFoAndReleaseOnError DeeUnixSysFSUtf8View_IsFiFoAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsLinkAndReleaseOnError
#define DeeSysFSUtf8View_IsLinkAndReleaseOnError DeeUnixSysFSUtf8View_IsLinkAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsFileAndReleaseOnError
#define DeeSysFSUtf8View_IsFileAndReleaseOnError DeeUnixSysFSUtf8View_IsFileAndReleaseOnError
#endif
#ifdef DeeUnixSysFSUtf8View_IsSocketAndReleaseOnError
#define DeeSysFSUtf8View_IsSocketAndReleaseOnError DeeUnixSysFSUtf8View_IsSocketAndReleaseOnError
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__UNIX_SYSFS_VIEW_H */
