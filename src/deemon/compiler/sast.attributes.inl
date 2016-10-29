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
#ifdef __INTELLISENSE__
#include "sast.c"
#endif

DEE_DECL_BEGIN

DEE_A_RET_WUNUSED DeeAstAttribute DeeSAst_AttrEx(
 DEE_A_IN DeeSAstObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast) {
 DeeAstAttribute result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(root_ast) && (DeeXAst_Check(root_ast) || DeeSAst_Check(root_ast)));
 switch (self->ast_kind) {
  case DEE_SASTKIND_EMPTY:
   result = DEE_AST_ATTRIBUTE_NOOP;
   break;
  case DEE_SASTKIND_EXPRESSION:
   result = DeeXAst_AttrEx(self->ast_expression.x_ast,flags,root_ast);
   break;
  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **iter,**end;
   result = DEE_AST_ATTRIBUTE_NOOP;
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   while (iter != end && result != DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) {
    result = DeeAstAttribute_MergeSequential(result,DeeSAst_AttrEx(*iter++,flags,root_ast),flags);
   }
  } break;
  case DEE_SASTKIND_RETURN:
   if (!self->ast_return.r_value) return (DEE_AST_ATTRIBUTE_NORETURN|DEE_AST_ATTRIBUTE_NOEXCEPT);
   result = (DeeXAst_AttrEx(self->ast_return.r_value,flags,root_ast)&~(
    DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOJUMP)); // This one's kind of obvios
   if ((result&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0 ||
       (flags&DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT)==0
       ) result |= DEE_AST_ATTRIBUTE_NORETURN;
   break;
  case DEE_SASTKIND_YIELD:
   result = DeeXAst_AttrEx(self->ast_yield.y_value,flags,root_ast)
    &~(DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOJUMP); // Yield may not always return
   break;
  case DEE_SASTKIND_ASSERT:
   // Not noexcept, because one if thrown on failure
   result = DeeXAst_AttrEx(self->ast_yield.y_value,flags,root_ast)&~(
    DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP);
   break;
  case DEE_SASTKIND_PRINT:
   result = DeeAstAttribute_MergeSequential(
    self->ast_print.p_file ? DeeXAst_AttrEx(self->ast_print.p_file,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
    self->ast_print.p_data ? DeeXAst_AttrEx(self->ast_print.p_data,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,flags)
    &~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
   break;
  case DEE_SASTKIND_IF:
   result = DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_if.if_cond,flags,root_ast),
    DeeAstAttribute_MergeParallel(
     self->ast_if.if_succ_block ? DeeSAst_AttrEx(self->ast_if.if_succ_block,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
     self->ast_if.if_fail_block ? DeeSAst_AttrEx(self->ast_if.if_fail_block,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
     flags),flags);
   break;
  case DEE_SASTKIND_FOR:
   result = DeeSAst_AttrEx(self->ast_for.f_block,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast);
   if (self->ast_for.f_head_init) result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_for.f_head_init,flags,root_ast),flags); // The break/continue block starts after the initializer
                                  result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_for.f_head_cond,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),flags);
   if (self->ast_for.f_head_next) result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_for.f_head_next,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),flags);
   break;
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
   result = DeeAstAttribute_MergeSequential(
    DeeVarDeclStorage_AttrEx(&self->ast_forin.fi_var,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),
    DeeAstAttribute_MergeSequential(
     DeeXAst_AttrEx(self->ast_forin.fi_seq,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),
     DeeSAst_AttrEx(self->ast_forin.fi_block,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),
     flags),flags)&~(DEE_AST_ATTRIBUTE_NOEXCEPT); // __iternext__/__iterself__ might throw something
   break;
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   result = DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_while.w_cond,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),
    DeeSAst_AttrEx(self->ast_while.w_block,flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast),flags);
   break;
  case DEE_SASTKIND_LOOPEVER:
   result = DeeSAst_AttrEx(self->ast_looponce.sl_block,flags&~(
    DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast);
   // This one is noreturn if the ast does not contain a break/goto (aka. is no-jump)
   if ((result&DEE_AST_ATTRIBUTE_NOJUMP)!=0)
    result |= DEE_AST_ATTRIBUTE_NORETURN;
   break;
  case DEE_SASTKIND_LOOPONCE:
   result = DeeSAst_AttrEx(self->ast_looponce.sl_block,flags&~(
    DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast);
   break;
  case DEE_SASTKIND_LOOPNONE: {
   return ((DeeSAst_AttrEx(self->ast_specialloop.sl_block,
    flags&~(DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK),root_ast)|
    DEE_AST_ATTRIBUTE_NOEXCEPT)&~(DEE_AST_ATTRIBUTE_NORETURN))|DEE_AST_ATTRIBUTE_NOJUMP;
  } break;

  case DEE_SASTKIND_CONTINUE:
  case DEE_SASTKIND_BREAK:
   result = (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK)!=0
    ? (DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NORETURN)
    // NOTE: If break/continue are not allowed, that means they jump back into the current scope.
    //       Meaning that as far as we are concerned, they always return.
    : (DEE_AST_ATTRIBUTE_NOEXCEPT));
   break;
  case DEE_SASTKIND_GOTO:
   // TODO:
   //   If the jump points somewhere into 'root_ast',
   //   then we need to use the attributes of where that jump points to
   //   Though that will be really f*ing difficult because it might
   //   point right in the middle of some other statement. e.g.: 
   //       >> jmp:
   //       >> assert __builtin_noreturn(({ // true, because jmp points outside the current context
   //       >>   if (foo) goto jmp;
   //       >>   return 42;
   //       >> }));
   //       >> assert __builtin_noreturn(({ // True, because 'jmp' traverses a return
   //       >>   if (foo) goto jmp2;
   //       >>   if (bar) {
   //       >>   jmp2:
   //       >>     print "Hi!";
   //       >>   }
   //       >>   return 42;
   //       >> }));
   //   And the it get even more complicated here:
   //       >> assert !__builtin_noreturn(({ // not noreturn
   //       >>   local x;
   //       >>   local foo;
   //       >>   try {
   //       >>     // The exception thrown at the
   //       >>     // other end will not be caught
   //       >>     if (foo) goto jmp;
   //       >>   } catch (...) {
   //       >>   }
   //       >>   if (x) {
   //       >> jmp: throw "Oh NO!";
   //       >>   }
   //       >> }));
   //   At least the following isn't allowed...:
   //       >> __builtin_noreturn(({
   //       >>   goto do_throw;
   //       >>   try {
   //       >> do_throw: // ERROR: Illegal jump: Invalid exception alignment
   //       >>     throw "Oh NO!";
   //       >>   } catch (...) {
   //       >>   }
   //       >> }));
   // v would only be noreturn if the jump ends outside of 'root_ast'
   DEE_ASSERT(self->ast_goto.g_label);
   if (DeeParserLabel_IS_FORWARD(self->ast_goto.g_label)) {
    // The label is only forward declared, meaning it
    // most definitely jumps out of the current scope.
    result = DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NORETURN;
   } else if (DeeAst_HasLabel(root_ast,self->ast_goto.g_label)) {
    // The root ast contains our label, meaning that we are not noreturn
    result = DEE_AST_ATTRIBUTE_NOEXCEPT/*|DEE_AST_ATTRIBUTE_NORETURN*/;
   } else {
    // The label points somewhere outside of our AST, meaning we're noreturn
    result = DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NORETURN;
   }
   break;
  case DEE_SASTKIND_THROW:
   result = DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags);
   break;
  case DEE_SASTKIND_TRY: {
   DeeAstAttribute block_attr;
   struct DeeSAstTryHandler *iter,*end;
   int has_catch_handler = 0;
   int has_nothrow_catch = 1;
   result = DEE_AST_ATTRIBUTE_NOOP;
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   do {
    block_attr = DeeSAst_AttrEx(iter->th_block,flags,root_ast);
    if (iter->th_kind == DeeExceptionHandleKind_FINALLY) {
     result = DeeAstAttribute_MergeSequential(result,block_attr,flags);
     has_nothrow_catch = 0;
    } else {
     has_catch_handler = 1;
     result = DeeAstAttribute_MergeParallel(result,block_attr,flags);
     if ((block_attr&DEE_AST_ATTRIBUTE_NOEXCEPT)==0
         ) has_nothrow_catch = 0;
    }
   } while (++iter != end);
   // In here we must handle noreturn code differently if it follows non-noexcept code:
   // But only if "has_catch_handler", as a finally-block still causes noreturn.
   // Sollution: new flag: DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT (changes the way DeeAstAttribute_MergeSequential works)
   // >> try {
   // >>   print "foobar"; // This isn't noexcept ...
   // >>   return 42;      // ... so this isn't noreturn
   // >> } catch (...) {
   // >> }
   // >> print "hi!"; // Reachable because ''
   block_attr = DeeSAst_AttrEx(self->ast_try.t_block,has_catch_handler ? flags|(
    // v Ignore the noreturn attribute from throwing constructs
    DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT) : flags,root_ast);
   result = DeeAstAttribute_MergeSequential(result,block_attr,flags);
   // If there was a noexcept catch(...) handler at the end, the try-statement is noexcept as well:
   // try {
   //   throw "NOPE";
   // } catch (...) {
   // }
   if (has_nothrow_catch) {
    result |= DEE_AST_ATTRIBUTE_NOEXCEPT;
   }
  } break;
  case DEE_SASTKIND_MODULE:
   result = DeeSAst_AttrEx(self->ast_module.m_block,flags,root_ast);
   break;

  case DEE_SASTKIND_SWITCH: {
   // Technically we'd have to check all the different
   // switch entry points and merge all of those together with parallel attributes.
   // But that is a bit too complicated for right now, so we only look at the switch expression
   return DeeXAst_AttrEx(self->ast_switch.sw_expr,flags,root_ast)&
    ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_SASTKIND_IFTRUE: {
   result = self->ast_iffalse.ic_succ_block
    ? DeeSAst_AttrEx(self->ast_iffalse.ic_succ_block,flags,root_ast)
    : DEE_AST_ATTRIBUTE_NOOP;
   if ((result&DEE_AST_ATTRIBUTE_NOEFFECT)!=0 &&
       self->ast_iffalse.ic_fail_block &&
       !DeeSAst_IsNoEffect(self->ast_iffalse.ic_fail_block)
       ) result &= ~(DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_SASTKIND_IFFALSE: {
   result = self->ast_iffalse.ic_fail_block
    ? DeeSAst_AttrEx(self->ast_iffalse.ic_fail_block,flags,root_ast)
    : DEE_AST_ATTRIBUTE_NOOP;
   if ((result&DEE_AST_ATTRIBUTE_NOEFFECT)!=0 &&
       self->ast_iffalse.ic_succ_block &&
       !DeeSAst_IsNoEffect(self->ast_iffalse.ic_succ_block)
       ) result &= ~(DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  default: result = DEE_AST_ATTRIBUTE_NONE; break;
 }
 if (!DeeParserLabelRefList_EMPTY(&self->ast_common.ast_labels))
  result &= ~DEE_AST_ATTRIBUTE_NOEFFECT;
 return result;
}

DEE_DECL_END
