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
#ifndef GUARD_DEEMON_OBJECT_H
#define GUARD_DEEMON_OBJECT_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>
#include <deemon/optional/object_util.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/object_refcnt.h>

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stddef.h> /* NULL */
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H */

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject *)
 Dee_Type(DEE_A_IN DeeObject const *self);

//////////////////////////////////////////////////////////////////////////
// Returns the implicit type of an object.
// Usually, this is equivalent to 'Dee_TYPE(ob)'.
// But for 'super' and 'lvalue' objects the assumed (aka. simulated) type is returned.
// >> assert ((int &)0).class == int;
// >> assert list().super.class == object;
// >> assert type ((int &)0) == int &;
// >> assert type (list().super) == __builtin_object(0x01A5); // super
#define DeeObject_ClassOf(ob) DeeObject_TClassOf(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject *) DeeObject_TClassOf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Generic swap interface
// NOTE: 'a' and 'b' must have the exact same type
DEE_FUNC_DECL(DEE_A_DEEMON("__builtin_swap(a,b)") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int)
 DeeObject_Swap(DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns >0 if 'self' is an immutable object, that is an object that cannot be modified.
// NOTE: In case 'tp_self' is a sequence (such as tuple), this function will
//       also check that all contained elements are just as immutable.
#define DeeObject_IsImmutable(ob) DeeObject_TIsImmutable(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_TIsImmutable(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *(*DeeObject_New_F(typename T))(Args ...args);
//  - Returns a macro that can be used to create deemon objects from c-instances of 'T'
//  - Throws a compiler-error for unsupported / invalid types
#define DeeObject_New_F(T) DEE_PRIVATE_OBJECT_NEW_F(T)
#define DEE_PRIVATE_OBJECT_NEW_F(T) DEE_PRIVATE_OBJECT_NEW_##T

//////////////////////////////////////////////////////////////////////////
// DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeIntegral_NEW(typename T, Args ...args);
//  - Returns a new deemon object containing data from 'args' as a c-instance of 'T'
//  - Throws a compiler-error for unsupported / invalid types
#define /*DEE_A_EXEC*/ DeeObject_New(T,...) DeeObject_New_F(T)(__VA_ARGS__)


//////////////////////////////////////////////////////////////////////////
// DeeTypeObject *DeeObject_TYPE(typename T);
//  - Returns a pointer to the type of object returned when calling 'DeeObject_New(T,...)'
#define DeeObject_TYPE(T) DEE_PRIVATE_OBJECT_TYPE(T)
#define DEE_PRIVATE_OBJECT_TYPE(T) DEE_PRIVATE_OBJECT_TYPE_##T

DEE_DECL_END

#include <deemon/optional/object_attr.h>
#include <deemon/optional/object_call.h>
#include <deemon/optional/object_cast.h>
#include <deemon/optional/object_compare.h>
#include <deemon/optional/object_copy.h>
#include <deemon/optional/object_hash.h>
#include <deemon/optional/object_math.h>
#include <deemon/optional/object_sequence.h>
#include <deemon/optional/object_str.h>

#ifdef GUARD_DEEMON_STRUCTURED_H
#ifdef GUARD_DEEMON_FLOAT_H
#include <deemon/optional/float_decl.h>
#endif
#ifdef GUARD_DEEMON_INTEGER_H
#ifndef GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H
#include <deemon/optional/integer_decl.h>
#endif
#endif
#endif
#ifdef GUARD_DEEMON_CFUNCTION_H
#ifndef GUARD_DEEMON_OPTIONAL_CFUNCTION_DECL_H
#include <deemon/optional/cfunction_decl.h>
#endif
#endif

#endif /* !GUARD_DEEMON_OBJECT_H */
