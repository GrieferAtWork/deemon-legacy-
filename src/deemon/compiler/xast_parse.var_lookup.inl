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

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupModifiers(
 DEE_A_INOUT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 (void)DEE_PARSER_ARGS;
again: switch (token.tk_id) {

  case KWD_module:
   if DEE_UNLIKELY((*vardecl_mode&(
     DEE_XAST_VARDECL_FLAG_STATIC
    |DEE_XAST_VARDECL_FLAG_STACK
    |DEE_XAST_VARDECL_FLAG_MODULE
    |DEE_XAST_VARDECL_FLAG_VOLATILE))!=0) {
    // Something's wrong...
    if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC)!=0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_INCOMPATIBLE_STATIC_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier 'module' is incompatible with 'static'") != 0) return -1;
    } else if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK)!=0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_INCOMPATIBLE_STACK_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier 'module' is incompatible with '__stack'") != 0) return -1;
    } else if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_MODULE) != 0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_ALREADY_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier 'module' was already used") != 0) return -1;
    } else {
     DEE_ASSERT((*vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE)!=0);
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_INCOMPATIBLE_VOLATILE_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier 'module' is incompatible with 'volatile'") != 0) return -1;
    }
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_MODULE;
y1_again:
   if DEE_UNLIKELY(!yield()) return -1;
   goto again;

  case KWD_forward:
   if DEE_UNLIKELY((*vardecl_mode&DEE_XAST_VARDECL_FLAG_FORWARD) != 0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_FORWARD_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier 'forward' was already used") != 0) return -1;
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_FORWARD;
   goto y1_again;

  DEE_PARSE_TOKENID_CASE_CONST
   if DEE_UNLIKELY((*vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_CONST_ALREADY_UNSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "The 'const' variable modifier was already used") != 0) return -1;
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_CONST;
   goto y1_again;

  DEE_PARSE_TOKENID_CASE_VOLATILE
   if DEE_UNLIKELY((*vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE)!=0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_VOLATILE_ALREADY_UNSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "The 'volatile' variable modifier was already used") != 0) return -1;
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_VOLATILE;
   goto y1_again;

  case KWD_static:
   if DEE_UNLIKELY((*vardecl_mode&(
     DEE_XAST_VARDECL_FLAG_MODULE
    |DEE_XAST_VARDECL_FLAG_STACK
    |DEE_XAST_VARDECL_FLAG_STATIC))!=0) {
    // Something's wrong...
    if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_MODULE)!=0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_INCOMPATIBLE_STATIC_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' is incompatible with 'module'") != 0) return -1;
    } else if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_STATIC_INCOMPATIBLE_STACK_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' is incompatible with 'static'") != 0) return -1;
    } else {
     DEE_ASSERT((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0);
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_STATIC_ALREADY_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' was already used") != 0) return -1;
    }
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_STATIC;
   goto y1_again;

#if DEE_CONFIG_LANGUAGE_HAVE_STACK_VARIABLES
  DEE_PARSE_TOKENID_CASE_STACK
   if DEE_UNLIKELY((*vardecl_mode&(
     DEE_XAST_VARDECL_FLAG_MODULE
    |DEE_XAST_VARDECL_FLAG_STATIC
    |DEE_XAST_VARDECL_FLAG_STACK))!=0) {
    // something's wrong...
    if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_MODULE)!=0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_MODULE_INCOMPATIBLE_STACK_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' is incompatible with 'module'") != 0) return -1;
    } else if ((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_STATIC_INCOMPATIBLE_STACK_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' is incompatible with 'static'") != 0) return -1;
    } else {
     DEE_ASSERT((*vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0);
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_STACK_ALREADY_USED,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Var reference modifier '__stack' was already used") != 0) return -1;
    }
   } else *vardecl_mode |= DEE_XAST_VARDECL_FLAG_STACK;
   goto y1_again;
#endif

  case KWD_local:
   if DEE_UNLIKELY(DEE_XAST_VARDECL_MODE(*vardecl_mode) != DEE_XAST_VARDECL_MODE_DEFAULT) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_LOCAL_ALREADY_UNSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "The 'local' variable modifier was already used / is overriding another scope-related modifier") != 0) return -1;
   }
   *vardecl_mode = DEE_XAST_VARDECL_FLAGS(*vardecl_mode)|DEE_XAST_VARDECL_MODE_LOCAL;
   goto y1_again;

  DEE_PARSE_TOKENID_CASE_GLOBAL
   if DEE_UNLIKELY(DEE_XAST_VARDECL_MODE(*vardecl_mode) != DEE_XAST_VARDECL_MODE_DEFAULT) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_GLOBAL_ALREADY_UNSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "The 'global' variable modifier was already used / is overriding another scope-related modifier") != 0) return -1;
   }
   *vardecl_mode = DEE_XAST_VARDECL_FLAGS(*vardecl_mode)|DEE_XAST_VARDECL_MODE_GLOBAL;
   goto y1_again;

  default: if (attr) {
   Dee_uint64_t old_pos = lexer->l_token_pos;
   if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
   if (lexer->l_token_pos != old_pos) goto again;
  } break;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupNTh(
 DEE_A_OUT Dee_size_t *nth, DEE_PARSER_PARAMS) {
 DeeTokenObject *ast_token;
 DeeObject *const_nth_id;
 Dee_ssize_t nth_id; int temp;
 DEE_ASSERT(token.tk_id == KWD___nth);
 if DEE_UNLIKELY(!yield()) return -1;
 if DEE_UNLIKELY(token.tk_id != '(') {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__nth'") != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 if DEE_UNLIKELY((const_nth_id = DeeXAst_ParseConst(&ast_token,DEE_PARSER_ARGS)) == NULL) return -1;
 temp = DeeObject_Cast(Dee_ssize_t,const_nth_id,&nth_id);
 Dee_DECREF(const_nth_id);
 if DEE_UNLIKELY(temp != 0) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)ast_token,"Failed to cast constant in '__nth(...)': %K",
   _DeeError_HandleAndReturnType()) != 0) {
err_ast_token: Dee_DECREF(ast_token); return -1;
  }
  nth_id = 1;
 }
 if DEE_UNLIKELY(nth_id < 0) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_POSITIVE_EXPRESSION_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)ast_token,
   "Expected positive constant in '__nth(...)', but got %Id",nth_id) != 0) goto err_ast_token;
  nth_id = -nth_id;
 }
 if DEE_UNLIKELY(!nth_id) {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_NONZERO_EXPRESSION_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)ast_token,"Expected non-zero constant in '__nth(...)'"
   ) != 0) goto err_ast_token;
 } else --nth_id;
 Dee_DECREF(ast_token);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && DEE_UNLIKELY(!yield())) return -1;
