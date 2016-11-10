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

#include "_gfx.h"
#include <deemon/dex.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

void DeePixel_FillStandardIndexMap(
 DEE_A_OUT_W(size) struct DeePixel *map, DEE_A_IN Dee_size_t size) {
}


#define blend_zero(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                   /* lhs* */0
#define blend_one(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                       lhs
#define blend_src_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                 lhs*csrc
#define blend_one_minus_src_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)       lhs*(0xFF-csrc)
#define blend_dst_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                 lhs*cdst
#define blend_one_minus_dst_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)       lhs*(0xFF-cdst)
#define blend_src_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                 lhs*asrc
#define blend_one_minus_src_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)       lhs*(0xFF-asrc)
#define blend_dst_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)                 lhs*adst
#define blend_one_minus_dst_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)       lhs*(0xFF-adst)
#define blend_src_alpha_saturate(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)        lhs*Dee_MIN(asrc,0xFF-adst)
#define blend_constant_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)           (lhs*cc)/0xFF
#define blend_one_minus_constant_color(lhs,rhs,cdst,csrc,adst,asrc,cc,ac) (lhs*(0xFF-cc))/0xFF
#define blend_constant_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac)           (lhs*ac)/0xFF
#define blend_one_minus_constant_alpha(lhs,rhs,cdst,csrc,adst,asrc,cc,ac) (lhs*(0xFF-ac))/0xFF

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
{ new_rhs.r = fun(src->r,dst->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info)); \
  new_rhs.g = fun(src->g,dst->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info)); \
  new_rhs.b = fun(src->b,dst->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info)); }
#define blend_a_src(fun)\
{ new_rhs.a = fun(dst->a,src->a,dst->a,src->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info)); }
#define blend_rgb_dst(fun)\
{ new_lhs.r = fun(dst->r,src->r,src->r,dst->r,src->a,dst->a,DEE_BLENDINFO_GET_CR(info),DEE_BLENDINFO_GET_CA(info)); \
  new_lhs.g = fun(dst->g,src->g,src->g,dst->g,src->a,dst->a,DEE_BLENDINFO_GET_CG(info),DEE_BLENDINFO_GET_CA(info)); \
  new_lhs.b = fun(dst->b,src->b,src->b,dst->b,src->a,dst->a,DEE_BLENDINFO_GET_CB(info),DEE_BLENDINFO_GET_CA(info)); }
#define blend_a_dst(fun)\
{ new_lhs.a = fun(dst->a,src->a,src->a,dst->a,src->a,dst->a,DEE_BLENDINFO_GET_CA(info),DEE_BLENDINFO_GET_CA(info)); }
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBSRC(info),blend_rgb_src);
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBSRC(info),blend_a_src);
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBDST(info),blend_rgb_dst);
 BLEND_SWITCH(DEE_BLENDINFO_GET_RGBDST(info),blend_a_dst);
#undef blend_a_dst
#undef blend_rgb_dst
#undef blend_a_src
#undef blend_rgb_src
#define func_rgb(fun)\
{ dst->r = fun(new_lhs.r,new_rhs.r); \
  dst->g = fun(new_lhs.g,new_rhs.g); \
  dst->b = fun(new_lhs.b,new_rhs.a); }
#define func_a(fun)\
{ dst->a = fun(new_lhs.a,new_rhs.a); }
 FUNC_SWITCH(DEE_BLENDINFO_GET_RGBFUN(info),func_rgb);
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



DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFilledEx(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_IN Dee_size_t sizex,
 DEE_A_IN Dee_size_t sizey, DEE_A_IN struct DeePixel const *color) {
 DeeSurfaceObject *result;
 if DEE_LIKELY((result = DeeSurface_New(stype,sizex,sizey)) != NULL) {
  DeeSurface_Fill(result,color,DEE_BLENDINFO_OVERRIDE);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_New(
 DEE_A_IN DeeSurfaceTypeObject const *stype,
 DEE_A_IN Dee_size_t sizex, DEE_A_IN Dee_size_t sizey) {
 DeeSurfaceObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(stype) && DeeSurfaceType_Check(stype));
 // Don't allow empty surfaces (silently extend them to 1x1)
 if (!sizex) sizex = 1;
 if (!sizey) sizey = 1;
 switch (DeeSurfaceType_KIND(stype)) {
  case DEE_SURFACETYPE_FORMAT_KIND_PIXEL:
   size = stype->st_pixelspec.st_pixelbytes*sizex*sizey;
   result = (DeeSurfaceObject *)DeeObject_Malloc(DeeSurface_PIXELBASESIZE+size);
   if DEE_UNLIKELY(!result) return NULL;
   result->s_pixeldata.s_pixlsize = stype->st_pixelspec.st_pixelbytes;
   result->s_pixeldata.s_scanline = result->s_pixeldata.s_pixlsize*sizex;
   break;
   // TODO: Index surface
  default: DEE_BUILTIN_UNREACHABLE();
 }
 DeeObject_INIT(result,(DeeTypeObject *)stype);
 result->s_sizex = sizex;
 result->s_sizey = sizey;
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
DeeSurfaceType_Get(DEE_A_IN Dee_uint64_t format) {
 DeeSurfaceTypeObject *result;
 (void)format; // TODO
 Dee_INCREF(result = &DeeSurfaceType_RGBA8888);
 return result;
}






static struct DeeMemberDef const _deesurface_tp_members[] = {
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("sizex",DeeSurfaceObject,s_sizex,Dee_size_t,"-> size_t"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("sizey",DeeSurfaceObject,s_sizey,Dee_size_t,"-> size_t"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("width",DeeSurfaceObject,s_sizex,Dee_size_t,"-> size_t\tAlias for #sizex"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("height",DeeSurfaceObject,s_sizey,Dee_size_t,"-> size_t\tAlias for #sizey"),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deesurface_tp_methods[] = {
 DEE_METHODDEF_END_v100
};

DeeSurfaceTypeObject DeeSurface_Type = {
 {
  DEE_TYPE_OBJECT_HEAD_v100(member("surface"),member(
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
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 }
};



//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ZLIB_C */
