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
#ifndef GUARD_DEEMON_SYS_EMULATED_SYSTLS_H
#define GUARD_DEEMON_SYS_EMULATED_SYSTLS_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/mp/thread.h>
#include <deemon/optional/atomic_mutex.h>
#include DEE_INCLUDE_MEMORY_API()

//////////////////////////////////////////////////////////////////////////
// === Emulated ===
DEE_DECL_BEGIN

struct DeeEmulatedSysTLSEntry;
struct DeeEmulatedSysTLSEntry {
 struct DeeEmulatedSysTLSEntry *este_next;   /*< [0..1][owned] Next thread entry. */
 Dee_tid_t                    este_thread; /*< Id of the thread this entry referrs to. */
 void                          *este_value;  /*< [?..?] Value thread-local to 'este_thread'. */
};
struct DeeEmulatedSysTLS {
 struct DeeEmulatedSysTLSEntry *est_threads; /*< [0..1][owned] Linked list of per-thread value entries. */
 struct DeeAtomicMutex          est_lock;    /*< Write exclusive lock for adding new entries. */
};
#define DeeEmulatedSysTLS_TryInit(ob) ((ob)->est_threads = NULL,DeeAtomicMutex_Init(&(ob)->est_lock),1)
#define DeeEmulatedSysTLS_Init(ob,...) do{ (ob)->est_threads = NULL; DeeAtomicMutex_Init(&(ob)->est_lock); }while(0)
#define DeeEmulatedSysTLS_Quit(ob)\
do{\
 struct DeeEmulatedSysTLSEntry *_est_iter,*_est_next;\
 if ((_est_iter = (ob)->est_threads) != NULL) {\
  while (1) {\
   _est_next = _est_iter->este_next;\
   free_nn(_est_iter);\
   if (!_est_next) break;\
   _est_iter = _est_next;\
  }\
 }\
}while(0)

DEE_STATIC_INLINE(void *) DeeEmulatedSysTLS_GetValue(struct DeeEmulatedSysTLS *self) {
 struct DeeEmulatedSysTLSEntry *iter;
 Dee_tid_t tid = DeeThread_SelfID();
 iter = self->est_threads;
 while (iter) {
  if (iter->este_thread == tid) return iter->este_value;
  iter = iter->este_next;
 }
 return NULL;
}
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int) DeeEmulatedSysTLS_SetValue(
 struct DeeEmulatedSysTLS *self, void *value) {
 struct DeeEmulatedSysTLSEntry *iter;
 Dee_tid_t tid = DeeThread_SelfID();
 iter = self->est_threads;
 while (iter) {
  if (iter->este_thread == tid) {
   iter->este_value = value;
   return 1;
  }
  iter = iter->este_next;
 }
 // Must allocate a new entry
 iter = (struct DeeEmulatedSysTLSEntry *)malloc_nz(sizeof(struct DeeEmulatedSysTLSEntry));
 if DEE_UNLIKELY(!iter) return 0; // Out of memory
 iter->este_thread = tid;
 iter->este_value = value;
 DeeAtomicMutex_AcquireRelaxed(&self->est_lock);
 iter->este_next = self->est_threads;
 self->est_threads = iter;
 DeeAtomicMutex_Release(&self->est_lock);
 return 1;
}

#define DeeEmulatedSysTLS_TryGetNofail(ob,result) (void)(*(void **)&(result)=DeeEmulatedSysTLS_GetValue(ob))
#define DeeEmulatedSysTLS_TryGet(ob,result)       (*(void **)&(result)=DeeEmulatedSysTLS_GetValue(ob),1)
#define DeeEmulatedSysTLS_TrySet                  DeeEmulatedSysTLS_SetValue
#define DeeEmulatedSysTLS_Get(ob,result,...)      do{ *(void **)&(result) = DeeEmulatedSysTLS_GetValue(ob); }while(0)
#define DeeEmulatedSysTLS_Set(ob,value,...)\
do{\
 if (!DeeEmulatedSysTLS_SetValue(ob,(void *)(value))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "emulated_tls_set(%p,%p) : Out of memory",\
                      ob,value)\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeSysTLS              DeeEmulatedSysTLS
#define DeeSysTLS_TryInit      DeeEmulatedSysTLS_TryInit
#define DeeSysTLS_Init         DeeEmulatedSysTLS_Init
#define DeeSysTLS_Quit         DeeEmulatedSysTLS_Quit
#define DeeSysTLS_TryGetNofail DeeEmulatedSysTLS_TryGetNofail
#define DeeSysTLS_TryGet       DeeEmulatedSysTLS_TryGet
#define DeeSysTLS_TrySet       DeeEmulatedSysTLS_TrySet
#define DeeSysTLS_Get          DeeEmulatedSysTLS_Get
#define DeeSysTLS_Set          DeeEmulatedSysTLS_Set

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_EMULATED_SYSTLS_H */
