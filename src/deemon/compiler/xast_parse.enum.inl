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

#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseEnum(
 DEE_A_INOUT DeeTokenObject *enum_token,
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_ParseEnumWithAttribute(enum_token,vardecl_mode,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseEnumWithAttribute(
 DEE_A_INOUT DeeTokenObject *enum_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 int is_enum_class; DeeTokenObject *enum_name;
 DeeXAstObject *enum_base,*ast_result;
 DeeTypeObject *enum_base_type;
 DeeObject *old_enum_type;
 if (token.tk_id == KWD_class || token.tk_id == KWD_union || token.tk_id == KWD_struct) {
  is_enum_class = 1; // TODO: Enum class
  if DEE_UNLIKELY(!yield()) return NULL;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
 } else is_enum_class = 0;
 // Parse variable modifiers
 if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
 // Parse the enum name
 if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
  Dee_INCREF(enum_name = token_ob); // Enum name
  if DEE_UNLIKELY(!yield()) {err_enum_name: Dee_XDECREF(enum_name); return NULL; }
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_enum_name;
 } else enum_name = NULL;
 // Parse the enum base
 if (DeeXAst_ParseClassBase(&enum_base,DEE_PARSER_ARGS) != 0) goto err_enum_name;
 if (enum_base) {
  if (enum_base->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_FOR_ENUM_BASE,
    (DeeObject *)lexer,(DeeObject *)enum_base->ast_common.ast_token,
    "Expected constant for enum base, but got %r",enum_base) != 0) {
err_enum_base: Dee_DECREF(enum_base); goto err_enum_name;
   }
   Dee_DECREF(enum_base); goto enum_base_use_int;
  } else if (!DeeType_Check(enum_base->ast_const.c_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_ENUM_BASE,
    (DeeObject *)lexer,(DeeObject *)enum_base->ast_common.ast_token,
    "Expected type for enum base, but got instance of %q",
    DeeType_NAME(Dee_TYPE(enum_base->ast_const.c_const))) != 0) goto err_enum_base;
   Dee_INCREF(enum_base_type = Dee_TYPE(enum_base->ast_const.c_const));
   Dee_DECREF(enum_base);
  } else Dee_INCREF(enum_base_type = (DeeTypeObject *)enum_base->ast_const.c_const);
  Dee_DECREF(enum_base);
 } else {
enum_base_use_int: // Default to using 'int' as base
  Dee_INCREF(enum_base_type = DeeObject_TYPE(Dee_rt_int));
 }

 // Lookup/Create the enum type (if a name was given)
 if (enum_name) {
  switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {
   case DEE_XAST_VARDECL_MODE_GLOBAL:
    old_enum_type = _DeeScope_GetName((DeeScopeObject *)DeeScope_Global((
     DeeObject *)scope),enum_name->tk_token.tk_id);
    break;
   case DEE_XAST_VARDECL_MODE_LOCAL:
    old_enum_type = _DeeScope_GetName(scope,enum_name->tk_token.tk_id);
    break;
   default: {
    DeeScopeObject *scope_iter = scope;
    while ((old_enum_type = _DeeScope_GetName(scope_iter,enum_name->tk_token.tk_id)) == NULL) {
     if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
     else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
     else break;
    }
   } break;
  }
  if (old_enum_type) {
   if ((DeeTypeObject *)old_enum_type != enum_base_type) {
    int temp;
    temp = DeeObject_DeepEquals((DeeObject *)enum_base_type,old_enum_type);
    if (temp < 0) { DeeError_Handled(); temp = 0; }
    if (!temp && DeeError_CompilerError(DEE_WARNING_ENUM_NAME_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)enum_name,
     "Enum name was already used by another variable") != 0) goto err_name_base_type;
   }
  } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
   if (DeeError_CompilerError(DEE_WARNING_CANT_DECLARE_NAMED_STRUCT_CONSTANT,
    (DeeObject *)lexer,(DeeObject *)enum_name,
    "Can't declare named enum constant (missing declaration rights)") != 0) {
err_name_base_type: Dee_DECREF(enum_base_type); goto err_enum_name;
   }
  } else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,enum_name) != 0) goto err_name_base_type;
#endif
   if (_DeeScope_AddDeprecatedName(scope,enum_name->tk_token.tk_id,
    (DeeObject *)enum_base_type,(DeeObject *)attr->a_depr
    ) != 0) goto err_name_base_type;
  }
 }

 // Parse the enum block
 if (token.tk_id == '{') {
  if DEE_UNLIKELY(!yield()) goto err_name_base_type;
  if (token.tk_id != '}' && DeeXAst_ParseEnumBlock(vardecl_mode,
   enum_base_type,attr,DEE_PARSER_ARGS) != 0) goto err_name_base_type;
  if (token.tk_id != '}') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_ENUM,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected '}' after 'enum { ...'") != 0) goto err_name_base_type;
  } else if DEE_UNLIKELY(!yield()) goto err_name_base_type;
 }
 // Return the enum base as a constant type
 ast_result = DeeXAst_NewConst(enum_token,(DeeObject *)enum_base_type);
 Dee_DECREF(enum_base_type);
 Dee_XDECREF(enum_name);
 return ast_result;
}


DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseEnumBlock(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT DeeTypeObject *elem_type,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 struct DeeXAstEnumDecl decl;
 decl.e_type = elem_type,decl.e_last = NULL;
 while (1) {
  struct DeeAttributeDecl entry_attr; int temp;
  _DeeAttributeDecl_InitCopy(&entry_attr,attr);
  if (DeeAttributeDecl_Parse(&entry_attr,DEE_PARSER_ARGS) == 0)
   temp = DeeXAst_ParseEnumBlockEntryWithAttribute(
    vardecl_mode,&decl,&entry_attr,DEE_PARSER_ARGS);
  else temp = -1;
  _DeeAttributeDecl_Quit(&entry_attr);
  if (temp != 0) { Dee_XDECREF(decl.e_last); return temp; }
  if (token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) { Dee_XDECREF(decl.e_last); return -1; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (!TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != '[') break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST */
 }
 Dee_XDECREF(decl.e_last);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseEnumBlockEntryWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT struct DeeXAstEnumDecl *decl,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeTokenObject *constant_name;
 DeeObject *constant_value;
 if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_FOR_ENUM_CONSTANT,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected keyword for enum constant") != 0) return -1;
  constant_name = NULL;
 } else {
  Dee_INCREF(constant_name = token_ob);
  if DEE_UNLIKELY(!yield()) {err_constant_name: Dee_XDECREF(constant_name); return -1; }
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_constant_name;
 }
 // Parse / generate the enum value
 if (token.tk_id == '=') {
  DeeXAstObject *constant_initializer;
  // Use the initializer
  if DEE_UNLIKELY(!yield()) goto err_constant_name;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_constant_name;
  if ((constant_initializer = _DeeXAst_ParseInitOrDecl(
   DEE_XAST_VARDECL_MODE_DEFAULT,
   DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
   DEE_XAST_INITORDECL_FLAGS_SINGLEVAL,
   DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS_EX(
   parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_constant_name;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
/*err_constant_initializer:*/ Dee_DECREF(constant_initializer); goto err_constant_name;
  }
  if (constant_initializer->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_ENUM_INITIALIZER,
    (DeeObject *)lexer,(DeeObject *)constant_initializer->ast_common.ast_token,
    "Expected constant expression for enum initializer, but got %r",
    constant_initializer) != 0) goto err_constant_name;
   constant_value = DeeNone_New();
   Dee_DECREF(constant_initializer);
  } else {
   Dee_INCREF(constant_value = constant_initializer->ast_const.c_const);
   Dee_DECREF(constant_initializer);
   if (DeeObject_InstanceOf(constant_value,decl->e_type)) {
    Dee_XDECREF(decl->e_last);
    Dee_INCREF(decl->e_last = constant_value);
   } else {
    DeeObject *cast_args;
    // Must cast the constant to the type of the initializer
    cast_args = DeeTuple_Pack(1,constant_value);
    Dee_DECREF(constant_value);
    if (!cast_args) goto err_constant_name;
    constant_value = DeeType_NewInstance(decl->e_type,cast_args);
    Dee_DECREF(cast_args);
    if (!constant_value) {
     if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_FAILED_TO_CAST_CONSTANT,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Failed to cast enum constant to %s: %K",
      DeeType_NAME(decl->e_last),_DeeError_HandleAndReturnType()) != 0) goto err_constant_name;
     constant_value = DeeNone_New();
    } else {
     Dee_XDECREF(decl->e_last);
     Dee_INCREF(decl->e_last = constant_value);
    }
   }
  }
 } else if (decl->e_last) {
  // Use the current enum position
  DeeObject *temp;
  if ((temp = DeeObject_Copy(decl->e_last)) == NULL)
   goto constant_gen_failed;
  constant_value = DeeObject_Inc(temp);
  Dee_DECREF(temp);
  if (!constant_value) {
constant_gen_failed:
   if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_CANT_INCREMENT_COUNTER,
    (DeeObject *)lexer,(DeeObject *)(constant_name ? constant_name : token_ob),
    "Failed to generate next enum constant value: %K",
    _DeeError_HandleAndReturnType()) != 0) goto err_constant_name;
   constant_value = DeeNone_New();
  } else {
   Dee_DECREF(decl->e_last);
   Dee_INCREF(decl->e_last = constant_value);
  }
 } else {
  // Generate a new enum
  if ((constant_value = DeeType_NewInstanceDefault(
   decl->e_type)) == NULL) goto constant_gen_failed;
  if (DeeStructuredType_Check(decl->e_type)) {
   // Initialize structured types to ZERO
   memset(DeeStructured_DATA(constant_value),0,
          DeeType_GET_SLOT(decl->e_type,tp_p_instance_size));
  }
  Dee_INCREF(decl->e_last = constant_value);
 }

 if (constant_name) {
  DeeObject *old_constant;
  // Not to store it
  // At this point the name is stored in 'constant_name'
  // and the raw value is stored in 'constant_value'
  if ((old_constant = _DeeScope_GetName(scope,constant_name->tk_token.tk_id)) != NULL) {
   int equal;
   /*if (Dee_TYPE(old_constant) != Dee_TYPE(constant_value)) equal = 0;
   else*/ if ((equal = DeeObject_DeepEquals(constant_value,old_constant)) < 0) { DeeError_Handled(); equal = 0; }
   if (!equal && DeeError_CompilerError(DEE_WARNING_ENUM_CONSTANT_NAME_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)constant_name,
    "Enum constant was already defined") != 0) goto err_constant_value;
  } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
   if (DeeError_CompilerError(DEE_WARNING_CANT_DECLARE_NEW_ENUM_CONSTANTS,
    (DeeObject *)lexer,(DeeObject *)constant_name,
    "Can't declare new enum constants (missing declaration rights)") != 0) goto err_constant_value;
  } else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,constant_name) != 0) goto err_constant_value;
#endif
   if (_DeeScope_AddDeprecatedName(scope,
    constant_name->tk_token.tk_id,constant_value,(DeeObject *)attr->a_depr) != 0) {
err_constant_value: Dee_DECREF(constant_value); goto err_constant_name;
   }
  }
 }
 Dee_DECREF(constant_value);
 Dee_XDECREF(constant_name);
 return 0;
}





DEE_DECL_END
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
