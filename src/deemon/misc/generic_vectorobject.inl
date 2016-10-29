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
 
// Began implementing this, but then descided to scrap it...

#ifdef __INTELLISENSE__
#include <deemon/__conf.inl>
#include <deemon/optional/object_refcnt.h>
#define DeeVectorObject   __intellisense__DeeObjectVectorObject
#define DeeVector(x)      __intellisense__DeeObjectVector_##x
#define DeeVectorElem     DeeObject *
#if 1
#define T_init_copy(x,y)  (Dee_IncRef(*(x) = *(y)),0)
#else
#define T_init_copy_noexcept
#define T_init_copy(x,y)  Dee_INCREF(*(x) = *(y))
#endif
#define T_quit(x)         Dee_DECREF(x)
#define T_BYRVALUE
#define DeeVector_LOCKED  2
#endif

//////////////////////////////////////////////////////////////////////////
// Options (everything gets #undef'ed at the end):
//////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------
// -- NAME              -- Meaning         -- Description
// ---------------------------------------------------------------
//  DeeVector_HEADERONLY - defined()        - Only generate the header
//  DeeVector_SOURCEONLY - defined()        - Only generate the source
//  DeeVectorObject      - DeeIntVector     - Name of the vector type
//  DeeVector(x)         - DeeIntVector_##x - Namespace for member functions
//  DeeVectorElem        - int              - Type of the vector element
// -- Optional macros ---------------------------------------------
//  DeeVector_LOCKED     - 0,1,2            - If defined, the vector will include a lock (0: no lock; 1: atomic lock; 2: semaphore lock)
//  DeeVector_DECL(T)    - static T         - Declaration of a member function
//  DeeVector_IMPL(T)    - static T         - Implementation of a member function
//  T_init(x)            - (*(x) = 0,0)     - Default-Initialize an instance of 'DeeVectorElem' (return non-zero on error)
//                                            If not defined, object can't be default-constructed
//  T_quit(x)            - (void)0          - Finalize an instance of 'DeeVectorElem' (don't define if no-op)
//  T_init_noexcept      - defined()        - Define, if 'T_init' never fails
//  T_init_copy(x,y)     - (*(x) = *(y),0)  - Copy-Initialize an instance of 'DeeVectorElem' (return non-zero on error; don't define if simple memcpy)
//  T_init_copy_noexcept - defined()        - Define, if 'T_init_copy' never fails
//  T_init_move(x,y)     - (*(x) = *(y),0)  - Move-Initialize an instance of 'DeeVectorElem' (return non-zero on error; don't define if simple memcpy)
//  T_init_move_noexcept - defined()        - Define, if 'T_init_move' never fails
//  T_check              - ...              - Optional define for assertions in debug mode (used to check the validity of a pointer to a 'DeeVectorElem'; NOTE: Non-null checks are already done internally)
//  T_BYRVALUE           - defined()        - Define, to have functions like append take an r-value instead of a pointer
//  T_DIRECTMOVE         - defined()        - If defined, T_init_move+T_quit can be merged into a simple assign (T_init_move(dst,src),T_quit(src) --> *dst = *src)
// ---------------------------------------------------------------


#ifndef DeeVectorObject /* Vector method namespace */
#error "Must '#define DeeVectorObject ...' before #including this file"
#endif
#ifndef DeeVector     /* Vector typename */
#error "Must '#define DeeVector(x) ...' before #including this file"
#endif
#ifndef DeeVectorElem /* Element typename */
#error "Must '#define DeeVectorElem ...' before #including this file"
#endif

//////////////////////////////////////////////////////////////////////////
// Optional declaration macros
#ifndef DeeVector_DECL
#define DeeVector_DECL DEE_STATIC_INLINE
#endif
#ifndef DeeVector_IMPL
#define DeeVector_IMPL DEE_STATIC_INLINE
#endif
#ifndef DeeVector_LOCKED
#define DeeVector_LOCKED 0
#endif

//////////////////////////////////////////////////////////////////////////
// Optional type operators
#ifndef T_init
#define T_init_noexcept
#endif

