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
#ifndef GUARD_DEEMON_COMPILER_XAST_OPTIMIZE_INL
#define GUARD_DEEMON_COMPILER_XAST_OPTIMIZE_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/xast.h>
#include <deemon/none.h>

#define VLOG_OPTIMIZE(tk,...) (\
 DEE_LVERBOSE1R("%s(%d) : %k : OPTIMIZE : ",\
 DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk),\
 DEE_LVERBOSE1R(__VA_ARGS__))

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXAst_Optimize(
 DEE_A_INOUT DeeXAstObject *self, DEE_OPTIMIZE_PARAMS) {
 int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 _DeeOptimizerAssumptions_EncounterXAst(assumptions,self,{ return -1 });
 DEE_ASSERT(assumptions->oas_scope == scope);
 switch (self->ast_kind) {
  
  case DEE_XASTKIND_VAR:
   // With assumptions, try to turn this variable into a constant
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_ASSUMPTIONS)!=0) {
    DeeObject *assumed_value;
    assumed_value = DeeOptimizerAssumptions_GetAssumedVarInit(assumptions,self->ast_var.vs_var);
    if (assumed_value) {
     // There is an assumption about the value of this variable,
     // meaning we can simply inline it as a constant.
     VLOG_OPTIMIZE(self->ast_common.ast_token,
                   "Transforming lookup of variable with known value into constant %r\n",
                   assumed_value);
     DeeXAst_AssignConst(self,assumed_value);
     ++*performed_optimizations;
    }
   }
   break;

  case DEE_XASTKIND_VARDECL: {
   int have_initializer_type;
   DeeTypeObject const *initializer_type = NULL;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_vardecl.vd_init,
    DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_ASSUMPTIONS)!=0) {
    if (self->ast_vardecl.vd_init->ast_kind == DEE_XASTKIND_CONST) {
     DeeObject *constant_initializer;
     constant_initializer = self->ast_vardecl.vd_init->ast_const.c_const;
     initializer_type = Dee_TYPE(constant_initializer);
     have_initializer_type = 1;
     // Notify about a constant initialization, as long as the value is immutable
     // -> If it isn't, only notify about the type
     // NOTE: If the variable is only used once, we can always just assume its absolute value
     // We know everything there is to know about the variable,
     // except for the ways in which it is used.
     // So these precautions are done to not break code like:
     // >> local i = 10;
     // >> i += 20; // Without this precaution, we would 
     // >> print i; // Must be 30
     // TODO: We can't even put immutable types just like that.
     //       Strings are immutable, but due to the work-around used
     //       to get inplace operators working with traceable slots,
     //       code like this...:
     //       >> local x = "foo";
     //       >> x += "bar"; // Can't inline '"foo"' even though it's immutable
     //       >> print x;
     //       ... would otherwise be broken.
     //       The todo in this is to track how a variable is used more in-depth.
     //       Mainly tracking whether or not it's being used in an inplace-operation.
     // TODO: What we'd really need here is some way of tracking how often a variable
     //       is used as the left-hand-side operand of a binary inplace-expression.
     //       And only if it's never used in such a place, and if its type is immutable,
     //       as indicated by 'DeeType_IsImmutable', then we can allow for constant
     //       replacement during optimization more than once.
     //      (NOTE: This effects: ++<x>,<x>++,--<x>,<x>--,+=,-=,*=,/=,%=,**=,<<=,>>=,&=,|=,^=)
     //       REMINDER: The compiler is allowed to substitute '++x' with
     //                 'x += 1', before replacing that with 'x = x+1'.
     if (/*DeeType_IsImmutable(initializer_type) ||*/
         DeeLocalVar_GET_USES(self->ast_vardecl.vd_var) == 1) {
      error = DeeOptimizerAssumptions_AddAssumeInit(
       assumptions,self->ast_vardecl.vd_var,constant_initializer);
      if DEE_UNLIKELY(error == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ERROR) return -1;
      if (error == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_MATCHOLD) {
       // This vardecl is redundant:
       // >> {
       // >>   local x = "foobar";
       // >>   if (bar()) {
       // >>     x = "foobar"; //< This is us
       // >>   }
       // >> }
       VLOG_OPTIMIZE(self->ast_vardecl.vd_token,
                     "Removing redundant var declaration with %r\n",
                     constant_initializer);
       // Move the initializer, thus still keeping the semantical
       // meaning of the decl if its result is used.
       // NOTE: Same-constant/noeffect-expr optimization will pick up on this later...
       DeeXAst_AssignMove(self,self->ast_vardecl.vd_init);
       ++*performed_optimizations;
       break;
      }
     } else {
      if DEE_UNLIKELY(DeeOptimizerAssumptions_AddAssumeType(
       assumptions,self->ast_vardecl.vd_var,initializer_type) ==
       DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ERROR) return -1;
     }
    } else {
     // Try to predict the type and use register it as an assumption
     initializer_type = DeeXAst_PredictType(self->ast_vardecl.vd_init);
     have_initializer_type = 1;
     if (initializer_type) {
      if DEE_UNLIKELY(DeeOptimizerAssumptions_AddAssumeType(
       assumptions,self->ast_vardecl.vd_var,initializer_type) ==
       DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ERROR) return -1;
     } else {
      // Must delete any previous assumptions
      if DEE_UNLIKELY(DeeOptimizerAssumptions_DelVarAssumptions(
       assumptions,self->ast_vardecl.vd_var) != 0) return -1;
     }
    }
   } else {
    have_initializer_type = 0;
   }

