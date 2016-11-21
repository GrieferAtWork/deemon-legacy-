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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_UNLINK_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_UNLINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_UNLINK
DeeError_NEW_STATIC(_dee_notimplemented_unlink,&DeeErrorType_NotImplemented,"unlink");
#endif

#ifdef DEE_NFS_HAVE_UNLINK
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8Unlink
 DeeSysFS_Utf8Unlink(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8UnlinkObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8UnlinkObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideUnlinkObject)\
   || defined(DeeSysFS_WideUnlink)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 DeeNFS_Utf8TryUnlink(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideUnlink
 DeeSysFS_WideUnlink(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideUnlinkObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideUnlinkObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8UnlinkObject)\
   || defined(DeeSysFS_Utf8Unlink)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8UnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 DeeNFS_WideTryUnlink(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8UnlinkObject
 DeeSysFS_Utf8UnlinkObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideUnlinkObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideUnlinkObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 DeeNFS_Utf8TryUnlinkObject(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideUnlinkObject
 DeeSysFS_WideUnlinkObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8UnlinkObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8UnlinkObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 DeeNFS_WideTryUnlinkObject(path);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryUnlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryUnlink
 return DeeSysFS_Utf8TryUnlink(path);
#elif defined(DeeSysFS_Utf8TryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8Unlink(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryUnlink(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryUnlink
 return DeeSysFS_WideTryUnlink(path);
#elif defined(DeeSysFS_WideTryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideUnlink(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryUnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryUnlinkObject
 return DeeSysFS_Utf8TryUnlinkObject(path);
#elif defined(DeeSysFS_WideTryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8UnlinkObject(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryUnlinkObject
 return DeeSysFS_WideTryUnlinkObject(path);
#elif defined(DeeSysFS_Utf8TryUnlinkObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return -1; }
 result = DeeSysFS_Utf8TryUnlinkObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideUnlinkObject(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_unlink); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_unlink); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_unlink); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_unlink); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryUnlink(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryUnlink(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryUnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_UNLINK_C_INL */