#endif
 if DEE_UNLIKELY(token.tk_id != ')') {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after '__nth(%Id'",nth_id) != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 *nth = (Dee_size_t)nth_id;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupScope(
 DEE_A_OUT Dee_size_t *scope_uprefs, DEE_PARSER_PARAMS) {
 DeeTokenObject *ast_token;
 DeeObject *const_scope_offset;
 Dee_ssize_t scope_offset; int temp;
 DEE_ASSERT(token.tk_id == KWD___scope);
 if DEE_UNLIKELY(!yield()) return -1;
 if DEE_UNLIKELY(token.tk_id != '(') {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_SCOPE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after '__scope'") != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 if DEE_UNLIKELY((const_scope_offset = DeeXAst_ParseConst(&ast_token,DEE_PARSER_ARGS)) == NULL) return -1;
 temp = DeeObject_Cast(Dee_ssize_t,const_scope_offset,&scope_offset);
 Dee_DECREF(const_scope_offset);
 if DEE_UNLIKELY(temp != 0) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_NTH,
   (DeeObject *)lexer,(DeeObject *)ast_token,"Failed to cast constant in '__scope(...)': %K",
   _DeeError_HandleAndReturnType()) != 0) {
err_ast_token: Dee_DECREF(ast_token); return -1;
  }
  scope_offset = 0;
 }
 if DEE_UNLIKELY(scope_offset > 0) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_NEGATIVE_EXPRESSION_AFTER_SCOPE,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected negative constant in '__scope(...)', but got %Id",
   scope_offset) != 0) goto err_ast_token;
  scope_offset = -scope_offset;
 }
 Dee_DECREF(ast_token);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && DEE_UNLIKELY(!yield())) return -1;