#if 1
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_UNUSED_VAR)!=0 &&
       (self->ast_vardecl.vd_var->lv_flags&DEE_LOCALVAR_FLAG_USED)==0) {
    Dee_size_t actual_uses = self->ast_vardecl.vd_var->lv_uses;
    // Self-references don't count if the initialized value is used.
    // Because in that situation, we can't optimize away the declaration, as it is indirectly being used:
    // >> recursive;
    // >> (function recursive(n) { // Can't optimize away the declaration, ...
    // >>   if (n) recursive(n-1);
    // >>   print n;
    // >> })(10); // ... because it is indirectly being used here
    if ((optimize_flags&DEE_OPTIMIZE_FLAG_USED)==0) {
     switch (self->ast_vardecl.vd_init->ast_kind) {
      // Object types that are allow to perform self-references
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
      case DEE_XASTKIND_CLASS:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
      case DEE_XASTKIND_FUNCTION:
      case DEE_XASTKIND_LIST:
      case DEE_XASTKIND_SET:
      case DEE_XASTKIND_DICT:
      case DEE_XASTKIND_CELL: {
       // Subtract self-references
#ifdef DEE_DEBUG
       Dee_size_t self_refs = DeeXAst_CountVariableUses(self->ast_vardecl.vd_init,self->ast_vardecl.vd_var);
       DEE_ASSERTF(actual_uses >= self_refs,"Initializer accounts for too many self-references "
                   "(%Iu exceeds maximum of %Iu)",self_refs,actual_uses);
       actual_uses -= self_refs;
#else
       actual_uses -= DeeXAst_CountVariableUses(self->ast_vardecl.vd_init,self->ast_vardecl.vd_var);
#endif
      } break;
      default: break;
     }
    } else if (!actual_uses && self->ast_vardecl.vd_var->lv_init == 1 &&
               self->ast_vardecl.vd_var->lv_scope == scope) {
     // The variable is being used once and is only initialized once
     // >> The use is the current expression, meaning we can safely remove the variable declaration
     // Optimizes something like this: 'print ({ x = 42; })' --> 'print ({ 42; })'
     // NOTE: The scope check is required to keep the variable assigned in this:
     // >> forward local x;
     // >> print ({ x = foo(); });
     // >> ... // code that never uses 'x'
     // -> Since 'foo()' could return something with a lifetime effect, we
     //    can't optimize the var declaration away in that case.
     // >> But if 'x' belonged to the scope in which 'foo' is called, meaning
     //    it wasn't forward declared in line #1, it could be optimized away.
     VLOG_OPTIMIZE(self->ast_vardecl.vd_token,
                   "Removing declaration of variable only used once locally\n");
     goto vardecl_assign_init_branch;
    }
    if (actual_uses) break; // Variable is being used
    if ((self->ast_vardecl.vd_var->lv_flags&DEE_LOCALVAR_FLAG_UNUSED)!=0) {
     DeeLocalVarObject *unused_var;
vardecl_assign_none_or_init_branch:
     // Remove this variable declaration
     VLOG_OPTIMIZE(self->ast_vardecl.vd_token,
                   "Removing unused variable from vardecl\n");
     if ((self->ast_vardecl.vd_var->lv_flags&DEE_LOCALVAR_FLAG_UNUSED)!=0) {
      Dee_INCREF(unused_var = self->ast_vardecl.vd_var);
      // Delete the initializer for [[unused]] variables
      // >> That is why dex imports are always marked as unused.
      //    Because that way, the initializer (aka. dex lookup)
      //    can be optimized away, too.
      // CAUTION: Don't mark something as unused, when you
      //          depend on the side-effects of its initializer.
      //          And don't rely on the fact that an [[unused]] variable
      //          will be optimized away if it is really never used.
      DeeXAst_AssignConst(self,Dee_None);
     } else {
vardecl_assign_init_branch:
      Dee_INCREF(unused_var = self->ast_vardecl.vd_var);
      DeeXAst_AssignMove(self,self->ast_vardecl.vd_init);
     }
     // Once no one is initializing the variable anymore, remove it from its scope
     if (!unused_var->lv_init) _DeeScope_DelName(unused_var->lv_scope,unused_var->lv_name);
     Dee_DECREF(unused_var);
     ++*performed_optimizations;
    } else {
     if (!have_initializer_type) initializer_type = DeeXAst_PredictType(self->ast_vardecl.vd_init);
     if (initializer_type && (DeeType_FLAGS(initializer_type)&DEE_TYPE_FLAG_NO_LIFETIME_EFFECT)!=0)
      goto vardecl_assign_none_or_init_branch;
     // Last chance: If the initializer doesn't have any side-effects,
     //              we can optimize away this variable, too.
     //              >> This optimizes away something like: 'x = [10,20,30];'
     if (DeeXAst_IsNoEffect(self->ast_vardecl.vd_init))
      goto vardecl_assign_none_or_init_branch;
     VLOG_OPTIMIZE(self->ast_vardecl.vd_token,
                   "Can't remove unused variable from vardecl\n");
    }
   }
