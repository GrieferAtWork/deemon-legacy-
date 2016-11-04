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
#ifndef GUARD_DEEMON_XAST_C
#define GUARD_DEEMON_XAST_C 1
#define DEE_LIMITED_API 1

// /include/*

#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/error.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/codewriter.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>
#include <deemon/runtime/builtin_functions.h>

// */ (nano...)

DEE_DECL_BEGIN

DEE_A_RET_WUNUSED int DeeLocalVar_ShouldInitialize(
 DEE_A_IN DeeLocalVarObject const *self,
 DEE_A_IN struct DeeXAstObject *initializer) {
 DeeTypeObject const *predicted_type;
 DEE_ASSERT(DeeObject_Check(self) && DeeLocalVar_Check(self));
 DEE_ASSERT(DeeObject_Check(initializer) && DeeXAst_Check(initializer));
 if (self->lv_uses) return 1; // Someone is use the variable --> always initialize it!
 if ((self->lv_flags&DEE_LOCALVAR_FLAG_USED)!=0) return 1; // Always compile as used
 if ((self->lv_flags&DEE_LOCALVAR_FLAG_UNUSED)!=0) return 0; // Always compile as unused
 if (DeeXAst_IsNoEffect(initializer)) return 0; // Skip initializers without side-effects
 predicted_type = DeeXAst_PredictType(initializer);
 if (predicted_type && (DeeType_FLAGS(predicted_type)&DEE_TYPE_FLAG_NO_LIFETIME_EFFECT)!=0
     ) return 0; // Keeping the variable around doesn't make a difference
 return 1; // Default: Yes
}
DEE_A_RET_WUNUSED int DeeLocalVar_ShouldInitializeWithUnknownInitializer(
 DEE_A_IN DeeLocalVarObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLocalVar_Check(self));
 if (self->lv_uses) return 1; // Someone is use the variable --> always initialize it!
 if ((self->lv_flags&DEE_LOCALVAR_FLAG_UNUSED)!=0) return 0; // Always compile as unused
 return 1; // Default: Yes
}

DEE_A_RET_Z char const *DeeLocalVarKind_Name(DEE_A_IN Dee_uint32_t kind) {
 DEE_ASSERTF((kind&~(DEE_LOCALVAR_KIND_MASK))==0,"Expected only the kind-mask");
 switch (DEE_LOCALVARFLAGS_KIND(kind)) {
  case DEE_LOCALVAR_KIND_LOCAL:  return "local";
  case DEE_LOCALVAR_KIND_PARAM:  return "argument";
  case DEE_LOCALVAR_KIND_RETVAL: return "return";
  case DEE_LOCALVAR_KIND_STATIC: return "static";
  case DEE_LOCALVAR_KIND_THIS:   return "this";
  case DEE_LOCALVAR_KIND_STACK:  return "stack";
  default: break;
 }
 return "??" "?";
}



#ifdef DEE_SCOPE_FLAG_USING_LOCALS
#define have_local_names \
 ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0||\
 (((DeeScopeObject *)scope)->ob_flags&DEE_SCOPE_FLAG_USING_LOCALS)!=0)
#else
#define have_local_names \
 ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0)
#endif
#define ret_used  ((compiler_flags&DEE_COMPILER_FLAG_USED)!=0)

DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileStore(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_COMPILER_PARAMS) {
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   if DEE_UNLIKELY((ret_used
    ? DeeCodeWriter_StoreVar   (writer,self->vds_var.vs_var,have_local_names ? lexer : NULL)
    : DeeCodeWriter_StoreVarPop(writer,self->vds_var.vs_var,have_local_names ? lexer : NULL)
    ) != 0) return -1;
   break;

  case DEE_VARDECLSTORAGEKIND_ATTR:
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_attr.as_object,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_attr.as_attr,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LROT_3) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_ATTR_SET) != 0) return -1;
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
   break;

  case DEE_VARDECLSTORAGEKIND_ATTR_C: {
   Dee_size_t const_id;
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_attr_c.acs_object,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((const_id = DeeCodeWriter_AllocConst(writer,(DeeObject *)self->vds_attr_c.acs_attr,compiler_flags)) == (Dee_size_t)-1) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LROT_3) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_ATTR_SET_C,const_id) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer);
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
  } break;

  case DEE_VARDECLSTORAGEKIND_ITEM:
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_item.is_object,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_item.is_key,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LROT_3) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_SEQ_SET) != 0) return -1;
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
   break;

  case DEE_VARDECLSTORAGEKIND_RANGE:
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_range.rs_object,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((self->vds_range.rs_begin
    ? DeeXAst_Compile(self->vds_range.rs_begin,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY((self->vds_range.rs_end
    ? DeeXAst_Compile(self->vds_range.rs_end,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LROT_4) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_SEQ_RANGE_SET) != 0) return -1;
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
   break;

  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*begin;
   // If the result is used, duplicate it beforehand!
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_Dup(writer) != 0)) return -1;
   // Expand the initializer
   // TODO: Try to predict the expansion type and use OP_UNPACK_TUPLE / OP_UNPACK_LIST
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_UNPACK_SEQUENCE,self->vds_expand.es_declc) != 0) return -1;
   DeeCodeWriter_INCSTACK_N(writer,self->vds_expand.es_declc-1);
   iter = (begin = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   DEE_ASSERT(self->vds_expand.es_declc);
   // Compile all the expanded storage locations (in reverse order)
   do {
    --iter;
    if DEE_UNLIKELY(DeeXAst_CompileStore(iter,DEE_COMPILER_ARGS_EX(
     compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
   } while (iter != begin);
  } break;

  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   if DEE_UNLIKELY(DeeXAst_Compile(self->vds_assign.as_ast,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_ROT_2) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_COPY_ASSIGN) != 0) return -1;
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
   break;

  default: // Drop value
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
   break;
 }
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileMultiVarDecl(
 DEE_A_IN Dee_size_t storagec, DEE_A_IN_R(storagec) struct DeeVarDeclStorage *storagev,
 DEE_A_IN DeeXAstObject *initializer, DEE_COMPILER_PARAMS) {
 DeeTypeObject const *unpack_type;
 struct DeeVarDeclStorage *iter,*begin;
 DEE_ASSERTF(storagec != 0,"Can't compile multi-var-decl with empty storage");
 // Compile the init expression
 if DEE_UNLIKELY(DeeXAst_Compile(initializer,DEE_COMPILER_ARGS_EX(
  compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
 unpack_type = DeeXAst_PredictType(initializer);
 iter = (begin = storagev)+storagec;
 // Expand the init expression
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,(Dee_uint8_t)(
  unpack_type == &DeeTuple_Type ? OP_UNPACK_TUPLE :
  unpack_type == &DeeList_Type ? OP_UNPACK_LIST :
  OP_UNPACK_SEQUENCE),storagec) != 0) return -1;
 DeeCodeWriter_INCSTACK_N(writer,storagec-1);
 // Store the initializer values in the stack
 // >> We do this by rotating through all the variables and storing each individually
 // Note the reverse order, as the last element is currently ontop of the stack
 while (iter != begin) {
  --iter;
  if DEE_UNLIKELY(DeeXAst_CompileStore(iter,DEE_COMPILER_ARGS) != 0) return -1;
  // Rotate the stack to get the new value on the top
  // If the result isn't used, one value was popped by 'DeeXAst_CompileStore'
  if (ret_used && DEE_UNLIKELY(DeeCodeWriter_RRot(writer,storagec) != 0)) return -1;
 }
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileClassEntry(
 DEE_A_IN struct DeeXAstClassAstEntry const *entry, DEE_COMPILER_PARAMS) {
 Dee_size_t name_id;
 switch (entry->ce_kind) {
  case DEE_XAST_CLASSENTRY_KIND_SLOT:
   if DEE_UNLIKELY(DeeXAst_Compile(entry->ce_slot.cs_callback,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_CLASS_SET_SLOT,(
    Dee_uint16_t)entry->ce_slot.cs_slotid) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer);
   break;
  case DEE_XAST_CLASSENTRY_KIND_METHOD:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
   if DEE_UNLIKELY((name_id = DeeCodeWriter_AllocConst(writer,(
    DeeObject *)entry->ce_method.cm_name,compiler_flags)) == (Dee_size_t)-1) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(entry->ce_method.cm_callback,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,(Dee_uint8_t)(
       entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_METHOD
       ? OP_CLASS_ADD_METHOD : OP_CLASS_ADD_CLASS_METHOD),name_id
       ) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer);
   break;
  case DEE_XAST_CLASSENTRY_KIND_GETSET:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
   if DEE_UNLIKELY((name_id = DeeCodeWriter_AllocConst(writer,(
    DeeObject *)entry->ce_getset.cg_name,compiler_flags)) == (Dee_size_t)-1) return -1;
   if (DEE_UNLIKELY(entry->ce_getset.cg_get
      ? DeeXAst_Compile(entry->ce_getset.cg_get,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
      : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString,compiler_flags)) != 0 || DEE_UNLIKELY(entry->ce_getset.cg_del
      ? DeeXAst_Compile(entry->ce_getset.cg_del,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
      : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString,compiler_flags)) != 0 || DEE_UNLIKELY(entry->ce_getset.cg_set
      ? DeeXAst_Compile(entry->ce_getset.cg_set,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
      : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString,compiler_flags)) != 0 ||
        DeeCodeWriter_WriteOpWithSizeArg(writer,(Dee_uint8_t)(
        entry->ce_kind == DEE_XAST_CLASSENTRY_KIND_GETSET
        ? OP_CLASS_ADD_GETSET : OP_CLASS_ADD_CLASS_GETSET),name_id) != 0
      ) return -1;
   DeeCodeWriter_DECSTACK_N(writer,3);
   break;
  default: break;
 }
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileSequence(
 DEE_A_IN Dee_uint8_t seq_op, DEE_A_IN Dee_uint8_t seq_concat_op,
 DEE_A_IN Dee_uint8_t seq_cast_op, DEE_A_IN DeeTypeObject *seq_type,
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeXAstObject *const *elemv,
 DEE_COMPILER_PARAMS) {
 DeeXAstObject **iter,**end,*elem,*expand_ast;
 int is_expand_construct; Dee_size_t active_seq_size;
 // Compile all sequence elements
 end = (iter = (DeeXAstObject **)elemv)+elemc;
 if (!ret_used) {
  // Don't create the sequence when it isn't even used
  while (iter != end) {
   elem = *iter++;
   if (elem->ast_kind == DEE_XASTKIND_EXPAND) elem = elem->ast_operator.op_a;
   if DEE_UNLIKELY(DeeXAst_Compile(elem,DEE_COMPILER_ARGS) != 0) return -1;
  }
  return 0;
 }
 // Empty sequences are simple
 if (!elemc) return _DeeCodeWriter_MakeSequence(writer,seq_op,0);
 is_expand_construct = 0;
 active_seq_size = 0;
 do {
  elem = *iter++;
  if (elem->ast_kind == DEE_XASTKIND_EXPAND) {
   expand_ast = elem->ast_operator.op_a;
   // Pack everything we've got so far
   if (active_seq_size && DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,seq_op,active_seq_size) != 0)) return -1;
   if (is_expand_construct && DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,seq_concat_op) != 0)) return -1;
   // Compile the new portion that should be expanded
   if DEE_UNLIKELY(DeeXAst_Compile(expand_ast,DEE_COMPILER_ARGS) != 0) return -1;
   if (active_seq_size) {
    // Concat the two portions
    if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,seq_concat_op) != 0) return -1;
   } else {
    DeeTypeObject const *real_seq_type;
    // This is the first portion (make sure it's of the correct typing)
    // NOTE: We only have to do this if we can't predict its type.
    real_seq_type = DeeXAst_PredictType(expand_ast);
    if ((!real_seq_type || real_seq_type != seq_type) &&
        DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,seq_cast_op) != 0)) return -1;
   }
   is_expand_construct = 1;
   active_seq_size = 0;
  } else {
   if DEE_UNLIKELY(DeeXAst_Compile(elem,DEE_COMPILER_ARGS) != 0) return -1;
   ++active_seq_size;
  }
 } while (iter != end);
 if (active_seq_size) {
  if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,seq_op,active_seq_size) != 0) return -1;
  // Must still cat the last section with the previous expand portion
  if (is_expand_construct && DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,seq_concat_op) != 0)) return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileDict(
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) struct DeeXAstDictEntry const *elemv, DEE_COMPILER_PARAMS) {
 struct DeeXAstDictEntry const *iter,*end;
 if (elemc) {
  end = (iter = elemv)+elemc;
  while (iter != end) { // Simply compile all key/item pairs
   if DEE_UNLIKELY(DeeXAst_Compile(iter->de_key,DEE_COMPILER_ARGS) != 0 ||
                   DeeXAst_Compile(iter->de_item,DEE_COMPILER_ARGS) != 0) return -1;
   ++iter;
  }
  if (ret_used) {
   // If the result is used, write the dict opcode
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_DICT,elemc) != 0) return -1;
   DeeCodeWriter_DECSTACK_N(writer,elemc*2); // Consumed by OP_DICT
   DeeCodeWriter_INCSTACK(writer); // Created by OP_DICT
  }
 } else if (ret_used) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_DICT) != 0) return -1;
  DeeCodeWriter_INCSTACK(writer); // Created by OPEXT_EMPTY_DICT
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeXAst_CompileFunctionGenerateReturnCode(
 DEE_A_INOUT struct DeeSAstObject *code, DEE_A_INOUT DeeScopeObject *scope,
 DEE_A_IN Dee_uint32_t function_flags, DEE_A_IN Dee_uint32_t scope_flags,
 DEE_A_IN Dee_uint32_t compiler_flags, DEE_A_INOUT struct DeeCodeWriter *writer) {
 DEE_ASSERT(DeeObject_Check(code) && DeeSAst_Check(code));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 if (!DeeSAst_IsNoReturn(code,DEE_AST_ATTRIBUTE_FLAG_NONE)) {
  DEE_ASSERT(writer->cw_last_op != OP_YIELDSTOP && writer->cw_last_op != OP_RET &&
             writer->cw_last_op != OP_RETNONE && writer->cw_last_op != OP_RETVAREXIT &&
             "Returning statement generated noreturn opcode");
  if ((function_flags&DEE_FUNCTION_FLAG_YILD)!=0) {
   // Put a missing yield exit statement, to stop iteration
   if DEE_UNLIKELY(DeeCodeWriter_YieldExit(writer) != 0) return -1;
  } else if ((scope_flags&DEE_SCOPE_FLAG_FOUND_RETVAR) != 0) {
   // Put a missing return statement, to return the stored result variable
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RETVAREXIT) != 0) return -1;
  } else {
   // Put a missing return statement, to return none by default
   if DEE_UNLIKELY(DeeCodeWriter_RetNone(writer) != 0) return -1;
  }
 } else if ((compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0) {
  if DEE_UNLIKELY(DeeCodeWriter_Unreachable(writer) != 0) return -1;
 }
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileNewClass(
 DEE_A_IN_OPT DeeXAstObject *class_base, DEE_A_IN_OPT DeeStringObject *class_name,
 DEE_A_IN Dee_uint32_t more_type_flags, DEE_A_IN_OPT struct DeeUUID const *uuid, DEE_COMPILER_PARAMS) {
 Dee_uint16_t flags_word;
 if DEE_UNLIKELY((class_base // Push the class base
  ? DeeXAst_Compile(class_base,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
  : DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)&DeeObject_Type,compiler_flags)) != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer, // Push the class name
  class_name ? (DeeObject *)class_name : Dee_EmptyString,compiler_flags) != 0) return -1;

 // Create the class
 if (uuid) {
  DeeObject *uuid_str; int temp;
  if DEE_UNLIKELY((uuid_str = DeeUUID_ToStringEx(uuid,
   DEE_UUID_TOSTRING_FLAG_NOBRACE|DEE_UUID_TOSTRING_FLAG_LOWER)) == NULL) return -1;
  temp = DeeCodeWriter_PushConstNoCopy(writer,uuid_str,compiler_flags);
  Dee_DECREF(uuid_str);
  if DEE_UNLIKELY(temp != 0) return temp;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_CLASS_NEW_UUID) != 0) return -1;
  DeeCodeWriter_DECSTACK_N(writer,2); // -3 +1
 } else {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_CLASS_NEW) != 0) return -1;
  DeeCodeWriter_DECSTACK(writer); // -2 +1
 }
 // Apply type flags
 more_type_flags &= DEE_TYPE_FLAG_PUBLIC_MASK;
 flags_word = (Dee_uint16_t)(more_type_flags&DEE_UINT32_C(0x0000FFFF));
 if (flags_word && DEE_UNLIKELY(
  DeeCodeWriter_WriteOpWithArg(writer,OP_PREFIX,flags_word) != 0 ||
  DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_TYPEFLAGS_LO) != 0)) return -1;
 flags_word = (Dee_uint16_t)((more_type_flags&DEE_UINT32_C(0xFFFF0000)) >> 16);
 if (flags_word && DEE_UNLIKELY(
  DeeCodeWriter_WriteOpWithArg(writer,OP_PREFIX,flags_word) != 0 ||
  DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_TYPEFLAGS_HI) != 0)) return -1;
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int DeeXAst_Compile(
 DEE_A_INOUT DeeXAstObject *self, DEE_COMPILER_PARAMS) {
 Dee_uint8_t opcode;
 Dee_uint16_t oparg;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
#if 1
 if ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_EXPR)!=0 &&
     (compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) {
  // Only put debug information if the expression isn't noexcept or has side-effects
  if ((DeeXAst_Attr(self,DEE_AST_ATTRIBUTE_FLAG_NONE)&
   (DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT)) ==
   (DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT)) goto no_debug;
/*put_debug:*/
  // Put some debug information
  if DEE_UNLIKELY(DeeCodeWriter_Debug(writer,
   (compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE) != 0 ? DeeToken_FILE(self->ast_common.ast_token) : NULL,
   (compiler_flags&DEE_COMPILER_FLAG_DEBUG_LINE) != 0 ? DeeToken_LINE(self->ast_common.ast_token) : 0
   ) != 0) return -1;
 }
no_debug:
#endif
 switch (self->ast_kind) {
  case DEE_XASTKIND_CONST:
   if (!ret_used) return 0;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
   if DEE_UNLIKELY(DeeXAst_Check(self->ast_const.c_const) ||
                   DeeSAst_Check(self->ast_const.c_const)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_LOAD_AST_AT_RUNTIME,
     (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
     "Cannot load xast/sast objects at runtime") != 0) return -1;
   } else
#endif
   {
    return DeeCodeWriter_PushConst(writer,self->ast_const.c_const,compiler_flags);
   }
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
   DEE_ATTRIBUTE_FALLTHROUGH
