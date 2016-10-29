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
#include <deemon/integer.c>
#endif


#ifndef INTEGER_NAME_I8
#if 1
#define INTEGER_NAME_ATOMIC_PREFIX  "atomic "
#define INTEGER_NAME_ATOMIC_PREFIX_ "atomic"
#else
#define INTEGER_NAME_ATOMIC_PREFIX  "_Atomic "
#define INTEGER_NAME_ATOMIC_PREFIX_ "_Atomic"
#endif
#define INTEGER_NAME_UNSIGNED_PREFIX "unsigned "
#define INTEGER_NAME_I8  "__int8"
#define INTEGER_NAME_I16 "__int16"
#define INTEGER_NAME_I32 "__int32"
#define INTEGER_NAME_I64 "__int64"

#if !DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES
// Try to make the names of integer types look a bit better
// >> We we 'char', 'short', 'int', 'long' and 'long long'
//    to get away from the __intN naming convention.
#if DEE_CONFIG_RT_SIZEOF_CHAR == 1
#undef INTEGER_NAME_I8
#define INTEGER_NAME_I8 "signed char"
#define INTEGER_NAME_UI8 "unsigned char"
#if DEE_CONFIG_RT_SIZEOF_LLONG == 1
#undef INTEGER_NAME_I8
#define INTEGER_NAME_I8 "long long"
#elif DEE_CONFIG_RT_SIZEOF_LLONG == 2
#undef INTEGER_NAME_I16
#define INTEGER_NAME_I16 "long long"
#elif DEE_CONFIG_RT_SIZEOF_LLONG == 4
#undef INTEGER_NAME_I32
#define INTEGER_NAME_I32 "long long"
#elif DEE_CONFIG_RT_SIZEOF_LLONG == 8
#undef INTEGER_NAME_I64
#define INTEGER_NAME_I64 "long long"
#endif
#if DEE_CONFIG_RT_SIZEOF_LONG == 1
#undef INTEGER_NAME_I8
#define INTEGER_NAME_I8 "long"
#elif DEE_CONFIG_RT_SIZEOF_LONG == 2
#undef INTEGER_NAME_I16
#define INTEGER_NAME_I16 "long"
#elif DEE_CONFIG_RT_SIZEOF_LONG == 4
#undef INTEGER_NAME_I32
#define INTEGER_NAME_I32 "long"
#elif DEE_CONFIG_RT_SIZEOF_LONG == 8
#undef INTEGER_NAME_I64
#define INTEGER_NAME_I64 "long"
#endif
#if DEE_CONFIG_RT_SIZEOF_SHORT == 1
#undef INTEGER_NAME_I8
#define INTEGER_NAME_I8 "short"
#elif DEE_CONFIG_RT_SIZEOF_SHORT == 2
#undef INTEGER_NAME_I16
#define INTEGER_NAME_I16 "short"
#elif DEE_CONFIG_RT_SIZEOF_SHORT == 4
#undef INTEGER_NAME_I32
#define INTEGER_NAME_I32 "short"
#elif DEE_CONFIG_RT_SIZEOF_SHORT == 8
#undef INTEGER_NAME_I64
#define INTEGER_NAME_I64 "short"
#endif
#elif DEE_CONFIG_RT_SIZEOF_CHAR == 2
#undef INTEGER_NAME_I16
#define INTEGER_NAME_I16 "signed char"
#define INTEGER_NAME_UI16 "unsigned char"
#elif DEE_CONFIG_RT_SIZEOF_CHAR == 4
#undef INTEGER_NAME_I32
#define INTEGER_NAME_I32 "signed char"
#define INTEGER_NAME_UI32 "unsigned char"
#elif DEE_CONFIG_RT_SIZEOF_CHAR == 8
#undef INTEGER_NAME_I64
#define INTEGER_NAME_I64 "signed char"
#define INTEGER_NAME_UI64 "unsigned char"
#endif
#if DEE_CONFIG_RT_SIZEOF_INT == 1
#undef INTEGER_NAME_I8
#define INTEGER_NAME_I8 "int"
#elif DEE_CONFIG_RT_SIZEOF_INT == 2
#undef INTEGER_NAME_I16
#define INTEGER_NAME_I16 "int"
#elif DEE_CONFIG_RT_SIZEOF_INT == 4
#undef INTEGER_NAME_I32
#define INTEGER_NAME_I32 "int"
#elif DEE_CONFIG_RT_SIZEOF_INT == 8
#undef INTEGER_NAME_I64
#define INTEGER_NAME_I64 "int"
#endif
#endif

#ifndef INTEGER_NAME_UI8
#define INTEGER_NAME_UI8  INTEGER_NAME_UNSIGNED_PREFIX INTEGER_NAME_I8
#endif
#ifndef INTEGER_NAME_UI16
#define INTEGER_NAME_UI16 INTEGER_NAME_UNSIGNED_PREFIX INTEGER_NAME_I16
#endif
#ifndef INTEGER_NAME_UI32
#define INTEGER_NAME_UI32 INTEGER_NAME_UNSIGNED_PREFIX INTEGER_NAME_I32
#endif
#ifndef INTEGER_NAME_UI64
#define INTEGER_NAME_UI64 INTEGER_NAME_UNSIGNED_PREFIX INTEGER_NAME_I64
#endif
#endif

#if defined(__INTELLISENSE__)
#define N 8
#elif !defined(N)
#error Must define 'N' before including this file
#endif

DEE_DECL_BEGIN

#if N == 8
#define SIZEOF_INTN_T 1
#elif N == 16
#define SIZEOF_INTN_T 2
#elif N == 32
#define SIZEOF_INTN_T 4
#elif N == 64
#define SIZEOF_INTN_T 8
#endif

#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define DeeIntMin_New  DeeObject_New_F(int)
#define DeeUIntMin_New DeeObject_New_F(int) // NOTE: We create a signed type here!
#define dee_intmin_t   DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT)
#define dee_uintmin_t  DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_INT)
#else
#define DeeIntMin_New  DeeINT_New_F(SIZEOF_INTN_T)
#define DeeUIntMin_New DeeUINT_New_F(SIZEOF_INTN_T)
#define dee_intmin_t   DEE_TYPES_INT(SIZEOF_INTN_T)
#define dee_uintmin_t  DEE_TYPES_UINT(SIZEOF_INTN_T)
#endif

#define DeeAtomicIN_Store                     DeeAtomicN_Store_F(SIZEOF_INTN_T)
#define DeeAtomicIN_Load                      DeeAtomicN_Load_F(SIZEOF_INTN_T)
#define DeeAtomicIN_Exchange                  DeeAtomicN_Exchange_F(SIZEOF_INTN_T)
#define DeeAtomicIN_CompareExchangeWeak       DeeAtomicN_CompareExchangeWeak_F(SIZEOF_INTN_T)
#define DeeAtomicIN_CompareExchangeStrong     DeeAtomicN_CompareExchangeStrong_F(SIZEOF_INTN_T)
#define DeeAtomicIN_CompareExchangeWeakVal    DeeAtomicN_CompareExchangeWeakVal_F(SIZEOF_INTN_T)
#define DeeAtomicIN_CompareExchangeStrongVal  DeeAtomicN_CompareExchangeStrongVal_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchInc                  DeeAtomicN_FetchInc_F(SIZEOF_INTN_T)
#define DeeAtomicIN_IncFetch                  DeeAtomicN_IncFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchDec                  DeeAtomicN_FetchDec_F(SIZEOF_INTN_T)
#define DeeAtomicIN_DecFetch                  DeeAtomicN_DecFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchAdd                  DeeAtomicN_FetchAdd_F(SIZEOF_INTN_T)
#define DeeAtomicIN_AddFetch                  DeeAtomicN_AddFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchSub                  DeeAtomicN_FetchSub_F(SIZEOF_INTN_T)
#define DeeAtomicIN_SubFetch                  DeeAtomicN_SubFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchAnd                  DeeAtomicN_FetchAnd_F(SIZEOF_INTN_T)
#define DeeAtomicIN_AndFetch                  DeeAtomicN_AndFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchOr                   DeeAtomicN_FetchOr_F(SIZEOF_INTN_T)
#define DeeAtomicIN_OrFetch                   DeeAtomicN_OrFetch_F(SIZEOF_INTN_T)
#define DeeAtomicIN_FetchXor                  DeeAtomicN_FetchXor_F(SIZEOF_INTN_T)
#define DeeAtomicIN_XorFetch                  DeeAtomicN_XorFetch_F(SIZEOF_INTN_T)

#define intn_t                    DEE_PP_CAT_3(Dee_int,N,_t)
#define uintn_t                   DEE_PP_CAT_3(Dee_uint,N,_t)
#define DeeIntN_Check             DEE_PP_CAT_3(DeeInt,N,_Check)
#define DeeIntN_CheckExact        DEE_PP_CAT_3(DeeInt,N,_CheckExact)
#define DeeUIntN_Check            DEE_PP_CAT_3(DeeUInt,N,_Check)
#define DeeUIntN_CheckExact       DEE_PP_CAT_3(DeeUInt,N,_CheckExact)
#define DeeAtomicIntN_Check       DEE_PP_CAT_3(DeeAtomicInt,N,_Check)
#define DeeAtomicIntN_CheckExact  DEE_PP_CAT_3(DeeAtomicInt,N,_CheckExact)
#define DeeAtomicUIntN_Check      DEE_PP_CAT_3(DeeAtomicUInt,N,_Check)
#define DeeAtomicUIntN_CheckExact DEE_PP_CAT_3(DeeAtomicUInt,N,_CheckExact)
#define DeeObject_IntN(ob,result)  DeeObject_CAST_INT_F(SIZEOF_INTN_T)(Dee_TYPE(ob),ob,result)
#define DeeObject_UIntN(ob,result) DeeObject_CAST_UINT_F(SIZEOF_INTN_T)(Dee_TYPE(ob),ob,result)
#define DeeIntNObject             DEE_PP_CAT_3(DeeInt,N,Object)
#define DeeIntN_New               DEE_PP_CAT_3(DeeInt,N,_New)
#define DeeUIntN_New              DEE_PP_CAT_3(DeeUInt,N,_New)
#define DeeAtomicIntN_New         DEE_PP_CAT_3(DeeAtomicInt,N,_New)
#define DeeAtomicUIntN_New        DEE_PP_CAT_3(DeeAtomicUInt,N,_New)
#define SLOT(name)                DEE_PP_CAT_4(_deeint,N,_,name)
#define USLOT(name)               DEE_PP_CAT_4(_deeuint,N,_,name)
#define ASLOT(name)               DEE_PP_CAT_4(_deeatomicint,N,_,name)
#define AUSLOT(name)              DEE_PP_CAT_4(_deeatomicuint,N,_,name)
#define POOL_NAME                 DEE_PP_CAT_3(int,N,_pool)

#if N == 8
#define INPOW i8pow
#elif N == 16
#define INPOW i16pow
#elif N == 32
#define INPOW i32pow
#elif N == 64
#define INPOW i64pow
#endif

//////////////////////////////////////////////////////////////////////////
// Power functions
// As seen here: http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED intn_t DEE_PP_CAT_2(_Dee_m_,INPOW)(
 DEE_A_IN intn_t x, DEE_A_IN intn_t y) {
 intn_t result = 1;
 while (y) {
  if (y & 1) result *= x;
  y >>= 1;
  x *= x;
 }
 return result;
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED uintn_t DEE_PP_CAT_2(_Dee_m_u,INPOW)(
 DEE_A_IN uintn_t x, DEE_A_IN uintn_t y) {
 uintn_t result = 1;
 while (y) {
  if (y & 1) result *= x;
  y >>= 1;
  x *= x;
 }
 return result;
}



//////////////////////////////////////////////////////////////////////////
// Alloc functions
DEE_A_RET_OBJECT_EXCEPT_REF(DeeIntNObject) *
DEE_PP_CAT_3(DeeInt,N,_New)(DEE_A_IN intn_t v) {
 DeeIntNObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(POOL_NAME)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DEE_PP_CAT_3(DeeInt,N,_Type));
  result->i_value.s = v;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeIntNObject) *
DEE_PP_CAT_3(DeeUInt,N,_New)(DEE_A_IN uintn_t v) {
 DeeIntNObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(POOL_NAME)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DEE_PP_CAT_3(DeeUInt,N,_Type));
  result->i_value.u = v;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeIntNObject) *
DEE_PP_CAT_3(DeeAtomicInt,N,_New)(DEE_A_IN intn_t v) {
 DeeIntNObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(POOL_NAME)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DEE_PP_CAT_3(DeeAtomicInt,N,_Type));
  result->i_value.s = v;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeIntNObject) *
