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
#ifndef GUARD_DEEMON_LEXER_H
#define GUARD_DEEMON_LEXER_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>

#ifdef GUARD_TPP_H
#error "Deemon must reconfigure tpp.h to function properly (don't #include <tpp.h> it yourself!)"
#endif

// Configure TPP for usage with deemon
// NOTE: TPP is linked under the same configuration
//       as deemon, meaning that it can be treated
//       an integrated library loaded alongside deemon.
#define TPP_LIMITED_API       1
#define TPP_DECL_DATA(T)      extern T
#define TPP_DECL_FUNC(T)      extern T
#define TPP_IMPL_DATA(decl)   decl
#define TPP_IMPL_FUNC(return) return
#define TPP_KEYWORD_FILE_0()  <deemon/compiler/__keywords.inl>
#define TPP_USE_DEBUG_NEW                   DEE_USE_DEBUG_NEW
#define TPP_CONFIG_HAVE_ARROW_TOKEN         1 // ->
#define TPP_CONFIG_HAVE_POW_TOKEN           1 // **
#define TPP_CONFIG_HAVE_LXOR_TOKEN          1 // ^^
#define TPP_CONFIG_HAVE_TILDE_TILDE_TOKEN   0 // ~~
#define TPP_CONFIG_HAVE_COLLON_COLLON_TOKEN 1 // ::
#define TPP_CONFIG_HAVE_COLLON_ASSIGN_TOKEN 1 // :=
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
#define TPP_CONFIG_HAVE_IF_COND_EXPRESSION  1 // Because deemon has this as well...
#else
#define TPP_CONFIG_HAVE_IF_COND_EXPRESSION  0
#endif
#define TPP_CONFIG_REEMIT_UNKNOWN_PRAGMAS   0 // Forward compatibility
#define TPP_ATTRIBUTE_DEPRECATED_PRIVATE_API /* nothing */
#ifdef DEE_DEBUG
#define TPP_DEBUG 1
#else
#define TPP_DEBUG 0
#endif
#define TPP_ASSERT DEE_ASSERT

// A little weird way of counting the amount of warnings,
// if it's stupid and works, it 'aint stupid!
enum{
#define WARNING(name,mode) DEE_PRIVATE_WARNING_COUNTER_##name,
#include <deemon/compiler/__warnings.inl>
#undef WARNING
#define TPPWarnings_COUNT_USER DEE_PRIVATE_WARNING_COUNT
 DEE_PRIVATE_WARNING_COUNT
};

#ifdef TPP_PRIVATE_RUNTIME_INTEGER_TYPES_DEFINED
#error "deemon already needs to override tpp's runtime types!"
#endif

#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif

// Setup TPP to use deemon runtime types
#define TPP_PRIVATE_RUNTIME_INTEGER_TYPES_DEFINED 1
typedef Dee_rt_int8_t   TPP_rt_int8_t;
typedef Dee_rt_int16_t  TPP_rt_int16_t;
typedef Dee_rt_int32_t  TPP_rt_int32_t;
typedef Dee_rt_int64_t  TPP_rt_int64_t;
typedef Dee_rt_uint8_t  TPP_rt_uint8_t;
typedef Dee_rt_uint16_t TPP_rt_uint16_t;
typedef Dee_rt_uint32_t TPP_rt_uint32_t;
typedef Dee_rt_uint64_t TPP_rt_uint64_t;
typedef Dee_rt_int      TPP_rt_int;
typedef Dee_rt_uint     TPP_rt_uint;
typedef Dee_rt_long     TPP_rt_long;
typedef Dee_rt_ulong    TPP_rt_ulong;
typedef Dee_rt_llong    TPP_rt_llong;
typedef Dee_rt_ullong   TPP_rt_ullong;
typedef Dee_rt_char     TPP_rt_char;
typedef Dee_rt_wchar_t  TPP_rt_wchar_t;
typedef Dee_rt_char16_t TPP_rt_char16_t;
typedef Dee_rt_char32_t TPP_rt_char32_t;


DEE_COMPILER_MSVC_WARNING_PUSH(4365)
#include <tpp.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif


