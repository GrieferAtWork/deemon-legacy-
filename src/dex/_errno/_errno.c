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
#ifndef GUARD_DEEMON_DEX_ERRNO_C
#define GUARD_DEEMON_DEX_ERRNO_C 1
#define DEE_EXTENSION 1

#include "_errno.h"
#include <deemon/dex.h>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/tuple.h>
//#include DEE_INCLUDE_MEMORY_API()
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/unicode/char_traits.inl>
#endif

DEE_DECL_BEGIN

DEE_A_RET_Z_OPT char const *
_DeeErrno_ErrorName(DEE_A_IN Dee_errno_t err) {
 switch (err) {
#ifndef __INTELLISENSE__
#define LINUX_ERROR(name) case name: return #name;
#include "linux_errors.inl"
#undef LINUX_ERROR
#endif
  default: break;
 }
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeErrno_ErrnoName(DEE_A_IN Dee_errno_t err) {
 char const *s;
 if ((s = _DeeErrno_ErrorName(err)) == NULL) DeeReturn_EmptyString;
 return DeeString_New(s);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeErrno_ErrnoText(DEE_A_IN Dee_errno_t err) {
 char const *s;
 if ((s = strerror(err)) == NULL) DeeReturn_EmptyString;
 return DeeString_New(s);
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_Z_OPT char const *_DeeErrno_Win32ErrorName(DEE_A_IN DWORD err) {
 switch (err) {
#ifndef __INTELLISENSE__
#define WIN32_ERROR(name,id) case id: return #name;
#include "win32_errors.inl"
#undef WIN32_ERROR
#endif
  default: break;
 }
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeErrno_Win32ErrnoName(DEE_A_IN DWORD err) {
 char const *s;
 if ((s = _DeeErrno_Win32ErrorName(err)) == NULL) DeeReturn_EmptyString;
 return DeeString_New(s);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeErrno_Win32ErrnoText(DEE_A_IN DWORD err) {
 DeeObject *result; DWORD result_length; DWORD error;
 result_length = 64;
 if ((result = DeeString_NewSized(64)) == NULL) return NULL;
 while (1) {
  error = FormatMessageA(
   FORMAT_MESSAGE_FROM_SYSTEM,NULL,err,
   MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
   DeeString_STR(result),result_length,(va_list *)NULL);
  if (error != 0) break;
  // MSDN says that this string cannot exceed 32*1024
  // >> So if it does, treat it as a failure of translating the message...
  if (result_length > 32*1024) {
   Dee_DECREF(result);
   DeeReturn_EmptyString;
  }
  if (DeeString_Resize(&result,(result_length *= 2)) != 0) {
err_r: Dee_DECREF(result); return NULL;
  }
 }
  // Trim any ending linefeeds
 while (error && DEE_CH_IS_SPACE(DeeString_STR(result)[error-1])) --error;
 if (DeeString_Resize(&result,(Dee_size_t)error) != 0) goto err_r;
 return result;
}
#endif /* DEE_PLATFORM_WINDOWS */


static DeeObject *_deeerrno_errnoname(DeeObject *args) {
 int id;
 if (DeeTuple_Unpack(args,"d:errnoname",&id) != 0) return NULL;
 return DeeErrno_ErrnoName(id);
}
static DeeObject *_deeerrno_errnotext(DeeObject *args) {
 int id;
 if (DeeTuple_Unpack(args,"d:errnotext",&id) != 0) return NULL;
 return DeeErrno_ErrnoText(id);
}

#ifdef DEE_PLATFORM_WINDOWS
static DeeObject *_deeerrno_win32_errnoname(DeeObject *args) {
 DWORD id;
 if (DeeTuple_Unpack(args,"lu:win32_errnoname",&id) != 0) return NULL;
 return DeeErrno_Win32ErrnoName(id);
}
static DeeObject *_deeerrno_win32_errnotext(DeeObject *args) {
 DWORD id;
 if (DeeTuple_Unpack(args,"lu:win32_errnotext",&id) != 0) return NULL;
 return DeeErrno_Win32ErrnoText(id);
}
#endif /* DEE_PLATFORM_WINDOWS */


//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_FUNCTION("errnoname",&_deeerrno_errnoname,"(int id) -> string\n"
  "@return: The name of a given errno-style error @id"),
 DeeDex_EXPORT_FUNCTION("errnotext",&_deeerrno_errnotext,"(int id) -> string\n"
  "@return: A human-readable description for a given errno-style error @id"),
#ifdef DEE_PLATFORM_WINDOWS
 DeeDex_EXPORT_FUNCTION("win32_errnoname",&_deeerrno_win32_errnoname,"(DWORD id) -> string\n"
  "@return: The name of a given GetLastError-style error @id"),
 DeeDex_EXPORT_FUNCTION("win32_errnotext",&_deeerrno_win32_errnotext,"(DWORD id) -> string\n"
  "@return: A human-readable description for a given GetLastError-style error @id"),
#endif /* DEE_PLATFORM_WINDOWS */
/*
#ifndef __INTELLISENSE__
#define LINUX_ERROR(name) DeeDex_EXPORT_CONST(#name,int,name),
#include "linux_errors.inl"
#undef LINUX_ERROR
#ifdef DEE_PLATFORM_WINDOWS
#define WIN32_ERROR(name,id) DeeDex_EXPORT_CONST(#name,DWORD,id),
#include "win32_errors.inl"
#undef WIN32_ERROR
#endif
#endif
*/
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ERRNO_C */
