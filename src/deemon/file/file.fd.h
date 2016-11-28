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
#ifndef GUARD_DEEMON_FILE_FD_H
#define GUARD_DEEMON_FILE_FD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/sys/sysfd.h>

DEE_DECL_BEGIN

#ifndef DEE_SYSFD_SEEK_SET
DeeError_NEW_STATIC(_unsupported_seek_set,&DeeErrorType_NotImplemented,"SEEK_SET is not supported");
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_SET(whence,...) if(whence==DEE_SEEK_SET){DeeError_Throw((DeeObject *)&_unsupported_seek_set);{__VA_ARGS__;}};else
#elif DEE_SYSFD_SEEK_SET != DEE_SEEK_SET
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_SET(whence,...) if(whence==DEE_SEEK_SET)whence=DEE_SYSFD_SEEK_SET;else
#else
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_SET(whence,...) /* nothing */
#endif

#ifndef DEE_SYSFD_SEEK_CUR
DeeError_NEW_STATIC(_unsupported_seek_cur,&DeeErrorType_NotImplemented,"SEEK_CUR is not supported");
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_CUR(whence,...) if(whence==DEE_SEEK_CUR){DeeError_Throw((DeeObject *)&_unsupported_seek_cur);{__VA_ARGS__;}};else
#elif DEE_SYSFD_SEEK_CUR != DEE_SEEK_CUR
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_CUR(whence,...) if(whence==DEE_SEEK_CUR)whence=DEE_SYSFD_SEEK_CUR;else
#else
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_CUR(whence,...) /* nothing */
#endif

#ifndef DEE_SYSFD_SEEK_END
DeeError_NEW_STATIC(_unsupported_seek_end,&DeeErrorType_NotImplemented,"SEEK_END is not supported");
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_END(whence,...) if(whence==DEE_SEEK_END){DeeError_Throw((DeeObject *)&_unsupported_seek_end);{__VA_ARGS__;}};else
#elif DEE_SYSFD_SEEK_END != DEE_SEEK_END
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_END(whence,...) if(whence==DEE_SEEK_END)whence=DEE_SYSFD_SEEK_END;else
#else
# define DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_END(whence,...) /* nothing */
#endif

#define DEE_FILEFD_FIX_SEEKWHENCE(whence,...) \
do{\
 DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_SET((whence),__VA_ARGS__)\
 DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_CUR((whence),__VA_ARGS__)\
 DEE_PRIVATE_FILEFD_FIX_SEEKWHENCE_END((whence),__VA_ARGS__);\
}while(0)



#ifdef DEE_LIMITED_DEX
#ifdef DeeSysFD
#define DEE_FILE_FD_OBJECT_HEAD \
 DEE_FILE_OBJECT_HEAD \
 Dee_uint32_t    fd_users; /*< [atomic,inc:lock(fo_lock)] Amount of threads currently using this file-descriptor (incrementing requires a lock to 'fo_lock'). */
#else
#define DEE_FILE_FD_OBJECT_HEAD DEE_FILE_OBJECT_HEAD
#endif

struct DeeFileFDObject {
 DEE_FILE_FD_OBJECT_HEAD
#ifdef DeeSysFD
 struct DeeSysFD fd_descr; /*< File descriptor (only writable when 'fo_lock' is owned and 'fd_users' is '0') */
#endif
};

#ifdef DeeSysFD
# define DeeFileFDObject_INIT_REF_INVALID(tp,ref)      {DEE_OBJECT_HEAD_INIT_REF(tp,ref),DeeAtomicMutex_INIT(),DEE_PRIVATE_FILEFLAG_NONE,0,/*{ don't initialize }*/}
# define DeeFileFDObject_INIT_REF_LATER(tp,ref)        {DEE_OBJECT_HEAD_INIT_REF(tp,ref),DeeAtomicMutex_INIT(),DEE_PRIVATE_FILEFLAG_FD_VALID,0,/*{ don't initialize }*/}
# define DeeFileFDObject_INIT_REF_VALID(tp,ref,descr)  {DEE_OBJECT_HEAD_INIT_REF(tp,ref),DeeAtomicMutex_INIT(),DEE_PRIVATE_FILEFLAG_FD_VALID,0,descr}
# define DeeFileFDObject_INIT_REF_OWNED(tp,ref,descr)  {DEE_OBJECT_HEAD_INIT_REF(tp,ref),DeeAtomicMutex_INIT(),DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED,0,descr}
#else
# define DeeFileFDObject_INIT_REF_INVALID(tp,ref)      {DEE_OBJECT_HEAD_INIT_REF(tp,ref),DeeAtomicMutex_INIT(),DEE_PRIVATE_FILEFLAG_NONE}
#endif


#ifdef DeeSysFD
#define DeeFileFD_InitBasic(ob,flags)\
 (DeeAtomicMutex_Init(&(ob)->fo_lock)\
 ,(ob)->fo_flags = (flags),(ob)->fd_users = 0)
#else
#define DeeFileFD_InitBasic(ob,flags)\
 (DeeAtomicMutex_Init(&(ob)->fo_lock)\
 ,(ob)->fo_flags = (flags))
#endif

