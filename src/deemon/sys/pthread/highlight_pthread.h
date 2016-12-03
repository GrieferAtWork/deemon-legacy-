/* Copyright(c) 2016 - deemon by Griefer@Work                                    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files(the "Software"), to deal  *
 * in the Software without ion, including without limitation the rights   *
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
#ifndef GUARD_DEEMON_SYS_PTHREAD_HIGHLIGHT_PTHREAD_H
#define GUARD_DEEMON_SYS_PTHREAD_HIGHLIGHT_PTHREAD_H 1

#ifdef __INTELLISENSE__
#include <deemon/__conf.inl>

DEE_DECL_BEGIN

// NOTE: Definitions in this file are merely meant for syntax highlighting in IDEs
//    >> prototypes are taken from an i386 Kubuntu 16.04 linux PC ("/usr/include/pthread.h")

typedef int cpu_set_t;
typedef int clockid_t;
typedef struct { int __placeholder; } pthread_attr_t;
typedef struct { int __placeholder; } pthread_t;
typedef struct { int __placeholder; } pthread_once_t;
typedef struct { int __placeholder; } pthread_unwind_buf_t;
typedef struct { int __placeholder; } pthread_mutex_t;
typedef struct { int __placeholder; } pthread_mutexattr_t;
typedef struct { int __placeholder; } pthread_rwlock_t;
typedef struct { int __placeholder; } pthread_rwlockattr_t;
typedef struct { int __placeholder; } pthread_cond_t;
typedef struct { int __placeholder; } pthread_condattr_t;
typedef struct { int __placeholder; } pthread_spinlock_t;
typedef struct { int __placeholder; } pthread_barrier_t;
typedef struct { int __placeholder; } pthread_barrierattr_t;
typedef struct { int __placeholder; } pthread_key_t;

extern int pthread_create(pthread_t *newthread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
extern DEE_ATTRIBUTE_NORETURN void pthread_exit(void *retval);
extern int pthread_join(pthread_t th, void **thread_return);
extern int pthread_tryjoin_np(pthread_t th, void **thread_return);
extern int pthread_timedjoin_np(pthread_t th, void **thread_return, const struct timespec *abstime);
extern int pthread_detach(pthread_t th);
extern DEE_ATTRIBUTE_CONST pthread_t pthread_self(void);
extern DEE_ATTRIBUTE_CONST int pthread_equal(pthread_t thread1, pthread_t thread2);
extern int pthread_attr_init(pthread_attr_t *attr);
extern int pthread_attr_destroy(pthread_attr_t *attr);
extern int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
extern int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
extern int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
extern int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
extern int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
extern int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
extern int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
extern int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
extern int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inherit);
extern int pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit);
extern int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
extern int pthread_attr_setscope(pthread_attr_t *attr, int scope);
extern int pthread_attr_getstackaddr(const pthread_attr_t * attr, void **stackaddr);
extern int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
extern int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
extern int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
extern int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
extern int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
extern int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset);
extern int pthread_attr_getaffinity_np(const pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset);
extern int pthread_getattr_default_np(pthread_attr_t *attr);
extern int pthread_setattr_default_np(const pthread_attr_t *attr);
extern int pthread_getattr_np(pthread_t th, pthread_attr_t *attr);
extern int pthread_setschedparam(pthread_t target_thread, int policy, const struct sched_param *param);
extern int pthread_getschedparam(pthread_t target_thread, int *policy, struct sched_param *param);
extern int pthread_setschedprio(pthread_t target_thread, int prio);
extern int pthread_getname_np(pthread_t target_thread, char *buf, size_t buflen);
extern int pthread_setname_np(pthread_t target_thread, const char *name);
extern int pthread_getconcurrency(void);
extern int pthread_setconcurrency(int level);
extern int pthread_yield(void);
extern int pthread_setaffinity_np(pthread_t th, size_t cpusetsize, const cpu_set_t *cpuset);
extern int pthread_getaffinity_np(pthread_t th, size_t cpusetsize, cpu_set_t *cpuset);
extern int pthread_once(pthread_once_t *once_control, void(*init_routine)(void));
extern int pthread_setcancelstate(int state, int *oldstate);
extern int pthread_setcanceltype(int type, int *oldtype);
extern int pthread_cancel(pthread_t th);
extern void pthread_testcancel(void);
struct pthread_cleanup_frame { int __placeholder; };
#define pthread_cleanup_push(routine,arg) do{
extern void pthread_register_cancel(pthread_unwind_buf_t *buf);
#define pthread_cleanup_pop(execute)    do{}while(0);}while(0)
extern void pthread_unregister_cancel(pthread_unwind_buf_t *buf);
#define pthread_cleanup_push_defer_np(routine,arg) do{
extern void pthread_register_cancel_defer(pthread_unwind_buf_t *buf);
#define pthread_cleanup_pop_restore_np(execute) do{}while(0);}while(0)
extern void pthread_unregister_cancel_restore(pthread_unwind_buf_t *buf);
extern DEE_ATTRIBUTE_NORETURN void pthread_unwind_next(pthread_unwind_buf_t *buf);

extern int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
extern int pthread_mutex_destroy(pthread_mutex_t *mutex);
extern int pthread_mutex_trylock(pthread_mutex_t *mutex);
extern int pthread_mutex_lock(pthread_mutex_t *mutex);
extern int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abstime);
extern int pthread_mutex_unlock(pthread_mutex_t *mutex);
extern int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling);
extern int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling);
extern int pthread_mutex_consistent(pthread_mutex_t *mutex);
extern int pthread_mutex_consistent_np(pthread_mutex_t *mutex);
extern int pthread_mutexattr_init(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared);
extern int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
extern int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *kind);
extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
extern int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol);
extern int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
extern int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling);
extern int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
extern int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robustness);
extern int pthread_mutexattr_getrobust_np(const pthread_mutexattr_t *attr, int *robustness);
extern int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robustness);
extern int pthread_mutexattr_setrobust_np(pthread_mutexattr_t *attr, int robustness);

extern int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
extern int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock, const struct timespec *abstime);
extern int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock, const struct timespec *abstime);
extern int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
extern int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
extern int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
extern int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared);
extern int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
extern int pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *attr, int *pref);
extern int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr, int pref);

extern int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *cond_attr);
extern int pthread_cond_destroy(pthread_cond_t *cond);
extern int pthread_cond_signal(pthread_cond_t *cond);
extern int pthread_cond_broadcast(pthread_cond_t *cond);
extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
extern int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
extern int pthread_condattr_init(pthread_condattr_t *attr);
extern int pthread_condattr_destroy(pthread_condattr_t *attr);
extern int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);
extern int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
extern int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock_id);
extern int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);

extern int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
extern int pthread_spin_destroy(pthread_spinlock_t *lock);
extern int pthread_spin_lock(pthread_spinlock_t *lock);
extern int pthread_spin_trylock(pthread_spinlock_t *lock);
extern int pthread_spin_unlock(pthread_spinlock_t *lock);

extern int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count);
extern int pthread_barrier_destroy(pthread_barrier_t *barrier);
extern int pthread_barrier_wait(pthread_barrier_t *barrier);
extern int pthread_barrierattr_init(pthread_barrierattr_t *attr);
extern int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
extern int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared);
extern int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);

extern int pthread_key_create(pthread_key_t *key, void(*destr_function)(void *));
extern int pthread_key_delete(pthread_key_t key);
extern void *pthread_getspecific(pthread_key_t key);
extern int pthread_setspecific(pthread_key_t key, const void *pointer);

extern int pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id);
extern int pthread_atfork(void(*prepare)(void), void(*parent)(void), void(*child)(void));

DEE_DECL_END
#endif

#endif /* !GUARD_DEEMON_SYS_PTHREAD_HIGHLIGHT_PTHREAD_H */
