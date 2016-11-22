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
#ifndef GUARD_DEEMON_FS_NATIVE_VIEW_UTF8_H
#define GUARD_DEEMON_FS_NATIVE_VIEW_UTF8_H 1
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

/*[[[deemon
#include <file>
#include <fs>
const begin_tag = "@DeeSysFSWideView:begin";
const end_tag   = "@DeeSysFSWideView:end";
fs::chdir(fs::path::head(__FILE__));
text = file.open("native_view.wide.h","r").read();
begin = text.index(begin_tag)+#begin_tag;
end = text.index(end_tag,begin);
text = text[begin:end];

text = text.replace("wide","utf8");
text = text.replace("Wide","Utf8");
text = text.replace("WIDE","UTF8");
text = text.strip().rsstrip("//").strip();
print text;
]]]*/
#ifdef DeeSysFSUtf8View
#if defined(DeeSysFSUtf8View_TryGetPath)\
 || defined(DeeSysFSUtf8View_GetPath)
#define DEE_NFSUTF8VIEW_HAVE_OSGETPATH
#endif

#if defined(DeeSysFSUtf8View_Release)
#define DEE_NFSUTF8VIEW_HAVE_LOCK
#endif

#define DeeNFSUtf8View DeeNFSUtf8View
struct DeeNFSUtf8View {
#ifndef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
 DEE_A_REF DeeUtf8StringObject *nwv_path; /*< [1..1][const] Path supplied to the underlying view. */
#endif
 struct DeeSysFSUtf8View        nwv_view; /*< Underlying view structure. */
};

#ifdef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
#ifdef DeeSysFSUtf8View_GetPath
# define DeeNFSUtf8View_GetPath(self)    DeeSysFSUtf8View_GetPath(&(self)->nwv_view)
#endif
#ifdef DeeSysFSUtf8View_TryGetPath
# define DeeNFSUtf8View_TryGetPath(self) DeeSysFSUtf8View_TryGetPath(&(self)->nwv_view)
#else
DEE_STATIC_INLINE(DeeObject *) DeeNFSUtf8View_TryGetPath(struct DeeNFSUtf8View *self) {
 DeeObject *result;
 if ((result = DeeNFSUtf8View_GetPath(self)) == NULL) DeeError_HandledOne();
 return result;
}
#endif
#else
# define DeeNFSUtf8View_TryGetPath(self) (Dee_IncRef((DeeObject *)(self)->nwv_path),(DeeObject *)(self)->nwv_path)
#endif
#ifndef DeeNFSUtf8View_GetPath
# define DeeNFSUtf8View_GetPath DeeNFSUtf8View_TryGetPath
#endif

#if defined(DeeSysFSUtf8View_IsEmpty) && defined(DeeSysFSUtf8View_Quit)
#define DeeNFSUtf8View_PRIVATE_QUIT(self) \
do{ if (!DeeSysFSUtf8View_IsEmpty(&(self)->nwv_view)) DeeSysFSUtf8View_Quit(&(self)->nwv_view); }while(0)
#elif defined(DeeSysFSUtf8View_Quit)
#define DeeNFSUtf8View_PRIVATE_QUIT(self) \
do{ DeeSysFSUtf8View_Quit(&(self)->nwv_view); }while(0)
#else
#define DeeNFSUtf8View_PRIVATE_QUIT(self) do{}while(0)
#endif

#ifdef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
# define DeeNFSUtf8View_Quit DeeNFSUtf8View_PRIVATE_QUIT
#else
# define DeeNFSUtf8View_Quit(self) do{Dee_DECREF(&(self)->nwv_path);DeeNFSUtf8View_PRIVATE_QUIT(self);}while(0)
#endif


