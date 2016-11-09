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
#ifndef GUARD_DEEMON_VIRTUAL_FS_C_INL
#define GUARD_DEEMON_VIRTUAL_FS_C_INL 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#include <deemon/random.h>
#include <deemon/dex.h>
#include "virtual_fs.h.inl"
#ifdef __INTELLISENSE__
#include "fs_api.c"
#endif
#include "virtual_fs.dev_urandom.inl"
#include "virtual_fs.dev_zero.inl"

DEE_DECL_BEGIN

// Locates the file of a given path and stores its data in 'file'
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Locate(
 DEE_A_IN_Z char const *path, DEE_A_OUT struct DeeVFSFile *file);

#if 0
// Locates the file of a given path and stores its data in 'file'
// >> Doesn't follow a vf_link if 'path' describes one
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_LLocate(
 DEE_A_IN_Z char const *path, DEE_A_OUT struct DeeVFSFile *file);
#endif




// Opens a read-only file.io object that reads its data from 'str'
struct _vfs_fileio_stringmemory_data {
 DEE_A_REF DeeObject *ob_string;
 char const          *ob_pos;
 char const          *ob_end;
};

static int DEE_CALL _vfs_fileio_stringmemory_read(
 DeeFileIOObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 struct _vfs_fileio_stringmemory_data *data = (
  struct _vfs_fileio_stringmemory_data *)self->fio_vfs.fio_data;
 Dee_size_t max_read;
 if DEE_UNLIKELY(!data) {
  DeeError_SetStringf(&DeeErrorType_IOError,
                      "Virtual file was closed: %r:%r",
                      DeeFileIO_FILE(self),DeeFileIO_Mode((DeeObject *)self));
  return -1;
 }
 if (data->ob_pos >= data->ob_end) { *rs = 0; return 0; }
 if (s > (max_read = (Dee_size_t)(data->ob_end-data->ob_pos))) s = max_read;
 memcpy(p,data->ob_pos,s);
 data->ob_pos += s;
 *rs = s;
 return 0;
}
static int DEE_CALL _vfs_fileio_stringmemory_seek(
 DeeFileIOObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 struct _vfs_fileio_stringmemory_data *data = (
  struct _vfs_fileio_stringmemory_data *)self->fio_vfs.fio_data;
 char const *new_pos;
 if DEE_UNLIKELY(!data) {
  DeeError_SetStringf(&DeeErrorType_IOError,
                      "Virtual file was closed: %r:%r",
                      DeeFileIO_FILE(self),DeeFileIO_Mode((DeeObject *)self));
  return -1;
 }
 switch (whence) {
  case DEE_SEEK_SET:
   new_pos = DeeString_STR(data->ob_string)+*(Dee_uint64_t*)&off;
   break;
  case DEE_SEEK_CUR:
   new_pos = data->ob_pos+off;
   break;
  case DEE_SEEK_END:
   new_pos = data->ob_end-*(Dee_uint64_t *)&off;
   break;
  default:
   return _deefile_invalid_seek_mode(whence),-1;
 }
 if (new_pos < DeeString_STR(data->ob_string)) {
  new_pos = DeeString_STR(data->ob_string);
 }
 data->ob_pos = new_pos;
 if (pos) {
  *pos = (Dee_uint64_t)((Dee_size_t)(
   data->ob_pos-DeeString_STR(data->ob_string)));
 }
 return 0;
}
static int DEE_CALL _vfs_fileio_stringmemory_flush(DeeFileIOObject *DEE_UNUSED(self)) {
 return 0;
}
static void DEE_CALL _vfs_fileio_stringmemory_close(DeeFileIOObject *self) {
 struct _vfs_fileio_stringmemory_data *data = (
  struct _vfs_fileio_stringmemory_data *)self->fio_vfs.fio_data;
 if (data) {
  self->fio_vfs.fio_data = NULL;
  Dee_DECREF(data->ob_string);
  free_nn(data);
 }
}
struct DeeFileIOVFSType _vfs_fileio_stringmemory_type = {
 &_vfs_fileio_stringmemory_read,
 NULL,
 &_vfs_fileio_stringmemory_seek,
 &_vfs_fileio_stringmemory_flush,
 NULL,
 &_vfs_fileio_stringmemory_close,
};
static int DEE_CALL _vfs_fileio_open_stringmemory(
 DeeFileIOObject *fp, DeeObject *str) {
 struct _vfs_fileio_stringmemory_data *data;
 DEE_ASSERT(DeeObject_Check(str) && DeeString_Check(str));
 while DEE_UNLIKELY((data = (struct _vfs_fileio_stringmemory_data *)mallocf_nz(
  sizeof(struct _vfs_fileio_stringmemory_data),"vfs-file.io stringmemory: %.*s",
  (int)DeeString_SIZE(str),DeeString_STR(str))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 fp->fo_flags |= DEE_FILE_FLAG_IO_VFSFILE;
 fp->fio_vfs.fio_type = &_vfs_fileio_stringmemory_type;
 fp->fio_vfs.fio_data = data;
 Dee_INCREF(data->ob_string = str);
 data->ob_end = (data->ob_pos = DeeString_STR(str))+DeeString_SIZE(str);
 return 0;
}


//@begin /dev/
static void DEE_CALL _vfserror_cannot_open_mode(
 char const *file, Dee_uint16_t given_mode, Dee_uint16_t required_mode) {
 DeeError_SetStringf(&DeeErrorType_IOError,
                     "%q can only be opened with %r; not %r",file,
                     DeeFile_ModeToString(required_mode),
                     DeeFile_ModeToString(given_mode));
}

static int DEE_CALL _vfs_open_duphandle(
 DEE_A_INOUT DeeFileIOObject *fp, HANDLE hnd) {
 if DEE_UNLIKELY(!DuplicateHandle(GetCurrentProcess(),hnd,
  GetCurrentProcess(),&fp->fio_handle,0,TRUE,DUPLICATE_SAME_ACCESS)) {
  DeeError_SetStringf(&DeeErrorType_IOError,
                      "DuplicateHandle() : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
static int DEE_CALL _vfsdev_stdin_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE) != DEE_FILE_MODE_READ) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_READ|DEE_FILE_MODE_APPEND);
  return -1;
 }
 fp->fio_mode = DEE_FILE_MODE_READ|DEE_FILE_MODE_APPEND;
 return _vfs_open_duphandle(fp,GetStdHandle(STD_INPUT_HANDLE));
}
static int DEE_CALL _vfsdev_stdout_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE) != DEE_FILE_MODE_WRITE) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_WRITE|DEE_FILE_MODE_APPEND);
  return -1;
 }
 fp->fio_mode = DEE_FILE_MODE_WRITE|DEE_FILE_MODE_APPEND;
 return _vfs_open_duphandle(fp,GetStdHandle(STD_OUTPUT_HANDLE));
}
static int DEE_CALL _vfsdev_stderr_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE) != DEE_FILE_MODE_WRITE) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_WRITE|DEE_FILE_MODE_APPEND);
  return -1;
 }
 fp->fio_mode = DEE_FILE_MODE_WRITE|DEE_FILE_MODE_APPEND;
 return _vfs_open_duphandle(fp,GetStdHandle(STD_ERROR_HANDLE));
}
static int DEE_CALL _vfsdev_null_open(
 DEE_A_IN struct DeeVFSFile const *DEE_UNUSED(self),
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t DEE_UNUSED(mode)) {
 fp->fio_handle = CreateFileA("NUL",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
                              NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if DEE_UNLIKELY(fp->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
  DeeError_SetStringf(&DeeErrorType_FileNotFound,
                      "CreateFileA(\"NUL\") : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 fp->fio_mode = DEE_FILE_MODE_READ|DEE_FILE_FLAG_UPDATE;
 return 0;
}


static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsdev_urandom_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE) != DEE_FILE_MODE_READ) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_READ|DEE_FILE_MODE_APPEND);
  return -1;
 }
 {
#if DEE_TYPES_SIZEOF_POINTER < 8
  struct _vfs_fileio_urandom_data *data;
  while DEE_UNLIKELY((data = (struct _vfs_fileio_urandom_data *)mallocf_nz(
   sizeof(struct _vfs_fileio_urandom_data),"vfs-file.io urandom")) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  fp->fio_vfs.fio_data = data;
  DeeRandomNumberGenerator_Randomize(&data->urd_rnd);
#else
  DeeRandomNumberGenerator_Randomize((
   struct DeeRandomNumberGenerator *)&fp->fio_vfs.fio_data);
#endif
 }
 fp->fo_flags |= DEE_FILE_FLAG_IO_VFSFILE;
 fp->fio_vfs.fio_type = &_vfs_fileio_urandom_type;
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsdev_zero_open(
 DEE_A_IN struct DeeVFSFile const *DEE_UNUSED(self),
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t DEE_UNUSED(mode)) {
 fp->fio_vfs.fio_data = NULL;
 fp->fo_flags |= DEE_FILE_FLAG_IO_VFSFILE;
 fp->fio_vfs.fio_type = &_vfs_fileio_zero_type;
 return 0;
}
//@end /dev/



static DEE_A_RET_EXCEPT_FAIL(-1,1) int DEE_CALL _vfsdir_walk(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end) {
 struct DeeVFSFile const *iter,*end; Dee_size_t path_len;
 end = (iter = self->vf_filev)+self->vf_filec;
 path_len = (Dee_size_t)(path_end-path_begin);
 while (iter != end) {
  if (iter->vf_name && strlen(iter->vf_name) == path_len &&
      memcmp(iter->vf_name,path_begin,path_len*sizeof(char)) == 0) {
   *file = *iter;
   return 0;
  }
  ++iter;
 }
 return 1;
}
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DEE_CALL _vfsdir_walk_flags(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end) {
 struct DeeVFSFile const *iter,*end; Dee_size_t path_len;
 end = (iter = self->vf_filev)+self->vf_filec;
 path_len = (Dee_size_t)(path_end-path_begin);
 while (iter != end) {
  if (iter->vf_name && strlen(iter->vf_name) == path_len &&
      memcmp(iter->vf_name,path_begin,path_len*sizeof(char)) == 0) {
   *file = *iter;
   file->vf_flags = self->vf_flags;
   return 0;
  }
  ++iter;
 }
 return 1;
}
static int DEE_CALL DEE_CALL _vfsdir_enum_yield(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DeeObject **result) {
 struct DeeVFSFile *file;
 do {
  if (self->di_vfs.div_elemiter == self->di_vfs.div_elemend) return 1;
  file = (*((struct DeeVFSFile **)&self->di_vfs.div_elemiter))++;
 } while (!file->vf_name);
 if DEE_UNLIKELY((*result = DeeString_New(file->vf_name)) == NULL) return -1;
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsdir_enum(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_INOUT DeeFSDirIteratorObject *file) {
 file->di_vfs.div_yield = &_vfsdir_enum_yield;
 file->di_vfs.div_elemiter = (void *)self->vf_filev;
 file->di_vfs.div_elemend = (void *)(self->vf_filev+self->vf_filec);
 file->di_vfs.div_elemlist = NULL;
 file->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_VFS;
 return 0;
}



#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
static int DEE_CALL _vfsproc_enum_yield(
 DEE_A_INOUT DeeFSDirIteratorObject *self, DeeObject **result) {
 DeeString_NEW_STATIC(_self,"self");
 DeeString_NEW_STATIC(_uptime,"uptime");
 if DEE_UNLIKELY((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_CTX0)==0) {
  self->di_flags |= DEE_FS_DIR_ITERATOR_FLAGS_CTX0;
  Dee_INCREF(*result = (DeeObject *)&_self);
  return 0;
 }
 if DEE_UNLIKELY((self->di_flags&DEE_FS_DIR_ITERATOR_FLAGS_CTX1)==0) {
  self->di_flags |= DEE_FS_DIR_ITERATOR_FLAGS_CTX1;
  Dee_INCREF(*result = (DeeObject *)&_uptime);
  return 0;
 }
 if (self->di_vfs.div_elemiter == self->di_vfs.div_elemend) return 1;
 if DEE_UNLIKELY((*result = DEE_PP_CAT_2(DeeString_FromUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_DEE_PROCESS_ID))
  (DeeProcess_ID((DeeObject *)(*(*((DeeProcessObject ***)&self->di_vfs.div_elemiter))++)))) == NULL) return -1;
 return 0;
}



static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsproc_cmdline_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 DeeObject *process_ref,*exe_string; int result;
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE)!=DEE_FILE_MODE_READ) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_READ);
  return -1;
 }
 if DEE_UNLIKELY((process_ref = DeeProcess_Win32NewFromID((
  DeeProcessID)self->vf_flags)) == NULL) return -1;
 exe_string = DeeProcess_Utf8Cmd(process_ref);
 Dee_DECREF(process_ref);
 if DEE_UNLIKELY(!exe_string) return -1;
 result = _vfs_fileio_open_stringmemory(fp,exe_string);
 Dee_DECREF(exe_string);
 return result;
}
static DeeObject *DEE_CALL _vfsproc_exe_link(
 DEE_A_IN struct DeeVFSFile const *self) {
 DeeObject *process_ref,*exe_string;
 if DEE_UNLIKELY((process_ref =  DeeProcess_Win32NewFromID((
  DeeProcessID)self->vf_flags)) == NULL) return NULL;
 exe_string = DeeProcess_Utf8Exe(process_ref);
 Dee_DECREF(process_ref);
 return exe_string;
}
static DeeObject *DEE_CALL _vfsproc_cwd_link(
 DEE_A_IN struct DeeVFSFile const *self) {
 DeeObject *process_ref,*exe_string;
 if DEE_UNLIKELY((process_ref = DeeProcess_Win32NewFromID((
  DeeProcessID)self->vf_flags)) == NULL) return NULL;
 exe_string = DeeProcess_Utf8Cwd(process_ref);
 Dee_DECREF(process_ref);
 return exe_string;
}
static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsproc_uptime_open(
 DEE_A_IN struct DeeVFSFile const *self,
 DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode) {
 DeeObject *uptext; Dee_uint64_t uptime; int result;
 if DEE_UNLIKELY((mode&DEE_FILE_MASK_MODE)!=DEE_FILE_MODE_READ) {
  _vfserror_cannot_open_mode(self->vf_name,mode,DEE_FILE_MODE_READ);
  return -1;
 }
 if DEE_UNLIKELY(DeeDex_CallAndCastf(&uptime,"_system.uptime()!I64u") != 0) return -1;
 if DEE_UNLIKELY((uptext = DeeString_Newf("%Lf 0.0\n",((long double)uptime)/1000.0L)) == NULL) return -1;
 result = _vfs_fileio_open_stringmemory(fp,uptext);
 Dee_DECREF(uptext);
 return result;
}


