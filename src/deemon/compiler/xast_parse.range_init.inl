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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRangeInitializerWithType(
 DEE_A_INOUT DeeXAstObject *range_type, DEE_PARSER_PARAMS) {
 DeeTokenObject *range_token;
 DeeXAstKind range_kind;
 DeeXAstObject *range_seq,*ctor_args,*ast_result;
 // Range initializer:
 // We parse the range as a sequence (if hinted at with 'range_type', of that type)
 // Then we simply cast it to 'range_type'
 Dee_INCREF(range_token = token_ob);
 if DEE_UNLIKELY(!yield()) {err_range_token: Dee_DECREF(range_token); return NULL; }
 if (range_type->ast_kind == DEE_XASTKIND_CONST) {
  DeeTypeObject *const_range_type = (DeeTypeObject *)range_type->ast_const.c_const;
       if (const_range_type == &DeeList_Type) range_kind = DEE_XASTKIND_LIST;
  else if (const_range_type == &DeeTuple_Type) range_kind = DEE_XASTKIND_TUPLE;
  else if (const_range_type == &DeeSet_Type) range_kind = DEE_XASTKIND_SET;
  else range_kind = 0;
 } else range_kind = 0;
 // NOTE: We default to a list range, which is the kind of
 //       range used as initializer for user/custom types.
 if (token.tk_id == '}') { // Empty range
  range_seq = DeeXAst_NewEmptySequence(range_kind ? range_kind : DEE_XASTKIND_LIST,range_token);
 } else range_seq = DeeXAst_ParseRangeInitializer(range_token,range_kind ? range_kind : DEE_XASTKIND_LIST,DEE_PARSER_ARGS);
 if (!range_seq) goto err_range_token;
 if (token.tk_id != '}') {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACE_AFTER_RANGE_INIT,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected '}' to end range initializer after '%r { ...",range_type) != 0) goto err_range_seq;
 } else if DEE_UNLIKELY(!yield()) {
err_range_seq: Dee_DECREF(range_seq); goto err_range_token;
 }
 // Skip the conversion asts if they're unnecessary (and would just be optimized away in any case)
 switch (range_kind) {
  case DEE_XASTKIND_TUPLE:
   if (range_seq->ast_kind == DEE_XASTKIND_CONST &&
       DeeTuple_Check(range_seq->ast_const.c_const)) goto reuse_direct_range;
   goto check_reuse_direct_range;
  case DEE_XASTKIND_LIST:
   if (range_seq->ast_kind == DEE_XASTKIND_CONST &&
       DeeList_Check(range_seq->ast_const.c_const)
       ) goto reuse_direct_range;
   goto check_reuse_direct_range;
  case DEE_XASTKIND_SET:
   if (range_seq->ast_kind == DEE_XASTKIND_CONST &&
       DeeSet_Check(range_seq->ast_const.c_const)
       ) goto reuse_direct_range;
check_reuse_direct_range:
   if (range_seq->ast_kind == DEE_XASTKIND_TUPLE ||
       range_seq->ast_kind == DEE_XASTKIND_LIST ||
       range_seq->ast_kind == DEE_XASTKIND_SET) {
    // Psht... ASTs are supposed to be immutable,
    // but we know this one's ours, so it's OK...
    range_seq->ast_kind = range_kind;
reuse_direct_range:
    Dee_DECREF(range_token);
    return range_seq;
   }
   break;
  default:
   if (range_type->ast_kind == DEE_XASTKIND_CONST &&
       range_type->ast_const.c_const == (DeeObject *)&DeeDict_Type &&
       DeeXAst_IsDict(range_seq)) goto reuse_direct_range;
   break;
 }

 // Must generate the conversion asts (range_type is probably a custom class)
 ctor_args = DeeXAst_NewSequence(DEE_XASTKIND_TUPLE,range_token,
                                 lexer,parser_flags,1,&range_seq);
 Dee_DECREF(range_seq);
 if (!ctor_args) goto err_range_token;
 ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,range_token,lexer,
                                parser_flags,range_type,ctor_args);
 Dee_DECREF(ctor_args);
 Dee_DECREF(range_token);
 if DEE_UNLIKELY(!ast_result) return NULL;
 return ast_result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRangeInitializer(
 DEE_A_INOUT DeeTokenObject *range_token, DEE_A_IN DeeXAstKind seq_kind, DEE_PARSER_PARAMS) {
 Dee_size_t entrya,entryc;
 struct DeeXAstDictEntry *entryv,*new_entryv;
 DeeXAstObject *key,*item;
 (void)range_token;
 if (token.tk_id == '.') {
  // Special parser for struct initializers
  // NOTE: These will be compiled as dicts, that should be
  //       accepted by the constructors as arguments.
  //       >> struct my_struct {
  //       >>   int x,y,z;
  //       >>   struct {
  //       >>     int w;
  //       >>   } more;
  //       >> };
  //       >> // The following two are identical
  //       >> data_1 = dict { "x": 42, "y": 10, "z": 19, "more": dict { "w": 7 } };
  //       >> data_2 = dict { .x = 42, .y = 10, .z = 19, .more = { .w = 7 } };
  //       >> inst_1 = (struct my_start)data_1;
  //       >> inst_2 = (struct my_start)data_2;
  //       >> inst_3 = (struct my_start){ .x = 42, .y = 10, .z = 19, .more = { .w = 7 } }; // c-style
  entrya = 0,entryc = 0,entryv = NULL;
  goto parse_dot_range_initializer;
 }

 if ((key = _DeeXAst_ParseInitOrDecl(
  DEE_XAST_VARDECL_MODE_DEFAULT,
  DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
  DEE_XAST_INITORDECL_FLAGS_RETURNSEQ|
  DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE|
  DEE_XAST_INITORDECL_FLAGS_NOSEQONCOLLON,
  seq_kind,NULL,NULL,DEE_PARSER_ARGS)) == NULL) return NULL;
 if (token.tk_id == ':') {
  // Special parser for dict initializers
  if DEE_UNLIKELY(!yield()) {err_key: Dee_DECREF(key); return NULL; }
  if ((item = _DeeXAst_ParseInitOrDecl(
   DEE_XAST_VARDECL_MODE_DEFAULT,
   DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
   DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
   DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA|
   DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE,
   seq_kind,NULL,NULL,DEE_PARSER_ARGS)) == NULL) goto err_key;
  entrya = 2,entryc = 0;
  while ((entryv = (struct DeeXAstDictEntry *)malloc_nz(
   2*sizeof(struct DeeXAstDictEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
/*err_key_item:*/
   Dee_DECREF(item);
   goto err_key;
  }
  entryc = 1;
  Dee_INHERIT_REF(entryv[0].de_key,key);
  Dee_INHERIT_REF(entryv[0].de_item,item);
  while (token.tk_id == ',') {
   if DEE_UNLIKELY(!yield()) {
err_entries:
    new_entryv = entryv+entryc;
    while (new_entryv != entryv) {
     --new_entryv;
     Dee_DECREF(new_entryv->de_key);
     Dee_DECREF(new_entryv->de_item);
    }
    free(entryv);
    return NULL;
   }
   if (token.tk_id == '.') {
parse_dot_range_initializer:
    // Struct-style initializer
    if DEE_UNLIKELY(!yield()) goto err_entries;
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT_RANGE_INITIALIZER,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected keyword after '.' in dot-range initializer") != 0) goto err_entries;
     if ((key = DeeXAst_NewConst(token_ob,Dee_None)) == NULL) goto err_entries;
    } else {
     DeeObject *key_string;
     if ((key_string = DeeToken_Str((DeeObject *)token_ob)) == NULL) goto err_entries;
     key = DeeXAst_NewConst(token_ob,key_string);
     Dee_DECREF(key_string);
     if (!key) goto err_entries;
     if DEE_UNLIKELY(!yield()) goto err_key_entries;
    }
    if (token.tk_id != '=') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_ASSIGN_AFTER_DOT_RANGE_INITIALIZER,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected '=' in dot-range initializer after '.%k'",
      key->ast_const.c_const) != 0) goto err_key_entries;
    } else if DEE_UNLIKELY(!yield()) goto err_key_entries;
   } else {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id != '{' && !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) break;
#endif
    if ((key = _DeeXAst_ParseInitOrDecl(
     DEE_XAST_VARDECL_MODE_DEFAULT,
     DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
     DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
     DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA|
     DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE,
     seq_kind,NULL,NULL,DEE_PARSER_ARGS)
     ) == NULL) goto err_entries;
    if (token.tk_id != ':') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COLLON_AFTER_EXPR_IN_DICT,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ':' in dict initializer after '{ ..., %r",key) != 0) goto err_key_entries;
    } else if DEE_UNLIKELY(!yield()) {
err_key_entries: Dee_DECREF(key); goto err_entries;
    }
   }
   if ((item = _DeeXAst_ParseInitOrDecl(
    DEE_XAST_VARDECL_MODE_DEFAULT,
    DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
    DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
    DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA|
    DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE,
    seq_kind,NULL,NULL,DEE_PARSER_ARGS)
    ) == NULL) goto err_key_entries;
   if (entryc == entrya) {
    if (!entrya) entrya = 2; else entrya *= 2;
    while ((new_entryv = (struct DeeXAstDictEntry *)realloc_nz(
     entryv,entrya*sizeof(struct DeeXAstDictEntry))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     Dee_DECREF(item);
     Dee_DECREF(key);
     DeeError_NoMemory();
     goto err_entries;
    }
    entryv = new_entryv;
   }
   Dee_INHERIT_REF(entryv[entryc].de_key,key);
   Dee_INHERIT_REF(entryv[entryc].de_item,item);
   ++entryc;
  }
  if (entryc != entrya && (new_entryv = (struct DeeXAstDictEntry *)realloc_nnz(
   entryv,entryc*sizeof(struct DeeXAstDictEntry))) != NULL) entryv = new_entryv;
  key = DeeXAst_NewDictFromInheritedEntryv(range_token,lexer,parser_flags,entryc,entryv);
  if (!key) goto err_entries;
 }
#undef parse_range
 return key;
}

DEE_DECL_END
