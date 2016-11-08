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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_CAST_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_CAST_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/std/stdarg.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// Generic integral casting interface
#define DEE_PRIVATE_OBJECT_GET_INT_F_2(x)  DeeObject_TGetInt##x
#define DEE_PRIVATE_OBJECT_GET_INT_F(x)    DEE_PRIVATE_OBJECT_GET_INT_F_2(x)
#define DEE_PRIVATE_OBJECT_GET_UINT_F_2(x) DeeObject_TGetUInt##x
#define DEE_PRIVATE_OBJECT_GET_UINT_F(x)   DEE_PRIVATE_OBJECT_GET_UINT_F_2(x)
#define DEE_PRIVATE_OBJECT_GET_XINT_F_0    DEE_PRIVATE_OBJECT_GET_UINT_F
#define DEE_PRIVATE_OBJECT_GET_XINT_F_1    DEE_PRIVATE_OBJECT_GET_INT_F
#define DEE_PRIVATE_OBJECT_GET_XINT_F(x)   DEE_PRIVATE_OBJECT_GET_XINT_F_##x
#define DeeObject_CAST_INT_F(sizeof)                 DEE_PRIVATE_OBJECT_GET_INT_F(DEE_PP_MUL8(sizeof))
#define DeeObject_CAST_XINT_F(signed,sizeof)         DEE_PRIVATE_OBJECT_GET_XINT_F(signed)(DEE_PP_MUL8(sizeof))
#define DeeObject_CAST_UINT_F(sizeof)                DEE_PRIVATE_OBJECT_GET_UINT_F(DEE_PP_MUL8(sizeof))
#define /*DEE_A_EXEC*/ DeeObject_CAST_INT(sizeof,tp_ob,ob,result)         DeeObject_CAST_INT_F(sizeof)(tp_ob,ob,result)
#define /*DEE_A_EXEC*/ DeeObject_CAST_UINT(sizeof,tp_ob,ob,result)        DeeObject_CAST_UINT_F(sizeof)(tp_ob,ob,result)
#define /*DEE_A_EXEC*/ DeeObject_CAST_XINT(signed,sizeof,tp_ob,ob,result) DeeObject_CAST_XINT_F(signed,sizeof)(tp_ob,ob,result)

#define DEE_PRIVATE_OBJECT_CAST_F(T)  DEE_PRIVATE_OBJECT_CAST_##T

//////////////////////////////////////////////////////////////////////////
// DEE_A_EXEC int (*DeeObject_Cast_F(typename T))(DEE_A_INOUT DeeObject *ob, DEE_A_OUT T *result);
// - Causes a compiler error if 'T' is not supported
// - Returns a macro that can be called to converted a DeeObject into type 'T'
#define DeeObject_Cast_F(T)  DEE_PRIVATE_OBJECT_CAST_F(T)

//////////////////////////////////////////////////////////////////////////
// DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_Cast(typename T, DEE_A_INOUT DeeObject *ob, DEE_A_OUT T *result);
// DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TCast(typename T, DEE_A_IN DeeTypeObject const *ob, DEE_A_INOUT DeeObject *ob, DEE_A_OUT T *result);
// - Causes a compiler error if 'T' is not supported
// - Casts 'T' to a builtin integral type and returns the new object
// - Returns (T)-1 and sets an error if 'ob' could not be converted
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int */DeeObject_Cast(T,ob,result) DeeObject_TCast(T,Dee_TYPE(ob),ob,result)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int */DeeObject_TCast(T,tp_ob,ob,result) DEE_PRIVATE_OBJECT_CAST_F(T)(tp_ob,ob,result)


//////////////////////////////////////////////////////////////////////////
// Cast a given object to some type using a format
// string equivalent to that used by DeeType_Structf.
// e.g.:
// >> int result;
// >> if (DeeObject_Castf(ob,(void *)&result,"d") != 0) { handle_error(); }
// NOTES:
//   - Whenever possible, 'DeeObject_Cast' should be used instead of this!
//   - The caller is responsible for ensuring that enough memory is
//     available in '*result' to encompass the c-version of any type
//     that may be specified as an argument to this function.
#define DeeObject_Castf(ob,result,...) \
 DeeObject_TCastf(Dee_TYPE(ob),ob,result,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_TCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *fmt, ...)
 DEE_ATTRIBUTE_NONNULL((1,2,3,4));

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define DeeObject_VCastf(ob,result,fmt,args) \
 DeeObject_TCastf(Dee_TYPE(ob),ob,result,fmt,args)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_VTCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *fmt, DEE_A_IN va_list args)
 DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