#ifdef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
#define DeeNFSUtf8View_TryInitObject(self,path,on_empty,...)\
do{DeeSysFSUtf8View_TryInitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#define DeeNFSUtf8View_InitObject(self,path,on_empty,...)\
do{DeeSysFSUtf8View_InitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#ifdef DeeSysFSUtf8View_TryInit
# define DeeNFSUtf8View_TryInit(self,path,on_empty,...)\
 do{DeeSysFSUtf8View_TryInit(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#else
# define DeeNFSUtf8View_TryInit(self,path,on_empty,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeUtf8String_New(path)) == NULL){DeeError_HandledOne();{__VA_ARGS__;}}\
  DeeSysFSUtf8View_TryInitObject(&(self)->nwv_view,_nv_path_ob,on_empty,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#ifdef DeeSysFSUtf8View_Init
# define DeeNFSUtf8View_Init(self,path,on_empty,...)\
 do{DeeSysFSUtf8View_Init(&(self)->nwv_view,path,on_empty,__VA_ARGS__);}while(0)
#else
# define DeeNFSUtf8View_Init(self,path,on_empty,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeUtf8String_New(path)) == NULL){__VA_ARGS__;}\
  DeeSysFSUtf8View_InitObject(&(self)->nwv_view,_nv_path_ob,on_empty,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#else
#define DeeNFSUtf8View_TryInitObject(self,path,on_empty,...)\
do{DeeSysFSUtf8View_TryInitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeUtf8StringObject *)(path));}while(0)
#define DeeNFSUtf8View_InitObject(self,path,on_empty,...)\
do{DeeSysFSUtf8View_InitObject(&(self)->nwv_view,path,on_empty,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeUtf8StringObject *)(path));}while(0)
#define DeeNFSUtf8View_TryInit(self,path,on_empty,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeUtf8StringObject *)DeeUtf8String_New(path)) == NULL){DeeError_HandledOne();{__VA_ARGS__;}}\
 DeeSysFSUtf8View_TryInitObject(&(self)->nwv_view,(self)->nwv_path,on_empty,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#define DeeNFSUtf8View_Init(self,path,on_empty,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeUtf8StringObject *)DeeUtf8String_New(path)) == NULL){__VA_ARGS__;}\
 DeeSysFSUtf8View_InitObject(&(self)->nwv_view,(self)->nwv_path,on_empty,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#endif

#ifdef DEE_NFSUTF8VIEW_HAVE_LOCK
#ifdef DeeSysFSUtf8View_TryAdvanceAndAcquire
# define DeeNFSUtf8View_TryAdvanceAndAcquire(self,on_done,...) DeeSysFSUtf8View_TryAdvanceAndAcquire(&(self)->nwv_view,on_done,__VA_ARGS__)
#elif defined(DeeSysFSUtf8View_AdvanceAndAcquire)
# define DeeNFSUtf8View_TryAdvanceAndAcquire(self,on_done,...) DeeSysFSUtf8View_AdvanceAndAcquire(&(self)->nwv_view,on_done,{DeeError_HandledOne();{__VA_ARGS__};})
#else
# define DeeNFSUtf8View_TryAdvanceAndAcquire(self,on_done,...) do{{on_done};}while(0)
#endif
#ifdef DeeSysFSUtf8View_AdvanceAndAcquire
# define DeeNFSUtf8View_AdvanceAndAcquire(self,on_done,...) DeeSysFSUtf8View_AdvanceAndAcquire(&(self)->nwv_view,on_done,__VA_ARGS__)
#else
# define DeeNFSUtf8View_AdvanceAndAcquire(self,on_done,...) do{{on_done};}while(0)
#endif
# define DeeNFSUtf8View_Acquire(self) DeeSysFSUtf8View_Acquire(&(self)->nwv_view)
# define DeeNFSUtf8View_Release(self) DeeSysFSUtf8View_Release(&(self)->nwv_view)
#endif

#ifdef DeeSysFSUtf8View_TryAdvance
# define DeeNFSUtf8View_TryAdvance(self,on_done,...) DeeSysFSUtf8View_TryAdvance(&(self)->nwv_view,on_done,__VA_ARGS__)
#elif defined(DeeSysFSUtf8View_Advance)
# define DeeNFSUtf8View_TryAdvance(self,on_done,...) DeeSysFSUtf8View_Advance(&(self)->nwv_view,on_done,{DeeError_HandledOne();{__VA_ARGS__};})
#else
# define DeeNFSUtf8View_TryAdvance(self,on_done,...) do{{on_done};}while(0)
#endif
#ifdef DeeSysFSUtf8View_Advance
# define DeeNFSUtf8View_Advance(self,on_done,...) DeeSysFSUtf8View_Advance(&(self)->nwv_view,on_done,__VA_ARGS__)
#else
# define DeeNFSUtf8View_Advance(self,on_done,...) do{{on_done};}while(0)
#endif


#ifdef DeeSysFSUtf8View_TryGetFilenameObject
# define DeeNFSUtf8View_TryGetFilenameObject(self) DeeSysFSUtf8View_TryGetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSUtf8View_GetFilenameObject)
#define DeeNFSUtf8View_TryGetFilenameObject DeeNFSUtf8View_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetFilenameObject(struct DeeNFSUtf8View const *self) {
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeSysFSUtf8View_GetFilenameObject(&(self)->nwv_view)) == NULL) DeeError_HandledOne();
 return result;
 }
#elif defined(DEE_NFSUTF8VIEW_HAVE_LOCK)
#if defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked)
#define DeeNFSUtf8View_TryGetFilenameObject DeeNFSUtf8View_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetFilenameObject(struct DeeNFSUtf8View const *self) {
 DeeObject *result;
 DeeNFSUtf8View_Acquire(self);
 result = DeeSysFSUtf8View_TryGetFilenameObjectLocked(&self->nwv_view)
 DeeNFSUtf8View_Release(self);
 return result;
}
#elif defined(DeeSysFSUtf8View_GetFilenameStrLocked)
#define DeeNFSUtf8View_TryGetFilenameObject DeeNFSUtf8View_TryGetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetFilenameObject(struct DeeNFSUtf8View const *self) {
 DeeObject *result; Dee_Utf8Char const *filename; Dee_size_t filename_size;
again:
 DeeNFSUtf8View_Acquire(self);
 filename = DeeSysFSUtf8View_GetFilenameStrLocked(&self->nwv_view);
#ifndef DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
 if (!filename) { DeeNFSUtf8View_Release(self); DeeReturn_EmptyUtf8String; }
#endif
#ifdef DeeSysFSUtf8View_GetFilenameSizeLocked
 filename_size = DeeSysFSUtf8View_GetFilenameSizeLocked(&self->nwv_view);
#else
 filename_size = Dee_Utf8StrLen(filename);
#endif
 result = (DeeObject *)DeeObject_TryWeakMalloc(
  Dee_OFFSETOF(DeeUtf8StringObject,s_str)+(filename_size+1)*sizeof(Dee_Utf8Char));
 DeeObject_INIT(result,&DeeUtf8String_Type);
 ((DeeUtf8StringObject *)result)->s_len = filename_size;
 memcpy(((DeeUtf8StringObject *)result)->s_str,filename,filename_size*sizeof(Dee_Utf8Char));
 ((DeeUtf8StringObject *)result)->s_str[filename_size] = 0;
 if DEE_UNLIKELY(!result) {
  DeeNFSUtf8View_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  return NULL;
 }
 DeeNFSUtf8View_Release(self);
 return result;
}
#endif
#endif

