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
#if defined(DeeSysFSWideView_TryGetPath)\
 || defined(DeeSysFSWideView_GetPath)
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
# define DeeNFSWideView_GetPath(self)    DeeSysFSWideView_GetPath(&(self)->nwv_view)
#endif
#ifdef DeeSysFSWideView_TryGetPath
# define DeeNFSWideView_TryGetPath(self) DeeSysFSWideView_TryGetPath(&(self)->nwv_view)
#else
DEE_STATIC_INLINE(DeeObject *) DeeNFSWideView_TryGetPath(struct DeeNFSWideView *self) {
 DeeObject *result;
 if ((result = DeeNFSWideView_GetPath(self)) == NULL) DeeError_HandledOne();
 return result;
}
#endif
#else
# define DeeNFSWideView_TryGetPath(self) (Dee_IncRef((DeeObject *)(self)->nwv_path),(DeeObject *)(self)->nwv_path)
#endif
#ifndef DeeNFSWideView_GetPath
# define DeeNFSWideView_GetPath DeeNFSWideView_TryGetPath
#endif

#if defined(DeeSysFSWideView_IsEmpty) && defined(DeeSysFSWideView_Quit)
#define DeeNFSWideView_PRIVATE_QUIT(self) \
do{ if (!DeeSysFSWideView_IsEmpty(&(self)->nwv_view)) DeeSysFSWideView_Quit(&(self)->nwv_view); }while(0)
#elif defined(DeeSysFSWideView_Quit)
#define DeeNFSWideView_PRIVATE_QUIT(self) \
do{ DeeSysFSWideView_Quit(&(self)->nwv_view); }while(0)
#else
#define DeeNFSWideView_PRIVATE_QUIT(self) do{}while(0)
#endif

#ifdef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
# define DeeNFSWideView_Quit DeeNFSWideView_PRIVATE_QUIT
#else
# define DeeNFSWideView_Quit(self) do{Dee_DECREF(&(self)->nwv_path);DeeNFSWideView_PRIVATE_QUIT(self);}while(0)
#endif


