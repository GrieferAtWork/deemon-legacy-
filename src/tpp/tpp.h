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
#ifndef GUARD_TPP_H
#define GUARD_TPP_H 1

#include "tpp_conf.inl"
#if TPP_USE_DEBUG_NEW
#include <debug_new.h>
#endif /* TPP_USE_DEBUG_NEW */

#if TPP_USE_DEBUG_NEW
#include <debug_new_disable.inl>
#endif /* TPP_USE_DEBUG_NEW */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifndef TPP_ASSERT
#if TPP_DEBUG
# include <assert.h>
# define TPP_ASSERT assert
#else /* TPP_DEBUG */
# if defined(_MSC_VER)
#  define TPP_ASSERT      __assume
# elif defined(__clang__) && __has_builtin(__builtin_assume)
#  define TPP_ASSERT      __builtin_assume
# elif defined(__GNUC__)
#  define TPP_ASSERT(x)   (!(x)?TPP_BUILTIN_UNREACHABLE():(void)0)
# else
#  define TPP_ASSERT(x)   (void)0
#  define TPP_VALIDATE(x) x
# endif
#endif /* !TPP_DEBUG */
#endif /* !TPP_ASSERT */
#ifndef TPP_VALIDATE
# define TPP_VALIDATE(x) (TPP_ASSERT(x),x)
#endif /* !TPP_VALIDATE */
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#if TPP_USE_DEBUG_NEW
#include <debug_new_enable.inl>
#endif /* TPP_USE_DEBUG_NEW */


