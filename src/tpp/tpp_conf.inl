/* Copyright (c) 2015-2016 Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                                    *
 * This software is provided 'as-is', without any express or implied                  *
 * warranty. In no event will the authors be held liable for any damages              *
 * arising from the use of this software.                                             *
 *                                                                                    *
 * Permission is granted to anyone to use this software for any purpose,              *
 * including commercial applications, and to alter it and redistribute it             *
 * freely, subject to the following restrictions:                                     *
 *                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not            *
 *    claim that you wrote the original software. If you use this software            *
 *    in a product, an acknowledgement in the product documentation would be          *
 *    appreciated but is not required.                                                *
 * 2. Altered source versions must be plainly marked as such, and must not be         *
 *    misrepresented as being the original software.                                  *
 * 3. This notice may not be removed or altered from any source distribution.         *
 */
#ifndef GUARD_TPP_CONF_INL
#define GUARD_TPP_CONF_INL 1

#ifndef __has_include
#define __has_include(x) 0
#endif /* !__has_include */
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif /* !__has_builtin */
#ifndef __has_feature
#define __has_feature(x) 0
#endif /* !__has_feature */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif /* !__has_attribute */
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif /* !__has_declspec_attribute */

#if defined(_WIN32) || defined(WIN32) || \
    defined(_WIN64) || defined(WIN64) || \
    defined(__WINDOWS__) || defined(__MINGW32__) || \
    defined(__MINGW64__) || defined(__CYGWIN__)
#define TPP_PLATFORM_WINDOWS 1
#endif


//////////////////////////////////////////////////////////////////////////
// "$" can be used in identifiers
// CHECK: #if __has_feature(tpp_dollar_is_alpha) || __has_extension(tpp_dollar_is_alpha)
#ifndef TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA
#define TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA 1
#endif /* !TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA */

//////////////////////////////////////////////////////////////////////////
// Builtin macros: __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__
// CHECK: #if defined(__TIME_SEC__)...
#ifndef TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS
#define TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS 1
#endif /* !TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS */

//////////////////////////////////////////////////////////////////////////
// Builtin macros: __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__
// CHECK: #if defined(__DATE_DAY__)...
#ifndef TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS
#define TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS 1
#endif /* !TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS */

//////////////////////////////////////////////////////////////////////////
// Directive: #include_next <include_string>
// CHECK: #if __has_feature(tpp_directive_include_next) || __has_extension(tpp_directive_include_next)
// - Includes the next file matching "include_string", that isn't already being included
#ifndef TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
#ifdef TPP_CONFIG_HAVE_include_next // deprecated version
# define TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT TPP_CONFIG_HAVE_include_next
#else
# define TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT 1
#endif
#endif /* !TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */

//////////////////////////////////////////////////////////////////////////
// Directive: #import <include_string>
// CHECK: #if __has_feature(tpp_directive_import) || __has_extension(tpp_directive_import)
// - Includes a file only once
// - Multiple #import statements will only include the same file once
// - This is implemented, by simulating a #pragma once at the
//   beginning of the file, to be imported.
//   NOTE: That should also explain how it behaves, if used with #include!
#ifndef TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
#define TPP_CONFIG_HAVE_DIRECTIVE_IMPORT 1
#endif /* !TPP_CONFIG_HAVE_DIRECTIVE_IMPORT */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: __TIMESTAMP__
// CHECK: #if defined(__TIMESTAMP__)
#ifndef TPP_CONFIG_HAVE___TIMESTAMP__
#define TPP_CONFIG_HAVE___TIMESTAMP__ 1
#endif /* !TPP_CONFIG_HAVE___TIMESTAMP__ */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: __COUNTER__
// CHECK: #if defined(__COUNTER__)
#ifndef TPP_CONFIG_HAVE___COUNTER__
#define TPP_CONFIG_HAVE___COUNTER__ 1
#endif /* !TPP_CONFIG_HAVE___COUNTER__ */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: __INCLUDE_DEPTH__
// CHECK: #if defined(__INCLUDE_DEPTH__)
#ifndef TPP_CONFIG_HAVE___INCLUDE_DEPTH__
#define TPP_CONFIG_HAVE___INCLUDE_DEPTH__ 1
#endif /* !TPP_CONFIG_HAVE___INCLUDE_DEPTH__ */

//////////////////////////////////////////////////////////////////////////
// int __TPP_COUNTER(kwd)
// CHECK: #if defined(__TPP_COUNTER)
// >> A counter for with a unique identifier (different counter for every "kwd")
// >> This fixes the usage problem of "__COUNTER__", where
//    you never really know, what it will be at any given point.
#ifndef TPP_CONFIG_HAVE___TPP_COUNTER
#define TPP_CONFIG_HAVE___TPP_COUNTER 1
#endif /* !TPP_CONFIG_HAVE___TPP_COUNTER */

//////////////////////////////////////////////////////////////////////////
// int __TPP_RANDOM(int min [,int max])
// CHECK: #if defined(__TPP_RANDOM)
// >> Returns a random "max >= integer >= min"
#ifndef TPP_CONFIG_HAVE___TPP_RANDOM
#define TPP_CONFIG_HAVE___TPP_RANDOM 1
#endif /* !TPP_CONFIG_HAVE___TPP_RANDOM */

//////////////////////////////////////////////////////////////////////////
// int __TPP_EVAL(Expression)
// CHECK: #if defined(__TPP_EVAL)
// >> Evaluate a mathematical "Expression" at preprocession time to a c token.
// >> Evaluation follows the same rules as expressions in "#if" directives.
// >> This is a very powerful macro!
#ifndef TPP_CONFIG_HAVE___TPP_EVAL
#define TPP_CONFIG_HAVE___TPP_EVAL 1
#endif /* !TPP_CONFIG_HAVE___TPP_EVAL */

//////////////////////////////////////////////////////////////////////////
// Code ...__TPP_STR_DECOMPILE(String str)
// CHECK: #if defined(__TPP_STR_DECOMPILE)
// >> Decompiles a string, add all of its textual contents
//    as tokens, as if from a macro expansion
// NOTE: Preprocessor directives are not executed, but macros are expanded
#ifndef TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
#define TPP_CONFIG_HAVE___TPP_STR_DECOMPILE 1
#endif /* !TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */

//////////////////////////////////////////////////////////////////////////
// Character __TPP_STR_AT(String str, int i, int n = 1)
// CHECK: #if defined(__TPP_STR_AT)
// >> Returns a character consisting of n characters at i from str
#ifndef TPP_CONFIG_HAVE___TPP_STR_AT
#define TPP_CONFIG_HAVE___TPP_STR_AT 1
#endif /* !TPP_CONFIG_HAVE___TPP_STR_AT */

