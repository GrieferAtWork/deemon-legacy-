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
#ifndef GUARD_DEEMON_STMT_AST_COMPILE_C
#define GUARD_DEEMON_STMT_AST_COMPILE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/expr_ast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/stmt_ast.h>
#include <deemon/file.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/extern.h>
#include <deemon/tuple.h>

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_iterator_type(DeeTypeObject *tp) {
 if (tp == &DeeString_Type) return &DeeStringIterator_Type;
 if (tp == &DeeTuple_Type || tp == &DeeList_Type) return &DeePointerIterator_Type;
 if (tp == &DeeDict_Type || tp == &DeeDictValues_Type) return &DeeDictIterator_Type;
 if (tp == &DeeDictKeys_Type) return &DeeDictKeysIterator_Type;
 if (tp == &DeeDictItems_Type) return &DeeDictItemsIterator_Type;
 if (tp == (DeeTypeObject *)&DeeNone_Type ||
     tp == &DeeRangeInt8Iterator_Type ||
     tp == &DeeRangeInt16Iterator_Type ||
     tp == &DeeRangeInt32Iterator_Type ||
     tp == &DeeRangeInt64Iterator_Type ||
     tp == &DeeRangeUInt8Iterator_Type ||
     tp == &DeeRangeUInt16Iterator_Type ||
     tp == &DeeRangeUInt32Iterator_Type ||
     tp == &DeeRangeUInt64Iterator_Type
     ) return tp;
 return NULL;
}      
DEE_STATIC_INLINE(int) _dee_nosideeffect_iterable(DeeTypeObject *tp) {
 return 
  (tp == &DeeString_Type) ||
  (tp == (DeeTypeObject *)&DeeNone_Type) ||
  (tp == &DeeTuple_Type) ||
  (tp == &DeeList_Type) ||
  (tp == &DeeDict_Type) ||
  (tp == &DeeDictKeys_Type) ||
  (tp == &DeeDictItems_Type) ||
  (tp == &DeeDictValues_Type) ||
  (tp == &DeeString_Type) ||
  (tp == &DeeRangeInt8Iterator_Type) ||
  (tp == &DeeRangeInt16Iterator_Type) ||
  (tp == &DeeRangeInt32Iterator_Type) ||
  (tp == &DeeRangeInt64Iterator_Type) ||
  (tp == &DeeRangeUInt8Iterator_Type) ||
  (tp == &DeeRangeUInt16Iterator_Type) ||
  (tp == &DeeRangeUInt32Iterator_Type) ||
  (tp == &DeeRangeUInt64Iterator_Type)
  ;
}      

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_WUNUSED int DeeStmtAst_IsNoeffect(
 DEE_A_IN_OBJECT(DeeStmtAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
#ifdef DEE_DEBUG
 DeeStmtAstObject *_stmt = (DeeStmtAstObject *)self;
#else
#define _stmt ((DeeStmtAstObject *)self)
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 switch (_stmt->ob_kind) {
  case DeeStmtAstKind_EMPTY:
   return 1;
  case DeeStmtAstKind_MODULE:
   return DeeStmtAst_IsNoeffect(
    (DeeObject *)_stmt->ob_module.ob_mod_block,
    (DeeObject *)_stmt->ob_module.ob_mod_module->ob_scope);
  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_inl_stmts.ob_inl_stmts_v)+_stmt->ob_inl_stmts.ob_inl_stmts_c;
   while (iter != end) if (!DeeStmtAst_IsNoeffect((DeeObject *)*iter++,scope)) return 0;
   return 1;
  } break;
  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_stmts.ob_stmts_v)+_stmt->ob_stmts.ob_stmts_c;
   while (iter != end) if (!DeeStmtAst_IsNoeffect((DeeObject *)*iter++,(DeeObject *)_stmt->ob_stmts.ob_stmts_scope)) return 0;
   return 1;
  } break;
  case DeeStmtAstKind_EXPR:
   return DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_expr.ob_expr_ast,scope);
  case DeeStmtAstKind_YIELD:
   return DeeTuple_SIZE(_stmt->ob_yield.ob_yld_exprs) == 0;
  case DeeStmtAstKind_DEL:
   return DeeTuple_SIZE(_stmt->ob_del.ob_del_exprs) == 0;
  case DeeStmtAstKind_PRINT_NOLF:
   DEE_ASSERT(_stmt->ob_print.ob_prt_tpl->ob_kind == DeeExprAstKind_TUPLE);
   return (!_stmt->ob_print.ob_prt_file || DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_print.ob_prt_file,scope)) &&
          (DeeTuple_SIZE(_stmt->ob_print.ob_prt_tpl->ob_args_a) == 0);
  case DeeStmtAstKind_IF:
   return DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_if.ob_if_expr,(DeeObject *)_stmt->ob_if.ob_if_thenscope) &&
          DeeStmtAst_IsNoeffect((DeeObject *)_stmt->ob_if.ob_if_thenstmt,(DeeObject *)_stmt->ob_if.ob_if_thenscope) &&
          (!_stmt->ob_if.ob_if_elsestmt || DeeStmtAst_IsNoeffect(
          (DeeObject *)_stmt->ob_if.ob_if_elsestmt,
          (DeeObject *)_stmt->ob_if.ob_if_elsescope));

  case DeeStmtAstKind_FOR:
   // Without a condition, we either have a break in the statement block (side-effect) or an infinite loop (also a side-effect)
   if (!_stmt->ob_for.ob_f_forcont) return 0;
   if (!DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_for.ob_f_forcont,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   if (_stmt->ob_for.ob_f_forinit && !DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_for.ob_f_forinit,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   if (_stmt->ob_for.ob_f_fornext && !DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_for.ob_f_fornext,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   return DeeStmtAst_IsNoeffect((DeeObject *)_stmt->ob_for.ob_f_block,(DeeObject *)_stmt->ob_for.ob_f_scope);

  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH: {
   DeeTypeObject *iter_type;
   if (!DeeExprAst_IsNoeffect((DeeObject *)_stmt->ob_for_in.ob_fi_iterlist,(DeeObject *)_stmt->ob_for_in.ob_fi_scope)) return 0;
   if (!DeeStmtAst_IsNoeffect((DeeObject *)_stmt->ob_for_in.ob_fi_block,(DeeObject *)_stmt->ob_for_in.ob_fi_scope)) return 0;
   iter_type = DeeExprAst_PredictType((DeeObject *)_stmt->ob_for_in.ob_fi_iterlist,(DeeObject *)_stmt->ob_for_in.ob_fi_scope);
   if (iter_type) return _dee_nosideeffect_iterable(iter_type);
  } break;

  case DeeStmtAstKind_SEXPR:
   return DeeExprAst_IsNoeffect(
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_scope);

  case DeeStmtAstKind_SSTMT:
   return DeeStmtAst_IsNoeffect(
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_scope);

  case DeeStmtAstKind_LOOP_ONCE:
   return DeeStmtAst_IsNoeffect(
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_block,
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_scope);

  default: break;
 }
 return 0;
#ifdef _stmt
#undef _stmt
#endif
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_WUNUSED DeeTypeObject *DeeStmtAst_PredictType(
 DEE_A_IN_OBJECT(DeeStmtAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
#ifdef DEE_DEBUG
 DeeStmtAstObject *_stmt = (DeeStmtAstObject *)self;
#else
#define _stmt ((DeeStmtAstObject *)self)
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 switch (_stmt->ob_kind) {
  case DeeStmtAstKind_EMPTY:
  case DeeStmtAstKind_RETURN:
  case DeeStmtAstKind_YIELD:
  case DeeStmtAstKind_DEL:
  case DeeStmtAstKind_BREAK:
  case DeeStmtAstKind_CONTINUE:
  case DeeStmtAstKind_ASSERT:
  case DeeStmtAstKind_FOR:
  case DeeStmtAstKind_WHILE:
  case DeeStmtAstKind_DO_WHILE:
  case DeeStmtAstKind_LOOP_ONCE:
  case DeeStmtAstKind_GOTO:
# if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
  case DeeStmtAstKind_RETHROW:
  case DeeStmtAstKind_TRY:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
   return (DeeTypeObject *)&DeeNone_Type;
  case DeeStmtAstKind_MODULE:
   return DeeStmtAst_PredictType(
    (DeeObject *)_stmt->ob_module.ob_mod_block,
    (DeeObject *)_stmt->ob_module.ob_mod_module->ob_scope);
  case DeeStmtAstKind_INL_STMTS:
   break; // TODO
  case DeeStmtAstKind_STMTS:
   break; // TODO
  case DeeStmtAstKind_EXPR:
   return DeeExprAst_PredictType((DeeObject *)_stmt->ob_expr.ob_expr_ast,scope);
  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF:
   return &DeeTuple_Type;
  case DeeStmtAstKind_IF: {
   DeeTypeObject *predict_a,*predict_b;
   predict_a = DeeStmtAst_PredictType(
    (DeeObject *)_stmt->ob_if.ob_if_thenstmt,
    (DeeObject *)_stmt->ob_if.ob_if_thenscope);
   if (!predict_a) return NULL; // Unpredictable
   predict_b = _stmt->ob_if.ob_if_elsestmt
    ? DeeStmtAst_PredictType((DeeObject *)_stmt->ob_if.ob_if_elsestmt,(DeeObject *)_stmt->ob_if.ob_if_elsescope)
    : (DeeTypeObject *)&DeeNone_Type;
   if (predict_a == predict_b) return predict_a; // Same type in both branches --> we can predict this!
  } break;
  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH: {
   DeeTypeObject *loop_type;
   if ((loop_type = DeeExprAst_PredictType(
    (DeeObject *)_stmt->ob_for_in.ob_fi_iterlist,
    (DeeObject *)_stmt->ob_for_in.ob_fi_scope)) != NULL)
    return _dee_predict_iterator_type(loop_type);
  } break;

  case DeeStmtAstKind_EXTERN: {
   DeeTypeObject *result;
   // TODO: Once I reworked extern functions to store their
   //       closures in their types, this has to be adjusted!
   result = DeeType_LValue((DeeTypeObject *)_stmt->ob_extern.ob_extern_decl.ob_type);
   if (!result) DeeError_Handled();
   return result;
  } break;

  case DeeStmtAstKind_SEXPR:
   return DeeExprAst_PredictType(
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_scope);
  case DeeStmtAstKind_SSTMT:
   return DeeStmtAst_PredictType(
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_scope);
  default: break;
 }
 return NULL;
#ifdef _stmt
#undef _stmt
#endif
}
DEE_COMPILER_MSVC_WARNING_POP



#ifdef DEE_SCOPE_FLAG_USING_LOCALS
#define have_local_names \
 ((flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0||\
 (((DeeScopeObject *)scope)->ob_flags&DEE_SCOPE_FLAG_USING_LOCALS)!=0)
#else
#define have_local_names \
 ((flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0)
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int DeeStmtAst_Compile(
 DEE_A_IN_OBJECT(DeeStmtAstObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCodeWriter *writer,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN Dee_uint32_t flags) {
#ifdef DEE_DEBUG
 DeeStmtAstObject *stmt = (DeeStmtAstObject *)self;
#else
#define stmt ((DeeStmtAstObject *)self)
#endif
 DeeExprAstObject *expr;
 Dee_size_t pos1,pos2,future_arg_1,future_arg_2;
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(writer);
 if (stmt->ob_label_endpoints.ob_c != 0) {
  DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
  struct DeeLabelRef *label_iter,*label_end; // Put label endpoints
  label_end = (label_iter = stmt->ob_label_endpoints.ob_v)+stmt->ob_label_endpoints.ob_c;
  while (label_iter != label_end) {
   struct DeeLabelDecl *label_decl;
   if ((label_iter->ob_flags&DEE_LABEL_REF_FLAGS_CASELABEL)!=0) {
    DEE_ASSERT(weak_root->ob_cases && "Compiling case label without list of cases");
    label_decl = &weak_root->ob_cases->ob_v[label_iter->ob_id].ob_decl;
   } else {
    label_decl = &weak_root->ob_labels.ob_v[label_iter->ob_id].ob_decl;
   }
   if (DeeCodeWriter_PutLabel(writer,label_decl,lexer,(DeeObject *)stmt->ob_token) == -1) return -1;
   ++label_iter;
  }
  // Always put debug info if this statement is a label endpoint
  if ((flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) goto put_debug;
 }
 if ((flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0 && (
  // Only put debug information if the statement isn't noexcept or has side-effects
  !DeeStmtAst_IsNoexcept((DeeObject *)stmt,(DeeObject *)scope) ||
  !DeeStmtAst_IsNoeffect((DeeObject *)stmt,(DeeObject *)scope))) {
put_debug:
  // Put some debug information
  if (DeeCodeWriter_Debug(writer,
   (flags&DEE_COMPILER_FLAG_DEBUG_FILE)!=0 ? DeeToken_FILE(stmt->ob_token) : NULL,
   (flags&DEE_COMPILER_FLAG_DEBUG_LINE)!=0 ? DeeToken_LINE(stmt->ob_token) : 0
   ) == -1) return -1;
 }
#define ret_used   ((flags&DEE_COMPILER_FLAG_USED)!=0)
 switch (stmt->ob_kind) {
  case DeeStmtAstKind_MODULE:
   DEE_ASSERT(DeeObject_Check(stmt->ob_module.ob_mod_module));
   DEE_ASSERT(DeeModule_Check(stmt->ob_module.ob_mod_module));
   DEE_ASSERT(DeeObject_Check(stmt->ob_module.ob_mod_module->ob_scope));
   DEE_ASSERT(DeeScope_Check(stmt->ob_module.ob_mod_module->ob_scope));
   // Compile the module block (without local variable cleanup)
   return DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_module.ob_mod_block,
    (DeeObject *)stmt->ob_module.ob_mod_module->ob_scope,writer,
    lexer,flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP);

  case DeeStmtAstKind_EMPTY:
push_none:
   if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
   break;

  case DeeStmtAstKind_INL_STMTS:
  case DeeStmtAstKind_STMTS: {
   DeeObject **begin,**end;
   DeeScopeObject *scope2;
   if (stmt->ob_kind == DeeStmtAstKind_INL_STMTS) {
    scope2 = (DeeScopeObject *)scope;
    end = (begin = (DeeObject **)stmt->ob_inl_stmts.ob_inl_stmts_v)
     +stmt->ob_inl_stmts.ob_inl_stmts_c;
   } else {
    scope2 = stmt->ob_stmts.ob_stmts_scope;
    end = (begin = (DeeObject **)stmt->ob_stmts.ob_stmts_v)+stmt->ob_stmts.ob_stmts_c;
   }
   temp = 0;
   DEE_ASSERT(begin != end);
   if (ret_used) --end; else goto stmts_iter; // Must compile the last statement a bit differently
   while (begin != end) {stmts_iter:
    if (((DeeStmtAstObject *)*begin)->ob_label_endpoints.ob_c != 0) temp = 0; // The label made this statement reachable again
    if (!temp) {
     // Intermediate statements aren't used for the end result
     temp = DeeStmtAst_Compile(*begin,(DeeObject *)scope2,
      writer,lexer,flags&~(DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP));
     if (temp == -1) return -1;
    }
    ++begin;
   }
   if (ret_used) {
    if (((DeeStmtAstObject *)*end)->ob_label_endpoints.ob_c != 0) temp = 0;
    if (temp) DeeCodeWriter_INCSTACK(writer); else {
     temp = DeeStmtAst_Compile(*end,(
      DeeObject *)scope2,writer,lexer,flags&~DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP);
     if (temp == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if (temp != 0) return temp;
#endif
    }
   }
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   else if (temp != 0) return temp;
#endif
   if ((flags&DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP) == 0 &&
       DeeCodeWriter_FinalizeScope(writer,(DeeObject *)scope2,lexer) == -1) return -1;
  } break;

  case DeeStmtAstKind_EXPR: // Simply compile an expression
   return DeeExprAst_Compile((DeeObject *)stmt->ob_expr.ob_expr_ast,scope,writer,lexer,flags);

  case DeeStmtAstKind_SSTMT: // Compile a scoped statement
   return DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)stmt->ob_sstmt.ob_sstmt_scope,
    writer,lexer,flags);

  case DeeStmtAstKind_SEXPR: // Compile a scoped expression
   return DeeExprAst_Compile(
    (DeeObject *)stmt->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)stmt->ob_sexpr.ob_sexpr_scope,
    writer,lexer,flags);

  case DeeStmtAstKind_RETURN:
   // TODO: Implement a noreturn signal to the caller if 'ret_used' is true
   //    >> function foo() {
   //    >>   a = ({ return 20; }); // Noreturn after this point!
   //    >>   print a;
   //    >>   return a;
   //    >> }
   //    >> b = foo();
   if (stmt->ob_return.ob_ret_expr) {
    // Return <expr>
    temp = DeeExprAst_Compile((DeeObject *)stmt->ob_return.ob_ret_expr,
                              scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED);
    if (temp == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
    if (temp != 0) {
     if (!ret_used) DeeCodeWriter_DECSTACK(writer); // Satisfy the write
     return temp;
    }
#endif
    if (DeeCodeWriter_Ret(writer) == -1) return -1;
   } else {
    DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
    if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_RETVAR)!=0) {
     if (DeeCodeWriter_WriteOp(writer,OP_RETVAREXIT) == -1) return -1; // Return variable exit
    } else if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) {
     if (DeeCodeWriter_YieldExit(writer) == -1) return -1; // Yield Exit
    } else {
     if (DeeCodeWriter_RetNone(writer) == -1) return -1; // Return none
    }
   }
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
on_noreturn:
   if ((flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0 &&
       DeeCodeWriter_Unreachable(writer) == -1) return -1;
   if (ret_used) DeeCodeWriter_INCSTACK(writer); // Satisfy the writer
   return 1;
#else
   goto push_none;
#endif

  case DeeStmtAstKind_YIELD: {
   DeeExprAstObject *yield_expr,**yield_expr_iter,**yield_expr_end;
   DEE_ASSERT(DeeObject_Check(stmt->ob_yield.ob_yld_exprs) &&
              DeeTuple_Check(stmt->ob_yield.ob_yld_exprs));
   yield_expr_end = (yield_expr_iter = (DeeExprAstObject **)DeeTuple_ELEM(
    stmt->ob_yield.ob_yld_exprs))+DeeTuple_SIZE(stmt->ob_yield.ob_yld_exprs);
   while (yield_expr_iter != yield_expr_end) {
    yield_expr = *yield_expr_iter++;
    if (yield_expr->ob_kind == DeeExprAstKind_EXPAND) {
     temp = DeeExprAst_Compile((DeeObject *)yield_expr->ob_op_a,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED);
     if (temp == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if (temp == 1) { DeeCodeWriter_DECSTACK(writer); return 1; }
#endif
     if (DeeCodeWriter_UnaryOp(writer,OP_SEQ_ITER_SELF) == -1 ||
         DeeCodeWriter_YieldAll(writer) == -1) return -1;
    } else {
     temp = DeeExprAst_Compile((DeeObject *)yield_expr,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED);
     if (temp == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if (temp == 1) { DeeCodeWriter_DECSTACK(writer); return 1; }
#endif
     if (DeeCodeWriter_Yield(writer) == -1) return -1;
    }
   }
  } goto push_none;

  case DeeStmtAstKind_DEL: {
   DeeExprAstObject **iter,**end;
   end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(
    stmt->ob_del.ob_del_exprs))+DeeTuple_SIZE(stmt->ob_del.ob_del_exprs);
   while (iter != end) {
    switch ((expr = *iter++)->ob_kind) {
     case DeeExprAstKind_DECLVARIABLE:
     case DeeExprAstKind_VARIABLE: {
      if ((expr->ob_var->ob_flags & DEE_LOCALVAR_FLAG_INITIALIZED) == 0) {
       if (DeeError_CompilerError(DEE_WARNING_UNINITIALIZED_VARIABLE_IN_DEL,
        lexer,(DeeObject *)stmt->ob_token,"Cannot delete uninitialized variable") == -1) return -1;
       goto push_none;
      } else {
       if (DeeCodeWriter_DelLocal(writer,(DeeObject *)expr->ob_var) == -1) return -1;
      }
     } break;
     case DeeExprAstKind_SEQ_GET:
      if ((temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
      if (temp == 1) { DeeCodeWriter_DECSTACK(writer); return 1; }
#endif
      if ((temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
      if (temp == 1) { DeeCodeWriter_DECSTACK_N(writer,2); return 1; }
#endif
      if (DeeCodeWriter_WriteOp(writer,OP_SEQ_DEL) == -1) return -1;
      DeeCodeWriter_DECSTACK_N(writer,2); // Consumed by 'OP_SEQ_DEL'
      break;
     case DeeExprAstKind_SEQ_RANGE_GET:
      if ((temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
      if (temp == 1) { DeeCodeWriter_DECSTACK(writer); return 1; }
#endif
      if (DeeExprAst_OP_B(expr)) {
       if ((temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
       if (temp == 1) { DeeCodeWriter_DECSTACK_N(writer,2); return 1; }
#endif
      } else if (DeeCodeWriter_PushNone(writer) == -1) return -1;
      if (DeeExprAst_OP_C(expr)) {
       if ((temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
       if (temp == 1) { DeeCodeWriter_DECSTACK_N(writer,3); return 1; }
#endif
      } else if (DeeCodeWriter_PushNone(writer) == -1) return -1;
      if (DeeCodeWriter_WriteOp(writer,OP_SEQ_RANGE_DEL) == -1) return -1;
      DeeCodeWriter_DECSTACK_N(writer,3); // Consumed by 'OP_SEQ_RANGE_DEL'
      break;
     case DeeExprAstKind_ATTR_GET: {
      Dee_uint16_t member_id;
      temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(expr),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED);
      if (temp == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
      if (temp == 1) { DeeCodeWriter_DECSTACK(writer); return 1; }
#endif
      if ((member_id = DeeCodeWriter_AllocConst(writer,(DeeObject *)DeeExprAst_MEMBER(expr))) == (Dee_uint16_t)-1 ||
          DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_DEL_C,member_id) == -1) return -1;
      DeeCodeWriter_DECSTACK(writer); // Consumed by 'OP_ATTR_DEL_C'
     } break;
     default: break;
    }
   }
   // del statements always return 'none'
  } goto push_none;

  case DeeStmtAstKind_BREAK:
   DEE_ASSERT(DeeCodeWriter_HAS_BREAK(writer) && "'break' statement outside of loop block");
   if (DeeCodeWriter_Break(writer) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
on_brk_cnt:
   if ((flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0 &&
       DeeCodeWriter_Unreachable(writer) == -1) return -1;
   if (ret_used) DeeCodeWriter_INCSTACK(writer);
   return 2;
#else
   goto push_none;
#endif

  case DeeStmtAstKind_CONTINUE:
   DEE_ASSERT(DeeCodeWriter_HAS_BREAK(writer) && "'continue' statement outside of loop block");
   if (DeeCodeWriter_Continue(writer) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   goto on_brk_cnt;
#else
   goto push_none;
#endif

  case DeeStmtAstKind_ASSERT: {
   Dee_size_t jump_base,jump_address;
   if ((flags&DEE_COMPILE_FLAG_ASSERT)!=0) {
#if 0
    if ((flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)==0 &&
        // Put manual debug information so the assertion knows where it is
        // NOTE: This only happens when assertions are enabled outside of debug mode
        DeeCodeWriter_Debug(writer,DeeToken_FILE(stmt->ob_token),DeeToken_LINE(stmt->ob_token)) == -1
        ) return -1;
#endif
    if ((temp = DeeExprAst_Compile((DeeObject *)stmt->ob_assert.ob_asrt_expr,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
    if (temp != 0) { DeeCodeWriter_DECSTACK(writer); return temp; }
#endif
    jump_base = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
    if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP_IF_TT,&jump_address) == -1) return -1;
    // Assertion failed --> Compile the message (if there is one)
    if (stmt->ob_assert.ob_asrt_msg) {
     if ((temp = DeeExprAst_Compile((DeeObject *)stmt->ob_assert.ob_asrt_msg,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if (temp != 0) { DeeCodeWriter_DECSTACK_N(writer,2); return temp; }
#endif
     // Pack the assert expression and message into a tuple
     if (_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,2) == -1) return -1;
    } else { // Pack ~only~ the assert expression into a tuple
     if (_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,1) == -1) return -1;
    }
    if (DeeCodeWriter_PushConst(writer,(DeeObject *)&DeeBuiltinFunction___builtin_assertion_failed) == -1) return -1;
    // Rotate the assertion_failed function with it's argument tuple
    if (DeeCodeWriter_WriteOp(writer,OP_ROT_2) == -1) return -1;
    // Call the function
    if (DeeCodeWriter_BinaryOp(writer,OP_CALL) == -1) return -1;
    // NOTE: At this point, the function return value is still on the stack, but
    //       the pop that would otherwise remove the assert expression can handle that as well!
    DeeCodeWriter_SetFutureArg(writer,jump_address,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-jump_base));
    // Pop the assert expression (or '__builtin_assertion_failed' return value, if it didn't throw an error for some reason...)
    if (DeeCodeWriter_Pop(writer) == -1) return -1;
   }
   goto push_none; // assert statements return 'none'
  }

  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF: {
#if 1
   DeeObject *print_tpl; DeeExprAstObject **print_begin,**print_end;
   if (stmt->ob_print.ob_prt_file) {
    if (!stmt->ob_print.ob_prt_tpl && stmt->ob_kind == DeeStmtAstKind_PRINT_NOLF)
     return DeeExprAst_Compile((DeeObject *)stmt->ob_print.ob_prt_file,scope,writer,lexer,flags);
    if (DeeExprAst_Compile((DeeObject *)stmt->ob_print.ob_prt_file,
     scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
   }
   if (stmt->ob_print.ob_prt_tpl) {
    print_tpl = (DeeObject *)stmt->ob_print.ob_prt_tpl->ob_args_a;
    print_end = (print_begin = (DeeExprAstObject **)DeeTuple_ELEM(print_tpl))+DeeTuple_SIZE(print_tpl);
    DEE_ASSERT(print_begin != print_end && "Empty print statements should have no print tuple assigned");
    while (1) {
     DeeExprAstObject *elem = *print_begin++;
     if (print_begin == print_end) { // last
      if (ret_used && stmt->ob_print.ob_prt_file) { // Duplicate the custom file
       if (DeeCodeWriter_WriteOp(writer,OP_DUP) == -1) return -1;
       DeeCodeWriter_INCSTACK(writer);
      }
      if (elem->ob_kind == DeeExprAstKind_EXPAND) {
       if (DeeExprAst_Compile((DeeObject *)elem->ob_op_a,
        scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
       if (stmt->ob_print.ob_prt_file) {
        if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
         stmt->ob_kind == DeeStmtAstKind_PRINT_NOLF
         ? OP_PRINT_ALL_F : OP_PRINT_ALL_F_END))) == -1) return -1;
        DeeCodeWriter_DECSTACK_N(writer,2); // Popped by 'OP_PRINT_ONE_F' / 'OP_PRINT_ONE_F_END'
       } else {
        if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
         stmt->ob_kind == DeeStmtAstKind_PRINT_NOLF
         ? OP_PRINT_ALL : OP_PRINT_ALL_END))) == -1) return -1;
        DeeCodeWriter_DECSTACK(writer);
        if (ret_used) goto push_stdout;
       }
      } else {
       if (DeeExprAst_Compile((DeeObject *)elem,
        scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
       if (stmt->ob_print.ob_prt_file) {
        if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
         stmt->ob_kind == DeeStmtAstKind_PRINT_NOLF
         ? OP_PRINT_ONE_F : OP_PRINT_ONE_F_END))) == -1) return -1;
        DeeCodeWriter_DECSTACK_N(writer,2); // Popped by 'OP_PRINT_ONE_F' / 'OP_PRINT_ONE_F_END'
       } else {
        if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
         stmt->ob_kind == DeeStmtAstKind_PRINT_NOLF
         ? OP_PRINT_ONE : OP_PRINT_ONE_END))) == -1) return -1;
        DeeCodeWriter_DECSTACK(writer);
        if (ret_used) {
push_stdout:
         if (DeeCodeWriter_PushConst(writer,DeeFile_Std(DEE_STDOUT)) == -1) return -1;
        }
       }
      }
      break;
     } else {
      if (elem->ob_kind == DeeExprAstKind_EXPAND) {
       if (DeeExprAst_Compile((DeeObject *)elem->ob_op_a,
        scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
       if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
        stmt->ob_print.ob_prt_file
        ? OP_PRINT_ALL_F_SEP : OP_PRINT_ALL_SEP))) == -1) return -1;
      } else {
       if (DeeExprAst_Compile((DeeObject *)elem,
        scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
       if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)((
        stmt->ob_print.ob_prt_file
        ? OP_PRINT_ONE_F_SEP : OP_PRINT_ONE_SEP))) == -1) return -1;
      }
      DeeCodeWriter_DECSTACK(writer);
     }
    }
   } else if (stmt->ob_kind != DeeStmtAstKind_PRINT_NOLF) {
    // print;
    // print my_file:;
    if (stmt->ob_print.ob_prt_file) {
     if (ret_used) { // Duplicate the custom file
      if (DeeCodeWriter_WriteOp(writer,OP_DUP) == -1) return -1;
      DeeCodeWriter_INCSTACK(writer);
     }
     if (DeeCodeWriter_WriteOp(writer,OP_PRINT_END_F) == -1) return -1;
     DeeCodeWriter_DECSTACK(writer);
    } else {
     if (DeeCodeWriter_WriteOp(writer,OP_PRINT_END) == -1) return -1;
    }
   } else {
    // print,;
    // print my_file:,;
    if (ret_used && !stmt->ob_print.ob_prt_file)
     goto push_stdout;
   }
#else
   // TODO: Expand expressions
   Dee_size_t print_size;
   if (stmt->ob_print.ob_prt_file) {
    if ((temp = DeeExprAst_Compile(
     (DeeObject *)stmt->ob_print.ob_prt_file,
     scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
    if (temp != 0) { DeeCodeWriter_DECSTACK(writer); return temp; }
#endif
   }
   if (stmt->ob_print.ob_prt_tpl) {
    DeeObject *print_tpl,**print_begin,**print_end;
    print_tpl = (DeeObject *)stmt->ob_print.ob_prt_tpl->ob_args_a;
    print_end = (print_begin = DeeTuple_ELEM(print_tpl))+(
     print_size = DeeTuple_SIZE(print_tpl));
    // Compile the print expressions
    while (print_begin != print_end) {
     // TODO: expand expression support here
     if ((temp = DeeExprAst_Compile(*print_begin++,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if (temp != 0) {
      DeeCodeWriter_DECSTACK_N(writer,(Dee_size_t)(print_begin-DeeTuple_ELEM(print_tpl)));
      if (stmt->ob_print.ob_prt_file) DeeCodeWriter_DECSTACK(writer);
      return temp;
     }
#endif
    }
   } else {
    print_size = 0;
   }
   if (ret_used) {
    if (DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint16_t)(
     stmt->ob_kind==DeeStmtAstKind_PRINT_NOLF
     ? (stmt->ob_print.ob_prt_file ? OP_PRINT_F_PUSH_TPL_NOLF : OP_PRINT_PUSH_TPL_NOLF)
     : (stmt->ob_print.ob_prt_file ? OP_PRINT_F_PUSH_TPL : OP_PRINT_PUSH_TPL)),
     (Dee_uint16_t)print_size) == -1) return -1;
    writer->ob_stack_size -= print_size;
    if (stmt->ob_print.ob_prt_file) { // Pop the file
     if (DeeCodeWriter_WriteOp(writer,OP_ROT_2) == -1 ||
         DeeCodeWriter_Pop(writer) == -1
         ) return -1;
    }
    DeeCodeWriter_INCSTACK(writer); // The tuple pushed by the operator
   } else {
    if (DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint16_t)(
     stmt->ob_kind==DeeStmtAstKind_PRINT_NOLF
     ? (stmt->ob_print.ob_prt_file ? OP_PRINT_F_NOLF : OP_PRINT_NOLF)
     : (stmt->ob_print.ob_prt_file ? OP_PRINT_F : OP_PRINT)),
     (Dee_uint16_t)print_size) == -1) return -1;
    writer->ob_stack_size -= print_size;
    if (stmt->ob_print.ob_prt_file && // Pop the file
        DeeCodeWriter_Pop(writer) == -1) return -1;
   }
#endif
  } break;


  case DeeStmtAstKind_IF: {
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   int lhs_noreturn = 0;
   int rhs_noreturn = 0;
#endif
   // TODO: We can optimize the jumps, if the 'then' or 'else'
   //       block only contains a 'goto' statement
   // Compile the if conditional expression
   if ((temp = DeeExprAst_Compile(
    (DeeObject *)stmt->ob_if.ob_if_expr,
    (DeeObject *)stmt->ob_if.ob_if_thenscope,
    writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp != 0) { DeeCodeWriter_DECSTACK(writer); return temp; }
#endif
   pos1 = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP_IF_FF_POP,&future_arg_1) == -1) return -1;
   DeeCodeWriter_DECSTACK(writer);
   // Write the then (true) branch
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if ((lhs_noreturn = DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_if.ob_if_thenstmt,
    (DeeObject *)stmt->ob_if.ob_if_thenscope,writer,
    flags&~(DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP))) == -1) return -1;
   if (lhs_noreturn == 0)
#else
   if (DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_if.ob_if_thenstmt,
    (DeeObject *)stmt->ob_if.ob_if_thenscope,writer,
    lexer,flags&~(DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)) == -1) return -1;
#endif
   {
    if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_if.ob_if_thenscope,lexer) == -1) return -1;
   }
   if (stmt->ob_if.ob_if_elsestmt || ret_used) {
    pos2 = DeeCodeWriter_ADDR(writer); // setup the jump if the check succeeds
    if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP,&future_arg_2) == -1) return -1;
    // if false, we jump after the jump instruction that skips the false branch
    DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos1));
    // Write the else (false) branch
    if (stmt->ob_if.ob_if_elsestmt) {
     // else branch exists
     DEE_ASSERT(stmt->ob_if.ob_if_elsescope);
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
     if ((rhs_noreturn = DeeStmtAst_Compile(
      (DeeObject *)stmt->ob_if.ob_if_elsestmt,
      (DeeObject *)stmt->ob_if.ob_if_elsescope,writer,
      flags&~(DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP))) == -1) return -1;
     if (rhs_noreturn == 0)
#else
     if (DeeStmtAst_Compile(
      (DeeObject *)stmt->ob_if.ob_if_elsestmt,
      (DeeObject *)stmt->ob_if.ob_if_elsescope,writer,
      lexer,flags&~(DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)) == -1) return -1;
#endif
     {
      if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_if.ob_if_elsescope,lexer) == -1) return -1;
     }
    } else {
     DEE_ASSERT(ret_used);
     // In case of a missing else branch and a requested return value,
     // we use 'none' at that return value.
     if (DeeCodeWriter_PushNone(writer) == -1) return -1;
    }
    DeeCodeWriter_SetFutureArg(writer,future_arg_2,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos2));
    if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
   } else {
    // Then block is skipped (we end up after the expression)
    DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos1));
   }
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (lhs_noreturn == 0 || rhs_noreturn == 0) return 0; // Either or do actually return
   if (lhs_noreturn == 2 || rhs_noreturn == 2) return 2; // Either or have a break/continue
   return 1; // Either or have a throw/return
