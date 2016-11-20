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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_C
#define GUARD_DEEMON_FS_NATIVE_FS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NATIVEFS_HAVE_GETCWD
DeeError_NEW_STATIC(_dee_notimplemented_getcwd,&DeeErrorType_NotImplemented,"getcwd");
#endif
#ifndef DEE_NATIVEFS_HAVE_CHDIR
DeeError_NEW_STATIC(_dee_notimplemented_chdir,&DeeErrorType_NotImplemented,"chdir");
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNativeFS_Utf8GetCwd(void) {
#ifdef DeeSysFS_Utf8GetCwd
 return DeeSysFS_Utf8GetCwd();
#elif defined(DeeSysFS_WideGetCwd)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY((result = DeeSysFS_WideGetCwd()) == NULL) return NULL;
 newresult = DeeUtf8String_FromWideStringWithLength(DeeWideString_SIZE(result),
                                                    DeeWideString_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getcwd);
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNativeFS_WideGetCwd(void) {
#ifdef DeeSysFS_WideGetCwd
 return DeeSysFS_WideGetCwd();
#elif defined(DeeSysFS_Utf8GetCwd)
 DeeObject *result,*newresult;
 if DEE_UNLIKELY((result = DeeSysFS_Utf8GetCwd()) == NULL) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getcwd);
 return NULL;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNativeFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path) {
#ifdef DeeSysFS_Utf8Chdir
 DeeSysFS_Utf8Chdir(path,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8ChdirObject)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
#elif defined(DeeSysFS_WideChdirObject)

#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_getcwd);
 return NULL;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNativeFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path) {
}
DEE_A_RET_EXCEPT(-1) int DeeNativeFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
}
DEE_A_RET_EXCEPT(-1) int DeeNativeFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_H */