#endif
  case DEE_XASTKIND_VAR:
   if (!ret_used) return 0;
  {
   if DEE_UNLIKELY(!DeeLocalVar_IS_COMPILERINIT(self->ast_var.vs_var)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNINITIALIZED_VARIABLE_IN_LOAD,
     (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
     "Cannot load uninitialized variable") != 0) return -1;
push_none: // Must recheck because of the label
    return ret_used ? DeeCodeWriter_PushNone(writer) : 0;
   }
   return DeeCodeWriter_LoadVar(writer,self->ast_var.vs_var,lexer);
  } break;

  case DEE_XASTKIND_VARDECL: {
   DEE_ASSERT(DeeObject_Check(self->ast_vardecl.vd_init) && DeeXAst_Check(self->ast_vardecl.vd_init));
#if 0 // There's nothing wrong with this, but this is the optimizer's job...
   // Check if the variable should be initialized
   // >> If not, simply compile the initializer with the ret_used parameter of the caller
   if (!DeeLocalVar_ShouldInitialize(self->ast_vardecl.vd_var,self->ast_vardecl.vd_init)) {
    DEE_LVERBOSE1R("%s(%d) : COMPILER : %k : Skipping initialization of unused variable\n",
                   DeeToken_FILE(self->ast_vardecl.vd_token),
                   DeeToken_LINE(self->ast_vardecl.vd_token)+1,
                   self->ast_vardecl.vd_token);
    return DeeXAst_Compile(self->ast_vardecl.vd_init,DEE_COMPILER_ARGS);
   }
#endif
   // Special handling for allowing objects to reference themself during their creation
   // e.g.: A function calling itself, or a list containing itself
   // Note though, that this will only work with a select few of intrinsic object types
   // and that doing so results in a somewhat performance loss.
   switch (self->ast_vardecl.vd_init->ast_kind) {

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
    case DEE_XASTKIND_CLASS: {
     struct DeeXAstClassAstEntry *iter,*end;
     DeeXAstObject *class_ast = self->ast_vardecl.vd_init;
     // Since there is no difference in the generated code when it
     // comes to classes, we simply always compile them with self-reference support
     if DEE_UNLIKELY(DeeXAst_CompileNewClass(class_ast->ast_class.c_base,class_ast->ast_class.c_name,
      class_ast->ast_class.c_tpflags,(class_ast->ast_class.c_tpflags&DEE_XAST_CLASSAST_FLAG_HAS_UUID)!=0
      ? &class_ast->ast_class.c_uuid : NULL,DEE_COMPILER_ARGS) != 0) return -1;
     // Store the new class in its variable
     if DEE_UNLIKELY(DeeCodeWriter_StoreVar(writer,self->ast_vardecl.vd_var,
      have_local_names ? lexer : NULL) != 0) return -1;
     end = (iter = class_ast->ast_class.c_entryv)+class_ast->ast_class.c_entryc;
     while (iter != end) {
      if DEE_UNLIKELY(DeeXAst_CompileClassEntry(iter,DEE_COMPILER_ARGS) != 0) return -1;
      ++iter;
     }
     goto pop_if_unused_and_end;
    } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

    case DEE_XASTKIND_FUNCTION: {
     // If the function doesn't use itself, compile it normally
     // NOTE: If it does use itself, code is geneated similar to the sequences below...
     if DEE_UNLIKELY(!DeeXAst_UsesVariable(self->ast_vardecl.vd_init,
      self->ast_vardecl.vd_var)) goto compile_default_vardecl;
     DEE_LVERBOSE1R("%s(%d) : Detected self-referencing function %q\n",
                    DeeToken_FILE(self->ast_vardecl.vd_token),
                    DeeToken_LINE(self->ast_vardecl.vd_token)+1,
                    TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),
                    self->ast_vardecl.vd_var->lv_name));
     // If the function uses itself, create it beforehand
     if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyTuple,compiler_flags) != 0 ||
                     DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyCode,compiler_flags) != 0 ||
                     DeeCodeWriter_WriteOpWithArg(writer,OP_FUNCTION,0) != 0) return -1;
     // Allow for functions to reference themselves
     DeeCodeWriter_DECSTACK(writer); // -2 +1
     // v store the function in the variable (allows the real references to access it)
     //   NOTE: But keep the variable on the stack to we don't have to reload it for assignment
     if DEE_UNLIKELY(DeeCodeWriter_StoreVar(writer,self->ast_vardecl.vd_var,
      have_local_names ? lexer : NULL) != 0) return -1;
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_vardecl.vd_init,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     // v assign the newly generated function to its variable
     if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_COPY_ASSIGN) != 0) return -1;
     // v Pop the function variable if it isn't used
pop_if_unused_and_end:
     if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
    } break;

    // Note how 'tuple' isn't in here. (That is because a
    // recursive tuple is illegal for multiple reasons)
    case DEE_XASTKIND_SET:
    case DEE_XASTKIND_CELL:
    case DEE_XASTKIND_LIST:
    case DEE_XASTKIND_DICT: {
     // If the initializer doesn't use the storage variable,
     // we don't need to generate code that allows for self-referencing.
     // NOTE: The self-referencing code is slower and looks like this:
     // >> l = list { 10,l,30 };
     // Compiled as:
     // >> l = list();
     // >> l.operator move = ([10,l,30]);
     if DEE_LIKELY(!DeeXAst_UsesVariable(
      self->ast_vardecl.vd_init,
      self->ast_vardecl.vd_var)) goto compile_default_vardecl;
     DEE_LVERBOSE1R("%s(%d) : Detected self-referencing sequence %q\n",
                    DeeToken_FILE(self->ast_vardecl.vd_token),
                    DeeToken_LINE(self->ast_vardecl.vd_token)+1,
                    TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),
                    self->ast_vardecl.vd_var->lv_name));
     // Create an empty sequence before the initializer is
     // invoked, then assign the initializer sequence to it.
     switch (self->ast_vardecl.vd_init->ast_kind) {
      case DEE_XASTKIND_SET:  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_SET) != 0) return -1; break;
      case DEE_XASTKIND_CELL: if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_CELL) != 0) return -1; break;
      case DEE_XASTKIND_LIST: if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_LIST) != 0) return -1; break;
      case DEE_XASTKIND_DICT: if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_DICT) != 0) return -1; break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     DeeCodeWriter_INCSTACK(writer);
     // NOTE: For better performance, we keep the actual sequence on the stack
     if DEE_UNLIKELY(DeeCodeWriter_StoreVar(
      writer,self->ast_vardecl.vd_var,have_local_names ? lexer : NULL) != 0) return -1;
     // An empty sequence is now stored in the variable.
     // -> Now we can generate the actual sequence
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_vardecl.vd_init,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     // Now simply assign the sequence by invoking OP_MOVE_ASSIGN
     // Note how this is actually a situation where 'move:=' has purpose (yay!)
     if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_MOVE_ASSIGN) != 0) return -1;
     // todo: We could optimize this by adding static OPEXT_ASSIGN_SET, OPEXT_ASSIGN_CELL, ... opcodes
     goto pop_if_unused_and_end;
    } break;

    default:
