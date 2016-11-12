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
#ifndef GUARD_DEEMON_DEX_GFX_SURFACE_RGBA8888_INL
#define GUARD_DEEMON_DEX_GFX_SURFACE_RGBA8888_INL 1

#ifdef __INTELLISENSE__
#include "_gfx.c"
#endif

DEE_DECL_BEGIN

#define DeeSurface_RGBA8888_PIXELADDR(dst,x,y)\
 ((struct DeePixel *)((dst)->s_pixeldata.s_pixels+((x)*4+(y)*(dst)->s_pixeldata.s_scanline)))


static void DEE_CALL _deesurface_rgba8888_st_getpixel(
 DeeSurfaceObject const *dst, Dee_size_t x, Dee_size_t y, struct DeePixel *pixel) {
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 *pixel = *DeeSurface_RGBA8888_PIXELADDR(dst,x,y);
}
static void DEE_CALL _deesurface_rgba8888_st_setpixel(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend) {
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 DeePixel_Blend(DeeSurface_RGBA8888_PIXELADDR(dst,x,y),pixel,blend);
}
static void DEE_CALL _deesurface_rgba8888_st_fill(
 DeeSurfaceObject *dst, struct DeePixel const *color, DEE_A_IN Dee_blendinfo_t blend) {
 struct DeePixel *iter,*end;
 end = (iter = (struct DeePixel *)dst->s_pixeldata.s_pixels)+(dst->s_sizex*dst->s_sizey);
 do DeePixel_Blend(iter,color,blend); while (++iter != end);
}
static void DEE_CALL _deesurface_rgba8888_st_fillrect(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend,
 Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend) {
 struct DeePixel *iter,*end,*aligned_linestart; Dee_size_t y,sizex,scanline;
 DEE_ASSERT(xbegin < xend); DEE_ASSERT(ybegin < yend);
 sizex = xend-xbegin,scanline = dst->s_pixeldata.s_scanline;
 aligned_linestart = ((struct DeePixel *)dst->s_pixeldata.s_pixels)+xbegin;
 for (y = ybegin; y != yend; ++y) {
  end = (iter = aligned_linestart)+(y*scanline);
  do DeePixel_Blend(iter,color,blend); while (++iter != end);
 }
}
static void DEE_CALL _deesurface_rgba8888_st_xline(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t xend,
 Dee_size_t y, struct DeePixel const *color, Dee_blendinfo_t blend) {
 struct DeePixel *iter,*end;
 DEE_ASSERT(xbegin < xend);
 DEE_ASSERT(xbegin < dst->s_sizex);
 DEE_ASSERT(y < dst->s_sizey);
 end = (iter = DeeSurface_RGBA8888_PIXELADDR(dst,xbegin,y))+(xend-xbegin);
 do DeePixel_Blend(iter,color,blend); while (++iter != end);
}
static void DEE_CALL _deesurface_rgba8888_st_yline(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t ybegin,
 Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend) {
 struct DeePixel *iter,*end; Dee_size_t scanline;
 DEE_ASSERT(ybegin < yend);
 DEE_ASSERT(ybegin < dst->s_sizey);
 DEE_ASSERT(x < dst->s_sizex);
 scanline = dst->s_pixeldata.s_scanline;
 iter = DeeSurface_RGBA8888_PIXELADDR(dst,x,ybegin);
 end = (struct DeePixel *)((Dee_uintptr_t)iter+(scanline*(yend-ybegin)));
 do DeePixel_Blend(iter,color,blend);
 while ((struct DeePixel *)(*(Dee_uintptr_t *)&iter += scanline) != end);
}
static void DEE_CALL _deesurface_rgba8888_st_blit(
 DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y,
 DeeSurfaceObject const *src, Dee_size_t src_x, Dee_size_t src_y,
 Dee_size_t sx, Dee_size_t sy, Dee_blendinfo_t blend) {
 struct DeePixel pixel,*begin,*iter; Dee_size_t x,y;
 PDeeSurfaceGetPixel getter;
 DEE_ASSERT(dst_x+sx <= dst->s_sizex);
 DEE_ASSERT(dst_y+sy <= dst->s_sizey);
 DEE_ASSERT(src_x+sx <= src->s_sizex);
 DEE_ASSERT(src_y+sy <= src->s_sizey);
 DEE_ASSERT(sx != 0);
 DEE_ASSERT(sy != 0);
 getter = DeeSurface_TYPE(src)->st_getpixel;
 begin = DeeSurface_RGBA8888_PIXELADDR(dst,dst_x,dst_y);
 for (y = 0; y != sy; ++y) {
  for (iter = begin,x = 0; x != sx; ++x) {
   (*getter)(src,x,y,&pixel);
   DeePixel_Blend(iter++,&pixel,blend);
  }
  *(Dee_uintptr_t *)&begin += dst->s_pixeldata.s_scanline;
 }
}



// TODO: Optimized versions for these
#define _deesurface_rgba8888_st_linellhh     _deesurface_pixel_st_linellhh
#define _deesurface_rgba8888_st_linelhhl     _deesurface_pixel_st_linelhhl
#define _deesurface_rgba8888_st_stretchblit  _deesurface_pixel_st_stretchblit
#define _deesurface_rgba8888_st_flipx        _deesurface_pixel_st_flipx
#define _deesurface_rgba8888_st_flipy        _deesurface_pixel_st_flipy
#define _deesurface_rgba8888_st_pixelmaskmsb _deesurface_pixel_st_pixelmaskmsb
#define _deesurface_rgba8888_st_pixelmasklsb _deesurface_pixel_st_pixelmasklsb


DeeSurfaceTypeObject DeeSurfaceType_RGBA8888 = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeSurfaceType_Type),member("surface_rgba8888"),
   member(_deesurface_doc),
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
 DeeSurfaceTypeFormat_RGBA8888, // st_format
 NULL,            // st_next
 {DeeSurfacePixelSpec_INIT32(
   (0xFF000000,24,8),
   (0x00FF0000,16,8),
   (0x0000FF00, 8,8),
   (0x000000FF, 0,8))},
 &_deesurface_rgba8888_st_getpixel,     // st_getpixel
 &_deesurface_rgba8888_st_setpixel,     // st_setpixel
 &_deesurface_rgba8888_st_fill,         // st_fill
 &_deesurface_rgba8888_st_fillrect,     // st_fillrect
 &_deesurface_rgba8888_st_xline,        // st_xline
 &_deesurface_rgba8888_st_yline,        // st_yline
 &_deesurface_rgba8888_st_linellhh,     // st_linellhh
 &_deesurface_rgba8888_st_linelhhl,     // st_linelhhl
 &_deesurface_rgba8888_st_blit,         // st_blit
 &_deesurface_rgba8888_st_stretchblit,  // st_stretchblit
 &_deesurface_rgba8888_st_flipx,        // st_flipx
 &_deesurface_rgba8888_st_flipy,        // st_flipy
 &_deesurface_rgba8888_st_pixelmaskmsb, // st_pixelmaskmsb
 &_deesurface_rgba8888_st_pixelmasklsb, // st_pixelmasklsb
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_SURFACE_RGBA8888_INL */
