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
#ifndef GUARD_DEEMON_SYS_STUB_SYSTLS_H
#define GUARD_DEEMON_SYS_STUB_SYSTLS_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

//////////////////////////////////////////////////////////////////////////
// === NOT-IMPLEMENTED ===
DEE_DECL_BEGIN

struct DeeStubSysTLS { int ss_unused; };
#define DeeStubSysTLS_TryInit(ob)             1
#define DeeStubSysTLS_Init(ob,...)            do{}while(0)
#define DeeStubSysTLS_Quit(ob)                (void)(ob)
#define DeeStubSysTLS_TryGetNofail(ob,result) (void)(*(void **)&(result)=NULL)
#define DeeStubSysTLS_TryGet(ob,result)       0
#define DeeStubSysTLS_TrySet(ob,value)        0
#define DeeStubSysTLS_Get(ob,result,...)      do{ DeeError_NotImplemented_str("systls"); {__VA_ARGS__;} }while(0)
#define DeeStubSysTLS_Set(ob,value,...)       do{ DeeError_NotImplemented_str("systls"); {__VA_ARGS__;} }while(0)

#define DeeSysTLS              DeeStubSysTLS
#define DeeSysTLS_TryInit      DeeStubSysTLS_TryInit
#define DeeSysTLS_Init         DeeStubSysTLS_Init
#define DeeSysTLS_Quit         DeeStubSysTLS_Quit
#define DeeSysTLS_TryGetNofail DeeStubSysTLS_TryGetNofail
#define DeeSysTLS_TryGet       DeeStubSysTLS_TryGet
#define DeeSysTLS_TrySet       DeeStubSysTLS_TrySet
#define DeeSysTLS_Get          DeeStubSysTLS_Get
#define DeeSysTLS_Set          DeeStubSysTLS_Set

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_STUB_SYSTLS_H */
