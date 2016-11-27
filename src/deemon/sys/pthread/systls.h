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
#ifndef GUARD_DEEMON_SYS_PTHREAD_SYSTLS_H
#define GUARD_DEEMON_SYS_PTHREAD_SYSTLS_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if defined(__INTELLISENSE__)
typedef int pthread_key_t;
static void *pthread_getspecific(pthread_key_t key);
static int pthread_setspecific(pthread_key_t key, const void *value);
static int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
static int pthread_key_delete(pthread_key_t key);
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include <pthread.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

struct DeePThreadSysTLS { pthread_key_t st_key; };
#define DeePThreadSysTLS_TryInit(ob) (pthread_key_create(&(ob)->st_key,NULL)==0)
#define DeePThreadSysTLS_Init(ob,...) \
do{\
 int error = pthread_key_create(&(ob)->st_key,NULL);\
 if DEE_UNLIKELY(error != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "pthread_key_create() : %K",\
                      DeeSystemError_ToString(error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeePThreadSysTLS_Quit(ob)                (void)pthread_key_delete((ob)->st_key)
#define DeePThreadSysTLS_TryGetNofail(ob,result) (void)(*(void **)&(result)=pthread_getspecific((ob)->st_key))
#define DeePThreadSysTLS_TryGet(ob,result)       (*(void **)&(result)=pthread_getspecific((ob)->st_key),1)
#define DeePThreadSysTLS_TrySet(ob,value)        (pthread_setspecific((ob)->st_key,(void *)(value))==0)
#define DeePThreadSysTLS_Get(ob,result,...)      do{ DeePThreadSysTLS_TryGetNofail(ob,result); }while(0)
#define DeePThreadSysTLS_Set(ob,value,...) \
do{\
 int error = pthread_setspecific((ob)->st_key,(void *)(value));\
 if DEE_UNLIKELY(error != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "pthread_setspecific(%.*q,%p) : %K",\
                      (unsigned)sizeof(pthread_key_t),&(ob)->st_key,\
                      value,DeeSystemError_ToString(error));\
  {__VA_ARGS__;}\
 }\
}while(0)


#define DeeSysTLS              DeePThreadSysTLS
#define DeeSysTLS_TryInit      DeePThreadSysTLS_TryInit
#define DeeSysTLS_Init         DeePThreadSysTLS_Init
#define DeeSysTLS_Quit         DeePThreadSysTLS_Quit
#define DeeSysTLS_TryGetNofail DeePThreadSysTLS_TryGetNofail
#define DeeSysTLS_TryGet       DeePThreadSysTLS_TryGet
#define DeeSysTLS_TrySet       DeePThreadSysTLS_TrySet
#define DeeSysTLS_Get          DeePThreadSysTLS_Get
#define DeeSysTLS_Set          DeePThreadSysTLS_Set

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS_PTHREAD_SYSTLS_H */