#ifdef DEE_NFSWIDEVIEW_HAVE_OSGETPATH
#define DeeNFSWideView_TryInitObject(self,path,on_empty,...)\
do{DeeSysFSWideView_TryInitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#define DeeNFSWideView_InitObject(self,path,on_empty,...)\
do{DeeSysFSWideView_InitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#ifdef DeeSysFSWideView_TryInit
# define DeeNFSWideView_TryInit(self,path,on_empty,...)\
 do{DeeSysFSWideView_TryInit(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#else
# define DeeNFSWideView_TryInit(self,path,on_empty,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeWideString_New(path)) == NULL){DeeError_HandledOne();{__VA_ARGS__;}}\
  DeeSysFSWideView_TryInitObject(&(self)->nwv_view,_nv_path_ob,on_empty,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#ifdef DeeSysFSWideView_Init
# define DeeNFSWideView_Init(self,path,on_empty,...)\
 do{DeeSysFSWideView_Init(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#else
# define DeeNFSWideView_Init(self,path,on_empty,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeWideString_New(path)) == NULL){__VA_ARGS__;}\
  DeeSysFSWideView_InitObject(&(self)->nwv_view,_nv_path_ob,on_empty,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#else
#define DeeNFSWideView_TryInitObject(self,path,on_empty,...)\
do{DeeSysFSWideView_TryInitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeWideStringObject *)(path));}while(0)
#define DeeNFSWideView_InitObject(self,path,on_empty,...)\
do{DeeSysFSWideView_InitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeWideStringObject *)(path));}while(0)
#define DeeNFSWideView_TryInit(self,path,on_empty,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeWideStringObject *)DeeWideString_New(path)) == NULL){DeeError_HandledOne();{__VA_ARGS__;}}\
 DeeSysFSWideView_TryInitObject(&(self)->nwv_view,(self)->nwv_path,on_empty,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#define DeeNFSWideView_Init(self,path,on_empty,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeWideStringObject *)DeeWideString_New(path)) == NULL){__VA_ARGS__;}\
 DeeSysFSWideView_InitObject(&(self)->nwv_view,(self)->nwv_path,on_empty,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#endif

#ifdef DEE_NFSWIDEVIEW_HAVE_LOCK
#ifdef DeeSysFSWideView_TryAdvanceAndAcquire
# define DeeNFSWideView_TryAdvanceAndAcquire(self,on_done,...) DeeSysFSWideView_TryAdvanceAndAcquire(&(self)->nwv_view,on_done,__VA_ARGS__)
#elif defined(DeeSysFSWideView_AdvanceAndAcquire)
# define DeeNFSWideView_TryAdvanceAndAcquire(self,on_done,...) DeeSysFSWideView_AdvanceAndAcquire(&(self)->nwv_view,on_done,{DeeError_HandledOne();{__VA_ARGS__};})
#else
# define DeeNFSWideView_TryAdvanceAndAcquire(self,on_done,...) do{{on_done};}while(0)
#endif
#ifdef DeeSysFSWideView_AdvanceAndAcquire
# define DeeNFSWideView_AdvanceAndAcquire(self,on_done,...) DeeSysFSWideView_AdvanceAndAcquire(&(self)->nwv_view,on_done,__VA_ARGS__)
#else
# define DeeNFSWideView_AdvanceAndAcquire(self,on_done,...) do{{on_done};}while(0)
#endif
# define DeeNFSWideView_Acquire(self) DeeSysFSWideView_Acquire(&(self)->nwv_view)
# define DeeNFSWideView_Release(self) DeeSysFSWideView_Release(&(self)->nwv_view)
#endif

#ifdef DeeSysFSWideView_TryAdvance
# define DeeNFSWideView_TryAdvance(self,on_done,...) DeeSysFSWideView_TryAdvance(&(self)->nwv_view,on_done,__VA_ARGS__)
#elif defined(DeeSysFSWideView_Advance)
# define DeeNFSWideView_TryAdvance(self,on_done,...) DeeSysFSWideView_Advance(&(self)->nwv_view,on_done,{DeeError_HandledOne();{__VA_ARGS__};})
#else
# define DeeNFSWideView_TryAdvance(self,on_done,...) do{{on_done};}while(0)
#endif
#ifdef DeeSysFSWideView_Advance
# define DeeNFSWideView_Advance(self,on_done,...) DeeSysFSWideView_Advance(&(self)->nwv_view,on_done,__VA_ARGS__)
#else
# define DeeNFSWideView_Advance(self,on_done,...) do{{on_done};}while(0)
#endif


#ifdef DeeSysFSWideView_TryGetFilenameObject
# define DeeNFSWideView_TryGetFilenameObject(self) DeeSysFSWideView_TryGetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSWideView_GetFilenameObject)
#define DeeNFSWideView_TryGetFilenameObject DeeNFSWideView_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetFilenameObject(struct DeeNFSWideView const *self) {
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeSysFSWideView_GetFilenameObject(&(self)->nwv_view)) == NULL) DeeError_HandledOne();
 return result;
 }
#elif defined(DEE_NFSWIDEVIEW_HAVE_LOCK)
#if defined(DeeSysFSWideView_TryGetFilenameObjectLocked)
#define DeeNFSWideView_TryGetFilenameObject DeeNFSWideView_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetFilenameObject(struct DeeNFSWideView const *self) {
 DeeObject *result;
 DeeNFSWideView_Acquire(self);
 result = DeeSysFSWideView_TryGetFilenameObjectLocked(&self->nwv_view)
 DeeNFSWideView_Release(self);
 return result;
}
#elif defined(DeeSysFSWideView_GetFilenameStrLocked)
#define DeeNFSWideView_TryGetFilenameObject DeeNFSWideView_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetFilenameObject(struct DeeNFSWideView const *self) {
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
 result = (DeeObject *)DeeObject_TryWeakMalloc(
  Dee_OFFSETOF(DeeWideStringObject,s_str)+(filename_size+1)*sizeof(Dee_WideChar));
 DeeObject_INIT(result,&DeeWideString_Type);
 ((DeeWideStringObject *)result)->s_len = filename_size;
 memcpy(((DeeWideStringObject *)result)->s_str,filename,filename_size*sizeof(Dee_WideChar));
 ((DeeWideStringObject *)result)->s_str[filename_size] = 0;
 if DEE_UNLIKELY(!result) {
  DeeNFSWideView_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  return NULL;
 }
 DeeNFSWideView_Release(self);
 return result;
}
#endif
#endif

#ifdef DeeSysFSWideView_GetFilenameObject
# define DeeNFSWideView_GetFilenameObject(self) DeeSysFSWideView_GetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSWideView_TryGetFilenameObject)
#define DeeNFSWideView_GetFilenameObject(self) DeeSysFSWideView_TryGetFilenameObject(&(self)->nwv_view)
#elif defined(DEE_NFSWIDEVIEW_HAVE_LOCK)
#if defined(DeeSysFSWideView_TryGetFilenameObjectLocked)
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
 result = (DeeObject *)DeeObject_TryWeakMalloc(
  Dee_OFFSETOF(DeeWideStringObject,s_str)+(filename_size+1)*sizeof(Dee_WideChar));
 DeeObject_INIT(result,&DeeWideString_Type);
 ((DeeWideStringObject *)result)->s_len = filename_size;
 memcpy(((DeeWideStringObject *)result)->s_str,filename,filename_size*sizeof(Dee_WideChar));
 ((DeeWideStringObject *)result)->s_str[filename_size] = 0;
 if DEE_UNLIKELY(!result) {
  DeeNFSWideView_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 DeeNFSWideView_Release(self);
 return result;
}
#endif
#endif

// v Should never happen, but lets be careful.
#if !defined(DeeNFSWideView_GetFilenameObject)\
 || !defined(DeeNFSWideView_TryGetFilenameObject)
#error "No way of retrieving filename from DeeNFSWideView!"
#endif

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *)
DeeNFSWideView_TryGetAbsoluteFilename(DEE_A_IN struct DeeNFSWideView const *self) {
 DeeObject *path,*filename,*result; Dee_WideChar path_end; Dee_size_t path_size;
 if DEE_UNLIKELY((path = DeeNFSWideView_TryGetPath(self)) == NULL) return NULL;
 if DEE_UNLIKELY((filename = DeeNFSWideView_TryGetFilenameObject(self)) == NULL) { Dee_DECREF(path); return NULL; }
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
 if DEE_UNLIKELY(!result) DeeError_HandledOne();
 return result;
}
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

