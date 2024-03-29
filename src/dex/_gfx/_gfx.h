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
#ifdef __DEEMON__
#pragma warning(disable: 16)
#endif
 
#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/object_refcnt.h>
#include <deemon/optional/type_newinstance.h>
#include <deemon/type.h>
#include <deemon/__bswap_intrin.inl>

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeePixelObject);
DEE_OBJECT_DEF(DeeSurfaceTypeObject);
DEE_OBJECT_DEF(DeeSurfaceObject);

#ifdef DEE_PLATFORM_64_BIT
typedef long double Dee_sizefloat_t;
#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif
#else
typedef double Dee_sizefloat_t;
#endif

#define DEE_SIZE_C  DEE_PP_CAT_3(DEE_UINT,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T),_C)
#define DEE_SSIZE_C  DEE_PP_CAT_3(DEE_INT,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T),_C)

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
// Get/Set a 24-bit unsigned integer value (required for 3-channel pixelformats)
#ifdef DEE_PLATFORM_LIL_ENDIAN
#define DeeUInt24_Get(p) \
 (((Dee_uint32_t)*(Dee_uint16_t *)(p) << 8) | (Dee_uint32_t)((Dee_uint8_t *)(p))[2])
#define DeeUInt24_Set(p,v) \
 (*(Dee_uint16_t *)(p) = (Dee_uint16_t)((v) >> 8),\
   ((Dee_uint8_t *)(p))[2] = (Dee_uint8_t)(v))
#else
#define DeeUInt24_Get(p) \
 ((Dee_uint32_t)*(Dee_uint16_t *)((Dee_uintptr_t)(p)+1) | (Dee_uint32_t)(*(Dee_uint8_t *)(p)) << 16)
#define DeeUInt24_Set(p,v) \
 (*(Dee_uint16_t *)((Dee_uintptr_t)(p)+1) = (Dee_uint16_t)(v),\
  *((Dee_uint8_t *)(p)) = (Dee_uint8_t)((Dee_uint32_t)(v) >> 16))