#define DEE_PRIVATE_OBJECT_CAST_object    /* nothing */
#define DEE_PRIVATE_OBJECT_CAST_DeeObject /* nothing */

#ifdef DEE_TYPES_INT8_T
#define DEE_PRIVATE_OBJECT_CAST_int8_t    DeeObject_TGetInt8
#define DEE_PRIVATE_OBJECT_CAST_uint8_t   DeeObject_TGetUInt8
#endif
#ifdef DEE_TYPES_INT16_T
#define DEE_PRIVATE_OBJECT_CAST_int16_t   DeeObject_TGetInt16
#define DEE_PRIVATE_OBJECT_CAST_uint16_t  DeeObject_TGetUInt16
#endif
#ifdef DEE_TYPES_INT32_T
#define DEE_PRIVATE_OBJECT_CAST_int32_t   DeeObject_TGetInt32
#define DEE_PRIVATE_OBJECT_CAST_uint32_t  DeeObject_TGetUInt32
#endif
#ifdef DEE_TYPES_INT64_T
#define DEE_PRIVATE_OBJECT_CAST_int64_t   DeeObject_TGetInt64
#define DEE_PRIVATE_OBJECT_CAST_uint64_t  DeeObject_TGetUInt64
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
#define DEE_PRIVATE_OBJECT_CAST_double    DeeObject_TGetDouble
#ifdef DEE_TYPES_SIZEOF_FLOAT
#define DEE_PRIVATE_OBJECT_CAST_float     DeeObject_TGetFloat
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
#define DEE_PRIVATE_OBJECT_CAST_ldouble   DeeObject_TGetLDouble
#endif
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_float      DEE_PRIVATE_OBJECT_CAST_float
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_float      DEE_PRIVATE_OBJECT_CAST_double
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_float      DEE_PRIVATE_OBJECT_CAST_ldouble
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_double     DEE_PRIVATE_OBJECT_CAST_float
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_double     DEE_PRIVATE_OBJECT_CAST_double
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_double     DEE_PRIVATE_OBJECT_CAST_ldouble
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_ldouble    DEE_PRIVATE_OBJECT_CAST_float
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_ldouble    DEE_PRIVATE_OBJECT_CAST_double
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
# define DEE_PRIVATE_OBJECT_CAST_Dee_rt_ldouble    DEE_PRIVATE_OBJECT_CAST_ldouble
#endif
#include <deemon/__tg_cast.inl>

#ifdef DEE_TYPES_INT8_T
#define /* DEE_A_EXEC int */DeeObject_GetInt8(ob,result)    DeeObject_TGetInt8(Dee_TYPE(ob),ob,result)
#endif
#ifdef DEE_TYPES_INT16_T
#define /* DEE_A_EXEC int */DeeObject_GetInt16(ob,result)   DeeObject_TGetInt16(Dee_TYPE(ob),ob,result)
#endif

