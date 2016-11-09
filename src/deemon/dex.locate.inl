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

#ifdef __INTELLISENSE__
#include "dex.c"
#define WIDE
#endif

#ifdef WIDE
#define DEE_CHAR             Dee_WideChar
#define DEESTRINGOBJECT      DeeWideStringObject
#define DEESTRING_F(name)    DeeWideString_##name
#define DEEDEXMODULE_F(name) DeeDexModule_Wide##name
#else
#define DEE_CHAR             Dee_Utf8Char
#define DEESTRINGOBJECT      DeeUtf8StringObject
#define DEESTRING_F(name)    DeeUtf8String_##name
#define DEEDEXMODULE_F(name) DeeDexModule_Utf8##name
#endif

#define DEESTRING_STR     DEESTRING_F(STR)
#define DEESTRING_SIZE    DEESTRING_F(SIZE)
#define DEESTRING_CHECK   DEESTRING_F(Check)
#define DEESTRING_NEWF    DEESTRING_F(Newf)
#define DEESTRING_CAST    DEESTRING_F(Cast)

DEE_DECL_BEGIN

static DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int DEEDEXMODULE_F(InitFromFilename)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_file,
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_file) && DEESTRING_CHECK(dex_file));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
 DEE_LVERBOSE2("Attempting to load dex %r from %r...",dex_name,dex_file);
#ifdef WIDE
 if (!DeeSysDynlibN_WideTryInitFromFilenameObject(&self->dm_dynlib,dex_file))
#else
 if (!DeeSysDynlibN_Utf8TryInitFromFilenameObject(&self->dm_dynlib,dex_file))
#endif
 {
  DEE_LVERBOSE2R(" (Failed)\n");
  return DEE_DEXMODULE_INIT_NOTFOUND;
 }
 DEE_LVERBOSE2R(" (Found)\n");

 // Time to load the basic exports
 if DEE_UNLIKELY(!DeeSysDynlib_TryImport(&self->dm_dynlib,"DeeDex_Exports",self->dm_exports)) {
  DeeSysDynlibN_Quit(&self->dm_dynlib);
  return DEE_DEXMODULE_INIT_BROKEN;
 } // Broken dex
 if (!DeeSysDynlib_TryImport(&self->dm_dynlib,"DeeDex_Main",self->dm_main)) self->dm_main = NULL;
 if DEE_UNLIKELY(DeeNativeMutex_Init(&self->dm_cachelock) != 0) {
  DeeSysDynlibN_Quit(&self->dm_dynlib);
  return DEE_DEXMODULE_INIT_ERROR;
 }
 Dee_INCREF(self->dm_name = (DeeAnyStringObject *)dex_name);
 self->dm_state = DEE_DEXMODULE_FLAGS_NONE;
 self->dm_tickets = 0;
 self->dm_initnext = NULL;
 self->dm_initprev = NULL;
 return DEE_DEXMODULE_INIT_SUCCESS;
}


DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromAnyPath)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 DeeObject *dex_file; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DeeAnyString_Check(dex_name));
 //DEE_ASSERT(DeeObject_Check(path) && DEESTRING_CHECK(path));
#ifdef DEE_PLATFORM_WINDOWS
 static DEE_CHAR const pattern[] = {'%','k',DEE_FS_SEP,'%','k','.','d','l','l',0};
#else
 static DEE_CHAR const pattern[] = {'%','k',DEE_FS_SEP,'%','k','.','s','o',0};
