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
#ifndef GUARD_DEEMON_SYS_SYSFS_H
#define GUARD_DEEMON_SYS_SYSFS_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetCwd(void);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetCwd(void);
// >> [[optional]] bool DeeSysFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] void DeeSysFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8TryGetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideTryGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
// >> [[optional]] bool DeeSysFS_Utf8TryHasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
// >> [[optional]] bool DeeSysFS_WideTryHasEnv(DEE_A_IN_Z Dee_WideChar const *envname);
// >> [[optional]] bool DeeSysFS_Utf8TryHasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
// >> [[optional]] bool DeeSysFS_WideTryHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
// >> [[optional]] void DeeSysFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8HasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, int result, CODE on_error);
// >> [[optional]] bool DeeSysFS_Utf8TryDelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
// >> [[optional]] bool DeeSysFS_WideTryDelEnv(DEE_A_IN_Z Dee_WideChar const *envname);
// >> [[optional]] bool DeeSysFS_Utf8TryDelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
// >> [[optional]] bool DeeSysFS_WideTryDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
// >> [[optional]] void DeeSysFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8DelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, int result, CODE on_error);
// >> [[optional]] bool DeeSysFS_Utf8TrySetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *newvalue);
// >> [[optional]] bool DeeSysFS_WideTrySetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *newvalue);
// >> [[optional]] bool DeeSysFS_Utf8TrySetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *newvalue);
// >> [[optional]] bool DeeSysFS_WideTrySetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *newvalue);
// >> [[optional]] void DeeSysFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *newvalue, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *newvalue, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8SetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *newvalue, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *newvalue, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8EnumEnv(void enum(Dee_size_t envname_size, Dee_Utf8Char const *envname, Dee_size_t envvalue_size, Dee_Utf8Char const *envvalue, CODE on_error), CODE on_error);
// >> [[optional]] void DeeSysFS_WideEnumEnv(void enum(Dee_size_t envname_size, Dee_WideChar const *envname, Dee_size_t envvalue_size, Dee_WideChar const *envvalue, CODE on_error), CODE on_error);
//                  - Execute a given macro 'enum' with the details to every known env variable.
//                    NOTE: If an error occurred in 'enum', it should run its 'on_error'
//                          code, which in turn executed the outer 'on_error' code.
//                    NOTE: Remember to make 'on_error' a variadic macro argument
// >> [[optional]] #define DEE_SYSFS_UTF8ENUMENV_ENVNAME_ZERO_TERMINATED
// >> [[optional]] #define DEE_SYSFS_UTF8ENUMENV_ENVVALUE_ZERO_TERMINATED
// >> [[optional]] #define DEE_SYSFS_WIDEENUMENV_ENVNAME_ZERO_TERMINATED
// >> [[optional]] #define DEE_SYSFS_WIDEENUMENV_ENVVALUE_ZERO_TERMINATED
//                   - Defined if the 'envname' or 'envvalue' arguments of the 'enum' callback in
//                     'DeeSysFS_Utf8EnumEnv' and 'DeeSysFS_WideEnumEnv' respectively are zero-terminated.
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetHome(void);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetHome(void);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetUserHome(DEE_A_IN_Z Dee_Utf8Char const *username);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetUserHome(DEE_A_IN_Z Dee_WideChar const *username);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetUserHomeObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetUserHomeObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFS_Utf8GetTmp(void);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFS_WideGetTmp(void);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] bool DeeSysFS_Utf8TryGetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
// >> [[optional]] bool DeeSysFS_WideTryGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
// >> [[optional]] bool DeeSysFS_Utf8TryGetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
// >> [[optional]] bool DeeSysFS_WideTryGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
// >> [[optional]] void DeeSysFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] bool DeeSysFS_Utf8TrySetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
// >> [[optional]] bool DeeSysFS_WideTrySetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
// >> [[optional]] bool DeeSysFS_Utf8TrySetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
// >> [[optional]] bool DeeSysFS_WideTrySetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
// >> [[optional]] void DeeSysFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] bool DeeSysFS_Utf8TryIsFile(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsFile(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsDir(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsDir(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsLink(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsDrive(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsDrive(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsMount(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsMount(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsHidden(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsHidden(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsHiddenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsHiddenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsExecutable(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsExecutableObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsExecutableObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsCharDev(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsCharDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsCharDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsBlockDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsBlockDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsFiFo(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsFiFoObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsFiFoObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsSocket(DEE_A_IN_Z Dee_Utf8Char const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsSocket(DEE_A_IN_Z Dee_WideChar const *path);
// >> [[optional]] bool DeeSysFS_Utf8TryIsSocketObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
// >> [[optional]] bool DeeSysFS_WideTryIsSocketObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
// >> [[optional]] void DeeSysFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsHiddenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsHiddenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsExecutableObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsExecutableObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsCharDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsCharDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsBlockDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsBlockDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsFiFoObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsFiFoObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsSocketObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsSocketObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
//////////////////////////////////////////////////////////////////////////


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.sysfs.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_unix.sysfs.h>
#else
# error "No Filesystem implementation for this platform"
#endif


