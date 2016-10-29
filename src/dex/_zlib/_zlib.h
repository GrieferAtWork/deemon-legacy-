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
#ifndef GUARD_DEEMON_DEX_ZLIB_H
#define GUARD_DEEMON_DEX_ZLIB_H 1
 
#include <deemon/__conf.inl>
#include <deemon/dex.h>
#include <deemon/object.h>
#include <deemon/string.h>
#include DEE_INCLUDE_MEMORY_API()


#ifdef GUARD_DEEMON_DEX_ZLIB_C
#ifndef __BYTE_ORDER__
#define __BYTE_ORDER__          DEE_PLATFORM_ENDIAN
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __ORDER_BIG_ENDIAN__    4321
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4820 4668 4365 4548)
#include "miniz.inl"
DEE_COMPILER_MSVC_WARNING_POP
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// High-level compress/uncompress callbacks
// Returns non-zero on error
#define DeeZLib_Compress(writer,p,s,compressed_size) \
 DeeZLib_CompressEx(writer,p,s,compressed_size,MZ_DEFAULT_COMPRESSION)
extern DEE_A_RET_EXCEPT(-1) int DeeZLib_CompressEx(
 DEE_A_INOUT DeeStringWriter *writer, DEE_A_IN_R(s) void const *p, DEE_A_IN size_t s,
 DEE_A_OUT_OPT size_t *compressed_size, DEE_A_IN int level);
extern DEE_A_RET_EXCEPT(-1) int DeeZLib_Uncompress(
 DEE_A_INOUT DeeStringWriter *writer, DEE_A_IN_R(s) void const *p,
 DEE_A_IN size_t s, DEE_A_OUT_OPT size_t *uncompressed_size);


extern void DeeZLib_ThrowError(DEE_A_IN int error);
extern DEE_A_RET_Z char const *DeeZLib_ErrorName(DEE_A_IN int error);


extern DeeTypeObject DeeZLibError_Type; // Extends Error.ValueError

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ZLIB_H */
