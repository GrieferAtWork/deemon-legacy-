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
#ifndef GUARD_DEEMON_TUPLE_H
#define GUARD_DEEMON_TUPLE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/std/stdarg.h>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

#if DEE_DEPRECATED_API_VERSION(100,101,103)
#include <deemon/object.h>
#include <deemon/type.h>
#endif


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

DEE_OBJECT_DEF(DeeTupleObject);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Tuple (An immutable inplace-vector)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_API
struct DeeTupleObject {
#ifdef __INTELLISENSE__
 DEE_OBJECT_HEAD;
            Dee_size_t t_size;       /*< Tuple size. */
 DEE_A_REF; DeeObject *t_elem[1024]; /*< [1..1][t_size] Elements. */
#else
 DEE_OBJECT_HEAD
           Dee_size_t t_size;       /*< Tuple size. */
 DEE_A_REF DeeObject *t_elem[1024]; /*< [1..1][t_size] Elements. */
#endif
};
#define DeeTuple_SIZE(ob)      ((Dee_size_t)((DeeTupleObject*)(ob))->t_size)
#define DeeTuple_ELEM(ob)    ((DeeObject **)((DeeTupleObject*)(ob))->t_elem)
#define DeeTuple_GET(ob,i)                 (((DeeTupleObject*)(ob))->t_elem[i])
#define DeeTuple_SET(ob,i,v)         (void)(((DeeTupleObject*)(ob))->t_elem[i]=(v))
#else
#define DeeTuple_SIZE(ob)            DeeTuple_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#define DeeTuple_ELEM(ob)           _DeeTuple_Elem((DeeObject*)Dee_REQUIRES_POINTER(ob))
#define DeeTuple_GET(ob,i)         (_DeeTuple_Elem((DeeObject*)Dee_REQUIRES_POINTER(ob))[i])
#define DeeTuple_SET(ob,i,v) (void)(_DeeTuple_Elem((DeeObject*)Dee_REQUIRES_POINTER(ob))[i]=(v))
#endif
#define DeeTuple_EMPTY             DeeTuple_CheckEmpty


DEE_DATA_DECL(DeeTypeObject)      DeeTuple_Type;
#define DeeTuple_CheckExact(ob)   DeeObject_InstanceOfExact(ob,&DeeTuple_Type)
#define DeeTuple_Check            DeeTuple_CheckExact
#define DeeTuple_CheckEmpty(ob) ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_EmptyTuple)

struct _DeeEmptyTupleObject { DEE_OBJECT_HEAD Dee_size_t _placeholder; };
DEE_DATA_DECL(struct _DeeEmptyTupleObject)       _Dee_EmptyTuple;
#define Dee_EmptyTuple             ((DeeObject*)&_Dee_EmptyTuple)
#define DeeReturn_EmptyTuple return _DeeObject_INCREF(Dee_EmptyTuple),Dee_EmptyTuple
#define DeeTuple_NewEmpty()        (_DeeObject_INCREF(Dee_EmptyTuple),Dee_EmptyTuple)


#ifdef DEE_LIMITED_API
struct DeeTupleIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeTupleObject *ti_tuple; /*<       [1..1] Associated tuple (keep-alive reference) */
 /*atomic*/DeeObject     **ti_pos;   /*< [?..?][1..1] Current position in tuple. */
           DeeObject     **ti_end;   /*< [?..?][1..1] End of tuple. */
};
#define DeeTupleIterator_TUPLE(ob) ((DeeTupleIteratorObject *)Dee_REQUIRES_POINTER(ob))->ti_tuple
#define DeeTupleIterator_POS(ob)   ((DeeTupleIteratorObject *)Dee_REQUIRES_POINTER(ob))->ti_pos
#define DeeTupleIterator_END(ob)   ((DeeTupleIteratorObject *)Dee_REQUIRES_POINTER(ob))->ti_end
#endif

#define DeeTupleIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeTupleIterator_Type)
#define DeeTupleIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTupleIterator_Type)
DEE_DATA_DECL(DeeTypeObject) DeeTupleIterator_Type;


struct DeeTupleIteratorObject;
struct DeeListObject;

