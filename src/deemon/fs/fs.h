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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_H
#define GUARD_DEEMON_FS_NATIVE_FS_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>
#include <deemon/sys/sysfs.h>
#include <deemon/sys/sysfd.h>

DEE_DECL_BEGIN

#if defined(DeeSysFS_Utf8GetCwd) || defined(DeeSysFS_WideGetCwd)
#define DEE_NATIVEFS_HAVE_GETCWD
#endif
#if defined(DeeSysFS_Utf8Chdir) || defined(DeeSysFS_WideChdir)\
 || defined(DeeSysFS_Utf8ChdirObject) || defined(DeeSysFS_WideChdirObject)
#define DEE_NATIVEFS_HAVE_CHDIR
#endif

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNativeFS_Utf8GetCwd(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNativeFS_WideGetCwd(void);
extern DEE_A_RET_EXCEPT(-1) int DeeNativeFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNativeFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNativeFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNativeFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_H */
