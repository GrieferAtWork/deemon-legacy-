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
#ifndef GUARD_DEEMON_VFS_VFS_CORE_H
#define GUARD_DEEMON_VFS_VFS_CORE_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/file.openmode.h>
#include DEE_INCLUDE_MEMORY_API()
#ifdef DEE_LIMITED_API
#include <deemon/sys/sysfd.h>
#endif

#include <deemon/fs_api.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_MODE_T
DEE_PRIVATE_DECL_DEE_MODE_T
#undef DEE_PRIVATE_DECL_DEE_MODE_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_UID_T
DEE_PRIVATE_DECL_DEE_UID_T
#undef DEE_PRIVATE_DECL_DEE_UID_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_GID_T
DEE_PRIVATE_DECL_DEE_GID_T
#undef DEE_PRIVATE_DECL_DEE_GID_T
#endif

// TODO: To have the new vfs-system fully working, we need to add another abstraction
//       layer, separating the native portions from the filesystem Apis.
#define DeeNativeFS_Utf8GetCwd  DeeFS_Utf8GetCwd
#define DeeNativeFS_WideGetCwd  DeeFS_WideGetCwd
#define DeeNativeFS_GetCwd      DeeNativeFS_Utf8GetCwd

#define DeeNativeFS_Utf8Chdir       _DeeFS_Utf8ChDir
#define DeeNativeFS_WideChdir       _DeeFS_WideChDir
#define DeeNativeFS_Utf8ChdirObject _DeeFS_ChDirObject
#define DeeNativeFS_WideChdirObject _DeeFS_ChDirObject
#define DeeNativeFS_Chdir           DeeNativeFS_Utf8Chdir
#define DeeNativeFS_ChdirObject     DeeNativeFS_Utf8ChdirObject

#define DeeNativeFS_Utf8Readlink       _DeeFS_Utf8ReadLink
#define DeeNativeFS_WideReadlink       _DeeFS_WideReadLink
#define DeeNativeFS_Utf8ReadlinkObject _DeeFS_ReadLinkObject
#define DeeNativeFS_WideReadlinkObject _DeeFS_ReadLinkObject
#define DeeNativeFS_Readlink           DeeNativeFS_Utf8Readlink
#define DeeNativeFS_ReadlinkObject     DeeNativeFS_Utf8ReadlinkObject

#define DeeNativeFS_Utf8IsFile       _DeeFS_Utf8IsFile
#define DeeNativeFS_WideIsFile       _DeeFS_WideIsFile
#define DeeNativeFS_Utf8IsFileObject _DeeFS_IsFileObject
#define DeeNativeFS_WideIsFileObject _DeeFS_IsFileObject
#define DeeNativeFS_IsFile           DeeNativeFS_Utf8IsFile
#define DeeNativeFS_IsFileObject     DeeNativeFS_Utf8IsFileObject

#define DeeNativeFS_Utf8IsDir       _DeeFS_Utf8IsDir
#define DeeNativeFS_WideIsDir       _DeeFS_WideIsDir
#define DeeNativeFS_Utf8IsDirObject _DeeFS_IsDirObject
#define DeeNativeFS_WideIsDirObject _DeeFS_IsDirObject
#define DeeNativeFS_IsDir           DeeNativeFS_Utf8IsDir
#define DeeNativeFS_IsDirObject     DeeNativeFS_Utf8IsDirObject

#define DeeNativeFS_Utf8IsLink       _DeeFS_Utf8IsLink
#define DeeNativeFS_WideIsLink       _DeeFS_WideIsLink
#define DeeNativeFS_Utf8IsLinkObject _DeeFS_IsLinkObject
#define DeeNativeFS_WideIsLinkObject _DeeFS_IsLinkObject
#define DeeNativeFS_IsLink           DeeNativeFS_Utf8IsLink
#define DeeNativeFS_IsLinkObject     DeeNativeFS_Utf8IsLinkObject

#define DeeNativeFS_Utf8IsHidden       _DeeFS_Utf8IsHidden
#define DeeNativeFS_WideIsHidden       _DeeFS_WideIsHidden
#define DeeNativeFS_Utf8IsHiddenObject _DeeFS_IsHiddenObject
#define DeeNativeFS_WideIsHiddenObject _DeeFS_IsHiddenObject
#define DeeNativeFS_IsHidden           DeeNativeFS_Utf8IsHidden
#define DeeNativeFS_IsHiddenObject     DeeNativeFS_Utf8IsHiddenObject

