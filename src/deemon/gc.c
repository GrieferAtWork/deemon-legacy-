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
#ifndef GUARD_DEEMON_GC_C
#define GUARD_DEEMON_GC_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/gc.h>
#include <deemon/fs_api.h>
#include <deemon/error.h>
#include <deemon/deemonrun.h>
#include <deemon/memberfunction.h>
#include <deemon/optional/object_pool.h>
#include <deemon/type.h>
#ifndef DEE_WITHOUT_THREADS
#include <deemon/mp/mutex.h>
#endif /* !DEE_WITHOUT_THREADS */

#ifndef DEE_WITHOUT_THREADS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/time.h>
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include <pthread.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H
#include <sched.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* !DEE_WITHOUT_THREADS */

DEE_DECL_BEGIN

#define GC_VISITID_NEVER_SEEN       0 // GC has never seen this object
#define GC_VISITID_FIRST_COLLECTION 1 // First garbage collection id


// Why is tp_clear required?
// Scenario:
//   
//  +----+ 
//  |    v
// [a]<-[b]<-+
//       |   |
//       +---+
//   
// - 'b' has a reference to itself and 'a'
// - 'a' has a reference to 'b'
//
// -- without tp_clear --
// 1. gc correctly detects 'a' as being dead
// 2. call tp_dtor on 'a', decref'ing, but not destroying 'b'
// 3. 'b' will remain alive and a still has a reference
// 4. CRITICAL_ERROR: 'a' still referenced after destructor was called
// 
// -- with tp_clear --
// 1. gc correctly detects 'a' as being dead
// 2. call tp_clear on 'a', decref'ing, but not destroying 'b'
// 3. detect that 'a' is still alive
// 4. 'a' wasn't destroyed
// 5. gc correctly detects 'b' as being dead
// 6. call tp_clear on 'b', decref'ing and destroying 'a'
// 7. call tp_dtor on 'b', successfully destroying it
//

unsigned int _dee_gc_next_visit_id = GC_VISITID_FIRST_COLLECTION;
#ifndef DEE_WITHOUT_THREADS
static struct _DeeGCHead *_dee_gc_tracked_pending = NULL;
#endif
// TODO: Add more generations for tracked objects
// TODO: Call __builtin_gc_collect() once a generation is full
static struct _DeeGCHead *_dee_gc_tracked = NULL;

#ifndef DEE_WITHOUT_THREADS
static struct DeeAtomicMutex _dee_gc_tracked_pending_lock = DeeAtomicMutex_INIT();
#endif /* !DEE_WITHOUT_THREADS */

// v Must be a recursive mutex, because recursion can happen if
//   we free a gc-able object through decref while calling the
//   destructor of a gc-object we just found out to be dead.
//   1. gc_collect()
//   2. lock '_dee_gc_tracked_lock'
//   3. object found
//   4. call destructor on object
//   5. 
#ifndef DEE_WITHOUT_THREADS
static struct DeeNativeMutex _dee_gc_tracked_lock;
struct DeeAtomicMutex _dee_gc_collect_lock = DeeAtomicMutex_INIT();
#endif /* !DEE_WITHOUT_THREADS */




DEE_A_RET_EXCEPT(-1) int DeeGC_Initialize(void) {
#ifndef DEE_WITHOUT_THREADS
 return DeeNativeMutex_Init(&_dee_gc_tracked_lock);
#else /* !DEE_WITHOUT_THREADS */
 return 0;
#endif /* DEE_WITHOUT_THREADS */
}


