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
#ifndef GUARD_DEEMON_ZIP_ITERATOR_INL
#define GUARD_DEEMON_ZIP_ITERATOR_INL 1
#define DEE_LIMITED_API 1

#ifdef __INTELLISENSE__
#include "tuple.c"
#endif

DEE_DECL_BEGIN

#define _DeeZipIterator_FreeUnsafe(ob) \
 _DeeObject_DELETE(&DeeZipIterator_Type,ob)

DeeObject *_DeeZipIterator_NewUnsafe(Dee_size_t iterc) {
 DeeTupleObject *result;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **pool;
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 if DEE_UNLIKELY(!iterc) DeeReturn_NEWREF(Dee_EmptyZipIterator);
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 if DEE_LIKELY(iterc < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  if DEE_LIKELY((*(pool = _DeeTuplePool_Pool+iterc)) != NULL) {
   result = (DeeTupleObject *)*pool;
   *pool = (*pool)->tp_next;
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
  } else {
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
   goto alloc_new;
  }
 } else
 {
alloc_new:
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
  if DEE_UNLIKELY((result = (DeeTupleObject *)DeeObject_Mallocf(
   DEE_PRIVATE_SIZEOF_TUPLE(iterc),
   "zip_iterator(size: %lu)",(unsigned long)iterc)) == NULL) return NULL;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 }
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DeeObject_INIT(result,&DeeZipIterator_Type);
 result->t_size = iterc;
 return (DeeObject *)result;
}
void _DeeZipIterator_Free(DeeObject *self) {
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 Dee_size_t tuple_size;
 DEE_ASSERT(DeeTuple_SIZE(self) != 0);
 if DEE_LIKELY((tuple_size = DeeTuple_SIZE(self)) < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  ((union _DeeTuplePool *)self)->tp_next = _DeeTuplePool_Pool[tuple_size];
  _DeeTuplePool_Pool[tuple_size] = ((union _DeeTuplePool *)self);
  DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
 } else
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 {
  free_nn(self);
 }
}


DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeZipIteratorObject) *DeeZipIterator_New(
 DEE_A_IN Dee_size_t iterc, DEE_A_IN_R(iterc) DeeObject *const *iterv) {
 DeeZipIteratorObject *result; DeeObject **iter,**end;
 if DEE_UNLIKELY((result = (DeeZipIteratorObject *)_DeeZipIterator_NewUnsafe(iterc)) == NULL) return NULL;
 end = (iter = DeeZipIterator_ELEM(result))+iterc;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *iterv++);
 return (DeeObject *)result;
}
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeZipIteratorObject) *DeeZipIterator_NewFromSeqv(
 DEE_A_IN Dee_size_t seqc, DEE_A_IN_R(seqc) DeeObject *const *seqv) {
 DeeZipIteratorObject *result; DeeObject **iter,**end,*seqiter;
 if DEE_UNLIKELY((result = (DeeZipIteratorObject *)_DeeZipIterator_NewUnsafe(seqc)) == NULL) return NULL;
 end = (iter = DeeZipIterator_ELEM(result))+seqc;
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((seqiter = DeeObject_IterSelf(*seqv)) == NULL) {
   while DEE_LIKELY(iter != DeeZipIterator_ELEM(result)) Dee_DECREF(*--iter);
   _DeeZipIterator_FreeUnsafe(result);
   return NULL;
  }
  Dee_INHERIT_REF(*iter++,seqiter);
  ++seqv;
 }
 return (DeeObject *)result;
}

static DeeObject *DEE_CALL _deezipiterator_tp_any_new(DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args) {
 return DeeZipIterator_NewFromSeqv(DeeTuple_SIZE(args),DeeTuple_ELEM(args));
}
static DeeObject *DEE_CALL _deezipiterator_tp_copy_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeZipIteratorObject *self) {
 return DeeZipIterator_New(DeeZipIterator_SIZE(self),DeeZipIterator_ELEM(self));
}
static int DEE_CALL _deezipiterator_tp_seq_iter_next(DeeObject *self, DeeObject **result) {
 DeeObject **iter,**end,**dst,*iterelem; Dee_size_t iterc; int error;
 if DEE_UNLIKELY((iterc = DeeZipIterator_SIZE(self)) == 0) return 1;
 if DEE_UNLIKELY((*result = _DeeTuple_NewUnsafe(iterc)) == NULL) return -1;
 dst = DeeTuple_ELEM(*result);
 end = (iter = DeeZipIterator_ELEM(self))+iterc;
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((error = DeeObject_IterNextEx(*iter,&iterelem)) != 0) { // Error or end of iterator
   while DEE_LIKELY(dst != DeeTuple_ELEM(*result)) Dee_DECREF(*--dst);
   _DeeTuple_FreeUnsafe(*result);
   return error; // Forward the error
  }
  *dst++ = iterelem; // Inherit reference
  ++iter;
 }
 return 0;
}


static DeeObject *DEE_CALL _deezipiterator_tp_marshal_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *map,
 DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeObject *result; Dee_size_t used_size; Dee_uint64_t read_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Zip-iterator is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 used_size = (Dee_size_t)read_size;
 if DEE_LIKELY((result = _DeeZipIterator_NewUnsafe(used_size)) != NULL) {
  (*after_alloc)(result,after_alloc_data);
  if (DeeMarshal_ReadObjectsWithMap(infile,map,used_size,DeeZipIterator_ELEM(result)) != 0) {
   _DeeZipIterator_FreeUnsafe(result);
   result = NULL;
  }
 }
 return result;
}

#if 1
// It's entirely the same thing...
#define _deezipiterator_tp_marshal_put _deetuple_tp_marshal_put
#else
static int DEE_CALL _deezipiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint64_t written_size;
 written_size = DeeZipIterator_SIZE(self);
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_size) != 0) return -1;
 return DeeMarshal_WriteObjectsWithMap(outfile,map,DeeZipIterator_SIZE(self),DeeZipIterator_ELEM(self));
}
#endif

static struct DeeTypeMarshal _deezipiterator_tp_marshal = DEE_TYPE_MARSHAL_VAR_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(zip_iterator)),
 member(&_deezipiterator_tp_marshal_new),member(&_deezipiterator_tp_marshal_put));

struct _DeeEmptyZipIteratorObject _Dee_EmptyZipIterator = {
 DEE_OBJECT_HEAD_INIT(&DeeZipIterator_Type),0};
DeeTypeObject DeeZipIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("zipiterator"),
  member("(iterable sequences...) -> zipiterator"),member(
  DEE_TYPE_FLAG_VAR_OBJECT),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(
  null,member(&_deezipiterator_tp_copy_new),
  null,member(&_deezipiterator_tp_any_new)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deetuple_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetuple_tp_str),member(&_deetuple_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetuple_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deetuple_tp_not),member(&_deetuple_tp_bool),
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetuple_tp_cmp_lo),member(&_deetuple_tp_cmp_le),
  member(&_deetuple_tp_cmp_eq),member(&_deetuple_tp_cmp_ne),
  member(&_deetuple_tp_cmp_gr),member(&_deetuple_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,member(&DeeObject_Copy),
  member(&_deezipiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_ZIP_ITERATOR_INL */
