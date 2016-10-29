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

#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1

#define GC

#include <deemon/__conf.inl>
#include <deemon/object.h>
#include <deemon/gc.h>
#include <deemon/optional/std/string.h>
#ifdef GC
#include "gc.c"
#endif
#endif

#ifdef __INTELLISENSE__
DEE_DECL_BEGIN
#endif

#ifdef GC
#ifdef AGGRESSIVE
DEE_STATIC_INLINE(int) _DeeObject_InternalDestroyGCAggressive(
 DEE_A_INOUT DeeObject *ob, struct _DeeGCHead **next,
 unsigned int initial_refcnt)
#else
DEE_STATIC_INLINE(int) _DeeObject_InternalDestroyGC(
 DEE_A_INOUT DeeObject *ob, struct _DeeGCHead **next,
 unsigned int initial_refcnt)
#endif
#else
#ifdef DEE_DEBUG
static void _dee_warn_link_error(void);
void _DeeObject_InternalDestroy(
 DEE_A_INOUT DEE_A_OUT_INVALID DeeObject *ob) {
 _dee_warn_link_error();
 _DeeObject_InternalDestroy_d(ob,"??" "?",0);
}
void _DeeObject_InternalDestroy_d(
 DEE_A_INOUT DEE_A_OUT_INVALID DeeObject *ob,
 DEE_A_IN_Z char const *file, DEE_A_IN int line)
