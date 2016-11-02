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

//////////////////////////////////////////////////////////////////////////
// Deemon keyword for TPP
#define KWD(x)       DEF(KWD_##x,#x)
#define MACRO_KWD(x) TPP_MACRO_DEF(KWD_##x,#x)


#if 1
TPP_MACRO_DEF(KWD___func__,"__func__")
#else
DEF(KWD___func__,"__func__")
#endif
TPP_MACRO_DEF(KWD___FUNCTION__,"__FUNCTION__")

// builtin constants
DEF(KWD_none,"none")
#define DEE_PARSE_TOKENID_CASE_NONE \
 case KWD_none:
DEF(KWD_true,"true")
DEF(KWD_false,"false")
DEF(KWD_this,"this")

// Builtin integer types
TPP_MACRO_DEF(KWD___int8,"__int8")   DEF(KWD_int8,"int8")   DEF(KWD_int8_t,"int8_t")
TPP_MACRO_DEF(KWD___int16,"__int16") DEF(KWD_int16,"int16") DEF(KWD_int16_t,"int16_t")
TPP_MACRO_DEF(KWD___int32,"__int32") DEF(KWD_int32,"int32") DEF(KWD_int32_t,"int32_t")
TPP_MACRO_DEF(KWD___int64,"__int64") DEF(KWD_int64,"int64") DEF(KWD_int64_t,"int64_t")
#define DEE_PARSE_TOKENID_CASE_INT8  case KWD___int8:  case KWD_int8:  case KWD_int8_t:
#define DEE_PARSE_TOKENID_CASE_INT16 case KWD___int16: case KWD_int16: case KWD_int16_t:
#define DEE_PARSE_TOKENID_CASE_INT32 case KWD___int32: case KWD_int32: case KWD_int32_t:
#define DEE_PARSE_TOKENID_CASE_INT64 case KWD___int64: case KWD_int64: case KWD_int64_t:

DEF(KWD_uint8,"uint8")
DEF(KWD_uint8_t,"uint8_t")
DEF(KWD_uint16,"uint16")
DEF(KWD_uint16_t,"uint16_t")
DEF(KWD_uint32,"uint32")
DEF(KWD_uint32_t,"uint32_t")
DEF(KWD_uint64,"uint64")
DEF(KWD_uint64_t,"uint64_t")
#define DEE_PARSE_TOKENID_CASE_UINT8  case KWD_uint8:  case KWD_uint8_t:
#define DEE_PARSE_TOKENID_CASE_UINT16 case KWD_uint16: case KWD_uint16_t:
#define DEE_PARSE_TOKENID_CASE_UINT32 case KWD_uint32: case KWD_uint32_t:
#define DEE_PARSE_TOKENID_CASE_UINT64 case KWD_uint64: case KWD_uint64_t:

// Builtin integer type modifiers
DEF(KWD_const,"const")
DEF(KWD_constexpr,"constexpr")
DEF(KWD___const,"__const")
DEF(KWD___const__,"__const__")
DEF(KWD_volatile,"volatile")
DEF(KWD___volatile,"__volatile")
DEF(KWD___volatile__,"__volatile__")
DEF(KWD_char,"char")
DEF(KWD_wchar_t,"wchar_t")
DEF(KWD___wchar_t,"__wchar_t")
DEF(KWD_char16_t,"char16_t")
DEF(KWD_char32_t,"char32_t")
DEF(KWD_short,"short")
DEF(KWD_int,"int")
DEF(KWD_long,"long")
DEF(KWD_signed,"signed")
DEF(KWD___signed,"__signed")
DEF(KWD___signed__,"__signed__")
DEF(KWD_unsigned,"unsigned")
DEF(KWD___unsigned,"__unsigned")
DEF(KWD___unsigned__,"__unsigned__")
DEF(KWD_atomic,"atomic")
DEF(KWD__Atomic,"_Atomic")
DEF(KWD_float,"float")
DEF(KWD_double,"double")
#define DEE_PARSE_TOKENID_CASE_CONST    case KWD_const: case KWD___const: case KWD___const__: case KWD_constexpr:
#define DEE_PARSE_TOKENID_CASE_VOLATILE case KWD_volatile: case KWD___volatile: case KWD___volatile__:
#define DEE_PARSE_TOKENID_CASE_WCHAR_T  case KWD_wchar_t: case KWD___wchar_t:
#define DEE_PARSE_TOKENID_CASE_CHAR16_T case KWD_char16_t:
#define DEE_PARSE_TOKENID_CASE_CHAR32_T case KWD_char32_t:
#define DEE_PARSE_TOKENID_CASE_SIGNED   case KWD_signed: case KWD___signed: case KWD___signed__:
#define DEE_PARSE_TOKENID_CASE_UNSIGNED case KWD_unsigned: case KWD___unsigned: case KWD___unsigned__:
#define DEE_PARSE_TOKENID_CASE_ATOMIC   case KWD_atomic: case KWD__Atomic:

