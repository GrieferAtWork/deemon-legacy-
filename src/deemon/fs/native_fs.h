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

#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_MODE_T
DEE_PRIVATE_DECL_DEE_MODE_T
#undef DEE_PRIVATE_DECL_DEE_MODE_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_UID_T
DEE_PRIVATE_DECL_DEE_UID_T
#undef DEE_PRIVATE_DECL_DEE_UID_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_GID_T
DEE_PRIVATE_DECL_DEE_GID_T
#undef DEE_PRIVATE_DECL_DEE_GID_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif


#if defined(DeeSysFS_Utf8GetCwd) || defined(DeeSysFS_WideGetCwd)
#define DEE_NFS_HAVE_GETCWD
#endif
#if defined(DeeSysFS_Utf8Chdir) || defined(DeeSysFS_WideChdir)\
 || defined(DeeSysFS_Utf8ChdirObject) || defined(DeeSysFS_WideChdirObject)\
 || defined(DeeSysFS_Utf8TryChdir) || defined(DeeSysFS_WideTryChdir)\
 || defined(DeeSysFS_Utf8TryChdirObject) || defined(DeeSysFS_WideTryChdirObject)
// TODO: Add fchdir support here
#define DEE_NFS_HAVE_CHDIR
#endif

#if defined(DeeSysFS_Utf8GetHome) || defined(DeeSysFS_WideGetHome)
#define DEE_NFS_HAVE_GETHOME
#endif
#if defined(DeeSysFS_Utf8GetUserHome) || defined(DeeSysFS_WideGetUserHome)\
 || defined(DeeSysFS_Utf8GetUserHomeObject) || defined(DeeSysFS_WideGetUserHomeObject)
#define DEE_NFS_HAVE_GETUSERHOME
#endif

#if defined(DeeSysFS_Utf8GetTmp) || defined(DeeSysFS_WideGetTmp)
#define DEE_NFS_HAVE_GETTMP
#endif

#if defined(DeeSysFS_Utf8EnumEnv) || defined(DeeSysFS_WideEnumEnv)
#define DEE_NFS_HAVE_ENUMENV
#endif

#if defined(DeeSysFS_Utf8GetEnv) || defined(DeeSysFS_WideGetEnv)\
 || defined(DeeSysFS_Utf8TryGetEnv) || defined(DeeSysFS_WideTryGetEnv)\
 || defined(DeeSysFS_Utf8GetEnvObject) || defined(DeeSysFS_WideGetEnvObject)\
 || defined(DeeSysFS_Utf8TryGetEnvObject) || defined(DeeSysFS_WideTryGetEnvObject)
#define DEE_NFS_HAVE_GETENV
#endif

#if defined(DeeSysFS_Utf8HasEnv) || defined(DeeSysFS_WideHasEnv)\
 || defined(DeeSysFS_Utf8TryHasEnv) || defined(DeeSysFS_WideTryHasEnv)\
 || defined(DeeSysFS_Utf8HasEnvObject) || defined(DeeSysFS_WideHasEnvObject)\
 || defined(DeeSysFS_Utf8TryHasEnvObject) || defined(DeeSysFS_WideTryHasEnvObject)
#define DEE_NFS_HAVE_HASENV
#endif

#if defined(DeeSysFS_Utf8DelEnv) || defined(DeeSysFS_WideDelEnv)\
 || defined(DeeSysFS_Utf8TryDelEnv) || defined(DeeSysFS_WideTryDelEnv)\
 || defined(DeeSysFS_Utf8DelEnvObject) || defined(DeeSysFS_WideDelEnvObject)\
 || defined(DeeSysFS_Utf8TryDelEnvObject) || defined(DeeSysFS_WideTryDelEnvObject)
#define DEE_NFS_HAVE_DELENV
#endif

#if defined(DeeSysFS_Utf8SetEnv) || defined(DeeSysFS_WideSetEnv)\
 || defined(DeeSysFS_Utf8TrySetEnv) || defined(DeeSysFS_WideTrySetEnv)\
 || defined(DeeSysFS_Utf8SetEnvObject) || defined(DeeSysFS_WideSetEnvObject)\
 || defined(DeeSysFS_Utf8TrySetEnvObject) || defined(DeeSysFS_WideTrySetEnvObject)
#define DEE_NFS_HAVE_SETENV
#endif

