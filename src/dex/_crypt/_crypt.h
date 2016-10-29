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
#ifndef GUARD_DEEMON_DEX_CRYPT_H
#define GUARD_DEEMON_DEX_CRYPT_H 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// Encode/Decode base64 memory
// NOTE: Set 'salt' to NULL for default salting
// NOTE: Not setting 'salt' uses "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define DeeCrypt_Base64Encode(writer,p,s) DeeCrypt_Base64EncodeEx(writer,p,s,(char const *)0)
#define DeeCrypt_Base64Decode(writer,p,s) DeeCrypt_Base64DecodeEx(writer,p,s,(char const *)0)
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeCrypt_Base64EncodeEx(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s, DEE_A_IN_RB_OPT(64) char const *salt);
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeCrypt_Base64DecodeEx(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s, DEE_A_IN_RB_OPT(64) char const *salt);




DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_CRYPT_H */