DEE_STATIC_INLINE(void) _DeeGC_CollectAggressive(void);
void DeeGC_Finalize(void) {
 // Collect anything that really doesn't want to be freed
 // - In this collection mode, deemon doesn't call tp_clear,
 //   and thereby doesn't give the objects any chance of being revived.
 _DeeGC_CollectAggressive();
#ifdef DEE_DEBUG
 if (_dee_gc_tracked) {
  struct _DeeGCHead *iter = _dee_gc_tracked;
  DEE_LDEBUG("-- Objects still being tracked --\n");
  do {
   DEE_LDEBUG("- %d references: %k:%r\n",
              _DeeRefCounter_Fetch(DeeGC_HEAD2OB(iter)->__ob_refcnt),
              Dee_TYPE(DeeGC_HEAD2OB(iter)),
              DeeGC_HEAD2OB(iter));
  } while ((iter = iter->gc_next) != NULL);
  DEE_LDEBUG("-- !!!!!!!!!!!!!!!!!!!!!!!!!!! --\n"
             "Objects still tracked in gc_finalize.\n"
             "This probably means a gc_object is kept alive through a reference leak.\n"
             "Make sure all types that could potentially reference themselves are gc-objects!\n"
             "\n"
             "Another reason could be missing 'tp_visit' functions. Make sure all\n"
             "objects referencing other objects implement tp_visit properly!\n");
 }
#endif
#ifndef DEE_WITHOUT_THREADS
 DEE_ASSERT(_dee_gc_tracked_pending == NULL);
#endif /* !DEE_WITHOUT_THREADS */
 _dee_gc_next_visit_id = GC_VISITID_FIRST_COLLECTION;
#ifndef DEE_WITHOUT_THREADS
 DeeNativeMutex_Quit(&_dee_gc_tracked_lock);
#endif /* !DEE_WITHOUT_THREADS */
}


#ifndef DEE_WITHOUT_THREADS
#define _dee_gc_tracked_lock_acquire() \
 DeeNativeMutex_AcquireNoexcept(&_dee_gc_tracked_lock)
#define _dee_gc_tracked_lock_try_acquire() \
 (DeeNativeMutex_TryAcquireNoexcept(&_dee_gc_tracked_lock)==0)
DEE_STATIC_INLINE(void) _dee_gc_tracked_lock_release(void) {
 // Acquire the lock to the list of pending additions
 // >> These additions have to be published as soon as
 //    the tracked list of objects is published
 // >> We lock the pending list first, to prevent more additions
 //    after the tracked list has been unlocked
 DeeAtomicMutex_Acquire(&_dee_gc_tracked_pending_lock);
 if (_dee_gc_tracked_pending) {
  // Publish pending additions
  struct _DeeGCHead *end = _dee_gc_tracked_pending;
  while (end->gc_next) end = end->gc_next;
  if ((end->gc_next = _dee_gc_tracked) != NULL)
   _dee_gc_tracked->gc_prev = end;
  _dee_gc_tracked = _dee_gc_tracked_pending;
  _dee_gc_tracked_pending = NULL;
 }
 DeeAtomicMutex_Release(&_dee_gc_tracked_pending_lock);
 DeeNativeMutex_ReleaseNoexcept(&_dee_gc_tracked_lock);
}
#else /* !DEE_WITHOUT_THREADS */
#define _dee_gc_tracked_lock_acquire()     (void)0
#define _dee_gc_tracked_lock_try_acquire() 1
#define _dee_gc_tracked_lock_release()     (void)0
#endif /* DEE_WITHOUT_THREADS */

#define FORMAT_DEBUG_NEW_INFO(p)\
 "%s(%d) : %Iu : %s : %s\n",\
 DEBUG_NEW_API_POINTER_GET_FILE(p),\
 DEBUG_NEW_API_POINTER_GET_LINE(p),\
 DEBUG_NEW_API_POINTER_GET_ID(p),\
 DEBUG_NEW_API_POINTER_GET_EXPR(p),\
 DEBUG_NEW_API_POINTER_GET_MSG(p)

#if defined(DEE_DEBUG) && 0
#define _DeeGC_AssertTrackedObjectListConsistent _DeeGC_AssertTrackedObjectListConsistent
void _DeeGC_AssertTrackedObjectListConsistent(void) {
 struct _DeeGCHead *iter;
 //_dee_gc_tracked_lock_acquire();
 iter = _dee_gc_tracked;
 DEE_ASSERTF(!iter || iter->gc_prev == NULL && "First element has predecessor",
             FORMAT_DEBUG_NEW_INFO(iter));
 while (iter) {
  DEE_ASSERTF(
   iter->gc_next != iter && iter->gc_prev != iter &&
   (iter->gc_next != iter->gc_prev || !iter->gc_prev) &&
   "Infinite loop in GC",FORMAT_DEBUG_NEW_INFO(iter));
  DEE_ASSERTF((iter->gc_prev == NULL) == (iter == _dee_gc_tracked) &&
             "Only the first element may have a NULL predecessor",
             FORMAT_DEBUG_NEW_INFO(iter));

  if (iter->gc_prev) DEE_ASSERTF(iter->gc_prev->gc_next == iter && "this->prev->next != this",FORMAT_DEBUG_NEW_INFO(iter->gc_prev));
  if (iter->gc_next) DEE_ASSERTF(iter->gc_next->gc_prev == iter && "this->next->prev != this",FORMAT_DEBUG_NEW_INFO(iter->gc_next));
  iter = iter->gc_next;
 }
 //_dee_gc_tracked_lock_release();
}
#endif


