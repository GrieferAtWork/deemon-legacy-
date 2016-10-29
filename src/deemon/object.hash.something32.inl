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

// Some random hash algorithm.
// >> Passed down from throughout the generations and gifted from our ancestors.
// (I have no idea what this one is. It should be somewhere in _hashlib, but I can't be bothered to look it up)

#define I DEE_UINT32_C
static Dee_hash_t const _DeeHashTable[256] = {
 I(0x00000000),I(0x77073096),I(0xee0e612c),I(0x990951ba),I(0x076dc419),I(0x706af48f),I(0xe963a535),I(0x9e6495a3),
 I(0x0edb8832),I(0x79dcb8a4),I(0xe0d5e91e),I(0x97d2d988),I(0x09b64c2b),I(0x7eb17cbd),I(0xe7b82d07),I(0x90bf1d91),
 I(0x1db71064),I(0x6ab020f2),I(0xf3b97148),I(0x84be41de),I(0x1adad47d),I(0x6ddde4eb),I(0xf4d4b551),I(0x83d385c7),
 I(0x136c9856),I(0x646ba8c0),I(0xfd62f97a),I(0x8a65c9ec),I(0x14015c4f),I(0x63066cd9),I(0xfa0f3d63),I(0x8d080df5),
 I(0x3b6e20c8),I(0x4c69105e),I(0xd56041e4),I(0xa2677172),I(0x3c03e4d1),I(0x4b04d447),I(0xd20d85fd),I(0xa50ab56b),
 I(0x35b5a8fa),I(0x42b2986c),I(0xdbbbc9d6),I(0xacbcf940),I(0x32d86ce3),I(0x45df5c75),I(0xdcd60dcf),I(0xabd13d59),
 I(0x26d930ac),I(0x51de003a),I(0xc8d75180),I(0xbfd06132),I(0x21b4f4b5),I(0x56b3c423),I(0xcfba9599),I(0xb8bda50f),
 I(0x2802b89e),I(0x5f058808),I(0xc60cd9b2),I(0xb10be924),I(0x2f6f7c87),I(0x58684c11),I(0xc3211dab),I(0xb6662d3d),
 I(0x76dc4190),I(0x01db7106),I(0x98d220bc),I(0xefd5102a),I(0x71b18589),I(0x06b6b51f),I(0x9fbfe4a5),I(0xe8b8d433),
 I(0x7807c9a2),I(0x0f00f934),I(0x9609a88e),I(0xe10e9818),I(0x7f6a0dbb),I(0x086d3d2d),I(0x93246c97),I(0xe6635c01),
 I(0x6b6b51f4),I(0x1c6c6322),I(0x856530d8),I(0xf262004e),I(0x6c0695ed),I(0x1b01a57b),I(0x8208f4c1),I(0xf50fc457),
 I(0x65b0d9c6),I(0x12b7e950),I(0x8bbeb8ea),I(0xfcb9887c),I(0x62dd1ddf),I(0x15da2d49),I(0x8cd37cf3),I(0xfbd44c65),
 I(0x4db26158),I(0x3ab551ce),I(0xa3bc0074),I(0xd4bb30e2),I(0x4adfa541),I(0x3dd895d7),I(0xa4d1c46d),I(0xd3d6f4fb),
 I(0x4369e96a),I(0x346ed9fc),I(0xad678846),I(0xda60b8d0),I(0x44042d73),I(0x33031de5),I(0xaa0a4c5f),I(0xdd0d7cc9),
 I(0x5005713c),I(0x270241aa),I(0xbe0b1010),I(0xc90c2086),I(0x5768b525),I(0x206f85b3),I(0xb966d409),I(0xce61e49f),
 I(0x5edef90e),I(0x29d9c998),I(0xb0d09822),I(0xc7d7a8b4),I(0x59b33d17),I(0x2eb40d81),I(0xb7bd5c3b),I(0xc0ba6cad),
 I(0xedb88320),I(0x9abfb3b6),I(0x03b6e20c),I(0x74b1d29a),I(0xead54739),I(0x9dd277af),I(0x04db2615),I(0x73dc3283),
 I(0xe3630b12),I(0x94643b84),I(0x0d6d6a3e),I(0x7a6a5aa8),I(0xe40ecf0b),I(0x9309ff9d),I(0x0a00ae27),I(0x7d079eb1),
 I(0xf00f9344),I(0x8708a3d2),I(0x1e01f268),I(0x6906c2fe),I(0xf762575d),I(0x806567cb),I(0x196c3671),I(0x6e6b06e7),
 I(0xfed41b76),I(0x89d32be0),I(0x10da7a5a),I(0x67dd4acc),I(0xf9b9df6f),I(0x8ebeeff9),I(0x17b7be43),I(0x60b08ed5),
 I(0xd6d6a3e8),I(0xa1d1937e),I(0x38d8c2c4),I(0x4fdff252),I(0xd1bb67f1),I(0xa6bc5767),I(0x3fb506dd),I(0x48b2364b),
 I(0xd80d2bda),I(0xaf0a1b4c),I(0x36034af6),I(0x41047a60),I(0xdf60efc3),I(0xa867df55),I(0x332e8eef),I(0x4669be79),
 I(0xcb61b38c),I(0xbc66831a),I(0x256fd2a0),I(0x5268e236),I(0xcc0c7795),I(0xbb0b4703),I(0x220232b9),I(0x5505262f),
 I(0xc5ba3bbe),I(0xb2bd0b28),I(0x2bb45a92),I(0x5cb36a04),I(0xc2d7ffa7),I(0xb5d0cf31),I(0x2cd99e8b),I(0x5bdeae1d),
 I(0x9b64c2b0),I(0xec63f226),I(0x756aa39c),I(0x026d930a),I(0x9c0906a9),I(0xeb0e363f),I(0x72076785),I(0x05005713),
 I(0x95bf4a82),I(0xe2b87a14),I(0x7bb12bae),I(0x0cb61b38),I(0x92d28e9b),I(0xe5d5be0d),I(0x7cdcefb7),I(0x0bdbdf21),
 I(0x86d3d2d4),I(0xf1d4e242),I(0x68ddb3f8),I(0x1fda836e),I(0x81be32cd),I(0xf6b9265b),I(0x6fb077e1),I(0x18b74777),
 I(0x88085ae6),I(0xff0f6a70),I(0x66063bca),I(0x11010b5c),I(0x8f659eff),I(0xf862ae69),I(0x632bffd3),I(0x326ccf45),
 I(0xa00ae278),I(0xd70dd2ee),I(0x4e048354),I(0x3903b3c2),I(0xa7672661),I(0xd06032f7),I(0x4969474d),I(0x3e6e77db),
 I(0xaed32a4a),I(0xd9d65adc),I(0x40df0b66),I(0x37d83bf0),I(0xa9bcae53),I(0xdebb9ec5),I(0x47b2cf7f),I(0x30b5ffe9),
 I(0xbdbdf21c),I(0xcabac28a),I(0x53b39330),I(0x24b4a3a6),I(0xbad03605),I(0xcdd70693),I(0x54de5729),I(0x23d967bf),
 I(0xb3667a2e),I(0xc4614ab8),I(0x5d681b02),I(0x2a6f2b94),I(0xb40bbe37),I(0xc30c8ea1),I(0x5a05df1b),I(0x2d02ef8d)
};
#undef I

DEE_A_RET_WUNUSED Dee_hash_t DeeHash_Memory(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN Dee_hash_t start) {
 DEE_ASSERT(!s || p);
 start = ~start;
 if DEE_LIKELY(s) while (1) {
  start = _DeeHashTable[(start ^ *(Dee_uint8_t const*)p) & DEE_UINT32_C(0x000000FF)] ^ (start >> 8);
  if DEE_UNLIKELY(!--s) break;
  ++*(Dee_uint8_t const**)&p;
 }
 return ~start;
}


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

