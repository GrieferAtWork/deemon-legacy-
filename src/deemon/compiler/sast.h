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
#ifndef GUARD_DEEMON_SAST_H
#define GUARD_DEEMON_SAST_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#ifndef DEE_LIMITED_API
#error "Limited-only header"
#endif

#include <deemon/__conf.inl>
#include <deemon/compiler/code.h>
#include <deemon/compiler/codewriter.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/xast.h>
#include <deemon/optional/uuid.h>
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#include <deemon/type.h>

//////////////////////////////////////////////////////////////////////////
// SAst -- v102+ Statement Ast replacement
//
//   - This time, let's do things right (Griefer@Work, 2016)
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#define DEE_SASTKIND_NONE        0
#define DEE_SASTKIND_EMPTY       0 /*< ; */
#define DEE_SASTKIND_EXPRESSION  1 /*< 10+32; */
#define DEE_SASTKIND_BLOCK       2 /*< { ... } */
#define DEE_SASTKIND_RETURN      3 /*< return 42; */
#define DEE_SASTKIND_YIELD       4 /*< yield x+y,20; */
#define DEE_SASTKIND_ASSERT      5 /*< assert y = 64: "$H17"; */
#define DEE_SASTKIND_PRINT       6 /*< print out: "value =",k; */
#define DEE_SASTKIND_IF          7 /*< if (foo()) ...; */
#define DEE_SASTKIND_FOR         8 /*< for (i = 0; i < 20; ++i) ...; */
#define DEE_SASTKIND_FORIN       9 /*< for (x: util::range(20)) ...; */
#define DEE_SASTKIND_FOREACH    10 /*< __foreach (x: iter) ...; */
#define DEE_SASTKIND_WHILE      11 /*< while (foo()) bar(); */
#define DEE_SASTKIND_DOWHILE    12 /*< do bar(); while (foo()); */
#define DEE_SASTKIND_LOOPEVER   13 /*< __loopever bar(); */
#define DEE_SASTKIND_LOOPONCE   14 /*< __looponce bar(); */
#define DEE_SASTKIND_LOOPNONE   15 /*< __loopnone bar(); */
#define DEE_SASTKIND_CONTINUE   16 /*< continue; */
#define DEE_SASTKIND_BREAK      17 /*< break; */
#define DEE_SASTKIND_GOTO       18 /*< goto label_2; */
#define DEE_SASTKIND_THROW      19 /*< throw "foobar"; (Also used for rethrow) */
#define DEE_SASTKIND_TRY        20 /*< try throw "foobar"; catch (e...) print e; */
#define DEE_SASTKIND_MODULE     21 /*< module mod1 { const C = 42; } */
#define DEE_SASTKIND_SWITCH     22 /*< switch (x) { case 0: print 0; default: break; } */
#define DEE_SASTKIND_IFTRUE     23 /*< __if_true { ...; } else { ...; } */
#define DEE_SASTKIND_IFFALSE    24 /*< __if_false { ...; } else { ...; } */



#ifdef DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
#undef DEE_PRIVATE_DECL_DEE_EXCEPTION_HANDLER_KIND
#endif
struct DeeSAstTryHandler {
 DeeExceptionHandlerKind            th_kind;  /*< The kind of exception handler this is. */
 DEE_A_REF struct DeeVarDeclStorage th_local; /*< The local variable we use to store the exception (only if a variable is used). */
 DEE_A_REF struct DeeTypeObject    *th_type;  /*< [0..1] The type of exception handled (only used by typed catch). */
 DEE_A_REF struct DeeScopeObject   *th_scope; /*< [1..1] Scope of the handler code. */
 DEE_A_REF struct DeeSAstObject    *th_block; /*< [1..1] Handler code block. */
 /* The following are only used during compilation
    to reference the runtime handler entry. */
 Dee_size_t                         th_runtime_id;
 Dee_size_t                         th_runtime_jmp;
};
#define _DeeSAstTryHandler_Quit(ob)\
do{\
 _DeeVarDeclStorage_Quit(&(ob)->th_local);\
 Dee_XDECREF((ob)->th_type);\
 Dee_DECREF((ob)->th_scope);\
 Dee_DECREF((ob)->th_block);\
}while(0)


//////////////////////////////////////////////////////////////////////////
// Returns '1' if 'self' was initialized as a reference copy
// in "sast.copy_with_scopy.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeSAstTryHandler_InitCopyWithScope(
 DEE_A_OUT struct DeeSAstTryHandler *self, DEE_A_IN struct DeeSAstTryHandler const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT DeeLexerObject *lexer);


#define DEE_SAST_AST_HEAD \
           DeeSAstKind        ast_kind;   /*< The kind of AST. */\
 DEE_A_REF DeeTokenObject    *ast_token;  /*< [1..1] token of this AST (used in debug/error output) */\
 struct DeeParserLabelRefList ast_labels; /*< List of label end points. */
