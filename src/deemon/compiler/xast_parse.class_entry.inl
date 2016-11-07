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
#define CONST_ENTRY
#endif

DEE_DECL_BEGIN

#ifdef CONST_ENTRY
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseConstantClassEntry(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *class_object, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT DeeXAstObject *class_base, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassEntry(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT DeeXAstObject *class_base, DEE_PARSER_PARAMS)
#endif
{
 int result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 // Predefined attributes
 attr.a_fun_flags = DeeFunctionFlags_THISCALL;
 Dee_INCREF(attr.a_super = class_base);
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
#ifdef CONST_ENTRY
  result = DeeXAst_ParseConstantClassEntryWithAttribute(class_object,class_name,class_name_str,&attr,DEE_PARSER_ARGS);
#else
  result = DeeXAst_ParseClassEntryWithAttribute(self,class_name,class_name_str,&attr,DEE_PARSER_ARGS);
#endif
 else result = -1;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}

#ifdef CONST_ENTRY
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseConstantClassEntryWithAttribute(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *class_object, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassEntryWithAttribute(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS)
#endif
{
 DeeObject *function_name;
 DeeXAstObject *function_ast;
 DeeTokenObject *ast_token;
 Dee_uint32_t function_syntax_mode;
 int error,operator_name,has_class_prefix = 0;

 // NOTE: We can't allow parsing of arbitrary code to be assigned to class members:
 // >> class my_class {
 // >>    member_x = ({
 // >>      print my_class(); // Initializing incomplete class (aka. very bad...)
 // >>      42;
 // >>    });
 // >> };

again: switch (token.tk_id) {
  case '}': // shouldn't happen...
  case 0: none_entry:
   if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_EOF_IN_CLASS,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Unexpected EOF in class %s",
    attr->a_name ? DeeString_STR(attr->a_name) : NULL) != 0) return -1;
#ifndef CONST_ENTRY
   self->ce_kind = DEE_XAST_CLASSENTRY_KIND_NONE;
#endif
   break;

  case '~': {
   if DEE_UNLIKELY(!yield()) return -1;
   Dee_INCREF(ast_token = token_ob);
   if (token.tk_id != KWD_class && token.tk_id != KWD_this && token.tk_id != class_name) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CLASS_AFTER_TILDE_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected 'this'%s 'class'%s%#q%s after '~' in class declaration",
     class_name ? "," : " or",class_name ? " or '" : "",
     class_name ? TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),class_name) : "",
     class_name ? "'" : "") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   if (has_class_prefix) {
    if (DeeError_CompilerError(DEE_WARNING_CLASS_DESTRUCTOR_NOT_IMPLEMENTED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Class destructor are not implemented (yet?)") != 0) goto err_ast_token;
   }
   operator_name = DeeType_SLOT_ID(__destructor__);
   goto parse_operator;
  } break;

  {
   if (0) { case KWD_copy: operator_name = DeeType_SLOT_ID(__copy__); }
   if (0) { case KWD_move: operator_name = DeeType_SLOT_ID(__move__); }
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return -1;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   goto parse_operator;
  }

  case KWD_this: {
constructor_decl:
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) return -1;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
constructor_decl_noyield:
   if (has_class_prefix) {
    if (DeeError_CompilerError(DEE_WARNING_CLASS_CONSTRUCTOR_NOT_IMPLEMENTED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Class constructor are not implemented (yet?)") != 0) goto err_ast_token;
   }
   operator_name = DeeType_SLOT_ID(__constructor__);
   goto parse_operator;
  } break;

  case KWD_class: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return -1; }
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   // Check for constructor declaration
   if (token.tk_id == '(' || token.tk_id == '{' ||
       token.tk_id == TPP_TOK_ARROW || token.tk_id == ':') goto constructor_decl_noyield;
   Dee_DECREF(ast_token);
   // Reparse w/ class prefix
   if (has_class_prefix) {
    if (DeeError_CompilerError(DEE_WARNING_CLASS_PREFIX_ALREADY_GIVEN,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "The 'class' prefix was already given") != 0) return -1;
   }
   has_class_prefix = 1;
   goto again;
  } break;

  case KWD_operator: {
   // Operator declaration
   Dee_size_t operator_argc;
   if (has_class_prefix) {
    if (DeeError_CompilerError(DEE_WARNING_CANT_DEFINE_CLASS_OPERATOR,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Can't define class operators") != 0) return -1;
   }
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   if ((operator_name = DeeXAstOperatorName_Parse(DEE_PARSER_ARGS)) < 0) goto err_ast_token;
parse_operator:
   // Fill in a missing function name
   if (!attr->a_name) {
    DeeObject *new_function_name;
    char const *used_class_named = class_name_str ? DeeString_STR(class_name_str) : "class";
    switch (operator_name) {
     case DeeType_SLOT_ID(tp_new): case DeeType_SLOT_ID(tp_ctor):
     case DeeType_SLOT_ID(tp_any_new): case DeeType_SLOT_ID(tp_any_ctor):
      new_function_name = DeeString_Newf("%#q.%#q",used_class_named,used_class_named);
      break;
     case DeeType_SLOT_ID(tp_copy_ctor): case DeeType_SLOT_ID(tp_copy_new):
      new_function_name = DeeString_Newf("%#q.copy",used_class_named);
      break;
     case DeeType_SLOT_ID(tp_move_ctor): case DeeType_SLOT_ID(tp_move_new):
      new_function_name = DeeString_Newf("%#q.move",used_class_named);
      break;
     case DeeType_SLOT_ID(tp_dtor): case DeeType_SLOT_ID(tp_clear):
      new_function_name = DeeString_Newf("%#q.~%#q",used_class_named,used_class_named);
      break;
     default:
      new_function_name = DeeString_Newf("%#q.operator %s",used_class_named,
                                         _DeeType_ClassOperatorName(operator_name));
      break;
    }
    if DEE_UNLIKELY(!new_function_name) goto err_ast_token;
    attr->a_name = (DeeStringObject *)new_function_name;
   }
   if DEE_UNLIKELY(operator_name == DEE_XAST_TYPESLOT_SUPERARGS) {
    DEE_ASSERT(DeeObject_Check(attr->a_super) && DeeXAst_Check(attr->a_super));
    //Special operator: This one isn't a [[thiscall]]
    attr->a_fun_flags &= ~DeeFunctionFlags_CC_MASK; // Delete cc-attributes
    // v Not really required...
    //Dee_CLEAR(attr->a_super); // Delete the super type
   }
#ifdef CONST_ENTRY
   function_ast = DeeXAst_ParseUnnamedFunction(
    ast_token,attr,&function_syntax_mode,DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT));
#else
   function_ast = DeeXAst_ParseUnnamedFunction(
    ast_token,attr,&function_syntax_mode,DEE_PARSER_ARGS);
#endif
   Dee_DECREF(ast_token);
   if (!function_ast) return -1;
   if (DEE_XAST_FUNCTION_SYNTAX_MODE(function_syntax_mode) !=
       DEE_XAST_FUNCTION_SYNTAX_MODE_STMT && token.tk_id != ';') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_OPERATOR_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ';' in class declaration after 'operator %s'",
     _DeeType_ClassOperatorName(operator_name)) != 0) {
err_function_ast: Dee_DECREF(function_ast); return -1;
    }
   }
   switch (function_ast->ast_kind) {
    case DEE_XASTKIND_FUNCTION:
     operator_argc = function_ast->ast_function.f_argc;
     if (operator_name == DeeType_SLOT_ID(__destructor__) &&
        (function_ast->ast_function.f_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_REFS)!=0) {
      // Warn about destructors with access to the global state
      if (DeeError_CompilerErrorf(DEE_WARNING_CLASS_DESTRUCTOR_IS_ACESSING_GLOBAL_STATE,
       (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
       "The destructor of class %#q is accessing the global state",
       class_name_str ? DeeString_STR(class_name_str) : "<unnamed>") != 0) goto err_function_ast;
     }
     break;
    case DEE_XASTKIND_CONST:
     if (DeeFunction_Check(function_ast->ast_const.c_const)) {
      operator_argc = DeeFunction_ARGC(function_ast->ast_const.c_const);
      break;
     }
     DEE_ATTRIBUTE_FALLTHROUGH
    default: operator_argc = 0; break; // *shrugs*
   }
   // Fix operator names based on argument count
   error = DeeXAst_FixOperatorWithArgcount(lexer,function_ast->ast_common.ast_token,
                                           &operator_name,operator_argc);
   if (error < 0) goto err_function_ast;
   if (error != 0) { Dee_DECREF(function_ast); goto none_entry; }
#ifdef CONST_ENTRY
   if (function_ast->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_OPERATOR_IN_CONSTANT_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
     "Expected constant for 'operator %s' in a constant class %#q",
     _DeeType_ClassOperatorName(operator_name),
     class_name_str ? DeeString_STR(class_name_str) : NULL) != 0) goto err_function_ast;
   } else {
    if (DeeClass_HasSlot(class_object,operator_name)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_ALREADY_EXISTS_IN_CLASSDECL,
        (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
        "Constant class %#q already has operator '%#q' defined",
      class_name_str ? DeeString_STR(class_name_str) : NULL,
      _DeeType_ClassOperatorName(operator_name)) != 0) goto err_function_ast;
    } 
    if (DeeClass_SetSlot(class_object,operator_name,
     function_ast->ast_const.c_const) != 0) goto err_function_ast;
   }
   Dee_DECREF(function_ast);