DEE_PP_CAT_3(DeeAtomicUInt,N,_New)(DEE_A_IN uintn_t v) {
 DeeIntNObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(POOL_NAME)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type));
  result->i_value.u = v;
 }
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeInt,N,_Force)(
 DEE_A_REF DEE_A_INOUT_OBJECT(DEE_PP_CAT_3(DeeInt,N,Object)) **ob) {
 DEE_ASSERT(ob); DEE_ASSERT(DeeObject_Check(*ob));
 if (!DeeIntN_Check(*ob)) {
  DeeObject *temp; intn_t itemp;
  if DEE_UNLIKELY(DeeObject_IntN(*ob,&itemp) != 0) return -1;
  if DEE_UNLIKELY((temp = DeeIntN_New(itemp)) == NULL) return -1;;
  Dee_DECREF(*ob);
  Dee_INHERIT_REF(*ob,temp);
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeUInt,N,_Force)(
 DEE_A_REF DEE_A_INOUT_OBJECT(DEE_PP_CAT_3(DeeInt,N,Object)) **ob) {
 DEE_ASSERT(ob); DEE_ASSERT(DeeObject_Check(*ob));
 if (!DeeUIntN_Check(*ob)) {
  DeeObject *temp; uintn_t itemp;
  if DEE_UNLIKELY(DeeObject_UIntN(*ob,&itemp) != 0) return -1;
  if DEE_UNLIKELY((temp = DeeUIntN_New(itemp)) == NULL) return -1;;
  Dee_DECREF(*ob);
  Dee_INHERIT_REF(*ob,temp);
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeAtomicInt,N,_Force)(
 DEE_A_REF DEE_A_INOUT_OBJECT(DEE_PP_CAT_3(DeeInt,N,Object)) **ob) {
 DEE_ASSERT(ob); DEE_ASSERT(DeeObject_Check(*ob));
 if (!DeeAtomicIntN_Check(*ob)) {
  DeeObject *temp; intn_t itemp;
  if DEE_UNLIKELY(DeeObject_IntN(*ob,&itemp) != 0) return -1;
  if DEE_UNLIKELY((temp = DeeAtomicIntN_New(itemp)) == NULL) return -1;;
  Dee_DECREF(*ob);
  Dee_INHERIT_REF(*ob,temp);
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeAtomicUInt,N,_Force)(
 DEE_A_REF DEE_A_INOUT_OBJECT(DEE_PP_CAT_3(DeeInt,N,Object)) **ob) {
 DEE_ASSERT(ob); DEE_ASSERT(DeeObject_Check(*ob));
 if (!DeeAtomicUIntN_Check(*ob)) {
  DeeObject *temp; uintn_t itemp;
  if DEE_UNLIKELY(DeeObject_UIntN(*ob,&itemp) != 0) return -1;
  if DEE_UNLIKELY((temp = DeeAtomicUIntN_New(itemp)) == NULL) return -1;;
  Dee_DECREF(*ob);
  Dee_INHERIT_REF(*ob,temp);
 }
 return 0;
}



//////////////////////////////////////////////////////////////////////////
// tp_alloc / tp_free
DeeIntNObject *SLOT(tp_alloc)(DeeTypeObject *tp) {
 DeeIntNObject *result = DEE_OBJECTPOOL_ALLOC(POOL_NAME);
 if (result) DeeObject_INIT(result,tp);
 return result;
}
void SLOT(tp_free)(DeeTypeObject *DEE_UNUSED(tp), DeeIntNObject *ob) {
 DEE_OBJECTPOOL_FREE(POOL_NAME,ob);
}


//////////////////////////////////////////////////////////////////////////
// Constructors / Assign
static int SLOT(tp_copy_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self, DeeIntNObject *right) {
 self->i_value.u = right->i_value.u;
 return 0;
}
static int SLOT(tp_copy_assign)(DeeIntNObject *self, DeeIntNObject *right) {
 self->i_value.u = right->i_value.u;
 return 0;
}
static int ASLOT(tp_copy_assign)(DeeIntNObject *self, DeeIntNObject *right) {
 DeeAtomicIN_Store(self->i_value.u,
                   DeeAtomicIN_Load(right->i_value.u,memory_order_seq_cst),
                   memory_order_seq_cst);
 return 0;
}
static int SLOT(tp_any_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self, DeeTupleObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack((DeeObject *)args,"o:" DEE_PP_CAT_2(INTEGER_NAME_I,N),&arg) == -1) return -1;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(arg)) return DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(arg),DeeWideString_STR(arg),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(arg)) return DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(arg)) return DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(arg)) return DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_IntN(arg,&self->i_value.s);
}
static int USLOT(tp_any_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self, DeeTupleObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack((DeeObject *)args,"o:" DEE_PP_CAT_2(INTEGER_NAME_UI,N),&arg) == -1) return -1;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(arg)) return DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(arg),DeeWideString_STR(arg),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(arg)) return DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(arg)) return DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(arg)) return DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_UIntN(arg,&self->i_value.u);
}
static int SLOT(tp_p_any_ctor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeTupleObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack((DeeObject *)args,"o:" DEE_PP_CAT_2(INTEGER_NAME_I,N),&arg) == -1) return -1;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(arg)) return DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(arg),DeeWideString_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(arg)) return DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(arg)) return DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(arg)) return DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_IntN(arg,self);
}
static int USLOT(tp_p_any_ctor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeTupleObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack((DeeObject *)args,"o:" DEE_PP_CAT_2(INTEGER_NAME_UI,N),&arg) == -1) return -1;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(arg)) return DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(arg),DeeWideString_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(arg)) return DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(arg)) return DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(arg)) return DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_UIntN(arg,self);
}

static int SLOT(tp_any_assign)(DeeIntNObject *self, DeeObject *right) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&self->i_value.s);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_IntN(right,&self->i_value.s);
}
static int USLOT(tp_any_assign)(DeeIntNObject *self, DeeObject *right) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&self->i_value.u);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_UIntN(right,&self->i_value.u);
}
static int ASLOT(tp_any_assign)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) { if (DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 { if (DeeObject_IntN(right,&temp) != 0) return -1; }
 DeeAtomicIN_Store(self->i_value.s,temp,memory_order_seq_cst);
 return 0;
}
static int AUSLOT(tp_any_assign)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) { if (DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 { if (DeeObject_UIntN(right,&temp) != 0) return -1; }
 DeeAtomicIN_Store(self->i_value.u,temp,memory_order_seq_cst);
 return 0;
}

static int SLOT(tp_p_any_assign)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_IntN(right,self);
}
static int USLOT(tp_p_any_assign)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) return DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) return DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) return DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) return DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeObject_UIntN(right,self);
}
static int ASLOT(tp_p_any_assign)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) { if (DEE_PP_CAT_3(DeeWideString_ToInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf8String_ToInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf16String_ToInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf32String_ToInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 { if (DeeObject_IntN(right,&temp) != 0) return -1; }
 DeeAtomicIN_Store(*self,temp,memory_order_seq_cst);
 return 0;
}
static int AUSLOT(tp_p_any_assign)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(right)) { if (DEE_PP_CAT_3(DeeWideString_ToUInt,N,WithLength)(DeeWideString_SIZE(right),DeeWideString_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf8String_ToUInt,N,WithLength)(DeeUtf8String_SIZE(right),DeeUtf8String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf16String_ToUInt,N,WithLength)(DeeUtf16String_SIZE(right),DeeUtf16String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(right)) { if (DEE_PP_CAT_3(DeeUtf32String_ToUInt,N,WithLength)(DeeUtf32String_SIZE(right),DeeUtf32String_STR(right),&temp) != 0) return -1; } else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 { if (DeeObject_UIntN(right,&temp) != 0) return -1; }
 DeeAtomicIN_Store(*self,temp,memory_order_seq_cst);
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// tp_str
static DeeObject *SLOT(tp_str)(DeeIntNObject *self) { return DEE_PP_CAT_2(DeeString_FromInt,N)(self->i_value.s); }
static DeeObject *USLOT(tp_str)(DeeIntNObject *self) { return DEE_PP_CAT_2(DeeString_FromUInt,N)(self->i_value.u); }
static DeeObject *ASLOT(tp_str)(DeeIntNObject *self) { return DEE_PP_CAT_2(DeeString_FromInt,N)(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_str)(DeeIntNObject *self) { return DEE_PP_CAT_2(DeeString_FromUInt,N)((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)); }
static DeeObject *SLOT(tp_p_str)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DEE_PP_CAT_2(DeeString_FromInt,N)(*self); }
static DeeObject *USLOT(tp_p_str)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DEE_PP_CAT_2(DeeString_FromUInt,N)(*self); }
static DeeObject *ASLOT(tp_p_str)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DEE_PP_CAT_2(DeeString_FromInt,N)(DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_p_str)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DEE_PP_CAT_2(DeeString_FromUInt,N)((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); }

//////////////////////////////////////////////////////////////////////////
// tp_int32 / tp_int64 / tp_double
static int SLOT(tp_int32)(DeeIntNObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)self->i_value.s; return 0; }
static int USLOT(tp_int32)(DeeIntNObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)self->i_value.u; return 0; }
static int SLOT(tp_int64)(DeeIntNObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)self->i_value.s; return 0; }
static int USLOT(tp_int64)(DeeIntNObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)self->i_value.u; return 0; }
static int SLOT(tp_double)(DeeIntNObject *self, double *result) { *result = (double)self->i_value.s; return 0; }
static int USLOT(tp_double)(DeeIntNObject *self, double *result) { *result = (double)self->i_value.u; return 0; }
static int ASLOT(tp_int32)(DeeIntNObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_int32)(DeeIntNObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)); return 0; }
static int ASLOT(tp_int64)(DeeIntNObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_int64)(DeeIntNObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)); return 0; }
static int ASLOT(tp_double)(DeeIntNObject *self, double *result) { *result = (double)DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_double)(DeeIntNObject *self, double *result) { *result = (double)(uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst); return 0; }
static int SLOT(tp_p_int32)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)*self; return 0; }
static int USLOT(tp_p_int32)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)*self; return 0; }
static int SLOT(tp_p_int64)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)*self; return 0; }
static int USLOT(tp_p_int64)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)*self; return 0; }
static int SLOT(tp_p_double)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, double *result) { *result = (double)*self; return 0; }
static int USLOT(tp_p_double)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, double *result) { *result = (double)*self; return 0; }
static int ASLOT(tp_p_int32)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)DeeAtomicIN_Load(*self,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_p_int32)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); return 0; }
static int ASLOT(tp_p_int64)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)DeeAtomicIN_Load(*self,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_p_int64)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); return 0; }
static int ASLOT(tp_p_double)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, double *result) { *result = (double)DeeAtomicIN_Load(*self,memory_order_seq_cst); return 0; }
static int AUSLOT(tp_p_double)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, double *result) { *result = (double)(uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_not
static DeeObject *SLOT(tp_not)(DeeIntNObject *self) { DeeReturn_Bool(!self->i_value.s); }
static DeeObject *ASLOT(tp_not)(DeeIntNObject *self) { DeeReturn_Bool(!DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *SLOT(tp_p_not)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { DeeReturn_Bool(!*self); }
static DeeObject *ASLOT(tp_p_not)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { DeeReturn_Bool(!DeeAtomicIN_Load(*self,memory_order_seq_cst)); }

//////////////////////////////////////////////////////////////////////////
// tp_bool
static int SLOT(tp_bool)(DeeIntNObject *self) { return self->i_value.s != 0; }
static int ASLOT(tp_bool)(DeeIntNObject *self) { return DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst) != 0; }
static int SLOT(tp_p_bool)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return *self != 0; }
static int ASLOT(tp_p_bool)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeAtomicIN_Load(*self,memory_order_seq_cst) != 0; }

//////////////////////////////////////////////////////////////////////////
// tp_inv
static DeeObject *SLOT(tp_inv)(DeeIntNObject *self) { return DeeIntMin_New(~self->i_value.s); }
static DeeObject *USLOT(tp_inv)(DeeIntNObject *self) { return DeeUIntMin_New(~self->i_value.u); }
static DeeObject *ASLOT(tp_inv)(DeeIntNObject *self) { return DeeIntMin_New(~DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_inv)(DeeIntNObject *self) { return DeeUIntMin_New(~(uintn_t)DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *SLOT(tp_p_inv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(~*self); }
static DeeObject *USLOT(tp_p_inv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(~*self); }
static DeeObject *ASLOT(tp_p_inv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(~DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_p_inv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(~(uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); }

//////////////////////////////////////////////////////////////////////////
// tp_pos
static DeeObject *SLOT(tp_p_pos)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(+*self); }
static DeeObject *USLOT(tp_p_pos)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(+*self); }
static DeeObject *ASLOT(tp_p_pos)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(+DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_p_pos)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(+(uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
static DeeObject *SLOT(tp_pos)(DeeIntNObject *self) { return DeeIntMin_New(+self->i_value.s); }
static DeeObject *ASLOT(tp_pos)(DeeIntNObject *self) { return DeeIntMin_New(+DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *USLOT(tp_pos)(DeeIntNObject *self) { return DeeUIntMin_New(+self->i_value.u); }
static DeeObject *AUSLOT(tp_pos)(DeeIntNObject *self) { return DeeUIntMin_New(+(uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)); }
#endif

//////////////////////////////////////////////////////////////////////////
// tp_neg
static DeeObject *SLOT(tp_neg)(DeeIntNObject *self) { return DeeIntMin_New(-self->i_value.s); }
static DeeObject *ASLOT(tp_neg)(DeeIntNObject *self) { return DeeIntMin_New(-DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *SLOT(tp_p_neg)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(-*self); }
static DeeObject *ASLOT(tp_p_neg)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntMin_New(-DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
static DeeObject *USLOT(tp_neg)(DeeIntNObject *self) { return DeeUIntMin_New(-self->i_value.u); }
static DeeObject *AUSLOT(tp_neg)(DeeIntNObject *self) { return DeeUIntMin_New(-(uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)); }
static DeeObject *USLOT(tp_p_neg)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(-*self); }
static DeeObject *AUSLOT(tp_p_neg)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntMin_New(-(uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)); }
#endif

//////////////////////////////////////////////////////////////////////////
// tp_inc / tp_incpost / tp_dec / tp_decpost
static DeeObject *SLOT(tp_inc)(DeeIntNObject *self) { ++self->i_value.s; DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_dec)(DeeIntNObject *self) { --self->i_value.s; DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_incpost)(DeeIntNObject *self) { return DeeIntN_New(self->i_value.s++); }
static DeeObject *SLOT(tp_decpost)(DeeIntNObject *self) { return DeeIntN_New(self->i_value.s--); }
static DeeObject *USLOT(tp_incpost)(DeeIntNObject *self) { return DeeUIntN_New(self->i_value.u++); }
static DeeObject *USLOT(tp_decpost)(DeeIntNObject *self) { return DeeUIntN_New(self->i_value.u--); }
static DeeObject *ASLOT(tp_inc)(DeeIntNObject *self) { DeeAtomicIN_IncFetch(self->i_value.s,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_dec)(DeeIntNObject *self) { DeeAtomicIN_DecFetch(self->i_value.s,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_incpost)(DeeIntNObject *self) { return DeeIntN_New(DeeAtomicIN_FetchInc(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *ASLOT(tp_decpost)(DeeIntNObject *self) { return DeeIntN_New(DeeAtomicIN_FetchDec(self->i_value.s,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_incpost)(DeeIntNObject *self) { return DeeUIntN_New((uintn_t)DeeAtomicIN_FetchInc(self->i_value.u,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_decpost)(DeeIntNObject *self) { return DeeUIntN_New((uintn_t)DeeAtomicIN_FetchDec(self->i_value.u,memory_order_seq_cst)); }
static int SLOT(tp_p_inc)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { ++*self; return 0; }
static int SLOT(tp_p_dec)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { --*self; return 0; }
static int ASLOT(tp_p_inc)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { DeeAtomicIN_IncFetch(*self,memory_order_seq_cst); return 0; }
static int ASLOT(tp_p_dec)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { DeeAtomicIN_IncFetch(*self,memory_order_seq_cst); return 0; }
static DeeObject *SLOT(tp_p_incpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntN_New(*self++); }
static DeeObject *SLOT(tp_p_decpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntN_New(*self--); }
static DeeObject *USLOT(tp_p_incpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntN_New(*self++); }
static DeeObject *USLOT(tp_p_decpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntN_New(*self--); }
static DeeObject *ASLOT(tp_p_incpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntN_New(DeeAtomicIN_FetchInc(*self,memory_order_seq_cst)); }
static DeeObject *ASLOT(tp_p_decpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self) { return DeeIntN_New(DeeAtomicIN_FetchDec(*self,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_p_incpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntN_New((uintn_t)DeeAtomicIN_FetchInc(*self,memory_order_seq_cst)); }
static DeeObject *AUSLOT(tp_p_decpost)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self) { return DeeUIntN_New((uintn_t)DeeAtomicIN_FetchDec(*self,memory_order_seq_cst)); }

//////////////////////////////////////////////////////////////////////////
// tp_add / tp_iadd
static DeeObject *SLOT(tp_add)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s+temp); }
static DeeObject *USLOT(tp_add)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u+temp); }
static DeeObject *ASLOT(tp_add)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)+temp); }
static DeeObject *AUSLOT(tp_add)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)+temp); }
static DeeObject *SLOT(tp_iadd)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s += temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_iadd)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u += temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_iadd)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchAdd(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_iadd)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchAdd(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_add)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self+temp); }
static DeeObject *USLOT(tp_p_add)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self+temp); }
static DeeObject *ASLOT(tp_p_add)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)+temp); }
static DeeObject *AUSLOT(tp_p_add)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)+temp); }
static int SLOT(tp_p_iadd)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self += temp; return 0; }
static int ASLOT(tp_p_iadd)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_FetchAdd(*self,temp,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_sub / tp_isub
static DeeObject *SLOT(tp_sub)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s-temp); }
static DeeObject *USLOT(tp_sub)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u-temp); }
static DeeObject *ASLOT(tp_sub)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)-temp); }
static DeeObject *AUSLOT(tp_sub)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)-temp); }
static DeeObject *SLOT(tp_isub)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s -= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_isub)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u -= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_isub)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchSub(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_isub)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchSub(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_sub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self-temp); }
static DeeObject *USLOT(tp_p_sub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self-temp); }
static DeeObject *ASLOT(tp_p_sub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)-temp); }
static DeeObject *AUSLOT(tp_p_sub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)-temp); }
static int SLOT(tp_p_isub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self -= temp; return 0; }
static int ASLOT(tp_p_isub)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_FetchSub(*self,temp,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_mul / tp_imul
#define DeeAtomicIN_Mul(x,v,order) \
do{\
 intn_t temp_atom; do {\
  temp_atom = (intn_t)(x);\
 } while (!DeeAtomicIN_CompareExchangeWeak(\
 x,temp_atom,temp_atom*(v),memory_order_seq_cst,memory_order_seq_cst));\
}while(0)
static DeeObject *SLOT(tp_mul)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s*temp); }
static DeeObject *USLOT(tp_mul)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u*temp); }
static DeeObject *ASLOT(tp_mul)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)*temp); }
static DeeObject *AUSLOT(tp_mul)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)*temp); }
static DeeObject *SLOT(tp_imul)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s *= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_imul)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u *= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_imul)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Mul(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_imul)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Mul(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_mul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self*temp); }
static DeeObject *USLOT(tp_p_mul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self*temp); }
static DeeObject *ASLOT(tp_p_mul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)*temp); }
static DeeObject *AUSLOT(tp_p_mul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)*temp); }
static int SLOT(tp_p_imul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self *= temp; return 0; }
static int ASLOT(tp_p_imul)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_Mul(*self,temp,memory_order_seq_cst); return 0; }
#undef DeeAtomicIN_Mul

