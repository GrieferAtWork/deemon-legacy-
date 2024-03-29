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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_GETOWN_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_GETOWN_C_INL 1
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

#ifndef DEE_NFS_HAVE_GETOWN
DeeError_NEW_STATIC(_dee_notimplemented_getown,&DeeErrorType_NotImplemented,"getown");
#endif

#ifdef DEE_NFS_HAVE_GETOWN
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwn(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8GetOwn
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8GetOwn(path,owner,group,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8GetOwnObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_Utf8GetOwnObject(path_ob,owner,group,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideGetOwnObject)\
   || defined(DeeSysFS_WideGetOwn)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideGetOwnObject(path_ob,owner,group);
 Dee_DECREF(path_ob);
 return result;
#else
 struct DeeNativeFileFD fd;
 DEE_NFS_CHECKINTERRUPT(return -1);
 DeeNativeFileFD_Utf8Init(&fd,path,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ DeeNativeFileFD_Quit(&fd); return -1; })
 DeeSysFileFD_GetOwn(&fd,owner,group,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwn(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideGetOwn
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideGetOwn(path,owner,group,return -1);
 return 0;
#elif defined(DeeSysFS_WideGetOwnObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_WideGetOwnObject(path_ob,owner,group,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8GetOwnObject)\
   || defined(DeeSysFS_Utf8GetOwn)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8GetOwnObject(path_ob,owner,group);
 Dee_DECREF(path_ob);
 return result;
#else
 struct DeeNativeFileFD fd;
 DEE_NFS_CHECKINTERRUPT(return -1);
 DeeNativeFileFD_WideInit(&fd,path,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ DeeNativeFileFD_Quit(&fd); return -1; })
 DeeSysFileFD_GetOwn(&fd,owner,group,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwnObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8GetOwnObject
 DEE_NFS_CHECKINTERRUPT(return -1);
 DeeSysFS_Utf8GetOwnObject(path,owner,group,return -1);
 return 0;
#elif defined(DeeSysFS_WideGetOwnObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_WideGetOwnObject(path_ob,owner,group,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 struct DeeNativeFileFD fd;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Utf8InitObject(&fd,path,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ DeeNativeFileFD_Quit(&fd); return -1; })
 DeeSysFileFD_GetOwn(&fd,owner,group,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwnObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideGetOwnObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideGetOwnObject(path,owner,group,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8GetOwnObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_Utf8GetOwnObject(path_ob,owner,group,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#else
 struct DeeNativeFileFD fd;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_WideInitObject(&fd,path,DEE_OPENMODE('r',0),0,return -1);
 DEE_NFS_CHECKINTERRUPT({ DeeNativeFileFD_Quit(&fd); return -1; })
 DeeSysFileFD_GetOwn(&fd,owner,group,{ DeeNativeFileFD_Quit(&fd); return -1; });
 DeeNativeFileFD_Quit(&fd);
 return 0;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *DEE_UNUSED(owner), DEE_A_OUT Dee_gid_t *DEE_UNUSED(group)) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_getown); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *DEE_UNUSED(owner), DEE_A_OUT Dee_gid_t *DEE_UNUSED(group)) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_getown); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *DEE_UNUSED(owner), DEE_A_OUT Dee_gid_t *DEE_UNUSED(group)) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_getown); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *DEE_UNUSED(owner), DEE_A_OUT Dee_gid_t *DEE_UNUSED(group)) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_getown); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_GETOWN_C_INL */