// Other builtin types
DEF(KWD_bool,"bool") DEF(KWD__Bool,"_Bool")
#define DEE_PARSE_TOKENID_CASE_BOOL \
 case KWD_bool: case KWD__Bool:
DEF(KWD_set,"set")
DEF(KWD_dict,"dict")
DEF(KWD_list,"list")
DEF(KWD_tuple,"tuple")
DEF(KWD_object,"object")

//////////////////////////////////////////////////////////////////////////
// Other builtin keywords
DEF(KWD_function,"function")
DEF(KWD_module,"module")
DEF(KWD_pack,"pack") // Forces following comma-separated expressions to be packed into a tuple

#if DEE_CONFIG_LANGUAGE_HAVE_ALIAS
DEF(KWD_alias,"alias")
#endif


//////////////////////////////////////////////////////////////////////////
// Builtin operator keywords / associated builtin functions
DEF(KWD_str,"str")   DEF(KWD_string,"string") DEF(KWD___str__,"__str__")
DEF(KWD_repr,"repr") DEF(KWD___repr__,"__repr__")
DEF(KWD_copy,"copy") DEF(KWD___copy__,"__copy__") // Copies the given expression using its copy constructor
DEF(KWD_move,"move") DEF(KWD___move__,"__move__") // Copies the given expression using its move constructor
DEF(KWD_type,"type") // Returns the type of the given expression
DEF(KWD_weak,"weak") // Returns a weak reference to the given expression


#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
// Keywords for exceptions
DEF(KWD_try,"try")
DEF(KWD_catch,"catch")
DEF(KWD_throw,"throw")
DEF(KWD_finally,"finally")
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

// Keyword operators
DEF(KWD_in,"in")
DEF(KWD_is,"is")

//////////////////////////////////////////////////////////////////////////
// Deemon extension namespace
DEF(KWD___builtin_object,"__builtin_object") // Returns a builtin object
DEF(KWD___builtin_dex,"__builtin_dex") // Returns a deemon extension object

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
DEF(KWD___noop,"__noop")
DEF(KWD___builtin_noop,"__builtin_noop")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED
DEF(KWD___builtin_unused,"__builtin_unused")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD
DEF(KWD___builtin_dead,"__builtin_dead")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
DEF(KWD___builtin_noexcept,"__builtin_noexcept")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
DEF(KWD___builtin_noreturn,"__builtin_noreturn")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
DEF(KWD___builtin_noeffect,"__builtin_noeffect")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
DEF(KWD___builtin_constant_p,"__builtin_constant_p")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
DEF(KWD___builtin_choose_expr,"__builtin_choose_expr")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
DEF(KWD___builtin_unreachable,"__builtin_unreachable")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
DEF(KWD___builtin_types_compatible_p,"__builtin_types_compatible_p")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
DEF(KWD___builtin_breakpoint,"__builtin_breakpoint")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
// Return the type of an expression if it is known at compile time (using type prediction)
// NOTE: Can produce constant expressions in situations where 'type' can't. Never generates any code from its argument
// HINT: __builtin_predictable;
DEF(KWD___builtin_typeof,"__builtin_typeof")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
DEF(KWD___builtin_help,"__builtin_help")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
DEF(KWD___builtin_offsetof,"__builtin_offsetof")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
DEF(KWD___builtin_exists,"__builtin_exists")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
DEF(KWD___builtin_expect,"__builtin_expect")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
DEF(KWD___builtin_predictable,"__builtin_predictable")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
DEF(KWD___builtin_exprast,"__builtin_exprast")
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
DEF(KWD___builtin_alloca,"__builtin_alloca")
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
DEF(KWD___builtin_bound,"__builtin_bound")
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