#define DeeNativeFS_Utf8IsCharDev       _DeeFS_Utf8IsCharDev
#define DeeNativeFS_WideIsCharDev       _DeeFS_WideIsCharDev
#define DeeNativeFS_Utf8IsCharDevObject _DeeFS_IsCharDevObject
#define DeeNativeFS_WideIsCharDevObject _DeeFS_IsCharDevObject
#define DeeNativeFS_IsCharDev           DeeNativeFS_Utf8IsCharDev
#define DeeNativeFS_IsCharDevObject     DeeNativeFS_Utf8IsCharDevObject

#define DeeNativeFS_Utf8IsBlockDev       _DeeFS_Utf8IsBlockDev
#define DeeNativeFS_WideIsBlockDev       _DeeFS_WideIsBlockDev
#define DeeNativeFS_Utf8IsBlockDevObject _DeeFS_IsBlockDevObject
#define DeeNativeFS_WideIsBlockDevObject _DeeFS_IsBlockDevObject
#define DeeNativeFS_IsBlockDev           DeeNativeFS_Utf8IsBlockDev
#define DeeNativeFS_IsBlockDevObject     DeeNativeFS_Utf8IsBlockDevObject

#define DeeNativeFS_Utf8IsFiFo       _DeeFS_Utf8IsFiFo
#define DeeNativeFS_WideIsFiFo       _DeeFS_WideIsFiFo
#define DeeNativeFS_Utf8IsFiFoObject _DeeFS_IsFiFoObject
#define DeeNativeFS_WideIsFiFoObject _DeeFS_IsFiFoObject
#define DeeNativeFS_IsFiFo           DeeNativeFS_Utf8IsFiFo
#define DeeNativeFS_IsFiFoObject     DeeNativeFS_Utf8IsFiFoObject

#define DeeNativeFS_Utf8IsSocket       _DeeFS_Utf8IsSocket
#define DeeNativeFS_WideIsSocket       _DeeFS_WideIsSocket
#define DeeNativeFS_Utf8IsSocketObject _DeeFS_IsSocketObject
#define DeeNativeFS_WideIsSocketObject _DeeFS_IsSocketObject
#define DeeNativeFS_IsSocket           DeeNativeFS_Utf8IsSocket
#define DeeNativeFS_IsSocketObject     DeeNativeFS_Utf8IsSocketObject

#define DeeNativeFS_Utf8GetMod       _DeeFS_Utf8GetMod
#define DeeNativeFS_WideGetMod       _DeeFS_WideGetMod
#define DeeNativeFS_Utf8GetModObject _DeeFS_GetModObject
#define DeeNativeFS_WideGetModObject _DeeFS_GetModObject
#define DeeNativeFS_GetMod           DeeNativeFS_Utf8GetMod
#define DeeNativeFS_GetModObject     DeeNativeFS_Utf8GetModObject

#define DeeNativeFS_Utf8GetOwn       _DeeFS_Utf8GetOwn
#define DeeNativeFS_WideGetOwn       _DeeFS_WideGetOwn
#define DeeNativeFS_Utf8GetOwnObject _DeeFS_GetOwnObject
#define DeeNativeFS_WideGetOwnObject _DeeFS_GetOwnObject
#define DeeNativeFS_GetOwn           DeeNativeFS_Utf8GetOwn
#define DeeNativeFS_GetOwnObject     DeeNativeFS_Utf8GetOwnObject

#define DeeNativeFS_Utf8Chmod       _DeeFS_Utf8Chmod
#define DeeNativeFS_WideChmod       _DeeFS_WideChmod
#define DeeNativeFS_Utf8ChmodObject _DeeFS_ChmodObject
#define DeeNativeFS_WideChmodObject _DeeFS_ChmodObject
#define DeeNativeFS_Chmod           DeeNativeFS_Utf8Chmod
#define DeeNativeFS_ChmodObject     DeeNativeFS_Utf8ChmodObject

#define DeeNativeFS_Utf8Chown       _DeeFS_Utf8Chown
#define DeeNativeFS_WideChown       _DeeFS_WideChown
#define DeeNativeFS_Utf8ChownObject _DeeFS_ChownObject
#define DeeNativeFS_WideChownObject _DeeFS_ChownObject
#define DeeNativeFS_Chown           DeeNativeFS_Utf8Chown
#define DeeNativeFS_ChownObject     DeeNativeFS_Utf8ChownObject




struct DeeVFSNode;
struct DeeVFSFile;
struct DeeVFSView;