//////////////////////////////////////////////////////////////////////////
// TPP - Tiny PreProcessor
//   Brought to you by Cyder aka. Griefer@Work
//////////////////////////////////////////////////////////////////////////
// >> TPP is meant as an easy-to-use, yet fully
//    featured preprocessor for c / c++ based languages.
//    - Using TPP as a Lexer, you can easily create your
//      own scripting languages, without having, to worry
//      about reading code from files, keeping track of
//      file locations or file dependencies.
// >> Implements everything from the c standard and more:
//    - [STD] Support for: c-style /* ... */ comments
//    - [STD] Support for: c++-style // ... comments
//    - [STD] Support for: # (stringize token)
//    - [STD] Support for: ## (Token concat)
//    - [STD] Support for: __LINE__
//    - [STD] Support for: __FILE__
//    - [STD] Support for: __DATE__
//    - [STD] Support for: __TIME__
//    - [STD] Support for: trigraphs
//    - [STD] Support for: digraphs
//    - [STD] Support for: __VA_ARGS__
//    - [STD] Support for: #ifdef / #ifndef / #if / #elif / #else / #endif
//    - [STD] Support for: #define / #undef
//    - [STD] Support for: #line
//    - [STD] Support for: #include
//    - [STD] Support for: #error
//    - [STD] Support for: #pragma / _Pragma(<string>)
//    - [EXT] Support for: __COUNTER__
//    - [EXT] Support for: __BASE_FILE__
//    - [EXT] Support for: __INCLUDE_DEPTH__ / __INCLUDE_LEVEL__
//    - [EXT] Support for: named __VA_ARGS__
//    - [EXT] Support for: iX / uiX integer suffix
//    - [EXT] Support for: #import
//    - [EXT] Support for: #include_next
//    - [EXT] Support for: #warning
//    - [EXT] Support for: #pragma once
//    - [EXT] Support for: #pragma warning
//    - [EXT] Support for: #pragma message
//    - [EXT] Support for: #pragma push_macro / #pragma pop_macro
//    - [EXT] Support for: #pragma region / #pragma endregion
//    - [EXT] Support for: #pragma deprecated
//    - [EXT] Support for: #@ (charize token)
//    - [EXT] Support for: Multi-char constants
//    - [EXT] Support for: __has_attribute(<ident>)
//    - [EXT] Support for: __has_builtin(<ident>)
//    - [EXT] Support for: __has_cpp_attribute(<ident>)
//    - [EXT] Support for: __has_declspec_attribute(<ident>)
//    - [EXT] Support for: __has_extension(<ident>)
//    - [EXT] Support for: __has_feature(<ident>)
//    - [EXT] Support for: __has_include(<include_str>) / __has_include_next(<include_str>)
//    - [EXT] Support for: __is_identifier(<ident>)
//    - [TPP] Support for: __is_builtin_identifier(<ident>)
//    - [TPP] Support for: __is_deprecated(<ident>)
//    - [TPP] Support for: __TIME_SEC__, __TIME_MIN__ and __TIME_HOUR__
//    - [TPP] Support for: __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__ and __DATE_YEAR__
//    - [TPP] Support for: __TPP_COUNTER(<keyword>)
//    - [TPP] Support for: __TPP_RANDOM(<Integer>[,<Integer>])
//    - [TPP] Support for: __TPP_EVAL(<Integer>)
//    - [TPP] Support for: __TPP_LOAD_FILE(<include_str>[,<Integer>])
//    - [TPP] Support for: __TPP_STR_DECOMPILE(String)
//    - [TPP] Support for: __VA_COMMA__
//    - [TPP] Support for: #pragma error
//    - [TPP] Support for: Additional macro calling conventions
// STD: standard
// EXT: common compiler extension
// TPP: TPP specific extension
//////////////////////////////////////////////////////////////////////////
// >> Implementation notes:
//    - TPP is __NOT__ meant as an external / shared library, as
//      language-specific keyword must be added to "TPP_KEYWORD_FILE_N()" files,
//      followed by recompiling and relinking "tpp.c"
//    - You can however link is statically and have
//      multiple version in different compilation units!
//      >> s.a.: "TPP_LOAD_STATIC"
//    - TPP can be compiled in c / c++ and is capable of using <debug_new> (another of my projects)
//      - When compiling in c++, it is recommended, to use the "tpp.hpp" header
//    - TPP does not set a hard limit on macro call depth (unlike MSVC at 256).
//    - Strings are not merged by the preprocessor, as that would interfere with
//      the way tokens store their data:
//      >> #define TEXT2 "bar"
//      >> "foo" TEXT2
//      >> // expands to ["foo"][TEXT2]
//      >> // expands to ["foo"]["bar"]
//      >> // >> Where "bar" is located in a different data block ([macro-]file)
//    - TPP doesn't support the ''' token, which C11 introduced as an alias for '\''
//    - (Almost) every extension TPP adds can be disabled in the config, when compiling,
//      allowing TPP to be turned into a clean C11 compatible preprocessor
//      NOTE: Although I wouldn't recomment disabling them, since all of them are designed
//            with full compatibility to existing code, only adding new features.
//    - TPP defines a very clean namespace and will only ever predefine macros
//      with a "__TPP" prefix, where keyword-style macros will always end with 2 "__".
//      - Exceptions to this rule are extension keywords, like "__has_include",
//        that I didn't come up with myself and am keeping the name, to ensure
//        compatibility with existing code.
//      - Another exception are the additional time / date macros,
//        which use the names of their origin macros as base name.
//////////////////////////////////////////////////////////////////////////
// >> Implementation details:
//    - TPP has a __VERY__ small memory footprint,
//      rarely taking up more that 1-4 MB of memory.
//      - This is due the fact, that instead of copying code into various buffers,
//        TPP will only store the original file and tokens only keep a reference
//        and 2 pointers to their begin and end in that file.
//    - Unlike other preprocessors, TPP allows for function-style macros,
//      to recursively call themselves, if the arguments are different from
//      any previous call in the macro call stack
//      !!!!!CAUTION!!!!!
//      >> #define foo(...) foo(__VA_ARGS__,more)
//      >> #define bar(...) bar(__VA_ARGS__)
//      >> foo(42) // INFINITE RECURSION (currently not detectable; preprocessor will freeze)
//      >> bar(42) // OK: expands to "bar(42)"
//    - TPP includes an optimized detection algorithm for #include guards:
//      - If a file starts with "#ifndef <KEYWORD>" and ends with "#endif" without an
//        "#elif" or "#else" in the same scope, the file will not be #included
//        again, until the <KEYWORD> is #undef'ed.
//      - HINT: #pragma once does the same, but sets <KEYWORD> to the builtin macro __LINE__
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// >> Some Non-C tokens are included:
//    - "^^" LXOR token (imagined as: logical xor)
//    - "~~" TILDE_TILDE token (imagined as: inplace-version of "~", like "--" is for "-")
//////////////////////////////////////////////////////////////////////////
// >> Warnings emitted by the preprocessor can be enabled / disabled
//    with the "#pragma warning" directive:
//    - #undef __LINE__ // Warning: can't #undef builtin macro
//    - #pragma warning(push)
//    - #pragma warning(disable:37)
//    - #undef __LINE__ // No warning (but it still won't work...)
//    - #pragma warning(pop)
//////////////////////////////////////////////////////////////////////////
// >> Multi-char constants:
//    >> 'AAAA' == 0x41414141
//    >> #define QUOTE2(x) #x
//    >> #define QUOTE(x) QUOTE2(x)
//    >> #pragma message(QUOTE(__TPP_EVAL('AAAA')))
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// >> In addition some custom extensions are included:
//////////////////////////////////////////////////////////////////////////
//    - Additional Macro calling conventions
//      - Macro functions can be defined using 4 different parenthesis styles
//        >> #define paren(x,y,z)   // STDC way: using parenthesis
//        >> #define bracket[x,y,z] // TPP extension #1: brackets
//        >> #define brace{x,y,z}   // TPP extension #2: braces
//        >> #define angle<x,y,z>   // TPP extension #3: angle brackets
//      - For more indepth information about parenthesis recursion and
//        usage examples, look at "TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS"
//        and "pp_test_20_macro_call_conv.h"
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Integer __TPP_EVAL(expr)
//      - Evaluates a constant integer expression to 1/2 tokens:
//        >> __TPP_EVAL(10+20) // Expands to [30]
//        >> __TPP_EVAL(10-20) // Expands to [-][10]
//      - Useful, as the Result can be reused for other Preprocessor operations:
//        >> #define CAT2(a,b) a ## b
//        >> #define CAT(a,b) CAT2(a,b)
//        >> CAT(foo,__TPP_EVAL(42*10)) // Expands to foo420
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Integer __is_builtin_identifier(Ident)
//      - Expands to 0 / 1 if "Ident" is a builtin identifier
//      - Builtin refers to all keyword IDs != 0 and below "TPP_TOK_USER_IDENT_START"
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Integer __TPP_RANDOM(Integer max) // same as __TPP_COUNTER(0,max)
//    - [Macro] Integer __TPP_RANDOM(Integer min, Integer max)
//      - Expands to a random Integer between "min" and "max"
//        >> __TPP_RANDOM(5) // Can expand to 0,1,2,3,4 or 5
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Integer __TPP_COUNTER(Keyword kwd)
//      - Expands to a different value every it is called
//      - Unlike "__COUNTER__", this macro will only expand to different
//        values for the same "kwd":
//      >> // NOTE: Assuming "foo" and "bar" havn't been used yet
//      >> __TPP_COUNTER(foo) // expands to 0
//      >> __TPP_COUNTER(foo) // expands to 1
//      >> __TPP_COUNTER(bar) // expands to 0
//      >> __TPP_COUNTER(bar) // expands to 1
//      >> __TPP_COUNTER(foo) // expands to 2
//      >> __TPP_COUNTER(bar) // expands to 2
//////////////////////////////////////////////////////////////////////////
//    - [Macro] String __TPP_LOAD_FILE(<include_string>,Integer include_next = 0)
//      - Expands to a string representation of the file described by "<include_string>"
//      - The file can be of binary or text format, as it will be returned
//        with escape codes encoded, like any normal string.
//      - NOTE: Macros, directives or comments are __NOT__ executed or removed
//      - Useful, to hard-code script into applications, without having to manually
//        encode all the escape codes into the code.
//      >> // Returns a string representation of "stdio.h"
//      >> static char stdio_text[] = __TPP_LOAD_FILE(<stdio.h>);
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Tokens ...__TPP_STR_DECOMPILE(String str)
//      - Decompiles a string into the tokens making up the text
//      - Being the inverse of the # (STRINGIZE) operator, you can convert
//        a string back into its original tokens.
//      >> __TPP_STR_DECOMPILE("int x = 10+20") // Expands to: [int][ ][x][ ][=][ ][10][+][20]
//////////////////////////////////////////////////////////////////////////
//    - [Macro] Integer __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__
//      - Expand to different integers based on the current time.
//      - HINT: Useful in conjunction with the stringify operator, to
//              create custom time representations.
//      >> __TIME_SEC__  // expands to 0..59
//      >> __TIME_MIN__  // expands to 0..59
//      >> __TIME_HOUR__ // expands to 0..23
//    - [Macro] Integer __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__ and __DATE_YEAR__
//      - Similar to the additional __TIME__ macros, but for __DATE__.
//      >> __DATE_DAY__   // expands to 1..31
//      >> __DATE_WDAY__  // expands to 1..7
//      >> __DATE_YDAY__  // expands to 1..365
//      >> __DATE_MONTH__ // expands to 1..12
//      >> __DATE_YEAR__  // expands to 2015..2496
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Version History
//////////////////////////////////////////////////////////////////////////
// API v106 / TPP v106 (deemon prerelease version #2)
//   - Added New functions for move/copy initialization of the entire lexer
//   - Fixed __DATE_MONTH__ being 0-based even though the documentation states that it's
//     one-based. (Sorry about that; it's 1-based now...)
//   - Fixed a bug that caused tpp to sometimes yield an EOF when detecting an #include guard
//   - Added the ability to define custom callbacks for error situations (like no-memory, syntax error, etc.)
//   - Fixed a bug that caused token concat to sometimes not deactivate
//     >> #define loc(x)  x##_t k
//     >> loc(int32); // wrongly expanded to [int32_tk]; now correctly expands to [int32_t][ ][k]
//   - Fixed a bug that caused __TPP_RANDOM not to restore compiler flags
//   - Fixed floating point numbers like '.123' being processed as integers, causing invalid-int-format errors
//   - Fixed a crash when the last token was the name of a macro function
//   - Fixed a crash when a file ends with an unclosed /* ... */ - style comment
//   - Fixed a bug that may have caused P0032 and P0033 to incorrectly be generated
//   - Fixed a bug where integer overflow was wrongly detected
//   - Fixed a bug that may have caused some builtin keywords to be loaded more than once
//   - Fixed a bug when evaluating floating point numbers like '42.'
//////////////////////////////////////////////////////////////////////////
// API v105 / TPP v105 (deemon prerelease version)
//   - Fixed a bug where the last character of a define at the end of a file got dropped
//   - Fixed a bug where tpp would have consumed linefeed when used to continue line-comments
//   - Added support for the arrow token "->" (s.a. TPP_CONFIG_HAVE_ARROW_TOKEN)
//   - Fixed a reference leak in "TPPLexer_IncludeStringS"
//   - Added warnings if #if expression doesn't evaluate to 0 / 1
//    - Similar warnings are emitted if operands of ||, &&, ^^ or ! are not 0 / 1
//   - Added support for named __VA_ARGS__:
//     >> #define error_printf(fmt,args...) fprintf(stderr,fmt __VA_COMMA__ args)
//     >> // Same as:
//     >> #define error_printf(fmt,...)     fprintf(stderr,fmt __VA_COMMA__ __VA_ARGS__)
//   - Added support for more intrinsic string macros:
//     >> Character __TPP_STR_AT(String/Character s, Integer i, Integer n = 1)            // Returns "n" character(s) starting at index "i" of "s"
//     >> String    __TPP_STR_SUBSTR(String/Character s, Integer i, Integer n = 1)        // Returns a "n" long sub-string of "s" at "i"
//     >> String    __TPP_STR_PACK(Integer ...s)                                          // Pack a string from a list of integers
//     >> Integer   __TPP_STR_SIZE(String/Character s)                                    // Returns the size of a string
//     - Using these macros, one can implement more, higher-level macros for string operations,
//       like "TPP_STR_LOWER" (s.a. the new std-include file: <TPP_STR.h>)
//   - Added warnings for reused macro parameter names and using
//     __VA_ARGS__ or __VA_COMMA__ as name (the __VA_COMMA__ warning is disabled by default)
//   - Fixed a few bugs with comments at the end of files
//   - Rewrote the the #include API (old api is still available)
//   - Added support for #pragma warning(suppress)
//     - Disables a warning once (warning value will reset to its default afterwards)
//     - Allows for recursion, where calling the pragma twice will result in the warning being suppressed twice
//   - Added a warning, when trying to divide or modulo by 0 in a constant expression
//   - [Windows-only] Added a warning for miss-typed path casing in #include filenames
//     - Improves portability, since only windows compilers will accept any path casing
//   - Changed "#pragma warning", to follow the visual c pragma more closely
//   - Update the feature list and copyright year in "tpp --version"
//   - Fixed #line directive emission, with escaped line-feed tokens
//   - Fixed a bug, that caused a crash, when defining a macro for the first time,
//     that has previously been pushed and popped with #pragma push_macro:
//     >> // NOTE: foobar wasn't defined yet!
//     >> #pragma push_macro("foobar")
//     >> #pragma pop_macro("foobar")
//     >> #define foobar 20 // __TPP_VERSION__ <= 104 --> crash / assertion failure
//   - Fixed a bug, that caused __has_feature and similar function,
//     to return invalid values, if the identifier started after whitespace
//     >> __has_feature(tpp_pragma_once) // 1
//     >> __has_feature( tpp_pragma_once) // v104: 0; v105: 1
//   - Added support for #pragma deprecated("str")
//     - Marks a keyword as deprecated, to emit a warning, when being used.
//       >> #pragma deprecated("bar")
//       >> #pragma deprecated("foobar")
//       >> #define foo foobar
//       >> int foo;    // warning: deprecated
//       >> int bar;    // warning: deprecated
//       >> int foobar; // warning: deprecated
//   - Added support for the barely known "#@" operator (Charize)
//     - Similar to the Stringize operator "#", but instead of
//       surrounding with '\"' this operator will surround the string with '\''
//     >> #define CHARIZE2(x) #@ x
//     >> #define CHARIZE(x) CHARIZE2(x)
//     >> CHARIZE(abc) // expands to: [CHARIZE][(][abc][)] >> ... >> ['abc']
//     - Don't ask me, how I learned about that one. I don't even remember.
//       But it does exist and you can use it in at least Visual C/C++
//   - Add support for #import
//     - https://gcc.gnu.org/onlinedocs/gcc-4.3.5/cpp/Obsolete-once_002donly-headers.html#Obsolete-once_002donly-headers
//////////////////////////////////////////////////////////////////////////
// API v104 / TPP v104
//   ::BIG NEWS::
//      - Ability for macros to #undef / #define other macros or themselves
//      - Standardized interface for feature checking (__has_feature, ...)
//      - Macro calling conventions using <...>, [...] and {...}
//      - Bug and Crash fixes
//   ::DETAILED::
//   - Fixed #include guard for <TPP_ASSERT.h>
//   - Changed the default output FILE of #pragma message to stderr,
//     to prevent interference with actual preprocessor output, when outputting to stdout
//   - Added support for 0 and 1 arguments in <TPP_CAT.h>
//   - Update the feature table at the start of "tpp.h"
//   - Added missing supported for the '%' (module) operator in constant expressions
//   - Added additional macro calling conventions [ ... ], { ... }, < ... >
//     - s.a.: "TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS"
//   - Finally fixed the whole mess with invisible line-feeds and what not,
//     by cleaning up the frontend, instead of messing with tpp.c!
//     - This should fix problems with missing line feeds everywhere!
//   - Added a new switch the the frontend: "-token"
//     - Outputs tokens using the [bracket] notation, already used a lot in the documentation
//     NOTE: Only meant for debug and visualization
//   - Added support for "__TPP_STR_DECOMPILE"
//     - Inversion of the Stringify "#" operator
//     - An intrinsic macro, that can be used, to decompile a string:
//     >> __TPP_STR_DECOMPILE("a+b") // Expands to: [a][+][b]
//     - NOTE: You can't use this, to execute directives!
//     >> __TPP_STR_DECOMPILE("#define FOOBAR 42\n") // Will expand to [#][define][ ][FOOBAR][ ][42][LF]
//   - Fixed some bugs related to special tokens (SPACE/LF), when used around intrinsic macros
//   - Added support for the "++" and "--" tokens.
//   - Updated "TPPFile_NewFromVStringf" to no longer require the resulting
//     format string, to be shorter that 4096 characters
//   - Fixed a missing #line directive at the start of the first file
//   - You can now specify multiple initial #include files on the command line:
//     - TPP will start with the last specified file and output everything into a single file.
//     - todo: Should I clear all macros, when switching between those initial files?
//   - Added new frontend cmd switch: "-i":
//     - Use the remainder of the command line as a preprocessor input string.
//     - e.g.: tpp.exe -i "__TPP_EVAL((1 << 5)+10)"
//             >> outputs "42" in "stdout"
//             NOTE: Quotes are required in this example because of the "<<"
//   - Added some more support for msvc sal and gcc attributes to the API
//   - Added tests for boost.preprocessor (s.a.: "pp_test_19_boost.preprocessor.h")
//   - Fixed #warning and #error directives with open strings:
//     >> //                    v problem here
//     >> #warning This one didn't work before 104
//     >> //       v -- Real string ----------------------- v
//     >> #warning "But it could ' already be done like this"
//     NOTE: Doing this before 104 caused access violations in "_TPPFile_Format"
//   - Fixed a bug with defining macros, that end with escaped line feeds.
//   - Fixed quoting strings containing '\0' characters
//   - Fixed a bug, that would have caused a leak, if warning 3 was configured as critical
//   - Optimized some bigger if-blocks, to use switch
//   - Fixed a bug, that required "TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN" for "TPP_CONFIG_HAVE_DIGRAPHS"
//   - Added an integrated float parser
//   - Added support for "1.0E-8" - style numbers
//   - Added support for ansi Characters in identifiers (ord >= 192)
//     - s.a.: "TPP_CONFIG_SUPPORT_ANSI_CHARACTERS"
//   - Fixed a bug, that caused problems, when tokenizing special strings
//   - Fixed a bug with evaluating hexadecimal integers (a..f had the values 0..5 assigned; I forgot, to add +10)
//   - Fixed a bug with detecting floating pointer numbers:
//     >> TPP_TOK_INT:     "123"
//     >> TPP_TOK_FLOAT:   "123f"
//     >> TPP_TOK_INT:   "0x123f"
//   - Fixed a bug with the #include guard detection, that may
//     have caused tpp, to randomly abort without any error.
//   - Fixed a bug, where "#" in strings, in macro functions didn't get emitted.
//   - Fixed the error message, when missing an identifier after "defined" in a constant expression
//   - Added support for clang's "__is_identifier(ident)"
//     - Returns 1, if "ident" is a known identifier
//     - NOTE: Also added a tpp-only extension similar to "__is_identifier":
//       - "__is_builtin_identifier" (Only returns 1, if ident is a builtin keyword)
//////////////////////////////////////////////////////////////////////////
// API v103 / TPP v103
//   - Added support for #pragma push_macro / pop_macro
//     - NOTE: Also works in _Pragma("push_macro(\"...\")")
//   - Unknown pragmas get (re)-emitted
//     >> #pragma foobar    // expands to [#][pragma][ ][foobar]
//     >> #pragma once      // expands to <nothing>
//     >> _Pragma("foobar") // expands to [_Pragma][(]["foobar"][)]
//     >> _Pragma("once")   // expands to <nothing>
//   - Implemented stricter preprocessor directive location checking:
//     Directives are now only allowed, if they start at the begin of a line,
//     only preceded by possible whitespace (s.a.: pp_test_12.h / TPPToken_AtStartOfLine)
//   - Added support the #pragma region
//     Well... wasn't hard. I just have to ignore them!
//   - Added support the __VA_COMMA__ (A really interesting idea for working with __VA_ARGS__)
//     - If __VA_ARGS__ are empty, "__VA_COMMA__" will expand to nothing, otherwise it expands to ","
//     - __VA_COMMA__ can only be used, where __VA_ARGS__ would be allowed, but is still
//       allowed to be used as a parameter name, even in a __VA_ARGS__ function:
//       >> #define foo(...) __VA_COMMA__
//       >> foo()         // Expands to <nothing> (TPPLexer_Next() returns the first token of the next construct)
//       >> foo(42)       // Expands to [,]
//       >> foo(42,a,b,c) // Expands to [,]
//       >> #define bar(__VA_COMMA__,...) __VA_COMMA__
//       >> bar(10)    // Expands to [10]
//       >> bar(10,20) // Expands to [10]
//   - Added more tests
//   - Added standard library of utility macros for TPP
//   - Fixed constant expressions, so:
//     >> #undef FOOBAR
//     >> #if FOOBAR                    // Warning: Undefined keyword: FOOBAR
//     >> #if defined(FOOBAR) && FOOBAR // No warning
//   - Fixed expansion of line-feeds in macros not yielding a #line directive
//     >> #define clone(...) __VA_ARGS__ __VA_ARGS__
//     >> // Expands to [A][LF][#line @B][B][LF][#line @B][B][LF][C]
//     >> A
//     >> clone(B
//     >> )
//     >> C
//   - Fixed macro function calls erasing or multiplying line-feeds without
//     the preprocessor yielding the appropriate #line directives:
//     >> s.a.: "pp_test_14.h"
//   - Fixed some bugs associated with token names
//   - Fixed a bug, where whitespace wasn't truncated from the end of macro functions:
//     >> #define foo(a,b,c) 42 // my function
//     >>                   // ^ whitespace here
//     >> foo(10,20,30) // v102: Expands to: [42][ ]
//     >> foo(10,20,30) // v103: Expands to: [42]
//   - Fixed a bug, where whitespace wasn't trimmed from the end of a keyword-style macro declaration
//   - Re-#define-ing a macros with the same code doesn't yield a redefine warning
//////////////////////////////////////////////////////////////////////////
// API v102 / TPP v102
//   - Updated the preprocessor version to 102.
//   - Added support for some more c++ features in tpp.hpp
//   - Renamed some private functions, to follow the naming
//     convention for private functions
//   - Optimized stuff a bit more, by in-lining more functions
//   - Optimized stuff, by adding alternative function, that take string lengths,
//     thus eliminating the need, of temporarily cutting some strings and not calling strlen as often.
//   - Added more comments to the c api functions
//   - Added support for "__TIMESTAMP__"
//   - Added a new #pragma error(string), that is used just like
//     #pragma message(), and behaves the same as a #error directive
//   - Fixed a bug, that caused space tokens after macro functions to be omitted:
//     >> #define func(a)
//     >> func w // previously this (seemingly) expanded to "funcw"
//     >>        // (kind of; The space token inbetween didn't get emitted)
//     >>        // >> v101: [func][w]
//     >>        // >> v102: [func][ ][w]
//   - Fixed a bug related to rollback, when expanding code like:
//     >> #define func2   func
//     >> #define func(a) func2(a)
//     >> func(42) // Expands to: [func][(][42][)] >> [func2][(][42][)] >> [func][(][42][)]
//     NOTE: The above code would have crashed before.
//   - Renamed the config "TPP_CONFIG_HAVE_TPP_LOAD_FILE" to "TPP_CONFIG_HAVE___TPP_LOAD_FILE"
//   - Renamed the config "TPP_HAVE_PRAGMA_ONCE" to "TPP_CONFIG_HAVE_PRAGMA_ONCE"
//   - Added builtin support for <iso646.h>, which defines some macros
//     >> http://en.cppreference.com/w/cpp/language/operator_alternative
//     NOTE: by builtin I mean, that they are predefined macros, that can however be #undef'd,
//           similar to the __TPP_VERSION__ Macro (Yes, you can #undef that one).
//     s.a.: "TPP_CONFIG_HAVE_BUILTIN_ISO646_KEYWORDS"
//   - Added support for digraphs (TPP_CONFIG_HAVE_DIGRAPHS)
//   - Operands for the "##" may be keyword and numbers now
//   - Fixed multiple bugs, that caused whitespace to be eaten
//     while expanding macro functions.
//   - Fixed preprocessor directives being processed, when part of a macro
//     You might have called this one a bug-feature, but it really was just a bug...
//     Plus: #endif didn't work correctly!
//   - Added a list of future features below
//   - Added a folder called "examples", which contains some
//     random files with usage examples for TPP
//   - Fixed __TPP_EVAL not restoring lexer flags, when used at the top scope
//   - Fixed a bug, that caused TPP, to not correctly expand multiple self/upward references in filenames.
//     - This could have caused problems with #pragma once or not finding files
//   - Fixed incomplete strings/chars in #error / #warning directives in disabled code:
//     >> #if 0
//     >> #error Incomplete string " as an error
//     >> #endif
//////////////////////////////////////////////////////////////////////////
// API v101 / TPP v101
//   - Updated the preprocessor version to 101.
//   - Added a front-end preprocessor executable.
//     - This is the perfect demo of how fast the preprocessor is,
//       what it can do and how to use it.
//     - Just try it out on the command line.
//   - Did some cleanups with static_cast and const-modifiers.
//   - Did some tweaks to rand() initialization, that improve output of "__TPP_RANDOM".
//   - Fixed a bug, that caused whitespace to be added in some macro expansions.
//   - Fixed a bug, that caused incorrect __LINE__ values (and even possibly some access violations),
//     when using __LINE__ deep within the macro expansions tree.
//   - Fixed problems with ' and " without \ in strings: '"' and "'" are now handled properly
//   - Added support for <stdint.h>, when compiling for custom integer suffixes
//   - Added support for the "::" token (c++)
//   - Added #ifndef guards to all configs
// API v100 / TPP v100
//   - Initial Release
//   - Known bugs: \LF isn't implemented in some places (yet)
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// todo LIST:
//////////////////////////////////////////////////////////////////////////
// - The Stuff from fixme.h
// - Add limits to macro call and #include depth
//   - Just emit a warning every X macro calls, dumping the call stack
// - Add a config to enable linefeed string termination with warning.
//   >> (Currently, strings can cross multiple lines and are only terminated by ' / ")
// - Support for the gcc's ", ## __VA_ARGS__" operation:
//   if __VA_ARGS__ is empty, the "," will be erased
//   NOTE: For now, you can use __VA_COMMA__ for that one!
// - Add support for #pragma include_alias
//   - https://msdn.microsoft.com/en-us/library/wbeh5h91.aspx
// - Add support for the #assert stuff
//   - https://gcc.gnu.org/onlinedocs/gcc-4.3.5/cpp/Assertions.html
//   - This stuff is really weird... I've never actually seen this in action!
//     But when I started this preprocessor, I swore to eventually support
//     everything that ever got thought up as a preprocessor extensions
//     (And that I can find information on)!
//     And I guess this too falls under that category.
// - Add builtin support for "__pragma".
//   - Low priority, as you can already get it with this:
//     >> #define STRINGIZE_PRAGMA(x) #x
//     >> #define __pragma(x) _Pragma(STRINGIZE_PRAGMA(x))
//     NOTE: When it gets added, "__pragma" will be a predefined macro,
//           meaning that you can already get compatibility with the future,
//           by adding a "#ifndef __pragma" block around your "#define __pragma".
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// FUTURE FEATURE LIST:
//////////////////////////////////////////////////////////////////////////
// NOTE: This is meant for orientation, and by no means does it describe definite features.
//       Names and Ideas might change or not be implemented at all!
// NOTE: The order of items in this list has no meaning either!
//////////////////////////////////////////////////////////////////////////
//   - Add functionality, to put arguments in macro functions, without expanding them.
//     - That would allow for custom is_defined - style macros:
//       >> #define IS_ENABLED(x) __TPP_EVAL(defined(#!x) && (x-0) != 0)
//       >> IS_ENABLED(foobar) // [0]
//       >> #define foobar 0
//       >> IS_ENABLED(foobar) // [0]
//       >> #undef foobar
//       >> #define foobar
//       >> IS_ENABLED(foobar) // [0]
//       >> #undef foobar
//       >> #define foobar 1
//       >> IS_ENABLED(foobar) // [1]
//       - The "#!" operator prefixing the argument in the
//         function block prevents parameter expansion.
//       - Note the above example can already be implemented with other,
//         creative workarounds (s.a. "pp_awesome_sauce.h")
//////////////////////////////////////////////////////////////////////////
//   - Add more preprocessor functions for working with tokens:
//     - __TPP_TOK_EAT(Integer n) / __TPP_TOK_EATS(Integer n)
//       - Eats "n" tokens after the intrinsic
//       - "__TPP_TOK_EATS" also eats SPACE+LF tokens.
//       >> __TPP_TOK_EATS(2)foo,bar  // expands to [__TPP_TOK_EATS][(][2][)][foo][,][bar] --> [bar]
//       >> __TPP_TOK_EATS(2) foo,bar // expands to [__TPP_TOK_EATS][(][2][)][ ][foo][,][bar] --> [,][bar]
//       >> __TPP_TOK_EAT (2) foo,bar // expands to [__TPP_TOK_EAT][(][2][)][ ][foo][,][bar] --> [bar]
//       - Could be useful, when working with macro arguments
//////////////////////////////////////////////////////////////////////////
//   - Macro function default parameters:
//     >> #define foo(a,b=42) a+b
//     >> #define bar(a,b=a)  a+b
//     >> foo(10)    // expands to: [10][+][42]
//     >> foo(10,20) // expands to: [10][+][20]
//     >> bar(10)    // expands to: [10][+][10]
//     >> bar(10,20) // expands to: [10][+][20]
//////////////////////////////////////////////////////////////////////////
//   - Macro Namespaces ???
//     - I always wanted something like:
//     >> #define std::add(a,b) a+b
//     >> #define BASE std
//     >> #define PREFIX BASE::
//     >> #define ADDX add
//     >> std::add(10,20)     // expands to [10][+][20]
//     >> std :: add(10,20)   // expands to [10][+][20]
//     >> std :: ADDX(10,20)  // expands to [10][+][20]
//     >> BASE :: add(10,20)  // expands to [10][+][20]
//     >> BASE :: ADDX(10,20) // expands to [10][+][20]
//     >> PREFIX add(10,20)   // expands to [10][+][20]
//     >> PREFIX ADDX(10,20)  // expands to [10][+][20]
//     >> std: :add(10,20)    // expands to [std][:][ ][:][add][(][10][,][20][)]
//////////////////////////////////////////////////////////////////////////