// Variable access modifiers
#define DEE_PARSE_TOKENID_CASE_GLOBAL   case KWD_global: case TPP_TOK_COLLON_COLLON:
KWD(global)  // force variable references from global scope (but can't create new vars)
KWD(local)   // force variable references from local scope (can create new vars)
KWD(forward) // Used to explicitly forward a variable
KWD(static)  // Used to declare a static variable (aka. modifiable constant)

#if DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES
KWD(__stack) // Used to declare a stack variable
#define DEE_PARSE_TOKENID_CASE_STACK   case KWD___stack:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES */

// force variable references from and explicit scope (can create new vars if local)
// Usage: '__scope' '(' <const_expr> ')' <keyword>
//        where const_expr is a negative constant integral expression,
//        describing the amount of scopes we have to walk upwards.
KWD(__scope)

// force variable references using the n'th available variable, starting in the local scope
// Usage: '__nth' '(' <const_expr> ')' <keyword>
//        where const_expr is a positive constant integral expression,
//        describing the amount of ignored variables +1
KWD(__nth)


// Statement keywords
#if !TPP_CONFIG_HAVE_DIRECTIVE_IMPORT
KWD(import)
#endif
KWD(extern)
KWD(print)
KWD(del)
KWD(return)
KWD(yield)
KWD(break)
KWD(continue)
KWD(goto)
KWD(for)
KWD(__foreach)
KWD(__loopever)
KWD(__looponce)
KWD(__loopnone)
KWD(__if_true)
KWD(__if_false)
KWD(while)
KWD(do)
KWD(switch)
KWD(case)
#if !TPP_CONFIG_HAVE_PRAGMA_WARNING
KWD(default)
#endif
KWD(typedef)

// if 'assert' is defined and the macros are changed, this might cause problems.
// So we just add the keyword the manual way (without actually writing it as a token!)
// KWD(assert)
DEF(KWD_assert,"assert")

// Alias keyword, so we can safely use assert from the 'assert' macro
DEF(KWD___assert,"__assert")


//////////////////////////////////////////////////////////////////////////
// Keywords for struct/union support
KWD(struct)
KWD(union)
KWD(extends)
#define DEE_PARSE_TOKENID_IS_BASE_INDICATOR(id)\
 ((id)==':'||(id)==KWD_extends/*||(id)=='('*/)

//////////////////////////////////////////////////////////////////////////
// Enum support
KWD(enum)

//////////////////////////////////////////////////////////////////////////
// Class support
KWD(class)
KWD(property)
KWD(operator)
KWD(final)

//////////////////////////////////////////////////////////////////////////
// function {
//   // compiles as: 'super.foo()' (aka. regular variable)
//   // Would produce an error for 'unknown variable'
//   super.foo();
// }
// function [[thiscall]] {
//   // compiles as: 'this.super.foo()'
//   // Emits a warning about unsafe use of 'super'
//   super.foo();
// }
// function [[thiscall,super(xxx)]] {
//   // compiles as: '__super(xxx,this).foo()'
//   // This is the implicit configuration of class member functions
//   super.foo();
// }
KWD(super)

//////////////////////////////////////////////////////////////////////////
// constexpr function __super(type object T) -> object
// constexpr function __super(type object T, object ob) -> object
KWD(__super)
KWD(__builtin_super) // Alternate keyword for '__super' (allows for querying with '__has_builtin')
TPP_DEF_BUILTIN("builtin_super")

//////////////////////////////////////////////////////////////////////////
// Dynamic memory (c++-style)
DEF(KWD_new,"new")
DEF(KWD_delete,"delete")

// static-if and __if_exists / __if_not_exists support
// NOTE: As an alias for '__static_if', you can also type 'static' 'if'
KWD(__static_if)
KWD(__static_else)
KWD(__static_elif)
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
KWD(__if_exists)
KWD(__if_not_exists)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS */


// Misc reserved keywords
// Probably never gonna use these,
// but better reserve them now...
// NOTE: But don't worry about using them variable names or
//       whatever for now. Just act like these weren't here
//       >> And if you should worry, the compiler will tell you
KWD(_Alignof)
KWD(_Thread_local)
KWD(_Complex)
KWD(_Imaginary)
KWD(_Static_assert)