#ifdef DEE_LIMITED_API
enum{
 DEE_PRIVATE_WARNING_BEFORE_FIRST = TPPWarnings_COUNT_WITH_RESERVED-1,
#define WARNING(name,mode) name,
#include <deemon/compiler/__warnings.inl>
#undef WARNING
 DEE_PRIVATE_WARNING_LAST
};

#if !defined(__INTELLISENSE__)
// Make sure we counted correctly
DEE_STATIC_ASSERT(TPPWarnings_COUNT == DEE_PRIVATE_WARNING_LAST);
#endif
#else
typedef unsigned int TPPTokenID;
#endif


DEE_OBJECT_DEF(DeeTokenObject);
DEE_OBJECT_DEF(DeeLexerObject);
DEE_OBJECT_DEF(DeeLexerIteratorObject);


//////////////////////////////////////////////////////////////////////////
// Lexer related objects

#ifdef DEE_LIMITED_API
struct DeeTokenObject {
 DEE_OBJECT_HEAD
#ifdef __INTELLISENSE__
  ;
#endif
 TPPTokenObject tk_token; /*< The token we are managing. */
};
#define DeeToken_TOKEN(ob)            (&((DeeTokenObject *)Dee_REQUIRES_POINTER(ob))->tk_token)
#define DeeToken_ID(ob)               ((DeeTokenObject *)Dee_REQUIRES_POINTER(ob))->tk_token.tk_id
#define DeeToken_FILE(ob)             TPPToken_File(DeeToken_TOKEN(ob))
#define DeeToken_LINE(ob)             TPPToken_Line(DeeToken_TOKEN(ob))
#define DeeToken_LINE_AFTER(ob)       TPPToken_LineAfter(DeeToken_TOKEN(ob))
#define DeeToken_AT_START_OF_LINE(ob) TPPToken_AtStartOfLine(DeeToken_TOKEN(ob))
#else
#define DeeToken_ID(ob)               DeeToken_Id((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeToken_FILE(ob)             DeeToken_File((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeToken_LINE(ob)             DeeToken_Line((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeToken_LINE_AFTER(ob)       DeeToken_LineAfter((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#ifdef DEE_LIMITED_API
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *DeeToken_NewFromTPPToken(DEE_A_IN TPPTokenObject const *tk);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeToken_StrTPPToken(TPPTokenObject const *self);
#endif


#define DeeToken_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeToken_Type)
#define DeeToken_Check          DeeToken_CheckExact
DEE_DATA_DECL(DeeTypeObject) DeeToken_Type;

