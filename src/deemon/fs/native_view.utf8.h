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
#if defined(DeeSysFSUtf8View_GetPath)
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
# define DeeNFSUtf8View_GetPath(self)  DeeSysFSUtf8View_GetPath(&(self)->nwv_view)
#endif
#else
# define DeeNFSUtf8View_GetPath(self) (Dee_IncRef((DeeObject *)(self)->nwv_path),(DeeObject *)(self)->nwv_path)
#endif

#ifdef DeeSysFSUtf8View_Quit
#define DeeNFSUtf8View_PRIVATE_QUIT(self) DeeSysFSUtf8View_Quit(&(self)->nwv_view)
#else
#define DeeNFSUtf8View_PRIVATE_QUIT(self) do{}while(0)
#endif
#ifdef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
# define DeeNFSUtf8View_Quit DeeNFSUtf8View_PRIVATE_QUIT
#else
# define DeeNFSUtf8View_Quit(self) do{Dee_DECREF((self)->nwv_path);DeeNFSUtf8View_PRIVATE_QUIT(self);}while(0)
#endif


#ifdef DEE_NFSUTF8VIEW_HAVE_OSGETPATH
#define DeeNFSUtf8View_InitObject(self,path,...)\
do{DeeSysFSUtf8View_InitObject(&(self)->nwv_view,path,__VA_ARGS__);}while(0)
#ifdef DeeSysFSUtf8View_Init
# define DeeNFSUtf8View_Init(self,path,...)\
 do{DeeSysFSUtf8View_Init(&(self)->nwv_view,path,__VA_ARGS__);}while(0)
#else
# define DeeNFSUtf8View_Init(self,path,...)\
 do{\
  DeeObject *_nv_path_ob;\
  if DEE_UNLIKELY((_nv_path_ob = DeeUtf8String_New(path)) == NULL){__VA_ARGS__;}\
  DeeSysFSUtf8View_InitObject(&(self)->nwv_view,_nv_path_ob,{Dee_DECREF(_nv_path_ob);{__VA_ARGS__;}});\
  Dee_DECREF(_nv_path_ob);\
 }while(0)
#endif
#else
#define DeeNFSUtf8View_InitObject(self,path,...)\
do{DeeSysFSUtf8View_InitObject(&(self)->nwv_view,path,__VA_ARGS__);\
   Dee_INCREF((self)->nwv_path=(DeeUtf8StringObject *)(path));}while(0)
#define DeeNFSUtf8View_Init(self,path,...)\
do{\
 if DEE_UNLIKELY(((self)->nwv_path = (DeeUtf8StringObject *)DeeUtf8String_New(path)) == NULL){__VA_ARGS__;}\
 DeeSysFSUtf8View_InitObject(&(self)->nwv_view,(self)->nwv_path,{Dee_DECREF((self)->nwv_path);{__VA_ARGS__;}});\
}while(0)
#endif

#define DeeNFSUtf8View_Acquire(self) DeeSysFSUtf8View_Acquire(&(self)->nwv_view)
#define DeeNFSUtf8View_Release(self) DeeSysFSUtf8View_Release(&(self)->nwv_view)
#define DeeNFSUtf8View_IsDone(self)  DeeSysFSUtf8View_IsDone(&(self)->nwv_view)
#define DeeNFSUtf8View_AdvanceAndReleaseOnError(self,...)    DeeSysFSUtf8View_AdvanceAndReleaseOnError(&(self)->nwv_view,__VA_ARGS__)

#if defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked)
#define DeeNFSUtf8View_TryGetFilenameObjectLocked DeeNFSUtf8View_TryGetFilenameObjectLocked
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetFilenameObjectLocked(struct DeeNFSUtf8View const *self) {
 DeeObject *result;
 result = DeeSysFSUtf8View_TryGetFilenameObjectLocked(&self->nwv_view)
 return result;
}
#elif defined(DeeSysFSUtf8View_GetFilenameStrLocked)
#define DeeNFSUtf8View_TryGetFilenameObjectLocked DeeNFSUtf8View_TryGetFilenameObjectLocked
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *)
DeeNFSUtf8View_TryGetFilenameObjectLocked(struct DeeNFSUtf8View const *self) {
 DeeObject *result; Dee_Utf8Char const *filename; Dee_size_t filename_size;
 filename = DeeSysFSUtf8View_GetFilenameStrLocked(&self->nwv_view);
#ifndef DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
 if (!filename) DeeReturn_EmptyUtf8String;
#endif
#ifdef DeeSysFSUtf8View_GetFilenameSizeLocked
 filename_size = DeeSysFSUtf8View_GetFilenameSizeLocked(&self->nwv_view);
#else
 filename_size = Dee_Utf8StrLen(filename);
#endif
 result = (DeeObject *)DeeObject_TryWeakMalloc(Dee_OFFSETOF(DeeUtf8StringObject,s_str)+
                                               (filename_size+1)*sizeof(Dee_Utf8Char));
 if DEE_UNLIKELY(!result) return NULL;
 DeeObject_INIT(result,&DeeUtf8String_Type);
 ((DeeUtf8StringObject *)result)->s_len = filename_size;
 memcpy(((DeeUtf8StringObject *)result)->s_str,filename,filename_size*sizeof(Dee_Utf8Char));
 ((DeeUtf8StringObject *)result)->s_str[filename_size] = 0;
 return result;
}
#endif

