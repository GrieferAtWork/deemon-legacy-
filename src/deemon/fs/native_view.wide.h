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
#ifndef GUARD_DEEMON_FS_NATIVE_VIEW_WIDE_H
#define GUARD_DEEMON_FS_NATIVE_VIEW_WIDE_H 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/type.h>
#include <deemon/string.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_decl.h>
#include <deemon/fs/native_fs.h>
#include <deemon/sys/sysfs.view.h>

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// NOTE: Changes to the code below can be mirrored to
//       "native_view.utf8.h" by running 'deemon -F native_new.utf8.h'
//////////////////////////////////////////////////////////////////////////

//@DeeSysFSWideView:begin
#ifdef DeeSysFSWideView
#if defined(DeeSysFSWideView_GetPath)
#define DEE_NFSWIDEVIEW_HAVE_OSGETPATH
#endif

#if defined(DeeSysFSWideView_Release)
#define DEE_NFSWIDEVIEW_HAVE_LOCK
#endif

#define DeeNFSWideView DeeNFSWideView
struct DeeNFSWideView {
#ifndef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
 DEE_A_REF DeeWideStringObject *nwv_path; /*< [1..1][const] Path supplied to the underlying view. */
#endif
 struct DeeSysFSWideView        nwv_view; /*< Underlying view structure. */
};

#ifdef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
#ifdef DeeSysFSWideView_GetPath
# define DeeNFSWideView_GetPath(self)  DeeSysFSWideView_GetPath(&(self)->nwv_view)
#endif
#else
# define DeeNFSWideView_GetPath(self) (Dee_IncRef((DeeObject *)(self)->nwv_path),(DeeObject *)(self)->nwv_path)
#endif

#ifdef DeeSysFSWideView_Quit
#define DeeNFSWideView_PRIVATE_QUIT(self) DeeSysFSWideView_Quit(&(self)->nwv_view)
#else
#define DeeNFSWideView_PRIVATE_QUIT(self) do{}while(0)
#endif
#ifdef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
# define DeeNFSWideView_Quit DeeNFSWideView_PRIVATE_QUIT
#else
# define DeeNFSWideView_Quit(self) do{Dee_DECREF((self)->nwv_path);DeeNFSWideView_PRIVATE_QUIT(self);}while(0)
#endif


#ifdef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
#define DeeNFSWideView_InitObject(self,path,...)\
do{DeeSysFSWideView_InitObject(&(self)->nwv_view,path,__VA_ARGS__);}while(0)
#ifdef DeeSysFSWideView_Init
# define DeeNFSWideView_Init(self,path,...)\
 do{DeeSysFSWideView_Init(&(self)->nwv_view,path,__VA_ARGS__);}while(0)
