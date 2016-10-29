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
#include "gc.c"
#define GC_TRYMALLOC
#define GC_DBGMALLOC
#define GC_NOCOLLECT
#endif

#ifdef GC_DBGMALLOC
#ifdef GC_NOCOLLECT
#ifdef GC_TRYMALLOC
#define FUNC(name)      DeeGC_TryWeak##name##_d
#else
#define FUNC(name)      DeeGC_Weak##name##_d
#endif
#else
#ifdef GC_TRYMALLOC
#define FUNC(name)      DeeGC_Try##name##_d
#else
#define FUNC(name)      DeeGC_##name##_d
#endif
#endif
#if defined(DEBUG_NEW_PRIVATE_DISABLED) || !defined(DEE_DEBUG)
#define DEBUG_ARGUMENTS , DEE_A_IN_Z_OPT char const *DEE_UNUSED(access)
#else
#define DEBUG_ARGUMENTS , DEE_A_IN_Z_OPT char const *access
#endif
#else
#ifdef GC_NOCOLLECT
#ifdef GC_TRYMALLOC
#define FUNC(name)      DeeGC_TryWeak##name
#else
#define FUNC(name)      DeeGC_Weak##name
#endif
#else
#ifdef GC_TRYMALLOC
#define FUNC(name)      DeeGC_Try##name
#else
#define FUNC(name)      DeeGC_##name
#endif
#endif
#define DEBUG_ARGUMENTS /* nothing */
#endif
#ifdef DeeGC_Malloc
#undef DeeGC_Malloc
#undef DeeGC_Calloc
#undef DeeGC_Realloc
#undef DeeGC_Free
#undef DeeGC_WeakMalloc
#undef DeeGC_WeakCalloc
#undef DeeGC_WeakRealloc
#undef DeeGC_TryMalloc
#undef DeeGC_TryCalloc
#undef DeeGC_TryRealloc
#undef DeeGC_TryWeakMalloc
#undef DeeGC_TryWeakCalloc
#undef DeeGC_TryWeakRealloc
#endif

#ifdef GC_TRYMALLOC
DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL)
#else
DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL)
#endif
void *FUNC(Malloc)(DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 struct _DeeGCHead *result;
 while ((result = (struct _DeeGCHead *)
#if defined(GC_DBGMALLOC) && !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(DEE_DEBUG)
  DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(sizeof(struct _DeeGCHead)+s,access)
#else
  malloc_nz(sizeof(struct _DeeGCHead)+s)
#endif
  ) == NULL) {
#ifndef GC_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef GC_TRYMALLOC
  DeeError_NoMemory();
#endif
  return NULL;
 }
 result->gc_last_seen = GC_VISITID_NEVER_SEEN;
#ifdef DEE_DEBUG
 result->gc_prev = NULL;
 result->gc_next = NULL;
#endif
 return (void *)DeeGC_HEAD2OB(result);
}

#ifdef GC_TRYMALLOC
DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL)
#else
DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL)
#endif
void *FUNC(Calloc)(DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 struct _DeeGCHead *result;
 while ((result = (struct _DeeGCHead *)
#if defined(GC_DBGMALLOC) && !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(DEE_DEBUG)
  DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(1,sizeof(struct _DeeGCHead)+s,access)
#else
  calloc_nz(1,sizeof(struct _DeeGCHead)+s)
#endif
  ) == NULL) {
#ifndef GC_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef GC_TRYMALLOC
  DeeError_NoMemory();
#endif
  return NULL;
 }
 result->gc_last_seen = GC_VISITID_NEVER_SEEN;
/* // Calloc makes this unnecessary
#ifdef DEE_DEBUG
 result->gc_prev = NULL;
 result->gc_next = NULL;
#endif
*/
 return (void *)DeeGC_HEAD2OB(result);
}

#ifdef GC_TRYMALLOC
DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL)
#else
DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL)
#endif
void *FUNC(Realloc)(
 DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 struct _DeeGCHead *result;
 while ((result = (struct _DeeGCHead *)
#if defined(GC_DBGMALLOC) && !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(DEE_DEBUG)
  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(DeeGC_OB2HEAD(p),sizeof(struct _DeeGCHead)+s,access)
#else
  realloc_nnz(DeeGC_OB2HEAD(p),sizeof(struct _DeeGCHead)+s)
#endif
  ) == NULL) {
#ifndef GC_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef GC_TRYMALLOC
  DeeError_NoMemory();
#endif
  return NULL;
 }
#ifdef DEE_DEBUG
 DEE_ASSERT(result->gc_prev == NULL &&
            result->gc_next == NULL &&
            "Can't reallocate tracked gc object");
#endif
 return (void *)DeeGC_HEAD2OB(result);
}

#ifndef GC_NOCOLLECT
#ifndef GC_TRYMALLOC
void FUNC(Free)(DEE_A_IN void *p DEBUG_ARGUMENTS) {
 DEE_ASSERT(p);
 DEE_ASSERT(DeeGC_OB2HEAD(p)->gc_prev == NULL &&
            DeeGC_OB2HEAD(p)->gc_next == NULL &&
            "GC object still tracked in free function");
#if defined(GC_DBGMALLOC) && !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(DEE_DEBUG)
 DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN(DeeGC_OB2HEAD(p),access);
#else
 free_nn(DeeGC_OB2HEAD(p));
#endif
#ifdef _DeeGC_AssertTrackedObjectListConsistent
 _dee_gc_tracked_lock_acquire();
 _DeeGC_AssertTrackedObjectListConsistent();
 _dee_gc_tracked_lock_release();
#endif
}
#endif
#endif

#undef FUNC
#undef DEBUG_ARGUMENTS
#ifdef GC_TRYMALLOC
#undef GC_TRYMALLOC
#endif
#ifdef GC_DBGMALLOC
#undef GC_DBGMALLOC
#endif
#ifdef GC_NOCOLLECT
#undef GC_NOCOLLECT
#endif
