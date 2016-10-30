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
#ifndef GUARD_DEEMON_TRACEBACK_C
#define GUARD_DEEMON_TRACEBACK_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/compiler/code.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/execute.h>
#include <deemon/string.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTracebackObject) *
DeeTraceback_New(void) {
 DeeObject *result;
#ifndef DEE_WITHOUT_THREADS
 DeeThreadObject *thread_self;
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if ((result = _DeeTraceback_New(thread_self)) == NULL) DeeError_NoMemory();
#else /* !DEE_WITHOUT_THREADS */
 if ((result = _DeeTraceback_New(DeeThread_SELF())) == NULL) DeeError_NoMemory();
#endif /* DEE_WITHOUT_THREADS */
 return result;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTracebackObject) *
_DeeTraceback_New(DEE_A_IN struct DeeThreadObject *thread_self) {
 struct DeeStackFrame *framev;
 DeeTracebackObject *result;
 Dee_size_t framec;
#ifndef DEE_WITHOUT_THREADS
 DEE_ASSERT(DeeObject_Check(thread_self) && DeeThread_Check(thread_self));
#endif
 DEE_ASSERT(thread_self == DeeThread_SELF() && "Given thread is not thread.self()");
 // NOTE: No need to lock 'ob_frame_lock' since we are thread_self
 //       and don't modify it 't_frame_last'. (Only we could and we aren't)
 framev = thread_self->t_frame_last,framec = 0;
 while (framev) ++framec,framev = framev->f_prev;
 if (!framec) DeeReturn_EmptyTraceback;
#ifndef DEE_WITHOUT_THREADS
 result = (DeeTracebackObject *)DeeObject_TryMallocf(
  Dee_OFFSETOF(DeeTracebackObject,tb_tracebackv)+framec*sizeof(struct DeeTracebackEntry),
  "traceback for thread %u (%u frames)",(unsigned)DeeThread_ID((DeeObject *)thread_self),framec);
#else
 result = (DeeTracebackObject *)DeeObject_TryMallocf(
  Dee_OFFSETOF(DeeTracebackObject,tb_tracebackv)+framec*sizeof(struct DeeTracebackEntry),
  "traceback (%u frames)",framec);
#endif
 if (result) {
  struct DeeTracebackEntry *iter;
  DeeObject_INIT(result,&DeeTraceback_Type);
  result->tb_tracebackc = framec;
  iter = DeeTraceback_ELEM(result);
  framev = thread_self->t_frame_last;
  while (framev) {
   Dee_XINCREF(iter->te_file = (DeeUtf8StringObject *)_DeeStackFrame_File(framev));
   Dee_XINCREF(iter->te_func = (DeeUtf8StringObject *)_DeeStackFrame_Func(framev));
   iter->te_line = _DeeStackFrame_Line(framev);
   ++iter,framev = framev->f_prev;
  }
 }
 return (DeeObject *)result;
}


DEE_A_RET_WUNUSED Dee_size_t DeeTraceback_Size(
 DEE_A_IN_OBJECT(DeeTracebackObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTraceback_Check(self));
 return DeeTraceback_SIZE(self);
}
void DeeTraceback_Get(
 DEE_A_IN_OBJECT(DeeTracebackObject) const *self, DEE_A_IN Dee_size_t i,
 DEE_A_OUT_OBJECT_OPT(DeeStringObject) **file, DEE_A_OUT_OPT Dee_int64_t *line,
 DEE_A_OUT_OBJECT_OPT(DeeStringObject) **func) {
 struct DeeTracebackEntry *entry;
 DEE_ASSERT(DeeObject_Check(self) && DeeTraceback_Check(self));
 DEE_ASSERT(i < DeeTraceback_SIZE(self));
 entry = DeeTraceback_ELEM(self)+i;
 if (file) *file = (DeeObject *)entry->te_file;
 if (line) *line = entry->te_line;
 if (func) *func = (DeeObject *)entry->te_func;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeTracebackEntryObject) *DeeTracebackEntry_New(
 DEE_A_INOUT_OBJECT(DeeTracebackObject) *tb, DEE_A_IN struct DeeTracebackEntry const *entry) {
 DeeTracebackEntryObject *result;
 DEE_ASSERT(DeeObject_Check(tb) && DeeTraceback_Check(tb) && entry);
 if ((result = DeeObject_MALLOCF(DeeTracebackEntryObject,
  "Traceback entry for %p:%p",tb,entry)) != NULL) {
  DeeObject_INIT(result,&DeeTracebackEntry_Type);
  Dee_INCREF(result->te_traceback = (DeeTracebackObject *)tb);
  result->te_elem = entry;
 }
 return (DeeObject *)result;
}










