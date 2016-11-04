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
#ifndef GUARD_DEEMON_VERSION_H
#ifndef GUARD_DEEMON___CONF_INL
#include "__conf.inl"
#endif
#ifndef GUARD_DEEMON_VERSION_H
#define GUARD_DEEMON_VERSION_H 1

// NOTE: If you add or remove features in this file, you have to
//       re-run "src/deemon/__codegen/make_feature_strings.dee"

//////////////////////////////////////////////////////////////////////////
// Deemon version control
//////////////////////////////////////////////////////////////////////////
// The version number is written 'api/compiler:revision' (e.g.: '102/102:0')
// NOTE: running 'deemon --version', or calling 'Dee_GetVersionString()'
//       will return this string among other things.
#define DEE_VERSION_API      102 // 12-bit
#define DEE_VERSION_COMPILER 102 // 12-bit
#define DEE_VERSION_REVISION 0   //  8-bit
#define DEE_VERSION_FULL \
     (((DEE_VERSION_API&DEE_UINT32_C(0x00000FFF))<<20)|\
 ((DEE_VERSION_COMPILER&DEE_UINT32_C(0x00000FFF))<<8)|\
  (DEE_VERSION_REVISION&DEE_UINT32_C(0x000000FF)))

#define DEE_VERSION_GET_API(ver)              ((DEE_TYPES_UINT16_T)(((ver)&DEE_UINT32_C(0xFFF00000))>>20))
#define DEE_VERSION_GET_COMPILER(ver)         ((DEE_TYPES_UINT16_T)(((ver)&DEE_UINT32_C(0x000FFF00))>>8))
#define DEE_VERSION_GET_REVISION(ver)         ((DEE_TYPES_UINT8_T)(((ver)&DEE_UINT32_C(0x000000FF))))
#define DEE_VERSION_GET_API_AND_COMPILER(ver) ((DEE_TYPES_UINT32_T)((ver)&DEE_UINT32_C(0xFFFFFF00)))
#define DEE_VERSION_MAKE(api,compiler,revision)\
     ((((api)&DEE_UINT32_C(0x00000FFF))<<20)|\
 (((compiler)&DEE_UINT32_C(0x00000FFF))<<8)|\
  ((revision)&DEE_UINT32_C(0x000000FF)))

// Deprecated names
#define DEE_API_VERSION      DEE_VERSION_API
#define DEE_COMPILER_VERSION DEE_VERSION_COMPILER
#define DEE_FULL_VERSION     DEE_VERSION_FULL


// Enables some hidden functionality that could break
// a lot of things if used by careless hands.
// >> Don't leave this enabled in actual release builds
#ifndef DEE_DEVEL_BUILD
#ifdef DEE_DEBUG
#define DEE_DEVEL_BUILD 1
#else
#define DEE_DEVEL_BUILD 0
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// Disable exporting of certain API portions
// >> Seriously required as I check and found out that deemon
//    had more than 3000 exports (srsly! Can you imagine that?)
#ifndef DEE_CONFIG_API_HAVE_HASHMAP_EXPORTS
#define DEE_CONFIG_API_HAVE_HASHMAP_EXPORTS 0
#endif
#ifndef DEE_CONFIG_API_HAVE_HASHSET_EXPORTS
#define DEE_CONFIG_API_HAVE_HASHSET_EXPORTS 0
#endif
#ifndef DEE_CONFIG_API_HAVE_MATH_EXPORTS
#define DEE_CONFIG_API_HAVE_MATH_EXPORTS    0
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// NOTES:
//   - All language features can be queried with '__has_feature',
//     after stripping away 'DEE_CONFIG_LANGUAGE_HAVE_' and converting to lower-case:
//     -> DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF -> __has_feature(static_if)
//
//   - All runtime features can be queried with '__has_extension',
//     after stripping away 'DEE_CONFIG_RUNTIME_HAVE_' and converting to lower-case:
//     -> DEE_CONFIG_RUNTIME_HAVE_ARRAYS -> __has_extension(arrays)
// 
// s.a.: The 'deemon -F' code generator near the end of '<src/deemon/compiler/__keywords.inl>'
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// v This one would technically work, but I don't want
//   to publish the address of my private server.
//   So until I find a more secure solution for this,
//   this won't get enabled anytime soon.
#ifndef DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
#define DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK 0
#endif

//////////////////////////////////////////////////////////////////////////
// >> __has_extension(platform_socket)
//    Enable the socket API
#ifndef DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
#define DEE_CONFIG_RUNTIME_HAVE_SOCKET_API   1
#endif

