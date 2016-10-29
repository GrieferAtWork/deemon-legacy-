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
#define DEE_LIMITED_API 1
#include "float.c"
#include DEE_INCLUDE_MEMORY_API()
#endif


#if defined(__INTELLISENSE__)
#define KIND 0
#elif !defined(KIND)
#error Must define 'KIND' before including this file
#endif

DEE_DECL_BEGIN

#if KIND == 0
#define F_NAME      "float"
#define F_SIZE      DEE_TYPES_SIZEOF_FLOAT
#define ID          DEE_CONFIG_RT_FLOATID_FLOAT
#define float_t     Dee_rt_float
#define DeeFObject  DeeFloatObject
#define DeeF_Type   DeeFloat_Type
#define DeeF_New    DeeFloat_New
#define DeeF_Check  DeeFloat_Check
#define DeeF_VALUE  DeeFloat_VALUE
#define DeeF_Value  DeeFloat_Value
#define SLOT(name)  _deefloat_##name
#elif KIND == 1
#define F_NAME      "double"
#define F_SIZE      DEE_TYPES_SIZEOF_DOUBLE
#define ID          DEE_CONFIG_RT_FLOATID_DOUBLE
#define float_t     Dee_rt_double
#define DeeFObject  DeeDoubleObject
#define DeeF_Type   DeeDouble_Type
#define DeeF_New    DeeDouble_New
#define DeeF_Check  DeeDouble_Check
#define DeeF_VALUE  DeeDouble_VALUE
#define DeeF_Value  DeeDouble_Value
#define SLOT(name)  _deedouble_##name
#elif KIND == 2
#define F_NAME      "long double"
#define F_SIZE      DEE_TYPES_SIZEOF_LDOUBLE
#define ID          DEE_CONFIG_RT_FLOATID_LDOUBLE
#define float_t     Dee_rt_ldouble
#define DeeFObject  DeeLDoubleObject
#define DeeF_Type   DeeLDouble_Type
#define DeeF_New    DeeLDouble_New
#define DeeF_Check  DeeLDouble_Check
#define DeeF_VALUE  DeeLDouble_VALUE
#define DeeF_Value  DeeLDouble_Value
#define SLOT(name)  _deeldouble_##name
#endif

#if KIND == 0
#define POOL_NAME float_pool
#elif KIND == 1
#define POOL_NAME double_pool
#elif KIND == 2
#define POOL_NAME ldouble_pool
#endif
DEE_OBJECTPOOL_DEFINE(POOL_NAME,DeeFObject)

#if defined(DEE_TYPES_FLOATID_FLOAT) && ID == DEE_TYPES_FLOATID_FLOAT
#define _Dee_wstof        DeeWideString_ToFloatWithLength
#define _Dee_8stof        DeeUtf8String_ToFloatWithLength
#define _Dee_16stof       DeeUtf16String_ToFloatWithLength
#define _Dee_32stof       DeeUtf32String_ToFloatWithLength
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && ID == DEE_TYPES_FLOATID_DOUBLE
#define _Dee_wstof        DeeWideString_ToDoubleWithLength
#define _Dee_8stof        DeeUtf8String_ToDoubleWithLength
#define _Dee_16stof       DeeUtf16String_ToDoubleWithLength
#define _Dee_32stof       DeeUtf32String_ToDoubleWithLength
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && ID == DEE_TYPES_FLOATID_LDOUBLE
#define _Dee_wstof        DeeWideString_ToLDoubleWithLength
#define _Dee_8stof        DeeUtf8String_ToLDoubleWithLength
#define _Dee_16stof       DeeUtf16String_ToLDoubleWithLength
#define _Dee_32stof       DeeUtf32String_ToLDoubleWithLength
#endif


static DeeFObject *SLOT(tp_alloc)(DeeTypeObject *tp) {
 DeeFObject *result = DEE_OBJECTPOOL_ALLOC(POOL_NAME);
 if (result) DeeObject_INIT(result,tp);
 return result;
}
static void SLOT(tp_free)(DeeTypeObject *DEE_UNUSED(tp), DeeFObject *ob) {
 DEE_OBJECTPOOL_FREE(POOL_NAME,ob);
}