#endif



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
// >>   DEE_A_IN Dee_uint8_t color_bits,
// >>   DEE_A_IN Dee_uint32_t rmask, DEE_A_IN Dee_uint32_t gmask,
// >>   DEE_A_IN Dee_uint32_t bmask, DEE_A_IN Dee_uint32_t amask);
//////////////////////////////////////////////////////////////////////////
// Generates a surfacetype ID from a given set of channel masks.
//  - Set a given mask to 0 to disable that channel
// WARNING: You really shouldn't use the macro.
//          It does work, but you wouldn't believe how much code it expands into...
//          Use the inline-function below whenever possible.
#define DEE_SURFACETYPE_FORMAT_PIXEL(color_bits,rmask,gmask,bmask,amask)\
 (DEE_SURFACETYPE_FORMAT_KIND_PIXEL\
 |((Dee_uint64_t)((((color_bits)/8)-1)&0x3) << 44)\
 |((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(rmask)) << 33\
 |((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(gmask)) << 22\
 |((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(bmask)) << 11\
 |((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE_MASK(amask)))
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_uint64_t) DEE_SURFACETYPE_FORMAT_PIXEL_(
 DEE_A_IN Dee_uint8_t color_bits,
 DEE_A_IN Dee_uint32_t rmask, DEE_A_IN Dee_uint32_t gmask,
 DEE_A_IN Dee_uint32_t bmask, DEE_A_IN Dee_uint32_t amask) {
 Dee_uint8_t off,bit; Dee_uint32_t aligned_mask;
 Dee_uint64_t result = DEE_SURFACETYPE_FORMAT_KIND_PIXEL;
 result |= (Dee_uint64_t)((((color_bits)/8)-1)&0x3) << 44;
#define DEE_GFX_ENCODE_CHANNEL(mask,offset)\
do{\
 off = (Dee_uint8_t)DEE_FIRSTBIT32(mask);\
 aligned_mask = (mask) >> (Dee_uint8_t)DEE_LASTBIT32(mask);\
 bit = (Dee_uint8_t)DEE_PRIVATE_CHANNEL_MASKSIZE2(aligned_mask);\
 result |= ((Dee_uint64_t)DEE_PRIVATE_CHANNEL_ENCODE(off,bit)) << (offset);\
}while(0)
 DEE_GFX_ENCODE_CHANNEL(rmask,33);
 DEE_GFX_ENCODE_CHANNEL(gmask,22);
 DEE_GFX_ENCODE_CHANNEL(bmask,11);
 DEE_GFX_ENCODE_CHANNEL(amask, 0);
#undef DEE_GFX_ENCODE_CHANNEL
 return result;
}
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_R(fmt)    ((fmt) >> 33)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_G(fmt)    ((fmt) >> 22)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_B(fmt)    ((fmt) >> 11)
#define DEE_PRIVATE_SURFACETYPE_FORMAT_PIXEL_A         /* nothing */

#define DEE_SURFACETYPE_FORMAT_PIXEL_BYTES(fmt)  ((((fmt) >> 44)&0x3)+1)
#define DEE_SURFACETYPE_FORMAT_PIXEL_BITS(fmt)   (DEE_SURFACETYPE_FORMAT_PIXEL_BYTES(fmt)*8)
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
#define DeeSurfaceTypeFormat_RGBA8888    DEE_UINT64_C(0x1000300F1E2F85F8)
#else
static Dee_uint64_t const DeeSurfaceTypeFormat_RGBA8888 = DEE_SURFACETYPE_FORMAT_PIXEL(32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
#endif

struct DeePixel { Dee_uint8_t r,g,b,a; };
#define DeePixel_EQUALS(a,b) (*(Dee_uint32_t const *)(a) == *(Dee_uint32_t const *)(b))

//////////////////////////////////////////////////////////////////////////
// Selection of standard colors
// NOTE: After adding more colors here, make sure to run:
//     $ deemon -F _gfx.h
//     $ deemon -F pixelobject.inl
#define DeePixel_AQUA    ((struct DeePixel const *)"\x00\xff\xff\xff")
#define DeePixel_BLACK   ((struct DeePixel const *)"\x00\x00\x00\xff")
#define DeePixel_BLUE    ((struct DeePixel const *)"\x00\x00\xff\xff")
#define DeePixel_EMPTY   ((struct DeePixel const *)"\xff\xff\xff\x00")
#define DeePixel_FUCHSIA ((struct DeePixel const *)"\xff\x00\xff\xff")
#define DeePixel_GRAY    ((struct DeePixel const *)"\x80\x80\x80\xff")
#define DeePixel_GREEN   ((struct DeePixel const *)"\x00\x80\x00\xff")
#define DeePixel_LIME    ((struct DeePixel const *)"\x00\xff\x00\xff")
#define DeePixel_MAROON  ((struct DeePixel const *)"\x80\x00\x00\xff")
#define DeePixel_NAVY    ((struct DeePixel const *)"\x00\x00\x80\xff")
#define DeePixel_OLIVE   ((struct DeePixel const *)"\x80\x80\x00\xff")
#define DeePixel_PURPLE  ((struct DeePixel const *)"\x80\x00\x80\xff")
#define DeePixel_RED     ((struct DeePixel const *)"\xff\x00\x00\xff")
#define DeePixel_SILVER  ((struct DeePixel const *)"\xc0\xc0\xc0\xff")
#define DeePixel_TEAL    ((struct DeePixel const *)"\x00\x80\x80\xff")
#define DeePixel_WHITE   ((struct DeePixel const *)"\xff\xff\xff\xff")
#define DeePixel_YELLOW  ((struct DeePixel const *)"\xff\xff\x00\xff")
#ifdef __DEEMON__
// Used for autoformat: Returns a sequence of all builtin pixel names
#define COLLECT_PIXEL_NAMES()\
(for (local line: file.open("_gfx.h")) ({\
  local name,r,g,b,a;\
  try name,r,g,b,a = line.scanf(" # define DeePixel_%[A-Za-z0-9] ( ( struct DeePixel const * ) \"\\x%[a-fA-F0-9]\\x%[a-fA-F0-9]\\x%[a-fA-F0-9]\\x%[a-fA-F0-9]\" )")...;\
  catch (e...) continue;\
  pack name,r,g,b,a;\
}))
#endif


//////////////////////////////////////////////////////////////////////////
// Returns the name of a builtin pixel color, or NULL if unknown/custom
// NOTE: The names match the class attributes of '_gfx.pixel', which are the
//       lowercase variants of the * in the 'DeePixel_*' constants above.
extern DEE_A_RET_Z_OPT char const *DeePixel_Name(DEE_A_IN struct DeePixel const *pixel);
extern DEE_A_RET_MAYBE_NULL struct DeePixel const *DeePixel_FromName(DEE_A_IN_Z char const *name);
extern DEE_A_RET_NOEXCEPT(0) int DeePixel_InitFromHTML(
 DEE_A_OUT struct DeePixel *self, DEE_A_IN_Z char const *text_after_hash,
 DEE_A_ON_FAILURE(DEE_A_OUT) char const **error_pos);


struct DeePixelObject {
 DEE_OBJECT_HEAD
 // Immutable representation object for pixels
 // NOTE: This one has its own cache!
 struct DeePixel p_pixel; /*< [const] Pixel */
};
#define DeePixel_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeePixel_Type)
#define DeePixel_Check          DeePixel_CheckExact
extern DeeTypeObject DeePixel_Type;
extern DEE_A_RET_EXCEPT_REF DeePixelObject *DeePixel_New(
 DEE_A_IN struct DeePixel const *pixel);

/*[[[deemon
#include <file>
for (local name,r,g,b,a: COLLECT_PIXEL_NAMES()) {
  print "extern DeePixelObject DeePixel_"+name.capitalize()+"Object;";
}
]]]*/
extern DeePixelObject DeePixel_AquaObject;
extern DeePixelObject DeePixel_BlackObject;
extern DeePixelObject DeePixel_BlueObject;
extern DeePixelObject DeePixel_EmptyObject;
extern DeePixelObject DeePixel_FuchsiaObject;
extern DeePixelObject DeePixel_GrayObject;
extern DeePixelObject DeePixel_GreenObject;
extern DeePixelObject DeePixel_LimeObject;
extern DeePixelObject DeePixel_MaroonObject;
extern DeePixelObject DeePixel_NavyObject;
extern DeePixelObject DeePixel_OliveObject;
extern DeePixelObject DeePixel_PurpleObject;
extern DeePixelObject DeePixel_RedObject;
extern DeePixelObject DeePixel_SilverObject;
extern DeePixelObject DeePixel_TealObject;
extern DeePixelObject DeePixel_WhiteObject;
extern DeePixelObject DeePixel_YellowObject;
//[[[end]]]




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
#define DEE_BLENDINFO_OVERRIDE  DEE_BLENDINFO(DEE_BLENDMODE_ONE,DEE_BLENDMODE_ZERO,DEE_BLENDFUNC_ADD)



//////////////////////////////////////////////////////////////////////////
// Fill a given pixel map with standard values meant
// to cover as many different colors as possible.
extern void DeePixel_FillStandardIndexMap(
 DEE_A_OUT_W(size) struct DeePixel *map, DEE_A_IN Dee_size_t size);
extern void DeePixel_Blend(
 DEE_A_INOUT struct DeePixel *dst,
 DEE_A_IN struct DeePixel const *src,
 DEE_A_IN Dee_blendinfo_t info);




struct DeeFontCharacter {
 Dee_uint8_t fc_size;     /*< Size of the character in x/y. */
#define DeeFontCharacter_SIZEX(ob) ((Dee_uint8_t)( (ob)->fc_size&0x0f)    )
#define DeeFontCharacter_SIZEY(ob) ((Dee_uint8_t)(((ob)->fc_size&0xf0)>>4))
 Dee_uint8_t fc_lines[8]; /*< 8x8 bitmask of used pixels (in msb format). */
};
struct DeeFont {
 Dee_uint8_t             f_spacex;   /*< Space between characters in x. */
 Dee_uint8_t             f_spacey;   /*< Space between characters in y. */
 struct DeeFontCharacter f_chr[256]; /*< Array of character descriptors. */
};

extern struct DeeFont DeeFont_Default;





typedef void (DEE_CALL *PDeeSurfaceGetPixel)(
 DEE_A_IN DeeSurfaceObject const *dst, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_OUT struct DeePixel *pixel);
typedef void (DEE_CALL *PDeeSurfaceSetPixel)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_IN struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t info);
typedef void (DEE_CALL *PDeeSurfaceFill)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceFillRect)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t xbegin,
 DEE_A_IN Dee_size_t ybegin, DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t yend,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceLineLLHH)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_IN Dee_size_t sizex, DEE_A_IN Dee_size_t sizey,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceLineLHHL)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t x,
 DEE_A_IN Dee_size_t y, DEE_A_IN Dee_size_t sizex, DEE_A_IN Dee_size_t sizey,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceXLine)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t xbegin, DEE_A_IN Dee_size_t xend,
 DEE_A_IN Dee_size_t y, DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceYLine)(
 DEE_A_INOUT DeeSurfaceObject *dst, DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t ybegin,
 DEE_A_IN Dee_size_t yend, DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceBlit)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dst_x, DEE_A_IN Dee_size_t dst_y, // Destination coords in 'dst'
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t src_x, DEE_A_IN Dee_size_t src_y, // Source coords in 'src'
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy, // Pixel copy dimensions (must fit in src-srccoord)
 DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceStretchBlit)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t dst_x, DEE_A_IN Dee_size_t dst_y, // Destination coords in 'dst'
 DEE_A_IN Dee_size_t dstsx, DEE_A_IN Dee_size_t dstsy, // Destination dimensions in 'dst'
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_sizefloat_t src_x, DEE_A_IN Dee_sizefloat_t src_y, // Source coords in 'src'
 DEE_A_IN Dee_sizefloat_t srcsx, DEE_A_IN Dee_sizefloat_t srcsy, // Source dimensions in 'src'
 DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfaceFlipX)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t xbegin, DEE_A_IN Dee_size_t ybegin,
 DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t yend);
typedef void (DEE_CALL *PDeeSurfaceFlipY)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t xbegin, DEE_A_IN Dee_size_t ybegin,
 DEE_A_IN Dee_size_t xend, DEE_A_IN Dee_size_t yend);

//////////////////////////////////////////////////////////////////////////
// Blit a mask of bits, only printing pixels with a 1-bit
// NOTE: MSB: Most significant bit first (in data)
// NOTE: LSB: Least significant bit first (in data)
typedef void (DEE_CALL *PDeeSurfacePixelMaskMSB)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t line_bytes, DEE_A_IN_RB(in_bytes) void const *data,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
typedef void (DEE_CALL *PDeeSurfacePixelMaskLSB)(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_size_t x, DEE_A_IN Dee_size_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t line_bytes, DEE_A_IN_RB(in_bytes) void const *data,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);


