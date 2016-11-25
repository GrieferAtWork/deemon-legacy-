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
#ifndef GUARD_DEEMON_DEX_GFX_SURFACE_GENERIC_INL
#define GUARD_DEEMON_DEX_GFX_SURFACE_GENERIC_INL 1

#ifdef __INTELLISENSE__
#include "_gfx.h"
#endif

//////////////////////////////////////////////////////////////////////////
// Generic surface operators (very slow, but work in all situations)
DEE_DECL_BEGIN

void DEE_CALL _deesurface_generic_st_fill(
 DeeSurfaceObject *dst, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t x,sx,y,sy; PDeeSurfaceSetPixel setter;
 sx = dst->s_sizex,sy = dst->s_sizey;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (y = 0; y != sy; ++y) for (x = 0; x != sx; ++x) {
  (*setter)(dst,x,y,color,blend);
 }
}
void DEE_CALL _deesurface_generic_st_fillrect(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t ybegin, Dee_size_t xend,
 Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t x,y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend); DEE_ASSERT(ybegin < yend);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (y = ybegin; y != yend; ++y) for (x = xbegin; x != xend; ++x) {
  (*setter)(dst,x,y,color,blend);
 }
}
void DEE_CALL _deesurface_generic_st_xline(
 DeeSurfaceObject *dst, Dee_size_t xbegin, Dee_size_t xend,
 Dee_size_t y, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t x; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(xbegin < xend);
 x = xbegin; setter = DeeSurface_TYPE(dst)->st_setpixel;
 do (*setter)(dst,x,y,color,blend);
 while (++x != xend);
}
void DEE_CALL _deesurface_generic_st_yline(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t ybegin,
 Dee_size_t yend, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t y; PDeeSurfaceSetPixel setter;
 DEE_ASSERT(ybegin < yend);
 y = ybegin; setter = DeeSurface_TYPE(dst)->st_setpixel;
 do (*setter)(dst,x,y,color,blend);
 while (++y != yend);
}
void DEE_CALL _deesurface_generic_st_linellhh(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex,
 Dee_size_t sizey, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_sizefloat_t relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step;
 DEE_ASSERT(sizex != 0);
 DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 step = 0;
 if (sizex > sizey) {
  relation = (Dee_sizefloat_t)sizey/(Dee_sizefloat_t)sizex;
  do (*setter)(dst,x+step,y+(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (Dee_sizefloat_t)sizex/(Dee_sizefloat_t)sizey;
  do (*setter)(dst,x+(Dee_size_t)(relation*step),y+step,color,blend);
  while (++step != sizey);
 } else {
  do (*setter)(dst,x+step,y+step,color,blend);
  while (++step != sizex);
 }
}
void DEE_CALL _deesurface_generic_st_linelhhl(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y, Dee_size_t sizex,
 Dee_size_t sizey, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_sizefloat_t relation; PDeeSurfaceSetPixel setter;
 Dee_size_t step;
 DEE_ASSERT(sizex != 0),DEE_ASSERT(sizey != 0);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 step = 0;
 if (sizex > sizey) {
  relation = (Dee_sizefloat_t)sizey/(Dee_sizefloat_t)sizex;
  do (*setter)(dst,x+step,y-(Dee_size_t)(relation*step),color,blend);
  while (++step != sizex);
 } else if (sizex < sizey) {
  relation = (Dee_sizefloat_t)sizex/(Dee_sizefloat_t)sizey;
  do (*setter)(dst,x+(Dee_size_t)(relation*step),y-step,color,blend);
  while (++step != sizey);
 } else {
  do (*setter)(dst,x+step,y-step,color,blend);
  while (++step != sizex);
 }
}

void DEE_CALL _deesurface_generic_st_blit(
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
 for (y = 0; y != sy; ++y) for (x = 0; x != sx; ++x) {
  (*getter)(src,src_x+x,src_y+y,&pixel);
  (*setter)(dst,dst_x+x,dst_y+y,&pixel,blend);
 }
}
void DEE_CALL _deesurface_generic_st_stretchblit(
 DeeSurfaceObject *dst, Dee_size_t dst_x, Dee_size_t dst_y, Dee_size_t dstsx, Dee_size_t dstsy,
 DeeSurfaceObject const *src, Dee_sizefloat_t src_x, Dee_sizefloat_t src_y, Dee_sizefloat_t srcsx, Dee_sizefloat_t srcsy,
 Dee_blendinfo_t blend) {
 struct DeePixel pixel; Dee_size_t x,y,srcx,srcy;
 PDeeSurfaceGetPixel getter;
 PDeeSurfaceSetPixel setter;
 Dee_sizefloat_t srcfacx,srcfacy;
 DEE_ASSERT(dst_x+dstsx <= dst->s_sizex); DEE_ASSERT(dstsx != 0);
 DEE_ASSERT(dst_y+dstsy <= dst->s_sizey); DEE_ASSERT(dstsy != 0);
 DEE_ASSERT(src_x >= 0);
 DEE_ASSERT(src_y >= 0);
 DEE_ASSERT((Dee_size_t)(src_x+srcsx) <= src->s_sizex);
 DEE_ASSERT((Dee_size_t)(src_y+srcsy) <= src->s_sizey);
 srcfacx = srcsx/(Dee_sizefloat_t)dstsx;
 srcfacy = srcsy/(Dee_sizefloat_t)dstsy;
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 getter = DeeSurface_TYPE(src)->st_getpixel;
 for (y = 0; y != dstsy; ++y) {
  srcy = (Dee_size_t)(src_y+((Dee_sizefloat_t)y*srcfacy));
  for (x = 0; x != dstsx; ++x) {
   srcx = (Dee_size_t)(src_x+((Dee_sizefloat_t)x*srcfacx));
   (*getter)(src,srcx,srcy,&pixel);
   (*setter)(dst,dst_x+x,dst_y+y,&pixel,blend);
  }
 }
}
void DEE_CALL _deesurface_generic_st_flipx(
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
void DEE_CALL _deesurface_generic_st_flipy(
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
void DEE_CALL _deesurface_generic_st_pixelmaskmsb(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y,
 Dee_size_t sx, Dee_size_t sy, Dee_size_t line_bytes,
 void const *data, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t diff,myy,ix,iy; Dee_uint8_t *line,b;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(sx); DEE_ASSERT(sy);
 DEE_ASSERT(x+sx < dst->s_sizex);
 DEE_ASSERT(y+sy < dst->s_sizey);
 DEE_ASSERT(data); DEE_ASSERT(line_bytes);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (iy = 0; iy != sy; ++iy) {
  line = (Dee_uint8_t *)data+iy*line_bytes;
  myy = y+iy,ix = 0; while (1) {
   b = *line++;
   switch ((diff = (sx-ix))) {
    default: if (b&(1 << 0)) (*setter)(dst,(x+ix)+7,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 7:  if (b&(1 << 1)) (*setter)(dst,(x+ix)+6,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 6:  if (b&(1 << 2)) (*setter)(dst,(x+ix)+5,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 5:  if (b&(1 << 3)) (*setter)(dst,(x+ix)+4,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 4:  if (b&(1 << 4)) (*setter)(dst,(x+ix)+3,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 3:  if (b&(1 << 5)) (*setter)(dst,(x+ix)+2,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 2:  if (b&(1 << 6)) (*setter)(dst,(x+ix)+1,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 1:  if (b&(1 << 7)) (*setter)(dst,(x+ix)+0,myy,color,blend); break;
   }
   if (diff <= 8) break;
   ix += 8;
  }
 }
}
void DEE_CALL _deesurface_generic_st_pixelmasklsb(
 DeeSurfaceObject *dst, Dee_size_t x, Dee_size_t y,
 Dee_size_t sx, Dee_size_t sy, Dee_size_t line_bytes,
 void const *data, struct DeePixel const *color, Dee_blendinfo_t blend) {
 Dee_size_t diff,myy,ix,iy; Dee_uint8_t *line,b;
 PDeeSurfaceSetPixel setter;
 DEE_ASSERT(sx); DEE_ASSERT(sy);
 DEE_ASSERT(x+sx < dst->s_sizex);
 DEE_ASSERT(y+sy < dst->s_sizey);
 DEE_ASSERT(data); DEE_ASSERT(line_bytes);
 setter = DeeSurface_TYPE(dst)->st_setpixel;
 for (iy = 0; iy != sy; ++iy) {
  line = (Dee_uint8_t *)data+iy*line_bytes;
  myy = y+iy,ix = 0; while (1) {
   b = *line++;
   switch ((diff = (sx-ix))) {
    default: if (b&(1 << 7)) (*setter)(dst,(x+ix)+7,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 7:  if (b&(1 << 6)) (*setter)(dst,(x+ix)+6,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 6:  if (b&(1 << 5)) (*setter)(dst,(x+ix)+5,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 5:  if (b&(1 << 4)) (*setter)(dst,(x+ix)+4,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 4:  if (b&(1 << 3)) (*setter)(dst,(x+ix)+3,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 3:  if (b&(1 << 2)) (*setter)(dst,(x+ix)+2,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 2:  if (b&(1 << 1)) (*setter)(dst,(x+ix)+1,myy,color,blend); DEE_ATTRIBUTE_FALLTHROUGH
    case 1:  if (b&(1 << 0)) (*setter)(dst,(x+ix)+0,myy,color,blend); break;
   }
   if (diff <= 8) break;
   ix += 8;
  }
 }
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_SURFACE_GENERIC_INL */