//////////////////////////////////////////////////////////////////////////
// String __TPP_STR_SUBSTR(String str, int i, int n = 1)
// CHECK: #if defined(__TPP_STR_SUBSTR)
// >> Same as __TPP_STR_AT, but returns a String instead
#ifndef TPP_CONFIG_HAVE___TPP_STR_SUBSTR
#define TPP_CONFIG_HAVE___TPP_STR_SUBSTR 1
#endif /* !TPP_CONFIG_HAVE___TPP_STR_SUBSTR */

//////////////////////////////////////////////////////////////////////////
// String __TPP_STR_PACK(int ...chrs)
// CHECK: #if defined(__TPP_STR_PACK)
// >> packs a strings from a list of integers, that describe the string
#ifndef TPP_CONFIG_HAVE___TPP_STR_PACK
#define TPP_CONFIG_HAVE___TPP_STR_PACK 1
#endif /* !TPP_CONFIG_HAVE___TPP_STR_PACK */

//////////////////////////////////////////////////////////////////////////
// int __TPP_STR_SIZE(String str)
// CHECK: #if defined(__TPP_STR_SIZE)
// >> Returns the size of a given string
#ifndef TPP_CONFIG_HAVE___TPP_STR_SIZE
#define TPP_CONFIG_HAVE___TPP_STR_SIZE 1
#endif /* !TPP_CONFIG_HAVE___TPP_STR_SIZE */

//////////////////////////////////////////////////////////////////////////
// string __TPP_LOAD_FILE(*include_string*, int include_next = 0)
// CHECK: #if defined(__TPP_LOAD_FILE)
// >> Returns the contents of a file as a c-string
// >> __TPP_LOAD_FILE(<stdio.h>)
#ifndef TPP_CONFIG_HAVE___TPP_LOAD_FILE
#define TPP_CONFIG_HAVE___TPP_LOAD_FILE 1
#endif /* !TPP_CONFIG_HAVE___TPP_LOAD_FILE */

//////////////////////////////////////////////////////////////////////////
// Variadic macros: __VA_ARGS__
// CHECK: #if __has_feature(tpp_va_args) || __has_extension(tpp_va_args)
// >> #define foo(...) __VA_ARGS__
#ifndef TPP_CONFIG_HAVE___VA_ARGS__
#define TPP_CONFIG_HAVE___VA_ARGS__ 1
#endif /* !TPP_CONFIG_HAVE___VA_ARGS__ */

//////////////////////////////////////////////////////////////////////////
// Variadic macros: __VA_ARGS__
// CHECK: #if __has_feature(tpp_named_va_args) || __has_extension(tpp_named_va_args)
// >> #define foo(args...) args
#ifndef TPP_CONFIG_HAVE_NAMED_VA_ARGS
#if TPP_CONFIG_HAVE___VA_ARGS__
#define TPP_CONFIG_HAVE_NAMED_VA_ARGS 1
#else
#define TPP_CONFIG_HAVE_NAMED_VA_ARGS 0
#endif
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */

//////////////////////////////////////////////////////////////////////////
// A small extension to the c/c++ standard proposed a while back:
// CHECK: #if __has_feature(tpp_va_comma) || __has_extension(tpp_va_comma)
// https://groups.google.com/a/isocpp.org/forum/#!topic/std-proposals/GtjR1TZnTJg
// >> Used in the same context, where "__VA_ARGS__" is used:
//    >> Expands to "" (nothing), if __VA_ARGS__ is empty
//    >> Expands to ",", if __VA_ARGS__ is non-empty
#ifndef TPP_CONFIG_HAVE___VA_COMMA__
#define TPP_CONFIG_HAVE___VA_COMMA__ 1
#endif /* !TPP_CONFIG_HAVE___VA_COMMA__ */

//////////////////////////////////////////////////////////////////////////
// Support to #@ (Charize) operator from vc / vc++
// CHECK: #if __has_feature(tpp_charize_operator) || __has_extension(tpp_charize_operator)
#ifndef TPP_CONFIG_HAVE_CHARIZE_OPERATOR
#define TPP_CONFIG_HAVE_CHARIZE_OPERATOR 1
#endif /* !TPP_CONFIG_HAVE_CHARIZE_OPERATOR */

//////////////////////////////////////////////////////////////////////////
// Sets the names of macro files during expansion (improves debug output)
// NOTE: This feature really slows down the preprocessor, since it
//       involves constant allocations, whenever a macro is called.
// >> And all it does, is telling you the names of macros, when
#ifndef TPP_CONFIG_NAME_MACRO_FILES
#define TPP_CONFIG_NAME_MACRO_FILES 0
#endif /* !TPP_CONFIG_NAME_MACRO_FILES */

//////////////////////////////////////////////////////////////////////////
// Re-Emit unknown #pragmas
// CHECK: #if __has_feature(tpp_reemit_unknown_pragmas) || __has_extension(tpp_reemit_unknown_pragmas)
#ifndef TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS
#define TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS 1
#endif /* !TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: __BASE_FILE__
// CHECK: #if defined(__BASE_FILE__)
#ifndef TPP_CONFIG_HAVE___BASE_FILE__
#define TPP_CONFIG_HAVE___BASE_FILE__ 1
#endif /* !TPP_CONFIG_HAVE___BASE_FILE__ */

//////////////////////////////////////////////////////////////////////////
// Builtin macros: "__has_include" / "__has_include_next"
// CHECK: #if defined(__has_include) / #if defined(__has_include_next)
#ifndef TPP_CONFIG_HAVE___has_include
#define TPP_CONFIG_HAVE___has_include 1
#endif /* !TPP_CONFIG_HAVE___has_include */

//////////////////////////////////////////////////////////////////////////
// Builtin macros: "__is_identifier"
// CHECK: #if defined(__is_identifier)
#ifndef TPP_CONFIG_HAVE___is_identifier
#define TPP_CONFIG_HAVE___is_identifier 1
#endif /* !TPP_CONFIG_HAVE___is_identifier */

//////////////////////////////////////////////////////////////////////////
// Special tpp - only extension: "__is_builtin_identifier"
// CHECK: #if defined(__is_builtin_identifier)
// - Similar to "__is_identifier", but only returns true,
//   if the identifier was builtin:
//   >> __is_identifier(foo)            // 0
//   >> __is_builtin_identifier(foo)    // 0
//   >> __is_identifier(undef)          // 1
//   >> __is_builtin_identifier(undef)  // 1
//   >> foo // Instantiate the "foo" identifier
//   >> __is_identifier(foo)            // 1
//   >> __is_builtin_identifier(foo)    // 0
#ifndef TPP_CONFIG_HAVE___is_builtin_identifier
#define TPP_CONFIG_HAVE___is_builtin_identifier 1
#endif /* !TPP_CONFIG_HAVE___is_builtin_identifier */

