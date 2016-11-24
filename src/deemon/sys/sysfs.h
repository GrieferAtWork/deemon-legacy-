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
// >> [[optional]] void DeeSysFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8HasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8DelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, int result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, int result, CODE on_error);
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
// >> [[optional]] void DeeSysFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
//////////////////////////////////////////////////////////////////////////
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
// >> [[optional]] void DeeSysFS_Utf8IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsAbs(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8IsAbsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideIsAbsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8ExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, int *result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, int *result, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8Remove(DEE_A_IN_T Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideRemove(DEE_A_IN_T Dee_WideChar const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8Unlink(DEE_A_IN_T Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideUnlink(DEE_A_IN_T Dee_WideChar const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8RmDir(DEE_A_IN_T Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideRmDir(DEE_A_IN_T Dee_WideChar const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8RmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8MkDir(DEE_A_IN_T Dee_Utf8Char const *path, CODE on_error, DEE_A_IN Dee_mode_t mode);
// >> [[optional]] void DeeSysFS_WideMkDir(DEE_A_IN_T Dee_WideChar const *path, CODE on_error, DEE_A_IN Dee_mode_t mode);
// >> [[optional]] void DeeSysFS_Utf8MkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error, DEE_A_IN Dee_mode_t mode);
// >> [[optional]] void DeeSysFS_WideMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, CODE on_error, DEE_A_IN Dee_mode_t mode);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8MoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst, CODE on_error);
// >> [[optional]] void DeeSysFS_WideMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *link_path, DEE_A_IN_Z Dee_Utf8Char const *target_path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideLink(DEE_A_IN_Z Dee_WideChar const *link_path, DEE_A_IN_Z Dee_WideChar const *target_path, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8LinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_path, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_path, CODE on_error);
// >> [[optional]] void DeeSysFS_WideLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_path, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_path, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFS_Utf8Readlink(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideReadlink(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OBJECT(DeeWideStringObject) **result, CODE on_error);
// >> [[optional]] void DeeSysFS_Utf8ReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
// >> [[optional]] void DeeSysFS_WideReadlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OBJECT(DeeWideStringObject) **result, CODE on_error);
//////////////////////////////////////////////////////////////////////////


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.sysfs.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_unix.sysfs.h>
#elif !defined(__DEEMON__)
# error "No Filesystem implementation for this platform"
#endif


//////////////////////////////////////////////////////////////////////////
// Implement missing, substitutable callbacks
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