void DeeGC_TrackedAdd(DEE_A_GC_UNTRACKED DEE_A_INOUT DeeObject *ob) {
 struct _DeeGCHead *head;
 DEE_ASSERT(DeeObject_Check(ob) && DeeObject_IS_GC(ob));
 head = DeeGC_OB2HEAD(ob);
#ifdef DEE_DEBUG
 DEE_ASSERTF(head->gc_prev == NULL && head->gc_next == NULL &&
             "GC Object is already tracked",FORMAT_DEBUG_NEW_INFO(head));
#endif
 head->gc_prev = NULL;
#ifndef DEE_WITHOUT_THREADS
 if (_dee_gc_tracked_lock_try_acquire()) {
#endif /* !DEE_WITHOUT_THREADS */
#ifdef _DeeGC_AssertTrackedObjectListConsistent
  _DeeGC_AssertTrackedObjectListConsistent();
#endif
  if ((head->gc_next = _dee_gc_tracked) != NULL)
   _dee_gc_tracked->gc_prev = head;
  _dee_gc_tracked = head;
#ifndef DEE_WITHOUT_THREADS
  _dee_gc_tracked_lock_release();
 } else {
  DeeAtomicMutex_Acquire(&_dee_gc_tracked_pending_lock);
  if ((head->gc_next = _dee_gc_tracked_pending) != NULL)
   _dee_gc_tracked_pending->gc_prev = head;
  _dee_gc_tracked_pending = head;
  DeeAtomicMutex_Release(&_dee_gc_tracked_pending_lock);
 }
#endif /* !DEE_WITHOUT_THREADS */
}
void DeeGC_TrackedRem(DEE_A_GC_TRACKED DEE_A_INOUT DeeObject *ob) {
 struct _DeeGCHead *head;
 //DEE_ASSERT(DeeObject_Check(ob) && DeeObject_IS_GC(ob));
 head = DeeGC_OB2HEAD(ob);
 _dee_gc_tracked_lock_acquire();
#ifdef _DeeGC_AssertTrackedObjectListConsistent
 _DeeGC_AssertTrackedObjectListConsistent();
#endif
 DeeAtomicMutex_Acquire(&_dee_gc_tracked_pending_lock);
 if (head->gc_next) head->gc_next->gc_prev = head->gc_prev;
 if (head->gc_prev) head->gc_prev->gc_next = head->gc_next; else {
  if (head == _dee_gc_tracked) {
   _dee_gc_tracked = head->gc_next;
#ifndef DEE_WITHOUT_THREADS
  } else if (head == _dee_gc_tracked_pending) {
   _dee_gc_tracked_pending = head->gc_next;
#endif /* !DEE_WITHOUT_THREADS */
  }
 }
 DeeAtomicMutex_Release(&_dee_gc_tracked_pending_lock);
 _dee_gc_tracked_lock_release();
#ifdef DEE_DEBUG
 head->gc_prev = NULL;
 head->gc_next = NULL;
#endif
}

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct dependency_list {
 Dee_size_t               ob_a;
 Dee_size_t               ob_c;
#ifdef DEE_DEBUG
 union{
  struct _DeeGCHead **ob_vgc;
  DeeObject         **ob_vob;
 };
#else
 struct _DeeGCHead **ob_vgc;
#endif
};
DEE_COMPILER_MSVC_WARNING_POP

