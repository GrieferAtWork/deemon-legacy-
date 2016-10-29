/* Copyright (c) 2015-2016 Cyder aka. Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                                               *
 * This software is provided 'as-is', without any express or implied                             *
 * warranty. In no event will the authors be held liable for any damages                         *
 * arising from the use of this software.                                                        *
 *                                                                                               *
 * Permission is granted to anyone to use this software for any purpose,                         *
 * including commercial applications, and to alter it and redistribute it                        *
 * freely, subject to the following restrictions:                                                *
 *                                                                                               *
 * 1. The origin of this software must not be misrepresented; you must not                       *
 *    claim that you wrote the original software. If you use this software                       *
 *    in a product, an acknowledgement in the product documentation would be                     *
 *    appreciated but is not required.                                                           *
 * 2. Altered source versions must be plainly marked as such, and must not be                    *
 *    misrepresented as being the original software.                                             *
 * 3. This notice may not be removed or altered from any source distribution.                    *
 */

#ifndef DEF
#define __TPP_KEYWORDS_INL_DEFINES_DEF
#define DEF(name,str)
#endif /* !DEF */
#ifndef TPP_MACRO_DEF
#define __TPP_KEYWORDS_INL_DEFINES_TPP_MACRO_DEF
#define TPP_MACRO_DEF DEF
#endif /* !TPP_MACRO_DEF */
#ifndef DEF_TRAIT
#define __TPP_KEYWORDS_INL_DEFINES_DEF_TRAIT
#define DEF_TRAIT(str,flag) /* nothing */
#endif /* !DEF_TRAIT */


//////////////////////////////////////////////////////////////////////////
// Builtin keyword (required by the preprocessor)
DEF(KWD_if,           "if")
DEF(KWD_ifdef,        "ifdef")
DEF(KWD_ifndef,       "ifndef")
DEF(KWD_elif,         "elif")
DEF(KWD_else,         "else")
DEF(KWD_endif,        "endif")
DEF(KWD_define,       "define")
DEF(KWD_undef,        "undef")
DEF(KWD_include,      "include")
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
DEF(KWD_include_next, "include_next")
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
#if TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
DEF(KWD_import,       "import")
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */
DEF(KWD_line,         "line")
DEF(KWD_pragma,       "pragma")
DEF(KWD_error,        "error") // When changing this, you must also update "_TPPFile_Format"
DEF(KWD_defined,      "defined")
#if TPP_CONFIG_HAVE_PRAGMA_MESSAGE
DEF(KWD_message,      "message")
#endif /* TPP_CONFIG_HAVE_PRAGMA_MESSAGE */
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
DEF(KWD_deprecated,   "deprecated")
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#if TPP_CONFIG_HAVE_PRAGMA_WARNING
DEF(KWD_push,         "push")         // required for #pragma warning
DEF(KWD_pop,          "pop")          // required for #pragma warning
DEF(KWD_disable,      "disable")      // required for #pragma warning
DEF(KWD_enable,       "enable")       // required for #pragma warning
DEF(KWD_default,      "default")      // required for #pragma warning
DEF(KWD_suppress,     "suppress")     // required for #pragma warning
#endif /* TPP_CONFIG_HAVE_PRAGMA_WARNING */
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
DEF(KWD_push_macro,   "push_macro")
DEF(KWD_pop_macro,    "pop_macro")
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
#if TPP_CONFIG_HAVE_PRAGMA_REGION
DEF(KWD_region,       "region")
DEF(KWD_endregion,    "endregion")
#endif /* TPP_CONFIG_HAVE_PRAGMA_REGION */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC
DEF(KWD_tpp_exec,     "tpp_exec")
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
DEF(KWD_tpp_set_keyword_flags,"tpp_set_keyword_flags")
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */


#if 1
// Dunno... You could look at it like this
// >> GCC defines _Pragma as a macro... So I'll just do that as well!
TPP_MACRO_DEF(KWD__Pragma,"_Pragma")
#else
DEF          (KWD__Pragma,"_Pragma")
#endif
#if TPP_CONFIG_HAVE_PRAGMA_ONCE
DEF(KWD_once,         "once")
#endif /* TPP_CONFIG_HAVE_PRAGMA_ONCE */
#if TPP_CONFIG_HAVE_WARNING || TPP_CONFIG_HAVE_PRAGMA_WARNING
DEF(KWD_warning,      "warning") // When changing this, you must also update "_TPPFile_Format"
#endif /* TPP_CONFIG_HAVE_WARNING || TPP_CONFIG_HAVE_PRAGMA_WARNING */
#if TPP_CONFIG_HAVE___VA_ARGS__
DEF(KWD___VA_ARGS__,  "__VA_ARGS__")
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE___VA_COMMA__
DEF(KWD___VA_COMMA__, "__VA_COMMA__")
#endif /* TPP_CONFIG_HAVE___VA_COMMA__ */
#if TPP_CONFIG_HAVE___BASE_FILE__
TPP_MACRO_DEF(KWD___BASE_FILE__,"__BASE_FILE__")
#endif /* TPP_CONFIG_HAVE___BASE_FILE__ */
#if TPP_CONFIG_HAVE___has_include
TPP_MACRO_DEF(KWD___has_include,     "__has_include")
TPP_MACRO_DEF(KWD___has_include_next,"__has_include_next")
#endif /* TPP_CONFIG_HAVE___has_include */
#if TPP_CONFIG_HAVE___TPP_LOAD_FILE
TPP_MACRO_DEF(KWD___TPP_LOAD_FILE,"__TPP_LOAD_FILE")
#endif /* TPP_CONFIG_HAVE___TPP_LOAD_FILE */
#if TPP_CONFIG_HAVE___TPP_COUNTER
TPP_MACRO_DEF(KWD___TPP_COUNTER,"__TPP_COUNTER")
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
#if TPP_CONFIG_HAVE___TPP_RANDOM
TPP_MACRO_DEF(KWD___TPP_RANDOM,"__TPP_RANDOM")
#endif /* TPP_CONFIG_HAVE___TPP_RANDOM */
#if TPP_CONFIG_HAVE___TPP_EVAL
TPP_MACRO_DEF(KWD___TPP_EVAL,"__TPP_EVAL")
#endif /* TPP_CONFIG_HAVE___TPP_EVAL */
#if TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
TPP_MACRO_DEF(KWD___TPP_STR_DECOMPILE,"__TPP_STR_DECOMPILE")
#endif /* TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */
#if TPP_CONFIG_HAVE___TPP_STR_AT
TPP_MACRO_DEF(KWD___TPP_STR_AT,"__TPP_STR_AT")
#endif /* TPP_CONFIG_HAVE___TPP_STR_AT */
#if TPP_CONFIG_HAVE___TPP_STR_SUBSTR
TPP_MACRO_DEF(KWD___TPP_STR_SUBSTR,"__TPP_STR_SUBSTR")
#endif /* TPP_CONFIG_HAVE___TPP_STR_SUBSTR */
#if TPP_CONFIG_HAVE___TPP_STR_PACK
TPP_MACRO_DEF(KWD___TPP_STR_PACK,"__TPP_STR_PACK")
#endif /* TPP_CONFIG_HAVE___TPP_STR_PACK */
#if TPP_CONFIG_HAVE___TPP_STR_SIZE
TPP_MACRO_DEF(KWD___TPP_STR_SIZE,"__TPP_STR_SIZE")
#endif /* TPP_CONFIG_HAVE___TPP_STR_SIZE */
TPP_MACRO_DEF(KWD___FILE__,"__FILE__")
TPP_MACRO_DEF(KWD___LINE__,"__LINE__")
TPP_MACRO_DEF(KWD___DATE__,"__DATE__")
TPP_MACRO_DEF(KWD___TIME__,"__TIME__")
#if TPP_CONFIG_HAVE___TIMESTAMP__
TPP_MACRO_DEF(KWD___TIMESTAMP__,"__TIMESTAMP__")
#endif /* TPP_CONFIG_HAVE___TIMESTAMP__ */
#if TPP_CONFIG_HAVE___INCLUDE_DEPTH__
TPP_MACRO_DEF(KWD___INCLUDE_LEVEL__,"__INCLUDE_LEVEL__")
TPP_MACRO_DEF(KWD___INCLUDE_DEPTH__,"__INCLUDE_DEPTH__")
#endif /* TPP_CONFIG_HAVE___INCLUDE_DEPTH__ */
#if TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS
TPP_MACRO_DEF(KWD___TIME_SEC__, "__TIME_SEC__")
TPP_MACRO_DEF(KWD___TIME_MIN__, "__TIME_MIN__")
TPP_MACRO_DEF(KWD___TIME_HOUR__,"__TIME_HOUR__")
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS */
#if TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS
TPP_MACRO_DEF(KWD___DATE_DAY__,  "__DATE_DAY__")
TPP_MACRO_DEF(KWD___DATE_WDAY__, "__DATE_WDAY__")
TPP_MACRO_DEF(KWD___DATE_YDAY__, "__DATE_YDAY__")
TPP_MACRO_DEF(KWD___DATE_MONTH__,"__DATE_MONTH__")
TPP_MACRO_DEF(KWD___DATE_YEAR__, "__DATE_YEAR__")
#endif /* TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS */
#if TPP_CONFIG_HAVE___COUNTER__
TPP_MACRO_DEF(KWD___COUNTER__,"__COUNTER__")
#endif /* TPP_CONFIG_HAVE___COUNTER__ */

