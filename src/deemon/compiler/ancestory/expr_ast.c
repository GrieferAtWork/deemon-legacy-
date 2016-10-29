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
#ifndef GUARD_DEEMON_EXPR_AST_C
#define GUARD_DEEMON_EXPR_AST_C 1
#define DEE_LIMITED_API 1
#define DEE_GUID_PROTOTYPES 1

#include <deemon/__conf.inl>

// What a mess...
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/class.h>
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/expr_ast.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/stmt_ast.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/deemonrun.h>
#include <deemon/foreign_function.h>
#include <deemon/guid.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/weakref.h>
#include DEE_INCLUDE_MEMORY_API()

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
#include <deemon/cell.h>
#endif

DEE_DECL_BEGIN


#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
static void _deeclassdecl_tp_dtor(struct DeeClassDeclObject *self) {
 struct DeeClassDeclEntry *begin,*end;
 Dee_XDECREF(self->ob_name);
 Dee_XDECREF(self->ob_base);
 end = (begin = self->ob_entryv)+self->ob_entryc;
 while (begin != end) { _DeeClassDeclEntry_VISIT(begin,Dee_DECREF,Dee_XDECREF); ++begin; }
 free(self->ob_entryv);
}
DEE_VISIT_PROC(_deeclassdecl_tp_visit,struct DeeClassDeclObject *self) {
 struct DeeClassDeclEntry *begin,*end;
 Dee_XVISIT(self->ob_name);
 Dee_XVISIT(self->ob_base);
 end = (begin = self->ob_entryv)+self->ob_entryc;
 while (begin != end) { _DeeClassDeclEntry_VISIT(begin,Dee_VISIT,Dee_XVISIT); ++begin; }
}

DeeTypeObject DeeClassDecl_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("class_decl"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(struct DeeClassDeclObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member((void(*)(DeeObject*))&_deeclassdecl_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member((DeeType_SLOT_TYPE(tp_visit))&_deeclassdecl_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeClassDeclObject) *DeeClassDecl_New(
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *base) {
 struct DeeClassDeclObject *result;
 DEE_ASSERT(!name || (DeeObject_Check(name) && DeeString_Check(name)));
 DEE_ASSERT(!base || (DeeObject_Check(base) && DeeExprAst_Check(base)));
 if ((result = DeeObject_MALLOC(struct DeeClassDeclObject)) != NULL) {
  DeeObject_INIT(result,&DeeClassDecl_Type);
  Dee_XINCREF(result->ob_name = (DeeStringObject *)name);
  Dee_XINCREF(result->ob_base = (DeeExprAstObject *)base);
  result->ob_entryc = 0;
  result->ob_entrya = 0;
  result->ob_entryv = NULL;
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int DeeClassDecl_IncreaseCapacity(
 DEE_A_INOUT struct DeeClassDeclObject *self) {
 struct DeeClassDeclEntry *new_list; Dee_size_t new_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 if (self->ob_entrya) new_size = self->ob_entrya * 2; else new_size = 2;
 if ((new_list = (struct DeeClassDeclEntry *)realloc_nz(
  self->ob_entryv,new_size*sizeof(struct DeeClassDeclEntry))
  ) == NULL) { DeeError_NoMemory(); return -1; }
 self->ob_entryv = new_list;
 self->ob_entrya = new_size;
 return 0;
}
DEE_A_RET_EXCEPT(NULL) struct DeeClassDeclEntry *
DeeClassDecl_AllocSlot(DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 if (((struct DeeClassDeclObject *)self)->ob_entrya ==
     ((struct DeeClassDeclObject *)self)->ob_entryc &&
     DeeClassDecl_IncreaseCapacity(((struct DeeClassDeclObject *)self)) == -1
     ) return NULL;
 return ((struct DeeClassDeclObject *)self)->ob_entryv+(
  ((struct DeeClassDeclObject *)self)->ob_entryc++);
}
DEE_STATIC_INLINE(int) _DeeType_SameSlot(int a, int b) {
 if (a == b) return 1;
 if ((a == DeeType_SLOT_ID(tp_seq_iter_self) ||
      a == DeeType_SLOT_ID(tp_p_seq_iter_self)) &&
     (b == DeeType_SLOT_ID(tp_seq_iter_self) ||
      b == DeeType_SLOT_ID(tp_p_seq_iter_self))
      ) return 1;
 return 0;
}

DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasSlot(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN int slot, DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token) {
 struct DeeClassDeclEntry *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(decl_token);
 end = (begin = ((struct DeeClassDeclObject *)self)->ob_entryv)+(
  (struct DeeClassDeclObject *)self)->ob_entryc;
 while (begin != end) {
  if (begin->ob_kind == DeeClassDeclEntryKind_SLOT &&
     _DeeType_SameSlot(begin->ob_slot.ob_slotid,slot)) {
   *decl_token = (DeeObject *)begin->ob_slot.ob_callback->ob_token;
   return 1;
  }
  ++begin;
 }
 return 0;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasFunctionOrGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token) {
 struct DeeClassDeclEntry *begin,*end; char const *name_str; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(decl_token);
 end = (begin = ((struct DeeClassDeclObject *)self)->ob_entryv)+(
  (struct DeeClassDeclObject *)self)->ob_entryc;
 name_str = DeeString_STR(name);
 name_len = DeeString_SIZE(name);
 while (begin != end) {
  switch (begin->ob_kind) {
   case DeeClassDeclEntryKind_METHOD:
    if (DeeString_SIZE(begin->ob_method.ob_name) == name_len &&
        memcmp(DeeString_STR(begin->ob_method.ob_name),name_str,name_len) == 0) {
     *decl_token = (DeeObject *)begin->ob_method.ob_callback->ob_token;
     return 1;
    }
    break;
   case DeeClassDeclEntryKind_GETSET:
    if (DeeString_SIZE(begin->ob_getset.ob_name) == name_len &&
        memcmp(DeeString_STR(begin->ob_getset.ob_name),name_str,name_len) == 0) {
     if (begin->ob_getset.ob_get) *decl_token = (DeeObject *)begin->ob_getset.ob_get->ob_token;
     else if (begin->ob_getset.ob_del) *decl_token = (DeeObject *)begin->ob_getset.ob_del->ob_token;
     else if (begin->ob_getset.ob_set) *decl_token = (DeeObject *)begin->ob_getset.ob_set->ob_token;
     else *decl_token = NULL;
     return 1;
    }
    break;
   default: break;
  }
  ++begin;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasClassFunctionOrGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token) {
 struct DeeClassDeclEntry *begin,*end; char const *name_str; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(decl_token);
 end = (begin = ((struct DeeClassDeclObject *)self)->ob_entryv)+(
  (struct DeeClassDeclObject *)self)->ob_entryc;
 name_str = DeeString_STR(name);
 name_len = DeeString_SIZE(name);
 while (begin != end) {
  switch (begin->ob_kind) {
   case DeeClassDeclEntryKind_CLASS_METHOD:
    if (DeeString_SIZE(begin->ob_method.ob_name) == name_len &&
        memcmp(DeeString_STR(begin->ob_method.ob_name),name_str,name_len) == 0) {
     *decl_token = (DeeObject *)begin->ob_method.ob_callback->ob_token;
     return 1;
    }
    break;
   case DeeClassDeclEntryKind_CLASS_GETSET:
    if (DeeString_SIZE(begin->ob_getset.ob_name) == name_len &&
        memcmp(DeeString_STR(begin->ob_getset.ob_name),name_str,name_len) == 0) {
     if (begin->ob_getset.ob_get) *decl_token = (DeeObject *)begin->ob_getset.ob_get->ob_token;
     else if (begin->ob_getset.ob_del) *decl_token = (DeeObject *)begin->ob_getset.ob_del->ob_token;
     else if (begin->ob_getset.ob_set) *decl_token = (DeeObject *)begin->ob_getset.ob_set->ob_token;
     else *decl_token = NULL;
     return 1;
    }
    break;
   default: break;
  }
  ++begin;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddSlot(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN int slot, DEE_A_INOUT_OBJECT(DeeExprAstObject) *decl) {
 struct DeeClassDeclEntry *entry;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(DeeObject_Check(decl) && DeeExprAst_Check(decl));
 if ((entry = DeeClassDecl_AllocSlot(self)) == NULL) return -1;
 entry->ob_kind = DeeClassDeclEntryKind_SLOT;
 entry->ob_visibility = visibility;
 entry->ob_slot.ob_slotid = slot;
 Dee_INCREF(entry->ob_slot.ob_callback = (DeeExprAstObject *)decl);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddMethod(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN enum DeeClassDeclEntryKind kind,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT(DeeExprAstObject) *decl) {
 struct DeeClassDeclEntry *entry;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(DeeObject_Check(decl) && DeeExprAst_Check(decl));
 DEE_ASSERT(kind == DeeClassDeclEntryKind_METHOD || kind == DeeClassDeclEntryKind_CLASS_METHOD);
 if ((entry = DeeClassDecl_AllocSlot(self)) == NULL) return -1;
 entry->ob_kind = kind;
 entry->ob_visibility = visibility;
 Dee_INCREF(entry->ob_method.ob_name = (DeeStringObject *)name);
 Dee_INCREF(entry->ob_method.ob_callback = (DeeExprAstObject *)decl);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN enum DeeClassDeclEntryKind kind,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *get_decl,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *del_decl,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *set_decl) {
 struct DeeClassDeclEntry *entry;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(!get_decl || (DeeObject_Check(get_decl) && DeeExprAst_Check(get_decl)));
 DEE_ASSERT(!del_decl || (DeeObject_Check(del_decl) && DeeExprAst_Check(del_decl)));
 DEE_ASSERT(!set_decl || (DeeObject_Check(set_decl) && DeeExprAst_Check(set_decl)));
 DEE_ASSERT(kind == DeeClassDeclEntryKind_GETSET || kind == DeeClassDeclEntryKind_CLASS_GETSET);
 if ((entry = DeeClassDecl_AllocSlot(self)) == NULL) return -1;
 entry->ob_kind = kind;
 entry->ob_visibility = visibility;
 Dee_INCREF(entry->ob_getset.ob_name = (DeeStringObject *)name);
 Dee_XINCREF(entry->ob_getset.ob_get = (DeeExprAstObject *)get_decl);
 Dee_XINCREF(entry->ob_getset.ob_del = (DeeExprAstObject *)del_decl);
 Dee_XINCREF(entry->ob_getset.ob_set = (DeeExprAstObject *)set_decl);
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


_DeeObjectPool_Define(_DeeExprAstPool,DeeExprAstObject)

#ifdef DEE_DEBUG
_DeeObjectPool_DefineStastics(_DeeExprAstPool)
#endif

Dee_size_t DeeExprAst_ClearPool(void) {
#ifdef DEE_DEBUG
 DEE_LVERBOSE3("Clearing expr_ast pool (%Iu)\n",_DeeExprAstPool_Size());
#endif
 return _DeeExprAstPool_Clear();
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeExprAst_New(
 DEE_A_IN DeeExprAstKind kind, DEE_A_IN_OBJECT(DeeTokenObject) *token_) {
 DeeExprAstObject *result;
 DEE_ASSERT(DeeObject_Check(token_) && DeeToken_Check(token_));
 if ((result = (DeeExprAstObject *)_DeeExprAstPool_Malloc()) != NULL) {
  DeeObject_INIT(result,&DeeExprAst_Type);
  result->ob_kind = kind;
  Dee_INCREF(result->ob_token = (DeeTokenObject *)token_);
  result->ob_op_a = NULL;
  result->ob_op_b = NULL;
  result->ob_op_c = NULL;
  result->ob_op_d = NULL;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeExprAst_CopyWithScope(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self,
 DEE_A_INOUT_OBJECT(DeeScopeObject) *scope) {
 DeeExprAstObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeScope_IS_STRONG(scope) && "scope argument must be a strong scope");
 if ((result = (DeeExprAstObject *)_DeeExprAstPool_Malloc()) == NULL) return NULL;
 DeeObject_INIT(result,&DeeExprAst_Type);
 result->ob_kind = DeeExprAst_KIND(self);
 Dee_INCREF(result->ob_token = DeeExprAst_TOKEN(self));
 switch (result->ob_kind) {
  case DeeExprAstKind_VARIABLE:
  case DeeExprAstKind_DECLVARIABLE:
   DEE_ASSERT(DeeObject_Check(DeeExprAst_VAR(self)) &&
              DeeLocalVar_Check(DeeExprAst_VAR(self)));
   if (!DeeScope_SameWeakScope((DeeObject *)DeeExprAst_VAR(self)->ob_scope,scope) &&
        DeeScope_ExternalReference(scope,(DeeObject *)DeeExprAst_VAR(self)) != 0) goto err_0;
   Dee_INCREF(result->ob_var = DeeExprAst_VAR(self));
   if (result->ob_kind != DeeExprAstKind_DECLVARIABLE) ++result->ob_var->ob_uses;
   result->ob_op_b = NULL;
   result->ob_op_c = NULL;
   result->ob_op_d = NULL;
   break;
  case DeeExprAstKind_VARDECL:
   // TODO
   result->ob_kind = DeeExprAstKind_ASSIGN;
   // fallthrough
  default:
   if (DeeExprAst_OP_A(self)) {
    if (DeeExprAst_Check(DeeExprAst_OP_A(self))) {
     if ((result->ob_op_a = (DeeExprAstObject *)DeeExprAst_CopyWithScope(
      (DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL)
     {err_0: Dee_DECREF(result->ob_token); _DeeObject_Delete((DeeObject *)result); return NULL; }
    } else Dee_INCREF(result->ob_op_a = DeeExprAst_OP_A(self));
   } else result->ob_op_a = NULL;
   if (DeeExprAst_OP_B(self)) {
    if (DeeExprAst_Check(DeeExprAst_OP_B(self))) {
     if ((result->ob_op_b = (DeeExprAstObject *)DeeExprAst_CopyWithScope(
      (DeeObject *)DeeExprAst_OP_B(self),scope)) == NULL)
     {err_1: Dee_XDECREF(result->ob_op_a); goto err_0; }
    } else Dee_INCREF(result->ob_op_b = DeeExprAst_OP_B(self));
   } else result->ob_op_b = NULL;
   if (DeeExprAst_OP_C(self)) {
    if (DeeExprAst_Check(DeeExprAst_OP_C(self))) {
     if ((result->ob_op_c = (DeeExprAstObject *)DeeExprAst_CopyWithScope(
      (DeeObject *)DeeExprAst_OP_C(self),scope)) == NULL)
     {err_2: Dee_XDECREF(result->ob_op_b); goto err_1; }
    } else Dee_INCREF(result->ob_op_c = DeeExprAst_OP_C(self));
   } else result->ob_op_c = NULL;
   if (DeeExprAst_OP_D(self)) {
    if (DeeExprAst_Check(DeeExprAst_OP_D(self))) {
     if ((result->ob_op_d = (DeeExprAstObject *)DeeExprAst_CopyWithScope(
      (DeeObject *)DeeExprAst_OP_D(self),scope)) == NULL)
     {/*err_3:*/ Dee_XDECREF(result->ob_op_c); goto err_2; }
    } else Dee_INCREF(result->ob_op_d = DeeExprAst_OP_D(self));
   } else result->ob_op_d = NULL;
   break;
 }
 return (DeeObject *)result;
}


DEE_A_RET_WUNUSED int DeeExprAst_NeedSemicollon(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 switch (DeeExprAst_KIND(self)) {
  case DeeExprAstKind_CONSTANT:
   return !DeeFunction_Check(DeeExprAst_CONST(self));
  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
   return 0;
  default: break;
 }
 return 1;
}

DEE_A_RET_WUNUSED int DeeExprAst_IsNoexcept(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 switch (DeeExprAst_KIND(self)) {
  
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL:
   return DeeExprAst_OP_A(self) ? DeeExprAst_IsNoexcept((
    DeeObject *)DeeExprAst_OP_A(self),scope) : 1;
#endif

  case DeeExprAstKind_COND:
   if (!DeeExprAst_IsNoexcept((DeeObject *)DeeExprAst_OP_A(self),scope) ||
       !DeeExprAst_IsNoexcept((DeeObject *)DeeExprAst_OP_B(self),scope)) return 0;
   if (DeeExprAst_OP_C(self) && !DeeExprAst_IsNoexcept((DeeObject *)DeeExprAst_OP_C(self),scope)) return 0;
   return 1;

#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   return DeeStmtAst_IsNoexcept((DeeObject *)DeeExprAst_STMT(self),scope);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */

  case DeeExprAstKind_VARDECL:
   return DeeExprAst_IsNoexcept((DeeObject *)DeeExprAst_OP_B(self),scope);

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  // 'delete' never throws an exception
  case DeeExprAstKind_DELETE:
#endif
  // 'is' never throws an exception
  case DeeExprAstKind_IS:
  case DeeExprAstKind_MODULE:
  case DeeExprAstKind_CONSTANT:
  case DeeExprAstKind_VARIABLE:
  case DeeExprAstKind_DECLVARIABLE:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND:
#endif

  // Dex must be noeffect, so unused dex variables can be optimized away!
  case DeeExprAstKind_DEX:

  // The function ast types might seem weird here, but
  // this isn't about calling those functions.
  // - And the function ast itself can't cause an exception.
  //   Only once we start assigning those asts can exceptions start to occurr
  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_FUNCTION_NONE:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_VFUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_NONE:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
   return 1;
  default: break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeExprAst_IsNoreturn(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope,
 DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 switch (DeeExprAst_KIND(self)) {

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL:
   return DeeExprAst_OP_A(self) ? DeeExprAst_IsNoreturn((
    DeeObject *)DeeExprAst_OP_A(self),scope,flags) : 0;
#endif


#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DeeExprAstKind_BUILTIN_UNREACHABLE:
   return 1;
#endif
 
  case DeeExprAstKind_COND:
   if (DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags)) return 1;
   if (DeeExprAst_OP_B(self) && DeeExprAst_OP_C(self) &&
       DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags) &&
       DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_C(self),scope,flags)) return 1;
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   return DeeStmtAst_IsNoreturn(
    (DeeObject *)DeeExprAst_STMT(self),
    (DeeObject *)DeeExprAst_SCOPE(self),flags);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */

  case DeeExprAstKind_NOT:
  case DeeExprAstKind_NEG:
  case DeeExprAstKind_POS:
  case DeeExprAstKind_INV:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF:
  case DeeExprAstKind_DEREF:
#endif
  case DeeExprAstKind_INC:
  case DeeExprAstKind_DEC:
  case DeeExprAstKind_INC_POST:
  case DeeExprAstKind_DEC_POST:
  case DeeExprAstKind_SEQ_SIZE:
  case DeeExprAstKind_EXPAND:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DeeExprAstKind_BUILTIN_ALLOCA:
#endif
  case DeeExprAstKind_BUILTIN_HELP:
  case DeeExprAstKind_STR:
  case DeeExprAstKind_REPR:
  case DeeExprAstKind_COPY:
  case DeeExprAstKind_MOVE:
  case DeeExprAstKind_TYPE:
  case DeeExprAstKind_WEAK:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_NEW:
  case DeeExprAstKind_DELETE:
#endif
  case DeeExprAstKind_BOOL:
  case DeeExprAstKind_SUPER:
  case DeeExprAstKind_CLASSOF:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND:
#endif
  case DeeExprAstKind_ATTR_GET:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags);

  case DeeExprAstKind_ASSIGN:
  case DeeExprAstKind_CMP_LO:
  case DeeExprAstKind_CMP_LE:
  case DeeExprAstKind_CMP_EQ:
  case DeeExprAstKind_CMP_NE:
  case DeeExprAstKind_CMP_GR:
  case DeeExprAstKind_CMP_GE:
  case DeeExprAstKind_ADD:
  case DeeExprAstKind_IADD:
  case DeeExprAstKind_SUB:
  case DeeExprAstKind_ISUB:
  case DeeExprAstKind_MUL:
  case DeeExprAstKind_IMUL:
  case DeeExprAstKind_DIV:
  case DeeExprAstKind_IDIV:
  case DeeExprAstKind_MOD:
  case DeeExprAstKind_IMOD:
  case DeeExprAstKind_SHL:
  case DeeExprAstKind_ISHL:
  case DeeExprAstKind_SHR:
  case DeeExprAstKind_ISHR:
  case DeeExprAstKind_AND:
  case DeeExprAstKind_IAND:
  case DeeExprAstKind_OR:
  case DeeExprAstKind_IOR:
  case DeeExprAstKind_XOR:
  case DeeExprAstKind_IXOR:
  case DeeExprAstKind_POW:
  case DeeExprAstKind_IPOW:
  case DeeExprAstKind_LAND:
  case DeeExprAstKind_LOR:
  case DeeExprAstKind_LXOR:
  case DeeExprAstKind_CALL:
  case DeeExprAstKind_SEQ_GET:
  case DeeExprAstKind_IS:
  case DeeExprAstKind_IN:
  case DeeExprAstKind_BUILTIN_EXPECT:
  case DeeExprAstKind___SUPER:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags) ||
          DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags);

  case DeeExprAstKind_VARDECL:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags);

  case DeeExprAstKind_SEQ_SET:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags) ||
          DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags) ||
          DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_C(self),scope,flags);
  case DeeExprAstKind_ATTR_SET:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags) ||
          DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_C(self),scope,flags);
  case DeeExprAstKind_SEQ_RANGE_GET:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags) ||
          (DeeExprAst_OP_B(self) && DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags)) ||
          (DeeExprAst_OP_C(self) && DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_C(self),scope,flags));
  case DeeExprAstKind_SEQ_RANGE_SET:
   return DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_A(self),scope,flags) ||
          (DeeExprAst_OP_B(self) && DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_B(self),scope,flags)) ||
          (DeeExprAst_OP_C(self) && DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_C(self),scope,flags)) ||
          DeeExprAst_IsNoreturn((DeeObject *)DeeExprAst_OP_D(self),scope,flags);
  case DeeExprAstKind_LIST:
  case DeeExprAstKind_TUPLE:
  case DeeExprAstKind_DICT:
  case DeeExprAstKind_SET: {
   DeeObject **iter,**end;
   end = (iter = DeeTuple_ELEM(DeeExprAst_ARGS_A(self)))+
    DeeTuple_SIZE(DeeExprAst_ARGS_A(self));
   while (iter != end) if (DeeExprAst_IsNoreturn(*iter++,scope,flags)) return 1;
  } break;

  default: break;
 }
 return 0;
}

DEE_STATIC_INLINE(int) _dee_tuple_of_exprast_is_constant(DeeTupleObject *self) {
 DeeExprAstObject **iter,**end;
 end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while (iter != end) if ((*iter++)->ob_kind != DeeExprAstKind_CONSTANT) return 0;
 return 1;
}
int _DeeExprAst_OptimizeTuple(DeeExprAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_TUPLE);
 if (_dee_tuple_of_exprast_is_constant(self->ob_args_a)) {
  DeeExprAstObject **iter,**end;
  DeeObject *const_tuple,**dst;
  if ((const_tuple = _DeeTuple_NewUnsafe(DeeTuple_SIZE(self->ob_args_a))) == NULL) return -1;
  end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(self->ob_args_a))+DeeTuple_SIZE(self->ob_args_a);
  dst = DeeTuple_ELEM(const_tuple);
  while (iter != end) Dee_INCREF(*dst++ = (*iter++)->ob_const);
  Dee_DECREF(self->ob_args_a);
  Dee_INHERIT_REF(self->ob_const,const_tuple);
  self->ob_kind = DeeExprAstKind_CONSTANT;
 }
 return 0;
}
int _DeeExprAst_OptimizeSet(DeeExprAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_SET);
 if (_dee_tuple_of_exprast_is_constant(self->ob_args_a)) {
  DeeExprAstObject **iter,**end;
  DeeObject *const_set;
  if ((const_set = DeeSet_New()) == NULL) return -1;
  end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(self->ob_args_a))+DeeTuple_SIZE(self->ob_args_a);
  while (iter != end) if (DeeSet_Insert(const_set,(*iter++)->ob_const) == -1) { Dee_DECREF(const_set); return -1; }
  Dee_DECREF(self->ob_args_a);
  Dee_INHERIT_REF(self->ob_const,const_set);
  self->ob_kind = DeeExprAstKind_CONSTANT;
 }
 return 0;
}
int _DeeExprAst_OptimizeList(DeeExprAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_LIST);
 if (_dee_tuple_of_exprast_is_constant(self->ob_args_a)) {
  DeeExprAstObject **iter,**end;
  DeeObject *const_list,**dst;
  if ((const_list = _DeeList_NewUnsafe(DeeTuple_SIZE(self->ob_args_a))) == NULL) return -1;
  end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(self->ob_args_a))+DeeTuple_SIZE(self->ob_args_a);
  dst = DeeList_ELEM(const_list);
  while (iter != end) Dee_INCREF(*dst++ = (*iter++)->ob_const);
  Dee_DECREF(self->ob_args_a);
  Dee_INHERIT_REF(self->ob_const,const_list);
  self->ob_kind = DeeExprAstKind_CONSTANT;
 }
 return 0;
}
int _DeeExprAst_OptimizeDict(DeeExprAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_DICT);
 if (_dee_tuple_of_exprast_is_constant(self->ob_dict)) {
  DeeExprAstObject **iter,**end;
  DeeObject *const_dict;
  if ((const_dict = DeeDict_New()) == NULL) return -1;
  DEE_ASSERT(DeeTuple_SIZE(self->ob_dict) % 2 == 0);
  end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(self->ob_dict))+DeeTuple_SIZE(self->ob_dict);
  while (iter != end) {
   if (DeeDict_SetItem(const_dict,iter[0]->ob_const,iter[1]->ob_const) == -1) {
    Dee_DECREF(const_dict);
    return -1;
   }
   iter += 2;
  }
  Dee_DECREF(self->ob_dict);
  Dee_INHERIT_REF(self->ob_const,const_dict);
  self->ob_kind = DeeExprAstKind_CONSTANT;
 }
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
DEE_A_RET_NOEXCEPT(0) int _DeeExprAst_CalculateOffsetof(
 DeeExprAstObject *self, DEE_A_OUT Dee_size_t *offset, DEE_A_OUT DeeTypeObject **last_type) {
 if (self->ob_kind == DeeExprAstKind_DEREF) {
  if (self->ob_op_a->ob_kind != DeeExprAstKind_CONSTANT ||
     !DeePointer_Check(self->ob_op_a->ob_const)) return 0;
  *offset = (Dee_size_t)DeePointer_ADDR(self->ob_op_a->ob_const);
  *last_type = DeePointerType_BASE(Dee_TYPE(self->ob_op_a->ob_const));
  return 1;
 } else if (self->ob_kind == DeeExprAstKind_ATTR_GET) {
  Dee_size_t more_offset;
  if (!_DeeExprAst_CalculateOffsetof(self->ob_op_a,offset,last_type)) return 0;
  if ((*last_type = DeeType_OffsetOf(*last_type,(
   DeeObject *)self->ob_member,&more_offset)) == NULL) { DeeError_Handled(); return 0; }
  *offset += more_offset;
  return 1;
 } else return 0;
}
#endif



static DeeExprAstObject *_deeparse_assign(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags);
DEE_STATIC_INLINE(DeeExprAstObject *) _deeparse_comma(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags);
DEE_STATIC_INLINE(DeeExprAstObject *) _deeparse_comma_tuple(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags);

#define new_ast(kind,token) (DeeExprAstObject *)DeeExprAst_New(kind,token)
#define token               self->ob_token->ob_token
#define token_ob            (DeeObject *)self->ob_token
#if DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF
#define yield()             (DeeTokenObject *)DeeLexer_YieldEx((DeeObject *)self,(DeeObject *)scope)
#else
#define yield()             (DeeTokenObject *)DeeLexer_Yield((DeeObject *)self)
#endif
#define _deeparse_top(self,scope,flags) \
 (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)(self),(DeeObject *)(scope),flags)
#define _deeparse_top_lv2   _deeparse_comma

static int _deeexprast_optimize_get_operators(DeeLexerObject *lexer, DeeExprAstObject *self);
#define _optimize_ast(a) ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0?_deeexprast_optimize_get_operators(self,a):0)
#define _do_optimize_ast(a)  _deeexprast_optimize_get_operators(self,a)


int _DeeLexer_ParseTypeSlotID(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 int result; (void)scope,flags;
again:
 switch (token.tk_id) {
  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
   if (!yield()) return -1;
   goto again;
  case KWD_long:
  case KWD_char:
  case KWD_short:
  case KWD_int:
  DEE_PARSE_TOKENID_CASE_WCHAR_T
  DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T
  DEE_PARSE_TOKENID_CASE_ATOMIC
  DEE_PARSE_TOKENID_CASE_SIGNED
  DEE_PARSE_TOKENID_CASE_UNSIGNED
  DEE_PARSE_TOKENID_CASE_UINT8
  DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32
  DEE_PARSE_TOKENID_CASE_UINT64
  DEE_PARSE_TOKENID_CASE_INT8
  DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32
  DEE_PARSE_TOKENID_CASE_INT64
  case KWD_float:
  case KWD_double: {
   int result;
   DeeExprAstObject *conversion_type_ast;
   DeeTypeObject *conversion_type;
   // TODO: No reason to create a real AST here (simplify '_DeeLexer_ParseType' into returning the type alone)
   if ((conversion_type_ast = _DeeLexer_ParseType(self,scope,flags)) == NULL) return -1;
   DEE_ASSERT(conversion_type_ast->ob_kind == DeeExprAstKind_CONSTANT);
   conversion_type = (DeeTypeObject *)conversion_type_ast->ob_const;
   if (conversion_type == DeeObject_TYPE(Dee_int32_t)) {
    result = DeeType_SLOT_ID(tp_int32);
   } else if (conversion_type == DeeObject_TYPE(Dee_int64_t)) {
    result = DeeType_SLOT_ID(tp_int64);
   } else if (conversion_type == DeeObject_TYPE(Dee_rt_double)) {
    result = DeeType_SLOT_ID(tp_double);
   } else if (conversion_type == DeeObject_TYPE(Dee_uint32_t)) {
    result = DeeType_SLOT_ID(tp_int32);
cant_override_unsigned:
    if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_CANT_OVERRIDE_UNSIGNED,
     (DeeObject *)self,(DeeObject *)conversion_type_ast->ob_token,
     "Can't override conversion for %q : unsigned type (overriding '%#q' instead)",
     DeeType_NAME(conversion_type),TPPKeyword_BuiltinToStr(
     _Dee_TypeSlot_ToTokenID(result))) == -1) return -1;
   } else if (conversion_type == DeeObject_TYPE(Dee_uint64_t)) {
    result = DeeType_SLOT_ID(tp_int64);
    goto cant_override_unsigned;
   } else if (conversion_type == DeeObject_TYPE(atomic(Dee_int32_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint32_t))) {
    result = DeeType_SLOT_ID(tp_int32);
cant_override_atomic:
    if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_CANT_OVERRIDE_ATOMICS,
     (DeeObject *)self,(DeeObject *)conversion_type_ast->ob_token,
     "Can't override conversion for %q : atomic type (overriding '%#q' instead)",
     DeeType_NAME(conversion_type),TPPKeyword_BuiltinToStr(
     _Dee_TypeSlot_ToTokenID(result))) == -1) return -1;
   } else if (conversion_type == DeeObject_TYPE(atomic(Dee_int64_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint64_t))) {
    result = DeeType_SLOT_ID(tp_int64);
    goto cant_override_atomic;
   } else if (conversion_type == DeeObject_TYPE(Dee_rt_float) ||
              conversion_type == DeeObject_TYPE(Dee_rt_ldouble)) {
    result = DeeType_SLOT_ID(tp_double);
    if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_CANT_OVERRIDE_NON_DOUBLE_FLOAT,
     (DeeObject *)self,(DeeObject *)conversion_type_ast->ob_token,
     "Can't override conversion for %q : non-double floating type (overriding '__double__' instead)",
     DeeType_NAME(conversion_type)) == -1) return -1;
   } else if (conversion_type == DeeObject_TYPE(Dee_int8_t) ||
              conversion_type == DeeObject_TYPE(Dee_uint8_t) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_int8_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint8_t)) ||
              conversion_type == DeeObject_TYPE(Dee_int16_t) ||
              conversion_type == DeeObject_TYPE(Dee_uint16_t) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_int16_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint16_t))) {
    result = DeeType_SLOT_ID(tp_int32);
    if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_CANT_OVERRIDE_NON_DOUBLE_FLOAT,
     (DeeObject *)self,(DeeObject *)conversion_type_ast->ob_token,
     "Can't override conversion for %q : type too narrow (overriding '__int32__' instead)",
     DeeType_NAME(conversion_type)) == -1) return -1;
   } else {
    // Shouldn't happen (technically...)
    result = 0;
    if (DeeError_CompilerErrorf(DEE_WARNING_OPERATOR_NAME_CANT_OVERRIDE_UNSUPPORTED,
     (DeeObject *)self,(DeeObject *)conversion_type_ast->ob_token,
     "Can't override convert operator for unsupported type: '%#q'",
     DeeType_NAME(conversion_type)) == -1) return -1;
   }
   Dee_DECREF(conversion_type_ast);
   return result;
  } break;

  case KWD_string: case KWD_str: case KWD___str__: 
   result = DeeType_SLOT_ID(tp_str);
y1: if (!yield()) return -1;
   return result;
#define RETURN(x) do{result=(x);goto y1;}while(0)
  case KWD_repr: case KWD___repr__: RETURN(DeeType_SLOT_ID(tp_repr));

  case KWD___copy__: RETURN(DeeType_SLOT_ID(tp_copy_ctor));
  case KWD_copy: if (!yield()) return -1;
   if (token.tk_id == '=' || token.tk_id == TPP_TOK_COLLON_ASSIGN
       ) RETURN(DeeType_SLOT_ID(tp_copy_assign));
   return DeeType_SLOT_ID(tp_copy_ctor);
  case KWD___move__: RETURN(DeeType_SLOT_ID(tp_move_ctor));
  case KWD_move: if (!yield()) return -1;
   if (token.tk_id == '=' || token.tk_id == TPP_TOK_COLLON_ASSIGN
       ) RETURN(DeeType_SLOT_ID(tp_move_assign));
   return DeeType_SLOT_ID(tp_move_ctor);
  case KWD___assign__: RETURN(DeeType_SLOT_ID(tp_any_assign));
  case '=': case TPP_TOK_COLLON_ASSIGN:
   if (!yield()) return -1;
   if (token.tk_id == KWD_copy || token.tk_id == KWD___copy__) RETURN(DeeType_SLOT_ID(tp_copy_assign));
   if (token.tk_id == KWD_move || token.tk_id == KWD___move__) RETURN(DeeType_SLOT_ID(tp_move_assign));
   return DeeType_SLOT_ID(tp_any_assign);
  case KWD___copyassign__: RETURN(DeeType_SLOT_ID(tp_copy_assign));
  case KWD___moveassign__: RETURN(DeeType_SLOT_ID(tp_move_assign));
  case KWD___constructor__: RETURN(DeeType_SLOT_ID(tp_p_any_ctor));
  case KWD___destructor__: RETURN(DeeType_SLOT_ID(tp_dtor));
  DEE_PARSE_TOKENID_CASE_BOOL case KWD___bool__: RETURN(DeeType_SLOT_ID(tp_bool));
  case '+': RETURN(DeeType_SLOT_ID(tp_p_add));
  case '-': RETURN(DeeType_SLOT_ID(tp_p_sub));
  case KWD___add__: RETURN(DeeType_SLOT_ID(tp_add));
  case KWD___sub__: RETURN(DeeType_SLOT_ID(tp_sub));
  case '!': case KWD___not__: RETURN(DeeType_SLOT_ID(tp_not));
  case '~': case KWD___inv__: RETURN(DeeType_SLOT_ID(tp_inv));
  case KWD___pos__: RETURN(DeeType_SLOT_ID(tp_pos));
  case KWD___neg__: RETURN(DeeType_SLOT_ID(tp_neg));
  case TPP_TOK_INC: case KWD___inc__: RETURN(DeeType_SLOT_ID(tp_inc));
  case TPP_TOK_DEC: case KWD___dec__: RETURN(DeeType_SLOT_ID(tp_dec));
  case KWD___incpost__: RETURN(DeeType_SLOT_ID(tp_incpost));
  case KWD___decpost__: RETURN(DeeType_SLOT_ID(tp_decpost));
  case TPP_TOK_CMP_LO: case KWD___lo__: case KWD___lt__: RETURN(DeeType_SLOT_ID(tp_cmp_lo));
  case TPP_TOK_CMP_LE: case KWD___le__: RETURN(DeeType_SLOT_ID(tp_cmp_le));
  case TPP_TOK_CMP_EQ: case KWD___eq__: RETURN(DeeType_SLOT_ID(tp_cmp_eq));
  case TPP_TOK_CMP_NE: case KWD___ne__: RETURN(DeeType_SLOT_ID(tp_cmp_ne));
  case TPP_TOK_CMP_GR: case KWD___gr__: case KWD___gt__: RETURN(DeeType_SLOT_ID(tp_cmp_gr));
  case TPP_TOK_CMP_GE: case KWD___ge__: RETURN(DeeType_SLOT_ID(tp_cmp_ge));
  case '*': case KWD___mul__: RETURN(DeeType_SLOT_ID(tp_mul));
  case '/': case KWD___div__: RETURN(DeeType_SLOT_ID(tp_div));
  case '%': case KWD___mod__: RETURN(DeeType_SLOT_ID(tp_mod));
  case TPP_TOK_SHL: case KWD___shl__: RETURN(DeeType_SLOT_ID(tp_shl));
  case TPP_TOK_SHR: case KWD___shr__: RETURN(DeeType_SLOT_ID(tp_shr));
  case '&': case KWD___and__: RETURN(DeeType_SLOT_ID(tp_and));
  case '|': case KWD___or__: RETURN(DeeType_SLOT_ID(tp_or));
  case '^': case KWD___xor__: RETURN(DeeType_SLOT_ID(tp_xor));
  case TPP_TOK_POW: case KWD___pow__: RETURN(DeeType_SLOT_ID(tp_pow));
  case TPP_TOK_ADD_ASSIGN:     case KWD___iadd__: RETURN(DeeType_SLOT_ID(tp_iadd));
  case TPP_TOK_SUB_ASSIGN:     case KWD___isub__: RETURN(DeeType_SLOT_ID(tp_isub));
  case TPP_TOK_MUL_ASSIGN:     case KWD___imul__: RETURN(DeeType_SLOT_ID(tp_imul));
  case TPP_TOK_DIV_ASSIGN:     case KWD___idiv__: RETURN(DeeType_SLOT_ID(tp_idiv));
  case TPP_TOK_MOD_ASSIGN:     case KWD___imod__: RETURN(DeeType_SLOT_ID(tp_imod));
  case TPP_TOK_SHL_ASSIGN:     case KWD___ishl__: RETURN(DeeType_SLOT_ID(tp_ishl));
  case TPP_TOK_SHR_ASSIGN:     case KWD___ishr__: RETURN(DeeType_SLOT_ID(tp_ishr));
  case TPP_TOK_BIN_AND_ASSIGN: case KWD___iand__: RETURN(DeeType_SLOT_ID(tp_iand));
  case TPP_TOK_BIN_OR_ASSIGN:  case KWD___ior__:  RETURN(DeeType_SLOT_ID(tp_ior));
  case TPP_TOK_BIN_XOR_ASSIGN: case KWD___ixor__: RETURN(DeeType_SLOT_ID(tp_ixor));
  case TPP_TOK_POW_ASSIGN:     case KWD___ipow__: RETURN(DeeType_SLOT_ID(tp_ipow));
  case KWD___call__: RETURN(DeeType_SLOT_ID(tp_call));
  case '#': case KWD___size__: RETURN(DeeType_SLOT_ID(tp_seq_size));
  case KWD___getitem__: RETURN(DeeType_SLOT_ID(tp_seq_get));
  case KWD___delitem__: RETURN(DeeType_SLOT_ID(tp_seq_del));
  case KWD___setitem__: RETURN(DeeType_SLOT_ID(tp_seq_set));
  case KWD___getrange__: RETURN(DeeType_SLOT_ID(tp_seq_range_get));
  case KWD___delrange__: RETURN(DeeType_SLOT_ID(tp_seq_range_del));
  case KWD___setrange__: RETURN(DeeType_SLOT_ID(tp_seq_range_set));
  /*case KWD_in: NO!!! - nonsensical parameter order*/
  case KWD___contains__: RETURN(DeeType_SLOT_ID(tp_seq_contains));
  case KWD___getattr__: RETURN(DeeType_SLOT_ID(tp_attr_get));
  case KWD___delattr__: RETURN(DeeType_SLOT_ID(tp_attr_del));
  case KWD___setattr__: RETURN(DeeType_SLOT_ID(tp_attr_set));
  case KWD_for: RETURN(DeeType_SLOT_ID(tp_p_seq_iter_self));
  case KWD___iterself__: RETURN(DeeType_SLOT_ID(tp_seq_iter_self));
  case KWD___iternext__: RETURN(DeeType_SLOT_ID(tp_seq_iter_next));
  case KWD___hash__: RETURN(DeeType_SLOT_ID(tp_hash));
  case '(': {
   int result;
   if (!yield()) return -1;
   if (token.tk_id == ')') {
    if (!yield()) return -1;
    // call operator
    return DeeType_SLOT_ID(tp_call);
   }
   // Operator name parenthesis
   if ((result = _DeeLexer_ParseTypeSlotID(self,scope,flags)) == -1) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_LPAREN_IN_OPERATOR_NAME,(DeeObject *)self,token_ob,
     "Expected ')' to close parenthesis after '(' <operator_name> in operator name") == -1) return -1;
   } else if (!yield()) return -1;
   return result;
  }
  case KWD_del:
   if (!yield()) return -1;
   if (token.tk_id == '.') RETURN(DeeType_SLOT_ID(tp_attr_del));
   if (token.tk_id == '[') {
    if (!yield()) return -1;
    if (token.tk_id == ':') {
     if (!yield()) return -1;
     if (token.tk_id != ']') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_DEL_LBRACKET_COLLON_IN_OPERATOR_NAME,
       (DeeObject *)self,token_ob,"Expected ']' after 'operator' 'del' '[' ':'") == -1
       ) return -1;
     } else if (!yield()) return -1;
     return DeeType_SLOT_ID(tp_seq_range_del);
    } else if (token.tk_id != ']') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_DEL_LBRACKET_IN_OPERATOR_NAME,
      (DeeObject *)self,token_ob,"Expected ']' after 'operator' 'del' '['") == -1
      ) return -1;
    } else if (!yield()) return -1;
    return DeeType_SLOT_ID(tp_p_seq_del);
   }
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACKET_OR_DOT_AFTER_DEL_IN_OPERATOR_NAME,
    (DeeObject *)self,token_ob,"Expected '[' or '.' after 'operator' 'del'") == -1
    ) return -1;
   return DeeType_SLOT_ID(tp_attr_del); // ~shrug's sholders~
  case '.':
   if (!yield()) return -1;
   if (token.tk_id == '=') RETURN(DeeType_SLOT_ID(tp_attr_set));
   return DeeType_SLOT_ID(tp_attr_get);
  case '[':
   if (!yield()) return -1;
   if (token.tk_id == ':') {
    if (!yield()) return -1;
    if (token.tk_id != ']') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_COLLON_IN_OPERATOR_NAME,
      (DeeObject *)self,token_ob,"Expected ']' after 'operator' '[' ':'") == -1
      ) return -1;
    } else if (!yield()) return -1;
    if (token.tk_id == '=') RETURN(DeeType_SLOT_ID(tp_seq_range_set));
    return DeeType_SLOT_ID(tp_seq_range_get);
   } else if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_IN_OPERATOR_NAME,
     (DeeObject *)self,token_ob,"Expected ']' after 'operator' '['") == -1) return -1;
   } else if (!yield()) return -1;
   if (token.tk_id == '=') RETURN(DeeType_SLOT_ID(tp_p_seq_set));
   return DeeType_SLOT_ID(tp_p_seq_get);
#undef RETURN
  default:
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_OPERATOR_NAME_IN_CLASSDECL,
    (DeeObject *)self,token_ob,"Expected operator name after 'operator'") == -1
    ) return -1;
   break;
 }
 return 0;
}
unsigned int _Dee_TypeSlot_ToTokenID(int id) {
 switch (id) {
  case DeeType_SLOT_ID(tp_str): return KWD___str__;
  case DeeType_SLOT_ID(tp_repr): return KWD___repr__;
  case DeeType_SLOT_ID(tp_copy_ctor): return KWD___copy__;
  case DeeType_SLOT_ID(tp_move_ctor): return KWD___move__;
  case DeeType_SLOT_ID(tp_move_assign): return KWD___moveassign__;
  case DeeType_SLOT_ID(tp_copy_assign): return KWD___copyassign__;
  case DeeType_SLOT_ID(tp_any_assign): return KWD___assign__;
  case DeeType_SLOT_ID(tp_ctor): case DeeType_SLOT_ID(tp_any_ctor):
  case DeeType_SLOT_ID(tp_p_any_ctor): return KWD___constructor__;
  case DeeType_SLOT_ID(tp_dtor): return KWD___destructor__;
  case DeeType_SLOT_ID(tp_bool): return KWD___bool__;
  case DeeType_SLOT_ID(tp_double): return KWD___double__;
  case DeeType_SLOT_ID(tp_int32): return KWD___int32__;
  case DeeType_SLOT_ID(tp_int64): return KWD___int64__;
  case DeeType_SLOT_ID(tp_not): return KWD___not__;
  case DeeType_SLOT_ID(tp_add): case DeeType_SLOT_ID(tp_p_add): return KWD___add__;
  case DeeType_SLOT_ID(tp_sub): case DeeType_SLOT_ID(tp_p_sub): return KWD___sub__;
  case DeeType_SLOT_ID(tp_inv): return KWD___inv__;
  case DeeType_SLOT_ID(tp_pos): return KWD___pos__;
  case DeeType_SLOT_ID(tp_neg): return KWD___neg__;
  case DeeType_SLOT_ID(tp_inc): return KWD___inc__;
  case DeeType_SLOT_ID(tp_dec): return KWD___dec__;
  case DeeType_SLOT_ID(tp_incpost): return KWD___incpost__;
  case DeeType_SLOT_ID(tp_decpost): return KWD___decpost__;
  case DeeType_SLOT_ID(tp_cmp_lo): return KWD___lo__;
  case DeeType_SLOT_ID(tp_cmp_le): return KWD___le__;
  case DeeType_SLOT_ID(tp_cmp_eq): return KWD___eq__;
  case DeeType_SLOT_ID(tp_cmp_ne): return KWD___ne__;
  case DeeType_SLOT_ID(tp_cmp_gr): return KWD___gr__;
  case DeeType_SLOT_ID(tp_cmp_ge): return KWD___ge__;
  case DeeType_SLOT_ID(tp_mul): return KWD___mul__;
  case DeeType_SLOT_ID(tp_div): return KWD___div__;
  case DeeType_SLOT_ID(tp_mod): return KWD___mod__;
  case DeeType_SLOT_ID(tp_shl): return KWD___shl__;
  case DeeType_SLOT_ID(tp_shr): return KWD___shr__;
  case DeeType_SLOT_ID(tp_and): return KWD___and__;
  case DeeType_SLOT_ID(tp_or): return KWD___or__;
  case DeeType_SLOT_ID(tp_xor): return KWD___xor__;
  case DeeType_SLOT_ID(tp_pow): return KWD___pow__;
  case DeeType_SLOT_ID(tp_iadd): return KWD___iadd__;
  case DeeType_SLOT_ID(tp_isub): return KWD___isub__;
  case DeeType_SLOT_ID(tp_imul): return KWD___imul__;
  case DeeType_SLOT_ID(tp_idiv): return KWD___idiv__;
  case DeeType_SLOT_ID(tp_imod): return KWD___imod__;
  case DeeType_SLOT_ID(tp_ishl): return KWD___ishl__;
  case DeeType_SLOT_ID(tp_ishr): return KWD___ishr__;
  case DeeType_SLOT_ID(tp_iand): return KWD___iand__;
  case DeeType_SLOT_ID(tp_ior): return KWD___ior__;
  case DeeType_SLOT_ID(tp_ixor): return KWD___ixor__;
  case DeeType_SLOT_ID(tp_ipow): return KWD___ipow__;
  case DeeType_SLOT_ID(tp_call): return KWD___call__;
  case DeeType_SLOT_ID(tp_seq_size): return KWD___size__;
  case DeeType_SLOT_ID(tp_p_seq_get): case DeeType_SLOT_ID(tp_seq_get): return KWD___getitem__;
  case DeeType_SLOT_ID(tp_p_seq_del): case DeeType_SLOT_ID(tp_seq_del): return KWD___delitem__;
  case DeeType_SLOT_ID(tp_p_seq_set): case DeeType_SLOT_ID(tp_seq_set): return KWD___setitem__;
  case DeeType_SLOT_ID(tp_seq_range_get): return KWD___getrange__;
  case DeeType_SLOT_ID(tp_seq_range_del): return KWD___delrange__;
  case DeeType_SLOT_ID(tp_seq_range_set): return KWD___setrange__;
  case DeeType_SLOT_ID(tp_seq_contains): return KWD___contains__;
  case DeeType_SLOT_ID(tp_attr_get): return KWD___getattr__;
  case DeeType_SLOT_ID(tp_attr_del): return KWD___delattr__;
  case DeeType_SLOT_ID(tp_attr_set): return KWD___setattr__;
  case DeeType_SLOT_ID(tp_seq_iter_self): return KWD___iterself__;
  case DeeType_SLOT_ID(tp_seq_iter_next): return KWD___iternext__;
  case DeeType_SLOT_ID(tp_p_seq_iter_self): return KWD_for;
  default: return 0;
 }
}

DEE_STATIC_INLINE(int) _DeeLexer_SkipVisibilityModifiers(
 struct DeeLexerObject *self, struct DeeScopeObject *scope);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
int _DeeLexer_ParseClassGetSet(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags,
 DEE_A_OUT DeeExprAstObject **getter_, DEE_A_OUT DeeExprAstObject **delete_,
 DEE_A_OUT DeeExprAstObject **setter_) {
 TPPTokenID kind; // __get__, __del__, __set__
 DEE_ASSERT(token.tk_id == '{');
 DEE_ASSERT(getter_ && delete_ && setter_);
 *getter_ = NULL,*delete_ = NULL,*setter_ = NULL;
 if (!yield()) goto err;
 while (1) {
  struct DeeAttributeDecl callback_attr = DeeAttributeDecl_INIT();
  if (_DeeLexer_ParseAttributeDecl(&callback_attr,self,scope,flags) == -1) goto err_attr;
again:
  switch (token.tk_id) {
   case ';':
    do if (!yield()) goto err_attr; while (token.tk_id == ';');
    break;
   case '}':
    if (!yield()) goto err_attr;
    // fallthrough
   case 0:
    DeeAttributeDecl_Quit(&callback_attr);
    goto end;
   case KWD_function:
    if (DeeError_CompilerError(DEE_WARNING_FUNCTION_DECLARATION_NOT_ALLOWED_IN_GETSET_IN_CLASSDECL,
     (DeeObject *)self,token_ob,"Function declarations are not allowed in properties"
     ) == -1) goto err_attr;
    // fallthrough
   case KWD_default: case KWD_local: case KWD_public:
   case KWD_private: case KWD_protected: case KWD_operator:
y1: if (!yield()) {err_attr: DeeAttributeDecl_Quit(&callback_attr); goto err; }
    goto again;
   case '.':
    if (!yield()) goto err_attr;
    if (token.tk_id == '=') goto parse_setter;
    kind = KWD___get__;
    goto parse_callback;
   case '-': case KWD_del:
    if (!yield()) goto err_attr;
    if (token.tk_id == '.') goto parse_delete;
    kind = KWD___del__;
    goto parse_callback;
   case KWD_get: case KWD___get__: case KWD_read: /*parse_getter:*/ kind = KWD___get__; goto parse_callback_y1;
                 case KWD___del__: case KWD_delete: parse_delete: kind = KWD___del__; goto parse_callback_y1;
   case KWD_set: case KWD___set__: case '=': /*case KWD___assign__:*/ case KWD_write: parse_setter: kind = KWD___set__;
parse_callback_y1: if (!yield()) goto err_attr; parse_callback: {
    DeeExprAstObject *callback,**slot;
    if ((callback_attr.ob_fun_flags&DeeFunctionFlags_THISCALL)!=0) {
     if (DeeError_CompilerError(DEE_WARNING_EXPLICIT_THISCALL_ATTRIBUTE_IN_CLASSDECL,
      (DeeObject *)self,token_ob,"[[thiscall]] attribute was explicitly defined for property callback") == -1) goto err_attr;
    } else callback_attr.ob_fun_flags |= DeeFunctionFlags_THISCALL; // Always a thiscall
    if ((callback = new_ast(DeeExprAstKind_FUNCTION_EXPR,token_ob)) == NULL) goto err_attr;
    if (_DeeLexer_ParseFunctionSuffix(callback,self,scope,flags,&callback_attr,kind) == -1) {
err_callback_attr: Dee_DECREF(callback); goto err_attr;
    }
    if (DeeExprAst_NeedSemicollon((DeeObject *)callback)) {
     if (token.tk_id != ';') {
      if ((flags&DEE_PARSER_FLAG_CONST_FUNCTION)==0) {
       if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_GETSETCALLBACK_IN_CLASSDECL,
        (DeeObject *)self,token_ob,"Expected ';' after class property callback declaration: %s = %r",
        TPPKeyword_BuiltinToStr(kind),callback) == -1) goto err_callback_attr;
      }
     } else do if (!yield()) goto err_callback_attr; while (token.tk_id == ';');
    }
    switch (kind) {
     case KWD___get__: slot = getter_; break;
     case KWD___del__: slot = delete_; break;
     case KWD___set__: slot = setter_; break;
     default: slot = NULL; break;
    }
    if (slot) {
     if (*slot) {
      if (DeeError_CompilerErrorf(DEE_WARNING_GETSET_CALLBACK_ALREADY_DEFINED_IN_CLASSDECL,(DeeObject *)self,token_ob,
       "property callback %q was already defined\n%s(%d) : See reference to previous definition",
       TPPKeyword_BuiltinToStr(kind),DeeToken_FILE((*slot)->ob_token),DeeToken_LINE((*slot)->ob_token)+1) == -1
       ) goto err_callback_attr;
      Dee_DECREF(*slot);
     }
     Dee_INHERIT_REF(*slot,callback);
    } else Dee_DECREF(callback); 
   } break;

   default:
    if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_PROPERTY_DECL,
     (DeeObject *)self,token_ob,"Unexpected token in class property decl") == -1) goto err;
    goto y1;
  }
  DeeAttributeDecl_Quit(&callback_attr);
 }
end:
 DEE_LVERBOSE2("Parsed property: { get = %O; del = %O; set = %O; }\n",
               *getter_,*delete_,*setter_);
 return 0;
err:
 Dee_XDECREF(*getter_);
 Dee_XDECREF(*delete_);
 Dee_XDECREF(*setter_);
 return -1;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *DeeClassDecl_MakeConstantClass(
 DEE_A_INOUT_OBJECT_OPT(struct DeeClassDeclObject) const *self) {
 DeeTypeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeClassDecl_Check(self));
 DEE_ASSERT(!DeeClassDecl_BASE(self) || DeeClassDecl_BASE(self)->ob_kind == DeeExprAstKind_CONSTANT);
 DEE_LVERBOSE2("Packing constant class: %O\n",DeeClassDecl_NAME(self));
 if ((result = DeeClass_NewObject(DeeClassDecl_BASE(self)
  ? (DeeTypeObject *)DeeClassDecl_BASE(self)->ob_const : &DeeObject_Type,
  DeeClassDecl_NAME(self))) != NULL) {
  struct DeeClassDeclEntry *iter,*end;
  end = (iter = DeeClassDecl_ELEM(self))+DeeClassDecl_SIZE(self);
  while (iter != end) {
   switch (iter->ob_kind) {
    case DeeClassDeclEntryKind_SLOT:
     DEE_ASSERT(DeeObject_Check(iter->ob_slot.ob_callback) && DeeExprAst_Check(iter->ob_slot.ob_callback));
     DEE_ASSERT(iter->ob_slot.ob_callback->ob_kind == DeeExprAstKind_CONSTANT);
     if (DeeClass_SetSlot(result,iter->ob_slot.ob_slotid,iter->ob_slot.ob_callback->ob_const) == -1) goto err_r;
     break;
    case DeeClassDeclEntryKind_METHOD:
     DEE_ASSERT(DeeObject_Check(iter->ob_method.ob_callback) && DeeExprAst_Check(iter->ob_method.ob_callback));
     DEE_ASSERT(iter->ob_method.ob_callback->ob_kind == DeeExprAstKind_CONSTANT);
     if (DeeClass_AddMethod(result,(DeeObject *)iter->ob_method.ob_name,
      iter->ob_method.ob_callback->ob_const) == -1) {
err_r: Dee_DECREF(result); return NULL;
     }
     break;
    case DeeClassDeclEntryKind_GETSET:
     DEE_ASSERT(!iter->ob_getset.ob_get || (DeeObject_Check(iter->ob_getset.ob_get) && DeeExprAst_Check(iter->ob_getset.ob_get)));
     DEE_ASSERT(!iter->ob_getset.ob_del || (DeeObject_Check(iter->ob_getset.ob_del) && DeeExprAst_Check(iter->ob_getset.ob_del)));
     DEE_ASSERT(!iter->ob_getset.ob_set || (DeeObject_Check(iter->ob_getset.ob_set) && DeeExprAst_Check(iter->ob_getset.ob_set)));
     DEE_ASSERT(!iter->ob_getset.ob_get || (iter->ob_getset.ob_get->ob_kind == DeeExprAstKind_CONSTANT));
     DEE_ASSERT(!iter->ob_getset.ob_del || (iter->ob_getset.ob_del->ob_kind == DeeExprAstKind_CONSTANT));
     DEE_ASSERT(!iter->ob_getset.ob_set || (iter->ob_getset.ob_set->ob_kind == DeeExprAstKind_CONSTANT));
     if (DeeClass_AddGetSet(result,(DeeObject *)iter->ob_getset.ob_name,
      iter->ob_getset.ob_get ? iter->ob_getset.ob_get->ob_const : NULL,
      iter->ob_getset.ob_del ? iter->ob_getset.ob_del->ob_const : NULL,
      iter->ob_getset.ob_set ? iter->ob_getset.ob_set->ob_const : NULL) == -1) goto err_r;
     break;
    case DeeClassDeclEntryKind_CLASS_METHOD:
     DEE_ASSERT(DeeObject_Check(iter->ob_method.ob_callback) && DeeExprAst_Check(iter->ob_method.ob_callback));
     DEE_ASSERT(iter->ob_method.ob_callback->ob_kind == DeeExprAstKind_CONSTANT);
     if (DeeClass_AddClassMethod(result,(DeeObject *)iter->ob_method.ob_name,
      iter->ob_method.ob_callback->ob_const) == -1) goto err_r;
     break;
    case DeeClassDeclEntryKind_CLASS_GETSET:
     DEE_ASSERT(!iter->ob_getset.ob_get || (DeeObject_Check(iter->ob_getset.ob_get) && DeeExprAst_Check(iter->ob_getset.ob_get)));
     DEE_ASSERT(!iter->ob_getset.ob_del || (DeeObject_Check(iter->ob_getset.ob_del) && DeeExprAst_Check(iter->ob_getset.ob_del)));
     DEE_ASSERT(!iter->ob_getset.ob_set || (DeeObject_Check(iter->ob_getset.ob_set) && DeeExprAst_Check(iter->ob_getset.ob_set)));
     DEE_ASSERT(!iter->ob_getset.ob_get || (iter->ob_getset.ob_get->ob_kind == DeeExprAstKind_CONSTANT));
     DEE_ASSERT(!iter->ob_getset.ob_del || (iter->ob_getset.ob_del->ob_kind == DeeExprAstKind_CONSTANT));
     DEE_ASSERT(!iter->ob_getset.ob_set || (iter->ob_getset.ob_set->ob_kind == DeeExprAstKind_CONSTANT));
     if (DeeClass_AddClassGetSet(result,(DeeObject *)iter->ob_getset.ob_name,
      iter->ob_getset.ob_get ? iter->ob_getset.ob_get->ob_const : NULL,
      iter->ob_getset.ob_del ? iter->ob_getset.ob_del->ob_const : NULL,
      iter->ob_getset.ob_set ? iter->ob_getset.ob_set->ob_const : NULL) == -1) goto err_r;
     break;
    default: break;
   }
   ++iter;
  }
 }
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP

#ifndef __INTELLISENSE__
#define CONST_CLASS
#include "__parse_classdecl.inl"
#include "__parse_classdecl.inl"
#endif

DeeTypeObject *_DeeLexer_ParseConstClass(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTypeObject *result,*class_base; DeeObject *class_name;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 TPPTokenID class_name_id; char const *class_name_str;
 DEE_ASSERT(token.tk_id == KWD_class);
 if (!yield()) {
err_attr: DeeAttributeDecl_Quit(&attr);
  return NULL;
 }
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
 if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
  // Named class
  class_name_id = token.tk_id;
  class_name_str = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id);
  DEE_ASSERT(class_name_str);
  if ((class_name = DeeString_New(class_name_str)) == NULL) goto err_attr;
  if (!yield()) {err_name: Dee_XDECREF(class_name); goto err_attr; }
 } else { class_name = NULL; class_name_id = 0; }
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_name;
 if (DEE_PARSE_TOKENID_IS_BASE_INDICATOR(token.tk_id)) {
  DeeExprAstObject *class_base_ast;
  if (!yield()) goto err_name;
  if ((class_base_ast = _deeparse_top(self,scope,flags|DEE_PARSER_FLAG_NO_BRACE_INIT)) == NULL) goto err_attr;
  if (class_base_ast->ob_kind != DeeExprAstKind_CONSTANT) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BASE_IN_CONSTANT_CLASSDECL,
    (DeeObject *)self,(DeeObject *)class_base_ast->ob_token,
    "Expected constant base in constant class declaration (got: %r)",
    class_base_ast) == -1) {err_cb_attr: Dee_DECREF(class_base_ast); goto err_attr; }
default_base_drop_base:
   Dee_DECREF(class_base_ast);
   goto default_base;
  } else if (DeeNone_Check(class_base_ast->ob_const)) {
   goto default_base_drop_base;
  } else if (!DeeType_Check(class_base_ast->ob_const)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_BASE_IN_CONSTANT_CLASSDECL,
    (DeeObject *)self,(DeeObject *)class_base_ast->ob_token,
    "Expected type for base in constant class declaration (got: %q)",
    DeeType_NAME(Dee_TYPE(class_base_ast->ob_const))) == -1) goto err_cb_attr;
   Dee_INCREF(class_base = Dee_TYPE(class_base_ast->ob_const));
  } else {
   Dee_INCREF(class_base = (DeeTypeObject *)class_base_ast->ob_const);
  }
  Dee_DECREF(class_base_ast);
 } else {
default_base:
  Dee_INCREF(class_base = (DeeTypeObject *)&DeeObject_Type);
 }
 result = DeeClass_NewObject(class_base,class_name);
 Dee_XDECREF(class_name);
 Dee_DECREF(class_base);
 if (!result) goto err_attr;
 if (class_name_id) {
  if (_DeeScope_HasName(scope,class_name_id)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_CLASS_NAME_ALREADY_USED,
    (DeeObject *)self,token_ob,"Class name %q is already used",
    TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id)) == -1) goto err_r;
  } else if (_DeeScope_AddDeprecatedName(scope,class_name_id,
   (DeeObject *)result,(DeeObject *)attr.ob_depr) == -1) goto err_r;
 }

 if (token.tk_id == '{') {
  if (_DeeLexer_ParseConstClassDecl(result,self,scope,flags,class_name_id,&attr) == -1) {
err_r: Dee_DECREF(result); goto err_attr;
  }
 }
 if (attr.ob_name && DeeType_SetName(
  result,DeeString_STR(attr.ob_name))) goto err_r;
 DeeAttributeDecl_Quit(&attr);
 return result;
}
DeeExprAstObject *_DeeLexer_ParseClass(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result,*class_ast,*class_base; struct DeeClassDeclObject *decl;
 DeeObject *class_name; DeeLocalVarObject *class_var; unsigned int old_uses;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 TPPTokenID class_name_id; char const *class_name_str;
 int class_is_const_candidate = 1;
 DEE_ASSERT(token.tk_id == KWD_class);
 if ((class_ast = new_ast(DeeExprAstKind_CLASS,token_ob)) == NULL) goto err_attr;
 if (!yield()) {
err_c: Dee_DECREF(class_ast);
err_attr: DeeAttributeDecl_Quit(&attr);
  return NULL;
 }
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_c;
 if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
  // Named class
  class_name_id = token.tk_id;
  if ((result = new_ast(DeeExprAstKind_VARDECL,token_ob)) == NULL) goto err_c;
  result->ob_op_b = class_ast; // Inherit reference
  if ((result->ob_op_a = new_ast(DeeExprAstKind_DECLVARIABLE,token_ob)) == NULL) goto err_r;
  class_var = (DeeLocalVarObject *)DeeScope_Lookup(
   (DeeObject *)scope,class_name_id,
   DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_CONST,NULL);
  if (class_var) {
   result->ob_kind = DeeExprAstKind_ASSIGN;
   result->ob_op_a->ob_kind = DeeExprAstKind_VARIABLE;
  } else {
   DeeObject *depr_reason = (DeeObject *)attr.ob_depr;
   class_var = (DeeLocalVarObject *)DeeScope_Lookup(
    (DeeObject *)scope,class_name_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,&depr_reason);
   if (!class_var) goto err_r;
  }
  if (!DeeLocalVar_Check(class_var)) {
   if (DeeError_CompilerError(DEE_WARNING_CLASS_NAME_ALREADY_USED,(DeeObject *)self,token_ob,
    "Can't use this name for a class (already used as module/constant)") == -1) goto err_r;
   if (!yield()) goto err_r;
   result->ob_op_b = NULL;
   Dee_DECREF(result);
   goto no_name;
  }
  class_var->ob_flags |= DEE_LOCAL_VAR_FLAG_AST_INITIALIZED;
  Dee_INCREF(result->ob_op_a->ob_var = class_var);
  if (result->ob_op_a->ob_kind == DeeExprAstKind_VARIABLE) ++class_var->ob_uses;
  class_name_str = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),class_name_id);
  DEE_ASSERT(class_name_str);
  if ((class_name = DeeString_New(class_name_str)) == NULL) goto err_r;
  if (!yield()) {err_rname: Dee_XDECREF(class_name); goto err_r; }
 } else {no_name: result = class_ast; class_name = NULL; class_var = NULL; class_name_id = 0; }
 if (class_var) old_uses = class_var->ob_uses;
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_rname;
 if (DEE_PARSE_TOKENID_IS_BASE_INDICATOR(token.tk_id)) {
  DeeTypeObject *class_base_type;
  if (!yield()) goto err_rname;
  if ((class_base = _deeparse_top(self,scope,flags|DEE_PARSER_FLAG_NO_BRACE_INIT)) == NULL) return NULL;
  if (class_base->ob_kind == DeeExprAstKind_CONSTANT && DeeNone_Check(class_base->ob_const)) {
   Dee_DECREF(class_base->ob_const);
   Dee_INCREF(class_base->ob_const = Dee_None);
  }
  if ((class_base_type = DeeExprAst_PredictType(
   (DeeObject *)class_base,(DeeObject *)scope)) != NULL && !DeeType_Check(class_base_type)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_BASE_IN_CLASSDECL,
    (DeeObject *)self,(DeeObject *)class_base->ob_token,
    "Expected type for class base (got %s). This will throw an Error.TypeError at runtime",
    DeeType_NAME(class_base_type)) == -1) goto err_r;
  }
  if (!DeeExprAst_IsNoeffect((DeeObject *)class_base,(DeeObject *)scope)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_CLASS_BASE_EXPRESSION_HAS_SIDE_EFFECTS_IN_CLASSDECL,
    (DeeObject *)self,(DeeObject *)class_base->ob_token,
    "Class base has side-effects but may be evaluated more than once: %r",class_base) == -1) goto err_r;
  }
  if (class_base->ob_kind != DeeExprAstKind_CONSTANT) class_is_const_candidate = 0;
 } else class_base = NULL;
 class_ast->ob_classdecl = (struct DeeClassDeclObject *)
  DeeClassDecl_New(class_name,(DeeObject *)class_base);
 Dee_XDECREF(class_name);
 Dee_XDECREF(class_base);
 if (!class_ast->ob_classdecl) {err_r: Dee_DECREF(result); goto err_attr; }
 decl = class_ast->ob_classdecl;
 if (token.tk_id == '{') {
  if (_DeeLexer_ParseClassDecl(decl,self,scope,flags,
   class_name_id,&class_is_const_candidate,&attr) == -1
   ) goto err_r;
 }
 if (attr.ob_name) {
  Dee_XDECREF(decl->ob_name);
  Dee_INCREF(decl->ob_name = attr.ob_name);
 }
 DeeAttributeDecl_Quit(&attr);
 if (class_var) class_var->ob_uses = old_uses;
 if (class_is_const_candidate &&
    (flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0) {
  DeeTypeObject *constant_class; // Create a constant class
  constant_class = DeeClassDecl_MakeConstantClass((DeeObject *)decl);
  if (!constant_class) goto err_r;
  Dee_DECREF(result->ob_op_a);
  Dee_XCLEAR(result->ob_op_b);
  result->ob_kind = DeeExprAstKind_CONSTANT;
  Dee_INHERIT_REF(result->ob_const,*(DeeObject **)&constant_class);
 }
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP
#endif

DeeExprAstObject *_DeeLexer_ParseList(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result;
 DeeTokenObject *tok;
 DEE_ASSERT(token.tk_id == '[');
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
 if (!yield()) goto err_0;
 if (token.tk_id == ']') {
  if (!yield()) goto err_0;
  if ((flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE) != 0) {
   result = new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)tok);
   if (result && (result->ob_const = DeeList_New()) == NULL) {
err_r:
    Dee_DECREF(result);
err_0:
    result = NULL;
   }
  } else {
   result = new_ast(DeeExprAstKind_LIST,(DeeObject *)tok);
   if (result) Dee_INCREF(result->ob_args_a = (DeeTupleObject *)Dee_EmptyTuple);
  }
  Dee_DECREF(tok);
 } else if ((result = _deeparse_comma_tuple(self,scope,flags)) != NULL) {
  if (token.tk_id != ']') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_LIST,
    (DeeObject *)self,token_ob,"Expected ']' to close list") == -1) goto err_r;
  } else if (!yield()) goto err_r;
  if (result->ob_kind == DeeExprAstKind_CONSTANT) {
   DeeObject *new_const;
   DEE_ASSERT(DeeTuple_Check(result->ob_const));
   if ((new_const = DeeList_NewFromVector(
    DeeTuple_SIZE(result->ob_const),
    DeeTuple_ELEM(result->ob_const))) == NULL) goto err_r;
   Dee_DECREF(result->ob_const);
   Dee_INHERIT_REF(result->ob_const,new_const);
  } else {
   DEE_ASSERT(result->ob_kind == DeeExprAstKind_TUPLE);
   result->ob_kind = DeeExprAstKind_LIST;
  }
  Dee_DECREF(result->ob_token);
  Dee_INHERIT_REF(result->ob_token,tok);
 }
 return result;
}
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
DeeExprAstObject *_DeeLexer_ParseTupleOrParenOrCode(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, int *is_double_paren)
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
DeeExprAstObject *_DeeLexer_ParseTupleOrParen(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, int *is_double_paren)
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
{
 DeeExprAstObject *result,*temp;
 DeeTokenObject *tok;
 DEE_ASSERT(is_double_paren);
 DEE_ASSERT(token.tk_id == '(');
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
 if (!yield()) goto err_0;
 *is_double_paren = token.tk_id == '(';
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
 if (token.tk_id == '{') {
  // Code expression
  if (!yield()) goto err_0;
  if (token.tk_id == '}') {
   if (!yield()) goto err_0;
ret_none:
   if ((result = new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)tok)) == NULL) goto err_0;
   Dee_INCREF(result->ob_const = Dee_None);
  } else {
   DeeScopeObject *new_scope;
   DeeStmtAstObject *code_stmt;
   if ((new_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_0;
   if ((code_stmt = (DeeStmtAstObject *)DeeLexer_ParseStatements(
    (DeeObject *)self,(DeeObject *)new_scope,flags,'}')) == NULL) {
    Dee_DECREF(new_scope);
    goto err_0;
   }
   if (code_stmt->ob_kind == DeeStmtAstKind_EMPTY) {
    // Simplify empty statement expression --> constant none
    Dee_DECREF(code_stmt);
    Dee_DECREF(new_scope);
    goto ret_none;
   } else if (code_stmt->ob_kind == DeeStmtAstKind_EXPR &&
              code_stmt->ob_expr.ob_expr_ast->ob_kind == DeeExprAstKind_CONSTANT) {
    // Simplify Constant statement expression --> constant expression
    Dee_INCREF(result = code_stmt->ob_expr.ob_expr_ast);
    Dee_DECREF(code_stmt);
    Dee_DECREF(new_scope);
   } else { // Dynamic statement expression
    if ((result = new_ast(DeeExprAstKind_CODE,(DeeObject *)tok)) == NULL) {
     Dee_DECREF(code_stmt);
     Dee_DECREF(new_scope);
     goto err_0;
    }
    Dee_INHERIT_REF(result->ob_stmt,code_stmt);
    Dee_INHERIT_REF(result->ob_scope,new_scope);
   }
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_EXPR_STATEMENT,(DeeObject *)self,token_ob,
    "Expected ')' to close code expression") == -1) goto err_r;
  } else if (!yield()) goto err_r;
  goto end;
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
 if ((result = _deeparse_top_lv2(self,scope,flags)) == NULL) { err_0: result = NULL; goto end; }
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_TUPLE,(DeeObject *)self,token_ob,"Expected ')' to close parenthesis / tuple") == -1) goto err_r;
 } else if (!yield()) {err_r: Dee_DECREF(result); goto err_0; }
 if (result->ob_kind == DeeExprAstKind_EXPAND) {
  // Convert (x...) -> (pack x...)
  if ((temp = new_ast(DeeExprAstKind_TUPLE,(DeeObject *)tok)) == NULL) goto err_r;
  if ((temp->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,result)) == NULL) { Dee_DECREF(temp); goto err_r; }
  Dee_DECREF(result);
  Dee_INHERIT_REF(result,temp);
 }
end:
 Dee_DECREF(tok);
 return result;
}

#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
DeeExprAstObject *_DeeLexer_ParseIfExpr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result,*cond_branch,*true_branch,*false_branch;
 DeeTokenObject *tok;
 DEE_ASSERT(token.tk_id == KWD_if);
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
 if (!yield()) {err_tok: Dee_DECREF(tok); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_EXPR_IF,(DeeObject *)self,token_ob,
   "Expected '(' after 'if'") == -1) goto err_tok;
 } else if (!yield()) goto err_tok;
 if ((cond_branch = _deeparse_top(self,scope,flags)) == NULL) goto err_tok;
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_EXPR_IF,(DeeObject *)self,token_ob,
   "Expected ')' after 'if' '(' <expr>") == -1) goto err_condb;
 } else if (!yield()) {err_condb: Dee_DECREF(cond_branch); goto err_tok; }
 if (cond_branch->ob_kind == DeeExprAstKind_CONSTANT &&
    (flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0 &&
    DEE_HAVE_CONSTOP_OPTIMIZATION(cond_branch->ob_const)) {
  int is_true; // Dead branch optimization
  if ((is_true = DeeObject_Bool(cond_branch->ob_const)) == -1) {
   if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_COND,(DeeObject *)self,(DeeObject *)cond_branch->ob_token,
    "Invalid constant operator: Using '%#q' as condition in if-expression will throw an '%K' at runtime",
    DeeType_NAME(Dee_TYPE(cond_branch->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_condb;
  } else {
   if (token.tk_id == KWD_else) {
    if (!yield() || (false_branch = _deeparse_top(
     self,scope,(Dee_uint32_t)(is_true ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)
     )) == NULL) goto err_condb;
    Dee_DECREF(tok);
    if (is_true) {
     Dee_DECREF(false_branch);
     return cond_branch;
    } else {
     Dee_DECREF(cond_branch);
     return false_branch;
    }
   }
   Dee_DECREF(cond_branch);
   if ((true_branch = _deeparse_top(
    self,scope,(Dee_uint32_t)(is_true ? flags : flags|DEE_PARSER_FLAG_UNREACHABLE)
    )) == NULL) goto err_condb;
   if (token.tk_id == KWD_else) {
    if (!yield() || (false_branch = _deeparse_top(
     self,scope,(Dee_uint32_t)(is_true ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)
     )) == NULL) goto err_trueb;
   } else {
    false_branch = NULL;
   }
   Dee_DECREF(tok);
   if (is_true) {
    Dee_XDECREF(false_branch);
   } else {
    if (false_branch) { Dee_DECREF(true_branch); return false_branch; }
    Dee_XCLEAR(true_branch->ob_op_a);
    Dee_XCLEAR(true_branch->ob_op_b);
    Dee_XCLEAR(true_branch->ob_op_c);
    Dee_XCLEAR(true_branch->ob_op_d);
    true_branch->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INCREF(true_branch->ob_const = Dee_None);
   }
   return true_branch;
  }
 }

 if (token.tk_id == KWD_else) {
  // Use the condition branch as true statement
  Dee_INCREF(true_branch = cond_branch);
prs_else: // Parse the else branch
  if (!yield() || (false_branch = _deeparse_top(self,scope,flags)) == NULL) {
err_trueb: Dee_DECREF(true_branch); goto err_condb;
  }
 } else {
  if ((true_branch = _deeparse_top(self,scope,flags)) == NULL) goto err_condb;
  if (token.tk_id == KWD_else) goto prs_else; else false_branch = NULL;
 }
 // Regular if expression
 result = new_ast(DeeExprAstKind_COND,(DeeObject *)tok);
 if (!result) { Dee_XDECREF(false_branch); goto err_trueb; }
 Dee_INHERIT_REF(result->ob_op_a,cond_branch);
 Dee_INHERIT_REF(result->ob_op_b,true_branch);
 Dee_INHERIT_REF(result->ob_op_c,false_branch);
 Dee_DECREF(tok);
 return result;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
static DeeStmtAstObject *_DeeLexer_ParseSingleYieldExpressionInOldScope(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeStmtAstObject *result;
 DeeExprAstObject *yield_expr;
 if ((result = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(token_ob)) == NULL) return NULL;
 if (token.tk_id == KWD_if) {
  DeeStmtAstObject *used_stmt;
  DeeScopeObject *used_scope;
  int expr_is_true; // 0: constant false; 1: constant true; -1: unknown
  // Special syntax for conditional generators:
  // >> print (for (x: util::range(20)) if (x%2 == 0) x)...; // Only yields every second number
  // This behaves the same to 'locate_all' as generators towards 'transform'
  if (!yield()) goto err_r_0;
  if (token.tk_id != '(') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_IF_IN_GENERATOR,
    (DeeObject *)self,token_ob,"Expected '(' after 'if' in generator") == -1) goto err_r_0;
  } else if (!yield()) goto err_r_0;
  result->ob_kind = DeeStmtAstKind_IF;
  if ((result->ob_if.ob_if_thenscope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_r_0;
  if ((result->ob_if.ob_if_expr = _DeeLexer_ParseConditionalExpressionLv2(
   self,result->ob_if.ob_if_thenscope,flags)) == NULL) {
err_rif_1: Dee_DECREF(result->ob_if.ob_if_thenscope); goto err_r_0;
  }
  if (result->ob_if.ob_if_expr->ob_kind == DeeExprAstKind_CONSTANT &&
     (flags & DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
      DEE_HAVE_CONSTOP_OPTIMIZATION(result->ob_if.ob_if_expr->ob_const)) {
   DeeObject *temp_copy;
   if ((temp_copy = DeeObject_DeepCopy(result->ob_if.ob_if_expr->ob_const)) == NULL)
    expr_is_true = -1,DeeError_Handled(); else {
    expr_is_true = DeeObject_Bool(temp_copy);
    Dee_DECREF(temp_copy);
    if (expr_is_true == -1 && DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_BOOL,(
     DeeObject *)self,(DeeObject *)result->ob_if.ob_if_expr->ob_token,
     "Invalid constant operator: Calling 'if (%#q)' will throw an '%K' at runtime",
     DeeType_NAME(Dee_TYPE(result->ob_if.ob_if_expr->ob_const)),_DeeError_HandleReturnType()
     ) == -1) goto err_rif_2;
   }
  } else expr_is_true = -1;
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_IF_IN_GENERATOR,
    (DeeObject *)self,token_ob,"Expected ')' after 'if' '(' <expr> in generator") == -1) goto err_r_0;
  } else if (!yield()) {
err_rif_2: Dee_DECREF(result->ob_if.ob_if_expr); goto err_rif_1;
  }
  if ((result->ob_if.ob_if_thenstmt = _DeeLexer_ParseSingleYieldExpressionInOldScope(
   self,result->ob_if.ob_if_thenscope,expr_is_true == 0 ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)) == NULL) goto err_rif_2;
  // Optional else branch
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
  if (token.tk_id == KWD_elif) { token.tk_id = KWD_if; goto parse_opelse; } else
#endif
  if (token.tk_id == KWD_else) { if (!yield()) goto err_rif_2;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
parse_opelse:
#endif
   if ((result->ob_if.ob_if_elsescope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_rif_2;
   if ((result->ob_if.ob_if_elsestmt = _DeeLexer_ParseSingleYieldExpressionInOldScope(
    self,result->ob_if.ob_if_elsescope,expr_is_true == 1 ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)) == NULL) {
/*err_rif_3:*/ Dee_DECREF(result->ob_if.ob_if_thenscope); goto err_rif_2;
   }
  } else {
   result->ob_if.ob_if_elsescope = NULL;
   result->ob_if.ob_if_elsestmt = NULL;
  }
  if (expr_is_true == 1) {
   // Only use the true branch
   used_stmt = result->ob_if.ob_if_thenstmt;
   used_scope = result->ob_if.ob_if_thenscope;
   Dee_DECREF(result->ob_if.ob_if_expr);
   Dee_XDECREF(result->ob_if.ob_if_elsestmt);
   Dee_XDECREF(result->ob_if.ob_if_elsescope);
   result->ob_kind = DeeStmtAstKind_SSTMT;
   result->ob_sstmt.ob_sstmt_ast = used_stmt;
   result->ob_sstmt.ob_sstmt_scope = used_scope;
  } else if (expr_is_true == 0) {
   // Only use the false branch (or nothing)
   used_stmt = result->ob_if.ob_if_elsestmt;
   used_scope = result->ob_if.ob_if_elsescope;
   Dee_DECREF(result->ob_if.ob_if_expr);
   Dee_XDECREF(result->ob_if.ob_if_thenstmt);
   Dee_XDECREF(result->ob_if.ob_if_thenscope);
   DEE_ASSERT((used_stmt != NULL) == (used_scope != NULL));
   if (used_stmt) {
    result->ob_kind = DeeStmtAstKind_SSTMT;
    result->ob_sstmt.ob_sstmt_ast = used_stmt;
    result->ob_sstmt.ob_sstmt_scope = used_scope;
   } else result->ob_kind = DeeStmtAstKind_EMPTY;
  }
 } else {
  if ((yield_expr = (DeeExprAstObject *)DeeLexer_ParseExpression(
   (DeeObject *)self,(DeeObject *)scope,flags)) == NULL) {
err_r_0: _DeeStmtAst_Free(result); return NULL;
  }
  result->ob_kind = DeeStmtAstKind_YIELD;
  result->ob_yield.ob_yld_exprs = (DeeTupleObject *)DeeTuple_Pack(1,yield_expr);
  Dee_DECREF(yield_expr);
  if (!result->ob_yield.ob_yld_exprs) goto err_r_0;
 }
 return result;
}
static DeeStmtAstObject *_DeeLexer_ParseSingleYieldExpressionInNewScope(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeStmtAstObject *result,*yield_stmt;
 if ((result = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(token_ob)) == NULL) return NULL;
 if ((scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) {
err_r_0: _DeeStmtAst_Free(result); return NULL;
 }
 while ((result->ob_stmts.ob_stmts_v = (struct DeeStmtAstObject **)
  malloc_nz(sizeof(struct DeeStmtAstObject *))) == NULL) {
  if (Dee_CollectMemory()) continue;
  DeeError_NoMemory();
err_r_1: Dee_DECREF(scope); goto err_r_0;
 }
 if ((yield_stmt = _DeeLexer_ParseSingleYieldExpressionInOldScope(self,scope,flags)) == NULL) goto err_r_1;
 result->ob_kind = DeeStmtAstKind_STMTS;
 result->ob_stmts.ob_stmts_c = 1;
 Dee_INHERIT_REF(result->ob_stmts.ob_stmts_v[0],yield_stmt);
 Dee_INHERIT_REF(result->ob_stmts.ob_stmts_scope,scope);
 return result;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DeeExprAstObject *_DeeLexer_ParseForGenerator(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeStmtAstKind loop_mode; TPPTokenID old_func_name;
 DeeExprAstObject *result,*function_ast;
 TPPTokenID loop_var;
 DeeStmtAstObject *stmt;
 DEE_ASSERT(token.tk_id == KWD_for || token.tk_id == KWD___foreach);
 loop_mode = token.tk_id == KWD___foreach ? DeeStmtAstKind_FOREACH : DeeStmtAstKind_FOR_IN;
 if ((result = new_ast(DeeExprAstKind_CALL,token_ob)) == NULL) return NULL;
 if ((result->ob_op_a = new_ast(DeeExprAstKind_YFUNCTION,token_ob)) == NULL) goto err_r;
 if ((result->ob_op_b = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_r;
 if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
 Dee_INCREF(result->ob_op_b->ob_const = Dee_EmptyTuple);
 function_ast = result->ob_op_a;
 if ((function_ast->ob_fun_name = (DeeStringObject *)DeeString_Newf(
  "%s:generator",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),self->ob_func_name))) == NULL) goto err_r;
 old_func_name = self->ob_func_name;
 if ((self->ob_func_name = TPPKeywordList_GetS(&DeeLexer_LEXER(self)->l_keywords,DeeString_STR(
  function_ast->ob_fun_name),DeeString_SIZE(function_ast->ob_fun_name),1)) == 0) {
err_fn_r: self->ob_func_name = old_func_name; goto err_r;
 }
 if (token.tk_id != '(') {
  if (loop_mode == DeeStmtAstKind_FOREACH) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_FOREACH,
    (DeeObject *)self,token_ob,"Expected '(' after generator-style '__foreach'") == -1) goto err_fn_r;
  } else {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_FOR,
    (DeeObject *)self,token_ob,"Expected '(' after generator-style 'for'") == -1) goto err_fn_r;
  }
 } else if (!yield()) goto err_fn_r;
 if (loop_mode == DeeStmtAstKind_FOREACH) {
  loop_var = token.tk_id;
  if (loop_var == KWD_local) { if (!yield()) goto err_fn_r; loop_var = token.tk_id; }
  if (!TPPTokenID_IS_KEYWORD(loop_var)) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_GENERATOR_FOREACH,
    (DeeObject *)self,token_ob,"Expected <keyword> after generator-style '__foreach' '('") == -1) goto err_fn_r;
   loop_var = KWD___anonymous__;
  } else if (!yield()) goto err_fn_r;
  if (token.tk_id != ':' && token.tk_id != KWD_in) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_GENERATOR_FOREACH,
    (DeeObject *)self,token_ob,"Expected ':' after generator-style '__foreach' '(' <keyword>") == -1) goto err_fn_r;
  } else if (!yield()) goto err_fn_r;
 } else if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  DeeTokenObject *temp_token,*temp_token2; // Check for for-in style loop
  Dee_INCREF(temp_token = (DeeTokenObject *)token_ob);
  if (token.tk_id == KWD_local) {
   if (!yield()) goto err_temptk_r2;
   Dee_INCREF(temp_token2 = (DeeTokenObject *)token_ob);
  } else temp_token2 = NULL;
  if (!yield()) {
err_temptk_r: Dee_XDECREF(temp_token2);
err_temptk_r2: Dee_DECREF(temp_token); goto err_fn_r;
  }
  if (token.tk_id == ':' || token.tk_id == KWD_in) {
   if (!yield()) goto err_temptk_r;
   loop_var = temp_token2 // for-in style loop
    ? temp_token2->ob_token.tk_id
    : temp_token->ob_token.tk_id;
  } else {
   if (temp_token2 && DeeLexer_Return((DeeObject *)self,(DeeObject *)temp_token2) == -1) goto err_temptk_r;
   if (DeeLexer_Return((DeeObject *)self,(DeeObject *)temp_token) == -1) goto err_temptk_r;
   loop_mode = DeeStmtAstKind_FOR;
  }
  Dee_XDECREF(temp_token2);
  Dee_DECREF(temp_token);
 } else loop_mode = DeeStmtAstKind_FOR;
 // Create the generator function and scope
 if ((function_ast->ob_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) goto err_fn_r;
 if ((stmt = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe((DeeObject *)function_ast->ob_token)) == NULL) goto err_fn_r;
 stmt->ob_kind = loop_mode;
 if (loop_mode == DeeStmtAstKind_FOR) {
  if ((stmt->ob_for.ob_f_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)function_ast->ob_scope,1)) == NULL) {
err_stmt_r: _DeeStmtAst_Free(stmt); goto err_fn_r;
  }
  // Set the yield flag in the generate scope
  stmt->ob_for.ob_f_scope->ob_flags |= DEE_SCOPE_FLAG_FOUND_YIELD;
  // for-mode: parse init-expression now
  if (token.tk_id == ';') { stmt->ob_for.ob_f_forinit = NULL; goto for_1_semi; }
  if ((stmt->ob_for.ob_f_forinit = (DeeExprAstObject *)DeeLexer_ParseExpressionDecl(
   (DeeObject *)self,(DeeObject *)stmt->ob_for.ob_f_scope,flags)) == NULL) {
err_for_1: Dee_DECREF(stmt->ob_for.ob_f_scope); goto err_stmt_r;
  }
  if (token.tk_id != ';') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_GENERATOR_FOR_1,
    (DeeObject *)self,token_ob,"Expected ';' after generator-style 'for' '(' [<expr>]") == -1) goto err_for_2;
  } else for_1_semi: if (!yield()) {
err_for_2: Dee_XDECREF(stmt->ob_for.ob_f_forinit); goto err_for_1;
  }
  // for-mode: parse continue-expression now
  if (token.tk_id == ';') { stmt->ob_for.ob_f_forcont = NULL; goto for_2_semi; }
  if ((stmt->ob_for.ob_f_forcont = _DeeLexer_ParseConditionalExpressionLv2(
   self,stmt->ob_for.ob_f_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_for_2;
  if (token.tk_id != ';') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_GENERATOR_FOR_2,
    (DeeObject *)self,token_ob,"Expected ';' after generator-style 'for' '(' [<expr>] ';' [<expr>]") == -1) goto err_for_3;
  } else for_2_semi: if (!yield()) {
err_for_3: Dee_XDECREF(stmt->ob_for.ob_f_forcont); goto err_for_2;
  }
  // for-mode: parse next-expression now
  if (token.tk_id == ')') { stmt->ob_for.ob_f_fornext = NULL; goto for_3_semi; }
  if ((stmt->ob_for.ob_f_fornext = (DeeExprAstObject *)DeeLexer_ParseExpressionLv2(
   (DeeObject *)self,(DeeObject *)stmt->ob_for.ob_f_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_for_2;
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_FOR,
    (DeeObject *)self,token_ob,"Expected ')' after generator-style 'for' '(' [<expr>] ';' [<expr>] ';' [<expr>]") == -1) goto err_for_3;
  } else for_3_semi: if (!yield()) {
err_for_4: Dee_XDECREF(stmt->ob_for.ob_f_fornext); goto err_for_3;
  }
  // for-mode: parse block now
  if ((stmt->ob_for.ob_f_block = _DeeLexer_ParseSingleYieldExpressionInNewScope(
   self,stmt->ob_for.ob_f_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_for_4;
 } else { // --- __foreach / for-in mode ---
  if ((stmt->ob_for_in.ob_fi_scope = (DeeScopeObject *)DeeScope_New((
   DeeObject *)function_ast->ob_scope,1)) == NULL) goto err_stmt_r;
  // foreach-mode: parse loop-expression now
  if ((stmt->ob_for_in.ob_fi_itername = (DeeLocalVarObject *)
   DeeScope_Lookup((DeeObject *)stmt->ob_for_in.ob_fi_scope,
   loop_var,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL)) == NULL) {
err_forin_1: Dee_DECREF(stmt->ob_for_in.ob_fi_scope); goto err_stmt_r;
  }
  DEE_ASSERT(DeeObject_Check(stmt->ob_for_in.ob_fi_itername) &&
             DeeLocalVar_Check(stmt->ob_for_in.ob_fi_itername));
  Dee_INCREF(stmt->ob_for_in.ob_fi_itername);
  if ((stmt->ob_for_in.ob_fi_iterlist = (DeeExprAstObject *)DeeLexer_ParseExpression((
   DeeObject *)self,(DeeObject *)stmt->ob_for_in.ob_fi_scope,flags)) == NULL) {
err_forin_2: Dee_DECREF(stmt->ob_for_in.ob_fi_itername); goto err_forin_1;
  }
  if (token.tk_id != ')') {
   if (loop_mode == DeeStmtAstKind_FOREACH) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_FOREACH,
     (DeeObject *)self,token_ob,"Expected ')' after generator-style '__foreach' '(' <keyword> ':' <expr>") == -1) {
err_forin_3: Dee_DECREF(stmt->ob_for_in.ob_fi_iterlist); goto err_forin_2;
    }
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_FORIN,
     (DeeObject *)self,token_ob,"Expected ')' after generator-style 'for' '(' <keyword> ':' <expr>") == -1) goto err_forin_3;
   }
  } else if (!yield()) goto err_forin_3;
  if ((stmt->ob_for_in.ob_fi_block = _DeeLexer_ParseSingleYieldExpressionInNewScope(
   self,stmt->ob_for_in.ob_fi_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_forin_3;
 }
 Dee_INCREF(function_ast->ob_fun_args = (struct _DeeTokenIDListObject *)_DeeTokenIDList_EmptyList);
 Dee_INHERIT_REF(function_ast->ob_fun_stmt,stmt);
 self->ob_func_name = old_func_name;
 // Optimize the function
 if ((flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0 &&
     _DeeExprAst_OptimizeFunction(function_ast,self) == -1
     ) Dee_CLEAR(result);
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP
DeeExprAstObject *_DeeLexer_ParseWhileGenerator(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 TPPTokenID old_func_name;
 DeeExprAstObject *result,*function_ast;
 DeeStmtAstObject *stmt;
 DEE_ASSERT(token.tk_id == KWD_while);
 if ((result = new_ast(DeeExprAstKind_CALL,token_ob)) == NULL) return NULL;
 if ((result->ob_op_a = new_ast(DeeExprAstKind_YFUNCTION,token_ob)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
 }
 if ((result->ob_op_b = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_r;
 Dee_INCREF(result->ob_op_b->ob_const = Dee_EmptyTuple);
 function_ast = result->ob_op_a;
 if ((function_ast->ob_fun_name = (DeeStringObject *)DeeString_Newf(
  "%s:generator",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),self->ob_func_name))) == NULL) goto err_r;
 old_func_name = self->ob_func_name;
 if ((self->ob_func_name = TPPKeywordList_GetS(&DeeLexer_LEXER(self)->l_keywords,DeeString_STR(
  function_ast->ob_fun_name),DeeString_SIZE(function_ast->ob_fun_name),1)) == 0) {
err_fn_r: self->ob_func_name = old_func_name; goto err_r;
 }
 if ((function_ast->ob_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) goto err_fn_r;
 if ((stmt = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(token_ob)) == NULL) goto err_fn_r;
 stmt->ob_kind = DeeStmtAstKind_WHILE;
 if ((stmt->ob_while.ob_w_scope = (DeeScopeObject *)
  DeeScope_New((DeeObject *)function_ast->ob_scope,1)) == NULL) {
err_1: _DeeStmtAst_Free(stmt); goto err_fn_r;
 }
 if (!yield()) {
err_2: Dee_DECREF(stmt->ob_while.ob_w_scope); goto err_1;
 }
 // Parse the condition
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_WHILE,
   (DeeObject *)self,token_ob,"Expected '(' after generator-style 'while'") == -1) goto err_2;
 } else if (!yield()) goto err_2;
 if ((stmt->ob_while.ob_w_loopcond = _DeeLexer_ParseConditionalExpressionLv2(
  self,stmt->ob_while.ob_w_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_2;
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_WHILE,
   (DeeObject *)self,token_ob,"Expected ')' after generator-style 'while' '(' <expr>") == -1) goto err_3;
 } else if (!yield()) {
err_3: Dee_DECREF(stmt->ob_while.ob_w_loopcond); goto err_2;
 }
 // Parse the block
 if ((stmt->ob_while.ob_w_block = _DeeLexer_ParseSingleYieldExpressionInNewScope(
  self,stmt->ob_while.ob_w_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_3;
 Dee_INCREF(function_ast->ob_fun_args = (struct _DeeTokenIDListObject *)_DeeTokenIDList_EmptyList);
 Dee_INHERIT_REF(function_ast->ob_fun_stmt,stmt);
 self->ob_func_name = old_func_name;
 // Optimize the function
 if ((flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0 &&
     _DeeExprAst_OptimizeFunction(function_ast,self) == -1
     ) Dee_CLEAR(result);
 return result;
}
DeeExprAstObject *_DeeLexer_ParseDoGenerator(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 TPPTokenID old_func_name;
 DeeExprAstObject *result,*function_ast;
 DeeStmtAstObject *stmt;
 DEE_ASSERT(token.tk_id == KWD_do || token.tk_id == KWD___looponce);
 if ((result = new_ast(DeeExprAstKind_CALL,token_ob)) == NULL) return NULL;
 if ((result->ob_op_a = new_ast(DeeExprAstKind_YFUNCTION,token_ob)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
 }
 if ((result->ob_op_b = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_r;
 Dee_INCREF(result->ob_op_b->ob_const = Dee_EmptyTuple);
 function_ast = result->ob_op_a;
 if ((function_ast->ob_fun_name = (DeeStringObject *)DeeString_Newf(
  "%s:generator",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),self->ob_func_name))) == NULL) goto err_r;
 old_func_name = self->ob_func_name;
 if ((self->ob_func_name = TPPKeywordList_GetS(&DeeLexer_LEXER(self)->l_keywords,DeeString_STR(
  function_ast->ob_fun_name),DeeString_SIZE(function_ast->ob_fun_name),1)) == 0) {
err_fn_r: self->ob_func_name = old_func_name; goto err_r;
 }
 if ((function_ast->ob_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) goto err_fn_r;
 if ((stmt = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(token_ob)) == NULL) goto err_fn_r;
 stmt->ob_kind = token.tk_id == KWD___looponce ? DeeStmtAstKind_LOOP_ONCE : DeeStmtAstKind_DO_WHILE;
 if ((stmt->ob_do_while.ob_dw_scope = (DeeScopeObject *)
  DeeScope_New((DeeObject *)function_ast->ob_scope,1)) == NULL) {
err_1: _DeeStmtAst_Free(stmt); goto err_fn_r;
 }
 if (!yield()) {
err_2: Dee_DECREF(stmt->ob_do_while.ob_dw_scope); goto err_1;
 }
 // Parse the block
 if ((stmt->ob_do_while.ob_dw_block = _DeeLexer_ParseSingleYieldExpressionInNewScope(
  self,stmt->ob_do_while.ob_dw_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_2;
 if (stmt->ob_kind != DeeStmtAstKind_LOOP_ONCE) {
  // Parse the condition
  if (token.tk_id != KWD_while) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_WHILE_AFTER_GENERATOR_DO,
    (DeeObject *)self,token_ob,"Expected 'while' after generator-style 'do' <expr>") == -1) goto err_3;
  } else if (!yield()) {
err_3: Dee_DECREF(stmt->ob_do_while.ob_dw_block); goto err_2;
  }
  if (token.tk_id != '(') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_DO_WHILE,
    (DeeObject *)self,token_ob,"Expected '(' after generator-style 'do' <expr> 'while'") == -1) goto err_3;
  } else if (!yield()) goto err_3;
  if ((stmt->ob_do_while.ob_dw_loopcond = _DeeLexer_ParseConditionalExpressionLv2(
   self,stmt->ob_do_while.ob_dw_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_2;
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_DO_WHILE,
    (DeeObject *)self,token_ob,"Expected ')' after generator-style 'do' <expr> 'while' '(' <expr>") == -1) goto err_4;
  } else if (!yield()) {
err_4: Dee_DECREF(stmt->ob_do_while.ob_dw_loopcond); goto err_3;
  }
 }
 Dee_INCREF(function_ast->ob_fun_args = (struct _DeeTokenIDListObject *)_DeeTokenIDList_EmptyList);
 Dee_INHERIT_REF(function_ast->ob_fun_stmt,stmt);
 self->ob_func_name = old_func_name;
 // Optimize the function
 if ((flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0 &&
     _DeeExprAst_OptimizeFunction(function_ast,self) == -1
     ) Dee_CLEAR(result);
 return result;
}
#endif


DEE_STATIC_INLINE(void) _deetype_add_atomic(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (ty == NULL) *t = DeeObject_TYPE(atomic(Dee_rt_int)); else
 if (ty == (DeeTypeObject *)&DeeInt8_Type) *t = (DeeTypeObject *)&DeeAtomicInt8_Type; else
 if (ty == (DeeTypeObject *)&DeeInt16_Type) *t = (DeeTypeObject *)&DeeAtomicInt16_Type; else
 if (ty == (DeeTypeObject *)&DeeInt32_Type) *t = (DeeTypeObject *)&DeeAtomicInt32_Type; else
 if (ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeAtomicInt64_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type) *t = (DeeTypeObject *)&DeeAtomicUInt8_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt16_Type) *t = (DeeTypeObject *)&DeeAtomicUInt16_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt32_Type) *t = (DeeTypeObject *)&DeeAtomicUInt32_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUInt64_Type;
}
DEE_STATIC_INLINE(void) _deetype_add_signed(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (ty != (DeeTypeObject *)&DeeInt8_Type  && ty != (DeeTypeObject *)&DeeAtomicInt8_Type &&
     ty != (DeeTypeObject *)&DeeInt16_Type && ty != (DeeTypeObject *)&DeeAtomicInt16_Type &&
     ty != (DeeTypeObject *)&DeeInt32_Type && ty != (DeeTypeObject *)&DeeAtomicInt32_Type &&
     ty != (DeeTypeObject *)&DeeInt64_Type && ty != (DeeTypeObject *)&DeeAtomicInt64_Type) {
  if (ty == (DeeTypeObject *)&DeeUInt8_Type)  *t = (DeeTypeObject *)&DeeInt8_Type;  else if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type)  *t = (DeeTypeObject *)&DeeAtomicInt8_Type;  else
  if (ty == (DeeTypeObject *)&DeeUInt16_Type) *t = (DeeTypeObject *)&DeeInt16_Type; else if (ty == (DeeTypeObject *)&DeeAtomicUInt16_Type) *t = (DeeTypeObject *)&DeeAtomicInt16_Type; else
  if (ty == (DeeTypeObject *)&DeeUInt32_Type) *t = (DeeTypeObject *)&DeeInt32_Type; else if (ty == (DeeTypeObject *)&DeeAtomicUInt32_Type) *t = (DeeTypeObject *)&DeeAtomicInt32_Type; else
  if (ty == (DeeTypeObject *)&DeeUInt64_Type) *t = (DeeTypeObject *)&DeeInt64_Type; else if (ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicInt64_Type; else
  if (ty == NULL) *t = DeeObject_TYPE(Dee_rt_int);
 }
}
DEE_STATIC_INLINE(void) _deetype_add_unsigned(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (ty != (DeeTypeObject *)&DeeUInt8_Type  && ty != (DeeTypeObject *)&DeeAtomicUInt8_Type &&
     ty != (DeeTypeObject *)&DeeUInt16_Type && ty != (DeeTypeObject *)&DeeAtomicUInt16_Type &&
     ty != (DeeTypeObject *)&DeeUInt32_Type && ty != (DeeTypeObject *)&DeeAtomicUInt32_Type &&
     ty != (DeeTypeObject *)&DeeUInt64_Type && ty != (DeeTypeObject *)&DeeAtomicUInt64_Type) {
  if (ty == (DeeTypeObject *)&DeeInt8_Type)  *t = (DeeTypeObject *)&DeeUInt8_Type;  else if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type)  *t = (DeeTypeObject *)&DeeAtomicUInt8_Type;  else
  if (ty == (DeeTypeObject *)&DeeInt16_Type) *t = (DeeTypeObject *)&DeeUInt16_Type; else if (ty == (DeeTypeObject *)&DeeAtomicInt16_Type) *t = (DeeTypeObject *)&DeeAtomicUInt16_Type; else
  if (ty == (DeeTypeObject *)&DeeInt32_Type) *t = (DeeTypeObject *)&DeeUInt32_Type; else if (ty == (DeeTypeObject *)&DeeAtomicInt32_Type) *t = (DeeTypeObject *)&DeeAtomicUInt32_Type; else
  if (ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeUInt64_Type; else if (ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUInt64_Type; else
  if (ty == NULL) *t = DeeObject_TYPE(Dee_rt_uint);
 }
}
DEE_STATIC_INLINE(void) _deetype_add_char(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (
#if DEE_CONFIG_RT_CHAR_SIGNED
  !ty || 
#endif /* DEE_CONFIG_RT_CHAR_SIGNED */
  ty == (DeeTypeObject *)&DeeInt8_Type ||
  ty == (DeeTypeObject *)&DeeInt16_Type ||
  ty == (DeeTypeObject *)&DeeInt32_Type ||
  ty == (DeeTypeObject *)&DeeInt64_Type
  ) *t = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR); else
 if (
#if !DEE_CONFIG_RT_CHAR_SIGNED
  !ty || 
#endif /* !DEE_CONFIG_RT_CHAR_SIGNED */
  ty == (DeeTypeObject *)&DeeUInt8_Type ||
  ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || 
  ty == (DeeTypeObject *)&DeeUInt32_Type || 
  ty == (DeeTypeObject *)&DeeAtomicUInt64_Type
  ) *t = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR); else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type ||
     ty == (DeeTypeObject *)&DeeAtomicInt16_Type ||
     ty == (DeeTypeObject *)&DeeAtomicInt32_Type ||
     ty == (DeeTypeObject *)&DeeAtomicInt64_Type
     ) *t = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR); else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type ||
     ty == (DeeTypeObject *)&DeeAtomicUInt16_Type ||
     ty == (DeeTypeObject *)&DeeAtomicUInt32_Type ||
     ty == (DeeTypeObject *)&DeeAtomicUInt64_Type
     ) *t = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR);
}
DEE_STATIC_INLINE(void) _deetype_add_short(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type ||
     ty == (DeeTypeObject *)&DeeInt16_Type ||
     ty == (DeeTypeObject *)&DeeInt32_Type ||
     ty == (DeeTypeObject *)&DeeInt64_Type
     ) *t = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT); else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type ||
     ty == (DeeTypeObject *)&DeeAtomicUInt16_Type ||
     ty == (DeeTypeObject *)&DeeUInt32_Type ||
     ty == (DeeTypeObject *)&DeeAtomicUInt64_Type
     ) *t = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT); else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type ||
     ty == (DeeTypeObject *)&DeeAtomicInt16_Type || 
     ty == (DeeTypeObject *)&DeeAtomicInt32_Type ||
     ty == (DeeTypeObject *)&DeeAtomicInt64_Type
     ) *t = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT); else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || 
     ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || 
     ty == (DeeTypeObject *)&DeeAtomicUInt32_Type || 
     ty == (DeeTypeObject *)&DeeAtomicUInt64_Type
     ) *t = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT);
}
DEE_STATIC_INLINE(void) _deetype_add_long(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type || ty == (DeeTypeObject *)&DeeInt16_Type || ty == (DeeTypeObject *)&DeeInt32_Type || ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_LONG); else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_LONG); else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type || ty == (DeeTypeObject *)&DeeAtomicInt16_Type || ty == (DeeTypeObject *)&DeeAtomicInt32_Type || ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_LONG); else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_LONG);
}
DEE_STATIC_INLINE(void) _deetype_add_llong(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type || ty == (DeeTypeObject *)&DeeInt16_Type || ty == (DeeTypeObject *)&DeeInt32_Type || ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG); else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG); else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type || ty == (DeeTypeObject *)&DeeAtomicInt16_Type || ty == (DeeTypeObject *)&DeeAtomicInt32_Type || ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG); else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG);
}
DEE_STATIC_INLINE(void) _deetype_add_int8(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt16_Type || ty == (DeeTypeObject *)&DeeInt32_Type || ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeInt8_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeUInt8_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicInt16_Type || ty == (DeeTypeObject *)&DeeAtomicInt32_Type || ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicInt8_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUInt8_Type;
}
DEE_STATIC_INLINE(void) _deetype_add_int16(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type || ty == (DeeTypeObject *)&DeeInt32_Type || ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeInt16_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type || ty == (DeeTypeObject *)&DeeUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeUInt16_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type || ty == (DeeTypeObject *)&DeeAtomicInt32_Type || ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicInt16_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt32_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUInt16_Type;
}
DEE_STATIC_INLINE(void) _deetype_add_int32(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type || ty == (DeeTypeObject *)&DeeInt16_Type || ty == (DeeTypeObject *)&DeeInt64_Type) *t = (DeeTypeObject *)&DeeInt32_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeUInt32_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type || ty == (DeeTypeObject *)&DeeAtomicInt16_Type || ty == (DeeTypeObject *)&DeeAtomicInt64_Type) *t = (DeeTypeObject *)&DeeAtomicInt32_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt64_Type) *t = (DeeTypeObject *)&DeeAtomicUInt32_Type;
}
DEE_STATIC_INLINE(void) _deetype_add_int64(DeeTypeObject **t) {
 DeeTypeObject *ty = *t;
 if (!ty || ty == (DeeTypeObject *)&DeeInt8_Type || ty == (DeeTypeObject *)&DeeInt16_Type || ty == (DeeTypeObject *)&DeeInt32_Type) *t = (DeeTypeObject *)&DeeInt64_Type; else
 if (ty == (DeeTypeObject *)&DeeUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeUInt32_Type) *t = (DeeTypeObject *)&DeeUInt64_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicInt8_Type || ty == (DeeTypeObject *)&DeeAtomicInt16_Type || ty == (DeeTypeObject *)&DeeAtomicInt32_Type) *t = (DeeTypeObject *)&DeeAtomicInt64_Type; else
 if (ty == (DeeTypeObject *)&DeeAtomicUInt8_Type || ty == (DeeTypeObject *)&DeeAtomicUInt16_Type || ty == (DeeTypeObject *)&DeeAtomicUInt32_Type) *t = (DeeTypeObject *)&DeeAtomicUInt64_Type;
}

DeeExprAstObject *_DeeLexer_ParseType(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTokenObject *tok;
 DeeExprAstObject *result;
#define FLAG_EXPLICIT_SIGN  0x01 // "signed" or "unsigned" was used
#define FLAG_EXPLICIT_ATOM  0x02 // "atomic" was used
#define FLAG_EXPLICIT_INT   0x04 // "int" was used
#define FLAG_EXPLICIT_SIZE  0x08 // "char", "short", "long" was used
#define FLAG_EXPLICIT_LSIZE (FLAG_EXPLICIT_SIZE|0x10) // "long" was used twice
 int type_flags = 0;
 DeeTypeObject *type = NULL;
 (void)scope,flags;
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
again_0:
 switch (token.tk_id) {
  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
   if (!yield()) goto err_t;
   goto again_0;
  case KWD_float:
   type = (DeeTypeObject *)&DeeFloat_Type; 
   if (!yield()) goto err_t;
   goto end;
  case KWD_double:
   type = (DeeTypeObject *)&DeeDouble_Type;
   if (!yield()) goto err_t;
   if (token.tk_id == KWD_long) {
    type = (DeeTypeObject *)&DeeLDouble_Type;
    if (!yield()) goto err_t;
   }
   goto end;
  default: break;
 }

again:
 switch (token.tk_id) {

  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
   if (!yield()) goto err_t;
   goto again;
  DEE_PARSE_TOKENID_CASE_ATOMIC
   if ((type_flags & FLAG_EXPLICIT_ATOM)!=0) {
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_ATOMIC,(DeeObject *)self,token_ob,
     "'atomic' was already used as modifier") == -1) goto err_t;
   } else _deetype_add_atomic(&type);
   type_flags |= FLAG_EXPLICIT_ATOM;
next:
   if (!yield()) {err_t: Dee_DECREF(tok); return NULL; }
   goto again;

  case KWD_int:
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) {
int_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_INT,(DeeObject *)self,token_ob,
     "'int' was already used as modifier") == -1) goto err_t;
   } else if (!type) type = DeeObject_TYPE(Dee_rt_int);
   type_flags |= FLAG_EXPLICIT_INT;
   goto next;

  DEE_PARSE_TOKENID_CASE_SIGNED
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) {
sign_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_SIGN,(DeeObject *)self,token_ob,
     "'signed' or 'unsigned' was already used as modifier") == -1) goto err_t;
   } else _deetype_add_signed(&type);
   type_flags |= FLAG_EXPLICIT_SIGN;
   goto next;

  DEE_PARSE_TOKENID_CASE_UNSIGNED
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   else _deetype_add_unsigned(&type);
   type_flags |= FLAG_EXPLICIT_SIGN;
   goto next;

  case KWD_char:
   if ((type_flags&FLAG_EXPLICIT_SIZE)!=0) {
size_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_SIZE,(DeeObject *)self,token_ob,
     "Type size was already used as modifier") == -1) goto err_t;
   } else _deetype_add_char(&type);
   type_flags |= FLAG_EXPLICIT_SIZE;
   goto next;
  
  case KWD_short:
   if ((type_flags&FLAG_EXPLICIT_SIZE)!=0) goto size_used;
   else _deetype_add_short(&type);
   type_flags |= FLAG_EXPLICIT_SIZE;
   goto next;
  
  case KWD_long:
   if ((type_flags&FLAG_EXPLICIT_LSIZE)==FLAG_EXPLICIT_LSIZE) goto size_used;
   else {
    if ((type_flags&FLAG_EXPLICIT_SIZE)==0) { // first long
     if (!yield()) goto err_t;
     if (token.tk_id == KWD_double) {
      type = (DeeTypeObject *)&DeeLDouble_Type;
      if (!yield()) goto err_t;
      goto end;
     }
     type_flags |= FLAG_EXPLICIT_SIZE;
     _deetype_add_long(&type);
     goto again;
    } else { // second long
     if (!yield()) goto err_t;
     if (token.tk_id == KWD_double) {
      type = (DeeTypeObject *)&DeeLDouble_Type;
      if (!yield()) goto err_t;
      goto end;
     }
     type_flags |= FLAG_EXPLICIT_LSIZE;
     _deetype_add_llong(&type);
     goto again;
    }
   }
   goto next;

  DEE_PARSE_TOKENID_CASE_WCHAR_T
  DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_WCHAR_T
     type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T);
     goto next;
    DEE_PARSE_TOKENID_CASE_CHAR16_T
     type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T);
     goto next;
    DEE_PARSE_TOKENID_CASE_CHAR32_T
     type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T);
     goto next;
    default: break;
   }
   DEE_BUILTIN_UNREACHABLE();

  DEE_PARSE_TOKENID_CASE_UINT8
  DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32
  DEE_PARSE_TOKENID_CASE_UINT64
#if 1
   if ((type_flags&FLAG_EXPLICIT_INT)!=0 ||
       (type_flags&FLAG_EXPLICIT_SIGN)!=0 ||
       (type_flags&FLAG_EXPLICIT_LSIZE)!=0
       ) break;
#else
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
#endif
   type_flags |= (FLAG_EXPLICIT_INT|FLAG_EXPLICIT_SIGN|FLAG_EXPLICIT_LSIZE);
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_UINT8  type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt8_Type:(DeeTypeObject *)&DeeUInt8_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT16 type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt16_Type:(DeeTypeObject *)&DeeUInt16_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT32 type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt32_Type:(DeeTypeObject *)&DeeUInt32_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT64 type = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt64_Type:(DeeTypeObject *)&DeeUInt64_Type; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   goto next;

  DEE_PARSE_TOKENID_CASE_INT8
  DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32
  DEE_PARSE_TOKENID_CASE_INT64
#if 1
   if ((type_flags&FLAG_EXPLICIT_INT)!=0 ||
       (type_flags&FLAG_EXPLICIT_LSIZE)!=0
       ) break;
#else
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
#endif
   type_flags |= (FLAG_EXPLICIT_INT|FLAG_EXPLICIT_LSIZE);
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_INT8  _deetype_add_int8(&type); break;
    DEE_PARSE_TOKENID_CASE_INT16 _deetype_add_int16(&type); break;
    DEE_PARSE_TOKENID_CASE_INT32 _deetype_add_int32(&type); break;
    DEE_PARSE_TOKENID_CASE_INT64 _deetype_add_int64(&type); break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   goto next;

  default:
   if (!type) {
    if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_TYPE_EXPRESSION,
     (DeeObject *)self,token_ob,"Unexpected token in type expression") == -1) goto err_t;
    type = DeeObject_TYPE(Dee_rt_int);
   }
   break;
 }
#undef FLAG_EXPLICIT_LSIZE
#undef FLAG_EXPLICIT_SIZE
#undef FLAG_EXPLICIT_INT
#undef FLAG_EXPLICIT_ATOM
#undef FLAG_EXPLICIT_SIGN
 DEE_ASSERT(type); // Should always be non-NULL
end:
 if ((result = (DeeExprAstObject *)new_ast(
  DeeExprAstKind_CONSTANT,(DeeObject *)tok)) == NULL) goto err_t;
 Dee_DECREF(tok);
 Dee_INCREF(result->ob_const = (DeeObject *)type);
 return result;
}



#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
DeeExprAstObject *_DeeLexer_ParseBuiltinChooseExpr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *ast1,*ast2; int ccond;
 DEE_ASSERT(token.tk_id == KWD___builtin_choose_expr);
 if (!yield()) return NULL;
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_CHOOSE_EXPR,(DeeObject *)self,token_ob,"Expected '(' after '__builtin_choose_expr'") == -1) return NULL;
 } else if (!yield()) return NULL;
 if ((ast1 = _deeparse_top(self,scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return NULL;
 if (ast1->ob_kind != DeeExprAstKind_CONSTANT) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_CHOOSE_EXPR,(DeeObject *)self,(DeeObject *)ast1->ob_token,
   "'__builtin_choose_expr' expects a constant expression as its first argument") == -1) {err_ast1: Dee_DECREF(ast1); return NULL; }
  Dee_XDECREF(ast1->ob_op_a);
  Dee_XCLEAR(ast1->ob_op_b);
  Dee_XCLEAR(ast1->ob_op_c);
  Dee_XCLEAR(ast1->ob_op_d);
  ast1->ob_kind = DeeExprAstKind_CONSTANT;
  Dee_INCREF(ast1->ob_const = Dee_None);
 }
 if ((ccond = DeeObject_Bool(ast1->ob_const)) == -1) goto err_ast1;
 Dee_DECREF(ast1);
 if (token.tk_id != ',') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_CHOOSE_EXPR_1,(DeeObject *)self,token_ob,"Expected ',' after '__builtin_choose_expr' '(' <expr>") == -1) return NULL;
 } else if (!yield()) return NULL;
 if (!ccond) ++self->ob_suppress_Wcop;
 ast1 = _deeparse_top(self,scope,(Dee_uint32_t)(ccond ? flags : flags|DEE_PARSER_FLAG_UNREACHABLE));
 if (!ccond) --self->ob_suppress_Wcop;
 if (!ast1) return NULL;
 if (token.tk_id != ',') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_CHOOSE_EXPR_2,(DeeObject *)self,token_ob,"Expected ',' after '__builtin_choose_expr' '(' <expr> ',' <expr>") == -1) goto err_ast1;
 } else if (!yield()) goto err_ast1;
 if (ccond) ++self->ob_suppress_Wcop;
 ast2 = _deeparse_top(self,scope,(Dee_uint32_t)(ccond ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags));
 if (ccond) --self->ob_suppress_Wcop;
 if (!ast2) goto err_ast1;
 if (ccond) {
  Dee_DECREF(ast2);
 } else {
  Dee_DECREF(ast1);
  Dee_INHERIT_REF(ast1,ast2);
 }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_ast1;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_CHOOSE_EXPR,(DeeObject *)self,token_ob,
   "Expected ')' after '__builtin_choose_expr' '(' <expr> ',' <expr> ',' <expr>") == -1) goto err_ast1;
 } else if (!yield()) goto err_ast1;
 return ast1;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
DeeExprAstObject *_DeeLexer_ParseBuiltinConstantP(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result;
 DEE_ASSERT(token.tk_id == KWD___builtin_constant_p);
 if (!yield()) return NULL;
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_CONSTANT_P,(DeeObject *)self,token_ob,
   "Expected '(' after '__builtin_constant_p'") == -1) return NULL;
 } else if (!yield()) return NULL;
 // NOTE: We don't add all the simplification flags because '__builtin_constant_p'
 //       must return true based on the compilation mode
 ++self->ob_suppress_Wcop;
 result = _deeparse_top(self,scope,flags);
 --self->ob_suppress_Wcop;
 if (result == NULL) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_r;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_CONSTANT_P,(DeeObject *)self,token_ob,
   "Expected ')' after '__builtin_constant_p' '(' <expr>") == -1) goto err_r;
 } else if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
 // Clear all sub-asts of the contained expression
 Dee_XDECREF(result->ob_op_a);
 Dee_XCLEAR(result->ob_op_b);
 Dee_XCLEAR(result->ob_op_c);
 Dee_XCLEAR(result->ob_op_d);
 if (result->ob_kind == DeeExprAstKind_CONSTANT) {
  result->ob_const = Dee_True;
 } else {
  result->ob_const = Dee_False;
  result->ob_kind = DeeExprAstKind_CONSTANT;
 }
 Dee_INCREF(result->ob_const);
 return result;
}
#endif


#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
DeeExprAstObject *_DeeLexer_ParseBuiltinNoop(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTokenObject *tok; int has_paren;
 DeeExprAstObject *result;
 DEE_ASSERT(token.tk_id == KWD___noop || token.tk_id == KWD___builtin_noop);
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
 if (!yield()) { err_t: Dee_DECREF(tok); return NULL; }
 has_paren = token.tk_id == '(';
 if (has_paren) {
  if (!yield()) goto err_t;
  while (token.tk_id == ',') if (!yield()) goto err_t;
  if (token.tk_id == ')') {
   if (!yield()) goto err_t;
   result = new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)tok);
   Dee_DECREF(tok);
   if (result) Dee_INCREF(result->ob_const = Dee_None);
   return result;
  }
 }
 while (1) {
  if ((result = _deeparse_top(self,scope,
   // v disable optimizations, because whatever they do is just ignored
   //   NOTE: Don't disable them, so all the regular warnings are produced
   flags/*&~DEE_PARSER_FLAG_OPTIMIZE_ALL*/)) == NULL) goto err_t;
  if (has_paren) {
   // Skip ignored commas
   while (token.tk_id == ',') if (!yield()) {
err_rt: Dee_DECREF(result); goto err_t;
   }
  } else {
   if (token.tk_id != ',') break;
   if (!yield()) goto err_rt;
  }
  Dee_DECREF(result);
 }
 if (has_paren) {
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NOOP,(DeeObject *)self,token_ob,
    "Expected ')' after '__builtin_noop' '(' ...") == -1) goto err_rt;
  } else if (!yield()) goto err_rt;
 }
 // Clear all sub-asts of the contained expression
 Dee_XDECREF(result->ob_op_a);
 Dee_XCLEAR(result->ob_op_b);
 Dee_XCLEAR(result->ob_op_c);
 Dee_XCLEAR(result->ob_op_d);
 // Convert the ast into a constant
 result->ob_kind = DeeExprAstKind_CONSTANT;
 Dee_INCREF(result->ob_const = Dee_None);
 Dee_DECREF(result->ob_token);
 Dee_INHERIT_REF(result->ob_token,tok);
 return result;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
DeeExprAstObject *_DeeLexer_ParseBuiltinAlloca(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result;
 DEE_ASSERT(token.tk_id == KWD___builtin_alloca);
 if ((result = new_ast(DeeExprAstKind_BUILTIN_ALLOCA,token_ob)) == NULL) return NULL;
 if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_ALLOCA,(DeeObject *)self,token_ob,
   "Expected '(' after '__builtin_alloca'") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((result->ob_op_a = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,(DeeObject *)scope,flags)) == NULL) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_r;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_ALLOCA,(DeeObject *)self,token_ob,
   "Expected ')' after '__builtin_alloca' '(' <expr>") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 return result;
}
#endif


struct DeeTupleObject *_DeeLexer_ParseSetInit(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTupleObject *result; DeeExprAstObject *item;
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return NULL;
 Dee_INCREF(result = (DeeTupleObject *)Dee_EmptyTuple);
 if (token.tk_id == '}') { // Empty Init
  if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
  return result;
 } else {
  while (1) {
   int temp;
   if ((item = _deeparse_top(self,scope,flags)) == NULL) goto err_r;
   temp = DeeTuple_Append((DeeObject **)&result,(DeeObject *)item);
   Dee_DECREF(item);
   if (temp == -1) goto err_r;
   if (token.tk_id != ',') break;
   if (!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == '}') break;
#endif
  }
  if (token.tk_id != '}') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_RANGE_INIT,(DeeObject *)self,token_ob,
    "set: Expected '}' after '{' ',' ~~ <expr>...") == -1) goto err_r;
  } else if (!yield()) goto err_r;
 }
 if (token.tk_id == '(') {
  if (DeeError_CompilerError(DEE_WARNING_SET_DIRECTLY_CALLED,(DeeObject *)self,token_ob,
   "Set object is immediately called after creation "
   "(This can't be what you want. Sets aren't even callable)") == -1) goto err_r;
 }
 return result;
}
struct DeeTupleObject *_DeeLexer_ParseListInit(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTupleObject *result; DeeExprAstObject *item;
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return NULL;
 Dee_INCREF(result = (DeeTupleObject *)Dee_EmptyTuple);
 if (token.tk_id == '}') { // Empty Init
  if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
  return result;
 } else {
  while (1) {
   int temp;
   if ((item = _deeparse_top(self,scope,flags)) == NULL) goto err_r;
   temp = DeeTuple_Append((DeeObject **)&result,(DeeObject *)item);
   Dee_DECREF(item);
   if (temp == -1) goto err_r;
   if (token.tk_id != ',') break;
   if (!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == '}') break;
#endif
  }
  if (token.tk_id != '}') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_RANGE_INIT,(DeeObject *)self,token_ob,
    "list: Expected '}' after '{' ',' ~~ <expr>...") == -1) goto err_r;
  } else if (!yield()) goto err_r;
 }
 if (token.tk_id == '(') {
  if (DeeError_CompilerError(DEE_WARNING_LIST_DIRECTLY_CALLED,(DeeObject *)self,token_ob,
   "List object is immediately called after creation "
   "(This can't be what you want. Lists aren't even callable)") == -1) goto err_r;
 }
 return result;
}
struct DeeTupleObject *_DeeLexer_ParseDictInit(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTupleObject *result; DeeExprAstObject *key,*item;
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return NULL;
 Dee_INCREF(result = (DeeTupleObject *)Dee_EmptyTuple);
 if (token.tk_id == '}') { // Empty Init
  if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
  return result;
 } else {
  while (1) {
   Dee_size_t old_size;
   if ((key = _deeparse_top(self,scope,flags)) == NULL) goto err_r;
   if (token.tk_id != ':') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_EXPR_IN_DICT,(DeeObject *)self,token_ob,
     "dict: Expected ':' after 'dict' '{' ... <expr>") == -1) goto err_rk;
   } else if (!yield()) goto err_rk;
   if ((item = _deeparse_top(self,scope,flags)) == NULL) {err_rk: Dee_DECREF(key); goto err_r; }
   old_size = DeeTuple_SIZE(result);
   if (_DeeTuple_ResizeUnsafe((DeeObject **)&result,old_size+2
    ) == -1) { Dee_DECREF(item); goto err_rk; }
   DeeTuple_SET(result,old_size,(DeeObject *)key); // Inherit reference
   DeeTuple_SET(result,old_size+1,(DeeObject *)item); // Inherit reference
   if (token.tk_id != ',') break;
   if (!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == '}') break;
#endif
  }
  if (token.tk_id != '}') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_RANGE_INIT,(DeeObject *)self,token_ob,
    "dict: Expected '}' after '{' (<expr> ':' <expr>)...") == -1) goto err_r;
  } else if (!yield()) goto err_r;
 }
 if (token.tk_id == '(') {
  if (DeeError_CompilerError(DEE_WARNING_DICT_DIRECTLY_CALLED,(DeeObject *)self,token_ob,
   "Dict object is immediately called after creation "
   "(This can't be what you want. Dicts aren't even callable)") == -1) goto err_r;
 }
 return result;
}

DeeExprAstObject *_DeeLexer_ParseGeneric(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTypeObject *expr_type;
 DeeExprAstObject *expr_ast;
 DeeExprAstObject *result;
 DeeTokenObject *first_token;
 DEE_ASSERT(token.tk_id == KWD__Generic);
 Dee_INCREF(first_token = (DeeTokenObject *)token_ob);
 if (!yield()) {err_tok: Dee_DECREF(first_token); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERIC,(DeeObject *)self,token_ob,
   "Expected '(' after '_Generic'") == -1) goto err_tok;
 } else if (!yield()) goto err_tok;
 if ((expr_ast = _deeparse_top(self,scope,flags)) == NULL) goto err_tok;
 expr_type = DeeExprAst_PredictType((DeeObject *)expr_ast,(DeeObject *)scope);
 if (!expr_type) expr_type = (DeeTypeObject *)&DeeObject_Type;
 Dee_INCREF(expr_type);
 Dee_DECREF(expr_ast);
 result = NULL;
 if (token.tk_id != ')') {
  int match_level;
  if (token.tk_id != ',') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_GENERIC,(DeeObject *)self,token_ob,
    "Expected ',' after '_Generic' '(' <expr>") == -1) goto err_tok;
  } else if (!yield()) goto err_ty_res;
  // LEVEL 0: no match
  // LEVEL 1: default match
  // LEVEL 2: sub-class match
  // LEVEL 3: exact match
  match_level = 0;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id != ')')
#endif
  while (1) {
   DeeExprAstObject *type_branch;
   DeeTypeObject *type;
   int new_match_level;
   if (token.tk_id == KWD_default) {
    if (!yield()) goto err_ty_res;
    Dee_INCREF(type = &DeeObject_Type);
   } else {
    if ((type = _DeeLexer_ParseNamedType(self,scope,flags,NULL,NULL,NULL)) == NULL) goto err_ty_res;
   }
   new_match_level =
    type == &DeeObject_Type ? 1 :           // match: default
    type == expr_type ? 3 :                 // match: exact
    DeeType_IsDerived(expr_type,type) ? 2 : // match: sub-class
    0;                                      // match: no
   if (token.tk_id != ':') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_GENERIC_TYPE,(DeeObject *)self,token_ob,
     "Expected ':' after '_Generic' '(' <type> ...") == -1) goto err_ty_type;
   } else if (!yield()) {err_ty_type: Dee_DECREF(type); goto err_ty_res; }
   if ((type_branch = _deeparse_top(self,scope,flags)) == NULL) goto err_ty_type;
   if (new_match_level > match_level) {
    // Better match
    match_level = new_match_level;
    Dee_XDECREF(result);
    Dee_INHERIT_REF(result,type_branch);
   } else Dee_DECREF(type_branch);
   Dee_DECREF(type);
   if (token.tk_id != ',') break;
   if (!yield()) goto err_ty_res;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ')') break;
#endif
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERIC,(DeeObject *)self,token_ob,
    "Expected ')' after '_Generic' '(' ...") == -1) {err_ty_res: Dee_DECREF(expr_type); Dee_XDECREF(result); goto err_tok; }
  } else if (!yield()) goto err_ty_res;
 } else if (!yield()) goto err_ty_res;
 Dee_DECREF(expr_type);
 if (!result) {
  if (DeeError_CompilerError(DEE_WARNING_NO_SUITABLE_BRANCH_IN_GENERIC,(DeeObject *)self,token_ob,
   "No suitable branch for 'object'") == -1) goto err_ty_res;
  if ((result = new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)first_token)) == NULL) goto err_tok;
  Dee_INCREF(result->ob_const = Dee_None);
 }
 Dee_DECREF(first_token);
 return result;
}


DEE_COMPILER_MSVC_WARNING_PUSH(4701)
int _DeeLexer_ParseFunctionSuffix(
 DeeExprAstObject *func_ast, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 struct DeeAttributeDecl *attr, unsigned int func_name) {
 int has_varargs = 0;
 struct DeeScopeObject *func_scope;
 TPPTokenID old_func_name; // Function name
 if (token.tk_id == '(') {
  DeeObject *param_names;
  if (!yield()) return -1;
  if (token.tk_id == ')') { if (!yield()) return -1; goto noargs; }
  param_names = NULL;
  goto begin_args;
  while (token.tk_id == ',') {
   if (!yield()) {err_pa: Dee_XDECREF(param_names); return -1; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ')') break;
#endif
begin_args:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (_DeeTokenIDList_Add(&param_names,token.tk_id) == -1) goto err_pa;
   } else if (token.tk_id == TPP_TOK_DOTS) {
    if (_DeeTokenIDList_Add(&param_names,TPP_TOK_DOTS) == -1) goto err_pa;
    goto va_dots;
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_PARAMETER_EXPECTED_KEYWORD,
     (DeeObject *)self,token_ob,"Expected <keyword> as function parameter name") == -1) goto err_pa;
    if (_DeeTokenIDList_Add(&param_names,0) == -1) goto err_pa;
   }
   if (!yield()) goto err_pa;
  }
  if (token.tk_id == TPP_TOK_DOTS) {
va_dots:
   func_ast->ob_kind = DeeExprAstKind_VFUNCTION_EXPR;
   has_varargs = 1;
   if (!yield()) goto err_pa;
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_TO_END_OF_FUNCTION_PARAMETER_LIST,(DeeObject *)self,token_ob,
    "Expected end of parameter list") == -1) goto err_pa;
  } else if (!yield()) goto err_pa;
  Dee_INHERIT_REF(func_ast->ob_fun_args,*(struct _DeeTokenIDListObject **)&param_names);
  // Parse attributes one more time (after argument list)
  if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) return -1;
 } else {noargs:
  Dee_INCREF(func_ast->ob_fun_args = (struct _DeeTokenIDListObject *)_DeeTokenIDList_EmptyList);
 }
 if (attr->ob_name) { // Custom function name
  DEE_ASSERT(DeeObject_Check(attr->ob_name) && DeeString_Check(attr->ob_name));
  func_name = TPPKeywordList_GetS(&self->ob_lexer.l_keywords,DeeString_STR(attr->ob_name),DeeString_SIZE(attr->ob_name),1);
  if (!func_name) return -1; // Failed to allocate keyword token
 }
 // Push the new function name, so we can generate appropriate debug information
 old_func_name = self->ob_func_name;
 self->ob_func_name = func_name ? func_name : KWD___anonymous__;
 if ((func_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) { // Strong scope
err_fn_a: self->ob_func_name = old_func_name; return -1;
 }
 if (DeeScope_SetupParameters((DeeObject *)func_scope,
  _DeeTokenIDList_SIZE(func_ast->ob_fun_args),
  _DeeTokenIDList_ELEM(func_ast->ob_fun_args),attr) == -1) {
err_fn_asc: Dee_DECREF(func_scope); goto err_fn_a;
 }
 if (attr->ob_super && (attr->ob_fun_flags&DeeFunctionFlags_CC_MASK)==DeeFunctionFlags_THISCALL) {
  Dee_INCREF(func_scope->ob_supertp = attr->ob_super);
 }
 if (token.tk_id == TPP_TOK_ARROW) { // '->' expression style function
  if (!yield()) goto err_fn_asc;
  if (token.tk_id == '{') goto brace_function;
  func_ast->ob_fun_expr = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,(
   DeeObject *)func_scope,flags&~(DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE));
  if (!func_ast->ob_fun_expr) goto err_fn_asc;
  Dee_INCREF(func_ast->ob_scope = func_scope); // Inherit the scope reference
 } else if (token.tk_id == ':') { // '{' statement style function with result variable
  if (!yield()) goto err_fn_asc; // Strong scope
  if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_FUNCTION_COLLON,(DeeObject *)self,token_ob,
    "Expected <keyword> after 'function' ... ':'") == -1) goto err_fn_asc;
  } else { // A result variable makes yield statements illegal
   DeeLocalVarObject *result_var;
   func_scope->ob_flags |= (DEE_SCOPE_FLAG_FOUND_RETURN|DEE_SCOPE_FLAG_FOUND_RETVAR);
   result_var = (DeeLocalVarObject *)DeeScope_Lookup( // retval variable
    (DeeObject *)func_scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL);
   if (!result_var) goto err_fn_asc;
   DEE_ASSERT(DeeLocalVar_Check(result_var));
   result_var->ob_flags |= (DEE_LOCALVAR_FLAG_INITIALIZED|DEE_LOCAL_VAR_FLAG_RETVAL);
   if (!yield()) goto err_fn_asc;
  }
  if (token.tk_id != '{') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_FUNCTION_COLLON_KEYWORD,(DeeObject *)self,token_ob,
    "Expected '{' after 'function' ... ':' <keyword>") == -1) goto err_fn_asc;
  } else if (!yield()) goto err_fn_asc;
  goto stmt_func;
 } else if (token.tk_id == '{') { // '{' statement style function
brace_function:
  if (!yield()) goto err_fn_asc;
stmt_func:
  func_ast->ob_kind = has_varargs ? DeeExprAstKind_VFUNCTION : DeeExprAstKind_FUNCTION;
  func_ast->ob_fun_stmt = (DeeStmtAstObject *)DeeLexer_ParseStatements(
   (DeeObject *)self,(DeeObject *)func_scope,flags&~(
   DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE),'}');
  if (!func_ast->ob_fun_stmt) goto err_fn_asc;
  // Update the function type if we encountered a yield inside
  if ((func_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_YIELD) != 0)
   func_ast->ob_kind = has_varargs ? DeeExprAstKind_YVFUNCTION : DeeExprAstKind_YFUNCTION;
  Dee_INCREF(func_ast->ob_scope = func_scope); // Inherit the scope reference
 } else { // Empty (none) function
  func_ast->ob_kind = has_varargs ? DeeExprAstKind_VFUNCTION_NONE : DeeExprAstKind_FUNCTION_NONE;
  DEE_ASSERT(DeeExprAst_FUNCTION_SCOPE(func_ast) == NULL);
  DEE_ASSERT(DeeExprAst_FUNCTION_EXPR(func_ast) == NULL);
 }
 Dee_DECREF(func_scope);
 if (!func_name) {
  DEE_ASSERT(func_ast->ob_fun_name == NULL);
 } else {
  char const *name = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),func_name);
  DEE_ASSERT(name && "Invalid function name");
  if ((func_ast->ob_fun_name = (
   DeeStringObject *)DeeString_New(name)) == NULL) return -1;
 }
 self->ob_func_name = old_func_name;
 if ((flags&DEE_PARSER_FLAG_OPTIMIZE_PREDICATES)!=0) {
  if (_DeeExprAst_OptimizeFunctionKnownPredicates(func_ast,self) == -1) return -1;
  if (func_ast->ob_kind != DeeExprAstKind_CONSTANT &&
     (flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0 &&
      _DeeExprAst_OptimizeFunction(func_ast,self) == -1) return -1;
 } else {
  if ((flags&DEE_PARSER_FLAG_CONST_FUNCTION)!=0 &&
      _DeeExprAst_OptimizeFunction(func_ast,self) == -1) return -1;
 }
 return 0;
}
int _DeeExprAst_OptimizeFunction(DeeExprAstObject *self, struct DeeLexerObject *lexer) {
 if (!self->ob_scope || self->ob_scope->ob_refc == 0) {
  DeeScopeObject *fun_scope = self->ob_scope;
  DeeObject *func_ob,*func_code;
  Dee_uint32_t scope_flags = fun_scope ? fun_scope->ob_flags : DEE_SCOPE_FLAG_NONE;
  struct DeeCodeWriter fun_writer = DeeCodeWriter_INIT();
  DEE_LVERBOSE2("Packing constant function: %r\n",self);
  if (fun_scope && (fun_scope->ob_flags&DEE_SCOPE_FLAG_CODE_COPYABLE)!=0)
   fun_writer.ob_flags |= DEE_CODE_FLAG_COPY; // Enable the copyable bit
  if (DeeExprAst_KIND(self) == DeeExprAstKind_FUNCTION_EXPR ||
      DeeExprAst_KIND(self) == DeeExprAstKind_VFUNCTION_EXPR) {
   // Since only statements generate debug code,
   // we have to improvise to get at least one line of debug information in!
   if ((DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) {
    // Put some debug information
    if (DeeCodeWriter_Debug(&fun_writer,
     (DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE)!=0 ? DeeToken_FILE(DeeExprAst_TOKEN(self)) : NULL,
     (DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_LINE)!=0 ? DeeToken_LINE(DeeExprAst_TOKEN(self)) : 0
     ) == -1) {err_const_fun1: DeeCodeWriter_Quit(&fun_writer); return -1; }
   }
   if (DeeExprAst_Compile(
    (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
    &fun_writer,(DeeObject *)lexer,DeeCompilerConfig_Default.compiler_flags|
    DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP) == -1 ||
    DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)fun_scope,(DeeObject *)lexer) == -1 ||
    DeeCodeWriter_Ret(&fun_writer) == -1) goto err_const_fun1;
  } else if (DeeExprAst_KIND(self) == DeeExprAstKind_FUNCTION ||
             DeeExprAst_KIND(self) == DeeExprAstKind_VFUNCTION ||
             DeeExprAst_KIND(self) == DeeExprAstKind_YFUNCTION ||
             DeeExprAst_KIND(self) == DeeExprAstKind_YVFUNCTION) {
   if (DeeStmtAst_Compile(
    (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
    &fun_writer,(DeeObject *)lexer,(DeeCompilerConfig_Default.compiler_flags|
    DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~(DEE_COMPILER_FLAG_USED)) == -1 ||
    DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)fun_scope,(DeeObject *)lexer) == -1
    ) goto err_const_fun1;
   if (!DeeStmtAst_IsNoreturn(
    (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
    DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS)) {
    DEE_ASSERT(fun_writer.ob_last != OP_YIELDSTOP &&
               fun_writer.ob_last != OP_RET &&
               fun_writer.ob_last != OP_RETNONE &&
               fun_writer.ob_last != OP_RETVAREXIT &&
               "Returning statement generated noreturn opcode");
    if (DeeExprAst_KIND(self) == DeeExprAstKind_YFUNCTION ||
        DeeExprAst_KIND(self) == DeeExprAstKind_YVFUNCTION) {
     // Put a missing yield exit statement, to stop iteration
     if (DeeCodeWriter_YieldExit(&fun_writer) == -1) goto err_const_fun1;
    } else if ((fun_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_RETVAR) != 0) {
     // Put a missing return statement, to return the stored result variable
     if (DeeCodeWriter_WriteOp(&fun_writer,OP_RETVAREXIT) == -1) goto err_const_fun1;
    } else {
     // Put a missing return statement, to return none by default
     if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_const_fun1;
    }
   } else if ((DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0) {
    if (DeeCodeWriter_Unreachable(&fun_writer) == -1) goto err_const_fun1;
   }
  } else {
   // No scope cleanup as we can't put our own code here...
   if (_DeeTokenIDList_SIZE(self->ob_fun_args) == 0) {
    Dee_INCREF(func_code = Dee_EmptyCode);
   } else {
    // Needed for correct allocation of argument locals
    if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_const_fun1;
    if ((func_code = DeeCodeWriter_Pack(&fun_writer,NULL)) == NULL) goto err_const_fun1;
   }
   goto empty_const_code; // Continue by putting no code
  }
  if ((func_code = DeeCodeWriter_Pack(&fun_writer,(DeeObject *)(
   (DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FUNC)!=0
   ?DeeExprAst_FUNCTION_NAME(self):NULL))
   ) == NULL) goto err_const_fun1; // Pack the code
empty_const_code:
  DeeCodeWriter_Quit(&fun_writer);
  if (!func_code) return -1;
  func_ob = DeeFunction_New(
   func_code,Dee_EmptyTuple,_DeeTokenIDList_SIZE(self->ob_fun_args),(Dee_uint32_t)(
   ((scope_flags&DEE_SCOPE_FLAG_FOUND_YIELD) ? DEE_FUNCTION_FLAG_YILD : DEE_FUNCTION_FLAG_NONE)|
   ((scope_flags&DEE_SCOPE_FLAG_FOUND_THIS) ? DEE_FUNCTION_FLAG_THIS : DEE_FUNCTION_FLAG_NONE)|
   ((self->ob_kind == DeeExprAstKind_VFUNCTION ||
     self->ob_kind == DeeExprAstKind_VFUNCTION_EXPR ||
     self->ob_kind == DeeExprAstKind_VFUNCTION_NONE ||
     self->ob_kind == DeeExprAstKind_YVFUNCTION)
     ? DEE_FUNCTION_FLAG_VARG : DEE_FUNCTION_FLAG_NONE)));
  Dee_DECREF(func_code);
  if (!func_ob) return -1;
  Dee_DECREF(self->ob_op_a);
  Dee_XCLEAR(self->ob_op_b);
  Dee_XCLEAR(self->ob_op_c);
  Dee_XCLEAR(self->ob_op_d);
  Dee_INHERIT_REF(self->ob_const,func_ob);
  self->ob_kind = DeeExprAstKind_CONSTANT;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4061)
int _DeeExprAst_OptimizeFunctionKnownPredicates(
 DeeExprAstObject *self, struct DeeLexerObject *lexer) {
 // Try to optimize small predicates that are
 // equivalent to builtin functions. e.g.: 
 // [1] >> function add(a,b) {
 // [1] >>     return a+b;
 // [1] >> }
 // [2] >> add = function [[name("add")]](a,b) {
 // [2] >>     return a+b;
 // [2] >> }
 // [3] >> add = __add__; // from lib:<__intrin>
 // This optimization is especially useful for users
 // who don't know about <__intrin>, are too lazy to use them,
 // or simply want their code to be easier to understand by
 // not littering it with a gejillion __underscores__.
 DeeExprAstObject *single_funexpr; Dee_size_t nargs;
 int allow_raw_expr = 0,is_varg = 0; DeeObject *builtin_function;
 DeeStmtAstObject *first_stmt;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 nargs = _DeeTokenIDList_SIZE(self->ob_fun_args);
 switch (self->ob_kind) {
  default: return 0;
  case DeeExprAstKind_VFUNCTION: is_varg = 1;
  case DeeExprAstKind_FUNCTION: {
   DEE_ASSERT(DeeObject_Check(self->ob_fun_stmt) && DeeStmtAst_Check(self->ob_fun_stmt));
   first_stmt = self->ob_fun_stmt;
iter_stmt:
   while (1) switch (first_stmt->ob_kind) {
    case DeeStmtAstKind_STMTS:
     if (first_stmt->ob_stmts.ob_stmts_c != 1) return 0;
     first_stmt = first_stmt->ob_stmts.ob_stmts_v[0];
     break;
    case DeeStmtAstKind_LOOP_ONCE:
     first_stmt = first_stmt->ob_loop_once.ob_lonce_block;
     break;
    case DeeStmtAstKind_INL_STMTS:
     if (first_stmt->ob_inl_stmts.ob_inl_stmts_c != 1) return 0;
     first_stmt = first_stmt->ob_inl_stmts.ob_inl_stmts_v[0];
     break;
    case DeeStmtAstKind_SSTMT:
     first_stmt = first_stmt->ob_sstmt.ob_sstmt_ast;
     break;
    case DeeStmtAstKind_EXPR:
     if (first_stmt->ob_expr.ob_expr_ast->ob_kind != DeeExprAstKind_CODE) {
      if (!allow_raw_expr) return 0;
      single_funexpr = first_stmt->ob_expr.ob_expr_ast;
      goto funexpr_check;
     }
     first_stmt = first_stmt->ob_expr.ob_expr_ast->ob_stmt;
     break;
    case DeeStmtAstKind_SEXPR:
     if (first_stmt->ob_sexpr.ob_sexpr_ast->ob_kind != DeeExprAstKind_CODE) return 0;
     first_stmt = first_stmt->ob_sexpr.ob_sexpr_ast->ob_stmt;
     break;
    default: goto funstmt_check;
   }
funstmt_check:
   if (first_stmt->ob_kind != DeeStmtAstKind_RETURN) return 0;
   single_funexpr = first_stmt->ob_return.ob_ret_expr;
   goto funexpr_check;
  } break;
  case DeeExprAstKind_VFUNCTION_EXPR: is_varg = 1;
  case DeeExprAstKind_FUNCTION_EXPR:
   DEE_ASSERT(DeeObject_Check(self->ob_fun_expr) && DeeExprAst_Check(self->ob_fun_expr));
   single_funexpr = self->ob_fun_expr;
funexpr_check:
   if (single_funexpr->ob_kind == DeeExprAstKind_CODE) {
    allow_raw_expr = 1;
    first_stmt = single_funexpr->ob_stmt;
    goto iter_stmt;
   }
   break;
 }
 DEE_ASSERT(DeeObject_Check(single_funexpr) && DeeExprAst_Check(single_funexpr));
 if (single_funexpr->ob_kind == DeeExprAstKind_CALL) {
  // Optimize "[](...) -> fun(...)" -> "fun"
  DeeExprAstObject **argiter,**argend,*arg; Dee_size_t argi;
  // v make sure the argument list isn't a constant
  if (single_funexpr->ob_op_b->ob_kind != DeeExprAstKind_TUPLE) return 0;
  argend = (argiter = (DeeExprAstObject **)DeeTuple_ELEM(
   single_funexpr->ob_op_b->ob_args_a))+DeeTuple_SIZE(
   single_funexpr->ob_op_b->ob_args_a);
  if ((Dee_size_t)(argend-argiter) != nargs) goto nofunopt; // nope!
  // Make sure all the arguments are identical
  argi = 0; while (argiter != argend) {
   arg = *argiter++;
   if (arg->ob_kind != DeeExprAstKind_VARIABLE ||
      (arg->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
      (arg->ob_var->ob_scope != self->ob_scope) ||
      (arg->ob_var->ob_loc_id != argi++)) goto nofunopt;
  }
  Dee_INCREF(single_funexpr = single_funexpr->ob_op_a);
  DEE_LVERBOSE2R(
   "%s(%d) : Detected forward proxy function: %r\n",
   DeeToken_FILE(self->ob_token),
   DeeToken_LINE(self->ob_token)+1,single_funexpr);
  Dee_XDECREF(self->ob_token);
  Dee_XDECREF(self->ob_op_a);
  Dee_XDECREF(self->ob_op_b);
  Dee_XDECREF(self->ob_op_c);
  Dee_XDECREF(self->ob_op_d);
  Dee_XINCREF(self->ob_token = single_funexpr->ob_token);
  Dee_XINCREF(self->ob_op_a = single_funexpr->ob_op_a);
  Dee_XINCREF(self->ob_op_b = single_funexpr->ob_op_b);
  Dee_XINCREF(self->ob_op_c = single_funexpr->ob_op_c);
  Dee_XINCREF(self->ob_op_d = single_funexpr->ob_op_d);
  self->ob_kind = single_funexpr->ob_kind;
  Dee_DECREF(single_funexpr);
  return 0;
 }
nofunopt:
 if (is_varg) return 0;
 switch (nargs) {
  case 1:
   switch (single_funexpr->ob_kind) {
    case DeeExprAstKind_STR: builtin_function = (DeeObject *)&DeeString_Type; break;
    case DeeExprAstKind_REPR: builtin_function = (DeeObject *)&DeeBuiltinFunction___repr__; break;
    case DeeExprAstKind_TYPE: builtin_function = (DeeObject *)&DeeBuiltinFunction___type__; break;
    case DeeExprAstKind_COPY: builtin_function = (DeeObject *)&DeeBuiltinFunction___copy__; break;
    case DeeExprAstKind_MOVE: builtin_function = (DeeObject *)&DeeBuiltinFunction___move__; break;
    case DeeExprAstKind_NOT: builtin_function = (DeeObject *)&DeeBuiltinFunction___not__; break;
    case DeeExprAstKind_NEG: builtin_function = (DeeObject *)&DeeBuiltinFunction___neg__; break;
    case DeeExprAstKind_POS: builtin_function = (DeeObject *)&DeeBuiltinFunction___pos__; break;
    case DeeExprAstKind_INV: builtin_function = (DeeObject *)&DeeBuiltinFunction___inv__; break;
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
    case DeeExprAstKind_REF: builtin_function = (DeeObject *)&DeeBuiltinFunction___ref__; break;
    case DeeExprAstKind_DEREF: builtin_function = (DeeObject *)&DeeBuiltinFunction___deref__; break;
#endif
    case DeeExprAstKind_INC: builtin_function = (DeeObject *)&DeeBuiltinFunction___inc__; break;
    case DeeExprAstKind_DEC: builtin_function = (DeeObject *)&DeeBuiltinFunction___dec__; break;
    case DeeExprAstKind_INC_POST: builtin_function = (DeeObject *)&DeeBuiltinFunction___incpost__; break;
    case DeeExprAstKind_DEC_POST: builtin_function = (DeeObject *)&DeeBuiltinFunction___decpost__; break;
    case DeeExprAstKind_SEQ_SIZE: builtin_function = (DeeObject *)&DeeBuiltinFunction___size__; break;
    default: return 0;
   }
   // Make sure the argument to the unary operator is parameter 0
check1:
   if (single_funexpr->ob_op_a->ob_kind != DeeExprAstKind_VARIABLE ||
      (single_funexpr->ob_op_a->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
      (single_funexpr->ob_op_a->ob_var->ob_loc_id != 0) ||
      (single_funexpr->ob_op_a->ob_var->ob_scope != self->ob_scope)) return 0;
   goto set_bltin;
  case 2:
   switch (single_funexpr->ob_kind) {
    case DeeExprAstKind_ASSIGN: builtin_function = (DeeObject *)&DeeBuiltinFunction___copyassign__; break;
    case DeeExprAstKind_CMP_LO: builtin_function = (DeeObject *)&DeeBuiltinFunction___lo__; break;
    case DeeExprAstKind_CMP_LE: builtin_function = (DeeObject *)&DeeBuiltinFunction___le__; break;
    case DeeExprAstKind_CMP_EQ: builtin_function = (DeeObject *)&DeeBuiltinFunction___eq__; break;
    case DeeExprAstKind_CMP_NE: builtin_function = (DeeObject *)&DeeBuiltinFunction___ne__; break;
    case DeeExprAstKind_CMP_GR: builtin_function = (DeeObject *)&DeeBuiltinFunction___gr__; break;
    case DeeExprAstKind_CMP_GE: builtin_function = (DeeObject *)&DeeBuiltinFunction___ge__; break;
    case DeeExprAstKind_ADD: builtin_function = (DeeObject *)&DeeBuiltinFunction___add__; break;
    case DeeExprAstKind_IADD: builtin_function = (DeeObject *)&DeeBuiltinFunction___iadd__; break;
    case DeeExprAstKind_SUB: builtin_function = (DeeObject *)&DeeBuiltinFunction___sub__; break;
    case DeeExprAstKind_ISUB: builtin_function = (DeeObject *)&DeeBuiltinFunction___isub__; break;
    case DeeExprAstKind_MUL: builtin_function = (DeeObject *)&DeeBuiltinFunction___mul__; break;
    case DeeExprAstKind_IMUL: builtin_function = (DeeObject *)&DeeBuiltinFunction___imul__; break;
    case DeeExprAstKind_DIV: builtin_function = (DeeObject *)&DeeBuiltinFunction___div__; break;
    case DeeExprAstKind_IDIV: builtin_function = (DeeObject *)&DeeBuiltinFunction___idiv__; break;
    case DeeExprAstKind_MOD: builtin_function = (DeeObject *)&DeeBuiltinFunction___mod__; break;
    case DeeExprAstKind_IMOD: builtin_function = (DeeObject *)&DeeBuiltinFunction___imod__; break;
    case DeeExprAstKind_SHL: builtin_function = (DeeObject *)&DeeBuiltinFunction___shl__; break;
    case DeeExprAstKind_ISHL: builtin_function = (DeeObject *)&DeeBuiltinFunction___ishl__; break;
    case DeeExprAstKind_SHR: builtin_function = (DeeObject *)&DeeBuiltinFunction___shr__; break;
    case DeeExprAstKind_ISHR: builtin_function = (DeeObject *)&DeeBuiltinFunction___ishr__; break;
    case DeeExprAstKind_AND: builtin_function = (DeeObject *)&DeeBuiltinFunction___and__; break;
    case DeeExprAstKind_IAND: builtin_function = (DeeObject *)&DeeBuiltinFunction___iand__; break;
    case DeeExprAstKind_OR: builtin_function = (DeeObject *)&DeeBuiltinFunction___or__; break;
    case DeeExprAstKind_IOR: builtin_function = (DeeObject *)&DeeBuiltinFunction___ior__; break;
    case DeeExprAstKind_XOR: builtin_function = (DeeObject *)&DeeBuiltinFunction___xor__; break;
    case DeeExprAstKind_IXOR: builtin_function = (DeeObject *)&DeeBuiltinFunction___ixor__; break;
    case DeeExprAstKind_POW: builtin_function = (DeeObject *)&DeeBuiltinFunction___pow__; break;
    case DeeExprAstKind_IPOW: builtin_function = (DeeObject *)&DeeBuiltinFunction___ipow__; break;
    case DeeExprAstKind_CALL: builtin_function = (DeeObject *)&DeeBuiltinFunction___call__; break;
    case DeeExprAstKind_SEQ_GET: builtin_function = (DeeObject *)&DeeBuiltinFunction___getitem__; break;
    case DeeExprAstKind_IS: builtin_function = (DeeObject *)&DeeBuiltinFunction___is__; break;
    case DeeExprAstKind_IN:
     if (single_funexpr->ob_op_a->ob_kind != DeeExprAstKind_VARIABLE ||
        (single_funexpr->ob_op_a->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
        (single_funexpr->ob_op_a->ob_var->ob_loc_id != 1) ||
        (single_funexpr->ob_op_a->ob_var->ob_scope != self->ob_scope) ||
         single_funexpr->ob_op_b->ob_kind != DeeExprAstKind_VARIABLE ||
        (single_funexpr->ob_op_b->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
        (single_funexpr->ob_op_b->ob_var->ob_loc_id != 0) ||
        (single_funexpr->ob_op_b->ob_var->ob_scope != self->ob_scope)) return 0;
     builtin_function = (DeeObject *)&DeeBuiltinFunction___contains__;
     goto set_bltin;
    default: return 0;
   }
   // Make sure the argument to the binary operators are parameters 0 and 1
check2:
   if (single_funexpr->ob_op_b->ob_kind != DeeExprAstKind_VARIABLE ||
      (single_funexpr->ob_op_b->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
      (single_funexpr->ob_op_b->ob_var->ob_loc_id != 1) ||
      (single_funexpr->ob_op_b->ob_var->ob_scope != self->ob_scope)) return 0;
   goto check1;
  case 3:
   switch (single_funexpr->ob_kind) {
    case DeeExprAstKind_SEQ_SET: builtin_function = (DeeObject *)&DeeBuiltinFunction___setitem__; break;
    case DeeExprAstKind_SEQ_RANGE_GET: builtin_function = (DeeObject *)&DeeBuiltinFunction___getrange__; break;
    default: return 0;
   }
   // Make sure the argument to the binary operators are parameters 0 and 1
check3:
   if (single_funexpr->ob_op_c->ob_kind != DeeExprAstKind_VARIABLE ||
      (single_funexpr->ob_op_c->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
      (single_funexpr->ob_op_c->ob_var->ob_loc_id != 2) ||
      (single_funexpr->ob_op_c->ob_var->ob_scope != self->ob_scope)) return 0;
   goto check2;
  case 4:
   switch (single_funexpr->ob_kind) {
    case DeeExprAstKind_SEQ_RANGE_SET: builtin_function = (DeeObject *)&DeeBuiltinFunction___setrange__; break;
    default: return 0;
   }
   // Make sure the argument to the binary operators are parameters 0 and 1
   if (single_funexpr->ob_op_d->ob_kind != DeeExprAstKind_VARIABLE ||
      (single_funexpr->ob_op_d->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)==0 ||
      (single_funexpr->ob_op_d->ob_var->ob_loc_id != 3) ||
      (single_funexpr->ob_op_d->ob_var->ob_scope != self->ob_scope)) return 0;
   goto check3;
  default: break;
 }
 return 0;
set_bltin:
 DEE_LVERBOSE2R(
  "%s(%d) : Detected builtin predicate: %r\n",
  DeeToken_FILE(self->ob_token),
  DeeToken_LINE(self->ob_token)+1,builtin_function);
 DEE_ASSERT(DeeObject_Check(builtin_function));
 Dee_INCREF(builtin_function);
 Dee_XDECREF(self->ob_op_a);
 Dee_XCLEAR(self->ob_op_b);
 Dee_XCLEAR(self->ob_op_c);
 Dee_XCLEAR(self->ob_op_d);
 self->ob_kind = DeeExprAstKind_CONSTANT;
 Dee_INHERIT_REF(self->ob_const,builtin_function);
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

int _DeeLexer_ParseConstFunctionSuffix(
 struct DeeFunctionObject *func, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 struct DeeAttributeDecl *attr, unsigned int func_name) {
 DeeObject *param_names,*expr_or_stmt_ast; int has_varargs = 0;
 struct DeeScopeObject *func_scope;
 TPPTokenID old_func_name; // Function name
 if (token.tk_id == '(') {
  if (!yield()) return -1;
  if (token.tk_id == ')') { if (!yield()) return -1; goto noargs; }
  param_names = NULL;
  goto begin_args;
  while (token.tk_id == ',') {
   if (!yield()) {err_pa: Dee_XDECREF(param_names); return -1; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ')') break;
#endif
begin_args:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (_DeeTokenIDList_Add(&param_names,token.tk_id) == -1) goto err_pa;
   } else if (token.tk_id == TPP_TOK_DOTS) {
    if (_DeeTokenIDList_Add(&param_names,TPP_TOK_DOTS) == -1) goto err_pa;
    goto va_dots;
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_PARAMETER_EXPECTED_KEYWORD,
     (DeeObject *)self,token_ob,"Expected <keyword> as function parameter name") == -1) goto err_pa;
    if (_DeeTokenIDList_Add(&param_names,0) == -1) goto err_pa;
   }
   if (!yield()) goto err_pa;
  }
  if (token.tk_id == TPP_TOK_DOTS) {
va_dots:
   func->ob_flags |= DEE_FUNCTION_FLAG_VARG;
   has_varargs = 1;
   if (!yield()) goto err_pa;
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_TO_END_OF_FUNCTION_PARAMETER_LIST,(DeeObject *)self,token_ob,
    "Expected end of parameter list") == -1) goto err_pa;
  } else if (!yield()) goto err_pa;
  // Parse attributes one more time (after argument list)
  if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) goto err_pa;
 } else {noargs:
  Dee_INCREF(param_names = _DeeTokenIDList_EmptyList);
 }
 if (attr->ob_name) { // Custom function name
  DEE_ASSERT(DeeObject_Check(attr->ob_name) && DeeString_Check(attr->ob_name));
  func_name = TPPKeywordList_GetS(&self->ob_lexer.l_keywords,DeeString_STR(attr->ob_name),DeeString_SIZE(attr->ob_name),1);
  if (!func_name) goto err_pa; // Failed to allocate keyword token
 }
 // Push the new function name, so we can generate appropriate debug information
 old_func_name = self->ob_func_name;
 self->ob_func_name = func_name ? func_name : KWD___anonymous__;
 if ((func_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) { // Strong scope
err_fn_a: self->ob_func_name = old_func_name; goto err_pa;
 }
 if (DeeScope_SetupParameters((DeeObject *)func_scope,
  _DeeTokenIDList_SIZE(param_names),
  _DeeTokenIDList_ELEM(param_names),attr) == -1) {
err_fn_asc: Dee_DECREF(func_scope); goto err_fn_a;
 }
 if ((attr->ob_fun_flags&DeeFunctionFlags_CC_MASK)==DeeFunctionFlags_THISCALL) {
  if (attr->ob_super) Dee_INCREF(func_scope->ob_supertp = attr->ob_super);
  func->ob_flags |= DEE_FUNCTION_FLAG_THIS;
 }
 if (token.tk_id == TPP_TOK_ARROW) { // '->' expression style function
  if (!yield()) goto err_fn_asc;
  if (token.tk_id == '{') goto brace_function;
  expr_or_stmt_ast = DeeLexer_ParseExpression((DeeObject *)self,(
   DeeObject *)func_scope,flags&~(DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE));
  if (!expr_or_stmt_ast) goto err_fn_asc;
 } else if (token.tk_id == ':') { // '{' statement style function with result variable
  if (!yield()) goto err_fn_asc; // Strong scope
  if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_FUNCTION_COLLON,(DeeObject *)self,token_ob,
    "Expected <keyword> after 'function' ... ':'") == -1) goto err_fn_asc;
  } else { // A result variable makes yield statements illegal
   DeeLocalVarObject *result_var;
   func_scope->ob_flags |= (DEE_SCOPE_FLAG_FOUND_RETURN|DEE_SCOPE_FLAG_FOUND_RETVAR);
   result_var = (DeeLocalVarObject *)DeeScope_Lookup( // retval variable
    (DeeObject *)func_scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL);
   if (!result_var) goto err_fn_asc;
   DEE_ASSERT(DeeLocalVar_Check(result_var));
   result_var->ob_flags |= (DEE_LOCALVAR_FLAG_INITIALIZED|DEE_LOCAL_VAR_FLAG_RETVAL);
   if (!yield()) goto err_fn_asc;
  }
  if (token.tk_id != '{') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_FUNCTION_COLLON_KEYWORD,(DeeObject *)self,token_ob,
    "Expected '{' after 'function' ... ':' <keyword>") == -1) goto err_fn_asc;
  } else if (!yield()) goto err_fn_asc;
  goto stmt_func;
 } else if (token.tk_id == '{') { // '{' statement style function
brace_function:
  if (!yield()) goto err_fn_asc;
stmt_func:
  expr_or_stmt_ast = DeeLexer_ParseStatements((DeeObject *)self,(DeeObject *)func_scope,flags&~(
   DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE),'}');
  if (!expr_or_stmt_ast) goto err_fn_asc;
  // Update the function type if we encountered a yield inside
  if ((func_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_YIELD) != 0)
   func->ob_flags |= DEE_FUNCTION_FLAG_YILD;
 } else { // Empty (none) function
  expr_or_stmt_ast = NULL;
 }
 if (func_scope->ob_refc != 0) {
  if (DeeError_CompilerError(DEE_WARNING_CANNOT_GENERATE_CONSTANT_FUNCTION,(
   DeeObject *)self,token_ob,"Constant function has runtime references") == -1) {
err_fnast_asc: Dee_XDECREF(expr_or_stmt_ast); goto err_fn_asc;
  }
 } else {
  DeeCodeObject *func_code;
  struct DeeCodeWriter fun_writer = DeeCodeWriter_INIT();
  DEE_LVERBOSE2("Packing constant function: %r\n",expr_or_stmt_ast);
  if ((func_scope->ob_flags&DEE_SCOPE_FLAG_CODE_COPYABLE)!=0)
   fun_writer.ob_flags |= DEE_CODE_FLAG_COPY; // Enable the copyable bit
  if (expr_or_stmt_ast && DeeExprAst_Check(expr_or_stmt_ast)) {
   // Since only statements generate debug code,
   // we have to improvise to get at least one line of debug information in!
   if ((DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) {
    // Put some debug information
    if (DeeCodeWriter_Debug(&fun_writer,
     (DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE)!=0 ? DeeToken_FILE(DeeExprAst_TOKEN(expr_or_stmt_ast)) : NULL,
     (DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_LINE)!=0 ? DeeToken_LINE(DeeExprAst_TOKEN(expr_or_stmt_ast)) : 0
     ) == -1) {err_const_fun1: DeeCodeWriter_Quit(&fun_writer); goto err_fnast_asc; }
   }
   if (DeeExprAst_Compile(expr_or_stmt_ast,(DeeObject *)func_scope,
    &fun_writer,(DeeObject *)self,DeeCompilerConfig_Default.compiler_flags|
    DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP) == -1 ||
    DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)func_scope,(DeeObject *)self) == -1 ||
    DeeCodeWriter_Ret(&fun_writer) == -1) goto err_const_fun1;
  } else if (expr_or_stmt_ast) {
   DEE_ASSERT(DeeStmtAst_Check(expr_or_stmt_ast));
   if (DeeStmtAst_Compile(expr_or_stmt_ast,(DeeObject *)func_scope,
    &fun_writer,(DeeObject *)self,(DeeCompilerConfig_Default.compiler_flags|
    DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~(DEE_COMPILER_FLAG_USED)) == -1 ||
    DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)func_scope,(DeeObject *)self) == -1
    ) goto err_const_fun1;
   if (!DeeStmtAst_IsNoreturn(expr_or_stmt_ast,(DeeObject *)func_scope,
    DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS)) {
    DEE_ASSERT(fun_writer.ob_last != OP_YIELDSTOP &&
               fun_writer.ob_last != OP_RET &&
               fun_writer.ob_last != OP_RETNONE &&
               fun_writer.ob_last != OP_RETVAREXIT &&
               "Returning statement generated noreturn opcode");
    if ((func->ob_flags&DEE_FUNCTION_FLAG_YILD)!=0) {
     // Put a missing yield exit statement, to stop iteration
     if (DeeCodeWriter_YieldExit(&fun_writer) == -1) goto err_const_fun1;
    } else if ((func_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_RETVAR) != 0) {
     // Put a missing return statement, to return the stored result variable
     if (DeeCodeWriter_WriteOp(&fun_writer,OP_RETVAREXIT) == -1) goto err_const_fun1;
    } else {
     // Put a missing return statement, to return none by default
     if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_const_fun1;
    }
   } else if ((DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0) {
    if (DeeCodeWriter_Unreachable(&fun_writer) == -1) goto err_const_fun1;
   }
  } else {
   // No scope cleanup as we can't put our own code here...
   if (_DeeTokenIDList_SIZE(param_names) == 0) {
    Dee_INCREF(func_code = (DeeCodeObject *)Dee_EmptyCode);
   } else {
    // Needed for correct allocation of argument locals
    if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_const_fun1;
    if ((func_code = (DeeCodeObject *)DeeCodeWriter_Pack(&fun_writer,NULL)) == NULL) goto err_const_fun1;
   }
   goto empty_const_code; // Continue by putting no code
  }
  {
   DeeObject *function_name;
   if ((DeeCompilerConfig_Default.compiler_flags&DEE_COMPILER_FLAG_DEBUG_FUNC)!=0 && func_name) {
    if ((function_name = DeeString_New(TPPLexer_TokenIDStr(
     DeeLexer_LEXER(self),func_name))) == NULL) goto err_const_fun1;
   } else function_name = NULL;
   func_code = (DeeCodeObject *)DeeCodeWriter_Pack(&fun_writer,function_name);
   Dee_XDECREF(function_name);
   if (!func_code) goto err_const_fun1; // Pack the code
  }
empty_const_code:
  DeeCodeWriter_Quit(&fun_writer);
  if (!func_code) return -1;
  Dee_DECREF(func->ob_code);
  Dee_INHERIT_REF(func->ob_code,func_code);
  func->ob_argc = _DeeTokenIDList_SIZE(param_names);
 }
 Dee_XDECREF(expr_or_stmt_ast);
 Dee_DECREF(func_scope);
 Dee_DECREF(param_names);
 self->ob_func_name = old_func_name;
 return 0;
}

struct DeeFunctionObject *_DeeLexer_ParseConstFunction(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeFunctionObject *result; TPPTokenID func_name;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
 DEE_ASSERT(token.tk_id == KWD_function || token.tk_id == ']');
#else
 DEE_ASSERT(token.tk_id == KWD_function);
#endif
 if (!yield()) {err_attr: DeeAttributeDecl_Quit(&attr); return NULL; }
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
 if ((result = (DeeFunctionObject *)DeeFunction_New(
  Dee_EmptyCode,Dee_EmptyTuple,0,DEE_FUNCTION_FLAG_NONE)) == NULL) goto err_attr;
 if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  func_name = token.tk_id;
  if (_DeeScope_HasName(scope,func_name)) {
   if (DeeError_CompilerError(DEE_WARNING_CONSTANT_FUNCTION_NAME_ALREADY_USED,
    (DeeObject *)self,token_ob,"Constant function name has already been used"
    ) == -1) goto err_r_attr;
   func_name = 0;
  } else if (_DeeScope_AddDeprecatedName(
   scope,func_name,(DeeObject *)result,(DeeObject *)attr.ob_depr) == -1) goto err_r_attr;
  if (!yield()) {err_r_attr: Dee_DECREF(result); goto err_attr; }
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_r_attr;
 } else func_name = 0;
 if (_DeeLexer_ParseConstFunctionSuffix(result,self,scope,flags,&attr,func_name) == -1) goto err_r_attr;
 if (token.tk_id == '(' && func_name) {
  // NOTE: We don't have to make sure it isn't an expression function, since
  //       the expression would have consumed the '(' in any case (because of parenthesis / func-call)
  if (DeeError_CompilerErrorf(DEE_WARNING_NAMED_CONSTANT_FUNCTION_DIRECTLY_CALLED,(
   DeeObject *)self,token_ob,"Named constant function %q directly called",
   TPPLexer_TokenIDStr(DeeLexer_LEXER(self),func_name)) == -1) goto err_r_attr;
 }
 DeeAttributeDecl_Quit(&attr);
 return result;
}
DeeExprAstObject *_DeeLexer_ParseFunction(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result,*func_ast;
 DeeLocalVarObject *func_variable;
 TPPTokenID func_name; //  Function name
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 unsigned int old_uses;
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
 DEE_ASSERT(token.tk_id == KWD_function || token.tk_id == ']');
#else
 DEE_ASSERT(token.tk_id == KWD_function);
#endif
 if ((func_ast = new_ast(DeeExprAstKind_FUNCTION_EXPR,token_ob)) == NULL) {
err_attr: DeeAttributeDecl_Quit(&attr); return NULL;
 }
 if (!yield()) {err_f_ast: Dee_DECREF(func_ast); goto err_attr; }
 // Parse initial attributes
 if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_f_ast;
 if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  if ((result = new_ast(DeeExprAstKind_VARDECL,token_ob)) == NULL) goto err_f_ast;
  result->ob_op_b = func_ast; // Inherit reference (we can't set r to NULL, because we still need to initialize it)
  if ((result->ob_op_a = new_ast(DeeExprAstKind_DECLVARIABLE,token_ob)) == NULL) {
err_r: Dee_DECREF(result); goto err_attr;
  }
  func_variable = (DeeLocalVarObject *)DeeScope_Lookup(
   (DeeObject *)scope,(func_name = token.tk_id),
   DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_CONST,NULL);
  if (func_variable) {
   result->ob_kind = DeeExprAstKind_ASSIGN;
   result->ob_op_a->ob_kind = DeeExprAstKind_VARIABLE;
  } else {
   DeeObject *depr_reason = (DeeObject *)attr.ob_depr;
   func_variable = (DeeLocalVarObject *)DeeScope_Lookup(
    (DeeObject *)scope,(func_name = token.tk_id),DEE_SCOPE_LOOKUP_MODE_LOCAL,&depr_reason);
   if (!func_variable) goto err_r;
  }
  if (!DeeLocalVar_Check(func_variable)) {
   if (DeeError_CompilerError(DEE_WARNING_FUNCTION_NAME_USED_AS_CONST,(DeeObject *)self,token_ob,
    "Can't use this name for a function (already used as module/constant)") == -1) goto err_r;
unnamed_func:
   if (!yield()) goto err_r;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_r;
   result->ob_op_b = NULL;
   Dee_DECREF(result);
   goto no_name;
  }
  if ((func_variable->ob_flags&DEE_LOCALVAR_FLAG_TYPEBOUND)!=0) {
   if (func_variable->ob_type != &DeeFunction_Type) {
    if (DeeError_CompilerErrorf(DEE_WARNING_FUNCTION_HAS_ALREADY_BEEN_BOUND_TO_DIFFERENT_TYPE,
     (DeeObject *)self,token_ob,"Function was already bound to a different type (old: %s; new: %s)",
     DeeType_NAME(func_variable->ob_type),DeeType_NAME(&DeeFunction_Type)) == -1) goto err_r;
    goto unnamed_func;
   }
  } else {
   func_variable->ob_flags |= DEE_LOCALVAR_FLAG_TYPEBOUND;
   Dee_INCREF(func_variable->ob_type = &DeeFunction_Type);
  }
  func_variable->ob_flags |= DEE_LOCAL_VAR_FLAG_AST_INITIALIZED;
  if (!DeeScope_SameWeakScope((DeeObject *)func_variable->ob_scope,(DeeObject *)scope)) {
   // External scope reference (must setup reference)
   if (DeeScope_ExternalReference((DeeObject *)scope,(DeeObject *)func_variable) == -1) goto err_r;
  }
  if (!yield()) goto err_r;
  Dee_INCREF(result->ob_op_a->ob_var = func_variable);
  if (result->ob_op_a->ob_kind == DeeExprAstKind_VARIABLE)
   ++func_variable->ob_uses;
  // Parse more attributes (after function name)
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_r;
 } else {
no_name:
  func_name = 0;
  result = func_ast; // Inherit reference (we can't set r to NULL, because we still need to initialize it)
  func_variable = NULL;
 }
 DEE_ASSERT(func_ast->ob_kind == DeeExprAstKind_FUNCTION_EXPR);
 if (func_variable) old_uses = func_variable->ob_uses;
 if (_DeeLexer_ParseFunctionSuffix(func_ast,self,scope,flags,&attr,func_name) == -1) goto err_r;
 if (func_variable) func_variable->ob_uses = old_uses;
 if (token.tk_id == '(' && func_name) {
  // NOTE: We don't have to make sure it isn't an expression function, since
  //       the expression would have consumed the '(' in any case (because of parenthesis / func-call)
  if (DeeError_CompilerErrorf(DEE_WARNING_NAMED_FUNCTION_DIRECTLY_CALLED,(DeeObject *)self,token_ob,
   "Named function %q directly called",DeeString_STR(func_ast->ob_fun_name)) == -1) goto err_r;
 }
 DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP



DeeExprAstObject *_DeeLexer_ParseFunCall(
 DeeExprAstObject *func, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result,*args;
 DeeTokenObject *tok;
 DEE_ASSERT(token.tk_id == '(');
 Dee_INCREF(tok = (DeeTokenObject *)token_ob);
 if (!yield()) return NULL;
 if (token.tk_id == ')') {
  args = (DeeExprAstObject *)new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)tok);
  Dee_DECREF(tok);
  if (!args) return NULL;
  Dee_INCREF(args->ob_const = Dee_EmptyTuple);
  goto have_args;
 }
 Dee_DECREF(tok);
 if ((args = _deeparse_comma_tuple(self,scope,flags)) == NULL) return NULL;
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_TO_END_ARGUMENT_LIST,(DeeObject *)self,token_ob,
   "Expected ')' to end argument list") == -1) goto err_a;
 } else {have_args:
  if (!yield()) goto err_a;
 }
 if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
  if (_do_optimize_ast(func) == -1) goto err_a;
  if (_do_optimize_ast(args) == -1) goto err_a;
 }
 if (func->ob_kind == DeeExprAstKind_CONSTANT &&
     args->ob_kind == DeeExprAstKind_CONSTANT &&
    (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
  DeeObject *func_copy,*args_copy,*func_result;
  // Optimize constant call expression
  if (DeeType_Check(func->ob_const)) { // constructor calls must be constant
   if ((DeeType_GET_SLOT(func->ob_const,tp_flags)&DEE_TYPE_FLAG_CONST_CTOR)==0) goto ncall;
  } else if (DeeFunction_Check(func->ob_const) || (
   DeeCFunction_Check(func->ob_const) && !_DeeBuiltin_AllowConstantCall(func->ob_const))) goto ncall;
  if ((func_copy = DeeObject_DeepCopy(func->ob_const)) == NULL) {err_a: Dee_DECREF(args); return NULL; }
  if ((args_copy = DeeObject_DeepCopy(args->ob_const)) == NULL) { Dee_DECREF(func_copy); goto err_a; }
  DEE_ASSERT(DeeTuple_Check(args_copy)); // Args must be a tuple
  // Call the function
  func_result = DeeObject_Call(func_copy,args_copy);
  // Cleanup the function copy and our arguments
  Dee_DECREF(args_copy);
  Dee_DECREF(func_copy);
  // If an error occurred: handle it and compile without constant optimization
  if (!func_result) {
   if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_CALL,
    (DeeObject *)self,(DeeObject *)func->ob_token,
    "Invalid constant operator: Calling '%#q%K' will throw an '%K' at runtime",
    DeeCFunction_Check(func->ob_const)
    ? DeeCFunction_NAME(func->ob_const)
    : DeeType_NAME(Dee_TYPE(func->ob_const))
    ,DeeTuple_Types(args->ob_const),
    _DeeError_HandleReturnType()
    ) == -1) goto err_a;
   goto ncall;
  }
  // We reuse the 'args' as new constant slot
  Dee_DECREF(args->ob_const);
  Dee_INHERIT_REF(args->ob_const,func_result);
  return args;
 } else {
ncall:
  // normal call ast
  if ((result = new_ast(DeeExprAstKind_CALL,token_ob)) == NULL) return NULL;
  Dee_INCREF(result->ob_op_a = func);
  Dee_INHERIT_REF(result->ob_op_b,args);
 }
 return result;
}
#if 0
DeeExprAstObject *_DeeLexer_ParseArrayInitializer(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, struct DeeArrayTypeObject *array_type) {
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return NULL;
 DeeExprAstKind_ARRAY;
 // TODO
}
#endif


DeeExprAstObject *_DeeLexer_ParseConditionalExpressionLv2(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *temp,*result = (DeeExprAstObject *)
  DeeLexer_ParseExpressionLv2((DeeObject *)self,(DeeObject *)scope,flags);
 if (result) {
  while (result->ob_kind == DeeExprAstKind_BOOL) {
   temp = result->ob_op_a; result->ob_op_a = NULL;
   Dee_DECREF(result); result = temp;
  }
  if (result->ob_kind == DeeExprAstKind_ASSIGN ||
      result->ob_kind == DeeExprAstKind_VARDECL ||
      result->ob_kind == DeeExprAstKind_SEQ_SET ||
      result->ob_kind == DeeExprAstKind_ATTR_SET ||
      result->ob_kind == DeeExprAstKind_SEQ_RANGE_SET) {
   if (DeeError_CompilerError(DEE_WARNING_ASSIGNMENT_IN_CONDITIONAL_EXPRESSION,
    (DeeObject *)self,(DeeObject *)result->ob_token,"Assignment within conditional expression") == -1) {
    Dee_DECREF(result);
    result = NULL;
   }
  }
 }
 return result;
}

DEE_STATIC_INLINE(DeeTypeObject *) _deelexer_parse_struct_expr_for_builtin_offsetof(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTypeObject *result; DeeExprAstObject *expr_ast;
 if ((expr_ast = _deeparse_top(self,scope,flags)) == NULL) return NULL;
 if (expr_ast->ob_kind != DeeExprAstKind_CONSTANT) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_BUILTIN_OFFSETOF,
   (DeeObject *)self,(DeeObject *)expr_ast->ob_token,"Expected constant expression after '__builtin_offsetof' '(' ") == -1) return NULL;
  Dee_XCLEAR(expr_ast->ob_op_a);
  Dee_XCLEAR(expr_ast->ob_op_b);
  Dee_XCLEAR(expr_ast->ob_op_c);
  Dee_XCLEAR(expr_ast->ob_op_d);
  expr_ast->ob_kind = DeeExprAstKind_CONSTANT;
  Dee_INCREF(expr_ast->ob_const = Dee_None);
 }
 if (DeeNone_Check(expr_ast->ob_const)) {
  Dee_DECREF(expr_ast->ob_const);
  Dee_INCREF(expr_ast->ob_const = (DeeObject *)&DeeNone_Type);
 }
 if (!DeeType_Check(expr_ast->ob_const)) {
  DeeObject *const_type;
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_AFTER_BUILTIN_OFFSETOF,
   (DeeObject *)self,(DeeObject *)expr_ast->ob_token,"Expected type expression after '__builtin_offsetof' '(' (got: %s:%k)",
   DeeType_NAME(Dee_TYPE(expr_ast->ob_const)),expr_ast->ob_const) == -1) return NULL;
  Dee_INCREF(const_type = (DeeObject *)Dee_TYPE(expr_ast->ob_const));
  Dee_DECREF(expr_ast->ob_const);
  Dee_INHERIT_REF(expr_ast->ob_const,const_type);
 }
 Dee_INCREF(result = (DeeTypeObject *)expr_ast->ob_const);
 Dee_DECREF(expr_ast);
 return result;
}

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
DeeExprAstObject *_DeeLexer_ParseBuiltinExpect(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result;
 DEE_ASSERT(token.tk_id == KWD___builtin_expect);
 if ((result = new_ast(DeeExprAstKind_BUILTIN_EXPECT,token_ob)) == NULL) return NULL;
 if (!yield()) {err_r: Dee_DECREF(result); return NULL;}
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXPECT,(DeeObject *)self,token_ob,
   "Expected '(' after '__builtin_expect'") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((result->ob_op_a = _deeparse_top(self,scope,flags)) == NULL) goto err_r;
 if (token.tk_id != ',') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_EXPECT,(DeeObject *)self,token_ob,
   "Expected ',' after '__builtin_expect' '(' <expr>") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((result->ob_op_b = _deeparse_top(self,scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) goto err_r;
 if (result->ob_op_b->ob_kind != DeeExprAstKind_CONSTANT) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_BUILTIN_EXPECT,
   (DeeObject *)self,(DeeObject *)result->ob_op_b->ob_token,
   "Expected constant expression after '__builtin_expect' '(' <expr> ','") == -1) goto err_r;
  result->ob_op_b->ob_kind = DeeExprAstKind_CONSTANT;
  Dee_XCLEAR(result->ob_op_b->ob_op_a);
  Dee_XCLEAR(result->ob_op_b->ob_op_b);
  Dee_XCLEAR(result->ob_op_b->ob_op_c);
  Dee_XCLEAR(result->ob_op_b->ob_op_d);
  Dee_INCREF(result->ob_op_b->ob_const = Dee_False);
 }
 if (!DeeBool_Check(result->ob_op_b->ob_const)) {
  int bool_val;
  if ((bool_val = DeeObject_Bool(result->ob_op_b->ob_const)) == -1) {
   if (!DeeError_Handle(&DeeErrorType_NotImplemented)) goto err_r;
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_BOOL_CONSTANT_EXPRESSION_AFTER_BUILTIN_EXPECT,
    (DeeObject *)self,(DeeObject *)result->ob_op_b->ob_token,
    "Expected boolean constant expression after '__builtin_expect' '(' <expr> ',' (got: %s:%k)",
    DeeType_NAME(Dee_TYPE(result->ob_op_b->ob_const)),result->ob_op_b->ob_const) == -1) goto err_r;
   bool_val = 0;
  }
  Dee_DECREF(result->ob_op_b->ob_const);
  result->ob_op_b->ob_const = DeeBool_New(bool_val);
 }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_r;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_EXPECT,(DeeObject *)self,token_ob,
   "Expected ')' after '__builtin_expect' '(' <expr> ',' <expr> ')'") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if (result->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT) {
  // If the first expression is constant as well, we can
  // just drop the '__builtin_expect' expression and compile
  // this as a constant itself.
  DeeExprAstObject *temp = result->ob_op_a;
  int blhs = DeeObject_Bool(temp->ob_const);
  if (blhs == -1) {
   if (!DeeError_Handle(&DeeErrorType_NotImplemented)) goto err_r;
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_NOT_A_BOOL_EXPRESSION_IN_BUILTIN_EXPECT,
    (DeeObject *)self,(DeeObject *)temp->ob_token,
    "Not a boolean expression after '__builtin_expect' '(' (got: %s:%k)",
    DeeType_NAME(Dee_TYPE(temp->ob_const)),temp->ob_const) == -1) goto err_r;
   blhs = 0;
  }
  if (blhs != DeeBool_VALUE(result->ob_op_b->ob_const)) {
   // Warn in an incorrect value was expected
   if (DeeError_CompilerErrorf(DEE_WARNING_WRONG_EXPECTANCY_ON_CONSTANT_VALUE,(DeeObject *)self,token_ob,
    "Wrong expectancy in '__builtin_expect': Expecting %R to be %k",
    DeeObject_Str(temp->ob_const),result->ob_op_b->ob_const) == -1) goto err_r;
  }
  result->ob_op_a = NULL;
  Dee_DECREF(result);
  Dee_INHERIT_REF(result,temp);
 }
 return result;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
DeeExprAstObject *_DeeLexer_ParseBuiltinExists(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result; int value;
 DEE_ASSERT(token.tk_id == KWD___builtin_exists);
 if ((result = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
 if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXISTS,
   (DeeObject *)self,token_ob,"Expected '(' after '__builtin_exists'") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((value = _DeeLexer_ParseExistsExpression(self,scope,flags)) == -1) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_r;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXISTS,
   (DeeObject *)self,token_ob,"Expected ')' after '__builtin_exists' '(' <exists_expr>") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 result->ob_const = DeeBool_New(value);
 return result;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
DeeExprAstObject *_DeeLexer_ParseBuiltinOffsetof(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 Dee_size_t total_offset = 0;
 DeeTypeObject *type;
 DeeExprAstObject *result;
 DEE_ASSERT(token.tk_id == KWD___builtin_offsetof);
 if ((result = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
 if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_OFFSETOF,
   (DeeObject *)self,token_ob,"Expected '(' after '__builtin_offsetof'") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((type = _deelexer_parse_struct_expr_for_builtin_offsetof(self,scope,flags)) == NULL) goto err_r;
 if (token.tk_id != ','/* && token.tk_id != '.' && token.tk_id != TPP_TOK_COLLON_COLLON*/) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_OFFSETOF,
   (DeeObject *)self,token_ob,"Expected ',' after '__builtin_offsetof' '(' <type_expr>") == -1) {
err_ty_r: Dee_DECREF(type); goto err_r;
  }
 } else if (!yield()) goto err_ty_r;
 while (1) {
  if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_BUILTIN_OFFSETOF,
    (DeeObject *)self,token_ob,"Expected <keyword> after '__builtin_offsetof' '(' <type_expr> ','") == -1) goto err_ty_r;
  } else {
   DeeObject *member_name; Dee_size_t member_offset; DeeTypeObject *new_struct_type;
   if ((member_name = DeeString_New(TPPLexer_TokenIDStr(
    DeeLexer_LEXER(self),token.tk_id))) == NULL) goto err_ty_r;
   new_struct_type = DeeType_OffsetOf((DeeTypeObject *)type,member_name,&member_offset);
   if (!new_struct_type) {
    if (DeeError_CompilerErrorf(DEE_WARNING_MEMBER_NOT_FOUND_IN_TYPE,
     (DeeObject *)self,token_ob,"Type %s doesn't contain a member %s",
     DeeType_NAME(type),DeeString_STR(member_name)) == -1) {
     Dee_DECREF(member_name);
     goto err_ty_r;
    }
   } else {
    Dee_DECREF(type);
    Dee_INCREF(type = new_struct_type);
    total_offset += member_offset;
   }
   Dee_DECREF(member_name);
   if (!yield()) goto err_ty_r;
  }
  if (token.tk_id != '.' && token.tk_id != TPP_TOK_COLLON_COLLON) break;
  if (!yield()) goto err_ty_r;
 }
 Dee_DECREF(type);
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (token.tk_id == ',' && !yield()) goto err_r;
#endif
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_OFFSETOF,
   (DeeObject *)self,token_ob,"Expected ')' after '__builtin_offsetof' '(' <type_expr> ',' <keyword> [('.' <keyword>)...]") == -1) goto err_r;
 } else if (!yield()) goto err_r;
 if ((result->ob_const = DeeObject_New(Dee_size_t,total_offset)) == NULL) goto err_r;
 return result;
}
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
DeeExprAstObject *_DeeLexer_ParseBuiltinBound(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *result; DeeTokenObject *builtin_token;
 DEE_ASSERT(token.tk_id == KWD___builtin_bound);
 Dee_INCREF(builtin_token = (DeeTokenObject *)token_ob);
 if (!yield()) {err_tok: Dee_DECREF(builtin_token); return NULL; }
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_BOUND,
   (DeeObject *)self,token_ob,"Expected '(' after '__builtin_bound'") == -1) goto err_tok;
 } else if (!yield()) goto err_tok;
 if ((result = _deeparse_top(self,scope,flags)) == NULL) goto err_tok;
 if (result->ob_kind != DeeExprAstKind_VARIABLE) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_VARIABLE_AFTER_BUILTIN_BOUND,
   (DeeObject *)self,(DeeObject *)result->ob_token,
   "Expected variable-ast after '__builtin_bound' '(' (got: %r)",result) == -1) goto err_r;
ret_true:
  Dee_XDECREF(result->ob_op_a);
  Dee_XCLEAR(result->ob_op_b);
  Dee_XCLEAR(result->ob_op_c);
  Dee_XCLEAR(result->ob_op_d);
  Dee_INCREF(result->ob_const = Dee_True);
  result->ob_kind = DeeExprAstKind_CONSTANT;
 } else {
  DeeExprAstObject *outer_ast;
  DeeLocalVarObject *ast_var = result->ob_var;
  DEE_ASSERT(DeeObject_Check(ast_var) && DeeLocalVar_Check(ast_var));
  if ((ast_var->ob_flags&DEE_LOCAL_VAR_FLAG_STATIC)!=0) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NONSTATIC_AFTER_BUILTIN_BOUND,
    (DeeObject *)self,(DeeObject *)result->ob_token,
    "Expected non-static variable after '__builtin_bound' '('") == -1) goto err_r;
   goto ret_true;
  }
  if ((ast_var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)!=0) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NONARGUMENT_AFTER_BUILTIN_BOUND,
    (DeeObject *)self,(DeeObject *)result->ob_token,
    "Expected non-parameter variable after '__builtin_bound' '('") == -1) goto err_r;
   goto ret_true;
  }
  if ((ast_var->ob_flags&DEE_LOCAL_VAR_FLAG_AST_INITIALIZED)==0) {
   if (DeeError_CompilerError(DEE_WARNING_BUILTIN_BOUND_NEVER_INITIALIZED,
    (DeeObject *)self,(DeeObject *)result->ob_token,
    "Variable in '__builtin_bound' '(' ... ')' is never initialized") == -1) goto err_r;
   Dee_XDECREF(result->ob_op_a);
   Dee_XCLEAR(result->ob_op_b);
   Dee_XCLEAR(result->ob_op_c);
   Dee_XCLEAR(result->ob_op_d);
   Dee_INCREF(result->ob_const = Dee_False);
   result->ob_kind = DeeExprAstKind_CONSTANT;
   goto end;
  }
  if ((outer_ast = (DeeExprAstObject *)new_ast(
   DeeExprAstKind_BUILTIN_BOUND,(DeeObject *)builtin_token)) == NULL) goto err_r;
  Dee_INHERIT_REF(outer_ast->ob_op_a,result);
  Dee_INHERIT_REF(result,outer_ast);
 }
end:
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_BOUND,
   (DeeObject *)self,token_ob,"Expected ')' after '__builtin_bound' '(' <expr>") == -1) goto err_r;
 } else if (!yield()) {
err_r: Dee_DECREF(result); goto err_tok;
 }
 Dee_DECREF(builtin_token);
 return result;
}
#endif


DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeLexer_ParseExistsExpression(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DEE_A_REF DeeObject *found_object;
 DeeExprAstObject *a;
 int lookup_mode,maybe_exists = 0;
 // TODO: Special handling for:
 //     - __builtin_exists(class a)                  >> __builtin_exists(a) && (__builtin_constant_p(a) ? true : DeeClass_Check(a))
 //     - __builtin_exists(function a)               >> __builtin_exists(a) && (__builtin_constant_p(a) ? true : DeeFunction_Check(a))
 //     - __builtin_exists(struct a)                 >> __builtin_exists(a) && __builtin_constant_p(a) && (__builtin_typeof(a) == type struct)
 //     - __builtin_exists(union a)                  >> __builtin_exists(a) && __builtin_constant_p(a) && (__builtin_typeof(a) == type union)
 //     - __builtin_exists(enum a)                   >> __builtin_exists(a) && __builtin_constant_p(a) && (a in pack([u]intN_t...))
 //     - __builtin_exists(x && y)                   >> __builtin_exists(x) && __builtin_exists(y)
 //     - __builtin_exists(x || y)                   >> __builtin_exists(x) || __builtin_exists(y)
 //     - __builtin_exists(x ^^ y)                   >> __builtin_exists(x) ^^ __builtin_exists(y)
 //     - __builtin_exists(__expr_lor ? x : y)       >> __expr_lor ? __builtin_exists(x) : __builtin_exists(y)
 //     - __builtin_exists(if (__expr_lor) x else y) >> __expr_lor ? __builtin_exists(x) : __builtin_exists(y)
 //     NOTE: '__expr_lor' because that's the one that comes before '__expr_cond'
 switch (token.tk_id) {

  // Stuff we simply use the regular expression parser for
  case KWD_const:
  case KWD_long:
  case KWD_char:
  case KWD_short:
  case KWD_int:
  DEE_PARSE_TOKENID_CASE_WCHAR_T
  DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T
  DEE_PARSE_TOKENID_CASE_ATOMIC
  DEE_PARSE_TOKENID_CASE_SIGNED
  DEE_PARSE_TOKENID_CASE_UNSIGNED
  DEE_PARSE_TOKENID_CASE_UINT8
  DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32
  DEE_PARSE_TOKENID_CASE_UINT64
  DEE_PARSE_TOKENID_CASE_INT8
  DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32
  DEE_PARSE_TOKENID_CASE_INT64
  case KWD_float:
  case KWD_double:
  case KWD___builtin_object:
  case KWD___str__: case KWD_string:
  case KWD___repr__:
  case KWD___copy__:
  case KWD___move__:
  case KWD___type__:
  case KWD___weak__:
  DEE_PARSE_TOKENID_CASE_BOOL
  case KWD_object:
  case KWD_dict:
  case KWD_list:
  case KWD_tuple:
  case KWD_true:
  case KWD_false:
  DEE_PARSE_TOKENID_CASE_NONE
  case TPP_TOK_INT:
  case TPP_TOK_FLOAT:
  case TPP_TOK_CHR:
  case KWD___FUNCTION__:
  case TPP_TOK_STR:
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
  case KWD_enum:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case KWD_class:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
  case KWD_struct:
  case KWD_union:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
  case KWD_if:
#endif
  case KWD_str:
  case KWD_repr:
  case KWD_copy:
  case KWD_move:
  case KWD_weak:
#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
  case KWD_for:
  case KWD___foreach:
  case KWD_while:
  case KWD_do:
  case KWD___looponce:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
parse_expr:
#endif
  {
   DeeExprAstObject *temp_ast;
   DeeScopeObject *temp_scope;
   //if (!yield()) return -1;
   // Parse expressions in their own scope
   if ((temp_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return -1;
   temp_ast = _deeparse_top(self,temp_scope,flags);
   Dee_DECREF(temp_scope);
   if (temp_ast == NULL) return -1;
   if (temp_ast->ob_kind == DeeExprAstKind_CONSTANT) {
    Dee_INCREF(found_object = temp_ast->ob_const);
   } else {
    found_object = NULL;
    maybe_exists = 1;
   }
   Dee_DECREF(temp_ast);
  } break;

  case KWD_static:
   lookup_mode = DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_STATIC;
   goto lookup_skip_loc;
  case KWD_forward:
   // Forward always implies 'local', because declaring a variable as forward
   // requires the variable to not exist yet (and we can only create new vars locally)
   // >> So declaration must take place locally or we might
   //    accidentally reference an older variable
   lookup_mode = DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_FORWARD;
lookup_skip_loc:
   if (!yield()) return -1;
   // Ignore 'local' after 'forward'
   if (token.tk_id == KWD_local && !yield()) return -1;
   goto lookup_check;

  case KWD_local:
    if (!yield()) return -1;
    if (token.tk_id == KWD_static) {
     lookup_mode = DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_STATIC;
     goto lookup_skip_loc;
    } else if (token.tk_id == KWD_forward) {
     lookup_mode = DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_FORWARD;
     goto lookup_skip_loc;
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
    } else if (
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
     token.tk_id == KWD_struct || token.tk_id == KWD_union
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
     ||
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
     token.tk_id == KWD_enum
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
     ||
#endif
     token.tk_id == KWD_class
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
     ) {
     goto parse_expr;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
    }
    lookup_mode = DEE_SCOPE_LOOKUP_MODE_LOCAL;
    goto lookup_check;

  case KWD___nth: {
   DeeScopeObject *new_scope;
   Dee_ssize_t nth_variable;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_NTH,(DeeObject *)self,token_ob,
     "Expected '(' after '__nth'") == -1) return -1;
   } else if (!yield()) return -1;
   if ((a = _deeparse_top(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_NTH,(DeeObject *)self,token_ob,
     "Expected ')' after '__nth' '(' <expr>") == -1) {err_a: Dee_DECREF(a); return -1;}
   } else if (!yield()) goto err_a;
   if (a->ob_kind != DeeExprAstKind_CONSTANT) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
     "Expected constant expression for <expr> in '__nth' '(' <expr> ')'") == -1) goto err_a;
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    a->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INCREF(a->ob_const = Dee_None);
   }
   if (DeeObject_Cast(Dee_ssize_t,a->ob_const,&nth_variable) != 0) {
    DeeError_Handled();
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
     "Expected integral constant for <expr> in '__nth' '(' <expr> ')'") == -1) goto err_a;
   }
   Dee_DECREF(a);
#ifdef DEE_DEBUG
   a = NULL;
#endif
   if (nth_variable < 0) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_POSITIVE_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
     "Expected positive constant for <expr> in '__nth' '(' <expr> ')'") == -1) return -1;
    nth_variable = -nth_variable;
   }
   // Go to the requested scope
   new_scope = scope;
   while (1) {
    found_object = _DeeScope_GetName(new_scope,token.tk_id);
    if (found_object) { if (!nth_variable) break; --nth_variable; }
    if (new_scope->ob_weak_prev) new_scope = new_scope->ob_weak_prev; else
    if (new_scope->ob_prev) new_scope = new_scope->ob_prev; else break;
   }
   if (nth_variable) found_object = NULL;
   else Dee_XINCREF(found_object);
  } break;

  case KWD___scope: {
   DeeScopeObject *new_scope;
   Dee_ssize_t scope_offset;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_SCOPE,(DeeObject *)self,token_ob,
     "Expected '(' after '__scope'") == -1) return -1;
   } else if (!yield()) return -1;
   if ((a = _deeparse_top(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_SCOPE,(DeeObject *)self,token_ob,
     "Expected ')' after '__scope' '(' <expr>") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if (a->ob_kind != DeeExprAstKind_CONSTANT) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
     "Expected constant expression for <expr> in '__scope' '(' <expr> ')'") == -1) goto err_a;
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    a->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INCREF(a->ob_const = Dee_None);
   }
   if (DeeObject_Cast(Dee_ssize_t,a->ob_const,&scope_offset) != 0) {
    DeeError_Handled();
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
     "Expected integral constant for <expr> in '__scope' '(' <expr> ')'") == -1) goto err_a;
   }
   Dee_DECREF(a);
#ifdef DEE_DEBUG
   a = NULL;
#endif
   if (scope_offset > 0) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NEGATIVE_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
     "Expected negative constant for <expr> in '__scope' '(' <expr> ')'") == -1) return -1;
    scope_offset = -scope_offset;
   }
   // Go to the requested scope
   new_scope = scope;
   while (scope_offset++) {
    if (new_scope->ob_weak_prev) new_scope = new_scope->ob_weak_prev; else
    if (new_scope->ob_prev) new_scope = new_scope->ob_prev; else break;
   }
   found_object = DeeScope_Lookup(
    (DeeObject *)new_scope,token.tk_id,
    DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_CONST,NULL);
   Dee_XINCREF(found_object);
  } break;

  case TPP_TOK_COLLON_COLLON: // C++ style
  case KWD_global:
   if (!yield()) return -1;
   lookup_mode = DEE_SCOPE_LOOKUP_MODE_GLOBAL;
lookup_check:
   if (!DEE_TOKENID_IS_VARNAME(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_LOCAL_GLOBAL_FORWARD,
     (DeeObject *)self,token_ob,"Expected <keyword> after 'forward' / 'local' / 'global'") == -1) return -1;
    found_object = NULL;
   } else goto do_lookup;
   break;


  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  case TPP_TOK_DOTS:
    lookup_mode = DEE_SCOPE_LOOKUP_MODE_DEFAULT;
do_lookup:
    if ((found_object = DeeScope_Lookup(
     (DeeObject *)scope,token.tk_id,lookup_mode|
     DEE_SCOPE_LOOKUP_MODE_CONST,NULL)) != NULL) {
     // This doesn't count as a use
     if (DeeLocalVar_Check(found_object))
      --((DeeLocalVarObject *)found_object)->ob_uses;
     Dee_INCREF(found_object);
    }
    if (!yield()) { Dee_XDECREF(found_object); return -1; }
   } else {
    if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_EXISTS_EXPRESSION,
     (DeeObject *)self,token_ob,"Unexpected token in __builtin_exists") == -1) return -1;
    return 0;
   }
   break;
 }
 while (1) switch (token.tk_id) {
  case '.':
  case TPP_TOK_COLLON_COLLON:
   if (!yield()) goto err_found;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT_IN_EXISTS_EXPRESSION,
     (DeeObject *)self,token_ob,"Unexpected <keyword> after '.' in exists_expr") == -1) return -1;
   } else {
    if (found_object) {
     DeeObject *found_object_attr;
     if (DeeModule_Check(found_object)) {
      found_object_attr = _DeeScope_GetName(
       ((DeeModuleObject *)found_object)->ob_scope,token.tk_id);
      Dee_DECREF(found_object);
      Dee_XINCREF(found_object_attr);
      Dee_INHERIT_REF(found_object,found_object_attr);
     } else {
      found_object_attr = DeeObject_GetAttrString(
       found_object,TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id));
      if (!found_object_attr) DeeError_Handled();
      Dee_DECREF(found_object);
      Dee_INHERIT_REF(found_object,found_object_attr);
     }
    }
    if (!yield()) goto err_found;
   }
   break;

  default: goto end;
 }
end:
 if (found_object) { Dee_DECREF(found_object); return 1; }
 return maybe_exists ? 2 : 0;
err_found: Dee_XDECREF(found_object); return -1;
}

DeeTypeObject *_DeeLexer_ParseConstantType(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *base_type_ast;
 DeeTypeObject *base_type;
 if ((base_type_ast = _deeparse_top(self,scope,flags)) == NULL) return NULL;
 if (base_type_ast->ob_kind != DeeExprAstKind_CONSTANT) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_NAMED_TYPE,
   (DeeObject *)self,(DeeObject *)base_type_ast->ob_token,
   "Expected constant expression for name type") == -1) {
err_bt_ast: Dee_DECREF(base_type_ast); return NULL;
  }
none_base:
  Dee_INCREF(base_type = (DeeTypeObject *)&DeeNone_Type);
 } else if (DeeNone_Check(base_type_ast->ob_const)) {
  goto none_base;
 } else if (!DeeType_Check(base_type_ast->ob_const)) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_FOR_NAMED_TYPE,
   (DeeObject *)self,(DeeObject *)base_type_ast->ob_token,
   "Expected structured type for name type") == -1) goto err_bt_ast;
  goto none_base;
 } else {
  Dee_INCREF(base_type = (DeeTypeObject *)base_type_ast->ob_const);
 }
 Dee_DECREF(base_type_ast);
 return base_type;
}

enum DeeTypeOperationKind {
 DEE_TYPEOP_ADD_POINTER,
 DEE_TYPEOP_ADD_LVALUE,
 DEE_TYPEOP_ADD_ARRAY,
 DEE_TYPEOP_ADD_FUNCTION,
};
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeTypeOperation {
 struct DeeTypeOperation  *op_prev;  /*< [0..1][owned] Prev operation that should be performed */
 struct DeeTypeOperation  *op_next;  /*< [0..1][owned] Next operation that should be performed */
 enum DeeTypeOperationKind op_kind;
 DEE_A_REF DeeTokenObject *op_token;
 union{
  Dee_size_t                    op_array_size;
  DEE_A_REF DeeTupleObject *op_function_args;
 };
 DeeFunctionFlags           op_function_flags;
};
DEE_COMPILER_MSVC_WARNING_POP
struct DeeTypeOperations {
 struct DeeTypeOperation *ob_list;
};

DEE_STATIC_INLINE(struct DeeTypeOperation *)
DeeTypeOperation_InsertAfter(
 struct DeeTypeOperation **list,
 struct DeeTypeOperation *pos,
 enum DeeTypeOperationKind kind,
 DeeObject *_token, Dee_uintptr_t arg,
 DeeFunctionFlags flags) {
 struct DeeTypeOperation *entry;
 DEE_ASSERT(DeeToken_Check(_token));
 if ((entry = (struct DeeTypeOperation *)malloc_nz(
  sizeof(struct DeeTypeOperation))) == NULL) {
  DeeError_NoMemory();
  return NULL;
 }
 if (pos) {
  if ((entry->op_next = pos->op_next) != NULL)
   entry->op_next->op_prev = entry;
  (entry->op_prev = pos)->op_next = entry;
 } else if ((pos = *list) != NULL) {
  while (pos->op_prev) pos = pos->op_prev;
  entry->op_prev = NULL;
  (entry->op_next = pos)->op_prev = entry;
 } else {
  entry->op_next = NULL;
  entry->op_prev = NULL;
  *list = entry;
 }
 entry->op_kind = kind;
 Dee_INCREF(entry->op_token = (DeeTokenObject *)_token);
 entry->op_array_size = arg;
 entry->op_function_flags = flags;
 if (kind == DEE_TYPEOP_ADD_FUNCTION)
  Dee_INCREF((DeeObject *)(void *)arg);
 return entry;
}
DEE_STATIC_INLINE(int) DeeTypeOperation_PushBack(
 struct DeeTypeOperation **list,
 enum DeeTypeOperationKind kind,
 DeeObject *_token, Dee_uintptr_t arg,
 DeeFunctionFlags flags) {
 struct DeeTypeOperation *entry;
 DEE_ASSERT(DeeToken_Check(_token));
 if ((entry = (struct DeeTypeOperation *)malloc_nz(
  sizeof(struct DeeTypeOperation))) == NULL) {
  DeeError_NoMemory();
  return -1;
 }
 if (*list) {
  if ((entry->op_next = (*list)->op_next) != NULL)
   entry->op_next->op_prev = entry;
  if ((entry->op_prev = *list) != NULL)
   entry->op_prev->op_next = entry;
  DEE_ASSERT((*list)->op_next == entry);
  DEE_ASSERT(entry->op_prev == (*list));
 } else {
  entry->op_next = NULL;
  entry->op_prev = NULL;
 }
 *list = entry;
 entry->op_kind = kind;
 Dee_INCREF(entry->op_token = (DeeTokenObject *)_token);
 entry->op_array_size = arg;
 entry->op_function_flags = flags;
 if (kind == DEE_TYPEOP_ADD_FUNCTION)
  Dee_INCREF((DeeObject *)(void *)arg);
 return 0;
}
DEE_STATIC_INLINE(void) DeeTypeOperation_Quit(struct DeeTypeOperation *list) {
 struct DeeTypeOperation *next;
 if (list) while (1) {
  next = list->op_next;
  Dee_DECREF(list->op_token);
  if (list->op_kind == DEE_TYPEOP_ADD_FUNCTION)
   Dee_DECREF(list->op_function_args);
  free_nn(list);
  if (!next) break;
  list = next;
 }
}


static DEE_A_RET_EXCEPT(-1) int _DeeLexer_ParseNamedTypeWithBaseOpsAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, struct DeeTypeOperation **operations,
 DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, struct DeeAttributeDecl *attr);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_ParseNamedTypeWithBaseOps(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, struct DeeTypeOperation **operations,
 DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args) {
 int result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 result = _DeeLexer_ParseNamedTypeWithBaseOpsAttr(
  self,scope,flags,unsized,operations,name_token,func_args,&attr);
 DeeAttributeDecl_Quit(&attr);
 return result;
}
static DEE_A_RET_EXCEPT(-1) int _DeeLexer_ParseNamedTypeWithBaseOpsAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, struct DeeTypeOperation **operations,
 DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, struct DeeAttributeDecl *attr) {
 struct DeeTypeOperation *before_name_pos;
 if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) return -1;
 while (1) switch (token.tk_id) {
  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
y1: if (!yield()) return -1;
   break;

  case TPP_TOK_BIN_AND_ASSIGN:
   token.tk_id = '=';
   ++token.tk_str_begin;
   goto mk_lval;
  case TPP_TOK_LAND:
   if (DeeTypeOperation_PushBack(operations,DEE_TYPEOP_ADD_LVALUE,token_ob,0,0) == -1) return -1;
   // fallthrough
  case '&':mk_lval: // create lvalue type
   if (DeeTypeOperation_PushBack(operations,DEE_TYPEOP_ADD_LVALUE,token_ob,0,0) == -1) return -1;
   goto y1;

  case TPP_TOK_MUL_ASSIGN:
   token.tk_id = '=';
   ++token.tk_str_begin;
   goto mk_ptr;
  case TPP_TOK_POW_ASSIGN:
   token.tk_id = '=';
   token.tk_str_begin += 2;
   // fallthrough
  case TPP_TOK_POW:
   if (DeeTypeOperation_PushBack(operations,DEE_TYPEOP_ADD_POINTER,token_ob,0,0) == -1) return -1;
   // fallthrough
  case '*':mk_ptr: // create pointer type
   if (DeeTypeOperation_PushBack(operations,DEE_TYPEOP_ADD_POINTER,token_ob,0,0) == -1) return -1;
   goto y1;

  default: goto end_ptr;
 }
end_ptr:
 if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) return -1;
 before_name_pos = *operations;
 if (token.tk_id == '(') { // Recursive type
  DeeTokenObject *recursive_token;
  Dee_uint64_t pre_recursion_tokenpos;
  Dee_INCREF(recursive_token = (DeeTokenObject *)token_ob);
  if (!yield()) {err_rec_tok: Dee_DECREF(recursive_token); return -1; }
  if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) goto err_rec_tok;
  pre_recursion_tokenpos = self->ob_token_pos;
  if (_DeeLexer_ParseNamedTypeWithBaseOps(self,scope,flags,NULL,operations,name_token,NULL) == -1) goto err_rec_tok;
  if (self->ob_token_pos == pre_recursion_tokenpos) {
   int temp; // No information in here (assume function type)
   temp = DeeLexer_Return((DeeObject *)self,(DeeObject *)recursive_token);
   Dee_DECREF(recursive_token);
   if (temp != 0) goto err_name;
  } else {
   Dee_DECREF(recursive_token);
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_RECURSIVE_TYPE,
     (DeeObject *)self,token_ob,"Expected ')' after recursive type") == -1) goto err_name;
   } else if (!yield()) return -1;
   if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) goto err_name;
  }
 } else if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  if (token.tk_id != KWD_none) { // 'none' used as placeholder that doesn't do anything
   if (name_token) Dee_INCREF(*name_token = (DeeTokenObject *)token_ob);
  } else {
   if (name_token) *name_token = NULL;
  }
  if (!yield()) {err_name: if (name_token) Dee_XDECREF(*name_token); return -1; }
 } else {
  // Unnamed
  if (name_token) *name_token = NULL;
 }
 if (unsized) *unsized = 0;
 if (token.tk_id == '(') { // Function suffix
  int temp;
  DeeObject *args = NULL;
  DeeTypeObject *arg;
  if (!yield()) goto err_name;
  if (token.tk_id != ')') {
   int first_pass = 1;
   while (1) {
    DeeTokenObject *argname_token;
    if (token.tk_id == TPP_TOK_DOTS) {
     attr->ob_fun_flags |= DeeFunctionFlags_VARARGS;
     if (!yield()) goto err_args_name_attr;
    } else {
     int is_unsized;
     if ((arg = _DeeLexer_ParseNamedType(self,scope,flags,&is_unsized,&argname_token,NULL)) == NULL) {
err_args_name_attr: Dee_XDECREF(args); goto err_name;
     }
     if (func_args && argname_token) {
      if (_DeeTokenIDList_Add((DeeObject **)func_args,argname_token->ob_token.tk_id) == -1) {
       Dee_DECREF(arg);
       goto err_args_name_attr;
      }
     }
     if (arg != (DeeTypeObject *)&DeeNone_Type || argname_token) {
      if (arg == (DeeTypeObject *)&DeeNone_Type) {
       temp = DeeError_CompilerError(
        DEE_WARNING_NAMED_NONE_FUNCTION_ARGUMENT_IS_IGNORED,
        (DeeObject *)self,(DeeObject *)argname_token,
        "Named none-argument in function is ignored");
      } else {
       DeeTypeObject *used_arg;
       if (is_unsized) {
        if ((used_arg = DeeType_Pointer(arg)) == NULL) {
         if (DeeError_Handle(&DeeErrorType_TypeError)) {
          temp = first_pass ? 0 : DeeError_CompilerErrorf(
           DEE_WARNING_CANT_CREATE_POINTER_OF_THIS_TYPE,
           (DeeObject *)self,token_ob,"Can't create type: \"%#q *\"",
           DeeType_NAME(arg));
         } else temp = -1;
         goto used_arg_err;
        }
       } else used_arg = arg;
       temp = DeeTuple_Append(&args,(DeeObject *)used_arg);
used_arg_err:;
      }
      Dee_XDECREF(argname_token);
     } else { // Ignored unnamed non-arguments
      temp = first_pass ? 0 : DeeError_CompilerError(
       DEE_WARNING_UNNAMED_NONE_ARGUMENT_SHOULD_ONLY_BE_FIRST_ARG,
       (DeeObject *)self,token_ob,
       "Unnamed none-arguments should only appear at the start of the argument list");
     }
     Dee_DECREF(arg);
     if (temp == -1) goto err_args_name_attr;
    }
    first_pass = 0;
    if (token.tk_id != ',') break;
    if (!yield()) goto err_args_name_attr;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')') break;
#endif
   }
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOREIGN_ARGUMENT_LIST,
    (DeeObject *)self,token_ob,"Expected ')' after argument list of foreign function type"
    ) == -1) goto err_args_name_attr;
  } else if (!yield()) goto err_args_name_attr;
  if (_DeeLexer_ParseAttributeDecl(attr,self,scope,flags) == -1) goto err_args_name_attr;
  if ((before_name_pos = DeeTypeOperation_InsertAfter(operations,before_name_pos,DEE_TYPEOP_ADD_FUNCTION,token_ob,
   args ? (Dee_uintptr_t)args : (Dee_uintptr_t)Dee_EmptyTuple,attr->ob_fun_flags)) == NULL) goto err_args_name_attr;
  Dee_XDECREF(args);
 } else if (token.tk_id == '[') {
  // Array suffix
  while (1) {
   if (!yield()) goto err_name;
   if (token.tk_id == ']') {
    if (unsized) *unsized = 1; else {
     if (DeeError_CompilerError(DEE_WARNING_UNSIZED_ARRAY_NOT_SUPPORTED_IN_THIS_CONTEXT,
      (DeeObject *)self,token_ob,"Unsized arrays '[' ']' are not supported in this context"
      ) == -1) goto err_name;
    }
    if (!yield()) goto err_name;
    break;
   } else {
    DeeExprAstObject *array_size_ast;
    if ((array_size_ast = _deeparse_top(self,scope,flags)) == NULL) goto err_name;
    if (array_size_ast->ob_kind != DeeExprAstKind_CONSTANT) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_AFTER_TYPE_KEYWORD_LBRACKET,(DeeObject *)self,(DeeObject *)array_size_ast->ob_token,
      "expected constant after <type> <keyword> '['") == -1) {err_array_ast: Dee_DECREF(array_size_ast); goto err_name; }
    } else {
     Dee_size_t array_size;
     if (DeeObject_Cast(Dee_size_t,array_size_ast->ob_const,&array_size) != 0) {
      DeeError_Handled();
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_SIZE_T_AFTER_TYPE_KEYWORD_LBRACKET,
       (DeeObject *)self,(DeeObject *)array_size_ast->ob_token,
       "expected constant Dee_size_t after <type> <keyword> '['") == -1) goto err_array_ast;
     } else {
      if ((/*before_name_pos = */DeeTypeOperation_InsertAfter(operations,before_name_pos,DEE_TYPEOP_ADD_ARRAY,
       token_ob,array_size,0)) == NULL) goto err_array_ast;
     }
    }
    Dee_DECREF(array_size_ast);
   }
   if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_IN_RECURSIVE_TYPE,(DeeObject *)self,token_ob,
     "expected ']' after <type> '[' [<expr>]") == -1) goto err_name;
   } else if (!yield()) goto err_name;
   if (token.tk_id != '[') break;
  }
 }
 return 0;
}
DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeWithBaseAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, DeeTypeObject *base, struct DeeAttributeDecl *attr) {
 DeeTypeObject *new_type;
 int temp;
 struct DeeTypeOperation *operations = NULL,*op_iter;
 temp = _DeeLexer_ParseNamedTypeWithBaseOpsAttr(self,scope,flags,unsized,&operations,name_token,func_args,attr);
 if (operations) while (operations->op_prev) operations = operations->op_prev;
 if (temp == -1) {
err_r_ops:
  DeeTypeOperation_Quit(operations);
  return NULL;
 }
 op_iter = operations;
 Dee_INCREF(base);
 while (op_iter) {
  switch (op_iter->op_kind) {
   case DEE_TYPEOP_ADD_POINTER:
    if ((new_type = DeeType_Pointer(base)) == NULL) {
     if (DeeError_Handle(&DeeErrorType_TypeError)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_POINTER_OF_THIS_TYPE,
       (DeeObject *)self,(DeeObject *)op_iter->op_token,"Can't create type: \"%#q *\"",
       DeeType_NAME(base)) == -1) {
err_r_ops_base:
       Dee_DECREF(base);
err_r_ops_2:
       if (name_token) Dee_XDECREF(*name_token);
       goto err_r_ops;
      }
     } else goto err_r_ops_base;
     goto next_op;
    } else Dee_INCREF(new_type);
    break;
   case DEE_TYPEOP_ADD_LVALUE:
    if ((new_type = DeeType_LValue(base)) == NULL) {
     if (DeeError_Handle(&DeeErrorType_TypeError)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_LVALUE_OF_THIS_TYPE,
       (DeeObject *)self,(DeeObject *)op_iter->op_token,"Can't create type: \"%#q &\"",
       DeeType_NAME(base)) == -1) goto err_r_ops_base;
     } else goto err_r_ops_base;
     goto next_op;
    } else Dee_INCREF(new_type);
    break;
   case DEE_TYPEOP_ADD_FUNCTION:
#if 0
    new_type = base;
#else
    if ((new_type = DeeType_ForeignFunction(base,
     op_iter->op_function_flags,DeeTuple_SIZE(op_iter->op_function_args),
     (DeeTypeObject const *const *)DeeTuple_ELEM(op_iter->op_function_args))) == NULL) {
     if (DeeError_Handle(&DeeErrorType_TypeError)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_FUNCTION_OF_THIS_TYPE,
       (DeeObject *)self,(DeeObject *)op_iter->op_token,"Can't create type: \"%#q%k\"",
       DeeType_NAME(base),op_iter->op_function_args) == -1) goto err_r_ops_base;
     } else goto err_r_ops_base;
     goto next_op;
    } else Dee_INCREF(new_type);
#endif
    break;
   case DEE_TYPEOP_ADD_ARRAY:
    if ((new_type = DeeType_Array(base,op_iter->op_array_size)) == NULL) {
     if (DeeError_Handle(&DeeErrorType_TypeError)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_ARRAY_OF_THIS_TYPE,
       (DeeObject *)self,(DeeObject *)op_iter->op_token,"Can't create type: \"%#q[%Iu]\"",
       DeeType_NAME(base),op_iter->op_array_size) == -1) goto err_r_ops_base;
     } else goto err_r_ops_base;
     goto next_op;
    }
    Dee_INCREF(new_type);
    break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  Dee_DECREF(base);
  if (!new_type) goto err_r_ops_2;
  base = new_type;
next_op:
  op_iter = op_iter->op_next;
 }
 DeeTypeOperation_Quit(operations);
 return base;
}
DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeWithBase(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, DeeTypeObject *base) {
 DeeTypeObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 result = _DeeLexer_ParseNamedTypeWithBaseAttr(self,scope,flags,unsized,name_token,func_args,base,&attr);
 DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedType(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args) {
 DeeTypeObject *result,*new_result;
 if ((result = _DeeLexer_ParseConstantType(self,scope,flags|
  DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX|
  DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return NULL;
 new_result = _DeeLexer_ParseNamedTypeWithBase(self,scope,flags,unsized,name_token,func_args,result);
 Dee_DECREF(result);
 return new_result;
}
DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, struct DeeAttributeDecl *attr) {
 DeeTypeObject *result,*new_result;
 if ((result = _DeeLexer_ParseConstantType(self,scope,flags|
  DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX|
  DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return NULL;
 new_result = _DeeLexer_ParseNamedTypeWithBaseAttr(self,scope,flags,unsized,name_token,func_args,result,attr);
 Dee_DECREF(result);
 return new_result;
}

#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
int _DeeLexer_ParseEnumDecl(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DeeTypeObject *constant_type) {
 DeeObject *current_value;
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return -1;
 if (token.tk_id == '}') return yield() ? 0 : -1; // Empty enum decl
 if ((current_value = DeeType_NewInstanceDefault(constant_type)) == NULL) return -1;
 if (DeeStructuredType_Check(constant_type)) // Initialize structured types to ZERO
  memset(DeeStructured_DATA(current_value),0,DeeType_GET_SLOT(constant_type,tp_p_instance_size));
 while (1) {
  DeeTokenObject *name_token;
  DeeObject *new_value;
  Dee_uint64_t old_token_pos = self->ob_token_pos;
  struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
   Dee_INCREF(name_token = (DeeTokenObject *)token_ob);
   if (!yield()) goto err_ntok;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_ntok;
  } else {
   name_token = NULL;
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_FOR_ENUM_CONSTANT,
    (DeeObject *)self,token_ob,"Unnamed enum constant"
    ) == -1) goto err_attr;
  }
  if (token.tk_id == '=') {
   DeeExprAstObject *const_ast;
   DeeObject *temp;
   if (!yield()) goto err_ntok;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_ntok;
   if ((const_ast = _deeparse_top(self,scope,flags)) == NULL) goto err_ntok;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_ntok;
   if (const_ast->ob_kind != DeeExprAstKind_CONSTANT) {
    int temp_err = DeeError_CompilerError(
     DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_ENUM_INITIALIZER,
     (DeeObject *)self,(DeeObject *)const_ast->ob_token,
     "Expected constant expression for enum initializer");
    Dee_DECREF(const_ast);
    if (temp_err == -1) goto err_attr;
    goto def_nv;
   }
   Dee_INCREF(new_value = const_ast->ob_const);
   Dee_DECREF(const_ast);
   if (Dee_TYPE(new_value) != constant_type) {
    DeeObject *new_value2,*cast_args;
    if ((cast_args = DeeTuple_Pack(1,new_value)) == NULL) goto err_nt_nv;
    new_value2 = DeeType_NewInstance(constant_type,cast_args);
    Dee_DECREF(cast_args);
    if (!new_value2) {
     if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_FAILED_TO_CAST_CONSTANT,
      (DeeObject *)self,name_token ? (DeeObject *)name_token : token_ob,
      "Failed to cast enum constant %s:%k to %s (%K)",
      DeeType_NAME(Dee_TYPE(new_value)),new_value,
      DeeType_NAME(constant_type),_DeeError_HandleReturnType()) == -1) goto err_nt_nv;
    } else {
     Dee_DECREF(new_value);
     Dee_INHERIT_REF(new_value,new_value2);
    }
   }
   Dee_DECREF(current_value);
   if ((current_value = DeeObject_Copy(new_value)) == NULL) {
err_nt_nv: Dee_DECREF(new_value); goto err_nt;
   }
   if ((temp = DeeObject_Inc(current_value)) == NULL) goto no_inc;
   Dee_DECREF(current_value);
   Dee_INHERIT_REF(current_value,temp);
  } else {
def_nv:
   if ((new_value = DeeObject_IncPost(current_value)) == NULL) {
no_inc:
    if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_CANT_INCREMENT_COUNTER,
     (DeeObject *)self,token_ob,"Can't increment enum counter %s:%r: %K",
     DeeType_NAME(Dee_TYPE(current_value)),current_value,
     _DeeError_HandleReturnType()) == -1) goto err_ntok;
    // Create a new zero-initialized instance of 'constant_type'
    Dee_DECREF(current_value);
    if ((current_value = DeeType_NewInstanceDefault(constant_type)) == NULL) {err_nt: Dee_XDECREF(name_token); return -1; }
    if (DeeStructuredType_Check(constant_type)) // Initialize structured types to ZERO
     memset(DeeStructured_DATA(current_value),0,DeeType_GET_SLOT(constant_type,tp_p_instance_size));
    Dee_INCREF(new_value = current_value); // *cringe*
   }
  }
  if (name_token) {
   int temp;
   DeeObject *old_value;
   if ((old_value = _DeeScope_GetName(scope,name_token->ob_token.tk_id)) != NULL) {
    if ((temp = DeeObject_CompareEq(old_value,new_value)) == -1) DeeError_Handled();
    else if (!temp) {
     if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_NAME_ALREADY_USED,
      (DeeObject *)self,(DeeObject *)name_token,
      "Enum constant name is already used (old value: %s:%k)",
      DeeType_NAME(Dee_TYPE(old_value)),old_value) == -1) goto err_nval;
    }
   } else {
    if (_DeeScope_AddDeprecatedName(
     scope,name_token->ob_token.tk_id,
     new_value,(DeeObject *)attr.ob_depr) == -1) {
err_nval: Dee_DECREF(new_value);
err_ntok: Dee_XDECREF(name_token);
err_attr: DeeAttributeDecl_Quit(&attr);
err_cv:   Dee_DECREF(current_value);
     return -1;
    }
   }
   Dee_DECREF(name_token);
  }
  Dee_DECREF(new_value);
  DeeAttributeDecl_Quit(&attr);
  if (self->ob_token_pos == old_token_pos) {
   if (!token.tk_id) break;
   if (!yield()) goto err_cv;
  }
  if (token.tk_id != ',') break;
  if (!yield()) goto err_cv;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id == '}') break;
#endif
 }
 Dee_DECREF(current_value);
 if (token.tk_id != '}') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACE_AFTER_ENUM,
   (DeeObject *)self,token_ob,"Expected '}' after 'enum' [<keyword> [':' <expr>]] '{' [...]"
   ) == -1) return -1;
 } else if (!yield()) return -1;
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */

//////////////////////////////////////////////////////////////////////////
// Returns '1' if a modifier was skipped
DEE_STATIC_INLINE(int) _DeeLexer_SkipVisibilityModifiers(
 struct DeeLexerObject *self, struct DeeScopeObject *scope) {
 switch (token.tk_id) {
  case KWD_public:
  case KWD_private:
  case KWD_protected:
   if (DeeError_CompilerError(DEE_WARNING_IGNORING_VISIBILITY_MODIFIER_IN_THIS_CONTEXT,
    (DeeObject *)self,token_ob,"Ignoring visibility modifiers in this context"
    ) == -1) return -1;
   if (!yield()) return -1;
   return 1;
  default: break;
 }
 return 0;
}

DeeTypeObject *_DeeLexer_ParseTypeBaseDecl(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTypeObject *result;
 switch (token.tk_id) {
  case ':': // c++ / java
  case KWD_extends:
   if (!yield()) return NULL;
   if (_DeeLexer_SkipVisibilityModifiers(self,scope) == -1) return NULL;
   if ((result = _DeeLexer_ParseConstantType(self,scope,flags)) == NULL) return NULL;
   break;

#if 0
  case '(': // Pascal
   if (!yield()) return NULL;
   if (_DeeLexer_SkipVisibilityModifiers(self,scope) == -1) return NULL;
   if ((result = _DeeLexer_ParseConstantType(self,scope,flags)) == NULL) return NULL;
   if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_LPAREN_IN_BASE_DECL,
    (DeeObject *)self,token_ob,"Expected ')' after '(' <expr> in base type decl"
    ) == -1) goto err_r;
   } else if (!yield()) goto err_r;
   break;
#endif

  default: DEE_BUILTIN_UNREACHABLE();
 }
 if ((DeeType_FLAGS(result)&DEE_TYPE_FLAG_NO_SUBCLASS)!=0) {
  if (DeeError_CompilerErrorf(DEE_WARNING_CANT_DERIVE_FROM_FINAL_TYPE_IN_BASE_DECL,
   (DeeObject *)self,token_ob,"Can't derive from final type %s in base type decl",
   DeeType_NAME(result)) == -1) {
#if 0
err_r:
#endif
   Dee_DECREF(result);
   return NULL;
  }
  Dee_DECREF(result);
  Dee_INCREF(result = &DeeObject_Type);
 }
 return result;
}

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
int _DeeLexer_ParseStructDecl(
 struct DeeStructTypeObject *struct_type, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags, int union_mode,
 struct DeeAttributeDecl *decl_attr) {
 DeeExprAstObject *type_ast;
 Dee_size_t current_offset = 0;
 DEE_ASSERT(token.tk_id == '{');
 if (!yield()) return -1;
 while (token.tk_id != '}') {
  struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
  int temp,maybe_inline_struct;
  Dee_uint64_t old_token_pos = self->ob_token_pos;
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  if ((temp = _DeeLexer_SkipVisibilityModifiers(self,scope)) == -1) goto err_attr;
  if (temp == 1) {
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
   if (token.tk_id == ':') {
    if (!yield()) goto err_attr;
    if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
   }
  }
  maybe_inline_struct = token.tk_id == KWD_struct || token.tk_id == KWD_union;
  if ((type_ast = _deeparse_top(self,scope,flags|
   DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX|
   DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) {
err_attr: DeeAttributeDecl_Quit(&attr); return -1;
  }
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_ast;
  if (type_ast->ob_kind != DeeExprAstKind_CONSTANT) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_STRUCT_BASE,
    (DeeObject *)self,(DeeObject *)type_ast->ob_token,"Expected constant expression for type in struct") == -1) {
err_ast: Dee_DECREF(type_ast); goto err_attr;
   }
   Dee_XCLEAR(type_ast->ob_op_a);
   Dee_XCLEAR(type_ast->ob_op_b);
   Dee_XCLEAR(type_ast->ob_op_c);
   Dee_XCLEAR(type_ast->ob_op_d);
   type_ast->ob_kind = DeeExprAstKind_CONSTANT;
   Dee_INCREF(type_ast->ob_const = Dee_None);
  }
  if (DeeNone_Check(type_ast->ob_const)) {
   Dee_DECREF(type_ast->ob_const);
   Dee_INCREF(type_ast->ob_const = (DeeObject *)&DeeNone_Type);
  } else {
   if (!DeeType_Check(type_ast->ob_const)) {
    DeeObject *ty;
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_TYPE_EXPRESSION_FOR_STRUCT_BASE,
     (DeeObject *)self,(DeeObject *)type_ast->ob_token,"Expected type expression for type in struct (got: %s:%k)",
     DeeType_NAME(Dee_TYPE(type_ast->ob_const)),type_ast->ob_const) == -1) goto err_ast;
    Dee_INCREF(ty = (DeeObject *)Dee_TYPE(type_ast->ob_const));
    Dee_DECREF(type_ast->ob_const);
    Dee_INHERIT_REF(type_ast->ob_const,ty);
   }
  }
  while (1) {
   DeeTokenObject *member_name_token;
   DeeObject *member_name;
   DeeTypeObject *used_type;
   if ((used_type = _DeeLexer_ParseNamedTypeWithBaseAttr(self,scope,flags,NULL,
    &member_name_token,NULL,(DeeTypeObject *)type_ast->ob_const,&attr)) == NULL) goto err_ast;
   if (DeeForeignFunctionType_Check(used_type)) {
    DeeTypeObject *new_used_type;
    if (DeeError_CompilerErrorf(DEE_WARNING_FOREIGN_FUNCTION_IN_STRUCT_MUST_BE_POINTER,
     (DeeObject *)self,(DeeObject *)type_ast->ob_token,
     "Foreign function type %q in struct must be a pointer",
     DeeType_NAME(used_type)) == -1) goto err_used_type_ast_name;
    if ((new_used_type = DeeType_Pointer(used_type)) == NULL) {
err_used_type_ast_name:
     Dee_XDECREF(member_name_token);
     goto err_used_type_ast;
    }
    Dee_DECREF(used_type);
    Dee_INCREF(used_type = new_used_type);
   }
   if (!DeeStructuredType_Check(used_type)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRUCTURED_TYPE_FOR_STRUCT_BASE,
     (DeeObject *)self,member_name_token ? (DeeObject *)member_name_token : token_ob,
     "Expected structured type expression for type in struct (got: %s)",
     DeeType_NAME(used_type)) == -1) goto err_ast;
    Dee_DECREF(used_type);
    Dee_INCREF(used_type = (DeeTypeObject *)&DeeNone_Type);
   }
   if ((DeeType_FLAGS(used_type)&DEE_TYPE_FLAG_INCOMPLETE)!=0) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COMPLETE_TYPE_FOR_STRUCT_BASE,
     (DeeObject *)self,member_name_token ? (DeeObject *)member_name_token : token_ob,
     "Expected complete type for type in struct (got: %s)",
     DeeType_NAME(used_type)) == -1) goto err_ast;
   }
   if (attr.ob_name) {
    Dee_INCREF(member_name = (DeeObject *)attr.ob_name);
   } else if (member_name_token) {
    if ((member_name = DeeString_New(TPPLexer_TokenIDStr(
     DeeLexer_LEXER(self),member_name_token->ob_token.tk_id))) == NULL) {
     Dee_DECREF(member_name_token);
err_used_type_ast: Dee_DECREF(used_type); goto err_ast;
    }
   } else member_name = NULL;
   Dee_XDECREF(member_name_token);
#define ALIGN(type_align)\
do{\
 Dee_size_t _type_align = (type_align);\
 if (decl_attr->ob_alignment && decl_attr->ob_alignment < _type_align) _type_align = decl_attr->ob_alignment; else \
 if (attr.ob_alignment && attr.ob_alignment < _type_align) _type_align = attr.ob_alignment; else \
 if (_type_align > DeeStructPackingStack_GET(&self->ob_pack)) _type_align = DeeStructPackingStack_GET(&self->ob_pack);\
 current_offset = (current_offset+_type_align-1)&((Dee_size_t)(-(Dee_ssize_t)_type_align));\
}while(0)
   if (member_name) {
    if (DeeStructType_HasField((DeeTypeObject *)struct_type,member_name)) {
     temp = DeeError_CompilerErrorf(DEE_WARNING_STRUCT_MEMBER_NAME_ALREADY_USED,
      (DeeObject *)self,(DeeObject *)type_ast->ob_token,
      "Member name %k is already used in struct %k",member_name,struct_type);
    } else {
     Dee_size_t type_size;
     type_size = DeeType_GET_SLOT(used_type,tp_p_instance_size);
     ALIGN(DeeType_GET_SLOT(used_type,tp_p_instance_size));
     // TODO: bitfields
     temp = DeeStructType_AddField((DeeTypeObject *)struct_type,member_name,current_offset,used_type);
#if 1 // Change this to allow consecutive union members
     // 'union{ int x,y; int z; }' // 'x' and 'y' share the same memory address
     if (!union_mode) current_offset += type_size;
#else
     // 'union{ int x,y; int z; }' // 'x' and 'y' don't share the same memory address
     current_offset += type_size;
#endif
    }
    Dee_DECREF(member_name);
   } else if (maybe_inline_struct && DeeStructType_Check(used_type)) {
    Dee_size_t type_size;
    struct DeeStructMember *members; // Inline this struct/union
    DEE_ASSERT(!member_name);
    members = ((DeeStructTypeObject *)used_type)->tp_members;
    type_size = DeeType_GET_SLOT(used_type,tp_p_instance_size);
    ALIGN(DeeType_GET_SLOT(used_type,tp_p_instance_size));
    if (members) while (members->ob_type) {
     if (DeeStructType_HasField((DeeTypeObject *)struct_type,(DeeObject *)members->ob_name)) {
      temp = DeeError_CompilerErrorf(DEE_WARNING_STRUCT_MEMBER_NAME_ALREADY_USED,
       (DeeObject *)self,(DeeObject *)type_ast->ob_token,
       "Member name %k is already used in struct %k",members->ob_name,struct_type);
     } else {
      temp = DeeStructType_AddField(
       (DeeTypeObject *)struct_type,(DeeObject *)members->ob_name,
       current_offset+members->ob_offset,(DeeTypeObject *)members->ob_type);
     }
     if (temp == -1) goto err_used_type_ast;
     ++members;
    }
    current_offset += type_size;
   } else { // Unnamed field
    ALIGN(DeeType_GET_SLOT(used_type,tp_p_instance_size));
    current_offset += DeeType_GET_SLOT(used_type,tp_p_instance_size);
   }
#undef ALIGN
   Dee_DECREF(used_type);
   if (token.tk_id != ',') break;
   if (!yield()) goto err_ast;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ';') break;
#endif
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_ast;
  }
  if (union_mode) current_offset = 0;
  if (token.tk_id != ';') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_TYPE_KEYWORD_IN_STRUCT,
    (DeeObject *)self,token_ob,"Expected ';' after type+keyword in struct decl") == -1) goto err_ast;
  } else if (!yield()) goto err_ast;
  Dee_DECREF(type_ast);
  DeeAttributeDecl_Quit(&attr);
  if (self->ob_token_pos == old_token_pos) {
   if (!token.tk_id) break;
   if (!yield()) return -1;
  }
 }
 if (!yield()) return -1; // Yield the '}'
 return 0;
}
#endif


static int _deeexprast_optimize_get_operators(DeeLexerObject *lexer, DeeExprAstObject *self) {
#if 1
 DeeObject *a_copy,*b_copy,*c_copy,*cresult;
 // Constant optimization for these operators must be performed
 // manually because we needed to check if these wouldn't have
 // actually been tp_seq_set, tp_attr_set or tp_seq_range_set.
 // NOTE: We might be optimizing away the AST of a del statement,
 //       but there wouldn't be any point in calling 'del' with a
 //       constant argument anyways!
 switch (self->ob_kind) {

  case DeeExprAstKind_ATTR_GET:
   if (self->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT) {
    if ((a_copy = DeeObject_DeepCopy(self->ob_op_a->ob_const)) == NULL) return -1;
    cresult = DeeObject_GetAttrConst(a_copy,(DeeObject *)self->ob_member);
    if (cresult) {
     Dee_DECREF(a_copy);
     Dee_DECREF(self->ob_op_a);
     Dee_CLEAR(self->ob_member);
     self->ob_kind = DeeExprAstKind_CONSTANT;
     Dee_INHERIT_REF(self->ob_const,cresult);
    } else {
     DeeTypeObject *error_type = _DeeError_HandleReturnType();
     cresult = DeeObject_GetAttr(a_copy,(DeeObject *)self->ob_member);
     Dee_DECREF(a_copy);
     if (cresult) {
      Dee_DECREF(error_type);
      Dee_DECREF(cresult);
     } else {
      DeeError_Handled();
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_ATTRGET,
       (DeeObject *)lexer,(DeeObject *)self->ob_token,
       "Invalid constant operator: Calling '%#q.%#q' will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(self->ob_op_a->ob_const)),
       DeeString_STR(self->ob_member),error_type) == -1) return -1;
     }
    }
   }
   break;
  case DeeExprAstKind_SEQ_GET:
   if (self->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
       self->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT) {
    if ((a_copy = DeeObject_DeepCopy(self->ob_op_a->ob_const)) == NULL) return -1;
    if ((b_copy = DeeObject_DeepCopy(self->ob_op_b->ob_const)) == NULL) { Dee_DECREF(a_copy); return -1; }
    cresult = DeeObject_GetItem(a_copy,b_copy);
    Dee_DECREF(b_copy);
    Dee_DECREF(a_copy);
    if (cresult) {
     Dee_DECREF(self->ob_op_a);
     Dee_CLEAR(self->ob_op_b);
     self->ob_kind = DeeExprAstKind_CONSTANT;
     Dee_INHERIT_REF(self->ob_const,cresult);
    } else {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_SEQGET,
      (DeeObject *)lexer,(DeeObject *)self->ob_token,
      "Invalid constant operator: Calling '%#q[%#q]' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(self->ob_op_a->ob_const)),
      DeeType_NAME(Dee_TYPE(self->ob_op_b->ob_const)),
      _DeeError_HandleReturnType()
      ) == -1) return -1;
    }
   }
   break;
  case DeeExprAstKind_SEQ_RANGE_GET:
   if (self->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
       (!self->ob_op_b || self->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT) &&
       (!self->ob_op_c || self->ob_op_c->ob_kind == DeeExprAstKind_CONSTANT)) {
    if ((a_copy = DeeObject_DeepCopy(self->ob_op_a->ob_const)) == NULL) return -1;
    if (self->ob_op_b) {
     if ((b_copy = DeeObject_DeepCopy(self->ob_op_b->ob_const)) == NULL) { err_aca: Dee_DECREF(a_copy); return -1; }
    } else b_copy = DeeNone_New();
    if (self->ob_op_c) {
     if ((c_copy = DeeObject_DeepCopy(self->ob_op_c->ob_const)) == NULL) { Dee_DECREF(b_copy); goto err_aca; }
    } else c_copy = DeeNone_New();
    cresult = DeeObject_GetRangeItem(a_copy,b_copy,c_copy);
    Dee_DECREF(c_copy);
    Dee_DECREF(b_copy);
    Dee_DECREF(a_copy);
    if (cresult) {
     Dee_DECREF(self->ob_op_a);
     Dee_XCLEAR(self->ob_op_b);
     Dee_XCLEAR(self->ob_op_c);
     self->ob_kind = DeeExprAstKind_CONSTANT;
     Dee_INHERIT_REF(self->ob_const,cresult);
    } else {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_SEQRANGEGET,
      (DeeObject *)lexer,(DeeObject *)self->ob_token,
      "Invalid constant operator: Calling '%#q[%#q:%#q]' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(self->ob_op_a->ob_const)),
      DeeType_NAME(Dee_TYPE(self->ob_op_b ? self->ob_op_b->ob_const : Dee_None)),
      DeeType_NAME(Dee_TYPE(self->ob_op_c ? self->ob_op_c->ob_const : Dee_None)),
      _DeeError_HandleReturnType()
      ) == -1) return -1;
    }
   }
   break;
  default: break;
 }
#endif
 return 0;
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4701) // 'kind' is always initialized
#endif
DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_unary(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind;
 DeeExprAstObject *a,*r;
 DeeObject *c;
again:
 switch (token.tk_id) {

  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
#if 1
   if (!yield()) return NULL;
   goto again;
#endif

//////////////////////////////////////////////////////////////////////////
// Builtin constants
  case KWD_long:
  case KWD_char:
  case KWD_short:
  case KWD_int:
  DEE_PARSE_TOKENID_CASE_WCHAR_T
  DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T
  DEE_PARSE_TOKENID_CASE_ATOMIC
  DEE_PARSE_TOKENID_CASE_SIGNED
  DEE_PARSE_TOKENID_CASE_UNSIGNED
  DEE_PARSE_TOKENID_CASE_UINT8
  DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32
  DEE_PARSE_TOKENID_CASE_UINT64
  DEE_PARSE_TOKENID_CASE_INT8
  DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32
  DEE_PARSE_TOKENID_CASE_INT64
   if ((a = _DeeLexer_ParseType(self,scope,flags)) == NULL) return NULL;
   break;
  case KWD_float: c = (DeeObject *)&DeeFloat_Type; goto cst;
  case KWD_double: c = (DeeObject *)&DeeDouble_Type; goto cst;
  case KWD___builtin_object: c = (DeeObject *)&DeeBuiltinFunction___builtin_object; goto cst;
  case KWD___str__: case KWD_string: c = (DeeObject *)&DeeString_Type; goto cst;
  case KWD___repr__: c = (DeeObject *)&DeeBuiltinFunction___repr__; goto cst;
  case KWD___copy__: c = (DeeObject *)&DeeBuiltinFunction___copy__; goto cst;
  case KWD___move__: c = (DeeObject *)&DeeBuiltinFunction___move__; goto cst;
  case KWD___type__: c = (DeeObject *)&DeeBuiltinFunction___type__; goto cst;
  case KWD___weak__: c = (DeeObject *)&DeeWeakref_Type; goto cst;
  DEE_PARSE_TOKENID_CASE_BOOL c = (DeeObject *)&DeeBool_Type; goto cst;
  case KWD_object:c = (DeeObject *)&DeeObject_Type; goto cst;
  case KWD_dict:  c = (DeeObject *)&DeeDict_Type; goto cst;
  case KWD_list:  c = (DeeObject *)&DeeList_Type; goto cst;
  case KWD_set:   c = (DeeObject *)&DeeSet_Type; goto cst;
  case KWD_tuple: c = (DeeObject *)&DeeTuple_Type; goto cst;
  case KWD_true:  c = Dee_True; goto cst;
  case KWD_false: c = Dee_False; goto cst;
  DEE_PARSE_TOKENID_CASE_NONE push_none: c = Dee_None; cst:
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
   Dee_INCREF(a->ob_const = c);
   if (!yield()) goto err_a;
   break;

  case KWD_pack: {
   int has_paren;
   DeeObject *pack_token;
   Dee_INCREF(pack_token = token_ob);
   if (!yield()) {err_pack_tok: Dee_DECREF(pack_token); return NULL;}
   if ((has_paren = token.tk_id == '(') != 0) {
    if (!yield()) goto err_pack_tok;
    if (token.tk_id == ')') {
     if (!yield()) goto err_pack_tok;
     a = new_ast(DeeExprAstKind_CONSTANT,pack_token);
     Dee_DECREF(pack_token);
     if (!a) return NULL;
     Dee_INCREF(a->ob_const = Dee_EmptyTuple);
     break;
    }
   }
   Dee_DECREF(pack_token);
   // Parse a tuple expression
   if ((a = _deeparse_comma_tuple(self,scope,has_paren ? (flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX)) : flags)) == NULL) return NULL;
   if (has_paren) {
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_PACK,(DeeObject *)self,token_ob,"Expected ')' after 'pack'") == -1) goto err_a;
    } else if (!yield()) goto err_a;
   }
  } break;


  case TPP_TOK_INT:
  case TPP_TOK_FLOAT:
  case TPP_TOK_STR:
  case TPP_TOK_CHR:
  case KWD___FUNCTION__:
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
   switch (token.tk_id) {
    case TPP_TOK_CHR:
    case TPP_TOK_INT:   a->ob_const = DeeToken_EvalInt(token_ob,(DeeObject *)self); break;
    case TPP_TOK_FLOAT: a->ob_const = DeeToken_EvalFloat(token_ob,(DeeObject *)self); break;
     // v Must use lexer because of preprocessor string concat
    case TPP_TOK_STR:
    case KWD___FUNCTION__:
     a->ob_const = DeeLexer_ParseString((DeeObject *)self);
     goto a_str; // skip the yield(), because 'DeeLexer_ParseString' yielded its token(s)
    default: DEE_BUILTIN_UNREACHABLE();
   }
   if (!yield()) goto err_a;
a_str: // after_str
   if (!a->ob_const) goto err_a;
   break;

  case KWD___func__:
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if ((a->ob_const = DeeString_New(TPPLexer_TokenIDStr(DeeLexer_LEXER(self),self->ob_func_name))) == NULL) goto err_a;
   break;

/*
  case KWD___dex:
   if (!yield()) return NULL;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER___DEX,(
     DeeObject *)self,token_ob,"Expected <keyword> after '__dex'") == -1) return NULL;
    goto push_none;
   }
   if ((a = new_ast(DeeExprAstKind_DEX,token_ob)) == NULL) return NULL;
   if ((a->ob_dexname = (DeeStringObject *)DeeString_New(TPPLexer_TokenIDStr(
    DeeLexer_LEXER(self),token.tk_id))) == NULL) goto err_a;
   if (!yield()) goto err_a;
   break;
*/

  case KWD_function:
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
func_decl:
#endif
   if ((a = _DeeLexer_ParseFunction(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;

  case KWD__Generic:
   if ((a = _DeeLexer_ParseGeneric(self,scope,flags)) == NULL) return NULL;
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
  case KWD___noop:
  case KWD___builtin_noop: // '__builtin_noop' '(' [','...] [<expr> [','...]]... ')'
   if ((a = _DeeLexer_ParseBuiltinNoop(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
  case KWD___builtin_choose_expr: // '__builtin_choose_expr' '(' <cexpr> ',' <expr> ',' <expr> ')'
   if ((a = _DeeLexer_ParseBuiltinChooseExpr(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
  case KWD___builtin_constant_p: // '__builtin_constant_p' '(' <expr> ')'
   if ((a = _DeeLexer_ParseBuiltinConstantP(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case KWD___builtin_expect: // '__builtin_expect' '(' <expr> ')'
   if ((a = _DeeLexer_ParseBuiltinExpect(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case KWD___builtin_help:
   if ((a = new_ast(DeeExprAstKind_BUILTIN_HELP,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_HELP,
     (DeeObject *)self,token_ob,"Expected '(' after '__builtin_help'") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   ++self->ob_suppress_Wcop;
   a->ob_op_a = _deeparse_top(self,scope, // v we need that subscript access!
    flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX|DEE_PARSER_FLAG_OPTIMIZE_CONST_SUBSCRIPT));
   --self->ob_suppress_Wcop;
   if (a->ob_op_a == NULL) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_a;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_HELP,
     (DeeObject *)self,token_ob,"Expected ')' after '__builtin_help' '(' <expr>") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P
  case KWD___builtin_types_compatible_p: {
   DeeTypeObject *t1,*t2; int is_compatible;
   if (!yield()) return NULL;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
     (DeeObject *)self,token_ob,"Expected '(' after '__builtin_types_compatible_p'") == -1) return NULL;
   } else if (!yield()) return NULL;
   if ((a = _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   if (token.tk_id != ',') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COMMA_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
     (DeeObject *)self,token_ob,"Expected ',' after '__builtin_types_compatible_p' '(' <expr>") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if ((r = _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_ra;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_COMPATIBLE_TYPES_P,
     (DeeObject *)self,token_ob,"Expected ')' after '__builtin_types_compatible_p' '(' <expr> ',' <expr>") == -1) goto err_ra;
   } else if (!yield()) goto err_ra;
   if (a->ob_kind != DeeExprAstKind_CONSTANT) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_1,
     (DeeObject *)self,token_ob,"'__builtin_types_compatible_p' expects a constant expression as its first argument") == -1) goto err_ra;
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    a->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INCREF(a->ob_const = Dee_None);
   }
   if (r->ob_kind != DeeExprAstKind_CONSTANT) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_2,
     (DeeObject *)self,token_ob,"'__builtin_types_compatible_p' expects a constant expression as its second argument") == -1) goto err_ra;
    Dee_XCLEAR(r->ob_op_a);
    Dee_XCLEAR(r->ob_op_b);
    Dee_XCLEAR(r->ob_op_c);
    Dee_XCLEAR(r->ob_op_d);
    r->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INCREF(r->ob_const = Dee_None);
   }
   if (DeeNone_Check((t1 = (DeeTypeObject *)a->ob_const))) t1 = (DeeTypeObject *)&DeeNone_Type;
   if (DeeNone_Check((t2 = (DeeTypeObject *)r->ob_const))) t2 = (DeeTypeObject *)&DeeNone_Type;
   if (!DeeType_Check(t1)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_1,
     (DeeObject *)self,token_ob,"'__builtin_types_compatible_p' expects a type as its first argument") == -1) goto err_ra;
    t1 = Dee_TYPE(t1);
   }
   if (!DeeType_Check(t2)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_FOR_BUILTIN_COMPATIBLE_TYPES_P_2,
     (DeeObject *)self,token_ob,"'__builtin_types_compatible_p' expects a type as its second argument") == -1) goto err_ra;
    t2 = Dee_TYPE(t2);
   }
   is_compatible = (t1 == t2);
   Dee_DECREF(a->ob_const);
   a->ob_const = DeeBool_New(is_compatible);
   Dee_DECREF(r);
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
  case KWD_noexcept:
  case KWD___builtin_noexcept:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
  case KWD___builtin_noreturn:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
  case KWD___builtin_noeffect:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE
  case KWD___builtin_predictable:
#endif
  {
   TPPTokenID mode = token.tk_id;
   int check_result;
   if (!yield()) return NULL;
   if (token.tk_id != '(') {
    if (DeeError_CompilerErrorf(
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
     mode == KWD___builtin_noexcept ? DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOEXCEPT :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
     mode == KWD___builtin_noreturn ? DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NORETURN :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
     mode == KWD___builtin_noeffect ? DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_NOEFFECT :
#endif
     DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_PREDICTABLE,(DeeObject *)self,token_ob,
     "Expected '(' after '%s'",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),mode)) == -1) return NULL;
   } else if (!yield()) return NULL;
   ++self->ob_suppress_Wcop;
   a = _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX));
   --self->ob_suppress_Wcop;
   if (a == NULL) return NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_a;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
     mode == KWD___builtin_noexcept ? DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NOEXCEPT :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
     mode == KWD___builtin_noreturn ? DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NORETURN :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
     mode == KWD___builtin_noeffect ? DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_NOEFFECT :
#endif
     DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_PREDICTABLE,(DeeObject *)self,token_ob,
     "Expected ')' after '%s' '(' <expr>",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),mode)) == -1) goto err_a;
   } else if (!yield()) goto err_a;
   check_result =
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
    mode == KWD___builtin_noexcept ? DeeExprAst_IsNoexcept((DeeObject *)a,(DeeObject *)scope) :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN
    mode == KWD___builtin_noreturn ? DeeExprAst_IsNoreturn((DeeObject *)a,(DeeObject *)scope,(unsigned int)(
     (flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0
     ? (DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS|DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE)
     : (DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS))) :
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT
    mode == KWD___builtin_noeffect ? DeeExprAst_IsNoeffect((DeeObject *)a,(DeeObject *)scope) :
#endif
    (DeeExprAst_PredictType((DeeObject *)a,(DeeObject *)scope) != NULL);
   Dee_XCLEAR(a->ob_op_a);
   Dee_XCLEAR(a->ob_op_b);
   Dee_XCLEAR(a->ob_op_c);
   Dee_XCLEAR(a->ob_op_d);
   a->ob_kind = DeeExprAstKind_CONSTANT;
   a->ob_const = DeeBool_New(check_result);
  } break;
#endif


#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case KWD___builtin_alloca: // '__builtin_alloca' '(' <expr> ')'
   if ((a = _DeeLexer_ParseBuiltinAlloca(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST
  case KWD___builtin_exprast: // '__builtin_exprast' '(' <expr> ')'
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_EXPRAST,
     (DeeObject *)self,token_ob,"Expected '(' after '__builtin_exprast'") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if ((a->ob_const = (DeeObject *)_deeparse_top(self,scope,flags)) == NULL) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_a;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_EXPRAST,
     (DeeObject *)self,token_ob,"Expected ')' after '__builtin_exprast' '(' <expr>") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case KWD___builtin_unreachable: kind = DeeExprAstKind_BUILTIN_UNREACHABLE; goto builtin_keyword_noargs;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case KWD___builtin_breakpoint: kind = DeeExprAstKind_BUILTIN_BREAKPOINT;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
builtin_keyword_noargs:
#endif
  {
   if ((a = new_ast(kind,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if (token.tk_id != '(') {
    if (DeeError_CompilerErrorf(
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
     kind == DeeExprAstKind_BUILTIN_UNREACHABLE ? DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_UNREACHABLE :
#endif
     DEE_WARNING_EXPECTED_LPAREN_AFTER_BUILTIN_BREAKPOINT,
     (DeeObject *)self,token_ob,"Expected '(' after '%s'",
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
     kind == DeeExprAstKind_BUILTIN_UNREACHABLE ? "__builtin_unreachable" :
#endif
     "__builtin_breakpoint") == -1) goto err_a;
   } else if (!yield()) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && !yield()) goto err_a;
#endif
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
     kind == DeeExprAstKind_BUILTIN_UNREACHABLE ? DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_UNREACHABLE :
#endif
     DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_BREAKPOINT,
     (DeeObject *)self,token_ob,"Expected ')' after '%s' '('",
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
     kind == DeeExprAstKind_BUILTIN_UNREACHABLE ? "__builtin_unreachable" :
#endif
     "__builtin_breakpoint") == -1) goto err_a;
   } else if (!yield()) goto err_a;
  } break;
#endif

  case '[': // '[' [<expr> [',' <expr>]...] ']'
#if DEE_CONFIG_LANGUAGE_HAVE_LAMBDA_SYNTAX
  {
   DeeTokenObject *tok_1,*tok_2;
   Dee_INCREF(tok_1 = (DeeTokenObject *)token_ob);
   if (!yield()) {err_tok1: Dee_DECREF(tok_1); return NULL; }
   if (token.tk_id == '&' || token.tk_id == '=') {
    Dee_DECREF(tok_1);
    Dee_INCREF(tok_1 = (DeeTokenObject *)token_ob);
    if (!yield()) goto err_tok1;
    if (token.tk_id != ']') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_AND_FOR_LAMBDA,
      (DeeObject *)self,token_ob,"Expected ']' after '[' '%c'",tok_1->ob_token.tk_id == '&' ? '&' : '='
      ) == -1) goto err_tok1;
     if (DeeLexer_Return((DeeObject *)self,token_ob) == -1) goto err_tok1;
    }
    Dee_DECREF(tok_1);
    goto func_decl;
   } else if (token.tk_id == ']') {
    TPPTokenID tok_id;
    Dee_INCREF(tok_2 = (DeeTokenObject *)token_ob);
    if (!yield()) {err_tok2: Dee_DECREF(tok_2); goto err_tok1; }
    tok_id = token.tk_id;
    if (DeeLexer_Return((DeeObject *)self,(DeeObject *)tok_2) == -1) goto err_tok2;
    Dee_DECREF(tok_2);
    if (tok_id == '{' || tok_id == '(' || tok_id == TPP_TOK_ARROW ||
        TPPTokenID_IS_KEYWORD(tok_id)) {
     Dee_DECREF(tok_1);
     goto func_decl;
    }
   }
   if (DeeLexer_Return((DeeObject *)self,(DeeObject *)tok_1) == -1) goto err_tok1;
   Dee_DECREF(tok_1);
  }
#endif
   if ((a = _DeeLexer_ParseList(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;

  case '(': { // '(' (<expr> [',' <expr>]..., '{' [<stmt>]... '}') ')'
   int is_double_paren;
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
   if ((a = _DeeLexer_ParseTupleOrParenOrCode(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX),&is_double_paren)) == NULL) return NULL;
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
   if ((a = _DeeLexer_ParseTupleOrParen(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX),&is_double_paren)) == NULL) return NULL;
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
   if ((a->ob_kind == DeeExprAstKind_CONSTANT && DeeType_Check(a->ob_const)) &&
       token.tk_id != ')' && !is_double_paren) { // Cast expression
    if (token.tk_id == '{' && (flags&DEE_PARSER_FLAG_NO_BRACE_INIT)==0) goto type_init; // Type Initializer
    if ((r = new_ast(DeeExprAstKind_CALL,(DeeObject *)a->ob_token)) == NULL) goto err_a;
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(a,r);
    DEE_ASSERT(a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT);
    if ((r = _deeparse_unary(self,scope,flags)) == NULL) goto err_a;
    if (r->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE) != 0) {
     DeeObject *args = DeeTuple_Pack(1,r->ob_const);
     if (!args) goto err_ra;
     if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
         DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_op_a->ob_const)
         /*&&a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT*/) {
      DeeObject *cast_result = DeeObject_Call(a->ob_op_a->ob_const,args);
      if (!cast_result) {
       if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_CAST,
        (DeeObject *)self,(DeeObject *)a->ob_token,
        "Invalid constant operator: Casting an '%#q' to '%#q' will throw an '%K' at runtime",
        DeeType_NAME(Dee_TYPE(r->ob_const)),
        DeeType_NAME(a->ob_op_a->ob_const),
        _DeeError_HandleReturnType()
        ) == -1) { Dee_DECREF(r); Dee_DECREF(args); goto err_a; }
       goto cst_cast_def;
      }
      Dee_DECREF(args);
      Dee_DECREF(r);
      Dee_DECREF(a->ob_op_a);
      Dee_INHERIT_REF(a->ob_const,cast_result);
      a->ob_kind = DeeExprAstKind_CONSTANT;
     } else {
cst_cast_def:
      Dee_DECREF(r->ob_const);
      Dee_INHERIT_REF(r->ob_const,args);
      Dee_INHERIT_REF(a->ob_op_b,r);
     }
    } else {
     if ((a->ob_op_b = new_ast(DeeExprAstKind_TUPLE,(DeeObject *)a->ob_token)) == NULL) {err_ra: Dee_DECREF(r); goto err_a; }
     a->ob_op_b->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,r);
     Dee_DECREF(r);
     if (a->ob_op_b->ob_args_a == NULL) goto err_a;
    }
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
  case KWD_if: // 'if' '(' <expr> ')' (<expr>, [<expr>] 'else' <expr>)
   if ((a = _DeeLexer_ParseIfExpr(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
  case KWD_for:
  case KWD___foreach:
   if ((a = _DeeLexer_ParseForGenerator(self,scope,flags)) == NULL) return NULL;
   break;
  case KWD_while:
   if ((a = _DeeLexer_ParseWhileGenerator(self,scope,flags)) == NULL) return NULL;
   break;
  case KWD_do:
  case KWD___looponce:
   if ((a = _DeeLexer_ParseDoGenerator(self,scope,flags)) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  // Split tokens in two, for ref/deref operators
  case TPP_TOK_LAND:           token.tk_id = TPP_TOK_BIN_AND; ++token.tk_str_begin; kind = DeeExprAstKind_REF; goto unary_noyield;
  case TPP_TOK_BIN_AND_ASSIGN: token.tk_id = TPP_TOK_ASSIGN; ++token.tk_str_begin; kind = DeeExprAstKind_REF; goto unary_noyield;
  case TPP_TOK_POW:            token.tk_id = TPP_TOK_MUL; ++token.tk_str_begin; kind = DeeExprAstKind_DEREF; goto unary_noyield;
  case TPP_TOK_MUL_ASSIGN:     token.tk_id = TPP_TOK_ASSIGN; ++token.tk_str_begin; kind = DeeExprAstKind_DEREF; goto unary_noyield;
  case TPP_TOK_POW_ASSIGN:     token.tk_id = TPP_TOK_MUL_ASSIGN; ++token.tk_str_begin; kind = DeeExprAstKind_DEREF; goto unary_noyield;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
  case KWD___builtin_exists:
   if ((a = _DeeLexer_ParseBuiltinExists(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
  case KWD___builtin_offsetof:
   if ((a = _DeeLexer_ParseBuiltinOffsetof(self,scope,flags&~(
    DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case KWD___builtin_bound:
   if ((a = _DeeLexer_ParseBuiltinBound(self,scope,flags)) == NULL) return NULL;
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
  case KWD_enum:parse_enum:{
   DeeTokenObject *enum_name;
   struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
   int is_enum_class;
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) { DeeAttributeDecl_Quit(&attr); return NULL; }
   if (!yield()) {err_en_attr_a: DeeAttributeDecl_Quit(&attr); goto err_a; }
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_en_attr_a;
   if (token.tk_id == KWD_final) {
    if (!yield()) goto err_en_attr_a;
    if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_en_attr_a;
   }
   if (token.tk_id == KWD_class || token.tk_id == KWD_struct || token.tk_id == KWD_union) {
    if (!yield()) goto err_en_attr_a;
    if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_en_attr_a;
    if (token.tk_id == KWD_final) {
     if (!yield()) goto err_en_attr_a;
     if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_en_attr_a;
    }
    is_enum_class = 1;
   } else is_enum_class = 0;
   if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
    Dee_INCREF(enum_name = (DeeTokenObject *)token_ob);
    if (!yield()) {err_enum_name: Dee_XDECREF(enum_name); goto err_en_attr_a; }
    if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_en_attr_a;
   } else enum_name = NULL;
   if (DEE_PARSE_TOKENID_IS_BASE_INDICATOR(token.tk_id)) {
    if ((a->ob_const = (DeeObject *)_DeeLexer_ParseTypeBaseDecl(
     self,scope,flags|DEE_PARSER_FLAG_NO_BRACE_INIT)) == NULL) goto err_enum_name;
   } else {
    Dee_INCREF(a->ob_const = (DeeObject *)DeeObject_TYPE(Dee_rt_int));
   }
   DEE_ASSERT(DeeObject_Check(a->ob_const) && DeeType_Check(a->ob_const));
   if (is_enum_class) {
    DeeObject *enum_class_module;
    enum_class_module = DeeModule_New(
     enum_name ? enum_name->ob_token.tk_id : KWD___anonymous__,
     (DeeObject *)scope,a->ob_const);
    if (!enum_class_module) goto err_enum_name;
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,enum_class_module);
    a->ob_kind = DeeExprAstKind_MODULE;
   }
   if (enum_name) {
    DeeObject *old_enum_const;
    if ((old_enum_const = _DeeScope_GetName(scope,enum_name->ob_token.tk_id)) != NULL) {
     int temp;
     if ((temp = DeeObject_CompareEq(old_enum_const,a->ob_const)) == -1)
      DeeError_Handled(),temp = 0;
     if (temp == 0) {
      if (DeeError_CompilerErrorf(DEE_WARNING_ENUM_NAME_ALREADY_USED,(DeeObject *)self,(DeeObject *)enum_name,
       "Enum name is already used: %s:%r",DeeType_NAME(Dee_TYPE(old_enum_const)),old_enum_const) == -1) goto err_enum_name;
     }
    } else {
     if (_DeeScope_AddDeprecatedName(
      scope,enum_name->ob_token.tk_id,
      a->ob_const,(DeeObject *)attr.ob_depr) == -1) goto err_enum_name;
    }
    Dee_DECREF(enum_name);
   }
   if (is_enum_class) {
    DEE_ASSERT(DeeObject_Check(a->ob_mod_module) && DeeModule_Check(a->ob_mod_module));
    DEE_ASSERT(DeeObject_Check(a->ob_mod_module->ob_load_val) && DeeType_Check(a->ob_mod_module->ob_load_val));
    if (token.tk_id == '{') {
     if (_DeeLexer_ParseEnumDecl(self,a->ob_mod_module->ob_scope,flags,
      (DeeTypeObject *)a->ob_mod_module->ob_load_val) == -1) goto err_en_attr_a;
    }
   } else {
    DEE_ASSERT(DeeObject_Check(a->ob_const) && DeeType_Check(a->ob_const));
    if (token.tk_id == '{') {
     if (_DeeLexer_ParseEnumDecl(self,scope,flags,(DeeTypeObject *)a->ob_const) == -1) goto err_en_attr_a;
    }
   }
   DeeAttributeDecl_Quit(&attr);
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
  case KWD_final: {
   if (!yield()) return NULL;
final_again:
   switch (token.tk_id) {
    case KWD_local:
     if (!yield()) return NULL;
     goto final_again;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
    case KWD_class: goto parse_class;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
    case KWD_enum: goto parse_enum;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
    case KWD_struct:
    case KWD_union:
     goto parse_final_struct;
#endif
    default:
     if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_AFTER_FINAL,
      (DeeObject *)self,token_ob,"Unexpected token after 'final'") == -1) return NULL;
     goto again;
   }
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case KWD_class:parse_class:
   a = _DeeLexer_ParseClass(self,scope,flags);
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
  {int has_final_modifier;
parse_final_struct:
   has_final_modifier = 1;
   goto parse_struct_begin;
  case KWD_struct:
  case KWD_union:
   has_final_modifier = 0;
parse_struct_begin:
  {DeeStructTypeObject *struct_type;
   struct DeeAttributeDecl struct_attr = DeeAttributeDecl_INIT();
   if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) {
    DeeAttributeDecl_Quit(&struct_attr);
    return NULL;
   }
   if (!yield()) {err_struct_attr_a: DeeAttributeDecl_Quit(&struct_attr); goto err_a;}
   if (_DeeLexer_ParseAttributeDecl(&struct_attr,self,scope,flags) == -1) goto err_struct_attr_a;
   if (token.tk_id == KWD_final && !has_final_modifier) {
    if (!yield()) goto err_struct_attr_a;
    has_final_modifier = 1;
   }
   if (TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != KWD_extends) {
    // Named struct type
#if 1
    // TODO: Deprecated + lookup_mode
    struct_type = (DeeStructTypeObject *)DeeScope_Lookup(
     (DeeObject *)scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_CONST,NULL);
#else
    struct_type = (DeeStructTypeObject *)_DeeScope_GetName(scope,token.tk_id);
#endif
    if (struct_type != NULL) {
     if (!DeeStructType_Check(struct_type)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_NAME_ALREADY_USED,(DeeObject *)self,token_ob,
       "Struct name %s is already used",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id)) == -1) goto err_struct_attr_a;
      if ((struct_type = (DeeStructTypeObject *)DeeStructType_New()) == NULL) goto err_struct_attr_a;
     } else Dee_INCREF(struct_type);
    } else {
     if ((struct_type = (DeeStructTypeObject *)DeeStructType_New()) == NULL) goto err_struct_attr_a;
     if (_DeeScope_AddDeprecatedName(scope,token.tk_id,
      (DeeObject *)struct_type,(DeeObject *)struct_attr.ob_depr) == -1) goto err_struct_a;
    }
    if (has_final_modifier) DeeType_GET_SLOT(struct_type,tp_flags) |= DEE_TYPE_FLAG_NO_SUBCLASS;
    if (struct_attr.ob_name) {
     if (DeeType_SetName((DeeTypeObject *)struct_type,
      DeeString_STR(struct_attr.ob_name)) == -1) goto err_struct_a;
    } else {
     if (DeeType_SetName((DeeTypeObject *)struct_type,
      TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id)) == -1) goto err_struct_a;
    }
    if (!yield()) goto err_struct_a;
    if (_DeeLexer_ParseAttributeDecl(&struct_attr,self,scope,flags) == -1) goto err_struct_a;
   } else {
    if ((struct_type = (DeeStructTypeObject *)DeeStructType_New()) == NULL) goto err_struct_attr_a;
   }
   // Struct flags
   if((struct_attr.ob_flags&DEE_ATTRIBUTE_DECL_FLAGS_GUID)!=0) {
    if (!DeeGUID_EQUAL(&struct_type->tp_guid,&DeeGUID_Empty)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_GUID_ALREADY_SET,(DeeObject *)self,
      (DeeObject *)a->ob_token,"Struct guid was already set (old: %K; new: %K)",
      DeeGUID_ToString(&struct_type->tp_guid),
      DeeGUID_ToString(&struct_attr.ob_guid)) == -1) goto err_struct_a;
    }
    DeeGUID_SET(&struct_type->tp_guid,&struct_attr.ob_guid);
   }
   // Struct initializer
   if (token.tk_id == '{' || DEE_PARSE_TOKENID_IS_BASE_INDICATOR(token.tk_id)) {
    if (DeeStructType_Initialized((DeeTypeObject *)struct_type)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_STRUCT_ALREADY_INITIALIZED,(DeeObject *)self,
      (DeeObject *)a->ob_token,"Struct %s is already initialized",DeeType_NAME(struct_type)) == -1) {
err_struct_a: Dee_DECREF(struct_type); goto err_struct_attr_a;
     }
    }
    if (DEE_PARSE_TOKENID_IS_BASE_INDICATOR(token.tk_id)) {
     // Struct base declaration
     DeeTypeObject *base_type;
     if ((base_type = _DeeLexer_ParseTypeBaseDecl(
      self,scope,flags|DEE_PARSER_FLAG_NO_BRACE_INIT)) == NULL) goto err_struct_a;
     if (base_type == &DeeObject_Type) goto struct_use_defbase;
     if (!DeeStructuredType_Check(base_type)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRUCTURED_TYPE_FOR_STRUCT_BASE,(DeeObject *)self,
       (DeeObject *)a->ob_token,"Expected structured type for struct base (got: %s)",DeeType_NAME(base_type)) == -1) {
       Dee_DECREF(base_type);
       goto err_struct_a;
      }
struct_use_defbase:
      Dee_DECREF(base_type);
      Dee_INCREF(base_type = (DeeTypeObject *)&DeeStructured_Type);
     }
     // Set the new structure base type
     DeeStructType_SetBase((DeeTypeObject *)struct_type,base_type);
     Dee_DECREF(base_type);
     if (token.tk_id == '{') goto struct_block;
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_STRUCT_WITH_BASE,(DeeObject *)self,
      (DeeObject *)a->ob_token,"Expected '{' after 'struct' <keyword> ':' <expr>"
      ) == -1) goto err_struct_a;
    } else {
     int temp;
struct_block:
     DeeType_GET_SLOT(struct_type,tp_flags) |= (DEE_TYPE_FLAG_INCOMPLETE);
     temp = _DeeLexer_ParseStructDecl(
      struct_type,self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX),
      a->ob_token->ob_token.tk_id == KWD_union,&struct_attr);
     DeeType_GET_SLOT(struct_type,tp_flags) &= ~(DEE_TYPE_FLAG_INCOMPLETE);
     if (temp == -1) goto err_struct_a;
    }
    DeeStructType_Changed((DeeTypeObject *)struct_type);
   }
   DeeAttributeDecl_Quit(&struct_attr);
   Dee_INHERIT_REF(a->ob_const,*(DeeObject **)&struct_type);
  }} break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
  case KWD___builtin_typeof:
#endif
  case KWD_type: {
   int has_paren;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
   int is_builtin_typeof = token.tk_id == KWD___builtin_typeof;
#endif
   DeeTokenObject *op_token;
   DeeTypeObject *predicted_type;
   Dee_INCREF(op_token = (DeeTokenObject *)token_ob);
   if (!yield()) {err_type_tok: Dee_DECREF(op_token); return NULL; }
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
   if (is_builtin_typeof && token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_TYPE,
     (DeeObject *)self,(DeeObject *)op_token,"Expected '(' after '__builtin_typeof'"
     ) == -1) goto err_type_tok;
   }
#endif
   has_paren = token.tk_id == '(';
   if (has_paren && !yield()) goto err_type_tok;
   if ((r = has_paren
    ? _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))
    : _deeparse_unary(self,scope,flags)) == NULL) goto err_type_tok;
   if (has_paren) {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ',' && !yield()) goto err_type_tokr;
#endif
    if (token.tk_id != ')') {
     if ((
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
      is_builtin_typeof ? 
      DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_BUILTIN_TYPEOF,
      (DeeObject *)self,(DeeObject *)op_token,"Expected ')' after '__builtin_typeof' '(' <expr>") :
#endif
      DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_TYPE,
      (DeeObject *)self,(DeeObject *)op_token,"Expected ')' after 'type' '(' <expr>"
      )) == -1) {err_type_tokr: Dee_DECREF(r); goto err_type_tok; }
    } else if (!yield()) goto err_type_tokr;
   }
   if ((
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
    is_builtin_typeof || 
#endif
    DeeExprAst_IsNoeffect((DeeObject *)r,(DeeObject *)scope)) &&
    (predicted_type = DeeExprAst_PredictType((DeeObject *)r,(DeeObject *)scope)) != NULL) {
    // Only allow use of type prediction if __builtin_typeof
    // is used, or the argument has no side-effects.
    Dee_INHERIT_REF(a,r);
    Dee_INCREF(predicted_type);
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    a->ob_kind = DeeExprAstKind_CONSTANT;
    Dee_INHERIT_REF(a->ob_const,*(DeeObject **)&predicted_type);
    Dee_DECREF(op_token);
   } else {
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF
    if (is_builtin_typeof) {
     // Emit a warning and lie about the type
     if (DeeError_CompilerError(DEE_WARNING_UNPREDICTABLE_TYPE_IN_BUILTIN_TYPEOF,
      (DeeObject *)self,(DeeObject *)op_token,"Argument of '__builtin_typeof' has an unpredictable type"
      ) == -1) goto err_type_tokr;
     Dee_INHERIT_REF(a,r);
     Dee_XCLEAR(a->ob_op_a);
     Dee_XCLEAR(a->ob_op_b);
     Dee_XCLEAR(a->ob_op_c);
     Dee_XCLEAR(a->ob_op_d);
     Dee_DECREF(a->ob_token);
     Dee_INHERIT_REF(a->ob_token,op_token);
     a->ob_kind = DeeExprAstKind_CONSTANT;
     Dee_INCREF(a->ob_const = (DeeObject *)&DeeNone_Type);
    } else
#endif
    {
     // Generate a runtime type ast
     a = new_ast(DeeExprAstKind_TYPE,(DeeObject *)op_token);
     Dee_DECREF(op_token);
     if (!a) goto err_r;
     Dee_INHERIT_REF(a->ob_op_a,r);
    }
   }
  } break;

  case KWD_str: kind = DeeExprAstKind_STR; goto unary_paren_opt;
  case KWD_repr: kind = DeeExprAstKind_REPR; goto unary_paren_opt;
  case KWD_copy: kind = DeeExprAstKind_COPY; goto unary_paren_opt;
  case KWD_move: kind = DeeExprAstKind_MOVE; goto unary_paren_opt;
  case KWD_weak: kind = DeeExprAstKind_WEAK;
unary_paren_opt: {
   DeeTokenObject *op_token;
   int has_paren;
   Dee_INCREF(op_token = (DeeTokenObject *)token_ob);
   if (!yield()) {err_unary_paren_tok: Dee_DECREF(op_token); return NULL; }
   has_paren = token.tk_id == '(';
   if (has_paren && !yield()) goto err_unary_paren_tok;
   if ((r = has_paren
    ? _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))
    : _deeparse_unary(self,scope,flags)) == NULL) goto err_unary_paren_tok;
   if (has_paren) {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ',' && !yield()) goto err_unary_paren_tokr;
#endif
    if (token.tk_id != ')') {
     int error;
     switch (kind) {
      case DeeExprAstKind_STR:  error = DEE_WARNING_EXPECTED_RPAREN_AFTER_STR; break;
      case DeeExprAstKind_REPR: error = DEE_WARNING_EXPECTED_RPAREN_AFTER_REPR; break;
      case DeeExprAstKind_COPY: error = DEE_WARNING_EXPECTED_RPAREN_AFTER_COPY; break;
      case DeeExprAstKind_MOVE: error = DEE_WARNING_EXPECTED_RPAREN_AFTER_MOVE; break;
      case DeeExprAstKind_WEAK: error = DEE_WARNING_EXPECTED_RPAREN_AFTER_WEAK; break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     if (DeeError_CompilerErrorf(error,(DeeObject *)self,(DeeObject *)op_token,
      "Expected ')' after '%s' '(' <expr>",
      TPPLexer_TokenIDStr(DeeLexer_LEXER(self),op_token->ob_token.tk_id)
      ) == -1) {err_unary_paren_tokr: Dee_DECREF(r); goto err_unary_paren_tok; }
    } else if (!yield()) goto err_unary_paren_tokr;
   }
   if (r->ob_kind == DeeExprAstKind_CONSTANT && 
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
      (kind != DeeExprAstKind_WEAK) && DEE_HAVE_CONSTOP_OPTIMIZATION(r->ob_const)) {
    DeeObject *ob,*cresult;
    if (!_DeeType_IsSlotConstant(Dee_TYPE(r->ob_const),kind)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_MODIFYING_CONSTANT,(DeeObject *)self,(DeeObject *)op_token,
      "Possible modifications to the constant in '%s %#q' will be ignored",
      TPPLexer_TokenIDStr(DeeLexer_LEXER(self),op_token->ob_token.tk_id),
      DeeType_NAME(Dee_TYPE(r->ob_const))) == -1) goto err_unary_paren_tokr;
    }
    if ((ob = DeeObject_DeepCopy(r->ob_const)) == NULL) goto err_unary_paren_tokr;
    switch (kind) {
     case DeeExprAstKind_STR:  cresult = DeeObject_Str(ob); break;
     case DeeExprAstKind_REPR: cresult = DeeObject_Repr(ob); break;
     case DeeExprAstKind_COPY: cresult = DeeObject_Copy(ob); break;
     case DeeExprAstKind_MOVE: cresult = DeeObject_Move(ob); break;
   //case DeeExprAstKind_WEAK: cresult = DeeWeakref_New(ob); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    Dee_DECREF(ob);
    if (!cresult) {
     int error;
     switch (kind) {
      case DeeExprAstKind_STR:  error = DEE_WARNING_INVALID_CONSTOP_STR; break;
      case DeeExprAstKind_REPR: error = DEE_WARNING_INVALID_CONSTOP_REPR; break;
      case DeeExprAstKind_COPY: error = DEE_WARNING_INVALID_CONSTOP_COPY; break;
      case DeeExprAstKind_MOVE: error = DEE_WARNING_INVALID_CONSTOP_MOVE; break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     if (DeeError_CompilerErrorf(error,(DeeObject *)self,(DeeObject *)op_token,
      "Invalid constant operator: Calling '%s %#q' will throw an '%K' at runtime",
      TPPLexer_TokenIDStr(DeeLexer_LEXER(self),op_token->ob_token.tk_id),
      DeeType_NAME(Dee_TYPE(r->ob_const)),_DeeError_HandleReturnType()) == -1
      ) goto err_unary_paren_tokr;
     goto unary_paren_opt_def;
    }
    Dee_DECREF(op_token);
    Dee_DECREF(r->ob_const);
    Dee_INHERIT_REF(r->ob_const,cresult);
    Dee_INHERIT_REF(a,r);
   } else {
unary_paren_opt_def:
    a = new_ast(kind,(DeeObject *)op_token);
    Dee_DECREF(op_token);
    if (!a) goto err_r;
    Dee_INHERIT_REF(a->ob_op_a,r);
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case '&':
   kind = DeeExprAstKind_REF;
   goto unary;
  case '*':
   if (!yield()) return NULL;
   if ((r = _deeparse_unary(self,scope,flags)) == NULL) return NULL;
   if (_optimize_ast(r) == -1) goto err_r;
   // '*&x' --> 'x' >> cancle each other out
   if (r->ob_kind == DeeExprAstKind_REF) {
    // Revert back to the ast of the '&' operator
    Dee_INCREF(a = r->ob_op_a);
    Dee_DECREF(r);
#ifdef DEE_DEBUG
    r = NULL;
#endif
   } else {
    if ((a = new_ast(DeeExprAstKind_DEREF,token_ob)) == NULL) goto err_r;
    Dee_INHERIT_REF(a->ob_op_a,r);
   }
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case TPP_TOK_CMP_LE:
  case TPP_TOK_SHL:
  case TPP_TOK_SHL_ASSIGN:
  case '<':
   if ((a = new_ast(DeeExprAstKind_CELL,token_ob)) == NULL) return NULL;
   switch (token.tk_id) {
    case TPP_TOK_CMP_LE:     token.tk_id = '='; break;
    case TPP_TOK_SHL:        token.tk_id = '<'; break;
    case TPP_TOK_SHL_ASSIGN: token.tk_id = TPP_TOK_CMP_LE; break;
    default: if (!yield()) goto err_a; break;
   }
   if (token.tk_id != '>' && token.tk_id != TPP_TOK_SHR &&
       token.tk_id != TPP_TOK_SHR_ASSIGN && token.tk_id != TPP_TOK_CMP_GE) {
    if ((a->ob_op_a = _deeparse_unary(self,scope,flags)) == NULL) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ',' && !yield()) goto err_a;
#endif
   }
   if (token.tk_id == TPP_TOK_SHR) token.tk_id = '>';
   else if (token.tk_id == TPP_TOK_SHR_ASSIGN) token.tk_id = TPP_TOK_CMP_GE;
   else if (token.tk_id == TPP_TOK_CMP_GE) token.tk_id = '=';
   else if (token.tk_id != '>') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_GR_AFTER_LO_FOR_CELL,(DeeObject *)self,token_ob,
     "Expected '>' after '<' [<expr>] to end cell expression") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
      (!a->ob_op_a || a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT)) {
    DeeObject *new_cell;
    if ((new_cell = a->ob_op_a ? DeeCell_New(a->ob_op_a->ob_const) : DeeCell_NewEmpty()) == NULL) goto err_a;
    Dee_XDECREF(a->ob_op_a);
    Dee_INHERIT_REF(a->ob_const,new_cell);
    a->ob_kind = DeeExprAstKind_CONSTANT;
   }
   break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case KWD_new: kind = DeeExprAstKind_NEW; goto unary;
  case KWD_delete: kind = DeeExprAstKind_DELETE; goto unary;
#endif
  case TPP_TOK_INC: kind = DeeExprAstKind_INC; goto unary;
  case TPP_TOK_DEC: kind = DeeExprAstKind_DEC; goto unary;
  case '!':
   if (!yield()) return NULL;
   if (token.tk_id == '!') {
    kind = DeeExprAstKind_BOOL;
    goto unary;
   } else {
    kind = DeeExprAstKind_NOT;
    goto unary_noyield;
   }
  case '#': kind = DeeExprAstKind_SEQ_SIZE; goto unary;
  case '-': kind = DeeExprAstKind_NEG; goto unary;
  case '+': kind = DeeExprAstKind_POS; goto unary;
  case '~': kind = DeeExprAstKind_INV; unary:
   if (!yield()) return NULL;
unary_noyield:
   if ((r = _deeparse_unary(self,scope,flags)) == NULL) return NULL;
   if (_optimize_ast(r) == -1) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
   if (kind == DeeExprAstKind_REF &&
       r->ob_kind == DeeExprAstKind_ATTR_GET) {
    // Special handling to allow for offsetof style constant expressions
    Dee_size_t total_offset;
    DeeTypeObject *last_type;
    if (_DeeExprAst_CalculateOffsetof(r,&total_offset,&last_type)) {
     DEE_LVERBOSE2("Special handling for constant offsetof: %r\n",r);
     DEE_ASSERT(last_type);
     Dee_CLEAR(r->ob_op_a);
     Dee_CLEAR(r->ob_op_b);
     if ((last_type = DeeType_Pointer(last_type)) == NULL) goto err_r;
     r->ob_kind = DeeExprAstKind_CONSTANT;
     if ((r->ob_const = DeePointer_New(last_type,(void *)total_offset)) == NULL) goto err_r;
     Dee_INHERIT_REF(a,r);
     break;
    }
   }
#endif
   if (r->ob_kind == DeeExprAstKind_CONSTANT && 
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
      DEE_HAVE_CONSTOP_OPTIMIZATION(r->ob_const)) {
    DeeObject *unary_res,*lhs; char const *op_symb;
    switch (kind) {
     case DeeExprAstKind_INC:      op_symb = "++"; break;
     case DeeExprAstKind_DEC:      op_symb = "--"; break;
     case DeeExprAstKind_SEQ_SIZE: op_symb = "#";  break;
     case DeeExprAstKind_NOT:      op_symb = "!";  break;
     case DeeExprAstKind_BOOL:     op_symb = "!!"; break;
     case DeeExprAstKind_NEG:      op_symb = "-";  break;
     case DeeExprAstKind_POS:      op_symb = "+";  break;
     case DeeExprAstKind_INV:      op_symb = "~";  break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    if (!_DeeType_IsSlotConstant(Dee_TYPE(r->ob_const),kind)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_MODIFYING_CONSTANT,(DeeObject *)self,(DeeObject *)r->ob_token,
      "Possible modifications to the constant in '%s %#q' will be ignored",
      op_symb,DeeType_NAME(Dee_TYPE(r->ob_const))) == -1) goto err_r;
    }
    if ((lhs = DeeObject_DeepCopy(r->ob_const)) == NULL) goto err_r;
    switch (kind) {
     case DeeExprAstKind_INC:      unary_res = DeeObject_Inc(lhs); break;
     case DeeExprAstKind_DEC:      unary_res = DeeObject_Dec(lhs); break;
     case DeeExprAstKind_SEQ_SIZE: unary_res = DeeObject_SizeObject(lhs); break;
     case DeeExprAstKind_BOOL: {
      int temp = DeeObject_Bool(lhs);
      unary_res = temp == -1 ? NULL : DeeBool_New(temp);
     } break;
     case DeeExprAstKind_NOT:      unary_res = DeeObject_Not(lhs); break;
     case DeeExprAstKind_NEG:      unary_res = DeeObject_Neg(lhs); break;
     case DeeExprAstKind_POS:      unary_res = DeeObject_Pos(lhs); break;
     case DeeExprAstKind_INV:      unary_res = DeeObject_Inv(lhs); break;
   //case DeeExprAstKind_REF:      unary_res = DeeObject_Ref(lhs); break;
   //case DeeExprAstKind_DEREF:    unary_res = DeeObject_Deref(lhs); break;
     default: Dee_DECREF(lhs); goto def_unary;
    }
    Dee_DECREF(lhs);
    if (!unary_res) {
     int error_code;
     switch (kind) {
      case DeeExprAstKind_INC:      error_code = DEE_WARNING_INVALID_CONSTOP_INC; break;
      case DeeExprAstKind_DEC:      error_code = DEE_WARNING_INVALID_CONSTOP_DEC; break;
      case DeeExprAstKind_SEQ_SIZE: error_code = DEE_WARNING_INVALID_CONSTOP_SEQ_SIZE; break;
      case DeeExprAstKind_NOT:      error_code = DEE_WARNING_INVALID_CONSTOP_NOT; break;
      case DeeExprAstKind_BOOL:     error_code = DEE_WARNING_INVALID_CONSTOP_BOOL; break;
      case DeeExprAstKind_NEG:      error_code = DEE_WARNING_INVALID_CONSTOP_NEG; break;
      case DeeExprAstKind_POS:      error_code = DEE_WARNING_INVALID_CONSTOP_POS; break;
      case DeeExprAstKind_INV:      error_code = DEE_WARNING_INVALID_CONSTOP_INV; break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     if (DeeError_CompilerErrorf(error_code,(DeeObject *)self,(DeeObject *)r->ob_token,
      "Invalid constant operator: Calling '%s(%#q)' will throw an '%K' at runtime",
      op_symb,DeeType_NAME(Dee_TYPE(r->ob_const)),_DeeError_HandleReturnType()
      ) == -1) goto err_r;
     goto def_unary;
    }
    Dee_DECREF(r->ob_const);
    Dee_INHERIT_REF(r->ob_const,unary_res);
    Dee_INHERIT_REF(a,r);
   } else {
def_unary:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
    if (kind == DeeExprAstKind_REF ||
        kind == DeeExprAstKind_DEREF) {
     // Warn when used with non-structured type
     DeeTypeObject *predicted_ty = DeeExprAst_PredictType((DeeObject *)r,(DeeObject *)scope);
     if (predicted_ty && !DeeStructuredType_Check(predicted_ty) && !DeeLValueType_Check(predicted_ty)) {
      if (DeeError_CompilerErrorf(kind == DeeExprAstKind_REF
       ? DEE_WARNING_INVALID_REF_ON_NONSTRUCTURED
       : DEE_WARNING_INVALID_DEREF_ON_NONSTRUCTURED,(DeeObject *)self,(DeeObject *)r->ob_token,
       "Invalid constant operator: %s non-structured type '%#q' will throw an 'Error.TypeError' at runtime",
       kind == DeeExprAstKind_REF ? "Referencing" : "Dereferencing",
       DeeType_NAME(predicted_ty)) == -1) goto err_r;
     }
    }
#endif
    // NOTE: The token we are using here isn't correct
    if ((a = new_ast(kind,(DeeObject *)r->ob_token)) == NULL) goto err_r;
    Dee_INHERIT_REF(a->ob_op_a,r);
   }
   break;

  case KWD_super: {
   DeeScopeObject *weak_root;
   weak_root = (DeeScopeObject *)DeeScope_WeakRoot((DeeObject *)scope);
   if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_THIS)==0) {
    if (DeeError_CompilerError(DEE_WARNING_SUPER_USED_AS_VARIABLE_NAME,
     (DeeObject *)self,token_ob,"'super' used as name for a variable"
     ) == -1) return NULL;
    goto vr_begin;
   }
   if (!weak_root->ob_supertp) {
    if (DeeError_CompilerError(DEE_WARNING_USE_OF_SUPER_UNSAFE_WITHOUT_ATTRIBUTE_SUPER,
     (DeeObject *)self,token_ob,"Use of 'super' may be unsafe without a [[super(...)]] attribute"
     ) == -1) return NULL;
    if ((r = new_ast(DeeExprAstKind_VARIABLE,token_ob)) == NULL) return NULL;
    r->ob_var = (DeeLocalVarObject *)_DeeScope_GetName(weak_root,KWD_this);
    ++r->ob_var->ob_uses;
    DEE_ASSERT(DeeObject_Check(r->ob_var) && DeeLocalVar_Check(r->ob_var));
    Dee_INCREF(r->ob_var);
    if ((a = new_ast(DeeExprAstKind_SUPER,token_ob)) == NULL) goto err_r;
    Dee_INHERIT_REF(a->ob_op_a,r);
   } else {
    if ((a = new_ast(DeeExprAstKind___SUPER,token_ob)) == NULL) return NULL;
    if ((a->ob_op_a = (DeeExprAstObject *)DeeExprAst_CopyWithScope(
     (DeeObject *)weak_root->ob_supertp,(DeeObject *)scope)) == NULL) goto err_a;
    if ((a->ob_op_b = new_ast(DeeExprAstKind_VARIABLE,token_ob)) == NULL) goto err_a;
    a->ob_op_b->ob_var = (DeeLocalVarObject *)_DeeScope_GetName(weak_root,KWD_this);
    ++a->ob_op_b->ob_var->ob_uses;
    DEE_ASSERT(DeeObject_Check(a->ob_op_b->ob_var) && DeeLocalVar_Check(a->ob_op_b->ob_var));
    Dee_INCREF(a->ob_op_b->ob_var);
   }
   if (!yield()) goto err_a;
  } break;

  case KWD___super:
  case KWD___builtin_super: {
   DeeScopeObject *weak_root;
   if ((a = new_ast(DeeExprAstKind___SUPER,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER___SUPER,
     (DeeObject *)self,token_ob,"Expected '(' after '__super'") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if ((a->ob_op_a = _deeparse_top(self,scope,flags)) == NULL) goto err_a;
   if (token.tk_id == ',') {
    if (!yield()) goto err_a;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')') goto super_no_arg2;
#endif
    if ((a->ob_op_b = _deeparse_top(self,scope,flags)) == NULL) goto err_a;
   } else {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
super_no_arg2:
#endif
    weak_root = (DeeScopeObject *)DeeScope_WeakRoot((DeeObject *)scope);
    if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_THIS)==0) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SECOND___SUPER_ARG_IN_NON_THISCALL,
      (DeeObject *)self,token_ob,"Expected ',' after '__super' '(' <expr> "
      "(single argument __super is only allowed in [[thiscall]])") == -1) goto err_a;
     Dee_DECREF(a->ob_op_a);
     Dee_INCREF(a->ob_const = Dee_None);
     a->ob_kind = DeeExprAstKind_CONSTANT;
    } else {
     if ((r = new_ast(DeeExprAstKind_VARIABLE,(DeeObject *)a->ob_token)) == NULL) goto err_a;
     r->ob_var = (DeeLocalVarObject *)_DeeScope_GetName(weak_root,KWD_this);
     ++r->ob_var->ob_uses;
     DEE_ASSERT(DeeObject_Check(r->ob_var) && DeeLocalVar_Check(r->ob_var));
     Dee_INCREF(r->ob_var);
     Dee_INHERIT_REF(a->ob_op_b,r);
    }
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER___SUPER,
     (DeeObject *)self,token_ob,"Expected ')' after '__super' '(' <expr> [',' <expr>]") == -1) goto err_a;
   } else if (!yield()) goto err_a;
   if (a->ob_kind == DeeExprAstKind___SUPER) {
    DeeTypeObject *opa_type = DeeExprAst_PredictType((DeeObject *)a->ob_op_a,(DeeObject *)scope);
    if (opa_type && !DeeType_Check(opa_type)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_IN___SUPER,
      (DeeObject *)self,token_ob,"Expected 'type' for first argument in '__super' (got: '%#q')",
      DeeType_NAME(opa_type)) == -1) goto err_a;
    }
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
        a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
        a->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT &&
        DeeType_Check(a->ob_op_a->ob_const)) {
     DeeObject *super_object;
     if ((super_object = DeeSuper_New((
      DeeTypeObject *)a->ob_op_a->ob_const,a->ob_op_b->ob_const)
      ) == NULL) goto err_a;
     Dee_CLEAR(a->ob_op_b);
     Dee_DECREF(a->ob_op_a);
     Dee_INHERIT_REF(a->ob_const,super_object);
     a->ob_kind = DeeExprAstKind_CONSTANT;
    }
   }
  } break;


#if !DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT
  case KWD_noexcept:
#endif
  case KWD_alignof:
  case KWD__Alignof:
  case KWD_array:
  case KWD_auto:
  case KWD_const_cast:
  case KWD_decltype:
  case KWD_dynamic_cast:
  case KWD_inherited:
  case KWD_interface:
  case KWD_mutable:
  case KWD_nil:
  case KWD_packed:
  case KWD_procedure:
  case KWD_record:
  case KWD_ref:
  case KWD_reinterpret_cast:
  case KWD_sizeof:
  case KWD_specialize:
  case KWD_static_cast:
  //case KWD_this:
  case KWD_thread_local:
  case KWD_typeid:
  case KWD_typename:
   if (DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,(DeeObject *)self,token_ob,
    "Reserved keyword used in a context that might change its meaning.") == -1) return NULL;
   // fallthrough

  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    DeeScopeObject *scope_iter;
    DeeObject *found_object,*is_deprecated;
    Dee_ssize_t nth_variable;
    Dee_ssize_t scope_offset;
    goto vr_begin;
  case KWD___nth:
    if (!yield()) return NULL;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected '(' after '__nth'") == -1) return NULL;
    } else if (!yield()) return NULL;
    if ((a = _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected ')' after '__nth' '(' <expr>") == -1) goto err_a;
    } else if (!yield()) goto err_a;
    if (a->ob_kind != DeeExprAstKind_CONSTANT) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected constant expression for <expr> in '__nth' '(' <expr> ')'") == -1) goto err_a;
     nth_variable = 1;
    } else if (DeeObject_Cast(Dee_ssize_t,a->ob_const,&nth_variable) != 0) {
     DeeError_Handled();
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected integral constant for <expr> in '__nth' '(' <expr> ')'") == -1) goto err_a;
    }
    if (nth_variable < 0) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_POSITIVE_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected positive constant for <expr> in '__nth' '(' <expr> ')'") == -1) goto err_a;
     nth_variable = -nth_variable;
    }
    if (!nth_variable) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NONZERO_EXPRESSION_AFTER_NTH,(DeeObject *)self,token_ob,
      "Expected non-zero constant for <expr> in '__nth' '(' <expr> ')' "
      "(How am I supposed to use the zeroth occurrence of a variable?)") == -1) goto err_a;
    } else --nth_variable;
    // Go to the requested scope
    scope_iter = scope;
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    while (1) {
     found_object = _DeeScope_GetNameEx(scope_iter,token.tk_id,&is_deprecated);
     if (found_object) { if (!nth_variable) goto vr_use_found_object; --nth_variable; }
     if (scope_iter->ob_weak_prev) scope_iter = scope_iter->ob_weak_prev;
     else if (scope_iter->ob_prev) scope_iter = scope_iter->ob_prev;
     else break;
    }
    if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_NTH_VARIABLE,(
     DeeObject *)self,token_ob,"Unknown __nth variable") == -1) return NULL;
    goto vr_begin_inherit_a;

  case KWD___scope:
    if (!yield()) return NULL;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_SCOPE,(DeeObject *)self,token_ob,
      "Expected '(' after '__scope'") == -1) return NULL;
    } else if (!yield()) return NULL;
    if ((a = _deeparse_top(self,scope,flags&~(DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) return NULL;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_SCOPE,(DeeObject *)self,token_ob,
      "Expected ')' after '__scope' '(' <expr>") == -1) goto err_a;
    } else if (!yield()) goto err_a;
    if (a->ob_kind != DeeExprAstKind_CONSTANT) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
      "Expected constant expression for <expr> in '__scope' '(' <expr> ')'") == -1) goto err_a;
     scope_offset = 0;
    } else if (DeeObject_Cast(Dee_ssize_t,a->ob_const,&scope_offset) != 0) {
     DeeError_Handled();
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_INTEGRAL_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
      "Expected integral constant for <expr> in '__scope' '(' <expr> ')'") == -1) goto err_a;
    }
    if (scope_offset > 0) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NEGATIVE_EXPRESSION_AFTER_SCOPE,(DeeObject *)self,token_ob,
      "Expected negative constant for <expr> in '__scope' '(' <expr> ')'") == -1) goto err_a;
     scope_offset = -scope_offset;
    }
    // Go to the requested scope
    scope_iter = scope;
    Dee_XCLEAR(a->ob_op_a);
    Dee_XCLEAR(a->ob_op_b);
    Dee_XCLEAR(a->ob_op_c);
    Dee_XCLEAR(a->ob_op_d);
    while (scope_offset++) {
     if (scope_iter->ob_weak_prev) scope_iter = scope_iter->ob_weak_prev; else
     if (scope_iter->ob_prev) scope_iter = scope_iter->ob_prev; else {
      if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_SCOPE_VARIABLE,(
       DeeObject *)self,token_ob,"Unknown __scope variable") == -1) return NULL;
      goto vr_begin_inherit_a;
     }
    }
    goto vr_search_iter_inherit_a;

  case KWD_local:
   if ((a = new_ast(DeeExprAstKind_VARIABLE,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_a;
   if ((found_object = _DeeScope_GetNameEx(
    scope,token.tk_id,&is_deprecated)) != NULL) goto vr_use_found_object;
   if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_LOCAL_VARIABLE,(
    DeeObject *)self,token_ob,"Unknown local variable") == -1) return NULL;
   goto vr_create_local;

  case TPP_TOK_COLLON_COLLON: // C++ style
  case KWD_global:
   if (!yield()) return NULL;
   if ((a = new_ast(DeeExprAstKind_VARIABLE,token_ob)) == NULL) return NULL;
   scope_iter = (DeeScopeObject *)DeeScope_Global((DeeObject *)scope);
   if ((found_object = _DeeScope_GetNameEx(
    scope_iter,token.tk_id,&is_deprecated)) != NULL) goto vr_use_found_object;
   Dee_DECREF(a);
   if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_GLOBAL_VARIABLE,(
    DeeObject *)self,token_ob,"Unknown global variable") == -1) return NULL;
   goto vr_begin;

  case TPP_TOK_DOTS:
vr_begin: // var_ref
    if ((a = new_ast(DeeExprAstKind_VARIABLE,token_ob)) == NULL) return NULL;
vr_begin_inherit_a:
    scope_iter = scope;
vr_search_iter_inherit_a:
    while (1) {
     if ((found_object = _DeeScope_GetNameEx(scope_iter,token.tk_id,&is_deprecated)) != NULL) {
vr_use_found_object:
      if (is_deprecated && DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,(
       DeeObject *)self,token_ob,"Deprecated : %r",is_deprecated) == -1) goto err_a;
      if (DeeModule_Check(found_object)) a->ob_kind = DeeExprAstKind_MODULE;
      else if (!DeeLocalVar_Check(found_object)) a->ob_kind = DeeExprAstKind_CONSTANT;
      else { // Add a use for this variable
       if (!DeeScope_SameWeakScope(
        (DeeObject *)((DeeLocalVarObject *)found_object)->ob_scope,(DeeObject *)scope) &&
        DeeScope_ExternalReference((DeeObject *)scope,found_object) == -1) goto err_a;
       ++((DeeLocalVarObject *)found_object)->ob_uses;
      }
      // This optimization assumes that 'ob_const', 'ob_mod_module' and 'ob_var' share the same address
      Dee_INCREF(a->ob_const = found_object);
      if (!yield()) goto err_a;
      goto parse_suffix;
     }
     if (scope_iter->ob_weak_prev) scope_iter = scope_iter->ob_weak_prev;
     else if (scope_iter->ob_prev) scope_iter = scope_iter->ob_prev;
     else break;
    }
/*vr_not_found:*/
    if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_VARIABLE,(
     DeeObject *)self,token_ob,"Unknown variable") == -1) goto err_a;
vr_create_local:
    if ((found_object = DeeLocalVar_New(token.tk_id,(DeeObject *)scope)) == NULL) goto err_a;
    Dee_INHERIT_REF(a->ob_var,*(DeeLocalVarObject **)&found_object);
    ++a->ob_var->ob_uses;
    if (_DeeScope_AddName(scope,token.tk_id,(DeeObject *)a->ob_var) == -1) goto err_a;
    if (!yield()) goto err_a;
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_TOKEN_IN_EXPRESSION,(
     DeeObject *)self,token_ob,"Unexpected token in <expr>") == -1) return NULL;
    if ((a = new_ast(DeeExprAstKind_CONSTANT,token_ob)) == NULL) return NULL;
    Dee_INCREF(a->ob_const = Dee_None);
   }
   break;
 }
parse_suffix:
// Suffix
 while (1) switch (token.tk_id) {

  // Ignored
  case KWD_const:
  case KWD___const:
  case KWD___const__:
  case KWD_volatile:
  case KWD___volatile:
  case KWD___volatile__:
   switch (a->ob_kind) {
    case DeeExprAstKind_CONSTANT:
     if (DeeType_Check(a->ob_const) || DeeNone_Check(a->ob_const)) break;
     // fallthrough
    default:
     if (!DeeExprAst_NeedSemicollon(DeeExprAst_End((DeeObject *)a))) goto end;
     if (DeeError_CompilerErrorf(DEE_WARNING_CONST_OR_VOLATILE_SUFFIX_ON_NON_TYPE,
      (DeeObject *)self,token_ob,"'const' or 'volatile' following a non-type ast %r",
      a) == -1) goto err_a;
     break;
   }
   if (!yield()) goto err_a;
   break;

  case '(': // tp_call
   //if (_optimize_ast(a) == -1) goto err_a;
   if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0 ||
       a->ob_kind != DeeExprAstKind_CONSTANT ||
       (!DeeNone_Check(a->ob_const) && !DeeType_Check(a->ob_const))) {
    if ((r = _DeeLexer_ParseFunCall(a,self,scope,flags&~(
     DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
    Dee_DECREF(a);
    Dee_INHERIT_REF(a,r);
   } else goto end;
   break;

  case '{': // Range initializer
   if ((flags&DEE_PARSER_FLAG_NO_BRACE_INIT)==0) {
    if (a->ob_kind == DeeExprAstKind_CONSTANT) {
type_init:
     if (a->ob_const == (DeeObject *)&DeeDict_Type ||
        (DeeDict_Check(a->ob_const) && DeeDict_SIZE(a->ob_const) == 0)) {
      DeeTupleObject *init;
      if ((init = _DeeLexer_ParseDictInit(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
      Dee_DECREF(a->ob_const);
      Dee_INHERIT_REF(a->ob_dict,init);
      a->ob_kind = DeeExprAstKind_DICT;
      if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0 &&
          _DeeExprAst_OptimizeDict(a) == -1) goto err_a;
     } else if (a->ob_const == (DeeObject *)&DeeList_Type ||
               (DeeList_Check(a->ob_const) && DeeList_SIZE(a->ob_const) == 0)) {
      DeeTupleObject *init;
      if ((init = _DeeLexer_ParseListInit(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
      Dee_DECREF(a->ob_const);
      Dee_INHERIT_REF(a->ob_args_a,init);
      a->ob_kind = DeeExprAstKind_LIST;
      if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0 &&
          _DeeExprAst_OptimizeList(a) == -1) goto err_a;
     } else if (a->ob_const == (DeeObject *)&DeeTuple_Type ||
               (DeeTuple_Check(a->ob_const) && DeeTuple_SIZE(a->ob_const) == 0)) {
      DeeTupleObject *init;
      if ((init = _DeeLexer_ParseListInit(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
      Dee_DECREF(a->ob_const);
      Dee_INHERIT_REF(a->ob_args_a,init);
      a->ob_kind = DeeExprAstKind_TUPLE;
      if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0 &&
          _DeeExprAst_OptimizeTuple(a) == -1) goto err_a;
     } else if (a->ob_const == (DeeObject *)&DeeSet_Type ||
               (DeeSet_Check(a->ob_const) && DeeSet_SIZE(a->ob_const) == 0)) {
      DeeTupleObject *init;
      if ((init = _DeeLexer_ParseSetInit(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_a;
      Dee_DECREF(a->ob_const);
      Dee_INHERIT_REF(a->ob_args_a,init);
      a->ob_kind = DeeExprAstKind_SET;
      if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0 &&
          _DeeExprAst_OptimizeSet(a) == -1) goto err_a;
#if 0
     } else if (DeeArrayType_Check(a->ob_const)) {
      if ((r = _DeeLexer_ParseArrayInitializer(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX),(DeeArrayTypeObject *)a->ob_const)) == NULL) goto err_a;
      Dee_DECREF(a);
      Dee_INHERIT_REF(a,r);
#endif
     } else {
      // TODO: Struct initializer
      // TODO: Array initializer
#if 0
      if (DeeError_CompilerErrorf(DEE_WARNING_UNSUPPORTED_TYPE_FOR_RANGE_INIT,(
       DeeObject *)self,token_ob,"Unsupported type <%s%s> for range initializer",
       DeeType_Check(a->ob_const)?"":"instance of ",DeeType_Check(a->ob_const)?
       DeeType_NAME(a->ob_const):DeeType_NAME(Dee_TYPE(a->ob_const))) == -1) goto err_a;
#endif
      goto end;
     }
    } else {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_FOR_RANGE_INIT,(DeeObject *)self,
      token_ob,"Expected a constant expression for range initializer (got: %r)",a) == -1) goto err_a;
     goto end;
    }
   } else goto end;
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case '*': // create pointer type
  case TPP_TOK_POW:
  case TPP_TOK_MUL_ASSIGN:
  case TPP_TOK_POW_ASSIGN: {
   if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0) {
    if (a->ob_kind == DeeExprAstKind_CONSTANT &&
        (DeeNone_Check(a->ob_const) || DeeType_Check(a->ob_const))) {
     DeeTypeObject *ptr_type = DeeType_Pointer(DeeNone_Check(a->ob_const)
      ? (DeeTypeObject *)&DeeNone_Type : (DeeTypeObject *)a->ob_const);
     if (!ptr_type) {
      // Handle the type error and instead display a compiler warning
      if (DeeError_Handle(&DeeErrorType_TypeError)) {
       if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_POINTER_OF_THIS_TYPE,
        (DeeObject *)self,token_ob,"Can't create the pointer type of %q",
        DeeType_NAME(a->ob_const)) == -1) goto err_a;
      } else goto err_a;
     } else {
      Dee_DECREF(a->ob_const);
      Dee_INCREF(a->ob_const = (DeeObject *)ptr_type);
     }
     if (token.tk_id == TPP_TOK_POW) {
      token.tk_id = '*';
      ++token.tk_str_begin;
     } else if (token.tk_id == TPP_TOK_POW_ASSIGN) {
      token.tk_id = TPP_TOK_MUL_ASSIGN;
      ++token.tk_str_begin;
     } else if (token.tk_id == TPP_TOK_MUL_ASSIGN) {
      token.tk_id = '=';
      ++token.tk_str_begin;
     } else if (!yield()) goto err_a;
    } else goto end;
   } else goto end;
  } break;

  case '&': // create lvalue type
  case TPP_TOK_LAND:
  case TPP_TOK_BIN_AND_ASSIGN: {
   if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0) {
    if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       (DeeNone_Check(a->ob_const) || DeeType_Check(a->ob_const))) {
     DeeTypeObject *lv_type = DeeType_LValue(DeeNone_Check(a->ob_const)
      ? (DeeTypeObject *)&DeeNone_Type : (DeeTypeObject *)a->ob_const);
     if (!lv_type) {
      if (DeeError_Handle(&DeeErrorType_TypeError)) {
       if (DeeError_CompilerErrorf(DEE_WARNING_CANT_CREATE_LVALUE_OF_THIS_TYPE,
        (DeeObject *)self,token_ob,"Can't create the lvalue type of %q",
        DeeType_NAME(a->ob_const)) == -1) goto err_a;
      } else goto err_a;
     } else {
      Dee_DECREF(a->ob_const);
      Dee_INCREF(a->ob_const = (DeeObject *)lv_type);
     }
     if (token.tk_id == TPP_TOK_LAND) {
      token.tk_id = '&';
      ++token.tk_str_begin;
     } else if (token.tk_id == TPP_TOK_BIN_AND_ASSIGN) {
      token.tk_id = TPP_TOK_ASSIGN;
      ++token.tk_str_begin;
     } else if (!yield()) goto err_a;
    } else goto end;
   } else goto end;
  } break;
#endif


  case TPP_TOK_COLLON_COLLON:
   // NOTE: We always optimize 'a' in this case!
   if (_do_optimize_ast(a) == -1) goto err_a;
   if (a->ob_kind != DeeExprAstKind_CONSTANT &&
       a->ob_kind != DeeExprAstKind_MODULE) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_BEFORE_COLLON_COLLON,
     (DeeObject *)self,token_ob,"Expected constant expression before '::'") == -1) goto err_a;
   }
   goto do_subscribt;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case TPP_TOK_ARROW: // tp_attr_get (after deref)
   if (a->ob_kind == DeeExprAstKind_REF) {
    Dee_INHERIT_REF(r,a->ob_op_a);
    Dee_DECREF(a);
   } else {
    if ((r = new_ast(DeeExprAstKind_DEREF,token_ob)) == NULL) goto err_r;
    Dee_INHERIT_REF(r->ob_op_a,a);
   }
   Dee_INHERIT_REF(a,r);
   // fallthrough
#endif

  case '.': {
   int require_constant;
   // tp_attr_get
   // NOTE: Constant optimization for this can only happen
   //       after the implicit and optional conversion to tp_attr_set
   // But optimize the lhs ast...
   if (_optimize_ast(a) == -1) goto err_a;
do_subscribt:
   require_constant = token.tk_id == TPP_TOK_COLLON_COLLON;
   if (a->ob_kind == DeeExprAstKind_MODULE) {
    if (!yield()) goto err_a;
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT,(DeeObject *)self,token_ob,
      "Expected <keyword> after '<expr>.'") == -1) goto err_a;
    } else {
     DeeObject *is_deprecated = NULL;
     c = DeeScope_Lookup((DeeObject *)a->ob_mod_module->ob_scope,token.tk_id,
                         DEE_SCOPE_LOOKUP_MODE_LOCAL|DEE_SCOPE_LOOKUP_MODE_CONST,&is_deprecated);
     if (is_deprecated) {
      if (DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,(DeeObject *)self,token_ob,
       "Deprecated : %r",is_deprecated) == -1) return NULL;
     }
     if (!c) {
      if (DeeError_CompilerError(DEE_WARNING_VARIABLE_COULD_NOT_FIND_VAR_OR_CONST_IN_MODULE,
       (DeeObject *)self,token_ob,"Could not find module var / const in module") == -1) goto err_a;
      if (!yield()) goto err_a;
     } else {
      if (!yield()) goto err_a;
      Dee_INCREF(c);
      Dee_DECREF(a->ob_mod_module);
      if (DeeLocalVar_Check(c)) {
       a->ob_kind = DeeExprAstKind_VARIABLE;
       a->ob_var = (DeeLocalVarObject *)c;
       ++a->ob_var->ob_uses; // Using this variable
       if (!DeeScope_SameWeakScope(
        (DeeObject *)((DeeLocalVarObject *)c)->ob_scope,
        (DeeObject *)scope)) {
        // External scope reference (must setup reference)
        DEE_LVERBOSE2("Defining reference to scope-extenal variable %s (%d)\n",
                      TPPLexer_TokenIDStr(DeeLexer_LEXER(self),
                      ((DeeLocalVarObject *)c)->ob_name),
                      ((DeeLocalVarObject *)c)->ob_name);
        if (DeeScope_ExternalReference((DeeObject *)scope,c) == -1) return NULL;
       }
      } else if (DeeModule_Check(c)) {
       a->ob_kind = DeeExprAstKind_MODULE;
       a->ob_mod_module = (DeeModuleObject *)c;
      } else {
       a->ob_kind = DeeExprAstKind_CONSTANT;
       a->ob_const = c;
      }
#ifdef DEE_DEBUG
      c = NULL;
#endif
     }
    }
   } else {
    if ((r = new_ast(DeeExprAstKind_ATTR_GET,token_ob)) == NULL) goto err_a;
    Dee_INHERIT_REF(r->ob_op_a,a); // Use the old ast as <expr> func
    if (!yield()) goto err_r;
    if (token.tk_id == KWD_super) {
     if (!yield()) goto err_r;
     r->ob_kind = DeeExprAstKind_SUPER;
     Dee_INHERIT_REF(a,r);
     if (a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
        (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
         DEE_HAVE_CONSTOP_OPTIMIZATION_SUPER(a->ob_op_a->ob_const)) {
      DeeObject *temp;
      if ((temp = DeeSuper_Of(a->ob_op_a->ob_const)) == NULL) {
       if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_SUPER,(DeeObject *)self,(DeeObject *)a->ob_token,
        "Invalid constant operator: Calling '%s.super' will throw an '%K' at runtime",
        DeeType_NAME(Dee_TYPE(a->ob_op_a->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_a;
      } else {
       Dee_DECREF(a->ob_op_a);
       a->ob_kind = DeeExprAstKind_CONSTANT;
       Dee_INHERIT_REF(a->ob_const,temp);
      }
     }
    } else if (token.tk_id == KWD_class) {
     if (!yield()) goto err_r;
     r->ob_kind = DeeExprAstKind_CLASSOF;
     Dee_INHERIT_REF(a,r);
     if (a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
        (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
         DEE_HAVE_CONSTOP_OPTIMIZATION_CLASSOF(a->ob_op_a->ob_const)) {
      DeeObject *temp = (DeeObject *)DeeObject_ClassOf(a->ob_op_a->ob_const);
      Dee_DECREF(a->ob_op_a);
      a->ob_kind = DeeExprAstKind_CONSTANT;
      Dee_INCREF(a->ob_const = temp);
     }
    } else if (token.tk_id == KWD_this) {
     if (!yield()) goto err_r;
     a = r->ob_op_a;
     r->ob_op_a = NULL;
     Dee_DECREF(r);
    } else if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT,(DeeObject *)self,token_ob,
      "Expected <keyword> after '<expr>.'") == -1) goto err_r;
     a = r->ob_op_a;
     r->ob_op_a = NULL;
     Dee_DECREF(r);
    } else {
     char const *name;
     name = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id);
     if ((r->ob_member = (name
      ? (DeeStringObject *)DeeString_New(name)
      : (DeeStringObject *)DeeToken_Str(token_ob)
      )) == NULL || !yield()) goto err_r;
     if (require_constant) {
      // NOTE: We always optimize 'a' in this case!
      if (_do_optimize_ast(r) == -1) goto err_r;
      if (r->ob_kind != DeeExprAstKind_CONSTANT) {
       DEE_ASSERT(r->ob_kind == DeeExprAstKind_ATTR_GET);
       if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_COLLON_COLLON,
        (DeeObject *)self,(DeeObject *)r->ob_token,
        "Expected constant expression resulting from <expr>::%s",
        DeeString_STR(r->ob_member)) == -1) goto err_r;
      }
     }
     goto use_r;
    }
   }
  } break;

  case '[': // tp_seq_get / tp_seq_range_get
   // NOTE: No constant optimization here for the same reason as tp_attr_get
   if (_optimize_ast(a) == -1) goto err_a;
   if ((r = new_ast(DeeExprAstKind_SEQ_GET,token_ob)) == NULL) goto err_a;
   Dee_INHERIT_REF(r->ob_op_a,a);  // Use the old ast as <expr> a
   if (!yield()) goto err_r;
   if (token.tk_id == ':') {
    r->ob_op_b = NULL; // Unspecified
    goto g_rng;
   } else {
    if ((r->ob_op_b = _deeparse_top(self,scope,flags&~(
     DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_r;
    if (token.tk_id == ':') {
g_rng:
     if (!yield()) goto err_r;
     r->ob_kind = DeeExprAstKind_SEQ_RANGE_GET;
     if (token.tk_id == ']') {
      r->ob_op_c = NULL; // Unspecified
     } else {
      if ((r->ob_op_c = _deeparse_top(self,scope,flags&~(
       DEE_PARSER_FLAG_NO_BRACE_INIT|DEE_PARSER_FLAG_NO_TYPE_SUFFIX))) == NULL) goto err_r;
     }
    }
   }
   if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_GETATTR,(DeeObject *)self,token_ob,"Expected ']' after '<expr>[<expr>'") == -1) goto err_r;
   } else if (!yield()) goto err_r;
   goto use_r; // Continue working with the new ast
   //break;

  case TPP_TOK_DOTS:
   if (a->ob_kind == DeeExprAstKind_EXPAND) {
    if (DeeError_CompilerError(DEE_WARNING_DOUBLE_EXPAND_REQUIRES_INNER_EXPAND_SINGLE,(DeeObject *)self,token_ob,
     "Expanding an expand-expression requires the inner sequence "
     "to have a length of 1 (are 6 dots really intended?)") == -1) goto err_a;
   } else if (a->ob_kind == DeeExprAstKind_CONSTANT &&
             (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0) {
    DeeObject *const_copy,*iterator,*elem,*next_elem;
    // Check if the constant of 'a' can be expanded to a single object
    // e.g.: optimize '[42]...' --> '42'
    if ((const_copy = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_a;
    iterator = DeeObject_IterSelf(const_copy);
    Dee_DECREF(const_copy);
    if (!iterator) {
invop_expand:
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_EXPAND,(DeeObject *)self,token_ob,
      "Invalid constant operator: Calling '%#q...' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_a;
    } else {
     int error;
     if ((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
      Dee_DECREF(iterator);
      if (error < 0) goto invop_expand;
      // Expanding to nothing
     } else {
      error = DeeObject_IterNextEx(iterator,&next_elem);
      Dee_DECREF(iterator);
      if (error == 0) {
       Dee_DECREF(elem);
       Dee_DECREF(next_elem);
      } else if (error < 0) {
       Dee_DECREF(elem);
       goto invop_expand;
      } else {
       // Yes! This constant expands to 'elem'
       Dee_DECREF(a->ob_const);
       Dee_INHERIT_REF(a->ob_const,elem);
       if (!yield()) goto err_a;
       break;
      }
     }
    }
   }
   kind = DeeExprAstKind_EXPAND;
   goto unaryp;
  case TPP_TOK_INC: kind = DeeExprAstKind_INC_POST; goto unaryp;
  case TPP_TOK_DEC: kind = DeeExprAstKind_DEC_POST; unaryp:
   if (_optimize_ast(a) == -1) goto err_a;
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
      (kind == DeeExprAstKind_INC_POST || kind == DeeExprAstKind_DEC_POST) &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
    DeeObject *unary_res,*lhs;
    if (!_DeeType_IsSlotConstant(Dee_TYPE(a->ob_const),kind)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_MODIFYING_CONSTANT,(DeeObject *)self,(DeeObject *)a->ob_token,
      "Possible modifications to the constant in '%#q %s' will be ignored",
      DeeType_NAME(Dee_TYPE(a->ob_const)),kind == DeeExprAstKind_INC_POST ? "++" : "--") == -1) goto err_a;
    }
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_a;
    switch (kind) {
     case DeeExprAstKind_INC_POST: unary_res = DeeObject_IncPost(lhs); break;
     case DeeExprAstKind_DEC_POST: unary_res = DeeObject_DecPost(lhs); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    Dee_DECREF(lhs);
    if (!unary_res) {
     if (DeeError_CompilerErrorf(kind == DeeExprAstKind_INC_POST
      ? DEE_WARNING_INVALID_CONSTOP_INCPOST
      : DEE_WARNING_INVALID_CONSTOP_DECPOST,(DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '%#q %s' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),
      kind == DeeExprAstKind_INC_POST ? "++" : "--",
      _DeeError_HandleReturnType()) == -1) goto err_a;
     goto def_unary_p;
    }
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,unary_res);
    if (!yield()) goto err_a;
    break;
   } else {
def_unary_p:
    if ((r = new_ast(kind,token_ob)) == NULL) goto err_a;
    Dee_INHERIT_REF(r->ob_op_a,a);
    if (!yield()) goto err_r;
   }
use_r: // Continue working with the new ast
   Dee_INHERIT_REF(a,r);
   break;

  default:
   goto end;
 }
end:
 // Transform a module constant
 if (a && a->ob_kind == DeeExprAstKind_MODULE) {
  DeeObject *module_repr;
  if ((module_repr = a->ob_mod_module->ob_load_val) == NULL) {
   if (DeeError_CompilerError(DEE_WARNING_CANT_USE_MODULE_IN_EXPRESSION,(DeeObject *)self,
    (DeeObject *)a->ob_token,"Can't use modules in expressions") == -1) goto err_a;
   module_repr = Dee_None;
  }
  Dee_INCREF(module_repr);
  Dee_DECREF(a->ob_mod_module);
  a->ob_kind = DeeExprAstKind_CONSTANT;
  Dee_INHERIT_REF(a->ob_const,module_repr);
 }
 return a;
err_a:
 Dee_DECREF(a);
 return NULL;
err_r:
 Dee_DECREF(r);
 return NULL;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_prod(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind;
 DeeExprAstObject *b,*a = _deeparse_unary(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case TPP_TOK_POW: kind = DeeExprAstKind_POW; goto def_tycheck;
  case '%': kind = DeeExprAstKind_MOD; goto def;
  case '/': kind = DeeExprAstKind_DIV; goto def;
  case '*':
   kind = DeeExprAstKind_MUL;
def_tycheck:
   if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0 ||
       a->ob_kind != DeeExprAstKind_CONSTANT ||
      (!DeeNone_Check(a->ob_const) && !DeeType_Check(a->ob_const))) {
def:
    if (!yield() || (b = _deeparse_unary(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
     if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
    }
    if (a->ob_kind == DeeExprAstKind_CONSTANT &&
        b->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
        DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
     DeeObject *lhs,*rhs,*cresult;
     if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
     if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
     switch (kind) {
      case DeeExprAstKind_MOD: cresult = DeeObject_Mod(lhs,rhs); break;
      case DeeExprAstKind_DIV: cresult = DeeObject_Div(lhs,rhs); break;
      case DeeExprAstKind_MUL: cresult = DeeObject_Mul(lhs,rhs); break;
      case DeeExprAstKind_POW: cresult = DeeObject_Pow(lhs,rhs); break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     Dee_DECREF(rhs);
     Dee_DECREF(lhs);
     if (!cresult) {
      if (DeeError_CompilerErrorf(kind == DeeExprAstKind_MOD
       ? DEE_WARNING_INVALID_CONSTOP_MOD : kind == DeeExprAstKind_DIV
       ? DEE_WARNING_INVALID_CONSTOP_DIV : kind == DeeExprAstKind_POW
       ? DEE_WARNING_INVALID_CONSTOP_POW : DEE_WARNING_INVALID_CONSTOP_MUL,
       (DeeObject *)self,(DeeObject *)a->ob_token,
       "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(a->ob_const)),
       kind == DeeExprAstKind_MOD ? "%" :
       kind == DeeExprAstKind_DIV ? "/" :
       kind == DeeExprAstKind_POW ? "**" : "*",
       DeeType_NAME(Dee_TYPE(b->ob_const)),
       _DeeError_HandleReturnType()) == -1) goto err_b;
      goto def_bin;
     }
     Dee_DECREF(b); // no longer needed
     Dee_DECREF(a->ob_const);
     Dee_INHERIT_REF(a->ob_const,cresult);
    } else {
def_bin:
     if ((r = new_ast(kind,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
     Dee_INHERIT_REF(r->ob_op_a,a);
     Dee_INHERIT_REF(r->ob_op_b,b);
     Dee_INHERIT_REF(a,r);
    }
   } else goto end;
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
_dee_optimize_empty_str_plus(DeeExprAstObject *self, DeeScopeObject *scope) {
 DeeExprAstObject *rhs;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_ADD);
 DEE_ASSERT(self->ob_op_a != NULL);
 DEE_ASSERT(self->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT);
 DEE_ASSERT(self->ob_op_a->ob_const == Dee_EmptyString);
 rhs = self->ob_op_b; self->ob_op_b = NULL;
 DEE_ASSERT(rhs != NULL);
 Dee_DECREF(self->ob_op_a);
 if (DeeExprAst_PredictType((DeeObject *)rhs,(DeeObject *)scope) == &DeeString_Type) {
  Dee_DECREF(self->ob_token);
  Dee_INCREF(self->ob_token = rhs->ob_token);
  self->ob_kind = rhs->ob_kind;
  self->ob_op_a = rhs->ob_op_a; rhs->ob_op_a = NULL;
  self->ob_op_b = rhs->ob_op_b; rhs->ob_op_b = NULL;
  self->ob_op_c = rhs->ob_op_c; rhs->ob_op_c = NULL;
  self->ob_op_d = rhs->ob_op_d; rhs->ob_op_d = NULL;
  Dee_DECREF(rhs);
 } else {
  self->ob_kind = DeeExprAstKind_STR;
  Dee_INHERIT_REF(self->ob_op_a,rhs);
 }
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_sum(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind;
 DeeExprAstObject *b,*a = _deeparse_prod(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case '+': kind = DeeExprAstKind_ADD; goto def;
  case '-': kind = DeeExprAstKind_SUB; def:
   if (!yield() || (b = _deeparse_prod(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
   }
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       b->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    switch (kind) {
     case DeeExprAstKind_ADD: cresult = DeeObject_Add(lhs,rhs); break;
     case DeeExprAstKind_SUB: cresult = DeeObject_Sub(lhs,rhs); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    if (!cresult) {
     if (DeeError_CompilerErrorf(kind == DeeExprAstKind_ADD
      ? DEE_WARNING_INVALID_CONSTOP_ADD : DEE_WARNING_INVALID_CONSTOP_SUB,
      (DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),
      kind == DeeExprAstKind_ADD ? "+" : "-",
      DeeType_NAME(Dee_TYPE(b->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    Dee_DECREF(b); // no longer needed
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(kind,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_b,b);
    Dee_INHERIT_REF(a,r);
    if (a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
        a->ob_op_a->ob_const == Dee_EmptyString &&
        (flags&DEE_PARSER_FLAG_OPTIMIZE_EMPTY_STR_ADD)!=0) {
     if (_dee_optimize_empty_str_plus(a,scope) == -1) goto err_a;
    }
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_shift(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind;
 DeeExprAstObject *b,*a = _deeparse_sum(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case TPP_TOK_SHL: kind = DeeExprAstKind_SHL; goto def;
  case TPP_TOK_SHR: kind = DeeExprAstKind_SHR; def:
   if (!yield() || (b = _deeparse_sum(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
   }
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       b->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION_SHIFT(a->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    switch (kind) {
     case DeeExprAstKind_SHL: cresult = DeeObject_Shl(lhs,rhs); break;
     case DeeExprAstKind_SHR: cresult = DeeObject_Shr(lhs,rhs); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    if (!cresult) {
     if (DeeError_CompilerErrorf(kind == DeeExprAstKind_SHL
      ? DEE_WARNING_INVALID_CONSTOP_SHL : DEE_WARNING_INVALID_CONSTOP_SHR,
      (DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),
      kind == DeeExprAstKind_SHL ? "<<" : ">>",
      DeeType_NAME(Dee_TYPE(b->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    Dee_DECREF(b); // no longer needed
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(kind,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_b,b);
    Dee_INHERIT_REF(a,r);
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

static DEE_A_RET_EXCEPT(-1) int _dee_optimize_is(
 DeeExprAstObject *self, DeeScopeObject *scope) {
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_IS);
 if (self->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT &&
     // v only compile away this check, if evaluating it doesn't have any side-effects
     DeeExprAst_IsNoeffect((DeeObject *)self->ob_op_a,(DeeObject *)scope)) {
  DeeTypeObject *predicted_lhs;
  if (self->ob_op_b->ob_const == (DeeObject *)&DeeObject_Type) {
   // Everything is an object!
   Dee_DECREF(self->ob_op_a);
   Dee_CLEAR(self->ob_op_b);
   self->ob_kind = DeeExprAstKind_CONSTANT;
   self->ob_const = DeeBool_NewTrue();
  } else if ((predicted_lhs = DeeExprAst_PredictType((DeeObject *)self->ob_op_a,(DeeObject *)scope)) != NULL) {
   // Evaluating 'lhs' doesn't have any side-effects and we can predict its type
   // >> So we can also predict the result of this is-expression
   int result = DeeType_IsSameOrDerived(predicted_lhs,(DeeTypeObject *)self->ob_op_b->ob_const);
   Dee_DECREF(self->ob_op_a);
   Dee_CLEAR(self->ob_op_b);
   self->ob_kind = DeeExprAstKind_CONSTANT;
   self->ob_const = DeeBool_New(result);
  }
 }
 return 0;
}

DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_cmp(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind; DeeObject *lhs,*rhs,*cresult;
 DeeTokenObject *temp_tok;
 DeeExprAstObject *b,*a = _deeparse_shift(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {

  case '!': {
   Dee_INCREF(temp_tok = (DeeTokenObject *)token_ob);
   if (!yield()){err_temp_tok: Dee_DECREF(temp_tok); err_a: Dee_DECREF(a); return NULL; }
   if (token.tk_id == KWD_in || token.tk_id == KWD_is) {
    // Alias 'x !in y' to '!(x in y)'
    r = new_ast(DeeExprAstKind_NOT,(DeeObject *)temp_tok);
    Dee_DECREF(temp_tok);
    if (!r) goto err_a;
    if ((b = new_ast((token.tk_id == KWD_in
     ? DeeExprAstKind_IN : DeeExprAstKind_IS),
     token_ob)) == NULL) { Dee_DECREF(r); goto err_a; }
    Dee_INHERIT_REF(b->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_a,b);
    Dee_INHERIT_REF(a,r);
parse_inv_inis:
    if (!yield()) goto err_a;
    if ((b = _deeparse_shift(self,scope,flags)) == NULL) goto err_a;
    Dee_INHERIT_REF(a->ob_op_a->ob_op_b,b);
    // Everything is now build >> check for constant optimizations
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
     if (_do_optimize_ast(a->ob_op_a->ob_op_a) == -1) goto err_a;
     if (_do_optimize_ast(a->ob_op_a->ob_op_b) == -1) goto err_a;
     if (a->ob_op_a->ob_kind == DeeExprAstKind_IS) {
      if (a->ob_op_a->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT &&
          DeeNone_Check(a->ob_op_a->ob_op_b->ob_const)) {
       // 'none' counts as 'type none' in this context
       Dee_DECREF(a->ob_op_a->ob_op_b->ob_const);
       Dee_INCREF(a->ob_op_a->ob_op_b->ob_const = (DeeObject *)&DeeNone_Type);
      }
      if (_dee_optimize_is(a->ob_op_a,scope) == -1) goto err_a;
      if (a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT) goto optimize_not;
     }
     if (a->ob_op_a->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
         a->ob_op_a->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT) {
      if (a->ob_op_a->ob_kind == DeeExprAstKind_IS) {
       int temp = DeeObject_Is(
                         a->ob_op_a->ob_op_a->ob_const,
        (DeeTypeObject *)a->ob_op_a->ob_op_b->ob_const);
       Dee_DECREF(a->ob_op_a);
       a->ob_kind = DeeExprAstKind_CONSTANT;
       a->ob_const = DeeBool_New(!temp);
      } else if (DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_op_a->ob_op_a->ob_const)) {
       if ((lhs = DeeObject_DeepCopy(a->ob_op_a->ob_op_a->ob_const)) == NULL) goto err_a;
       if ((rhs = DeeObject_DeepCopy(a->ob_op_a->ob_op_b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_a; }
       cresult = DeeObject_ContainsObject(rhs,lhs);
       Dee_DECREF(rhs);
       Dee_DECREF(lhs);
       if (!cresult) {
        if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_IN,
         (DeeObject *)self,(DeeObject *)a->ob_op_a->ob_token,
         "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
         DeeType_NAME(Dee_TYPE(a->ob_op_a->ob_op_a->ob_const)),
         a->ob_op_a->ob_kind == DeeExprAstKind_IN ? "in" : "is",
         DeeType_NAME(Dee_TYPE(a->ob_op_a->ob_op_b->ob_const)),
         _DeeError_HandleReturnType()) == -1) goto err_a;
       } else {
        Dee_DECREF(a->ob_op_a->ob_op_a);
        Dee_CLEAR(a->ob_op_a->ob_op_b);
        Dee_INHERIT_REF(a->ob_op_a->ob_const,cresult);
        a->ob_op_a->ob_kind = DeeExprAstKind_CONSTANT;
        // Now optimize the 'tp_not'
optimize_not:
        if ((lhs = DeeObject_DeepCopy(a->ob_op_a->ob_const)) == NULL) goto err_a;
        cresult = DeeObject_Not(lhs);
        Dee_DECREF(lhs);
        if (!cresult) {
         if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_NOT,
          (DeeObject *)self,(DeeObject *)a->ob_token,
          "Invalid constant operator: Calling '!(%#q)' will throw an '%K' at runtime",
          DeeType_NAME(Dee_TYPE(a->ob_op_a->ob_const)),
          _DeeError_HandleReturnType()) == -1) goto err_a;
        } else {
         Dee_DECREF(a->ob_op_a);
         Dee_INHERIT_REF(a->ob_const,cresult);
         a->ob_kind = DeeExprAstKind_CONSTANT;
        }
       }
      }
     }
    }
   } else {
    if (DeeLexer_Return((DeeObject *)self,(DeeObject *)temp_tok) == -1) goto err_temp_tok;
    Dee_DECREF(temp_tok);
    goto end;
   }
  } break;


  case KWD_is: kind = DeeExprAstKind_IS; goto def;
  case KWD_in: kind = DeeExprAstKind_IN; goto def;
  case TPP_TOK_CMP_LO: kind = DeeExprAstKind_CMP_LO; goto def;
  case TPP_TOK_CMP_LE: kind = DeeExprAstKind_CMP_LE; goto def;
  case TPP_TOK_CMP_GR: kind = DeeExprAstKind_CMP_GR; goto def;
  case TPP_TOK_CMP_GE: kind = DeeExprAstKind_CMP_GE; def:
   Dee_INCREF(temp_tok = (DeeTokenObject *)token_ob);
   if (!yield()) goto err_temp_tok;
   if (token.tk_id == '!' && (kind == DeeExprAstKind_IN || kind == DeeExprAstKind_IS)) {
    // Alias 'x in! y' to '!(x in y)'
    if ((r = new_ast(DeeExprAstKind_NOT,token_ob)) == NULL) goto err_temp_tok;
    if ((b = new_ast(kind,(DeeObject *)temp_tok)) == NULL) { Dee_DECREF(r); goto err_temp_tok; }
    Dee_DECREF(temp_tok);
    Dee_INHERIT_REF(b->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_a,b);
    Dee_INHERIT_REF(a,r);
    goto parse_inv_inis;
   }
   if ((b = _deeparse_shift(self,scope,flags)) == NULL) goto err_temp_tok;
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_ba_tok;
    if (a->ob_kind == DeeExprAstKind_CONSTANT &&
        b->ob_kind == DeeExprAstKind_CONSTANT &&
       (kind == DeeExprAstKind_IS || DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const))) {
     if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_ba_tok;
     if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_ba_tok;}
     switch (kind) {
      case DeeExprAstKind_IS: cresult = DeeBool_New(DeeObject_Is(lhs,(DeeTypeObject *)rhs)); break;
      case DeeExprAstKind_IN: cresult = DeeObject_ContainsObject(rhs,lhs); break;
      case DeeExprAstKind_CMP_LO: cresult = DeeObject_CompareLoObject(lhs,rhs); break;
      case DeeExprAstKind_CMP_LE: cresult = DeeObject_CompareLeObject(lhs,rhs); break;
      case DeeExprAstKind_CMP_GR: cresult = DeeObject_CompareGrObject(lhs,rhs); break;
      case DeeExprAstKind_CMP_GE: cresult = DeeObject_CompareGeObject(lhs,rhs); break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     Dee_DECREF(rhs);
     Dee_DECREF(lhs);
     if (!cresult) {
      char const *op_symb; int error_code;
      switch (kind) {
       case DeeExprAstKind_IS:     op_symb = "is"; error_code = DEE_WARNING_INVALID_CONSTOP_IS; break;
       case DeeExprAstKind_IN:     op_symb = "in"; error_code = DEE_WARNING_INVALID_CONSTOP_IN; break;
       case DeeExprAstKind_CMP_LO: op_symb = "<";  error_code = DEE_WARNING_INVALID_CONSTOP_CMP_LO; break;
       case DeeExprAstKind_CMP_LE: op_symb = "<="; error_code = DEE_WARNING_INVALID_CONSTOP_CMP_LE; break;
       case DeeExprAstKind_CMP_GR: op_symb = ">";  error_code = DEE_WARNING_INVALID_CONSTOP_CMP_GR; break;
       case DeeExprAstKind_CMP_GE: op_symb = ">="; error_code = DEE_WARNING_INVALID_CONSTOP_CMP_GE; break;
       default: DEE_BUILTIN_UNREACHABLE();
      }
      if (DeeError_CompilerErrorf(error_code,
       (DeeObject *)self,(DeeObject *)a->ob_token,
       "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(a->ob_const)),op_symb,
       DeeType_NAME(Dee_TYPE(b->ob_const)),
       _DeeError_HandleReturnType()) == -1) goto err_ba_tok;
      goto def_bin;
     }
     Dee_DECREF(b); // no longer needed
     Dee_DECREF(a->ob_const);
     Dee_INHERIT_REF(a->ob_const,cresult);
     Dee_DECREF(temp_tok);
    } else {
     goto def_bin;
    }
   } else {
def_bin:
    if ((r = new_ast(kind,(DeeObject *)temp_tok)) == NULL) {err_ba_tok: Dee_DECREF(b); goto err_temp_tok; }
    Dee_DECREF(temp_tok);
    Dee_INHERIT_REF(r->ob_op_a,a);
    r->ob_op_b = b; // inherit reference
    Dee_INHERIT_REF(a,r);
    if (a->ob_kind == DeeExprAstKind_IS) {
     if (b->ob_kind == DeeExprAstKind_CONSTANT && DeeNone_Check(b->ob_const)) {
      // Optimization: 'x is none' --> 'x is type none'
      Dee_DECREF(b->ob_const);
      Dee_INCREF(b->ob_const = (DeeObject *)&DeeNone_Type);
     }
     if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
      if (_dee_optimize_is(a,scope) == -1) goto err_a;
     }
    }
#ifdef DEE_DEBUG
    b = NULL;
#endif
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_cmp_eq(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstKind kind;
 DeeExprAstObject *b,*a = _deeparse_cmp(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case TPP_TOK_CMP_EQ: kind = DeeExprAstKind_CMP_EQ; goto def;
  case TPP_TOK_CMP_NE: kind = DeeExprAstKind_CMP_NE; def:
   if (!yield() || (b = _deeparse_cmp(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
   }
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       b->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    switch (kind) {
     case DeeExprAstKind_CMP_EQ: cresult = DeeObject_CompareEqObject(lhs,rhs); break;
     case DeeExprAstKind_CMP_NE: cresult = DeeObject_CompareNeObject(lhs,rhs); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    if (!cresult) {
     if (DeeError_CompilerErrorf(kind == DeeExprAstKind_CMP_EQ
      ? DEE_WARNING_INVALID_CONSTOP_CMP_EQ : DEE_WARNING_INVALID_CONSTOP_CMP_NE,
      (DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),
      kind == DeeExprAstKind_CMP_EQ ? "==" : "!=",
      DeeType_NAME(Dee_TYPE(b->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    Dee_DECREF(b); // no longer needed
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(kind,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_b,b);
    Dee_INHERIT_REF(a,r);
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_and(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *b,*a = _deeparse_cmp_eq(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case '&':
   if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0 ||
       a->ob_kind != DeeExprAstKind_CONSTANT ||
      (!DeeNone_Check(a->ob_const) && !DeeType_Check(a->ob_const))) {
    if (!yield() || (b = _deeparse_cmp_eq(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
     if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
    }
    if (a->ob_kind == DeeExprAstKind_CONSTANT &&
        b->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
        DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
     DeeObject *lhs,*rhs,*cresult;
     if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
     if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
     cresult = DeeObject_And(lhs,rhs);
     Dee_DECREF(rhs);
     Dee_DECREF(lhs);
     if (!cresult) {
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_AND,
       (DeeObject *)self,(DeeObject *)a->ob_token,
       "Invalid constant operator: Calling '(%#q) & (%#q)' will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(a->ob_const)),DeeType_NAME(Dee_TYPE(b->ob_const)),
       _DeeError_HandleReturnType()) == -1) goto err_b;
      goto def_bin;
     }
     Dee_DECREF(b); // no longer needed
     Dee_DECREF(a->ob_const);
     Dee_INHERIT_REF(a->ob_const,cresult);
    } else {
def_bin:
     if ((r = new_ast(DeeExprAstKind_AND,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
     Dee_INHERIT_REF(r->ob_op_a,a);
     Dee_INHERIT_REF(r->ob_op_b,b);
     Dee_INHERIT_REF(a,r);
    }
   } else goto end;
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_xor(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *b,*a = _deeparse_and(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case '^':
   if (!yield() || (b = _deeparse_and(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
   }
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       b->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    cresult = DeeObject_Xor(lhs,rhs);
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    if (!cresult) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_XOR,
      (DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '(%#q) ^ (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),DeeType_NAME(Dee_TYPE(b->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    Dee_DECREF(b); // no longer needed
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(DeeExprAstKind_XOR,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_b,b);
    Dee_INHERIT_REF(a,r);
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_or(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *b,*a = _deeparse_xor(self,scope,flags),*r;
 if (a) while (1) switch (token.tk_id) {
  case '|':
   if (!yield() || (b = _deeparse_xor(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(a) == -1 || _do_optimize_ast(b) == -1) goto err_b;
   }
   if (a->ob_kind == DeeExprAstKind_CONSTANT &&
       b->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION_OR(a->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    cresult = DeeObject_Or(lhs,rhs);
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    if (!cresult) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_AND,
      (DeeObject *)self,(DeeObject *)a->ob_token,
      "Invalid constant operator: Calling '(%#q) | (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(a->ob_const)),DeeType_NAME(Dee_TYPE(b->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    Dee_DECREF(b); // no longer needed
    Dee_DECREF(a->ob_const);
    Dee_INHERIT_REF(a->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(DeeExprAstKind_OR,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,a);
    Dee_INHERIT_REF(r->ob_op_b,b);
    Dee_INHERIT_REF(a,r);
   }
   break;
  default:
   goto end;
 }
end:
 return a;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_land(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *rhs_branch,*lhs_branch,*r; DeeTokenObject *tok;
 lhs_branch = _deeparse_or(self,scope,flags);
 if (lhs_branch) while (1) switch (token.tk_id) {
  case TPP_TOK_LAND:
   if (_optimize_ast(lhs_branch) == -1) goto err_lhsb_raw;
   Dee_INCREF(tok = (DeeTokenObject *)token_ob);
   if (lhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs_branch->ob_const)) {
    int lhs_is_true;
    if ((lhs_is_true = DeeObject_Bool(lhs_branch->ob_const)) == -1) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_LOR,
      (DeeObject *)self,(DeeObject *)lhs_branch->ob_token,
      "Invalid constant operator: Calling '(%#q) && ...' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs_branch->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_lhsb;
    } else {
     if (!yield() || (rhs_branch = _deeparse_or(self,scope,(Dee_uint32_t)(lhs_is_true
      ? flags : flags|DEE_PARSER_FLAG_UNREACHABLE))) == NULL) goto err_lhsb;
     if (_optimize_ast(rhs_branch) == -1) goto err_rhsb;
     Dee_DECREF(lhs_branch->ob_token);
     Dee_INHERIT_REF(lhs_branch->ob_token,tok);
     Dee_DECREF(lhs_branch->ob_const);
     if (!lhs_is_true) {
      Dee_DECREF(rhs_branch);
      lhs_branch->ob_const = DeeBool_NewFalse();
      return lhs_branch;
     } else {
      // !!rhs_branch
      lhs_branch->ob_kind = DeeExprAstKind_BOOL;
      lhs_branch->ob_op_a = rhs_branch; // Inherit reference
      if (rhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
         (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
          DEE_HAVE_CONSTOP_OPTIMIZATION(rhs_branch->ob_const)) {
       if ((lhs_is_true = DeeObject_Bool(rhs_branch->ob_const)) == -1) {
        if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_LOR,
         (DeeObject *)self,(DeeObject *)rhs_branch->ob_token,
         "Invalid constant operator: Calling 'true && (%#q)' will throw an '%K' at runtime",
         DeeType_NAME(Dee_TYPE(rhs_branch->ob_const)),_DeeError_HandleReturnType()) == -1) {
err_lhsb_raw: Dee_DECREF(lhs_branch); return NULL;
        }
       } else {
        Dee_DECREF(rhs_branch->ob_const);
        rhs_branch->ob_const = DeeBool_New(lhs_branch);
        lhs_branch->ob_op_a = NULL;
        Dee_DECREF(lhs_branch);
        return rhs_branch;
       }
      }
      return lhs_branch;
     }
    }
   }
   if (!yield() || (rhs_branch = _deeparse_or(self,scope,flags)) == NULL) {
err_lhsb: Dee_DECREF(lhs_branch); Dee_DECREF(tok); return NULL;
   }
   if (_optimize_ast(rhs_branch) == -1) goto err_rhsb;
   if ((r = new_ast(DeeExprAstKind_LAND,(DeeObject *)tok)) == NULL) {err_rhsb: Dee_DECREF(rhs_branch); goto err_lhsb; }
   Dee_INHERIT_REF(r->ob_op_a,lhs_branch);
   Dee_INHERIT_REF(r->ob_op_b,rhs_branch);
   Dee_INHERIT_REF(lhs_branch,r);
   Dee_DECREF(tok);
   break;
  default: goto end;
 }
end: return lhs_branch;
}

DEE_A_RET_EXCEPT_REF DeeExprAstObject *
_deeparse_lxor(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *rhs_branch,*lhs_branch,*r;
 lhs_branch = _deeparse_land(self,scope,flags);
 if (lhs_branch) while (1) switch (token.tk_id) {
  case TPP_TOK_LXOR:
   if (!yield() || (rhs_branch = _deeparse_land(self,scope,flags)) == NULL) {err_a: Dee_DECREF(lhs_branch); return NULL; }
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    if (_do_optimize_ast(lhs_branch) == -1 || _do_optimize_ast(rhs_branch) == -1) goto err_b;
   }
   if (lhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
       rhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs_branch->ob_const)) {
    DeeObject *lhs,*rhs,*cresult;
    int b_lhs,b_rhs;
    if ((lhs = DeeObject_DeepCopy(lhs_branch->ob_const)) == NULL) goto err_b;
    if ((rhs = DeeObject_DeepCopy(rhs_branch->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
    if ((b_lhs = DeeObject_Bool(lhs)) == -1) {
err_ceval:
     Dee_DECREF(lhs);
     Dee_DECREF(rhs);
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_LXOR,
      (DeeObject *)self,(DeeObject *)lhs_branch->ob_token,
      "Invalid constant operator: Calling '(%#q) ^^ (%#q)' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs_branch->ob_const)),DeeType_NAME(Dee_TYPE(rhs_branch->ob_const)),
      _DeeError_HandleReturnType()) == -1) goto err_b;
     goto def_bin;
    }
    if ((b_rhs = DeeObject_Bool(rhs)) == -1) goto err_ceval;
    Dee_INCREF(cresult = (b_lhs^b_rhs) ? Dee_True : Dee_False);
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    Dee_DECREF(rhs_branch); // no longer needed
    Dee_DECREF(lhs_branch->ob_const);
    Dee_INHERIT_REF(lhs_branch->ob_const,cresult);
   } else {
def_bin:
    if ((r = new_ast(DeeExprAstKind_LXOR,token_ob)) == NULL) {err_b: Dee_DECREF(rhs_branch); goto err_a; }
    Dee_INHERIT_REF(r->ob_op_a,lhs_branch);
    Dee_INHERIT_REF(r->ob_op_b,rhs_branch);
    Dee_INHERIT_REF(lhs_branch,r);
   }
   break;
  default:
   goto end;
 }
end:
 return lhs_branch;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_lor(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *rhs_branch,*lhs_branch,*r; DeeTokenObject *tok;
 lhs_branch = _deeparse_lxor(self,scope,flags);
 if (lhs_branch) while (1) switch (token.tk_id) {
  case TPP_TOK_LOR:
   if (_optimize_ast(lhs_branch) == -1) goto err_lhsb_raw;
   Dee_INCREF(tok = (DeeTokenObject *)token_ob);
   if (lhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
      (flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs_branch->ob_const)) {
    int lhs_is_true;
    if ((lhs_is_true = DeeObject_Bool(lhs_branch->ob_const)) == -1) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_LOR,
      (DeeObject *)self,(DeeObject *)lhs_branch->ob_token,
      "Invalid constant operator: Calling '(%#q) || ...' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs_branch->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_lhsb;
    } else {
     if (!yield() || (rhs_branch = _deeparse_lxor(self,scope,(Dee_uint32_t)(lhs_is_true
      ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags))) == NULL) goto err_lhsb;
     if (_optimize_ast(rhs_branch) == -1) goto err_rhsb;
     Dee_DECREF(lhs_branch->ob_token);
     Dee_INHERIT_REF(lhs_branch->ob_token,tok);
     Dee_DECREF(lhs_branch->ob_const);
     if (lhs_is_true) {
      Dee_DECREF(rhs_branch);
      lhs_branch->ob_const = DeeBool_NewTrue();
      return lhs_branch;
     } else {
      // !!rhs_branch
      lhs_branch->ob_kind = DeeExprAstKind_BOOL;
      lhs_branch->ob_op_a = rhs_branch; // Inherit reference
      if (rhs_branch->ob_kind == DeeExprAstKind_CONSTANT &&
         (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
          DEE_HAVE_CONSTOP_OPTIMIZATION(rhs_branch->ob_const)) {
       if ((lhs_is_true = DeeObject_Bool(rhs_branch->ob_const)) == -1) {
        if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_LOR,
         (DeeObject *)self,(DeeObject *)rhs_branch->ob_token,
         "Invalid constant operator: Calling 'false || (%#q)' will throw an '%K' at runtime",
         DeeType_NAME(Dee_TYPE(rhs_branch->ob_const)),_DeeError_HandleReturnType()) == -1) {
err_lhsb_raw: Dee_DECREF(lhs_branch); return NULL;
        }
       } else {
        Dee_DECREF(rhs_branch->ob_const);
        rhs_branch->ob_const = DeeBool_New(lhs_branch);
        lhs_branch->ob_op_a = NULL;
        Dee_DECREF(lhs_branch);
        return rhs_branch;
       }
      }
      return lhs_branch;
     }
    }
   }
   if (!yield() || (rhs_branch = _deeparse_lxor(self,scope,flags)) == NULL) {
err_lhsb: Dee_DECREF(lhs_branch); Dee_DECREF(tok); return NULL;
   }
   if (_optimize_ast(rhs_branch) == -1) goto err_rhsb;
   if ((r = new_ast(DeeExprAstKind_LOR,(DeeObject *)tok)) == NULL) {err_rhsb: Dee_DECREF(rhs_branch); goto err_lhsb; }
   Dee_INHERIT_REF(r->ob_op_a,lhs_branch);
   Dee_INHERIT_REF(r->ob_op_b,rhs_branch);
   Dee_INHERIT_REF(lhs_branch,r);
   Dee_DECREF(tok);
   break;
  default: goto end;
 }
end: return lhs_branch;
}
static DeeExprAstObject *
_deeparse_cond(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTokenObject *tok;
 DeeExprAstObject *true_branch,*false_branch,*cond_branch = _deeparse_lor(self,scope,flags),*r;
 if (cond_branch) while (1) if (token.tk_id == '?') {
  Dee_INCREF(tok = (DeeTokenObject *)token_ob);
  if (!yield()) goto err_tok;
  if (_optimize_ast(cond_branch) == -1) goto err_condb;
  if (cond_branch->ob_kind == DeeExprAstKind_CONSTANT &&
     (flags & DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
      DEE_HAVE_CONSTOP_OPTIMIZATION(cond_branch->ob_const)) {
   int is_true;
   if ((is_true = DeeObject_Bool(cond_branch->ob_const)) == -1) {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_COND,(DeeObject *)self,(DeeObject *)cond_branch->ob_token,
     "Invalid constant operator: Using '%#q' as condition in if-expression will throw an '%K' at runtime",
     DeeType_NAME(Dee_TYPE(cond_branch->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_condb;
   } else {
    if (token.tk_id == ':') {
     if (!yield() || (false_branch = _deeparse_top(
      self,scope,(Dee_uint32_t)(is_true ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)
      )) == NULL) goto err_condb;
     Dee_DECREF(tok);
     if (is_true) {
      Dee_DECREF(false_branch);
      return cond_branch;
     } else {
      Dee_DECREF(cond_branch);
      return false_branch;
     }
    }
    Dee_DECREF(cond_branch);
    if ((true_branch = _deeparse_top(
     self,scope,(Dee_uint32_t)(is_true ? flags : flags|DEE_PARSER_FLAG_UNREACHABLE)
     )) == NULL) goto err_condb;
    if (token.tk_id == ':') {
     if (!yield() || (false_branch = _deeparse_top(
      self,scope,(Dee_uint32_t)(is_true ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)
      )) == NULL) goto err_trueb;
    } else {
     false_branch = NULL;
    }
    Dee_DECREF(tok);
    if (is_true) {
     Dee_XDECREF(false_branch);
    } else {
     if (false_branch) { Dee_DECREF(true_branch); return false_branch; }
     Dee_XCLEAR(true_branch->ob_op_a);
     Dee_XCLEAR(true_branch->ob_op_b);
     Dee_XCLEAR(true_branch->ob_op_c);
     Dee_XCLEAR(true_branch->ob_op_d);
     true_branch->ob_kind = DeeExprAstKind_CONSTANT;
     Dee_INCREF(true_branch->ob_const = Dee_None);
    }
    return true_branch;
   }
  }
  if (token.tk_id == ':') {
   // GCC extension: in '?:' reuse the condition branch as true value
   Dee_INCREF(true_branch = cond_branch);
prs_fbranch: // Parse c branch
   if (!yield() || (false_branch = _deeparse_cond(self,scope,flags)) == NULL) {err_trueb: Dee_DECREF(true_branch); goto err_condb; }
  } else {
   if ((true_branch = _deeparse_top(self,scope,flags)) == NULL) goto err_condb;
   if (token.tk_id == ':') goto prs_fbranch; else false_branch = NULL;
  }
  if ((r = new_ast(DeeExprAstKind_COND,(DeeObject *)tok)) == NULL) {
   Dee_XDECREF(false_branch);
   goto err_trueb;
  }
  Dee_INHERIT_REF(r->ob_op_a,cond_branch);
  Dee_INHERIT_REF(r->ob_op_b,true_branch);
  Dee_INHERIT_REF(r->ob_op_c,false_branch);
  Dee_INHERIT_REF(cond_branch,r); // reuse the new ast in more conditional expressions
  Dee_DECREF(tok);
 } else goto end;
end: return cond_branch;
err_condb: Dee_DECREF(cond_branch);
err_tok: Dee_DECREF(tok);
 return NULL;
}

static DeeExprAstObject *
_deeparse_assign(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *a = _deeparse_cond(self,scope,flags),*b,*r;
 DeeExprAstKind kind;
 if (a) {
  while (1) switch (token.tk_id) {
   case TPP_TOK_COLLON_ASSIGN:  kind = DeeExprAstKind_ASSIGN; goto def_ass;
   case TPP_TOK_ADD_ASSIGN:     kind = DeeExprAstKind_IADD; goto def_ass;
   case TPP_TOK_SUB_ASSIGN:     kind = DeeExprAstKind_ISUB; goto def_ass;
   case TPP_TOK_MUL_ASSIGN:     kind = DeeExprAstKind_IMUL; goto def_ass;
   case TPP_TOK_DIV_ASSIGN:     kind = DeeExprAstKind_IDIV; goto def_ass;
   case TPP_TOK_MOD_ASSIGN:     kind = DeeExprAstKind_IMOD; goto def_ass;
   case TPP_TOK_SHL_ASSIGN:     kind = DeeExprAstKind_ISHL; goto def_ass;
   case TPP_TOK_SHR_ASSIGN:     kind = DeeExprAstKind_ISHR; goto def_ass;
   case TPP_TOK_BIN_AND_ASSIGN: kind = DeeExprAstKind_IAND; goto def_tycheck;
   case TPP_TOK_BIN_OR_ASSIGN:  kind = DeeExprAstKind_IOR;  goto def_ass;
   case TPP_TOK_BIN_XOR_ASSIGN: kind = DeeExprAstKind_IXOR; goto def_ass;
   case TPP_TOK_POW_ASSIGN:
    kind = DeeExprAstKind_IPOW;
def_tycheck:
    if ((flags&DEE_PARSER_FLAG_NO_TYPE_SUFFIX)==0 ||
        a->ob_kind != DeeExprAstKind_CONSTANT ||
       (!DeeNone_Check(a->ob_const) && !DeeType_Check(a->ob_const))) {
def_ass:
     if (!yield() || (b = _deeparse_assign(self,scope,flags)) == NULL) {err_a: Dee_DECREF(a); return NULL; }
     if (a->ob_kind == DeeExprAstKind_CONSTANT &&
         b->ob_kind == DeeExprAstKind_CONSTANT &&
        (flags & DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
         DEE_HAVE_CONSTOP_OPTIMIZATION(a->ob_const)) {
      DeeObject *lhs,*rhs,*cresult;
      if ((lhs = DeeObject_DeepCopy(a->ob_const)) == NULL) goto err_b;
      if ((rhs = DeeObject_DeepCopy(b->ob_const)) == NULL) { Dee_DECREF(lhs); goto err_b;}
      // Optimize constant assign operation
      switch (kind) {
       case DeeExprAstKind_ASSIGN:
        if (DeeObject_Assign(lhs,rhs) == -1) cresult = NULL;
        else Dee_INCREF(cresult = lhs);
        break;
       case DeeExprAstKind_IADD:   cresult = DeeObject_InplaceAdd(lhs,rhs); break;
       case DeeExprAstKind_ISUB:   cresult = DeeObject_InplaceSub(lhs,rhs); break;
       case DeeExprAstKind_IMUL:   cresult = DeeObject_InplaceMul(lhs,rhs); break;
       case DeeExprAstKind_IDIV:   cresult = DeeObject_InplaceDiv(lhs,rhs); break;
       case DeeExprAstKind_IMOD:   cresult = DeeObject_InplaceMod(lhs,rhs); break;
       case DeeExprAstKind_ISHL:   cresult = DeeObject_InplaceShl(lhs,rhs); break;
       case DeeExprAstKind_ISHR:   cresult = DeeObject_InplaceShr(lhs,rhs); break;
       case DeeExprAstKind_IAND:   cresult = DeeObject_InplaceAnd(lhs,rhs); break;
       case DeeExprAstKind_IOR:    cresult = DeeObject_InplaceOr(lhs,rhs); break;
       case DeeExprAstKind_IXOR:   cresult = DeeObject_InplaceXor(lhs,rhs); break;
       case DeeExprAstKind_IPOW:   cresult = DeeObject_InplacePow(lhs,rhs); break;
       default: Dee_DECREF(rhs); Dee_DECREF(lhs); goto def_bin; // Shouldn't happen
      }
      Dee_DECREF(rhs);
      Dee_DECREF(lhs);
      if (!cresult) {
       int error_code; char const *op_symb;
       switch (kind) {
        case DeeExprAstKind_ASSIGN: op_symb = ":="; error_code = DEE_WARNING_INVALID_CONSTOP_ASSIGN; break;
        case DeeExprAstKind_IADD:   op_symb = "+="; error_code = DEE_WARNING_INVALID_CONSTOP_IADD; break;
        case DeeExprAstKind_ISUB:   op_symb = "-="; error_code = DEE_WARNING_INVALID_CONSTOP_ISUB; break;
        case DeeExprAstKind_IMUL:   op_symb = "*="; error_code = DEE_WARNING_INVALID_CONSTOP_IMUL; break;
        case DeeExprAstKind_IDIV:   op_symb = "/="; error_code = DEE_WARNING_INVALID_CONSTOP_IDIV; break;
        case DeeExprAstKind_IMOD:   op_symb = "%="; error_code = DEE_WARNING_INVALID_CONSTOP_IMOD; break;
        case DeeExprAstKind_ISHL:   op_symb = "<<="; error_code = DEE_WARNING_INVALID_CONSTOP_ISHL; break;
        case DeeExprAstKind_ISHR:   op_symb = ">>="; error_code = DEE_WARNING_INVALID_CONSTOP_ISHR; break;
        case DeeExprAstKind_IAND:   op_symb = "&="; error_code = DEE_WARNING_INVALID_CONSTOP_IAND; break;
        case DeeExprAstKind_IOR:    op_symb = "|="; error_code = DEE_WARNING_INVALID_CONSTOP_IOR; break;
        case DeeExprAstKind_IXOR:   op_symb = "^="; error_code = DEE_WARNING_INVALID_CONSTOP_IXOR; break;
        case DeeExprAstKind_IPOW:   op_symb = "**="; error_code = DEE_WARNING_INVALID_CONSTOP_IPOW; break;
        default: DEE_BUILTIN_UNREACHABLE();
       }
       if (DeeError_CompilerErrorf(error_code,(DeeObject *)self,(DeeObject *)a->ob_token,
        "Invalid constant operator: Calling '(%#q) %s (%#q)' will throw an '%K' at runtime",
        DeeType_NAME(Dee_TYPE(a->ob_const)),op_symb,DeeType_NAME(Dee_TYPE(b->ob_const)),
        _DeeError_HandleReturnType()) == -1) goto err_b;
       goto def_bin;
      }
      Dee_DECREF(b); // no longer needed
      Dee_DECREF(a->ob_const);
      Dee_INHERIT_REF(a->ob_const,cresult);
     } else {
def_bin:
      if ((r = new_ast(kind,token_ob)) == NULL) {err_b: Dee_DECREF(b); goto err_a; }
      Dee_INHERIT_REF(r->ob_op_a,a);
      Dee_INHERIT_REF(r->ob_op_b,b);
      Dee_INHERIT_REF(a,r); // Reuse ast as lhs operand
     }
    } else goto end;
    break;
   default: goto end;
  }
end:
  if (_optimize_ast(a) == -1) goto err_a;
 }
 return a;
}


//////////////////////////////////////////////////////////////////////////
// Generate an assign ast from 'lhs' and 'rhs'
// NOTE: May modify / reuse 'lhs' to save on memory
static DeeExprAstObject *_dee_make_assign_ast(
 DeeLexerObject *lexer, DeeScopeObject *scope,
 DeeTokenObject *assign_token, DeeExprAstObject *lhs,
 DeeExprAstObject *rhs, Dee_uint32_t flags) {
 DeeExprAstObject *result;
 switch (lhs->ob_kind) {
  // Merge an assign expression with a tuple
  // >> a,b,c = d; >> (a,b,(c = d));
  // >> a,b,c = d...; >> ((a,b,c) = d...);
  // NOTE: May modify 'tuple_ast' and it's members asts
  case DeeExprAstKind_TUPLE:
   if (rhs->ob_kind == DeeExprAstKind_EXPAND) {
    // Transform all ending variables from 'lhs' into an expand-assign expression
    DeeExprAstObject *rhs_assign,**lhs_begin,**lhs_vars_begin,**lhs_end; int temp;
    lhs_vars_begin = lhs_end = (lhs_begin = (
     DeeExprAstObject **)DeeTuple_ELEM(lhs->ob_args_a))+DeeTuple_SIZE(lhs->ob_args_a);
    while (lhs_vars_begin != lhs_begin && lhs_vars_begin[-1]->ob_kind == DeeExprAstKind_VARIABLE) --lhs_vars_begin;
#if 0
    if (rhs->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT) {
     DeeObject *rhs_iterator,*rhs_copy,*rhs_elem;
     // TODO: Optimization if 'rhs->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT'
     if ((rhs_copy = DeeObject_DeepCopy(rhs->ob_op_a->ob_const)) == NULL) return NULL;
     rhs_iterator = DeeObject_IterSelf(rhs_copy);
     Dee_DECREF(rhs_copy);
     if (!rhs_iterator) return NULL;

     Dee_INCREF(lhs);
     return lhs;
    }
#endif
    if ((rhs_assign = (DeeExprAstObject *)new_ast(DeeExprAstKind_VARDECL,(DeeObject *)assign_token)) == NULL) return NULL;
    if ((rhs_assign->ob_op_a = (DeeExprAstObject *)new_ast(DeeExprAstKind_TUPLE,(DeeObject *)assign_token)) == NULL) {
err_rhs_assign: Dee_DECREF(rhs_assign); return NULL;
    }
    if ((rhs_assign->ob_op_a->ob_args_a = (DeeTupleObject *)DeeTuple_NewFromVector((
     Dee_size_t)(lhs_end-lhs_vars_begin),(DeeObject *const *)lhs_vars_begin)) == NULL) goto err_rhs_assign;
    {DeeExprAstObject **iter,**end,*elem;
     end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(rhs_assign->ob_op_a->ob_args_a))+
      DeeTuple_SIZE(rhs_assign->ob_op_a->ob_args_a);
     while (iter != end) {
      elem = *iter++;
      DEE_ASSERT(DeeObject_Check(elem) && DeeExprAst_Check(elem));
      if (elem->ob_kind == DeeExprAstKind_VARIABLE) {
       DEE_ASSERT(DeeObject_Check(elem->ob_var) && DeeLocalVar_Check(elem->ob_var));
       DEE_ASSERT(elem->ob_var->ob_uses);
       --elem->ob_var->ob_uses;
       elem->ob_kind = DeeExprAstKind_DECLVARIABLE;
      }
     }
    }
    Dee_INCREF(rhs_assign->ob_op_b = rhs);
    // Expand the variable expression to fit it into the a tuple
    if ((result = (DeeExprAstObject *)new_ast(DeeExprAstKind_EXPAND,(DeeObject *)assign_token)) == NULL) goto err_rhs_assign;
    Dee_INHERIT_REF(result->ob_op_a,rhs_assign);
    Dee_INHERIT_REF(rhs_assign,result);
    if (lhs_begin == lhs_vars_begin) return rhs_assign; // simple case: don't need to predent non-assign variables
    // Must prepend a tuple of expression from 'lhs_begin' to 'lhs_vars_begin'
    if ((result = (DeeExprAstObject *)new_ast(DeeExprAstKind_TUPLE,(DeeObject *)assign_token)) == NULL) goto err_rhs_assign;
    if ((result->ob_args_a = (DeeTupleObject *)DeeTuple_NewFromVector((
     Dee_size_t)(lhs_vars_begin-lhs_begin),(DeeObject *const *)lhs_begin)) == NULL) {
     Dee_DECREF(result); goto err_rhs_assign;
    }
    temp = DeeTuple_Append((DeeObject **)&result->ob_args_a,(DeeObject *)rhs_assign);
    Dee_DECREF(rhs_assign);
    if (temp == -1) Dee_CLEAR(result);
    return result;
   } else {
    // Transform the last element of 'tuple_ast' into a vardecl / assign with 'assign_ast'
    DeeExprAstObject *new_last,*last = (DeeExprAstObject *)
     DeeTuple_GET(lhs->ob_args_a,DeeTuple_SIZE(lhs->ob_args_a)-1);
    new_last = _dee_make_assign_ast(lexer,scope,assign_token,last,rhs,flags);
    if (!new_last) return NULL;
    Dee_DECREF(last);
    DeeTuple_SET(lhs->ob_args_a,DeeTuple_SIZE(lhs->ob_args_a)-1,(DeeObject *)new_last); // inherit reference
    goto ret_rhs;
   }
   break;

  case DeeExprAstKind_DECLVARIABLE:
   lhs->ob_kind = DeeExprAstKind_VARIABLE;
   DEE_ASSERT(DeeObject_Check(lhs->ob_var) && DeeLocalVar_Check(lhs->ob_var));
   ++lhs->ob_var->ob_uses;
   // fallthrough
  case DeeExprAstKind_VARIABLE:
   if ((lhs->ob_var->ob_flags&(DEE_LOCAL_VAR_FLAG_STATIC_INITIALIZED|
    DEE_LOCAL_VAR_FLAG_STATIC)) == DEE_LOCAL_VAR_FLAG_STATIC) {
    // Static var initializer decl (must make sure rhs is constant)
    if (rhs->ob_kind != DeeExprAstKind_CONSTANT) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_FOR_STATIC_VAR_INITIALIZER,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Expected constant expression for static var initializer") == -1) return NULL;
     if ((rhs = new_ast(DeeExprAstKind_CONSTANT,(DeeObject *)rhs->ob_token)) == NULL) return NULL;
     Dee_INCREF(rhs->ob_const = Dee_None);
    } else Dee_INCREF(rhs);
    if (!DeeScope_SameWeakScope((DeeObject *)lhs->ob_var->ob_scope,(DeeObject *)scope)) {
     if (DeeError_CompilerError(DEE_WARNING_CANT_INITIALIZE_STATIC_VAR_FROM_DIFFERENT_SCOPE,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Can't initialize static variable from different strong scope") == -1) return NULL;
     return rhs; // Inherit reference
    } else {
     if ((result = (DeeExprAstObject *)new_ast(DeeExprAstKind_VARDECL,(DeeObject *)assign_token)) == NULL) return NULL;
     lhs->ob_kind = DeeExprAstKind_DECLVARIABLE;
     DEE_ASSERT(lhs->ob_var->ob_uses);
     --lhs->ob_var->ob_uses; // Transform this variable into a decl var
     lhs->ob_var->ob_flags |= DEE_LOCAL_VAR_FLAG_STATIC_INITIALIZED;
     Dee_INCREF(result->ob_op_a = lhs);
     Dee_INHERIT_REF(result->ob_op_b,rhs);
    }
    return result;
   } else if ((lhs->ob_var->ob_flags&DEE_LOCALVAR_FLAG_TYPEBOUND)!=0) {
    // Type-bound variable (generate assign operator)
    if ((result = (DeeExprAstObject *)new_ast(DeeExprAstKind_ASSIGN,(DeeObject *)assign_token)) == NULL) return NULL;
    goto fill_res;
   } else {
    DeeTypeObject *rhs_type = DeeExprAst_PredictType((DeeObject *)rhs,(DeeObject *)scope);
    //DEE_ASSERT(lhs->ob_var->ob_uses);
    if (!rhs_type || (DeeType_FLAGS(rhs_type)&DEE_TYPE_FLAG_NO_LIFETIME_EFFECT)==0)
     ++lhs->ob_var->ob_uses; // Don't kill this variable; instance of this must be kept alive
    // Store in local
    result = (DeeExprAstObject *)new_ast(DeeExprAstKind_VARDECL,(DeeObject *)assign_token);
    if (!result) return NULL;
    goto fill_res;
   }

  case DeeExprAstKind_SEQ_GET:
   DEE_ASSERT(!lhs->ob_op_c);
   lhs->ob_kind = DeeExprAstKind_SEQ_SET;
   Dee_INCREF(lhs->ob_op_c = rhs);
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
       lhs->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
       lhs->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT &&
       lhs->ob_op_c->ob_kind == DeeExprAstKind_CONSTANT &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs->ob_op_a->ob_const)) {
    DeeObject *seq,*item,*value; int op_res;
    // We still have to call the operator to make sure it doesn't throw an exception
    if ((seq = DeeObject_DeepCopy(lhs->ob_op_a->ob_const)) == NULL) return NULL;
    if ((item = DeeObject_DeepCopy(lhs->ob_op_b->ob_const)) == NULL) {err_sq_s: Dee_DECREF(seq); return NULL;}
    if ((value = DeeObject_DeepCopy(lhs->ob_op_c->ob_const)) == NULL) { Dee_DECREF(value); goto err_sq_s;}
    op_res = DeeObject_SetItem(seq,item,value);
    Dee_DECREF(value);
    Dee_DECREF(item);
    Dee_DECREF(seq);
    if (op_res == 0) {
     // Collapse constant value
     Dee_DECREF(lhs->ob_op_a);
     Dee_DECREF(lhs->ob_op_b);
     Dee_INCREF(lhs->ob_const = lhs->ob_op_c->ob_const);
     Dee_DECREF(lhs->ob_op_c);
     lhs->ob_kind = DeeExprAstKind_CONSTANT;
    } else {
     // Warn about runtime error
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_SEQSET,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Invalid constant operator: Calling '%#q[%#q] = %#q' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs->ob_op_a->ob_const)),
      DeeType_NAME(Dee_TYPE(lhs->ob_op_b->ob_const)),
      DeeType_NAME(Dee_TYPE(lhs->ob_op_c->ob_const)),
      _DeeError_HandleReturnType()) == -1) return NULL;
    }
   }
   goto ret_rhs;

  case DeeExprAstKind_ATTR_GET:
   DEE_ASSERT(!lhs->ob_op_c);
   lhs->ob_kind = DeeExprAstKind_ATTR_SET;
   Dee_INCREF(lhs->ob_op_c = rhs);
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
       lhs->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
       lhs->ob_op_c->ob_kind == DeeExprAstKind_CONSTANT &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs->ob_op_a->ob_const)) {
    DeeObject *ob,*value; int op_res;
    // We still have to call the operator to make sure it doesn't throw an exception
    if ((ob = DeeObject_DeepCopy(lhs->ob_op_a->ob_const)) == NULL) return NULL;
    if ((value = DeeObject_DeepCopy(lhs->ob_op_c->ob_const)) == NULL) { Dee_DECREF(ob); return NULL; }
    op_res = DeeObject_SetAttr(ob,(DeeObject *)lhs->ob_member,value);
    Dee_DECREF(value);
    Dee_DECREF(ob);
    if (op_res == 0) {
     // Collapse constant value
     Dee_DECREF(lhs->ob_op_a);
     Dee_DECREF(lhs->ob_member);
     Dee_INCREF(lhs->ob_const = lhs->ob_op_c->ob_const);
     Dee_DECREF(lhs->ob_op_c);
     lhs->ob_kind = DeeExprAstKind_CONSTANT;
    } else {
     // Warn about runtime error
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_ATTRSET,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Invalid constant operator: Calling '%#q.%#q = %#q' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs->ob_op_a->ob_const)),DeeString_STR(lhs->ob_member),
      DeeType_NAME(Dee_TYPE(lhs->ob_op_c->ob_const)),
      _DeeError_HandleReturnType()) == -1) return NULL;
    }
   }
   goto ret_rhs;

  case DeeExprAstKind_SEQ_RANGE_GET:
   DEE_ASSERT(!lhs->ob_op_d);
   lhs->ob_kind = DeeExprAstKind_SEQ_RANGE_SET;
   Dee_INCREF(lhs->ob_op_d = rhs);
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
       lhs->ob_op_a->ob_kind == DeeExprAstKind_CONSTANT &&
       (!lhs->ob_op_b || lhs->ob_op_b->ob_kind == DeeExprAstKind_CONSTANT) &&
       (!lhs->ob_op_c || lhs->ob_op_c->ob_kind == DeeExprAstKind_CONSTANT) &&
       lhs->ob_op_d->ob_kind == DeeExprAstKind_CONSTANT &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs->ob_op_a->ob_const)) {
    DeeObject *seq,*lo,*hi,*value; int op_res;
    // We still have to call the operator to make sure it doesn't throw an exception
    if ((seq = DeeObject_DeepCopy(lhs->ob_op_a->ob_const)) == NULL) return NULL;
    if (lhs->ob_op_b) {
     if ((lo = DeeObject_DeepCopy(lhs->ob_op_b->ob_const)) == NULL) {err_sqr_s: Dee_DECREF(seq); return NULL; }
    } else lo = DeeNone_New();
    if (lhs->ob_op_c) {
     if ((hi = DeeObject_DeepCopy(lhs->ob_op_c->ob_const)) == NULL) {err_sqr_slo: Dee_DECREF(lo); goto err_sqr_s; }
    } else hi = DeeNone_New();
    if ((value = DeeObject_DeepCopy(lhs->ob_op_d->ob_const)) == NULL) { Dee_DECREF(hi); goto err_sqr_slo; }
    op_res = DeeObject_SetRangeItem(seq,lo,hi,value);
    Dee_DECREF(value);
    Dee_DECREF(hi);
    Dee_DECREF(lo);
    Dee_DECREF(seq);
    if (op_res == 0) {
     // Collapse constant value
     Dee_DECREF(lhs->ob_op_a);
     Dee_DECREF(lhs->ob_op_b);
     Dee_DECREF(lhs->ob_op_c);
     Dee_INCREF(lhs->ob_const = lhs->ob_op_d->ob_const);
     Dee_DECREF(lhs->ob_op_d);
     lhs->ob_kind = DeeExprAstKind_CONSTANT;
    } else {
     // Warn about runtime error
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_SEQRANGESET,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Invalid constant operator: Calling '%#q[%#q:%#q] = %#q' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs->ob_op_a->ob_const)),
      DeeType_NAME(lhs->ob_op_b ? Dee_TYPE(lhs->ob_op_b->ob_const) : (DeeTypeObject *)&DeeNone_Type),
      DeeType_NAME(lhs->ob_op_c ? Dee_TYPE(lhs->ob_op_c->ob_const) : (DeeTypeObject *)&DeeNone_Type),
      DeeType_NAME(Dee_TYPE(lhs->ob_op_d->ob_const)),
      _DeeError_HandleReturnType()) == -1) return NULL;
    }
   }
ret_rhs:
   Dee_INCREF(lhs);
   return lhs;
   //break;

  default:
   if (DeeError_CompilerErrorf(DEE_WARNING_STORE_IS_ASSIGN,(DeeObject *)lexer,(DeeObject *)assign_token,
    "Store operation will be compiled as assign (compiling as '%r := %r')",lhs,rhs) == -1) return NULL;
   if ((flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
       lhs->ob_kind == DeeExprAstKind_CONSTANT &&
       rhs->ob_kind == DeeExprAstKind_CONSTANT &&
       DEE_HAVE_CONSTOP_OPTIMIZATION(lhs->ob_const)) {
    DeeObject *lhs_cpy,*rhs_cpy; int op_res;
    // We still have to call the operator to make sure it doesn't throw an exception
    if ((lhs_cpy = DeeObject_DeepCopy(lhs->ob_const)) == NULL) return NULL;
    if ((rhs_cpy = DeeObject_DeepCopy(rhs->ob_const)) == NULL) { Dee_DECREF(lhs_cpy); return NULL; }
    op_res = DeeObject_Assign(lhs_cpy,rhs_cpy);
    Dee_DECREF(rhs_cpy);
    Dee_DECREF(lhs_cpy);
    if (op_res == 0) {
     goto ret_rhs;
    } else {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_ASSIGN,(DeeObject *)lexer,(DeeObject *)assign_token,
      "Invalid constant operator: Calling '%#q = %#q' will throw an '%K' at runtime",
      DeeType_NAME(Dee_TYPE(lhs->ob_const)),DeeType_NAME(Dee_TYPE(rhs->ob_const)),
      _DeeError_HandleReturnType()) == -1) return NULL;
     // fallthrough
    }
   }
   if ((result = (DeeExprAstObject *)new_ast(DeeExprAstKind_ASSIGN,(DeeObject *)assign_token)) == NULL) return NULL;
fill_res:
   Dee_INCREF(result->ob_op_a = lhs);
   Dee_INCREF(result->ob_op_b = rhs);
   break;
 }
 return result;
}





DEE_STATIC_INLINE(int) _deeoptimize_constant_tuple_ast(DeeExprAstObject *ast);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_comma(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTokenObject *assign_token;
 DeeExprAstObject *lhs_branch,*rhs_branch,*result;
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((lhs_branch = _deeparse_assign(self,scope,flags)) == NULL) return NULL;
 while (1) switch (token.tk_id) {
  case '=': {
   Dee_INCREF(assign_token = (DeeTokenObject *)token_ob);
   if (!yield() || (rhs_branch = _deeparse_assign(self,scope,flags)) == NULL) {
    Dee_DECREF(assign_token);
    goto err_a;
   }
   result = _dee_make_assign_ast(self,scope,assign_token,lhs_branch,rhs_branch,flags);
   Dee_DECREF(assign_token);
   Dee_DECREF(lhs_branch);
   Dee_DECREF(rhs_branch);
   if (!result) return NULL;
   Dee_INHERIT_REF(lhs_branch,result);
  } break;
  case ',': {
   int is_const,temp;
   if (!yield()) {err_a: Dee_DECREF(lhs_branch); return NULL; }
   if ((result = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,DeeLexer_TOKEN(self))) == NULL) goto err_a;
   result->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,lhs_branch);
   Dee_DECREF(lhs_branch);
   if (!result->ob_args_a) goto err_r;
   is_const = lhs_branch->ob_kind == DeeExprAstKind_CONSTANT;
   while (1) {
    if ((lhs_branch = _deeparse_assign((DeeLexerObject *)self,(DeeScopeObject *)scope,flags)) == NULL) goto err_r;
    if (is_const) is_const = lhs_branch->ob_kind == DeeExprAstKind_CONSTANT;
    if (result->ob_kind != DeeExprAstKind_TUPLE) {
     DeeExprAstObject *expand_tuple_ast; // Make sure we're still building a tuple here...
     DEE_ASSERT(result->ob_kind == DeeExprAstKind_EXPAND);
     if ((expand_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
      DeeExprAstKind_TUPLE,(DeeObject *)result->ob_token)) == NULL) goto err_r;
     expand_tuple_ast->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,result);
     Dee_DECREF(result);
     if (!expand_tuple_ast->ob_args_a) goto err_r;
     Dee_INHERIT_REF(result,expand_tuple_ast);
    }
    temp = DeeTuple_Append((DeeObject **)&result->ob_args_a,(DeeObject *)lhs_branch);
    Dee_DECREF(lhs_branch);
    if (temp == -1) goto err_r;
    while (token.tk_id == '=') {
     DeeExprAstObject *assign_branch;
     is_const = 0;
     Dee_INCREF(assign_token = (DeeTokenObject *)token_ob);
     if (!yield() || (rhs_branch = _deeparse_assign(self,scope,flags)) == NULL) {
      Dee_DECREF(assign_token); goto err_r;
     }
     assign_branch = _dee_make_assign_ast(self,scope,assign_token,result,rhs_branch,flags);
     Dee_DECREF(assign_token);
     Dee_DECREF(rhs_branch);
     Dee_DECREF(result);
     if (!assign_branch) return NULL;
     Dee_INHERIT_REF(result,assign_branch);
    }
    if (token.tk_id != ',') break;
    if (!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')' || token.tk_id == ']' ||
        token.tk_id == ';' || token.tk_id == '}' ||
        token.tk_id == '>') break;
#endif
   }
   if (is_const && (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0) {
    if (_deeoptimize_constant_tuple_ast(result) == -1) goto err_r;
   }
   return result;
  }
  default: return lhs_branch;
 }
err_r:
 Dee_DECREF(result);
 return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Lexer: Parse expression
// >> Returns NULL on error
DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpression(
 DEE_A_IN_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) {
 DeeExprAstObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 if (++((DeeLexerObject *)self)->ob_syntax_recursion ==
       ((DeeLexerObject *)self)->ob_max_syntax_recursion) {
  --((DeeLexerObject *)self)->ob_syntax_recursion;
  if (DeeError_CompilerError(DEE_WARNING_TOO_DEEP_SYNTAX_RECURSION,
   self,DeeLexer_TOKEN(self),"Too deep syntax recursion"
   ) == -1) return NULL;
 }
 result = _deeparse_assign((DeeLexerObject *)self,(DeeScopeObject *)scope,flags);
 --((DeeLexerObject *)self)->ob_syntax_recursion;
 if (result && (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
     _deeexprast_optimize_get_operators((DeeLexerObject *)self,result) == -1
     ) Dee_CLEAR(result);
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionLv2(
 DEE_A_IN_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 return (DeeObject *)_deeparse_top_lv2((DeeLexerObject *)self,(DeeScopeObject *)scope,flags);
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeExprAstObject *)
_deeparse_comma_tuple(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeTokenObject *assign_token;
 DeeExprAstObject *lhs_branch,*rhs_branch,*result;
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((lhs_branch = _deeparse_assign(self,scope,flags)) == NULL) return NULL;
 while (1) switch (token.tk_id) {
  case '=': {
   Dee_INCREF(assign_token = (DeeTokenObject *)token_ob);
   if (!yield() || (rhs_branch = _deeparse_assign(self,scope,flags)) == NULL) {
    Dee_DECREF(assign_token);
    goto err_lhs;
   }
   result = _dee_make_assign_ast(self,scope,assign_token,lhs_branch,rhs_branch,flags);
   Dee_DECREF(assign_token);
   Dee_DECREF(lhs_branch);
   Dee_DECREF(rhs_branch);
   if (!result) return NULL;
   Dee_INHERIT_REF(lhs_branch,result);
  } break;
  case ',': {
   int is_const,temp;
   if (!yield()) {err_lhs: Dee_DECREF(lhs_branch); return NULL; }
   if ((result = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,DeeLexer_TOKEN(self))) == NULL) goto err_lhs;
   result->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,lhs_branch);
   Dee_DECREF(lhs_branch);
   if (!result->ob_args_a) goto err_r;
   is_const = lhs_branch->ob_kind == DeeExprAstKind_CONSTANT;
   while (1) {
    if ((lhs_branch = _deeparse_assign((DeeLexerObject *)self,(DeeScopeObject *)scope,flags)) == NULL) goto err_r;
    if (is_const) is_const = lhs_branch->ob_kind == DeeExprAstKind_CONSTANT;
    if (result->ob_kind != DeeExprAstKind_TUPLE) {
     DeeExprAstObject *expand_tuple_ast; // Make sure we're still building a tuple here...
     DEE_ASSERT(result->ob_kind == DeeExprAstKind_EXPAND);
     if ((expand_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
      DeeExprAstKind_TUPLE,(DeeObject *)result->ob_token)) == NULL) goto err_r;
     expand_tuple_ast->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,result);
     Dee_DECREF(result);
     if (!expand_tuple_ast->ob_args_a) goto err_r;
     Dee_INHERIT_REF(result,expand_tuple_ast);
    }
    temp = DeeTuple_Append((DeeObject **)&result->ob_args_a,(DeeObject *)lhs_branch);
    Dee_DECREF(lhs_branch);
    if (temp == -1) goto err_r;
    while (token.tk_id == '=') {
     DeeExprAstObject *assign_branch;
     is_const = 0;
     Dee_INCREF(assign_token = (DeeTokenObject *)token_ob);
     if (!yield() || (rhs_branch = _deeparse_assign(self,scope,flags)) == NULL) {
      Dee_DECREF(assign_token); goto err_r;
     }
     assign_branch = _dee_make_assign_ast(self,scope,assign_token,result,rhs_branch,flags);
     Dee_DECREF(assign_token);
     Dee_DECREF(rhs_branch);
     Dee_DECREF(result);
     if (!assign_branch) return NULL;
     Dee_INHERIT_REF(result,assign_branch);
    }
    if (token.tk_id != ',') break;
    if (!yield()) goto err_r;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ')' || token.tk_id == ']' ||
        token.tk_id == ';' || token.tk_id == '}' ||
        token.tk_id == '>') break;
#endif
   }
   if (result->ob_kind != DeeExprAstKind_TUPLE) {
    Dee_INHERIT_REF(lhs_branch,result);
    goto make_tuple;
   }
   if (is_const && (flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0) {
    if (_deeoptimize_constant_tuple_ast(result) == -1) goto err_r;
   }
   return result;
  }
  default:
   if (lhs_branch->ob_kind == DeeExprAstKind_CONSTANT) {
    DeeObject *new_const;
    if ((new_const = DeeTuple_Pack(1,lhs_branch->ob_const)) == NULL) goto err_lhs;
    Dee_DECREF(lhs_branch->ob_const);
    Dee_INHERIT_REF(lhs_branch->ob_const,new_const);
   } else {
    DeeTupleObject *ast_tuple;
make_tuple:
    if ((result = new_ast(DeeExprAstKind_TUPLE,(
     DeeObject *)lhs_branch->ob_token)) == NULL) goto err_lhs;
    ast_tuple = (DeeTupleObject *)DeeTuple_Pack(1,lhs_branch);
    Dee_DECREF(lhs_branch);
    if (!ast_tuple) goto err_r;
    Dee_INHERIT_REF(result->ob_args_a,ast_tuple);
    return result;
   }
   return lhs_branch;
 }
err_r:
 Dee_DECREF(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionTuple(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 return (DeeObject *)_deeparse_comma_tuple((DeeLexerObject *)self,(DeeScopeObject *)scope,flags);
}

static DeeExprAstObject *_deeparse_decl(DeeLexerObject *self, DeeScopeObject *scope, Dee_uint32_t flags) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 switch (token.tk_id) {
  {
   DeeObject *is_deprecated;
   DeeLocalVarObject *localvar;
   DeeExprAstObject *result_tuple_ast,*var_ast,*init_expr,*assign_expr;
   // Portion of uninitialized variables (used for expand assignments)
   DEE_A_REF DeeTupleObject *uninitialized_portion;
#define VARDECL_MODE_DEFAULT     0x00 // Lookup underlying variables; create+init new in current scope
#define VARDECL_MODE_LOCAL       0x01 // create+init new in current scope
#define VARDECL_MODE_FORWARD     0x02 // create new in current scope (warn if initialized)
#define VARDECL_MODE_FLAG_STATIC 0x04 // create static variables
#define VARDECL_MODE(x) ((x)&0x03)
   int mode,temp;
  case KWD_static: // Static modifier
   mode = VARDECL_MODE_FLAG_STATIC;
   if ((result_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_r_vd_ast;
   if (token.tk_id == KWD_local) {
    mode = VARDECL_MODE_LOCAL|VARDECL_MODE_FLAG_STATIC;
    if (!yield()) goto err_r_vd_ast;
    if (token.tk_id == KWD_forward) {
     mode = VARDECL_MODE_FORWARD|VARDECL_MODE_FLAG_STATIC;
     if (!yield()) goto err_r_vd_ast;
    }
   } else if (token.tk_id == KWD_forward) {
    mode = VARDECL_MODE_FORWARD|VARDECL_MODE_FLAG_STATIC;
    if (!yield()) goto err_r_vd_ast;
   }
   goto begin_vardecl;

  case KWD_local: // Local variable declaration
   if ((result_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,token_ob)) == NULL) return NULL;
   if (!yield()) {err_r_vd_ast: Dee_DECREF(result_tuple_ast); return NULL; }
   if (token.tk_id == KWD_forward) goto forward_decl_y1;
   mode = VARDECL_MODE_LOCAL;
   goto begin_vardecl;

  case KWD_forward: // Forward variable declaration
   if ((result_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,token_ob)) == NULL) return NULL;
   if (!yield()) goto err_r_vd_ast;
   if (token.tk_id == KWD_local) {
forward_decl_y1: if (!yield()) goto err_r_vd_ast;
   }
   mode = VARDECL_MODE_FORWARD;
   goto begin_vardecl;
default_vardecl:
   if ((result_tuple_ast = (DeeExprAstObject *)DeeExprAst_New(
    DeeExprAstKind_TUPLE,token_ob)) == NULL) return NULL;
   mode = VARDECL_MODE_DEFAULT;
begin_vardecl:
   Dee_INCREF(result_tuple_ast->ob_args_a = (DeeTupleObject *)Dee_EmptyTuple);
   Dee_INCREF(uninitialized_portion = (DeeTupleObject *)Dee_EmptyTuple);
   while (1) {
    struct DeeAttributeDecl vardecl_attr = DeeAttributeDecl_INIT();
    if (_DeeLexer_ParseAttributeDecl(&vardecl_attr,(
     DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) {
err_vd_attr: DeeAttributeDecl_Quit(&vardecl_attr);
     goto err_vd_uninit;
    }
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     // Not a variable declaration
     if (VARDECL_MODE(mode) != VARDECL_MODE_DEFAULT) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_LOCAL,(DeeObject *)self,token_ob,
       "expected <keyword> after 'local' [<keyword> ',']...") == -1) goto err_vd_attr;
     }
     if (DeeTuple_AppendVector(
      (DeeObject **)&result_tuple_ast->ob_args_a,
      DeeTuple_SIZE(uninitialized_portion),
      DeeTuple_ELEM(uninitialized_portion)) == -1) goto err_vd_attr;
     Dee_DECREF(uninitialized_portion);
     Dee_INCREF(uninitialized_portion = (DeeTupleObject *)Dee_EmptyTuple);
     if ((init_expr = (DeeExprAstObject *)DeeLexer_ParseExpression(
      (DeeObject *)self,(DeeObject *)scope,flags)) == NULL) goto err_vd_attr;
     temp = DeeTuple_Append((DeeObject **)&result_tuple_ast->ob_args_a,(DeeObject *)init_expr);
     Dee_DECREF(init_expr);
     if (temp == -1) goto err_vd_attr;
     goto vardecl_next;
    } else if (VARDECL_MODE(mode) == VARDECL_MODE_DEFAULT) {
     DeeScopeObject *scope_iter = (DeeScopeObject *)scope;
     while (1) {
      if ((localvar = (DeeLocalVarObject *)_DeeScope_GetNameEx((
       DeeScopeObject *)scope_iter,token.tk_id,&is_deprecated)) != NULL) {
       if (is_deprecated && DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,
        (DeeObject *)self,token_ob,"Deprecated : %r",is_deprecated) == -1) goto err_vd_attr;
       goto vd_check_localvar;
      }
      if (scope_iter->ob_weak_prev) scope_iter = scope_iter->ob_weak_prev;
      else if (scope_iter->ob_prev) scope_iter = scope_iter->ob_prev;
      else break;
     }
     // Allocate a new variable in the local scope
     goto vd_new_local;
    } else if ((localvar = (DeeLocalVarObject *)_DeeScope_GetNameEx((
     DeeScopeObject *)scope,token.tk_id,&is_deprecated)) != NULL) {
     if (is_deprecated && DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,
      (DeeObject *)self,token_ob,"Deprecated : %r",is_deprecated) == -1) goto err_vd_attr;
vd_check_localvar:
     if (!DeeLocalVar_Check(localvar)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_VARIABLE_AFTER_LOCAL,(DeeObject *)self,token_ob,
       "Expected variable after '%s' (got: constant %q)",VARDECL_MODE(mode) == VARDECL_MODE_FORWARD
       ? "forward" : "local",DeeType_NAME(Dee_TYPE(localvar))) == -1) goto err_vd_attr;
      if (!yield()) goto err_vd_attr;
      if (token.tk_id == '=') {
       if (!yield()) goto err_vd_attr;
       if ((init_expr = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
        (DeeObject *)scope,flags|DEE_PARSER_FLAG_UNREACHABLE)) == NULL) goto err_vd_attr;
       Dee_DECREF(init_expr);
       Dee_DECREF(uninitialized_portion);
       Dee_INCREF(uninitialized_portion = (DeeTupleObject *)Dee_EmptyTuple);
      }
      goto vardecl_next;
     }
     if (VARDECL_MODE(mode) == VARDECL_MODE_FORWARD && (localvar->ob_flags&DEE_LOCAL_VAR_FLAG_AST_INITIALIZED)!=0) {
      if (DeeError_CompilerError(DEE_WARNING_FORWARD_VARIABLE_ALREADY_INITIALIZED,(DeeObject *)self,
       token_ob,"Forward declared variable is already initialized") == -1) goto err_vd_attr;
     }
     Dee_INCREF(localvar);
    } else {
vd_new_local:
     if ((localvar = (DeeLocalVarObject *)DeeLocalVar_New(token.tk_id,(DeeObject *)scope)) == NULL) goto err_vd_attr;
     if ((mode&VARDECL_MODE_FLAG_STATIC)!=0) localvar->ob_flags |= DEE_LOCAL_VAR_FLAG_STATIC;
     // TODO: VARDECL_MODE_FLAG_STATIC
     if (_DeeScope_AddDeprecatedName((DeeScopeObject *)scope,token.tk_id,
      (DeeObject *)localvar,(DeeObject *)vardecl_attr.ob_depr) == -1) {
err_vd_attr_var: Dee_DECREF(localvar); goto err_vd_attr;
     }
    }
    if ((var_ast = (DeeExprAstObject *)DeeExprAst_New(DeeExprAstKind_VARIABLE,token_ob)) == NULL) goto err_vd_attr_var;
    Dee_INHERIT_REF(var_ast->ob_var,localvar);
    ++var_ast->ob_var->ob_uses;
    if (!yield()) {
err_vd_var_ast: Dee_DECREF(var_ast); goto err_vd_attr;
    }
    if (token.tk_id == '=') { // init-expression
     DeeExprAstObject *assign_expand_expand;
     DeeTypeObject *init_expr_type;
     if (VARDECL_MODE(mode) == VARDECL_MODE_FORWARD && DeeError_CompilerError(DEE_WARNING_FORWARD_VARIABLE_DIRECTLY_INITIALIZED,
      (DeeObject *)self,token_ob,"Forward variable directly initialized") == -1) goto err_vd_var_ast;
     if (!DeeScope_SameWeakScope((DeeObject *)var_ast->ob_var->ob_scope,(DeeObject *)scope)) {
      if (DeeError_CompilerError(DEE_WARNING_CANT_STORE_VAR_FROM_DIFFERENT_SCOPE,
       (DeeObject *)self,token_ob,"Can't store value in variable from underlying scope (compiling as assign ':=')") == -1) goto err_vd_var_ast;
      if (DeeScope_ExternalReference((DeeObject *)scope,(DeeObject *)var_ast->ob_var) == -1) goto err_vd_var_ast;
       ++var_ast->ob_var->ob_uses;
      if ((assign_expr = (DeeExprAstObject *)DeeExprAst_New(DeeExprAstKind_ASSIGN,token_ob)) == NULL) goto err_vd_var_ast;
     } else {
      if ((assign_expr = (DeeExprAstObject *)DeeExprAst_New(DeeExprAstKind_VARDECL,token_ob)) == NULL) goto err_vd_var_ast;
     }
     if (!yield()) {err_vd_assign_expr: Dee_DECREF(assign_expr); goto err_vd_var_ast;}
     if ((init_expr = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
      (DeeObject *)scope,flags|DEE_PARSER_FLAG_UNREACHABLE)) == NULL) goto err_vd_assign_expr;
     assign_expr->ob_op_b = init_expr; // Inherit reference
     // Mark the variable of the var-ast as initialized
     var_ast->ob_var->ob_flags |= DEE_LOCAL_VAR_FLAG_AST_INITIALIZED;
     if ((var_ast->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_STATIC)!=0 &&
         assign_expr->ob_kind == DeeExprAstKind_VARDECL) {
      if (init_expr->ob_kind != DeeExprAstKind_CONSTANT) {
       if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_FOR_STATIC_VAR_INITIALIZER,(
        DeeObject *)self,(DeeObject *)init_expr->ob_token,
        "Expected constant expression for static var initializer (got: %r)",init_expr) == -1) return NULL;
       Dee_XDECREF(init_expr->ob_op_a);
       Dee_XCLEAR(init_expr->ob_op_b);
       Dee_XCLEAR(init_expr->ob_op_c);
       Dee_XCLEAR(init_expr->ob_op_d);
       Dee_INCREF(init_expr->ob_const = Dee_None);
       init_expr->ob_kind = DeeExprAstKind_CONSTANT;
      }
      DEE_ASSERT(DeeScope_SameWeakScope((DeeObject *)var_ast->ob_var->ob_scope,(DeeObject *)scope));
      var_ast->ob_var->ob_flags |= DEE_LOCAL_VAR_FLAG_STATIC_INITIALIZED;
     }
     if (init_expr->ob_kind != DeeExprAstKind_DEX) { // Don't keep dex variables alive!
      init_expr_type = DeeExprAst_PredictType((DeeObject *)init_expr,(DeeObject *)scope);
      if (!init_expr_type || (DeeType_FLAGS(init_expr_type)&DEE_TYPE_FLAG_NO_LIFETIME_EFFECT)==0)
       ++var_ast->ob_var->ob_uses; // Don't kill this variable; instance of this must be kept alive
     }
     if (init_expr->ob_kind == DeeExprAstKind_EXPAND) {
      DeeExprAstObject *expand_dst_ast,**init_iter,**init_end;
      // Add the initialized flags to all the variables.
      init_end = (init_iter = (DeeExprAstObject **)DeeTuple_ELEM(uninitialized_portion))+DeeTuple_SIZE(uninitialized_portion);
      while (init_iter != init_end) {
       // TODO: Optimization: If the value that will be assigned to this variable doesn't have any lifetime effects, don't add a use
       // TODO: Support for initializing statics?
       ++(*init_iter)->ob_var->ob_uses;
       (*init_iter)->ob_var->ob_flags |= DEE_LOCAL_VAR_FLAG_AST_INITIALIZED;
       ++init_iter;
      }
      // Expand into all currently uninitialized variables (including 'var_ast')
      if (DeeTuple_Append((DeeObject **)&uninitialized_portion,(DeeObject *)var_ast) == -1) goto err_vd_assign_expr;
      if ((expand_dst_ast = (DeeExprAstObject *)DeeExprAst_New(
       DeeExprAstKind_TUPLE,(DeeObject *)assign_expr->ob_token)) == NULL) goto err_vd_assign_expr;
      Dee_INHERIT_REF(expand_dst_ast->ob_args_a,uninitialized_portion);
      if (assign_expr->ob_kind == DeeExprAstKind_VARDECL) {
       DeeExprAstObject **iter,**end,*elem;
       end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(
        expand_dst_ast->ob_args_a))+DeeTuple_SIZE(expand_dst_ast->ob_args_a);
       while (iter != end) {
        elem = *iter++;
        DEE_ASSERT(DeeObject_Check(elem) && DeeExprAst_Check(elem));
        if (elem->ob_kind == DeeExprAstKind_VARIABLE) {
         DEE_ASSERT(DeeObject_Check(elem->ob_var) && DeeLocalVar_Check(elem->ob_var));
         DEE_ASSERT(elem->ob_var->ob_uses);
         --elem->ob_var->ob_uses;
         elem->ob_kind = DeeExprAstKind_DECLVARIABLE;
        }
       }
      }
      Dee_INHERIT_REF(assign_expr->ob_op_a,expand_dst_ast);
      Dee_DECREF(var_ast);
      if ((assign_expand_expand = (DeeExprAstObject *)DeeExprAst_New(
       DeeExprAstKind_EXPAND,(DeeObject *)assign_expr->ob_token)) == NULL) {
       Dee_DECREF(expand_dst_ast); goto err_vd_assign_expr;
      }
      Dee_INHERIT_REF(assign_expand_expand->ob_op_a,assign_expr);
      Dee_INHERIT_REF(assign_expr,assign_expand_expand);
     } else {
      if (DeeTuple_AppendVector(
       (DeeObject **)&result_tuple_ast->ob_args_a,
       DeeTuple_SIZE(uninitialized_portion),
       DeeTuple_ELEM(uninitialized_portion)) == -1) goto err_vd_assign_expr;
      Dee_INHERIT_REF(assign_expr->ob_op_a,var_ast);
      Dee_DECREF(uninitialized_portion);
     }
     Dee_INCREF(uninitialized_portion = (DeeTupleObject *)Dee_EmptyTuple);
     temp = DeeTuple_Append((DeeObject **)&result_tuple_ast->ob_args_a,(DeeObject *)assign_expr);
     Dee_DECREF(assign_expr);
     if (temp == -1) goto err_vd_attr;
    } else {
     if (DeeTuple_Append((DeeObject **)&uninitialized_portion,(DeeObject *)var_ast) == -1) goto err_vd_var_ast;
     Dee_DECREF(var_ast);
    }
vardecl_next:
    DeeAttributeDecl_Quit(&vardecl_attr);
    if (token.tk_id != ',') break;
    if (!yield()) goto err_vd_uninit;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (token.tk_id == ';') break;
#endif
   }
   if (DeeTuple_AppendVector(
    (DeeObject **)&result_tuple_ast->ob_args_a,
    DeeTuple_SIZE(uninitialized_portion),
    DeeTuple_ELEM(uninitialized_portion)) == -1) {
err_vd_uninit: Dee_DECREF(uninitialized_portion);
    goto err_r_vd_ast;
   }
   Dee_DECREF(uninitialized_portion);
   if (DeeTuple_SIZE(result_tuple_ast->ob_args_a) == 1) {
    DeeExprAstObject *result_ast;
    Dee_INCREF(result_ast = (DeeExprAstObject *)DeeTuple_GET(result_tuple_ast->ob_args_a,0));
    Dee_DECREF(result_tuple_ast);
    return result_ast;
   }
   return result_tuple_ast;
  } break;
#undef VARDECL_MODE_DEFAULT
#undef VARDECL_MODE_LOCAL
#undef VARDECL_MODE_FORWARD
#undef VARDECL_MODE_FLAG_STATIC
#undef VARDECL_MODE

  default: {
   DeeTokenObject *temp_token;
   DeeExprAstObject *expr,*result;
   Dee_INCREF(temp_token = (DeeTokenObject *)token_ob);
   if (!yield()) {err_temp_tok: Dee_DECREF(temp_token); return NULL; }
   if (token.tk_id == '=' || token.tk_id == ',') {
    if (DeeLexer_Return((DeeObject *)self,(DeeObject *)temp_token) == -1) goto err_temp_tok;
    Dee_DECREF(temp_token);
    goto default_vardecl; // Parse a variable declaration
   } else { // Return the original token
    if (DeeLexer_Return((DeeObject *)self,(DeeObject *)temp_token) == -1) goto err_temp_tok;
   }
   Dee_DECREF(temp_token);
   // Assume expression
   if ((expr = (DeeExprAstObject *)DeeLexer_ParseExpressionLv2(
    (DeeObject *)self,(DeeObject *)scope,flags/*|DEE_PARSER_FLAG_NO_TYPE_SUFFIX*/)) == NULL) return NULL;
   if (expr->ob_kind == DeeExprAstKind_CONSTANT &&
      (DeeNone_Check(expr->ob_const) || DeeType_Check(expr->ob_const)) && token.tk_id != ';') {
    DeeObject *typed_vardecl_tuple = NULL;
    DeeExprAstObject *ass_expr = NULL;
    if (DeeNone_Check(expr->ob_const)) {
     Dee_DECREF(expr->ob_const);
     Dee_INCREF(expr->ob_const = (DeeObject *)&DeeNone_Type);
    }
    while (1) {
     struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
     DeeTokenObject *name_token;
     DeeTypeObject *used_type;
     DeeExprAstObject *init_ast,*initializer;
     struct _DeeTokenIDListObject *func_argnames = NULL;
     if (ass_expr) {
      int temp;
      temp = DeeTuple_Append(&typed_vardecl_tuple,(DeeObject *)ass_expr);
      Dee_DECREF(ass_expr);
      if (temp == -1) {
err_typed_var_tpl_attr:
       DeeAttributeDecl_Quit(&attr);
       Dee_XDECREF(func_argnames);
       goto err_typed_var_tpl;
      }
     }
     if ((used_type = _DeeLexer_ParseNamedTypeWithBaseAttr(
      (DeeLexerObject *)self,(DeeScopeObject *)scope,flags,
      NULL, // TODO: Unsized arrays
      &name_token,&func_argnames,
      (DeeTypeObject *)expr->ob_const,&attr)) == NULL) {
      goto err_typed_var_tpl_attr;
     }
     if (name_token) {
      DeeLocalVarObject *lookup_var;
      lookup_var = (DeeLocalVarObject *)DeeScope_Lookup(
       (DeeObject *)scope,name_token->ob_token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL);
      if (!lookup_var) {
err_typed_ut_var_tpl:
       Dee_DECREF(used_type);
err_typed_var_tpl_attr_nametok:
       Dee_DECREF(name_token);
       goto err_typed_var_tpl_attr;
      }
      if (!DeeLocalVar_Check(lookup_var)) {
       if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_VARIABLE_AFTER_TYPE_KEYWORD,
        (DeeObject *)self,(DeeObject *)name_token,
        "Expected variable name for typed var decl (got: %s:%k)",
        DeeType_NAME(Dee_TYPE(lookup_var)),lookup_var) == -1) goto err_typed_ut_var_tpl;
       goto unnamed_var_decl;
      } else {
       // Type-bound variables
       if ((lookup_var->ob_flags&DEE_LOCALVAR_FLAG_TYPEBOUND)==0){
        lookup_var->ob_flags |= DEE_LOCALVAR_FLAG_TYPEBOUND;
        Dee_INCREF(lookup_var->ob_type = used_type);
       } else {
        DEE_ASSERT(lookup_var->ob_type);
        if (lookup_var->ob_type != used_type) {
         if (DeeError_CompilerErrorf(DEE_WARNING_VARIABLE_HAS_ALREADY_BEEN_BOUND_TO_DIFFERENT_TYPE,
          (DeeObject *)self,(DeeObject *)name_token,
          "Variable was already bound to a different type (old: %s; new: %s)",
          DeeType_NAME(lookup_var->ob_type),DeeType_NAME(used_type)) == -1) goto err_typed_ut_var_tpl;
         Dee_DECREF(used_type);
         Dee_INCREF(used_type = lookup_var->ob_type);
        }
       }
       if ((ass_expr = (DeeExprAstObject *)DeeExprAst_New(
        DeeExprAstKind_VARDECL,token_ob)) == NULL) goto err_typed_ut_var_tpl;
       if ((ass_expr->ob_op_a = (DeeExprAstObject *)DeeExprAst_New(
        DeeExprAstKind_DECLVARIABLE,(DeeObject *)name_token)) == NULL) {
err_typed_ut_ass_var_tpl: Dee_DECREF(ass_expr); goto err_typed_ut_var_tpl;
       }
       Dee_INCREF(ass_expr->ob_op_a->ob_var = lookup_var);
      }
      if ((init_ast = (DeeExprAstObject *)DeeExprAst_New(
       DeeExprAstKind_CALL,token_ob)) == NULL) goto err_typed_ut_ass_var_tpl;
      ass_expr->ob_op_b = init_ast; // inherit reference
     } else {
unnamed_var_decl:
      // Unnamed var decl
      // e.g.: 'int(*)(int);'
      if ((ass_expr = init_ast = (DeeExprAstObject *)DeeExprAst_New(
       DeeExprAstKind_CALL,token_ob)) == NULL) goto err_typed_ut_ass_var_tpl;
     }
     if ((init_ast->ob_op_a = (DeeExprAstObject *)DeeExprAst_New(
      DeeExprAstKind_CONSTANT,token_ob)) == NULL) goto err_typed_ut_ass_var_tpl;
     init_ast->ob_op_a->ob_const = (DeeObject *)used_type; // Setup the used type (inherit reference)
     if ((init_ast->ob_op_b = (DeeExprAstObject *)DeeExprAst_New(
      DeeExprAstKind_TUPLE,token_ob)) == NULL) {
err_typed_ass_var_tpl: Dee_DECREF(ass_expr); goto err_typed_var_tpl_attr_nametok;
     }
     // >> At this point all we need to do is create a tuple of
     //    expressions in 'init_ast->ob_op_b->ob_args_a'
     if (DeeForeignFunctionType_Check(used_type)) {
      // Parse foreign closure function code block
      TPPTokenID old_func_name,func_name =
       (ass_expr->ob_kind == DeeExprAstKind_VARDECL)
       ? ass_expr->ob_op_a->ob_var->ob_name : KWD___anonymous__;
      old_func_name = ((DeeLexerObject *)self)->ob_func_name;
      ((DeeLexerObject *)self)->ob_func_name = func_name;
      initializer = _DeeLexer_ParseForeignClosureCodeBlock(
       (DeeLexerObject *)self,(DeeScopeObject *)scope,flags,
       &attr,(DeeForeignFunctionTypeObject *)used_type,func_argnames,name_token);
      ((DeeLexerObject *)self)->ob_func_name = old_func_name;
      if (!initializer) goto err_typed_ass_var_tpl;
      // Assign the function to a local variable
      if (ass_expr->ob_kind == DeeExprAstKind_VARDECL) {
       DEE_ASSERT(ass_expr->ob_op_b == init_ast);
       Dee_DECREF(init_ast);
       Dee_INHERIT_REF(ass_expr->ob_op_b,initializer);
      } else {
       Dee_DECREF(ass_expr);
       Dee_INHERIT_REF(ass_expr,initializer);
      }
     } else if (token.tk_id == '=') { // with initializer
      if (!yield()) goto err_typed_ass_var_tpl;
      if ((initializer = (DeeExprAstObject *)DeeLexer_ParseExpression(
       (DeeObject *)self,(DeeObject *)scope,flags)) == NULL) goto err_typed_ass_var_tpl;
      init_ast->ob_op_b->ob_args_a = (DeeTupleObject *)DeeTuple_Pack(1,initializer);
      Dee_DECREF(initializer);
      if (!init_ast->ob_op_b->ob_args_a) goto err_typed_ass_var_tpl;
     } else { // No arguments given
      Dee_INCREF(init_ast->ob_op_b->ob_args_a = (DeeTupleObject *)Dee_EmptyTuple);
     }
     Dee_XDECREF(name_token);
     Dee_XDECREF(func_argnames);
     DeeAttributeDecl_Quit(&attr);
     if (token.tk_id != ',') break;
     if (!yield()) goto err_typed_var_tpl;
    }
    DEE_ASSERT(ass_expr != NULL);
    if (typed_vardecl_tuple) {
     int temp;
     DeeExprAstObject *final_ast;
     temp = DeeTuple_Append(&typed_vardecl_tuple,(DeeObject *)ass_expr);
     Dee_DECREF(ass_expr);
#ifdef DEE_DEBUG
     ass_expr = NULL;
#endif
     if (temp == -1) {
err_typed_var_tpl:
      Dee_XDECREF(typed_vardecl_tuple);
/*err_expr: */
      Dee_DECREF(expr);
      return NULL;
     }
     if ((final_ast = (DeeExprAstObject *)DeeExprAst_New(
      DeeExprAstKind_TUPLE,(DeeObject *)expr->ob_token)
      ) == NULL) goto err_typed_var_tpl;
     Dee_INHERIT_REF(final_ast->ob_args_a,*(DeeTupleObject **)&typed_vardecl_tuple);
     Dee_INHERIT_REF(result,final_ast);
    } else {
     Dee_INHERIT_REF(result,ass_expr);
    }
    Dee_DECREF(expr);
   } else { // Default expression
    Dee_INHERIT_REF(result,expr); // Inherit reference
   }
   return result;
  } /*break;*/
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionDecl(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, DEE_A_IN Dee_uint32_t flags) {
 return (DeeObject *)_deeparse_decl((DeeLexerObject *)self,(DeeScopeObject *)scope,flags);
}

DEE_STATIC_INLINE(int) _deeoptimize_constant_tuple_ast(DeeExprAstObject *ast) {
 DeeObject *tuple,**dst;
 DeeExprAstObject **src,**src_end;
 Dee_size_t tuple_size = DeeTuple_SIZE(ast->ob_args_a);
 if ((tuple = _DeeTuple_NewUnsafe(tuple_size)) == NULL) return -1;
 src_end = (src = (DeeExprAstObject **)DeeTuple_ELEM(ast->ob_args_a))+tuple_size;
 dst = DeeTuple_ELEM(tuple);
 while (src != src_end) Dee_INCREF(*dst++ = (*src++)->ob_const);
 Dee_DECREF(ast->ob_args_a);
 Dee_INHERIT_REF(ast->ob_const,tuple);
 ast->ob_kind = DeeExprAstKind_CONSTANT;
 return 0;
}


#ifndef __INTELLISENSE__
#undef yield
#undef token
#undef token_ob
#undef new_ast
#undef _do_optimize_ast
#undef _optimize_ast
#undef _deeparse_top
#endif

void DeeExprAst_Assign(DeeExprAstObject *self, DeeExprAstObject const *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeExprAst_Check(right));
 Dee_INCREF(right); // in case 'right' is currently owned by 'self'
 Dee_DECREF(self->ob_token);
 Dee_XDECREF(self->ob_op_a);
 Dee_XDECREF(self->ob_op_b);
 Dee_XDECREF(self->ob_op_c);
 Dee_XDECREF(self->ob_op_d);
 self->ob_kind = right->ob_kind;
 Dee_INCREF(self->ob_token = right->ob_token);
 Dee_XINCREF(self->ob_op_a = right->ob_op_a);
 Dee_XINCREF(self->ob_op_b = right->ob_op_b);
 Dee_XINCREF(self->ob_op_c = right->ob_op_c);
 Dee_XINCREF(self->ob_op_d = right->ob_op_d);
 Dee_DECREF(right);
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_EXCEPT(-1) int DeeExprAst_Optimize(
 DEE_A_INOUT_OBJECT(DeeExprAstObject) *self,
 DEE_A_INOUT_OBJECT(DeeScopeObject) *scope, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
#ifdef DEE_DEBUG
 DeeExprAstObject *ast = (DeeExprAstObject *)self;
#else
#define ast ((DeeExprAstObject *)self)
#endif
 switch (ast->ob_kind) {
  case DeeExprAstKind_VARDECL:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1 ||
       DeeExprAst_Optimize((DeeObject *)ast->ob_op_b,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   // TODO: Add this optimization for when ob_op_a is a tuple
   if (ast->ob_op_a->ob_kind == DeeExprAstKind_DECLVARIABLE &&
      (flags&DEE_OPTIMIZE_FLAG_UNUSED_VAR)!=0 &&
       !ast->ob_op_a->ob_var->ob_uses) {
    DeeTypeObject *predicted_rhs;
    predicted_rhs = DeeExprAst_PredictType((DeeObject *)ast->ob_op_b,scope);
    if (predicted_rhs && (DeeType_FLAGS(predicted_rhs)&DEE_TYPE_FLAG_NO_LIFETIME_EFFECT)!=0) {
     // Assignment of value without lifetime effect to unused variable
     if ((flags&DEE_OPTIMIZE_FLAG_USED)==0 &&
         DeeExprAst_IsNoeffect((DeeObject *)ast->ob_op_b,scope)) {
      // Delete branch
      Dee_CLEAR(ast->ob_op_b);
      Dee_DECREF(ast->ob_op_a);
      Dee_INCREF(ast->ob_const = Dee_None);
      ast->ob_kind = DeeExprAstKind_CONSTANT;
     } else {
      // Remove vardecl (only keep the initializer)
      DeeExprAst_Assign(ast,ast->ob_op_b);
     }
    }
   }
   break;
   
  case DeeExprAstKind_CODE:
   return DeeStmtAst_Optimize(
    (DeeObject *)ast->ob_stmt,
    (DeeObject *)ast->ob_scope,flags);

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL:
   if (!ast->ob_op_a) return 0;
   // fallthrough
#endif
  case DeeExprAstKind_NOT:
  case DeeExprAstKind_NEG:
  case DeeExprAstKind_POS:
  case DeeExprAstKind_INV:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF:
  case DeeExprAstKind_DEREF:
#endif
  case DeeExprAstKind_INC:
  case DeeExprAstKind_DEC:
  case DeeExprAstKind_INC_POST:
  case DeeExprAstKind_DEC_POST:
  case DeeExprAstKind_SEQ_SIZE:
  case DeeExprAstKind_EXPAND:
  case DeeExprAstKind_BUILTIN_HELP:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_NEW:
  case DeeExprAstKind_DELETE:
#endif
  case DeeExprAstKind_BOOL:
  case DeeExprAstKind_SUPER:
  case DeeExprAstKind_CLASSOF:
  case DeeExprAstKind_ATTR_GET:
  case DeeExprAstKind_STR:
  case DeeExprAstKind_REPR:
  case DeeExprAstKind_COPY:
  case DeeExprAstKind_MOVE:
  case DeeExprAstKind_TYPE:
  case DeeExprAstKind_WEAK:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DeeExprAstKind_BUILTIN_ALLOCA:
#endif
   return DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(
    DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED);

  case DeeExprAstKind_ASSIGN:
  case DeeExprAstKind_CMP_LO:
  case DeeExprAstKind_CMP_LE:
  case DeeExprAstKind_CMP_EQ:
  case DeeExprAstKind_CMP_NE:
  case DeeExprAstKind_CMP_GR:
  case DeeExprAstKind_CMP_GE:
  case DeeExprAstKind_ADD:
  case DeeExprAstKind_IADD:
  case DeeExprAstKind_SUB:
  case DeeExprAstKind_ISUB:
  case DeeExprAstKind_MUL:
  case DeeExprAstKind_IMUL:
  case DeeExprAstKind_DIV:
  case DeeExprAstKind_IDIV:
  case DeeExprAstKind_MOD:
  case DeeExprAstKind_IMOD:
  case DeeExprAstKind_SHL:
  case DeeExprAstKind_ISHL:
  case DeeExprAstKind_SHR:
  case DeeExprAstKind_ISHR:
  case DeeExprAstKind_AND:
  case DeeExprAstKind_IAND:
  case DeeExprAstKind_OR:
  case DeeExprAstKind_IOR:
  case DeeExprAstKind_XOR:
  case DeeExprAstKind_IXOR:
  case DeeExprAstKind_POW:
  case DeeExprAstKind_IPOW:
  case DeeExprAstKind_LAND:
  case DeeExprAstKind_LOR:
  case DeeExprAstKind_LXOR:
  case DeeExprAstKind_SEQ_GET:
  case DeeExprAstKind_IS:
  case DeeExprAstKind_IN:
  case DeeExprAstKind_BUILTIN_EXPECT:
  case DeeExprAstKind_CALL:
  case DeeExprAstKind___SUPER:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_op_b,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED);

  case DeeExprAstKind_COND:
  case DeeExprAstKind_SEQ_SET:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_b,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_op_c,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED);

  case DeeExprAstKind_SEQ_RANGE_GET:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (ast->ob_op_b && DeeExprAst_Optimize((DeeObject *)ast->ob_op_b,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return ast->ob_op_c ? DeeExprAst_Optimize((DeeObject *)ast->ob_op_c,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) : 0;

  case DeeExprAstKind_ATTR_SET:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_op_c,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED);

  case DeeExprAstKind_SEQ_RANGE_SET:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_op_a,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (ast->ob_op_b && DeeExprAst_Optimize((DeeObject *)ast->ob_op_b,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (ast->ob_op_c && DeeExprAst_Optimize((DeeObject *)ast->ob_op_c,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_op_d,(DeeObject *)scope,flags|DEE_OPTIMIZE_FLAG_USED);

  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
   return DeeStmtAst_Optimize((DeeObject *)ast->ob_fun_stmt,(DeeObject *)ast->ob_scope,flags|DEE_OPTIMIZE_FLAG_USED);


  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_EXPR:
   return DeeExprAst_Optimize((DeeObject *)ast->ob_fun_expr,(DeeObject *)ast->ob_scope,flags|DEE_OPTIMIZE_FLAG_USED);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DeeExprAstKind_CLASS: {
   struct DeeClassDeclEntry *iter,*end;
   if (ast->ob_classdecl->ob_base && DeeExprAst_Optimize((DeeObject *)ast->ob_classdecl->ob_base,(
    DeeObject *)ast->ob_scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   end = (iter = ast->ob_classdecl->ob_entryv)+ast->ob_classdecl->ob_entryc;
   while (iter != end) {
    switch (iter->ob_kind) {
     case DeeClassDeclEntryKind_SLOT:
      if (DeeExprAst_Optimize((DeeObject *)iter->ob_slot.ob_callback,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
      break;
     case DeeClassDeclEntryKind_METHOD:
     case DeeClassDeclEntryKind_CLASS_METHOD:
      if (DeeExprAst_Optimize((DeeObject *)iter->ob_method.ob_callback,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
      break;
     case DeeClassDeclEntryKind_GETSET:
     case DeeClassDeclEntryKind_CLASS_GETSET:
      if (iter->ob_getset.ob_get && DeeExprAst_Optimize((DeeObject *)iter->ob_getset.ob_get,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
      if (iter->ob_getset.ob_del && DeeExprAst_Optimize((DeeObject *)iter->ob_getset.ob_del,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
      if (iter->ob_getset.ob_set && DeeExprAst_Optimize((DeeObject *)iter->ob_getset.ob_set,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
      break;
     default: break;
    }
    ++iter;
   }
  } break;
#endif
  case DeeExprAstKind_LIST:
  case DeeExprAstKind_TUPLE:
  case DeeExprAstKind_DICT:
  case DeeExprAstKind_SET: {
   DeeObject **iter,**end;
   end = (iter = DeeTuple_ELEM(ast->ob_args_a))+DeeTuple_SIZE(ast->ob_args_a);
   while (iter != end) if (DeeExprAst_Optimize(*iter++,scope,flags) == -1) return -1;
  } break;

  default: break;
 }
 return 0;
#ifdef ast
#undef ast
#endif
}
DEE_COMPILER_MSVC_WARNING_POP



DEE_A_RET_OBJECT(DeeExprAstObject) *DeeExprAst_Begin(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) {
 while (1) {
  DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
  switch (DeeExprAst_KIND(self)) {
   case DeeExprAstKind_VARDECL:
   case DeeExprAstKind_ASSIGN:
   case DeeExprAstKind_COND:
   case DeeExprAstKind_INC_POST:
   case DeeExprAstKind_DEC_POST:
   case DeeExprAstKind_CMP_LO:
   case DeeExprAstKind_CMP_LE:
   case DeeExprAstKind_CMP_EQ:
   case DeeExprAstKind_CMP_NE:
   case DeeExprAstKind_CMP_GR:
   case DeeExprAstKind_CMP_GE:
   case DeeExprAstKind_ADD:
   case DeeExprAstKind_IADD:
   case DeeExprAstKind_SUB:
   case DeeExprAstKind_ISUB:
   case DeeExprAstKind_MUL:
   case DeeExprAstKind_IMUL:
   case DeeExprAstKind_DIV:
   case DeeExprAstKind_IDIV:
   case DeeExprAstKind_MOD:
   case DeeExprAstKind_IMOD:
   case DeeExprAstKind_SHL:
   case DeeExprAstKind_ISHL:
   case DeeExprAstKind_SHR:
   case DeeExprAstKind_ISHR:
   case DeeExprAstKind_AND:
   case DeeExprAstKind_IAND:
   case DeeExprAstKind_OR:
   case DeeExprAstKind_IOR:
   case DeeExprAstKind_XOR:
   case DeeExprAstKind_IXOR:
   case DeeExprAstKind_POW:
   case DeeExprAstKind_IPOW:
   case DeeExprAstKind_LAND:
   case DeeExprAstKind_LOR:
   case DeeExprAstKind_LXOR:
   case DeeExprAstKind_CALL:
   case DeeExprAstKind_SEQ_GET:
   case DeeExprAstKind_SEQ_SET:
   case DeeExprAstKind_SEQ_RANGE_GET:
   case DeeExprAstKind_SEQ_RANGE_SET:
   case DeeExprAstKind_ATTR_GET:
   case DeeExprAstKind_ATTR_SET:
   case DeeExprAstKind_EXPAND:
   case DeeExprAstKind_IS:
   case DeeExprAstKind_IN:
    self = (DeeObject *)DeeExprAst_OP_A(self);
    break;
   default: goto end;
  }
 }
end:
 return (DeeObject *)self;
}
DEE_A_RET_OBJECT(DeeExprAstObject) *DeeExprAst_End(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) {
 while (1) {
  DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
  switch (DeeExprAst_KIND(self)) {
   case DeeExprAstKind_BOOL:
   case DeeExprAstKind_NOT:
   case DeeExprAstKind_NEG:
   case DeeExprAstKind_POS:
   case DeeExprAstKind_INV:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
   case DeeExprAstKind_REF:
   case DeeExprAstKind_DEREF:
#endif
   case DeeExprAstKind_INC:
   case DeeExprAstKind_DEC:
   case DeeExprAstKind_INC_POST:
   case DeeExprAstKind_DEC_POST:
    self = (DeeObject *)DeeExprAst_OP_A(self);
    break;
   case DeeExprAstKind_VARDECL:
   case DeeExprAstKind_ASSIGN:
   case DeeExprAstKind_CMP_LO:
   case DeeExprAstKind_CMP_LE:
   case DeeExprAstKind_CMP_EQ:
   case DeeExprAstKind_CMP_NE:
   case DeeExprAstKind_CMP_GR:
   case DeeExprAstKind_CMP_GE:
   case DeeExprAstKind_ADD:
   case DeeExprAstKind_IADD:
   case DeeExprAstKind_SUB:
   case DeeExprAstKind_ISUB:
   case DeeExprAstKind_MUL:
   case DeeExprAstKind_IMUL:
   case DeeExprAstKind_DIV:
   case DeeExprAstKind_IDIV:
   case DeeExprAstKind_MOD:
   case DeeExprAstKind_IMOD:
   case DeeExprAstKind_SHL:
   case DeeExprAstKind_ISHL:
   case DeeExprAstKind_SHR:
   case DeeExprAstKind_ISHR:
   case DeeExprAstKind_AND:
   case DeeExprAstKind_IAND:
   case DeeExprAstKind_OR:
   case DeeExprAstKind_IOR:
   case DeeExprAstKind_XOR:
   case DeeExprAstKind_IXOR:
   case DeeExprAstKind_POW:
   case DeeExprAstKind_IPOW:
   case DeeExprAstKind_LAND:
   case DeeExprAstKind_LOR:
   case DeeExprAstKind_LXOR:
   case DeeExprAstKind_IS:
   case DeeExprAstKind_IN:
    self = (DeeObject *)DeeExprAst_OP_B(self);
    break;
   case DeeExprAstKind_COND:
    // The C branch might be NULL if it is omitted
    self = DeeExprAst_OP_C(self)
     ? (DeeObject *)DeeExprAst_OP_C(self)
     : (DeeObject *)DeeExprAst_OP_B(self);
    break;
   case DeeExprAstKind_SEQ_SET:
   case DeeExprAstKind_ATTR_SET:
    self = (DeeObject *)DeeExprAst_OP_C(self);
    break;
   case DeeExprAstKind_SEQ_RANGE_SET:
    self = (DeeObject *)DeeExprAst_OP_D(self);
    break;
   case DeeExprAstKind_FUNCTION_EXPR:
    self = (DeeObject *)DeeExprAst_FUNCTION_EXPR(self);
    break;
   default: goto end;
  }
 }
end:
 return (DeeObject *)self;
}






static void _deeexprast_tp_dtor(DeeExprAstObject *self) {
 if (self->ob_kind == DeeExprAstKind_VARIABLE && self->ob_var) {
  DEE_ASSERT(DeeObject_Check(self->ob_var) && DeeLocalVar_Check(self->ob_var));
  DEE_ASSERT(self->ob_var->ob_uses); // Variable was never used
  --self->ob_var->ob_uses;
 }
 DEE_ASSERT(DeeObject_Check(self->ob_token) && DeeToken_Check(self->ob_token));
 Dee_DECREF(self->ob_token);
 Dee_XDECREF(self->ob_op_a);
 Dee_XDECREF(self->ob_op_b);
 Dee_XDECREF(self->ob_op_c);
 Dee_XDECREF(self->ob_op_d);
}
DEE_VISIT_PROC(_deeexprast_tp_visit,DeeExprAstObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 Dee_VISIT(self->ob_token);
 Dee_XVISIT(self->ob_op_a);
 Dee_XVISIT(self->ob_op_b);
 Dee_XVISIT(self->ob_op_c);
 Dee_XVISIT(self->ob_op_d);
}
static int _deeexprast_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeExprAstObject *self, DeeExprAstObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeExprAst_Check(right));
 self->ob_kind = right->ob_kind;
 Dee_INCREF(self->ob_token = right->ob_token);
 Dee_XINCREF(self->ob_op_a = right->ob_op_a);
 Dee_XINCREF(self->ob_op_b = right->ob_op_b);
 Dee_XINCREF(self->ob_op_c = right->ob_op_c);
 Dee_XINCREF(self->ob_op_d = right->ob_op_d);
 return 0;
}
static int _deeexprast_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeExprAstObject *self, DeeExprAstObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeExprAst_Check(right));
 self->ob_kind = right->ob_kind;
 Dee_INCREF(self->ob_token = right->ob_token);
 self->ob_op_a = right->ob_op_a;
 self->ob_op_b = right->ob_op_b;
 self->ob_op_c = right->ob_op_c;
 self->ob_op_d = right->ob_op_d;
 self->ob_kind = DeeExprAstKind_CONSTANT;
 Dee_INCREF(right->ob_const = Dee_None);
 right->ob_op_b = NULL;
 right->ob_op_c = NULL;
 right->ob_op_d = NULL;
 return 0;
}
static int _deeexprast_tp_copy_assign(DeeExprAstObject *self, DeeExprAstObject *right) {
 if (self != right) {
  _deeexprast_tp_dtor(self);
  _deeexprast_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
static int _deeexprast_tp_move_assign(DeeExprAstObject *self, DeeExprAstObject *right) {
 if (self != right) {
  _deeexprast_tp_dtor(self);
  _deeexprast_tp_move_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static DeeObject *_deeexprast_tp_repr(DeeExprAstObject *self) {
 static char const binary_fmt[] = "(%r) %s (%r)";
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 switch (DeeExprAst_KIND(self)) {
  case DeeExprAstKind_DECLVARIABLE:
  case DeeExprAstKind_VARIABLE:
  case DeeExprAstKind_MODULE:
   result = DeeToken_Str((DeeObject *)DeeExprAst_TOKEN(self));
   break;
  case DeeExprAstKind_DEX:
   result = DeeString_Newf("__dex %K",DeeToken_Str((DeeObject *)DeeExprAst_TOKEN(self)));
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   result = DeeString_Newf("(%r)",DeeExprAst_STMT(self));
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
  case DeeExprAstKind_CONSTANT:
   result = DeeObject_Repr(DeeExprAst_CONST(self)); break;
  case DeeExprAstKind_COND:
   result = DeeString_Newf(
    "(%r)?(%r):(%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self),
    DeeExprAst_OP_C(self) ? (DeeObject *)DeeExprAst_OP_C(self) : Dee_None);
   break;
  case DeeExprAstKind_STR: result = DeeString_Newf("str(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_REPR: result = DeeString_Newf("repr(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_COPY: result = DeeString_Newf("copy(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_MOVE: result = DeeString_Newf("move(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_TYPE: result = DeeString_Newf("type(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_WEAK: result = DeeString_Newf("weak(%r)",DeeExprAst_OP_A(self)); break;
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF: result = DeeString_Newf("&(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_DEREF: result = DeeString_Newf("*(%r)",DeeExprAst_OP_A(self)); break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DeeExprAstKind_BUILTIN_ALLOCA: result = DeeString_Newf("__builtin_alloca(%r)",DeeExprAst_OP_A(self)); break;
#endif
  case DeeExprAstKind_BUILTIN_HELP: result = DeeString_Newf("__builtin_help(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_BOOL: result = DeeString_Newf("!!(%r)",DeeExprAst_OP_A(self)); break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND:
   result = DeeString_Newf("__builtin_bound(%r)",DeeExprAst_OP_A(self)); break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL: {
   DeeString_NEW_STATIC(_empty_cell_string,"<>");
   if (DeeExprAst_OP_A(self)) result = DeeString_Newf("<(%r)>",DeeExprAst_OP_A(self));
   else Dee_INCREF(result = (DeeObject *)&_empty_cell_string);
  } break;
#endif
  case DeeExprAstKind_NOT: result = DeeString_Newf("!(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_NEG: result = DeeString_Newf("-(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_POS: result = DeeString_Newf("+(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_INV: result = DeeString_Newf("~(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_INC: result = DeeString_Newf("++(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_DEC: result = DeeString_Newf("--(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_INC_POST: result = DeeString_Newf("(%r)++",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_DEC_POST: result = DeeString_Newf("(%r)--",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_BUILTIN_EXPECT: result = DeeString_Newf("__builtin_expect(%r,%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self)); break;
  case DeeExprAstKind_SUPER:
   if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_VARIABLE &&
      (DeeExprAst_OP_A(self)->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_THIS)!=0) {
    DeeReturn_STATIC_STRING("(this).super");
   } else {
    result = DeeString_Newf("(%r).super",DeeExprAst_OP_A(self));
   }
   break;
  case DeeExprAstKind___SUPER:
   if (DeeExprAst_OP_B(self)->ob_kind == DeeExprAstKind_VARIABLE &&
      (DeeExprAst_OP_B(self)->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_THIS)!=0) {
    result = DeeString_Newf("__super(%r,(this))",DeeExprAst_OP_A(self));
   } else {
    result = DeeString_Newf("__super(%r,%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self));
   }
   break;
  case DeeExprAstKind_CLASSOF: result = DeeString_Newf("(%r).class",DeeExprAst_OP_A(self)); break;
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_NEW: result = DeeString_Newf("new(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_DELETE: result = DeeString_Newf("delete(%r)",DeeExprAst_OP_A(self)); break;
#endif
#define BINARY_SIMPLE(op)\
   result = DeeString_Newf(binary_fmt,\
    DeeExprAst_OP_A(self),#op,\
    DeeExprAst_OP_B(self))
  case DeeExprAstKind_VARDECL:BINARY_SIMPLE(=); break; // <expr> "=" <expr>
  case DeeExprAstKind_ASSIGN: BINARY_SIMPLE(:=); break; // <expr> ":=" <expr>
  case DeeExprAstKind_CMP_LO: BINARY_SIMPLE(<); break; // <expr> "<" <expr>
  case DeeExprAstKind_CMP_LE: BINARY_SIMPLE(<=); break; // <expr> "<=" <expr>
  case DeeExprAstKind_CMP_EQ: BINARY_SIMPLE(==); break; // <expr> "==" <expr>
  case DeeExprAstKind_CMP_NE: BINARY_SIMPLE(!=); break; // <expr> "!=" <expr>
  case DeeExprAstKind_CMP_GR: BINARY_SIMPLE(>); break; // <expr> ">" <expr>
  case DeeExprAstKind_CMP_GE: BINARY_SIMPLE(>=); break; // <expr> ">=" <expr>
  case DeeExprAstKind_ADD:    BINARY_SIMPLE(+); break; // <expr> "+" <expr>
  case DeeExprAstKind_IADD:   BINARY_SIMPLE(+=); break; // <expr> "+=" <expr>
  case DeeExprAstKind_SUB:    BINARY_SIMPLE(-); break; // <expr> "-" <expr>
  case DeeExprAstKind_ISUB:   BINARY_SIMPLE(-=); break; // <expr> "-=" <expr>
  case DeeExprAstKind_MUL:    BINARY_SIMPLE(*); break; // <expr> "*" <expr>
  case DeeExprAstKind_IMUL:   BINARY_SIMPLE(*=); break; // <expr> "*=" <expr>
  case DeeExprAstKind_DIV:    BINARY_SIMPLE(/); break; // <expr> "/" <expr>
  case DeeExprAstKind_IDIV:   BINARY_SIMPLE(/=); break; // <expr> "/=" <expr>
  case DeeExprAstKind_MOD:    BINARY_SIMPLE(%); break; // <expr> "%" <expr>
  case DeeExprAstKind_IMOD:   BINARY_SIMPLE(%=); break; // <expr> "%=" <expr>
  case DeeExprAstKind_SHL:    BINARY_SIMPLE(<<); break; // <expr> "<<" <expr>
  case DeeExprAstKind_ISHL:   BINARY_SIMPLE(<<=); break; // <expr> "<<=" <expr>
  case DeeExprAstKind_SHR:    BINARY_SIMPLE(>>); break; // <expr> ">>" <expr>
  case DeeExprAstKind_ISHR:   BINARY_SIMPLE(>>=); break; // <expr> ">>=" <expr>
  case DeeExprAstKind_AND:    BINARY_SIMPLE(&); break; // <expr> "&" <expr>
  case DeeExprAstKind_IAND:   BINARY_SIMPLE(&=); break; // <expr> "&=" <expr>
  case DeeExprAstKind_OR:     BINARY_SIMPLE(|); break; // <expr> "|" <expr>
  case DeeExprAstKind_IOR:    BINARY_SIMPLE(|=); break; // <expr> "|=" <expr>
  case DeeExprAstKind_XOR:    BINARY_SIMPLE(^); break; // <expr> "^" <expr>
  case DeeExprAstKind_IXOR:   BINARY_SIMPLE(^=); break; // <expr> "^=" <expr>
  case DeeExprAstKind_POW:    BINARY_SIMPLE(**); break; // <expr> "**" <expr>
  case DeeExprAstKind_IPOW:   BINARY_SIMPLE(**=); break; // <expr> "**=" <expr>
  case DeeExprAstKind_LAND:   BINARY_SIMPLE(&&); break; // <expr> "&&" <expr>
  case DeeExprAstKind_LOR :   BINARY_SIMPLE(||); break; // <expr> "||" <expr>
  case DeeExprAstKind_LXOR:   BINARY_SIMPLE(^^); break; // <expr> "^^" <expr>
#undef BINARY_SIMPLE
  case DeeExprAstKind_LIST: {
   DeeObject *l = DeeList_New();
   if (l) {
    if (DeeList_AppendSequence(l,(DeeObject *)DeeExprAst_ARGS_A(self)) != -1) {
     result = DeeObject_Repr(l);
    } else result = NULL;
    Dee_DECREF(l);
   } else result = NULL;
  } break;
  case DeeExprAstKind_TUPLE: result = DeeObject_Repr((DeeObject *)DeeExprAst_ARGS_A(self)); break;
  case DeeExprAstKind_CALL:  result = DeeString_Newf("(%r)%r",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self)); break;
  case DeeExprAstKind_SEQ_SIZE: result = DeeString_Newf("#(%r)",DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_SEQ_GET: result = DeeString_Newf("(%r)[%r]",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self)); break;
  case DeeExprAstKind_SEQ_SET: result = DeeString_Newf("(%r)[%r] = (%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self),DeeExprAst_OP_C(self)); break;
  case DeeExprAstKind_SEQ_RANGE_GET:
   result = DeeString_Newf("(%r)[(%r):(%r)]",
    (DeeObject *)DeeExprAst_OP_A(self),
    DeeExprAst_OP_B(self)?(DeeObject *)DeeExprAst_OP_B(self):Dee_None,
    DeeExprAst_OP_C(self)?(DeeObject *)DeeExprAst_OP_C(self):Dee_None);
   break;
  case DeeExprAstKind_SEQ_RANGE_SET:
   result = DeeString_Newf("(%r)[(%r):(%r)] = (%r)",
    DeeExprAst_OP_A(self),
    DeeExprAst_OP_B(self)?(DeeObject *)DeeExprAst_OP_B(self):Dee_None,
    DeeExprAst_OP_C(self)?(DeeObject *)DeeExprAst_OP_C(self):Dee_None,
    DeeExprAst_OP_D(self));
   break;
  case DeeExprAstKind_ATTR_GET: result = DeeString_Newf("(%r).%k",DeeExprAst_OP_A(self),DeeExprAst_MEMBER(self)); break;
  case DeeExprAstKind_ATTR_SET: result = DeeString_Newf("(%r).%k = (%r)",DeeExprAst_OP_A(self),DeeExprAst_MEMBER(self),DeeExprAst_OP_C(self)); break;
  case DeeExprAstKind_EXPAND: result = DeeString_Newf("(%r)...",(DeeObject *)DeeExprAst_OP_A(self)); break;
  case DeeExprAstKind_IS: result = DeeString_Newf("(%r) is (%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self)); break;
  case DeeExprAstKind_IN: result = DeeString_Newf("(%r) in (%r)",DeeExprAst_OP_A(self),DeeExprAst_OP_B(self)); break;
  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
   result = DeeExprAst_FUNCTION_NAME(self)
    ? DeeString_Newf("(function [[name(%r)]] %k %r)",DeeExprAst_FUNCTION_NAME(self),DeeExprAst_FUNCTION_ARGS(self),DeeExprAst_FUNCTION_STMTS(self))
    : DeeString_Newf("(function%k %r)",DeeExprAst_FUNCTION_ARGS(self),DeeExprAst_FUNCTION_STMTS(self));
   break;
  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_FUNCTION_NONE:
  case DeeExprAstKind_VFUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_NONE:
   result = DeeExprAst_FUNCTION_NAME(self) ? (DeeExprAst_FUNCTION_EXPR(self)
    ? DeeString_Newf("(function [[name(%r)]] %k -> (%r))",DeeExprAst_FUNCTION_NAME(self),DeeExprAst_FUNCTION_ARGS(self),DeeExprAst_FUNCTION_EXPR(self))
    : DeeString_Newf("(function [[name(%r)]] %k)",DeeExprAst_FUNCTION_NAME(self),DeeExprAst_FUNCTION_ARGS(self))) : DeeExprAst_FUNCTION_EXPR(self)
    ? DeeString_Newf("(function%k -> (%r))",DeeExprAst_FUNCTION_ARGS(self),DeeExprAst_FUNCTION_EXPR(self))
    : DeeString_Newf("(function%k)",DeeExprAst_FUNCTION_ARGS(self));
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DeeExprAstKind_BUILTIN_UNREACHABLE:
   DeeReturn_STATIC_STRING("__builtin_unreachable()");
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DeeExprAstKind_BUILTIN_BREAKPOINT:
   DeeReturn_STATIC_STRING("__builtin_breakpoint()");
#endif

  case DeeExprAstKind_SET: {
   DeeStringWriter writer = DeeStringWriter_INIT();
   DeeObject **iter,**end,*key;
   end = (iter = DeeTuple_ELEM(((DeeExprAstObject *)self)->ob_dict))+DeeTuple_SIZE(((DeeExprAstObject *)self)->ob_dict);
   if (DeeStringWriter_WRITE(&writer,"set { ") == -1) goto set_err;
   while (iter != end) {
    key = *iter++;
    if (DeeStringWriter_Writef(&writer,"%r%s",
     key,iter == end ? "" : ", ") == -1) goto set_err;
   }
   if (DeeStringWriter_WRITE(&writer," }") == -1) goto set_err;
   result = DeeStringWriter_Pack(&writer);
set_end: DeeStringWriter_Quit(&writer);
   return result;
set_err: result = NULL; goto set_end;
  } break;

  case DeeExprAstKind_DICT: {
   DeeStringWriter writer = DeeStringWriter_INIT();
   DeeObject **iter,**end,*key,*item;
   end = (iter = DeeTuple_ELEM(((DeeExprAstObject *)self)->ob_dict))+DeeTuple_SIZE(((DeeExprAstObject *)self)->ob_dict);
   if (DeeStringWriter_WRITE(&writer,"dict { ") == -1) goto dict_err;
   while (iter != end) {
    key = *iter++,item = *iter++;
    if (DeeStringWriter_Writef(&writer,"%r: %r%s",
     key,item,iter == end ? "" : ", ") == -1) goto dict_err;
   }
   if (DeeStringWriter_WRITE(&writer," }") == -1) goto dict_err;
   result = DeeStringWriter_Pack(&writer);
dict_end: DeeStringWriter_Quit(&writer);
   return result;
dict_err: result = NULL; goto dict_end;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DeeExprAstKind_CLASS: {
   struct DeeClassDeclEntry *iter,*end;
   struct DeeClassDeclObject *decl;
   DeeStringWriter writer = DeeStringWriter_INIT();
   decl = ((DeeExprAstObject *)self)->ob_classdecl;
   end = (iter = decl->ob_entryv)+decl->ob_entryc;
   if ((decl->ob_base ? (decl->ob_name
    ? DeeStringWriter_Writef(&writer,"class [[name(%r)]]: %r {\n",decl->ob_name,decl->ob_base)
    : DeeStringWriter_Writef(&writer,"class: %r {\n",decl->ob_base)) : (decl->ob_name
    ? DeeStringWriter_Writef(&writer,"class [[name(%r)]] {\n",decl->ob_name)
    : DeeStringWriter_WRITE(&writer,"class {\n"))
    ) == -1) goto class_err;
   while (iter != end) {
    switch (iter->ob_kind) {
     case DeeClassDeclEntryKind_SLOT:
      if (DeeStringWriter_Writef(&writer,"operator %s = %r;\n",
       TPPKeyword_BuiltinToStr(_Dee_TypeSlot_ToTokenID(iter->ob_slot.ob_slotid)),
       iter->ob_slot.ob_callback) == -1) goto class_err;
      break;
     case DeeClassDeclEntryKind_CLASS_METHOD:
      if (DeeStringWriter_WRITE(&writer,"class ") == -1) goto class_err;
      goto class_method;
     case DeeClassDeclEntryKind_CLASS_GETSET:
      if (DeeStringWriter_WRITE(&writer,"class ") == -1) goto class_err;
      goto class_getset;
     case DeeClassDeclEntryKind_METHOD:
class_method:
      if (DeeStringWriter_Writef(&writer,"function %k = %r;\n",
       iter->ob_method.ob_name,iter->ob_method.ob_callback) == -1) goto class_err;
      break;
     case DeeClassDeclEntryKind_GETSET:
class_getset:;
      if (DeeStringWriter_Writef(&writer,"property %k = { ",iter->ob_getset.ob_name) == -1) goto class_err;
      if (iter->ob_getset.ob_get && DeeStringWriter_Writef(&writer,"operator __get__ = %r; ",iter->ob_getset.ob_get) == -1) goto class_err;
      if (iter->ob_getset.ob_del && DeeStringWriter_Writef(&writer,"operator __del__ = %r; ",iter->ob_getset.ob_del) == -1) goto class_err;
      if (iter->ob_getset.ob_set && DeeStringWriter_Writef(&writer,"operator __set__ = %r; ",iter->ob_getset.ob_set) == -1) goto class_err;
      if (DeeStringWriter_WRITE(&writer,"};\n") == -1) goto class_err;
      break;
     default: break;
    }
    ++iter;
   }
   if (DeeStringWriter_WRITE(&writer,"}") == -1) goto class_err;
   result = DeeStringWriter_Pack(&writer);
class_end: DeeStringWriter_Quit(&writer);
   return result;
class_err: result = NULL; goto class_end;
  } break;
#endif

  default:
   DeeReturn_STATIC_STRING("expr_ast");
 }
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP

static DeeObject *_deeexprast_tp_alloc(DeeTypeObject *tp_self) {
 DeeObject *result = _DeeExprAstPool_Malloc();
 if (result) DeeObject_INIT(result,tp_self);
 return result;
}
static void _deeexprast_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *ob) {
 _DeeExprAstPool_Free(ob);
}


static struct DeeMemberDef const _deeexprast_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_kind,enum),
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_token,object),
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_op_a,object_null),
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_op_b,object_null),
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_op_c,object_null),
 DEE_MEMBERDEF_RO_v100(DeeExprAstObject,ob_op_d,object_null),
 DEE_MEMBERDEF_END_v100
};

DeeTypeObject DeeExprAst_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("expr_ast"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeExprAstObject),
  member(&_deeexprast_tp_alloc),null,
  member(&_deeexprast_tp_copy_ctor),
  member(&_deeexprast_tp_move_ctor),null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_deeexprast_tp_free),
  member(&_deeexprast_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deeexprast_tp_copy_assign),
  member(&_deeexprast_tp_move_assign),null),
 DEE_TYPE_OBJECT_CAST_v101(null,
  member(&_deeexprast_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deeexprast_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeexprast_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_EXPR_AST_C */