TPP_DECLS_BEGIN_DISABLE_WARNINGS
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
# define TPP_FORWARD_DECL(x) struct x
#else /* __cplusplus */
# define TPP_FORWARD_DECL(x) typedef struct x x
#endif /* !__cplusplus */
// forward-declare typedefs
TPP_FORWARD_DECL(TPPFileObject);
TPP_FORWARD_DECL(TPPTokenObject);
TPP_FORWARD_DECL(TPPKeywordListHashEntryObject);
TPP_FORWARD_DECL(TPPKeywordListObject);
TPP_FORWARD_DECL(TPPMacroObject);
TPP_FORWARD_DECL(TPPMacroListObject);
TPP_FORWARD_DECL(TPPIncludeListObject);
TPP_FORWARD_DECL(TPPIfdefStackEntryObject);
TPP_FORWARD_DECL(TPPIfdefStackObject);
TPP_FORWARD_DECL(TPPIncludeCacheEntry);
TPP_FORWARD_DECL(TPPIncludeCacheObject);
TPP_FORWARD_DECL(TPPWarningsFrameObject);
TPP_FORWARD_DECL(TPPWarningsObject);
TPP_FORWARD_DECL(TPPLexerObject);
#undef TPP_FORWARD_DECL

struct TPPFileObject {
 unsigned int   f_refcnt;       /*< reference counter of this file */
#ifdef TPP_LIMITED_API
 TPPFileObject *f_prev;         /*< [0..1][ref] Intrusive: prev file */
 char          *f_name;         /*< [0..1][owned] Name of this file */
 char          *f_new_name;     /*< [0..1][owned] Overwritten name of this file */
 TPPFileObject *f_ref_file;     /*< [0..1][ref] Macro reference file (used, to store the location of the macro expansion starting point) */
 size_t         f_ref_file_off; /*< Offset in "f_ref_file" (Only valid, if f_ref_file != NULL) */
 int            f_line_offset;  /*< Offset added to __LINE__, when in this file (needed for #line directive) */
 char const    *f_iter;         /*< Current position in this file */
 char const    *f_end;          /*< End of the file (first invalid pointer) */
 char           f_data[1024];   /*< Inlined data of this file */
#endif
};

#define TPPFile_INCREF(self)        (void)++(self)->f_refcnt
#define TPPFile_DECREF(self)        (--(self)->f_refcnt == 0 ? _TPPFile_Free(self) : (void)0)
#define TPPFile_XINCREF(self)       ((self) ? TPPFile_INCREF(self) : (void)0)
#define TPPFile_XDECREF(self)       ((self) ? TPPFile_DECREF(self) : (void)0)
#ifdef TPP_LIMITED_API
#define TPPFile_REWIND(self)        (void)((self)->f_iter = (self)->f_data)
#define TPPFile_IS_MACRO_FILE(self) ((self)->f_ref_file != NULL)
#define TPPFile_DATA(self)          (self)->f_data
#ifdef __cplusplus
#define TPPFile_SIZE(self)          static_cast<size_t>((self)->f_end-(self)->f_data)
#define TPPFile_ITER_OFF(self)      static_cast<size_t>((self)->f_iter-(self)->f_data)
#else /* __cplusplus */
#define TPPFile_SIZE(self)          (size_t)((self)->f_end-(self)->f_data)
#define TPPFile_ITER_OFF(self)      (size_t)((self)->f_iter-(self)->f_data)
#endif /* !__cplusplus */
#else
#define TPPFile_REWIND        TPPFile_Rewind
#define TPPFile_IS_MACRO_FILE TPPFile_IsMacroFile
#define TPPFile_SIZE          TPPFile_Size
#define TPPFile_ITER_OFF      TPPFile_IterOff
#endif

TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_IsMacroFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(void) TPPFile_Rewind(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN size_t) TPPFile_Size(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN size_t) TPPFile_IterOff(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));


// v Never call directly (use incref/decref)
TPP_DECL_FUNC(void) _TPPFile_Free(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns a New file with a capacity for "n" characters
// Returns: The file or NULL, if the allocation failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_New(size_t n);
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_Copy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self) TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Resizes "self", to fit "n" characters.
// Precondition: "self->f_refcnt == 1"
// Returns: The file or NULL, if the reallocation failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_Resize(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self, size_t n)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

TPP_DECL_ENUM_WITH_BASE(TPPFileFormatFlag,int) {
 TPPFileFormatFlag_NONE          = 0x00000000,
 TPPFileFormatFlag_ENABLED       = 0x00000001,
 TPPFileFormatFlag_FMT_LINE_WRAP = 0x00000002|TPPFileFormatFlag_ENABLED,
 TPPFileFormatFlag_KEEP_COMMENTS = 0x00000004|TPPFileFormatFlag_ENABLED
};

//////////////////////////////////////////////////////////////////////////
// Creates a FileObject from a string
// Returns: The file or NULL, if the allocation failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringEx(
 TPPFileFormatFlag flags, char const *s, size_t n);
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringExf(
 TPPFileFormatFlag flags, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, ...)
 TPP_ATTRIBUTE_GCC_PRINTF_STRING(2,3);
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromVStringExf(
 TPPFileFormatFlag flags, TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, va_list args);
#define TPPFile_NewFromString(s,n) TPPFile_NewFromStringEx(TPPFileFormatFlag_ENABLED,s,n)
#define TPPFile_NewFromStringf(...) TPPFile_NewFromStringExf(TPPFileFormatFlag_NONE,__VA_ARGS__)
#define TPPFile_NewFromVStringf(fmt,args) TPPFile_NewFromVStringExf(TPPFileFormatFlag_NONE,fmt,args)


//////////////////////////////////////////////////////////////////////////
// Creates a FileObject from a filename.
// Returns: The file or NULL, if the allocation failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromFileEx(
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPFileFormatFlag flags);
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromFILEEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL FILE *fp,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPFileFormatFlag flags)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// If "format_file" is true, comments will be removed from the file
#define TPPFile_NewFromFile(filename,format_file) \
 TPPFile_NewFromFileEx(filename,(format_file)?TPPFileFormatFlag_ENABLED:TPPFileFormatFlag_NONE)
#define TPPFile_NewFromFILE(fp,filename,format_file) \
 TPPFile_NewFromFILEEx(fp,filename,(format_file)?TPPFileFormatFlag_ENABLED:TPPFileFormatFlag_NONE)

//////////////////////////////////////////////////////////////////////////
// Returns a new FileObject with the quoted version of "s"
// Returns: The file or NULL, if the allocation failed
// >> TPPFile_NewFromQuotedString("foo\nbar") --> "\"foo\\nbar\""
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromQuotedStringL(char const *s, size_t s_len);
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *)
TPPFile_NewFromQuotedString(TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s) {
 return TPPFile_NewFromQuotedStringL(s,strlen(s));
}