compile_default_vardecl:
    {
     DeeLocalVarObject *init_var = self->ast_vardecl.vd_var;
     if (DeeLocalVar_KIND(init_var) == DEE_LOCALVAR_KIND_STATIC) {
      // Compile a static variable initializer
      if (!DeeLocalVar_IS_COMPILERINIT(init_var) &&
          self->ast_vardecl.vd_init->ast_kind == DEE_XASTKIND_CONST) {
       // Allocate the static variable with its initializer
       if DEE_UNLIKELY((init_var->lv_loc_id = DeeCodeWriter_AllocStatic(
        writer,self->ast_vardecl.vd_init->ast_const.c_const,have_local_names ?
        TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),init_var->lv_name) : NULL)) == (Dee_size_t)-1) return -1;
       init_var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
      } else {
       if DEE_UNLIKELY((init_var->lv_loc_id = DeeCodeWriter_AllocStatic(writer,Dee_None,have_local_names ?
        TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),init_var->lv_name) : NULL)) == (Dee_size_t)-1) return -1;
       init_var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
       { // The static initializer may only be executed once.
        struct DeeCodeWriterAtomicOnceBlock ao_block;
        if DEE_UNLIKELY(DeeCodeWriter_AtomicOnceBegin(writer,&ao_block,(Dee_uint32_t)(
         DeeXAst_IsReallyNoExcept(self->ast_vardecl.vd_init)
         ? DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT
         : DEE_CODEWRITER_ATOMICONCE_FLAG_NONE),compiler_flags) != 0) return -1;
        // Compile the initializer
        if DEE_UNLIKELY(DeeXAst_Compile(self->ast_vardecl.vd_init,
         DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
        // Store its value in the static variable
        if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
         writer,OP_STORE_CST_POP,init_var->lv_loc_id) != 0) return -1;
        DeeCodeWriter_DECSTACK(writer);
        if DEE_UNLIKELY(DeeCodeWriter_AtomicOnceEnd(
         writer,&ao_block,compiler_flags) != 0) return -1;
       }
      }
      // Push the static variable if it is used
      if (ret_used && DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
       writer,OP_LOAD_CST_LOCKED,init_var->lv_loc_id) != 0)) return -1;
      break;
     }
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_vardecl.vd_init,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY((ret_used ? DeeCodeWriter_StoreVar   (writer,init_var,have_local_names ? lexer : NULL)
                               : DeeCodeWriter_StoreVarPop(writer,init_var,have_local_names ? lexer : NULL)
                     ) != 0) return -1;
    } break;
   }
  } break;

  case DEE_XASTKIND_MULTIVARDECL: {
   if (DeeXAst_CompileMultiVarDecl(self->ast_multivardecl.mvd_varc,
    self->ast_multivardecl.mvd_varv,self->ast_multivardecl.mvd_init,
    DEE_COMPILER_ARGS) != 0) return -1;
   // Pack the list of variables into a sequence
   if (ret_used) switch (self->ast_multivardecl.mvd_type) {
    case DEE_XASTKIND_SET:  if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,OP_SET,self->ast_multivardecl.mvd_varc) != 0) return -1; break;
    case DEE_XASTKIND_LIST: if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,OP_LIST,self->ast_multivardecl.mvd_varc) != 0) return -1; break;
    default:                if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,self->ast_multivardecl.mvd_varc) != 0) return -1; break;
   }
  } break;

  case DEE_XASTKIND_STATEMENT: {
   return DeeSAst_Compile(self->ast_statement.s_stmt,DEE_COMPILER_ARGS);
  } break;

  case DEE_XASTKIND_IF: {
   Dee_size_t succ_pos,succ_jmparg;
   Dee_size_t fail_pos,fail_jmparg;
   // Compile the condition code
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_if.if_cond,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   succ_pos = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,(Dee_uint8_t)(
    self->ast_if.if_succ ? OP_JUMP_IF_FF_POP : OP_JUMP_IF_FF),&succ_jmparg) != 0) return -1;
   if (self->ast_if.if_succ) {
    DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
    // Write the true branch
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_if.if_succ,DEE_COMPILER_ARGS) != 0) return -1;
   }
   fail_pos = DeeCodeWriter_ADDR(writer); // setup the jump if the check succeeds
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&fail_jmparg) != 0) return -1;
   // if false, we jump after the jump instruction that skips the false branch
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,succ_jmparg,
    (Dee_size_t)(DeeCodeWriter_ADDR(writer)-succ_pos)) != 0) return -1;
   // Write the false branch
   if (self->ast_if.if_fail) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_if.if_fail,DEE_COMPILER_ARGS) != 0) return -1;
   } else if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,fail_jmparg,
    (Dee_size_t)(DeeCodeWriter_ADDR(writer)-fail_pos)) != 0) return -1;
   if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch remains on the stack
  } break;

  case DEE_XASTKIND_FUNCTION:
   if DEE_UNLIKELY(!ret_used) return 0;
  {
   DeeScopeObject *fun_scope;
   if DEE_LIKELY((fun_scope = self->ast_function.f_scope) != NULL) {
    DeeCodeObject *code;
    Dee_size_t argc = self->ast_function.f_argc;
    struct DeeCodeWriter fun_writer = DeeCodeWriter_INIT();
    fun_writer.cw_root_scope = fun_scope;
    DEE_ASSERT(DeeObject_Check(fun_scope) && DeeScope_Check(fun_scope));
    DEE_ASSERT(((fun_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0) ==
               ((self->ast_function.f_flags&DEE_FUNCTION_FLAG_THIS)!=0));
    DEE_ASSERT(((fun_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) ==
               ((self->ast_function.f_flags&DEE_FUNCTION_FLAG_YILD)!=0));
    if ((fun_scope->sc_flags&DEE_SCOPE_FLAG_CODE_COPYABLE)!=0)
     fun_writer.cw_flags |= DEE_CODE_FLAG_COPY; // Enable the copyable bit
    if DEE_UNLIKELY(DeeSAst_CompileStrongScopeRoot(self->ast_function.f_code,&fun_writer,
     lexer,self->ast_function.f_scope,config,compiler_flags&~(
     DEE_COMPILER_FLAG_USED|DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)
     ) != 0) goto err_function_1;
    if DEE_UNLIKELY(_DeeXAst_CompileFunctionGenerateReturnCode(
     self->ast_function.f_code,self->ast_function.f_scope,
     self->ast_function.f_flags,self->ast_function.f_scope->sc_flags,
     compiler_flags,&fun_writer) != 0) goto err_function_1;
    if DEE_UNLIKELY(DeeCodeWriter_Optimize(&fun_writer,compiler_flags) != 0) goto err_function_1;
    if DEE_UNLIKELY((code = (DeeCodeObject *)DeeCodeWriter_Pack(&fun_writer,(DeeObject *)(
     (compiler_flags&DEE_COMPILER_FLAG_DEBUG_FUNC)!=0 ? self->ast_function.f_name : NULL
     ))) == NULL) goto err_function_1; // Pack the code
    if (fun_writer.cw_refc) {
     // Compile the list of all variables referenced by the function
     DeeLocalVarObject **ref_iter,**ref_end;
     ref_end = (ref_iter = fun_writer.cw_refv)+fun_writer.cw_refc;
     // Recursively load variables, thus allowing for long references
     while (ref_iter != ref_end) {
      if DEE_UNLIKELY(DeeCodeWriter_LoadVar(
       writer,*ref_iter,have_local_names ? lexer : NULL) != 0) goto err_function_2;
      ++ref_iter;
     }
     // Pack all references variable references into a tuple
     if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,fun_writer.cw_refc) != 0) goto err_function_2;
    } else if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyTuple,compiler_flags) != 0) goto err_function_2; // empty refs
    if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)code,compiler_flags) != 0)  goto err_function_2; // Push the code
    Dee_DECREF(code);
    // Write the function flag prefix
    if DEE_UNLIKELY(self->ast_function.f_flags && DeeCodeWriter_WriteOpWithArg(writer,
     OP_PREFIX,(Dee_uint16_t)self->ast_function.f_flags) != 0) goto err_function_1;
    // Write the actual function opcode
    //if (argc > 0xFFFF) {
    // // TODO: Error
    //}
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,
     OP_FUNCTION,(Dee_uint16_t)argc) != 0) goto err_function_1;
    DeeCodeWriter_DECSTACK(writer); // consumed by the function generator
    DeeCodeWriter_Quit(&fun_writer);
    break;
err_function_2: Dee_DECREF(code);
err_function_1: DeeCodeWriter_Quit(&fun_writer);
    return -1;
   } else {
    // Empty function
    if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyTuple,compiler_flags) != 0 ||
                    DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyCode,compiler_flags) != 0 ||
                    DeeCodeWriter_WriteOpWithArg(writer,OP_FUNCTION,0) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -2 +1
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:
   if (!ret_used) return 0;
  {
   struct DeeXAstClassAstEntry *iter,*end;
   if DEE_UNLIKELY(DeeXAst_CompileNewClass(self->ast_class.c_base,self->ast_class.c_name,
    self->ast_class.c_tpflags,(self->ast_class.c_tpflags&DEE_XAST_CLASSAST_FLAG_HAS_UUID)!=0
    ? &self->ast_class.c_uuid : NULL,DEE_COMPILER_ARGS) != 0) return -1;
   // Store the new class in its variable
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   while (iter != end) {
    if DEE_UNLIKELY(DeeXAst_CompileClassEntry(iter,DEE_COMPILER_ARGS) != 0) return -1;
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
   DEE_ASSERT(self->ast_sequence.s_elemv != NULL);
   DEE_ASSERT(self->ast_sequence.s_elemc != 0);
   return DeeXAst_CompileSequence(OP_TUPLE,OP_CONCAT_TUPLE,OP_CAST_TUPLE,&DeeTuple_Type,
                                  self->ast_sequence.s_elemc,self->ast_sequence.s_elemv,
                                  DEE_COMPILER_ARGS);
  case DEE_XASTKIND_LIST:
   DEE_ASSERT(self->ast_sequence.s_elemv != NULL);
   DEE_ASSERT(self->ast_sequence.s_elemc != 0);
   return DeeXAst_CompileSequence(OP_LIST,OP_CONCAT_LIST,OP_CAST_LIST,&DeeList_Type,
                                  self->ast_sequence.s_elemc,self->ast_sequence.s_elemv,
                                  DEE_COMPILER_ARGS);
  case DEE_XASTKIND_SET:
   DEE_ASSERT(self->ast_sequence.s_elemv != NULL);
   DEE_ASSERT(self->ast_sequence.s_elemc != 0);
   return DeeXAst_CompileSequence(OP_SET,OP_INPLACE_ADD,OP_CAST_SET,&DeeSet_Type,
                                  self->ast_sequence.s_elemc,self->ast_sequence.s_elemv,
                                  DEE_COMPILER_ARGS);

  case DEE_XASTKIND_DICT:
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   return DeeXAst_CompileDict(self->ast_dict.d_elemc,self->ast_dict.d_elemv,
                              DEE_COMPILER_ARGS);

  case DEE_XASTKIND_SEQ_RANGE_GET:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_seq_range_get.sr_seq,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_begin
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_begin,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_end
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_end,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_TrinaryOp(writer,OP_SEQ_RANGE_GET) != 0) return -1;
   goto pop_if_unused_and_end;

  case DEE_XASTKIND_SEQ_RANGE_DEL:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_seq_range_get.sr_seq,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_begin
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_begin,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_end
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_end,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if (DeeCodeWriter_WriteOp(writer,OP_SEQ_RANGE_DEL) != 0) return -1;
   DeeCodeWriter_DECSTACK_N(writer,3); // consumed by 'OP_SEQ_RANGE_DEL'
   goto push_none;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_seq_range_get.sr_seq,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_begin
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_begin,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY((self->ast_seq_range_get.sr_end
    ? DeeXAst_Compile(self->ast_seq_range_get.sr_end,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
    : DeeCodeWriter_PushNone(writer)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_seq_range_get.sr_value,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_QuadaryOp(writer,OP_SEQ_RANGE_SET) != 0) return -1;
   goto pop_if_unused_and_end;

  case DEE_XASTKIND_ATTR_GET_C: opcode = OP_ATTR_GET_C; goto writer_attr_operator;
  case DEE_XASTKIND_ATTR_HAS_C: opcode = OP_ATTR_HAS_C; goto writer_attr_operator;
  case DEE_XASTKIND_ATTR_DEL_C: opcode = OP_ATTR_DEL_C;
writer_attr_operator: {
   Dee_size_t attr_id;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_attr_c.ac_object,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((attr_id = DeeCodeWriter_AllocConst(writer,(
    DeeObject *)self->ast_attr_c.ac_name,compiler_flags)) == (Dee_size_t)-1) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,opcode,attr_id) != 0) return -1;
   if (opcode == OP_ATTR_DEL_C) {
    DeeCodeWriter_DECSTACK(writer); // Consumed
    if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   } else {
    goto pop_if_unused_and_end;
   }
  } break;
  case DEE_XASTKIND_ATTR_SET_C: {
   Dee_size_t attr_id;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_attr_set_c.ac_object,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY((attr_id = DeeCodeWriter_AllocConst(writer,(
    DeeObject *)self->ast_attr_set_c.ac_name,compiler_flags)) == (Dee_size_t)-1) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_attr_set_c.ac_value,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_ATTR_SET_C,attr_id) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer); // from OP_ATTR_SET_C: -2 +1
   goto pop_if_unused_and_end;
  } break;

  case DEE_XASTKIND_EXPAND: {
   DeeTypeObject const *unpack_type;
   unpack_type = DeeXAst_PredictType(self->ast_operator.op_a);
   if (!ret_used) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
     compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_SEQ_ITERALL) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer);
   } else {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
     compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    // Expand the init expression
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint8_t)(
     unpack_type == &DeeTuple_Type ? OP_UNPACK_TUPLE :
     unpack_type == &DeeList_Type ? OP_UNPACK_LIST :
     OP_UNPACK_SEQUENCE),1) != 0) return -1;
    goto pop_if_unused_and_end;
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX || \
    DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  {
   DeeObject *builtin_function;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
   if (0) { case DEE_XASTKIND_BUILTIN_DEX: builtin_function = (DeeObject *)&DeeBuiltinFunction___builtin_dex; }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX */
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   if (0) { case DEE_XASTKIND_PTROF: builtin_function = (DeeObject *)&DeeBuiltinFunction___builtin_pointer_add; }
   if (0) { case DEE_XASTKIND_LVALOF: builtin_function = (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add; }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,builtin_function,compiler_flags) != 0)) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS) != 0) return -1;
   if (ret_used) {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_TUPLE,1) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
   }
   break;
  }