struct DeeSAstCommonAst { DEE_SAST_AST_HEAD };
#ifdef __INTELLISENSE__
// So I don't have to scroll all the time...
#undef DEE_SAST_AST_HEAD
#define DEE_SAST_AST_HEAD /* nothing */
#endif

struct DeeXAstObject;
struct DeeSAstExpressionAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject *x_ast;   /*< [1..1] Expression ast. */
};
#define _DeeSAstExpressionAst_Quit(ob)\
do{ Dee_DECREF((ob)->x_ast); }while(0)

struct DeeSAstBlockAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *b_scope; /*< [1..1] Scope used by the statements in this block. */
           Dee_size_t             b_astc;  /*< Amount of statements in this block. */
 DEE_A_REF struct DeeSAstObject **b_astv;  /*< [1..1][owned][1..b_astc][owned] Statements in this block. */
};
struct DeeSAstReturnAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject *r_value; /*< [0..1] Return expression. */
};
struct DeeSAstYieldAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject *y_value; /*< [1..1] Yield expression. */
};
struct DeeSAstAssertAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject *a_check;   /*< [1..1] Expressions that should be asserted. */
 DEE_A_REF struct DeeXAstObject *a_message; /*< [0..1] Message printed if the assertion fails. */
};
struct DeeSAstPrintAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject  *p_file;  /*< [0..1] Output file to stream data into (set to NULL for default - 'file.io.stdout'). */
 DEE_A_REF struct DeeXAstObject  *p_data;  /*< [0..1] Data that will be printed (Must be a tuple AST). */
#define DEE_SASTPRINT_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_SASTPRINT_FLAG_NOLF   DEE_UINT32_C(0x00000001) /*< Don't append a linefeed. */
#define DEE_SASTPRINT_FLAG_NOSEP  DEE_UINT32_C(0x00000002) /*< Don't insert space characters in-between written objects (No way of accessing from usercode right now). */
#define DEE_SASTPRINT_FLAG_BUFFER DEE_UINT32_C(0x00000004) /*< Buffer all objects before writing them all at once (aka. v100-style printing) (No way of accessing from usercode right now). */
           Dee_uint32_t           p_flags; /*< Flags applied when streaming data. */
};
struct DeeSAstIfAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject  *if_cond_scope; /*< [1..1] Scope of the if condition (parent to 'i_succ_scope' and 'i_fail_scope'). */
 DEE_A_REF struct DeeXAstObject   *if_cond;       /*< [1..1] If-condition. */
 DEE_A_REF struct DeeScopeObject  *if_succ_scope; /*< [0..1] Scope for 'i_succ_block'. */
 DEE_A_REF struct DeeSAstObject   *if_succ_block; /*< [0..1] Block executed if 'i_cond' succeeds. */
 DEE_A_REF struct DeeScopeObject  *if_fail_scope; /*< [0..1] Scope for 'i_fail_block'. */
 DEE_A_REF struct DeeSAstObject   *if_fail_block; /*< [0..1] Block executed if 'i_cond' fails. */
};
struct DeeSAstForAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *f_head_init_scope; /*< [1..1] Scope used for 'f_head_init'. */
 DEE_A_REF struct DeeXAstObject  *f_head_init;       /*< [0..1] Loop initializer: 'for (f_head_init;;)'. */
 DEE_A_REF struct DeeScopeObject *f_head_cond_scope; /*< [1..1] Scope used for 'f_head_cond' and 'f_head_next'. */
 DEE_A_REF struct DeeXAstObject  *f_head_cond;       /*< [1..1] Loop condition:   'for (;f_head_cond;)'. */
 DEE_A_REF struct DeeScopeObject *f_scope;           /*< [1..1] Scope in which 'f_block' and 'f_head_next' is executed. */
 DEE_A_REF struct DeeXAstObject  *f_head_next;       /*< [0..1] Loop advance:     'for (;;f_head_next)'. */
 DEE_A_REF struct DeeSAstObject  *f_block;           /*< [1..1] Block executed each time the loop iterates. */
};
struct DeeSAstForInAst {
 DEE_SAST_AST_HEAD
           struct DeeVarDeclStorage fi_var;       /*< Variable to store elements that get yielded inside of (executed in 'fi_block'). */
 DEE_A_REF struct DeeScopeObject   *fi_seq_scope; /*< [1..1] Scope for 'fi_seq' (parent to 'fi_scope'). */
 DEE_A_REF struct DeeXAstObject    *fi_seq;       /*< [1..1] Sequence to iterate. */
 DEE_A_REF struct DeeScopeObject   *fi_scope;     /*< [1..1] Scope in which 'fi_block' is executed. */
 DEE_A_REF struct DeeSAstObject    *fi_block;     /*< [1..1] Block executed each time the loop iterates. */
};
struct DeeSAstWhileAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *w_cond_scope; /*< [1..1] Scope for 'w_cond' (parent to 'w_scope'). */
 DEE_A_REF struct DeeXAstObject  *w_cond;       /*< [1..1] Condition of the loop. */
 DEE_A_REF struct DeeScopeObject *w_scope;      /*< [1..1] Scope in which 'w_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *w_block;      /*< [1..1] Block executed each time the loop iterates. */
};
struct DeeSAstSpecialLoopAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *sl_scope; /*< [1..1] Scope in which 'sl_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *sl_block; /*< [1..1] Block that will be executed. */
};
struct DeeSAstGotoAst {
 DEE_SAST_AST_HEAD
 struct DeeParserLabel *g_label; /*< [1..1] Label destination. */
};
struct DeeSAstThrowAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeXAstObject *t_object; /*< [0..1] Expression that will be thrown, or NULL for rethrow. */
};
struct DeeSAstTryAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *t_rootscope; /*< [1..1] Root scope (common parent of the block & handler scopes. - Exists to have a single point of reference for all scopes in a try block). */
 DEE_A_REF struct DeeScopeObject *t_scope;     /*< [1..1] Scope in which 't_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *t_block;     /*< [1..1] Block that this try-block is protecting. */
 Dee_size_t                       t_handlerc;  /*< Amount of catch/finall handlers. */
 // TODO: Change the rules for this from '0..t_handlerc' to '1..t_handlerc'
 //       -> 0 should be an 'DEE_SASTKIND_EMPTY' Ast
 // NOTE: 't_handlerc' may be 0 before 'DeeSAst_FinalizeTry' was called
 struct DeeSAstTryHandler        *t_handlerv;  /*< [1..t_handlerc][owned] List of catch/finall handlers. */
};
struct DeeSAstModuleAst {
#define DeeSAstModuleAst_NAME(ob) ((ob)->ast_token->tk_token.tk_id)
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *m_module; /*< [1..1] Module scope. */
 DEE_A_REF struct DeeSAstObject  *m_block;  /*< [1..1] Block executed to initialize the module. */
};
struct DeeSAstSwitchAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *sw_expr_scope; /*< [1..1] Scope of the switch expression (parent to 'sw_scope'). */
 DEE_A_REF struct DeeXAstObject  *sw_expr;       /*< [1..1] Switch Expression. */
 DEE_A_REF struct DeeScopeObject *sw_scope;      /*< [1..1] Scope in which 'sw_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *sw_block;      /*< [1..1] Switch Statement. */
 struct DeeParserLabelList        sw_cases;      /*< Switch case labels. */
};
struct DeeSAstIfConstAst {
 DEE_SAST_AST_HEAD
 DEE_A_REF struct DeeScopeObject *ic_succ_scope; /*< [0..1] Scope in which 'ic_succ_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *ic_succ_block; /*< [0..1] Live statements. */
 DEE_A_REF struct DeeScopeObject *ic_fail_scope; /*< [0..1] Scope in which 'ic_fail_block' is executed. */
 DEE_A_REF struct DeeSAstObject  *ic_fail_block; /*< [0..1] Dead statements. */
};
#undef DEE_SAST_AST_HEAD


DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeSAstObject {
#define DeeSAst_UNCOMMON_SIZE \
 (sizeof(DeeSAstObject)-(sizeof(DeeObject)+sizeof(struct DeeSAstCommonAst)))
#define DeeSAst_UNCOMMON_DATA(ob)\
 (void *)(((uintptr_t)(ob))+(sizeof(DeeObject)+sizeof(struct DeeSAstCommonAst)))
 DEE_OBJECT_HEAD
 union{
  DeeSAstKind                  ast_kind;        /*< The kind of AST. */
  struct DeeSAstCommonAst      ast_common;      /*< DEE_SASTKIND_*. */
  struct DeeSAstCommonAst      ast_empty;       /*< DEE_SASTKIND_EMPTY. */
  struct DeeSAstExpressionAst  ast_expression;  /*< DEE_SASTKIND_EXPRESSION. */
  struct DeeSAstBlockAst       ast_block;       /*< DEE_SASTKIND_BLOCK. */
  struct DeeSAstReturnAst      ast_return;      /*< DEE_SASTKIND_RETURN. */
  struct DeeSAstYieldAst       ast_yield;       /*< DEE_SASTKIND_YIELD. */
  struct DeeSAstAssertAst      ast_assert;      /*< DEE_SASTKIND_ASSERT. */
  struct DeeSAstPrintAst       ast_print;       /*< DEE_SASTKIND_PRINT. */
  struct DeeSAstIfAst          ast_if;          /*< DEE_SASTKIND_IF. */
  struct DeeSAstForAst         ast_for;         /*< DEE_SASTKIND_FOR. */
  struct DeeSAstForInAst       ast_forin;       /*< DEE_SASTKIND_FORIN. */
  struct DeeSAstForInAst       ast_foreach;     /*< DEE_SASTKIND_FOREACH. */
  struct DeeSAstWhileAst       ast_while;       /*< DEE_SASTKIND_WHILE. */
  struct DeeSAstWhileAst       ast_dowhile;     /*< DEE_SASTKIND_DOWHILE. */
  struct DeeSAstSpecialLoopAst ast_specialloop; /*< DEE_SASTKIND_LOOPONCE, DEE_SASTKIND_LOOPEVER, DEE_SASTKIND_LOOPNONE. */
  struct DeeSAstSpecialLoopAst ast_loopever;    /*< DEE_SASTKIND_LOOPEVER. */
  struct DeeSAstSpecialLoopAst ast_looponce;    /*< DEE_SASTKIND_LOOPONCE. */
  struct DeeSAstSpecialLoopAst ast_loopnone;    /*< DEE_SASTKIND_LOOPNONE. */
  struct DeeSAstCommonAst      ast_continue;    /*< DEE_SASTKIND_CONTINUE. */
  struct DeeSAstCommonAst      ast_break;       /*< DEE_SASTKIND_BREAK. */
  struct DeeSAstGotoAst        ast_goto;        /*< DEE_SASTKIND_GOTO. */
  struct DeeSAstThrowAst       ast_throw;       /*< DEE_SASTKIND_THROW. */
  struct DeeSAstTryAst         ast_try;         /*< DEE_SASTKIND_TRY. */
  struct DeeSAstModuleAst      ast_module;      /*< DEE_SASTKIND_MODULE. */
  struct DeeSAstSwitchAst      ast_switch;      /*< DEE_SASTKIND_SWITCH. */
  struct DeeSAstIfConstAst     ast_ifconst;     /*< DEE_SASTKIND_IFTRUE, DEE_SASTKIND_IFFALSE. */
  struct DeeSAstIfConstAst     ast_iftrue;      /*< DEE_SASTKIND_IFTRUE. */
  struct DeeSAstIfConstAst     ast_iffalse;     /*< DEE_SASTKIND_IFFALSE. */
 };
};
DEE_COMPILER_MSVC_WARNING_POP