struct _DeeVFSNodeTypeData {
 Dee_size_t vnt_size; /*< Instance size. */
 // +++ Node callbacks
 void                                          (DEE_CALL *vnt_quit)(DEE_A_INOUT struct DeeVFSNode *self); /*< destructor. */
 DEE_A_RET_EXCEPT_REF struct DeeVFSNode       *(DEE_CALL *vnt_walk)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN_Z char const *name); /*< a/b/c/d... */
 DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *(DEE_CALL *vnt_nameof)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_INOUT struct DeeVFSNode *child); /*< Returns the name of a given child. */
 DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *(DEE_CALL *vnt_link)(DEE_A_INOUT struct DeeVFSNode *self); /*< readlink. */
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_isfile)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_isdir)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_islink)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_ishidden)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_ischardev)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_isblockdev)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_isfifo)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT_FAIL(-1,0) int               (DEE_CALL *vnt_issocket)(DEE_A_INOUT struct DeeVFSNode *self);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_getmod)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT Dee_mode_t *mode);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_chmod)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN Dee_mode_t mode);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_getown)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_chown)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_gettimes)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
 DEE_A_RET_EXCEPT(-1) int                      (DEE_CALL *vnt_settimes)(DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime);
 // --- Node callbacks
};
struct _DeeVFSFileTypeData {
 Dee_size_t vft_size;  /*< Instance size. */
 // +++ File callbacks
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_open )(DEE_A_INOUT struct DeeVFSFile *self, DEE_A_IN Dee_openmode_t openmode, Dee_mode_t permissions); /*< open (Only initialize non-common data). */
                     void (DEE_CALL *vft_quit )(DEE_A_INOUT struct DeeVFSFile *self); /*< Only finalize 'DeeVFSFileData' */
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_read )(DEE_A_INOUT struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs);
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_write)(DEE_A_INOUT struct DeeVFSFile *self, void const *p, Dee_size_t s, Dee_size_t *ws);
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_seek )(DEE_A_INOUT struct DeeVFSFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos);
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_flush)(DEE_A_INOUT struct DeeVFSFile *self);
 DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vft_trunc)(DEE_A_INOUT struct DeeVFSFile *self);
 // --- File callbacks
};
struct _DeeVFSViewTypeData {
 Dee_size_t vvt_size; /*< Instance size. */
 // +++ View callbacks
        DEE_A_RET_EXCEPT(-1) int (DEE_CALL *vvt_open)(DEE_A_INOUT struct DeeVFSView *self); /*< opendir (Only initialize non-common data). */
                            void (DEE_CALL *vvt_quit)(DEE_A_INOUT struct DeeVFSView *self);
 DEE_A_RET_EXCEPT_FAIL(-1,1) int (DEE_CALL *vvt_yield)(DEE_A_INOUT struct DeeVFSView *self, struct DeeVFSNode **result);
 // --- View callbacks
};

struct DeeVFSNodeType {
 // NOTE: All callbacks can be NULL, which is considered Not-Implemented
 struct _DeeVFSNodeTypeData vnt_node;
 struct _DeeVFSFileTypeData vnt_file;
 struct _DeeVFSViewTypeData vnt_view;
};
#define DeeVFSNodeType_AllocFile(self) ((struct DeeVFSFile *)(DEE_ASSERTF((self)->vnt_file.vft_size,"Node type doesn't support file-routines"),malloc_nz((self)->vnt_file.vft_size)))
#define DeeVFSNodeType_AllocNode(self) ((struct DeeVFSNode *)malloc_nz((self)->vnt_node.vnt_size))
#define DeeVFSNodeType_AllocView(self) ((struct DeeVFSView *)(DEE_ASSERTF((self)->vnt_view.vvt_size,"Node type doesn't support view-routines"),malloc_nz((self)->vnt_view.vvt_size)))


struct DeeVFSView {
 DEE_A_REF struct DeeVFSNode           *vv_node; /*< [1..1] Node opened. */
           struct DeeVFSNodeType const *vv_type; /*< [1..1] Node type (alias for 'vf_node->vn_type'). */
 /* User-specific data... */
};
#define DeeVFSView_Quit(self)         (*(self)->vv_type->vnt_view_quit)(self)
#define DeeVFSView_Delete(self)       (DeeVFSView_Quit(self),free_nn(self))
#define DeeVFSView_Yield(self,result) (*(self)->vv_type->vnt_view_yield)(self,result)

