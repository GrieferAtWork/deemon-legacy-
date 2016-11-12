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
#ifndef GUARD_DEEMON_DEX_GFX__GFX_C
#define GUARD_DEEMON_DEX_GFX__GFX_C 1
#define DEE_EXTENSION 1

//@ dex.name = _gfx

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4820 4350)
#include "lodepng/lodepng.h"
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include "_gfx.h"
#include <deemon/dex.h>
#include <deemon/bool.h>
#include <deemon/none.h>
#include <deemon/error.h>
#include <deemon/memberdef.h>
#include <deemon/tuple.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/__bswap_intrin.inl>
#include DEE_INCLUDE_MEMORY_API()
#include "pixelobject.inl"

DEE_DECL_BEGIN


void DeePixel_FillStandardIndexMap(
 DEE_A_OUT_W(size) struct DeePixel *map, DEE_A_IN Dee_size_t size) {
 memset(map,0,size*sizeof(struct DeePixel)); // TODO
}


#if 0 /*< Define as 1 to have the blender round upwards. */
#define div256(x) ((x)+0xfe)/0xff
#else
#define div256(x) (x)/0xff
#endif

#define blend_zero(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                                       /* lhs* */0
#define blend_one(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                                           lhs
#define blend_src_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                div256((Dee_uint32_t)lhs*csrc)
#define blend_one_minus_src_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)      div256((Dee_uint32_t)lhs*(0xFF-csrc))
#define blend_dst_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                div256((Dee_uint32_t)lhs*cdst)
#define blend_one_minus_dst_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)      div256((Dee_uint32_t)lhs*(0xFF-cdst))
#define blend_src_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                div256((Dee_uint32_t)lhs*asrc)
#define blend_one_minus_src_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)      div256((Dee_uint32_t)lhs*(0xFF-asrc))
#define blend_dst_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)                div256((Dee_uint32_t)lhs*adst)
#define blend_one_minus_dst_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)      div256((Dee_uint32_t)lhs*(0xFF-adst))
#define blend_src_alpha_saturate(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)       div256((Dee_uint32_t)lhs*Dee_MIN(asrc,0xFF-adst))
#define blend_constant_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)           div256((Dee_uint32_t)lhs*cc)
#define blend_one_minus_constant_color(lhs,rhs,csrc,cdst,asrc,adst,cc,ac) div256((Dee_uint32_t)lhs*(0xFF-cc))
#define blend_constant_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac)           div256((Dee_uint32_t)lhs*ac)
#define blend_one_minus_constant_alpha(lhs,rhs,csrc,cdst,asrc,adst,cc,ac) div256((Dee_uint32_t)lhs*(0xFF-ac))

#define BLEND_SWITCH(kind,callback)\
do{\
 switch (kind) {\
  case DEE_BLENDMODE_ONE                     : callback(blend_one); break;\
  case DEE_BLENDMODE_SRC_COLOR               : callback(blend_src_color); break;\
  case DEE_BLENDMODE_ONE_MINUS_SRC_COLOR     : callback(blend_one_minus_src_color); break;\
  case DEE_BLENDMODE_DST_COLOR               : callback(blend_dst_color); break;\
  case DEE_BLENDMODE_ONE_MINUS_DST_COLOR     : callback(blend_one_minus_dst_color); break;\
  case DEE_BLENDMODE_SRC_ALPHA               : callback(blend_src_alpha); break;\
  case DEE_BLENDMODE_ONE_MINUS_SRC_ALPHA     : callback(blend_one_minus_src_alpha); break;\
  case DEE_BLENDMODE_DST_ALPHA               : callback(blend_dst_alpha); break;\
  case DEE_BLENDMODE_ONE_MINUS_DST_ALPHA     : callback(blend_one_minus_dst_alpha); break;\
  case DEE_BLENDMODE_SRC_ALPHA_SATURATE      : callback(blend_src_alpha_saturate); break;\
  case DEE_BLENDMODE_CONSTANT_COLOR          : callback(blend_constant_color); break;\
  case DEE_BLENDMODE_ONE_MINUS_CONSTANT_COLOR: callback(blend_one_minus_constant_color); break;\
  case DEE_BLENDMODE_CONSTANT_ALPHA          : callback(blend_constant_alpha); break;\
  case DEE_BLENDMODE_ONE_MINUS_CONSTANT_ALPHA: callback(blend_one_minus_constant_alpha); break;\
  default                                    : callback(blend_zero); break;\
 }\
}while(0)

#define func_add(a,b)              a+b
#define func_subtract(a,b)         a-b
#define func_reverse_subtract(a,b) b-a
#define FUNC_SWITCH(kind,callback)\
do{\
 switch (kind) {\
  case DEE_BLENDFUNC_SUBTRACT        : callback(func_subtract); break;\
  case DEE_BLENDFUNC_REVERSE_SUBTRACT: callback(func_reverse_subtract); break;\
  case DEE_BLENDFUNC_MIN             : callback(Dee_MIN); break;\
  case DEE_BLENDFUNC_MAX             : callback(Dee_MAX); break;\
  default                            : callback(func_add); break;\
 }\
}while(0)

