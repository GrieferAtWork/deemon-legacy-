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
#ifndef GUARD_DEEMON_FS_EXPAND_FS_H
#define GUARD_DEEMON_FS_EXPAND_FS_H 1

#include <deemon/__conf.inl>
#include <deemon/fs_api.h>

DEE_DECL_BEGIN

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryChdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryChdir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8ChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryChdirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryChdirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8GetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8GetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryGetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryGetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryGetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryGetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8SetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideSetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8SetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideSetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TrySetTimes(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTrySetTimes(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TrySetTimesObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTrySetTimesObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);

extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXFS_Utf8HasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXFS_WideHasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXFS_Utf8HasPropertyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXFS_WideHasPropertyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryHasProperty(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryHasProperty(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryHasPropertyObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryHasPropertyObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_fileproperty_t prop);

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryRemove(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryRemove(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryRemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryUnlink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryUnlink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryUnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8RmDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideRmDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8RmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryRmDir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryRmDir(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryRmDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryRmDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);

extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8MkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8MkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_EXCEPT(-1) int DeeXFS_WideMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryMkDir(DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryMkDir(DEE_A_IN_Z Dee_WideChar const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryMkDirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_IN Dee_mode_t mode);
extern DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryMkDirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_IN Dee_mode_t mode);

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_EXPAND_FS_H */
