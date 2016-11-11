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
#ifndef GUARD_DEEMON_DEX_ZLIB_C
#define GUARD_DEEMON_DEX_ZLIB_C 1
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

DEE_DECL_BEGIN


void DeePixel_FillStandardIndexMap(
 DEE_A_OUT_W(size) struct DeePixel *map, DEE_A_IN Dee_size_t size) {
 memset(map,0,size*sizeof(struct DeePixel)); // TODO
}


#define blend_zero(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                   /* lhs* */0
#define blend_one(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                       lhs
#define blend_src_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                ((Dee_uint32_t)lhs*csrc)/0xFF
#define blend_one_minus_src_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)      ((Dee_uint32_t)lhs*(0xFF-csrc))/0xFF
#define blend_dst_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                ((Dee_uint32_t)lhs*cdst)/0xFF
#define blend_one_minus_dst_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)      ((Dee_uint32_t)lhs*(0xFF-cdst))/0xFF
#define blend_src_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                ((Dee_uint32_t)lhs*asrc)/0xFF
#define blend_one_minus_src_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)      ((Dee_uint32_t)lhs*(0xFF-asrc))/0xFF
#define blend_dst_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                ((Dee_uint32_t)lhs*adst)/0xFF
#define blend_one_minus_dst_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)      ((Dee_uint32_t)lhs*(0xFF-adst))/0xFF
#define blend_src_alpha_saturate(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)       ((Dee_uint32_t)lhs*Dee_MIN(asrc,0xFF-adst))/0xFF
#define blend_constant_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)           ((Dee_uint32_t)lhs*cc)/0xFF
#define blend_one_minus_constant_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac) ((Dee_uint32_t)lhs*(0xFF-cc))/0xFF
#define blend_constant_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)           ((Dee_uint32_t)lhs*ac)/0xFF
#define blend_one_minus_constant_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac) ((Dee_uint32_t)lhs*(0xFF-ac))/0xFF

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
#define blend_rgb_dst(fun)\
{ new_lhs.r = (Dee_uint8_t)(fun(dst->r,src->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info))); \
  new_lhs.g = (Dee_uint8_t)(fun(dst->g,src->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info))); \
  new_lhs.b = (Dee_uint8_t)(fun(dst->b,src->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_a_dst(fun)\
{ new_lhs.a = (Dee_uint8_t)(fun(dst->a,src->a,src->a,dst->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_rgb_src(fun)\
{ new_rhs.r = (Dee_uint8_t)(fun(src->r,dst->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info))); \
  new_rhs.g = (Dee_uint8_t)(fun(src->g,dst->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info))); \
  new_rhs.b = (Dee_uint8_t)(fun(src->b,dst->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info))); }
#define blend_a_src(fun)\
{ new_rhs.a = (Dee_uint8_t)(fun(src->a,dst->a,src->a,dst->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info))); }
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBSRC(info),blend_rgb_dst);
 BLEND_SWITCH(DEE_BLENDINFO_GET_ASRC(info),blend_a_dst);
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBDST(info),blend_rgb_src);
 BLEND_SWITCH(DEE_BLENDINFO_GET_ADST(info),blend_a_src);
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
 DeeSurfaceObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t x1,y1,x2,y2; // TODO: color
 Dee_size_t used_x1,used_y1,used_x2,used_y2;
 if (DeeTuple_Unpack(args,"IdIdIdId:line",&x1,&y1,&x2,&y2) != 0) return NULL;
 used_x1 = (Dee_size_t)x1,used_y1 = (Dee_size_t)y1;
 used_x2 = (Dee_size_t)x2,used_y2 = (Dee_size_t)y2;
 DeeSurface_Line(self,used_x1,used_y1,used_x2,used_y2,
                 DeePixel_BLACK,DEE_BLENDINFO_NORMAL);
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
 DEE_METHODDEF_v100("save_png",member(&_deesurface_save_png),
  "(string filename) -> none"),
 DEE_METHODDEF_v100("line",member(&_deesurface_line),
  "(ssize_t x1, ssize_t y1, ssize_t x2, ssize_t y2) -> none"),
 DEE_METHODDEF_END_v100
};