//////////////////////////////////////////////////////////////////////////
// traceback VTable
static void _deetraceback_tp_dtor(DeeTracebackObject *self) {
 struct DeeTracebackEntry *iter,*end;
 end = (iter = DeeTraceback_ELEM(self))+DeeTraceback_SIZE(self);
 while (iter != end) {
  Dee_XDECREF(iter->te_func);
  Dee_XDECREF(iter->te_file);
  ++iter;
 }
}
DEE_VISIT_PROC(_deetraceback_tp_visit,DeeTracebackObject *self) {
 struct DeeTracebackEntry *iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeTraceback_Check(self));
 end = (iter = DeeTraceback_ELEM(self))+DeeTraceback_SIZE(self);
 while (iter != end) {
  Dee_XVISIT(iter->te_func);
  Dee_XVISIT(iter->te_file);
  ++iter;
 }
}
static DeeTracebackObject *_deetraceback_tp_copy_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTracebackObject *right) {
 DeeTracebackObject *result;
 struct DeeTracebackEntry *dst,*src,*src_end;
 result = (DeeTracebackObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeTracebackObject,tb_tracebackv)+
  DeeTraceback_SIZE(right)*sizeof(struct DeeTracebackEntry));
 if (!result) return NULL;
 DeeObject_INIT(result,&DeeTraceback_Type);
 result->tb_tracebackc = right->tb_tracebackc;
 dst = DeeTraceback_ELEM(result);
 src_end = (src = DeeTraceback_ELEM(right))+DeeTraceback_SIZE(right);
 while (src != src_end) {
  Dee_XINCREF(dst->te_func = src->te_func);
  Dee_XINCREF(dst->te_file = src->te_file);
  dst->te_line = src->te_line;
  ++dst,++src;
 }
 return 0;
}
static DeeObject *_deetraceback_tp_seq_size(DeeTracebackObject *self) {
 return DeeObject_New(Dee_size_t,DeeTraceback_SIZE(self));
}
static DeeObject *_deetraceback_tp_seq_get(DeeTracebackObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 DEE_PRIVATE_CLAMP_INDEX(ii,DeeTraceback_SIZE(self));
 return DeeTracebackEntry_New((DeeObject *)self,self->tb_tracebackv+ii);
}
DeeString_NEW_STATIC(_deetb_unknown_field,"??" "?");
static DeeObject *_deetraceback_tp_repr(DeeTracebackObject const *self) {
 struct DeeTracebackEntry *iter,*end;
 DeeObject *result;
 DeeStringWriter writer = DeeStringWriter_INIT();
 end = (iter = DeeTraceback_ELEM(self))+DeeTraceback_SIZE(self);
 while (iter != end) {
  if (DeeStringWriter_Writef(&writer,
   "%s(%I64d) : %s\n",
   iter->te_file ? DeeString_STR(iter->te_file) : "??" "?",iter->te_line+1,
   iter->te_func ? DeeString_STR(iter->te_func) : "??" "?") == -1) goto err;
  ++iter;
 }
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeTracebackIteratorObject *
_deetraceback_tp_seq_iter_self(DeeTracebackObject *self) {
 DeeTracebackIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTraceback_Check(self));
 if ((result = DeeObject_MALLOCF(DeeTracebackIteratorObject,
  "Traceback iterator for %p",self)) != NULL) {
  DeeObject_INIT(result,&DeeTracebackIterator_Type);
  Dee_INCREF(result->ti_traceback = (DeeTracebackObject *)self);
  result->ti_end = (result->ti_iter = DeeTraceback_ELEM(self))+DeeTraceback_SIZE(self);
 }
 return result;
}