#ifndef T_init_copy
#define T_init_copy_noexcept
#endif /* !T_init_copy */
#ifndef T_init_move
#ifdef T_init_copy
#ifdef T_init_copy_noexcept
#define T_init_move_noexcept
#endif /* T_init_copy_noexcept */
#define T_init_move T_init_copy
#else /* T_init_copy */
#define T_init_move_noexcept
#endif /* !T_init_copy */
#endif /* !T_init_move */


#define T  DeeVector(TElem)
#ifdef T_BYRVALUE
#define RT        T
#define RT_PTR(x) &(x)
#else
#define RT        T const *
#define RT_PTR(x) x
#endif

#ifndef DeeVector_SOURCEONLY
#if DeeVector_LOCKED == 2
#include <deemon/mp/mutex.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

typedef DeeVectorElem DeeVector(TElem);
struct DeeVectorObject {
 Dee_size_t vo_size;  /*< Used vector size. */
 Dee_size_t vo_alloc; /*< Allocated vector size. */
 T         *vo_elem;  /*< [owned][0..vo_size][owned] Vector of elements. */
#if DeeVector_LOCKED == 1
 struct DeeAtomicMutex     vo_lock;
#elif DeeVector_LOCKED == 2
 struct DeeNativeSemaphore vo_lock;
#endif
};

#if DeeVector_LOCKED == 2
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Init)(
 DEE_A_OUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitMove)(
 DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignMove)(
 DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Clear)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Flush)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Reserve)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Reverse)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
#else
DeeVector_DECL(void) DeeVector(Init)(
 DEE_A_OUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(void) DeeVector(InitMove)(
 DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(void) DeeVector(AssignMove)(
 DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(void) DeeVector(Clear)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(void) DeeVector(Flush)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(void) DeeVector(Reserve)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(void) DeeVector(Reverse)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
DeeVector_DECL(void) DeeVector(Quit)(DEE_A_IN struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitCopy)(DEE_A_OUT struct DeeVectorObject *self, DEE_A_IN struct DeeVectorObject const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitCat)(
 DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) RT const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) RT const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignCopy)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN struct DeeVectorObject const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignCat)(
 DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) RT const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) RT const *rhsv) DEE_ATTRIBUTE_NONNULL((1));
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) DeeVector(Size)(DEE_A_IN struct DeeVectorObject const *self) { return self->vo_size; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED T *) DeeVector(Elem)(DEE_A_IN struct DeeVectorObject const *self) { return self->vo_elem; }
#if defined(T_init_copy_noexcept) || DeeVector_LOCKED == 2
DeeVector_DECL(void) DeeVector(FillAssign)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT filler) DEE_ATTRIBUTE_NONNULL((1));
#else
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(FillAssign)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT filler) DEE_ATTRIBUTE_NONNULL((1));
#endif
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AppendElem)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT right) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AppendVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InsertElem)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN RT right) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InsertVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(ResizeWith)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t new_size, DEE_A_IN RT elem) DEE_ATTRIBUTE_NONNULL((1));
#ifdef T_init
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Resize)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t new_size) DEE_ATTRIBUTE_NONNULL((1));
#endif
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(PopFront)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(PopBack)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Pop)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_DECL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Erase)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DEE_DECL_END
#endif /* DeeVector_SOURCEONLY */


#ifndef DeeVector_HEADERONLY
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stddef.h> /* NULL */
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H */
#include <deemon/error.h>     /* DeeError_NoMemory() */
#include <deemon/deemonrun.h> /* Dee_CollectMemory() */
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN
#ifdef __INTELLISENSE__
#define DeeVector_DECL DEE_STATIC_INLINE
#define DeeVector_IMPL(T) T
#endif