//////////////////////////////////////////////////////////////////////////
// Allows the user to reconfigure deemon at runtime,
// using calls to 'DeeFS_SetEnv'. e.g.:
// >> DeeFS_SetEnv("DEEMON_NOVFS","0");
// >> DeeRun_String("#include <fs>\nprint fs::dir(\"/proc/\")...;\n",NULL);
// >> DeeFS_SetEnv("DEEMON_NOVFS","1");
// >> DeeRun_String("#include <fs>\nprint fs::dir(\"/proc/\")...;\n",NULL); // Unknown directory (because vfs was disabled)
#ifndef DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
#define DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV  1
#endif

//////////////////////////////////////////////////////////////////////////
// >> __has_extension(platform_process)
//    Enable the process API
#ifndef DEE_CONFIG_RUNTIME_HAVE_PROCESS
#define DEE_CONFIG_RUNTIME_HAVE_PROCESS  DEE_PLATFORM_HAVE_PROCESS
#endif

//////////////////////////////////////////////////////////////////////////
// >> __has_extension(platform_vfs)
//    Enables the virtual filesystem that simulates some of the basic
//    files as they are found in a posix-style filesystem
//    NOTE: Can only be enabled on windows
#ifndef DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_CONFIG_RUNTIME_HAVE_VFS 1
#endif

//////////////////////////////////////////////////////////////////////////
// Automatically convert long path names to UNC, easily breaking
// the wall of PATH_MAX for deemon user code.
// NOTE: Can only be enabled on windows
#ifndef DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#define DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC 1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables documentation strings (accessible through. __builtin_doc / DeeObject_Doc)
#ifndef DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
#define DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS 1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables checks for the stacklimit when executing user code
// NOTE: It is not recommended to disable this
#ifndef DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
#define DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS 1
#endif

//////////////////////////////////////////////////////////////////////////
// Strictly use the internal names of the builtin integral types
// When this is enabled:  'int.__name__' returns '__int32' (by default)
// When this is disabled: 'int.__name__' returns 'int'
#ifndef DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES
#define DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES 0
#endif

//////////////////////////////////////////////////////////////////////////
// Protect lvalue operations with signals
// >> Slows down operations with pointers, but allows deemon exceptions
//    to be thrown rather when working with invalid pointers,
//    rather than the entire runtime breaking.
#ifndef DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#define DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED 1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable the use of signal handlers
// >> Catches signals such as 'SIGSEGV' / 'EXCEPTION_ACCESS_VIOLATION' in
//    specially protected regions of code and turns them into deemon exceptions.
// NOTE: Disabled internally not supported by the Host system
#ifndef DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#define DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS 1
#endif


//////////////////////////////////////////////////////////////////////////
// Restrict runtime/language features to not include those ~crazy~ ideas of mine
// Level 0: All features enabled (pointers, extern, ...)
// Level 1: Only safe features enabled (classes, most builtins, ...)
// Level 2: No features enabled (only basic functionality, ...)
// NOTE: The runtime restrictions work the same
#ifndef DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL
#define DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL  0
#endif
#ifndef DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL
#define DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL
#endif

#ifndef DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
#define DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS    1
#endif

//////////////////////////////////////////////////////////////////////////
// Language features
// NOTE: Only minimal testing is performed outside this default configuration (especially exceptions)

// Exceptions in user code ('try, catch, finally, throw')
// NOTE: Internal exceptions are still thrown, but can't be handled
#ifndef DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS             DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
#endif
// hide statements in expressions ('a = ({ print "foobar"; });')
#ifndef DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
#define DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS        1
#endif
// Ignore padding ',' in comma-lists (NOTE: does not affect print statements)
#ifndef DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
#define DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST     1
#endif

//////////////////////////////////////////////////////////////////////////
// allow '[]', '[=]' and '[&]' as alias for 'function'
#ifndef DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
#define DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX          1
#endif

//////////////////////////////////////////////////////////////////////////
// allow the use of the '__stack' variable modifier in usercode
#ifndef DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES
#define DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES        1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable the switch expression syntax
#ifndef DEE_CONFIG_LANGUAGE_HAVE_SWITCH_EXPRESSION
#define DEE_CONFIG_LANGUAGE_HAVE_SWITCH_EXPRESSION      1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable the operator function syntax
#ifndef DEE_CONFIG_LANGUAGE_HAVE_OPERATOR_FUNCTIONS
#define DEE_CONFIG_LANGUAGE_HAVE_OPERATOR_FUNCTIONS     1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable generator expression as short forms for yield-functions:
// >> print (for (x: util::range(5)) x*2)...; // 0 2 4 6 8
// >> // Internally transformed into:
// >> print ([]{ for (x: util::range(5)) yield x*2; }())...; // 0 2 4 6 8
// This works with __foreach(x: y), for(x: y) and for(i = 0; i < 10; ++i) -style loops
// WARNING: Keep in mind that this implies the loop being executed in a different strong scope!
#ifndef DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
#define DEE_CONFIG_LANGUAGE_HAVE_GENERATORS             1
#endif

