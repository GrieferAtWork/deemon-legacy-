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
#ifndef GUARD_DEEMON_XAST_PARSE_C
#define GUARD_DEEMON_XAST_PARSE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
#include <deemon/optional/uuid.h>
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */

#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/class.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/xast.h>
#include <deemon/struct.h>
#include <deemon/deemonrun.h>
#include <deemon/optional/uuid.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/runtime/function.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/set.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


#define yield()  DeeLexer_YieldEx((DeeObject *)lexer,(DeeObject *)scope,config)
#define token_ob lexer->l_token
#define token    lexer->l_token->tk_token


DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_ParseConst(
 DEE_A_REF DEE_A_OUT_OPT DeeTokenObject **const_token, DEE_PARSER_PARAMS) {
 DeeXAstObject *constant_ast; DeeObject *result;
 if DEE_UNLIKELY((constant_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return NULL;
 if DEE_UNLIKELY(constant_ast->ast_kind != DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected constant, but got %r",
   constant_ast) != 0) return NULL;
  if (const_token) Dee_INCREF(*const_token = constant_ast->ast_common.ast_token);
  Dee_DECREF(constant_ast);
  DeeReturn_None;
 }
 Dee_INCREF(result = constant_ast->ast_const.c_const);
 if (const_token) Dee_INCREF(*const_token = constant_ast->ast_common.ast_token);
 Dee_DECREF(constant_ast);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnaryEx(
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_result;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseRawUnaryEx(vardecl_mode,attr,DEE_PARSER_ARGS)) == NULL) return NULL;
 ast_result = _DeeXAst_ParseUnarySuffixWith(ast_a,flags,DEE_PARSER_ARGS);
 Dee_DECREF(ast_a);
 return ast_result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseProdEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseUnaryEx(
  DEE_XAST_UNARAYSUFFIX_FLAG_NONE,vardecl_mode,
  NULL,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case '*':
  case TPP_TOK_POW:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if DEE_UNLIKELY(!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) goto recursive_c_type;
   {int temp; // Check for pointer type
    if DEE_UNLIKELY((temp = DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_ARGS)) < 0) goto err_a_token;
    if DEE_RECURSIVE_CTYPE_UNLIKELY(temp == 0) { // This is a recursive c-type
recursive_c_type:
     if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)operator_token) != 0) goto err_a_token;
     Dee_DECREF(operator_token);
     return ast_a;
    }
   }
   kind = (DeeXAstKind)(operator_token->tk_token.tk_id == '*'
                        ? DEE_XASTKIND_MUL : DEE_XASTKIND_POW);
   goto normal_binary_noyield;
#else /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case '*': kind = DEE_XASTKIND_MUL; goto normal_binary;
  case TPP_TOK_POW: kind = DEE_XASTKIND_POW; goto normal_binary;
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case '/': kind = DEE_XASTKIND_DIV; goto normal_binary;
  case '%': kind = DEE_XASTKIND_MOD;