#if DeeVector_LOCKED == 1
#define V_INIT(vec,...)     do{DeeAtomicMutex_Init(&(vec)->vo_lock);}while(0)
#define V_ACQUIRE(vec,...)  do{DeeAtomicMutex_Acquire(&(vec)->vo_lock);}while(0)
#define V_RELEASE(vec)      DeeAtomicMutex_Release(&(vec)->vo_lock)
#elif DeeVector_LOCKED == 2
#define V_INIT(vec,...)     do{if(DeeNativeSemaphore_Init(&(vec)->vo_lock,0) != 0){__VA_ARGS__;}}while(0)
#define V_QUIT(vec)         DeeNativeSemaphore_Quit(&(vec)->vo_lock)
#define V_ACQUIRE(vec,...)  do{if(DeeNativeSemaphore_Acquire((struct DeeNativeSemaphore *)&(vec)->vo_lock)!=0){__VA_ARGS__;}}while(0)
#define V_ACQUIRENX(vec)    DeeNativeSemaphore_AcquireNoexcept((struct DeeNativeSemaphore *)&(vec)->vo_lock)
#define V_RELEASE(vec)      DeeNativeSemaphore_ReleaseNoexcept((struct DeeNativeSemaphore *)&(vec)->vo_lock)
#else
#define V_ACQUIRE(vec,...)  do{}while(0)
#define V_RELEASE(vec)      (void)0
#endif

#ifndef V_ACQUIRENX
#define V_ACQUIRENX(vec) V_ACQUIRE(vec,{})
#endif


#if DeeVector_LOCKED == 2
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Init)
#else
DeeVector_IMPL(void) DeeVector(Init)
#endif
(DEE_A_OUT struct DeeVectorObject *self) {
 DEE_ASSERT(self);
#ifdef V_INIT
 V_INIT(self,return -1);
#endif
 self->vo_size = 0;
 self->vo_alloc = 0;
 self->vo_elem = NULL;
#if DeeVector_LOCKED == 2
 return 0;
#endif
}
DeeVector_IMPL(void) DeeVector(Quit)(
 DEE_A_IN struct DeeVectorObject *self) {
#ifdef T_quit
 T *iter,*end;
 end = (iter = self->vo_elem)+self->vo_size;
 while (iter != end) { T_quit(iter); ++iter; }
#endif
 free(self->vo_elem);
}
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitCopy)(
 DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_IN struct DeeVectorObject const *right) {
 T *new_elem;
 DEE_ASSERT(self);
 DEE_ASSERT(right);
#ifdef V_INIT
 V_INIT(self,return -1);
#endif
again:
#ifdef V_QUIT
 V_ACQUIRE(right,{ V_QUIT(self); return -1; });
#else
 V_ACQUIRE(right,return -1);
#endif
 self->vo_size = self->vo_alloc = right->vo_size;
 if (self->vo_size) {
  while ((new_elem = (T *)malloc_nz(
   self->vo_size*sizeof(T))) == NULL) {
   V_RELEASE(right);
   if (Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
#ifdef V_QUIT
   V_QUIT(self);
#endif
   return -1;
  }
  self->vo_elem = new_elem;
#ifdef T_init_copy
  {
   T *src = right->vo_elem,*end = new_elem+self->vo_size;
   while (new_elem != end) {
#ifdef T_init_copy_noexcept
    T_init_copy(new_elem,src);
#else
    if (T_init_copy(new_elem,src) != 0) {
#ifdef T_quit
     while (new_elem != self->vo_elem) { --new_elem; T_quit(new_elem); }
#endif
#ifdef V_QUIT
     V_QUIT(self);
#endif
     return -1;
    }
#endif
    ++src,++new_elem;
   }
  }
#else
  memcpy(new_elem,right->vo_elem,
         self->vo_size*sizeof(T));
#endif
 } else {
  self->vo_elem = NULL;
 }
 return 0;
}
#if DeeVector_LOCKED == 2
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitMove)
#else
DeeVector_IMPL(void) DeeVector(InitMove)
#endif
(DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) {
 DEE_ASSERT(self);
 DEE_ASSERT(right);
#ifdef V_INIT
 V_INIT(self,return -1);
#endif
#ifdef V_QUIT
 V_ACQUIRE(right,{V_QUIT(self); return -1;});
#else
 V_ACQUIRE(right,{return -1;});
#endif
 self->vo_size = right->vo_size;
 self->vo_alloc = right->vo_alloc;
 self->vo_elem = right->vo_elem;
 right->vo_size = 0;
 right->vo_alloc = 0;
 right->vo_elem = NULL;
 V_RELEASE(right);
#if DeeVector_LOCKED == 2
 return 0;
#endif
}
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InitCat)(
 DEE_A_OUT struct DeeVectorObject *self,
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) RT const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) RT const *rhsv) {
 Dee_size_t total_size; T *elemv;
 DEE_ASSERT(self);
 DEE_ASSERT(!lhsc || lhsv);
 DEE_ASSERT(!rhsc || rhsv);
#ifdef V_INIT
 V_INIT(self,return -1);
#endif
 total_size = lhsc+rhsc;
 self->vo_size = self->vo_alloc = total_size;
 if (total_size) {
  while ((elemv = (T *)malloc_nz(total_size*sizeof(T))) == NULL) {
   if (Dee_CollectMemory()) continue;
   DeeError_NoMemory();
#ifdef V_QUIT
   V_QUIT(self);
#endif
   return -1;
  }
  self->vo_elem = elemv;
#ifdef T_init_copy
  while (lhsc--) {
#ifdef T_init_copy_noexcept
   T_init_copy(elemv,lhsv);
#else
   if (T_init_copy(elemv,lhsv) != 0) {
err_elemv:
#ifdef T_quit
    while (elemv != self->vo_elem) { --elemv; T_quit(elemv); }
#endif
    free_nn(self->vo_elem);
#ifdef V_QUIT
    V_QUIT(self);
#endif
    return -1;
   }
#endif
   ++lhsv,++elemv;
  }
  while (rhsc--) {
#ifdef T_init_copy_noexcept
   T_init_copy(elemv,rhsv);
#else
   if (T_init_copy(elemv,rhsv) != 0) goto err_elemv;
#endif
   ++rhsv,++elemv;
  }
#else
  memcpy(elemv,lhsv,lhsc*sizeof(T));
  memcpy(elemv+lhsc,rhsv,rhsc*sizeof(T));
#endif
 } else {
  self->vo_elem = NULL;
 }
 return 0;
}

DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignCopy)(
 DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_IN struct DeeVectorObject const *right) {
 T *old_elem,*new_elem;
 Dee_size_t new_size;
#ifdef T_quit
 Dee_size_t old_size;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(right);
 V_ACQUIRE(right,return -1);


}

#if DeeVector_LOCKED == 2
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignMove)
#else
DeeVector_IMPL(void) DeeVector(AssignMove)
#endif
(DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_INOUT struct DeeVectorObject *right) {
 T *old_elem,*new_elem;
 Dee_size_t new_size,new_alloc;
#ifdef T_quit
 T *iter;
 Dee_size_t old_size;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(right);
 V_ACQUIRE(right,return -1);
 new_size = right->vo_size;
 new_alloc = right->vo_alloc;
 new_elem = right->vo_elem;
 right->vo_size = 0;
 right->vo_alloc = 0;
 right->vo_elem = NULL;
 V_RELEASE(right);
 V_ACQUIRE(self,{
  V_ACQUIRENX(right);
  right->vo_size = new_size;
  right->vo_alloc = new_alloc;
  right->vo_elem = new_elem;
  V_RELEASE(right);
  return -1;
 });
 old_elem = self->vo_elem;
#ifdef T_quit
 old_size = self->vo_size;
#endif
 self->vo_elem = new_elem;
 self->vo_size = new_size;
 self->vo_alloc = new_alloc;
 V_RELEASE(self);
#ifdef T_quit
 iter = old_elem+old_size;
 while (iter != old_elem) { --iter; T_quit(iter); }
#endif
 free(old_elem);
#if DeeVector_LOCKED == 2
 return 0;
#endif
}
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignCat)(
 DEE_A_INOUT struct DeeVectorObject *self,
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) RT const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) RT const *rhsv) {
 Dee_size_t total_size; T *elemv,*old_elem;
#if defined(T_init_copy) || defined(T_quit)
 T *iter;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(!lhsc || lhsv);
 DEE_ASSERT(!rhsc || rhsv);
 total_size = lhsc+rhsc;
 if (total_size) {
  while ((elemv = (T *)malloc_nz(total_size*sizeof(T))) == NULL) {
   if (Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->vo_elem = elemv;
#ifdef T_init_copy
  iter = elemv;
  while (lhsc--) {
#ifdef T_init_copy_noexcept
   T_init_copy(iter,lhsv);
#else
   if (T_init_copy(iter,lhsv) != 0) {
err_elemv:
#ifdef T_quit
    while (iter != self->vo_elem) { --iter; T_quit(iter); }
#endif
    free_nn(self->vo_elem);
#ifdef V_QUIT
    V_QUIT(self);
#endif
    return -1;
   }
#endif
   ++lhsv,++iter;
  }
  while (rhsc--) {
#ifdef T_init_copy_noexcept
   T_init_copy(iter,rhsv);
#else
   if (T_init_copy(iter,rhsv) != 0) goto err_elemv;
#endif
   ++rhsv,++iter;
  }
#else
  memcpy(elemv,lhsv,lhsc*sizeof(T));
  memcpy(elemv+lhsc,rhsv,rhsc*sizeof(T));
#endif
 } else elemv = NULL;
#ifdef T_quit
 V_ACQUIRE(self,{
  iter = elemv+total_size;
  while (iter != elemv) { --iter; T_quit(iter); }
  free(elemv);
  return -1;
 });
#else
 V_ACQUIRE(self,{ free(elemv); return -1; });
#endif
 old_elem = self->vo_elem;
#ifdef T_quit
 iter = old_elem+self->vo_size;
#endif
 self->vo_elem = elemv;
 self->vo_size = total_size;
 self->vo_alloc = total_size;
 V_RELEASE(self);
#ifdef T_quit
 while (iter != elemv) { --iter; T_quit(iter); }
#endif
 free(elemv);
 return 0;
}
#if DeeVector_LOCKED == 2
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Clear)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Flush)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Reserve)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Reverse)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
#else
DeeVector_IMPL(void) DeeVector(Clear)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(void) DeeVector(Flush)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(void) DeeVector(Reserve)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(void) DeeVector(Reverse)(DEE_A_INOUT struct DeeVectorObject *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
#if defined(T_init_copy_noexcept) || DeeVector_LOCKED == 2
DeeVector_IMPL(void) DeeVector(FillAssign)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT filler) DEE_ATTRIBUTE_NONNULL((1));
#else
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(FillAssign)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT filler) DEE_ATTRIBUTE_NONNULL((1));
#endif
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AssignVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AppendElem)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN RT right) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(AppendVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InsertElem)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN RT right) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(InsertVector)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) RT const *elemv) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(ResizeWith)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t new_size, DEE_A_IN RT elem) DEE_ATTRIBUTE_NONNULL((1));
#ifdef T_init
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Resize)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t new_size) DEE_ATTRIBUTE_NONNULL((1));
#endif
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(PopFront)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(PopBack)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Pop)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_OUT_OPT RT *elem) DEE_ATTRIBUTE_NONNULL((1));
DeeVector_IMPL(DEE_A_RET_EXCEPT(-1) int) DeeVector(Erase)(DEE_A_INOUT struct DeeVectorObject *self, DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
DEE_DECL_END
#undef V_INIT
#undef V_QUIT
#undef V_ACQUIRE
#undef V_ACQUIRENX
#undef V_RELEASE
#endif /* DeeVector_HEADERONLY */

#undef DeeVector
#undef DeeVectorElem
#undef DeeVectorObject
#undef DeeVector_DECL
#undef DeeVector_IMPL
#undef DeeVector_LOCKED
#undef RT
#undef RT_PTR
#undef T
#undef T_BYRVALUE
#undef T_DIRECTMOVE
#undef T_init
#undef T_init_copy
#undef T_init_copy_noexcept
#undef T_init_move
#undef T_init_move_noexcept
#undef T_init_noexcept
#undef T_quit