//////////////////////////////////////////////////////////////////////////
// Implement missing, substitutable callbacks
#if !defined(DeeSysFS_Utf8TryChdirObject) && defined(DeeSysFS_Utf8TryChdir)
#define DeeSysFS_Utf8TryChdirObject(path) DeeSysFS_Utf8TryChdir(DeeUtf8String_STR(path))
#endif
#if !defined(DeeSysFS_WideTryChdirObject) && defined(DeeSysFS_WideTryChdir)
#define DeeSysFS_WideTryChdirObject(path) DeeSysFS_WideTryChdir(DeeWideString_STR(path))
#endif
#if !defined(DeeSysFS_Utf8ChdirObject) && defined(DeeSysFS_Utf8Chdir)
#define DeeSysFS_Utf8ChdirObject(path,...) DeeSysFS_Utf8Chdir(DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideChdirObject) && defined(DeeSysFS_WideChdir)
#define DeeSysFS_WideChdirObject(path,...) DeeSysFS_WideChdir(DeeWideString_STR(path),__VA_ARGS__)
#endif

#if !defined(DeeSysFS_Utf8GetEnvObject) && defined(DeeSysFS_Utf8GetEnv)
#define DeeSysFS_Utf8GetEnvObject(envname) DeeSysFS_Utf8GetEnv(DeeUtf8String_STR(envname))
#endif
#if !defined(DeeSysFS_WideGetEnvObject) && defined(DeeSysFS_WideGetEnv)
#define DeeSysFS_WideGetEnvObject(envname) DeeSysFS_WideGetEnv(DeeWideString_STR(envname))
#endif
#if !defined(DeeSysFS_Utf8TryGetEnvObject) && defined(DeeSysFS_Utf8TryGetEnv)
#define DeeSysFS_Utf8TryGetEnvObject(envname) DeeSysFS_Utf8TryGetEnv(DeeUtf8String_STR(envname))
#endif
#if !defined(DeeSysFS_WideTryGetEnvObject) && defined(DeeSysFS_WideTryGetEnv)
#define DeeSysFS_WideTryGetEnvObject(envname) DeeSysFS_WideTryGetEnv(DeeWideString_STR(envname))
#endif
#if !defined(DeeSysFS_Utf8HasEnvObject) && defined(DeeSysFS_Utf8HasEnv)
#define DeeSysFS_Utf8HasEnvObject(envname,result,...) DeeSysFS_Utf8HasEnv(DeeUtf8String_STR(envname),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideHasEnvObject) && defined(DeeSysFS_WideHasEnv)
#define DeeSysFS_WideHasEnvObject(envname,result,...) DeeSysFS_WideHasEnv(DeeWideString_STR(envname),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryHasEnvObject) && defined(DeeSysFS_Utf8TryHasEnv)
#define DeeSysFS_Utf8TryHasEnvObject(envname) DeeSysFS_Utf8TryHasEnv(DeeUtf8String_STR(envname))
#endif
#if !defined(DeeSysFS_WideTryHasEnvObject) && defined(DeeSysFS_WideTryHasEnv)
#define DeeSysFS_WideTryHasEnvObject(envname) DeeSysFS_WideTryHasEnv(DeeWideString_STR(envname))
#endif
#if !defined(DeeSysFS_Utf8DelEnvObject) && defined(DeeSysFS_Utf8DelEnv)
#define DeeSysFS_Utf8DelEnvObject(envname,...) DeeSysFS_Utf8DelEnv(DeeUtf8String_STR(envname),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideDelEnvObject) && defined(DeeSysFS_WideDelEnv)
#define DeeSysFS_WideDelEnvObject(envname,...) DeeSysFS_WideDelEnv(DeeWideString_STR(envname),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryDelEnvObject) && defined(DeeSysFS_Utf8TryDelEnv)
#define DeeSysFS_Utf8TryDelEnvObject(envname) DeeSysFS_Utf8TryDelEnv(DeeUtf8String_STR(envname))
#endif
#if !defined(DeeSysFS_WideTryDelEnvObject) && defined(DeeSysFS_WideTryDelEnv)
#define DeeSysFS_WideTryDelEnvObject(envname) DeeSysFS_WideTryDelEnv(DeeWideString_STR(envname))
#endif
#if !defined(DeeSysFS_Utf8SetEnvObject) && defined(DeeSysFS_Utf8SetEnv)
#define DeeSysFS_Utf8SetEnvObject(envname,newvalue,...) DeeSysFS_Utf8SetEnv(DeeUtf8String_STR(envname),DeeUtf8String_STR(newvalue),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideSetEnvObject) && defined(DeeSysFS_WideSetEnv)
#define DeeSysFS_WideSetEnvObject(envname,newvalue,...) DeeSysFS_WideSetEnv(DeeWideString_STR(envname),DeeWideString_STR(newvalue),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TrySetEnvObject) && defined(DeeSysFS_Utf8TrySetEnv)
#define DeeSysFS_Utf8TrySetEnvObject(envname,newvalue) DeeSysFS_Utf8TrySetEnv(DeeUtf8String_STR(envname),DeeUtf8String_STR(newvalue))
#endif
#if !defined(DeeSysFS_WideTrySetEnvObject) && defined(DeeSysFS_WideTrySetEnv)
#define DeeSysFS_WideTrySetEnvObject(envname,newvalue) DeeSysFS_WideTrySetEnv(DeeWideString_STR(envname),DeeWideString_STR(newvalue))
#endif

