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

DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseNoReturn(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS) {
 int new_noreturn;
 DeeXAstObject *constant_ast; DeeObject *const_copy;
 if ((constant_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
 if (constant_ast->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_NORETURN,
   (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
   "Expected constant expression after '__attribute__((noreturn(...))), but got %r'",constant_ast) != 0) {
err_ast: Dee_DECREF(constant_ast); return -1;
  }
 } else {
  if ((const_copy = DeeObject_DeepCopy(constant_ast->ast_const.c_const)) == NULL) goto err_cast;
  new_noreturn = DeeObject_Bool(const_copy);
  Dee_DECREF(const_copy);
  if (new_noreturn < 0) {
err_cast:
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BOOLEAN_EXPRESSION_AFTER_NORETURN,
    (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
    "Failed to get boolean value of constant %q in __attribute__((noreturn(...))): %R",
    DeeType_NAME(Dee_TYPE(constant_ast->ast_const.c_const)),
    _DeeError_HandleAndReturnType()) != 0) goto err_ast;
   goto end;
  }
  if (new_noreturn) self->a_fun_flags |= DeeFunctionFlags_NORETURN;
  else self->a_fun_flags &= ~DeeFunctionFlags_NORETURN;
 }
end: Dee_DECREF(constant_ast);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseNoExcept(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS) {
 int new_nothrow;
 DeeXAstObject *constant_ast; DeeObject *const_copy;
 if DEE_UNLIKELY((constant_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
 if DEE_UNLIKELY(constant_ast->ast_kind != DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_NOTHROW,
   (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
   "Expected constant expression after '__attribute__((nothrow(...))), but got %r'",constant_ast) != 0) {
err_ast: Dee_DECREF(constant_ast); return -1;
  }
 } else {
  if DEE_UNLIKELY((const_copy = DeeObject_DeepCopy(constant_ast->ast_const.c_const)) == NULL) goto err_cast;
  new_nothrow = DeeObject_Bool(const_copy);
  Dee_DECREF(const_copy);
  if DEE_UNLIKELY(new_nothrow < 0) {
err_cast:
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BOOLEAN_EXPRESSION_AFTER_NOTHROW,
    (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
    "Failed to get boolean value of constant %q in __attribute__((nothrow(...))): %R",
    DeeType_NAME(Dee_TYPE(constant_ast->ast_const.c_const)),
    _DeeError_HandleAndReturnType()) != 0) goto err_ast;
   goto end;
  }
  if (new_nothrow) self->a_fun_flags |= DeeFunctionFlags_NOEXCEPT;
  else self->a_fun_flags &= ~DeeFunctionFlags_NOEXCEPT;
 }
end: Dee_DECREF(constant_ast);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseAlignas(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS) {
 Dee_size_t new_align;
 DeeXAstObject *constant_ast; DeeObject *const_copy;
 if DEE_UNLIKELY((constant_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
 if DEE_UNLIKELY(constant_ast->ast_kind != DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ALIGNAS,
   (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
   "Expected constant expression in '__attribute__((aligned(...)))', but got %r",constant_ast) != 0) {
err_ast: Dee_DECREF(constant_ast); return -1;
  }
 } else {
  if DEE_UNLIKELY((const_copy = DeeObject_DeepCopy(constant_ast->ast_const.c_const)) == NULL) goto err_cast;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,const_copy,&new_align) != 0) {
   Dee_DECREF(const_copy);
err_cast:
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_INTEGRAL_EXPRESSION_AFTER_ALIGNAS,
    (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
    "Failed to get integral value of constant %q in __attribute__((alignas(...))): %R",
    DeeType_NAME(Dee_TYPE(constant_ast->ast_const.c_const)),
    _DeeError_HandleAndReturnType()) != 0) goto err_ast;
   goto end;
  }
  Dee_DECREF(const_copy);
  if DEE_UNLIKELY(new_align == 0) {
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_INVALID_ALIGNMENT_IN_ALIGNAS,
    (DeeObject *)lexer,(DeeObject *)constant_ast->ast_common.ast_token,
    "Invalid alignment in __attribute__((alignas(...))): 0") == -1) goto err_ast;
   goto end;
  }
  self->a_alignment = new_align;
 }