#else
   self->ce_kind = DEE_XAST_CLASSENTRY_KIND_SLOT;
   self->ce_slot.cs_slotid = operator_name;
   Dee_INHERIT_REF(self->ce_slot.cs_callback,function_ast);
#endif
  } break;

  case KWD_function: {
   // Function declaration
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    function_name = NULL;
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_FUNCTION_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after 'function' in class declaration") != 0) return -1;
   } else {
    if ((function_name = DeeToken_Str((DeeObject *)token_ob)) == NULL) goto err_ast_token;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_function_name;
    if DEE_UNLIKELY(!yield()) goto err_function_name;
parse_function:
    if (!attr->a_name) {
     if ((attr->a_name = (DeeStringObject *)DeeString_Newf("%#q.%s",
      class_name_str ? DeeString_STR(class_name_str) : "class",
      DeeString_STR(function_name))) == NULL) goto err_function_name;
    }
   }
#ifdef CONST_ENTRY
   function_ast = DeeXAst_ParseUnnamedFunction(
    ast_token,attr,&function_syntax_mode,DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT));
#else
   function_ast = DeeXAst_ParseUnnamedFunction(
    ast_token,attr,&function_syntax_mode,DEE_PARSER_ARGS);
#endif
   Dee_DECREF(ast_token);
   if (!function_ast) {err_function_name: Dee_XDECREF(function_name); goto err_ast_token; }
   if (DEE_XAST_FUNCTION_SYNTAX_MODE(function_syntax_mode) !=
       DEE_XAST_FUNCTION_SYNTAX_MODE_STMT && token.tk_id != ';') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_FUNCTION_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ';' in class declaration after 'function %s'",
     function_name ? DeeString_STR(function_name) : "?") != 0) {
#ifdef CONST_ENTRY
err_function_name_ast:
#endif
     Dee_DECREF(function_name); goto err_function_ast;
    }
   }
