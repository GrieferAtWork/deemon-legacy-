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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_GETUSERHOME_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_GETUSERHOME_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_GETUSERHOME
DeeError_NEW_STATIC(_dee_notimplemented_getuserhome,&DeeErrorType_NotImplemented,"getuserhome");
#endif
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetUserHome(DEE_A_IN_Z Dee_Utf8Char const *username) {
 DEE_ASSERT(username);
#ifdef DeeSysFS_Utf8GetUserHome
 return DeeSysFS_Utf8GetUserHome(username);
#elif defined(DeeSysFS_Utf8GetUserHomeObject)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeUtf8String_New(username)) == NULL) return NULL;
 result = DeeSysFS_Utf8GetUserHomeObject(username);
 Dee_DECREF(username_ob);
 return result;
#elif defined(DeeSysFS_WideGetUserHomeObject)\
   || defined(DeeSysFS_WideGetUserHome)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeWideString_FromUtf8String(username)) == NULL) return NULL;
 result = DeeNFS_WideGetUserHomeObject(username_ob);
 Dee_DECREF(username_ob);
 return result;
#else
 (void)username;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getuserhome);
 return NULL;
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetUserHome(DEE_A_IN_Z Dee_WideChar const *username) {
 DEE_ASSERT(username);
#ifdef DeeSysFS_WideGetUserHome
 return DeeSysFS_WideGetUserHome(username);
#elif defined(DeeSysFS_WideGetUserHomeObject)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeWideString_New(username)) == NULL) return NULL;
 result = DeeSysFS_WideGetUserHomeObject(username);
 Dee_DECREF(username_ob);
 return result;
#elif defined(DeeSysFS_Utf8GetUserHomeObject)\
   || defined(DeeSysFS_Utf8GetUserHome)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeUtf8String_FromWideString(username)) == NULL) return NULL;
 result = DeeNFS_Utf8GetUserHomeObject(username_ob);
 Dee_DECREF(username_ob);
 return result;
#else
 (void)username;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getuserhome);
 return NULL;
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8GetUserHomeObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *username) {
 DEE_ASSERT(DeeObject_Check(username) && DeeUtf8String_Check(username));
#ifdef DeeSysFS_Utf8GetUserHomeObject
 return DeeSysFS_Utf8GetUserHomeObject(username);
#elif defined(DeeSysFS_WideGetUserHomeObject)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(username),DeeUtf8String_STR(username))) == NULL) return NULL;
 result = DeeSysFS_WideGetUserHomeObject(username_ob);
 Dee_DECREF(username_ob);
 return result;
#else
 (void)username;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getuserhome);
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideGetUserHomeObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *username) {
 DEE_ASSERT(DeeObject_Check(username) && DeeWideString_Check(username));
#ifdef DeeSysFS_WideGetUserHomeObject
 return DeeSysFS_WideGetUserHomeObject(username);
#elif defined(DeeSysFS_Utf8GetUserHomeObject)
 DeeObject *username_ob,*result;
 if DEE_UNLIKELY((username_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(username),DeeWideString_STR(username))) == NULL) return NULL;
 result = DeeSysFS_Utf8GetUserHomeObject(username_ob);
 Dee_DECREF(username_ob);
 return result;
#else
 (void)username;
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getuserhome);
 return NULL;
#endif
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_GETUSERHOME_C_INL */
