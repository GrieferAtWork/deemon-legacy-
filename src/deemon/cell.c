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
#ifndef GUARD_DEEMON_CELL_C
#define GUARD_DEEMON_CELL_C 1
#define DEE_LIMITED_API 1

// /include/*
#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

// NOTE: Cells must be GC objects since it is possible to have them reference themselves

DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *
DeeCell_New(DEE_A_INOUT DeeObject *value) {
 DeeCellObject *result;
 DEE_ASSERT(DeeObject_Check(value));
 if ((result = DeeGC_MALLOC(DeeCellObject)) != NULL) {
  DeeObject_INIT(result,&DeeCell_Type);
  DeeAtomicMutex_Init(&result->c_lock);
  Dee_INCREF(result->c_value = value);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *
DeeCell_NewEmpty(void) {
 DeeCellObject *result;
 if ((result = DeeGC_MALLOC(DeeCellObject)) != NULL) {
  DeeObject_INIT(result,&DeeCell_Type);
  DeeAtomicMutex_Init(&result->c_lock);
  result->c_value = NULL;
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *
DeeCell_DeepCopy(DEE_A_IN_OBJECT(DeeCellObject) const *self) {
 DeeCellObject *result; DeeObject *contained;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 Dee_XINCREF(contained = DeeCell_VALUE(self));
 DeeCell_RELEASE(self);
 if ((result = DeeGC_MALLOC(DeeCellObject)) != NULL) {
  DeeObject_INIT(result,&DeeCell_Type);
  DeeAtomicMutex_Init(&result->c_lock);
  Dee_INHERIT_REF(result->c_value,contained);
  DeeGC_TrackedAdd((DeeObject *)result);
 } else Dee_XDECREF(contained);
 return (DeeObject *)result;
}

DeeError_NEW_STATIC(_DeeError_EmptyCell,&DeeErrorType_ValueError,"Empty cell");
DEE_A_RET_EXCEPT_REF DeeObject *DeeCell_Get(
 DEE_A_IN_OBJECT(DeeCellObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 if ((result = DeeCell_VALUE(self)) == NULL) {
  DeeCell_RELEASE(self);
  DeeError_Throw((DeeObject *)&_DeeError_EmptyCell);
  return NULL;
 }
 Dee_INCREF(result);
 DeeCell_RELEASE(self);
 return result;
}
DEE_A_RET_NOEXCEPT_REF DeeObject *DeeCell_TryGet(
 DEE_A_IN_OBJECT(DeeCellObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 Dee_XINCREF(result = DeeCell_VALUE(self));
 DeeCell_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeCell_Pop(
 DEE_A_INOUT_OBJECT(DeeCellObject) *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 if ((result = DeeCell_VALUE(self)) == NULL) {
  DeeCell_RELEASE(self);
  DeeError_Throw((DeeObject *)&_DeeError_EmptyCell);
 } else {
  DeeCell_VALUE(self) = NULL;
  DeeCell_RELEASE(self);
 }
 return result;
}
DEE_A_RET_NOEXCEPT_REF DeeObject *DeeCell_TryPop(
 DEE_A_INOUT_OBJECT(DeeCellObject) *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 result = DeeCell_VALUE(self);
 DeeCell_VALUE(self) = NULL;
 DeeCell_RELEASE(self);
 return result;
}
void DeeCell_Del(DEE_A_INOUT_OBJECT(DeeCellObject) *self) {
 DeeObject *old_value;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DeeCell_ACQUIRE(self);
 old_value = DeeCell_VALUE(self);
 DeeCell_VALUE(self) = NULL;
 DeeCell_RELEASE(self);
 Dee_XDECREF(old_value);
}
void DeeCell_Set(DEE_A_INOUT_OBJECT(DeeCellObject) *self,
                          DEE_A_INOUT DeeObject *value) {
 DeeObject *old_value;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DEE_ASSERT(DeeObject_Check(value));
 DeeCell_ACQUIRE(self);
 old_value = DeeCell_VALUE(self);
 Dee_INCREF(DeeCell_VALUE(self) = value);
 DeeCell_RELEASE(self);
 Dee_XDECREF(old_value);
}
DEE_A_RET_NOEXCEPT_REF DeeObject *DeeCell_Exchange(
 DEE_A_INOUT_OBJECT(DeeCellObject) *self, DEE_A_INOUT DeeObject *value) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeCell_Check(self));
 DEE_ASSERT(DeeObject_Check(value));
 DeeCell_ACQUIRE(self);
 result = DeeCell_VALUE(self);
 Dee_INCREF(DeeCell_VALUE(self) = value);
 DeeCell_RELEASE(self);
 return result;
}



static int _deecell_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self) {
 DeeAtomicMutex_Init(&self->c_lock);
 self->c_value = NULL;
 return 0;
}
static int _deecell_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self, DeeCellObject *right) {
 DeeAtomicMutex_Init(&self->c_lock);
 DeeCell_ACQUIRE(right);
 Dee_XINCREF(self->c_value = right->c_value);
 DeeCell_RELEASE(right);
 return 0;
}
static int _deecell_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self, DeeCellObject *right) {
 DeeAtomicMutex_Init(&self->c_lock);
 DeeCell_ACQUIRE(right);
 self->c_value = right->c_value;
 right->c_value = NULL;
 DeeCell_RELEASE(right);
 return 0;
}
static int _deecell_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self, DeeObject *args) {
 DeeAtomicMutex_Init(&self->c_lock);
 self->c_value = NULL;
 if (DeeTuple_Unpack(args,"|o:cell",&self->c_value) != 0) return -1;
 Dee_XINCREF(self->c_value);
 return 0;
}
static void _deecell_tp_dtor(DeeCellObject *self) {
 Dee_XDECREF(self->c_value);
}
static void _deecell_tp_clear(DeeCellObject *self) {
 Dee_XCLEAR(self->c_value);
}
DEE_VISIT_PROC(_deecell_tp_visit,DeeCellObject *self) {
 Dee_XVISIT(self->c_value);
}
static int _deecell_tp_copy_assign(DeeCellObject *self, DeeCellObject *right) {
 DeeObject *new_ob,*old_ob;
 if (self != right) {
  DeeCell_ACQUIRE(right);
  Dee_XINCREF(new_ob = right->c_value);
  DeeCell_RELEASE(right);
  DeeCell_ACQUIRE(self);
  Dee_INHERIT_REF(old_ob,self->c_value);
  Dee_INHERIT_REF(self->c_value,new_ob);
  DeeCell_RELEASE(self);
  Dee_XDECREF(old_ob);
 }
 return 0;
}
static int _deecell_tp_move_assign(DeeCellObject *self, DeeCellObject *right) {
 DeeObject *new_ob,*old_ob;
 if (self != right) {
  DeeCell_ACQUIRE(right);
  new_ob = right->c_value;
  right->c_value = NULL;
  DeeCell_RELEASE(right);
  DeeCell_ACQUIRE(self);
  Dee_INHERIT_REF(old_ob,self->c_value);
  Dee_INHERIT_REF(self->c_value,new_ob);
  DeeCell_RELEASE(self);
  Dee_XDECREF(old_ob);
 }
 return 0;
}
static int _deecell_tp_any_assign(DeeCellObject *self, DeeObject *right) {
 DeeObject *old_ob;
 Dee_INCREF(right);
 DeeCell_ACQUIRE(self);
 Dee_INHERIT_REF(old_ob,self->c_value);
 Dee_INHERIT_REF(self->c_value,right);
 DeeCell_RELEASE(self);
 Dee_XDECREF(old_ob);
 return 0;
}
static DeeObject *_deecell_tp_str(DeeCellObject *self) {
 DeeObject *cell_value;
 DeeCell_ACQUIRE(self);
 Dee_XINCREF(cell_value = self->c_value);
 DeeCell_RELEASE(self);
 if (!cell_value) DeeReturn_STATIC_STRING("<>");
 return DeeString_Newf("<%K>",cell_value);
}
static DeeObject *_deecell_tp_repr(DeeCellObject *self) {
 DeeObject *cell_value;
 DeeCell_ACQUIRE(self);
 Dee_XINCREF(cell_value = self->c_value);
 DeeCell_RELEASE(self);
 if (!cell_value) DeeReturn_STATIC_STRING("<>");
 return DeeString_Newf("<%R>",cell_value);
}
static int _deecell_tp_bool(DeeCellObject *self) {
 int result;
 DeeCell_ACQUIRE(self);
 result = self->c_value != NULL;
 DeeCell_RELEASE(self);
 return result;
}
static DeeObject *_deecell_tp_not(DeeCellObject *self) {
 int result;
 DeeCell_ACQUIRE(self);
 result = self->c_value == NULL;
 DeeCell_RELEASE(self);
 DeeReturn_Bool(result);
}
static DeeObject *_deecell_tp_cmp_lo(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value < right->c_value); }
static DeeObject *_deecell_tp_cmp_le(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value <= right->c_value); }
static DeeObject *_deecell_tp_cmp_eq(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value == right->c_value); }
static DeeObject *_deecell_tp_cmp_ne(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value != right->c_value); }
static DeeObject *_deecell_tp_cmp_gr(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value > right->c_value); }
static DeeObject *_deecell_tp_cmp_ge(DeeCellObject *self, DeeCellObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeCell_Type) != 0) return NULL; DeeReturn_Bool(self->c_value >= right->c_value); }
static DeeObject *_deecell_tp_seq_get(DeeCellObject *self, DeeObject *DEE_UNUSED(i)) {
 DeeObject *result;
 DeeCell_ACQUIRE(self);
 if ((result = self->c_value) == NULL) {
  DeeCell_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeCell_Type);
  return NULL;
 } else Dee_INCREF(result);
 DeeCell_RELEASE(self);
 return result;
}
static int _deecell_tp_seq_del(DeeCellObject *self, DeeObject *DEE_UNUSED(i)) {
 DeeObject *result;
 DeeCell_ACQUIRE(self);
 result = self->c_value;
 self->c_value = NULL;
 DeeCell_RELEASE(self);
 Dee_XDECREF(result);
 return 0;
}
static int _deecell_tp_seq_set(DeeCellObject *self, DeeObject *DEE_UNUSED(i), DeeObject *v) {
 DeeObject *result;
 Dee_INCREF(v);
 DeeCell_ACQUIRE(self);
 result = self->c_value;
 self->c_value = v;
 DeeCell_RELEASE(self);
 Dee_XDECREF(result);
 return 0;
}
static DeeObject *_deecell_tp_seq_range_get(
 DeeCellObject *self, DeeObject *lo, DeeObject *hi) {
 DeeObject *value,*result; Dee_ssize_t ilo,ihi;
 if (DeeObject_Cast(Dee_ssize_t,lo,&ilo) != 0) return NULL;
 if (DeeObject_Cast(Dee_ssize_t,hi,&ihi) != 0) return NULL;
 if (ilo >= ihi || ilo != 0) empty: return DeeCell_NewEmpty();
 DeeCell_ACQUIRE(self);
 if (!self->c_value) {
  DeeCell_RELEASE(self);
  goto empty;
 }
 Dee_XINCREF(value = self->c_value);
 DeeCell_RELEASE(self);
 if (value) {
  result = DeeCell_New(value);
  Dee_DECREF(value);
 } else result = DeeCell_NewEmpty();
 return result;
}
static int _deecell_tp_seq_range_del(
 DeeCellObject *self, DeeObject *lo, DeeObject *hi) {
 DeeObject *value; Dee_ssize_t ilo,ihi;
 if (DeeObject_Cast(Dee_ssize_t,lo,&ilo) != 0) return -1;
 if (DeeObject_Cast(Dee_ssize_t,hi,&ihi) != 0) return -1;
 if (ilo >= ihi || ilo != 0) return 0;
 DeeCell_ACQUIRE(self);
 if (!self->c_value) {
  DeeCell_RELEASE(self);
  return 0;
 }
 value = self->c_value;
 self->c_value = NULL;
 DeeCell_RELEASE(self);
 Dee_XDECREF(value);
 return 0;
}
static int _deecell_tp_seq_range_set(
 DeeCellObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 DeeObject *value,*elem; Dee_ssize_t ilo,ihi; int error;
 if (DeeObject_Cast(Dee_ssize_t,lo,&ilo) != 0) return -1;
 if (DeeObject_Cast(Dee_ssize_t,hi,&ihi) != 0) return -1;
 if (ilo >= ihi || ilo != 0) return 0;
 if ((v = DeeObject_IterSelf(v)) == NULL) return -1;
 if ((error = DeeObject_IterNextEx(v,&elem)) < 0) { Dee_DECREF(v); return -1; }
 if (error == 0) {
  error = DeeObject_IterNextEx(v,&value);
  Dee_DECREF(v);
  if (error < 0) { Dee_DECREF(elem); return error; }
  if (error == 0) {
   Dee_DECREF(value);
   Dee_DECREF(elem);
   DeeError_SET_STRING(&DeeErrorType_ValueError,
                       "Can't assign more than one item to a cell");
   return -1;
  }
 } else {
  Dee_DECREF(v);
  elem = NULL;
 }
 DeeCell_ACQUIRE(self);
 if (!self->c_value) {
  DeeCell_RELEASE(self);
  return 0;
 }
 value = self->c_value;
 self->c_value = elem; // inherit reference
 DeeCell_RELEASE(self);
 Dee_XDECREF(value);
 return 0;
}
static DeeObject *_deecell_tp_seq_size(DeeCellObject *self) {
 Dee_size_t result;
 DeeCell_ACQUIRE(self);
 result = (Dee_size_t)(self->c_value ? 1 : 0);
 DeeCell_RELEASE(self);
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deecell_tp_seq_contains(DeeCellObject *self, DeeObject *v) {
 DeeObject *value,*result;
 DeeCell_ACQUIRE(self);
 Dee_INCREF(value = self->c_value);
 DeeCell_RELEASE(self);
 result = DeeObject_CompareEqObject(value,v);
 Dee_DECREF(value);
 return result;
}
static int _deecell_tp_seq_iter_next(DeeCellObject *self, DeeObject **result) {
 DeeCell_ACQUIRE(self);
 *result = self->c_value;
 self->c_value = NULL;
 DeeCell_RELEASE(self);
 return *result ? 0 : 1;
}

static DeeObject *_deecell_get(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*fallback = NULL;
 if (DeeTuple_Unpack(args,"|o:get",&fallback) != 0) return NULL;
 if (fallback) {
  if ((result = DeeCell_TryGet((DeeObject *)self)) == NULL)
   DeeReturn_NEWREF(fallback);
  return result;
 }
 return DeeCell_Get((DeeObject *)self);
}
static DeeObject *_deecell_del(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":del") != 0) return NULL;
 DeeCell_Del((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *_deecell_pop(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*fallback = NULL;
 if (DeeTuple_Unpack(args,"|o:pop",&fallback) != 0) return NULL;
 if (!fallback) return DeeCell_Pop((DeeObject *)self);
 if ((result = DeeCell_TryPop((DeeObject *)self)) != NULL) return result;
 DeeReturn_NEWREF(fallback);
}
static DeeObject *_deecell_set(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *new_value;
 if (DeeTuple_Unpack(args,"o:set",&new_value) != 0) return NULL;
 DeeCell_Set((DeeObject *)self,new_value);
 DeeReturn_None;
}
static DeeObject *_deecell_exchange(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*new_value,*fallback = NULL;
 if (DeeTuple_Unpack(args,"o|o:exchange",&new_value,&fallback) != 0) return NULL;
 result = DeeCell_Exchange((DeeObject *)self,new_value);
 if (!result) {
  if (fallback) Dee_INCREF(result = fallback);
  else DeeError_Throw((DeeObject *)&_DeeError_EmptyCell);
 }
 return result;
}
static DeeObject *_deecell_at(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result; Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 if (index != 0) {idx: DeeError_IndexError((DeeObject *)self,index); return NULL; }
 DeeCell_ACQUIRE(self);
 Dee_XINCREF(result = self->c_value);
 DeeCell_RELEASE(self);
 if (!result) goto idx;
 return result;
}
static DeeObject *_deecell_back(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 return DeeCell_Get((DeeObject *)self);
}
static DeeObject *_deecell_front(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 return DeeCell_Get((DeeObject *)self);
}
static DeeObject *_deecell_contains(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*cellval,*elem,*pred = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:contains",&elem,&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 result = DeeNone_Check(pred)
  ? DeeObject_CompareEqObject(cellval,elem)
  : DeeObject_Callf(pred,"oo",cellval,elem);
 Dee_DECREF(cellval);
 if (!result && DeeError_Catch(&DeeErrorType_NotImplemented))
  result = DeeBool_NewFalse();
 return result;
}
static DeeObject *_deecell_count(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result; DeeObject *pred_result,*cellval,*elem,*pred = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:count",&elem,&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 pred_result = DeeNone_Check(pred)
  ? DeeObject_CompareEqObject(cellval,elem)
  : DeeObject_Callf(pred,"oo",cellval,elem);
 Dee_DECREF(cellval);
 if (pred_result) {
  result = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (result != 0 && DeeError_Catch(&DeeErrorType_NotImplemented)) result = 0;
 } else if (DeeError_Catch(&DeeErrorType_NotImplemented)) {
  result = 0;
 } else return NULL;
 return DeeObject_New(Dee_size_t,(Dee_size_t)result);
}
static DeeObject *_deecell_empty(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeCell_ACQUIRE(self);
 result = self->c_value == NULL;
 DeeCell_RELEASE(self);
 DeeReturn_Bool(result);
}
static DeeObject *_deecell_non_empty(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if (DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeCell_ACQUIRE(self);
 result = self->c_value != NULL;
 DeeCell_RELEASE(self);
 DeeReturn_Bool(result);
}
static DeeObject *_deecell_find(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result; DeeObject *pred_result,*cellval,*elem,*pred = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:find",&elem,&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 pred_result = DeeNone_Check(pred)
  ? DeeObject_CompareEqObject(cellval,elem)
  : DeeObject_Callf(pred,"oo",cellval,elem);
 Dee_DECREF(cellval);
 if (pred_result) {
  result = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (result != 0 && DeeError_Catch(&DeeErrorType_NotImplemented)) result = 0;
 } else if (DeeError_Catch(&DeeErrorType_NotImplemented)) {
  result = 0;
 } else return NULL;
 return DeeObject_New(Dee_ssize_t,result ? 0 : -1);
}
static DeeObject *_deecell_index(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result; DeeObject *pred_result,*cellval,*elem,*pred = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:index",&elem,&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 pred_result = DeeNone_Check(pred)
  ? DeeObject_CompareEqObject(cellval,elem)
  : DeeObject_Callf(pred,"oo",cellval,elem);
 Dee_DECREF(cellval);
 if (pred_result) {
  result = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (result != 0 && DeeError_Catch(&DeeErrorType_NotImplemented)) goto not_found;
 } else if (DeeError_Catch(&DeeErrorType_NotImplemented)) {
not_found: DeeError_ValueError_NotFound((DeeObject *)self,elem);
  return NULL;
 } else return NULL;
 if (!result) goto not_found;
 return DeeObject_New(Dee_size_t,0);
}
static DeeObject *_deecell_locate(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred,*cellval,*pred_result; int result;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 if (!cellval) goto not_found;
 pred_result = DeeObject_Callf(pred,"o",cellval);
 if (!pred_result) {err_cellval: Dee_DECREF(cellval); return NULL; }
 Dee_DECREF(cellval);
 result = DeeObject_Bool(pred_result);
 Dee_DECREF(pred_result);
 if (result < 0) goto err_cellval;
 if (result) return cellval;
 Dee_DECREF(cellval);
not_found:
 DeeError_ValueError_NotFound_Pred((DeeObject *)self,pred);
 return NULL;
}
static DeeObject *_deecell_locate_all(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred,*cellval,*pred_result; int result;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) DeeReturn_False;
 if (!cellval) goto not_found;
 pred_result = DeeObject_Callf(pred,"o",cellval);
 if (!pred_result) {err_cellval: Dee_DECREF(cellval); return NULL; }
 Dee_DECREF(cellval);
 result = DeeObject_Bool(pred_result);
 Dee_DECREF(pred_result);
 if (result < 0) goto err_cellval;
 if (result) { pred_result = DeeCell_New(cellval); Dee_DECREF(cellval); return pred_result; }
 Dee_DECREF(cellval);
not_found:
 return DeeCell_NewEmpty();
}
static DeeObject *_deecell_reversed(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":reversed") != 0) return NULL;
 return DeeObject_Copy((DeeObject *)self);
}
static DeeObject *_deecell_sorted(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"|o:sorted",&pred) != 0) return NULL;
 return DeeObject_Copy((DeeObject *)self); // Duh...
}
static DeeObject *_deecell_transform(
 DeeCellObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred,*cellval,*resultval,*result;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 if ((cellval = DeeCell_TryGet((DeeObject *)self)) == NULL) return DeeCell_NewEmpty();
 resultval = DeeObject_Callf(pred,"o",cellval);
 Dee_DECREF(cellval);
 if (!resultval) return NULL;
 result = DeeCell_New(resultval);
 Dee_DECREF(resultval);
 return result;
}