//////////////////////////////////////////////////////////////////////////
// parse 'elif' as an alias for 'else if'
#ifndef DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
#define DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS     1
#endif

//////////////////////////////////////////////////////////////////////////
// conditional expression that look like (kind-of-like) if statements
// >> x = if (foo()) 10 else 20;
// >> // Same as:
// >> x = foo() ? 10 : 20;
#ifndef DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
#define DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION          1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables use of 'struct' and 'union' types in user code
#ifndef DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
#define DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES           1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables use of 'enum' types in user code
#ifndef DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
#define DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES             1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables use of 'alias' declarations
// Work as a hybrid between macros and constants,
// in that they will be replaced with different
// code when used somewhere, while still allowing
// for scoping by being resolved by the parser.
#ifndef DEE_CONFIG_LANGUAGE_HAVE_ALIAS
#define DEE_CONFIG_LANGUAGE_HAVE_ALIAS                  1
#endif

#ifndef DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#define DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES             1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables class syntax
#ifndef DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES            DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// The class vtable has a static size.
// - Enabling this option increases the size of smaller class types,
//   but drastically improves the performance of operator lookups.
// - You should only disable this in embedded environments
//   with little available heap memory.
#ifndef DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE     1
#endif

//////////////////////////////////////////////////////////////////////////
// Enables cell syntax
#ifndef DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
#define DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX            1
#endif

//////////////////////////////////////////////////////////////////////////
// Use the high-level static_if yield function when parsing
// NOTES:
//   - The parser for this will always be available, but
//     will not be used by deemon if this is disabled
#ifndef DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF
#define DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF              1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable intrinsic support for '__if_exists' and '__if_not_exists'
#ifndef DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
#define DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS              1
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Stuff that could break existing usercode when disabled.
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifndef DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_CONFIG_RUNTIME_HAVE_POINTERS        1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION 1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE 1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#define DEE_CONFIG_RUNTIME_HAVE_ARRAYS          1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
#define DEE_CONFIG_RUNTIME_HAVE_NEWDELETE       0 /* Not done (yet) */
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_EXTERN
#define DEE_CONFIG_RUNTIME_HAVE_EXTERN          1
#endif

//////////////////////////////////////////////////////////////////////////
// Enable support for pointers/lvalues
// NOTES:
//   - Disabling this might currently break some parts of the standard library
//   - Does not disable the actual type-system or availability of pointers,
//     as pointer and lvalue types are still required for extern objects.
#ifndef DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_CONFIG_LANGUAGE_HAVE_POINTERS               DEE_CONFIG_RUNTIME_HAVE_POINTERS
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION        DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#define DEE_CONFIG_LANGUAGE_HAVE_ARRAYS                 DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define DEE_CONFIG_LANGUAGE_HAVE_EXTERN                 DEE_CONFIG_RUNTIME_HAVE_EXTERN
#endif

//////////////////////////////////////////////////////////////////////////
// Enables c++-style new/delete pointer instance allocation
// NOTES:
//   - Not fully done (yet)
//   - Requires DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#ifndef DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
#define DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE  DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
#endif

//////////////////////////////////////////////////////////////////////////
// >> __has_feature(runtime_exit)
//    Enable support for 'sys.exit' and friends in user code
// NOTES:
//   - Disable if you don't want user code to do that
#ifndef DEE_CONFIG_RUNTIME_HAVE_EXIT
#define DEE_CONFIG_RUNTIME_HAVE_EXIT  1
#endif


#ifndef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA     DEE_CONFIG_RUNTIME_HAVE_POINTERS
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP       1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT 1
#endif
#ifndef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND      1
#endif


//////////////////////////////////////////////////////////////////////////
// Builtin functions
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP               1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT           1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN           1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT           1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P         1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR        1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE        1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P 1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT         DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF             1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP               DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF           1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS             1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT             1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE        1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST            DEE_DEVEL_BUILD
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND              DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX                1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA             DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED             1
#endif
#ifndef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD               1
#endif



#if DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL > \
    DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL
#undef DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL
#define DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL
#endif


