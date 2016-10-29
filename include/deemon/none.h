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
#ifndef GUARD_DEEMON_NONE_H
#define GUARD_DEEMON_NONE_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// None
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_DATA_DECL(DeeStructuredTypeObject) DeeNone_Type;
DEE_DATA_DECL(DeeStructuredObject)     _Dee_None;

//////////////////////////////////////////////////////////////////////////
// NOTE: None object allow all operators and most of the time return none again!
//       >> a = none;
//       >> (a.foo[10].bar*10).foobar; // Operators always return none
//       An exception to this rule are the compare operators:
//       >> none == none; // Returns 'true'
//       >> none != none; // Returns 'false'
#define Dee_None          ((DeeObject *)&_Dee_None)
#define DeeNone_New()     (_DeeObject_INCREF(Dee_None),Dee_None)
#define DeeReturn_None    return DeeNone_New()
#if 1
// Since we override tp_alloc and none cannot be sub-classed, it's a true singleton!
// Meaning we can simply check for none by comparing against 'Dee_None'
#define DeeNone_XCheck(ob) ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_None)
#define DeeNone_Check(ob)  ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_None)
#else
#define DeeNone_XCheck(ob) ((ob)&&DeeObject_InstanceOfExact(ob,&DeeNone_Type))
#define DeeNone_Check(ob)  DeeObject_InstanceOfExact(ob,&DeeNone_Type)
#endif

#define DEE_PRIVATE_OBJECT_TYPE_void      ((DeeTypeObject *)&DeeNone_Type)
#define DEE_PRIVATE_OBJECT_TYPE_none      ((DeeTypeObject *)&DeeNone_Type)
#define DEE_PRIVATE_OBJECT_TYPE_nullptr_t ((DeeTypeObject *)&DeeNone_Type)
#define DEE_PRIVATE_OBJECT_NEW_void       DeeNone_New
#define DEE_PRIVATE_OBJECT_NEW_none       DeeNone_New
#define DEE_PRIVATE_OBJECT_NEW_nullptr_t  DeeNone_New

DEE_DECL_END

#endif /* !GUARD_DEEMON_NONE_H */