static DeeTracebackObject *DEE_CALL _deetraceback_tp_marshal_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *map,
 DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeTracebackObject *result; Dee_size_t used_size; Dee_uint64_t read_size;
 if (DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Traceback is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 if (!read_size) { Dee_INCREF(Dee_EmptyTraceback); return (DeeTracebackObject *)Dee_EmptyTraceback; }
 used_size = (Dee_size_t)read_size;
 if ((result = (DeeTracebackObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeTracebackObject,tb_tracebackv)+used_size*sizeof(struct DeeTracebackEntry))) != NULL) {
  struct DeeTracebackEntry *iter,*end;
  DeeObject_INIT(result,&DeeTraceback_Type);
  result->tb_tracebackc = used_size;
  (*after_alloc)((DeeObject *)result,after_alloc_data);
  end = (iter = DeeTraceback_ELEM(result))+used_size;
  while (iter != end) {
   if ((iter->te_file = (DeeStringObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_elem:
    while (iter != DeeTraceback_ELEM(result)) {
     --iter;
     Dee_XDECREF(iter->te_file);
     Dee_XDECREF(iter->te_func);
    }
    _DeeObject_DELETE(&DeeTraceback_Type,result);
    return NULL;
   }
   if (DeeNone_Check(iter->te_file) || DeeString_CheckEmpty(iter->te_file)) Dee_CLEAR(iter->te_file);
   else if (DeeError_TypeError_CheckTypeExact((DeeObject *)iter->te_file,&DeeString_Type) != 0) goto err_elem;
   if (DeeFile_GetLe(infile,iter->te_line) != 0) {err_file_elem: Dee_XDECREF(iter->te_file); goto err_elem; }
   if ((iter->te_func = (struct DeeStringObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_file_elem;
   if (DeeNone_Check(iter->te_func) || DeeString_CheckEmpty(iter->te_func)) Dee_CLEAR(iter->te_func);
   else if (DeeError_TypeError_CheckTypeExact((DeeObject *)iter->te_func,&DeeString_Type) != 0) { Dee_DECREF(iter->te_func); goto err_file_elem; }
   ++iter;
  }
 }
 return result;
}

static int DEE_CALL _deetraceback_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint64_t written_size; struct DeeTracebackEntry *iter,*end;
 written_size = DeeTraceback_SIZE(self);
 if (DeeFile_PutLeSmall64(outfile,written_size) != 0) return -1;
 end = (iter = DeeTraceback_ELEM(self))+DeeTraceback_SIZE(self);
 while (iter != end) {
  if (DeeMarshal_WriteObjectWithMap(outfile,map,iter->te_file ? (DeeObject *)iter->te_file : Dee_None) != 0) return -1;
  if (DeeFile_PutLe(outfile,iter->te_line) != 0) return -1;
  if (DeeMarshal_WriteObjectWithMap(outfile,map,iter->te_func ? (DeeObject *)iter->te_func : Dee_None) != 0) return -1;
  ++iter;
 }
 return 0;
}

static struct DeeTypeMarshal _deetraceback_tp_marshal = DEE_TYPE_MARSHAL_VAR_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(traceback)),
 member(&_deetraceback_tp_marshal_new),member(&_deetraceback_tp_marshal_put));



//////////////////////////////////////////////////////////////////////////
// traceback.iterator VTable
static int _deetracebackiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) {
 Dee_INCREF(self->ti_traceback = right->ti_traceback);
 self->ti_iter = (struct DeeTracebackEntry *)DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst);
 self->ti_end = right->ti_end;
 return 0;
}
static void _deetracebackiterator_tp_dtor(DeeTracebackIteratorObject *self) {
 Dee_DECREF(self->ti_traceback);
}
DEE_VISIT_PROC(_deetracebackiterator_tp_visit,DeeTracebackIteratorObject *self) {
 Dee_VISIT(self->ti_traceback);
}
static int _deetracebackiterator_tp_copy_assign(
 DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) {
 if (self != right) {
  _deetracebackiterator_tp_dtor(self);
  _deetracebackiterator_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
static DeeObject *_deetracebackiterator_tp_cmp_lo(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) <  DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static DeeObject *_deetracebackiterator_tp_cmp_le(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) <= DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static DeeObject *_deetracebackiterator_tp_cmp_eq(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) == DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static DeeObject *_deetracebackiterator_tp_cmp_ne(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) != DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static DeeObject *_deetracebackiterator_tp_cmp_gr(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) >  DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static DeeObject *_deetracebackiterator_tp_cmp_ge(DeeTracebackIteratorObject *self, DeeTracebackIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst) >= DeeAtomicPtr_Load(right->ti_iter,memory_order_seq_cst)); }
static int _deetracebackiterator_tp_seq_iter_next(DeeTracebackIteratorObject *self, DeeObject **result) {
 struct DeeTracebackEntry *entry_pos;
 entry_pos = (struct DeeTracebackEntry *)DeeAtomicPtr_FetchAdd(
  self->ti_iter,sizeof(struct DeeTracebackEntry),memory_order_seq_cst);
 if (entry_pos >= self->ti_end || entry_pos < DeeTraceback_ELEM(self->ti_traceback)) return 1;
 if ((*result = DeeTracebackEntry_New((
  DeeObject *)self->ti_traceback,entry_pos)) == NULL) return -1;
 return 0;
}