DEE_A_RET_OBJECT_EXCEPT_REF(obT) *DeeF_New(DEE_A_IN float_t x) {
 DeeFObject *result;
 if ((result = DEE_OBJECTPOOL_ALLOC(POOL_NAME)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeF_Type);
  result->ob_val = x;
 }
 return (DeeObject *)result;
}

DEE_A_RET_WUNUSED float_t DeeF_Value(DEE_A_IN_OBJECT(DeeFObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
 return DeeF_VALUE(self);
}


//////////////////////////////////////////////////////////////////////////
// Constructors
static int SLOT(tp_copy_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFObject *self, DeeFObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeF_Check(right));
 self->ob_val = right->ob_val;
 return 0;
}
static int SLOT(tp_any_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFObject *self, DeeTupleObject *args) {
 DeeObject *right = NULL;
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
 if (DeeTuple_Unpack((DeeObject *)args,"|o:" F_NAME,&right) != 0) return -1;
 if (!right) return 0;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return _Dee_wstof(DeeWideString_SIZE(right),DeeWideString_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return _Dee_8stof(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return _Dee_16stof(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return _Dee_32stof(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_Cast(float_t,right,&self->ob_val);
}
static int SLOT(tp_copy_assign)(DeeFObject *self, DeeFObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeF_Check(right));
 self->ob_val = right->ob_val;
 return 0;
}
static int SLOT(tp_any_assign)(DeeFObject *self, DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return _Dee_wstof(DeeWideString_SIZE(right),DeeWideString_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return _Dee_8stof(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return _Dee_16stof(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return _Dee_32stof(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&self->ob_val);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_Cast(float_t,right,&self->ob_val);
}
static int SLOT(tp_p_any_ctor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeTupleObject *args) {
 DeeObject *right;
 if (DeeTuple_Unpack((DeeObject *)args,"o:" DEE_PP_STR(float_t),&right) != 0) return -1;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return _Dee_wstof(DeeWideString_SIZE(right),DeeWideString_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return _Dee_8stof(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return _Dee_16stof(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return _Dee_32stof(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_Cast(float_t,right,self);
}
static int SLOT(tp_p_any_assign)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(right));
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return _Dee_wstof(DeeWideString_SIZE(right),DeeWideString_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return _Dee_8stof(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return _Dee_16stof(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return _Dee_32stof(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_Cast(float_t,right,self);
}


//////////////////////////////////////////////////////////////////////////
// Cast VTable
static DeeObject *SLOT(tp_str)(DeeFObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeF_Check(self));
#if KIND == 0
 return DeeString_FromFloat(self->ob_val);
#elif KIND == 1
 return DeeString_FromDouble(self->ob_val);
#elif KIND == 2
 return DeeString_FromLDouble(self->ob_val);
#endif
}
static DeeObject *SLOT(tp_p_str)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self) {
#if KIND == 0
 return DeeString_FromFloat(*self);
#elif KIND == 1
 return DeeString_FromDouble(*self);
#elif KIND == 2
 return DeeString_FromLDouble(*self);
#endif
}
static int SLOT(tp_int32)(DeeFObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)self->ob_val; return 0; }
static int SLOT(tp_int64)(DeeFObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)self->ob_val; return 0; }
static int SLOT(tp_double)(DeeFObject *self, double *result) { *result = (double)self->ob_val; return 0; }
static int SLOT(tp_p_int32)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)*self; return 0; }
static int SLOT(tp_p_int64)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)*self; return 0; }
static int SLOT(tp_p_double)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, double *result) { *result = (double)*self; return 0; }



//////////////////////////////////////////////////////////////////////////
// Math VTable
static DeeObject *SLOT(tp_not)(DeeFObject *self) { DeeReturn_Bool(self->ob_val == (float_t)0.0); }
static DeeObject *SLOT(tp_p_not)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self) { DeeReturn_Bool(*self == (float_t)0.0); }
static int SLOT(tp_bool)(DeeFObject *self) { return self->ob_val != (float_t)0.0; }
static int SLOT(tp_p_bool)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self) { return *self != (float_t)0.0; }
static DeeObject *SLOT(tp_neg)(DeeFObject *self) { return DeeF_New(-self->ob_val); }
static DeeObject *SLOT(tp_p_pos)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self) { return DeeF_New(*self); }
static DeeObject *SLOT(tp_p_neg)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self) { return DeeF_New(-*self); }
static DeeObject *SLOT(tp_add)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(self->ob_val+temp);
}
static DeeObject *SLOT(tp_p_add)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(*self+temp);
}
static DeeObject *SLOT(tp_iadd)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 self->ob_val += temp;
 DeeReturn_NEWREF(self);
}
static int SLOT(tp_p_iadd)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return -1;
 *self += temp;
 return 0;
}
static DeeObject *SLOT(tp_sub)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(self->ob_val-temp);
}
static DeeObject *SLOT(tp_p_sub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(*self-temp);
}
static DeeObject *SLOT(tp_isub)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 self->ob_val -= temp;
 DeeReturn_NEWREF(self);
}
static int SLOT(tp_p_isub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return -1;
 *self -= temp;
 return 0;
}
static DeeObject *SLOT(tp_mul)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(self->ob_val*temp);
}
static DeeObject *SLOT(tp_p_mul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(*self*temp);
}
static DeeObject *SLOT(tp_imul)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 self->ob_val *= temp;
 DeeReturn_NEWREF(self);
}
static int SLOT(tp_p_imul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return -1;
 *self *= temp;
 return 0;
}
static DeeObject *SLOT(tp_div)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(self->ob_val/temp);
}
static DeeObject *SLOT(tp_p_div)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 return DeeF_New(*self/temp);
}
static DeeObject *SLOT(tp_idiv)(DeeFObject *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL;
 self->ob_val /= temp;
 DeeReturn_NEWREF(self);
}
static int SLOT(tp_p_idiv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t temp;
 if (DeeObject_Cast(float_t,right,&temp) != 0) return -1;
 *self /= temp;
 return 0;
}
#if ID == DEE_TYPES_FLOATID_FLOAT
#define NAME(x) x##f
#elif ID == DEE_TYPES_FLOATID_DOUBLE
#define NAME(x) x
#elif ID == DEE_TYPES_FLOATID_LDOUBLE
#define NAME(x) x##l
#endif