#endif /* ... */

  case DEE_XASTKIND_STR: opcode = OP_STR; goto unary_operator;
  case DEE_XASTKIND_REPR: opcode = OP_REPR; goto unary_operator;
  case DEE_XASTKIND_COPY: opcode = OP_COPY; goto unary_operator;
  case DEE_XASTKIND_MOVE: opcode = OP_MOVE; goto unary_operator;
  case DEE_XASTKIND_TYPE: opcode = OP_TYPE; goto unary_operator;
  case DEE_XASTKIND_WEAK: opcode = OP_WEAK; goto unary_operator;
  case DEE_XASTKIND_BOOL: opcode = OP_BOOL; goto unary_operator;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XASTKIND_REF: opcode = OP_REF; goto unary_operator;
  case DEE_XASTKIND_DEREF: opcode = OP_DEREF; goto unary_operator;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DEE_XASTKIND_NOT: opcode = OP_NOT; goto unary_operator;
  case DEE_XASTKIND_NEG: opcode = OP_NEG; goto unary_operator;
  case DEE_XASTKIND_POS: opcode = OP_POS; goto unary_operator;
  case DEE_XASTKIND_INV: opcode = OP_INV; goto unary_operator;
  case DEE_XASTKIND_SEQ_SIZE: opcode = OP_SEQ_SIZE; goto unary_operator;
  case DEE_XASTKIND_ITERSELF: opcode = OP_SEQ_ITER_SELF; goto unary_operator;
  case DEE_XASTKIND_CELL: opcode = OP_CELL;
unary_operator:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,opcode) != 0) return -1;
   goto pop_if_unused_and_end;

  // TODO
  //case DEE_XASTKIND_NEW: oparg = ...; goto unary_operator_ext;
  //case DEE_XASTKIND_DELETE: oparg = ...; goto unary_operator_ext;
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  case DEE_XASTKIND_VARRAYOF: oparg = OPEXT_VARRAYOF; goto unary_operator_ext;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
  case DEE_XASTKIND_ITERNEXT: oparg = OPEXT_SEQ_ITER_NEXT; goto unary_operator_ext;
  case DEE_XASTKIND_SEQ_ANY: oparg = OPEXT_SEQ_ANY; goto unary_operator_ext;
  case DEE_XASTKIND_SEQ_ALL: oparg = OPEXT_SEQ_ALL; goto unary_operator_ext;
  case DEE_XASTKIND_SEQ_SUM: oparg = OPEXT_SEQ_SUM; goto unary_operator_ext;
  case DEE_XASTKIND_SUPEROF: oparg = OPEXT_SUPEROF; goto unary_operator_ext;
  case DEE_XASTKIND_CLASSOF: oparg = OPEXT_CLASSOF;
unary_operator_ext:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,oparg) != 0) return -1;
   goto pop_if_unused_and_end;


  case DEE_XASTKIND_LAND:
  case DEE_XASTKIND_LOR: {
   Dee_size_t succ_pos,succ_jmparg;
   // Compile the lhs AST
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_BOOL) != 0)) return -1; // cast lhs to bool
   succ_pos = DeeCodeWriter_ADDR(writer); // Jump over the rhs AST if we know it'll result in false
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,(Dee_uint8_t)(
    self->ast_kind == DEE_XASTKIND_LAND
    ? (ret_used ? OP_JUMP_IF_FF : OP_JUMP_IF_FF_POP) // a && b (skip b if a is false)
    : (ret_used ? OP_JUMP_IF_TT : OP_JUMP_IF_TT_POP) // a || b (skip b if a is true)
    ),&succ_jmparg) != 0) return -1;
   if (!ret_used) {
    // 'OP_JUMP_IF_FF_POP' / 'OP_JUMP_IF_TT_POP' popped this one
    DeeCodeWriter_DECSTACK(writer);
   } else {
    // pop lhs if we generated a value
    if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1;
   }
   // compile the rhs AST
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS) != 0) return -1;
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_BOOL) != 0)) return -1; // cast rhs to bool if it's used
   // This is where we jump if rhs was skipped
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,succ_jmparg,(
    Dee_size_t)(DeeCodeWriter_ADDR(writer)-succ_pos)) != 0) return -1;
  } break;

  case DEE_XASTKIND_LXOR: {
   if (ret_used) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS) != 0 ||
                    DeeCodeWriter_UnaryOp(writer,OP_BOOL) != 0 ||
                    DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS) != 0 ||
                    DeeCodeWriter_UnaryOp(writer,OP_BOOL) != 0 ||
                    DeeCodeWriter_BinaryOp(writer,OP_XOR) != 0) return -1;
   } else {
    // NOTE: Technically we'd have to call the bool operator here... But what the hell!
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS) != 0 ||
                    DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS) != 0) return -1;
   }
  } break;

#if defined(OP_NOT) && (defined(OP_IS) || defined(OP_IN))
#ifdef OP_IS
  case DEE_XASTKIND_NOT_IS: opcode = OP_IS;
#ifdef OP_IN
   goto binary_operator_not;
#endif
#endif
#ifdef OP_IN
  case DEE_XASTKIND_NOT_IN: opcode = OP_IN;
#endif
#if defined(OP_IS) && defined(OP_IN)
binary_operator_not:
#endif
  {
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,opcode) != 0) return -1;
   if (ret_used) {
    if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_NOT) != 0) return -1;
   } else if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1;
  } break;
#endif

#if defined(OP_SEQ_DEL) || defined(OP_ATTR_DEL)
#ifdef OP_SEQ_DEL
  case DEE_XASTKIND_SEQ_DEL: opcode = OP_SEQ_DEL;
#ifdef OP_ATTR_DEL
   goto binary_operator_retnone;
#endif
#endif
#ifdef OP_ATTR_DEL
  case DEE_XASTKIND_ATTR_DEL: opcode = OP_ATTR_DEL;
#endif
#if defined(OP_SEQ_DEL) && defined(OP_ATTR_DEL)
binary_operator_retnone:
#endif
  {
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,opcode) != 0) return -1;
   DeeCodeWriter_DECSTACK_N(writer,2);
   goto push_none;
  } break;