struct DeeVFSFile {
 DEE_A_REF struct DeeVFSNode           *vf_node; /*< [1..1] Node opened. */
           struct DeeVFSNodeType const *vf_type; /*< [1..1] Node type (alias for 'vf_node->vn_type'). */
 /* User-specific data... */
};
#define DeeVFSFile_HasRead(self)             ((self)->vf_type->vnt_file.vft_read!=NULL)
#define DeeVFSFile_HasWrite(self)            ((self)->vf_type->vnt_file.vft_write!=NULL)
#define DeeVFSFile_HasSeek(self)             ((self)->vf_type->vnt_file.vft_seek!=NULL)
#define DeeVFSFile_HasFlush(self)            ((self)->vf_type->vnt_file.vft_flush!=NULL)
#define DeeVFSFile_HasTrunc(self)            ((self)->vf_type->vnt_file.vft_trunc!=NULL)
#define DeeVFSFile_Quit(self)                ((self)->vf_type->vnt_file_quit ? (*(self)->vf_type->vnt_file_quit)(self) : (void)0)
#define DeeVFSFile_Delete(self)              (DeeVFSFile_Quit(self),free_nn(self))
#define DeeVFSFile_Read(self,p,s,rs)         (DEE_ASSERTF(DeeVFSFile_HasRead(self),"Can't read file %R",DeeVFSNode_Filename(self->vf_node)),(*(self)->vf_type->vnt_file.vft_read)(self,p,s,rs))
#define DeeVFSFile_Write(self,p,s,ws)        (DEE_ASSERTF(DeeVFSFile_HasWrite(self),"Can't write file %R",DeeVFSNode_Filename(self->vf_node)),(*(self)->vf_type->vnt_file.vft_write)(self,p,s,ws))
#define DeeVFSFile_Seek(self,off,whence,pos) (DEE_ASSERTF(DeeVFSFile_HasSeek(self),"Can't seek file %R",DeeVFSNode_Filename(self->vf_node)),(*(self)->vf_type->vnt_file.vft_seek)(self,off,whence,pos))
#define DeeVFSFile_Flush(self)               (DEE_ASSERTF(DeeVFSFile_HasFlush(self),"Can't flush file %R",DeeVFSNode_Filename(self->vf_node)),(*(self)->vf_type->vnt_file.vft_flush)(self))
#define DeeVFSFile_Trunc(self)               (DEE_ASSERTF(DeeVFSFile_HasTrunc(self),"Can't trunc file %R",DeeVFSNode_Filename(self->vf_node)),(*(self)->vf_type->vnt_file.vft_trunc)(self))


typedef Dee_uint16_t Dee_vfsnode_refcnt_t;
struct DeeVFSNode {
 /*atomic*/Dee_vfsnode_refcnt_t vn_refcnt; /*< Reference counter. */
 struct DeeVFSNodeType const   *vn_type;   /*< [1..1] The type of node. */
 DEE_A_REF struct DeeVFSNode   *vn_parent; /*< [0..1] Parent directory. */
 /* User-specific data... */
};
#define DeeVFSNode_INIT(type,parent) {1,type,parent}

#define DeeVFSNode_ALLOC DeeObject_MALLOC
#define DeeVFSNode_FREE  DeeObject_Free
#define DeeVFSNode_InitWithParent(self,type,parent)\
 ((self)->vn_refcnt = 1\
 ,(self)->vn_type = (type)\
 ,(self)->vn_parent,DeeVFSNode_INCREF(parent))


extern void _DeeVFSNode_Delete(DEE_A_IN struct DeeVFSNode *self);
#define DeeVFSNode_DECREF(self) (!DeeAtomic16_DecFetch((self)->vn_refcnt,memory_order_seq_cst)?_DeeVFSNode_Delete(self):(void)0)
#define DeeVFSNode_INCREF(self) (void)DeeAtomic16_FetchInc((self)->vn_refcnt,memory_order_seq_cst)

extern DEE_A_RET_EXCEPT(-1) int DeeVFSNode_WriteFilename(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN struct DeeVFSNode const *node);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeVFSNode_Filename(DEE_A_IN struct DeeVFSNode const *node);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeVFSNode_Pathname(DEE_A_IN struct DeeVFSNode const *node);

#define DeeVFSNode_FileHasOpen(self)   ((self)->vn_type->vnt_file.vft_open!=NULL)
#define DeeVFSNode_FileHasView(self)   ((self)->vn_type->vnt_view.vvt_open!=NULL)
#define DeeVFSNode_FileHasLink(self)   ((self)->vn_type->vnt_node.vnt_link!=NULL)
#define DeeVFSNode_FileHasWalk(self)   ((self)->vn_type->vnt_node.vnt_walk!=NULL)