#if defined(DeeSysFS_Utf8GetTimes) || defined(DeeSysFS_WideGetTimes)\
 || defined(DeeSysFS_Utf8GetTimesObject) || defined(DeeSysFS_WideGetTimesObject)\
 || defined(DeeSysFS_Utf8TryGetTimes) || defined(DeeSysFS_WideTryGetTimes)\
 || defined(DeeSysFS_Utf8TryGetTimesObject) || defined(DeeSysFS_WideTryGetTimesObject)\
 || defined(DeeSysFileFD_GetTimes) || defined(DeeSysFileFD_TryGetTimes)
#define DEE_NFS_HAVE_GETTIMES
#endif

#if defined(DeeSysFS_Utf8SetTimes) || defined(DeeSysFS_WideSetTimes)\
 || defined(DeeSysFS_Utf8SetTimesObject) || defined(DeeSysFS_WideSetTimesObject)\
 || defined(DeeSysFS_Utf8TrySetTimes) || defined(DeeSysFS_WideTrySetTimes)\
 || defined(DeeSysFS_Utf8TrySetTimesObject) || defined(DeeSysFS_WideTrySetTimesObject)\
 || defined(DeeSysFileFD_SetTimes) || defined(DeeSysFileFD_TrySetTimes)
#define DEE_NFS_HAVE_SETTIMES
#endif

#if defined(DeeSysFS_Utf8GetMod) || defined(DeeSysFS_WideGetMod)\
 || defined(DeeSysFS_Utf8GetModObject) || defined(DeeSysFS_WideGetModObject)\
 || defined(DeeSysFS_Utf8TryGetMod) || defined(DeeSysFS_WideTryGetMod)\
 || defined(DeeSysFS_Utf8TryGetModObject) || defined(DeeSysFS_WideTryGetModObject)\
 || defined(DeeSysFileFD_GetMod) || defined(DeeSysFileFD_TryGetMod)
#define DEE_NFS_HAVE_GETMOD
#endif

#if defined(DeeSysFS_Utf8Chmod) || defined(DeeSysFS_WideChmod)\
 || defined(DeeSysFS_Utf8ChmodObject) || defined(DeeSysFS_WideChmodObject)\
 || defined(DeeSysFS_Utf8TryChmod) || defined(DeeSysFS_WideTryChmod)\
 || defined(DeeSysFS_Utf8TryChmodObject) || defined(DeeSysFS_WideTryChmodObject)\
 || defined(DeeSysFileFD_Chmod) || defined(DeeSysFileFD_TryChmod)
#define DEE_NFS_HAVE_CHMOD
#endif

#if defined(DeeSysFS_Utf8GetOwn) || defined(DeeSysFS_WideGetOwn)\
 || defined(DeeSysFS_Utf8GetOwnObject) || defined(DeeSysFS_WideGetOwnObject)\
 || defined(DeeSysFS_Utf8TryGetOwn) || defined(DeeSysFS_WideTryGetOwn)\
 || defined(DeeSysFS_Utf8TryGetOwnObject) || defined(DeeSysFS_WideTryGetOwnObject)\
 || defined(DeeSysFileFD_GetOwn) || defined(DeeSysFileFD_TryGetOwn)
#define DEE_NFS_HAVE_GETOWN
#endif

#if defined(DeeSysFS_Utf8Chown) || defined(DeeSysFS_WideChown)\
 || defined(DeeSysFS_Utf8ChownObject) || defined(DeeSysFS_WideChownObject)\
 || defined(DeeSysFS_Utf8TryChown) || defined(DeeSysFS_WideTryChown)\
 || defined(DeeSysFS_Utf8TryChownObject) || defined(DeeSysFS_WideTryChownObject)\
 || defined(DeeSysFileFD_Chown) || defined(DeeSysFileFD_TryChown)
#define DEE_NFS_HAVE_CHOWN
#endif

#if defined(DeeSysFS_Utf8IsFile) || defined(DeeSysFS_WideIsFile)\
 || defined(DeeSysFS_Utf8IsFileObject) || defined(DeeSysFS_WideIsFileObject)\
 || defined(DeeSysFS_Utf8TryIsFile) || defined(DeeSysFS_WideTryIsFile)\
 || defined(DeeSysFS_Utf8TryIsFileObject) || defined(DeeSysFS_WideTryIsFileObject)\
 || defined(DeeSysFileFD_IsFile) || defined(DeeSysFileFD_TryIsFile)