#define dependency_list_init(ob) ((ob)->ob_a=0,(ob)->ob_c=0,(ob)->ob_vgc=NULL)
#define dependency_list_quit(ob) free((ob)->ob_vgc)
#define dependency_list_contains_or_addob(self,ob) \
 dependency_list_contains_or_addgc(self,(struct _DeeGCHead *)(ob))
static int dependency_list_contains_or_addgc(
 struct dependency_list *self, struct _DeeGCHead *ob) {
 struct _DeeGCHead **iter,**end;
 end = (iter = self->ob_vgc)+self->ob_c;
 while (iter != end) if (*iter++ == ob) return 0;
 if (self->ob_c == self->ob_a) {
  Dee_size_t new_size = self->ob_a ? self->ob_a*2 : 2;
  iter = (struct _DeeGCHead **)realloc_nz(
   self->ob_vgc,new_size*sizeof(struct _DeeGCHead *));
  if (!iter) return 1; // This might end badly...
  self->ob_vgc = iter;
  self->ob_a = new_size;
 }
 self->ob_vgc[self->ob_c++] = ob;
 return 1;
}
static void dependency_list_addgc(
 struct dependency_list *self, struct _DeeGCHead *ob) {
 struct _DeeGCHead **iter;
#ifdef DEE_DEBUG
 struct _DeeGCHead **end;
 end = (iter = self->ob_vgc)+self->ob_c;
 while (iter != end) { DEE_ASSERT(*iter != ob); ++iter; }
#endif
 if (self->ob_c == self->ob_a) {
  Dee_size_t new_size = self->ob_a ? self->ob_a*2 : 2;
  iter = (struct _DeeGCHead **)realloc_nz(
   self->ob_vgc,new_size*sizeof(struct _DeeGCHead *));
  if (!iter) return; // This might end badly...
  self->ob_vgc = iter;
  self->ob_a = new_size;
 }
 self->ob_vgc[self->ob_c++] = ob;
}
static int dependency_list_containsgc(
 struct dependency_list *self, struct _DeeGCHead *ob) {
 struct _DeeGCHead **iter,**end;
 end = (iter = self->ob_vgc)+self->ob_c;
 while (iter != end) if (*iter++ == ob) return 1;
 return 0;
}

#define dependency_set_size 64
struct dependency_set {
 struct dependency_list slots[dependency_set_size];
};
#define dependency_set_init(ob) memset(ob,0,sizeof(struct dependency_set))
#define dependency_set_addr2slot(addr) (((Dee_size_t)(addr))%dependency_set_size)
#define dependency_set_contains_or_addgc(self,ob) \
 dependency_list_contains_or_addgc((self)->slots+dependency_set_addr2slot(ob),ob)
#define dependency_set_contains_or_addob(self,ob) \
 dependency_list_contains_or_addob((self)->slots+dependency_set_addr2slot(ob),ob)
#define dependency_set_addgc(self,ob) \
 dependency_list_addgc((self)->slots+dependency_set_addr2slot(ob),ob)
#define dependency_set_containsgc(self,ob) \
 dependency_list_containsgc((self)->slots+dependency_set_addr2slot(ob),ob)
static void dependency_set_quit(struct dependency_set *self) {
 struct dependency_list *iter,*end;
 end = (iter = self->slots)+dependency_set_size;
 while (iter != end) { dependency_list_quit(iter); ++iter; }
}




// Step #1: Starting with 'ob', copy all references of all reachable gc objects into 'gc_refs'
//          All objects currently being visited when reaching 'ob' again are put into a dependency set
// Step #2: Starting with 'ob', for every reachable object, subtract
//          1 for every reference that an object has to itself.
// Step #3: If all objects in the dependency set have a reference count of 0, 'ob' can be deleted
//          NOTE: We can't delete the whole set, because tp_clear of 'ob' could revive it again




static void _deeobject_visit_all(
 DeeObject *ob, DeeVisitProc visit, void *data) {
 DeeTypeObject *tp_iter; DeeObject *ob_next;
 while (1) {
  ob_next = (DeeObject *)(tp_iter = Dee_TYPE(ob));
  while (1) {
   if (DeeType_HAS_SLOT(tp_iter,tp_visit))
    (*DeeType_GET_SLOT(tp_iter,tp_visit))(tp_iter,ob,visit,data);
   if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
  }
  if (ob == ob_next) break; // nothing left to check...
  ob = ob_next;
 }
}



