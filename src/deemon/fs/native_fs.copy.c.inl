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

#if !defined(DEE_NFS_HAVE_OSCOPY) && defined(DEE_NFS_HAVE_COPY)
#include DEE_INCLUDE_MEMORY_API()
#endif /* ... */

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_COPY
DeeError_NEW_STATIC(_dee_notimplemented_copy,&DeeErrorType_NotImplemented,"copy");
#endif
#if !defined(DEE_NFS_HAVE_OSCOPY) && defined(DeeNativeFileFD)\
  && defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)
static DEE_A_RET_EXCEPT(-1) int DeeNativeFileFD_TransferFile(
 DEE_A_INOUT struct DeeNativeFileFD *src_fd,
 DEE_A_INOUT struct DeeNativeFileFD *dst_fd) {
 Dee_size_t read_size,written_size;
 Dee_uint8_t *write_pos;
#ifdef DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY_ALLOCHEAP
#define RETURN(x) do{free_nn(buffer);return(x);}while(0)
 Dee_uint8_t *buffer;
 while DEE_UNLIKELY((buffer = (Dee_uint8_t *)malloc_nz(
  DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY*sizeof(Dee_uint8_t))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
#else
#define RETURN(x) return (x)
 Dee_uint8_t buffer[DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY];
#endif
 while (1) {
  DEE_NFS_CHECKINTERRUPT(RETURN(-1))
  DeeNativeFileFD_Read(src_fd,buffer,DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY,&read_size,RETURN(-1));
  if (!read_size) break;
  write_pos = buffer; while (1) {
   DEE_NFS_CHECKINTERRUPT(RETURN(-1))
   DeeNativeFileFD_Write(dst_fd,write_pos,read_size,&written_size,RETURN(-1));
   if (!written_size) RETURN(0);
   if (written_size >= read_size) break;
   read_size -= written_size;
   write_pos += written_size;
  }
 }
 RETURN(0);
#undef RETURN

}
#endif


#ifdef DEE_NFS_HAVE_COPY
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_Utf8Copy
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8Copy(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8CopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
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
#else
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Utf8Init(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ result = -1; goto end_srcfd; })
 DeeNativeFileFD_Utf8Init(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_WideCopy
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideCopy(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_WideCopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
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
#else
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_WideInit(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ result = -1; goto end_srcfd; })
 DeeNativeFileFD_WideInit(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst));
#ifdef DeeSysFS_Utf8CopyObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8CopyObject(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_WideCopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(src),DeeUtf8String_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(dst),DeeUtf8String_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_WideCopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#else
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Utf8InitObject(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ result = -1; goto end_srcfd; })
 DeeNativeFileFD_Utf8InitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst));
#ifdef DeeSysFS_WideCopyObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideCopyObject(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8CopyObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(src),DeeWideString_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(dst),DeeWideString_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_Utf8CopyObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#else
 struct DeeNativeFileFD src_fd,dst_fd; int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_WideInitObject(&src_fd,src,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ result = -1; goto end_srcfd; })
 DeeNativeFileFD_WideInitObject(&dst_fd,dst,DEE_OPENMODE('c',0),0,{ result = -1; goto end_srcfd; });
 result = DeeNativeFileFD_TransferFile(&src_fd,&dst_fd);
 DeeNativeFileFD_Quit(&dst_fd);
end_srcfd: DeeNativeFileFD_Quit(&src_fd);
 return result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_copy); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_COPY_C_INL */
