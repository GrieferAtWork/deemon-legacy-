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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCastSuffix(
 DEE_A_INOUT DeeXAstObject *cast_type, DEE_PARSER_PARAMS) {
 DeeTokenObject *ast_token; int temp;
 DeeXAstObject *cast_result,*cast_argument,*cast_args;
 DEE_ASSERT(DeeObject_Check(cast_type) && DeeXAst_Check(cast_type));
 switch (token.tk_id) {
  case '+': // __pos__   vs. __add__
  case '-': // __neg__   vs. __sub__
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case '*': // __deref__ vs. __mul__
  case '&': // __ref__   vs. __and__
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case '<': // cell {}   vs. __lo__
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX */
#if 1 /*< Don't do special handling for types... */
   if DEE_UNLIKELY(cast_type->ast_kind == DEE_XASTKIND_CONST &&
                   DeeType_Check(cast_type->ast_const.c_const)) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CAST_ONLY_BECAUSE_KNOWN_TYPE,
     (DeeObject *)lexer,(DeeObject *)token_ob,"\n"
     "\tExpression only compiled as a cast, because the left operand is known to be a type.\n"
     "\tSince the token following would also have meaning as a binary operator, it is recommended to revise your code:\n"
     "\t%>r(...)",cast_type) != 0) return NULL;
    goto parse_cast_argument; // We know this is a type, so with the warning given, left generate a cast
   }
#endif
  case '[': // list {}   vs. __getitem__ (NOTE: Also a problem with types)
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_CODER_CAST_INTENTION,
    (DeeObject *)lexer,(DeeObject *)token_ob,"\n"
    "\tSome tokens like '+' and '-' after parenthesis do not generate casts.\n"
    "\tIf this isn't supposed to evaluate as cast, add double the parenthesis: ((%r))...\n"
    "\tIf a cast is intended, change your code to perform a call-style casts: %r(...)",
    cast_type,cast_type) != 0) return NULL;
   break;

  // 'is', 'in' and some other tokens after the ')'
  // are clear indicators of this not being a cast
  DEE_XAST_UNARY_SUFFIX_TOKENS break;
    
  case '(': {
   Dee_uint32_t syntax_mode;
   // Special handle for '(' to all all of the following:
   // >> (float)42;
   // >> (float)(42);
   // >> (foo)(10,20,30);
   // >> (int)(*x)(int,float);
   // >> (int)(float)20.0;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return NULL; }
   if (token.tk_id == ')' || token.tk_id == '{') {
    if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token;
    Dee_DECREF(ast_token);
    break; // Special tokens only allowed in the unary suffix
   }
   if DEE_UNLIKELY((temp = DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_ARGS)) < 0) goto err_ast_token;
   if DEE_RECURSIVE_CTYPE_UNLIKELY(temp == 0) {
    // This is a recursive c-type
    if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token;
    Dee_DECREF(ast_token);
    break;
   }
   syntax_mode = 0;
   if DEE_UNLIKELY((cast_argument = _DeeXAst_ParseInitOrDecl(
    DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT,
    DEE_XASTKIND_TUPLE,NULL,&syntax_mode,DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_TO_END_CAST_WITH_PARENTHESIS,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' to end cast with parenthesis") != 0) {
err_cast_argument: Dee_DECREF(cast_argument); goto err_ast_token;
    }
   } else if DEE_UNLIKELY(!yield()) goto err_cast_argument;
   // v This check is required, because of the following:
   //   >> (foo)(10);            // call w/ 1 argument
   //   >> (foo)(pack 10,20,30); // call w/ 1 argument
   //   >> (foo)(10,20,30);      // call w/ 3 arguments
   if ((syntax_mode&DEE_XAST_INITORDECL_SYNTAXMODE_ISSEQUENCE)==0) {
    // Allow for something like this: '(int)(double)x' (this line parses the 'x')
    cast_args = DeeXAst_ParseCastSuffix(cast_argument,DEE_PARSER_ARGS);
    Dee_DECREF(ast_token);
    Dee_DECREF(cast_argument);
    if DEE_UNLIKELY(!cast_args) goto err_ast_token;
    // Finally, parse one more unary suffix expression
    // >> Required for something like this: '(int)(42).foo' to be compiled as '(int)((42).foo)'
    cast_argument = _DeeXAst_ParseUnarySuffixWith(cast_args,DEE_XAST_UNARAYSUFFIX_FLAG_NONE,
                                                  DEE_PARSER_ARGS);
    Dee_DECREF(cast_args);
    if DEE_UNLIKELY(!cast_argument) return NULL;
    goto use_cast_argument;
   }
   DEE_ASSERT(DeeXAst_IsTuple(cast_argument));
   cast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,ast_token,
                                   lexer,parser_flags,cast_type,cast_argument);
   Dee_DECREF(cast_argument);
   Dee_DECREF(ast_token);
   return cast_result;
  }

  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  DEE_XAST_UNARY_TOKENS DEE_XAST_VARIBLE_LOOKUP_TOKENS
  case '~': case TPP_TOK_INC: case TPP_TOK_DEC:
  case '#': case '!':
parse_cast_argument:
    // Cast operation
    if DEE_UNLIKELY((cast_argument = DeeXAst_ParseUnary(DEE_PARSER_ARGS)) == NULL) return NULL;
use_cast_argument:
    cast_args = DeeXAst_NewTuple(cast_argument->ast_common.ast_token,lexer,parser_flags,1,&cast_argument);
    Dee_DECREF(cast_argument);
    if DEE_UNLIKELY(!cast_args) return NULL;
    cast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,cast_type->ast_common.ast_token,
                                    lexer,parser_flags,cast_type,cast_args);
    Dee_DECREF(cast_args);
    return cast_result;
   }
   break;
 }
 Dee_INCREF(cast_type);
 return cast_type;
}

DEE_DECL_END