//////////////////////////////////////////////////////////////////////////
// More integer suffix types:
// CHECK: #if __has_feature(tpp_msvc_integer_suffix) || __has_extension(tpp_msvc_integer_suffix)
//  - i8, i16, i32, i64, ui8, ui16, ui32, ui64
#ifndef TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX
#define TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX 1
#endif /* !TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX */

//////////////////////////////////////////////////////////////////////////
// Enable trigraphs: ??=, ...
// CHECK: #if __has_feature(tpp_trigraphs) || __has_extension(tpp_trigraphs)
#ifndef TPP_CONFIG_HAVE_TRIGRAPHS
#define TPP_CONFIG_HAVE_TRIGRAPHS 0
#endif /* !TPP_CONFIG_HAVE_TRIGRAPHS */

//////////////////////////////////////////////////////////////////////////
// Enable digraphs: <%, <:, ...
// CHECK: #if __has_feature(tpp_digraphs) || __has_extension(tpp_digraphs)
#ifndef TPP_CONFIG_HAVE_DIGRAPHS
#define TPP_CONFIG_HAVE_DIGRAPHS 1
#endif /* !TPP_CONFIG_HAVE_DIGRAPHS */

//////////////////////////////////////////////////////////////////////////
// #warning directive
// CHECK: #if __has_feature(tpp_directive_warning) || __has_extension(tpp_directive_warning)
#ifndef TPP_CONFIG_HAVE_WARNING
#define TPP_CONFIG_HAVE_WARNING 1
#endif /* !TPP_CONFIG_HAVE_WARNING */

//////////////////////////////////////////////////////////////////////////
// #pragma push_macro(<string>)
// #pragma pop_macro(<string>)
// CHECK: #if __has_feature(tpp_pragma_push_macro) || __has_extension(tpp_pragma_push_macro)
//    OR: #if __has_feature(tpp_pragma_pop_macro) || __has_extension(tpp_pragma_pop_macro)
#ifndef TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
#define TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO 1
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */

//////////////////////////////////////////////////////////////////////////
// #pragma region ...
// #pragma endregion ...
// CHECK: #if __has_feature(tpp_pragma_region) || __has_extension(tpp_pragma_region)
//    OR: #if __has_feature(tpp_pragma_endregion) || __has_extension(tpp_pragma_endregion)
// >> Marks regions in code (We just ignore these pragma)
#ifndef TPP_CONFIG_HAVE_PRAGMA_REGION
#define TPP_CONFIG_HAVE_PRAGMA_REGION 1
#endif /* TPP_CONFIG_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// #pragma warning(...)
// CHECK: #if __has_feature(tpp_pragma_warning) || __has_extension(tpp_pragma_warning)
// >> Configure warnings emited by the preprocessor
#ifndef TPP_CONFIG_HAVE_PRAGMA_WARNING
#define TPP_CONFIG_HAVE_PRAGMA_WARNING 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_WARNING */

//////////////////////////////////////////////////////////////////////////
// #pragma message(...)
// CHECK: #if __has_feature(tpp_pragma_message) || __has_extension(tpp_pragma_message)
// >> Emits a message to stdout of the preprocessor
#ifndef TPP_CONFIG_HAVE_PRAGMA_MESSAGE
#define TPP_CONFIG_HAVE_PRAGMA_MESSAGE 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_MESSAGE */

//////////////////////////////////////////////////////////////////////////
// #pragma error(...)
// CHECK: #if __has_feature(tpp_pragma_error) || __has_extension(tpp_pragma_error)
// - Same as #pragma message, but the message string is
//   handled the same as a #error directive
#ifndef TPP_CONFIG_HAVE_PRAGMA_ERROR
#define TPP_CONFIG_HAVE_PRAGMA_ERROR 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_ERROR */

//////////////////////////////////////////////////////////////////////////
// Enables the "#pragma once" directive
// CHECK: #if __has_feature(tpp_pragma_once) || __has_extension(tpp_pragma_once)
#ifndef TPP_CONFIG_HAVE_PRAGMA_ONCE
#define TPP_CONFIG_HAVE_PRAGMA_ONCE 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_ONCE */

//////////////////////////////////////////////////////////////////////////
// Enables the "#pragma deprecated(<string>,...)" directive
// CHECK: #if __has_feature(tpp_pragma_deprecated) || __has_extension(tpp_pragma_deprecated)
#ifndef TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
#define TPP_CONFIG_HAVE_PRAGMA_DEPRECATED 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */

//////////////////////////////////////////////////////////////////////////
// Enables the "#pragma tpp_set_keyword_flags(<string>,int)" directive
// CHECK: #if __has_feature(tpp_pragma_tpp_set_keyword_flags) || __has_extension(tpp_pragma_tpp_set_keyword_flags)
//  - Can be used, to set the internal flags associated with a given keyword
//  - NOTE: This can be useful, if you have special libraries, that add
//          features to the compiler during compilation and you wan't
//          to be able, to query for those features.
#ifndef TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

//////////////////////////////////////////////////////////////////////////
// Builtin macros: "int __is_deprecated(Ident)"
// CHECK: #if defined(__is_deprecated)
//  - Can be used, to check if a given Identifier was marked with "#pragma deprecated"
// NOTE: Using "Ident" in this macro will not yield a deprecated warning.
#ifndef TPP_CONFIG_HAVE___is_deprecated
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
# define TPP_CONFIG_HAVE___is_deprecated 1
#else /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
# define TPP_CONFIG_HAVE___is_deprecated 0
#endif /* !TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#endif /* !TPP_CONFIG_HAVE___is_deprecated */


