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

// TODO: Support for fchdir


//////////////////////////////////////////////////////////////////////////
// >> [[optional]] #define DEE_SYSFD_SEEK_SET <implementation_dependent>
// >> [[optional]] #define DEE_SYSFD_SEEK_CUR <implementation_dependent>
// >> [[optional]] #define DEE_SYSFD_SEEK_END <implementation_dependent>
//
// >> [[optional]] struct DeeSysFD { ... };
// >> [[optional]] void DeeSysFD_Quit(DEE_A_INOUT struct DeeSysFD *self);
// >> [[optional]] void DeeSysFD_InitCopy(DEE_A_INOUT struct DeeSysFD *self, DEE_A_INOUT struct DeeSysFD *right, CODE on_error);
// >> [[optional]] void DeeSysFD_Read(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> [[optional]] void DeeSysFD_Write(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);
//         - Read/Write data, or return false/throw an Error.IOError
// >> [[optional]] void DeeSysFD_Seek(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff, CODE on_error);
//         - Moves the fd's r/w pointer to a given position, or return false/throw an Error.IOError / Error.RuntimeError.NotImplemented
// >> [[optional]] void DeeSysFD_Flush(DEE_A_INOUT struct DeeSysFD *self, CODE on_error);
//         - Flushes unwritten data, or return false/throw an Error.IOError
// >> [[optional]] void DeeSysFD_Trunc(DEE_A_INOUT struct DeeSysFD *self, CODE on_error);
//         - Truncate 'this' fd to its current r/w pointer, or return false/throw an Error.IOError
// >> [[optional]] void DeeSysFD_GetSize(DEE_A_IN struct DeeSysFD const *self, DEE_A_OUT Dee_uint64_t *result, CODE on_error);
//
// >> [[optional]] #define DeeSysFD_INIT_STDIN()   {...}
// >> [[optional]] #define DeeSysFD_INIT_STDOUT()  {...}
// >> [[optional]] #define DeeSysFD_INIT_STDERR()  {...}
//         - Static initializer for a 'DeeSysFD' structure, that initialize
//           to that structure with the same values as the getter functions below.
//           NOTE: These initializer macros are optional and may be
//                 omitted if not implementable on the hosting platform.
// >> [[optional]] void DeeSysFD_GET_STDIN(DEE_A_OUT struct DeeSysFD *self);
// >> [[optional]] void DeeSysFD_GET_STDOUT(DEE_A_OUT struct DeeSysFD *self);
// >> [[optional]] void DeeSysFD_GET_STDERR(DEE_A_OUT struct DeeSysFD *self);
//         - Fills '*self' with information about the stdin/stdout/stderr file descriptors.
//           NOTE: The generated 'self' object should not be destroyed
//                 with 'DeeSysFD_Quit', but may be closed.
//
// >> [[optional]] struct DeeSysFileFD: DeeSysFD { ... };
// >> [[optional]] void DeeSysFileFD_Quit(DEE_A_IN struct DeeSysFileFD *self);
//         - Destruction code that must be execute in addition of 'DeeSysFD_Quit'
// >> [[optional]] void DeeSysFileFD_Utf8Init(struct DeeSysFileFD *self, Dee_Utf8Char const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> [[optional]] void DeeSysFileFD_WideInit(struct DeeSysFileFD *self, Dee_WideChar const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Utf8InitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
// >> [[optional]] void DeeSysFileFD_WideInitObject(struct DeeSysFileFD *self, DeeObject const *filename, Dee_uint16_t mode, Dee_mode_t perms, CODE on_error);
//         - Opens a given file-descriptor, or execute 'on_error' after throwing an Error.IOError
//         - NOTE: These function can be used to open a directory
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeSysFileFD_Filename(DEE_A_IN struct DeeSysFileFD const *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFileFD_Utf8Filename(DEE_A_IN struct DeeSysFileFD const *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *DeeSysFileFD_WideFilename(DEE_A_IN struct DeeSysFileFD const *self);
//         - Returns the filename associated with a given fs-file-descriptor
// >> [[optional]] void DeeSysFileFD_Read(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Write(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Seek(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN Dee_int64_t pos, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *newoff, CODE on_error);
// >> [[optional]] void DeeSysFileFD_GetTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
// >> [[optional]] void DeeSysFileFD_SetTimes(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsFile(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsDir(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsLink(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsDrive(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsMount(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsHidden(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsExecutable(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsCharDev(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsBlockDev(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsFiFo(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_IsSocket(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_GetMod(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT Dee_mode_t *result, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Chmod(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN Dee_mode_t mode, CODE on_error);
// >> [[optional]] void DeeSysFileFD_GetOwn(DEE_A_INOUT struct DeeSysFD *self, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group, CODE on_error);
// >> [[optional]] void DeeSysFileFD_Chown(DEE_A_INOUT struct DeeSysFD *self, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group, CODE on_error);
// 
//
// >> [[optional]] struct DeeSysPipeFD: DeeSysFD { ... };
//         - Pipes are generally an optional platform-dependent feature,
//           not being required per-sé, but being a simple way of doing for IPC.
//           They are optional for the same reason why processes themselves are optional
//           as well: To keep the deemon runtime compatible with embedded systems.
//         - NOTE: Even when pipes are available, the platform may not require overwriting
//                 of the r/w operators, meaning those are optional even then (check for existence with #ifdef).
// >> [[optional]] void DeeSysPipeFD_Quit(DEE_A_IN struct DeeSysPipeFD *self);
//         - Destruction code that must be execute in addition of 'DeeSysFD_Quit'
// >> [[optional]] void DeeSysPipeFD_InitEx(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer, DEE_A_IN Dee_size_t size_hint, CODE on_error);
// >> [[optional]] void DeeSysPipeFD_Init(DEE_A_OUT struct DeeSysPipeFD *reader, DEE_A_OUT struct DeeSysPipeFD *writer, CODE on_error);
// >> [[optional]] void DeeSysPipeFD_Read(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_OUT_WB(*rs) void *p, DEE_A_IN size_t s, DEE_A_OUT size_t *rs, CODE on_error);
// >> [[optional]] void DeeSysPipeFD_Write(DEE_A_INOUT struct DeeSysFileFD *self, DEE_A_IN_RB(*ws) void const *p, DEE_A_IN size_t s, DEE_A_OUT size_t *ws, CODE on_error);


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.sysfd.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_unix.sysfd.h>
#elif 1
// Stub: Everything is optional...
#else
# error "No File implementation for this platform"
#endif


#if !defined(DeeSysFileFD_Utf8InitObject)\
  && defined(DeeSysFileFD_Utf8Init)
#define DeeSysFileFD_Utf8InitObject(self,filename,mode,perms,...) \
 DeeSysFileFD_Utf8Init(self,DeeUtf8String_STR(filename),mode,perms,__VA_ARGS__)
#endif
#if !defined(DeeSysFileFD_WideInitObject)\
  && defined(DeeSysFileFD_WideInit)
#define DeeSysFileFD_WideInitObject(self,filename,mode,perms,...) \
 DeeSysFileFD_WideInit(self,DeeWideString_STR(filename),mode,perms,__VA_ARGS__)
#endif


#if !defined(DeeSysFileFD_Utf8Init) && !defined(DeeSysFileFD_Utf8InitObject)\
 && !defined(DeeSysFileFD_WideInit) && !defined(DeeSysFileFD_WideInitObject)
// Without any initializers, make sure that no member functions are defined either
#undef DeeSysFileFD
#undef DeeSysFileFD_Quit
#undef DeeSysFileFD_Filename
#undef DeeSysFileFD_Utf8Filename
#undef DeeSysFileFD_WideFilename
#undef DeeSysFileFD_Read
#undef DeeSysFileFD_Write
#undef DeeSysFileFD_Seek
#undef DeeSysFileFD_GetTimes
#undef DeeSysFileFD_SetTimes
#undef DeeSysFileFD_IsFile
#undef DeeSysFileFD_IsDir
#undef DeeSysFileFD_IsLink
#undef DeeSysFileFD_IsDrive
#undef DeeSysFileFD_IsMount
#undef DeeSysFileFD_IsHidden
#undef DeeSysFileFD_IsExecutable
#undef DeeSysFileFD_IsCharDev
#undef DeeSysFileFD_IsBlockDev
#undef DeeSysFileFD_IsFiFo
#undef DeeSysFileFD_IsSocket
#undef DeeSysFileFD_GetMod
#undef DeeSysFileFD_Chmod
#undef DeeSysFileFD_GetOwn
#undef DeeSysFileFD_Chown
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
#if defined(DeeSysPipeFD_InitEx) \
 || defined(DeeSysPipeFD_Init)
#ifndef DeeSysPipeFD_Init
# define DeeSysPipeFD_Init(reader,writer,...) DeeSysPipeFD_InitEx(reader,writer,0,__VA_ARGS__)
#endif
#ifndef DeeSysPipeFD_InitEx
# define DeeSysPipeFD_InitEx(reader,writer,size_hint,...) DeeSysPipeFD_Init(reader,writer,__VA_ARGS__)
#endif
#endif


#endif /* !GUARD_DEEMON_SYS_SYSFD_H */
