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
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8SetEnv(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8SetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_New(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_New(envvalue)) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(envvalue_ob); goto err_envname; })
 DeeSysFS_Utf8SetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8String(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8String(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 result = DeeNFS_WideSetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(
 DEE_A_IN_Z Dee_WideChar const *envname,
 DEE_A_IN_Z Dee_WideChar const *envvalue) {
 DEE_ASSERT(envname);
 DEE_ASSERT(envvalue);
#ifdef DeeSysFS_WideSetEnv
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideSetEnv(envname,envvalue,return -1);
 return 0;
#elif defined(DeeSysFS_WideSetEnvObject)
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_New(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_New(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(envname_ob); return -1; })
 DeeSysFS_WideSetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envname_ob); return -1; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#else
 DeeObject *envname_ob,*envvalue_ob; int result;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideString(envname)) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideString(envvalue)) == NULL) { Dee_DECREF(envname_ob); return -1; }
 result = DeeNFS_Utf8SetEnvObject(envname_ob,envvalue_ob);
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue));
#ifdef DeeSysFS_Utf8SetEnvObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8SetEnvObject(envname,envvalue,return -1);
 return 0;
#else
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envvalue),DeeUtf8String_STR(envvalue))) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(envvalue_ob); goto err_envname; })
 DeeSysFS_WideSetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envname,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
 DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue));
#ifdef DeeSysFS_WideSetEnvObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideSetEnvObject(envname,envvalue,return -1);
 return 0;
#else
 DeeObject *envname_ob,*envvalue_ob;
 if DEE_UNLIKELY((envname_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) return -1;
 if DEE_UNLIKELY((envvalue_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envvalue),DeeWideString_STR(envvalue))) == NULL) {err_envname: Dee_DECREF(envname_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(envvalue_ob); goto err_envname; })
 DeeSysFS_Utf8SetEnvObject(envname_ob,envvalue_ob,{ Dee_DECREF(envvalue_ob); goto err_envname; });
 Dee_DECREF(envvalue_ob);
 Dee_DECREF(envname_ob);
 return 0;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue) { DEE_ASSERT(envname); DEE_ASSERT(envvalue); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeUtf8String_Check(envvalue)); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); DEE_ASSERT(DeeObject_Check(envvalue) && DeeWideString_Check(envvalue)); (void)envname,envvalue; DeeError_Throw((DeeObject *)&_dee_notimplemented_setenv); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_SETENV_C_INL */