struct DeeSurfacePixelSpec {
 Dee_uint8_t   st_pixelbits;  /*< Size of a single pixel (in bits). */
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
};
#define DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_MASK(mask,shift,bits)     mask
#define DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_SHIFT(mask,shift,bits)    shift
#define DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS(mask,shift,bits)     bits
#define DEE_PRIVATE_SURFACE_PIXELSPEC_GENMASK DEE_BUILTIN_BESWAP32_M
#ifdef DEE_PLATFORM_LIL_ENDIAN
#define DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(total_bits,shift,bits) (total_bits)-((shift)+(bits))
#else
#define DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(total_bits,shift,bits) bits
#endif
#define DeeSurfacePixelSpec_INIT32(_r,_g,_b,_a) DeeSurfacePixelSpec_INIT(32,_r,_g,_b,_a)
#define DeeSurfacePixelSpec_INIT(bits,_r,_g,_b,_a)\
{/* st_pixelbits  */(bits)\
,/* st_pixelbytes */(bits)/8\
,/* st_rmask      */DEE_PRIVATE_SURFACE_PIXELSPEC_GENMASK(DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_MASK _r) \
,/* st_gmask      */DEE_PRIVATE_SURFACE_PIXELSPEC_GENMASK(DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_MASK _g) \
,/* st_bmask      */DEE_PRIVATE_SURFACE_PIXELSPEC_GENMASK(DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_MASK _b) \
,/* st_amask      */DEE_PRIVATE_SURFACE_PIXELSPEC_GENMASK(DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_MASK _a) \
,/* st_rshift     */DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(bits,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_SHIFT _r,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _r) \
,/* st_gshift     */DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(bits,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_SHIFT _g,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _g) \
,/* st_bshift     */DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(bits,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_SHIFT _b,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _b) \
,/* st_ashift     */DEE_PRIVATE_SURFACE_PIXELSPEC_GENSHIFT(bits,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_SHIFT _a,DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _a) \
,/* st_rbits      */DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _r \
,/* st_gbits      */DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _g \
,/* st_bbits      */DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _b \
,/* st_abits      */DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _a \
,/* st_rmod       */DEE_UINT16_C(1) << DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _r \
,/* st_gmod       */DEE_UINT16_C(1) << DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _g \
,/* st_bmod       */DEE_UINT16_C(1) << DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _b \
,/* st_amod       */DEE_UINT16_C(1) << DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _a \
,/* st_rinvmod    */DEE_UINT16_C(1) << (8-DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _r) \
,/* st_ginvmod    */DEE_UINT16_C(1) << (8-DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _g) \
,/* st_binvmod    */DEE_UINT16_C(1) << (8-DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _b) \
,/* st_ainvmod    */DEE_UINT16_C(1) << (8-DEE_PRIVATE_SURFACE_PIXELSPEC_BLOCK_BITS _a) \
}
extern DEE_A_RET_EXCEPT(-1) int DeeSurfacePixelSpec_Init(
 DEE_A_OUT struct DeeSurfacePixelSpec *self, DEE_A_IN Dee_uint64_t formatid);


struct DeeSurfaceIndexSpec {
 Dee_uint8_t st_indexbits; /*< Amount of index bits / pixel (also the size of a pixel; may not be byte aligned). */
 Dee_size_t  st_mapsize;   /*< Size of the index map (entry count). */
};


