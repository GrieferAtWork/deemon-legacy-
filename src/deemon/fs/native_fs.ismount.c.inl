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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_ISMOUNT_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_ISMOUNT_C_INL 1
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

#ifndef DEE_NFS_HAVE_ISMOUNT
DeeError_NEW_STATIC(_dee_notimplemented_ismount,&DeeErrorType_NotImplemented,"ismount");
#endif

#ifdef DEE_NFS_HAVE_ISMOUNT
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8IsMount
 int result;
 DeeSysFS_Utf8IsMount(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8IsMountObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8IsMountObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideIsMountObject)\
   || defined(DeeSysFS_WideIsMount)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsMount(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 DeeNFS_Utf8TryIsMount(path,&result);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideIsMount
 int result;
 DeeSysFS_WideIsMount(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideIsMountObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8IsMountObject)\
   || defined(DeeSysFS_Utf8IsMount)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8IsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsMount(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 DeeNFS_WideTryIsMount(path,&result);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8IsMountObject
 int result;
 DeeSysFS_Utf8IsMountObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideIsMountObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsMount(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 DeeNFS_Utf8TryIsMountObject(path,&result);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideIsMountObject
 int result;
 DeeSysFS_WideIsMountObject(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8IsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8IsMountObject(path_ob,&result,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_IsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_IsMount(&fd,&result,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 DeeNFS_WideTryIsMountObject(path,&result);
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMount(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryIsMount
 return DeeSysFS_Utf8TryIsMount(path);
#elif defined(DeeSysFS_Utf8TryIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryIsMountObject)\
   || defined(DeeSysFS_WideTryIsMount)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeNFS_WideTryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsMount(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8IsMount(path,&result)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMount(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryIsMount
 return DeeSysFS_WideTryIsMount(path);
#elif defined(DeeSysFS_WideTryIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryIsMountObject)\
   || defined(DeeSysFS_Utf8TryIsMount)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeNFS_Utf8TryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsMount(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideIsMount(path,&result)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryIsMountObject
 return DeeSysFS_Utf8TryIsMountObject(path);
#elif defined(DeeSysFS_WideTryIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsMount(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8IsMountObject(path,&result)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryIsMountObject
 return DeeSysFS_WideTryIsMountObject(path);
#elif defined(DeeSysFS_Utf8TryIsMountObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return -1; }
 result = DeeSysFS_Utf8TryIsMountObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryIsMount)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryIsMount(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideIsMountObject(path,&result)) < 0) { DeeError_HandledOne(); result = 0; }
 return result;
#endif
}
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_ismount); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_ismount); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_ismount); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_ismount); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMount(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMount(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_ISMOUNT_C_INL */