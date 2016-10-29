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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRawUnaryEx(
 DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*ast_args;
 DeeObject *constant_value;
 DeeTokenObject *ast_token; DeeXAstKind kind;
again: switch (token.tk_id) {

  DEE_ATTRIBUTEDECL_TOKENS_NOUNARY
   if (!attr) break;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
   goto again;

  case KWD___builtin_object: constant_value = (DeeObject *)&DeeBuiltinFunction___builtin_object; goto return_constant;
  case KWD___builtin_dex: constant_value = (DeeObject *)&DeeBuiltinFunction___builtin_dex; goto return_constant;
  case KWD_true: constant_value = Dee_True; goto return_constant;
  case KWD_false: constant_value = Dee_False; goto return_constant;
  DEE_PARSE_TOKENID_CASE_NONE constant_value = Dee_None;
return_constant: {
   ast_result = DeeXAst_NewConst(token_ob,constant_value);
   if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;

  case TPP_TOK_INT:
  case TPP_TOK_CHR:
  case TPP_TOK_FLOAT:
  case TPP_TOK_STR:
  case KWD___FUNCTION__:
  case KWD___func__: {
   Dee_INCREF(ast_token = token_ob);
   switch (token.tk_id) {
    case TPP_TOK_STR:
    case KWD___FUNCTION__:
     // v Must use lexer because of preprocessor string concat
     if ((constant_value = DeeLexer_ParseString((DeeObject *)lexer)) == NULL) {
err_ast_token: Dee_DECREF(ast_token); return NULL;
     }
     goto return_const_noyield;
    case TPP_TOK_CHR:
    case TPP_TOK_INT:
     constant_value = DeeToken_EvalInt((DeeObject *)token_ob,(DeeObject *)lexer);
     break;
    case TPP_TOK_FLOAT:
     constant_value = DeeToken_EvalFloat((DeeObject *)token_ob,(DeeObject *)lexer);
     break;
    case KWD___func__:
     constant_value = DeeString_New(TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),lexer->l_func_name));
     break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   if DEE_UNLIKELY(!yield()) { Dee_DECREF(constant_value); goto err_ast_token; }
return_const_noyield:
   ast_result = DeeXAst_NewConst(ast_token,constant_value);
   Dee_DECREF(constant_value);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  case '(': {
   int has_double_parenthesis = 0;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
   if (token.tk_id == '{') {
    DeeSAstObject *expr_statement;
    if ((expr_statement = DeeSAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
    ast_result = DeeXAst_NewStatement(
     expr_statement->ast_common.ast_token,
     lexer,parser_flags,expr_statement);
    Dee_DECREF(expr_statement);
   } else
#endif
   {
    if (token.tk_id == '(') has_double_parenthesis = 1; // Suppress casts
    ast_result = _DeeXAst_ParseInitOrDecl(
     DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT,
     DEE_XASTKIND_TUPLE,NULL,NULL,DEE_PARSER_ARGS);
    if (ast_result && ast_result->ast_kind == DEE_XASTKIND_EXPAND) {
     DeeXAstObject *forced_tuple;
     // '(x...)' is a tuple
     forced_tuple = DeeXAst_NewTuple(ast_token,lexer,parser_flags,1,&ast_result);
     Dee_DECREF(ast_result);
     Dee_INHERIT_REF(ast_result,forced_tuple);
    }
   }
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_TUPLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '(%r",ast_result) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   if (!has_double_parenthesis) {
    ast_args = DeeXAst_ParseCastSuffix(ast_result,DEE_PARSER_ARGS);
    Dee_DECREF(ast_result);
    if (!ast_args) return NULL;
    Dee_INHERIT_REF(ast_result,ast_args);
   }
   return ast_result;
  } break;

  case '[': {
   if DEE_UNLIKELY(!yield()) return NULL;
   if (attr && token.tk_id == '[') {
    if DEE_UNLIKELY(!yield()) return NULL; // Parse attributes
    if (DeeAttributeDecl_ParseAfterSecondBracket(
     attr,DEE_PARSER_ARGS) != 0) return NULL;
    goto again;
   }
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
   if (token.tk_id == '&' || token.tk_id == '=') {
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    if (token.tk_id != ']') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_AND_FOR_LAMBDA,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ']' after '[' '%c' in a lambda expression",
      ast_token->tk_token.tk_id) != 0) goto err_ast_token;
    } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
    if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
deprecated_named_lambda:
     if (DeeError_CompilerError(DEE_WARNING_NAMED_LAMBDA_FUNCTIONS_ARE_DEPRECATED,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Named lambda-functions are deprecated since '__DEEMON__ >= 101'"
      ) != 0) goto err_ast_token;
    }
parse_function:
    ast_result = attr
     ? DeeXAst_ParseFunctionWithAttribute(ast_token,DEE_XAST_VARDECL_MODE_DEFAULT,NULL,attr,DEE_PARSER_ARGS)
     : DeeXAst_ParseFunction(ast_token,DEE_XAST_VARDECL_MODE_DEFAULT,NULL,DEE_PARSER_ARGS);
    Dee_DECREF(ast_token);
    return ast_result;
   }
#endif
   if (token.tk_id == ']') {
    // Empty list / lambda expression
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    switch (token.tk_id) {
     DEE_XAST_UNARY_SUFFIX_TOKENS case KWD_pack: break; // Cannot be used as names
     case '{': case '(': case TPP_TOK_ARROW: case ':':
     DEE_ATTRIBUTEDECL_TOKENS
      goto parse_function;
     default:
      if (TPPTokenID_IS_KEYWORD(token.tk_id))
       goto deprecated_named_lambda;
      break;
    }
    ast_result = DeeXAst_NewEmptyList(ast_token);
    Dee_DECREF(ast_token);
#else
    if ((ast_result = DeeXAst_NewEmptyList(token_ob)) == NULL) return NULL;
    if DEE_UNLIKELY(!yield()) goto err_r;
#endif
    return ast_result;
   }
   if ((ast_result = _DeeXAst_ParseInitOrDecl(
    DEE_XAST_VARDECL_MODE_DEFAULT,
    DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
    DEE_XAST_INITORDECL_FLAGS_RETURNSEQ,
    DEE_XASTKIND_LIST,NULL,NULL,DEE_PARSER_ARGS)) == NULL) return NULL;
   if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_LIST,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' to close list") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;


  case KWD_pack: {
   int has_paren;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   has_paren = (token.tk_id == '(');
   if (has_paren && !yield()) goto err_ast_token;
   if (DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) {
    if ((ast_result = _DeeXAst_ParseInitOrDecl(
     DEE_XAST_VARDECL_MODE_DEFAULT,
     DEE_XAST_INITORDECL_FLAGS_RETURNSEQ,
     DEE_XASTKIND_TUPLE,NULL,NULL,DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   } else if ((ast_result = DeeXAst_NewConst( // Empty packed tuple
    ast_token,Dee_EmptyTuple)) == NULL) goto err_ast_token;
   Dee_DECREF(ast_token);
   if (has_paren) if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_PACK,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'pack(...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;

  case KWD_str:  kind = DEE_XASTKIND_STR;  goto parse_unary_parenopt;
  case KWD_repr: kind = DEE_XASTKIND_REPR; goto parse_unary_parenopt;
  case KWD_copy: kind = DEE_XASTKIND_COPY; goto parse_unary_parenopt;
  case KWD_move: kind = DEE_XASTKIND_MOVE; goto parse_unary_parenopt;
  case KWD_type: kind = DEE_XASTKIND_TYPE; goto parse_unary_parenopt;
  case KWD_weak: kind = DEE_XASTKIND_WEAK; parse_unary_parenopt: {
   int had_parens;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    had_parens = 1;
   } else had_parens = 0;
   if ((ast_args = had_parens
    ? DeeXAst_ParseSingle(DEE_PARSER_ARGS)
    : DeeXAst_ParseUnary(DEE_PARSER_ARGS)
    ) == NULL) goto err_ast_token;
   if (had_parens) {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
    if (token.tk_id != ')') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_STR,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '%s' '(' <expr>",
      TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),ast_token->tk_token.tk_id)
      ) != 0) goto err_args_ast_token;
    } else if DEE_UNLIKELY(!yield()) {
err_args_ast_token: Dee_DECREF(ast_args); goto err_ast_token;
    }
   }
   ast_result = DeeXAst_NewUnary(kind,ast_token,lexer,parser_flags,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  case '!':
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == '!') {
    // "!!" is the bool operator
    kind = DEE_XASTKIND_BOOL;
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    goto parse_unary_noyield;
   }
   kind = DEE_XASTKIND_NOT;
   goto parse_unary_noyield;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case '&': kind = DEE_XASTKIND_REF; goto parse_unary;
  case '*': kind = DEE_XASTKIND_DEREF; goto parse_unary;
  case TPP_TOK_LAND: token.tk_id = '&'; kind = DEE_XASTKIND_REF; goto parse_unary;
  case TPP_TOK_BIN_AND_ASSIGN: token.tk_id = '='; kind = DEE_XASTKIND_REF; goto parse_unary;
  case TPP_TOK_POW: token.tk_id = '*'; kind = DEE_XASTKIND_DEREF; goto parse_unary;
  case TPP_TOK_POW_ASSIGN: token.tk_id = TPP_TOK_MUL_ASSIGN; kind = DEE_XASTKIND_DEREF; goto parse_unary;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
  case KWD_new: kind = DEE_XASTKIND_NEW; goto parse_unary;
  case KWD_delete: kind = DEE_XASTKIND_DELETE; goto parse_unary;