#endif
  } break;

  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   do if DEE_UNLIKELY(DeeVarDeclStorage_Optimize(iter,
    DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_NONE,DEE_OPTIMIZE_ARGS) != 0) return -1;
   while (++iter != end);
   return DeeXAst_Optimize(self->ast_multivardecl.mvd_init,
                           DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED));
  } break;

  case DEE_XASTKIND_STATEMENT: {
   if DEE_UNLIKELY(DeeSAst_Optimize(self->ast_statement.s_stmt,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0) {
    DeeSAstObject *statement = self->ast_statement.s_stmt;
    if (DeeParserLabelRefList_EMPTY(&statement->ast_common.ast_labels)) {
     switch (statement->ast_kind) {

      case DEE_SASTKIND_EMPTY: {
       // Optimize empty statement --> none constant
       VLOG_OPTIMIZE(statement->ast_common.ast_token,
                     "Transforming empty statement into none-constant\n");
       DeeXAst_AssignConst(self,Dee_None);
       ++*performed_optimizations;
      } break;

      case DEE_SASTKIND_IF: {
       // Optimize: '({ if (x) { y; } else { z; } })' --> 'x ? y : z'
       DeeXAstObject *if_ast;
       VLOG_OPTIMIZE(statement->ast_common.ast_token,
                     "Transforming if statment to if expression\n");
       if DEE_UNLIKELY((if_ast = DeeXAst_NewIfFromIfSAst(
        self->ast_common.ast_token,statement,lexer,
        config->parser_flags)) == NULL) return -1;
       DeeXAst_AssignMove(self,if_ast);
       Dee_DECREF(if_ast);
       ++*performed_optimizations;
      } break;

      case DEE_SASTKIND_IFTRUE:
      case DEE_SASTKIND_IFFALSE: {
       // Optimize:  '__if_true { x; } else { y; }' --> '/* constant */true ? x : y'
       // Optimize: '__if_false { x; } else { y; }' --> '/* constant */false ? x : y'
       DeeXAstObject *if_ast;
       VLOG_OPTIMIZE(statement->ast_common.ast_token,
                     "Transforming constant if statment to constant if expression\n");
       if DEE_UNLIKELY((if_ast = DeeXAst_NewIfConstFromIfConstSAst((DeeXAstKind)(
        statement->ast_kind == DEE_SASTKIND_IFTRUE ? DEE_XASTKIND_IFTRUE : DEE_XASTKIND_IFFALSE),
        statement->ast_common.ast_token,
        statement->ast_ifconst.ic_succ_block,statement->ast_ifconst.ic_succ_scope,
        statement->ast_ifconst.ic_fail_block,statement->ast_ifconst.ic_fail_scope,
        lexer,config->parser_flags)) == NULL) return -1;
       DeeXAst_AssignMove(self,if_ast);
       Dee_DECREF(if_ast);
       ++*performed_optimizations;
      } break;

      case DEE_SASTKIND_EXPRESSION: {
       // Optimize '({ 42; })' --> '42'
       VLOG_OPTIMIZE(statement->ast_common.ast_token,
                     "Transforming expression in expr-statement to expression\n");
       DeeXAst_AssignMove(self,statement->ast_expression.x_ast);
       ++*performed_optimizations;
      } break;

      default: break;
     }
    }
   }
  } break;

  case DEE_XASTKIND_IF:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_if.if_cond,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(self->ast_if.if_succ && DeeXAst_Optimize(self->ast_if.if_succ,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(self->ast_if.if_fail && DeeXAst_Optimize(self->ast_if.if_fail,DEE_OPTIMIZE_ARGS) != 0) return -1;
   // TODO: Optimize: 'v ? ({ x = 10; }) : ({ x = 20; })' -> 'x = v ? 10 : 20;'
   // TODO: Optimize: 'v ? ({ x += 10; }) : ({ x += 20; })' -> 'x += v ? 10 : 20;' (Only if 'x' doesn't have any side-effects)
   // TODO: Optimize: 'v ? ({ yield 10; }) : ({ yield 20; })' --> '({ yield v ? 10 : 20; })'
   // TODO: Optimize: 'v ? ({ print 10; }) : ({ print 20; })' --> '({ print v ? 10 : 20; })'
   // TODO: Optimize: 'v ? ({ assert 10; }) : ({ assert 20; })' --> '({ print v ? 10 : 20; })'
   break;

  case DEE_XASTKIND_FUNCTION:
   if (!self->ast_function.f_scope) break;
  {
   struct DeeXAstFunctionAstArgument *iter,*end;
   DEE_ASSERT(self->ast_function.f_code != NULL);
   DeeScope_OPTIMIZE(self->ast_function.f_scope);
   end = (iter = self->ast_function.f_argv)+self->ast_function.f_argc;
   DEE_OPTIMIZE_SCOPE_ENTER_EX(function_assumptions,self->ast_function.f_scope,assumptions,
                               DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG) {
    while (iter != end) {
     if DEE_UNLIKELY(iter->a_default && DeeXAst_Optimize(iter->a_default,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_function.f_scope,&function_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
      DEE_OPTIMIZE_SCOPE_BREAK(function_assumptions);
      return -1;
     }
     ++iter;
    }
    error = DeeSAst_Optimize(self->ast_function.f_code,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_function.f_scope,&function_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED)));
   } DEE_OPTIMIZE_SCOPE_LEAVE(function_assumptions);
   if (error != 0) return error;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_CONST_FUNCTION)!=0) {
    if ((self->ast_function.f_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_REFS)==0) {
     // TODO: Generate a constant function
    } else {
     // TODO: Check if the function ~still~ has references
     //    >> Just like the doc for 'DEE_SCOPE_FLAG_FOUND_REFS' says:
     //       Even when that flag is self, it is possible that
     //       no references exist.
    }
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *iter,*end;
   if DEE_UNLIKELY(self->ast_class.c_base && DeeXAst_Optimize(self->ast_class.c_base,
    DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   while (iter != end) {
    switch (iter->ce_kind) {
     case DEE_XAST_CLASSENTRY_KIND_SLOT:
      if DEE_UNLIKELY(DeeXAst_Optimize(iter->ce_slot.cs_callback,
       DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
      break;
     case DEE_XAST_CLASSENTRY_KIND_METHOD:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
      if DEE_UNLIKELY(DeeXAst_Optimize(iter->ce_method.cm_callback,
       DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
      break;
     case DEE_XAST_CLASSENTRY_KIND_GETSET:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
      if DEE_UNLIKELY(iter->ce_getset.cg_get && DeeXAst_Optimize(
       iter->ce_getset.cg_get,DEE_OPTIMIZE_ARGS_EX(
       optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
      if DEE_UNLIKELY(iter->ce_getset.cg_del && DeeXAst_Optimize(
       iter->ce_getset.cg_del,DEE_OPTIMIZE_ARGS_EX(
       optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
      if DEE_UNLIKELY(iter->ce_getset.cg_set && DeeXAst_Optimize(
       iter->ce_getset.cg_set,DEE_OPTIMIZE_ARGS_EX(
       optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
      break;
     default: break;
    }
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **iter,**end;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   DEE_ASSERT(self->ast_sequence.s_elemc);
   do if DEE_UNLIKELY(DeeXAst_Optimize(*iter,DEE_OPTIMIZE_ARGS) != 0) return -1;
   while (++iter != end);
   if ((optimize_flags&(DEE_OPTIMIZE_FLAG_DEL_NOEFFECT|
    DEE_OPTIMIZE_FLAG_USED))==DEE_OPTIMIZE_FLAG_DEL_NOEFFECT) {
    end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
    DEE_ASSERT(self->ast_sequence.s_elemc);
    do {
     if (DeeXAst_IsNoEffect(*iter)) {
      VLOG_OPTIMIZE((*iter)->ast_common.ast_token,
                    "Removing ast without side-effects from unused sequence\n");
      Dee_DECREF(*iter);
      memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(DeeXAstObject *));
      --self->ast_sequence.s_elemc,--end;
      ++*performed_optimizations;
     } else ++iter;
    } while (iter != end);
    if (!self->ast_sequence.s_elemc) {
     DEE_ASSERT((optimize_flags&DEE_OPTIMIZE_FLAG_USED)==0);
     free(self->ast_sequence.s_elemv);
     self->ast_kind = DEE_XASTKIND_CONST;
     // Can use anything, because the constant isn't really used
     self->ast_const.c_const = DeeNone_New();
    } else if (self->ast_sequence.s_elemc == 1) {
     VLOG_OPTIMIZE(self->ast_sequence.s_elemv[0]->ast_common.ast_token,
                   "Inlining first item of unused single-element sequence\n");
     DeeXAst_AssignMove(self,self->ast_sequence.s_elemv[0]);
     ++*performed_optimizations;
    }
   }
   return 0;
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   do if DEE_UNLIKELY(DeeXAst_Optimize(iter->de_key,DEE_OPTIMIZE_ARGS) != 0 ||
                      DeeXAst_Optimize(iter->de_item,DEE_OPTIMIZE_ARGS) != 0) return -1;
   while (++iter != end);
   if ((optimize_flags&(DEE_OPTIMIZE_FLAG_DEL_NOEFFECT|
    DEE_OPTIMIZE_FLAG_USED))==DEE_OPTIMIZE_FLAG_DEL_NOEFFECT) {
    end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
    DEE_ASSERT(self->ast_dict.d_elemv != NULL);
    DEE_ASSERT(self->ast_dict.d_elemc != 0);
    do {
     int is_key_noeffect,is_item_noeffect;
     is_key_noeffect = DeeXAst_IsNoEffect(iter->de_key);
     is_item_noeffect = DeeXAst_IsNoEffect(iter->de_item);
     if (is_key_noeffect && is_item_noeffect) {
      VLOG_OPTIMIZE(iter->de_key->ast_common.ast_token,
                    "Removing dict entry without side-effects from unused dict\n");
      _DeeXAstDictEntry_Quit(iter);
      memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(struct DeeXAstDictEntry));
      --self->ast_dict.d_elemc,--end;
      ++*performed_optimizations;
     } else {
      if (is_key_noeffect) {
       VLOG_OPTIMIZE(iter->de_key->ast_common.ast_token,
                     "Removing key without side-effects from unused dict\n");
       DeeXAst_AssignConst(iter->de_key,Dee_None);
       ++*performed_optimizations;
      } else if (is_item_noeffect) {
       VLOG_OPTIMIZE(iter->de_item->ast_common.ast_token,
                     "Removing item without side-effects from unused dict\n");
       DeeXAst_AssignConst(iter->de_item,Dee_None);
       ++*performed_optimizations;
      }
      ++iter;
     }
    } while (iter != end);
    if (!self->ast_dict.d_elemc) {
     DEE_ASSERT((optimize_flags&DEE_OPTIMIZE_FLAG_USED)==0);
     DEE_ASSERT(self->ast_dict.d_elemv != NULL);
     free_nn(self->ast_dict.d_elemv);
     self->ast_kind = DEE_XASTKIND_CONST;
     // Can use anything, because the constant isn't really used
     self->ast_const.c_const = DeeNone_New();
    }
   }
   return 0;
  } break;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_seq_range_set.sr_value,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_seq_range.sr_seq,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(self->ast_seq_range.sr_begin && DeeXAst_Optimize(self->ast_seq_range.sr_begin,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(self->ast_seq_range.sr_end && DeeXAst_Optimize(self->ast_seq_range.sr_end,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   break;
  case DEE_XASTKIND_ATTR_SET_C:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_attr_set_c.ac_value,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
   return DeeXAst_Optimize(self->ast_attr_c.ac_object,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED));
  case DEE_XASTKIND_DEL_VAR:
   // Must delete all assumptions about the variable
   if DEE_UNLIKELY((optimize_flags&DEE_OPTIMIZE_FLAG_ASSUMPTIONS)!=0 &&
                   DeeOptimizerAssumptions_DelVarAssumptions(assumptions,self->ast_delvar.d_var) != 0
                   ) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_UNUSED_VAR)!=0 &&
       !self->ast_delvar.d_var->lv_init) {
    VLOG_OPTIMIZE(self->ast_common.ast_token,
                  "Removing uninitialized variable from del\n");
    DeeXAst_AssignConst(self,Dee_None);
    ++*performed_optimizations;
   }
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_foreignfunction.ff_base,DEE_OPTIMIZE_ARGS) != 0) return -1;
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) {
    if DEE_UNLIKELY(DeeXAst_Optimize(*iter,DEE_OPTIMIZE_ARGS) != 0) return -1;
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND: {
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_UNINIT_BOUND)!=0 &&
       !self->ast_builtin_bound.b_var->lv_init) {
    VLOG_OPTIMIZE(self->ast_builtin_bound.b_token,
                  "Removing never-initialized variable from __builtin_bound\n");
    DeeXAst_AssignConst(self,Dee_False);
    ++*performed_optimizations;
   } else if ((optimize_flags&DEE_OPTIMIZE_FLAG_ASSUMPTIONS)!=0) {
    // With assumptions enabled, we can predict whether or not a variable will be bound
    if (DeeOptimizerAssumptions_IsVarBound(assumptions,self->ast_builtin_bound.b_var)) {
     VLOG_OPTIMIZE(self->ast_builtin_bound.b_token,
                   "Removing always-initialized variable from __builtin_bound\n");
     DeeXAst_AssignConst(self,Dee_True);
     ++*performed_optimizations;
    }
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN: {
   if DEE_UNLIKELY(self->ast_builtin_extern.ex_file && DeeXAst_Optimize(
    self->ast_builtin_extern.ex_file,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_builtin_extern.ex_name,DEE_OPTIMIZE_ARGS) != 0) return -1;
   return DeeXAst_Optimize(self->ast_builtin_extern.ex_type,DEE_OPTIMIZE_ARGS);
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeXAst_Optimize(self->ast_builtin_expect.e_value,DEE_OPTIMIZE_ARGS);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  case DEE_XASTKIND_TYPE:
   // With assumptions, we might be able to predict a
   // type if it is referring to a local variable.
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_ASSUMPTIONS)!=0 &&
       self->ast_operator.op_a->ast_kind == DEE_XASTKIND_VAR) {
    DeeTypeObject *assumed_type;
    assumed_type = DeeOptimizerAssumptions_GetAssumedVarType(
     assumptions,self->ast_operator.op_a->ast_var.vs_var);
    if (assumed_type) {
     VLOG_OPTIMIZE(self->ast_common.ast_token,
                   "Managed to deduce type of local variable to be %q\n",
                   DeeType_NAME(assumed_type));
     DeeXAst_AssignConst(self,(DeeObject *)assumed_type);
     ++*performed_optimizations;
     break;
    }
   }
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_WEAK:
  case DEE_XASTKIND_SUPEROF:
  case DEE_XASTKIND_CLASSOF:
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XASTKIND_REF:
  case DEE_XASTKIND_DEREF:
  case DEE_XASTKIND_PTROF:
  case DEE_XASTKIND_LVALOF:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  case DEE_XASTKIND_VARRAYOF:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
  case DEE_XASTKIND_CELL:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DEE_XASTKIND_BUILTIN_ALLOCA:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case DEE_XASTKIND_BUILTIN_HELP:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
  case DEE_XASTKIND_BUILTIN_DEX:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX */
   return DeeXAst_Optimize(self->ast_operator.op_a,DEE_OPTIMIZE_ARGS);

  case DEE_XASTKIND_SUPER_AT:
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
  case DEE_XASTKIND_BUILTIN_FF_CLOSURE:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_a,DEE_OPTIMIZE_ARGS) != 0) return -1;
   return DeeXAst_Optimize(self->ast_operator.op_b,DEE_OPTIMIZE_ARGS);

  case DEE_XASTKIND_IFTRUE:
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)==0) {
ifc_optimize_basic:
    if DEE_UNLIKELY(self->ast_ifconst.ic_succ && DeeXAst_Optimize(self->ast_ifconst.ic_succ,DEE_OPTIMIZE_ARGS) != 0) return -1;
    if DEE_UNLIKELY(self->ast_ifconst.ic_fail && DeeXAst_Optimize(self->ast_ifconst.ic_fail,DEE_OPTIMIZE_ARGS) != 0) return -1;
    break;
   }
   if (!self->ast_iftrue.ic_fail) {
    if (!self->ast_iftrue.ic_succ) {
     goto assign_empty;
    } else {
assign_ifc_succ_branch:
     DeeXAst_AssignMove(self,self->ast_iftrue.ic_succ);
    }
    ++*performed_optimizations;
    return 0;
   }
   if DEE_UNLIKELY(self->ast_iftrue.ic_succ && DeeXAst_Optimize(
    self->ast_iftrue.ic_succ,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_iftrue.ic_fail,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0 &&
       !DeeXAst_HasLabels(self->ast_iftrue.ic_fail)) {
    VLOG_OPTIMIZE(self->ast_iftrue.ic_fail->ast_common.ast_token,
                  "Removing unreachable dead fail-branch in expression __if_true\n");
    if (self->ast_iftrue.ic_succ) goto assign_ifc_succ_branch;
    goto assign_empty;
   }
   break;

  case DEE_XASTKIND_IFFALSE:
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)==0) goto ifc_optimize_basic;
   if (!self->ast_iffalse.ic_succ) {
    if (!self->ast_iffalse.ic_fail) {
assign_empty:
     DeeXAst_AssignConst(self,Dee_None);
    } else {
assign_ifc_fail_branch:
     DeeXAst_AssignMove(self,self->ast_iffalse.ic_fail);
    }
    ++*performed_optimizations;
    return 0;
   }
   if DEE_UNLIKELY(self->ast_iffalse.ic_fail && DeeXAst_Optimize(self->ast_iffalse.ic_fail,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_iffalse.ic_succ,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0 &&
       !DeeXAst_HasLabels(self->ast_iffalse.ic_succ)) {
    VLOG_OPTIMIZE(self->ast_iffalse.ic_succ->ast_common.ast_token,
                  "Removing unreachable dead succ-branch in expression __if_false\n");
    if (self->ast_iffalse.ic_fail) goto assign_ifc_fail_branch;
    goto assign_empty;
   }
   break;

  case DEE_XASTKIND_BUILTIN_UNUSED:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_a,
    DEE_OPTIMIZE_ARGS_EX(optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_b,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0 &&
       DeeXAst_IsNoEffect(self->ast_operator.op_a)) {
    VLOG_OPTIMIZE(self->ast_operator.op_a->ast_common.ast_token,
                  "Removing no-effect eval branch from __builtin_unused\n");
    DeeXAst_AssignMove(self,self->ast_operator.op_b);
    ++*performed_optimizations;
   }
   break;

  case DEE_XASTKIND_EXPAND:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_a,
    DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if ((optimize_flags&(DEE_OPTIMIZE_FLAG_DEL_NOEFFECT|
    DEE_OPTIMIZE_FLAG_USED)) == DEE_OPTIMIZE_FLAG_DEL_NOEFFECT) {
    DeeTypeObject const *predicted_expand_type;
    predicted_expand_type = DeeXAst_PredictType(self->ast_operator.op_a);
    if (predicted_expand_type && (DeeType_FLAGS(
     predicted_expand_type)&DEE_TYPE_FLAG_NO_ITERATE_EFFECT)!=0) {
     // Skip the expand if it's not used and has no effect
     VLOG_OPTIMIZE(self->ast_common.ast_token,"Removing unused expand without side-effects\n");
     DeeXAst_AssignMove(self,self->ast_operator.op_a);
     ++*performed_optimizations;
    }
   }
   break;


  default: if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3: if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_c,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_b,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: if DEE_UNLIKELY(DeeXAst_Optimize(self->ast_operator.op_a,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    default: break;
   }
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_MERGE_CONSTS)!=0) {
#define inlined_ast inlined_succ
    DeeXAstObject *if_ast,*temp_ast,*inlined_succ,*inlined_fail;
    switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
     case 1: {
      // Optimize: '-(foo() ? 10 : 20)' --> '(foo() ? -10 : -20)'
      // Requirements:
      //   - At least one of the conditional branches is constant
      if_ast = self->ast_operator.op_a;
      if (if_ast->ast_kind == DEE_XASTKIND_IF && if_ast->ast_if.if_succ &&
        ((if_ast->ast_if.if_succ->ast_kind == DEE_XASTKIND_CONST) ||
        (!if_ast->ast_if.if_fail || if_ast->ast_if.if_fail->ast_kind == DEE_XASTKIND_CONST))) {
       VLOG_OPTIMIZE(self->ast_common.ast_token,
                     "Inlining unary operator on branches of conditional ast\n");
       inlined_succ = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,lexer,
                                       config->parser_flags,if_ast->ast_if.if_succ);
       if DEE_UNLIKELY(!inlined_succ) return -1;
       if (if_ast->ast_if.if_fail) {
        inlined_fail = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,lexer,
                                        config->parser_flags,if_ast->ast_if.if_fail);
        if DEE_UNLIKELY(!inlined_fail) goto err_inlined_succ;
       } else inlined_fail = NULL; // technically this would have to be '<op> none', but that's always 'none'
       temp_ast = if_ast->ast_if.if_succ;
       Dee_INHERIT_REF(if_ast->ast_if.if_succ,inlined_succ);
       Dee_DECREF(temp_ast);
       temp_ast = if_ast->ast_if.if_fail;
       Dee_INHERIT_REF(if_ast->ast_if.if_fail,inlined_fail);
       Dee_XDECREF(temp_ast);
assign_if_ast:
       DeeXAst_AssignMove(self,if_ast);
       ++*performed_optimizations;
      } else if (if_ast->ast_kind == DEE_XASTKIND_IFTRUE ||
                 if_ast->ast_kind == DEE_XASTKIND_IFFALSE) {
       VLOG_OPTIMIZE(self->ast_common.ast_token,
                     "Inlining unary operator in branches of %s\n",
                     if_ast->ast_kind == DEE_XASTKIND_IFTRUE ? "__if_true" : "__if_false");
       if (if_ast->ast_ifconst.ic_succ) {
        inlined_succ = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,lexer,
                                        config->parser_flags,if_ast->ast_ifconst.ic_succ);
        if DEE_UNLIKELY(!inlined_succ) return -1;
       } else inlined_succ = NULL;
       if (if_ast->ast_ifconst.ic_fail) {
        inlined_fail = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,lexer,
                                        config->parser_flags,if_ast->ast_ifconst.ic_fail);
        if DEE_UNLIKELY(!inlined_fail) goto err_inlined_succ;
       } else inlined_fail = NULL;
       temp_ast = if_ast->ast_ifconst.ic_succ;
       Dee_INHERIT_REF(if_ast->ast_ifconst.ic_succ,inlined_succ);
       Dee_XDECREF(temp_ast);
       temp_ast = if_ast->ast_ifconst.ic_fail;
       Dee_INHERIT_REF(if_ast->ast_ifconst.ic_fail,inlined_fail);
       Dee_XDECREF(temp_ast);
       goto assign_if_ast;
      } else if (if_ast->ast_kind == DEE_XASTKIND_STATEMENT) {
       DeeSAstObject *statement = if_ast->ast_statement.s_stmt;
       switch (statement->ast_kind) {
        case DEE_SASTKIND_EXPRESSION:
         VLOG_OPTIMIZE(self->ast_common.ast_token,
                       "Inlining unary operator into statement containing an expression\n");
         inlined_ast = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,
                                        lexer,config->parser_flags,statement->ast_expression.x_ast);
         if DEE_UNLIKELY(!inlined_ast) return -1;
         temp_ast = statement->ast_expression.x_ast;
         Dee_INHERIT_REF(statement->ast_expression.x_ast,inlined_ast);
         Dee_DECREF(temp_ast);
         goto assign_if_ast;
        case DEE_SASTKIND_BLOCK: {
         DeeXAstObject *wrapped_statement_xast;
         DeeSAstObject *wrapped_statement_sast,*wrapped_expr_sast;
         DEE_ASSERT(statement->ast_block.b_astc);
         VLOG_OPTIMIZE(self->ast_common.ast_token,
                       "Inlining unary operator into statement containing a block\n");
         wrapped_statement_sast = statement->ast_block.b_astv[statement->ast_block.b_astc-1];
         wrapped_statement_xast = DeeXAst_NewStatement(wrapped_statement_sast->ast_common.ast_token,
                                                       lexer,config->parser_flags,wrapped_statement_sast);
         if DEE_UNLIKELY(!wrapped_statement_xast) return -1;
         inlined_ast = DeeXAst_NewUnary(self->ast_kind,self->ast_common.ast_token,
                                        lexer,config->parser_flags,wrapped_statement_xast);
         Dee_DECREF(wrapped_statement_xast);
         if DEE_UNLIKELY(!inlined_ast) return -1;
         wrapped_expr_sast = DeeSAst_NewExpression(inlined_ast->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                                   lexer,config->parser_flags,inlined_ast);
         Dee_DECREF(inlined_ast);
         if DEE_UNLIKELY(!wrapped_expr_sast) return -1;
         DEE_ASSERT(wrapped_statement_sast == statement->ast_block.b_astv[statement->ast_block.b_astc-1]);
         Dee_INHERIT_REF(statement->ast_block.b_astv[statement->ast_block.b_astc-1],wrapped_expr_sast);
         Dee_DECREF(wrapped_statement_sast);
         goto assign_if_ast;
        }
        default: break;
       }
      }
#if 0
      // TODO: Optimize unary operators that have no effect on the actual type
      DeeTypeObject const *predicted_type;
      if ((predicted_type = DeeXAst_PredictType(if_ast)) != NULL) {
       DeeObject *(DEE_CALL *typeslot)(DeeObject *);
       // TODO: tp_pos must be replaced by the appropriate typeslot, based on self->ast_kind
       typeslot = DeeType_GET_SLOT(predicted_type,tp_pos);
       if (typeslot == &_DeeGeneric_ReturnSelf) goto assign_if_ast;
       else if (typeslot == &DeeObject_Copy) self->ast_kind = DEE_XASTKIND_COPY;
       else if (typeslot == &DeeObject_Move) self->ast_kind = DEE_XASTKIND_MOVE;
      }
#endif
     } break;
     case 2: {
      if (self->ast_operator.op_b->ast_kind == DEE_XASTKIND_CONST) {
       // Optimize: '(foo() ? 10 : 20)*2' --> '(foo() ? 20 : 40)'
       // Optimize: '2*(foo() ? 10 : 20)' --> '(foo() ? 20 : 40)'
       // Requirements:
       //   - The non-if operand is constant
       //   - At least one of the conditional branches is constant
       if_ast = self->ast_operator.op_a;
       if (if_ast->ast_kind == DEE_XASTKIND_IF && if_ast->ast_if.if_succ &&
         ((if_ast->ast_if.if_succ->ast_kind == DEE_XASTKIND_CONST) ||
         (!if_ast->ast_if.if_fail || if_ast->ast_if.if_fail->ast_kind == DEE_XASTKIND_CONST))) {
        VLOG_OPTIMIZE(self->ast_common.ast_token,
                      "Inlining binary operator with conditional on its left side\n");
        inlined_succ = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                         if_ast->ast_if.if_succ,self->ast_operator.op_b);
        if DEE_UNLIKELY(!inlined_succ) return -1;
        if (if_ast->ast_if.if_fail) {
         inlined_fail = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          if_ast->ast_if.if_fail,self->ast_operator.op_b);
         if DEE_UNLIKELY(!inlined_fail) {err_inlined_succ: Dee_XDECREF(inlined_succ); return -1; }
        } else inlined_fail = NULL; // technically this would have to be 'none <op> self->ast_operator.op_b', but that's always 'none'
        temp_ast = if_ast->ast_if.if_succ;
        Dee_INHERIT_REF(if_ast->ast_if.if_succ,inlined_succ);
        Dee_DECREF(temp_ast);
        temp_ast = if_ast->ast_if.if_fail;
        Dee_INHERIT_REF(if_ast->ast_if.if_fail,inlined_fail);
        Dee_XDECREF(temp_ast);
        goto assign_if_ast;
       } else if (if_ast->ast_kind == DEE_XASTKIND_IFTRUE ||
                  if_ast->ast_kind == DEE_XASTKIND_IFFALSE) {
        VLOG_OPTIMIZE(self->ast_common.ast_token,
                      "Inlining binary operator in branches of %s on its left side\n",
                      if_ast->ast_kind == DEE_XASTKIND_IFTRUE ? "__if_true" : "__if_false");
        if (if_ast->ast_ifconst.ic_succ) {
         inlined_succ = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          if_ast->ast_ifconst.ic_succ,self->ast_operator.op_b);
         if DEE_UNLIKELY(!inlined_succ) return -1;
        } else inlined_succ = NULL;
        if (if_ast->ast_ifconst.ic_fail) {
         inlined_fail = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          if_ast->ast_ifconst.ic_fail,self->ast_operator.op_b);
         if DEE_UNLIKELY(!inlined_fail) goto err_inlined_succ;
        } else inlined_fail = NULL;
        temp_ast = if_ast->ast_ifconst.ic_succ;
        Dee_INHERIT_REF(if_ast->ast_ifconst.ic_succ,inlined_succ);
        Dee_XDECREF(temp_ast);
        temp_ast = if_ast->ast_ifconst.ic_fail;
        Dee_INHERIT_REF(if_ast->ast_ifconst.ic_fail,inlined_fail);
        Dee_XDECREF(temp_ast);
        goto assign_if_ast;
       }
      } else if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
       if_ast = self->ast_operator.op_b;
       if (if_ast->ast_kind == DEE_XASTKIND_IF && if_ast->ast_if.if_succ &&
         ((if_ast->ast_if.if_succ->ast_kind == DEE_XASTKIND_CONST) ||
         (!if_ast->ast_if.if_fail || if_ast->ast_if.if_fail->ast_kind == DEE_XASTKIND_CONST))) {
        VLOG_OPTIMIZE(self->ast_common.ast_token,
                      "Inlining binary operator with conditional on its right side\n");
        inlined_succ = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                         self->ast_operator.op_a,if_ast->ast_if.if_succ);
        if DEE_UNLIKELY(!inlined_succ) return -1;
        if (if_ast->ast_if.if_fail) {
         inlined_fail = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          self->ast_operator.op_a,if_ast->ast_if.if_fail);
        } else {
         if ((temp_ast = DeeXAst_NewConst(if_ast->ast_common.ast_token,Dee_None)) == NULL) goto err_inlined_succ;
         inlined_fail = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          self->ast_operator.op_a,temp_ast);
         Dee_DECREF(temp_ast);
        }
        if DEE_UNLIKELY(!inlined_fail) goto err_inlined_succ;
        temp_ast = if_ast->ast_if.if_succ;
        Dee_INHERIT_REF(if_ast->ast_if.if_succ,inlined_succ);
        Dee_DECREF(temp_ast);
        temp_ast = if_ast->ast_if.if_fail;
        Dee_INHERIT_REF(if_ast->ast_if.if_fail,inlined_fail);
        Dee_XDECREF(temp_ast);
        goto assign_if_ast;
       } else if (if_ast->ast_kind == DEE_XASTKIND_IFTRUE ||
                  if_ast->ast_kind == DEE_XASTKIND_IFFALSE) {
        VLOG_OPTIMIZE(self->ast_common.ast_token,
                      "Inlining binary operator in branches of %s on its right side\n",
                      if_ast->ast_kind == DEE_XASTKIND_IFTRUE ? "__if_true" : "__if_false");
        if (if_ast->ast_ifconst.ic_succ) {
         inlined_succ = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          self->ast_operator.op_a,if_ast->ast_ifconst.ic_succ);
         if DEE_UNLIKELY(!inlined_succ) return -1;
        } else inlined_succ = NULL;
        if (if_ast->ast_ifconst.ic_fail) {
         inlined_fail = DeeXAst_NewBinary(self->ast_kind,self->ast_common.ast_token,lexer,config->parser_flags,
                                          self->ast_operator.op_a,if_ast->ast_ifconst.ic_fail);
         if DEE_UNLIKELY(!inlined_fail) goto err_inlined_succ;
        } else inlined_fail = NULL;
        temp_ast = if_ast->ast_ifconst.ic_succ;
        Dee_INHERIT_REF(if_ast->ast_ifconst.ic_succ,inlined_succ);
        Dee_XDECREF(temp_ast);
        temp_ast = if_ast->ast_ifconst.ic_fail;
        Dee_INHERIT_REF(if_ast->ast_ifconst.ic_fail,inlined_fail);
        Dee_XDECREF(temp_ast);
        goto assign_if_ast;
       }
      }
     } break;
     default: break;
    }