normal_binary:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
normal_binary_noyield:
#endif
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseUnary(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  default: break;
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSumEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseProdEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
  {
   if (0) { case '+': kind = DEE_XASTKIND_ADD; }
   if (0) { case '-': kind = DEE_XASTKIND_SUB; }
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id == TPP_TOK_DOTS && kind == DEE_XASTKIND_ADD) {
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    ast_result = DeeXAst_NewUnary(DEE_XASTKIND_SEQ_SUM,operator_token,lexer,parser_flags,ast_a);
   } else {
    if DEE_UNLIKELY((ast_b = DeeXAst_ParseProd(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
     Dee_DECREF(ast_a);
     return NULL;
    }
    ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
    Dee_DECREF(ast_b);
   }
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  }
  default: break;
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseShiftEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseSumEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
  case TPP_TOK_SHL: kind = DEE_XASTKIND_SHL; goto normal_binary;
  case TPP_TOK_SHR: kind = DEE_XASTKIND_SHR; normal_binary:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
//normal_binary_token:
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseSum(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  default: break;
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCmpEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseShiftEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
  case TPP_TOK_CMP_LO: kind = DEE_XASTKIND_COMPARE_LO; goto normal_binary;
  case TPP_TOK_CMP_LE: kind = DEE_XASTKIND_COMPARE_LE; goto normal_binary;
  case TPP_TOK_CMP_GR: kind = DEE_XASTKIND_COMPARE_GR; goto normal_binary;
  case TPP_TOK_CMP_GE: kind = DEE_XASTKIND_COMPARE_GE; normal_binary:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id == TPP_TOK_DOTS && (
    kind == DEE_XASTKIND_COMPARE_LO || kind == DEE_XASTKIND_COMPARE_GR)) {
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    ast_result = DeeXAst_NewUnary((DeeXAstKind)(kind == DEE_XASTKIND_COMPARE_LO
     ? DEE_XASTKIND_SEQ_MIN : DEE_XASTKIND_SEQ_MAX),operator_token,lexer,parser_flags,ast_a);
   } else {
    if DEE_UNLIKELY((ast_b = DeeXAst_ParseShift(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
     Dee_DECREF(ast_a);
     return NULL;
    }
    ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
    Dee_DECREF(ast_b);
   }
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  default: break;
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCmpEqEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseCmpEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
  case '!':
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   switch (token.tk_id) {
    case KWD_in: kind = DEE_XASTKIND_NOT_IN; break;
    case KWD_is: kind = DEE_XASTKIND_NOT_IS; break;
    default: // If we didn't find what we were looking for, return the token we took.
     if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)operator_token) != 0) goto err_a_token;
     Dee_DECREF(operator_token);
     return ast_a;
   }
   Dee_DECREF(operator_token);
   goto normal_binary;
  case KWD_in:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id == '!') {
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    kind = DEE_XASTKIND_NOT_IN;
   } else kind = DEE_XASTKIND_IN;
   goto normal_binary_token;
  case KWD_is:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   if (token.tk_id == '!') {
    if DEE_UNLIKELY(!yield()) goto err_a_token;
    kind = DEE_XASTKIND_NOT_IS;
   } else kind = DEE_XASTKIND_IS;
   goto normal_binary_token;
  case TPP_TOK_CMP_EQ: kind = DEE_XASTKIND_COMPARE_EQ; goto normal_binary;
  case TPP_TOK_CMP_NE: kind = DEE_XASTKIND_COMPARE_NE; normal_binary:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
