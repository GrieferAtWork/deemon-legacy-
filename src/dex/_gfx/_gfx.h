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
#ifndef GUARD_DEEMON_DEX_GFX__GFX_H
#define GUARD_DEEMON_DEX_GFX__GFX_H 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
#include <deemon/type.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#define DEE_BIT(n)  (1<<((n)-1)) /*< Integer with the nth bit set. */
#define DEE_BITS(n) ((1<<(n))-1) /*< Integer with all bits leading up to the nth set. */
#define DEE_FIRSTBIT32(mask)\
(((mask)&0x80000000) ?  0 : ((mask)&0x40000000) ?  1 : \
 ((mask)&0x20000000) ?  2 : ((mask)&0x10000000) ?  3 : \
 ((mask)&0x08000000) ?  4 : ((mask)&0x04000000) ?  5 : \
 ((mask)&0x02000000) ?  6 : ((mask)&0x01000000) ?  7 : \
 ((mask)&0x00800000) ?  8 : ((mask)&0x00400000) ?  9 : \
 ((mask)&0x00200000) ? 10 : ((mask)&0x00100000) ? 11 : \
 ((mask)&0x00080000) ? 12 : ((mask)&0x00040000) ? 13 : \
 ((mask)&0x00020000) ? 14 : ((mask)&0x00010000) ? 15 : \
 ((mask)&0x00008000) ? 16 : ((mask)&0x00004000) ? 17 : \
 ((mask)&0x00002000) ? 18 : ((mask)&0x00001000) ? 19 : \
 ((mask)&0x00000800) ? 20 : ((mask)&0x00000400) ? 21 : \
 ((mask)&0x00000200) ? 22 : ((mask)&0x00000100) ? 23 : \
 ((mask)&0x00000080) ? 24 : ((mask)&0x00000040) ? 25 : \
 ((mask)&0x00000020) ? 26 : ((mask)&0x00000010) ? 27 : \
 ((mask)&0x00000008) ? 28 : ((mask)&0x00000004) ? 29 : \
 ((mask)&0x00000002) ? 30 :/*((mask)&0x00000001)?*/31)
#define DEE_LASTBIT32(mask)\
(((mask)&0x00000001) ?  0 : ((mask)&0x00000002) ?  1 : \
 ((mask)&0x00000004) ?  2 : ((mask)&0x00000008) ?  3 : \
 ((mask)&0x00000010) ?  4 : ((mask)&0x00000020) ?  5 : \
 ((mask)&0x00000040) ?  6 : ((mask)&0x00000080) ?  7 : \
 ((mask)&0x00000100) ?  8 : ((mask)&0x00000200) ?  9 : \
 ((mask)&0x00000400) ? 10 : ((mask)&0x00000800) ? 11 : \
 ((mask)&0x00001000) ? 12 : ((mask)&0x00002000) ? 13 : \
 ((mask)&0x00004000) ? 14 : ((mask)&0x00008000) ? 15 : \
 ((mask)&0x00010000) ? 16 : ((mask)&0x00020000) ? 17 : \
 ((mask)&0x00040000) ? 18 : ((mask)&0x00080000) ? 19 : \
 ((mask)&0x00100000) ? 20 : ((mask)&0x00200000) ? 21 : \
 ((mask)&0x00400000) ? 22 : ((mask)&0x00800000) ? 23 : \
 ((mask)&0x01000000) ? 24 : ((mask)&0x02000000) ? 25 : \
 ((mask)&0x04000000) ? 26 : ((mask)&0x08000000) ? 27 : \
 ((mask)&0x10000000) ? 28 : ((mask)&0x20000000) ? 29 : \
 ((mask)&0x40000000) ? 30 : ((mask)&0x80000000) ? 31 : 0)

//////////////////////////////////////////////////////////////////////////
// Encode/Decode the offset/bits of a given color channel into 11 bits to data
// >> [3:offbits][8:data]
// -> 'offbits' is the amount of bits used in 'data' to store the offset (at its start)
//    When equal to 0x7, the channel isn't available.
//    The remaining data is the offset
#define DEE_BITSFOR(x) ((x) >= 16 ? 5 : (x) >= 8 ? 4 : (x) >= 4 ? 3 : (x) >= 2 ? 2 : (x) >= 1 ? 1 : 0)
#define DEE_PRIVATE_CHANNEL_ENCODE(off,bits) \
 ((bits)?((DEE_BITSFOR(off) << 8)|(((bits)-1) << DEE_BITSFOR(off))|(off)):0x700)
#define DEE_PRIVATE_CHANNEL_HAS(fmt)      (((fmt)&0x700)!=0x700)
#define DEE_PRIVATE_CHANNEL_BITBITS(fmt)  (8-(((fmt)&0x700) >> 8))
#define DEE_PRIVATE_CHANNEL_OFFBITS(fmt)     (((fmt)&0x700) >> 8)
#define DEE_PRIVATE_CHANNEL_BIT(fmt)\
 ((((fmt) >> DEE_PRIVATE_CHANNEL_OFFBITS(fmt))&\
    DEE_BITS(DEE_PRIVATE_CHANNEL_BITBITS(fmt)))+1)
#define DEE_PRIVATE_CHANNEL_OFF(fmt)\
 ((fmt)&DEE_BITS(DEE_PRIVATE_CHANNEL_OFFBITS(fmt)))
#define DEE_PRIVATE_CHANNEL_MASK(fmt)\
 ((Dee_uint32_t)DEE_BITS(DEE_PRIVATE_CHANNEL_BIT(fmt)) << DEE_PRIVATE_CHANNEL_OFF(fmt))

#define DEE_PRIVATE_CHANNEL_MASKSIZE(mask)\
 DEE_PRIVATE_CHANNEL_MASKSIZE2((mask) >> DEE_LASTBIT32(mask))