// NOTE: Also used for dependency calculation
struct dependency_chain {
 struct dependency_chain *prev;
 struct _DeeGCHead       *entry;
};
struct copyrefs_data {
 unsigned int             id;
 struct dependency_set    deps;
 struct dependency_chain *chain;
 unsigned int             changed;
 // v set of visited non-gc objects
 struct dependency_set    visited;
};
static int _deegc_copyrefs_visitproc(
 DeeObject *ob, struct copyrefs_data *data) {
 struct _DeeGCHead *head;
 struct dependency_chain chain_entry,*chain_iter;
 if (!DeeObject_IS_GC(ob)) return dependency_set_contains_or_addob(&data->visited,ob);
 head = DeeGC_OB2HEAD(ob);
 // If this is a dependency, copy all of our's
 if (dependency_set_containsgc(&data->deps,head)) {
  chain_iter = data->chain;
  while (chain_iter && dependency_set_contains_or_addgc(
   &data->deps,chain_iter->entry)) {
   chain_iter = chain_iter->prev;
   data->changed = 1;
  }
 }
 // Make sure that we haven't visited this object yet
 if (head->gc_last_seen != data->id) {
  // Actually copy the reference counter
  head->gc_refs = _DeeRefCounter_Fetch(ob->__ob_refcnt);
  head->gc_last_seen = data->id;
  // Recursively search 'ob' while expanding the dependency chain
  chain_entry.prev = data->chain;
  chain_entry.entry = head;
  data->chain = &chain_entry;
  _deeobject_visit_all(ob,(DeeVisitProc)&_deegc_copyrefs_visitproc,data);
  data->chain = chain_entry.prev;
 }
 // Don't let the caller do the recursion (we already performed it)
 return 0;
}

static int _deegc_collectdeps_visitproc(
 DeeObject *ob, struct copyrefs_data *data) {
 struct _DeeGCHead *head;
 struct dependency_chain chain_entry,*chain_iter;
 if (!DeeObject_IS_GC(ob)) return dependency_set_contains_or_addob(&data->visited,ob);
 head = DeeGC_OB2HEAD(ob);
 // If this is a dependency, copy all of our's
 if (dependency_set_containsgc(&data->deps,head)) {
  chain_iter = data->chain;
  while (chain_iter && dependency_set_contains_or_addgc(
   &data->deps,chain_iter->entry)) {
   chain_iter = chain_iter->prev;
   data->changed = 1;
  }
 }
 // Make sure that we haven't visited this object yet
 if (head->gc_last_seen != data->id) {
  head->gc_last_seen = data->id;
  // Recursively search 'ob' while expanding the dependency chain
  chain_entry.prev = data->chain;
  chain_entry.entry = head;
  data->chain = &chain_entry;
  _deeobject_visit_all(ob,(DeeVisitProc)&_deegc_collectdeps_visitproc,data);
  data->chain = chain_entry.prev;
 }
 // Don't let the caller do the recursion (we already performed it)
 return 0;
}


struct subrefs_data {
 unsigned int           id;
 struct _DeeGCHead     *ob;
 // v set of visited non-gc objects
 struct dependency_set *visited;
};
static int _deegc_subrefs_visitproc(
 DeeObject *ob, struct subrefs_data *data) {
 struct _DeeGCHead *head;
 if (DeeObject_IS_GC(ob)) {
  head = DeeGC_OB2HEAD(ob);
  if (data->ob == head) {
   DEE_ASSERT(head->gc_refs && "GC : subrefs : reference leak");
   --head->gc_refs;
  } else if (head->gc_last_seen != data->id) {
   head->gc_last_seen = data->id;
   return 1;
  }
  return 0;
 }
 return dependency_set_contains_or_addob(data->visited,ob);
}

