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
#ifndef GUARD_DEEMON_SYS__UNIX_SYSFS_H
#define GUARD_DEEMON_SYS__UNIX_SYSFS_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/posix_features.inl>
#include <deemon/time.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H
#include <stdlib.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_DIRECT_H
#include <direct.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_DIRECT_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H
#include <pwd.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H
#include <sys/stat.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H
#include <sys/time.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H\
   && DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#include <sys/times.h>
#include <utime.h>
#endif /* ... */
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

DEE_STATIC_INLINE(int) DeeUnixSys_CheckGetCwdError(void) {
 int error = errno;
 switch (error) {
  case 0: case ERANGE: break;
  default:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "getcwd() : %K",
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
 }
 return 0;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUnixSys_Utf8GetCwd(void) {
 Dee_size_t bufsize,last_size = 0; DeeObject *result; Dee_Utf8Char *cwd_start; int error;
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) == NULL) return NULL;
 if DEE_UNLIKELY(!getcwd(DeeUtf8String_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) {
  if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0) {err_r: Dee_DECREF(result); return NULL; }
  if DEE_UNLIKELY(DeeUtf8String_Resize(&result,(bufsize = (
   last_size = DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)*2)) != 0) goto err_r;
  while DEE_UNLIKELY(!getcwd(DeeUtf8String_STR(result),bufsize)) {
   Dee_size_t new_size;
   if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0) goto err_r;
   if DEE_UNLIKELY(DeeUtf8String_Resize(&result,(new_size = bufsize*2)) != 0) goto err_r;
   last_size = bufsize,bufsize = new_size;
  }
 }
 cwd_start = DeeUtf8String_STR(result)+last_size;
 while (*cwd_start) ++cwd_start;
 last_size = (Dee_size_t)(cwd_start-DeeUtf8String_STR(result));
 bufsize = DeeUtf8String_SIZE(result);
 if (last_size != bufsize && DEE_UNLIKELY(DeeUtf8String_Resize(&result,last_size) != 0)) goto err_r;
 return result;
}
#define DeeUnixSysFS_Utf8GetCwd DeeUnixSys_Utf8GetCwd

#if DEE_HAVE__WGETCWD
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeUnixSys_WideGetCwd(void) {
 Dee_size_t bufsize,last_size = 0; DeeObject *result; Dee_WideChar *cwd_start; int error;
 if DEE_UNLIKELY((result = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) == NULL) return NULL;
 if DEE_UNLIKELY(!_wgetcwd(DeeWideString_STR(result),DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)) {
  if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0) {err_r: Dee_DECREF(result); return NULL; }
  if DEE_UNLIKELY(DeeWideString_Resize(&result,(bufsize = (
   last_size = DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD)*2)) != 0) goto err_r;
  while DEE_UNLIKELY(!_wgetcwd(DeeWideString_STR(result),bufsize)) {
   Dee_size_t new_size;
   if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0) goto err_r;
   if DEE_UNLIKELY(DeeWideString_Resize(&result,(new_size = bufsize*2)) != 0) goto err_r;
   last_size = bufsize,bufsize = new_size;
  }
 }
 cwd_start = DeeWideString_STR(result)+last_size;
 while (*cwd_start) ++cwd_start;
 last_size = (Dee_size_t)(cwd_start-DeeWideString_STR(result));
 bufsize = DeeWideString_SIZE(result);
 if (last_size != bufsize && DEE_UNLIKELY(DeeWideString_Resize(&result,last_size) != 0)) goto err_r;
 return result;
}
#define DeeUnixSysFS_WideGetCwd DeeUnixSys_WideGetCwd
#endif