#ifdef DEE_LIMITED_API
#define _DeeTuple_FreeUnsafe(ob)  _DeeObject_DELETE(&DeeTuple_Type,ob)
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *_DeeTuple_NewUnsafe(DEE_A_IN Dee_size_t n);
extern DEE_A_EXEC DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTupleObject) *_DeeTuple_TryNewUnsafe(DEE_A_IN Dee_size_t n);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *_DeeTuple_WeakNewUnsafe(DEE_A_IN Dee_size_t n);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTupleObject) *_DeeTuple_TryWeakNewUnsafe(DEE_A_IN Dee_size_t n);
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeTuple_ResizeUnsafe(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
#endif

//////////////////////////////////////////////////////////////////////////
// Public tuple API
#define /* DEE_A_EXEC DeeTupleObject * */DeeTuple_NewFromSequence  DeeTuple_Cast
#define /* DEE_A_EXEC DeeTupleObject * */DeeTuple_TNewFromSequence DeeTuple_TCast
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_NewFromIterator(DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_NewFromVector(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_Resize(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN Dee_size_t n, DEE_A_INOUT DeeObject *filler) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_Append(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_AppendVector(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC int */DeeTuple_AppendIterator(ob,iterator) DeeTuple_TAppendIterator(ob,Dee_TYPE(iterator),iterator)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_TAppendIterator(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN DeeTypeObject const *tp_iterator, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
#define /* DEE_A_EXEC int */DeeTuple_AppendSequence(ob,sequence) DeeTuple_TAppendSequence(ob,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_TAppendSequence(DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_DeepCopy(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeTuple_Size(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeObject **) _DeeTuple_Elem(DEE_A_IN_OBJECT(DeeTupleObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) *) DeeTuple_AsList(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC DeeTupleObject * */DeeTuple_Concat(lhs,rhs) DeeTuple_TConcat(lhs,Dee_TYPE(rhs),rhs)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeTuple_TConcat(DEE_A_IN_OBJECT(DeeTupleObject) const *lhs, DEE_A_IN DeeTypeObject const *tp_rhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* DEE_A_EXEC DeeTupleObject * */DeeTuple_Cast(ob) DeeTuple_TCast(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_TCast(DEE_A_IN DeeTypeObject const *tp_seq, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_Pack(DEE_A_IN Dee_size_t n, ...); //< Pack a tuple of n "DeeObject*" varargs
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) _DeeTuple_VPack(DEE_A_IN Dee_size_t n, DEE_A_INOUT va_list args);
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


//////////////////////////////////////////////////////////////////////////
// Returns the reversed version of 'self'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *)
 DeeTuple_Reversed(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the sorted version of 'self'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *)
 DeeTuple_Sorted(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_SortedPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));


#if DEE_DEPRECATED_API_VERSION(100,101,103)
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("'DeeTuple_Copy()' is deprecated. Use 'DeeObject_Copy' instead")
 DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_Copy(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 return DeeObject_Copy((DeeObject *)self);
}
#endif

#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeTuple_Hash(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
#endif


//////////////////////////////////////////////////////////////////////////
// Find an element in a tuple
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeTuple_Find(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeTuple_RFind(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_Index(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_RIndex(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_Count(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Find an element in a tuple (with a predicate)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeTuple_FindPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeTuple_RFindPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_IndexPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_RIndexPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeTuple_CountPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));


#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleIteratorObject) *DeeTuple_IterSelf(
 DEE_A_IN_OBJECT(DeeTupleObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#endif

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#undef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#endif

DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *)
 DeeTuple_Front(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *)
 DeeTuple_Back(DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *)
 DeeTuple_Get(DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_ssize_t i) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_DEPRECATED_API_VERSION(101,102,104)
#define DeeTuple_GetIndex(ob,index) DeeTuple_Get(ob,(Dee_ssize_t)(index))
#endif
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *)
 DeeTuple_At(DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_GetRangeIndex(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC_IF((*func)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeTuple_Traverse(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN DeeObjectIterateFunc func, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Locate the first element for which 'pred(elem)' is true
// WARNING: Look at the return specs; these functions don't return references!
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *) DeeTuple_Locate(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeObject *) DeeTuple_RLocate(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_LocateAll(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns a new list with all elements of 'self' transformed with 'pred(elem)'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_Transform(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeTuple_Segments(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_API
// NOTE: 'ob' is allowed to be a tuple, but there is no optimization happening if it is.
//       So you should only use this, if you're pretty sure it isn't going to be a tuple.
//       Otherwise you should always use 'DeeTuple_Cast'.
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeTuple_TAppendNonTupleSeq(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN DeeTypeObject const *tp_sequence,
 DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif

//////////////////////////////////////////////////////////////////////////
// Returns a new tuple containing all the types of the elements of 'self'
// Useful when wanting to output the names of all argument types in a function
// >> DeeError_SetStringf(&DeeErrorType_NotImplemented,"Not implemented: my_func%K",DeeTuple_Types(args));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_Types(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Create a tuple from a NULL-terminated vararg-list of 'DeeObject *'
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeTuple_VNewObjArgs(DEE_A_INOUT va_list args);
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) _DeeTuple_NewObjArgs(DEE_A_IN int prefix, ...) DEE_ATTRIBUTE_SENTINAL;
#define DeeTuple_NewObjArgs(...) _DeeTuple_NewObjArgs(0,__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
// v returns 1 if found; 0 if not found; -1 on error
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeTuple_Contains(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeTuple_ContainsPred(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Clear the pool of free tuples
DEE_FUNC_DECL(Dee_size_t) DeeTuple_ClearPool(void);

//////////////////////////////////////////////////////////////////////////
// Same as printf format, but outputs into va_args. e.g.:
// NOTES:
//   - The '%' prefix is optional, but might be
//     required in some cases to prevent ambiguity
//   - 'b' is bool (accepts same length modifiers as %u)
//   - 'f' is float
//   - 'D' is double
//   - 'LD' is long double
//   - 'k', 'K', 'r', 'R', 'o', 'O' all do the same (output a DeeObject)
//   - The fmt may contain a ":" indicating that the remained should be used for a function name
//   - illegal: '%%' (in printf: output '%')
//   - illegal: '%q' (in printf: output quoted string)
//   - '%.*s' first outputs the string size (unsigned int *) then the string (char [const] **)
//   - '|' can be used to specify optional arguments, where all following arguments are not required
// >> Dee_size_t a,c; DeeObject *b,*d = Dee_None;
// >> if (DeeTuple_Unpack(args,"IdoId|o:my_func",&a,&b,&c,&d) == -1) return -1;
// NOTE: 
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeTuple_Unpack(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_IN_UNPACKTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeTuple_VUnpack(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_IN_UNPACKTUPLEF char const *fmt,
 DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Retrieve information about a given unpack format string
// NOTE: If the function name isn't included and func_name != NULL, store NULL in '*func_name'
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTuple_UnpackInfo(
 DEE_A_IN_Z char const *fmt, DEE_A_OUT_OPT Dee_size_t *argc_min,
 DEE_A_OUT_OPT Dee_size_t *argc_max, DEE_A_OUT_OPT char const **func_name)
 DEE_ATTRIBUTE_NONNULL((1));

#define DEE_TUPLE_UNPACKFMT_INT(sizeof)  "I" DEE_PP_STR(DEE_PP_MUL8(sizeof)) "d"
#define DEE_TUPLE_UNPACKFMT_UINT(sizeof) "I" DEE_PP_STR(DEE_PP_MUL8(sizeof)) "u"

#if defined(GUARD_DEEMON_DEEMONRUN_H) && !defined(__INTELLISENSE__)
#define DeeRun_FileStream(filestream,config)                             DeeRun_FileStreamArgs(filestream,config,Dee_EmptyTuple)
#define DeeRun_FileStreamEx(filestream,disp_filename,config)             DeeRun_FileStreamExArgs(filestream,disp_filename,config,Dee_EmptyTuple)
#define DeeRun_Filename(filename,config)                                 DeeRun_FilenameArgs(filename,config,Dee_EmptyTuple)
#define DeeRun_FilenameEx(filename,disp_filename,config)                 DeeRun_FilenameExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define DeeRun_FilenameObject(filename,config)                           DeeRun_FilenameObjectArgs(filename,config,Dee_EmptyTuple)
#define DeeRun_FilenameObjectEx(filename,disp_filename,config)           DeeRun_FilenameObjectExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define DeeRun_String(code,config)                                       DeeRun_StringArgs(code,config,Dee_EmptyTuple)
#define DeeRun_StringObject(code,config)                                 DeeRun_StringObjectArgs(code,config,Dee_EmptyTuple)
#define DeeRun_StringWithLength(code,code_length,config)                 DeeRun_StringWithLengthArgs(code,code_length,config,Dee_EmptyTuple)
#define DeeRun_StringEx(code,disp_filename,config)                       DeeRun_StringExArgs(code,disp_filename,config,Dee_EmptyTuple)
#define DeeRun_StringObjectEx(code,disp_filename,config)                 DeeRun_StringObjectExArgs(code,disp_filename,config,Dee_EmptyTuple)
#define DeeRun_StringWithLengthEx(code,code_length,disp_filename,config) DeeRun_StringWithLengthExArgs(code,code_length,disp_filename,config,Dee_EmptyTuple)
#define DeeExec_FileStream(filestream,config)                            DeeExec_FileStreamArgs(filestream,config,Dee_EmptyTuple)
#define DeeExec_FileStreamEx(filestream,disp_filename,config)            DeeExec_FileStreamExArgs(filestream,disp_filename,config,Dee_EmptyTuple)
#define DeeExec_Filename(filename,config)                                DeeExec_FilenameArgs(filename,config,Dee_EmptyTuple)
#define DeeExec_FilenameEx(filename,disp_filename,config)                DeeExec_FilenameExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define DeeExec_FilenameObject(filename,config)                          DeeExec_FilenameObjectArgs(filename,config,Dee_EmptyTuple)
#define DeeExec_FilenameObjectEx(filename,disp_filename,config)          DeeExec_FilenameObjectExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_TUPLE_H */
