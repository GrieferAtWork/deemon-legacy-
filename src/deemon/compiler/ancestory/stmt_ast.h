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
#ifndef GUARD_DEEMON_STMT_AST_H
#define GUARD_DEEMON_STMT_AST_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#ifndef DEE_LIMITED_API
#error "Limited-only header"
#endif

#include <deemon/__conf.inl>
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/object.h>
#include <deemon/compiler/code.h>
#include <deemon/compiler/lexer.h>
#include <deemon/runtime/extern.h>
#include <deemon/guid.h>
#include <deemon/optional/string_forward.h>

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeStmtAstObject);

typedef enum DeeStmtAstKind {
 DeeStmtAstKind_EMPTY      = 0x00000000, // ';'
 DeeStmtAstKind_STMTS      = 0x00000001, // '{' <statement> ';' [<statement> ';'...] '}'
 DeeStmtAstKind_EXPR       = 0x00000002, // <expr> ';'
 DeeStmtAstKind_RETURN     = 0x00000003, // 'return' [<expr> [',' <expr>...]] ';'
 DeeStmtAstKind_YIELD      = 0x00000004, // 'yield' <expr> [',' <expr>...] ';'
 DeeStmtAstKind_DEL        = 0x00000005, // 'del' <expr> ';'
 DeeStmtAstKind_BREAK      = 0x00000006, // 'break' ';'
 DeeStmtAstKind_CONTINUE   = 0x00000007, // 'continue' ';'
 DeeStmtAstKind_ASSERT     = 0x00000008, // 'assert' <expr> [',' <expr>] ';'
 DeeStmtAstKind_PRINT      = 0x00000009, // 'print' [<expr> [',' <expr>...]] ';'
 DeeStmtAstKind_PRINT_NOLF = 0x0000000A, // 'print' [<expr> [',' <expr>...]] ',' ';'
 DeeStmtAstKind_IF         = 0x0000000B, // 'if' '(' <expr> ')' <statement> ['else' <statement>]
 DeeStmtAstKind_FOR        = 0x0000000C, // 'for' '(' [<expr>] ';' [<expr>] ';' [<expr>] ')' <statement>
 DeeStmtAstKind_FOR_IN     = 0x0000000D, // 'for' '(' <keyword> ':' <expr> ')' <statement>
 DeeStmtAstKind_FOREACH    = 0x0000000E, // '__foreach' '(' <keyword> ':' <expr> ')' <statement>
 DeeStmtAstKind_WHILE      = 0x0000000F, // 'while' '(' <expr> ')' <statement>
 DeeStmtAstKind_DO_WHILE   = 0x00000010, // 'do' <statement> 'while' '(' <expr> ')' ';'
 DeeStmtAstKind_GOTO       = 0x00000011, // 'goto' <keyword> ';'
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DeeStmtAstKind_THROW      = 0x00000012, // 'throw' <expr> ';'
 DeeStmtAstKind_RETHROW    = 0x00000013, // 'throw' ';'
 DeeStmtAstKind_TRY        = 0x00000014, // 'try' <stmt> [('catch' '(' '...' ')' <stmt>, 'finally' <stmt>)]...
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 DeeStmtAstKind_EXTERN     = 0x00000015, // 'extern' <string> ':' <cexpr> <keyword> '(' [<cexpr> [<keyword>] [',' <cexpr> [<keyword>]]...] ')' ';'
 DeeStmtAstKind_MODULE     = 0x00000016, // 'module' <keyword> <stmt>
 DeeStmtAstKind_SEXPR      = 0x00000017, // [internal] Expression with custom scope
 DeeStmtAstKind_SSTMT      = 0x00000018, // [internal] Statement with custom scope
 DeeStmtAstKind_INL_STMTS  = 0x00000019, // [internal] Inlined list of statements (use parenting scope)
 DeeStmtAstKind_LOOP_ONCE  = 0x0000001A, // [internal] A loop that is only executed once ('do { ... } while(0)')
 DeeStmtAstKind_SWITCH     = 0x0000001B, // 'switch' '(' <expr> ')' <stmt>
} DeeStmtAstKind;


