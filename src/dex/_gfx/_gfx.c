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
#define DEE_LIMITED_DEX_GFX 1

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
#include <deemon/file.h>
#include <deemon/deemonrun.h>
#include <deemon/memberdef.h>
#include <deemon/tuple.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/__bswap_intrin.inl>
#include DEE_INCLUDE_MEMORY_API()
#include "pixelobject.inl"
#include "surface.generic.inl"

#ifdef DEE_PLATFORM_WINDOWS
#include "window/window.h"
#endif

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

DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFromStream(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *fp) {
 DeeSurfaceObject *result; int error;
 error = DeeSurface_TryNewFromPNGStream(stype,fp,&result);
 if (error == 0) return result;
 if (error < 0) return NULL;
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Not an image file: %k",fp);
 return NULL;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSurface_TryNewFromPNGStream(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *fp,
 DEE_A_REF DEE_A_OUT DeeSurfaceObject **result) {
 Dee_uint64_t old_pos,max_size; Dee_size_t file_buffer_size;
 void *file_buffer; unsigned char *pixels; unsigned w,h,error;
 DeeSurfaceObject *read_image;
 if (DeeFile_Tell(fp,&old_pos) != 0) return -1;
 if (DeeFile_Seek(fp,0,DEE_SEEK_END,&max_size) != 0) return -1;
 if (max_size <= old_pos) return 1; // empty file
#if DEE_TYPES_SIZEOF_POINTER < 8
 if ((max_size-old_pos) > (Dee_size_t)-1) return 1; // File too large
#endif
 file_buffer_size = (Dee_size_t)(max_size-old_pos);
 DEE_ASSERT(file_buffer_size != 0);
 // Allocate a buffer large enough to hold the entire file
 while DEE_UNLIKELY((file_buffer = malloc_nz(file_buffer_size)) == NULL) {
  if (Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 if (DeeFile_SetPos(fp,old_pos) != 0) {err_filebuffer: free_nn(file_buffer); return -1; }
 if (DeeFile_ReadAll(fp,file_buffer,file_buffer_size) != 0) {
  DeeError_PUSH_STATE() {
   if (DeeFile_SetPos(fp,old_pos) != 0) DeeError_Handled();
  } DeeError_POP_STATE();
  goto err_filebuffer;
 }
 // TODO: Select the proper encoding based on stype (may help to skip a cast below)
 error = lodepng_decode_memory(&pixels,&w,&h,
                               (unsigned char const *)file_buffer,
                               file_buffer_size,LCT_RGBA,8);
 free_nn(file_buffer);
 switch (error) {
  case 0: break;
  case 27: case 28: return 1; // Not a PNG file
  default:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid PNG file : %d : %s",
                       lodepng_error_text(error));
   return -1;
 }
 // Convert the read data into the requested pixel format
 read_image = DeeSurface_New(&DeeSurfaceType_RGBA8888,w,h);
 if DEE_UNLIKELY(!read_image) { free_nn(pixels); return -1; }
 memcpy(read_image->s_pixeldata.s_pixels,pixels,w*h*4);
 free_nn(pixels);
 // NOTE: This if 'stype' is rgba8888, this
 //       function does not allocate a new surface.
 *result = DeeSurface_Convert(read_image,stype);
 Dee_DECREF(read_image);
 if DEE_UNLIKELY(!*result) return -1;
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeSurfaceObject *DeeSurface_NewFromFilename(
 DEE_A_IN DeeSurfaceTypeObject const *stype, DEE_A_INOUT DeeObject *filename) {
 DeeObject *fp; DeeSurfaceObject *result;
 if DEE_UNLIKELY((fp = DeeFileIO_NewObject(filename,"r")) == NULL) return NULL;
 result = DeeSurface_NewFromStream(stype,fp);
 Dee_DECREF(fp);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeSurface_SavePNGStream(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_INOUT DeeObject *fp) {
 DeeSurfaceObject *rgba8888; unsigned error; int error2;
 unsigned char *buffer; size_t buffersize;
 if DEE_UNLIKELY((rgba8888 = DeeSurface_Convert(self,&DeeSurfaceType_RGBA8888)) == NULL) return -1;
 error = lodepng_encode_memory(&buffer,&buffersize,
                               (unsigned char const *)rgba8888->s_pixeldata.s_pixels,
                               rgba8888->s_sizex,rgba8888->s_sizey,LCT_RGBA,8);
 Dee_DECREF(rgba8888);
 if (error != 0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Failed to save png : %d : %s",
                      error,lodepng_error_text(error));
  return -1;
 }
 error2 = DeeFile_WriteAll(fp,buffer,buffersize);
 free_nn(buffer);
 return error2;
}
DEE_A_RET_EXCEPT(-1) int DeeSurface_SavePNGFile(
 DEE_A_IN DeeSurfaceObject const *self, DEE_A_INOUT DeeObject *filename) {
 DeeSurfaceObject *rgba8888; unsigned error; int error2;
 unsigned char *buffer; size_t buffersize; DeeObject *fp;
 if DEE_UNLIKELY((rgba8888 = DeeSurface_Convert(self,&DeeSurfaceType_RGBA8888)) == NULL) return -1;
 error = lodepng_encode_memory(&buffer,&buffersize,
                               (unsigned char const *)rgba8888->s_pixeldata.s_pixels,
                               rgba8888->s_sizex,rgba8888->s_sizey,LCT_RGBA,8);
 Dee_DECREF(rgba8888);
 if (error != 0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Failed to save png : %d : %s",
                      error,lodepng_error_text(error));
  return -1;
 }
 if DEE_UNLIKELY((fp = DeeFileIO_NewObject(filename,"w")) == NULL) {
/*err_buffer:*/ free_nn(buffer); return -1;
 }
 error2 = DeeFile_WriteAll(fp,buffer,buffersize);
 free_nn(buffer);
 Dee_DECREF(fp);
 return error2;
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
static DeeObject *DEE_CALL _deesurface_box(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t x,y; Dee_size_t sx,sy;
 DeePixelObject *color = &DeePixel_BlackObject; Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIuIu|oI64u:box",&x,&y,&sx,&sy,&color,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)color,&DeePixel_Type) != 0) return NULL;
 DeeSurface_Box(dst,x,y,sx,sy,&color->p_pixel,blend);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_triangle(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t dx1,dy1,dx2,dy2,dx3,dy3;
 DeePixelObject *color = &DeePixel_BlackObject; Dee_uint64_t blend = DEE_BLENDINFO_NORMAL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIdIdIdId|oI64u:box",&dx1,&dy1,&dx2,&dy2,&dx3,&dy3,&color,&blend) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)color,&DeePixel_Type) != 0) return NULL;
 DeeSurface_Triangle(dst,dx1,dy1,dx2,dy2,dx3,dy3,&color->p_pixel,blend);
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
static DeeObject *DEE_CALL _deesurface_flipx(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t dx,dy; Dee_size_t dsx,dsy;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIuIu:flipx",&dx,&dy,&dsx,&dsy) != 0) return NULL;
 DeeSurface_FlipXRect(dst,dx,dy,dsx,dsy);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_flipy(
 DeeSurfaceObject *dst, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t dx,dy; Dee_size_t dsx,dsy;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdIuIu:flipy",&dx,&dy,&dsx,&dsy) != 0) return NULL;
 DeeSurface_FlipYRect(dst,dx,dy,dsx,dsy);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesurface_save_png(
 DeeSurfaceObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *filename;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:save_png",&filename) != 0) return NULL;
 if DEE_UNLIKELY((DeeFile_Check(filename)
  ? DeeSurface_SavePNGStream(self,filename)
  : DeeSurface_SavePNGFile(self,filename)) != 0) return NULL;
 DeeReturn_None;
}

//////////////////////////////////////////////////////////////////////////
// We (ab)use the seq-range operators for pixel access
// >> my_surface = surface_rgba8888(16,16);
// >> print my_surface[0:0]; // Get pixel color
// >> my_surface[0:0] = pixel.green; // Set pixel color
// >> del my_surface[0:0]; // Same as set with 'pixel.empty'
// NOTE: Pixels outside of the surface are empty/blank
// NOTE: Writing pixels is always performed with 'DEE_BLENDINFO_OVERRIDE'
static DeePixelObject *DEE_CALL _deesurface_tp_seq_range_get(
 DeeSurfaceObject *self, DeeObject *xob, DeeObject *yob) {
 DeePixelObject *result; Dee_ssize_t x,y;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,xob,&x) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,yob,&y) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_OBJECTPOOL_ALLOC(pixelpool)) == NULL) return NULL;
 DeeObject_INIT(result,&DeePixel_Type);
 DeeSurface_GetPixel(self,x,y,&result->p_pixel);
 return result;
}
static int DEE_CALL _deesurface_tp_seq_range_del(
 DeeSurfaceObject *self, DeeObject *xob, DeeObject *yob) {
 Dee_ssize_t x,y;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,xob,&x) != 0) return -1;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,yob,&y) != 0) return -1;
 DeeSurface_SetPixel(self,x,y,DeePixel_EMPTY,DEE_BLENDINFO_OVERRIDE);
 return 0;
}
static int DEE_CALL _deesurface_tp_seq_range_set(
 DeeSurfaceObject *self, DeeObject *xob, DeeObject *yob, DeePixelObject *v) {
 Dee_ssize_t x,y;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,xob,&x) != 0) return -1;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,yob,&y) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)v,&DeePixel_Type) != 0) return -1;
 DeeSurface_SetPixel(self,x,y,&v->p_pixel,DEE_BLENDINFO_OVERRIDE);
 return 0;
}