//////////////////////////////////////////////////////////////////////////
// Enabled the "#pragma tpp_exec(<string>)" directive
// CHECK: #if __has_feature(tpp_pragma_tpp_exec) || __has_extension(tpp_pragma_tpp_exec)
// - string will be executed in a protected environment, to define macros
//   NOTE: Tokens that would be emitted from the string will be ignored.
//   >> FOOBAR // expands to [FOOBAR]
//   >> #pragma tpp_exec("#define FOOBAR 42")
//   >> FOOBAR // expands to [42]
//   NOTE: Tested directives include:
//     - #define / #undef
//     - #if / #ifdef / #ifndef / #else / #elif / #endif
//   NOTE: Don't use this, to #include files. Weird stuff will happen and assertions will trigger!
//   NOTE: With this you can do some awesome stuff with self-defining macros:
//   >> #define __pragma(...) _Pragma(#__VA_ARGS__)
//   >> #define REDEF(m,b)  __pragma(tpp_exec("#undef " #m "\n#define " #m " " #b))
//   >> #define Word  w1
//   >> #define w1    REDEF(Word,w2)Hello
//   >> #define w2    REDEF(Word,w3)There
//   >> #define w3    REDEF(Word,w4)Good
//   >> #define w4    REDEF(Word,w1)Sir!
//   >> Word Word Word Word
//   >> Word Word Word Word
//   >> Word Word Word Word
//   NOTE: This can also be used for dynamic macro defining:
//   >> #include <TPP_REPEAT.h>
//   >> #define __ITER(i,d) __pragma(tpp_exec("#define foo_" #i " foo: " #i ""))
//   >> TPP_REPEAT(20,__ITER,~)
//   >> foo_0
//   >> foo_12
//   >> foo_7
#ifndef TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC
#define TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_attribute(Ident)
// CHECK: #if defined(__has_attribute)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_attribute
#define TPP_CONFIG_HAVE___has_attribute 1
#endif /* !TPP_CONFIG_HAVE___has_attribute */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_builtin(Ident)
// CHECK: #if defined(__has_builtin)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_builtin
#define TPP_CONFIG_HAVE___has_builtin 1
#endif /* !TPP_CONFIG_HAVE___has_builtin */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_cpp_attribute(Ident)
// CHECK: #if defined(__has_cpp_attribute)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_cpp_attribute
#define TPP_CONFIG_HAVE___has_cpp_attribute 1
#endif /* !TPP_CONFIG_HAVE___has_cpp_attribute */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_declspec_attribute(Ident)
// CHECK: #if defined(__has_declspec_attribute)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_declspec_attribute
#define TPP_CONFIG_HAVE___has_declspec_attribute 1
#endif /* !TPP_CONFIG_HAVE___has_declspec_attribute */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_extension(Ident)
// CHECK: #if defined(__has_extension)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_extension
#define TPP_CONFIG_HAVE___has_extension 1
#endif /* !TPP_CONFIG_HAVE___has_extension */

//////////////////////////////////////////////////////////////////////////
// Builtin macro: int __has_feature(Ident)
// CHECK: #if defined(__has_feature)
// NOTE: Whitespace and '_' Characters surrounding Ident are ignored
#ifndef TPP_CONFIG_HAVE___has_feature
#define TPP_CONFIG_HAVE___has_feature 1
#endif /* !TPP_CONFIG_HAVE___has_feature */


//////////////////////////////////////////////////////////////////////////
// Enables builtin support for the iso646 operator macros
// Disabled by default, As I've never seen anyone actually use these... (plus: there's always that header)
// CHECK: #if defined(and)...
#ifndef TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS
#define TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS 0
#endif /* !TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS */

//////////////////////////////////////////////////////////////////////////
// "~~" token
// CHECK: #if __has_feature(tpp_token_tilde_tilde) || __has_extension(tpp_token_tilde_tilde)
#ifndef TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
#define TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN 1
#endif /* !TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */

//////////////////////////////////////////////////////////////////////////
// "->" token
// CHECK: #if __has_feature(tpp_token_arrow) || __has_extension(tpp_token_arrow)
#ifndef TPP_CONFIG_HAVE_ARROW_TOKEN
#define TPP_CONFIG_HAVE_ARROW_TOKEN 1
#endif /* !TPP_CONFIG_HAVE_ARROW_TOKEN */

//////////////////////////////////////////////////////////////////////////
// ":=" token
// CHECK: #if __has_feature(tpp_token_collon_assign) || __has_extension(tpp_token_collon_assign)
#ifndef TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
#define TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN 1
#endif /* !TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN */

//////////////////////////////////////////////////////////////////////////
// functionality for "^^" in expressions (requires TPP_CONFIG_HAVE_LXOR_TOKEN)
// CHECK: #if __has_feature(tpp_lxor) || __has_extension(tpp_lxor)
#ifndef TPP_CONFIG_HAVE_LXOR
#define TPP_CONFIG_HAVE_LXOR 1
#endif /* !TPP_CONFIG_HAVE_LXOR */

//////////////////////////////////////////////////////////////////////////
// "^^" token
// CHECK: #if __has_feature(tpp_token_lxor) || __has_extension(tpp_token_lxor)
#ifndef TPP_CONFIG_HAVE_LXOR_TOKEN
#if TPP_CONFIG_HAVE_LXOR
#define TPP_CONFIG_HAVE_LXOR_TOKEN 1
#else
#define TPP_CONFIG_HAVE_LXOR_TOKEN 0
#endif
#endif /* !TPP_CONFIG_HAVE_LXOR_TOKEN */

#if TPP_CONFIG_HAVE_LXOR && !TPP_CONFIG_HAVE_LXOR_TOKEN
#error Require ^^ token to enable ^^ functionality
#endif

//////////////////////////////////////////////////////////////////////////
// "**" token
// CHECK: #if __has_feature(tpp_token_pow) || __has_extension(tpp_token_pow)
#ifndef TPP_CONFIG_HAVE_POW_TOKEN
#define TPP_CONFIG_HAVE_POW_TOKEN 1
#endif /* !TPP_CONFIG_HAVE_POW_TOKEN */

//////////////////////////////////////////////////////////////////////////
// "::" token
// CHECK: #if __has_feature(tpp_token_collon_collon) || __has_extension(tpp_token_collon_collon)
#ifndef TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
#ifdef TPP_CONFIG_HAVE_COLLON_COLLON // deprecated name
#define TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN TPP_CONFIG_HAVE_COLLON_COLLON
#else
#define TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN 1
#endif
#endif /* !TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN */

//////////////////////////////////////////////////////////////////////////
// Comment tokens
//  - This enables a flag in the parser to yield comments as regular tokens
// NOTE though, that files must be included without being formatted for this to work
// NOTE: This feature hasn't been fully tested (yet)
#ifndef TPP_CONFIG_HAVE_COMMENT_TOKEN
#define TPP_CONFIG_HAVE_COMMENT_TOKEN 1
#endif /* !TPP_CONFIG_HAVE_COMMENT_TOKEN */