#define DEE_PRIVATE_CHANNEL_MASKSIZE2(mask)\
(((mask)&0x80000000) ? 32 : ((mask)&0x40000000) ? 31 : \
 ((mask)&0x20000000) ? 30 : ((mask)&0x10000000) ? 29 : \
 ((mask)&0x08000000) ? 28 : ((mask)&0x04000000) ? 27 : \
 ((mask)&0x02000000) ? 26 : ((mask)&0x01000000) ? 25 : \
 ((mask)&0x00800000) ? 24 : ((mask)&0x00400000) ? 23 : \
 ((mask)&0x00200000) ? 22 : ((mask)&0x00100000) ? 21 : \
 ((mask)&0x00080000) ? 20 : ((mask)&0x00040000) ? 19 : \
 ((mask)&0x00020000) ? 18 : ((mask)&0x00010000) ? 17 : \
 ((mask)&0x00008000) ? 16 : ((mask)&0x00004000) ? 15 : \
 ((mask)&0x00002000) ? 14 : ((mask)&0x00001000) ? 13 : \
 ((mask)&0x00000800) ? 12 : ((mask)&0x00000400) ? 11 : \
 ((mask)&0x00000200) ? 10 : ((mask)&0x00000100) ?  9 : \
 ((mask)&0x00000080) ?  8 : ((mask)&0x00000040) ?  7 : \
 ((mask)&0x00000020) ?  6 : ((mask)&0x00000010) ?  5 : \
 ((mask)&0x00000008) ?  4 : ((mask)&0x00000004) ?  3 : \
 ((mask)&0x00000002) ?  2 : ((mask)&0x00000001) ?  1 : 0)
#define DEE_PRIVATE_CHANNEL_ENCODE_MASK(mask)\
 DEE_PRIVATE_CHANNEL_ENCODE(DEE_FIRSTBIT32(mask),DEE_PRIVATE_CHANNEL_MASKSIZE(mask))


//////////////////////////////////////////////////////////////////////////
// List of known surfacetype format kinds
#define DEE_SURFACETYPE_FORMAT_KIND_NONE    DEE_UINT64_C(0x0000000000000000)
#define DEE_SURFACETYPE_FORMAT_KIND_PIXEL   DEE_UINT64_C(0x1000000000000000)
#define DEE_SURFACETYPE_FORMAT_KIND_INDEX   DEE_UINT64_C(0x2000000000000000)
#define DEE_SURFACETYPE_FORMAT_KIND(fmt)  ((fmt)&DEE_UINT64_C(0xF000000000000000))