/*[[[deemon
const pattern = "\
#if !defined(DeeSysFS_Utf8{name}Object) && defined(DeeSysFS_Utf8{name})
#define DeeSysFS_Utf8{name}Object(path,result,...) DeeSysFS_Utf8{name}(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Wide{name}Object) && defined(DeeSysFS_Wide{name})
#define DeeSysFS_Wide{name}Object(path,result,...) DeeSysFS_Wide{name}(DeeWideString_STR(path),result,__VA_ARGS__)
#endif";
function make(name) { print pattern.format({ .name = name }); }
make("IsFile");
make("IsDir");
make("IsLink");
make("IsDrive");
make("IsMount");
make("IsHidden");
make("IsExecutable");
make("IsCharDev");
make("IsBlockDev");
make("IsFiFo");
make("IsSocket");
make("IsAbs");
make("Exists");
]]]*/
#if !defined(DeeSysFS_Utf8IsFileObject) && defined(DeeSysFS_Utf8IsFile)
#define DeeSysFS_Utf8IsFileObject(path,result,...) DeeSysFS_Utf8IsFile(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsFileObject) && defined(DeeSysFS_WideIsFile)
#define DeeSysFS_WideIsFileObject(path,result,...) DeeSysFS_WideIsFile(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsDirObject) && defined(DeeSysFS_Utf8IsDir)
#define DeeSysFS_Utf8IsDirObject(path,result,...) DeeSysFS_Utf8IsDir(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsDirObject) && defined(DeeSysFS_WideIsDir)
#define DeeSysFS_WideIsDirObject(path,result,...) DeeSysFS_WideIsDir(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsLinkObject) && defined(DeeSysFS_Utf8IsLink)
#define DeeSysFS_Utf8IsLinkObject(path,result,...) DeeSysFS_Utf8IsLink(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsLinkObject) && defined(DeeSysFS_WideIsLink)
#define DeeSysFS_WideIsLinkObject(path,result,...) DeeSysFS_WideIsLink(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsDriveObject) && defined(DeeSysFS_Utf8IsDrive)
#define DeeSysFS_Utf8IsDriveObject(path,result,...) DeeSysFS_Utf8IsDrive(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsDriveObject) && defined(DeeSysFS_WideIsDrive)
#define DeeSysFS_WideIsDriveObject(path,result,...) DeeSysFS_WideIsDrive(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsMountObject) && defined(DeeSysFS_Utf8IsMount)
#define DeeSysFS_Utf8IsMountObject(path,result,...) DeeSysFS_Utf8IsMount(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsMountObject) && defined(DeeSysFS_WideIsMount)
#define DeeSysFS_WideIsMountObject(path,result,...) DeeSysFS_WideIsMount(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsHiddenObject) && defined(DeeSysFS_Utf8IsHidden)
#define DeeSysFS_Utf8IsHiddenObject(path,result,...) DeeSysFS_Utf8IsHidden(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsHiddenObject) && defined(DeeSysFS_WideIsHidden)
#define DeeSysFS_WideIsHiddenObject(path,result,...) DeeSysFS_WideIsHidden(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsExecutableObject) && defined(DeeSysFS_Utf8IsExecutable)
#define DeeSysFS_Utf8IsExecutableObject(path,result,...) DeeSysFS_Utf8IsExecutable(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsExecutableObject) && defined(DeeSysFS_WideIsExecutable)
#define DeeSysFS_WideIsExecutableObject(path,result,...) DeeSysFS_WideIsExecutable(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsCharDevObject) && defined(DeeSysFS_Utf8IsCharDev)
#define DeeSysFS_Utf8IsCharDevObject(path,result,...) DeeSysFS_Utf8IsCharDev(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsCharDevObject) && defined(DeeSysFS_WideIsCharDev)
#define DeeSysFS_WideIsCharDevObject(path,result,...) DeeSysFS_WideIsCharDev(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsBlockDevObject) && defined(DeeSysFS_Utf8IsBlockDev)
#define DeeSysFS_Utf8IsBlockDevObject(path,result,...) DeeSysFS_Utf8IsBlockDev(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsBlockDevObject) && defined(DeeSysFS_WideIsBlockDev)
#define DeeSysFS_WideIsBlockDevObject(path,result,...) DeeSysFS_WideIsBlockDev(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsFiFoObject) && defined(DeeSysFS_Utf8IsFiFo)
#define DeeSysFS_Utf8IsFiFoObject(path,result,...) DeeSysFS_Utf8IsFiFo(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsFiFoObject) && defined(DeeSysFS_WideIsFiFo)
#define DeeSysFS_WideIsFiFoObject(path,result,...) DeeSysFS_WideIsFiFo(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsSocketObject) && defined(DeeSysFS_Utf8IsSocket)
#define DeeSysFS_Utf8IsSocketObject(path,result,...) DeeSysFS_Utf8IsSocket(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsSocketObject) && defined(DeeSysFS_WideIsSocket)
#define DeeSysFS_WideIsSocketObject(path,result,...) DeeSysFS_WideIsSocket(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8IsAbsObject) && defined(DeeSysFS_Utf8IsAbs)
#define DeeSysFS_Utf8IsAbsObject(path,result,...) DeeSysFS_Utf8IsAbs(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideIsAbsObject) && defined(DeeSysFS_WideIsAbs)
#define DeeSysFS_WideIsAbsObject(path,result,...) DeeSysFS_WideIsAbs(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8ExistsObject) && defined(DeeSysFS_Utf8Exists)
#define DeeSysFS_Utf8ExistsObject(path,result,...) DeeSysFS_Utf8Exists(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideExistsObject) && defined(DeeSysFS_WideExists)
#define DeeSysFS_WideExistsObject(path,result,...) DeeSysFS_WideExists(DeeWideString_STR(path),result,__VA_ARGS__)
#endif
//[[[end]]]