#define DeeSAst_IsEmpty(ob) \
((ob)->ast_kind == DEE_SASTKIND_EMPTY &&\
 (ob)->ast_common.ast_labels.ob_c == 0)


extern DeeTypeObject DeeSAst_Type;
#define DeeSAst_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSAst_Type)
#define DeeSAst_Check          DeeSAst_CheckExact


//////////////////////////////////////////////////////////////////////////
// NOTE: The return ast will be move-constructed from 'labels_'
extern DEE_A_RET_WUNUSED Dee_size_t DeeSAst_ClearPool(void);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewUnsafe(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_);
#define _DeeSAst_FreeUnsafe(ob)\
do{\
 Dee_DECREF((ob)->ast_common.ast_token);\
 _DeeParserLabelRefList_Quit(&(ob)->ast_common.ast_labels);\
 _DeeObject_DELETE(&DeeSAst_Type,(DeeObject *)(ob));\
}while(0)


//////////////////////////////////////////////////////////////////////////
// Creates a new empty SAst
#define DeeSAst_NewEmpty(tk,labels_)    _DeeSAst_NewEmptyEx(DEE_SASTKIND_EMPTY,tk,labels_)
#define DeeSAst_NewContinue(tk,labels_) _DeeSAst_NewEmptyEx(DEE_SASTKIND_CONTINUE,tk,labels_)
#define DeeSAst_NewBreak(tk,labels_)    _DeeSAst_NewEmptyEx(DEE_SASTKIND_BREAK,tk,labels_)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewEmptyEx(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_);

//////////////////////////////////////////////////////////////////////////
// Collects the top scopes used by 'self'
// NOTE: 'scope' must be scope that 'self' was parsed
//       inside of, or one of its weak parents.
// e.g.: 'if (x) { ... }' Returns one scope
//           ^ this one, because only the first (aka. top) scopes are collected
// in "sast.collect_top_scope_children.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_CollectTopScopeChildren(
 DEE_A_IN DeeSAstObject const *self, DEE_A_INOUT struct DeeScopeList *scopes);

//////////////////////////////////////////////////////////////////////////
// Visit all reachable asts
// Stop visiting a branch if the callback returned '!= 0'
// in "sast.collect_asts.inl"
extern int DeeSAst_VisitAll(
 DEE_A_IN DeeSAstObject *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure);

//////////////////////////////////////////////////////////////////////////
// Copies any given AST into the context of a new scope
// in "sast.copy_with_scope.inl"
// NOTE: 'lexer' is only used for warnings
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_CopyWithScope(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeScopeObject *new_scope,
 DEE_A_INOUT DeeLexerObject *lexer);

//////////////////////////////////////////////////////////////////////////
// Returns true/false if 'self' is equal to 'right'
// in "sast.equals.inl"
// NOTE: Tokens will be ignored during comparison
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSAst_Equals(
 DEE_A_IN DeeSAstObject const *self,
 DEE_A_IN DeeSAstObject const *right);

//////////////////////////////////////////////////////////////////////////
// Returns true/false if 'self' uses 'variable' in some way, shape or form
// in "sast.uses_variable.inl"
extern DEE_A_RET_WUNUSED int DeeSAst_UsesVariable(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);

