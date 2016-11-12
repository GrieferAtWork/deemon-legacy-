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
#ifndef GUARD_DEEMON_DEX_GFX_PIXEL_INL
#define GUARD_DEEMON_DEX_GFX_PIXEL_INL 1
#ifdef __DEEMON__
#pragma warning(disable: 16)
#endif

#include "_gfx.h"
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/std/string.h>

DEE_DECL_BEGIN
DEE_OBJECTPOOL_DEFINE(pixelpool,DeePixelObject)


static struct DeeMemberDef const _deepixel_tp_class_members[] = {
/*[[[deemon
#include <file>
for (local name,r,g,b,a: COLLECT_PIXEL_NAMES()) {
  print " DEE_MEMBERDEF_CONST_v100("+repr name.lower()+",object,&DeePixel_"+name.capitalize()+"Object),";
}
]]]*/
 DEE_MEMBERDEF_CONST_v100("aqua",object,&DeePixel_AquaObject),
 DEE_MEMBERDEF_CONST_v100("black",object,&DeePixel_BlackObject),
 DEE_MEMBERDEF_CONST_v100("blue",object,&DeePixel_BlueObject),
 DEE_MEMBERDEF_CONST_v100("empty",object,&DeePixel_EmptyObject),
 DEE_MEMBERDEF_CONST_v100("fuchsia",object,&DeePixel_FuchsiaObject),
 DEE_MEMBERDEF_CONST_v100("gray",object,&DeePixel_GrayObject),
 DEE_MEMBERDEF_CONST_v100("green",object,&DeePixel_GreenObject),
 DEE_MEMBERDEF_CONST_v100("lime",object,&DeePixel_LimeObject),
 DEE_MEMBERDEF_CONST_v100("maroon",object,&DeePixel_MaroonObject),
 DEE_MEMBERDEF_CONST_v100("navy",object,&DeePixel_NavyObject),
 DEE_MEMBERDEF_CONST_v100("olive",object,&DeePixel_OliveObject),
 DEE_MEMBERDEF_CONST_v100("purple",object,&DeePixel_PurpleObject),
 DEE_MEMBERDEF_CONST_v100("red",object,&DeePixel_RedObject),
 DEE_MEMBERDEF_CONST_v100("silver",object,&DeePixel_SilverObject),
 DEE_MEMBERDEF_CONST_v100("teal",object,&DeePixel_TealObject),
 DEE_MEMBERDEF_CONST_v100("white",object,&DeePixel_WhiteObject),
 DEE_MEMBERDEF_CONST_v100("yellow",object,&DeePixel_YellowObject),
//[[[end]]]
 DEE_MEMBERDEF_END_v100
};


DEE_A_RET_Z_OPT char const *DeePixel_Name(
 DEE_A_IN struct DeePixel const *pixel) {
 struct DeeMemberDef const *iter;
 iter = _deepixel_tp_class_members;
 while (iter->d_name) {
  if ((iter->d_type&(DeeMemberType_CONST|DeeMemberType_object))==
                    (DeeMemberType_CONST|DeeMemberType_object)) {
   DeePixelObject *constant_pixel;
   constant_pixel = (DeePixelObject *)(void *)iter->d_offset;
   DEE_ASSERT(DeeObject_Check(constant_pixel));
   if (DeePixel_Check(constant_pixel) &&
       DeePixel_EQUALS(&constant_pixel->p_pixel,pixel)
       ) return iter->d_name;
  }
  ++iter;
 }
 return NULL;
}

DEE_A_RET_MAYBE_NULL struct DeePixel const *
DeePixel_FromName(DEE_A_IN_Z char const *name) {
 struct DeeMemberDef const *iter;
 iter = _deepixel_tp_class_members;
 while (iter->d_name) {
  if ((iter->d_type&(DeeMemberType_CONST|DeeMemberType_object))==
                    (DeeMemberType_CONST|DeeMemberType_object) &&
      strcmp(iter->d_name,name) == 0) {
   DeePixelObject *constant_pixel;
   constant_pixel = (DeePixelObject *)(void *)iter->d_offset;
   DEE_ASSERT(DeeObject_Check(constant_pixel));
   if (DeePixel_Check(constant_pixel)) return &constant_pixel->p_pixel;
  }
  ++iter;
 }
 return NULL;
}