//////////////////////////////////////////////////////////////////////////
// Returns a new FileObject with the decoded escape codes of "token_"
// Returns: The file or NULL, if the allocation failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPFile_NewFromStringToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_AppendStringToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject **self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Sets the name of a file.
// Returns -1 on error
// NOTE: Always use "TPPFile_SetNewName", as f_name of a file
//       is used as the cwd, when processing a #include directory
//    >> There must be 2 names, to correctly handle #line and #include directives
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API int) _TPPFile_SetName(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPFile_SetNewName(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the name of "self"
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN char const *) TPPFile_Name(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#ifdef TPP_LIMITED_API
#define TPPFile_NAME(self) ((self)->f_new_name ? (self)->f_new_name : (self)->f_name)
#else
#define TPPFile_NAME       TPPFile_Name
#endif

//////////////////////////////////////////////////////////////////////////
// Reads and returns character from "self"
// >> Skips '\0' chars
// >> Return '\0' for EOF
TPP_DECL_FUNC(char) TPPFile_Read(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char) TPPFile_Peek(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(void) TPPFile_Undo(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPFile_LineAt(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *self, size_t offset)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));


enum{
 TPP_TOK_EOF       = 0,
 TPP_TOK_MISC      = 1,
 TPP_TOK_SPACE     = ' ',
 TPP_TOK_LF        = '\n',
 TPP_TOK_INT       = 'i',
 TPP_TOK_FLOAT     = 'f',
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
 TPP_TOK_COMMENT   = 'c',
#endif /* TPP_CONFIG_HAVE_COMMENT_TOKEN */
 TPP_TOK_CHR       = '\'',
 TPP_TOK_STR       = '\"',
 TPP_TOK_ASSIGN    = '=',
 TPP_TOK_CMP_LO    = '<',
 TPP_TOK_CMP_GR    = '>',
 TPP_TOK_QUESTION  = '?',
 TPP_TOK_COLLON    = ':',
 TPP_TOK_ADD       = '+',
 TPP_TOK_SUB       = '-',
 TPP_TOK_MUL       = '*',
 TPP_TOK_DIV       = '/',
 TPP_TOK_MOD       = '%',
 TPP_TOK_DOT       = '.',
 TPP_TOK_COMMA     = ',',
 TPP_TOK_SEMICOLON = ';',
 TPP_TOK_HASH      = '#',
 TPP_TOK_LPAREN    = '(',
 TPP_TOK_RPAREN    = ')',
 TPP_TOK_LBRACKET  = '[',
 TPP_TOK_RBRACKET  = ']',
 TPP_TOK_LBRACE    = '{',
 TPP_TOK_RBRACE    = '}',
 TPP_TOK_TILDE     = '~',
 TPP_TOK_AT        = '@',
 TPP_TOK_BIN_AND   = '&',
 TPP_TOK_BIN_OR    = '|',
 TPP_TOK_BIN_XOR   = '^',
 TPP_TOK_EXCLAIM   = '!',
 //////////////////////////////////////////////////////////////////////////
 // Multi-char tokens
 TPP_TOK_CMP_LE = 256,   // <=
 TPP_TOK_CMP_EQ,         // ==
 TPP_TOK_CMP_NE,         // !=
 TPP_TOK_CMP_GE,         // >=
 TPP_TOK_ADD_ASSIGN,     // +=
 TPP_TOK_SUB_ASSIGN,     // -=
 TPP_TOK_MUL_ASSIGN,     // *=
 TPP_TOK_DIV_ASSIGN,     // /=
 TPP_TOK_MOD_ASSIGN,     // %=
 TPP_TOK_SHL,            // <<
 TPP_TOK_SHR,            // >>
 TPP_TOK_SHL_ASSIGN,     // <<=
 TPP_TOK_SHR_ASSIGN,     // >>=
 TPP_TOK_DOTS,           // ...
 TPP_TOK_GLUE,           // ##
 TPP_TOK_BIN_AND_ASSIGN, // &=
 TPP_TOK_BIN_OR_ASSIGN,  // |=
 TPP_TOK_BIN_XOR_ASSIGN, // ^=
 TPP_TOK_LAND,           // &&
 TPP_TOK_LOR,            // ||
 TPP_TOK_INC,            // ++
 TPP_TOK_DEC,            // --
#if TPP_CONFIG_HAVE_POW_TOKEN
 TPP_TOK_POW,            // **
 TPP_TOK_POW_ASSIGN,     // **=
#endif /* TPP_CONFIG_HAVE_POW_TOKEN */
#if TPP_CONFIG_HAVE_LXOR_TOKEN
 TPP_TOK_LXOR,           // ^^
#endif /* TPP_CONFIG_HAVE_LXOR_TOKEN */
#if TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN
 TPP_TOK_TILDE_TILDE,    // ~~
#endif /* TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN */
#if TPP_CONFIG_HAVE_ARROW_TOKEN
 TPP_TOK_ARROW,          // ->
#endif /* TPP_CONFIG_HAVE_ARROW_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN
 TPP_TOK_COLLON_ASSIGN,  // :=
#endif /* TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN */
#if TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN
 TPP_TOK_COLLON_COLLON,  // ::
#endif /* TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN */
 TPP_TOK_IDENT_START
};

enum{
 _TPP_KWD_FIRST=TPP_TOK_IDENT_START-1,
 // WARNING: Keyword ids are automatically assigned, meaning
 //          that no guaranty is given that they will remain
 //          constant from release to release!
 //          This also applies to any library extending onto tpp. (e.g.: deemon)
#define DEF(name,value) name,
#include "tpp_keywords.inl"
#undef DEF
 TPP_TOK_USER_IDENT_START // First id for user keywords
};

#ifdef TPPInteger
#endif

#if defined(_MSC_VER)
// intmax_t?
typedef __int64 TPPInteger;
typedef unsigned __int64 TPPUInteger;
#define TPPInteger_PRINTF_FMT "%I64d"
#define TPPInteger_C(x) x##ui64
#elif 0
typedef int64_t TPPInteger;
typedef uint64_t TPPUInteger;
#define TPPInteger_PRINTF_FMT "%lld"
#define TPPInteger_C(x) x##ull
#elif 1
typedef long long TPPInteger;
typedef unsigned long long TPPUInteger;
#define TPPInteger_PRINTF_FMT "%lld"
#define TPPInteger_C(x) x##ull
#else
typedef long TPPInteger;
typedef unsigned long TPPUInteger;
#define TPPInteger_PRINTF_FMT "%ld"
#define TPPInteger_C(x) x##ul
#endif

#if 1
typedef long double TPPFloat;
#define TPPFloat_PRINTF_FMT "%Lf"
#else
typedef double TPPFloat;
#define TPPFloat_PRINTF_FMT "%f"
#endif


#ifndef TPP_PRIVATE_RUNTIME_INTEGER_TYPES_DEFINED
#define TPP_PRIVATE_RUNTIME_INTEGER_TYPES_DEFINED 1
// The types of user integers
// - Override before inclusion
// - Default to host compiler types
typedef int8_t        TPP_rt_int8_t;
typedef int16_t       TPP_rt_int16_t;
typedef int32_t       TPP_rt_int32_t;
typedef int64_t       TPP_rt_int64_t;
typedef uint8_t       TPP_rt_uint8_t;
typedef uint16_t      TPP_rt_uint16_t;
typedef uint32_t      TPP_rt_uint32_t;
typedef uint64_t      TPP_rt_uint64_t;
typedef int           TPP_rt_int;
typedef unsigned int  TPP_rt_uint;
typedef long          TPP_rt_long;
typedef unsigned long TPP_rt_ulong;
#if (defined(_MSC_VER) && (defined(_MSC_EXTENSIONS) || _MSC_VER >= 1400)) || \
    (defined(__clang__)) || \
    (defined(__GNUC__) && !defined(__DARWIN_NO_LONG_LONG)) || \
    (defined(__BORLANDC__) && __BORLANDC__ >= 0x561 && !defined(__NO_LONG_LONG))
typedef long long          TPP_rt_llong;
typedef unsigned long long TPP_rt_ullong;
#else
typedef int64_t   TPP_rt_llong;
typedef uint64_t  TPP_rt_ullong;
#endif
typedef char           TPP_rt_char;   // data format for "foobar"
#if defined(_WCHAR_T_DEFINED)
typedef wchar_t        TPP_rt_wchar_t;  // data format for L"foobar"
#elif defined(__CHAR_UNSIGNED__) || defined(__WCHAR_UNSIGNED__)
typedef unsigned short TPP_rt_wchar_t;  // data format for L"foobar"
#else
typedef short          TPP_rt_wchar_t;  // data format for L"foobar"
#endif
#if defined(__CHAR_UNSIGNED__) || \
    defined(__CHAR16_UNSIGNED__) || \
    defined(__CHAR32_UNSIGNED__)
typedef unsigned short TPP_rt_char16_t; // data format for u"foobar"
typedef unsigned int   TPP_rt_char32_t; // data format for U"foobar"
#else
typedef short          TPP_rt_char16_t; // data format for u"foobar"
typedef int            TPP_rt_char32_t; // data format for U"foobar"
#endif
#endif

// Deprecated names
typedef TPP_rt_char TPPChar;
typedef TPP_rt_wchar_t TPPWchar;
typedef TPP_rt_char16_t TPPChar16;
typedef TPP_rt_char32_t TPPChar32;


typedef unsigned int TPPTokenID; /* TPP_TOK_* / TPPTokenID_IS_KEYWORD(...) */
#define TPPTokenID_IS_KEYWORD(id)      ((id) >= TPP_TOK_IDENT_START)
#define TPPTokenID_IS_USER_KEYWORD(id) ((id) >= TPP_TOK_USER_IDENT_START)
#define TPPTokenID_IS_INT(id)          ((id) == TPP_TOK_INT || (id) == TPP_TOK_CHR)

struct TPPTokenObject {
 TPPTokenID     tk_id;        /*< Token ID */
 TPPFileObject *tk_file;      /*< [0..1][ref] Final position of this token after expansion */
 size_t         tk_file_off;  /*< Offset in "tk_file" */
 TPPFileObject *tk_str_file;  /*< [0..1][ref] Reference to the file, where this token was written in */
 char const    *tk_str_begin; /*< [0..1 == tk_str_file != NULL] Pointer to the unformatted begin of this token */
 char const    *tk_str_end;   /*< [0..1 == tk_str_file != NULL] Pointer to the unformatted end of this token */
};
#define TPPToken_INIT() {TPP_TOK_EOF,NULL,0,NULL,NULL,NULL}
#define TPPToken_Init(self) (\
 (self)->tk_id = TPP_TOK_EOF,\
 (self)->tk_file = NULL,\
 (self)->tk_file_off = 0,\
 (self)->tk_str_file = NULL,\
 (self)->tk_str_begin = NULL,\
 (self)->tk_str_end = NULL)
TPP_DECL_FUNC(void) TPPToken_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(void) TPPToken_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPToken_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns true, if "self" is located at the start of a line (not counting whitespace)
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPToken_AtStartOfLine(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Fills "s" with the string representation of "self".
// Writes at most "n" bytes to "s" and always returns the amount of required bytes
TPP_DECL_FUNC(size_t) TPPToken_Str(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_BUFFER_VAR(n,return) char *s, size_t n)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns a pointer to a static buffer, that describes the str repr of the token.
// Useful for printf() arguments
// NOTE: Not thread-safe!
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *)
TPPToken_SStr(TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the filename of "self"
// (Used for "__FILE__")
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *)
TPPToken_File(TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self) {
 TPP_ASSERT(self);
 return self->tk_file ? TPPFile_NAME(self->tk_file) : NULL;
}

//////////////////////////////////////////////////////////////////////////
// Returns the line position of "self"
// (Used for "__LINE__")
// NOTE: Returns the line number as 0-based,
//       meaning that tokens in the 1st line will have a line number of "0"
//       >> To get the line number used in text editors, just add +1
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_SHOULD_USE_RETURN int)
TPPToken_Line(TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self) {
 TPP_ASSERT(self);
 //TPP_ASSERT(!self->tk_file || !TPPFile_IS_MACRO_FILE(self->tk_file));
 return self->tk_file ? TPPFile_LineAt(self->tk_file,self->tk_file_off) : -1;
}

//////////////////////////////////////////////////////////////////////////
// Returns the line at the end of "self".
// Identical to "TPPToken_Line", but 1 is added, if "self" is a LF token
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_SHOULD_USE_RETURN int)
TPPToken_LineAfter(TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self) {
 char const *iter,*end; int result;
 // We now add the sum of '\n' characters in the token's
 // STR, instead of just adding 1, if its a line-feed token.
 result = TPPToken_Line(self);
 iter = self->tk_str_begin,end = self->tk_str_end;
 while (iter != end) {
  if (*iter == '\n') ++result;
  else if (*iter == '\r') {
   if (iter[1] == '\n') ++iter;
    ++result;
  }
  ++iter;
 }
 return result;
}

TPP_DECL_ENUM_WITH_BASE(TPPIntegerType,int) {
 TPPIntegerType_CHAR               = 0x0100, // 'a'
 TPPIntegerType_WCHAR              = 0x0200, // L'a'
 TPPIntegerType_CHAR16             = 0x0300, // u'a'
 TPPIntegerType_CHAR32             = 0x0400, // U'a'
 TPPIntegerType_INT                = 0x0000, // 1234
 TPPIntegerType_UNSIGNED           = 0x0001, // 1234u
 TPPIntegerType_LONG               = 0x0002, // 1234l
 TPPIntegerType_LONG_LONG          = 0x0004, // 1234ll
 TPPIntegerType_UNSIGNED_LONG      = TPPIntegerType_LONG|TPPIntegerType_UNSIGNED, // 1234ul
 TPPIntegerType_UNSIGNED_LONG_LONG = TPPIntegerType_LONG_LONG|TPPIntegerType_UNSIGNED, // 1234ull
#if TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX
 TPPIntegerType_INT8               = 0x0010, // 1234i8
 TPPIntegerType_INT16              = 0x0020, // 1234i16
 TPPIntegerType_INT32              = 0x0040, // 1234i32
 TPPIntegerType_INT64              = 0x0080, // 1234i64
 TPPIntegerType_UINT8              = TPPIntegerType_INT8 |TPPIntegerType_UNSIGNED, // 1234i8
 TPPIntegerType_UINT16             = TPPIntegerType_INT16|TPPIntegerType_UNSIGNED, // 1234i16
 TPPIntegerType_UINT32             = TPPIntegerType_INT32|TPPIntegerType_UNSIGNED, // 1234i32
 TPPIntegerType_UINT64             = TPPIntegerType_INT64|TPPIntegerType_UNSIGNED, // 1234i64
#endif /* TPP_CONFIG_HAVE_MSVC_INTEGER_SUFFIX */
 TPPIntegerType_TYPE_MASK          = 0xFFFF & ~TPPIntegerType_UNSIGNED
};

TPP_DECL_ENUM_WITH_BASE(TPPFloatType,int) {
 TPPFloatType_DOUBLE,
 TPPFloatType_FLOAT,
 TPPFloatType_LDOUBLE
};

//////////////////////////////////////////////////////////////////////////
// Evaluates "self" as an integer and saves the result in "i"
// >> "self" must be a TPP_TOK_INT token
// >> if type != NULL, it will be filled with the suffix type of "i"
// Returns: 1:  Overflow while evaluating
// Returns: 0:  no error
// Returns: -1: if "self" is an invalid integer
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPToken_EvalInt(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPUInteger *i, TPPIntegerType *type)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPToken_EvalFloat(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFloat *f, TPPFloatType *type)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Fills "s" with the char / string representation of "self" with decoded escape codes.
// Writes at most "n" bytes to "s" and always returns the amount of required bytes
// NOTE: Bytes written per char depend on the encoding of the string
TPP_DECL_FUNC(size_t) TPPToken_EvalStr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self,
 TPP_ATTRIBUTE_MSVC_BUFFER_VAR(n,return) char *s, size_t n)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

TPP_DECL_ENUM_WITH_BASE(TPPStringType,int) {
 TPPStringType_CHAR   = '\"',
 TPPStringType_WCHAR  = 'L',
 TPPStringType_CHAR16 = 'u',
 TPPStringType_CHAR32 = 'U',
};
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPStringType)
TPPToken_GetStringType(TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *self) {
 TPPStringType result;
 TPP_ASSERT(self);
#ifdef __cplusplus
 result = static_cast<TPPStringType>(*self->tk_str_begin);
#else
 result = (TPPStringType)*self->tk_str_begin;
#endif
 if (result == '\'') result = TPPStringType_CHAR;
 return result;
}



typedef unsigned int TPPCounter;
typedef unsigned int TPPKeywordHash;
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
TPP_DECL_ENUM_WITH_BASE(TPPKeywordFlag,unsigned char) {
 TPPKeywordFlag_NONE = 0x00,
#if TPP_CONFIG_HAVE___has_attribute
 TPPKeywordFlag_HAS_ATTRIBUTE = 0x01,
#endif /* TPP_CONFIG_HAVE___has_attribute */
#if TPP_CONFIG_HAVE___has_builtin
 TPPKeywordFlag_HAS_BUILTIN = 0x02,
#endif /* TPP_CONFIG_HAVE___has_builtin */
#if TPP_CONFIG_HAVE___has_cpp_attribute
 TPPKeywordFlag_HAS_CPP_ATTRIBUTE = 0x04,
#endif /* TPP_CONFIG_HAVE___has_cpp_attribute */
#if TPP_CONFIG_HAVE___has_declspec_attribute
 TPPKeywordFlag_HAS_DECLSPEC_ATTRIBUTE = 0x08,
#endif /* TPP_CONFIG_HAVE___has_declspec_attribute */
#if TPP_CONFIG_HAVE___has_extension
 TPPKeywordFlag_HAS_EXTENSION = 0x10,
#endif /* TPP_CONFIG_HAVE___has_extension */
#if TPP_CONFIG_HAVE___has_feature
 TPPKeywordFlag_HAS_FEATURE = 0x20,
#endif /* TPP_CONFIG_HAVE___has_feature */
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
 TPPKeywordFlag_IS_DEPRECATED = 0x40,
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
};
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */

