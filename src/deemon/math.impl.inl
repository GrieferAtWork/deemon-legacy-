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

#include <deemon/__conf.inl>
#include <deemon/math.h>

DEE_DECL_BEGIN
#define T float
#endif

#ifndef DEE_MATH_IMPL_UTILS_DEFINED
#define DEE_MATH_IMPL_UTILS_DEFINED 1
#define DEE_PRIVATE_NAME_float(x) x##f
#define DEE_PRIVATE_NAME_double(x) x
#define DEE_PRIVATE_NAME_ldouble(x) x##l
#define DEE_PRIVATE_NNAME_float DEE_NATIVE_MATH_F
#define DEE_PRIVATE_NNAME_double DEE_NATIVE_MATH_D
#define DEE_PRIVATE_NNAME_ldouble DEE_NATIVE_MATH_L
#define DEE_PRIVATE_TYPE_float Dee_float
#define DEE_PRIVATE_TYPE_double Dee_double
#define DEE_PRIVATE_TYPE_ldouble Dee_ldouble
#define DEE_PRIVATE_CONST_float DEE_FLOAT_C
#define DEE_PRIVATE_CONST_double DEE_DOUBLE_C
#define DEE_PRIVATE_CONST_ldouble DEE_LDOUBLE_C
#define M_NAME(_T)  DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_NAME_,_T)
#define M_NNAME(_T) DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_NNAME_,_T)
#define M_TYPE(_T)  DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_TYPE_,_T)
#define M_CONST(_T) DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_CONST_,_T)
#endif


#ifndef T
#error "Must #define 'T' before including this file"
#endif

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define float_t M_TYPE(T)
#define N       M_NAME(T)
#define NN      M_NNAME(T)
#define C       M_CONST(T)


DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_sin)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_SIN
 return N(Dee_m_sin)(x);
#else
 float_t temp[2];
 N(Dee_m_sincos)(x,temp);
 return temp[0];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_cos)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_COS
 return N(Dee_m_cos)(x);
#else
 float_t temp[2];
 N(Dee_m_sincos)(x,temp);
 return temp[1];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_tan)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_COS
 return N(Dee_m_tan)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST void N(_Dee_m_sincos)(DEE_A_IN float_t x, DEE_A_OUT_W(2) float_t *sc) {
#if DEE_ENVIRONMENT_HAVE_MATH_SIN&&\
    DEE_ENVIRONMENT_HAVE_MATH_COS
 sc[0] = N(Dee_m_sin)(x);
 sc[1] = N(Dee_m_cos)(x);
#else
#define calculate_sincos_cache(x) (((x)<C(0.0))\
 ? ((C(1.27323954)*(x))+(C(0.405284735)*sqr(x)))\
 : ((C(1.27323954)*(x))-(C(0.405284735)*sqr(x))))
#define calculate_sincos_value(cache) (((cache)<C(0.0))\
 ? ((C(0.225)*((cache)*-(cache)-(cache)))+(cache))\
 : ((C(0.225)*((cache)* (cache)-(cache)))+(cache)))
#define calculate_sincos_advance(x) (\
 (((x) += N(DEE_M_PI_2)) > N(DEE_M_PI))?((x) -= N(DEE_M_PI_X2)):(x))

 while (x < -N(DEE_M_PI)) x += N(DEE_M_PI_X2);
 while (x > N(DEE_M_PI)) x -= N(DEE_M_PI_X2);
 float_t cache = calculate_sincos_cache(x);
 sc[0] = calculate_sincos_value(cache);
 calculate_sincos_advance(x);
 x = calculate_sincos_cache(x);
 sc[1] = calculate_sincos_value(x);

#undef calculate_sincos_cache
#undef calculate_sincos_value
#undef calculate_sincos_advance
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_asin)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ASIN
 return N(Dee_m_asin)(x);
#else
 float_t temp[2];
 N(Dee_m_asincos)(x,temp);
 return temp[0];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_acos)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ACOS
 return N(Dee_m_acos)(x);
