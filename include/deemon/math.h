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
#ifndef GUARD_DEEMON_MATH_H
#define GUARD_DEEMON_MATH_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_API_HAVE_MATH_EXPORTS || defined(DEE_LIMITED_API)

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_MATH_H
DEE_COMPILER_MSVC_WARNING_PUSH(4820)
#include <math.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif
#if !defined(NAN) && DEE_ENVIRONMENT_HAVE_INCLUDE_FLOAT_H
#include <float.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#if DEE_CONFIG_API_HAVE_MATH_EXPORTS
#define DEE_MATH_FUNC_DECL    DEE_FUNC_DECL
#else
#define DEE_MATH_FUNC_DECL(T) extern T
#endif

#define DEE_ENVIRONMENT_HAVE_MATH_C99_FLOAT_LDOUBLE 1
#define DEE_ENVIRONMENT_HAVE_MATH_SIN               1
#define DEE_ENVIRONMENT_HAVE_MATH_COS               1
#define DEE_ENVIRONMENT_HAVE_MATH_TAN               1
#define DEE_ENVIRONMENT_HAVE_MATH_ASIN              1
#define DEE_ENVIRONMENT_HAVE_MATH_ACOS              1
#define DEE_ENVIRONMENT_HAVE_MATH_ATAN              1
#define DEE_ENVIRONMENT_HAVE_MATH_ATAN2             1
#define DEE_ENVIRONMENT_HAVE_MATH_SINH              1
#define DEE_ENVIRONMENT_HAVE_MATH_COSH              1
#define DEE_ENVIRONMENT_HAVE_MATH_TANH              1
#define DEE_ENVIRONMENT_HAVE_MATH_ASINH             1
#define DEE_ENVIRONMENT_HAVE_MATH_ACOSH             1
#define DEE_ENVIRONMENT_HAVE_MATH_ATANH             1
#define DEE_ENVIRONMENT_HAVE_MATH_EXP               1
#define DEE_ENVIRONMENT_HAVE_MATH_FREXP             1
#define DEE_ENVIRONMENT_HAVE_MATH_LOG               1
#define DEE_ENVIRONMENT_HAVE_MATH_LOG10             1
#define DEE_ENVIRONMENT_HAVE_MATH_MODF              1
#define DEE_ENVIRONMENT_HAVE_MATH_EXP2              1
#define DEE_ENVIRONMENT_HAVE_MATH_EXPM1             1
#define DEE_ENVIRONMENT_HAVE_MATH_ILOGB             1
#define DEE_ENVIRONMENT_HAVE_MATH_LOG1P             1
#define DEE_ENVIRONMENT_HAVE_MATH_LOG2              1
#define DEE_ENVIRONMENT_HAVE_MATH_LOGB              1
#define DEE_ENVIRONMENT_HAVE_MATH_SCALBN            1
#define DEE_ENVIRONMENT_HAVE_MATH_SCALBLN           1
#define DEE_ENVIRONMENT_HAVE_MATH_POW               1
#define DEE_ENVIRONMENT_HAVE_MATH_SQRT              1
#define DEE_ENVIRONMENT_HAVE_MATH_CBRT              1
#define DEE_ENVIRONMENT_HAVE_MATH_HYPOT             1
#define DEE_ENVIRONMENT_HAVE_MATH_ERF               1
#define DEE_ENVIRONMENT_HAVE_MATH_TGAMMA            1
#define DEE_ENVIRONMENT_HAVE_MATH_LGAMMA            1
#define DEE_ENVIRONMENT_HAVE_MATH_CEIL              1
#define DEE_ENVIRONMENT_HAVE_MATH_FLOOR             1
#define DEE_ENVIRONMENT_HAVE_MATH_FMOD              1
#define DEE_ENVIRONMENT_HAVE_MATH_TRUNC             1
#define DEE_ENVIRONMENT_HAVE_MATH_ROUND             1
#define DEE_ENVIRONMENT_HAVE_MATH_LROUND            1
#define DEE_ENVIRONMENT_HAVE_MATH_LLROUND           1
#define DEE_ENVIRONMENT_HAVE_MATH_RINT              1
#define DEE_ENVIRONMENT_HAVE_MATH_LRINT             1
#define DEE_ENVIRONMENT_HAVE_MATH_LLRINT            1
#define DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT         1
#define DEE_ENVIRONMENT_HAVE_MATH_REMAINDER         1
#define DEE_ENVIRONMENT_HAVE_MATH_REMQUO            1
#define DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN          1
#define DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER         1
#define DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD        1
#define DEE_ENVIRONMENT_HAVE_MATH_FDIM              1
#define DEE_ENVIRONMENT_HAVE_MATH_ISFINITE          1
#define DEE_ENVIRONMENT_HAVE_MATH_ISINF             1
#define DEE_ENVIRONMENT_HAVE_MATH_ISNAN             1
#define DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL          1
#define DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT           1
#define DEE_ENVIRONMENT_HAVE_MATH_ISGREATER         1
#define DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL    1
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESS            1
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL       1
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER     1
#define DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED       1

#ifndef DEE_TYPES_SIZEOF_LDOUBLE
// Without 'long double', this can't possibly exist
#undef DEE_ENVIRONMENT_HAVE_MATH_C99_FLOAT_LDOUBLE
#endif

#ifdef DEE_TYPES_SIZEOF_LLONG
#undef DEE_ENVIRONMENT_HAVE_MATH_LLROUND
#undef DEE_ENVIRONMENT_HAVE_MATH_LLRINT
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1600
// Math functions not present on older versions of vc/vc++
#undef DEE_ENVIRONMENT_HAVE_MATH_ASINH
#undef DEE_ENVIRONMENT_HAVE_MATH_ACOSH
#undef DEE_ENVIRONMENT_HAVE_MATH_ATANH
#undef DEE_ENVIRONMENT_HAVE_MATH_EXP2
#undef DEE_ENVIRONMENT_HAVE_MATH_EXPM1
#undef DEE_ENVIRONMENT_HAVE_MATH_LOGB
#undef DEE_ENVIRONMENT_HAVE_MATH_ILOGB
#undef DEE_ENVIRONMENT_HAVE_MATH_LOG1P
#undef DEE_ENVIRONMENT_HAVE_MATH_LOG2
#undef DEE_ENVIRONMENT_HAVE_MATH_SCALBN
#undef DEE_ENVIRONMENT_HAVE_MATH_SCALBLN
#undef DEE_ENVIRONMENT_HAVE_MATH_CBRT
#undef DEE_ENVIRONMENT_HAVE_MATH_ERF
#undef DEE_ENVIRONMENT_HAVE_MATH_TGAMMA
#undef DEE_ENVIRONMENT_HAVE_MATH_LGAMMA
#undef DEE_ENVIRONMENT_HAVE_MATH_TRUNC
#undef DEE_ENVIRONMENT_HAVE_MATH_ROUND
#undef DEE_ENVIRONMENT_HAVE_MATH_LROUND
#undef DEE_ENVIRONMENT_HAVE_MATH_LLROUND
#undef DEE_ENVIRONMENT_HAVE_MATH_RINT
#undef DEE_ENVIRONMENT_HAVE_MATH_LRINT
#undef DEE_ENVIRONMENT_HAVE_MATH_LLRINT
#undef DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT
#undef DEE_ENVIRONMENT_HAVE_MATH_REMAINDER
#undef DEE_ENVIRONMENT_HAVE_MATH_REMQUO
#undef DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN
#undef DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER
#undef DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD
#undef DEE_ENVIRONMENT_HAVE_MATH_FDIM
#undef DEE_ENVIRONMENT_HAVE_MATH_ISFINITE
#undef DEE_ENVIRONMENT_HAVE_MATH_ISINF
#undef DEE_ENVIRONMENT_HAVE_MATH_ISNAN
#undef DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL
#undef DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT
#undef DEE_ENVIRONMENT_HAVE_MATH_ISGREATER
#undef DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL
#undef DEE_ENVIRONMENT_HAVE_MATH_ISLESS
#undef DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL
#undef DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER
#undef DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED
#endif


