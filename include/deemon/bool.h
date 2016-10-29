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
#ifndef GUARD_DEEMON_BOOL_H
#define GUARD_DEEMON_BOOL_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/type.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeBoolObject);

#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif
DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeBoolObject {
 DEE_OBJECT_HEAD
#ifdef __INTELLISENSE__
  ;
#endif
 Dee_rt_bool b_value;
};
DEE_COMPILER_STRUCTURE_PACKED_END
#define DeeBool_VALUE(ob)  ((Dee_rt_bool)((DeeBoolObject *)Dee_REQUIRES_POINTER(ob))->b_value)
#else
#define DeeBool_VALUE(ob)   DeeBool_Value((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

DEE_DATA_DECL(DeeTypeObject) DeeBool_Type;
#define DeeBool_Check(ob)      DeeObject_InstanceOf(ob,&DeeBool_Type)
#define DeeBool_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeBool_Type)

// NOTE: Be careful to never change b_value of
//       true / false, because you would succeed...
DEE_DATA_DECL(DeeBoolObject) _Dee_True;
DEE_DATA_DECL(DeeBoolObject) _Dee_False;
#define Dee_True   ((DeeObject *)&_Dee_True)
#define Dee_False  ((DeeObject *)&_Dee_False)

//////////////////////////////////////////////////////////////////////////
// Same as calling '(bool)ob' in user code.
// >> This function does the same as DeeObject_Bool, but if 'ob' is
//    a string, the string is parsed for a boolean expression.
// NOTE: Slightly different from 'DeeObject_Bool', which is equivalent to '!!ob'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeBool_FromUserObject(DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeBool_Value(DEE_A_IN_OBJECT(DeeBoolObject) const *ob) DEE_ATTRIBUTE_NONNULL((1));


// We can't just compare against 'Dee_True', because
// we can't guaranty that someone isn't going to create
// copies of true or false with 'DeeObject_Copy'
#define DeeBool_NewTrue()  (_DeeObject_INCREF(Dee_True),Dee_True)
#define DeeBool_NewFalse() (_DeeObject_INCREF(Dee_False),Dee_False)
#define DeeBool_New(b)     ((b)?DeeBool_NewTrue():DeeBool_NewFalse())
#define DeeBool_Const(b)   ((b)?Dee_True:Dee_False)
#define DeeReturn_True     return DeeBool_NewTrue()
#define DeeReturn_False    return DeeBool_NewFalse()
#define DeeReturn_Bool(v)  return DeeBool_New(v)

#define DEE_PRIVATE_OBJECT_NEW_bool   DeeBool_New
#define DEE_PRIVATE_OBJECT_TYPE_bool  &DeeBool_Type
#define DEE_PRIVATE_OBJECT_NEW__Bool  DEE_PRIVATE_OBJECT_NEW_bool
#define DEE_PRIVATE_OBJECT_TYPE__Bool DEE_PRIVATE_OBJECT_TYPE_bool

DEE_DECL_END

#endif /* !GUARD_DEEMON_BOOL_H */