static struct DeeMethodDef const _deesurface_tp_methods[] = {
 DEE_METHODDEF_v100("save_png",member(&_deesurface_save_png),"(string filename) -> none"),
 DEE_METHODDEF_v100("line",member(&_deesurface_line),
  "(ssize_t x1, ssize_t y1, ssize_t x2, ssize_t y2, pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none\n"),
 DEE_METHODDEF_v100("fill",member(&_deesurface_fill),
  "(ssize_t x, ssize_t y, size_t sx, size_t sy, pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("box",member(&_deesurface_box),
  "(ssize_t x, ssize_t y, size_t sx, size_t sy, pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("triangle",member(&_deesurface_triangle),
  "(ssize_t dx1, ssize_t dy1, ssize_t dx2, ssize_t dy2, ssize_t dx3, ssize_t dy3, "
   "pixel color = pixel.black, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("blit",member(&_deesurface_blit),
  "(ssize_t dx, ssize_t dy, surface src, size_t sx = 0, size_t sy = 0, size_t ssx = size_t(-1), size_t ssy = size_t(-1), uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("stretch",member(&_deesurface_stretch),
  "(ssize_t dx, ssize_t dy, size_t dsx, size_t dsy, surface src, size_t sx = 0, size_t sy = 0, size_t ssx = src.sizex, size_t ssy = src.sizey, uint64_t blend = DEE_BLENDINFO_NORMAL) -> none"),
 DEE_METHODDEF_v100("flipx",member(&_deesurface_flipx),
  "(ssize_t dx, ssize_t dy, size_t dsx, size_t dsy) -> none"),
 DEE_METHODDEF_v100("flipy",member(&_deesurface_flipy),
  "(ssize_t dx, ssize_t dy, size_t dsx, size_t dsy) -> none"),
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
static void DEE_CALL _deenoopsurface_st_pixelmaskmsb(DeeSurfaceObject *DEE_UNUSED(self), Dee_size_t DEE_UNUSED(x), Dee_size_t DEE_UNUSED(y), Dee_size_t DEE_UNUSED(sx), Dee_size_t DEE_UNUSED(sy), Dee_size_t DEE_UNUSED(line_bytes), void const *DEE_UNUSED(data), struct DeePixel const *DEE_UNUSED(color), Dee_blendinfo_t DEE_UNUSED(blend)) {}
#define _deenoopsurface_st_pixelmasklsb _deenoopsurface_st_pixelmaskmsb

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
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,
   member(&_deesurface_tp_seq_range_get),
   member(&_deesurface_tp_seq_range_del),
   member(&_deesurface_tp_seq_range_set),null,null),
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
 &_deenoopsurface_st_getpixel,     // st_getpixel
 &_deenoopsurface_st_setpixel,     // st_setpixel
 &_deenoopsurface_st_fill,         // st_fill
 &_deenoopsurface_st_fillrect,     // st_fillrect
 &_deenoopsurface_st_xline,        // st_xline
 &_deenoopsurface_st_yline,        // st_yline
 &_deenoopsurface_st_linellhh,     // st_linellhh
 &_deenoopsurface_st_linelhhl,     // st_linelhhl
 &_deenoopsurface_st_blit,         // st_blit
 &_deenoopsurface_st_stretchblit,  // st_stretchblit
 &_deenoopsurface_st_flipx,        // st_flipx
 &_deenoopsurface_st_flipy,        // st_flipy
 &_deenoopsurface_st_pixelmaskmsb, // st_pixelmaskmsb
 &_deenoopsurface_st_pixelmasklsb, // st_pixelmasklsb
};


