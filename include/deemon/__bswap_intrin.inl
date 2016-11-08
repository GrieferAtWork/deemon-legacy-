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
#ifndef GUARD_DEEMON___BSWAP_INTRIN_INL
#define GUARD_DEEMON___BSWAP_INTRIN_INL 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// Note about all these byte-swap assembly implementations
// I don't claim that I created these (Sadly I know barely anything about assembler).
// No. I collected these over the years from all over the Internet, but
// I no longer have any idea where exactly I got them from. So aside from putting it
// all together, all the credit for figuring these out goes to whoever created them!
#if !defined(DEE_BUILTIN_BSWAP16) || \
    !defined(DEE_BUILTIN_BSWAP32) || \
    !defined(DEE_BUILTIN_BSWAP64)
#if defined(__DEEMON__) && __has_include(<__builtin_bswap.dee>)
#include <__builtin_bswap.dee>
#endif
#ifndef DEE_BUILTIN_BSWAP16_M
#define DEE_BUILTIN_BSWAP16_M(x) (\
 (((DEE_TYPES_UINT16_T)(x)&DEE_UINT16_C(0x00FF)) << 8)|\
 (((DEE_TYPES_UINT16_T)(x)&DEE_UINT16_C(0xFF00)) >> 8))
#endif
#ifndef DEE_BUILTIN_BSWAP32_M
#define DEE_BUILTIN_BSWAP32_M(x) (\
 (((DEE_TYPES_UINT32_T)(x)&DEE_UINT32_C(0x000000FF)) << 24)|\
 (((DEE_TYPES_UINT32_T)(x)&DEE_UINT32_C(0x0000FF00)) << 8)|\
 (((DEE_TYPES_UINT32_T)(x)&DEE_UINT32_C(0x00FF0000)) >> 8)|\
 (((DEE_TYPES_UINT32_T)(x)&DEE_UINT32_C(0xFF000000)) >> 24))
#endif
#ifndef DEE_BUILTIN_BSWAP64_M
#define DEE_BUILTIN_BSWAP64_M(x) (\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x00000000000000FF)) << 56)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x000000000000FF00)) << 40)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x0000000000FF0000)) << 24)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x00000000FF000000)) << 8)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x000000FF00000000)) >> 8)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x0000FF0000000000)) >> 24)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0x00FF000000000000)) >> 40)|\
 (((DEE_TYPES_UINT64_T)(x)&DEE_UINT64_C(0xFF00000000000000)) >> 56))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// -- 16-bit
#ifndef DEE_BUILTIN_BSWAP16
#if defined(__bswap_16)
#define DEE_BUILTIN_BSWAP16 __bswap_16
#elif defined(bswap_16)
#define DEE_BUILTIN_BSWAP16   bswap_16
#endif /* bswap_16... */
#endif /* !DEE_BUILTIN_BSWAP16 */
#ifdef DEE_BUILTIN_BSWAP16
#define DEE_BUILTIN_BSWAP16_F DEE_BUILTIN_BSWAP16
#else /* DEE_BUILTIN_BSWAP16 */
#if defined(__GNUC__) && defined(__i386__) && \
    !(__GNUC__ == 2 && __GNUC_MINOR__ == 95 /* broken gcc version */)