#ifdef CONST_ENTRY
   if (function_ast->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_FUNCTION_IN_CONSTANT_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
     "Expected constant for 'function %s' in a constant class",
     DeeString_STR(function_name)) != 0) goto err_function_name_ast;
    Dee_DECREF(function_name);
   } else if (function_name) {
    if DEE_UNLIKELY((DEE_UNLIKELY(has_class_prefix)
     ? (DeeClass_HasClassMethod(class_object,function_name) || DeeClass_HasClassProperty(class_object,function_name))
     : (     DeeClass_HasMethod(class_object,function_name) ||      DeeClass_HasProperty(class_object,function_name)))) {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_MEMBER_NAME_ALREADY_EXISTS_IN_CLASSDECL,
       (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
       "Constant class %#q already has a member '%#q' defined",
       DEE_LIKELY(class_name_str) ? DeeString_STR(class_name_str) : NULL,
       DeeString_STR(function_name)) != 0) goto err_function_name_ast;
    } else {
     if DEE_UNLIKELY((DEE_UNLIKELY(has_class_prefix)
      ? DeeClass_AddClassMethod(class_object,function_name,function_ast->ast_const.c_const)
      : DeeClass_AddMethod(class_object,function_name,function_ast->ast_const.c_const)
      ) != 0) goto err_function_name_ast;
    }
    Dee_DECREF(function_name);
   }
   Dee_DECREF(function_ast);
#else
   if (function_name) {
    Dee_INHERIT_REF(self->ce_method.cm_name,*(DeeStringObject **)&function_name);
    Dee_INHERIT_REF(self->ce_method.cm_callback,function_ast);
   } else Dee_DECREF(function_ast);
   self->ce_kind = has_class_prefix
    ? DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD
    : DEE_XAST_CLASSENTRY_KIND_METHOD;