static int _deegc_should_delete(
 struct _DeeGCHead *head, unsigned int initial_refcnt) {
 struct copyrefs_data copy_data;
 struct subrefs_data sub_data;
 struct dependency_list *set_iter,*set_end;
 struct _DeeGCHead **iter,**end;
 //DEE_LDEBUG("[GC] Check: %s\n",DeeType_NAME(Dee_TYPE(DeeGC_HEAD2OB(head))));
 DEE_LVERBOSE3("[GC] Check: %s\n",DeeType_NAME(Dee_TYPE(DeeGC_HEAD2OB(head))));

 // Copy references And calculate dependencies
 dependency_set_init(&copy_data.deps);
 dependency_set_addgc(&copy_data.deps,head); // The object depends on itself
 copy_data.id = _dee_gc_next_visit_id++;
 copy_data.chain = NULL;
 copy_data.changed = 0;
 head->gc_refs = initial_refcnt;
 head->gc_last_seen = copy_data.id;
 dependency_set_init(&copy_data.visited);
 _deeobject_visit_all(DeeGC_HEAD2OB(head),(DeeVisitProc)&_deegc_copyrefs_visitproc,&copy_data);
 dependency_set_quit(&copy_data.visited);

 // Collect additional dependencies
 // This is required for situations like this:
 //    +-------+
 //    v       |
 //   [a] --> (b)<-+
 //            |   |
 //            v   |
 //           (c) -+
 //   ...------^
 // 'a' is the object in question
 // 1. a --> walk to b
 // 2. b --> walk to c
 // 3. c --> walk to b (not added as dependency as 'b' isn't know as dependency for a, yet)
 // 4. b --> walk to a (add as dependency)
 // >> We're able to add 'c' as dependency due to visit order in 'b'
 // ERROR:
 //   - 'c' is used externally, but we'd destroy its dependency on 'b' and implicity 'a'
 // SOLLUTION:
 //   #1: Track dependencies of potential future dependencies.
 //       + Reduces calls to visit functions
 //       - Increase overhead 
 //   #2: Keep searching for additional dependencies. (chosen for easy implementation)
 //       + Less overhead
 //       - A lot more calls to tp_visit procs
 //       - Actual implementation of 'tp_visit' may be extremely expensive
 while (copy_data.changed) {
  copy_data.changed = 0; 
  copy_data.id = _dee_gc_next_visit_id++;
  head->gc_last_seen = copy_data.id;
  dependency_set_init(&copy_data.visited);
  _deeobject_visit_all(DeeGC_HEAD2OB(head),(DeeVisitProc)&_deegc_collectdeps_visitproc,&copy_data);
  dependency_set_quit(&copy_data.visited);
 }

 // Subtract references
 // >> For all dependencies, calculate how often they reference each other an themselves.
 // >> This is required as we may only destroy an object if
 //    everything that depends on it could be destroyed as well.
 sub_data.visited = &copy_data.visited;
 set_end = (set_iter = copy_data.deps.slots)+dependency_set_size; do {
  end = (iter = set_iter->ob_vgc)+set_iter->ob_c; while (iter != end) {
   sub_data.ob = *iter++;
   sub_data.id = _dee_gc_next_visit_id++;
   sub_data.ob->gc_last_seen = sub_data.id;
   DEE_ASSERT(sub_data.ob->gc_refs && "GC : should_delete : Dead object in dependency chain");
   dependency_set_init(&copy_data.visited);
   _deeobject_visit_all(DeeGC_HEAD2OB(sub_data.ob),(DeeVisitProc)&_deegc_subrefs_visitproc,&sub_data);
   dependency_set_quit(&copy_data.visited);
  }
 } while (++set_iter != set_end);

 // Check if the entire dependency chain is dead
 set_end = (set_iter = copy_data.deps.slots)+dependency_set_size; do {
  end = (iter = set_iter->ob_vgc)+set_iter->ob_c;
  while (iter != end) if ((*iter++)->gc_refs) { dependency_set_quit(&copy_data.deps); return 0; }
 } while (++set_iter != set_end);
 DEE_ASSERT(head->gc_refs == 0);

 // NOTE: We can't just delete everything from the dependency set, as
 //       after calling the first tp_clear function, we can no longer
 //       guaranty that any of our supposed dead objects are still dead.
 //       >> This is because tp_clear is allowed to revive fainted objects.
 dependency_set_quit(&copy_data.deps);

 return 1;
}


