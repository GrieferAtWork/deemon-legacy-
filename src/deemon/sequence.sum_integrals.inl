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
//#ifdef __INTELLISENSE__
//#include "sequence.c"
//DEE_DECL_BEGIN
//#define UNSIGNED
//#define SIZEOF   4
//#endif

#if !defined(SIZEOF) && !defined(FLOATID)
#error "Must '#define SIZEOF' before #including this file"
#endif

#ifdef FLOATID
#if FLOATID == DEE_TYPES_FLOATID_FLOAT
#define T      float
#define T_cast float
#elif FLOATID == DEE_TYPES_FLOATID_DOUBLE
#define T      double
#define T_cast double
#elif FLOATID == DEE_TYPES_FLOATID_LDOUBLE
#define T      long double
#define T_cast ldouble
#else
#error "Invalid float id"
#endif
#else
#ifdef UNSIGNED
#define T      DEE_TYPES_UINT(SIZEOF)
#define T_cast DEE_PP_CAT_2(uint,DEE_PP_MUL8(SIZEOF))
#else
#define T      DEE_TYPES_INT(SIZEOF)
#define T_cast DEE_PP_CAT_2(int,DEE_PP_MUL8(SIZEOF))
#endif
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *)
#ifdef FLOATID
DEE_PP_CAT_2(_DeeIteratorSum_,T_cast)
#elif defined(UNSIGNED)
DEE_PP_CAT_2(_DeeIteratorSum_UInt,DEE_PP_MUL8(SIZEOF))
#else
DEE_PP_CAT_2(_DeeIteratorSum_Int,DEE_PP_MUL8(SIZEOF))
#endif
(DEE_A_IN T start, DEE_A_IN DeeObject *iterator) {
 DeeObject *elem; int error; T temp;
 while ((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  error = DeeObject_Cast(T_cast,elem,&temp);
  Dee_DECREF(elem);
  if (error != 0) return NULL;
  start += temp;
 }
 if (error < 0) return NULL;
 return DeeObject_New(T_cast,start);
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *)
#ifdef FLOATID
DEE_PP_CAT_2(_DeeVectorSum_,T_cast)
#elif defined(UNSIGNED)
DEE_PP_CAT_2(_DeeVectorSum_UInt,DEE_PP_MUL8(SIZEOF))
#else
DEE_PP_CAT_2(_DeeVectorSum_Int,DEE_PP_MUL8(SIZEOF))
#endif
(DEE_A_IN T start, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 T temp; DeeObject **end;
#ifndef FLOATID
 DEE_LVERBOSE2("Summing integrals: %Iu (start: %I" DEE_PP_STR(DEE_PP_MUL8(SIZEOF)) "u)\n",elemc,start);
#endif
 end = elemv+elemc;
 while (elemv != end) {
  if (DeeObject_Cast(T_cast,*elemv,&temp) != 0) return NULL;
  start += temp;
  ++elemv;
 }
 return DeeObject_New(T_cast,start);
}


// In:  'list' is acquired
// Out: 'list' is released
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *)
#ifdef FLOATID
DEE_PP_CAT_2(_DeeListSum_,T_cast)
#elif defined(UNSIGNED)
DEE_PP_CAT_2(_DeeListSum_UInt,DEE_PP_MUL8(SIZEOF))
#else
DEE_PP_CAT_2(_DeeListSum_Int,DEE_PP_MUL8(SIZEOF))
#endif
(DEE_A_IN T start, DEE_A_IN DeeListObject *list) {
 Dee_size_t i; int error; T temp; DeeObject *elem;
 // NOTE: The list was already acquired
 for (i = 1; i < DeeList_SIZE(list); ++i) {
  Dee_INCREF(elem = DeeList_GET(list,i));
  DeeList_RELEASE(list);
  error = DeeObject_Cast(T_cast,elem,&temp);
  Dee_DECREF(elem);
  if (error != 0) return NULL;
  start += temp;
  DeeList_ACQUIRE(list);
 }
 DeeList_RELEASE(list);
 return DeeObject_New(T_cast,start);
}


#undef T
#undef T_cast
#ifdef FLOATID
#undef FLOATID
#else
#undef SIZEOF
#endif

//#ifdef __INTELLISENSE__
//DEE_DECL_END
//#endif

