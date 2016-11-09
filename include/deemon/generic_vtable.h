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
#ifndef GUARD_DEEMON_GENERIC_VTABLE_H
#define GUARD_DEEMON_GENERIC_VTABLE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

#ifdef DEE_LIMITED_API
extern int DEE_CALL _deegenericsameob_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_le(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericsameob_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs);
extern int DEE_CALL _deegenericmemcmp_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_le(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs);
extern DeeObject *DEE_CALL _deegenericmemcmp_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs);
#endif

#ifdef DEE_LIMITED_DEX
//////////////////////////////////////////////////////////////////////////
// Uses the iterator interface on lhs and rhs to compare their elements against each other
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_le(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_seq_size(DeeObject *self);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_seq_get(DeeObject *self, DeeObject *i);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_seq_range_get(DeeObject *self, DeeObject *lo, DeeObject *hi);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_tp_seq_contains(DeeObject *self, DeeObject *ob);
#endif

#ifdef DEE_LIMITED_DEX
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_at(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_back(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_count(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_empty(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_find(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_front(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_index(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_locate(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_locate_all(DeeObject *self, DeeObject *args, void *closure); // Returns a list
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_non_empty(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_reversed(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_rfind(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_rindex(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_rlocate(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_sorted(DeeObject *self, DeeObject *args, void *closure);
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_transform(DeeObject *self, DeeObject *args, void *closure); // Returns a list
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_segments(DeeObject *self, DeeObject *args, void *closure); // Returns a list
DEE_FUNC_DECL(DeeObject *) _deegenericiterable_contains(DeeObject *self, DeeObject *args, void *closure);

/*>> For instant iterable support:
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
*/
#endif

#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Returns a '[elema,elemb,...]' style representation for any iterable object
extern DeeObject *_deegenericlist_tp_str(DeeObject *self);

typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *_DeeGeneric_MergeSortPred)(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b, void *closure);

// Warning: This function doesn't give jack shit about tracking references
// >> So when you call it, know that all references in 'in_v' get transferred to 'out_v'
// NOTE: mergesort always transfers all data from 'in_v' to 'out_v', even if an error occurred
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeGeneric_MergeSort(
 DEE_A_IN Dee_size_t c,
 DEE_A_ON_ALWAYS(DEE_A_IN_R(c)) DeeObject *const *in_v,
 DEE_A_ON_ALWAYS(DEE_A_OUT_W(c)) DeeObject **out_v,
 DEE_A_IN _DeeGeneric_MergeSortPred pred_lo, void *closure) ;
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeGeneric_SortPredCmpLo(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b, void *closure);
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeGeneric_SortPredUserFunc(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b, void *closure); // 'closure' is a callable 'DeeObject *'

#endif /* DEE_LIMITED_API */

#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif



//////////////////////////////////////////////////////////////////////////
// Merges the elements of two iterators into a list.
// >> If both iterators were sorted before, the new sequence is sorted as well
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_MergeIterators(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_MergeIteratorsPred(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Returns a list of elements present in both given iterators
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_CommonIterators(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_CommonIteratorsPred(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Generic sequence functions that work on everything that is iterable
// WARNING: - Objects of infinite size, such as 'none' will cause
//            these functions to go into an infinite loop.
//          - But don't worry if you're calling from user code!
//            Because calling something like 'none.find("foo")',
//            you'll just get 'none' returned as none overwrites __getattr__.
// NOTE: You don't have to check if 'self' is iterable, as these functions only require 'DeeObject_Check(self)' to be true
#define /* DEE_A_EXEC Dee_ssize_t */DeeGeneric_IterableFind(ob,findob) DeeGeneric_TIterableFind(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_ssize_t */DeeGeneric_IterableRFind(ob,findob) DeeGeneric_TIterableRFind(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableIndex(ob,findob) DeeGeneric_TIterableIndex(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableRIndex(ob,findob) DeeGeneric_TIterableRIndex(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableCount(ob,findob) DeeGeneric_TIterableCount(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_ssize_t */DeeGeneric_IterableFindPred(ob,findob,pred) DeeGeneric_TIterableFindPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_ssize_t */DeeGeneric_IterableRFindPred(ob,findob,pred) DeeGeneric_TIterableRFindPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableIndexPred(ob,findob,pred) DeeGeneric_TIterableIndexPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableRIndexPred(ob,findob,pred) DeeGeneric_TIterableRIndexPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeGeneric_IterableCountPred(ob,findob,pred) DeeGeneric_TIterableCountPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableLocate(ob,pred) DeeGeneric_TIterableLocate(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableRLocate(ob,pred) DeeGeneric_TIterableRLocate(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableLocateAll(ob,pred) DeeGeneric_TIterableLocateAll(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableTransform(ob,pred) DeeGeneric_TIterableTransform(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC int */DeeGeneric_IterableEmpty(ob) DeeGeneric_TIterableEmpty(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeGeneric_IterableNonEmpty(ob) DeeGeneric_TIterableNonEmpty(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableFront(ob) DeeGeneric_TIterableFront(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableBack(ob) DeeGeneric_TIterableBack(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableGet(ob,idx) DeeGeneric_TIterableGet(Dee_TYPE(ob),ob,idx)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableAt(ob,idx) DeeGeneric_TIterableAt(Dee_TYPE(ob),ob,idx)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableReversed(ob) DeeGeneric_TIterableReversed(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableSorted(ob) DeeGeneric_TIterableSorted(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableSortedPred(ob,pred) DeeGeneric_TIterableSortedPred(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeGeneric_IterableSegments(ob,n) DeeGeneric_TIterableSegments(Dee_TYPE(ob),ob,n)
#define /* DEE_A_EXEC int */DeeGeneric_IterableContains(ob,elem) DeeGeneric_TIterableContains(Dee_TYPE(ob),ob,elem)
#define /* DEE_A_EXEC int */DeeGeneric_IterableContainsPred(ob,elem,pred) DeeGeneric_TIterableContainsPred(Dee_TYPE(ob),ob,elem,pred)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeGeneric_TIterableFind(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeGeneric_TIterableRFind(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableRIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableCount(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeGeneric_TIterableFindPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeGeneric_TIterableRFindPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableIndexPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableRIndexPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeGeneric_TIterableCountPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableLocate(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableRLocate(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableLocateAll(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableTransform(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_TIterableEmpty(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_TIterableNonEmpty(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableFront(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableBack(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableGet(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_ssize_t idx) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableAt(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t idx) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableReversed(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableSorted(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableSortedPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeGeneric_TIterableSegments(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_TIterableContains(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_TIterableContainsPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));



DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVLo(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVLe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVEq(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVNe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVGr(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVVGe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVILo(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVILe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVIEq(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVINe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVIGr(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareVIGe(DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVLo(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVLe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVEq(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVNe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVGr(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIVGe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIILo(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIILe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIIEq(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIINe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIIGr(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeGeneric_CompareIIGe(DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) DEE_ATTRIBUTE_NONNULL((1,2));


DEE_DECL_END

#endif /* !GUARD_DEEMON_GENERIC_VTABLE_H */
