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

#ifdef __INTELLISENSE__
#include "class.c"
DEE_DECL_BEGIN
//#define VALUE
#endif

#ifdef VALUE
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeClass_SetSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self,
 DEE_A_IN int slot, DEE_A_INOUT DeeObject *callback)
#else
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeClass_DelSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN int slot)
#endif
{
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeObject **v_slot;
#define SET_VSLOT(x) (v_slot=&(suffix->x))
#else
#define SET_VSLOT(x) (void)0
#endif
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define GET_OPERATOR(name,loc) suffix->loc
#else
#define GET_OPERATOR(name,loc) DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(name))
#endif
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
#ifdef VALUE
 DEE_ASSERT(DeeObject_Check(callback));
#endif
 suffix = DeeClass_SUFFIX(self);
 switch (slot) {

  // tp_ctor/tp_clear
  case DeeType_SLOT_ID(tp_dtor):
  case DeeType_SLOT_ID(tp_clear):
   SET_VSLOT(cs_destructor.ctp_dtor);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_clear) = (void(*)(DeeObject*))&_deeinstance_tp_clear_default;
   DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_ALWAYS_CLEAR;
#else
   DeeType_GET_SLOT(self,tp_clear) = (void(*)(DeeObject*))&_deeinstance_tp_clear_builtin;
   DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_ALWAYS_CLEAR;
#endif
   break;

  // tp_ctor <---> tp_copy_ctor <---> tp_move_ctor <---> tp_any_ctor
  case DeeType_SLOT_ID(tp_ctor):
   SET_VSLOT(cs_constructor.ctp_ctor);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_default;
   if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctor;
   else if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callcopy)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctorcopy;
