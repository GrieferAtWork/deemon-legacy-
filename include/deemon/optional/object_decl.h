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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_DECL_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_DECL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_util.h>

DEE_DECL_BEGIN

struct DeeObject;
struct DeeTypeObject;

#define DEE_OBJECT_HEAD_INIT(ob_type)                     1,1,ob_type
#define DEE_OBJECT_HEAD_INIT_REF(ob_type,ref)             ref,1,ob_type
#define DEE_VAR_OBJECT_HEAD_INIT(ob_type,ob_size)         1,1,ob_type,ob_size
#define DEE_VAR_OBJECT_HEAD_INIT_REF(ob_type,ref,ob_size) ref,1,ob_type,ob_size

#define DEE_OBJECT_HEAD_EX(tyT) \
 unsigned int __ob_refcnt;\
 unsigned int __ob_weakcnt;\
 /*DEE_A_REF*/tyT *__ob_type;
#define DEE_VAR_OBJECT_HEAD_EX(tyT) \
 DEE_OBJECT_HEAD_EX(tyT)\
 Dee_size_t __ob_size; /*< [const] Type-defined size value. */

#define DEE_OBJECT_HEAD     DEE_OBJECT_HEAD_EX(DeeTypeObject)
#define DEE_VAR_OBJECT_HEAD DEE_VAR_OBJECT_HEAD_EX(DeeTypeObject)

//////////////////////////////////////////////////////////////////////////
// Generic object structures
struct DeeObject {
 unsigned int          __ob_refcnt;
 unsigned int          __ob_weakcnt;
 struct DeeTypeObject *__ob_type;
 // more data...
};

//////////////////////////////////////////////////////////////////////////
// Makes sure "self" is really a living Deemon Object
// NOTE: Only meant for assertions, cannot and should not
//       be used to check if any random pointer you may
//       have found is actually a deemon object...
#if defined(__INTELLISENSE__)
#define DeeObject_Check(ob) \
 __noop(Dee_REQUIRES_POINTER(ob))
#else
#define DeeObject_Check(ob) \
                    ((ob)&&\
((struct DeeObject *)(ob))->__ob_refcnt&&\
((struct DeeObject *)(ob))->__ob_weakcnt&&\
((struct DeeObject *)(ob))->__ob_type)
#endif

#define Dee_TYPE(ob) ((struct DeeObject*)Dee_REQUIRES_POINTER(ob))->__ob_type

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_DECL_H */