static int DEE_CALL _deetracebackiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTracebackIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_pos;
 if (DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Traceback iterator position is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
#endif
 if ((self->ti_traceback = (DeeTracebackObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeNone_Check(self->ti_traceback)) { Dee_DECREF(self->ti_traceback); Dee_INCREF(self->ti_traceback = (DeeTracebackObject *)Dee_EmptyTraceback); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->ti_traceback,&DeeTraceback_Type) != 0) { Dee_DECREF(self->ti_traceback); return -1; }
 self->ti_end = DeeTraceback_ELEM(self->ti_traceback)+DeeTraceback_SIZE(self->ti_traceback);
 self->ti_iter = DeeTraceback_ELEM(self->ti_traceback)+(Dee_size_t)read_pos;
 return 0;
}

static int DEE_CALL _deetracebackiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTracebackIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct DeeTracebackEntry *entry_pos; Dee_uint64_t written_pos;
 entry_pos = (struct DeeTracebackEntry *)DeeAtomicPtr_Load(self->ti_iter,memory_order_seq_cst);
 written_pos = (Dee_uint64_t)((Dee_size_t)(entry_pos-DeeTraceback_ELEM(self->ti_traceback)));
 if (DeeFile_PutLeSmall64(outfile,written_pos) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,DeeTraceback_CheckEmpty(self->ti_traceback)
                                      ? Dee_None : (DeeObject *)self->ti_traceback);
}

static struct DeeTypeMarshal _deetracebackiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(traceback_iterator)),
 member(&_deetracebackiterator_tp_marshal_ctor),
 member(&_deetracebackiterator_tp_marshal_put));



//////////////////////////////////////////////////////////////////////////
// traceback.entry VTable
static int _deetracebackentry_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) {
 Dee_INCREF(self->te_traceback = right->te_traceback);
 self->te_elem = right->te_elem;
 return 0;
}
static void _deetracebackentry_tp_dtor(DeeTracebackEntryObject *self) {
 Dee_DECREF(self->te_traceback);
}
DEE_VISIT_PROC(_deetracebackentry_tp_visit,DeeTracebackEntryObject *self) {
 Dee_VISIT(self->te_traceback);
}
static int _deetracebackentry_tp_copy_assign(
 DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) {
 if (self != right) {
  _deetracebackentry_tp_dtor(self);
  _deetracebackentry_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
static DeeObject *_deetracebackentry_tp_str(DeeTracebackEntryObject *self) {
 return DeeString_Newf("%k(%I64d) : %k\n",
                       self->te_elem->te_file ? (DeeObject *)self->te_elem->te_file : (DeeObject *)&_deetb_unknown_field,
                       self->te_elem->te_line+1,
                       self->te_elem->te_func ? (DeeObject *)self->te_elem->te_func : (DeeObject *)&_deetb_unknown_field);
}
static DeeObject *_deetracebackentry_tp_cmp_lo(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem < right->te_elem); }
static DeeObject *_deetracebackentry_tp_cmp_le(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem <= right->te_elem); }
static DeeObject *_deetracebackentry_tp_cmp_eq(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem == right->te_elem); }
static DeeObject *_deetracebackentry_tp_cmp_ne(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem != right->te_elem); }
static DeeObject *_deetracebackentry_tp_cmp_gr(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem > right->te_elem); }
static DeeObject *_deetracebackentry_tp_cmp_ge(DeeTracebackEntryObject *self, DeeTracebackEntryObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeTracebackEntry_Type) == -1) return NULL; DeeReturn_Bool(self->te_elem >= right->te_elem); }
static DeeObject *_deetracebackentry_file(DeeTracebackEntryObject *self, void *DEE_UNUSED(closure)) { if (!self->te_elem->te_file) DeeReturn_EmptyString; DeeReturn_NEWREF(self->te_elem->te_file); }
static DeeObject *_deetracebackentry_line(DeeTracebackEntryObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(Dee_int64_t,self->te_elem->te_line); }
static DeeObject *_deetracebackentry_func(DeeTracebackEntryObject *self, void *DEE_UNUSED(closure)) { if (!self->te_elem->te_func) DeeReturn_EmptyString; DeeReturn_NEWREF(self->te_elem->te_func); }
static int DEE_CALL _deetracebackentry_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTracebackEntryObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_pos;
 if (DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Traceback entry position is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
#endif
 if ((self->te_traceback = (DeeTracebackObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeNone_Check(self->te_traceback)) { Dee_DECREF(self->te_traceback); Dee_INCREF(self->te_traceback = (DeeTracebackObject *)Dee_EmptyTraceback); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->te_traceback,&DeeTraceback_Type) != 0) { Dee_DECREF(self->te_traceback); return -1; }
 self->te_elem = DeeTraceback_ELEM(self->te_traceback)+(Dee_size_t)read_pos;
 return 0;
}

static int DEE_CALL _deetracebackentry_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTracebackEntryObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)((Dee_size_t)(
  self->te_elem-DeeTraceback_ELEM(self->te_traceback)))) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,DeeTraceback_CheckEmpty(self->te_traceback)
                                      ? Dee_None : (DeeObject *)self->te_traceback);
}

