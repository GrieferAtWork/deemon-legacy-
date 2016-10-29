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
#ifndef GUARD_DEEMON_OPTIONAL_STRUCTURED_DECL_H
#define GUARD_DEEMON_OPTIONAL_STRUCTURED_DECL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

struct DeeStructuredObject;
struct DeeStructuredTypeObject;

#define DEE_STRUCTURED_OBJECT_HEAD_INIT(type) \
 DEE_OBJECT_HEAD_INIT((struct DeeStructuredTypeObject *)(type))
#define DEE_STRUCTURED_OBJECT_HEAD \
 DEE_OBJECT_HEAD_EX(struct DeeStructuredTypeObject)
struct DeeStructuredObject { DEE_STRUCTURED_OBJECT_HEAD };
#define DeeStructured_DATA(ob) ((void *)((DeeObject *)(ob)+1))
#define DeeStructured_TYPE(ob) ((struct DeeStructuredTypeObject *)Dee_TYPE(ob))
#define DeeStructured_SIZE(ob) DeeType_GET_SLOT(Dee_TYPE(ob),tp_p_instance_size)
#define DeeStructured_ADDR     DeeStructured_DATA

DEE_DECL_END

#endif /* !GUARD_DEEMON_STRUCTURED_H */