#ifdef TPP_LIMITED_API
struct TPPKeywordListHashEntryObject {
 TPPKeywordListHashEntryObject *he_next;      /*< [0..1][owned] Next entry with the same hash. */
 TPPTokenID                     he_id;        /*< Token ID of this keyword entry. */
#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
 TPPKeywordFlag                 he_flags;     /*< Keyword-specific flags. */
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */
#if TPP_CONFIG_HAVE___TPP_COUNTER
 TPPCounter                     he_counter;   /*< Keyword-specific counter for __TPP_COUNTER. */
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
 TPPKeywordHash                 he_raw_hash;  /*< Raw hash (without modulo). */
 size_t                         he_str_len;   /*< Length of the keyword (== strlen(he_str)). */
 char                           he_str[1024]; /*< '\0'-terminated, inline string repr of the keyword. */
};
#if TPP_LOAD_STATIC
static TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_Copy(TPPKeywordListHashEntryObject const *self);
static TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_NewS(TPPTokenID id, TPPKeywordHash raw_hash, char const *str, size_t str_len);
#else
extern TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_Copy(TPPKeywordListHashEntryObject const *self);
extern TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *TPPKeywordListHashEntry_NewS(TPPTokenID id, TPPKeywordHash raw_hash, char const *str, size_t str_len);
#endif
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordListHashEntry_New(
 TPPTokenID id, TPPKeywordHash raw_hash, TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *str) {
 return TPPKeywordListHashEntry_NewS(id,raw_hash,str,strlen(str));
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordListHashEntry_NewL(
 TPPTokenID id, TPPKeywordHash raw_hash, char const *str_begin, char const *str_end) {
 return TPPKeywordListHashEntry_NewS(id,raw_hash,str_begin,(size_t)(str_end-str_begin));
}
#define TPPKeywordListHashEntry_Free(self) TPP_CALLBACK_FREE_NN(self)
#endif

#ifdef TPP_LIMITED_API
#define TPPKeywordList_BUFSIZE 64
struct TPPKeywordListObject {
 TPPTokenID                      kl_next;       /*< Next available token ID. */
 size_t                          kl_hash_alloc; /*< Amount of allocated hash entries. */
 TPPKeywordListHashEntryObject **kl_hash;       /*< [0..1][owned][0..kl_hash_alloc][owned] Hash-map of keyword names. */
};
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the amount of user keywords managed by "self"
#ifdef TPP_LIMITED_API
#define TPPKeywordList_SIZE(self) ((self)->kl_next-TPP_TOK_USER_IDENT_START)
#else
#define TPPKeywordList_SIZE       TPPKeywordList_Size
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN unsigned int) TPPKeywordList_Size(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject const *self);

#ifdef TPP_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// static-var initializer for "TPPKeywordListObject"
#define TPPKeywordList_INIT() {TPP_TOK_USER_IDENT_START,0,NULL}

//////////////////////////////////////////////////////////////////////////
// same as initialization with "TPPKeywordList_INIT()"
#define TPPKeywordList_Init(self) (\
 (self)->kl_next = TPP_TOK_USER_IDENT_START,\
 (self)->kl_hash_alloc = 0,(self)->kl_hash = NULL)
#if TPP_LOAD_STATIC
static int TPPKeywordList_InitCopy(TPPKeywordListObject *self, TPPKeywordListObject const *right);
static void TPPKeywordList_InitMove(TPPKeywordListObject *self, TPPKeywordListObject *right);
#else
extern int TPPKeywordList_InitCopy(TPPKeywordListObject *self, TPPKeywordListObject const *right);
extern void TPPKeywordList_InitMove(TPPKeywordListObject *self, TPPKeywordListObject *right);
#endif
#endif

#ifdef TPP_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Finalizes a KeywordList Object.
#if TPP_LOAD_STATIC
static void TPPKeywordList_Quit(TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self);
#else
extern void TPPKeywordList_Quit(TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self);
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// Sets up the builtin keywords.
// NOTE: Must be called manually after initialization:
// >> TPPKeywordListObject kwd_list = TPPKeywordList_INIT();
// >> if (TPPKeywordList_SetupBuiltin(&kwd_list) == -1) { /* error-handling */ }
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_SetupBuiltin(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_Set(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_SetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_SetL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name_begin, char const *name_end, TPPTokenID id) {
 return TPPKeywordList_SetS(self,name_begin,(size_t)(name_end-name_begin),id);
}

#if TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_DefTraitS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, TPPKeywordFlag flags)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_DefTrait(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, 
 char const *name, TPPKeywordFlag flags) {
 return TPPKeywordList_DefTraitS(self,name,strlen(name),flags);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPKeywordList_DefTraitL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name_begin, char const *name_end, TPPKeywordFlag flags) {
 return TPPKeywordList_DefTraitS(self,name_begin,(size_t)(name_end-name_begin),flags);
}
#endif /* TPP_CONFIG_HAVE_KEYWORD_SPECIFIC_TRAITS */

//////////////////////////////////////////////////////////////////////////
// Returns the name of a valid keyword ID.
// Returns NULL, if the id could not be found or isn't a keyword ID
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPKeywordList_NameOf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject const *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Return a unique ID for a given keyword.
// Returns 0, on failure:
//   if insert_missing == 0:
//     - Returns 0, if the keyword doesn't exist
//   if insert_missing == 1:
//     - Returns 0, if the map reallocation failed
// NOTE: if "insert_missing == 0", "self" is never modified
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPTokenID) TPPKeywordList_GetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name, size_t name_len, int insert_missing)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPTokenID) TPPKeywordList_Get(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, int insert_missing) {
 return TPPKeywordList_GetS(self,name,strlen(name),insert_missing);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPTokenID) TPPKeywordList_GetL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self,
 char const *name_begin, char const *name_end, int insert_missing) {
 return TPPKeywordList_GetS(self,name_begin,(size_t)(name_end-name_begin),insert_missing);
}
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_GetEntryS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, char const *name, size_t name_len, int insert_missing)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_GetEntry(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, int insert_missing) {
 return TPPKeywordList_GetEntryS(self,name,strlen(name),insert_missing);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_GetEntryL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, char const *name_begin, char const *name_end, int insert_missing) {
 return TPPKeywordList_GetEntryS(self,name_begin,(size_t)(name_end-name_begin),insert_missing);
}

//////////////////////////////////////////////////////////////////////////
// Returns the internal entry for a given keyword
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPKeywordListHashEntryObject *) TPPKeywordList_FindEntry(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPKeywordListObject *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));


#ifdef TPP_LIMITED_API
struct TPPMacroArgument {
 TPPTokenID a_name;             /*< Name of this argument. */
 size_t     a_references;       /*< Amount of references to this argument. */
 size_t     a_quote_references; /*< Amount stringize references to this argument. */
};
#endif

#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
TPP_DECL_ENUM_WITH_BASE(TPPMacroCallingConvention,unsigned char) {
 // WARNING: The order in this enum encodes parenthesis order!
 TPPMacroCallingConvention_PAREN   = 0,
 TPPMacroCallingConvention_BRACKET = 1,
 TPPMacroCallingConvention_BRACE   = 2,
 TPPMacroCallingConvention_ANGLE   = 3
};
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */

#ifdef TPP_LIMITED_API
struct TPPMacroObject {
#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
 TPPMacroObject         *m_push_prev; /*< [0..1][owned] Previous iteration of this macro. */
 TPPFileObject          *m_code;      /*< [0..1][ref] Macro code preformatted in a FileObject.
                                          NOTE: NULL, if this macro was #undef'd, but still has a backup. */
#else /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 TPPFileObject          *m_code;      /*< [1..1][ref] Macro code preformatted in a FileObject.
                                          NOTE: May be NULL in the destructor for macros. */
#endif /* !TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
 // v (NOTE: Only calculated for function-style macros)
 //   >> base size of this macro's code without arguments
 size_t                  m_code_noarg_size;
#if TPP_CONFIG_HAVE___VA_ARGS__
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
 TPPTokenID              m_va_args_name; /*< != 0, if this is a variadic macro. */
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
 unsigned char           m_is_va_args;   /*< != 0, if this is a variadic macro. */
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS
 TPPMacroCallingConvention m_call_conv;
#endif /* TPP_CONFIG_HAVE_MACRO_CALLING_CONVENTIONS */
 size_t                  m_argc;
 struct TPPMacroArgument m_argv[1024]; /*< [m_argc] List of macro argument names. */
};
extern struct TPPMacroObject *TPPMacro_Copy(struct TPPMacroObject const *self);
#endif

#if TPP_CONFIG_HAVE___VA_ARGS__
#ifdef TPP_LIMITED_API
#if TPP_CONFIG_HAVE_NAMED_VA_ARGS
#define TPPMacro_IS_VA_ARGS_FUNCTION(self) ((self)->m_va_args_name != 0)
#else /* TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#define TPPMacro_IS_VA_ARGS_FUNCTION(self) ((self)->m_is_va_args != 0)
#endif /* !TPP_CONFIG_HAVE_NAMED_VA_ARGS */
#else
#define TPPMacro_IS_VA_ARGS_FUNCTION        TPPMacro_IsVaArgsFunction
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_IsVaArgsFunction(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */

#ifdef TPP_LIMITED_API
#define TPPMacro_KEYWORD_IS_EXPANDING(self) (\
 TPP_ASSERT((self)->m_code),(self)->m_code->f_prev != NULL)
#else
#define TPPMacro_KEYWORD_IS_EXPANDING TPPMacro_KeywordIsExpanding
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_KeywordIsExpanding(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns true, if "self" is a function-style macro
#ifdef TPP_LIMITED_API
#define TPPMacro_HAS_ARGS(self) ((self)->m_argc != (size_t)-1)
#else
#define TPPMacro_HAS_ARGS       TPPMacro_HasArgs
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_HasArgs(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns a new Macro object.
#define TPPMacro_New() TPPMacro_NewFunction((size_t)-1)
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPMacroObject *) TPPMacro_NewFunction(size_t n_args);

#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
//////////////////////////////////////////////////////////////////////////
// Push / Pop the state of the macro "self"
// >> #pragma push_macro(name)
// >> #pragma pop_macro(name)
// Returns 0 on success
// TPPMacro_Push Returns -1 on error
// TPPMacro_Pop Returns 1, if the macro wasn't previously pushed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject **self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_Defined(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#ifdef TPP_LIMITED_API
#define TPPMacro_DEFINED(self) ((self)->m_code != NULL)
#else
#define TPPMacro_DEFINED       TPPMacro_Defined
#endif
#else /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */
#define TPPMacro_DEFINED(self) 1
#endif /* !TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */



//////////////////////////////////////////////////////////////////////////
// Free a Macro Object
TPP_DECL_FUNC(void) TPPMacro_Free(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Called, to finalize a macro function declaration
// Calculate argument references and "m_code_noarg_size"
// NOTE: "self" must a function-style macro "TPPMacro_HAS_ARGS(self) == true"
TPP_DECL_FUNC(TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API TPP_ATTRIBUTE_USE_RETURN int) TPPMacro_CalculateArgumentRefs(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *self, TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *lexer) TPP_ATTRIBUTE_GCC_NON_NULL((1,2));


#ifdef TPP_LIMITED_API
struct TPPMacroListObject {
 TPPTokenID       ml_size;   /*< Amount of existing macros. */
 TPPMacroObject **ml_macros; /*< [0..1][owned][0..m_size][owned]
                                 Vector of macro names (Index is "token_id-TPP_TOK_IDENT_START"). */
};
#define TPPMacroList_INIT() {0,NULL}
#endif

#ifdef TPP_LIMITED_API
#define _TPPMacroList_Init(self) ((self)->ml_size = 0,(self)->ml_macros = NULL)
#else
#define _TPPMacroList_Init       TPPMacroList_Init
#endif
TPP_DECL_FUNC(void) TPPMacroList_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPMacroList_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPMacroList_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Adds a macro to "self".
// NOTE: The macro is inherited by the list
// Returns: 0:  no error
// Returns: 1:  macro already existed (but got overwritten)
// Returns: -1: allocation error
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_Add(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroObject *macro)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,3));

//////////////////////////////////////////////////////////////////////////
// Returns true, if "name" is defined (NOTE: "name" may be a builtin macro)
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_Defined(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns true, if "name" is a builtin macro.
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPMacroList_IsBuiltinMacro(TPPTokenID name);

#if TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO
//////////////////////////////////////////////////////////////////////////
// Push / Pop macros from "self"
// Returns 0 on success
// TPPMacroList_PushMacro Returns -1 on error
// TPPMacroList_PopMacro Returns 1, if the macro wasn't previously pushed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_PushMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPMacroList_PopMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#endif /* TPP_CONFIG_HAVE_PRAGMA_PUSH_POP_MACRO */

//////////////////////////////////////////////////////////////////////////
// #undef's a Macro "name" from "self"
// Returns: 0:  no error
// Returns: -1: Macro didn't exist
// Returns: -2: can't #undef Builtin macro
TPP_DECL_FUNC(int) TPPMacroList_Undef(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// TPPMacroObject *TPPMacroList_LOOKUP(TPPMacroListObject const *self, TPPTokenID name)
// >> Returns the macro associated with "name", or NULL, if the macro doesn't exist.
// >> NOTE: "name" must be >= "TPP_TOK_IDENT_START"
#ifdef TPP_LIMITED_API
#define TPPMacroList_LOOKUP(self,name) (\
 ((name)-TPP_TOK_IDENT_START) >= (self)->ml_size ? NULL : (\
 TPP_ASSERT((name) >= TPP_TOK_IDENT_START),\
 (self)->ml_macros[(name)-TPP_TOK_IDENT_START]))
#else
#define TPPMacroList_LOOKUP TPPMacroList_Lookup
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPMacroObject *) TPPMacroList_Lookup(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPMacroListObject const *self, TPPTokenID name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));


#ifdef TPP_LIMITED_API
struct TPPIncludeListObject {
 size_t il_size;  /*< Amount of #include paths. */
 char **il_paths; /*< [1..1][owned][0..il_size][owned] Vector of #include paths. */
};
#define TPPIncludeList_INIT() {0,NULL}
#define _TPPIncludeList_Init(self) ((self)->il_size = 0,(self)->il_paths = NULL)
#else
#define _TPPIncludeList_Init       TPPIncludeList_Init
#endif
TPP_DECL_FUNC(void) TPPIncludeList_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIncludeList_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIncludeList_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