static struct DeeMethodDef const _deecell_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("get",member(&_deecell_get),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("del",member(&_deecell_del),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop",member(&_deecell_pop),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("set",member(&_deecell_set),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("exchange",member(&_deecell_exchange),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("at",member(&_deecell_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deecell_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deecell_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deecell_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deecell_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deecell_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deecell_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deecell_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deecell_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deecell_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deecell_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deecell_reversed),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deecell_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deecell_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deecell_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deecell_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deecell_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};


static DeeObject *_deecell_value_get(DeeCellObject *self, void *DEE_UNUSED(closure)) { return DeeCell_Get((DeeObject *)self); }
static int _deecell_value_del(DeeCellObject *self, void *DEE_UNUSED(closure)) { DeeCell_Del((DeeObject *)self); return 0; }
static int _deecell_value_set(DeeCellObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { DeeCell_Set((DeeObject *)self,v); return 0; }

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deecell_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deecell_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__c_value",DeeCellObject,c_value,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__c_lock",DeeCellObject,c_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeGetSetDef const _deecell_tp_getsets[] = {
 DEE_GETSETDEF_v100("value",
  member(&_deecell_value_get),member(&_deecell_value_del),
  member(&_deecell_value_set),"-> object\n"
  "@throws Error.ValueError: #(operator __get__) was called on an empty cell\n"
  "\tProperty-style access to the stored value"),
 DEE_GETSETDEF_END_v100
};


static int DEE_CALL _deecell_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint8_t cell_filled;
 if (DeeFile_Getc(infile,&cell_filled) != 0) return -1;
 if (!cell_filled) self->c_value = NULL;
 else if ((self->c_value = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 DeeAtomicMutex_Init(&self->c_lock);
 return 0;
}

static int DEE_CALL _deecell_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCellObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DeeObject *cellval; int result;
 cellval = DeeCell_TryGet((DeeObject *)self);
 if (DeeFile_Putc(outfile,(Dee_uint8_t)(cellval ? 1 : 0)) != 0) { Dee_XDECREF(cellval); return -1; }
 if (cellval) {
  result = DeeMarshal_WriteObjectWithMap(outfile,map,cellval);
  Dee_DECREF(cellval);
 } else result = 0;
 return result;
}

static struct DeeTypeMarshal _deecell_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(cell)),
 member(&_deecell_tp_marshal_ctor),member(&_deecell_tp_marshal_put));




DeeTypeObject DeeCell_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("cell"),member(
  "()\n"
  "(object stored_value)\n"
  "@param stored_value: The value stored inside the cell\n"
  "\tA cell is a builtin sequence type only capable of storing up one element.\n"
  "\tCells are mutable, allowing the stored element to be exchanged at any time.\n"
  "\tA cell not containing an object is referred to as an #(empty cell)\n"),member(
  DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_MUST_COPY|
  DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCellObject),member(&_DeeGC_TpAlloc),
  member(&_deecell_tp_ctor),member(&_deecell_tp_copy_ctor),
  member(&_deecell_tp_move_ctor),member(&_deecell_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deecell_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deecell_tp_copy_assign),member(&_deecell_tp_move_assign),
  member(&_deecell_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deecell_tp_str),
  member(&_deecell_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deecell_tp_visit),member(&_deecell_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deecell_tp_not),member(&_deecell_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deecell_tp_cmp_lo),member(&_deecell_tp_cmp_le),
  member(&_deecell_tp_cmp_eq),member(&_deecell_tp_cmp_ne),
  member(&_deecell_tp_cmp_gr),member(&_deecell_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deecell_tp_seq_get),member(&_deecell_tp_seq_del),
  member(&_deecell_tp_seq_set),member(&_deecell_tp_seq_size),
  member(&_deecell_tp_seq_contains),member(&_deecell_tp_seq_range_get),
  member(&_deecell_tp_seq_range_del),member(&_deecell_tp_seq_range_set),
  member(&DeeObject_Copy),member(&_deecell_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deecell_tp_members),member(_deecell_tp_getsets),
  member(_deecell_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(
  member(&_deecell_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_CELL_H */