//////////////////////////////////////////////////////////////////////////
// Adds 3 additional macro calling conventions to TPP:
// CHECK: #if __has_feature(tpp_macro_calling_conventions) || __has_extension(tpp_macro_calling_conventions)
// NOTE: Having this enabled will not change the behavior of existing macro calls,
//       because ( and ) are considered the weakest parens, not allowing for recursion
//       with [ ], { } or < >!
// NOTE: Like with normal function-style macros, a function is only defined, if
//       the macro name is directly followed by the calling convention token.
// - Parenthesis order: <{[( ... )]}>
// >> #define angel<x>   +x-
// >> #define brace{x}   +x-
// >> #define bracket[x] +x-
// >> #define paren(x)   +x- // Original (Unchanged)
// >> angel<<>>    // Expands to: [+][<][>][-]
// >> angel<<<>>   // ERROR: EOF in macro argument list
// >> angel<{<}>   // Expands to: [+][{][<][}][-]
// >> angel<[<]>   // Expands to: [+][[][<][]][-]
// >> angel<(<)>   // Expands to: [+][(][<][)][-]
// >> brace{<{>}   // ERROR: EOF in macro argument list
// >> brace{{{}}   // ERROR: EOF in macro argument list
// >> brace{[{]}   // Expands to: [+][[][{][]][-]
// >> brace{({)}   // Expands to: [+][(][{][)][-]
// >> bracket[<[>] // ERROR: EOF in macro argument list
// >> bracket[{[}] // ERROR: EOF in macro argument list
// >> bracket[[[]] // ERROR: EOF in macro argument list
// >> bracket[([)] // Expands to: [+][(][[][)][-]
// >> paren(<(>)   // ERROR: EOF in macro argument list
// >> paren({(})   // ERROR: EOF in macro argument list
// >> paren([(])   // ERROR: EOF in macro argument list
// >> paren((())   // ERROR: EOF in macro argument list
#ifndef TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
#define TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS 1
#endif /* !TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */


//////////////////////////////////////////////////////////////////////////
// When enabled, whitespace (including line-feed tokens) are
// stripped from arguments in macro function calls:
// CHECK: #if __has_feature(tpp_strict_whitespace) || __has_extension(tpp_strict_whitespace)
// >> #define my_func(a,b) a+b
// >> my_func( 10 ,
// >> 20)
// >> // Expands to (With TPP_CONFIG_STRICT_WHITESPACE):
// >> //  >> [ ][10][ ][,][LF][20]
// >> // Expands to (Without TPP_CONFIG_STRICT_WHITESPACE):
// >> //  >> [10][,][20]
// NOTE: Disabled again in v104, as this caused infinite recursion in:
// >> #define func(a) func(a ) // don't count whitespace here
// >> func(42)
#ifndef TPP_CONFIG_STRICT_WHITESPACE
#define TPP_CONFIG_STRICT_WHITESPACE 0
#endif /* !TPP_CONFIG_STRICT_WHITESPACE */

//////////////////////////////////////////////////////////////////////////
// Strict integer overflow checking:
//  - Integers constants will overflow, if they are not marked
//    with a type, that can fit all their data.
//  - If disabled, they will not be truncated based on their type:
// CHECK: #if __has_feature(tpp_strict_integer_overflow) || __has_extension(tpp_strict_integer_overflow)
// >> Enabled:
// >> 0xFFFFFFFFFFFFFFFF    // truncate to int                --> 0xFFFFFFFF
// >> 0xFFFFFFFFFFFFFFFFull // truncate to unsigned long long --> 0xFFFFFFFFFFFFFFFFull
// >> Disabled:
// >> 0xFFFFFFFFFFFFFFFF    // Not truncated
// >> 0xFFFFFFFFFFFFFFFFull // Not truncated
#ifndef TPP_CONFIG_STRICT_INTEGER_OVERFLOW
#define TPP_CONFIG_STRICT_INTEGER_OVERFLOW 1
#endif /* !TPP_CONFIG_STRICT_INTEGER_OVERFLOW */


//////////////////////////////////////////////////////////////////////////
// Optimizes TPP, by using a pre-generated character traits lookup table
// >> This improves runtime performance, but adds 257 bytes
//    of static const data to the executable.
#ifndef TPP_CONFIG_USE_CHARACTER_TRAITS
#define TPP_CONFIG_USE_CHARACTER_TRAITS 1
#endif /* !TPP_CONFIG_USE_CHARACTER_TRAITS */


//////////////////////////////////////////////////////////////////////////
// Accept ANSI characters in keywords (like the German 'ß')
// CHECK: #if __has_feature(tpp_support_ansi_characters) || __has_extension(tpp_support_ansi_characters)
#ifndef TPP_CONFIG_SUPPORT_ANSI_CHARACTERS
#define TPP_CONFIG_SUPPORT_ANSI_CHARACTERS 1
#endif /* !TPP_CONFIG_SUPPORT_ANSI_CHARACTERS */


//////////////////////////////////////////////////////////////////////////
// When enabled, line-feed tokens after directives will be emitted
// CHECK: #if __has_feature(tpp_emit_lf_after_directive) || __has_extension(tpp_emit_lf_after_directive)
#ifndef TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE
#define TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE 1
#endif /* !TPP_CONFIG_EMIT_LF_AFTER_DIRECTIVE */


//////////////////////////////////////////////////////////////////////////
// Enables the use of some crazy if statements in constant expressions:
// Similar to the ?-operator, but follows the standard c
// if-statement with its blocks replaced by expressions
// CHECK: #if __has_feature(tpp_if_cond_expression) || __has_extension(tpp_if_cond_expression)
// >> __TPP_EVAL((
// >>  if   (1) 10; // if true: evalues to 15
// >>  elif (0) 20; // if true: evalues to 25
// >>  elif (0) 30; // if true: evalues to 35
// >>  else     40; // else:    evalues to 45
// >> )+5)
// NOTE: Disabled by default, since this is just for fun and not really
//       something you should use if you want to remain compatible
#ifndef TPP_CONFIG_HAVE_IF_COND_EXPRESSION
#define TPP_CONFIG_HAVE_IF_COND_EXPRESSION 0
#endif /* !TPP_CONFIG_HAVE_IF_COND_EXPRESSION */



//////////////////////////////////////////////////////////////////////////
// Maximum amount of arguments in a single macro function
#ifndef TPP_CONFIG_MACRO_MAX_ARGC
#define TPP_CONFIG_MACRO_MAX_ARGC 1024
#endif /* !TPP_CONFIG_MACRO_MAX_ARGC */

// Maximum length of a #include string
#ifndef TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH
#define TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH PATH_MAX
#endif /* !TPP_CONFIG_MAX_INCLUDE_STRING_LENGTH */


//////////////////////////////////////////////////////////////////////////
// Determines whether or not tpp will generate 0x / 0X
#ifndef TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X
#define TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X  0
#endif /* !TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_0X */

