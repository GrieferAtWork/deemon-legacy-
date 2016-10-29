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
#ifndef GUARD_TPP_C
#define GUARD_TPP_C 1
#ifndef TPP_LIMITED_API
#define TPP_LIMITED_API 1 // Enable the limited API
#endif

#include "tpp_conf.inl"

#if TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO
#pragma push_macro("TPP_CONFIG_HAVE_DEPRECATED_API")
#else /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
#undef TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED
#ifdef TPP_CONFIG_HAVE_DEPRECATED_API
#if TPP_CONFIG_HAVE_DEPRECATED_API
# define TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED 1
#else /* TPP_CONFIG_HAVE_DEPRECATED_API */
# define TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED 0
#endif /* !TPP_CONFIG_HAVE_DEPRECATED_API */
#endif
#endif /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
// Disable deprecated API functions
#undef TPP_CONFIG_HAVE_DEPRECATED_API
#define TPP_CONFIG_HAVE_DEPRECATED_API 0

#include "tpp.h"
#if TPP_USE_DEBUG_NEW
#include <debug_new.h>
#endif /* TPP_USE_DEBUG_NEW */

#if TPP_USE_DEBUG_NEW
#include <debug_new_disable.inl>
#endif /* TPP_USE_DEBUG_NEW */
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#if TPP_USE_DEBUG_NEW
#include <debug_new_enable.inl>
#endif /* TPP_USE_DEBUG_NEW */

#ifdef TPP_PLATFORM_WINDOWS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4668)
#endif
#include <Windows.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#else /* TPP_PLATFORM_WINDOWS */
#include <unistd.h>
#endif /* !TPP_PLATFORM_WINDOWS */
#include <sys/stat.h>

#ifndef TPP_CALLBACK_PRAGMA_MESSAGE
//////////////////////////////////////////////////////////////////////////
// int TPP_CALLBACK_PRAGMA_MESSAGE(TPPLexerObject const *self, char const *p, size_t s)
// >> Called from #pragma message(...)
//  - A return value of -1 is interpreted as an error
#define TPP_CALLBACK_PRAGMA_MESSAGE(self,p,s) (fwrite(p,sizeof(char),s,stderr),0)
#endif /* !TPP_CALLBACK_PRAGMA_MESSAGE */

#ifndef TPP_CALLBACK_ERROR_MESSAGE
//////////////////////////////////////////////////////////////////////////
// int TPP_CALLBACK_ERROR_MESSAGE(TPPLexerObject const *self, char const *p)
// >> Called to output error text
//  - A return value of -1 is interpreted as an error
#endif /* !TPP_CALLBACK_ERROR_MESSAGE */

#ifndef TPP_CALLBACK_PARSE_PRAGMA
//////////////////////////////////////////////////////////////////////////
// int TPP_CALLBACK_PARSE_PRAGMA(TPPLexerObject *self, TPPTokenObject *token, int is_directive)
// >> Called 
#ifdef TPPLexer_PARSE_PRAGMA // Deprecated name
#define TPP_CALLBACK_PARSE_PRAGMA TPPLexer_PARSE_PRAGMA
#endif
#endif

#ifndef TPP_CALLBACK_NO_MEMORY
//////////////////////////////////////////////////////////////////////////
// void TPP_CALLBACK_NO_MEMORY(void)
// >> Called if tpp fails to allocate more memory
#define TPP_CALLBACK_NO_MEMORY() (void)0
#endif

#ifndef TPP_CALLBACK_MALLOC_NZ
//////////////////////////////////////////////////////////////////////////
// Default allocator implementation
#if TPP_USE_DEBUG_NEW
#define TPP_CALLBACK_FREE        free
#define TPP_CALLBACK_FREE_NN     free_nn
#define TPP_CALLBACK_MALLOC_NZ   malloc_nz
#define TPP_CALLBACK_CALLOC_NZ   calloc_nz
#define TPP_CALLBACK_REALLOC_NZ  realloc_nz
#define TPP_CALLBACK_REALLOC_NNZ realloc_nnz
#define TPP_CALLBACK_STRDUP_NZ   strdup_nz
#else /* TPP_USE_DEBUG_NEW */
#define TPP_CALLBACK_FREE        free
#define TPP_CALLBACK_FREE_NN     free
#define TPP_CALLBACK_MALLOC_NZ   malloc
#define TPP_CALLBACK_CALLOC_NZ   calloc
#define TPP_CALLBACK_REALLOC_NZ  realloc
#define TPP_CALLBACK_REALLOC_NNZ realloc
#define TPP_CALLBACK_STRDUP_NZ   strdup
#endif /* !TPP_USE_DEBUG_NEW */
#endif

#ifndef TPP_CALLBACK_FREE
#define TPP_CALLBACK_FREE(p) \
 do{void*_temp_p=(p);if(_temp_p)TPP_CALLBACK_FREE_NN(_temp_p);}while(0)
#endif

#ifndef TPP_CALLBACK_FATAL_ERROR
//////////////////////////////////////////////////////////////////////////
// int TPP_CALLBACK_FATAL_ERROR(TPPLexerObject *lexer, int code, TPPTokenObject *token_, char const *message)
// >> Called if a the lexer has failed because it encountered a fatal error
#endif

#if defined(TPP_PLATFORM_WINDOWS)
#define TPP_IS_DIRSEP(c)  ((c) == '/' || c == '\\')
#define TPP_IS_ABSPATH(p) (TPP_IS_DIRSEP((p)[0]) || ((p)[0] && (p)[1] == ':' && TPP_IS_DIRSEP((p)[2])))
#define TPP_PATHCMP       stricmp
#define TPP_SYS_SEP       '\\'
#else
#define TPP_IS_DIRSEP(c)  (c == '/')
#define TPP_IS_ABSPATH(p) TPP_IS_DIRSEP(p[0])
#define TPP_PATHCMP       strcmp
#define TPP_SYS_SEP       '/'
#endif

#ifdef __cplusplus
#define TPP_REINTERPRET_CAST(T,x) reinterpret_cast<T>(x)
#define TPP_CONST_CAST(T,x)       const_cast<T>(x)
#define TPP_STATIC_CAST(T,x)      static_cast<T>(x)
#define TPP_DYNAMIC_CAST(T,x)     dynamic_cast<T>(x)
#define TPP_VOID_CAST             static_cast<void>
#define TPP_LVALUE_CAST(T,x)      reinterpret_cast<T&>(x)
#else
#define TPP_REINTERPRET_CAST(T,x) ((T)(x))
#define TPP_CONST_CAST(T,x)       ((T)(x))
#define TPP_STATIC_CAST(T,x)      ((T)(x))
#define TPP_DYNAMIC_CAST(T,x)     ((T)(x))
#define TPP_VOID_CAST             (void)
#define TPP_LVALUE_CAST(T,x)      *((T*)&(x))
#endif

//////////////////////////////////////////////////////////////////////////
// Attributes are not written in the source
#define TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL(args)
#define TPP_ATTRIBUTE_GCC_SOURCE_PRINTF_STRING(fmt,va_args)

TPP_DECLS_BEGIN_DISABLE_WARNINGS
#ifdef __cplusplus
extern "C" {
#endif

#if __has_builtin(__builtin_expect) ||\
  (!defined(__clang__) && defined(__GNUC__))
#define TPP_LIKELY(x)   __builtin_expect(!!(x),1)
#define TPP_UNLIKELY(x) __builtin_expect(!!(x),0)
#else
#define TPP_LIKELY      /* nothing */
#define TPP_UNLIKELY    /* nothing */
#endif

#if TPP_CONFIG_USE_CHARACTER_TRAITS
// TODO: This should really be written in deemon...
/*[[[cog
from cog import *
flag_lower = 0x01
flag_upper = 0x02
flag_alpha = 0x04
flag_lower_ansi = 0x10
flag_upper_ansi = 0x20
flag_alpha_ansi = 0x40
flag_digit = 0x08
flag_white = 0x80
def is_lower(c): return ord(c) >= ord('a') and ord(c) <= ord('z')
def is_upper(c): return ord(c) >= ord('A') and ord(c) <= ord('Z')
def is_alpha(c): return is_lower(c) or is_upper(c) or c == '_'
def is_lower_ansi(c): return ord(c) >= 224 and ord(c) < 255
def is_upper_ansi(c): return ord(c) >= 192 and ord(c) < 223
def is_alpha_ansi(c): return ord(c) >= 192
def is_digit(c): return ord(c) >= ord('0') and ord(c) <= ord('9')
def is_white(c): return (ord(c) >= 0x09 and ord(c) <= 0x0D) or (ord(c) == ord(' '))
def get_flags(c):
 if not c is int: c = ord(c)
 res = 0
 if is_lower(chr(c)):      res = res | flag_lower
 if is_upper(chr(c)):      res = res | flag_upper
 if is_alpha(chr(c)):      res = res | flag_alpha
 if is_digit(chr(c)):      res = res | flag_digit
 if is_white(chr(c)):      res = res | flag_white
 if is_lower_ansi(chr(c)): res = res | flag_lower_ansi
 if is_upper_ansi(chr(c)): res = res | flag_upper_ansi
 if is_alpha_ansi(chr(c)): res = res | flag_alpha_ansi
 return res
was_lf = 0
def write(s):
 global was_lf
 out(s)
 was_lf = s.endswith("\n")
def outch(c):
 h = hex(c)[2:].upper()
 if len(h) == 1: h = "0"+h
 write("\\x"+h)
def lf():
 global was_lf
 if not was_lf: out("\n")
 was_lf = 1
for dollar in reversed(range(0,2)):
 if dollar:
  write("#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA\n")
 else:
  write("\"")
  lf()
  write("#else /"+"* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA *"+"/\n")
 write("#define TPP_CHARACTER_MAP()\\\n \"")
 for c in range(0,256):
  flags = get_flags(chr(c))
  if chr(c) == '$' and dollar:
   flags = flags | flag_alpha
  outch(get_flags(chr(c)))
  if c != 255 and c % 16 == 15:
   write("\"\\")
   lf()
   write(" \"")
write("\"")
lf()
write("#endif /"+"* !TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA *"+"/")
]]]*/
#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
#define TPP_CHARACTER_MAP()\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x80\x80\x80\x80\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x00\x00\x00\x00\x00\x00"\
 "\x00\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06"\
 "\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x00\x00\x00\x00\x04"\
 "\x00\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05"\
 "\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60"\
 "\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x40"\
 "\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50"\
 "\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x40"
#else /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
#define TPP_CHARACTER_MAP()\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x80\x80\x80\x80\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x00\x00\x00\x00\x00\x00"\
 "\x00\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06"\
 "\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x06\x00\x00\x00\x00\x04"\
 "\x00\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05"\
 "\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"\
 "\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60"\
 "\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x60\x40"\
 "\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50"\
 "\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x40"
#endif /* !TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
 //[[[end]]]
static char const TPP_CHARACTER_MAP[] = TPP_CHARACTER_MAP();
#undef TPP_CHARACTER_MAP

#define TPP_CH_IS_LOWER_UTF8(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x01')!=0)
#define TPP_CH_IS_UPPER_UTF8(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x02')!=0)
#define TPP_CH_IS_ALPHA_UTF8(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x03')!=0)
#define TPP_CH_IS_LOWER_ANSI(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x10')!=0)
#define TPP_CH_IS_UPPER_ANSI(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x20')!=0)
#if 1
// Same effect, but faster!
// NOTE: Only possible, because "c" is only evaluated once!
#define TPP_CH_IS_ALPHA_ANSI(c) ((c)>=192)
#else
#define TPP_CH_IS_ALPHA_ANSI(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x40')!=0)
#endif
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
#define TPP_CH_IS_LOWER(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x11')!=0)
#define TPP_CH_IS_UPPER(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x22')!=0)
#define TPP_CH_IS_ALPHA(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x44')!=0)
#define TPP_CH_IS_ALNUM(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x4C')!=0) // alpha or digit
#else /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
#define TPP_CH_IS_LOWER(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x01')!=0)
#define TPP_CH_IS_UPPER(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x02')!=0)
#define TPP_CH_IS_ALPHA(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x04')!=0)
#define TPP_CH_IS_ALNUM(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x0C')!=0) // alpha or digit
#endif /* !TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
#define TPP_CH_IS_WHSPC(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x80')!=0)
#define TPP_CH_IS_DIGIT(c) ((TPP_CHARACTER_MAP[(unsigned char)(c)]&'\x08')!=0)

#else /* TPP_CONFIG_USE_CHARACTER_TRAITS */

#define TPP_PRIVATE_DECLARE_CHAR_FUNC(name,result)\
TPP_DECL_STATIC_INLINE(int) name(char c){return result;}
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_LOWER_UTF8,c >= 'a' && c <= 'z')
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_UPPER_UTF8,c >= 'A' && c <= 'Z')
#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_ALPHA_UTF8,(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '$'))
#else /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_ALPHA_UTF8,(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
#endif /* !TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_LOWER_ANSI,c >= 224 && c < 255)
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_UPPER_ANSI,c >= 192 && c < 223)
#define TPP_CH_IS_ALPHA_ANSI(c) ((c) >= 192)
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_LOWER,TPP_CH_IS_LOWER_UTF8(c) || TPP_CH_IS_LOWER_ANSI(c))
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_UPPER,TPP_CH_IS_UPPER_UTF8(c) || TPP_CH_IS_UPPER_ANSI(c))
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_ALPHA,TPP_CH_IS_ALPHA_UTF8(c) || TPP_CH_IS_ALPHA_ANSI(c))
#else /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
#define TPP_CH_IS_LOWER TPP_CH_IS_LOWER_UTF8
#define TPP_CH_IS_UPPER TPP_CH_IS_UPPER_UTF8
#define TPP_CH_IS_ALPHA TPP_CH_IS_ALPHA_UTF8
#endif /* !TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_DIGIT,c >= '0' && c <= '9')
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_ALNUM,TPP_CH_IS_ALPHA(c) || (c >= '0' && c <= '9'))
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_WHITE,c >= 0x09 && c <= 0x0D)
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_SPACE,c == '\t' || c == ' ')
TPP_PRIVATE_DECLARE_CHAR_FUNC(TPP_CH_IS_WHSPC,(c >= 0x09 && c <= 0x0D) || c == ' ')
#undef TPP_PRIVATE_DECLARE_CHAR_FUNC

#endif /* !TPP_CONFIG_USE_CHARACTER_TRAITS */

//TPP_STATIC_ASSERT(!TPP_CH_IS_WHSPC('\0'),"zero-char must not be whitespace");
TPP_STATIC_ASSERT(TPP_TOK_STR == '\"',"TPP_TOK_STR must be \'\\\"\'");
TPP_STATIC_ASSERT(TPP_TOK_CHR == '\'',"TPP_TOK_CHR must be \'\\\'\'");


#if 0 && TPP_DEBUG
#define TPP_PRIVATE_CONFIG_VERBOSE 1
#else
#define TPP_PRIVATE_CONFIG_VERBOSE 0
#endif
#define _TPP_VERBOSE_LOG(...) (void)0
#if TPP_PRIVATE_CONFIG_VERBOSE
#define TPP_VERBOSE_LOG(...) (printf("[verbose] "),printf(__VA_ARGS__))
#else /* TPP_PRIVATE_CONFIG_VERBOSE */
#define TPP_VERBOSE_LOG(...) (void)0
#endif /* !TPP_PRIVATE_CONFIG_VERBOSE */

#ifndef TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL
#define TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL TPP_PRIVATE_CONFIG_VERBOSE
#endif /* !TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL */

#define TPP_VERBOSE_LOG_PARSING_STRING_PRAGMA(token_,text)\
 TPP_VERBOSE_LOG("%s(%d) : Parsing string _Pragma: %s\n",\
 TPPToken_File(token_),TPPToken_Line(token_)+1,(text))
#define TPP_VERBOSE_LOG_CONSTANT_EXPRESSION_PARSED(token_,result)\
 TPP_VERBOSE_LOG("%s(%d) : Evaluated constant expression: %d\n",\
 TPPToken_File(token_),TPPToken_Line(token_)+1,TPP_STATIC_CAST(int,result))
#define TPP_VERBOSE_LOG_SKITPPING_GUARDED_INCLUDE(filename,lexer,guard_id)\
 TPP_VERBOSE_LOG("Skipping #include\"%s\" guarded by\"%s\"\n",\
 (filename),TPPKeywordList_NameOf(&(lexer)->l_keywords,(guard_id)))
#define TPP_VERBOSE_LOG_DETECTED_PRAGMA_ONCE(filename)\
 TPP_VERBOSE_LOG("file\"%s\" now guarded with #pragma once\n",(filename))
#define TPP_VERBOSE_LOG_DETECTED_INCLUDE_GUARD(filename,lexer,guard_id)\
 TPP_VERBOSE_LOG("Detected #include guard\"%s\" for file\"%s\"\n",\
 TPPKeywordList_NameOf(&(lexer)->l_keywords,(guard_id)),(filename))
#define TPP_VERBOSE_LOG_INCLUDE(filename,sys_include,include_next)\
 TPP_VERBOSE_LOG("#%s %c%s%c\n",\
  (include_next) ? "include_next" : "include",\
  (sys_include) ? '<' : '\"',(filename),(sys_include) ? '>' : '\"')
#define TPP_VERBOSE_LOG_HAS_INCLUDE(filename,sys_include,include_next)\
 TPP_VERBOSE_LOG("%s(%c%s%c)\n",\
  (include_next) ? "__has_include_next" : "__has_include",\
  (sys_include) ? '<' : '\"',(filename),(sys_include) ? '>' : '\"')
#define TPP_VERBOSE_LOG_TPP_COUNTER(name)\
 TPP_VERBOSE_LOG("__TPP_COUNTER(%s)",(name))


#if TPP_DEBUG && defined(_MSC_VER)
#if TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO
#pragma push_macro("memcpy")
#pragma push_macro("memmove")
#pragma push_macro("memcmp")
#endif /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
#undef memcpy
#undef memmove
#undef memcmp
// If an access violation happens in the builtin
// memcpy/memmove/memset function, it will corrupt the stack for the debugger.
// So in debug mode, we use our own functions,
// that don't corrupt the stack! (and do some additional checking)
static void *TPP_debug_memmove(void *dst, void const *src, size_t s) {
 char *dst_ = TPP_STATIC_CAST(char *,dst);
 char const *src_ = TPP_STATIC_CAST(char const *,src);
 TPP_ASSERT(dst),TPP_ASSERT(src);
 while (s--) *dst_++ = *src_++;
 return dst;
}
static void *TPP_debug_memcpy(void *dst, void const *src, size_t s) {
 TPP_ASSERT(!(
  TPP_STATIC_CAST(char *,dst) > TPP_STATIC_CAST(char const *,src) &&
  TPP_STATIC_CAST(char *,dst) < TPP_STATIC_CAST(char const *,src)+s) &&
  "memcpy: dst and src are overlapping");
 return TPP_debug_memmove(dst,src,s);
}
static int TPP_debug_memcmp(void const *lhs, void const *rhs, size_t s) {
 if (!s) return 0;
 TPP_ASSERT(lhs);
 TPP_ASSERT(rhs);
 while (--s && *TPP_REINTERPRET_CAST(char const *,lhs) == 
               *TPP_REINTERPRET_CAST(char const *,rhs)) {
  ++TPP_LVALUE_CAST(char const *,lhs);
  ++TPP_LVALUE_CAST(char const *,rhs);
 }
 return *TPP_REINTERPRET_CAST(unsigned char const *,lhs)-
        *TPP_REINTERPRET_CAST(unsigned char const *,rhs);
}
#define memcpy  TPP_debug_memcpy
#define memmove TPP_debug_memmove
#define memcmp  TPP_debug_memcmp
#define TPP_PRIVATE_CONFIG_HAVE_DEBUG_MEMCPY 1
#endif

#define TPP_WARNINGS_PUSH

#if TPP_LOAD_STATIC
static
#endif /* TPP_LOAD_STATIC */
TPPWarningMode const TPPWarningsDefaultData[TPPWarnings_COUNT] = {
 TPPWarningMode_Warn,    // {Warning P0000} TPPWarn_ExpectedKeywordAfterDefine
 TPPWarningMode_Warn,    // {Warning P0001} TPPWarn_RedefiningBuiltinKeyword
 TPPWarningMode_Warn,    // {Warning P0002} TPPWarn_UnknownPreprocessorDirective
 TPPWarningMode_Warn,    // {Warning P0003} TPPWarn_StarSlashOutsideOfComment
 TPPWarningMode_Error,   // {Error   P0004} TPPWarn_DirectiveError  (make #error an error)
 TPPWarningMode_Warn,    // {Warning P0005} TPPWarn_DirectiveWarning
 TPPWarningMode_Warn,    // {Warning P0006} TPPWarn_ExpectedKeywordOrLParenAfterDefined
 TPPWarningMode_Warn,    // {Warning P0007} TPPWarn_ExpectedKeywordAfterDefined
 TPPWarningMode_Warn,    // {Warning P0008} TPPWarn_ExpectedRParenAfterDefined
 TPPWarningMode_Warn,    // {Warning P0009} TPPWarn_ExpectedKeywordForMacroArgument
 TPPWarningMode_Warn,    // {Warning P0010} TPPWarn_ExpectedRParenAfterVaArgs
 TPPWarningMode_Warn,    // {Warning P0011} TPPWarn_ExpectedCommaOrRParenForMacroFuncDecl
 TPPWarningMode_Warn,    // {Warning P0012} TPPWarn_InvalidMacroArgCount
 TPPWarningMode_Warn,    // {Warning P0013} TPPWarn_UnexpectedEOFInMacroArgList
 TPPWarningMode_Warn,    // {Warning P0014} TPPWarn_ExpectedIncludeString
 TPPWarningMode_Warn,    // {Warning P0015} TPPWarn_IncludeFileNotFound
 TPPWarningMode_Warn,    // {Warning P0016} TPPWarn_SysIncludeFileNotFound
 TPPWarningMode_Warn,    // {Warning P0017} TPPWarn_ExpectedLParenAfterTPPCounter
 TPPWarningMode_Warn,    // {Warning P0018} TPPWarn_ExpectedKeywordAfterTPPCounter
 TPPWarningMode_Warn,    // {Warning P0019} TPPWarn_ExpectedRParenAfterTPPCounter
 TPPWarningMode_Warn,    // {Warning P0020} TPPWarn_ExpectedLParenAfterTPPRandom
 TPPWarningMode_Warn,    // {Warning P0021} TPPWarn_ExpectedIntegerAfterTPPRandom1
 TPPWarningMode_Warn,    // {Warning P0022} TPPWarn_ExpectedCommaAfterTPPRandom
 TPPWarningMode_Warn,    // {Warning P0023} TPPWarn_ExpectedIntegerAfterTPPRandom2
 TPPWarningMode_Warn,    // {Warning P0024} TPPWarn_ExpectedRParenAfterTPPRandom
 TPPWarningMode_Warn,    // {Warning P0025} TPPWarn_MacroAlreadyDefined
 TPPWarningMode_Warn,    // {Warning P0026} TPPWarn_ExpectedKeywordAfterUndef
 TPPWarningMode_Warn,    // {Warning P0027} TPPWarn_ExpectedKeywordAfterIfdef
 TPPWarningMode_Warn,    // {Warning P0028} TPPWarn_ElseWithoutIfdef
 TPPWarningMode_Warn,    // {Warning P0029} TPPWarn_ElseAfterElse
 TPPWarningMode_Warn,    // {Warning P0030} TPPWarn_ElifWithoutIfdef
 TPPWarningMode_Warn,    // {Warning P0031} TPPWarn_ElifAfterElse
 TPPWarningMode_Warn,    // {Warning P0032} TPPWarn_IfdefWithoutEndif
 TPPWarningMode_Warn,    // {Warning P0033} TPPWarn_EndifWithoutIfdef
 TPPWarningMode_Warn,    // {Warning P0034} TPPWarn_ExpectedIntAfterLine
 TPPWarningMode_Warn,    // {Warning P0035} TPPWarn_ExpectedStringAfterLine
 TPPWarningMode_Ignored, // {Ignored P0036} TPPWarn_MacroDoesntExist    (ignore #undef for non-existent macro)
 TPPWarningMode_Warn,    // {Warning P0037} TPPWarn_CantUndefBuiltinMacro
 TPPWarningMode_Warn,    // {Warning P0038} TPPWarn_ExpectedLParenAfterHasInclude
 TPPWarningMode_Warn,    // {Warning P0039} TPPWarn_ExpectedRParenAfterHasInclude
 TPPWarningMode_Warn,    // {Warning P0040} TPPWarn_ExpectedCollonAfterQuestion
 TPPWarningMode_Warn,    // {Warning P0041} TPPWarn_ExpectedInteger
 TPPWarningMode_Warn,    // {Warning P0042} TPPWarn_ExpectedRparenAfterLParen
 TPPWarningMode_Warn,    // {Warning P0043} TPPWarn_UnexpectedTokenInConstExpr
 TPPWarningMode_Warn,    // {Warning P0044} TPPWarn_ExpectedLParenAfterTPPEval
 TPPWarningMode_Warn,    // {Warning P0045} TPPWarn_ExpectedRParenAfterTPPEval
 TPPWarningMode_Warn,    // {Warning P0046} TPPWarn_ExpectedLParenAfterPragma
 TPPWarningMode_Warn,    // {Warning P0047} TPPWarn_ExpectedStringAfterPragma
 TPPWarningMode_Warn,    // {Warning P0048} TPPWarn_ExpectedRParenAfterPragma
 TPPWarningMode_Warn,    // {Warning P0049} TPPWarn_UnexpectedToken
 TPPWarningMode_Warn,    // {Warning P0050} TPPWarn_InvalidWarning
 TPPWarningMode_Warn,    // {Warning P0051} TPPWarn_CantPopWarnings
 TPPWarningMode_Warn,    // {Warning P0052} TPPWarn_TPPLoadFileNotFound
 TPPWarningMode_Warn,    // {Warning P0053} TPPWarn_ExpectedStringAfterPushMacro
 TPPWarningMode_Warn,    // {Warning P0054} TPPWarn_CantPopMacro
 TPPWarningMode_Warn,    // {Warning P0055} TPPWarn_ExpectedLParenAfterTPPStrDecompile
 TPPWarningMode_Warn,    // {Warning P0056} TPPWarn_ExpectedStringAfterTPPStrDecompile
 TPPWarningMode_Warn,    // {Warning P0057} TPPWarn_ExpectedRParenAfterTPPStrDecompile
 TPPWarningMode_Warn,    // {Warning P0058} TPPWarn_ExpectedLParen
 TPPWarningMode_Warn,    // {Warning P0059} TPPWarn_ExpectedRParen
 TPPWarningMode_Warn,    // {Warning P0060} TPPWarn_DeprecatedKeyword
 TPPWarningMode_Warn,    // {Warning P0061} TPPWarn_InvalidPathCasing
 TPPWarningMode_Warn,    // {Warning P0062} TPPWarn_DivideByZero
 TPPWarningMode_Warn,    // {Warning P0063} TPPWarn_ReusedMacroParameter
 TPPWarningMode_Warn,    // {Warning P0064} TPPWarn_VaArgsUsedAsMacroParameter
 TPPWarningMode_Warn,    // {Warning P0065} TPPWarn_VaCommaUsedAsMacroParameter
 TPPWarningMode_Warn,    // {Warning P0066} TPPWarn_Unexpected
 TPPWarningMode_Warn,    // {Warning P0067} TPPWarn_VaArgsMustBeLastParameter
 TPPWarningMode_Warn,    // {Warning P0068} TPPWarn_ExpectedBoolExpression
 TPPWarningMode_Warn,    // {Warning P0069} TPPWarn_ExpectedBoolExpressionNot
 TPPWarningMode_Warn,    // {Warning P0070} TPPWarn_ExpectedBoolExpressionLhsOP
 TPPWarningMode_Warn,    // {Warning P0071} TPPWarn_ExpectedBoolExpressionRhsOP
 TPPWarningMode_Warn,    // {Warning P0072} reserved
 TPPWarningMode_Warn,    // {Warning P0073} reserved
 TPPWarningMode_Warn,    // {Warning P0074} reserved
 TPPWarningMode_Warn,    // {Warning P0075} reserved
 TPPWarningMode_Warn,    // {Warning P0076} reserved
 TPPWarningMode_Warn,    // {Warning P0077} reserved
 TPPWarningMode_Warn,    // {Warning P0078} reserved
 TPPWarningMode_Warn,    // {Warning P0079} reserved
 TPPWarningMode_Warn,    // {Warning P0080} reserved
 TPPWarningMode_Warn,    // {Warning P0081} reserved
 TPPWarningMode_Warn,    // {Warning P0082} reserved
 TPPWarningMode_Warn,    // {Warning P0083} reserved
 TPPWarningMode_Warn,    // {Warning P0084} reserved
 TPPWarningMode_Warn,    // {Warning P0085} reserved
 TPPWarningMode_Warn,    // {Warning P0086} reserved
 TPPWarningMode_Warn,    // {Warning P0087} reserved
 TPPWarningMode_Warn,    // {Warning P0088} reserved
 TPPWarningMode_Warn,    // {Warning P0089} reserved
 TPPWarningMode_Warn,    // {Warning P0090} reserved
 TPPWarningMode_Warn,    // {Warning P0091} reserved
 TPPWarningMode_Warn,    // {Warning P0092} reserved
 TPPWarningMode_Warn,    // {Warning P0093} reserved
 TPPWarningMode_Warn,    // {Warning P0094} reserved
 TPPWarningMode_Warn,    // {Warning P0095} reserved
 TPPWarningMode_Warn,    // {Warning P0096} reserved
 TPPWarningMode_Warn,    // {Warning P0097} reserved
 TPPWarningMode_Warn,    // {Warning P0098} reserved
 TPPWarningMode_Warn,    // {Warning P0099} reserved
 TPPWarningMode_Warn,    // {Warning P0100} reserved
 TPPWarningMode_Warn,    // {Warning P0101} reserved
 TPPWarningMode_Warn,    // {Warning P0102} reserved
 TPPWarningMode_Warn,    // {Warning P0103} reserved
 TPPWarningMode_Warn,    // {Warning P0104} reserved
 TPPWarningMode_Warn,    // {Warning P0105} reserved
 TPPWarningMode_Warn,    // {Warning P0106} reserved
 TPPWarningMode_Warn,    // {Warning P0107} reserved
 TPPWarningMode_Warn,    // {Warning P0108} reserved
 TPPWarningMode_Warn,    // {Warning P0109} reserved
 TPPWarningMode_Warn,    // {Warning P0110} reserved
 TPPWarningMode_Warn,    // {Warning P0111} reserved
 TPPWarningMode_Warn,    // {Warning P0112} reserved
 TPPWarningMode_Warn,    // {Warning P0113} reserved
 TPPWarningMode_Warn,    // {Warning P0114} reserved
 TPPWarningMode_Warn,    // {Warning P0115} reserved
 TPPWarningMode_Warn,    // {Warning P0116} reserved
 TPPWarningMode_Warn,    // {Warning P0117} reserved
 TPPWarningMode_Warn,    // {Warning P0118} reserved
 TPPWarningMode_Warn,    // {Warning P0119} reserved
 TPPWarningMode_Warn,    // {Warning P0120} reserved
 TPPWarningMode_Warn,    // {Warning P0121} reserved
 TPPWarningMode_Warn,    // {Warning P0122} reserved
 TPPWarningMode_Warn,    // {Warning P0123} reserved
 TPPWarningMode_Warn,    // {Warning P0124} reserved
 TPPWarningMode_Warn,    // {Warning P0125} reserved
 TPPWarningMode_Warn,    // {Warning P0126} reserved
 TPPWarningMode_Warn,    // {Warning P0127} reserved
 TPPWarningMode_Warn,    // {Warning P0128} reserved
 TPPWarningMode_Warn,    // {Warning P0129} reserved
 TPPWarningMode_Warn,    // {Warning P0130} reserved
 TPPWarningMode_Warn,    // {Warning P0131} reserved
 TPPWarningMode_Warn,    // {Warning P0132} reserved
 TPPWarningMode_Warn,    // {Warning P0133} reserved
 TPPWarningMode_Warn,    // {Warning P0134} reserved
 TPPWarningMode_Warn,    // {Warning P0135} reserved
 TPPWarningMode_Warn,    // {Warning P0136} reserved
 TPPWarningMode_Warn,    // {Warning P0137} reserved
 TPPWarningMode_Warn,    // {Warning P0138} reserved
 TPPWarningMode_Warn,    // {Warning P0139} reserved
 TPPWarningMode_Warn,    // {Warning P0140} reserved
 TPPWarningMode_Warn,    // {Warning P0141} reserved
 TPPWarningMode_Warn,    // {Warning P0142} reserved
 TPPWarningMode_Warn,    // {Warning P0143} reserved
 TPPWarningMode_Warn,    // {Warning P0144} reserved
 TPPWarningMode_Warn,    // {Warning P0145} reserved
 TPPWarningMode_Warn,    // {Warning P0146} reserved
 TPPWarningMode_Warn,    // {Warning P0147} reserved
 TPPWarningMode_Warn,    // {Warning P0148} reserved
 TPPWarningMode_Warn,    // {Warning P0149} reserved
#define W(mode)            mode,
#define WARNING(name,mode) mode,
#ifdef TPP_USER_WARNINGS_FILE_0
#include TPP_USER_WARNINGS_FILE_0()
#endif /* TPP_USER_WARNINGS_FILE_0 */
#ifdef TPP_USER_WARNINGS_FILE_1
#include TPP_USER_WARNINGS_FILE_1()
#endif /* TPP_USER_WARNINGS_FILE_1 */
#ifdef TPP_USER_WARNINGS_FILE_2
#include TPP_USER_WARNINGS_FILE_2()
#endif /* TPP_USER_WARNINGS_FILE_2 */
#ifdef TPP_USER_WARNINGS_FILE_3
#include TPP_USER_WARNINGS_FILE_3()
#endif /* TPP_USER_WARNINGS_FILE_3 */
#undef WARNING
#undef W
};



#ifndef TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
#define TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR 0
#endif /* !TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */


#ifndef TPP_REPLACE_COMMENTS_WITH_SPACE
// Will be needed later, if I decide to add col positioning and
// more in-depth information to include trees, like a pointer showing
// you the exact positions of tokens in question (siliar to what GCC does)
#define TPP_REPLACE_COMMENTS_WITH_SPACE 0
#endif /* !TPP_REPLACE_COMMENTS_WITH_SPACE */

static void _TPPFile_Format(TPPFileObject *self, TPPFileFormatFlag flags) {
 char *write_iter;
 char const *read_iter,*end,*temp_iter;
 int in_str = 0,in_err_dir = 0,at_start_of_line = 0;
 TPP_ASSERT(self);
 read_iter = write_iter = self->f_data;
 end = self->f_end;
#define WRITE(ch) (*write_iter++ = (ch))
 while (read_iter != end) {
  char ch = *read_iter++;
hnd:
  switch (ch) {
   case '\0': break; // Ignore '\0' chars
#if TPP_CONFIG_HAVE_TRIGRAPHS
   case '?':
    if (TPP_UNLIKELY(*read_iter == '?')) {
     switch (read_iter[1]) {
      case '=':  *write_iter++ = '#';  break; /* ??= */
      case '(':  *write_iter++ = '[';  break; /* ??( */
      case '/':  *write_iter++ = '\\'; break; /* ??/ */
      case ')':  *write_iter++ = ']';  break; /* ??) */
      case '\'': *write_iter++ = '^';  break; /* ??' */
      case '<':  *write_iter++ = '{';  break; /* ??< */
      case '!':  *write_iter++ = '|';  break; /* ??! */
      case '>':  *write_iter++ = '}';  break; /* ??> */
      case '-':  *write_iter++ = '~';  break; /* ??- */
      default: goto def;
     }
     read_iter += 2;
    }
    break;
#endif /* TPP_CONFIG_HAVE_TRIGRAPHS */
   case '/':
    if (TPP_LIKELY(!in_str && (flags&TPPFileFormatFlag_KEEP_COMMENTS)!=TPPFileFormatFlag_KEEP_COMMENTS)) {
     ch = *read_iter;
     if (TPP_UNLIKELY(ch == '/')) {
      ++read_iter;
      // Line comment (like this one)
      while (1) {
       ch = *read_iter++;
       while (ch == '\\' && (*read_iter == '\r' || *read_iter == '\n')) {
        if (*read_iter++ == '\r' && *read_iter == '\n') ++read_iter;
        ch = *read_iter++;
        WRITE('\n');
       }
       if (TPP_UNLIKELY(ch == '\n')) break;
#if TPP_REPLACE_COMMENTS_WITH_SPACE && 0 // Could never affect anything!
       WRITE(' ');
#endif /* TPP_REPLACE_COMMENTS_WITH_SPACE */
       if (TPP_UNLIKELY(read_iter == end)) goto do_end;
      }
      goto hnd; // write the '\n' or '\0'
     } else if (TPP_UNLIKELY(ch == '*')) {
      WRITE(' '); // Write space, to prevent token concat with comments
      if (TPP_UNLIKELY(++read_iter == end)) goto do_end;
      /* Multi-line comment (like this one) */
      while (1) {
       ch = *read_iter++;
       if (read_iter == end) {
#if TPP_REPLACE_COMMENTS_WITH_SPACE
        WRITE(' ');
#endif /* TPP_REPLACE_COMMENTS_WITH_SPACE */
        goto do_end;
       }
       if (TPP_UNLIKELY(ch == '*' && *read_iter == '/')) {
        if (TPP_UNLIKELY(++read_iter == end)) goto do_end;
        ch = *read_iter;
        if (TPP_UNLIKELY(++read_iter == end)) goto do_end;
#if TPP_REPLACE_COMMENTS_WITH_SPACE
        WRITE(' ');
        WRITE(' ');
#endif /* TPP_REPLACE_COMMENTS_WITH_SPACE */
        goto hnd;
       } else if (TPP_UNLIKELY(ch == '\r')) {
        // Need, to keep the amount of lines correct
        if (*read_iter == '\n') if (TPP_UNLIKELY(++read_iter == end)) goto do_end;
        WRITE('\n');
       } else if (TPP_UNLIKELY(ch == '\n')) {
        // ditto
        WRITE('\n');
       }
#if TPP_REPLACE_COMMENTS_WITH_SPACE
       else WRITE(' ');
#endif /* TPP_REPLACE_COMMENTS_WITH_SPACE */
      }
     } else ch = '/';
    }
    goto def;
   case '\r':
    if (*read_iter == '\n') ++read_iter;
    ch = '\n';
    // fallthrough
   case '\n':
    in_err_dir = 0;
    at_start_of_line = 1;
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
    in_str = 0;
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
    goto def;
   case '\\':
    if (TPP_UNLIKELY(
     (flags&TPPFileFormatFlag_FMT_LINE_WRAP)==TPPFileFormatFlag_FMT_LINE_WRAP &&
     (*read_iter == '\r' || *read_iter == '\n'))) {
     ++read_iter; // Skip this linefeed (NOTE: Affects the line count)
     if (read_iter[-1] == '\r' && *read_iter == '\n') ++read_iter;
    } else if (in_str) {
     WRITE('\\');
     if (read_iter == end) goto do_end;
     ch = *read_iter++;
     if (ch == '\r') {
      if (*read_iter == '\n') ++read_iter;
      ch = '\n';
     }
     goto def;
    } else {
     goto def;
    }
    break;
   case '\"': if (!in_err_dir && (in_str & 0x02) == 0) in_str ^= 0x01; goto def;
   case '\'': if (!in_err_dir && (in_str & 0x01) == 0) in_str ^= 0x02; goto def;
   case '#':
    if (at_start_of_line
#if !TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
        && !in_str
#endif /* !TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
        ) {
     temp_iter = read_iter;
     while (1) {
      ch = *temp_iter++;
      if (!TPP_CH_IS_WHSPC(ch) && (ch != '\\' || (
       *temp_iter != '\n' && *temp_iter != '\r'))) break;
     }
#define READ()\
do{\
 ch = *temp_iter++; \
 if (ch != '\\')break;\
 if (*temp_iter == '\r'){\
  if (*++temp_iter == '\n') ++temp_iter;\
 } else if (*temp_iter == '\n') {\
  ++temp_iter;\
 } else break;\
}while(1)
     // These are required so we correctly ignore
     // unresolved ' and " characters after #error and #warning
     // >> This ~is~ kind of complicated to do and not a problem
     //    you might run into, but tpp is good enough to handle this case!
#if TPP_CONFIG_HAVE_WARNING
     if (ch == 'w') {READ();
     if (ch == 'a') {READ();
     if (ch == 'r') {READ();
     if (ch == 'n') {READ();
     if (ch == 'i') {READ();
     if (ch == 'n') {READ();
     if (ch == 'g') {READ();
     if (TPP_CH_IS_WHSPC(ch)) in_err_dir = 1;
     }}}}}}} else
#endif /* TPP_CONFIG_HAVE_WARNING */
     if (ch == 'e') {READ();
     if (ch == 'r') {READ();
     if (ch == 'r') {READ();
     if (ch == 'o') {READ();
     if (ch == 'r') {READ();
     if (TPP_CH_IS_WHSPC(ch)) in_err_dir = 1;
     }}}}}
#undef READ
     ch = '#';
    }
    // fallthrough
   default:def:
    if (!TPP_CH_IS_WHSPC(ch)) at_start_of_line = 0;
    WRITE(ch);
    break;
  }
 }
do_end:
 // Update the end of the file
 *write_iter = '\0';
 self->f_end = write_iter;
#undef WRITE
}


#define _TPPFile_NewSetReferenceFromToken(self,tk)\
 do{\
  TPP_ASSERT(self);\
  TPP_ASSERT(tk);\
  TPP_ASSERT((self)->f_ref_file == NULL);\
  (self)->f_ref_file = (tk)->tk_file;\
  TPPFile_XINCREF((self)->f_ref_file);\
  (self)->f_ref_file_off = (tk)->tk_file_off;\
 }while(0)
#define _TPPFile_SetReferenceFromToken(self,tk)\
 do{\
  TPP_ASSERT(self);\
  TPP_ASSERT(tk);\
  TPPFile_XDECREF((self)->f_ref_file);\
  (self)->f_ref_file = (tk)->tk_file;\
  TPPFile_XINCREF((self)->f_ref_file);\
  (self)->f_ref_file_off = (tk)->tk_file_off;\
 }while(0)

TPP_IMPL_FUNC(void) _TPPFile_Free(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPPFile_XDECREF(self->f_prev);
 TPPFile_XDECREF(self->f_ref_file);
 TPP_CALLBACK_FREE(self->f_name);
 TPP_CALLBACK_FREE(self->f_new_name);
 TPP_CALLBACK_FREE_NN(self);
}

TPP_DECL_STATIC_INLINE(int) _TPPFile_CmpEq(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *lhs,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *rhs)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 size_t size;
 TPP_ASSERT(lhs);
 TPP_ASSERT(rhs);
 size = TPPFile_SIZE(lhs);
 if (TPP_LIKELY(size != TPPFile_SIZE(rhs))) return 0;
 return memcmp(lhs->f_data,rhs->f_data,size) == 0;
}

#ifndef TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC
#define TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC 1
#endif /* !TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC */

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_New(size_t n) {
#if TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC
 TPPFileObject *result = TPP_REINTERPRET_CAST(TPPFileObject *,TPP_CALLBACK_CALLOC_NZ(
  1,offsetof(TPPFileObject,f_data)+((n+1)*sizeof(char))));
#else /* TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC */
 TPPFileObject *result = TPP_REINTERPRET_CAST(TPPFileObject *,TPP_CALLBACK_MALLOC_NZ(
  offsetof(TPPFileObject,f_data)+((n+1)*sizeof(char))));
#endif /* !TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC */
 if (TPP_LIKELY(result)) {
  result->f_refcnt = 1;
#if !TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC
  result->f_prev = 0;
  result->f_name = NULL;
  result->f_new_name = NULL;
  result->f_ref_file = NULL;
  result->f_line_offset = 0;
  //result->f_ref_file_off = 0;
  *TPP_CONST_CAST(char *,result->f_end = (
   result->f_iter = result->f_data)+n) = '\0'; // Terminating\0 char
#else /* !TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC */
  result->f_end = (result->f_iter = result->f_data)+n;
#endif /* TPP_PRIVATE_CONFIG_TPPFILE_NEW_USES_CALLOC */
 } else TPP_CALLBACK_NO_MEMORY();
 return result;
}
TPP_ATTRIBUTE_USE_RETURN TPPFileObject *TPPFile_Copy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *result; size_t data_size;
 if ((result = (TPPFileObject *)TPP_CALLBACK_MALLOC_NZ(
  (((uintptr_t)self->f_end)-(uintptr_t)self)+sizeof(char))) == NULL) {
no_mem:
  TPP_CALLBACK_NO_MEMORY();
  return NULL;
 }
 if (self->f_name) {
  if ((result->f_name = TPP_CALLBACK_STRDUP_NZ(self->f_name)) == NULL) {
no_mem_1: TPP_CALLBACK_FREE_NN(result); goto no_mem;
  }
 } else result->f_name = NULL;
 if (self->f_new_name) {
  if ((result->f_new_name = TPP_CALLBACK_STRDUP_NZ(self->f_new_name)) == NULL) {
   TPP_CALLBACK_FREE(result->f_name); goto no_mem_1;
  }
 } else result->f_new_name = NULL;
 result->f_refcnt = 1;
 result->f_prev = self->f_prev;
 TPPFile_XINCREF(result->f_prev);
 result->f_ref_file = self->f_ref_file;
 TPPFile_XINCREF(result->f_ref_file);
 result->f_ref_file_off = self->f_ref_file_off;
 result->f_line_offset = self->f_line_offset;
 result->f_iter = result->f_data+(self->f_iter-self->f_data);
 data_size = (size_t)(self->f_end-self->f_data);
 result->f_end = result->f_data+data_size;
 memcpy(result->f_data,self->f_data,data_size);
 *(char *)result->f_end = 0;
 return result;
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_Resize(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self, size_t n)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *result;
 TPP_ASSERT(self);
 TPP_ASSERT(self->f_refcnt == 1 && "Can't resize shared file (you must copy it first)");
 result = TPP_REINTERPRET_CAST(TPPFileObject *,TPP_CALLBACK_REALLOC_NNZ(
  self,offsetof(TPPFileObject,f_data)+((n+1)*sizeof(char))));
 if (TPP_LIKELY(result)) {
  *TPP_CONST_CAST(char *,result->f_end = (
   result->f_iter = result->f_data)+n) = '\0'; // Terminating\0 char
 } else TPP_CALLBACK_NO_MEMORY();
 return result;
}
#define TPPFile_NEW_FROM_STRING(s) TPPFile_NewFromString(s,(sizeof(s)/sizeof(char))-1)
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringEx(
 TPPFileFormatFlag flags, char const *s, size_t n) {
 TPPFileObject *result;
 TPP_ASSERT(s);
 if (TPP_LIKELY((result = TPPFile_New(n)) != NULL)) {
  memcpy(result->f_data,s,n);
  if (flags) _TPPFile_Format(result,flags);
 }
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringExf(
 TPPFileFormatFlag flags, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, ...)
 TPP_ATTRIBUTE_GCC_SOURCE_PRINTF_STRING(2,3) {
 TPPFileObject *result;
 va_list args;
 va_start(args,fmt);
 result = TPPFile_NewFromVStringExf(flags,fmt,args);
 va_end(args);
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromVStringExf(
 TPPFileFormatFlag flags, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, va_list args) {
 enum{bufsize=128};
 TPPFileObject *result;
 int len;
#ifdef _MSC_VER
 size_t size = bufsize;
 result = TPPFile_New(bufsize);
 while (result != NULL) {
  TPP_ASSERT(TPPFile_SIZE(result) == size);
  len = _vsnprintf(result->f_data,size+1,fmt,args);
  if (len < 0) {
   result = TPPFile_Resize(result,(size *= 2));
  } else {
   result->f_end = result->f_data+len;
   break;
  }
 }
#else
 result = TPPFile_New(bufsize);
 if (result) {
  len = vsnprintf(result->f_data,bufsize+1,fmt,args);
  if (len > bufsize) {
   result = TPPFile_Resize(result,len);
   vsprintf(result->f_data,fmt,args);
  } else {
   TPP_ASSERT(len >= 0 && "Invalid printf format string");
   result->f_end = result->f_data+len;
  }
 }
#endif
 if (flags) _TPPFile_Format(result,flags);
 return result;
}
TPP_DECL_STATIC_INLINE(size_t) _TPPFile_GetQuotedCharLen(TPPInteger chr) {
 switch (chr) {
  case '\x07': // '\\' 'a'
  case '\x08': // '\\' 'b'
  case '\x09': // '\\' 't'
  case '\x0A': // '\\' 'n'
  case '\x0B': // '\\' 'v'
  case '\x0C': // '\\' 'c'
  case '\x0D': // '\\' 'r'
  case '\x1B': // '\\' 'e'
  case '\\':   // '\\' '\\'
  case '\"':   // '\\' '\"'
  case '\'':   // '\\' '\''
  case '\x7F': // '\\' 'd'
  case '\0':   // '\\' '0'
  case '\1':   // '\\' '1'
  case '\2':   // '\\' '2'
  case '\3':   // '\\' '3'
  case '\4':   // '\\' '4'
  case '\5':   // '\\' '5'
  case '\6':   // '\\' '6'
   return TPP_STATIC_CAST(size_t,2);
   break;
  case '\x0E': // '\\' 'x' 'E'
  case '\x0F': // '\\' 'x' 'F'
   return TPP_STATIC_CAST(size_t,3);
   break;
  case '\x10': case '\x11': case '\x12': case '\x13':
  case '\x14': case '\x15': case '\x16': case '\x17':
  case '\x18': case '\x19': case '\x1A': case '\x1C':
  case '\x1D': case '\x1E': case '\x1F': case '\x80':
  case '\x81': case '\x82': case '\x83': case '\x84':
  case '\x85': case '\x86': case '\x87': case '\x88':
  case '\x89': case '\x8A': case '\x8B': case '\x8C':
   return TPP_STATIC_CAST(size_t,4); // '\\' 'x' '?' '?'
   break;
  default:
   if (chr > 0xFFFFFFFFFFFFFFFull) return 2+16;
   if (chr > 0xFFFFFFFFFFFFFFull) return 2+15;
   if (chr > 0xFFFFFFFFFFFFFull) return 2+14;
   if (chr > 0xFFFFFFFFFFFFull) return 2+13;
   if (chr > 0xFFFFFFFFFFFull) return 2+12;
   if (chr > 0xFFFFFFFFFFull) return 2+11;
   if (chr > 0xFFFFFFFFFull) return 2+10;
   if (chr > 0xFFFFFFFFull) return 2+9;
   if (chr > 0xFFFFFFFull) return 2+8;
   if (chr > 0xFFFFFFull) return 2+7;
   if (chr > 0xFFFFFull) return 2+6;
   if (chr > 0xFFFFull) return 2+5;
   if (chr > 0xFFFull) return 2+4;
   if (chr > 0xFFull) return 2+3;
   return 1;
 }
}
TPP_DECL_STATIC_INLINE(size_t) _TPPFile_GetQuotedStringLen(char const *begin, char const *end) {
 size_t result = 2; // first + last '"'
 char ch;
 while (begin != end) {
  switch ((ch = *begin++)) {
   case '\x07': // '\\' 'a'
   case '\x08': // '\\' 'b'
   case '\x09': // '\\' 't'
   case '\x0A': // '\\' 'n'
   case '\x0B': // '\\' 'v'
   case '\x0C': // '\\' 'c'
   case '\x0D': // '\\' 'r'
   case '\x1B': // '\\' 'e'
   case '\\':   // '\\' '\\'
   case '\"':   // '\\' '\"'
   case '\'':   // '\\' '\''
   case '\x7F': // '\\' 'd'
   case '\0':   // '\\' '0'
   case '\1':   // '\\' '1'
   case '\2':   // '\\' '2'
   case '\3':   // '\\' '3'
   case '\4':   // '\\' '4'
   case '\5':   // '\\' '5'
   case '\6':   // '\\' '6'
    result += TPP_STATIC_CAST(size_t,2);
    break;
   case '\x0E': // '\\' 'x' 'E'
   case '\x0F': // '\\' 'x' 'F'
    result += TPP_STATIC_CAST(size_t,3);
    break;
   case '\x10': case '\x11': case '\x12': case '\x13':
   case '\x14': case '\x15': case '\x16': case '\x17':
   case '\x18': case '\x19': case '\x1A': case '\x1C':
   case '\x1D': case '\x1E': case '\x1F': case '\x80':
   case '\x81': case '\x82': case '\x83': case '\x84':
   case '\x85': case '\x86': case '\x87': case '\x88':
   case '\x89': case '\x8A': case '\x8B': case '\x8C':
    result += TPP_STATIC_CAST(size_t,4); // '\\' 'x' '?' '?'
    break;
   default: 
    ++result;
    break;
  }
 }
 return result;
}
TPP_DECL_STATIC_INLINE(char *) _TPPFile_WriteQuotedString(char *dst, char const *src_begin, char const *src_end) {
#define WRITE(ch) *dst++ = (ch)
 WRITE('\"');
 while (src_begin != src_end) {
  char ch = *src_begin++;
  switch (ch) {
   case '\x07': ch = 'a';   goto ch1;
   case '\x08': ch = 'b';   goto ch1;
   case '\x09': ch = 't';   goto ch1;
   case '\x0A': ch = 'n';   goto ch1;
   case '\x0B': ch = 'v';   goto ch1;
   case '\x0C': ch = 'c';   goto ch1;
   case '\x0D': ch = 'r';   goto ch1;
   case '\x1B': ch = 'e';   goto ch1;
   case '\\': /*ch = '\\';  goto ch1;*/
   case '\"': /*ch = '\"';  goto ch1;*/
   case '\'': /*ch = '\'';*/goto ch1;
   case '\x7F': ch = 'd';   goto ch1;
   case '\0': case '\1': case '\2':
   case '\3': case '\4': case '\5':
   case '\6': ch += ('0'-'\0');
ch1:
    WRITE('\\');
    WRITE(ch);
    break;
#if TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X && TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX
   case '\x0E': WRITE('\\'); WRITE('X'); WRITE('E'); break;
   case '\x0F': WRITE('\\'); WRITE('X'); WRITE('F'); break;
#elif TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X
   case '\x0E': WRITE('\\'); WRITE('X'); WRITE('e'); break;
   case '\x0F': WRITE('\\'); WRITE('X'); WRITE('f'); break;
#elif TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX
   case '\x0E': WRITE('\\'); WRITE('x'); WRITE('E'); break;
   case '\x0F': WRITE('\\'); WRITE('x'); WRITE('F'); break;
#else
   case '\x0E': WRITE('\\'); WRITE('x'); WRITE('e'); break;
   case '\x0F': WRITE('\\'); WRITE('x'); WRITE('f'); break;
#endif
   case '\x10': case '\x11': case '\x12': case '\x13':
   case '\x14': case '\x15': case '\x16': case '\x17':
   case '\x18': case '\x19': case '\x1A': case '\x1C':
   case '\x1D': case '\x1E': case '\x1F': case '\x80':
   case '\x81': case '\x82': case '\x83': case '\x84':
   case '\x85': case '\x86': case '\x87': case '\x88':
   case '\x89': case '\x8A': case '\x8B': case '\x8C': {
    char temp;
    WRITE('\\');
#if TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X
    WRITE('X');
#else /* TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X */
    WRITE('x');
#endif /* !TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X */
    temp = (ch >> 4) & '\x0F';
#if TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX
    if (temp <= 9) WRITE('0'+temp); else WRITE('A'+temp);
    temp = ch & '\x0F';
    if (temp <= 9) WRITE('0'+temp); else WRITE('A'+temp);
#else /* TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX */
    if (temp <= 9) WRITE('0'+temp); else WRITE('a'+temp);
    temp = ch & '\x0F';
    if (temp <= 9) WRITE('0'+temp); else WRITE('a'+temp);
#endif /* !TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX */
   } break;
   default: 
    WRITE(ch);
    break;
  }
 }
 WRITE('\"');
#undef WRITE
 return dst;
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *)
TPPFile_NewFromQuotedStringL(char const *s, size_t s_len) {
 TPPFileObject *result = TPPFile_New(_TPPFile_GetQuotedStringLen(s,s+s_len));
 if (TPP_LIKELY(result)) _TPPFile_WriteQuotedString(result->f_data,s,s+s_len);
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 size_t s = TPPToken_EvalStr(token_,NULL,0);
 TPPFileObject *result = TPPFile_New(s);
 if (TPP_LIKELY(result)) {
  TPPToken_EvalStr(token_,result->f_data,TPPFile_SIZE(result));
 }
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_AppendStringToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject **self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 size_t old_size,s;
 TPPFileObject *new_self;
 TPP_ASSERT(self);
 TPP_ASSERT(*self);
 TPP_ASSERT(token_);
 s = TPPToken_EvalStr(token_,NULL,0);
 old_size = TPPFile_SIZE(*self);
 new_self = TPPFile_Resize(*self,old_size+s);
 if (TPP_UNLIKELY(new_self == NULL)) return -1;
 *self = new_self;
 TPPToken_EvalStr(token_,new_self->f_data+old_size,s);
 return 0;
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromFILEEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL FILE *fp,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPFileFormatFlag flags)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *result;
 long fp_size;
 if (TPP_UNLIKELY(fseek(fp,0,SEEK_END) != 0)) goto err0;
 if (TPP_UNLIKELY((fp_size = ftell(fp)) == -1)) goto err0;
 if (TPP_UNLIKELY(fseek(fp,0,SEEK_SET) != 0)) goto err0;
 if (TPP_UNLIKELY((result = TPPFile_New(TPP_STATIC_CAST(size_t,fp_size))) == NULL)) goto err0;
 if (TPP_UNLIKELY(_TPPFile_SetName(result,filename) == -1)) goto err1;
 *TPP_CONST_CAST(char *,result->f_end = result->f_data+fread(
  result->f_data,sizeof(char),TPP_STATIC_CAST(size_t,fp_size),fp)) = '\0';
 //if (fread(result->f_data,sizeof(char),fp_size,fp) != fp_size/sizeof(char)) goto err1;
 if (TPP_LIKELY(flags)) _TPPFile_Format(result,flags);
 return result;
err1:
 _TPPFile_Free(result);
err0:
 return NULL;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromFileEx(
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPFileFormatFlag flags) {
 TPPFileObject *result;
 FILE *fp = fopen(filename,"r");
 if (TPP_UNLIKELY(!fp)) return NULL;
 result = TPPFile_NewFromFILEEx(fp,filename,flags);
 fclose(fp);
 return result;
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) _TPPFile_SetName(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPP_CALLBACK_FREE(self->f_name);
 if (s && *s) {
  if (TPP_UNLIKELY((self->f_name = TPP_CALLBACK_STRDUP_NZ(s)) == NULL))
   return -1;
 } else self->f_name = NULL;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_SetNewName(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPP_CALLBACK_FREE(self->f_new_name);
 if (s && *s) {
  if (TPP_UNLIKELY((self->f_new_name = TPP_CALLBACK_STRDUP_NZ(s)) == NULL))
   return -1;
 } else self->f_new_name = NULL;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_IsMacroFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPFile_IS_MACRO_FILE(self);
}
TPP_IMPL_FUNC(void) TPPFile_Rewind(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPPFile_REWIND(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN size_t) TPPFile_Size(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPFile_SIZE(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN size_t) TPPFile_IterOff(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPFile_ITER_OFF(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN char const *) TPPFile_Name(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPFile_NAME(self);
}

TPP_IMPL_FUNC(char) TPPFile_Read(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char result;
 TPP_ASSERT(self);
 TPP_ASSERT(self->f_iter >= self->f_data && self->f_iter <= self->f_end);
again:
 result = *TPP_VALIDATE(self->f_iter);
 if (TPP_LIKELY(self->f_iter != self->f_end)) {
  ++self->f_iter;
  if (TPP_UNLIKELY(result == '\\')) {
   // wrap lines
   if (*self->f_iter == '\n') {
    ++self->f_iter;
    goto again;
   } else if (*self->f_iter == '\r') {
    if (*++self->f_iter == '\n') ++self->f_iter;
    goto again;
   }
  }
 }
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char) TPPFile_Peek(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char const *ch_p;
 char result;
 TPP_ASSERT(self);
 ch_p = self->f_iter;
again:
 result = *TPP_VALIDATE(ch_p);
 if (TPP_LIKELY(ch_p != self->f_end)) {
  ++ch_p;
  if (TPP_UNLIKELY(result == '\\')) {
   // wrap lines
   if (*ch_p == '\n') {
    ++ch_p;
    goto again;
   } else if (*ch_p == '\r') {
    if (*++ch_p == '\n') ++ch_p;
    goto again;
   }
  }
 }
 return result;
}
TPP_IMPL_FUNC(void) TPPFile_Undo(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char ch;
 TPP_ASSERT(self);
 TPP_ASSERT(self->f_iter != self->f_data);
again:
 ch = *--self->f_iter;
 if (self->f_iter != self->f_data) {
  if (TPP_UNLIKELY(ch == '\n')) {
   if (self->f_iter[-1] == '\r' &&
       --self->f_iter == self->f_data) return;
   if (self->f_iter[-1] == '\\') {
    --self->f_iter;
    goto again;
   } else if (self->f_iter[-1] == '\\') {
    --self->f_iter;
    goto again;
   }
  } else if (TPP_UNLIKELY(ch == '\r' && self->f_iter[-1] == '\\')) {
   --self->f_iter;
   goto again;
  }
 }
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPFile_LineAt(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self, size_t offset)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 int result;
 char const *iter;
 char const *end;
 TPP_ASSERT(self);
 result = 0;
 end = (iter = self->f_data)+offset;
 if (TPP_LIKELY(iter != end)) {
  while (1) {
   if (TPP_UNLIKELY(*iter == '\r')) {
    ++result;
    if (TPP_UNLIKELY(++iter == end)) break;
    if (*iter == '\n') if (TPP_UNLIKELY(++iter == end)) break;
   } else {
    if (TPP_UNLIKELY(*iter == '\n')) ++result;
    if (TPP_UNLIKELY(++iter == end)) break;
   }
  }
 }
 //if (TPPFile_IS_MACRO_FILE(self))
 // result += TPPFile_LineAt(self->f_ref_file,self->f_ref_file_off);
 return result+self->f_line_offset;
}



TPP_IMPL_FUNC(void) TPPToken_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPPFile_XDECREF(self->tk_file);
 TPPFile_XDECREF(self->tk_str_file);
}
TPP_IMPL_FUNC(void) TPPToken_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self);
 TPP_ASSERT(right);
 self->tk_id = right->tk_id;
 if (TPP_LIKELY((self->tk_file = right->tk_file) != NULL))
  TPPFile_INCREF(self->tk_file);
 self->tk_file_off = right->tk_file_off;
 if (TPP_LIKELY((self->tk_str_file = right->tk_str_file) != NULL))
  TPPFile_INCREF(self->tk_str_file);
 self->tk_str_begin = right->tk_str_begin;
 self->tk_str_end = right->tk_str_end;
}
TPP_IMPL_FUNC(void) TPPToken_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self);
 TPP_ASSERT(right);
 self->tk_id = right->tk_id;
 self->tk_file = right->tk_file;
 self->tk_file_off = right->tk_file_off;
 self->tk_str_file = right->tk_str_file;
 self->tk_str_begin = right->tk_str_begin;
 self->tk_str_end = right->tk_str_end;
 right->tk_id = 0;
 right->tk_file = NULL;
 right->tk_file_off = 0;
 right->tk_str_file = NULL;
 self->tk_str_begin = NULL;
 self->tk_str_end = NULL;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPToken_AtStartOfLine(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char ch;
 char const *iter,*end;
 TPP_ASSERT(self);
 iter = TPP_VALIDATE(self->tk_str_begin);
 end = TPP_VALIDATE(self->tk_str_file)->f_data;
 TPP_ASSERT(iter >= end);
 while (iter != end) {
  ch = *--iter;
  if (ch == '\n' || ch == '\r') {
   if (ch == '\n' && iter != end && iter[-1] == '\r') --iter;
   if (iter == end || iter[-1] != '\\')
    break; // jump to return 1;
   --iter; // skip "\\\n"
  } else if (!TPP_CH_IS_WHSPC(ch)) {
   return 0;
  }
 }
 return 1;
}

TPP_IMPL_FUNC(size_t) TPPToken_Str(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_BUFFER_VAR(n,return) char *s, size_t n)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 size_t result;
 char ch;
 char const *iter,*end;
 TPP_ASSERT(self);
 TPP_ASSERT(n != 0 ? s != NULL : 1);
 if (TPP_UNLIKELY((iter = self->tk_str_begin) == NULL)) return 0;
 end = self->tk_str_end;
 result = 0;
#define WRITE(ch) (n ? TPP_VOID_CAST((*s++ = (ch),--n)) : TPP_VOID_CAST(0),TPP_VOID_CAST(++result))
 while (iter != end) {
next:
  switch ((ch = *iter++)) {
   case '\0': break;
   case '\\':
    if (TPP_LIKELY(iter != end)) {
     if (*iter == '\n') {
      ++iter;
      goto next;
     } else if (*iter == '\r') {
      if (*++iter == '\n') ++iter;
      goto next;
     }
    }
    // fallthrough
   default:
    WRITE(ch);
    break;
  }
 }
#undef WRITE
 return result;
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPToken_SStr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 static char buffer[1024];
 size_t end = TPPToken_Str(self,buffer,sizeof(buffer));
 if (TPP_UNLIKELY(end >= sizeof(buffer))) end = sizeof(buffer)-1;
 buffer[end] = '\0';
 return buffer;
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPToken_EvalInt(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPUInteger *i, TPPIntegerType *type)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 char ch;
 char const *iter,*end;
 TPPIntegerType _type;
 TPPUInteger result,new_result;
 int numsys,more,overflow = 0;
 TPP_ASSERT(self);
 TPP_ASSERT(TPPTokenID_IS_INT(self->tk_id));
 TPP_ASSERT(i);
 if (TPP_UNLIKELY(self->tk_id == TPP_TOK_CHR)) {
  enum{size=sizeof(TPPUInteger)/sizeof(char)};
  result = 0;
#if TPP_CONFIG_STRICT_INTEGER_OVERFLOW
  if (TPP_UNLIKELY(TPPToken_EvalStr(self,TPP_REINTERPRET_CAST(char *,&result),size) > size))
   return -1;
  switch (TPPToken_GetStringType(self)) {
   case TPPStringType_CHAR32: new_result = (TPP_rt_char)result;   _type = TPPIntegerType_CHAR32; break;
   case TPPStringType_CHAR16: new_result = (TPP_rt_wchar_t)result;  _type = TPPIntegerType_CHAR16; break;
   case TPPStringType_WCHAR:  new_result = (TPP_rt_char16_t)result; _type = TPPIntegerType_WCHAR; break;
   default:                   new_result = (TPP_rt_char32_t)result; _type = TPPIntegerType_CHAR; break;
  }
  if (TPP_UNLIKELY(type)) *type = _type;
  return result > (*i = new_result) ? 1 : 0;
#else /* TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
  if (TPP_UNLIKELY(TPPToken_EvalStr(self,TPP_REINTERPRET_CAST(char *,i),size) > size))
   return -1;
  return 0;
#endif /* !TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
 }
 iter = self->tk_str_begin;
 end = self->tk_str_end;
 TPP_ASSERT(iter != end);
 if (*iter == '0') {
  if (++iter == end) { *i = 0; if (type) *type = TPPIntegerType_INT; return 0; }
  if (*iter == 'x' || *iter == 'X') {
   if (TPP_UNLIKELY(++iter == end)) return -1;
   numsys = 16;
  } else if (*iter == 'b' || *iter == 'B') {
   if (TPP_UNLIKELY(++iter == end)) return -1;
   numsys = 2;
  } else {
   numsys = 8;
  }
 } else {
  numsys = 10;
 }
 result = 0;
 TPP_ASSERT(iter != end);
 do {
next:
  switch ((ch = *iter++)) {
   case '0': case '1': case '2': case '3':
   case '4': case '5': case '6': case '7':
   case '8': case '9': more = ch-'0'; break;
   case 'a': case 'b': case 'c': case 'd':
   case 'e': case 'f': more = 10+ch-'a'; break;
   case 'A': case 'B': case 'C': case 'D':
   case 'E': case 'F': more = 10+ch-'A'; break;
   case '\\':
    if (iter != end) {
     if (*iter == '\n') { ++iter; goto next; }
     if (*iter == '\r') { if (*++iter == '\n') ++iter; goto next; }
    }
    return -1;
   default: _type = (TPPIntegerType)0; goto sfx;
  }
  if (TPP_UNLIKELY(more >= numsys)) return -1;
  new_result = result*numsys+more;
  if (TPP_UNLIKELY(new_result < result)) overflow = 1;
  result = new_result;
 } while (iter != end);
 if (type) *type = TPPIntegerType_INT;
#if TPP_CONFIG_STRICT_INTEGER_OVERFLOW
 *i = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint,result));
#else /* TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
 *i = result;
#endif /* TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
 return overflow;
 while (iter != end) {
sfx_next:
  ch = *iter++;
sfx:
  switch (ch) {
   case '\\':
    if (TPP_LIKELY(iter != end)) {
     if (*iter == '\n') ++iter;
     else if (*iter == '\r') {
      if (*++iter == '\n') ++iter;
     } else return -1;
    } else return -1;
    goto sfx_next;
   case 'u': case 'U':
    _type |= TPPIntegerType_UNSIGNED;
    break;
   case 'l': case 'L':
    if (TPP_UNLIKELY((_type & TPPIntegerType_LONG) != 0)) {
     _type = (_type & ~TPPIntegerType_LONG)|TPPIntegerType_LONG_LONG;
    } else if (TPP_LIKELY((_type & TPPIntegerType_TYPE_MASK) == 0)) {
     _type |= TPPIntegerType_LONG;
    } else return -1;
    break;
#if TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX
   case 'i': case 'I':
    if (TPP_UNLIKELY(iter == end || (_type & TPPIntegerType_TYPE_MASK) != 0))
     return -1;
    switch (*iter++) {
     case '8': _type |= TPPIntegerType_INT8; break;
     case '1': if (TPP_UNLIKELY(iter == end || *iter++ != '6')) return -1; _type |= TPPIntegerType_INT16; break;
     case '3': if (TPP_UNLIKELY(iter == end || *iter++ != '2')) return -1; _type |= TPPIntegerType_INT32; break;
     case '6': if (TPP_UNLIKELY(iter == end || *iter++ != '4')) return -1; _type |= TPPIntegerType_INT64; break;
     default: return -1;
    }
    break;
#endif /* TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX */
   default:
    return -1;
  }
 }
 if (type) *type = _type;
#if TPP_CONFIG_STRICT_INTEGER_OVERFLOW
 switch (_type) {
  case TPPIntegerType_LONG:
  case TPPIntegerType_UNSIGNED_LONG:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_ulong,result)); break;
  case TPPIntegerType_LONG_LONG:
  case TPPIntegerType_UNSIGNED_LONG_LONG:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_ullong,result)); break;
#if TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX
  case TPPIntegerType_INT8:
  case TPPIntegerType_UINT8:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint8_t,result)); break;
  case TPPIntegerType_INT16:
  case TPPIntegerType_UINT16:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint16_t,result)); break;
  case TPPIntegerType_INT32:
  case TPPIntegerType_UINT32:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint32_t,result)); break;
  case TPPIntegerType_INT64:
  case TPPIntegerType_UINT64:
   new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint64_t,result)); break;
#endif /* TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX */
  //case TPPIntegerType_UNSIGNED:
  default: new_result = TPP_STATIC_CAST(TPPUInteger,TPP_STATIC_CAST(TPP_rt_uint,result)); break;
 }
 if (new_result != result) overflow = 1;
 *i = new_result;
#else /* TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
 *i = result;
#endif /* !TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
 return overflow;
}
#define TPP_READ(ch,iter,end)\
while (1) {\
 (ch) = *(iter)++;\
 if ((ch) == '\\' && (iter) != (end)) {\
  if (*(iter) == '\n') {\
   ++(iter);\
  } else if (*(iter) == '\r') {\
   if (*++(iter) == '\n') ++(iter);\
  } else break;\
 } else break;\
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPToken_EvalFloat(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFloat *f, TPPFloatType *type)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPFloat result = 0;
 char ch;
 char const *iter,*end;
 int numsys,more,float_extension_mult;
 TPP_ASSERT(f);
 TPP_ASSERT(self);
 TPP_ASSERT(self->tk_str_begin);
 TPP_ASSERT(self->tk_str_end);
 TPP_ASSERT(self->tk_str_end >= self->tk_str_begin);
 TPP_ASSERT(TPPTokenID_IS_INT(self->tk_id) || self->tk_id == TPP_TOK_FLOAT);
 if (TPPTokenID_IS_INT(self->tk_id)) {
  // Evaluate Integer as Float
  TPPUInteger i;
  TPPIntegerType ty;
  if (TPPToken_EvalInt(self,&i,&ty) == -1) return -1;
  switch (ty) {
   case TPPIntegerType_INT:
   case TPPIntegerType_CHAR: // 123 --> double
    if (type) *type = TPPFloatType_DOUBLE;
    break;
   case TPPIntegerType_LONG: // 123L --> long double
    if (type) *type = TPPFloatType_LDOUBLE;
    break;
   default: return -1;
  }
  *f = TPP_STATIC_CAST(TPPFloat,i);
  return 0;
 }
 iter = self->tk_str_begin;
 end = self->tk_str_end;
 if (*iter == '0') {
  if (++iter == end) { *f = 0.0; if (type) *type = TPPFloatType_DOUBLE; return 0; }
  if (*iter == 'x' || *iter == 'X') {
   if (TPP_UNLIKELY(++iter == end)) return -1;
   numsys = 16;
  } else if (*iter == 'b' || *iter == 'B') {
   if (TPP_UNLIKELY(++iter == end)) return -1;
   numsys = 2;
  } else if (*iter == '.') {
   numsys = 10;
  } else {
   numsys = 8;
  }
 } else {
  numsys = 10;
 }
 float_extension_mult = 0;
 do {
  switch ((ch = *iter++)) {
   case 'p': case 'P': goto flt_ext;
   case '0': case '1': case '2': case '3':
   case '4': case '5': case '6': case '7':
   case '8': case '9': more = ch-'0'; break;
   case 'e': if (numsys < (10+'e'-'a')) goto flt_ext; // fallthrough
   case 'a': case 'b': case 'c': case 'd': more = 10+ch-'a'; break;
   case 'E': if (numsys < (10+'E'-'A')) goto flt_ext; // fallthrough
   case 'A': case 'B': case 'C': case 'D': more = 10+ch-'A'; break;
   case 'f': case 'F': if (numsys < (10+'f'-'a')) goto sfx; more = 0xF; break;
   case '\\':
    if (iter != end) {
     if (*iter == '\n') {
      ++iter;
      goto next;
     } else if (*iter == '\r') {
      if (*iter == '\n') ++iter;
      ++iter;
      goto next;
     }
    }
    return -1;
   case '.': float_extension_mult = numsys; goto next;
   default: --iter; goto sfx;
  }
  if (TPP_UNLIKELY(more >= numsys)) return -1;
  if (float_extension_mult != 0) {
   result += (TPP_STATIC_CAST(TPPFloat,more)/TPP_STATIC_CAST(TPPFloat,float_extension_mult));
   float_extension_mult *= numsys;
  } else {
   result = result*numsys+more;
  }
next:;
 } while (iter != end);
 goto dbl;
flt_ext:
 // Read the Float extension: E[+/-][int]
#define float_extension_pos numsys
#define float_extension_off more
 float_extension_pos = 1;
 float_extension_off = 0;
 if (iter == end) return -1;
 TPP_READ(ch,iter,end);
 if (ch == '-' || ch == '+') {
  float_extension_pos = (ch == '+');
  if (iter == end) return -1;
  TPP_READ(ch,iter,end);
 }
 while (TPP_CH_IS_DIGIT(ch)) {
  float_extension_off = float_extension_off*10+(ch-'0');
  if (iter == end) { ch = '\0'; break; }
  TPP_READ(ch,iter,end);
 }
 float_extension_mult = 1;
 while (float_extension_off != 0) {
  float_extension_mult *= 10;
  --float_extension_off;
 }
 if (float_extension_pos) {
  result *= float_extension_mult;
 } else {
  result /= float_extension_mult;
 }
#undef float_extension_pos
#undef float_extension_off
sfx:
 // Floating pointer type suffix
 switch (ch) {
   // fallthrough
  case '\0':dbl:      if (type) *type = TPPFloatType_DOUBLE; break;
  case 'f': case 'F': if (type) *type = TPPFloatType_FLOAT; break;
  case 'l': case 'L': if (type) *type = TPPFloatType_LDOUBLE; break;
  default: return -1; // Invalid suffix
 }
 *f = result;
 return 0;
}
#undef TPP_READ


union TPPCharRec {
 TPP_rt_char   *s;
 TPP_rt_wchar_t  *L;
 TPP_rt_char16_t *u;
 TPP_rt_char32_t *U;
};
TPP_IMPL_FUNC(size_t) TPPToken_EvalStr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_BUFFER_VAR(n,return) char *s, size_t n)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 size_t result,n_digits,encoding_size;
 char const *iter,*end;
 TPP_rt_char32_t ch,ch2;
 TPPStringType encoding;
 union TPPCharRec str;
 TPP_ASSERT(self);
 TPP_ASSERT(self->tk_id == TPP_TOK_STR || self->tk_id == TPP_TOK_CHR);
 TPP_ASSERT(n != 0 ? s != 0 : 1);
 str.s = (TPP_rt_char *)s;
 if (TPP_UNLIKELY((iter = self->tk_str_begin) == NULL)) return 0;
 encoding = *iter++;
 if (TPP_UNLIKELY(encoding == '\'')) {
  encoding = '\"';
 } else if (TPP_UNLIKELY(encoding != '\"')) {
  TPP_ASSERT(*iter == '\"' || *iter == '\'');
  ++iter;
 }
 switch (encoding) {
  case TPPStringType_WCHAR:  encoding_size = sizeof(TPP_rt_wchar_t); break;
  case TPPStringType_CHAR16: encoding_size = sizeof(TPP_rt_char16_t); break;
  case TPPStringType_CHAR32: encoding_size = sizeof(TPP_rt_char32_t); break;
  default: encoding_size = sizeof(TPP_rt_char); break;
 }
 end = self->tk_str_end-1;
 result = 0;
next:
 while (iter != end) {
  TPP_ASSERT(iter >= self->tk_str_begin && iter <= self->tk_str_end);
  switch ((ch = *iter++)) {
   case '\\':
    switch ((ch = *iter++)) {
     case '0': case '1': case '2': case '3':
     case '4': case '5': case '6': case '7': {
      ch2 = ch-'0';
      // NOTE: The real digit count would be one ore each,
      //       but since the initial digit is already consumed,
      //       we have to adjust for that and expect one less
      switch (encoding_size) {
          case 4:  n_digits = 10; break;
          case 2:  n_digits = 5; break;
default:/*case 1:*/n_digits = 2; break;
      }
      while (n_digits--) {
       ch = *iter++;
       if (TPP_LIKELY(ch >= '0' && ch <= '7')) {
        ch2 = ch2*16+(ch-'0');
       } else if (ch == '\\') {
        if (*iter == '\n') ++iter;
        else if (*iter == '\r') {
         if (*++iter == '\n') ++iter;
        } else --iter;
       } else { --iter; break; }
      }
      ch = ch2;
      goto put;
     } break;
     case 'x': case 'u': case 'U': {
      ch2 = 0;
      n_digits = encoding_size*2;
      while (n_digits--) {
       ch = *iter++;
       if (TPP_LIKELY(ch >= 'a' && ch <= 'f')) {
        ch2 = ch2*16+(10+(ch-'a'));
       } else if (TPP_LIKELY(ch >= 'A' && ch <= 'F')) {
        ch2 = ch2*16+(10+(ch-'A'));
       } else if (TPP_LIKELY(ch >= '0' && ch <= '9')) {
        ch2 = ch2*16+(ch-'0');
       } else if (ch == '\\') {
        if (*iter == '\n') ++iter;
        else if (*iter == '\r') {
         if (*++iter == '\n') ++iter;
        } else --iter;
       } else { --iter; break; }
      }
      ch = ch2;
      goto put;
     } break;
     case 'a': ch = '\a'; goto put;
     case 'b': ch = '\b'; goto put;
     case 'c': ch = '\x0C'; goto put;
     case 'd': ch = '\x7F'; goto put;
#if 1
     case 'e': ch = 27;   goto put;
#endif
     case 'f': ch = '\f'; goto put;
     case 'n': ch = '\n'; goto put;
     case 'r': ch = '\r'; goto put;
     case 't': ch = '\t'; goto put;
     case 'v': ch = '\v'; //goto put; // fallthrough
     case '\'': case '\"': case '\\': goto put;
     case '\r':
      if (*iter == '\n') ++iter;
      // fallthrough
     case '\n':
      goto next; // escaped linefeed

     default:
      --iter,ch='\\';
      goto put;
    }
    break;
   default:put: {
    switch (encoding) {
     case TPPStringType_CHAR32: if (TPP_LIKELY(n >= sizeof(TPP_rt_char32_t))) { *str.U++ = TPP_STATIC_CAST(TPP_rt_char32_t,ch); n -= sizeof(TPP_rt_char32_t); } result += sizeof(TPP_rt_char32_t); break;
     case TPPStringType_CHAR16: if (TPP_LIKELY(n >= sizeof(TPP_rt_char16_t))) { *str.u++ = TPP_STATIC_CAST(TPP_rt_char16_t,ch); n -= sizeof(TPP_rt_char16_t); } result += sizeof(TPP_rt_char16_t); break;
     case TPPStringType_WCHAR:  if (TPP_LIKELY(n >= sizeof(TPP_rt_wchar_t)))  { *str.L++ = TPP_STATIC_CAST(TPP_rt_wchar_t,ch);  n -= sizeof(TPP_rt_wchar_t);  } result += sizeof(TPP_rt_wchar_t);  break;
     default:                   if (TPP_LIKELY(n >= sizeof(TPP_rt_char)))     { *str.s++ = TPP_STATIC_CAST(TPP_rt_char,ch);     n -= sizeof(TPP_rt_char);     } result += sizeof(TPP_rt_char);     break;
    }
   } break;
  }
 }
 return result;
}







#if TPP_LOAD_STATIC
static TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_NewS(
#else
TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_NewS(
#endif
 TPPTokenID id, TPPKeywordHash raw_hash, char const *str, size_t str_len) {
 TPPKeywordListHashEntryObject *result;
 TPP_ASSERT(str);
 if (TPP_LIKELY((result = TPP_REINTERPRET_CAST(TPPKeywordListHashEntryObject *,TPP_CALLBACK_MALLOC_NZ(
  offsetof(TPPKeywordListHashEntryObject,he_str)+((str_len+1)*sizeof(char))))) != NULL)) {
  result->he_id = id;
#if TPP_CONFIG_HAVE___TPP_COUNTER
  result->he_counter = 0;
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
  result->he_flags = TPPKeywordFlag_NONE;
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
  result->he_next = NULL;
  result->he_raw_hash = raw_hash;
  result->he_str_len = str_len;
  TPP_REINTERPRET_CAST(char *,memcpy(
   result->he_str,str,str_len*sizeof(char)))[str_len] = '\0';
 } else TPP_CALLBACK_NO_MEMORY();
 return result;
}

TPP_DECL_STATIC_INLINE(TPPKeywordHash) _TPPKeywordList_HashOf(char const *name) {
 TPPKeywordHash result = 1;
 char ch;
 for (ch = *name; ch != '\0'; ch = *++name)
  result = result*263+ch;
 return result;
}
#define _TPPKeywordList_HashOfS(name,name_len) _TPPKeywordList_HashOfL(name,(name)+(name_len))
TPP_DECL_STATIC_INLINE(TPPKeywordHash) _TPPKeywordList_HashOfL(char const *name_begin, char const *name_end) {
 TPPKeywordHash result = 1;
 TPP_ASSERT(name_begin);
 TPP_ASSERT(name_end);
 TPP_ASSERT(name_end >= name_begin);
 for (;name_begin != name_end; ++name_begin) 
  result = result*263+*name_begin;
 return result;
}

#define TPPKeywordList_GET_NEW_HASH_ALLOC(size) \
 (((size)/TPPKeywordList_BUFSIZE)*TPPKeywordList_BUFSIZE+\
  ((size)%TPPKeywordList_BUFSIZE!=0?1:0))
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int)
_TPPKeywordList_Rehash(TPPKeywordListObject *self, size_t new_hash_alloc) {
 TPPKeywordListHashEntryObject 
  **old_map_iter,**old_map_end,**new_map,**new_map_dst,*iter,*next;
 TPP_ASSERT(self);
 TPP_ASSERT(self->kl_hash ? self->kl_hash_alloc != 0 : self->kl_hash_alloc == 0);
 if (TPP_UNLIKELY((new_map = TPP_REINTERPRET_CAST(TPPKeywordListHashEntryObject **,TPP_CALLBACK_CALLOC_NZ(
  new_hash_alloc,sizeof(TPPKeywordListHashEntryObject *)))) == NULL))
  return -1;
 old_map_end = (old_map_iter = self->kl_hash)+self->kl_hash_alloc;
 while (old_map_iter != old_map_end) {
  iter = *old_map_iter++;
  while (iter != NULL) {
   next = iter->he_next;
   new_map_dst = new_map+(iter->he_raw_hash % new_hash_alloc);
   iter->he_next = *new_map_dst;
   *new_map_dst = iter;
   iter = next;
  }
 }
 TPP_CALLBACK_FREE(self->kl_hash);
 self->kl_hash = new_map;
 self->kl_hash_alloc = new_hash_alloc;
 return 0;
}

TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) _TPPKeywordList_SetWithKnownHashS(
 TPPKeywordListObject *self, char const *name, size_t name_len, TPPKeywordHash hash, TPPTokenID id);
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) _TPPKeywordList_SetWithKnownHash(
 TPPKeywordListObject *self, char const *name, TPPKeywordHash hash, TPPTokenID id) {
 TPP_ASSERT(name);
 return _TPPKeywordList_SetWithKnownHashS(self,name,strlen(name),hash,id);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) _TPPKeywordList_SetWithKnownHashL(
 TPPKeywordListObject *self, char const *name_begin, char const *name_end, TPPKeywordHash hash, TPPTokenID id) {
 TPP_ASSERT(name_begin);
 TPP_ASSERT(name_end);
 TPP_ASSERT(name_end >= name_begin);
 return _TPPKeywordList_SetWithKnownHashS(
  self,name_begin,TPP_STATIC_CAST(size_t,name_end-name_begin),hash,id);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) _TPPKeywordList_SetWithKnownHashS(
 TPPKeywordListObject *self, char const *name, size_t name_len, TPPKeywordHash hash, TPPTokenID id) {
 TPPKeywordListHashEntryObject **result_slot,*result;
 size_t new_size,new_buf_size;
 TPP_ASSERT(self);
 TPP_ASSERT(name);
 TPP_ASSERT(TPPTokenID_IS_KEYWORD(id));
 if (TPP_UNLIKELY((result = TPPKeywordListHashEntry_NewS(id,hash,name,name_len)) == NULL))
  return NULL;
 if (TPP_LIKELY(self->kl_hash != 0)) {
  new_size = TPPKeywordList_SIZE(self)+1;
  new_buf_size = TPPKeywordList_GET_NEW_HASH_ALLOC(new_size);
  if (TPP_UNLIKELY(new_buf_size != self->kl_hash_alloc)) {
   if (TPP_UNLIKELY(_TPPKeywordList_Rehash(self,new_buf_size) == -1)) {
    TPPKeywordListHashEntry_Free(result);
    return NULL;
   }
  }
 } else {
  self->kl_hash = TPP_REINTERPRET_CAST(TPPKeywordListHashEntryObject **,TPP_CALLBACK_CALLOC_NZ(
   TPPKeywordList_BUFSIZE,sizeof(TPPKeywordListHashEntryObject *)));
  if (TPP_UNLIKELY(!self->kl_hash)) {
   TPPKeywordListHashEntry_Free(result);
   return NULL;
  }
  self->kl_hash_alloc = TPPKeywordList_BUFSIZE;
 }
 //DEE_LDEBUG("[tpp] New keyword: %d: %.*s\n",id,(int)name_len,name);
 result_slot = self->kl_hash+(hash % self->kl_hash_alloc);
 result->he_next = *result_slot;
 return *result_slot = result;
}
TPP_ATTRIBUTE_USE_RETURN unsigned int TPPKeywordList_Size(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject const *self) {
 TPP_ASSERT(self);
 return TPPKeywordList_SIZE(self);
}

#if TPP_LOAD_STATIC
static
#endif
TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *
TPPKeywordListHashEntry_Copy(TPPKeywordListHashEntryObject const *self) {
 TPPKeywordListHashEntryObject *result;
 TPP_ASSERT(self);
 if ((result = (TPPKeywordListHashEntryObject *)TPP_CALLBACK_MALLOC_NZ(
  offsetof(TPPKeywordListHashEntryObject,he_str)+(self->he_str_len+1)*sizeof(char))) == NULL) {
  TPP_CALLBACK_NO_MEMORY();
  return NULL;
 }
 if (self->he_next) {
  if ((result->he_next = TPPKeywordListHashEntry_Copy(self->he_next)) == NULL) {
   TPP_CALLBACK_FREE_NN(result);
   return NULL;
  }
 } else result->he_next = NULL;
 result->he_id = self->he_id;
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
 result->he_flags = self->he_flags;
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
#if TPP_CONFIG_HAVE___TPP_COUNTER
 result->he_counter = self->he_counter;
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
 result->he_raw_hash = self->he_raw_hash;
 result->he_str_len = self->he_str_len;
 memcpy(result->he_str,self->he_str,(result->he_str_len+1)*sizeof(char));
 return result;
}

#if TPP_LOAD_STATIC
static
#endif
int TPPKeywordList_InitCopy(
 TPPKeywordListObject *self, TPPKeywordListObject const *right) {
 TPPKeywordListHashEntryObject **iter,**end,**src,*new_entry,*next;
 TPP_ASSERT(self && right);
 self->kl_next = right->kl_next;
 if ((self->kl_hash_alloc = right->kl_hash_alloc) != 0) {
  if ((iter = (TPPKeywordListHashEntryObject **)TPP_CALLBACK_MALLOC_NZ(
   self->kl_hash_alloc*sizeof(TPPKeywordListHashEntryObject *))) == NULL) {
   TPP_CALLBACK_NO_MEMORY();
   return -1;
  }
  end = (self->kl_hash = iter)+self->kl_hash_alloc;
  src = right->kl_hash;
  while (iter != end) {
   if (*src) {
    if ((new_entry = TPPKeywordListHashEntry_Copy(*src)) == NULL) {
     while (iter != self->kl_hash) {
      new_entry = *--iter;
      while (new_entry != NULL) {
       next = new_entry->he_next;
       TPPKeywordListHashEntry_Free(new_entry);
       new_entry = next;
      }
     }
     TPP_CALLBACK_FREE_NN(self->kl_hash);
     return -1;
    }
    *iter = new_entry;
   } else *iter = NULL;
   ++iter,++src;
  }
 } else self->kl_hash = NULL;
 return 0;
}

#if TPP_LOAD_STATIC
static
#endif
void TPPKeywordList_InitMove(
 TPPKeywordListObject *self,
 TPPKeywordListObject *right) {
 self->kl_next = right->kl_next;
 self->kl_hash_alloc = right->kl_hash_alloc;
 self->kl_hash = right->kl_hash;
 right->kl_next = TPP_TOK_USER_IDENT_START;
 right->kl_hash_alloc = 0;
 right->kl_hash = NULL;
}


#if TPP_LOAD_STATIC
static
#endif
void TPPKeywordList_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordListHashEntryObject **map_iter,**map_end,*iter,*next;
 TPP_ASSERT(self);
 map_end = (map_iter = self->kl_hash)+self->kl_hash_alloc;
 for (iter = *map_iter; map_iter != map_end; iter = *++map_iter) {
  while (iter != NULL) {
   next = iter->he_next;
   TPPKeywordListHashEntry_Free(iter);
   iter = next;
  }
 }
 TPP_CALLBACK_FREE(self->kl_hash);
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_SetupBuiltin(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
#if 1
#define DEF(name,value) if(TPP_UNLIKELY(\
 TPPKeywordList_SetS(self,TPPKeyword_BuiltinToStr(name),\
 (sizeof(value)/sizeof(char))-1,name)==-1))return-1;
#else
#define DEF(name,value) if(TPP_UNLIKELY(\
 TPPKeywordList_SetS(self,value,\
 (sizeof(value)/sizeof(char))-1,name)==-1))return-1;
#endif
#include "tpp_keywords.inl"
#undef DEF
#define DEF(name,value) /* nothing */
#define DEF_TRAIT(str,flags) if(TPP_UNLIKELY(\
 TPPKeywordList_DefTraitS(self,str,(sizeof(str)/sizeof(char))-1,flags)==-1))return-1;
#include "tpp_keywords.inl"
#undef DEF_TRAIT
#undef DEF
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_Set(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 return _TPPKeywordList_SetWithKnownHash(self,name,_TPPKeywordList_HashOf(name),id) ? 0 : -1;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_SetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 return _TPPKeywordList_SetWithKnownHashS(self,name,name_len,_TPPKeywordList_HashOfS(name,name_len),id) ? 0 : -1;
}
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_DefTraitS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, TPPKeywordFlag flags)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordListHashEntryObject *entry;
 if ((entry = TPPKeywordList_GetEntryS(self,name,name_len,1)) == NULL) return -1;
 entry->he_flags = flags;
 return 0;
}
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_FindEntry(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordListHashEntryObject **map_iter,**map_end,*iter;
 TPP_ASSERT(self);
 map_end = (map_iter = self->kl_hash)+self->kl_hash_alloc;
 for (iter = *map_iter; map_iter != map_end; iter = *++map_iter)
  for (;iter != NULL; iter = iter->he_next)
   if (TPP_UNLIKELY(iter->he_id == id)) return iter;
 return NULL;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPKeywordList_NameOf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject const *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordListHashEntryObject const *entry = 
  TPPKeywordList_FindEntry(TPP_CONST_CAST(TPPKeywordListObject *,self),id);
 return entry ? entry->he_str : NULL;
}

// Returns 0, if the keyword doesn't exist and insert_missing == 0
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPTokenID) TPPKeywordList_GetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, int insert_missing)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordHash hash;
 TPPKeywordListHashEntryObject *slot_iter;
 TPP_ASSERT(self);
 TPP_ASSERT(name);
 hash = _TPPKeywordList_HashOfS(name,name_len);
 if (TPP_LIKELY(self->kl_hash != 0)) {
  TPP_ASSERT(self->kl_hash_alloc != 0);
  slot_iter = self->kl_hash[hash % self->kl_hash_alloc];
  for (; slot_iter != NULL; slot_iter = slot_iter->he_next) {
   if (TPP_UNLIKELY(slot_iter->he_str_len == name_len &&
    memcmp(slot_iter->he_str,name,name_len*sizeof(char)) == 0))
    return slot_iter->he_id; // Found it
  }
 }
 if (insert_missing) {
  TPPTokenID result = self->kl_next;
  if (TPP_UNLIKELY(_TPPKeywordList_SetWithKnownHashS(self,name,name_len,hash,result) == NULL))
   return 0;
  ++self->kl_next; // Reserve the id
  return result;
 }
 return 0; // ID not found
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_GetEntryS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, char const *name, size_t name_len, int insert_missing)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPKeywordHash hash;
 TPPKeywordListHashEntryObject *slot_iter;
 TPP_ASSERT(self);
 TPP_ASSERT(name);
 hash = _TPPKeywordList_HashOfS(name,name_len);
 if (TPP_LIKELY(self->kl_hash != NULL)) {
  TPP_ASSERT(self->kl_hash_alloc != 0);
  slot_iter = self->kl_hash[hash % self->kl_hash_alloc];
  for (; slot_iter != NULL; slot_iter = slot_iter->he_next) {
   if (TPP_UNLIKELY(slot_iter->he_str_len == name_len &&
    memcmp(slot_iter->he_str,name,name_len*sizeof(char)) == 0))
    return slot_iter; // Found it
  }
 }
 if (insert_missing) {
  TPPKeywordListHashEntryObject *result = _TPPKeywordList_SetWithKnownHashS(
   self,name,name_len,hash,self->kl_next);
  if (result) ++self->kl_next; // Reserve the id
  return result;
 }
 return NULL; // ID not found
}









TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPMacroObject *) TPPMacro_NewFunction(size_t n_args) {
 TPPMacroObject *result = TPP_REINTERPRET_CAST(TPPMacroObject *,TPP_CALLBACK_CALLOC_NZ(
  1,offsetof(TPPMacroObject,m_argv)+((n_args == TPP_STATIC_CAST(size_t,-1))
  ? 0 : (n_args*sizeof(struct TPPMacroArgument)))));
 if (TPP_LIKELY(result)) {
  result->m_argc = n_args;
  //result->m_push_prev = NULL; // calloc
  //result->m_code = NULL; // calloc
 }
 return result;
}



TPP_IMPL_FUNC(void) TPPMacro_Free(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self) {
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
 TPPMacroObject *push_prev;
 TPP_ASSERT(self);
 do {
  push_prev = self->m_push_prev;
  TPPFile_XDECREF(self->m_code);
  TPP_CALLBACK_FREE_NN(self);
 } while ((self = push_prev) != NULL);
#else /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 TPP_ASSERT(self);
 TPPFile_XDECREF(self->m_code);
 TPP_CALLBACK_FREE_NN(self);
#endif /* !TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
}

struct TPPMacroObject *TPPMacro_Copy(struct TPPMacroObject const *self) {
 TPPMacroObject *result; size_t argc_size;
 TPP_ASSERT(self);
 if ((argc_size = self->m_argc) == (size_t)-1) argc_size = 0;
 if ((result = (TPPMacroObject *)TPP_CALLBACK_MALLOC_NZ(
  offsetof(TPPMacroObject,m_argv)+argc_size*sizeof(struct TPPMacroArgument))) == NULL) {
  TPP_CALLBACK_NO_MEMORY();
  return NULL;
 }
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
 if (self->m_push_prev) {
  if ((result->m_push_prev = TPPMacro_Copy(self->m_push_prev)) == NULL) {
err_0: TPP_CALLBACK_FREE_NN(result); return NULL;
  }
 } else result->m_push_prev = NULL;
 if (self->m_code) {
  if ((result->m_code = TPPFile_Copy(self->m_code)) == NULL) {
/*err_1:*/ if (self->m_push_prev) TPPMacro_Free(result->m_push_prev); goto err_0;
  }
 } else result->m_code = NULL;
#else /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 TPP_ASSERT(self->m_code);
 if ((result->m_code = TPPFile_Copy(self->m_code)) == NULL) {
/*err_1:*/ TPP_CALLBACK_FREE_NN(result); return NULL;
 }
#endif /* !TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 result->m_code_noarg_size = self->m_code_noarg_size;
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
 result->m_va_args_name = self->m_va_args_name;
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
 result->m_is_va_args = self->m_is_va_args;
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
 result->m_call_conv = self->m_call_conv;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
 result->m_argc = self->m_argc;
 memcpy(result->m_argv,self->m_argv,argc_size*sizeof(struct TPPMacroArgument));
 return result;
}




TPP_DECL_STATIC_INLINE(int) _TPPMacro_FindArgument(
 TPPMacroObject const *self, TPPTokenID name) {
 size_t i;
 TPP_ASSERT(self);
 TPP_ASSERT(TPPMacro_HAS_ARGS(self));
 for (i = 0; i < self->m_argc; ++i)
  if (TPP_UNLIKELY(self->m_argv[i].a_name == name))
   return TPP_STATIC_CAST(int,i);
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
 if (TPP_UNLIKELY(name == self->m_va_args_name))
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
 if (TPP_UNLIKELY(name == KWD___VA_ARGS__ && self->m_is_va_args))
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
  return TPP_STATIC_CAST(int,self->m_argc-1);
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
 return -1;
}

#if TPP_CONFIG_HAVE___VA_ARGS__
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_IsVaArgsFunction(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPMacro_IS_VA_ARGS_FUNCTION(self);
}
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_KeywordIsExpanding(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPMacro_KEYWORD_IS_EXPANDING(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_HasArgs(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPMacro_HAS_ARGS(self);
}

#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPMacroObject *macro_copy;
 TPP_ASSERT(self);
 macro_copy = TPPMacro_NewFunction(self->m_argc);
 if (macro_copy == NULL) return -1;
 if ((macro_copy->m_code = self->m_code) != NULL)
  TPPFile_INCREF(macro_copy->m_code);
 macro_copy->m_code_noarg_size = self->m_code_noarg_size;
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
 macro_copy->m_call_conv = self->m_call_conv;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
 if (TPPMacro_HAS_ARGS(self)) {
  // Copy argument data
  memcpy(macro_copy->m_argv,self->m_argv,self->m_argc*sizeof(struct TPPMacroArgument));
 }
 // Insert the macro in the linked list of macro backups
 macro_copy->m_push_prev = self->m_push_prev;
 self->m_push_prev = macro_copy;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject **self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPMacroObject *backup;
 TPP_ASSERT(self);
 TPP_ASSERT(*self);
 if ((backup = (*self)->m_push_prev) == NULL) return 1; // No backups
 TPP_ASSERT(backup != *self);
 (*self)->m_push_prev = NULL; // Don't TPP_CALLBACK_FREE the backup
 TPPMacro_Free(*self);
 *self = backup;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Defined(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPMacro_DEFINED(self);
}
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */


//////////////////////////////////////////////////////////////////////////
// Inline, as only used once
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPMacro_CalculateArgumentRefs(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *lexer)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 size_t total_size;
 char const *code_iter,*kwd_start;
 char ch; TPPTokenID kwd;
 int is_quote_ref = 0,in_str = 0,argi;
 TPP_ASSERT(self);
 TPP_ASSERT(lexer);
 TPP_ASSERT(self->m_code != NULL);
 TPP_ASSERT(TPPMacro_HAS_ARGS(self));
 total_size = TPPFile_SIZE(self->m_code);
 code_iter = self->m_code->f_data;
 // calloc
 //for (i = 0; i < self->m_argc; ++i) {
 // self->m_argv[i].a_references = 0;
 // self->m_argv[i].a_quote_references = 0;
 //}
 while (1) {
  switch ((ch = *code_iter++)) {
   case '\0': goto end;
#if TPP_CONFIG_HAVE_DIGRAPHS
   case '%': // [%:] --> [#]
    if (*code_iter != ':') {
     is_quote_ref = 0;
     break;
    }
    ++code_iter;
    // Fallthrough
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
   case '#':
    if (TPP_LIKELY(!in_str)) {
#if TPP_CONFIG_HAVE_CHARIZE_OPERATOR
     if (*code_iter == '@') ++code_iter;
#endif
     is_quote_ref = 1;
    }
    break;
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
   case '\r':
    if (*code_iter == '\n') ++code_iter;
    // fallthrough
   case '\n': in_str = 0; break;
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
   case '\\':
    if (in_str) {
     if (*code_iter == '\0') goto end;
     ++code_iter;
    }
    break;
   case '\"': if ((in_str & 0x02) == 0) in_str ^= 0x01; is_quote_ref = 0; break;
   case '\'': if ((in_str & 0x01) == 0) in_str ^= 0x02; is_quote_ref = 0; break;
#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
   case '$':
#endif /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
   case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
   case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
   case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
   case 'y': case 'z':
   case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
   case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
   case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
   case 'Y': case 'Z':
   case '_':ident:
    if (TPP_LIKELY(!in_str)) {
     kwd_start = code_iter-1;
     for (ch = *code_iter; TPP_LIKELY(TPP_CH_IS_ALNUM(ch)); ch = *++code_iter);
     kwd = TPPKeywordList_GetL(&lexer->l_keywords,kwd_start,code_iter,1);
     if (kwd == 0) return -1;
     argi = _TPPMacro_FindArgument(self,kwd);
     if (TPP_UNLIKELY(argi != -1)) {
      // Increment the reference counter
      if (TPP_UNLIKELY(is_quote_ref)) {
       ++self->m_argv[argi].a_quote_references;
      } else {
       ++self->m_argv[argi].a_references;
      }
      total_size -= (code_iter-kwd_start); // Remove the space used from the total size
     }
     is_quote_ref = 0;
    }
    break;
   default:
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
    if (TPP_UNLIKELY(TPP_CH_IS_ALPHA_ANSI(ch))) goto ident;
#endif /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
    if (TPP_UNLIKELY(is_quote_ref && !TPP_CH_IS_WHSPC(ch)))
     is_quote_ref = 0;
    break;
  }
 }
end:
 self->m_code_noarg_size = total_size;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_CalculateArgumentRefs(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *lexer)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 return _TPPMacro_CalculateArgumentRefs(self,lexer);
}


TPP_IMPL_FUNC(void) TPPMacroList_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 _TPPMacroList_Init(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPMacroObject **iter,**end,**src,*mac1;
 if ((self->ml_size = right->ml_size) != 0) {
  if ((iter = (TPPMacroObject **)TPP_CALLBACK_MALLOC_NZ(
   self->ml_size*sizeof(TPPMacroObject *))) == NULL) {
   TPP_CALLBACK_NO_MEMORY();
   return -1;
  }
  end = (self->ml_macros = iter)+self->ml_size;
  src = right->ml_macros;
  while (iter != end) {
   if (*src) {
    if ((mac1 = TPPMacro_Copy(*src)) == NULL) {
     while (iter != self->ml_macros) {
      if (*--iter) TPPMacro_Free(*iter);
     }
     TPP_CALLBACK_FREE_NN(self->ml_macros);
     return -1;
    }
    *iter = mac1; // inherit object
   } else *iter = NULL;
   ++iter,++src;
  }
 } else self->ml_macros = NULL;
 return 0;
}
TPP_IMPL_FUNC(void) TPPMacroList_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 self->ml_size = right->ml_size;
 self->ml_macros = right->ml_macros;
 right->ml_size = 0;
 right->ml_macros = NULL;
}
TPP_IMPL_FUNC(void) TPPMacroList_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPMacroObject **list_iter,**list_end;
 TPP_ASSERT(self);
 TPP_ASSERT((self->ml_size != 0) == (self->ml_macros != NULL));
 list_end = (list_iter = self->ml_macros)+self->ml_size;
 for (; list_iter != list_end; ++list_iter)
  if (TPP_UNLIKELY(*list_iter))
   TPPMacro_Free(*list_iter);
 TPP_CALLBACK_FREE(self->ml_macros);
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_Add(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *macro)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 TPPMacroObject **new_macros,*old_macro;
 TPPTokenID new_size;
 TPP_ASSERT(self);
 TPP_ASSERT(macro);
 TPP_ASSERT(TPPTokenID_IS_KEYWORD(name));
 name -= TPP_TOK_IDENT_START;
 if (TPP_UNLIKELY(name >= self->ml_size)) {
  if (TPP_UNLIKELY((new_macros = TPP_REINTERPRET_CAST(TPPMacroObject **,TPP_CALLBACK_REALLOC_NZ(
   self->ml_macros,(new_size = name+1)*sizeof(TPPMacroObject *)))) == NULL)) { TPP_CALLBACK_NO_MEMORY(); return -1; }
  memset(new_macros+self->ml_size,0,(new_size-self->ml_size)*sizeof(TPPMacroObject *));
  self->ml_macros = new_macros;
  self->ml_size = new_size;
 } else if (TPP_UNLIKELY((old_macro = self->ml_macros[name]) != NULL)) {
  int result = 1;
  TPP_ASSERT(old_macro != macro);
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
  if (old_macro->m_push_prev != NULL) {
   // Inherit macro push stack from old macro
   macro->m_push_prev = old_macro->m_push_prev;
   old_macro->m_push_prev = NULL;
   if (!TPPMacro_DEFINED(old_macro)) result = 0;
  }
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
  if (
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
   result == 1 && (old_macro->m_code == NULL ||
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
   _TPPFile_CmpEq(old_macro->m_code,macro->m_code)
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
   )
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
   ) result = 0; // Redefine as the same macro (no warning)
  TPPMacro_Free(old_macro);
  self->ml_macros[name] = macro;
  return result;
 }
 self->ml_macros[name] = macro;
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_IsBuiltinMacro(TPPTokenID name) {
 switch (name) {
#define TPP_MACRO_DEF(name,value) case name:
#include "tpp_keywords.inl"
#undef TPP_MACRO_DEF
   return 1;
  default: break;
 }
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_Defined(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 if (TPP_UNLIKELY(TPPMacroList_IsBuiltinMacro(name))) return 1;
 if (TPP_UNLIKELY(name < TPP_TOK_IDENT_START)) return 0; // Shouldn't really happen...
 name -= TPP_TOK_IDENT_START;
 if (TPP_UNLIKELY(name >= self->ml_size)) return 0;
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
 return self->ml_macros[name] != NULL && self->ml_macros[name]->m_code != NULL;
#else /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 return self->ml_macros[name] != NULL;
#endif /* !TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
}
TPP_IMPL_FUNC(int) TPPMacroList_Undef(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPMacroObject *macro;
 TPP_ASSERT(self);
 TPP_ASSERT(TPPTokenID_IS_KEYWORD(name));
 if (TPP_UNLIKELY(TPPMacroList_IsBuiltinMacro(name))) return -2;
 name -= TPP_TOK_IDENT_START;
 if (TPP_UNLIKELY(name >= self->ml_size)) return -1;
 if (TPP_UNLIKELY((macro = self->ml_macros[name]) == NULL)) return -1;
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
 if (macro->m_push_prev != NULL) {
  TPPFile_XDECREF(macro->m_code);
  macro->m_code = NULL; // Not defined, but has a backup
 } else 
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 {
  TPPMacro_Free(macro);
  self->ml_macros[name] = NULL;
 }
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPMacroObject *) TPPMacroList_Lookup(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 return TPPMacroList_LOOKUP(self,name);
}

#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_PushMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPMacroObject *macro;
 TPP_ASSERT(self);
 TPP_ASSERT(TPPTokenID_IS_KEYWORD(name));
 name -= TPP_TOK_IDENT_START;
 if (name >= self->ml_size || ((macro = self->ml_macros[name]) == NULL)) {
  // Create an empty stub macro (We don't set m_code, because we don't really define it)
  if ((macro = TPPMacro_New()) == NULL) return -1;
  //if ((macro->m_code = TPPFile_New(0)) == NULL) goto err;
  if (TPPMacro_Push(macro) == -1) goto err;
  if (TPPMacroList_Add(self,name+TPP_TOK_IDENT_START,macro) == -1) {
err:
   TPPMacro_Free(macro);
   return -1;
  }
  return 0;
 } else {
  TPP_ASSERT(macro != NULL);
  return TPPMacro_Push(macro);
 }
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_PopMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPP_ASSERT(TPPTokenID_IS_KEYWORD(name));
 name -= TPP_TOK_IDENT_START;
 if (name >= self->ml_size || self->ml_macros[name] == NULL) {
  return 1; // Macro not defined
 } else {
  return TPPMacro_Pop(self->ml_macros+name);
 }
}
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */





TPP_IMPL_FUNC(void) TPPIncludeList_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 _TPPIncludeList_Init(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 char **iter,**end,**src,*path_copy;
 if ((self->il_size = right->il_size) != 0) {
  if ((iter = (char **)TPP_CALLBACK_MALLOC_NZ(
   self->il_size*sizeof(char *))) == NULL) {
   TPP_CALLBACK_NO_MEMORY();
   return -1;
  }
  end = (self->il_paths = iter)+self->il_size;
  src = right->il_paths;
  while (iter != end) {
   if ((path_copy = TPP_CALLBACK_STRDUP_NZ(*src++)) == NULL) {
    while (iter != self->il_paths) TPP_CALLBACK_FREE_NN(*--iter);
    TPP_CALLBACK_FREE_NN(self->il_paths);
    TPP_CALLBACK_NO_MEMORY();
    return -1;
   }
   *iter++ = path_copy;
  }
 } else self->il_paths = NULL;
 return 0;
}
TPP_IMPL_FUNC(void) TPPIncludeList_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 self->il_size = right->il_size;
 self->il_paths = right->il_paths;
 right->il_size = 0;
 right->il_paths = NULL;
}

TPP_IMPL_FUNC(void) TPPIncludeList_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char **iter,**end;
 TPP_ASSERT(self);
 end = (iter = self->il_paths)+self->il_size;
 while (iter != end) TPP_CALLBACK_FREE(*iter++);
 TPP_CALLBACK_FREE(self->il_paths);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_HasS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self, char const *path, size_t path_len)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char **iter,**end;
 TPP_ASSERT(self);
 end = (iter = self->il_paths)+self->il_size;
 while (iter != end) {
  if (strlen(*iter) == path_len && memcmp(*iter,path,path_len) == 0) return 1;
  ++iter;
 }
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_AddS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self, char const *path, size_t path_len)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char **new_paths,*new_path;
 size_t index;
 TPP_ASSERT(self);
 TPP_ASSERT(path);
 index = self->il_size;
 while (path_len > 0 && (
  path[path_len-1] == '/'
#if 1 || defined(TPP_PLATFORM_WINDOWS)
  || path[path_len-1] == '\\'
#endif
  )) --path_len;
 if (TPP_UNLIKELY(path_len == 0)) return 0; // Ignore empty paths
 new_path = TPP_REINTERPRET_CAST(char *,TPP_CALLBACK_MALLOC_NZ((path_len+1)*sizeof(char)));
 if (TPP_UNLIKELY(new_path == NULL)) {no_mem: TPP_CALLBACK_NO_MEMORY(); return -1; }
 new_paths = TPP_REINTERPRET_CAST(char **,TPP_CALLBACK_REALLOC_NZ(self->il_paths,(index+1)*sizeof(char *)));
 if (TPP_UNLIKELY(new_paths == NULL)) { TPP_CALLBACK_FREE_NN(new_path); goto no_mem; }
 ++self->il_size;
 TPP_REINTERPRET_CAST(char *,memcpy(
  new_path,path,path_len*sizeof(char)))[path_len] = '\0';
 (self->il_paths = new_paths)[index] = new_path;
 return 0;
}


TPP_IMPL_FUNC(void) TPPIfdefStack_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 _TPPIfdefStack_Init(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPIfdefStackEntryObject *iter,*end,*src;
 TPP_ASSERT(self && right);
 if ((self->s_size = right->s_size) != 0) {
  while ((iter = (TPPIfdefStackEntryObject *)TPP_CALLBACK_MALLOC_NZ(
   self->s_size*sizeof(TPPIfdefStackEntryObject))) == NULL) {
   TPP_CALLBACK_NO_MEMORY();
   return -1;
  }
  end = (self->s_stack = iter)+self->s_size;
  src = right->s_stack;
  while (iter != end) {
   iter->e_value = src->e_value;
   TPPToken_InitCopy(&iter->e_token,&src->e_token);
   ++iter,++src;
  }
 } else self->s_stack = NULL;
 self->s_alloc = self->s_size;
 return 0;
}
TPP_IMPL_FUNC(void) TPPIfdefStack_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self && right);
 self->s_size = right->s_size;
 self->s_alloc = right->s_alloc;
 self->s_stack = right->s_stack;
 right->s_size = 0;
 right->s_alloc = 0;
 right->s_stack = NULL;
}

TPP_IMPL_FUNC(void) TPPIfdefStack_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPIfdefStackEntryObject *iter,*end;
 TPP_ASSERT(self);
 end = (iter = self->s_stack)+self->s_size;
 for (; iter != end; ++iter) TPPIfdefStackEntry_Quit(iter);
 TPP_CALLBACK_FREE(self->s_stack);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self, int v,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 TPPIfdefStackEntryObject *entry,*new_stack;
 TPP_ASSERT(self);
 if (TPP_UNLIKELY(self->s_size == self->s_alloc)) {
  if ((new_stack = TPP_REINTERPRET_CAST(TPPIfdefStackEntryObject *,TPP_CALLBACK_REALLOC_NZ(
   self->s_stack,(++self->s_alloc)*sizeof(TPPIfdefStackEntryObject)))) == NULL
   ) { TPP_CALLBACK_NO_MEMORY(); return -1; }
  self->s_stack = new_stack;
 }
 entry = self->s_stack+(self->s_size++);
 TPPIfdefStackEntry_Init(entry,v,token_);
 return 0;
}
TPP_IMPL_FUNC(void) TPPIfdefStack_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid stack");
 TPPIfdefStack_POP(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Top(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid stack");
 TPP_ASSERT(!TPPIfdefStack_EMPTY(self) && "Empty stack");
 return TPPIfdefStack_TOP(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPTokenObject *) TPPIfdefStack_TopToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid stack");
 TPP_ASSERT(!TPPIfdefStack_EMPTY(self) && "Empty stack");
 return TPPIfdefStack_TOP_TOKEN(self);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Empty(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid stack");
 return TPPIfdefStack_EMPTY(self);
}







typedef TPPKeywordHash TPPIncludeCacheHash;
#ifdef TPP_PLATFORM_WINDOWS
TPP_DECL_STATIC_INLINE(TPPKeywordHash) _TPPIncludeCache_HashOf(
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name) {
 TPPKeywordHash result = 1;
 char ch;
 for (ch = *name; ch != '\0'; ch = *++name) {
  // Ignore caseing on windows
  if (TPP_CH_IS_LOWER_UTF8(ch)) ch += 'a'-'A';
  result = result*263+ch;
 }
 return result % TPPIncludeCache_SIZE;
}
#else /* TPP_PLATFORM_WINDOWS */
#define _TPPIncludeCache_HashOf(str) (_TPPKeywordList_HashOf(str)%TPPIncludeCache_SIZE)
#endif /* !TPP_PLATFORM_WINDOWS */

TPP_IMPL_FUNC(void) TPPIncludeCache_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 _TPPIncludeCache_Init(self);
}
struct TPPIncludeCacheEntry *TPPIncludeCacheEntry_Copy(struct TPPIncludeCacheEntry *self) {
 struct TPPIncludeCacheEntry *result; size_t filename_size;
 TPP_ASSERT(self);
 filename_size = strlen(self->ce_filename);
 if ((result = (struct TPPIncludeCacheEntry *)TPP_CALLBACK_MALLOC_NZ(
  offsetof(struct TPPIncludeCacheEntry,ce_filename)+filename_size*sizeof(char))) == NULL) {
  TPP_CALLBACK_NO_MEMORY();
  return NULL;
 }
 if (self->ce_next) {
  if ((result->ce_next = TPPIncludeCacheEntry_Copy(self->ce_next)) == NULL) {
   TPP_CALLBACK_FREE_NN(result);
   return NULL;
  }
 } else result->ce_next = NULL;
 result->ce_guard_name = self->ce_guard_name;
 memcpy(result->ce_filename,self->ce_filename,filename_size*sizeof(char));
 return result;
}

#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28182)
#endif
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPIncludeCache_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPIncludeCacheEntry **iter,**end,*elem,*next,*const *src;
 TPP_ASSERT(self && right);
 end = (iter = self->ce_cache)+TPPIncludeCache_SIZE;
 src = right->ce_cache;
 while (iter != end) {
  if (*src) {
   if ((*iter = TPPIncludeCacheEntry_Copy(*src)) == NULL) {
    while (iter != self->ce_cache) {
     elem = *--iter;
     while (elem) {
      next = elem->ce_next;
      TPPIncludeCacheEntry_Free(elem);
      elem = next;
     }
    }
    return -1;
   }
  } else *iter = NULL;
  ++iter,++src;
 }
 return 0;
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif

TPP_IMPL_FUNC(void) TPPIncludeCache_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self && right);
 memcpy(self->ce_cache,right->ce_cache,sizeof(self->ce_cache));
 memset(right->ce_cache,0,sizeof(self->ce_cache));
}
TPP_IMPL_FUNC(void) TPPIncludeCache_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPIncludeCacheEntry **iter,**end,*elem,*next;
 TPP_ASSERT(self);
 end = (iter = self->ce_cache)+TPPIncludeCache_SIZE;
 for (elem = *iter; TPP_LIKELY(iter != end); elem = *++iter) {
  while (elem) {
   next = elem->ce_next;
   TPPIncludeCacheEntry_Free(elem);
   elem = next;
  }
 }
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_Get(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPIncludeCacheEntry *result;
 TPP_ASSERT(self);
 result = self->ce_cache[_TPPIncludeCache_HashOf(filename)];
 for (; result; result = result->ce_next)
  if (TPP_UNLIKELY(TPP_PATHCMP(result->ce_filename,filename) == 0))
   break;
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_SetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 char const *filename, size_t filename_len, TPPTokenID guard_name)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPIncludeCacheEntry *result,**slot;
 TPP_ASSERT(self);
 TPP_ASSERT(filename);
 slot = self->ce_cache+_TPPIncludeCache_HashOf(filename);
 // Check existing entry
 for (result = *slot; result; result = result->ce_next)
  if (TPP_UNLIKELY(TPP_PATHCMP(result->ce_filename,filename) == 0))
   return result;
 if (TPP_LIKELY((result = TPP_REINTERPRET_CAST(TPPIncludeCacheEntry *,TPP_CALLBACK_MALLOC_NZ(
  offsetof(TPPIncludeCacheEntry,ce_filename)+((filename_len+1)*sizeof(char))))) != NULL)) {
  result->ce_guard_name = guard_name;
  TPP_REINTERPRET_CAST(char *,memcpy(
   result->ce_filename,filename,filename_len*sizeof(char)))[filename_len] = '\0';
  result->ce_next = *slot;
  *slot = result;
 } else TPP_CALLBACK_NO_MEMORY();
 return result;
}







TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 self->l_files = NULL;
 self->l_flags = TPPLexer_FLAG_NONE;
 self->l_one_file_rec = 0;
#if TPP_CONFIG_HAVE___COUNTER__
 self->l_counter = 0;
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
 TPPKeywordList_Init(&self->l_keywords);
 _TPPIncludeList_Init(&self->l_include_list);
 _TPPIncludeList_Init(&self->l_sys_include_list);
 _TPPIfdefStack_Init(&self->l_ifdef_stack);
 _TPPIncludeCache_Init(&self->l_include_cache);
 if (TPP_UNLIKELY(TPPWarnings_Init(&self->l_warnings) == -1)) return -1;
 if (TPP_UNLIKELY(TPPKeywordList_SetupBuiltin(&self->l_keywords) == -1)) {
  TPPWarnings_Quit(&self->l_warnings);
  return -1;
 }
 _TPPMacroList_Init(&self->l_macros);
#define DEFINE_MACRO(name,value) if (TPPLexer_FastDefine(self,name,value) == -1) goto err
 DEFINE_MACRO("__TPP_VERSION__",TPP_PP_STRINGIZE(TPP_PREPROCESSOR_VERSION));
#if TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS
 DEFINE_MACRO("and","&&");
 DEFINE_MACRO("and_eq","&=");
 DEFINE_MACRO("bitand","&");
 DEFINE_MACRO("bitor","|");
 DEFINE_MACRO("compl","~");
 DEFINE_MACRO("not","!");
 DEFINE_MACRO("not_eq","!=");
 DEFINE_MACRO("or","||");
 DEFINE_MACRO("or_eq","|=");
 DEFINE_MACRO("xor","^");
 DEFINE_MACRO("xor_eq","^=");
#endif /* TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS */
#undef DEFINE_MACRO
 return 0;
err:
 TPPLexer_Quit(self);
 return -1;
}
TPP_ATTRIBUTE_USE_RETURN int TPPLexer_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *right,
 unsigned int flags)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self && right);
 if ((flags&TPP_LEXER_COPY_FLAG_FILES) != 0) {
  TPPFileObject *file_iter,*file_next;
  if ((file_iter = right->l_files) != NULL) {
   if ((self->l_files = TPPFile_Copy(file_iter)) == NULL) return -1;
   file_iter = self->l_files;
   while (file_iter->f_prev) {
    file_next = TPPFile_Copy(file_iter->f_prev);
    TPPFile_DECREF(file_iter->f_prev);
    if ((file_iter->f_prev = file_next) == NULL) goto err_0;
    file_iter = file_iter->f_prev;
   }
  } else self->l_files = NULL;
  if (TPPIfdefStack_InitCopy(&self->l_ifdef_stack,&right->l_ifdef_stack) == -1) {
err_0: file_iter = self->l_files;
   while (file_iter) {
    file_next = file_iter->f_prev;
    file_iter->f_prev = NULL; // steal reference - Don't use stack-recursion
    TPPFile_DECREF(file_iter);
    file_iter = file_next;
   }
   return -1;
  }
  self->l_one_file_rec = right->l_one_file_rec;
 } else {
  self->l_files = NULL;
  TPPIfdefStack_Init(&self->l_ifdef_stack);
  self->l_one_file_rec = 0;
 }
 if ((flags&TPP_LEXER_COPY_FLAG_KEYWORDS) != 0) {
  if (TPPKeywordList_InitCopy(&self->l_keywords,&right->l_keywords) == -1) {
err_1: TPPIfdefStack_Quit(&self->l_ifdef_stack); goto err_0;
  }
#if TPP_CONFIG_HAVE___COUNTER__
  self->l_counter = right->l_counter;
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
 } else {
  TPPKeywordList_Init(&self->l_keywords);
  if (TPP_UNLIKELY(TPPKeywordList_SetupBuiltin(&self->l_keywords) == -1)) goto err_1;
#if TPP_CONFIG_HAVE___COUNTER__
  self->l_counter = 0;
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
 }
 if ((flags&0x00000004)!=0) { // TPP_LEXER_COPY_FLAG_MACROS
  if (TPPMacroList_InitCopy(&self->l_macros,&right->l_macros) == -1) {
err_2: TPPKeywordList_Quit(&self->l_keywords); goto err_1;
  }
 } else TPPMacroList_Init(&self->l_macros);
 if ((flags&TPP_LEXER_COPY_FLAG_INCLUDES)!=0) {
  if (TPPIncludeList_InitCopy(&self->l_include_list,&right->l_include_list) == -1) {
err_3: TPPMacroList_Quit(&self->l_macros); goto err_2;
  }
 } else TPPIncludeList_Init(&self->l_include_list);
 if ((flags&TPP_LEXER_COPY_FLAG_SYSINCLUDES)!=0) {
  if (TPPIncludeList_InitCopy(&self->l_sys_include_list,&right->l_sys_include_list) == -1) {
err_4: TPPIncludeList_Quit(&self->l_include_list); goto err_3;
  }
 } else TPPIncludeList_Init(&self->l_sys_include_list);
 if ((flags&0x00000020)!=0) { // TPP_LEXER_COPY_FLAG_INCLUDE_CACHE
  if (TPPIncludeCache_InitCopy(&self->l_include_cache,&right->l_include_cache) == -1) {
err_5: TPPIncludeList_Quit(&self->l_sys_include_list); goto err_4;
  }
 } else TPPIncludeCache_Init(&self->l_include_cache);
 if ((flags&TPP_LEXER_COPY_FLAG_WARNINGS)!=0) {
  if (TPPWarnings_InitCopy(&self->l_warnings,&right->l_warnings) == -1) {
/*err_6:*/ TPPIncludeCache_Quit(&self->l_include_cache); goto err_5;
  }
 } else if (TPPWarnings_Init(&self->l_warnings) == -1) goto err_5;
 self->l_flags = right->l_flags;
 return 0;
}
TPP_ATTRIBUTE_USE_RETURN int TPPLexer_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self && right);
 if (TPPWarnings_InitMove(&self->l_warnings,&right->l_warnings) == -1) return -1;
 self->l_files = right->l_files;
 right->l_files = NULL;
 TPPKeywordList_InitMove(&self->l_keywords,&right->l_keywords);
 self->l_flags = right->l_flags;
 TPPMacroList_InitMove(&self->l_macros,&right->l_macros);
 TPPIncludeList_InitMove(&self->l_include_list,&right->l_include_list);
 TPPIncludeList_InitMove(&self->l_sys_include_list,&right->l_sys_include_list);
 TPPIfdefStack_InitMove(&self->l_ifdef_stack,&right->l_ifdef_stack);
 TPPIncludeCache_InitMove(&self->l_include_cache,&right->l_include_cache);
 self->l_one_file_rec = right->l_one_file_rec;
#if TPP_CONFIG_HAVE___COUNTER__
 self->l_counter = right->l_counter;
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
 return 0;
}


TPP_IMPL_FUNC(void) TPPLexer_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *file_iter;
 TPP_ASSERT(self);
 TPPKeywordList_Quit(&self->l_keywords);
 TPPMacroList_Quit(&self->l_macros);
 TPPIncludeList_Quit(&self->l_include_list);
 TPPIncludeList_Quit(&self->l_sys_include_list);
 TPPIfdefStack_Quit(&self->l_ifdef_stack);
 TPPIncludeCache_Quit(&self->l_include_cache);
 TPPWarnings_Quit(&self->l_warnings);
 file_iter = self->l_files;
 while (file_iter) {
  TPPFileObject *file_next = file_iter->f_prev;
  file_iter->f_prev = NULL; // steal reference - Don't use stack-recursion
  TPPFile_DECREF(file_iter);
  file_iter = file_next;
 }
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_AddStdInc(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 static char const include_dir[] = "include";
 char path[PATH_MAX];
 char *path_end;
 // from http://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
#if defined(TPP_PLATFORM_WINDOWS)
 if (GetModuleFileNameA(NULL,path,sizeof(path)) == 0) return -1;
#else
 {char szTmp[32];
 int bytes;
  sprintf(szTmp,"/proc/%d/exe",getpid());
  bytes = readlink(szTmp,path,sizeof(path));
  if (sizeof(path)-1 < bytes) bytes = sizeof(path)-1;
  if(bytes >= 0) path[bytes] = '\0';
 }
#endif
 if ((path_end = TPP_CONST_CAST(char *,path+(strlen(path)-1))) != path) {
  while (!TPP_IS_DIRSEP(*path_end))
   if (--path_end == path) break;
  ++path_end;
 } else {
  path[0] = '.';
  path[1] = TPP_SYS_SEP;
  path_end = path+2;
 }
 memcpy(path_end,include_dir,sizeof(include_dir));
 return TPPIncludeList_AddS(&self->l_sys_include_list,
  path,(path_end-path)+((sizeof(include_dir)/sizeof(char))-1));
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_ExecStringS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 char const *s, size_t string_length)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *file;
 unsigned int old_one_file_rec;
 TPPLexerFlags old_flags;
 TPPTokenObject token_ = TPPToken_INIT();
 int result = 0;
 TPP_ASSERT(self);
 TPP_ASSERT(s);
 //TPP_ASSERT(self->l_files == NULL || !TPPFile_IS_MACRO_FILE(self->l_files) &&
 //           "Can\'t exec string, while inside of a macro");
 if (TPP_UNLIKELY((file = TPPFile_NewFromString(s,string_length)) == NULL)) return -1;
 old_flags = self->l_flags;
 old_one_file_rec = self->l_one_file_rec;
 self->l_one_file_rec = 0;
 self->l_flags |= (TPPLexer_FLAG_ONE_FILE|TPPLexer_FLAG_ONE_REAL_FILE);
 self->l_flags &= ~(TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_NO_DIRECTIVES|TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_WANT_SPC);
 //TPPFile_XINCREF(self->l_files);
 file->f_prev = self->l_files;
 //TPPFile_XDECREF(self->l_files);
 TPPFile_INCREF(file);
 self->l_files = file;
 while (1) {
  if (TPP_UNLIKELY(TPPLexer_Next(self,&token_) == -1)) {
   result = -1;
   goto err0;
  }
  if (TPP_UNLIKELY(token_.tk_id == TPP_TOK_EOF))
   break;
  TPP_ASSERT(token_.tk_file == file &&
             "Can\'t use #include directives in TPPLexer_ExecString");
 }
err0:
 TPPToken_Quit(&token_);
 TPPFile_DECREF(self->l_files);
 //TPPFile_INCREF(file->f_prev);
 self->l_files = file->f_prev;
 //TPPFile_DECREF(file->f_prev);
 file->f_prev = NULL;
 self->l_one_file_rec = old_one_file_rec;
 self->l_flags = old_flags;
 TPPFile_DECREF(file);
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeStringS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, char const *code, size_t code_len)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *file;
 TPP_ASSERT(self);
 TPP_ASSERT(code);
 //TPP_ASSERT(self->l_files == NULL || !TPPFile_IS_MACRO_FILE(self->l_files) &&
 //           "Can\'t exec code, while inside of a macro");
 if (TPP_UNLIKELY((file = TPPFile_NewFromString(code,code_len)) == NULL)) return -1;
 if (self->l_files) {
  // Setup the reference to the #include file
  if ((file->f_ref_file = self->l_files->f_ref_file) != NULL) {
   file->f_ref_file_off = self->l_files->f_ref_file_off;
  } else {
   file->f_ref_file     = self->l_files;
   file->f_ref_file_off = TPPFile_ITER_OFF(self->l_files);
  }
  TPPFile_INCREF(file->f_ref_file);
 }
 //TPPFile_XINCREF(self->l_files);
 file->f_prev = self->l_files;
 //TPPFile_XDECREF(self->l_files);
 //TPPFile_INCREF(file);
 self->l_files = file;
 //TPPFile_DECREF(file);
 return 0;
}



TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefine(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, 
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, 
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *code)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *file;
 TPP_ASSERT(code);
 if (TPP_UNLIKELY((file = TPPFile_NewFromString(code,strlen(code))) == NULL)) return-1;
 if (TPP_UNLIKELY(TPPLexer_FastDefineWithFileInherited(self,name,file) == -1)) {
  TPPFile_DECREF(file);
  return -1;
 }
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefineWithFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(file);
 TPPFile_INCREF(file);
 return TPPLexer_FastDefineWithFileInherited(self,name,file);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefineWithFileInherited(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 TPPMacroObject *macro;
 TPPTokenID name_id;
 TPP_ASSERT(self);
 TPP_ASSERT(name);
 if (TPP_UNLIKELY((macro = TPPMacro_New()) == NULL)) goto err0;
 if (TPP_UNLIKELY((name_id = TPPKeywordList_Get(&self->l_keywords,name,1)) == 0)) goto err1;
 macro->m_code = file;
 if (TPPMacroList_Add(&self->l_macros,name_id,macro) == -1) goto err2;
 return 0;
err2: macro->m_code = NULL;
err1: TPPMacro_Free(macro);
err0: return -1;
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_ExpandingEquivalentMacroFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPFileObject const *iter;
 TPP_ASSERT(self);
 TPP_ASSERT(file);
 for (iter = self->l_files; iter != NULL; iter = iter->f_prev)
  if (TPP_UNLIKELY(TPPFile_IS_MACRO_FILE(iter) && _TPPFile_CmpEq(iter,file)))
   return 1;
 return 0;
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPFileObject *) TPPLexer_PublicFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *result;
 TPP_ASSERT(self);
 result = self->l_files;
 if (TPP_LIKELY(result != NULL && result->f_ref_file != NULL))
  result = result->f_ref_file;
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPFileObject *) TPPLexer_PublicBaseFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *result = TPPLexer_PublicFile(self);
 while (result->f_prev) result = result->f_prev;
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_PublicFileDepthEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int include_depth)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 int result = 0;
 TPPFileObject *iter;
 TPP_ASSERT(self);
 for (iter = self->l_files; iter; iter = iter->f_prev)
  if (TPPFile_IS_MACRO_FILE(iter) ^ include_depth)
   ++result;
 return result;
}

TPP_IMPL_FUNC(void) TPPLexer_PushFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPP_ASSERT(file);
 file->f_prev = self->l_files; // inherit reference
 self->l_files = file;
 TPPFile_INCREF(file);
}
TPP_IMPL_FUNC(void) TPPLexer_PushFileInheritedNoErr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 TPP_ASSERT(file);
 file->f_prev = self->l_files; // inherit reference
 self->l_files = file; // inherit reference
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_PushFileInherited(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 if (TPP_UNLIKELY(file == NULL)) return -1;
 file->f_prev = self->l_files; // inherit reference
 self->l_files = file; // inherit reference
 return 0;
}



TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *)
TPPKeyword_BuiltinToStr(TPPTokenID id) {
 switch (id) {
#define DEF(name,value) case name: return value; break;
#include "tpp_keywords.inl"
#undef DEF
  default: break;
 }
 return NULL;
}


//////////////////////////////////////////////////////////////////////////
// List of default token names for all tokens < 256
#define _TPPLexer_GetNameOfSmallTokenID(id) TPP_REINTERPRET_CAST(char const *,TPPLexer_TokenIDBaseNames+((id)*2))
static unsigned char const TPPLexer_TokenIDBaseNames[256*2] = {
#ifdef __TPP_EVAL
#define DECL2(i) __TPP_EVAL(i),0,__TPP_EVAL(i+1),0
#else
#define DECL2(i) i,0,i+1,0
#endif
#define DECL8(i) DECL2(i),DECL2(i+2),DECL2(i+4),DECL2(i+6)
#define DECL32(i) DECL8(i),DECL8(i+8),DECL8(i+16),DECL8(i+24)
 DECL32(0),DECL32(32),DECL32(64),DECL32(96),
 DECL32(128),DECL32(160),DECL32(192),DECL32(224)
#undef DECL32
#undef DECL8
#undef DECL2
};
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPLexer_TokenIDStr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char const *result;
 TPP_ASSERT(self);
 switch (id) {
  case TPP_TOK_EOF:         result = "EOF"; break;
  case TPP_TOK_MISC:        result = "MISC"; break;
  case TPP_TOK_SPACE:       result = "SPACE"; break;
  case TPP_TOK_LF:          result = "LF"; break;
  case TPP_TOK_INT:         result = "int"; break;
  case TPP_TOK_FLOAT:       result = "float"; break;
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
  case TPP_TOK_COMMENT:     result = "COMMENT"; break;
#endif
  case TPP_TOK_STR:         result = "string"; break;
  case TPP_TOK_CHR:         result = "char"; break;
  case TPP_TOK_CMP_LE:      result = "<="; break;
  case TPP_TOK_CMP_EQ:      result = "=="; break;
  case TPP_TOK_CMP_NE:      result = "!="; break;
  case TPP_TOK_CMP_GE:      result = ">="; break;
  case TPP_TOK_ADD_ASSIGN:  result = "+="; break;
  case TPP_TOK_SUB_ASSIGN:  result = "-="; break;
  case TPP_TOK_MUL_ASSIGN:  result = "*="; break;
  case TPP_TOK_DIV_ASSIGN:  result = "/="; break;
  case TPP_TOK_MOD_ASSIGN:  result = "%="; break;
  case TPP_TOK_SHL:         result = "<<"; break;
  case TPP_TOK_SHR:         result = ">>"; break;
  case TPP_TOK_SHL_ASSIGN:  result = "<<="; break;
  case TPP_TOK_SHR_ASSIGN:  result = ">>="; break;
  case TPP_TOK_DOTS:        result = "..."; break;
  case TPP_TOK_GLUE:        result = "##"; break;
#if TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
  case TPP_TOK_TILDE_TILDE: result = "~~"; break;
#endif /* TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
  case TPP_TOK_COLLON_ASSIGN: result = ":="; break;
#endif /* TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
  case TPP_TOK_COLLON_COLLON: result = "::"; break;
#endif /* TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN */
  case TPP_TOK_BIN_AND_ASSIGN: result = "&="; break;
  case TPP_TOK_BIN_OR_ASSIGN: result = "|="; break;
  case TPP_TOK_BIN_XOR_ASSIGN: result = "^="; break;
  case TPP_TOK_LAND:        result = "&&"; break;
  case TPP_TOK_LOR:         result = "||"; break;
#if TPP_CONFIG_HAVE_POW_TOKEN
  case TPP_TOK_POW:         result = "**"; break;
  case TPP_TOK_POW_ASSIGN:  result = "**="; break;
#endif /* TPP_CONFIG_HAVE_POW_TOKEN */
#if TPP_CONFIG_HAVE_LXOR_TOKEN
  case TPP_TOK_LXOR:        result = "^^"; break;
#endif /* TPP_CONFIG_HAVE_LXOR_TOKEN */
#if TPP_CONFIG_HAVE_ARROW_TOKEN
  case TPP_TOK_ARROW:       result = "->"; break;
#endif /* TPP_CONFIG_HAVE_ARROW_TOKEN */
  default:
   if (TPP_UNLIKELY(id <= 255)) {
    return _TPPLexer_GetNameOfSmallTokenID(id);
   } else {
    result = TPPKeyword_BuiltinToStr(id);
    if (TPP_LIKELY(result == NULL)) {
     result = TPPKeywordList_NameOf(&self->l_keywords,id);
#if 0
     if (TPP_UNLIKELY(result == NULL)) result = "UNKNOWN";
#endif
    }
   }
   break;
 }
 return result;
#undef SET_C_STR
}



#define _TPPLexer_PopFileEx(self,DisposeOldFile,HandleCrit)\
 do{\
  TPPFileObject *old_file = (self)->l_files;\
  if (TPP_LIKELY(TPPFile_IS_MACRO_FILE(old_file))) {\
  } else if (TPP_LIKELY(!TPPIfdefStack_EMPTY(&(self)->l_ifdef_stack))) {\
   while (1) {\
    TPPTokenObject *ifdef_tk = TPPIfdefStack_TOP_TOKEN(&(self)->l_ifdef_stack);\
    if (ifdef_tk->tk_file != old_file) break;\
    if (TPPWarn_IfdefWithoutEndif(self,ifdef_tk) == -1)\
     do HandleCrit while(0);\
    TPPIfdefStack_POP(&(self)->l_ifdef_stack);\
   }\
  }\
  /* Pop file from stack */\
  (self)->l_files = old_file->f_prev;\
  old_file->f_prev = NULL;\
  /* Drop the reference from the current file */\
  DisposeOldFile(old_file);\
 }while(0)
#define _TPPLexer_PopFile(self,HandleCrit) _TPPLexer_PopFileEx(self,TPPFile_DECREF,HandleCrit)

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_NextNoMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 int is_float;
 char ch,ch2;
 TPPFileObject *in_file;
 TPPTokenObject old_token;
 TPP_ASSERT(self);
 TPP_ASSERT(token_);
 old_token = *token_;
again:
 in_file = self->l_files;
 if (TPP_LIKELY((token_->tk_str_file = in_file) != NULL)) {
  TPPFile_INCREF(in_file);
 } else {
eof:
  token_->tk_id = TPP_TOK_EOF;
  token_->tk_str_file = NULL;
  token_->tk_str_begin = NULL;
  token_->tk_str_end = NULL;
  token_->tk_file = NULL;
  token_->tk_file_off = 0;
  goto nerror;
 }
 TPP_ASSERT(in_file);
 goto start;
repeat:
 TPPFile_DECREF(token_->tk_file);
start:
 token_->tk_str_begin = in_file->f_iter;
 if (TPP_LIKELY((token_->tk_file = in_file->f_ref_file) != NULL)) {
  token_->tk_file_off = in_file->f_ref_file_off;
 } else {
  token_->tk_file = in_file;
  token_->tk_file_off = TPPFile_ITER_OFF(in_file);
 }
 TPPFile_INCREF(token_->tk_file);
#define TRY_SKIP_UNLIKELY(ch) (TPPFile_Peek(in_file) == (ch) ? (TPPFile_Read(in_file),1) : 0)
#if 0
#define TRY_SKIP_LIKELY(ch)   (TPPFile_Read(in_file) == (ch) ? 1 : (TPPFile_Undo(in_file),0))
#else
#define TRY_SKIP_LIKELY(ch)   TRY_SKIP_UNLIKELY(ch)
#endif
 switch ((ch = TPPFile_Read(in_file))) {
  case '\0': // EOF
   TPPFile_DECREF(token_->tk_str_file);
   TPPFile_DECREF(token_->tk_file);
   TPP_ASSERT(in_file == self->l_files);
   if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_ONE_FILE) != 0)) {
    if (TPP_UNLIKELY(self->l_one_file_rec != 0)) { --self->l_one_file_rec; goto do_pop; }
    goto eof;
   }
   if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_ONE_REAL_FILE) != 0 &&
       !TPPFile_IS_MACRO_FILE(in_file))
       ) {
       goto eof; // Can't pop non-macro file
   }
do_pop:
   _TPPLexer_PopFile(self,{goto error;});
   goto again;
   break;

  case '.':
   ch = TPPFile_Peek(in_file);
   if (TPP_UNLIKELY(TPP_CH_IS_DIGIT(ch))) {
    // .123 (float)
    is_float = 1;
    goto num;
   } else if (TPP_UNLIKELY(ch == '.')) {
    // Check for dots '...'
    TPPFile_Read(in_file);
    ch = TPPFile_Peek(in_file);
    if (TPP_LIKELY(ch == '.')) {
     TPPFile_Read(in_file);
     token_->tk_id = TPP_TOK_DOTS;
    } else {
     TPPFile_Undo(in_file);
     goto onech;
    }
   } else {
    ch = '.';
    goto onech;
   }
   break;

  case '0': case '1': case '2': case '3':
  case '4': case '5': case '6': case '7':
  case '8': case '9': is_float = 0; num: {
   int is_hex = 0;
   // Read digits and "."
   ch2 = '\0';
   while (1) {
    switch ((ch = TPPFile_Peek(in_file))) {
     case '.':
      if (TPP_UNLIKELY(is_float)) goto num_end;
      is_float = 1;
      break;
     case 'x': case 'X': is_hex = 1; break;
     case 'e': case 'E': if (is_hex) goto num_end;
     case 'p': case 'P': // exponent suffix
      is_float = 1;
      TPPFile_Read(in_file);
      ch = TPPFile_Read(in_file);
      if (ch == '+' || ch == '-') ch = TPPFile_Read(in_file);
      while (TPP_CH_IS_DIGIT(ch)) ch = TPPFile_Read(in_file);
      if (ch != '\0') TPPFile_Undo(in_file);
      goto num_end;
     default:
      // must use alnum, to support "0xDEADBEEF"
      if (TPP_UNLIKELY(!TPP_CH_IS_ALNUM(ch))) goto num_end2;
    }
    ch2 = TPPFile_Read(in_file);
   }
num_end:
   // Read number suffix
   while (TPP_CH_IS_ALNUM(ch)) {
    TPPFile_Read(in_file);
    ch = TPPFile_Peek(in_file);
   }
num_end2:
   // Exception: If the number ends with "f"/"F" and isn't hexadecimal, it's a float
   if ((ch == 'f' || ch == 'F') && !is_hex) is_float = 1;
   token_->tk_id = is_float ? TPP_TOK_FLOAT : TPP_TOK_INT;
  } break;
  case '<':
   ch = TPPFile_Read(in_file);
   if (TPP_UNLIKELY(ch == '<')) {
    token_->tk_id = TRY_SKIP_UNLIKELY('=') ? TPP_TOK_SHL_ASSIGN : TPP_TOK_SHL;
   } else if (TPP_UNLIKELY(ch == '=')) {
    token_->tk_id = TPP_TOK_CMP_LE;
#if TPP_CONFIG_HAVE_DIGRAPHS
   } else if (TPP_UNLIKELY(ch == '%')) {
    token_->tk_id = TPP_TOK_LBRACE; // [<%] --> [{]
   } else if (TPP_UNLIKELY(ch == ':')) {
    token_->tk_id = TPP_TOK_LBRACKET; // [<:] --> [[]
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
   } else {
    if (ch != '\0') TPPFile_Undo(in_file);
    ch = '<';
    goto onech;
   }
   break;
  case '>':
   ch = TPPFile_Read(in_file);
   if (TPP_UNLIKELY(ch == '>')) {
    token_->tk_id = TRY_SKIP_UNLIKELY('=') ? TPP_TOK_SHR_ASSIGN : TPP_TOK_SHR;
   } else if (TPP_UNLIKELY(ch == '=')) {
    token_->tk_id = TPP_TOK_CMP_GE;
   } else {
    if (ch != '\0') TPPFile_Undo(in_file);
    ch = '>';
    goto onech;
   }
   break;
  case '*':
   ch = TPPFile_Peek(in_file);
#if TPP_CONFIG_HAVE_POW_TOKEN
   if (TPP_UNLIKELY(ch == '*')) {
    TPPFile_Read(in_file);
    if ((ch = TPPFile_Peek(in_file)) == '=') {
     TPPFile_Read(in_file);
     token_->tk_id = TPP_TOK_POW_ASSIGN;
    } else {
     token_->tk_id = TPP_TOK_POW;
    }
   } else
#endif /* TPP_CONFIG_HAVE_POW_TOKEN */
   if (TPP_UNLIKELY(ch == '=')) {
    TPPFile_Read(in_file);
    token_->tk_id = TPP_TOK_MUL_ASSIGN;
   } else {
    if (TPP_UNLIKELY(ch == '/')) {
     if (TPPWarn_StarSlashOutsideOfComment(self,token_) == -1) goto error;
    }
    ch = '*';
    goto onech;
   }
   break;
  case '!': if (TPP_UNLIKELY(!TRY_SKIP_LIKELY('='))) goto onech; token_->tk_id = TPP_TOK_CMP_NE; break;
  case '=': if (TPP_UNLIKELY(!TRY_SKIP_LIKELY('='))) goto onech; token_->tk_id = TPP_TOK_CMP_EQ; break;
  case '+':
   switch ((ch = TPPFile_Peek(in_file))) {
    case '=': token_->tk_id = TPP_TOK_ADD_ASSIGN; break;
    case '+': token_->tk_id = TPP_TOK_INC; break;
    default: ch = '+'; goto onech;
   }
   TPPFile_Read(in_file);
   break;
  case '-':
   switch ((ch = TPPFile_Peek(in_file))) {
    case '=': token_->tk_id = TPP_TOK_SUB_ASSIGN; break;
    case '-': token_->tk_id = TPP_TOK_DEC; break;
#if TPP_CONFIG_HAVE_ARROW_TOKEN
    case '>': token_->tk_id = TPP_TOK_ARROW; break;
#endif /* TPP_CONFIG_HAVE_ARROW_TOKEN */
    default: ch = '-'; goto onech;
   }
   TPPFile_Read(in_file);
   break;
  case '/':
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
   switch ((ch = TPPFile_Peek(in_file))) {
    case '=':
     token_->tk_id = TPP_TOK_DIV_ASSIGN;
     TPPFile_Read(in_file);
     break;
    case '*':
     if ((self->l_flags & TPPLexer_FLAG_TOK_COMMENTS) == 0) goto no_comment;
     token_->tk_id = TPP_TOK_COMMENT;
     TPPFile_Read(in_file);
     while (1) {
      ch = TPPFile_Read(in_file);
      if (TPP_UNLIKELY(ch == '\0')) break;
      if (TPP_UNLIKELY(ch == '*')) {
       ch = TPPFile_Read(in_file);
       if (TPP_LIKELY(ch == '/' || ch == '\0')) break;
      }
     }
     break;
    case '/':
     if ((self->l_flags & TPPLexer_FLAG_TOK_COMMENTS) != 0) goto line_comment;
     // fallthrough
    default:no_comment: ch = '/'; goto onech;
   }
#else /* TPP_CONFIG_HAVE_COMMENT_TOKEN */
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('='))) goto onech;
   token_->tk_id = TPP_TOK_DIV_ASSIGN;
#endif /* TPP_CONFIG_HAVE_COMMENT_TOKEN */
   break;
#if TPP_CONFIG_HAVE_DIGRAPHS
  case '%':
   switch ((ch = TPPFile_Peek(in_file))) {
    case '=': token_->tk_id = TPP_TOK_MOD_ASSIGN; break;
    case '>': token_->tk_id = TPP_TOK_RBRACE; break; // [%>] --> [}]
    case ':': token_->tk_id = TPP_TOK_HASH; break; // [%:] --> [#]
    default: ch = '%'; goto onech;
   }
   TPPFile_Read(in_file);
   break;
#else /* TPP_CONFIG_HAVE_DIGRAPHS */
  case '%':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('='))) goto onech;
   token_->tk_id = TPP_TOK_MOD_ASSIGN;
   break;
#endif /* !TPP_CONFIG_HAVE_DIGRAPHS */
  case '#':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('#'))) {
    if ((self->l_flags & TPPLexer_FLAG_ASM_COMMENTS) != 0) {
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
     if ((self->l_flags & TPPLexer_FLAG_TOK_COMMENTS) != 0) {
line_comment:
      do ch = TPPFile_Read(in_file);
      while (ch != '\0' && ch != '\n');
      if (ch == '\n') {
       if (TPPFile_Peek(in_file) == '\\') goto line_comment;
       TPPFile_Undo(in_file);
      }
      token_->tk_id = TPP_TOK_COMMENT;
     } else
#endif /* TPP_CONFIG_HAVE_COMMENT_TOKEN */
     {
      do {
       ch = TPPFile_Read(in_file);
       if (ch == '\0') goto eof;
      } while (ch != '\n');
      goto lf;
     }
    } else goto onech;
   } else {
    token_->tk_id = TPP_TOK_GLUE;
   }
   break;
#define DEFINE_LOGIC_OPERATOR(op1,op2,l_name,ass_name)\
  case op1:\
   switch ((ch = TPPFile_Peek(in_file))) {\
    case op2: token_->tk_id = l_name; break;\
    case '=': token_->tk_id = ass_name; break;\
    default: ch = op1; goto onech;\
   }\
   TPPFile_Read(in_file);\
   break
  DEFINE_LOGIC_OPERATOR('&','&',TPP_TOK_LAND,TPP_TOK_BIN_AND_ASSIGN);
  DEFINE_LOGIC_OPERATOR('|','|',TPP_TOK_LOR,TPP_TOK_BIN_OR_ASSIGN);
#if TPP_CONFIG_HAVE_LXOR_TOKEN
  DEFINE_LOGIC_OPERATOR('^','^',TPP_TOK_LXOR,TPP_TOK_BIN_XOR_ASSIGN);
#else /* TPP_CONFIG_HAVE_LXOR_TOKEN */
  case '^':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('='))) goto onech;
   token_->tk_id = TPP_TOK_BIN_XOR_ASSIGN;
   break;
#endif /* TPP_CONFIG_HAVE_LXOR_TOKEN */
#undef DEFINE_LOGIC_OPERATOR
#if TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
  case '~':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('~'))) goto onech;
   token_->tk_id = TPP_TOK_TILDE_TILDE;
   break;
#endif /* TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */

#if (!!TPP_CONFIG_HAVE_DIGRAPHS)+\
    (!!TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN)+\
    (!!TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN) > 1
  case ':': // Use a switch if more than 1 token starting with ':' is used
   switch ((ch = TPPFile_Peek(in_file))) {
#if TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
    case '=': token_->tk_id = TPP_TOK_COLLON_ASSIGN; break;
#endif /* TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
    case ':': token_->tk_id = TPP_TOK_COLLON_COLLON; break;
#endif /* TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN */
#if TPP_CONFIG_HAVE_DIGRAPHS
    case '>': token_->tk_id = TPP_TOK_RBRACKET; break; // [:>] --> []]
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
    default: ch = ':'; goto onech;
   }
   TPPFile_Read(in_file);
   break;
#elif TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
  case ':':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('='))) goto onech;
   token_->tk_id = TPP_TOK_COLLON_ASSIGN;
   break;
#elif TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
  case ':':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY(':'))) goto onech;
   token_->tk_id = TPP_TOK_COLLON_COLLON;
   break;
#elif TPP_CONFIG_HAVE_DIGRAPHS
  case ':':
   if (TPP_LIKELY(!TRY_SKIP_UNLIKELY('>'))) goto onech;
   token_->tk_id = TPP_TOK_RBRACKET; // [:>] --> []]
   break;
#else
  case ':': // fallthrough
#endif

#if !TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
  case '~':
#endif /* !TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */
  case '(': case ')': case '[': case ']':
  case '{': case '}': case '?':
  case ';': case ',': case '@': onech: {
   token_->tk_id = TPP_STATIC_CAST(TPPTokenID,ch);
  } break;

  case 'L': case 'u': case 'U':
   if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_INC_STRING) == 0)) {
    ch2 = TPPFile_Peek(in_file);
    if (ch2 == '\'' || ch2 == '\"') {
     TPPFile_Read(in_file);
     goto str;
    }
   }
   goto kwd;

  case '\"': case '\'':
   ch2 = ch;
str:
#if 0
   // This wouldn't work out...
   if (TPP_UNLIKELY(ch2 == '\'' && TPPFile_Peek(in_file) == '\'')) {
    // Special handling: ''' --> '\''
    TPPFile_Read(in_file);
   }
#endif
   while (1) {
    ch = TPPFile_Read(in_file);
    if (TPP_UNLIKELY(ch == '\\' && (self->l_flags & TPPLexer_FLAG_INC_STRING) == 0)) {
     TPPFile_Read(in_file);
    } else if (ch == ch2 || ch == '\0'
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
               || ch == '\n'
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
               ) {
     break;
    }
   }
   token_->tk_id = TPP_STATIC_CAST(TPPTokenID,ch2);
   break;

#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
  case '$':
#endif /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
  case 'a': case 'b': case 'c':  case 'd':  case 'e':  case 'f': case 'g': case 'h':
  case 'i': case 'j': case 'k':  case 'l':  case 'm':  case 'n': case 'o': case 'p':
  case 'q': case 'r': case 's':  case 't':/*case 'u':*/case 'v': case 'w': case 'x':
  case 'y': case 'z':
  case 'A': case 'B': case 'C':  case 'D':  case 'E':  case 'F': case 'G': case 'H':
  case 'I': case 'J': case 'K':/*case 'L':*/case 'M':  case 'N': case 'O': case 'P':
  case 'Q': case 'R': case 'S':  case 'T':/*case 'U':*/case 'V': case 'W': case 'X':
  case 'Y': case 'Z':
  case '_':kwd: { // Keyword
   char kwd_buffer[1024];
   size_t kwd_buffer_len = 1;
   TPPKeywordListHashEntryObject *entry;
   kwd_buffer[0] = ch;
   while (1) {
    ch = TPPFile_Read(in_file);
    if (TPP_UNLIKELY(!TPP_CH_IS_ALNUM(ch))) {
     if (ch != '\0') TPPFile_Undo(in_file);
     break;
    }
    kwd_buffer[kwd_buffer_len++] = ch;
   }
#if TPP_DEBUG
   kwd_buffer[kwd_buffer_len] = '\0';
#endif /* TPP_DEBUG */
   entry = TPPKeywordList_GetEntryS(&self->l_keywords,kwd_buffer,kwd_buffer_len,1);;
   if (entry == NULL) goto error;
   token_->tk_id = entry->he_id;
   TPP_ASSERT(token_->tk_id != 0);
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
   if ((self->l_flags & TPPLexer_FLAG_NO_DEPRECATED) == 0 && 
       (entry->he_flags & TPPKeywordFlag_IS_DEPRECATED) != 0) {
    token_->tk_str_end = in_file->f_iter; // Would have been done later
    if (TPPWarn_DeprecatedKeyword(self,token_) == -1) goto error;
   }
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
  } break;
  case '\r':
   if (TPPFile_Peek(in_file) == '\n') TPPFile_Read(in_file);
   // fallthrough
  case '\n':lf: {
   if ((self->l_flags & TPPLexer_FLAG_WANT_LF) != 0) {
    token_->tk_id = TPP_TOK_LF;
   } else {
    goto white;
   }
  } break;
  case ' ':white: {
   if ((self->l_flags & TPPLexer_FLAG_WANT_SPC) != 0) {
    while (1) {
     ch = TPPFile_Peek(in_file);
     if (ch == '\n' || !TPP_CH_IS_WHSPC(ch)) break;
     TPPFile_Read(in_file);
    }
    token_->tk_id = TPP_TOK_SPACE;
   } else {
    goto repeat;
   }
  } break;

  default:
#if TPP_CONFIG_USE_CHARACTER_TRAITS
   if (TPP_LIKELY(TPP_CH_IS_WHSPC(ch))) goto white; // Whitespace
#else /* TPP_CONFIG_USE_CHARACTER_TRAITS */
   if (TPP_LIKELY(TPP_CH_IS_WHITE(ch))) goto white; // Whitespace
#endif /* !TPP_CONFIG_USE_CHARACTER_TRAITS */
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
   if (TPP_LIKELY(TPP_CH_IS_ALPHA_ANSI(ch))) goto kwd;
#endif /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
   // single-character token: ???
   token_->tk_id = TPP_TOK_MISC;
   break;
 }
#undef TRY_SKIP_UNLIKELY
#undef TRY_SKIP_LIKELY
 token_->tk_str_end = in_file->f_iter;
nerror:
 // Drop all references, the token might have held before this call
 TPPFile_XDECREF(old_token.tk_file);
 TPPFile_XDECREF(old_token.tk_str_file);
 return 0;
error:
 TPP_ASSERT(token_->tk_file != 0);
 TPP_ASSERT(token_->tk_str_file != 0);
 TPPFile_DECREF(token_->tk_file);
 TPPFile_DECREF(token_->tk_str_file);
 *token_ = old_token; // Restore old token
 return -1;
}


TPP_IMPL_FUNC(char) TPPLexer_SkipSpace(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *in_file;
 char ch;
 TPP_ASSERT(self);
again:
 if (TPP_UNLIKELY((in_file = self->l_files) == NULL)) {
eof: return '\0';
 }
 while (1) {
  if (TPP_UNLIKELY((ch = TPPFile_Read(in_file)) == '\0')) {
   // Next file
   if ((self->l_flags & TPPLexer_FLAG_ONE_FILE) != 0) {
    if (self->l_one_file_rec != 0) { --self->l_one_file_rec; goto do_pop; }
    goto eof;
   }
   if ((self->l_flags & TPPLexer_FLAG_ONE_REAL_FILE) != 0 &&
       !TPPFile_IS_MACRO_FILE(in_file)) goto eof; // Can't pop non-macro file
do_pop:
   _TPPLexer_PopFile(self,{goto eof;});
   goto again;
  }
  if (TPP_UNLIKELY(!TPP_CH_IS_WHSPC(ch))) break;
 }
 return ch;
}
TPP_IMPL_FUNC(char) TPPLexer_PeekSpace(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *in_file;
 char ch;
 unsigned int one_file_rec;
 char const *in_iter;
 TPP_ASSERT(self);
 one_file_rec = self->l_one_file_rec;
 if (TPP_UNLIKELY((in_file = self->l_files) == NULL)) {
eof: return '\0';
 }
again:
 in_iter = in_file->f_iter;
 TPP_ASSERT(in_iter);
 while (1) {
  if (TPP_UNLIKELY((ch = *in_iter++) == '\0')) {
   // Next file
   if ((self->l_flags & TPPLexer_FLAG_ONE_FILE) != 0) {
    if (one_file_rec != 0) { --one_file_rec; goto do_pop; }
    goto eof;
   }
   if ((self->l_flags & TPPLexer_FLAG_ONE_REAL_FILE) != 0 &&
       !TPPFile_IS_MACRO_FILE(in_file)) goto eof; // Can't pop non-macro file
do_pop:
   if (TPP_UNLIKELY((in_file = in_file->f_prev) == NULL)) goto eof;
   goto again;
  } else if (ch == '\\') {
   if (*in_iter == '\n') {
    ++in_iter;
   } else if (*in_iter == '\r') {
    if (*++in_iter == '\n') ++in_iter;
   } else break;
  } else {
   if (TPP_UNLIKELY(!TPP_CH_IS_WHSPC(ch))) break;
  }
 }
 return ch;
}


struct TPPFileListObject {
 size_t         fl_size;       /*< Size of the list (NOTE: Never 0). */
 TPPFileObject *fl_list[1024]; /*< [0..1][ref][fl_size] List elements (inline). */
};
TPP_DECL_STATIC_INLINE(int) TPPFileList_AppendInherited(
 struct TPPFileListObject **pself, TPPFileObject *file) {
 struct TPPFileListObject *new_self;
 TPP_ASSERT(pself); // NOTE: "*pself" may be NULL
 TPP_ASSERT(file);
 if ((new_self = *pself) != NULL) {
  new_self = TPP_REINTERPRET_CAST(struct TPPFileListObject *,TPP_CALLBACK_REALLOC_NNZ(
   new_self,offsetof(struct TPPFileListObject,fl_list)+
   ((++new_self->fl_size)*sizeof(TPPFileObject *))));
  if (new_self == NULL) {no_mem: TPP_CALLBACK_NO_MEMORY(); return -1; }
 } else {
  new_self = TPP_REINTERPRET_CAST(struct TPPFileListObject *,TPP_CALLBACK_MALLOC_NZ(
   offsetof(struct TPPFileListObject,fl_list)+sizeof(TPPFileObject *)));
  if (new_self == NULL) goto no_mem;
  new_self->fl_size = 1;
 }
 new_self->fl_list[new_self->fl_size-1] = file;
 *pself = new_self;
 return 0;
}
TPP_DECL_STATIC_INLINE(void) TPPFileList_Free(struct TPPFileListObject *self) {
 TPPFileObject **begin,**end;
 TPP_ASSERT(self);
 TPP_ASSERT(self->fl_size != 0);
 end = (begin = self->fl_list)+self->fl_size;
 while (1) {
  TPPFile_XDECREF(*begin);
  if (++begin == end) break;
 }
 TPP_CALLBACK_FREE_NN(self);
}


//////////////////////////////////////////////////////////////////////////
// >> Skip space, pop files and keep a list of all popped files.
//    NOTE: When a file is popped, its start iterator position is restored
// >> This funciton is used, to skip whitespace between a macro function and the '(' token
// >> If the resulting macro code is already being expanded, then the list in "files" can
//    be used, to rollback the lexer to the first whitespace token after the original file
// Returns the start f_iter position of the last non-popped top file of "self"
// Returns NULL on error
// NOTE: self must have at least 1 file
// NOTE: self must not lease its last file due to this call
// NOTE: This function is inline, as it is private and only called from 1 place
TPP_DECL_STATIC_INLINE(char const *) _TPPLexer_SkipSpaceAndGetPoppedFiles(
 TPPLexerObject *self, struct TPPFileListObject **files) {
 TPPFileObject *in_file;
 char ch;
 char const *start_iter;
 TPP_ASSERT(self);
 TPP_ASSERT(files);
again:
 TPP_ASSERT(self->l_files != NULL);
 start_iter = (in_file = self->l_files)->f_iter;
 while (1) {
  if (TPP_UNLIKELY((ch = TPPFile_Read(in_file)) == '\0')) {
   // Next file
   if ((self->l_flags & TPPLexer_FLAG_ONE_FILE) != 0) {
    if (self->l_one_file_rec != 0) { --self->l_one_file_rec; goto do_pop; }
    goto end;
   }
   if ((self->l_flags & TPPLexer_FLAG_ONE_REAL_FILE) != 0 &&
       !TPPFile_IS_MACRO_FILE(in_file)) goto end; // Can't pop non-macro file
do_pop:
   TPP_ASSERT(in_file == self->l_files);
   in_file->f_iter = start_iter;
   if (TPPFileList_AppendInherited(files,in_file) == -1) {
err:
    return NULL; // TODO: Restore
   }
#define IGNORE_FILE(file) (void)0 // Reference is inherited by the "TPPFileList_AppendInherited"
   _TPPLexer_PopFileEx(self,IGNORE_FILE,{goto err;});
#undef IGNORE_FILE
   goto again;
  }
  if (TPP_UNLIKELY(!TPP_CH_IS_WHSPC(ch))) break;
 }
end:
 return start_iter;
}


#undef _TPPLexer_PopFile





struct TPPMacroArg {
 char const *begin; /* [1..1] begin of the argument text. */
 char const *end;   /* [1..1] end of the argument text. */
};

#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
static TPPTokenID const _TPPMacroCallingConvLChars[4] = {'(','[','{','<'};
static TPPTokenID const _TPPMacroCallingConvRChars[4] = {')',']','}','>'};
#define TPP_MACRO_GET_LPAREN_CHAR(macro) _TPPMacroCallingConvLChars[(macro)->m_call_conv]
#define TPP_MACRO_GET_RPAREN_CHAR(macro) _TPPMacroCallingConvRChars[(macro)->m_call_conv]
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
#define TPP_MACRO_GET_LPAREN_CHAR(macro) '('
#define TPP_MACRO_GET_RPAREN_CHAR(macro) ')'
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */

//////////////////////////////////////////////////////////////////////////
// Reads tokens from the input stream until ')' is recursively encountered.
// Using those tokens, a FileObject is created with the arguments previously
// read already expanded. NOTE: the stringing "#" and cat "##" operators are
// also executed here, alongside expansion of arguments during insertion
// NOTE: This function is inline, as it is private and only called from 1 place
TPP_DECL_STATIC_INLINE(TPPFileObject *) _TPPLexer_CreateExpandedMacroFunction(
 TPPLexerObject *self, TPPMacroObject *macro, TPPTokenObject const *token_) {
 int inv_argc_warning_emitted = 0;
 char ch;
 TPPFileObject *in_file;
 // v Start iterator of the input file
 //  (restored, if macro expansion failed)
 char const *in_file_start;
 TPP_ASSERT(self);
 TPP_ASSERT(macro);
 TPP_ASSERT(TPPMacro_DEFINED(macro));
 TPP_ASSERT(TPPMacro_HAS_ARGS(macro));
 in_file = self->l_files;
 in_file_start = in_file->f_iter;
 if (macro->m_argc == 0) {
  // Special handling for "#define foo()"
  while (1) {
   ch = TPPFile_Read(in_file);
   if (ch == TPP_STATIC_CAST(char,TPP_MACRO_GET_RPAREN_CHAR(macro))) break;
   if (!TPP_CH_IS_WHSPC(ch) && !inv_argc_warning_emitted) {
    inv_argc_warning_emitted = 1;
    if (TPPWarn_InvalidMacroArgCount(self,token_,macro) == -1)
     goto err0;
   }
  }
  // Use the macro function code itself!
  TPPFile_REWIND(macro->m_code);
  TPPFile_INCREF(macro->m_code);
  _TPPFile_SetReferenceFromToken(macro->m_code,token_); // Setup the token reference in the file
  return macro->m_code;
 } else {
  TPPFileObject *result;
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
  TPPMacroCallingConvention call_conv = macro->m_call_conv;
  unsigned int recursion[4] = {0,0,0,0};
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
  unsigned int paren_recursion; // '(' / ')' recursion
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
  size_t i,argc,total_file_size;
  int in_str = 0,cat_tokens = 0;
  struct TPPMacroArg *argv,*argv_end,*argv_iter;
  if ((argv = TPP_REINTERPRET_CAST(struct TPPMacroArg *,TPP_CALLBACK_MALLOC_NZ(
   (argc = macro->m_argc)*sizeof(struct TPPMacroArg)))
   ) == NULL) { TPP_CALLBACK_NO_MEMORY(); return NULL; }
  // First paren was already consumed by caller
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
  recursion[call_conv] = 1;
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
  paren_recursion = 1;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
  argv_end = (argv_iter = argv)+argc;
#if !TPP_CONFIG_STRICT_WHITESPACE
  while (1) {
   ch = TPPFile_Read(in_file);
   if (ch == '\0') {
    argv_iter->begin = in_file->f_iter;
    goto arg_eof;
   }
   if (!TPP_CH_IS_WHSPC(ch)) break;
  }
  TPPFile_Undo(in_file);
#endif /* !TPP_CONFIG_STRICT_WHITESPACE */
  argv_iter->begin = in_file->f_iter;
  while (
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
   recursion[call_conv] != 0
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
   paren_recursion != 0
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
   ) {
   switch ((ch = TPPFile_Read(in_file))) {
    case '\0':arg_eof:
     if (TPPWarn_UnexpectedEOFInMacroArgList(self,token_,macro) == -1)
      goto err1;
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
     recursion[call_conv] = 0;
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
     paren_recursion = 0;
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
     if ((argv_iter->end = in_file->f_iter-1) < argv_iter->begin)
      argv_iter->end = argv_iter->begin; // This can happen, is the argument started at EOF
     goto eval_args;
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
    case '\n': in_str = 0; break;
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
    case '\\':
     if (in_str) TPPFile_Read(in_file);
     break;
    case '\"': if ((in_str & 0x02) == 0) in_str ^= 0x01; break;
    case '\'': if ((in_str & 0x01) == 0) in_str ^= 0x02; break;
    case ',':
     if (!in_str &&
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
         recursion[call_conv] == 1 &&
         recursion[(call_conv+1)%4] == 0 &&
         recursion[(call_conv+2)%4] == 0 &&
         recursion[(call_conv+3)%4] == 0
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
         paren_recursion == 1
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
         ) {
      // Next argument
      argv_iter->end = in_file->f_iter-1;
#if !TPP_CONFIG_STRICT_WHITESPACE
      while (argv_iter->end > argv_iter->begin) {
       ch = argv_iter->end[-1];
       if (!TPP_CH_IS_WHSPC(ch)) break;
       --argv_iter->end;
      }
#endif /* !TPP_CONFIG_STRICT_WHITESPACE */
      if (argv_iter != argv_end-1) {
#if !TPP_CONFIG_STRICT_WHITESPACE
       while (1) {
        ch = TPPFile_Read(in_file);
        if (ch == '\0') goto arg_eof;
        if (!TPP_CH_IS_WHSPC(ch)) break;
       }
#else /* !TPP_CONFIG_STRICT_WHITESPACE */
       if (TPPFile_Read(in_file) == '\0') goto arg_eof;
#endif /* TPP_CONFIG_STRICT_WHITESPACE */
       TPPFile_Undo(in_file);
       (++argv_iter)->begin = in_file->f_iter;
      } else if (
#if TPP_CONFIG_HAVE___VA_ARGS__
       !TPPMacro_IS_VA_ARGS_FUNCTION(macro) &&
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
       !inv_argc_warning_emitted) {
       inv_argc_warning_emitted = 1;
       if (TPPWarn_InvalidMacroArgCount(self,token_,macro) == -1)
        goto err0;
      }
     }
     break;
#if !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
    case '(': if (!in_str) ++paren_recursion; break;
    case ')': if (!in_str) --paren_recursion; break;
#else /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
    case '(': if (!in_str) ++recursion[TPPMacroCallingConvention_PAREN]; break;
    case ')': if (!in_str) --recursion[TPPMacroCallingConvention_PAREN]; break;
    case '[':
     if (!in_str && call_conv >= TPPMacroCallingConvention_BRACKET &&
         recursion[TPPMacroCallingConvention_PAREN] == 0
         ) ++recursion[TPPMacroCallingConvention_BRACKET];
     break;
    case ']':
     if (!in_str && call_conv >= TPPMacroCallingConvention_BRACKET &&
         recursion[TPPMacroCallingConvention_PAREN] == 0
         ) --recursion[TPPMacroCallingConvention_BRACKET];
     break;
    case '{':
     if (!in_str && call_conv >= TPPMacroCallingConvention_BRACE &&
         recursion[TPPMacroCallingConvention_PAREN] == 0 &&
         recursion[TPPMacroCallingConvention_BRACKET] == 0
         ) ++recursion[TPPMacroCallingConvention_BRACE];
     break;
    case '}':
     if (!in_str && call_conv >= TPPMacroCallingConvention_BRACE &&
         recursion[TPPMacroCallingConvention_PAREN] == 0 &&
         recursion[TPPMacroCallingConvention_BRACKET] == 0
         ) --recursion[TPPMacroCallingConvention_BRACE];
     break;
    case '<':
     if (!in_str && call_conv >= TPPMacroCallingConvention_ANGLE &&
         recursion[TPPMacroCallingConvention_PAREN] == 0 &&
         recursion[TPPMacroCallingConvention_BRACKET] == 0 &&
         recursion[TPPMacroCallingConvention_BRACE] == 0
         ) ++recursion[TPPMacroCallingConvention_ANGLE];
     break;
    case '>':
     if (!in_str && call_conv >= TPPMacroCallingConvention_ANGLE &&
         recursion[TPPMacroCallingConvention_PAREN] == 0 &&
         recursion[TPPMacroCallingConvention_BRACKET] == 0 &&
         recursion[TPPMacroCallingConvention_BRACE] == 0
         ) --recursion[TPPMacroCallingConvention_ANGLE];
     break;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
    default: break;
   }
  }
  argv_iter->end = in_file->f_iter-1;
eval_args:
#if !TPP_CONFIG_STRICT_WHITESPACE
  while (argv_iter->end > argv_iter->begin) {
   ch = argv_iter->end[-1];
   if (!TPP_CH_IS_WHSPC(ch)) break;
   --argv_iter->end;
  }
#endif /* !TPP_CONFIG_STRICT_WHITESPACE */
  if (argv_iter != argv_end-1) {
   if (!inv_argc_warning_emitted
#if TPP_CONFIG_HAVE___VA_ARGS__
       && (
    !TPPMacro_IS_VA_ARGS_FUNCTION(macro) || argv_iter < argv_end-2
    //|| (argv_iter == argv_end-2 && argv_iter->begin == argv_iter->end)
    )
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
    ) {
    inv_argc_warning_emitted = 1;
    if (TPPWarn_InvalidMacroArgCount(self,token_,macro) == -1)
     goto err0;
   }
   ++argv_iter;
   while (1) {
    argv_iter->end = argv_iter->begin = "";
    if (argv_iter == argv_end-1) break;
    ++argv_iter;
   }
  }
  TPP_ASSERT(argv_iter == argv_end-1);
  total_file_size = macro->m_code_noarg_size;
  for (i = 0; i < argc; ++i) {
   struct TPPMacroArg *argi = argv+i;
   struct TPPMacroArgument *arg = macro->m_argv+i;
   // Add additional space needed by the arguments to the result file size
   total_file_size += ((
    argi->end-argi->begin)*arg->a_references);
   if (arg->a_quote_references != 0) {
    total_file_size += (_TPPFile_GetQuotedStringLen(argi->begin,argi->end)*arg->a_quote_references);
   }
  }
  if ((result = TPPFile_New(total_file_size)) == NULL)
   goto err1;
  _TPPFile_NewSetReferenceFromToken(result,token_);
  TPP_ASSERT(in_str == 0);
#if TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL
  printf("Macro: %s(\n",TPPToken_SStr(token_));
  for (i = 0; i < argc; ++i) {
   printf("\t%.*s",TPP_STATIC_CAST(int,argv[i].end-argv[i].begin),argv[i].begin);
   if (i != argc-1) printf(",");
   printf("\n");
  }
  printf(")\n");
#endif /* TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL */
  {
   char const *kwd_start,*stringize_start,
    *src_iter = macro->m_code->f_data;
   char *dst_iter = result->f_data;
   TPPTokenID kwd_id;
   while (1) {
    switch ((ch = *src_iter++)) {
#if TPP_CONFIG_HAVE_DIGRAPHS
     case '%':
      if (*src_iter != ':') goto write_def;
      ++src_iter;
      // fallthrough
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
     case '#': 
      if (!in_str) { // stringize
#if TPP_CONFIG_HAVE_CHARIZE_OPERATOR
       int is_charize;
#endif /* TPP_CONFIG_HAVE_CHARIZE_OPERATOR */
       stringize_start = src_iter;
#if TPP_CONFIG_HAVE_CHARIZE_OPERATOR
       if (*src_iter == '@') { is_charize = 1; ++src_iter; } else is_charize = 0;
#endif /* TPP_CONFIG_HAVE_CHARIZE_OPERATOR */
       while (TPP_CH_IS_WHSPC(*src_iter)) ++src_iter;
       kwd_start = src_iter;
       while (TPP_CH_IS_ALNUM(*src_iter)) ++src_iter;
       kwd_id = TPPKeywordList_GetL(&self->l_keywords,kwd_start,src_iter,1);
       if (kwd_id == 0) goto err2;
       for (i = 0; i < argc; ++i) {
        if (macro->m_argv[i].a_name == kwd_id) goto quote_arg;
       }
#if TPP_CONFIG_HAVE_DIGRAPHS
       if (ch == '%') {
        *dst_iter++ = '%';
        *dst_iter++ = ':';
       } else
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
       *dst_iter++ = '#';
       memcpy(dst_iter,stringize_start,(src_iter-stringize_start)*sizeof(char));
       dst_iter += (src_iter-stringize_start);
       break;
quote_arg: {
       struct TPPMacroArg *arg = argv+i;
#if TPP_CONFIG_HAVE_CHARIZE_OPERATOR
       char *new_dst = _TPPFile_WriteQuotedString(dst_iter,arg->begin,arg->end);
       if (is_charize) { *dst_iter = '\''; new_dst[-1] = '\''; }
       dst_iter = new_dst;
#else /* TPP_CONFIG_HAVE_CHARIZE_OPERATOR */
       dst_iter = _TPPFile_WriteQuotedString(dst_iter,arg->begin,arg->end);
#endif /* !TPP_CONFIG_HAVE_CHARIZE_OPERATOR */
       }
      } else goto write_def;
      break;
     case '\0':
      TPP_ASSERT(src_iter == macro->m_code->f_end+1);
      goto exp_end;
     case '0': case '1': case '2': case '3':
     case '4': case '5': case '6': case '7':
     case '8': case '9':
      if (in_str) goto write_def;
      kwd_start = src_iter-1;
      while (TPP_CH_IS_ALNUM(*src_iter)) ++src_iter;
      goto noarg;
#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
     case '$':
#endif /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
     case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
     case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
     case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
     case 'y': case 'z':
     case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
     case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
     case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
     case 'Y': case 'Z':
     case '_':ident:
     if (!in_str) {
      // Keyword
      kwd_start = src_iter-1;
      while (TPP_CH_IS_ALNUM(*src_iter)) ++src_iter;
      kwd_id = TPPKeywordList_GetL(&self->l_keywords,kwd_start,src_iter,1);
      if (kwd_id == 0) goto err2;
      for (i = 0; i < argc; ++i) {
       if (macro->m_argv[i].a_name == kwd_id) goto ins_arg;
      }
#if TPP_CONFIG_HAVE___VA_ARGS__ && TPP_CONFIG_HAVE___VA_COMMA__
      if (kwd_id == KWD___VA_COMMA__ && TPPMacro_IS_VA_ARGS_FUNCTION(macro)) {
       // Pretty simple hack
       *dst_iter++ = (argv_end[-1].end == argv_end[-1].begin) ? ' ' : ',';
       goto check_cat;
      }
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ && TPP_CONFIG_HAVE___VA_COMMA__ */
noarg:
      if (cat_tokens) {
       // iterate backward, erasing all written whitespace
       while (TPP_CH_IS_WHSPC(dst_iter[-1])) --dst_iter;
      }
      // Not an argument (just copy it)
      memcpy(dst_iter,kwd_start,(src_iter-kwd_start)*sizeof(char));
      dst_iter += (src_iter-kwd_start);
      cat_tokens = 0;
      goto check_cat;
ins_arg: {
      struct TPPMacroArg *arg = argv+i;
      if (cat_tokens) {
       // iterate backward, erasing all written whitespace
       while (TPP_CH_IS_WHSPC(dst_iter[-1])) --dst_iter;
#if TPP_CONFIG_STRICT_WHITESPACE
       {
        char const *arg_begin = arg->begin;
        while (arg_begin != arg->end) {
         ch = *arg_begin;
         if (!TPP_CH_IS_WHSPC(ch)) break;
         ++arg_begin;
        }
        memcpy(dst_iter,arg_begin,TPP_STATIC_CAST(size_t,arg->end-arg_begin));
        dst_iter += arg->end-arg_begin;
       }
#else /* TPP_CONFIG_STRICT_WHITESPACE */
       memcpy(dst_iter,arg->begin,TPP_STATIC_CAST(size_t,arg->end-arg->begin));
       dst_iter += arg->end-arg->begin;
#endif /* TPP_CONFIG_STRICT_WHITESPACE */
       cat_tokens = 0;
       goto check_cat;
      } else {
       TPPLexerFlags old_flags;
       TPPFileObject *new_result;
       size_t dst_iter_pos;
       TPPFileObject *expanded_file = TPPFile_NewFromString(
        arg->begin,TPP_STATIC_CAST(size_t,arg->end-arg->begin));
       unsigned int old_one_file_rec = self->l_one_file_rec;
       if (expanded_file == NULL) goto err2;
       if (in_file->f_ref_file != NULL) {
        expanded_file->f_ref_file = in_file->f_ref_file;
        // v We must inherit the raw offset here, because this is a recursive macro call
        //   and we would be adding some offset within the macro code itself!
        expanded_file->f_ref_file_off = in_file->f_ref_file_off/*+(arg->begin-in_file->f_data)*/;
       } else {
        expanded_file->f_ref_file = in_file;
        expanded_file->f_ref_file_off = TPP_STATIC_CAST(size_t,arg->begin-in_file->f_data);
       }
       TPPFile_INCREF(expanded_file->f_ref_file);
       old_flags = self->l_flags;
       self->l_flags |= (
        TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_WANT_SPC|
        TPPLexer_FLAG_NO_DIRECTIVES|TPPLexer_FLAG_ONE_FILE);
       TPP_ASSERT(!expanded_file->f_prev);
       expanded_file->f_prev = self->l_files;
       self->l_one_file_rec = 0;
       //TPPFile_INCREF(self->l_files);
       //TPPFile_DECREF(self->l_files);
       self->l_files = expanded_file; // Only 1 file
       TPPFile_INCREF(expanded_file);
       // Must use our own token here
       // >> the given token must remain the same, to allow for the recursion check later!
       {
        TPPTokenObject token2 = TPPToken_INIT();
        while (1) {
         size_t token_size;
         if (TPPLexer_Next(self,&token2) == -1) goto exp_error;
         if (token2.tk_id == TPP_TOK_EOF) break;
#if 0
         if (token2.tk_id == ' ') token2.tk_str_end = token2.tk_str_begin+1; // trim whitespace
#endif
         token_size = TPP_STATIC_CAST(size_t,token2.tk_str_end-token2.tk_str_begin);
         dst_iter_pos = TPP_STATIC_CAST(size_t,dst_iter-result->f_data);
         new_result = TPPFile_Resize(result,TPPFile_SIZE(result)+token_size);
         if (new_result == NULL) goto exp_error;
         dst_iter = (result = new_result)->f_data+dst_iter_pos;
         memcpy(dst_iter,token2.tk_str_begin,token_size);
         dst_iter += token_size;
         TPP_ASSERT(dst_iter <= result->f_end);
        }
        TPP_ASSERT(self->l_one_file_rec == 0);
        TPP_ASSERT(self->l_files == expanded_file);
/*
        if (self->l_files != expanded_file) {
         TPPFileObject *old_file = self->l_files;
         TPP_ASSERT(TPPFile_IS_MACRO_FILE(old_file));
         self->l_files = old_file->f_prev;            
         old_file->f_prev = 0;
         / * Drop the reference from the current file * /
         TPPFile_DECREF(old_file);
        }
*/
        TPPToken_Quit(&token2);
        TPPFile_DECREF(self->l_files);
        //TPPFile_INCREF(expanded_file->f_prev);
        self->l_files = expanded_file->f_prev;
        //TPPFile_DECREF(expanded_file->f_prev);
        expanded_file->f_prev = NULL;
        TPPFile_DECREF(expanded_file);
        self->l_flags = old_flags;
        self->l_one_file_rec = old_one_file_rec;
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4533) /* Initialization of token2 skipped (never used because of the break) */
#endif
check_cat:
#ifdef _MSC_VER
# pragma warning(pop)
#endif
        ch = *src_iter;
        while (TPP_CH_IS_WHSPC(ch)) {
         *dst_iter++ = ch;
         ch = *++src_iter;
        }
        // No Digraph support here, as "%:%:" expands to [#][#] and not [##]
        if (ch == '#' && src_iter[1] == '#') {
         cat_tokens = 1;
         src_iter += 2;
        }
        break;
exp_error:
        TPPToken_Quit(&token2);
       }
       TPPFile_XDECREF(self->l_files);
       //TPPFile_INCREF(expanded_file->f_prev);
       self->l_files = expanded_file->f_prev;
       //TPPFile_DECREF(expanded_file->f_prev);
       expanded_file->f_prev = NULL;
       TPPFile_DECREF(expanded_file);
       self->l_flags = old_flags;
       self->l_one_file_rec = old_one_file_rec;
       goto err2;
      }
      }
     } else {
      goto write_def;
     }
     break;
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
     case '\r':
      if (*src_iter == '\n') ++src_iter;
      // fallthrough
     case '\n': in_str = 0; goto write_def;
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
     case '\\':
      if (in_str) {
       if (*src_iter != '\0') {
        *dst_iter++ = ch;
        ch = *src_iter++;
       }
      }
      goto write_def;
     case '\"': if ((in_str & 0x02) == 0) in_str ^= 0x01; goto write_def;
     case '\'': if ((in_str & 0x01) == 0) in_str ^= 0x02; goto write_def;
     default:
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
      if (TPP_UNLIKELY(TPP_CH_IS_ALPHA_ANSI(ch))) goto ident;
#endif /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
write_def:
      *dst_iter++ = ch;
      break;
    }
   }
exp_end:
   TPP_ASSERT(dst_iter <= result->f_end);
   *dst_iter = '\0';
   result->f_end = dst_iter;
  }
#undef WRITE
  TPP_CALLBACK_FREE_NN(argv);
  return result;
err2:
  TPPFile_DECREF(result);
err1:
  TPP_CALLBACK_FREE_NN(argv);
 }
err0:
 in_file->f_iter = in_file_start;
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// Fixes path seperators '/' and '\\', to conform to the current platform.
// Solves ../ and ./ path references:
// Window: "C:\\Include\\.\\..\\Include/stdio.h" >> "C:\\Include\\stdio.h"
// Unix:   "\\usr\\.\\include\\sys/../stdio.h"   >> "/usr/include/stdio.h"
// NOTE: "filename" must be absolute, and upward folder references
// NOTE: Returns -1, if the filename is impossible
TPP_DECL_STATIC_INLINE(int) _TPPFile_FixFilename(char *filename) {
 char *path_last = NULL;
 char *fix_iter = filename;
 size_t rem;
#if defined(TPP_PLATFORM_WINDOWS)
 while (*fix_iter) { if (*fix_iter == '/') *fix_iter = '\\'; ++fix_iter; }
#else
 while (*fix_iter) { if (*fix_iter == '\\') *fix_iter = '/'; ++fix_iter; }
#endif
 fix_iter = filename;
 while (*fix_iter) {
  if (*fix_iter == TPP_SYS_SEP) {
   if (fix_iter[1] == '.') {
    if (fix_iter[2] == TPP_SYS_SEP) {
     rem = strlen(fix_iter+2);
     TPP_REINTERPRET_CAST(char *,memmove(fix_iter,fix_iter+2,rem))[rem] = '\0';
     goto find_pl;
    } else if (fix_iter[2] == '.' && fix_iter[3] == TPP_SYS_SEP /*&& path_last != NULL*/) {
     rem = strlen(fix_iter+3);
     TPP_REINTERPRET_CAST(char *,memmove(path_last,fix_iter+3,rem))[rem] = '\0';
find_pl: // find_path_last
     fix_iter = path_last;
     while (*--path_last != TPP_SYS_SEP)
      if (path_last == filename) return -1;
     continue;
    } else {
     path_last = fix_iter;
    }
   } else {
    path_last = fix_iter;
   }
  }
  ++fix_iter;
 }
 return 0;
}

#ifdef TPP_PLATFORM_WINDOWS
// Checks, if the spelling of a filename uses the correct case
// >> Used, to emit warnings for incorrect cases under windows
TPP_DECL_STATIC_INLINE(int) TPPLexer_CheckPathSpelling(
 TPPLexerObject const *self, TPPTokenObject const *token_, char const *filename) {
 WIN32_FIND_DATAA find; HANDLE fh; int temp;
 // No need, to go through all of this, if the warning won't get emitted either way
 // NOTE: This only affects the case of '#pragma warning(disable: 61)'
 if (TPPWarnings_GET(&self->l_warnings,61) == 1) return 0; // warning disabled!
 fh = FindFirstFileA(filename,&find);
 if (fh != INVALID_HANDLE_VALUE) {
  char const *filename_end = filename+strlen(filename);
  char const *filename_begin = filename_end-1;
  while (!TPP_IS_DIRSEP(*filename_begin)) if (filename_begin-- == filename) break;
  ++filename_begin;
  if (strcmp(filename_begin,find.cFileName) != 0) {
   size_t correct_file_len = strlen(find.cFileName);
   size_t correct_path_len = (filename_begin-filename);
   char *correct_path = (char *)TPP_CALLBACK_MALLOC_NZ(
    (correct_path_len+correct_file_len+1)*sizeof(char));
   if (correct_path) {
    memcpy(correct_path,filename,correct_path_len);
    memcpy(correct_path+correct_path_len,find.cFileName,correct_file_len);
    correct_path[correct_path_len+correct_file_len] = '\0';
    temp = TPPWarn_InvalidPathCasing(self,token_,filename,correct_path);
    TPP_CALLBACK_FREE_NN(correct_path);
    if (temp == -1) return -1;
   } else {
    if (TPPWarn_InvalidPathCasing(self,token_,filename_begin,find.cFileName) == -1)
     return -1;
   }
  }
  if (!FindClose(fh)) SetLastError(ERROR_SUCCESS);
  return 0;
 } else {
  // Ignore failure here!
  // NOTE: This can happen, if the file doesn't exist
  //    >> Which is detected and handled later, if fopen returns NULL
  SetLastError(ERROR_SUCCESS);
  return 0;
 }
}
#endif /* TPP_PLATFORM_WINDOWS */

// NOTE: This function may modify the contents of "filename"
static TPPFileObject *_TPPLexer_FindAbsInclude(
 TPPLexerObject const *self, TPPTokenObject const *token_, 
 char *filename, TPPLexer_IncludeFlag flags, int *fail_if_cached) {
 TPPFileObject *result;
 if (_TPPFile_FixFilename(filename) == -1)
  return NULL;
#ifdef TPP_PLATFORM_WINDOWS
 if ((flags & TPPLexer_IncludeFlag_DONT_WARN_CASING) == 0) {
  TPP_ASSERT(token_ != NULL);
  if (TPPLexer_CheckPathSpelling(self,token_,filename) == -1) return NULL;
 }
#endif /* TPP_PLATFORM_WINDOWS */
 if ((flags & TPPLexer_IncludeFlag_INCLUDE_NEXT) != 0) {
  result = self->l_files;
  while (result != NULL) {
   if (result->f_name != NULL && TPP_PATHCMP(filename,result->f_name) == 0)
    return NULL; // This one is already being included
   result = result->f_prev;
  }
 }
 if (fail_if_cached != NULL) {
  // Check for a guarded #include file
  TPPIncludeCacheEntry const *cache_entry = TPPIncludeCache_Get(
   TPP_CONST_CAST(TPPIncludeCacheObject *,&self->l_include_cache),filename);
  if (cache_entry && TPPMacroList_Defined(&self->l_macros,cache_entry->ce_guard_name)) {
   TPP_VERBOSE_LOG_SKITPPING_GUARDED_INCLUDE(filename,self,cache_entry->ce_guard_name);
   *fail_if_cached = 1;
   return NULL;
  }
 }
 result = TPPFile_NewFromFileEx(filename,TPPLexer_IncludeFlag_GetFileFormatFlags(flags));
 return result;
}

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPLexer_FindIncludeExS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self,
 TPPTokenObject const *token_, char const *filename, size_t filename_len,
 TPPLexer_IncludeFlag flags, int *fail_if_cached)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 char buf[PATH_MAX];
 TPPFileObject *result;
 TPP_ASSERT(self);
 TPP_ASSERT(filename);
 if (fail_if_cached) *fail_if_cached = 0;
#ifdef TPP_PLATFORM_WINDOWS
 if (filename_len >= 2 && TPP_IS_ABSPATH(filename))
#else /* TPP_PLATFORM_WINDOWS */
 if (filename_len >= 1 && TPP_IS_ABSPATH(filename))
#endif /* !TPP_PLATFORM_WINDOWS */
 {
  memcpy(buf,filename,filename_len);
  buf[filename_len] = '\0';
  result = _TPPLexer_FindAbsInclude(self,token_,buf,flags,fail_if_cached);
 } else {
  if ((flags & TPPLexer_IncludeFlag_SYS_INCLUDE) == 0) {
#ifdef TPP_PLATFORM_WINDOWS
   DWORD cwd_len;
#else /* TPP_PLATFORM_WINDOWS */
   size_t cwd_len;
#endif /* !TPP_PLATFORM_WINDOWS */
   result = TPPLexer_PublicFile(self);
   if (result && result->f_name) {
    // Use the cwd path of the top file
    char *filename_end = result->f_name+(strlen(result->f_name)-1);
    while (!TPP_IS_DIRSEP(*filename_end)) {
     if (--filename_end < result->f_name) {
      result = NULL;
      goto skip_cwd;
     }
    }
#ifdef TPP_PLATFORM_WINDOWS
    cwd_len = TPP_STATIC_CAST(DWORD,(filename_end-result->f_name)+1);
#else
    cwd_len = TPP_STATIC_CAST(size_t,(filename_end-result->f_name)+1);
#endif
    memcpy(buf,result->f_name,cwd_len*sizeof(char));
   } else {
    result = _TPPLexer_FindAbsInclude(self,token_,buf,flags,fail_if_cached);
    if (result != NULL || (fail_if_cached && *fail_if_cached)) goto end;
#ifdef _MSC_VER
#pragma warning(push)
    // The sal annotations of "GetCurrentDirectoryA" say,
    // that the function also fails, is the return value is larger,
    // than the given buffer size.
    // The online documentation say otherwise!
    // >> So we ignore the warnings resulting from that!
#pragma warning(disable:6102)
#pragma warning(disable:6386)
#endif /* _MSC_VER */
    // Check the *real* current directory
#ifdef TPP_PLATFORM_WINDOWS
    cwd_len = GetCurrentDirectoryA(sizeof(buf),buf);
    if (cwd_len == 0 /*|| cwd_len >= sizeof(buf)*/)
#else
    if (getcwd(buf,sizeof(buf)) != NULL) {
     cwd_len = strlen(buf);
    } else
#endif
    {// Fallback: Use a symbolic cwd reference
     buf[0] = '.';
     buf[1] = TPP_SYS_SEP;
     cwd_len = 2;
    }
    if (!TPP_IS_DIRSEP(buf[cwd_len-1])) buf[cwd_len++] = TPP_SYS_SEP;
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
   }
   memcpy(buf+cwd_len,filename,filename_len);
   buf[cwd_len+filename_len] = '\0';
   result = _TPPLexer_FindAbsInclude(self,token_,buf,flags,fail_if_cached);
   if (result != NULL || (fail_if_cached && *fail_if_cached)) goto end;
  } else {
   result = NULL;
  }
skip_cwd:
  {
   size_t i,path_len,
    n_include_paths = self->l_include_list.il_size,
    n_paths = n_include_paths+self->l_sys_include_list.il_size;
   for (i = 0; i < n_paths; ++i) {
    char const *path = i < n_include_paths ? 
     self->l_include_list.il_paths[i] :
     self->l_sys_include_list.il_paths[i-n_include_paths];
    TPP_ASSERT(path);
    path_len = strlen(path);
    memcpy(buf,path,path_len);
    buf[path_len] = TPP_SYS_SEP;
    memcpy(buf+(path_len+1),filename,filename_len);
    buf[path_len+1+filename_len] = '\0';
    result = _TPPLexer_FindAbsInclude(self,token_,buf,flags,fail_if_cached);
    if (result != NULL || (fail_if_cached && *fail_if_cached)) goto end;
   }
  }
 }
end:
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPLexer_IncludeFlag flags)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 int fail_if_cached;
 TPPFileObject *file;
 TPP_ASSERT(self);
 TPP_VERBOSE_LOG_INCLUDE(filename,
                         flags & TPPLexer_IncludeFlag_SYS_INCLUDE,
                         flags & TPPLexer_IncludeFlag_INCLUDE_NEXT);
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
 if ((self->l_flags&TPPLexer_FLAG_TOK_COMMENTS)!=0)
  flags |= TPPLexer_IncludeFlag_KEEP_COMMENTS;
#endif
 file = TPPLexer_FindIncludeEx(self,token_,filename,flags,&fail_if_cached);
 if (file == NULL) return fail_if_cached ? 0 : -1; // Only signal an error, if this isn't wanted behavior
 // Push the file onto the lexer's file stack
 TPPLexer_PushFileInheritedNoErr(self,file);
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_HasIncludeEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPLexer_IncludeFlag flags)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPFileObject *file;
 TPP_ASSERT(self);
 TPP_VERBOSE_LOG_HAS_INCLUDE(filename,sys_include,include_next);
 file = TPPLexer_FindIncludeEx(self,token_,filename,flags,NULL);
 if (file == NULL) return 0;
 TPPFile_DECREF(file);
 return 1;
}

//////////////////////////////////////////////////////////////////////////
// Skips until the next #elif / #else / endif
// >> Also skips pairs of #if[n][def] / #endif recursively
// Return 3, if eof was encountered
// Return 2, if end at #elif
// Return 1, if end at #endif
// Return 0, if end at #else
// Return -1, if an error occurred
TPP_DECL_STATIC_INLINE(int) TPPLexer_SkipPreprocessorBlock(
 TPPLexerObject *self, TPPTokenObject *token_) {
 int recursion = 1; // initial #ifdef
 char ch;
 int result;
 int in_str = 0;
 TPPFileObject *in_file;
 TPPLexerFlags old_flags;
 TPP_ASSERT(self);
 TPP_ASSERT(self->l_files);
 TPP_ASSERT(!TPPFile_IS_MACRO_FILE(self->l_files));
 old_flags = self->l_flags;
 in_file = self->l_files;
 self->l_flags |= (
  TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_NO_MACROS|
  TPPLexer_FLAG_NO_DIRECTIVES|TPPLexer_FLAG_ONE_FILE);
 while (1) {
  ch = TPPFile_Read(in_file);
parse_ch:
  switch (ch) {
   case '\0':eof:
    result = 3;
    goto end;
   case '/':
    // Skip comments
    if (!in_str) {
     ch = TPPFile_Read(in_file);
     if (ch == '/') {
      do {
       ch = TPPFile_Read(in_file);
       if (!ch) goto eof;
      } while (ch != '\n');
     } else if (ch == '*') {
      while (1) {
       ch = TPPFile_Read(in_file);
       if (!ch) goto eof;
       if (ch == '*') {
        ch = TPPFile_Read(in_file);
        if (!ch) goto eof;
        if (ch == '/') break;
       }
      }
     } else goto parse_ch;
    }
    break;
   case '\\':
    if (in_str) TPPFile_Read(in_file);
    break;
#if TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
   case '\n': in_str = 0; break;
#endif /* TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR */
   case '\"': if ((in_str & 0x02) == 0) in_str ^= 0x01; break;
   case '\'': if ((in_str & 0x01) == 0) in_str ^= 0x02; break;
#if TPP_CONFIG_HAVE_DIGRAPHS
   case '%':
    if (TPPFile_Peek(in_file) != ':') break;
    TPPFile_Read(in_file);
    // fallthrough
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
   case '#':
    if (!in_str) {
     if (TPPLexer_NextNoMacro(self,token_) == -1) { result = -1; goto end; }
     switch (token_->tk_id) {
      case TPP_TOK_EOF: goto eof;
      case KWD_else:  if (  recursion == 1) { result = 0; goto end; } break;
      case KWD_elif:  if (  recursion == 1) { result = 2; goto end; } break;
      case KWD_endif: if (--recursion == 0) { result = 1; goto end; } break;
      case KWD_if:
      case KWD_ifdef:
      case KWD_ifndef:
       ++recursion;
       break;
      case KWD_error:
#if TPP_CONFIG_HAVE_WARNING
      case KWD_warning:
#endif /* TPP_CONFIG_HAVE_WARNING */
       // Skip the line, ignoring any incomplete strings
       while (1) {
        ch = TPPFile_Read(in_file);
        if (ch == '\0') goto eof;
        if (ch == '\n') break;
       }
       break;
      default: break;
     }
    }
    break;
   default:
    break;
  }
 }
end:
 self->l_flags = old_flags;
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Parses a #include string.
// NOTE: The caller is responsible for backup and restoration of
//       the lexer flags before and after a call to this function
static int _TPPLexer_ParseIncludeString(
 TPPLexerObject *self, TPPTokenObject *token_,
 char *include_string, char **include_filename,
 int *flags) {
 size_t include_string_len = 0;
 // v We need space tokens and string to not decode their escape codes!
 self->l_flags |= (TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_INC_STRING);
 self->l_flags &= ~TPPLexer_FLAG_NO_MACROS; // Macros are allowed (to some degree...)
 // ** allow macros after "#include"
 do {
  if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
 } while (token_->tk_id == ' ' || token_->tk_id == '\n');
 if (token_->tk_id == TPP_TOK_STR) {
  // Simple case: Normal #include string
  include_string_len = TPPToken_Str(token_,include_string,TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH);
  include_string[include_string_len-1] = '\0';
  *include_filename = include_string+1;
 } else if (token_->tk_id == '<') {
  char *include_writer = include_string;
  while (1) {
   // We can't have macros here, as they would be expanded, that part of the filename is a macro!
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) return -1;
   if (token_->tk_id == '>') break;
   include_writer += TPPToken_Str(token_,
    include_writer,TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH/*-(include_writer-include_string)*/);
  }
  *include_writer = '\0';
  *include_filename = include_string;
  *flags |= TPPLexer_IncludeFlag_SYS_INCLUDE;
 } else {
  if (TPPWarn_ExpectedIncludeString(self,token_) == -1) return -1;
  return -2;
 }
 return 0;
}

TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int)
_TPPLexer_ParseMallocedString(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_,
 TPP_ATTRIBUTE_MSVC_NON_NULL char **str,
 TPP_ATTRIBUTE_MSVC_NON_NULL size_t *str_len,
 TPPTokenID end_token) {
 char buffer[1024];
 char *result_str = NULL;
 size_t buffer_len,result_str_len = 0;
 while (token_->tk_id != end_token) {
  if ((token_->tk_id != TPP_TOK_STR && token_->tk_id != TPP_TOK_CHR) ||
      TPPToken_GetStringType(token_) != TPPStringType_CHAR) {
   if (TPPWarn_UnexpectedToken(self,token_,TPP_TOK_STR) == -1)
    return -1;
  } else {
   buffer_len = TPPToken_EvalStr(token_,buffer,sizeof(buffer));
   if (buffer_len) {
    char *new_result_str = (char *)TPP_CALLBACK_REALLOC_NZ(
     result_str,(result_str_len+buffer_len)*sizeof(char));
    if (!new_result_str) { TPP_CALLBACK_NO_MEMORY(); goto cerr; }
    result_str = new_result_str;
    if (buffer_len > sizeof(buffer)) {
     TPPToken_EvalStr(token_,result_str+result_str_len,buffer_len);
    } else {
     memcpy(result_str+result_str_len,buffer,buffer_len);
    }
    result_str_len += buffer_len;
   }
  }
  if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto cerr;
 }
 if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto cerr;
 *str = result_str;
 *str_len = result_str_len;
 return 0;
cerr:
 TPP_CALLBACK_FREE(result_str);
 return -1;
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4065)
#endif
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_ParsePragma(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, int is_directive)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
#if TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC || TPP_CONFIG_HAVE_PRAGMA_MESSAGE || TPP_CONFIG_HAVE_PRAGMA_ERROR
 size_t msg_len;
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC || TPP_CONFIG_HAVE_PRAGMA_MESSAGE || TPP_CONFIG_HAVE_PRAGMA_ERROR */
 int result = 0; // Known #pragma
 TPP_ASSERT(self);
 TPP_ASSERT(token_);
 (void)is_directive;
 switch (token_->tk_id) {
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
  case KWD_deprecated: {
   char *deprecated_str;
   size_t deprecated_str_len;
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1) ||
       TPPLexer_Skip(self,token_,'(') == -1 ||
       _TPPLexer_ParseMallocedString(self,token_,
       &deprecated_str,&deprecated_str_len,')') == -1
       ) return -1;
   if (deprecated_str) {
    TPPKeywordListHashEntryObject *entry =
     TPPKeywordList_GetEntryS(&self->l_keywords,deprecated_str,deprecated_str_len,1);
    TPP_CALLBACK_FREE_NN(deprecated_str);
    if (!entry) return -1;
    entry->he_flags |= TPPKeywordFlag_IS_DEPRECATED;
   } else TPP_ASSERT(deprecated_str_len == 0);
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
  case KWD_tpp_set_keyword_flags: {
   TPPInteger new_flags;
   char *tpp_kwd_str = NULL,*tpp_used_kwd_str;
   size_t tpp_kwd_str_len = 0;
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1) ||
       TPPLexer_Skip(self,token_,'(') == -1 ||
       _TPPLexer_ParseMallocedString(self,token_,&tpp_kwd_str,&tpp_kwd_str_len,',') == -1) return -1;
   if (TPPLexer_EvalConst1(self,token_,&new_flags) == -1 || 
       TPPLexer_Skip(self,token_,')') == -1) {/*err_kwd_str: */TPP_CALLBACK_FREE(tpp_kwd_str); return -1; }
   if (tpp_kwd_str) {
    TPPKeywordListHashEntryObject *entry;
    tpp_used_kwd_str = tpp_kwd_str;
    while (tpp_kwd_str_len && *tpp_used_kwd_str == '_') --tpp_kwd_str_len,++tpp_used_kwd_str;
    while (tpp_kwd_str_len && tpp_used_kwd_str[tpp_kwd_str_len-1] == '_') --tpp_kwd_str_len;
    entry = TPPKeywordList_GetEntryS(&self->l_keywords,tpp_used_kwd_str,tpp_kwd_str_len,1);
    TPP_CALLBACK_FREE_NN(tpp_kwd_str); 
    if (!entry) return -1;
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
    entry->he_flags = (TPPKeywordFlag)new_flags;
#else /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
    (void)new_flags;
#endif /* !>TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
   } else TPP_ASSERT(tpp_kwd_str_len == 0);
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC
  case KWD_tpp_exec: {
   char *tpp_exec_str = NULL;
   size_t tpp_exec_str_len = 0;
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1) ||
       TPPLexer_Skip(self,token_,'(') == -1 ||
       _TPPLexer_ParseMallocedString(self,token_,
       &tpp_exec_str,&tpp_exec_str_len,')') == -1) return -1;
   if (tpp_exec_str) {
    int temp = TPPLexer_ExecStringS(self,tpp_exec_str,tpp_exec_str_len);
    TPP_CALLBACK_FREE_NN(tpp_exec_str);
    if (TPP_UNLIKELY(temp == -1)) return -1;
   } else TPP_ASSERT(tpp_exec_str_len == 0);
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC */
#if TPP_CONFIG_HAVE_PRAGMA_REGION
  case KWD_region:
  case KWD_endregion:
   break; // Just ignore these 2
#endif /* TPP_CONFIG_HAVE_PRAGMA_REGION */
#if TPP_CONFIG_HAVE_PRAGMA_ONCE
  case KWD_once: {
   // #pragma once
   TPP_ASSERT(!TPPFile_IS_MACRO_FILE(token_->tk_file));
   TPP_ASSERT(token_->tk_file->f_name);
   // We use "__LINE__" as a guard for #pragma once files, as "__LINE__" cannot be #undef'd!
   if (TPPIncludeCache_Set(&self->l_include_cache,token_->tk_file->f_name,KWD___LINE__) == NULL)
    return -1;
   TPP_VERBOSE_LOG_DETECTED_PRAGMA_ONCE(token_->tk_file->f_name);
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_ONCE */
#if TPP_CONFIG_HAVE_PRAGMA_WARNING
  case KWD_warning: {
   TPPInteger id,value;
   int set_default = 0;
   int error;
   // #pragma warning
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1) ||
       TPP_UNLIKELY(TPPLexer_Skip(self,token_,'(') == -1)) return -1;
   if (token_->tk_id == KWD_push) {
    if (TPP_UNLIKELY(TPPWarnings_Push(&self->l_warnings) == -1) ||
        TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
    if (token_->tk_id == ')') return 0;
    if (TPP_UNLIKELY(TPPLexer_Skip(self,token_,',') == -1)) return -1;
   } else if (token_->tk_id == KWD_pop) {
    if (TPPWarnings_Pop(&self->l_warnings) == 1) {
     if (TPPWarn_CantPopWarnings(self,token_) == -1) return -1;
    }
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
    if (token_->tk_id == ')') return 0;
    if (TPP_UNLIKELY(TPPLexer_Skip(self,token_,',') == -1)) return -1;
   }
   if (token_->tk_id == KWD_disable ||
       token_->tk_id == KWD_enable ||
     //token_->tk_id == KWD_warning ||
       token_->tk_id == KWD_error ||
       token_->tk_id == KWD_suppress ||
       token_->tk_id == KWD_default) {
    value = (token_->tk_id == KWD_enable /*|| token_->tk_id == KWD_warning*/) ? TPPWarningMode_Warn : (
            (token_->tk_id == KWD_error) ? TPPWarningMode_Error : TPPWarningMode_Ignored);
    if (token_->tk_id == KWD_default) set_default = 1; else
    if (token_->tk_id == KWD_suppress) set_default = 2;
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   } else {
    if (TPP_UNLIKELY(TPPLexer_EvalConst1(self,token_,&value) == -1)) return -1;
    if (value < -1) value = -1;
   }
   error = TPPLexer_TrySkip(self,token_,':');
   if (TPP_UNLIKELY(error == -1)) return -1;
   if (error == 0) while (1) {
    if (TPP_UNLIKELY(TPPLexer_EvalConst1(self,token_,&id) == -1)) return -1;
    if (id < 0 || id >= TPPWarnings_COUNT) {
     if (TPP_UNLIKELY(TPPWarn_InvalidWarning(self,token_,id) == -1)) return -1;
    } else {
     if (set_default == 1) {
      value = TPPWarnings_GET_DEFAULT(id);
     } else if (set_default == 2) {
      value = TPPWarnings_GET(&self->l_warnings,id)+1;
      if (value < TPPWarningMode_Suppress) value = TPPWarningMode_Suppress;
     }
     TPPWarnings_SET(&self->l_warnings,id,value);
    }
    error = TPPLexer_TrySkip(self,token_,')');
    if (TPP_UNLIKELY(error == -1)) return -1;
    if (error == 0) break;
   }
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_WARNING */
#if TPP_CONFIG_HAVE_PRAGMA_MESSAGE
  case KWD_message: {
   char msg_buffer[1024];
   // #pragma message
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1) ||
       TPP_UNLIKELY(TPPLexer_Skip(self,token_,'(') == -1)) return -1;
   while (token_->tk_id != ')') {
    if ((token_->tk_id != TPP_TOK_STR && token_->tk_id != TPP_TOK_CHR) ||
        TPPToken_GetStringType(token_) != TPPStringType_CHAR) {
     if (TPPWarn_UnexpectedToken(self,token_,TPP_TOK_STR) == -1)
      return -1;
    } else {
     msg_len = TPPToken_EvalStr(token_,msg_buffer,sizeof(msg_buffer));
     if (msg_len > sizeof(msg_buffer)) {
      int temp;
      char *msg_buffer2 = (char *)TPP_CALLBACK_MALLOC_NZ(msg_len*sizeof(char));
      if (!msg_buffer2) { TPP_CALLBACK_NO_MEMORY(); return -1;}
      TPPToken_EvalStr(token_,msg_buffer2,msg_len);
      temp = TPP_CALLBACK_PRAGMA_MESSAGE(self,msg_buffer2,msg_len);
      TPP_CALLBACK_FREE_NN(msg_buffer2);
      if (temp == -1) return -1;
     } else {
      if (TPP_CALLBACK_PRAGMA_MESSAGE(self,msg_buffer,msg_len) == -1) return -1;
     }
    }
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   }
   if (TPP_CALLBACK_PRAGMA_MESSAGE(self,"\n",1) == -1) return -1;
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_MESSAGE */
#if TPP_CONFIG_HAVE_PRAGMA_ERROR
  case KWD_error: {
   char *error_message = NULL;
   char *new_error_message;
   // #pragma error
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   if (TPPLexer_Skip(self,token_,'(') == -1) return -1;
   msg_len = 0;
   while (token_->tk_id != ')') {
    if (token_->tk_id != TPP_TOK_STR || TPPToken_GetStringType(token_) != TPPStringType_CHAR) {
     if (TPPWarn_UnexpectedToken(self,token_,TPP_TOK_STR) == -1)
      return -1;
    } else {
     size_t more_msg_len = TPPToken_EvalStr(token_,NULL,0);
     new_error_message = (char *)TPP_CALLBACK_REALLOC_NZ(error_message,(msg_len+more_msg_len+1)*sizeof(char));
     if (new_error_message == NULL) { TPP_CALLBACK_FREE(error_message); TPP_CALLBACK_NO_MEMORY(); return -1; }
     TPPToken_EvalStr(token_,new_error_message+msg_len,more_msg_len);
     error_message = new_error_message;
     msg_len += more_msg_len;
    }
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   }
   if (error_message) {
    error_message[msg_len] = '\0';
    if (TPPWarn_DirectiveError(self,token_,error_message) == -1) {
     TPP_CALLBACK_FREE_NN(error_message);
     return -1;
    }
    TPP_CALLBACK_FREE_NN(error_message);
   } else {
    if (TPPWarn_DirectiveError(self,token_,"") == -1) return -1;
   }
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_ERROR */
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
  case KWD_push_macro:
  case KWD_pop_macro: {
   // #pragma push_macro()
   // #pragma pop_macro()
   int push_macro = token_->tk_id == KWD_push_macro;
   TPPFileObject *macro_name_file;
   TPPTokenID macro_name;
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   if (TPPLexer_Skip(self,token_,'(') == -1) return -1;
   if (token_->tk_id != TPP_TOK_STR || TPPToken_GetStringType(token_) != TPPStringType_CHAR) {
    if (TPPWarn_ExpectedStringAfterPushMacro(self,token_) == -1) return -1;
   }
   if ((macro_name_file = TPPFile_NewFromStringToken(token_)) == NULL) return -1;
   while (1) { // Keep reading the input string
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto pm_err;
    if (token_->tk_id != TPP_TOK_STR || TPPToken_GetStringType(token_) != TPPStringType_CHAR) break;
    if (TPPFile_AppendStringToken(&macro_name_file,token_) == -1) {
pm_err:
     TPPFile_DECREF(macro_name_file);
     return -1;
    }
   }
   macro_name = TPPKeywordList_GetL(&self->l_keywords,macro_name_file->f_data,macro_name_file->f_end,1);
   TPPFile_DECREF(macro_name_file);
   if (macro_name == 0) return -1;
   if (push_macro) {
    if (TPPMacroList_PushMacro(&self->l_macros,macro_name) == -1) return -1;
   } else if (TPPMacroList_PopMacro(&self->l_macros,macro_name) == 1) {
    if (TPPWarn_CantPopMacro(self,token_,macro_name) == -1) return -1;
   }
   break;
  } break;
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
  default:
   // Unknown #pragma
#if defined(TPP_CALLBACK_PARSE_PRAGMA)
   // Try out the user version
   result = TPP_CALLBACK_PARSE_PRAGMA(self,token_,is_directive);
#else /* TPP_CALLBACK_PARSE_PRAGMA */
   // No user version available
   result = 1;
#endif /* !TPP_CALLBACK_PARSE_PRAGMA */
   break; // Ignore unknown #pragmas
 }
 return result;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


#define TPP_PRIVATE_TO_BOOL_EXPRESSION(x)  ((x)&=TPPInteger_C(0x1))
#define TPP_PRIVATE_IS_BOOL_EXPRESSION(x) (((x)&~TPPInteger_C(0x1))==0)
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalDirectiveBool(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2));


static char const TPPLexer_DateMonthNames[12][4] = {
 "Jan","Feb","Mar","Apr","May","Jun",
 "Jul","Aug","Sep","Oct","Nov","Dec"};
#if TPP_CONFIG_HAVE___TIMESTAMP__
static char const TPPLexer_DateWDayNames[7][4] = {
 "Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
#endif /* TPP_CONFIG_HAVE___TIMESTAMP__ */

//////////////////////////////////////////////////////////////////////////
// This is the main method, where:
//  - Directives are executed
//  - Builtin macros are implemented
//  - User-macros are expanded
//  - User-macro recursion is check and handled
//  - #include guards are detected
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Next(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPTokenObject _temp_token;
 char include_string[TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH]; // TODO: make dynamic
 char ch;
 int base_line;
 TPPInteger ifdef_value;
 TPPFileObject *in_file,*macro_file;
 TPPLexerFlags old_flags;
 size_t intrin_file_off;
again:
 if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) return -1;
#if TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
process:
#endif /* TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */
 switch (token_->tk_id) {

  case TPP_TOK_HASH:
   if ((self->l_flags & TPPLexer_FLAG_NO_DIRECTIVES) == 0 &&
       self->l_files != NULL && !TPPFile_IS_MACRO_FILE(self->l_files) &&
       TPPToken_AtStartOfLine(token_)) {
    old_flags = self->l_flags;
    self->l_flags |= (TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_ONE_REAL_FILE);
    self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC);
    if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
    switch (token_->tk_id) {
     case '!': // Ignore posix execute decl
     case TPP_TOK_LF: // Ignore empty directives
      break;

     case KWD_pragma: {
#define pragma_parse_error base_line
      if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
      self->l_flags &= ~(TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_WANT_SPC
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
                         |TPPLexer_FLAG_NO_DEPRECATED
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
                         );
#if TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS
      pragma_parse_error = TPPLexer_ParsePragma(self,token_,1);
      if (pragma_parse_error == -1) goto dir_err2;
      if (pragma_parse_error == 1) {
       char const **directive_iter;
       TPP_ASSERT(self->l_files);
       TPP_ASSERT(!TPPFile_IS_MACRO_FILE(self->l_files));
       directive_iter = &self->l_files->f_iter;
       while (*--*directive_iter != '#');
       self->l_flags = old_flags;
       return TPPLexer_NextNoMacro(self,token_);
      }
#else /* TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
      if (TPPLexer_ParsePragma(self,token_,1) == -1) goto dir_err2;
#endif /* !TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
#undef pragma_parse_error
     } break;

     case KWD_line: {
#define line_off ifdef_value
      base_line = TPPToken_Line(token_)+2;
      if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
      if (TPPLexer_EvalConst1(self,token_,&line_off) == -1) {
       if (TPPWarn_ExpectedIntAfterLine(self,token_) == -1) goto dir_err;
       goto dir_skip;
      }
      if (token_->tk_id == TPP_TOK_STR && TPPToken_GetStringType(token_) == TPPStringType_CHAR) {
       char *new_name_str;
       size_t str_len = TPPToken_EvalStr(token_,NULL,0);
       if (str_len) {
        if ((new_name_str = (char *)TPP_CALLBACK_MALLOC_NZ((str_len+1)*sizeof(char))) == NULL) { TPP_CALLBACK_NO_MEMORY(); goto dir_err2; }
        TPPToken_EvalStr(token_,new_name_str,str_len);
        new_name_str[str_len] = '\0';
       } else new_name_str = NULL;
       TPP_ASSERT(token_->tk_file);
       TPP_CALLBACK_FREE(token_->tk_file->f_new_name);
       token_->tk_file->f_new_name = new_name_str; // Inherit string
      } else if (token_->tk_id != '\n') {
       if (TPPWarn_ExpectedStringAfterLine(self,token_) == -1) goto dir_err;
       goto dir_skip;
      }
      TPP_ASSERT(!TPPFile_IS_MACRO_FILE(token_->tk_file));
      // Update the line offset of the current file
      token_->tk_file->f_line_offset = TPP_STATIC_CAST(int,line_off-base_line);
#undef line_off
     } break;
#define include_flags base_line
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
     case KWD_include_next:
      include_flags = TPPLexer_IncludeFlag_INCLUDE_NEXT|TPPLexer_IncludeFlag_FORMAT_FILE;
      goto inc_dir;
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
#if TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
     case KWD_import:
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */
     case KWD_include: include_flags = TPPLexer_IncludeFlag_NONE|TPPLexer_IncludeFlag_FORMAT_FILE;
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
inc_dir:
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
     {
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT || \
    TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
      TPPTokenID include_mode = token_->tk_id;
#else /* ... */
#     define include_mode KWD_include
#endif /* !... */
      char *include_filename;
      switch (_TPPLexer_ParseIncludeString(
       self,token_,include_string,&include_filename,&include_flags)) {
       case -1: return -1;
       case -2: goto dir_skip;
       default: break;
      }
      while (token_->tk_id != TPP_TOK_LF && token_->tk_id != TPP_TOK_EOF) {
       if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2; // Skip rest of the line
      }
      // Actually include the file
#if TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
      in_file = self->l_files;
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */
      if (TPPLexer_IncludeEx(self,token_,include_filename,include_flags) == -1) {
       if ((include_flags & TPPLexer_IncludeFlag_SYS_INCLUDE) != 0) {
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
        if (TPPWarn_SysIncludeFileNotFound(self,token_,include_filename,include_mode == KWD_include_next) == -1) goto dir_err;
#else /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
        if (TPPWarn_SysIncludeFileNotFound(self,token_,include_filename) == -1) goto dir_err;
#endif /* !TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
       } else {
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
        if (TPPWarn_IncludeFileNotFound(self,token_,include_filename,include_mode == KWD_include_next) == -1) goto dir_err;
#else /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
        if (TPPWarn_IncludeFileNotFound(self,token_,include_filename) == -1) goto dir_err;
#endif /* !TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
       }
      }
#if TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
      if (self->l_files != in_file && include_mode == KWD_import) {
       if (TPPIncludeCache_Set(&self->l_include_cache,self->l_files->f_name,KWD___LINE__) == NULL) goto dir_err;
      }
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */
      goto dir_end;
#undef include_flags
#ifdef include_mode
#     undef include_mode
#endif /* include_mode */
     } break;

#if TPP_CONFIG_HAVE_WARNING
     case KWD_warning:
#endif /* TPP_CONFIG_HAVE_WARNING */
     case KWD_error: {
      char const *message = token_->tk_str_end;
      char *message_end;
#define directive_is_critical base_line
      while (*message && TPP_CH_IS_WHSPC(*message)) ++message;
      message_end = TPP_CONST_CAST(char *,message);
      while (*message_end != '\r' && *message_end != '\n' && *message_end != '\0') ++message_end;
      ch = *message_end;
      *message_end = '\0';
#if TPP_CONFIG_HAVE_WARNING
      if (token_->tk_id == KWD_warning) {
       directive_is_critical = TPPWarn_DirectiveWarning(self,token_,message) == -1;
      } else 
#endif /* TPP_CONFIG_HAVE_WARNING */
       directive_is_critical = TPPWarn_DirectiveError(self,token_,message) == -1;
      *message_end = ch;
      //TPP_ASSERT(token_->tk_str_file == self->l_files);
      token_->tk_str_file->f_iter = message_end;
      if (directive_is_critical) goto dir_err;
#undef directive_is_critical
     } break;
     case KWD_if: {
      _temp_token = *token_; // NOT A real copy of the token
      self->l_flags &= ~(TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_ONE_REAL_FILE);
      if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto dir_err2; // Skip the line
      if (_TPPLexer_EvalDirectiveBool(self,token_,&ifdef_value) == -1) goto dir_err;
      self->l_flags |= TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_ONE_REAL_FILE;
      while (token_->tk_id != TPP_TOK_LF && token_->tk_id != TPP_TOK_EOF) {
       if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2; // Skip the line
      }
      ifdef_value = !!ifdef_value;
      goto push_if;
     } break;
     case KWD_ifdef:
     case KWD_ifndef: {
      ifdef_value = token_->tk_id == KWD_ifndef;
      _temp_token = *token_; // NOT A real copy of the token
      if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
      if (!TPPTokenID_IS_KEYWORD(token_->tk_id)) {
       if (TPPWarn_ExpectedKeywordAfterIfdef(self,token_) == -1) goto dir_err;
       goto dir_skip;
      }
      ifdef_value ^= TPPMacroList_Defined(&self->l_macros,token_->tk_id);
push_if:
      if (TPPIfdefStack_Push(&self->l_ifdef_stack,TPP_STATIC_CAST(int,ifdef_value),&_temp_token) == -1) return -1;
      goto check_skip;
     } break;
     case KWD_else: {
      if (TPPIfdefStack_EMPTY(&self->l_ifdef_stack)) {
       if (TPPWarn_ElseWithoutIfdef(self,token_) == -1) goto dir_err;
      }
      if (TPPIfdefStack_TOP(&self->l_ifdef_stack) & 2) {
       if (TPPWarn_ElseAfterElse(self,token_) == -1) goto dir_err;
      }
do_else:
      ifdef_value = (TPPIfdefStack_TOP(&self->l_ifdef_stack) ^= 3);
      goto check_skip;
     } break;
     case KWD_elif: {
do_elif:
      if (TPPIfdefStack_EMPTY(&self->l_ifdef_stack)) {
       if (TPPWarn_ElifWithoutIfdef(self,token_) == -1) goto dir_err;
      }
      ifdef_value = TPPIfdefStack_TOP(&self->l_ifdef_stack);
      if (ifdef_value > 1) {
       if (TPPWarn_ElifAfterElse(self,token_) == -1) goto dir_err;
      }
      if (ifdef_value == 1) goto do_skip;
      self->l_flags &= ~(TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_ONE_REAL_FILE);
      if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto dir_err2; // Skip the line
      if (_TPPLexer_EvalDirectiveBool(self,token_,&ifdef_value) == -1) goto dir_err;
      self->l_flags |= (TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_ONE_REAL_FILE);
      while (token_->tk_id != TPP_TOK_LF && token_->tk_id != TPP_TOK_EOF) {
       if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2; // Skip the line
      }
      ifdef_value = !!ifdef_value;
      TPPIfdefStack_TOP(&self->l_ifdef_stack) = TPP_STATIC_CAST(int,ifdef_value);
check_skip:
      if (!(ifdef_value & 1)) {
do_skip:
       // Skip the preprocessor block
       switch (TPPLexer_SkipPreprocessorBlock(self,token_)) {
        case -1: goto dir_err2;
        case 0: goto do_else;
        case 1: goto do_endif;
        case 2: goto do_elif;
        case 3: {
         TPPFileObject *old_file = self->l_files;
         TPP_ASSERT(!TPPFile_IS_MACRO_FILE(old_file));
         if (!TPPIfdefStack_EMPTY(&self->l_ifdef_stack)) {
          while (1) {
           TPPTokenObject *ifdef_tk = TPPIfdefStack_TOP_TOKEN(&self->l_ifdef_stack);
           if (ifdef_tk->tk_file != old_file) break;
           if (TPPWarn_IfdefWithoutEndif(self,ifdef_tk) == -1) goto dir_err2;
           TPPIfdefStack_POP(&self->l_ifdef_stack);
          }
         }
         /* Pop file from stack */
         self->l_files = old_file->f_prev;
         old_file->f_prev = 0;
         /* Drop the reference from the current file */
         TPPFile_DECREF(old_file);
        } break;
        default: break;
       }
      }
     } break;
     case KWD_endif: {
do_endif:
      if (TPPIfdefStack_EMPTY(&self->l_ifdef_stack) ||
          TPPIfdefStack_TOP_TOKEN(&self->l_ifdef_stack)->tk_file != token_->tk_file) {
       if (TPPWarn_EndifWithoutIfdef(self,token_) == -1) goto dir_err;
       goto dir_skip;
      }
#if 1
      // Check, if the #ifndef token was really the first token of the file
      // and that this #endif is the last token of the file
      // NOTE: We do however allow "#pragma once" before the #ifndef token
      TPP_ASSERT(token_->tk_file);
      //TPP_ASSERT(token_->tk_file->f_name);
      if (
#if 1
       token_->tk_file->f_name && // Real file
#endif
       self->l_ifdef_stack.s_size == 1 && // top #ifdef scope
       self->l_ifdef_stack.s_stack[0].e_token.tk_id == KWD_ifndef && // started with #ifndef
       (self->l_ifdef_stack.s_stack[0].e_value & 2) == 0 && // no #else in scope
       TPPIncludeCache_Get(&self->l_include_cache,token_->tk_file->f_name) == NULL // file not guarded (yet)
       ) {
       char const *old_file_iter;
       TPPTokenObject guard_check_token = TPPToken_INIT();
       old_file_iter = (in_file = self->l_files)->f_iter;
       // Skip ignored tokens
       if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
       // Skip the line
       while (guard_check_token.tk_id != TPP_TOK_LF && guard_check_token.tk_id != TPP_TOK_EOF) {
        if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
       }
       // Skip the empty lines
       while (guard_check_token.tk_id == TPP_TOK_LF) {
        if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
       }
       //old_flags = self->l_flags;
       if (guard_check_token.tk_id == TPP_TOK_EOF) {
        // Ok. This is the last token of the file
        // Now to check, if "ifdef_start" was the first token (but allow "#pragma once" to come before)
        TPPTokenObject *ifdef_start = &self->l_ifdef_stack.s_stack[0].e_token;
        TPPFile_REWIND(in_file);
        self->l_flags &= ~(TPPLexer_FLAG_WANT_LF);
        if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
        if (guard_check_token.tk_id != TPP_TOK_HASH) goto no_guard;
        if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
        // Derp! If there has already been a #pragma once, we would even be here!
#if 0 && TPP_CONFIG_HAVE_PRAGMA_ONCE
        // Skip #pragma once directives at the start of the file
        while (guard_check_token.tk_id == KWD_pragma) {
         if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
         if (guard_check_token.tk_id != KWD_once) goto no_guard;
         self->l_flags |= TPPLexer_FLAG_WANT_LF;
         do {
          if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
         } while (guard_check_token.tk_id != TPP_TOK_EOF && guard_check_token.tk_id != '\n');
         self->l_flags &= ~TPPLexer_FLAG_WANT_LF;
         if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
         if (guard_check_token.tk_id != TPP_TOK_HASH) goto no_guard;
         if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
        }
#endif /* TPP_CONFIG_HAVE_PRAGMA_ONCE */
        // v This is the *real* check: If we actually meet
        //   up with the start of the first #ifdef, this
        //   is really a #include guard
        if (guard_check_token.tk_str_begin == ifdef_start->tk_str_begin) {
         // Yes! This is a #include guard!
         TPP_ASSERT(!TPPFile_IS_MACRO_FILE(in_file));
         in_file->f_iter = ifdef_start->tk_str_end;
         if (TPPLexer_NextNoMacro(self,&guard_check_token) == -1) goto guard_err2;
         in_file->f_iter = old_file_iter;
         TPP_ASSERT(TPPTokenID_IS_KEYWORD(guard_check_token.tk_id));
         // The guard_name token is now loaded in "token_"
         TPP_VERBOSE_LOG_DETECTED_INCLUDE_GUARD(in_file->f_name,self,guard_check_token.tk_id);
         if (TPPIncludeCache_Set(&self->l_include_cache,in_file->f_name,guard_check_token.tk_id) == NULL)
          goto guard_err;
        }
       }
no_guard:
       //self->l_flags = old_flags;
       in_file->f_iter = old_file_iter;
       TPPToken_Quit(&guard_check_token);
       TPPIfdefStack_POP(&self->l_ifdef_stack);
       goto dir_skip;
guard_err:
       self->l_flags = old_flags;
       in_file->f_iter = old_file_iter;
       TPPToken_Quit(&guard_check_token);
       TPPIfdefStack_POP(&self->l_ifdef_stack);
       goto dir_err;
guard_err2:
       self->l_flags = old_flags;
       in_file->f_iter = old_file_iter;
       TPPToken_Quit(&guard_check_token);
       TPPIfdefStack_POP(&self->l_ifdef_stack);
       goto dir_err2;
      } else 
#endif
      {
       TPPIfdefStack_POP(&self->l_ifdef_stack);
      }
     } break;
     case KWD_undef: {
      if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
      if (!TPPTokenID_IS_KEYWORD(token_->tk_id)) {
       if (TPPWarn_ExpectedKeywordAfterUndef(self,token_) == -1) goto dir_err;
       goto dir_skip;
      }
      switch (TPPMacroList_Undef(&self->l_macros,token_->tk_id)) {
       case -1: if (TPPWarn_MacroDoesntExist(self,token_) == -1) goto dir_err; break;
       case -2: if (TPPWarn_CantUndefBuiltinMacro(self,token_) == -1) goto dir_err; break;
       default: break;
      }
     } break;
     case KWD_define: {
      int def_error;
      TPPTokenID macro_name;
      TPPMacroObject *new_macro;
      TPPFileObject *macro_code;
      char const *macro_code_begin;
      char const *macro_code_end;
      in_file = self->l_files;
      if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
      if (!TPPTokenID_IS_KEYWORD(token_->tk_id)) {
       if (TPPWarn_ExpectedKeywordAfterDefine(self,token_) == -1) goto dir_err;
       goto dir_skip;
      } else if (TPP_UNLIKELY(!TPPTokenID_IS_USER_KEYWORD(token_->tk_id))) {
       if (TPP_UNLIKELY(TPPWarn_RedefiningBuiltinKeyword(self,token_) == -1)) goto dir_err;
      }
      macro_name = token_->tk_id;
      ch = TPPFile_Read(in_file);
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
      if (ch == '(' || ch == '[' || ch == '{' || ch == '<')
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
      if (ch == '(')
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
      {
       // Macro function
       TPPTokenID argv[TPP_CONFIG_MACRO_MAX_ARGC];
       size_t argc = 0;
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
       TPPTokenID va_args_name = 0;
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
       unsigned char is_va_args = 0;
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
       size_t i;
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
       TPPMacroCallingConvention call_conv;
#define macro_rparen _TPPMacroCallingConvRChars[call_conv]
       switch (ch) {
        case '<': call_conv = TPPMacroCallingConvention_ANGLE; break;
        case '{': call_conv = TPPMacroCallingConvention_BRACE; break;
        case '[': call_conv = TPPMacroCallingConvention_BRACKET; break;
        default:  call_conv = TPPMacroCallingConvention_PAREN; break;
       }
#else /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
#define macro_rparen ')'
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
       if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err;
       if (token_->tk_id != macro_rparen) {
        while (1) {
         if (TPP_UNLIKELY(!TPPTokenID_IS_KEYWORD(token_->tk_id))) {
#if TPP_CONFIG_HAVE___VA_ARGS__
          if (TPP_UNLIKELY(token_->tk_id == TPP_TOK_DOTS)) {
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
           va_args_name = KWD___VA_ARGS__;
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
           is_va_args = 1;
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
           if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err;
           if (TPP_UNLIKELY(token_->tk_id != macro_rparen))
            if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterVaArgs(self,token_) == -1)) goto dir_err;
           argv[argc++] = KWD___VA_ARGS__;
           break;
          } else
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
          {
           if (TPP_UNLIKELY(TPPWarn_ExpectedKeywordForMacroArgument(self,token_) == -1)) goto dir_err;
          }
         }
#if TPP_CONFIG_HAVE___VA_ARGS__
         if (token_->tk_id == KWD___VA_ARGS__) {
          if (TPPWarn_VaArgsUsedAsMacroParameter(self,token_) == -1) goto dir_err;
         } else
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE___VA_COMMA__
         if (token_->tk_id == KWD___VA_COMMA__) {
          if (TPPWarn_VaCommaUsedAsMacroParameter(self,token_) == -1) goto dir_err;
         } else
#endif /* TPP_CONFIG_HAVE___VA_COMMA__ */
         {
          for (i = 0; i < argc; ++i) if (argv[i] == token_->tk_id) {
           if (TPPWarn_ReusedMacroParameter(self,token_) == -1) goto dir_err;
           break;
          }
         }
         argv[argc++] = token_->tk_id;
         if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err;
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
         if (TPP_UNLIKELY(token_->tk_id == TPP_TOK_DOTS)) { // Named __VA_ARGS__
          if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err;
          va_args_name = argv[argc-1];
         }
#endif /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
         if (token_->tk_id == macro_rparen) break;
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
         if (va_args_name)
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
         if (is_va_args)
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
         {
          if (TPP_UNLIKELY(TPPWarn_VaArgsMustBeLastParameter(self,token_) == -1)) goto dir_err;
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
          va_args_name = 0;
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
          is_va_args = 0;
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
         }
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
         if (TPP_UNLIKELY(token_->tk_id != ',')) {
          if (TPP_UNLIKELY(TPPWarn_ExpectedCommaOrRParenForMacroFuncDecl(self,token_) == -1)) goto dir_err;
         }
         if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err;
        }
       }
       ch = TPPFile_Read(in_file);
       while (ch != '\n' && TPP_CH_IS_WHSPC(ch)) ch = TPPFile_Read(in_file);
       macro_code_begin = in_file->f_iter-1;
       while (ch != '\n' && ch != '\0') ch = TPPFile_Read(in_file); // Read the macro
       macro_code_end = ch ? in_file->f_iter-1 : in_file->f_iter;
       while (macro_code_end != macro_code_begin) {
        // Strip whitespace from the macro end
        ch = macro_code_end[-1];
        if (!TPP_CH_IS_WHSPC(ch) && (ch != '\\' || (
         *macro_code_end != '\r' && *macro_code_end != '\n'
         ))) break;
        --macro_code_end;
       }
       // Create the file used, to store the macro code
       if (TPP_UNLIKELY((macro_code = TPPFile_New(TPP_STATIC_CAST(size_t,macro_code_end-macro_code_begin))) == NULL))
        goto dir_err;
       // Create the macro
       if (TPP_UNLIKELY((new_macro = TPPMacro_NewFunction(argc)) == NULL)) {
        TPPFile_DECREF(macro_code);
        goto dir_err;
       }
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
       new_macro->m_call_conv = call_conv;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
       new_macro->m_va_args_name = va_args_name;
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
       new_macro->m_is_va_args = is_va_args;
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
       memcpy(macro_code->f_data,macro_code_begin,TPP_STATIC_CAST(size_t,macro_code_end-macro_code_begin));
       _TPPFile_Format(macro_code,TPPFileFormatFlag_FMT_LINE_WRAP); // Remove line wraps from macro code
       //TPPFile_XDECREF(new_macro->m_code);
       new_macro->m_code = macro_code; // consume reference
       //TPPFile_INCREF(new_macro->m_code);
       //TPPFile_DECREF(macro_code);
       // Fill in the macro argument name information
       for (i = 0; i < argc; ++i) new_macro->m_argv[i].a_name = argv[i];
       if (TPP_UNLIKELY(_TPPMacro_CalculateArgumentRefs(new_macro,self) == -1)) {
        TPPMacro_Free(new_macro);
        goto dir_err;
       }
      } else {
       // Keyword macro
       while (ch != '\n' && TPP_CH_IS_WHSPC(ch)) ch = TPPFile_Read(in_file);
       macro_code_begin = in_file->f_iter-1;
       while (ch != '\n' && ch != '\0') ch = TPPFile_Read(in_file); // Read the macro
       macro_code_end = ch == '\0' ? in_file->f_iter : in_file->f_iter-1;
       while (macro_code_end != macro_code_begin) {
        // Strip whitespace from the macro end
        ch = macro_code_end[-1];
        if (!TPP_CH_IS_WHSPC(ch) && (ch != '\\' || (
         *macro_code_end != '\r' && *macro_code_end != '\n'
         ))) break;
        --macro_code_end;
       }
       if (TPP_UNLIKELY((macro_code = TPPFile_New(TPP_STATIC_CAST(size_t,macro_code_end-macro_code_begin))) == NULL))
        goto dir_err;
       if (TPP_UNLIKELY((new_macro = TPPMacro_New()) == NULL)) {
        TPPFile_DECREF(macro_code);
        goto dir_err;
       }
       memcpy(macro_code->f_data,macro_code_begin,TPP_STATIC_CAST(size_t,macro_code_end-macro_code_begin));
       _TPPFile_Format(macro_code,TPPFileFormatFlag_FMT_LINE_WRAP); // Remove line wraps from macro code
       //TPPFile_XDECREF(new_macro->m_code);
       new_macro->m_code = macro_code; // consume reference
       //TPPFile_INCREF(new_macro->m_code);
       //TPPFile_DECREF(macro_code);
      }
#undef macro_rparen
#if TPP_CONFIG_NAME_MACRO_FILES
      TPPFile_SetNewName(new_macro->m_code,TPPKeywordList_NameOf(&self->l_keywords,macro_name));
#endif /* TPP_CONFIG_NAME_MACRO_FILES */
      def_error = TPPMacroList_Add(&self->l_macros,macro_name,new_macro);
      if (TPP_UNLIKELY(def_error == -1)) {
       TPPMacro_Free(new_macro);
       goto dir_err;
      } else if (TPP_UNLIKELY(def_error == 1)) {
       if (TPPWarn_MacroAlreadyDefined(self,token_,new_macro,macro_name) == -1) goto dir_err;
      }
#if TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE
      if ((self->l_flags & TPPLexer_FLAG_WANT_LF) != 0)
       TPPFile_Undo(in_file);
#endif /* TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE */
      goto dir_end;
     } break;
     default:
      if (TPP_LIKELY((self->l_flags & TPPLexer_FLAG_ASM_COMMENTS) == 0)) {
       // Only warn about unknown directive, if we aren't supposed, to parse asm directives!
       if (TPP_UNLIKELY(TPPWarn_UnknownPreprocessorDirective(self,token_) == -1))
        goto dir_err;
      }
      break;
    }
dir_skip:
    while (token_->tk_id != TPP_TOK_LF && token_->tk_id != TPP_TOK_EOF) {
     // Skip the line
     if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
    }
dir_end:
    self->l_flags = old_flags;
#if 0
    if (token_->tk_id == TPP_TOK_EOF && self->l_files) {
     if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_ONE_FILE) != 0)) {
      if (TPP_UNLIKELY(self->l_one_file_rec != 0)) { --self->l_one_file_rec; goto do_pop; }
     } else {
      if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_ONE_REAL_FILE) == 0 ||
         TPPFile_IS_MACRO_FILE(self->l_files))) { // Can only pop macro file
do_pop:
       _TPPLexer_PopFileEx(self,TPPFile_DECREF,{return -1;});
       goto again;
      }
     }
    }
#if TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE
    else
#endif /* TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE */
#endif
#if TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE
    if ((old_flags & TPPLexer_FLAG_WANT_LF) != 0 &&
             token_->tk_id == TPP_TOK_LF) {
     return 0;
    }
#endif /* TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE */
    goto again;
dir_err: // Skip the line
    while (token_->tk_id != TPP_TOK_LF && token_->tk_id != TPP_TOK_EOF) {
     if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto dir_err2;
    }
dir_err2:
    self->l_flags = old_flags;
    return -1;
   }
   break;
  case KWD__Pragma: {
#define pragma_parse_error base_line
   unsigned int old_one_file_rec;
   char const *pragma_start = token_->tk_str_begin;
   intrin_file_off = token_->tk_file_off;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_ONE_FILE|TPPLexer_FLAG_NO_DIRECTIVES);
   old_one_file_rec = self->l_one_file_rec;
   self->l_one_file_rec = 0;
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto prgm_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) {
    if (TPPWarn_ExpectedLParenAfterPragma(self,token_) == -1) goto prgm_cerr;
    goto prgm_uk2;
   }
   // Allow macros during string token parsing
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto prgm_cerr;
   if (TPP_LIKELY(token_->tk_id == TPP_TOK_STR && TPPToken_GetStringType(token_) == TPPStringType_CHAR)) {
    macro_file = TPPFile_NewFromStringToken(token_);
    if (TPP_UNLIKELY(macro_file == NULL)) goto prgm_cerr;
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto prgm_cerr2;
    while (token_->tk_id == TPP_TOK_STR && TPPToken_GetStringType(token_) == TPPStringType_CHAR) {
     // Read all the strings
     if (TPP_UNLIKELY(TPPFile_AppendStringToken(&macro_file,token_) == -1)) goto prgm_cerr2;
     if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto prgm_cerr2;
    }
   } else {
    self->l_flags = old_flags;
    self->l_one_file_rec = old_one_file_rec;
    if (TPP_UNLIKELY(token_->tk_id != ')')) {
     // Allow (and ignore) empty "_Pragma()"
     if (TPP_UNLIKELY(TPPWarn_ExpectedStringAfterPragma(self,token_) == -1)) goto prgm_cerr;
     goto prgm_uk;
    }
    goto again;
   }
   if (TPP_UNLIKELY(token_->tk_id != ')')) {
    if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterPragma(self,token_) == -1)) goto prgm_cerr2;
    TPPFile_DECREF(macro_file);
prgm_uk2:
    self->l_flags = old_flags;
    self->l_one_file_rec = old_one_file_rec;
    goto prgm_uk;
   }
#if TPP_CONFIG_NAME_MACRO_FILES
   TPPFile_SetNewName(macro_file,"_Pragma");
#endif /* TPP_CONFIG_NAME_MACRO_FILES */
   // No macros / directives inside of "_Pragma"
   self->l_flags |= (TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_NO_DIRECTIVES);
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
   self->l_flags &= ~TPPLexer_FLAG_NO_DEPRECATED;
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
   TPP_ASSERT(macro_file->f_ref_file == NULL);
   macro_file->f_ref_file = self->l_files->f_ref_file ? self->l_files->f_ref_file : self->l_files;
   macro_file->f_ref_file_off = intrin_file_off;
   TPPFile_INCREF(macro_file->f_ref_file);
   TPPLexer_PushFileInheritedNoErr(self,macro_file);
   TPP_VERBOSE_LOG_PARSING_STRING_PRAGMA(token_,macro_file->f_data);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto prgm_late_cerr; // Parse the first pragma token
   // evaluate the pragma
#if TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS
   pragma_parse_error = TPPLexer_ParsePragma(self,token_,0);
   if (TPP_UNLIKELY(pragma_parse_error == -1)) goto prgm_late_cerr;
#else /* TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
   if (TPP_UNLIKELY(TPPLexer_ParsePragma(self,token_,0) == -1)) goto prgm_late_cerr;
#endif /* !TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
   TPP_ASSERT(self->l_files == macro_file);
   self->l_files = macro_file->f_prev;
   macro_file->f_prev = NULL;
   TPPFile_DECREF(macro_file);
   self->l_flags = old_flags;
   self->l_one_file_rec = old_one_file_rec;
#if TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS
   if (TPP_UNLIKELY(pragma_parse_error == 1))
#else/* TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
   if (0)
#endif /* !TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
   {
prgm_uk:
    // Unknown pragma
    TPP_ASSERT(self->l_files &&
               pragma_start >= self->l_files->f_data &&
               pragma_start < self->l_files->f_end &&
               "TPPLexer_ParsePragma() Signaled an unknown pragma, but "
               "made unknown changed to the #include stack "
               ">> Can't Revert those changes!");
    // Restore the file pointer to the "_Pragma" keyword and read it again
    self->l_files->f_iter = pragma_start;
    return TPPLexer_NextNoMacro(self,token_); // This will return a "_Pragma" token
   }
   goto again;
prgm_late_cerr:
   self->l_files = macro_file->f_prev;
   macro_file->f_prev = NULL;
   self->l_flags = old_flags;
   self->l_one_file_rec = old_one_file_rec;
   TPPFile_DECREF(macro_file);
   return -1;
prgm_cerr2:
   TPPFile_DECREF(macro_file);
prgm_cerr:
   return -1;
#undef pragma_parse_error
  } break;
  // Builtin Macros
#if TPP_CONFIG_HAVE___TPP_COUNTER
  case KWD___TPP_COUNTER: {
   TPPTokenID counter_id;
   TPPKeywordListHashEntryObject *kwd_entry;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto ppcnt_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParenAfterTPPCounter(self,token_) == -1)) goto ppcnt_cerr; goto ppcnt_err; }
   // The keyword is allowed, to be a macro
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ppcnt_cerr;
   TPP_VERBOSE_LOG_TPP_COUNTER(TPPToken_SStr(token_));
   counter_id = token_->tk_id;
   if (TPP_UNLIKELY(!TPPTokenID_IS_KEYWORD(counter_id))) { if (TPP_UNLIKELY(TPPWarn_ExpectedKeywordAfterTPPCounter(self,token_) == -1)) goto ppcnt_cerr; goto ppcnt_err; }
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ppcnt_cerr;
   if (TPP_UNLIKELY(token_->tk_id != ')')) { if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterTPPCounter(self,token_) == -1)) goto ppcnt_cerr; goto ppcnt_err; }
   kwd_entry = TPPKeywordList_FindEntry(&self->l_keywords,counter_id);
   if (TPP_UNLIKELY(kwd_entry == NULL)) return -1; // can happen, unless critical error
   macro_file = TPPFile_NewFromStringf("%u",kwd_entry->he_counter++);
   self->l_flags = old_flags;
   goto bi_macro;
ppcnt_err:
   macro_file = TPPFile_NewFromString("0",1);
   self->l_flags = old_flags;
   goto bi_macro;
ppcnt_cerr:
   self->l_flags = old_flags;
   return -1;
  } break;
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
#if TPP_CONFIG_HAVE___TPP_EVAL
  case KWD___TPP_EVAL: {
#define ppevl_err ppcnt_err
#define ppevl_cerr ppcnt_cerr
   TPPInteger eval_result;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto ppevl_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParenAfterTPPEval(self,token_) == -1)) goto ppevl_cerr; goto ppevl_err; }
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;
   // Allow "," in expression
   if (TPP_UNLIKELY(TPPLexer_EvalConst(self,token_,&eval_result) == -1)) goto ppevl_cerr;
   if (TPP_UNLIKELY(token_->tk_id != ')')) {
    if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterTPPEval(self,token_) == -1)) goto ppevl_cerr;
    goto ppevl_err;
   }
   macro_file = TPPFile_NewFromStringf(TPPInteger_PRINTF_FMT,eval_result);
   self->l_flags = old_flags;
   goto bi_macro;
#undef ppevl_cerr
#undef ppevl_err
  } break;
#endif /* TPP_CONFIG_HAVE___TPP_EVAL */
#if TPP_CONFIG_HAVE___TPP_RANDOM
  case KWD___TPP_RANDOM: {
#define pprnd_err ppcnt_err
#define pprnd_cerr ppcnt_cerr
   TPPInteger low,high,rnd;
   long double rnd_float;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto pprnd_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParenAfterTPPRandom(self,token_) == -1)) goto pprnd_cerr; goto pprnd_err; }
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto pprnd_cerr;
   if (TPP_UNLIKELY(TPPLexer_EvalConst1(self,token_,&low) == -1)) { if (TPP_UNLIKELY(TPPWarn_ExpectedIntegerAfterTPPRandom1(self,token_) == -1)) goto pprnd_cerr; goto pprnd_err; }
   if (token_->tk_id == ')') {
    high = low;
    low = 0;
   } else {
    if (TPP_UNLIKELY(token_->tk_id != ',')) { if (TPP_UNLIKELY(TPPWarn_ExpectedCommaAfterTPPRandom(self,token_) == -1)) goto pprnd_cerr; goto pprnd_err; }
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto pprnd_cerr;
    if (TPP_UNLIKELY(TPPLexer_EvalConst1(self,token_,&high) == -1)) {
     if (TPP_UNLIKELY(TPPWarn_ExpectedIntegerAfterTPPRandom2(self,token_) == -1)) goto pprnd_cerr; goto pprnd_err; }
    if (TPP_UNLIKELY(token_->tk_id != ')')) { if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterTPPRandom(self,token_) == -1)) goto pprnd_cerr; goto pprnd_err; }
   }
   self->l_flags = old_flags;
   if (TPP_UNLIKELY((self->l_flags & TPPLexer_FLAG_RAND_INIT) == 0)) {
#if defined(TPP_PLATFORM_WINDOWS)
    LARGE_INTEGER x;
    if (!QueryPerformanceCounter(&x))
#if (_WIN32_WINNT >= 0x0600)
     x.QuadPart = TPP_STATIC_CAST(LONGLONG,GetTickCount64());
#else
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28159)
#endif
     x.LowPart = GetTickCount();
     // NOTE: We intentionally leave "HighPart" in an undefined state
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#endif
    srand(x.LowPart ^ x.HighPart);
#else
    srand((unsigned int)time(NULL));
#endif
    self->l_flags |= TPPLexer_FLAG_RAND_INIT;
   }
   rnd_float = TPP_STATIC_CAST(long double,rand())/TPP_STATIC_CAST(long double,RAND_MAX);
   rnd = low+TPP_STATIC_CAST(TPPInteger,rnd_float*(1+high-low));
   macro_file = TPPFile_NewFromStringf(TPPInteger_PRINTF_FMT,rnd);
   goto bi_macro;
#undef pprnd_cerr
#undef pprnd_err
  } break;
#endif /* TPP_CONFIG_HAVE___TPP_RANDOM */
#if TPP_CONFIG_HAVE___TPP_STR_AT || TPP_CONFIG_HAVE___TPP_STR_SUBSTR
#if TPP_CONFIG_HAVE___TPP_STR_AT
  case KWD___TPP_STR_AT:
#endif /* TPP_CONFIG_HAVE___TPP_STR_AT */
#if TPP_CONFIG_HAVE___TPP_STR_SUBSTR
  case KWD___TPP_STR_SUBSTR:
#endif /* TPP_CONFIG_HAVE___TPP_STR_SUBSTR */
  {
#define sstr_err  ppcnt_err
#define sstr_cerr ppcnt_cerr
#if TPP_CONFIG_HAVE___TPP_STR_AT && TPP_CONFIG_HAVE___TPP_STR_SUBSTR
   int strat_want_chr = token_->tk_id == KWD___TPP_STR_AT;
#endif
   char *strat_str;
   size_t strat_strlen;
   TPPInteger temp;
   TPPUInteger str_at,str_at_n;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPPLexer_NextNoMacro(self,token_) == -1) goto sstr_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParen(self,token_) == -1)) goto sstr_cerr; goto sstr_err; }
   if (TPPLexer_Next(self,token_) == -1) goto sstr_cerr;
   if (_TPPLexer_ParseMallocedString(self,token_,&strat_str,&strat_strlen,',') == -1) goto sstr_cerr;
   if (TPPLexer_EvalConst1(self,token_,&temp) == -1) goto sstr_cerr2;
   if (strat_strlen) {
    temp %= (ptrdiff_t)strat_strlen;
    str_at = (temp < 0) ? temp+(TPPInteger)strat_strlen : temp;
   } else str_at = 0;
   if (TPPLexer_TrySkip(self,token_,',') == 0) {
    if (TPPLexer_EvalConst1(self,token_,&temp) == -1) goto sstr_cerr2;
    if (strat_strlen) {
     temp %= (ptrdiff_t)strat_strlen;
     str_at_n = (temp < 0) ? temp+(TPPInteger)strat_strlen : temp;
    } else str_at_n = 0;
   } else str_at_n = strat_strlen ? 1 : 0;
   if (str_at_n >= strat_strlen-str_at) str_at_n = strat_strlen-str_at;
   if (TPP_UNLIKELY(token_->tk_id != ')')) { if (TPP_UNLIKELY(TPPWarn_ExpectedRParen(self,token_) == -1)) goto sstr_cerr2; goto sstr_err2; }
   macro_file = TPPFile_NewFromQuotedStringL(strat_str+(size_t)str_at,(size_t)str_at_n);
   if (macro_file == NULL) goto sstr_cerr2;
#if TPP_CONFIG_HAVE___TPP_STR_AT
#if TPP_CONFIG_HAVE___TPP_STR_SUBSTR
   if(strat_want_chr)
#endif /* TPP_CONFIG_HAVE___TPP_STR_SUBSTR */
   {
    ((char *)macro_file->f_data)[0] = '\'';
    ((char *)macro_file->f_end)[-1] = '\'';
   }
#endif /* TPP_CONFIG_HAVE___TPP_STR_AT */
   TPP_CALLBACK_FREE(strat_str);
   self->l_flags = old_flags;
   goto bi_macro2;
sstr_cerr2: TPP_CALLBACK_FREE(strat_str); goto sstr_cerr;
sstr_err2: TPP_CALLBACK_FREE(strat_str); goto sstr_err;
  } break;
#undef sstr_cerr
#undef sstr_err
#endif /* TPP_CONFIG_HAVE___TPP_STR_AT || TPP_CONFIG_HAVE___TPP_STR_SUBSTR */
#if TPP_CONFIG_HAVE___TPP_STR_PACK
  case KWD___TPP_STR_PACK: {
#define spck_err  ppcnt_err
#define spck_cerr ppcnt_cerr
   TPPInteger chr;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPPLexer_NextNoMacro(self,token_) == -1) goto spck_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParen(self,token_) == -1)) goto spck_cerr; goto spck_err; }
   if (TPPLexer_Next(self,token_) == -1) goto spck_cerr;
   if ((macro_file = TPPFile_New(2)) == NULL) goto spck_cerr;
   while (TPP_UNLIKELY(token_->tk_id != ')')) {
    size_t old_size,more_len;
    TPPFileObject *new_file;
    if (TPPLexer_EvalConst1(self,token_,&chr) == -1) goto spck_cerr2;
    if (TPP_UNLIKELY(token_->tk_id == ',')) if (TPPLexer_Next(self,token_) == -1) goto spck_cerr2;
    old_size = TPPFile_SIZE(macro_file);
    more_len = 1;
    //more_len = _TPPFile_GetQuotedCharLen(chr);
    new_file = TPPFile_Resize(macro_file,old_size+more_len);
    if (new_file == NULL) goto spck_cerr2;
    macro_file = new_file;
    macro_file->f_data[old_size-1] = (char)chr;
    //TODO: WriteChar
   }
   ((char *)macro_file->f_data)[0] = '\"';
   ((char *)macro_file->f_end)[-1] = '\"';
   self->l_flags = old_flags;
   goto bi_macro2;
spck_cerr2: TPPFile_DECREF(macro_file); goto spck_cerr;
//spck_err2: TPPFile_DECREF(macro_file); goto spck_err;
#undef spck_cerr
#undef spck_err
  } break;
#endif /* TPP_CONFIG_HAVE___TPP_STR_PACK */
#if TPP_CONFIG_HAVE___TPP_STR_SIZE
#define ssz_err  ppcnt_err
#define ssz_cerr ppcnt_cerr
  case KWD___TPP_STR_SIZE: {
   TPPInteger str_size = 0;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPPLexer_NextNoMacro(self,token_) == -1) goto ssz_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPP_UNLIKELY(TPPWarn_ExpectedLParen(self,token_) == -1)) goto ssz_cerr; goto ssz_err; }
   if (TPPLexer_Next(self,token_) == -1) goto ssz_cerr;
   while (token_->tk_id != ')') {
    if ((token_->tk_id != TPP_TOK_STR && token_->tk_id != TPP_TOK_CHR) ||
        TPPToken_GetStringType(token_) != TPPStringType_CHAR) {
     if (TPPWarn_UnexpectedToken(self,token_,TPP_TOK_STR) == -1)
      goto ssz_cerr;
     goto ssz_err;
    } else {
     str_size += TPPToken_EvalStr(token_,NULL,0);
    }
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ssz_cerr;
   }
   self->l_flags = old_flags;
   macro_file = TPPFile_NewFromStringf(TPPInteger_PRINTF_FMT,str_size);
   goto bi_macro;
  } break;
#undef ssz_cerr
#undef ssz_err
#endif /* TPP_CONFIG_HAVE___TPP_STR_SIZE */
#if TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
  case KWD___TPP_STR_DECOMPILE: {
   TPPFileObject *intrin_file;
   intrin_file = token_->tk_file;
   intrin_file_off = token_->tk_file_off;
   TPPFile_XINCREF(intrin_file);
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) {
ppsdc_cerr:
    self->l_flags = old_flags;
    TPPFile_XDECREF(intrin_file);
    return -1;
   }
   if (TPP_UNLIKELY(token_->tk_id != '(')) {
    if (TPPWarn_ExpectedLParenAfterTPPStrDecompile(self,token_) == -1) goto ppsdc_cerr;
ppsdc_err:
    TPPFile_XDECREF(intrin_file);
    self->l_flags = old_flags;
    goto process;
   }
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ppsdc_cerr;
   if (TPP_LIKELY((token_->tk_id == TPP_TOK_STR || token_->tk_id == TPP_TOK_CHR) && TPPToken_GetStringType(token_) == TPPStringType_CHAR)) {
    if (TPP_UNLIKELY((macro_file = TPPFile_NewFromStringToken(token_)) == NULL)) goto ppsdc_cerr;
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ppsdc_cerr;
    while ((token_->tk_id == TPP_TOK_STR || token_->tk_id == TPP_TOK_CHR) && TPPToken_GetStringType(token_) == TPPStringType_CHAR) {
     // Read all the strings
     if (TPP_UNLIKELY(TPPFile_AppendStringToken(&macro_file,token_) == -1)) goto ppsdc_cerr;
     if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto ppsdc_cerr;
    }
    if (TPP_UNLIKELY(token_->tk_id != ')')) {
     TPPFile_DECREF(macro_file);
     if (TPP_UNLIKELY(TPPWarn_ExpectedRParenAfterTPPStrDecompile(self,token_) == -1)) goto ppsdc_cerr;
     goto ppsdc_err;
    }
   } else {
    if (TPP_UNLIKELY(token_->tk_id != ')')) {
     // Allow (and ignore) empty "__TPP_STR_DECOMPILE()"
     if (TPP_UNLIKELY(TPPWarn_ExpectedStringAfterTPPStrDecompile(self,token_) == -1)) goto ppsdc_cerr;
    }
    TPPFile_XDECREF(intrin_file);
    self->l_flags = old_flags;
    goto again;
   }
   self->l_flags = old_flags;
   macro_file->f_ref_file_off = intrin_file_off;
   TPP_ASSERT(macro_file->f_ref_file == NULL);
   // TPPFile_XDECREF(macro_file->f_ref_file);
   // TPPFile_XINCREF(intrin_file);
   macro_file->f_ref_file = intrin_file;
   // TPPFile_XDECREF(intrin_file);
   goto bi_macro3;
  } break;
#endif /* TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */
#if TPP_CONFIG_HAVE___TPP_LOAD_FILE
#define include_flags base_line
  case KWD___TPP_LOAD_FILE: {
#define pplf_cerr ppcnt_cerr
   char *include_filename;
   include_flags = TPPLexer_IncludeFlag_NONE;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto pplf_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPPWarn_UnexpectedToken(self,token_,'(') == -1) goto pplf_cerr; goto pplf_err; }
   switch (_TPPLexer_ParseIncludeString(
    self,token_,include_string,&include_filename,&include_flags)) {
    case -1: goto pplf_cerr;
    case -2: 
pplf_err:
     self->l_flags = old_flags;
     macro_file = TPPFile_NewFromString("\"\"",2);
     goto bi_macro;
    default: break;
   }
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_INC_STRING);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto pplf_cerr;
   if (token_->tk_id == ',') {
    TPPInteger check_next;
    if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) goto pplf_cerr;
    if (TPPLexer_EvalConst1(self,token_,&check_next) == -1) goto pplf_cerr;
    if (TPP_UNLIKELY(TPPWarn_UnexpectedToken(self,token_,')') == -1)) goto pplf_cerr;
    if (check_next) include_flags |= TPPLexer_IncludeFlag_INCLUDE_NEXT;
   }
   if (TPP_UNLIKELY(token_->tk_id != ')')) {
    if (TPP_UNLIKELY(TPPWarn_UnexpectedToken(self,token_,')') == -1)) goto pplf_cerr;
   }
   // Load the file (reuse the "in_file" variable)
   in_file = TPPLexer_FindIncludeEx(self,token_,include_filename,include_flags,NULL);
   if (TPP_UNLIKELY(in_file == NULL)) {
    if (TPP_UNLIKELY(TPPWarn_TPPLoadFileNotFound(self,token_,include_filename,
     (include_flags & TPPLexer_IncludeFlag_SYS_INCLUDE) != 0,
     (include_flags & TPPLexer_IncludeFlag_INCLUDE_NEXT) != 0) == -1))
     goto pplf_cerr;
    goto pplf_err;
   }
   macro_file = TPPFile_NewFromQuotedStringL(in_file->f_data,TPPFile_SIZE(in_file));
   if (TPP_UNLIKELY(macro_file == NULL)) { TPPFile_DECREF(in_file); goto pplf_cerr; }
   TPPFile_DECREF(in_file);
   self->l_flags = old_flags;
   goto bi_macro2;
#undef pplf_cerr
  }
#undef include_flags
#endif /* TPP_CONFIG_HAVE___TPP_LOAD_FILE */
#if TPP_CONFIG_HAVE___is_deprecated || \
    TPP_CONFIG_HAVE___is_identifier || \
    TPP_CONFIG_HAVE___is_builtin_identifier || \
    TPP_CONFIG_HAVE___has_attribute || \
    TPP_CONFIG_HAVE___has_builtin || \
    TPP_CONFIG_HAVE___has_cpp_attribute || \
    TPP_CONFIG_HAVE___has_declspec_attribute || \
    TPP_CONFIG_HAVE___has_extension || \
    TPP_CONFIG_HAVE___has_feature
#if TPP_CONFIG_HAVE___is_deprecated
  case KWD___is_deprecated:
#endif /* TPP_CONFIG_HAVE___is_deprecated */
#if TPP_CONFIG_HAVE___is_identifier
  case KWD___is_identifier:
#endif /* TPP_CONFIG_HAVE___is_identifier */
#if TPP_CONFIG_HAVE___is_builtin_identifier
  case KWD___is_builtin_identifier:
#endif /* TPP_CONFIG_HAVE___is_builtin_identifier */
#if TPP_CONFIG_HAVE___has_attribute
  case KWD___has_attribute:
#endif /* TPP_CONFIG_HAVE___has_attribute */
#if TPP_CONFIG_HAVE___has_builtin
  case KWD___has_builtin:
#endif /* TPP_CONFIG_HAVE___has_builtin */
#if TPP_CONFIG_HAVE___has_cpp_attribute
  case KWD___has_cpp_attribute:
#endif /* TPP_CONFIG_HAVE___has_cpp_attribute */
#if TPP_CONFIG_HAVE___has_declspec_attribute
  case KWD___has_declspec_attribute:
#endif /* TPP_CONFIG_HAVE___has_declspec_attribute */
#if TPP_CONFIG_HAVE___has_extension
  case KWD___has_extension:
#endif /* TPP_CONFIG_HAVE___has_extension */
#if TPP_CONFIG_HAVE___has_feature
  case KWD___has_feature:
#endif /* TPP_CONFIG_HAVE___has_feature */
  {
   TPPTokenID trait_id = token_->tk_id;
   TPPKeywordListHashEntryObject *trait_entry;
   int trait_value;
   char const *ident_start;
#define ppii_err      ppcnt_err
#define ppii_cerr     ppcnt_cerr
#define ident_buf     include_string
#define ident_buf_len intrin_file_off
#define ident_rec     base_line
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto ppii_cerr;
   if (TPP_UNLIKELY(token_->tk_id != '(')) { if (TPPWarn_ExpectedLParen(self,token_) == -1) goto ppii_cerr; goto ppii_err; }
   ident_buf_len = 0;
   ident_rec = 1;
   while (1) {
    ch = TPPFile_Read(self->l_files);
    if (TPP_UNLIKELY(ch == '(')) ++ident_rec; else
    if (TPP_UNLIKELY(ch == ')')) if (--ident_rec == 0) break;
    ident_buf[ident_buf_len++] = ch;
   }
   TPPFile_Undo(self->l_files);
#if 0
   ident_buf[ident_buf_len] = '\0';
#endif
   ident_start = ident_buf;
   while (ident_buf_len && TPP_CH_IS_WHSPC(*ident_start)) { ++ident_start; --ident_buf_len; }
   while (ident_buf_len && TPP_CH_IS_WHSPC(ident_start[ident_buf_len-1])) --ident_buf_len;
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto ppii_cerr;
   if (TPP_UNLIKELY(token_->tk_id != ')')) { if (TPPWarn_ExpectedRParen(self,token_) == -1) goto ppii_cerr; goto ppii_err; }
   self->l_flags = old_flags;
#if TPP_CONFIG_HAVE___is_builtin_identifier
   if (trait_id == KWD___is_builtin_identifier) {
    trait_id = TPPKeywordList_GetS(&self->l_keywords,ident_buf,ident_buf_len,0);
    trait_value = trait_id && trait_id != 0 && trait_id < TPP_TOK_USER_IDENT_START;
   } else
#endif /* TPP_CONFIG_HAVE___is_builtin_identifier */
#if TPP_CONFIG_HAVE___is_identifier
   if (trait_id == KWD___is_identifier) {
    trait_value = TPPKeywordList_GetS(&self->l_keywords,ident_buf,ident_buf_len,0) != 0;
   } else
#endif /* TPP_CONFIG_HAVE___is_identifier */
#if TPP_CONFIG_HAVE___is_deprecated
   if (trait_id == KWD___is_deprecated) {
    trait_entry = TPPKeywordList_GetEntryS(&self->l_keywords,ident_buf,ident_buf_len,0);
    trait_value = trait_entry ? ((trait_entry->he_flags & TPPKeywordFlag_IS_DEPRECATED) != 0) : 0;
   } else
#endif /* TPP_CONFIG_HAVE___is_deprecated */
#if TPP_CONFIG_HAVE___has_attribute || \
    TPP_CONFIG_HAVE___has_builtin || \
    TPP_CONFIG_HAVE___has_cpp_attribute || \
    TPP_CONFIG_HAVE___has_declspec_attribute || \
    TPP_CONFIG_HAVE___has_extension || \
    TPP_CONFIG_HAVE___has_feature
   {
    while (ident_buf_len && *ident_start == '_') { ++ident_start; --ident_buf_len; }
    while (ident_buf_len && ident_start[ident_buf_len-1] == '_') --ident_buf_len;
    trait_entry = TPPKeywordList_GetEntryS(&self->l_keywords,ident_start,ident_buf_len,0);
    if (trait_entry) {
     switch (trait_id) {
#if TPP_CONFIG_HAVE___has_attribute
      case KWD___has_attribute:
       trait_value = (trait_entry->he_flags & TPPKeywordFlag_HAS_ATTRIBUTE) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_attribute */
#if TPP_CONFIG_HAVE___has_builtin
      case KWD___has_builtin:
       trait_value = (trait_entry->he_flags & TPPKeywordFlag_HAS_BUILTIN) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_builtin */
#if TPP_CONFIG_HAVE___has_cpp_attribute
      case KWD___has_cpp_attribute:
       trait_value = (trait_entry->he_flags & TPPKeywordFlag_HAS_CPP_ATTRIBUTE) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_cpp_attribute */
#if TPP_CONFIG_HAVE___has_declspec_attribute
      case KWD___has_declspec_attribute:
       trait_value = (trait_entry->he_flags & TPPKeywordFlag_HAS_DECLSPEC_ATTRIBUTE) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_declspec_attribute */
#if TPP_CONFIG_HAVE___has_extension
      case KWD___has_extension:
       trait_value = (trait_entry->he_flags & TPPKeywordFlag_HAS_EXTENSION) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_extension */
#if TPP_CONFIG_HAVE___has_feature
      case KWD___has_feature:
       trait_value = (trait_entry->he_flags & (
                      TPPKeywordFlag_HAS_FEATURE
#if TPP_CONFIG_HAVE___has_extension && \
   !defined(TPP_CONFIG_EXTENSIONS_ARE_NOT_FEATURES)
                      // Extensions are features
                      |TPPKeywordFlag_HAS_EXTENSION
#endif /* TPP_CONFIG_HAVE___has_extension */
                      )) != 0;
       break;
#endif /* TPP_CONFIG_HAVE___has_feature */
      default: trait_value = 0; break; // Shouldn't happen!
     }
    } else trait_value = 0;
#else /* TPP_CONFIG_HAVE___has_... */
    {
     trait_value = 0;
    }
#endif /* !TPP_CONFIG_HAVE___has_... */
   }
   macro_file = TPPFile_NewFromStringf("%d",trait_value);
   goto bi_macro;
#undef ident_rec
#undef ident_buf_len
#undef ident_buf
#undef ppii_cerr
#undef ppii_err
  } break;
#endif /* ... */

#if TPP_CONFIG_HAVE___has_include
#define include_flags base_line
  case KWD___has_include: include_flags = TPPLexer_IncludeFlag_NONE; goto int_hi;
  case KWD___has_include_next: include_flags = TPPLexer_IncludeFlag_INCLUDE_NEXT;
int_hi: {
#define pphi_err  ppcnt_err
#define pphi_cerr ppcnt_cerr
   char *include_filename;
   old_flags = self->l_flags;
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_WANT_LF);
   self->l_flags |= (TPPLexer_FLAG_NO_DIRECTIVES);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto pphi_cerr;
   if (token_->tk_id != '(') { if (TPPWarn_ExpectedLParenAfterHasInclude(self,token_) == -1) goto pphi_cerr; goto pphi_err; }
   switch (_TPPLexer_ParseIncludeString(
    self,token_,include_string,&include_filename,&include_flags)) {
    case -1: goto pphi_cerr;
    case -2: goto pphi_err;
    default: break;
   }
   self->l_flags &= ~(TPPLexer_FLAG_WANT_SPC|TPPLexer_FLAG_INC_STRING);
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) goto pphi_cerr;
   if (token_->tk_id != ')') {
    if (TPPWarn_ExpectedRParenAfterHasInclude(self,token_) == -1) goto pphi_cerr;
   }
   self->l_flags = old_flags;
   // Actually include the file
   macro_file = TPPFile_NewFromStringf(
    "%d",TPP_STATIC_CAST(int,TPPLexer_HasIncludeEx(self,token_,include_filename,include_flags)));
   goto bi_macro;
#undef pphi_cerr
#undef pphi_err
  }
#undef include_flags
#endif /* TPP_CONFIG_HAVE___has_include */
#if TPP_CONFIG_HAVE___COUNTER__
  case KWD___COUNTER__:
   macro_file = TPPFile_NewFromStringf("%u",self->l_counter++);
   goto bi_macro;
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
#if TPP_CONFIG_HAVE___INCLUDE_DEPTH__
  case KWD___INCLUDE_LEVEL__:
  case KWD___INCLUDE_DEPTH__:
   macro_file = TPPFile_NewFromStringf("%d",TPPLexer_PublicIncludeDepth(self));
   goto bi_macro;
#endif /* TPP_CONFIG_HAVE___INCLUDE_DEPTH__ */
  case KWD___LINE__:
   macro_file = TPPFile_NewFromStringf("%d",TPPToken_Line(token_)+1);
   goto bi_macro;
#if TPP_CONFIG_HAVE___BASE_FILE__
  case KWD___BASE_FILE__: {
   macro_file = TPPLexer_PublicBaseFile(self);
   macro_file = TPPFile_NewFromQuotedString(TPPFile_NAME(macro_file));
   goto bi_macro;
  }
#endif /* TPP_CONFIG_HAVE___BASE_FILE__ */
#if TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS
  case KWD___TIME_SEC__:
  case KWD___TIME_MIN__:
  case KWD___TIME_HOUR__:
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS */
#if TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS
  case KWD___DATE_DAY__:
  case KWD___DATE_WDAY__:
  case KWD___DATE_YDAY__:
  case KWD___DATE_MONTH__:
  case KWD___DATE_YEAR__:
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS */
  case KWD___TIME__:
  case KWD___DATE__: {
   time_t ti;
   struct tm *tm;
   time(&ti);
   tm = localtime(&ti);
   switch (token_->tk_id) {
#if TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS
    case KWD___TIME_SEC__:   macro_file = TPPFile_NewFromStringf("%d",tm->tm_sec); goto bi_macro;
    case KWD___TIME_MIN__:   macro_file = TPPFile_NewFromStringf("%d",tm->tm_min); goto bi_macro;
    case KWD___TIME_HOUR__:  macro_file = TPPFile_NewFromStringf("%d",tm->tm_hour); goto bi_macro;
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS */
#if TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS
    case KWD___DATE_DAY__:   macro_file = TPPFile_NewFromStringf("%d",tm->tm_mday); goto bi_macro;
    case KWD___DATE_WDAY__:  macro_file = TPPFile_NewFromStringf("%d",tm->tm_wday); goto bi_macro;
    case KWD___DATE_YDAY__:  macro_file = TPPFile_NewFromStringf("%d",tm->tm_yday); goto bi_macro;
    case KWD___DATE_MONTH__: macro_file = TPPFile_NewFromStringf("%d",tm->tm_mon+1); goto bi_macro;
    case KWD___DATE_YEAR__:  macro_file = TPPFile_NewFromStringf("%d",tm->tm_year+1900); goto bi_macro;
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS */
    case KWD___DATE__: macro_file = TPPFile_NewFromStringf("\"%.3s %2d %d\"",TPPLexer_DateMonthNames[tm->tm_mon],tm->tm_mday,tm->tm_year+1900); goto bi_macro;
    case KWD___TIME__: macro_file = TPPFile_NewFromStringf("\"%02d:%02d:%02d\"",tm->tm_hour,tm->tm_min,tm->tm_sec); goto bi_macro;
    default: TPP_BUILTIN_UNREACHABLE();
   }
  } break;
#if TPP_CONFIG_HAVE___TIMESTAMP__
  case KWD___TIMESTAMP__: {
   struct stat attrib;
   // Reuse the macro_file data slot
   macro_file = TPPLexer_PublicFile(self);
   if (stat(macro_file->f_name,&attrib) == 0) {
    time_t ti = attrib.st_mtime; // Modification time
    struct tm *tm = localtime(&ti);
    // NOTE: macro_file will always have a size of 25 bytes
    macro_file = TPPFile_NewFromStringf(
     "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
     TPPLexer_DateWDayNames[tm->tm_wday],
     TPPLexer_DateMonthNames[tm->tm_mon],
     tm->tm_mday,tm->tm_hour,tm->tm_min,
     tm->tm_sec,1900+tm->tm_year);
   } else {
    macro_file = TPPFile_NEW_FROM_STRING("??? ??? ?? ??:??:?? ????");
   }
   goto bi_macro;
  } break;
#endif /* TPP_CONFIG_HAVE___TIMESTAMP__ */
  case KWD___FILE__: {
   macro_file = TPPLexer_PublicFile(self);
   macro_file = TPPFile_NewFromQuotedString(TPPFile_NAME(macro_file));
bi_macro: // builtin_macro
   if (macro_file == NULL) return -1;
#if TPP_CONFIG_HAVE___TPP_LOAD_FILE || \
    TPP_CONFIG_HAVE___TPP_STR_AT || \
    TPP_CONFIG_HAVE___TPP_STR_SUBSTR || \
    TPP_CONFIG_HAVE___TPP_STR_PACK
bi_macro2:
#endif /* ... */
#if TPP_CONFIG_NAME_MACRO_FILES
   if (macro_file) TPPFile_SetNewName(macro_file,TPPLexer_TokenIDStr(self,token_->tk_id));
#endif /* TPP_CONFIG_NAME_MACRO_FILES */
   _TPPFile_NewSetReferenceFromToken(macro_file,token_);
#if TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
bi_macro3:
#endif /* TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */
   if (self->l_flags & TPPLexer_FLAG_ONE_FILE) ++self->l_one_file_rec;
   TPPLexer_PushFileInheritedNoErr(self,macro_file);
   goto again;
  } break;

  default: {
   // NOTE: using "TPPTokenID_IS_USER_KEYWORD" here would result in
   //       no builtin keywords being expanded as macros.
   if (TPPTokenID_IS_KEYWORD(token_->tk_id) && (self->l_flags & TPPLexer_FLAG_NO_MACROS) == 0) {
#if TPP_CONFIG_NAME_MACRO_FILES
    TPPTokenID macro_name = token_->tk_id;
#endif /* TPP_CONFIG_NAME_MACRO_FILES */
    TPPMacroObject *macro = TPPMacroList_LOOKUP(&self->l_macros,token_->tk_id);
    if (macro != NULL
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
        && TPPMacro_DEFINED(macro)
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
        ) {
     TPP_ASSERT(macro->m_code != NULL);
     if (TPPMacro_HAS_ARGS(macro) /*&& self->l_files != NULL*/) {
      ch = TPPLexer_PeekSpace(self);
      if (ch == TPP_STATIC_CAST(char,TPP_MACRO_GET_LPAREN_CHAR(macro))) {
       char const *old_iter;
       struct TPPFileListObject *popped_files = NULL;
       old_iter = _TPPLexer_SkipSpaceAndGetPoppedFiles(self,&popped_files);
       // Expand the macro argument list and generate
       old_flags = self->l_flags;
       self->l_flags |= (TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_WANT_SPC);
       // TODO: Expand "_TPPLexer_CreateExpandedMacroFunction",
       //       to use accept "popped_files", thus allowing:
       //       >> #define b(x) ~x
       //       >> #define a b(
       //       >> a 42)
       //       // s.a.: "pp_unsupported.h"
       macro_file = _TPPLexer_CreateExpandedMacroFunction(self,macro,token_);
       self->l_flags = old_flags;
       TPP_ASSERT(
        self->l_files != NULL &&
        old_iter >= self->l_files->f_data &&
        old_iter < self->l_files->f_end &&
        "_TPPLexer_CreateExpandedMacroFunction didn't leave "
        "the lexer with the same top file");
       if (macro_file == NULL) {
        if (popped_files) TPPFileList_Free(popped_files);
        return -1;
       }
       if (TPPLexer_ExpandingEquivalentMacroFile(self,macro_file)) {
        TPPFile_DECREF(macro_file);
        // NOTE: A way, to get here:
        // >> #define func2   func
        // >> #define func(a) func2(a)
        // >> func(42)
        // Explanation:
        //  1. In call to func(42)
        //  2. In expansion of func2:
        //  3. Popped file of macro "func2", while searching for '('
        //  4. Found arguments for macro "func2": "(42)"
        //  5. Generated expanded macro file for "func2" with "(42)":
        //     >> "func2(42)"
        //  6. Equivalent macro is already being expanded
        //  7. Can't recover file with the "func" token (popped in step 3)
        // Solution:
        //  "TPPLexer_SkipSpace" must somehow give us a list of all the
        //  files it removed, so we can restore that list here!
        //  NOTE: We can't use the intrusive links of "f_prev", as that
        //        would prevent keyword-style macro expansion, because
        //        TPPMacro_KEYWORD_IS_EXPANDING() would return true for
        //        files in the backup list.
        //     >> Otherwise, "func(func2)" would expand to "func(func2)" instead of "func(func)"
        self->l_files->f_iter = old_iter; // Restore latest file
        if (popped_files != NULL) {
         TPPFileObject **popped_file_iter;
         TPP_ASSERT(popped_files->fl_size != 0);
         // Reuse the "macro_file" variable
         popped_file_iter = popped_files->fl_list+(popped_files->fl_size-1);
         while (1) {
          macro_file = *popped_file_iter;
          TPP_ASSERT(macro_file->f_prev == NULL);
          macro_file->f_prev = self->l_files;
          self->l_files = macro_file;
          if (popped_file_iter == popped_files->fl_list)
           break;
          --popped_file_iter;
         }
         // We don't call "TPPFileList_Free", because we are inheriting all the references
         TPP_CALLBACK_FREE_NN(popped_files);
        }
       } else {
        if (popped_files) TPPFileList_Free(popped_files);
        //_TPPFile_SetReferenceFromToken(macro_file,token_); // Setup the token reference in the file
#if TPP_CONFIG_NAME_MACRO_FILES
        TPPFile_SetNewName(macro_file,TPPKeywordList_NameOf(&self->l_keywords,macro_name));
#endif /* TPP_CONFIG_NAME_MACRO_FILES */
        TPPLexer_PushFileInheritedNoErr(self,macro_file);
        if (self->l_flags & TPPLexer_FLAG_ONE_FILE) ++self->l_one_file_rec;
        // Do it all over again...
        goto again;
       }
      } else if (ch != '\0' /*&& self->l_files != NULL*/) {
       // Put back the last character (this isn't a macro function call...)
       //TPPFile_Undo(self->l_files);
      }
     } else if (!TPPMacro_KEYWORD_IS_EXPANDING(macro)) {
      // Keyword-style macro
      TPPLexer_PushFile(self,macro->m_code);
      if (self->l_flags & TPPLexer_FLAG_ONE_FILE) ++self->l_one_file_rec;
      TPPFile_REWIND(macro->m_code); // Rewind the macro file
      _TPPFile_SetReferenceFromToken(macro->m_code,token_); // Setup the token reference in the file
      goto again;
     }
    }
   }
  } break;
 }
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// Constant expression parser
#define TPP_IF_TRY_SKIP(id,...)\
 do{\
  if (token_->tk_id == (id)) {\
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;\
   do __VA_ARGS__ while(0);\
  }\
 }while(0)
#define TPP_IF_TRY_SKIP_NOMACRO(id,...)\
 do{\
  if (token_->tk_id == (id)) {\
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) return -1;\
   do __VA_ARGS__ while(0);\
  }\
 }while(0)
#define TPP_WHILE_TRY_SKIP(id,...)\
 do{\
  while (token_->tk_id == (id)) {\
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;\
   do __VA_ARGS__ while(0);\
  }\
 }while(0)
#define TPP_IF_NOT_TRY_SKIP(id,...)\
 do{\
  if (token_->tk_id == (id)) {\
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;\
  } else {\
   do __VA_ARGS__ while(0);\
  }\
 }while(0)
#define TPP_CALL(func) do{if ((func) == -1) return -1;}while(0)
#define TPP_SKIP(tk,err)\
 do{\
  if(token_->tk_id != (tk)){if(err(self,token_) == -1) return -1;}else{\
   if (TPP_UNLIKELY(TPPLexer_Next(self,token_) == -1)) return -1;\
 }}while(0)
#define TPP_SKIP_NOMACRO(tk,err)\
 do{\
  if(token_->tk_id != (tk)){if(err(self,token_) == -1) return -1;}else{\
   if (TPP_UNLIKELY(TPPLexer_NextNoMacro(self,token_) == -1)) return -1;\
 }}while(0)
#define TPP_NEXT() TPP_CALL(TPPLexer_Next(self,token_))
#define TPP_NEXT_NOMACRO() TPP_CALL(TPPLexer_NextNoMacro(self,token_))


static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_unary(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 switch (token_->tk_id) {
  case TPP_TOK_INT:
  case TPP_TOK_CHR:
   if (result) {
    if (TPPToken_EvalInt(token_,(TPPUInteger *)result,NULL) < 0) {
     TPP_CALL(TPPWarn_ExpectedInteger(self,token_));
     *result = 0;
    }
   }
   TPP_NEXT();
   break;
  case '(':
   TPP_NEXT();
   // (...) expression (return last value of tuple)
   TPP_CALL(TPPLexer_EvalConst(self,token_,result));
   TPP_SKIP(')',TPPWarn_ExpectedRparenAfterLParen);
   break;
  case '!': 
   TPP_NEXT();
   TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,result));
   if (result) {
    if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(*result))
     TPP_CALL(TPPWarn_ExpectedBoolExpressionNot(self,token_,*result));
    *result = !*result;
   }
   break;
  case '~': TPP_NEXT(); TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,result)); if (result) *result = ~*result; break;
  case '+': TPP_NEXT(); TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,result)); if (result) *result = +*result; break;
  case '-': TPP_NEXT(); TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,result)); if (result) *result = -*result; break;
#if TPP_CONFIG_HAVE_IF_COND_EXPRESSION
  case KWD_if: {
   int is_true;
   TPP_NEXT();
   TPP_SKIP('(',TPPWarn_ExpectedLParen);
   TPP_CALL(TPPLexer_EvalConst(self,token_,result));
   TPP_SKIP(')',TPPWarn_ExpectedRParen);
   is_true = result != NULL ? TPP_STATIC_CAST(int,*result) : 0; // The fallback really doesn't matter here!
   TPP_CALL(TPPLexer_EvalConst1(self,token_,is_true ? result : NULL));
   while (token_->tk_id == ';') TPP_NEXT();
   while (token_->tk_id == KWD_elif) {
    TPP_NEXT();
    TPP_SKIP('(',TPPWarn_ExpectedLParen);
    TPP_CALL(TPPLexer_EvalConst(self,token_,is_true ? NULL : result));
    TPP_SKIP(')',TPPWarn_ExpectedRParen);
    TPP_CALL(TPPLexer_EvalConst1(self,token_,(
     !is_true && result && *result) ? (is_true = 1,result) : NULL));
    while (token_->tk_id == ';') TPP_NEXT();
   }
   if (token_->tk_id == KWD_else) {
    TPP_NEXT();
    TPP_CALL(TPPLexer_EvalConst1(self,token_,is_true ? NULL : result));
    while (token_->tk_id == ';') TPP_NEXT();
   } else if (!is_true && result) {
    TPP_CALL(TPPWarn_UnexpectedToken(self,token_,KWD_else));
    *result = 0; // Undefined return value
   }
  } break;
#endif /* TPP_CONFIG_HAVE_IF_COND_EXPRESSION */
  case KWD_defined: {
   int has_paren = 0;
   TPPTokenID kwd;
   TPP_NEXT_NOMACRO();
   TPP_IF_TRY_SKIP_NOMACRO('(',{has_paren=1;});
   kwd = token_->tk_id;
   if (!TPPTokenID_IS_KEYWORD(kwd)) TPP_CALL(has_paren ? 
    TPPWarn_ExpectedKeywordAfterDefined(self,token_):
    TPPWarn_ExpectedKeywordOrLParenAfterDefined(self,token_));
   TPP_NEXT_NOMACRO();
   if (has_paren) TPP_SKIP_NOMACRO(')',TPPWarn_ExpectedRParenAfterDefined);
   if (result) *result = TPPMacroList_Defined(&self->l_macros,kwd);
  } break;
  default:
   if (result) {
    // Process unknown tokens as "0"
    TPP_CALL(TPPWarn_UnexpectedTokenInConstExpr(self,token_));
    *result = TPP_STATIC_CAST(TPPInteger,0);
   }
   if (token_->tk_id != '\n' ||
       (self->l_flags & TPPLexer_FLAG_WANT_LF) == 0
       ) TPP_NEXT();
   break;
 }
 return 0;
}
#define BINARY_RECURSION_MULTI_ICALL(tok_id,low_call,ioperator)\
TPP_IF_TRY_SKIP(tok_id,{\
 if (result) {\
  TPP_CALL(low_call(self,token_,&rhs));\
  *result ioperator rhs;\
 } else {\
  TPP_CALL(low_call(self,token_,NULL));\
 }\
 goto cont;\
});
#define BINARY_RECURSION_MULTI_CALL(tok_id,low_call,operator)\
TPP_IF_TRY_SKIP(tok_id,{\
 if (result) {\
  TPP_CALL(low_call(self,token_,&rhs));\
  *result = (*result operator rhs);\
 } else {\
  TPP_CALL(low_call(self,token_,NULL));\
 }\
 goto cont;\
});
#define BINARY_RECURSION_ICALL(tok_id,low_call,ioperator)\
TPP_WHILE_TRY_SKIP(tok_id,{\
 if (result) {\
  TPPInteger rhs;\
  TPP_CALL(low_call(self,token_,&rhs));\
  *result ioperator rhs;\
 } else {\
  TPP_CALL(low_call(self,token_,NULL));\
 }\
});
#define BINARY_RECURSION_CALL(tok_id,low_call,operator)\
TPP_WHILE_TRY_SKIP(tok_id,{\
 if (result) {\
  TPPInteger rhs;\
  TPP_CALL(low_call(self,token_,&rhs));\
  *result = (*result operator rhs);\
 } else {\
  TPP_CALL(low_call(self,token_,NULL));\
 }\
});

TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_prod(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPInteger rhs;
 TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,result));
 while (1) {cont:
  BINARY_RECURSION_MULTI_ICALL('*',_TPPLexer_EvalConst_unary,*=);
#if 1
  TPP_IF_TRY_SKIP('/',{
   if (result) {
    TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,&rhs));
    if (rhs != 0) *result /= rhs; else {
     if (TPPWarn_DivideByZero(self,token_,*result) == -1) return -1;
     *result = 0;
    }
   } else {
    TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,NULL));
   }
   goto cont;
  });
  TPP_IF_TRY_SKIP('%',{
   if (result) {
    TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,&rhs));
    if (rhs != 0) *result %= rhs; else {
     if (TPPWarn_ModuloByZero(self,token_,*result) == -1) return -1;
     *result = 0;
    }
   } else {
    TPP_CALL(_TPPLexer_EvalConst_unary(self,token_,NULL));
   }
   goto cont;
  });
#else
  BINARY_RECURSION_MULTI_ICALL('/',_TPPLexer_EvalConst_unary,/=);
  BINARY_RECURSION_MULTI_ICALL('%',_TPPLexer_EvalConst_unary,%=);
#endif
  break;
 }
 return 0;
}
static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_sum(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPInteger rhs;
 TPP_CALL(_TPPLexer_EvalConst_prod(self,token_,result));
 while (1) {cont:
  BINARY_RECURSION_MULTI_ICALL('+',_TPPLexer_EvalConst_prod,+=);
  BINARY_RECURSION_MULTI_ICALL('-',_TPPLexer_EvalConst_prod,-=);
  break;
 }
 return 0;
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_shift(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPInteger rhs;
 TPP_CALL(_TPPLexer_EvalConst_sum(self,token_,result));
 while (1) {cont:
  BINARY_RECURSION_MULTI_ICALL(TPP_TOK_SHL,_TPPLexer_EvalConst_sum,<<=);
  BINARY_RECURSION_MULTI_ICALL(TPP_TOK_SHR,_TPPLexer_EvalConst_sum,>>=);
  break;
 }
 return 0;
}
static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_cmp(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPInteger rhs;
 TPP_CALL(_TPPLexer_EvalConst_shift(self,token_,result));
 while (1) {cont:
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_LO,_TPPLexer_EvalConst_shift,<);
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_LE,_TPPLexer_EvalConst_shift,<=);
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_GR,_TPPLexer_EvalConst_shift,>);
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_GE,_TPPLexer_EvalConst_shift,>=);
  break;
 }
 return 0;
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_cmp_eq(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPInteger rhs;
 TPP_CALL(_TPPLexer_EvalConst_cmp(self,token_,result));
 while (1) {cont:
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_EQ,_TPPLexer_EvalConst_cmp,==);
  BINARY_RECURSION_MULTI_CALL(TPP_TOK_CMP_NE,_TPPLexer_EvalConst_cmp,!=);
  break;
 }
 return 0;
}
static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_and(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_cmp_eq(self,token_,result));
 BINARY_RECURSION_ICALL('&',_TPPLexer_EvalConst_cmp_eq,&=);
 return 0;
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_xor(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_and(self,token_,result));
 BINARY_RECURSION_ICALL('^',_TPPLexer_EvalConst_and,^=);
 return 0;
}
static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_or(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_xor(self,token_,result));
 BINARY_RECURSION_ICALL('|',_TPPLexer_EvalConst_xor,|=);
 return 0;
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_Land(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_or(self,token_,result));
 TPP_WHILE_TRY_SKIP(TPP_TOK_LAND,{
  if (result && *result) {
   if (*result != 1) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionLhsOP(self,token_,"&&",*result));
    //TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
   }
   TPP_CALL(_TPPLexer_EvalConst_or(self,token_,result));
   if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(*result)) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionRhsOP(self,token_,"&&",*result));
    TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
   }
  } else {
   // error-less evaluation of expression
   // Required for "#if defined(X) && X"
   // >> If X isn't defined, *result will be false and
   //    we must ignore invalid expressions for the the remainder of the && scope
   TPP_CALL(_TPPLexer_EvalConst_or(self,token_,NULL));
  }
 });
 return 0;
}
#if TPP_CONFIG_HAVE_LXOR
static TPP_ATTRIBUTE_USE_RETURN int _TPPLexer_EvalConst_Lxor(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_Land(self,token_,result));
 TPP_WHILE_TRY_SKIP(TPP_TOK_LXOR,{
  if (result) {
   TPPInteger rhs;
   if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(*result)) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionLhsOP(self,token_,"^^",*result));
    TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
   }
   TPP_CALL(_TPPLexer_EvalConst_Land(self,token_,&rhs));
   if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(rhs)) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionRhsOP(self,token_,"^^",rhs));
    TPP_PRIVATE_TO_BOOL_EXPRESSION(rhs);
   }
   *result = /*!!*/(*result ^ rhs);
  } else {
   TPP_CALL(_TPPLexer_EvalConst_Land(self,token_,NULL));
  }
 });
 return 0;
}
#else /* TPP_CONFIG_HAVE_LXOR */
#define _TPPLexer_EvalConst_Lxor _TPPLexer_EvalConst_Land
#endif /* TPP_CONFIG_HAVE_LXOR */

TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalConst_Lor(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_CALL(_TPPLexer_EvalConst_Lxor(self,token_,result));
 TPP_WHILE_TRY_SKIP(TPP_TOK_LOR,{
  // Same as with "&&"
  if (result && !*result) {
   TPP_CALL(_TPPLexer_EvalConst_Lxor(self,token_,result));
   if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(*result)) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionRhsOP(self,token_,"||",*result));
    TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
   }
  } else {
   if (result && *result != 1) {
    TPP_CALL(TPPWarn_ExpectedBoolExpressionLhsOP(self,token_,"||",*result));
    TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
   }
   TPP_CALL(_TPPLexer_EvalConst_Lxor(self,token_,NULL));
  }
 });
 return 0;
}
#ifdef _TPPLexer_EvalConst_Lxor
#undef _TPPLexer_EvalConst_Lxor
#endif /* _TPPLexer_EvalConst_Lxor */

TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_EvalConst1(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 int is_true;
 TPP_CALL(_TPPLexer_EvalConst_Lor(self,token_,result));
 TPP_IF_NOT_TRY_SKIP('?',{return 0;});
 is_true = result != NULL ? TPP_STATIC_CAST(int,*result) : 0; // The fallback really doesn't matter here!
 TPP_IF_NOT_TRY_SKIP(':',{
  TPP_CALL(TPPLexer_EvalConst(self,token_,is_true ? result : NULL));
  TPP_SKIP(':',TPPWarn_ExpectedCollonAfterQuestion);
 });
 TPP_CALL(TPPLexer_EvalConst1(self,token_,is_true ? NULL : result));
 return 0;
}
#undef BINARY_RECURSION_CALL
#undef BINARY_RECURSION_ICALL
#undef BINARY_RECURSION_MULTI_CALL
#undef BINARY_RECURSION_MULTI_ICALL

TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_EvalDirectiveBool(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self),TPP_ASSERT(token_),TPP_ASSERT(result);
 if (TPPLexer_EvalConst(self,token_,result) == -1) return -1;
 if (!TPP_PRIVATE_IS_BOOL_EXPRESSION(*result)) {
  if (TPPWarn_ExpectedBoolExpression(self,token_,*result) == -1) return -1;
  TPP_PRIVATE_TO_BOOL_EXPRESSION(*result);
 }
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_EvalConst(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPP_ASSERT(self);
 while (1) {
  TPP_CALL(TPPLexer_EvalConst1(self,token_,result));
  TPP_IF_NOT_TRY_SKIP(',',{goto end;});
 }
end:
#if TPP_PRIVATE_CONFIG_VERBOSE
 if (result) TPP_VERBOSE_LOG_CONSTANT_EXPRESSION_PARSED(token_,*result);
#endif /* TPP_PRIVATE_CONFIG_VERBOSE */
 return 0;
}
#undef TPP_NEXT_NOMACRO
#undef TPP_NEXT
#undef TPP_SKIP_NOMACRO
#undef TPP_SKIP
#undef TPP_CALL
#undef TPP_IF_NOT_TRY_SKIP
#undef TPP_WHILE_TRY_SKIP
#undef TPP_IF_TRY_SKIP_NOMACRO
#undef TPP_IF_TRY_SKIP



TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Warnf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_, 
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, ...)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3))
 TPP_ATTRIBUTE_GCC_SOURCE_PRINTF_STRING(4,5) {
 va_list args;
 int result;
 va_start(args,fmt);
 result = TPPLexer_VWarnf(self,code,token_,fmt,args);
 va_end(args);
 return result;
}

#ifdef TPP_CALLBACK_ERROR_MESSAGE
#define TPPWarning_Print TPP_CALLBACK_ERROR_MESSAGE
static int TPPWarning_VPrintf(
 TPPLexerObject const *self, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *s, va_list args) {
 char buffer[4096];
#ifdef _MSC_VER
 int len = _vsnprintf(buffer,sizeof(buffer),s,args);
#else
 int len = vsnprintf(buffer,sizeof(buffer),s,args);
#endif
 if (len > 0) {
  buffer[len] = 0;
  return TPPWarning_Print(self,buffer);
 }
 return 0;
}
static int TPPWarning_Printf(
 TPPLexerObject const *self, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *s, ...) {
 va_list args; int result;
 va_start(args,s);
 result = TPPWarning_VPrintf(self,s,args);
 va_end(args);
 return result;
}
#else
#define TPPWarning_Print(self,s)        (_TPPWarning_Print(s),0)
#define TPPWarning_VPrintf(self,s,args) (_TPPWarning_VPrintf(s,args),0)
#define TPPWarning_Printf(self,s,...)   (_TPPWarning_Printf(s,__VA_ARGS__),0)
static void _TPPWarning_Print(TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s) {
#ifdef TPP_PLATFORM_WINDOWS
 OutputDebugStringA(s);
#endif
 fwrite(s,sizeof(char),strlen(s),stderr);
}
static void _TPPWarning_VPrintf(
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *s, va_list args) {
 char buffer[4096];
#ifdef _MSC_VER
 int len = _vsnprintf(buffer,sizeof(buffer),s,args);
#else
 int len = vsnprintf(buffer,sizeof(buffer),s,args);
#endif
 if (len > 0) {
  buffer[len] = 0;
  _TPPWarning_Print(buffer);
 }
}
static void _TPPWarning_Printf(
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *s, ...) {
 va_list args;
 va_start(args,s);
 _TPPWarning_VPrintf(s,args);
 va_end(args);
}
#endif


TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN int) _TPPLexer_WarnImpl(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *msg)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 TPPFileObject *include_tree;
 int result = TPPWarnings_GET(&self->l_warnings,code);
 if (TPPWarning_Printf(self,"%s(%d) : %s : %s %c%.4d : ",
      TPPToken_File(token_),TPPToken_Line(token_)+1,
      TPPLexer_TokenIDStr(self,token_->tk_id),result == -1 ? "Error" : "Warning",
      code >= TPPWarnings_COUNT_WITH_RESERVED ? 'C' : 'P',code) == -1 ||
     TPPWarning_Print(self,msg) == -1 ||
     TPPWarning_Print(self,"\n") == -1) return -1;
 include_tree = token_->tk_file;
 if (TPP_LIKELY(include_tree)) {
  TPP_ASSERT(!TPPFile_IS_MACRO_FILE(include_tree));
  if ((include_tree = include_tree->f_prev) != NULL) {
   do {
    if (TPPWarning_Printf(
     self,"%s(%d) : Included from here\n",
     TPPFile_NAME(include_tree),
     // NOTE: We don't add +1 here, because older files in the tree
     //       are always positioned on the line after the #include directive.
     //       >> So technically we would have to do "+1-1"
     TPPFile_LineAt(include_tree,TPPFile_ITER_OFF(include_tree))
     ) == -1) return -1;
   } while ((include_tree = include_tree->f_prev) != NULL);
   if (TPPWarning_Print(self,"\n") == -1) return -1;
  }
 }
#ifdef TPP_CALLBACK_FATAL_ERROR
 if (result == -1) result = TPP_CALLBACK_FATAL_ERROR(self,code,token_,msg);
#endif
 return result;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Warn(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *msg)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 int result = TPPWarnings_GET(&self->l_warnings,code);
 if (result == 1) return 0; // Disabled warning
 if (result == 2) { // Last suppress
  TPPWarnings_SET(&self->l_warnings,code,TPPWarnings_GET_DEFAULT(code));
  return 0;
 } else if (result > 2) { // Recursive suppress
  TPPWarnings_SET(&self->l_warnings,code,result-1);
  return 0;
 }
 return _TPPLexer_WarnImpl(self,code,token_,msg);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_VWarnf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, va_list args)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,3)) {
 char buffer[4096];
 int len,result = TPPWarnings_GET(&self->l_warnings,code);
 if (result == 1) return 0; // Disabled warning
 if (result == 2) { // Last suppress
  TPPWarnings_SET(&self->l_warnings,code,TPPWarnings_GET_DEFAULT(code));
  return 0;
 } else if (result > 2) { // Recursive suppress
  TPPWarnings_SET(&self->l_warnings,code,result-1);
  return 0;
 }
#ifdef _MSC_VER
 if ((len = _vsnprintf(buffer,sizeof(buffer),fmt,args)) < 0) len = 0;
#else
 if ((len = vsnprintf(buffer,sizeof(buffer),fmt,args)) < 0) len = 0;
#endif
 buffer[len] = 0;
 return _TPPLexer_WarnImpl(self,code,token_,buffer);
}
TPP_IMPL_FUNC(void) TPPLexer_EnumIncludeTree(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL void (*callback)(TPPFileObject const *file, int line, void *arg),
 void *arg) TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 TPPFileObject const *include_tree = self->l_files;
 TPP_ASSERT(callback);
 if (TPP_LIKELY(include_tree)) {
  while (TPPFile_IS_MACRO_FILE(include_tree)) // Skip macro files
   if ((include_tree = include_tree->f_prev) == NULL) return;
  while ((include_tree = include_tree->f_prev) != NULL) { // iterate + skip the first file
   callback(include_tree,TPPFile_LineAt(include_tree,TPPFile_ITER_OFF(include_tree)),arg);
  }
 }
}


TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self);
 self->w_top = TPPWarningsFrame_Alloc();
 if (self->w_top == NULL) return -1;
 self->w_top->wc_prev = NULL;
 memcpy(self->w_top->wc_warnings,
        TPPWarningsDefaultData,
        sizeof(TPPWarningsDefaultData));
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject const *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 struct TPPWarningsFrameObject *frame_iter,*next_frame,*frame_src;
 TPP_ASSERT(self && right);
 TPP_ASSERT(right->w_top);
 if ((frame_iter = (struct TPPWarningsFrameObject *)TPP_CALLBACK_MALLOC_NZ(
  sizeof(struct TPPWarningsFrameObject))) == NULL) {
  TPP_CALLBACK_NO_MEMORY();
  return -1;
 }
 memcpy(frame_iter,right->w_top,sizeof(struct TPPWarningsFrameObject));
 self->w_top = frame_iter;
 frame_src = right->w_top;
 while (frame_src->wc_prev) {
  if ((next_frame = (struct TPPWarningsFrameObject *)TPP_CALLBACK_MALLOC_NZ(
   sizeof(struct TPPWarningsFrameObject))) == NULL) {
   frame_iter = self->w_top;
   while (1) {
    next_frame = frame_iter->wc_prev;
    TPP_CALLBACK_FREE_NN(frame_iter);
    if (!next_frame) break;
    frame_iter = next_frame;
   }
   TPP_CALLBACK_NO_MEMORY();
   return -1;
  }
  memcpy(next_frame,frame_src,sizeof(struct TPPWarningsFrameObject));
  frame_iter->wc_prev = next_frame;
  frame_src = frame_src->wc_prev;
  frame_iter = next_frame;
 }
 TPP_ASSERT(frame_iter->wc_prev == NULL);
 return 0;
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *right)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1,2)) {
 struct TPPWarningsFrameObject *frame_iter;
 TPP_ASSERT(self && right);
 TPP_ASSERT(right->w_top);
 if ((frame_iter = (struct TPPWarningsFrameObject *)TPP_CALLBACK_MALLOC_NZ(
  sizeof(struct TPPWarningsFrameObject))) == NULL) {
  TPP_CALLBACK_NO_MEMORY();
  return -1;
 }
 memcpy(frame_iter,right->w_top,sizeof(struct TPPWarningsFrameObject));
 self->w_top = frame_iter;
 right->w_top->wc_prev = NULL;
 return 0;
}

TPP_IMPL_FUNC(void) TPPWarnings_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPWarningsFrameObject *frames;
 TPP_ASSERT(self);
 TPP_ASSERT(self->w_top);
 frames = self->w_top;
 while (1) {
  TPPWarningsFrameObject *next_frame = frames->wc_prev;
  TPPWarningsFrame_Free(frames);
  if ((frames = next_frame) == NULL) break;
 }
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPWarningsFrameObject *new_frame;
 TPP_ASSERT(self);
 new_frame = TPPWarningsFrame_Alloc();
 if (new_frame == NULL) return -1;
 memcpy(&new_frame->wc_warnings,&self->w_top->wc_warnings,sizeof(new_frame->wc_warnings));
 new_frame->wc_prev = self->w_top;
 self->w_top = new_frame;
 return 0;
}
TPP_IMPL_FUNC(int) TPPWarnings_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPPWarningsFrameObject *frame;
 TPP_ASSERT(self);
 TPP_ASSERT(self->w_top);
 frame = self->w_top;
 if (frame->wc_prev == NULL) return 1; // last frame!
 self->w_top = frame->wc_prev;
 TPPWarningsFrame_Free(frame);
 return 0;
}
TPP_IMPL_FUNC(void) TPPWarnings_Set(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self, int code, TPPWarningMode value)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid warnings object");
 TPP_ASSERT(code < TPPWarnings_COUNT && "Invalid warning code");
 TPPWarnings_SET(self,code,value);
}
TPP_IMPL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPWarningMode) TPPWarnings_Get(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self, int code)
 TPP_ATTRIBUTE_GCC_SOURCE_NON_NULL((1)) {
 TPP_ASSERT(self && "Invalid warnings object");
 TPP_ASSERT(code < TPPWarnings_COUNT && "Invalid warning code");
 return TPPWarnings_GET(self,code);
}

#undef TPP_PRIVATE_IS_BOOL_EXPRESSION
#undef TPP_PRIVATE_TO_BOOL_EXPRESSION

#undef TPP_LIKELY
#undef TPP_UNLIKELY

#undef TPP_PRIVATE_CONFIG_LF_CANCLES_INSTR
#undef TPP_PRIVATE_CONFIG_VERBOSE
#undef TPP_PRIVATE_CONFIG_VERBOSE_LOG_MACRO_CALL

#undef _TPPFile_NewSetReferenceFromToken
#undef _TPPFile_SetReferenceFromToken
#undef TPP_LVALUE_CAST
#undef TPP_REINTERPRET_CAST
#undef TPP_CONST_CAST
#undef TPP_STATIC_CAST
#undef TPP_DYNAMIC_CAST
#undef TPP_VOID_CAST
#undef TPP_ENUM_LVALUE
#undef TPP_IS_DIRSEP
#undef TPP_IS_ABSPATH
#undef TPP_PATHCMP
#undef TPP_SYS_SEP
#undef _TPP_VERBOSE_LOG
#undef TPP_VERBOSE_LOG
#undef TPP_VERBOSE_LOG_PARSING_STRING_PRAGMA
#undef TPP_VERBOSE_LOG_CONSTANT_EXPRESSION_PARSED
#undef TPP_VERBOSE_LOG_SKITPPING_GUARDED_INCLUDE
#undef TPP_VERBOSE_LOG_DETECTED_PRAGMA_ONCE
#undef TPP_VERBOSE_LOG_DETECTED_INCLUDE_GUARD
#undef TPP_VERBOSE_LOG_INCLUDE
#undef TPP_VERBOSE_LOG_HAS_INCLUDE
#undef TPP_VERBOSE_LOG_TPP_COUNTER
#undef TPP_REPLACE_COMMENTS_WITH_SPACE

#ifdef TPP_PRIVATE_CONFIG_HAVE_DEBUG_MEMCPY
#undef TPP_PRIVATE_CONFIG_HAVE_DEBUG_MEMCPY
#if TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO
#pragma pop_macro("memcpy")
#pragma pop_macro("memmove")
#pragma pop_macro("memcmp")
#else /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
#undef memcpy
#undef memmove
#undef memcmp
#endif /* !TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
#endif /* TPP_PRIVATE_CONFIG_HAVE_DEBUG_MEMCPY */
#undef TPP_CH_IS_ALPHA_ANSI
#if TPP_CONFIG_USE_CHARACTER_TRAITS
#undef TPP_CHARACTER_MAP
#undef TPP_CH_IS_LOWER_UTF8
#undef TPP_CH_IS_UPPER_UTF8
#undef TPP_CH_IS_ALPHA_UTF8
#undef TPP_CH_IS_LOWER_ANSI
#undef TPP_CH_IS_UPPER_ANSI
#undef TPP_CH_IS_ALNUM
#undef TPP_CH_IS_WHSPC
#undef TPP_CH_IS_DIGIT
#endif /* TPP_CONFIG_USE_CHARACTER_TRAITS */
#if TPP_CONFIG_USE_CHARACTER_TRAITS || !TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
#undef TPP_CH_IS_LOWER
#undef TPP_CH_IS_UPPER
#undef TPP_CH_IS_ALPHA
#endif /* TPP_CONFIG_USE_CHARACTER_TRAITS || !TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */

#ifdef __cplusplus
}
#endif
TPP_DECLS_END_DISABLE_WARNINGS

#if TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO
#pragma pop_macro("TPP_CONFIG_HAVE_DEPRECATED_API")
#else /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */
#undef TPP_CONFIG_HAVE_DEPRECATED_API
#ifdef TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED
#if TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED
# define TPP_CONFIG_HAVE_DEPRECATED_API 1
#else /* TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED */
# define TPP_CONFIG_HAVE_DEPRECATED_API 0
#endif /* !TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED */
#undef TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED
#endif /* TPP_PRIVATE_SOURCE_OLD_CONFIG_HAVE_DEPRECATED_API_ENABLED */
#endif /* TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */

#endif /* !GUARD_TPP_C */