static struct DeeVFSFile const _vfsproc_elem_files[] = {
 DeeVFSFile_INIT_FILE("cmdline",&_vfsproc_cmdline_open),
 {"exe",DEE_VFS_FILEATTRIBUTE_LINK|DEE_VFS_FILEATTRIBUTE_REFS|
  DEE_VFS_FILEATTRIBUTE_EXEC,0,NULL,NULL,NULL,NULL,&_vfsproc_exe_link,0},
 {"cwd",DEE_VFS_FILEATTRIBUTE_LINK|DEE_VFS_FILEATTRIBUTE_REFS|
  DEE_VFS_FILEATTRIBUTE_PATH,0,NULL,NULL,NULL,NULL,&_vfsproc_cwd_link,0},
 // TODO: environ
 // TODO: Other stuff
};
static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsproc_walk(
 DEE_A_IN struct DeeVFSFile const *DEE_UNUSED(self), DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end) {
 Dee_size_t path_size = (Dee_size_t)(path_end-path_begin);
 if (path_size == 4 && memcmp(path_begin,"self",4*sizeof(char)) == 0) {
  file->vf_flags = (Dee_uint64_t)GetCurrentProcessId();
 } else if (path_size == 6 && memcmp(path_begin,"uptime",6*sizeof(char)) == 0) {
  file->vf_name = "uptime";
  file->vf_attr = DEE_VFS_FILEATTRIBUTE_FILE|DEE_VFS_FILEATTRIBUTE_CHRD;
  file->vf_filec = 0;
  file->vf_filev = NULL;
  file->vf_open = &_vfsproc_uptime_open;
  file->vf_walk = NULL;
  file->vf_enum = NULL;
  file->vf_link = NULL;
  return 0;
 } else {
  if DEE_UNLIKELY(DeeString_ToUInt64WithLength(path_size,path_begin,&file->vf_flags) == -1) return -1;
  // FUTURE: Maybe check if this is a valid process id?
 }
 file->vf_name = NULL;
 file->vf_attr = DEE_VFS_FILEATTRIBUTE_PATH;
 file->vf_filec = sizeof(_vfsproc_elem_files)/sizeof(struct DeeVFSFile);
 file->vf_filev = _vfsproc_elem_files;
 file->vf_open = NULL;
 file->vf_walk = &_vfsdir_walk_flags;
 file->vf_enum = &_vfsdir_enum;
 file->vf_link = NULL;
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int DEE_CALL _vfsproc_enum(
 DEE_A_IN struct DeeVFSFile const *DEE_UNUSED(self), DEE_A_INOUT DeeFSDirIteratorObject *file) {
 if DEE_UNLIKELY((file->di_vfs.div_elemlist = DeeProcess_Enumerate()) == NULL) return -1;
 file->di_vfs.div_yield = &_vfsproc_enum_yield;
 file->di_vfs.div_elemiter = (void *)DeeList_ELEM(file->di_vfs.div_elemlist);
 file->di_vfs.div_elemend = (void *)(
  DeeList_ELEM(file->di_vfs.div_elemlist)+
  DeeList_SIZE(file->di_vfs.div_elemlist));
 file->di_flags = DEE_FS_DIR_ITERATOR_FLAGS_VFS;
 return 0;
}
#endif


static DeeObject *_vfstmp_link(
 DEE_A_IN struct DeeVFSFile const *DEE_UNUSED(self)) {
 return DeeFS_Utf8GetTmp();
}





static struct DeeVFSFile const _DeeVFS_RootDev[] = {
 DeeVFSFile_INIT_FILE("null",&_vfsdev_null_open),
 DeeVFSFile_INIT_FILE("stderr",&_vfsdev_stderr_open),
 DeeVFSFile_INIT_FILE("stdin",&_vfsdev_stdin_open),
 DeeVFSFile_INIT_FILE("stdout",&_vfsdev_stdout_open),
 DeeVFSFile_INIT_FILE("random",&_vfsdev_urandom_open), // todo: non-probabilistic RNG?
 DeeVFSFile_INIT_FILE("urandom",&_vfsdev_urandom_open),
 DeeVFSFile_INIT_FILE("zero",&_vfsdev_zero_open),
};
static struct DeeVFSFile const _DeeVFS_Root[] = {
 DeeVFSFile_INIT_DIR("dev",_DeeVFS_RootDev),
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
 DeeVFSFile_INIT_DIR_EX("proc",&_vfsproc_walk,&_vfsproc_enum),
#endif
 {"tmp",DEE_VFS_FILEATTRIBUTE_LINK|DEE_VFS_FILEATTRIBUTE_REFS|
  DEE_VFS_FILEATTRIBUTE_PATH,0,NULL,NULL,NULL,NULL,&_vfstmp_link,0},
};
static struct DeeVFSFile const _DeeVFS_FileSystemRoot = DeeVFSFile_INIT_DIR(NULL,_DeeVFS_Root);

#define MAKE_VFS_ISXXX_CHECK(utf8_name,wide_name,real_utf8_fs_check,real_wide_fs_check,attr_flag)\
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int)\
 utf8_name(DEE_A_IN_Z Dee_Utf8Char const *path)\
{\
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;\
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;\
 if (temp) return 0;\
 if (rfs_path) {\
  temp = real_utf8_fs_check(DeeUtf8String_STR(rfs_path));\
  Dee_DECREF(rfs_path);\
  return temp;\
 }\
 return (file.vf_attr&attr_flag)!=0;\
}\
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int)\
 wide_name(DEE_A_IN_Z Dee_WideChar const *path)\
{\
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;\
 if DEE_UNLIKELY((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;\
 if (temp) return 0;\
 if (rfs_path) {\
  temp = real_wide_fs_check(DeeWideString_STR(rfs_path));\
  Dee_DECREF(rfs_path);\
  return temp;\
 }\
 return (file.vf_attr&attr_flag)!=0;\
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_Utf8Exists(DeeUtf8String_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_WideExists(DeeWideString_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 return 1;
}
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsFile,DeeVFS_WideIsFile,_DeeFS_Utf8IsFile,_DeeFS_WideIsFile,DEE_VFS_FILEATTRIBUTE_FILE)
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsDir,DeeVFS_WideIsDir,_DeeFS_Utf8IsDir,_DeeFS_WideIsDir,DEE_VFS_FILEATTRIBUTE_PATH)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LLocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_Utf8IsLink(DeeUtf8String_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 return (file.vf_attr&DEE_VFS_FILEATTRIBUTE_LINK)!=0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_WideLLocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_WideIsLink(DeeWideString_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 return (file.vf_attr&DEE_VFS_FILEATTRIBUTE_LINK)!=0;
}
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsMount,DeeVFS_WideIsMount,_DeeFS_Utf8IsMount,_DeeFS_WideIsMount,DEE_VFS_FILEATTRIBUTE_MONT)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path; Dee_Utf8Char const *path_end;
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_Utf8IsHidden(DeeUtf8String_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_HIDN)!=0) return 1;
 if (file.vf_name) return *file.vf_name == '.';
 path_end = path+Dee_Utf8StrLen(path);
 while (path_end != path && !IS_SEP(path_end[-1])) --path_end;
 return *path_end == '.';
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSFile file; int temp; DeeObject *rfs_path; Dee_WideChar const *path_end;
 if DEE_UNLIKELY((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&rfs_path)) != 0) return -1;
 if (temp) return 0;
 if (rfs_path) {
  temp = _DeeFS_WideIsHidden(DeeWideString_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return temp;
 }
 if ((file.vf_attr&DEE_VFS_FILEATTRIBUTE_HIDN)!=0) return 1;
 if (file.vf_name) return *file.vf_name == '.';
 path_end = path+Dee_WideStrLen(path);
 while (path_end != path && !IS_SEP(path_end[-1])) --path_end;
 return *path_end == '.';
}
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsExecutable,DeeVFS_WideIsExecutable,_DeeFS_Utf8IsExecutable,_DeeFS_WideIsExecutable,DEE_VFS_FILEATTRIBUTE_EXEC)
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsCharDev,DeeVFS_WideIsCharDev,_DeeFS_Utf8IsCharDev,_DeeFS_WideIsCharDev,DEE_VFS_FILEATTRIBUTE_CHRD)
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsBlockDev,DeeVFS_WideIsBlockDev,_DeeFS_Utf8IsBlockDev,_DeeFS_WideIsBlockDev,DEE_VFS_FILEATTRIBUTE_BLKD)
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsFiFo,DeeVFS_WideIsFiFo,_DeeFS_Utf8IsFiFo,_DeeFS_WideIsFiFo,DEE_VFS_FILEATTRIBUTE_FIFO)
MAKE_VFS_ISXXX_CHECK(DeeVFS_Utf8IsSocket,DeeVFS_WideIsSocket,_DeeFS_Utf8IsSocket,_DeeFS_WideIsSocket,DEE_VFS_FILEATTRIBUTE_SOCK)
#undef MAKE_VFS_ISXXX_CHECK