//////////////////////////////////////////////////////////////////////////
// Determines whether or not tpp will generate 0xff / 0xFF
#ifndef TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX
#define TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX 1
#endif /* !TPP_CONFIG_QUOTE_STRING_USE_UPPERCASE_HEX */

//////////////////////////////////////////////////////////////////////////
// Enables debug-mode for tpp
// CHECK: #if __has_feature(tpp_debug) || __has_extension(tpp_debug)
#ifndef TPP_DEBUG
#if defined(_DEBUG)
# define TPP_DEBUG 1
#else
# define TPP_DEBUG 0
#endif
#endif /* !TPP_DEBUG */

//////////////////////////////////////////////////////////////////////////
// Compiles tpp with debug_new
// CHECK: #if __has_feature(tpp_debug_new) || __has_extension(tpp_debug_new)
#ifndef TPP_USE_DEBUG_NEW
#if defined(_DEBUG_NEW_H) || __has_include(<debug_new.h>)
# define TPP_USE_DEBUG_NEW 1
#else
# define TPP_USE_DEBUG_NEW 0
#endif
#endif /* !TPP_USE_DEBUG_NEW */


//#define TPP_CONFIG_HAVE_ALL_EXTENSIONS 1
#ifdef TPP_CONFIG_HAVE_ALL_EXTENSIONS
#undef TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS 
#undef TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS 
#undef TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS 
#undef TPP_CONFIG_HAVE_CHARIZE_OPERATOR 
#undef TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN 
#undef TPP_CONFIG_HAVE_DIGRAPHS 
#undef TPP_CONFIG_HAVE_DIRECTIVE_IMPORT 
#undef TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT 
#undef TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA 
#undef TPP_CONFIG_HAVE_IF_COND_EXPRESSION 
#undef TPP_CONFIG_HAVE_LXOR
#undef TPP_CONFIG_HAVE_LXOR_TOKEN
#undef TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS 
#undef TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX 
#undef TPP_CONFIG_HAVE_NAMED_VA_ARGS 
#undef TPP_CONFIG_HAVE_PRAGMA_DEPRECATED 
#undef TPP_CONFIG_HAVE_PRAGMA_ERROR 
#undef TPP_CONFIG_HAVE_PRAGMA_MESSAGE 
#undef TPP_CONFIG_HAVE_PRAGMA_ONCE 
#undef TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO 
#undef TPP_CONFIG_HAVE_PRAGMA_REGION 
#undef TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC 
#undef TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS 
#undef TPP_CONFIG_HAVE_PRAGMA_WARNING 
#undef TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
#undef TPP_CONFIG_HAVE_ARROW_TOKEN
#undef TPP_CONFIG_HAVE_TRIGRAPHS 
#undef TPP_CONFIG_HAVE_WARNING 
#undef TPP_CONFIG_HAVE___BASE_FILE__ 
#undef TPP_CONFIG_HAVE___COUNTER__ 
#undef TPP_CONFIG_HAVE___INCLUDE_DEPTH__ 
#undef TPP_CONFIG_HAVE___TIMESTAMP__ 
#undef TPP_CONFIG_HAVE___TPP_COUNTER 
#undef TPP_CONFIG_HAVE___TPP_EVAL 
#undef TPP_CONFIG_HAVE___TPP_LOAD_FILE 
#undef TPP_CONFIG_HAVE___TPP_RANDOM 
#undef TPP_CONFIG_HAVE___TPP_STR_AT 
#undef TPP_CONFIG_HAVE___TPP_STR_DECOMPILE 
#undef TPP_CONFIG_HAVE___TPP_STR_PACK 
#undef TPP_CONFIG_HAVE___TPP_STR_SIZE 
#undef TPP_CONFIG_HAVE___TPP_STR_SUBSTR 
#undef TPP_CONFIG_HAVE___VA_ARGS__ 
#undef TPP_CONFIG_HAVE___VA_COMMA__ 
#undef TPP_CONFIG_HAVE___has_attribute 
#undef TPP_CONFIG_HAVE___has_builtin 
#undef TPP_CONFIG_HAVE___has_cpp_attribute 
#undef TPP_CONFIG_HAVE___has_declspec_attribute 
#undef TPP_CONFIG_HAVE___has_extension 
#undef TPP_CONFIG_HAVE___has_feature 
#undef TPP_CONFIG_HAVE___has_include 
#undef TPP_CONFIG_HAVE___is_builtin_identifier 
#undef TPP_CONFIG_HAVE___is_deprecated 
#undef TPP_CONFIG_HAVE___is_identifier 
#define TPP_CONFIG_HAVE_ADDITIONAL_DATE_MACROS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_ADDITIONAL_TIME_MACROS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_CHARIZE_OPERATOR TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_DIGRAPHS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_DIRECTIVE_IMPORT TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_DOLLAR_AS_ALPHA TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_IF_COND_EXPRESSION TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_LXOR TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_LXOR_TOKEN TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_NAMED_VA_ARGS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_DEPRECATED TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_ERROR TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_MESSAGE TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_ONCE TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_REGION TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_TPP_EXEC TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_PRAGMA_WARNING TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_ARROW_TOKEN TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_TRIGRAPHS TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE_WARNING TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___BASE_FILE__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___COUNTER__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___INCLUDE_DEPTH__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TIMESTAMP__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_COUNTER TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_EVAL TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_LOAD_FILE TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_RANDOM TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_STR_AT TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_STR_DECOMPILE TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_STR_PACK TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_STR_SIZE TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___TPP_STR_SUBSTR TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___VA_ARGS__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___VA_COMMA__ TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_attribute TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_builtin TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_cpp_attribute TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_declspec_attribute TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_extension TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_feature TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___has_include TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___is_builtin_identifier TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___is_deprecated TPP_CONFIG_HAVE_ALL_EXTENSIONS
#define TPP_CONFIG_HAVE___is_identifier TPP_CONFIG_HAVE_ALL_EXTENSIONS
#endif /* TPP_CONFIG_HAVE_ALL_EXTENSIONS */




#if TPP_CONFIG_HAVE___has_attribute || \
    TPP_CONFIG_HAVE___has_builtin || \
    TPP_CONFIG_HAVE___has_cpp_attribute || \
    TPP_CONFIG_HAVE___has_declspec_attribute || \
    TPP_CONFIG_HAVE___has_extension || \
    TPP_CONFIG_HAVE___has_feature || \
    TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
#define TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS 1
#else
#define TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS 0
#endif


