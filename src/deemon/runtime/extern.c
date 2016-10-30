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
#ifndef GUARD_DEEMON_EXTERN_C
#define GUARD_DEEMON_EXTERN_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/runtime/extern.h>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/integer.h>
#include <deemon/none.h>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/bool.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/hashmap.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

#if defined(DEE_PLATFORM_WINDOWS)
#define DEE_DLL_USE_WINDOWS_LOAD_LIBRARY
#else /* DEE_PLATFORM_WINDOWS */
#define DEE_DLL_USE_POSIX_DLFCN
#endif /* !DEE_PLATFORM_WINDOWS */


#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if defined(DEE_DLL_USE_WINDOWS_LOAD_LIBRARY)
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#elif defined(DEE_DLL_USE_POSIX_DLFCN)
#include <dlfcn.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeSharedLib_LoadExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type) {
 void *import_addr;
 if ((object_type = DeeType_LValue(object_type)) == NULL) return NULL;
 if ((object_name = DeeString_Cast(object_name)) == NULL) return NULL;
 import_addr = DeeSharedLib_PoolImportObject(filename,DeeString_STR(object_name));
 Dee_DECREF(object_name);
 if (!import_addr) return NULL;
 return DeeLValue_New(object_type,import_addr);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeSharedLib_LoadAutoExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type) {
 // TODO: In the future, this is supposed to implement
 //       automatic linkage with external libraries.
 (void)object_name,object_type;
 DeeError_NotImplemented_str("DeeSharedLib_LoadAutoExtern()");
 return NULL;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_Utf8Init(DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_Z Dee_Utf8Char const *filename);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_WideInit(DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_Z Dee_WideChar const *filename);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_Utf8InitObject(DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_WideInitObject(DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename);

#ifdef DEE_PLATFORM_UNIX
#define DEE_DLOPEN_FLAGS RTLD_NOW|RTLD_GLOBAL
//#define DEE_DLOPEN_FLAGS RTLD_NOW|RTLD_LOCAL
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_Utf8InitObject(
 DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 DEE_ASSERT(DeeObject_Check(filename) && DeeUtf8String_Check(filename));
#if defined(DEE_PLATFORM_WINDOWS)
 return DeeSharedLib_Utf8Init(self,DeeUtf8String_STR(filename));
#elif defined(DEE_PLATFORM_UNIX)
 if ((self->sl_handle = (void *)(DeeUtf8String_SIZE(filename)
  ? dlopen(DeeUtf8String_STR(filename),DEE_DLOPEN_FLAGS)
  : dlopen(NULL,RTLD_LAZY)
  )) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r) : %s",filename,dlerror());
  return -1;
 }
 Dee_INCREF(self->sl_name = (DeeAnyStringObject *)filename);
 return 0;
#else
 (void)self,filename;
 DeeError_NotImplemented();
 return -1;
#endif
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_WideInitObject(
 DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 DEE_ASSERT(DeeObject_Check(filename) && DeeWideString_Check(filename));
#if defined(DEE_PLATFORM_WINDOWS)
 return DeeSharedLib_WideInit(self,DeeWideString_STR(filename));
#elif defined(DEE_PLATFORM_UNIX)
 int result;
 Dee_INCREF(self->sl_name = (DeeAnyStringObject *)filename);
 if ((filename = DeeUtf8String_Cast(filename)) == NULL) return -1;
 if ((self->sl_handle = (void *)(DeeUtf8String_SIZE(filename)
  ? dlopen(DeeUtf8String_STR(filename),DEE_DLOPEN_FLAGS)
  : dlopen(NULL,RTLD_LAZY)
  )) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r) : %s",filename,dlerror());
  Dee_DECREF(self->sl_name);
  result = -1;
 } else result = 0;
 Dee_DECREF(filename);
 return result;
#else
 (void)self,filename;
 DeeError_NotImplemented();
 return -1;
#endif
}


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_Utf8Init(
 DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DEE_ASSERT(filename);
 // TODO: VFS Support
#ifdef DEE_PLATFORM_WINDOWS
 if (*filename) {
  if ((self->sl_handle = (void *)LoadLibraryA(filename)) == NULL) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "LoadLibraryA(%q) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
 } else {
  if ((self->sl_handle = (void *)GetModuleHandleA(NULL)) == NULL) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetModuleHandleA(NULL) : %#.8x : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
 }
 return 0;
#else
 int result; DeeObject *filename_ob;
 if ((filename_ob = DeeUtf8String_New(filename)) == NULL) return -1;
 result = DeeSharedLib_Utf8InitObject(self,filename_ob);
 Dee_DECREF(filename_ob);
 return result;
#endif
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSharedLib_WideInit(
 DEE_A_INOUT DeeSharedLibObject *self, DEE_A_IN_Z Dee_WideChar const *filename) {
 DEE_ASSERT(filename);
 // TODO: VFS Support
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_LIKELY(*filename) {
  if DEE_UNLIKELY((self->sl_handle = (void *)LoadLibraryW(filename)) == NULL) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "LoadLibraryW(%lq) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
 } else {
  if DEE_UNLIKELY((self->sl_handle = (void *)GetModuleHandleW(NULL)) == NULL) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetModuleHandleW(NULL) : %K",
                       filename,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
 }
 return 0;
#else
 int result; DeeObject *filename_ob;
 if DEE_UNLIKELY((filename_ob = DeeWideString_New(filename)) == NULL) return -1;
 result = DeeSharedLib_WideInitObject(self,filename_ob);
 Dee_DECREF(filename_ob);
 return result;
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_Utf8New(
 DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DeeSharedLibObject *result; DEE_ASSERT(filename);
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%s)",filename)) != NULL) {
  DeeObject_INIT(result,&DeeSharedLib_Type);
  if DEE_UNLIKELY(DeeSharedLib_Utf8Init(result,filename) != 0) {
   _DeeObject_DELETE(&DeeSharedLib_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_WideNew(
 DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeSharedLibObject *result; DEE_ASSERT(filename);
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%ls)",filename)) != NULL) {
  DeeObject_INIT(result,&DeeSharedLib_Type);
  if DEE_UNLIKELY(DeeSharedLib_WideInit(result,filename) != 0) {
   _DeeObject_DELETE(&DeeSharedLib_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_NewObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DeeSharedLibObject *result;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%p)",filename)) != NULL) {
  DeeObject_INIT(result,&DeeSharedLib_Type);
  if (DeeWideString_Check(filename)) {
   if DEE_UNLIKELY(DeeSharedLib_WideInitObject(result,filename) != 0) {
err:
    _DeeObject_DELETE(&DeeSharedLib_Type,result);
    result = NULL;
   }
  } else {
   int temp;
   if DEE_UNLIKELY((filename = DeeUtf8String_Cast(filename)) == NULL) goto err;
   temp = DeeSharedLib_Utf8InitObject(result,filename);
   Dee_DECREF(filename);
   if DEE_UNLIKELY(temp != 0) goto err;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeSharedLib_Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_Win32GetModuleName(DeeSharedLib_HANDLE(self));
#else
 Dee_INCREF(((DeeSharedLibObject *)self)->sl_name);
 return (DeeObject *)((DeeSharedLibObject *)self)->sl_name;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeSharedLib_Utf8Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_Utf8Win32GetModuleName(DeeSharedLib_HANDLE(self));
#else
 return DeeUtf8String_Cast((DeeObject *)((DeeSharedLibObject *)self)->sl_name);
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeSharedLib_WideName(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_WideWin32GetModuleName(DeeSharedLib_HANDLE(self));
#else
 return DeeWideString_Cast((DeeObject *)((DeeSharedLibObject *)self)->sl_name);
#endif
}

DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_Import(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self,
 DEE_A_IN_Z char const *name) {
 void *result;
 if DEE_UNLIKELY((result = DeeSharedLib_TryImport(self,name)) == NULL) {
  // Custom system error, to include dll+import names
#if defined(DEE_DLL_USE_WINDOWS_LOAD_LIBRARY)
  unsigned long error = DeeSystemError_Win32Consume();
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetProcAddress(%R,%q) : %K",
                      DeeSharedLib_Name(self),name,
                      DeeSystemError_Win32ToString(error));
#elif defined(DEE_DLL_USE_POSIX_DLFCN)
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "dlsym(%R,%q) : %s",
                      DeeSharedLib_Name(self),name,
                      dlerror());
#endif
 }
 return result;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4054)
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_TryImport(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self,
 DEE_A_IN_Z char const *name) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
#if defined(DEE_DLL_USE_WINDOWS_LOAD_LIBRARY)
 return (void *)GetProcAddress((HMODULE)DeeSharedLib_HANDLE(self),name);
#elif defined(DEE_DLL_USE_POSIX_DLFCN)
 void *symbol = (void *)dlsym(DeeSharedLib_HANDLE(self),name);
 if (DEE_UNLIKELY(!symbol) && DEE_LIKELY(name)) {
  char *name2; Dee_size_t len = strlen(name);
#if DEE_HAVE_ALLOCA
  name2 = (char *)alloca(len+2);
#else
  name2 = (char *)malloc(len+2);
  // Psht! We can't report this one, so lets just 
  //       act like the failed first dlsym call
  //       was responsible.
  //       But seriously: We have no way of reporting
  //                      this out-of-memory error.
  if DEE_UNLIKELY(!name2) return NULL;
#endif
  name2[0] = '_';
  memcpy(name2+1,name,len*sizeof(char));
  name2[len+1] = 0;
  symbol = (void *)dlsym(DeeSharedLib_HANDLE(self),name2);
#if !DEE_HAVE_ALLOCA
  free_nn(name2);
#endif
 }
 return symbol;
#endif
}
DEE_COMPILER_MSVC_WARNING_POP
DEE_A_RET_WUNUSED void *DeeSharedLib_Handle(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 return DeeSharedLib_HANDLE(self);
}




static struct DeeAtomicMutex _dll_lock = DeeAtomicMutex_INIT();
static struct DeeHashMap _dll_pool = DeeHashMap_INIT();

DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_Utf8PoolTryImport(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_Utf8PoolLoad(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_WidePoolTryImport(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_WidePoolLoad(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_PoolTryImportObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_PoolLoadObject(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_Utf8PoolImport(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_Utf8PoolLoad(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}
DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_WidePoolImport(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_WidePoolLoad(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}

DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_PoolImportObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_PoolLoadObject(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}

DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_PoolLoadObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DeeObject *result,*utf8_filename;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_UNLIKELY((utf8_filename = DeeUtf8String_Cast(filename)) == NULL) return NULL;
 filename_hash = DeeHash_Memory(DeeUtf8String_STR(utf8_filename),DeeUtf8String_SIZE(utf8_filename)*sizeof(Dee_Utf8Char),DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,DeeUtf8String_STR(utf8_filename),filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(DEE_LIKELY(temp == 0)) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
  result = DeeSharedLib_NewObject(filename);
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kep alive through the dict
  }
 }
 Dee_DECREF(utf8_filename);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_Utf8PoolLoad(
 DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DeeObject *result;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(filename);
 filename_hash = DeeHash_String(filename,DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,filename,filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(temp == 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
  result = DeeSharedLib_Utf8New(filename);
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,filename,filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,filename,filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kept alive through the dict
  }
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_WidePoolLoad(
 DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeObject *result,*utf8_filename;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_UNLIKELY((utf8_filename = DeeUtf8String_FromWideString(filename)) == NULL) return NULL;
 filename_hash = DeeHash_Memory(DeeUtf8String_STR(utf8_filename),
                                DeeUtf8String_SIZE(utf8_filename)*
                                sizeof(Dee_Utf8Char),DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,DeeUtf8String_STR(utf8_filename),filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(temp == 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
#ifdef DEE_PLATFORM_WINDOWS
  result = DeeSharedLib_WideNew(filename);
#else
  result = DeeSharedLib_Utf8New(DeeUtf8String_STR(utf8_filename));
#endif
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kep alive through the dict
  }
 }
 Dee_DECREF(utf8_filename);
 return result;
}

void DeeSharedLib_Shutdown(void) {
 DeeAtomicMutex_Acquire(&_dll_lock);
 DeeHashMap_Clear(&_dll_pool);
 DeeAtomicMutex_Release(&_dll_lock);
}

static void _deesharedlib_tp_dtor(DeeSharedLibObject *self) {
 if (self->sl_handle) {
#if defined(DEE_DLL_USE_WINDOWS_LOAD_LIBRARY)
  if DEE_UNLIKELY(!FreeLibrary((HMODULE)self->sl_handle)) SetLastError(0);
#elif defined(DEE_DLL_USE_POSIX_DLFCN)
  if DEE_UNLIKELY(dlclose(self->sl_handle) != 0) (void)dlerror(); // Clear the error
#endif
 }
#ifndef DEE_PLATFORM_WINDOWS
 Dee_DECREF(self->sl_name);
#endif
}
#ifndef DEE_PLATFORM_WINDOWS
DEE_VISIT_PROC(_deesharedlib_tp_visit,DeeSharedLibObject *self) {
 Dee_VISIT(self->sl_name);
}
#endif
static DeeObject *_deesharedlib_tp_cmp_lo(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle < right->sl_handle); }
static DeeObject *_deesharedlib_tp_cmp_le(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle <= right->sl_handle); }
static DeeObject *_deesharedlib_tp_cmp_eq(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle == right->sl_handle); }
static DeeObject *_deesharedlib_tp_cmp_ne(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle != right->sl_handle); }
static DeeObject *_deesharedlib_tp_cmp_gr(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle > right->sl_handle); }
static DeeObject *_deesharedlib_tp_cmp_ge(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(self->sl_handle >= right->sl_handle); }


static int _deesharedlib_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSharedLibObject *self, DeeSharedLibObject *right) {
 self->sl_handle = right->sl_handle;
 right->sl_handle = NULL;
#ifndef DEE_PLATFORM_WINDOWS
 self->sl_name = right->sl_name;
 Dee_INCREF(right->sl_name = (DeeAnyStringObject *)Dee_EmptyString);
#endif
 return 0;
}
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
static DeeObject *_deesharedlib_tp_call(DeeSharedLibObject *self, DeeObject *args) {
 DeeObject *import_name; void *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dll.__call__",&import_name) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(import_name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSharedLib_Import((DeeObject *)self,DeeString_STR(import_name))) == NULL) return NULL;
 return DeeVoidPointer_New(result);
}
static DeeObject *_deesharedlib_tp_seq_get(DeeSharedLibObject *self, DeeObject *import_name) {
 char const *import_name_str; void *result;
 if DEE_UNLIKELY(DeeObject_GetPointerEx(import_name,DeeObject_TYPE(char),(void **)&import_name_str) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSharedLib_TryImport((DeeObject *)self,import_name_str)) == NULL) return NULL;
 return DeeVoidPointer_New(result);
}
static DeeObject *_deesharedlib_tp_seq_contains(DeeSharedLibObject *self, DeeObject *import_name) {
 char const *import_name_str;
 if DEE_UNLIKELY(DeeObject_GetPointerEx(import_name,DeeObject_TYPE(char),(void **)&import_name_str) != 0) return NULL;
 DeeReturn_Bool(DeeSharedLib_TryImport((DeeObject *)self,import_name_str) != NULL);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */

static DeeObject *_deesharedlib_tp_repr(DeeSharedLibObject *self) {
 return DeeString_Newf("<dll(%R): %p>",DeeSharedLib_Name((DeeObject *)self),DeeSharedLib_HANDLE(self));
}
static int _deesharedlib_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSharedLibObject *self, DeeObject *args) {
 DeeObject *dll_name; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dll",&dll_name) != 0) return -1;
 if (DeeWideString_Check(dll_name)) {
  return DeeSharedLib_WideInitObject(self,dll_name);
 } else {
  if DEE_UNLIKELY((dll_name = DeeUtf8String_Cast(dll_name)) == NULL) return -1;
  result = DeeSharedLib_Utf8InitObject(self,dll_name);
  Dee_DECREF(dll_name);
  return result;
 }
}
static int _deesharedlib_tp_bool(DeeSharedLibObject *self) { return self->sl_handle != NULL; }
static DeeObject *_deesharedlib_tp_not(DeeSharedLibObject *self) { DeeReturn_Bool(self->sl_handle == NULL); }

static DeeObject *_deedll_name_get(
 DeeSharedLibObject *self, void *DEE_UNUSED(closure)) {
 return DeeSharedLib_Name((DeeObject *)self);
}
static DeeObject *_deedll_close(
 DeeSharedLibObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":close") != 0) return NULL;
 if (self->sl_handle) {
#if defined(DEE_DLL_USE_WINDOWS_LOAD_LIBRARY)
  if DEE_UNLIKELY(!FreeLibrary((HMODULE)self->sl_handle)) SetLastError(0);
#elif defined(DEE_DLL_USE_POSIX_DLFCN)
  if DEE_UNLIKELY(dlclose(self->sl_handle) != 0) (void)dlerror(); // Clear the error
#endif
  self->sl_handle = NULL;
 }
#ifndef DEE_PLATFORM_WINDOWS
 Dee_DECREF(self->sl_name);
 Dee_INCREF(self->sl_name = (DeeAnyStringObject *)Dee_EmptyString);
#endif
 DeeReturn_None;
}
static DeeObject *_deedllclass_open(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *dll_name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:open",&dll_name) != 0) return NULL;
 DeeReturn_XNEWREF(DeeSharedLib_PoolLoadObject(dll_name));
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesharedlib_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deesharedlib_tp_members[] = {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_handle",DeeSharedLibObject,sl_handle,HMODULE),
#else
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_handle",DeeSharedLibObject,sl_handle,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_name",DeeSharedLibObject,sl_name,object),
#endif
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeGetSetDef const _deesharedlib_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deedll_name_get),null,null,"-> string"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deesharedlib_tp_methods[] = {
 DEE_METHODDEF_v100("close",member(&_deedll_close),"() -> none"),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deesharedlib_tp_class_methods[] = {
 DEE_METHODDEF_v100("open",member(&_deedllclass_open),"(string name) -> shared_lib"),
 DEE_METHODDEF_END_v100
};


static struct DeeTypeMarshal _deesharedlib_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_SHAREDLIB),member(NULL),member(NULL)); // Special marshal type

DeeTypeObject DeeSharedLib_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("shared_lib"),member(
  "Wrapper for externally loadable shared libraries."),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSharedLibObject),null,null,null,
  member(&_deesharedlib_tp_move_ctor),
  member(&_deesharedlib_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deesharedlib_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&DeeSharedLib_Name),
  member(&_deesharedlib_tp_repr),null,null,null),
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
#ifdef DEE_PLATFORM_WINDOWS
 DEE_TYPE_OBJECT_OBJECT_v101(member(&_deesharedlib_tp_call),null,null),