#endif
 if DEE_UNLIKELY(token.tk_id != ')') {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_SCOPE,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ')' after '__scope(%Id'",scope_offset) != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 *scope_uprefs = (Dee_size_t)-scope_offset;
 return 0;
}



DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseVariableLookup(
 DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result; TPPTokenID name;
 DeeObject *found_entry,*deprecated_reason;
 DeeTokenObject *ast_token;
 DeeScopeObject *scope_iter;
again:
 switch (token.tk_id) {

  DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   goto again;

  case KWD_function:
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return NULL;
   if (attr) {
    if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_ast_token: Dee_DECREF(ast_token); return NULL;
    }
    ast_result = DeeXAst_ParseFunctionWithAttribute(ast_token,vardecl_mode,NULL,attr,DEE_PARSER_ARGS);
   } else ast_result = DeeXAst_ParseFunction(ast_token,vardecl_mode,NULL,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;

#if DEE_CONFIG_LANGUAGE_HAVE_ALIAS
  case KWD_alias: {
   TPPTokenID alias_name; int alias_equals;
   DeeXAstObject *alias_value,*old_alias_value;
   if DEE_UNLIKELY(!yield()) return NULL;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_ALIAS,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected keyword after 'alias'") != 0) return NULL;
    alias_name = 0;
   } else {
    alias_name = token.tk_id;
    if DEE_UNLIKELY(!yield()) return NULL;
   }
   if (token.tk_id == '=') {
    // Alias initializer
    if DEE_UNLIKELY(!yield()) return NULL;
    if DEE_UNLIKELY((alias_value = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return NULL;
   } else alias_value = NULL;
   if (!alias_name) {
    if (alias_value) return alias_value;
    // todo: The token here in incorrect (should be the 'alias' token)
    return DeeXAst_NewConst(token_ob,Dee_None);
   }
   switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {
    case DEE_XAST_VARDECL_MODE_GLOBAL:
     old_alias_value = (DeeXAstObject *)_DeeScope_GetName((
      DeeScopeObject *)DeeScope_Global((DeeObject *)scope),alias_name);
     break;
    case DEE_XAST_VARDECL_MODE_LOCAL:
     old_alias_value = (DeeXAstObject *)_DeeScope_GetName(scope,alias_name);
     break;
    default: scope_iter = scope; while (1) {
     if ((old_alias_value = (DeeXAstObject *)_DeeScope_GetName(scope_iter,alias_name)) != NULL) break;
     if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
     else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
     else break;
    } break;
   }
   if (old_alias_value) {
    if DEE_UNLIKELY(!DeeXAst_Check(old_alias_value)) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_ALIAS_CONSTANT,
      (DeeObject *)lexer,(DeeObject *)token_ob, // todo: incorrect token
      "Expected alias constant after 'alias', but got instance of %s",
      DeeType_NAME(Dee_TYPE(old_alias_value))) != 0) goto err_alias_value;
     Dee_XDECREF(alias_value);
     // todo: incorrect token
     if (DeeScope_Check(old_alias_value)) return DeeXAst_NewModule(token_ob,(DeeScopeObject *)old_alias_value);
     if (DeeLocalVar_Check(old_alias_value)) return DeeXAst_NewVar(token_ob,(DeeLocalVarObject *)old_alias_value,scope);
     return DeeXAst_NewConst(token_ob,(DeeObject *)old_alias_value);
    } else {
     if (alias_value) {
      if DEE_UNLIKELY((alias_equals = DeeObject_CompareEq((DeeObject *)old_alias_value,(
       DeeObject *)alias_value)) < 0) { DeeError_Handled(); alias_equals = 0; }
      if DEE_UNLIKELY(!alias_equals) {
       if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_ALIAS_CONSTANT_ALREADY_DECLARED,
        (DeeObject *)lexer,(DeeObject *)token_ob, // todo: incorrect token
        "Alias was already defined with a different value") != 0) goto err_alias_value;
      }
      Dee_DECREF(alias_value);
      // todo: incorrect token
      return DeeXAst_NewConst(token_ob,Dee_None);
     }
     return DeeXAst_CopyWithScope(old_alias_value,scope,lexer);
    }
   } else {
    if (alias_value) {
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
      if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CANT_DECLARE_ALIAS_CONSTANT_WITHOUT_DECL_RIGHTS,
       (DeeObject *)lexer,(DeeObject *)token_ob, // todo: incorrect token
       "Can't declare alias without declaration rights: 'alias %s = %r'",
       TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),alias_name),alias_value) != 0) goto err_alias_value;
     } else {
      if DEE_UNLIKELY(_DeeScope_AddDeprecatedName(scope,alias_name,(
       alias_value->ast_kind == DEE_XASTKIND_CONST ? alias_value->ast_const.c_const
       : (DeeObject *)alias_value),attr ? (DeeObject *)attr->a_depr : NULL) != 0) {
err_alias_value: Dee_XDECREF(alias_value); return NULL;
      }
     }
     Dee_DECREF(alias_value);
    }
    // todo: incorrect token
    return DeeXAst_NewConst(token_ob,Dee_None);
   }
  }
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
  case KWD_final: {
   if DEE_UNLIKELY(!yield()) return NULL;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   switch (token.tk_id) {
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
    case KWD_class: goto parse_final_class;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
    case KWD_struct: goto parse_final_struct;
    case KWD_union: goto parse_final_union;
#endif
    default:
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_AFTER_FINAL,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Unexpected token after 'final'") != 0) return NULL;
     goto default_case;
   }
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case KWD_class: {
   Dee_uint32_t flags;
   flags = DEE_XAST_PARSECLASS_FLAGS_NONE;
   if (0) {parse_final_class: flags = DEE_XAST_PARSECLASS_FLAGS_FINAL; }
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return NULL;
   if (attr) {
    if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
    ast_result = DeeXAst_ParseClassWithAttribute(ast_token,vardecl_mode,flags,attr,DEE_PARSER_ARGS);
   } else ast_result = DeeXAst_ParseClass(ast_token,vardecl_mode,flags,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
  {
   Dee_uint32_t flags;
   if (0) { case KWD_struct:    flags = DEE_XAST_PARSESTRUCT_FLAG_NONE; }
   if (0) { case KWD_union:     flags = DEE_XAST_PARSESTRUCT_FLAG_UNION; }
   if (0) { parse_final_struct: flags = DEE_XAST_PARSESTRUCT_FLAG_FINAL; }
   if (0) { parse_final_union:  flags = DEE_XAST_PARSESTRUCT_FLAG_UNION|DEE_XAST_PARSESTRUCT_FLAG_FINAL; }
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return NULL;
   if (attr) {
    if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
    ast_result = DeeXAst_ParseStructWithAttribute(ast_token,vardecl_mode,flags,attr,DEE_PARSER_ARGS);
   } else ast_result = DeeXAst_ParseStruct(ast_token,vardecl_mode,flags,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */

#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
  case KWD_enum:
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return NULL;
   if (attr) {
    if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
    ast_result = DeeXAst_ParseEnumWithAttribute(ast_token,vardecl_mode,attr,DEE_PARSER_ARGS);
   } else ast_result = DeeXAst_ParseEnum(ast_token,vardecl_mode,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */

  case KWD___nth: {
   Dee_size_t nth_id,nth_id_iter;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupNTh(&nth_id,DEE_PARSER_ARGS) != 0) return NULL;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   if DEE_UNLIKELY(!TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != TPP_TOK_DOTS) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_KEYWORD_AFTER_NTH,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after '__nth(-%Iu)'",nth_id) != 0) return NULL;
   } else {
    nth_id_iter = nth_id;
    while (1) {
     if ((found_entry = _DeeScope_GetNameEx(scope,token.tk_id,&deprecated_reason)) != NULL) {
      if (!nth_id_iter) goto return_found_entry; --nth_id_iter;
     }
     if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
     else if (scope->sc_prev) scope = scope->sc_prev;
     else break;
    }
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_NTH_VARIABLE,(DeeObject *)lexer,(DeeObject *)token_ob,
     "Unknown nth variable (found %Id instances, but %Id%s instance requested)",nth_id-nth_id_iter,nth_id+1,
     nth_id == 0 ? "st" : nth_id == 1 ? "nd" : nth_id == 2 ? "rd" : "th") != 0) return NULL;
   }
   goto yield_and_ret_none;
  } break;

  case KWD___scope: {
   Dee_size_t scope_uprefs,scope_uprefs_iter;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupScope(&scope_uprefs,DEE_PARSER_ARGS) != 0) return NULL;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   scope_uprefs_iter = scope_uprefs;
   while (scope_uprefs_iter--) {
    if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
    else if (scope->sc_prev) scope = scope->sc_prev;
    else break;
   }
   name = token.tk_id;
   if DEE_UNLIKELY(!TPPTokenID_IS_KEYWORD(name) && name != TPP_TOK_DOTS) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_KEYWORD_AFTER_SCOPE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after '__scope(-%Iu)'",scope_uprefs) != 0) return NULL;
   } else {
    if ((found_entry = _DeeScope_GetNameEx(scope,name,
     &deprecated_reason)) != NULL) goto return_found_entry;
    if DEE_LIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED) != 0 && scope_uprefs == 0) goto create_local;
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_SCOPE_VARIABLE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Unknown variable in scope at -%Iu",scope_uprefs) != 0) return NULL;
   }
   goto yield_and_ret_none;
  } break;

  DEE_ATTRIBUTEDECL_TOKENS if (attr) {
   Dee_uint64_t old_pos = lexer->l_token_pos;
   if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
   if (lexer->l_token_pos != old_pos) goto again;
   DEE_ATTRIBUTE_FALLTHROUGH
  }

