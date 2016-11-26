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
#ifndef GUARD_DEEMON_SYS__UNIX_SYSINFO_H
#define GUARD_DEEMON_SYS__UNIX_SYSINFO_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/posix_features.inl>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h> /* gethostname, getlogin_r, getlogin */
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>  /* E*, errno  */
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>  /* L_cuserid */
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H */
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#if !DEE_HAVE_GETLOGIN_R && !DEE_HAVE_GETLOGIN
#include <deemon/sys/sysfs.h> /* DeeSysFS_Utf8GetEnv */
#endif

//////////////////////////////////////////////////////////////////////////
// === Unix ===

DEE_DECL_BEGIN

#ifdef ENAMETOOLONG
#define DEE_UNIXSYS_PRIVATE_ISSHORTBUFERROR(eno) ((eno) == ENAMETOOLONG || (eno) == EINVAL)
#else
#define DEE_UNIXSYS_PRIVATE_ISSHORTBUFERROR(eno) ((eno) == EINVAL)
#endif

#if DEE_HAVE_GETHOSTNAME
#define DeeUnixSys_Utf8GetHostname(result,...) \
do{\
 int _hn_error; Dee_size_t _hn_result_size = DEE_XCONFIG_FSBUFSIZE_POSIXGETHOSTNAME;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXGETHOSTNAME)) == NULL) return NULL;\
 while (1) {\
  if DEE_LIKELY(gethostname(DeeUtf8String_STR(*(result)),_hn_result_size) == 0) {\
   if DEE_LIKELY(DeeUtf8String_Resize(result,Dee_Utf8StrLen(DeeUtf8String_STR(*(result)))) == 0) break;\
  } else {\
   _hn_error = errno;\
   if (DEE_UNIXSYS_PRIVATE_ISSHORTBUFERROR(_hn_error)) {\
    if DEE_LIKELY(DeeUtf8String_Resize(result,(_hn_result_size *= 2)) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "gethostname() : %K",\
                        DeeSystemError_ToString(_hn_error));\
   }\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif /* DEE_HAVE_GETHOSTNAME */
#ifdef DeeUnixSys_Utf8GetHostname
#define DeeUnixSysInfo_Utf8GetHostname DeeUnixSys_Utf8GetHostname
#endif


#if DEE_HAVE_GETLOGIN_R
#define DeeUnixSys_Utf8GetUsername(result,...) \
do{\
 int _un_error; Dee_size_t _un_result_size = DEE_XCONFIG_FSBUFSIZE_POSIXGETLOGIN;\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_POSIXGETLOGIN)) == NULL) return NULL;\
 while (1) {\
  if DEE_LIKELY(getlogin_r(DeeUtf8String_STR(*(result)),_un_result_size) == 0) {\
   if DEE_LIKELY(DeeUtf8String_Resize(result,Dee_Utf8StrLen(DeeUtf8String_STR(*(result)))) == 0) break;\
  } else {\
   _un_error = errno;\
   if (DEE_UNIXSYS_PRIVATE_ISSHORTBUFERROR(_un_error)) {\
    if DEE_LIKELY(DeeUtf8String_Resize(result,(_un_result_size *= 2)) == 0) continue;\
   } else {\
    DeeError_SetStringf(&DeeErrorType_SystemError,\
                        "getlogin_r() : %K",\
                        DeeSystemError_ToString(_un_error));\
   }\
  }\
  Dee_DECREF(*(result));\
  {__VA_ARGS__;}\
 }\
}while(0)
#elif DEE_HAVE_GETLOGIN
#define DeeUnixSys_Utf8GetUsername(result,...) \
do{\
 char const *_un_result;\
 if DEE_UNLIKELY((_un_result = getlogin()) == NULL) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "getlogin() : %K",\
                      DeeSystemError_ToString(errno));\
  {__VA_ARGS__;}\
 }\
 if DEE_UNLIKELY((*(result) = DeeUtf8String_New(_un_result)) == NULL) {__VA_ARGS__;}\
}while(0)
#elif defined(DeeSysFS_Utf8GetEnv)
#define DeeUnixSys_Utf8GetUsername(result,...) \
do{\
 static Dee_Utf8Char const _un_envname[] = {'L','O','G','N','A','M','E',0};\
 if DEE_UNLIKELY((*(result) = DeeSysFS_Utf8GetEnv(_un_envname)) == NULL) {__VA_ARGS__;}\
}while(0)
#endif /* ... */

#ifdef DeeUnixSys_Utf8GetUsername
#define DeeUnixSysInfo_Utf8GetUsername DeeUnixSys_Utf8GetUsername
#endif


#ifdef DeeUnixSysInfo_Utf8GetHostname
#define DeeSysInfo_Utf8GetHostname DeeUnixSysInfo_Utf8GetHostname
#endif
#ifdef DeeUnixSysInfo_Utf8GetUsername
#define DeeSysInfo_Utf8GetUsername DeeUnixSysInfo_Utf8GetUsername
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__UNIX_SYSINFO_H */