//////////////////////////////////////////////////////////////////////////
// === Pixel-based Surface Format ===
//////////////////////////////////////////////////////////////////////////
// >> Dee_uint64_t DEE_SURFACETYPE_FORMAT_PIXEL(
// >>   DEE_A_IN Dee_uint32_t rmask, DEE_A_IN Dee_uint32_t gmask,
// >>   DEE_A_IN Dee_uint32_t bmask, DEE_A_IN Dee_uint32_t amask);
//////////////////////////////////////////////////////////////////////////
// Generates a surfacetype ID from a given set of channel masks.
//  - Set a given mask to 0 to disable that channel
#define DEE_SURFACETYPE_FORMAT_PIXEL(rmask,gmask,bmask,amask)\
(DEE_SURFACETYPE_FORMAT_KIND_PIXEL\
|((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(rmask)) << 33\
|((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(gmask)) << 22\
|((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(bmask)) << 11\
|((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(amask)))
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt) ((fmt) >> 33)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt) ((fmt) >> 22)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt) ((fmt) >> 11)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A      /* nothing */

#define DEE_SURFACETYPE_FORMAT_PIXEL_HAS_R(fmt)  DEE_PRIVATE_CHANNEL_HAS(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_HAS_G(fmt)  DEE_PRIVATE_CHANNEL_HAS(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_HAS_B(fmt)  DEE_PRIVATE_CHANNEL_HAS(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_HAS_A(fmt)  DEE_PRIVATE_CHANNEL_HAS(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_OFF_R(fmt)  DEE_PRIVATE_CHANNEL_OFF(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_OFF_G(fmt)  DEE_PRIVATE_CHANNEL_OFF(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_OFF_B(fmt)  DEE_PRIVATE_CHANNEL_OFF(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_OFF_A(fmt)  DEE_PRIVATE_CHANNEL_OFF(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_BIT_R(fmt)  DEE_PRIVATE_CHANNEL_BIT(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_BIT_G(fmt)  DEE_PRIVATE_CHANNEL_BIT(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_BIT_B(fmt)  DEE_PRIVATE_CHANNEL_BIT(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_BIT_A(fmt)  DEE_PRIVATE_CHANNEL_BIT(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_MASK_R(fmt) DEE_PRIVATE_CHANNEL_MASK(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_MASK_G(fmt) DEE_PRIVATE_CHANNEL_MASK(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_MASK_B(fmt) DEE_PRIVATE_CHANNEL_MASK(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt))
#define DEE_SURFACETYPE_FORMAT_PIXEL_MASK_A(fmt) DEE_PRIVATE_CHANNEL_MASK(DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A(fmt))


//////////////////////////////////////////////////////////////////////////
// === Index-based Surface Format ===
//////////////////////////////////////////////////////////////////////////
// >> Dee_uint64_t DEE_SURFACETYPE_FORMAT_INDEX(
// >>   DEE_A_IN Dee_uint8_t index_bits, DEE_A_IN int index_bitorder);
//////////////////////////////////////////////////////////////////////////
// Generates a surfacetype ID from a given set of data.
//  - Set 'index_bitorder' to 1234 if indices start with the MSB (most significant bit)
//    Set to 4321 if they start with the LSB (least significant bit)
#define DEE_SURFACETYPE_FORMAT_INDEX(index_bits,index_bitorder) \
 (DEE_SURFACETYPE_FORMAT_KIND_INDEX\
 |(Dee_uint64_t)(Dee_uint8_t)(index_bits)\
 |(Dee_uint64_t)((index_bitorder) != 1234 ? DEE_UINT64_C(0x0000000000000100) : DEE_UINT64_C(0x0000000000000000)))
#define DEE_SURFACETYPE_FORMAT_INDEX_INDEXBITS(fmt)    ((Dee_uint8_t)(fmt))
#define DEE_SURFACETYPE_FORMAT_INDEX_INDEX_IS_MSB(fmt) (((fmt)&DEE_UINT64_C(0x0000000000000100))==0)
#define DEE_SURFACETYPE_FORMAT_INDEX_INDEX_IS_LSB(fmt) (((fmt)&DEE_UINT64_C(0x0000000000000100))!=0)


#if 1
static Dee_uint64_t const format_rgba8888 = DEE_UINT64_C(0x1000000F1E2F85F8);
#else
static Dee_uint64_t const format_rgba8888 =
 DEE_SURFACETYPE_FORMAT_PIXEL(0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
#endif

DEE_OBJECT_DEF(DeeSurfaceTypeObject);
DEE_OBJECT_DEF(DeeSurfaceObject);

struct DeePixel { Dee_uint8_t r,g,b,a; };

//////////////////////////////////////////////////////////////////////////
// Selection of standard colors
#define DeePixel_AQUA    ((struct DeePixel const *)"\x00\xff\xff\xff")
#define DeePixel_BLACK   ((struct DeePixel const *)"\x00\x00\x00\xff")
#define DeePixel_BLUE    ((struct DeePixel const *)"\x00\x00\xff\xff")
#define DeePixel_FUCHSIA ((struct DeePixel const *)"\xff\x00\xff\xff")
#define DeePixel_GRAY    ((struct DeePixel const *)"\x80\x80\x80\xff")
#define DeePixel_GREEN   ((struct DeePixel const *)"\x00\x80\x00\xff")
#define DeePixel_LIME    ((struct DeePixel const *)"\x00\xff\x00\xff")
#define DeePixel_MAROON  ((struct DeePixel const *)"\x80\x00\x00\xff")
#define DeePixel_NAVY    ((struct DeePixel const *)"\x00\x00\x80\xff")
#define DeePixel_OLIVE   ((struct DeePixel const *)"\x80\x80\x00\xff")
#define DeePixel_PURPLE  ((struct DeePixel const *)"\x80\x00\x80\xff")
#define DeePixel_RED     ((struct DeePixel const *)"\xff\x00\x00\xff")
#define DeePixel_YELLOW  ((struct DeePixel const *)"\xff\xff\x00\xff")
#define DeePixel_SILVER  ((struct DeePixel const *)"\xc0\xc0\xc0\xff")
#define DeePixel_TEAL    ((struct DeePixel const *)"\x00\x80\x80\xff")
#define DeePixel_WHITE   ((struct DeePixel const *)"\xff\xff\xff\xff")
#define DeePixel_EMPTY   ((struct DeePixel const *)"\xff\xff\xff\x00")


typedef Dee_uint64_t Dee_blendinfo_t;
#define DEE_BLENDMODE_ZERO                     0x00
#define DEE_BLENDMODE_ONE                      0x01
#define DEE_BLENDMODE_SRC_COLOR                0x02
#define DEE_BLENDMODE_ONE_MINUS_SRC_COLOR      0x03
#define DEE_BLENDMODE_DST_COLOR                0x04
#define DEE_BLENDMODE_ONE_MINUS_DST_COLOR      0x05
#define DEE_BLENDMODE_SRC_ALPHA                0x06
#define DEE_BLENDMODE_ONE_MINUS_SRC_ALPHA      0x07
#define DEE_BLENDMODE_DST_ALPHA                0x08
#define DEE_BLENDMODE_ONE_MINUS_DST_ALPHA      0x09
#define DEE_BLENDMODE_SRC_ALPHA_SATURATE       0x0A
#define DEE_BLENDMODE_CONSTANT_COLOR           0x0B
#define DEE_BLENDMODE_ONE_MINUS_CONSTANT_COLOR 0x0C
#define DEE_BLENDMODE_CONSTANT_ALPHA           0x0D
#define DEE_BLENDMODE_ONE_MINUS_CONSTANT_ALPHA 0x0E
#define DEE_BLENDFUNC_ADD                      0x00
#define DEE_BLENDFUNC_SUBTRACT                 0x01
#define DEE_BLENDFUNC_REVERSE_SUBTRACT         0x02
#define DEE_BLENDFUNC_MIN                      0x03
#define DEE_BLENDFUNC_MAX                      0x04

#define DEE_BLENDINFO_GET_RGBSRC(info) ((Dee_uint8_t)((info)&0xf))
#define DEE_BLENDINFO_GET_ASRC(info)   ((Dee_uint8_t)(((info)>>4)&0xf))
#define DEE_BLENDINFO_GET_RGBDST(info) ((Dee_uint8_t)(((info)>>8)&0xf))
#define DEE_BLENDINFO_GET_ADST(info)   ((Dee_uint8_t)(((info)>>12)&0xf))
#define DEE_BLENDINFO_GET_RGBFUN(info) ((Dee_uint8_t)(((info)>>15)&0x7))
#define DEE_BLENDINFO_GET_AFUN(info)   ((Dee_uint8_t)(((info)>>18)&0x7))
#define DEE_BLENDINFO_GET_CR(info)     ((Dee_uint8_t)(((info)>>56)&0xff))
#define DEE_BLENDINFO_GET_CG(info)     ((Dee_uint8_t)(((info)>>48)&0xff))
#define DEE_BLENDINFO_GET_CB(info)     ((Dee_uint8_t)(((info)>>40)&0xff))
#define DEE_BLENDINFO_GET_CA(info)     ((Dee_uint8_t)(((info)>>32)&0xff))

#define DEE_BLENDINFO(src,dst,func) DEE_BLENDINFO_EX(src,src,dst,dst,func,func,0,0,0,0)
#define DEE_BLENDINFO_EX(rgb_src,a_src,rgb_dst,a_dst,rgb_fun,a_fun,cr,cg,cb,ca)\
 ((Dee_uint64_t)((rgb_src)&0xf)       |((Dee_uint64_t)((a_src)&0xf) <<  4)\
|((Dee_uint64_t)((rgb_dst)&0xf) <<  8)|((Dee_uint64_t)((a_dst)&0xf) << 12)\
|((Dee_uint64_t)((rgb_fun)&0x7) << 15)|((Dee_uint64_t)((a_fun)&0x7) << 18)\
|((Dee_uint64_t)((cr)&0xff) << 56)|((Dee_uint64_t)((cg)&0xff) << 48)\
|((Dee_uint64_t)((cb)&0xff) << 40)|((Dee_uint64_t)((ca)&0xff) << 32))

//////////////////////////////////////////////////////////////////////////
// Default blending mode (what you would call alpha blending)
#define DEE_BLENDINFO_NORMAL    DEE_BLENDINFO(DEE_BLENDMODE_SRC_ALPHA,DEE_BLENDMODE_ONE_MINUS_SRC_ALPHA,DEE_BLENDFUNC_ADD)

//////////////////////////////////////////////////////////////////////////
// Override blending mode (simply override 'dst' with 'src')
#define DEE_BLENDINFO_OVERRIDE  DEE_BLENDINFO(DEE_BLENDMODE_ZERO,DEE_BLENDMODE_ONE,DEE_BLENDFUNC_ADD)



//////////////////////////////////////////////////////////////////////////
// Fill a given pixel map with standard values meant
// to cover as many different colors as possible.
extern void DeePixel_FillStandardIndexMap(
 DEE_A_OUT_W(size) struct DeePixel *map, DEE_A_IN Dee_size_t size);
extern void DeePixel_Blend(
 DEE_A_INOUT struct DeePixel *dst,
 DEE_A_IN struct DeePixel const *src,
 DEE_A_IN Dee_blendinfo_t info);


typedef void (DEE_CALL *PDeeSurfaceGetPixel)(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_OUT struct DeePixel *pixel);
typedef void (DEE_CALL *PDeeSurfaceSetPixel)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_IN struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t info);
typedef void (DEE_CALL *PDeeSurfaceFill)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceFillRect)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t xbegin,
 DEE_A_IN Dee_size_t ybegin, DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t yend,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceLineLLHH)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t xbegin,
 DEE_A_IN Dee_size_t ybegin_lo, DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t yend_hi,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceLineLHHL)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t xbegin,
 DEE_A_IN Dee_size_t beginy_hi, DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t endy_lo,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceHLine)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t xbegin, DEE_A_IN Dee_size_t xend,
 DEE_A_IN Dee_size_t y, DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceVLine)(
 DEE_A_INOUT DeeSurfaceObject *self, DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t beginy,
 DEE_A_IN Dee_size_t endy, DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceBlit)(
 DEE_A_INOUT DeeSurfaceObject *self,
 DEE_A_IN Dee_size_t dst_x, DEE_A_IN Dee_size_t dst_y, // Destination coords in 'self'
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t src_x, DEE_A_IN Dee_size_t src_y, // Source coords in 'src'
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy, // Pixel copy dimensions (must fit in src-srccoord)
 DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceStretchBlit)(
 DEE_A_INOUT DeeSurfaceObject *self,
 DEE_A_IN Dee_size_t dst_x, DEE_A_IN Dee_size_t dst_y, // Destination coords in 'self'
 DEE_A_IN Dee_size_t dstsx, DEE_A_IN Dee_size_t dstsy, // Destination dimensions in 'self'
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t src_x, DEE_A_IN Dee_size_t src_y, // Source coords in 'src'
 DEE_A_IN Dee_size_t srcsx, DEE_A_IN Dee_size_t srcsy, // Source dimensions in 'src'
 DEE_A_IN Dee_blendinfo_t blend);