#endif
  } break;

  case KWD_property: {
   // Property declaration
   if DEE_UNLIKELY(!yield()) return -1;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    function_name = NULL;
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_PROPERTY_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected keyword after 'property' in class declaration") != 0) return -1;
   } else {
    if ((function_name = DeeToken_Str((DeeObject *)token_ob)) == NULL) return -1;
    if DEE_UNLIKELY(!yield()) { Dee_DECREF(function_name); return -1; }
    if (!attr->a_name) {
     if ((attr->a_name = (DeeStringObject *)DeeString_Newf("%#q.%s",
      class_name_str ? DeeString_STR(class_name_str) : "class",
      DeeString_STR(function_name))) == NULL) goto err_function_name_notok;
    }
   }
   if (token.tk_id != '=') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STORE_AFTER_PROPERTY_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '=' after 'property %s' in class declaration",
     function_name ? DeeString_STR(function_name) : NULL) != 0) goto err_function_name_notok;
   } else if DEE_UNLIKELY(!yield()) goto err_function_name_notok;
parse_property_at_brace:
   if (token.tk_id != '{') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LBRACE_AFTER_PROPERTY_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '{' in class declaration after 'property %s ='",
     function_name ? DeeString_STR(function_name) : "?") != 0) goto err_function_name_notok;
   } else if DEE_UNLIKELY(!yield()) {
err_function_name_notok: Dee_XDECREF(function_name); return -1;
   }
#ifdef CONST_ENTRY
   {
    struct DeeXAstClassAstEntry temp_entry;
    if (DeeXAst_ParseClassPropertyBlock(&temp_entry,attr->a_super,DEE_PARSER_ARGS_EX(
     parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) != 0) goto err_function_name_notok;
    if (temp_entry.ce_getset.cg_get && temp_entry.ce_getset.cg_get->ast_kind != DEE_XASTKIND_CONST) {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_GET_IN_CONSTANT_CLASSDECL,
      (DeeObject *)lexer,(DeeObject *)temp_entry.ce_getset.cg_get->ast_common.ast_token,
      "Expected constant for 'operator __get__' in 'property %#q' in a constant class",
      DeeString_STR(temp_entry.ce_getset.cg_name)) != 0) goto err_const_property;
    } else if (temp_entry.ce_getset.cg_del && temp_entry.ce_getset.cg_del->ast_kind != DEE_XASTKIND_CONST) {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_DEL_IN_CONSTANT_CLASSDECL,
      (DeeObject *)lexer,(DeeObject *)temp_entry.ce_getset.cg_del->ast_common.ast_token,
      "Expected constant for 'operator __del__' in 'property %#q' in a constant class",
      DeeString_STR(temp_entry.ce_getset.cg_name)) != 0) goto err_const_property;
    } else if (temp_entry.ce_getset.cg_set && temp_entry.ce_getset.cg_set->ast_kind != DEE_XASTKIND_CONST) {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_SET_IN_CONSTANT_CLASSDECL,
      (DeeObject *)lexer,(DeeObject *)temp_entry.ce_getset.cg_set->ast_common.ast_token,
      "Expected constant for 'operator __set__' in 'property %#q' in a constant class",
      DeeString_STR(temp_entry.ce_getset.cg_name)) != 0) goto err_const_property;
    } else if (function_name) {
     DeeObject *used_getter,*used_delete,*used_setter;
     used_getter = temp_entry.ce_getset.cg_get ? temp_entry.ce_getset.cg_get->ast_const.c_const : NULL;
     used_delete = temp_entry.ce_getset.cg_del ? temp_entry.ce_getset.cg_del->ast_const.c_const : NULL;
     used_setter = temp_entry.ce_getset.cg_set ? temp_entry.ce_getset.cg_set->ast_const.c_const : NULL;
     if ((has_class_prefix
      ? DeeClass_AddClassProperty(class_object,function_name,used_getter,used_delete,used_setter)
      : DeeClass_AddProperty(class_object,function_name,used_getter,used_delete,used_setter)) != 0) {
err_const_property:
      Dee_XDECREF(temp_entry.ce_getset.cg_get);
      Dee_XDECREF(temp_entry.ce_getset.cg_del);
      Dee_XDECREF(temp_entry.ce_getset.cg_set);
      goto err_function_name_notok;
     }
     Dee_DECREF(function_name);
    }
    Dee_XDECREF(temp_entry.ce_getset.cg_get);
    Dee_XDECREF(temp_entry.ce_getset.cg_del);
    Dee_XDECREF(temp_entry.ce_getset.cg_set);
   }