#endif
   
  {
   Dee_uint8_t inplace_opcode; Dee_uint16_t inplace_try_opcode;
   DeeLocalVarObject *local_var; DeeXAstObject *op_a;
   DEE_STATIC_ASSERT(Dee_OFFSETOF(struct DeeXAstUnaryInplaceVarAst,uiv_var) == Dee_OFFSETOF(struct DeeXAstBinaryInplaceVarAst,biv_var));
   DEE_STATIC_ASSERT(Dee_OFFSETOF(struct DeeXAstUnaryInplaceVarAst,uiv_tok) == Dee_OFFSETOF(struct DeeXAstBinaryInplaceVarAst,biv_tok));
#define INPLACE_OP_DATA(iop,try_iop) (inplace_opcode=(iop),inplace_try_opcode=(try_iop))
   // Inplace-var unary/binary asts
   if (0) { case DEE_XASTKIND_VAR_INC:     INPLACE_OP_DATA(OP_INC,OPEXT_TRY_INPLACE_INC); }
   if (0) { case DEE_XASTKIND_VAR_DEC:     INPLACE_OP_DATA(OP_DEC,OPEXT_TRY_INPLACE_DEC); }
   if (0) { case DEE_XASTKIND_VAR_INCPOST: INPLACE_OP_DATA(OP_INC_POST,OPEXT_TRY_INPLACE_INC_POST); }
   if (0) { case DEE_XASTKIND_VAR_DECPOST: INPLACE_OP_DATA(OP_DEC_POST,OPEXT_TRY_INPLACE_DEC_POST); }
   if (0) { case DEE_XASTKIND_VAR_IADD:    INPLACE_OP_DATA(OP_INPLACE_ADD,OPEXT_TRY_INPLACE_ADD); }
   if (0) { case DEE_XASTKIND_VAR_ISUB:    INPLACE_OP_DATA(OP_INPLACE_SUB,OPEXT_TRY_INPLACE_SUB); }
   if (0) { case DEE_XASTKIND_VAR_IMUL:    INPLACE_OP_DATA(OP_INPLACE_MUL,OPEXT_TRY_INPLACE_MUL); }
   if (0) { case DEE_XASTKIND_VAR_IDIV:    INPLACE_OP_DATA(OP_INPLACE_DIV,OPEXT_TRY_INPLACE_DIV); }
   if (0) { case DEE_XASTKIND_VAR_IMOD:    INPLACE_OP_DATA(OP_INPLACE_MOD,OPEXT_TRY_INPLACE_MOD); }
   if (0) { case DEE_XASTKIND_VAR_ISHL:    INPLACE_OP_DATA(OP_INPLACE_SHL,OPEXT_TRY_INPLACE_SHL); }
   if (0) { case DEE_XASTKIND_VAR_ISHR:    INPLACE_OP_DATA(OP_INPLACE_SHR,OPEXT_TRY_INPLACE_SHR); }
   if (0) { case DEE_XASTKIND_VAR_IAND:    INPLACE_OP_DATA(OP_INPLACE_AND,OPEXT_TRY_INPLACE_AND); }
   if (0) { case DEE_XASTKIND_VAR_IOR:     INPLACE_OP_DATA(OP_INPLACE_OR, OPEXT_TRY_INPLACE_OR);  }
   if (0) { case DEE_XASTKIND_VAR_IXOR:    INPLACE_OP_DATA(OP_INPLACE_XOR,OPEXT_TRY_INPLACE_XOR); }
   if (0) { case DEE_XASTKIND_VAR_IPOW:    INPLACE_OP_DATA(OP_INPLACE_POW,OPEXT_TRY_INPLACE_POW); }
   local_var = self->ast_unary_var.uiv_var;
   DEE_ASSERTF(DeeCodeWriter_IsVarLocal(writer,local_var),
               "%s(%d) : %k : Unary inplace-var ast doesn't reference scope-local variable",
               DeeToken_FILE(self->ast_unary_var.uiv_tok),
               DeeToken_LINE(self->ast_unary_var.uiv_tok)+1,
               self->ast_unary_var.uiv_tok);
   if (!DeeCodeWriter_IsVarLocal(writer,local_var)) goto inplace_default;
   if DEE_UNLIKELY(!DeeLocalVar_IS_COMPILERINIT(local_var)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNINITIALIZED_VARIABLE_IN_LOAD,
     (DeeObject *)lexer,(DeeObject *)self->ast_unary_var.uiv_tok,
     "Cannot load uninitialized variable in inplace-var operation") != 0) return -1;
    goto push_none;
   }
   if (DeeLocalVar_KIND(local_var) == DEE_LOCALVAR_KIND_THIS) {
    if (DeeCodeWriter_LoadThis(writer) != 0) return -1;
    if (DEE_XASTKIND_ISBINARYVAR(self->ast_kind)) {
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_binary_var.biv_arg,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,inplace_opcode) != 0) return -1;
    } else {
     DEE_ASSERT(DEE_XASTKIND_ISUNARYVAR(self->ast_kind));
     if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,inplace_opcode) != 0) return -1;
    }
   } else {
    // Load the stack entry
    if DEE_UNLIKELY(DeeCodeWriter_LoadVar(writer,local_var,have_local_names ? lexer : NULL) != 0) return -1;
    if (DEE_XASTKIND_ISBINARYVAR(self->ast_kind)) {
     // Compile the right operand
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_binary_var.biv_arg,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1;
     // No alignment required for -2 +2
    } else {
     DEE_ASSERT(DEE_XASTKIND_ISUNARYVAR(self->ast_kind));
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer); // align for -1 +2
    }
    // Now store the result back into the stack variable
    if DEE_UNLIKELY(DeeCodeWriter_StoreVarPop(writer,local_var,have_local_names ? lexer : NULL) != 0) return -1;
   }
   goto pop_if_unused_and_end;

   // Inplace asts
   if (0) { case DEE_XASTKIND_IADD: INPLACE_OP_DATA(OP_INPLACE_ADD,OPEXT_TRY_INPLACE_ADD); }
   if (0) { case DEE_XASTKIND_ISUB: INPLACE_OP_DATA(OP_INPLACE_SUB,OPEXT_TRY_INPLACE_SUB); }
   if (0) { case DEE_XASTKIND_IMUL: INPLACE_OP_DATA(OP_INPLACE_MUL,OPEXT_TRY_INPLACE_MUL); }
   if (0) { case DEE_XASTKIND_IDIV: INPLACE_OP_DATA(OP_INPLACE_DIV,OPEXT_TRY_INPLACE_DIV); }
   if (0) { case DEE_XASTKIND_IMOD: INPLACE_OP_DATA(OP_INPLACE_MOD,OPEXT_TRY_INPLACE_MOD); }
   if (0) { case DEE_XASTKIND_ISHL: INPLACE_OP_DATA(OP_INPLACE_SHL,OPEXT_TRY_INPLACE_SHL); }
   if (0) { case DEE_XASTKIND_ISHR: INPLACE_OP_DATA(OP_INPLACE_SHR,OPEXT_TRY_INPLACE_SHR); }
   if (0) { case DEE_XASTKIND_IAND: INPLACE_OP_DATA(OP_INPLACE_AND,OPEXT_TRY_INPLACE_AND); }
   if (0) { case DEE_XASTKIND_IOR:  INPLACE_OP_DATA(OP_INPLACE_OR, OPEXT_TRY_INPLACE_OR);  }
   if (0) { case DEE_XASTKIND_IXOR: INPLACE_OP_DATA(OP_INPLACE_XOR,OPEXT_TRY_INPLACE_XOR); }
   if (0) { case DEE_XASTKIND_IPOW: INPLACE_OP_DATA(OP_INPLACE_POW,OPEXT_TRY_INPLACE_POW); }
   if (0) { case DEE_XASTKIND_INC:  INPLACE_OP_DATA(OP_INC,OPEXT_TRY_INPLACE_INC); }
   if (0) { case DEE_XASTKIND_DEC:  INPLACE_OP_DATA(OP_DEC,OPEXT_TRY_INPLACE_DEC); }
   if (0) { case DEE_XASTKIND_INCPOST: INPLACE_OP_DATA(OP_INC_POST,OPEXT_TRY_INPLACE_INC_POST); }
   if (0) { case DEE_XASTKIND_DECPOST: INPLACE_OP_DATA(OP_DEC_POST,OPEXT_TRY_INPLACE_DEC_POST); }
