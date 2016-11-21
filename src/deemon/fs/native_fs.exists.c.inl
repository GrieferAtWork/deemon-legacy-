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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_EXISTS_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_EXISTS_C_INL 1
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

#ifndef DEE_NFS_HAVE_EXISTS
DeeError_NEW_STATIC(_dee_notimplemented_exists,&DeeErrorType_NotImplemented,"exists");
#endif

#ifdef DEE_NFS_HAVE_EXISTS
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8Exists
 int result;
 DeeSysFS_Utf8Exists(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8ExistsObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8ExistsObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideExistsObject)\
   || defined(DeeSysFS_WideExists)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 return DeeNFS_Utf8TryExists(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideExists
 int result;
 DeeSysFS_WideExists(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideExistsObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8ExistsObject)\
   || defined(DeeSysFS_Utf8Exists)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8ExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 return DeeNFS_WideTryExists(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8ExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8ExistsObject
 int result;
 DeeSysFS_Utf8ExistsObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideExistsObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 return DeeNFS_Utf8TryExistsObject(path);
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideExistsObject
 int result;
 DeeSysFS_WideExistsObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8ExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8ExistsObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 return DeeNFS_WideTryExistsObject(path);
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryExists(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryExists
 return DeeSysFS_Utf8TryExists(path);
#elif defined(DeeSysFS_Utf8TryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 int result;
 if ((result = DeeNFS_Utf8Exists(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryExists(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryExists
 return DeeSysFS_WideTryExists(path);
#elif defined(DeeSysFS_WideTryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 int result;
 if ((result = DeeNFS_WideExists(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryExistsObject
 return DeeSysFS_Utf8TryExistsObject(path);
#elif defined(DeeSysFS_WideTryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 int result;
 if ((result = DeeNFS_Utf8ExistsObject(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryExistsObject
 return DeeSysFS_WideTryExistsObject(path);
#elif defined(DeeSysFS_Utf8TryExistsObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return -1; }
 result = DeeSysFS_Utf8TryExistsObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeNativeFileFD)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeNativeFileFD_Quit(&fd);
 return 1;
#else
 int result;
 if ((result = DeeNFS_WideExistsObject(path)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_exists); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_exists); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8ExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_exists); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_exists); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryExists(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryExists(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_EXISTS_C_INL */
