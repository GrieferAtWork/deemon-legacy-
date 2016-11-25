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
#ifndef GUARD_DEEMON_FILE_VFS_C_INL
#define GUARD_DEEMON_FILE_VFS_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/file.h>
#include <deemon/file/file.fd.h>
#include <deemon/file/file.vfs.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN


DEE_A_RET_EXCEPT_REF DeeFileVFSObject *DeeFileVFS_OpenNode(
 DEE_A_INOUT struct DeeVFSNode *node, DEE_A_IN Dee_openmode_t openmode,
 DEE_A_IN Dee_mode_t permissions) {
 DeeFileVFSObject *result; Dee_size_t file_size;
 if (!DeeVFSNode_HasOpen(node)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't open virtual node: %R",
                      DeeVFSNode_Filename(node));
  return NULL;
 }
 file_size = Dee_OFFSETOF(DeeFileVFSObject,vf_file)+
             _DeeVFSNode_GetFileBufferSize(node);
 if ((result = (DeeFileVFSObject *)DeeObject_Malloc(file_size)) == NULL) return NULL;
 if (_DeeVFSNode_OpenFileBuffer(node,&result->vf_file,openmode,permissions) != 0) {
  DeeObject_Free(result);
  return NULL;
 }
 DeeAtomicMutex_Init(&result->fo_lock);
 result->fo_flags = DEE_PRIVATE_FILEFLAG_VFS_VALID;
 result->vf_users = 0;
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileVFS_Type);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeFileVFS_Filename(DEE_A_IN DeeFileVFSObject const *self) {
 DeeObject *result;
 DeeFileVFS_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return NULL;
 });
 result = DeeVFSNode_Filename(self->vf_file.vf_node);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}




DeeString_NEW_STATIC_EX(_deefilevfs_default_mode,1,{'r'});
static DeeFileVFSObject *DEE_CALL _deefilevfs_tp_any_new(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeObject *args) {
 DeeAnyStringObject *file; Dee_openmode_t openmode;
 DeeFileVFSObject *result; struct DeeVFSNode *filenode;
 Dee_mode_t permissions = DEE_FILEIO_DEFAULT_PERMISSIONS;
 DeeStringObject *mode = (DeeStringObject *)&_deefilevfs_default_mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oI" DEE_PP_STR(DEE_PP_MUL8(
  DEE_TYPES_SIZEOF_MODE_T)) "u:file.vfs",&file,&mode,&permissions) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)mode,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY((file = (DeeAnyStringObject *)DeeString_Cast((DeeObject *)file)) == NULL) return NULL;
 openmode = DeeFile_TryParseMode(DeeString_STR(mode));
 filenode = DeeVFS_Locate(DeeString_STR(file));
 Dee_DECREF(file);
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeFileVFS_OpenNode(filenode,openmode,permissions);
 DeeVFSNode_DECREF(filenode);
 return result;
}
static void DEE_CALL _deefilevfs_tp_dtor(DeeFileVFSObject *self) {
 if ((self->fo_flags&DEE_PRIVATE_FILEFLAG_VFS_VALID)!=0) DeeVFSFile_Quit(&self->vf_file);
}
static DeeObject *DEE_CALL _deefilevfs_tp_str(DeeFileVFSObject *self) {
 return DeeString_Newf("<file.vfs(%R)>",DeeFileVFS_Filename(self));
}


static int DEE_CALL _deefilevfs_tp_io_read(
 DeeFileVFSObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 int result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileVFS_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
 if (!DeeVFSFile_HasRead(&self->vf_file)) {
  DeeFileVFS_RELEASE_SHARED(self);
  DeeError_NotImplemented_str("read");
  return -1;
 }
 result = DeeVFSFile_Read(&self->vf_file,p,s,rs);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}
static int DEE_CALL _deefilevfs_tp_io_write(
 DeeFileVFSObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 int result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileVFS_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
 if (!DeeVFSFile_HasWrite(&self->vf_file)) {
  DeeFileVFS_RELEASE_SHARED(self);
  DeeError_NotImplemented_str("write");
  return -1;
 }
 result = DeeVFSFile_Write(&self->vf_file,p,s,ws);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}
static int DEE_CALL _deefilevfs_tp_io_seek(
 DeeFileVFSObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 int result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileVFS_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
 if (!DeeVFSFile_HasSeek(&self->vf_file)) {
  DeeFileVFS_RELEASE_SHARED(self);
  DeeError_NotImplemented_str("seek");
  return -1;
 }
 result = DeeVFSFile_Seek(&self->vf_file,off,whence,pos);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}
static int DEE_CALL _deefilevfs_tp_io_flush(DeeFileVFSObject *self) {
 int result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileVFS_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
 if (!DeeVFSFile_HasFlush(&self->vf_file)) {
  DeeFileVFS_RELEASE_SHARED(self);
  DeeError_NotImplemented_str("flush");
  return -1;
 }
 result = DeeVFSFile_Flush(&self->vf_file);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}
static int DEE_CALL _deefilevfs_tp_io_trunc(DeeFileVFSObject *self) {
 int result;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileVFS_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return -1; });
 if (!DeeVFSFile_HasTrunc(&self->vf_file)) {
  DeeFileVFS_RELEASE_SHARED(self);
  DeeError_NotImplemented_str("trunc");
  return -1;
 }
 result = DeeVFSFile_Trunc(&self->vf_file);
 DeeFileVFS_RELEASE_SHARED(self);
 return result;
}
static void DEE_CALL _deefilevfs_tp_io_close(DeeFileVFSObject *self) {
 DeeFileVFS_ACQUIRE_EXCLUSIVE(self,return);
 DeeVFSFile_Quit(&self->vf_file);
 self->fo_flags &= ~(DEE_PRIVATE_FILEFLAG_VFS_VALID);
 DeeFileVFS_RELEASE_EXCLUSIVE(self);
}

DeeFileTypeObject DeeFileVFS_Type = {
 // TODO: tp_move_ctor + tp_move_assign
 // TODO: Member functions remenicent of file.io
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.vfs"),member(
    "(string filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "(string.wide filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "(file.io filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "\tThe file type used to access virtual files."),
   member(DEE_TYPE_FLAG_VAR_OBJECT),member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,null,null,member(&_deefilevfs_tp_any_new)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deefilevfs_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilevfs_tp_str),null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilevfs_tp_io_read),
  member(&_deefilevfs_tp_io_write),
  member(&_deefilevfs_tp_io_seek),
  member(&_deefilevfs_tp_io_flush),
  member(&_deefilevfs_tp_io_trunc),
  member(&_deefilevfs_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_FILE_VFS_C_INL */