#else
   self->ce_kind = has_class_prefix
    ? DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET
    : DEE_XAST_CLASSENTRY_KIND_GETSET;
   if (function_name) self->ce_getset.cg_name = (DeeStringObject *)function_name; // Inherit reference
   else Dee_INCREF(self->ce_getset.cg_name = (DeeStringObject *)Dee_EmptyString);
   if (DeeXAst_ParseClassPropertyBlock(self,attr->a_super,DEE_PARSER_ARGS) != 0) {
    Dee_DECREF(self->ce_getset.cg_name);
    return -1;
   }
#endif
   if (token.tk_id != '}') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACE_AFTER_PROPERTY_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '}' in class declaration after 'property %s = { ...'",
     function_name ? DeeString_STR(function_name) : "?") != 0) goto err_cg_post;
   } else if DEE_UNLIKELY(!yield()) {
err_cg_post:
#ifndef CONST_ENTRY
    Dee_DECREF(self->ce_getset.cg_name);
    Dee_XDECREF(self->ce_getset.cg_get);
    Dee_XDECREF(self->ce_getset.cg_del);
    Dee_XDECREF(self->ce_getset.cg_set);
#endif
    return -1;
   }
#ifndef CONST_ENTRY
   if (!function_name) {
    Dee_DECREF(self->ce_getset.cg_name);
    Dee_XDECREF(self->ce_getset.cg_get);
    Dee_XDECREF(self->ce_getset.cg_del);
    Dee_XDECREF(self->ce_getset.cg_set);
    self->ce_kind = DEE_XAST_CLASSENTRY_KIND_NONE;
   }
#endif
  } break;


  {
#if defined(DEE_XAST_UNARY_TOKENS_NOSTMT_NOCLASSOPS) || \
    defined(DEE_XAST_RESERVED_KEYWORD_TOKENS) || \
    defined(DEE_SAST_STATEMENT_ONLY_TOKENS)
#ifdef DEE_XAST_UNARY_TOKENS_NOSTMT
   DEE_XAST_UNARY_TOKENS_NOSTMT_NOCLASSOPS
#endif
#ifdef DEE_XAST_RESERVED_KEYWORD_TOKENS
   DEE_XAST_RESERVED_KEYWORD_TOKENS
#endif
#ifdef DEE_SAST_STATEMENT_ONLY_TOKENS
   DEE_SAST_STATEMENT_ONLY_TOKENS
#endif
   if (DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Using reserved keyword for member function name") != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
#endif /* ... */

  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (token.tk_id == class_name) goto constructor_decl;
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
    if (token.tk_id == '=') {
     // Property
     if DEE_UNLIKELY(!yield()) goto err_ast_token;
     function_name = DeeToken_Str((DeeObject *)ast_token);
     Dee_DECREF(ast_token);
     if (!function_name) return -1;
     goto parse_property_at_brace;
    }
    if ((function_name = DeeToken_Str((DeeObject *)ast_token)) == NULL) goto err_ast_token;
    // NOTE: We require a '(' to follow the function name, in case I decide
    //       to add some new keywords to class declarations, or want to
    //       start simulating the class syntax of more languages.
    if (token.tk_id != '(') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LPAREN_AFTER_CLASS_MEMBER_FUNCTION_NAME,
      (DeeObject *)lexer,(DeeObject *)ast_token,
      "Expected '(' for argument list in member function %q for class %q",
      DeeString_STR(function_name),class_name_str
      ? DeeString_STR(class_name_str) : NULL) != 0) return -1;
    }
    goto parse_function;
   }
   if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_CLASSDECL,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Unexpected token in class declaration") != 0) return -1;
   goto none_entry;
  } break;
 }
 // Skip all ';' at the end of the entry
 while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) {
#ifndef CONST_ENTRY
  _DeeXAstClassAstEntry_Quit(self);
#endif
  return -1;
 }
 return 0;
}

DEE_DECL_END

#ifdef CONST_ENTRY
#undef CONST_ENTRY
#endif