struct DeeSurfaceTypeObject {
 DeeTypeObject          st_base;
 Dee_uint64_t           st_format;
 union{ /*< DEE_SURFACETYPE_FORMAT_KIND(st_format). */
  struct { /*< DEE_SURFACETYPE_FORMAT_KIND_PIXEL >*/
   Dee_uint8_t   st_pixelbytes; /*< Size of a single pixel. */
   Dee_uint32_t  st_rmask;      /*< 32-bit mask for the red channel. */
   Dee_uint32_t  st_gmask;      /*< 32-bit mask for the green channel. */
   Dee_uint32_t  st_bmask;      /*< 32-bit mask for the blue channel. */
   Dee_uint32_t  st_amask;      /*< 32-bit mask for the alpha channel. */
   Dee_uint8_t   st_rshift;     /*< [0..31] Required shift for reading/writing the red channel. */
   Dee_uint8_t   st_gshift;     /*< [0..31] Required shift for reading/writing the green channel. */
   Dee_uint8_t   st_bshift;     /*< [0..31] Required shift for reading/writing the blue channel. */
   Dee_uint8_t   st_ashift;     /*< [0..31] Required shift for reading/writing the alpha channel. */
   Dee_uint8_t   st_rbits;      /*< [0..8] Amount of bits used by the red channel. */
   Dee_uint8_t   st_gbits;      /*< [0..8] Amount of bits used by the green channel. */
   Dee_uint8_t   st_bbits;      /*< [0..8] Amount of bits used by the blue channel. */
   Dee_uint8_t   st_abits;      /*< [0..8] Amount of bits used by the alpha channel. */
   Dee_uint16_t  st_rmod;       /*< == (1 << st_rbits). */
   Dee_uint16_t  st_gmod;       /*< == (1 << st_gbits). */
   Dee_uint16_t  st_bmod;       /*< == (1 << st_bbits). */
   Dee_uint16_t  st_amod;       /*< == (1 << st_abits). */
   Dee_uint16_t  st_rinvmod;    /*< == (1 << (8-st_rbits)). */
   Dee_uint16_t  st_ginvmod;    /*< == (1 << (8-st_gbits)). */
   Dee_uint16_t  st_binvmod;    /*< == (1 << (8-st_bbits)). */
   Dee_uint16_t  st_ainvmod;    /*< == (1 << (8-st_abits)). */
  } st_pixelspec;
  struct { /*< DEE_SURFACETYPE_FORMAT_KIND_INDEX >*/
   Dee_uint8_t   st_indexbits; /*< Amount of index bits / pixel (also the size of a pixel; may not be byte aligned). */
   Dee_size_t    st_mapsize;   /*< Size of the index map (entry count). */
  } st_indexspec;
 };
 PDeeSurfaceGetPixel    st_getpixel;    /*< [1..1]. */
 PDeeSurfaceSetPixel    st_setpixel;    /*< [1..1]. */
 PDeeSurfaceFill        st_fill;        /*< [1..1]. */
 PDeeSurfaceFillRect    st_fillrect;    /*< [1..1]. */
 PDeeSurfaceHLine       st_xline;       /*< [1..1]. */
 PDeeSurfaceVLine       st_yline;       /*< [1..1]. */
 PDeeSurfaceLineLLHH    st_linellhh;    /*< [1..1]. */
 PDeeSurfaceLineLHHL    st_linelhhl;    /*< [1..1]. */
 PDeeSurfaceBlit        st_blit;        /*< [1..1]. */
 PDeeSurfaceStretchBlit st_stretchblit; /*< [1..1]. */
};
#define DeeSurfaceType_PIXEL_GETPIXEL(ty,datap,result)\
do{\
 Dee_uint32_t data;\
 switch ((ty)->st_pixelspec.st_pixelbytes) {\
  case 1:  data = (Dee_uint32_t)*(Dee_uint8_t *)(datap); break;\
  case 2:  data = (Dee_uint32_t)*(Dee_uint16_t *)(datap); break;\
  case 3:  data = ((Dee_uint32_t)*(Dee_uint16_t *)(datap) << 8) | ((Dee_uint8_t *)(datap))[2]; break;\
  default: data = *(Dee_uint32_t *)(datap); break;\
 }\
 (result)->r = (Dee_uint8_t)(((((data)&(ty)->st_pixelspec.st_rmask) >> (ty)->st_pixelspec.st_rshift)*256)/(ty)->st_pixelspec.st_rmod);\
 (result)->g = (Dee_uint8_t)(((((data)&(ty)->st_pixelspec.st_gmask) >> (ty)->st_pixelspec.st_gshift)*256)/(ty)->st_pixelspec.st_gmod);\
 (result)->b = (Dee_uint8_t)(((((data)&(ty)->st_pixelspec.st_bmask) >> (ty)->st_pixelspec.st_bshift)*256)/(ty)->st_pixelspec.st_bmod);\
 (result)->a = (ty)->st_pixelspec.st_abits\
             ? (Dee_uint8_t)(((((data)&(ty)->st_pixelspec.st_amask) >> (ty)->st_pixelspec.st_ashift)*256)/(ty)->st_pixelspec.st_amod)\
             : (Dee_uint8_t)0xFF;\
}while(0)
struct _dee3bytes { Dee_uint8_t data[3]; };
#define DeeSurfaceType_PIXEL_SETPIXEL(ty,datap,val)\
do{\
 Dee_uint32_t newdata;\
 newdata = (((Dee_uint32_t)((val)->r/(ty)->st_pixelspec.st_rinvmod) << (ty)->st_pixelspec.st_rshift) & (ty)->st_pixelspec.st_rmask)\
         | (((Dee_uint32_t)((val)->g/(ty)->st_pixelspec.st_ginvmod) << (ty)->st_pixelspec.st_gshift) & (ty)->st_pixelspec.st_gmask)\
         | (((Dee_uint32_t)((val)->b/(ty)->st_pixelspec.st_binvmod) << (ty)->st_pixelspec.st_bshift) & (ty)->st_pixelspec.st_bmask)\
         | (((Dee_uint32_t)((val)->a/(ty)->st_pixelspec.st_ainvmod) << (ty)->st_pixelspec.st_ashift) & (ty)->st_pixelspec.st_amask);\
 switch ((ty)->st_pixelspec.st_pixelbytes) {\
  case 1: *(Dee_uint8_t *)(datap)     = (Dee_uint8_t)newdata; break;\
  case 2: *(Dee_uint16_t *)(datap)    = (Dee_uint16_t)newdata; break;\
  case 3: *(Dee_uint16_t *)(datap)    = (Dee_uint16_t)(newdata >> 8);\
          ((Dee_uint8_t *)(datap))[3] = (Dee_uint8_t)newdata; break;\
  default: *(Dee_uint32_t *)(datap)   = newdata; break;\
 }\
}while(0)


