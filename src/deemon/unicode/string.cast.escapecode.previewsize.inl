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

#ifndef DEE_PREVIEW_QUOTED_LENGTH_FORWARD_DEFINED
#define DEE_PREVIEW_QUOTED_LENGTH_FORWARD_DEFINED 1
#ifdef __INTELLISENSE__
#include "string.c"
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_size_t) _Dee_WidePreviewQuotedLength(DEE_A_IN Dee_WideChar const *s, DEE_A_IN Dee_WideChar const *end);
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_size_t) _Dee_Utf8PreviewQuotedLength(DEE_A_IN Dee_Utf8Char const *s, DEE_A_IN Dee_Utf8Char const *end);
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_size_t) _Dee_Utf16PreviewQuotedLength(DEE_A_IN Dee_Utf16Char const *s, DEE_A_IN Dee_Utf16Char const *end);
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_size_t) _Dee_Utf32PreviewQuotedLength(DEE_A_IN Dee_Utf32Char const *s, DEE_A_IN Dee_Utf32Char const *end);
#else
#define PREVIEW_ENC Wide
#include "string.cast.escapecode.previewsize.impl.inl"
#define PREVIEW_ENC Utf8
#include "string.cast.escapecode.previewsize.impl.inl"
#define PREVIEW_ENC Utf16
#include "string.cast.escapecode.previewsize.impl.inl"
#define PREVIEW_ENC Utf32
#include "string.cast.escapecode.previewsize.impl.inl"
#endif
#endif