#ifdef DeeSysFSUtf8View_GetFilenameObject
# define DeeNFSUtf8View_GetFilenameObject(self) DeeSysFSUtf8View_GetFilenameObject(&(self)->nwv_view)
#elif defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked)
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
 result = (DeeObject *)DeeObject_TryWeakMalloc(Dee_OFFSETOF(DeeUtf8StringObject,s_str)+
                                               (filename_size+1)*sizeof(Dee_Utf8Char));
 if DEE_UNLIKELY(!result) {
  DeeNFSUtf8View_Release(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 DeeObject_INIT(result,&DeeUtf8String_Type);
 ((DeeUtf8StringObject *)result)->s_len = filename_size;
 memcpy(((DeeUtf8StringObject *)result)->s_str,filename,filename_size*sizeof(Dee_Utf8Char));
 ((DeeUtf8StringObject *)result)->s_str[filename_size] = 0;
 DeeNFSUtf8View_Release(self);
 return result;
}
#endif

// v Should never happen, but lets be careful.
#if !defined(DeeNFSUtf8View_GetFilenameObject)\
 || !defined(DeeNFSUtf8View_TryGetFilenameObjectLocked)
#error "No way of retrieving filename from DeeNFSUtf8View!"
#endif

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

#ifdef DeeSysFSUtf8View_IsFile
#define DeeNFSUtf8View_IsFile(self,result,...) DeeSysFSUtf8View_IsFile(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsFile(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsFileObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsDir
#define DeeNFSUtf8View_IsDir(self,result,...) DeeSysFSUtf8View_IsDir(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsDir(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsDirObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsLink
#define DeeNFSUtf8View_IsLink(self,result,...) DeeSysFSUtf8View_IsLink(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsLink(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsLinkObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsDrive
#define DeeNFSUtf8View_IsDrive(self,result,...) DeeSysFSUtf8View_IsDrive(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsDrive(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsDriveObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsMount
#define DeeNFSUtf8View_IsMount(self,result,...) DeeSysFSUtf8View_IsMount(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsMount(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsMountObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsHidden
#define DeeNFSUtf8View_IsHidden(self,result,...) DeeSysFSUtf8View_IsHidden(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsHidden(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsHiddenObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsExecutable
#define DeeNFSUtf8View_IsExecutable(self,result,...) DeeSysFSUtf8View_IsExecutable(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsExecutable(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsExecutableObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsCharDev
#define DeeNFSUtf8View_IsCharDev(self,result,...) DeeSysFSUtf8View_IsCharDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsCharDev(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsCharDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsBlockDev
#define DeeNFSUtf8View_IsBlockDev(self,result,...) DeeSysFSUtf8View_IsBlockDev(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsBlockDev(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsBlockDevObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsFiFo
#define DeeNFSUtf8View_IsFiFo(self,result,...) DeeSysFSUtf8View_IsFiFo(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsFiFo(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsFiFoObject,result,__VA_ARGS__)
#endif
#ifdef DeeSysFSUtf8View_IsSocket
#define DeeNFSUtf8View_IsSocket(self,result,...) DeeSysFSUtf8View_IsSocket(&(self)->nwv_view,result,__VA_ARGS__)
#else
#define DeeNFSUtf8View_IsSocket(self,result,...) DeeNFSUtf8View_PRIVATE_GET_ATTRIBUTE(self,DeeNFS_Utf8IsSocketObject,result,__VA_ARGS__)
#endif

#endif /* DeeSysFSUtf8View */
//[[[end]]]


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_UTF8_H */
