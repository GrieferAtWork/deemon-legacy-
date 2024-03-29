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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_HASENV_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_HASENV_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/fs/native_fs.h>
#include <deemon/sys/sysfs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_HASENV
DeeError_NEW_STATIC(_dee_notimplemented_hasenv,&DeeErrorType_NotImplemented,"hasenv");
#endif
#ifdef DEE_NFS_HAVE_HASENV
DEE_A_RET_EXCEPT_FAIL(-1,0) int
DeeNFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_Utf8HasEnv
 int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8HasEnv(envname,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8HasEnvObject)
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeUtf8String_New(envname)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return -1; })
 DeeSysFS_Utf8HasEnvObject(envname,&result,{ Dee_DECREF(newenvname); return -1; });
 Dee_DECREF(newenvname);
 return result;
#else
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8String(envname)) == NULL) return -1;
 result = DeeNFS_WideHasEnvObject(newenvname);
 Dee_DECREF(newenvname);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int
DeeNFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *envname) {
 DEE_ASSERT(envname);
#ifdef DeeSysFS_WideHasEnv
 int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideHasEnv(envname,&result,return -1);
 return result;
#elif defined(DeeSysFS_WideHasEnvObject)
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeWideString_New(envname)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return -1; })
 DeeSysFS_WideHasEnvObject(envname,&result,{ Dee_DECREF(newenvname); return -1; });
 Dee_DECREF(newenvname);
 return result;
#else
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideString(envname)) == NULL) return -1;
 result = DeeNFS_Utf8HasEnvObject(newenvname);
 Dee_DECREF(newenvname);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasEnvObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname));
#ifdef DeeSysFS_Utf8HasEnvObject
 int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8HasEnvObject(envname,&result,return -1);
 return result;
#else
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(envname),DeeUtf8String_STR(envname))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return -1; })
 DeeSysFS_WideHasEnvObject(newenvname,&result,{ Dee_DECREF(newenvname); return -1; });
 Dee_DECREF(newenvname);
 return result;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasEnvObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) {
 DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname));
#ifdef DeeSysFS_WideHasEnvObject
 int result;
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideHasEnvObject(envname,&result,return -1);
 return result;
#else
 DeeObject *newenvname; int result;
 if DEE_UNLIKELY((newenvname = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(envname),DeeWideString_STR(envname))) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(newenvname); return -1; })
 DeeSysFS_Utf8HasEnvObject(newenvname,&result,{ Dee_DECREF(newenvname); return -1; });
 Dee_DECREF(newenvname);
 return result;
#endif
}

#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname) { DEE_ASSERT(envname); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_hasenv); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *envname) { DEE_ASSERT(envname); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_hasenv); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeUtf8String_Check(envname)); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_hasenv); return -1; }
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname) { DEE_ASSERT(DeeObject_Check(envname) && DeeWideString_Check(envname)); (void)envname; DeeError_Throw((DeeObject *)&_dee_notimplemented_hasenv); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_HASENV_C_INL */