struct DeeSurfaceTypeObject {
 DeeTypeObject                   st_base;
 Dee_uint64_t                    st_format;
 DEE_A_REF DeeSurfaceTypeObject *st_next; /*< [0..1] Next entry in the global cache of surface types. */
 union{ /*< DEE_SURFACETYPE_FORMAT_KIND(st_format). */
  struct DeeSurfacePixelSpec st_pixelspec;
  struct DeeSurfaceIndexSpec st_indexspec;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define st_pixelspec _st_spec.st_pixelspec
#define st_indexspec _st_spec.st_indexspec
 _st_spec
#endif
 ;
 PDeeSurfaceGetPixel     st_getpixel;     /*< [1..1]. */
 PDeeSurfaceSetPixel     st_setpixel;     /*< [1..1]. */
 PDeeSurfaceFill         st_fill;         /*< [1..1]. */
 PDeeSurfaceFillRect     st_fillrect;     /*< [1..1]. */
 PDeeSurfaceXLine        st_xline;        /*< [1..1]. */
 PDeeSurfaceYLine        st_yline;        /*< [1..1]. */
 PDeeSurfaceLineLLHH     st_linellhh;     /*< [1..1]. */
 PDeeSurfaceLineLHHL     st_linelhhl;     /*< [1..1]. */
 PDeeSurfaceBlit         st_blit;         /*< [1..1]. */
 PDeeSurfaceStretchBlit  st_stretchblit;  /*< [1..1]. */
 PDeeSurfaceFlipX        st_flipx;        /*< [1..1]. */
 PDeeSurfaceFlipY        st_flipy;        /*< [1..1]. */
 PDeeSurfacePixelMaskMSB st_pixelmaskmsb; /*< [1..1]. */
 PDeeSurfacePixelMaskLSB st_pixelmasklsb; /*< [1..1]. */
};

#define DEEPIXELSURFACE_GENERIC_GETPIXEL(pixelspec,datap,result)\
do{\
 Dee_uint32_t data;\
 switch ((pixelspec)->st_pixelbytes) {\
  case 1:  data = (Dee_uint32_t)*(Dee_uint8_t *)(datap); break;\
  case 2:  data = (Dee_uint32_t)*(Dee_uint16_t *)(datap); break;\
  case 3:  data = DeeUInt24_Get(datap); break;\
  default: data = *(Dee_uint32_t *)(datap); break;\
 }\
 (result)->r = (Dee_uint8_t)(((Dee_uint16_t)(((data)&(pixelspec)->st_rmask) >> (pixelspec)->st_rshift)*256)/(pixelspec)->st_rmod);\
 (result)->g = (Dee_uint8_t)(((Dee_uint16_t)(((data)&(pixelspec)->st_gmask) >> (pixelspec)->st_gshift)*256)/(pixelspec)->st_gmod);\
 (result)->b = (Dee_uint8_t)(((Dee_uint16_t)(((data)&(pixelspec)->st_bmask) >> (pixelspec)->st_bshift)*256)/(pixelspec)->st_bmod);\
 (result)->a = (pixelspec)->st_abits\
             ? (Dee_uint8_t)(((Dee_uint16_t)(((data)&(pixelspec)->st_amask) >> (pixelspec)->st_ashift)*256)/(pixelspec)->st_amod)\
             : (Dee_uint8_t)0xFF;\
}while(0)
#define DEEPIXELSURFACE_GENERIC_SETPIXEL(pixelspec,datap,val)\
do{\
 Dee_uint32_t newdata;\
 newdata = (((Dee_uint32_t)((val)->r/(pixelspec)->st_rinvmod) << (pixelspec)->st_rshift) & (pixelspec)->st_rmask)\
         | (((Dee_uint32_t)((val)->g/(pixelspec)->st_ginvmod) << (pixelspec)->st_gshift) & (pixelspec)->st_gmask)\
         | (((Dee_uint32_t)((val)->b/(pixelspec)->st_binvmod) << (pixelspec)->st_bshift) & (pixelspec)->st_bmask)\
         | (((Dee_uint32_t)((val)->a/(pixelspec)->st_ainvmod) << (pixelspec)->st_ashift) & (pixelspec)->st_amask);\
 switch ((pixelspec)->st_pixelbytes) {\
  case 1: *(Dee_uint8_t *)(datap) = (Dee_uint8_t)newdata; break;\
  case 2: *(Dee_uint16_t *)(datap) = (Dee_uint16_t)newdata; break;\
  case 3: DeeUInt24_Set(datap,newdata); break;\
  default: *(Dee_uint32_t *)(datap) = newdata; break;\
 }\
}while(0)

#define DeeSurfaceType_PIXEL_GETPIXEL(ty,datap,result)\
 DEEPIXELSURFACE_GENERIC_GETPIXEL(&(ty)->st_pixelspec,datap,result)
#define DeeSurfaceType_PIXEL_SETPIXEL(ty,datap,val)\
 DEEPIXELSURFACE_GENERIC_SETPIXEL(&(ty)->st_pixelspec,datap,val)


#define DeeSurfaceType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSurfaceType_Type)
#define DeeSurfaceType_Check          DeeSurfaceType_CheckExact
extern DeeTypeObject DeeSurfaceType_Type;
#define DeeSurfaceType_FORMAT(ob) ((DeeSurfaceTypeObject *)Dee_REQUIRES_POINTER(ob))->st_format
#define DeeSurfaceType_KIND(ob)   DEE_SURFACETYPE_FORMAT_KIND(DeeSurfaceType_FORMAT(ob))

extern DeeSurfaceTypeObject DeeSurface_Type; /*< Base class for all surfaces. */
#if 1
#define DeeSurface_CheckExact(ob) DeeSurfaceType_Check(Dee_TYPE(ob))
#else
#define DeeSurface_CheckExact(ob) DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeSurface_Type)
#endif
#define DeeSurface_Check       DeeSurface_CheckExact


#ifdef DEE_LIMITED_DEX_GFX
//////////////////////////////////////////////////////////////////////////
// Some hard-coded surface types (with special optimizations)
//extern DeeSurfaceTypeObject DeeSurfaceType_RGB888;
extern DeeSurfaceTypeObject DeeSurfaceType_RGBA8888;
#endif


//////////////////////////////////////////////////////////////////////////
// === Surface Object ===
#define DEE_SURFACE_HEAD \
 DEE_OBJECT_HEAD_EX(DeeSurfaceTypeObject)\
 Dee_size_t s_sizex; /*< Surface size in x (aka. width). */\
 Dee_size_t s_sizey; /*< Surface size in y (aka. height). */
struct DeeSurfaceObject {
 DEE_SURFACE_HEAD
 union{
  struct{
   Dee_size_t  s_pixlsize;     /*< Size of a single pixel (== __ob_type->st_pixelspec.st_pixelbytes). */
   Dee_size_t  s_scanline;     /*< Amount of bytes per scanline (== s_sizex*s_pixlsize). */
   Dee_uint8_t s_pixels[4096]; /*< Raw pixeldata (pixeladdr = x*s_pixlsize+y*s_scanline) */
#define DeeSurface_PIXELADDR(self,x,y) \
 (void *)((self)->s_pixeldata.s_pixels+(x)*(self)->s_pixeldata.s_pixlsize+\
                                       (y)*(self)->s_pixeldata.s_scanline)
#define DeeSurface_PIXELBASESIZE Dee_OFFSETOF(DeeSurfaceObject,s_pixeldata.s_pixels)
  } s_pixeldata;
  struct {
#define DeeSurface_INDEXMAP(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_indexdata.s_map
#define DeeSurface_INDEXPIX(ob) ((Dee_uint8_t *)(((Dee_uintptr_t)DeeSurface_INDEXMAP(ob))+DeeSurface_TYPE(ob)->st_indexspec.st_mapsize))
   struct DeePixel s_map[256];     /*< [__ob_type->st_indexspec.st_mapsize] Index map (Actual size depends on surface type). */
   Dee_uint8_t     s_pixels[4096]; /*< Raw pixeldata (pixeladdr = x*s_pixlsize+y*s_scanline) */
  } s_indexdata;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define s_pixeldata _s_data.s_pixeldata
#define s_indexdata _s_data.s_indexdata
 _s_data
#endif
 ;
};

#define DeeSurface_TYPE(ob)  ((DeeSurfaceTypeObject *)Dee_TYPE(ob))
#define DeeSurface_SIZEX(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_sizex
#define DeeSurface_SIZEY(ob) ((DeeSurfaceObject *)Dee_REQUIRES_POINTER(ob))->s_sizey


//////////////////////////////////////////////////////////////////////////
// Create a new surface of a given type and dimensions
#define DeeSurface_NewEmpty(stype,sx,sy) \
 DeeSurface_NewFilled(stype,sx,sy,DeePixel_EMPTY)
#ifdef DEE_LIMITED_DEX_GFX
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_New(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy);
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFromStream(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *fp);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSurface_TryNewFromPNGStream(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *fp,
 DEE_A_REF DEE_A_OUT DeeSurfaceObject **result);
extern DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFromFilename(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *filename);
extern DEE_A_RET_EXCEPT(-1) int DeeSurface_SavePNGStream(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_INOUT DeeObject *fp);
extern DEE_A_RET_EXCEPT(-1) int DeeSurface_SavePNGFile(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_INOUT DeeObject *filename);
#else
#define DeeSurface_New(stype,sx,sy) \
 ((DeeSurfaceObject *)DeeType_NewInstancef((DeeTypeObject *)(stype),"IuIu",sx,sy))
#endif
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeSurfaceObject *) DeeSurface_NewFilled(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx,
 DEE_A_IN Dee_size_t sy, DEE_A_IN struct DeePixel const *color);




//////////////////////////////////////////////////////////////////////////
// Returns a new surface containing the same data as 'self', but converted to 'ty' format
// >> Same as creating a new surface, the size of 'self' and format 'ty',
//    then proceeding to blit 'self' onto that new surface.
// NOTE: Returns 'self' if 'ty' is the format of 'self'
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeSurfaceObject *) DeeSurface_Convert(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_IN DeeSurfaceTypeObject const *ty);


//////////////////////////////////////////////////////////////////////////
// Fills the entire surface with a single color
#define DeeSurface_Fill(dst,color,blend) (*DeeSurface_TYPE(dst)->st_fill)(dst,color,blend)

//////////////////////////////////////////////////////////////////////////
// Get/set the pixel at a given position 'x-y' in 'dst'
// NOTE: When setting the pixel in an index-based surface,
//       the color closest to the given pixel will be chosen.
DEE_STATIC_INLINE(void) DeeSurface_GetPixel(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_OUT struct DeePixel *pixel);
DEE_STATIC_INLINE(void) DeeSurface_SetPixel(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_GetPixel(dst,x,y,pixel) (*DeeSurface_TYPE(dst)->st_getpixel)(dst,x,y,pixel)
#define _DeeSurface_SetPixel(dst,x,y,pixel,blend) (*DeeSurface_TYPE(dst)->st_setpixel)(dst,x,y,pixel,blend)

//////////////////////////////////////////////////////////////////////////
// Flip a given area around the X/Y-axis
DEE_STATIC_INLINE(void) DeeSurface_FlipXRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy);
DEE_STATIC_INLINE(void) DeeSurface_FlipYRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy);
#define DeeSurface_FlipX(dst)\
 (*DeeSurface_TYPE(dst)->st_flipx)(dst,0,0,DeeSurface_SIZEX(dst),DeeSurface_SIZEY(dst))
#define DeeSurface_FlipY(dst)\
 (*DeeSurface_TYPE(dst)->st_flipy)(dst,0,0,DeeSurface_SIZEX(dst),DeeSurface_SIZEY(dst))
#define _DeeSurface_FlipXRect(dst,x,y,sx,sy)\
 (((sx) && (sy)) ? (*DeeSurface_TYPE(dst)->st_flipx)(dst,x,y,x+(sx),y+(sy)) : (void)0)