#define DeeUnixSys_Utf8Chdir(path,...) \
do{\
 if DEE_UNLIKELY(chdir(path) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "chdir(%q) : %K",path,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#if DEE_HAVE__WCHDIR
#define DeeUnixSys_WideChdir(path,...) \
do{\
 if DEE_UNLIKELY(_wchdir(path) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "_wchdir(%lq) : %K",path,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#define DeeUnixSysFS_Utf8Chdir    DeeUnixSys_Utf8Chdir
#ifdef DeeUnixSys_WideChdir
#define DeeUnixSysFS_WideChdir    DeeUnixSys_WideChdir
#endif



#define DeeUnixSysFS_Utf8GetEnv DeeUnixSys_Utf8GetEnv
#define DeeUnixSys_Utf8GetEnv   DeeUnixSys_Utf8GetEnv
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeUnixSys_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 char const *env_value;
 if DEE_UNLIKELY((env_value = getenv(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getenv(%q) : Env var not found",
                      envname);
  return NULL;
 }
 return DeeUtf8String_New(env_value);
}
#define DeeUnixSysFS_Utf8TryGetEnv DeeUnixSys_Utf8TryGetEnv
#define DeeUnixSys_Utf8TryGetEnv   DeeUnixSys_Utf8TryGetEnv
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeUnixSys_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 char const *env_value; DeeObject *result;
 if DEE_UNLIKELY((env_value = getenv(envname)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeUtf8String_New(env_value)) == NULL) DeeError_HandledOne();
 return result;
}
#define DeeUnixSys_Utf8HasEnv(envname,result,...) \
do{ *(result) = getenv(envname) != NULL; }while(0)
#define DeeUnixSysFS_Utf8HasEnv    DeeUnixSys_Utf8HasEnv

#if DEE_HAVE__WGETENV
#define DeeUnixSysFS_WideGetEnv    DeeUnixSys_WideGetEnv
#define DeeSysFS_WideGetEnv DeeUnixSysFS_WideGetEnv
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
DeeUnixSys_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 Dee_WideChar const *env_value;
 if DEE_UNLIKELY((env_value = _wgetenv(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "_wgetenv(%lq) : Env var not found",
                      envname);
  return NULL;
 }
 return DeeWideString_New(env_value);
}
#define DeeUnixSysFS_WideTryGetEnv DeeUnixSys_WideTryGetEnv
#define DeeSysFS_WideTryGetEnv DeeUnixSysFS_WideTryGetEnv
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeUnixSys_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 Dee_WideChar const *env_value; DeeObject *result;
 if DEE_UNLIKELY((env_value = _wgetenv(envname)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeWideString_New(env_value)) == NULL) DeeError_HandledOne();
 return result;
}
#define DeeUnixSys_WideHasEnv(envname,result,...) \
do{ *(result) = (_wgetenv(envname) != NULL); }while(0)
#define DeeUnixSysFS_WideHasEnv    DeeUnixSys_WideHasEnv
#endif /* DEE_HAVE__WGETENV */

#if DEE_HAVE_UNSETENV
#define DeeUnixSys_Utf8DelEnv(envname,...) \
do{\
 if DEE_UNLIKELY(unsetenv(envname) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "unsetenv(%q) : %K",envname,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeUnixSysFS_Utf8DelEnv    DeeUnixSys_Utf8DelEnv
#endif /* DEE_HAVE_UNSETENV */

#if DEE_HAVE__WUNSETENV
#define DeeUnixSys_WideDelEnv(envname,...) \
do{\
 if DEE_UNLIKELY(_wunsetenv(envname) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "_wunsetenv(%lq) : %K",envname,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeUnixSysFS_WideDelEnv    DeeUnixSys_WideDelEnv
#endif /* DEE_HAVE__WUNSETENV */

#if DEE_HAVE_SETENV
#define DeeUnixSys_Utf8SetEnv(envname,newvalue,...) \
do{\
 if (setenv(envname,newvalue,1) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "setenv(%q,%q) : %K",envname,newvalue,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#elif DEE_HAVE_PUTENV
#define DeeUnixSys_Utf8SetEnv(envname,newvalue,...) \
do{\
 DeeObject *_env_cmd;\
 if DEE_UNLIKELY((cmd = DeeString_Newf("%s=%s",envname,newvalue)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(putenv(DeeString_STR(cmd)) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "putenv(%q) : %K",DeeString_STR(cmd),\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF(cmd);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF(cmd);\
}while(0)
#endif

#if DEE_HAVE__WSETENV
#define DeeUnixSys_WideSetEnv(envname,newvalue,...) \
do{\
 if (_wsetenv(envname,newvalue,1) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "_wsetenv(%lq,%lq) : %K",envname,newvalue,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#elif DEE_HAVE__WPUTENV
#define DeeUnixSys_WideSetEnv(envname,newvalue,...) \
do{\
 static Dee_WideChar const _env_fmt[] = {'%','l','s','=','%','l','s',0};\
 DeeObject *_env_cmd;\
 if DEE_UNLIKELY((cmd = DeeWideString_Newf(_env_fmt,envname,newvalue)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(_wputenv(DeeWideString_STR(cmd)) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "_wputenv(%lq) : %K",DeeWideString_STR(cmd),\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF(cmd);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF(cmd);\
}while(0)
#endif

#ifdef DeeUnixSys_Utf8SetEnv
#define DeeUnixSysFS_Utf8SetEnv    DeeUnixSys_Utf8SetEnv
#endif
#ifdef DeeUnixSys_WideSetEnv
#define DeeUnixSysFS_WideSetEnv    DeeUnixSys_WideSetEnv
#endif


#define DEE_UNIXSYS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeUnixSys_Utf8EnumEnv(enum,...) \
do{\
 char **_env_iter,*_env_one,*_env_value_begin;\
 if ((_env_iter = environ) != NULL) for (; (_env_one = *_env_iter) != NULL; ++_env_iter) {\
  _env_value_begin = _env_one;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  if (!*_env_value_begin) continue;\
  enum((Dee_size_t)(_env_value_begin-_env_one),_env_one),\
       strlen(_env_value_begin+1),_env_value_begin,__VA_ARGS__);\
 }\
}while(0)

#if DEE_HAVE__WENVIRON
#define DEE_UNIXSYS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeUnixSys_WideEnumEnv(enum,...) \
do{\
 Dee_WideChar **_env_iter,*_env_one,*_env_value_begin;\
 if ((_env_iter = _wenviron) != NULL) for (; (_env_one = *_env_iter) != NULL; ++_env_iter) {\
  _env_value_begin = _env_one;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  if (!*_env_value_begin) continue;\
  enum((Dee_size_t)(_env_value_begin-_env_one),_env_one),\
       strlen(_env_value_begin+1),_env_value_begin,__VA_ARGS__);\
 }\
}while(0)
#endif

#define DEE_UNIXSYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeUnixSysFS_Utf8EnumEnv DeeUnixSys_Utf8EnumEnv
#ifdef DeeUnixSys_WideEnumEnv
#define DEE_UNIXSYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeUnixSysFS_WideEnumEnv DeeUnixSys_WideEnumEnv
#endif


#if DEE_ENVIRONMENT_HAVE_INCLUDE_PWD_H
#define DeeUnixSys_Utf8GetHome DeeUnixSys_Utf8GetHome
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUnixSys_Utf8GetHome(void) {
 DeeObject *result;
 struct passwd *pw; Dee_uid_t my_uid;
#ifdef DeeUnixSys_Utf8TryGetEnv
 if DEE_UNLIKELY((result = DeeUnixSys_Utf8TryGetEnv("HOME")) != NULL) return result;
#endif
 my_uid = getuid();
 if DEE_UNLIKELY((pw = getpwuid(my_uid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwuid(%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_UID_T)) "u) : %K",
                      my_uid,DeeSystemError_ToString(DeeSystemError_Consume()));
  return NULL;
 }
 if DEE_UNLIKELY(!pw->pw_dir) DeeReturn_EmptyUtf8String;
 return DeeUtf8String_New(pw->pw_dir);
}
#define DeeUnixSys_Utf8GetUserHome DeeUnixSys_Utf8GetUserHome
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeUnixSys_Utf8GetUserHome(DEE_A_IN_Z Dee_Utf8Char const *username) {
 struct passwd *pd;
 pd = getpwnam(user); // TODO: getpwnam_r
 if DEE_UNLIKELY(!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(%q) : %K",user,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return NULL;
 }
 if DEE_UNLIKELY(!pd->pw_dir) DeeReturn_EmptyString; // Lets be careful...
 return DeeUtf8String_New(pd->pw_dir);
}
#endif

#ifdef DeeUnixSys_Utf8GetHome
#define DeeUnixSysFS_Utf8GetHome DeeUnixSys_Utf8GetHome
#endif
#ifdef DeeUnixSys_Utf8GetUserHome
#define DeeUnixSysFS_Utf8GetUserHome DeeUnixSys_Utf8GetUserHome
#endif


#define DeeUnixSys_Utf8GetTmp DeeUnixSys_Utf8GetTmp
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUnixSys_Utf8GetTmp(void) {
#ifdef P_tmpdir
 // NOTE: According to this:
 // http://www.gnu.org/software/libc/manual/html_node/Temporary-Files.html#index-P_005ftmpdir-1608
 // 'P_tmpdir' is a 'char const *', meaning we can't 'sizeof()' its length
 // (And while we probably still could, lets better not count on that...)
 return DeeUtf8String_New(P_tmpdir);
#else
 DEERETURN_STATIC_STRING_EX(4,{'/','t','m','p'});
#endif
}

#ifdef _wP_tmpdir
#define DeeUnixSys_WideGetTmp DeeUnixSys_WideGetTmp
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeUnixSys_WideGetTmp(void) {
 return DeeWideString_New(_wP_tmpdir);
}
#endif

#ifdef DeeUnixSys_Utf8GetTmp
#define DeeUnixSysFS_Utf8GetTmp DeeUnixSys_Utf8GetTmp
#endif
#ifdef DeeUnixSys_WideGetTmp
#define DeeUnixSysFS_WideGetTmp DeeUnixSys_WideGetTmp
#endif

#define DeeUnixSys_Utf8GetTimes(path,atime,ctime,mtime,...) \
do{\
 struct stat _stat_st;\
 _wstat\
 if DEE_UNLIKELY(stat(path,&_stat_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "stat(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 if (atime) *(atime) = DeeTime_TimeT2Mseconds(_stat_st.st_atime);\
 if (ctime) *(ctime) = DeeTime_TimeT2Mseconds(_stat_st.st_ctime);\
 if (mtime) *(mtime) = DeeTime_TimeT2Mseconds(_stat_st.st_mtime);\
}while(0)
#if DEE_HAVE__WSTAT
#define DeeUnixSys_WideGetTimes(path,atime,ctime,mtime,...) \
do{\
 struct _stat _stat_st;\
 if DEE_UNLIKELY(_wstat(path,&_stat_st) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "_wstat(%lq) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 if (atime) *(atime) = DeeTime_TimeT2Mseconds(_stat_st.st_atime);\
 if (ctime) *(ctime) = DeeTime_TimeT2Mseconds(_stat_st.st_ctime);\
 if (mtime) *(mtime) = DeeTime_TimeT2Mseconds(_stat_st.st_mtime);\
}while(0)
#endif

#define DeeUnixSysFS_Utf8GetTimes DeeUnixSys_Utf8GetTimes
#ifdef DeeUnixSys_WideGetTimes
#define DeeUnixSysFS_WideGetTimes DeeUnixSys_WideGetTimes
#endif

#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H
#define DeeUnixSys_Utf8SetTimes(path,atime,ctime,mtime,...) \
 DeeUnixSys_Utf8SetTimes_impl(path,atime,mtime,__VA_ARGS__)
#define DeeUnixSys_Utf8SetTimes_impl(path,atime,mtime,...) \
do{\
 struct timeval _tm_val[2];\
 if ((atime) && (mtime)) {\
  _tm_val[0].tv_sec = (long)DeeTime_TimeT2Mseconds(*atime);\
  _tm_val[0].tv_usec = (long)((*atime) % 1000);\
  _tm_val[1].tv_sec = (long)DeeTime_TimeT2Mseconds(*mtime);\
  _tm_val[1].tv_usec = (long)((*mtime) % 1000);\
 } else {\
  struct stat st;\
  if DEE_UNLIKELY(stat(path,&st) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "stat(%q) : %K",path,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
   {__VA_ARGS__;}\
  }\
  if (atime) {\
   _tm_val[0].tv_sec = (long)DeeTime_TimeT2Mseconds(*atime);\
   _tm_val[0].tv_usec = (long)((*atime) % 1000);\
  } else _tm_val[0].tv_sec = st.st_atime,_tm_val[0].tv_usec = 0;\
  if (mtime) {\
   _tm_val[1].tv_sec = (long)DeeTime_TimeT2Mseconds(*mtime);\
   _tm_val[1].tv_usec = (long)((*mtime) % 1000);\
  } else _tm_val[1].tv_sec = st.st_mtime,_tm_val[1].tv_usec = 0;\
 }\
 if DEE_UNLIKELY(utimes(path,tm_val) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "utimes(%q,{{%ld,%ld},{%ld,%ld}}) : %K",path,\
                      (long)_tm_val[0].tv_sec,(long)_tm_val[0].tv_usec,\
                      (long)_tm_val[1].tv_sec,(long)_tm_val[1].tv_usec,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_UTIME_H\
   && DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#define DeeUnixSys_Utf8SetTimes(path,atime,ctime,mtime,...) \
 DeeUnixSys_Utf8SetTimes_impl(path,atime,mtime,__VA_ARGS__)
#define DeeUnixSys_Utf8SetTimes_impl(path,atime,mtime,...) \
do{\
 struct utimbuf _tm_val;\
 if ((atime) && (mtime)) {\
  _tm_val.actime = (Dee_time_t)DeeTime_TimeT2Mseconds(*(atime));\
  _tm_val.modtime = (Dee_time_t)DeeTime_TimeT2Mseconds(*(mtime));\
 } else {\
  struct stat st;\
  if DEE_UNLIKELY(stat(path,&st) == -1) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "stat(%q) : %K",path,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
   {__VA_ARGS__;}\
  }\
  _tm_val.actime = (atime) ? (Dee_time_t)DeeTime_TimeT2Mseconds(*(atime)) : st.st_atime;\
  _tm_val.modtime = (mtime) ? (Dee_time_t)DeeTime_TimeT2Mseconds(*(mtime)) : st.st_mtime;\
 }\
 if DEE_UNLIKELY(utime(path,&_tm_val) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "utime(%q,{%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_TIME_T)) "u,"\
                                "%I" DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_TIME_T)) "u}) : %K",\
                      path,(Dee_time_t)_tm_val.actime,(Dee_time_t)_tm_val.modtime,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#ifdef DeeUnixSys_Utf8SetTimes
#define DeeUnixSysFS_Utf8SetTimes DeeUnixSys_Utf8SetTimes
#endif

#if DEE_HAVE_READLINK
#define DeeUnixSysFS_Utf8Readlink DeeUnixSys_Utf8Readlink
#define DeeUnixSys_Utf8Readlink(path,result,...) \
do{\
 Dee_ssize_t _ur_bytes,_ur_new_bytes; int _ur_error;\
 if DEE_UNLIKELY(((*result) = DeeUtf8String_NewSized(\
  DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK)) == NULL) {__VA_ARGS__;}\
 _ur_bytes = (Dee_ssize_t)readlink(path,DeeUtf8String_STR(*(result)),\
                                   DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);\
 while (1) {\
  if DEE_UNLIKELY(_ur_bytes < 0) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"readlink(%q) : %K",path,\
                       DeeSystemError_ToString(DeeSystemError_Consume()));\
   Dee_DECREF(*(result));\
   {__VA_ARGS__;}\
  } else if DEE_UNLIKELY(_ur_bytes > DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK) {\
   if DEE_UNLIKELY(DeeUtf8String_Resize(result,_ur_bytes) != 0) {Dee_DECREF(*(result));{__VA_ARGS__;}}\
   _ur_new_bytes = (Dee_ssize_t)readlink(path,DeeUtf8String_STR(*(result)),\
                                         DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK+1);\
   if DEE_UNLIKELY(_ur_new_bytes < 0) {\
    DeeError_SetStringf(&DeeErrorType_SystemError,"readlink(%q) : %K",path,\
                        DeeSystemError_ToString(DeeSystemError_Consume()));\
    Dee_DECREF(*(result));\
    {__VA_ARGS__;}\
   }\
   if DEE_UNLIKELY(_ur_new_bytes == _ur_bytes) break;\
   _ur_bytes = _ur_new_bytes; /* Link must have changed */\
  } else {\
   if DEE_UNLIKELY(DeeUtf8String_Resize(result,_ur_bytes) != 0) {Dee_DECREF(*(result));{__VA_ARGS__;}}\
   break;\
  }\
 }\
}while(0)
#endif /* DEE_HAVE_READLINK */


#if 0 /* TODO */
DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeFS_F(IsMount)(DEE_A_IN_Z DEE_CHAR const *path) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(path)) return DeeVFS_F(IsMount)(path);
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  return _DeeFS_F(IsDrive)(path);
#elif defined(WIDE)
  DeeObject *path_ob; int result;
  if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
  result = _DeeFS_Utf8IsMount(DeeUtf8String_STR(path_ob));
  Dee_DECREF(path_ob);
  return result;
#elif defined(DEE_PLATFORM_UNIX)
  // As described here:
  // http://stackoverflow.com/questions/10410513/function-or-a-systemcall-similar-to-mountpoint-command-in-linux
  struct stat mp,mp_parent; int temp;
  char *path_parent,*path_end; Dee_size_t path_size;
  if DEE_UNLIKELY((temp = _deefs_posix_try_stat(path,&mp)) != 0)
   return DEE_UNLIKELY(temp < 0) ? temp : 0;
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
  while DEE_UNLIKELY((path_parent = (char *)malloc_nz(
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
  if DEE_UNLIKELY((temp = _deefs_posix_try_stat(path_parent,&mp_parent)) != 0) {
#if !DEE_HAVE_ALLOCA
   free_nn(path_parent);
#endif
   return DEE_UNLIKELY(temp < 0) ? temp : 0;
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
#endif

#define DeeSysFS_Utf8GetCwd      DeeUnixSysFS_Utf8GetCwd
#ifdef DeeUnixSysFS_WideGetCwd
#define DeeSysFS_WideGetCwd      DeeUnixSysFS_WideGetCwd
#endif
#define DeeSysFS_Utf8Chdir       DeeUnixSysFS_Utf8Chdir
#ifdef DeeUnixSysFS_WideChdir
#define DeeSysFS_WideChdir       DeeUnixSysFS_WideChdir
#endif
#define DeeSysFS_Utf8GetEnv      DeeUnixSysFS_Utf8GetEnv
#ifdef DeeUnixSysFS_WideGetEnv
#define DeeSysFS_WideGetEnv      DeeUnixSysFS_WideGetEnv
#endif
#define DeeSysFS_Utf8HasEnv      DeeUnixSysFS_Utf8HasEnv
#ifdef DeeUnixSysFS_WideHasEnv
#define DeeSysFS_WideHasEnv      DeeUnixSysFS_WideHasEnv
#endif
#ifdef DeeUnixSysFS_Utf8DelEnv
#define DeeSysFS_Utf8DelEnv      DeeUnixSysFS_Utf8DelEnv
#endif
#ifdef DeeUnixSysFS_WideDelEnv
#define DeeSysFS_WideDelEnv      DeeUnixSysFS_WideDelEnv
#endif
#ifdef DeeUnixSysFS_Utf8SetEnv
#define DeeSysFS_Utf8SetEnv      DeeUnixSysFS_Utf8SetEnv
#endif
#ifdef DeeUnixSysFS_WideSetEnv
#define DeeSysFS_WideSetEnv      DeeUnixSysFS_WideSetEnv
#endif
#define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_Utf8EnumEnv     DeeUnixSysFS_Utf8EnumEnv
#ifdef DeeUnixSysFS_WideEnumEnv
#define DEE_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_WideEnumEnv     DeeUnixSysFS_WideEnumEnv
#endif
#ifdef DeeUnixSysFS_Utf8GetHome
#define DeeSysFS_Utf8GetHome     DeeUnixSysFS_Utf8GetHome
#endif
#ifdef DeeUnixSysFS_Utf8GetUserHome
#define DeeSysFS_Utf8GetUserHome DeeUnixSysFS_Utf8GetUserHome
#endif
#ifdef DeeUnixSysFS_Utf8GetTmp
#define DeeSysFS_Utf8GetTmp      DeeUnixSysFS_Utf8GetTmp
#endif
#ifdef DeeUnixSysFS_WideGetTmp
#define DeeSysFS_WideGetTmp      DeeUnixSysFS_WideGetTmp
#endif
#define DeeSysFS_Utf8GetTimes    DeeUnixSysFS_Utf8GetTimes
#ifdef DeeUnixSysFS_WideGetTimes
#define DeeSysFS_WideGetTimes    DeeUnixSysFS_WideGetTimes
#endif
#ifdef DeeUnixSysFS_Utf8SetTimes
#define DeeSysFS_Utf8SetTimes    DeeUnixSysFS_Utf8SetTimes
#endif
#ifdef DeeUnixSysFS_Utf8Readlink
#define DeeSysFS_Utf8Readlink    DeeUnixSysFS_Utf8Readlink
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_H */