extern int DEE_CALL _DeeSAst_HasLabelVisit(DEE_A_IN DeeSAstObject *ast, struct DeeParserLabel *lbl);
extern int DEE_CALL _DeeSAst_HasLabelsVisit(DEE_A_IN DeeSAstObject *ast, void *DEE_UNUSED(closure));
extern int DEE_CALL _DeeSAst_HasBreakContinueVisit(DEE_A_IN DeeSAstObject *ast, void *DEE_UNUSED(closure));
#define DeeSAst_HasLabel(ob,label) DeeSAst_VisitAll(ob,NULL,(DeeSAstVisitProc)&_DeeSAst_HasLabelVisit,label)
#define DeeXAst_HasLabel(ob,label) DeeXAst_VisitAll(ob,NULL,(DeeSAstVisitProc)&_DeeSAst_HasLabelVisit,label)
#define DeeSAst_HasLabels(ob) DeeSAst_VisitAll(ob,NULL,&_DeeSAst_HasLabelsVisit,NULL)
#define DeeXAst_HasLabels(ob) DeeXAst_VisitAll(ob,NULL,&_DeeSAst_HasLabelsVisit,NULL)
#define DeeSAst_HasBreakOrContinue(ob) DeeSAst_VisitAll(ob,NULL,&_DeeSAst_HasBreakContinueVisit,NULL)
#define DeeXAst_HasBreakOrContinue(ob) DeeXAst_VisitAll(ob,NULL,&_DeeSAst_HasBreakContinueVisit,NULL)

//////////////////////////////////////////////////////////////////////////
// Returns the combined effect that 'self' has on 'variable->ob_uses'
// NOTE: Even if 'DeeSAst_UsesVariable(self,variable)' returned true, this can return false
extern DEE_A_RET_WUNUSED Dee_size_t DeeSAst_CountVariableUses(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);

//////////////////////////////////////////////////////////////////////////
// Transferrs all label endpoints from 'from_' to 'to_'
#define DeeSAst_TransferAllLabels(from_,to_) \
 DeeParserLabelRefList_AppendMoveAll(\
  &(to_)->ast_common.ast_labels,\
  &(from_)->ast_common.ast_labels)

//////////////////////////////////////////////////////////////////////////
// Assigns different values to a given SAst
// in "sast.assign.inl"
extern void DeeSAst_AssignEmpty(DEE_A_INOUT DeeSAstObject *self);
extern void DeeSAst_InitMove(DEE_A_OUT DeeSAstObject *self, DEE_A_INOUT DeeSAstObject *right);
extern void DeeSAst_AssignMove(DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeSAstObject *right);


//////////////////////////////////////////////////////////////////////////
// Creates a new SAst containing an expression (XAst)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewExpression(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_expr);

//////////////////////////////////////////////////////////////////////////
// Create a new Block-SAst
// NOTE: The inherited version will only inherit its data if the function succeeds
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewBlock(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_IN Dee_size_t statementc,
 DEE_A_IN_R(statementc) DeeSAstObject *const *statementv);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewBlockFromInheritedStatements(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_IN Dee_size_t statementc,
 DEE_A_IN_R(statementc) DeeSAstObject **statementv);

//////////////////////////////////////////////////////////////////////////
// Create a new Return-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewReturn(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeXAstObject *return_expr);

//////////////////////////////////////////////////////////////////////////
// Create a new Yield-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewYield(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeXAstObject *yield_expr);

//////////////////////////////////////////////////////////////////////////
// Create a new Assertion-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewAssert(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *check_expr, DEE_A_INOUT_OPT DeeXAstObject *fail_msg);

//////////////////////////////////////////////////////////////////////////
// Create a new Print-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewPrint(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_IN Dee_uint32_t print_flags, DEE_A_INOUT_OPT DeeXAstObject *print_file,
 DEE_A_INOUT_OPT DeeXAstObject *print_data);

//////////////////////////////////////////////////////////////////////////
// Create a new If-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewIf(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *cond_scope, DEE_A_INOUT DeeXAstObject *ast_cond, 
 DEE_A_INOUT_OPT DeeScopeObject *succ_scope, DEE_A_INOUT_OPT DeeSAstObject *succ_block,
 DEE_A_INOUT_OPT DeeScopeObject *fail_scope, DEE_A_INOUT_OPT DeeSAstObject *fail_block);

//////////////////////////////////////////////////////////////////////////
// Create a new For-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewFor(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *head_init_scope, DEE_A_INOUT_OPT DeeXAstObject *head_init,
 DEE_A_INOUT DeeScopeObject *head_cond_scope, DEE_A_INOUT_OPT DeeXAstObject *head_cond,
 DEE_A_INOUT_OPT DeeXAstObject *head_next, DEE_A_INOUT DeeScopeObject *block_scope,
 DEE_A_INOUT DeeSAstObject *block);

//////////////////////////////////////////////////////////////////////////
// Create a new ForIn/Foreach-SAst
#define DeeSAst_NewForIn(tk,labels_,lexer,parser_flags,var,seq_scope,seq,block_scope,block) \
 _DeeSAst_NewForInOrForeach(DEE_SASTKIND_FORIN,tk,labels_,lexer,parser_flags,var,seq_scope,seq,block_scope,block)
