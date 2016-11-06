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
#ifndef GUARD_DEEMON_SYS__NOTHREAD_SYSTLS_H
#define GUARD_DEEMON_SYS__NOTHREAD_SYSTLS_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// === NoThread ===
DEE_DECL_BEGIN

struct DeeNoThreadSysTLS { void *ns_value; };
#define DeeNoThreadSysTLS_TryInit(ob) ((ob)->ns_value = NULL,1)
#define DeeNoThreadSysTLS_Init(ob,...) do{ (ob)->ns_value = NULL; }while(0)
#define DeeNoThreadSysTLS_Quit(ob)                (void)(ob)
#define DeeNoThreadSysTLS_TryGetNofail(ob,result) (void)(*(void **)&(result)=(ob)->ns_value)
#define DeeNoThreadSysTLS_TryGet(ob,result)       (*(void **)&(result)=(ob)->ns_value,1)
#define DeeNoThreadSysTLS_TrySet(ob,value)        ((ob)->ns_value=(void *)(value),1)
#define DeeNoThreadSysTLS_Get(ob,result,...)      do{ *(void **)&(result) = (ob)->ns_value; }while(0)
#define DeeNoThreadSysTLS_Set(ob,value,...)       do{ (ob)->ns_value = (void *)(value); }while(0)

#define DeeSysTLS              DeeNoThreadSysTLS
#define DeeSysTLS_TryInit      DeeNoThreadSysTLS_TryInit
#define DeeSysTLS_Init         DeeNoThreadSysTLS_Init
#define DeeSysTLS_Quit         DeeNoThreadSysTLS_Quit
#define DeeSysTLS_TryGetNofail DeeNoThreadSysTLS_TryGetNofail
#define DeeSysTLS_TryGet       DeeNoThreadSysTLS_TryGet
#define DeeSysTLS_TrySet       DeeNoThreadSysTLS_TrySet
#define DeeSysTLS_Get          DeeNoThreadSysTLS_Get
#define DeeSysTLS_Set          DeeNoThreadSysTLS_Set

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__NOTHREAD_SYSTLS_H */