void DeePixel_Blend(
 DEE_A_INOUT struct DeePixel *dst,
 DEE_A_IN struct DeePixel const *src,
 DEE_A_IN Dee_blendinfo_t info) {
 struct DeePixel new_lhs,new_rhs;
#define blend_rgb_src(fun)\
{ new_lhs.r = (Dee_uint8_t)(fun(src->r,dst->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info))); \
  new_lhs.g = (Dee_uint8_t)(fun(src->g,dst->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info))); \
  new_lhs.b = (Dee_uint8_t)(fun(src->b,dst->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_a_src(fun)\
{ new_lhs.a = (Dee_uint8_t)(fun(src->a,dst->a,src->a,dst->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_rgb_dst(fun)\
{ new_rhs.r = (Dee_uint8_t)(fun(dst->r,src->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info))); \
  new_rhs.g = (Dee_uint8_t)(fun(dst->g,src->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info))); \
  new_rhs.b = (Dee_uint8_t)(fun(dst->b,src->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_a_dst(fun)\
{ new_rhs.a = (Dee_uint8_t)(fun(dst->a,src->a,src->a,dst->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info))); }
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBSRC(info),blend_rgb_src);
 BLEND_SWITCH(DEE_BLENDINFO_GET_ASRC(info),blend_a_src);
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBDST(info),blend_rgb_dst);
 BLEND_SWITCH(DEE_BLENDINFO_GET_ADST(info),blend_a_dst);
#undef blend_a_dst
#undef blend_rgb_dst
#undef blend_a_src
#undef blend_rgb_src
#define func_rgb(fun)\
{ dst->r = (Dee_uint8_t)(fun(new_lhs.r,new_rhs.r)); \
  dst->g = (Dee_uint8_t)(fun(new_lhs.g,new_rhs.g)); \
  dst->b = (Dee_uint8_t)(fun(new_lhs.b,new_rhs.b)); }
#define func_a(fun)\
{ dst->a = (Dee_uint8_t)(fun(new_lhs.a,new_rhs.a)); }
 FUNC_SWITCH(DEE_BLENDINFO_GET_RGBFUN(info),func_rgb);
 FUNC_SWITCH(DEE_BLENDINFO_GET_AFUN(info),func_a);
#undef func_a
#undef func_rgb
}
#undef FUNC_SWITCH
#undef BLEND_SWITCH
#undef func_add
#undef func_subtract
#undef func_reverse_subtract
#undef blend_zero
#undef blend_one
#undef blend_src_color
#undef blend_one_minus_src_color
#undef blend_dst_color
#undef blend_one_minus_dst_color
#undef blend_src_alpha
#undef blend_one_minus_src_alpha
#undef blend_dst_alpha
#undef blend_one_minus_dst_alpha
#undef blend_src_alpha_saturate
#undef blend_constant_color
#undef blend_one_minus_constant_color
#undef blend_constant_alpha
#undef blend_one_minus_constant_alpha



DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFilled(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sx,
 DEE_A_IN Dee_size_t sy, DEE_A_IN struct DeePixel const *color) {
 DeeSurfaceObject *result;
 if DEE_LIKELY((result = DeeSurface_New(stype,sx,sy)) != NULL) {
  DeeSurface_Fill(result,color,DEE_BLENDINFO_OVERRIDE);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_Convert(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_IN DeeSurfaceTypeObject const *ty) {
 DeeSurfaceObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSurface_Check(self));
 DEE_ASSERT(DeeObject_Check(ty) && DeeSurfaceType_Check(ty));
 if (DeeSurface_TYPE(self) == ty) {
  Dee_INCREF(self);
  return (DeeSurfaceObject *)self;
 }
 if DEE_UNLIKELY((result = DeeSurface_New(
  ty,DeeSurface_SIZEX(self),DeeSurface_SIZEY(self))) == NULL) return NULL;
 DeeSurface_Blit(result,0,0,self,DEE_BLENDINFO_OVERRIDE);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_New(
 DEE_A_IN DeeSurfaceTypeObject const *stype,
 DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy) {
 DeeSurfaceObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(stype) && DeeSurfaceType_Check(stype));
 // Don't allow empty surfaces (silently extend them to 1x1)
 if DEE_UNLIKELY(!sx) sx = 1;
 if DEE_UNLIKELY(!sy) sy = 1;
 switch (DeeSurfaceType_KIND(stype)) {
  case DEE_SURFACETYPE_FORMAT_KIND_PIXEL:
   size = stype->st_pixelspec.st_pixelbytes*sx*sy;
   result = (DeeSurfaceObject *)DeeObject_Malloc(DeeSurface_PIXELBASESIZE+size);
   if DEE_UNLIKELY(!result) return NULL;
   result->s_pixeldata.s_pixlsize = stype->st_pixelspec.st_pixelbytes;
   result->s_pixeldata.s_scanline = result->s_pixeldata.s_pixlsize*sx;
   break;
   // TODO: Index surface
  default: DEE_BUILTIN_UNREACHABLE();
 }
 DeeObject_INIT(result,(DeeTypeObject *)stype);
 result->s_sizex = sx;
 result->s_sizey = sy;
 return result;
}








static struct DeeMemberDef const _deesurface_tp_members[] = {
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("sizex",DeeSurfaceObject,s_sizex,Dee_size_t,"-> size_t"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("sizey",DeeSurfaceObject,s_sizey,Dee_size_t,"-> size_t"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("width",DeeSurfaceObject,s_sizex,Dee_size_t,"-> size_t\tAlias for #sizex"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("height",DeeSurfaceObject,s_sizey,Dee_size_t,"-> size_t\tAlias for #sizey"),
 DEE_MEMBERDEF_END_v100
};


static DeeObject *DEE_CALL _deesurface_line(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t x1,y1,x2,y2; DeePixelObject *color = &DeePixel_BlackObject;
 Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIdId|oI64u:line",&x1,&y1,&x2,&y2,&color,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)color,&DeePixel_Type) != 0) return NULL;
 DeeSurface_Line(dst,x1,y1,x2,y2,&color->p_pixel,blend);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_fill(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t x = 0,y = 0; Dee_size_t sx = (Dee_size_t)-1,sy = (Dee_size_t)-1;
 DeePixelObject *color = &DeePixel_BlackObject; Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|IdIdIuIuoI64u:fill",&x,&y,&sx,&sy,&color,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)color,&DeePixel_Type) != 0) return NULL;
 DeeSurface_FillRect(dst,x,y,sx,sy,&color->p_pixel,blend);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_blit(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t dx,dy; DeeSurfaceObject *src;
 Dee_size_t sx = 0,ssx = (Dee_size_t)-1,sy = 0,ssy = (Dee_size_t)-1;
 Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdo|IuIuIuIuI64u:blit",&dx,&dy,&src,&sx,&sy,&ssx,&ssy,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)src,(DeeTypeObject *)&DeeSurface_Type) != 0) return NULL;
 DeeSurface_BlitArea(dst,dx,dy,src,sx,sy,ssx,ssy,blend);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_stretch(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t dx,dy; Dee_size_t dsx,dsy; DeeSurfaceObject *src; Dee_size_t sx,sy,ssx,ssy;
 DeeObject *sx_ob = NULL,*sy_ob = NULL,*ssx_ob = NULL,*ssy_ob = NULL;
 Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIuIuo|ooooI64u:stretch",
  &dx,&dy,&dsx,&dsy,&src,&sx_ob,&sy_ob,&ssx_ob,&ssy_ob,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)src,(DeeTypeObject *)&DeeSurface_Type) != 0) return NULL;
 if (!sx_ob) sx = 0; else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,sx_ob,&sx) != 0) return NULL;
 if (!sy_ob) sy = 0; else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,sy_ob,&sy) != 0) return NULL;
 if (!ssx_ob) ssx = DeeSurface_SIZEX(src); else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,ssx_ob,&ssx) != 0) return NULL;
 if (!ssy_ob) ssy = DeeSurface_SIZEY(src); else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,ssy_ob,&ssy) != 0) return NULL;
 DeeSurface_StretchBlitArea(dst,dx,dy,dsx,dsy,
                            src,sx,sy,ssx,ssy,blend);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_save_png(
 DeeSurfaceObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 char const *filename; DeeSurfaceObject *rgba8888; unsigned error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"s:save_png",&filename) != 0) return NULL;
 if DEE_UNLIKELY((rgba8888 = DeeSurface_Convert(self,&DeeSurfaceType_RGBA8888)) == NULL) return NULL;
 error = lodepng_encode32_file(filename,rgba8888->s_pixeldata.s_pixels,
                               (unsigned)rgba8888->s_sizex,
                               (unsigned)rgba8888->s_sizey);
 Dee_DECREF(rgba8888);
 if (error != 0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Failed to save png : %d : %s",
                      error,lodepng_error_text(error));
  return NULL;
 }
 DeeReturn_None;
}