#undef INPLACE_OP_DATA
   op_a = self->ast_operator.op_a;
   switch (op_a->ast_kind) {
#if 0
    case DEE_XASTKIND_VAR: {
     local_var = op_a->ast_var.vs_var;
     if (!DeeCodeWriter_IsVarLocal(writer,local_var)) goto inplace_default;
     if DEE_UNLIKELY(!DeeLocalVar_IS_COMPILERINIT(local_var)) { self = op_a; goto var_uninit; } // psht... we're not supposed to overwrite 'self'
     if (DeeLocalVar_KIND(local_var) == DEE_LOCALVAR_KIND_THIS) goto inplace_default;
     // Load the stack entry
     if DEE_UNLIKELY(DeeCodeWriter_LoadVar(writer,local_var,have_local_names ? lexer : NULL) != 0) return -1;
     if (DEE_XASTKIND_ISBINARY(self->ast_kind)) {
      // Compile the right operand
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,
       DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1;
      // No alignment required for -2 +2
     } else {
      DEE_ASSERT(DEE_XASTKIND_ISUNARY(self->ast_kind));
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1;
      DeeCodeWriter_INCSTACK(writer); // align for -1 +2
     }
     // Now store the result back into the stack variable
     if DEE_UNLIKELY(DeeCodeWriter_StoreVarPop(writer,local_var,have_local_names ? lexer : NULL) != 0) return -1;
    } break;
#endif
    case DEE_XASTKIND_ATTR_GET_C: {
     Dee_size_t name_id;
     // Compile 'a.b += d' --> 'a.b = a.b += d'
     // Compile '++a.b' --> 'a.b = ++a.b'
     if DEE_UNLIKELY(DeeXAst_Compile(op_a->ast_attr_get_c.ac_object,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_Dup(writer) != 0) return -1; // Now: aa
     if DEE_UNLIKELY((name_id = DeeCodeWriter_AllocConst(writer,(
      DeeObject *)op_a->ast_attr_get_c.ac_name,compiler_flags)) == (Dee_size_t)-1) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_ATTR_GET_C,name_id) != 0) return -1; // now: av
     if (DEE_XASTKIND_ISBINARY(self->ast_kind)) {
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(
       compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1; // now: avx
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: arw
      // -> No alignment required for -2 +2
     } else {
      DEE_ASSERT(DEE_XASTKIND_ISUNARY(self->ast_kind));
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: arw
      DeeCodeWriter_INCSTACK(writer); // align for -1 +2
     }
     // Reminder: stack: 'arw' (a: object, r: result, w: new_attribute_value)
     // Rotate the stack from 'arw' --> 'raw'
     if DEE_UNLIKELY(DeeCodeWriter_LRot(writer,3) != 0) return -1; // now: rwa
     if DEE_UNLIKELY(DeeCodeWriter_LRot(writer,2) != 0) return -1; // now: raw
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_ATTR_SET_C,name_id) != 0) return -1;
     DeeCodeWriter_DECSTACK(writer); // Now: rt
     if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1; // now: r
    } break;
    case DEE_XASTKIND_SEQ_RANGE_GET: {
     // Compile 'a[b:c] += d' --> 'a[b:c] = a[b:c] += d'
     // Compile '++a[b:c]' --> 'a[b:c] = ++a[b:c]'
     if DEE_UNLIKELY(DeeXAst_Compile(op_a->ast_seq_range_get.sr_seq,
        DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_DUP) != 0) return -1; // now: aa
     DeeCodeWriter_INCSTACK(writer);
     if DEE_UNLIKELY((op_a->ast_seq_range_get.sr_begin
      ? DeeXAst_Compile(op_a->ast_seq_range_get.sr_begin,
        DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
      : DeeCodeWriter_PushNone(writer)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_DUP) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer); // Now: aabb
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RROT_3) != 0) return -1; // Now: abab
     if DEE_UNLIKELY((op_a->ast_seq_range_get.sr_end
      ? DeeXAst_Compile(op_a->ast_seq_range_get.sr_end,
        DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED))
      : DeeCodeWriter_PushNone(writer)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_DUP) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer); // Now: ababcc
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RROT_4) != 0) return -1; // Now: abcabc
     if DEE_UNLIKELY(DeeCodeWriter_TrinaryOp(writer,OP_SEQ_RANGE_GET) != 0) return -1; // Now: abcv
     if (DEE_XASTKIND_ISBINARY(self->ast_kind)) {
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(
       compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1; // Now: abcvd
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: abcrw
      // -> No alignment required for -2 +2
     } else {
      DEE_ASSERT(DEE_XASTKIND_ISUNARY(self->ast_kind));
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: abcrw
      DeeCodeWriter_INCSTACK(writer); // align for -1 +2
     }
     // Reminder: stack: 'abcrw' (a: seq, b: lower_bound, c: upper_bound, r: result, w: new_range_value)
     if DEE_UNLIKELY(DeeCodeWriter_RRot(writer,2) != 0) return -1; // Now: abcwr
     if DEE_UNLIKELY(DeeCodeWriter_RRot(writer,5) != 0) return -1; // Now: rabcw
     if DEE_UNLIKELY(DeeCodeWriter_QuadaryOp(writer,OP_SEQ_RANGE_SET) != 0) return -1; // Now: rw
     if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1; // now: r
    } break;
    case DEE_XASTKIND_SEQ_GET:
    case DEE_XASTKIND_ATTR_GET: {
     // Compile 'a[b] += c' --> 'a[b] = a[b]+c'
     // Compile '++a[b]' --> 'a[b] = ++a[b]'
     if DEE_UNLIKELY(DeeXAst_Compile(op_a->ast_operator.op_a,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_DUP) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer); // now: aa
     if DEE_UNLIKELY(DeeXAst_Compile(op_a->ast_operator.op_b,
      DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_DUP) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer); // Now: aabb
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RROT_3) != 0) return -1; // Now: abab
     if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,(Dee_uint8_t)(
      op_a->ast_kind == DEE_XASTKIND_SEQ_GET ? OP_SEQ_GET : OP_ATTR_GET)) != 0) return -1; // Now: abv
     if (DEE_XASTKIND_ISBINARY(self->ast_kind)) {
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(
       compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1; // Now: abvc
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: abrw
     } else {
      DEE_ASSERT(DEE_XASTKIND_ISUNARY(self->ast_kind));
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,inplace_try_opcode) != 0) return -1; // now: abrw
      DeeCodeWriter_INCSTACK(writer); // align for -1 +2
     }
     // Reminder: stack: 'abrw' (a: seq, b: index/key, r: result, w: new_value)
     if DEE_UNLIKELY(DeeCodeWriter_RRot(writer,2) != 0) return -1; // Now: abwr
     if DEE_UNLIKELY(DeeCodeWriter_RRot(writer,4) != 0) return -1; // Now: rabw
     if DEE_UNLIKELY(DeeCodeWriter_TrinaryOp(writer,(Dee_uint8_t)(
      op_a->ast_kind == DEE_XASTKIND_SEQ_GET ? OP_SEQ_SET : OP_ATTR_SET)) != 0) return -1; // Now: rw
     if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1; // now: r
    } break;
    default:
inplace_default:
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if (DEE_XASTKIND_ISBINARY(self->ast_kind)) {
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,inplace_opcode) != 0) return -1;
     } else {
      DEE_ASSERT(DEE_XASTKIND_ISUNARY(self->ast_kind));
      if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,inplace_opcode) != 0) return -1;
     }
     break;
   }
   goto pop_if_unused_and_end;
#undef inplace_cst_opcode
  } break;

  case DEE_XASTKIND_CALL: {
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_ATTR_GET_C) {
    Dee_size_t attr_id;
    // Compile the called attribute base
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a->ast_attr_get_c.ac_object,
     DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY((attr_id = DeeCodeWriter_AllocConst(writer,(
     DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
     compiler_flags)) == (Dee_size_t)-1) return -1;
    // Compile the argument tuple
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,
     DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_CALL_MEMBER_C,attr_id) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -2 +1
    goto pop_if_unused_and_end;
   }
   if ((compiler_flags&DEE_COMPILER_FLAG_OPTIMIZE_CONSTRUCTOR_CALLS)!=0) {
    // Using type prediction, optimize constructor calls
    DeeTypeObject const *fun_type = DeeXAst_PredictType(self->ast_operator.op_a);
    opcode = (Dee_uint8_t)(fun_type && DeeType_IsSameOrDerived(
     fun_type,&DeeType_Type) ? OP_NEW_INSTANCE : OP_CALL);
   } else opcode = OP_CALL;
  } goto binary_operator;

  case DEE_XASTKIND_COMPARE_LO: opcode = OP_CMP_LO; goto binary_operator;
  case DEE_XASTKIND_COMPARE_LE: opcode = OP_CMP_LE; goto binary_operator;
  case DEE_XASTKIND_COMPARE_EQ: opcode = OP_CMP_EQ; goto binary_operator;
  case DEE_XASTKIND_COMPARE_NE: opcode = OP_CMP_NE; goto binary_operator;
  case DEE_XASTKIND_COMPARE_GR: opcode = OP_CMP_GR; goto binary_operator;
  case DEE_XASTKIND_COMPARE_GE: opcode = OP_CMP_GE; goto binary_operator;
  case DEE_XASTKIND_ADD: opcode = OP_ADD; goto binary_operator;
  case DEE_XASTKIND_SUB: opcode = OP_SUB; goto binary_operator;
  case DEE_XASTKIND_MUL: opcode = OP_MUL; goto binary_operator;
  case DEE_XASTKIND_DIV: opcode = OP_DIV; goto binary_operator;
  case DEE_XASTKIND_MOD: opcode = OP_MOD; goto binary_operator;
  case DEE_XASTKIND_SHL: opcode = OP_SHL; goto binary_operator;
  case DEE_XASTKIND_SHR: opcode = OP_SHR; goto binary_operator;
  case DEE_XASTKIND_AND: opcode = OP_AND; goto binary_operator;
  case DEE_XASTKIND_OR: opcode = OP_OR; goto binary_operator;
  case DEE_XASTKIND_XOR: opcode = OP_XOR; goto binary_operator;
  case DEE_XASTKIND_POW: opcode = OP_POW; goto binary_operator;
  case DEE_XASTKIND_SEQ_GET: opcode = OP_SEQ_GET; goto binary_operator;
  case DEE_XASTKIND_ATTR_GET: opcode = OP_ATTR_GET; goto binary_operator;
  case DEE_XASTKIND_ATTR_HAS: opcode = OP_ATTR_HAS; goto binary_operator;
  case DEE_XASTKIND_IS: opcode = OP_IS; goto binary_operator;
  case DEE_XASTKIND_IN: opcode = OP_IN; goto binary_operator;
  case DEE_XASTKIND_MOVE_ASSIGN: opcode = OP_MOVE_ASSIGN; goto binary_operator;
  case DEE_XASTKIND_COPY_ASSIGN: opcode = OP_COPY_ASSIGN;
binary_operator:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,opcode) != 0) return -1;
   goto pop_if_unused_and_end;

  case DEE_XASTKIND_SEQ_SET: opcode = OP_SEQ_SET; goto trinary_operator;
  case DEE_XASTKIND_ATTR_SET: opcode = OP_ATTR_SET;
trinary_operator:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_c,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_TrinaryOp(writer,opcode) != 0) return -1;
   goto pop_if_unused_and_end;

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN: {
#if !defined(OP_LOAD_EXTERN) && !defined(OP_LOAD_AUTOEXTERN)
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(
    writer,(DeeObject *)&DeeBuiltinFunction___builtin_extern) != 0)) return -1;
#elif !defined(OP_LOAD_EXTERN)
   if (ret_used && self->ast_builtin_extern.ex_file && DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(
    writer,(DeeObject *)&DeeBuiltinFunction___builtin_extern) != 0)) return -1;
#elif !defined(OP_LOAD_AUTOEXTERN)
   if (ret_used && !self->ast_builtin_extern.ex_file && DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(
    writer,(DeeObject *)&DeeBuiltinFunction___builtin_extern) != 0)) return -1;
#endif
   if (self->ast_builtin_extern.ex_file && DEE_UNLIKELY(DeeXAst_Compile(self->ast_builtin_extern.ex_file,DEE_COMPILER_ARGS) != 0)) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_builtin_extern.ex_name,DEE_COMPILER_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_builtin_extern.ex_type,DEE_COMPILER_ARGS) != 0) return -1;
   if (!ret_used) return 0;
#if !defined(OP_LOAD_EXTERN) && !defined(OP_LOAD_AUTOEXTERN)
   if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,self->ast_builtin_extern.ex_file ? 3 : 2) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
#elif !defined(OP_LOAD_EXTERN)
   if (self->ast_builtin_extern.ex_file) {
    if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,3) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LOAD_AUTOEXTERN) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -2 +1
   }