#ifdef NAME
#define pow_func NAME(Dee_m_pow)
#else
#define pow_func(x,y) (float_t)Dee_m_pow((double)(x),(double)(y))
#endif
static DeeObject *SLOT(tp_pow)(DeeFObject *self, DeeObject *right) {
 float_t y;
 if (DeeObject_Cast(float_t,right,&y) != 0) return NULL;
 return DeeF_New(pow_func(self->ob_val,y));
}
static DeeObject *SLOT(tp_p_pow)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t y;
 if (DeeObject_Cast(float_t,right,&y) != 0) return NULL;
 return DeeF_New(pow_func(*self,y));
}
static DeeObject *SLOT(tp_ipow)(DeeFObject *self, DeeObject *right) {
 float_t y;
 if (DeeObject_Cast(float_t,right,&y) != 0) return NULL;
 self->ob_val = pow_func(self->ob_val,y);
 DeeReturn_NEWREF(self);
}
static int SLOT(tp_p_ipow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) {
 float_t y;
 if (DeeObject_Cast(float_t,right,&y) != 0) return -1;
 *self = pow_func(*self,y);
 return 0;
}
#undef pow_func


//////////////////////////////////////////////////////////////////////////
// Compare functions
static DeeObject *SLOT(tp_cmp_lo)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val < temp); }
static DeeObject *SLOT(tp_cmp_le)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val <= temp); }
static DeeObject *SLOT(tp_cmp_eq)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val == temp); }
static DeeObject *SLOT(tp_cmp_ne)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val != temp); }
static DeeObject *SLOT(tp_cmp_gr)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val > temp); }
static DeeObject *SLOT(tp_cmp_ge)(DeeFObject *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(self->ob_val >= temp); }
static DeeObject *SLOT(tp_p_cmp_lo)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self < temp); }
static DeeObject *SLOT(tp_p_cmp_le)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self <= temp); }
static DeeObject *SLOT(tp_p_cmp_eq)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self == temp); }
static DeeObject *SLOT(tp_p_cmp_ne)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self != temp); }
static DeeObject *SLOT(tp_p_cmp_gr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self > temp); }
static DeeObject *SLOT(tp_p_cmp_ge)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self, DeeObject *right) { float_t temp; if (DeeObject_Cast(float_t,right,&temp) != 0) return NULL; DeeReturn_Bool(*self >= temp); }
static int SLOT(tp_hash)(DeeFObject *self, Dee_hash_t start, Dee_hash_t *result) {
 *result = DeeHash_Memory(&self->ob_val,sizeof(self->ob_val),start);
 return 0;
}
static int SLOT(tp_p_hash)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), float_t *self,
 Dee_hash_t start, Dee_hash_t *result) {
 *result = DeeHash_Memory(self,sizeof(*self),start);
 return 0;
}

