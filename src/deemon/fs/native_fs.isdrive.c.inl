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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_ISDRIVE_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_ISDRIVE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/sys/sysfd.h>
#include <deemon/fs/native_filefd.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_ISDRIVE
DeeError_NEW_STATIC(_dee_notimplemented_isdrive,&DeeErrorType_NotImplemented,"isdrive");
#endif

#ifdef DEE_NFS_HAVE_ISDRIVE
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8IsDrive
 int result;
 DeeSysFS_Utf8IsDrive(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8IsDriveObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8IsDriveObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideIsDriveObject)\
   || defined(DeeSysFS_WideIsDrive)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsDrive(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_Utf8TryIsDrive(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideIsDrive
 int result;
 DeeSysFS_WideIsDrive(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideIsDriveObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8IsDriveObject)\
   || defined(DeeSysFS_Utf8IsDrive)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8IsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsDrive(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_WideTryIsDrive(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8IsDriveObject
 int result;
 DeeSysFS_Utf8IsDriveObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideIsDriveObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsDrive(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_Utf8TryIsDriveObject(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideIsDriveObject
 int result;
 DeeSysFS_WideIsDriveObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8IsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8IsDriveObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsDrive(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_WideTryIsDriveObject(path);
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryIsDrive
 return DeeSysFS_Utf8TryIsDrive(path);
#elif defined(DeeSysFS_Utf8TryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsDrive(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8IsDrive(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDrive(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryIsDrive
 return DeeSysFS_WideTryIsDrive(path);
#elif defined(DeeSysFS_WideTryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsDrive(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideIsDrive(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryIsDriveObject
 return DeeSysFS_Utf8TryIsDriveObject(path);
#elif defined(DeeSysFS_WideTryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsDrive(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8IsDriveObject(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryIsDriveObject
 return DeeSysFS_WideTryIsDriveObject(path);
#elif defined(DeeSysFS_Utf8TryIsDriveObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryIsDriveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsDrive)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsDrive(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideIsDriveObject(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_isdrive); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_isdrive); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_isdrive); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_isdrive); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDrive(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDrive(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_ISDRIVE_C_INL */