//////////////////////////////////////////////////////////////////////////
// tp_div / tp_idiv
#define DeeAtomicIN_Div(x,v,order) \
do{\
 intn_t temp_atom; do {\
  temp_atom = (intn_t)(x);\
 } while (!DeeAtomicIN_CompareExchangeWeak(\
 x,temp_atom,temp_atom/(v),memory_order_seq_cst,memory_order_seq_cst));\
}while(0)
#if !DEE_XCONFIG_INTEGER_DIVIDE_BY_ZERO_USE_SIGNALHANDLER || defined(DEE_WITHOUT_SIGNALHANDLERS)
static DeeObject *SLOT(tp_div)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(self->i_value.s/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *USLOT(tp_div)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New(self->i_value.u/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *ASLOT(tp_div)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *AUSLOT(tp_div)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *SLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; self->i_value.s /= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; self->i_value.u /= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; DeeAtomicIN_Div(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; DeeAtomicIN_Div(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_div)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(*self/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *USLOT(tp_p_div)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New(*self/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *ASLOT(tp_p_div)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *AUSLOT(tp_p_div)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)/temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static int SLOT(tp_p_idiv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; if (!temp) { DeeError_Throw(DeeErrorInstance_DivByZero); return -1; } *self /= temp; return 0; }
static int ASLOT(tp_p_idiv)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; if (!temp) { DeeError_Throw(DeeErrorInstance_DivByZero); return -1; } DeeAtomicIN_Div(*self,temp,memory_order_seq_cst); return 0; }
#else
static DeeObject *SLOT(tp_div)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = self->i_value.s/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeIntMin_New(result);
}
static DeeObject *USLOT(tp_div)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = self->i_value.u/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeUIntMin_New(result);
}
static DeeObject *ASLOT(tp_div)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeIntMin_New(result);
}
static DeeObject *AUSLOT(tp_div)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = (uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static DeeObject *SLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  self->i_value.s /= temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *USLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  self->i_value.u /= temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *ASLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Div(self->i_value.s,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *AUSLOT(tp_idiv)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Div(self->i_value.u,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *SLOT(tp_p_div)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = *self/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeIntMin_New(result);
}
static DeeObject *USLOT(tp_p_div)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = *self/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static DeeObject *ASLOT(tp_p_div)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = DeeAtomicIN_Load(*self,memory_order_seq_cst)/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeIntMin_New(result);
}
static DeeObject *AUSLOT(tp_p_div)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = (uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)/temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static int SLOT(tp_p_idiv)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 DEE_SIGNALHANDLER_BEGIN {
  *self /= temp;
 } DEE_SIGNALHANDLER_END({
  return -1;
 });
 return 0;
}
static int ASLOT(tp_p_idiv)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Div(*self,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return -1;
 });
 return 0;
}
#endif
#undef DeeAtomicIN_Div

//////////////////////////////////////////////////////////////////////////
// tp_mod / tp_imod
#define DeeAtomicIN_Mod(x,v,order) \
do{\
 intn_t temp_atom; do {\
  temp_atom = (intn_t)(x);\
 } while (!DeeAtomicIN_CompareExchangeWeak(\
 x,temp_atom,temp_atom%(v),memory_order_seq_cst,memory_order_seq_cst));\
}while(0)
#if !DEE_XCONFIG_INTEGER_MODULO_BY_ZERO_USE_SIGNALHANDLER || defined(DEE_WITHOUT_SIGNALHANDLERS)
static DeeObject *SLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(self->i_value.s%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *USLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New(self->i_value.u%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *ASLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *AUSLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *SLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; self->i_value.s %= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; self->i_value.u %= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; DeeAtomicIN_Mod(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; if (!temp) return DeeError_Throw(DeeErrorInstance_DivByZero),NULL; DeeAtomicIN_Mod(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_mod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(*self%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *USLOT(tp_p_mod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New(*self%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *ASLOT(tp_p_mod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : (temp ? DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static DeeObject *AUSLOT(tp_p_mod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : (temp ? DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)%temp) : (DeeError_Throw(DeeErrorInstance_DivByZero),NULL)); }
static int SLOT(tp_p_imod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; if (!temp) { DeeError_Throw(DeeErrorInstance_DivByZero); return -1; } *self %= temp; return 0; }
static int ASLOT(tp_p_imod)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; if (!temp) { DeeError_Throw(DeeErrorInstance_DivByZero); return -1; } DeeAtomicIN_Mod(*self,temp,memory_order_seq_cst); return 0; }
#else
static DeeObject *SLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = self->i_value.s%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeIntMin_New(result);
}
static DeeObject *USLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = self->i_value.u%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeUIntMin_New(result);
}
static DeeObject *ASLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeIntMin_New(result);
}
static DeeObject *AUSLOT(tp_mod)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = (uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static DeeObject *SLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  self->i_value.s %= temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *USLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  self->i_value.u %= temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *ASLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Mod(self->i_value.s,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *AUSLOT(tp_imod)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Mod(self->i_value.u,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeReturn_NEWREF(self);
}
static DeeObject *SLOT(tp_p_mod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = *self%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeIntMin_New(result);
}
static DeeObject *USLOT(tp_p_mod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = *self%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static DeeObject *ASLOT(tp_p_mod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp,result;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = DeeAtomicIN_Load(*self,memory_order_seq_cst)%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeIntMin_New(result);
}
static DeeObject *AUSLOT(tp_p_mod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp,result;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 DEE_SIGNALHANDLER_BEGIN {
  result = (uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)%temp;
 } DEE_SIGNALHANDLER_END({
  return NULL;
 });
 DeeUIntMin_New(result);
}
static int SLOT(tp_p_imod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 DEE_SIGNALHANDLER_BEGIN {
  *self %= temp;
 } DEE_SIGNALHANDLER_END({
  return -1;
 });
 return 0;
}
static int ASLOT(tp_p_imod)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 DEE_SIGNALHANDLER_BEGIN {
  DeeAtomicIN_Mod(*self,temp,memory_order_seq_cst);
 } DEE_SIGNALHANDLER_END({
  return -1;
 });
 return 0;
}
#endif
#undef DeeAtomicIN_Mod

//////////////////////////////////////////////////////////////////////////
// tp_shl / tp_ishl
#define DeeAtomicIN_Shl(x,v,order) \
do{\
 intn_t temp_atom; do {\
  temp_atom = (intn_t)(x);\
 } while (!DeeAtomicIN_CompareExchangeWeak(\
 x,temp_atom,temp_atom<<(v),memory_order_seq_cst,memory_order_seq_cst));\
}while(0)
static DeeObject *SLOT(tp_shl)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s<<temp); }
static DeeObject *USLOT(tp_shl)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u<<temp); }
static DeeObject *ASLOT(tp_shl)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)<<temp); }
static DeeObject *AUSLOT(tp_shl)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)<<temp); }
static DeeObject *SLOT(tp_ishl)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s <<= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_ishl)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u <<= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_ishl)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Shl(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_ishl)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Shl(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_shl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self<<temp); }
static DeeObject *USLOT(tp_p_shl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self<<temp); }
static DeeObject *ASLOT(tp_p_shl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)<<temp); }
static DeeObject *AUSLOT(tp_p_shl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)<<temp); }
static int SLOT(tp_p_ishl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self <<= temp; return 0; }
static int ASLOT(tp_p_ishl)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_Shl(*self,temp,memory_order_seq_cst); return 0; }
#undef DeeAtomicIN_Shl

//////////////////////////////////////////////////////////////////////////
// tp_shr / tp_ishr
#define DeeAtomicIN_Shr(x,v,order) \
do{\
 intn_t temp_atom; do {\
  temp_atom = (intn_t)(x);\
 } while (!DeeAtomicIN_CompareExchangeWeak(\
 x,temp_atom,temp_atom>>(v),memory_order_seq_cst,memory_order_seq_cst));\
}while(0)
static DeeObject *SLOT(tp_shr)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s>>temp); }
static DeeObject *USLOT(tp_shr)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u>>temp); }
static DeeObject *ASLOT(tp_shr)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)>>temp); }
static DeeObject *AUSLOT(tp_shr)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)>>temp); }
static DeeObject *SLOT(tp_ishr)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s >>= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_ishr)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u >>= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_ishr)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Shr(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_ishr)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_Shr(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_shr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self>>temp); }
static DeeObject *USLOT(tp_p_shr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self>>temp); }
static DeeObject *ASLOT(tp_p_shr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)>>temp); }
static DeeObject *AUSLOT(tp_p_shr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)>>temp); }
static int SLOT(tp_p_ishr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self >>= temp; return 0; }
static int ASLOT(tp_p_ishr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_Shr(*self,temp,memory_order_seq_cst); return 0; }
#undef DeeAtomicIN_Shr