//////////////////////////////////////////////////////////////////////////
// Acquire shared access to a given file descriptor
// >> You, and any other thread is allowed to execute any system-operation
//    on the descriptor, knowing that the descriptor will not just randomly
//    be closed before you release the your ticket.
// >> The provided block of code is executed if the descriptor is (no longer) valid.
#ifdef DeeSysFD
#define DeeFileFD_ACQUIRE_SHARED(ob,...)\
do{\
 DeeFile_ACQUIRE(ob);\
 if ((((DeeFileFDObject *)(ob))->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID)==0)\
 { DeeFile_RELEASE(ob); {__VA_ARGS__;} } else {\
  DeeAtomic32_FetchInc(((DeeFileFDObject *)(ob))->fd_users,memory_order_seq_cst);\
  DeeFile_RELEASE(ob);\
 }\
}while(0)
#define DeeFileFD_RELEASE_SHARED(ob)\
 DeeAtomic32_FetchDec(((DeeFileFDObject *)(ob))->fd_users,memory_order_seq_cst)
#else
#define DeeFileFD_ACQUIRE_SHARED(ob,...) do{}while(0)
#define DeeFileFD_RELEASE_SHARED(ob)     (void)0
#endif

//////////////////////////////////////////////////////////////////////////
// Acquire exclusive access to a given file descriptor
// >> Only you are allowed to operate on a given descriptor.
//    Such a ticket is required for closing a descriptor.
// >> The provided block of code is executed if the descriptor is (no longer) valid.
#ifdef DeeSysFD
#define DeeFileFD_ACQUIRE_EXCLUSIVE(ob,...)\
do{\
 DeeFile_ACQUIRE(ob);\
 while (1) {\
  if (((ob)->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID)==0)\
  { DeeFile_RELEASE(ob); {__VA_ARGS__;} break; }\
  /* NOTE: No need for atomically loading 'fd_users' \
           here (compare against 0 - quirk) */\
  if ((ob)->fd_users == 0) break; /*< We did it! */\
  DeeFile_RELEASE(ob);\
  /* Sleep a bit while waiting for other threads \
     to finish (and hope this isn't a deadlock...). */\
  DeeThread_SleepNoInterrupt(1);\
  DeeFile_ACQUIRE(ob);\
 }\
}while(0)
#define DeeFileFD_RELEASE_EXCLUSIVE(ob) DeeFile_RELEASE(ob)
#else /* DeeSysFD */
#define DeeFileFD_ACQUIRE_EXCLUSIVE(ob,...) do{}while(0)
#define DeeFileFD_RELEASE_EXCLUSIVE(ob) (void)0
#endif /* !DeeSysFD */

#if defined(DeeSysFD) || DEE_CONFIG_RUNTIME_HAVE_VFS
DeeError_NEW_STATIC(_DeeErrorInstance_FileFDAlreadyClosed,&DeeErrorType_IOError,
                    "Invalid/Closed file descriptor");
#define DeeErrorInstance_FileFDAlreadyClosed ((DeeObject *)&_DeeErrorInstance_FileFDAlreadyClosed)
#endif /* DeeSysFD */


#define _pdeefilefd_tp_ctor &_deefilefd_tp_ctor
extern int DEE_CALL _deefilefd_tp_ctor(DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self);

#ifdef DeeSysFD_InitCopy /*< The system supports fd duplication. */
#define _pdeefilefd_tp_copy_ctor &_deefilefd_tp_copy_ctor
extern int DEE_CALL _deefilefd_tp_copy_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self, DeeFileFDObject *right);
#else
#define _pdeefilefd_tp_copy_ctor DeeType_DEFAULT_SLOT(tp_copy_ctor)
#endif
#define _pdeefilefd_tp_move_ctor &_deefilefd_tp_move_ctor
extern int DEE_CALL _deefilefd_tp_move_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self, DeeFileFDObject *right);
#if (defined(DEE_PLATFORM_WINDOWS)\
  || defined(DEE_PLATFORM_UNIX))\
  && defined(DeeSysFD)
#define _pdeefilefd_tp_any_ctor &_deefilefd_tp_any_ctor
extern int DEE_CALL _deefilefd_tp_any_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self, DeeObject *args);
#else
#define _pdeefilefd_tp_any_ctor DeeType_DEFAULT_SLOT(tp_any_ctor)
#endif

#ifdef DeeSysFD_Quit
#define _pdeefilefd_tp_dtor &_deefilefd_tp_dtor
extern void DEE_CALL _deefilefd_tp_dtor(DeeFileFDObject *self);
#else
#define _pdeefilefd_tp_dtor DeeType_DEFAULT_SLOT(tp_dtor)
#endif

#define _pdeefilefd_tp_move_assign &_deefilefd_tp_move_assign
extern int DEE_CALL _deefilefd_tp_move_assign(
 DeeFileFDObject *self, DeeFileFDObject *right);
#ifdef DeeSysFD_InitCopy
#define _pdeefilefd_tp_copy_assign &_deefilefd_tp_copy_assign
extern int DEE_CALL _deefilefd_tp_copy_assign(
 DeeFileFDObject *self, DeeFileFDObject *right);
#else
#define _pdeefilefd_tp_copy_assign DeeType_DEFAULT_SLOT(tp_copy_assign)
#endif

#ifdef DEE_PLATFORM_UNIX
#define _pdeefilefd_tp_str &_deefilefd_tp_str
extern DeeObject *_deefilefd_tp_str(DeeFileFDObject *self);
#else
#define _pdeefilefd_tp_str  DeeType_DEFAULT_SLOT(tp_str)
#endif
#endif


DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_FD_H */