#ifdef DeeSysFSUtf8View_GetFilenameObject
# define DeeNFSUtf8View_GetFilenameObject(self) DeeSysFSUtf8View_GetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSUtf8View_TryGetFilenameObject)
#define DeeNFSUtf8View_GetFilenameObject(self) DeeSysFSUtf8View_TryGetFilenameObject(&(self)->nwv_view)
#elif defined(DEE_NFSUTF8VIEW_HAVE_LOCK)
#if defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked)
#define DeeNFSUtf8View_GetFilenameObject DeeNFSUtf8View_GetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_GetFilenameObject(struct DeeNFSUtf8View const *self) {
 DeeObject *result;
 DeeNFSUtf8View_Acquire(self);
 result = DeeSysFSUtf8View_TryGetFilenameObjectLocked(&self->nwv_view)
 DeeNFSUtf8View_Release(self);
 return result;
}
#elif defined(DeeSysFSUtf8View_GetFilenameStrLocked)
#define DeeNFSUtf8View_GetFilenameObject DeeNFSUtf8View_GetFilenameObject
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_GetFilenameObject(struct DeeNFSUtf8View const *self) {
 DeeObject *result; Dee_Utf8Char const *filename; Dee_size_t filename_size;
again:
 DeeNFSUtf8View_Acquire(self);
 filename = DeeSysFSUtf8View_GetFilenameStrLocked(&self->nwv_view);
#ifndef DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
 if (!filename) { DeeNFSUtf8View_Release(self); DeeReturn_EmptyUtf8String; }
#endif
#ifdef DeeSysFSUtf8View_GetFilenameSizeLocked
 filename_size = DeeSysFSUtf8View_GetFilenameSizeLocked(&self->nwv_view);
#else
 filename_size = Dee_Utf8StrLen(filename);
#endif
 result = (DeeObject *)DeeObject_TryWeakMalloc(
  Dee_OFFSETOF(DeeUtf8StringObject,s_str)+(filename_size+1)*sizeof(Dee_Utf8Char));
 DeeObject_INIT(result,&DeeUtf8String_Type);
 ((DeeUtf8StringObject *)result)->s_len = filename_size;
 memcpy(((DeeUtf8StringObject *)result)->s_str,filename,filename_size*sizeof(Dee_Utf8Char));
 ((DeeUtf8StringObject *)result)->s_str[filename_size] = 0;
 if DEE_UNLIKELY(!result) {
  DeeNFSUtf8View_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 DeeNFSUtf8View_Release(self);
 return result;
}
#endif
#endif

// v Should never happen, but lets be careful.
#if !defined(DeeNFSUtf8View_GetFilenameObject)\
 || !defined(DeeNFSUtf8View_TryGetFilenameObject)
#error "No way of retrieving filename from DeeNFSUtf8View!"
#endif

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetAbsoluteFilename(DEE_A_IN struct DeeNFSUtf8View const *self) {
 DeeObject *path,*filename,*result; Dee_Utf8Char path_end; Dee_size_t path_size;
 if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetPath(self)) == NULL) return NULL;
 if DEE_UNLIKELY((filename = DeeNFSUtf8View_TryGetFilenameObject(self)) == NULL) { Dee_DECREF(path); return NULL; }
 if DEE_UNLIKELY(!DeeUtf8String_SIZE(path)) { Dee_DECREF(path); return filename; }
 path_size = DeeUtf8String_SIZE(path);
 path_end = DeeUtf8String_STR(path)[path_size-1];
 if (path_end == '\\' || path_end == '/') {
  result = DeeUtf8String_ConcatWithLength(path,DeeUtf8String_SIZE(filename),
                                          DeeUtf8String_STR(filename));
 } else if ((result = DeeUtf8String_NewSized(path_size+1+DeeUtf8String_SIZE(filename))) != NULL) {
  memcpy(DeeUtf8String_STR(result),DeeUtf8String_STR(path),path_size);
#ifdef DEE_PLATFORM_WINDOWS
  DeeUtf8String_STR(result)[path_size] = '\\';
#else
  DeeUtf8String_STR(result)[path_size] = '/';
#endif
  memcpy(DeeUtf8String_STR(result)+(path_size+1),
         DeeUtf8String_STR(filename),DeeUtf8String_SIZE(filename));
 }
 Dee_DECREF(filename);
 Dee_DECREF(path);
 if DEE_UNLIKELY(!result) DeeError_HandledOne();
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_GetAbsoluteFilename(DEE_A_IN struct DeeNFSUtf8View const *self) {
 DeeObject *path,*filename,*result; Dee_Utf8Char path_end; Dee_size_t path_size;
 if DEE_UNLIKELY((path = DeeNFSUtf8View_GetPath(self)) == NULL) return NULL;
 if DEE_UNLIKELY((filename = DeeNFSUtf8View_GetFilenameObject(self)) == NULL) { Dee_DECREF(path); return NULL; }
 if DEE_UNLIKELY(!DeeUtf8String_SIZE(path)) { Dee_DECREF(path); return filename; }
 path_size = DeeUtf8String_SIZE(path);
 path_end = DeeUtf8String_STR(path)[path_size-1];
 if (path_end == '\\' || path_end == '/') {
  result = DeeUtf8String_ConcatWithLength(path,DeeUtf8String_SIZE(filename),
                                          DeeUtf8String_STR(filename));
 } else if ((result = DeeUtf8String_NewSized(path_size+1+DeeUtf8String_SIZE(filename))) != NULL) {
  memcpy(DeeUtf8String_STR(result),DeeUtf8String_STR(path),path_size);
#ifdef DEE_PLATFORM_WINDOWS
  DeeUtf8String_STR(result)[path_size] = '\\';
#else
  DeeUtf8String_STR(result)[path_size] = '/';
#endif
  memcpy(DeeUtf8String_STR(result)+(path_size+1),
         DeeUtf8String_STR(filename),DeeUtf8String_SIZE(filename));
 }
 Dee_DECREF(filename);
 Dee_DECREF(path);
 return result;
}
#define DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,nfs_function,result,...) \
do{\
 DeeObject *_wv_path;\
 if ((_wv_path = DeeNFSUtf8View_GetAbsoluteFilename(self)) == NULL) {__VA_ARGS__;}\
 *(result) = nfs_function(_wv_path);\
 Dee_DECREF(_wv_path);\
 if (*(result) < 0){__VA_ARGS__;}\
}while(0)

