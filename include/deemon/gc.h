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
#ifndef GUARD_DEEMON_GC_H
#define GUARD_DEEMON_GC_H 1

#include <deemon/__conf.inl>

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

#define DEE_A_GC_TRACKED   /* gc-able object that is tracked */
#define DEE_A_GC_UNTRACKED /* gc-able object that isn't tracked */

// --Important--
// 
//  - After allocating an object that uses the gc, you have to
//    add it to manually add it to the list of tracked objects
//    __AFTER__ you've initialized to a state in which tp_visit
//    can safely be called on the instance.
//
//  - Functions that return an untracked gc-object
//    are annotated with 'DEE_A_GC_UNTRACKED'.
//   
//  - 'tp_clear' of must leave an object in a
//    state where visit can still be called.

#ifdef DEE_LIMITED_API
typedef unsigned int Dee_gc_visitid_t;
extern Dee_gc_visitid_t      _dee_gc_next_visit_id;
extern struct DeeAtomicMutex _dee_gc_collect_lock;
#endif

#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_REFCNT_TYPES
DEE_PRIVATE_DECL_DEE_REFCNT_TYPES
#undef DEE_PRIVATE_DECL_DEE_REFCNT_TYPES
#endif

struct _DeeGCHead {
 struct _DeeGCHead *gc_prev;      /*< [0..1] Prev (younger) object in global linked list of gc objects. */
 struct _DeeGCHead *gc_next;      /*< [0..1] Next (older) object in global linked list of gc objects. */
 Dee_gc_visitid_t   gc_last_seen; /*< Last time this object was seen. */
 Dee_refcnt_t       gc_refs;      /*< The amount of tracked references. */
};
#define DeeGC_OB2HEAD(ob) (((struct _DeeGCHead *)Dee_REQUIRES_POINTER(ob))-1)
#define DeeGC_HEAD2OB(ob) ((DeeObject *)((struct _DeeGCHead *)Dee_REQUIRES_POINTER(ob)+1))
#endif

#define DeeObject_IS_GC(ob) \
 ((DeeType_FLAGS(Dee_TYPE(ob))&DEE_TYPE_FLAG_HAS_GC)!=0)


//////////////////////////////////////////////////////////////////////////
// Collect garbage on all generations
DEE_FUNC_DECL(DEE_A_EXEC Dee_size_t) DeeGC_CollectNow(void);

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int DeeGC_Initialize(void);
extern DEE_A_REQ_INIT void DeeGC_Finalize(void);
#endif

//////////////////////////////////////////////////////////////////////////
// Same as '_DeeObject_Delete', but is faster and only works for gc types.
DEE_FUNC_DECL(DEE_A_EXEC void) _DeeGCObject_Delete(
 DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1));
#define _DeeGCObject_DELETE(ob)\
do{\
 DEE_ASSERTF(DeeObject_Check(ob),"Not a deemon object");\
 DEE_ASSERTF(DeeObject_IS_UNIQUE(ob),"Object isn't unique");\
 DEE_ASSERTF(DeeObject_IS_GC(ob),"Not a GC object");\
 Dee_DECREF(Dee_TYPE(ob)); /* Drop reference to the type. */ \
 DeeGC_Free(ob);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Begin or end tracking an object 'ob'
// NOTES:
//  - 'DeeObject_IS_GC(ob)' must be true
//  - 'DeeGC_TrackedAdd' requires the object not to be tracked
//  - 'DeeGC_TrackedRem' is a no-op if the object isn't tracked
DEE_FUNC_DECL(DEE_A_REQ_INIT void) DeeGC_TrackedAdd(DEE_A_GC_UNTRACKED DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_REQ_INIT void) DeeGC_TrackedRem(DEE_A_GC_TRACKED DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Set these two functions as tp_alloc and tp_free in your type,
// and add the 'DEE_TYPE_FLAG_HAS_GC' flag to its tp_flags, to enable gc for it.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_GC_UNTRACKED DEE_A_RET_EXCEPT_REF DeeObject *) _DeeGC_TpAlloc(DEE_A_IN DeeTypeObject *tp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_REQ_INIT void) _DeeGC_TpFree(DEE_A_IN DeeTypeObject *tp, DEE_A_GC_UNTRACKED DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));



//////////////////////////////////////////////////////////////////////////
// NOTES:
//   - Objects returned by 'DeeGC_Malloc' are not tracked
//   - Objects given to 'DeeGC_Free' may not be tracked
//   - 's' must not be zero and 'p' may not be NULL
#define /*DEE_A_EXEC*/ DeeGC_MALLOC(T)          ((T*)DeeGC_Malloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_CALLOC(T)          ((T*)DeeGC_Calloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_WEAKMALLOC(T)      ((T*)DeeGC_WeakMalloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_WEAKCALLOC(T)      ((T*)DeeGC_WeakCalloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_MALLOCF(T,...)     ((T*)DeeGC_Malloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_CALLOCF(T,...)     ((T*)DeeGC_Calloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_WEAKMALLOCF(T,...) ((T*)DeeGC_WeakMalloc(sizeof(T)))
#define /*DEE_A_EXEC*/ DeeGC_WEAKCALLOCF(T,...) ((T*)DeeGC_WeakCalloc(sizeof(T)))
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Malloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Calloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Realloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Malloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Calloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_Realloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeGC_Free(DEE_A_IN void *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeGC_Free_d(DEE_A_IN void *p, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeGC_WeakRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeGC_TryWeakRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_DEBUG
#ifndef __INTELLISENSE__
#define /*DEE_A_EXEC*/ DeeGC_Malloc(s)           DeeGC_Malloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_Malloc(" #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_Calloc(s)           DeeGC_Calloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_Calloc(" #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_Realloc(p,s)        DeeGC_Realloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeGC_Realloc(" #p "," #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_Free(p)             DeeGC_Free_d(p,DEE_DEBUG_NEW_ACCESS("DeeGC_Free(" #p ")"))
#define                DeeGC_WeakMalloc(s)       DeeGC_WeakMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_WeakMalloc(" #s ")"))
#define                DeeGC_WeakCalloc(s)       DeeGC_WeakCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_WeakCalloc(" #s ")"))
#define                DeeGC_WeakRealloc(p,s)    DeeGC_WeakRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeGC_WeakRealloc(" #p "," #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_TryMalloc(s)        DeeGC_TryMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryMalloc(" #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_TryCalloc(s)        DeeGC_TryCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryCalloc(" #s ")"))
#define /*DEE_A_EXEC*/ DeeGC_TryRealloc(p,s)     DeeGC_TryRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryRealloc(" #p "," #s ")"))
#define                DeeGC_TryWeakMalloc(s)    DeeGC_TryWeakMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryWeakMalloc(" #s ")"))
#define                DeeGC_TryWeakCalloc(s)    DeeGC_TryWeakCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryWeakCalloc(" #s ")"))
#define                DeeGC_TryWeakRealloc(p,s) DeeGC_TryWeakRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeGC_TryWeakRealloc(" #p "," #s ")"))
#endif
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_GC_H */
