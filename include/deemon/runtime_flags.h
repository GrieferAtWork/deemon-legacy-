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
#ifndef GUARD_DEEMON_RUNTIME_FLAGS_H
#define GUARD_DEEMON_RUNTIME_FLAGS_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#ifdef DEE_DEBUG
#ifndef DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED
#define DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED 1
DEE_DATA_DECL(int) _DeeFlag_NoAssert; // deemonrun.c
DEE_DATA_DECL(int) _DeeFlag_Verbose; // deemonrun.c
#endif /* !DEE_PRIVATE_DEBUG_ONLY_FLAGS_DEFINED */
#endif /*DEE_DEBUG */

#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
//////////////////////////////////////////////////////////////////////////
// Enable signal handlers
// NOTE: Can be initialized with 'DEEMON_NOSIGHANDLERS' (s.a.: 'DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS')
// NOTE: Changing this flag while inside a signal-block produces undefined behavior
DEE_DATA_DECL(int) DeeFlag_NoSignalHandlers; // signalhandler.c
#endif /* DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS */

#if DEE_CONFIG_RUNTIME_HAVE_VFS
//////////////////////////////////////////////////////////////////////////
// Enable the virutal file system
// NOTE: Can be initialized with 'DEEMON_NOVFS' (s.a.: 'DEE_AUTOCONF_VARNAME_DEEMON_NOVFS')
DEE_DATA_DECL(int) DeeFlag_NoVFS; // fs_api.c
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
//////////////////////////////////////////////////////////////////////////
// Set the maximum stack limit (aka. stack recursion) for deemon usercode
// NOTE: Can be initialized with 'DEEMON_STACKLIMIT' (s.a.: 'DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT')
// The default is platform and compile-settings dependent
// WARNING: The default is already near the border of what is possible,
//          so don't overwrite this unless you know what you're doing,
//          as you will otherwise start getting real stack overflows.
DEE_DATA_DECL(/*atomic*/Dee_size_t) Dee_StackLimit; // deemonrun.c
#define Dee_GetStackLimit()  (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,Dee_StackLimit,memory_order_seq_cst)
#define Dee_SetStackLimit(v) DeeAtomicN_Store(DEE_TYPES_SIZEOF_SIZE_T,Dee_StackLimit,v,memory_order_seq_cst)
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_RUNTIME_FLAGS_H */
