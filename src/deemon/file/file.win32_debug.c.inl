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
#ifndef GUARD_DEEMON_FILE_WIN32_DEBUG_C_INL
#define GUARD_DEEMON_FILE_WIN32_DEBUG_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/file.h>
#include <deemon/generic_vtable.h>
#include "file.win32_debug.h.inl"

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

static DeeObject *DEE_CALL _deefilewin32debug_tp_str(DeeFileWin32DbgObject *DEE_UNUSED(self)) {
 DeeReturn_STATIC_STRING("<file.io.stddbg: mode \"w\", pos 0/0>");
}
static int DEE_CALL _deefilewin32debug_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWin32DbgObject *self) {
 _DeeFile_Init(self);
 return 0;
}
static int DEE_CALL _deefilewin32debug_tp_io_read(
 DeeFileWin32DbgObject *DEE_UNUSED(self), void *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(rs)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"Can't read file.io.stddbg");
 return -1;
}
static int DEE_CALL _deefilewin32debug_tp_io_write(
#ifndef DEE_WITHOUT_THREADS
 DeeFileWin32DbgObject *self,
#else
 DeeFileWin32DbgObject *DEE_UNUSED(self),
#endif
 void const *p, Dee_size_t s, Dee_size_t *ws) {
 enum{buffer_size=256};
 char buffer[buffer_size+1];
 *ws = s;
 DeeFile_ACQUIRE(self);
 while DEE_LIKELY(s >= buffer_size) {
  memcpy(buffer,p,buffer_size);
  buffer[buffer_size] = '\0';
  OutputDebugStringA(buffer);
  *(Dee_uintptr_t *)&p += buffer_size;
  s -= buffer_size;
 }
 if DEE_LIKELY(s) {
  memcpy(buffer,p,s);
  buffer[s] = '\0';
  OutputDebugStringA(buffer);
 }
 DeeFile_RELEASE(self);
 return 0;
}
static int DEE_CALL _deefilewin32debug_tp_io_seek(
 DeeFileWin32DbgObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *pos) {
 if (pos) *pos = 0;
 return 0;
}
static int DEE_CALL _deefilewin32debug_tp_io_flush(DeeFileWin32DbgObject *DEE_UNUSED(self)) { return 0; }
static int DEE_CALL _deefilewin32debug_tp_io_trunc(DeeFileWin32DbgObject *DEE_UNUSED(self)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"Can't truncate file.io.stddbg");
 return -1;
}

DeeFileTypeObject DeeFileWin32Dbg_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.win32_debug"),null,
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileWin32DbgObject),null,
   member(&_deefilewin32debug_tp_ctor),null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilewin32debug_tp_str),
   member(&_deefilewin32debug_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,
   member(&_deegenericsameob_tp_hash)),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&_deegenericsameob_tp_cmp_lo),
   member(&_deegenericsameob_tp_cmp_le),
   member(&_deegenericsameob_tp_cmp_eq),
   member(&_deegenericsameob_tp_cmp_ne),
   member(&_deegenericsameob_tp_cmp_gr),
   member(&_deegenericsameob_tp_cmp_ge)),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilewin32debug_tp_io_read),
  member(&_deefilewin32debug_tp_io_write),
  member(&_deefilewin32debug_tp_io_seek),
  member(&_deefilewin32debug_tp_io_flush),
  member(&_deefilewin32debug_tp_io_trunc),null),
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* DEE_PLATFORM_WINDOWS */

#endif /* !GUARD_DEEMON_FILE_WIN32_DEBUG_C_INL */