#else
# define DeeNFSWideView_Init(self,path,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeWideString_New(path)) == NULL){__VA_ARGS__;}\
  DeeSysFSWideView_InitObject(&(self)->nwv_view,_nv_path_ob,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#else
#define DeeNFSWideView_InitObject(self,path,...)\
do{DeeSysFSWideView_InitObject(&(self)->nwv_view,path,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeWideStringObject *)(path));}while(0)
#define DeeNFSWideView_Init(self,path,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeWideStringObject *)DeeWideString_New(path)) == NULL){__VA_ARGS__;}\
 DeeSysFSWideView_InitObject(&(self)->nwv_view,(self)->nwv_path,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#endif

#define DeeNFSWideView_Acquire(self) DeeSysFSWideView_Acquire(&(self)->nwv_view)
#define DeeNFSWideView_Release(self) DeeSysFSWideView_Release(&(self)->nwv_view)
#define DeeNFSWideView_IsDone(self)  DeeSysFSWideView_IsDone(&(self)->nwv_view)
#define DeeNFSWideView_AdvanceAndReleaseOnError(self,...)    DeeSysFSWideView_AdvanceAndReleaseOnError(&(self)->nwv_view,__VA_ARGS__)

#if defined(DeeSysFSWideView_TryGetFilenameObjectLocked)
#define DeeNFSWideView_TryGetFilenameObjectLocked DeeNFSWideView_TryGetFilenameObjectLocked
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetFilenameObjectLocked(struct DeeNFSWideView const *self) {
 DeeObject *result;
 result = DeeSysFSWideView_TryGetFilenameObjectLocked(&self->nwv_view)
 return result;
}
#elif defined(DeeSysFSWideView_GetFilenameStrLocked)
#define DeeNFSWideView_TryGetFilenameObjectLocked DeeNFSWideView_TryGetFilenameObjectLocked
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetFilenameObjectLocked(struct DeeNFSWideView const *self) {
 DeeObject *result; Dee_WideChar const *filename; Dee_size_t filename_size;
 filename = DeeSysFSWideView_GetFilenameStrLocked(&self->nwv_view);
#ifndef DEE_SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_NEVER_NULL
 if (!filename) DeeReturn_EmptyWideString;
#endif
#ifdef DeeSysFSWideView_GetFilenameSizeLocked
 filename_size = DeeSysFSWideView_GetFilenameSizeLocked(&self->nwv_view);
#else
 filename_size = Dee_WideStrLen(filename);
#endif
 result = (DeeObject *)DeeObject_TryWeakMalloc(Dee_OFFSETOF(DeeWideStringObject,s_str)+
                                               (filename_size+1)*sizeof(Dee_WideChar));
 if DEE_UNLIKELY(!result) return NULL;
 DeeObject_INIT(result,&DeeWideString_Type);
 ((DeeWideStringObject *)result)->s_len = filename_size;
 memcpy(((DeeWideStringObject *)result)->s_str,filename,filename_size*sizeof(Dee_WideChar));
 ((DeeWideStringObject *)result)->s_str[filename_size] = 0;
 return result;
}
#endif

#ifdef DeeSysFSWideView_GetFilenameObject
# define DeeNFSWideView_GetFilenameObject(self) DeeSysFSWideView_GetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSWideView_TryGetFilenameObjectLocked)
#define DeeNFSWideView_GetFilenameObject DeeNFSWideView_GetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_GetFilenameObject(struct DeeNFSWideView const *self) {
 DeeObject *result;
 DeeNFSWideView_Acquire(self);
 result = DeeSysFSWideView_TryGetFilenameObjectLocked(&self->nwv_view)
 DeeNFSWideView_Release(self);
 return result;
}
#elif defined(DeeSysFSWideView_GetFilenameStrLocked)
#define DeeNFSWideView_GetFilenameObject DeeNFSWideView_GetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_GetFilenameObject(struct DeeNFSWideView const *self) {
 DeeObject *result; Dee_WideChar const *filename; Dee_size_t filename_size;
again:
 DeeNFSWideView_Acquire(self);
 filename = DeeSysFSWideView_GetFilenameStrLocked(&self->nwv_view);
#ifndef DEE_SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_NEVER_NULL
 if (!filename) { DeeNFSWideView_Release(self); DeeReturn_EmptyWideString; }
#endif
#ifdef DeeSysFSWideView_GetFilenameSizeLocked
 filename_size = DeeSysFSWideView_GetFilenameSizeLocked(&self->nwv_view);
#else
 filename_size = Dee_WideStrLen(filename);
#endif
 result = (DeeObject *)DeeObject_TryWeakMalloc(Dee_OFFSETOF(DeeWideStringObject,s_str)+
                                               (filename_size+1)*sizeof(Dee_WideChar));
 if DEE_UNLIKELY(!result) {
  DeeNFSWideView_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 DeeObject_INIT(result,&DeeWideString_Type);
 ((DeeWideStringObject *)result)->s_len = filename_size;
 memcpy(((DeeWideStringObject *)result)->s_str,filename,filename_size*sizeof(Dee_WideChar));
 ((DeeWideStringObject *)result)->s_str[filename_size] = 0;
 DeeNFSWideView_Release(self);
 return result;
}
#endif

