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
#ifndef GUARD_DEEMON_SCOPE_C
#define GUARD_DEEMON_SCOPE_C 1
#define DEE_LIMITED_API 1

// include/*
#include <deemon/__conf.inl>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/scope.h>
#include DEE_INCLUDE_MEMORY_API()


DEE_DECL_BEGIN


DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetLabel(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN TPPTokenID name) {
 struct DeeParserLabel *iter;
 DEE_ASSERT(self);
 DEE_ASSERTF(name != 0,"Invalid name (0 is used internally)");
 iter = self->ll_front;
 while (iter && iter->pl_name != name) iter = iter->pl_next;
 return iter;
}
DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetCase(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN DeeObject *case_value) {
 struct DeeParserLabel *iter; int temp;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(case_value));
 iter = self->ll_front;
 while (iter) {
  switch (iter->pl_kind) {
   {
    if (0) { case DEE_PARSERLABEL_KIND_C_CASE:
     temp = DeeObject_DeepEquals(iter->pl_c_case.clc_case,case_value);
     if (temp == DEE_OBJECT_DEEPEQUALS_LIKELY) temp = 0;
    }
    if (0) { case DEE_PARSERLABEL_KIND_C_RANGE:
     temp = DeeObject_CompareLe(iter->pl_c_range.clr_begin,case_value);
     if (temp == 1) temp = DeeObject_CompareGe(iter->pl_c_range.clr_end,case_value);
    }
    if (temp < 0) DeeError_Handled(); // No equal
    else if (temp) return iter;
   } break;
   default: break;
  }
  iter = iter->pl_next;
 }
 return iter;
}
DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetCaseRange(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN DeeObject *case_begin,
 DEE_A_IN DeeObject *case_end) {
 struct DeeParserLabel *iter; int temp;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(case_begin));
 DEE_ASSERT(DeeObject_Check(case_end));
 iter = self->ll_front;
 while (iter) {
  switch (iter->pl_kind) {
   {
    if (0) { case DEE_PARSERLABEL_KIND_C_CASE:
     temp = DeeObject_CompareLe(case_begin,iter->pl_c_case.clc_case);
     if (temp == 1) temp = DeeObject_CompareGe(case_end,iter->pl_c_case.clc_case);
    }
    if (0) { case DEE_PARSERLABEL_KIND_C_RANGE:
     temp = DeeObject_CompareGe(case_begin,iter->pl_c_range.clr_begin);
     if (temp == 1) temp = DeeObject_CompareLe(case_begin,iter->pl_c_range.clr_end);
     if (temp == 0) {
      temp = DeeObject_CompareLe(case_begin,iter->pl_c_range.clr_begin);
      if (temp == 1) temp = DeeObject_CompareGe(case_end,iter->pl_c_range.clr_begin);
     }
    }
    if (temp < 0) DeeError_Handled(); // No equal
    else if (temp) return iter;
   } break;
   default: break;
  }
  ++iter;
 }
 return iter;
}
DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetDefaultCase(
 DEE_A_IN struct DeeParserLabelList const *self) {
 struct DeeParserLabel *iter;
 DEE_ASSERT(self);
 iter = self->ll_front;
 while (iter && iter->pl_kind != DEE_PARSERLABEL_KIND_C_DEFAULT) iter = iter->pl_next;
 return iter;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *)
