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
#include <deemon/optional/fs_api.fileproperty.h>
#include <deemon/sys/sysfs.view.h>
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
 || defined(DeeSysFS_Utf8ChdirObject) || defined(DeeSysFS_WideChdirObject)
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
 || defined(DeeSysFS_Utf8HasEnvObject) || defined(DeeSysFS_WideHasEnvObject)
#define DEE_NFS_HAVE_HASENV
#endif

#if defined(DeeSysFS_Utf8DelEnv) || defined(DeeSysFS_WideDelEnv)\
 || defined(DeeSysFS_Utf8DelEnvObject) || defined(DeeSysFS_WideDelEnvObject)
#define DEE_NFS_HAVE_DELENV
#endif

#if defined(DeeSysFS_Utf8SetEnv) || defined(DeeSysFS_WideSetEnv)\
 || defined(DeeSysFS_Utf8SetEnvObject) || defined(DeeSysFS_WideSetEnvObject)
#define DEE_NFS_HAVE_SETENV
#endif

#if defined(DeeSysFS_Utf8GetTimes) || defined(DeeSysFS_WideGetTimes)\
 || defined(DeeSysFS_Utf8GetTimesObject) || defined(DeeSysFS_WideGetTimesObject)\
 ||(defined(DeeSysFileFD_GetTimes) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_GETTIMES
#endif

#if defined(DeeSysFS_Utf8SetTimes) || defined(DeeSysFS_WideSetTimes)\
 || defined(DeeSysFS_Utf8SetTimesObject) || defined(DeeSysFS_WideSetTimesObject)\
 ||(defined(DeeSysFileFD_SetTimes) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_SETTIMES
#endif

#if defined(DeeSysFS_Utf8GetMod) || defined(DeeSysFS_WideGetMod)\
 || defined(DeeSysFS_Utf8GetModObject) || defined(DeeSysFS_WideGetModObject)\
 ||(defined(DeeSysFileFD_GetMod) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_GETMOD
#endif

#if defined(DeeSysFS_Utf8Chmod) || defined(DeeSysFS_WideChmod)\
 || defined(DeeSysFS_Utf8ChmodObject) || defined(DeeSysFS_WideChmodObject)\
 ||(defined(DeeSysFileFD_Chmod) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_CHMOD
#endif

#if defined(DeeSysFS_Utf8GetOwn) || defined(DeeSysFS_WideGetOwn)\
 || defined(DeeSysFS_Utf8GetOwnObject) || defined(DeeSysFS_WideGetOwnObject)\
 ||(defined(DeeSysFileFD_GetOwn) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_GETOWN
#endif

#if defined(DeeSysFS_Utf8Chown) || defined(DeeSysFS_WideChown)\
 || defined(DeeSysFS_Utf8ChownObject) || defined(DeeSysFS_WideChownObject)\
 ||(defined(DeeSysFileFD_Chown) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_CHOWN
#endif

#if defined(DeeSysFS_Utf8IsFile) || defined(DeeSysFS_WideIsFile)\
 || defined(DeeSysFS_Utf8IsFileObject) || defined(DeeSysFS_WideIsFileObject)\
 ||(defined(DeeSysFileFD_IsFile) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISFILE
#endif

#if defined(DeeSysFS_Utf8IsDir) || defined(DeeSysFS_WideIsDir)\
 || defined(DeeSysFS_Utf8IsDirObject) || defined(DeeSysFS_WideIsDirObject)\
 ||(defined(DeeSysFileFD_IsDir) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISDIR
#endif

#if defined(DeeSysFS_Utf8IsLink) || defined(DeeSysFS_WideIsLink)\
 || defined(DeeSysFS_Utf8IsLinkObject) || defined(DeeSysFS_WideIsLinkObject)\
 ||(defined(DeeSysFileFD_IsLink) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISLINK
#endif

#if defined(DeeSysFS_Utf8IsDrive) || defined(DeeSysFS_WideIsDrive)\
 || defined(DeeSysFS_Utf8IsDriveObject) || defined(DeeSysFS_WideIsDriveObject)\
 ||(defined(DeeSysFileFD_IsDrive) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISDRIVE
#endif

#if defined(DeeSysFS_Utf8IsMount) || defined(DeeSysFS_WideIsMount)\
 || defined(DeeSysFS_Utf8IsMountObject) || defined(DeeSysFS_WideIsMountObject)\
 ||(defined(DeeSysFileFD_IsMount) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISMOUNT
#endif

#if defined(DeeSysFS_Utf8IsHidden) || defined(DeeSysFS_WideIsHidden)\
 || defined(DeeSysFS_Utf8IsHiddenObject) || defined(DeeSysFS_WideIsHiddenObject)\
 ||(defined(DeeSysFileFD_IsHidden) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISHIDDEN
#endif

#if defined(DeeSysFS_Utf8IsExecutable) || defined(DeeSysFS_WideIsExecutable)\
 || defined(DeeSysFS_Utf8IsExecutableObject) || defined(DeeSysFS_WideIsExecutableObject)\
 ||(defined(DeeSysFileFD_IsExecutable) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISEXECUTABLE
#endif

#if defined(DeeSysFS_Utf8IsCharDev) || defined(DeeSysFS_WideIsCharDev)\
 || defined(DeeSysFS_Utf8IsCharDevObject) || defined(DeeSysFS_WideIsCharDevObject)\
 ||(defined(DeeSysFileFD_IsCharDev) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISCHARDEV
#endif

#if defined(DeeSysFS_Utf8IsBlockDev) || defined(DeeSysFS_WideIsBlockDev)\
 || defined(DeeSysFS_Utf8IsBlockDevObject) || defined(DeeSysFS_WideIsBlockDevObject)\
 ||(defined(DeeSysFileFD_IsBlockDev) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISBLOCKDEV
#endif

#if defined(DeeSysFS_Utf8IsFiFo) || defined(DeeSysFS_WideIsFiFo)\
 || defined(DeeSysFS_Utf8IsFiFoObject) || defined(DeeSysFS_WideIsFiFoObject)\
 ||(defined(DeeSysFileFD_IsFiFo) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISFIFO
#endif

#if defined(DeeSysFS_Utf8IsSocket) || defined(DeeSysFS_WideIsSocket)\
 || defined(DeeSysFS_Utf8IsSocketObject) || defined(DeeSysFS_WideIsSocketObject)\
 ||(defined(DeeSysFileFD_IsSocket) && (defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject)))
#define DEE_NFS_HAVE_ISSOCKET
#endif

#if defined(DeeSysFS_Utf8IsAbs) || defined(DeeSysFS_WideIsAbs)\
 || defined(DeeSysFS_Utf8IsAbsObject) || defined(DeeSysFS_WideIsAbsObject)
#define DEE_NFS_HAVE_ISABS
#endif

#if defined(DeeSysFS_Utf8Exists) || defined(DeeSysFS_WideExists)\
 || defined(DeeSysFS_Utf8ExistsObject) || defined(DeeSysFS_WideExistsObject)\
 ||(defined(DeeSysFileFD_Utf8InitObject) || defined(DeeSysFileFD_WideInitObject))
#define DEE_NFS_HAVE_EXISTS
#endif

#if  defined(DeeSysFS_Utf8Remove) || defined(DeeSysFS_WideRemove)\
 ||  defined(DeeSysFS_Utf8RemoveObject) || defined(DeeSysFS_WideRemoveObject)
#define DEE_NFS_HAVE_OSREMOVE
#endif

#if  defined(DEE_NFS_HAVE_OSREMOVE)\
 || (defined(DeeSysFS_Utf8Unlink) && defined(DeeSysFS_Utf8RmDir))\
 || (defined(DeeSysFS_WideUnlink) && defined(DeeSysFS_WideRmDir))\
 || (defined(DeeSysFS_Utf8UnlinkObject) && defined(DeeSysFS_Utf8RmDirObject))\
 || (defined(DeeSysFS_WideUnlinkObject) && defined(DeeSysFS_WideRmDirObject))
#define DEE_NFS_HAVE_REMOVE /*< We can (and do) simulate remove with try_unlink+rmdir/try_rmdir+unlink. */
#endif

#if defined(DeeSysFS_Utf8Unlink) || defined(DeeSysFS_WideUnlink)\
 || defined(DeeSysFS_Utf8UnlinkObject) || defined(DeeSysFS_WideUnlinkObject)
#define DEE_NFS_HAVE_UNLINK
#endif

#if defined(DeeSysFS_Utf8RmDir) || defined(DeeSysFS_WideRmDir)\
 || defined(DeeSysFS_Utf8RmDirObject) || defined(DeeSysFS_WideRmDirObject)
#define DEE_NFS_HAVE_RMDIR
#endif

#if defined(DeeSysFS_Utf8MkDir) || defined(DeeSysFS_WideMkDir)\
 || defined(DeeSysFS_Utf8MkDirObject) || defined(DeeSysFS_WideMkDirObject)
#define DEE_NFS_HAVE_MKDIR
#endif

#if defined(DeeSysFS_Utf8Copy) || defined(DeeSysFS_WideCopy)\
 || defined(DeeSysFS_Utf8CopyObject) || defined(DeeSysFS_WideCopyObject)
#define DEE_NFS_HAVE_OSCOPY
#endif
#if defined(DEE_NFS_HAVE_OSCOPY)\
 ||(defined(DeeSysFileFD) && (defined(DeeSysFileFD_Read) || defined(DeeSysFD_Read))\
                          && (defined(DeeSysFileFD_Write) || defined(DeeSysFD_Write)))
#define DEE_NFS_HAVE_COPY /*< We can (and do) simulate copy through sysfd. */
#endif

#if defined(DeeSysFS_Utf8Move) || defined(DeeSysFS_WideMove)\
 || defined(DeeSysFS_Utf8MoveObject) || defined(DeeSysFS_WideMoveObject)
#define DEE_NFS_HAVE_MOVE
#endif

#if defined(DeeSysFS_Utf8Link) || defined(DeeSysFS_WideLink)\
 || defined(DeeSysFS_Utf8LinkObject) || defined(DeeSysFS_WideLinkObject)
#define DEE_NFS_HAVE_LINK
#endif

#if defined(DeeSysFSUtf8View) || defined(DeeSysFSWideView)
#define DEE_NFS_HAVE_OPENDIR
#endif

#if defined(DeeSysFileFD)
#define DEE_NFS_HAVE_OPEN
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
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnv(DEE_A_IN_Z Dee_Utf8Char const *envname, DEE_A_IN_Z Dee_Utf8Char const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnv(DEE_A_IN_Z Dee_WideChar const *envname, DEE_A_IN_Z Dee_WideChar const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetEnvObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envname, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *envvalue);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetEnvObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *envname, DEE_A_IN_OBJECT(DeeWideStringObject) const *envvalue);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetModObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetModObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChmodObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChmodObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8GetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8ChownObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChownObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8HasPropertyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideHasPropertyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFileObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFileObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDrive(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDrive(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsDriveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsDriveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsMountObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsMountObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsHiddenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsHiddenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsExecutableObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsExecutableObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsCharDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsCharDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsBlockDevObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsBlockDevObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsFiFoObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsFiFoObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsSocketObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsSocketObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsAbs(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsAbs(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8IsAbsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideIsAbsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8ExistsObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_WideExistsObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8RmDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRmDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8RmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8MkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8MoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);

extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8LinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name);

extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Opendir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpendirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpendirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8Open(DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpen(DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_Utf8OpenObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeNFS_WideOpenObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions);


#define DeeNFS_GetCwd                DeeNFS_Utf8GetCwd
#define DeeNFS_Chdir                 DeeNFS_Utf8Chdir
#define DeeNFS_ChdirObject           DeeNFS_Utf8ChdirObject
#define DeeNFS_GetHome               DeeNFS_Utf8GetHome
#define DeeNFS_GetUserHome           DeeNFS_Utf8GetUserHome
#define DeeNFS_GetUserHomeObject     DeeNFS_Utf8GetUserHomeObject
#define DeeNFS_GetTmp                DeeNFS_Utf8GetTmp
#define DeeNFS_EnumEnv               DeeNFS_Utf8EnumEnv
#define DeeNFS_HasEnv                DeeNFS_Utf8HasEnv
#define DeeNFS_HasEnvObject          DeeNFS_Utf8HasEnvObject
#define DeeNFS_GetEnv                DeeNFS_Utf8GetEnv
#define DeeNFS_GetEnvObject          DeeNFS_Utf8GetEnvObject
#define DeeNFS_TryGetEnv             DeeNFS_Utf8TryGetEnv
#define DeeNFS_TryGetEnvObject       DeeNFS_Utf8TryGetEnvObject
#define DeeNFS_DelEnv                DeeNFS_Utf8DelEnv
#define DeeNFS_DelEnvObject          DeeNFS_Utf8DelEnvObject
#define DeeNFS_SetEnv                DeeNFS_Utf8SetEnv
#define DeeNFS_SetEnvObject          DeeNFS_Utf8SetEnvObject
#define DeeNFS_GetTimes              DeeNFS_Utf8GetTimes
#define DeeNFS_GetTimesObject        DeeNFS_Utf8GetTimesObject
#define DeeNFS_SetTimes              DeeNFS_Utf8SetTimes
#define DeeNFS_SetTimesObject        DeeNFS_Utf8SetTimesObject
#define DeeNFS_GetMod                DeeNFS_Utf8GetMod
#define DeeNFS_GetModObject          DeeNFS_Utf8GetModObject
#define DeeNFS_Chmod                 DeeNFS_Utf8Chmod
#define DeeNFS_ChmodObject           DeeNFS_Utf8ChmodObject
#define DeeNFS_GetOwn                DeeNFS_Utf8GetOwn
#define DeeNFS_GetOwnObject          DeeNFS_Utf8GetOwnObject
#define DeeNFS_Chown                 DeeNFS_Utf8Chown
#define DeeNFS_ChownObject           DeeNFS_Utf8ChownObject
#define DeeNFS_IsFile                DeeNFS_Utf8IsFile
#define DeeNFS_IsFileObject          DeeNFS_Utf8IsFileObject
#define DeeNFS_IsDir                 DeeNFS_Utf8IsDir
#define DeeNFS_IsDirObject           DeeNFS_Utf8IsDirObject
#define DeeNFS_IsLink                DeeNFS_Utf8IsLink
#define DeeNFS_IsLinkObject          DeeNFS_Utf8IsLinkObject
#define DeeNFS_IsDrive               DeeNFS_Utf8IsDrive
#define DeeNFS_IsDriveObject         DeeNFS_Utf8IsDriveObject
#define DeeNFS_IsMount               DeeNFS_Utf8IsMount
#define DeeNFS_IsMountObject         DeeNFS_Utf8IsMountObject
#define DeeNFS_IsHidden              DeeNFS_Utf8IsHidden
#define DeeNFS_IsHiddenObject        DeeNFS_Utf8IsHiddenObject
#define DeeNFS_IsExecutable          DeeNFS_Utf8IsExecutable
#define DeeNFS_IsExecutableObject    DeeNFS_Utf8IsExecutableObject
#define DeeNFS_IsCharDev             DeeNFS_Utf8IsCharDev
#define DeeNFS_IsCharDevObject       DeeNFS_Utf8IsCharDevObject
#define DeeNFS_IsBlockDev            DeeNFS_Utf8IsBlockDev
#define DeeNFS_IsBlockDevObject      DeeNFS_Utf8IsBlockDevObject
#define DeeNFS_IsFiFo                DeeNFS_Utf8IsFiFo
#define DeeNFS_IsFiFoObject          DeeNFS_Utf8IsFiFoObject
#define DeeNFS_IsSocket              DeeNFS_Utf8IsSocket
#define DeeNFS_IsSocketObject        DeeNFS_Utf8IsSocketObject
#define DeeNFS_IsAbs                 DeeNFS_Utf8IsAbs
#define DeeNFS_IsAbsObject           DeeNFS_Utf8IsAbsObject
#define DeeNFS_Exists                DeeNFS_Utf8Exists
#define DeeNFS_ExistsObject          DeeNFS_Utf8ExistsObject
#define DeeNFS_Remove                DeeNFS_Utf8Remove
#define DeeNFS_RemoveObject          DeeNFS_Utf8RemoveObject
#define DeeNFS_Unlink                DeeNFS_Utf8Unlink
#define DeeNFS_UnlinkObject          DeeNFS_Utf8UnlinkObject
#define DeeNFS_RmDir                 DeeNFS_Utf8RmDir
#define DeeNFS_RmDirObject           DeeNFS_Utf8RmDirObject
#define DeeNFS_MkDir                 DeeNFS_Utf8MkDir
#define DeeNFS_MkDirObject           DeeNFS_Utf8MkDirObject
#define DeeNFS_Copy                  DeeNFS_Utf8Copy
#define DeeNFS_CopyObject            DeeNFS_Utf8CopyObject
#define DeeNFS_Move                  DeeNFS_Utf8Move
#define DeeNFS_MoveObject            DeeNFS_Utf8MoveObject
#define DeeNFS_Link                  DeeNFS_Utf8Link
#define DeeNFS_LinkObject            DeeNFS_Utf8LinkObject
#define DeeNFS_Opendir               DeeNFS_Utf8Opendir
#define DeeNFS_OpendirObject         DeeNFS_Utf8OpendirObject


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_H */