#if !defined(DeeSysFS_Utf8GetUserHomeObject) && defined(DeeSysFS_Utf8GetUserHome)
#define DeeSysFS_Utf8GetUserHomeObject(username) DeeSysFS_Utf8GetUserHome(DeeUtf8String_STR(username))
#endif
#if !defined(DeeSysFS_WideGetUserHomeObject) && defined(DeeSysFS_WideGetUserHome)
#define DeeSysFS_WideGetUserHomeObject(username) DeeSysFS_WideGetUserHome(DeeWideString_STR(username))
#endif

#if !defined(DeeSysFS_Utf8GetTimesObject) && defined(DeeSysFS_Utf8GetTimes)
#define DeeSysFS_Utf8GetTimesObject(path,atime,ctime,mtime,...) DeeSysFS_Utf8GetTimes(DeeUtf8String_STR(path),atime,ctime,mtime,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideGetTimesObject) && defined(DeeSysFS_WideGetTimes)
#define DeeSysFS_WideGetTimesObject(path,atime,ctime,mtime,...) DeeSysFS_WideGetTimes(DeeWideString_STR(path),atime,ctime,mtime,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryGetTimesObject) && defined(DeeSysFS_Utf8TryGetTimes)
#define DeeSysFS_Utf8TryGetTimesObject(path,atime,ctime,mtime) DeeSysFS_Utf8TryGetTimes(DeeUtf8String_STR(path),atime,ctime,mtime)
#endif
#if !defined(DeeSysFS_WideTryGetTimesObject) && defined(DeeSysFS_WideTryGetTimes)
#define DeeSysFS_WideTryGetTimesObject(path,atime,ctime,mtime) DeeSysFS_WideTryGetTimes(DeeWideString_STR(path),atime,ctime,mtime)
#endif
#if !defined(DeeSysFS_Utf8SetTimesObject) && defined(DeeSysFS_Utf8SetTimes)
#define DeeSysFS_Utf8SetTimesObject(path,atime,ctime,mtime,...) DeeSysFS_Utf8SetTimes(DeeUtf8String_STR(path),atime,ctime,mtime,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideSetTimesObject) && defined(DeeSysFS_WideSetTimes)
#define DeeSysFS_WideSetTimesObject(path,atime,ctime,mtime,...) DeeSysFS_WideSetTimes(DeeWideString_STR(path),atime,ctime,mtime,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TrySetTimesObject) && defined(DeeSysFS_Utf8TrySetTimes)
#define DeeSysFS_Utf8TrySetTimesObject(path,atime,ctime,mtime) DeeSysFS_Utf8TrySetTimes(DeeUtf8String_STR(path),atime,ctime,mtime)
#endif
#if !defined(DeeSysFS_WideTrySetTimesObject) && defined(DeeSysFS_WideTrySetTimes)
#define DeeSysFS_WideTrySetTimesObject(path,atime,ctime,mtime) DeeSysFS_WideTrySetTimes(DeeWideString_STR(path),atime,ctime,mtime)
#endif


//////////////////////////////////////////////////////////////////////////
// Link the default encoding to the mapped functions
#ifdef DeeSysFS_Utf8GetCwd
#define DeeSysFS_GetCwd DeeSysFS_Utf8GetCwd
#endif
#ifdef DeeSysFS_Utf8TryChdir
#define DeeSysFS_TryChdir DeeSysFS_Utf8TryChdir
#endif
#ifdef DeeSysFS_Utf8TryChdirObject
#define DeeSysFS_TryChdirObject DeeSysFS_Utf8TryChdirObject
#endif
#ifdef DeeSysFS_Utf8Chdir
#define DeeSysFS_Chdir DeeSysFS_Utf8Chdir
#endif
#ifdef DeeSysFS_Utf8ChdirObject
#define DeeSysFS_ChdirObject DeeSysFS_Utf8ChdirObject
#endif