#if defined(DEE_MARSHALTYPE_FLOAT4_ID) && DEE_MARSHALTYPE_FLOAT4_ID == ID && F_SIZE == 4
# define MARSHAL_ID DEE_MARSHALTYPEID_float4
#elif defined(DEE_MARSHALTYPE_FLOAT8_ID) && DEE_MARSHALTYPE_FLOAT8_ID == ID && F_SIZE == 8
# define MARSHAL_ID DEE_MARSHALTYPEID_float8
#elif defined(DEE_MARSHALTYPE_FLOAT12_ID) && DEE_MARSHALTYPE_FLOAT12_ID == ID && F_SIZE == 12
# define MARSHAL_ID DEE_MARSHALTYPEID_float12
#elif defined(DEE_MARSHALTYPE_FLOAT16_ID) && DEE_MARSHALTYPE_FLOAT17_ID == ID && F_SIZE == 16
# define MARSHAL_ID DEE_MARSHALTYPEID_float16
#endif

#ifdef MARSHAL_ID
static int DEE_CALL SLOT(tp_marshal_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 return DeeFile_ReadAll(infile,&self->ob_val,sizeof(self->ob_val));
}
static int DEE_CALL SLOT(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return DeeFile_WriteAll(outfile,&self->ob_val,sizeof(self->ob_val));
}
static struct DeeTypeMarshal SLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(MARSHAL_ID),
 member(&SLOT(tp_marshal_ctor)),member(&SLOT(tp_marshal_put)));
#endif


#ifdef NAME
DEE_COMPILER_MSVC_WARNING_PUSH(4756)
static DeeObject *SLOT(class_inf)(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeF_New(NAME(DEE_M_INFINITY)); }
static DeeObject *SLOT(class_nan)(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeF_New(NAME(DEE_M_NAN)); }
static DeeObject *SLOT(class_huge)(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeF_New(NAME(DEE_M_HUGE_VAL)); }
static DeeObject *SLOT(class_e)(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeF_New(NAME(DEE_M_E)); }
static DeeObject *SLOT(class_pi)(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeF_New(NAME(DEE_M_PI)); }
DEE_COMPILER_MSVC_WARNING_POP
static struct DeeGetSetDef const SLOT(tp_class_getsets)[] = {
 DEE_GETSETDEF_CONST_v100("INF",member(&SLOT(class_inf)),null,null,"-> " F_NAME),
 DEE_GETSETDEF_CONST_v100("NAN",member(&SLOT(class_nan)),null,null,"-> " F_NAME),
 DEE_GETSETDEF_CONST_v100("HUGE",member(&SLOT(class_huge)),null,null,"-> " F_NAME),
 DEE_GETSETDEF_CONST_v100("E",member(&SLOT(class_e)),null,null,"-> " F_NAME),
 DEE_GETSETDEF_CONST_v100("PI",member(&SLOT(class_pi)),null,null,"-> " F_NAME),
 DEE_GETSETDEF_END_v100
};
#endif