#endif
  } break;

  // generated code:
  //{
  //                  [init]
  //                  cont:
  // if_exists(done): if ([done]) goto end;
  //                  [block]
  //                  [next]
  //                  goto cont;
  // if_exists(done): end:
  //}
  case DeeStmtAstKind_FOR: {
   if (stmt->ob_for.ob_f_forinit) { // initializer
    if ((temp = DeeExprAst_Compile(
     (DeeObject *)stmt->ob_for.ob_f_forinit,
     (DeeObject *)stmt->ob_for.ob_f_scope,
     writer,lexer,flags&~DEE_COMPILER_FLAG_USED
     )) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
    if (temp != 0) { if (ret_used) DeeCodeWriter_INCSTACK(writer); return temp; }
#endif
   }
   // setup the 'cont' label with 'pos1'
   pos1 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   if (stmt->ob_for.ob_f_forcont) {
    // setup the jump to 'end' if 'done' is true
    if ((temp = DeeExprAst_Compile(
     (DeeObject *)stmt->ob_for.ob_f_forcont,
     (DeeObject *)stmt->ob_for.ob_f_scope,
     writer,lexer,flags|DEE_COMPILER_FLAG_USED
     )) == -1) goto err_for;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
    if (temp == 1) return 1;       // Condition has a return/throws
    if (temp == 2) goto push_none; // Condition has a break/continue
#endif
    pos2 = DeeCodeWriter_ADDR(writer); // required for origin of 'goto end'
    if (DeeCodeWriter_WriteOpWithFutureArg(writer,
     OP_JUMP_IF_FF_POP,&future_arg_1) == -1) goto err_for;
    DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
   }
   if ((temp = DeeStmtAst_Compile( // Compile the for [block]
    (DeeObject *)stmt->ob_for.ob_f_block,
    (DeeObject *)stmt->ob_for.ob_f_scope,
    writer,lexer,flags&~(DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)
    )) == -1) goto err_for;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   // TODO
#endif
   if (stmt->ob_for.ob_f_fornext) {
    if (DeeExprAst_Compile(// Compile the for [next]
     (DeeObject *)stmt->ob_for.ob_f_fornext,
     (DeeObject *)stmt->ob_for.ob_f_scope,
     writer,lexer,flags&~DEE_COMPILER_FLAG_USED) == -1) goto err_for;
   }
   // Jump back to 'cont'
   if (DeeCodeWriter_WriteOpWithArg(writer,OP_JUMP,(Dee_uint16_t)(
    -(Dee_int16_t)(DeeCodeWriter_ADDR(writer)-pos1))) == -1) {
err_for:
    DeeCodeWriter_FAIL_LOOP(writer);
    return -1;
   }
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
   if (stmt->ob_for.ob_f_forcont) {
    // This is where we jump if 'done' is true
    DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos2));
   }
   // Cleanup the for scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_for.ob_f_scope,lexer) == -1) return -1;
  } goto push_none;



  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH: {
   // Compile+Push the list expression to loop
   if ((temp = DeeExprAst_Compile(
    (DeeObject *)stmt->ob_for_in.ob_fi_iterlist,
    (DeeObject *)stmt->ob_for_in.ob_fi_scope,
    writer,lexer,flags|DEE_COMPILER_FLAG_USED)) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp != 0) { if (!ret_used) DeeCodeWriter_DECSTACK(writer); return temp; }
#endif
   if (stmt->ob_kind != DeeStmtAstKind_FOREACH) {
    // Create an iterator for the list
    if (DeeCodeWriter_UnaryOp(writer,OP_SEQ_ITER_SELF) == -1) return -1;
   }
   // Setup the jump and iter_next operator
   pos1 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_SEQ_ITER_NEXT,&future_arg_1) == -1) goto err_for_in;
   // If we don't jump, we iterate the block
   DeeCodeWriter_INCSTACK(writer); // notify of the existence of the pushed value
   // Store the iterator value in the given local variable
   if (DeeCodeWriter_StoreLocalPop(writer,
    (DeeObject *)stmt->ob_for_in.ob_fi_itername,
    have_local_names?lexer:NULL
    ) == -1) goto err_for_in;
   // Now lets execute the block
   if ((temp = DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_for_in.ob_fi_block,
    (DeeObject *)stmt->ob_for_in.ob_fi_scope,writer,lexer,flags&~(
    DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)
    )) == -1) goto err_for_in;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp == 0) // Don't jump back if the block doesn't return