#define TPPIncludeList_HAS(self,path) TPPIncludeList_HasS(self,path,(sizeof(path)/sizeof(char))-1)
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_HasS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self, char const *path, size_t path_len)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_Has(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *path) {
 return TPPIncludeList_HasS(self,path,strlen(path));
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_HasL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 char const *path_begin, char const *path_end) {
 return TPPIncludeList_HasS(self,path_begin,(size_t)(path_end-path_begin));
}
//////////////////////////////////////////////////////////////////////////
// Add "path" to the #include list of "self"
// Returns -1, if the call failed.
// NOTE: "path" should be absolute, but can be relative to the
//       cwd of the application, as tpp never modifies the cwd
#define TPPIncludeList_ADD(self,path) TPPIncludeList_AddS(self,path,(sizeof(path)/sizeof(char))-1)
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_AddS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 char const *path, size_t path_len)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_Add(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *path) {
 return TPPIncludeList_AddS(self,path,strlen(path));
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPIncludeList_AddL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeListObject *self,
 char const *path_begin, char const *path_end) {
 return TPPIncludeList_AddS(self,path_begin,(size_t)(path_end-path_begin));
}


#ifdef TPP_LIMITED_API
struct TPPIfdefStackEntryObject {
 int            e_value; /*< Value of this entry. */
 TPPTokenObject e_token; /*< Token used in error / warning output. */
};
#define TPPIfdefStackEntry_Init(self,v,token_) (\
 (self)->e_value = (v),TPPToken_InitCopy(&(self)->e_token,token_))
#define TPPIfdefStackEntry_Quit(self) TPPToken_Quit(&(self)->e_token)
struct TPPIfdefStackObject {
 size_t                    s_size;  /*< Size of this stack. */
 size_t                    s_alloc; /*< Allocated size of this stack. */
 TPPIfdefStackEntryObject *s_stack; /*< [0..s_alloc][owned] Vector of this #ifdef-stack */
};
#define TPPIfdefStack_INIT()     {0,0,NULL}
#define _TPPIfdefStack_Init(self) ((self)->s_size = 0,(self)->s_alloc = 0,(self)->s_stack = 0)
#else
#define _TPPIfdefStack_Init       TPPIfdefStack_Init
#endif
TPP_DECL_FUNC(void) TPPIfdefStack_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIfdefStack_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIfdefStack_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Pushes an #ifdef onto "self"
// Returns -1, if the (re)allocation failed
// v:      The mask of what kind of block is begin pushed
// token_: The token used for debug/warning output
// NOTE: "token_" is copied into "self" and ownership still remains with the caller
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self, int v,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#ifdef TPP_LIMITED_API
#define TPPIfdefStack_POP(self) (\
 TPP_ASSERT((self)->s_size != 0 && "Empty stack"),\
 TPPIfdefStackEntry_Quit((self)->s_stack+(--(self)->s_size)))
#else
#define TPPIfdefStack_POP  TPPIfdefStack_Pop
#endif
TPP_DECL_FUNC(void) TPPIfdefStack_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the block-mask of the top-level #ifdef stack entry
// NOTE: "self" must be non-NULL and non-empty
#ifdef TPP_LIMITED_API
#define TPPIfdefStack_TOP(self) (/*TPP_ASSERT((self)->s_size != 0),*/(self)->s_stack[(self)->s_size-1].e_value)
#else
#define TPPIfdefStack_TOP       TPPIfdefStack_Top
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Top(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the token of the top-level #ifdef stack entry (s.a. "token_" in "TPPIfdefStack_Push")
// NOTE: "self" must be non-NULL and non-empty
#ifdef TPP_LIMITED_API
#define TPPIfdefStack_TOP_TOKEN(self) (/*TPP_ASSERT((self)->s_size != 0),*/&(self)->s_stack[(self)->s_size-1].e_token)
#else
#define TPPIfdefStack_TOP_TOKEN       TPPIfdefStack_TopToken
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPTokenObject *) TPPIfdefStack_TopToken(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns true, if "self" is empty
// NOTE: "self" must be non-NULL
#ifdef TPP_LIMITED_API
#define TPPIfdefStack_EMPTY(self) ((self)->s_size == 0)
#else
#define TPPIfdefStack_EMPTY       TPPIfdefStack_Empty
#endif
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPIfdefStack_Empty(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIfdefStackObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

#ifdef TPP_LIMITED_API
#ifndef TPPIncludeCache_SIZE
#define TPPIncludeCache_SIZE 64
#endif /* !TPPIncludeCache_SIZE */
struct TPPIncludeCacheEntry {
 struct TPPIncludeCacheEntry *ce_next;           /*< [0..1][owned] Next cache entry. */
 TPPTokenID                   ce_guard_name;     /*< [0..1] Name of the include-guard (Builtin macro, if #pragma once was used) */
 char                         ce_filename[1024]; /*< 0-terminated filename (should be absolute). */
};
extern struct TPPIncludeCacheEntry *TPPIncludeCacheEntry_Copy(struct TPPIncludeCacheEntry *self);
#define TPPIncludeCacheEntry_Free(self) TPP_CALLBACK_FREE_NN(self)
struct TPPIncludeCacheObject {
 TPPIncludeCacheEntry *ce_cache[TPPIncludeCache_SIZE]; /*< [0..1][owned][TPPIncludeCache_SIZE] include cache. */
};
#define _TPPIncludeCache_Init(self) (void)memset((self),0,sizeof(TPPIncludeCacheObject))
#else
#define _TPPIncludeCache_Init       TPPIncludeCache_Init
#endif

TPP_DECL_FUNC(void) TPPIncludeCache_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPIncludeCache_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIncludeCache_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(void) TPPIncludeCache_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the cache entry for a given filename or NULL, if no such cache exists
// NOTE: "filename" must be an absolute and '\\' / '/' fixed path
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_Get(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Use a builtin macro, like "KWD___LINE__", to signify #pragma once
// >> Returns NULL on error; Otherwise the new/old entry
// NOTE: Does nothing, if a #include guard was already specified for the given file
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_SetS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 char const *filename, size_t filename_len, TPPTokenID guard_name)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_Set(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPTokenID guard_name) {
 return TPPIncludeCache_SetS(self,filename,strlen(filename),guard_name);
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPIncludeCacheEntry *) TPPIncludeCache_SetL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPIncludeCacheObject *self,
 char const *filename_begin, char const *filename_end, TPPTokenID guard_name) {
 return TPPIncludeCache_SetS(self,filename_begin,(size_t)(filename_end-filename_begin),guard_name);
}



//////////////////////////////////////////////////////////////////////////
// Emits a warning
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Warn(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *msg)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,3));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Warnf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, ...)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,3))
 TPP_ATTRIBUTE_GCC_PRINTF_STRING(4,5);
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_VWarnf(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int code,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_PRINTF_STRING char const *fmt, va_list args)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,3));

//////////////////////////////////////////////////////////////////////////
// Warning emitters
#define TPPWarn_ExpectedKeywordAfterDefine(lexer,token_)           TPPLexer_Warnf(lexer, 0,token_,"Expected keyword after #define (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_RedefiningBuiltinKeyword(lexer,token_)             TPPLexer_Warnf(lexer, 1,token_,"Redefining builtin keyword: \"%s\"",TPPKeyword_BuiltinToStr((token_)->tk_id))
#define TPPWarn_UnknownPreprocessorDirective(lexer,token_)         TPPLexer_Warnf(lexer, 2,token_,"Unknown preprocessor directive: \"%s\"",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_StarSlashOutsideOfComment(lexer,token_)            TPPLexer_Warnf(lexer, 3,token_,"\"*/\" outside of a comment")
#define TPPWarn_DirectiveError(lexer,token_,message)               TPPLexer_Warnf(lexer, 4,token_,"#error : \"%s\"",(message))
#if TPP_CONFIG_HAVE_WARNING
#define TPPWarn_DirectiveWarning(lexer,token_,message)             TPPLexer_Warnf(lexer, 5,token_,"#warning : \"%s\"",(message))
#endif /* TPP_CONFIG_HAVE_WARNING */
#define TPPWarn_ExpectedKeywordOrLParenAfterDefined(lexer,token_)  TPPLexer_Warnf(lexer, 6,token_,"Expected keyword or \"(\" after \"defined\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedKeywordAfterDefined(lexer,token_)          TPPLexer_Warnf(lexer, 7,token_,"Expected keyword after \"defined(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterDefined(lexer,token_)           TPPLexer_Warnf(lexer, 8,token_,"Expected \")\" after \"defined ...\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedKeywordForMacroArgument(lexer,token_)      TPPLexer_Warnf(lexer, 9,token_,"Expected keyword for macro argument (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterVaArgs(lexer,token_)            TPPLexer_Warnf(lexer,10,token_,"Expected \")\" after \"...\" in macro function declaration (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedCommaOrRParenForMacroFuncDecl(lexer,token_)TPPLexer_Warnf(lexer,11,token_,"Expected \",\" or \")\" in macro function declaration (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE___VA_ARGS__
# define TPPWarn_InvalidMacroArgCount(lexer,token_,macro)          TPPLexer_Warnf(lexer,12,token_,"Macro \"%s\" : Invalid argument count (expected: %s%u)",TPPLexer_TokenIDStr(lexer,(token_)->tk_id),TPPMacro_IS_VA_ARGS_FUNCTION(macro) ? ">= " : "",(unsigned int)(TPPMacro_IS_VA_ARGS_FUNCTION(macro) ? (macro)->m_argc-1 : (macro)->m_argc))
#else /* TPP_CONFIG_HAVE___VA_ARGS__ */
# define TPPWarn_InvalidMacroArgCount(lexer,token_,macro)          TPPLexer_Warnf(lexer,12,token_,"Macro \"%s\" : Invalid argument count (expected: %u)",TPPLexer_TokenIDStr(lexer,(token_)->tk_id),(unsigned int)(macro)->m_argc)
#endif /* !TPP_CONFIG_HAVE___VA_ARGS__ */
#define TPPWarn_UnexpectedEOFInMacroArgList(lexer,token_,macro)    TPPLexer_Warnf(lexer,13,token_,"Macro \"%s\" : Unexpected EOF in argument list",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedIncludeString(lexer,token_)                TPPLexer_Warnf(lexer,14,token_,"Expected #include string (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT
#define TPPWarn_IncludeFileNotFound(lexer,token_,filename,inc_nx)  TPPLexer_Warnf(lexer,15,token_,"File not found: #%s \"%s\"",inc_nx ? "include_next" : "include",filename)
#define TPPWarn_SysIncludeFileNotFound(lexer,token_,filename,inc_nx)TPPLexer_Warnf(lexer,16,token_,"File not found: #%s <%s>",inc_nx ? "include_next" : "include",filename)
#else /* TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
#define TPPWarn_IncludeFileNotFound(lexer,token_,filename)         TPPLexer_Warnf(lexer,15,token_,"File not found: #include \"%s\"",filename)
#define TPPWarn_SysIncludeFileNotFound(lexer,token_,filename)      TPPLexer_Warnf(lexer,16,token_,"File not found: #include <%s>",filename)
#endif /* !TPP_CONFIG_HAVE_DIRECTIVE_INCLUDE_NEXT */
#if TPP_CONFIG_HAVE___TPP_COUNTER
#define TPPWarn_ExpectedLParenAfterTPPCounter(lexer,token_)        TPPLexer_Warnf(lexer,17,token_,"Expected keyword for \"<kwd>\" in \"__TPP_COUNTER(<kwd>)\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedKeywordAfterTPPCounter(lexer,token_)       TPPLexer_Warnf(lexer,18,token_,"Expected \"(\" after \"__TPP_COUNTER\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterTPPCounter(lexer,token_)        TPPLexer_Warnf(lexer,19,token_,"Expected \")\" after \"__TPP_COUNTER(<kwd>\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#endif /* TPP_CONFIG_HAVE___TPP_COUNTER */
#if TPP_CONFIG_HAVE___TPP_RANDOM
#define TPPWarn_ExpectedLParenAfterTPPRandom(lexer,token_)         TPPLexer_Warnf(lexer,20,token_,"Expected \"(\" after \"__TPP_RANDOM\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedIntegerAfterTPPRandom1(lexer,token_)       TPPLexer_Warnf(lexer,21,token_,"Expected <int> after \"__TPP_RANDOM(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedCommaAfterTPPRandom(lexer,token_)          TPPLexer_Warnf(lexer,22,token_,"Expected \",\" or \")\" after \"__TPP_RANDOM(<int>\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedIntegerAfterTPPRandom2(lexer,token_)       TPPLexer_Warnf(lexer,23,token_,"Expected <int> after \"__TPP_RANDOM(<int>,\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterTPPRandom(lexer,token_)         TPPLexer_Warnf(lexer,24,token_,"Expected \")\" after \"__TPP_RANDOM(<int>,<int>\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#endif /* TPP_CONFIG_HAVE___TPP_RANDOM */
#define TPPWarn_MacroAlreadyDefined(lexer,token_,macro,name)       TPPLexer_Warnf(lexer,25,token_,"Macro \"%s\" was already defined",TPPLexer_TokenIDStr(lexer,name))
#define TPPWarn_ExpectedKeywordAfterUndef(lexer,token_)            TPPLexer_Warnf(lexer,26,token_,"Expected keyword after #undef (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedKeywordAfterIfdef(lexer,token_)            TPPLexer_Warnf(lexer,27,token_,"Expected keyword after #ifdef (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ElseWithoutIfdef(lexer,token_)                     TPPLexer_Warn (lexer,28,token_,"#else without #if[n][def]")
#define TPPWarn_ElseAfterElse(lexer,token_)                        TPPLexer_Warn (lexer,29,token_,"#else after #else")
#define TPPWarn_ElifWithoutIfdef(lexer,token_)                     TPPLexer_Warn (lexer,30,token_,"#elif without #if[n][def]")
#define TPPWarn_ElifAfterElse(lexer,token_)                        TPPLexer_Warn (lexer,31,token_,"#elif after #else")
#define TPPWarn_IfdefWithoutEndif(lexer,token_)                    TPPLexer_Warnf(lexer,32,token_,"#%s without #endif",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_EndifWithoutIfdef(lexer,token_)                    TPPLexer_Warn (lexer,33,token_,"#endif without #if[n][def]")
#define TPPWarn_ExpectedIntAfterLine(lexer,token_)                 TPPLexer_Warnf(lexer,34,token_,"Expected <int> after #line (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedStringAfterLine(lexer,token_)              TPPLexer_Warnf(lexer,35,token_,"Expected <string> after #line <int> (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_MacroDoesntExist(lexer,token_)                     TPPLexer_Warnf(lexer,36,token_,"can\'t #undef \"%s\" (macro not found)",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_CantUndefBuiltinMacro(lexer,token_)                TPPLexer_Warnf(lexer,37,token_,"can\'t #undef \"%s\" (builtin macro)",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE___has_include
#define TPPWarn_ExpectedLParenAfterHasInclude(lexer,token_)        TPPLexer_Warnf(lexer,38,token_,"Expected \"(\" after \"__has_include\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterHasInclude(lexer,token_)        TPPLexer_Warnf(lexer,39,token_,"Expected \")\" after \"__has_include(...\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#endif /* TPP_CONFIG_HAVE___has_include */
#define TPPWarn_ExpectedCollonAfterQuestion(lexer,token_)          TPPLexer_Warnf(lexer,40,token_,"Expected \":\" after \"?\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedInteger(lexer,token_)                      TPPLexer_Warnf(lexer,41,token_,"Expected <int> (got: %s: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id),TPPToken_SStr(token_))
#define TPPWarn_ExpectedRparenAfterLParen(lexer,token_)            TPPLexer_Warnf(lexer,42,token_,"Expected \")\" after \"(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_UnexpectedTokenInConstExpr(lexer,token_)           TPPLexer_Warnf(lexer,43,token_,"Unexpected token %s: \"%s\" in constant expression replaced with \"0\"",TPPLexer_TokenIDStr(lexer,(token_)->tk_id),TPPToken_SStr(token_))
#define TPPWarn_ExpectedLParenAfterTPPEval(lexer,token_)           TPPLexer_Warnf(lexer,44,token_,"Expected \"(\" after \"__TPP_EVAL\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterTPPEval(lexer,token_)           TPPLexer_Warnf(lexer,45,token_,"Expected \")\" after \"__TPP_EVAL(<expr>\" (got: \"%s\" : \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id),TPPToken_SStr(token_))
#define TPPWarn_ExpectedLParenAfterPragma(lexer,token_)            TPPLexer_Warnf(lexer,46,token_,"Expected \"(\" after \"_Pragma\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedStringAfterPragma(lexer,token_)            TPPLexer_Warnf(lexer,47,token_,"Expected <string> after \"_Pragma(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterPragma(lexer,token_)            TPPLexer_Warnf(lexer,48,token_,"Expected \")\" after \"_Pragma(<string>\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_UnexpectedToken(lexer,token_,expected)             TPPLexer_Warnf(lexer,49,token_,"Expected \"%s\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,expected),TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_InvalidWarning(lexer,token_,id)                    TPPLexer_Warnf(lexer,50,token_,"Invalid warning: %d",(int)(id))
#define TPPWarn_CantPopWarnings(lexer,token_)                      TPPLexer_Warn (lexer,51,token_,"Can't pop warnings")
#define TPPWarn_TPPLoadFileNotFound(lexer,token_,filename,inc_sys,inc_nx) TPPLexer_Warnf(lexer,52,token_,"File not found : __TPP_LOAD_FILE(%c%s%c,%d)",(inc_sys) ? '<' : '\"',(filename),(inc_sys) ? '>' : '\"',(int)(inc_nx))
#define TPPWarn_ExpectedStringAfterPushMacro(lexer,token_)         TPPLexer_Warnf(lexer,53,token_,"Expected <string> after \"#pragma push_macro(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_CantPopMacro(lexer,token_,macro_name)              TPPLexer_Warnf(lexer,54,token_,"Can't #pragma pop_macro(\"%s\") (missing #pragma push_macro)",TPPLexer_TokenIDStr(lexer,macro_name))
#if TPP_CONFIG_HAVE___TPP_STR_DECOMPILE
#define TPPWarn_ExpectedLParenAfterTPPStrDecompile(lexer,token_)   TPPLexer_Warnf(lexer,55,token_,"Expected \"(\" after \"__TPP_STR_DECOMPILE\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedStringAfterTPPStrDecompile(lexer,token_)   TPPLexer_Warnf(lexer,56,token_,"Expected <string> after \"__TPP_STR_DECOMPILE(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParenAfterTPPStrDecompile(lexer,token_)   TPPLexer_Warnf(lexer,57,token_,"Expected \")\" after \"__TPP_STR_DECOMPILE(<string>\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#endif /* TPP_CONFIG_HAVE___TPP_STR_DECOMPILE */
#define TPPWarn_ExpectedLParen(lexer,token_)                       TPPLexer_Warnf(lexer,58,token_,"Expected \"(\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#define TPPWarn_ExpectedRParen(lexer,token_)                       TPPLexer_Warnf(lexer,59,token_,"Expected \")\" (got: \"%s\")",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
#define TPPWarn_DeprecatedKeyword(lexer,token_)                    TPPLexer_Warnf(lexer,60,token_,"#pragma deprecated: \"%s\"",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#define TPPWarn_InvalidPathCasing(lexer,token_,path,correct_path)  TPPLexer_Warnf(lexer,61,token_,"Invalid path casing \"%s\" should be \"%s\"",(path),(correct_path))
#define TPPWarn_DivideByZero(lexer,token_,lhs_val)                 TPPLexer_Warnf(lexer,62,token_,"Divide-By-Zero in constant expression: \"" TPPInteger_PRINTF_FMT " / 0\"",(lhs_val))
#define TPPWarn_ModuloByZero(lexer,token_,lhs_val)                 TPPLexer_Warnf(lexer,62,token_,"Modulo-By-Zero in constant expression: \"" TPPInteger_PRINTF_FMT " %% 0\"",(lhs_val))
#define TPPWarn_ReusedMacroParameter(lexer,token_)                 TPPLexer_Warnf(lexer,63,token_,"Macro function parameter name \"%s\" is already being used",TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE___VA_ARGS__
#define TPPWarn_VaArgsUsedAsMacroParameter(lexer,token_)           TPPLexer_Warn (lexer,64,token_,"Using \"__VA_ARGS__\" as macro parameter name")
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#if TPP_CONFIG_HAVE___VA_COMMA__
#define TPPWarn_VaCommaUsedAsMacroParameter(lexer,token_)          TPPLexer_Warn (lexer,65,token_,"Using \"__VA_COMMA__\" as macro parameter name")
#endif /* TPP_CONFIG_HAVE___VA_COMMA__ */
#define TPPWarn_Unexpected(lexer,token_,expected_str)              TPPLexer_Warnf(lexer,66,token_,"Expected \"%s\" (got: \"%s\")",(expected_str),TPPLexer_TokenIDStr(lexer,(token_)->tk_id))
#if TPP_CONFIG_HAVE___VA_ARGS__
#define TPPWarn_VaArgsMustBeLastParameter(lexer,token_)            TPPLexer_Warn (lexer,67,token_,"__VA_ARGS__ parameter must terminate parameter list")
#endif /* TPP_CONFIG_HAVE___VA_ARGS__ */
#define TPPWarn_ExpectedBoolExpression(lexer,token_,got)           TPPLexer_Warnf(lexer,68,token_,"Expected expression evaluating to 0 or 1 (got: \"" TPPInteger_PRINTF_FMT "\")",(got))
#define TPPWarn_ExpectedBoolExpressionNot(lexer,token_,got)        TPPLexer_Warnf(lexer,69,token_,"Expected x in \"!x\" evaluating to 0 or 1 (got: \"" TPPInteger_PRINTF_FMT "\"); replace with \"x != 0\"",(got))
#define TPPWarn_ExpectedBoolExpressionLhsOP(lexer,token_,op,got)   TPPLexer_Warnf(lexer,70,token_,"Expected x in \"x " op " y\" evaluating to 0 or 1 (got: \"" TPPInteger_PRINTF_FMT "\"); replace with \"(x != 0) " op " y\"",(got))
#define TPPWarn_ExpectedBoolExpressionRhsOP(lexer,token_,op,got)   TPPLexer_Warnf(lexer,71,token_,"Expected y in \"x " op " y\" evaluating to 0 or 1 (got: \"" TPPInteger_PRINTF_FMT "\"); replace with \"x " op " (y != 0)\"",(got))
#define TPPWarn_ExpectedKeyword(lexer,token_)                      TPPLexer_Warn (lexer,72,token_,"Expected keyword")


