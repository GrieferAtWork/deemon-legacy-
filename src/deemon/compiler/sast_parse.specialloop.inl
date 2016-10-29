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
#include "sast_parse.c"
#define GENERATOR
#endif

DEE_DECL_BEGIN

#ifdef GENERATOR
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorSpecialLoopStatement(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *specialloop_token, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseSpecialLoopStatement(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *specialloop_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS)
#endif
{
 DeeScopeObject *loop_scope;
 DeeSAstObject *ast_result,*loop_block;
 // __looponce Statement
 if ((loop_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
#ifdef GENERATOR
 if ((loop_block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#else
 if ((loop_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#endif
 {
/*err_looponce_1:*/ Dee_DECREF(loop_scope); return NULL;
 }
 ast_result = DeeSAst_NewSpecialLoop(kind,specialloop_token,
#ifdef GENERATOR
                                     DeeParserLabelRefList_Empty,
#else
                                     labels_,
#endif
                                     loop_scope,loop_block);
 Dee_DECREF(loop_block);
 Dee_DECREF(loop_scope);
 return ast_result;
}


DEE_DECL_END
