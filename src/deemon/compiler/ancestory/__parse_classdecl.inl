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
#include "expr_ast.c"
#define CONST_CLASS
#endif


// TODO: Custom super initializer in constructors

#ifdef CONST_CLASS
int _DeeLexer_ParseConstClassDecl(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *decl,
#else
int _DeeLexer_ParseClassDecl(
 struct DeeClassDeclObject *decl,
#endif
 struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 // v set to '0' for unnamed
 DEE_A_IN unsigned int class_name_id,
#ifndef CONST_CLASS
 DEE_A_OUT int *class_is_const_candidate,
#endif
 DEE_A_INOUT struct DeeAttributeDecl *attr) {
 DEE_ASSERT(token.tk_id == '{');
 enum DeeClassDeclEntryVisibility
  visibility = DeeClassDeclEntryKind_DEFAULT;
 DEE_LVERBOSE2("Parsing class declaration: %q\n",class_name_id
               ? TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id)
               : "<unnamed>");
 if (!yield()) return -1;
 while (token.tk_id == ';') if (!yield()) return -1;
 if (token.tk_id == '}') {
  if (!yield()) return -1;
 } else while (1) {
  struct DeeAttributeDecl elem_attr = DeeAttributeDecl_INIT();
  DeeExprAstObject *function_decl;
  TPPTokenID mode = 0;
  int operator_name,is_class_element = 0;
elem_again:
  if (_DeeLexer_ParseAttributeDecl(&elem_attr,self,scope,flags) == -1) goto err_elemr;
  switch (token.tk_id) {
   case '}':
    if (!yield()) return -1;
    if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) return -1;
    // fallthrough
   case 0:
    DeeAttributeDecl_Quit(&elem_attr);
    goto end_elems;
   case KWD_local:
   case KWD_default:
    visibility = DeeClassDeclEntryKind_DEFAULT;
vis_suffix:
    if (!yield()) goto err_elemr;
    while (token.tk_id == ':') if (!yield()) goto err_elemr;
    goto elem_again;
   case KWD_private:   visibility = DeeClassDeclEntryKind_PRIVATE; goto vis_suffix;
   case KWD_public:    visibility = DeeClassDeclEntryKind_PUBLIC; goto vis_suffix;
   case KWD_protected: visibility = DeeClassDeclEntryKind_PROTECTED; goto vis_suffix;
   case KWD_class:     is_class_element = 1; goto vis_suffix;
   case '~':
    if (!yield()) goto err_elemr;
    if (token.tk_id != KWD_class && token.tk_id != KWD_this && token.tk_id != class_name_id) {
     if ((class_name_id
      ? DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CLASS_AFTER_TILDE_IN_CLASSDECL,(DeeObject *)self,token_ob,
       "Expected 'this', 'class' or '%#q' after '~' in class decl",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id))
      : DeeError_CompilerError(DEE_WARNING_EXPECTED_CLASS_AFTER_TILDE_IN_CLASSDECL,(DeeObject *)self,token_ob,
       "Expected 'this' or 'class' after '~' in class decl")
      ) == -1) goto err_elemr;
     // v squint one eye for typos in destructor names
     if (TPPTokenID_IS_KEYWORD(token.tk_id)) goto dtor_y1;
    } else {dtor_y1: if (!yield()) goto err_elemr; }
    operator_name = DeeType_SLOT_ID(tp_dtor);
    goto parse_operator;
   case KWD_this:
    operator_name = DeeType_SLOT_ID(tp_p_any_ctor);
    goto yield_and_parse_operator;

   case KWD_copy: case KWD___copy__:
    operator_name = DeeType_SLOT_ID(tp_copy_ctor);
