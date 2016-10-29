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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOffsetofSuffix(
 DEE_A_INOUT DeeTokenObject *operator_token,
 DEE_A_IN DeeTypeObject const *base, DEE_PARSER_PARAMS) {
 DeeXAstObject *result,*index_branch; DeeObject *result_const;
 Dee_size_t index,more_offset,result_offset = 0;
 DeeTypeObject *new_base;
 if (TPPTokenID_IS_KEYWORD(token.tk_id)) goto begin_keyword;
 while (1) {
  if (token.tk_id == '[') {
   // Array/Index offsetof
   if DEE_UNLIKELY(!yield()) return NULL;
   if ((index_branch = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return NULL;
   if (token.tk_id != ']') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_IN_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ']' after '__builtin_offsetof(...[%r'",index_branch) != 0) goto err_index_branch;
   } else if DEE_UNLIKELY(!yield()) {
err_index_branch: Dee_DECREF(index_branch); return NULL;
   }
   if (index_branch->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_IN_OFFSETOF_INDEX_LOOKUP,
     (DeeObject *)lexer,(DeeObject *)index_branch->ast_common.ast_token,
     "Expected constant in '__builtin_offsetof(...[...]' index offset, but got %r",
     index_branch) != 0) goto err_index_branch;
    index = 0; // *shrugs*
   } else {
    if (DeeObject_Cast(Dee_size_t,index_branch->ast_const.c_const,&index) != 0) {
     DeeError_Handled();
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_INTEGRAL_IN_OFFSETOF_INDEX_LOOKUP,
      (DeeObject *)lexer,(DeeObject *)index_branch->ast_common.ast_token,
      "Expected integral constant in '__builtin_offsetof(...[...]', but got instance of %q",
      DeeType_NAME(Dee_TYPE(index_branch->ast_const.c_const))) != 0) goto err_index_branch;
     index = 0;
    }
   }
   if ((new_base = DeeType_OffsetofItemIndex(base,index,&more_offset)) == NULL) {
    DeeError_Handled();
    if (DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_INDEX_IN_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)index_branch->ast_common.ast_token,
     "Failed to find index %Iu in %q for __builtin_offsetof",index,DeeType_NAME(base)) != 0) goto err_index_branch;
   } else base = new_base,result_offset += more_offset;
   Dee_DECREF(index_branch);
  } else if (token.tk_id == '.') {
   DeeObject *keyword_name;
   // Attribute/Member offsetof
   if DEE_UNLIKELY(!yield()) return NULL;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_IN_OFFSETOF_ATTRIBUTE_LOOKUP,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after '.' in __builtin_offsetof") != 0) return NULL;
    break;
   }
begin_keyword:
   DEE_ASSERT(TPPTokenID_IS_KEYWORD(token.tk_id));
   if ((keyword_name = DeeToken_Str((DeeObject *)token_ob)) == NULL) return NULL;
   if ((new_base = DeeType_OffsetofAttr(base,keyword_name,&more_offset)) == NULL) {
    DeeError_Handled();
    if (DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_ATTRIBUTE_IN_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Failed to find attribute %q in %q for __builtin_offsetof",
     DeeString_STR(keyword_name),DeeType_NAME(base)) != 0) {
/*err_keyword_name:*/ Dee_DECREF(keyword_name); return NULL;
    }
   } else base = new_base,result_offset += more_offset;
   Dee_DECREF(keyword_name);
   if DEE_UNLIKELY(!yield()) return NULL;
  } else break;
 }
/*end:*/
 result_const = DeeObject_New(Dee_size_t,result_offset);
 result = DeeXAst_NewConst(operator_token,result_const);
 Dee_DECREF(result_const);
 return result;
}


DEE_DECL_END