//////////////////////////////////////////////////////////////////////////
// tp_and / tp_iand
static DeeObject *SLOT(tp_and)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s&temp); }
static DeeObject *USLOT(tp_and)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u&temp); }
static DeeObject *ASLOT(tp_and)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)&temp); }
static DeeObject *AUSLOT(tp_and)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)&temp); }
static DeeObject *SLOT(tp_iand)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s &= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_iand)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u &= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_iand)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchAnd(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_iand)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchAnd(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_and)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self&temp); }
static DeeObject *USLOT(tp_p_and)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self&temp); }
static DeeObject *ASLOT(tp_p_and)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)&temp); }
static DeeObject *AUSLOT(tp_p_and)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)&temp); }
static int SLOT(tp_p_iand)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self &= temp; return 0; }
static int ASLOT(tp_p_iand)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_FetchAnd(*self,temp,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_or / tp_ior
static DeeObject *SLOT(tp_or)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s|temp); }
static DeeObject *USLOT(tp_or)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u|temp); }
static DeeObject *ASLOT(tp_or)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)|temp); }
static DeeObject *AUSLOT(tp_or)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)|temp); }
static DeeObject *SLOT(tp_ior)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s |= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_ior)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u |= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_ior)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchOr(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_ior)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchOr(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_or)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self|temp); }
static DeeObject *USLOT(tp_p_or)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self|temp); }
static DeeObject *ASLOT(tp_p_or)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)|temp); }
static DeeObject *AUSLOT(tp_p_or)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)|temp); }
static int SLOT(tp_p_ior)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self |= temp; return 0; }
static int ASLOT(tp_p_ior)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_FetchOr(*self,temp,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_xor / tp_ixor
static DeeObject *SLOT(tp_xor)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(self->i_value.s^temp); }
static DeeObject *USLOT(tp_xor)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(self->i_value.u^temp); }
static DeeObject *ASLOT(tp_xor)(DeeIntNObject *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst)^temp); }
static DeeObject *AUSLOT(tp_xor)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst)^temp); }
static DeeObject *SLOT(tp_ixor)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; self->i_value.s ^= temp; DeeReturn_NEWREF(self); }
static DeeObject *USLOT(tp_ixor)(DeeIntNObject *self, DeeObject *right) { uintn_t temp; if (DeeObject_UIntN(right,&temp) != 0) return NULL; self->i_value.u ^= temp; DeeReturn_NEWREF(self); }
static DeeObject *ASLOT(tp_ixor)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchXor(self->i_value.s,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *AUSLOT(tp_ixor)(DeeIntNObject *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return NULL; DeeAtomicIN_FetchXor(self->i_value.u,temp,memory_order_seq_cst); DeeReturn_NEWREF(self); }
static DeeObject *SLOT(tp_p_xor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(*self^temp); }
static DeeObject *USLOT(tp_p_xor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New(*self^temp); }
static DeeObject *ASLOT(tp_p_xor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; return DeeObject_IntN(right,&temp) != 0 ? NULL : DeeIntMin_New(DeeAtomicIN_Load(*self,memory_order_seq_cst)^temp); }
static DeeObject *AUSLOT(tp_p_xor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) { uintn_t temp; return DeeObject_UIntN(right,&temp) != 0 ? NULL : DeeUIntMin_New((uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst)^temp); }
static int SLOT(tp_p_ixor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; *self ^= temp; return 0; }
static int ASLOT(tp_p_ixor)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) { intn_t temp; if (DeeObject_IntN(right,&temp) != 0) return -1; DeeAtomicIN_FetchXor(*self,temp,memory_order_seq_cst); return 0; }

//////////////////////////////////////////////////////////////////////////
// tp_pow / tp_ipow
static DeeObject *SLOT(tp_pow)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 return DeeIntN_New(DEE_PP_CAT_2(_Dee_m_,INPOW)(self->i_value.s,temp));
}
static DeeObject *USLOT(tp_pow)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 return DeeUIntN_New(DEE_PP_CAT_2(_Dee_m_u,INPOW)(self->i_value.u,temp));
}
static DeeObject *ASLOT(tp_pow)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 return DeeIntN_New(DEE_PP_CAT_2(_Dee_m_,INPOW)(
  DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst),temp));
}
static DeeObject *AUSLOT(tp_pow)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 return DeeUIntN_New(DEE_PP_CAT_2(_Dee_m_u,INPOW)(
  (uintn_t)DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst),temp));
}
static DeeObject *SLOT(tp_ipow)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 self->i_value.s = DEE_PP_CAT_2(_Dee_m_,INPOW)(self->i_value.s,temp);
 DeeReturn_NEWREF(self);
}
static DeeObject *USLOT(tp_ipow)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 self->i_value.u = DEE_PP_CAT_2(_Dee_m_u,INPOW)(self->i_value.u,temp);
 DeeReturn_NEWREF(self);
}
static DeeObject *ASLOT(tp_ipow)(DeeIntNObject *self, DeeObject *right) {
 intn_t temp,oldval,newval;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 do {
  oldval = self->i_value.s;
  newval = DEE_PP_CAT_2(_Dee_m_,INPOW)(oldval,temp);
 } while (!DeeAtomicIN_CompareExchangeWeak(
  self->i_value.s,oldval,newval,
  memory_order_seq_cst,memory_order_seq_cst));
 DeeReturn_NEWREF(self);
}
static DeeObject *AUSLOT(tp_ipow)(DeeIntNObject *self, DeeObject *right) {
 uintn_t temp,oldval,newval;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 do {
  oldval = self->i_value.u;
  newval = DEE_PP_CAT_2(_Dee_m_u,INPOW)(oldval,temp);
 } while (!DeeAtomicIN_CompareExchangeWeak(
  self->i_value.u,oldval,newval,
  memory_order_seq_cst,memory_order_seq_cst));
 DeeReturn_NEWREF(self);
}
static DeeObject *SLOT(tp_p_pow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 return DeeIntN_New(DEE_PP_CAT_2(_Dee_m_,INPOW)(*self,temp));
}
static DeeObject *USLOT(tp_p_pow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 return DeeUIntN_New(DEE_PP_CAT_2(_Dee_m_u,INPOW)(*self,temp));
}
static DeeObject *ASLOT(tp_p_pow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return NULL;
 return DeeIntN_New(DEE_PP_CAT_2(_Dee_m_,INPOW)(DeeAtomicIN_Load(*self,memory_order_seq_cst),temp));
}
static DeeObject *AUSLOT(tp_p_pow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return NULL;
 return DeeUIntN_New(DEE_PP_CAT_2(_Dee_m_u,INPOW)((
  uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst),temp));
}
static int SLOT(tp_p_ipow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 *self = DEE_PP_CAT_2(_Dee_m_,INPOW)(*self,temp);
 return 0;
}
static int USLOT(tp_p_ipow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp;
 if (DeeObject_UIntN(right,&temp) != 0) return -1;
 *self = DEE_PP_CAT_2(_Dee_m_u,INPOW)(*self,temp);
 return 0;
}
static int ASLOT(tp_p_ipow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *right) {
 intn_t temp,oldval,newval;
 if (DeeObject_IntN(right,&temp) != 0) return -1;
 do {
  oldval = *self;
  newval = DEE_PP_CAT_2(_Dee_m_,INPOW)(oldval,temp);
 } while (!DeeAtomicIN_CompareExchangeWeak(
  *self,oldval,newval,memory_order_seq_cst,memory_order_seq_cst));
 return 0;
}
static int AUSLOT(tp_p_ipow)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *right) {
 uintn_t temp,oldval,newval;
 if (DeeObject_UIntN(right,&temp) != 0) return -1;
 do {
  oldval = *self;
  newval = DEE_PP_CAT_2(_Dee_m_u,INPOW)(oldval,temp);
 } while (!DeeAtomicIN_CompareExchangeWeak(
  *self,oldval,newval,memory_order_seq_cst,memory_order_seq_cst));
 return 0;
}


#undef INPOW

//////////////////////////////////////////////////////////////////////////
// tp_hash
static int SLOT(tp_hash)(DeeIntNObject *self, Dee_hash_t start, Dee_hash_t *result) {
#if SIZEOF_INTN_T <= DEE_CONFIG_SIZEOF_DEE_HASH_T
 *result = start ^ (Dee_hash_t)self->i_value.u;
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*2
 *result = start ^ ((Dee_hash_t *)&self->i_value.u)[0] ^ ((Dee_hash_t *)&self->i_value.u)[1];
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*4
 *result = start ^ ((Dee_hash_t *)&self->i_value.u)[0] ^ ((Dee_hash_t *)&self->i_value.u)[1] ^
                   ((Dee_hash_t *)&self->i_value.u)[2] ^ ((Dee_hash_t *)&self->i_value.u)[3];
#else
#error "Invalid/Unsupported hash/integer size"
#endif
 return 0;
}
static int ASLOT(tp_hash)(DeeIntNObject *self, Dee_hash_t start, Dee_hash_t *result) {
 uintn_t temp = (uintn_t)DeeAtomicIN_Load(self->i_value.s,memory_order_seq_cst);
#if SIZEOF_INTN_T <= DEE_CONFIG_SIZEOF_DEE_HASH_T
 *result = start ^ (Dee_hash_t)temp;
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*2
 *result = start ^ ((Dee_hash_t *)&temp)[0] ^ ((Dee_hash_t *)&temp)[1];
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*4
 *result = start ^ ((Dee_hash_t *)&temp)[0] ^ ((Dee_hash_t *)&temp)[1] ^
                   ((Dee_hash_t *)&temp)[2] ^ ((Dee_hash_t *)&temp)[3];
#else
#error "Invalid/Unsupported hash/integer size"
#endif
 return 0;
}
static int SLOT(tp_p_hash)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_hash_t start, Dee_hash_t *result) {
#if SIZEOF_INTN_T <= DEE_CONFIG_SIZEOF_DEE_HASH_T
 *result = start ^ (Dee_hash_t)*self;
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*2
 *result = start ^ ((Dee_hash_t *)self)[0] ^ ((Dee_hash_t *)self)[1];
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*4
 *result = start ^ ((Dee_hash_t *)self)[0] ^ ((Dee_hash_t *)self)[1] ^
                   ((Dee_hash_t *)self)[2] ^ ((Dee_hash_t *)self)[3];
#else
#error "Invalid/Unsupported hash/integer size"
#endif
 return 0;
}
static int ASLOT(tp_p_hash)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, Dee_hash_t start, Dee_hash_t *result) {
 uintn_t temp = (uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst);
#if SIZEOF_INTN_T <= DEE_CONFIG_SIZEOF_DEE_HASH_T
 *result = start ^ (Dee_hash_t)temp;
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*2
 *result = start ^ ((Dee_hash_t *)&temp)[0] ^ ((Dee_hash_t *)&temp)[1];
#elif SIZEOF_INTN_T == DEE_CONFIG_SIZEOF_DEE_HASH_T*4
 *result = start ^ ((Dee_hash_t *)&temp)[0] ^ ((Dee_hash_t *)&temp)[1] ^
                   ((Dee_hash_t *)&temp)[2] ^ ((Dee_hash_t *)&temp)[3];
#else
#error "Invalid/Unsupported hash/integer size"
#endif
 return 0;
}

//////////////////////////////////////////////////////////////////////////
// tp_compare.tp_cmp_xx
static DEE_A_REF DeeObject *SLOT(tp_cmp_lo)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s < temp); }
static DEE_A_REF DeeObject *SLOT(tp_cmp_le)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s <= temp); }
static DEE_A_REF DeeObject *SLOT(tp_cmp_eq)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s == temp); }
static DEE_A_REF DeeObject *SLOT(tp_cmp_ne)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s != temp); }
static DEE_A_REF DeeObject *SLOT(tp_cmp_gr)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s > temp); }
static DEE_A_REF DeeObject *SLOT(tp_cmp_ge)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.s >= temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_lo)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u < temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_le)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u <= temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_eq)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u == temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_ne)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u != temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_gr)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u > temp); }
static DEE_A_REF DeeObject *USLOT(tp_cmp_ge)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(lhs->i_value.u >= temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_lo)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) < temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_le)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) <= temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_eq)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) == temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_ne)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) != temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_gr)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) > temp); }
static DEE_A_REF DeeObject *ASLOT(tp_cmp_ge)(DeeIntNObject *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(lhs->i_value.s,memory_order_seq_cst) >= temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_lo)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) < temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_le)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) <= temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_eq)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) == temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_ne)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) != temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_gr)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) > temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_cmp_ge)(DeeIntNObject *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(lhs->i_value.u,memory_order_seq_cst) >= temp); }

static DEE_A_REF DeeObject *SLOT(tp_p_cmp_lo)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs < temp); }
static DEE_A_REF DeeObject *SLOT(tp_p_cmp_le)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs <= temp); }
static DEE_A_REF DeeObject *SLOT(tp_p_cmp_eq)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs == temp); }
static DEE_A_REF DeeObject *SLOT(tp_p_cmp_ne)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs != temp); }
static DEE_A_REF DeeObject *SLOT(tp_p_cmp_gr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs > temp); }
static DEE_A_REF DeeObject *SLOT(tp_p_cmp_ge)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs >= temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_lo)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs < temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_le)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs <= temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_eq)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs == temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_ne)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs != temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_gr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs > temp); }
static DEE_A_REF DeeObject *USLOT(tp_p_cmp_ge)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(*lhs >= temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_lo)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) < temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_le)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) <= temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_eq)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) == temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_ne)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) != temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_gr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) > temp); }
static DEE_A_REF DeeObject *ASLOT(tp_p_cmp_ge)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *lhs, DeeObject *rhs) { intn_t temp; if (DeeObject_IntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool(DeeAtomicIN_Load(*lhs,memory_order_seq_cst) >= temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_lo)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) < temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_le)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) <= temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_eq)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) == temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_ne)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) != temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_gr)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) > temp); }
static DEE_A_REF DeeObject *AUSLOT(tp_p_cmp_ge)(DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *lhs, DeeObject *rhs) { uintn_t temp; if (DeeObject_UIntN(rhs,&temp) != 0) return NULL; DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(*lhs,memory_order_seq_cst) >= temp); }