#define TPPWarnings_COUNT_BUILTIN       73  /*< Keep in sync with highest warning-id +1 */
#define TPPWarnings_COUNT_WITH_RESERVED 150 /*< Must be at least 'TPPWarnings_COUNT_BUILTIN' */

#ifdef TPPWarnings_COUNT_USER
#define TPPWarnings_COUNT  (TPPWarnings_COUNT_WITH_RESERVED+TPPWarnings_COUNT_USER)
#else /* TPPWarnings_COUNT_USER */
#define TPPWarnings_COUNT  TPPWarnings_COUNT_WITH_RESERVED
#endif /* !TPPWarnings_COUNT_USER */

TPP_DECL_ENUM_WITH_BASE(TPPWarningMode,short) {
   TPPWarningMode_Error          = -1,
   TPPWarningMode_Warn           =  0,
   TPPWarningMode_Ignored        =  1,
   TPPWarningMode_Suppress       =  2, // suppress once;
 //TPPWarningMode_SuppressTwice  =  3, // suppress twice
 //TPPWarningMode_SuppressThrice =  4, // ...
};

#ifdef TPP_LIMITED_API
#if !TPP_LOAD_STATIC
extern TPPWarningMode const TPPWarningsDefaultData[TPPWarnings_COUNT];
#endif
#define TPPWarnings_GET_DEFAULT(id) TPPWarningsDefaultData[id]
#endif

#ifdef TPP_LIMITED_API
struct TPPWarningsFrameObject {
 struct TPPWarningsFrameObject *wc_prev;        /*< [0..1][owned] Prev warnings configuration (used by push() / pop()) */
 TPPWarningMode wc_warnings[TPPWarnings_COUNT]; /*< Warnings configuration. */
};
#define TPPWarningsFrame_Calloc()   (TPPWarningsFrameObject *)TPP_CALLBACK_CALLOC_NZ(1,sizeof(TPPWarningsFrameObject))
#define TPPWarningsFrame_Alloc()    (TPPWarningsFrameObject *)TPP_CALLBACK_MALLOC_NZ(  sizeof(TPPWarningsFrameObject))
#define TPPWarningsFrame_Free(self) TPP_CALLBACK_FREE_NN(self)
struct TPPWarningsObject {
 TPPWarningsFrameObject *w_top; /*< [1..1][owned] Top warnings frame. */
};
#endif

//////////////////////////////////////////////////////////////////////////
// Initializes a Warnings Object
// Returns -1 on error
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject const *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Finalizes a Warnings Object
TPP_DECL_FUNC(void) TPPWarnings_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Push a warnings frame (Returns -1, if the associated allocation failed)
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPWarnings_Push(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Pop a warnings frame (Returns 1, if we are down to the last frame)
TPP_DECL_FUNC(int) TPPWarnings_Pop(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Get/Set a warning
#ifdef TPP_LIMITED_API
#define TPPWarnings_SET(self,code,value) ((self)->w_top->wc_warnings[code]=(TPPWarningMode)(value))
#define TPPWarnings_GET(self,code)        (self)->w_top->wc_warnings[code]
#else
#define TPPWarnings_SET TPPWarnings_Set
#define TPPWarnings_GET TPPWarnings_Get
#endif
TPP_DECL_FUNC(void) TPPWarnings_Set(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self, int code, TPPWarningMode value)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPWarningMode) TPPWarnings_Get(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPWarningsObject *self, int code)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));




TPP_DECL_ENUM_WITH_BASE(TPPLexerFlags,unsigned int) {
 TPPLexer_FLAG_NONE          = 0x00000000,
 TPPLexer_FLAG_WANT_LF       = 0x00000001, // Return '\n' tokens
 TPPLexer_FLAG_WANT_SPC      = 0x00000002, // Return ' ' tokens
 TPPLexer_FLAG_NO_MACROS     = 0x00000004, // Macros will not be expanded
 TPPLexer_FLAG_NO_DIRECTIVES = 0x00000008, // Preprocessor directive will not be expanded
 TPPLexer_FLAG_ASM_COMMENTS  = 0x00000010, // Assembler comments will be removed
 TPPLexer_FLAG_ONE_FILE      = 0x00000020, // Files are not removed from the file-stack (when enabled, one must push "l_one_file_rec" and set it to 0)
 TPPLexer_FLAG_ONE_REAL_FILE = 0x00000040, // Only macro-files are removed from the file-stack
 TPPLexer_FLAG_INC_STRING    = 0x00000080, // Parse #include strings
 TPPLexer_FLAG_RAND_INIT     = 0x00000100, // rand() is initialized
#if TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
 TPPLexer_FLAG_NO_DEPRECATED = 0x00000200, // Don't warn about deprecated identifiers.
#endif /* TPP_CONFIG_HAVE_PRAGMA_DEPRECATED */
#if TPP_CONFIG_HAVE_COMMENT_TOKEN
 TPPLexer_FLAG_TOK_COMMENTS  = 0x00000400, // Enable comment tokens
#endif /* TPP_CONFIG_HAVE_COMMENT_TOKEN */
};

#ifdef TPP_LIMITED_API
struct TPPLexerObject {
 TPPFileObject        *l_files;            /*< [0..1][ref][intrusive] Stack of files (This is the pointer to the top file). */
 TPPKeywordListObject  l_keywords;         /*< Keyword known in this lexer. */
 TPPLexerFlags         l_flags;            /*< Parser flags. */
 TPPMacroListObject    l_macros;           /*< Macro namespace. */
 TPPIncludeListObject  l_include_list;     /*< #include "xxx" list. */
 TPPIncludeListObject  l_sys_include_list; /*< #include <xxx> list. */
 TPPIfdefStackObject   l_ifdef_stack;      /*< #ifdef stack. */
 TPPIncludeCacheObject l_include_cache;    /*< cache for include guards and #pragma once. */
 unsigned int          l_one_file_rec;     /*< Recursion for the ONE_FILE flag. */
 TPPWarningsObject     l_warnings;         /*< Warnings configuration. */
#if TPP_CONFIG_HAVE___COUNTER__
 unsigned int          l_counter;          /*< __COUNTER__ value. */
#endif /* TPP_CONFIG_HAVE___COUNTER__ */
};
#endif

