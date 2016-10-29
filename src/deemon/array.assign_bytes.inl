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

#if defined(__INTELLISENSE__) && 0
#include "array.c"
#define BYTESIZE 8
DEE_DECL_BEGIN
#endif

#ifndef BYTESIZE
#error "Must '#define BYTESIZE' before #including this file"
#endif


static int DEE_PP_CAT_2(_DeeArray_AssignIntegralRangeFromBytes,BYTESIZE)(
 DeeStructuredTypeObject *elem_type, Dee_size_t elem_count,
 Dee_uint8_t *dst, DEE_PP_CAT_3(Dee_uint,BYTESIZE,_t) const *src) {
 Dee_size_t elem_size;
 DEE_ASSERT(DeeType_Check(elem_type) && DeeStructuredType_Check(elem_type));
 DEE_ASSERT(!elem_count || dst);
 DEE_ASSERT(!elem_count || src);
 DEE_ASSERT(DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 1 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 2 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 4 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 8);
 elem_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size);
 switch (elem_size) {
  // Simple case: memcpy all the way
  case BYTESIZE/8: memcpy(dst,src,elem_count); break;
#if BYTESIZE != 8
  case 1: while (elem_count--) *(Dee_uint8_t  *)dst++ = (Dee_uint8_t )*src++; break;
#endif
#if BYTESIZE != 16
  case 2: while (elem_count--) *(Dee_uint16_t *)dst++ = (Dee_uint16_t)*src++; break;
#endif
#if BYTESIZE != 32
  case 4: while (elem_count--) *(Dee_uint32_t *)dst++ = (Dee_uint32_t)*src++; break;
#endif
#if BYTESIZE != 64
  case 8: while (elem_count--) *(Dee_uint64_t *)dst++ = (Dee_uint64_t)*src++; break;
#endif
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return 0;
}


#undef BYTESIZE
//#ifdef __INTELLISENSE__
//DEE_DECL_END
//#endif


