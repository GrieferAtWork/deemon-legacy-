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

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES ||\
    DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES ||\
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassBase(
 DEE_A_REF DEE_A_OUT DeeXAstObject **base, DEE_PARSER_PARAMS) {
 DEE_ASSERT(base);
 switch (token.tk_id) {
  case ':':
  case KWD_extends:
   if DEE_UNLIKELY(!yield()) return -1;
   if ((*base = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_NO_RANGE_INITIALIZER))) == NULL) return -1;
   break;
#if 0 /* not just yet... (currently collides with code like: 'struct foo({ 10,20,30 })') */
  case '(':
   if DEE_UNLIKELY(!yield()) return -1;
   if ((*base = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_LPAREN_IN_CLASS_BASE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' to end class base declaration after '(%r'",*base) != 0) goto err_base_ast;
   } else if DEE_UNLIKELY(!yield()) {
err_base_ast:
    Dee_DECREF(*base);
    return -1;
   }
   break;
#endif
  default: *base = NULL; break;
 }
 return 0;
}


#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlockEntry(
 DEE_A_INOUT struct DeeXAstClassAstEntry *self,
 DEE_A_IN DeeXAstObject *class_base, DEE_PARSER_PARAMS) {
 int result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 // Predefined attributes
 attr.a_fun_flags = DeeFunctionFlags_THISCALL;
 Dee_INCREF(attr.a_super = class_base);
 result = DeeXAst_ParseClassPropertyBlockEntryWithAttribute(self,&attr,DEE_PARSER_ARGS);
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlockEntryWithAttribute(
 DEE_A_INOUT struct DeeXAstClassAstEntry *self,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeString_NEW_STATIC(callback_name_get,"__get__");
 DeeString_NEW_STATIC(callback_name_del,"__del__");
 DeeString_NEW_STATIC(callback_name_set,"__set__");
 static DeeObject *const callback_names[3] = {
  (DeeObject *)&callback_name_get,
  (DeeObject *)&callback_name_del,
  (DeeObject *)&callback_name_set};
#define CALLBACK_GET 0
#define CALLBACK_DEL 1
#define CALLBACK_SET 2
 DeeTokenObject *function_token;
 int has_prefix = 0;
 int kind;
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
 switch (token.tk_id) {
  case 0: case '}': break;

  case KWD_function:
  case KWD_operator:
   if (has_prefix) {
    if (DeeError_CompilerError(DEE_WARNING_PREFIX_ALREADY_USED_IN_PROPERTY_DECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "operator/function prefix already used in property declaration") != 0) return -1;
   }
   if DEE_UNLIKELY(!yield()) return -1;
   has_prefix = 1;
   break;

  case '.':
   Dee_INCREF(function_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_function_token;
   if (token.tk_id == '=') {
    Dee_DECREF(function_token);
    goto parse_setter;
   }
   kind = CALLBACK_GET;
   goto parse_callback_noyield;
  case '-': case KWD_del:
   Dee_INCREF(function_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_function_token;
   if (token.tk_id == '.' && !yield()) goto err_function_token;
   kind = CALLBACK_DEL;
   goto parse_callback_noyield;
  DEE_PARSE_TOKENID_CASE_PROPERTY_GETTER kind = CALLBACK_GET; goto parse_callback;
  DEE_PARSE_TOKENID_CASE_PROPERTY_DELETE /*parse_delete:*/ kind = CALLBACK_DEL; goto parse_callback;
  DEE_PARSE_TOKENID_CASE_PROPERTY_SETTER case '=': parse_setter: kind = CALLBACK_SET; goto parse_callback;
  {
   DeeXAstObject *function_ast,**slot;
   Dee_uint32_t function_syntax_mode;
parse_callback:
   Dee_INCREF(function_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_function_token: Dee_DECREF(function_token); return -1; }
parse_callback_noyield:
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_function_token;
   if (!attr->a_name) Dee_INCREF(attr->a_name = (DeeStringObject *)callback_names[kind]);
   function_ast = DeeXAst_ParseUnnamedFunction(
    function_token,attr,&function_syntax_mode,DEE_PARSER_ARGS);
   Dee_DECREF(function_token);
   if (!function_ast) return -1;
   if (DEE_XAST_FUNCTION_SYNTAX_MODE(function_syntax_mode) !=
       DEE_XAST_FUNCTION_SYNTAX_MODE_STMT && token.tk_id != ';') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_CALLBACK_IN_PROPERTY_DECL,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ';' in property declaration after callback '%r'",
     function_ast) != 0) {err_function_ast: Dee_DECREF(function_ast); return -1; }
   }
   switch (kind) {
    case CALLBACK_GET: slot = &self->ce_getset.cg_get; break;
    case CALLBACK_DEL: slot = &self->ce_getset.cg_del; break;
    case CALLBACK_SET: slot = &self->ce_getset.cg_set; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   if (*slot) {
    if (DeeError_CompilerErrorf(DEE_WARNING_GETSET_CALLBACK_ALREADY_DEFINED_IN_CLASSDECL,
     (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
     "Property callback '%s' was already defined",
     DeeString_STR(callback_names[kind])) != 0) goto err_function_ast;
    Dee_DECREF(*slot);
   }
   Dee_INHERIT_REF(*slot,function_ast);
  } break;

  default:
   if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_PROPERTY_DECL,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Unexpected token in property declaration") != 0) return -1;
   break;
 }
 while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) return -1;
 return 0;
#undef CALLBACK_SET
#undef CALLBACK_DEL
#undef CALLBACK_GET
}


DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlock(
 DEE_A_OUT struct DeeXAstClassAstEntry *self,
 DEE_A_IN DeeXAstObject *class_base, DEE_PARSER_PARAMS) {
 Dee_uint64_t old_pos;
 self->ce_getset.cg_get = NULL;
 self->ce_getset.cg_del = NULL;
 self->ce_getset.cg_set = NULL;
 while (token.tk_id && token.tk_id != '}') {
  old_pos = lexer->l_token_pos;
  if (DeeXAst_ParseClassPropertyBlockEntry(self,class_base,DEE_PARSER_ARGS) != 0) {
   Dee_XDECREF(self->ce_getset.cg_get);
   Dee_XDECREF(self->ce_getset.cg_del);
   Dee_XDECREF(self->ce_getset.cg_set);
   return -1;
  }
  if (lexer->l_token_pos == old_pos) break;
 }
 return 0;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnnamedClass(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN TPPTokenID real_class_name,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *class_base,*technical_class_base,*ast_result;
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
 if (DeeXAst_ParseClassBase(&class_base,DEE_PARSER_ARGS) != 0) return NULL;
 if (class_base) {
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_base_ast: Dee_XDECREF(class_base); return NULL;
  }
  if (!DeeXAst_IsNoEffect(class_base)) {
   // Warn about a base AST with side-effects
   if (DeeError_CompilerErrorf(DEE_WARNING_CLASS_BASE_EXPRESSION_HAS_SIDE_EFFECTS_IN_CLASSDECL,
    (DeeObject *)lexer,(DeeObject *)class_base->ast_common.ast_token,
    "Class base for %#q may be evaluated more than once, but has side-effects:\n"
    "\t%>r",attr->a_name ? DeeString_STR(attr->a_name) : NULL,class_base) != 0) goto err_base_ast;
  }
 }
 if (token.tk_id != '{') {
empty_class:
  // Class without declaration block
  ast_result = DeeXAst_NewClass(class_token,lexer,parser_flags,
                                (DeeObject *)attr->a_name,class_base,
                                DEE_XAST_PARSECLASS_FLAGS_GETTYPEFLAGS(flags),
                                0,NULL,(attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UUID)!=0
                                ? &attr->a_uuid : NULL);
  Dee_XDECREF(class_base);
  return ast_result;
 }
 if DEE_UNLIKELY(!yield()) goto err_base_ast;
 // Skip empty leading blocks
 while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) goto err_base_ast;
 if (token.tk_id == '}') {
  // Class was still empty
  if DEE_UNLIKELY(!yield()) goto err_base_ast;
  goto empty_class;
 }
 if (class_base) Dee_INCREF(technical_class_base = class_base);
 else if ((technical_class_base = DeeXAst_NewConst(
  class_token,(DeeObject *)&DeeObject_Type)) == NULL) return NULL;
 {
  Dee_uint64_t                 old_pos;
  Dee_size_t                   entrya = 2;
  Dee_size_t                   entryc = 0;
  struct DeeXAstClassAstEntry *entryv;
  struct DeeXAstClassAstEntry *new_entryv,*curr_entry;
  while ((entryv = (struct DeeXAstClassAstEntry *)malloc_nz(
   2*sizeof(struct DeeXAstClassAstEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
err_techincal_base_ast:
   Dee_DECREF(technical_class_base);
   Dee_XDECREF(class_base);
   return NULL;
  }
  // Parse all class entries
  do {
   if (entryc == entrya) {
    DEE_ASSERT(entrya); entrya *= 2;
    while ((new_entryv = (struct DeeXAstClassAstEntry *)realloc_nnz(
     entryv,entrya*sizeof(struct DeeXAstClassAstEntry))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     goto err_entryv;
    }
    entryv = new_entryv;
   }
   old_pos = lexer->l_token_pos;
   curr_entry = &entryv[entryc];
   if (DeeXAst_ParseClassEntry(curr_entry,real_class_name,(
    DeeObject *)attr->a_name,technical_class_base,DEE_PARSER_ARGS) != 0) {
err_entryv: new_entryv = entryv+entryc;
    while (new_entryv != entryv) { _DeeXAstClassAstEntry_Quit(new_entryv); --new_entryv; }
    free(entryv);
    goto err_techincal_base_ast;
   }
   if (entryv[entryc].ce_kind == DEE_XAST_CLASSENTRY_KIND_NONE) {
quit_curr_entry_and_dont_add: // Empty entry
    _DeeXAstClassAstEntry_Quit(curr_entry);
   } else {
    struct DeeXAstClassAstEntry *iter,*end;
    end = (iter = entryv)+entryc;
    // Make sure this is the first declaration of this entry
    while (iter != end) {
     if (DeeXAstClassAstEntry_SameDeclaration(iter,curr_entry)) {
      if ((curr_entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_SLOT
       ? DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_ALREADY_EXISTS_IN_CLASSDECL,
       (DeeObject *)lexer,(DeeObject *)curr_entry->ce_slot.cs_callback,
       "Class %#q already has operator '%#q' defined",
       attr->a_name ? DeeString_STR(attr->a_name) : NULL,
       _DeeType_ClassOperatorName(curr_entry->ce_slot.cs_slotid))
       : DeeError_CompilerErrorf(DEE_WARNING_MEMBER_NAME_ALREADY_EXISTS_IN_CLASSDECL,
       (DeeObject *)lexer,(DeeObject *)(
       curr_entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_METHOD ||
       curr_entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD
       ? curr_entry->ce_method.cm_callback->ast_common.ast_token : (
       curr_entry->ce_getset.cg_get ? curr_entry->ce_getset.cg_get->ast_common.ast_token :
       curr_entry->ce_getset.cg_del ? curr_entry->ce_getset.cg_del->ast_common.ast_token :
       curr_entry->ce_getset.cg_set ? curr_entry->ce_getset.cg_set->ast_common.ast_token :
       token_ob)),"Class %#q already has a member '%#q' defined",
       attr->a_name ? DeeString_STR(attr->a_name) : NULL,
       curr_entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_METHOD ||
       curr_entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD
       ? DeeString_STR(curr_entry->ce_method.cm_name)
       : DeeString_STR(curr_entry->ce_getset.cg_name))) != 0) goto err_base_ast;
      goto quit_curr_entry_and_dont_add;
     }
     ++iter;
    }
    ++entryc;
   }
   if (lexer->l_token_pos == old_pos) break;
  } while (token.tk_id && token.tk_id != '}');
  Dee_DECREF(technical_class_base);
  // Try to save some memory
  if (entryc != entrya) {
   if (!entryc) free_nn(entryv),entryv = NULL;
   else if ((new_entryv = (struct DeeXAstClassAstEntry *)realloc_nnz(
    entryv,entryc*sizeof(struct DeeXAstClassAstEntry))) != NULL) entryv = new_entryv;
  }
  ast_result = DeeXAst_NewClassFromInheritedEntries(class_token,lexer,parser_flags,
                                                    (DeeObject *)attr->a_name,class_base,
                                                    DEE_XAST_PARSECLASS_FLAGS_GETTYPEFLAGS(flags),
                                                    entryc,entryv,(attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UUID)!=0
                                                    ? &attr->a_uuid : NULL);
  Dee_XDECREF(class_base);
  if (!ast_result) {
   new_entryv = entryv+entryc;
   while (new_entryv != entryv) { _DeeXAstClassAstEntry_Quit(new_entryv); --new_entryv; }
   free(entryv);
  }
  if DEE_UNLIKELY(!yield()) Dee_CLEAR(ast_result);
  return ast_result;
 }
}




DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseClass(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_ParseClassWithAttribute(class_token,vardecl_mode,flags,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseClassWithAttribute(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *class_storage,*class_decl,*ast_result;
 while (1) {
  if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
  if (token.tk_id != KWD_final) break;
  if ((flags&DEE_XAST_PARSECLASS_FLAGS_FINAL)!=0) {
   if (DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_FINAL_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "The 'final' modifier was already used") != 0) return NULL;
  } else flags |= DEE_XAST_PARSECLASS_FLAGS_FINAL;
  if DEE_UNLIKELY(!yield()) return NULL;
 }
 if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0)
  return DeeXAst_ParseConstantClassWithAttribute(
   class_token,vardecl_mode,flags,attr,DEE_PARSER_ARGS);
 if ((!TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != TPP_TOK_COLLON_COLLON)
     || token.tk_id == KWD_extends) {
  class_storage = NULL;
  goto unnamed;
 }
 // Named class
 if ((class_storage = DeeXAst_ParseVariableLookup(
  vardecl_mode,attr,DEE_PARSER_ARGS)) == NULL) return NULL;
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_storage;
unnamed:
 if (token.tk_id == '{' || token.tk_id == '(' ||
     token.tk_id == ':' || token.tk_id == KWD_extends) {
  DeeObject *real_class_name_str; TPPTokenID real_class_name;
  if (class_storage) {
   if ((real_class_name_str = DeeXAst_Name(class_storage)) == NULL) {
err_storage: Dee_XDECREF(class_storage); return NULL;
   }
   real_class_name = TPPKeywordList_GetS(&DeeLexer_LEXER(lexer)->l_keywords,
                                         DeeString_STR(real_class_name_str),
                                         DeeString_SIZE(real_class_name_str),1);
   if (!real_class_name) { Dee_DECREF(real_class_name_str); goto err_storage; }
  } else real_class_name_str = NULL,real_class_name = 0;
  if (!attr->a_name) Dee_XINCREF(attr->a_name = (DeeStringObject *)real_class_name_str);
  class_decl = DeeXAst_ParseUnnamedClass(class_token,real_class_name,flags,attr,DEE_PARSER_ARGS);
  Dee_XDECREF(real_class_name_str);
  if (!class_decl) goto err_storage;
 } else class_decl = NULL;
 if (class_storage && class_decl) {
  ast_result = DeeXAst_NewStore(
   class_token,lexer,parser_flags,
   class_storage,class_decl,scope);
  Dee_DECREF(class_storage);
  Dee_DECREF(class_decl);
 } else if (class_storage) {
  return class_storage;
 } else if (class_decl) {
  return class_decl;
 } else {
  ast_result = DeeXAst_NewClass(class_token,lexer,parser_flags,
                                (DeeObject *)attr->a_name,NULL,
                                DEE_XAST_PARSECLASS_FLAGS_GETTYPEFLAGS(flags),
                                0,NULL,(attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UUID)!=0
                                ? &attr->a_uuid : NULL);
 }
 return ast_result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantClassWithAttribute(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeTokenObject *class_name; Dee_uint64_t old_token_pos;
 DeeTypeObject *const_class_object,*class_base;
 DeeXAstObject *class_base_ast,*ast_result;
 DEE_ASSERT((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0);
 if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
  Dee_INCREF(class_name = token_ob);
  if (!attr->a_name && (attr->a_name = (DeeStringObject *)
   DeeToken_Str((DeeObject *)class_name)) == NULL) {
err_class_name: Dee_XDECREF(class_name); return NULL;
  }
  if DEE_UNLIKELY(!yield()) goto err_class_name;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_class_name;
 } else class_name = NULL;
 if (DeeXAst_ParseClassBase(&class_base_ast,DEE_PARSER_ARGS) != 0) goto err_class_name;
 if (class_base_ast) {
  if (class_base_ast->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BASE_IN_CONSTANT_CLASSDECL,
    (DeeObject *)lexer,(DeeObject *)class_base_ast->ast_common.ast_token,
    "Expected constant for base in constant class, but got %r",class_base_ast) != 0) goto err_class_base_ast;
   class_base = &DeeObject_Type;
  } else if (!DeeType_Check(class_base_ast->ast_const.c_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_BASE_IN_CONSTANT_CLASSDECL,
    (DeeObject *)lexer,(DeeObject *)class_base_ast->ast_common.ast_token,
    "Expected type for base in constant class, but got instance of %q",
    DeeType_NAME(Dee_TYPE(class_base_ast))) != 0) goto err_class_base_ast;
   class_base = Dee_TYPE(class_base_ast->ast_const.c_const);
  } else class_base = (DeeTypeObject *)class_base_ast->ast_const.c_const;
 } else {
  class_base = &DeeObject_Type;
  class_base_ast = NULL;
 }
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_class_base_ast: Dee_DECREF(class_base_ast); goto err_class_name;
 }
 const_class_object = DeeClass_NewObjectEx(
  class_base,(DeeObject *)attr->a_name,
  (attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UUID)!=0
  ? &attr->a_uuid : NULL);
 if (!const_class_object) goto err_class_base_ast;
 if ((flags&DEE_XAST_PARSECLASS_FLAGS_FINAL)!=0)
  DeeType_GET_SLOT(const_class_object,tp_flags) |= DEE_TYPE_FLAG_NO_SUBCLASS;
 // The constant class is now created
 // >> Time to store it in the scope
 if (class_name) {
  DEE_ASSERT(DeeObject_Check(attr->a_name) && DeeString_Check(attr->a_name));
  if (_DeeScope_HasName(scope,class_name->tk_token.tk_id)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_CLASS_NAME_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)class_name,
    "Constant class name %q was already used",
    DeeString_STR(attr->a_name)) != 0) goto err_class_base_ast;
  } else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,class_name) != 0) goto err_const_class_object;
