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

/*
#if defined(__INTELLISENSE__) && 1
#include "_hashlib.c"
#define WIDTH         8
#define IN_REFLECTED  0
#define OUT_REFLECTED 0
#endif
*/

#ifndef WIDTH
#error "Must '#define WIDTH' before #including this file"
#endif
#ifndef IN_REFLECTED
#error "Must '#define IN_REFLECTED' before #including this file"
#endif
#ifndef OUT_REFLECTED
#error "Must '#define OUT_REFLECTED' before #including this file"
#endif
#if IN_REFLECTED != 0 && IN_REFLECTED != 1
#error "'IN_REFLECTED' must be defined as 0|1"
#endif
#if OUT_REFLECTED != 0 && OUT_REFLECTED != 1
#error "'OUT_REFLECTED' must be defined as 0|1"
#endif


#if WIDTH > 32
#define SIZE   64
#define HASH_T Dee_uint64_t
#elif WIDTH > 16
#define SIZE   32
#define HASH_T Dee_uint32_t
#elif WIDTH > 8
#define SIZE   16
#define HASH_T Dee_uint16_t
#else
#define SIZE   8
#define HASH_T Dee_uint8_t
#endif

#if IN_REFLECTED && OUT_REFLECTED
#define NAME   DEE_PP_CAT_3(_hashimpl_,WIDTH,_ioref)
#elif OUT_REFLECTED
#define NAME   DEE_PP_CAT_3(_hashimpl_,WIDTH,_oref)
#elif OUT_REFLECTED
#define NAME   DEE_PP_CAT_3(_hashimpl_,WIDTH,_oref)
#else
#define NAME   DEE_PP_CAT_2(_hashimpl_,WIDTH)
#endif

#define ALGO   DEE_PP_CAT_3(DeeCRC,SIZE,Algorithm)


static HASH_T DEE_CALL NAME(
 struct ALGO const *self, HASH_T start, Dee_uint8_t *p, Dee_size_t s) {
 DEE_ASSERT(self);
 DEE_ASSERT(self->crc_algo.crc_width == WIDTH);
 DEE_ASSERT(self->crc_algo.crc_size == SIZE/8);
 DEE_ASSERT(!s || p);
#if SIZE != WIDTH
 start <<= (SIZE-WIDTH);
#endif
#if SIZE > 8
#if OUT_REFLECTED
 while (s--) start = (HASH_T)(self->crc_table[(
  Dee_uint8_t)(((start >> (SIZE-WIDTH)) ^ *p++) & 0xFF)] ^ (start >> 8));
/*#if !IN_REFLECTED
 start >>= (SIZE-WIDTH);
 {
  unsigned int i;
  for (i = 0; i < 12; i++) {
   if ((start & 0x800) != 0) {
    start <<= 1;
    start ^= 0x80f;
   } else {
    start <<= 1;
   }
  }
  / *start = crc_reflect(start,12);* /
  return start & 0xFFF;
 }
#endif*/
#else
 while (s--) start = (HASH_T)(self->crc_table[(
  Dee_uint8_t)((start >> ((SIZE-WIDTH)+(WIDTH-8))) ^ *p++)] ^ (start << 8));
#endif
#else
#if OUT_REFLECTED && (SIZE != WIDTH)
 while (s--) start = (HASH_T)(self->crc_table[(Dee_uint8_t)(
  (start >> (SIZE-WIDTH)) ^ *p++)]);
#else
 while (s--) start = (HASH_T)(self->crc_table[start ^ *p++]);
#endif
#endif
#if SIZE != WIDTH
 start >>= (SIZE-WIDTH);
#endif
 return start;
}





#undef ALGO
#undef NAME
#undef HASH_T
#undef SIZE
#undef WIDTH
#undef IN_REFLECTED
#undef OUT_REFLECTED