static DeeSurfaceObject *DEE_CALL _deepixelsurface_tp_any_new(
 DeeSurfaceTypeObject *tp_self, DeeObject *args) {
 Dee_size_t sx,sy; DeeObject *arg0,*arg1 = NULL,*arg2 = NULL;
 DeeSurfaceObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:surface",&arg0,&arg1,&arg2) != 0) return NULL;
 if (DeeSurface_Check(arg0)) {
  // Cast operation (w/ optional resize/stretch)
  if (!arg1) sx = DeeSurface_SIZEX(arg0); else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,arg1,&sx) != 0) return NULL;
  if (!arg2) sy = DeeSurface_SIZEY(arg0); else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,arg2,&sy) != 0) return NULL;
  if DEE_UNLIKELY((result = DeeSurface_New(tp_self,sx,sy)) == NULL) return NULL;
  if (sx == DeeSurface_SIZEX(arg0) && sy == DeeSurface_SIZEY(arg0)) {
   // Simple blit (no stretch required)
   _DeeSurface_Blit(result,0,0,(DeeSurfaceObject *)arg0,DEE_BLENDINFO_OVERRIDE);
  } else { // Advanced blit (stretch is required)
   _DeeSurface_StretchBlit(result,0,0,sx,sy,(DeeSurfaceObject *)arg0,0,0,
                           DeeSurface_SIZEX(arg0),DeeSurface_SIZEY(arg0),
                           DEE_BLENDINFO_OVERRIDE);
  }
 } else {
  // Create new surface
  if DEE_UNLIKELY(!arg1) {
   if (DeeFile_Check(arg0)) {
    // Load a picture from a stream (currently only supports PNG)
    return DeeSurface_NewFromStream(tp_self,arg0);
   } else if (DeeAnyString_Check(arg0)) {
    // Load a picture from a file
    return DeeSurface_NewFromFilename(tp_self,arg0);
   }
   DeeError_SetStringf(&DeeErrorType_TypeError,
                       "surface requires at 2 ... 3 argument(s) (%Iu given)",
                       DeeTuple_SIZE(args));
   return NULL;
  }
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,arg0,&sx) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,arg1,&sy) != 0) return NULL;
  if (!arg2) arg2 = (DeeObject *)&DeePixel_EmptyObject;
  else if (DeeError_TypeError_CheckTypeExact(arg2,&DeePixel_Type) != 0) return NULL;
  result = DeeSurface_NewFilled(tp_self,sx,sy,&((DeePixelObject *)arg2)->p_pixel);
 }
 return result;
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

