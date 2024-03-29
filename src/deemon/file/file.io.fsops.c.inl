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
#ifndef GUARD_DEEMON_FILE_IO_FSOPS_C_INL
#define GUARD_DEEMON_FILE_IO_FSOPS_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include "file.io.h"
#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/time.h>
#include <deemon/fs_api.h>
#include <deemon/sys/sysfd.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN


#ifdef DEE_PLATFORM_WINDOWS
#define DeeFileIO_FastFilename DeeFileIO_WideFilename
#define DeeNFS_FastF(x)        DeeNFS_Wide##x
#else
#define DeeFileIO_FastFilename DeeFileIO_Filename
#define DeeNFS_FastF(x)        DeeNFS_##x
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
DeeFileIO_GetTimes(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *a,*c,*m,*result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,&a,&c,&m) != 0) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetATime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,&result,NULL,NULL) != 0) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetCTime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,NULL,&result,NULL) != 0) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetMTime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,NULL,NULL,&result) != 0) return NULL;
 return result;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int DeeFileIO_SetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_modification) {
 Dee_timetick_t ttm_access,ttm_creation,ttm_modification;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
#ifdef DeeSysFileFD_SetTimes
 DeeFileIO_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
#endif
 if (tm_access) ttm_access = DeeTime_GetTick(tm_access);
 if (tm_creation) ttm_creation = DeeTime_GetTick(tm_creation);
 if (tm_modification) ttm_modification = DeeTime_GetTick(tm_modification);
 {
#ifdef DeeSysFileFD_SetTimes
  DeeSysFileFD_SetTimes(&((DeeFileIOObject *)self)->io_descr,
                        tm_access ? &ttm_access : NULL,
                        tm_creation ? &ttm_creation : NULL,
                        tm_modification ? &ttm_modification : NULL,{
                         DeeFileIO_RELEASE_SHARED(self);
                         return -1;
                        });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(SetTimesObject)(filename,
                                       tm_access ? &ttm_access : NULL,
                                       tm_creation ? &ttm_creation : NULL,
                                       tm_modification ? &ttm_modification : NULL);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
 Dee_timetick_t ttm_access,ttm_creation,ttm_modification;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_GetTimes
  DeeFileIO_ACQUIRE_SHARED(self,{
   DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
   return -1;
  });
  DeeSysFileFD_GetTimes(&((DeeFileIOObject *)self)->io_descr,
                        tm_access ? &ttm_access : NULL,
                        tm_creation ? &ttm_creation : NULL,
                        tm_modification ? &ttm_modification : NULL,{
                         DeeFileIO_RELEASE_SHARED(self);
                         return -1;
                        });
  DeeFileIO_RELEASE_SHARED(self);
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(GetTimesObject)(filename,tm_access,tm_creation,tm_modification);
  Dee_DECREF(filename);
  if (error != 0) return error;
#endif
 }
 if (tm_access && DEE_UNLIKELY((*tm_access = DeeTime_New(ttm_access)) == NULL)) return -1;
 if (tm_creation && DEE_UNLIKELY((*tm_creation = DeeTime_New(ttm_creation)) == NULL)) {err_atm: if (tm_access) Dee_DECREF(*tm_access); return -1; }
 if (tm_modification && DEE_UNLIKELY((*tm_modification = DeeTime_New(ttm_modification)) == NULL)) {/*err_mtm:*/ if (tm_creation) Dee_DECREF(*tm_creation); goto err_atm; }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP



DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsFile(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsFile
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsFile(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsFileObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsDir(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsDir
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsDir(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsDirObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsLink(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsLink
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsLink(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsLinkObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsDrive(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsDrive
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsDrive(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsDriveObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsMount(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsMount
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsMount(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsMountObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsHidden(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsHidden
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsHidden(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsHiddenObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsExecutable(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_IsExecutable
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsExecutable(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsExecutableObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsCharDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#if defined(DEE_PLATFORM_WINDOWS) && !defined(FILE_TYPE_CHAR)
  (void)self;
  return 0;
#elif defined(DeeSysFileFD_IsCharDev)
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsCharDev(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsCharDevObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsBlockDev(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#if defined(DEE_PLATFORM_WINDOWS) && !defined(FILE_TYPE_DISK)
  (void)self;
  return 0;
#elif defined(DeeSysFileFD_IsBlockDev)
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsBlockDev(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsBlockDevObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsFiFo(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#if defined(DEE_PLATFORM_WINDOWS) && !defined(FILE_TYPE_PIPE)
  (void)self;
  return 0;
#elif defined(DeeSysFileFD_IsFiFo)
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsFiFo(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsFiFoObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeFileIO_IsSocket(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#if defined(DEE_PLATFORM_WINDOWS) && !defined(FILE_TYPE_REMOTE)
  (void)self;
  return 0;
#elif defined(DeeSysFileFD_IsSocket)
  int result;
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_IsSocket(&((DeeFileIOObject *)self)->io_descr,&result,{ DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return result;
#else // Fallback: Implement with file name
  DeeObject *filename; int error;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  error = DeeNFS_FastF(IsSocketObject)(filename);
  Dee_DECREF(filename);
  return error;
#endif
 }
}







DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetMod(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self, DEE_A_OUT Dee_mode_t *mode) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_GetMod
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_GetMod(&((DeeFileIOObject *)self)->io_descr,mode,
                      { DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  int result;
  if DEE_UNLIKELY((self = DeeFileIO_FastFilename(self)) == NULL) return -1;
  result = DeeNFS_FastF(GetModObject)(self,mode);
  Dee_DECREF(self);
  return result;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_Chmod(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN Dee_mode_t mode) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 {
#ifdef DeeSysFileFD_Chmod
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_Chmod(&((DeeFileIOObject *)self)->io_descr,mode,
                     { DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  DeeObject *filename; int result;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  result = DeeNFS_FastF(ChmodObject)(filename,mode);
  Dee_DECREF(filename);
  return result;
#endif
 }
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeFileIO_ChmodEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self,
 DEE_A_IN Dee_size_t mcc, DEE_A_IN_R(mcc) Dee_modechange_t const *mcv) {
 Dee_mode_t newmode;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(mcc); DEE_ASSERT(mcv);
 {
#if defined(DeeSysFileFD_GetMod) && defined(DeeSysFileFD_Chmod)
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET) {
   DeeSysFileFD_GetMod(&((DeeFileIOObject *)self)->io_descr,&newmode,
                       { DeeFileIO_RELEASE_SHARED(self); return -1; });
  }
  _DeeFS_ExecModeChange(&newmode,mcc,mcv);
  DeeSysFileFD_Chmod(&((DeeFileIOObject *)self)->io_descr,newmode,
                     { DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  DeeObject *filename; int result;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  if (DEE_MODECHANGE_KIND(mcv[0]) != DEE_MODECHANGE_KIND_SET &&
      DeeNFS_FastF(GetModObject)(filename,&newmode) != 0) return -1;
  _DeeFS_ExecModeChange(&newmode,mcc,mcv);
  result = DeeNFS_FastF(ChmodObject)(filename,newmode);
  Dee_DECREF(filename);
  return result;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetOwn(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(owner),DEE_ASSERT(group);
 {
#ifdef DeeSysFileFD_GetOwn
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_GetOwn(&((DeeFileIOObject *)self)->io_descr,owner,group,
                      { DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  int result;
  if DEE_UNLIKELY((self = DeeFileIO_FastFilename(self)) == NULL) return -1;
  result = DeeNFS_FastF(GetOwnObject)(self,owner,group);
  Dee_DECREF(self);
  return result;
#endif
 }
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_Chown(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(owner),DEE_ASSERT(group);
 {
#ifdef DeeSysFileFD_Chown
  DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
  DeeSysFileFD_Chown(&((DeeFileIOObject *)self)->io_descr,owner,group,
                     { DeeFileIO_RELEASE_SHARED(self); return -1; });
  DeeFileIO_RELEASE_SHARED(self);
  return 0;
#else // Fallback: Implement with file name
  DeeObject *filename; int result;
  if DEE_UNLIKELY((filename = DeeFileIO_FastFilename(self)) == NULL) return -1;
  result = DeeNFS_FastF(ChownObject)(filename,owner,group);
  Dee_DECREF(filename);
  return result;
#endif
 }
}

#undef DeeFileIO_FastFilename
#undef DeeNFS_FastF

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_IO_FSOPS_C_INL */