static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Locate(
 DEE_A_IN_Z char const *path, DEE_A_OUT struct DeeVFSFile *file) {
 char const *path_iter,*path_end; struct DeeVFSFile temp_file; int temp;
 *file = _DeeVFS_FileSystemRoot,path_iter = path;
 while (1) {
  if DEE_UNLIKELY(file->vf_link) { // Read vf_link
   DeeObject *link_dst;
   if DEE_UNLIKELY((link_dst = (*file->vf_link)(file)) == NULL) return -1;
   temp = DeeVFS_Locate(DeeString_STR(link_dst),file);
   Dee_DECREF(link_dst);
   if (temp != 0) return temp;
  }
  while (IS_SEP(*path_iter)) ++path_iter;
  if DEE_UNLIKELY(!*path_iter) break;
  if DEE_UNLIKELY(!file->vf_walk) {
   DeeError_SetStringf(&DeeErrorType_SystemError,"Not a virtual path: %q",path);
   return -1;
  }
  path_end = path_iter;
  while (*path_end && !IS_SEP(*path_end)) ++path_end;
  if DEE_UNLIKELY((temp = (*file->vf_walk)(
   file,&temp_file,path_iter,path_end)) != 0) return temp;
  path_iter = path_end,*file = temp_file;
 }
 return 0;
}
#if 0
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_LLocate(
 DEE_A_IN_Z char const *path, DEE_A_OUT struct DeeVFSFile *file) {
 char const *path_iter,*path_end; struct DeeVFSFile temp_file; int temp;
 *file = _DeeVFS_FileSystemRoot,path_iter = path;
 while (1) {
  if DEE_UNLIKELY(!*path_iter) break;
  if (file->vf_link) { // Read vf_link
   DeeObject *link_dst;
   if DEE_UNLIKELY((link_dst = (*file->vf_link)(file)) == NULL) return -1;
   temp = DeeVFS_Locate(DeeString_STR(link_dst),file);
   Dee_DECREF(link_dst);
   if (temp != 0) return temp;
  }
  while (IS_SEP(*path_iter)) ++path_iter;
  if DEE_UNLIKELY(!*path_iter) break;
  if DEE_UNLIKELY(!file->vf_walk) {
   DeeError_SetStringf(&DeeErrorType_SystemError,"Not a virtual path: %q",path);
   return -1;
  }
  path_end = path_iter;
  while (*path_end && !IS_SEP(*path_end)) ++path_end;
  if DEE_UNLIKELY((temp = (*file->vf_walk)(file,&temp_file,path_iter,path_end)) != 0) return temp;
  path_iter = path_end,*file = temp_file;
 }
 return 0;
}
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeVFS_Utf8ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSFile vfs_file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LLocateOrReadReFsLink(path,&vfs_file,&rfs_path)) < 0) return NULL;
 if DEE_UNLIKELY(temp == 1) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Virtual file not found: %q",path);
  return NULL;
 }
 if (rfs_path) {
  DeeObject *result;
  result = _DeeFS_Utf8ReadLink(DeeUtf8String_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return result;
 }
 if DEE_UNLIKELY(!vfs_file.vf_link) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Virtual file is not a vf_link: %q",path);
  return NULL;
 }
 return (*vfs_file.vf_link)(&vfs_file);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeVFS_WideReadLink(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSFile vfs_file; int temp; DeeObject *rfs_path;
 if DEE_UNLIKELY((temp = DeeVFS_WideLLocateOrReadReFsLink(path,&vfs_file,&rfs_path)) < 0) return NULL;
 if DEE_UNLIKELY(temp == 1) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Virtual file not found: %lq",path);
  return NULL;
 }
 if (rfs_path) {
  DeeObject *result;
  result = _DeeFS_WideReadLink(DeeWideString_STR(rfs_path));
  Dee_DECREF(rfs_path);
  return result;
 }
 if DEE_UNLIKELY(!vfs_file.vf_link) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Virtual file is not a vf_link: %lq",path);
  return NULL;
 }
 return (*vfs_file.vf_link)(&vfs_file);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeVFS_Utf8ReadReFsLink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSFile file; int temp; DeeObject *result;
 if DEE_UNLIKELY((temp = DeeVFS_Utf8LocateOrReadReFsLink(path,&file,&result)) < 0) return NULL;
 if DEE_LIKELY(temp == 0) {
  if DEE_UNLIKELY(!result) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "Didn't expect virtual file %q",path);
  }
  return result;
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Virtual file not found: %q",path);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeVFS_WideReadReFsLink(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSFile file; int temp; DeeObject *result;
 if DEE_UNLIKELY((temp = DeeVFS_WideLocateOrReadReFsLink(path,&file,&result)) < 0) return NULL;
 if DEE_LIKELY(temp == 0) {
  if DEE_UNLIKELY(!result) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "Didn't expect virtual file %lq",path);
  }
  return result;
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Virtual file not found: %lq",path);
 return NULL;
}


DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Utf8LocateOrReadReFsLink(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeUtf8StringObject) **fs_link) {
 char const *path_iter,*path_end; struct DeeVFSFile temp_file; int temp;
 *file = _DeeVFS_FileSystemRoot,path_iter = path;
 while (1) {
  while (IS_SEP(*path_iter)) ++path_iter;
  if (file->vf_link) { // Read vf_link
   DeeObject *link_dst;
   if DEE_UNLIKELY((link_dst = (*file->vf_link)(file)) == NULL) return -1;
   if ((file->vf_attr&DEE_VFS_FILEATTRIBUTE_REFS)!=0) {
    // Read-filesystem vf_link
    if (path_iter != path && IS_SEP(path_iter[-1])) {
     if DEE_UNLIKELY(DeeUtf8String_Append(&link_dst,path_iter-1) != 0) {
      Dee_DECREF(link_dst);
      return -1;
     }
    } else if (*path_iter) {
     static char const sep_str[] = {SEP,0};
     if (DEE_UNLIKELY(DeeUtf8String_AppendWithLength(&link_dst,1,sep_str) != 0)
      || DEE_UNLIKELY(DeeUtf8String_Append(&link_dst,path_iter) != 0)) {
      Dee_DECREF(link_dst);
      return -1;
     }
    }
    *fs_link = link_dst;
    return 0;
   }
   temp = DeeVFS_Locate(DeeString_STR(link_dst),file);
   Dee_DECREF(link_dst);
   if DEE_UNLIKELY(temp != 0) return temp;
  }
  if (!*path_iter) break;
  if DEE_UNLIKELY(!file->vf_walk) {
   DeeError_SetStringf(&DeeErrorType_SystemError,"Not a virtual path: %q",path);
   return -1;
  }
  path_end = path_iter;
  while (*path_end && !IS_SEP(*path_end)) ++path_end;
  if ((temp = (*file->vf_walk)(file,&temp_file,path_iter,path_end)) != 0) return temp;
  path_iter = path_end,*file = temp_file;
 }
 *fs_link = NULL;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_WideLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeWideStringObject) **fs_link) {
 DeeObject *utf8_path; int result;
 if DEE_UNLIKELY((utf8_path = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeVFS_Utf8LocateOrReadReFsLink(DeeUtf8String_STR(utf8_path),file,fs_link);
 if (result == 0 && *fs_link && DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)fs_link) != 0)) {
  Dee_DECREF(*fs_link);
  return -1;
 }
 return result;
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Utf8LLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeUtf8StringObject) **fs_link) {
 char const *path_iter,*path_end; struct DeeVFSFile temp_file; int temp;
 *file = _DeeVFS_FileSystemRoot,path_iter = path;
 while (1) {
  while (IS_SEP(*path_iter)) ++path_iter;
  if (!*path_iter) break;
  if (file->vf_link) { // Read vf_link
   DeeObject *link_dst;
   if DEE_UNLIKELY((link_dst = (*file->vf_link)(file)) == NULL) return -1;
   if ((file->vf_attr&DEE_VFS_FILEATTRIBUTE_REFS)!=0) {
    // Read-filesystem vf_link
    if (path_iter != path && IS_SEP(path_iter[-1])) {
     if DEE_UNLIKELY(DeeUtf8String_Append(&link_dst,path_iter-1) != 0) {
      Dee_DECREF(link_dst);
      return -1;
     }
    } else if (*path_iter) {
     static char const sep_str[] = {SEP,0};
     if (DEE_UNLIKELY(DeeUtf8String_AppendWithLength(&link_dst,1,sep_str) != 0)
      || DEE_UNLIKELY(DeeUtf8String_Append(&link_dst,path_iter) != 0)) {
      Dee_DECREF(link_dst);
      return -1;
     }
    }
    *fs_link = link_dst;
    return 0;
   }
   temp = DeeVFS_Locate(DeeString_STR(link_dst),file);
   Dee_DECREF(link_dst);
   if DEE_UNLIKELY(temp != 0) return temp;
  }
  if DEE_UNLIKELY(!file->vf_walk) {
   DeeError_SetStringf(&DeeErrorType_SystemError,"Not a virtual path: %q",path);
   return -1;
  }
  path_end = path_iter;
  while (*path_end && !IS_SEP(*path_end)) ++path_end;
  if ((temp = (*file->vf_walk)(file,&temp_file,path_iter,path_end)) != 0) return temp;
  path_iter = path_end,*file = temp_file;
 }
 *fs_link = NULL;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_WideLLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeWideStringObject) **fs_link) {
 DeeObject *utf8_path; int result;
 if DEE_UNLIKELY((utf8_path = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeVFS_Utf8LLocateOrReadReFsLink(DeeUtf8String_STR(utf8_path),file,fs_link);
 if (DEE_UNLIKELY(result == 0) && *fs_link &&
     DEE_UNLIKELY(DeeWideString_InplaceCast((DeeObject const **)fs_link) != 0)) {
  Dee_DECREF(*fs_link);
  return -1;
 }
 return result;
}



DEE_DECL_END

#endif /* !GUARD_DEEMON_VIRTUAL_FS_C_INL */
