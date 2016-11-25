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
#ifndef GUARD_DEEMON_OPTIONAL_FS_API_FSAPIMODE_H
#define GUARD_DEEMON_OPTIONAL_FS_API_FSAPIMODE_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef DEE_TYPES_UINT16_T Dee_fsapimode_t;
#define DEE_FSAPIMODE_NONE       DEE_UINT16_C(0x0000)
#define DEE_FSAPIMODE_EXPANDVARS DEE_UINT16_C(0x0001)
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_FSAPIMODE_ENABLEVFS  DEE_UINT16_C(0x0002)
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_FSAPIMODE_DEFAULT   (DEE_FSAPIMODE_EXPANDVARS|DEE_FSAPIMODE_ENABLEVFS)
#else /* DEE_CONFIG_RUNTIME_HAVE_VFS */
#define DEE_FSAPIMODE_DEFAULT   (DEE_FSAPIMODE_EXPANDVARS)
#endif /* !DEE_CONFIG_RUNTIME_HAVE_VFS */

//////////////////////////////////////////////////////////////////////////
// Get/Set the current FS-API Mode
// NOTE: When attempting to set an invalid/unsupported mode, an Error.ValueError will be thrown.
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_fsapimode_t) DeeFS_GetAPIMode(void);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeFS_SetAPIMode(DEE_A_IN Dee_fsapimode_t mode);

typedef void (*DeeFunPointer)(void);
//////////////////////////////////////////////////////////////////////////
// Returns the function pointer of a given ID, associated with the selected FS-API mode.
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL DeeFunPointer) DeeFS_GetFunction_v102(
 DEE_A_IN DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T) id);
#if defined(_MSC_VER) && !defined(__INTELLISENSE__)
#define DeeFS_GETFUNCTION_v102(T,id) __pragma(warning(suppress: 4191)) \
                                     (*(T)DeeFS_GetFunction_v102(DEE_TYPES_SIZEOF_POINTER*id))
#else
#define DeeFS_GETFUNCTION_v102(T,id) (*(T)DeeFS_GetFunction_v102(DEE_TYPES_SIZEOF_POINTER*id))
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FS_API_FSAPIMODE_H */