static struct DeeMethodDef const _deesurface_tp_methods[] = {
 DEE_METHODDEF_v100("save_png",member(&_deesurface_save_png),"(string filename) -> none"),
 DEE_METHODDEF_v100("line",member(&_deesurface_line),
  "(ssize_t x1, ssize_t y1, ssize_t x2, ssize_t y2, pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none\n"),
 DEE_METHODDEF_v100("fill",member(&_deesurface_fill),
  "(ssize_t x, ssize_t y, size_t sx, size_t sy, pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("blit",member(&_deesurface_blit),
  "(ssize_t dx, ssize_t dy, surface src, size_t sx = 0, size_t sy = 0, size_t ssx = size_t(-1), size_t ssy = size_t(-1), uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("stretch",member(&_deesurface_stretch),
  "(ssize_t dx, ssize_t dy, size_t dsx, size_t dsy, surface src, size_t sx = 0, size_t sy = 0, size_t ssx = src.sizex, size_t ssy = src.sizey, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_END_v100
};


static void DEE_CALL _deenoopsurface_st_getpixel(DeeSurfaceObject const *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), struct DeePixel *pixel) { *pixel = *DeePixel_EMPTY; }
static void DEE_CALL _deenoopsurface_st_setpixel(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), struct DeePixel const *DEE_UNUSED(pixel), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_fill(DeeSurfaceObject *DEE_UNUSED(self), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_fillrect(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), Dee_size_t DEE_UNUSED(sx), Dee_size_t DEE_UNUSED(sy), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_linellhh(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(ybegin_lo), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(yend_hi), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
#define _deenoopsurface_st_linelhhl _deenoopsurface_st_linellhh
static void DEE_CALL _deenoopsurface_st_xline(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(y), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
#define _deenoopsurface_st_yline _deenoopsurface_st_xline
static void DEE_CALL _deenoopsurface_st_blit(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(dst_x), Dee_size_t DEE_UNUSED(dst_y), DeeSurfaceObject const *DEE_UNUSED(src), Dee_size_t DEE_UNUSED(src_x), Dee_size_t DEE_UNUSED(src_y), Dee_size_t DEE_UNUSED(sx), Dee_size_t DEE_UNUSED(sy), Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_stretchblit(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(dst_x), Dee_size_t DEE_UNUSED(dst_y), Dee_size_t DEE_UNUSED(dstsx), Dee_size_t DEE_UNUSED(dstsy), DeeSurfaceObject const *DEE_UNUSED(src), double DEE_UNUSED(src_x), double DEE_UNUSED(src_y), double DEE_UNUSED(srcsx), double DEE_UNUSED(srcsy), Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_flipx(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(ybegin), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(yend)) {}
#define _deenoopsurface_st_flipy _deenoopsurface_st_flipx

DeeTypeObject DeeSurfaceType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("surface_type"),null,null,member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSurfaceTypeObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeSurfaceTypeObject DeeSurface_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(member(&DeeSurfaceType_Type),member("surface"),member(
   "\tBase class for all surface objects (Not constructable)."),null,null),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(Dee_OFFSETOF(
   DeeSurfaceObject,s_pixeldata),null,null,null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deesurface_tp_members),null,
   member(_deesurface_tp_methods),null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 0,    // st_format
 NULL, // st_next
 {{
  0, // st_pixelbits
  0, // st_pixelbytes
  0, // st_rmask
  0, // st_gmask
  0, // st_bmask
  0, // st_amask
  0, // st_rshift
  0, // st_gshift
  0, // st_bshift
  0, // st_ashift
  0, // st_rbits
  0, // st_gbits
  0, // st_bbits
  0, // st_abits
  0, // st_rmod
  0, // st_gmod
  0, // st_bmod
  0, // st_amod
  0, // st_rinvmod
  0, // st_ginvmod
  0, // st_binvmod
  0, // st_ainvmod
 }},
 &_deenoopsurface_st_getpixel,    // st_getpixel
 &_deenoopsurface_st_setpixel,    // st_setpixel
 &_deenoopsurface_st_fill,        // st_fill
 &_deenoopsurface_st_fillrect,    // st_fillrect
 &_deenoopsurface_st_xline,       // st_xline
 &_deenoopsurface_st_yline,       // st_yline
 &_deenoopsurface_st_linellhh,    // st_linellhh
 &_deenoopsurface_st_linelhhl,    // st_linelhhl
 &_deenoopsurface_st_blit,        // st_blit
 &_deenoopsurface_st_stretchblit, // st_stretchblit
 &_deenoopsurface_st_flipx,       // st_flipx
 &_deenoopsurface_st_flipy,       // st_flipy
};


static DeeSurfaceObject *DEE_CALL _deepixelsurface_tp_any_new(
 DeeSurfaceTypeObject *tp_self, DeeObject *args) {
 Dee_size_t sx,sy; DeePixelObject *filler = &DeePixel_EmptyObject;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IuIu|o:surface",&sx,&sy,&filler) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)filler,&DeePixel_Type) != 0) return NULL;
 return DeeSurface_NewFilled(tp_self,sx,sy,&filler->p_pixel);
}
static DeeSurfaceObject *DEE_CALL _deepixelsurface_tp_copy_new(
 DeeSurfaceTypeObject *tp_self, DeeSurfaceObject *self) {
 DeeSurfaceObject *result; Dee_size_t pixeldata;
 pixeldata = self->s_sizey*self->s_pixeldata.s_scanline;
 if DEE_UNLIKELY((result = (DeeSurfaceObject *)DeeObject_Malloc(
  DeeSurface_PIXELBASESIZE+pixeldata)) == NULL) return NULL;;
 DeeObject_INIT(result,(DeeTypeObject *)tp_self);
 result->s_sizex = self->s_sizex;
 result->s_sizey = self->s_sizey;
 result->s_pixeldata.s_pixlsize = self->s_pixeldata.s_pixlsize;
 result->s_pixeldata.s_scanline = self->s_pixeldata.s_scanline;
 DEE_ASSERT(result->s_pixeldata.s_pixlsize == tp_self->st_pixelspec.st_pixelbytes);
 DEE_ASSERT(result->s_pixeldata.s_scanline == result->s_sizex*result->s_pixeldata.s_pixlsize);
 memcpy(result->s_pixeldata.s_pixels,self->s_pixeldata.s_pixels,pixeldata);
 return result;
}


static void DEE_CALL _deesurface_pixel_st_getpixel(
 DeeSurfaceObject const *dst, Dee_size_t x, Dee_size_t y, struct DeePixel *pixel) {
 void *datap; DeeSurfaceTypeObject *dst_type;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 datap = DeeSurface_PIXELADDR(dst,x,y);
 dst_type = DeeSurface_TYPE(dst);
 DeeSurfaceType_PIXEL_GETPIXEL(dst_type,datap,pixel);
}
static void DEE_CALL _deesurface_pixel_st_setpixel(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend) {
 void *datap; struct DeePixel pix;
 DeeSurfaceTypeObject *dst_type;
 DEE_ASSERT(DeeObject_Check(dst) && DeeSurface_Check(dst));
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 datap = DeeSurface_PIXELADDR(dst,x,y);
 dst_type = DeeSurface_TYPE(dst);
 DeeSurfaceType_PIXEL_GETPIXEL(dst_type,datap,&pix);
 DeePixel_Blend(&pix,pixel,blend);
 DeeSurfaceType_PIXEL_SETPIXEL(dst_type,datap,&pix);
}

#define _deesurface_pixel_st_fill        _deesurface_generic_st_fill
#define _deesurface_pixel_st_fillrect    _deesurface_generic_st_fillrect
#define _deesurface_pixel_st_xline       _deesurface_generic_st_xline
#define _deesurface_pixel_st_yline       _deesurface_generic_st_yline
#define _deesurface_pixel_st_linellhh    _deesurface_generic_st_linellhh
#define _deesurface_pixel_st_linelhhl    _deesurface_generic_st_linelhhl
#define _deesurface_pixel_st_blit        _deesurface_generic_st_blit
#define _deesurface_pixel_st_stretchblit _deesurface_generic_st_stretchblit
#define _deesurface_pixel_st_flipx       _deesurface_generic_st_flipx
#define _deesurface_pixel_st_flipy       _deesurface_generic_st_flipy

//////////////////////////////////////////////////////////////////////////
// Generic surface operators (very slow, but work in all situations)
static void DEE_CALL _deesurface_generic_st_fill(
 DeeSurfaceObject *dst, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x,sx,y,sy; PDeeSurfaceSetPixel setter;
 sx = dst->s_sizex,sy = dst->s_sizey;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (y = 0; y < sy; ++y) for (x = 0; x < sx; ++x) {
  (*setter)(dst,x,y,color,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_fillrect(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend,
 Dee_size_t yend, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x,y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend); DEE_ASSERT(ybegin < yend);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (y = ybegin; y < yend; ++y) for (x = xbegin; x < xend; ++x) {
  (*setter)(dst,x,y,color,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_xline(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t xend,
 Dee_size_t y, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend);
 x = xbegin; setter = DeeSurface_TYPE(dst)->st_setpixel;
 do (*setter)(dst,x,y,color,blend);
 while (++x != xend);
}
static void DEE_CALL _deesurface_generic_st_yline(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t beginy,
 Dee_size_t endy, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(beginy < endy);
 y = beginy; setter = DeeSurface_TYPE(dst)->st_setpixel;
 do (*setter)(dst,x,y,color,blend);
 while (++y != endy);
}
static void DEE_CALL _deesurface_generic_st_linellhh(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex,
 Dee_size_t sizey, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 double relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step;
 DEE_ASSERT(sizex != 0);
 DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 step = 0;
 if (sizex > sizey) {
  relation = (double)sizey/(double)sizex;
  do (*setter)(dst,x+step,y+(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (double)sizex/(double)sizey;
  do (*setter)(dst,x+(Dee_size_t)(relation*step),y+step,color,blend);
  while (++step != sizex);
 } else {
  do (*setter)(dst,x+step,y+step,color,blend);
  while (++step != sizex);
 }
}
static void DEE_CALL _deesurface_generic_st_linelhhl(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex,
 Dee_size_t sizey, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 double relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step;
 DEE_ASSERT(sizex != 0),DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 step = 0;
 if (sizex > sizey) {
  relation = (double)sizey/(double)sizex;
  do (*setter)(dst,x+step,y-(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (double)sizex/(double)sizey;
  do (*setter)(dst,x+(Dee_size_t)(relation*step),y-step,color,blend);
  while (++step != sizex);
 } else {
  do (*setter)(dst,x+step,y-step,color,blend);
  while (++step != sizex);
 }
}

static void DEE_CALL _deesurface_generic_st_blit(
 DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y,
 DeeSurfaceObject const *src, Dee_size_t src_x, Dee_size_t src_y,
 Dee_size_t sx, Dee_size_t sy, Dee_blendinfo_t blend) {
 struct DeePixel pixel; Dee_size_t x,y;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(dst_x+sx <= dst->s_sizex);
 DEE_ASSERT(dst_y+sy <= dst->s_sizey);
 DEE_ASSERT(src_x+sx <= src->s_sizex);
 DEE_ASSERT(src_y+sy <= src->s_sizey);
 DEE_ASSERT(sx != 0);
 DEE_ASSERT(sy != 0);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 getter = DeeSurface_TYPE(src)->st_getpixel;
 for (y = 0; y < sy; ++y) for (x = 0; x < sx; ++x) {
  (*getter)(src,src_x+x,src_y+y,&pixel);
  (*setter)(dst,dst_x+x,dst_y+y,&pixel,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_stretchblit(
 DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y, Dee_size_t dstsx, Dee_size_t dstsy,
 DeeSurfaceObject const *src, double src_x, double src_y, double srcsx, double srcsy,
 Dee_blendinfo_t blend) {
 struct DeePixel pixel; Dee_size_t x,y,srcx,srcy;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 double srcfacx,srcfacy;
 DEE_ASSERT(dst_x+dstsx <= dst->s_sizex); DEE_ASSERT(dstsx != 0);
 DEE_ASSERT(dst_y+dstsy <= dst->s_sizey); DEE_ASSERT(dstsy != 0);
 DEE_ASSERT(src_x >= 0);
 DEE_ASSERT(src_y >= 0);
 DEE_ASSERT((Dee_size_t)(src_x+srcsx) <= src->s_sizex);
 DEE_ASSERT((Dee_size_t)(src_y+srcsy) <= src->s_sizey);
 srcfacx = srcsx/(double)dstsx;
 srcfacy = srcsy/(double)dstsy;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 getter = DeeSurface_TYPE(src)->st_getpixel;
 for (y = 0; y < dstsy; ++y) {
  srcy = (Dee_size_t)(src_y+((double)y*srcfacy));
  for (x = 0; x < dstsx; ++x) {
   srcx = (Dee_size_t)(src_x+((double)x*srcfacx));
   (*getter)(src,srcx,srcy,&pixel);
   (*setter)(dst,dst_x+x,dst_y+y,&pixel,blend);
  }
 }
}
static void DEE_CALL _deesurface_generic_st_flipx(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin,
 Dee_size_t xend, Dee_size_t yend) {
 struct DeePixel pixel1,pixel2; Dee_size_t x,y,half_xend,invdiff;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend);
 DEE_ASSERT(ybegin < yend);
 DEE_ASSERT(xend <= dst->s_sizex);
 DEE_ASSERT(yend <= dst->s_sizey);
 half_xend = xbegin+(xend-xbegin)/2;
 invdiff = xend+xbegin;
 getter = DeeSurface_TYPE(dst)->st_getpixel;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (y = ybegin; y != yend; ++y) {
  for (x = xbegin; x != half_xend; ++x) {
   (*getter)(dst,x,y,&pixel1);
   (*getter)(dst,invdiff-x,y,&pixel2);
   (*setter)(dst,invdiff-x,y,&pixel1,DEE_BLENDINFO_OVERRIDE);
   (*setter)(dst,x,y,&pixel2,DEE_BLENDINFO_OVERRIDE);
  }
 }
}
static void DEE_CALL _deesurface_generic_st_flipy(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin,
 Dee_size_t xend, Dee_size_t yend) {
 struct DeePixel pixel1,pixel2; Dee_size_t x,y,half_yend,invdiff;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend);
 DEE_ASSERT(ybegin < yend);
 DEE_ASSERT(xend <= dst->s_sizex);
 DEE_ASSERT(yend <= dst->s_sizey);
 half_yend = ybegin+(yend-ybegin)/2;
 invdiff = yend+ybegin;
 getter = DeeSurface_TYPE(dst)->st_getpixel;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (x = xbegin; x != xend; ++x) {
  for (y = ybegin; y != half_yend; ++y) {
   (*getter)(dst,x,y,&pixel1);
   (*getter)(dst,x,invdiff-y,&pixel2);
   (*setter)(dst,x,invdiff-y,&pixel1,DEE_BLENDINFO_OVERRIDE);
   (*setter)(dst,x,y,&pixel2,DEE_BLENDINFO_OVERRIDE);
  }
 }
}




// TODO: Optimized versions for these
#define _deesurface_rgba8888_st_getpixel   _deesurface_pixel_st_getpixel
#define _deesurface_rgba8888_st_setpixel   _deesurface_pixel_st_setpixel
#define _deesurface_rgba8888_st_fill        _deesurface_pixel_st_fill
#define _deesurface_rgba8888_st_fillrect    _deesurface_pixel_st_fillrect
#define _deesurface_rgba8888_st_linellhh    _deesurface_pixel_st_linellhh
#define _deesurface_rgba8888_st_linelhhl    _deesurface_pixel_st_linelhhl
#define _deesurface_rgba8888_st_xline       _deesurface_pixel_st_xline
#define _deesurface_rgba8888_st_yline       _deesurface_pixel_st_yline
#define _deesurface_rgba8888_st_blit        _deesurface_pixel_st_blit
#define _deesurface_rgba8888_st_stretchblit _deesurface_pixel_st_stretchblit
#define _deesurface_rgba8888_st_flipx       _deesurface_pixel_st_flipx
#define _deesurface_rgba8888_st_flipy       _deesurface_pixel_st_flipy



DeeSurfaceTypeObject DeeSurfaceType_RGBA8888 = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeSurfaceType_Type),member("surface_rgba8888"),null,
   member(DEE_TYPE_FLAG_VAR_OBJECT|DEE_TYPE_FLAG_NO_SUBCLASS),
   member((DeeTypeObject *)&DeeSurface_Type)),
  DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,
   member(&_deepixelsurface_tp_copy_new),null,
   member(&_deepixelsurface_tp_any_new)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 format_rgba8888, // st_format
 NULL,            // st_next
 {{
  32, // st_pixelbits
  4,  // st_pixelbytes
#ifdef DEE_PLATFORM_LIL_ENDIAN
  0x000000FF, // st_rmask
  0x0000FF00, // st_gmask
  0x00FF0000, // st_bmask
  0xFF000000, // st_amask
  0,          // st_rshift
  8,          // st_gshift
  16,         // st_bshift
  24,         // st_ashift
#else
  0xFF000000, // st_rmask
  0x00FF0000, // st_gmask
  0x0000FF00, // st_bmask
  0x000000FF, // st_amask
  24,         // st_rshift
  16,         // st_gshift
  8,          // st_bshift
  0,          // st_ashift
#endif
  8,          // st_rbits
  8,          // st_gbits
  8,          // st_bbits
  8,          // st_abits
  256,        // st_rmod
  256,        // st_gmod
  256,        // st_bmod
  256,        // st_amod
  1,          // st_rinvmod
  1,          // st_ginvmod
  1,          // st_binvmod
  1,          // st_ainvmod
 }},
 &_deesurface_rgba8888_st_getpixel,    // st_getpixel
 &_deesurface_rgba8888_st_setpixel,    // st_setpixel
 &_deesurface_rgba8888_st_fill,        // st_fill
 &_deesurface_rgba8888_st_fillrect,    // st_fillrect
 &_deesurface_rgba8888_st_xline,       // st_xline
 &_deesurface_rgba8888_st_yline,       // st_yline
 &_deesurface_rgba8888_st_linellhh,    // st_linellhh
 &_deesurface_rgba8888_st_linelhhl,    // st_linelhhl
 &_deesurface_rgba8888_st_blit,        // st_blit
 &_deesurface_rgba8888_st_stretchblit, // st_stretchblit
 &_deesurface_rgba8888_st_flipx,       // st_flipx
 &_deesurface_rgba8888_st_flipy,       // st_flipy
};


#define DEESURFACETYPE_CACHE_SIZE 16
static struct DeeAtomicMutex _deesurfacetype_cache_lock = DeeAtomicMutex_INIT();
static DeeSurfaceTypeObject *_deesurfacetype_cache[DEESURFACETYPE_CACHE_SIZE];
#define _deesurfacetype_cache_clear()\
do{\
 DeeSurfaceTypeObject *elem,*next,**iter,**end,*old_cache[DEESURFACETYPE_CACHE_SIZE];\
 DeeAtomicMutex_AcquireRelaxed(&_deesurfacetype_cache_lock);\
 memcpy(old_cache,_deesurfacetype_cache,sizeof(_deesurfacetype_cache));\
 memset(_deesurfacetype_cache,0,sizeof(_deesurfacetype_cache));\
 DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);\
 end = (iter = old_cache)+DEESURFACETYPE_CACHE_SIZE;\
 do { elem = *iter;\
  while (elem) {\
   DeeAtomicMutex_AcquireRelaxed(&_deesurfacetype_cache_lock);\
   next = elem->st_next; elem->st_next = NULL;\
   DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);\
   Dee_DECREF(elem);\
   elem = next;\
  }\
 } while (++iter != end);\
}while(0)

DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
DeeSurfaceType_Get(DEE_A_IN Dee_uint64_t format) {
 DeeSurfaceTypeObject *result,*newresult;
 Dee_uint8_t cache_id;
 switch (format) {
  case format_rgba8888: result = &DeeSurfaceType_RGBA8888; break;
  default: goto no_builtin;
 }
ret_result:
 Dee_INCREF(result);
 return result;
no_builtin:
 cache_id = (Dee_uint8_t)(
    (Dee_uint8_t)(format >> 56) ^ (Dee_uint8_t)(format >> 48)
  ^ (Dee_uint8_t)(format >> 40) ^ (Dee_uint8_t)(format >> 32)
  ^ (Dee_uint8_t)(format >> 24) ^ (Dee_uint8_t)(format >> 16)
  ^ (Dee_uint8_t)(format >> 8)  ^ (Dee_uint8_t)(format));
 cache_id %= DEESURFACETYPE_CACHE_SIZE;
 DeeAtomicMutex_AcquireRelaxed(&_deesurfacetype_cache_lock);
 result = _deesurfacetype_cache[cache_id];
 while (result) {
  if (result->st_format == format) {
   DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);
   goto ret_result;
  }
  result = result->st_next;
 }
 DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);
 // Must create a new format type
 if DEE_UNLIKELY((result = DeeSurfaceType_New(format)) == NULL) return NULL;
 DeeAtomicMutex_AcquireRelaxed(&_deesurfacetype_cache_lock);
 if ((newresult = _deesurfacetype_cache[cache_id]) == NULL) {
  Dee_INCREF(_deesurfacetype_cache[cache_id] = result);
 } else {
  // Make sure that the type still doesn't exist
  do {
   if DEE_UNLIKELY(newresult->st_format == format) {
    // Unlikely: Some other thread created the format before we could
    Dee_INCREF(newresult);
    DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);
    Dee_DECREF(result);
    return newresult;
   }
   newresult = newresult->st_next;
  } while (newresult);
  // Format doesn't exist --> Append our new one to the linked list in this cache
  result->st_next = _deesurfacetype_cache[cache_id];
  Dee_INCREF(_deesurfacetype_cache[cache_id] = result);
 }
 DeeAtomicMutex_Release(&_deesurfacetype_cache_lock);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
DeeSurfaceType_New(DEE_A_IN Dee_uint64_t format) {
 DeeSurfaceTypeObject *result; Dee_uint32_t pixel_mask;
 DeeObject *new_name; int error;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeSurfaceTypeObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeSurfaceType_Type);
 if ((*DeeType_GET_SLOT(&DeeType_Type,tp_ctor))(&DeeType_Type,(DeeObject *)result) != 0) {
  _DeeObject_DELETE(&DeeSurfaceType_Type,result);
  return NULL;
 }
 // Time to initialize this format
 result->st_format = format;
 result->st_next = NULL;
 DeeType_GET_SLOT(result,tp_flags) |= (DEE_TYPE_FLAG_VAR_OBJECT|DEE_TYPE_FLAG_NO_SUBCLASS);
 Dee_XDECREF(DeeType_GET_SLOT(result,tp_base));
 Dee_INCREF(DeeType_GET_SLOT(result,tp_base) = (DeeTypeObject *)&DeeSurface_Type);

 // Initialize the dynamic operator functions to NULL (will be overwritten later)
 memset(((uint8_t *)&result->st_pixelspec)+sizeof(result->st_pixelspec),0,sizeof(DeeSurfaceTypeObject)-
        (Dee_OFFSETOF(DeeSurfaceTypeObject,st_pixelspec)+sizeof(result->st_pixelspec)));
 switch (DEE_SURFACETYPE_FORMAT_KIND(format)) {
  case DEE_SURFACETYPE_FORMAT_KIND_PIXEL:
   result->st_pixelspec.st_pixelbits = DEE_SURFACETYPE_FORMAT_PIXEL_BITS(format);
   result->st_pixelspec.st_pixelbytes = (Dee_uint8_t)(result->st_pixelspec.st_pixelbits/8);
   if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_R(format)) {
    result->st_pixelspec.st_rbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_R(format);
    result->st_pixelspec.st_rshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_R(format);
   } else {
    result->st_pixelspec.st_rbits = 0;
    result->st_pixelspec.st_rshift = 0;
   }
   if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_G(format)) {
    result->st_pixelspec.st_gbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_G(format);
    result->st_pixelspec.st_gshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_G(format);
   } else {
    result->st_pixelspec.st_gbits = 0;
    result->st_pixelspec.st_gshift = 0;
   }
   if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_B(format)) {
    result->st_pixelspec.st_bbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_B(format);
    result->st_pixelspec.st_bshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_B(format);
   } else {
    result->st_pixelspec.st_bbits = 0;
    result->st_pixelspec.st_bshift = 0;
   }
   if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_A(format)) {
    result->st_pixelspec.st_abits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_A(format);
    result->st_pixelspec.st_ashift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_A(format);
   } else {
    result->st_pixelspec.st_abits = 0;
    result->st_pixelspec.st_ashift = 0;
   }
#ifdef DEE_PLATFORM_LIL_ENDIAN
   {
    Dee_uint8_t max_shift,temp;
    max_shift = (Dee_uint8_t)(result->st_pixelspec.st_rshift+result->st_pixelspec.st_rbits);
         temp = (Dee_uint8_t)(result->st_pixelspec.st_gshift+result->st_pixelspec.st_gbits); if (temp > max_shift) max_shift = temp;
         temp = (Dee_uint8_t)(result->st_pixelspec.st_bshift+result->st_pixelspec.st_bbits); if (temp > max_shift) max_shift = temp;
         temp = (Dee_uint8_t)(result->st_pixelspec.st_ashift+result->st_pixelspec.st_abits); if (temp > max_shift) max_shift = temp;
    if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_R(format)) result->st_pixelspec.st_rshift = (Dee_uint8_t)((max_shift-result->st_pixelspec.st_rbits)-result->st_pixelspec.st_rshift);
    if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_G(format)) result->st_pixelspec.st_gshift = (Dee_uint8_t)((max_shift-result->st_pixelspec.st_gbits)-result->st_pixelspec.st_gshift);
    if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_B(format)) result->st_pixelspec.st_bshift = (Dee_uint8_t)((max_shift-result->st_pixelspec.st_bbits)-result->st_pixelspec.st_bshift);
    if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_A(format)) result->st_pixelspec.st_ashift = (Dee_uint8_t)((max_shift-result->st_pixelspec.st_abits)-result->st_pixelspec.st_ashift);
   }