#endif
   {// Jump back to collect the next iterator value
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_JUMP,(Dee_uint16_t)(
     -(Dee_int16_t)(DeeCodeWriter_ADDR(writer)-pos1))) == -1) {
err_for_in:
     DeeCodeWriter_FAIL_LOOP(writer);
     return -1;
    }
   }
   // This is where the jump leads if the for-in iterator is done (after the block jump)
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
   DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos1));
   // Cleanup the for scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_for_in.ob_fi_scope,lexer) == -1) return -1;
   // Finally pop the finished iterator from the stack (if we aren't supposed to return a value)
   if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
  } break;



  case DeeStmtAstKind_WHILE:
   // Setup the jump back to the condition check
   // NOTE: This is where 'continue' jump to!
   pos1 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   if ((temp = DeeExprAst_Compile(
    (DeeObject *)stmt->ob_while.ob_w_loopcond,
    (DeeObject *)stmt->ob_while.ob_w_scope,writer,
    lexer,flags|DEE_COMPILER_FLAG_USED)) == -1)  goto err_while;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp != 0) { if (!ret_used) DeeCodeWriter_DECSTACK(writer); return temp; }
#endif
   // Skip the block if the condition fails
   pos2 = DeeCodeWriter_ADDR(writer);
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,
    OP_JUMP_IF_FF_POP,&future_arg_1) == -1) goto err_while;
   DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
   // Compile the block
   if ((temp = DeeStmtAst_Compile((DeeObject *)stmt->ob_while.ob_w_block,(DeeObject *)stmt->ob_while.ob_w_scope,
    writer,lexer,flags&~(DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP))) == -1) goto err_while;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp == 0) // Don't jump back if the block doesn't return
