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
#ifndef GUARD_DEEMON_FILE_H
#define GUARD_DEEMON_FILE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/std/stdarg.h>
#ifdef DEE_LIMITED_DEX
#include <deemon/__bswap_intrin.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/object_decl.h>
#include <deemon/string.h>
#include <deemon/type.h>
#endif
#ifdef DEE_LIMITED_API
#include <deemon/error.h>
#include <deemon/object.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeFileObject);
DEE_OBJECT_DEF(DeeFileIteratorObject);
DEE_OBJECT_DEF(DeeFileWriterObject);
DEE_OBJECT_DEF(DeeFileReaderObject);
DEE_OBJECT_DEF(DeeFileJoinedObject);

#if DEE_PLATFORM_HAVE_IO
DEE_OBJECT_DEF(DeeFileIOObject);
#endif /* DEE_PLATFORM_HAVE_IO */

#ifdef DEE_PRIVATE_DECL_DEE_MODE_T
DEE_PRIVATE_DECL_DEE_MODE_T
#undef DEE_PRIVATE_DECL_DEE_MODE_T
#endif

#ifdef DEE_LIMITED_DEX
struct _DeeFileTypeIOOperators {
 DeeType_SLOT_TYPE(tp_io_read)  tp_io_read;  /*< [0..1] Read operator. */
 DeeType_SLOT_TYPE(tp_io_write) tp_io_write; /*< [0..1] Write operator. */
 DeeType_SLOT_TYPE(tp_io_seek)  tp_io_seek;  /*< [0..1] Seek operator. */
 DeeType_SLOT_TYPE(tp_io_flush) tp_io_flush; /*< [0..1] Flush operator. */
 DeeType_SLOT_TYPE(tp_io_trunc) tp_io_trunc; /*< [0..1] Trunc operator. */
 DeeType_SLOT_TYPE(tp_io_close) tp_io_close; /*< [0..1] Close operator. File must remain in consistent state! */
};
struct DeeFileTypeObject {
 DeeTypeObject                  tp_base;
 struct _DeeFileTypeIOOperators tp_io;
};
#define DEE_FILE_TYPE_OBJECT_FOOTER_v100 /* nothing (yet) */
#define DEE_FILE_TYPE_OBJECT_IO_v100(\
 _tp_read,_tp_write,_tp_seek,_tp_flush,_tp_trunc,_tp_close)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_read),DEE_TYPEMEMBER(_tp_read,DeeType_DEFAULT_SLOT(tp_io_read)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_write),DEE_TYPEMEMBER(_tp_write,DeeType_DEFAULT_SLOT(tp_io_write)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_seek),DEE_TYPEMEMBER(_tp_seek,DeeType_DEFAULT_SLOT(tp_io_seek)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_flush),DEE_TYPEMEMBER(_tp_flush,DeeType_DEFAULT_SLOT(tp_io_flush)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_trunc),DEE_TYPEMEMBER(_tp_trunc,DeeType_DEFAULT_SLOT(tp_io_trunc)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_io_close),DEE_TYPEMEMBER(_tp_close,DeeType_DEFAULT_SLOT(tp_io_close)))\
}
#endif /* DEE_LIMITED_DEX */

#define DeeFileType_Check(ob)      DeeObject_InstanceOf(ob,&DeeFileType_Type)
#define DeeFileType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeFileType_Type)
DEE_DATA_DECL(DeeTypeObject) DeeFileType_Type;

#define DEE_FILE_FLAG_NONE               0x00000000
#define DEE_FILE_FLAG_SKIP_READLINE_LF   0x00000001 /*< If true: skip the first character in read_line it it's a '\n'. */
#if DEE_PLATFORM_HAVE_IO
#define DEE_FILE_FLAG_IO_DELETE_ON_CLOSE 0x80000000 /*< Used by file.io (delete file when the handle is closed). */
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_FILE_FLAG_IO_VFSFILE         0x40000000 /*< Virtual file system file. */
#endif
#endif

#ifdef DEE_LIMITED_DEX
#define DEE_FILE_OBJECT_HEAD \
 DEE_OBJECT_HEAD_EX(DeeFileTypeObject) \
 struct DeeAtomicMutex fo_lock;  /*< File lock. */\
 Dee_uint32_t          fo_flags; /*< flags. */
struct DeeFileObject { DEE_FILE_OBJECT_HEAD }; // Common base class
#define DEE_FILE_OBJECT_HEAD_INIT(tp) \
 DEE_OBJECT_HEAD_INIT(tp),DeeAtomicMutex_INIT(),DEE_FILE_FLAG_NONE
#define DEE_FILE_OBJECT_HEAD_INIT_REF(tp,refs) \
 DEE_OBJECT_HEAD_INIT_REF(tp,refs),DeeAtomicMutex_INIT(),DEE_FILE_FLAG_NONE
#define DeeFile_LOCK(ob)    (&((DeeFileObject *)Dee_REQUIRES_POINTER(ob))->fo_lock)
#define DeeFile_ACQUIRE(ob) DeeAtomicMutex_AcquireRelaxed(DeeFile_LOCK(ob))
#define DeeFile_RELEASE(ob) DeeAtomicMutex_Release(DeeFile_LOCK(ob))
#define DeeFile_FLAGS(ob) ((DeeFileObject *)Dee_REQUIRES_POINTER(ob))->fo_flags
#define _DeeFile_Init(ob)\
 (DeeAtomicMutex_Init(&(ob)->fo_lock),\
 (ob)->fo_flags=DEE_FILE_FLAG_NONE)