yield_and_parse_operator:
    if (!yield()) goto err_elemr;
    goto parse_operator;
   case KWD_move: case KWD___move__:
    operator_name = DeeType_SLOT_ID(tp_move_ctor);
    goto yield_and_parse_operator;

   case KWD_operator: {
    int temp;
    if (!yield()) goto err_elemr;
    if (_DeeLexer_ParseAttributeDecl(&elem_attr,self,scope,flags) == -1) goto err_elemr;
    if ((operator_name = _DeeLexer_ParseTypeSlotID(self,scope,flags)) == -1) goto err_elemr;
parse_operator:
    if (_DeeLexer_ParseAttributeDecl(&elem_attr,self,scope,flags) == -1) goto err_elemr;
    if (token.tk_id == '=') {
     if (!yield()) goto err_elemr;
     if ((function_decl = _deeparse_top(self,scope,flags)) == NULL) goto err_elemr;
    } else {
     if ((function_decl = new_ast(DeeExprAstKind_FUNCTION_EXPR,token_ob)) == NULL) goto err_elemr;
     if (elem_attr.ob_super) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPLICIT_SUPER_ATTRIBUTE_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Member operator %q has an explicit [[super(...)]] attribute",
       _Dee_TypeSlot_ToTokenID(operator_name)) == -1) goto err_elemr;
     } else {
#ifdef CONST_CLASS
      if ((elem_attr.ob_super = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_elemr;
      Dee_INCREF(elem_attr.ob_super->ob_const = (DeeObject *)DeeType_BASE(decl));
#else
      if ((elem_attr.ob_super = DeeClassDecl_BASE(decl)) != NULL)
       Dee_INCREF(elem_attr.ob_super);
      else {
       if ((elem_attr.ob_super = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_elemr;
       Dee_INCREF(elem_attr.ob_super->ob_const = (DeeObject *)&DeeObject_Type);
      }
#endif
     }
     if ((elem_attr.ob_fun_flags&DeeFunctionFlags_CC_MASK)!=0) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPLICIT_THISCALL_ATTRIBUTE_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Member operator %q has an explicit calling convention",
       _Dee_TypeSlot_ToTokenID(operator_name)) == -1) goto err_elemr;
     } else elem_attr.ob_fun_flags = (elem_attr.ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL; // Always a thiscall
     if (_DeeLexer_ParseFunctionSuffix(function_decl,self,scope,flags,&elem_attr,
      _Dee_TypeSlot_ToTokenID(operator_name)) == -1) {
err_fun_decl_elemr: Dee_DECREF(function_decl); goto err_elemr;
     }
    }
    if (operator_name) {
     Dee_size_t given_argcount,required_argcount;
#ifndef CONST_CLASS
     if (function_decl->ob_kind != DeeExprAstKind_CONSTANT) class_is_const_candidate = 0;
#endif
     // Automatic operator deduction from argument count
     switch (function_decl->ob_kind) {
      case DeeExprAstKind_FUNCTION:
      case DeeExprAstKind_FUNCTION_EXPR:
      case DeeExprAstKind_FUNCTION_NONE:
      case DeeExprAstKind_VFUNCTION:
      case DeeExprAstKind_VFUNCTION_EXPR:
      case DeeExprAstKind_VFUNCTION_NONE:
      case DeeExprAstKind_YFUNCTION:
      case DeeExprAstKind_YVFUNCTION:
       given_argcount = _DeeTokenIDList_SIZE(function_decl->ob_fun_args);
       if ((function_decl->ob_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0)
        ++given_argcount; // Account for an implicit 'this' argument
       break;
      case DeeExprAstKind_CONSTANT:
       if (DeeFunction_Check(function_decl->ob_const)) {
        given_argcount = DeeFunction_ARGC(function_decl->ob_const);
        if ((DeeFunction_FLAGS(function_decl->ob_const)&DEE_FUNCTION_FLAG_THIS)!=0)
         ++given_argcount; // Account for an implicit 'this' argument
        break;
       }
       // fallthrough
      default: given_argcount = 0; break;
     }

     switch (operator_name) {
      case DeeType_SLOT_ID(tp_p_seq_get):
       if (given_argcount == 3) operator_name = DeeType_SLOT_ID(tp_seq_range_get);
       else if (given_argcount == 2) operator_name = DeeType_SLOT_ID(tp_seq_get);
       else {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_AUTO_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,"Unexpected argument count %Iu for operator "
         "(expected 3 for '__getrange__' or 2 for '__getitem__')",given_argcount) == -1) goto err_fun_decl_elemr;
        operator_name = 0;
       }
       goto operator_check_class;
      case DeeType_SLOT_ID(tp_p_seq_del):
       if (given_argcount == 3) operator_name = DeeType_SLOT_ID(tp_seq_range_del);
       else if (given_argcount == 2) operator_name = DeeType_SLOT_ID(tp_seq_del);
       else {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_AUTO_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,"Unexpected argument count %Iu for operator "
         "(expected 3 for '__delrange__' or 2 for '__delitem__')",given_argcount) == -1) goto err_fun_decl_elemr;
        operator_name = 0;
       }
       goto operator_check_class;
      case DeeType_SLOT_ID(tp_p_seq_set):
       if (given_argcount == 4) operator_name = DeeType_SLOT_ID(tp_seq_range_set);
       else if (given_argcount == 3) operator_name = DeeType_SLOT_ID(tp_seq_set);
       else {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_AUTO_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,"Unexpected argument count %Iu for operator "
         "(expected 4 for '__setrange__' or 3 for '__setitem__')",given_argcount) == -1) goto err_fun_decl_elemr;
        operator_name = 0;
       }
       goto operator_check_class;
      case DeeType_SLOT_ID(tp_p_add):
       if (given_argcount == 1) operator_name = DeeType_SLOT_ID(tp_pos);
       else if (given_argcount == 2) operator_name = DeeType_SLOT_ID(tp_add);
       else {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_AUTO_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,"Unexpected argument count %Iu for operator "
         "(expected 1 for '__pos__' or 2 for '__add__')",given_argcount) == -1) goto err_fun_decl_elemr;
        operator_name = 0;
       }
       goto operator_check_class;
      case DeeType_SLOT_ID(tp_p_sub):
       if (given_argcount == 1) operator_name = DeeType_SLOT_ID(tp_neg);
       else if (given_argcount == 2) operator_name = DeeType_SLOT_ID(tp_sub);
       else {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_AUTO_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,"Unexpected argument count %Iu for operator "
         "(expected 1 for '__neg__' or 2 for '__sub__')",given_argcount) == -1) goto err_fun_decl_elemr;
        operator_name = 0;
       }
       goto operator_check_class;
      case DeeType_SLOT_ID(tp_p_any_ctor):
       if (given_argcount == 1) operator_name = DeeType_SLOT_ID(tp_ctor);
       else operator_name = DeeType_SLOT_ID(tp_any_ctor);
       break;
      case DeeType_SLOT_ID(tp_ctor):
      case DeeType_SLOT_ID(tp_dtor): break;
      default:operator_check_class:
       if (is_class_element) {
        if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CLASS_OPERATOR,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,
         "Invalid class operator '%#q' (must be used without 'class' prefix)",
         TPPLexer_TokenIDStr(DeeLexer_LEXER(self),_Dee_TypeSlot_ToTokenID(operator_name))) == -1
         ) goto err_fun_decl_elemr;
       }
       break;
     }
     // Confirm argument count for special operators
     switch (operator_name) {
      case DeeType_SLOT_ID(tp_copy_ctor):
      case DeeType_SLOT_ID(tp_move_ctor):
      case DeeType_SLOT_ID(tp_move_assign):
      case DeeType_SLOT_ID(tp_copy_assign):
      case DeeType_SLOT_ID(tp_any_assign):
      case DeeType_SLOT_ID(tp_add):
      case DeeType_SLOT_ID(tp_sub):
      case DeeType_SLOT_ID(tp_cmp_lo):
      case DeeType_SLOT_ID(tp_cmp_le):
      case DeeType_SLOT_ID(tp_cmp_eq):
      case DeeType_SLOT_ID(tp_cmp_ne):
      case DeeType_SLOT_ID(tp_cmp_gr):
      case DeeType_SLOT_ID(tp_cmp_ge):
      case DeeType_SLOT_ID(tp_mul):
      case DeeType_SLOT_ID(tp_div):
      case DeeType_SLOT_ID(tp_mod):
      case DeeType_SLOT_ID(tp_shl):
      case DeeType_SLOT_ID(tp_shr):
      case DeeType_SLOT_ID(tp_and):
      case DeeType_SLOT_ID(tp_or):
      case DeeType_SLOT_ID(tp_xor):
      case DeeType_SLOT_ID(tp_pow):
      case DeeType_SLOT_ID(tp_iadd):
      case DeeType_SLOT_ID(tp_isub):
      case DeeType_SLOT_ID(tp_imul):
      case DeeType_SLOT_ID(tp_idiv):
      case DeeType_SLOT_ID(tp_imod):
      case DeeType_SLOT_ID(tp_ishl):
      case DeeType_SLOT_ID(tp_ishr):
      case DeeType_SLOT_ID(tp_iand):
      case DeeType_SLOT_ID(tp_ior):
      case DeeType_SLOT_ID(tp_ixor):
      case DeeType_SLOT_ID(tp_ipow):
      case DeeType_SLOT_ID(tp_seq_get):
      case DeeType_SLOT_ID(tp_seq_del):
      case DeeType_SLOT_ID(tp_seq_contains):
      case DeeType_SLOT_ID(tp_attr_get):
      case DeeType_SLOT_ID(tp_attr_del):
       required_argcount = 2;
       break;
      case DeeType_SLOT_ID(tp_seq_set):
      case DeeType_SLOT_ID(tp_seq_range_get):
      case DeeType_SLOT_ID(tp_seq_range_del):
      case DeeType_SLOT_ID(tp_attr_set):
       required_argcount = 3;
       break;
      case DeeType_SLOT_ID(tp_seq_range_set):
       required_argcount = 4;
       break;
      case DeeType_SLOT_ID(tp_any_ctor):
      case DeeType_SLOT_ID(tp_call):
       if (given_argcount == 0) {
        if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_OPERATOR_IN_CLASSDECL,
         (DeeObject *)self,(DeeObject *)function_decl->ob_token,
         "Unexpected argument count for operator '%#q' (expected at least one argument)",
         TPPLexer_TokenIDStr(DeeLexer_LEXER(self),_Dee_TypeSlot_ToTokenID(operator_name))) == -1
         ) goto err_fun_decl_elemr;
       }
       goto operator_argc_correct;
      default: required_argcount = 1; break;
     }
     if (given_argcount != required_argcount) {
      if (DeeError_CompilerErrorf(DEE_WARNING_UNEXPECTED_ARGC_FOR_OPERATOR_IN_CLASSDECL,
       (DeeObject *)self,(DeeObject *)function_decl->ob_token,
       "Unexpected argument count for operator '%#q' (expected %Iu; got %Iu)",
       TPPLexer_TokenIDStr(DeeLexer_LEXER(self),_Dee_TypeSlot_ToTokenID(operator_name)),
       required_argcount,given_argcount) == -1) goto err_fun_decl_elemr;
     }
