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
#ifdef __INTELLISENSE__
#include "object.c"
DEE_DECL_BEGIN
#endif

// Stats:
// 
//   == Hashing ~185000 human-written, non-recurring ascii strings ==
//      Cycles:  32
//      Average: 0.117661 seconds
//      Total:   3.765137 seconds
//      Min:     0.111744 seconds
//      Max:     0.133254 seconds

// ... Ok. So I did the same test with the old algo, and didn't really get any different results...
// >> Whether or not this algo is really better might need some more testing
//    (like running it in non-debug mode, counting collisions, etc.)

// This Hash function is based on code from here:
// https://en.wikipedia.org/wiki/MurmurHash
// It was referenced as pretty good here:
// http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed

#define ROT32(x, y) ((x << y) | (x >> (32 - y))) // avoid effort
#define c1 0xcc9e2d51
#define c2 0x1b873593
#define r1 15
#define r2 13
#define m  5
#define n  0xe6546b64
uint32_t DeeHash_Memory(void const *p, Dee_size_t s, uint32_t seed) {
 uint8_t const *tail;
 uint32_t k1; Dee_size_t i;
 Dee_size_t const nblocks = s / 4;
 uint32_t const *blocks = (uint32_t const *)p;
 uint32_t hash = seed;
 uint32_t k;
 for (i = 0; i < nblocks; ++i) {
  k = blocks[i];
  k *= c1;
  k = ROT32(k,r1);
  k *= c2;
  hash ^= k;
  hash = ROT32(hash,r2)*m+n;
 }
 tail = ((uint8_t const *)p)+(nblocks*4);
 k1 = 0;
 switch (s & 3) {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
   k1 *= c1;
   k1 = ROT32(k1, r1);
   k1 *= c2;
   hash ^= k1;
 }
 hash ^= s;
 hash ^= (hash >> 16);
 hash *= 0x85ebca6b;
 hash ^= (hash >> 13);
 hash *= 0xc2b2ae35;
 hash ^= (hash >> 16);
 return hash;
}
#undef c1
#undef c2
#undef r1
#undef r2
#undef m
#undef n


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