#endif
   {// Jump back to the condition
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_JUMP,(Dee_uint16_t)(
     -(Dee_int16_t)(DeeCodeWriter_ADDR(writer)-pos1))) == -1) {
err_while:
     DeeCodeWriter_FAIL_LOOP(writer);
     return -1;
    }
   }
   // This is where we jump if the condition fails or break is encountered
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
   DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos2));
   // Cleanup the loop scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_while.ob_w_scope,lexer) == -1) return -1;
   goto push_none;



  case DeeStmtAstKind_LOOP_ONCE: {
   // Setup the jump back to the condition checks out
   pos1 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   // Compile the block
   if ((temp = DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_loop_once.ob_lonce_block,
    (DeeObject *)stmt->ob_loop_once.ob_lonce_scope,
    writer,lexer,flags&~(DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)
    )) == -1) { DeeCodeWriter_FAIL_LOOP(writer); return -1; }
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp != 0) { if (ret_used) DeeCodeWriter_INCSTACK(writer); return temp; }
#endif
   // Cleanup the loop scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_loop_once.ob_lonce_scope,lexer) == -1) return -1;
  } goto push_none;


  case DeeStmtAstKind_DO_WHILE: {
   // Setup the jump back to the condition checks out
   pos1 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   // Compile the block
   if ((temp = DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_do_while.ob_dw_block,
    (DeeObject *)stmt->ob_do_while.ob_dw_scope,writer,
    lexer,flags&~(DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)
    )) == -1) goto err_do_while;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   if (temp == 1) { if (ret_used) DeeCodeWriter_INCSTACK(writer); return 1; } // Block throws
   if (temp != 2) // Don't compile the condition if the block contains break/continue