DEE_FUNC_DECL(Dee_size_t) DeeToken_ClearPool(void);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *) DeeToken_New(void);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *) DeeToken_Copy(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeToken_Str(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED TPPTokenID) DeeToken_Id(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_Z_OPT char const *) DeeToken_File(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeToken_Line(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeToken_LineAfter(DEE_A_IN_OBJECT(DeeTokenObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF /*Any integer type*/DeeObject *) DeeToken_EvalInt(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self, DEE_A_IN_OBJECT(DeeLexerObject) *lexer);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF /*Any float type*/DeeObject *) DeeToken_EvalFloat(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self, DEE_A_IN_OBJECT(DeeLexerObject) *lexer);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeToken_EvalString(DEE_A_IN_OBJECT(DeeTokenObject) const *self);

#ifdef DEE_LIMITED_API
struct DeeStaticIfStackEntry {
 DEE_A_REF DeeTokenObject *sie_tok;             /*< [1..1] Token used when creating this entry */
           int             sie_value;           /*< Value of this entry. */
           int             sie_brace_recursion; /*< Brace recursion required to leave this entry. */
};
#define DeeStaticIfStackEntry_Quit(ob) Dee_DECREF((ob)->sie_tok)
struct DeeStaticIfStack {
 Dee_size_t                    si_c; /*< Amount of entries. */
 struct DeeStaticIfStackEntry *si_v; /*< [0..si_c][owned] Entry vector. */
};
#define DeeStaticIfStack_INIT() {0,NULL}
#define DeeStaticIfStack_EMPTY(ob) ((ob)->si_c==0)
#define DeeStaticIfStack_TOP(ob)   ((ob)->si_v+((ob)->si_c-1))
#define DeeStaticIfStack_POP(ob) \
 DeeStaticIfStackEntry_Quit((ob)->si_v+(--(ob)->si_c))
#define _DeeStaticIfStack_Init(ob)\
 ((ob)->si_c=0,(ob)->si_v=NULL)
extern DEE_A_RET_EXCEPT(-1) int DeeStaticIfStack_Push(
 struct DeeStaticIfStack *self, DeeTokenObject *tok, int val, int brace_rec);
extern DEE_A_RET_EXCEPT(-1) int DeeStaticIfStack_InitCopy(
 struct DeeStaticIfStack *self, struct DeeStaticIfStack const *right);
#define _DeeStaticIfStack_InitMove(ob,right)\
do{\
 (ob)->si_c = (right)->si_c,(right)->si_c = 0;\
 (ob)->si_v = (right)->si_v,(right)->si_v = NULL;\
}while(0)

extern void DeeStaticIfStack_Clear(DEE_A_INOUT struct DeeStaticIfStack *self);
extern void DeeStaticIfStack_Quit(DEE_A_IN struct DeeStaticIfStack *self);
#endif

#ifdef DEE_LIMITED_API
#define DEE_STRUCT_PACKING_DEFAULT DEE_TYPES_SIZEOF_POINTER
struct DeeStructPackingStack {
 Dee_size_t  sps_packing;   /*< Current struct packing mode. */
 Dee_size_t  sps_packing_c; /*< Size of the pack stack. */
 Dee_size_t *sps_packing_v; /*< [0..sps_packing_c] Pushed stack of pack levels. */
};
#define _DeeStructPackingStack_Init(ob) \
 ((ob)->sps_packing=DEE_STRUCT_PACKING_DEFAULT,\
  (ob)->sps_packing_c=0,\
  (ob)->sps_packing_v=NULL)

#define DeeStructPackingStack_SIZE(ob)  ((Dee_size_t const)(ob)->sps_packing_c)
#define DeeStructPackingStack_GET(ob)   ((Dee_size_t const)(ob)->sps_packing)
#define DeeStructPackingStack_SET(ob,v) (void)((ob)->sps_packing=(v))
#define DeeStructPackingStack_Quit(ob)  free((ob)->sps_packing_v)
extern DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_InitCopy(
 DEE_A_OUT struct DeeStructPackingStack *self, DEE_A_IN struct DeeStructPackingStack const *right);
extern void DeeStructPackingStack_InitMove(
 DEE_A_OUT struct DeeStructPackingStack *self, DEE_A_IN struct DeeStructPackingStack *right);

//////////////////////////////////////////////////////////////////////////
// Push / Pop the current packing level
extern DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_Push(DEE_A_INOUT struct DeeStructPackingStack *self);
extern DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_Pop(DEE_A_INOUT struct DeeStructPackingStack *self);
#endif

#ifdef DEE_LIMITED_API
struct DeeFutureTokensList {
           Dee_size_t       ftl_tokenc; /*< Amount of future tokens. */
           Dee_size_t       ftl_tokena; /*< Allocated amount of future tokens. */
 DEE_A_REF DeeTokenObject **ftl_tokenv; /*< [1..1][ftl_tokenc:0..1] List of future tokens. */
};
#define _DeeFutureTokensList_Init(ob) \
 ((ob)->ftl_tokenc=0,\
  (ob)->ftl_tokena=0,\
  (ob)->ftl_tokenv=NULL)
#define DeeFutureTokensList_SIZE(ob)    ((Dee_size_t const)(ob)->ftl_tokenc)
extern void DeeFutureTokensList_Quit(DEE_A_IN struct DeeFutureTokensList *self);
extern DEE_A_RET_EXCEPT(-1) int DeeFutureTokensList_InitCopy(
 DEE_A_OUT struct DeeFutureTokensList *self, DEE_A_IN struct DeeFutureTokensList const *right);
extern void DeeFutureTokensList_InitMove(
 DEE_A_OUT struct DeeFutureTokensList *self, DEE_A_IN struct DeeFutureTokensList *right);

#endif


#ifdef DEE_LIMITED_API
struct DeeFileObject;
struct DeeLexerObject {
 DEE_OBJECT_HEAD
#ifdef __INTELLISENSE__
  ;
#endif
           TPPLexerObject        l_lexer;             /*< The lexer object used for parsing. */
 DEE_A_REF DeeTokenObject       *l_token;             /*< [1..1] The token used during parsing. NOTE: Replaced, if not unique. */
 struct DeeFutureTokensList      l_future;            /*< List of future tokens. */
 DEE_A_REF struct DeeFileObject *l_error_out;         /*< [1..1] File used for outputting errors (default: file.io.stderr / file.io.stderr|file.io.stddbg). */
 DEE_A_REF struct DeeFileObject *l_message_out;       /*< [1..1] File used for #pragma message output (default: file.io.stderr). */
 Dee_uint32_t                    l_error_cur;         /*< Current amount of errors. */
 Dee_uint32_t                    l_error_max;         /*< Maximum amount of errors before the parser/compiler will abort (default: 100). */
 DEE_A_REF DeeObject            *l_error_last;        /*< [0..1] Last error that occurred (!= NULL if ob_error_cur != 0). */
 struct DeeStaticIfStack         l_static_if_stack;
 TPPTokenID                      l_func_name;         /*< Name of the current function (for the __FUNCTION__ builtin macro). */
 unsigned int                    l_suppress_warnings; /*< != 0, if warnings should be suppressed. */
 unsigned int                    l_suppress_Wcop;     /*< != 0, if invalid const-op warnings should be suppressed. */
 struct DeeStructPackingStack    l_pack;              /*< Structure packing. */
 Dee_uint64_t                    l_token_pos;
 Dee_int32_t                     l_brace_recursion;
 unsigned int                    l_syntax_recursion;
 unsigned int                    l_max_syntax_recursion;
};
#define DeeLexer_WSUPPRESS_BEGIN(ob) do{DEE_LDEBUG("Begin suppress warnings\n");++((DeeLexerObject *)Dee_REQUIRES_POINTER(ob))->l_suppress_warnings;}while(0)
#define DeeLexer_WSUPPRESS_END(ob)   do{--((DeeLexerObject *)Dee_REQUIRES_POINTER(ob))->l_suppress_warnings;DEE_LDEBUG("End suppress warnings\n");}while(0)

#define DeeLexer_LEXER(ob)  (&((DeeLexerObject *)Dee_REQUIRES_POINTER(ob))->l_lexer)
#define DeeLexer_TOKEN(ob)  ((DeeObject *)((DeeLexerObject *)Dee_REQUIRES_POINTER(ob))->l_token)
#define DeeLexer_FAILED(ob) (((DeeLexerObject *)Dee_REQUIRES_POINTER(ob))->l_error_cur!=0)
#else
#define DeeLexer_TOKEN(ob) DeeLexer_Token((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#ifdef DEE_LIMITED_API
struct DeeLexerIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeLexerObject *li_lexer; /*< [1..1] Lexer that is being iterated. */
};
#endif

#define DeeLexer_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeLexer_Type)
#define DeeLexer_Check                  DeeLexer_CheckExact
#define DeeLexerIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeLexerIterator_Type)
#define DeeLexerIterator_Check          DeeLexerIterator_CheckExact
DEE_DATA_DECL(DeeTypeObject) DeeLexer_Type;
DEE_DATA_DECL(DeeTypeObject) DeeLexerIterator_Type;

DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLexerObject) *) DeeLexer_New(void);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Init(DEE_A_IN DeeLexerObject *self);