#define TPP_PP_STRINGIZE2(x) #x
#define TPP_PP_STRINGIZE(x) TPP_PP_STRINGIZE2(x)
#define TPP_PP_CAT2(x,y) x ## y
#define TPP_PP_CAT(x,y) TPP_PP_CAT2(x,y)
#if defined(__TPP_COUNTER)
// Why not? Lets use some TPP features!
# define TPP_STATIC_ASSERT_ID()  TPP_PP_CAT(__STATIC_ASSERT,__TPP_COUNTER(TPP_STATIC_ASSERT))
#elif defined(__COUNTER__)
# define TPP_STATIC_ASSERT_ID()  TPP_PP_CAT(__STATIC_ASSERT,__COUNTER__)
#else
# define TPP_STATIC_ASSERT_ID()  TPP_PP_CAT(__STATIC_ASSERT,__LINE__)
#endif
#define TPP_STATIC_ASSERT(expr,msg) \
 typedef int TPP_STATIC_ASSERT_ID()[!!(expr) ? 1 : 0]

#if defined(_MSC_VER)
#undef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#undef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS 1
#define TPP_DECLS_BEGIN_DISABLE_WARNINGS \
 __pragma(warning(push))\
 __pragma(warning(disable:4820)) /* 'x' bytes padding added after data member 'y' */\
 __pragma(warning(disable:4061)) /* enumerator 'x' in switch of enum 'y' is not explicitly handled by a case label */\
 __pragma(warning(disable:4255)) /* 'x' : no function prototype given: converting '()' to '(void)' */\
 __pragma(warning(disable:4996)) /* 'x': This function is part of the PRIVATE TPP Api and should no longer be considered as PUBLIC */\
 __pragma(warning(disable:4127)) /* conditional expression is constant */
# define TPP_DECLS_END_DISABLE_WARNINGS\
 __pragma(warning(pop))
#endif

#if defined(__GNUC__)
#define TPP_COMPILER_HAVE_GCC_VERSION(major,minor) \
 (__GNUC__ > major || (__GNUC__ == major && __GNUC_MINOR__ >= minor))
#else
#define TPP_COMPILER_HAVE_GCC_VERSION(major,minor) 0
#endif

#ifndef TPP_BUILTIN_UNREACHABLE
#if __has_builtin(__builtin_unreachable) || \
   (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)))
# define TPP_BUILTIN_UNREACHABLE   __builtin_unreachable
#elif defined(_MSC_VER)
# define TPP_BUILTIN_UNREACHABLE() __assume(0)
#else
# define TPP_BUILTIN_UNREACHABLE() (void)0
#endif
#endif /* !TPP_BUILTIN_UNREACHABLE */

#ifndef TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO
#if defined(_MSC_VER) || \
    __has_feature(__tpp_pragma_push_macro__) || \
   (defined(__TPP_VERSION__) && __TPP_VERSION__ == 103) // Before "__has_feature"
# define TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO 1
#else
# define TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO 0
#endif
#endif /* !TPP_CONFIG_COMPILER_HAS_PRAGMA_PUSH_MACRO */

#if !defined(_MSC_VER)
// vc doesn't define PATH_MAX in limits anyways...
#include <limits.h>
#endif /* !_MSC_VER */

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif /* !PATH_MAX */

#ifndef TPP_LOAD_STATIC
#define TPP_LOAD_STATIC 0
#endif /* !TPP_LOAD_STATIC */

#ifndef TPP_ATTRIBUTE_NORETURN
#if __has_attribute(__noreturn__) || \
    TPP_COMPILER_HAVE_GCC_VERSION(2,5)
# define TPP_ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#elif __has_declspec_attribute(noreturn) || defined(_MSC_VER)
# define TPP_ATTRIBUTE_NORETURN __declspec(noreturn)
#else
# define TPP_ATTRIBUTE_NORETURN /* nothing */
#endif
#endif /* !TPP_ATTRIBUTE_NORETURN */

#ifndef TPP_ATTRIBUTE_DEPRECATED
#if __has_attribute(__deprecated__) || \
    TPP_COMPILER_HAVE_GCC_VERSION(3,1)
#define TPP_ATTRIBUTE_DEPRECATED       __attribute__((__deprecated__))
#if TPP_COMPILER_HAVE_GCC_VERSION(4,5) || defined(__DEEMON__)
#define TPP_ATTRIBUTE_DEPRECATED_(msg) __attribute__((__deprecated__(msg)))
#else
#define TPP_ATTRIBUTE_DEPRECATED_(msg) __attribute__((__deprecated__))
#endif
#elif __has_declspec_attribute(deprecated) || \
     (defined(_MSC_VER) && _MSC_VER >= 1200)
#define TPP_ATTRIBUTE_DEPRECATED       __declspec(deprecated)
#define TPP_ATTRIBUTE_DEPRECATED_(msg) __declspec(deprecated(msg))
#else
#define TPP_ATTRIBUTE_DEPRECATED       /* nothing */
#define TPP_ATTRIBUTE_DEPRECATED_(msg) /* nothing */
#endif
#endif /* !TPP_ATTRIBUTE_DEPRECATED */

