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
#ifndef GUARD_DEEMON_TYPE_H
#define GUARD_DEEMON_TYPE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/std/stdarg.h>
#ifdef DEE_LIMITED_DEX
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
//#include <deemon/optional/type_flags.h>
//#include <deemon/optional/type_slots.h>
//#include <deemon/optional/type_util.h>
#endif
#if DEE_DEPRECATED_API_VERSION(100,101,103)
#include <deemon/optional/type_slots.h>
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

#define /* int */DeeType_Check(ob)      DeeObject_InstanceOf(ob,&DeeType_Type)
#define /* int */DeeType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeType_Type)

//////////////////////////////////////////////////////////////////////////
// The type for all type objects
DEE_DATA_DECL(DEE_A_DEEMON("type object") DeeTypeObject) DeeType_Type;

//////////////////////////////////////////////////////////////////////////
// The base class of all things
// >> This must implements all operatos as not_implemented
// >> Also known as 'object' during runtime
DEE_DATA_DECL(DEE_A_DEEMON("object") DeeTypeObject) DeeObject_Type;

//////////////////////////////////////////////////////////////////////////
// Returns 1 if 'subclass' is same or derived from 'base'; 0 otherwise
// NOTE: This function return 0 if 'base' is not a type object
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeType_IsSameOrDerived(
 DEE_A_IN DeeTypeObject const *subclass,
 DEE_A_IN DeeTypeObject const *base) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeType_IsDerived(
 DEE_A_IN DeeTypeObject const *subclass,
 DEE_A_IN DeeTypeObject const *base) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns true, if 'ob' is a var object
#define /* int */DeeVarObject_Check(ob)  ((DeeType_FLAGS(Dee_TYPE(ob))&DEE_TYPE_FLAG_VAR_OBJECT)!=0)
#define /* int */DeeObject_IS_VAR        DeeVarObject_Check // Deprecated name

//////////////////////////////////////////////////////////////////////////
// Checks if 'ob' is an instance of 'tp' or any type derived from 'tp'
// >> Equivalent to the deemon expression: 'ob is tp'
#define /* int */DeeObject_InstanceOf(ob,tp)      DeeType_IsSameOrDerived(Dee_TYPE(ob),tp)

//////////////////////////////////////////////////////////////////////////
// Checks if 'ob' is an instance of 'tp', and only 'tp'.
// Unlike 'DeeObject_InstanceOf', this function returns false if the object
// is of a type derived from 'tp', but still returns true if they match exactly
#define /* int */DeeObject_InstanceOfExact(ob,tp) (Dee_TYPE(ob)==(tp))

//////////////////////////////////////////////////////////////////////////
// Alias for 'DeeObject_InstanceOf', to make the connection with the 'is' operator more clear
// - This function returns true, if 'ob' is an lvalue to 'tp'
// - Expected output:
//   >> i = 42;
//   >> p = &i;
//   >> print *p is int;  // true
//   >> print *p is int&; // true
//   >> print i is int;   // true
//   >> print i is int&;  // false
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeObject_Is(
 DEE_A_IN DeeObject const *ob,
 DEE_A_IN DeeTypeObject const *tp) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Returns the object instance associated with 'tp_instance'
// This function is meant to replace type-checks in right-hand-side operator implementation:
// Turn this: >> if (DeeError_TypeError_CheckType(right,&DeeMy_Type) != 0) return NULL;
// Into this: >> if ((result = DeeObject_GetInstance(right,&DeeMy_Type)) == NULL) return NULL;
// This function takes super objects into account.
#define DeeObject_GetInstance(ob,tp_instance) DeeObject_TGetInstance(Dee_TYPE(ob),ob,tp_instance)
#define DeeObject_InplaceGetInstance(ob,tp_instance) \
 ((*(DeeObject **)(ob) = DeeObject_TGetInstance(Dee_TYPE(*(ob)),(DeeObject *)*(ob),tp_instance)) == NULL ? -1 : 0)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) DeeObject *) DeeObject_TGetInstance(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_instance) DEE_ATTRIBUTE_NONNULL((1,2,3));


