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
#ifndef GUARD_DEEMON_OPTIONAL_ATOMIC_MUTEX_H
#define GUARD_DEEMON_OPTIONAL_ATOMIC_MUTEX_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>

DEE_DECL_BEGIN


struct DeeAtomicMutex { // aka. spinlock
#ifdef __INTELLISENSE__
 /*atomic*/ unsigned char m_value;
#else
 /*atomic*/ DEE_TYPES_UINT8_T m_value;
#endif
};
#define DeeAtomicMutex_INIT()         {0}
#ifndef DEE_WITHOUT_THREADS
#define DeeAtomicMutex_Init(ob)       ((ob)->m_value=0)
#define DeeAtomicMutex_InitLocked(ob) ((ob)->m_value=1)
#define DeeAtomicMutex_TryAcquire(m) \
 DeeAtomic8_CompareExchangeStrong((m)->m_value,\
  0,1,memory_order_seq_cst,memory_order_seq_cst)
#define DeeAtomicMutex_Acquire(m) \
 do{while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeWeak((m)->m_value,\
  0,1,memory_order_seq_cst,memory_order_seq_cst))DeeThread_Yield();}while(0)
#define DeeAtomicMutex_WaitFor(m) \
 do{while DEE_UNLIKELY(DeeAtomic8_Load((m)->m_value,memory_order_seq_cst))DeeThread_Yield();}while(0)
#define DeeAtomicMutex_WaitForRelaxed(m) \
 do{while DEE_UNLIKELY(DeeAtomic8_Load((m)->m_value,memory_order_seq_cst))DeeThread_SleepNoInterrupt(1);}while(0)
// v Less overhead, but way more delay if we do actually fail the lock on the first try
#define DeeAtomicMutex_AcquireRelaxed(m) \
 do{while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeStrong((m)->m_value,\
  0,1,memory_order_seq_cst,memory_order_seq_cst))DeeThread_SleepNoInterrupt(1);}while(0)
#define DeeAtomicMutex_RELEASE(m) \
 DeeAtomic8_Store((m)->m_value,0,memory_order_seq_cst)
#ifdef DEE_DEBUG
#define DeeAtomicMutex_RELEASE_D(m) (_DeeFlag_NoAssert ? \
 DeeAtomicMutex_RELEASE(m) : DEE_ASSERT(DeeAtomic8_CompareExchangeStrong(\
 (m)->m_value,1,0,memory_order_seq_cst,memory_order_seq_cst) && \
  "Atomic mutex locking error (Either you are tinkering with the lock counter, "\
  "or you forgot to call 'DeeAtomicMutex_TryAcquire' / 'DeeAtomicMutex_Acquire')"))
#endif
#ifdef DeeAtomicMutex_RELEASE_D
# define DeeAtomicMutex_Release DeeAtomicMutex_RELEASE_D
#else /* DeeAtomicMutex_RELEASE_D */
# define DeeAtomicMutex_Release DeeAtomicMutex_RELEASE
#endif /* !DeeAtomicMutex_RELEASE_D */
#ifndef DEE_PRIVATE_THREAD_SLEEP_DEFINED
#define DEE_PRIVATE_THREAD_SLEEP_DEFINED 1
DEE_FUNC_DECL(void) DeeThread_SleepNoInterrupt(DEE_A_IN unsigned int msecs);
#endif
#ifndef DEE_PRIVATE_THREAD_YIELD_DEFINED
#define DEE_PRIVATE_THREAD_YIELD_DEFINED 1
DEE_FUNC_DECL(void) DeeThread_Yield(void);
#endif /* !DEE_PRIVATE_THREAD_YIELD_DEFINED */
#else /* !DEE_WITHOUT_THREADS */
#define DeeAtomicMutex_Init(ob)          (void)0
#define DeeAtomicMutex_InitLocked(ob)    (void)0
#define DeeAtomicMutex_TryAcquire(m)     1
#define DeeAtomicMutex_Acquire(m)        do{}while(0)
#define DeeAtomicMutex_AcquireRelaxed(m) do{}while(0)
#define DeeAtomicMutex_RELEASE(m)        (void)0
#define DeeAtomicMutex_Release(m)        (void)0
#endif /* !DEE_WITHOUT_THREADS */

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_ATOMIC_MUTEX_H */