#define TPP_ATTRIBUTE_DEPRECATED_USE(new_func)\
 TPP_ATTRIBUTE_DEPRECATED_("Deprected : Please use : " #new_func)

#ifndef TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API
#if !defined(GUARD_TPP_C) && !TPP_LOAD_STATIC
#define TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API \
 TPP_ATTRIBUTE_DEPRECATED_("This function is part of the PRIVATE TPP Api and should no longer be considered as PUBLIC")
#else
#define TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API /* nothing */
#endif
#endif /* !TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API */

#ifndef TPP_CONFIG_HAVE_DEPRECATED_API
#define TPP_CONFIG_HAVE_DEPRECATED_API 1
#endif /* !TPP_CONFIG_HAVE_DEPRECATED_API */


#ifndef TPP_COMPILER_HAVE_SAL_H
#if defined(_PREFAST_)
# define TPP_COMPILER_HAVE_SAL_H 1
#else
# define TPP_COMPILER_HAVE_SAL_H 0
#endif
#endif /* !TPP_COMPILER_HAVE_SAL_H */

//////////////////////////////////////////////////////////////////////////
// Indicates, that the returned value must be used in some way
#ifndef TPP_ATTRIBUTE_USE_RETURN
#if TPP_COMPILER_HAVE_SAL_H
#include <sal.h>
#ifdef _Check_return_
# define TPP_ATTRIBUTE_USE_RETURN _Check_return_
#endif /* _Check_return_ */
#elif __has_attribute(__warn_unused_result__) || defined(__GNUC__)
# define TPP_ATTRIBUTE_USE_RETURN __attribute__((__warn_unused_result__))
#endif
#ifndef TPP_ATTRIBUTE_USE_RETURN
# define TPP_ATTRIBUTE_USE_RETURN /* nothing */
#endif /* !TPP_ATTRIBUTE_USE_RETURN */
#endif /* !TPP_ATTRIBUTE_USE_RETURN */

//////////////////////////////////////////////////////////////////////////
// The return value should be used
// (just because that's the only reason, you would call the function!)
#ifndef TPP_ATTRIBUTE_SHOULD_USE_RETURN
#define TPP_ATTRIBUTE_SHOULD_USE_RETURN TPP_ATTRIBUTE_USE_RETURN
#endif /* !TPP_ATTRIBUTE_SHOULD_USE_RETURN */


#if TPP_COMPILER_HAVE_SAL_H
#include <sal.h>
#if !defined(TPP_ATTRIBUTE_MSVC_ZERO_STRING) && defined(_In_z_)
# define TPP_ATTRIBUTE_MSVC_ZERO_STRING _In_z_
#endif /* !TPP_ATTRIBUTE_MSVC_ZERO_STRING && _In_z_ */
#if !defined(TPP_ATTRIBUTE_MSVC_PRINTF_STRING) && defined(_Printf_format_string_)
# define TPP_ATTRIBUTE_MSVC_PRINTF_STRING TPP_ATTRIBUTE_MSVC_ZERO_STRING _Printf_format_string_
#endif /* !TPP_ATTRIBUTE_MSVC_PRINTF_STRING && _Printf_format_string_ */
#if !defined(TPP_ATTRIBUTE_MSVC_BUFFER_VAR) && defined(_Out_writes_)
# define TPP_ATTRIBUTE_MSVC_BUFFER_VAR(max,s) _Out_writes_bytes_to_opt_(max,s)
#endif /* !TPP_ATTRIBUTE_MSVC_BUFFER_VAR && _Out_writes_ */
#if !defined(TPP_ATTRIBUTE_MSVC_NON_NULL) && defined(_Notnull_)
# define TPP_ATTRIBUTE_MSVC_NON_NULL _Notnull_
#endif /* !TPP_ATTRIBUTE_MSVC_NON_NULL && _Notnull_ */
#endif /* TPP_COMPILER_HAVE_SAL_H */

#ifndef TPP_ATTRIBUTE_MSVC_ZERO_STRING
#define TPP_ATTRIBUTE_MSVC_ZERO_STRING /* nothing */
#endif /* !TPP_ATTRIBUTE_MSVC_ZERO_STRING */
#ifndef TPP_ATTRIBUTE_MSVC_PRINTF_STRING
#define TPP_ATTRIBUTE_MSVC_PRINTF_STRING TPP_ATTRIBUTE_MSVC_ZERO_STRING
#endif /* !TPP_ATTRIBUTE_MSVC_PRINTF_STRING */
#ifndef TPP_ATTRIBUTE_MSVC_BUFFER_VAR
#define TPP_ATTRIBUTE_MSVC_BUFFER_VAR(max,s) /* nothing */
#endif /* !TPP_ATTRIBUTE_MSVC_BUFFER_VAR */
#ifndef TPP_ATTRIBUTE_MSVC_NON_NULL
#define TPP_ATTRIBUTE_MSVC_NON_NULL /* nothing */
#endif /* !TPP_ATTRIBUTE_MSVC_NON_NULL */

#ifndef TPP_ATTRIBUTE_GCC_NON_NULL
#if __has_attribute(__nonnull__) || \
    TPP_COMPILER_HAVE_GCC_VERSION(3,3)
# define TPP_ATTRIBUTE_GCC_NON_NULL(args) __attribute__((__nonnull__ args))
#else /* __GNUC__ */
# define TPP_ATTRIBUTE_GCC_NON_NULL(args) /* nothing */
#endif /* !__GNUC__ */
#endif /* !TPP_ATTRIBUTE_GCC_NON_NULL */


#ifndef TPP_ATTRIBUTE_GCC_PRINTF_STRING
#if __has_attribute(__format__) || \
    defined(__GNUC__)
# define TPP_ATTRIBUTE_GCC_PRINTF_STRING(fmt,va_args) __attribute__((__format__(printf,fmt,va_args)))
#else
# define TPP_ATTRIBUTE_GCC_PRINTF_STRING(fmt,va_args) /* nothing */
#endif
#endif /* !TPP_ATTRIBUTE_GCC_PRINTF_STRING */

#ifndef TPP_DECL_STATIC_INLINE
#if defined(__cplusplus)
# define TPP_DECL_STATIC_INLINE(return) inline /*static*/ return
#elif defined(_MSC_VER)
# define TPP_DECL_STATIC_INLINE(return) __inline /*static*/ return
#elif defined(__GNUC__)
# define TPP_DECL_STATIC_INLINE(return) __inline static return
#elif defined(__DEEMON__)
# define TPP_DECL_STATIC_INLINE(return) return
#else
# define TPP_DECL_STATIC_INLINE(return) static return
#endif
#endif /* !TPP_DECL_STATIC_INLINE */

#ifndef TPP_DECL_ENUM_WITH_BASE
#if defined(__DEEMON__)
# define TPP_DECL_ENUM_WITH_BASE(name,base) enum name: base
#else
# define TPP_DECL_ENUM_WITH_BASE(name,base) typedef base name; enum
#endif
#endif /* TPP_DECL_ENUM_WITH_BASE */

#ifndef TPP_DECL_FUNC
#if TPP_LOAD_STATIC
# define TPP_DECL_DATA(decl)      /* nothing */
# define TPP_IMPL_DATA(decl)      static decl
# define TPP_DECL_FUNC(return)    static return
# define TPP_IMPL_FUNC(return)    static return
#else /* TPP_LOAD_STATIC */
# define TPP_DECL_DATA(decl)      extern decl
# define TPP_IMPL_DATA(decl)             decl
# define TPP_DECL_FUNC(return)    extern return
# define TPP_IMPL_FUNC(return)           return
#endif /* !TPP_LOAD_STATIC */
#endif /* !TPP_DECL_FUNC */

#ifndef TPP_DECLS_BEGIN_DISABLE_WARNINGS
#define TPP_DECLS_BEGIN_DISABLE_WARNINGS /* nothing */
#endif /* !TPP_DECLS_BEGIN_DISABLE_WARNINGS */
#ifndef TPP_DECLS_END_DISABLE_WARNINGS
#define TPP_DECLS_END_DISABLE_WARNINGS /* nothing */
#endif /* !TPP_DECLS_END_DISABLE_WARNINGS */

// Value of the "__TPP_VERSION__" builtin macro
#define TPP_PREPROCESSOR_VERSION 106 // Preprocessor version
#define TPP_API_VERSION          106 // Api version (Version of this api)


#endif /* !GUARD_TPP_CONF_INL */