#define DEE_GC_DESTROY_ABORTED  0 // Destruction aborted (e.g.: incref through weakref)
#define DEE_GC_DESTROY_SUCCESS  1 // Destruction successful (caller must free the object)
#define DEE_GC_DESTROY_DONTFREE 2 // Destroyed, but don't free (weak references still exist)
#define DEE_GC_DESTROY_PARTIAL  3 // Partially destroyed (e.g.: revive in tp_clear)

#ifndef __INTELLISENSE__
// Load the free object function in gc-mode
#define GC
#include "object.core.destroy.inl"
// Load the free object function in gc-aggressive-mode
#define GC
#define AGGRESSIVE
#include "object.core.destroy.inl"
#else
DEE_STATIC_INLINE(int) _DeeObject_InternalDestroyGC(
 DEE_A_INOUT DeeObject *ob, struct _DeeGCHead **next, unsigned int initial_refcnt);
DEE_STATIC_INLINE(int) _DeeObject_InternalDestroyGCAggressive(
 DEE_A_INOUT DeeObject *ob, struct _DeeGCHead **next, unsigned int initial_refcnt);
#endif


Dee_size_t DeeGC_CollectNow(void) {
 Dee_size_t result; int temp; unsigned int initial_refcnt;
 struct _DeeGCHead *iter,*next;
 if (!DeeAtomicMutex_TryAcquire(&_dee_gc_collect_lock)) {
  // Somewhere, garbage is already being collected (don't do anything; return 0)
  // TODO: We should probably wait for them instead and return the same as them...
  return 0;
 }
 result = 0;
 _dee_gc_tracked_lock_acquire();
#ifdef _DeeGC_AssertTrackedObjectListConsistent
 _DeeGC_AssertTrackedObjectListConsistent();
#endif
 iter = _dee_gc_tracked;
 while (iter) {
  // Do some basic checks about the GC entry
  DEE_ASSERTF(iter->gc_next != iter && iter->gc_prev != iter &&
             (iter->gc_next != iter->gc_prev || !iter->gc_prev) &&
              "Infinite loop in GC",FORMAT_DEBUG_NEW_INFO(iter));
  initial_refcnt = _DeeRefCounter_Fetch(DeeGC_HEAD2OB(iter)->__ob_refcnt);
  if (initial_refcnt != 0 && // Rare case: The object is currently being destroyed
      _deegc_should_delete(iter,initial_refcnt)) {
   // Must destroy this object
   DEE_LVERBOSE1("[GC] Delete: %q\n",DeeType_NAME(Dee_TYPE(DeeGC_HEAD2OB(iter))));
   temp = _DeeObject_InternalDestroyGC(DeeGC_HEAD2OB(iter),&next,initial_refcnt);
   if DEE_LIKELY(temp != DEE_GC_DESTROY_ABORTED) {
    ++result; // We managed to do something!
    if DEE_LIKELY(temp != DEE_GC_DESTROY_PARTIAL) {
     // Object got destroyed --> remove it from the list of tracked objects
     DEE_ASSERT(_DeeRefCounter_Fetch(DeeGC_HEAD2OB(iter)->__ob_refcnt) == 0 &&
                "non-zero reference counter after object has been destroyed by the gc. "
                "There should be no way this could happen, because new "
                "reference during destruction abort said destruction. "
                ""
                "The only way this can happen is if the gc destroy method is broken.");
     // If '_DeeObject_InternalDestroyGC' returned DEE_GC_DESTROY_DONTFREE,
     // a weakref is still holding onto the object, meaning we can't destroy it.
     if (temp != DEE_GC_DESTROY_DONTFREE) {
      // Finally free the object
      free_nn(iter); // technically: DeeGC_Free(DeeGC_HEAD2OB(iter));
     }
    }
   }
   iter = next;
  } else {
   iter = iter->gc_next;
  }
 }
 _dee_gc_tracked_lock_release();

 DeeAtomicMutex_Release(&_dee_gc_collect_lock);
 return result;
}