//////////////////////////////////////////////////////////////////////////
// v allocates and returns a new instance of "self" with "args"
DEE_FUNC_DECL(DEE_A_DEEMON("self(args...)") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstance(
 DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("self()") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstanceDefault(
 DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstancef(DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstanceObjArgs(DEE_A_IN DeeTypeObject const *self, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_VNewInstancef(DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_VNewInstanceObjArgs(DEE_A_IN DeeTypeObject const *self, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Object constructors
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_Construct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_ConstructDef(
 DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_CopyConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_MoveConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) DEE_ATTRIBUTE_NONNULL((1,2));
#elif DEE_DEPRECATED_API_VERSION(100,101,103)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2))
 DEE_ATTRIBUTE_DEPRECATED("DeeObject_Construct() is a limited function. Use 'DeeType_NewInstance()' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_Construct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self));
 return (*DeeType_GET_SLOT(Dee_TYPE(self),tp_any_ctor))(Dee_TYPE(self),self,args);
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("DeeObject_ConstructDef() is a limited function. Use 'DeeType_NewInstanceDefault()' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_ConstructDef(DEE_A_INOUT DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return (*DeeType_GET_SLOT(Dee_TYPE(self),tp_ctor))(Dee_TYPE(self),self);
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2))
 DEE_ATTRIBUTE_DEPRECATED("DeeObject_CopyConstruct() is a limited function. Use 'DeeObject_Copy()' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_CopyConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 DEE_ASSERT(DeeObject_InstanceOf(right,Dee_TYPE(self)));
 return (*DeeType_GET_SLOT(Dee_TYPE(self),tp_copy_ctor))(Dee_TYPE(self),self,right);
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2))
 DEE_ATTRIBUTE_DEPRECATED("DeeObject_CopyConstruct() is a limited function. Use 'DeeObject_Move()' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_MoveConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 DEE_ASSERT(DeeObject_InstanceOf(right,Dee_TYPE(self)));
 return (*DeeType_GET_SLOT(Dee_TYPE(self),tp_move_ctor))(Dee_TYPE(self),self,right);
}
#endif

//////////////////////////////////////////////////////////////////////////
// Sets the name/doc of a type (Only allowed on heap types)
// WARNING: These functions are not thread-safe and should only
//          be called before the type is globally visible.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeType_SetName(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeType_SetDoc(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *doc) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(void) DeeType_SetStaticName(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(void) DeeType_SetStaticDoc(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *doc) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns the type of the member 'member' while writing its offset to '*offset'
// NOTE: Also works for struct types
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_OffsetofAttrString(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN_Z char const *attr, DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_OffsetofAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_OffsetofItemIndex(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_size_t index, DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_OffsetofItem(
 DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT DeeObject *index, DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns 1 for types that must be copied with pushed as constants during runtime
#define /* int */DeeType_MustCopy(tp) ((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_MUST_COPY)!=0)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeType_MustDeepCopy(
 DEE_A_IN DeeTypeObject const *tp) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Only meant to be called from a destructor:
//  - Goes through all the base types, calling the destructors for each base
//  - After initializing your type's base, if your initializer fails,
//    call this to destruct what you had previously constructed.
// NOTE: Only call this function with an exception already set
// NOTE: If this function returns '0', one of the base destructor
//       managed to revive the object, meaning that you can't free
//       it, or tell your caller that an error has occurred.
//       Also note, that the error will have already been
//       handled by this function, too.
// NOTE: If this function returns '-1', it won't actually have
//       set another error, but simply hasn't handled the error
//       that you had set when calling it.
// >> static int _myty_tp_ctor(DeeTypeObject *tp_self, DeeObject *self) {
// >>   // Ensure a consistent state, as operators might call upwards into 'tp_self'
// >>   // NOTE: This is only required if you don't know for certain that
// >>   //       your base won't be trying to use your members.
// >>   set_consistent_state();
// >>
// >>   // Call the base default constructor
// >>   if ((*DeeType_GET_SLOT(DeeType_BASE(tp_self),tp_ctor))(DeeType_BASE(tp_self),self) != 0) return temp;
// >>   
// >>   do_my_initialization();
// >>   
// >>   if (my_initialization_failed()) {
// >>     return _DeeObject_DestructChain(DeeType_BASE(tp_self),self);
// >>   }
// >>   return 0;
// >> }
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) _DeeObject_DestructChain(
 DEE_A_IN DeeTypeObject const *start, DEE_A_INOUT DeeObject *self);

#ifdef DEE_LIMITED_API
extern            DEE_A_RET_NOEXCEPT(0) int _DeeType_HasAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_GetAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_GetConstAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_DelAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_SetAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_CallAttrString(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetAttr(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetAttrString(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
#define /*DEE_A_EXEC*/ DeeType_GetConstAttr(self,attr) DeeType_GetConstAttrString(self,DeeString_STR(attr))
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetConstAttrString(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeType_DelAttr(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeType_DelAttrString(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeType_SetAttr(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *name, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeType_SetAttrString(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /*DEE_A_EXEC*/ DeeType_CallAttr(self,attr,args) DeeType_CallAttrString(self,DeeString_STR(attr),args)
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeType_CallAttrString(DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
#elif defined(DEE_LIMITED_DEX)
#define DeeType_GetAttr  (*DeeType_GET_SLOT(&DeeType_Type,tp_attr_get))
#define DeeType_DelAttr  (*DeeType_GET_SLOT(&DeeType_Type,tp_attr_del))
#define DeeType_SetAttr  (*DeeType_GET_SLOT(&DeeType_Type,tp_attr_set))
#endif

#ifdef DEE_LIMITED_API
extern int DEE_CALL _deetype_tp_marshal_ctor(DeeTypeObject *tp_self, DeeTypeObject *self, DeeObject *infile, struct DeeMarshalReadMap *map);
extern int DEE_CALL _deetype_tp_marshal_put(DeeTypeObject *tp_self, DeeTypeObject *self, DeeObject *outfile, struct DeeMarshalWriteMap *map);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_TYPE_H */
