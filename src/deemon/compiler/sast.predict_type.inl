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

DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
DeeSAst_PredictType(DEE_A_IN DeeSAstObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 switch (self->ast_kind) {
  case DEE_SASTKIND_BREAK:
  case DEE_SASTKIND_CONTINUE:
  case DEE_SASTKIND_GOTO:
  case DEE_SASTKIND_RETURN:
  case DEE_SASTKIND_THROW:
   // The above could actually be considered to return anything
  case DEE_SASTKIND_ASSERT:
  case DEE_SASTKIND_DOWHILE:
  case DEE_SASTKIND_EMPTY:
  case DEE_SASTKIND_FOR:
  case DEE_SASTKIND_MODULE:
  case DEE_SASTKIND_PRINT:
  case DEE_SASTKIND_TRY:
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_YIELD:
  case DEE_SASTKIND_IFFALSE:
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:
   return (DeeTypeObject *)&DeeNone_Type;
  case DEE_SASTKIND_EXPRESSION:
   return DeeXAst_PredictType(self->ast_expression.x_ast);
  case DEE_SASTKIND_BLOCK:
   DEE_ASSERT(self->ast_block.b_astc);
   return DeeSAst_PredictType(self->ast_block.b_astv[self->ast_block.b_astc-1]);
  case DEE_SASTKIND_IF: {
   DeeTypeObject const *prediction_a,*prediction_b;
   prediction_a = self->ast_if.if_succ_block ? DeeSAst_PredictType(self->ast_if.if_succ_block) : (DeeTypeObject *)&DeeNone_Type;
   prediction_b = self->ast_if.if_fail_block ? DeeSAst_PredictType(self->ast_if.if_fail_block) : (DeeTypeObject *)&DeeNone_Type;
   return prediction_a == prediction_b ? prediction_a : NULL;
  } break;
  case DEE_SASTKIND_FORIN:
   return _DeeType_IteratorType(DeeXAst_PredictType(self->ast_foreach.fi_seq));
  case DEE_SASTKIND_FOREACH:
   return DeeXAst_PredictType(self->ast_foreach.fi_seq);
  case DEE_SASTKIND_SWITCH:
   return DeeXAst_PredictType(self->ast_switch.sw_expr);
  default: break;
 }
 return NULL;
}

DEE_DECL_END