//////////////////////////////////////////////////////////////////////////
// Returns a new lexer already including all StdInc paths
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLexerObject) *) DeeLexer_NewWithStdInc(void);


//////////////////////////////////////////////////////////////////////////
// Add standard include paths (not called by 'DeeLexer_Init'; must be called manually)
// Included paths are:
//  - $(dirname $(readlink /proc/self/exe))/include
//  - [ifdef DEEMON_HOME] foreach: $DEEMON_HOME/include
//  - [ifndef DEEMON_HOME && posix] /usr/lib/deemon/include
// foreach on $DEEMON_HOME means that the environment variable may
// contain ';' on windows and ':' to indicate multiple paths.
// Note, that every include path expands to two additional paths:
//  - $HOMEPATH/cxx
//  - $HOMEPATH/tpp
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_AddStdInc(DEE_A_INOUT_OBJECT(DeeLexerObject) *self);
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *) DeeLexer_Token(DEE_A_INOUT_OBJECT(DeeLexerObject) *self);

//////////////////////////////////////////////////////////////////////////
// Add an include path
// - The format functions are just meant for convenience
// NOTE: Environment vars and user folder are expanded in the given path!
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Utf8AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_WideAddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_AddSysIncludePathObject(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_Utf8AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_WideAddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_AddSysIncludePathObject(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Various include helper functions
// >> 'disp_filename' (if available) specifies the filename that is returned by __FILE__ and found in tracebacks
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFileStream(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFileStreamEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Utf8IncludeFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Utf8IncludeFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_WideIncludeFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_WideIncludeFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilenameObject(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilenameObjectEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeString(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeStringObject(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *code) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeStringWithLength(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeStringEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeStringObjectEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeStringWithLengthEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename) DEE_ATTRIBUTE_NONNULL((1,4));

//////////////////////////////////////////////////////////////////////////
// Yield one token from the lexer input
// Returns NULL on error
// NOTE: You can incref the returned token to prevent it from being
//       overwritten the next you call this function
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE
              DEE_A_RET_OBJECT_EXCEPT(DeeTokenObject) *) DeeLexer_Yield(
              DEE_A_INOUT_OBJECT(DeeLexerObject) *self);

#ifdef DEE_LIMITED_API
struct DeeCompilerConfig;
extern DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_EXCEPT(DeeTokenObject) *DeeLexer_YieldEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeScopeObject) *scope, DEE_A_IN struct DeeCompilerConfig const *config);
#endif

//////////////////////////////////////////////////////////////////////////
// Return a token, to be offered the next time "DeeLexer_Yield" is called
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Return(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeTokenObject) *token);

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int DeeLexer_ReturnTPPToken(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN TPPTokenObject const *token);
extern DEE_A_RET_EXCEPT(-1) int DeeLexer_SkipTokensUntil(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCompilerConfig const *config, DEE_A_IN TPPTokenID end_token_1,
 DEE_A_IN TPPTokenID end_token_2);
extern DEE_A_RET_EXCEPT(-1) int DeeLexer_SetFunctionNameStringWithLength(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN Dee_size_t s, DEE_A_IN_R(s) char const *name);
#endif

//////////////////////////////////////////////////////////////////////////
// When encountering a string, always call this function
// >> This is required to properly handle preprocessor string concantation
// NOTE: This also consumes all the string tokens
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *)
 DeeLexer_ParseString(DEE_A_INOUT_OBJECT(DeeLexerObject) *self);