#ifdef DeeSysFS_Utf8GetEnv
#define DeeSysFS_GetEnv DeeSysFS_Utf8GetEnv
#endif
#ifdef DeeSysFS_Utf8TryGetEnv
#define DeeSysFS_TryGetEnv DeeSysFS_Utf8TryGetEnv
#endif
#ifdef DeeSysFS_Utf8GetEnvObject
#define DeeSysFS_GetEnvObject DeeSysFS_Utf8GetEnvObject
#endif
#ifdef DeeSysFS_Utf8TryGetEnvObject
#define DeeSysFS_TryGetEnvObject DeeSysFS_Utf8TryGetEnvObject
#endif
#ifdef DeeSysFS_Utf8HasEnv
#define DeeSysFS_HasEnv DeeSysFS_Utf8HasEnv
#endif
#ifdef DeeSysFS_Utf8TryHasEnv
#define DeeSysFS_TryHasEnv DeeSysFS_Utf8TryHasEnv
#endif
#ifdef DeeSysFS_Utf8HasEnvObject
#define DeeSysFS_HasEnvObject DeeSysFS_Utf8HasEnvObject
#endif
#ifdef DeeSysFS_Utf8TryHasEnvObject
#define DeeSysFS_TryHasEnvObject DeeSysFS_Utf8TryHasEnvObject
#endif
#ifdef DeeSysFS_Utf8DelEnv
#define DeeSysFS_DelEnv DeeSysFS_Utf8DelEnv
#endif
#ifdef DeeSysFS_Utf8TryDelEnv
#define DeeSysFS_TryDelEnv DeeSysFS_Utf8TryDelEnv
#endif
#ifdef DeeSysFS_Utf8DelEnvObject
#define DeeSysFS_DelEnvObject DeeSysFS_Utf8DelEnvObject
#endif
#ifdef DeeSysFS_Utf8TryDelEnvObject
#define DeeSysFS_TryDelEnvObject DeeSysFS_Utf8TryDelEnvObject
#endif
#ifdef DeeSysFS_Utf8SetEnv
#define DeeSysFS_SetEnv DeeSysFS_Utf8SetEnv
#endif
#ifdef DeeSysFS_Utf8TrySetEnv
#define DeeSysFS_TrySetEnv DeeSysFS_Utf8TrySetEnv
#endif
#ifdef DeeSysFS_Utf8SetEnvObject
#define DeeSysFS_SetEnvObject DeeSysFS_Utf8SetEnvObject
#endif
#ifdef DeeSysFS_Utf8TrySetEnvObject
#define DeeSysFS_TrySetEnvObject DeeSysFS_Utf8TrySetEnvObject
#endif
#ifdef DeeSysFS_Utf8EnumEnv
#define DeeSysFS_EnumEnv DeeSysFS_Utf8EnumEnv
#endif

#ifdef DeeSysFS_Utf8GetHome
#define DeeSysFS_GetHome DeeSysFS_Utf8GetHome
#endif
#ifdef DeeSysFS_Utf8GetUserHome
#define DeeSysFS_GetUserHome DeeSysFS_Utf8GetUserHome
#endif
#ifdef DeeSysFS_Utf8GetUserHomeObject
#define DeeSysFS_GetUserHomeObject DeeSysFS_Utf8GetUserHomeObject
#endif

#ifdef DeeSysFS_Utf8GetTimes
#define DeeSysFS_GetTimes DeeSysFS_Utf8GetTimes
#endif
#ifdef DeeSysFS_Utf8TryGetTimes
#define DeeSysFS_TryGetTimes DeeSysFS_Utf8TryGetTimes
#endif
#ifdef DeeSysFS_Utf8GetTimesObject
#define DeeSysFS_GetTimesObject DeeSysFS_Utf8GetTimesObject
#endif
#ifdef DeeSysFS_Utf8TryGetTimesObject
#define DeeSysFS_TryGetTimesObject DeeSysFS_Utf8TryGetTimesObject
#endif
#ifdef DeeSysFS_Utf8SetTimes
#define DeeSysFS_SetTimes DeeSysFS_Utf8SetTimes
#endif
#ifdef DeeSysFS_Utf8TrySetTimes
#define DeeSysFS_TrySetTimes DeeSysFS_Utf8TrySetTimes
#endif
#ifdef DeeSysFS_Utf8SetTimesObject
#define DeeSysFS_SetTimesObject DeeSysFS_Utf8SetTimesObject
#endif
#ifdef DeeSysFS_Utf8TrySetTimesObject
#define DeeSysFS_TrySetTimesObject DeeSysFS_Utf8TrySetTimesObject
#endif

#endif /* !GUARD_DEEMON_SYS_SYSFS_H */
