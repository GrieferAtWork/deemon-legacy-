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
#ifndef GUARD_DEEMON_DEX_SYSTEM_H
#define GUARD_DEEMON_DEX_SYSTEM_H 1
 
#include <deemon/__conf.inl>
#include <deemon/dex.h>

//////////////////////////////////////////////////////////////////////////
//
// Dex extension implementing deemon's core <deemon/system.h> file, starting in v102
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif




//////////////////////////////////////////////////////////////////////////
// Returns the current uptime of the system in milliseconds
// >> Used to implement '/proc/uptime' on windows VFS
extern DEE_A_RET_EXCEPT(-1) int DeeSystem_GetUptimeImpl(
 DEE_A_OUT Dee_uint64_t *msecs) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the amount of (possibly) available CPUs
extern DEE_A_RET_EXCEPT(-1) int DeeSystem_GetCPUCountImpl(
 DEE_A_OUT Dee_uint64_t *ncpus) DEE_ATTRIBUTE_NONNULL((1));



DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_SYSTEM_H */