#define _DeeFile_InitCopy(ob,right)\
 (DeeAtomicMutex_Init(&(ob)->fo_lock),\
 (ob)->fo_flags=(right)->fo_flags)
#define _DeeFile_InitMove _DeeFile_InitCopy
#endif


#define DEE_FILE_MODE_READ       DEE_UINT16_C(0x0000)
#define DEE_FILE_MODE_WRITE      DEE_UINT16_C(0x0001)
#define DEE_FILE_MODE_APPEND     DEE_UINT16_C(0x0002)
#define DEE_FILE_FLAG_UPDATE     DEE_UINT16_C(0x0004) // flag
#define DEE_FILE_MASK_MODE       DEE_UINT16_C(0x0003) // last 2 bits
#define DEE_FILE_MASK_BASIC_MODE DEE_UINT16_C(0x0007) // last 3 bits

#ifdef DEE_PRIVATE_DECL_DEE_IO_HANDLE
DEE_PRIVATE_DECL_DEE_IO_HANDLE
#undef DEE_PRIVATE_DECL_DEE_IO_HANDLE
#endif

#if DEE_PLATFORM_HAVE_IO
#ifdef DEE_LIMITED_DEX
#if DEE_CONFIG_RUNTIME_HAVE_VFS
struct DeeFileIOVFSType {
 int  (*vfs_read)(DeeFileIOObject *self, void *p, Dee_size_t s, Dee_size_t *rs);
 int  (*vfs_write)(DeeFileIOObject *self, void const *p, Dee_size_t s, Dee_size_t *ws);
 int  (*vfs_seek)(DeeFileIOObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos);
 int  (*vfs_flush)(DeeFileIOObject *self);
 int  (*vfs_trunc)(DeeFileIOObject *self);
 void (*vfs_close)(DeeFileIOObject *self);
};
#endif
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeFileIOObject {
 DEE_FILE_OBJECT_HEAD
union{
 DEE_A_REF DeeUtf8StringObject *fio_utf8file; /*< [1..1] Utf8 name of the file that was opened. */
 DEE_A_REF DeeWideStringObject *fio_widefile; /*< [1..1] Wide name of the file that was opened. */
}
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define fio_utf8file _fio_filename.fio_utf8file
#define fio_widefile _fio_filename.fio_widefile
 _fio_filename
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
 Dee_uint16_t                   fio_mode;     /*< The mode the file was opened under. */
#if DEE_PLATFORM_HAVE_IO_HANDLE || \
    DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H || \
    DEE_CONFIG_RUNTIME_HAVE_VFS
 union{
#if DEE_PLATFORM_HAVE_IO_HANDLE
  DeeIOHandle                   fio_handle;   /*< [0..1] File handle. */
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  /*FILE*/void                 *fio_handle;   /*< [0..1] File handle. */
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
  struct {
   struct DeeFileIOVFSType *fio_type; /*< [1..1] VFS Type. */
   void                    *fio_data; /*< [?..?] VFS Data. */
  } fio_vfs;
#endif
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#if DEE_PLATFORM_HAVE_IO_HANDLE || DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#define fio_handle _fio_handledata.fio_handle
#endif /* DEE_PLATFORM_HAVE_IO_HANDLE || DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H */
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define fio_vfs    _fio_handledata.fio_vfs
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
 _fio_handledata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
#endif
 ;
};
DEE_COMPILER_MSVC_WARNING_POP
#endif /* DEE_LIMITED_DEX */
#endif /* DEE_PLATFORM_HAVE_IO */


//////////////////////////////////////////////////////////////////////////
// Parse the open mode for a file
// >> Raises a ValueError if the mode is malformed
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeFile_ParseMode(
 DEE_A_IN_Z char const *mode, DEE_A_OUT Dee_uint16_t *result) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT(DeeStringObject) *)
 DeeFile_ModeToString(DEE_A_IN Dee_uint16_t mode);


#ifdef DEE_LIMITED_DEX
struct DeeFileWriterObject {
 DEE_FILE_OBJECT_HEAD
 char           *fw_pos;    /*< [0..1] Current position within the writer. */
 DeeStringWriter fw_writer; /*< The string writer we are writing to. */
};

struct DeeFileReaderObject {
 DEE_FILE_OBJECT_HEAD
 DEE_A_REF DeeAnyStringObject *fr_string; /*< [1..1] The string we are iterating. */
 Dee_uint8_t const            *fr_pos;    /*< [1..1] Current position within the string. */
 Dee_uint8_t const            *fr_end;    /*< [1..1] End of the string. */
};
#define _DeeFileReader_SET_STRING(ob,str_,on_err)\
do{\
 Dee_DECREF(((DeeFileReaderObject *)Dee_REQUIRES_POINTER(ob))->fr_string);\
 _DeeFileReader_SET_INITIAL_STRING(ob,str_,on_err);\
}while(0)
#define _DeeFileReader_SET_INITIAL_STRING(ob,str_,on_err)\
do{\
 if (DeeAnyString_Check(str_)) {\
  Dee_INCREF(((DeeFileReaderObject *)(ob))->fr_string = (DeeAnyStringObject *)Dee_REQUIRES_POINTER(str_));\
  ((DeeFileReaderObject *)(ob))->fr_end = (((DeeFileReaderObject *)(ob))->fr_pos = \
   (Dee_uint8_t *)DeeAnyString_DATAV(str_))+DeeAnyString_DATAC(str_);\
 } else if (DeeNone_Check(str_)) {\
  Dee_INCREF(((DeeFileReaderObject *)(ob))->fr_string = (DeeAnyStringObject *)Dee_EmptyString);\
  ((DeeFileReaderObject *)(ob))->fr_end = \
  ((DeeFileReaderObject *)(ob))->fr_pos = \
   (Dee_uint8_t *)DeeString_STR(Dee_EmptyString);\
 } else {\
  DeeError_SetStringf(&DeeErrorType_TypeError,"Unexpected type (expected: any string; got: %s)",\
                      DeeType_NAME(Dee_TYPE(str_)));\
  on_err;\
 }\
}while(0)