#ifdef DEE_TYPES_INT32_T
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetInt32(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int32_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetInt32(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int32_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* DEE_A_EXEC int */DeeObject_GetInt32(ob,result)   DeeObject_TGetInt32(Dee_TYPE(ob),ob,result)
#endif

#ifdef DEE_TYPES_INT64_T
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetInt64(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int64_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetInt64(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int64_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* DEE_A_EXEC int */DeeObject_GetInt64(ob,result)   DeeObject_TGetInt64(Dee_TYPE(ob),ob,result)
#endif

#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetDouble(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT double *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetDouble(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT double *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* DEE_A_EXEC int */DeeObject_GetDouble(ob,result)  DeeObject_TGetDouble(Dee_TYPE(ob),ob,result)
#ifdef DEE_TYPES_SIZEOF_FLOAT
#define /* DEE_A_EXEC int */DeeObject_GetFloat(ob,result)   DeeObject_TGetFloat(Dee_TYPE(ob),ob,result)
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
#define /* DEE_A_EXEC int */DeeObject_GetLDouble(ob,result) DeeObject_TGetLDouble(Dee_TYPE(ob),ob,result)
#endif
#endif

#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#ifdef DEE_TYPES_INT8_T
#define /* DEE_A_EXEC int */DeeObject_TGetInt8(tp_ob,ob,result)             DEE_GCC_EXTENSION({Dee_int32_t _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_TGetInt32(tp_ob,ob,&_get_temp) == 0) ? (*(result) = (Dee_int8_t)_get_temp,0) : -1;})
#define /* DEE_A_EXEC int */DeeObject_PGetInt8(tp_self,base_addr,result)    DEE_GCC_EXTENSION({Dee_int32_t _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_PGetInt32(tp_self,base_addr,&_get_temp) == 0) ? (*(result) = (Dee_int8_t)_get_temp,0) : -1;})
#endif
#ifdef DEE_TYPES_INT16_T
#define /* DEE_A_EXEC int */DeeObject_TGetInt16(tp_ob,ob,result)            DEE_GCC_EXTENSION({Dee_int32_t _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_TGetInt32(tp_ob,ob,&_get_temp) == 0) ? (*(result) = (Dee_int16_t)_get_temp,0) : -1;})
#define /* DEE_A_EXEC int */DeeObject_PGetInt16(tp_self,base_addr,result)   DEE_GCC_EXTENSION({Dee_int32_t _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_PGetInt32(tp_self,base_addr,&_get_temp) == 0) ? (*(result) = (Dee_int16_t)_get_temp,0) : -1;})
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
#ifdef DEE_TYPES_SIZEOF_FLOAT
#define /* DEE_A_EXEC int */DeeObject_TGetFloat(tp_ob,ob,result)            DEE_GCC_EXTENSION({double _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_TGetDouble(tp_ob,ob,&_get_temp) == 0) ? (*(result) = (float)_get_temp,0) : -1;})
#define /* DEE_A_EXEC int */DeeObject_PGetFloat(tp_self,base_addr,result)   DEE_GCC_EXTENSION({double _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_PGetDouble(tp_self,base_addr,&_get_temp) == 0) ? (*(result) = (float)_get_temp,0) : -1;})
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
#define /* DEE_A_EXEC int */DeeObject_TGetLDouble(tp_ob,ob,result)          DEE_GCC_EXTENSION({double _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_TGetDouble(tp_ob,ob,&_get_temp) == 0) ? (*(result) = (long double)_get_temp,0) : -1;})
#define /* DEE_A_EXEC int */DeeObject_PGetLDouble(tp_self,base_addr,result) DEE_GCC_EXTENSION({double _get_temp; DEE_ASSERT(result); DEE_LIKELY(DeeObject_PGetDouble(tp_self,base_addr,&_get_temp) == 0) ? (*(result) = (long double)_get_temp,0) : -1;})
#endif
#endif /* DEE_TYPES_SIZEOF_DOUBLE */
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#ifdef DEE_TYPES_INT8_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetInt8(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int8_t *result) { Dee_int32_t result2; DEE_ASSERT(result); if (DeeObject_TGetInt32(tp_self,self,&result2) != 0) return -1; *result = (Dee_int8_t)result2; return 0; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetInt8(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int8_t *result) { Dee_int32_t result2; DEE_ASSERT(result); if (DeeObject_PGetInt32(tp_self,base_addr,&result2) != 0) return -1; *result = (Dee_int8_t)result2; return 0; }
#endif
#ifdef DEE_TYPES_INT16_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetInt16(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int16_t *result) { Dee_int32_t result2; DEE_ASSERT(result); if (DeeObject_TGetInt32(tp_self,self,&result2) != 0) return -1; *result = (Dee_int16_t)result2; return 0; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetInt16(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int16_t *result) { Dee_int32_t result2; DEE_ASSERT(result); if (DeeObject_PGetInt32(tp_self,base_addr,&result2) != 0) return -1; *result = (Dee_int16_t)result2; return 0; }
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetFloat(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT float *result) { double result2; DEE_ASSERT(result); if (DeeObject_TGetDouble(tp_self,self,&result2) != 0) return -1; *result = (float)result2; return 0; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetFloat(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT float *result) { double result2; DEE_ASSERT(result); if (DeeObject_PGetDouble(tp_self,base_addr,&result2) != 0) return -1; *result = (float)result2; return 0; }
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetLDouble(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT long double *result) { double result2; DEE_ASSERT(result); if (DeeObject_TGetDouble(tp_self,self,&result2) != 0) return -1; *result = (long double)result2; return 0; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetLDouble(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT long double *result) { double result2; DEE_ASSERT(result); if (DeeObject_PGetDouble(tp_self,base_addr,&result2) != 0) return -1; *result = (long double)result2; return 0; }
#endif
#endif /* DEE_TYPES_SIZEOF_DOUBLE */
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */

#if defined(__INTELLISENSE__) || defined(_PREFAST_)
#ifdef DEE_TYPES_UINT8_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_GetUInt8(DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint8_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetUInt8(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint8_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetUInt8(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_uint8_t *result);
#endif
#ifdef DEE_TYPES_UINT16_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_GetUInt16(DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint16_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetUInt16(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint16_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetUInt16(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_uint16_t *result);
#endif
#ifdef DEE_TYPES_UINT32_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_GetUInt32(DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint32_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetUInt32(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint32_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetUInt32(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_uint32_t *result);
#endif
#ifdef DEE_TYPES_UINT64_T
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_GetUInt64(DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint64_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetUInt64(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *ob, DEE_A_OUT Dee_uint64_t *result);
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PGetUInt64(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_uint64_t *result);
#endif
#else
#ifdef DEE_TYPES_UINT8_T
#define /*DEE_A_EXEC*/ DeeObject_GetUInt8(ob,result)                  DeeObject_GetInt8(ob,(Dee_int8_t *)(result)) 
#define /*DEE_A_EXEC*/ DeeObject_TGetUInt8(tp_self,ob,result)         DeeObject_TGetInt8(tp_self,ob,(Dee_int8_t *)(result)) 
#define /*DEE_A_EXEC*/ DeeObject_PGetUInt8(tp_self,base_addr,result)  DeeObject_PGetInt8(tp_self,base_addr,(Dee_int8_t *)(result)) 
#endif
#ifdef DEE_TYPES_UINT16_T
#define /*DEE_A_EXEC*/ DeeObject_GetUInt16(ob,result)                 DeeObject_GetInt16(ob,(Dee_int16_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_TGetUInt16(tp_self,ob,result)        DeeObject_TGetInt16(tp_self,ob,(Dee_int16_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_PGetUInt16(tp_self,base_addr,result) DeeObject_PGetInt16(tp_self,base_addr,(Dee_int16_t *)(result))
#endif
#ifdef DEE_TYPES_UINT32_T
#define /*DEE_A_EXEC*/ DeeObject_GetUInt32(ob,result)                 DeeObject_GetInt32(ob,(Dee_int32_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_TGetUInt32(tp_self,ob,result)        DeeObject_TGetInt32(tp_self,ob,(Dee_int32_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_PGetUInt32(tp_self,base_addr,result) DeeObject_PGetInt32(tp_self,base_addr,(Dee_int32_t *)(result))
#endif
#ifdef DEE_TYPES_UINT64_T
#define /*DEE_A_EXEC*/ DeeObject_GetUInt64(ob,result)                 DeeObject_GetInt64(ob,(Dee_int64_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_TGetUInt64(tp_self,ob,result)        DeeObject_TGetInt64(tp_self,ob,(Dee_int64_t *)(result))
#define /*DEE_A_EXEC*/ DeeObject_PGetUInt64(tp_self,base_addr,result) DeeObject_PGetInt64(tp_self,base_addr,(Dee_int64_t *)(result))
#endif
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_CAST_H */
