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

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXAstStructDecl_AddMember(
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_INOUT struct DeeAttributeDecl const *attr,
 DEE_A_INOUT DeeObject *name, DEE_A_INOUT DeeTypeObject *type_,
 DEE_A_INOUT DeeTokenObject *name_token, DEE_A_INOUT DeeLexerObject *lexer) {
 Dee_size_t type_size,type_align;
 if (!DeeStructuredType_Check(type_)) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_FOR_STRUCT_MEMBER,
   (DeeObject *)lexer,(DeeObject *)name_token,
   "Expected structured type for struct member, but got %s%q",
   DeeType_Check(type_) ? "" : "instance of ",
   DeeType_Check(type_) ? DeeType_NAME(type_)
   : DeeType_NAME(Dee_TYPE(type_))) != 0) return -1;
  return 0;
 }
 if ((DeeType_FLAGS(type_)&DEE_TYPE_FLAG_INCOMPLETE)!=0) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMPLETE_TYPE_FOR_STRUCT_MEMBER,
   (DeeObject *)lexer,(DeeObject *)name_token,
   "Expected complete type struct member, but got incomplete type %q",
   DeeType_NAME(type_)) != 0) return -1;
  return 0;
 }
 if (DeeStructType_HasField(decl->s_object,name)) {
  if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_MEMBER_NAME_ALREADY_USED,
   (DeeObject *)lexer,(DeeObject *)name_token,
   "struct %k already has a member %k",decl->s_object,name) != 0) return -1;
  return 0;
 }
 if ((decl->s_flags&DEE_XAST_PARSESTRUCT_FLAG_UNION)!=0)
  return DeeStructType_AddField(decl->s_object,name,0,type_);
 type_size = DeeType_GET_SLOT(type_,tp_p_instance_size);
 type_align = DeeType_GET_SLOT(type_,tp_p_instance_size); // todo?
 if (attr->a_alignment) {
  if (attr->a_alignment < type_align) type_align = attr->a_alignment;
 } else if (type_align > DeeStructPackingStack_GET(&lexer->l_pack)) {
  type_align = DeeStructPackingStack_GET(&lexer->l_pack);
 }
 // Align the current offset as per request
 decl->s_current_offset = _DeeXAstStructDecl_AlignOffset(decl->s_current_offset,type_align);
 // Actually add the struct field
 if (DeeStructType_AddField(decl->s_object,name,decl->s_current_offset,type_) != 0) return -1;
 // Advance the current offset by the type's size
 decl->s_current_offset += type_size;
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlockEntryWithAttribute(
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 int is_explicit_struct_or_union;
 DeeXAstObject *base,*member_type;
 DeeTokenObject *member_name;
 // If it's an explicit struct/union, allow unnamed to inline the declaration
 // >> struct foo {
 // >>   int x;
 // >>   union {
 // >>     int   iy;
 // >>     float fy;
 // >>   };
 // >>   int z;
 // >> };
 is_explicit_struct_or_union = token.tk_id == KWD_struct || token.tk_id == KWD_union;
 if ((base = _DeeXAst_ParseInitOrDecl(vardecl_mode,
  DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
  DEE_XAST_INITORDECL_FLAGS_FORCECTYPES|
  DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
  DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))
  ) == NULL) return -1;
 if (base->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_STRUCT_MEMBER,
   (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
   "Expected constant expression for struct member base, but got %r",base) != 0) goto err_base;