KWD(from)
KWD(auto)
KWD(const_cast)
KWD(decltype)
KWD(dynamic_cast)
KWD(exec)
KWD(synchronized)
KWD(explicit)
KWD(export)
KWD(friend)
KWD(inline)
KWD(mutable)
KWD(override)
KWD(reinterpret_cast)
KWD(sizeof)
KWD(static_cast)
KWD(template)
KWD(throws)
KWD(with)
KWD(protected)
KWD(private)
KWD(public)

//////////////////////////////////////////////////////////////////////////
// getset callback names (official / __FUNCTION__ / internal values)
KWD(__get__)   KWD(get)   KWD(read)
KWD(__del__) /*KWD(del)*/
KWD(__set__) /*KWD(set)*/ KWD(write) KWD(put)
#define DEE_PARSE_TOKENID_CASE_PROPERTY_GETTER case KWD___get__: case KWD_get: case KWD_read:
#define DEE_PARSE_TOKENID_CASE_PROPERTY_DELETE case KWD___del__: case KWD_delete: /* NOTE: 'del' not listed, due to special handling */
#define DEE_PARSE_TOKENID_CASE_PROPERTY_SETTER case KWD___set__: case KWD_set: case KWD_write: case KWD_put:



//////////////////////////////////////////////////////////////////////////
// Builtin operator functions
KWD(__constructor__)
KWD(__destructor__)
KWD(__assign__)
KWD(__copyassign__)
KWD(__moveassign__)
KWD(__bool__)
KWD(__double__)
KWD(__int32__)
KWD(__int64__)
KWD(__not__)
KWD(__neg__)
KWD(__pos__)
KWD(__inv__)
KWD(__inc__)
KWD(__dec__)
KWD(__incpost__)
KWD(__decpost__)
KWD(__lo__)
KWD(__le__)
KWD(__eq__)
KWD(__ne__)
KWD(__gr__)
KWD(__ge__)
KWD(__add__)
KWD(__iadd__)
KWD(__sub__)
KWD(__isub__)
KWD(__mul__)
KWD(__imul__)
KWD(__div__)
KWD(__idiv__)
KWD(__mod__)
KWD(__imod__)
KWD(__shl__)
KWD(__ishl__)
KWD(__shr__)
KWD(__ishr__)
KWD(__and__)
KWD(__iand__)
KWD(__or__)
KWD(__ior__)
KWD(__xor__)
KWD(__ixor__)
KWD(__pow__)
KWD(__ipow__)
KWD(__call__)
KWD(__size__)
KWD(__getitem__)
KWD(__setitem__)
KWD(__delitem__)
KWD(__getrange__)
KWD(__setrange__)
KWD(__delrange__)
KWD(__contains__)
KWD(__getattr__)
KWD(__delattr__)
KWD(__setattr__)
KWD(__iterself__)
KWD(__iternext__)
KWD(__hash__)

// File type operators
KWD(__read__)  KWD(__readnp__)
KWD(__write__) KWD(__writenp__)
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
KWD(__readp__)
KWD(__writep__)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
KWD(__seek__)
KWD(__flush__)
KWD(__trunc__)
KWD(__close__)

// Operators you can't override
KWD(__deepcopy__)
KWD(__is__)
KWD(__ref__)
KWD(__deref__)
KWD(__hasattr__)

// Alternative operator names (some people might prefer these...)
KWD(__lt__)
KWD(__gt__)


//////////////////////////////////////////////////////////////////////////
// Keywords for function attributes
DEF(KWD___attribute,"__attribute")
DEF(KWD___attribute__,"__attribute__")
DEF(KWD__declspec,"_declspec")
DEF(KWD___declspec,"__declspec")

#if 1
#define DEF_ATTRIBUTE(x) \
 DEF_TRAIT(x,(TPPKeywordFlag_HAS_ATTRIBUTE|\
              TPPKeywordFlag_HAS_CPP_ATTRIBUTE|\
              TPPKeywordFlag_HAS_DECLSPEC_ATTRIBUTE))