#define DeeSurfaceType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSurfaceType_Type)
#define DeeSurfaceType_Check          DeeSurfaceType_CheckExact
extern DeeTypeObject DeeSurfaceType_Type;
#define DeeSurfaceType_FORMAT(ob) ((DeeSurfaceTypeObject *)Dee_REQUIRES_POINTER(ob))->st_format
#define DeeSurfaceType_KIND(ob)   DEE_SURFACETYPE_FORMAT_KIND(DeeSurfaceType_FORMAT(ob))


struct DeeSurfaceObject {
 DEE_OBJECT_HEAD_EX(DeeSurfaceTypeObject)
 Dee_size_t s_sizex; /*< Surface size in x (aka. width). */
 Dee_size_t s_sizey; /*< Surface size in y (aka. height). */
 union{
  struct{
   Dee_size_t  s_pixlsize;     /*< Size of a single pixel (== __ob_type->st_pixelspec.st_pixelbytes). */
   Dee_size_t  s_scanline;     /*< Amount of bytes per scanline (== s_sizex*s_pixlsize). */
   Dee_uint8_t s_pixels[4096]; /*< Raw pixeldata (pixeladdr = x*s_pixlsize+y*s_scanline) */
#define DeeSurface_PIXELADDR(self,x,y) \
 (void *)((self)->s_pixeldata.s_pixels+(x)*(self)->s_pixeldata.s_pixlsize+(y)*(self)->s_pixeldata.s_scanline)
#define DeeSurface_PIXELBASESIZE Dee_OFFSETOF(DeeSurfaceObject,s_pixeldata.s_pixels)
  } s_pixeldata;
  struct {
#define DeeSurface_INDEXMAP(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_indexdata.s_map
#define DeeSurface_INDEXPIX(ob) ((Dee_uint8_t *)(((Dee_uintptr_t)DeeSurface_INDEXMAP(ob))+DeeSurface_TYPE(ob)->st_indexspec.st_mapsize))
   struct DeePixel s_map[256];     /*< [__ob_type->st_indexspec.st_mapsize] Index map (Actual size depends on surface type). */
   Dee_uint8_t     s_pixels[4096]; /*< Raw pixeldata (pixeladdr = x*s_pixlsize+y*s_scanline) */
  } s_indexdata;
 };
};

