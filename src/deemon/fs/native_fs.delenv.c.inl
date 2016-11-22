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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_DELENV_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_DELENV_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_DELENV
DeeError_NEW_STATIC(_dee_notimplemented_delenv,&DeeErrorType_NotImplemented,"delenv");
#endif

#ifdef DEE_NFS_HAVE_DELENV
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_Utf8DelEnv
 DeeSysFS_Utf8DelEnv(envname,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8DelEnvObject)
 DeeObject *envname_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_New(envname)) == NULL) return -1;
 DeeSysFS_Utf8DelEnvObject(envname_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envname_ob);
 return 0;
#elif defined(DeeSysFS_WideDelEnvObject)\
   || defined(DeeSysFS_WideDelEnv)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8String(envname)) == NULL) return -1;
 result = DeeNFS_WideDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 DeeNFS_Utf8TryDelEnv(envname);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_WideDelEnv
 DeeSysFS_WideDelEnv(envname,return -1);
 return 0;
#elif defined(DeeSysFS_WideDelEnvObject)
 DeeObject *envname_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_New(envname)) == NULL) return -1;
 DeeSysFS_WideDelEnvObject(envname_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envname_ob);
 return 0;
#elif defined(DeeSysFS_Utf8DelEnvObject)\
   || defined(DeeSysFS_Utf8DelEnv)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideString(envname)) == NULL) return -1;
 result = DeeNFS_Utf8DelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 DeeNFS_WideTryDelEnv(envname);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
#ifdef DeeSysFS_Utf8DelEnvObject
 DeeSysFS_Utf8DelEnvObject(envname,return -1);
 return 0;
#elif defined(DeeSysFS_WideDelEnvObject)
 DeeObject *envname_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) return -1;
 DeeSysFS_WideDelEnvObject(envname_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeNFS_Utf8TryDelEnvObject(envname);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
#ifdef DeeSysFS_WideDelEnvObject
 DeeSysFS_WideDelEnvObject(envname,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8DelEnvObject)
 DeeObject *envname_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) return -1;
 DeeSysFS_Utf8DelEnvObject(envname_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeNFS_WideTryDelEnvObject(envname);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_Utf8TryDelEnv
 return DeeSysFS_Utf8TryDelEnv(envname);
#elif defined(DeeSysFS_Utf8TryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_New(envname)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#elif defined(DeeSysFS_WideTryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8String(envname)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8DelEnv(envname)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_WideTryDelEnv
 return DeeSysFS_WideTryDelEnv(envname);
#elif defined(DeeSysFS_WideTryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_New(envname)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideString(envname)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideDelEnv(envname)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
#ifdef DeeSysFS_Utf8TryDelEnvObject
 return DeeSysFS_Utf8TryDelEnvObject(envname);
#elif defined(DeeSysFS_WideTryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8DelEnvObject(envname)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
#ifdef DeeSysFS_WideTryDelEnvObject
 return DeeSysFS_WideTryDelEnvObject(envname);
#elif defined(DeeSysFS_Utf8TryDelEnvObject)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryDelEnvObject(envname_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideDelEnvObject(envname)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) { DEE_ASSERT(envname); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_delenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *envname) { DEE_ASSERT(envname); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_delenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_delenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_delenv); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) { DEE_ASSERT(envname); (void)envname; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnv(DEE_A_IN_Z Dee_WideChar const *envname) { DEE_ASSERT(envname); (void)envname; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); (void)envname; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); (void)envname; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_DELENV_C_INL */