#endif
  case TPP_TOK_INC: kind = DEE_XASTKIND_INC; goto parse_unary;
  case TPP_TOK_DEC: kind = DEE_XASTKIND_DEC; goto parse_unary;
  case '#': kind = DEE_XASTKIND_SEQ_SIZE; goto parse_unary;
  case '~': kind = DEE_XASTKIND_INV; goto parse_unary;
  case '-': kind = DEE_XASTKIND_NEG; goto parse_unary;
  case '+': kind = DEE_XASTKIND_POS; parse_unary: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
parse_unary_noyield:
   if ((ast_args = DeeXAst_ParseUnary(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   ast_result = DeeXAst_NewUnary(kind,ast_token,lexer,parser_flags,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case '<': {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (0) {
  case TPP_TOK_SHL_ASSIGN: token.tk_id = TPP_TOK_SHL; goto cell_begin_noyield;
  case TPP_TOK_SHL: token.tk_id = '<';
cell_begin_noyield: Dee_INCREF(ast_token = token_ob);
   }
   switch (token.tk_id) {
    case '>': {
     DeeObject *empty_cell_const;
     if DEE_UNLIKELY(!yield()) goto err_ast_token;
empty_cell:
     if ((empty_cell_const = DeeCell_NewEmpty()) == NULL) goto err_ast_token;
     ast_result = DeeXAst_NewConst(ast_token,empty_cell_const);
     Dee_DECREF(empty_cell_const);
     Dee_DECREF(ast_token);
    } break;
    case TPP_TOK_SHR: token.tk_id = '>'; goto empty_cell;
    case TPP_TOK_SHR_ASSIGN: token.tk_id = TPP_TOK_SHR; goto empty_cell;
    default:
     if ((ast_args = DeeXAst_ParseUnary(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
     ast_result = DeeXAst_NewCell(ast_token,lexer,parser_flags,ast_args);
     Dee_DECREF(ast_args);
     Dee_DECREF(ast_token);
     if DEE_UNLIKELY(!ast_result) return NULL;
     if (token.tk_id == TPP_TOK_SHR) token.tk_id = '>';
     else if (token.tk_id == TPP_TOK_SHR_ASSIGN) token.tk_id = TPP_TOK_SHR;
     else if (token.tk_id != '>') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_GR_AFTER_LO_FOR_CELL,
       (DeeObject *)lexer,(DeeObject *)token_ob,
       "Expected '>' after '<' [<expr>] to end cell expression") != 0) goto err_r;
     } else if DEE_UNLIKELY(!yield()) goto err_r;
     break;
   }
   return ast_result;
  } break;
#endif

  case KWD_super: {
   DeeScopeObject *weak_root;
   DeeLocalVarObject *this_var;
   DeeXAstObject *this_ast;
   weak_root = DeeScope_WEAK_ROOT(scope);
   if ((weak_root->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS)==0) {
    if (DeeError_CompilerError(DEE_WARNING_SUPER_USED_AS_VARIABLE_NAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,"'super' used as name for a variable"
     ) != 0) return NULL;
    break; // Fall through to a variable lookup
   }
   this_var = (DeeLocalVarObject *)_DeeScope_GetName(weak_root,KWD_this);
   DEE_ASSERT(DeeObject_Check(this_var) && DeeLocalVar_Check(this_var));
   if ((this_ast = DeeXAst_NewLocalVar(token_ob,this_var,scope)) == NULL) return NULL;
   if (!weak_root->sc_supertp) {
    if (DeeError_CompilerError(DEE_WARNING_USE_OF_SUPER_UNSAFE_WITHOUT_ATTRIBUTE_SUPER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Use of 'super' may be unsafe without an explicit __attribute__((super(...)))"
     ) != 0) {
err_this_ast: Dee_DECREF(this_ast); return NULL;
    }
    ast_result = DeeXAst_NewSuperOf(token_ob,lexer,parser_flags,this_ast);
   } else {
    DeeXAstObject *type_in_current_scope;
    if ((type_in_current_scope = DeeXAst_CopyWithScope(
     weak_root->sc_supertp,scope,lexer)) == NULL) goto err_this_ast;
    ast_result = DeeXAst_NewSuperAt(token_ob,lexer,parser_flags,type_in_current_scope,this_ast);
    Dee_DECREF(type_in_current_scope);
   }
   Dee_DECREF(this_ast);
   if DEE_UNLIKELY(!ast_result) return NULL;
   if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;

  case KWD___super:
  case KWD___builtin_super: {
   DeeXAstObject *super_type,*super_object;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER___SUPER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__super'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((super_type = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id == ',') {
    if DEE_UNLIKELY(!yield()) {err_super_type_token: Dee_DECREF(super_type); goto err_ast_token; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')') goto super_no_object;
#endif
    if ((super_object = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_super_type_token;
   } else {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
super_no_object:
#endif
    super_object = NULL;
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER___SUPER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__super(%r,%K'",super_type,super_object
     ? DeeObject_Repr((DeeObject *)super_object) : DeeString_New("this")
     ) != 0) goto err_super_object_token;
   } else if DEE_UNLIKELY(!yield()) {
err_super_object_token: Dee_XDECREF(super_object); goto err_super_type_token;
   }
   // Make sure this is a [[thiscall]] function scope if no object was given
   if (!super_object) {
    DeeLocalVarObject *this_var;
    DeeScopeObject *weak_root = DeeScope_WEAK_ROOT(scope);
    if ((weak_root->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS) == 0) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SECOND___SUPER_ARG_IN_NON_THISCALL,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "'__super' expects a second argument when not used in a [[thiscall]] function") != 0) goto err_super_type_token;
     Dee_DECREF(super_type);
     ast_result = DeeXAst_NewConst(ast_token,Dee_None);
     Dee_DECREF(ast_token);
     return ast_result;
    }
    this_var = (DeeLocalVarObject *)_DeeScope_GetName(weak_root,KWD_this);
    DEE_ASSERT(DeeObject_Check(this_var) && DeeLocalVar_Check(this_var));
    if ((super_object = DeeXAst_NewLocalVar(token_ob,this_var,scope)) == NULL) goto err_super_type_token;
   }
   ast_result = DeeXAst_NewSuperAt(ast_token,lexer,parser_flags,super_type,super_object);
   Dee_DECREF(super_object);
   Dee_DECREF(super_type);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
















//////////////////////////////////////////////////////////////////////////
// -==================================================================- //
// | Statement-like expressions                                       | //
// -==================================================================- //
//////////////////////////////////////////////////////////////////////////
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
  case KWD_if: {
   DeeXAstObject *ast_cond,*ast_succ,*ast_fail;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_EXPR_IF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' in expression after 'if'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_cond = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_EXPR_IF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' in expression after 'if (%r'",ast_cond) != 0) {
err_ast_cond_token: Dee_DECREF(ast_cond); goto err_ast_token;
    }
   } else if DEE_UNLIKELY(!yield()) goto err_ast_cond_token;
   if (token.tk_id == KWD_else) ast_succ = NULL;
   else if ((ast_succ = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_cond_token;
   if (token.tk_id == KWD_else) {
    if DEE_UNLIKELY(!yield()) {err_ast_succ_token: Dee_XDECREF(ast_succ); goto err_ast_cond_token; }
    if ((ast_fail = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_succ_token;
   } else ast_fail = NULL;
   ast_result = DeeXAst_NewIf(ast_token,lexer,parser_flags,ast_cond,ast_succ,ast_fail);
   Dee_XDECREF(ast_fail);
   Dee_XDECREF(ast_succ);
   Dee_DECREF(ast_cond);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
  case KWD_for: case KWD___foreach:
  case KWD_while: case KWD_do:
  case KWD___looponce: case KWD___loopever:
  case KWD___loopnone: {
   DeeScopeObject *generator_root;
   DeeSAstObject *generator_block;
   DeeXAstObject *function_ast,*args_ast;
   DeeSAstKind generator_kind;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((generator_root = (DeeScopeObject *)DeeScope_New((
    DeeObject *)scope,0)) == NULL) goto err_ast_token;
   generator_root->sc_flags |= DEE_SCOPE_FLAG_FOUND_YIELD;
   switch (ast_token->tk_token.tk_id) {
    case KWD_for: case KWD___foreach:
     generator_block = DeeSAst_ParseGeneratorForOrForeachStatement(
      ast_token,(DeeSAstKind)(ast_token->tk_token.tk_id == KWD_for
      ? DEE_SASTKIND_FORIN : DEE_SASTKIND_FOREACH),
      DEE_PARSER_ARGS_SCOPE(generator_root));
     break;
    case KWD_while:
     generator_block = DeeSAst_ParseGeneratorWhileStatement(
      ast_token,DEE_PARSER_ARGS_SCOPE(generator_root));
     break;
    case KWD_do:
     generator_block = DeeSAst_ParseGeneratorDoWhileStatement(
      ast_token,DEE_PARSER_ARGS_SCOPE(generator_root));
     break;
    if (0) { case KWD___looponce: generator_kind = DEE_SASTKIND_LOOPONCE; }
    if (0) { case KWD___loopever: generator_kind = DEE_SASTKIND_LOOPEVER; }
    if (0) { case KWD___loopnone: generator_kind = DEE_SASTKIND_LOOPNONE; }
     generator_block = DeeSAst_ParseGeneratorSpecialLoopStatement(
      generator_kind,ast_token,DEE_PARSER_ARGS_SCOPE(generator_root));
     break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   if (!generator_block) { Dee_DECREF(generator_root); goto err_ast_token; }
   function_ast = DeeXAst_NewFunctionFromInheritedArgv(ast_token,lexer,parser_flags,config,
                                                       attr ? (DeeObject *)attr->a_name : NULL, // TODO: Default name
                                                       DEE_FUNCTION_FLAG_YILD,0,NULL,generator_block,generator_root);
   Dee_DECREF(generator_block);
   Dee_DECREF(generator_root);
   if (!function_ast) goto err_ast_token;
   // Call the function AST with an empty argument tuple
   args_ast = DeeXAst_NewConst(ast_token,Dee_EmptyTuple);
   if (!args_ast) { Dee_DECREF(function_ast); goto err_ast_token; }
   ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,ast_token,lexer,parser_flags,
                                  function_ast,args_ast);
   Dee_DECREF(args_ast);
   Dee_DECREF(function_ast);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_SWITCH_EXPRESSION
  case KWD_switch: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_EXPR_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' in expression after 'switch'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_EXPR_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' in expression after 'switch(%r'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   if (token.tk_id != '{') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LBRACE_AFTER_EXPR_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '{' in expression after 'switch(%r)'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   if (token.tk_id != '}') {
    ast_result = DeeXAst_ParseSwitchBlock(ast_token,ast_args,DEE_PARSER_ARGS);
   } else ast_result = DeeXAst_NewSwitch(ast_token,lexer,parser_flags,ast_args,0,NULL,NULL);
   if (!ast_result) goto err_args_ast_token;
   while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) goto err_r_args_ast_token;
   if (token.tk_id != '}') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACE_AFTER_EXPR_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '}' in expression after 'switch(%r) { ...'",ast_args) != 0) goto err_r_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) {
err_r_args_ast_token: Dee_DECREF(ast_result); goto err_args_ast_token;
   }
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif












//////////////////////////////////////////////////////////////////////////
// -==================================================================- //
// | Builtin functions                                                | //
// -==================================================================- //
//////////////////////////////////////////////////////////////////////////
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
#define _has_paren  require_parens
  case KWD___noop: { // '__noop' can be used just like 'pack'
   int require_parens;
   require_parens = 0;
   if (0) { case KWD___builtin_noop: require_parens = 1; }
   if ((ast_result = DeeXAst_NewConst(token_ob,Dee_None)) == NULL) return NULL;
   if DEE_UNLIKELY(!yield()) return NULL;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) return NULL;
    _has_paren = 1;
   } else if (require_parens) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOOP,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_noop'") != 0) goto err_r;
   } else DEE_ASSERT(_has_paren == 0); // more of an annotation
   // Slim down everything that would otherwise be done, so we don't waste
   // any unnecessary strength on optimizing asts that will just be dropped.
   // NOTE: A minor problem with this (that could and is considered intended)
   //       is that warnings related to valid operations on constants
   //       (e.g.: '1/0' causing warning) will not be generated.
   if ((ast_args = DeeXAst_Parse(DEE_PARSER_ARGS_EX(parser_flags&~(
    // Don't generate constants and don't perform any optimizations
    DEE_PARSER_FLAG_GENERATE_CONSTANT|DEE_PARSER_FLAG_OPTIMIZE_ALL(1))))) == NULL) return NULL;
   Dee_DECREF(ast_args);
   if (require_parens) if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOOP,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_noop(...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#undef _has_paren
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
  case KWD___builtin_constant_p: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_CONSTANT_P,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_constant_p'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
#if 1
   // Keep using the same parser flags
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return NULL;
#else
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return NULL;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_CONSTANT_P,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_constant_p(...'") != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const(ast_args->ast_kind == DEE_XASTKIND_CONST));
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
  case KWD__Noexcept:
  case KWD___builtin_noexcept: {
   DeeAstAttribute attr;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOEXCEPT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_noexcept'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   attr = DeeXAst_Attr(ast_result,DEE_AST_ATTRIBUTE_FLAG_NONE|(Dee_uint32_t)
                     ((parser_flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0
                       ? DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK : 0));
   Dee_DECREF(ast_result);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NOEXCEPT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_noexcept(...'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const((
    attr&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0));
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
  case KWD__Noreturn:
  case KWD___builtin_noreturn: {
   DeeAstAttribute attr;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NORETURN,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_noreturn'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   attr = DeeXAst_Attr(ast_result,DEE_AST_ATTRIBUTE_FLAG_NONE|(Dee_uint32_t)
                     ((parser_flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0
                       ? DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK : 0));
   Dee_DECREF(ast_result);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NORETURN,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_noreturn(...'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const((
    attr&DEE_AST_ATTRIBUTE_NORETURN)!=0));
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
  case KWD___builtin_noeffect: {
   DeeAstAttribute attr;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOEFFECT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_noeffect'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   attr = DeeXAst_Attr(ast_result,DEE_AST_ATTRIBUTE_FLAG_NONE|(Dee_uint32_t)
                     ((parser_flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0
                       ? DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK : 0));
   Dee_DECREF(ast_result);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NOEFFECT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_noeffect(...'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const((
    attr&DEE_AST_ATTRIBUTE_NOEFFECT)!=0));
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
  case KWD___builtin_choose_expr: {
   int choose_succ_branch;
   if DEE_UNLIKELY(!yield()) return NULL;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_CHOOSE_EXPR,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_choose_expr'"
     ) != 0) return NULL;
   } else if DEE_UNLIKELY(!yield()) return NULL;
   if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return NULL;
   if (ast_result->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_CHOOSE_EXPR,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected constant after '__builtin_choose_expr(', but got %r",ast_result) != 0) goto err_r;
    choose_succ_branch = 0;
   } else {
    // No need to copy the constant, as we won't be re-using this branch
    choose_succ_branch = DeeObject_Bool(ast_result->ast_const.c_const);
    if (choose_succ_branch < 0) {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_BOOL_EXPRESSION_FOR_BUILTIN_CHOOSE_EXPR,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Failed to convert constant instance of %q in'__builtin_choose_expr' to bool: %K",
      DeeType_NAME(Dee_TYPE(ast_result->ast_const.c_const)),
      _DeeError_HandleAndReturnType()) != 0) goto err_r;
     choose_succ_branch = 0;
    }
   }
   Dee_DECREF(ast_result);
   if (token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_CHOOSE_EXPR_1,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ',' after '__builtin_choose_expr(%s'",
     choose_succ_branch ? "true" : "false") != 0) return NULL;
   } else if DEE_UNLIKELY(!yield()) return NULL;
   ast_result = NULL;
   if (choose_succ_branch) {
    if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return NULL;
   } else if (DeeLexer_SkipTokensUntil((DeeObject *)lexer,(DeeObject *)scope,config,',',')') != 0) return NULL;
   if (token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_CHOOSE_EXPR_2,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ',' after '__builtin_choose_expr(%s,...'",
     choose_succ_branch ? "true" : "false") != 0) {err_r_maybe: Dee_XDECREF(ast_result); return NULL; }
   } else if DEE_UNLIKELY(!yield()) goto err_r_maybe;
   if (!choose_succ_branch) {
    if ((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return NULL;
   } else if (DeeLexer_SkipTokensUntil((DeeObject *)lexer,(DeeObject *)scope,config,',',')') != 0) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_CHOOSE_EXPR,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_choose_expr(%s,...,...'",
     choose_succ_branch ? "true" : "false") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case KWD___builtin_unreachable: {
   if ((ast_result = DeeXAst_NewZeroOp(
    DEE_XASTKIND_BUILTIN_UNREACHABLE,token_ob)) == NULL) return NULL;
   if DEE_UNLIKELY(!yield()) goto err_r;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_UNREACHABLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_unreachable'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_UNREACHABLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_unreachable('"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
  case KWD___builtin_types_compatible_p: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__builtin_types_compatible_p'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_ParseTypesCompatibleP(ast_token,DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT));
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_types_compatible_p(...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case KWD___builtin_breakpoint: {
   if ((ast_result = DeeXAst_NewZeroOp(
    DEE_XASTKIND_BUILTIN_BREAKPOINT,token_ob)) == NULL) return NULL;
   if DEE_UNLIKELY(!yield()) goto err_r;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_BREAKPOINT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_breakpoint'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_BREAKPOINT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_breakpoint('"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
  case KWD___builtin_typeof: {
   DeeTypeObject const *predicted_type;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_TYPEOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__builtin_typeof'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_TYPEOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_typeof(%r'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   if ((predicted_type = DeeXAst_PredictType(ast_args)) == NULL) {
    if (DeeError_CompilerErrorf(DEE_WARNING_UNPREDICTABLE_TYPE_IN_BUILTIN_TYPEOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__builtin_typeof cannot predict the type of %r",
     ast_args) != 0) goto err_args_ast_token;
    predicted_type = (DeeTypeObject *)&DeeNone_Type;
   }
   Dee_DECREF(ast_args);
   ast_result = DeeXAst_NewConst(ast_token,(DeeObject *)predicted_type);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
  case KWD___builtin_predictable: {
   int is_predictable;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_PREDICTABLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__builtin_predictable'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_PREDICTABLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_predictable(%r'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   is_predictable = DeeXAst_PredictType(ast_args) != NULL;
   Dee_DECREF(ast_args);
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const(is_predictable));
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case KWD___builtin_help: {
   if (DeeError_CompilerError(DEE_WARNING_BUILTIN_HELP_DEPRECATED,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "'__builtin_help' is deprecated; Use the '_docgen' dex instead.") != 0) return NULL;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_HELP,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__builtin_help'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_NO_OPTIMIZE_SUBSCRIPT))) == NULL) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_HELP,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_help(%r'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   ast_result = DeeXAst_NewBuiltinHelp(ast_token,lexer,parser_flags,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case KWD___builtin_expect: {
   DeeObject *expected_value;
   DeeXAstObject *expected_value_ast;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXPECT,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__builtin_expect'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_EXPECT,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ',' after '__builtin_expect(%r'",ast_args) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   if ((expected_value_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_expected_var_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_EXPECT,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_expect(%r,%r'",
     ast_args,expected_value_ast) != 0) goto err_expected_var_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) {
err_expected_var_args_ast_token: Dee_DECREF(expected_value_ast); goto err_args_ast_token;
   }
   if (expected_value_ast->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_BUILTIN_EXPECT,
     (DeeObject *)lexer,(DeeObject *)expected_value_ast->ast_common.ast_token,
     "Expected constant expression in '__builtin_expect(%r,...)'",
     expected_value_ast) != 0) goto err_expected_var_args_ast_token;
    expected_value = DeeNone_New();
   } else Dee_INCREF(expected_value = expected_value_ast->ast_const.c_const);
   Dee_DECREF(expected_value_ast);
   ast_result = DeeXAst_NewBuiltinExpect(ast_token,ast_args,expected_value);
   Dee_DECREF(expected_value);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
  case KWD___builtin_offsetof: {
   DeeTypeObject *offsetof_base;
   DeeXAstObject *offsetof_base_ast;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_offsetof'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((offsetof_base_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_ast_token;
   if (offsetof_base_ast->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)offsetof_base_ast->ast_common.ast_token,
     "Expected constant expression in '__builtin_offsetof', but got %r",offsetof_base_ast) != 0) {
     Dee_DECREF(offsetof_base_ast);
     goto err_ast_token;
    }
    offsetof_base = (DeeTypeObject *)&DeeNone_Type;
   } else Dee_INCREF(offsetof_base = (DeeTypeObject *)offsetof_base_ast->ast_const.c_const);
   Dee_DECREF(offsetof_base_ast);
   if (DeeNone_Check(offsetof_base)) {
    Dee_DECREF(offsetof_base);
    Dee_INCREF(offsetof_base = (DeeTypeObject *)&DeeNone_Type);
   } else if (!DeeType_Check(offsetof_base)) {
    DeeTypeObject *temp;
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_AFTER_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)offsetof_base_ast->ast_common.ast_token,
     "Expected constant type in '__builtin_offsetof', but got instance of %q",
     DeeType_NAME(Dee_TYPE(offsetof_base))) != 0) {
err_offset_base: Dee_DECREF(offsetof_base); goto err_ast_token;
    }
    Dee_INCREF(temp = Dee_TYPE(offsetof_base));
    Dee_DECREF(offsetof_base);
    Dee_INHERIT_REF(offsetof_base,temp);
   }
   if (token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)offsetof_base_ast->ast_common.ast_token,
     "Expected ',' after '__builtin_offsetof(%q'",DeeType_NAME(offsetof_base)
     ) != 0) goto err_offset_base;
   } else if DEE_UNLIKELY(!yield()) goto err_offset_base;
   ast_result = DeeXAst_ParseOffsetofSuffix(ast_token,offsetof_base,DEE_PARSER_ARGS);
   Dee_DECREF(offsetof_base);
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_OFFSETOF,
     (DeeObject *)lexer,(DeeObject *)offsetof_base_ast->ast_common.ast_token,
     "Expected ')' after '__builtin_offsetof(...'") != 0) goto err_offset_base;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif




