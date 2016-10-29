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
#ifndef GUARD_DEEMON_OPTIONAL_CFUNCTION_DECL_H
#define GUARD_DEEMON_OPTIONAL_CFUNCTION_DECL_H 1

#include <deemon/__conf.inl>
#ifndef GUARD_DEEMON_OBJECT_H
#include <deemon/object.h>
#endif
#ifndef GUARD_DEEMON_CFUNCTION_H
#include <deemon/cfunction.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_CFUNCTION
DEE_PRIVATE_DECL_DEE_CFUNCTION
#undef DEE_PRIVATE_DECL_DEE_CFUNCTION
#endif

//////////////////////////////////////////////////////////////////////////
// Simple CFunction wrapper object
struct DeeCFunctionObject {
 DEE_OBJECT_HEAD
 char const  *cf_name; /*< [0..1] Function name. */
 char const  *cf_doc;  /*< [0..1] Function doc. */
 DeeCFunction cf_func; /*< [1..1] C function to call. */
};
#define DeeCFunction_INIT(name,doc,func) {DEE_OBJECT_HEAD_INIT(&DeeCFunction_Type),name,doc,func}
#define DeeCFunction_FUNC(ob)            ((DeeCFunctionObject *)Dee_REQUIRES_POINTER(ob))->cf_func
#define DeeCFunction_NAME(ob)            ((DeeCFunctionObject *)Dee_REQUIRES_POINTER(ob))->cf_name
#define DeeCFunction_DOC(ob)             ((DeeCFunctionObject *)Dee_REQUIRES_POINTER(ob))->cf_doc
#define DeeCFunction_CALL(ob,args)       (*DeeCFunction_FUNC(ob))(args)
#define DeeCFunction_Check(ob)           DeeObject_InstanceOf(ob,&DeeCFunction_Type)
#define DeeCFunction_CheckExact(ob)      DeeObject_InstanceOfExact(ob,&DeeCFunction_Type)

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_CFUNCTION_DECL_H */
