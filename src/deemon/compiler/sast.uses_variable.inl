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
#define DO_COUNT
#endif

#ifdef DO_COUNT
#define CHECK_XAST(x) result+=DeeXAst_CountVariableUses(x,variable)
#define CHECK_SAST(x) result+=DeeSAst_CountVariableUses(x,variable)
#else
#define CHECK_XAST(x) if(DeeXAst_UsesVariable(x,variable))return 1;else;
#define CHECK_SAST(x) if(DeeSAst_UsesVariable(x,variable))return 1;else;
#endif

DEE_DECL_BEGIN

#ifdef DO_COUNT
DEE_A_RET_WUNUSED Dee_size_t DeeSAst_CountVariableUses
#else
DEE_A_RET_WUNUSED int DeeSAst_UsesVariable
#endif
(DEE_A_IN DeeSAstObject const *self,
 DEE_A_IN DeeLocalVarObject const *variable) {
#ifdef DO_COUNT
 Dee_size_t result = 0;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(variable) && DeeLocalVar_Check(variable));
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION:
   CHECK_XAST(self->ast_expression.x_ast);
   break;
  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **iter,**end;
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   while (iter != end) CHECK_SAST(*iter++);
  } break;
  case DEE_SASTKIND_RETURN:
   if (self->ast_return.r_value) CHECK_XAST(self->ast_return.r_value);
   break;
  case DEE_SASTKIND_YIELD:
   CHECK_XAST(self->ast_yield.y_value);
   break;
  case DEE_SASTKIND_ASSERT:
   CHECK_XAST(self->ast_assert.a_check);
   if (self->ast_assert.a_message) CHECK_XAST(self->ast_assert.a_message);
   break;
  case DEE_SASTKIND_PRINT:
   if (self->ast_print.p_file) CHECK_XAST(self->ast_print.p_file);
   if (self->ast_print.p_data) CHECK_XAST(self->ast_print.p_data);
   break;
  case DEE_SASTKIND_IF:
   CHECK_XAST(self->ast_if.if_cond);
   if (self->ast_if.if_succ_block) CHECK_SAST(self->ast_if.if_succ_block);
   if (self->ast_if.if_fail_block) CHECK_SAST(self->ast_if.if_fail_block);
   break;
  case DEE_SASTKIND_FOR:
   if (self->ast_for.f_head_init) CHECK_XAST(self->ast_for.f_head_init);
   CHECK_XAST(self->ast_for.f_head_cond);
   if (self->ast_for.f_head_next) CHECK_XAST(self->ast_for.f_head_next);
   CHECK_SAST(self->ast_for.f_block);
   break;
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
#ifdef DO_COUNT
   result += DeeVarDeclStorage_CountVariableUses(&self->ast_forin.fi_var,variable);
#else
   if (DeeVarDeclStorage_UsesVariable(&self->ast_forin.fi_var,variable)) return 1;
#endif
   CHECK_XAST(self->ast_forin.fi_seq);
   CHECK_SAST(self->ast_forin.fi_block);
   break;
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   CHECK_XAST(self->ast_while.w_cond);
   CHECK_SAST(self->ast_while.w_block);
   break;
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:
   CHECK_SAST(self->ast_specialloop.sl_block);
   break;
  case DEE_SASTKIND_THROW:
   if (self->ast_throw.t_object) CHECK_XAST(self->ast_throw.t_object);
   break;
  case DEE_SASTKIND_TRY: {
   struct DeeSAstTryHandler *iter,*end;
   CHECK_SAST(self->ast_try.t_block);
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   do {
#ifdef DO_COUNT
    result += DeeVarDeclStorage_CountVariableUses(&iter->th_local,variable);
#else
    if (DeeVarDeclStorage_UsesVariable(&iter->th_local,variable)) return 1;
#endif
    CHECK_SAST(iter->th_block);
   } while (++iter != end);
  } break;
  case DEE_SASTKIND_MODULE:
   CHECK_SAST(self->ast_module.m_block);
   break;
  case DEE_SASTKIND_SWITCH:
   CHECK_XAST(self->ast_switch.sw_expr);
   CHECK_SAST(self->ast_switch.sw_block);
   break;
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:
   if (self->ast_ifconst.ic_succ_block) CHECK_SAST(self->ast_ifconst.ic_succ_block);
   if (self->ast_ifconst.ic_fail_block) CHECK_SAST(self->ast_ifconst.ic_fail_block);
   break;
  default: break;
 }
#ifdef DO_COUNT
 return result;
#else
 return 0;
#endif
}

#undef CHECK_SAST
#undef CHECK_XAST

#ifdef DO_COUNT
#undef DO_COUNT
#endif

DEE_DECL_END