DEE_A_RET_EXCEPT_REF DeePixelObject *DeePixel_New(
 DEE_A_IN struct DeePixel const *pixel) {
 DeePixelObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(pixelpool)) != NULL) {
  DeeObject_INIT(result,&DeePixel_Type);
  result->p_pixel = *pixel;
 }
 return result;
}

                     
/*[[[deemon
#include <file>
for (local name,r,g,b,a: COLLECT_PIXEL_NAMES()) {
  print "DeePixelObject DeePixel_"+name.capitalize()+"Object = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x"+r+",0x"+g+",0x"+b+",0x"+a+"}};";
}
]]]*/
DeePixelObject DeePixel_AquaObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0xff,0xff,0xff}};
DeePixelObject DeePixel_BlackObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0x00,0x00,0xff}};
DeePixelObject DeePixel_BlueObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0x00,0xff,0xff}};
DeePixelObject DeePixel_EmptyObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xff,0xff,0xff,0x00}};
DeePixelObject DeePixel_FuchsiaObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xff,0x00,0xff,0xff}};
DeePixelObject DeePixel_GrayObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x80,0x80,0x80,0xff}};
DeePixelObject DeePixel_GreenObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0x80,0x00,0xff}};
DeePixelObject DeePixel_LimeObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0xff,0x00,0xff}};
DeePixelObject DeePixel_MaroonObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x80,0x00,0x00,0xff}};
DeePixelObject DeePixel_NavyObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0x00,0x80,0xff}};
DeePixelObject DeePixel_OliveObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x80,0x80,0x00,0xff}};
DeePixelObject DeePixel_PurpleObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x80,0x00,0x80,0xff}};
DeePixelObject DeePixel_RedObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xff,0x00,0x00,0xff}};
DeePixelObject DeePixel_SilverObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xc0,0xc0,0xc0,0xff}};
DeePixelObject DeePixel_TealObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0x00,0x80,0x80,0xff}};
DeePixelObject DeePixel_WhiteObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xff,0xff,0xff,0xff}};
DeePixelObject DeePixel_YellowObject = {DEE_OBJECT_HEAD_INIT(&DeePixel_Type),{0xff,0xff,0x00,0xff}};
//[[[end]]]


static DeePixelObject *DEE_CALL _deepixel_tp_alloc(
 DeeTypeObject *DEE_UNUSED(tp_self)) {
 DeePixelObject *result = DEE_OBJECTPOOL_ALLOC(pixelpool);
 if DEE_LIKELY(result) DeeObject_INIT(result,&DeePixel_Type);
 return result;
}
static void DEE_CALL _deepixel_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePixelObject *self) {
 DEE_OBJECTPOOL_FREE(pixelpool,self);
}
static int DEE_CALL _deepixel_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePixelObject *self, DeeObject *args) {
 if (DeeTuple_SIZE(args) == 1) {
  struct DeePixel const *named_pixel;
  DeeObject *pixel_name = DeeString_Cast(DeeTuple_GET(args,0));
  if DEE_UNLIKELY(!pixel_name) return -1;
  named_pixel = DeePixel_FromName(DeeString_STR(pixel_name));
  if (!named_pixel) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Unknown pixel name: %q",
                       DeeString_STR(pixel_name));
   Dee_DECREF(pixel_name);
   return -1;
  }
  Dee_DECREF(pixel_name);
  self->p_pixel = *named_pixel;
  return 0;
 }
 self->p_pixel.a = 0xFF;
 return DeeTuple_Unpack(args,"I8uI8uI8u|I8u:pixel",
                        &self->p_pixel.r,&self->p_pixel.g,
                        &self->p_pixel.b,&self->p_pixel.a);
}
static DeeObject *DEE_CALL _deepixel_tp_str(DeePixelObject *self) {
 char const *pixelname;
 if ((pixelname = DeePixel_Name(&self->p_pixel)) != NULL)
  return DeeString_New(pixelname);
 return DeeString_Newf("#%.2I8x%.2I8x%.2I8x%.2I8x",
                       self->p_pixel.r,self->p_pixel.g,
                       self->p_pixel.b,self->p_pixel.a);
}
static DeeObject *DEE_CALL _deepixel_tp_repr(DeePixelObject *self) {
 char const *pixelname;
 if ((pixelname = DeePixel_Name(&self->p_pixel)) != NULL)
  return DeeString_Newf("pixel(%q)",pixelname);
 return DeeString_Newf("pixel(%#.2I8x,%#.2I8x,%#.2I8x,%#.2I8x)",
                       self->p_pixel.r,self->p_pixel.g,
                       self->p_pixel.b,self->p_pixel.a);
}

static struct DeeMemberDef const _deepixel_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("r",DeePixelObject,p_pixel.r,Dee_uint8_t),
 DEE_MEMBERDEF_NAMED_RO_v100("g",DeePixelObject,p_pixel.g,Dee_uint8_t),
 DEE_MEMBERDEF_NAMED_RO_v100("b",DeePixelObject,p_pixel.b,Dee_uint8_t),
 DEE_MEMBERDEF_NAMED_RO_v100("a",DeePixelObject,p_pixel.a,Dee_uint8_t),
 DEE_MEMBERDEF_END_v100
};


DeeTypeObject DeePixel_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("pixel"),null,
  member(DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_DONT_COPY|
         DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePixelObject),
  member(&_deepixel_tp_alloc),null,null,null,
  member(&_deepixel_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deepixel_tp_free),null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deepixel_tp_str),
  member(&_deepixel_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deepixel_tp_members),null,null,
  member(_deepixel_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_PIXEL_INL */