operator_argc_correct:;
#ifdef CONST_CLASS
     if (DeeClass_GetSlot(decl,operator_name))
#else
     DeeObject *decl_token;
     if (DeeClassDecl_HasSlot((DeeObject *)decl,operator_name,&decl_token))
#endif
     {
      temp = DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_ALREADY_EXISTS_IN_CLASSDECL,(DeeObject *)self,token_ob,
       "The operator '%#q' has already been used"
#ifndef CONST_CLASS
       "\n%s(%d) : See reference to previous declaration of this operator"
#endif
       ,TPPKeyword_BuiltinToStr(_Dee_TypeSlot_ToTokenID(operator_name))
#ifndef CONST_CLASS
       ,decl_token ? DeeToken_FILE(decl_token) : NULL,decl_token ? DeeToken_LINE(decl_token)+1 : 1
#endif
       );
     } else {
#ifdef CONST_CLASS
      if (function_decl->ob_kind != DeeExprAstKind_CONSTANT) {
       temp = DeeError_CompilerErrorf(
        DEE_WARNING_EXPECTED_CONSTANT_OPERATOR_IN_CONSTANT_CLASSDECL,
        (DeeObject *)self,(DeeObject *)function_decl->ob_token,
        "Non-constant operator callback for %q in constant class %q: %r",
        TPPKeyword_BuiltinToStr(_Dee_TypeSlot_ToTokenID(operator_name)),
        class_name_id ? TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id) : "<unnamed>",
        function_decl);
      } else {
       temp = DeeClass_SetSlot(decl,operator_name,function_decl->ob_const);
      }