#define _DeeSurface_FlipYRect(dst,x,y,sx,sy)\
 (((sx) && (sy)) ? (*DeeSurface_TYPE(dst)->st_flipy)(dst,x,y,x+(sx),y+(sy)) : (void)0)

//////////////////////////////////////////////////////////////////////////
// Fill a given rectable with a solid color
DEE_STATIC_INLINE(void) DeeSurface_FillRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_FillRect(dst,xbegin,ybegin,xend,yend,color,blend) \
 (*DeeSurface_TYPE(dst)->st_fillrect)(dst,xbegin,ybegin,xend,yend,color,blend)

//////////////////////////////////////////////////////////////////////////
// Draw Line parallel to the x/y axis
#define _DeeSurface_XLine(dst,xbegin,xend,y,color,blend)\
 (*DeeSurface_TYPE(dst)->st_xline)(dst,xbegin,xend,y,color,blend)
#define _DeeSurface_YLine(dst,x,ybegin,yend,color,blend)\
 (*DeeSurface_TYPE(dst)->st_yline)(dst,x,ybegin,yend,color,blend)
DEE_STATIC_INLINE(void) DeeSurface_XLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x1, DEE_A_IN Dee_ssize_t x2, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);
DEE_STATIC_INLINE(void) DeeSurface_YLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y1, DEE_A_IN Dee_ssize_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);

//////////////////////////////////////////////////////////////////////////
// Draw A regular of line (may be diagonal)
DEE_STATIC_INLINE(void) DeeSurface_Line(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x1, DEE_A_IN Dee_ssize_t y1,
 DEE_A_IN Dee_ssize_t x2, DEE_A_IN Dee_ssize_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);

//////////////////////////////////////////////////////////////////////////
// Draw A box, that is a non-filled rectangle
DEE_STATIC_INLINE(void) DeeSurface_Box(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend);

//////////////////////////////////////////////////////////////////////////
// Draw A filled triangle between 3 points
DEE_STATIC_INLINE(void) DEE_CALL DeeSurface_Triangle(
 DeeSurfaceObject *dst, Dee_ssize_t dx1, Dee_ssize_t dy1,
 /* ................ */ Dee_ssize_t dx2, Dee_ssize_t dy2,
 /* ................ */ Dee_ssize_t dx3, Dee_ssize_t dy3,
 struct DeePixel const *color, Dee_blendinfo_t blend);


//////////////////////////////////////////////////////////////////////////
// Blit a full surface 'src' onto 'dst' at 'dx-dy'
DEE_STATIC_INLINE(void) DeeSurface_Blit(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_Blit(dst,dx,dy,src,blend)\
 (*DeeSurface_TYPE(dst)->st_blit)(dst,dx,dy,src,0,0,\
   DeeSurface_SIZEX(src),DeeSurface_SIZEY(src),blend);


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
 DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_BlitArea(dst,dx,dy,src,sx,sy,ssx,ssy,blend) \
 (*DeeSurface_TYPE(dst)->st_blit)(dst,dx,dy,src,sx,sy,ssx,ssy,blend)


//////////////////////////////////////////////////////////////////////////
// Stretch-blits a given area from 'src' onto 'dst' 'dx-dy'
// NOTE: Nearest-neighbor interpolation is used
DEE_STATIC_INLINE(void) DeeSurface_StretchBlitArea(
 DEE_A_INOUT DeeSurfaceObject    *dst, DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy, DEE_A_IN Dee_size_t dsx, DEE_A_IN Dee_size_t dsy,
 DEE_A_IN DeeSurfaceObject const *src, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy, DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend);
#define _DeeSurface_StretchBlit(dst,dx,dy,dsx,dsy,src,sx,sy,ssx,ssy,blend) \
 (*DeeSurface_TYPE(dst)->st_stretchblit)(dst,dx,dy,dsx,dsy,src,sx,sy,ssx,ssy,blend)


//////////////////////////////////////////////////////////////////////////
// Returns the surface type associated with a given format
#ifdef DEE_LIMITED_DEX_GFX
extern DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
 DeeSurfaceType_Get(DEE_A_IN Dee_uint64_t format);
extern DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
 DeeSurfaceType_New(DEE_A_IN Dee_uint64_t format);
#else
#define DeeSurfaceType_Get(format) DeeDex_Callf("_gfx.__surface_formatid(I64u)",format)
#endif





//////////////////////////////////////////////////////////////////////////
// Implementation of inline functions
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeSurfaceObject *) DeeSurface_NewFilled(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx,
 DEE_A_IN Dee_size_t sy, DEE_A_IN struct DeePixel const *color) {
 DeeSurfaceObject *result;
 if DEE_LIKELY((result = DeeSurface_New(stype,sx,sy)) != NULL) {
  DeeSurface_Fill(result,color,DEE_BLENDINFO_OVERRIDE);
 }
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeSurfaceObject *) DeeSurface_Convert(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_IN DeeSurfaceTypeObject const *ty) {
 DeeSurfaceObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSurface_Check(self));
 DEE_ASSERT(DeeObject_Check(ty) && DeeSurfaceType_Check(ty));
 if (DeeSurface_TYPE(self) == ty) { Dee_INCREF(self); return (DeeSurfaceObject *)self; }
 if DEE_UNLIKELY((result = DeeSurface_New(
  ty,DeeSurface_SIZEX(self),DeeSurface_SIZEY(self))) == NULL) return NULL;
 _DeeSurface_Blit(result,0,0,self,DEE_BLENDINFO_OVERRIDE);
 return result;
}

DEE_STATIC_INLINE(void) DeeSurface_GetPixel(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_OUT struct DeePixel *pixel) {
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x >= 0 && (Dee_size_t)x < DeeSurface_SIZEX(dst) &&
     y >= 0 && (Dee_size_t)y < DeeSurface_SIZEY(dst)) {
  (*DeeSurface_TYPE(dst)->st_getpixel)(dst,(Dee_size_t)x,(Dee_size_t)y,pixel);
 } else {
  *pixel = *DeePixel_EMPTY;
 }
}
DEE_STATIC_INLINE(void) DeeSurface_SetPixel(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend) {
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x >= 0 && (Dee_size_t)x < DeeSurface_SIZEX(dst) &&
     y >= 0 && (Dee_size_t)y < DeeSurface_SIZEY(dst)) {
  (*DeeSurface_TYPE(dst)->st_setpixel)(dst,(Dee_size_t)x,(Dee_size_t)y,pixel,blend);
 }
}

DEE_STATIC_INLINE(void) DeeSurface_XLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x1, DEE_A_IN Dee_ssize_t x2, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_x1,used_x2,used_y,sx;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (y < 0) return;
 used_y = (Dee_size_t)y;
 if (used_y >= DeeSurface_SIZEY(dst)) return;
 if (x1 < x2) {
  if (x1 < 0) used_x1 = 0; else used_x1 = (Dee_size_t)x1;
  if (x2 < 0 || used_x1 > (sx = DeeSurface_SIZEX(dst))) return;
  used_x2 = (Dee_size_t)x2;
  if (used_x2 > sx) used_x2 = sx;
  DEE_ASSERT(used_x2 >= used_x1);
  _DeeSurface_XLine(dst,used_x1,used_x2,used_y,color,blend);
 } else if (x1 > x2) {
  if (x2 < 0) used_x1 = 0; else used_x1 = (Dee_size_t)x2;
  if (x1 < 0 || used_x1 > (sx = DeeSurface_SIZEX(dst))) return;
  used_x2 = (Dee_size_t)x1;
  if (used_x2 > sx) used_x2 = sx;
  DEE_ASSERT(used_x2 >= used_x1);
  _DeeSurface_XLine(dst,used_x1,used_x2,used_y,color,blend);
 }
}