#endif
   {
    if (DeeExprAst_Compile((DeeObject *)stmt->ob_do_while.ob_dw_loopcond,
     (DeeObject *)stmt->ob_do_while.ob_dw_scope,writer,lexer,
     flags|DEE_COMPILER_FLAG_USED) == -1) goto err_do_while;
    // Only jump back if the condition prevails
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_JUMP_IF_TT_POP,
     (Dee_uint16_t)(pos1-DeeCodeWriter_ADDR(writer))) == -1) {
err_do_while:
     DeeCodeWriter_FAIL_LOOP(writer);
     return -1;
    }
    DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_TT_POP' popped this one
   }
   // This is where we end up if the condition fails
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
   // Cleanup the loop scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_do_while.ob_dw_scope,lexer) == -1) return -1;
  } goto push_none;


  case DeeStmtAstKind_GOTO: {
   DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
   if (DeeCodeWriter_GotoLabel(writer,
    &weak_root->ob_labels.ob_v[stmt->ob_goto.ob_gt_label_id].ob_decl,
    lexer,(DeeObject *)stmt->ob_token) == -1) return -1;
   // Just like return, this statement is 'noreturn'
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   goto on_noreturn;
#else
   goto push_none;
#endif
  }

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *begin,*end;
   end = (begin = stmt->ob_try.ob_try_handlerv)+stmt->ob_try.ob_try_handlerc;
   // Setup all the handler blocks (in reverse order)
   while (end != begin) if (DeeCodeWriter_TryBegin(writer,&(--end)->ob_runtime_id) == -1) return -1;
   end = begin+stmt->ob_try.ob_try_handlerc;
   // Compile the protected code
   if (DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_try.ob_try_block,
    (DeeObject *)stmt->ob_try.ob_try_scope,
    writer,lexer,(flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~DEE_COMPILER_FLAG_USED) == -1 ||
    DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_try.ob_try_scope,lexer) == -1
    ) return -1;
   // End all the exception handler blocks
   begin = stmt->ob_try.ob_try_handlerv;
   while (begin++ != end) if (DeeCodeWriter_TryEnd(writer) == -1) return -1;
   // Setup the jump across the handler code blocks
   pos1 = DeeCodeWriter_ADDR(writer);
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP,&future_arg_1) == -1) return -1;
   begin = stmt->ob_try.ob_try_handlerv;
   if (begin != end) while (1) {
    // Initialize the exception headers + compile the handler code
    int is_finally;
    struct DeeExceptionHandlerEntry *rt_entry;
    rt_entry = writer->ob_exceptv+begin->ob_runtime_id;
    rt_entry->e_kind = begin->ob_kind;
    Dee_XINCREF(rt_entry->e_type = begin->ob_type);
    if (begin->ob_local) {
     // Local variable to store the exception within
     if ((begin->ob_local->ob_flags&DEE_LOCALVAR_FLAG_INITIALIZED) == 0) {
      if ((begin->ob_local->ob_loc_id = DeeCodeWriter_AllocLocal(
       writer,have_local_names?TPPLexer_TokenIDStr(
       DeeLexer_LEXER(lexer),begin->ob_local->ob_name):NULL)
       ) == (Dee_uint16_t)-1) return -1;
      begin->ob_local->ob_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
     }
     rt_entry->e_store = begin->ob_local->ob_loc_id;
    }
    // Align the stack to the current stack size
    rt_entry->e_stack = (Dee_uint16_t)writer->ob_stack_size;
    rt_entry->e_handler = DeeCodeWriter_ADDR(writer);
    // Header is now initialized
    // Update finally recursion of the entire code
    is_finally = begin->ob_kind == DeeExceptionHandleKind_FINALLY;
    // >> Compile the actual handler code
    if (is_finally && writer->ob_finally_rec++ == writer->ob_min_finally_rec)
     writer->ob_min_finally_rec = writer->ob_finally_rec;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NOEXCEPT_OPTIMIZATIONS
    if (DeeStmtAst_IsNoexcept((DeeObject *)begin->ob_block,(DeeObject *)begin->ob_scope) &&
       !DeeStmtAst_IsNoreturn((DeeObject *)begin->ob_block,(DeeObject *)begin->ob_scope,
        DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS)) {
     if (DeeStmtAst_Compile((DeeObject *)begin->ob_block,(DeeObject *)begin->ob_scope,
      writer,lexer,(flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~DEE_COMPILER_FLAG_USED) == -1) return -1;
     if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)begin->ob_scope,lexer) == -1) return -1;
     if (DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(is_finally ? OP_FINALLY_END : OP_EXCEPT_END)) == -1) return -1;
    } else