#undef inlined_ast
   }
  } break;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_Optimize(
 DEE_A_INOUT struct DeeVarDeclStorage *self,
 DEE_A_IN Dee_uint32_t flags, DEE_OPTIMIZE_PARAMS) {
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_UNUSED_VAR) != 0 &&
       !self->vds_var.vs_var->lv_uses &&
       (self->vds_var.vs_var->lv_flags&DEE_LOCALVAR_FLAG_USED)==0 &&
      ((flags&DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_OPTIONAL)!=0||
       (self->vds_var.vs_var->lv_flags&DEE_LOCALVAR_FLAG_UNUSED)!=0)) {
    VLOG_OPTIMIZE(self->vds_var.vs_token,
                  "Removing unused variable from generic vardecl storage\n");
    // Unused variable --> transform into a none-storage
    DeeLocalVar_REM_INIT(self->vds_var.vs_var);
    // Once no one is initializing the variable anymore, remove it from its scope
    if (!self->vds_var.vs_var->lv_init) _DeeScope_DelName(
     self->vds_var.vs_var->lv_scope,self->vds_var.vs_var->lv_name);
    Dee_DECREF(self->vds_var.vs_token);
    Dee_DECREF(self->vds_var.vs_var);
    self->vds_kind = DEE_VARDECLSTORAGEKIND_NONE;
    ++*performed_optimizations;
   }
   break;
  case DEE_VARDECLSTORAGEKIND_ATTR:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->vds_attr.as_object,DEE_OPTIMIZE_ARGS) != 0) return -1;
   return DeeXAst_Optimize(self->vds_attr.as_attr,DEE_OPTIMIZE_ARGS);
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   return DeeXAst_Optimize(self->vds_attr_c.acs_object,DEE_OPTIMIZE_ARGS);
  case DEE_VARDECLSTORAGEKIND_ITEM:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->vds_item.is_object,DEE_OPTIMIZE_ARGS) != 0) return -1;
   return DeeXAst_Optimize(self->vds_item.is_key,DEE_OPTIMIZE_ARGS);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   if DEE_UNLIKELY(DeeXAst_Optimize(self->vds_range.rs_object,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(self->vds_range.rs_begin && DeeXAst_Optimize(self->vds_range.rs_begin,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if DEE_UNLIKELY(self->vds_range.rs_end && DeeXAst_Optimize(self->vds_range.rs_end,DEE_OPTIMIZE_ARGS) != 0) return -1;
   break;
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   DEE_ASSERT(self->vds_expand.es_declc);
   do if DEE_UNLIKELY(DeeVarDeclStorage_Optimize(iter,flags,DEE_OPTIMIZE_ARGS) != 0) return -1;
   while (++iter != end);
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeXAst_Optimize(self->vds_assign.as_ast,DEE_OPTIMIZE_ARGS);
  default: break;
 }
 return 0;
}



DEE_DECL_END

#endif /* !GUARD_DEEMON_COMPILER_XAST_OPTIMIZE_INL */