end: Dee_DECREF(constant_ast);
 return 0;
}


DEE_STATIC_INLINE(TPPTokenID) _TPPLexer_GetKeywordWithoutUnderscores(
 TPPLexerObject *self, TPPTokenID tokid) {
 char const *str_begin,*str_end;
 str_begin = TPPLexer_TokenIDStr(self,tokid);
 while (*str_begin == '_') ++str_begin;
 str_end = str_begin+strlen(str_begin);
 while (str_end[-1] == '_') if DEE_UNLIKELY(str_end-- == str_begin) return 0;
 return TPPKeywordList_GetL(&self->l_keywords,str_begin,str_end,0);
}
DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseContent(
 DEE_A_INOUT struct DeeAttributeDecl *self,
 DEE_A_IN TPPTokenID end_token, DEE_PARSER_PARAMS) {
 switch (_TPPLexer_GetKeywordWithoutUnderscores(
  DeeLexer_LEXER(lexer),token.tk_id)) {

  case KWD_used: {
   if DEE_UNLIKELY((self->a_flags&DEE_ATTRIBUTE_DECL_FLAG_USED)!=0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_USED_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__attribute__((used)) was already used") != 0) return -1;
   } else if DEE_UNLIKELY((self->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UNUSED)!=0) {
warn_used_with_unused:
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_USED_WITH_UNUSED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__attribute__((used)) and __attribute__((unused)) cannot be used together") != 0) return -1;
   } else self->a_flags |= DEE_ATTRIBUTE_DECL_FLAG_USED;
  } goto y1;

  case KWD_unused: {
   if DEE_UNLIKELY((self->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UNUSED)!=0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_UNUSED_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__attribute__((unused)) was already used") != 0) return -1;
   } else if DEE_UNLIKELY((self->a_flags&DEE_ATTRIBUTE_DECL_FLAG_USED)!=0) {
    goto warn_used_with_unused;
   } else self->a_flags |= DEE_ATTRIBUTE_DECL_FLAG_UNUSED;
  } goto y1;

  case KWD_copyable: {
   if DEE_UNLIKELY((self->a_flags&DEE_ATTRIBUTE_DECL_FLAG_COPYABLE) != 0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_COPYABLE_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__attribute__((copyable)) was already used") != 0) return -1;
   } else self->a_flags |= DEE_ATTRIBUTE_DECL_FLAG_COPYABLE;
  } goto y1;

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
  case KWD_add_result_reference: {
   if DEE_UNLIKELY((self->a_fun_flags&DeeFunctionFlags_ADD_RESULT_REFERENCE) != 0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_ADD_RESULT_REFERENCE_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "__attribute__((add_result_reference)) was already used") != 0) return -1;
   } else self->a_fun_flags |= DeeFunctionFlags_ADD_RESULT_REFERENCE;
  } goto y1;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

  case KWD_name: {
   DeeXAstObject *temp;
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_NAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__((name'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY((temp = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
   if DEE_UNLIKELY(temp->ast_kind != DEE_XASTKIND_CONST) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_NAME,
     (DeeObject *)lexer,(DeeObject *)temp->ast_common.ast_token,
     "Expected constant expression in '__attribute__((name(...)))', but got %r",
     temp) != 0) goto err_name_temp;
   } else if DEE_LIKELY(DeeString_Check(temp->ast_const.c_const)) {
    Dee_XDECREF(self->a_name);
    Dee_INCREF(self->a_name = (DeeStringObject *)temp->ast_const.c_const);
   } else if DEE_LIKELY(DeeNone_Check(temp->ast_const.c_const)) {
    Dee_XDECREF(self->a_name);
    Dee_INCREF(self->a_name = (DeeStringObject *)Dee_EmptyString);
   } else {
    DeeStringObject *temp2;
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_NAME,
     (DeeObject *)lexer,(DeeObject *)temp->ast_common.ast_token,
     "Expected string in '__attribute__((name(...)))', but got instance of %q",
     DeeType_NAME(Dee_TYPE(temp->ast_const.c_const))) != 0) {
err_name_temp: Dee_DECREF(temp); return -1;
    }
    if DEE_UNLIKELY((temp2 = (DeeStringObject *)DeeObject_Str(
     temp->ast_const.c_const)) == NULL) DeeError_Handled(); else {
     Dee_XDECREF(self->a_name);
     Dee_INHERIT_REF(self->a_name,temp2);
    }
   }
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_NAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((name(%r'",temp) != 0) goto err_name_temp;
   } else if DEE_UNLIKELY(!yield()) goto err_name_temp;
   Dee_DECREF(temp);
  } break;

#define DEE_ATTRIBUTE_INSTANCE_BOUND_MODE_DEFAULT \
  DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC
  case KWD_instance_bound: {
   Dee_uint32_t new_flags;
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '(') {
    DeeXAstObject *instance_bound_mode_ast;
    DeeObject *instance_bound_mode;
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY((instance_bound_mode_ast = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
     parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
    if DEE_UNLIKELY(instance_bound_mode_ast->ast_kind != DEE_XASTKIND_CONST) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_INSTANCE_BOUND,
      (DeeObject *)lexer,(DeeObject *)instance_bound_mode_ast->ast_common.ast_token,
      "Expected constant after '__attribute__((instance_bound(...', but got %r",
      instance_bound_mode_ast) != 0) {
err_instance_bound_mode_ast: Dee_DECREF(instance_bound_mode_ast); return -1;
     }
     instance_bound_mode = Dee_None;
    } else {
     instance_bound_mode = instance_bound_mode_ast->ast_const.c_const;
    }
    if DEE_LIKELY(DeeString_Check(instance_bound_mode)) {
     // Select the specified binding mode
          if (DeeString_EQUALS(instance_bound_mode,"move")) new_flags = DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM;
     else if (DeeString_EQUALS(instance_bound_mode,"copy")) new_flags = DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC;
     else {
      if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNKNOWN_INSTANCE_BOUND_MODE,
       (DeeObject *)lexer,(DeeObject *)instance_bound_mode_ast->ast_common.ast_token,
       "Unknown instance-bound mode in '__attribute__((instance_bound(%r)))'",
       instance_bound_mode) != 0) goto err_instance_bound_mode_ast;
      new_flags = DEE_ATTRIBUTE_INSTANCE_BOUND_MODE_DEFAULT;
     }
    } else if DEE_LIKELY(DeeNone_Check(instance_bound_mode)) {
     // Delete any previous [[instance_bound]] attributes
     self->a_flags &= ~(DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC|DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM);
     new_flags = 0;
    } else {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRING_EXPRESSION_AFTER_ATTRIBUTE_INSTANCE_BOUND,
      (DeeObject *)lexer,(DeeObject *)instance_bound_mode_ast->ast_common.ast_token,
      "Expected string after '__attribute__((instance_bound(...', but got instance of %q",
      DeeType_NAME(Dee_TYPE(instance_bound_mode))) != 0) goto err_instance_bound_mode_ast;
     new_flags = DEE_ATTRIBUTE_INSTANCE_BOUND_MODE_DEFAULT;
    }
    Dee_DECREF(instance_bound_mode_ast);
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_INSTANCE_BOUND,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '__attribute__((instance_bound(...'") != 0) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
   } else {
    // Default binding mode if no argument is given
    new_flags = DEE_ATTRIBUTE_INSTANCE_BOUND_MODE_DEFAULT;
   }
   if DEE_UNLIKELY((self->a_flags&new_flags)!=0) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_ATTRIBUTE_INSTANCE_BOUND_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "'__attribute__((instance_bound))' was already used") != 0) return -1;
   }
   // Add the new flags
   self->a_flags |= new_flags;
  } break;

  case KWD_deprecated: {
   DeeXAstObject *temp;
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id != '(') {
    DeeString_NEW_STATIC(_depr_default,"deprecated object");
    Dee_XDECREF(self->a_depr);
    Dee_INCREF(self->a_depr = (DeeStringObject *)&_depr_default);
   } else {
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY((temp = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
     parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
    if DEE_UNLIKELY(temp->ast_kind != DEE_XASTKIND_CONST) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_DEPRECATED,
      (DeeObject *)lexer,(DeeObject *)temp->ast_common.ast_token,
      "Expected constant expression in '__attribute__((deprecated(...)))', but got %r",
      temp) != 0) goto err_depr_temp;
    } else if DEE_LIKELY(DeeString_Check(temp->ast_const.c_const)) {
     Dee_XDECREF(self->a_depr);
     Dee_INCREF(self->a_depr = (DeeStringObject *)temp->ast_const.c_const);
    } else if DEE_LIKELY(DeeNone_Check(temp->ast_const.c_const)) {
     Dee_XCLEAR(self->a_depr);
    } else {
     DeeStringObject *temp2;
     if DEE_UNLIKELY(DeeError_CompilerErrorf(
      DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_DEPRECATED,
      (DeeObject *)lexer,(DeeObject *)temp->ast_common.ast_token,
      "Expected string in '__attribute__((deprecated(...)))', but got instance of %q",
      DeeType_NAME(Dee_TYPE(temp->ast_const.c_const))) != 0) {
err_depr_temp: Dee_DECREF(temp); return -1;
     }
     if DEE_UNLIKELY((temp2 = (DeeStringObject *)DeeObject_Str(
      temp->ast_const.c_const)) == NULL) DeeError_Handled();
     else {
      Dee_XDECREF(self->a_depr);
      Dee_INHERIT_REF(self->a_depr,temp2);
     }
    }
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_DEPRECATED,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '__attribute__((deprecated(%r'",temp) != 0) goto err_depr_temp;
    } else if DEE_UNLIKELY(!yield()) goto err_depr_temp;
    Dee_DECREF(temp);
   }
  } break;

  case KWD_super: {
   DeeXAstObject *new_super;
   DeeTypeObject const *predicted_super_type;
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_SUPER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__((super'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY((new_super = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return -1;
   Dee_XDECREF(self->a_super);
   self->a_super = new_super; // Install the new super ast (inherit reference)
   if DEE_UNLIKELY(!DeeXAst_IsNoEffect(new_super)) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(
     DEE_WARNING_ATTRIBUTE_SUPER_EXPRESSION_HAS_SIDE_EFFECTS,
     (DeeObject *)lexer,(DeeObject *)new_super->ast_common.ast_token,
     "Argument of __attribute__((super(...))) has side-effects "
     "but may be evaluated more than once: %r",new_super) != 0) return -1;
   }
   predicted_super_type = DeeXAst_PredictType(new_super);
   if DEE_UNLIKELY(predicted_super_type && DEE_UNLIKELY(!DeeType_Check(predicted_super_type))) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_ATTRIBUTE_SUPER,
     (DeeObject *)lexer,(DeeObject *)new_super->ast_common.ast_token,
     "Expected 'type' as argument in __attribute__((super(...))), but got instance of %q",
     DeeType_NAME(predicted_super_type)) != 0) return -1;
   }
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_SUPER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((super(%r",
     new_super) != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
  } break;

  case KWD_uuid: {
   DeeXAstObject *temp;
   DeeStringObject *uuid_string;
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_UUID,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__((uuid'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY((temp = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
   if DEE_UNLIKELY(temp->ast_kind != DEE_XASTKIND_CONST) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_UUID,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected constant expression after '__attribute__((uuid(', but got %r",
     temp) != 0) goto err_uuid_temp;
    uuid_string = NULL;
   } else if DEE_LIKELY(DeeString_Check(temp->ast_const.c_const)) {
    Dee_INCREF(uuid_string = (DeeStringObject *)temp->ast_const.c_const);
   } else if DEE_LIKELY(DeeNone_Check(temp->ast_const.c_const)) {
    uuid_string = NULL;
   } else {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(
     DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_UUID,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected string after '__attribute__((uuid(', but got instance of %q",
     DeeType_NAME(Dee_TYPE(temp->ast_const.c_const))) != 0) {
err_uuid_temp: Dee_DECREF(temp); return -1;
    }
    if DEE_UNLIKELY((uuid_string = (DeeStringObject *)DeeObject_Str(
     temp->ast_const.c_const)) == NULL) DeeError_Handled();
   }
   Dee_DECREF(temp);
   if (uuid_string) {
    char const *invalid_character_pos;
    if DEE_UNLIKELY(!DeeUUID_FromString(&self->a_uuid,DeeString_STR(uuid_string),&invalid_character_pos)) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_UUID_STRING,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Invalid character '%#.1q' in uuid string %q at pos %Iu",
      invalid_character_pos,DeeString_STR(uuid_string),
      (Dee_size_t)(invalid_character_pos-DeeString_STR(uuid_string))
      ) != 0) {err_uuid_string: Dee_XDECREF(uuid_string); return -1; }
    } else self->a_flags |= DEE_ATTRIBUTE_DECL_FLAG_UUID;
   }
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(
     DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_UUID,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((uuid(%q'",
     uuid_string ? DeeString_STR(uuid_string) : "none") != 0) goto err_uuid_string;
   } else if DEE_UNLIKELY(!yield()) goto err_uuid_string;
   Dee_DECREF(uuid_string);
  } break;

  { // Calling conventions
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   if (0) { case KWD_syssv:    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_SYSV); }
   if (0) { case KWD_stdcall:  self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_STDCALL); }
   if (0) { case KWD_thiscall: self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL); }
   if (0) { case KWD_fastcall: self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_FASTCALL); }
   if (0) { case KWD_cdecl:    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_MS_CDECL); }
   if (0) { case KWD_pascal:   self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_PASCAL); }
   if (0) { case KWD_register: self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_REGISTER); }
   if (0) { case KWD_win64:    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_WIN64); }
   if (0) { case KWD_unix64:   self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_UNIX64); }
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   if (0) { case KWD_thiscall: self->a_fun_flags = (DeeFunctionFlags)(self->a_fun_flags|DeeFunctionFlags_THISCALL); }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   if (0) { case KWD_packed:   self->a_alignment = 1; }
y1: if DEE_UNLIKELY(!yield()) return -1;
  } break;

  case KWD_aligned: {
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_ALIGNAS,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__((alignas'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(DeeAttributeDecl_ParseAlignas(self,DEE_PARSER_ARGS) != 0) return -1;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_ALIGNAS,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((alignas(...'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
  } break;

  case KWD_nothrow: {
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY(DeeAttributeDecl_ParseNoExcept(self,DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_NOTHROW,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '__attribute__((nothrow(...'") != 0) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
   } else self->a_fun_flags |= DeeFunctionFlags_NOEXCEPT;
  } break;

  case KWD_noreturn: {
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY(DeeAttributeDecl_ParseNoExcept(self,DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_NORETURN,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '__attribute__((noreturn(...'") != 0) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
   } else self->a_fun_flags |= DeeFunctionFlags_NORETURN;
  } break;

  default: {
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_ATTRIBUTE_UNKNOWN,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Unknown attribute %R",DeeToken_Str((DeeObject *)token_ob)) != 0) return -1;
    if DEE_UNLIKELY(!yield()) return -1;
   } else {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_ATTRIBUTE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after '__attribute__(('") != 0) return -1;
   }
   // Try to consume the attribute with all its arguments
   return DeeLexer_SkipTokensUntil((DeeObject *)lexer,(DeeObject *)scope,config,',',end_token);
  } break;

 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseAfterSecondBracket(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS) {
 while (token.tk_id != ']') {
  if DEE_UNLIKELY(!token.tk_id) return 0; // EOF
  if DEE_UNLIKELY(DeeAttributeDecl_ParseContent(self,']',DEE_PARSER_ARGS) != 0) return -1;
  if DEE_UNLIKELY(token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) return -1;
 }
 if DEE_UNLIKELY(token.tk_id != ']') {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_BRACKET_ATTRIBUTE_1,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ']' after '[[...'") != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 if DEE_UNLIKELY(token.tk_id != ']') {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_BRACKET_ATTRIBUTE_2,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ']' after '[[...]'") != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_Parse(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS) {
 DEE_ASSERT(self);
again: switch (token.tk_id) {

  case KWD__Noreturn: {
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY(DeeAttributeDecl_ParseNoReturn(self,DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_NORETURN,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '_Noreturn(...'") != 0) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
   } else self->a_flags |= DeeFunctionFlags_NORETURN;
  } goto again;

  case KWD__Noexcept: {
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '(') {
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY(DeeAttributeDecl_ParseNoReturn(self,DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ')') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_NORETURN,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' after '_Noreturn(...'") != 0) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
   } else self->a_flags |= DeeFunctionFlags_NOEXCEPT;
  } goto again;

  case KWD__Alignas: {
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ALIGNAS,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '_Alignas'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(DeeAttributeDecl_ParseAlignas(self,DEE_PARSER_ARGS) != 0) return -1;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ALIGNAS,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '_Alignas(...'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
  } goto again;
 
  case KWD___attribute:
  case KWD___attribute__: {
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_1,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_2,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__attribute__('") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   while (token.tk_id != ')') {
    if DEE_UNLIKELY(!token.tk_id) return 0; // EOF
    if DEE_UNLIKELY(DeeAttributeDecl_ParseContent(self,')',DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ',') break;
    if DEE_UNLIKELY(!yield()) return -1;
   }
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_1,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((...'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_2,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__attribute__((...)'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
  } goto again;

  case KWD__declspec:
  case KWD___declspec: {
   if DEE_UNLIKELY(!yield()) return -1;
   if DEE_UNLIKELY(token.tk_id != '(') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_DECLSPEC,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '(' after '__declspec'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   while (token.tk_id != ')') {
    if DEE_UNLIKELY(!token.tk_id) return 0; // EOF
    if DEE_UNLIKELY(DeeAttributeDecl_ParseContent(self,')',DEE_PARSER_ARGS) != 0) return -1;
    if DEE_UNLIKELY(token.tk_id != ',') break;
    if DEE_UNLIKELY(!yield()) return -1;
   }
   if DEE_UNLIKELY(token.tk_id != ')') {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_DECLSPEC,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' after '__declspec(...'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
  } goto again;

  case '[': {
   DeeTokenObject *bracket_token;
   Dee_INCREF(bracket_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_bracket_token: Dee_DECREF(bracket_token); return -1; }
   if (token.tk_id == '[') {
    Dee_DECREF(bracket_token);
    if DEE_UNLIKELY(!yield()) return -1;
    if DEE_UNLIKELY(DeeAttributeDecl_ParseAfterSecondBracket(self,DEE_PARSER_ARGS) != 0) return -1;
    goto again;
   } else {
    if DEE_UNLIKELY(DeeLexer_Return((DeeObject *)lexer,(DeeObject *)bracket_token) != 0) goto err_bracket_token;
    Dee_DECREF(bracket_token);
   }
  } break;

  { // Function calling convention flags
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(syssv)    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_SYSV); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(stdcall)  self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_STDCALL); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(thiscall) self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(fastcall) self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_FASTCALL); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(cdecl)    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_MS_CDECL); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(pascal)   self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_PASCAL); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(register) self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_REGISTER); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(win64)    self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_WIN64); }
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(unix64)   self->a_fun_flags = (DeeFunctionFlags)((self->a_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_UNIX64); }
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   if (0) { DEE_PARSE_TOKENID_CASE_CC_SOME(thiscall) self->a_fun_flags = (DeeFunctionFlags)(self->a_fun_flags|DeeFunctionFlags_THISCALL); }
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   if DEE_UNLIKELY(!yield()) return -1;
  } goto again;

  default: break;
 }
 return 0;
}


DEE_DECL_END