#if !defined(DeeSysFS_Utf8ChdirObject) && defined(DeeSysFS_Utf8Chdir)
#define DeeSysFS_Utf8ChdirObject(path,...) DeeSysFS_Utf8Chdir(DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideChdirObject) && defined(DeeSysFS_WideChdir)
#define DeeSysFS_WideChdirObject(path,...) DeeSysFS_WideChdir(DeeWideString_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8UnlinkObject) && defined(DeeSysFS_Utf8Unlink)
#define DeeSysFS_Utf8UnlinkObject(path,...) DeeSysFS_Utf8Unlink(DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideUnlinkObject) && defined(DeeSysFS_WideUnlink)
#define DeeSysFS_WideUnlinkObject(path,...) DeeSysFS_WideUnlink(DeeWideString_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8RmDirObject) && defined(DeeSysFS_Utf8RmDir)
#define DeeSysFS_Utf8RmDirObject(path,...) DeeSysFS_Utf8RmDir(DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideRmDirObject) && defined(DeeSysFS_WideRmDir)
#define DeeSysFS_WideRmDirObject(path,...) DeeSysFS_WideRmDir(DeeWideString_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8RemoveObject) && defined(DeeSysFS_Utf8Remove)
#define DeeSysFS_Utf8RemoveObject(path,...) DeeSysFS_Utf8Remove(DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideRemoveObject) && defined(DeeSysFS_WideRemove)
#define DeeSysFS_WideRemoveObject(path,...) DeeSysFS_WideRemove(DeeWideString_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryMkDirObject) && defined(DeeSysFS_Utf8TryMkDir)
#define DeeSysFS_Utf8TryMkDirObject(path,mode) DeeSysFS_Utf8TryMkDir(DeeUtf8String_STR(path),mode)
#endif
#if !defined(DeeSysFS_WideTryMkDirObject) && defined(DeeSysFS_WideTryMkDir)
#define DeeSysFS_WideTryMkDirObject(path,mode) DeeSysFS_WideTryMkDir(DeeWideString_STR(path),mode)
#endif
#if !defined(DeeSysFS_Utf8MkDirObject) && defined(DeeSysFS_Utf8MkDir)
#define DeeSysFS_Utf8MkDirObject(path,mode,...) DeeSysFS_Utf8MkDir(DeeUtf8String_STR(path),mode,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideMkDirObject) && defined(DeeSysFS_WideMkDir)
#define DeeSysFS_WideMkDirObject(path,mode,...) DeeSysFS_WideMkDir(DeeWideString_STR(path),mode,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryCopyObject) && defined(DeeSysFS_Utf8TryCopy)
#define DeeSysFS_Utf8TryCopyObject(src,dst) DeeSysFS_Utf8TryCopy(DeeUtf8String_STR(src),DeeUtf8String_STR(dst))
#endif
#if !defined(DeeSysFS_WideTryCopyObject) && defined(DeeSysFS_WideTryCopy)
#define DeeSysFS_WideTryCopyObject(src,dst) DeeSysFS_WideTryCopy(DeeWideString_STR(src),DeeWideString_STR(dst))
#endif
#if !defined(DeeSysFS_Utf8CopyObject) && defined(DeeSysFS_Utf8Copy)
#define DeeSysFS_Utf8CopyObject(src,dst,...) DeeSysFS_Utf8Copy(DeeUtf8String_STR(src),DeeUtf8String_STR(dst),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideCopyObject) && defined(DeeSysFS_WideCopy)
#define DeeSysFS_WideCopyObject(src,dst,...) DeeSysFS_WideCopy(DeeWideString_STR(src),DeeWideString_STR(dst),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryMoveObject) && defined(DeeSysFS_Utf8TryMove)
#define DeeSysFS_Utf8TryMoveObject(src,dst) DeeSysFS_Utf8TryMove(DeeUtf8String_STR(src),DeeUtf8String_STR(dst))
#endif
#if !defined(DeeSysFS_WideTryMoveObject) && defined(DeeSysFS_WideTryMove)
#define DeeSysFS_WideTryMoveObject(src,dst) DeeSysFS_WideTryMove(DeeWideString_STR(src),DeeWideString_STR(dst))
#endif
#if !defined(DeeSysFS_Utf8MoveObject) && defined(DeeSysFS_Utf8Move)
#define DeeSysFS_Utf8MoveObject(src,dst,...) DeeSysFS_Utf8Move(DeeUtf8String_STR(src),DeeUtf8String_STR(dst),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideMoveObject) && defined(DeeSysFS_WideMove)
#define DeeSysFS_WideMoveObject(src,dst,...) DeeSysFS_WideMove(DeeWideString_STR(src),DeeWideString_STR(dst),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8TryLinkObject) && defined(DeeSysFS_Utf8TryLink)
#define DeeSysFS_Utf8TryLinkObject(link_name,target_name) DeeSysFS_Utf8TryLink(DeeUtf8String_STR(link_name),DeeUtf8String_STR(target_name))
#endif
#if !defined(DeeSysFS_WideTryLinkObject) && defined(DeeSysFS_WideTryLink)
#define DeeSysFS_WideTryLinkObject(link_name,target_name) DeeSysFS_WideTryLink(DeeWideString_STR(link_name),DeeWideString_STR(target_name))
#endif
#if !defined(DeeSysFS_Utf8LinkObject) && defined(DeeSysFS_Utf8Link)
#define DeeSysFS_Utf8LinkObject(link_name,target_name,...) DeeSysFS_Utf8Link(DeeUtf8String_STR(link_name),DeeUtf8String_STR(target_name),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideLinkObject) && defined(DeeSysFS_WideLink)
#define DeeSysFS_WideLinkObject(link_name,target_name,...) DeeSysFS_WideLink(DeeWideString_STR(link_name),DeeWideString_STR(target_name),__VA_ARGS__)
#endif
#if !defined(DeeSysFS_Utf8ReadlinkObject) && defined(DeeSysFS_Utf8Readlink)
#define DeeSysFS_Utf8ReadlinkObject(path,result,...) DeeSysFS_Utf8Readlink(DeeUtf8String_STR(path),result,__VA_ARGS__)
#endif
#if !defined(DeeSysFS_WideReadlinkObject) && defined(DeeSysFS_WideReadlink)
#define DeeSysFS_WideReadlinkObject(path,result,...) DeeSysFS_WideReadlink(DeeWideString_STR(path),result,__VA_ARGS__)
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