struct DeeFileJoinedObject {
 DEE_FILE_OBJECT_HEAD
 DEE_A_REF DeeFileObject *fj_fa; /*< [1..1] First output stream. */
 DEE_A_REF DeeFileObject *fj_fb; /*< [1..1] Second output stream. */
};
#define DeeFileJoined_FA(ob) ((DeeObject *)((DeeFileJoinedObject *)Dee_REQUIRES_POINTER(ob))->fj_fa)
#define DeeFileJoined_FB(ob) ((DeeObject *)((DeeFileJoinedObject *)Dee_REQUIRES_POINTER(ob))->fj_fb)
#endif /* !DEE_LIMITED_DEX */


// v 'DeeFile_Check' checks for any type that is derived from 'file'
//   - This applies to 'file', 'file.io', 'file.writer' and 'file.reader'
//     NOTE: Also checks out for additional user defined sub-classes
#define DeeFile_Check(ob)            DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFile_Type)
#define DeeFile_CheckExact(ob)       DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFile_Type)
#define DeeFileIO_Check(ob)          DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFileIO_Type)
#define DeeFileIO_CheckExact(ob)     DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFileIO_Type)
#define DeeFileWriter_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFileWriter_Type)
#define DeeFileWriter_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFileWriter_Type)
#define DeeFileReader_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFileReader_Type)
#define DeeFileReader_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFileReader_Type)
#define DeeFileJoined_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFileJoined_Type)
#define DeeFileJoined_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFileJoined_Type)

                                                     // -read- -write- -seek- -trunc-
DEE_DATA_DECL(DeeFileTypeObject) DeeFile_Type;       // yes     yes     yes    yes
#if DEE_PLATFORM_HAVE_IO
DEE_DATA_DECL(DeeFileTypeObject) DeeFileIO_Type;     // maybe   maybe   maybe  maybe
#endif
DEE_DATA_DECL(DeeFileTypeObject) DeeFileReader_Type; // yes     no      yes    no
DEE_DATA_DECL(DeeFileTypeObject) DeeFileWriter_Type; // yes     yes     yes    yes
DEE_DATA_DECL(DeeFileTypeObject) DeeFileJoined_Type; // no      maybe   maybe  maybe

#ifdef DEE_LIMITED_API
extern DeeFileTypeObject DeeFileWin32Dbg_Type;       // no      yes     no     no
#endif


#define DEE_SEEK_SET 0
#define DEE_SEEK_CUR 1
#define DEE_SEEK_END 2


#ifdef DEE_LIMITED_DEX
struct DeeFileIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeFileObject *fi_file; /*< [1..1] The file we are reading from. */
};
#endif

DEE_DATA_DECL(DeeTypeObject) DeeFileIterator_Type;
#define DeeFileIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeFileIterator_Type)
#define DeeFileIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeFileIterator_Type)



//////////////////////////////////////////////////////////////////////////
// Create the different file sub-classes
#define DeeFileWriter_New() DeeFileWriter_NewWithSizeHint(0)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileWriterObject) *)
 DeeFileWriter_NewWithSizeHint(DEE_A_IN Dee_size_t size_hint);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileReaderObject) *) DeeFileReader_New(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *str_) DEE_ATTRIBUTE_NONNULL((1));

#if DEE_PLATFORM_HAVE_IO
//////////////////////////////////////////////////////////////////////////
// Create a new file object
// >> DeeFileIO_NewTemporary()              -- Creates a temporary file (file will be deleted when closed if 'delete_when_closed != 0')
// >> DeeFileIO_New / DeeFileIO_NewObject   -- Creates a file at a given path (the path is expanded)
// >> _DeeFileIO_New / _DeeFileIO_NewObject -- Creates a file at a given path (the path is not expanded)
#define DEE_FILEIO_NEWTEMPORARY_FLAG_NONE               DEE_UINT32_C(0x00000000)
#define DEE_FILEIO_NEWTEMPORARY_FLAG_DELETE_WHEN_CLOSED DEE_UINT32_C(0x00000001)
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *)
 DeeFileIO_NewTemporary(DEE_A_IN Dee_uint32_t flags);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) DeeFileIO_Utf8NewEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) DeeFileIO_WideNewEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) DeeFileIO_NewObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) _DeeFileIO_Utf8NewEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) _DeeFileIO_WideNewEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) _DeeFileIO_NewObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2));
