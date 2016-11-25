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
#include DEE_INCLUDE_MEMORY_API()

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

#define DeeUnixSys_Utf8GetCwd(result,...) \
do{\
 Dee_size_t _uc_bufsize,_uc_last_size = 0; Dee_Utf8Char *_uc_cwd_start; int _uc_error;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized((\
  _uc_bufsize = DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD))) == NULL) {__VA_ARGS__;}\
 while DEE_UNLIKELY(!getcwd(DeeUtf8String_STR(*(result)),_uc_bufsize)) {\
  if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0 || DeeUtf8String_Resize(\
   result,(_uc_bufsize = (_uc_last_size = _uc_bufsize)*2)) != 0) { Dee_DECREF(*(result)); {__VA_ARGS__;} }\
 }\
 _uc_last_size += Dee_Utf8StrLen(DeeUtf8String_STR(*(result))+_uc_last_size);\
 if (_uc_last_size != _uc_bufsize && DEE_UNLIKELY(DeeUtf8String_Resize(\
  result,_uc_last_size) != 0)) { Dee_DECREF(*(result)); {__VA_ARGS__;} }\
}while(0)
#define DeeUnixSysFS_Utf8GetCwd DeeUnixSys_Utf8GetCwd

#if DEE_HAVE__WGETCWD
#define DeeUnixSys_WideGetCwd(result,...) \
do{\
 Dee_size_t _uc_bufsize,_uc_last_size = 0; Dee_WideChar *_uc_cwd_start; int _uc_error;\
 if DEE_UNLIKELY((*(result) = DeeWideString_NewSized((\
  _uc_bufsize = DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD))) == NULL) {__VA_ARGS__;}\
 while DEE_UNLIKELY(!_wgetcwd(DeeWideString_STR(*(result)),_uc_bufsize)) {\
  if DEE_UNLIKELY(DeeUnixSys_CheckGetCwdError() != 0 || DeeWideString_Resize(\
   result,(_uc_bufsize = (_uc_last_size = _uc_bufsize)*2)) != 0) { Dee_DECREF(*(result)); {__VA_ARGS__;} }\
 }\
 _uc_last_size += Dee_WideStrLen(DeeWideString_STR(*(result))+_uc_last_size);\
 if (_uc_last_size != _uc_bufsize && DEE_UNLIKELY(DeeWideString_Resize(\
  result,_uc_last_size) != 0)) { Dee_DECREF(*(result)); {__VA_ARGS__;} }\
}while(0)
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
#ifndef environ
extern char **environ;
#endif
#define DeeUnixSys_Utf8EnumEnv(enum,...) \
do{\
 char **_env_iter,*_env_one,*_env_value_begin;\
 if ((_env_iter = environ) != NULL) for (; (_env_one = *_env_iter) != NULL; ++_env_iter) {\
  _env_value_begin = _env_one;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  if (!*_env_value_begin) continue;\
  enum((Dee_size_t)(_env_value_begin-_env_one),_env_one,\
       strlen(_env_value_begin+1),_env_value_begin,__VA_ARGS__);\
 }\
}while(0)