#else
   if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctor)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
   else if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctorcopy)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callcopy;
   DeeType_GET_SLOT(self,tp_ctor) = GET_OPERATOR(tp_any_ctor,cs_constructor.ctp_any_ctor)
    ? (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_callany
    : (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_builtin;
#endif
   break;
  case DeeType_SLOT_ID(tp_copy_ctor):
   SET_VSLOT(cs_constructor.ctp_copy_ctor);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_default;
   if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callcopy;
   else if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctor)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctorcopy;
   if (DeeType_GET_SLOT(self,tp_move_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callcopy;
#else
   if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callcopy)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
   else if (DeeType_GET_SLOT(self,tp_any_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctorcopy)
    DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctor;
   if (DeeType_GET_SLOT(self,tp_move_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callcopy)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
   DeeType_GET_SLOT(self,tp_copy_ctor) = GET_OPERATOR(tp_any_ctor,cs_constructor.ctp_any_ctor)
    ? (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_callany
    : (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
#endif
   break;
  case DeeType_SLOT_ID(tp_move_ctor):
   SET_VSLOT(cs_constructor.ctp_move_ctor);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_default;
#else
   DeeType_GET_SLOT(self,tp_move_ctor) = GET_OPERATOR(tp_copy_ctor,cs_constructor.ctp_copy_ctor)
    ? (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callcopy : GET_OPERATOR(tp_any_ctor,cs_constructor.ctp_any_ctor)
    ? (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callany
    : (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
#endif
   break;
  case DeeType_SLOT_ID(tp_any_ctor):
   SET_VSLOT(cs_constructor.ctp_any_ctor);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_default;
   if (DeeType_GET_SLOT(self,tp_move_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callany;
   if (DeeType_GET_SLOT(self,tp_copy_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_callany;
   if (DeeType_GET_SLOT(self,tp_ctor) == (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_builtin)
    DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_callany;
#else
   if (DeeType_GET_SLOT(self,tp_move_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_callany)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
   if (DeeType_GET_SLOT(self,tp_copy_ctor) == (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_callany)
    DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
   if (DeeType_GET_SLOT(self,tp_ctor) == (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_callany)
    DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_builtin;
   DeeType_GET_SLOT(self,tp_any_ctor) = GET_OPERATOR(tp_copy_ctor,cs_constructor.ctp_copy_ctor)
    ? (GET_OPERATOR(tp_ctor,cs_constructor.ctp_ctor)
     ? (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctorcopy
     : (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callcopy)
    : GET_OPERATOR(tp_ctor,cs_constructor.ctp_ctor)
    ? (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_callctor
    : (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
#endif
   break;

  // tp_copy_assign <---> tp_move_assign <---> tp_any_assign
  case DeeType_SLOT_ID(tp_any_assign):
   SET_VSLOT(cs_assign.ctp_any_assign);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_any_assign_default;
   if (DeeType_GET_SLOT(self,tp_move_assign) == DeeType_DEFAULT_SLOT(tp_move_assign))
    DeeType_GET_SLOT(self,tp_move_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callany;
   if (DeeType_GET_SLOT(self,tp_copy_assign) == DeeType_DEFAULT_SLOT(tp_copy_assign))
    DeeType_GET_SLOT(self,tp_copy_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_any_assign_default;
#else
   if (DeeType_GET_SLOT(self,tp_move_assign) == (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callany)
    DeeType_GET_SLOT(self,tp_move_assign) = GET_OPERATOR(tp_copy_assign,cs_assign.ctp_copy_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callcopy
    : DeeType_DEFAULT_SLOT(tp_move_assign);
   if (DeeType_GET_SLOT(self,tp_copy_assign) == (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_copy_assign_callany)
    DeeType_GET_SLOT(self,tp_copy_assign) = DeeType_DEFAULT_SLOT(tp_copy_assign);
   DeeType_GET_SLOT(self,tp_any_assign) = GET_OPERATOR(tp_copy_assign,cs_assign.ctp_copy_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_any_assign_callcopy
    : DeeType_DEFAULT_SLOT(tp_any_assign);
#endif
   break;
  case DeeType_SLOT_ID(tp_copy_assign):
   SET_VSLOT(cs_assign.ctp_copy_assign);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_copy_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_copy_assign_default;
   if (DeeType_GET_SLOT(self,tp_any_assign) == DeeType_DEFAULT_SLOT(tp_any_assign))
    DeeType_GET_SLOT(self,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_any_assign_callcopy;
   if (DeeType_GET_SLOT(self,tp_move_assign) == DeeType_DEFAULT_SLOT(tp_move_assign) ||
       DeeType_GET_SLOT(self,tp_move_assign) == (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callany)
       DeeType_GET_SLOT(self,tp_move_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callcopy;
#else
   if (DeeType_GET_SLOT(self,tp_any_assign) == (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_any_assign_callcopy)
    DeeType_GET_SLOT(self,tp_any_assign) = DeeType_DEFAULT_SLOT(tp_any_assign);
   if (DeeType_GET_SLOT(self,tp_move_assign) == (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callcopy)
    DeeType_GET_SLOT(self,tp_move_assign) = GET_OPERATOR(tp_any_assign,cs_assign.ctp_any_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callany
    : DeeType_DEFAULT_SLOT(tp_move_assign);
   DeeType_GET_SLOT(self,tp_copy_assign) = GET_OPERATOR(tp_any_assign,cs_assign.ctp_any_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_copy_assign_callany
    : DeeType_DEFAULT_SLOT(tp_copy_assign);
#endif
   break;
  case DeeType_SLOT_ID(tp_move_assign):
   SET_VSLOT(cs_assign.ctp_move_assign);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_move_assign) = (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_default;
#else
   DeeType_GET_SLOT(self,tp_move_assign) = GET_OPERATOR(tp_copy_assign,cs_assign.ctp_copy_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callcopy : GET_OPERATOR(tp_any_assign,cs_assign.ctp_any_assign)
    ? (int(*)(DeeObject*,DeeObject*))&_deeinstance_tp_move_assign_callany
    : DeeType_DEFAULT_SLOT(tp_move_assign);
#endif
   break;

  // tp_not <---> tp_bool
  case DeeType_SLOT_ID(tp_not):
   SET_VSLOT(cs_math.ctp_not);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_not) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_not;
   if (DeeType_GET_SLOT(self,tp_bool) == DeeType_DEFAULT_SLOT(tp_bool))
    DeeType_GET_SLOT(self,tp_bool) = (int(*)(DeeObject*))&_deeinstance_tp_bool_callnot;
#else
   if (DeeType_GET_SLOT(self,tp_bool) == (int(*)(DeeObject*))&_deeinstance_tp_bool_callnot)
    DeeType_GET_SLOT(self,tp_bool) = DeeType_DEFAULT_SLOT(tp_bool);
   DeeType_GET_SLOT(self,tp_not) = GET_OPERATOR(tp_bool,cs_math.ctp_bool)
    ? (DeeObject*(*)(DeeObject*))&_deeinstance_tp_not_callbool
    : DeeType_DEFAULT_SLOT(tp_not);
#endif
   break;
  case DeeType_SLOT_ID(tp_bool):
   SET_VSLOT(cs_math.ctp_bool);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_bool) = (int(*)(DeeObject*))&_deeinstance_tp_bool;
   if (DeeType_GET_SLOT(self,tp_not) == DeeType_DEFAULT_SLOT(tp_not))
    DeeType_GET_SLOT(self,tp_not) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_not_callbool;
#else
   if (DeeType_GET_SLOT(self,tp_not) == (DeeObject*(*)(DeeObject*))&_deeinstance_tp_not_callbool)
    DeeType_GET_SLOT(self,tp_not) = DeeType_DEFAULT_SLOT(tp_not);
   DeeType_GET_SLOT(self,tp_bool) = GET_OPERATOR(tp_not,cs_math.ctp_not)
    ? (int(*)(DeeObject*))&_deeinstance_tp_bool_callnot
    : DeeType_DEFAULT_SLOT(tp_bool);
#endif
   break;

  // tp_int32 <---> tp_int64 <---> tp_double
  case DeeType_SLOT_ID(tp_int32):
   SET_VSLOT(cs_cast.ctp_int32);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_int32) = (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32;
   if (DeeType_GET_SLOT(self,tp_int64) == DeeType_DEFAULT_SLOT(tp_int64) ||
       DeeType_GET_SLOT(self,tp_int64) == (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_calldouble)
       DeeType_GET_SLOT(self,tp_int64) = (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_callint32;
   if (DeeType_GET_SLOT(self,tp_double) == DeeType_DEFAULT_SLOT(tp_double))
    DeeType_GET_SLOT(self,tp_double) = (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint32;
#else
   DeeType_GET_SLOT(self,tp_int32) = GET_OPERATOR(tp_int64,cs_cast.ctp_int64)
    ? (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_callint64 : GET_OPERATOR(tp_double,cs_cast.ctp_double)
    ? (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_calldouble
    : DeeType_DEFAULT_SLOT(tp_int32);
#endif
   break;
  case DeeType_SLOT_ID(tp_int64):
   SET_VSLOT(cs_cast.ctp_int64);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_int64) = (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64;
   if (DeeType_GET_SLOT(self,tp_int32) == DeeType_DEFAULT_SLOT(tp_int32))
    DeeType_GET_SLOT(self,tp_int32) = (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_callint64;
   if (DeeType_GET_SLOT(self,tp_double) == DeeType_DEFAULT_SLOT(tp_double))
    DeeType_GET_SLOT(self,tp_double) = (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint64;
#else
   if (DeeType_GET_SLOT(self,tp_int32) == (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_callint64)
    DeeType_GET_SLOT(self,tp_int32) = GET_OPERATOR(tp_double,cs_cast.ctp_double)
    ? (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_calldouble : DeeType_DEFAULT_SLOT(tp_int32);
   if (DeeType_GET_SLOT(self,tp_double) == (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint64)
    DeeType_GET_SLOT(self,tp_double) = GET_OPERATOR(tp_int32,cs_cast.ctp_int32)
    ? (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint32 : DeeType_DEFAULT_SLOT(tp_double);
   DeeType_GET_SLOT(self,tp_int64) = GET_OPERATOR(tp_int32,cs_cast.ctp_int32)
    ? (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_callint32 : GET_OPERATOR(tp_double,cs_cast.ctp_double)
    ? (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_calldouble
    : DeeType_DEFAULT_SLOT(tp_int64);
#endif
   break;
  case DeeType_SLOT_ID(tp_double):
   SET_VSLOT(cs_cast.ctp_double);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_double) = (int(*)(DeeObject*,double*))&_deeinstance_tp_double;
   if (DeeType_GET_SLOT(self,tp_int32) == DeeType_DEFAULT_SLOT(tp_int32))
    DeeType_GET_SLOT(self,tp_int32) = (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_calldouble;
   if (DeeType_GET_SLOT(self,tp_int64) == DeeType_DEFAULT_SLOT(tp_int64))
    DeeType_GET_SLOT(self,tp_int64) = (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_calldouble;
#else
   if (DeeType_GET_SLOT(self,tp_int32) == (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_calldouble)
    DeeType_GET_SLOT(self,tp_int32) = GET_OPERATOR(tp_int64,cs_cast.ctp_int64)
    ? (int(*)(DeeObject*,Dee_int32_t*))&_deeinstance_tp_int32_callint64 : DeeType_DEFAULT_SLOT(tp_int32);
   if (DeeType_GET_SLOT(self,tp_int64) == (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_calldouble)
    DeeType_GET_SLOT(self,tp_int64) = GET_OPERATOR(tp_int32,cs_cast.ctp_int32)
    ? (int(*)(DeeObject*,Dee_int64_t*))&_deeinstance_tp_int64_callint32 : DeeType_DEFAULT_SLOT(tp_int64);
   DeeType_GET_SLOT(self,tp_double) = GET_OPERATOR(tp_int64,cs_cast.ctp_int64)
    ? (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint64 : GET_OPERATOR(tp_int32,cs_cast.ctp_int32)
    ? (int(*)(DeeObject*,double*))&_deeinstance_tp_double_callint32
    : DeeType_DEFAULT_SLOT(tp_double);
#endif
   break;

  // tp_inc <---> tp_incpost
  case DeeType_SLOT_ID(tp_inc):
   SET_VSLOT(cs_math.ctp_inc);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_inc) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_inc;
   if (DeeType_GET_SLOT(self,tp_incpost) == DeeType_DEFAULT_SLOT(tp_incpost))
    DeeType_GET_SLOT(self,tp_incpost) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_incpost_callinc;
#else
   if (DeeType_GET_SLOT(self,tp_incpost) == (DeeObject*(*)(DeeObject*))&_deeinstance_tp_incpost_callinc)
    DeeType_GET_SLOT(self,tp_incpost) = DeeType_DEFAULT_SLOT(tp_incpost);
   DeeType_GET_SLOT(self,tp_inc) = DeeType_DEFAULT_SLOT(tp_inc);
#endif
   break;
  case DeeType_SLOT_ID(tp_incpost):
   SET_VSLOT(cs_math.ctp_incpost);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_incpost) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_incpost;
#else
   DeeType_GET_SLOT(self,tp_incpost) = GET_OPERATOR(tp_inc,cs_math.ctp_inc)
    ? (DeeObject*(*)(DeeObject*))&_deeinstance_tp_incpost_callinc
    : DeeType_DEFAULT_SLOT(tp_incpost);
#endif
   break;

  // tp_dec <---> tp_decpost
  case DeeType_SLOT_ID(tp_dec):
   SET_VSLOT(cs_math.ctp_dec);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_dec) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_dec;
   if (DeeType_GET_SLOT(self,tp_decpost) == DeeType_DEFAULT_SLOT(tp_decpost))
    DeeType_GET_SLOT(self,tp_decpost) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_decpost_calldec;
#else
   if (DeeType_GET_SLOT(self,tp_decpost) == (DeeObject*(*)(DeeObject*))&_deeinstance_tp_decpost_calldec)
    DeeType_GET_SLOT(self,tp_decpost) = DeeType_DEFAULT_SLOT(tp_decpost);
   DeeType_GET_SLOT(self,tp_dec) = DeeType_DEFAULT_SLOT(tp_dec);
#endif
   break;
  case DeeType_SLOT_ID(tp_decpost):
   SET_VSLOT(cs_math.ctp_decpost);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_decpost) = (DeeObject*(*)(DeeObject*))&_deeinstance_tp_decpost;
#else
   DeeType_GET_SLOT(self,tp_decpost) = GET_OPERATOR(tp_dec,cs_math.ctp_dec)
    ? (DeeObject*(*)(DeeObject*))&_deeinstance_tp_decpost_calldec
    : DeeType_DEFAULT_SLOT(tp_decpost);
#endif
   break;

  case DeeType_SLOT_ID(tp_cmp_lo):
   SET_VSLOT(cs_compare.ctp_cmp_lo);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_lo) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_lo;
   if (DeeType_GET_SLOT(self,tp_cmp_ge) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ge_calllo)
    DeeType_GET_SLOT(self,tp_cmp_ge) = DeeType_DEFAULT_SLOT(tp_cmp_ge);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_ge) == DeeType_DEFAULT_SLOT(tp_cmp_ge))
    DeeType_GET_SLOT(self,tp_cmp_ge) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ge_calllo;
   DeeType_GET_SLOT(self,tp_cmp_lo) = GET_OPERATOR(tp_cmp_ge,cs_compare.ctp_cmp_ge)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_lo_callge
    : DeeType_DEFAULT_SLOT(tp_cmp_lo);
#endif
   break;
  case DeeType_SLOT_ID(tp_cmp_ge):
   SET_VSLOT(cs_compare.ctp_cmp_ge);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_ge) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ge;
   if (DeeType_GET_SLOT(self,tp_cmp_lo) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_lo_callge)
    DeeType_GET_SLOT(self,tp_cmp_lo) = DeeType_DEFAULT_SLOT(tp_cmp_lo);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_lo) == DeeType_DEFAULT_SLOT(tp_cmp_lo))
    DeeType_GET_SLOT(self,tp_cmp_lo) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_lo_callge;
   DeeType_GET_SLOT(self,tp_cmp_ge) = GET_OPERATOR(tp_cmp_lo,cs_compare.ctp_cmp_lo)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ge_calllo
    : DeeType_DEFAULT_SLOT(tp_cmp_ge);
#endif
   break;

  case DeeType_SLOT_ID(tp_cmp_le):
   SET_VSLOT(cs_compare.ctp_cmp_le);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_le) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_le;
   if (DeeType_GET_SLOT(self,tp_cmp_gr) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_gr_callle)
    DeeType_GET_SLOT(self,tp_cmp_gr) = DeeType_DEFAULT_SLOT(tp_cmp_gr);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_gr) == DeeType_DEFAULT_SLOT(tp_cmp_gr))
    DeeType_GET_SLOT(self,tp_cmp_gr) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_gr_callle;
   DeeType_GET_SLOT(self,tp_cmp_le) = GET_OPERATOR(tp_cmp_gr,cs_compare.ctp_cmp_gr)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_le_callgr
    : DeeType_DEFAULT_SLOT(tp_cmp_le);
#endif
   break;
  case DeeType_SLOT_ID(tp_cmp_gr):
   SET_VSLOT(cs_compare.ctp_cmp_gr);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_gr) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_gr;
   if (DeeType_GET_SLOT(self,tp_cmp_le) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_le_callgr)
    DeeType_GET_SLOT(self,tp_cmp_le) = DeeType_DEFAULT_SLOT(tp_cmp_le);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_le) == DeeType_DEFAULT_SLOT(tp_cmp_le))
    DeeType_GET_SLOT(self,tp_cmp_le) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_le_callgr;
   DeeType_GET_SLOT(self,tp_cmp_gr) = GET_OPERATOR(tp_cmp_le,cs_compare.ctp_cmp_le)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_gr_callle
    : DeeType_DEFAULT_SLOT(tp_cmp_gr);
#endif
   break;

  case DeeType_SLOT_ID(tp_cmp_eq):
   SET_VSLOT(cs_compare.ctp_cmp_eq);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_eq) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_eq;
   if (DeeType_GET_SLOT(self,tp_cmp_ne) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ne_calleq)
    DeeType_GET_SLOT(self,tp_cmp_ne) = DeeType_DEFAULT_SLOT(tp_cmp_ne);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_ne) == DeeType_DEFAULT_SLOT(tp_cmp_ne))
    DeeType_GET_SLOT(self,tp_cmp_ne) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ne_calleq;
   DeeType_GET_SLOT(self,tp_cmp_eq) = GET_OPERATOR(tp_cmp_ne,cs_compare.ctp_cmp_ne)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_eq_callne
    : DeeType_DEFAULT_SLOT(tp_cmp_eq);
#endif
   break;
  case DeeType_SLOT_ID(tp_cmp_ne):
   SET_VSLOT(cs_compare.ctp_cmp_ne);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_cmp_ne) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ne;
   if (DeeType_GET_SLOT(self,tp_cmp_eq) == (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_eq_callne)
    DeeType_GET_SLOT(self,tp_cmp_eq) = DeeType_DEFAULT_SLOT(tp_cmp_eq);
#else
   if (DeeType_GET_SLOT(self,tp_cmp_eq) == DeeType_DEFAULT_SLOT(tp_cmp_eq))
    DeeType_GET_SLOT(self,tp_cmp_eq) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_eq_callne;
   DeeType_GET_SLOT(self,tp_cmp_ne) = GET_OPERATOR(tp_cmp_eq,cs_compare.ctp_cmp_eq)
    ? (DeeObject*(*)(DeeObject*,DeeObject*))&_deeinstance_tp_cmp_ne_calleq
    : DeeType_DEFAULT_SLOT(tp_cmp_ne);
#endif
   break;
  case DeeType_SLOT_ID(tp_hash):
   SET_VSLOT(cs_math.ctp_hash);
   break;

#ifdef VALUE
#define OPERATOR_SLOT(tp_name,tp_c_name)\
  case DeeType_SLOT_ID(tp_name):\
   SET_VSLOT(tp_c_name);\
   DeeType_GET_SLOT(self,tp_name) = (DeeType_SLOT_TYPE(tp_name))&_deeinstance_##tp_name;\
   break
#else
#define OPERATOR_SLOT(tp_name,tp_c_name)\
  case DeeType_SLOT_ID(tp_name):\
   SET_VSLOT(tp_c_name);\
   DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);\
   break
#endif
  OPERATOR_SLOT(tp_str,     cs_cast.ctp_str);
  OPERATOR_SLOT(tp_repr,    cs_cast.ctp_repr);
  OPERATOR_SLOT(tp_call,    cs_object.ctp_call);
  OPERATOR_SLOT(tp_inv,     cs_math.ctp_inv);
  OPERATOR_SLOT(tp_pos,     cs_math.ctp_pos);
  OPERATOR_SLOT(tp_neg,     cs_math.ctp_neg);
  // TODO: add from copy+iadd
  // TODO: iadd from add+move_assign
  OPERATOR_SLOT(tp_add,     cs_math.ctp_add);
  OPERATOR_SLOT(tp_iadd,    cs_math.ctp_iadd);
  OPERATOR_SLOT(tp_sub,     cs_math.ctp_sub);
  OPERATOR_SLOT(tp_isub,    cs_math.ctp_isub);
  OPERATOR_SLOT(tp_mul,     cs_math.ctp_mul);
  OPERATOR_SLOT(tp_imul,    cs_math.ctp_imul);
  OPERATOR_SLOT(tp_div,     cs_math.ctp_div);
  OPERATOR_SLOT(tp_idiv,    cs_math.ctp_idiv);
  OPERATOR_SLOT(tp_mod,     cs_math.ctp_mod);
  OPERATOR_SLOT(tp_imod,    cs_math.ctp_imod);
  OPERATOR_SLOT(tp_shl,     cs_math.ctp_shl);
  OPERATOR_SLOT(tp_ishl,    cs_math.ctp_ishl);
  OPERATOR_SLOT(tp_shr,     cs_math.ctp_shr);
  OPERATOR_SLOT(tp_ishr,    cs_math.ctp_ishr);
  OPERATOR_SLOT(tp_and,     cs_math.ctp_and);
  OPERATOR_SLOT(tp_iand,    cs_math.ctp_iand);
  OPERATOR_SLOT(tp_or,      cs_math.ctp_or);
  OPERATOR_SLOT(tp_ior,     cs_math.ctp_ior);
  OPERATOR_SLOT(tp_xor,     cs_math.ctp_xor);
  OPERATOR_SLOT(tp_ixor,    cs_math.ctp_ixor);
  OPERATOR_SLOT(tp_pow,     cs_math.ctp_pow);
  OPERATOR_SLOT(tp_ipow,    cs_math.ctp_ipow);
  OPERATOR_SLOT(tp_seq_set, cs_seq.ctp_seq_set);
  OPERATOR_SLOT(tp_seq_del, cs_seq.ctp_seq_del);
  OPERATOR_SLOT(tp_seq_range_del,cs_seq.ctp_seq_range_del);
  OPERATOR_SLOT(tp_seq_range_set,cs_seq.ctp_seq_range_set);
  OPERATOR_SLOT(tp_seq_iter_next,cs_seq.ctp_seq_iter_next);
#undef OPERATOR_SLOT

  case DeeType_SLOT_ID(tp_seq_get):
   SET_VSLOT(cs_seq.ctp_seq_get);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_seq_get) = (DeeType_SLOT_TYPE(tp_seq_get))&_deeinstance_tp_seq_get;
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == DeeType_DEFAULT_SLOT(tp_seq_iter_self) &&
       DeeType_GET_SLOT(self,tp_seq_size) != DeeType_DEFAULT_SLOT(tp_seq_size))
       DeeType_GET_SLOT(self,tp_seq_iter_self) = &DeeGenericSequenceIterator_New;
#else
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == &DeeGenericSequenceIterator_New)
    DeeType_GET_SLOT(self,tp_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_seq_iter_self);
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) != DeeType_DEFAULT_SLOT(tp_seq_iter_self)) {
    DeeType_GET_SLOT(self,tp_seq_get) = &_deegenericiterable_tp_seq_get;
    if (DeeType_GET_SLOT(self,tp_seq_contains) == &_deegenericiterable_tp_seq_contains) DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
    if (DeeType_GET_SLOT(self,tp_seq_range_get) == &_deegenericiterable_tp_seq_range_get) DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
   } else DeeType_GET_SLOT(self,tp_seq_get) = DeeType_DEFAULT_SLOT(tp_seq_get);
#endif
   break;
  case DeeType_SLOT_ID(tp_seq_size):
   SET_VSLOT(cs_seq.ctp_seq_size);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_seq_size) = (DeeType_SLOT_TYPE(tp_seq_size))&_deeinstance_tp_seq_size;
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == DeeType_DEFAULT_SLOT(tp_seq_iter_self) &&
       DeeType_GET_SLOT(self,tp_seq_get) != DeeType_DEFAULT_SLOT(tp_seq_get))
       DeeType_GET_SLOT(self,tp_seq_iter_self) = &DeeGenericSequenceIterator_New;
#else
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == &DeeGenericSequenceIterator_New)
    DeeType_GET_SLOT(self,tp_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_seq_iter_self);
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) != DeeType_DEFAULT_SLOT(tp_seq_iter_self)) {
    DeeType_GET_SLOT(self,tp_seq_size) = &_deegenericiterable_tp_seq_size;
    if (DeeType_GET_SLOT(self,tp_seq_contains) == &_deegenericiterable_tp_seq_contains) DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
    if (DeeType_GET_SLOT(self,tp_seq_range_get) == &_deegenericiterable_tp_seq_range_get) DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
   } else DeeType_GET_SLOT(self,tp_seq_size) = DeeType_DEFAULT_SLOT(tp_seq_size);
#endif
   break;
  case DeeType_SLOT_ID(tp_seq_contains):
   SET_VSLOT(cs_seq.ctp_seq_contains);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_seq_contains) = (DeeType_SLOT_TYPE(tp_seq_contains))&_deeinstance_tp_seq_contains;
#else
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) != DeeType_DEFAULT_SLOT(tp_seq_iter_self)) {
    DeeType_GET_SLOT(self,tp_seq_contains) = &_deegenericiterable_tp_seq_contains;
   } else DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
#endif
   break;
  case DeeType_SLOT_ID(tp_seq_range_get):
   SET_VSLOT(cs_seq.ctp_seq_range_get);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_seq_range_get) = (DeeType_SLOT_TYPE(tp_seq_range_get))&_deeinstance_tp_seq_range_get;
#else
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) != DeeType_DEFAULT_SLOT(tp_seq_iter_self)) {
    DeeType_GET_SLOT(self,tp_seq_range_get) = &_deegenericiterable_tp_seq_range_get;
   } else DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
#endif
   break;
  case DeeType_SLOT_ID(tp_seq_iter_self):
  case DEE_CLASS_SLOTID_OPERATOR_FOR:
   SET_VSLOT(cs_seq.ctp_seq_iter_self);
#ifdef VALUE
   DeeType_GET_SLOT(self,tp_seq_iter_self) = slot == DEE_CLASS_SLOTID_OPERATOR_FOR
    ? (DeeType_SLOT_TYPE(tp_seq_iter_self))&_deeinstance_tp_seq_iter_self_callfor
    : (DeeType_SLOT_TYPE(tp_seq_iter_self))&_deeinstance_tp_seq_iter_self;
   if (DeeType_GET_SLOT(self,tp_seq_get) == DeeType_DEFAULT_SLOT(tp_seq_get)) DeeType_GET_SLOT(self,tp_seq_get) = &_deegenericiterable_tp_seq_get;
   if (DeeType_GET_SLOT(self,tp_seq_size) == DeeType_DEFAULT_SLOT(tp_seq_size)) DeeType_GET_SLOT(self,tp_seq_size) = &_deegenericiterable_tp_seq_size;
   if (DeeType_GET_SLOT(self,tp_seq_contains) == DeeType_DEFAULT_SLOT(tp_seq_contains)) DeeType_GET_SLOT(self,tp_seq_contains) = &_deegenericiterable_tp_seq_contains;
   if (DeeType_GET_SLOT(self,tp_seq_range_get) == DeeType_DEFAULT_SLOT(tp_seq_range_get)) DeeType_GET_SLOT(self,tp_seq_range_get) = &_deegenericiterable_tp_seq_range_get;
#else
   if (DeeType_GET_SLOT(self,tp_seq_size) != DeeType_DEFAULT_SLOT(tp_seq_size) &&
       DeeType_GET_SLOT(self,tp_seq_get) != DeeType_DEFAULT_SLOT(tp_seq_get)) {
    DeeType_GET_SLOT(self,tp_seq_iter_self) = &DeeGenericSequenceIterator_New;
   } else {
    DeeType_GET_SLOT(self,tp_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_seq_iter_self);
    if (DeeType_GET_SLOT(self,tp_seq_get) == &_deegenericiterable_tp_seq_get) DeeType_GET_SLOT(self,tp_seq_get) = DeeType_DEFAULT_SLOT(tp_seq_get);
    if (DeeType_GET_SLOT(self,tp_seq_size) == &_deegenericiterable_tp_seq_size) DeeType_GET_SLOT(self,tp_seq_size) = DeeType_DEFAULT_SLOT(tp_seq_size);
    if (DeeType_GET_SLOT(self,tp_seq_contains) == &_deegenericiterable_tp_seq_contains) DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
    if (DeeType_GET_SLOT(self,tp_seq_range_get) == &_deegenericiterable_tp_seq_range_get) DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
   }
#endif
   break;

  case DeeType_SLOT_ID(tp_attr_get): SET_VSLOT(cs_attr.ctp_attr_get); break;
  case DeeType_SLOT_ID(tp_attr_del): SET_VSLOT(cs_attr.ctp_attr_del); break;
  case DeeType_SLOT_ID(tp_attr_set): SET_VSLOT(cs_attr.ctp_attr_set); break;

  {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define FILE_SET_VSLOT(x) (v_slot=&(((struct DeeFileClassSuffix *)suffix)->x))
#else
#define FILE_SET_VSLOT(x) (void)0
#endif
   case DeeType_SLOT_ID(tp_io_seek):
    if (!DeeFileClass_Check(self)) {
err_not_a_file_class:
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "The given slot id is only supported for file types: %d",
                         slot);
     return -1;
    }
#ifdef VALUE
    DeeType_GET_SLOT(self,tp_io_seek) = (DeeType_SLOT_TYPE(tp_io_seek))&_deefileinstance_tp_io_seek;
#else
    DeeType_GET_SLOT(self,tp_io_seek) = DeeType_DEFAULT_SLOT(tp_io_seek);
#endif
    FILE_SET_VSLOT(tp_file.ctp_io_seek); 
    break;
#ifdef VALUE
#define FILE_OPERATOR_SLOT_EX(slot_id,tp_name,tp_c_name,callback)\
   case slot_id:\
    if (!DeeFileClass_Check(self)) goto err_not_a_file_class;\
    DeeType_GET_SLOT(self,tp_name) = (DeeType_SLOT_TYPE(tp_name))&callback;\
    FILE_SET_VSLOT(tp_c_name);\
    break
#else
#define FILE_OPERATOR_SLOT_EX(slot_id,tp_name,tp_c_name,callback)\
   case slot_id:\
    if (!DeeFileClass_Check(self)) goto err_not_a_file_class;\
    DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);\
    FILE_SET_VSLOT(tp_c_name);\
    break
#endif
#define FILE_OPERATOR_SLOT(tp_name,tp_c_name,callback) \
 FILE_OPERATOR_SLOT_EX(DeeType_SLOT_ID(tp_name),tp_name,tp_c_name,callback)
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   FILE_OPERATOR_SLOT(tp_io_read,tp_file.ctp_io_read,_deefileinstance_tp_io_read);
   FILE_OPERATOR_SLOT(tp_io_write,tp_file.ctp_io_write,_deefileinstance_tp_io_write);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   FILE_OPERATOR_SLOT(tp_io_flush,tp_file.ctp_io_flush,_deefileinstance_tp_io_flush);
   FILE_OPERATOR_SLOT(tp_io_trunc,tp_file.ctp_io_trunc,_deefileinstance_tp_io_trunc);
   FILE_OPERATOR_SLOT(tp_io_close,tp_file.ctp_io_close,_deefileinstance_tp_io_close);
   FILE_OPERATOR_SLOT_EX(DEE_CLASS_SLOTID_OPERATOR_READNP,tp_io_read,tp_file.ctp_io_read,_deefileinstance_tp_io_read_np);
   FILE_OPERATOR_SLOT_EX(DEE_CLASS_SLOTID_OPERATOR_WRITENP,tp_io_write,tp_file.ctp_io_write,_deefileinstance_tp_io_write_np);
#undef FILE_OPERATOR_SLOT_EX
#undef FILE_OPERATOR_SLOT
  }
#undef FILE_SET_VSLOT

  default:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid/Unsupported slot id: %d",
                       slot);
   return -1;
 }
#undef GET_OPERATOR
#undef SET_VSLOT
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 Dee_XDECREF(*v_slot);
#ifdef VALUE
 Dee_INCREF(*v_slot = callback);
#endif
 return 0;
#else
#ifdef VALUE
 return DeeClassSuffix_SetVirtOperator(suffix,slot,callback);
#else
 return DeeClassSuffix_DelVirtOperator(suffix,slot);
#endif
#endif
}


#ifdef VALUE
#undef VALUE
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
