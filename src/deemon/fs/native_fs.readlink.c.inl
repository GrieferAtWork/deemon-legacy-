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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_READLINK_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_READLINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_READLINK
DeeError_NEW_STATIC(_dee_notimplemented_readlink,&DeeErrorType_NotImplemented,"readlink");
#endif

#ifdef DEE_NFS_HAVE_READLINK
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8Readlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
#ifdef DeeSysFS_Utf8Readlink
 DeeObject *result; DEE_ASSERT(path);
 DeeSysFS_Utf8Readlink(path,&result,return NULL);
 return result;
#elif defined(DeeSysFS_Utf8ReadlinkObject)
 DeeObject *result,*path_ob; DEE_ASSERT(path);
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return NULL;
 DeeSysFS_Utf8ReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#else
 DeeObject *result,*path_ob; DEE_ASSERT(path);
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return NULL;
 DeeSysFS_WideReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideReadlink(DEE_A_IN_Z Dee_WideChar const *path) {
#ifdef DeeSysFS_WideReadlink
 DeeObject *result; DEE_ASSERT(path);
 DeeSysFS_WideReadlink(path,&result,return NULL);
 return result;
#elif defined(DeeSysFS_WideReadlinkObject)
 DeeObject *result,*path_ob; DEE_ASSERT(path);
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return NULL;
 DeeSysFS_WideReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#else
 DeeObject *result,*path_ob; DEE_ASSERT(path);
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 DeeSysFS_Utf8ReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeNFS_Utf8ReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
#ifdef DeeSysFS_Utf8ReadlinkObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 DeeSysFS_Utf8ReadlinkObject(path,&result,return NULL);
 return result;
#else
 DeeObject *result,*path_ob;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return NULL;
 DeeSysFS_WideReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeNFS_WideReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
#ifdef DeeSysFS_WideReadlinkObject
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 DeeSysFS_WideReadlinkObject(path,&result,return NULL);
 return result;
#else
 DeeObject *result,*path_ob;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 DeeSysFS_Utf8ReadlinkObject(path_ob,&result,{ Dee_DECREF(path_ob); return NULL; });
 Dee_DECREF(path_ob);
 return result;
#endif
}
#else
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8Readlink(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_readlink); return NULL; }
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideReadlink(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_readlink); return NULL; }
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8ReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_readlink); return NULL; }
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_readlink); return NULL; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_READLINK_C_INL */