#else
      temp = DeeClassDecl_AddSlot((DeeObject *)decl,visibility,operator_name,(DeeObject *)function_decl);
#endif
     }
    } else temp = 0;
    Dee_DECREF(function_decl);
    if (temp == -1) goto err_elemr;
   } goto skip_semi;

   case KWD_property: mode = KWD_property; goto begin_elem_decl;
   case KWD_function: mode = KWD_function; /*goto begin_elem_decl;*/
begin_elem_decl: {
    int temp;
    DeeObject *function_name;
    TPPTokenID function_name_id;
    char const *function_name_str;
    if (!yield()) goto err_elemr;
    if (_DeeLexer_ParseAttributeDecl(&elem_attr,self,scope,flags) == -1) goto err_elemr;
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_NAME_IN_CLASSDECL,
      (DeeObject *)self,token_ob,"Expected member function name after 'function'") == -1) goto err_elemr;
     function_name = NULL;
     function_name_str = NULL;
     function_name_id = 0;
    } else {
parse_named_element:
     function_name_id = token.tk_id;
     // Special function names (map to operators based on operator member functions in object.tp_members)
     switch (function_name_id) {
      case KWD___str__: operator_name = DeeType_SLOT_ID(tp_str);
parse_operator_y1: if (!yield()) goto err_elemr; goto parse_operator;
      case KWD___repr__: operator_name = DeeType_SLOT_ID(tp_repr); goto parse_operator_y1;
      case KWD___assign__: operator_name = DeeType_SLOT_ID(tp_any_assign); goto parse_operator_y1;
      case KWD___bool__: operator_name = DeeType_SLOT_ID(tp_bool); goto parse_operator_y1;
      case KWD___not__: operator_name = DeeType_SLOT_ID(tp_not); goto parse_operator_y1;
      case KWD___neg__: operator_name = DeeType_SLOT_ID(tp_neg); goto parse_operator_y1;
      case KWD___pos__: operator_name = DeeType_SLOT_ID(tp_pos); goto parse_operator_y1;
      case KWD___inv__: operator_name = DeeType_SLOT_ID(tp_inv); goto parse_operator_y1;
      case KWD___inc__: operator_name = DeeType_SLOT_ID(tp_inc); goto parse_operator_y1;
      case KWD___dec__: operator_name = DeeType_SLOT_ID(tp_dec); goto parse_operator_y1;
      case KWD___incpost__: operator_name = DeeType_SLOT_ID(tp_incpost); goto parse_operator_y1;
      case KWD___decpost__: operator_name = DeeType_SLOT_ID(tp_decpost); goto parse_operator_y1;
      case KWD___lo__: operator_name = DeeType_SLOT_ID(tp_cmp_lo); goto parse_operator_y1;
      case KWD___le__: operator_name = DeeType_SLOT_ID(tp_cmp_le); goto parse_operator_y1;
      case KWD___eq__: operator_name = DeeType_SLOT_ID(tp_cmp_eq); goto parse_operator_y1;
      case KWD___ne__: operator_name = DeeType_SLOT_ID(tp_cmp_ne); goto parse_operator_y1;
      case KWD___gr__: operator_name = DeeType_SLOT_ID(tp_cmp_gr); goto parse_operator_y1;
      case KWD___ge__: operator_name = DeeType_SLOT_ID(tp_cmp_ge); goto parse_operator_y1;
      case KWD___add__: operator_name = DeeType_SLOT_ID(tp_add); goto parse_operator_y1;
      case KWD___iadd__: operator_name = DeeType_SLOT_ID(tp_iadd); goto parse_operator_y1;
      case KWD___sub__: operator_name = DeeType_SLOT_ID(tp_sub); goto parse_operator_y1;
      case KWD___isub__: operator_name = DeeType_SLOT_ID(tp_isub); goto parse_operator_y1;
      case KWD___mul__: operator_name = DeeType_SLOT_ID(tp_mul); goto parse_operator_y1;
      case KWD___imul__: operator_name = DeeType_SLOT_ID(tp_imul); goto parse_operator_y1;
      case KWD___div__: operator_name = DeeType_SLOT_ID(tp_div); goto parse_operator_y1;
      case KWD___idiv__: operator_name = DeeType_SLOT_ID(tp_idiv); goto parse_operator_y1;
      case KWD___mod__: operator_name = DeeType_SLOT_ID(tp_mod); goto parse_operator_y1;
      case KWD___imod__: operator_name = DeeType_SLOT_ID(tp_imod); goto parse_operator_y1;
      case KWD___shl__: operator_name = DeeType_SLOT_ID(tp_shl); goto parse_operator_y1;
      case KWD___ishl__: operator_name = DeeType_SLOT_ID(tp_ishl); goto parse_operator_y1;
      case KWD___shr__: operator_name = DeeType_SLOT_ID(tp_shr); goto parse_operator_y1;
      case KWD___ishr__: operator_name = DeeType_SLOT_ID(tp_ishr); goto parse_operator_y1;
      case KWD___and__: operator_name = DeeType_SLOT_ID(tp_and); goto parse_operator_y1;
      case KWD___iand__: operator_name = DeeType_SLOT_ID(tp_iand); goto parse_operator_y1;
      case KWD___or__: operator_name = DeeType_SLOT_ID(tp_or); goto parse_operator_y1;
      case KWD___ior__: operator_name = DeeType_SLOT_ID(tp_ior); goto parse_operator_y1;
      case KWD___xor__: operator_name = DeeType_SLOT_ID(tp_xor); goto parse_operator_y1;
      case KWD___ixor__: operator_name = DeeType_SLOT_ID(tp_ixor); goto parse_operator_y1;
      case KWD___pow__: operator_name = DeeType_SLOT_ID(tp_pow); goto parse_operator_y1;
      case KWD___ipow__: operator_name = DeeType_SLOT_ID(tp_ipow); goto parse_operator_y1;
      case KWD___call__: operator_name = DeeType_SLOT_ID(tp_call); goto parse_operator_y1;
      case KWD___size__: operator_name = DeeType_SLOT_ID(tp_seq_size); goto parse_operator_y1;
      case KWD___getitem__: operator_name = DeeType_SLOT_ID(tp_seq_get); goto parse_operator_y1;
      case KWD___delitem__: operator_name = DeeType_SLOT_ID(tp_seq_del); goto parse_operator_y1;
      case KWD___setitem__: operator_name = DeeType_SLOT_ID(tp_seq_set); goto parse_operator_y1;
      case KWD___getrange__: operator_name = DeeType_SLOT_ID(tp_seq_range_get); goto parse_operator_y1;
      case KWD___delrange__: operator_name = DeeType_SLOT_ID(tp_seq_range_del); goto parse_operator_y1;
      case KWD___setrange__: operator_name = DeeType_SLOT_ID(tp_seq_range_set); goto parse_operator_y1;
      case KWD___contains__: operator_name = DeeType_SLOT_ID(tp_seq_contains); goto parse_operator_y1;
      case KWD___getattr__: operator_name = DeeType_SLOT_ID(tp_attr_get); goto parse_operator_y1;
      case KWD___delattr__: operator_name = DeeType_SLOT_ID(tp_attr_del); goto parse_operator_y1;
      case KWD___setattr__: operator_name = DeeType_SLOT_ID(tp_attr_set); goto parse_operator_y1;
      case KWD___iterself__: operator_name = DeeType_SLOT_ID(tp_seq_iter_self); goto parse_operator_y1;
      case KWD___iternext__: operator_name = DeeType_SLOT_ID(tp_seq_iter_next); goto parse_operator_y1;
      case KWD___hash__: operator_name = DeeType_SLOT_ID(tp_hash); goto parse_operator_y1;
      case KWD___deepcopy__:
      case KWD___is__:
      case KWD___ref__:
      case KWD___deref__:
       if (DeeError_CompilerErrorf(DEE_WARNING_CANT_OVERWRITE_OPERATOR_IN_CLASSDECL,
        (DeeObject *)self,token_ob,"Can't override special operator '%#q' (implementing as regular member function)",
        TPPLexer_TokenIDStr(DeeLexer_LEXER(self),function_name_id)) == -1) goto err_elemr;
       break;
      case KWD___copy__:
      case KWD___move__:
       if (DeeError_CompilerErrorf(DEE_WARNING_CANT_OVERWRITE_CTOR_OPERATOR_AS_FUNCTION_IN_CLASSDECL,
        (DeeObject *)self,token_ob,"Can't override constructor operator '%#q' as regular function",
        TPPLexer_TokenIDStr(DeeLexer_LEXER(self),function_name_id)) == -1) goto err_elemr;
       break;
      default: break;
     }
     function_name_str = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),function_name_id);
     if (!yield()) goto err_elemr;
     if (_DeeLexer_ParseAttributeDecl(&elem_attr,self,scope,flags) == -1) goto err_elemr;
     DEE_ASSERT(function_name_str);
     if ((function_name = DeeString_New(function_name_str)) == NULL) goto err_elemr;
    }
    if (token.tk_id == '=') {
     if (!yield()) goto err_funname_elemr;
     if (token.tk_id == '{') {
      DeeExprAstObject *getter_,*delete_,*setter_;
      if (is_class_element && !mode) {
       if (DeeError_CompilerError(DEE_WARNING_EXPECTED_PROPERTY_AFTER_CLASS_IN_CLASSDECL,
        (DeeObject *)self,token_ob,"Expected 'property' after 'class' for class property") == -1
        ) {err_funname_elemr: Dee_XDECREF(function_name); goto err_elemr;}
      }
      if (mode && mode != KWD_property) {
       if (DeeError_CompilerErrorf(DEE_WARNING_DIDNT_EXPECT_GETSET_ELEM_IN_CLASSDECL,
        (DeeObject *)self,token_ob,"Did not expect property element in class declaration after '%#q'",
        TPPKeyword_BuiltinToStr(mode)) == -1) goto err_funname_elemr;
      }
      if (_DeeLexer_ParseClassGetSet(self,scope,flags,
       &getter_,&delete_,&setter_) == -1) goto err_funname_elemr;
      if (function_name) {
#ifdef CONST_CLASS
       if (is_class_element
           ? (DeeClass_HasClassMethod(decl,function_name) || DeeClass_HasClassGetSet(decl,function_name))
           : (DeeClass_HasMethod(decl,function_name) || DeeClass_HasGetSet(decl,function_name))
       )
#else
       DeeObject *decl_token;
       if (is_class_element
           ? DeeClassDecl_HasClassFunctionOrGetSet((DeeObject *)decl,function_name,&decl_token)
           : DeeClassDecl_HasFunctionOrGetSet((DeeObject *)decl,function_name,&decl_token)
       )
#endif
       {
        temp = DeeError_CompilerErrorf(DEE_WARNING_MEMBER_NAME_ALREADY_EXISTS_IN_CLASSDECL,(DeeObject *)self,token_ob,
         "The property name '%#q' has already been used"
#ifndef CONST_CLASS
         "\n%s(%d) : See reference to previous declaration using this name"
#endif
         ,DeeString_STR(function_name)
#ifndef CONST_CLASS
         ,decl_token ? DeeToken_FILE(decl_token) : NULL,decl_token ? DeeToken_LINE(decl_token)+1 : 1
#endif
         );
       } else {
        if (!getter_ && !delete_ && !setter_) {
         if (DeeError_CompilerErrorf(DEE_WARNING_GETSET_WITHOUT_CALLBACKS_IN_CLASSDECL,
          (DeeObject *)self,token_ob,"property '%#q' does not have any callbacks",
          DeeString_STR(function_name)) == -1) goto err_funname_elemr;
        }
#ifdef CONST_CLASS
#define ENSURE_CONSTANT(callback,warning,name)\
        if (callback && callback->ob_kind != DeeExprAstKind_CONSTANT) {\
         if (DeeError_CompilerErrorf(\
          warning,(DeeObject *)self,(DeeObject *)callback->ob_token,\
          "Non-constant property." name " callback for %q in constant class %q: %r",\
          DeeString_STR(function_name),class_name_id ? TPPLexer_TokenIDStr(\
           DeeLexer_LEXER(self),class_name_id) : "<unnamed>",callback\
          ) == -1) goto no_property;\
         Dee_CLEAR(callback);\
        }
        ENSURE_CONSTANT(getter_,DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_GET_IN_CONSTANT_CLASSDECL,"__get__");
        ENSURE_CONSTANT(delete_,DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_DEL_IN_CONSTANT_CLASSDECL,"__del__");
        ENSURE_CONSTANT(setter_,DEE_WARNING_EXPECTED_CONSTANT_PROPERTY_SET_IN_CONSTANT_CLASSDECL,"__set__");
#undef ENSURE_CONSTANT
        temp = is_class_element
         ? DeeClass_AddClassGetSet(decl,function_name,
           getter_ ? (DeeObject *)getter_->ob_const : NULL,
           delete_ ? (DeeObject *)delete_->ob_const : NULL,
           setter_ ? (DeeObject *)setter_->ob_const : NULL)
         : DeeClass_AddGetSet(decl,function_name,
           getter_ ? (DeeObject *)getter_->ob_const : NULL,
           delete_ ? (DeeObject *)delete_->ob_const : NULL,
           setter_ ? (DeeObject *)setter_->ob_const : NULL);
no_property:;
#else
        temp = DeeClassDecl_AddGetSet(
         (DeeObject *)decl,visibility,is_class_element
         ? DeeClassDeclEntryKind_CLASS_GETSET
         : DeeClassDeclEntryKind_GETSET,function_name,
         (DeeObject *)getter_,(DeeObject *)delete_,(DeeObject *)setter_);
        if ((getter_ && getter_->ob_kind != DeeExprAstKind_CONSTANT) ||
            (delete_ && delete_->ob_kind != DeeExprAstKind_CONSTANT) ||
            (setter_ && setter_->ob_kind != DeeExprAstKind_CONSTANT)
            ) class_is_const_candidate = 0;
#endif
       }
      } else temp = 0;
      Dee_DECREF(function_name);
      Dee_XDECREF(getter_);
      Dee_XDECREF(delete_);
      Dee_XDECREF(setter_);
      if (temp == -1) goto err_elemr;
      goto skip_semi;
     }
     if (mode) {
      if (DeeError_CompilerErrorf(DEE_WARNING_DIDNT_EXPECT_ASSIGNED_FUNCTION_ELEM_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Did not expect assigned function element in class declaration after '%#q'",
       TPPKeyword_BuiltinToStr(mode)) == -1) goto err_elemr;
     }
     if ((function_decl = _deeparse_top(self,scope,flags)) == NULL) goto err_elemr;
    } else {
     if (is_class_element && !mode) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_AFTER_CLASS_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Expected 'function' after 'class' for class function") == -1) goto err_funname_elemr;
     }
     if (mode && mode != KWD_function) {
      if (DeeError_CompilerErrorf(DEE_WARNING_DIDNT_EXPECT_FUNCTION_ELEM_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Did not expect function element in class declaration after '%#q'",
       TPPKeyword_BuiltinToStr(mode)) == -1) goto err_funname_elemr;
     }
     if (elem_attr.ob_super) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPLICIT_SUPER_ATTRIBUTE_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Member function %q has an explicit [[super(...)]] attribute",
       function_name_str ? function_name_str : "<unnamed>") == -1) goto err_funname_elemr;
     } else {
#ifdef CONST_CLASS
      if ((elem_attr.ob_super = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_funname_elemr;
      Dee_INCREF(elem_attr.ob_super->ob_const = (DeeObject *)DeeType_BASE(decl));
#else
      if ((elem_attr.ob_super = DeeClassDecl_BASE(decl)) != NULL)
       Dee_INCREF(elem_attr.ob_super);
      else {
       if ((elem_attr.ob_super = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_funname_elemr;
       Dee_INCREF(elem_attr.ob_super->ob_const = (DeeObject *)&DeeObject_Type);
      }
#endif
     }
     if ((elem_attr.ob_fun_flags&DeeFunctionFlags_CC_MASK)!=0) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPLICIT_THISCALL_ATTRIBUTE_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Member function %q has an explicit calling convention",
       function_name_str ? function_name_str : "<unnamed>") == -1) goto err_funname_elemr;
     } else elem_attr.ob_fun_flags = (elem_attr.ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL; // Always a thiscall
     if ((function_decl = new_ast(DeeExprAstKind_FUNCTION_EXPR,token_ob)) == NULL) goto err_funname_elemr;
     if (_DeeLexer_ParseFunctionSuffix(function_decl,self,scope,flags,&elem_attr,function_name_id) == -1
         ) { Dee_DECREF(function_name); goto err_fun_decl_elemr; }
#ifndef CONST_CLASS
     if (function_decl->ob_kind != DeeExprAstKind_CONSTANT) class_is_const_candidate = 0;
#endif
    }
#if 0 // __attribute__((name(...))) doesn't actually overwrite the name through which the method is accessed
    if (elem_attr.ob_name) {
     Dee_XDECREF(function_name);
     Dee_INCREF(function_name = (DeeObject *)elem_attr.ob_name);
    }
#endif
    if (function_name) {
#ifdef CONST_CLASS
     if (is_class_element
         ? (DeeClass_HasClassMethod(decl,function_name) || DeeClass_HasClassGetSet(decl,function_name))
         : (DeeClass_HasMethod(decl,function_name) || DeeClass_HasGetSet(decl,function_name))
     )
#else
     DeeObject *decl_token;
     if (DeeClassDecl_HasFunctionOrGetSet((DeeObject *)decl,function_name,&decl_token))
#endif
     {
      temp = DeeError_CompilerErrorf(DEE_WARNING_MEMBER_NAME_ALREADY_EXISTS_IN_CLASSDECL,(DeeObject *)self,token_ob,
       "The function name '%#q' has already been used"
#ifndef CONST_CLASS
       "\n%s(%d) : See reference to previous declaration using this name"
#endif
       ,DeeString_STR(function_name)
#ifndef CONST_CLASS
       ,decl_token ? DeeToken_FILE(decl_token) : NULL,decl_token ? DeeToken_LINE(decl_token)+1 : 1
#endif
       );
     } else {
#ifdef CONST_CLASS
      if (function_decl->ob_kind != DeeExprAstKind_CONSTANT) {
       temp = DeeError_CompilerErrorf(
        DEE_WARNING_EXPECTED_CONSTANT_FUNCTION_IN_CONSTANT_CLASSDECL,
        (DeeObject *)self,(DeeObject *)function_decl->ob_token,
        "Non-constant function %q in constant class %q: %r",DeeString_STR(function_name),
        class_name_id ? TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id) : "<unnamed>",
        function_decl);
      } else {
       temp = DeeProperty_Check(function_decl->ob_const) ? (is_class_element
        ? DeeClass_AddClassGetSet(decl,function_name,
          DeeProperty_GETTER(function_decl->ob_const),
          DeeProperty_DELETE(function_decl->ob_const),
          DeeProperty_SETTER(function_decl->ob_const))
        : DeeClass_AddGetSet(decl,function_name,
          DeeProperty_GETTER(function_decl->ob_const),
          DeeProperty_DELETE(function_decl->ob_const),
          DeeProperty_SETTER(function_decl->ob_const)))
        : (is_class_element
        ? DeeClass_AddClassMethod(decl,function_name,(DeeObject *)function_decl->ob_const)
        : DeeClass_AddMethod(decl,function_name,(DeeObject *)function_decl->ob_const));
      }
#else
      temp = (function_decl->ob_kind == DeeExprAstKind_CONSTANT && DeeProperty_Check(function_decl->ob_const))
       ? DeeClassDecl_AddGetSet((DeeObject *)decl,visibility,is_class_element
       ? DeeClassDeclEntryKind_CLASS_GETSET : DeeClassDeclEntryKind_GETSET,function_name,
         DeeProperty_GETTER(function_decl->ob_const),
         DeeProperty_DELETE(function_decl->ob_const),
         DeeProperty_SETTER(function_decl->ob_const))
       : DeeClassDecl_AddMethod((DeeObject *)decl,visibility,is_class_element
       ? DeeClassDeclEntryKind_CLASS_METHOD : DeeClassDeclEntryKind_METHOD,
       function_name,(DeeObject *)function_decl);
#endif
     }
    } else temp = 0;
    Dee_DECREF(function_name);
    if (temp == -1) goto err_fun_decl_elemr;
    if (DeeExprAst_NeedSemicollon((DeeObject *)function_decl)) {
     if (token.tk_id != ';') {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_ELEMDECL_IN_CLASSDECL,
       (DeeObject *)self,token_ob,"Expected ';' after class member method declaration: %s = %r",
       function_name_str ? function_name_str : "<unnamed>",function_decl) == -1) goto err_fun_decl_elemr;
     } else do if (!yield()) goto err_fun_decl_elemr; while (token.tk_id == ';');
    }
    Dee_DECREF(function_decl);
   } goto skip_semi;

   case ';':
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_ELEMENT_IN_CLASSDECL,
     (DeeObject *)self,token_ob,"Expected element in class decl") == -1) goto err_elemr;
    do {
     if (!yield()) goto err_elemr;
skip_semi:;
    } while (token.tk_id == ';');
    break;

   default:
    // Make the 'function' keyword optional
    if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
     // c++-style class constructor
     if (token.tk_id == class_name_id) {
      operator_name = DeeType_SLOT_ID(tp_p_any_ctor);
      goto yield_and_parse_operator;
     }
     goto parse_named_element;
    }
    if (is_class_element) { operator_name = DeeType_SLOT_ID(tp_p_any_ctor); goto parse_operator; }
    if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_CLASSDECL,
     (DeeObject *)self,token_ob,"Unexpected token in class decl") == -1) goto err_elemr;
    if (!yield()) {err_elemr: DeeAttributeDecl_Quit(&elem_attr); return -1; }
    break;
  }
  DeeAttributeDecl_Quit(&elem_attr);
 }
end_elems:;
 return 0;
}

#ifdef CONST_CLASS
#undef CONST_CLASS
#endif