// v Base type for all surfaces
extern DeeSurfaceTypeObject DeeSurface_Type;
#if 1
#define DeeSurface_CheckExact(ob) DeeSurfaceType_Check(Dee_TYPE(ob))
#else
#define DeeSurface_CheckExact(ob) DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeSurface_Type)
#endif
#define DeeSurface_Check       DeeSurface_CheckExact
#define DeeSurface_TYPE(ob)  ((DeeSurfaceTypeObject *)Dee_TYPE(ob))
#define DeeSurface_SIZEX(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_sizex
#define DeeSurface_SIZEY(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_sizey


//////////////////////////////////////////////////////////////////////////
// Create a new surface of a given type and dimensions
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_New(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy);
#define DeeSurface_NewEmpty(stype,sx,sy) \
 DeeSurface_NewFilled(stype,sx,sy,DeePixel_EMPTY)
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFilled(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx,
 DEE_A_IN Dee_size_t sy, DEE_A_IN struct DeePixel const *color);


//////////////////////////////////////////////////////////////////////////
// Returns a new surface containing the same data as 'self', but converted to 'ty' format
// >> Same as creating a new surface, the size of 'self' and format 'ty',
//    then proceeding to blit 'self' onto that new surface.
// NOTE: Returns 'self' if 'ty' is the format of 'self'
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_Convert(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_IN DeeSurfaceTypeObject const *ty);


#define DeeSurface_GetPixel(self,x,y,pixel) \
((x) < DeeSurface_SIZEX(self) && (y) < DeeSurface_SIZEY(self)\
  ? _DeeSurface_GetPixel(self,x,y,pixel) : (void)(*(pixel) = *DeePixel_EMPTY))
#define DeeSurface_SetPixel(self,x,y,pixel,blend) \
((x) < DeeSurface_SIZEX(self) && (y) < DeeSurface_SIZEY(self)\
  ? _DeeSurface_SetPixel(self,x,y,pixel,blend) : (void)0)
#define _DeeSurface_GetPixel(self,x,y,pixel) (*DeeSurface_TYPE(self)->st_getpixel)(self,x,y,pixel)
#define _DeeSurface_SetPixel(self,x,y,pixel,blend) (*DeeSurface_TYPE(self)->st_setpixel)(self,x,y,pixel,blend)

#define DeeSurface_Fill(self,color,blend) (*DeeSurface_TYPE(self)->st_fill)(self,color,blend)
#define _DeeSurface_FillRect(self,xbegin,ybegin,xend,yend,color,blend) \
 (*DeeSurface_TYPE(self)->st_fillrect)(self,xbegin,ybegin,xend,yend,color,blend)

//////////////////////////////////////////////////////////////////////////
// Fill a given rectable with a solid color
DEE_STATIC_INLINE(void) DeeSurface_FillRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t y1,
 DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend);

//////////////////////////////////////////////////////////////////////////
// Draw Line parallel to the x/y axis
#define _DeeSurface_XLine(dst,xbegin,xend,y,color,blend)\
 (*DeeSurface_TYPE(dst)->st_xline)(dst,xbegin,xend,y,color,blend)
#define _DeeSurface_YLine(dst,x,beginy,endy,color,blend)\
 (*DeeSurface_TYPE(dst)->st_yline)(dst,x,beginy,endy,color,blend)
DEE_STATIC_INLINE(void) DeeSurface_XLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
DEE_STATIC_INLINE(void) DeeSurface_YLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t y1, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);

//////////////////////////////////////////////////////////////////////////
// Draw A regular of line (may be diagonal)
DEE_STATIC_INLINE(void) DeeSurface_Line(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t y1,
 DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);