#define DEE_NFS_HAVE_ISFILE
#endif

#if defined(DeeSysFS_Utf8IsDir) || defined(DeeSysFS_WideIsDir)\
 || defined(DeeSysFS_Utf8IsDirObject) || defined(DeeSysFS_WideIsDirObject)\
 || defined(DeeSysFS_Utf8TryIsDir) || defined(DeeSysFS_WideTryIsDir)\
 || defined(DeeSysFS_Utf8TryIsDirObject) || defined(DeeSysFS_WideTryIsDirObject)\
 || defined(DeeSysFileFD_IsDir) || defined(DeeSysFileFD_TryIsDir)
#define DEE_NFS_HAVE_ISDIR
#endif

#if defined(DeeSysFS_Utf8IsLink) || defined(DeeSysFS_WideIsLink)\
 || defined(DeeSysFS_Utf8IsLinkObject) || defined(DeeSysFS_WideIsLinkObject)\
 || defined(DeeSysFS_Utf8TryIsLink) || defined(DeeSysFS_WideTryIsLink)\
 || defined(DeeSysFS_Utf8TryIsLinkObject) || defined(DeeSysFS_WideTryIsLinkObject)\
 || defined(DeeSysFileFD_IsLink) || defined(DeeSysFileFD_TryIsLink)
#define DEE_NFS_HAVE_ISLINK
#endif

#if defined(DeeSysFS_Utf8IsDrive) || defined(DeeSysFS_WideIsDrive)\
 || defined(DeeSysFS_Utf8IsDriveObject) || defined(DeeSysFS_WideIsDriveObject)\
 || defined(DeeSysFS_Utf8TryIsDrive) || defined(DeeSysFS_WideTryIsDrive)\
 || defined(DeeSysFS_Utf8TryIsDriveObject) || defined(DeeSysFS_WideTryIsDriveObject)\
 || defined(DeeSysFileFD_IsDrive) || defined(DeeSysFileFD_TryIsDrive)
#define DEE_NFS_HAVE_ISDRIVE
#endif

#if defined(DeeSysFS_Utf8IsMount) || defined(DeeSysFS_WideIsMount)\
 || defined(DeeSysFS_Utf8IsMountObject) || defined(DeeSysFS_WideIsMountObject)\
 || defined(DeeSysFS_Utf8TryIsMount) || defined(DeeSysFS_WideTryIsMount)\
 || defined(DeeSysFS_Utf8TryIsMountObject) || defined(DeeSysFS_WideTryIsMountObject)\
 || defined(DeeSysFileFD_IsMount) || defined(DeeSysFileFD_TryIsMount)
#define DEE_NFS_HAVE_ISMOUNT
#endif

#if defined(DeeSysFS_Utf8IsHidden) || defined(DeeSysFS_WideIsHidden)\
 || defined(DeeSysFS_Utf8IsHiddenObject) || defined(DeeSysFS_WideIsHiddenObject)\
 || defined(DeeSysFS_Utf8TryIsHidden) || defined(DeeSysFS_WideTryIsHidden)\
 || defined(DeeSysFS_Utf8TryIsHiddenObject) || defined(DeeSysFS_WideTryIsHiddenObject)\
 || defined(DeeSysFileFD_IsHidden) || defined(DeeSysFileFD_TryIsHidden)
#define DEE_NFS_HAVE_ISHIDDEN
#endif

#if defined(DeeSysFS_Utf8IsExecutable) || defined(DeeSysFS_WideIsExecutable)\
 || defined(DeeSysFS_Utf8IsExecutableObject) || defined(DeeSysFS_WideIsExecutableObject)\
 || defined(DeeSysFS_Utf8TryIsExecutable) || defined(DeeSysFS_WideTryIsExecutable)\
 || defined(DeeSysFS_Utf8TryIsExecutableObject) || defined(DeeSysFS_WideTryIsExecutableObject)\
 || defined(DeeSysFileFD_IsExecutable) || defined(DeeSysFileFD_TryIsExecutable)
#define DEE_NFS_HAVE_ISEXECUTABLE
#endif

