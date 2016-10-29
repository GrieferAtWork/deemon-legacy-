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
#ifndef GUARD_DEEMON_MEMBERFUNCTION_H
#define GUARD_DEEMON_MEMBERFUNCTION_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
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
#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#endif
#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif

DEE_OBJECT_DEF(DeeMemberFunctionObject);

//////////////////////////////////////////////////////////////////////////
// CFunction wrapper object
#ifdef DEE_LIMITED_DEX
struct DeeStructuredTypeObject;
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeMemberFunctionObject {
 DEE_OBJECT_HEAD
#define DEE_MEMBERFUNCTION_FLAG_NONE            DEE_UINT32_C(0x00000000)
#define DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL DEE_UINT32_C(0x00000001) /*< Structured call (self isn't a DeeObject, but rather the object data). */
#define DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB   DEE_UINT32_C(0x00000002) /*< The member name is an object. */
 Dee_uint32_t           mf_flags;   /*< Function call flags. */
 union{
  struct{
   DEE_A_REF DeeObject *mfo_self; /*< [1..1] Self argument. */
   DeeMemberFunction    mfo_func; /*< [1..1] C function to call. */
  } mf_object;
  struct {
   DEE_A_REF struct DeeStructuredTypeObject *mfs_type; /*< [1..1] Self argument type. */
   DeeStructuredMemberFunction               mfs_func; /*< [1..1] C function to call. */
   void                                     *mfs_self; /*< [1..1] Self pointer. */
  } mf_struct;
 };
 void                             *mf_closure;  /*< [?..?] Closure (last argument to c function). */
 union{
 DEE_A_REF struct DeeStringObject *mf_member_o; /*< [1..1] Member name. */
 char const                       *mf_member_s; /*< [1..1] Member name. */
 };
};
DEE_COMPILER_MSVC_WARNING_POP
#endif /* !DEE_LIMITED_API */

#define DeeMemberFunction_Check(ob)      DeeObject_InstanceOf(ob,&DeeMemberFunction_Type)
#define DeeMemberFunction_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeMemberFunction_Type)
DEE_DATA_DECL(DeeTypeObject) DeeMemberFunction_Type;

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *) DeeMemberFunction_NewObject(
 DEE_A_INOUT DeeObject *self_arg, DEE_A_IN DeeMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *) DeeMemberFunction_NewStruct(
 DEE_A_IN DeeTypeObject *self_type, DEE_A_IN void *self, DEE_A_IN DeeStructuredMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *) DeeMemberFunction_NewObjectString(
 DEE_A_INOUT DeeObject *self_arg, DEE_A_IN DeeMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *) DeeMemberFunction_NewStructString(
 DEE_A_IN DeeTypeObject *self_type, DEE_A_IN void *self, DEE_A_IN DeeStructuredMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2,3,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED char const *) DeeMemberFunction_MemberName(
 DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeMemberFunction_MemberNameObject(
 DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeMemberFunction_Call(
 DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(struct DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern Dee_size_t DeeMemberFunction_ClearPool(void);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_MEMBERFUNCTION_H */