#define DeeVFSNode_IsNative(self) ((self)->vn_type == &DeeVFSNativeNode_Type)

//////////////////////////////////////////////////////////////////////////
// Generic node attribute query functions
#define DeeVFSNode_IsFile(self)     ((self)->vn_type->vnt_node.vnt_isfile ? (*(self)->vn_type->vnt_node.vnt_isfile)(self) : DeeVFSNode_FileHasOpen(self))
#define DeeVFSNode_IsDir(self)      ((self)->vn_type->vnt_node.vnt_isdir  ? (*(self)->vn_type->vnt_node.vnt_isdir )(self) : DeeVFSNode_FileHasView(self))
#define DeeVFSNode_IsLink(self)     ((self)->vn_type->vnt_node.vnt_islink ? (*(self)->vn_type->vnt_node.vnt_islink)(self) : DeeVFSNode_FileHasLink(self))
#define DeeVFSNode_IsHidden(self)   ((self)->vn_type->vnt_node.vnt_ishidden ? (*(self)->vn_type->vnt_node.vnt_ishidden)(self) : (self)->vn_name[0] == '.')
#define DeeVFSNode_IsCharDev(self)  ((self)->vn_type->vnt_node.vnt_ischardev && (*(self)->vn_type->vnt_node.vnt_ischardev)(self))
#define DeeVFSNode_IsBlockDev(self) ((self)->vn_type->vnt_node.vnt_isblockdev && (*(self)->vn_type->vnt_node.vnt_isblockdev)(self))
#define DeeVFSNode_IsFiFo(self)     ((self)->vn_type->vnt_node.vnt_isfifo && (*(self)->vn_type->vnt_node.vnt_isfifo)(self))
#define DeeVFSNode_IsSocket(self)   ((self)->vn_type->vnt_node.vnt_issocket && (*(self)->vn_type->vnt_node.vnt_issocket)(self))
#define DeeVFSNode_IsDrive(self)    (((self)->vn_flags&DEE_VFSNODE_FLAG_DRIVE)!=0)


#define DeeVFSNode_Walk(self,elemname) \
 (DEE_ASSERTF(DeeVFSNode_FileHasWalk(self),"Can't walk node %r",DeeVFSNode_Filename(self)),\
             (*(self)->vn_type->vnt_node.vnt_walk)(self,elemname))

#define DeeVFSNode_ReadLink(self) \
 (DEE_ASSERTF(DeeVFSNode_FileHasWalk(self),"Node is not a link: %r",DeeVFSNode_Filename(self)),\
             (*(self)->vn_type->vnt_node.vnt_link)(self))

struct DeeVFSLocateState;
//////////////////////////////////////////////////////////////////////////
// Walk the link of a given VFS node, returning a node pointing to the link destination.
// NOTE: Undefined behavior if 'DeeVFSNode_IsLink(self)' isn't true.
// NOTE: The walk is only performed once, meaning that the returned node may be another link
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFSNode_WalkLink_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state, DEE_A_INOUT struct DeeVFSNode *self);

#define _DeeVFSNode_GetViewBufferSize(self) (self)->vn_type->vnt_file.vft_size
#define _DeeVFSNode_OpenViewBuffer(self)\
 (DEE_ASSERTF(DeeVFSNode_FileHasOpen(self),"Can't view node %r",DeeVFSNode_Filename(self)),\
  DeeVFSNode_INCREF(self),((struct DeeVFSView *)(buf))->vv_node = (self),\
                          ((struct DeeVFSView *)(buf))->vv_type = (self)->vn_type,\
  ((*(self)->vn_type->vnt_view.vvt_open)(ob,(struct DeeVFSView *)(buf)) != 0)\
  ? (DeeVFSNode_DECREF(self),-1) : 0)


// Size of the buffer that should be passed to '_DeeVFSNode_OpenFileBuffer'.
#define _DeeVFSNode_GetFileBufferSize(self) (self)->vn_type->vnt_file.vft_size
// Opens the file of a node inside the given buffer memory
//  - Returns non-zero on error.
#define _DeeVFSNode_OpenFileBuffer(self,buf,mode,perms)\
 (DEE_ASSERTF(DeeVFSNode_FileHasOpen(self),"Can't open node %r",DeeVFSNode_Filename(self)),\
  DeeVFSNode_INCREF(self),((struct DeeVFSFile *)(buf))->vf_node = (self),\
                          ((struct DeeVFSFile *)(buf))->vf_type = (self)->vn_type,\
  ((*(self)->vn_type->vnt_file.vft_open)(ob,(struct DeeVFSFile *)(buf),mode,perms) != 0)\
  ? (DeeVFSNode_DECREF(self),-1) : 0)