//////////////////////////////////////////////////////////////////////////
// Initializes a Lexer Object
// NOTE: Returns -1, if "TPPKeywordList_SetupBuiltin" failed
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Init(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

#define TPP_LEXER_COPY_FLAG_NONE           0x00000000
#define TPP_LEXER_COPY_FLAG_FILES          0x00000001
#define TPP_LEXER_COPY_FLAG_KEYWORDS       0x00000002
#define TPP_LEXER_COPY_FLAG_MACROS        (0x00000004|TPP_LEXER_COPY_FLAG_KEYWORDS)
#define TPP_LEXER_COPY_FLAG_INCLUDES       0x00000008
#define TPP_LEXER_COPY_FLAG_SYSINCLUDES    0x00000010
#define TPP_LEXER_COPY_FLAG_INCLUDE_CACHE (0x00000020|TPP_LEXER_COPY_FLAG_KEYWORDS)
#define TPP_LEXER_COPY_FLAG_WARNINGS       0x00000040
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_InitCopy(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *right, unsigned int flags)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_InitMove(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *right)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Finalizes a Lexer Object
TPP_DECL_FUNC(void) TPPLexer_Quit(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Adds the standard include path, using the value of argv[0] from main
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_AddStdInc(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Simplified interface for executing a string, to define function macros.
// Returns -1, if an error occurred
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_ExecStringS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 char const *s, size_t string_length)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_ExecString(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *s) {
 return TPPLexer_ExecStringS(self,s,strlen(s));
}
TPP_DECL_STATIC_INLINE(int TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN
                       TPP_ATTRIBUTE_DEPRECATED_USE(
 TPPLexer_ExecStringS(TPPLexerObject *self, char const *s, size_t string_length))) TPPLexer_ExecStringEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, char const *s, size_t string_length) {
 return TPPLexer_ExecStringS(self,s,string_length);
}

//////////////////////////////////////////////////////////////////////////
// Quickly #define a macro:
// >> TPPLexer_FastDefine(...,"foo","bar")
// >> // Same as
// >> #define foo bar
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefine(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *code)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefineWithFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name, TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_FastDefineWithFileInherited(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *name,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,3));

//////////////////////////////////////////////////////////////////////////
// Returns true, if an equivalent macro file to "file" is already being expanded
// This function is used, to prevent infinite recursion:
// >> #define foo(a,b,c) 42+foo(a,b,c)
// >> foo(10,20,30) // expands to "42+foo(10,20,30)", as this function returns true during the 2nd expansion
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_ExpandingEquivalentMacroFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject const *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

#if TPP_CONFIG_HAVE_COMMENT_TOKEN
#define TPPLexer_IncludeFlag_GetFileFormatFlags(f)\
(TPPFileFormatFlag)(\
 ((f)&TPPLexer_IncludeFlag_FORMAT_FILE)!=0\
 ?(((f)&TPPLexer_IncludeFlag_KEEP_COMMENTS)!=0\
 ?TPPFileFormatFlag_ENABLED|TPPFileFormatFlag_KEEP_COMMENTS\
 :TPPFileFormatFlag_ENABLED):TPPFileFormatFlag_NONE)
#else
#define TPPLexer_IncludeFlag_GetFileFormatFlags(f)\
(TPPFileFormatFlag)(\
 ((f)&TPPLexer_IncludeFlag_FORMAT_FILE)!=0\
 ?TPPFileFormatFlag_ENABLED:TPPFileFormatFlag_NONE)
#endif

TPP_DECL_ENUM_WITH_BASE(TPPLexer_IncludeFlag,int) {
 TPPLexer_IncludeFlag_NONE          = 0x00,
 TPPLexer_IncludeFlag_SYS_INCLUDE   = 0x01,
 TPPLexer_IncludeFlag_INCLUDE_NEXT  = 0x02,
 TPPLexer_IncludeFlag_FORMAT_FILE   = 0x04,
 TPPLexer_IncludeFlag_FROM_CMD      = 0x08,
#ifdef TPP_PLATFORM_WINDOWS
 TPPLexer_IncludeFlag_DONT_WARN_CASING = 0x10,
#else /* TPP_PLATFORM_WINDOWS */
 TPPLexer_IncludeFlag_DONT_WARN_CASING = 0x00,
#endif /* !TPP_PLATFORM_WINDOWS */
 TPPLexer_IncludeFlag_KEEP_COMMENTS = 0x20,
};

//////////////////////////////////////////////////////////////////////////
// Returns the file for "filename"
// >> Returns NULL, if the file could not be found
// If "fail_if_cached" != NULL and the found file is disabled with a guard,
// *fail_if_cached is set to 1 and NULL is returned
// NOTE: Returns a new reference
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPLexer_FindIncludeExS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenObject const *token_,
 char const *filename, size_t filename_len, TPPLexer_IncludeFlag flags, int *fail_if_cached)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPLexer_FindIncludeEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenObject const *token_,
 char const *filename, TPPLexer_IncludeFlag flags, int *fail_if_cached) {
 return TPPLexer_FindIncludeExS(self,token_,filename,strlen(filename),flags,fail_if_cached);
}


#if TPP_CONFIG_HAVE_DEPRECATED_API
#define /*[DEPRECATED]*/TPPLexer_FindIncludeS(self,filename,filename_len,sys_include,include_next,format_file,fail_if_cached)\
 TPPLexer_FindIncludeExS(self,NULL,filename,filename_len,TPPLexer_IncludeFlag_DONT_WARN_CASING|\
  ((sys_include) ? TPPLexer_IncludeFlag_SYS_INCLUDE : TPPLexer_IncludeFlag_NONE) | \
  ((include_next) ? TPPLexer_IncludeFlag_INCLUDE_NEXT : TPPLexer_IncludeFlag_NONE) | \
  ((format_file) ? TPPLexer_IncludeFlag_FORMAT_FILE : TPPLexer_IncludeFlag_NONE),fail_if_cached)
TPP_DECL_STATIC_INLINE(/*[DEPRECATED]*/TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN TPPFileObject *) TPPLexer_FindInclude(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename,
 int sys_include, int include_next, int format_file, int *fail_if_cached) {
 return TPPLexer_FindIncludeS(self,filename,strlen(filename),sys_include,include_next,format_file,fail_if_cached);
}
#endif /* TPP_CONFIG_HAVE_DEPRECATED_API */


//////////////////////////////////////////////////////////////////////////
// Public interface: #include a given file
// Returns 0, on success
// Returns -1, if the file could not be found
// >> #include      "file" // same as TPPLexer_IncludeEx(...,file,TPPLexer_IncludeFlag_NONE)
// >> #include_next "file" // same as TPPLexer_IncludeEx(...,file,TPPLexer_IncludeFlag_INCLUDE_NEXT)
// >> #include      <file> // same as TPPLexer_IncludeEx(...,file,TPPLexer_IncludeFlag_SYS_INCLUDE)
// >> #include_next <file> // same as TPPLexer_IncludeEx(...,file,TPPLexer_IncludeFlag_INCLUDE_NEXT|TPPLexer_IncludeFlag_SYS_INCLUDE)
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPLexer_IncludeFlag flags)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#if TPP_CONFIG_HAVE_DEPRECATED_API
#define /*[DEPRECATED]*/TPPLexer_Include(self,filename,sys_include,include_next) \
 TPPLexer_IncludeEx((self),NULL,(filename),\
  /*TPPLexer_IncludeFlag_DONT_WARN_CASING|*/TPPLexer_IncludeFlag_FORMAT_FILE|\
  ((sys_include) ? TPPLexer_IncludeFlag_SYS_INCLUDE : TPPLexer_IncludeFlag_NONE) |\
  ((include_next) ? TPPLexer_IncludeFlag_INCLUDE_NEXT : TPPLexer_IncludeFlag_NONE))
#endif /* TPP_CONFIG_HAVE_DEPRECATED_API */
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeStringS(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, char const *code, size_t code_len)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeString(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *code) {
 return TPPLexer_IncludeStringS(self,code,strlen(code));
}
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_IncludeStringL(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, char const *code_begin, char const *code_end) {
 TPP_ASSERT(code_end >= code_begin);
 return TPPLexer_IncludeStringS(self,code_begin,(size_t)(code_end-code_begin));
}

//////////////////////////////////////////////////////////////////////////
// Public interface: __has_include / __has_include_next
// Returns 1, on success
// Returns 0, if the file could not be found
// >> __has_include     ("file") // same as TPPLexer_HasInclude(...,file,0,0)
// >> __has_include_next("file") // same as TPPLexer_HasInclude(...,file,0,1)
// >> __has_include     (<file>) // same as TPPLexer_HasInclude(...,file,1,0)
// >> __has_include_next(<file>) // same as TPPLexer_HasInclude(...,file,1,1)
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_HasIncludeEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenObject const *token_,
 TPP_ATTRIBUTE_MSVC_ZERO_STRING char const *filename, TPPLexer_IncludeFlag flags)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
#if TPP_CONFIG_HAVE_DEPRECATED_API
#define /*[DEPRECATED]*/TPPLexer_HasInclude(self,filename,sys_include,include_next) \
 TPPLexer_HasIncludeEx((self),NULL,(filename),/*TPPLexer_IncludeFlag_DONT_WARN_CASING|*/\
  ((sys_include) ? TPPLexer_IncludeFlag_SYS_INCLUDE : TPPLexer_IncludeFlag_NONE) |\
  ((include_next) ? TPPLexer_IncludeFlag_INCLUDE_NEXT : TPPLexer_IncludeFlag_NONE))
#endif /* TPP_CONFIG_HAVE_DEPRECATED_API */

//////////////////////////////////////////////////////////////////////////
// Returns the public file of the lexer (that is the file,
// that we tell the user, the lexer is in right now)
// NOTE: The actual top-file may be a macro, but this
//       function always returns the top non-macro file
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPFileObject *) TPPLexer_PublicFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN int) TPPLexer_PublicFileDepthEx(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, int include_depth)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the public include-level (Used for "__INCLUDE_LEVEL__")
#define TPPLexer_PublicIncludeDepth(self) TPPLexer_PublicFileDepthEx(self,1)

//////////////////////////////////////////////////////////////////////////
// Goes alongside of "TPPLexer_PublicIncludeDepth":
// Returns the file-depth only for macros
#define TPPLexer_PublicMacroDepth(self) TPPLexer_PublicFileDepthEx(self,0)

//////////////////////////////////////////////////////////////////////////
// Returns the public base file of the lexer (that is the file,
// that we tell the user, the lexer is in right now)
// (Used for "__BASE_FILE__")
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN TPPFileObject *) TPPLexer_PublicBaseFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Pushes "file" on top of the file stack of "self".
// NOTE: "file->f_name" must be assigned and an absolute path
// NOTE: "file" must be != NULL and its reference counter will be incremented
TPP_DECL_FUNC(void) TPPLexer_PushFile(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Pushes "file" on top of the file stack of "self".
// NOTE: "file->f_name" must be assigned and an absolute path
// NOTE: TPPLexer_PushFileInherited only: return -1, if "file" == NULL
// NOTE: consumes a reference from "file"
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_PushFileInherited(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self, TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(void) TPPLexer_PushFileInheritedNoErr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPFileObject *file)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Read 1 token into "token_" (must be initialized)
// and moves the lexer to the next token
// Returns -1 on error
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_NextNoMacro(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Skips all space tokens in the current lexer
// Returns The next non-whitespace char
// Returns '\0', if the (last) file ended
// NOTE: This function respects the "TPPLexer_FLAG_ONE_FILE" flag and its recursion
TPP_DECL_FUNC(char) TPPLexer_SkipSpace(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));
TPP_DECL_FUNC(char) TPPLexer_PeekSpace(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Read 1 token into "token_" (must be initialized)
// and moves the lexer to the next token
// >> If the token describes a macro, it will be expanded (recursively)
// >> If preprocessor directives are encountered, they will be performed
// Returns -1 on error
// NOTE: This function respects all parser flags set in "self"
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Next(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns the name of a builtin keyword
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPKeyword_BuiltinToStr(TPPTokenID id);

//////////////////////////////////////////////////////////////////////////
// Returns name of any kind of valid token-id ([non-]keyword, builtin and user tokens)
// Although not exactly like TPPToken_SStr(), this function often returns similar values
TPP_DECL_FUNC(TPP_ATTRIBUTE_SHOULD_USE_RETURN char const *) TPPLexer_TokenIDStr(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self, TPPTokenID id)
 TPP_ATTRIBUTE_GCC_NON_NULL((1));

//////////////////////////////////////////////////////////////////////////
// Parses directives following "#pragma" with self.
// NOTE: May be called after "#pragma" or "decode(str) | str in _Pragma(str)"
// NOTE: "is_directive" is true, if the function is called from a "#pragma" (false, if called from _Pragma)
// Returns 1 if the pragma was unknown
// Returns 0 on success
// Returns -1 on error
// NOTE: When statically #including tpp, you can define a macro "TPPLexer_PARSE_PRAGMA", that
//       will be called with the same parameters as this function for unknown pragmas
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_ParsePragma(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, int is_directive)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Evaluate a constant expression starting with "token_".
// Allow for "," on the top level and only returns the last value of the tuple
// NOTE: If result is NULL, the expression will be skipped without any warnings
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_EvalConst(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Evaluate a constant expression starting with "token_".
// Does not evaluate tuples on the top level and should therefor
// be used, when processing function arguments
// NOTE: If result is NULL, the expression will be skipped without any warnings
TPP_DECL_FUNC(TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_EvalConst1(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPInteger *result)
 TPP_ATTRIBUTE_GCC_NON_NULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Enumerates the current #include tree, excluding the top file
TPP_DECL_FUNC(void) TPPLexer_EnumIncludeTree(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject const *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL void (*callback)(TPPFileObject const *file, int line, void *arg),
 void *arg) TPP_ATTRIBUTE_GCC_NON_NULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Skips a token "tk" or emits a warning, if the token wasn't encountered
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1,2)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Skip(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPTokenID tk) {
 TPP_ASSERT(token_);
 if (token_->tk_id == tk) {
  return TPPLexer_Next(self,token_);
 } else {
  if (TPPWarn_UnexpectedToken(self,token_,tk) == -1)
   return -1;
  return 1;
 }
}
//////////////////////////////////////////////////////////////////////////
// Checks, if "token_" is "tk" and emits a warning, if is isn't
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1,2)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_Check(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPTokenID tk) {
 TPP_ASSERT(token_);
 if (token_->tk_id != tk) {
  if (TPPWarn_UnexpectedToken(self,token_,tk) == -1)
   return -1;
 }
 return 0;
}
//////////////////////////////////////////////////////////////////////////
// Tries, to skip a token "tk"
// Returns -1, if "TPPLexer_Next" failed
// Returns  0, on success
// Returns  1, if a different token was encountered
TPP_DECL_STATIC_INLINE(TPP_ATTRIBUTE_GCC_NON_NULL((1,2)) TPP_ATTRIBUTE_USE_RETURN int) TPPLexer_TrySkip(
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPLexerObject *self,
 TPP_ATTRIBUTE_MSVC_NON_NULL TPPTokenObject *token_, TPPTokenID tk) {
 TPP_ASSERT(token_);
 return token_->tk_id == tk ? TPPLexer_Next(self,token_) : 1;
}

#ifdef __cplusplus
}
#endif
TPP_DECLS_END_DISABLE_WARNINGS

#if TPP_LOAD_STATIC
// Load the source statically
#include "tpp.c"
#endif /* TPP_LOAD_STATIC */

#endif /* !GUARD_TPP_H */