#define DeeSAst_NewForeach(tk,labels_,lexer,parser_flags,var,seq_scope,seq,block_scope,block) \
 _DeeSAst_NewForInOrForeach(DEE_SASTKIND_FOREACH,tk,labels_,lexer,parser_flags,var,seq_scope,seq,block_scope,block)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewForInOrForeach(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *var,
 DEE_A_INOUT DeeScopeObject *seq_scope, DEE_A_INOUT DeeXAstObject *seq,
 DEE_A_INOUT DeeScopeObject *block_scope, DEE_A_INOUT DeeSAstObject *block);

//////////////////////////////////////////////////////////////////////////
// Create a new While/DoWhile-SAst
#define DeeSAst_NewWhile(tk,labels_,lexer,parser_flags,cond_scope,cond,block_scope,block) \
 _DeeSAst_NewWhileOrDoWhile(DEE_SASTKIND_WHILE,tk,labels_,lexer,parser_flags,cond_scope,cond,block_scope,block)
#define DeeSAst_NewDoWhile(tk,labels_,lexer,parser_flags,cond_scope,cond,block_scope,block) \
 _DeeSAst_NewWhileOrDoWhile(DEE_SASTKIND_DOWHILE,tk,labels_,lexer,parser_flags,cond_scope,cond,block_scope,block)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewWhileOrDoWhile(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeScopeObject *cond_scope,
 DEE_A_INOUT DeeXAstObject *cond, DEE_A_INOUT DeeScopeObject *block_scope,
 DEE_A_INOUT DeeSAstObject *block);

//////////////////////////////////////////////////////////////////////////
// Create a new __loopever/__looponce - SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewSpecialLoop(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *block_scope, DEE_A_INOUT DeeSAstObject *block);
#define DeeSAst_NewLoopEver(tk,labels_,block_scope,block) \
 DeeSAst_NewSpecialLoop(DEE_SASTKIND_LOOPEVER,tk,labels_,block_scope,block)
#define DeeSAst_NewLoopOnce(tk,labels_,block_scope,block) \
 DeeSAst_NewSpecialLoop(DEE_SASTKIND_LOOPONCE,tk,labels_,block_scope,block)
#define DeeSAst_NewLoopNone(tk,labels_,block_scope,block) \
 DeeSAst_NewSpecialLoop(DEE_SASTKIND_LOOPNONE,tk,labels_,block_scope,block)

//////////////////////////////////////////////////////////////////////////
// Create a new goto-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewGoto(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_IN struct DeeParserLabel *dest_label);

//////////////////////////////////////////////////////////////////////////
// Create a new throw-SAst
#define DeeSAst_NewRethrow(tk,labels_) DeeSAst_NewThrow(tk,labels_,NULL)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewThrow(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeXAstObject *ast_throw);

//////////////////////////////////////////////////////////////////////////
// Create a new Try-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewTry(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *rootscope, DEE_A_INOUT DeeScopeObject *scope,
 DEE_A_INOUT DeeSAstObject  *block);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_AddTryHandler(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_IN DeeExceptionHandlerKind kind,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OPT DeeXAstObject *exception_storage, DEE_A_INOUT_OPT DeeTypeObject *type_mask,
 DEE_A_INOUT DeeScopeObject *handler_scope, DEE_A_INOUT DeeSAstObject *handler_block);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_FinalizeTry(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags);

//////////////////////////////////////////////////////////////////////////
// Create a new Module-SAst
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewModule(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *module_, DEE_A_INOUT DeeSAstObject *block);

//////////////////////////////////////////////////////////////////////////
// Create a new Switch-SAst
// NOTE: 'cases' will be moved into the returned statement
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewSwitch(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *expr_scope, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_INOUT DeeSAstObject *block,
 DEE_A_INOUT struct DeeParserLabelList *case_labels);

//////////////////////////////////////////////////////////////////////////
// Create a new Dead-SAst
// Created by dead branches from 'if' statements
#define DeeSAst_NewIfTrue(tk,label_,succ_scope,succ_block,fail_scope,fail_block) \
 DeeSAst_NewIfConst(DEE_SASTKIND_IFTRUE,tk,label_,succ_scope,succ_block,fail_scope,fail_block)
#define DeeSAst_NewIfFalse(tk,label_,succ_scope,succ_block,fail_scope,fail_block) \
 DeeSAst_NewIfConst(DEE_SASTKIND_IFFALSE,tk,label_,succ_scope,succ_block,fail_scope,fail_block)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewIfConst(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeScopeObject *succ_scope, DEE_A_INOUT_OPT DeeSAstObject *succ_block,
 DEE_A_INOUT_OPT DeeScopeObject *fail_scope, DEE_A_INOUT_OPT DeeSAstObject *fail_block);


