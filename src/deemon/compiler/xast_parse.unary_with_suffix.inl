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

DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_ParseUnarySuffixWith(
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_IN Dee_uint32_t flags, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*ast_args;
 DeeXAstKind kind; DeeTokenObject *ast_token;
 Dee_INCREF(ast_a);
again: switch (token.tk_id) {

  // Ignored suffix modifiers
  DEE_PARSE_TOKENID_CASE_CONST
  DEE_PARSE_TOKENID_CASE_VOLATILE
   if DEE_UNLIKELY(!yield()) goto err_a;
   goto again;


  case KWD_pack: {
   int has_paren;
   // Packed parameter list
   // >> function foo(a,b,c) { print a,b,c; }
   // >> foo pack 10,20,30; // same as: 'foo(10,20,30);'
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   has_paren = (token.tk_id == '(');
   if (has_paren && DEE_UNLIKELY(!yield())) goto err_a_token;
   if (token.tk_id == '{') { // Range initializer
    Dee_DECREF(ast_token);
    // TODO: Warn if the lhs operand is constant, but not callable
    if DEE_UNLIKELY((ast_args = DeeXAst_ParseRangeInitializerWithType(
     ast_a,DEE_PARSER_ARGS)) == NULL) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if DEE_UNLIKELY(token.tk_id == ',' && DEE_UNLIKELY(!yield())) { Dee_DECREF(ast_args); goto err_a; }
#endif
   } else if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) {
    // Empty pack argument list
    if DEE_UNLIKELY((ast_args = DeeXAst_NewConst(token_ob,Dee_EmptyTuple)) == NULL) goto err_a_token;
   } else {
    if DEE_UNLIKELY((ast_args = DeeXAst_ParseInitTuple(DEE_PARSER_ARGS)) == NULL) goto err_a_token;
   }
   ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,ast_token,lexer,
                                  parser_flags,ast_a,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   if (has_paren) {
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_PACK,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after 'pack(...' in unary suffix") != 0) goto err_a;
    } else if DEE_UNLIKELY(!yield()) goto err_a;
   }
   goto again;
  } break;


  case TPP_TOK_COLLON_COLLON:
   if DEE_UNLIKELY(ast_a->ast_kind != DEE_XASTKIND_CONST &&
                   ast_a->ast_kind != DEE_XASTKIND_MODULE) {
    // Warn about non-constant left side
    // NOTE, that v102+ has relaxed this rule a bit, in that it no
    // longer requires the resulting expression to be constant as well.
    if DEE_UNLIKELY(DeeError_CompilerErrorf(
     DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_BEFORE_COLLON_COLLON,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected constant expression before '::' (got: %r)",ast_a) != 0) goto err_a;
   }
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   if (0) {
  case TPP_TOK_ARROW: // Dereference before sub-script on '->'
    ast_result = DeeXAst_NewDeref(token_ob,lexer,parser_flags,ast_a);
    Dee_DECREF(ast_a);
    if DEE_UNLIKELY(!ast_result) return NULL;
    Dee_INHERIT_REF(ast_a,ast_result);
   }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
   DEE_ATTRIBUTE_FALLTHROUGH
  case '.': {
   DeeObject *attr_name;
   Dee_INCREF(ast_token = token_ob);
   if (ast_a->ast_kind == DEE_XASTKIND_MODULE) {
    DeeObject *module_attribute,*deprecated_reason = NULL;
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    module_attribute = _DeeScope_GetNameEx(ast_a->ast_module.m_module,token.tk_id,&deprecated_reason);
    if DEE_UNLIKELY(!module_attribute) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARIABLE_COULD_NOT_FIND_VAR_OR_CONST_IN_MODULE,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Could not find attribute in module") != 0) goto err_a_token;
     module_attribute = Dee_None;
    } else if DEE_UNLIKELY(deprecated_reason) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Deprecated : %r",deprecated_reason) != 0) goto err_a_token;
    }
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    if (DeeScope_Check(module_attribute))
     ast_result = DeeXAst_NewModule(ast_token,(DeeScopeObject *)module_attribute);
    else if (DeeLocalVar_Check(module_attribute))
     ast_result = DeeXAst_NewVar(ast_token,(DeeLocalVarObject *)module_attribute,scope);
    else if (DeeXAst_Check(module_attribute))
     ast_result = DeeXAst_CopyWithScope((DeeXAstObject *)module_attribute,scope,lexer);
    else ast_result = DeeXAst_NewConst(ast_token,module_attribute);
    Dee_DECREF(ast_token);
    if DEE_UNLIKELY(!ast_result) goto err_a;
    Dee_DECREF(ast_a);
    Dee_INHERIT_REF(ast_a,ast_result);
    goto again; // Scan more...
   }
   if DEE_UNLIKELY(!yield()) {err_a_token: Dee_DECREF(ast_token); goto err_a; }
   switch (token.tk_id) {

    case KWD_this: {
     // Syntax food: 'x.this' --> 'x'
     Dee_DECREF(ast_token);
     if DEE_UNLIKELY(!yield()) goto err_a;
     goto again;
    } break;

    case KWD_super: {
     // Super class access
     if DEE_UNLIKELY(!yield()) goto err_a_token;
     ast_result = DeeXAst_NewSuperOf(ast_token,lexer,parser_flags,ast_a);
     Dee_DECREF(ast_token);
     Dee_DECREF(ast_a);
     if DEE_UNLIKELY(!ast_result) return NULL;
     Dee_INHERIT_REF(ast_a,ast_result);
     goto again;
    } break;

    case KWD_class: {
     // Class type access
     if DEE_UNLIKELY(!yield()) goto err_a_token;
     ast_result = DeeXAst_NewClassOf(ast_token,lexer,parser_flags,ast_a);
     Dee_DECREF(ast_token);
     Dee_DECREF(ast_a);
     if DEE_UNLIKELY(!ast_result) return NULL;
     Dee_INHERIT_REF(ast_a,ast_result);
     goto again;
    } break;

    case KWD_operator: {
     int typeslot; // Operator call through attribute
     if DEE_UNLIKELY(!yield()) goto err_a_token;
     if DEE_UNLIKELY((typeslot = DeeXAstOperatorName_Parse(DEE_PARSER_ARGS)) < 0) goto err_a_token;
     ast_result = DeeXAst_ParseOperatorCall(ast_token,ast_a,typeslot,DEE_PARSER_ARGS);
     Dee_DECREF(ast_token);
     Dee_DECREF(ast_a);
     if DEE_UNLIKELY(!ast_result) return NULL;
     Dee_INHERIT_REF(ast_a,ast_result);
     goto again;
    } break;

    default: {
     if DEE_UNLIKELY(!TPPTokenID_IS_KEYWORD(token.tk_id)) {
      if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT,
       (DeeObject *)lexer,(DeeObject *)token_ob,
       "Expected keyword after '.' in expression %r",ast_a) != 0) goto err_a;
      Dee_DECREF(ast_token);
      goto again;
     }
     if DEE_UNLIKELY((attr_name = DeeToken_Str((DeeObject *)token_ob)) == NULL) goto err_a_token;
     ast_result = DeeXAst_NewAttrGetC(ast_token,lexer,parser_flags,ast_a,attr_name);
     Dee_DECREF(attr_name);
     Dee_DECREF(ast_a);
     Dee_DECREF(ast_token);
     if DEE_UNLIKELY(!ast_result) return NULL;
     Dee_INHERIT_REF(ast_a,ast_result);
     if DEE_UNLIKELY(!yield()) goto err_a;
     goto again; // Scan more...
    } break;
   }
  } break;

  case '[': { // item lookup
   DeeXAstObject *ast_begin_or_item,*ast_end;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id != ':') {
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
    if (token.tk_id == ']') { // VarArray suffix
     DeeTypeObject const *predicted_lhs;
     if DEE_UNLIKELY(!yield()) goto err_a_token;
     predicted_lhs = DeeXAst_PredictType(ast_a);
     if DEE_UNLIKELY(predicted_lhs && DEE_UNLIKELY(!DeeStructuredType_Check(predicted_lhs))) {
      if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRUCTURED_TYPE_BEFORE_VARRAY,
       (DeeObject *)lexer,(DeeObject *)token_ob,
       "Expected structured type before '[]', but got %r "
       "evaluating to an instance of %q",ast_a,
       DeeType_NAME(predicted_lhs)) != 0) goto err_a_token;
     }
     ast_result = DeeXAst_NewUnary(DEE_XASTKIND_VARRAYOF,ast_token,lexer,parser_flags,ast_a);
     Dee_DECREF(ast_token);
     Dee_DECREF(ast_a);
     if DEE_UNLIKELY(!ast_result) return NULL;
     Dee_INHERIT_REF(ast_a,ast_result);
     goto again;
    }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
    if DEE_UNLIKELY((ast_begin_or_item = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_a_token;
   } else ast_begin_or_item = NULL;
   if (token.tk_id == ':') {
    if DEE_UNLIKELY(!yield()) {
err_item_begin_a_token: Dee_XDECREF(ast_begin_or_item); goto err_a_token;
    }
    if (token.tk_id != ']') {
     if DEE_UNLIKELY((ast_end = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_item_begin_a_token;
    } else ast_end = NULL;
    ast_result = DeeXAst_NewSeqRangeGet(ast_token,lexer,parser_flags,
                                        ast_a,ast_begin_or_item,ast_end);
    Dee_XDECREF(ast_end);
   } else {
    DEE_ASSERT(ast_begin_or_item != NULL);
    ast_result = DeeXAst_NewBinary(DEE_XASTKIND_SEQ_GET,ast_token,lexer,
                                   parser_flags,ast_a,ast_begin_or_item);
    //Dee_DECREF(ast_begin_or_item);
   }
   Dee_XDECREF(ast_begin_or_item);
   Dee_DECREF(ast_token);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   if (token.tk_id != ']') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_GETATTR,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' after '<expr>[<expr>'") == -1) goto err_a;
   } else if DEE_UNLIKELY(!yield()) goto err_a;
   goto again; // Scan more...
  } break;

  case '{': { // Range initializer
   if ((parser_flags&DEE_PARSER_FLAG_NO_RANGE_INITIALIZER)!=0) break;
   // TODO: Warn if the lhs operand is constant, but not callable
   if DEE_UNLIKELY((ast_result = DeeXAst_ParseRangeInitializerWithType(
    ast_a,DEE_PARSER_ARGS)) == NULL) goto err_a;
   Dee_DECREF(ast_a);
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  } break;

  case '(': { // Function call
   int temp;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id == '{') {
    // Range initializer (w/ single parenthesis)
    Dee_DECREF(ast_token);
    // TODO: Warn if the lhs operand is constant, but not callable
    if DEE_UNLIKELY((ast_result = DeeXAst_ParseRangeInitializerWithType(
     ast_a,DEE_PARSER_ARGS)) == NULL) goto err_a;
    Dee_DECREF(ast_a);
    Dee_INHERIT_REF(ast_a,ast_result);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if DEE_UNLIKELY(token.tk_id == ',' && DEE_UNLIKELY(!yield())) goto err_a;
#endif
    goto end_arglist;
   }
   if (token.tk_id == ')') {
    if DEE_UNLIKELY((ast_args = DeeXAst_NewConst(token_ob,Dee_EmptyTuple)) == NULL) goto err_a_token;
   } else {
    // Check for a recursive c-type
    if DEE_UNLIKELY((temp = DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_ARGS)) < 0) goto err_a_token;
    if DEE_RECURSIVE_CTYPE_UNLIKELY(temp == 0) {
     // This is a recursive c-type
     if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_a_token;
     Dee_DECREF(ast_token);
     break;
    }
    if DEE_UNLIKELY((ast_args = DeeXAst_ParseInitTuple(DEE_PARSER_ARGS)) == NULL) goto err_a_token;
   }
   ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,ast_token,lexer,parser_flags,ast_a,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
end_arglist:
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_TO_END_ARGUMENT_LIST,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' to end argument list") == -1) goto err_a;
   } else if DEE_UNLIKELY(!yield()) goto err_a;
   goto again; // Scan more...
  } break;

  case TPP_TOK_DOTS:
   // Make sure we're allowed to parse expand expressions
   if ((flags&DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND)!=0) break;
   kind = DEE_XASTKIND_EXPAND;
   goto normal_unary;
  case TPP_TOK_INC: kind = DEE_XASTKIND_INCPOST; goto normal_unary;
  case TPP_TOK_DEC: kind = DEE_XASTKIND_DECPOST; normal_unary:
   ast_result = DeeXAst_NewUnary(kind,token_ob,lexer,parser_flags,ast_a);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   if DEE_UNLIKELY(!yield()) {err_a: Dee_DECREF(ast_a); return NULL; }
   goto again;
  default: break;
 }
 return ast_a;
}

DEE_DECL_END