#if KIND == 0
DeeStructuredTypeObject DeeFloat_Type = 
#elif KIND == 1
DeeStructuredTypeObject DeeDouble_Type = 
#elif KIND == 2
DeeStructuredTypeObject DeeLDouble_Type = 
#endif
{
 {// tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),member(F_NAME),null,
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DeeStructured_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFObject),
   member(&SLOT(tp_alloc)),null,
   member(&SLOT(tp_copy_ctor)),null,
   member(&SLOT(tp_any_ctor))),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(
   member(&SLOT(tp_free)),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&SLOT(tp_copy_assign)),null,
   member(&SLOT(tp_any_assign))),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&SLOT(tp_str)),
   member(&SLOT(tp_str)),
   member(&SLOT(tp_int32)),
   member(&SLOT(tp_int64)),
   member(&SLOT(tp_double))),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&SLOT(tp_not)),
   member(&SLOT(tp_bool)),null,
   member(&_DeeGeneric_ReturnSelf),
   member(&SLOT(tp_neg)),null,null,null,null,
   member(&SLOT(tp_add)),member(&SLOT(tp_iadd)),
   member(&SLOT(tp_sub)),member(&SLOT(tp_isub)),
   member(&SLOT(tp_mul)),member(&SLOT(tp_imul)),
   member(&SLOT(tp_div)),member(&SLOT(tp_idiv)),
   null,null,null,null,null,null,null,null,null,null,null,null,
   member(&SLOT(tp_pow)),member(&SLOT(tp_ipow)),
   member(&SLOT(tp_hash))),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&SLOT(tp_cmp_lo)),member(&SLOT(tp_cmp_le)),
   member(&SLOT(tp_cmp_eq)),member(&SLOT(tp_cmp_ne)),
   member(&SLOT(tp_cmp_gr)),member(&SLOT(tp_cmp_ge))),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
#ifdef NAME
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   null,null,null,null,member(SLOT(tp_class_getsets)),null),
#else
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   null,null,null,null,null,null),
#endif
#ifdef MARSHAL_ID
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&SLOT(tp_marshal))),
  DEE_TYPE_OBJECT_FOOTER_v101
#undef MARSHAL_ID
#else
  DEE_TYPE_OBJECT_MARSHAL_v101(null),
  DEE_TYPE_OBJECT_FOOTER_v101
#endif
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeFObject)-sizeof(DeeObject)),
  member(&SLOT(tp_p_any_ctor))),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&SLOT(tp_p_any_assign))),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&SLOT(tp_p_str)),
  member(&SLOT(tp_p_str)),
  member(&SLOT(tp_p_int32)),
  member(&SLOT(tp_p_int64)),
  member(&SLOT(tp_p_double))),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&SLOT(tp_p_not)),member(&SLOT(tp_p_bool)),null,
  member(&SLOT(tp_p_pos)),member(&SLOT(tp_p_neg)),null,null,null,null,
  member(&SLOT(tp_p_add)),member(&SLOT(tp_p_iadd)),
  member(&SLOT(tp_p_sub)),member(&SLOT(tp_p_isub)),
  member(&SLOT(tp_p_mul)),member(&SLOT(tp_p_imul)),
  member(&SLOT(tp_p_div)),member(&SLOT(tp_p_idiv)),
  null,null,null,null,null,null,
  null,null,null,null,null,null,
  member(&SLOT(tp_p_pow)),member(&SLOT(tp_p_ipow)),
  member(&SLOT(tp_p_hash))),
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&SLOT(tp_p_cmp_lo)),member(&SLOT(tp_p_cmp_le)),
  member(&SLOT(tp_p_cmp_eq)),member(&SLOT(tp_p_cmp_ne)),
  member(&SLOT(tp_p_cmp_gr)),member(&SLOT(tp_p_cmp_ge))),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(
  null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};
#undef NAME


#ifndef __INTELLISENSE__
#undef POOL_NAME
#undef _DeeFPool_Malloc
#undef _DeeFPool_Free
#undef _Dee_wstof
#undef _Dee_8stof
#undef _Dee_16stof
#undef _Dee_32stof
#undef F_SIZE
#undef F_NAME
#undef float_t
#undef DeeFObject
#undef DeeF_Type
#undef DeeF_New
#undef DeeF_Check
#undef DeeF_VALUE
#undef DeeF_Value
#undef SLOT
#undef KIND
#undef ID
#endif

DEE_DECL_END