#ifdef DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
#undef DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
#endif

struct DeeExprAstObject;
struct DeeTokenObject;
struct DeeLocalVarObject;
struct DeeScopeObject;
struct DeeStmtAstObject;
struct DeeModuleObject;

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
struct DeeStmtAstTryHandler {
 DeeExceptionHandlerKind             ob_kind;  /*< The kind of exception handler this is. */
 DEE_A_REF struct DeeLocalVarObject *ob_local; /*< [0..1] The local variable we use to store the exception (only if a variable is used). */
 DEE_A_REF DeeTypeObject            *ob_type;  /*< [0..1] The type of exception handled (only used by typed catch). */
 DEE_A_REF struct DeeScopeObject    *ob_scope; /*< [1..1] Scope of the handler code. */
 DEE_A_REF struct DeeStmtAstObject  *ob_block; /*< [1..1] Handler code block. */
 /* The following are only used during compilation
    to reference the runtime handler entry. */
 Dee_uint16_t                        ob_runtime_id;
 Dee_size_t                          ob_runtime_jmp;
};
#define DeeStmtAstTryHandler_QUIT(ob)\
do{\
 Dee_XDECREF((ob)->ob_local);\
 Dee_XDECREF((ob)->ob_type);\
 Dee_DECREF((ob)->ob_scope);\
 Dee_DECREF((ob)->ob_block);\
}while(0)
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeStmtAstObject {
 DEE_OBJECT_HEAD
 DEE_A_REF struct DeeTokenObject      *ob_token; /*< [1..1] Main token of this expression (for output) */
 DeeStmtAstKind                        ob_kind;  /*< The kind of expression. */
 struct DeeLabelRefList                ob_label_endpoints; /*< List of labels ending at this statement. */
 union{
  struct { // DeeStmtAstKind_STMTS
   DEE_A_REF struct DeeScopeObject    *ob_stmts_scope; /*< [1..1] Scope of the statements. */
   Dee_size_t                          ob_stmts_c;     /*< Amount of statements. */
   DEE_A_REF struct DeeStmtAstObject **ob_stmts_v;     /*< [1..1][ob_stmts_c:1..1] List of statements. */
  } ob_stmts;
  struct { // DeeStmtAstKind_INL_STMTS
   Dee_size_t                          ob_inl_stmts_c; /*< Amount of statements. */
   DEE_A_REF struct DeeStmtAstObject **ob_inl_stmts_v; /*< [1..1][ob_stmts_c:1..1] List of statements. */
  } ob_inl_stmts;
  struct { // DeeStmtAstKind_EXPR
   DEE_A_REF struct DeeExprAstObject  *ob_expr_ast; /*< [1..1] Expression. */
  } ob_expr;
  struct { // DeeStmtAstKind_SSTMT
   DEE_A_REF struct DeeScopeObject    *ob_sstmt_scope; /*< [1..1] Scope of the statement. */
   DEE_A_REF struct DeeStmtAstObject  *ob_sstmt_ast;   /*< [1..1] Statement. */
  } ob_sstmt;
  struct { // DeeStmtAstKind_SEXPR
   DEE_A_REF struct DeeScopeObject    *ob_sexpr_scope; /*< [1..1] Scope of the expression. */
   DEE_A_REF struct DeeExprAstObject  *ob_sexpr_ast;   /*< [1..1] Expression. */
  } ob_sexpr;
  struct { // DeeStmtAstKind_RETURN
   DEE_A_REF struct DeeExprAstObject  *ob_ret_expr; /*< [0..1] Value to return (may be a tuple expression). */
  } ob_return;
  struct { // DeeStmtAstKind_YIELD
   DEE_A_REF struct DeeTupleObject    *ob_yld_exprs; /*< [1..1] Tuple of expression to yield individually. */
  } ob_yield;
  struct { // DeeStmtAstKind_DEL
   DEE_A_REF struct DeeTupleObject    *ob_del_exprs; /*< [1..1] Tuple of delete expression (tp_attr_get, tp_seq_get, tp_seq_range_get). */
  } ob_del;
  struct { // DeeStmtAstKind_ASSERT
   DEE_A_REF struct DeeExprAstObject  *ob_asrt_expr; /*< [1..1] Expression to assert. */
   DEE_A_REF struct DeeExprAstObject  *ob_asrt_msg;  /*< [0..1] Message expression for errors. */
  } ob_assert;
  struct { // DeeStmtAstKind_PRINT
   DEE_A_REF struct DeeExprAstObject  *ob_prt_file; /*< [0..1] File to print to. */
   DEE_A_REF struct DeeExprAstObject  *ob_prt_tpl;  /*< [0..1] Tuple expression. */
  } ob_print;
  struct { // DeeStmtAstKind_IF
   DEE_A_REF struct DeeExprAstObject  *ob_if_expr;      /*< [1..1] If expression. */
   DEE_A_REF struct DeeStmtAstObject  *ob_if_thenstmt;  /*< [1..1] Then statement. */
   DEE_A_REF struct DeeScopeObject    *ob_if_thenscope; /*< [1..1] Then scope. */
   DEE_A_REF struct DeeStmtAstObject  *ob_if_elsestmt;  /*< [0..1] Else statement. */
   DEE_A_REF struct DeeScopeObject    *ob_if_elsescope; /*< [0..1] Else scope. */
  } ob_if;
  struct { // DeeStmtAstKind_FOR
   DEE_A_REF struct DeeScopeObject    *ob_f_scope;    /*< [1..1] Scope of the loop. */
   DEE_A_REF struct DeeExprAstObject  *ob_f_forinit;  /*< [0..1] For initializer. */
   DEE_A_REF struct DeeExprAstObject  *ob_f_forcont;  /*< [0..1] For done expression. */
   DEE_A_REF struct DeeExprAstObject  *ob_f_fornext;  /*< [0..1] For next expression. */
   DEE_A_REF struct DeeStmtAstObject  *ob_f_block;    /*< [1..1] Block executed during for loop. */
  } ob_for;
  struct { // DeeStmtAstKind_FOR_IN|DeeStmtAstKind_FOREACH
   DEE_A_REF struct DeeScopeObject    *ob_fi_scope;    /*< [1..1] Scope of the loop. */
   DEE_A_REF struct DeeLocalVarObject *ob_fi_itername; /*< [1..1] Name of the local to iterate with. */
   DEE_A_REF struct DeeExprAstObject  *ob_fi_iterlist; /*< [1..1] Expression to loop over. */
   DEE_A_REF struct DeeStmtAstObject  *ob_fi_block;    /*< [1..1] Block executed during for loop. */
  } ob_for_in;
  struct { // DeeStmtAstKind_WHILE
   DEE_A_REF struct DeeScopeObject    *ob_w_scope;    /*< [1..1] Scope of the loop. */
   DEE_A_REF struct DeeExprAstObject  *ob_w_loopcond; /*< [1..1] Loop condition. */
   DEE_A_REF struct DeeStmtAstObject  *ob_w_block;    /*< [1..1] Block executed during for loop. */
  } ob_while;
  struct { // DeeStmtAstKind_DO_WHILE
   // NOTE: The first field in here must have the same meaning as in 'DeeStmtAstKind_LOOP_ONCE'
   DEE_A_REF struct DeeScopeObject    *ob_dw_scope;    /*< [1..1] Scope of the loop. */
   DEE_A_REF struct DeeStmtAstObject  *ob_dw_block;    /*< [1..1] Block executed during for loop. */
   DEE_A_REF struct DeeExprAstObject  *ob_dw_loopcond; /*< [1..1] Loop condition. */
  } ob_do_while;
  struct { // DeeStmtAstKind_LOOP_ONCE
   DEE_A_REF struct DeeScopeObject    *ob_lonce_scope; /*< [1..1] Scope of the loop. */
   DEE_A_REF struct DeeStmtAstObject  *ob_lonce_block; /*< [1..1] Statements to loop. */
  } ob_loop_once;
  struct { // DeeStmtAstKind_GOTO
   Dee_size_t ob_gt_label_id; /*< [1..1] Label id to jump to. */
  } ob_goto;
  struct { // DeeStmtAstKind_THROW
   DEE_A_REF struct DeeExprAstObject  *ob_trw_expr; /*< [1..1] Expression that will be thrown. */
  } ob_throw;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  struct { // DeeStmtAstKind_TRY
   DEE_A_REF struct DeeScopeObject    *ob_try_scope;    /*< [1..1] Scope of the protected code. */
   DEE_A_REF struct DeeStmtAstObject  *ob_try_block;    /*< [1..1] Block of protected code. */
   Dee_size_t                          ob_try_handlerc; /*< Amount of handler blocks. */
   struct DeeStmtAstTryHandler        *ob_try_handlerv; /*< [0..ob_try_handlerc][owned] Handler blocks. */
  } ob_try;
#endif
  struct {  // DeeStmtAstKind_EXTERN
   struct DeeExternFuncDecl            ob_extern_decl;
   DEE_A_REF struct DeeLocalVarObject *ob_extern_store; /*< [1..1] Variable to store the decl under. */
  } ob_extern;
  struct { // DeeStmtAstKind_MODULE
   DEE_A_REF struct DeeModuleObject   *ob_mod_module; /*< [1..1] Module object. */
   DEE_A_REF struct DeeStmtAstObject  *ob_mod_block;  /*< [1..1] Block executed by the module. */
  } ob_module;
  struct { // DeeStmtAstKind_SWITCH
   DEE_A_REF struct DeeExprAstObject  *ob_sw_expr;  /*< [1..1] Switch Expression. */
   DEE_A_REF struct DeeStmtAstObject  *ob_sw_stmt;  /*< [1..1] Switch Statement. */
   DEE_A_REF struct DeeScopeObject    *ob_sw_scope; /*< [1..1] Switch scope. */
   struct DeeCaseLabelDeclList         ob_sw_cases; /*< Switch cases. */
  } ob_switch;
 };
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeeStmtAst_KIND(ob) ((DeeStmtAstObject *)Dee_REQUIRES_POINTER(ob))->ob_kind

extern DeeTypeObject DeeStmtAst_Type;
#define DeeStmtAst_Check(ob)      DeeObject_InstanceOf(ob,&DeeStmtAst_Type)
#define DeeStmtAst_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeStmtAst_Type)


//////////////////////////////////////////////////////////////////////////
// Unsafe, because we don't fill in members as NULL, because we can't, because the spec says so
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *
_DeeStmtAst_NewUnsafe(DEE_A_IN_OBJECT(DeeTokenObject) *token_);
#define _DeeStmtAst_Free(ob)\
do{\
 Dee_DECREF(((DeeStmtAstObject *)(ob))->ob_token);\
 DeeLabelRefList_Quit(&((DeeStmtAstObject *)(ob))->ob_label_endpoints);\
 _DeeObject_Delete((DeeObject *)(ob));\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Clears the pool of cached statement ast objects
extern Dee_size_t DeeStmtAst_ClearPool(void);


extern DEE_A_RET_WUNUSED DeeTypeObject *DeeStmtAst_PredictType(DEE_A_IN_OBJECT(DeeStmtAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope);
extern DEE_A_RET_WUNUSED int DeeStmtAst_IsNoexcept(DEE_A_IN_OBJECT(DeeStmtAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope);
#define DEE_COMPILER_IS_NO_RETURN_FLAG_NONE                 0x00000000
#define DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE 0x00000001
#define DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS     0x00000002
extern DEE_A_RET_WUNUSED int DeeStmtAst_IsNoreturn(DEE_A_IN_OBJECT(DeeStmtAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope, DEE_A_IN Dee_uint32_t flags);
extern DEE_A_RET_WUNUSED int DeeStmtAst_IsNoeffect(DEE_A_IN_OBJECT(DeeStmtAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope);


extern DEE_A_RET_EXCEPT(-1) int DeeStmtAst_Optimize(
 DEE_A_INOUT_OBJECT(DeeStmtAstObject) *self,
 DEE_A_INOUT_OBJECT(DeeScopeObject) *scope, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Compiles a statement ast
//  - Leaves the writer with +1 stack entries (if DEE_COMPILER_FLAG_USED is set)
//  - scope should be the scope set in the statement AST where this expression is executed from
//  - Returns -1 on error
//  - Returns 1 if the code generated is noreturn
struct DeeCodeWriter;
extern DEE_A_RET_EXCEPT(-1) int DeeStmtAst_Compile(
 DEE_A_IN_OBJECT(DeeStmtAstObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCodeWriter *writer,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN Dee_uint32_t flags);


//////////////////////////////////////////////////////////////////////////
// Parses a <statement> (as defined in 'input.c'; includes the ';' at the end)
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatement(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatementNewScope(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatementOldScope(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags);

//////////////////////////////////////////////////////////////////////////
// Parses a statement list until 'end_token' is encountered
// >> 'end_token' is usually '}' or '\0', but can be anything
// NOTE: Don't set ';' as end_token! if you feel like you should,
//       you don't understand what this function is really for!
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatements(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags, TPPTokenID end_token);

#ifdef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#undef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#endif

struct DeeAttributeDecl {
 DEE_A_REF struct DeeStringObject  *ob_name;      /*< [0..1] Name override. */
 DEE_A_REF struct DeeStringObject  *ob_depr;      /*< [0..1] Deprecation reason. */
 DEE_A_REF struct DeeExprAstObject *ob_super;     /*< [0..1] Super class type. */
 DeeFunctionFlags                   ob_fun_flags; /*< Function flags (used in extern functions). */
#define DEE_ATTRIBUTE_DECL_FLAGS_NONE     0x0000
#define DEE_ATTRIBUTE_DECL_FLAGS_GUID     0x0001 /*< GUID has been set. */
#define DEE_ATTRIBUTE_DECL_FLAGS_COPYABLE 0x0002 /*< __attribute__((__copyable__)). */
 Dee_uint16_t                       ob_flags;     /*< Function flags (used in extern functions). */
 Dee_size_t                         ob_alignment; /*< Manual data alignment. */
 struct DeeGUID                     ob_guid;      /*< GUID attribute. */
};
#define DeeAttributeDecl_INIT() \
 {NULL,NULL,NULL,DeeFunctionFlags_DEFAULT,DEE_ATTRIBUTE_DECL_FLAGS_NONE,0,DeeGUID_INIT()}
DEE_STATIC_INLINE(void) DeeAttributeDecl_Quit(struct DeeAttributeDecl *self) {
 Dee_XDECREF(self->ob_depr);
 Dee_XDECREF(self->ob_super);
 Dee_XDECREF(self->ob_name);
}

//extern int _DeeLexer_ParseCatchOrFinally(struct DeeStmtAstTryHandler *handler, struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern int _DeeLexer_ParseAttributeDecl(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags);
extern int _DeeLexer_ParseAttributeNothrowDecl(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags);

struct DeeExprAstObject;
struct DeeForeignFunctionTypeObject;
struct _DeeTokenIDListObject;
// Parses the code block for a foreign closure function,
// starting directly after the argument list has been parsed
// (usually at the '{' or '->')
// >> The function should then return the ast containing closure_function decl
// NOTE: The caller is responsible for setting self->ob_func_name
extern struct DeeExprAstObject *_DeeLexer_ParseForeignClosureCodeBlock(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags,
 struct DeeAttributeDecl *attr, struct DeeForeignFunctionTypeObject *type,
 struct _DeeTokenIDListObject *argnames, DEE_A_IN_OPT struct DeeTokenObject *name_token);
extern int _DeeLexer_ParseConstDecl(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern int _DeeStmtAst_OptimizeFor(DeeStmtAstObject *self);
extern int _DeeStmtAst_OptimizeForIn(DeeStmtAstObject *self, Dee_uint32_t flags);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
extern int _DeeStmtAst_OptimizeTry(DeeStmtAstObject *self);
#endif

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_STMT_AST_H */