DEE_STATIC_INLINE(void) _DeeGC_CollectAggressive(void) {
 struct _DeeGCHead *iter,*next;
#ifdef _DeeGC_AssertTrackedObjectListConsistent
 _DeeGC_AssertTrackedObjectListConsistent();
#endif
 iter = _dee_gc_tracked;
 while (iter) {
  unsigned int initial_refcnt = _DeeRefCounter_Fetch(DeeGC_HEAD2OB(iter)->__ob_refcnt);
  DEE_ASSERT(iter->gc_next != iter &&
             iter->gc_prev != iter &&
             (iter->gc_next != iter->gc_prev || !iter->gc_prev) &&
             "Infinite loop in GC");
  if (initial_refcnt != 0 && // Rare case: The object is currently being destroyed
      _deegc_should_delete(iter,initial_refcnt)) {
   int temp; // Must destroy this object
   DEE_LVERBOSE1("Destroying GC object through aggressive collect: Instance of %q\n",
                 DeeType_NAME(Dee_TYPE(DeeGC_HEAD2OB(iter))));
   temp = _DeeObject_InternalDestroyGCAggressive(DeeGC_HEAD2OB(iter),&next,initial_refcnt);
   if (DEE_LIKELY(temp != 0)) {
    // Object got destroyed --> remove it from the list of tracked objects
#if 0 // v this is allowed to happen in aggressive mode!
    DEE_ASSERT(_DeeRefCounter_Fetch(DeeGC_HEAD2OB(iter)->__ob_refcnt) == 0 &&
               "non-zero reference counter after object has been destroyed by the gc. "
               "There should be no way this could happen, because new "
               "reference during destruction abort said destruction. "
               ""
               "The only way this can happen is if the gc destroy method is broken.");
#endif
    // If '_DeeObject_InternalDestroyGC' returned 2,
    // a weakref is still holding onto the object
    if (temp != 2) {
     // Finally free the object
     free_nn(iter); // aka: DeeGC_Free(DeeGC_HEAD2OB(iter));
    }
   }
   iter = next;
  } else {
   DEE_LDEBUG("[gc:aggressive] Skipping: %s:%k\n",
              DeeType_NAME(Dee_TYPE(DeeGC_HEAD2OB(iter))),
              DeeGC_HEAD2OB(iter));
   iter = iter->gc_next;
  }
 }
}


DEE_A_GC_UNTRACKED DEE_A_RET_EXCEPT_REF DeeObject *
_DeeGC_TpAlloc(DEE_A_IN DeeTypeObject *tp) {
 DeeObject *result;
 if ((result = (DeeObject *)DeeGC_Malloc(
  DeeType_GET_SLOT(tp,tp_instance_size))
  ) != NULL) DeeObject_INIT(result,tp);
 return result;
}
void _DeeGC_TpFree(
 DEE_A_IN DeeTypeObject *DEE_UNUSED(tp),
 DEE_A_GC_UNTRACKED DeeObject *ob) {
 //DeeGC_TrackedRem(ob);
#if 0
 DeeGC_Free(ob);
#else
 DEE_ASSERT(ob); // Inlined for speed
 DEE_ASSERT(DeeGC_OB2HEAD(ob)->gc_prev == NULL &&
            DeeGC_OB2HEAD(ob)->gc_next == NULL &&
            "GC object still tracked in free function");
 free_nn(DeeGC_OB2HEAD(ob));
#endif
}

void _DeeGCObject_Delete(DEE_A_INOUT DeeObject *self) {
 _DeeGCObject_DELETE(self);
}


#ifndef __INTELLISENSE__
#define GC_TRYMALLOC
#define GC_NOCOLLECT
#define GC_DBGMALLOC
#include "object.core.gc_malloc.inl"

#define GC_TRYMALLOC
#define GC_NOCOLLECT
#include "object.core.gc_malloc.inl"

#define GC_TRYMALLOC
#define GC_DBGMALLOC
#include "object.core.gc_malloc.inl"

#define GC_TRYMALLOC
#include "object.core.gc_malloc.inl"

#define GC_NOCOLLECT
#define GC_DBGMALLOC
#include "object.core.gc_malloc.inl"

#define GC_NOCOLLECT
#include "object.core.gc_malloc.inl"

#define GC_DBGMALLOC
#include "object.core.gc_malloc.inl"

#include "object.core.gc_malloc.inl"
#endif

#undef FORMAT_DEBUG_NEW_INFO

DEE_DECL_END

#endif /* !GUARD_DEEMON_GC_C */
