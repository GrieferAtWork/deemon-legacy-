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
#ifndef GUARD_DEEMON_MEMBERFUNCTION_C
#define GUARD_DEEMON_MEMBERFUNCTION_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/std/string.h>
#include <deemon/object.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN
DEE_OBJECTPOOL_DEFINE(memberfunction,DeeMemberFunctionObject)

//////////////////////////////////////////////////////////////////////////
// CFunction wrapper object

Dee_size_t DeeMemberFunction_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(memberfunction);
}

#define _self ((DeeMemberFunctionObject *)self)
DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *DeeMemberFunction_NewObject(
 DEE_A_INOUT DeeObject *self_arg, DEE_A_IN DeeMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DeeMemberFunctionObject *result;
 DEE_ASSERT(DeeObject_Check(self_arg));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 if ((result = DEE_OBJECTPOOL_ALLOC(memberfunction)) != NULL) {
  DeeObject_INIT(result,&DeeMemberFunction_Type);
  result->mf_flags = DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB;
  Dee_INCREF(result->mf_object.mfo_self = self_arg);
  result->mf_object.mfo_func = func;
  result->mf_closure = closure;
  Dee_INCREF(result->mf_member_o = (DeeStringObject *)name);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *DeeMemberFunction_NewStruct(
 DEE_A_IN DeeTypeObject *self_type, DEE_A_IN void *self, DEE_A_IN DeeStructuredMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DeeMemberFunctionObject *result;
 DEE_ASSERT(DeeObject_Check(self_type) && DeeStructuredType_Check(self_type));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(self);
 if ((result = DEE_OBJECTPOOL_ALLOC(memberfunction)) != NULL) {
  DeeObject_INIT(result,&DeeMemberFunction_Type);
  result->mf_flags = DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB|DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL;
  Dee_INCREF(result->mf_struct.mfs_type = (DeeStructuredTypeObject *)self_type);
  result->mf_struct.mfs_self = self;
  result->mf_struct.mfs_func = func;
  result->mf_closure = closure;
  Dee_INCREF(result->mf_member_o = (DeeStringObject *)name);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *DeeMemberFunction_NewObjectString(
 DEE_A_INOUT DeeObject *self_arg, DEE_A_IN DeeMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_Z char const *name) {
 DeeMemberFunctionObject *result;
 DEE_ASSERT(name);
 DEE_ASSERT(DeeObject_Check(self_arg));
 if ((result = DEE_OBJECTPOOL_ALLOC(memberfunction)) != NULL) {
  DeeObject_INIT(result,&DeeMemberFunction_Type);
  result->mf_flags = 0;
  Dee_INCREF(result->mf_object.mfo_self = self_arg);
  result->mf_object.mfo_func = func;
  result->mf_closure = closure;
  result->mf_member_s = name;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeMemberFunctionObject) *DeeMemberFunction_NewStructString(
 DEE_A_IN DeeTypeObject *self_type, DEE_A_IN void *self, DEE_A_IN DeeStructuredMemberFunction func,
 DEE_A_IN_OPT void *closure, DEE_A_IN_Z char const *name) {
 DeeMemberFunctionObject *result;
 DEE_ASSERT(DeeObject_Check(self_type) && DeeStructuredType_Check(self_type));
 DEE_ASSERT(name);
 DEE_ASSERT(self);
 if ((result = DEE_OBJECTPOOL_ALLOC(memberfunction)) != NULL) {
  DeeObject_INIT(result,&DeeMemberFunction_Type);
  result->mf_flags = DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL;
  Dee_INCREF(result->mf_struct.mfs_type = (DeeStructuredTypeObject *)self_type);
  result->mf_struct.mfs_self = self;
  result->mf_struct.mfs_func = func;
  result->mf_closure = closure;
  result->mf_member_s = name;
 }
 return (DeeObject *)result;
}


DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED char const *
DeeMemberFunction_MemberName(DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self) {
 if ((_self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB) != 0) {
  return DeeString_STR(_self->mf_member_o);
 } else {
  return _self->mf_member_s;
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeMemberFunction_MemberNameObject(DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self) {
 if ((_self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB) != 0) {
  DeeReturn_NEWREF(_self->mf_member_o);
 } else {
  return DeeString_New(_self->mf_member_s);
 }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeMemberFunction_Call(
 DEE_A_IN_OBJECT(DeeMemberFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMemberFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 if ((_self->mf_flags&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL)!=0) {
  return (*_self->mf_struct.mfs_func)(_self->mf_struct.mfs_type,_self->mf_struct.mfs_self,args,_self->mf_closure);
 } else {
  return (*_self->mf_object.mfo_func)(_self->mf_object.mfo_self,args,_self->mf_closure);
 }
}




static void _deememberfunction_tp_dtor(DeeMemberFunctionObject *self) {
 Dee_DECREF(self->mf_object.mfo_self); // This is tp_self in structured calls
 if ((self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB) != 0)
  Dee_DECREF(self->mf_member_o);
}
static DeeObject *_deememberfunction_tp_str(DeeMemberFunctionObject *self) {
 return DeeString_Newf("%s.%s",
                       (self->mf_flags&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL)!=0
                       ? DeeType_NAME(self->mf_struct.mfs_type)
                       : DeeType_NAME(Dee_TYPE(self->mf_object.mfo_self)),
                       DeeMemberFunction_MemberName((DeeObject *)self));
}
static DeeObject *_deememberfunction_tp_repr(DeeMemberFunctionObject *self) {
 return DeeString_Newf("<memberfunction %s.%s>",
                       (self->mf_flags&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL)!=0
                       ? DeeType_NAME(self->mf_struct.mfs_type)
                       : DeeType_NAME(Dee_TYPE(self->mf_object.mfo_self)),
                       DeeMemberFunction_MemberName((DeeObject *)self));
}
static int _deememberfunction_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeMemberFunctionObject *self, DeeMemberFunctionObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMemberFunction_Check(self));
 Dee_INCREF(self->mf_object.mfo_self = right->mf_object.mfo_self); // This is tp_self in structured calls
 self->mf_object.mfo_func = right->mf_object.mfo_func;
 if (((self->mf_flags=right->mf_flags)&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL) == 0)
  self->mf_struct.mfs_self = right->mf_struct.mfs_self;
 self->mf_member_o = right->mf_member_o; // This might be a 'char const *'
 if ((self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB) != 0) Dee_INCREF(self->mf_member_o);
 self->mf_closure = right->mf_closure;
 return 0;
}
static int _deememberfunction_tp_copy_assign(DeeMemberFunctionObject *self, DeeMemberFunctionObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMemberFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeMemberFunction_Check(right));
 if (self != right) {
  _deememberfunction_tp_dtor(self);
  _deememberfunction_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
DEE_VISIT_PROC(_deememberfunction_tp_visit,DeeMemberFunctionObject *self) {
 Dee_VISIT(self->mf_object.mfo_self); // This is tp_self in structured calls
 if ((self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB)!=0)
  Dee_VISIT(self->mf_member_o);
}

static DeeObject *_deememberfunction_tp_cmp_lo(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&rhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(
  lhs->mf_object.mfo_self < rhs->mf_object.mfo_self || (
  lhs->mf_object.mfo_self == rhs->mf_object.mfo_self &&
  lhs->mf_object.mfo_func < rhs->mf_object.mfo_func));
}
static DeeObject *_deememberfunction_tp_cmp_le(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&lhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(
  lhs->mf_object.mfo_self < rhs->mf_object.mfo_self || (
  lhs->mf_object.mfo_self == rhs->mf_object.mfo_self &&
  lhs->mf_object.mfo_func <= rhs->mf_object.mfo_func));
}
static DeeObject *_deememberfunction_tp_cmp_eq(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&lhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(lhs->mf_object.mfo_self == rhs->mf_object.mfo_self &&
                lhs->mf_object.mfo_func == rhs->mf_object.mfo_func);
}
static DeeObject *_deememberfunction_tp_cmp_ne(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&lhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(lhs->mf_object.mfo_self != rhs->mf_object.mfo_self ||
                lhs->mf_object.mfo_func != rhs->mf_object.mfo_func);
}
static DeeObject *_deememberfunction_tp_cmp_gr(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&lhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(
  lhs->mf_object.mfo_self > rhs->mf_object.mfo_self || (
  lhs->mf_object.mfo_self == rhs->mf_object.mfo_self &&
  lhs->mf_object.mfo_func > rhs->mf_object.mfo_func));
}
static DeeObject *_deememberfunction_tp_cmp_ge(DeeMemberFunctionObject *lhs, DeeMemberFunctionObject *rhs) {
 if (DeeObject_InplaceGetInstance(&lhs,&DeeMemberFunction_Type) != 0) return NULL;
 // TODO: Object-compare 'mf_self' if it is an object
 DeeReturn_Bool(
  lhs->mf_object.mfo_self > rhs->mf_object.mfo_self || (
  lhs->mf_object.mfo_self == rhs->mf_object.mfo_self &&
  lhs->mf_object.mfo_func >= rhs->mf_object.mfo_func));
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deememberfunction_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deememberfunction_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_object_mfo_self",DeeMemberFunctionObject,mf_object.mfo_self,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_object_mfo_func",DeeMemberFunctionObject,mf_object.mfo_func,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_struct_mfs_type",DeeMemberFunctionObject,mf_struct.mfs_type,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_struct_mfs_func",DeeMemberFunctionObject,mf_struct.mfs_func,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_struct_mfs_self",DeeMemberFunctionObject,mf_struct.mfs_self,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_closure",DeeMemberFunctionObject,mf_closure,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_flags",DeeMemberFunctionObject,mf_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_member_o",DeeMemberFunctionObject,mf_member_o,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__mf_member_s",DeeMemberFunctionObject,mf_member_s,string),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */


static int DEE_CALL _deememberfunction_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMemberFunctionObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 DeeTypeObject *tp_ob; struct DeeMethodDef const *methods;
 char const *member_name;
 self->mf_flags = DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB;
 if ((self->mf_object.mfo_self = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if ((self->mf_member_o = (DeeStringObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_self: Dee_DECREF(self->mf_object.mfo_self); return -1; }
 if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->mf_member_o,&DeeString_Type) != 0) {err_membero: Dee_DECREF(self->mf_member_o); goto err_self; }
 tp_ob = Dee_TYPE(self->mf_object.mfo_self);
 member_name = DeeString_STR(self->mf_member_o);
 while (1) {
  methods = DeeType_GET_SLOT(tp_ob,tp_methods);
  while (methods->d_name) {
   if (strcmp(member_name,methods->d_name) == 0) {
    self->mf_object.mfo_func = methods->d_func; // Found it!
    self->mf_closure = methods->d_closure;
    return 0;
   }
   ++methods;
  }
  if (tp_ob == &DeeType_Type) {
   DEE_ASSERT(DeeType_Check(self->mf_object.mfo_self));
   methods = DeeType_GET_SLOT(self->mf_object.mfo_self,tp_class_methods);
   while (methods->d_name) {
    if (strcmp(member_name,methods->d_name) == 0) {
     self->mf_object.mfo_func = methods->d_func; // Found it!
     self->mf_closure = methods->d_closure;
     return 0;
    }
    ++methods;
   }
  }
  if ((tp_ob = DeeType_BASE(tp_ob)) == NULL) break;
 }
#if 0
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "[%k] Could not find generic member function %q in %q:%r",
                     infile,DeeString_STR(self->mf_member_o),
                     DeeType_NAME(Dee_TYPE(self->mf_object.mfo_self)),
                     self->mf_object.mfo_self);
#else
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Could not find generic member function %q in %s%q",
                     DeeString_STR(self->mf_member_o),
                     DeeType_Check(self->mf_object.mfo_self) ? "" : "instance of ",
                     DeeType_Check(self->mf_object.mfo_self) ? DeeType_NAME(self->mf_object.mfo_self)
                     : DeeType_NAME(Dee_TYPE(self->mf_object.mfo_self)));
#endif
 goto err_membero;
}
static int DEE_CALL _deememberfunction_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMemberFunctionObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if ((self->mf_flags&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL)!=0) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Structured member function %k cannot be marshaled",
                      self);
  return -1;
 }
 if (DeeMarshal_WriteObjectWithMap(outfile,map,self->mf_object.mfo_self) != 0) return -1;
 // Write the member function name
 return ((self->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB)!=0)
  ? DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->mf_member_o)
  : DeeMarshal_WriteStringWithMapAndLength(outfile,map,strlen(self->mf_member_s),self->mf_member_s);
}

static struct DeeTypeMarshal _deememberfunction_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(memberfunction)),
 member(&_deememberfunction_tp_marshal_ctor),
 member(&_deememberfunction_tp_marshal_put));

static DeeMemberFunctionObject *_deememberfunction_tp_alloc(DeeTypeObject *tp_self) {
 DeeMemberFunctionObject *result;
 result = DEE_OBJECTPOOL_ALLOC(memberfunction);
 if (result) DeeObject_INIT(result,tp_self);
 return result;
}
static void _deememberfunction_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMemberFunctionObject *self) {
 DEE_OBJECTPOOL_FREE(memberfunction,self);
}


DeeTypeObject DeeMemberFunction_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("memberfunction"),null,member(
  DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeMemberFunctionObject),
  member(&_deememberfunction_tp_alloc),null,
  member(&_deememberfunction_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deememberfunction_tp_free),
  member(&_deememberfunction_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deememberfunction_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deememberfunction_tp_str),
  member(&_deememberfunction_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(
  member(&DeeMemberFunction_Call),
  member(&_deememberfunction_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deememberfunction_tp_cmp_lo),
  member(&_deememberfunction_tp_cmp_le),
  member(&_deememberfunction_tp_cmp_eq),
  member(&_deememberfunction_tp_cmp_ne),
  member(&_deememberfunction_tp_cmp_gr),
  member(&_deememberfunction_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deememberfunction_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deememberfunction_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_MEMBERFUNCTION_C */