//////////////////////////////////////////////////////////////////////////
// Member functions of atomic types
#ifndef DEE_PRIVATE_INTEGER_ATOMIC_INVALID_ORDER_DEFINED
#define DEE_PRIVATE_INTEGER_ATOMIC_INVALID_ORDER_DEFINED 1
static void _dee_invalid_order(char const *func) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Invalid memory order for %s",
                     func);
}
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static DeeObject *ASLOT(clear)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:clear",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: DeeAtomicIN_Store(*self,0,memory_order_relaxed); break;
  case Dee_memory_order_release: DeeAtomicIN_Store(*self,0,memory_order_release); break;
  case Dee_memory_order_seq_cst: DeeAtomicIN_Store(*self,0,memory_order_seq_cst); break;
  default: _dee_invalid_order("clear"); return NULL;
 }
 DeeReturn_None;
}
static DEE_A_REF DeeObject *ASLOT(test_and_set)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:exchange",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_Exchange(*self,1,memory_order_relaxed); break;
/*case Dee_memory_order_consume: v = DeeAtomicIN_Exchange(*self,1,memory_order_consume); break;*/
  case Dee_memory_order_acquire: v = DeeAtomicIN_Exchange(*self,1,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_Exchange(*self,1,memory_order_release); break;
  case Dee_memory_order_acq_rel: v = DeeAtomicIN_Exchange(*self,1,memory_order_acq_rel); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_Exchange(*self,1,memory_order_seq_cst); break;
  default: _dee_invalid_order("test_and_set"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(test_and_set)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 uintn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:test_and_set",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_relaxed); break;
/*case Dee_memory_order_consume: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_consume); break;*/
  case Dee_memory_order_acquire: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_acquire); break;
  case Dee_memory_order_release: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_release); break;
  case Dee_memory_order_acq_rel: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_acq_rel); break;
  case Dee_memory_order_seq_cst: v = (uintn_t)DeeAtomicIN_Exchange(*self,1,memory_order_seq_cst); break;
  default: _dee_invalid_order("test_and_set"); return NULL;
 }
 return DeeUIntN_New(v);
}


static DeeObject *ASLOT(init)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d:init",self) == -1) return NULL;
 DeeReturn_None;
}
static DeeObject *ASLOT(store)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:store",&v,&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: DeeAtomicIN_Store(*self,v,memory_order_relaxed); break;
  case Dee_memory_order_release: DeeAtomicIN_Store(*self,v,memory_order_release); break;
  case Dee_memory_order_seq_cst: DeeAtomicIN_Store(*self,v,memory_order_seq_cst); break;
  default: _dee_invalid_order("store"); return NULL;
 }
 DeeReturn_None;
}

static DEE_A_REF DeeObject *ASLOT(load)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:load",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_Load(*self,memory_order_relaxed); break;
  case Dee_memory_order_consume: v = DeeAtomicIN_Load(*self,memory_order_consume); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_Load(*self,memory_order_acquire); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_Load(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("load"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(load)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 uintn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:load",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = (uintn_t)DeeAtomicIN_Load(*self,memory_order_relaxed); break;
  case Dee_memory_order_consume: v = (uintn_t)DeeAtomicIN_Load(*self,memory_order_consume); break;
  case Dee_memory_order_acquire: v = (uintn_t)DeeAtomicIN_Load(*self,memory_order_acquire); break;
  case Dee_memory_order_seq_cst: v = (uintn_t)DeeAtomicIN_Load(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("load"); return NULL;
 }
 return DeeUIntN_New(v);
}

static DEE_A_REF DeeObject *ASLOT(exchange)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:exchange",&v,&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_Exchange(*self,v,memory_order_relaxed); break;
/*case Dee_memory_order_consume: v = DeeAtomicIN_Exchange(*self,v,memory_order_consume); break;*/
  case Dee_memory_order_acquire: v = DeeAtomicIN_Exchange(*self,v,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_Exchange(*self,v,memory_order_release); break;
  case Dee_memory_order_acq_rel: v = DeeAtomicIN_Exchange(*self,v,memory_order_acq_rel); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_Exchange(*self,v,memory_order_seq_cst); break;
  default: _dee_invalid_order("exchange"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(exchange)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 uintn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "u|d:exchange",&v,&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_relaxed); break;
/*case Dee_memory_order_consume: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_consume); break;*/
  case Dee_memory_order_acquire: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_acquire); break;
  case Dee_memory_order_release: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_release); break;
  case Dee_memory_order_acq_rel: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_acq_rel); break;
  case Dee_memory_order_seq_cst: v = (uintn_t)DeeAtomicIN_Exchange(*self,v,memory_order_seq_cst); break;
  default: _dee_invalid_order("exchange"); return NULL;
 }
 return DeeUIntN_New(v);
}

static DEE_A_REF DeeObject *ASLOT(compare_exchange_weak)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; int r; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_weak",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeWeak(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_weak"); return NULL;
 }
 DeeReturn_Bool(r);
}

static DEE_A_REF DeeObject *ASLOT(compare_exchange_strong)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; int r; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_strong",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: r = DeeAtomicIN_CompareExchangeStrong(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_strong"); return NULL;
 }
 DeeReturn_Bool(r);
}

static DEE_A_REF DeeObject *ASLOT(compare_exchange_weak_val)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_weak_val",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_weak_val"); return NULL;
 }
 return DeeIntN_New(old_v);
}

static DEE_A_REF DeeObject *ASLOT(compare_exchange_strong_val)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_strong_val",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_strong_val"); return NULL;
 }
 return DeeIntN_New(old_v);
}

static DEE_A_REF DeeObject *AUSLOT(compare_exchange_weak_val)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_weak_val",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeWeakVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_weak_val"); return NULL;
 }
 return DeeUIntN_New((uintn_t)old_v);
}

static DEE_A_REF DeeObject *AUSLOT(compare_exchange_strong_val)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t old_v,new_v; DeeMemoryOrder 
  success_order = Dee_memory_order_seq_cst,
  failure_order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "dI" DEE_PP_STR(N) "d|dd:compare_exchange_strong_val",
     &old_v,&new_v,&success_order,&failure_order) == -1) return NULL;
 switch (success_order<<8|failure_order) {
  case Dee_memory_order_relaxed<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_relaxed); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_relaxed); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_relaxed); break;
  case Dee_memory_order_release<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_relaxed); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_relaxed); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_relaxed: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_relaxed); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_consume); break;
  case Dee_memory_order_consume<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_consume); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_consume); break;
  case Dee_memory_order_release<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_consume); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_consume); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_consume: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_consume); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acquire); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_acquire); break;
  case Dee_memory_order_acquire<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_acquire); break;
  case Dee_memory_order_release<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_acquire); break;
  case Dee_memory_order_acq_rel<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acquire); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_acquire: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acquire); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_release); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_release); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_release); break;
//case Dee_memory_order_release<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_release); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_release); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_release: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_release); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_acq_rel); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_acq_rel); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_acq_rel); break;
//case Dee_memory_order_release<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_acq_rel); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_acq_rel); break;
//case Dee_memory_order_seq_cst<<8|Dee_memory_order_acq_rel: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_acq_rel); break;
//case Dee_memory_order_relaxed<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_relaxed,memory_order_seq_cst); break;
//case Dee_memory_order_consume<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_consume,memory_order_seq_cst); break;
//case Dee_memory_order_acquire<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acquire,memory_order_seq_cst); break;
//case Dee_memory_order_release<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_release,memory_order_seq_cst); break;
//case Dee_memory_order_acq_rel<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_acq_rel,memory_order_seq_cst); break;
  case Dee_memory_order_seq_cst<<8|Dee_memory_order_seq_cst: old_v = DeeAtomicIN_CompareExchangeStrongVal(*self,old_v,new_v,memory_order_seq_cst,memory_order_seq_cst); break;
  default: _dee_invalid_order("compare_exchange_strong_val"); return NULL;
 }
 return DeeUIntN_New((uintn_t)old_v);
}

static DEE_A_REF DeeObject *ASLOT(fetch_inc)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:fetch_inc",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_FetchInc(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_FetchInc(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_FetchInc(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_FetchInc(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_inc"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(fetch_inc)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:fetch_inc",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_FetchInc(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_FetchInc(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_FetchInc(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_FetchInc(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_inc"); return NULL;
 }
 return DeeUIntN_New((uintn_t)v);
}

static DEE_A_REF DeeObject *ASLOT(inc_fetch)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:inc_fetch",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_IncFetch(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_IncFetch(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_IncFetch(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_IncFetch(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_inc"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(inc_fetch)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:inc_fetch",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_IncFetch(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_IncFetch(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_IncFetch(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_IncFetch(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("inc_fetch"); return NULL;
 }
 return DeeUIntN_New((uintn_t)v);
}

static DEE_A_REF DeeObject *ASLOT(fetch_dec)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:fetch_dec",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_FetchDec(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_FetchDec(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_FetchDec(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_FetchDec(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_dec"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(fetch_dec)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:fetch_dec",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_FetchDec(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_FetchDec(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_FetchDec(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_FetchDec(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_dec"); return NULL;
 }
 return DeeUIntN_New((uintn_t)v);
}

static DEE_A_REF DeeObject *ASLOT(dec_fetch)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:dec_fetch",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_DecFetch(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_DecFetch(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_DecFetch(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_DecFetch(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("fetch_dec"); return NULL;
 }
 return DeeIntN_New(v);
}

static DEE_A_REF DeeObject *AUSLOT(dec_fetch)(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){
 intn_t v; DeeMemoryOrder order = Dee_memory_order_seq_cst;
 if (DeeTuple_Unpack(args,"|d:dec_fetch",&order) == -1) return NULL;
 switch (order) {
  case Dee_memory_order_relaxed: v = DeeAtomicIN_DecFetch(*self,memory_order_relaxed); break;
  case Dee_memory_order_acquire: v = DeeAtomicIN_DecFetch(*self,memory_order_acquire); break;
  case Dee_memory_order_release: v = DeeAtomicIN_DecFetch(*self,memory_order_release); break;
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_DecFetch(*self,memory_order_seq_cst); break;
  default: _dee_invalid_order("dec_fetch"); return NULL;
 }
 return DeeUIntN_New((uintn_t)v);
}