//////////////////////////////////////////////////////////////////////////
// Minimal configuration disables most, if not all there is to disable
#if DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL >= 1
#undef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA
#undef DEE_CONFIG_RUNTIME_HAVE_EXTERN
#undef DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#undef DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#undef DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
#undef DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA                     0
#define DEE_CONFIG_RUNTIME_HAVE_EXTERN                             0
#define DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION                    0
#define DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE             0
#define DEE_CONFIG_RUNTIME_HAVE_NEWDELETE                          0
#define DEE_CONFIG_RUNTIME_HAVE_POINTERS                           0
#endif
#if DEE_CONFIG_RUNTIME_RESTRICTION_LEVEL >= 2
#undef DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#undef DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#undef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
#undef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT
#undef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#undef DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#undef DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
#undef DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#undef DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#undef DEE_CONFIG_RUNTIME_HAVE_PROCESS
#undef DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
#undef DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#undef DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
#undef DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
#undef DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES
#undef DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_CONFIG_RUNTIME_HAVE_ARRAYS                             0
#define DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC                           0
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND                      0
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT                 0
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP                       0
#define DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES                        0
#define DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS                         0
#define DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP                       0
#define DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED        0
#define DEE_CONFIG_RUNTIME_HAVE_PROCESS                            0
#define DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV                    0
#define DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS                    0
#define DEE_CONFIG_RUNTIME_HAVE_SOCKET_API                         0
#define DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS                  0
#define DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES 0
#define DEE_CONFIG_RUNTIME_HAVE_VFS                                0
#endif




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                   -- Fix invalid Configurations --                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL >= 1
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
#undef DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#undef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#undef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
#undef DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
#undef DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA         0
#define DEE_CONFIG_LANGUAGE_HAVE_EXTERN                 0
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION        0
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE 0
#define DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE              0
#define DEE_CONFIG_LANGUAGE_HAVE_POINTERS               0
#endif

#if DEE_CONFIG_LANGUAGE_RESTRICTION_LEVEL >= 2
#undef DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED
#undef DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
#undef DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
#undef DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
#undef DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF
#undef DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
#define DEE_CONFIG_LANGUAGE_HAVE_ARRAYS                     0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND              0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT         0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR        0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P         0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD               0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX                0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS             0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT             0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST            0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP               0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT           0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT           0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP               0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN           0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF           0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE        0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF             0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P 0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE        0
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED             0
#define DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES                 0
#define DEE_CONFIG_LANGUAGE_HAVE_GENERATORS                 0
#define DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION              0
#define DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF                  0
#define DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES               0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_POINTERS
#undef DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#undef DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
#undef DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#undef DEE_CONFIG_RUNTIME_HAVE_EXTERN
#define DEE_CONFIG_LANGUAGE_HAVE_POINTERS                   0
#define DEE_CONFIG_RUNTIME_HAVE_NEWDELETE                   0
#define DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED 0
#define DEE_CONFIG_RUNTIME_HAVE_EXTERN                      0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#undef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#undef DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION       0
#define DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#undef DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#undef DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#define DEE_CONFIG_LANGUAGE_HAVE_ARRAYS 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
#undef DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
#define DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#undef DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
#define DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_EXTERN
#undef DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define DEE_CONFIG_LANGUAGE_HAVE_EXTERN 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#undef DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#undef DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#define DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED 0
#endif

#if !DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS && \
    !DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS && \
    !DEE_CONFIG_RUNTIME_HAVE_VFS && \
    !DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
#undef DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
#define DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV 0
#endif

#ifndef DEE_PLATFORM_WINDOWS
#undef DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_CONFIG_RUNTIME_HAVE_VFS 0
#undef DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
#define DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC 0
#endif





#define DEE_AUTOCONF_VARNAME_DEEMON_HOME          "DEEMON_HOME"
#define DEE_AUTOCONF_VARNAME_DEEMON_DEXPATH       "DEEMON_DEXPATH"
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DEE_AUTOCONF_VARNAME_DEEMON_NOVFS         "DEEMON_NOVFS"
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
#define DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT    "DEEMON_STACKLIMIT"
#endif
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
#define DEE_AUTOCONF_VARNAME_DEEMON_NOUPDATE      "DEEMON_NOUPDATE"
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
#define DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE "DEEMON_UDPPACKETSIZE"
#define DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG    "DEEMON_MAXBACKLOG"
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#define DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS "DEEMON_NOSIGHANDLERS"
#endif
#ifdef DEE_DEBUG
#define DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT      "DEEMON_NOASSERT"
#define DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE       "DEEMON_VERBOSE"
#endif

#ifndef DEE_DEFAULT_HOMEPATH
#ifdef DEE_PLATFORM_UNIX
// Only defined under unix: The default value '$DEEMON_HOMEPATH',
//                          also affecting '$DEEMON_DEXPATH'
// e.g.: DEE_DEFAULT_HOMEPATH == "/usr/lib/deemon" infers
//       '$DEEMON_DEXPATH' == '/usr/lib/deemon/dex'
#define DEE_DEFAULT_HOMEPATH "/usr/lib/deemon"
#endif
#endif

#endif /* !GUARD_DEEMON_VERSION_H */
#endif /* !GUARD_DEEMON_VERSION_H */