#endif
    {
     Dee_uint16_t handler_finally_id;
     Dee_size_t handler_finally_jmp;
     // This is a bit complex, but we need a finally block to
     // cleanup our handler (even if he's a finally block himself...)
     if (writer->ob_finally_rec++ == writer->ob_min_finally_rec)
      writer->ob_min_finally_rec = writer->ob_finally_rec;
     if (DeeCodeWriter_TryBegin(writer,&handler_finally_id) == -1) return -1;;
     // We need a small finally block across the catch handler, so we
     // can make sure to always remove the exception from the list of handled exceptions
     if (DeeStmtAst_Compile((DeeObject *)begin->ob_block,(DeeObject *)begin->ob_scope,
      writer,lexer,(flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~DEE_COMPILER_FLAG_USED) == -1) return -1;
     if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)begin->ob_scope,lexer) == -1) return -1;
     // End the try block and initialize the finally block at the end of the catch
     if (DeeCodeWriter_TryEnd(writer) == -1) return -1;
     // Skip the finally block once try_end returned
     if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP,&handler_finally_jmp) == -1) return -1;
     rt_entry = writer->ob_exceptv+handler_finally_id;
     rt_entry->e_type = NULL;
     rt_entry->e_kind = DeeExceptionHandleKind_FINALLY;
     rt_entry->e_stack = (Dee_uint16_t)writer->ob_stack_size;
     rt_entry->e_handler = DeeCodeWriter_ADDR(writer);
     if (is_finally) {
      if (DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) == -1) return -1;
      DeeCodeWriter_SetFutureArg(writer,handler_finally_jmp,
       (Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-(handler_finally_jmp-1)));
      if (DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) == -1) return -1;
     } else {
      if (DeeCodeWriter_WriteOp(writer,OP_EXCEPT_END) == -1 ||
          DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) == -1) return -1;
      DeeCodeWriter_SetFutureArg(writer,handler_finally_jmp,
       (Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-(handler_finally_jmp-1)));
     }
     --writer->ob_finally_rec;
    }
    if (is_finally) --writer->ob_finally_rec;
    if (++begin == end) break; else {
     // Jump to the end of all handlers
     if (DeeCodeWriter_WriteOpWithFutureArg(
      writer,OP_JUMP,&begin[-1].ob_runtime_jmp) == -1) return -1;
    }
   }
   // This is where we jump to if no exception occurred
   pos2 = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(pos2-pos1));
   begin = stmt->ob_try.ob_try_handlerv;
   // Initialize all the jumps from the exception handlers to the end
   if (begin != end) {
    --end;
    while (begin != end) {
     // Calcuate the jumps from the end of the handlers
     *(Dee_uint16_t *)(writer->ob_code_begin+begin->ob_runtime_jmp) = (
      Dee_uint16_t)(pos2-(begin->ob_runtime_jmp-1));
     ++begin;
    }
   }
  } goto push_none;

  case DeeStmtAstKind_THROW:
   if (DeeExprAst_Compile((DeeObject *)stmt->ob_throw.ob_trw_expr,scope,
    writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
    DeeCodeWriter_WriteOp(writer,OP_THROW) == -1
    ) return -1;
   DeeCodeWriter_DECSTACK(writer); // consumed by 'OP_THROW'
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   goto on_noreturn;
#else
   goto push_none;
#endif

  case DeeStmtAstKind_RETHROW:
   if (DeeCodeWriter_WriteOp(writer,OP_RETHROW) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_EXPERIMENTAL_NORETURN_OPTIMIZATIONS
   goto on_noreturn;
#else
   goto push_none;
#endif
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

  case DeeStmtAstKind_SWITCH: {
   struct DeeLabelDecl *default_label;
   struct DeeCaseLabelDecl *case_iter,*case_end;
   struct DeeCaseLabelDeclList *old_cases;
   DeeScopeObject *weak_root;
   pos1 = DeeCodeWriter_ADDR(writer);
   // Compile the switch expression
   if (DeeExprAst_Compile(
    (DeeObject *)stmt->ob_switch.ob_sw_expr,
    (DeeObject *)stmt->ob_switch.ob_sw_scope,
    writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
   default_label = NULL;
   case_end = (case_iter = stmt->ob_switch.ob_sw_cases.ob_v)+stmt->ob_switch.ob_sw_cases.ob_c;
   // Go through all the labels and compile the forward jumps into the switch block
   while (case_iter != case_end) {
    if (case_iter->ob_case) {
     if (DeeCodeWriter_PushConst(writer,case_iter->ob_case) == -1) return -1;
     if (case_iter->ob_case_end) {
      if (DeeCodeWriter_PushConst(writer,case_iter->ob_case_end) == -1) return -1;
      DeeCodeWriter_DECSTACK(writer); // Will be consumed by OP_JUMP_IF_RCASE_COMPARE
     }
     DeeCodeWriter_DECSTACK(writer); // Will be consumed by OP_JUMP_IF_CASE_COMPARE
     if (_DeeCodeWriter_GotoForwardLabelEx(
      writer,&case_iter->ob_decl,(DeeObject *)stmt->ob_token,(Dee_uint16_t)(
      case_iter->ob_case_end ? OP_JUMP_IF_RCASE_COMPARE : OP_JUMP_IF_CASE_COMPARE)) == -1) return -1;
    } else {
     DEE_ASSERT(!default_label && "Default label already set");
     default_label = &case_iter->ob_decl;
    }
    ++case_iter;
   }
   weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   old_cases = weak_root->ob_cases;
   weak_root->ob_cases = &stmt->ob_switch.ob_sw_cases;
   // Compile the fallback label
   if (default_label) { // User-defined default label
    if (DeeCodeWriter_GotoLabel(writer,default_label,lexer,(DeeObject *)stmt->ob_token) == -1) {
err_switch_loop:
     DeeCodeWriter_FAIL_LOOP(writer);
     weak_root->ob_cases = old_cases;
     return -1;
    }
   } else { // Compiler-generated default label (jump over the block)
    if (DeeCodeWriter_Break(writer) == -1) goto err_switch_loop;
   }
   if (DeeStmtAst_Compile(
    (DeeObject *)stmt->ob_switch.ob_sw_stmt,
    (DeeObject *)stmt->ob_switch.ob_sw_scope,writer,lexer,
    (flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~(DEE_COMPILER_FLAG_USED)) == -1) goto err_switch_loop;
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer));
   weak_root->ob_cases = old_cases;
   // Cleanup the all labels
   case_iter = stmt->ob_switch.ob_sw_cases.ob_v;
   while (case_iter != case_end) {
    if (DeeCodeWriter_FinalizeLabel(writer,&case_iter->ob_decl,lexer) == -1) return -1;
    ++case_iter;
   }
   // Cleanup the switch scope
   if (DeeCodeWriter_FinalizeScope(writer,(DeeObject *)stmt->ob_switch.ob_sw_scope,lexer) == -1) return -1;
   // Pop the switch condition expression
   if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
  } break;

  case DeeStmtAstKind_EXTERN: {
   if (ret_used || stmt->ob_extern.ob_extern_store->ob_uses != 0) {
    if (DeeCodeWriter_PushExtern(writer,&stmt->ob_extern.ob_extern_decl) == -1) return -1;
    // Only store the decl if the variable is actually used
    // - If it isn't we optimize cases like:
    //   >> ({ extern [[__stdcall__]] "Kernel32.dll": none OutputDebugStringA(string); })("Hello!\n");
    if (stmt->ob_extern.ob_extern_store->ob_uses) {
     if (ret_used
         ? DeeCodeWriter_StoreLocal(writer,(DeeObject *)stmt->ob_extern.ob_extern_store,have_local_names?lexer:NULL) == -1
         : DeeCodeWriter_StoreLocalPop(writer,(DeeObject *)stmt->ob_extern.ob_extern_store,have_local_names?lexer:NULL) == -1
         ) return -1;
    } else {
     // The following fallback code should never be required!
     if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
    }
   }
  } break;

  default: DEE_BUILTIN_UNREACHABLE();
 }
#undef PUSH_UNREACHABLE
#undef have_local_names
#undef ret_used
#ifdef stmt
#undef stmt
#endif
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_STMT_AST_COMPILE_C */
