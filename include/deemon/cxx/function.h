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
#ifndef GUARD_DEEMON_CXX_FUNCTION_H
#define GUARD_DEEMON_CXX_FUNCTION_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/compiler/code.h>
#include <deemon/runtime/function.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/file.h>
#include <deemon/cxx/tuple.h>
#include <deemon/cxx/string.h>
#include <deemon/deemonrun.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct code: object {
 public: /* public methods (asmrepr) */
  inline DEE_A_RET_WUNUSED string asmrepr() const { return string(::DeeCode_AsmRepr(this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(code,object,DeeCode_Check)
};

struct function: object {
 public: /* public methods (call) */
  inline DEE_A_RET_WUNUSED object __call__(DEE_A_IN tuple const &args) const { return object(DeeFunction_Call(this->ob_ptr,args),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object __thiscall__(DEE_A_INOUT ::DeeObject *this_arg, DEE_A_IN tuple const &args) const { return object(::DeeFunction_ThisCall(this->ob_ptr,this_arg,args),detail::tag_ref_or_err()); }
 public: /* public methods (asmrepr) */
  inline DEE_A_RET_WUNUSED string asmrepr() const { return string(::DeeCode_AsmRepr(DeeFunction_Code(this->ob_ptr)),detail::tag_ref_or_err()); }
 public: /* public methods (compile) */
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN string const &code) { return function(::DeeCompile_StringObject(code,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN_Z char const *code) { return function(::DeeCompile_String(code,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length) { return function(::DeeCompile_StringWithLength(code,code_length,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN file const &filestream) { return function(::DeeCompile_FileStream(filestream,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN string const &code, DEE_A_IN_Z char const *disp_filename) { return function(::DeeCompile_StringObjectEx(code,disp_filename,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename) { return function(::DeeCompile_StringEx(code,disp_filename,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename) { return function(::DeeCompile_StringWithLengthEx(code,code_length,disp_filename,NULL),detail::tag_ref_or_err()); }
  static inline DEE_A_RET_WUNUSED function compile(DEE_A_IN file const &filestream, DEE_A_IN_Z char const *disp_filename) { return function(::DeeCompile_FileStreamEx(filestream,disp_filename,NULL),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  inline explicit function(): object(::DeeFunction_New(Dee_EmptyCode,Dee_EmptyTuple,0,DEE_FUNCTION_FLAG_NONE),detail::tag_ref_or_err()) {}
  inline explicit function(DEE_A_IN code const &code_): object(::DeeFunction_New(code_,Dee_EmptyTuple,0,DEE_FUNCTION_FLAG_NONE),detail::tag_ref_or_err()) {}
  inline explicit function(DEE_A_IN code const &code_, DEE_A_IN tuple const &refs): object(::DeeFunction_New(code_,refs,0,DEE_FUNCTION_FLAG_NONE),detail::tag_ref_or_err()) {}
  inline explicit function(DEE_A_IN code const &code_, DEE_A_IN tuple const &refs, DEE_A_IN Dee_size_t argc): object(::DeeFunction_New(code_,refs,argc,DEE_FUNCTION_FLAG_NONE),detail::tag_ref_or_err()) {}
  inline explicit function(DEE_A_IN code const &code_, DEE_A_IN tuple const &refs, DEE_A_IN Dee_size_t argc, DEE_A_IN Dee_uint32_t flags): object(::DeeFunction_New(code_,refs,argc,flags),detail::tag_ref_or_err()) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(function,object,DeeFunction_Check)
};

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_FUNCTION_H */
