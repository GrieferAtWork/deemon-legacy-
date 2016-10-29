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
#ifndef GUARD_DEEMON_OPTIONAL_FLOAT_DECL_H
#define GUARD_DEEMON_OPTIONAL_FLOAT_DECL_H 1

#include <deemon/__conf.inl>
#ifndef GUARD_DEEMON_OBJECT_H
#include <deemon/object.h>
#endif
#ifndef GUARD_DEEMON_FLOAT_H
#include <deemon/float.h>
#endif
#ifndef GUARD_DEEMON_STRUCTURED_H
#include <deemon/structured.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif

DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeFloatObject { DEE_STRUCTURED_OBJECT_HEAD Dee_rt_float ob_val; };
struct DeeDoubleObject { DEE_STRUCTURED_OBJECT_HEAD Dee_rt_double ob_val; };
struct DeeLDoubleObject { DEE_STRUCTURED_OBJECT_HEAD Dee_rt_ldouble ob_val; };
DEE_COMPILER_STRUCTURE_PACKED_END

#define DeeFloat_VALUE(ob)   ((DeeFloatObject *)Dee_REQUIRES_POINTER(ob))->ob_val
#define DeeDouble_VALUE(ob)  ((DeeDoubleObject *)Dee_REQUIRES_POINTER(ob))->ob_val
#define DeeLDouble_VALUE(ob) ((DeeLDoubleObject *)Dee_REQUIRES_POINTER(ob))->ob_val

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FLOAT_DECL_H */