#define DEE_FILEIO_DEFAULT_PERMISSIONS 0644
#define DeeFileIO_Utf8New(file,mode)    DeeFileIO_Utf8NewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_WideNew(file,mode)    DeeFileIO_WideNewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_NewObject(file,mode)  DeeFileIO_NewObjectEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_New(file,mode)        DeeFileIO_NewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_NewEx                 DeeFileIO_Utf8NewEx
#define _DeeFileIO_Utf8New(file,mode)   _DeeFileIO_Utf8NewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_WideNew(file,mode)   _DeeFileIO_WideNewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_NewObject(file,mode) _DeeFileIO_NewObjectEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_New(file,mode)       _DeeFileIO_NewEx(file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_NewEx                _DeeFileIO_Utf8NewEx
#endif /* DEE_PLATFORM_HAVE_IO */

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIteratorObject) *DeeFile_IterSelf(
 DEE_A_INOUT_OBJECT(DeeFileObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TRead(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TWrite(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TReadAll(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TWriteAll(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TSeek(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_int64_t off, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *pos) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TFlush(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TTrunc(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeFile_TClose(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self) DEE_ATTRIBUTE_NONNULL((1,2));
#define /*DEE_A_EXEC*/ DeeFile_TTell(tp_fp,fp,pos)     DeeFile_TSeek(tp_fp,fp,0,DEE_SEEK_CUR,pos)
#define /*DEE_A_EXEC*/ DeeFile_TRewind(tp_fp,fp)       DeeFile_TSeek(tp_fp,fp,0,DEE_SEEK_SET,NULL)
#define /*DEE_A_EXEC*/ DeeFile_TSetPos(tp_fp,fp,pos)   DeeFile_TSeek(tp_fp,fp,(Dee_int64_t)(pos),DEE_SEEK_SET,NULL)
#define /*DEE_A_EXEC*/ DeeFile_Read(fp,p,s,rs)         DeeFile_TRead(Dee_TYPE(fp),fp,p,s,rs)
#define /*DEE_A_EXEC*/ DeeFile_Write(fp,p,s,ws)        DeeFile_TWrite(Dee_TYPE(fp),fp,p,s,ws)
#define /*DEE_A_EXEC*/ DeeFile_ReadAll(fp,p,s)         DeeFile_TReadAll(Dee_TYPE(fp),fp,p,s)
#define /*DEE_A_EXEC*/ DeeFile_WriteAll(fp,p,s)        DeeFile_TWriteAll(Dee_TYPE(fp),fp,p,s)
#define /*DEE_A_EXEC*/ DeeFile_Seek(fp,off,whence,pos) DeeFile_TSeek(Dee_TYPE(fp),fp,off,whence,pos)
#define /*DEE_A_EXEC*/ DeeFile_Flush(fp)               DeeFile_TFlush(Dee_TYPE(fp),fp)
#define /*DEE_A_EXEC*/ DeeFile_Trunc(fp)               DeeFile_TTrunc(Dee_TYPE(fp),fp)
#define /*DEE_A_EXEC*/ DeeFile_Close(fp)               DeeFile_TClose(Dee_TYPE(fp),fp)
#define /*DEE_A_EXEC*/ DeeFile_Tell(fp,pos)            DeeFile_TTell(Dee_TYPE(fp),fp,pos)
#define /*DEE_A_EXEC*/ DeeFile_Rewind(fp)              DeeFile_TRewind(Dee_TYPE(fp),fp)
#define /*DEE_A_EXEC*/ DeeFile_SetPos(fp,pos)          DeeFile_TSetPos(Dee_TYPE(fp),fp,pos)

#define DeeFile_PosAndSize(fp,pos,size)\
 DeeFile_TPosAndSize(Dee_TYPE(fp),fp,pos,size)
#define DeeFile_TPosAndSize(tp_fp,fp,pos,size)\
(DEE_UNLIKELY(\
 DeeFile_TTell(tp_fp,fp,pos) != 0 ||\
 DeeFile_TSeek(tp_fp,fp,0,DEE_SEEK_END,size) != 0 ||\
 DeeFile_TSeek(tp_fp,fp,*(Dee_int64_t*)(pos),DEE_SEEK_SET,NULL) != 0\
) ? -1 : 0)
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define /*DEE_A_EXEC*/ DeeFile_TSize(tp_fp,fp,size) DEE_GCC_EXTENSION({ Dee_uint64_t _temp_old_pos; DeeFile_TPosAndSize(tp_fp,fp,&_temp_old_pos,size); })
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TSize(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT Dee_uint64_t *size) { Dee_uint64_t old_pos; return DeeFile_TPosAndSize(tp_self,self,&old_pos,size); }
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define /*DEE_A_EXEC*/ DeeFile_Size(fp,size) DeeFile_TSize(Dee_TYPE(fp),fp,size)

// Read/Write structured objects (Throws an Error.TypeError if it's not a structured type)
#define DeeFile_WriteObject(fp,ob) DeeFile_TWriteObject(Dee_TYPE(fp),fp,ob)
#define DeeFile_ReadObject(fp,ob)  DeeFile_TReadObject(Dee_TYPE(fp),fp,ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TWriteObject(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN DeeObject const *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeFile_TReadObject(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN DeeTypeObject const *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));

#define DeeFile_Getc(fp,c)     DeeFile_TGetc(Dee_TYPE(fp),fp,c)
#define DeeFile_Putc(fp,c)     DeeFile_TPutc(Dee_TYPE(fp),fp,c)
#define DeeFile_Puts(fp,str_)  DeeFile_TPuts(Dee_TYPE(fp),fp,str_)
#define DeeFile_Print(fp,str_) DeeFile_TPrint(Dee_TYPE(fp),fp,str_)
#define DeeFile_Printf(fp,...) DeeFile_TPrintf(Dee_TYPE(fp),fp,__VA_ARGS__)
#define DeeFile_TPuts(tp_fp,fp,str_) (DEE_UNLIKELY(DeeFile_TPrint(tp_fp,fp,str_) != 0 || DeeFile_TPutc(tp_fp,fp,'\n') != 0) ? -1 : 0)
#define DeeFile_TPrint(tp_fp,fp,str_) DeeFile_TWriteAll(tp_fp,fp,str_,strlen(str_))
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TGetc(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT Dee_uint8_t *c) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TPutc(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint8_t c) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TPrintf(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_PRINTF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define DeeFile_VPrintf(fp,fmt,args) DeeFile_VTPrintf(Dee_TYPE(fp),fp,fmt,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_VTPrintf(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_PRINTF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
#define /*DEE_A_EXEC*/ DeeFile_PRINT(fp,str_)        DeeFile_TPRINT(Dee_TYPE(fp),fp,str_)
#define /*DEE_A_EXEC*/ DeeFile_TPRINT(tp_fp,fp,str_) DeeFile_TWriteAll(tp_fp,fp,str_,(sizeof(str_)/sizeof(char))-1)

//////////////////////////////////////////////////////////////////////////
// v Same as the '.read()' function of file objects
#define DeeFile_ReadData(fp,max_bytes) DeeFile_TReadData(Dee_TYPE(fp),fp,max_bytes)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeFile_TReadData(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_size_t max_bytes) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// v Same as the '.readall()' function of file objects
#define DeeFile_ReadAllData(fp,max_bytes) DeeFile_TReadAllData(Dee_TYPE(fp),fp,max_bytes)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeFile_TReadAllData(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_size_t max_bytes) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Reads one line from a file and returns it (suffixed with a '\n' character)
// - If no more data can be read, an empty string is returned
// - Returns NULL on error
// NOTE: This function is used to implement file iterators
#define DeeFile_ReadLine(fp,max_bytes) DeeFile_TReadLine(Dee_TYPE(fp),fp,max_bytes)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeFile_TReadLine(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_size_t max_bytes) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// High-level printer functions
// NOTE: These functions support super objects and non-file types
#define DeeFile_PrintOne(ob,v) DeeFile_TPrintOne(Dee_TYPE(ob),ob,Dee_TYPE(v),v)
#define DeeFile_PrintAll(ob,v) DeeFile_TPrintAll(Dee_TYPE(ob),ob,Dee_TYPE(v),v)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TPrintOne(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_TPrintAll(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));

#ifdef DEE_LIMITED_DEX
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TReadSwapped16(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT void *p) { int result = DeeFile_TReadAll(tp_self,self,p,2); if (result == 0) *(Dee_uint16_t *)p = DEE_BUILTIN_BESWAP16(*(Dee_uint16_t *)p); return result; }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TReadSwapped32(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT void *p) { int result = DeeFile_TReadAll(tp_self,self,p,4); if (result == 0) *(Dee_uint32_t *)p = DEE_BUILTIN_BESWAP32(*(Dee_uint32_t *)p); return result; }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TReadSwapped64(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT void *p) { int result = DeeFile_TReadAll(tp_self,self,p,8); if (result == 0) *(Dee_uint64_t *)p = DEE_BUILTIN_BESWAP64(*(Dee_uint64_t *)p); return result; }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWrite16(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint16_t x) { return DeeFile_TWriteAll(tp_self,self,&x,2); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWrite32(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint32_t x) { return DeeFile_TWriteAll(tp_self,self,&x,4); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWrite64(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t x) { return DeeFile_TWriteAll(tp_self,self,&x,8); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWriteSwapped16(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint16_t x) { x = DEE_BUILTIN_BESWAP16(x); return DeeFile_TWriteAll(tp_self,self,&x,2); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWriteSwapped32(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint32_t x) { x = DEE_BUILTIN_BESWAP32(x); return DeeFile_TWriteAll(tp_self,self,&x,4); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFile_TWriteSwapped64(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t x) { x = DEE_BUILTIN_BESWAP64(x); return DeeFile_TWriteAll(tp_self,self,&x,8); }
#ifdef DEE_PLATFORM_LIL_ENDIAN
#define _DeeFile_TGetLe16(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,2)
#define _DeeFile_TGetLe32(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,4)
#define _DeeFile_TGetLe64(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,8)
#define _DeeFile_TGetBe16             _DeeFile_TReadSwapped16
#define _DeeFile_TGetBe32             _DeeFile_TReadSwapped32
#define _DeeFile_TGetBe64             _DeeFile_TReadSwapped64
#define _DeeFile_TPutLe16             _DeeFile_TWrite16
#define _DeeFile_TPutLe32             _DeeFile_TWrite32
#define _DeeFile_TPutLe64             _DeeFile_TWrite64
#define _DeeFile_TPutBe16             _DeeFile_TWriteSwapped16
#define _DeeFile_TPutBe32             _DeeFile_TWriteSwapped32
#define _DeeFile_TPutBe64             _DeeFile_TWriteSwapped64
#else
#define _DeeFile_TGetLe16             _DeeFile_TReadSwapped16
#define _DeeFile_TGetLe32             _DeeFile_TReadSwapped32
#define _DeeFile_TGetLe64             _DeeFile_TReadSwapped64
#define _DeeFile_TGetBe16(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,2)
#define _DeeFile_TGetBe32(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,4)
#define _DeeFile_TGetBe64(tp_fp,fp,p) DeeFile_TReadAll(tp_fp,fp,p,8)
#define _DeeFile_TPutLe16             _DeeFile_TWriteSwapped16
#define _DeeFile_TPutLe32             _DeeFile_TWriteSwapped32
#define _DeeFile_TPutLe64             _DeeFile_TWriteSwapped64
#define _DeeFile_TPutBe16             _DeeFile_TWrite16
#define _DeeFile_TPutBe32             _DeeFile_TWrite32
#define _DeeFile_TPutBe64             _DeeFile_TWrite64
#endif
#if defined(__INTELLISENSE__) && defined(__cplusplus)
#define DeeFile_TGetLe(tp_fp,fp,lval)\
(([&]{\
 enum{lv_size=sizeof(lval)};\
 static_assert(lv_size == 1 || lv_size == 2 || lv_size == 4 || lv_size == 8,\
               "Cannot only read 1, 2, 4 or 8-byte objects");\
},DeeFile_TGetc(tp_fp,fp,(Dee_uint8_t *)&(lval))))
#define DeeFile_TPutLe(tp_fp,fp,val)\
(([&]{\
 enum{v_size=sizeof(val)};\
 static_assert(v_size == 1 || v_size == 2 || v_size == 4 || v_size == 8,\
               "Cannot only write 1, 2, 4 or 8-byte objects");\
},DeeFile_TPutc(tp_fp,fp,(Dee_uint8_t)(val))))
#define DeeFile_TGetBe DeeFile_TGetLe
#define DeeFile_TPutBe DeeFile_TPutLe
#else
#define DeeFile_TGetLe(tp_fp,fp,lval)\
(sizeof(lval) == 1 ?  DeeFile_TGetc(tp_fp,fp,(Dee_uint8_t *)&(lval)) :\
(sizeof(lval) == 2 ? _DeeFile_TGetLe16(tp_fp,fp,&(lval)) :\
(sizeof(lval) == 4 ? _DeeFile_TGetLe32(tp_fp,fp,&(lval)) :\
                     _DeeFile_TGetLe64(tp_fp,fp,&(lval)))))
#define DeeFile_TGetBe(tp_fp,fp,lval)\
(sizeof(lval) == 1 ?  DeeFile_TGetc(tp_fp,fp,(Dee_uint8_t *)&(lval)) :\
(sizeof(lval) == 2 ? _DeeFile_TGetBe16(tp_fp,fp,&(lval)) :\
(sizeof(lval) == 4 ? _DeeFile_TGetBe32(tp_fp,fp,&(lval)) :\
                     _DeeFile_TGetBe64(tp_fp,fp,&(lval)))))
#define DeeFile_TPutLe(tp_fp,fp,val)\
(sizeof(val) == 1 ?  DeeFile_TPutc(tp_fp,fp,(Dee_uint8_t)(val)) :\
(sizeof(val) == 2 ? _DeeFile_TPutLe16(tp_fp,fp,(Dee_uint16_t)(val)) :\
(sizeof(val) == 4 ? _DeeFile_TPutLe32(tp_fp,fp,(Dee_uint32_t)(val)) :\
                    _DeeFile_TPutLe64(tp_fp,fp,(Dee_uint64_t)(val)))))
#define DeeFile_TPutBe(tp_fp,fp,val)\
(sizeof(val) == 1 ?  DeeFile_TPutc(tp_fp,fp,(Dee_uint8_t)(val)) :\
(sizeof(val) == 2 ? _DeeFile_TPutBe16(tp_fp,fp,(Dee_uint16_t)(val)) :\
(sizeof(val) == 4 ? _DeeFile_TPutBe32(tp_fp,fp,(Dee_uint32_t)(val)) :\
                    _DeeFile_TPutBe64(tp_fp,fp,(Dee_uint64_t)(val)))))
#endif
#define DeeFile_GetLe(fp,lval) DeeFile_TGetLe(Dee_TYPE(fp),fp,lval)
#define DeeFile_GetBe(fp,lval) DeeFile_TGetBe(Dee_TYPE(fp),fp,lval)
#define DeeFile_PutLe(fp,val)  DeeFile_TPutLe(Dee_TYPE(fp),fp,val)
#define DeeFile_PutBe(fp,val)  DeeFile_TPutBe(Dee_TYPE(fp),fp,val)
#endif



#if DEE_PLATFORM_HAVE_IO
enum DeeStdPrinter {
 DEE_STDOUT  = 0,
 DEE_STDERR  = 1,
 DEE_STDIN   = 2,
 DEE_STDNULL = 3,
 DEE_STDDBG  = 4, // OutputDebugString / stderr
 _DEE_STDMAX = 4
};

//////////////////////////////////////////////////////////////////////////
// Return a file object for one of the std printer / reader objects
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeFileObject) *)
 DeeFile_DefaultStd(DEE_A_IN enum DeeStdPrinter printer);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_REF(DeeFileObject) *) DeeFile_Std(DEE_A_IN enum DeeStdPrinter printer);
DEE_FUNC_DECL(DEE_A_EXEC void) DeeFile_SetStd(
 DEE_A_IN enum DeeStdPrinter printer, DEE_A_INOUT_OBJECT(DeeFileObject) *fp) DEE_ATTRIBUTE_NONNULL((2));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeFile_DelStd(DEE_A_IN enum DeeStdPrinter printer);
DEE_FUNC_DECL(DEE_A_EXEC void) DeeFile_ResetStd(void);
// Helper functions/macros for printing to std files
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define /*DEE_A_EXEC*/ DeeFile_StdWriteAll(printer,p,s) DEE_GCC_EXTENSION({int _temp_result;DeeObject*const _temp_fp=DeeFile_Std(printer);_temp_result=DeeFile_WriteAll(_temp_fp,p,s);Dee_DECREF(_temp_fp);_temp_result;})
#define /*DEE_A_EXEC*/ DeeFile_StdPrint(printer,str_)   DEE_GCC_EXTENSION({int _temp_result;DeeObject*const _temp_fp=DeeFile_Std(printer);_temp_result=DeeFile_Print(_temp_fp,str_);Dee_DECREF(_temp_fp);_temp_result;})
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_StdWriteAll(DEE_A_IN enum DeeStdPrinter printer, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) { int result; DeeObject *const fp = DeeFile_Std(printer); result = DeeFile_WriteAll(fp,p,s); Dee_DECREF(fp); return result; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_StdPrint(DEE_A_IN enum DeeStdPrinter printer, DEE_A_IN_Z char const *str_) { int result; DeeObject *const fp = DeeFile_Std(printer); result = DeeFile_Print(fp,str_); Dee_DECREF(fp); return result; }
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define /*DEE_A_EXEC*/ DeeFile_VStdPrintf(printer,fmt,args) DEE_GCC_EXTENSION({int _temp_result;DeeObject*const _temp_fp=DeeFile_Std(printer);_temp_result=DeeFile_VPrintf(_temp_fp,fmt,args);Dee_DECREF(_temp_fp);_temp_result;})
#define /*DEE_A_EXEC*/ DeeFile_StdPrintf(printer,...)       DEE_GCC_EXTENSION({int _temp_result;DeeObject*const _temp_fp=DeeFile_Std(printer);_temp_result=DeeFile_Printf(_temp_fp,__VA_ARGS__);Dee_DECREF(_temp_fp);_temp_result;})
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_VStdPrintf(DEE_A_IN enum DeeStdPrinter printer, DEE_A_IN_PRINTF char const *fmt, DEE_A_IN va_list args) { int result; DeeObject *const fp = DeeFile_Std(printer); result = DeeFile_VPrintf(fp,fmt,args); Dee_DECREF(fp); return result; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFile_StdPrintf(DEE_A_IN enum DeeStdPrinter printer, DEE_A_IN_PRINTF char const *fmt, ...) { int result; va_list args; DeeObject *const fp = DeeFile_Std(printer); DEE_VA_START(args,fmt); result = DeeFile_VPrintf(fp,fmt,args); DEE_VA_END(args); Dee_DECREF(fp); return result; }
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
#define /*DEE_A_EXEC*/ DeeFile_STDPRINT(printer,str_) DeeFile_StdWriteAll(printer,str_,(sizeof(str_)/sizeof(char))-1)
#endif /* DEE_PLATFORM_HAVE_IO */

//////////////////////////////////////////////////////////////////////////
// Returns a joined file object referencing 'fa' and 'fb' with have to be file objects as well
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileJoinedObject) *) DeeFileJoined_New(
 DEE_A_INOUT_OBJECT(DeeFileObject) *fa, DEE_A_INOUT_OBJECT(DeeFileObject) *fb) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// API functions for FileIO
#if DEE_PLATFORM_HAVE_IO
#ifdef DEE_LIMITED_DEX
#define DeeFileIO_FILE(ob)   ((DeeObject *)((DeeFileIOObject *)Dee_REQUIRES_POINTER(ob))->fio_utf8file)
#define DeeFileIO_MODE(ob)   ((DeeFileIOObject *)Dee_REQUIRES_POINTER(ob))->fio_mode
#define DeeFileIO_HANDLE(ob) ((DeeFileIOObject *)Dee_REQUIRES_POINTER(ob))->fio_handle
#endif /* !DEE_LIMITED_DEX */

DEE_FUNC_DECL(DEE_A_RET_OBJECT_REF(DeeAnyStringObject) *) DeeFileIO_File(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT(DeeUtf8StringObject) *) DeeFileIO_Mode(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

#if DEE_PLATFORM_HAVE_IO_HANDLE
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Utf8ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_WideReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_ReOpenObjectEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_Utf8ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_WideReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_ReOpenObjectEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define DeeFileIO_Utf8ReOpen(self,file,mode)    DeeFileIO_Utf8ReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_WideReOpen(self,file,mode)    DeeFileIO_WideReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_ReOpenObject(self,file,mode)  DeeFileIO_ReOpenObjectEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_ReOpen(self,file,mode)        DeeFileIO_ReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define DeeFileIO_ReOpenEx                      DeeFileIO_Utf8ReOpenEx
#define _DeeFileIO_Utf8ReOpen(self,file,mode)   _DeeFileIO_Utf8ReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_WideReOpen(self,file,mode)   _DeeFileIO_WideReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_ReOpenObject(self,file,mode) _DeeFileIO_ReOpenObjectEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_ReOpen(self,file,mode)       _DeeFileIO_ReOpenEx(self,file,mode,DEE_FILEIO_DEFAULT_PERMISSIONS)
#define _DeeFileIO_ReOpenEx                     _DeeFileIO_Utf8ReOpenEx
#endif /* DEE_PLATFORM_HAVE_IO_HANDLE */

//////////////////////////////////////////////////////////////////////////
// Optimized size getter for file.io files
// NOTE: Only faster on some platforms
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Size(
 DEE_A_IN_OBJECT(DeeFileObject) const *self, DEE_A_OUT Dee_uint64_t *size);

struct DeeTupleObject;
struct DeeTimeObject;
//////////////////////////////////////////////////////////////////////////
// Returns a 3-elem tuple equivalent to
// 'pack(DeeFileIO_GetATime(self),DeeFileIO_GetCTime(self),DeeFileIO_GetMTime(self))'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeFileIO_GetTimes(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFileIO_GetATime(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFileIO_GetCTime(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *) DeeFileIO_GetMTime(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeFileIO_SetATime(ob,tm_access)       DeeFileIO_SetTimes2(ob,tm_access,NULL,NULL)
#define DeeFileIO_SetCTime(ob,tm_creation)     DeeFileIO_SetTimes2(ob,NULL,tm_creation,NULL)
#define DeeFileIO_SetMTime(ob,tm_modification) DeeFileIO_SetTimes2(ob,NULL,NULL,tm_modification)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_GetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification)
 DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_SetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_modification)
 DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_PLATFORM_HAVE_IO */


//////////////////////////////////////////////////////////////////////////
// API functions for FileStringWriter
#ifdef DEE_LIMITED_DEX
#define DeeFileWriter_POS(ob)    ((DeeFileWriterObject *)Dee_REQUIRES_POINTER(ob))->fw_pos
#define DeeFileWriter_WRITER(ob) (&((DeeFileWriterObject *)Dee_REQUIRES_POINTER(ob))->fw_writer)
#define DeeFileWriter_PACK(ob)   DeeStringWriter_Pack(DeeFileWriter_WRITER(ob))
#else /* DEE_LIMITED_DEX */
#define DeeFileWriter_PACK(ob)   DeeFileWriter_Pack((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif /* !DEE_LIMITED_DEX */

//////////////////////////////////////////////////////////////////////////
#define DeeFileWriter_Pack(ob) DeeFileWriter_PackEx(ob,&DeeString_Type)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeFileWriter_PackEx(
 DEE_A_INOUT_OBJECT(DeeFileWriterObject) *self, DEE_A_IN DeeTypeObject const *string_type)
 DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// API functions for FileStringReader
#ifdef DEE_LIMITED_DEX
#define DeeFileReader_POS(ob)    ((DeeFileReaderObject *)Dee_REQUIRES_POINTER(ob))->fr_pos
#define DeeFileReader_END(ob)    ((DeeFileReaderObject *)Dee_REQUIRES_POINTER(ob))->fr_end
#define DeeFileReader_STRING(ob) ((DeeObject *)((DeeFileReaderObject *)Dee_REQUIRES_POINTER(ob))->fr_string)
#else /* !DEE_LIMITED_DEX */
#define DeeFileReader_STRING(ob) DeeFileReader_String((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif /* !DEE_LIMITED_DEX */
DEE_FUNC_DECL(DEE_A_RET_OBJECT_REF(DeeAnyStringObject) *) DeeFileReader_String(
 DEE_A_IN_OBJECT(DeeFileReaderObject) const *self) DEE_ATTRIBUTE_NONNULL((1));


#if DEE_PLATFORM_HAVE_PIPES
#ifdef GUARD_DEEMON_PIPE_H
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeIOHandle)
DeePipe_Handle(DEE_A_IN_OBJECT(DeePipeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
#endif

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC void _deefile_invalid_seek_mode(DEE_A_IN int mode);
#endif

DEE_DECL_END

#ifdef __cplusplus
#if DEE_PLATFORM_HAVE_IO
#undef DeeFileIO_NewEx
#undef DeeFileIO_ReOpenEx
#undef _DeeFileIO_NewEx
#undef _DeeFileIO_ReOpenEx
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) DeeFileIO_NewEx(DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return DeeFileIO_Utf8NewEx(file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) DeeFileIO_NewEx(DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return DeeFileIO_WideNewEx(file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) _DeeFileIO_NewEx(DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return _DeeFileIO_Utf8NewEx(file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *) _DeeFileIO_NewEx(DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return _DeeFileIO_WideNewEx(file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return DeeFileIO_Utf8ReOpenEx(self,file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFileIO_ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return DeeFileIO_WideReOpenEx(self,file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return _DeeFileIO_Utf8ReOpenEx(self,file,mode,permissions); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_ReOpenEx(DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) { return _DeeFileIO_WideReOpenEx(self,file,mode,permissions); }
#endif /* DEE_PLATFORM_HAVE_IO */
#endif /* __cplusplus */

#endif /* !GUARD_DEEMON_FILE_H */