#define _deesurface_pixel_st_fill           _deesurface_generic_st_fill
#define _deesurface_pixel_st_fillrect       _deesurface_generic_st_fillrect
#define _deesurface_pixel_st_xline          _deesurface_generic_st_xline
#define _deesurface_pixel_st_yline          _deesurface_generic_st_yline
#define _deesurface_pixel_st_linellhh       _deesurface_generic_st_linellhh
#define _deesurface_pixel_st_linelhhl       _deesurface_generic_st_linelhhl
#define _deesurface_pixel_st_blit           _deesurface_generic_st_blit
#define _deesurface_pixel_st_stretchblit    _deesurface_generic_st_stretchblit
#define _deesurface_pixel_st_flipx          _deesurface_generic_st_flipx
#define _deesurface_pixel_st_flipy          _deesurface_generic_st_flipy
#define _deesurface_pixel_st_pixelmaskmsb   _deesurface_generic_st_pixelmaskmsb
#define _deesurface_pixel_st_pixelmasklsb   _deesurface_generic_st_pixelmasklsb



#if DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
#define _deesurface_doc \
 "(size_t sx, size_t sy, pixel filler = pixel.empty)\n" \
 "@return: A new surface with the given dimensions @sx|@sy and all pixels set to @filler\n" \
 "(surface other, size_t sx = other.sizex, size_t sy = other.sizey)\n" \
 "@return: A new surface with the given dimensions @sx|@sy and containing the same pixels as @other\n" \
 "\tNOTE: During the conversion operation data may be lost if the new surface type can't represent the data from @other\n"