#else
 float_t temp[2];
 N(Dee_m_asincos)(x,temp);
 return temp[1];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_atan)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ATAN
 return N(Dee_m_atan)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_atan2)(DEE_A_IN float_t y, DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ATAN2
 return N(Dee_m_atan2)(y,x);
#else
 return x*y; // TODO
#endif
}
DEE_ATTRIBUTE_CONST void N(_Dee_m_asincos)(DEE_A_IN float_t x, DEE_A_OUT_W(2) float_t *sc) {
#if DEE_ENVIRONMENT_HAVE_MATH_ASIN&&\
    DEE_ENVIRONMENT_HAVE_MATH_ACOS
 sc[0] = N(Dee_m_asin)(x);
 sc[1] = N(Dee_m_acos)(x);
#else
 sc[0] = sc[1] = x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_sinh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_SINH
 return N(Dee_m_sinh)(x);
#else
 float_t temp[2];
 N(Dee_m_sincosh)(x,temp);
 return temp[0];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_cosh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_COSH
 return N(Dee_m_cosh)(x);
#else
 float_t temp[2];
 N(Dee_m_sincosh)(x,temp);
 return temp[1];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_tanh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_TANH
 return N(Dee_m_tanh)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST void N(_Dee_m_sincosh)(DEE_A_IN float_t x, DEE_A_OUT_W(2) float_t *sc) {
#if DEE_ENVIRONMENT_HAVE_MATH_SINH&&\
    DEE_ENVIRONMENT_HAVE_MATH_COSH
 sc[0] = N(Dee_m_sinh)(x);
 sc[1] = N(Dee_m_cosh)(x);
#else
 sc[0] = sc[1] = x; // TODO
#endif
}


DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_asinh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ASINH
 return N(Dee_m_asinh)(x);
#else
 float_t temp[2];
 N(Dee_m_asincosh)(x,temp);
 return temp[0];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_acosh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ACOSH
 return N(Dee_m_acosh)(x);
#else
 float_t temp[2];
 N(Dee_m_asincosh)(x,temp);
 return temp[1];
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_atanh)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ATANH
 return N(Dee_m_atanh)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST void N(_Dee_m_asincosh)(DEE_A_IN float_t x, DEE_A_OUT_W(2) float_t *sc) {
#if DEE_ENVIRONMENT_HAVE_MATH_ASINH&&\
    DEE_ENVIRONMENT_HAVE_MATH_ACOSH
 sc[0] = N(Dee_m_asinh)(x);
 sc[1] = N(Dee_m_acosh)(x);
#else
 sc[0] = sc[1] = x; // TODO
#endif
}

DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_exp)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_EXP
 return N(Dee_m_exp)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_frexp)(DEE_A_IN float_t x, DEE_A_OUT int *exp) {
#if DEE_ENVIRONMENT_HAVE_MATH_FREXP
 return N(Dee_m_frexp)(x,exp);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_log)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LOG
 return N(Dee_m_log)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_log10)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LOG10
 return N(Dee_m_log10)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_modf)(DEE_A_IN float_t x, DEE_A_OUT float_t *int_part) {
#if DEE_ENVIRONMENT_HAVE_MATH_MODF
 return N(Dee_m_modf)(x,int_part);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_exp2)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_EXP2
 return N(Dee_m_exp2)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_expm1)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_EXPM1
 return N(Dee_m_expm1)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_ilogb)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ILOGB
 return N(Dee_m_ilogb)(x);
