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
#ifndef GUARD_DEEMON_CFUNCTION_C
#define GUARD_DEEMON_CFUNCTION_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/cfunction.h>
#include <deemon/error.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>

// */ (nano...)

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// CFunction wrapper object

DEE_A_RET_OBJECT_EXCEPT_REF(DeeCFunctionObject) *
DeeCFunction_New(DEE_A_IN_Z_OPT char const *name, DEE_A_IN DeeCFunction func) {
 DeeCFunctionObject *result;
 if ((result = DeeObject_MALLOCF(
  DeeCFunctionObject,"simple_cfunction: %p",func)) != NULL) {
  DeeObject_INIT(result,&DeeCFunction_Type);
  result->cf_func = func;
  result->cf_name = name;
 }
 return (DeeObject *)result;
}
DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED DeeCFunction DeeCFunction_Func(
 DEE_A_IN_OBJECT(DeeCFunctionObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCFunction_Check(self));
 return DeeCFunction_FUNC(self);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeCFunction_Call(
 DEE_A_IN_OBJECT(DeeCFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 return DeeCFunction_CALL(self,args);
}




DEE_COMPILER_MSVC_WARNING_PUSH(4054)
static DeeObject *_deecfunction_tp_str(DeeCFunctionObject *self) {
 if (self->cf_name) return DeeString_New(self->cf_name);
 return DeeString_Newf("<cfunction: %#p>",(void *)self->cf_func);
}
DEE_COMPILER_MSVC_WARNING_POP
static int _deecfunction_tp_copy_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeCFunctionObject *self, DeeObject *right) { self->cf_func = DeeCFunction_FUNC(right); return 0; }
static DeeObject *_deecfunction_tp_cmp_lo(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func <  DeeCFunction_FUNC(rhs)); }
static DeeObject *_deecfunction_tp_cmp_le(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func <= DeeCFunction_FUNC(rhs)); }
static DeeObject *_deecfunction_tp_cmp_eq(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func == DeeCFunction_FUNC(rhs)); }
static DeeObject *_deecfunction_tp_cmp_ne(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func != DeeCFunction_FUNC(rhs)); }
static DeeObject *_deecfunction_tp_cmp_gr(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func >  DeeCFunction_FUNC(rhs)); }
static DeeObject *_deecfunction_tp_cmp_ge(DeeCFunctionObject *lhs, DeeCFunctionObject *rhs) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeCFunction_Type) != 0) return NULL; DeeReturn_Bool(lhs->cf_func >= DeeCFunction_FUNC(rhs)); }

static struct DeeMemberDef const _deecfunction_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__name__",DeeCFunctionObject,cf_name,string),
 DEE_MEMBERDEF_NAMED_RO_v100("__doc__",DeeCFunctionObject,cf_doc,string),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__cf_name",DeeCFunctionObject,cf_name,p(char)),
 DEE_MEMBERDEF_NAMED_RO_v100("__cf_doc",DeeCFunctionObject,cf_doc,p(char)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4054)
static DeeObject *_deecfunction_cf_func_get(
 DeeCFunctionObject *self, void *DEE_UNUSED(closure)) {
 return DeePointer_Newf((void *)self->cf_func,"o(o)");
}
DEE_COMPILER_MSVC_WARNING_POP
static int _deecfunction_cf_func_set(
 DeeCFunctionObject *self, DeeObject *ob, void *DEE_UNUSED(closure)) {
 return DeeObject_GetPointerExf(ob,(void **)&self->cf_func,"o(o)");
}

#define _deecfunction_tp_getsets _deecfunction_tp_getsets
static struct DeeGetSetDef const _deecfunction_tp_getsets[] = {
 DEE_GETSETDEF_v100("__cf_func",
  member(&_deecfunction_cf_func_get),null,
  member(&_deecfunction_cf_func_set),
  "-> object(*)(object)"),
 DEE_GETSETDEF_END_v100
};
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

#ifndef _deecfunction_tp_getsets
#define _deecfunction_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#endif

DeeTypeObject DeeCFunction_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("cfunction"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCFunctionObject),null,null,
  member(&_deecfunction_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deecfunction_tp_str),
  member(&_deecfunction_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&DeeCFunction_Call),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deecfunction_tp_cmp_lo),
  member(&_deecfunction_tp_cmp_le),
  member(&_deecfunction_tp_cmp_eq),
  member(&_deecfunction_tp_cmp_ne),
  member(&_deecfunction_tp_cmp_gr),
  member(&_deecfunction_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deecfunction_tp_members),
  member(_deecfunction_tp_getsets),null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_CFUNCTION_C */
