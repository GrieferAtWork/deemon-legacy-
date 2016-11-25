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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_REMOVE_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_REMOVE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_REMOVE
DeeError_NEW_STATIC(_dee_notimplemented_remove,&DeeErrorType_NotImplemented,"remove");
#endif

#ifdef DEE_NFS_HAVE_REMOVE
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8Remove
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8Remove(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8RemoveObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_Utf8RemoveObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8Unlink) && defined(DeeSysFS_Utf8RmDir)
 DEE_NFS_CHECKINTERRUPT(return 1);
 DeeSysFS_Utf8Unlink(path,goto try2);
try2:
 if (DeeError_HandleOne(&DeeErrorType_SystemError)) {
  DEE_NFS_CHECKINTERRUPT(return 1);
  DeeSysFS_Utf8RmDir(path,return -1);
  return 0;
 }
 return -1;
#else
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_WideRemoveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path) {
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideRemove
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideRemove(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideRemoveObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_WideRemoveObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideUnlink) && defined(DeeSysFS_WideRmDir)
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideUnlink(path,goto try2);
try2:
 if (DeeError_HandleOne(&DeeErrorType_SystemError)) {
  DEE_NFS_CHECKINTERRUPT(return -1)
  DeeSysFS_WideRmDir(path,return -1);
  return 0;
 }
 return -1;
#else
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8RemoveObject(path_ob);
 Dee_DECREF(path_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8RemoveObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8RemoveObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_WideRemoveObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_WideRemoveObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_Utf8UnlinkObject) && defined(DeeSysFS_Utf8RmDirObject)
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8UnlinkObject(path,goto try2);
try2:
 if (DeeError_HandleOne(&DeeErrorType_SystemError)) {
  DEE_NFS_CHECKINTERRUPT(return -1)
  DeeSysFS_Utf8RmDirObject(path,return -1);
  return 0;
 }
 return -1;
#else
 DeeObject *path_ob; int error;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 error = DeeNFS_WideRemoveObject(path_ob);
 Dee_DECREF(path_ob);
 return error;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideRemoveObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideRemoveObject(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8RemoveObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(path_ob); return -1; })
 DeeSysFS_Utf8RemoveObject(path_ob,{ Dee_DECREF(path_ob); return -1; });
 Dee_DECREF(path_ob);
 return 0;
#elif defined(DeeSysFS_WideUnlinkObject) && defined(DeeSysFS_WideRmDirObject)
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideUnlinkObject(path,goto try2);
try2:
 if (DeeError_HandleOne(&DeeErrorType_SystemError)) {
  DEE_NFS_CHECKINTERRUPT(return -1)
  DeeSysFS_WideRmDirObject(path,return -1);
  return 0;
 }
 return -1;
#else
 DeeObject *path_ob; int error;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 error = DeeNFS_Utf8RemoveObject(path_ob);
 Dee_DECREF(path_ob);
 return error;
#endif
}

#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_remove); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path) { DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_remove); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_remove); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) { DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_remove); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_REMOVE_C_INL */
