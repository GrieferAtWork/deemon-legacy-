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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_GETTIMES_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_GETTIMES_C_INL 1
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

#ifndef DEE_NFS_HAVE_GETTIMES
DeeError_NEW_STATIC(_dee_notimplemented_gettimes,&DeeErrorType_NotImplemented,"gettimes");
#endif

#ifdef DEE_NFS_HAVE_GETTIMES
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimes(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8GetTimes
 DeeSysFS_Utf8GetTimes(path,atime,ctime,mtime,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8GetTimesObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8GetTimesObject(path_ob,atime,ctime,mtime,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideGetTimesObject)\
   || defined(DeeSysFS_WideGetTimes)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_GetTimes)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_GetTimes(&fd,atime,ctime,mtime,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#else
 DeeNFS_Utf8TryGetTimes(path,atime,ctime,mtime);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimes(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideGetTimes
 DeeSysFS_WideGetTimes(path,atime,ctime,mtime,return -1);
 return 0;
#elif defined(DeeSysFS_WideGetTimesObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_WideGetTimesObject(path_ob,atime,ctime,mtime,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8GetTimesObject)\
   || defined(DeeSysFS_Utf8GetTimes)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8GetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_GetTimes)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_GetTimes(&fd,atime,ctime,mtime,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#else
 DeeNFS_WideTryGetTimes(path,atime,ctime,mtime);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimesObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8GetTimesObject
 DeeSysFS_Utf8GetTimesObject(path,atime,ctime,mtime,return -1);
 return 0;
#elif defined(DeeSysFS_WideGetTimesObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_WideGetTimesObject(path_ob,atime,ctime,mtime,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFileFD_GetTimes)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_GetTimes(&fd,atime,ctime,mtime,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#else
 DeeNFS_Utf8TryGetTimesObject(path,atime,ctime,mtime);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimesObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideGetTimesObject
 DeeSysFS_WideGetTimesObject(path,atime,ctime,mtime,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8GetTimesObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8GetTimesObject(path_ob,atime,ctime,mtime,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFileFD_GetTimes)
 struct DeeNativeFileFD fd;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_GetTimes(&fd,atime,ctime,mtime,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#else
 DeeNFS_WideTryGetTimesObject(path,atime,ctime,mtime);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimes(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8TryGetTimes
 return DeeSysFS_Utf8TryGetTimes(path,atime,ctime,mtime);
#elif defined(DeeSysFS_Utf8TryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryGetTimes)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryGetTimes(&fd,atime,ctime,mtime);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8GetTimes(path,atime,ctime,mtime)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimes(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTryGetTimes
 return DeeSysFS_WideTryGetTimes(path,atime,ctime,mtime);
#elif defined(DeeSysFS_WideTryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryGetTimes)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryGetTimes(&fd,atime,ctime,mtime);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideGetTimes(path,atime,ctime,mtime)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimesObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8TryGetTimesObject
 return DeeSysFS_Utf8TryGetTimesObject(path,atime,ctime,mtime);
#elif defined(DeeSysFS_WideTryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_WideTryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryGetTimes)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryGetTimes(&fd,atime,ctime,mtime);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8GetTimesObject(path,atime,ctime,mtime)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimesObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTryGetTimesObject
 return DeeSysFS_WideTryGetTimesObject(path,atime,ctime,mtime);
#elif defined(DeeSysFS_Utf8TryGetTimesObject)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFS_Utf8TryGetTimesObject(path_ob,atime,ctime,mtime);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_TryGetTimes)
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_TryGetTimes(&fd,atime,ctime,mtime);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideGetTimesObject(path,atime,ctime,mtime)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_gettimes); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_gettimes); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_gettimes); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_gettimes); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(path); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(atime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(ctime), DEE_A_OUT_OPT Dee_timetick_t *DEE_UNUSED(mtime)) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_GETTIMES_C_INL */