#endif
 dex_file = DEESTRING_NEWF(pattern,path,dex_name);
 if DEE_UNLIKELY(!dex_file) return DEE_DEXMODULE_INIT_ERROR;
 error = DEEDEXMODULE_F(InitFromFilename)(self,dex_file,dex_name);
 Dee_DECREF(dex_file);
 return error;
}
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromPathList)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name,
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *paths) {
#ifdef WIDE
#ifdef DEE_PLATFORM_WINDOWS
 static DEE_CHAR const pattern[] = {'%','.','*','l','s',DEE_FS_SEP,'%','k','.','d','l','l',0};
#else
 static DEE_CHAR const pattern[] = {'%','.','*','l','s',DEE_FS_SEP,'%','k','.','s','o',0};
#endif
#else
#ifdef DEE_PLATFORM_WINDOWS
 static DEE_CHAR const pattern[] = {'%','.','*','s',DEE_FS_SEP,'%','k','.','d','l','l',0};
#else
 static DEE_CHAR const pattern[] = {'%','.','*','s',DEE_FS_SEP,'%','k','.','s','o',0};
#endif
#endif
 DeeObject *dex_file; int temp; DEE_CHAR const *path_begin,*path_end,*iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
 DEE_ASSERT(DeeObject_Check(paths) && DEESTRING_CHECK(paths));
 end = (path_begin = iter = DEESTRING_STR(paths))+DEESTRING_SIZE(paths);
 while (iter != end) {
  if (*iter == DEE_FS_DELIM) {
   path_end = iter;
   while (path_end != path_begin && (
    path_end[-1] == '/' || path_end[-1] == '\\')) --path_end;
   if (path_end != path_begin) {
    dex_file = DEESTRING_NEWF(pattern,(unsigned)(path_end-path_begin),path_begin,dex_name);
    if DEE_UNLIKELY(!dex_file) return DEE_DEXMODULE_INIT_ERROR;
    temp = DEEDEXMODULE_F(InitFromFilename)(self,dex_file,dex_name);
    Dee_DECREF(dex_file);
    if (temp != DEE_DEXMODULE_INIT_NOTFOUND) return temp; // Success or error
    path_begin = iter;
   }
  }
  ++iter;
 }
 return DEE_DEXMODULE_INIT_NOTFOUND;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromHomeDex)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 DeeObject *dex_file,*homepath; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
#ifdef DEE_PLATFORM_WINDOWS
 static DEE_CHAR const pattern[] = {'%','k',DEE_FS_SEP,'d','e','x',DEE_FS_SEP,'%','k','.','d','l','l',0};
#else
 static DEE_CHAR const pattern[] = {'%','k',DEE_FS_SEP,'d','e','x',DEE_FS_SEP,'%','k','.','s','o',0};
#endif
 if DEE_UNLIKELY((homepath = Dee_GetHome()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
 dex_file = DEESTRING_NEWF(pattern,homepath,dex_name);
 Dee_DECREF(homepath);
 if DEE_UNLIKELY(!dex_file) return DEE_DEXMODULE_INIT_ERROR;
 error = DEEDEXMODULE_F(InitFromFilename)(self,dex_file,dex_name);
 Dee_DECREF(dex_file);
 return error;
}
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromHomeDexVerdep)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 DeeObject *dex_file,*homepath; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
 static DEE_CHAR const pattern[] = {
  '%','k',DEE_FS_SEP,'d','e','x'
#if DEE_API_VERSION >= 1000
  ,'0'+((DEE_API_VERSION%10000)/1000)
#endif
  ,'0'+((DEE_API_VERSION%1000)/100)
  ,'0'+((DEE_API_VERSION%100)/10)
  ,'0'+(DEE_API_VERSION%10)
  ,DEE_FS_SEP,'%','k'
#ifdef DEE_PLATFORM_WINDOWS
  ,'.','d','l','l'
#else
  ,'.','s','o'
#endif
  ,0};
 if DEE_UNLIKELY((homepath = Dee_GetHome()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
 dex_file = DEESTRING_NEWF(pattern,homepath,dex_name);
 Dee_DECREF(homepath);
 if DEE_UNLIKELY(!dex_file) return DEE_DEXMODULE_INIT_ERROR;
 error = DEEDEXMODULE_F(InitFromFilename)(self,dex_file,dex_name);
 Dee_DECREF(dex_file);
 return error;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromDexPath)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 Dee_size_t i; DeeAnyStringObject *rawpath; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
 DeeList_ACQUIRE(DeeDex_SearchPath);
 for (i = 0; i < DeeList_SIZE(DeeDex_SearchPath); ++i) {
  Dee_INCREF(rawpath = (DeeAnyStringObject *)DeeList_GET(DeeDex_SearchPath,i));
  DeeList_RELEASE(DeeDex_SearchPath);
  error = DEEDEXMODULE_F(InitFromAnyPath)(self,dex_name,(DeeObject *)rawpath);
  Dee_DECREF(rawpath);
  if (error != DEE_DEXMODULE_INIT_NOTFOUND) return error; // Success or error
  DeeList_ACQUIRE(DeeDex_SearchPath);
 }
 DeeList_RELEASE(DeeDex_SearchPath);
 return DEE_DEXMODULE_INIT_NOTFOUND;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromCwd)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 DEESTRINGOBJECT *cwd; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