decref_base_and_dont:
  Dee_DECREF(base);
  return 0;
 }

 if (token.tk_id == ';') {
  Dee_size_t inline_baseoffset;
  DeeStructTypeObject *inlined_struct;
  struct DeeStructMember *inlined_members;
  if (!is_explicit_struct_or_union) {
   if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_UNNAMED_MEMBERS_MUST_BE_NEW_INLINED_STRUCTS,
    (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
    "The inlined struct member isn't prefixed with 'struct' or 'union': %r",base) != 0) goto err_base;
   goto decref_base_and_dont;
  } else if (!DeeStructType_Check(base->ast_const.c_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_FOR_STRUCT_INLINE,
    (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
    "Expected structured type for inlined struct member, but got %s%q",
    DeeType_Check(base->ast_const.c_const) ? "" : "instance of ",
    DeeType_Check(base->ast_const.c_const) ? DeeType_NAME(base->ast_const.c_const)
    : DeeType_NAME(Dee_TYPE(base->ast_const.c_const))) != 0) goto err_base;
   goto decref_base_and_dont;
  } else if ((DeeType_FLAGS(base->ast_const.c_const)&DEE_TYPE_FLAG_INCOMPLETE)!=0) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMPLETE_TYPE_FOR_STRUCT_INLINE,
    (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
    "Expected complete type inlined struct member, but got incomplete type %q",
    DeeType_NAME(base->ast_const.c_const)) != 0) goto err_base;
   goto decref_base_and_dont;
  }
  // Unnamed, inlined struct member
  inlined_struct = (DeeStructTypeObject *)base->ast_const.c_const;
  inlined_members = inlined_struct->tp_smembers;
  inline_baseoffset = DeeType_GET_SLOT(inlined_struct,tp_p_instance_size); // todo?
  if (attr->a_alignment) {
   if (attr->a_alignment < inline_baseoffset) inline_baseoffset = attr->a_alignment;
  } else if (inline_baseoffset > DeeStructPackingStack_GET(&lexer->l_pack)) {
   inline_baseoffset = DeeStructPackingStack_GET(&lexer->l_pack);
  }
  // Align the base offset as per request
  inline_baseoffset = _DeeXAstStructDecl_AlignOffset(decl->s_current_offset,inline_baseoffset);
  DEE_ASSERT(inlined_members);
  while (inlined_members->sm_type) {
   if (DeeStructType_HasField(decl->s_object,(DeeObject *)inlined_members->sm_name)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_MEMBER_NAME_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
     "struct %k already has a member %k",decl->s_object,inlined_members->sm_name) != 0) goto err_base;
   } else if (DeeStructType_AddField(
    decl->s_object,(DeeObject *)inlined_members->sm_name,
    inline_baseoffset+inlined_members->sm_offset,
    (DeeTypeObject *)inlined_members->sm_type) != 0) goto err_base;
   ++inlined_members;
  }
  Dee_DECREF(base);
  return 0;
 } else while (1) {
  if ((member_type = DeeXAst_DoParseNamedTypeSuffixWithAttribute(
   base,&member_name,NULL,attr,DEE_PARSER_ARGS_EX(
   parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) {
err_base: Dee_DECREF(base); return -1;
  }
  if (token.tk_id == ':') {
   // TODO: Bit fields
  }
  if (!member_name) {
   if (DeeError_CompilerError(DEE_WARNING_STRUCT_UNNAMED_MEMBER_ISNT_INLINED_STRUCT,
    (DeeObject *)lexer,(DeeObject *)member_type->ast_common.ast_token,
    "Unnamed struct members must be inlined structs") != 0) goto err_base;
  } else if (member_type->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_STRUCT_MEMBER,
    (DeeObject *)lexer,(DeeObject *)member_name,
    "Expected constant expression for struct member type, but got %r",
    member_type) != 0) goto err_member_name;
  } else {
   DeeObject *member_name_str; int temp;
   if ((member_name_str = DeeToken_Str((DeeObject *)member_name)) == NULL) {
err_member_name: Dee_DECREF(member_name); goto err_base;
   }
   temp = DeeXAstStructDecl_AddMember(decl,attr,member_name_str,(
    DeeTypeObject *)member_type->ast_const.c_const,member_name,lexer);
   Dee_DECREF(member_name_str);
   if (temp != 0) goto err_member_name;
  }
  Dee_XDECREF(member_name);
  Dee_DECREF(member_type);
  if (token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) goto err_base;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id == ';') break;
#endif
 }
 Dee_DECREF(base);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlockEntryWithAttributeCopy(
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 struct DeeAttributeDecl used_attr; int result;
 _DeeAttributeDecl_InitCopy(&used_attr,attr);
 if ((DeeAttributeDecl_Parse(&used_attr,DEE_PARSER_ARGS)) == 0)
  result = DeeXAst_ParseStructBlockEntryWithAttribute(decl,vardecl_mode,&used_attr,DEE_PARSER_ARGS);
 else result = -1;
 _DeeAttributeDecl_Quit(&used_attr);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlock(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *struct_object,
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 struct DeeXAstStructDecl decl;
 decl.s_current_offset = 0;
 decl.s_flags = flags;
 decl.s_object = struct_object;
 while (1) {
  if (DeeXAst_ParseStructBlockEntryWithAttributeCopy(&decl,vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
  if (token.tk_id != ';') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_TYPE_KEYWORD_IN_STRUCT,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected ';' after member declaration in struct/union") != 0) return -1;
  } else do if DEE_UNLIKELY(!yield()) return -1; while (token.tk_id == ';');
  if (!DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)) break;
 }
 DeeStructType_Changed(struct_object);
 return 0;
}



DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseStruct(
 DEE_A_INOUT DeeTokenObject *struct_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if ((DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS)) == 0)
  result = DeeXAst_ParseStructWithAttribute(struct_token,vardecl_mode,flags,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseStructWithAttribute(
 DEE_A_INOUT DeeTokenObject *struct_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*struct_base;
 DeeTokenObject *struct_name;
 DeeTypeObject *struct_object;
 while (1) {
  if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
  if (token.tk_id != KWD_final) break;
  if ((flags&DEE_XAST_PARSESTRUCT_FLAG_FINAL)!=0) {
   if (DeeError_CompilerError(DEE_WARNING_VARREF_FLAG_FINAL_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "The 'final' modifier was already used") != 0) return NULL;
  } else flags |= DEE_XAST_PARSESTRUCT_FLAG_FINAL;
  if DEE_UNLIKELY(!yield()) return NULL;
 }
 if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
  Dee_INCREF(struct_name = token_ob);
  if DEE_UNLIKELY(!yield()) {err_struct_name: Dee_XDECREF(struct_name); return NULL; }
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_struct_name;
 } else struct_name = NULL;
 //if (token.tk_id == '(' && struct_name) {
 // // TODO: Special handling for this kind of base name:
 // // >> x = 42;
 // // >> struct foo { int x,y; };
 // // >> struct foo(x); // --> '(struct foo)x'
 // // >> struct foo(x) { int x,y,z; }; // --> 'struct foo: x { int x,y,z; }'
 //}
 if (DeeXAst_ParseClassBase(&struct_base,DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) != 0) goto err_struct_name;
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_struct_base;
 // Name & base are parsed. - Time to create/edit the struct
 if (struct_name) {
  switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {
   case DEE_XAST_VARDECL_MODE_GLOBAL:
    struct_object = (DeeTypeObject *)_DeeScope_GetName(
     (DeeScopeObject *)DeeScope_Global((DeeObject *)scope),struct_name->tk_token.tk_id);
    break;
   case DEE_XAST_VARDECL_MODE_LOCAL:
    struct_object = (DeeTypeObject *)_DeeScope_GetName(scope,struct_name->tk_token.tk_id);
    break;
   default: {
    DeeScopeObject *scope_iter = scope;
    while ((struct_object = (DeeTypeObject *)_DeeScope_GetName(
     scope_iter,struct_name->tk_token.tk_id)) == NULL) {
     if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
     else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
     else break;
    }
   } break;
  }
  if (!struct_object) goto new_struct_type;
  if (!DeeStructType_Check(struct_object)) {
   if (DeeError_CompilerError(DEE_WARNING_STRUCT_NAME_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)struct_name,
    "Struct name was already used by another variable") != 0) goto err_struct_base;
   Dee_CLEAR(struct_name);
   goto new_struct_type;
  } else if ((flags&DEE_XAST_PARSESTRUCT_FLAG_FINAL)!=0 && !DeeType_IsFinal(struct_object)) {
   if (DeeError_CompilerError(DEE_WARNING_NON_FINAL_STRUCT_REFERENCED_WITH_FINAL,
    (DeeObject *)lexer,(DeeObject *)struct_name,
    "Non-final struct referenced with 'final'") != 0) goto err_struct_base;
  }
  Dee_INCREF(struct_object);
 } else new_struct_type: if ((struct_object = DeeStructType_New()) == NULL) {
err_struct_base: Dee_XDECREF(struct_base); goto err_struct_name;
 } else {
  // Set the final struct attribute
  if ((flags&DEE_XAST_PARSESTRUCT_FLAG_FINAL)!=0)
   DeeType_GET_SLOT(struct_object,tp_flags) |= DEE_TYPE_FLAG_NO_SUBCLASS;
  // Set the name of the struct
  if (struct_name) {
   if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
    if (DeeError_CompilerError(DEE_WARNING_CANT_DECLARE_NAMED_STRUCT_CONSTANT,
     (DeeObject *)lexer,(DeeObject *)struct_name,
     "Can't declare named struct constant (missing declaration rights)") != 0) goto err_struct_base;
   } else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
    if (DeeXAst_CheckReservedToken(lexer,struct_name) != 0) goto err_struct_object_base;
#endif
    if (_DeeScope_AddDeprecatedName(scope,struct_name->tk_token.tk_id,
     (DeeObject *)struct_object,(DeeObject *)attr->a_depr) != 0) {
err_struct_object_base: Dee_DECREF(struct_object); goto err_struct_base;
    }
   }
  }
  if (attr->a_name) {
   if (DeeType_SetName(struct_object,DeeString_STR(attr->a_name)) != 0) goto err_struct_object_base;
  } else if (struct_name) {
   DeeObject *struct_name_str; int temp;
   if ((struct_name_str = DeeToken_Str((
    DeeObject *)struct_name)) == NULL) goto err_struct_object_base;
   temp = DeeType_SetName(struct_object,DeeString_STR(struct_name_str));
   Dee_DECREF(struct_name_str);
   if (temp != 0) goto err_struct_object_base;
  }
 }
 // Set the struct base
 if (struct_base) {
  if (struct_base->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_STRUCT_BASE,
    (DeeObject *)lexer,(DeeObject *)struct_base->ast_common.ast_token,
    "Expected constant expression for struct base, but got %r",
    struct_base) != 0) goto err_struct_object_base;
  } else if (!DeeStructuredType_Check(struct_base->ast_const.c_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_FOR_STRUCT_BASE,
    (DeeObject *)lexer,(DeeObject *)struct_base->ast_common.ast_token,
    "Expected structured type for struct base, but got %s%q",
    DeeType_Check(struct_base->ast_const.c_const) ? "" : "instance of ",
    DeeType_Check(struct_base->ast_const.c_const) ? DeeType_NAME(struct_base->ast_const.c_const)
    : DeeType_NAME(Dee_TYPE(struct_base->ast_const.c_const))) != 0) goto err_struct_object_base;
  } else if (DeeType_IsFinal(struct_base->ast_const.c_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SUBCLASSABLE_TYPE_FOR_STRUCT_BASE,
    (DeeObject *)lexer,(DeeObject *)struct_base->ast_common.ast_token,
    "Type %q in struct base was declared as final",
    DeeType_NAME(struct_base->ast_const.c_const)) != 0) goto err_struct_object_base;
  } else if ((DeeType_FLAGS(struct_base->ast_const.c_const)&DEE_TYPE_FLAG_INCOMPLETE)!=0) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMPLETE_TYPE_FOR_STRUCT_BASE,
    (DeeObject *)lexer,(DeeObject *)struct_base->ast_common.ast_token,
    "Expected complete type struct base, but got incomplete type %q",
    DeeType_NAME(struct_base->ast_const.c_const)) != 0) goto err_struct_object_base;
  } else {
   if (DeeStructType_SetBase(struct_object,(DeeTypeObject *)
    struct_base->ast_const.c_const) != 0) goto err_struct_object_base;
  }
  Dee_DECREF(struct_base);
 }

 // TODO: Apply uuid to struct type

 // Parse the struct block
 if (token.tk_id == '{') {
  if (DeeStructType_Initialized(struct_object)) {
   if (DeeError_CompilerError(DEE_WARNING_STRUCT_ALREADY_INITIALIZED,
    (DeeObject *)lexer,(DeeObject *)struct_token,
    "Struct was already initialized") != 0) goto err_struct_object;
  }
  if DEE_UNLIKELY(!yield()) {err_struct_object: Dee_DECREF(struct_object); goto err_struct_name; }
  DeeType_GET_SLOT(struct_object,tp_flags) |= DEE_TYPE_FLAG_INCOMPLETE;
  if (token.tk_id != '}' && DeeXAst_ParseStructBlock(
   struct_object,vardecl_mode,flags,attr,DEE_PARSER_ARGS) != 0) {
   DeeType_GET_SLOT(struct_object,tp_flags) &= ~DEE_TYPE_FLAG_INCOMPLETE;
   goto err_struct_object;
  }
  DeeType_GET_SLOT(struct_object,tp_flags) &= ~DEE_TYPE_FLAG_INCOMPLETE;
  if (token.tk_id != '}') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_STRUCT,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected '}' after 'struct { ...'") != 0) goto err_struct_object_base;
  } else if DEE_UNLIKELY(!yield()) goto err_struct_object;
 }
 Dee_XDECREF(struct_name);
 ast_result = DeeXAst_NewConst(struct_token,(DeeObject *)struct_object);
 Dee_DECREF(struct_object);
 return ast_result;
}

DEE_DECL_END
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