#else /* DEE_DEBUG */
void _DeeObject_InternalDestroy_d(
 DEE_A_INOUT DEE_A_OUT_INVALID DeeObject *ob,
 DEE_A_IN_Z char const *DEE_UNUSED(file), DEE_A_IN int DEE_UNUSED(line)) {
 _DeeObject_InternalDestroy(ob);
}
void _DeeObject_InternalDestroy(DEE_A_INOUT DeeObject *ob)
#endif /* !DEE_DEBUG */
#endif
{
 DeeTypeObject *tp_first,*tp_iter;
#ifndef GC
 int gc_object;
 DEE_ASSERT(ob && DeeAtomicInt_Load(ob->__ob_refcnt,memory_order_seq_cst) == 0);
#else
#ifndef AGGRESSIVE
 unsigned int temp;
#endif
 DEE_ASSERT(ob && (unsigned int)DeeAtomicInt_Load(
  ob->__ob_refcnt,memory_order_seq_cst) >= initial_refcnt);
#endif
 tp_first = Dee_TYPE(ob);
 DEE_ASSERTF(DeeObject_Check(tp_first) && DeeType_Check(tp_first),
             "Not a type: %#p (%q)",tp_first,tp_first ? DeeType_NAME(tp_first) : NULL);
#ifdef DEE_DEBUG
#ifndef GC
 if (_DeeFlag_Verbose >= 4) _Dee_VerboseOut(
  "%s(%d) : Destroying Object %s@%p\n",
  file,line,DeeType_NAME(tp_first),ob);
#endif
#endif /* DEE_DEBUG */
 tp_iter = tp_first;
#ifndef GC
 gc_object = DeeObject_IS_GC(ob);
 // Stop tracking this object
 // We can't have the garbage collector and possibly also
 // destroying this object, while we are already trying to do so.
 // NOTE: We must do this while the object's reference counter is still '0',
 //       because '0' references already makes the gc ignore the object.
 if (gc_object) DeeGC_TrackedRem(ob);
#else
 DEE_ASSERTF(DeeType_GET_SLOT(tp_first,tp_free) == &_DeeGC_TpFree,
             "GC Type with invalid free function");
#endif
 // In the first round, we can be sure that 'tp_iter' doesn't have
 // a different destructor than 'tp_first'.
 // Mainly because 'tp_iter == tp_first'...
 // >> So we skip the check if a custom type must be
 //    created and directly call the destructor
#ifndef AGGRESSIVE
 if (DeeType_HAS_SLOT(tp_iter,tp_clear)
#ifndef GC
    &&((DeeType_FLAGS(tp_iter)&DEE_TYPE_FLAG_ALWAYS_CLEAR)!=0)
#endif
     ) {
  goto begin_clear_call;
 } else
#endif
 if (DeeType_HAS_SLOT(tp_iter,tp_dtor)) {
  goto begin_dtor_call;
 } else {
  // First destructor isn't set. - skip forward to the second one
  goto begin_next;
 }
 while (1) {
#ifndef AGGRESSIVE
  if (DeeType_HAS_SLOT(tp_iter,tp_clear)
#ifndef GC
      &&((DeeType_FLAGS(tp_iter)&DEE_TYPE_FLAG_ALWAYS_CLEAR)!=0)
#endif
      ) {
   // Only call tp_clear, it it's non-NULL
#ifndef GC
   if (DeeType_GET_SLOT(tp_iter,tp_free) != DeeType_GET_SLOT(tp_first,tp_free) &&
       // This is required because custom class types are GC objects and
       // derived from 'type', which isn't a GC type.
       // So we need this exception for gc objects derived from 'type'.
       !DeeType_IsSameOrDerived(tp_iter,&DeeType_Type)
       ) {
    DEE_ASSERTF(!gc_object,"GC Object with invalid free function");
    DEE_ASSERT(tp_iter != tp_first);
    // TODO: We must create a new, custom type here, that is
    //       equivalent to '*tp_iter', but has the 'tp_free' function of 'tp_first'
    //       - This is required to make sure that if 'tp_clear' interrupts
    //         destruction by adding a new, external reference, we still
    //         call the correct tp_free function when the object will eventually be destroyed
    // NOTE: We can't do this once we've detected that the object has been revived,
    //       because we won't be able to override the object's type at that time.
    //       'ob->__ob_type' isn't atomic, meaning that we need exclusive access
    //       to the whole object if we want to modify it. - Something we do have
    //       right now, as long as it's reference counter is still 0
    DEE_ASSERTF(0,"Not implemented: Custom free types");
   }
#endif
begin_clear_call:
   // Fake reference to satisfy the state an object
   // has to be in when tp_clear is called
#ifdef GC
   if (!DeeAtomicInt_CompareExchangeStrong(ob->__ob_refcnt,
        initial_refcnt,initial_refcnt+1,
        memory_order_seq_cst,memory_order_seq_cst)) {
    *next = DeeGC_OB2HEAD(ob)->gc_next;
    return DEE_GC_DESTROY_ABORTED; // destruction aborted
   }
#else
#ifdef DEE_DEBUG
   if (DeeAtomicInt_FetchInc(ob->__ob_refcnt,memory_order_seq_cst) != 0)
    DEE_ASSERTF(0,"Object wasn't dead or someone added a reference to a dead object");
#else
   DeeAtomicInt_IncFetch(ob->__ob_refcnt,memory_order_seq_cst);
#endif
#endif
   // Do some assertions, to make people implementing destructors feel safer.
   DEE_ASSERT(DeeObject_Check(ob) && DeeObject_InstanceOf(ob,tp_iter));

   // Call the clear function and allow it to revive the object
   // NOTE: gc objects aren't tracked during this destruction phase!
   (*DeeType_GET_SLOT(tp_iter,tp_clear))(ob);

   // Remove the temporary reference and check if destruction was aborted
#ifdef GC
   temp = (unsigned int)DeeAtomicInt_DecFetch(ob->__ob_refcnt,memory_order_seq_cst);
   // NOTE: We need to re-confirm tha the object should still be deleted!
   //       >> If we simply say that it shouldn't if it still has references,
   //          we could potentially skip clear callbacks that are actually
   //          responsible for clearing lookback references:
   // >> cl = <>;
   // >> class a { this() { cl.set(this); } ~this() { print "Last dtor"; } };
   // >> class b: a { ~this() { print "Don't skip me!"; } };
   // >> // After tp_clear was called on this, we must make sure that it didn't
   // >> // revive the object somewhere outside the reference loop cl-a-inst.
   // >> class c: b { ~this() { print "First dtor"; } };
   // >> inst = c();
   if DEE_UNLIKELY(temp && !_deegc_should_delete(DeeGC_OB2HEAD(ob),temp))
   //if (temp /*&& !_deegc_should_delete(DeeGC_OB2HEAD(ob),temp)*/)
#else
   if DEE_UNLIKELY(DeeAtomicInt_DecFetch(ob->__ob_refcnt,memory_order_seq_cst) != 0)
#endif
   {
    // Destruction aborted (object will remain alive as the portion we haven't destroyed yet)
    // Adjust references to the types
    // If the types have changed, remove the reference from the first type
    if DEE_UNLIKELY(tp_iter != tp_first) {
     // NOTE: In the hidden else block, we inherit a reference from the custom free type
     if (ob->__ob_type == tp_iter) Dee_INCREF(tp_iter);
     Dee_DECREF(tp_first);
    }
#ifndef GC
    if (gc_object) DeeGC_TrackedAdd(ob); // Start tracking this object (again)
#endif
#ifdef GC
    *next = DeeGC_OB2HEAD(ob)->gc_next;
    return DEE_GC_DESTROY_PARTIAL; // Partially destroyed
#else
    return;
#endif
   } else {
#ifdef GC
    // Update the base reference counter to match the new
    // reference counter after the destructor was called
    initial_refcnt = temp;
#endif
    // Clear didn't revive it. Time to call the destructor!
    if DEE_LIKELY(DeeType_HAS_SLOT(tp_iter,tp_dtor)) goto begin_dtor_call;
   }
  } else
#endif
  if (DeeType_HAS_SLOT(tp_iter,tp_dtor)) {
   // Only call tp_dtor, it it's non-NULL
begin_dtor_call:
#ifndef GC
   // NOTE: The gc is allowed to destroy objects that still have references
   DEE_ASSERTF(!DeeAtomicInt_Load(ob->__ob_refcnt,memory_order_seq_cst),
               "Object %s:%p has non-zero reference count before call to destructor",
               DeeType_NAME(Dee_TYPE(tp_iter)),ob);
#endif
   // Do some assertions, to make people implementing destructors feel safer.
   DEE_ASSERT(DeeType_IsSameOrDerived(Dee_TYPE(ob),tp_iter));
   // NOTE: DeeObject_Check is no longer true within the destructor
   // NOTE: gc objects aren't tracked during their destruction phase!
   (*DeeType_GET_SLOT(tp_iter,tp_dtor))(ob);
  }
begin_next:
  if DEE_UNLIKELY(ob->__ob_type != tp_iter) Dee_DECREF(ob->__ob_type); // Custom type
  // The object's reference counter is now '0' (marked as dead, but not freed)
  if DEE_UNLIKELY(!DeeType_BASE(tp_iter)) break;
  ob->__ob_type = (tp_iter = DeeType_BASE(tp_iter));
  // Continue with the next destructor
 }
#ifdef DEE_DEBUG
#ifdef GC
 DEE_ASSERTF((unsigned int)DeeAtomicInt_Load(ob->__ob_refcnt,memory_order_seq_cst) == initial_refcnt,
             "Object still referenced after all destructors have been called. "
             "If this assertion fails when called by the gc, there might be "
             "some missing 'tp_visit' functions!");
#else
 DEE_ASSERTF(DeeAtomicInt_Load(ob->__ob_refcnt,memory_order_seq_cst) == 0,
             "Object still referenced after all destructors have been called. "
             "If this assertion fails when called by the gc, there might be "
             "some missing 'tp_visit' functions!");
#endif
#endif
 // Release a weak reference (and possibly free the object)
 if DEE_LIKELY(DeeAtomicInt_CompareExchangeStrong(ob->__ob_weakcnt,
  1,0,memory_order_seq_cst,memory_order_seq_cst)) {
do_free:
#ifdef GC
  DEE_ASSERTF(DeeType_GET_SLOT(tp_first,tp_free) == &_DeeGC_TpFree,
              "GC Type with invalid free function");
#else
  ob->__ob_type = NULL; // Make sure no type is set while in the free function
  (*DeeType_GET_SLOT(tp_first,tp_free))(tp_first,ob);
#endif
  Dee_DECREF(tp_first); // we inherited the type reference
 } else {
  // Restore the original type if we didn't free the object
  // >> Required so we use the correct free function when all
  //    remaining weak references have been removed
  // NOTE: Technically the perfect type in this case would be
  //       a custom sub-class of 'object', only overwriting
  //       the free function with the one from 'tp_first'.
  //       But since the object is dead, tp_free is the only
  //       thing that is still allowed to be executed, meaning
  //       we can simply set the original type (which contains
  //       that free function).
  ob->__ob_type = tp_first;
  if DEE_UNLIKELY(_DeeRefCounter_DecFetch(ob->__ob_weakcnt) == 0) {
   // The weak reference was removed in this split millisecond
   // But now we are the owner of this object!
   goto do_free;
  }
#ifdef GC
  else {
   // Unlink the object from the gc list and
   // fill in the address to the next object
   struct _DeeGCHead *prev_,*next_;
   next_ = *next = DeeGC_OB2HEAD(ob)->gc_next;
   prev_ = DeeGC_OB2HEAD(ob)->gc_prev;
   if (next_) next_->gc_prev = prev_;
   if (prev_) prev_->gc_next = next_; else {
    DEE_ASSERT(DeeGC_OB2HEAD(ob) == _dee_gc_tracked);
    _dee_gc_tracked = next_;
   }
#ifdef DEE_DEBUG
   // Satisfy the assertions checking that the object isn't tracked
   DeeGC_OB2HEAD(ob)->gc_prev = NULL;
   DeeGC_OB2HEAD(ob)->gc_next = NULL;
#endif
   // destructed, but can't free (a weakref prevents us from doing so)
   return DEE_GC_DESTROY_DONTFREE;
  }
#endif
 }
#ifdef GC
 // Unlink the object from the gc list and
 // fill in the address to the next object
 {
  struct _DeeGCHead *gcprev,*gcnext;
  gcnext = *next = DeeGC_OB2HEAD(ob)->gc_next;
  gcprev = DeeGC_OB2HEAD(ob)->gc_prev;
  if (gcnext) gcnext->gc_prev = gcprev;
  if (gcprev) gcprev->gc_next = gcnext; else {
   DEE_ASSERT(DeeGC_OB2HEAD(ob) == _dee_gc_tracked);
   _dee_gc_tracked = gcnext;
  }
#ifdef DEE_DEBUG
  // Satisfy the assertions for checking that the object isn't tracked
  DeeGC_OB2HEAD(ob)->gc_prev = NULL;
  DeeGC_OB2HEAD(ob)->gc_next = NULL;
#endif
 }
 return DEE_GC_DESTROY_SUCCESS;
#endif
}
#ifdef GC
#undef GC
#endif
#ifdef AGGRESSIVE
#undef AGGRESSIVE
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