normal_binary_token:
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseCmp(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  default: break;
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAndEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseCmpEqEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == '&') {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  if DEE_RECURSIVE_CTYPE_UNLIKELY(!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) goto recursive_c_type;
  {int temp; // Check for lvalue type
   if DEE_UNLIKELY((temp = DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_ARGS)) < 0) goto err_a_token;
   if DEE_RECURSIVE_CTYPE_UNLIKELY(temp == 0) {
recursive_c_type:
    if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)operator_token) != 0) goto err_a_token;
    Dee_DECREF(operator_token);
    return ast_a; // This is a recursive c-type
   }
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  if DEE_UNLIKELY((ast_b = DeeXAst_ParseCmpEq(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   return NULL;
  }
  ast_result = DeeXAst_NewBinary(DEE_XASTKIND_AND,operator_token,lexer,parser_flags,ast_a,ast_b);
  Dee_DECREF(operator_token);
  Dee_DECREF(ast_b);
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseXorEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseAndEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == '^') {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
  if DEE_UNLIKELY((ast_b = DeeXAst_ParseAnd(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   return NULL;
  }
  ast_result = DeeXAst_NewBinary(DEE_XASTKIND_XOR,operator_token,lexer,parser_flags,ast_a,ast_b);
  Dee_DECREF(operator_token);
  Dee_DECREF(ast_b);
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOrEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseXorEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == '|') {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
  if DEE_UNLIKELY((ast_b = DeeXAst_ParseXor(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   return NULL;
  }
  ast_result = DeeXAst_NewBinary(DEE_XASTKIND_OR,operator_token,lexer,parser_flags,ast_a,ast_b);
  Dee_DECREF(operator_token);
  Dee_DECREF(ast_b);
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLandEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseOrEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == TPP_TOK_LAND) {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
  if (token.tk_id == TPP_TOK_DOTS) {
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   ast_result = DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ALL,operator_token,lexer,parser_flags,ast_a);
   Dee_DECREF(operator_token);
  } else {
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseOr(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(DEE_XASTKIND_LAND,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
  }
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLxorEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseLandEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == TPP_TOK_LXOR) {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
  if DEE_UNLIKELY((ast_b = DeeXAst_ParseLand(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
   Dee_DECREF(ast_a);
   return NULL;
  }
  ast_result = DeeXAst_NewBinary(DEE_XASTKIND_LXOR,operator_token,lexer,parser_flags,ast_a,ast_b);
  Dee_DECREF(operator_token);
  Dee_DECREF(ast_b);
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLorEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseLxorEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == TPP_TOK_LOR) {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_a_token;
  if (token.tk_id == TPP_TOK_DOTS) {
   if DEE_UNLIKELY(!yield()) goto err_a_token;
   ast_result = DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ANY,operator_token,lexer,parser_flags,ast_a);
   Dee_DECREF(operator_token);
  } else {
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseLxor(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(DEE_XASTKIND_LOR,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
  }
  Dee_DECREF(ast_a);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_a,ast_result);
 }
 return ast_a;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseIfEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_cond,*ast_succ,*ast_fail,*ast_result; DeeTokenObject *operator_token;
 if ((ast_cond = DeeXAst_ParseLorEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
 while (token.tk_id == '?') {
  Dee_INCREF(operator_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_cond_tok;
  if (token.tk_id == ':') ast_succ = NULL;
  else if DEE_UNLIKELY((ast_succ = DeeXAst_ParseInit(DEE_PARSER_ARGS)) == NULL) {
err_cond_tok: Dee_DECREF(operator_token);
   Dee_DECREF(ast_cond);
   return NULL;
  }
  if (token.tk_id == ':') {
   if DEE_UNLIKELY(!yield()) {
err_succ_cond_tok: Dee_XDECREF(ast_succ); goto err_cond_tok;
   }
   if ((ast_fail = DeeXAst_ParseIf(DEE_PARSER_ARGS)) == NULL) goto err_succ_cond_tok;
  } else ast_fail = NULL;
  ast_result = DeeXAst_NewIf(operator_token,lexer,parser_flags,
                             ast_cond,ast_succ,ast_fail);
  Dee_XDECREF(ast_fail);
  Dee_XDECREF(ast_succ);
  Dee_DECREF(operator_token);
  Dee_DECREF(ast_cond);
  if DEE_UNLIKELY(!ast_result) return NULL;
  Dee_INHERIT_REF(ast_cond,ast_result);
 }
 return ast_cond;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAssignEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_a,*ast_b,*ast_result; DeeXAstKind kind;
 DeeTokenObject *operator_token;
 if DEE_UNLIKELY((ast_a = DeeXAst_ParseIfEx(vardecl_mode,DEE_PARSER_ARGS)) == NULL) return NULL;
again: switch (token.tk_id) {
  case TPP_TOK_COLLON_ASSIGN:  kind = DEE_XASTKIND_COPY_ASSIGN; goto normal_binary;
  case TPP_TOK_ADD_ASSIGN:     kind = DEE_XASTKIND_IADD; goto normal_binary;
  case TPP_TOK_SUB_ASSIGN:     kind = DEE_XASTKIND_ISUB; goto normal_binary;
  case TPP_TOK_MUL_ASSIGN:     kind = DEE_XASTKIND_IMUL; goto normal_binary;
  case TPP_TOK_DIV_ASSIGN:     kind = DEE_XASTKIND_IDIV; goto normal_binary;
  case TPP_TOK_MOD_ASSIGN:     kind = DEE_XASTKIND_IMOD; goto normal_binary;
  case TPP_TOK_SHL_ASSIGN:     kind = DEE_XASTKIND_ISHL; goto normal_binary;
  case TPP_TOK_SHR_ASSIGN:     kind = DEE_XASTKIND_ISHR; goto normal_binary;
  case TPP_TOK_BIN_AND_ASSIGN: kind = DEE_XASTKIND_IAND; goto normal_binary;
  case TPP_TOK_BIN_OR_ASSIGN:  kind = DEE_XASTKIND_IOR;  goto normal_binary;
  case TPP_TOK_BIN_XOR_ASSIGN: kind = DEE_XASTKIND_IXOR; goto normal_binary;
  case TPP_TOK_POW_ASSIGN:     kind = DEE_XASTKIND_IPOW; normal_binary:
   Dee_INCREF(operator_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_a_token;
//normal_binary_token:
   if DEE_UNLIKELY((ast_b = DeeXAst_ParseIf(DEE_PARSER_ARGS)) == NULL) {
err_a_token: Dee_DECREF(operator_token);
    Dee_DECREF(ast_a);
    return NULL;
   }
   ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_a,ast_b);
   Dee_DECREF(operator_token);
   Dee_DECREF(ast_b);
   Dee_DECREF(ast_a);
   if DEE_UNLIKELY(!ast_result) return NULL;
   Dee_INHERIT_REF(ast_a,ast_result);
   goto again;
  default: break;
 }
 return ast_a;
}






DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOperatorCall(
 DEE_A_INOUT DeeTokenObject *operator_token,
 DEE_A_INOUT DeeXAstObject *ast_this, DEE_A_IN int typeslot, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*ast_args;
 if DEE_UNLIKELY(token.tk_id != '(') {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LPAREN_AFTER_SUBSCRIPT_OPERATOR,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected '(' after '%r.operator %s'",
   ast_this,_DeeType_ClassOperatorName(typeslot)) != 0) return NULL;
 } else if DEE_UNLIKELY(!yield()) return NULL;
 if DEE_UNLIKELY((ast_args = (token.tk_id == ')')
  ? DeeXAst_NewConst(token_ob,Dee_EmptyTuple)
  : DeeXAst_ParseInitTuple(DEE_PARSER_ARGS)
  ) == NULL) return NULL;
 if DEE_UNLIKELY(token.tk_id != ')') {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_SUBSCRIPT_OPERATOR,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ')' after '%r.operator %s (...'",
   ast_this,_DeeType_ClassOperatorName(typeslot)) != 0) {
err_ast_args: Dee_DECREF(ast_args); return NULL;
  }
 } else if DEE_UNLIKELY(!yield()) goto err_ast_args;
 ast_result = DeeXAst_NewOperatorCall(operator_token,ast_this,ast_args,
                                      typeslot,lexer,parser_flags);
 Dee_DECREF(ast_args);
 return ast_result;
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_PARAMS) {
 int result;
 Dee_size_t       backup_tokena = 0;
 Dee_size_t       backup_tokenc = 0;
 DeeTokenObject **backup_tokenv = NULL;
 DeeTokenObject **new_backup_tokenv;
 Dee_uint32_t vardecl_mode = DEE_XAST_VARDECL_MODE_DEFAULT;
 (void)parser_flags;
#define DO_BACKUP_TOKEN(tk,on_error)\
do{\
  if DEE_UNLIKELY(backup_tokenc == backup_tokena) {\
   if DEE_UNLIKELY(!backup_tokena) backup_tokena = 2; else backup_tokena *= 2;\
   while DEE_UNLIKELY((new_backup_tokenv = (DeeTokenObject **)realloc_nz(\
    backup_tokenv,backup_tokena*sizeof(DeeTokenObject *))) == NULL) {\
    if DEE_LIKELY(Dee_CollectMemory()) continue;\
    DeeError_NoMemory();\
    {on_error;}\
    goto backup_error;\
   }\
   backup_tokenv = new_backup_tokenv;\
  }\
  Dee_INCREF(backup_tokenv[backup_tokenc++] = (tk));\
}while(0)
 while (1) {
  switch (token.tk_id) {
   case '*': case TPP_TOK_POW:  break; // Ignored token (pointer type)
   case '&': case TPP_TOK_LAND: break; // Ignored token (lvalue type)
   case '(': break; // Ignored token (recursive type)

   case ')':
    // This can happen in situations like:
    //        v-- this token
    // 'int (*)()'
    result = 0;
    goto end;

   // Attribute tokens
   case KWD__syssv: case KWD___syssv:
   case KWD__stdcall: case KWD___stdcall:
   case KWD__thiscall: case KWD___thiscall:
   case KWD__fastcall: case KWD___fastcall:
   case KWD__cdecl: case KWD___cdecl:
   case KWD__pascal: case KWD___pascal:
   case KWD__register: case KWD___register:
   case KWD__win64: case KWD___win64:
   case KWD__unix64: case KWD___unix64: break;

   case KWD__Alignas:
   case KWD__Noexcept:
   case KWD__Noreturn:
   case KWD__declspec:
   case KWD___declspec:
   case KWD___attribute:
   case KWD___attribute__: {
    int paren_recursion,bracket_recursion,brace_recursion;
                       paren_recursion = 1; bracket_recursion = 0; brace_recursion = 0;
    if (0) { case '[': paren_recursion = 0; bracket_recursion = 1; brace_recursion = 0; }
    DO_BACKUP_TOKEN(token_ob,{});
    if (token.tk_id != '[') {
     if DEE_UNLIKELY(!yield()) goto err_backup;
     if (token.tk_id != '(') goto next_token;
    } else if DEE_UNLIKELY(!yield()) goto err_backup;
    goto skip_attribute_begin;
    while (paren_recursion || bracket_recursion || brace_recursion) {
     switch (token.tk_id) {
      case 0: result = 1; goto end;
      case '(': ++paren_recursion; break;
      case ')': --paren_recursion; break;
      case '[': ++bracket_recursion; break;
      case ']': --bracket_recursion; break;
      case '{': ++brace_recursion; break;
      case '}': --brace_recursion; break;
      default: break;
     }
skip_attribute_begin:
     DO_BACKUP_TOKEN(token_ob,{});
     if DEE_UNLIKELY(!yield()) goto err_backup;
    }
   } break;
    
  case KWD_module: /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_MODULE;*/ break;
  case KWD_forward: /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_FORWARD;*/ break;
  DEE_PARSE_TOKENID_CASE_CONST /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_CONST;*/ break;
  DEE_PARSE_TOKENID_CASE_VOLATILE /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_VOLATILE;*/ break;
  case KWD_static: /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_STATIC;*/ break;
#ifdef DEE_PARSE_TOKENID_CASE_STACK
  DEE_PARSE_TOKENID_CASE_STACK /*vardecl_mode |= DEE_XAST_VARDECL_FLAG_STACK;*/ break;
#endif
  case KWD_local: vardecl_mode = /*DEE_XAST_VARDECL_FLAGS(vardecl_mode)|*/DEE_XAST_VARDECL_MODE_LOCAL; break;
  DEE_PARSE_TOKENID_CASE_GLOBAL vardecl_mode = /*DEE_XAST_VARDECL_FLAGS(vardecl_mode)|*/DEE_XAST_VARDECL_MODE_GLOBAL; break;

   default:
    if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
     switch (/*DEE_XAST_VARDECL_MODE*/(vardecl_mode)) {
      case DEE_XAST_VARDECL_MODE_LOCAL:
       if (_DeeScope_HasName(scope,token.tk_id)) goto var_found;
       break;
      case DEE_XAST_VARDECL_MODE_GLOBAL:
       if (_DeeScope_HasName((DeeScopeObject *)DeeScope_Global((DeeObject *)scope),token.tk_id)) goto var_found;
       break;
      default: while (1) {
       if (_DeeScope_HasName(scope,token.tk_id)) {var_found: result = 1; goto end; } // Yes, it does exist
       if (DeeScope_WEAK_PREV(scope)) scope = (DeeScopeObject *)DeeScope_WEAK_PREV(scope);
       else if (DeeScope_PREV(scope)) scope = (DeeScopeObject *)DeeScope_PREV(scope);
       else break;
      } break;
     }
     result = 0; // No, doesn't exist, meaning this can be used as a c-type
     goto end;
    } else {
     // Unexpected token --> not a recursive c-type
     // NOTE: Also listed are tokens that can never be used as c-type names
   DEE_XAST_UNARY_TOKENS
   DEE_XAST_VARIABLE_DECL_TOKENS
   case KWD___nth: case KWD___scope: // These variable modifiers can't be used for c-type names
     result = 1;
     goto end;
    }
    break;
  }
#undef DO_BACKUP_TOKEN
/*backup_and_yield:*/

  if DEE_UNLIKELY(backup_tokenc == backup_tokena) {
   if DEE_UNLIKELY(!backup_tokena) backup_tokena = 2; else backup_tokena *= 2;
   while DEE_UNLIKELY((new_backup_tokenv = (DeeTokenObject **)realloc_nz(
    backup_tokenv,backup_tokena*sizeof(DeeTokenObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
backup_error:
    new_backup_tokenv = backup_tokenv+backup_tokenc;
    while DEE_LIKELY(new_backup_tokenv != backup_tokenv) Dee_DECREF(*--new_backup_tokenv);
    free(backup_tokenv);
    return -1;
   }
   backup_tokenv = new_backup_tokenv;
  }
  Dee_INCREF(backup_tokenv[backup_tokenc++] = token_ob);
next_token:
  if DEE_UNLIKELY(!yield()) {
err_backup:
   new_backup_tokenv = backup_tokenv+backup_tokenc;
   while DEE_LIKELY(new_backup_tokenv != backup_tokenv) Dee_DECREF(*--new_backup_tokenv);
   free(backup_tokenv);
   return -1;
  }
 }
end:
 // Return all stored tokens in reverse order
 while DEE_LIKELY(backup_tokenc) {
  if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(
   DeeObject *)backup_tokenv[backup_tokenc-1]) != 0) goto err_backup;
  Dee_DECREF(backup_tokenv[--backup_tokenc]);
 }
 free(backup_tokenv);
 return result;
}


#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseBuiltinBoundExpression(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_PARSER_PARAMS) {
 DeeXAstObject *bound_ast,*ast_result;
 if DEE_UNLIKELY((bound_ast = DeeXAst_ParseUnary(DEE_PARSER_ARGS)) == NULL) return NULL;
 switch (bound_ast->ast_kind) {
  case DEE_XASTKIND_ATTR_GET:
   if DEE_UNLIKELY((ast_result = DeeXAst_NewAttrHas(operator_token,
    lexer,parser_flags,bound_ast->ast_operator.op_a,
    bound_ast->ast_operator.op_b)) == NULL) goto err_bound_ast;
   break;
  case DEE_XASTKIND_ATTR_GET_C:
   if DEE_UNLIKELY((ast_result = DeeXAst_NewAttrHasC(operator_token,
    lexer,parser_flags,bound_ast->ast_attr_has_c.ac_object,
    (DeeObject *)bound_ast->ast_attr_has_c.ac_name)) == NULL) goto err_bound_ast;
   break;
  case DEE_XASTKIND_VAR:
   if DEE_UNLIKELY((ast_result = DeeXAst_NewBuiltinBound(operator_token,
    bound_ast->ast_common.ast_token,lexer,parser_flags,
    bound_ast->ast_var.vs_var,scope)) == NULL) goto err_bound_ast;
   break;
  default:
   if DEE_UNLIKELY(DeeError_CompilerErrorf(
    DEE_WARNING_EXPECTED_VARIABLE_AFTER_BUILTIN_BOUND,
    (DeeObject *)lexer,(DeeObject *)bound_ast->ast_common.ast_token,
    "Expected variable in '__builtin_bound(...)', but got %r",bound_ast) != 0) {
err_bound_ast: Dee_DECREF(bound_ast); return NULL;
   }
   if DEE_UNLIKELY((ast_result = DeeXAst_NewConst(operator_token,Dee_False)) == NULL) return NULL;
   break;
 }
 Dee_DECREF(bound_ast);
 return ast_result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */


DEE_DECL_END

#ifndef __INTELLISENSE__
#include "xast_parse.attribute.inl"
#include "xast_parse.cast_suffix.inl"
#include "xast_parse.class.inl"
#include "xast_parse.del.inl"
#include "xast_parse.enum.inl"
#include "xast_parse.exists.inl"
#include "xast_parse.foreign_function_closure.inl"
#include "xast_parse.function.inl"
#include "xast_parse.import.inl"
#include "xast_parse.init_or_decl.inl"
#include "xast_parse.named_type_suffix.inl"
#include "xast_parse.offsetof.inl"
#include "xast_parse.operator.inl"
#include "xast_parse.range_init.inl"
#include "xast_parse.struct.inl"
#include "xast_parse.switch_block.inl"
#include "xast_parse.type.inl"
#include "xast_parse.types_compatible_p.inl"
#include "xast_parse.unary.inl"
#include "xast_parse.unary_with_suffix.inl"
#include "xast_parse.unnamed_function.inl"
#include "xast_parse.var_lookup.inl"
#endif


#endif /* !GUARD_DEEMON_XAST_PARSE_C */