#define DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(name,Name)\
static DEE_A_REF DeeObject *ASLOT(fetch_##name)(\
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){\
 intn_t r,v; DeeMemoryOrder order = Dee_memory_order_seq_cst;\
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:fetch_" #name,&r,&order) == -1) return NULL;\
 switch (order) {\
  case Dee_memory_order_relaxed: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_relaxed); break;\
  case Dee_memory_order_acquire: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_acquire); break;\
  case Dee_memory_order_release: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_release); break;\
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_seq_cst); break;\
  default: _dee_invalid_order("fetch_" #name); return NULL;\
 }\
 return DeeIntN_New(v);\
}\
static DEE_A_REF DeeObject *AUSLOT(fetch_##name)(\
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){\
 intn_t r,v; DeeMemoryOrder order = Dee_memory_order_seq_cst;\
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:fetch_" #name,&r,&order) == -1) return NULL;\
 switch (order) {\
  case Dee_memory_order_relaxed: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_relaxed); break;\
  case Dee_memory_order_acquire: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_acquire); break;\
  case Dee_memory_order_release: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_release); break;\
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_Fetch##Name(*self,r,memory_order_seq_cst); break;\
  default: _dee_invalid_order("fetch_" #name); return NULL;\
 }\
 return DeeUIntN_New((uintn_t)v);\
}\
static DEE_A_REF DeeObject *ASLOT(name##_fetch)(\
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), intn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){\
 intn_t r,v; DeeMemoryOrder order = Dee_memory_order_seq_cst;\
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:" #name "_fetch",&r,&order) == -1) return NULL;\
 switch (order) {\
  case Dee_memory_order_relaxed: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_relaxed); break;\
  case Dee_memory_order_acquire: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_acquire); break;\
  case Dee_memory_order_release: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_release); break;\
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_seq_cst); break;\
  default: _dee_invalid_order(#name "_fetch"); return NULL;\
 }\
 return DeeIntN_New(v);\
}\
static DEE_A_REF DeeObject *AUSLOT(name##_fetch)(\
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), uintn_t *self, DeeObject *args, void *DEE_UNUSED(closure)){\
 intn_t r,v; DeeMemoryOrder order = Dee_memory_order_seq_cst;\
 if (DeeTuple_Unpack(args,"I" DEE_PP_STR(N) "d|d:" #name "_fetch",&r,&order) == -1) return NULL;\
 switch (order) {\
  case Dee_memory_order_relaxed: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_relaxed); break;\
  case Dee_memory_order_acquire: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_acquire); break;\
  case Dee_memory_order_release: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_release); break;\
  case Dee_memory_order_seq_cst: v = DeeAtomicIN_##Name##Fetch(*self,r,memory_order_seq_cst); break;\
  default: _dee_invalid_order(#name "_fetch"); return NULL;\
 }\
 return DeeUIntN_New((uintn_t)v);\
}

DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(add,Add)
DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(sub,Sub)
DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(and,And)
DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(or,Or)
DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION(xor,Xor)
#undef DEE_PRIVATE_INT_MAKE_ATOMIC_MEMBER_FUNCTION
DEE_COMPILER_MSVC_WARNING_POP

#define DEE_ATOMIC_INTEGER_COMPARE_EXCHANGE_IS_STRONG 1
static struct DeeMethodDef const ASLOT(tp_methods)[] = {
 DEE_METHODDEF_STRUCT_v100("init",member(&ASLOT(init)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("store",member(&ASLOT(store)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_CONST_STRUCT_v100("load",member(&ASLOT(load)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("exchange",member(&ASLOT(exchange)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_weak",member(&ASLOT(compare_exchange_weak)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_strong",member(&ASLOT(compare_exchange_strong)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_weak_val",member(&ASLOT(compare_exchange_weak_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_strong_val",member(&ASLOT(compare_exchange_strong_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
#if DEE_ATOMIC_INTEGER_COMPARE_EXCHANGE_IS_STRONG
 DEE_METHODDEF_STRUCT_v100("compare_exchange",member(&ASLOT(compare_exchange_strong)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_val",member(&ASLOT(compare_exchange_strong_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
#else
 DEE_METHODDEF_STRUCT_v100("compare_exchange",member(&ASLOT(compare_exchange_weak)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_val",member(&ASLOT(compare_exchange_weak_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
#endif
 DEE_METHODDEF_STRUCT_v100("fetch_inc",member(&ASLOT(fetch_inc)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("inc_fetch",member(&ASLOT(inc_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_dec",member(&ASLOT(fetch_dec)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("dec_fetch",member(&ASLOT(dec_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_add",member(&ASLOT(fetch_add)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("add_fetch",member(&ASLOT(add_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_sub",member(&ASLOT(fetch_sub)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("sub_fetch",member(&ASLOT(sub_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_and",member(&ASLOT(fetch_and)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("and_fetch",member(&ASLOT(and_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_or", member(&ASLOT(fetch_or)), DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("or_fetch", member(&ASLOT(or_fetch)), DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_xor",member(&ASLOT(fetch_xor)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("xor_fetch",member(&ASLOT(xor_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("clear",member(&ASLOT(clear)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_STRUCT_v100("test_and_set",member(&ASLOT(test_and_set)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_I,N))),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const AUSLOT(tp_methods)[] = {
 DEE_METHODDEF_STRUCT_v100("init",member(&ASLOT(init)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("store",member(&ASLOT(store)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_CONST_STRUCT_v100("load",member(&AUSLOT(load)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("exchange",member(&AUSLOT(exchange)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_weak",member(&ASLOT(compare_exchange_weak)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_strong",member(&ASLOT(compare_exchange_strong)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_weak_val",member(&AUSLOT(compare_exchange_weak_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_strong_val",member(&AUSLOT(compare_exchange_strong_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
#if DEE_ATOMIC_INTEGER_COMPARE_EXCHANGE_IS_STRONG
 DEE_METHODDEF_STRUCT_v100("compare_exchange",member(&ASLOT(compare_exchange_strong)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_val",member(&AUSLOT(compare_exchange_strong_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
#else
 DEE_METHODDEF_STRUCT_v100("compare_exchange",member(&ASLOT(compare_exchange_weak)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("compare_exchange_val",member(&AUSLOT(compare_exchange_weak_val)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
#endif
 DEE_METHODDEF_STRUCT_v100("fetch_inc",member(&AUSLOT(fetch_inc)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("inc_fetch",member(&AUSLOT(inc_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_dec",member(&AUSLOT(fetch_dec)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("dec_fetch",member(&AUSLOT(dec_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_add",member(&AUSLOT(fetch_add)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("add_fetch",member(&AUSLOT(add_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_sub",member(&AUSLOT(fetch_sub)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("sub_fetch",member(&AUSLOT(sub_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_and",member(&AUSLOT(fetch_and)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("and_fetch",member(&AUSLOT(and_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_or", member(&AUSLOT(fetch_or)), DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("or_fetch", member(&AUSLOT(or_fetch)), DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("fetch_xor",member(&AUSLOT(fetch_xor)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("xor_fetch",member(&AUSLOT(xor_fetch)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("clear",member(&ASLOT(clear)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_STRUCT_v100("test_and_set",member(&AUSLOT(test_and_set)),DEE_DOC_AUTO_ARGS("T=" DEE_PP_CAT_2(INTEGER_NAME_UI,N))),
 DEE_METHODDEF_END_v100
};

static struct DeeMemberDef const SLOT(tp_class_members)[] = {
 DEE_MEMBERDEF_CONST_v100("range",object,&DEE_PP_CAT_3(DeeRangeInt,N,Iterator_Type)),
 DEE_MEMBERDEF_CONST_v100("signed",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("unsigned",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("atomic",object,&DEE_PP_CAT_3(DeeAtomicInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("_Atomic",object,&DEE_PP_CAT_3(DeeAtomicInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed__",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned__",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const USLOT(tp_class_members)[] = {
 DEE_MEMBERDEF_CONST_v100("range",object,&DEE_PP_CAT_3(DeeRangeUInt,N,Iterator_Type)),
 DEE_MEMBERDEF_CONST_v100("signed",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("unsigned",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("atomic",object,&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("_Atomic",object,&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed__",object,&DEE_PP_CAT_3(DeeInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned__",object,&DEE_PP_CAT_3(DeeUInt,N,_Type)),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const ASLOT(tp_class_members)[] = {
 DEE_MEMBERDEF_CONST_v100("signed",object,&DEE_PP_CAT_3(DeeAtomicInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("unsigned",object,&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed",object,&DEE_PP_CAT_3(DeeAtomicInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__signed__",object,&DEE_PP_CAT_3(DeeAtomicInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned",object,&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type)),
 DEE_MEMBERDEF_CONST_v100("__unsigned__",object,&DEE_PP_CAT_3(DeeAtomicUInt,N,_Type)),
 DEE_MEMBERDEF_END_v100
};

static int DEE_CALL SLOT(tp_marshal_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 return DeeFile_GetLe(infile,self->i_value.u);
}
static int DEE_CALL SLOT(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return DeeFile_PutLe(outfile,self->i_value.u);
}
static int DEE_CALL ASLOT(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeIntNObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return DeeFile_PutLe(outfile,DeeAtomicIN_Load(self->i_value.u,memory_order_seq_cst));
}

static struct DeeTypeMarshal SLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(int,N))),
 member(&SLOT(tp_marshal_ctor)),member(&SLOT(tp_marshal_put)));
static struct DeeTypeMarshal USLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(uint,N))),
 member(&SLOT(tp_marshal_ctor)),member(&SLOT(tp_marshal_put)));
static struct DeeTypeMarshal ASLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(atomic_int,N))),
 member(&SLOT(tp_marshal_ctor)),member(&ASLOT(tp_marshal_put)));
static struct DeeTypeMarshal AUSLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(atomic_uint,N))),
 member(&SLOT(tp_marshal_ctor)),member(&ASLOT(tp_marshal_put)));



DeeStructuredTypeObject DEE_PP_CAT_3(DeeInt,N,_Type) = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),
   member(DEE_PP_CAT_2(INTEGER_NAME_I,N)),
   member("() -> " DEE_PP_CAT_2(INTEGER_NAME_I,N) "\n"
          "(object ob) -> " DEE_PP_CAT_2(INTEGER_NAME_I,N)),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DeeStructured_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeIntNObject),
   member(&SLOT(tp_alloc)),member(&_DeeGeneric_Noop),
   member(&SLOT(tp_copy_ctor)),null,member(&SLOT(tp_any_ctor))),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&SLOT(tp_free)),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&SLOT(tp_copy_assign)),null,
   member(&SLOT(tp_any_assign))),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&SLOT(tp_str)),member(&SLOT(tp_str)),
   member(&SLOT(tp_int32)),member(&SLOT(tp_int64)),
   member(&SLOT(tp_double))),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define SLOT_TP_POS SLOT(tp_pos)
#else
#define SLOT_TP_POS _DeeGeneric_ReturnSelf
#endif
  DEE_TYPE_OBJECT_MATH_v101(
   member(&SLOT(tp_not)),member(&SLOT(tp_bool)),
   member(&SLOT(tp_inv)),
   member(&SLOT_TP_POS),member(&SLOT(tp_neg)),
   member(&SLOT(tp_inc)),member(&SLOT(tp_incpost)),
   member(&SLOT(tp_dec)),member(&SLOT(tp_decpost)),
   member(&SLOT(tp_add)),member(&SLOT(tp_iadd)),
   member(&SLOT(tp_sub)),member(&SLOT(tp_isub)),
   member(&SLOT(tp_mul)),member(&SLOT(tp_imul)),
   member(&SLOT(tp_div)),member(&SLOT(tp_idiv)),
   member(&SLOT(tp_mod)),member(&SLOT(tp_imod)),
   member(&SLOT(tp_shl)),member(&SLOT(tp_ishl)),
   member(&SLOT(tp_shr)),member(&SLOT(tp_ishr)),
   member(&SLOT(tp_and)),member(&SLOT(tp_iand)),
   member(&SLOT(tp_or)),member(&SLOT(tp_ior)),
   member(&SLOT(tp_xor)),member(&SLOT(tp_ixor)),
   member(&SLOT(tp_pow)),member(&SLOT(tp_ipow)),
   member(&SLOT(tp_hash))),
#undef SLOT_TP_POS
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&SLOT(tp_cmp_lo)),
   member(&SLOT(tp_cmp_le)),
   member(&SLOT(tp_cmp_eq)),
   member(&SLOT(tp_cmp_ne)),
   member(&SLOT(tp_cmp_gr)),
   member(&SLOT(tp_cmp_ge))),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
   member(SLOT(tp_class_members)),null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&SLOT(tp_marshal))),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeIntNObject)-sizeof(DeeObject)),
  member(&SLOT(tp_p_any_ctor))),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&SLOT(tp_p_any_assign))),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&SLOT(tp_p_str)),member(&SLOT(tp_p_str)),
  member(&SLOT(tp_p_int32)),member(&SLOT(tp_p_int64)),
  member(&SLOT(tp_p_double))),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&SLOT(tp_p_not)),member(&SLOT(tp_p_bool)),member(&SLOT(tp_p_inv)),
  member(&SLOT(tp_p_pos)),member(&SLOT(tp_p_neg)),
  member(&SLOT(tp_p_inc)),member(&SLOT(tp_p_incpost)),
  member(&SLOT(tp_p_dec)),member(&SLOT(tp_p_decpost)),
  member(&SLOT(tp_p_add)),member(&SLOT(tp_p_iadd)),
  member(&SLOT(tp_p_sub)),member(&SLOT(tp_p_isub)),
  member(&SLOT(tp_p_mul)),member(&SLOT(tp_p_imul)),
  member(&SLOT(tp_p_div)),member(&SLOT(tp_p_idiv)),
  member(&SLOT(tp_p_mod)),member(&SLOT(tp_p_imod)),
  member(&SLOT(tp_p_shl)),member(&SLOT(tp_p_ishl)),
  member(&SLOT(tp_p_shr)),member(&SLOT(tp_p_ishr)),
  member(&SLOT(tp_p_and)),member(&SLOT(tp_p_iand)),
  member(&SLOT(tp_p_or)),member(&SLOT(tp_p_ior)),
  member(&SLOT(tp_p_xor)),member(&SLOT(tp_p_ixor)),
  member(&SLOT(tp_p_pow)),member(&SLOT(tp_p_ipow)),
  member(&SLOT(tp_p_hash))),
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&SLOT(tp_p_cmp_lo)),member(&SLOT(tp_p_cmp_le)),
  member(&SLOT(tp_p_cmp_eq)),member(&SLOT(tp_p_cmp_ne)),
  member(&SLOT(tp_p_cmp_gr)),member(&SLOT(tp_p_cmp_ge))),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};


DeeStructuredTypeObject DEE_PP_CAT_3(DeeUInt,N,_Type) = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),
   member(DEE_PP_CAT_2(INTEGER_NAME_UI,N)),
   member("() -> " DEE_PP_CAT_2(INTEGER_NAME_UI,N) "\n"
          "(object ob) -> " DEE_PP_CAT_2(INTEGER_NAME_UI,N)),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DeeStructured_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeIntNObject),
   member(&SLOT(tp_alloc)),member(&_DeeGeneric_Noop),
   member(&SLOT(tp_copy_ctor)),null,member(&USLOT(tp_any_ctor))),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&SLOT(tp_free)),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&SLOT(tp_copy_assign)),null,
   member(&USLOT(tp_any_assign))),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&USLOT(tp_str)),member(&USLOT(tp_str)),
   member(&USLOT(tp_int32)),member(&USLOT(tp_int64)),
   member(&USLOT(tp_double))),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define USLOT_TP_POS USLOT(tp_pos)
#define USLOT_TP_NEG USLOT(tp_neg)
#else
#define USLOT_TP_POS _DeeGeneric_ReturnSelf
#define USLOT_TP_NEG _DeeGeneric_ReturnSelf
#endif
  DEE_TYPE_OBJECT_MATH_v101(
   member(&SLOT(tp_not)),member(&SLOT(tp_bool)),member(&USLOT(tp_inv)),
   member(&USLOT_TP_POS),member(&USLOT_TP_NEG),
   member(&SLOT(tp_inc)),member(&USLOT(tp_incpost)),
   member(&SLOT(tp_dec)),member(&USLOT(tp_decpost)),
   member(&USLOT(tp_add)),member(&USLOT(tp_iadd)),
   member(&USLOT(tp_sub)),member(&USLOT(tp_isub)),
   member(&USLOT(tp_mul)),member(&USLOT(tp_imul)),
   member(&USLOT(tp_div)),member(&USLOT(tp_idiv)),
   member(&USLOT(tp_mod)),member(&USLOT(tp_imod)),
   member(&USLOT(tp_shl)),member(&USLOT(tp_ishl)),
   member(&USLOT(tp_shr)),member(&USLOT(tp_ishr)),
   member(&USLOT(tp_and)),member(&USLOT(tp_iand)),
   member(&USLOT(tp_or)),member(&USLOT(tp_ior)),
   member(&USLOT(tp_xor)),member(&USLOT(tp_ixor)),
   member(&USLOT(tp_pow)),member(&USLOT(tp_ipow)),
   member(&SLOT(tp_hash))),
#undef USLOT_TP_POS
#undef USLOT_TP_NEG
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&USLOT(tp_cmp_lo)),member(&USLOT(tp_cmp_le)),
   member(&USLOT(tp_cmp_eq)),member(&USLOT(tp_cmp_ne)),
   member(&USLOT(tp_cmp_gr)),member(&USLOT(tp_cmp_ge))),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
   member(USLOT(tp_class_members)),null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&USLOT(tp_marshal))),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeIntNObject)-sizeof(DeeObject)),
  member(&USLOT(tp_p_any_ctor))),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&USLOT(tp_p_any_assign))),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&USLOT(tp_p_str)),member(&USLOT(tp_p_str)),
  member(&USLOT(tp_p_int32)),member(&USLOT(tp_p_int64)),
  member(&USLOT(tp_p_double))),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define USLOT_TP_P_NEG USLOT(tp_p_neg)
#else
#define USLOT_TP_P_NEG USLOT(tp_p_pos)
#endif
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&SLOT(tp_p_not)),member(&SLOT(tp_p_bool)),member(&USLOT(tp_p_inv)),
  member(&USLOT(tp_p_pos)),member(&USLOT_TP_P_NEG),
  member(&SLOT(tp_p_inc)),member(&USLOT(tp_p_incpost)),
  member(&SLOT(tp_p_dec)),member(&USLOT(tp_p_decpost)),
  member(&USLOT(tp_p_add)),member(&SLOT(tp_p_iadd)),
  member(&USLOT(tp_p_sub)),member(&SLOT(tp_p_isub)),
  member(&USLOT(tp_p_mul)),member(&SLOT(tp_p_imul)),
  member(&USLOT(tp_p_div)),member(&SLOT(tp_p_idiv)),
  member(&USLOT(tp_p_mod)),member(&SLOT(tp_p_imod)),
  member(&USLOT(tp_p_shl)),member(&SLOT(tp_p_ishl)),
  member(&USLOT(tp_p_shr)),member(&SLOT(tp_p_ishr)),
  member(&USLOT(tp_p_and)),member(&SLOT(tp_p_iand)),
  member(&USLOT(tp_p_or)),member(&SLOT(tp_p_ior)),
  member(&USLOT(tp_p_xor)),member(&SLOT(tp_p_ixor)),
  member(&USLOT(tp_p_pow)),member(&USLOT(tp_p_ipow)),
  member(&SLOT(tp_p_hash))),
#undef USLOT_TP_P_NEG
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&USLOT(tp_p_cmp_lo)),member(&USLOT(tp_p_cmp_le)),
  member(&USLOT(tp_p_cmp_eq)),member(&USLOT(tp_p_cmp_ne)),
  member(&USLOT(tp_p_cmp_gr)),member(&USLOT(tp_p_cmp_ge))),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};



DeeStructuredTypeObject DEE_PP_CAT_3(DeeAtomicInt,N,_Type) = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),
   member(INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_I,N)),
   member("() -> " INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_I,N) "\n"
          "(object ob) -> " INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_I,N)),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DEE_PP_CAT_3(DeeInt,N,_Type))),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeIntNObject),
   member(&SLOT(tp_alloc)),member(&_DeeGeneric_Noop),
   member(&SLOT(tp_copy_ctor)),null,member(&SLOT(tp_any_ctor))),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&SLOT(tp_free)),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&ASLOT(tp_copy_assign)),null,
   member(&ASLOT(tp_any_assign))),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&ASLOT(tp_str)),member(&ASLOT(tp_str)),
   member(&ASLOT(tp_int32)),member(&ASLOT(tp_int64)),
   member(&ASLOT(tp_double))),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define ASLOT_TP_POS ASLOT(tp_pos)
#else
#define ASLOT_TP_POS _DeeGeneric_ReturnSelf
#endif
  DEE_TYPE_OBJECT_MATH_v101(
   member(&ASLOT(tp_not)),member(&ASLOT(tp_bool)),member(&ASLOT(tp_inv)),
   member(&ASLOT_TP_POS),member(&ASLOT(tp_neg)),
   member(&ASLOT(tp_inc)),member(&ASLOT(tp_incpost)),
   member(&ASLOT(tp_dec)),member(&ASLOT(tp_decpost)),
   member(&ASLOT(tp_add)),member(&ASLOT(tp_iadd)),
   member(&ASLOT(tp_sub)),member(&ASLOT(tp_isub)),
   member(&ASLOT(tp_mul)),member(&ASLOT(tp_imul)),
   member(&ASLOT(tp_div)),member(&ASLOT(tp_idiv)),
   member(&ASLOT(tp_mod)),member(&ASLOT(tp_imod)),
   member(&ASLOT(tp_shl)),member(&ASLOT(tp_ishl)),
   member(&ASLOT(tp_shr)),member(&ASLOT(tp_ishr)),
   member(&ASLOT(tp_and)),member(&ASLOT(tp_iand)),
   member(&ASLOT(tp_or)),member(&ASLOT(tp_ior)),
   member(&ASLOT(tp_xor)),member(&ASLOT(tp_ixor)),
   member(&ASLOT(tp_pow)),member(&ASLOT(tp_ipow)),
   member(&ASLOT(tp_hash))),
#undef ASLOT_TP_POS
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&ASLOT(tp_cmp_lo)),member(&ASLOT(tp_cmp_le)),
   member(&ASLOT(tp_cmp_eq)),member(&ASLOT(tp_cmp_ne)),
   member(&ASLOT(tp_cmp_gr)),member(&ASLOT(tp_cmp_ge))),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,
   member(ASLOT(tp_methods)),
   member(ASLOT(tp_class_members)),null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&ASLOT(tp_marshal))),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeIntNObject)-sizeof(DeeObject)),
  member(&SLOT(tp_p_any_ctor))),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&ASLOT(tp_p_any_assign))),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&ASLOT(tp_p_str)),
  member(&ASLOT(tp_p_str)),
  member(&ASLOT(tp_p_int32)),
  member(&ASLOT(tp_p_int64)),
  member(&ASLOT(tp_p_double))),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&ASLOT(tp_p_not)),member(&ASLOT(tp_p_bool)),member(&ASLOT(tp_p_inv)),
  member(&ASLOT(tp_p_pos)),member(&ASLOT(tp_p_neg)),
  member(&ASLOT(tp_p_inc)),member(&ASLOT(tp_p_incpost)),
  member(&ASLOT(tp_p_dec)),member(&ASLOT(tp_p_decpost)),
  member(&ASLOT(tp_p_add)),member(&ASLOT(tp_p_iadd)),
  member(&ASLOT(tp_p_sub)),member(&ASLOT(tp_p_isub)),
  member(&ASLOT(tp_p_mul)),member(&ASLOT(tp_p_imul)),
  member(&ASLOT(tp_p_div)),member(&ASLOT(tp_p_idiv)),
  member(&ASLOT(tp_p_mod)),member(&ASLOT(tp_p_imod)),
  member(&ASLOT(tp_p_shl)),member(&ASLOT(tp_p_ishl)),
  member(&ASLOT(tp_p_shr)),member(&ASLOT(tp_p_ishr)),
  member(&ASLOT(tp_p_and)),member(&ASLOT(tp_p_iand)),
  member(&ASLOT(tp_p_or)),member(&ASLOT(tp_p_ior)),
  member(&ASLOT(tp_p_xor)),member(&ASLOT(tp_p_ixor)),
  member(&ASLOT(tp_p_pow)),member(&ASLOT(tp_p_ipow)),
  member(&ASLOT(tp_p_hash))),
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&ASLOT(tp_p_cmp_lo)),member(&ASLOT(tp_p_cmp_le)),
  member(&ASLOT(tp_p_cmp_eq)),member(&ASLOT(tp_p_cmp_ne)),
  member(&ASLOT(tp_p_cmp_gr)),member(&ASLOT(tp_p_cmp_ge))),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};