#define DEE_BUILTIN_BSWAP16_F _DeePrivateBSwap16_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT16_T)
_DeePrivateBSwap16_Impl(DEE_TYPES_UINT16_T x) {
 __asm__("xchgb %b0,%h0": "=q"(x):"0"(x));
 return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
#define DEE_BUILTIN_BSWAP16_F _DeePrivateBSwap16_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT16_T)
_DeePrivateBSwap16_Impl(DEE_TYPES_UINT16_T x) {
 __asm__("xchgb %b0,%h0": "=Q"(x):"0"(x));
 return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
#define DEE_BUILTIN_BSWAP16_F _DeePrivateBSwap16_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT16_T)
_DeePrivateBSwap16_Impl(DEE_TYPES_UINT16_T x) {
 int result;
 __asm__("rlwimi %0,%2,8,16,23": "=&r"(result):"0"(x >> 8), "r"(x));
 return (DEE_TYPES_UINT16_T)result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
#define DEE_BUILTIN_BSWAP16_F _DeePrivateBSwap16_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT16_T)
_DeePrivateBSwap16_Impl(DEE_TYPES_UINT16_T x) {
 __asm__("rorw #8,%0": "=d"(x): "0"(x):"cc");
 return x;
}
#elif defined(__GNUC__) || __has_builtin(__builtin_bswap16)
#define DEE_BUILTIN_BSWAP16_F __builtin_bswap16
#elif defined(_MSC_VER)
#define DEE_BUILTIN_BSWAP16_F _byteswap_ushort
DEE_A_RET_WUNUSED unsigned short __cdecl _byteswap_ushort(DEE_A_IN unsigned short);
#pragma intrinsic(_byteswap_ushort)
#else
#define DEE_BUILTIN_BSWAP16_F _DeePrivateBSwap16_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT16_T)
_DeePrivateBSwap16_Impl(DEE_TYPES_UINT16_T x) {
 return DEE_BUILTIN_BSWAP16_M(x);
}
#endif /* ... */
#endif /* !DEE_BUILTIN_BSWAP16 */

// -- 32-bit
#ifndef DEE_BUILTIN_BSWAP32
#if defined(__bswap_32)
#define DEE_BUILTIN_BSWAP32 __bswap_32
#elif defined(bswap_32)
#define DEE_BUILTIN_BSWAP32   bswap_32
#endif /* bswap_32... */
#endif /* !DEE_BUILTIN_BSWAP32 */
#ifdef DEE_BUILTIN_BSWAP32
#define DEE_BUILTIN_BSWAP32_F DEE_BUILTIN_BSWAP32
#else /* DEE_BUILTIN_BSWAP32 */
#if defined(__GNUC__) && defined(__i386__)
#define DEE_BUILTIN_BSWAP32_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT32_T)
_DeePrivateBSwap32_Impl(DEE_TYPES_UINT32_T x) {
 __asm__("bswap %0": "=r"(x):"0"(x));
 return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
#define DEE_BUILTIN_BSWAP32_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT32_T)
_DeePrivateBSwap32_Impl(DEE_TYPES_UINT32_T x) {
 __asm__("bswapl %0": "=r"(x):"0"(x));
 return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
#define DEE_BUILTIN_BSWAP32_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT32_T)
_DeePrivateBSwap32_Impl(DEE_TYPES_UINT32_T x) {
 DEE_TYPES_UINT32_T result;
 __asm__("rlwimi %0,%2,24,16,23": "=&r"(result):"0"(x >> 24), "r"(x));
 __asm__("rlwimi %0,%2,8,8,15": "=&r"(result):"0"(result), "r"(x));
 __asm__("rlwimi %0,%2,24,0,7": "=&r"(result):"0"(result), "r"(x));
 return result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
#define DEE_BUILTIN_BSWAP32_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT32_T)
_DeePrivateBSwap32_Impl(DEE_TYPES_UINT32_T x) {
 __asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0": "=d"(x): "0"(x):"cc");
 return x;
}
#elif defined(__GNUC__) || __has_builtin(__builtin_bswap32)
#define DEE_BUILTIN_BSWAP32_F __builtin_bswap32
#elif defined(_MSC_VER)
#define DEE_BUILTIN_BSWAP32_F _byteswap_ulong
DEE_A_RET_WUNUSED unsigned long __cdecl _byteswap_ulong(DEE_A_IN unsigned long);
#pragma intrinsic(_byteswap_ulong)
#else
#define DEE_BUILTIN_BSWAP32_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT32_T)
_DeePrivateBSwap32_Impl(DEE_TYPES_UINT32_T x) {
 return DEE_BUILTIN_BSWAP32_M(x);
}
#endif /* ... */
#endif /* !DEE_BUILTIN_BSWAP32 */

// -- 64-bit
#ifndef DEE_BUILTIN_BSWAP64
#if defined(__bswap_64)
#define DEE_BUILTIN_BSWAP64 __bswap_64
#elif defined(bswap_64)
#define DEE_BUILTIN_BSWAP64   bswap_64
#endif /* bswap_64... */
#endif /* !DEE_BUILTIN_BSWAP64 */
#ifdef DEE_BUILTIN_BSWAP64
#define DEE_BUILTIN_BSWAP64_F DEE_BUILTIN_BSWAP64
#else /* DEE_BUILTIN_BSWAP64 */
#if defined(__GNUC__) && defined(__i386__)
#define DEE_BUILTIN_BSWAP64_F _DeePrivateBSwap64_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT64_T)
_DeePrivateBSwap64_Impl(DEE_TYPES_UINT64_T x) {
 union{struct{DEE_TYPES_UINT32_T a,b;} s; DEE_TYPES_UINT64_T u;} v;
 v.u = x;
 __asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1": "=r"(v.s.a), "=r"(v.s.b):"0"(v.s.a),"1"(v.s.b));
 return v.u;
}
#elif defined(__GNUC__) && defined(__x86_64__)
#define DEE_BUILTIN_BSWAP64_F _DeePrivateBSwap64_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT64_T)
_DeePrivateBSwap64_Impl(DEE_TYPES_UINT64_T x) {
 __asm__("bswapq %0": "=r"(x):"0"(x));
 return x;
}
#elif defined(__GNUC__) || __has_builtin(__builtin_bswap64)
#define DEE_BUILTIN_BSWAP64_F __builtin_bswap64
#elif defined(_MSC_VER)
#define DEE_BUILTIN_BSWAP64_F _byteswap_uint64
DEE_A_RET_WUNUSED unsigned __int64 __cdecl _byteswap_uint64(DEE_A_IN unsigned __int64);
#pragma intrinsic(_byteswap_uint64)
#else
#define DEE_BUILTIN_BSWAP64_F _DeePrivateBSwap32_Impl
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_TYPES_UINT64_T)
_DeePrivateBSwap64_Impl(DEE_TYPES_UINT64_T x) {
 return DEE_BUILTIN_BSWAP64_M(x);
}
#endif /* ... */
#endif /* !DEE_BUILTIN_BSWAP64 */


#ifdef __cplusplus
}
#endif

#if (__has_builtin(__builtin_choose_expr) || defined(__GNUC__)) && \
    (__has_builtin(__builtin_constant_p) || defined(__GNUC__))
#if defined(__DEEMON__)
#ifndef DEE_BUILTIN_BSWAP16
# define DEE_BUILTIN_BSWAP16(x) __builtin_choose_expr(__builtin_constant_p(x),DEE_BUILTIN_BSWAP16_M(x),DEE_BUILTIN_BSWAP16_F(x))
#endif
#ifndef DEE_BUILTIN_BSWAP32
# define DEE_BUILTIN_BSWAP32(x) __builtin_choose_expr(__builtin_constant_p(x),DEE_BUILTIN_BSWAP32_M(x),DEE_BUILTIN_BSWAP32_F(x))
#endif
#ifndef DEE_BUILTIN_BSWAP64
# define DEE_BUILTIN_BSWAP64(x) __builtin_choose_expr(__builtin_constant_p(x),DEE_BUILTIN_BSWAP64_M(x),DEE_BUILTIN_BSWAP64_F(x))
#endif
#else
#ifndef DEE_BUILTIN_BSWAP16
# define DEE_BUILTIN_BSWAP16(x) (__builtin_constant_p(x) ? DEE_BUILTIN_BSWAP16_M(x) : DEE_BUILTIN_BSWAP16_F(x))
#endif
#ifndef DEE_BUILTIN_BSWAP32
# define DEE_BUILTIN_BSWAP32(x) (__builtin_constant_p(x) ? DEE_BUILTIN_BSWAP32_M(x) : DEE_BUILTIN_BSWAP32_F(x))
#endif
#ifndef DEE_BUILTIN_BSWAP64
# define DEE_BUILTIN_BSWAP64(x) (__builtin_constant_p(x) ? DEE_BUILTIN_BSWAP64_M(x) : DEE_BUILTIN_BSWAP64_F(x))
#endif
#endif
#else
#ifndef DEE_BUILTIN_BSWAP16
#define DEE_BUILTIN_BSWAP16    DEE_BUILTIN_BSWAP16_F
#endif
#ifndef DEE_BUILTIN_BSWAP32
#define DEE_BUILTIN_BSWAP32    DEE_BUILTIN_BSWAP32_F
#endif
#ifndef DEE_BUILTIN_BSWAP64
#define DEE_BUILTIN_BSWAP64    DEE_BUILTIN_BSWAP64_F
#endif
#endif
#endif

#ifdef DEE_PLATFORM_LIL_ENDIAN
# define DEE_BUILTIN_LESWAP16    /* nothing */
# define DEE_BUILTIN_LESWAP32    /* nothing */
# define DEE_BUILTIN_LESWAP64    /* nothing */
# define DEE_BUILTIN_BESWAP16    DEE_BUILTIN_BSWAP16
# define DEE_BUILTIN_BESWAP32    DEE_BUILTIN_BSWAP32
# define DEE_BUILTIN_BESWAP64    DEE_BUILTIN_BSWAP64
# define DEE_BUILTIN_LESWAP16_M  /* nothing */
# define DEE_BUILTIN_LESWAP32_M  /* nothing */
# define DEE_BUILTIN_LESWAP64_M  /* nothing */
# define DEE_BUILTIN_BESWAP16_M  DEE_BUILTIN_BSWAP16_M
# define DEE_BUILTIN_BESWAP32_M  DEE_BUILTIN_BSWAP32_M
# define DEE_BUILTIN_BESWAP64_M  DEE_BUILTIN_BSWAP64_M
#elif defined(DEE_PLATFORM_BIG_ENDIAN)
# define DEE_BUILTIN_LESWAP16    DEE_BUILTIN_BSWAP16
# define DEE_BUILTIN_LESWAP32    DEE_BUILTIN_BSWAP32
# define DEE_BUILTIN_LESWAP64    DEE_BUILTIN_BSWAP64
# define DEE_BUILTIN_BESWAP16    /* nothing */
# define DEE_BUILTIN_BESWAP32    /* nothing */
# define DEE_BUILTIN_BESWAP64    /* nothing */
# define DEE_BUILTIN_LESWAP16_M  DEE_BUILTIN_BSWAP16_M
# define DEE_BUILTIN_LESWAP32_M  DEE_BUILTIN_BSWAP32_M
# define DEE_BUILTIN_LESWAP64_M  DEE_BUILTIN_BSWAP64_M
# define DEE_BUILTIN_BESWAP16_M  /* nothing */
# define DEE_BUILTIN_BESWAP32_M  /* nothing */
# define DEE_BUILTIN_BESWAP64_M  /* nothing */
#else
# error "Unknown/Unsupported Endian"
#endif

#endif /* !GUARD_DEEMON___BSWAP_INTRIN_INL */
