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
#include "object.c"
#define OB_TRYMALLOC
#define OB_DBGMALLOC
#define OB_NOCOLLECT
#endif

#if defined(OB_DBGMALLOC) && defined(OB_TRYMALLOC) && defined(OB_NOCOLLECT)
#define NAME(x) DeeObject_TryWeak##x##_d
#elif defined(OB_DBGMALLOC) && defined(OB_NOCOLLECT)
#define NAME(x) DeeObject_Weak##x##_d
#elif defined(OB_TRYMALLOC) && defined(OB_NOCOLLECT)
#define NAME(x) DeeObject_TryWeak##x
#elif defined(OB_NOCOLLECT)
#define NAME(x) DeeObject_Weak##x
#elif defined(OB_DBGMALLOC) && defined(OB_TRYMALLOC)
#define NAME(x) DeeObject_Try##x##_d
#elif defined(OB_DBGMALLOC)
#define NAME(x) DeeObject_##x##_d
#elif defined(OB_TRYMALLOC)
#define NAME(x) DeeObject_Try##x
#else
#define NAME(x) DeeObject_##x
#endif

#ifndef OB_DBGMALLOC
#define DEBUG_ARGUMENTS /* nothing */
#elif defined(DEBUG_NEW_PRIVATE_DISABLED) || !defined(DEE_DEBUG)
#define DEBUG_ARGUMENTS , DEE_A_IN_Z_OPT char const *DEE_UNUSED(access)
#else
#define DEBUG_ARGUMENTS , DEE_A_IN_Z_OPT char const *access
#endif

#ifdef OB_TRYMALLOC
#define EXCEPT_ANNOCATION DEE_A_RET_NOEXCEPT(NULL)
#else
#define EXCEPT_ANNOCATION DEE_A_RET_EXCEPT(NULL)
#endif

#ifdef DeeObject_Malloc
#undef DeeObject_Malloc
#undef DeeObject_Calloc
#undef DeeObject_Realloc
#undef DeeObject_TryMalloc
#undef DeeObject_TryCalloc
#undef DeeObject_TryRealloc
#undef DeeObject_Free
#undef DeeObject_WeakMalloc
#undef DeeObject_WeakCalloc
#undef DeeObject_WeakRealloc
#undef DeeObject_TryWeakMalloc
#undef DeeObject_TryWeakCalloc
#undef DeeObject_TryWeakRealloc
#endif


DEE_ATTRIBUTE_MALLOC EXCEPT_ANNOCATION void *NAME(Malloc)(
 DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 void *result;
#if !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(OB_DBGMALLOC) && defined(DEE_DEBUG)
 while ((result = DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,access)) == NULL)
#else
 while ((result = malloc_nz(s)) == NULL)
#endif
 {
#ifndef OB_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef OB_TRYMALLOC
  DeeError_NoMemory();
#endif
  break;
 }
 return result;
}
DEE_ATTRIBUTE_MALLOC EXCEPT_ANNOCATION void *NAME(Calloc)(
 DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 void *result;
#if !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(OB_DBGMALLOC) && defined(DEE_DEBUG)
 while ((result = DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(1,s,access)) == NULL)
#else
 while ((result = calloc_nz(1,s)) == NULL)
#endif
 {
#ifndef OB_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef OB_TRYMALLOC
  DeeError_NoMemory();
#endif
  break;
 }
 return result;
}

DEE_ATTRIBUTE_MALLOC EXCEPT_ANNOCATION void *NAME(Realloc)(
 DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s DEBUG_ARGUMENTS) {
 void *result;
#if !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(OB_DBGMALLOC) && defined(DEE_DEBUG)
 while ((result = DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,access)) == NULL)
#else
 while ((result = realloc_nnz(p,s)) == NULL)
#endif
 {
#ifndef OB_NOCOLLECT
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#endif
#ifndef OB_TRYMALLOC
  DeeError_NoMemory();
#endif
  break;
 }
 return result;
}

#ifndef OB_TRYMALLOC
#ifndef OB_NOCOLLECT
void NAME(Free)(DEE_A_IN DEE_A_OUT_INVALID void *p DEBUG_ARGUMENTS) {
#if !defined(DEBUG_NEW_PRIVATE_DISABLED) && defined(OB_DBGMALLOC) && defined(DEE_DEBUG)
 DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN(p,access);
#else
 free_nn(p);
#endif
}
#endif
#endif


#undef EXCEPT_ANNOCATION
#undef DEBUG_ARGUMENTS
#undef NAME
#ifdef OB_NOCOLLECT
#undef OB_NOCOLLECT
#endif
#ifdef OB_TRYMALLOC
#undef OB_TRYMALLOC
#endif
#ifdef OB_DBGMALLOC
#undef OB_DBGMALLOC
#endif