#endif
   result->st_pixelspec.st_rmask = ((Dee_uint32_t)DEE_BITS(result->st_pixelspec.st_rbits)) << result->st_pixelspec.st_rshift;
   result->st_pixelspec.st_gmask = ((Dee_uint32_t)DEE_BITS(result->st_pixelspec.st_gbits)) << result->st_pixelspec.st_gshift;
   result->st_pixelspec.st_bmask = ((Dee_uint32_t)DEE_BITS(result->st_pixelspec.st_bbits)) << result->st_pixelspec.st_bshift;
   result->st_pixelspec.st_amask = ((Dee_uint32_t)DEE_BITS(result->st_pixelspec.st_abits)) << result->st_pixelspec.st_ashift;
   pixel_mask = ~(Dee_uint32_t)DEE_BITS(result->st_pixelspec.st_pixelbits);
   // Make sure all the masks fit into the given bitsize
   if ((result->st_pixelspec.st_rmask&pixel_mask)!=0
    || (result->st_pixelspec.st_gmask&pixel_mask)!=0
    || (result->st_pixelspec.st_bmask&pixel_mask)!=0
    || (result->st_pixelspec.st_amask&pixel_mask)!=0) {
    DeeError_SET_STRING(&DeeErrorType_ValueError,
                        "Invalid pixelformat: A channel mask exceeds the maximum bitindex");
    goto err_r;
   }

   result->st_pixelspec.st_rmod = (Dee_uint16_t)(DEE_UINT16_C(1) << result->st_pixelspec.st_rbits);
   result->st_pixelspec.st_gmod = (Dee_uint16_t)(DEE_UINT16_C(1) << result->st_pixelspec.st_gbits);
   result->st_pixelspec.st_bmod = (Dee_uint16_t)(DEE_UINT16_C(1) << result->st_pixelspec.st_bbits);
   result->st_pixelspec.st_amod = (Dee_uint16_t)(DEE_UINT16_C(1) << result->st_pixelspec.st_abits);
   result->st_pixelspec.st_rinvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-result->st_pixelspec.st_rbits));
   result->st_pixelspec.st_ginvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-result->st_pixelspec.st_gbits));
   result->st_pixelspec.st_binvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-result->st_pixelspec.st_bbits));
   result->st_pixelspec.st_ainvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-result->st_pixelspec.st_abits));
   result->st_getpixel = &_deesurface_pixel_st_getpixel;
   result->st_setpixel = &_deesurface_pixel_st_setpixel;
   DeeType_GET_SLOT(result,tp_copy_new) = (DeeType_SLOT_TYPE(tp_copy_new))&_deepixelsurface_tp_copy_new;
   DeeType_GET_SLOT(result,tp_any_new) = (DeeType_SLOT_TYPE(tp_any_new))&_deepixelsurface_tp_any_new;
   if DEE_UNLIKELY((new_name = DeeString_Newf(
    "surface_type<%#.8I32x,%#.8I32x,%#.8I32x,%#.8I32x>",
    result->st_pixelspec.st_rmask,result->st_pixelspec.st_gmask,
    result->st_pixelspec.st_bmask,result->st_pixelspec.st_amask)) == NULL) goto err_r;
   error = DeeType_SetName((DeeTypeObject *)result,DeeString_STR(new_name));
   Dee_DECREF(new_name);
   if (error != 0) goto err_r;
   break;
  case DEE_SURFACETYPE_FORMAT_KIND_INDEX:
   result->st_indexspec.st_indexbits = DEE_SURFACETYPE_FORMAT_INDEX_INDEXBITS(format);
   if DEE_UNLIKELY(!result->st_indexspec.st_indexbits) goto inv_format;
   result->st_indexspec.st_mapsize   = DEE_SIZE_C(1) << (result->st_indexspec.st_indexbits-1);
   goto inv_format; // TODO: DEE_SURFACETYPE_FORMAT_KIND_INDEX
   break;
  default:inv_format:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid surfacetype id: %#.16I64x",
                       format);