static struct DeeTypeMarshal _deetracebackentry_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(traceback_entry)),
 member(&_deetracebackentry_tp_marshal_ctor),
 member(&_deetracebackentry_tp_marshal_put));




static struct DeeMemberDef _deetraceback_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeTracebackIterator_Type),
 DEE_MEMBERDEF_CONST_v100("entry",object,&DeeTracebackEntry_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef _deetraceback_tp_methods[] = {
 // todo: Intrinsic support
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),NULL),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),NULL),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),NULL),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),NULL),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),NULL),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),NULL),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),NULL),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),NULL),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),NULL),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),NULL),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),NULL),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef _deetracebackentry_tp_getsets[] = {
 DEE_GETSETDEF_v100("file",member(&_deetracebackentry_file),null,null,"-> string"),
 DEE_GETSETDEF_v100("func",member(&_deetracebackentry_func),null,null,"-> string"),
 DEE_GETSETDEF_v100("line",member(&_deetracebackentry_line),null,null,"-> int64_t"),
 DEE_GETSETDEF_END_v100
};

struct _DeeEmptyTracebackObject _Dee_EmptyTraceback = {
 DEE_OBJECT_HEAD_INIT(&DeeTraceback_Type),0};
DeeTypeObject DeeTraceback_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("traceback"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_VAR_OBJECT|
  DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,
  member(&_deetraceback_tp_copy_new),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetraceback_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetraceback_tp_repr),
  member(&_deetraceback_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetraceback_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deetraceback_tp_seq_get),null,null,
  member(&_deetraceback_tp_seq_size),
  member(&_deegenericiterable_tp_seq_contains),
  member(&_deegenericiterable_tp_seq_range_get),null,null,
  member(&_deetraceback_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deetraceback_tp_methods),
  member(_deetraceback_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetraceback_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



DeeTypeObject DeeTracebackIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("traceback.iterator"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTracebackIteratorObject),null,null,
  member(&_deetracebackiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetracebackiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deetracebackiterator_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetracebackiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetracebackiterator_tp_cmp_lo),
  member(&_deetracebackiterator_tp_cmp_le),
  member(&_deetracebackiterator_tp_cmp_eq),
  member(&_deetracebackiterator_tp_cmp_ne),
  member(&_deetracebackiterator_tp_cmp_gr),
  member(&_deetracebackiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,null,
  member(&_deetracebackiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetracebackiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeTracebackEntry_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("traceback.entry"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTracebackIteratorObject),null,null,
  member(&_deetracebackentry_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetracebackentry_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deetracebackentry_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetracebackentry_tp_str),
  member(&_deetracebackentry_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetracebackentry_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetracebackentry_tp_cmp_lo),member(&_deetracebackentry_tp_cmp_le),
  member(&_deetracebackentry_tp_cmp_eq),member(&_deetracebackentry_tp_cmp_ne),
  member(&_deetracebackentry_tp_cmp_gr),member(&_deetracebackentry_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deetracebackentry_tp_getsets),null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetracebackentry_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_TRACEBACK_C */