struct DeeFunctionObject;
struct DeeCompilerConfig;

//////////////////////////////////////////////////////////////////////////
// If you will ever use a lexer for compiling deemon code,
// you probably want to call this function at one point.
//   - 'main_name' describes the name of the main-scope (aka. __FUNCTION__ value and name in tracebacks)
//     If not set, __FUNCTION__ will expand to '"__main__"' and tracebacks will list '???'
// This function:
//   1. Generates a statement ast from all code contained in 'self'.
//      While parsing, it uses parser_flags which should be a combination of 'DEE_PARSER_FLAG_*'
//   2. The generated ast is then optimized using optimize_flags,
//      which should be a combination of 'DEE_OPTIMIZE_FLAG_*'
//   3. With the finished statement ast generated, it is compiled into a code writer,
//      using 'compiler_flags' which should be a combination of 'DEE_COMPILE_FLAG_*'
//   4. Finally everything is written into a code object.
//   5. That code object is then packaged into a function object accepting a variadic
//      amount of arguments, that should specify the values of '__builtin_argv()'
//   6. Simply return that function object
// NOTE: NULL is returned alongside an error, if anything went wrong along the way
// NOTE: If 'DEE_COMPILER_FLAG_USED' is specified in 'compiler_flags', the last statement's value
//       will be returned (in the same way code expressions return values from statements).
//       But note, that a 'return' statement will always ensure a return value. And that a
//       'yield' statement will overrule this and make the returned function a yield function.
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeLexer_ParseAndCompile(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OPT struct DeeCompilerConfig const *config);