err_r:
   Dee_DECREF(result);
   return NULL;
 }
 // Fill missing slots with generic functions
 if (!result->st_fill       ) result->st_fill        = &_deesurface_generic_st_fill;
 if (!result->st_fillrect   ) result->st_fillrect    = &_deesurface_generic_st_fillrect;
 if (!result->st_xline      ) result->st_xline       = &_deesurface_generic_st_xline;
 if (!result->st_yline      ) result->st_yline       = &_deesurface_generic_st_yline;
 if (!result->st_linellhh   ) result->st_linellhh    = &_deesurface_generic_st_linellhh;
 if (!result->st_linelhhl   ) result->st_linelhhl    = &_deesurface_generic_st_linelhhl;
 if (!result->st_blit       ) result->st_blit        = &_deesurface_generic_st_blit;
 if (!result->st_stretchblit) result->st_stretchblit = &_deesurface_generic_st_stretchblit;
 if (!result->st_flipx      ) result->st_flipx       = &_deesurface_generic_st_flipx;
 if (!result->st_flipy      ) result->st_flipy       = &_deesurface_generic_st_flipy;
 return result;
}




static DeeSurfaceTypeObject *DEE_CALL _deegfx_surface_format(DeeObject *args) {
 Dee_uint8_t bits; Dee_uint32_t rmask,gmask,bmask,amask;
 Dee_uint64_t format_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,
  "I8uI32uI32uI32uI32u:surface_format",
  &bits,&rmask,&gmask,&bmask,&amask) != 0) return NULL;
 if (!bits || (bits%8)!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid value for bits: %I8u (expected non-zero multiple of 8)",
                      bits);
  return NULL;
 }