#if defined(DeeSysFS_Utf8IsCharDev) || defined(DeeSysFS_WideIsCharDev)\
 || defined(DeeSysFS_Utf8IsCharDevObject) || defined(DeeSysFS_WideIsCharDevObject)\
 || defined(DeeSysFS_Utf8TryIsCharDev) || defined(DeeSysFS_WideTryIsCharDev)\
 || defined(DeeSysFS_Utf8TryIsCharDevObject) || defined(DeeSysFS_WideTryIsCharDevObject)\
 || defined(DeeSysFileFD_IsCharDev) || defined(DeeSysFileFD_TryIsCharDev)
#define DEE_NFS_HAVE_ISCHARDEV
#endif

#if defined(DeeSysFS_Utf8IsBlockDev) || defined(DeeSysFS_WideIsBlockDev)\
 || defined(DeeSysFS_Utf8IsBlockDevObject) || defined(DeeSysFS_WideIsBlockDevObject)\
 || defined(DeeSysFS_Utf8TryIsBlockDev) || defined(DeeSysFS_WideTryIsBlockDev)\
 || defined(DeeSysFS_Utf8TryIsBlockDevObject) || defined(DeeSysFS_WideTryIsBlockDevObject)\
 || defined(DeeSysFileFD_IsBlockDev) || defined(DeeSysFileFD_TryIsBlockDev)
#define DEE_NFS_HAVE_ISBLOCKDEV
#endif

#if defined(DeeSysFS_Utf8IsFiFo) || defined(DeeSysFS_WideIsFiFo)\
 || defined(DeeSysFS_Utf8IsFiFoObject) || defined(DeeSysFS_WideIsFiFoObject)\
 || defined(DeeSysFS_Utf8TryIsFiFo) || defined(DeeSysFS_WideTryIsFiFo)\
 || defined(DeeSysFS_Utf8TryIsFiFoObject) || defined(DeeSysFS_WideTryIsFiFoObject)\
 || defined(DeeSysFileFD_IsFiFo) || defined(DeeSysFileFD_TryIsFiFo)
#define DEE_NFS_HAVE_ISFIFO
#endif

#if defined(DeeSysFS_Utf8IsSocket) || defined(DeeSysFS_WideIsSocket)\
 || defined(DeeSysFS_Utf8IsSocketObject) || defined(DeeSysFS_WideIsSocketObject)\
 || defined(DeeSysFS_Utf8TryIsSocket) || defined(DeeSysFS_WideTryIsSocket)\
 || defined(DeeSysFS_Utf8TryIsSocketObject) || defined(DeeSysFS_WideTryIsSocketObject)\
 || defined(DeeSysFileFD_IsSocket) || defined(DeeSysFileFD_TryIsSocket)
#define DEE_NFS_HAVE_ISSOCKET
#endif

