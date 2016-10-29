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
#ifndef GUARD_DEEMON_ITERATOR_H
#define GUARD_DEEMON_ITERATOR_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_DEX
#include <deemon/optional/object_decl.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeTupleIteratorObject);
DEE_OBJECT_DEF(DeeGenericRangeIteratorObject);
DEE_OBJECT_DEF(DeeGenericSequenceIteratorObject);
DEE_OBJECT_DEF(DeeSubRangeIteratorObject);
DEE_OBJECT_DEF(DeeZipIteratorObject);
DEE_OBJECT_DEF(DeeCatIteratorObject);
DEE_OBJECT_DEF(DeeEnumIteratorObject);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Special Iterators
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_DEX
struct DeeGenericRangeIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject *ri_iter; /*< [1..1] Low range value (also used as iterator) */
 DEE_A_REF DeeObject *ri_end;  /*< [1..1] High range value. */
 DEE_A_REF DeeObject *ri_step; /*< [0..1] Iterator step value (If NULL, use __inc__ instead of __iadd__). */
};
#endif /* DEE_LIMITED_DEX */

#define DeeGenericRangeIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeGenericRangeIterator_Type)
#define DeeGenericRangeIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeGenericRangeIterator_Type)
DEE_DATA_DECL(DeeTypeObject)  DeeGenericRangeIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Creates a generic range iterator
//  - The returned iterator can be re-used as an iterable,
//    returning a copy of itself in tp_seq_iter_self.
//  - 'begin' will be copied with DeeObject_Copy, as it is used as the iterator loop variable
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeGenericRangeIteratorObject) *) DeeGenericRangeIterator_New(
 DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end, DEE_A_IN_OPT DeeObject *step) DEE_ATTRIBUTE_NONNULL((1,2));


#ifdef DEE_LIMITED_DEX
struct DeeGenericSequenceIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject *si_seq;  /*< [1..1] Low range value (also used as iterator) */
 /*atomic*/Dee_size_t si_pos;  /*< Position inside the sequence. */
           Dee_size_t si_size; /*< Size of the sequence (a la 'tp_seq_size'). */
};
#endif /* DEE_LIMITED_DEX */

#define DeeGenericSequenceIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeGenericSequenceIterator_Type)
#define DeeGenericSequenceIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeGenericSequenceIterator_Type)
DEE_DATA_DECL(DeeTypeObject)  DeeGenericSequenceIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Creates a generic sequence iterator
//  - The returned iterator can be re-used as an iterable,
//    returning a copy of itself in tp_seq_iter_self.
//  - These iterators call the 'tp_seq_get' operator in the range of '0..#seq'
// NOTE: '#seq' is calculated only once in the constructor of such an iterator.
// NOTE: Set this function as 'tp_iter_self' to enable iterator support using this type
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeGenericSequenceIteratorObject) *)
 DeeGenericSequenceIterator_New(DEE_A_INOUT DeeObject *seq) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_DEX
struct DeeSubRangeIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject *sri_iter;  /*< [1..1] Inner iterator. */
 /*atomic*/Dee_size_t sri_pos;   /*< Current index. */
 Dee_size_t           sri_begin; /*< Begin index of items that should be yielded. */
 Dee_size_t           sri_end;   /*< End index of items that should be yielded. */
};
#endif /* DEE_LIMITED_DEX */

#define DeeSubRangeIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeSubRangeIterator_Type)
#define DeeSubRangeIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSubRangeIterator_Type)
DEE_DATA_DECL(DeeTypeObject)  DeeSubRangeIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Creates a sub-range iterator
//  - The returned iterator can be re-used as an iterable,
//    returning a copy of itself in tp_seq_iter_self.
//  - These iterators are used to implement a generic iterator-based tp_seq_range_get.
//  - Conforming to usual range standards, simply set
//    'begin = 0' for '[:end]' and 'end = (Dee_size_t)-1' for '[begin:]'
// They work like this:
// >> function subrange_impl(iter,begin,end) {
// >>   local pos = atomic(0);
// >>   __foreach(elem: iter) {
// >>     local my_index = pos++;
// >>     if (my_index >= end) return; // End of requested range
// >>     if (my_index >= begin) yield elem; // Index in requested range
// >>   }
// >> }
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSubRangeIteratorObject) *) DeeSubRangeIterator_New(
 DEE_A_INOUT DeeObject *iter, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1));