//////////////////////////////////////////////////////////////////////////
// Blit a full surface 'src' onto 'dst' at 'dx-dy'
DEE_STATIC_INLINE(void) DeeSurface_Blit(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_blendinfo_t blend);


//////////////////////////////////////////////////////////////////////////
// Blit a given area from 'src' onto 'dst' 'dx-dy'
// NOTE: '_DeeSurface_BlitArea' should only be used if the source-area
//       is confirmed to be contained within the source surface.
//////////////////////////////////////////////////////////////////////////
// >> [dst]-------------+
// >> |                 |    [src]--------------+
// >> |                 |    |                  |
// >> | [dx|dy]---+     |    |  [sx|sx]---+     |
// >> |    |  ~~  |     |    |     |  ~~  |     |
// >> |    | ~~~~ |     |    |     | ~~~~ |     |
// >> |    |  ~~  |     |    |     |  ~~  |     |
// >> |    +------+     |    |     +--[ssx|ssy] |
// >> |       ^         |    |      |           |
// >> |       |         |    |      |           |
// >> |       +---------+----+------+           |
// >> |                 |    |                  |
// >> +-----------------+    +------------------+
//////////////////////////////////////////////////////////////////////////
DEE_STATIC_INLINE(void) DeeSurface_BlitArea(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_BlitArea(dst,dx,dy,src,sx,sy,ssx,ssy,blend) \
 (*DeeSurface_TYPE(dst)->st_blit)(dst,dx,dy,(DeeSurfaceObject *)Dee_REQUIRES_POINTER(src),sx,sy,ssx,ssy,blend)


//////////////////////////////////////////////////////////////////////////
// Stretch-blits a given area from 'src' onto 'dst' 'dx-dy'
// NOTE: Nearest-neighbor interpolation is used
DEE_STATIC_INLINE(void) DeeSurface_StretchBlitArea(
 DEE_A_INOUT DeeSurfaceObject    *dst, DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy, DEE_A_IN Dee_size_t dsx, DEE_A_IN Dee_size_t dsy,
 DEE_A_IN DeeSurfaceObject const *src, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy, DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_StretchBlit(dst,dx,dy,dsx,dsy,src,sx,sy,ssx,ssy,blend) \
 (*DeeSurface_TYPE(dst)->st_stretchblit)(dst,dx,dy,dsx,dsy,\
 (DeeSurfaceObject *)Dee_REQUIRES_POINTER(src),sx,sy,ssx,ssy,blend)


//////////////////////////////////////////////////////////////////////////
// Returns the surface type associated with a given format
extern DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
 DeeSurfaceType_Get(DEE_A_IN Dee_uint64_t format);



//////////////////////////////////////////////////////////////////////////
// Some hard-coded surface types (with special optimizations)
//extern DeeSurfaceTypeObject DeeSurfaceType_RGB888;
extern DeeSurfaceTypeObject DeeSurfaceType_RGBA8888;







//////////////////////////////////////////////////////////////////////////
// Implementation of inline functions
DEE_STATIC_INLINE(void) DeeSurface_XLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t temp;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (y < DeeSurface_SIZEY(dst)) {
  if (x1 < x2) {
   if (x1 < DeeSurface_SIZEX(dst)) {
    temp = x2+1;
    temp = Dee_MIN(temp,DeeSurface_SIZEX(dst));
    _DeeSurface_XLine(dst,x1,y,temp,color,blend);
   }
  } else if (x1 > x2) {
   if (x2 < DeeSurface_SIZEX(dst)) {
    temp = x2+1;
    temp = Dee_MIN(temp,DeeSurface_SIZEX(dst));
    _DeeSurface_XLine(dst,x2,y,temp,color,blend);
   }
  }
 }
}
DEE_STATIC_INLINE(void) DeeSurface_YLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t y1, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t temp;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x < DeeSurface_SIZEX(dst)) {
  if (y1 < y2) {
   if (y1 < DeeSurface_SIZEY(dst)) {
    temp = y2+1;
    temp = Dee_MIN(temp,DeeSurface_SIZEY(dst));
    _DeeSurface_YLine(dst,x,y1,temp,color,blend);
   }
  } else if (y1 > y2) {
   if (y2 < DeeSurface_SIZEY(dst)) {
    temp = y2+1;
    temp = Dee_MIN(temp,DeeSurface_SIZEY(dst));
    _DeeSurface_YLine(dst,x,y2,temp,color,blend);
   }
  }
 }
}

DEE_STATIC_INLINE(void) DeeSurface_Line(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t y1,
 DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_xbegin,used_y1,used_xend,used_y2,sizex,sizey;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x1 < x2) {
  used_xbegin = x1,  used_y1 = y1;
  used_xend   = x2+1,used_y2 = y2;
 } else if (x1 > x2) {
  used_xbegin = x2,  used_y1 = y2;
  used_xend   = x1+1,used_y2 = y1;
 } else {
  DeeSurface_YLine(dst,x1,y1,y2,color,blend);
  return;
 }
 sizex = DeeSurface_SIZEX(dst);
 sizey = DeeSurface_SIZEY(dst);
 DEE_ASSERT(used_xbegin < used_xend);
 if (used_y1 < used_y2) { // ll --> hh
  ++used_y2;
#define used_ybegin used_y1
#define used_yend   used_y2
  if (used_xend > sizex) {
   used_yend -= ((used_xend-sizex)*(used_yend-used_ybegin))/(used_xend-used_xbegin);
   used_xend  = sizex;
  } else if (used_yend > sizey) {
   used_xend -= ((used_yend-sizey)*(used_xend-used_xbegin))/(used_yend-used_ybegin);
   used_yend  = sizey;
  }
  (*DeeSurface_TYPE(dst)->st_linellhh)(dst,used_xbegin,used_ybegin,used_xend,used_yend,color,blend);
#undef used_yend
#undef used_ybegin
 } else if (used_y2 < used_y1) { // lh --> hl
  --used_y2;
#define used_ybegin used_y1 // beginy_hi
#define used_yend   used_y2 // endy_lo
  if (used_xend > sizex) {
   used_yend -= ((used_xend-sizex)*(used_ybegin-used_yend))/(used_xend-used_xbegin);
   used_xend  = sizex;
  } else if (used_yend > sizey) {
   used_xend -= ((used_yend-sizey)*(used_xend-used_xbegin))/(used_ybegin-used_yend);
   used_yend  = sizey;
  }
  (*DeeSurface_TYPE(dst)->st_linelhhl)(dst,used_xbegin,used_ybegin,used_xend,used_yend,color,blend);
#undef used_yend
#undef used_ybegin
 } else if (used_y1 < sizey && used_xbegin < sizex) {
  if (used_xend > sizex) used_xend = sizex;
  _DeeSurface_XLine(dst,used_xbegin,used_xend,used_y1,color,blend);
 }
}