#if TPP_CONFIG_HAVE___is_identifier
TPP_MACRO_DEF(KWD___is_identifier,"__is_identifier")
#endif /* TPP_CONFIG_HAVE___is_identifier */
#if TPP_CONFIG_HAVE___is_builtin_identifier
TPP_MACRO_DEF(KWD___is_builtin_identifier,"__is_builtin_identifier")
#endif /* TPP_CONFIG_HAVE___is_builtin_identifier */
#if TPP_CONFIG_HAVE___is_deprecated
TPP_MACRO_DEF(KWD___is_deprecated,"__is_deprecated")
#endif /* TPP_CONFIG_HAVE___is_deprecated */

#if TPP_CONFIG_HAVE___has_attribute
#undef TPP_DEF_ATTRIBUTE
#define TPP_DEF_ATTRIBUTE(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_ATTRIBUTE)
TPP_MACRO_DEF(KWD___has_attribute,"__has_attribute")
#endif /* TPP_CONFIG_HAVE___has_attribute */

#if TPP_CONFIG_HAVE___has_builtin
#undef TPP_DEF_BUILTIN
#define TPP_DEF_BUILTIN(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_BUILTIN)
TPP_MACRO_DEF(KWD___has_builtin,"__has_builtin")
#endif /* TPP_CONFIG_HAVE___has_builtin */

#if TPP_CONFIG_HAVE___has_cpp_attribute
#undef TPP_DEF_CPP_ATTRIBUTE
#define TPP_DEF_CPP_ATTRIBUTE(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_CPP_ATTRIBUTE)
TPP_MACRO_DEF(KWD___has_cpp_attribute,"__has_cpp_attribute")
#endif /* TPP_CONFIG_HAVE___has_cpp_attribute */

#if TPP_CONFIG_HAVE___has_declspec_attribute
#undef TPP_DEF_DECLSPEC_ATTRIBUTE
#define TPP_DEF_DECLSPEC_ATTRIBUTE(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_DECLSPEC_ATTRIBUTE)
TPP_MACRO_DEF(KWD___has_declspec_attribute,"__has_declspec_attribute")
#endif /* TPP_CONFIG_HAVE___has_declspec_attribute */

#if TPP_CONFIG_HAVE___has_extension
#undef TPP_DEF_EXTENSION
#define TPP_DEF_EXTENSION(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_EXTENSION)
TPP_MACRO_DEF(KWD___has_extension,"__has_extension")
#endif /* TPP_CONFIG_HAVE___has_extension */

#if TPP_CONFIG_HAVE___has_feature
#undef TPP_DEF_FEATURE
#define TPP_DEF_FEATURE(name) DEF_TRAIT(name,TPPKeywordFlag_HAS_FEATURE)
TPP_MACRO_DEF(KWD___has_feature,"__has_feature")
#endif /* TPP_CONFIG_HAVE___has_feature */