_DeeParserLabelList_AllocLabel(DEE_A_INOUT struct DeeParserLabelList *self) {
 struct DeeParserLabel *result;
 while ((result = (struct DeeParserLabel *)malloc_nz(
  sizeof(struct DeeParserLabel))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 _DeeCodeLabel_Init(&result->pl_code_label);
 result->pl_next = self->ll_front;
 self->ll_front = result;
 return result;
}

DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddLabel(
 DEE_A_INOUT struct DeeParserLabelList *self,
 DEE_A_INOUT DeeTokenObject *dest_token, DEE_A_IN TPPTokenID name) {
 struct DeeParserLabel *result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(dest_token) && DeeToken_Check(dest_token));
 // NOTE: The caller must check for forward labels and
 //       use 'DeeParserLabel_DECLARE' to initialize them
 DEE_ASSERTF(!DeeParserLabelList_GetLabel(self,name),"Label already exists");
 if ((result = _DeeParserLabelList_AllocLabel(self)) == NULL) return NULL;
 result->pl_refs = 0;
 result->pl_name = name;
 Dee_INCREF(result->pl_dest_token = dest_token);
 result->pl_kind = DEE_PARSERLABEL_KIND_LABEL;
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddCase(
 DEE_A_INOUT struct DeeParserLabelList *self,
 DEE_A_INOUT DeeTokenObject *dest_token, DEE_A_INOUT DeeObject *case_value) {
 struct DeeParserLabel *result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(dest_token) && DeeToken_Check(dest_token));
 DEE_ASSERT(DeeObject_Check(case_value));
 DEE_ASSERTF(!DeeParserLabelList_GetCase(self,case_value),"Label already exists");
 if ((result = _DeeParserLabelList_AllocLabel(self)) == NULL) return NULL;
 result->pl_name = 0;
 result->pl_refs = 1;
 Dee_INCREF(result->pl_dest_token = dest_token);
 result->pl_kind = DEE_PARSERLABEL_KIND_C_CASE;
 Dee_INCREF(result->pl_c_case.clc_case = case_value);
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddCaseRange(
 DEE_A_INOUT struct DeeParserLabelList *self,
 DEE_A_INOUT DeeTokenObject *dest_token, DEE_A_INOUT DeeObject *case_begin,
 DEE_A_INOUT DeeObject *case_end) {
 struct DeeParserLabel *result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(dest_token) && DeeToken_Check(dest_token));
 DEE_ASSERT(DeeObject_Check(case_begin));
 DEE_ASSERT(DeeObject_Check(case_end));
 DEE_ASSERTF(!DeeParserLabelList_GetCaseRange(self,case_begin,case_end),"Label already exists");
 if ((result = _DeeParserLabelList_AllocLabel(self)) == NULL) return NULL;
 result->pl_name = 0;
 result->pl_refs = 1;
 Dee_INCREF(result->pl_dest_token = dest_token);
 result->pl_kind = DEE_PARSERLABEL_KIND_C_RANGE;
 Dee_INCREF(result->pl_c_range.clr_begin = case_begin);
 Dee_INCREF(result->pl_c_range.clr_end = case_end);
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddDefaultCase(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_INOUT DeeTokenObject *dest_token) {
 struct DeeParserLabel *result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(dest_token) && DeeToken_Check(dest_token));
 DEE_ASSERTF(!DeeParserLabelList_GetDefaultCase(self),"Label already exists");
 if ((result = _DeeParserLabelList_AllocLabel(self)) == NULL) return NULL;
 result->pl_name = 0;
 result->pl_refs = 1;
 Dee_INCREF(result->pl_dest_token = dest_token);
 result->pl_kind = DEE_PARSERLABEL_KIND_C_DEFAULT;
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_GetOrAddForwardLabel(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_IN TPPTokenID name) {
 struct DeeParserLabel *result;
 DEE_ASSERT(self);
 if ((result = DeeParserLabelList_GetLabel(self,name)) != NULL) return result;
 // Must create a new forward label
 if ((result = _DeeParserLabelList_AllocLabel(self)) == NULL) return NULL;
 result->pl_name = name;
 result->pl_refs = 0;
 result->pl_kind = DEE_PARSERLABEL_KIND_LABEL;
 result->pl_dest_token = NULL;
 return result;
}



struct DeeParserLabelRefList _DeeParserLabelRefList_Empty = DeeParserLabelRefList_INIT();
DEE_A_RET_EXCEPT(-1) int DeeParserLabelRefList_Append(
 DEE_A_INOUT struct DeeParserLabelRefList *self,
 DEE_A_IN struct DeeParserLabel *lbl) {
 struct DeeParserLabel **new_v;
 DEE_ASSERT(self && lbl);
 while ((new_v = (struct DeeParserLabel **)realloc_nz(
  self->lrl_v,(self->lrl_c+1)*sizeof(struct DeeParserLabel *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->lrl_v = new_v;
 new_v[self->lrl_c++] = lbl;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeParserLabelRefList_AppendMoveAll(
 DEE_A_INOUT struct DeeParserLabelRefList *self,
 DEE_A_INOUT struct DeeParserLabelRefList *right) {
 struct DeeParserLabel **new_v;
 Dee_size_t new_size;
 DEE_ASSERT(self && right);
 DEE_ASSERT((self->lrl_c == 0) == (self->lrl_v == NULL));
 DEE_ASSERT((right->lrl_c == 0) == (right->lrl_v == NULL));
 if (!right->lrl_c) return 0; // Nothing to do here...
 if (!self->lrl_c) { _DeeParserLabelRefList_InitMove(self,right); return 0; } // Simple move-construct
 // Complicated case: Must realloc list of labels
 new_size = self->lrl_c+right->lrl_c;
 DEE_ASSERT(new_size);
 while ((new_v = (struct DeeParserLabel **)realloc_nnz(
  self->lrl_v,new_size*sizeof(struct DeeParserLabel *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->lrl_v = new_v;
 // Copy all labels into the new vector
 memcpy(new_v+self->lrl_c,right->lrl_v,
        right->lrl_c*sizeof(struct DeeParserLabel *));
 self->lrl_c = new_size;
 free_nn(right->lrl_v);
 right->lrl_c = 0;
 right->lrl_v = NULL;
 return 0;
}


























DEE_A_RET_OBJECT_EXCEPT_REF(DeeLocalVarObject) *DeeLocalVar_New(
 DEE_A_IN TPPTokenID name, DEE_A_IN_OBJECT(DeeScopeObject) *scope) {
 DeeLocalVarObject *result;
 DEE_ASSERT(!name || DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 if ((result = DeeObject_MALLOCF(DeeLocalVarObject,
  "local_var (name: %u, scope: %p)",(unsigned)name,scope)) != NULL) {
  DeeObject_INIT(result,&DeeLocalVar_Type);
  Dee_WEAKINCREF(result->lv_scope = (DeeScopeObject *)scope);
  result->lv_name = name;
  result->lv_flags = DEE_LOCAL_VAR_FLAG_NONE;
  result->lv_init = 0;
  result->lv_uses = 0;
  result->lv_module_refs = 1;
 }
 return (DeeObject *)result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeScopeObject) *DeeScope_New(
 DEE_A_INOUT_OBJECT_OPT(DeeScopeObject) *prev, DEE_A_IN int weak) {
 DeeScopeObject *result;
 DEE_ASSERT(!prev || (DeeObject_Check(prev) && DeeScope_Check(prev)));
 if ((result = (DeeScopeObject *)DeeObject_Mallocf(weak
  ? Dee_OFFSETOF(DeeScopeObject,sc_supertp) : sizeof(DeeScopeObject),
  "scope (prev: %p; weak: %d)",prev,weak)) != NULL) {
  DeeObject_INIT(result,&DeeScope_Type);
  result->sc_namesc = 0;
  result->sc_namesa = 0;
  result->sc_namesv = NULL;
  result->sc_flags = DEE_SCOPE_FLAG_NONE;
  if (weak) {
   result->sc_prev = NULL;
   result->sc_weak_prev = (DeeScopeObject *)prev;
  } else {
   result->sc_prev = (DeeScopeObject *)prev;
   _DeeParserLabelList_Init(&result->sc_labels);
   result->sc_supertp = NULL;
   result->sc_cases = NULL;
   result->sc_weak_prev = NULL;
  }
 }
 return (DeeObject *)result;
}


DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_Global(
 DEE_A_IN_OBJECT(DeeScopeObject) *self) {
 while (1) {
  while (1) {
   DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
   if (!DeeScope_WEAK_PREV(self)) break;
   self = DeeScope_WEAK_PREV(self);
  }
  if (!DeeScope_PREV(self)) break;
  self = DeeScope_PREV(self);
 }
 return self;
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_WeakRoot(
 DEE_A_IN_OBJECT(DeeScopeObject) *self) {
 while (1) {
  DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
  if (!DeeScope_WEAK_PREV(self)) break;
  self = DeeScope_WEAK_PREV(self);
 }
 return self;
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_ModuleRoot(
 DEE_A_IN_OBJECT(DeeScopeObject) *self) {
 while (1) {
  DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
  if (!DeeScope_WEAK_PREV(self) || (((
   DeeScopeObject *)self)->sc_flags&DEE_SCOPE_FLAG_MODULE_SCOPE)!=0) break;
  self = DeeScope_WEAK_PREV(self);
 }
 return self;
}
DEE_A_RET_WUNUSED int DeeScope_IsParent(
 DEE_A_IN_OBJECT(DeeScopeObject) const *parent,
 DEE_A_IN_OBJECT(DeeScopeObject) const *child) {
 DEE_ASSERT(DeeObject_Check(parent) && DeeScope_Check(parent));
 while (1) {
  DEE_ASSERT(DeeObject_Check(child) && DeeScope_Check(child));
  if (DeeScope_WEAK_PREV(child)) child = DeeScope_WEAK_PREV(child);
  else if (DeeScope_PREV(child)) child = DeeScope_PREV(child);
  else break;
  if (child == parent) return 1;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeScope_IsWeakParent(
 DEE_A_IN_OBJECT(DeeScopeObject) const *parent,
 DEE_A_IN_OBJECT(DeeScopeObject) const *child) {
 DEE_ASSERT(DeeObject_Check(parent) && DeeScope_Check(parent));
 DEE_ASSERT(DeeObject_Check(child) && DeeScope_Check(child));
 while (1) {
  if ((child = DeeScope_WEAK_PREV(child)) == NULL) break;
  DEE_ASSERT(DeeObject_Check(child) && DeeScope_Check(child));
  if (child == parent) return 1;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeScope_IsSameOrParent(
 DEE_A_IN_OBJECT(DeeScopeObject) const *parent,
 DEE_A_IN_OBJECT(DeeScopeObject) const *child) {
 DEE_ASSERT(DeeObject_Check(parent) && DeeScope_Check(parent));
 while (1) {
  DEE_ASSERT(DeeObject_Check(child) && DeeScope_Check(child));
  if (child == parent) return 1;
  if (DeeScope_WEAK_PREV(child)) child = DeeScope_WEAK_PREV(child);
  else if (DeeScope_PREV(child)) child = DeeScope_PREV(child);
  else break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeScope_IsSameOrWeakParent(
 DEE_A_IN_OBJECT(DeeScopeObject) const *parent,
 DEE_A_IN_OBJECT(DeeScopeObject) const *child) {
 DEE_ASSERT(DeeObject_Check(parent) && DeeScope_Check(parent));
 while (1) {
  DEE_ASSERT(DeeObject_Check(child) && DeeScope_Check(child));
  if (child == parent) return 1;
  if (!DeeScope_WEAK_PREV(child)) break;
  child = DeeScope_WEAK_PREV(child);
 }
 return 0;
}

DEE_A_RET_WUNUSED int DeeScope_SameWeakScope(
 DEE_A_IN_OBJECT(DeeScopeObject) const *self, 
 DEE_A_IN_OBJECT(DeeScopeObject) const *right) {
 while (1) {
  DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
  if (!DeeScope_WEAK_PREV(self)) break;
  self = DeeScope_WEAK_PREV(self);
 }
 while (1) {
  DEE_ASSERT(DeeObject_Check(right) && DeeScope_Check(right));
  if (!DeeScope_WEAK_PREV(right)) break;
  right = DeeScope_WEAK_PREV(right);
 }
 return self == right;
}

DEE_A_RET_EXCEPT(-1) int _DeeScope_AddName(
 DEE_A_INOUT DeeScopeObject *self, DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *v) {
 struct _DeeScopeEntry *names;
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(!name || DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(!name || !_DeeScope_HasName(self,name));
 DEE_ASSERTF(!DeeLocalVar_Check(v) || ((DeeLocalVarObject *)v)->lv_name == name,
             "Inconsistent naming of local variable (variable says: %u; scope says: %u)",
             ((DeeLocalVarObject *)v)->lv_name,name);
 if (self->sc_namesc == self->sc_namesa) {
  if (self->sc_namesa) self->sc_namesa *= 2; else self->sc_namesa = 8;
  if ((names = (struct _DeeScopeEntry *)realloc_nz(
   self->sc_namesv,self->sc_namesa*sizeof(struct _DeeScopeEntry))
   ) == NULL) return -1;
  self->sc_namesv = names;
 } else names = self->sc_namesv;
 names += self->sc_namesc++;
 names->e_name = name;
 names->e_depr = NULL;
 Dee_INCREF(names->e_const = v);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeScope_AddDeprecatedName(
 DEE_A_INOUT DeeScopeObject *self,
 DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *v, DEE_A_IN_OPT DeeObject *depr) {
 struct _DeeScopeEntry *names;
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(!depr || (DeeObject_Check(depr) && DeeString_Check(depr)));
 DEE_ASSERT(!name || DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(!name || !_DeeScope_HasName(self,name));
 DEE_ASSERTF(!DeeLocalVar_Check(v) || ((DeeLocalVarObject *)v)->lv_name == name,
             "Inconsistent naming of local variable (variable says: %u; scope says: %u)",
             ((DeeLocalVarObject *)v)->lv_name,name);
 if (self->sc_namesc == self->sc_namesa) {
  if (self->sc_namesa) self->sc_namesa *= 2; else self->sc_namesa = 8;
  if ((names = (struct _DeeScopeEntry *)realloc_nz(
   self->sc_namesv,self->sc_namesa*sizeof(struct _DeeScopeEntry))
   ) == NULL) return -1;
  self->sc_namesv = names;
 } else names = self->sc_namesv;
 names += self->sc_namesc++;
 names->e_name = name;
 Dee_XINCREF(names->e_depr = (DeeStringObject *)depr);
 Dee_INCREF(names->e_const = v);
 return 0;
}
void _DeeScope_DelName(DEE_A_INOUT DeeScopeObject *self, DEE_A_IN TPPTokenID name) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) if (iter->e_name == name) {
  _DeeScopeEntry_Quit(iter);
  memmove(iter,iter+1,(Dee_size_t)((end-iter)-1)*sizeof(struct _DeeScopeEntry));
  --self->sc_namesc;
  break;
 } else ++iter;
}
/*noexcept*/int _DeeScope_HasName(
 DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) if ((iter++)->e_name == name) return 1;
 return 0;
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeScope_GetName(
 DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) if (iter->e_name == name) return iter->e_const; else ++iter;
 return NULL;
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeScope_GetNameEx(
 DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name, DEE_A_OUT_OPT DeeObject **is_deprecated) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) if (iter->e_name == name) {
  if (is_deprecated) *is_deprecated = (DeeObject *)iter->e_depr;
  return iter->e_const;
 } else ++iter;
 return NULL;
}

DEE_A_RET_EXCEPT(-1) int DeeScope_ImportScope(
 DEE_A_INOUT_OBJECT(DeeScopeObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) const *src,
 DEE_A_INOUT_OBJECT(DeeLexerObject) *lexer, DEE_A_INOUT_OBJECT(DeeTokenObject) *token,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *depr) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 DEE_ASSERT(DeeObject_Check(src) && DeeScope_Check(src));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(token) && DeeToken_Check(token));
 end = (iter = ((DeeScopeObject *)src)->sc_namesv)+((DeeScopeObject *)src)->sc_namesc;
 while (iter != end) {
  if (DeeScope_ImportSingle(self,iter->e_name,iter->e_const,lexer,token,depr) == -1) return -1;
  ++iter;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeScope_ImportSingle(
 DEE_A_INOUT_OBJECT(DeeScopeObject) *self, DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *ob,
 DEE_A_INOUT_OBJECT(DeeLexerObject) *lexer, DEE_A_INOUT_OBJECT(DeeTokenObject) *token,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *depr) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DEE_TOKENID_IS_VARNAME(name));
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(token) && DeeToken_Check(token));
 end = (iter = ((DeeScopeObject *)self)->sc_namesv)+((DeeScopeObject *)self)->sc_namesc;
 while (1) {
  if (iter == end) {
   if (DeeLocalVar_Check(ob)) ++((DeeLocalVarObject *)ob)->lv_module_refs;
   return _DeeScope_AddDeprecatedName((DeeScopeObject *)self,name,ob,depr);
  }
  if (iter->e_name == name) {
   if (iter->e_const == ob) {
same_ob:
    // Overwrite existing deprecation string
    // >> This allows one to re-import things from a module
    //    while also removing a associated deprecation
    Dee_XDECREF(iter->e_depr);
    Dee_XINCREF(iter->e_depr = (DeeStringObject *)depr);
    return 0; // Same object
   }
   if (Dee_TYPE(iter->e_const) == Dee_TYPE(ob)) {
    int temp;
    if ((temp = DeeObject_DeepEquals(iter->e_const,ob)) < 0) DeeError_Handled();
    else if (temp) goto same_ob; // Same object
   }
   if (DeeError_CompilerErrorf(DEE_WARNING_IMPORT_CAUSES_NAME_COLLISION,lexer,token,
    "Import causes name collision: %q",TPPLexer_TokenIDStr(
    DeeLexer_LEXER(lexer),name)) == -1) return -1;
  } else ++iter;
 }
 return 0;
}










//////////////////////////////////////////////////////////////////////////
// LocalVar VTable
static int _deelocalvar_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeLocalVarObject *self, DeeLocalVarObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLocalVar_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeLocalVar_Check(right));
 self->lv_name = right->lv_name;
 Dee_WEAKINCREF(self->lv_scope = right->lv_scope);
 self->lv_flags = right->lv_flags;
 self->lv_init = right->lv_init;
 self->lv_uses = right->lv_uses;
 self->lv_module_refs = right->lv_module_refs;
 return 0;
}
static void _deelocalvar_tp_dtor(DeeLocalVarObject *self) {
 Dee_WEAKDECREF(self->lv_scope);
}
static int _deelocalvar_tp_copy_assign(DeeLocalVarObject *self, DeeLocalVarObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLocalVar_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeLocalVar_Check(right));
 if (self != right) {
  _deelocalvar_tp_dtor(self);
  _deelocalvar_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}




//////////////////////////////////////////////////////////////////////////
// Scope VTable
static void _deescope_tp_dtor(DeeScopeObject *self) {
 struct _DeeScopeEntry *iter,*end;
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) { _DeeScopeEntry_Quit(iter); ++iter; }
 free(self->sc_namesv);
 if (DeeScope_IS_STRONG(self)) {
  DEE_ASSERTF(self->sc_cases == NULL,"Case labels still assigned in scope destructor");
  _DeeParserLabelList_Quit(&self->sc_labels);
  Dee_XDECREF(self->sc_supertp);
 }
}
DEE_VISIT_PROC(_deescope_tp_visit,DeeScopeObject *self) {
 struct _DeeScopeEntry *iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) Dee_VISIT((iter++)->e_const);
 if (DeeScope_IS_STRONG(self)) {
  _DeeParserLabelList_Visit(&self->sc_labels);
  Dee_XVISIT(self->sc_supertp);
 }
}


#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deelocalvar_tp_members DeeType_DEFAULT_SLOT(tp_members)
#define _deescope_tp_members    DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deelocalvar_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_name",DeeLocalVarObject,lv_name,TPPTokenID),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_scope",DeeLocalVarObject,lv_scope,weak_object),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_flags",DeeLocalVarObject,lv_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_loc_id",DeeLocalVarObject,lv_loc_id,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_init",DeeLocalVarObject,lv_init,uint),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_uses",DeeLocalVarObject,lv_uses,uint),
 DEE_MEMBERDEF_NAMED_RO_v100("__lv_module_refs",DeeLocalVarObject,lv_module_refs,uint),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deescope_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_prev",DeeScopeObject,sc_prev,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_weak_prev",DeeScopeObject,sc_weak_prev,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_namesc",DeeScopeObject,sc_namesc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_namesa",DeeScopeObject,sc_namesa,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_namesv",DeeScopeObject,sc_namesv,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_flags",DeeScopeObject,sc_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_supertp",DeeScopeObject,sc_supertp,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_labels_ll_front",DeeScopeObject,sc_labels.ll_front,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__sc_cases",DeeScopeObject,sc_cases,p(void)),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */


DeeTypeObject DeeLocalVar_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("local_var"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeLocalVarObject),null,null,
  member(&_deelocalvar_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deelocalvar_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deelocalvar_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deelocalvar_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DeeTypeObject DeeScope_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("scope"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeScopeObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deescope_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deescope_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deescope_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};



DEE_DECL_END

#endif /* !GUARD_DEEMON_SCOPE_C */
