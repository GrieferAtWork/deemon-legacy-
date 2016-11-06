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
#ifndef GUARD_DEEMON_SYS_SYSTLS_H
#define GUARD_DEEMON_SYS_SYSTLS_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// >> struct DeeSysTLS { ... };
// >> bool DeeSysTLS_TryInit(struct DeeSysTLS *self);
// >> void DeeSysTLS_Init(struct DeeSysTLS *self, CODE on_error);
// >> void DeeSysTLS_Quit(struct DeeSysTLS *self);
// >> void DeeSysTLS_TryGetNofail(struct DeeSysTLS *self, void *&result);
// >> bool DeeSysTLS_TryGet(struct DeeSysTLS *self, void *&result);
// >> bool DeeSysTLS_TrySet(struct DeeSysTLS *self, void *value);
// >> void DeeSysTLS_Get(struct DeeSysTLS *self, void *&result, CODE on_error);
// >> void DeeSysTLS_Set(struct DeeSysTLS *self, void *value, CODE on_error);

#if defined(DEE_WITHOUT_THREADS)
# include <deemon/sys/_nothread.systls.h>
#elif defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.systls.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
# include <deemon/sys/_pthread.systls.h>
#elif 1
# include <deemon/sys/_emulated.systls.h>
#elif 1
# include <deemon/sys/_stub.systls.h>
#else
# error "No TLS implementation for this platform"
#endif

#ifndef DeeSysTLS_TryGetNofail
#define DeeSysTLS_TryGetNofail(ob,result) \
 (DeeSysTLS_TryGet(ob,result)?(void)0:(void)((result)=NULL))
#endif


#endif /* !GUARD_DEEMON_SYS_SYSTLS_H */