struct DeePreprocessConfig {
 DeeObject   *output_file;               /*< [1..1] Must be a file object (usually DeeFile_Std(DEE_STDOUT)). */
 unsigned int output_line_directives: 1; /*< -no-line. */
 unsigned int output_tok_brackets:    1; /*< -tok. */
 unsigned int padding_0: 30;
 Dee_uint32_t padding_1;
 Dee_uint32_t padding_2;
};

DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Preprocess(DEE_A_INOUT_OBJECT(DeeLexerObject) *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_PreprocessEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN struct DeePreprocessConfig const *config);

struct DeeDeemonFormatConfig {
 // Don't use a temporary file as output; instead overwrite the original file directly.
 //  - Ignored if 'in' isn't the same as 'out' or if 'in' isn't a 'file.io' object
 unsigned int unsafe_format: 1;
 unsigned int padding_0: 31;
 Dee_uint32_t padding[16];
};
#define DeeDeemonFormatConfig_INIT() \
 {0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}

//////////////////////////////////////////////////////////////////////////
// Searches for comments in all included files, executing special code that can be written inside.
// NOTE: Set out == in to output to the same file
// $ cat some_file.dee
// >> /*[[[deemon
// >> #include <random>
// >> print random().nextflt();
// >> ]]]*/
// >>  This will get deleted!
// >> //[[[end]]]
// $ deemon -F some_file.dee
// $ cat some_file.dee
// >> /*[[[deemon
// >> #include <random>
// >> print random().nextflt();
// >> ]]]*/
// >> 0.463721487541
// >> //[[[end]]]
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Utf8FormatFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename)
 DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_Utf8FormatFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_WideFormatFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename)
 DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_WideFormatFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilenameObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename)
 DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilenameObjectEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFileStream(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFileStreamEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out, DEE_A_IN struct DeeDeemonFormatConfig const *config)
 DEE_ATTRIBUTE_NONNULL((1,2,3,4));

DEE_DECL_END

#ifdef __cplusplus
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) { return DeeLexer_Utf8AddSysIncludePath(self,path); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) { return DeeLexer_WideAddSysIncludePath(self,path); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeLexer_AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) { return _DeeLexer_Utf8AddSysIncludePath(self,path); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) _DeeLexer_AddSysIncludePath(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) { return _DeeLexer_WideAddSysIncludePath(self,path); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename) { return DeeLexer_Utf8IncludeFilename(self,filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename) { return DeeLexer_WideIncludeFilename(self,filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *disp_filename) { return DeeLexer_Utf8IncludeFilenameEx(self,filename,disp_filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_IncludeFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *disp_filename) { return DeeLexer_WideIncludeFilenameEx(self,filename,disp_filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename) { return DeeLexer_Utf8FormatFilename(self,filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilename(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename) { return DeeLexer_WideFormatFilename(self,filename); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN struct DeeDeemonFormatConfig const *config) { return DeeLexer_Utf8FormatFilenameEx(self,filename,config); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_RET_EXCEPT(-1) int) DeeLexer_FormatFilenameEx(DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN struct DeeDeemonFormatConfig const *config) { return DeeLexer_WideFormatFilenameEx(self,filename,config); }
#else
#define DeeLexer_AddSysIncludePath  DeeLexer_Utf8AddSysIncludePath
#define _DeeLexer_AddSysIncludePath _DeeLexer_Utf8AddSysIncludePath
#define DeeLexer_IncludeFilename    DeeLexer_Utf8IncludeFilename
#define DeeLexer_IncludeFilenameEx  DeeLexer_Utf8IncludeFilenameEx
#define DeeLexer_FormatFilename     DeeLexer_Utf8FormatFilename
#define DeeLexer_FormatFilenameEx   DeeLexer_Utf8FormatFilenameEx
#endif


#endif /* !GUARD_DEEMON_LEXER_H */