#else
 return (int)x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_log1p)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LOG1P
 return N(Dee_m_log1p)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_log2)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LOG2
 return N(Dee_m_log2)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_logb)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LOGB
 return N(Dee_m_logb)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_scalbn)(DEE_A_IN float_t x, DEE_A_IN int n) {
#if DEE_ENVIRONMENT_HAVE_MATH_SCALBN
 return N(Dee_m_scalbn)(x,n);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_scalbi32n)(DEE_A_IN float_t x, DEE_A_IN Dee_int32_t n) {
#if (DEE_ENVIRONMENT_HAVE_MATH_SCALBN && (DEE_TYPES_SIZEOF_INT == 4)) || \
    (DEE_ENVIRONMENT_HAVE_MATH_SCALBLN && (DEE_TYPES_SIZEOF_LONG == 4))
 return N(Dee_m_scalbi32n)(x,n);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_pow)(DEE_A_IN float_t x, DEE_A_IN float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_POW
 return N(Dee_m_pow)(x,y);
#else
 return x*y; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_sqrt)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_SQRT
 return N(Dee_m_sqrt)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_cbrt)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_CBRT
 return N(Dee_m_cbrt)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_hypot)(DEE_A_IN float_t x, DEE_A_IN float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_HYPOT
 return N(Dee_m_hypot)(x,y);
#else
 return x*y; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_erf)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ERF
 return N(Dee_m_erf)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_tgamma)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_TGAMMA
 return N(Dee_m_tgamma)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_lgamma)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_LGAMMA
 return N(Dee_m_lgamma)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_ceil)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_CEIL
 return N(Dee_m_ceil)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_floor)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_FLOOR
 return N(Dee_m_floor)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_fmod)(DEE_A_IN float_t number, DEE_A_IN float_t denom) {
#if DEE_ENVIRONMENT_HAVE_MATH_FMOD
 return N(Dee_m_fmod)(number,denom);
#else
 return number*denom; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_trunc)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_TRUNC
 return N(Dee_m_trunc)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_round)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ROUND
 return N(Dee_m_round)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t N(_Dee_m_i32round)(DEE_A_IN float_t x) {
#if (DEE_ENVIRONMENT_HAVE_MATH_LROUND && DEE_TYPES_SIZEOF_LONG == 4) || \
    (DEE_ENVIRONMENT_HAVE_MATH_LLROUND && DEE_TYPES_SIZEOF_LLONG == 4)
 return N(Dee_m_i32round)(x);
#else
 return (Dee_int32_t)x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t N(_Dee_m_i64round)(DEE_A_IN float_t x) {
#if (DEE_ENVIRONMENT_HAVE_MATH_LROUND && DEE_TYPES_SIZEOF_LONG == 8) || \
    (DEE_ENVIRONMENT_HAVE_MATH_LLROUND && DEE_TYPES_SIZEOF_LLONG == 8)
 return N(Dee_m_i64round)(x);
#else
 return (Dee_int64_t)x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_rint)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_RINT
 return N(Dee_m_rint)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t N(_Dee_m_i32rint)(DEE_A_IN float_t x) {
#if (DEE_ENVIRONMENT_HAVE_MATH_LRINT && DEE_TYPES_SIZEOF_LONG == 4) || \
    (DEE_ENVIRONMENT_HAVE_MATH_LLRINT && DEE_TYPES_SIZEOF_LLONG == 4)
 return N(Dee_m_i32rint)(x);
#else
 return (Dee_int32_t)x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t N(_Dee_m_i64rint)(DEE_A_IN float_t x) {
#if (DEE_ENVIRONMENT_HAVE_MATH_LRINT && DEE_TYPES_SIZEOF_LONG == 8) || \
    (DEE_ENVIRONMENT_HAVE_MATH_LLRINT && DEE_TYPES_SIZEOF_LLONG == 8)
 return N(Dee_m_i64rint)(x);
#else
 return (Dee_int64_t)x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_nearbyint)(DEE_A_IN float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT
 return N(Dee_m_nearbyint)(x);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_remainder)(DEE_A_IN float_t number, DEE_A_IN float_t denom) {
#if DEE_ENVIRONMENT_HAVE_MATH_REMAINDER
 return N(Dee_m_remainder)(number,denom);
#else
 return number*denom; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_remquo)(DEE_A_IN float_t number, DEE_A_IN float_t denom, DEE_A_OUT int *quot) {
#if DEE_ENVIRONMENT_HAVE_MATH_REMQUO
 return N(Dee_m_remquo)(number,denom,quot);
#else
 return number*denom; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_copysign)(DEE_A_IN float_t x, DEE_A_IN float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN
 return N(Dee_m_copysign)(x,y);
#else
 return ((y < C(0.0)) != (x < C(0.0))) ? -x : x;
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_nextafter)(DEE_A_IN float_t x, DEE_A_IN float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER
 return N(Dee_m_nextafter)(x,y);
#else
 return x; // TODO
#endif
}
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_nexttoward)(DEE_A_IN float_t x, DEE_A_IN Dee_ldouble y)
#elif defined(DEE_TYPES_SIZEOF_DOUBLE)
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_nexttoward)(DEE_A_IN float_t x, DEE_A_IN Dee_double y)
#elif defined(DEE_TYPES_SIZEOF_FLOAT)
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_nexttoward)(DEE_A_IN float_t x, DEE_A_IN Dee_float y)
#else
#error "How did you get here?"
#endif
{
#if DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD
 return N(Dee_m_nexttoward)(x,y);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED float_t N(_Dee_m_fdim)(DEE_A_IN float_t x, DEE_A_IN float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_FDIM
 return N(Dee_m_fdim)(x,y);
#else
 return x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isfinite)(float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISFINITE
 return N(Dee_m_isfinite)(x);
#else
 return !N(Dee_m_isinf)(x) && !N(Dee_m_isnan)(x);
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isinf)(float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISINF
 return N(Dee_m_isinf)(x);
#else
 return x*0.5 == x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isnan)(float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISNAN
 return N(Dee_m_isnan)(x);
#else
 return x != x; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isnormal)(float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL
 return N(Dee_m_isnormal)(x);
#else
 return 1; // TODO
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_signbit)(float_t x) {
#if DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT
 return N(Dee_m_signbit)(x);
#else
 return (x) < C(0.0) ? 1 : 0; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isgreater)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISGREATER
 return N(Dee_m_isgreater)(x,y);
#else
 return x > y; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isgreaterequal)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL
 return N(Dee_m_isgreaterequal)(x,y);
#else
 return x >= y; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isless)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISLESS
 return N(Dee_m_isless)(x,y);
#else
 return x < y; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_islessequal)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL
 return N(Dee_m_islessequal)(x,y);
#else
 return x <= y; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_islessgreater)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER
 return N(Dee_m_islessgreater)(x,y);
#else
 return x < y || x > y; // todo?
#endif
}
DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int N(_Dee_m_isunordered)(float_t x, float_t y) {
#if DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED
 return N(Dee_m_isunordered)(x,y);
#else
 return 0; // TODO
#endif
}

#undef float_t
#undef N
#undef NN
#undef C
#undef T
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