//////////////////////////////////////////////////////////////////////////
// Merge consecutive statements
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_MergeYield(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeSAstObject *other_yield);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_MergePrint(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeSAstObject *other_print);


// NOTE: Uses the same flags as 'DeeXAst_AttrEx'
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeSAst_AttrEx(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast);
#define DeeSAst_Attr(ob,flags) DeeSAst_AttrEx(ob,flags,(DeeObject *)(ob))
#define DeeSAst_IsNoReturn(ob,flags) ((DeeSAst_Attr(ob,flags)&DEE_AST_ATTRIBUTE_NORETURN)!=0)
#define DeeSAst_IsNoExcept(ob)       ((DeeSAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_NONE)&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0)
#define DeeSAst_IsNoEffect(ob)       ((DeeSAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_NONE)&DEE_AST_ATTRIBUTE_NOEFFECT)!=0)
#define DeeSAst_IsReallyNoExcept(ob) ((DeeSAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0)

//////////////////////////////////////////////////////////////////////////
// Predict the runtime typing of an SAst.
//  - Returns NULL, but doesn't throw an error if the AST is unpredictable
// In: "sast.predict_type.inl"
extern DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
 DeeSAst_PredictType(DEE_A_IN DeeSAstObject const *self);


//////////////////////////////////////////////////////////////////////////
// Compiles the given Statement AST
// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+1'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_Compile(DEE_A_INOUT DeeSAstObject *self, DEE_COMPILER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileBlockWithoutLocalCleanup(
 DEE_A_IN Dee_size_t blockc, DEE_A_IN_R(blockc) DeeSAstObject *const *blockv, DEE_COMPILER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileStrongScopeRoot(
 DEE_A_INOUT DeeSAstObject *self, DEE_COMPILER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileLabelEndpoints(
 DEE_A_IN struct DeeParserLabelRefList const *endpts,
 DEE_A_IN DeeTokenObject *error_token, DEE_COMPILER_PARAMS);


















//////////////////////////////////////////////////////////////////////////
// -==================================================================- //
// |  SAst Parser functions                                           | //
// -==================================================================- //
//////////////////////////////////////////////////////////////////////////

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define DEE_PRIVATE_SAST_RESERVED_KEYWORD_EXTERN_TOKENS
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#define DEE_PRIVATE_SAST_RESERVED_KEYWORD_EXTERN_TOKENS \
 case KWD_extern:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#define DEE_SAST_RESERVED_KEYWORD_TOKENS \
 DEE_PRIVATE_SAST_RESERVED_KEYWORD_EXTERN_TOKENS \
 case KWD_exec: case KWD_export: case KWD_friend: case KWD_template:\
 case KWD_with: case KWD__Static_assert: case KWD_from: case KWD_synchronized:


#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
#define DEE_SAST_STATEMENT_ONLY_ELIF_TOKENS case KWD_elif:
#else
#define DEE_SAST_STATEMENT_ONLY_ELIF_TOKENS
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define DEE_SAST_STATEMENT_ONLY_EXTERN_TOKENS case KWD_extern:
#else
#define DEE_SAST_STATEMENT_ONLY_EXTERN_TOKENS
#endif
#define DEE_SAST_STATEMENT_ONLY_TOKENS \
 case KWD_case: case KWD_default: case KWD_import:\
 case ';': case '{': case KWD_return: case KWD_yield:\
 case KWD___assert: case KWD_assert: case KWD_print:\
 case KWD___if_true: case KWD___if_false: case KWD_else:\
 DEE_SAST_STATEMENT_ONLY_ELIF_TOKENS case KWD_break:\
 case KWD_continue: case KWD_goto: case KWD_throw:\
 case KWD_try: case KWD_catch: case KWD_finally:\
 case KWD_del: DEE_SAST_STATEMENT_ONLY_EXTERN_TOKENS\
 case KWD_typedef: DEE_SAST_RESERVED_KEYWORD_TOKENS



//////////////////////////////////////////////////////////////////////////
// Parse a statement
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_Parse(DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseExpression(DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseExpressionWithAttribute(
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses all statements inside of a new scope until 'end_token' is encountered
// HINT: 'end_token' is usually set to '}'
// NOTES:
//   - Also works if the first token was an 'end_token',
//     in which case an DEE_SASTKIND_EMPTY ast will be returned.
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseAllInNewScopeUntil(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_IN TPPTokenID end_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseAllInOldScopeUntil(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_IN TPPTokenID end_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_ParseCaseLabel(
 DEE_A_INOUT DeeTokenObject *case_token, // in "sast_parse.case_label.inl"
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_ParseDefaultLabel(
 DEE_A_INOUT DeeTokenObject *default_token, // in "sast_parse.case_label.inl"
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses an if statment
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseIfStatement(
 DEE_A_INOUT DeeTokenObject *if_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_PARSER_PARAMS); // in "sast_parse.if_statement.inl"

extern DEE_A_RET_EXCEPT(-1) int _DeeSAst_ParseTryHandler(
 DEE_A_INOUT DeeSAstObject *self, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses a for/__foreach statement
// in: "sast_parse.for_or_foreach.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseForOrForeachStatement(
 DEE_A_INOUT DeeTokenObject *for_token, DEE_A_IN DeeSAstKind forin_kind,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses a while statement
// in: "sast_parse.while.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseWhileStatement(
 DEE_A_INOUT DeeTokenObject *while_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses a do...while statement
// in: "sast_parse.while.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseDoWhileStatement(
 DEE_A_INOUT DeeTokenObject *do_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses a __looponce statement
// in: "sast_parse.looponce.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseSpecialLoopStatement(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *specialloop_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses a typedef statement
// in: "sast_parse.typedef.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseTypedefStatement(
 DEE_A_INOUT DeeTokenObject *typedef_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSingleTypedefWithBaseStatement(
 DEE_A_INOUT DeeXAstObject *typedef_base,
 DEE_A_IN struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS);


//////////////////////////////////////////////////////////////////////////
// Parses a module statement
// in: "sast_parse.module.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseModuleStatement(
 DEE_A_INOUT DeeTokenObject *module_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseModuleStatementWithAttribute(
 DEE_A_INOUT DeeTokenObject *module_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);


//////////////////////////////////////////////////////////////////////////
// Parses an extern statement
// in: "sast_parse.extern.inl"
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseExternStatement(
 DEE_A_INOUT DeeTokenObject *extern_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseExternStatementWithAttribute(
 DEE_A_INOUT DeeTokenObject *extern_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseSingleExternWithBaseStatement(
 DEE_A_INOUT_OPT DeeXAstObject *extern_file, DEE_A_INOUT DeeXAstObject *extern_base,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */



#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorForOrForeachStatement(
 DEE_A_INOUT DeeTokenObject *for_token, DEE_A_IN DeeSAstKind forin_kind, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses the block of a generator (creates yield statements,
// as well as parsing special for some branch-statements)
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorBlock(
 DEE_PARSER_PARAMS); // in: "sast_parse.generator_block.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorIfStatement(
 DEE_A_INOUT DeeTokenObject *if_token, DEE_PARSER_PARAMS); // in "sast_parse.if_statement.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorWhileStatement(
 DEE_A_INOUT DeeTokenObject *while_token, DEE_PARSER_PARAMS); // in "sast_parse.while.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorDoWhileStatement(
 DEE_A_INOUT DeeTokenObject *do_token, DEE_PARSER_PARAMS); // in "sast_parse.do_while.inl"
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorSpecialLoopStatement(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *specialloop_token, DEE_PARSER_PARAMS); // in "sast_parse.specialloop.inl"
#endif






//////////////////////////////////////////////////////////////////////////
// Optimizer
// WARNING: For performance, this function is allowed to modify 'self'
// NOTE: Some optimizations will not work if specific parser flags weren't set during parsing
// in: "sast_optimize.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeSAst_Optimize(
 DEE_A_INOUT DeeSAstObject *self, DEE_OPTIMIZE_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Removes unused and uninitialized variables from 'self' as well as all constants
// Returns the about of removed entires
extern Dee_size_t DeeScope_OptimizeRemoveUnusedNames(
 DEE_A_INOUT DeeScopeObject *self);
#define DeeScope_OPTIMIZE(self)\
do{\
 if ((optimize_flags&DEE_OPTIMIZE_FLAG_UNUSED_VAR)!=0)\
  *performed_optimizations += DeeScope_OptimizeRemoveUnusedNames(self);\
}while(0)



#define DeeAst_HasLabel(ob,label)     (DeeSAst_Check(ob) ? DeeSAst_HasLabel((DeeSAstObject *)(ob),label) : DeeXAst_HasLabel((DeeXAstObject *)(ob),label))
#define DeeAst_HasLabels(ob)          (DeeSAst_Check(ob) ? DeeSAst_HasLabels((DeeSAstObject *)(ob)) : DeeXAst_HasLabels((DeeXAstObject *)(ob)))
#define DeeAst_HasBreakOrContinue(ob) (DeeSAst_Check(ob) ? DeeSAst_HasBreakOrContinue((DeeSAstObject *)(ob)) : DeeXAst_HasBreakOrContinue((DeeXAstObject *)(ob)))

extern DEE_A_RET_EXCEPT(-1) int DeeSAst_CollectTopScopeChildren(
 DEE_A_IN DeeSAstObject const *self, DEE_A_INOUT struct DeeScopeList *scopes);
extern int DeeSAst_VisitAll(
 DEE_A_IN DeeSAstObject *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure);
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_CopyWithScope(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeScopeObject *new_scope,
 DEE_A_INOUT DeeLexerObject *lexer);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSAst_Equals(
 DEE_A_IN DeeSAstObject const *self,
 DEE_A_IN DeeSAstObject const *right);
extern DEE_A_RET_WUNUSED int DeeSAst_UsesVariable(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeSAst_CountVariableUses(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);


DEE_DECL_END

#endif /* !GUARD_DEEMON_SAST_H */
