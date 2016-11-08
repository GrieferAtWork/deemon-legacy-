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
#ifndef GUARD_DEEMON_CXX_FILE_H
#define GUARD_DEEMON_CXX_FILE_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/file.h>
#include <deemon/error.h>
#include <deemon/marshal.h>
#include <deemon/fs_api.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/type.h>
#include <deemon/cxx/string.h>
#include <deemon/cxx/tuple.h>
#include <deemon/cxx/time.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct file: object {
 public: /* public types */
  struct io;
  struct reader;
  struct writer;
  struct joined;
 public: /* public constants */
  DEE_STATIC_CONST(int,SEEK_SET_ = DEE_SEEK_SET);
  DEE_STATIC_CONST(int,SEEK_CUR_ = DEE_SEEK_CUR);
  DEE_STATIC_CONST(int,SEEK_END_ = DEE_SEEK_END);
 public: /* public operators */
  inline file &operator << (DEE_A_INOUT DeeObject *right) {
   int temp;
   if ((right = ::DeeObject_Str(right)) == NULL) detail::throw_last_error();
   temp = DeeFile_WriteAll(this->ob_ptr,DeeString_STR(right),DeeString_SIZE(right)*sizeof(char));
   Dee_DECREF(right);
   if (temp != 0) detail::throw_last_error();
   return *this;
  }
  inline file const &operator << (DEE_A_INOUT DeeObject *right) const {
   int temp;
   if ((right = ::DeeObject_Str(right)) == NULL) detail::throw_last_error();
   temp = DeeFile_WriteAll(this->ob_ptr,DeeString_STR(right),DeeString_SIZE(right)*sizeof(char));
   Dee_DECREF(right);
   if (temp != 0) detail::throw_last_error();
   return *this;
  }
  inline file &operator << (DEE_A_IN object const &right) { return *this << right.priv_get(); }
  inline file const &operator << (DEE_A_IN object const &right) const { return *this << right.priv_get(); }
  inline DEE_A_RET_WUNUSED file operator | (DEE_A_IN file const &right) const { return file(::DeeFileJoined_New(this->ob_ptr,right),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED string read() const { return string(::DeeFile_ReadData(this->ob_ptr,(Dee_size_t)-1),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED string read(DEE_A_IN Dee_size_t max_bytes) const { return string(::DeeFile_ReadData(this->ob_ptr,max_bytes),detail::tag_ref_or_err()); }
  inline void read(DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) const { if (::DeeFile_ReadAll(this->ob_ptr,p,s) != 0) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED string readall() const { return string(::DeeFile_ReadAllData(this->ob_ptr,(Dee_size_t)-1),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED string readall(DEE_A_IN Dee_size_t max_bytes) const { return string(::DeeFile_ReadAllData(this->ob_ptr,max_bytes),detail::tag_ref_or_err()); }
  inline void readall(DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) const { if (::DeeFile_ReadAll(this->ob_ptr,p,s) != 0) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED string maybe_read() const { return string(::DeeFile_ReadData(this->ob_ptr,(Dee_size_t)-1),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED string maybe_read(DEE_A_IN Dee_size_t max_bytes) const { return string(::DeeFile_ReadData(this->ob_ptr,max_bytes),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED Dee_size_t maybe_read(DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) const { Dee_size_t result; if (::DeeFile_Read(this->ob_ptr,p,s,&result) != 0) detail::throw_last_error(); return result; }
  inline void write(DEE_A_IN string const &ob) const { if (::DeeFile_WriteAll(this->ob_ptr,ob.c_str(),ob.size()) != 0) detail::throw_last_error(); }
  inline void write(DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) const { if (::DeeFile_WriteAll(this->ob_ptr,p,s) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline Dee_uint64_t seek(DEE_A_IN Dee_uint64_t off) const { Dee_uint64_t result; if (::DeeFile_Seek(this->ob_ptr,(Dee_int64_t)off,DEE_SEEK_SET,&result) != 0) detail::throw_last_error(); return result; }
  inline Dee_uint64_t seek(DEE_A_IN Dee_int64_t off, DEE_A_IN int whence) const { Dee_uint64_t result; if (::DeeFile_Seek(this->ob_ptr,off,whence,&result) != 0) detail::throw_last_error(); return result; }
  inline void setpos(DEE_A_IN Dee_uint64_t pos) const { if (::DeeFile_SetPos(this->ob_ptr,pos) != 0) detail::throw_last_error(); }
  inline void rewind() const { if (::DeeFile_Rewind(this->ob_ptr) != 0) detail::throw_last_error(); }
  inline void trunc() const { if (::DeeFile_Trunc(this->ob_ptr) != 0) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED Dee_uint64_t tell() const { Dee_uint64_t result; if (::DeeFile_Tell(this->ob_ptr,&result) != 0) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_uint64_t size() const { Dee_uint64_t result; if (::DeeFile_Size(this->ob_ptr,&result) != 0) detail::throw_last_error(); return result; }
  inline void flush() const { if (::DeeFile_Flush(this->ob_ptr) != 0) detail::throw_last_error(); }
  inline void close() const DEE_CXX11_NOEXCEPT { DeeFile_Close(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED Dee_uint8_t getc() const { Dee_uint8_t result; if (DeeFile_Getc(this->ob_ptr,&result) != 0) detail::throw_last_error(); return result; }
  inline void putc(DEE_A_IN Dee_uint8_t c) const { if (DeeFile_Putc(this->ob_ptr,c) != 0) detail::throw_last_error(); }
  inline void puts(DEE_A_IN_Z char const *s) const { if (DeeFile_Puts(this->ob_ptr,s) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object reado(DEE_A_IN deemon::type const &tp) const { return object(::DeeFile_ReadObject(this->ob_ptr,tp),detail::tag_ref_or_err()); }
  inline void writeo(DEE_A_IN ::DeeObject const *ob) const { if (::DeeFile_WriteObject(this->ob_ptr,ob)) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object readm() const { return object(::DeeMarshal_ReadObject(this->ob_ptr,DEE_MARSHAL_VERSION),detail::tag_ref_or_err()); }
  inline void writem(DEE_A_IN ::DeeObject *ob) const { if (::DeeMarshal_WriteObject(this->ob_ptr,ob) != 0) detail::throw_last_error(); }
  inline void writem(DEE_A_IN ::DeeObject *ob, DEE_A_IN Dee_uint32_t flags) const { if (::DeeMarshal_WriteObjectEx(this->ob_ptr,ob,flags) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED string readline() const { return string(::DeeFile_ReadLine(this->ob_ptr,(Dee_size_t)-1),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED string readline(DEE_A_IN Dee_size_t max_size) const { return string(::DeeFile_ReadLine(this->ob_ptr,max_size),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline void print(DEE_A_IN_Z char const *s) const { if (DeeFile_Print(this->ob_ptr,s) != 0) detail::throw_last_error(); }
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
  inline void printf(DEE_A_IN_PRINTF char const *fmt, ...) const { va_list args; int error; DEE_VA_START(args,fmt); error = DeeFile_VPrintf(this->ob_ptr,fmt,args); DEE_VA_END(args); if (error != 0) detail::throw_last_error(); }
  inline void vprintf(DEE_A_IN_PRINTF char const *fmt, DEE_A_IN va_list args) const { if (DeeFile_VPrintf(this->ob_ptr,fmt,args) != 0) detail::throw_last_error(); }
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
 public: /* public methods */
  static inline DEE_A_RET_WUNUSED file std(DEE_A_IN enum ::DeeStdPrinter printer) { return file(DeeFile_Std(printer),detail::tag_inherit_ref()); }
  static inline DEE_A_RET_WUNUSED file default_std(DEE_A_IN enum ::DeeStdPrinter printer) { return file(DeeFile_DefaultStd(printer)); }
 public: /* public constructor / destructor */
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(file,object,DeeFile_Check)
};


struct file::io: ::deemon::file {
 public: /* public types */
  struct tag_noexpand {};
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object file() const { return object(DeeFileIO_File(this->ob_ptr),detail::tag_inherit_ref()); }
  inline DEE_A_RET_WUNUSED string mode() const { return string(DeeFileIO_Mode(this->ob_ptr)); }
 public: /* public methods */
  inline void reopen(DEE_A_IN ::DeeObject const *filename) const { if (::DeeFileIO_ReOpenObject(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void reopen(DEE_A_IN ::DeeObject const *filename, DEE_A_IN_Z char const *mode) const { if (::DeeFileIO_ReOpenObject(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
  inline void reopen(DEE_A_IN_Z ::Dee_Utf8Char const *filename) const { if (::DeeFileIO_Utf8ReOpen(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void reopen(DEE_A_IN_Z ::Dee_WideChar const *filename) const { if (::DeeFileIO_WideReOpen(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void reopen(DEE_A_IN_Z ::Dee_Utf8Char const *filename, DEE_A_IN_Z char const *mode) const { if (::DeeFileIO_Utf8ReOpen(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
  inline void reopen(DEE_A_IN_Z ::Dee_WideChar const *filename, DEE_A_IN_Z char const *mode) const { if (::DeeFileIO_WideReOpen(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN ::DeeObject const *filename) const { if (::_DeeFileIO_ReOpenObject(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN ::DeeObject const *filename, DEE_A_IN_Z char const *mode) const { if (::_DeeFileIO_ReOpenObject(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN_Z ::Dee_Utf8Char const *filename) const { if (::_DeeFileIO_Utf8ReOpen(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN_Z ::Dee_WideChar const *filename) const { if (::_DeeFileIO_WideReOpen(this->ob_ptr,filename,"r") != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN_Z ::Dee_Utf8Char const *filename, DEE_A_IN_Z char const *mode) const { if (::_DeeFileIO_Utf8ReOpen(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
  inline void _reopen(DEE_A_IN_Z ::Dee_WideChar const *filename, DEE_A_IN_Z char const *mode) const { if (::_DeeFileIO_WideReOpen(this->ob_ptr,filename,mode) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED tuple gettimes() const { return tuple(::DeeFileIO_GetTimes(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED time getatime() const { return time(::DeeFileIO_GetATime(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED time getctime() const { return time(::DeeFileIO_GetCTime(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED time getmtime() const { return time(::DeeFileIO_GetMTime(this->ob_ptr),detail::tag_ref_or_err()); }
  inline void settimes(DEE_A_IN time const &tm_access) const { if (::DeeFileIO_SetTimes2(this->ob_ptr,tm_access,NULL,NULL) != 0) detail::throw_last_error(); }
  inline void settimes(DEE_A_IN time const &tm_access, DEE_A_IN time const &tm_creation) const { if (::DeeFileIO_SetTimes2(this->ob_ptr,tm_access,tm_creation,NULL) != 0) detail::throw_last_error(); }
  inline void settimes(DEE_A_IN time const &tm_access, DEE_A_IN time const &tm_creation, DEE_A_IN time const &tm_modification) const { if (::DeeFileIO_SetTimes2(this->ob_ptr,tm_access,tm_creation,tm_modification) != 0) detail::throw_last_error(); }
  inline void setatime(DEE_A_IN time const &tm_access) const { if (::DeeFileIO_SetATime(this->ob_ptr,tm_access) != 0) detail::throw_last_error(); }
  inline void setctime(DEE_A_IN time const &tm_creation) const { if (::DeeFileIO_SetCTime(this->ob_ptr,tm_creation) != 0) detail::throw_last_error(); }
  inline void setmtime(DEE_A_IN time const &tm_modification) const { if (::DeeFileIO_SetMTime(this->ob_ptr,tm_modification) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED bool isfile() const { int result; if ((result = ::DeeFileIO_IsFile(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool isdir() const { int result; if ((result = ::DeeFileIO_IsDir(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool islink() const { int result; if ((result = ::DeeFileIO_IsLink(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool isdrive() const { int result; if ((result = ::DeeFileIO_IsDrive(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool ismount() const { int result; if ((result = ::DeeFileIO_IsMount(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool ishidden() const { int result; if ((result = ::DeeFileIO_IsHidden(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool isxfile() const { int result; if ((result = ::DeeFileIO_IsExecutable(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED object dir() const { return object(::DeeFileIO_ListDir(this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_uint64_t size() const { Dee_uint64_t result; if (::DeeFileIO_Size(this->ob_ptr,&result) != 0) detail::throw_last_error(); return result; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED file::io tmp() { return file::io(DeeFileIO_NewTemporary(1),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED file::io tmp(DEE_A_IN bool delete_when_closed) { return file::io(DeeFileIO_NewTemporary((int)delete_when_closed),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  inline io(): ::deemon::file(::DeeType_NewInstanceDefault((::DeeTypeObject *)&::DeeFileIO_Type),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_Utf8Char const *filename): ::deemon::file(::DeeFileIO_Utf8New(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_WideChar const *filename): ::deemon::file(::DeeFileIO_WideNew(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_Utf8Char const *filename, DEE_A_IN_Z char const *mode): ::deemon::file(::DeeFileIO_Utf8New(filename,mode),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_WideChar const *filename, DEE_A_IN_Z char const *mode): ::deemon::file(::DeeFileIO_WideNew(filename,mode),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN ::DeeObject const *filename): ::deemon::file(::DeeFileIO_NewObject(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN ::DeeObject const *filename, DEE_A_IN_Z char const *mode): ::deemon::file(::DeeFileIO_NewObject(filename,mode),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_Utf8Char const *filename, tag_noexpand): ::deemon::file(::_DeeFileIO_Utf8New(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_WideChar const *filename, tag_noexpand): ::deemon::file(::_DeeFileIO_WideNew(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_Utf8Char const *filename, DEE_A_IN_Z char const *mode, tag_noexpand): ::deemon::file(::_DeeFileIO_Utf8New(filename,mode),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN_Z ::Dee_WideChar const *filename, DEE_A_IN_Z char const *mode, tag_noexpand): ::deemon::file(::_DeeFileIO_WideNew(filename,mode),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN ::DeeObject const *filename, tag_noexpand): ::deemon::file(::_DeeFileIO_NewObject(filename,"r"),detail::tag_ref_or_err()) {}
  inline explicit io(DEE_A_IN ::DeeObject const *filename, DEE_A_IN_Z char const *mode, tag_noexpand): ::deemon::file(::_DeeFileIO_NewObject(filename,mode),detail::tag_ref_or_err()) {}
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(io,deemon::file,DeeFileIO_Check)
};


struct file::reader: file {
 public: /* public constructor / destructor */
  inline explicit reader(DEE_A_IN string const &ob): file(::DeeFileReader_New(ob),detail::tag_ref_or_err()) {}
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(reader,deemon::file,DeeFileReader_Check)
};
struct file::writer: file {
 public: /* public methods */
  inline string pack() const { return string(::DeeFileWriter_Pack(this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  inline explicit writer(): file(::DeeFileWriter_New(),detail::tag_ref_or_err()) {}
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(writer,deemon::file,DeeFileWriter_Check)
};
struct file::joined: file {
 public: /* public constructor / destructor */
  inline explicit joined(DEE_A_IN file const &fa, DEE_A_IN file const &fb):
   file(::DeeFileJoined_New(fa,fb),detail::tag_ref_or_err()) {}
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(joined,deemon::file,DeeFileJoined_Check)
};


namespace detail {
struct deemon_printer {
 private: DeeObject *ob_outfile; public:
 template<class T> DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,deemon_printer&) operator , (T const &ob) {
  if (DeeFile_PRINT(this->ob_outfile," ") != 0) detail::throw_last_error();
  return *this << ob;
 }
 template<Dee_size_t N> deemon_printer &operator << (char const (&ob)[N]) {
  if (DeeFile_PRINT(this->ob_outfile,ob) != 0) detail::throw_last_error();
  return *this;
 }
 template<class T> DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,deemon_printer&) operator << (T const &ob) {
  DeeObject *val; int error;
  if ((val = new_(ob)) == NULL) detail::throw_last_error();
  error = DeeFile_PrintOne(this->ob_outfile,val);
  Dee_DECREF(val);
  if (error != 0) detail::throw_last_error();
  return *this;
 }
 deemon_printer(DEE_A_IN DeeObject *outfile) DEE_CXX11_NOEXCEPT: ob_outfile(outfile) { Dee_INCREF(outfile); }
 deemon_printer(DEE_A_IN DeeObject *outfile, detail::tag_inherit_ref) DEE_CXX11_NOEXCEPT: ob_outfile(outfile) {}
 ~deemon_printer() DEE_CXX11_NOEXCEPT {
  if (DeeFile_PRINT(this->ob_outfile,"\n") != 0) DeeError_Handled();
  Dee_DECREF(this->ob_outfile);
 }
};
}

//////////////////////////////////////////////////////////////////////////
// Can be used just like the 'print' statement in deemon usercode
// -> Use 'deemon_print_f' for output into an explicit file
#define deemon_print \
 ::deemon::detail::deemon_printer(::DeeFile_Std(DEE_STDOUT),::deemon::detail::tag_inherit_ref())<<
#define deemon_print_f(file) \
 ::deemon::detail::deemon_printer(file)<<


}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_FILE_H */