#ifndef TPP_DEF_ATTRIBUTE
#define TPP_DEF_ATTRIBUTE(name)  /* nothing */
#endif /* !TPP_DEF_ATTRIBUTE */
#ifndef TPP_DEF_BUILTIN
#define TPP_DEF_BUILTIN(name)  /* nothing */
#endif /* !TPP_DEF_BUILTIN */
#ifndef TPP_DEF_CPP_ATTRIBUTE
#define TPP_DEF_CPP_ATTRIBUTE(name)  /* nothing */
#endif /* !TPP_DEF_CPP_ATTRIBUTE */
#ifndef TPP_DEF_DECLSPEC_ATTRIBUTE
#define TPP_DEF_DECLSPEC_ATTRIBUTE(name)  /* nothing */
#endif /* !TPP_DEF_DECLSPEC_ATTRIBUTE */
#ifndef TPP_DEF_EXTENSION
#define TPP_DEF_EXTENSION(name)  /* nothing */
#endif /* !TPP_DEF_EXTENSION */
#ifndef TPP_DEF_FEATURE
#define TPP_DEF_FEATURE(name) /* nothing */
#endif /* !TPP_DEF_FEATURE */


//////////////////////////////////////////////////////////////////////////
// Builtin extensions
#if TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
TPP_DEF_EXTENSION("tpp_dollar_is_alpha")
#endif /* TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */
#if TPP_CONFIG_HAVE___VA_ARGS__
TPP_DEF_EXTENSION("tpp_va_args")
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
TPP_DEF_EXTENSION("tpp_named_va_args")
#endif /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#if TPP_CONFIG_HAVE___VA_COMMA__
TPP_DEF_EXTENSION("tpp_va_comma")
#endif /* TPP_CONFIG_HAVE___VA_COMMA__ */
#if TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS
TPP_DEF_EXTENSION("tpp_reemit_unknown_pragmas")
#endif /* TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */
#if TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX
TPP_DEF_EXTENSION("tpp_msvc_integer_suffix")
#endif /* TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX */
#if TPP_CONFIG_HAVE_CHARIZE_OPERATOR
TPP_DEF_EXTENSION("tpp_charize_operator")
#endif /* TPP_CONFIG_HAVE_CHARIZE_OPERATOR */
#if TPP_CONFIG_HAVE_TRIGRAPHS
TPP_DEF_EXTENSION("tpp_trigraphs")
#endif /* TPP_CONFIG_HAVE_TRIGRAPHS */
#if TPP_CONFIG_HAVE_DIGRAPHS
TPP_DEF_EXTENSION("tpp_digraphs")
#endif /* TPP_CONFIG_HAVE_DIGRAPHS */
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
TPP_DEF_EXTENSION("tpp_pragma_push_macro")
TPP_DEF_EXTENSION("tpp_pragma_pop_macro")
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
#if TPP_CONFIG_HAVE_PRAGMA_REGION
TPP_DEF_EXTENSION("tpp_pragma_region")
TPP_DEF_EXTENSION("tpp_pragma_endregion")
#endif /* TPP_CONFIG_HAVE_PRAGMA_REGION */
#if TPP_CONFIG_HAVE_PRAGMA_WARNING
TPP_DEF_EXTENSION("tpp_pragma_warning")
#endif /* TPP_CONFIG_HAVE_PRAGMA_WARNING */
#if TPP_CONFIG_HAVE_PRAGMA_MESSAGE
TPP_DEF_EXTENSION("tpp_pragma_message")
#endif /* TPP_CONFIG_HAVE_PRAGMA_MESSAGE */
#if TPP_CONFIG_HAVE_PRAGMA_ERROR
TPP_DEF_EXTENSION("tpp_pragma_error")
#endif /* TPP_CONFIG_HAVE_PRAGMA_ERROR */
#if TPP_CONFIG_HAVE_PRAGMA_ONCE
TPP_DEF_EXTENSION("tpp_pragma_once")
#endif /* TPP_CONFIG_HAVE_PRAGMA_ONCE */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC
TPP_DEF_EXTENSION("tpp_pragma_tpp_exec")
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC */
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
TPP_DEF_EXTENSION("tpp_pragma_deprecated")
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#if TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
TPP_DEF_EXTENSION("tpp_pragma_tpp_set_keyword_flags")
#endif /* TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
TPP_DEF_EXTENSION("tpp_directive_include_next")
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
#if TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
TPP_DEF_EXTENSION("tpp_directive_import")
#endif /* TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */
#if TPP_CONFIG_HAVE_WARNING
TPP_DEF_EXTENSION("tpp_directive_warning")
#endif /* TPP_CONFIG_HAVE_WARNING */
#if TPP_CONFIG_HAVE_LXOR
TPP_DEF_EXTENSION("tpp_lxor")
#endif /* TPP_CONFIG_HAVE_LXOR */

