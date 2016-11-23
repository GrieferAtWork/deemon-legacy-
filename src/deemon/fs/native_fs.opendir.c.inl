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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_OPENDIR_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_OPENDIR_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/objects/native_view.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_OPENDIR
DeeError_NEW_STATIC(_dee_notimplemented_opendir,&DeeErrorType_NotImplemented,"opendir");
#endif

#ifdef DEE_NFS_HAVE_OPENDIR
#if !defined(DeeNFSUtf8View) && !defined(DeeNFSWideView)
#error "No view objects defined for opendir"
#endif

DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Opendir(DEE_A_IN_Z Dee_Utf8Char const *path) {
#ifdef DeeNFSUtf8View
 DEE_ASSERT(path);
 return (DeeObject *)DeeNFSUtf8View_OpenPath(path);
#else
 DeeObject *wpath,*result;
 DEE_ASSERT(path);
 if ((wpath = DeeWideString_FromUtf8String(path)) == NULL) return NULL;
 result = (DeeObject *)DeeNFSWideView_OpenPathObject(wpath);
 Dee_DECREF(wpath);
 return result;
#endif
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendir(DEE_A_IN_Z Dee_WideChar const *path) {
#ifdef DeeNFSWideView
 DEE_ASSERT(path);
 return (DeeObject *)DeeNFSWideView_OpenPath(path);
#else
 DeeObject *u8path,*result;
 DEE_ASSERT(path);
 if ((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = (DeeObject *)DeeNFSUtf8View_OpenPath(u8path);
 Dee_DECREF(u8path);
 return result;
#endif
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpendirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
#ifdef DeeNFSUtf8View
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 return (DeeObject *)DeeNFSUtf8View_OpenPathObject(path);
#else
 DeeObject *wpath,*result;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if ((wpath = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return NULL;
 result = (DeeObject *)DeeNFSWideView_OpenPathObject(wpath);
 Dee_DECREF(wpath);
 return result;
#endif
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
#ifdef DeeNFSWideView
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 return (DeeObject *)DeeNFSWideView_OpenPathObject(path);
#else
 DeeObject *u8path,*result;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = (DeeObject *)DeeNFSUtf8View_OpenPathObject(u8path);
 Dee_DECREF(u8path);
 return result;
#endif
}
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Opendir(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_opendir); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendir(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_opendir); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpendirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_opendir); return NULL; }
DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_opendir); return NULL; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_OPENDIR_C_INL */