#if DEE_HAVE__WENVIRON
/*#ifndef _wenviron
extern wchar_t **_wenviron;
#endif*/
#define DEE_UNIXSYS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeUnixSys_WideEnumEnv(enum,...) \
do{\
 Dee_WideChar **_env_iter,*_env_one,*_env_value_begin;\
 if ((_env_iter = _wenviron) != NULL) for (; (_env_one = *_env_iter) != NULL; ++_env_iter) {\
  _env_value_begin = _env_one;\
  while (*_env_value_begin && *_env_value_begin != '=') ++_env_value_begin;\
  if (!*_env_value_begin) continue;\
  enum((Dee_size_t)(_env_value_begin-_env_one),_env_one,\
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
#ifdef DEE_PRIVATE_DECL_DEE_UID_T
DEE_PRIVATE_DECL_DEE_UID_T
#undef DEE_PRIVATE_DECL_DEE_UID_T
#endif
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
 pd = getpwnam(username); // TODO: getpwnam_r
 if DEE_UNLIKELY(!pd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getpwnam(%q) : %K",username,
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


#ifdef P_tmpdir
// NOTE: According to this:
// http://www.gnu.org/software/libc/manual/html_node/Temporary-Files.html#index-P_005ftmpdir-1608
// 'P_tmpdir' is a 'char const *', meaning we can't 'sizeof()' its length
// (And while we probably still could, lets better not count on that...)
#define DeeUnixSys_Utf8GetTmp(result,...) \
do{if DEE_UNLIKELY((*(result) = DeeUtf8String_New(P_tmpdir)) == NULL){__VA_ARGS__;}}while(0)
#else
#define DeeUnixSys_Utf8GetTmp(result,...) \
do{DeeString_NEW_STATIC_EX(_tmp_path,4,{'/','t','m','p'});\
   Dee_INCREF(*(result) = (DeeObject *)&_tmp_path);}while(0)
#endif

#ifdef _wP_tmpdir
#define DeeUnixSys_WideGetTmp(result,...) \
do{if DEE_UNLIKELY((*(result) = DeeWideString_New(_wP_tmpdir)) == NULL){__VA_ARGS__;}}while(0)
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
 if DEE_UNLIKELY(utimes(path,_tm_val) == -1) {\
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

#define DeeUnixSys_Utf8IsAbs(path,result,...) do{ *(result) = *(path) == '/'; }while(0)
#define DeeUnixSys_WideIsAbs(path,result,...) do{ *(result) = *(path) == '/'; }while(0)
#define DeeUnixSysFS_Utf8IsAbs DeeUnixSys_Utf8IsAbs
#define DeeUnixSysFS_WideIsAbs DeeUnixSys_WideIsAbs

// As described here:
// http://stackoverflow.com/questions/10410513/function-or-a-systemcall-similar-to-mountpoint-command-in-linux
// Minor modifications were made, as described here:
// https://fossies.org/linux/misc/sysvinit-2.88.tar.gz:a/sysvinit-2.88/src/mountpoint.c
#define DeeUnixSys_Utf8IsMountWithLength(path,path_size,result,...) \
do{\
 struct stat _mp,_mp_parent; int _temp;\
 char *_path_parent,*_path_end; Dee_size_t _path_size;\
 if DEE_UNLIKELY(stat(path,&_mp) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"stat(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 _path_size = (path_size);\
 while DEE_UNLIKELY((_path_parent = (char *)malloc_nz((_path_size+4)*sizeof(char))) == NULL) {\
  if DEE_LIKELY(Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 _path_end = _path_parent+_path_size;\
 if (_path_size) {\
  memcpy(_path_parent,path,_path_size*sizeof(char));\
  if (_path_end[-1] != '/') *_path_end++ = '/';\
 }\
 *_path_end++ = '.';\
 *_path_end++ = '.';\
 *_path_end++ = 0;\
 if DEE_UNLIKELY(stat(_path_parent,&_mp_parent) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"stat(%q) : %K",_path_parent,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  free_nn(_path_parent);\
  {__VA_ARGS__;}\
 }\
 free_nn(_path_parent);\
 *(result) = (_mp.st_dev != _mp_parent.st_dev)\
          || (_mp.st_dev == _mp_parent.st_dev && _mp.st_ino == _mp_parent.st_ino);\
}while(0)
#define DeeUnixSys_Utf8IsMount(path,result,...) \
 DeeUnixSys_Utf8IsMountWithLength(path,strlen(path),result,__VA_ARGS__)
#define DeeUnixSys_Utf8IsMountObject(path,result,...) \
 DeeUnixSys_Utf8IsMountWithLength(DeeUtf8String_STR(path),DeeUtf8String_SIZE(path),result,__VA_ARGS__)
#define DeeUnixSysFS_Utf8IsMount       DeeUnixSys_Utf8IsMount
#define DeeUnixSysFS_Utf8IsMountObject DeeUnixSys_Utf8IsMountObject

#define DEE_UNIXSYS_PRIVATE_ISNOTFOUNDERROR(eno)\
 ((eno) == ENOENT || (eno) == ENAMETOOLONG || (eno) == ENOTDIR)

#define DeeUnixSys_Utf8HasProperty(path,result,prop_query,...) \
do{\
 struct stat _hp_st; int _hp_error;\
 if DEE_UNLIKELY(stat(path,&_hp_st) == -1) {\
  _hp_error = errno;\
  if (DEE_UNIXSYS_PRIVATE_ISNOTFOUNDERROR(_hp_error)) *(result) = 0; else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"stat(%q) : %K",path,\
                       DeeSystemError_ToString(_hp_error));\
   {__VA_ARGS__;}\
  }\
 } else {\
  *(result) = prop_query(_hp_st.st_mode);\
 }\
}while(0)
#if DEE_HAVE_LSTAT
#define DeeUnixSys_Utf8LHasProperty(path,result,prop_query,...) \
do{\
 struct stat _hp_st; int _hp_error;\
 if DEE_UNLIKELY(lstat(path,&_hp_st) == -1) {\
  _hp_error = errno;\
  if (DEE_UNIXSYS_PRIVATE_ISNOTFOUNDERROR(_hp_error)) *(result) = 0; else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"lstat(%q) : %K",path,\
                       DeeSystemError_ToString(_hp_error));\
   {__VA_ARGS__;}\
  }\
 } else {\
  *(result) = prop_query(_hp_st.st_mode);\
 }\
}while(0)
#endif

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

#ifdef S_ISREG
#define DeeUnixSys_Utf8IsFile(path,result,...)       DeeUnixSys_Utf8HasProperty(path,result,S_ISREG,__VA_ARGS__)
#endif
#ifdef S_ISDIR
#define DeeUnixSys_Utf8IsDir(path,result,...)        DeeUnixSys_Utf8HasProperty(path,result,S_ISDIR,__VA_ARGS__)
#endif
#if defined(S_ISLNK) && defined(DeeUnixSys_Utf8LHasProperty)
#define DeeUnixSys_Utf8IsLink(path,result,...)       DeeUnixSys_Utf8LHasProperty(path,result,S_ISLNK,__VA_ARGS__)
#endif
#define DeeUnixSys_Utf8IsHidden(path,result,...)     do{*(result) = *(path) == '.';}while(0)
#define DEE_UNIXSYS_PRIVATE_S_ISEXECUTABLE(x)     (((x)&0111)!=0)
#define DeeUnixSys_Utf8IsExecutable(path,result,...) DeeUnixSys_Utf8HasProperty(path,result,DEE_UNIXSYS_PRIVATE_S_ISEXECUTABLE,__VA_ARGS__)
#ifdef S_ISCHR
#define DeeUnixSys_Utf8IsCharDev(path,result,...)    DeeUnixSys_Utf8HasProperty(path,result,S_ISCHR,__VA_ARGS__)
#endif
#ifdef S_ISBLK
#define DeeUnixSys_Utf8IsBlockDev(path,result,...)   DeeUnixSys_Utf8HasProperty(path,result,S_ISBLK,__VA_ARGS__)
#endif
#ifdef S_ISFIFO
#define DeeUnixSys_Utf8IsFiFo(path,result,...)       DeeUnixSys_Utf8HasProperty(path,result,S_ISFIFO,__VA_ARGS__)
#endif
#ifdef S_ISSOCK
#define DeeUnixSys_Utf8IsSocket(path,result,...)     DeeUnixSys_Utf8HasProperty(path,result,S_ISREG,__VA_ARGS__)
#endif
#define DEE_UNIXSYS_PRIVATE_S_EXISTS(x)              1
#define DeeUnixSys_Utf8Exists(path,result,...)       DeeUnixSys_Utf8HasProperty(path,result,DEE_UNIXSYS_PRIVATE_S_EXISTS,__VA_ARGS__)



#ifdef DeeUnixSys_Utf8IsFile
#define DeeUnixSysFS_Utf8IsFile DeeUnixSys_Utf8IsFile
#endif
#ifdef DeeUnixSys_Utf8IsDir
#define DeeUnixSysFS_Utf8IsDir DeeUnixSys_Utf8IsDir
#endif
#ifdef DeeUnixSys_Utf8IsLink
#define DeeUnixSysFS_Utf8IsLink DeeUnixSys_Utf8IsLink
#endif
#ifdef DeeUnixSys_Utf8IsHidden
#define DeeUnixSysFS_Utf8IsHidden DeeUnixSys_Utf8IsHidden
#endif
#ifdef DeeUnixSys_Utf8IsExecutable
#define DeeUnixSysFS_Utf8IsExecutable DeeUnixSys_Utf8IsExecutable
#endif
#ifdef DeeUnixSys_Utf8IsCharDev
#define DeeUnixSysFS_Utf8IsCharDev DeeUnixSys_Utf8IsCharDev
#endif
#ifdef DeeUnixSys_Utf8IsBlockDev
#define DeeUnixSysFS_Utf8IsBlockDev DeeUnixSys_Utf8IsBlockDev
#endif
#ifdef DeeUnixSys_Utf8IsFiFo
#define DeeUnixSysFS_Utf8IsFiFo DeeUnixSys_Utf8IsFiFo
#endif
#ifdef DeeUnixSys_Utf8IsSocket
#define DeeUnixSysFS_Utf8IsSocket DeeUnixSys_Utf8IsSocket
#endif
#ifdef DeeUnixSys_Utf8Exists
#define DeeUnixSysFS_Utf8Exists DeeUnixSys_Utf8Exists
#endif

#if !DEE_HAVE_REMOVE && DEE_HAVE_UNLINK && DEE_HAVE_RMDIR
#undef DEE_HAVE_REMOVE
#define DEE_HAVE_REMOVE 1
#define remove(path) ((unlink(path) == -1 && rmdir(path) == -1) ? -1 : 0)
#endif

#if DEE_HAVE_REMOVE
#define DeeUnixSys_Utf8Remove(path,...) \
do{\
 if DEE_UNLIKELY(remove(path) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"remove(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#if DEE_HAVE_UNLINK
#define DeeUnixSys_Utf8Unlink(path,...) \
do{\
 if DEE_UNLIKELY(unlink(path) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"unlink(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#if DEE_HAVE_RMDIR
#define DeeUnixSys_Utf8RmDir(path,...) \
do{\
 if DEE_UNLIKELY(rmdir(path) == -1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"rmdir(%q) : %K",path,\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif


#ifdef DeeUnixSys_Utf8Remove
#define DeeUnixSysFS_Utf8Remove DeeUnixSys_Utf8Remove
#endif
#ifdef DeeUnixSys_Utf8Unlink
#define DeeUnixSysFS_Utf8Unlink DeeUnixSys_Utf8Unlink
#endif
#ifdef DeeUnixSys_Utf8RmDir
#define DeeUnixSysFS_Utf8RmDir DeeUnixSys_Utf8RmDir
#endif





#ifdef DeeUnixSysFS_Utf8GetCwd
#define DeeSysFS_Utf8GetCwd      DeeUnixSysFS_Utf8GetCwd
#endif
#ifdef DeeUnixSysFS_WideGetCwd
#define DeeSysFS_WideGetCwd      DeeUnixSysFS_WideGetCwd
#endif
#ifdef DeeUnixSysFS_Utf8Chdir
#define DeeSysFS_Utf8Chdir       DeeUnixSysFS_Utf8Chdir
#endif
#ifdef DeeUnixSysFS_WideChdir
#define DeeSysFS_WideChdir       DeeUnixSysFS_WideChdir
#endif
#ifdef DeeUnixSysFS_Utf8GetEnv
#define DeeSysFS_Utf8GetEnv      DeeUnixSysFS_Utf8GetEnv
#endif
#ifdef DeeUnixSysFS_WideGetEnv
#define DeeSysFS_WideGetEnv      DeeUnixSysFS_WideGetEnv
#endif
#ifdef DeeUnixSysFS_Utf8HasEnv
#define DeeSysFS_Utf8HasEnv      DeeUnixSysFS_Utf8HasEnv
#endif
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
#ifdef DeeUnixSysFS_Utf8EnumEnv
#define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
#define DeeSysFS_Utf8EnumEnv     DeeUnixSysFS_Utf8EnumEnv
#endif
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
#ifdef DeeUnixSysFS_Utf8GetTimes
#define DeeSysFS_Utf8GetTimes    DeeUnixSysFS_Utf8GetTimes
#endif
#ifdef DeeUnixSysFS_WideGetTimes
#define DeeSysFS_WideGetTimes    DeeUnixSysFS_WideGetTimes
#endif
#ifdef DeeUnixSysFS_Utf8SetTimes
#define DeeSysFS_Utf8SetTimes    DeeUnixSysFS_Utf8SetTimes
#endif
#ifdef DeeUnixSysFS_Utf8Readlink
#define DeeSysFS_Utf8Readlink    DeeUnixSysFS_Utf8Readlink
#endif
#ifdef DeeUnixSysFS_Utf8IsAbs
#define DeeSysFS_Utf8IsAbs       DeeUnixSysFS_Utf8IsAbs
#endif
#ifdef DeeUnixSysFS_WideIsAbs
#define DeeSysFS_WideIsAbs       DeeUnixSysFS_WideIsAbs
#endif
#ifdef DeeUnixSysFS_Utf8IsMount
#define DeeSysFS_Utf8IsMount       DeeUnixSysFS_Utf8IsMount
#define DeeSysFS_Utf8IsDrive       DeeUnixSysFS_Utf8IsMount
#endif
#ifdef DeeUnixSysFS_Utf8IsMountObject
#define DeeSysFS_Utf8IsMountObject DeeUnixSysFS_Utf8IsMountObject
#define DeeSysFS_Utf8IsDriveObject DeeUnixSysFS_Utf8IsMountObject
#endif
#ifdef DeeUnixSysFS_Utf8IsFile
#define DeeSysFS_Utf8IsFile DeeUnixSysFS_Utf8IsFile
#endif
#ifdef DeeUnixSysFS_Utf8IsDir
#define DeeSysFS_Utf8IsDir DeeUnixSysFS_Utf8IsDir
#endif
#ifdef DeeUnixSysFS_Utf8IsLink
#define DeeSysFS_Utf8IsLink DeeUnixSysFS_Utf8IsLink
#endif
#ifdef DeeUnixSysFS_Utf8IsHidden
#define DeeSysFS_Utf8IsHidden DeeUnixSysFS_Utf8IsHidden
#endif
#ifdef DeeUnixSysFS_Utf8IsExecutable
#define DeeSysFS_Utf8IsExecutable DeeUnixSysFS_Utf8IsExecutable
#endif
#ifdef DeeUnixSysFS_Utf8IsCharDev
#define DeeSysFS_Utf8IsCharDev DeeUnixSysFS_Utf8IsCharDev
#endif
#ifdef DeeUnixSysFS_Utf8IsBlockDev
#define DeeSysFS_Utf8IsBlockDev DeeUnixSysFS_Utf8IsBlockDev
#endif
#ifdef DeeUnixSysFS_Utf8IsFiFo
#define DeeSysFS_Utf8IsFiFo DeeUnixSysFS_Utf8IsFiFo
#endif
#ifdef DeeUnixSysFS_Utf8IsSocket
#define DeeSysFS_Utf8IsSocket DeeUnixSysFS_Utf8IsSocket
#endif
#ifdef DeeUnixSysFS_Utf8Exists
#define DeeSysFS_Utf8Exists DeeUnixSysFS_Utf8Exists
#endif
#ifdef DeeUnixSysFS_Utf8Remove
#define DeeSysFS_Utf8Remove DeeUnixSysFS_Utf8Remove
#endif
#ifdef DeeUnixSysFS_Utf8Unlink
#define DeeSysFS_Utf8Unlink DeeUnixSysFS_Utf8Unlink
#endif
#ifdef DeeUnixSysFS_Utf8RmDir
#define DeeSysFS_Utf8RmDir DeeUnixSysFS_Utf8RmDir
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_H */