#if TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
TPP_DEF_EXTENSION("tpp_token_tilde_tilde")
#endif /* TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */
#if TPP_CONFIG_HAVE_POW_TOKEN
TPP_DEF_EXTENSION("tpp_token_pow")
#endif /* TPP_CONFIG_HAVE_POW_TOKEN */
#if TPP_CONFIG_HAVE_LXOR_TOKEN
TPP_DEF_EXTENSION("tpp_token_lxor")
#endif /* TPP_CONFIG_HAVE_LXOR_TOKEN */
#if TPP_CONFIG_HAVE_ARROW_TOKEN
TPP_DEF_EXTENSION("tpp_token_arrow")
#endif /* TPP_CONFIG_HAVE_ARROW_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
TPP_DEF_EXTENSION("tpp_token_collon_assign")
#endif /* TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
TPP_DEF_EXTENSION("tpp_token_collon_collon")
#endif /* TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN */

#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
TPP_DEF_EXTENSION("tpp_macro_calling_conventions")
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
#if TPP_CONFIG_STRICT_WHITESPACE
TPP_DEF_EXTENSION("tpp_strict_whitespace")
#endif /* TPP_CONFIG_STRICT_WHITESPACE */
#if TPP_CONFIG_STRICT_INTEGER_OVERFLOW
TPP_DEF_EXTENSION("tpp_strict_integer_overflow")
#endif /* TPP_CONFIG_STRICT_INTEGER_OVERFLOW */
#if TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
TPP_DEF_EXTENSION("tpp_support_ansi_characters")
#endif /* TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */
#if TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE
TPP_DEF_EXTENSION("tpp_emit_lf_after_directive")
#endif /* TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE */
#if TPP_CONFIG_HAVE_IF_COND_EXPRESSION
TPP_DEF_EXTENSION("tpp_if_cond_expression")
#endif /* TPP_CONFIG_HAVE_IF_COND_EXPRESSION */
#if TPP_DEBUG
TPP_DEF_EXTENSION("tpp_debug")
#endif /* TPP_DEBUG */
#if TPP_USE_DEBUG_NEW
TPP_DEF_EXTENSION("tpp_debug_new")
#endif /* TPP_USE_DEBUG_NEW */


//////////////////////////////////////////////////////////////////////////
// Add the keywords of your language here...
#ifdef TPP_KEYWORD_FILE_0
#include TPP_KEYWORD_FILE_0()
#endif /* TPP_KEYWORD_FILE_0 */
#ifdef TPP_KEYWORD_FILE_1
#include TPP_KEYWORD_FILE_1()
#endif /* TPP_KEYWORD_FILE_1 */
#ifdef TPP_KEYWORD_FILE_2
#include TPP_KEYWORD_FILE_2()
#endif /* TPP_KEYWORD_FILE_2 */
#ifdef TPP_KEYWORD_FILE_3
#include TPP_KEYWORD_FILE_3()
#endif /* TPP_KEYWORD_FILE_3 */

#ifdef __TPP_KEYWORDS_INL_DEFINES_DEF
#undef __TPP_KEYWORDS_INL_DEFINES_DEF
#undef DEF
#endif /* __TPP_KEYWORDS_INL_DEFINES_DEF */
#ifdef __TPP_KEYWORDS_INL_DEFINES_TPP_MACRO_DEF
#undef __TPP_KEYWORDS_INL_DEFINES_TPP_MACRO_DEF
#undef TPP_MACRO_DEF
#endif /* __TPP_KEYWORDS_INL_DEFINES_TPP_MACRO_DEF */
#ifdef __TPP_KEYWORDS_INL_DEFINES_DEF_TRAIT
#undef __TPP_KEYWORDS_INL_DEFINES_DEF_TRAIT
#undef DEF_TRAIT
#endif /* __TPP_KEYWORDS_INL_DEFINES_DEF_TRAIT */
