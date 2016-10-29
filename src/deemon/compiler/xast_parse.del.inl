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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseDel(DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_ParseDelWithAttribute(&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseDelWithAttribute(
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *del_ast,*ast_result;
 if ((del_ast = DeeXAst_ParseUnaryEx(DEE_XAST_UNARAYSUFFIX_FLAG_NONE,
  DEE_XAST_VARDECL_MODE_DEFAULT,attr,DEE_PARSER_ARGS)) == NULL) return NULL;
 ast_result = DeeXAst_NewDel(del_ast->ast_common.ast_token,
                             lexer,parser_flags,del_ast,scope);
 Dee_DECREF(del_ast);
 return ast_result;
}


extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllDel(
 DEE_A_IN DeeTokenObject *del_token, DEE_PARSER_PARAMS) {
 DeeXAstObject  *del_ast,*ast_result;
 Dee_size_t      del_asta;
 Dee_size_t      del_astc;
 DeeXAstObject **del_astv,**new_del_astv;
 int should_end;
 if ((del_ast = DeeXAst_ParseDel(DEE_PARSER_ARGS)) == NULL) return NULL;
 if (token.tk_id != ',') return del_ast;
 if DEE_UNLIKELY(!yield()) {/*err_import_ast:*/ Dee_DECREF(del_ast); return NULL; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) return del_ast;
#endif
 del_asta = 0;
 del_astc = 0;
 del_astv = NULL;
 should_end = 0;
 goto seq_begin;
 while (1) {
  if (token.tk_id != ',') should_end = 1; else {
   if DEE_UNLIKELY(!yield()) goto err_import_ast_v;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) should_end = 1;
#endif
  }
seq_begin:
  if (del_astc == del_asta) {
   if (!del_asta) del_asta = 2; else del_asta *= 2;
   while ((new_del_astv = (DeeXAstObject **)realloc_nz(
    del_astv,del_asta*sizeof(DeeXAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    Dee_DECREF(del_ast);
    DeeError_NoMemory();
err_import_ast_v:
    new_del_astv = del_astv+del_astc;
    while (new_del_astv != del_astv) Dee_DECREF(*--new_del_astv);
    free(del_astv);
    return NULL;
   }
   del_astv = new_del_astv;
  }
  Dee_INHERIT_REF(del_astv[del_astc++],del_ast);
  if (should_end) break;
  if ((del_ast = DeeXAst_ParseDel(DEE_PARSER_ARGS)) == NULL) goto err_import_ast_v;
 }
 if (del_astc != del_asta) {
  // Try to conserve memory
  if ((new_del_astv = (DeeXAstObject **)realloc_nnz(
   del_astv,del_astc*sizeof(DeeXAstObject *))
   ) != NULL) del_astv = new_del_astv;
 }
 ast_result = DeeXAst_NewSequenceFromInheritedAstv(DEE_XASTKIND_TUPLE,del_token,
                                                   lexer,parser_flags,del_astc,del_astv);
 if (!ast_result) {
  new_del_astv = del_astv+del_astc;
  while (new_del_astv != del_astv) Dee_DECREF(*--new_del_astv);
  free(del_astv);
 }
 return ast_result;
}

DEE_DECL_END