#ifdef DEE_SAST_STATEMENT_ONLY_TOKENS
  if (0) {
   // Warn about statement tokens in expression
  DEE_SAST_STATEMENT_ONLY_TOKENS
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_STATEMENT_TOKEN_IN_EXPRESSION,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Unexpected statement token in expression") != 0) return NULL;
  }
#endif

  default:
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
default_case:
#endif /* ... */
   if DEE_LIKELY(TPPTokenID_IS_KEYWORD(token.tk_id)) {
  case TPP_TOK_DOTS: // Parse a variable name
    name = token.tk_id;
    switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {

     case DEE_XAST_VARDECL_MODE_LOCAL: {
/*search_local:*/
      if ((found_entry = _DeeScope_GetNameEx(scope,name,&deprecated_reason)) != NULL)
       goto return_found_entry; // Found it!
      if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED) != 0) {
create_local: // Create a new local variable
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
       if DEE_UNLIKELY(DeeXAst_CheckReservedToken(lexer,token_ob) != 0) return NULL;
#endif
       if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_MODULE) != 0) {
        if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_CREATE_MODULE_IN_EXPRESSION,
         (DeeObject *)lexer,(DeeObject *)token_ob,
         "Can't create module in expression") != 0) return NULL;
        ast_result = DeeXAst_NewConst(token_ob,Dee_None);
       } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST) != 0) {
        Dee_INCREF(ast_token = token_ob);
        if DEE_UNLIKELY(!yield()) goto err_ast_token;
        if DEE_UNLIKELY(token.tk_id != '=') {
         if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_ASSIGN_AFTER_UNKNOWN_CONST,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Expected '=' after unknown 'const %k'",ast_token) != 0) goto err_ast_token;
        } else assign_constant: if DEE_UNLIKELY(!yield()) goto err_ast_token;
        if DEE_UNLIKELY((ast_result = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
         parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_ast_token;
        if DEE_UNLIKELY(ast_result->ast_kind != DEE_XASTKIND_CONST) {
         if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CONST,
          (DeeObject *)lexer,(DeeObject *)ast_result->ast_common.ast_token,
          "Expected constant after 'const %k =', but got %r",
          ast_token,ast_result) != 0) { Dee_DECREF(ast_result); goto err_ast_token; }
         Dee_DECREF(ast_result);
         ast_result = DeeXAst_NewConst(ast_token,Dee_None);
        } else {
         TPPTokenID constant_name;
         DEE_LVERBOSE2R("%s(%d) : Defining constant : %k = %r\n",
                        DeeToken_FILE(ast_token),DeeToken_LINE(ast_token)+1,
                        ast_token,ast_result->ast_const.c_const);
         constant_name = ast_token->tk_token.tk_id;
         if (found_entry) { // Warn if redefining a constant with a different value
          if DEE_LIKELY(Dee_TYPE(found_entry) == Dee_TYPE(ast_result->ast_const.c_const)) {
           int temp = DeeObject_DeepEquals(found_entry,ast_result->ast_const.c_const);
           if DEE_UNLIKELY(temp < 0) { DeeError_Handled(); goto constant_already_defined; }
           if DEE_UNLIKELY(!temp) goto constant_already_defined;
           // Same constant value (though still re-define the result ast to save memory)
           Dee_DECREF(ast_result->ast_const.c_const);
           Dee_INCREF(ast_result->ast_const.c_const = found_entry);
          } else {
constant_already_defined:
           if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CONST_NAME_ALREADY_USED,
            (DeeObject *)lexer,(DeeObject *)ast_token,
            "Constant was already defined with a different value:\n"
            "\tOld value: %>r\n\tNew value: %>r",
            found_entry,ast_result->ast_const.c_const) != 0) goto err_r_ast_token;
          }
         } else if DEE_UNLIKELY(_DeeScope_AddDeprecatedName(scope,ast_token->tk_token.tk_id,
          ast_result->ast_const.c_const,(DeeObject *)(attr ? attr->a_depr : NULL)) != 0) {
err_r_ast_token: Dee_DECREF(ast_result); goto err_ast_token;
         }
        }
        Dee_DECREF(ast_token);
        return ast_result;
       } else {
        if DEE_UNLIKELY((found_entry = DeeLocalVar_New(name,(DeeObject *)scope)) == NULL) return NULL;
        // Set the static flag when creating variables in a static context
        if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0) ((DeeLocalVarObject *)found_entry)->lv_flags = DEE_LOCALVAR_KIND_STACK;
        else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0) ((DeeLocalVarObject *)found_entry)->lv_flags = DEE_LOCALVAR_KIND_STATIC;
        if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE)!=0) ((DeeLocalVarObject *)found_entry)->lv_flags |= DEE_LOCALVAR_FLAG_VOLATILE;
        if DEE_UNLIKELY(_DeeScope_AddDeprecatedName(scope,name,
         found_entry,attr ? (DeeObject *)attr->a_depr : NULL) != 0) {
/*err_found_entry:*/ Dee_DECREF(found_entry); return NULL;
        }
        ast_result = DeeXAst_NewLocalVar(token_ob,(DeeLocalVarObject *)found_entry,scope);
        if (attr && ast_result) {
         // Parse & apply used/unused attributes
         Dee_DECREF(found_entry);
         DEE_ASSERT(DeeObject_IS_ALIVE(found_entry)); // Still exists in 'ast_result'
         if DEE_UNLIKELY(!yield()) goto err_r;
         if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_r;
         DeeLocalVar_ApplyAttributes(((DeeLocalVarObject *)found_entry),attr);
         return ast_result;
        }
       }
       Dee_DECREF(found_entry);
       goto yield_and_return_result;
      }
      // Unknown local variable
      if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNKNOWN_LOCAL_VARIABLE,
       (DeeObject *)lexer,(DeeObject *)token_ob,"Unknown local variable") != 0) return NULL;
      goto yield_and_ret_none;
     } break;

     case DEE_XAST_VARDECL_MODE_GLOBAL: {
/*search_global:*/
      scope_iter = (DeeScopeObject *)DeeScope_Global((DeeObject *)scope);
      if ((found_entry = _DeeScope_GetNameEx(scope_iter,name,&deprecated_reason)) != NULL)
       goto return_found_entry; // Found it!
      if DEE_LIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED) != 0 && scope_iter == scope) goto create_local;
      // Unknown global variable
      if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNKNOWN_GLOBAL_VARIABLE,
       (DeeObject *)lexer,(DeeObject *)token_ob,"Unknown global variable") != 0) return NULL;
      goto yield_and_ret_none;
     } break;

     default: break;
    }
    scope_iter = scope;
    while (1) {
     if ((found_entry = _DeeScope_GetNameEx(scope_iter,name,&deprecated_reason)) != NULL)
      break; // Found it!
     if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
     else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
     else {
      // Variable not found
      if DEE_LIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED) != 0) goto create_local;
      if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNKNOWN_VARIABLE,
       (DeeObject *)lexer,(DeeObject *)token_ob,"Unknown variable") != 0) return NULL;
