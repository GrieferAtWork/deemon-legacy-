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
#ifndef GUARD_DEEMON_VIRTUAL_FS_H_INL
#define GUARD_DEEMON_VIRTUAL_FS_H_INL 1

#include <deemon/fs_api.h>
#include <deemon/file.h>

DEE_DECL_BEGIN

#define DEE_VFS_FILEATTRIBUTE_NONE DEE_UINT16_C(0x0000)
#define DEE_VFS_FILEATTRIBUTE_FILE DEE_UINT16_C(0x0001) /*< Regular file. */
#define DEE_VFS_FILEATTRIBUTE_PATH DEE_UINT16_C(0x0002) /*< Path / Directory. */
#define DEE_VFS_FILEATTRIBUTE_LINK DEE_UINT16_C(0x0004) /*< Link. */
#define DEE_VFS_FILEATTRIBUTE_MONT DEE_UINT16_C(0x0008) /*< Mounting point. */
#define DEE_VFS_FILEATTRIBUTE_HIDN DEE_UINT16_C(0x0010) /*< Hidden file. */
#define DEE_VFS_FILEATTRIBUTE_EXEC DEE_UINT16_C(0x0020) /*< Executable. */
#define DEE_VFS_FILEATTRIBUTE_CHRD DEE_UINT16_C(0x0040) /*< Character device. */
#define DEE_VFS_FILEATTRIBUTE_BLKD DEE_UINT16_C(0x0080) /*< Block device. */
#define DEE_VFS_FILEATTRIBUTE_FIFO DEE_UINT16_C(0x0100) /*< Pipe. */
#define DEE_VFS_FILEATTRIBUTE_SOCK DEE_UINT16_C(0x0200) /*< Socket. */
#define DEE_VFS_FILEATTRIBUTE_REFS DEE_UINT16_C(0x0400) /*< A link that points back into the regular file system (e.g.: '/tmp/') */

struct DeeVFSFile;
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeVFSFileOpenProc)(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_INOUT DeeFileIOObject *fp, DEE_A_IN Dee_uint16_t mode);
typedef DEE_A_RET_EXCEPT_FAIL(-1,1) int (DEE_CALL *DeeVFSFileWalkProc)(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end);
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeVFSFileEnumProc)(
 DEE_A_IN struct DeeVFSFile const *self, DEE_A_INOUT DeeFSDirIteratorObject *file);
typedef DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *DeeVFSFileLinkProc)(DEE_A_IN struct DeeVFSFile const *self);
struct DeeVFSFile {
 char const              *vf_name;  /*< [0..1] Name (not an absolute path!). */
 Dee_uint16_t             vf_attr;  /*< File Attributes. */
 Dee_size_t               vf_filec; /*< Amount of underlying files (used if this is a folder). */
 struct DeeVFSFile const *vf_filev; /*< [0..filec] Vector of underlying files. */
 DeeVFSFileOpenProc       vf_open;  /*< [0..1] Open function. */
 DeeVFSFileWalkProc       vf_walk;  /*< [0..1] Walk function. */
 DeeVFSFileEnumProc       vf_enum;  /*< [0..1] Enumerate function. */
 DeeVFSFileLinkProc       vf_link;  /*< [0..1] Read link function. */
 Dee_uint64_t             vf_flags; /*< Custom file flags. */
};

#define DeeVFSFile_INIT_LINK(name,link)\
 {name,DEE_VFS_FILEATTRIBUTE_LINK,0,NULL,NULL,NULL,NULL,link,0}
#define DeeVFSFile_INIT_REGLINK(name,link)\
 {name,DEE_VFS_FILEATTRIBUTE_LINK|DEE_VFS_FILEATTRIBUTE_REFS,0,NULL,NULL,NULL,NULL,link,0}
#define DeeVFSFile_INIT_FILE(name,open)\
 {name,DEE_VFS_FILEATTRIBUTE_FILE,0,NULL,open,NULL,NULL,NULL,0}
#define DeeVFSFile_INIT_DIR(name,elem)\
 {name,DEE_VFS_FILEATTRIBUTE_PATH,\
  (sizeof(elem)/sizeof(struct DeeVFSFile)),\
  elem,NULL,&_vfsdir_walk,&_vfsdir_enum,NULL,0}
#define DeeVFSFile_INIT_DIR_EX(name,walk,enum)\
 {name,DEE_VFS_FILEATTRIBUTE_PATH,0,NULL,NULL,walk,enum,NULL,0}
static DEE_A_RET_EXCEPT_FAIL(-1,1) int _vfsdir_walk(DEE_A_IN struct DeeVFSFile const *self, DEE_A_OUT struct DeeVFSFile *file, DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end);
static DEE_A_RET_EXCEPT_FAIL(-1,1) int _vfsdir_walk_flags(DEE_A_IN struct DeeVFSFile const *self, DEE_A_OUT struct DeeVFSFile *file, DEE_A_IN char const *path_begin, DEE_A_IN char const *path_end);
static DEE_A_RET_EXCEPT(-1) int _vfsdir_enum(DEE_A_IN struct DeeVFSFile const *self, DEE_A_INOUT DeeFSDirIteratorObject *file);

#if 1 // For compatibility with network paths
#define DeeVFS_Utf8IsVirtualPath(path) ((path)[0]=='/'&&!DEE_FS_ISSEP((path)[1]))
#define DeeVFS_WideIsVirtualPath(path) ((path)[0]=='/'&&!DEE_FS_ISSEP((path)[1]))
#elif 1 // Only allow '/' for virtual paths (as to not interfere with network paths...)
#define DeeVFS_Utf8IsVirtualPath(path) ((path)[0]=='/')
#define DeeVFS_WideIsVirtualPath(path) ((path)[0]=='/')
#else
#define DeeVFS_Utf8IsVirtualPath(path) DEE_FS_ISSEP((path)[0])
#define DeeVFS_WideIsVirtualPath(path) DEE_FS_ISSEP((path)[0])
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8Exists(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsFile(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsDir(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsMount(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsHidden(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsExecutable(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsCharDev(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsBlockDev(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsFiFo(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_Utf8IsSocket(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideExists(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsFile(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsDir(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsLink(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsMount(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsHidden(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsExecutable(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsCharDev(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsBlockDev(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsFiFo(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeVFS_WideIsSocket(DEE_A_IN_Z Dee_WideChar const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeVFS_Utf8ReadLink(DEE_A_IN_Z Dee_Utf8Char const *path);
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeVFS_WideReadLink(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeVFS_Utf8ReadReFsLink(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeVFS_WideReadReFsLink(DEE_A_IN_Z Dee_WideChar const *path);

// These functions store reference to a filesystem path in 'fs_link' or set it to NULL
// to indicate that the given path isn't a link into the real filesystem.
// e.g.: 'DeeVFS_Utf8LocateOrReadReFsLink("/tmp/foobar")'
//       will store the equivalent of 'fs::path::join(fs::tmp(),"foobar")' in '*fs_link'
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Utf8LocateOrReadReFsLink(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeUtf8StringObject) **fs_link);
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_WideLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeWideStringObject) **fs_link);
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_Utf8LLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeUtf8StringObject) **fs_link);
static DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeVFS_WideLLocateOrReadReFsLink(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT struct DeeVFSFile *file,
 DEE_A_REF DEE_A_OUT_OBJECT(DeeWideStringObject) **fs_link);


DEE_DECL_END
#endif /* !GUARD_DEEMON_VIRTUAL_FS_HEAD */