DEE_STATIC_INLINE(void) DeeSurface_YLine(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y1, DEE_A_IN Dee_ssize_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_y1,used_y2,used_x,sy;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if (x < 0) return;
 used_x = (Dee_size_t)x;
 if (used_x >= DeeSurface_SIZEY(dst)) return;
 if (y1 < y2) {
  if (y1 < 0) used_y1 = 0; else used_y1 = (Dee_size_t)y1;
  if (y2 < 0 || used_y1 > (sy = DeeSurface_SIZEX(dst))) return;
  used_y2 = (Dee_size_t)y2;
  if (used_y2 > sy) used_y2 = sy;
  DEE_ASSERT(used_y2 >= used_y1);
  _DeeSurface_YLine(dst,used_x,used_y1,used_y2,color,blend);
 } else if (y1 > y2) {
  if (y2 < 0) used_y1 = 0; else used_y1 = (Dee_size_t)y2;
  if (y1 < 0 || used_y1 > (sy = DeeSurface_SIZEX(dst))) return;
  used_y2 = (Dee_size_t)y1;
  if (used_y2 > sy) used_y2 = sy;
  DEE_ASSERT(used_y2 >= used_y1);
  _DeeSurface_YLine(dst,used_x,used_y1,used_y2,color,blend);
 }
}


DEE_STATIC_INLINE(void) DeeSurface_Line(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x1, DEE_A_IN Dee_ssize_t y1,
 DEE_A_IN Dee_ssize_t x2, DEE_A_IN Dee_ssize_t y2,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 // >> Cohen�Sutherland algorithm
 // https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 Dee_ssize_t xmax,ymax,temp,x,y;
 int outcode0,outcode1,outcodeOut;
#define COHSUTH_INSIDE 0 // 0000
#define COHSUTH_XMIN   1 // 0001
#define COHSUTH_XMAX   2 // 0010
#define COHSUTH_YMIN   4 // 0100
#define COHSUTH_YMAX   8 // 1000
 xmax = (Dee_ssize_t)(DeeSurface_SIZEX(dst)-1);
 ymax = (Dee_ssize_t)(DeeSurface_SIZEY(dst)-1);
#define COHSUTH_COMPUTEOUTCODE(x,y,result)\
do{\
 (result) = COHSUTH_INSIDE;\
 if ((x) < 0) (result) |= COHSUTH_XMIN;\
 else if ((x) > xmax) (result) |= COHSUTH_XMAX;\
 if ((y) < 0) (result) |= COHSUTH_YMIN;\
 else if ((y) > ymax) (result) |= COHSUTH_YMAX;\
}while(0)
 COHSUTH_COMPUTEOUTCODE(x1,y1,outcode0);
 COHSUTH_COMPUTEOUTCODE(x2,y2,outcode1);
 while ((outcode0 | outcode1) != 0) {
  if ((outcode0 & outcode1) != 0) return;
  outcodeOut = outcode0 ? outcode0 : outcode1;
         if ((outcodeOut & COHSUTH_YMAX) != 0)   x = x1+(x2-x1)*(ymax-y1)/(y2-y1),y = ymax;
  else   if ((outcodeOut & COHSUTH_YMIN) != 0)   x = x1+(x2-x1)*(0-y1)/(y2-y1),y = 0;
  else   if ((outcodeOut & COHSUTH_XMAX) != 0)   y = y1+(y2-y1)*(xmax-x1)/(x2-x1),x = xmax;
  else /*if ((outcodeOut & COHSUTH_XMIN) != 0)*/ y = y1+(y2-y1)*(0-x1)/(x2-x1),x = 0;
  if (outcodeOut == outcode0) { x1 = x,y1 = y; COHSUTH_COMPUTEOUTCODE(x1,y1,outcode0); }
  else                        { x2 = x,y2 = y; COHSUTH_COMPUTEOUTCODE(x2,y2,outcode1); }
 }
 // Coords are clamped! --> Now select the proper line algorithm
 if (x1 > x2) {
  temp = x2,x2 = x1,x1 = temp;
  temp = y2,y2 = y1,y1 = temp;
 } else if (x2 == x1) {
  if (y1 > y2) temp = y2,y2 = y1,y1 = temp;
  else if (y2 == y1) return;
  (*DeeSurface_TYPE(dst)->st_yline)(dst,(Dee_size_t)x1,(Dee_size_t)y1,
                                    (Dee_size_t)y2,color,blend);
  return;
 }
 if (y2 > y1) {
  (*DeeSurface_TYPE(dst)->st_linellhh)(dst,(Dee_size_t)x1,(Dee_size_t)y1,
                                       (Dee_size_t)(x2-x1)+1,
                                       (Dee_size_t)(y2-y1)+1,
                                       color,blend);
 } else if (y2 < y1) {
  (*DeeSurface_TYPE(dst)->st_linelhhl)(dst,(Dee_size_t)x1,(Dee_size_t)y1,
                                       (Dee_size_t)(x2-x1)+1,
                                       (Dee_size_t)(y1-y2)+1,
                                       color,blend);
 } else if (x1 != x2) {
  (*DeeSurface_TYPE(dst)->st_xline)(dst,(Dee_size_t)x1,(Dee_size_t)x2,
                                    (Dee_size_t)y1,color,blend);
 }
#undef COHSUTH_COMPUTEOUTCODE
#undef COHSUTH_INSIDE
#undef COHSUTH_XMIN
#undef COHSUTH_XMAX
#undef COHSUTH_YMIN
#undef COHSUTH_YMAX
}

DEE_STATIC_INLINE(void) DeeSurface_Box(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t fixed_x,fixed_y,fixed_sx,fixed_sy,dsx,dsy,offset; int features;
#define SURFACEBOX_HAVE_LINE_XMIN 0x1
#define SURFACEBOX_HAVE_LINE_XMAX 0x2
#define SURFACEBOX_HAVE_LINE_YMIN 0x4
#define SURFACEBOX_HAVE_LINE_YMAX 0x8
 dsx = DeeSurface_SIZEX(dst); if DEE_UNLIKELY(x >= 0 && (Dee_size_t)x >= dsx) return;
 dsy = DeeSurface_SIZEY(dst); if DEE_UNLIKELY(y >= 0 && (Dee_size_t)y >= dsy) return;
 features = 0xF;
 if (x < 0) { if (sx <= (offset = (Dee_size_t)(-x))) return; fixed_x = 0,fixed_sx = sx-offset; features &= ~(SURFACEBOX_HAVE_LINE_XMIN); }
 else { fixed_x = (Dee_size_t)x; fixed_sx = (fixed_x+sx > dsx) ? (features &= ~(SURFACEBOX_HAVE_LINE_XMAX),dsx-fixed_x) : sx; }
 if (y < 0) { if (sy <= (offset = (Dee_size_t)(-y))) return; fixed_y = 0,fixed_sy = sy-offset; features &= ~(SURFACEBOX_HAVE_LINE_YMIN); }
 else { fixed_y = (Dee_size_t)y; fixed_sy = (fixed_y+sy > dsy) ? (features &= ~(SURFACEBOX_HAVE_LINE_YMAX),dsy-fixed_y) : sy; }
 if DEE_UNLIKELY(!fixed_sx || !fixed_sy) return;
 // NOTE: Leave out the ending two pixels on y-lines
 // >> The reverse would produce the same results,
 //    but for most surface types this way is faster
 if ((features&SURFACEBOX_HAVE_LINE_YMIN)!=0) _DeeSurface_XLine(dst,fixed_x,fixed_x+fixed_sx,fixed_y,             color,blend); // YMin line
 if ((features&SURFACEBOX_HAVE_LINE_YMAX)!=0) _DeeSurface_XLine(dst,fixed_x,fixed_x+fixed_sx,fixed_y+(fixed_sy-1),color,blend); // YMax line
 if (fixed_sy >= 2 && fixed_y+1 != dsy) {
  if ((features&SURFACEBOX_HAVE_LINE_XMIN)!=0) _DeeSurface_YLine(dst,fixed_x,             fixed_y+1,fixed_y+(fixed_sy-1),color,blend); // XMin line
  if ((features&SURFACEBOX_HAVE_LINE_XMAX)!=0) _DeeSurface_YLine(dst,fixed_x+(fixed_sx-1),fixed_y+1,fixed_y+(fixed_sy-1),color,blend); // XMax line
 }
}