#ifdef WIDE
 if DEE_UNLIKELY((cwd = (DEESTRINGOBJECT *)DeeFS_WideGetCwd()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#else
 if DEE_UNLIKELY((cwd = (DEESTRINGOBJECT *)DeeFS_Utf8GetCwd()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#endif
 error = DEEDEXMODULE_F(InitFromAnyPath)(self,dex_name,(DeeObject *)cwd);
 Dee_DECREF(cwd);
 return error;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromExePath)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 DEESTRINGOBJECT *exepath,*exefile; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
#ifdef WIDE
 if DEE_UNLIKELY((exefile = (DEESTRINGOBJECT *)DeeFS_WideGetDeemonExecutable()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#else
 if DEE_UNLIKELY((exefile = (DEESTRINGOBJECT *)DeeFS_Utf8GetDeemonExecutable()) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#endif
 exepath = (DEESTRINGOBJECT *)DeeFS_PathHeadObject((DeeObject *)exefile);
 Dee_DECREF(exefile);
 if DEE_UNLIKELY(!exepath) return DEE_DEXMODULE_INIT_ERROR;
 error = DEEDEXMODULE_F(InitFromAnyPath)(self,dex_name,(DeeObject *)exepath);
 Dee_DECREF(exepath);
 return error;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int) DEEDEXMODULE_F(InitFromSysPath)(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 static DEE_CHAR const envname[] = {'P','A','T','H',0};
 DEESTRINGOBJECT *syspath; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(DeeObject_Check(dex_name) && DEESTRING_CHECK(dex_name));
#ifdef WIDE
 if DEE_UNLIKELY((syspath = (DEESTRINGOBJECT *)DeeFS_WideGetEnv(envname)) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#else
 if DEE_UNLIKELY((syspath = (DEESTRINGOBJECT *)DeeFS_Utf8GetEnv(envname)) == NULL) return DEE_DEXMODULE_INIT_ERROR;
#endif
 error = DEEDEXMODULE_F(InitFromPathList)(self,dex_name,(DeeObject *)syspath);
 Dee_DECREF(syspath);
 return error;
}

static DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int DEEDEXMODULE_F(InitFromName)(
 DEE_A_INOUT DeeDexModuleObject *self,
 DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *dex_name) {
 char order[DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE];
 char const *iter; int error;
 DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
 memcpy(order,_dex_searchorder,DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE);
 DeeAtomicMutex_Release(&_dex_searchorder_lock);
 iter = order;
next:
 switch (*iter++) {
  case 'S': error = DEEDEXMODULE_F(InitFromHomeDex)(self,dex_name); break;
  case 'V': error = DEEDEXMODULE_F(InitFromHomeDexVerdep)(self,dex_name); break;
  case 'D': error = DEEDEXMODULE_F(InitFromDexPath)(self,dex_name); break;
  case 'C': error = DEEDEXMODULE_F(InitFromCwd)(self,dex_name); break;
  case 'X': error = DEEDEXMODULE_F(InitFromExePath)(self,dex_name); break;
  case 'P': error = DEEDEXMODULE_F(InitFromSysPath)(self,dex_name); break;
  default: return DEE_DEXMODULE_INIT_NOTFOUND; break;
 }
 if (error == DEE_DEXMODULE_INIT_NOTFOUND) goto next;
 return error;
}



DEE_DECL_END

#undef DEE_CHAR
#undef DEESTRINGOBJECT
#undef DEESTRING_F
#undef DEEDEXMODULE_F
#undef DEESTRING_STR
#undef DEESTRING_SIZE
#undef DEESTRING_CHECK
#undef DEESTRING_NEWF
#undef DEESTRING_CAST

#ifdef WIDE
#undef WIDE
#endif

