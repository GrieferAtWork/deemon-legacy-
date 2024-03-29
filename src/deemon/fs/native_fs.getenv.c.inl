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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_GETENV_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_GETENV_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_GETENV
DeeError_NEW_STATIC(_dee_notimplemented_getenv,&DeeErrorType_NotImplemented,"getenv");
#endif
#ifdef DEE_NFS_HAVE_GETENV
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
#ifdef DeeSysFS_Utf8GetEnv
 DeeObject *result;
 DEE_ASSERT(envname);
 DEE_NFS_CHECKINTERRUPT(return NULL)
 DeeSysFS_Utf8GetEnv(envname,&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 return result;
#elif defined(DeeSysFS_Utf8GetEnvObject)
 DeeObject *newenvname,*result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeUtf8String_New(envname)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ result = NULL; goto end; })
 DeeSysFS_Utf8GetEnvObject(newenvname,&result,{ result = NULL; goto end; });
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
end: Dee_DECREF(newenvname);
 return result;
#elif defined(DeeSysFS_WideGetEnvObject)\
   || defined(DeeSysFS_WideGetEnv)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8String(envname)) == NULL) return NULL;
 result = DeeNFS_WideGetEnvObject(newenvname);
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(envname);
 DEE_NFS_CHECKINTERRUPT(return NULL)
 if ((result = DeeNFS_Utf8TryGetEnv(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Unknown env variable: %q",envname);
 }
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
#ifdef DeeSysFS_WideGetEnv
 DeeObject *result;
 DEE_ASSERT(envname);
 DEE_NFS_CHECKINTERRUPT(return NULL)
 DeeSysFS_WideGetEnv(envname,&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 return result;
#elif defined(DeeSysFS_WideGetEnvObject)
 DeeObject *newenvname,*result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeWideString_New(envname)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ result = NULL; goto end; })
 DeeSysFS_WideGetEnvObject(newenvname,&result,{ result = NULL; goto end; });
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
end: Dee_DECREF(newenvname);
 return result;
#elif defined(DeeSysFS_Utf8GetEnvObject)\
   || defined(DeeSysFS_Utf8GetEnv)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideString(envname)) == NULL) return NULL;
 result = DeeNFS_Utf8GetEnvObject(newenvname);
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(envname);
 DEE_NFS_CHECKINTERRUPT(return NULL)
 if ((result = DeeNFS_WideTryGetEnv(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Unknown env variable: %lq",envname);
 }
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
#ifdef DeeSysFS_Utf8GetEnvObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DEE_NFS_CHECKINTERRUPT(return NULL)
 DeeSysFS_Utf8GetEnvObject(envname,&result,return NULL);
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 return result;
#elif defined(DeeSysFS_WideGetEnvObject)
 DeeObject *newenvname,*result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return NULL; })
 DeeSysFS_WideGetEnvObject(newenvname,&result,{ Dee_DECREF(newenvname); return NULL; });
 DEE_ASSERT(DeeObject_Check(result) && DeeWideString_Check(result));
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DEE_NFS_CHECKINTERRUPT(return NULL)
 if ((result = DeeNFS_Utf8TryGetEnvObject(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Unknown env variable: %r",envname);
 }
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
#ifdef DeeSysFS_WideGetEnvObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DEE_NFS_CHECKINTERRUPT(return NULL)
 DeeSysFS_WideGetEnvObject(envname,&result,return NULL);
 return result;
#elif defined(DeeSysFS_Utf8GetEnvObject)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return NULL; })
 DeeSysFS_Utf8GetEnvObject(newenvname,&result,{ Dee_DECREF(newenvname); return NULL; });
 DEE_ASSERT(DeeObject_Check(result) && DeeUtf8String_Check(result));
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DEE_NFS_CHECKINTERRUPT(return NULL)
 if ((result = DeeNFS_WideTryGetEnvObject(envname)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Unknown env variable: %r",envname);
 }
 return result;
#endif
}

DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
#ifdef DeeSysFS_Utf8TryGetEnv
 DeeObject *result;
 DEE_ASSERT(envname);
 DeeSysFS_Utf8TryGetEnv(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeUtf8String_Check(result)));
 return result;
#elif defined(DeeSysFS_Utf8TryGetEnvObject)
 DeeObject *newenvname,*result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeUtf8String_New(
  envname)) == NULL) { DeeError_HandledOne(); return NULL; }
 DeeSysFS_Utf8TryGetEnvObject(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeUtf8String_Check(result)));
 Dee_DECREF(newenvname);
 return result;