static void DEE_CALL _deenoopsurface_st_get_pixel(DeeSurfaceObject const *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), struct DeePixel *pixel) { *pixel = *DeePixel_EMPTY; }
static void DEE_CALL _deenoopsurface_st_set_pixel(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), struct DeePixel const *DEE_UNUSED(pixel), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_fill(DeeSurfaceObject *DEE_UNUSED(self), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_fill_rect(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), Dee_size_t DEE_UNUSED(sx), Dee_size_t DEE_UNUSED(sy), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_linellhh(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(ybegin_lo), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(yend_hi), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_linelhhl(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(beginy_hi), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(endy_lo), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_xline(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(xbegin), Dee_size_t DEE_UNUSED(xend), Dee_size_t DEE_UNUSED(y), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_yline(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(beginy), Dee_size_t DEE_UNUSED(endy), struct DeePixel const *DEE_UNUSED(color), DEE_A_IN Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_blit(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(dst_x), Dee_size_t DEE_UNUSED(dst_y), DeeSurfaceObject const *DEE_UNUSED(src), Dee_size_t DEE_UNUSED(src_x), Dee_size_t DEE_UNUSED(src_y), Dee_size_t DEE_UNUSED(sx), Dee_size_t DEE_UNUSED(sy), Dee_blendinfo_t DEE_UNUSED(blend)) {}
static void DEE_CALL _deenoopsurface_st_stretch_blit(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(dst_x), Dee_size_t DEE_UNUSED(dst_y), Dee_size_t DEE_UNUSED(dstsx), Dee_size_t DEE_UNUSED(dstsy), DeeSurfaceObject const *DEE_UNUSED(src), Dee_size_t DEE_UNUSED(src_x), Dee_size_t DEE_UNUSED(src_y), Dee_size_t DEE_UNUSED(srcsx), Dee_size_t DEE_UNUSED(srcsy), Dee_blendinfo_t DEE_UNUSED(blend)) {}

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
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
   Dee_OFFSETOF(DeeSurfaceObject,s_pixeldata),null,null,null,null,null),
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
 0, // st_format
 {{
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
 &_deenoopsurface_st_get_pixel,    // st_getpixel
 &_deenoopsurface_st_set_pixel,    // st_setpixel
 &_deenoopsurface_st_fill,         // st_fill
 &_deenoopsurface_st_fill_rect,    // st_fillrect
 &_deenoopsurface_st_xline,        // st_xline
 &_deenoopsurface_st_yline,        // st_yline
 &_deenoopsurface_st_linellhh,     // st_linellhh
 &_deenoopsurface_st_linelhhl,     // st_linelhhl
 &_deenoopsurface_st_blit,         // st_blit
 &_deenoopsurface_st_stretch_blit, // st_stretchblit
};


static DeeSurfaceObject *DEE_CALL _deepixelsurface_tp_any_new(
 DeeSurfaceTypeObject *tp_self, DeeObject *args) {
 Dee_size_t sx,sy;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IuIu:surface_rgba8888",&sx,&sy) != 0) return NULL;
 return DeeSurface_NewEmpty(tp_self,sx,sy);
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


static void DEE_CALL _deesurface_pixel_st_get_pixel(
 DeeSurfaceObject const *self, Dee_size_t x, Dee_size_t y, struct DeePixel *pixel) {
 void *datap;
 DEE_ASSERT(DeeObject_Check(self) && DeeSurface_Check(self));
 DEE_ASSERT(x < DeeSurface_SIZEX(self));
 DEE_ASSERT(y < DeeSurface_SIZEY(self));
 datap = DeeSurface_PIXELADDR(self,x,y);
 DeeSurfaceType_PIXEL_GETPIXEL(DeeSurface_TYPE(self),datap,pixel);
}
static void DEE_CALL _deesurface_pixel_st_set_pixel(
 DeeSurfaceObject *self, Dee_size_t x, Dee_size_t y, struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend) {
 void *datap; struct DeePixel pix;
 DEE_ASSERT(DeeObject_Check(self) && DeeSurface_Check(self));
 DEE_ASSERT(x < DeeSurface_SIZEX(self));
 DEE_ASSERT(y < DeeSurface_SIZEY(self));
 datap = DeeSurface_PIXELADDR(self,x,y);
 DeeSurfaceType_PIXEL_GETPIXEL(DeeSurface_TYPE(self),datap,&pix);
 DeePixel_Blend(&pix,pixel,blend);
 DeeSurfaceType_PIXEL_SETPIXEL(DeeSurface_TYPE(self),datap,&pix);
}

#define _deesurface_pixel_st_fill         _deesurface_generic_st_fill
#define _deesurface_pixel_st_fill_rect    _deesurface_generic_st_fill_rect
#define _deesurface_pixel_st_xline        _deesurface_generic_st_xline
#define _deesurface_pixel_st_yline        _deesurface_generic_st_yline
#define _deesurface_pixel_st_linellhh     _deesurface_generic_st_linellhh
#define _deesurface_pixel_st_linelhhl     _deesurface_generic_st_linelhhl
#define _deesurface_pixel_st_blit         _deesurface_generic_st_blit
#define _deesurface_pixel_st_stretch_blit _deesurface_generic_st_stretch_blit

//////////////////////////////////////////////////////////////////////////
// Generic surface operators (very slow, but work in all situations)
static void DEE_CALL _deesurface_generic_st_fill(
 DeeSurfaceObject *self, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x,sx,y,sy; PDeeSurfaceSetPixel setter;
 sx = self->s_sizex,sy = self->s_sizey;
 setter = DeeSurface_TYPE(self)->st_setpixel;
 for (y = 0; y < sy; ++y) for (x = 0; x < sx; ++x) {
  (*setter)(self,x,y,color,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_fill_rect(
 DeeSurfaceObject *self, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend,
 Dee_size_t yend, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x,y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend); DEE_ASSERT(ybegin < yend);
 setter = DeeSurface_TYPE(self)->st_setpixel;
 for (y = ybegin; y < yend; ++y) for (x = xbegin; x < xend; ++x) {
  (*setter)(self,x,y,color,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_xline(
 DeeSurfaceObject *self, Dee_size_t xbegin, Dee_size_t xend,
 Dee_size_t y, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t x; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend);
 x = xbegin; setter = DeeSurface_TYPE(self)->st_setpixel;
 do (*setter)(self,x,y,color,blend);
 while (++x != xend);
}
static void DEE_CALL _deesurface_generic_st_yline(
 DeeSurfaceObject *self, Dee_size_t x, Dee_size_t beginy,
 Dee_size_t endy, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 Dee_size_t y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(beginy < endy);
 y = beginy; setter = DeeSurface_TYPE(self)->st_setpixel;
 do (*setter)(self,x,y,color,blend);
 while (++y != endy);
}
static void DEE_CALL _deesurface_generic_st_linellhh(
 DeeSurfaceObject *self, Dee_size_t xbegin, Dee_size_t ybegin_lo, Dee_size_t xend,
 Dee_size_t yend_hi, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 double relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step,sizex,sizey;
 DEE_ASSERT(xbegin < xend),DEE_ASSERT(ybegin_lo < yend_hi);
 sizex = xend-xbegin;
 sizey = yend_hi-ybegin_lo;
 DEE_ASSERT(sizex != 0);
 DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(self)->st_setpixel;
 step = 0;
 if (sizex > sizey) {
  relation = (double)sizey/(double)sizex;
  do (*setter)(self,xbegin+step,ybegin_lo+(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (double)sizex/(double)sizey;
  do (*setter)(self,xbegin+(Dee_size_t)(relation*step),ybegin_lo+step,color,blend);
  while (++step != sizex);
 } else {
  do (*setter)(self,xbegin+step,ybegin_lo+step,color,blend);
  while (++step != sizex);
 }
}
static void DEE_CALL _deesurface_generic_st_linelhhl(
 DeeSurfaceObject *self, Dee_size_t xbegin, Dee_size_t beginy_hi, Dee_size_t xend,
 Dee_size_t endy_lo, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 double relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step,sizex,sizey;
 DEE_ASSERT(xbegin < xend),DEE_ASSERT(endy_lo < beginy_hi);
 sizex = xend-xbegin;
 sizey = beginy_hi-endy_lo;
 DEE_ASSERT(sizex != 0);
 DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(self)->st_setpixel;
 step = 0; --endy_lo;
 if (sizex > sizey) {
  relation = (double)sizey/(double)sizex;
  do (*setter)(self,xbegin+step,endy_lo-(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (double)sizex/(double)sizey;
  do (*setter)(self,xbegin+(Dee_size_t)(relation*step),beginy_hi-step,color,blend);
  while (++step != sizex);
 } else {
  do (*setter)(self,xbegin+step,beginy_hi-step,color,blend);
  while (++step != sizex);
 }
}

static void DEE_CALL _deesurface_generic_st_blit(
 DeeSurfaceObject *self, Dee_size_t dst_x, Dee_size_t dst_y,
 DeeSurfaceObject const *src, Dee_size_t src_x, Dee_size_t src_y,
 Dee_size_t sx, Dee_size_t sy, Dee_blendinfo_t blend) {
 struct DeePixel pixel; Dee_size_t x,y;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(dst_x+sx <= self->s_sizex);
 DEE_ASSERT(dst_y+sy <= self->s_sizey);
 DEE_ASSERT(src_x+sx <= src->s_sizex);
 DEE_ASSERT(src_y+sy <= src->s_sizey);
 DEE_ASSERT(sx != 0);
 DEE_ASSERT(sy != 0);
 setter = DeeSurface_TYPE(self)->st_setpixel;
 getter = DeeSurface_TYPE(src)->st_getpixel;
 for (y = 0; y < sy; ++y) for (x = 0; x < sx; ++x) {
  (*getter)(src,src_x+x,src_y+y,&pixel);
  (*setter)(self,dst_x+x,dst_y+y,&pixel,blend);
 }
}
static void DEE_CALL _deesurface_generic_st_stretch_blit(
 DeeSurfaceObject      *self, Dee_size_t dst_x, Dee_size_t dst_y, Dee_size_t dstsx, Dee_size_t dstsy,
 DeeSurfaceObject const *src, Dee_size_t src_x, Dee_size_t src_y, Dee_size_t srcsx, Dee_size_t srcsy,
 Dee_blendinfo_t blend) {
 struct DeePixel pixel; Dee_size_t x,y,srcx,srcy;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 double srcfacx,srcfacy;
 DEE_ASSERT(dst_x+dstsx <= self->s_sizex); DEE_ASSERT(dstsx != 0);
 DEE_ASSERT(dst_y+dstsy <= self->s_sizey); DEE_ASSERT(dstsy != 0);
 DEE_ASSERT(src_x+srcsx <= src ->s_sizex); DEE_ASSERT(srcsx != 0);
 DEE_ASSERT(src_y+srcsy <= src ->s_sizey); DEE_ASSERT(srcsy != 0);
 srcfacx = (double)srcsx/(double)dstsx;
 srcfacy = (double)srcsy/(double)dstsy;
 setter = DeeSurface_TYPE(self)->st_setpixel;
 getter = DeeSurface_TYPE(src)->st_getpixel;
 for (y = 0; y < dstsy; ++y) {
  srcy = src_y+(Dee_size_t)((double)y*srcfacy);
  for (x = 0; x < dstsx; ++x) {
   srcx = src_x+(Dee_size_t)((double)x*srcfacx);
   (*getter)(src,srcy,srcy,&pixel);
   (*setter)(self,dst_x+x,dst_y+y,&pixel,blend);
  }
 }
}




// TODO: Optimized versions for these
#define _deesurface_rgba8888_st_get_pixel    _deesurface_pixel_st_get_pixel
#define _deesurface_rgba8888_st_set_pixel    _deesurface_pixel_st_set_pixel
#define _deesurface_rgba8888_st_fill         _deesurface_pixel_st_fill
#define _deesurface_rgba8888_st_fill_rect    _deesurface_pixel_st_fill_rect
#define _deesurface_rgba8888_st_linellhh     _deesurface_pixel_st_linellhh
#define _deesurface_rgba8888_st_linelhhl     _deesurface_pixel_st_linelhhl
#define _deesurface_rgba8888_st_xline        _deesurface_pixel_st_xline
#define _deesurface_rgba8888_st_yline        _deesurface_pixel_st_yline
#define _deesurface_rgba8888_st_blit         _deesurface_pixel_st_blit
#define _deesurface_rgba8888_st_stretch_blit _deesurface_pixel_st_stretch_blit



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
 {{
  4, // st_pixelbytes
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
 &_deesurface_rgba8888_st_get_pixel,    // st_getpixel
 &_deesurface_rgba8888_st_set_pixel,    // st_setpixel
 &_deesurface_rgba8888_st_fill,         // st_fill
 &_deesurface_rgba8888_st_fill_rect,    // st_fillrect
 &_deesurface_rgba8888_st_xline,        // st_xline
 &_deesurface_rgba8888_st_yline,        // st_yline
 &_deesurface_rgba8888_st_linellhh,     // st_linellhh
 &_deesurface_rgba8888_st_linelhhl,     // st_linelhhl
 &_deesurface_rgba8888_st_blit,         // st_blit
 &_deesurface_rgba8888_st_stretch_blit, // st_stretchblit
};


DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
DeeSurfaceType_Get(DEE_A_IN Dee_uint64_t format) {
 DeeSurfaceTypeObject *result;
 (void)format; // TODO
 Dee_INCREF(result = &DeeSurfaceType_RGBA8888);
 return result;
}



//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("surface",&DeeSurface_Type),
 DeeDex_EXPORT_OBJECT("surface_rgba8888",&DeeSurfaceType_RGBA8888),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#ifndef __INTELLISENSE__
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4365 4242)
#include "lodepng/lodepng.c.inl"
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

#endif /* !GUARD_DEEMON_DEX_ZLIB_C */