DEE_STATIC_INLINE(void) DeeSurface_Blit(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy,
 DEE_A_IN DeeSurfaceObject const *src, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_dx,used_dy,used_sx,used_sy,used_ssx,used_ssy,off;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 used_ssx = DeeSurface_SIZEX(src),used_ssy = DeeSurface_SIZEY(src);
 if (dx >= 0) used_dx = (Dee_size_t)dx,used_sx = 0; else { if ((off = (Dee_size_t)(-dx)) >= used_ssx) return; used_dx = 0; used_sx = off; used_ssx -= off; }
 if (dy >= 0) used_dy = (Dee_size_t)dy,used_sy = 0; else { if ((off = (Dee_size_t)(-dy)) >= used_ssy) return; used_dy = 0; used_sy = off; used_ssy -= off; }
 if (used_dx >= DeeSurface_SIZEX(dst)) return;
 if (used_dy >= DeeSurface_SIZEY(dst)) return;
 (*DeeSurface_TYPE(dst)->st_blit)(dst,used_dx,used_dy,
                                  src,used_sx,used_sy,
                                  used_ssx,used_ssy,blend);
}
DEE_STATIC_INLINE(void) DeeSurface_BlitArea(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy,
 DEE_A_IN DeeSurfaceObject const *src,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_dx,used_dy,used_sx,used_sy,used_ssx,used_ssy;
 Dee_size_t dstsx,dstsy,srcsx,srcsy,maxblitsx,maxblitsy,off;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 if (dx >= 0) used_dx = (Dee_size_t)dx,used_sx = sx,used_ssx = ssx; else { if ((off = (Dee_size_t)(-dx)) >= ssx) return; used_dx = 0; used_sx = sx+off; used_ssx = ssx-off; }
 if (dy >= 0) used_dy = (Dee_size_t)dy,used_sy = sy,used_ssy = ssy; else { if ((off = (Dee_size_t)(-dy)) >= ssy) return; used_dy = 0; used_sy = sy+off; used_ssy = ssy-off; }
 if (used_dx >= (dstsx = DeeSurface_SIZEX(dst))) return;
 if (used_dy >= (dstsy = DeeSurface_SIZEY(dst))) return;
 if (used_sx >= (srcsx = DeeSurface_SIZEX(src))) return;
 if (used_sy >= (srcsy = DeeSurface_SIZEY(src))) return;
 dstsx -= used_dx,dstsy -= used_dy;
 srcsx -= used_sx,srcsy -= used_sy;
 maxblitsx = Dee_MIN(dstsx,srcsx);
 maxblitsy = Dee_MIN(dstsy,srcsy);
 (*DeeSurface_TYPE(dst)->st_blit)(dst,used_dx,used_dy,
                                  src,used_sx,used_sy,
                                  Dee_MIN(used_ssx,maxblitsx),
                                  Dee_MIN(used_ssy,maxblitsy),
                                  blend);
}

DEE_STATIC_INLINE(void) DeeSurface_StretchBlitArea(
 DEE_A_INOUT DeeSurfaceObject    *dst, DEE_A_IN Dee_ssize_t dx, DEE_A_IN Dee_ssize_t dy, DEE_A_IN Dee_size_t dsx, DEE_A_IN Dee_size_t dsy,
 DEE_A_IN DeeSurfaceObject const *src, DEE_A_IN  Dee_size_t sx, DEE_A_IN  Dee_size_t sy, DEE_A_IN Dee_size_t ssx, DEE_A_IN Dee_size_t ssy,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_dx,used_dy,dstsx,dstsy,used_dsx,used_dsy,off;
 Dee_sizefloat_t srcsx,srcsy,used_sx,used_sy,used_ssx,used_ssy,soff;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(DeeObject_Check(src) && DeeSurface_Check(src));
 used_dsx = dsx,used_dsy = dsy,used_ssx = ssx,used_ssy = ssy;
 if (dx >= 0) used_dx = (Dee_size_t)dx,used_sx = sx; else { if ((off = (Dee_size_t)(-dx)) >= dsx) return; soff = (Dee_sizefloat_t)(off*ssx)/(Dee_sizefloat_t)dsx; used_dx = 0; used_sx = (Dee_sizefloat_t)sx+soff; used_ssx -= soff; used_dsx -= off; }
 if (dy >= 0) used_dy = (Dee_size_t)dy,used_sy = sy; else { if ((off = (Dee_size_t)(-dy)) >= dsy) return; soff = (Dee_sizefloat_t)(off*ssy)/(Dee_sizefloat_t)dsy; used_dy = 0; used_sy = (Dee_sizefloat_t)sy+soff; used_ssy -= soff; used_dsy -= off; }
 if (used_dx >= (dstsx = DeeSurface_SIZEX(dst))) return;
 if (used_dy >= (dstsy = DeeSurface_SIZEY(dst))) return;
 if (used_sx >= (srcsx = DeeSurface_SIZEX(src))) return;
 if (used_sy >= (srcsy = DeeSurface_SIZEY(src))) return;
 dstsx -= used_dx,dstsy -= used_dy,srcsx -= used_sx,srcsy -= used_sy;
 if (used_dsx > dstsx) { used_ssx -= (Dee_sizefloat_t)((used_dsx-dstsx)*used_ssx)/used_dsx; used_dsx = dstsx; }
 if (used_dsy > dstsy) { used_ssy -= (Dee_sizefloat_t)((used_dsy-dstsy)*used_ssy)/used_dsy; used_dsy = dstsy; }
 if (used_ssx > srcsx) { used_dsx = (Dee_size_t)(((Dee_sizefloat_t)used_dsx*(used_ssx-srcsx))/used_ssx); used_ssx = srcsx; }
 if (used_ssy > srcsy) { used_dsy = (Dee_size_t)(((Dee_sizefloat_t)used_dsy*(used_ssy-srcsy))/used_ssy); used_ssy = srcsy; }
 if (used_dsx && used_dsy && used_ssx && used_ssy) {
  (*DeeSurface_TYPE(dst)->st_stretchblit)(dst,used_dx,used_dy,used_dsx,used_dsy,
                                          src,used_sx,used_sy,used_ssx,used_ssy,
                                          blend);
 }
}


