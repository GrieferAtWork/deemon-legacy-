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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_SETENV_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_SETENV_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_SETENV
DeeError_NEW_STATIC(_dee_notimplemented_setenv,&DeeErrorType_NotImplemented,"setenv");
#endif

#ifdef DEE_NFS_HAVE_SETENV
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnv(
 DEE_A_IN_Z Dee_Utf8Char const *envname,
 DEE_A_IN_Z Dee_Utf8Char const *envvalue) {
 DEE_ASSERT(envname);
 DEE_ASSERT(envvalue);
#ifdef DeeSysFS_Utf8SetEnv
 DeeSysFS_Utf8SetEnv(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8SetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_New(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_New(envvalue)) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DeeSysFS_Utf8SetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#elif defined(DeeSysFS_WideSetEnvObject)\
   || defined(DeeSysFS_WideSetEnv)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8String(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8String(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 result = DeeNFS_WideSetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 DeeNFS_Utf8TrySetEnv(envname);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(
 DEE_A_IN_Z Dee_WideChar const *envname,
 DEE_A_IN_Z Dee_WideChar const *envvalue) {
 DEE_ASSERT(envname);
 DEE_ASSERT(envvalue);
#ifdef DeeSysFS_WideSetEnv
 DeeSysFS_WideSetEnv(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_WideSetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_New(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_New(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 DeeSysFS_WideSetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#elif defined(DeeSysFS_Utf8SetEnvObject)\
   || defined(DeeSysFS_Utf8SetEnv)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideString(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideString(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 result = DeeNFS_Utf8SetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 DeeNFS_WideTrySetEnv(envname,envnvalue);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue));
#ifdef DeeSysFS_Utf8SetEnvObject
 DeeSysFS_Utf8SetEnvObject(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_WideSetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envvalue),DeeUtf8String_STR(envvalue))) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DeeSysFS_WideSetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeNFS_Utf8TrySetEnvObject(envname,envvalue);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envname,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue));
#ifdef DeeSysFS_WideSetEnvObject
 DeeSysFS_WideSetEnvObject(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8SetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envvalue),DeeWideString_STR(envvalue))) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DeeSysFS_Utf8SetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeNFS_WideTrySetEnvObject(envname,envvalue);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnv(
 DEE_A_IN_Z Dee_Utf8Char const *envname,
 DEE_A_IN_Z Dee_Utf8Char const *envvalue) {
 DEE_ASSERT(envname);
 DEE_ASSERT(envvalue);
#ifdef DeeSysFS_Utf8TrySetEnv
 return DeeSysFS_Utf8TrySetEnv(envname,envvalue);
#elif defined(DeeSysFS_Utf8TrySetEnvObject)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_New(envname)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_New(envvalue)) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeSysFS_Utf8TrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#elif defined(DeeSysFS_WideTrySetEnvObject)\
   || defined(DeeSysFS_WideTrySetEnv)
 DeeObject *envname_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8String(envname)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8String(envvalue)) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeNFS_WideTrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if DEE_UNLIKELY((result = DeeNFS_Utf8SetEnv(envname,envvalue)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnv(
 DEE_A_IN_Z Dee_WideChar const *envname,
 DEE_A_IN_Z Dee_WideChar const *envvalue) {
 DEE_ASSERT(envname);
 DEE_ASSERT(envvalue);
#ifdef DeeSysFS_WideTrySetEnv
 return DeeSysFS_WideTrySetEnv(envname,envvalue);
#elif defined(DeeSysFS_WideTrySetEnvObject)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_New(envname)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_New(envvalue)) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeSysFS_WideTrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#elif defined(DeeSysFS_Utf8TrySetEnvObject)\
   || defined(DeeSysFS_Utf8TrySetEnv)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideString(envname)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideString(envvalue)) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeNFS_Utf8TrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if DEE_UNLIKELY((result = DeeNFS_WideSetEnv(envname,envvalue)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnvObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue));
#ifdef DeeSysFS_Utf8TrySetEnvObject
 return DeeSysFS_Utf8TrySetEnvObject(envname,envvalue);
#elif defined(DeeSysFS_WideTrySetEnvObject)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envvalue),DeeUtf8String_STR(envvalue))) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeSysFS_WideTrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if DEE_UNLIKELY((result = DeeNFS_Utf8SetEnvObject(envname,envvalue)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnvObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envname,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue));
#ifdef DeeSysFS_WideTrySetEnvObject
 return DeeSysFS_WideTrySetEnvObject(envname,envvalue);
#elif defined(DeeSysFS_Utf8TrySetEnvObject)
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envvalue),DeeWideString_STR(envvalue))) == NULL) { Dee_DECREF(envname_ob); goto err_h1; }
 result = DeeSysFS_Utf8TrySetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#else
 int result;
 if DEE_UNLIKELY((result = DeeNFS_WideSetEnvObject(envname,envvalue)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue)); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue)); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue)); (void)envname,envvalue; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue)); (void)envname,envvalue; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_SETENV_C_INL */
