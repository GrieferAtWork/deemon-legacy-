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

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Copy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8CopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryCopy(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryCopy(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryCopyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryCopyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8MoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryMove(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryMoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst);

extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8LinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryLink(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name);
extern DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name);

#define DeeVFS_GetCwd                DeeVFS_Utf8GetCwd
#define DeeVFS_Chdir                 DeeVFS_Utf8Chdir
#define DeeVFS_ChdirObject           DeeVFS_Utf8ChdirObject
#define DeeVFS_TryChdir              DeeVFS_Utf8TryChdir
#define DeeVFS_TryChdirObject        DeeVFS_Utf8TryChdirObject
#define DeeVFS_GetTimes              DeeVFS_Utf8GetTimes
#define DeeVFS_GetTimesObject        DeeVFS_Utf8GetTimesObject
#define DeeVFS_TryGetTimes           DeeVFS_Utf8TryGetTimes
#define DeeVFS_TryGetTimesObject     DeeVFS_Utf8TryGetTimesObject
#define DeeVFS_SetTimes              DeeVFS_Utf8SetTimes
#define DeeVFS_SetTimesObject        DeeVFS_Utf8SetTimesObject
#define DeeVFS_TrySetTimes           DeeVFS_Utf8TrySetTimes
#define DeeVFS_TrySetTimesObject     DeeVFS_Utf8TrySetTimesObject
#define DeeVFS_HasProperty           DeeVFS_Utf8HasProperty
#define DeeVFS_HasPropertyObject     DeeVFS_Utf8HasPropertyObject
#define DeeVFS_TryHasProperty        DeeVFS_Utf8TryHasProperty
#define DeeVFS_TryHasPropertyObject  DeeVFS_Utf8TryHasPropertyObject
#define DeeVFS_Remove                DeeVFS_Utf8Remove
#define DeeVFS_RemoveObject          DeeVFS_Utf8RemoveObject
#define DeeVFS_TryRemove             DeeVFS_Utf8TryRemove
#define DeeVFS_TryRemoveObject       DeeVFS_Utf8TryRemoveObject
#define DeeVFS_Unlink                DeeVFS_Utf8Unlink
#define DeeVFS_UnlinkObject          DeeVFS_Utf8UnlinkObject
#define DeeVFS_TryUnlink             DeeVFS_Utf8TryUnlink
#define DeeVFS_TryUnlinkObject       DeeVFS_Utf8TryUnlinkObject
#define DeeVFS_RmDir                 DeeVFS_Utf8RmDir
#define DeeVFS_RmDirObject           DeeVFS_Utf8RmDirObject
#define DeeVFS_TryRmDir              DeeVFS_Utf8TryRmDir
#define DeeVFS_TryRmDirObject        DeeVFS_Utf8TryRmDirObject
#define DeeVFS_MkDir                 DeeVFS_Utf8MkDir
#define DeeVFS_MkDirObject           DeeVFS_Utf8MkDirObject
#define DeeVFS_TryMkDir              DeeVFS_Utf8TryMkDir
#define DeeVFS_TryMkDirObject        DeeVFS_Utf8TryMkDirObject
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

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_H */