#define DEE_VFS_MAX_LINK_INDIRECTION 1024
struct DeeVFSLocateState {
 char const  *vld_startpath; /*< [1..1] Original start path. */
 unsigned int vld_link_ind;  /*< Current level of link indirection. */
};

extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LLocateAt_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path);
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LocateAt_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path);

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF struct DeeVFSNode *) DeeVFS_LLocateAt(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 state.vld_link_ind = 0,state.vld_startpath = path;
 return DeeVFS_LLocateAt_impl(&state,root,path);
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF struct DeeVFSNode *) DeeVFS_LocateAt(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 state.vld_link_ind = 0,state.vld_startpath = path;
 return DeeVFS_LocateAt_impl(&state,root,path);
}

#define DEE_VFS_SEP    '/'
#define DEE_VFS_ALTSEP '\\'
#define DEE_VFS_ISSEP(c) ((c)==DEE_VFS_SEP||(c)==DEE_VFS_ALTSEP)

//////////////////////////////////////////////////////////////////////////
// A path can be forced to be virtual when its prepended by a '/'
#define DeeVFS_Utf8IsVirtualPath(path) (*(path)==   DEE_CHAR8_C('/'))
#define DeeVFS_WideIsVirtualPath(path) (*(path)==DEE_WIDECHAR_C('/'))

#ifdef DEE_PLATFORM_WINDOWS
#define DeeVFS_Utf8IsAbsoluteNativePath(path) (*(path)&&(path)[1]==   DEE_CHAR8_C(':'))
#define DeeVFS_WideIsAbsoluteNativePath(path) (*(path)&&(path)[1]==DEE_WIDECHAR_C(':'))
#else
#define DeeVFS_Utf8IsAbsoluteNativePath(path) 0
#define DeeVFS_WideIsAbsoluteNativePath(path) 0
#endif

//////////////////////////////////////////////////////////////////////////
// Locate the node associated with a given path, and return a reference to it.
// -> Returns NULL if the node could not be found
// NOTE: 'DeeVFS_LLocate' will return a link if the last portion of the given path
//       is a symbolic link, while 'DeeVFS_Locate' still follows that link.
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LLocate(DEE_A_IN_Z char const *path);
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_Locate(DEE_A_IN_Z char const *path);

//////////////////////////////////////////////////////////////////////////
// Locates a native file/directory, and returns a virtual node associated with it.
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LocateNative(DEE_A_IN_Z char const *path);

//////////////////////////////////////////////////////////////////////////
// Get/Set the current directory within the virtual file System.
// NOTE: Deleting the cwd will opt the current virtual directory to mirror the native one.
// NOTE: if the given node is part of the native filesystem, the native CWD will be updated.
extern DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_GetCwdNode(void);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_SetCwdNode(DEE_A_IN struct DeeVFSNode *cwd);
extern void DeeVFS_DelCwdNode(void);


//////////////////////////////////////////////////////////////////////////
// Get/Set the CWD path.
// NOTE: The path is not required to be virtual. And if it isn't, the
//       native CWD will be modified according to a given absolut path,
//       or the previous virtual/native current working directory.
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeVFS_Utf8GetCwd(void);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeVFS_WideGetCwd(void);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Chdir(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT(-1) int DeeVFS_WideChdir(DEE_A_IN_Z Dee_WideChar const *path);


//////////////////////////////////////////////////////////////////////////
// Convert a (possibly) virtual path into a native one.
// NOTE: Throw an Error.SystemError if the given path is virtual, and not part of the native filesystem.
// These functions are used for calls such as LoadLibrary, allowing the use
// of virtual paths (path point back into the real filesystem) even there.
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeVFS_Utf8ForceNativePathObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeVFS_WideForceNativePathObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeVFS_Utf8ForceNativePath(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeVFS_WideForceNativePath(DEE_A_IN_Z Dee_WideChar const *path);


//////////////////////////////////////////////////////////////////////////
// Shutdown the virtual filesystem (called during Dee_Finalize)
extern void DeeVFS_Shutdown(void);

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_CORE_H */