#ifdef DeeSysFSUtf8View_TryIsFile
#define DeeNFSUtf8View_TryIsFile(self) DeeSysFSUtf8View_TryIsFile(&(self)->nwv_view)
#define DeeNFSUtf8View_IsFile(self,result,...) DeeSysFSUtf8View_IsFile(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsFile DeeNFSUtf8View_TryIsFile
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsFile(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsFileObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsFile(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsFileObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsDir
#define DeeNFSUtf8View_TryIsDir(self) DeeSysFSUtf8View_TryIsDir(&(self)->nwv_view)
#define DeeNFSUtf8View_IsDir(self,result,...) DeeSysFSUtf8View_IsDir(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsDir DeeNFSUtf8View_TryIsDir
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsDir(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsDirObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsDir(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsDirObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsLink
#define DeeNFSUtf8View_TryIsLink(self) DeeSysFSUtf8View_TryIsLink(&(self)->nwv_view)
#define DeeNFSUtf8View_IsLink(self,result,...) DeeSysFSUtf8View_IsLink(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsLink DeeNFSUtf8View_TryIsLink
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsLink(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsLinkObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsLink(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsLinkObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsDrive
#define DeeNFSUtf8View_TryIsDrive(self) DeeSysFSUtf8View_TryIsDrive(&(self)->nwv_view)
#define DeeNFSUtf8View_IsDrive(self,result,...) DeeSysFSUtf8View_IsDrive(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsDrive DeeNFSUtf8View_TryIsDrive
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsDrive(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsDriveObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsDrive(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsDriveObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsMount
#define DeeNFSUtf8View_TryIsMount(self) DeeSysFSUtf8View_TryIsMount(&(self)->nwv_view)
#define DeeNFSUtf8View_IsMount(self,result,...) DeeSysFSUtf8View_IsMount(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsMount DeeNFSUtf8View_TryIsMount
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsMount(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsMountObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsMount(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsMountObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsHidden
#define DeeNFSUtf8View_TryIsHidden(self) DeeSysFSUtf8View_TryIsHidden(&(self)->nwv_view)
#define DeeNFSUtf8View_IsHidden(self,result,...) DeeSysFSUtf8View_IsHidden(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsHidden DeeNFSUtf8View_TryIsHidden
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsHidden(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsHiddenObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsHidden(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsHiddenObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsExecutable
#define DeeNFSUtf8View_TryIsExecutable(self) DeeSysFSUtf8View_TryIsExecutable(&(self)->nwv_view)
#define DeeNFSUtf8View_IsExecutable(self,result,...) DeeSysFSUtf8View_IsExecutable(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsExecutable DeeNFSUtf8View_TryIsExecutable
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsExecutable(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsExecutableObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsExecutable(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsExecutableObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsCharDev
#define DeeNFSUtf8View_TryIsCharDev(self) DeeSysFSUtf8View_TryIsCharDev(&(self)->nwv_view)
#define DeeNFSUtf8View_IsCharDev(self,result,...) DeeSysFSUtf8View_IsCharDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsCharDev DeeNFSUtf8View_TryIsCharDev
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsCharDev(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsCharDevObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsCharDev(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsCharDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsBlockDev
#define DeeNFSUtf8View_TryIsBlockDev(self) DeeSysFSUtf8View_TryIsBlockDev(&(self)->nwv_view)
#define DeeNFSUtf8View_IsBlockDev(self,result,...) DeeSysFSUtf8View_IsBlockDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsBlockDev DeeNFSUtf8View_TryIsBlockDev
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsBlockDev(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsBlockDevObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsBlockDev(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsBlockDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsFiFo
#define DeeNFSUtf8View_TryIsFiFo(self) DeeSysFSUtf8View_TryIsFiFo(&(self)->nwv_view)
#define DeeNFSUtf8View_IsFiFo(self,result,...) DeeSysFSUtf8View_IsFiFo(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsFiFo DeeNFSUtf8View_TryIsFiFo
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsFiFo(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsFiFoObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsFiFo(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsFiFoObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_TryIsSocket
#define DeeNFSUtf8View_TryIsSocket(self) DeeSysFSUtf8View_TryIsSocket(&(self)->nwv_view)
#define DeeNFSUtf8View_IsSocket(self,result,...) DeeSysFSUtf8View_IsSocket(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_TryIsSocket DeeNFSUtf8View_TryIsSocket
DEE_STATIC_INLINE(int) DeeNFSUtf8View_TryIsSocket(DEE_A_IN struct DeeNFSUtf8View const *self) { DeeObject *path; int result; if DEE_UNLIKELY((path = DeeNFSUtf8View_TryGetAbsoluteFilename(self)) == NULL) return 0; result = DeeNFS_Utf8TryIsSocketObject(path); Dee_DECREF(path); return result; }
#define DeeNFSUtf8View_IsSocket(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsSocketObject,result,__VA_ARGS__)
#endif

#endif /* DeeSysFSUtf8View */
//[[[end]]]


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_UTF8_H */