void DeeSurface_Blit(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t srcsx,srcsy,dstsx,dstsy,dstmaxsx,dstmaxsy;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 if (dx >= (dstsx = DeeSurface_SIZEX(dst))) return;
 if (dy >= (dstsy = DeeSurface_SIZEY(dst))) return;
 srcsx = DeeSurface_SIZEX(src),srcsy = DeeSurface_SIZEY(src);
 dstmaxsx = dstsx-dx,dstmaxsy = dstsy-dy;
 (*DeeSurface_TYPE(dst)->st_blit)(dst,dx,dy,src,0,0,
                                  Dee_MIN(srcsx,dstmaxsx),
                                  Dee_MIN(srcsy,dstmaxsy),
                                  blend);
}


DEE_STATIC_INLINE(void) DeeSurface_FillRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x1, DEE_A_IN Dee_size_t y1,
 DEE_A_IN Dee_size_t x2, DEE_A_IN Dee_size_t y2,
 DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_x1,used_y1,used_x2,used_y2,sizex,sizey;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x1 < x2) used_x1 = x1,used_x2 = x2; else used_x1 = x2,used_x2 = x1;
 if (y1 < y2) used_y1 = y1,used_y2 = y2; else used_y1 = y2,used_y2 = y1;
 if (used_x1 >= (sizex = DeeSurface_SIZEX(dst))) return;
 if (used_y1 >= (sizey = DeeSurface_SIZEY(dst))) return;
 if (++used_x2 > sizex) used_x2 = sizex;
 if (++used_y2 > sizey) used_y2 = sizey;
 (*DeeSurface_TYPE(dst)->st_fillrect)(dst,used_x1,used_y1,
                                      used_x2,used_y2,color,blend);
}

DEE_STATIC_INLINE(void) DeeSurface_BlitArea(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t dstsx,dstsy,srcsx,srcsy,maxblitsx,maxblitsy;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 if (dx >= (dstsx = DeeSurface_SIZEX(dst))) return;
 if (dy >= (dstsy = DeeSurface_SIZEY(dst))) return;
 if (sx >= (srcsx = DeeSurface_SIZEX(src))) return;
 if (sy >= (srcsy = DeeSurface_SIZEY(src))) return;
 dstsx -= dx,dstsy -= dy,srcsx -= sx,srcsy -= sy;
 maxblitsx = Dee_MIN(dstsx,srcsx);
 maxblitsy = Dee_MIN(dstsy,srcsy);
 (*DeeSurface_TYPE(dst)->st_blit)(dst,dx,dy,src,sx,sy,
                                  Dee_MIN(ssx,maxblitsx),
                                  Dee_MIN(ssy,maxblitsy),
                                  blend);
}

DEE_STATIC_INLINE(void) DeeSurface_StretchBlitArea(
 DEE_A_INOUT DeeSurfaceObject    *dst, DEE_A_IN Dee_size_t dx, DEE_A_IN Dee_size_t dy, DEE_A_IN Dee_size_t dsx, DEE_A_IN Dee_size_t dsy,
 DEE_A_IN DeeSurfaceObject const *src, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy, DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t dstsx,dstsy,srcsx,srcsy,used_dsx,used_dsy,used_ssx,used_ssy;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 if (dx >= (dstsx = DeeSurface_SIZEX(dst))) return;
 if (dy >= (dstsy = DeeSurface_SIZEY(dst))) return;
 if (sx >= (srcsx = DeeSurface_SIZEX(src))) return;
 if (sy >= (srcsy = DeeSurface_SIZEY(src))) return;
 dstsx -= dx,dstsy -= dy;
 srcsx -= sx,srcsy -= sy;
 used_dsx = dsx,used_dsy = dsy;
 used_ssx = ssx,used_ssy = ssy;
 if (dstsx < used_dsx) { used_ssx = (used_ssx*(used_dsx-dstsx))/used_dsx; used_dsx = dstsx; }
 if (dstsy < used_dsy) { used_ssy = (used_ssy*(used_dsy-dstsy))/used_dsy; used_dsy = dstsy; }
 if (srcsx < used_ssx) { used_dsx = (used_dsx*(used_ssx-srcsx))/used_ssx; used_ssx = srcsx; }
 if (srcsy < used_ssy) { used_dsy = (used_dsy*(used_ssy-srcsy))/used_ssy; used_ssy = srcsy; }
 (*DeeSurface_TYPE(dst)->st_stretchblit)(dst,dx,dy,used_dsx,used_dsy,
                                         src,sx,sy,used_ssx,used_ssy,
                                         blend);
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX__GFX_H */