#else /* DEE_PLATFORM_WINDOWS */
 DEE_TYPE_OBJECT_OBJECT_v101(member(&_deesharedlib_tp_call),member(&_deesharedlib_tp_visit),null),
#endif /* !DEE_PLATFORM_WINDOWS */
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#ifdef DEE_PLATFORM_WINDOWS
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
#else /* DEE_PLATFORM_WINDOWS */
 DEE_TYPE_OBJECT_OBJECT_v101(null,member(&_deesharedlib_tp_visit),null),
#endif /* !DEE_PLATFORM_WINDOWS */
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deesharedlib_tp_not),member(&_deesharedlib_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesharedlib_tp_cmp_lo),member(&_deesharedlib_tp_cmp_le),
  member(&_deesharedlib_tp_cmp_eq),member(&_deesharedlib_tp_cmp_ne),
  member(&_deesharedlib_tp_cmp_gr),member(&_deesharedlib_tp_cmp_ge)),
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deesharedlib_tp_seq_get),null,null,null,
  member(&_deesharedlib_tp_seq_contains),null,null,null,null,null),
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,null,null),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesharedlib_tp_members),member(_deesharedlib_tp_getsets),
  member(_deesharedlib_tp_methods),null,null,member(_deesharedlib_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deesharedlib_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_EXTERN_C */