yield_and_ret_none:
      // Fallback: Return constant 'none'
      if DEE_UNLIKELY((ast_result = DeeXAst_NewConst(token_ob,Dee_None)) == NULL) return NULL;
yield_and_return_result:
      if DEE_UNLIKELY(!yield()) {err_r: Dee_DECREF(ast_result); return NULL; }
      return ast_result;
     }
    }
return_found_entry:
    // Warn about a possible deprecation tag
    if DEE_UNLIKELY(deprecated_reason && DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,(
     DeeObject *)lexer,(DeeObject *)token_ob,"Deprecated : %r",deprecated_reason) != 0) return NULL;
    if (DeeScope_Check(found_entry)) {
     // Warn about referencing a module with the static flag set
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STATIC,(DeeObject *)lexer,(DeeObject *)token_ob,"module referenced with 'static'") != 0) return NULL;
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STACK,(DeeObject *)lexer,(DeeObject *)token_ob,"module referenced with '__stack'") != 0) return NULL;
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_VOLATILE,(DeeObject *)lexer,(DeeObject *)token_ob,"module referenced with 'volatile'") != 0) return NULL;
     ast_result = DeeXAst_NewModule(token_ob,(DeeScopeObject *)found_entry);
    } else if (DeeXAst_Check(found_entry)) {
     // Warn about referencing an alias with the static flag set
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STATIC,(DeeObject *)lexer,(DeeObject *)token_ob,"alias referenced with 'static'") != 0) return NULL;
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STACK,(DeeObject *)lexer,(DeeObject *)token_ob,"alias referenced with '__stack'") != 0) return NULL;
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE) != 0 && DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_VOLATILE,(DeeObject *)lexer,(DeeObject *)token_ob,"alias referenced with 'volatile'") != 0) return NULL;
     ast_result = DeeXAst_CopyWithScope((DeeXAstObject *)found_entry,scope,lexer);
    } else {
     if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_MODULE)!=0 && DeeError_CompilerError(
      DEE_WARNING_VARREF_VARIABLE_NOT_MODULE,(DeeObject *)lexer,(DeeObject *)token_ob,
      "Variable reference with 'module' is not a module") != 0) return NULL;
     if (DeeLocalVar_Check(found_entry)) {
      // Warn about initialized variables in forward-vardecl_mode
      if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST) != 0 &&
          DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_CONST,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Variable reference with 'const' is not known at compile-time") != 0) return NULL;
      if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_FORWARD) != 0 && DeeLocalVar_IS_INIT(found_entry) &&
          DeeError_CompilerError(DEE_WARNING_FORWARD_VARIABLE_ALREADY_INITIALIZED,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Variable reference with 'forward' is already initialized") != 0) return NULL;
      // Warn about referencing a non-static with the static flag set
      if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STATIC) != 0 &&
          (DeeLocalVar_KIND(found_entry) != DEE_LOCALVAR_KIND_STATIC) &&
          DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STATIC,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Variable referenced with 'static' is not static") != 0) return NULL;
      if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE) != 0 &&
          ((DeeLocalVar_FLAGS(found_entry)&DEE_LOCALVAR_FLAG_VOLATILE) == 0) &&
          DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_VOLATILE,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Variable referenced with 'volatile' is not volatile") != 0) return NULL;
      if DEE_UNLIKELY((vardecl_mode&DEE_XAST_VARDECL_FLAG_STACK) != 0 &&
          (DeeLocalVar_KIND(found_entry) != DEE_LOCALVAR_KIND_STACK) &&
          DeeError_CompilerError(DEE_WARNING_VARREF_VARIABLE_NOT_STACK,
          (DeeObject *)lexer,(DeeObject *)token_ob,
          "Variable referenced with '__stack' is not a stack variable") != 0) return NULL;
      ast_result = DeeXAst_NewVar(token_ob,(DeeLocalVarObject *)found_entry,scope);
     } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0) {
      Dee_INCREF(ast_token = token_ob);
      if DEE_UNLIKELY(!yield()) goto err_ast_token;
      if (token.tk_id == '=') goto assign_constant;
      ast_result = DeeXAst_NewConst(ast_token,found_entry);
      Dee_DECREF(ast_token);
      return ast_result;
     } else ast_result = DeeXAst_NewConst(token_ob,found_entry);
    }
    if DEE_UNLIKELY(!ast_result) return NULL;
    goto yield_and_return_result;
   } else {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_TOKEN_IN_EXPRESSION,(
     DeeObject *)lexer,(DeeObject *)token_ob,"Expected variable name") != 0) return NULL;
    goto yield_and_ret_none;
   }
   break;
 }
}

DEE_DECL_END