DeeStructuredTypeObject DEE_PP_CAT_3(DeeAtomicUInt,N,_Type) = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),
   member(INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_UI,N)),
   member("() -> " INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_UI,N) "\n"
          "(object ob) -> " INTEGER_NAME_ATOMIC_PREFIX DEE_PP_CAT_2(INTEGER_NAME_UI,N)),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DEE_PP_CAT_3(DeeUInt,N,_Type))),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeIntNObject),
   member(&SLOT(tp_alloc)),member(&_DeeGeneric_Noop),
   member(&SLOT(tp_copy_ctor)),null,member(&USLOT(tp_any_ctor))),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&SLOT(tp_free)),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&ASLOT(tp_copy_assign)),null,
   member(&AUSLOT(tp_any_assign))),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&AUSLOT(tp_str)),member(&AUSLOT(tp_str)),
   member(&AUSLOT(tp_int32)),member(&AUSLOT(tp_int64)),
   member(&AUSLOT(tp_double))),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define AUSLOT_TP_POS AUSLOT(tp_pos)
#define AUSLOT_TP_NEG AUSLOT(tp_neg)
#else
#define AUSLOT_TP_POS _DeeGeneric_ReturnSelf
#define AUSLOT_TP_NEG _DeeGeneric_ReturnSelf
#endif
  DEE_TYPE_OBJECT_MATH_v101(
   member(&ASLOT(tp_not)),member(&ASLOT(tp_bool)),member(&AUSLOT(tp_inv)),
   member(&AUSLOT_TP_POS),member(&AUSLOT_TP_NEG),
   member(&ASLOT(tp_inc)),member(&AUSLOT(tp_incpost)),
   member(&ASLOT(tp_dec)),member(&AUSLOT(tp_decpost)),
   member(&AUSLOT(tp_add)),member(&AUSLOT(tp_iadd)),
   member(&AUSLOT(tp_sub)),member(&AUSLOT(tp_isub)),
   member(&AUSLOT(tp_mul)),member(&AUSLOT(tp_imul)),
   member(&AUSLOT(tp_div)),member(&AUSLOT(tp_idiv)),
   member(&AUSLOT(tp_mod)),member(&AUSLOT(tp_imod)),
   member(&AUSLOT(tp_shl)),member(&AUSLOT(tp_ishl)),
   member(&AUSLOT(tp_shr)),member(&AUSLOT(tp_ishr)),
   member(&AUSLOT(tp_and)),member(&AUSLOT(tp_iand)),
   member(&AUSLOT(tp_or)),member(&AUSLOT(tp_ior)),
   member(&AUSLOT(tp_xor)),member(&AUSLOT(tp_ixor)),
   member(&AUSLOT(tp_pow)),member(&AUSLOT(tp_ipow)),
   member(&ASLOT(tp_hash))),
#undef AUSLOT_TP_POS
#undef AUSLOT_TP_NEG
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&AUSLOT(tp_cmp_lo)),member(&AUSLOT(tp_cmp_le)),
   member(&AUSLOT(tp_cmp_eq)),member(&AUSLOT(tp_cmp_ne)),
   member(&AUSLOT(tp_cmp_gr)),member(&AUSLOT(tp_cmp_ge))),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,
   member(AUSLOT(tp_methods)),
   member(ASLOT(tp_class_members)),null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&AUSLOT(tp_marshal))),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeIntNObject)-sizeof(DeeObject)),
  member(&USLOT(tp_p_any_ctor))),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&AUSLOT(tp_p_any_assign))),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&AUSLOT(tp_p_str)),member(&AUSLOT(tp_p_str)),
  member(&AUSLOT(tp_p_int32)),member(&AUSLOT(tp_p_int64)),
  member(&AUSLOT(tp_p_double))),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
#if SIZEOF_INTN_T < DEE_CONFIG_RT_SIZEOF_INT
#define AUSLOT_TP_P_NEG AUSLOT(tp_p_neg)
#else
#define AUSLOT_TP_P_NEG AUSLOT(tp_p_pos)
#endif
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&ASLOT(tp_p_not)),member(&ASLOT(tp_p_bool)),member(&AUSLOT(tp_p_inv)),
  member(&AUSLOT(tp_p_pos)),member(&AUSLOT_TP_P_NEG),
  member(&ASLOT(tp_p_inc)),member(&AUSLOT(tp_p_incpost)),
  member(&ASLOT(tp_p_dec)),member(&AUSLOT(tp_p_decpost)),
  member(&AUSLOT(tp_p_add)),member(&ASLOT(tp_p_iadd)),
  member(&AUSLOT(tp_p_sub)),member(&ASLOT(tp_p_isub)),
  member(&AUSLOT(tp_p_mul)),member(&ASLOT(tp_p_imul)),
  member(&AUSLOT(tp_p_div)),member(&ASLOT(tp_p_idiv)),
  member(&AUSLOT(tp_p_mod)),member(&ASLOT(tp_p_imod)),
  member(&AUSLOT(tp_p_shl)),member(&ASLOT(tp_p_ishl)),
  member(&AUSLOT(tp_p_shr)),member(&ASLOT(tp_p_ishr)),
  member(&AUSLOT(tp_p_and)),member(&ASLOT(tp_p_iand)),
  member(&AUSLOT(tp_p_or)),member(&ASLOT(tp_p_ior)),
  member(&AUSLOT(tp_p_xor)),member(&ASLOT(tp_p_ixor)),
  member(&AUSLOT(tp_p_pow)),member(&AUSLOT(tp_p_ipow)),
  member(&ASLOT(tp_p_hash))),
#undef AUSLOT_TP_P_NEG
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&AUSLOT(tp_p_cmp_lo)),member(&AUSLOT(tp_p_cmp_le)),
  member(&AUSLOT(tp_p_cmp_eq)),member(&AUSLOT(tp_p_cmp_ne)),
  member(&AUSLOT(tp_p_cmp_gr)),member(&AUSLOT(tp_p_cmp_ge))),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};


//////////////////////////////////////////////////////////////////////////
// Range iterators
#define DeeRangeIntNIteratorObject      DEE_PP_CAT_3(DeeRangeInt,N,IteratorObject)
#define DeeRangeUIntNIteratorObject     DEE_PP_CAT_3(DeeRangeUInt,N,IteratorObject)
#define R_SLOT(name)   DEE_PP_CAT_4(_deerangeint,N,iterator_,name)
#define R_USLOT(name)  DEE_PP_CAT_4(_deerangeuint,N,iterator_,name)
#define FMT  DEE_TYPES_IPRINTF(SIZEOF_INTN_T)
#define UFMT DEE_TYPES_IUPRINTF(SIZEOF_INTN_T)

DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeIntNIteratorObject) *
DEE_PP_CAT_3(DeeRangeInt,N,Iterator_New)(DEE_A_IN intn_t lo, DEE_A_IN intn_t hi, DEE_A_IN intn_t step) {
 DeeRangeIntNIteratorObject *result;
 DEE_ASSERTF(step != 0,"Step in an integer iterator may not be zero");
 if ((result = DeeObject_MALLOCF(DeeRangeIntNIteratorObject,
  "range(%d,%d,%d)",(int)lo,(int)hi,(int)step)) != NULL) {
  DeeObject_INIT(result,&DEE_PP_CAT_3(DeeRangeInt,N,Iterator_Type));
  result->rii_begin = lo;
  result->rii_end = hi;
  result->rii_step = step;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeUIntNIteratorObject) *
DEE_PP_CAT_3(DeeRangeUInt,N,Iterator_New)(DEE_A_IN uintn_t lo, DEE_A_IN uintn_t hi, DEE_A_IN uintn_t step) {
 DeeRangeUIntNIteratorObject *result;
 DEE_ASSERTF(step != 0,"Step in an integer iterator may not be zero");
 if ((result = DeeObject_MALLOCF(DeeRangeUIntNIteratorObject,
  "range(%u,%u,%u)",(unsigned)lo,(unsigned)hi,(unsigned)step)) != NULL) {
  DeeObject_INIT(result,&DEE_PP_CAT_3(DeeRangeInt,N,Iterator_Type));
  result->rii_begin = lo;
  result->rii_end = hi;
  result->rii_step = step;
 }
 return (DeeObject *)result;
}


static int R_SLOT(tp_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeRangeIntNIteratorObject *self) {
 self->rii_begin = 0;
 self->rii_end = 0;
 self->rii_step = 1;
 return 0;
}

#ifndef DEE_PRIVATE_RANGE_ZERO_TEMP_ERROR_DEFINED
#define DEE_PRIVATE_RANGE_ZERO_TEMP_ERROR_DEFINED 1
DeeError_NEW_EXTERN(_DeeError_ValueError_RangeZeroStep,
                    &DeeErrorType_ValueError,"Step argument of range must be non-zero");
#endif
static int R_SLOT(tp_copy_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeRangeIntNIteratorObject *self,
 DeeRangeIntNIteratorObject *right) {
 self->rii_begin = right->rii_begin;
 self->rii_end = right->rii_end;
 self->rii_step = right->rii_step;
 return 0;
}
static int R_SLOT(tp_copy_assign)(
 DeeRangeIntNIteratorObject *self,
 DeeRangeIntNIteratorObject *right) {
 self->rii_begin = right->rii_begin;
 self->rii_end = right->rii_end;
 self->rii_step = right->rii_step;
 return 0;
}
static int R_SLOT(tp_any_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), 
 DeeRangeIntNIteratorObject *self, DeeObject *args) {
 if (DeeTuple_SIZE(args) == 1) {
  DeeObject *arg0 = DeeTuple_GET(args,0);
  if (DeeRangeInt8Iterator_Check(arg0) || DeeRangeUInt8Iterator_Check(arg0)) {
   self->rii_begin = (intn_t)((DeeRangeInt8IteratorObject *)arg0)->rii_begin;
   self->rii_end = (intn_t)((DeeRangeInt8IteratorObject *)arg0)->rii_end;
   self->rii_step = (intn_t)((DeeRangeInt8IteratorObject *)arg0)->rii_step;
   return 0;
  }
  if (DeeRangeInt16Iterator_Check(arg0) || DeeRangeUInt16Iterator_Check(arg0)) {
   self->rii_begin = (intn_t)((DeeRangeInt16IteratorObject *)arg0)->rii_begin;
   self->rii_end = (intn_t)((DeeRangeInt16IteratorObject *)arg0)->rii_end;
   self->rii_step = (intn_t)((DeeRangeInt16IteratorObject *)arg0)->rii_step;
   return 0;
  }
  if (DeeRangeInt32Iterator_Check(arg0) || DeeRangeUInt32Iterator_Check(arg0)) {
   self->rii_begin = (intn_t)((DeeRangeInt32IteratorObject *)arg0)->rii_begin;
   self->rii_end = (intn_t)((DeeRangeInt32IteratorObject *)arg0)->rii_end;
   self->rii_step = (intn_t)((DeeRangeInt32IteratorObject *)arg0)->rii_step;
   return 0;
  }
  if (DeeRangeInt64Iterator_Check(arg0) || DeeRangeUInt64Iterator_Check(arg0)) {
   self->rii_begin = (intn_t)((DeeRangeInt64IteratorObject *)arg0)->rii_begin;
   self->rii_end = (intn_t)((DeeRangeInt64IteratorObject *)arg0)->rii_end;
   self->rii_step = (intn_t)((DeeRangeInt64IteratorObject *)arg0)->rii_step;
   return 0;
  }
 }
 self->rii_step = 1;
 if (DeeTuple_Unpack(args,FMT FMT "|" FMT,&self->rii_begin,&self->rii_end,&self->rii_step) == -1) return -1;
 if (!self->rii_step) { DeeError_Throw((DeeObject *)&_DeeError_ValueError_RangeZeroStep); return -1; }
 return 0;
}
static DeeObject *R_SLOT(tp_str)(DeeRangeIntNIteratorObject *self) {
 return self->rii_step == 1
  ? DeeString_Newf("[" FMT "..." FMT "]",DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst),self->rii_end)
  : DeeString_Newf("[" FMT "..." FMT " / " FMT "]",DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst),self->rii_end,self->rii_step);
}
static DeeObject *R_USLOT(tp_str)(DeeRangeUIntNIteratorObject *self) {
 return self->rii_step == 1
  ? DeeString_Newf("[" UFMT "..." UFMT "]",DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst),self->rii_end)
  : DeeString_Newf("[" UFMT "..." UFMT " / " UFMT "]",DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst),self->rii_end,self->rii_step);
}
static int R_SLOT(tp_bool)(DeeRangeIntNIteratorObject *self) { return DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) < self->rii_end; }
static int R_USLOT(tp_bool)(DeeRangeUIntNIteratorObject *self) { return (uintn_t)DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) < self->rii_end; }
static DeeObject *R_SLOT(tp_not)(DeeRangeIntNIteratorObject *self) { DeeReturn_Bool(DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) >= self->rii_end); }
static DeeObject *R_USLOT(tp_not)(DeeRangeUIntNIteratorObject *self) { DeeReturn_Bool((uintn_t)DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) >= self->rii_end); }
static DeeObject *R_SLOT(tp_seq_size)(DeeRangeIntNIteratorObject *self) {
 intn_t loval = DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst);
 if (loval >= self->rii_end) return DeeUIntN_New(0);
 return DeeUIntN_New((uintn_t)((self->rii_end-loval)/self->rii_step));
}
static DeeObject *R_USLOT(tp_seq_size)(DeeRangeUIntNIteratorObject *self) {
 uintn_t loval = (uintn_t)DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst);
 if (loval >= self->rii_end) return DeeUIntN_New(0);
 return DeeUIntN_New((uintn_t)((self->rii_end-loval)/self->rii_step));
}
static DeeObject *R_SLOT(tp_seq_contains)(DeeRangeIntNIteratorObject *self, DeeObject *ob) {
 intn_t v;
 if (DeeObject_IntN(ob,&v) != 0) return NULL;
 DeeReturn_Bool(v >= DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) && v < self->rii_end);
}
static DeeObject *R_USLOT(tp_seq_contains)(DeeRangeUIntNIteratorObject *self, DeeObject *ob) {
 uintn_t v;
 if (DeeObject_UIntN(ob,&v) != 0) return NULL;
 DeeReturn_Bool(v >= (uintn_t)DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst) && v < self->rii_end);
}
static int R_SLOT(tp_seq_iter_next)(DeeRangeIntNIteratorObject *self, DeeObject **result) {
 // Use atomics to ensure thread-safety
 intn_t resval = (intn_t)DeeAtomicIN_FetchAdd(self->rii_begin,self->rii_step,memory_order_seq_cst);
 if (resval >= self->rii_end) return 1; // Iterator end
 return ((*result = DeeIntN_New(resval)) != NULL) ? 0 : -1;
}
static int R_USLOT(tp_seq_iter_next)(DeeRangeUIntNIteratorObject *self, DeeObject **result) {
 // Use atomics to ensure thread-safety
 uintn_t resval = (uintn_t)DeeAtomicIN_FetchAdd(self->rii_begin,self->rii_step,memory_order_seq_cst);
 if (resval >= self->rii_end) return 1; // Iterator end
 return ((*result = DeeUIntN_New(resval)) != NULL) ? 0 : -1;
}
static struct DeeMemberDef const R_SLOT(tp_members)[] = {
 DEE_MEMBERDEF_v100(DeeRangeIntNIteratorObject,rii_begin,atomic(intn_t)),
 DEE_MEMBERDEF_v100(DeeRangeIntNIteratorObject,rii_end,atomic(intn_t)),
 DEE_MEMBERDEF_RO_v100(DeeRangeIntNIteratorObject,rii_step,atomic(intn_t)),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const R_USLOT(tp_members)[] = {
 DEE_MEMBERDEF_v100(DeeRangeUIntNIteratorObject,rii_begin,atomic(uintn_t)),
 DEE_MEMBERDEF_v100(DeeRangeUIntNIteratorObject,rii_end,atomic(uintn_t)),
 DEE_MEMBERDEF_RO_v100(DeeRangeUIntNIteratorObject,rii_step,atomic(uintn_t)),
 DEE_MEMBERDEF_END_v100
};

static int DEE_CALL R_SLOT(tp_marshal_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeRangeUIntNIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 if (DeeFile_GetLe(infile,self->rii_begin) != 0) return -1;
 if (DeeFile_GetLe(infile,self->rii_end) != 0) return -1;
 return DeeFile_GetLe(infile,self->rii_step);
}
static int DEE_CALL R_SLOT(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeRangeUIntNIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 if (DeeFile_PutLe(outfile,DeeAtomicIN_Load(self->rii_begin,memory_order_seq_cst)) != 0) return -1;
 if (DeeFile_PutLe(outfile,DeeAtomicIN_Load(self->rii_end,memory_order_seq_cst)) != 0) return -1;
 return DeeFile_PutLe(outfile,DeeAtomicIN_Load(self->rii_step,memory_order_seq_cst));
}

static struct DeeTypeMarshal R_SLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_3(int,N,_range))),
 member(&R_SLOT(tp_marshal_ctor)),member(&R_SLOT(tp_marshal_put)));
static struct DeeTypeMarshal R_USLOT(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_3(uint,N,_range))),
 member(&R_SLOT(tp_marshal_ctor)),member(&R_SLOT(tp_marshal_put)));


DeeTypeObject DEE_PP_CAT_3(DeeRangeInt,N,Iterator_Type) = {
 DEE_TYPE_OBJECT_HEAD_v100(member(DEE_PP_CAT_2(INTEGER_NAME_I,N) ".range"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
         DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_NO_ITERATE_EFFECT|
         DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeRangeIntNIteratorObject),null,
  member(&R_SLOT(tp_ctor)),
  member(&R_SLOT(tp_copy_ctor)),null,
  member(&R_SLOT(tp_any_ctor))),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&R_SLOT(tp_copy_assign)),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&R_SLOT(tp_str)),
  member(&R_SLOT(tp_str)),
  null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&R_SLOT(tp_not)),
  member(&R_SLOT(tp_bool)),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo), // TODO: Dedicated compare function
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,
  member(&R_SLOT(tp_seq_size)),
  member(&R_SLOT(tp_seq_contains)),
  null,null,null,member(&DeeObject_Copy),
  member(&R_SLOT(tp_seq_iter_next))),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(R_SLOT(tp_members)),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&R_SLOT(tp_marshal))),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DEE_PP_CAT_3(DeeRangeUInt,N,Iterator_Type) = {
 DEE_TYPE_OBJECT_HEAD_v100(member(DEE_PP_CAT_2(INTEGER_NAME_UI,N) ".range"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
         DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_NO_ITERATE_EFFECT|
         DEE_TYPE_FLAG_FUNDAMENTAL),
  member(&DEE_PP_CAT_3(DeeRangeInt,N,Iterator_Type))),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeRangeUIntNIteratorObject),null,
  member(&R_SLOT(tp_ctor)),member(&R_SLOT(tp_copy_ctor)),null,
  member(&R_SLOT(tp_any_ctor))),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&R_SLOT(tp_copy_assign)),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&R_USLOT(tp_str)),
  member(&R_USLOT(tp_str)),
  null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&R_USLOT(tp_not)),member(&R_USLOT(tp_bool)),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo), // TODO: Dedicated compare function
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,
  member(&R_USLOT(tp_seq_size)),
  member(&R_USLOT(tp_seq_contains)),
  null,null,null,member(&DeeObject_Copy),
  member(&R_USLOT(tp_seq_iter_next))),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(R_USLOT(tp_members)),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&R_USLOT(tp_marshal))),
 DEE_TYPE_OBJECT_FOOTER_v101
};

#undef UFMT
#undef FMT
#undef R_USLOT
#undef R_SLOT
#undef DeeRangeUIntNIteratorObject
#undef DeeRangeIntNIteratorObject

#ifndef __INTELLISENSE__
#undef DeeIntMin_New
#undef DeeUIntMin_New
#undef dee_intmin_t
#undef dee_uintmin_t

#undef intn_t
#undef uintn_t
#undef DeeIntN_Check
#undef DeeIntN_CheckExact
#undef DeeUIntN_Check
#undef DeeUIntN_CheckExact
#undef DeeAtomicIntN_Check
#undef DeeAtomicIntN_CheckExact
#undef DeeAtomicUIntN_Check
#undef DeeAtomicUIntN_CheckExact
#undef DeeObject_IntN
#undef DeeObject_UIntN
#undef DeeIntNObject
#undef DeeIntN_New
#undef DeeUIntN_New
#undef DeeAtomicIntN_New
#undef DeeAtomicUIntN_New
#undef SLOT
#undef USLOT
#undef ASLOT
#undef AUSLOT
#undef POOL_NAME

#undef DeeAtomicIN_Store
#undef DeeAtomicIN_Load
#undef DeeAtomicIN_Exchange
#undef DeeAtomicIN_CompareExchangeWeak
#undef DeeAtomicIN_CompareExchangeStrong
#undef DeeAtomicIN_CompareExchangeWeakVal
#undef DeeAtomicIN_CompareExchangeStrongVal
#undef DeeAtomicIN_FetchInc
#undef DeeAtomicIN_IncFetch
#undef DeeAtomicIN_FetchDec
#undef DeeAtomicIN_DecFetch
#undef DeeAtomicIN_FetchAdd
#undef DeeAtomicIN_AddFetch
#undef DeeAtomicIN_FetchSub
#undef DeeAtomicIN_SubFetch
#undef DeeAtomicIN_FetchAnd
#undef DeeAtomicIN_AndFetch
#undef DeeAtomicIN_FetchOr
#undef DeeAtomicIN_OrFetch
#undef DeeAtomicIN_FetchXor
#undef DeeAtomicIN_XorFetch
#undef SIZEOF_INTN_T
#undef N
#endif

DEE_DECL_END