#ifndef DEE_ENVIRONMENT_HAVE_MATH_C99_FLOAT_LDOUBLE
#define DEE_ENVIRONMENT_HAVE_MATH_C99_FLOAT_LDOUBLE 0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SIN
#define DEE_ENVIRONMENT_HAVE_MATH_SIN               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_COS
#define DEE_ENVIRONMENT_HAVE_MATH_COS               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_TAN
#define DEE_ENVIRONMENT_HAVE_MATH_TAN               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ASIN
#define DEE_ENVIRONMENT_HAVE_MATH_ASIN              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ACOS
#define DEE_ENVIRONMENT_HAVE_MATH_ACOS              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ATAN
#define DEE_ENVIRONMENT_HAVE_MATH_ATAN              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ATAN2
#define DEE_ENVIRONMENT_HAVE_MATH_ATAN2             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SINH
#define DEE_ENVIRONMENT_HAVE_MATH_SINH              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_COSH
#define DEE_ENVIRONMENT_HAVE_MATH_COSH              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_TANH
#define DEE_ENVIRONMENT_HAVE_MATH_TANH              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ASINH
#define DEE_ENVIRONMENT_HAVE_MATH_ASINH             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ACOSH
#define DEE_ENVIRONMENT_HAVE_MATH_ACOSH             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ATANH
#define DEE_ENVIRONMENT_HAVE_MATH_ATANH             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_EXP
#define DEE_ENVIRONMENT_HAVE_MATH_EXP               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_FREXP
#define DEE_ENVIRONMENT_HAVE_MATH_FREXP             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LOG
#define DEE_ENVIRONMENT_HAVE_MATH_LOG               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LOG10
#define DEE_ENVIRONMENT_HAVE_MATH_LOG10             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_MODF
#define DEE_ENVIRONMENT_HAVE_MATH_MODF              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_EXP2
#define DEE_ENVIRONMENT_HAVE_MATH_EXP2              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_EXPM1
#define DEE_ENVIRONMENT_HAVE_MATH_EXPM1             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ILOGB
#define DEE_ENVIRONMENT_HAVE_MATH_ILOGB             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LOG1P
#define DEE_ENVIRONMENT_HAVE_MATH_LOG1P             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LOG2
#define DEE_ENVIRONMENT_HAVE_MATH_LOG2              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LOGB
#define DEE_ENVIRONMENT_HAVE_MATH_LOGB              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SCALBN
#define DEE_ENVIRONMENT_HAVE_MATH_SCALBN            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SCALBLN
#define DEE_ENVIRONMENT_HAVE_MATH_SCALBLN           0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_POW
#define DEE_ENVIRONMENT_HAVE_MATH_POW               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SQRT
#define DEE_ENVIRONMENT_HAVE_MATH_SQRT              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_CBRT
#define DEE_ENVIRONMENT_HAVE_MATH_CBRT              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_HYPOT
#define DEE_ENVIRONMENT_HAVE_MATH_HYPOT             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ERF
#define DEE_ENVIRONMENT_HAVE_MATH_ERF               0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_TGAMMA
#define DEE_ENVIRONMENT_HAVE_MATH_TGAMMA            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LGAMMA
#define DEE_ENVIRONMENT_HAVE_MATH_LGAMMA            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_CEIL
#define DEE_ENVIRONMENT_HAVE_MATH_CEIL              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_FLOOR
#define DEE_ENVIRONMENT_HAVE_MATH_FLOOR             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_FMOD
#define DEE_ENVIRONMENT_HAVE_MATH_FMOD              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_TRUNC
#define DEE_ENVIRONMENT_HAVE_MATH_TRUNC             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ROUND
#define DEE_ENVIRONMENT_HAVE_MATH_ROUND             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LROUND
#define DEE_ENVIRONMENT_HAVE_MATH_LROUND            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LLROUND
#define DEE_ENVIRONMENT_HAVE_MATH_LLROUND           0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_RINT
#define DEE_ENVIRONMENT_HAVE_MATH_RINT              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LRINT
#define DEE_ENVIRONMENT_HAVE_MATH_LRINT             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_LLRINT
#define DEE_ENVIRONMENT_HAVE_MATH_LLRINT            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT
#define DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT         0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_REMAINDER
#define DEE_ENVIRONMENT_HAVE_MATH_REMAINDER         0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_REMQUO
#define DEE_ENVIRONMENT_HAVE_MATH_REMQUO            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN
#define DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN          0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER
#define DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER         0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD
#define DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD        0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_FDIM
#define DEE_ENVIRONMENT_HAVE_MATH_FDIM              0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISFINITE
#define DEE_ENVIRONMENT_HAVE_MATH_ISFINITE          0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISINF
#define DEE_ENVIRONMENT_HAVE_MATH_ISINF             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISNAN
#define DEE_ENVIRONMENT_HAVE_MATH_ISNAN             0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL
#define DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL          0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT
#define DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT           0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISGREATER
#define DEE_ENVIRONMENT_HAVE_MATH_ISGREATER         0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL
#define DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL    0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISLESS
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESS            0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL       0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER
#define DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER     0
#endif
#ifndef DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED
#define DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED       0
#endif


#if DEE_ENVIRONMENT_HAVE_MATH_C99_FLOAT_LDOUBLE
#define DEE_NATIVE_MATH_F(func) func##f
#define DEE_NATIVE_MATH_D(func) func
#define DEE_NATIVE_MATH_L(func) func##l
#else
#define DEE_NATIVE_MATH_F(func) func
#define DEE_NATIVE_MATH_D(func) func
#define DEE_NATIVE_MATH_L(func) func
#endif

#ifdef DEE_TYPES_SIZEOF_FLOAT
typedef float Dee_float;
#if 1
#define DEE_FLOAT_C(x) x##f
#else
#define DEE_FLOAT_C(x) ((Dee_float)x)
#endif
#else
#undef DEE_NATIVE_MATH_F
#endif

#ifdef DEE_TYPES_SIZEOF_DOUBLE
typedef double Dee_double;
#if 1
#define DEE_DOUBLE_C(x) x
#else
#define DEE_DOUBLE_C(x) ((Dee_double)x)
#endif
#else
#undef DEE_NATIVE_MATH_D
#endif

#ifdef DEE_TYPES_SIZEOF_LDOUBLE
typedef long double Dee_ldouble;
#if 1
#define DEE_LDOUBLE_C(x) x##L
#else
#define DEE_LDOUBLE_C(x) ((Dee_ldouble)x)
#endif
#else
#undef DEE_NATIVE_MATH_L
#endif


#ifdef HUGE_VAL
#ifdef HUGE_VALF
#define DEE_M_HUGE_VALf  HUGE_VALF
#else
#define DEE_M_HUGE_VALf  ((Dee_float)HUGE_VAL)
#endif
#define DEE_M_HUGE_VAL   HUGE_VAL
#ifdef HUGE_VALL
#define DEE_M_HUGE_VALl  HUGE_VALL
#else
#define DEE_M_HUGE_VALl  ((Dee_ldouble)HUGE_VAL)
#endif
#else
#define DEE_M_HUGE_VALf  ((Dee_float)(1.0/0.0))
#define DEE_M_HUGE_VAL   ((Dee_double)(1.0/0.0))
#define DEE_M_HUGE_VALl  ((Dee_ldouble)(1.0/0.0))
#endif

#ifdef INFINITY
#define DEE_M_INFINITYf  ((Dee_float)INFINITY)
#define DEE_M_INFINITY   ((Dee_double)INFINITY)
#define DEE_M_INFINITYl  ((Dee_ldouble)INFINITY)
#elif defined(_MSC_VER)
#define DEE_M_INFINITYf  ((Dee_float)(DBL_MAX+DBL_MAX))
#define DEE_M_INFINITY   ((Dee_double)(DBL_MAX+DBL_MAX))
#define DEE_M_INFINITYl  ((Dee_ldouble)(DBL_MAX+DBL_MAX))
#else
#define DEE_M_INFINITYf  DEE_M_HUGE_VALf
#define DEE_M_INFINITY   DEE_M_HUGE_VAL
#define DEE_M_INFINITYl  DEE_M_HUGE_VALl
#endif