#ifndef __INTELLISENSE__
 format_id = DEE_SURFACETYPE_FORMAT_PIXEL(bits,rmask,gmask,bmask,amask);
#endif
 return DeeSurfaceType_Get(format_id);
}

//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("pixel",&DeePixel_Type),
 DeeDex_EXPORT_OBJECT("surface",&DeeSurface_Type),
 DeeDex_EXPORT_FUNCTION("surface_format",&_deegfx_surface_format,
  "(uint8_t bits, uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask) -> surface_type\n"
  "@return: The surface-type describing a pixelformat compatible with the given arguments\n"
  "\tNOTE: The mask arguments are written in big-endian, meaning that a mask of 0xff000000 describes the first of 4 bytes"),
 DeeDex_EXPORT_OBJECT("surface_type",&DeeSurfaceType_Type),
 DeeDex_EXPORT_OBJECT("surface_rgba8888",&DeeSurfaceType_RGBA8888),
 DeeDex_EXPORT_END
};

int DeeDex_Main(DEE_A_INOUT DeeDexContext *context) {
 switch (context->dc_kind) {
  case DEE_DEXCONTEXTKIND_COLLECTMEM:
   context->dc_collectmem.ccm_total += DEE_OBJECTPOOL_CLEAR(pixelpool);
   break;
  case DEE_DEXCONTEXTKIND_SHUTDOWN:
  case DEE_DEXCONTEXTKIND_FINALIZE:
   _deesurfacetype_cache_clear();
   DEE_OBJECTPOOL_CLEAR(pixelpool);
   break;
  default: break;
 }
 return 0;
}



DEE_DECL_END

#ifndef __INTELLISENSE__
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4365 4242)
#include "lodepng/lodepng.c.inl"
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

#endif /* !GUARD_DEEMON_DEX_GFX__GFX_C */
