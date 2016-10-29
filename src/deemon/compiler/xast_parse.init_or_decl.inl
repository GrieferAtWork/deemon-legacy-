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

#define is_expr_begin()\
(((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0\
 ? DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)\
 : DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) ||\
((flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)!=0 && token.tk_id == '{'))

//////////////////////////////////////////////////////////////////////////
DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_ParseInitOrDecl(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstKind seq_kind, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*ast_decl,*ast_init;
 DeeTokenObject *ast_token;
again:
 switch (token.tk_id) {

  DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS
   if (DeeXAst_ParseVariableLookupModifiers(
    &vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   goto again;

  case '{':
   if ((flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)!=0 &&
       (parser_flags&DEE_PARSER_FLAG_NO_RANGE_INITIALIZER)==0) {
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    if (token.tk_id == '}')
     ast_result = DeeXAst_NewEmptySequence(seq_kind,ast_token);
    else ast_result = DeeXAst_ParseRangeInitializer(ast_token,seq_kind,DEE_PARSER_ARGS);
    Dee_DECREF(ast_token);
    if DEE_UNLIKELY(!ast_result) return NULL;
    if (token.tk_id != '}') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_RANGE_INIT,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected '}' to end range initializer after '{ ...") != 0) goto err_r;
    } else if DEE_UNLIKELY(!yield()) goto err_r;
    goto parse_vardecl_after_first;
   }
   DEE_ATTRIBUTE_FALLTHROUGH

  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    // Check for '$keyword = ' / '$keyword,'
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return NULL; }
    // Check if this is a variable declaration
    if (!DeeXAst_TokenIDIsExpressionEnd(token.tk_id)) {
     // Parse a regular expression first (without the right to create variables)
     if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token;
     Dee_DECREF(ast_token);
    } else {
     if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token;
     Dee_DECREF(ast_token);
  //case KWD___nth: case KWD___scope: case TPP_TOK_DOTS:
  DEE_XAST_VARIABLE_DECL_TOKENS
//parse_vardecl:
     if ((ast_result = DeeXAst_ParseUnaryEx(
      DEE_XAST_UNARAYSUFFIX_FLAG_NONE,vardecl_mode,
      attr,DEE_PARSER_ARGS)) == NULL) return NULL;
     // At this point we have the first portion stored in 'ast_result'
parse_vardecl_after_first:
     if (token.tk_id == '=') {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
assign_after_first:
#endif
      if ((flags&DEE_XAST_INITORDECL_FLAGS_ALLOWINIT)==0)
       goto create_single_sequence_from_ast_result;
      // Single vardecl (with initialization branch)
      Dee_INCREF(ast_token = token_ob);
      if DEE_UNLIKELY(!yield()) goto err_ast_token_r;
      if ((ast_init = _DeeXAst_ParseInitOrDecl(
       DEE_XAST_VARDECL_MODE_DEFAULT,
       DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
       DEE_XAST_INITORDECL_FLAGS_SINGLEVAL,
       DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)) == NULL) goto err_ast_token_r;
      ast_decl = DeeXAst_NewStore(
       ast_token,lexer,parser_flags,
       ast_result,ast_init,scope);
      Dee_DECREF(ast_init);
      Dee_DECREF(ast_token);
      Dee_DECREF(ast_result);
      if (token.tk_id == ',' && (flags&DEE_XAST_INITORDECL_FLAGS_SINGLEVAL)==0) {
       // Parse a comma-separated list of expressions after the first initializer
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
       if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0) {
        // Don't skip a post-script ',' if it has special meaning
        Dee_INCREF(ast_token = token_ob);
        if DEE_UNLIKELY(!yield()) {err_ast_token_decl: Dee_DECREF(ast_token); goto err_ast_decl; }
        if (!is_expr_begin()) {
         if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token_decl;
         Dee_DECREF(ast_token);
         Dee_INHERIT_REF(ast_result,ast_decl);
         goto create_single_sequence_from_ast_result;
        }
        Dee_DECREF(ast_token);
        goto uninitialized_after_first_in_ast_decl;
       } else
#endif
       {
        if DEE_UNLIKELY(!yield()) {err_ast_decl: Dee_DECREF(ast_decl); return NULL; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
        if (!is_expr_begin()) {
         Dee_INHERIT_REF(ast_result,ast_decl);
         goto create_single_sequence_from_ast_result;
        }
#endif
       }
       goto uninitialized_after_first_in_ast_decl;
      }
      Dee_INHERIT_REF(ast_result,ast_decl);
      goto create_single_sequence_from_ast_result;
     } else if (token.tk_id != ',' || (flags&DEE_XAST_INITORDECL_FLAGS_SINGLEVAL) != 0) {
      // Single vardecl (without initialization branch)
create_single_sequence_from_ast_result:
      if ((flags&DEE_XAST_INITORDECL_FLAGS_RETURNSEQ)!=0) {
       if ((flags&DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX)==0) {
        // Remember to still parse a named type suffix
        ast_decl = attr
         ? DeeXAst_ParseNamedTypeSuffixWithAttribute(vardecl_mode,flags,ast_result,attr,DEE_PARSER_ARGS)
         : DeeXAst_ParseNamedTypeSuffix(vardecl_mode,flags,ast_result,DEE_PARSER_ARGS);
        Dee_DECREF(ast_result);
        if (!ast_decl) return NULL;
       } else Dee_INHERIT_REF(ast_decl,ast_result);
       if ((flags&DEE_XAST_INITORDECL_FLAGS_NOSEQONCOLLON)!=0 && token.tk_id == ':') {
        return ast_decl; // Override the return_sequence flag
       }
       // Form a 1-elem sequence as per request by the caller
       ast_result = DeeXAst_NewSequence(seq_kind,ast_decl->ast_common.ast_token,
                                        lexer,parser_flags,1,&ast_decl);
       Dee_DECREF(ast_decl);
       return ast_result;
       //Dee_INHERIT_REF(ast_result,ast_decl);
      }
      break;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
     } else if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0) {
      // Don't skip a post-script ',' if it has special meaning
      Dee_INCREF(ast_token = token_ob);
      DEE_ASSERT(token.tk_id == ',');
      if DEE_UNLIKELY(!yield()) {err_ast_token_r: Dee_DECREF(ast_token); goto err_r; }
      if (((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0
          ? !DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)
          : !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) &&
          (token.tk_id != '{' || (flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)==0)) {
       if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token_r;
       Dee_DECREF(ast_token);
       goto create_single_sequence_from_ast_result;
      }
      Dee_DECREF(ast_token);
      goto create_multi_vardecl_noyield;
#endif
     } else {
      Dee_size_t      uninitializeda;
      Dee_size_t      uninitializedc;
      DeeXAstObject **uninitializedv,**new_uninitializedv;
      DeeXAstObject  *uninitialized_part;
      int             uninitialized_should_end;
      if DEE_UNLIKELY(!yield()) {err_r: Dee_DECREF(ast_result); return NULL; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
      // Relaxed comma lists allow for one pending comma in all situations
      if (token.tk_id == '=') goto assign_after_first;
      if (((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0
          ? !DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)
          : !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) &&
          (token.tk_id != '{' || (flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)==0)) {
       goto create_single_sequence_from_ast_result;
      }
create_multi_vardecl_noyield:
#endif
      uninitialized_should_end = 0;
      uninitializeda = 0;
      uninitializedc = 0;
      uninitializedv = NULL;
      // Multi-vardecl (initialization-branch pending)
      // Currently, the first element is parsed in 'ast_result'
      Dee_INHERIT_REF(uninitialized_part,ast_result);
      if ((ast_result = _DeeXAst_NewUnsafe(
       seq_kind,uninitialized_part->ast_common.ast_token)
       ) == NULL) { Dee_DECREF(uninitialized_part); return NULL; }
      ast_result->ast_sequence.s_elemc = 0;
      ast_result->ast_sequence.s_elemv = NULL;
      goto uninitialized_begin;
uninitialized_after_first_in_ast_decl:
      // Multi-vardecl (after single-element initialization)
      // >> //     vvv here
      // >> x = 42,foo
      // Currently, the 'x = 42' is stored in 'ast_decl'
      uninitialized_should_end = 0;
      uninitializeda = 0;
      uninitializedc = 0;
      uninitializedv = NULL;
#ifdef DEE_DEBUG
      uninitialized_part = NULL;
#endif
      // Create the result ast as a single-element sequence from 'ast_decl'
      // NOTE: We do this manually, as optimizations must be performed later
      if ((ast_result = _DeeXAst_NewUnsafe(
       seq_kind,ast_decl->ast_common.ast_token)
       ) == NULL) goto err_ast_decl;
      ast_result->ast_sequence.s_elemc = 1;
      while ((ast_result->ast_sequence.s_elemv = (
       DeeXAstObject **)malloc_nz(1*sizeof(DeeXAstObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       _DeeXAst_FreeUnsafe(ast_result);
       goto err_ast_decl;
      }
      Dee_INHERIT_REF(ast_result->ast_sequence.s_elemv[0],ast_decl);
      goto uninitialized_next;
      while (1) {
       switch (token.tk_id) {
        case ',': // More element(s)
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
         if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0) {
          Dee_INCREF(ast_token = token_ob);
          if DEE_UNLIKELY(!yield()) {err_uninitialized_part_ast_token: Dee_DECREF(ast_token); goto err_uninitialized_part; }
          if (((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0
              ? !DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)
              : !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) &&
              (token.tk_id != '{' || (flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)==0)) {
           // Return the token we previously took
           if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_uninitialized_part_ast_token;
           Dee_DECREF(ast_token);
           goto end_multivardecl_after_append;
          }
          Dee_DECREF(ast_token);
         } else
#endif
         {
          if DEE_UNLIKELY(!yield()) goto err_uninitialized_part;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
          // Relaxed comma lists allow for one pending comma in all situations
          if (((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0
              ? !DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)
              : !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) &&
              (token.tk_id != '{' || (flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)==0)
              ) goto end_multivardecl_after_append;
#endif
         }
         break;
        case '=':
         if ((flags&DEE_XAST_INITORDECL_FLAGS_ALLOWINIT)==0) { uninitialized_should_end = 1; break; }
        {
         DeeXAstObject **new_result_sequencev;
         Dee_INCREF(ast_token = token_ob);
         // Initialize 'uninitializedv'+uninitialized_part if we parse an expand expression next
         // Commit 'uninitializedv' and initialize uninitialized_part otherwise
         if DEE_UNLIKELY(!yield()) goto err_uninitialized_part_ast_token;
         if ((ast_init = _DeeXAst_ParseInitOrDecl(
          DEE_XAST_VARDECL_MODE_DEFAULT,
          DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
          DEE_XAST_INITORDECL_FLAGS_SINGLEVAL,
          DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)) == NULL) goto err_uninitialized_part_ast_token;
         DEE_ASSERT((ast_result->ast_sequence.s_elemc != 0) == 
                    (ast_result->ast_sequence.s_elemv != NULL));
         if (ast_init->ast_kind == DEE_XASTKIND_EXPAND) {
          DeeXAstObject *ast_expand_multivardecl,**mvd_iter,**mvd_end;
          struct DeeVarDeclStorage *mvd_varv,*mvd_dst;
          // This ast is meant to initialize everything from
          // 'uninitializedv' and 'uninitialized_part'
          while ((mvd_varv = (struct DeeVarDeclStorage *)malloc_nz((
           uninitializedc+1)*sizeof(struct DeeVarDeclStorage))) == NULL) {
           if DEE_LIKELY(Dee_CollectMemory()) continue;
           DeeError_NoMemory();
           goto err_uninitialized_part_ast_init;
          }
          mvd_dst = mvd_varv;
          mvd_end = (mvd_iter = uninitializedv)+uninitializedc;
          // Initialize all variable storage objects from the uninitialized asts
          while (mvd_iter != mvd_end) {
           if (DeeVarDeclStorage_InitFromXAst(mvd_dst,lexer,parser_flags,*mvd_iter,scope) != 0) {
err_mvd_varv: while (mvd_dst-- != mvd_varv) _DeeVarDeclStorage_Quit(mvd_dst);
            free_nn(mvd_varv);
            goto err_uninitialized_part_ast_init;
           }
           ++mvd_dst,++mvd_iter;
          }

          // Finally append the latest entry from 'uninitialized_part'
          DEE_ASSERT(mvd_dst == mvd_varv+uninitializedc);
          if (DeeVarDeclStorage_InitFromXAst(mvd_dst,lexer,parser_flags,uninitialized_part,scope) != 0) goto err_mvd_varv;

          // Create the multi-vardecl AST
          if ((ast_decl = _DeeXAst_NewUnsafe(DEE_XASTKIND_MULTIVARDECL,
           ast_token)) == NULL) { ++mvd_dst; goto err_mvd_varv; }
          ast_decl->ast_multivardecl.mvd_varv = mvd_varv;
          ast_decl->ast_multivardecl.mvd_varc = uninitializedc+1;
          ast_decl->ast_multivardecl.mvd_type = seq_kind;
          Dee_INCREF(ast_decl->ast_multivardecl.mvd_init = ast_init->ast_operator.op_a);
          // Optimize the vardecl
          ast_expand_multivardecl = DeeXAst_OptimizeMultiVarDecl(ast_decl,lexer,parser_flags,scope);
          Dee_DECREF(ast_decl);
          if (!ast_expand_multivardecl) goto err_uninitialized_part_ast_init;
          Dee_INHERIT_REF(ast_decl,ast_expand_multivardecl);

          // Store all uninitialized asts (including 'uninitialized_part') in 'ast_decl->ast_multivardecl.mvd_varv'
          ast_expand_multivardecl = DeeXAst_NewUnary(DEE_XASTKIND_EXPAND,ast_token,lexer,
                                                     parser_flags,ast_decl);
          Dee_DECREF(ast_decl);
          if (!ast_expand_multivardecl) goto err_uninitialized_part_ast_init;

          // Now append 'ast_expand_multivardecl' to 'ast_result->ast_sequence.s_elemv'
          while ((new_result_sequencev = (DeeXAstObject **)realloc_nz(
           ast_result->ast_sequence.s_elemv,
           (ast_result->ast_sequence.s_elemc+1)*sizeof(DeeXAstObject *))) == NULL) {
           if DEE_LIKELY(Dee_CollectMemory()) continue;
           Dee_DECREF(ast_expand_multivardecl);
           goto err_uninitialized_part_ast_init;
          }
          ast_result->ast_sequence.s_elemv = new_result_sequencev;
          Dee_INHERIT_REF(new_result_sequencev[ast_result->ast_sequence.s_elemc++],ast_expand_multivardecl);

          // Still need to do some cleanup by clearing the uninitialized buffer vector
          new_uninitializedv = uninitializedv+uninitializedc;
          while (new_uninitializedv != uninitializedv) Dee_DECREF(*--new_uninitializedv);
          uninitializedc = 0;
         } else {
          // This ast is meant to commit 'uninitializedv'
          // and initialize 'uninitialized_part'
          if ((ast_decl = DeeXAst_NewStore(
           ast_token,lexer,parser_flags,
           uninitialized_part,ast_init,scope)) == NULL) {
err_uninitialized_part_ast_init: Dee_DECREF(ast_init); goto err_uninitialized_part_ast_token;
          }
          while ((new_result_sequencev = (DeeXAstObject **)realloc_nz(
           ast_result->ast_sequence.s_elemv,(
           ast_result->ast_sequence.s_elemc+
           uninitializedc+1)*sizeof(DeeXAstObject *))) == NULL) {
           if DEE_LIKELY(Dee_CollectMemory()) continue;
           Dee_DECREF(ast_decl);
           DeeError_NoMemory();
           goto err_uninitialized_part_ast_token;
          }
          ast_result->ast_sequence.s_elemv = new_result_sequencev;
          // Move all references from our list into the result
          memcpy(new_result_sequencev+ast_result->ast_sequence.s_elemc,
                 uninitializedv,uninitializedc*sizeof(DeeXAstObject *));
          ast_result->ast_sequence.s_elemc += (uninitializedc+1);
          // Keep the buffer we already created, but mark it as empty
          uninitializedc = 0;
          Dee_INHERIT_REF(ast_result->ast_sequence.s_elemv[
           ast_result->ast_sequence.s_elemc-1],ast_decl);
         }
         Dee_DECREF(ast_init);
         Dee_DECREF(ast_token);
#ifdef DEE_DEBUG
         ast_init = NULL;
         ast_token = NULL;
#endif
         Dee_DECREF(uninitialized_part);
         if (token.tk_id == ',') {
          if DEE_UNLIKELY(!yield()) goto err_uninitialized_portion;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
          // Relaxed comma lists allow for one pending comma in all situations
          if (((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0
              ? !DeeXAst_TokenIDIsExpressionBeginDecl(token.tk_id)
              : !DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) &&
              (token.tk_id != '{' || (flags&DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE)==0)
              ) goto end_multivardecl_after_append;
#endif
          goto uninitialized_next;
         }
         goto end_multivardecl;
        } break;
        // Stop once we appended the current part to the uninitialized list
        default:end_multivardecl_after_append: uninitialized_should_end = 1; break;
       }
uninitialized_begin:
       // Append 'uninitialized_part' to the list of uninitialized variables
       if (uninitializedc == uninitializeda) {
        if (!uninitializeda) uninitializeda = 2; else uninitializeda *= 2;
        while ((new_uninitializedv = (DeeXAstObject **)realloc_nz(
         uninitializedv,uninitializeda*sizeof(DeeXAstObject *))) == NULL) {
         if DEE_LIKELY(Dee_CollectMemory()) continue;
         DeeError_NoMemory();
err_uninitialized_part: Dee_DECREF(uninitialized_part);
err_uninitialized_portion:
         if (!ast_result->ast_sequence.s_elemc) {
          DEE_ASSERT(!ast_result->ast_sequence.s_elemv);
          ast_result->ast_kind = DEE_XASTKIND_NONE;
         }
         Dee_DECREF(ast_result);
         new_uninitializedv = uninitializedv+uninitializedc;
         while (new_uninitializedv != uninitializedv) Dee_DECREF(*--new_uninitializedv);
         free(uninitializedv);
         return NULL;
        }
        uninitializedv = new_uninitializedv;
       }
       Dee_INHERIT_REF(uninitializedv[uninitializedc++],uninitialized_part);
       if (uninitialized_should_end) goto end_multivardecl;
uninitialized_next:
       // Parse the next expression portion
       if ((uninitialized_part = _DeeXAst_ParseInitOrDecl(
        vardecl_mode,(flags|DEE_XAST_INITORDECL_FLAGS_SINGLEVAL)&~(
        DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|DEE_XAST_INITORDECL_FLAGS_RETURNSEQ|
        DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA|DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON),
        seq_kind,attr,NULL,DEE_PARSER_ARGS)) == NULL) goto err_uninitialized_portion;
      }
end_multivardecl:
      {
       // Commit everything still uninitialized & free the uninialized-vector
       if (!uninitializedc) free(uninitializedv); else {
        if (!ast_result->ast_sequence.s_elemv) {
         // Inherit the buffer
         if (uninitializedc != uninitializeda) {
          while ((new_uninitializedv = (DeeXAstObject **)realloc_nz(
           uninitializedv,uninitializedc*sizeof(DeeXAstObject *))) == NULL) {
           if DEE_LIKELY(Dee_CollectMemory()) continue;
           new_uninitializedv = uninitializedv;
           break;
          }
          uninitializedv = new_uninitializedv;
         }
         ast_result->ast_sequence.s_elemv = uninitializedv; // inherit
         ast_result->ast_sequence.s_elemc = uninitializedc;
        } else {
         while ((new_uninitializedv = (DeeXAstObject **)realloc_nnz(
          ast_result->ast_sequence.s_elemv,
          (ast_result->ast_sequence.s_elemc+uninitializedc)*sizeof(DeeXAstObject *))) == NULL) {
          if DEE_LIKELY(Dee_CollectMemory()) continue;
          DeeError_NoMemory();
          goto err_uninitialized_portion;
         }
         // More all the references
         memcpy(new_uninitializedv+ast_result->ast_sequence.s_elemc,
                uninitializedv,uninitializedc*sizeof(DeeXAstObject *));
         ast_result->ast_sequence.s_elemv = new_uninitializedv;
         ast_result->ast_sequence.s_elemc += uninitializedc;
         free(uninitializedv);
#ifdef DEE_DEBUG
         uninitializedc = 0;
         uninitializeda = 0;
         uninitializedv = NULL;
#endif
        }
       }
       if (syntax_mode) *syntax_mode |= DEE_XAST_INITORDECL_SYNTAXMODE_ISSEQUENCE;
       // v I don't know if these can be a situation where this is still '0'...
       if (!ast_result->ast_sequence.s_elemc) {
        DeeObject *const_empty_seq;
        DEE_ASSERT(!ast_result->ast_sequence.s_elemv);
        ast_result->ast_kind = DEE_XASTKIND_CONST;
        switch (seq_kind) {
         case DEE_XASTKIND_TUPLE: Dee_INCREF(const_empty_seq = Dee_EmptyTuple); break;
         case DEE_XASTKIND_LIST: if ((const_empty_seq = DeeList_NewEmpty()) == NULL) {err_r_unsafe: _DeeXAst_FreeUnsafe(ast_result); return NULL; } break;
         case DEE_XASTKIND_SET: if ((const_empty_seq = DeeSet_New()) == NULL) goto err_r_unsafe; break;
         default: DEE_BUILTIN_UNREACHABLE();
        }
        Dee_INHERIT_REF(ast_result->ast_const.c_const,const_empty_seq);
       } else {
        ast_decl = DeeXAst_OptimizeSequence(ast_result,lexer,parser_flags);
        Dee_DECREF(ast_result);
        if (!ast_decl) return NULL;
        Dee_INHERIT_REF(ast_result,ast_decl);
       }
      }
     }
     break;
    }
   }
   DEE_ATTRIBUTE_FALLTHROUGH

  // Skip vardecl checks for these keywords
  // >> They are used in unary expressions
  // NOTE: This is just an optimization to we don't have to peek
  //      the first token of an expression when it's a keyword.
  DEE_ATTRIBUTEDECL_TOKENS
  DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS_NOATTR
//parse_expression:
   if ((ast_result = DeeXAst_ParseAssignEx(
    vardecl_mode&~(DEE_XAST_VARDECL_FLAG_ENABLED),
    DEE_PARSER_ARGS)) == NULL) return NULL;
   goto parse_vardecl_after_first;
 }
 DEE_ASSERT(!ast_result || (DeeObject_Check(ast_result) && DeeXAst_Check(ast_result)));
#ifdef DEE_DEBUG
 if ((flags&DEE_XAST_INITORDECL_FLAGS_RETURNSEQ) != 0 && ast_result) switch (seq_kind) {
  case DEE_XASTKIND_TUPLE: DEE_ASSERT(DeeXAst_IsTuple(ast_result)); break;
  case DEE_XASTKIND_LIST: DEE_ASSERT(DeeXAst_IsList(ast_result)); break;
  case DEE_XASTKIND_SET: DEE_ASSERT(DeeXAst_IsSet(ast_result)); break;
  default: break;
 }
#endif
 if (ast_result && (flags&(
  DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX|
  DEE_XAST_INITORDECL_FLAGS_RETURNSEQ))==0) { // Parse a type suffix
  DeeXAstObject *result2 = (flags&DEE_XAST_INITORDECL_FLAGS_SINGLEVAL)!=0 ? (attr
   ? DeeXAst_ParseNamedTypeSuffixWithAttribute(vardecl_mode,flags,ast_result,attr,DEE_PARSER_ARGS)
   : DeeXAst_ParseNamedTypeSuffix(vardecl_mode,flags,ast_result,DEE_PARSER_ARGS)) : (attr
   ? DeeXAst_ParseAllNamedTypeSuffixWithAttribute(vardecl_mode,flags,ast_result,seq_kind,attr,DEE_PARSER_ARGS)
   : DeeXAst_ParseAllNamedTypeSuffix(vardecl_mode,flags,ast_result,seq_kind,DEE_PARSER_ARGS));
  Dee_DECREF(ast_result);
  Dee_INHERIT_REF(ast_result,result2);
 } else if ((flags&DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON)!=0) {
  if (token.tk_id != ';') {
   if ((flags&DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA)==0 || token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after expression '%r'",
     ast_result) != 0) goto err_r;
   }
  } else if DEE_UNLIKELY(!yield()) goto err_r;
 }
 return ast_result;
}
#undef is_expr_begin


DEE_DECL_END