#else
#define DEF_ATTRIBUTE(x) \
 TPP_DEF_ATTRIBUTE(x) \
 TPP_DEF_CPP_ATTRIBUTE(x) \
 TPP_DEF_DECLSPEC_ATTRIBUTE(x)
#endif


#define DEF_CALLING_CONCENTION(x)\
 DEF_ATTRIBUTE(#x) \
 DEF(KWD_##x,#x) \
 DEF(KWD__##x,"_" #x) \
 DEF(KWD___##x,"__" #x)
DEF_CALLING_CONCENTION(syssv)
DEF_CALLING_CONCENTION(stdcall)
DEF_CALLING_CONCENTION(thiscall)
DEF_CALLING_CONCENTION(fastcall)
DEF_CALLING_CONCENTION(cdecl)
DEF_CALLING_CONCENTION(pascal)
DEF_CALLING_CONCENTION(register)
DEF_CALLING_CONCENTION(win64)
DEF_CALLING_CONCENTION(unix64)
#undef DEF_CALLING_CONCENTION
#define DEE_PARSE_TOKENID_CASE_CC_ALL(name)\
 case KWD_##name: case KWD__##name: \
 case KWD___##name: case KWD___##name##__:
#define DEE_PARSE_TOKENID_CASE_CC_SOME(name)\
 case KWD__##name: case KWD___##name:

KWD(_Alignas)
KWD(_Noexcept)
KWD(_Noreturn)

DEF(KWD_used,"used")         DEF_ATTRIBUTE("used")
DEF(KWD_unused,"unused")     DEF_ATTRIBUTE("unused")
DEF(KWD_name,"name")         DEF_ATTRIBUTE("name")
DEF(KWD_noreturn,"noreturn") DEF_ATTRIBUTE("noreturn")
DEF(KWD_nothrow,"nothrow")   DEF_ATTRIBUTE("nothrow")
DEF(KWD_packed,"packed")     DEF_ATTRIBUTE("packed")
DEF(KWD_aligned,"aligned")   DEF_ATTRIBUTE("aligned")
DEF(KWD_copyable,"copyable") DEF_ATTRIBUTE("copyable")
#if !TPP_CONFIG_HAVE_PRAGMA_DEPRECATED
DEF(KWD_deprecated,"deprecated")
#endif
DEF_ATTRIBUTE("deprecated")

// Used for declaring a variable that is bound to an explicit instance
KWD(instance_bound)
DEF_ATTRIBUTE("instance_bound")

// Used in extern functions that return 'DeeObject'.
// >> When set, deemon should add another reference to the returned object
KWD(add_result_reference)
DEF_ATTRIBUTE("add_result_reference")

KWD(uuid)
DEF_ATTRIBUTE("uuid")
#undef DEF_ATTRIBUTE

// Used for pragmas
KWD(deemon)
KWD(set_max_syntax_recursion)
KWD(set_function_name)
KWD(yield_token)
KWD(add_include_path)


// v Not a real keyword, but used as stub name for anonymous functions
KWD(__anonymous__)
// v Not a keyword either, but the default name of the root scope
KWD(__main__)

//////////////////////////////////////////////////////////////////////////
// Predefined macro keywords

#ifdef DEE_PLATFORM_WINDOWS
TPP_MACRO_DEF(KWD___WINDOWS__,"__WINDOWS__")
#endif
#ifdef DEE_PLATFORM_UNIX
TPP_MACRO_DEF(KWD___POSIX__,"__POSIX__")
#endif
#ifdef _WIN32
TPP_MACRO_DEF(KWD__WIN32,"_WIN32")
#endif
#ifdef _WIN64
TPP_MACRO_DEF(KWD__WIN64,"_WIN64")
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
TPP_MACRO_DEF(KWD___linux__,"__linux__")
#endif
#if defined(__unix__) || defined(__unix)
TPP_MACRO_DEF(KWD___unix__,"__unix__")
#endif
#if defined(__APPLE__)
TPP_MACRO_DEF(KWD___APPLE__,"__APPLE__")
#endif
#if defined(__MACH__)
TPP_MACRO_DEF(KWD___MACH__,"__MACH__")
#endif
#if defined(__CYGWIN__)
TPP_MACRO_DEF(KWD___CYGWIN__,"__CYGWIN__")
#endif
#if defined(__ANDROID__) || defined(__ANDROID) || defined(_ANDROID) || \
    defined(__android__) || defined(__android) || defined(_android)
TPP_MACRO_DEF(KWD___ANDROID__,"__ANDROID__")
#endif
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
TPP_MACRO_DEF(KWD___IPHONE__,"__IPHONE__")
#endif

TPP_MACRO_DEF(KWD___BOOL_DEFINED,"__BOOL_DEFINED")
#if DEE_CONFIG_RT_CHAR_SIGNED
TPP_MACRO_DEF(KWD___SIGNED_CHARS__,"__SIGNED_CHARS__")
#endif

#if DEE_DEVEL_BUILD
// Development build
TPP_MACRO_DEF(KWD___DEEMON_DEVEL__,"__DEEMON_DEVEL__")
#endif

#ifdef DEE_DEBUG
// Development build
TPP_MACRO_DEF(KWD___DEEMON_DEBUG__,"__DEEMON_DEBUG__")
#endif

/*[[[deemon
#include <file>
#include <fs>
fs::chdir(fs::path::head(__FILE__));
language_features = [];
runtime_features = [];
for (local line: file.io("../../../include/deemon/version.h","r")) {
  line = line.strip();
  if (line.startswith("#define DEE_CONFIG_RUNTIME_HAVE_")) {
    line = line[#"#define DEE_CONFIG_RUNTIME_HAVE_":];
    line = line.scanf("%[^ ]")...;
    runtime_features.append(line);
  } else if (line.startswith("#define DEE_CONFIG_LANGUAGE_HAVE_")) {
    line = line[#"#define DEE_CONFIG_LANGUAGE_HAVE_":];
    line = line.scanf("%[^ ]")...;
    language_features.append(line);
  }
}

runtime_features.sort();
runtime_features.unique();
language_features.sort();
language_features.unique();

for (local feature: language_features) {
  if (feature in runtime_features) {
    print "#if DEE_CONFIG_RUNTIME_HAVE_"+feature,"&& DEE_CONFIG_LANGUAGE_HAVE_"+feature;
    if (feature.startswith("BUILTIN")) {
      print "DEF_TRAIT("+repr feature.lower()+",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_BUILTIN)";
    } else {
      print "DEF_TRAIT("+repr feature.lower()+",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)";
    }
    print "#elif DEE_CONFIG_RUNTIME_HAVE_"+feature;
    print "TPP_DEF_EXTENSION("+repr feature.lower()+")";
    print "#elif DEE_CONFIG_LANGUAGE_HAVE_"+feature;
  } else {
    print "#if DEE_CONFIG_LANGUAGE_HAVE_"+feature;
  }
  if (feature.startswith("BUILTIN")) {
    print "TPP_DEF_BUILTIN("+repr feature.lower()+")";
  } else {
    print "TPP_DEF_FEATURE("+repr feature.lower()+")";
  }
  print "#endif";
}
for (local feature: runtime_features) {
  if (feature !in language_features) {
    print "#if DEE_CONFIG_RUNTIME_HAVE_"+feature;
    print "TPP_DEF_EXTENSION("+repr feature.lower()+")";
    print "#endif";
  }
}


]]]*/
#if DEE_CONFIG_LANGUAGE_HAVE_ALIAS
TPP_DEF_FEATURE("alias")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS && DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
DEF_TRAIT("arrays",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_ARRAYS
TPP_DEF_EXTENSION("arrays")
#elif DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
TPP_DEF_FEATURE("arrays")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA && DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
DEF_TRAIT("builtin_alloca",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_BUILTIN)
#elif DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA
TPP_DEF_EXTENSION("builtin_alloca")
#elif DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
TPP_DEF_BUILTIN("builtin_alloca")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND && DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
DEF_TRAIT("builtin_bound",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_BUILTIN)
#elif DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
TPP_DEF_EXTENSION("builtin_bound")
#elif DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
TPP_DEF_BUILTIN("builtin_bound")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT && DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
DEF_TRAIT("builtin_breakpoint",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_BUILTIN)
#elif DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT
TPP_DEF_EXTENSION("builtin_breakpoint")
#elif DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
TPP_DEF_BUILTIN("builtin_breakpoint")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
TPP_DEF_BUILTIN("builtin_choose_expr")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
TPP_DEF_BUILTIN("builtin_constant_p")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD
TPP_DEF_BUILTIN("builtin_dead")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
TPP_DEF_BUILTIN("builtin_dex")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
TPP_DEF_BUILTIN("builtin_exists")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
TPP_DEF_BUILTIN("builtin_expect")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
TPP_DEF_BUILTIN("builtin_exprast")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP && DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
DEF_TRAIT("builtin_help",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_BUILTIN)
#elif DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
TPP_DEF_EXTENSION("builtin_help")
#elif DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
TPP_DEF_BUILTIN("builtin_help")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
TPP_DEF_BUILTIN("builtin_noeffect")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
TPP_DEF_BUILTIN("builtin_noexcept")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
TPP_DEF_BUILTIN("builtin_noop")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
TPP_DEF_BUILTIN("builtin_noreturn")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
TPP_DEF_BUILTIN("builtin_offsetof")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
TPP_DEF_BUILTIN("builtin_predictable")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
TPP_DEF_BUILTIN("builtin_typeof")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
TPP_DEF_BUILTIN("builtin_types_compatible_p")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
TPP_DEF_BUILTIN("builtin_unreachable")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED
TPP_DEF_BUILTIN("builtin_unused")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
TPP_DEF_FEATURE("cell_syntax")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES && DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEF_TRAIT("class_types",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
TPP_DEF_EXTENSION("class_types")
#elif DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
TPP_DEF_FEATURE("class_types")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
TPP_DEF_FEATURE("elif_else_if_alias")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
TPP_DEF_FEATURE("enum_types")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS && DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
DEF_TRAIT("exceptions",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
TPP_DEF_EXTENSION("exceptions")
#elif DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
TPP_DEF_FEATURE("exceptions")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
TPP_DEF_FEATURE("expr_statements")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN && DEE_CONFIG_LANGUAGE_HAVE_EXTERN
DEF_TRAIT("extern",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_EXTERN
TPP_DEF_EXTENSION("extern")
#elif DEE_CONFIG_LANGUAGE_HAVE_EXTERN
TPP_DEF_FEATURE("extern")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION && DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
DEF_TRAIT("foreignfunction",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
TPP_DEF_EXTENSION("foreignfunction")
#elif DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
TPP_DEF_FEATURE("foreignfunction")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE && DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
DEF_TRAIT("foreignfunctionclosure",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
TPP_DEF_EXTENSION("foreignfunctionclosure")
#elif DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
TPP_DEF_FEATURE("foreignfunctionclosure")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
TPP_DEF_FEATURE("generators")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
TPP_DEF_FEATURE("if_exists")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
TPP_DEF_FEATURE("if_expression")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
TPP_DEF_FEATURE("lambda_syntax")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_NEWDELETE && DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
DEF_TRAIT("newdelete",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
TPP_DEF_EXTENSION("newdelete")
#elif DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
TPP_DEF_FEATURE("newdelete")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS && DEE_CONFIG_LANGUAGE_HAVE_POINTERS
DEF_TRAIT("pointers",TPPKeywordFlag_HAS_EXTENSION|TPPKeywordFlag_HAS_FEATURE)
#elif DEE_CONFIG_RUNTIME_HAVE_POINTERS
TPP_DEF_EXTENSION("pointers")
#elif DEE_CONFIG_LANGUAGE_HAVE_POINTERS
TPP_DEF_FEATURE("pointers")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
TPP_DEF_FEATURE("relaxed_comma_list")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES
TPP_DEF_FEATURE("stack_variables")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF
TPP_DEF_FEATURE("static_if")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
TPP_DEF_FEATURE("struct_types")
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_SWITCH_EXPRESSION
TPP_DEF_FEATURE("switch_expression")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
TPP_DEF_EXTENSION("auto_unc")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
TPP_DEF_EXTENSION("class_static_vtable")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
TPP_DEF_EXTENSION("docstrings")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_EXIT
TPP_DEF_EXTENSION("exit")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
TPP_DEF_EXTENSION("lvalue_ops_signal_protected")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
TPP_DEF_EXTENSION("process")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_RECONFIGURE_ENV
TPP_DEF_EXTENSION("reconfigure_env")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
TPP_DEF_EXTENSION("signal_handlers")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
TPP_DEF_EXTENSION("socket_api")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
TPP_DEF_EXTENSION("stacklimit_checks")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STRICT_INTERNAL_INTEGRAL_TYPENAMES
TPP_DEF_EXTENSION("strict_internal_integral_typenames")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
TPP_DEF_EXTENSION("update_check")
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
TPP_DEF_EXTENSION("vfs")
#endif
//[[[end]]]


#if 1
TPP_MACRO_DEF(KWD___INT8_DEFINED,"__INT8_DEFINED")
TPP_MACRO_DEF(KWD___INT16_DEFINED,"__INT16_DEFINED")
TPP_MACRO_DEF(KWD___INT32_DEFINED,"__INT32_DEFINED")
TPP_MACRO_DEF(KWD___INT64_DEFINED,"__INT64_DEFINED")
TPP_MACRO_DEF(KWD___UINT8_DEFINED,"__UINT8_DEFINED")
TPP_MACRO_DEF(KWD___UINT16_DEFINED,"__UINT16_DEFINED")
TPP_MACRO_DEF(KWD___UINT32_DEFINED,"__UINT32_DEFINED")
TPP_MACRO_DEF(KWD___UINT64_DEFINED,"__UINT64_DEFINED")
TPP_MACRO_DEF(KWD___INT8_T_DEFINED,"__INT8_T_DEFINED")
TPP_MACRO_DEF(KWD___INT16_T_DEFINED,"__INT16_T_DEFINED")
TPP_MACRO_DEF(KWD___INT32_T_DEFINED,"__INT32_T_DEFINED")
TPP_MACRO_DEF(KWD___INT64_T_DEFINED,"__INT64_T_DEFINED")
TPP_MACRO_DEF(KWD___UINT8_T_DEFINED,"__UINT8_T_DEFINED")
TPP_MACRO_DEF(KWD___UINT16_T_DEFINED,"__UINT16_T_DEFINED")
TPP_MACRO_DEF(KWD___UINT32_T_DEFINED,"__UINT32_T_DEFINED")
TPP_MACRO_DEF(KWD___UINT64_T_DEFINED,"__UINT64_T_DEFINED")
#endif

#if 1
// Some msvc-style predefined macros, indicating
// that we are defining additional character types.
TPP_MACRO_DEF(KWD__NATIVE_WCHAR_T_DEFINED,"_NATIVE_WCHAR_T_DEFINED")
TPP_MACRO_DEF(KWD__NATIVE_CHAR16_T_DEFINED,"_NATIVE_CHAR16_T_DEFINED")
TPP_MACRO_DEF(KWD__NATIVE_CHAR32_T_DEFINED,"_NATIVE_CHAR32_T_DEFINED")
TPP_MACRO_DEF(KWD__WCHAR_T_DEFINED,"_WCHAR_T_DEFINED")
TPP_MACRO_DEF(KWD__CHAR16_T_DEFINED,"_CHAR16_T_DEFINED")
TPP_MACRO_DEF(KWD__CHAR32_T_DEFINED,"_CHAR32_T_DEFINED")
TPP_MACRO_DEF(KWD___WCHAR_T_DEFINED,"__WCHAR_T_DEFINED")
TPP_MACRO_DEF(KWD___CHAR16_T_DEFINED,"__CHAR16_T_DEFINED")
TPP_MACRO_DEF(KWD___CHAR32_T_DEFINED,"__CHAR32_T_DEFINED")
#endif

#if DEE_CONFIG_RT_CHAR_SIGNED
TPP_MACRO_DEF(KWD___CHAR_SIGNED__,"__CHAR_SIGNED__")
#endif
#if DEE_CONFIG_RT_WCHAR_T_SIGNED
TPP_MACRO_DEF(KWD___WCHAR_SIGNED__,"__WCHAR_SIGNED__")
#endif
#if DEE_CONFIG_RT_CHAR16_T_SIGNED
TPP_MACRO_DEF(KWD___CHAR16_SIGNED__,"__CHAR16_SIGNED__")
#endif
#if DEE_CONFIG_RT_CHAR32_T_SIGNED
TPP_MACRO_DEF(KWD___CHAR32_SIGNED__,"__CHAR32_SIGNED__")
#endif


#undef MACRO_KWD
#undef KWD