#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
  case KWD___builtin_exprast: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXPRAST,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_exprast'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   // NOTE: We disable generation of constant functions on purpose!
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags&~(DEE_PARSER_FLAG_CONST_FUNCTION)))) == NULL) goto err_ast_token;
#if 1
   { Dee_size_t performed_optimizations; do {
    struct DeeOptimizerAssumptions assumptions; int error;
    performed_optimizations = 0;
    _DeeOptimizerAssumptions_InitRoot(&assumptions,scope);
    error = DeeXAst_Optimize(ast_args,&performed_optimizations,lexer,scope,config,
                             config->optimize_flags|DEE_OPTIMIZE_FLAG_USED,&assumptions);
    _DeeOptimizerAssumptions_Quit(&assumptions);
    if (error != 0) goto err_args_ast_token;
   } while (performed_optimizations); }
#endif
   ast_result = DeeXAst_NewConst(ast_token,(DeeObject *)ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_EXPRAST,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_exprast(...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case KWD___builtin_alloca: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_ALLOCA,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_alloca'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_args_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_ALLOCA,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_alloca'"
     ) != 0) goto err_args_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_args_ast_token;
   ast_result = DeeXAst_NewBuiltinAlloca(ast_token,lexer,parser_flags,ast_args);
   Dee_DECREF(ast_args);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case KWD___builtin_bound: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_BOUND,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_bound'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_ParseBuiltinBoundExpression(ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_r;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_BOUND,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_bound(...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
  case KWD___builtin_exists: {
   int exists_value;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXISTS,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_exists'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((exists_value = DeeXAst_ParseExistsExpression(DEE_PARSER_ARGS)) < 0) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_ast_token;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_EXISTS,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_exists(...'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeXAst_NewConst(ast_token,DeeBool_Const(exists_value));
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED
  case KWD___builtin_unused: {
   DeeXAstObject *ast_eval,*ast_override;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_UNUSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_unused'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if DEE_UNLIKELY((ast_eval = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id == ',') {
    if DEE_UNLIKELY(!yield()) {err_used_override_ast_eval: Dee_DECREF(ast_eval); goto err_ast_token; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')') ast_override = NULL; else
#endif
    {
     if DEE_UNLIKELY((ast_override = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL)
      goto err_used_override_ast_eval;
    }
   } else ast_override = NULL;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_UNUSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__builtin_unused(%r,%K'",
     ast_eval,ast_override ? DeeObject_Repr((DeeObject *)ast_override)
     : DeeNone_New()) != 0) goto err_used_override_ast_override;
   } else if DEE_UNLIKELY(!yield()) {
err_used_override_ast_override: Dee_XDECREF(ast_override); goto err_used_override_ast_eval;
   }
   if DEE_UNLIKELY(!ast_override && (ast_override = DeeXAst_NewConst(ast_token,Dee_None)) == NULL)
    goto err_used_override_ast_eval;
   ast_result = DeeXAst_NewBuiltinUnused(ast_token,lexer,parser_flags,
                                  ast_eval,ast_override);
   Dee_DECREF(ast_override);
   Dee_DECREF(ast_eval);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD
  case KWD___builtin_dead: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_DEAD,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__builtin_dead'"
     ) != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if DEE_UNLIKELY((ast_args = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   ast_result = DeeXAst_NewBuiltinDead(ast_token,lexer,parser_flags,ast_args);
   Dee_DECREF(ast_args);
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_DEAD,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__builtin_dead(%r'",
     ast_result->ast_operator.op_a) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
   return ast_result;
  } break;
#endif

#ifdef DEE_XAST_RESERVED_KEYWORD_TOKENS
  DEE_XAST_RESERVED_KEYWORD_TOKENS
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Using reserved keyword"
    ) != 0) return NULL;
   DEE_ATTRIBUTE_FALLTHROUGH
#endif
  default: break;
 }
 return DeeXAst_ParseType(vardecl_mode,attr,DEE_PARSER_ARGS);
err_r: Dee_DECREF(ast_result); return NULL;
}

DEE_DECL_END