#else
#define _deesurface_doc NULL
#endif


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
 if (format == DeeSurfaceTypeFormat_RGBA8888) {
  result = &DeeSurfaceType_RGBA8888;
 } else goto no_builtin;
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


DEE_A_RET_EXCEPT(-1) int DeeSurfacePixelSpec_Init(
 DEE_A_OUT struct DeeSurfacePixelSpec *self, DEE_A_IN Dee_uint64_t formatid) {
 Dee_uint32_t pixel_mask;
 self->st_pixelbits = DEE_SURFACETYPE_FORMAT_PIXEL_BITS(formatid);
 self->st_pixelbytes = (Dee_uint8_t)(self->st_pixelbits/8);
 if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_R(formatid)) {
  self->st_rbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_R(formatid);
#ifdef DEE_PLATFORM_LIL_ENDIAN
  self->st_rshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_R(formatid);
#else
  self->st_rshift = DEE_SURFACETYPE_FORMAT_PIXEL_OFF_R(formatid);
#endif
 } else {
  self->st_rbits = 0;
  self->st_rshift = 0;
 }
 if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_G(formatid)) {
  self->st_gbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_G(formatid);
#ifdef DEE_PLATFORM_LIL_ENDIAN
  self->st_gshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_G(formatid);
#else
  self->st_gshift = DEE_SURFACETYPE_FORMAT_PIXEL_OFF_G(formatid);
#endif
 } else {
  self->st_gbits = 0;
  self->st_gshift = 0;
 }
 if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_B(formatid)) {
  self->st_bbits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_B(formatid);
#ifdef DEE_PLATFORM_LIL_ENDIAN
  self->st_bshift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_B(formatid);
#else
  self->st_bshift = DEE_SURFACETYPE_FORMAT_PIXEL_OFF_B(formatid);
#endif
 } else {
  self->st_bbits = 0;
  self->st_bshift = 0;
 }
 if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_A(formatid)) {
  self->st_abits = DEE_SURFACETYPE_FORMAT_PIXEL_BIT_A(formatid);
#ifdef DEE_PLATFORM_LIL_ENDIAN
  self->st_ashift = 32-DEE_SURFACETYPE_FORMAT_PIXEL_OFF_A(formatid);
#else
  self->st_ashift = DEE_SURFACETYPE_FORMAT_PIXEL_OFF_A(formatid);
#endif
 } else {
  self->st_abits = 0;
  self->st_ashift = 0;
 }
