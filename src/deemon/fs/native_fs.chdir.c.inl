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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_CHDIR_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_CHDIR_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_CHDIR
DeeError_NEW_STATIC(_dee_notimplemented_chdir,&DeeErrorType_NotImplemented,"chdir");
#endif

#ifdef DEE_NFS_HAVE_CHDIR
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8Chdir
 DeeSysFS_Utf8Chdir(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8ChdirObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8ChdirObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideChdirObject)\
   || defined(DeeSysFS_WideChdir)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 DeeNFS_Utf8TryChdir(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideChdir
 DeeSysFS_WideChdir(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideChdirObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideChdirObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8ChdirObject)\
   || defined(DeeSysFS_Utf8Chdir)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8ChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 DeeNFS_WideTryChdir(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8ChdirObject
 DeeSysFS_Utf8ChdirObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideChdirObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideChdirObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 DeeNFS_Utf8TryChdirObject(path);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideChdirObject
 DeeSysFS_WideChdirObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8ChdirObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8ChdirObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 DeeNFS_WideTryChdirObject(path);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryChdir
 return DeeSysFS_Utf8TryChdir(path);
#elif defined(DeeSysFS_Utf8TryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8Chdir(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryChdir
 return DeeSysFS_WideTryChdir(path);
#elif defined(DeeSysFS_WideTryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideChdir(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryChdirObject
 return DeeSysFS_Utf8TryChdirObject(path);
#elif defined(DeeSysFS_WideTryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8ChdirObject(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryChdirObject
 return DeeSysFS_WideTryChdirObject(path);
#elif defined(DeeSysFS_Utf8TryChdirObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryChdirObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideChdirObject(path)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_chdir); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_chdir); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_chdir); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_chdir); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_CHDIR_C_INL */
