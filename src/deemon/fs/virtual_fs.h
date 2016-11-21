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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_H
#define GUARD_DEEMON_FS_VIRTUAL_FS_H 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/optional/fs_api.fileproperty.h>
#include <deemon/vfs/vfs_core.h>

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

//////////////////////////////////////////////////////////////////////////
// === FS_API Exports for linking against native filesystem APIS ===
// >> To check if a set of routines is available, use the macros defined above.
// >> Routines not available will throw an 'Error.RuntimeError.NotImplemented' when called.
//////////////////////////////////////////////////////////////////////////

#if 0 /*< Declared in 'vfs_core.h' */
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeVFS_Utf8GetCwd(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeVFS_WideGetCwd(void);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path);
#endif
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TrySetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTrySetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TrySetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTrySetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_Utf8HasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_WideHasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_Utf8HasPropertyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_WideHasPropertyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryHasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryHasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryHasPropertyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryHasPropertyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryRemove(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryRemove(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryRemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryUnlink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryUnlink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryUnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8RmDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRmDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8RmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryRmDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryRmDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryRmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8MkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryMkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryMkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

#if 0

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetModObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetModObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetMod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetMod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetModObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetModObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_mode_t *mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Chmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8ChmodObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChmodObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChmod(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChmod(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChmodObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChmodObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetOwn(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetOwn(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetOwnObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetOwnObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Chown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8ChownObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChownObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChown(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChown(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryChownObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryChownObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
#endif

#define DeeVFS_GetCwd                DeeVFS_Utf8GetCwd
#define DeeVFS_Chdir                 DeeVFS_Utf8Chdir
#define DeeVFS_ChdirObject           DeeVFS_Utf8ChdirObject
#define DeeVFS_TryChdir              DeeVFS_Utf8TryChdir
#define DeeVFS_TryChdirObject        DeeVFS_Utf8TryChdirObject

#if 0 /*< TODO */
#define DeeVFS_GetHome               DeeVFS_Utf8GetHome
#define DeeVFS_GetUserHome           DeeVFS_Utf8GetUserHome
#define DeeVFS_GetUserHomeObject     DeeVFS_Utf8GetUserHomeObject
#define DeeVFS_GetTmp                DeeVFS_Utf8GetTmp
#define DeeVFS_ListEnv               DeeVFS_Utf8ListEnv
#define DeeVFS_HasEnv                DeeVFS_Utf8HasEnv
#define DeeVFS_HasEnvObject          DeeVFS_Utf8HasEnvObject
#define DeeVFS_TryHasEnv             DeeVFS_Utf8TryHasEnv
#define DeeVFS_TryHasEnvObject       DeeVFS_Utf8TryHasEnvObject
#define DeeVFS_GetEnv                DeeVFS_Utf8GetEnv
#define DeeVFS_GetEnvObject          DeeVFS_Utf8GetEnvObject
#define DeeVFS_TryGetEnv             DeeVFS_Utf8TryGetEnv
#define DeeVFS_TryGetEnvObject       DeeVFS_Utf8TryGetEnvObject
#define DeeVFS_DelEnv                DeeVFS_Utf8DelEnv
#define DeeVFS_DelEnvObject          DeeVFS_Utf8DelEnvObject
#define DeeVFS_TryDelEnv             DeeVFS_Utf8TryDelEnv
#define DeeVFS_TryDelEnvObject       DeeVFS_Utf8TryDelEnvObject
#define DeeVFS_SetEnv                DeeVFS_Utf8SetEnv
#define DeeVFS_SetEnvObject          DeeVFS_Utf8SetEnvObject
#define DeeVFS_TrySetEnv             DeeVFS_Utf8TrySetEnv
#define DeeVFS_TrySetEnvObject       DeeVFS_Utf8TrySetEnvObject
#define DeeVFS_GetTimes              DeeVFS_Utf8GetTimes
#define DeeVFS_GetTimesObject        DeeVFS_Utf8GetTimesObject
#define DeeVFS_TryGetTimes           DeeVFS_Utf8TryGetTimes
#define DeeVFS_TryGetTimesObject     DeeVFS_Utf8TryGetTimesObject
#define DeeVFS_SetTimes              DeeVFS_Utf8SetTimes
#define DeeVFS_SetTimesObject        DeeVFS_Utf8SetTimesObject
#define DeeVFS_TrySetTimes           DeeVFS_Utf8TrySetTimes
#define DeeVFS_TrySetTimesObject     DeeVFS_Utf8TrySetTimesObject
#define DeeVFS_GetMod                DeeVFS_Utf8GetMod
#define DeeVFS_GetModObject          DeeVFS_Utf8GetModObject
#define DeeVFS_TryGetMod             DeeVFS_Utf8TryGetMod
#define DeeVFS_TryGetModObject       DeeVFS_Utf8TryGetModObject
#define DeeVFS_Chmod                 DeeVFS_Utf8Chmod
#define DeeVFS_ChmodObject           DeeVFS_Utf8ChmodObject
#define DeeVFS_TryChmod              DeeVFS_Utf8TryChmod
#define DeeVFS_TryChmodObject        DeeVFS_Utf8TryChmodObject
#define DeeVFS_GetOwn                DeeVFS_Utf8GetOwn
#define DeeVFS_GetOwnObject          DeeVFS_Utf8GetOwnObject
#define DeeVFS_TryGetOwn             DeeVFS_Utf8TryGetOwn
#define DeeVFS_TryGetOwnObject       DeeVFS_Utf8TryGetOwnObject
#define DeeVFS_Chown                 DeeVFS_Utf8Chown
#define DeeVFS_ChownObject           DeeVFS_Utf8ChownObject
#define DeeVFS_TryChown              DeeVFS_Utf8TryChown
#define DeeVFS_TryChownObject        DeeVFS_Utf8TryChownObject
#define DeeVFS_IsFile                DeeVFS_Utf8IsFile
#define DeeVFS_IsFileObject          DeeVFS_Utf8IsFileObject
#define DeeVFS_TryIsFile             DeeVFS_Utf8TryIsFile
#define DeeVFS_TryIsFileObject       DeeVFS_Utf8TryIsFileObject
#define DeeVFS_IsDir                 DeeVFS_Utf8IsDir
#define DeeVFS_IsDirObject           DeeVFS_Utf8IsDirObject
#define DeeVFS_TryIsDir              DeeVFS_Utf8TryIsDir
#define DeeVFS_TryIsDirObject        DeeVFS_Utf8TryIsDirObject
#define DeeVFS_IsLink                DeeVFS_Utf8IsLink
#define DeeVFS_IsLinkObject          DeeVFS_Utf8IsLinkObject
#define DeeVFS_TryIsLink             DeeVFS_Utf8TryIsLink
#define DeeVFS_TryIsLinkObject       DeeVFS_Utf8TryIsLinkObject
#define DeeVFS_IsDrive               DeeVFS_Utf8IsDrive
#define DeeVFS_IsDriveObject         DeeVFS_Utf8IsDriveObject
#define DeeVFS_TryIsDrive            DeeVFS_Utf8TryIsDrive
#define DeeVFS_TryIsDriveObject      DeeVFS_Utf8TryIsDriveObject
#define DeeVFS_IsMount               DeeVFS_Utf8IsMount
#define DeeVFS_IsMountObject         DeeVFS_Utf8IsMountObject
#define DeeVFS_TryIsMount            DeeVFS_Utf8TryIsMount
#define DeeVFS_TryIsMountObject      DeeVFS_Utf8TryIsMountObject
#define DeeVFS_IsHidden              DeeVFS_Utf8IsHidden
#define DeeVFS_IsHiddenObject        DeeVFS_Utf8IsHiddenObject
#define DeeVFS_TryIsHidden           DeeVFS_Utf8TryIsHidden
#define DeeVFS_TryIsHiddenObject     DeeVFS_Utf8TryIsHiddenObject
#define DeeVFS_IsExecutable          DeeVFS_Utf8IsExecutable
#define DeeVFS_IsExecutableObject    DeeVFS_Utf8IsExecutableObject
#define DeeVFS_TryIsExecutable       DeeVFS_Utf8TryIsExecutable
#define DeeVFS_TryIsExecutableObject DeeVFS_Utf8TryIsExecutableObject
#define DeeVFS_IsCharDev             DeeVFS_Utf8IsCharDev
#define DeeVFS_IsCharDevObject       DeeVFS_Utf8IsCharDevObject
#define DeeVFS_TryIsCharDev          DeeVFS_Utf8TryIsCharDev
#define DeeVFS_TryIsCharDevObject    DeeVFS_Utf8TryIsCharDevObject
#define DeeVFS_IsBlockDev            DeeVFS_Utf8IsBlockDev
#define DeeVFS_IsBlockDevObject      DeeVFS_Utf8IsBlockDevObject
#define DeeVFS_TryIsBlockDev         DeeVFS_Utf8TryIsBlockDev
#define DeeVFS_TryIsBlockDevObject   DeeVFS_Utf8TryIsBlockDevObject
#define DeeVFS_IsFiFo                DeeVFS_Utf8IsFiFo
#define DeeVFS_IsFiFoObject          DeeVFS_Utf8IsFiFoObject
#define DeeVFS_TryIsFiFo             DeeVFS_Utf8TryIsFiFo
#define DeeVFS_TryIsFiFoObject       DeeVFS_Utf8TryIsFiFoObject
#define DeeVFS_IsSocket              DeeVFS_Utf8IsSocket
#define DeeVFS_IsSocketObject        DeeVFS_Utf8IsSocketObject
#define DeeVFS_TryIsSocket           DeeVFS_Utf8TryIsSocket
#define DeeVFS_TryIsSocketObject     DeeVFS_Utf8TryIsSocketObject
#endif

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_H */