#ifdef DEE_PLATFORM_LIL_ENDIAN
 {
  Dee_uint8_t max_shift,temp;
  max_shift = (Dee_uint8_t)(self->st_rshift+self->st_rbits);
       temp = (Dee_uint8_t)(self->st_gshift+self->st_gbits); if (temp > max_shift) max_shift = temp;
       temp = (Dee_uint8_t)(self->st_bshift+self->st_bbits); if (temp > max_shift) max_shift = temp;
       temp = (Dee_uint8_t)(self->st_ashift+self->st_abits); if (temp > max_shift) max_shift = temp;
  if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_R(formatid)) self->st_rshift = (Dee_uint8_t)((max_shift-self->st_rbits)-self->st_rshift);
  if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_G(formatid)) self->st_gshift = (Dee_uint8_t)((max_shift-self->st_gbits)-self->st_gshift);
  if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_B(formatid)) self->st_bshift = (Dee_uint8_t)((max_shift-self->st_bbits)-self->st_bshift);
  if (DEE_SURFACETYPE_FORMAT_PIXEL_HAS_A(formatid)) self->st_ashift = (Dee_uint8_t)((max_shift-self->st_abits)-self->st_ashift);
 }
#endif
 self->st_rmask = ((Dee_uint32_t)DEE_BITS(self->st_rbits)) << self->st_rshift;
 self->st_gmask = ((Dee_uint32_t)DEE_BITS(self->st_gbits)) << self->st_gshift;
 self->st_bmask = ((Dee_uint32_t)DEE_BITS(self->st_bbits)) << self->st_bshift;
 self->st_amask = ((Dee_uint32_t)DEE_BITS(self->st_abits)) << self->st_ashift;
 if (self->st_pixelbits != 32) {
  pixel_mask = ~(Dee_uint32_t)DEE_BITS(self->st_pixelbits);
  // Make sure all the masks fit into the given bitsize
  if ((self->st_rmask&pixel_mask)!=0
   || (self->st_gmask&pixel_mask)!=0
   || (self->st_bmask&pixel_mask)!=0
   || (self->st_amask&pixel_mask)!=0) {
   DeeError_SET_STRING(&DeeErrorType_ValueError,
                       "Invalid pixelformat: A channel mask exceeds the maximum bitindex");
   return -1;
  }
 }

 self->st_rmod = (Dee_uint16_t)(DEE_UINT16_C(1) << self->st_rbits);
 self->st_gmod = (Dee_uint16_t)(DEE_UINT16_C(1) << self->st_gbits);
 self->st_bmod = (Dee_uint16_t)(DEE_UINT16_C(1) << self->st_bbits);
 self->st_amod = (Dee_uint16_t)(DEE_UINT16_C(1) << self->st_abits);
 self->st_rinvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-self->st_rbits));
 self->st_ginvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-self->st_gbits));
 self->st_binvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-self->st_bbits));
 self->st_ainvmod = (Dee_uint16_t)(DEE_UINT16_C(1) << (8-self->st_abits));
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeSurfaceTypeObject *
DeeSurfaceType_New(DEE_A_IN Dee_uint64_t format) {
 DeeSurfaceTypeObject *result;
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
   if (DeeSurfacePixelSpec_Init(&result->st_pixelspec,format) != 0) goto err_r;
   result->st_getpixel = &_deesurface_pixel_st_getpixel;
   result->st_setpixel = &_deesurface_pixel_st_setpixel;
   DeeType_GET_SLOT(result,tp_copy_new) = (DeeType_SLOT_TYPE(tp_copy_new))&_deepixelsurface_tp_copy_new;
   DeeType_GET_SLOT(result,tp_any_new) = (DeeType_SLOT_TYPE(tp_any_new))&_deepixelsurface_tp_any_new;
   if DEE_UNLIKELY((new_name = DeeString_Newf("surface_type<%#I64x>",format)) == NULL) goto err_r;
   error = DeeType_SetName((DeeTypeObject *)result,DeeString_STR(new_name));
   Dee_DECREF(new_name);
   if (error != 0) goto err_r;
#if DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
   DeeType_SetStaticDoc((DeeTypeObject *)result,_deesurface_doc);
#endif
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
 if (!result->st_fill        ) result->st_fill         = &_deesurface_generic_st_fill;
 if (!result->st_fillrect    ) result->st_fillrect     = &_deesurface_generic_st_fillrect;
 if (!result->st_xline       ) result->st_xline        = &_deesurface_generic_st_xline;
 if (!result->st_yline       ) result->st_yline        = &_deesurface_generic_st_yline;
 if (!result->st_linellhh    ) result->st_linellhh     = &_deesurface_generic_st_linellhh;
 if (!result->st_linelhhl    ) result->st_linelhhl     = &_deesurface_generic_st_linelhhl;
 if (!result->st_blit        ) result->st_blit         = &_deesurface_generic_st_blit;
 if (!result->st_stretchblit ) result->st_stretchblit  = &_deesurface_generic_st_stretchblit;
 if (!result->st_flipx       ) result->st_flipx        = &_deesurface_generic_st_flipx;
 if (!result->st_flipy       ) result->st_flipy        = &_deesurface_generic_st_flipy;
 if (!result->st_pixelmaskmsb) result->st_pixelmaskmsb = &_deesurface_generic_st_pixelmaskmsb;
 if (!result->st_pixelmasklsb) result->st_pixelmasklsb = &_deesurface_generic_st_pixelmasklsb;
 return result;
}




