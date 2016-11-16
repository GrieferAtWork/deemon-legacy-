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
#ifndef GUARD_DEEMON_SYS_SYSFD_H
#define GUARD_DEEMON_SYS_SYSFD_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
//
// >> #define DEE_SYSFD_SEEK_SET <implementation_dependent>
// >> #define DEE_SYSFD_SEEK_CUR <implementation_dependent>
// >> #define DEE_SYSFD_SEEK_END <implementation_dependent>
//
// >> struct DeeSysFD { ... };
// >> void DeeSysFD_Quit(DEE_A_INOUT struct DeeSysFD *self);
// >> bool DeeSysFD_TryRead(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs);
// >> bool DeeSysFD_TryWrite(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws);
// >> void DeeSysFD_Read(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> void DeeSysFD_Write(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);
//         - Read/Write data, or return false/throw an Error.IOError
// >> bool DeeSysFD_TrySeek(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff);
// >> void DeeSysFD_Seek(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff, CODE on_error);
//         - Moves the fd's r/w pointer to a given position, or return false/throw an Error.IOError / Error.RuntimeError.NotImplemented
// >> [[optional]] bool DeeSysFD_TryFlush(DEE_A_INOUT struct DeeSysFD *self);
// >> [[optional]] void DeeSysFD_Flush(DEE_A_INOUT struct DeeSysFD *self, CODE on_error);
//         - Flushes unwritten data, or return false/throw an Error.IOError
// >> [[optional]] bool DeeSysFD_TryTrunc(DEE_A_INOUT struct DeeSysFD *self);
// >> [[optional]] void DeeSysFD_Trunc(DEE_A_INOUT struct DeeSysFD *self, CODE on_error);
//         - Truncate 'this' fd to its current r/w pointer, or return false/throw an Error.IOError
//
// >> struct DeeSysStdFD: DeeSysFD { ... };
// >> [[optional]] #define DeeSysStdFD_INIT_STDIN()   {...}
// >> [[optional]] #define DeeSysStdFD_INIT_STDOUT()  {...}
// >> [[optional]] #define DeeSysStdFD_INIT_STDERR()  {...}
//         - Static initializer for a 'DeeSysStdFD' structure, that initialize
//           to that structure with the same values as the getter functions below.
//           NOTE: These initializer macros are optional and may be
//                 omitted if not implementable on the hosting platform.
// >> void DeeSysStdFD_GET_STDIN(DEE_A_OUT struct DeeSysFD *self);
// >> void DeeSysStdFD_GET_STDOUT(DEE_A_OUT struct DeeSysFD *self);
// >> void DeeSysStdFD_GET_STDERR(DEE_A_OUT struct DeeSysFD *self);
//         - Fills '*self' with information about the stdin/stdout/stderr file descriptors.
//           NOTE: The generated 'self' object should not be destroyed
//                 with 'DeeSysFD_Quit', but may be closed.
//
// >> struct DeeSysFileFD: DeeSysFD { ... };
// >> [[optional]] void DeeSysFileFD_Quit(DEE_A_IN struct DeeSysFileFD *self);
//         - Destruction code that must be execute in addition of 'DeeSysFD_Quit'
// >> bool DeeSysFileFD_Utf8TryInit(struct DeeSysFileFD *self, Dee_Utf8Char const *filename, Dee_uint16_t mode, Dee_mode_t perms);
// >> bool DeeSysFileFD_WideTryInit(struct DeeSysFileFD *self, Dee_WideChar const *filename, Dee_uint16_t mode, Dee_mode_t perms);
// >> bool DeeSysFileFD_Utf8TryInitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms);
// >> bool DeeSysFileFD_WideTryInitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms);
// >> void DeeSysFileFD_Utf8Init(struct DeeSysFileFD *self, Dee_Utf8Char const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> void DeeSysFileFD_WideInit(struct DeeSysFileFD *self, Dee_WideChar const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> void DeeSysFileFD_Utf8InitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> void DeeSysFileFD_WideInitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
//         - Opens a given file-descriptor, or execute 'on_error' after throwing an Error.IOError
//         - NOTE: These function can be used to open a directory
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeSysFileFD_Filename(DEE_A_IN struct DeeSysFileFD const *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFileFD_Utf8Filename(DEE_A_IN struct DeeSysFileFD const *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFileFD_WideFilename(DEE_A_IN struct DeeSysFileFD const *self);
//         - Returns the filename associated with a given fs-file-descriptor
// >> [[optional]] bool DeeSysFileFD_TryRead(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs);
// >> [[optional]] bool DeeSysFileFD_TryWrite(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws);
// >> [[optional]] void DeeSysFileFD_Read(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Write(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);
// >> [[optional]] bool DeeSysFileFD_TrySeek(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff);
// >> [[optional]] void DeeSysFileFD_Seek(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff, CODE on_error);
//
//[[optional
// >> struct DeeSysPipeFD: DeeSysFD { ... };
//         - Pipes are generally an optional platform-dependent feature,
//           not being required per-sé, but being a simple way of doing for IPC.
//           They are optional for the same reason why processes themselves are optional
//           as well: To keep the deemon runtime compatible with embedded systems.
//         - NOTE: Even when pipes are available, the platform may not require overwriting
//                 of the r/w operators, meaning those are optional even then (check for existence with #ifdef).
// >> [[optional]] void DeeSysPipeFD_Quit(DEE_A_IN struct DeeSysPipeFD *self);
//         - Destruction code that must be execute in addition of 'DeeSysFD_Quit'
// >> bool DeeSysPipeFD_TryInitEx(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer, DEE_A_IN Dee_size_t size_hint);
// >> void DeeSysPipeFD_InitEx(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer, DEE_A_IN Dee_size_t size_hint, CODE on_error);
// >> bool DeeSysPipeFD_TryInit(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer);
// >> void DeeSysPipeFD_Init(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer, CODE on_error);
// >> [[optional]] bool DeeSysPipeFD_TryRead(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs);
// >> [[optional]] bool DeeSysPipeFD_TryWrite(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws);
// >> [[optional]] void DeeSysPipeFD_Read(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> [[optional]] void DeeSysPipeFD_Write(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);
//]]


#ifdef DEE_PLATFORM_WINDOWS
# include <deemon/sys/_win32.sysfd.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_unix.sysfd.h>
#else
# error "No File implementation for this platform"
#endif




//////////////////////////////////////////////////////////////////////////
// Implement missing, substitutable callbacks
#ifndef DeeSysFileFD_Filename
#ifdef DeeSysFileFD_Utf8Filename
# define DeeSysFileFD_Filename DeeSysFileFD_Utf8Filename
#elif defined(DeeSysFileFD_WideFilename)
# define DeeSysFileFD_Filename DeeSysFileFD_WideFilename
#endif
#endif
#if defined(DeeSysPipeFD_TryInitEx) \
 || defined(DeeSysPipeFD_TryInit)
#ifndef DeeSysPipeFD_TryInit
# define DeeSysPipeFD_TryInit(reader,writer) DeeSysPipeFD_TryInitEx(reader,writer,0)
#endif
#ifndef DeeSysPipeFD_TryInitEx
# define DeeSysPipeFD_TryInitEx(reader,writer,size_hint) DeeSysPipeFD_TryInit(reader,writer)
#endif
#endif
#if defined(DeeSysPipeFD_InitEx) \
 || defined(DeeSysPipeFD_Init)
#ifndef DeeSysPipeFD_Init
# define DeeSysPipeFD_Init(reader,writer) DeeSysPipeFD_InitEx(reader,writer,0)
#endif
#ifndef DeeSysPipeFD_InitEx
# define DeeSysPipeFD_InitEx(reader,writer,size_hint) DeeSysPipeFD_Init(reader,writer)
#endif
#endif


#endif /* !GUARD_DEEMON_SYS_SYSFD_H */