#ifdef NAN
#define DEE_M_NANf       ((Dee_float)NAN)
#define DEE_M_NAN        ((Dee_double)NAN)
#define DEE_M_NANl       ((Dee_ldouble)NAN)
#elif defined(_MSC_VER)
#define DEE_M_NANf       ((Dee_float)((DBL_MAX+DBL_MAX)-(DBL_MAX+DBL_MAX)))
#define DEE_M_NAN        ((Dee_double)((DBL_MAX+DBL_MAX)-(DBL_MAX+DBL_MAX)))
#define DEE_M_NANl       ((Dee_ldouble)((DBL_MAX+DBL_MAX)-(DBL_MAX+DBL_MAX)))
#else
#define DEE_M_NANf       ((Dee_float)(0.0/0.0))
#define DEE_M_NAN        ((Dee_double)(0.0/0.0))
#define DEE_M_NANl       ((Dee_ldouble)(0.0/0.0))
#endif

#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_sinf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_cosf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_tanf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincosf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_asinf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_acosf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_atanf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_atan2f(DEE_A_IN Dee_float y, DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincosf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_sinhf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_coshf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_tanhf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincoshf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_asinhf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_acoshf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_atanhf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincoshf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_expf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_frexpf(DEE_A_IN Dee_float x, DEE_A_OUT int *exp) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_logf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_log10f(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_modff(DEE_A_IN Dee_float x, DEE_A_OUT Dee_float *int_part) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_exp2f(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_expm1f(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_ilogbf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_log1pf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_log2f(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_logbf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_scalbnf(DEE_A_IN Dee_float x, DEE_A_IN int n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_scalbi32nf(DEE_A_IN Dee_float x, DEE_A_IN Dee_int32_t n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_powf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_sqrtf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_cbrtf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_hypotf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_erff(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_tgammaf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_lgammaf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_ceilf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_floorf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_fmodf(DEE_A_IN Dee_float number, DEE_A_IN Dee_float denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_truncf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_roundf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32roundf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64roundf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_rintf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32rintf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64rintf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_nearbyintf(DEE_A_IN Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_remainderf(DEE_A_IN Dee_float number, DEE_A_IN Dee_float denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_remquof(DEE_A_IN Dee_float number, DEE_A_IN Dee_float denom, DEE_A_OUT int *quot) DEE_ATTRIBUTE_NONNULL((3));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_copysignf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_nextafterf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_nexttowardf(DEE_A_IN Dee_float x, DEE_A_IN Dee_ldouble y);
#elif defined(DEE_TYPES_SIZEOF_DOUBLE)
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_nexttowardf(DEE_A_IN Dee_float x, DEE_A_IN Dee_double y);
#elif defined(DEE_TYPES_SIZEOF_FLOAT)
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_nexttowardf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
#endif
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_float) _Dee_m_fdimf(DEE_A_IN Dee_float x, DEE_A_IN Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isfinitef(Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isinff(Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnanf(Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnormalf(Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_signbitf(Dee_float x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreaterf(Dee_float x, Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreaterequalf(Dee_float x, Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessf(Dee_float x, Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessequalf(Dee_float x, Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessgreaterf(Dee_float x, Dee_float y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isunorderedf(Dee_float x, Dee_float y);
#endif

#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_sin(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_cos(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_tan(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincos(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_asin(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_acos(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_atan(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_atan2(DEE_A_IN Dee_double y, DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincos(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_sinh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_cosh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_tanh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincosh(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_asinh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_acosh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_atanh(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincosh(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_exp(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_frexp(DEE_A_IN Dee_double x, DEE_A_OUT int *exp) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_log(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_log10(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_modf(DEE_A_IN Dee_double x, DEE_A_OUT Dee_double *int_part) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_exp2(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_expm1(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_ilogb(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_log1p(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_log2(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_logb(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_scalbn(DEE_A_IN Dee_double x, DEE_A_IN int n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_scalbi32n(DEE_A_IN Dee_double x, DEE_A_IN Dee_int32_t n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_pow(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_sqrt(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_cbrt(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_hypot(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_erf(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_tgamma(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_lgamma(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_ceil(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_floor(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_fmod(DEE_A_IN Dee_double number, DEE_A_IN Dee_double denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_trunc(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_round(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32round(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64round(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_rint(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32rint(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64rint(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_nearbyint(DEE_A_IN Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_remainder(DEE_A_IN Dee_double number, DEE_A_IN Dee_double denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_remquo(DEE_A_IN Dee_double number, DEE_A_IN Dee_double denom, DEE_A_OUT int *quot) DEE_ATTRIBUTE_NONNULL((3));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_copysign(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_nextafter(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_nexttoward(DEE_A_IN Dee_double x, DEE_A_IN Dee_ldouble y);
#elif defined(DEE_TYPES_SIZEOF_DOUBLE)
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_nexttoward(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
#elif defined(DEE_TYPES_SIZEOF_FLOAT)
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_nexttoward(DEE_A_IN Dee_double x, DEE_A_IN Dee_float y);
#endif
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_double) _Dee_m_fdim(DEE_A_IN Dee_double x, DEE_A_IN Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isfinite(Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isinf(Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnan(Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnormal(Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_signbit(Dee_double x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreater(Dee_double x, Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreaterequal(Dee_double x, Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isless(Dee_double x, Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessequal(Dee_double x, Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessgreater(Dee_double x, Dee_double y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isunordered(Dee_double x, Dee_double y);
#endif

#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_sinl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_cosl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_tanl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincosl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_asinl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_acosl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_atanl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_atan2l(DEE_A_IN Dee_ldouble y, DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincosl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_sinhl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_coshl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_tanhl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_sincoshl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_asinhl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_acoshl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_atanhl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST void) _Dee_m_asincoshl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_expl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_frexpl(DEE_A_IN Dee_ldouble x, DEE_A_OUT int *exp) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_logl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_log10l(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_modfl(DEE_A_IN Dee_ldouble x, DEE_A_OUT Dee_ldouble *int_part) DEE_ATTRIBUTE_NONNULL((2));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_exp2l(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_expm1l(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_ilogbl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_log1pl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_log2l(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_logbl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_scalbnl(DEE_A_IN Dee_ldouble x, DEE_A_IN int n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_scalbi32nl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_int32_t n);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_powl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_sqrtl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_cbrtl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_hypotl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_erfl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_tgammal(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_lgammal(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_ceill(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_floorl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_fmodl(DEE_A_IN Dee_ldouble number, DEE_A_IN Dee_ldouble denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_truncl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_roundl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32roundl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64roundl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_rintl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32rintl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64rintl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_nearbyintl(DEE_A_IN Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_remainderl(DEE_A_IN Dee_ldouble number, DEE_A_IN Dee_ldouble denom);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_remquol(DEE_A_IN Dee_ldouble number, DEE_A_IN Dee_ldouble denom, DEE_A_OUT int *quot) DEE_ATTRIBUTE_NONNULL((3));
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_copysignl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_nextafterl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_nexttowardl(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_ldouble) _Dee_m_fdiml(DEE_A_IN Dee_ldouble x, DEE_A_IN Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isfinitel(Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isinfl(Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnanl(Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isnormall(Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_signbitl(Dee_ldouble x);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreaterl(Dee_ldouble x, Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isgreaterequall(Dee_ldouble x, Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessl(Dee_ldouble x, Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessequall(Dee_ldouble x, Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_islessgreaterl(Dee_ldouble x, Dee_ldouble y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED int) _Dee_m_isunorderedl(Dee_ldouble x, Dee_ldouble y);
#endif



#if DEE_ENVIRONMENT_HAVE_MATH_SIN
#define Dee_m_sinf DEE_NATIVE_MATH_F(sin)
#define Dee_m_sin  DEE_NATIVE_MATH_D(sin)
#define Dee_m_sinl DEE_NATIVE_MATH_L(sin)
#else
#define Dee_m_sinf _Dee_m_sinf
#define Dee_m_sin  _Dee_m_sin
#define Dee_m_sinl _Dee_m_sinl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_COS
#define Dee_m_cosf DEE_NATIVE_MATH_F(cos)
#define Dee_m_cos  DEE_NATIVE_MATH_D(cos)
#define Dee_m_cosl DEE_NATIVE_MATH_L(cos)
#else
#define Dee_m_cosf _Dee_m_cosf
#define Dee_m_cos  _Dee_m_cos
#define Dee_m_cosl _Dee_m_cosl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_TAN
#define Dee_m_tanf DEE_NATIVE_MATH_F(tan)
#define Dee_m_tan  DEE_NATIVE_MATH_D(tan)
#define Dee_m_tanl DEE_NATIVE_MATH_L(tan)
#else
#define Dee_m_tanf _Dee_m_tanf
#define Dee_m_tan  _Dee_m_tan
#define Dee_m_tanl _Dee_m_tanl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SIN||\
    DEE_ENVIRONMENT_HAVE_MATH_COS
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincosf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc) { sc[0] = Dee_m_sinf(x),sc[1] = Dee_m_cosf(x); }
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincos(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc) { sc[0] = Dee_m_sin(x),sc[1] = Dee_m_cos(x); }
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincosl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc) { sc[0] = Dee_m_sinl(x),sc[1] = Dee_m_cosl(x); }
#endif
#else
#define Dee_m_sincosf _Dee_m_sincosf
#define Dee_m_sincos  _Dee_m_sincos
#define Dee_m_sincosl _Dee_m_sincosl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ASIN
#define Dee_m_asinf DEE_NATIVE_MATH_F(asin)
#define Dee_m_asin  DEE_NATIVE_MATH_D(asin)
#define Dee_m_asinl DEE_NATIVE_MATH_L(asin)
#else
#define _Dee_m_asinf _Dee_m_asinf
#define _Dee_m_asin  _Dee_m_asin
#define _Dee_m_asinl _Dee_m_asinl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ACOS
#define Dee_m_acosf DEE_NATIVE_MATH_F(acos)
#define Dee_m_acos  DEE_NATIVE_MATH_D(acos)
#define Dee_m_acosl DEE_NATIVE_MATH_L(acos)
#else
#define Dee_m_acosf _Dee_m_acosf
#define Dee_m_acos  _Dee_m_acos
#define Dee_m_acosl _Dee_m_acosl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ATAN
#define Dee_m_atanf DEE_NATIVE_MATH_F(atan)
#define Dee_m_atan  DEE_NATIVE_MATH_D(atan)
#define Dee_m_atanl DEE_NATIVE_MATH_L(atan)
#else
#define Dee_m_atanf _Dee_m_atanf
#define Dee_m_atan  _Dee_m_atan
#define Dee_m_atanl _Dee_m_atanl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ATAN2
#define Dee_m_atan2f DEE_NATIVE_MATH_F(atan2)
#define Dee_m_atan2  DEE_NATIVE_MATH_D(atan2)
#define Dee_m_atan2l DEE_NATIVE_MATH_L(atan2)
#else
#define Dee_m_atan2f _Dee_m_atan2f
#define Dee_m_atan2  _Dee_m_atan2
#define Dee_m_atan2l _Dee_m_atan2l
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ASIN||\
    DEE_ENVIRONMENT_HAVE_MATH_ACOS
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincosf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc) { sc[0] = Dee_m_asinf(x),sc[1] = Dee_m_acosf(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincos(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc) { sc[0] = Dee_m_asin(x),sc[1] = Dee_m_acos(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincosl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc) { sc[0] = Dee_m_asinl(x),sc[1] = Dee_m_acosl(x);  }
#endif
#else
#define Dee_m_asincosf _Dee_m_asincosf
#define Dee_m_asincos  _Dee_m_asincos
#define Dee_m_asincosl _Dee_m_asincosl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SINH
#define Dee_m_sinhf DEE_NATIVE_MATH_F(sinh)
#define Dee_m_sinh  DEE_NATIVE_MATH_D(sinh)
#define Dee_m_sinhl DEE_NATIVE_MATH_L(sinh)
#else
#define Dee_m_sinhf _Dee_m_sinhf
#define Dee_m_sinh  _Dee_m_sinh
#define Dee_m_sinhl _Dee_m_sinhl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_COSH
#define Dee_m_coshf DEE_NATIVE_MATH_F(cosh)
#define Dee_m_cosh  DEE_NATIVE_MATH_D(cosh)
#define Dee_m_coshl DEE_NATIVE_MATH_L(cosh)
#else
#define Dee_m_coshf _Dee_m_coshf
#define Dee_m_cosh  _Dee_m_cosh
#define Dee_m_coshl _Dee_m_coshl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_TANH
#define Dee_m_tanhf DEE_NATIVE_MATH_F(tanh)
#define Dee_m_tanh  DEE_NATIVE_MATH_D(tanh)
#define Dee_m_tanhl DEE_NATIVE_MATH_L(tanh)
#else
#define Dee_m_tanhf _Dee_m_tanhf
#define Dee_m_tanh  _Dee_m_tanh
#define Dee_m_tanhl _Dee_m_tanhl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SINH||\
    DEE_ENVIRONMENT_HAVE_MATH_COSH
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincoshf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc) { sc[0] = Dee_m_sinhf(x),sc[1] = Dee_m_coshf(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincosh(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc) { sc[0] = Dee_m_sinh(x),sc[1] = Dee_m_cosh(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_sincoshl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc) { sc[0] = Dee_m_sinhl(x),sc[1] = Dee_m_coshl(x);  }
#endif
#else
#define Dee_m_sincoshf _Dee_m_sincoshf
#define Dee_m_sincosh  _Dee_m_sincosh
#define Dee_m_sincoshl _Dee_m_sincoshl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ASINH
#define Dee_m_asinhf asinhf
#define Dee_m_asinh  asinh
#define Dee_m_asinhl asinhl
#else
#define Dee_m_asinhf _Dee_m_asinhf
#define Dee_m_asinh  _Dee_m_asinh
#define Dee_m_asinhl _Dee_m_asinhl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ACOSH
#define Dee_m_acoshf acoshf
#define Dee_m_acosh  acosh
#define Dee_m_acoshl acoshl
#else
#define Dee_m_acoshf _Dee_m_acoshf
#define Dee_m_acosh  _Dee_m_acosh
#define Dee_m_acoshl _Dee_m_acoshl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ATANH
#define Dee_m_atanhf atanhf
#define Dee_m_atanh  atanh
#define Dee_m_atanhl atanhl
#else
#define Dee_m_atanhf _Dee_m_atanhf
#define Dee_m_atanh  _Dee_m_atanh
#define Dee_m_atanhl _Dee_m_atanhl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ASINH||\
    DEE_ENVIRONMENT_HAVE_MATH_ACOSH
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincoshf(DEE_A_IN Dee_float x, DEE_A_OUT_W(2) Dee_float *sc) { sc[0] = Dee_m_asinhf(x),sc[1] = Dee_m_acoshf(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincosh(DEE_A_IN Dee_double x, DEE_A_OUT_W(2) Dee_double *sc) { sc[0] = Dee_m_asinh(x),sc[1] = Dee_m_acosh(x);  }
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((2)) DEE_ATTRIBUTE_CONST void) Dee_m_asincoshl(DEE_A_IN Dee_ldouble x, DEE_A_OUT_W(2) Dee_ldouble *sc) { sc[0] = Dee_m_asinhl(x),sc[1] = Dee_m_acoshl(x);  }
#endif
#else
#define Dee_m_asincoshf _Dee_m_asincoshf
#define Dee_m_asincosh  _Dee_m_asincosh
#define Dee_m_asincoshl _Dee_m_asincoshl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_EXP
#define Dee_m_expf expf
#define Dee_m_exp  exp
#define Dee_m_expl expl
#else
#define Dee_m_expf _Dee_m_expf
#define Dee_m_exp  _Dee_m_exp
#define Dee_m_expl _Dee_m_expl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_FREXP
#define Dee_m_frexpf frexpf
#define Dee_m_frexp  frexp
#define Dee_m_frexpl frexpl
#else
#define Dee_m_frexpf _Dee_m_frexpf
#define Dee_m_frexp  _Dee_m_frexp
#define Dee_m_frexpl _Dee_m_frexpl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LOG
#define Dee_m_logf logf
#define Dee_m_log  log
#define Dee_m_logl logl
#else
#define Dee_m_logf _Dee_m_logf
#define Dee_m_log  _Dee_m_log
#define Dee_m_logl _Dee_m_logl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LOG10
#define Dee_m_log10f log10f
#define Dee_m_log10  log10
#define Dee_m_log10l log10l
#else
#define Dee_m_log10f _Dee_m_log10f
#define Dee_m_log10  _Dee_m_log10
#define Dee_m_log10l _Dee_m_log10l
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_MODF
#define Dee_m_modff modff
#define Dee_m_modf  modf
#define Dee_m_modfl modfl
#else
#define Dee_m_modff _Dee_m_modff
#define Dee_m_modf  _Dee_m_modf
#define Dee_m_modfl _Dee_m_modfl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_EXP2
#define Dee_m_exp2f exp2f
#define Dee_m_exp2  exp2
#define Dee_m_exp2l exp2l
#else
#define Dee_m_exp2f _Dee_m_exp2f
#define Dee_m_exp2  _Dee_m_exp2
#define Dee_m_exp2l _Dee_m_exp2l
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_EXPM1
#define Dee_m_expm1f expm1f
#define Dee_m_expm1  expm1
#define Dee_m_expm1l expm1l
#else
#define Dee_m_expm1f _Dee_m_expm1f
#define Dee_m_expm1  _Dee_m_expm1
#define Dee_m_expm1l _Dee_m_expm1l
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ILOGB
#define Dee_m_ilogbf ilogbf
#define Dee_m_ilogb  ilogb
#define Dee_m_ilogbl ilogbl
#else
#define Dee_m_ilogbf _Dee_m_ilogbf
#define Dee_m_ilogb  _Dee_m_ilogb
#define Dee_m_ilogbl _Dee_m_ilogbl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LOG1P
#define Dee_m_log1pf log1pf
#define Dee_m_log1p  log1p
#define Dee_m_log1pl log1pl
#else
#define Dee_m_log1pf _Dee_m_log1pf
#define Dee_m_log1p  _Dee_m_log1p
#define Dee_m_log1pl _Dee_m_log1pl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LOG2
#define Dee_m_log2f log2f
#define Dee_m_log2  log2
#define Dee_m_log2l log2l
#else
#define Dee_m_log2f _Dee_m_log2f
#define Dee_m_log2  _Dee_m_log2
#define Dee_m_log2l _Dee_m_log2l
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LOGB
#define Dee_m_logbf logbf
#define Dee_m_logb  logb
#define Dee_m_logbl logbl
#else
#define Dee_m_logbf _Dee_m_logbf
#define Dee_m_logb  _Dee_m_logb
#define Dee_m_logbl _Dee_m_logbl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SCALBN
#define Dee_m_scalbnf scalbnf
#define Dee_m_scalbn  scalbn
#define Dee_m_scalbnl scalbnl
#else
#define Dee_m_scalbnf _Dee_m_scalbnf
#define Dee_m_scalbn  _Dee_m_scalbn
#define Dee_m_scalbnl _Dee_m_scalbnl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SCALBN && \
    DEE_TYPES_SIZEOF_INT == 4
#define Dee_m_scalbi32nf scalbnf
#define Dee_m_scalbi32n  scalbn
#define Dee_m_scalbi32nl scalbnl
#elif DEE_ENVIRONMENT_HAVE_MATH_SCALBLN && \
      DEE_TYPES_SIZEOF_LONG == 4
#define Dee_m_scalbi32nf scalblnf
#define Dee_m_scalbi32n  scalbln
#define Dee_m_scalbi32nl scalblnl
#else
#define Dee_m_scalbi32nf _Dee_m_scalbi32nf
#define Dee_m_scalbi32n  _Dee_m_scalbi32n
#define Dee_m_scalbi32nl _Dee_m_scalbi32nl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_POW
#define Dee_m_powf powf
#define Dee_m_pow  pow
#define Dee_m_powl powl
#else
#define Dee_m_powf _Dee_m_powf
#define Dee_m_pow  _Dee_m_pow
#define Dee_m_powl _Dee_m_powl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SQRT
#define Dee_m_sqrtf sqrtf
#define Dee_m_sqrt  sqrt
#define Dee_m_sqrtl sqrtl
#else
#define Dee_m_sqrtf _Dee_m_sqrtf
#define Dee_m_sqrt  _Dee_m_sqrt
#define Dee_m_sqrtl _Dee_m_sqrtl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_CBRT
#define Dee_m_cbrtf cbrtf
#define Dee_m_cbrt  cbrt
#define Dee_m_cbrtl cbrtl
#else
#define Dee_m_cbrtf _Dee_m_cbrtf
#define Dee_m_cbrt  _Dee_m_cbrt
#define Dee_m_cbrtl _Dee_m_cbrtl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_HYPOT
#define Dee_m_hypotf hypotf
#define Dee_m_hypot  hypot
#define Dee_m_hypotl hypotl
#else
#define Dee_m_hypotf _Dee_m_hypotf
#define Dee_m_hypot  _Dee_m_hypot
#define Dee_m_hypotl _Dee_m_hypotl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ERF
#define Dee_m_erff erff
#define Dee_m_erf  erf
#define Dee_m_erfl erfl
#else
#define Dee_m_erff _Dee_m_erff
#define Dee_m_erf  _Dee_m_erf
#define Dee_m_erfl _Dee_m_erfl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_TGAMMA
#define Dee_m_tgammaf tgammaf
#define Dee_m_tgamma  tgamma
#define Dee_m_tgammal tgammal
#else
#define Dee_m_tgammaf _Dee_m_tgammaf
#define Dee_m_tgamma  _Dee_m_tgamma
#define Dee_m_tgammal _Dee_m_tgammal
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LGAMMA
#define Dee_m_lgammaf lgammaf
#define Dee_m_lgamma  lgamma
#define Dee_m_lgammal lgammal
#else
#define Dee_m_lgammaf _Dee_m_lgammaf
#define Dee_m_lgamma  _Dee_m_lgamma
#define Dee_m_lgammal _Dee_m_lgammal
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_CEIL
#define Dee_m_ceilf ceilf
#define Dee_m_ceil  ceil
#define Dee_m_ceill ceill
#else
#define Dee_m_ceilf _Dee_m_ceilf
#define Dee_m_ceil  _Dee_m_ceil
#define Dee_m_ceill _Dee_m_ceill
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_FLOOR
#define Dee_m_floorf floorf
#define Dee_m_floor  floor
#define Dee_m_floorl floorl
#else
#define Dee_m_floorf _Dee_m_floorf
#define Dee_m_floor  _Dee_m_floor
#define Dee_m_floorl _Dee_m_floorl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_FMOD
#define Dee_m_fmodf fmodf
#define Dee_m_fmod  fmod
#define Dee_m_fmodl fmodl
#else
#define Dee_m_fmodf _Dee_m_fmodf
#define Dee_m_fmod  _Dee_m_fmod
#define Dee_m_fmodl _Dee_m_fmodl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_TRUNC
#define Dee_m_truncf truncf
#define Dee_m_trunc  trunc
#define Dee_m_truncl truncl
#else
#define Dee_m_truncf _Dee_m_truncf
#define Dee_m_trunc  _Dee_m_trunc
#define Dee_m_truncl _Dee_m_truncl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ROUND
#define Dee_m_roundf roundf
#define Dee_m_round  round
#define Dee_m_roundl roundl
#else
#define Dee_m_roundf _Dee_m_roundf
#define Dee_m_round  _Dee_m_round
#define Dee_m_roundl _Dee_m_roundl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LROUND && \
    DEE_TYPES_SIZEOF_LONG == 4
#define Dee_m_i32roundf lroundf
#define Dee_m_i32round  lround
#define Dee_m_i32roundl lroundl
#elif DEE_ENVIRONMENT_HAVE_MATH_LLROUND && \
      DEE_TYPES_SIZEOF_LLONG == 4
#define Dee_m_i32roundf llroundf
#define Dee_m_i32round  llround
#define Dee_m_i32roundl llroundl
#else
#define Dee_m_i32roundf _Dee_m_i32roundf
#define Dee_m_i32round  _Dee_m_i32round
#define Dee_m_i32roundl _Dee_m_i32roundl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LROUND && \
    DEE_TYPES_SIZEOF_LONG == 8
#define Dee_m_i64roundf lroundf
#define Dee_m_i64round  lround
#define Dee_m_i64roundl lroundl
#elif DEE_ENVIRONMENT_HAVE_MATH_LLROUND && \
      DEE_TYPES_SIZEOF_LLONG == 8
#define Dee_m_i64roundf llroundf
#define Dee_m_i64round  llround
#define Dee_m_i64roundl llroundl
#else
#define Dee_m_i64roundf _Dee_m_i64roundf
#define Dee_m_i64round  _Dee_m_i64round
#define Dee_m_i64roundl _Dee_m_i64roundl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_RINT
#define Dee_m_rintf rintf
#define Dee_m_rint  rint
#define Dee_m_rintl rintl
#else
#define Dee_m_rintf _Dee_m_rintf
#define Dee_m_rint  _Dee_m_rint
#define Dee_m_rintl _Dee_m_rintl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LRINT && \
    DEE_TYPES_SIZEOF_LONG == 4
#define Dee_m_i32rintf lrintf
#define Dee_m_i32rint  lrint
#define Dee_m_i32rintl lrintl
#elif DEE_ENVIRONMENT_HAVE_MATH_LLRINT && \
      DEE_TYPES_SIZEOF_LLONG == 4
#define Dee_m_i32rintf llrintf
#define Dee_m_i32rint  llrint
#define Dee_m_i32rintl llrintl
#else
#define Dee_m_i32rintf _Dee_m_i32rintf
#define Dee_m_i32rint  _Dee_m_i32rint
#define Dee_m_i32rintl _Dee_m_i32rintl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_LRINT && \
    DEE_TYPES_SIZEOF_LONG == 8
#define Dee_m_i64rintf lrintf
#define Dee_m_i64rint  lrint
#define Dee_m_i64rintl lrintl
#elif DEE_ENVIRONMENT_HAVE_MATH_LLRINT && \
      DEE_TYPES_SIZEOF_LLONG == 8
#define Dee_m_i64rintf llrintf
#define Dee_m_i64rint  llrint
#define Dee_m_i64rintl llrintl
#else
#define Dee_m_i64rintf _Dee_m_i64rintf
#define Dee_m_i64rint  _Dee_m_i64rint
#define Dee_m_i64rintl _Dee_m_i64rintl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_NEARBYINT
#define Dee_m_nearbyintf nearbyintf
#define Dee_m_nearbyint  nearbyint
#define Dee_m_nearbyintl nearbyintl
#else
#define Dee_m_nearbyintf _Dee_m_nearbyintf
#define Dee_m_nearbyint  _Dee_m_nearbyint
#define Dee_m_nearbyintl _Dee_m_nearbyintl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_REMAINDER
#define Dee_m_remainderf remainderf
#define Dee_m_remainder  remainder
#define Dee_m_remainderl remainderl
#else
#define Dee_m_remainderf _Dee_m_remainderf
#define Dee_m_remainder  _Dee_m_remainder
#define Dee_m_remainderl _Dee_m_remainderl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_REMQUO
#define Dee_m_remquof remquof
#define Dee_m_remquo  remquo
#define Dee_m_remquol remquol
#else
#define Dee_m_remquof _Dee_m_remquof
#define Dee_m_remquo  _Dee_m_remquo
#define Dee_m_remquol _Dee_m_remquol
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_COPYSIGN
#define Dee_m_copysignf copysignf
#define Dee_m_copysign  copysign
#define Dee_m_copysignl copysignl
#else
#define Dee_m_copysignf _Dee_m_copysignf
#define Dee_m_copysign  _Dee_m_copysign
#define Dee_m_copysignl _Dee_m_copysignl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_NEXTAFTER
#define Dee_m_nextafterf nextafterf
#define Dee_m_nextafter  nextafter
#define Dee_m_nextafterl nextafterl
#else
#define Dee_m_nextafterf _Dee_m_nextafterf
#define Dee_m_nextafter  _Dee_m_nextafter
#define Dee_m_nextafterl _Dee_m_nextafterl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_NEXTTOWARD
#define Dee_m_nexttowardf nexttowardf
#define Dee_m_nexttoward  nexttoward
#define Dee_m_nexttowardl nexttowardl
#else
#define Dee_m_nexttowardf _Dee_m_nexttowardf
#define Dee_m_nexttoward  _Dee_m_nexttoward
#define Dee_m_nexttowardl _Dee_m_nexttowardl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_FDIM
#define Dee_m_fdimf fdimf
#define Dee_m_fdim  fdim
#define Dee_m_fdiml fdiml
#else
#define Dee_m_fdimf _Dee_m_fdimf
#define Dee_m_fdim  _Dee_m_fdim
#define Dee_m_fdiml _Dee_m_fdiml
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISFINITE
#define Dee_m_isfinitef isfinite
#define Dee_m_isfinite  isfinite
#define Dee_m_isfinitel isfinite
#else
#define Dee_m_isfinitef _Dee_m_isfinitef
#define Dee_m_isfinite  _Dee_m_isfinite
#define Dee_m_isfinitel _Dee_m_isfinitel
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISINF
#define Dee_m_isinff isinf
#define Dee_m_isinf  isinf
#define Dee_m_isinfl isinf
#else
#define Dee_m_isinff _Dee_m_isinff
#define Dee_m_isinf  _Dee_m_isinf
#define Dee_m_isinfl _Dee_m_isinfl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISNAN
#define Dee_m_isnanf isnan
#define Dee_m_isnan  isnan
#define Dee_m_isnanl isnan
#else
#define Dee_m_isnanf _Dee_m_isnanf
#define Dee_m_isnan  _Dee_m_isnan
#define Dee_m_isnanl _Dee_m_isnanl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISNORMAL
#define Dee_m_isnormalf isnormal
#define Dee_m_isnormal  isnormal
#define Dee_m_isnormall isnormal
#else
#define Dee_m_isnormalf _Dee_m_isnormalf
#define Dee_m_isnormal  _Dee_m_isnormal
#define Dee_m_isnormall _Dee_m_isnormall
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_SIGNBIT
#define Dee_m_signbitf signbit
#define Dee_m_signbit  signbit
#define Dee_m_signbitl signbit
#else
#define Dee_m_signbitf _Dee_m_signbitf
#define Dee_m_signbit  _Dee_m_signbit
#define Dee_m_signbitl _Dee_m_signbitl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISGREATER
#define Dee_m_isgreaterf isgreater
#define Dee_m_isgreater  isgreater
#define Dee_m_isgreaterl isgreater
#else
#define Dee_m_isgreaterf _Dee_m_isgreaterf
#define Dee_m_isgreater  _Dee_m_isgreater
#define Dee_m_isgreaterl _Dee_m_isgreaterl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISGREATEREQUAL
#define Dee_m_isgreaterequalf isgreaterequal
#define Dee_m_isgreaterequal  isgreaterequal
#define Dee_m_isgreaterequall isgreaterequal
#else
#define Dee_m_isgreaterequalf _Dee_m_isgreaterequalf
#define Dee_m_isgreaterequal  _Dee_m_isgreaterequal
#define Dee_m_isgreaterequall _Dee_m_isgreaterequall
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISLESS
#define Dee_m_islessf isless
#define Dee_m_isless  isless
#define Dee_m_islessl isless
#else
#define Dee_m_islessf _Dee_m_islessf
#define Dee_m_isless  _Dee_m_isless
#define Dee_m_islessl _Dee_m_islessl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISLESSEQUAL
#define Dee_m_islessequalf islessequal
#define Dee_m_islessequal  islessequal
#define Dee_m_islessequall islessequal
#else
#define Dee_m_islessequalf _Dee_m_islessequalf
#define Dee_m_islessequal  _Dee_m_islessequal
#define Dee_m_islessequall _Dee_m_islessequall
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISLESSGREATER
#define Dee_m_islessgreaterf islessgreater
#define Dee_m_islessgreater  islessgreater
#define Dee_m_islessgreaterl islessgreater
#else
#define Dee_m_islessgreaterf _Dee_m_islessgreaterf
#define Dee_m_islessgreater  _Dee_m_islessgreater
#define Dee_m_islessgreaterl _Dee_m_islessgreaterl
#endif

#if DEE_ENVIRONMENT_HAVE_MATH_ISUNORDERED
#define Dee_m_isunorderedf isunordered
#define Dee_m_isunordered  isunordered
#define Dee_m_isunorderedl isunordered
#else
#define Dee_m_isunorderedf _Dee_m_isunorderedf
#define Dee_m_isunordered  _Dee_m_isunordered
#define Dee_m_isunorderedl _Dee_m_isunorderedl
#endif



#define DEE_M_Ef        DEE_FLOAT_C(2.71828182845904523536)
#define DEE_M_E         DEE_DOUBLE_C(2.71828182845904523536)
#define DEE_M_El        DEE_LDOUBLE_C(2.718281828459045235360287471352662498)
#define DEE_M_LOG2Ef    DEE_FLOAT_C(1.44269504088896340736)
#define DEE_M_LOG2E     DEE_DOUBLE_C(1.44269504088896340736)
#define DEE_M_LOG2El    DEE_LDOUBLE_C(1.442695040888963407359924681001892137)
#define DEE_M_LOG10Ef   DEE_FLOAT_C(0.434294481903251827651)
#define DEE_M_LOG10E    DEE_DOUBLE_C(0.434294481903251827651)
#define DEE_M_LOG10El   DEE_LDOUBLE_C(0.434294481903251827651128918916605082)
#define DEE_M_LN2f      DEE_FLOAT_C(0.693147180559945309417)
#define DEE_M_LN2       DEE_DOUBLE_C(0.693147180559945309417)
#define DEE_M_LN2l      DEE_LDOUBLE_C(0.693147180559945309417232121458176568)
#define DEE_M_LN10f     DEE_FLOAT_C(2.30258509299404568402)
#define DEE_M_LN10      DEE_DOUBLE_C(2.30258509299404568402)
#define DEE_M_LN10l     DEE_LDOUBLE_C(2.302585092994045684017991454684364208)
#define DEE_M_PIf       DEE_FLOAT_C(3.14159265358979323846)
#define DEE_M_PI        DEE_DOUBLE_C(3.14159265358979323846)
#define DEE_M_PIl       DEE_LDOUBLE_C(3.1415926535897932384626433832795029)
#define DEE_M_PI_X2f    DEE_FLOAT_C(6.28318530717958647692)
#define DEE_M_PI_X2     DEE_DOUBLE_C(6.28318530717958647692)
#define DEE_M_PI_X2l    DEE_LDOUBLE_C(6.28318530717958647692)
#define DEE_M_PI_2f     DEE_FLOAT_C(1.57079632679489661923)
#define DEE_M_PI_2      DEE_DOUBLE_C(1.57079632679489661923)
#define DEE_M_PI_2l     DEE_LDOUBLE_C(1.570796326794896619231321691639751442)
#define DEE_M_PI_4f     DEE_FLOAT_C(0.785398163397448309616)
#define DEE_M_PI_4      DEE_DOUBLE_C(0.785398163397448309616)
#define DEE_M_PI_4l     DEE_LDOUBLE_C(0.785398163397448309615660845819875721)
#define DEE_M_1_PIf     DEE_FLOAT_C(0.318309886183790671538)
#define DEE_M_1_PI      DEE_DOUBLE_C(0.318309886183790671538)
#define DEE_M_1_PIl     DEE_LDOUBLE_C(0.318309886183790671537767526745028724L)
#define DEE_M_2_PIf     DEE_FLOAT_C(0.636619772367581343076)
#define DEE_M_2_PI      DEE_DOUBLE_C(0.636619772367581343076)
#define DEE_M_2_PIl     DEE_LDOUBLE_C(0.636619772367581343075535053490057448)
#define DEE_M_1_SQRTPIf DEE_FLOAT_C(0.564189583547756286948)
#define DEE_M_1_SQRTPI  DEE_DOUBLE_C(0.564189583547756286948)
#define DEE_M_1_SQRTPIl DEE_LDOUBLE_C(0.564189583547756286948)
#define DEE_M_2_SQRTPIf DEE_FLOAT_C(1.12837916709551257390)
#define DEE_M_2_SQRTPI  DEE_DOUBLE_C(1.12837916709551257390)
#define DEE_M_2_SQRTPIl DEE_LDOUBLE_C(1.128379167095512573896158903121545172)
#define DEE_M_SQRT2f    DEE_FLOAT_C(1.41421356237309504880)
#define DEE_M_SQRT2     DEE_DOUBLE_C(1.41421356237309504880)
#define DEE_M_SQRT2l    DEE_LDOUBLE_C(1.414213562373095048801688724209698079)
#define DEE_M_1_SQRT2f  DEE_FLOAT_C(0.707106781186547524401)
#define DEE_M_1_SQRT2   DEE_DOUBLE_C(0.707106781186547524401)
#define DEE_M_1_SQRT2l  DEE_LDOUBLE_C(0.707106781186547524400844362104849039)
#define DEE_M_RAD2DEGf  DEE_FLOAT_C(57.2957795)
#define DEE_M_RAD2DEG   DEE_DOUBLE_C(57.2957795)
#define DEE_M_RAD2DEGl  DEE_LDOUBLE_C(57.2957795)
#define DEE_M_DEG2RADf  DEE_FLOAT_C(0.0174532925)
#define DEE_M_DEG2RAD   DEE_DOUBLE_C(0.0174532925)
#define DEE_M_DEG2RADl  DEE_LDOUBLE_C(0.0174532925)
#define DEE_M_0_DEGf    DEE_FLOAT_C(0.0)
#define DEE_M_0_DEG     DEE_DOUBLE_C(0.0)
#define DEE_M_0_DEGl    DEE_LDOUBLE_C(0.0)
#define DEE_M_90DEGf    DEE_M_PI_2f
#define DEE_M_90DEG     DEE_M_PI_2
#define DEE_M_90DEGl    DEE_M_PI_2l
#define DEE_M_180DEGf   DEE_M_PIf
#define DEE_M_180DEG    DEE_M_PI
#define DEE_M_180DEGl   DEE_M_PIl
#define DEE_M_270DEGf   DEE_FLOAT_C(4.71238898)
#define DEE_M_270DEG    DEE_DOUBLE_C(4.71238898)
#define DEE_M_270DEGl   DEE_LDOUBLE_C(4.71238898)
#define DEE_M_360DEGf   DEE_M_0_DEGf
#define DEE_M_360DEG    DEE_M_0_DEG
#define DEE_M_360DEGl   DEE_M_0_DEGl

#ifndef DEE_TYPES_SIZEOF_FLOAT
#undef Dee_m_sinf
#undef Dee_m_cosf
#undef Dee_m_tanf
#undef Dee_m_sincosf
#undef Dee_m_asinf
#undef Dee_m_acosf
#undef Dee_m_atanf
#undef Dee_m_atan2f
#undef Dee_m_asincosf
#undef Dee_m_sinhf
#undef Dee_m_coshf
#undef Dee_m_tanhf
#undef Dee_m_sincoshf
#undef Dee_m_asinhf
#undef Dee_m_acoshf
#undef Dee_m_atanhf
#undef Dee_m_asincoshf
#undef Dee_m_expf
#undef Dee_m_frexpf
#undef Dee_m_logf
#undef Dee_m_log10f
#undef Dee_m_modff
#undef Dee_m_exp2f
#undef Dee_m_expm1f
#undef Dee_m_ilogbf
#undef Dee_m_log1pf
#undef Dee_m_log2f
#undef Dee_m_logbf
#undef Dee_m_scalbnf
#undef Dee_m_scalbi32nf
#undef Dee_m_powf
#undef Dee_m_sqrtf
#undef Dee_m_cbrtf
#undef Dee_m_hypotf
#undef Dee_m_erff
#undef Dee_m_tgammaf
#undef Dee_m_lgammaf
#undef Dee_m_ceilf
#undef Dee_m_floorf
#undef Dee_m_fmodf
#undef Dee_m_truncf
#undef Dee_m_roundf
#undef Dee_m_i32roundf
#undef Dee_m_i64roundf
#undef Dee_m_rintf
#undef Dee_m_i32rintf
#undef Dee_m_i64rintf
#undef Dee_m_nearbyintf
#undef Dee_m_remainderf
#undef Dee_m_remquof
#undef Dee_m_copysignf
#undef Dee_m_nextafterf
#undef Dee_m_nexttowardf
#undef Dee_m_fdimf
#undef Dee_m_isfinitef
#undef Dee_m_isinff
#undef Dee_m_isnanf
#undef Dee_m_isnormalf
#undef Dee_m_signbitf
#undef Dee_m_isgreaterf
#undef Dee_m_isgreaterequalf
#undef Dee_m_islessf
#undef Dee_m_islessequalf
#undef Dee_m_islessgreaterf
#undef Dee_m_isunorderedf
#undef DEE_M_HUGE_VALf
#undef DEE_M_INFINITYf
#undef DEE_M_NANf
#undef DEE_M_Ef
#undef DEE_M_LOG2Ef
#undef DEE_M_LOG10Ef
#undef DEE_M_LN2f
#undef DEE_M_LN10f
#undef DEE_M_PIf
#undef DEE_M_PI_X2f
#undef DEE_M_PI_2f
#undef DEE_M_PI_4f
#undef DEE_M_1_PIf
#undef DEE_M_2_PIf
#undef DEE_M_1_SQRTPIf
#undef DEE_M_2_SQRTPIf
#undef DEE_M_SQRT2f
#undef DEE_M_SQRT_2f
#undef DEE_M_RAD2DEGf
#undef DEE_M_DEG2RADf
#undef DEE_M_0_DEGf
#undef DEE_M_90DEGf
#undef DEE_M_180DEGf
#undef DEE_M_270DEGf
#undef DEE_M_360DEGf
#endif

#ifndef DEE_TYPES_SIZEOF_DOUBLE
#undef Dee_m_sin
#undef Dee_m_cos
#undef Dee_m_tan
#undef Dee_m_sincos
#undef Dee_m_asin
#undef Dee_m_acos
#undef Dee_m_atan
#undef Dee_m_atan2
#undef Dee_m_asincos
#undef Dee_m_sinh
#undef Dee_m_cosh
#undef Dee_m_tanh
#undef Dee_m_sincosh
#undef Dee_m_asinh
#undef Dee_m_acosh
#undef Dee_m_atanh
#undef Dee_m_asincosh
#undef Dee_m_exp
#undef Dee_m_frexp
#undef Dee_m_log
#undef Dee_m_log10
#undef Dee_m_modf
#undef Dee_m_exp2
#undef Dee_m_expm1
#undef Dee_m_ilogb
#undef Dee_m_log1p
#undef Dee_m_log2
#undef Dee_m_logb
#undef Dee_m_scalbn
#undef Dee_m_scalbi32n
#undef Dee_m_pow
#undef Dee_m_sqrt
#undef Dee_m_cbrt
#undef Dee_m_hypot
#undef Dee_m_erf
#undef Dee_m_tgamma
#undef Dee_m_lgamma
#undef Dee_m_ceil
#undef Dee_m_floor
#undef Dee_m_fmod
#undef Dee_m_trunc
#undef Dee_m_round
#undef Dee_m_i32round
#undef Dee_m_i64round
#undef Dee_m_rint
#undef Dee_m_i32rint
#undef Dee_m_i64rint
#undef Dee_m_nearbyint
#undef Dee_m_remainder
#undef Dee_m_remquo
#undef Dee_m_copysign
#undef Dee_m_nextafter
#undef Dee_m_nexttoward
#undef Dee_m_fdim
#undef Dee_m_isfinite
#undef Dee_m_isinf
#undef Dee_m_isnan
#undef Dee_m_isnormal
#undef Dee_m_signbit
#undef Dee_m_isgreater
#undef Dee_m_isgreaterequal
#undef Dee_m_isless
#undef Dee_m_islessequal
#undef Dee_m_islessgreater
#undef Dee_m_isunordered
#undef DEE_M_HUGE_VAL
#undef DEE_M_INFINITY
#undef DEE_M_NAN
#undef DEE_M_E
#undef DEE_M_LOG2E
#undef DEE_M_LOG10E
#undef DEE_M_LN2
#undef DEE_M_LN10
#undef DEE_M_PI
#undef DEE_M_PI_X2
#undef DEE_M_PI_2
#undef DEE_M_PI_4
#undef DEE_M_1_PI
#undef DEE_M_2_PI
#undef DEE_M_1_SQRTPI
#undef DEE_M_2_SQRTPI
#undef DEE_M_SQRT2
#undef DEE_M_SQRT_2
#undef DEE_M_RAD2DEG
#undef DEE_M_DEG2RAD
#undef DEE_M_0_DEG
#undef DEE_M_90DEG
#undef DEE_M_180DEG
#undef DEE_M_270DEG
#undef DEE_M_360DEG
#endif

#ifndef DEE_TYPES_SIZEOF_LDOUBLE
#undef Dee_m_sinl
#undef Dee_m_cosl
#undef Dee_m_tanl
#undef Dee_m_sincosl
#undef Dee_m_asinl
#undef Dee_m_acosl
#undef Dee_m_atanl
#undef Dee_m_atan2l
#undef Dee_m_asincosl
#undef Dee_m_sinhl
#undef Dee_m_coshl
#undef Dee_m_tanhl
#undef Dee_m_sincoshl
#undef Dee_m_asinhl
#undef Dee_m_acoshl
#undef Dee_m_atanhl
#undef Dee_m_asincoshl
#undef Dee_m_expl
#undef Dee_m_frexpl
#undef Dee_m_logl
#undef Dee_m_log10l
#undef Dee_m_modfl
#undef Dee_m_exp2l
#undef Dee_m_expm1l
#undef Dee_m_ilogbl
#undef Dee_m_log1pl
#undef Dee_m_log2l
#undef Dee_m_logbl
#undef Dee_m_scalbnl
#undef Dee_m_scalbi32nl
#undef Dee_m_powl
#undef Dee_m_sqrtl
#undef Dee_m_cbrtl
#undef Dee_m_hypotl
#undef Dee_m_erfl
#undef Dee_m_tgammal
#undef Dee_m_lgammal
#undef Dee_m_ceill
#undef Dee_m_floorl
#undef Dee_m_fmodl
#undef Dee_m_truncl
#undef Dee_m_roundl
#undef Dee_m_i32roundl
#undef Dee_m_i64roundl
#undef Dee_m_rintl
#undef Dee_m_i32rintl
#undef Dee_m_i64rintl
#undef Dee_m_nearbyintl
#undef Dee_m_remainderl
#undef Dee_m_remquol
#undef Dee_m_copysignl
#undef Dee_m_nextafterl
#undef Dee_m_nexttowardl
#undef Dee_m_fdiml
#undef Dee_m_isfinitel
#undef Dee_m_isinfl
#undef Dee_m_isnanl
#undef Dee_m_isnormall
#undef Dee_m_signbitl
#undef Dee_m_isgreaterl
#undef Dee_m_isgreaterequall
#undef Dee_m_islessl
#undef Dee_m_islessequall
#undef Dee_m_islessgreaterl
#undef Dee_m_isunorderedl
#undef DEE_M_HUGE_VALl
#undef DEE_M_INFINITYl
#undef DEE_M_NANl
#undef DEE_M_El
#undef DEE_M_LOG2El
#undef DEE_M_LOG10El
#undef DEE_M_LN2l
#undef DEE_M_LN10l
#undef DEE_M_PIl
#undef DEE_M_PI_X2l
#undef DEE_M_PI_2l
#undef DEE_M_PI_4l
#undef DEE_M_1_PIl
#undef DEE_M_2_PIl
#undef DEE_M_1_SQRTPIl
#undef DEE_M_2_SQRTPIl
#undef DEE_M_SQRT2l
#undef DEE_M_SQRT_2l
#undef DEE_M_RAD2DEGl
#undef DEE_M_DEG2RADl
#undef DEE_M_0_DEGl
#undef DEE_M_90DEGl
#undef DEE_M_180DEGl
#undef DEE_M_270DEGl
#undef DEE_M_360DEGl
#endif

// NOTE: Implemented in "integer_impl.inl"
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int8_t) _Dee_m_i8pow(DEE_A_IN Dee_int8_t x, DEE_A_IN Dee_int8_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int16_t) _Dee_m_i16pow(DEE_A_IN Dee_int16_t x, DEE_A_IN Dee_int16_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int32_t) _Dee_m_i32pow(DEE_A_IN Dee_int32_t x, DEE_A_IN Dee_int32_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_int64_t) _Dee_m_i64pow(DEE_A_IN Dee_int64_t x, DEE_A_IN Dee_int64_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_uint8_t) _Dee_m_ui8pow(DEE_A_IN Dee_uint8_t x, DEE_A_IN Dee_uint8_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_uint16_t) _Dee_m_ui16pow(DEE_A_IN Dee_uint16_t x, DEE_A_IN Dee_uint16_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_uint32_t) _Dee_m_ui32pow(DEE_A_IN Dee_uint32_t x, DEE_A_IN Dee_uint32_t y);
DEE_MATH_FUNC_DECL(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_uint64_t) _Dee_m_ui64pow(DEE_A_IN Dee_uint64_t x, DEE_A_IN Dee_uint64_t y);

DEE_DECL_END
#undef DEE_MATH_FUNC_DECL
#endif /* DEE_CONFIG_API_HAVE_MATH_EXPORTS || DEE_LIMITED_API */

#endif /* !GUARD_DEEMON_MATH_H */