#elif !defined(OP_LOAD_AUTOEXTERN)
   if (!self->ast_builtin_extern.ex_file) {
    if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,2) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LOAD_EXTERN) != 0) return -1;
    DeeCodeWriter_DECSTACK_N(writer,2); // -3 +1
   }
#else
   if (self->ast_builtin_extern.ex_file) {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LOAD_EXTERN) != 0) return -1;
    DeeCodeWriter_DECSTACK_N(writer,2); // -3 +1
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_LOAD_AUTOEXTERN) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -3 +1
   }
#endif
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
  case DEE_XASTKIND_BUILTIN_FF_CLOSURE: {
#ifndef OP_FF_CLOSURE
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(
    writer,(DeeObject *)&DeeBuiltinFunction___builtin_ff_closure) != 0)) return -1;
#endif
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS) != 0) return -1;
   if (ret_used) {
#ifdef OP_FF_CLOSURE
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_FF_CLOSURE) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -2 +1
#else
    if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,2) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
#endif
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DEE_XASTKIND_BUILTIN_ALLOCA:
   if (!ret_used) return 0; // don't even try this!
  {
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_ALLOCA) != 0) return -1;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case DEE_XASTKIND_BUILTIN_HELP:
   if (!ret_used) return 0;
  {
   switch (self->ast_operator.op_a->ast_kind) {
    case DEE_XASTKIND_ATTR_GET:
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a->ast_operator.op_a,DEE_COMPILER_ARGS) != 0) return -1;
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a->ast_operator.op_b,DEE_COMPILER_ARGS) != 0) return -1;
     if (0) {
    case DEE_XASTKIND_ATTR_GET_C:
      if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a->ast_attr_get_c.ac_object,DEE_COMPILER_ARGS) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,compiler_flags) != 0) return -1;
     }
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_DOC_ATTR) != 0) return -1;
     DeeCodeWriter_DECSTACK(writer);
     break;

    default:
     if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_DOC) != 0) return -1;
     break;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DEE_XASTKIND_BUILTIN_BREAKPOINT:
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BREAKPOINT) != 0) return -1;
   goto push_none;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DEE_XASTKIND_BUILTIN_UNREACHABLE:
   if DEE_UNLIKELY((compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0 &&
                   DeeCodeWriter_WriteOp(writer,OP_UNREACHABLE) != 0) return -1;
   //if (ret_used && DeeCodeWriter_PushNone(writer) != 0) return -1;
   if (ret_used) DeeCodeWriter_INCSTACK(writer); // psht...
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
   if (!ret_used) return 0;
  {
   DeeLocalVarObject *bound_var = self->ast_builtin_bound.b_var;
   DEE_ASSERT(DeeCodeWriter_IsVarLocal(writer,bound_var));
   if (DeeLocalVar_IS_COMPILERINIT(bound_var)) {
    switch (DeeLocalVar_KIND(bound_var)) {
     case DEE_LOCALVAR_KIND_LOCAL:
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_PREFIX,bound_var->lv_loc_id) != 0 ||
                      DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BOUND_LOCAL) != 0) return -1;
      DeeCodeWriter_INCSTACK(writer);
      break;
     case DEE_LOCALVAR_KIND_RETVAL:
      if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BOUND_RESULT) != 0) return -1;
      DeeCodeWriter_INCSTACK(writer);
      break;
     default:
      if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,Dee_True,compiler_flags) != 0) return -1;
      break;
    }
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_PushConstNoCopy(writer,Dee_False,compiler_flags) != 0) return -1;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

  case DEE_XASTKIND_BUILTIN_UNUSED: {
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
    compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS) != 0) return -1;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeXAst_Compile(self->ast_builtin_expect.e_value,DEE_COMPILER_ARGS);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  case DEE_XASTKIND_SUPER_AT:
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_SUPER_AT) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer);
   goto pop_if_unused_and_end;

  case DEE_XASTKIND_DEL_VAR: {
   DEE_ASSERT(DeeObject_Check(self->ast_delvar.d_var) && DeeLocalVar_Check(self->ast_delvar.d_var));
   if DEE_UNLIKELY(DeeCodeWriter_DelVar(writer,self->ast_delvar.d_var,compiler_flags) != 0) return -1;
   goto push_none;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_foreignfunction.ff_base,DEE_COMPILER_ARGS) != 0) return -1;
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) {
    if DEE_UNLIKELY(DeeXAst_Compile(*iter,DEE_COMPILER_ARGS) != 0) return -1;
    ++iter;
   }
   if (ret_used) {
    if (self->ast_foreignfunction.ff_flags) {
     // Function type requires a prefix
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_PREFIX,(
      Dee_uint16_t)self->ast_foreignfunction.ff_flags) != 0) return -1;
    }
    //if (self->ast_foreignfunction.ff_argc > 0xFFFF) {
    // // TODO: Error
    //}
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_FOREIGN_FUNCTION,(
     Dee_uint16_t)self->ast_foreignfunction.ff_argc) != 0) return -1;
    DeeCodeWriter_DECSTACK_N(writer,self->ast_foreignfunction.ff_argc); // consumed by 'OP_FOREIGN_FUNCTION'
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_switch.s_expr,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   DEE_ASSERT(self->ast_switch.s_entryc != 0);
   DEE_ASSERT(self->ast_switch.s_entryv != 0);
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   // TODO: Generate a jump table for all cases known at compile-time
   do {
    Dee_size_t switchval_jmparg;
    if DEE_UNLIKELY(DeeCodeWriter_Dup(writer) != 0) return -1;
    if DEE_UNLIKELY(DeeXAst_Compile(iter->se_key,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_TRY_CMP_EQ) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // Removed by 'OPEXT_TRY_CMP_EQ'
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP_IF_FF_POP,&switchval_jmparg) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // Removed by 'OP_JUMP_IF_FF_POP'
    if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1; // Pop the expression value
    if DEE_UNLIKELY(DeeXAst_Compile(iter->se_value,DEE_COMPILER_ARGS) != 0) return -1;
    if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one value will be kept
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&iter->se_jmparg) != 0) return -1;
    // Jump over the switch value if it didn't match
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,switchval_jmparg,(Dee_size_t)(
     DeeCodeWriter_ADDR(writer)-(switchval_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
    DeeCodeWriter_INCSTACK(writer); // Bring back the expression value
   } while (++iter != end);
   // Pop the expression value
   if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1;
   // Push the default value
   if DEE_UNLIKELY(self->ast_switch.s_default) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_switch.s_default,DEE_COMPILER_ARGS) != 0) return -1;
   } else if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   // go through and setup all the jumps
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   do {
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,iter->se_jmparg,(Dee_size_t)(
     DeeCodeWriter_ADDR(writer)-(iter->se_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
   } while (++iter != end);
  } break;

  {
   DeeXAstObject *live_ast,*dead_ast;
   Dee_size_t dead_start,dead_jump;
   if (0) {case DEE_XASTKIND_IFFALSE: live_ast = self->ast_iffalse.ic_fail,dead_ast = self->ast_iffalse.ic_succ; }
   if (0) {case DEE_XASTKIND_IFTRUE:  live_ast = self->ast_iftrue.ic_succ, dead_ast = self->ast_iftrue.ic_fail;  }
   if (!dead_ast) { if (!live_ast) goto push_none; return DeeXAst_Compile(live_ast,DEE_COMPILER_ARGS); }
   if (live_ast) {
    if DEE_UNLIKELY(DeeXAst_Compile(live_ast,DEE_COMPILER_ARGS) != 0) return -1;
   } else {
    if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   }
   // Setup the jump to skip the dead block
   dead_start = DeeCodeWriter_ADDR(writer);
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&dead_jump) != 0) return -1;
   if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
   if DEE_UNLIKELY(DeeXAst_Compile(dead_ast,DEE_COMPILER_ARGS) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,dead_jump,(Dee_size_t)(
    DeeCodeWriter_ADDR(writer)-dead_start)) != 0) return -1;
  } break;

  {
   if (0) { case DEE_XASTKIND_IO_READNP: oparg = OPEXT_IO_READNP; }
   if (0) { case DEE_XASTKIND_IO_WRITENP: oparg = OPEXT_IO_WRITENP; }
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,oparg) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer); // -2 +1
   goto pop_if_unused_and_end;
  }

  case DEE_XASTKIND_IO_SEEK: {
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if (ret_used) {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_IO_SEEK_USED) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer); // -2 +1
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_IO_SEEK) != 0) return -1;
    DeeCodeWriter_DECSTACK_N(writer,2); // -2
   }
  } break;

  {
   if (0) { case DEE_XASTKIND_IO_FLUSH: oparg = OPEXT_IO_FLUSH; }
   if (0) { case DEE_XASTKIND_IO_TRUNC: oparg = OPEXT_IO_TRUNC; }
   if (0) { case DEE_XASTKIND_IO_CLOSE: oparg = OPEXT_IO_CLOSE; }
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,
    DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,oparg) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer); // -1
   goto pop_if_unused_and_end;
  }

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  {
   if (0) { case DEE_XASTKIND_IO_READP:  oparg = OPEXT_IO_READP; }
   if (0) { case DEE_XASTKIND_IO_WRITEP: oparg = OPEXT_IO_WRITEP; }
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_a,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_b,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_operator.op_c,DEE_COMPILER_ARGS_EX(compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,oparg) != 0) return -1;
   DeeCodeWriter_DECSTACK_N(writer,2); // -3 +1
   goto pop_if_unused_and_end;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

  //case DEE_XASTKIND_MODULE: // unsupported
  // break;

  default: // Shouldn't happen
   DEE_LVERBOSE1("Compiling unknown/unsupported XAst: %r\n",self);
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   break;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_DECL_END

#endif /* !GUARD_DEEMON_XAST_H */