#ifdef DEE_LIMITED_DEX
struct DeeZipIteratorObject {
 DEE_OBJECT_HEAD
           Dee_size_t zi_iteratorc;
 DEE_A_REF DeeObject *zi_iteratorv[1024]; /*< Amount of contained iterators */
};
#define DeeZipIterator_SIZE(ob)   ((Dee_size_t)((DeeZipIteratorObject *)Dee_REQUIRES_POINTER(ob))->zi_iteratorc)
#define DeeZipIterator_ELEM(ob) ((DeeObject **)((DeeZipIteratorObject *)Dee_REQUIRES_POINTER(ob))->zi_iteratorv)
#endif /* DEE_LIMITED_DEX */

#define DeeZipIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeZipIterator_Type)
#define DeeZipIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeZipIterator_Type)
DEE_DATA_DECL(DeeTypeObject)  DeeZipIterator_Type;

struct _DeeEmptyZipIteratorObject { DEE_OBJECT_HEAD Dee_size_t _placeholder; };
DEE_DATA_DECL(struct _DeeEmptyZipIteratorObject) _Dee_EmptyZipIterator;
#define Dee_EmptyZipIterator       ((DeeObject*)&_Dee_EmptyZipIterator)

//////////////////////////////////////////////////////////////////////////
// Creates a zipped iterator from a given list of iterators/sequences
//  - The returned iterator can be re-used as an iterable,
//    returning a copy of itself through tp_seq_iter_self.
//  - It will yield tuples of elements from all iterators until at least one of its iterators is empty.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeZipIteratorObject) *)
 DeeZipIterator_New(DEE_A_IN Dee_size_t iterc, DEE_A_IN_R(iterc) DeeObject *const *iterv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeZipIteratorObject) *)
 DeeZipIterator_NewFromSeqv(DEE_A_IN Dee_size_t seqc, DEE_A_IN_R(seqc) DeeObject *const *seqv);



#if 0
#ifdef DEE_LIMITED_DEX
struct DeeCatIteratorObject {
 DEE_OBJECT_HEAD
 /*atomic*/Dee_size_t ci_iterpos;         /*< Index of the first non-empty iterator. */
           Dee_size_t ci_iteratorc;       /*< Amount of contained iterators. */
 DEE_A_REF DeeObject *ci_iteratorv[1024]; /*< Inlined vector of iterators. */
};
#define DeeCatIterator_ELEM(ob) ((DeeObject **)((DeeCatIteratorObject *)Dee_REQUIRES_POINTER(ob))->ci_iteratorv)
#define DeeCatIterator_SIZE(ob) ((Dee_VARSIZE(ob)-Dee_OFFSETOF(DeeCatIteratorObject,ci_iterators))/sizeof(DeeObject *))
#endif /* DEE_LIMITED_DEX */

#define DeeCatIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeCatIterator_Type)
#define DeeCatIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCatIterator_Type)
DEE_DATA_DECL(DeeTypeObject)  DeeCatIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Creates a concat iterator from a given list of iterators/sequences
//  - The returned iterator can be re-used as an iterable,
//    returning a copy of itself through tp_seq_iter_self.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCatIteratorObject) *)
 DeeCatIterator_New(DEE_A_IN Dee_size_t iterc, DEE_A_IN_R(iterc) DeeObject *const *iterv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCatIteratorObject) *)
 DeeCatIterator_NewFromSeqv(DEE_A_IN Dee_size_t seqc, DEE_A_IN_R(seqc) DeeObject *const *seqv);
#endif


#ifdef DEE_LIMITED_DEX
struct DeeEnumIteratorObject {
 DEE_OBJECT_HEAD
 /*atomic*/Dee_size_t ei_index;    /*< Current Iterator position (aka. first iteration element). */
 DEE_A_REF DeeObject *ei_iterator; /*< [1..1] Underlying enumeration iterator. */
};
#endif /* DEE_LIMITED_DEX */

#define DeeEnumIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeEnumIterator_Type)
#define DeeEnumIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeEnumIterator_Type)
DEE_DATA_DECL(DeeTypeObject) DeeEnumIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Creates a new enumeration iterator, that is an iterator that will
// yield 2-element tuples where the first element is an incrementing
// index starting at 0, or the given 'start_index' and the second element
// is the actual object yielded by the given iterator.
// >> function enum_iterator_impl(start_index,iterator) {
// >>   local index = atomic __SIZE_TYPE__(start_index);
// >>   __foreach(local elem: iterator) {
// >>     yield pack index++,elem;
// >>   }
// >> }
// NOTE: The iterator can be used as both an iterator, as well as an iterable
#define DeeEnumIterator_New(iterator) DeeEnumIterator_NewEx(0,iterator)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeEnumIteratorObject) *)
 DeeEnumIterator_NewEx(DEE_A_IN Dee_size_t start_index, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((2));

DEE_DECL_END

#endif /* !GUARD_DEEMON_ITERATOR_H */
