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
#ifndef GUARD_DEEMON_OPTIONAL_ATOMIC_ONCE_H
#define GUARD_DEEMON_OPTIONAL_ATOMIC_ONCE_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>
#include <deemon/optional/atomic_mutex.h>

DEE_DECL_BEGIN

#ifndef DEE_WITHOUT_THREADS
#ifndef DEE_PRIVATE_THREAD_SLEEP_DEFINED
#define DEE_PRIVATE_THREAD_SLEEP_DEFINED 1
DEE_FUNC_DECL(void) DeeThread_SleepNoInterrupt(DEE_A_IN unsigned int msecs);
#endif

struct DeeAtomicOnceFlag { DEE_TYPES_UINT8_T aof_flag; };
#define DeeAtomicOnceFlag_INIT() {0}
#define DeeAtomicOnceFlag_DID_RUN(atom) (DeeAtomic8_Load((atom)->aof_flag,memory_order_seq_cst)==2)
#define DeeAtomicOnceFlag_RESET(atom) ((atom)->aof_flag=0)
#define DeeAtomicOnceFlag_RUN(atom,...)\
do{\
 if (DeeAtomic8_CompareExchangeStrong((atom)->aof_flag,0,1,\
  memory_order_seq_cst,memory_order_seq_cst)) {\
  __VA_ARGS__;\
  DeeAtomic8_Store((atom)->aof_flag,2,memory_order_seq_cst);\
 } else {\
  while (DeeAtomic8_Load((atom)->aof_flag,memory_order_seq_cst) != 2)\
   DeeThread_SleepNoInterrupt(1);\
 }\
}while(0)
#else
struct DeeAtomicOnceFlag { DEE_TYPES_UINT8_T aof_flag; };
#define DeeAtomicOnceFlag_INIT() 0
#define DEE_ATOMIC_ONCE_EX(atom,...)\
do{\
 if (!(atom)->aof_flag) {\
  __VA_ARGS__;\
  (atom)->aof_flag = 1;\
 }\
}while(0)
#endif

#define DEE_ATOMIC_ONCE(...)\
do{\
 static struct DeeAtomicOnceFlag _aflag = DeeAtomicOnceFlag_INIT();\
 DeeAtomicOnceFlag_RUN(&_aflag,__VA_ARGS__);\
}while(0)

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_ATOMIC_ONCE_H */
