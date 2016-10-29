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
#include "xast_parse.c"
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseTypesCompatibleP(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*result; DeeObject *a_val,*b_val;
 if ((ast_a = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return NULL;
 if (token.tk_id != ',') {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ',' after '__builtin_types_compatible_p(%r'",ast_a) != 0) goto err_ast_a;
 } else if DEE_UNLIKELY(!yield()) {
err_ast_a: Dee_DECREF(ast_a); return NULL;
 }
 if ((ast_b = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_a;
 if (ast_a->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_1,
   (DeeObject *)lexer,(DeeObject *)ast_a->ast_common.ast_token,
   "Expected a constant expression for the first argument in '__builtin_types_compatible_p(...)', "
   "but got %r",ast_a) != 0) {
err_ast_b: Dee_DECREF(ast_b); goto err_ast_a;
  }
  a_val = Dee_None;
 } else a_val = ast_a->ast_const.c_const;
 if (ast_b->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_2,
   (DeeObject *)lexer,(DeeObject *)ast_b->ast_common.ast_token,
   "Expected a constant expression for the second argument in '__builtin_types_compatible_p(...)', "
   "but got %r",ast_b) != 0) goto err_ast_b;
  b_val = Dee_None;
 } else b_val = ast_b->ast_const.c_const;
 if (!DeeType_Check(a_val)) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_1,
   (DeeObject *)lexer,(DeeObject *)ast_a->ast_common.ast_token,
   "Expected a type for the first argument in '__builtin_types_compatible_p(...)', "
   "but got instance of %q",DeeType_NAME(Dee_TYPE(a_val))) != 0) goto err_ast_b;
  a_val = (DeeObject *)Dee_TYPE(a_val);
 }
 if (!DeeType_Check(b_val)) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_2,
   (DeeObject *)lexer,(DeeObject *)ast_b->ast_common.ast_token,
   "Expected a type for the second argument in '__builtin_types_compatible_p(...)', "
   "but got instance of %q",DeeType_NAME(Dee_TYPE(b_val))) != 0) goto err_ast_b;
  b_val = (DeeObject *)Dee_TYPE(b_val);
 }
 result = DeeXAst_NewConst(operator_token,DeeBool_Const(a_val == b_val));
 Dee_DECREF(ast_b);
 Dee_DECREF(ast_a);
 return result;
}


DEE_DECL_END