static DeeSurfaceTypeObject *DEE_CALL _deegfx_surface_format(DeeObject *args) {
 Dee_uint8_t bits; Dee_uint32_t rmask,gmask,bmask,amask = 0;
 Dee_uint64_t format_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,
  "I8uI32uI32uI32u|I32u:surface_format",
  &bits,&rmask,&gmask,&bmask,&amask) != 0) return NULL;
 if (!bits || (bits%8)!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid value for bits: %I8u (expected non-zero multiple of 8)",
                      bits);
  return NULL;
 }
 format_id = DEE_SURFACETYPE_FORMAT_PIXEL_(bits,rmask,gmask,bmask,amask);
 return DeeSurfaceType_Get(format_id);
}
static DeeSurfaceTypeObject *DEE_CALL _deegfx___surface_formatid(DeeObject *args) {
 Dee_uint64_t format_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:__surface_formatid",&format_id) != 0) return NULL;
 return DeeSurfaceType_Get(format_id);
}

//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("pixel",&DeePixel_Type),
 DeeDex_EXPORT_OBJECT("surface",&DeeSurface_Type),
 DeeDex_EXPORT_FUNCTION("surface_format",&_deegfx_surface_format,
  "(uint8_t bits, uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask = 0) -> surface_type\n"
  "@return: The surface-type describing a pixelformat compatible with the given arguments\n"
  "\tNOTE: The mask arguments are written in big-endian, meaning that a mask of 0xff000000 describes the first of 4 bytes"),
 DeeDex_EXPORT_FUNCTION("__surface_formatid",&_deegfx___surface_formatid,"(uint64_t id) -> surface_type"),
 DeeDex_EXPORT_OBJECT("surface_type",&DeeSurfaceType_Type),
 DeeDex_EXPORT_OBJECT("surface_rgba8888",&DeeSurfaceType_RGBA8888),
#ifdef DEE_PLATFORM_WINDOWS
 DeeDex_EXPORT_OBJECT("window",&DeeWindow_Type),
#endif
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
#include "surface.rgba8888.inl"
#endif

#endif /* !GUARD_DEEMON_DEX_GFX__GFX_C */