#elif defined(DeeSysFS_WideTryGetEnvObject)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8String(
  envname)) == NULL) {err_h1: DeeError_HandledOne(); return NULL; }
 DeeSysFS_WideTryGetEnvObject(newenvname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeWideString_Check(result)));
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 if DEE_UNLIKELY(!newresult) goto err_h1;
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((result = DeeNFS_Utf8GetEnv(envname)) == NULL) DeeError_HandledOne();
 return result;
#endif
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
#ifdef DeeSysFS_WideTryGetEnv
 DeeObject *result;
 DEE_ASSERT(envname);
 DeeSysFS_WideTryGetEnv(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeWideString_Check(result)));
 return result;
#elif defined(DeeSysFS_WideTryGetEnvObject)
 DeeObject *newenvname,*result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeWideString_New(
  envname)) == NULL) { DeeError_HandledOne(); return NULL; }
 DeeSysFS_WideTryGetEnvObject(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeWideString_Check(result)));
 Dee_DECREF(newenvname);
 return result;
#elif defined(DeeSysFS_Utf8TryGetEnvObject)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideString(
  envname)) == NULL) {err_h1: DeeError_HandledOne(); return NULL; }
 DeeSysFS_Utf8TryGetEnvObject(newenvname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeUtf8String_Check(result)));
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 if DEE_UNLIKELY(!newresult) goto err_h1;
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(envname);
 if DEE_UNLIKELY((result = DeeNFS_WideGetEnv(envname)) == NULL) DeeError_HandledOne();
 return result;
#endif
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8TryGetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
#ifdef DeeSysFS_Utf8TryGetEnvObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DeeSysFS_Utf8TryGetEnvObject(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeUtf8String_Check(result)));
 return result;
#elif defined(DeeSysFS_WideTryGetEnvObject)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(
  envname),DeeUtf8String_STR(envname))) == NULL) {err_h1: DeeError_HandledOne(); return NULL; }
 DeeSysFS_WideTryGetEnvObject(newenvname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeWideString_Check(result)));
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 if DEE_UNLIKELY(!newresult) goto err_h1;
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 if DEE_UNLIKELY((result = DeeNFS_Utf8GetEnvObject(envname)) == NULL) DeeError_HandledOne();
 return result;
#endif
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideTryGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
#ifdef DeeSysFS_WideTryGetEnvObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DeeSysFS_WideTryGetEnvObject(envname,&result);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeWideString_Check(result)));
 return result;
#elif defined(DeeSysFS_Utf8TryGetEnvObject)
 DeeObject *newenvname,*result,*newresult;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(
  envname),DeeWideString_STR(envname))) == NULL) {err_h1: DeeError_HandledOne(); return NULL; }
 DeeSysFS_Utf8TryGetEnvObject(newenvname,&result);
 Dee_DECREF(newenvname);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 if DEE_UNLIKELY(!newresult) goto err_h1;
 return newresult;
#else
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 if DEE_UNLIKELY((result = DeeNFS_WideGetEnvObject(envname)) == NULL) DeeError_HandledOne();
 return result;
#endif
}
#else
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 DEE_ASSERT(envname); (void)envname;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getenv);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 DEE_ASSERT(envname); (void)envname;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getenv);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); (void)envname;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getenv);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); (void)envname;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getenv);
 return NULL;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 DEE_ASSERT(envname); (void)envname;
 return NULL;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 DEE_ASSERT(envname); (void)envname;
 return NULL;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8TryGetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); (void)envname;
 return NULL;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideTryGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); (void)envname;
 return NULL;
}
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_GETENV_C_INL */