//////////////////////////////////////////////////////////////////////////
// === FS_API Exports for linking against native filesystem APIS ===
// >> To check if a set of routines is available, use the macros defined above.
// >> Routines not available will throw an 'Error.RuntimeError.NotImplemented' when called.
//////////////////////////////////////////////////////////////////////////

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetCwd(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetCwd(void);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetHome(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetHome(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetUserHome(DEE_A_IN_Z Dee_Utf8Char const *username);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetUserHome(DEE_A_IN_Z Dee_WideChar const *username);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetUserHomeObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *username);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetUserHomeObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *username);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetTmp(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetTmp(void);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeNFS_Utf8EnumEnv(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeNFS_WideEnumEnv(void);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryHasEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryHasEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryHasEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryHasEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8GetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeNFS_WideGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8TryGetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *DeeNFS_WideTryGetEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeNFS_Utf8TryGetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *DeeNFS_WideTryGetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8DelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnv(DEE_A_IN_Z Dee_Utf8Char const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnv(DEE_A_IN_Z Dee_WideChar const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryDelEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryDelEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue);


extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TrySetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTrySetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetModObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetModObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetModObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetModObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChmodObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChmodObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChmodObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChmodObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryGetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChownObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChownObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryChownObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryChownObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);


extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsFile(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsFile(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsLink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDrive(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDrive(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMount(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMount(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsHiddenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsHiddenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsHidden(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsHidden(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsHiddenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsHiddenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsExecutableObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsExecutableObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsExecutable(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsExecutableObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsExecutableObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsCharDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsCharDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsCharDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsCharDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsCharDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsBlockDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsBlockDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsBlockDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsBlockDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFiFoObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFiFoObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsFiFo(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsFiFoObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsFiFoObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsSocketObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsSocketObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsSocket(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsSocket(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryIsSocketObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryIsSocketObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

#define DeeNFS_GetCwd                DeeNFS_Utf8GetCwd
#define DeeNFS_Chdir                 DeeNFS_Utf8Chdir
#define DeeNFS_ChdirObject           DeeNFS_Utf8ChdirObject
#define DeeNFS_TryChdir              DeeNFS_Utf8TryChdir
#define DeeNFS_TryChdirObject        DeeNFS_Utf8TryChdirObject
#define DeeNFS_GetHome               DeeNFS_Utf8GetHome
#define DeeNFS_GetUserHome           DeeNFS_Utf8GetUserHome
#define DeeNFS_GetUserHomeObject     DeeNFS_Utf8GetUserHomeObject
#define DeeNFS_GetTmp                DeeNFS_Utf8GetTmp
#define DeeNFS_EnumEnv               DeeNFS_Utf8EnumEnv
#define DeeNFS_HasEnv                DeeNFS_Utf8HasEnv
#define DeeNFS_HasEnvObject          DeeNFS_Utf8HasEnvObject
#define DeeNFS_TryHasEnv             DeeNFS_Utf8TryHasEnv
#define DeeNFS_TryHasEnvObject       DeeNFS_Utf8TryHasEnvObject
#define DeeNFS_GetEnv                DeeNFS_Utf8GetEnv
#define DeeNFS_GetEnvObject          DeeNFS_Utf8GetEnvObject
#define DeeNFS_TryGetEnv             DeeNFS_Utf8TryGetEnv
#define DeeNFS_TryGetEnvObject       DeeNFS_Utf8TryGetEnvObject
#define DeeNFS_DelEnv                DeeNFS_Utf8DelEnv
#define DeeNFS_DelEnvObject          DeeNFS_Utf8DelEnvObject
#define DeeNFS_TryDelEnv             DeeNFS_Utf8TryDelEnv
#define DeeNFS_TryDelEnvObject       DeeNFS_Utf8TryDelEnvObject
#define DeeNFS_SetEnv                DeeNFS_Utf8SetEnv
#define DeeNFS_SetEnvObject          DeeNFS_Utf8SetEnvObject
#define DeeNFS_TrySetEnv             DeeNFS_Utf8TrySetEnv
#define DeeNFS_TrySetEnvObject       DeeNFS_Utf8TrySetEnvObject
#define DeeNFS_GetTimes              DeeNFS_Utf8GetTimes
#define DeeNFS_GetTimesObject        DeeNFS_Utf8GetTimesObject
#define DeeNFS_TryGetTimes           DeeNFS_Utf8TryGetTimes
#define DeeNFS_TryGetTimesObject     DeeNFS_Utf8TryGetTimesObject
#define DeeNFS_SetTimes              DeeNFS_Utf8SetTimes
#define DeeNFS_SetTimesObject        DeeNFS_Utf8SetTimesObject
#define DeeNFS_TrySetTimes           DeeNFS_Utf8TrySetTimes
#define DeeNFS_TrySetTimesObject     DeeNFS_Utf8TrySetTimesObject
#define DeeNFS_GetMod                DeeNFS_Utf8GetMod
#define DeeNFS_GetModObject          DeeNFS_Utf8GetModObject
#define DeeNFS_TryGetMod             DeeNFS_Utf8TryGetMod
#define DeeNFS_TryGetModObject       DeeNFS_Utf8TryGetModObject
#define DeeNFS_Chmod                 DeeNFS_Utf8Chmod
#define DeeNFS_ChmodObject           DeeNFS_Utf8ChmodObject
#define DeeNFS_TryChmod              DeeNFS_Utf8TryChmod
#define DeeNFS_TryChmodObject        DeeNFS_Utf8TryChmodObject
#define DeeNFS_GetOwn                DeeNFS_Utf8GetOwn
#define DeeNFS_GetOwnObject          DeeNFS_Utf8GetOwnObject
#define DeeNFS_TryGetOwn             DeeNFS_Utf8TryGetOwn
#define DeeNFS_TryGetOwnObject       DeeNFS_Utf8TryGetOwnObject
#define DeeNFS_Chown                 DeeNFS_Utf8Chown
#define DeeNFS_ChownObject           DeeNFS_Utf8ChownObject
#define DeeNFS_TryChown              DeeNFS_Utf8TryChown
#define DeeNFS_TryChownObject        DeeNFS_Utf8TryChownObject
#define DeeNFS_IsFile                DeeNFS_Utf8IsFile
#define DeeNFS_IsFileObject          DeeNFS_Utf8IsFileObject
#define DeeNFS_TryIsFile             DeeNFS_Utf8TryIsFile
#define DeeNFS_TryIsFileObject       DeeNFS_Utf8TryIsFileObject
#define DeeNFS_IsDir                 DeeNFS_Utf8IsDir
#define DeeNFS_IsDirObject           DeeNFS_Utf8IsDirObject
#define DeeNFS_TryIsDir              DeeNFS_Utf8TryIsDir
#define DeeNFS_TryIsDirObject        DeeNFS_Utf8TryIsDirObject
#define DeeNFS_IsLink                DeeNFS_Utf8IsLink
#define DeeNFS_IsLinkObject          DeeNFS_Utf8IsLinkObject
#define DeeNFS_TryIsLink             DeeNFS_Utf8TryIsLink
#define DeeNFS_TryIsLinkObject       DeeNFS_Utf8TryIsLinkObject
#define DeeNFS_IsDrive               DeeNFS_Utf8IsDrive
#define DeeNFS_IsDriveObject         DeeNFS_Utf8IsDriveObject
#define DeeNFS_TryIsDrive            DeeNFS_Utf8TryIsDrive
#define DeeNFS_TryIsDriveObject      DeeNFS_Utf8TryIsDriveObject
#define DeeNFS_IsMount               DeeNFS_Utf8IsMount
#define DeeNFS_IsMountObject         DeeNFS_Utf8IsMountObject
#define DeeNFS_TryIsMount            DeeNFS_Utf8TryIsMount
#define DeeNFS_TryIsMountObject      DeeNFS_Utf8TryIsMountObject
#define DeeNFS_IsHidden              DeeNFS_Utf8IsHidden
#define DeeNFS_IsHiddenObject        DeeNFS_Utf8IsHiddenObject
#define DeeNFS_TryIsHidden           DeeNFS_Utf8TryIsHidden
#define DeeNFS_TryIsHiddenObject     DeeNFS_Utf8TryIsHiddenObject
#define DeeNFS_IsExecutable          DeeNFS_Utf8IsExecutable
#define DeeNFS_IsExecutableObject    DeeNFS_Utf8IsExecutableObject
#define DeeNFS_TryIsExecutable       DeeNFS_Utf8TryIsExecutable
#define DeeNFS_TryIsExecutableObject DeeNFS_Utf8TryIsExecutableObject
#define DeeNFS_IsCharDev             DeeNFS_Utf8IsCharDev
#define DeeNFS_IsCharDevObject       DeeNFS_Utf8IsCharDevObject
#define DeeNFS_TryIsCharDev          DeeNFS_Utf8TryIsCharDev
#define DeeNFS_TryIsCharDevObject    DeeNFS_Utf8TryIsCharDevObject
#define DeeNFS_IsBlockDev            DeeNFS_Utf8IsBlockDev
#define DeeNFS_IsBlockDevObject      DeeNFS_Utf8IsBlockDevObject
#define DeeNFS_TryIsBlockDev         DeeNFS_Utf8TryIsBlockDev
#define DeeNFS_TryIsBlockDevObject   DeeNFS_Utf8TryIsBlockDevObject
#define DeeNFS_IsFiFo                DeeNFS_Utf8IsFiFo
#define DeeNFS_IsFiFoObject          DeeNFS_Utf8IsFiFoObject
#define DeeNFS_TryIsFiFo             DeeNFS_Utf8TryIsFiFo
#define DeeNFS_TryIsFiFoObject       DeeNFS_Utf8TryIsFiFoObject
#define DeeNFS_IsSocket              DeeNFS_Utf8IsSocket
#define DeeNFS_IsSocketObject        DeeNFS_Utf8IsSocketObject
#define DeeNFS_TryIsSocket           DeeNFS_Utf8TryIsSocket
#define DeeNFS_TryIsSocketObject     DeeNFS_Utf8TryIsSocketObject

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_H */
