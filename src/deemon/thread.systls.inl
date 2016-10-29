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
#ifndef GUARD_DEEMON_THREAD_SYSTLS_INL
#define GUARD_DEEMON_THREAD_SYSTLS_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#ifndef DEE_WITHOUT_THREADS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include <pthread.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
struct DeeSYSTls { DWORD tls_id; };
#define DeeSYSTls_Init(tls) \
 ((((tls)->tls_id = TlsAlloc()) == TLS_OUT_OF_INDEXES)?-1:0)
#define DeeSYSTls_Quit(tls)  (TlsFree((tls)->tls_id)?(void)0:SetLastError(0))
#define DeeSYSTls_Get(tls)    TlsGetValue((tls)->tls_id)
#define DeeSYSTls_Set(tls,v) (TlsSetValue((tls)->tls_id,v)?0:-1)
#else
struct DeeSYSTls { pthread_key_t tls_id; };
#define DeeSYSTls_Init(tls)   pthread_key_create(&(tls)->tls_id,0)
#define DeeSYSTls_Quit(tls)   \
 (pthread_key_delete((tls)->tls_id)!=0?(void)(errno=0):(void)0)
#define DeeSYSTls_Get(tls)    pthread_getspecific((tls)->tls_id)
#define DeeSYSTls_Set(tls,v)  pthread_setspecific((tls)->tls_id,v)
#endif

DEE_DECL_END
#else /* !DEE_WITHOUT_THREADS */
DEE_DECL_BEGIN

struct DeeSYSTls { void *tls_val; };
#define DeeSYSTls_Init(tls)  ((tls)->tls_val = NULL,0)
#define DeeSYSTls_Quit(tls)  (void)0
#define DeeSYSTls_Get(tls)    (tls)->tls_val
#define DeeSYSTls_Set(tls,v) ((tls)->tls_val=(v),0)

DEE_DECL_END
#endif /* DEE_WITHOUT_THREADS */

#endif /* !GUARD_DEEMON_THREAD_SYSTLS_INL */
