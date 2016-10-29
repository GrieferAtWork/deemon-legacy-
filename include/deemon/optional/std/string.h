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
#ifndef GUARD_DEEMON_OPTIONAL_STD_STRING_H
#define GUARD_DEEMON_OPTIONAL_STD_STRING_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_util.h>
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STRING_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4820)
#include <string.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif


#define DeeMemory_IsOverlapping(a,b,s)\
(((Dee_uintptr_t)(a) > (Dee_uintptr_t)(b) && (Dee_uintptr_t)(a) < ((Dee_uintptr_t)(b)+(s))) ||\
 ((Dee_uintptr_t)(b) > (Dee_uintptr_t)(a) && (Dee_uintptr_t)(b) < ((Dee_uintptr_t)(a)+(s))))

#ifdef DEE_DEBUG
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#define Dee_memcpy_d(dst,src,s)  _Dee_memcpy_d(dst,src,s,__FILE__,__LINE__)
#define Dee_memmove_d(dst,src,s) _Dee_memmove_d(dst,src,s,__FILE__,__LINE__)
#define Dee_memcmp_d(a,b,s)      _Dee_memcmp_d(a,b,s,__FILE__,__LINE__)

DEE_DECL_BEGIN
DEE_STATIC_INLINE(void *) _Dee_memcpy_d(
 DEE_A_OUT_W(s) void *dst, DEE_A_IN_R(s) void const *src, DEE_A_IN Dee_size_t s,
 DEE_A_IN_Z char const *file, DEE_A_IN int line) {
 if (s) {
  DEE_ASSERTF(dst,"%s(%d) : memcpy(NULL,%#p,%Iu) : Expected non-NULL dst with non-ZERO size",file,line,src,s);
  DEE_ASSERTF(src,"%s(%d) : memcpy(%#p,NULL,%Iu) : Expected non-NULL src with non-ZERO size",file,line,dst,s);
  DEE_ASSERTF(!DeeMemory_IsOverlapping(dst,src,s),"%s(%d) : memcpy(%#p,%#p,%Iu) : dst & src are overlapping (use 'memmove' instead)",file,line,dst,src,s);
 }
 return memcpy(dst,src,s);
}
DEE_STATIC_INLINE(void *) _Dee_memmove_d(
 DEE_A_OUT_W(s) void *dst, DEE_A_IN_R(s) void const *src, DEE_A_IN Dee_size_t s,
 DEE_A_IN_Z char const *file, DEE_A_IN int line) {
 if (s) {
  DEE_ASSERTF(dst,"%s(%d) : memmove(NULL,%#p,%Iu) : Expected non-NULL dst with non-ZERO size",file,line,src,s);
  DEE_ASSERTF(src,"%s(%d) : memmove(%#p,NULL,%Iu) : Expected non-NULL src with non-ZERO size",file,line,dst,s);
 }
 return memmove(dst,src,s);
}
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) _Dee_memcmp_d(
 DEE_A_IN_R(s) void const *a, DEE_A_IN_R(s) void const *b, DEE_A_IN Dee_size_t s,
 DEE_A_IN_Z char const *file, DEE_A_IN int line) {
 if (s) {
  DEE_ASSERTF(a,"%s(%d) : memcmp(NULL,%#p,%Iu) : Expected non-NULL a with non-ZERO size",file,line,a,s);
  DEE_ASSERTF(b,"%s(%d) : memcmp(%#p,NULL,%Iu) : Expected non-NULL b with non-ZERO size",file,line,b,s);
 }
 return memcmp(a,b,s);
}
DEE_DECL_END

#ifndef __INTELLISENSE__
#define memcpy  Dee_memcpy_d
#define memmove Dee_memmove_d
#define memcmp  Dee_memcmp_d
#endif
#endif


#endif /* !GUARD_DEEMON_OPTIONAL_STD_STRING_H */