#endif
   if (_DeeScope_AddDeprecatedName(scope,class_name->tk_token.tk_id,
    (DeeObject *)const_class_object,(DeeObject *)attr->a_depr) != 0) {
err_const_class_object: Dee_DECREF(const_class_object); goto err_class_base_ast;
   }
  }
 }

 if (token.tk_id == '{') {
  DeeXAstObject *technical_class_base; TPPTokenID class_name_id;
  if (class_base_ast) Dee_INCREF(technical_class_base = class_base_ast);
  else if ((technical_class_base = DeeXAst_NewConst(
   class_token,(DeeObject *)&DeeObject_Type)) == NULL) goto err_const_class_object;
  if DEE_UNLIKELY(!yield()) {err_technical_class_base: Dee_DECREF(technical_class_base); goto err_const_class_object; }
  while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) goto err_technical_class_base;
  class_name_id = (TPPTokenID)(class_name ? class_name->tk_token.tk_id : 0);
  while (token.tk_id != '}') {
   old_token_pos = lexer->l_token_pos;
   if (DeeXAst_ParseConstantClassEntry(const_class_object,class_name_id,(
    DeeObject *)attr->a_name,technical_class_base,DEE_PARSER_ARGS) != 0) goto err_technical_class_base;
   if (lexer->l_token_pos == old_token_pos) break;
  }
  Dee_DECREF(technical_class_base);
  if DEE_UNLIKELY(!yield()) goto err_const_class_object;
 }
 Dee_XDECREF(class_base_ast);
 Dee_XDECREF(class_name);

 ast_result = DeeXAst_NewConst(class_token,(DeeObject *)const_class_object);
 Dee_DECREF(const_class_object);
 return ast_result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


DEE_DECL_END

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#ifndef __INTELLISENSE__
#define CONST_ENTRY
#include "xast_parse.class_entry.inl"
#include "xast_parse.class_entry.inl"
#endif
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#endif /* ... */

