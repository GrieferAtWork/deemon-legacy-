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
#ifndef GUARD_DEEMON_OPTIONAL_RAISED_EXCEPTION_H
#define GUARD_DEEMON_OPTIONAL_RAISED_EXCEPTION_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_DEX
DEE_DECL_BEGIN

struct DeeObject;
struct DeeRaisedException;
struct DeeTracebackObject;

struct DeeRaisedException {
 struct DeeRaisedException           *re_prev;      /*< [0..1][owned] Previous exception(s). */
 DEE_A_REF struct DeeObject          *re_error;     /*< [1..1] Error that was raised. */
 DEE_A_REF struct DeeTracebackObject *re_traceback; /*< [1..1] Traceback of the error. */
};

#ifdef DEE_LIMITED_API
#define _DeeRaisedException_Quit(ob)\
do{\
 Dee_DECREF((ob)->re_error);\
 Dee_DECREF((ob)->re_traceback);\
}while(0)
#define _DeeRaisedException_Visit(ob)\
do{\
 Dee_VISIT((ob)->re_error);\
 Dee_VISIT((ob)->re_traceback);\
}while(0)
#define _DeeRaisedException_InitCopy(ob,right)\
do{\
 Dee_INCREF((ob)->re_error = (right)->re_error);\
 Dee_INCREF((ob)->re_traceback = (right)->re_traceback);\
}while(0)
// Implemented in <src/deemon/error.c>
extern DEE_A_RET_EXCEPT(NULL) struct DeeRaisedException *_DeeRaisedException_Alloc(void);
extern DEE_A_RET_EXCEPT(NULL) struct DeeRaisedException *_DeeRaisedException_CopyChain(DEE_A_IN struct DeeRaisedException *chain);
extern void _DeeRaisedException_Free(DEE_A_IN DEE_A_OUT_INVALID struct DeeRaisedException *e);
extern void _DeeRaisedException_DeleteChain(DEE_A_IN DEE_A_OUT_INVALID struct DeeRaisedException *e);
#endif /* DEE_LIMITED_API */

DEE_DECL_END
#endif /* DEE_LIMITED_DEX */

#endif /* !GUARD_DEEMON_OPTIONAL_RAISED_EXCEPTION_H */
