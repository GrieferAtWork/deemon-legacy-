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
#ifndef GUARD_DEEMON_TRACEBACK_H
#define GUARD_DEEMON_TRACEBACK_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#endif
#if DEE_DEPRECATED_API_VERSION(100,101,103)
#include <deemon/type.h> // Required for deprecated functions
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
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

DEE_OBJECT_DEF(DeeTracebackObject);
DEE_OBJECT_DEF(DeeTracebackIteratorObject);
DEE_OBJECT_DEF(DeeTracebackEntryObject);

#ifdef DEE_LIMITED_API
struct DeeTupleObject;
struct DeeTracebackEntry {
 DEE_A_REF struct DeeStringObject *te_file; /*< [0..1] Filename of this entry. */
 Dee_int64_t                       te_line; /*< Line within the given file. */
 DEE_A_REF struct DeeStringObject *te_func; /*< [0..1] Name of this entry's function. */
};
#define DeeTracebackEntry_FILE(ob) ((DeeObject *)(ob)->te_file)
#define DeeTracebackEntry_LINE(ob) (ob)->te_line
#define DeeTracebackEntry_FUNC(ob) ((DeeObject *)(ob)->te_func)
struct DeeTracebackObject {
 DEE_OBJECT_HEAD
        Dee_size_t        tb_tracebackc;       /*< Traceback size. */
 struct DeeTracebackEntry tb_tracebackv[1024]; /*< [tb_tracebackc] Traceback elements. */
};
#define DeeTraceback_SIZE(ob) ((Dee_size_t)((DeeTracebackObject *)Dee_REQUIRES_POINTER(ob))->tb_tracebackc)
#define DeeTraceback_ELEM(ob) ((struct DeeTracebackEntry *)((DeeTracebackObject *)Dee_REQUIRES_POINTER(ob))->tb_tracebackv)
#endif

struct _DeeEmptyTracebackObject { DEE_OBJECT_HEAD Dee_size_t _placeholder; };
DEE_DATA_DECL(struct _DeeEmptyTracebackObject) _Dee_EmptyTraceback;
#define Dee_EmptyTraceback  ((DeeObject *)&_Dee_EmptyTraceback)
#define DeeReturn_EmptyTraceback  do{Dee_INCREF(Dee_EmptyTraceback);return Dee_EmptyTraceback;}while(0)


DEE_DATA_DECL(DeeTypeObject) DeeTraceback_Type;
#define DeeTraceback_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTraceback_Type)
#define DeeTraceback_Check          DeeTraceback_CheckExact
#define DeeTraceback_CheckEmpty(ob) ((DeeObject *)Dee_REQUIRES_POINTER(ob) == Dee_EmptyTraceback)

DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTracebackObject) *) DeeTraceback_New(void);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeTraceback_Size(
 DEE_A_IN_OBJECT(DeeTracebackObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTraceback_Get(
 DEE_A_IN_OBJECT(DeeTracebackObject) const *self, DEE_A_IN Dee_size_t i,
 DEE_A_OUT_OBJECT_OPT(DeeStringObject) **file, DEE_A_OUT_OPT Dee_int64_t *line,
 DEE_A_OUT_OBJECT_OPT(DeeStringObject) **func) DEE_ATTRIBUTE_NONNULL((1));

#if DEE_DEPRECATED_API_VERSION(100,101,103)
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("'DeeTraceback_Repr()' is deprecated. Use 'DeeObject_Repr' instead")
 DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeTraceback_Repr(
 DEE_A_IN_OBJECT(DeeTracebackObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTraceback_Check(self));
 return DeeObject_Repr((DeeObject *)self);
}
#endif


#ifdef DEE_LIMITED_API
struct DeeThreadObject;
// NOTE: 'thread_self' must be DeeThread_Self(), since otherwise we'd
//       break some of the rules set up to stabilize the thread-local stack.
//       >> This function is only used if the caller already knows the value of
//          DeeThread_Self(), so we don't have to query the (probably) slow tls lookup again.
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTracebackObject) *
 _DeeTraceback_New(DEE_A_IN struct DeeThreadObject *thread_self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_LIMITED_API */


#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Some helper objects for iterating a traceback and accessing its elements
struct DeeTracebackIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeTracebackObject       *ti_traceback; /*< [1..1] Keep-alive reference. */
 /*atomic*/struct DeeTracebackEntry *ti_iter;      /*< [1..1] Current iterator position. */
 struct DeeTracebackEntry           *ti_end;       /*< [1..1] End of the traceback. */
};
struct DeeTracebackEntryObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeTracebackObject   *te_traceback; /*< [1..1] Keep-alive reference. */
 struct DeeTracebackEntry const *te_elem;      /*< [1..1] Traceback element. */
};
#endif

DEE_DATA_DECL(DeeTypeObject) DeeTracebackIterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeTracebackEntry_Type;
#define DeeTracebackIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeTracebackIterator_Type)
#define DeeTracebackIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTracebackIterator_Type)
#define DeeTracebackEntry_Check(ob)      DeeObject_InstanceOf(ob,&DeeTracebackEntry_Type)
#define DeeTracebackEntry_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTracebackEntry_Type)

#ifdef DEE_LIMITED_API
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeTracebackEntryObject) *DeeTracebackEntry_New(
 DEE_A_INOUT_OBJECT(DeeTracebackObject) *tb, DEE_A_IN struct DeeTracebackEntry const *entry)
 DEE_ATTRIBUTE_NONNULL((1,2));
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_TRACEBACK_H */