DEE_STATIC_INLINE(void) DeeSurface_FlipXRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy) {
 Dee_size_t used_x,used_y,used_sx,used_sy,sizex,sizey,off;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if DEE_UNLIKELY(!sx || !sy) return;
 if DEE_LIKELY(x >= 0) used_x = (Dee_size_t)x,used_sx = sx; else { if DEE_UNLIKELY((off = (Dee_size_t)(-x)) >= sx) return; used_x = 0; used_sx = sx-off; }
 if DEE_LIKELY(y >= 0) used_y = (Dee_size_t)y,used_sy = sy; else { if DEE_UNLIKELY((off = (Dee_size_t)(-y)) >= sy) return; used_y = 0; used_sy = sy-off; }
 if DEE_UNLIKELY(used_x+used_sx >= (sizex = DeeSurface_SIZEX(dst))) { if (used_x >= sizex) return; used_sx = sizex-used_x; }
 if DEE_UNLIKELY(used_x+used_sy >= (sizey = DeeSurface_SIZEY(dst))) { if (used_y >= sizey) return; used_sy = sizey-used_y; }
 (*DeeSurface_TYPE(dst)->st_flipx)(dst,used_x,used_y,used_x+sizex,used_y+sizey);
}
DEE_STATIC_INLINE(void) DeeSurface_FlipYRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy) {
 Dee_size_t used_x,used_y,used_sx,used_sy,sizex,sizey,off;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if DEE_UNLIKELY(!sx || !sy) return;
 if DEE_LIKELY(x >= 0) used_x = (Dee_size_t)x,used_sx = sx; else { if DEE_UNLIKELY((off = (Dee_size_t)(-x)) >= sx) return; used_x = 0; used_sx = sx-off; }
 if DEE_LIKELY(y >= 0) used_y = (Dee_size_t)y,used_sy = sy; else { if DEE_UNLIKELY((off = (Dee_size_t)(-y)) >= sy) return; used_y = 0; used_sy = sy-off; }
 if DEE_UNLIKELY(used_x+used_sx >= (sizex = DeeSurface_SIZEX(dst))) { if (used_x >= sizex) return; used_sx = sizex-used_x; }
 if DEE_UNLIKELY(used_x+used_sy >= (sizey = DeeSurface_SIZEY(dst))) { if (used_y >= sizey) return; used_sy = sizey-used_y; }
 (*DeeSurface_TYPE(dst)->st_flipy)(dst,used_x,used_y,used_x+sizex,used_y+sizey);
}

DEE_STATIC_INLINE(void) DeeSurface_FillRect(
 DEE_A_INOUT DeeSurfaceObject *dst,
 DEE_A_IN Dee_ssize_t x, DEE_A_IN Dee_ssize_t y,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy,
 DEE_A_IN struct DeePixel const *color,
 DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t used_x,used_y,used_sx,used_sy,sizex,sizey,off;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 if DEE_UNLIKELY(!sx || !sy) return;
 if DEE_LIKELY(x >= 0) used_x = (Dee_size_t)x,used_sx = sx; else { if DEE_UNLIKELY((off = (Dee_size_t)(-x)) >= sx) return; used_x = 0; used_sx = sx-off; }
 if DEE_LIKELY(y >= 0) used_y = (Dee_size_t)y,used_sy = sy; else { if DEE_UNLIKELY((off = (Dee_size_t)(-y)) >= sy) return; used_y = 0; used_sy = sy-off; }
 if DEE_UNLIKELY(used_x+used_sx >= (sizex = DeeSurface_SIZEX(dst))) { if (used_x >= sizex) return; used_sx = sizex-used_x; }
 if DEE_UNLIKELY(used_x+used_sy >= (sizey = DeeSurface_SIZEY(dst))) { if (used_y >= sizey) return; used_sy = sizey-used_y; }
 if (used_x == 0 && used_y == 0 && sx == sizex && sy == sizex) {
  (*DeeSurface_TYPE(dst)->st_fill)(dst,color,blend);
 } else {
  (*DeeSurface_TYPE(dst)->st_fillrect)(dst,used_x,used_y,used_x+sizex,used_y+sizey,color,blend);
 }
}


DEE_STATIC_INLINE(void) DEE_CALL DeeSurface_Triangle(
 DeeSurfaceObject *dst, Dee_ssize_t dx1, Dee_ssize_t dy1,
 /* ................ */ Dee_ssize_t dx2, Dee_ssize_t dy2,
 /* ................ */ Dee_ssize_t dx3, Dee_ssize_t dy3,
 struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_ssize_t temp,y,ymax,xbegin,xend,sx,sy;
 Dee_sizefloat_t dx_far,dx_upper,dx_low,xff,xtt;
 PDeeSurfaceXLine xline;
#define swap(a,b) (temp = (a),(a) = (b),(b) = temp)
 if (dy2 > dy3) swap(dx2,dx3),swap(dy2,dy3);
 if (dy1 > dy2) swap(dx1,dx2),swap(dy1,dy2);
 if (dy2 > dy3) swap(dx2,dx3),swap(dy2,dy3);
#undef swap
 dx_far = (Dee_sizefloat_t)(dx3-dx1)/(dy3-dy1+1);
 dx_upper = (Dee_sizefloat_t)(dx2-dx1)/(dy2-dy1+1);
 dx_low = (Dee_sizefloat_t)(dx3-dx2)/(dy3-dy2+1);
 xff = dx1,xtt = dx1+dx_upper;
 sx = (Dee_ssize_t)dst->s_sizex,sy = (Dee_ssize_t)dst->s_sizey;
 xline = DeeSurface_TYPE(dst)->st_xline;
 ymax = (dy3 >= sy ? sy-1 : dy3);
 for (y = dy1; y <= ymax; ++y) {
  if (y >= 0) {
   xbegin = (xff > 0 ? (Dee_ssize_t)xff : 0);
   xend = (xtt < sx ? (Dee_ssize_t)xtt+1 : sx);
   if (xbegin < xend) (*xline)(dst,(Dee_size_t)xbegin,(Dee_size_t)xend,(Dee_size_t)y,color,blend);
   xbegin = (xtt > 0 ? (Dee_ssize_t)xtt : 0);
   xend = (xff < sx ? (Dee_ssize_t)(xff+1) : sx);
   if (xbegin < xend) (*xline)(dst,(Dee_size_t)xbegin,(Dee_size_t)xend,(Dee_size_t)y,color,blend);
  }
  xff += dx_far;
  xtt += (y < dy2) ? dx_upper : dx_low;
 }
}






#ifdef DEE_LIMITED_DEX_GFX
extern void DEE_CALL _deesurface_generic_st_fill(DeeSurfaceObject *dst, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_fillrect(DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend, Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_xline(DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t xend, Dee_size_t y, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_yline(DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t ybegin, Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_linellhh(DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex, Dee_size_t sizey, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_linelhhl(DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex, Dee_size_t sizey, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_blit(DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y, DeeSurfaceObject const *src, Dee_size_t src_x, Dee_size_t src_y, Dee_size_t sx, Dee_size_t sy, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_stretchblit(DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y, Dee_size_t dstsx, Dee_size_t dstsy, DeeSurfaceObject const *src, Dee_sizefloat_t src_x, Dee_sizefloat_t src_y, Dee_sizefloat_t srcsx, Dee_sizefloat_t srcsy, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_flipx(DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend, Dee_size_t yend);
extern void DEE_CALL _deesurface_generic_st_flipy(DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend, Dee_size_t yend);
extern void DEE_CALL _deesurface_generic_st_pixelmaskmsb(DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sx, Dee_size_t sy, Dee_size_t line_bytes, void const *data, struct DeePixel const *color, Dee_blendinfo_t blend);
extern void DEE_CALL _deesurface_generic_st_pixelmasklsb(DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sx, Dee_size_t sy, Dee_size_t line_bytes, void const *data, struct DeePixel const *color, Dee_blendinfo_t blend);
#endif /* DEE_LIMITED_DEX_GFX */


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX__GFX_H */
