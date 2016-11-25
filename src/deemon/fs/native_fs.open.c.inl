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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_OPEN_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_OPEN_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/file/file.io.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/fs/native_filefd.h>
#include <deemon/sys/sysfd.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_OPEN
DeeError_NEW_STATIC(_dee_notimplemented_open,&DeeErrorType_NotImplemented,"open");
#endif

#ifdef DEE_NFS_HAVE_OPEN
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Open(
 DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ DeeObject_Free(result); return NULL; })
 DeeNativeFileFD_Utf8Init(&result->io_descr,file,openmode,permissions,
                          { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpen(
 DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ DeeObject_Free(result); return NULL; })
 DeeNativeFileFD_WideInit(&result->io_descr,file,openmode,permissions,
                          { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpenObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ DeeObject_Free(result); return NULL; })
 DeeNativeFileFD_Utf8InitObject(&result->io_descr,file,openmode,permissions,
                                { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpenObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DEE_NFS_CHECKINTERRUPT({ DeeObject_Free(result); return NULL; })
 DeeNativeFileFD_WideInitObject(&result->io_descr,file,openmode,permissions,
                                { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Open(DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) { DEE_ASSERT(file); (void)file,openmode,permissions; DeeError_Throw((DeeObject *)&_dee_notimplemented_open); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpen(DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) { DEE_ASSERT(file); (void)file,openmode,permissions; DeeError_Throw((DeeObject *)&_dee_notimplemented_open); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) { DEE_ASSERT(DeeObject_Check(file) && DeeUtf8String_Check(file)); (void)file,openmode,permissions; DeeError_Throw((DeeObject *)&_dee_notimplemented_open); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) { DEE_ASSERT(DeeObject_Check(file) && DeeWideString_Check(file)); (void)file,openmode,permissions; DeeError_Throw((DeeObject *)&_dee_notimplemented_open); return NULL; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_OPEN_C_INL */