#ifdef DeeSysFSWideView_TryIsFile
#define DeeNFSWideView_TryIsFile(self) DeeSysFSWideView_TryIsFile(&(self)->nwv_view)
#define DeeNFSWideView_IsFile(self,result,...) DeeSysFSWideView_IsFile(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsFile DeeNFSWideView_TryIsFile
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsFile(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsFileObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsFile(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsFileObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsDir
#define DeeNFSWideView_TryIsDir(self) DeeSysFSWideView_TryIsDir(&(self)->nwv_view)
#define DeeNFSWideView_IsDir(self,result,...) DeeSysFSWideView_IsDir(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsDir DeeNFSWideView_TryIsDir
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsDir(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsDirObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsDir(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsDirObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsLink
#define DeeNFSWideView_TryIsLink(self) DeeSysFSWideView_TryIsLink(&(self)->nwv_view)
#define DeeNFSWideView_IsLink(self,result,...) DeeSysFSWideView_IsLink(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsLink DeeNFSWideView_TryIsLink
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsLink(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsLinkObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsLink(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsLinkObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsDrive
#define DeeNFSWideView_TryIsDrive(self) DeeSysFSWideView_TryIsDrive(&(self)->nwv_view)
#define DeeNFSWideView_IsDrive(self,result,...) DeeSysFSWideView_IsDrive(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsDrive DeeNFSWideView_TryIsDrive
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsDrive(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsDriveObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsDrive(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsDriveObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsMount
#define DeeNFSWideView_TryIsMount(self) DeeSysFSWideView_TryIsMount(&(self)->nwv_view)
#define DeeNFSWideView_IsMount(self,result,...) DeeSysFSWideView_IsMount(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsMount DeeNFSWideView_TryIsMount
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsMount(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsMountObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsMount(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsMountObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsHidden
#define DeeNFSWideView_TryIsHidden(self) DeeSysFSWideView_TryIsHidden(&(self)->nwv_view)
#define DeeNFSWideView_IsHidden(self,result,...) DeeSysFSWideView_IsHidden(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsHidden DeeNFSWideView_TryIsHidden
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsHidden(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsHiddenObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsHidden(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsHiddenObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsExecutable
#define DeeNFSWideView_TryIsExecutable(self) DeeSysFSWideView_TryIsExecutable(&(self)->nwv_view)
#define DeeNFSWideView_IsExecutable(self,result,...) DeeSysFSWideView_IsExecutable(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsExecutable DeeNFSWideView_TryIsExecutable
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsExecutable(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsExecutableObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsExecutable(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsExecutableObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsCharDev
#define DeeNFSWideView_TryIsCharDev(self) DeeSysFSWideView_TryIsCharDev(&(self)->nwv_view)
#define DeeNFSWideView_IsCharDev(self,result,...) DeeSysFSWideView_IsCharDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsCharDev DeeNFSWideView_TryIsCharDev
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsCharDev(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsCharDevObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsCharDev(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsCharDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsBlockDev
#define DeeNFSWideView_TryIsBlockDev(self) DeeSysFSWideView_TryIsBlockDev(&(self)->nwv_view)
#define DeeNFSWideView_IsBlockDev(self,result,...) DeeSysFSWideView_IsBlockDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsBlockDev DeeNFSWideView_TryIsBlockDev
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsBlockDev(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsBlockDevObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsBlockDev(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsBlockDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsFiFo
#define DeeNFSWideView_TryIsFiFo(self) DeeSysFSWideView_TryIsFiFo(&(self)->nwv_view)
#define DeeNFSWideView_IsFiFo(self,result,...) DeeSysFSWideView_IsFiFo(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsFiFo DeeNFSWideView_TryIsFiFo
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsFiFo(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsFiFoObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsFiFo(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsFiFoObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSWideView_TryIsSocket
#define DeeNFSWideView_TryIsSocket(self) DeeSysFSWideView_TryIsSocket(&(self)->nwv_view)
#define DeeNFSWideView_IsSocket(self,result,...) DeeSysFSWideView_IsSocket(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSWideView_TryIsSocket DeeNFSWideView_TryIsSocket
DEE_STATIC_INLINE(int) DeeNFSWideView_TryIsSocket(DEE_A_IN struct DeeNFSWideView const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSWideView_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_WideTryIsSocketObject(path); Dee_DECREF(path); return result; }
#define DeeNFSWideView_IsSocket(self,result,...) DeeNFSWideView_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_WideIsSocketObject,result,__VA_ARGS__)
#endif

#endif /* DeeSysFSWideView */
//@DeeSysFSWideView:end


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_WIDE_H */