// v Should never happen, but lets be careful.
#if !defined(DeeNFSWideView_GetFilenameObject)\
 || !defined(DeeNFSWideView_TryGetFilenameObjectLocked)
#error "No way of retrieving filename from DeeNFSWideView!"
#endif

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_GetAbsoluteFilename(DEE_A_IN struct DeeNFSWideView const *self) {
 DeeObject *path,*filename,*result; Dee_WideChar path_end; Dee_size_t path_size;
 if DEE_UNLIKELY((path = DeeNFSWideView_GetPath(self)) == NULL) return NULL;
 if DEE_UNLIKELY((filename = DeeNFSWideView_GetFilenameObject(self)) == NULL) { Dee_DECREF(path); return NULL; }
 if DEE_UNLIKELY(!DeeWideString_SIZE(path)) { Dee_DECREF(path); return filename; }
 path_size = DeeWideString_SIZE(path);
 path_end = DeeWideString_STR(path)[path_size-1];
 if (path_end == '\\' || path_end == '/') {
  result = DeeWideString_ConcatWithLength(path,DeeWideString_SIZE(filename),
                                          DeeWideString_STR(filename));
 } else if ((result = DeeWideString_NewSized(path_size+1+DeeWideString_SIZE(filename))) != NULL) {
  memcpy(DeeWideString_STR(result),DeeWideString_STR(path),path_size);
#ifdef DEE_PLATFORM_WINDOWS
  DeeWideString_STR(result)[path_size] = '\\';
#else
  DeeWideString_STR(result)[path_size] = '/';
#endif
  memcpy(DeeWideString_STR(result)+(path_size+1),
         DeeWideString_STR(filename),DeeWideString_SIZE(filename));
 }
 Dee_DECREF(filename);
 Dee_DECREF(path);
 return result;
}
#define DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,nfs_function,result,...) \
do{\
 DeeObject *_wv_path;\
 if ((_wv_path = DeeNFSWideView_GetAbsoluteFilename(self)) == NULL) {__VA_ARGS__;}\
 *(result) = nfs_function(_wv_path);\
 Dee_DECREF(_wv_path);\
 if (*(result) < 0){__VA_ARGS__;}\
}while(0)

#ifdef DeeSysFSWideView_IsFile
#define DeeNFSWideView_IsFile(self,result,...) DeeSysFSWideView_IsFile(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsFile(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsFileObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsDir
#define DeeNFSWideView_IsDir(self,result,...) DeeSysFSWideView_IsDir(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsDir(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsDirObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsLink
#define DeeNFSWideView_IsLink(self,result,...) DeeSysFSWideView_IsLink(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsLink(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsLinkObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsDrive
#define DeeNFSWideView_IsDrive(self,result,...) DeeSysFSWideView_IsDrive(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsDrive(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsDriveObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsMount
#define DeeNFSWideView_IsMount(self,result,...) DeeSysFSWideView_IsMount(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsMount(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsMountObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsHidden
#define DeeNFSWideView_IsHidden(self,result,...) DeeSysFSWideView_IsHidden(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsHidden(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsHiddenObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsExecutable
#define DeeNFSWideView_IsExecutable(self,result,...) DeeSysFSWideView_IsExecutable(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsExecutable(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsExecutableObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsCharDev
#define DeeNFSWideView_IsCharDev(self,result,...) DeeSysFSWideView_IsCharDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsCharDev(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsCharDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsBlockDev
#define DeeNFSWideView_IsBlockDev(self,result,...) DeeSysFSWideView_IsBlockDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsBlockDev(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsBlockDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsFiFo
#define DeeNFSWideView_IsFiFo(self,result,...) DeeSysFSWideView_IsFiFo(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsFiFo(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsFiFoObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_IsSocket
#define DeeNFSWideView_IsSocket(self,result,...) DeeSysFSWideView_IsSocket(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_IsSocket(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsSocketObject,result,__VA_ARGS__)
#endif

#endif /* DeeSysFSWideView */
//@DeeSysFSWideView:end


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_WIDE_H */
