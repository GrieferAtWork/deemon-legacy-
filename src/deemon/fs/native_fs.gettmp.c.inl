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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_GETTMP_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_GETTMP_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_GETTMP
DeeError_NEW_STATIC(_dee_notimplemented_gettmp,&DeeErrorType_NotImplemented,"gettmp");
#endif
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetTmp(void) {
#ifdef DEE_NFS_HAVE_GETENV
 static Dee_Utf8Char const name_TMPDIR[] = {'T','M','P','D','I','R',0};
 static Dee_Utf8Char const name_TEMP[] = {'T','E','M','P',0};
 static Dee_Utf8Char const name_TMP[] = {'T','M','P',0};
 DeeObject *result;
 if ((result = DeeNFS_Utf8TryGetEnv(name_TMPDIR)) != NULL) return result;
 if ((result = DeeNFS_Utf8TryGetEnv(name_TEMP)) != NULL) return result;
 if ((result = DeeNFS_Utf8TryGetEnv(name_TMP)) != NULL) return result;
#endif /* DEE_NFS_HAVE_GETENV */
 {
#ifdef DeeSysFS_Utf8GetTmp
  return DeeSysFS_Utf8GetTmp();
#elif defined(DeeSysFS_WideGetTmp)
  DeeObject *newresult;
  if DEE_UNLIKELY((result = DeeSysFS_WideGetTmp()) == NULL) return NULL;
  newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                     DeeWideString_STR(result));
  Dee_DECREF(result);
  return newresult;
#else
  DeeError_Throw((DeeObject *)&_dee_notimplemented_gettmp);
  return NULL;
#endif
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetTmp(void) {
#ifdef DEE_NFS_HAVE_GETENV
 static Dee_WideChar const name_TMPDIR[] = {'T','M','P','D','I','R',0};
 static Dee_WideChar const name_TEMP[] = {'T','E','M','P',0};
 static Dee_WideChar const name_TMP[] = {'T','M','P',0};
 DeeObject *result;
 if ((result = DeeNFS_WideTryGetEnv(name_TMPDIR)) != NULL) return result;
 if ((result = DeeNFS_WideTryGetEnv(name_TEMP)) != NULL) return result;
 if ((result = DeeNFS_WideTryGetEnv(name_TMP)) != NULL) return result;
#endif /* DEE_NFS_HAVE_GETENV */
 {
#ifdef DeeSysFS_WideGetTmp
  return DeeSysFS_WideGetTmp();
#elif defined(DeeSysFS_Utf8GetTmp)
  DeeObject *newresult;
  if DEE_UNLIKELY((result = DeeSysFS_Utf8GetTmp()) == NULL) return NULL;
  newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                     DeeUtf8String_STR(result));
  Dee_DECREF(result);
  return newresult;
#else
  DeeError_Throw((DeeObject *)&_dee_notimplemented_gettmp);
  return NULL;
#endif
 }
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_GETTMP_C_INL */
