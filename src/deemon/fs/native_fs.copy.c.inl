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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_COPY_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_COPY_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/native_filefd.h>


DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_COPY
DeeError_NEW_STATIC(_dee_notimplemented_copy,&DeeErrorType_NotImplemented,"copy");
#endif
#ifndef DEE_NFS_HAVE_OSCOPY
static DEE_A_RET_EXCEPT(-1) int DeeNativeFileFD_TransferFile(
 DEE_A_INOUT struct DeeNativeFileFD *src_fd,
 DEE_A_INOUT struct DeeNativeFileFD *dst_fd);
static DEE_A_RET_NOEXCEPT(0) int DeeNativeFileFD_TryTransferFile(
 DEE_A_INOUT struct DeeNativeFileFD *src_fd,
 DEE_A_INOUT struct DeeNativeFileFD *dst_fd);
#endif

#ifdef DEE_NFS_HAVE_COPY
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_Utf8Copy
 DeeSysFS_Utf8Copy(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8CopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DeeSysFS_Utf8CopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#elif defined(DeeSysFS_WideCopyObject)\
   || defined(DeeSysFS_WideCopy)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8String(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8String(dst)) == NULL) { result = -1; goto end_srcob; }
 result = DeeNFS_WideCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
end_srcob: Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DeeNativeFileFD_Utf8Init(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DeeNativeFileFD_Utf8Init(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 DeeNFS_Utf8TryCopy(src,dst);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_WideCopy
 DeeSysFS_WideCopy(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_WideCopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DeeSysFS_WideCopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#elif defined(DeeSysFS_Utf8CopyObject)\
   || defined(DeeSysFS_Utf8Copy)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideString(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { result = -1; goto end_srcob; }
 result = DeeNFS_Utf8CopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
end_srcob: Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DeeNativeFileFD_WideInit(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DeeNativeFileFD_WideInit(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 DeeNFS_WideTryCopy(src,dst);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst));
#ifdef DeeSysFS_Utf8CopyObject
 DeeSysFS_Utf8CopyObject(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_WideCopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(src),DeeUtf8String_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(dst),DeeUtf8String_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DeeSysFS_WideCopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DeeNativeFileFD_Utf8InitObject(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DeeNativeFileFD_Utf8InitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 DeeNFS_Utf8TryCopyObject(src,dst);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst));
#ifdef DeeSysFS_WideCopyObject
 DeeSysFS_WideCopyObject(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8CopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(src),DeeWideString_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(dst),DeeWideString_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DeeSysFS_Utf8CopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DeeNativeFileFD_WideInitObject(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DeeNativeFileFD_WideInitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 DeeNFS_WideTryCopyObject(src,dst);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryCopy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_Utf8TryCopy
 return DeeSysFS_Utf8TryCopy(src,dst);
#elif defined(DeeSysFS_Utf8TryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_New(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_New(dst)) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeSysFS_WideTryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8String(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8String(dst)) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_WideTryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&src_fd,src,DEE_OPENMODE('r',0),0)) return 0;
 if (!DeeNativeFileFD_Utf8TryInit(&dst_fd,dst,DEE_OPENMODE('c',0),0)) { result = 0; goto end_srcfd; }
 result = DeeNativeFileFD_TryTransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8Copy(src,dst)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_WideTryCopy
 return DeeSysFS_WideTryCopy(src,dst);
#elif defined(DeeSysFS_WideTryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeWideString_New(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeWideString_New(dst)) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_WideTryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideString(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&src_fd,src,DEE_OPENMODE('r',0),0)) return 0;
 if (!DeeNativeFileFD_WideTryInit(&dst_fd,dst,DEE_OPENMODE('c',0),0)) { result = 0; goto end_srcfd; }
 result = DeeNativeFileFD_TryTransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideCopy(src,dst)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryCopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst));
#ifdef DeeSysFS_Utf8TryCopyObject
 return DeeSysFS_Utf8TryCopyObject(src,dst);
#elif defined(DeeSysFS_WideTryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(src),DeeUtf8String_STR(src))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(dst),DeeUtf8String_STR(dst))) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_WideTryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&src_fd,src,DEE_OPENMODE('r',0),0)) return 0;
 if (!DeeNativeFileFD_Utf8TryInitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0)) { result = 0; goto end_srcfd; }
 result = DeeNativeFileFD_TryTransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8CopyObject(src,dst)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst));
#ifdef DeeSysFS_WideTryCopyObject
 return DeeSysFS_WideTryCopyObject(src,dst);
#elif defined(DeeSysFS_Utf8TryCopyObject)
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(src),DeeWideString_STR(src))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(dst),DeeWideString_STR(dst))) == NULL) { Dee_DECREF(src_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryCopyObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return result;
#elif defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&src_fd,src,DEE_OPENMODE('r',0),0)) return 0;
 if (!DeeNativeFileFD_WideTryInitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0)) { result = 0; goto end_srcfd; }
 result = DeeNativeFileFD_TryTransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideCopyObject(src,dst)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryCopy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryCopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst)); (void)src,dst; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst)); (void)src,dst; return 0; }
#endif

DEE_DECL_END

#ifndef DEE_NFS_HAVE_OSCOPY
#ifndef __INTELLISENSE__
#define DO_TRY
#include "native_fs.copy.emulated.impl.inl"
#include "native_fs.copy.emulated.impl.inl"
#endif
#endif

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_COPY_C_INL */
