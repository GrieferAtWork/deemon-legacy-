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
#ifndef GUARD_DEEMON_EXPR_AST_COMPILE_C
#define GUARD_DEEMON_EXPR_AST_COMPILE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/bool.h>
#include <deemon/cfunction.h>
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/expr_ast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/stmt_ast.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/fs_api.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/time.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/none.h>
#include <deemon/random.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/super.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
#include <deemon/cell.h>
#endif

DEE_DECL_BEGIN

#define _dee_type_is_float(tp) ((tp)==(DeeTypeObject *)&DeeFloat_Type||(tp)==(DeeTypeObject *)&DeeDouble_Type||(tp)==(DeeTypeObject *)&DeeLDouble_Type)
#define _dee_type_is_basic_signed(tp) ((tp)==(DeeTypeObject *)&DeeInt8_Type||(tp)==(DeeTypeObject *)&DeeInt16_Type||(tp)==(DeeTypeObject *)&DeeInt32_Type||(tp)==(DeeTypeObject *)&DeeInt64_Type)
#define _dee_type_is_atomic_signed(tp) ((tp)==(DeeTypeObject *)&DeeAtomicInt8_Type||(tp)==(DeeTypeObject *)&DeeAtomicInt16_Type||(tp)==(DeeTypeObject *)&DeeAtomicInt32_Type||(tp)==(DeeTypeObject *)&DeeAtomicInt64_Type)
#define _dee_type_is_basic_unsigned(tp) ((tp)==(DeeTypeObject *)&DeeUInt8_Type||(tp)==(DeeTypeObject *)&DeeUInt16_Type||(tp)==(DeeTypeObject *)&DeeUInt32_Type||(tp)==(DeeTypeObject *)&DeeUInt64_Type)
#define _dee_type_is_atomic_unsigned(tp) ((tp)==(DeeTypeObject *)&DeeAtomicUInt8_Type||(tp)==(DeeTypeObject *)&DeeAtomicUInt16_Type||(tp)==(DeeTypeObject *)&DeeAtomicUInt32_Type||(tp)==(DeeTypeObject *)&DeeAtomicUInt64_Type)
#define _dee_type_is_signed(tp) (_dee_type_is_basic_signed(tp)||_dee_type_is_atomic_signed(tp))
#define _dee_type_is_unsigned(tp) (_dee_type_is_basic_unsigned(tp)||_dee_type_is_atomic_unsigned(tp))
#define _dee_type_is_basic_int(tp) (_dee_type_is_basic_signed(tp)||_dee_type_is_basic_unsigned(tp))
#define _dee_type_is_atomic_int(tp) (_dee_type_is_atomic_signed(tp)||_dee_type_is_atomic_unsigned(tp))
#define _dee_type_is_int(tp) (_dee_type_is_basic_int(tp)||_dee_type_is_atomic_int(tp))


int _DeeType_IsSlotConstant(DeeTypeObject const *tp, DeeExprAstKind kind) {
 switch (kind) {
  case DeeExprAstKind_INC:
  case DeeExprAstKind_DEC:
  case DeeExprAstKind_INC_POST:
  case DeeExprAstKind_DEC_POST:
   if (tp == &DeeTime_Type) return 0;
  case DeeExprAstKind_IADD:
  case DeeExprAstKind_ISUB:
  case DeeExprAstKind_IMUL:
  case DeeExprAstKind_IDIV:
  case DeeExprAstKind_IPOW:
   if (_dee_type_is_float(tp)) return 0;
  case DeeExprAstKind_IMOD:
  case DeeExprAstKind_IAND:
  case DeeExprAstKind_IOR:
  case DeeExprAstKind_IXOR:
  case DeeExprAstKind_ISHL:
  case DeeExprAstKind_ISHR:
   return !_dee_type_is_int(tp);
  default: break;
 }
 return 1;
}





DEE_STATIC_INLINE(DeeTypeObject *) _dee_type_min_integral_int(DeeTypeObject *a) {
#if DEE_TYPES_SIZEOF_INT > 1
 if (
  a == (DeeTypeObject *)&DeeInt8_Type 
#if DEE_TYPES_SIZEOF_INT > 2
  || a == (DeeTypeObject *)&DeeInt16_Type
#endif
#if DEE_TYPES_SIZEOF_INT > 4
  || a == (DeeTypeObject *)&DeeInt32_Type
#endif
#if DEE_TYPES_SIZEOF_INT > 8
  || a == (DeeTypeObject *)&DeeInt64_Type
#endif
  ) return DeeObject_TYPE(Dee_rt_int);
 if (
  a == (DeeTypeObject *)&DeeUInt8_Type 
#if DEE_TYPES_SIZEOF_INT > 2
  || a == (DeeTypeObject *)&DeeUInt16_Type
#endif
#if DEE_TYPES_SIZEOF_INT > 4
  || a == (DeeTypeObject *)&DeeUInt32_Type
#endif
#if DEE_TYPES_SIZEOF_INT > 8
  || a == (DeeTypeObject *)&DeeUInt64_Type
#endif
  ) return DeeObject_TYPE(Dee_rt_uint);
#endif
 return a;
}

DEE_STATIC_INLINE(DeeTypeObject *) _dee_type_remove_atomic(DeeTypeObject *a) {
 if (a == (DeeTypeObject *)&DeeAtomicInt8_Type) return (DeeTypeObject *)&DeeInt8_Type;
 if (a == (DeeTypeObject *)&DeeAtomicInt16_Type) return (DeeTypeObject *)&DeeInt16_Type;
 if (a == (DeeTypeObject *)&DeeAtomicInt32_Type) return (DeeTypeObject *)&DeeInt32_Type;
 if (a == (DeeTypeObject *)&DeeAtomicInt64_Type) return (DeeTypeObject *)&DeeInt64_Type;
 if (a == (DeeTypeObject *)&DeeAtomicUInt8_Type) return (DeeTypeObject *)&DeeUInt8_Type;
 if (a == (DeeTypeObject *)&DeeAtomicUInt16_Type) return (DeeTypeObject *)&DeeUInt16_Type;
 if (a == (DeeTypeObject *)&DeeAtomicUInt32_Type) return (DeeTypeObject *)&DeeUInt32_Type;
 if (a == (DeeTypeObject *)&DeeAtomicUInt64_Type) return (DeeTypeObject *)&DeeUInt64_Type;
 return a;
}


DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_not(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_int(a) ||
     a == (DeeTypeObject *)&DeeBool_Type || a == &DeeString_Type ||
     a == (DeeTypeObject *)&DeeNone_Type || a == &DeeList_Type ||
     a == &DeeTuple_Type || a == &DeeDict_Type ||
     a == &DeeDictKeys_Type || a == &DeeDictItems_Type ||
     a == &DeeDictValues_Type
     ) return (DeeTypeObject *)&DeeBool_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_inv(DeeTypeObject *a) {
 if (_dee_type_is_basic_int(a)) return _dee_type_min_integral_int(a);
 if (_dee_type_is_atomic_int(a)) return _dee_type_min_integral_int(_dee_type_remove_atomic(a));
 if (a == (DeeTypeObject *)&DeeBool_Type) return (DeeTypeObject *)&DeeBool_Type;
 if (a == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_pos(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_int(a)) return a;
 if (a == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_neg(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_signed(a)) return a;
 if (a == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_inc_dec_opt_post(DeeTypeObject *a) {
 if (_dee_type_is_basic_int(a)) return a;
 if (_dee_type_is_atomic_int(a)) return _dee_type_remove_atomic(a);
 if (a == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_cmp(DeeTypeObject *lhs) {
 if (_dee_type_is_float(lhs) || _dee_type_is_int(lhs) ||
     lhs == (DeeTypeObject *)&DeeNone_Type || lhs == &DeeString_Type ||
     lhs == &DeeList_Type || lhs == &DeeTuple_Type ||
     lhs == &DeeType_Type || lhs == (DeeTypeObject *)&DeeBool_Type ||
     lhs == &DeeDict_Type
     ) return (DeeTypeObject *)&DeeBool_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_add(DeeTypeObject *lhs) {
 if (_dee_type_is_float(lhs)) return lhs;
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (lhs == &DeeString_Type) return &DeeString_Type;
 if (lhs == &DeeList_Type) return &DeeList_Type;
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_iadd(DeeTypeObject *lhs) {
 if (_dee_type_is_float(lhs)) return lhs;
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (lhs == &DeeList_Type) return &DeeList_Type;
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_math_def(DeeTypeObject *lhs) {
 if (_dee_type_is_float(lhs)) return lhs;
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_math_div(DeeTypeObject *lhs) {
 if (_dee_type_is_float(lhs)) return lhs;
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_mul(DeeTypeObject *lhs) {
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (_dee_type_is_float(lhs) || lhs == &DeeString_Type ||
     lhs == (DeeTypeObject *)&DeeNone_Type || lhs == &DeeList_Type) return lhs;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_imul(DeeTypeObject *lhs) {
 if (_dee_type_is_basic_int(lhs)) return _dee_type_min_integral_int(lhs);
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_min_integral_int(_dee_type_remove_atomic(lhs));
 if (_dee_type_is_float(lhs) || lhs == (DeeTypeObject *)&DeeNone_Type || lhs == &DeeList_Type) return lhs;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_binary_op(DeeTypeObject *lhs) {
 if (_dee_type_is_basic_int(lhs) || // and, or, xor
     lhs == (DeeTypeObject *)&DeeNone_Type || lhs == &DeeList_Type) return lhs;
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_remove_atomic(lhs);
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_pow(DeeTypeObject *lhs) {
 if (_dee_type_is_basic_int(lhs)) return lhs;
 if (_dee_type_is_atomic_int(lhs)) return _dee_type_remove_atomic(lhs);
 if (_dee_type_is_float(lhs) || lhs == (DeeTypeObject *)&DeeNone_Type) return lhs;
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_seq_size(DeeTypeObject *a) {
 if (a == &DeeList_Type || a == &DeeTuple_Type || a == &DeeString_Type ||
     a == &DeeDict_Type || a == &DeeDictKeys_Type ||
     a == &DeeDictItems_Type || a == &DeeDictValues_Type
     ) return DeeObject_TYPE(Dee_size_t);
 return NULL;
}
DEE_STATIC_INLINE(DeeTypeObject *) _dee_predict_typeof_seq_contains(DeeTypeObject *lhs) {
 if (lhs == &DeeList_Type || lhs == &DeeTuple_Type || lhs == &DeeString_Type ||
     lhs == &DeeDict_Type || lhs == &DeeDictKeys_Type ||
     lhs == &DeeDictItems_Type || lhs == &DeeDictValues_Type
     ) return (DeeTypeObject *)&DeeBool_Type;
 return NULL;
}


DEE_A_RET_WUNUSED DeeTypeObject *DeeExprAst_PredictType(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 switch (DeeExprAst_KIND(self)) {
  case DeeExprAstKind_VARDECL:
   return DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope);

  case DeeExprAstKind_DECLVARIABLE:
  case DeeExprAstKind_VARIABLE:
   if ((DeeExprAst_VAR(self)->ob_flags&DEE_LOCALVAR_FLAG_TYPEBOUND)!=0)
    return DeeExprAst_VAR(self)->ob_type; // We can easily predict type-bound variables
   break;

  case DeeExprAstKind_CALL:
   if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_CONSTANT) {
    Dee_size_t argc;
    DeeObject *intrin_func = DeeExprAst_OP_A(self)->ob_const;
    DEE_ASSERT(DeeExprAst_OP_B(self)->ob_kind == DeeExprAstKind_TUPLE || (
               DeeExprAst_OP_B(self)->ob_kind == DeeExprAstKind_CONSTANT &&
               DeeTuple_Check(DeeExprAst_OP_B(self)->ob_const)));
    // Constructor call (return instance of their type)
    if (DeeType_Check(intrin_func)) return (DeeTypeObject *)intrin_func;
    argc = DeeTuple_SIZE(DeeExprAst_OP_B(self)->ob_args_a);
    // predict typing of intrinsic functions
    if (intrin_func == (DeeObject *)&DeeBuiltinFunction___type__ && argc == 1) return &DeeType_Type;
    if (intrin_func == (DeeObject *)&DeeBuiltinFunction___repr__ && argc == 1) return &DeeString_Type;
    if ((intrin_func == (DeeObject *)&DeeBuiltinFunction___copy__ ||
        intrin_func == (DeeObject *)&DeeBuiltinFunction___move__) && argc == 1)
     return DeeExprAst_OP_B(self)->ob_kind == DeeExprAstKind_CONSTANT
     ? Dee_TYPE(DeeTuple_GET(DeeExprAst_OP_B(self)->ob_const,0))
     : DeeExprAst_PredictType(DeeTuple_GET(DeeExprAst_OP_B(self)->ob_args_a,0),scope); // copy+move return the same type as their argument
#if 0
    if (DeeMemberFunction_CheckExact(intrin_func)) {
     // TODO: Member function (e.g.: 'str(42).upper()')
    }
#endif
   }
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   return DeeStmtAst_PredictType((DeeObject *)DeeExprAst_STMT(self),scope);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */

  case DeeExprAstKind_CONSTANT: // Constant
   return Dee_TYPE(DeeExprAst_CONST(self));

  case DeeExprAstKind_COND: {
   DeeTypeObject *tt_ast_type,*ff_ast_type;
   tt_ast_type = DeeExprAst_OP_B(self) ? DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope) : (DeeTypeObject *)&DeeNone_Type;
   ff_ast_type = DeeExprAst_OP_C(self) ? DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_C(self),scope) : (DeeTypeObject *)&DeeNone_Type;
   // If both asts have the same type we can safely say this one has the same!
   if (tt_ast_type == ff_ast_type) return tt_ast_type;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF: {
   DeeTypeObject *base_type;
   if ((base_type = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) != NULL)
    if ((base_type = DeeType_Pointer(base_type)) == NULL) DeeError_Handled();
   return base_type;
  }// break;

  case DeeExprAstKind_DEREF: {
   DeeTypeObject *base_type;
   if ((base_type = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) != NULL) {
    if (DeePointerType_Check(base_type)) {
     // Pointer type
     if ((base_type = DeeType_LValue(DeePointerType_BASE(base_type))) == NULL) DeeError_Handled();
    } else if (DeeLValueType_Check(base_type) && DeePointerType_Check(DeeLValueType_BASE(base_type))) {
     // LValue --> Pointer type
     if ((base_type = DeeType_LValue(DeePointerType_BASE(DeeLValueType_BASE(base_type)))) == NULL) DeeError_Handled();
    } else base_type = NULL;
   }
   return base_type;
  }// break
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_NEW: {
   DeeTypeObject *type;
   type = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope);
   if (!type || !DeeStructuredType_Check(type)) return NULL;
   if ((type = DeeStructuredType_Pointer(type)) == NULL) DeeError_Handled();
   return type;
  } break;
#endif


  case DeeExprAstKind_STR:
  case DeeExprAstKind_REPR: return &DeeString_Type;
  case DeeExprAstKind_COPY: 
  case DeeExprAstKind_MOVE: return DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope);
  case DeeExprAstKind_CLASSOF:
  case DeeExprAstKind_TYPE: return &DeeType_Type;
  case DeeExprAstKind_WEAK: return &DeeWeakref_Type;

#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL: return &DeeCell_Type;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND:
#endif
  case DeeExprAstKind_BOOL: return (DeeTypeObject *)&DeeBool_Type;
  case DeeExprAstKind___SUPER:
  case DeeExprAstKind_SUPER: return (DeeTypeObject *)&DeeSuper_Type;
  case DeeExprAstKind_NOT: return _dee_predict_typeof_not(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_INV: return _dee_predict_typeof_inv(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_POS: return _dee_predict_typeof_pos(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_NEG: return _dee_predict_typeof_neg(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_INC: case DeeExprAstKind_INC_POST:
  case DeeExprAstKind_DEC: case DeeExprAstKind_DEC_POST:
   return _dee_predict_typeof_inc_dec_opt_post(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_CMP_LO: case DeeExprAstKind_CMP_LE:
  case DeeExprAstKind_CMP_EQ: case DeeExprAstKind_CMP_NE:
  case DeeExprAstKind_CMP_GR: case DeeExprAstKind_CMP_GE:
   return _dee_predict_typeof_cmp(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_ADD:  return _dee_predict_typeof_add(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_IADD: return _dee_predict_typeof_iadd(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_SUB: case DeeExprAstKind_ISUB: case DeeExprAstKind_IDIV:
  case DeeExprAstKind_SHL: case DeeExprAstKind_ISHL:
  case DeeExprAstKind_SHR: case DeeExprAstKind_ISHR:
   return _dee_predict_typeof_math_def(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_DIV: return _dee_predict_typeof_math_div(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_MUL:  return _dee_predict_typeof_mul(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_IMUL: return _dee_predict_typeof_imul(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_AND: case DeeExprAstKind_IAND:
  case DeeExprAstKind_OR:  case DeeExprAstKind_IOR:
  case DeeExprAstKind_XOR: case DeeExprAstKind_IXOR:
   return _dee_predict_typeof_binary_op(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_POW: case DeeExprAstKind_IPOW:
   return _dee_predict_typeof_pow(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_LAND:
  case DeeExprAstKind_LOR:
  case DeeExprAstKind_LXOR:
   return (DeeTypeObject *)&DeeBool_Type;
  case DeeExprAstKind_SET:
   return &DeeSet_Type;
  case DeeExprAstKind_DICT:
   return &DeeDict_Type;
  case DeeExprAstKind_LIST:
   return &DeeList_Type;
  case DeeExprAstKind_TUPLE:
   return &DeeTuple_Type;
  case DeeExprAstKind_SEQ_SIZE:
   return _dee_predict_typeof_seq_size(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope));
  case DeeExprAstKind_SEQ_SET:
  case DeeExprAstKind_ATTR_SET:
   return DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_C(self),scope);
  case DeeExprAstKind_SEQ_RANGE_SET:
   return DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_D(self),scope);
  case DeeExprAstKind_ATTR_GET:
   // TODO: search tp_members for types
   break;
  case DeeExprAstKind_IS:
   return (DeeTypeObject *)&DeeBool_Type;
  case DeeExprAstKind_IN:
   return _dee_predict_typeof_seq_contains(DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope));
  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_FUNCTION_NONE:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_VFUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_NONE:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
   return &DeeFunction_Type;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DeeExprAstKind_BUILTIN_ALLOCA: {
   DeeTypeObject *temp = DeeType_Pointer(DeeObject_TYPE(none));
   if (!temp) DeeError_Handled(); // Type prediction can't throw an error
   return temp;
  } break;
#endif


#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE || \
    DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_DELETE:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DeeExprAstKind_BUILTIN_BREAKPOINT:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DeeExprAstKind_BUILTIN_UNREACHABLE:
   // technically we could say anything, since type prediction
   // assumes a world without exceptions or noreturn.
   // But what should we return in this case? Maybe DeeObject_Type.
   // This is that divide-by-zero situation all over again...
   //
   // But I decided for none, since I always used none to satisfy
   // the code generated before I added noreturn optimizations.
#endif
   return (DeeTypeObject *)&DeeNone_Type;
#endif

  default: break;
 }
 return NULL;
}

DEE_STATIC_INLINE(int) _deenosideeffects_basic_common(DeeTypeObject *a) {
 return 
  _dee_type_is_float(a) || _dee_type_is_int(a) ||
  a == (DeeTypeObject *)&DeeBool_Type || a == &DeeString_Type ||
  a == (DeeTypeObject *)&DeeNone_Type || a == &DeeList_Type ||
  a == &DeeTuple_Type || a == &DeeDict_Type ||
  a == &DeeDictKeys_Type || a == &DeeDictItems_Type ||
  a == &DeeDictValues_Type || a == &DeeObject_Type ||
  a == &DeeMemberFunction_Type || a == &DeeCFunction_Type ||
  a == &DeeRangeInt8Iterator_Type || a == &DeeRangeInt16Iterator_Type ||
  a == &DeeRangeInt32Iterator_Type || a == &DeeRangeInt64Iterator_Type ||
  a == &DeeRangeUInt8Iterator_Type || a == &DeeRangeUInt16Iterator_Type ||
  a == &DeeRangeUInt32Iterator_Type || a == &DeeRangeUInt64Iterator_Type ||
  a == &DeeFSDir_Type || a == &DeeFSQuery_Type || a == &DeeRandom_Type
 ;
}

#define _deenosideeffects_move _deenosideeffects_copy
DEE_STATIC_INLINE(int) _deenosideeffects_copy(DeeTypeObject *a) {
 return _deenosideeffects_basic_common(a);
}
#define _deenosideeffects_repr _deenosideeffects_str
DEE_STATIC_INLINE(int) _deenosideeffects_str(DeeTypeObject *a) {
 if (_deenosideeffects_basic_common(a)
     ) return 1;
 return 0;
}


#define _deenosideeffects_bool _deenosideeffects_not
DEE_STATIC_INLINE(int) _deenosideeffects_not(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_int(a) ||
     a == (DeeTypeObject *)&DeeBool_Type || a == &DeeString_Type ||
     a == (DeeTypeObject *)&DeeNone_Type || a == &DeeList_Type ||
     a == &DeeTuple_Type || a == &DeeDict_Type ||
     a == &DeeDictKeys_Type || a == &DeeDictItems_Type ||
     a == &DeeDictValues_Type || a == &DeeObject_Type
     ) return 1;
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_pos(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_int(a) ||
     a == (DeeTypeObject *)&DeeNone_Type
     ) return 1;
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_neg(DeeTypeObject *a) {
 if (_dee_type_is_float(a) || _dee_type_is_signed(a) ||
     a == (DeeTypeObject *)&DeeNone_Type
     ) return 1;
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_inv(DeeTypeObject *a) {
 if (_dee_type_is_basic_int(a) || _dee_type_is_atomic_int(a) ||
     a == (DeeTypeObject *)&DeeBool_Type ||
     a == (DeeTypeObject *)&DeeNone_Type) return 1;
 return 0;
}
DEE_STATIC_INLINE(int) _dee_is_integral(DeeTypeObject *a) {
 return _dee_type_is_float(a) || _dee_type_is_int(a);
}
DEE_STATIC_INLINE(int) _dee_is_nosideeffect_iterable(DeeTypeObject *a) {
 return 
  a == (DeeTypeObject *)&DeeNone_Type ||
  a == &DeeTuple_Type ||
  a == &DeeList_Type ||
  a == &DeeDict_Type ||
  a == &DeeDictKeys_Type ||
  a == &DeeDictItems_Type ||
  a == &DeeDictValues_Type ||
  a == &DeeString_Type ||
  a == &DeeRangeInt8Iterator_Type ||
  a == &DeeRangeInt16Iterator_Type ||
  a == &DeeRangeInt32Iterator_Type ||
  a == &DeeRangeInt64Iterator_Type ||
  a == &DeeRangeUInt8Iterator_Type ||
  a == &DeeRangeUInt16Iterator_Type ||
  a == &DeeRangeUInt32Iterator_Type ||
  a == &DeeRangeUInt64Iterator_Type
  ;
}

DEE_STATIC_INLINE(int) _deenosideeffects_cmp(DeeTypeObject *lhs, DeeTypeObject *rhs) {
 if (_dee_is_integral(lhs)) return _dee_is_integral(rhs); // Integral on integral doesn't have any side-effects
 if (lhs == (DeeTypeObject *)&DeeBool_Type)  return _deenosideeffects_bool(rhs); // strcmp
 if (lhs == &DeeString_Type)  return rhs == &DeeString_Type; // strcmp
 if (lhs == (DeeTypeObject *)&DeeNone_Type)  return 1; // Always!
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_add(DeeTypeObject *lhs, DeeTypeObject *rhs) {
 if (_dee_is_integral(lhs)) return _dee_is_integral(rhs); // Integral on integral doesn't have any side-effects
 if (lhs == &DeeString_Type) return rhs == &DeeString_Type; // string concat
 if (lhs == &DeeList_Type)  return _dee_is_nosideeffect_iterable(rhs); // list concat
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_mul(DeeTypeObject *lhs, DeeTypeObject *rhs) {
 if (_dee_is_integral(lhs)) return _dee_is_integral(rhs);
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return 1;
 if (lhs == &DeeString_Type || lhs == &DeeList_Type) return _dee_is_integral(lhs); // Sequence repeat
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_seq_size(DeeTypeObject *a) {
 if (a == (DeeTypeObject *)&DeeNone_Type) return 1;
 if (a == &DeeList_Type || a == &DeeTuple_Type || a == &DeeString_Type ||
     a == &DeeDict_Type || a == &DeeDictKeys_Type ||
     a == &DeeDictItems_Type || a == &DeeDictValues_Type
     ) return 1;
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_seq_get(DeeTypeObject *lhs, DeeTypeObject *rhs) {
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return 1;
 if (lhs == &DeeList_Type || lhs == &DeeTuple_Type || lhs == &DeeString_Type ||
     lhs == &DeeDict_Type || lhs == &DeeDictKeys_Type ||
     lhs == &DeeDictItems_Type || lhs == &DeeDictValues_Type
     ) return (rhs == (DeeTypeObject *)&DeeNone_Type) || _dee_is_integral(rhs);
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_seq_get_range(
 DeeTypeObject *lhs, DeeTypeObject *rhs1, DeeTypeObject *rhs2) {
 if (lhs == (DeeTypeObject *)&DeeNone_Type) return 1;
 if (lhs == &DeeList_Type || lhs == &DeeTuple_Type || lhs == &DeeString_Type ||
     lhs == &DeeDict_Type || lhs == &DeeDictKeys_Type ||
     lhs == &DeeDictItems_Type || lhs == &DeeDictValues_Type
     ) return ((rhs1 == (DeeTypeObject *)&DeeNone_Type) || _dee_is_integral(rhs1)) &&
              ((rhs2 == (DeeTypeObject *)&DeeNone_Type) || _dee_is_integral(rhs2));
 return 0;
}
DEE_STATIC_INLINE(int) _deenosideeffects_getattr(
 DeeTypeObject *ty, DeeStringObject *attr) {
 char const *attr_name;
 if (ty == (DeeTypeObject *)&DeeNone_Type) return 1; // Always!
 attr_name = DeeString_STR(attr);
 do {
  if (DeeType_HAS_SLOT(ty,tp_attr_get)) return 0;
  {struct DeeMemberDef const *iter;
   iter = DeeType_GET_SLOT(ty,tp_members);
   while (iter->d_name) {
    if ((iter->d_access & DeeMemberAccess_READ) != 0 &&
        strcmp(iter->d_name,attr_name) == 0) return 1;
    ++iter;
   }
  }
  {struct DeeMethodDef const *iter;
   iter = DeeType_GET_SLOT(ty,tp_methods);
   while (iter->d_name) {
    if (strcmp(iter->d_name,attr_name) == 0) return 1;
    ++iter;
   }
  }
 } while ((ty = DeeType_BASE(ty)) != NULL);
 return 0;
}

DEE_A_RET_WUNUSED int DeeExprAst_IsNoeffect(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
 DeeTypeObject *tya,*tyb,*tyc;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 // Enable optimizations for best results
 switch (DeeExprAst_KIND(self)) {
  case DeeExprAstKind_MODULE:
  case DeeExprAstKind_DECLVARIABLE:
  case DeeExprAstKind_VARIABLE:
   // Technically this can have the side effect of an unbound local error.
   // But lets just pretend that can never happen...
   // Because similar to out-of-memory, this isn't
   // something you should want to have happening.
  case DeeExprAstKind_CONSTANT:
  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_FUNCTION_NONE:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_VFUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_NONE:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DeeExprAstKind_BUILTIN_UNREACHABLE:
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DeeExprAstKind_CLASS:
#endif
  // Dex must be noeffect, so unused dex variables can be optimized away!
  case DeeExprAstKind_DEX:
   return 1;

#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   return DeeStmtAst_IsNoeffect((DeeObject *)DeeExprAst_STMT(self),scope);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */

  case DeeExprAstKind_COND:
   return DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) &&
          DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope) &&
          (!DeeExprAst_OP_C(self) || DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_C(self),scope));

  case DeeExprAstKind_TYPE:
  case DeeExprAstKind_CLASSOF:
   // If we can predict the type, and the exception doesn't
   // have any side-effects, then the typeof doesn't either!
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope)) return 0;
   return DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope) != NULL;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_NEW: {
   DeeTypeObject *inst_type;
   inst_type = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope);
   if (inst_type && DeeStructuredType_Check(inst_type) &&
       DeeType_GET_SLOT(inst_type,tp_p_instance_size) == 0
       ) return 1; // 'new', where the type is empty has no side-effects
  } break;

  case DeeExprAstKind_DELETE: {
   if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_CONSTANT) {
    void *expr_ptr;
    if (DeeObject_GetVoidPointer(DeeExprAst_OP_A(self)->ob_const,&expr_ptr) != 0) DeeError_Handled();
    return expr_ptr == NULL; // 'delete' with NULL argument has no side-effects
   }
  } break;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND: return 1;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL:
   return DeeExprAst_OP_A(self) ? DeeExprAst_IsNoeffect(
    (DeeObject *)DeeExprAst_OP_A(self),scope) : 1;
#endif
  case DeeExprAstKind_BOOL:
  case DeeExprAstKind_NOT:
  case DeeExprAstKind_NEG:
  case DeeExprAstKind_POS:
  case DeeExprAstKind_INV:
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF:
  case DeeExprAstKind_DEREF:
#endif
  case DeeExprAstKind_INC:
  case DeeExprAstKind_DEC:
  case DeeExprAstKind_INC_POST:
  case DeeExprAstKind_DEC_POST:
  case DeeExprAstKind_SEQ_SIZE:
  case DeeExprAstKind_STR:
  case DeeExprAstKind_REPR:
  case DeeExprAstKind_COPY:
  case DeeExprAstKind_MOVE:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   switch (DeeExprAst_KIND(self)) {
    case DeeExprAstKind_STR:  return _deenosideeffects_str(tya);
    case DeeExprAstKind_REPR: return _deenosideeffects_repr(tya);
    case DeeExprAstKind_COPY: return _deenosideeffects_copy(tya);
    case DeeExprAstKind_MOVE: return _deenosideeffects_move(tya);
    case DeeExprAstKind_BOOL: return _deenosideeffects_bool(tya);
    case DeeExprAstKind_NOT: return _deenosideeffects_not(tya);
    case DeeExprAstKind_NEG: return _deenosideeffects_neg(tya);
    case DeeExprAstKind_POS: return _deenosideeffects_pos(tya);
    case DeeExprAstKind_INV: return _deenosideeffects_inv(tya);
    case DeeExprAstKind_SEQ_SIZE: return _deenosideeffects_seq_size(tya);
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
    case DeeExprAstKind_REF:
     // if it isn't a structured type, this throws a runtime exception
     return !DeeStructuredType_Check(tya);
    case DeeExprAstKind_DEREF:
     // if it isn't a pointer/lvalue to pointer type, this throws a runtime exception
     if (!DeePointerType_Check(tya)) {
      if (DeeLValueType_Check(tya))
       return !DeePointerType_Check(DeeLValueType_BASE(tya));
      return 1;
     }
     return 0;
#endif
    case DeeExprAstKind_INC:
    case DeeExprAstKind_DEC:
    case DeeExprAstKind_INC_POST:
    case DeeExprAstKind_DEC_POST:
     return tya != (DeeTypeObject *)&DeeNone_Type;

    default: break;
   }
   DEE_BUILTIN_UNREACHABLE();

  case DeeExprAstKind_IS:
   return !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) &&
          !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope);
  case DeeExprAstKind_IN:
  case DeeExprAstKind_ASSIGN:
  case DeeExprAstKind_IADD:
  case DeeExprAstKind_ISUB:
  case DeeExprAstKind_IMUL:
  case DeeExprAstKind_IDIV:
  case DeeExprAstKind_IMOD:
  case DeeExprAstKind_ISHL:
  case DeeExprAstKind_ISHR:
  case DeeExprAstKind_IAND:
  case DeeExprAstKind_IOR:
  case DeeExprAstKind_IXOR:
  case DeeExprAstKind_IPOW:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) ||
       !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   return tya == (DeeTypeObject *)&DeeNone_Type;

  case DeeExprAstKind_CMP_LO:
  case DeeExprAstKind_CMP_LE:
  case DeeExprAstKind_CMP_EQ:
  case DeeExprAstKind_CMP_NE:
  case DeeExprAstKind_CMP_GR:
  case DeeExprAstKind_CMP_GE:
  case DeeExprAstKind_ADD:
  case DeeExprAstKind_SUB:
  case DeeExprAstKind_MUL:
  case DeeExprAstKind_DIV:
  case DeeExprAstKind_MOD:
  case DeeExprAstKind_SHL:
  case DeeExprAstKind_SHR:
  case DeeExprAstKind_AND:
  case DeeExprAstKind_OR:
  case DeeExprAstKind_XOR:
  case DeeExprAstKind_POW:
  case DeeExprAstKind_LAND:
  case DeeExprAstKind_LOR:
  case DeeExprAstKind_LXOR:
  case DeeExprAstKind_SEQ_GET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) ||
       !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   if ((tyb = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope)) == NULL) return 0;
   switch (DeeExprAst_KIND(self)) {
    case DeeExprAstKind_LAND:
    case DeeExprAstKind_LOR:
    case DeeExprAstKind_LXOR:
     return _deenosideeffects_bool(tya) && _deenosideeffects_bool(tyb);
    case DeeExprAstKind_CMP_LO:
    case DeeExprAstKind_CMP_LE:
    case DeeExprAstKind_CMP_EQ:
    case DeeExprAstKind_CMP_NE:
    case DeeExprAstKind_CMP_GR:
    case DeeExprAstKind_CMP_GE:
     return _deenosideeffects_cmp(tya,tyb);
    case DeeExprAstKind_ADD:
     return _deenosideeffects_add(tya,tyb);
    case DeeExprAstKind_MUL:
     return _deenosideeffects_mul(tya,tyb);
    case DeeExprAstKind_SUB:
    case DeeExprAstKind_DIV:
    case DeeExprAstKind_MOD:
    case DeeExprAstKind_SHL:
    case DeeExprAstKind_SHR:
    case DeeExprAstKind_POW:
     return (tya == (DeeTypeObject *)&DeeNone_Type) ||
      (_dee_is_integral(tya) && _dee_is_integral(tyb));
    case DeeExprAstKind_AND:
    case DeeExprAstKind_OR:
    case DeeExprAstKind_XOR:
     return (tya == (DeeTypeObject *)&DeeNone_Type) ||
      ((_dee_is_integral(tya) || tya == (DeeTypeObject *)&DeeBool_Type) &&
       (_dee_is_integral(tyb) || tyb == (DeeTypeObject *)&DeeBool_Type));
    case DeeExprAstKind_SEQ_GET:
     return _deenosideeffects_seq_get(tya,tyb);
    default: break;
   }
   DEE_BUILTIN_UNREACHABLE();

  case DeeExprAstKind_SET:
  case DeeExprAstKind_DICT:
  case DeeExprAstKind_LIST:
  case DeeExprAstKind_TUPLE: {
   DeeExprAstObject **iter,**end;
   end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(
    DeeExprAst_ARGS_A(self)))+DeeTuple_SIZE(DeeExprAst_ARGS_A(self));
   while (iter != end) if (!DeeExprAst_IsNoeffect((DeeObject *)*iter++,scope)) return 0;
   return 1;
  } break;

  case DeeExprAstKind_CALL:
   // TODO: Call to constant type continue with nosideeffect arguments
   break;

  case DeeExprAstKind_SEQ_SET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) ||
       !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope) ||
       !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_C(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   if ((tyb = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope)) == NULL) return 0;
   if ((tyc = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_C(self),scope)) == NULL) return 0;
   return (tya == (DeeTypeObject *)&DeeNone_Type);

  case DeeExprAstKind_SEQ_RANGE_GET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   if (DeeExprAst_OP_B(self)) {
    if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope)) return 0;
    if ((tyb = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope)) == NULL) return 0;
   } else tyb = (DeeTypeObject *)&DeeNone_Type;
   if (DeeExprAst_OP_C(self)) {
    if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_C(self),scope)) return 0;
    if ((tyc = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_C(self),scope)) == NULL) return 0;
   } else tyc = (DeeTypeObject *)&DeeNone_Type;
   return _deenosideeffects_seq_get_range(tya,tyb,tyc);

  case DeeExprAstKind_SEQ_RANGE_SET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope) ||
       (DeeExprAst_OP_B(self) && !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_B(self),scope)) ||
       (DeeExprAst_OP_C(self) && !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_C(self),scope)) ||
       !DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_D(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   return (tya == (DeeTypeObject *)&DeeNone_Type);

  case DeeExprAstKind_ATTR_GET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   return _deenosideeffects_getattr(tya,DeeExprAst_MEMBER(self));

  case DeeExprAstKind_ATTR_SET:
   if (!DeeExprAst_IsNoeffect((DeeObject *)DeeExprAst_OP_A(self),scope)) return 0;
   if ((tya = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) == NULL) return 0;
   return (tya == (DeeTypeObject *)&DeeNone_Type);

  default: break;
 }
 return 0;
}






#define ret_used   ((flags&DEE_COMPILER_FLAG_USED)!=0)
int _DeeExprAst_CompileCall(
 DeeExprAstObject *self, struct DeeScopeObject *scope,
 struct DeeCodeWriter *writer, struct DeeLexerObject *lexer, Dee_uint32_t flags) {
 DeeExprAstObject *func_ast,*args_ast; Dee_uint8_t op; Dee_uint16_t op_ext;
 DEE_ASSERT(self->ob_kind == DeeExprAstKind_CALL);
 func_ast = self->ob_op_a,args_ast = self->ob_op_b;
 DEE_ASSERT(args_ast->ob_kind == DeeExprAstKind_TUPLE || (
  args_ast->ob_kind == DeeExprAstKind_CONSTANT &&
  DeeTuple_Check(args_ast->ob_const)));
#define compile_arg(i) (args_ast->ob_kind==DeeExprAstKind_TUPLE\
 ?DeeExprAst_Compile(DeeTuple_GET(args_ast->ob_args_a,(i)),(DeeObject *)scope,writer,(DeeObject *)lexer,flags|DEE_COMPILER_FLAG_USED)\
 :DeeCodeWriter_PushConst(writer,DeeTuple_GET(args_ast->ob_const,(i))))
#define compile_arg_unused(i) (args_ast->ob_kind==DeeExprAstKind_TUPLE\
 ?DeeExprAst_Compile(DeeTuple_GET(args_ast->ob_args_a,(i)),(DeeObject *)scope,writer,(DeeObject *)lexer,flags&~DEE_COMPILER_FLAG_USED):0)
#define predict_arg_type(i) (args_ast->ob_kind==DeeExprAstKind_TUPLE\
 ?DeeExprAst_PredictType(DeeTuple_GET(args_ast->ob_args_a,(i)),(DeeObject *)scope)\
 :Dee_TYPE(DeeTuple_GET(args_ast->ob_const,(i))))
#define is_constant(i) (args_ast->ob_kind==DeeExprAstKind_TUPLE\
 ?DeeExprAst_KIND(DeeTuple_GET(args_ast->ob_args_a,(i)))==DeeExprAstKind_CONSTANT:1)
#define constant_arg(i) (args_ast->ob_kind==DeeExprAstKind_TUPLE\
 ?DeeExprAst_CONST(DeeTuple_GET(args_ast->ob_args_a,(i)))\
 :DeeTuple_GET(args_ast->ob_const,(i)))

 if (func_ast->ob_kind == DeeExprAstKind_CONSTANT &&
    (flags&DEE_COMPILE_FLAG_INTRINSIC_FUNCTIONS)!=0) {

  // Check if we know this function as something builtin
  // NOTE: We don't have to differentiate between the arguments being constant,
  //       because 'args_ast->ob_args_a' and 'args_ast->ob_const' have the same
  //       memory address and would both be tuple objects.
  Dee_size_t argc = DeeTuple_SIZE(args_ast->ob_args_a);
  DeeObject *func = func_ast->ob_const;
  if (func == (DeeObject *)&DeeBool_Type && argc == 1) {
   // bool(<expr>) --> OPEXT_BOOL_USER
   if ((flags&DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY)==
       DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY &&
       predict_arg_type(0) == (DeeTypeObject *)&DeeBool_Type
       ) goto unary_intrin_direct_arg; // Already a bool (this call is unnecessary)
   if (!ret_used) {
    if (args_ast->ob_kind==DeeExprAstKind_TUPLE) {
     // Compile unused argument (and ignore the result)
     return compile_arg_unused(0);
    } else return 0;
   }
   if (compile_arg(0) == -1 || DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BOOL_USER)) return -1;
  } else if (func == (DeeObject *)&DeeString_Type && argc == 0) {
   // string() --> OP_PUSH: empty_string
   if (ret_used) return DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString);
  } else if (func == (DeeObject *)&DeeString_Type && argc == 1) {
   // string(<expr>) --> OP_STR
   if ((flags&DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY)==DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY &&
       predict_arg_type(0) == &DeeString_Type) {
unary_intrin_direct_arg:
    if (!ret_used) goto unary_intrin_unused_arg; // Already a string (this call is unnecessary)
    return compile_arg(0);
   }
#if 0
   goto def;
#else
   op = OP_STR;
#endif
unary_intrin_maybe_call:
   if (!ret_used) {
unary_intrin_unused_arg:
    if (args_ast->ob_kind==DeeExprAstKind_TUPLE) {
     // Compile unused argument (and ignore the result)
     return compile_arg_unused(0);
    } else return 0;
   }
   if (compile_arg(0) == -1 || DeeCodeWriter_UnaryOp(writer,op)) return -1;
  } else if (func == (DeeObject *)&DeeBuiltinFunction___builtin_code && argc == 0) {
   // __builtin_code() --> OP_EXTENDED:OPEXT_CODE_SELF
   op_ext = OPEXT_CODE_SELF;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
zero_intrin_maybe_call_ext:
#endif
   if (ret_used) {
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,op_ext) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer);
   }
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  } else if (func == (DeeObject *)&DeeBuiltinFunction___builtin_last_exception && argc == 0) {
   // __builtin_last_exception() --> OP_EXTENDED:OPEXT_LOAD_LAST_EXCEPTION
   op_ext = OPEXT_LOAD_LAST_EXCEPTION; goto zero_intrin_maybe_call_ext;
  } else if (func == (DeeObject *)&DeeBuiltinFunction___builtin_last_traceback && argc == 0) {
   // __builtin_last_traceback() --> OP_EXTENDED:OPEXT_LOAD_LAST_TRACEBACK
   op_ext = OPEXT_LOAD_LAST_TRACEBACK; goto zero_intrin_maybe_call_ext;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  } else if (func == (DeeObject *)&DeeBuiltinFunction___getattr__ && argc == 2) {
   // __builtin_get_attr(<expr>,<expr>) --> OP_ATTR_GET / OP_ATTR_GET_C
   if (is_constant(1) && (flags&DEE_COMPILE_FLAG_INTRINSIC_PARTIAL)
       ==DEE_COMPILE_FLAG_INTRINSIC_PARTIAL) {
    DeeObject *attr_name = constant_arg(1);
    Dee_uint16_t attr_slot;
    if (!DeeString_Check(attr_name)) goto getattr_def;
    if ((attr_slot = DeeCodeWriter_AllocConst(writer,attr_name)) == (Dee_uint16_t)-1) return -1;
    if (compile_arg(0) == -1 || DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_GET_C,attr_slot) == -1) return -1;
   } else {
getattr_def:
    if (compile_arg(0) == -1 || compile_arg(1) == -1 ||
        DeeCodeWriter_BinaryOp(writer,OP_ATTR_GET) == -1
        ) return -1;
   }
   if (!ret_used && DeeCodeWriter_BinaryOp(writer,OP_POP) == -1) return -1;
  } else if (func == (DeeObject *)&DeeBuiltinFunction___delattr__ && argc == 2) {
   // __builtin_del_attr(<expr>,<expr>) --> OP_ATTR_DEL / OP_ATTR_DEL_C
   if (is_constant(1) && (flags&DEE_COMPILE_FLAG_INTRINSIC_PARTIAL)
       ==DEE_COMPILE_FLAG_INTRINSIC_PARTIAL) {
    DeeObject *attr_name = constant_arg(1);
    Dee_uint16_t attr_slot;
    if (!DeeString_Check(attr_name)) goto delattr_def;
    if ((attr_slot = DeeCodeWriter_AllocConst(writer,attr_name)) == (Dee_uint16_t)-1) return -1;
    if (compile_arg(0) == -1 || DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_DEL_C,attr_slot) == -1) return -1;
   } else {
delattr_def:
    if (compile_arg(0) == -1 || compile_arg(1) == -1 ||
        DeeCodeWriter_WriteOp(writer,OP_ATTR_DEL) == -1
        ) return -1;
   }
   if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
  } else if (func == (DeeObject *)&DeeBuiltinFunction___setattr__ && argc == 3) {
   // __builtin_set_attr(<expr>,<expr>) --> OP_ATTR_DEL
   if (is_constant(1) && (flags&DEE_COMPILE_FLAG_INTRINSIC_PARTIAL)
       ==DEE_COMPILE_FLAG_INTRINSIC_PARTIAL) {
    DeeObject *attr_name = constant_arg(1);
    Dee_uint16_t attr_slot;
    if (!DeeString_Check(attr_name)) goto setattr_def;
    if ((attr_slot = DeeCodeWriter_AllocConst(writer,attr_name)) == (Dee_uint16_t)-1) return -1;
    if (compile_arg(0) == -1 || compile_arg(2) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_SET_C,attr_slot) == -1) return -1;
   } else {
setattr_def:
    if (compile_arg(0) == -1 || compile_arg(1) == -1 || compile_arg(2) == -1 ||
        DeeCodeWriter_WriteOp(writer,OP_ATTR_SET) == -1
        ) return -1;
   }
   if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
  } else if (func == (DeeObject *)&DeeBuiltinFunction___repr__ && argc == 1) {
   op = OP_REPR; goto unary_intrin_maybe_call; // repr(<expr>) --> OP_REPR
  } else if (func == (DeeObject *)&DeeBuiltinFunction___copy__ && argc == 1) {
   // TODO: We can optimize this, by adding another ast analyzer to check if
   //       the result is already a copy of a builtin type
   op = OP_COPY; goto unary_intrin_maybe_call; // copy(<expr>) --> OP_COPY
  } else if (func == (DeeObject *)&DeeBuiltinFunction___move__ && argc == 1) {
   op = OP_MOVE; goto unary_intrin_maybe_call; // move(<expr>) --> OP_MOVE
  } else if (func == (DeeObject *)&DeeBuiltinFunction___type__ && argc == 1) {
   if ((flags&DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY)==DEE_COMPILE_FLAG_INTRINSIC_UNNECESSARY) { // type(<any_type>) --> push(type)
    DeeTypeObject *arg_type = predict_arg_type(0);
    if (arg_type) {
     // >> If type() is called and its argument is a type, we always return 'DeeType_Type',
     //    since we can be certain that that's correct
     // Compile the argument as stub and push 'DeeType_Type'
     if (compile_arg_unused(0) == -1) return -1;
     return ret_used ? DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)arg_type) : 0;
    } else goto type_def;
   } else {
    // type(<expr>) --> OP_TYPE
type_def:
    op = OP_TYPE;
    goto unary_intrin_maybe_call;
   }
  } else if (func == (DeeObject *)&DeeWeakref_Type && argc == 1) {
   op = OP_WEAK; goto unary_intrin_maybe_call; // weak(<expr>) --> OP_WEAK
  } else if (func == Dee_None) { // none(...) --> OP_LOAD_NONE
   // Stub-compile the arguments to none (drop their values) before pushing none if it was used
   if (DeeExprAst_Compile((DeeObject *)args_ast,(DeeObject *)scope,writer,(DeeObject *)lexer,flags&~DEE_COMPILER_FLAG_USED) == -1) return -1;
   if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
  } else
   goto def; // Use the fallback version
 } else {
def: // Fallback: compile as regular call expression
  // Check if we can compile it as a member call
  if (func_ast->ob_kind == DeeExprAstKind_ATTR_GET) {
   // Compile as member call
   Dee_uint16_t member_slot = DeeCodeWriter_AllocConst(writer,(DeeObject *)func_ast->ob_member);
   if (member_slot == (Dee_uint16_t)-1) return -1;
   if (DeeExprAst_Compile((DeeObject *)func_ast->ob_op_a,(DeeObject *)scope,writer,(DeeObject *)lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)args_ast,(DeeObject *)scope,writer,(DeeObject *)lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_WriteOpWithArg(writer,OP_CALL_MEMBER_C,member_slot) == -1) return -1;
   DeeCodeWriter_DECSTACK(writer); // OP_CALL_MEMBER_C consumes one stack entry
  } else {
   DeeTypeObject *predicted_func_type;
   predicted_func_type = (flags&DEE_COMPILER_FLAG_OPTIMIZE_CONSTRUCTOR_CALLS)!=0
    ? DeeExprAst_PredictType((DeeObject *)func_ast,(DeeObject *)scope) : NULL;
   if (DeeExprAst_Compile((DeeObject *)func_ast,(DeeObject *)scope,writer,(DeeObject *)lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)args_ast,(DeeObject *)scope,writer,(DeeObject *)lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_BinaryOp(writer,(Dee_uint8_t)(
       predicted_func_type && DeeType_IsSameOrDerived(predicted_func_type,&DeeType_Type)
       // Use the new_instace opcode for types (speeds up constructor calls)
       ? OP_NEW_INSTANCE : OP_CALL)) == -1) return -1;
  }
  // Pop the return value if it isn't used
  if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
 }
#undef constant_arg
#undef is_constant
#undef predict_arg_type
#undef compile_arg_unused
#undef compile_arg
 return 0;
}


static int _dee_exprast_compile_and_unpack(
 DeeExprAstObject *self, DeeScopeObject *scope,
 struct DeeCodeWriter *writer, DeeLexerObject *lexer,
 Dee_uint32_t flags, Dee_uint16_t n_output) {
 DeeTypeObject *expand_type;
 if (!n_output) flags &= ~DEE_COMPILER_FLAG_USED;
 if (DeeExprAst_Compile((DeeObject *)self,(DeeObject *)scope,writer,(DeeObject *)lexer,flags) == -1) return -1;
 if (ret_used) {
  expand_type = DeeExprAst_PredictType((DeeObject *)self,(DeeObject *)scope);
  if (DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint8_t)(expand_type
   ? (expand_type == &DeeTuple_Type ? OP_UNPACK_TUPLE
   : (expand_type == &DeeList_Type ? OP_UNPACK_LIST : OP_UNPACK_SEQUENCE))
   : OP_UNPACK_SEQUENCE),n_output) == -1) return -1;
  DeeCodeWriter_INCSTACK_N(writer,n_output-1);
 }
 return 0;
}


#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static int _DeeExprAst_CompileClassElements(
 struct DeeClassDeclObject const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCodeWriter *writer,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer, DEE_A_IN Dee_uint32_t flags) {
 struct DeeClassDeclEntry *entry_iter,*entry_end; Dee_uint16_t name_id;
 entry_end = (entry_iter = self->ob_entryv)+self->ob_entryc;
 while (entry_iter != entry_end) {
  switch (entry_iter->ob_kind) {
   case DeeClassDeclEntryKind_SLOT:
    if (DeeExprAst_Compile((DeeObject *)entry_iter->ob_slot.ob_callback,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,OP_CLASS_SET_SLOT,(Dee_uint16_t)entry_iter->ob_slot.ob_slotid) == -1
        ) return -1;
    DeeCodeWriter_DECSTACK(writer);
    break;
   case DeeClassDeclEntryKind_METHOD:
   case DeeClassDeclEntryKind_CLASS_METHOD:
    if ((name_id = DeeCodeWriter_AllocConst(writer,(DeeObject *)entry_iter->ob_method.ob_name)) == (Dee_uint16_t)-1) return -1;
    if (DeeExprAst_Compile((DeeObject *)entry_iter->ob_method.ob_callback,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint8_t)(
        entry_iter->ob_kind == DeeClassDeclEntryKind_METHOD
        ? OP_CLASS_ADD_METHOD : OP_CLASS_ADD_CLASS_METHOD),name_id) == -1
        ) return -1;
    DeeCodeWriter_DECSTACK(writer);
    break;
   case DeeClassDeclEntryKind_GETSET:
   case DeeClassDeclEntryKind_CLASS_GETSET:
    if ((name_id = DeeCodeWriter_AllocConst(writer,(DeeObject *)entry_iter->ob_getset.ob_name)) == (Dee_uint16_t)-1) return -1;
    if ((entry_iter->ob_getset.ob_get
     ? DeeExprAst_Compile((DeeObject *)entry_iter->ob_getset.ob_get,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)
     : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString)) == -1 || (entry_iter->ob_getset.ob_del
     ? DeeExprAst_Compile((DeeObject *)entry_iter->ob_getset.ob_del,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)
     : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString)) == -1 || (entry_iter->ob_getset.ob_set
     ? DeeExprAst_Compile((DeeObject *)entry_iter->ob_getset.ob_set,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)
     : DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyString)) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,(Dee_uint8_t)(
        entry_iter->ob_kind == DeeClassDeclEntryKind_GETSET
        ? OP_CLASS_ADD_GETSET : OP_CLASS_ADD_CLASS_GETSET),name_id) == -1
        ) return -1;
    DeeCodeWriter_DECSTACK_N(writer,3);
    break;
   default: break; // shouldn't happen
  }
  ++entry_iter;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
#endif

#ifdef DEE_SCOPE_FLAG_USING_LOCALS
#define have_local_names \
 ((flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0||\
 (((DeeScopeObject *)scope)->ob_flags&DEE_SCOPE_FLAG_USING_LOCALS)!=0)
#else
#define have_local_names \
 ((flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0)
#endif

DEE_A_RET_EXCEPT(-1) int DeeExprAst_Compile(
 DEE_A_IN_OBJECT(DeeExprAstObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCodeWriter *writer,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN Dee_uint32_t flags) {
 DeeLocalVarObject *var;
 Dee_size_t pos1,pos2,future_arg_1,future_arg_2; Dee_uint8_t op,op_loc,op_arg,op_cst; Dee_uint16_t op_retval;
 Dee_uint16_t slot;
 DeeTupleObject *expr_list;
 DEE_ASSERT(DeeObject_Check(self) && DeeExprAst_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(writer);
 switch (DeeExprAst_KIND(self)) {
  case DeeExprAstKind_DECLVARIABLE:
  case DeeExprAstKind_VARIABLE:
   if (!ret_used) return 0; // Nothing to do here...
   // Make sure the variable is initialized
   if ((DeeExprAst_VAR(self)->ob_flags & DEE_LOCALVAR_FLAG_INITIALIZED) == 0) {
    if (DeeError_CompilerError(DEE_WARNING_UNINITIALIZED_VARIABLE_IN_LOAD,
     lexer,(DeeObject *)DeeExprAst_TOKEN(self),"Cannot load uninitialized variable") == -1) return -1;
    return DeeCodeWriter_PushNone(writer);
   }
   // Push a variable
   return DeeCodeWriter_PushVar(writer,scope,(DeeObject *)DeeExprAst_VAR(self));
   //break;

  case DeeExprAstKind_DEX: if (ret_used) {
   DeeObject *dex_args; int temp;
   if (DeeCodeWriter_PushConst(writer,(DeeObject *)&DeeBuiltinFunction___builtin_dex) != 0) return -1;
   if ((dex_args = DeeTuple_Pack(1,DeeExprAst_DEXNAME(self))) == NULL) return -1;
   temp = DeeCodeWriter_PushConst(writer,dex_args);
   Dee_DECREF(dex_args);
   if (temp != 0) return temp;
   if (DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
  } break;

  case DeeExprAstKind_BUILTIN_EXPECT:
   // No optimizations in generic context
   return DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags);

  case DeeExprAstKind_EXPAND:
   if (ret_used) {
    // Expand to a single output
    return _dee_exprast_compile_and_unpack(
     DeeExprAst_OP_A(self),(DeeScopeObject *)scope,writer,(DeeLexerObject *)lexer,flags,1);
   } else {
    DeeTypeObject *expand_type;
    expand_type = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),(DeeObject *)scope);
    if (expand_type && (
     expand_type == &DeeTuple_Type ||
     expand_type == &DeeList_Type ||
     expand_type == &DeeDict_Type ||
     expand_type == &DeeString_Type ||
     expand_type == &DeeTraceback_Type
     )) {
     // Don't iterate sequence without side-effects
     if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),
      scope,writer,lexer,flags&~DEE_COMPILER_FLAG_USED) == -1) return -1;
    } else {
     // Iterate a sequence
     if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),
      scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
     if (DeeCodeWriter_WriteOp(writer,OP_ITERATE_SEQUENCE) == -1) return -1;
     if (DeeCodeWriter_Pop(writer) == -1) return -1;
    }
   }
   break;

  case DeeExprAstKind_VARDECL: {
   DeeExprAstObject *dst,*src;
   dst = DeeExprAst_OP_A(self);
   src = DeeExprAst_OP_B(self);
   if (dst->ob_kind == DeeExprAstKind_TUPLE) {
    DeeExprAstObject **dst_iter,**dst_begin,**dst_end;
    // Multiple output values
    DEE_ASSERT(src->ob_kind == DeeExprAstKind_EXPAND &&
               "Multi-out vardecl src isn't an expand expression");
    dst_end = (dst_begin = (DeeExprAstObject **)DeeTuple_ELEM(dst->ob_args_a))+DeeTuple_SIZE(dst->ob_args_a);
    dst_iter = dst_begin; while (dst_iter != dst_end) {
     dst = *dst_iter++;
     if (dst->ob_kind == DeeExprAstKind_VARIABLE ||
         dst->ob_kind == DeeExprAstKind_DECLVARIABLE) {
      var = dst->ob_var;
      if (!DeeScope_SameWeakScope(scope,(DeeObject *)var->ob_scope)) {
       if (DeeError_CompilerError(DEE_WARNING_CANT_INITIALIZE_VAR_FROM_DIFFERENT_SCOPE,lexer,(DeeObject *)
        dst->ob_token,"Cannot store value in variable from underlying scope") == -1) return -1;
       // Just act like it's an assignment to none
       dst->ob_kind = DeeExprAstKind_CONSTANT;
       Dee_DECREF(dst->ob_var);
       Dee_INCREF(dst->ob_const = Dee_None);
      }
     }
    }
    // Compile the source expression and unpack it
    if (_dee_exprast_compile_and_unpack(
     src->ob_op_a,(DeeScopeObject *)scope,
     writer,(DeeLexerObject *)lexer,flags|DEE_COMPILER_FLAG_USED,
     (Dee_uint16_t)(dst_end-dst_begin)) == -1) return -1;
    // Store the values in the dst elements
    // NOTE: We have to do this in reverse order
    dst_iter = dst_end; while (dst_iter != dst_begin) {
     dst = *--dst_iter;
     if (dst->ob_kind == DeeExprAstKind_VARIABLE ||
         dst->ob_kind == DeeExprAstKind_DECLVARIABLE) { // Vardecl
      // Store the value in the variable
      if (DeeCodeWriter_StoreLocalPop(writer,(DeeObject *)dst->ob_var,
       have_local_names?lexer:NULL) == -1) return -1;
     } else if (dst->ob_kind != DeeExprAstKind_CONSTANT || !DeeNone_Check(dst->ob_const)) { // Assign to none (just pop it)
      if (DeeCodeWriter_Pop(writer) == -1) return -1;
     } else { // Assignment
      // Compile the dst expression
      // NOTE: This kind of breaks left-to-right evaluation order, but what ev's...
      if (DeeExprAst_Compile((DeeObject *)dst,
       scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
      // Rotate our 'dst' so the assign operator goes the right direction
      if (DeeCodeWriter_WriteOp(writer,OP_ROT_2) == -1) return -1;
      // Assign one entry to 'dst'
      if (DeeCodeWriter_BinaryOp(writer,OP_COPY_ASSIGN) == -1) return -1;
      // Pop 'dst' from the stack
      // TODO: We have to store it somewhere, so we can reuse it
      //       if we have to build a tuple when 'ret_used' is true.
      if (DeeCodeWriter_Pop(writer) == -1) return -1;
     }
    }
    // Everything is stored where it belongs (stacksize should be back to normal)
    // Now we have to build a tuple if ret_used is true
    if (ret_used) {
     dst_iter = dst_begin; while (dst_iter != dst_end) {
      dst = *dst_iter++;
      if (dst->ob_kind == DeeExprAstKind_VARIABLE ||
          dst->ob_kind == DeeExprAstKind_DECLVARIABLE) {
       if (DeeCodeWriter_LoadLocal(writer,(DeeObject *)dst->ob_var) == -1) return -1;
      } else {
       // TODO: We'd have to push the return value of the OP_COPY_ASSIGN,
       //       but currently I have to no idea where to store the values
       //       - We can't use the stack, since OP_ROT_X only works for up to 2 temporaries
       //       So for right now we just push none, but this is technically wrong!
       if (DeeCodeWriter_PushNone(writer) == -1) return -1;
      }
     }
     // Finally generate the tuple consisting of the dst variables
     if (_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,(Dee_uint16_t)(dst_end-dst_begin)) == -1) return -1;
    }
   } else {
    // Single output values
    DEE_ASSERTF(dst->ob_kind == DeeExprAstKind_VARIABLE ||
                dst->ob_kind == DeeExprAstKind_DECLVARIABLE,
                "Single-out vardecl dst isn't a variable");
    var = dst->ob_var;
    if ((var->ob_flags&(DEE_LOCALVAR_FLAG_INITIALIZED|DEE_LOCAL_VAR_FLAG_STATIC
     ))==DEE_LOCAL_VAR_FLAG_STATIC) { // Uninitialized static
     Dee_uint16_t static_id;
     DEE_ASSERT(src->ob_kind == DeeExprAstKind_CONSTANT);
     DEE_ASSERT(DeeScope_SameWeakScope(scope,(DeeObject *)var->ob_scope));
     if ((static_id = DeeCodeWriter_AllocConst(writer,src->ob_const)) == (Dee_uint16_t)-1) return -1;
     var->ob_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
     var->ob_loc_id = static_id;
     if (ret_used) {
      if (DeeCodeWriter_WriteOpWithArg(writer,OP_LOAD_CST_LOCKED,static_id) == -1) return -1;
      DeeCodeWriter_INCSTACK(writer);
     }
    } else {
     if (!DeeScope_SameWeakScope(scope,(DeeObject *)var->ob_scope)) {
      if (DeeError_CompilerError(DEE_WARNING_CANT_INITIALIZE_VAR_FROM_DIFFERENT_SCOPE,lexer,(DeeObject *)
       dst->ob_token,"Cannot store value in variable from underlying scope") == -1) return -1;
      // Just compile the source expression without assigning it
      return DeeExprAst_Compile((DeeObject *)src,scope,writer,lexer,flags);
     }
     if (!ret_used && !var->ob_uses) {
      // Unused variable
      DEE_LVERBOSE2("Removing unused local variable %K\n",
                    DeeToken_Str((DeeObject *)dst->ob_token));
      return DeeExprAst_Compile((DeeObject *)src,scope,writer,lexer,flags);
     } else {
      // Compile the source expression
#if DEE_CONFIG_COMPILER_HAVE_DYNAMIC_RUNTIME_SELF_REFERENCE
      switch (src->ob_kind) {
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
       case DeeExprAstKind_CLASS: {
        struct DeeClassDeclObject *class_decl;
        class_decl = ((DeeExprAstObject *)src)->ob_classdecl;
        if ((class_decl->ob_base
         ? DeeExprAst_Compile((DeeObject *)class_decl->ob_base,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)
         : DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)&DeeObject_Type)) == -1 ||
         DeeCodeWriter_PushConstNoCopy(writer,class_decl->ob_name ? (DeeObject *)class_decl->ob_name : Dee_EmptyString) == -1 ||
         DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_CLASS_NEW) == -1
         ) return -1;
        DeeCodeWriter_DECSTACK_N(writer,1); // -2 +1
        // Store the new class in its variable
        if (DeeCodeWriter_StoreLocal(writer,(DeeObject *)var,
         have_local_names?lexer:NULL) == -1) return -1;
        if (_DeeExprAst_CompileClassElements(class_decl,scope,writer,lexer,flags) == -1) return -1;
        // Variable isn't used
        if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
       } break;
#endif
       case DeeExprAstKind_FUNCTION:
       case DeeExprAstKind_FUNCTION_EXPR:
       case DeeExprAstKind_VFUNCTION:
       case DeeExprAstKind_VFUNCTION_EXPR:
       case DeeExprAstKind_YFUNCTION:
       case DeeExprAstKind_YVFUNCTION:
        if (_DeeScope_HasRef(DeeExprAst_SCOPE(src),var)) {
         if (DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyTuple) == -1 ||
             DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyCode) == -1 ||
             DeeCodeWriter_WriteOpWithArg(writer,OP_FUNCTION,0) == -1) return -1;
         // Allow for functions to reference themselves
         DeeCodeWriter_DECSTACK(writer); // -2 +1
         // v store the function in the variable (allows the real references to access it)
         //   NOTE: But keep the variable on the stack to we don't have to reload it for assignment
         if (DeeCodeWriter_StoreLocal(writer,(DeeObject *)var,have_local_names?lexer:NULL) == -1) return -1;
         if (DeeExprAst_Compile((DeeObject *)src,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
         // v assign the newly generated function to its variable
         if (DeeCodeWriter_BinaryOp(writer,OP_COPY_ASSIGN) == -1) return -1;
         // v Pop the function variable if it isn't used
         if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
         break;
        }
        // fallthrough
       default:
#endif /* DEE_CONFIG_COMPILER_HAVE_DYNAMIC_RUNTIME_SELF_REFERENCE */
        if (DeeExprAst_Compile((DeeObject *)src,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
        if ((ret_used // Store the value in the local variable
         ? DeeCodeWriter_StoreLocal(writer,(DeeObject *)var,have_local_names?lexer:NULL)
         : DeeCodeWriter_StoreLocalPop(writer,(DeeObject *)var,have_local_names?lexer:NULL)
         ) == -1) return -1;
#if DEE_CONFIG_COMPILER_HAVE_DYNAMIC_RUNTIME_SELF_REFERENCE
        break;
      }
#endif /* DEE_CONFIG_COMPILER_HAVE_DYNAMIC_RUNTIME_SELF_REFERENCE */
     }
    }
   }
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DeeExprAstKind_BUILTIN_BREAKPOINT:
   if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BREAKPOINT) == -1) return -1;
   goto push_none;
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DeeExprAstKind_BUILTIN_UNREACHABLE:
   if ((flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0) {
    if (DeeCodeWriter_Unreachable(writer) == -1) return -1;
   }
   if (ret_used) DeeCodeWriter_INCSTACK(writer); // Satisfy the writer
   return 1;
#endif

  case DeeExprAstKind_ASSIGN:
   op = OP_COPY_ASSIGN;
   goto binary;

  case DeeExprAstKind_MODULE:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
push_none:
#endif
   // TODO: Warning (can't reference module at runtime)
   if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
   break;

  case DeeExprAstKind_CONSTANT:
   if (ret_used && DeeCodeWriter_PushConst(writer,DeeExprAst_CONST(self)) == -1)
    return -1;
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  case DeeExprAstKind_CODE:
   // Code expression: ({ ... })
   return (DeeStmtAst_Compile((DeeObject *)DeeExprAst_STMT(self),
    (DeeObject *)DeeExprAst_SCOPE(self),writer,lexer,
    flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP) == -1 ||
    DeeCodeWriter_FinalizeScope(writer,(DeeObject *)DeeExprAst_SCOPE(self),lexer) == -1
    ) ? -1 : 0;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */

  case DeeExprAstKind_COND: {
   int temp;
   int cond_is_true = DeeExprAst_OP_A(self) == DeeExprAst_OP_B(self);
   // Compile the condition code
   temp = DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),
                             scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED);
   if (temp == -1) return -1;
   if (temp == 1) { // Unreachable
    if (!ret_used) DeeCodeWriter_DECSTACK(writer); // Satisfy the writer
    return 1;
   }
   pos1 = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,
    cond_is_true ? OP_JUMP_IF_FF : OP_JUMP_IF_FF_POP,&future_arg_1) == -1) return -1;
   if (!cond_is_true) {
    DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
    // Write the true branch
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1) return -1;
   }
   pos2 = DeeCodeWriter_ADDR(writer); // setup the jump if the check succeeds
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,OP_JUMP,&future_arg_2) == -1) return -1;
   // if false, we jump after the jump instruction that skips the false branch
   DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos1));
   // Write the false branch
   if (DeeExprAst_OP_C(self)) {
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(self),scope,writer,lexer,flags) == -1) return -1;
   } else {
    if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
   }
   DeeCodeWriter_SetFutureArg(writer,future_arg_2,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos2));
   if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
  } break;

  case DeeExprAstKind_BUILTIN_HELP: {
   DeeExprAstObject *help_ast = DeeExprAst_OP_A(self);
   if (!ret_used) {
    return DeeExprAst_Compile((DeeObject *)help_ast,scope,writer,lexer,flags);
   } else if (help_ast->ob_kind == DeeExprAstKind_ATTR_GET) {
    if (DeeExprAst_Compile((DeeObject *)help_ast->ob_op_a,scope,writer,lexer,flags) == -1 ||
        DeeCodeWriter_PushConst(writer,(DeeObject *)help_ast->ob_member) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_DOC_ATTR) == -1) return -1;
    DeeCodeWriter_DECSTACK(writer); // Consumed by 'OPEXT_DOC_ATTR'
   } else {
    if (DeeExprAst_Compile((DeeObject *)help_ast,scope,writer,lexer,flags) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_DOC) == -1) return -1;
   }
  } break;

  case DeeExprAstKind_SUPER:
   if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_VARIABLE &&
      (DeeExprAst_OP_A(self)->ob_var->ob_flags&DEE_LOCAL_VAR_FLAG_THIS)!=0) {
    // Compile as 'OPEXT_SUPER_THIS'
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_SUPER_THIS) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer);
    break;
   }
   op = OP_SUPEROF;
   goto unary;
  case DeeExprAstKind___SUPER:
   // Compile as 'OPEXT_SUPER_THIS'
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1 ||
       DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_SUPER_AT) == -1) return -1;
   DeeCodeWriter_DECSTACK(writer);
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
  case DeeExprAstKind_DELETE:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_WriteOp(writer,OP_STRUCT_PTR_DELETE) == -1) return -1;
   DeeCodeWriter_DECSTACK(writer); // Consumed by 'OP_STRUCT_PTR_DELETE'
   if (ret_used && DeeCodeWriter_PushNone(writer) == -1) return -1;
   break;
  case DeeExprAstKind_NEW:      op = OP_STRUCT_PTR_NEW; goto unary;
#endif

  case DeeExprAstKind_STR:      op = OP_STR; goto unary;
  case DeeExprAstKind_REPR:     op = OP_REPR; goto unary;
  case DeeExprAstKind_COPY:     op = OP_COPY; goto unary;
  case DeeExprAstKind_MOVE:     op = OP_MOVE; goto unary;
  case DeeExprAstKind_TYPE:     op = OP_TYPE; goto unary;
  case DeeExprAstKind_CLASSOF:  op = OP_CLASSOF; goto unary;
  case DeeExprAstKind_WEAK:     op = OP_WEAK; goto unary;
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
  case DeeExprAstKind_REF:      op = OP_REF; goto unary;
  case DeeExprAstKind_DEREF:    op = OP_DEREF; goto unary;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DeeExprAstKind_BUILTIN_BOUND: if (ret_used) {
   DeeLocalVarObject *localvar;
   DEE_ASSERT(DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_VARIABLE ||
              DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_DECLVARIABLE);
   localvar = DeeExprAst_OP_A(self)->ob_var;
   if ((localvar->ob_flags&DEE_LOCAL_VAR_FLAG_RETVAL)!=0) {
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BOUND_RESULT) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer);
   } else if ((localvar->ob_flags&DEE_LOCALVAR_FLAG_INITIALIZED)==0) {
    // Variable isn't initialized at this point (shouldn't happen, but is a constant false)
    if (DeeCodeWriter_PushConstNoCopy(writer,Dee_False) == -1) return -1;
   } else {
    DEE_ASSERT((localvar->ob_flags&(
     DEE_LOCAL_VAR_FLAG_PARAMENTER|
     DEE_LOCAL_VAR_FLAG_STATIC|
     DEE_LOCAL_VAR_FLAG_THIS
     ))==0 && "Invalid variable type");
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_PREFIX,localvar->ob_loc_id) == -1 ||
        DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_BOUND_LOCAL) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer);
   }
  } break;
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
  case DeeExprAstKind_CELL:
   if (DeeExprAst_OP_A(self)) { op = OP_CELL; goto unary; }
   if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_CELL) == -1) return -1;
   DeeCodeWriter_INCSTACK(writer);
   return 0;
#endif
  case DeeExprAstKind_BOOL:     op = OP_BOOL; goto unary;
  case DeeExprAstKind_NOT:      op = OP_NOT; goto unary;
  case DeeExprAstKind_NEG:      op = OP_NEG; goto unary;
  case DeeExprAstKind_POS:      op = OP_POS; goto unary;
  case DeeExprAstKind_INV:      op = OP_INV; goto unary;
  case DeeExprAstKind_INC:      op = OP_INC; goto unary;
  case DeeExprAstKind_DEC:      op = OP_DEC; goto unary;
  case DeeExprAstKind_INC_POST: op = OP_INC_POST; goto unary;
  case DeeExprAstKind_DEC_POST: op = OP_DEC_POST; unary:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_UnaryOp(writer,op) == -1) return -1;
pop_unused:
   if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
   break;
  case DeeExprAstKind_CMP_EQ: op = OP_CMP_EQ; goto cmp_eq_op;
  case DeeExprAstKind_CMP_NE: op = OP_CMP_NE; cmp_eq_op: {
#if 1
   DeeTypeObject *lhs_ty,*rhs_ty;
   if ((lhs_ty = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_A(self),scope)) != NULL &&
       (rhs_ty = DeeExprAst_PredictType((DeeObject *)DeeExprAst_OP_B(self),scope)) != NULL) {
    Dee_uint64_t temp; int lhs_kind,rhs_kind; // 0 --> zero(0); 1 --> non-zero(e.g.: 1); 2 --> ?
    //           INPUT            |||    OUTPUT | OUTPUT in cond-expr
    // integral:~0 == integral:0  -->      !lhs / lhs // +signal cond inversion
    // integral:0  == integral:~0 -->      !rhs / rhs // +signal cond inversion
    // integral:~0 != integral:0  --> (bool)lhs / lhs
    // integral:0  != integral:~0 --> (bool)rhs / rhs
    if ((_dee_type_is_int(lhs_ty) || lhs_ty == (DeeTypeObject *)&DeeBool_Type || DeePointerType_Check(lhs_ty)) &&
        (_dee_type_is_int(rhs_ty) || rhs_ty == (DeeTypeObject *)&DeeBool_Type || DeePointerType_Check(rhs_ty))) {
     if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_CONSTANT) {
      if (DeePointerType_Check(lhs_ty)) lhs_kind = DeePointer_ADDR(DeeExprAst_OP_A(self)->ob_const) ? 1 : 0;
      else if (DeeObject_Cast(Dee_uint64_t,DeeExprAst_OP_A(self)->ob_const,&temp) != 0) DeeError_Handled(),lhs_kind = 2;
      else lhs_kind = temp ? 1 : 0;
     } else lhs_kind = 2;
     if (DeeExprAst_OP_B(self)->ob_kind == DeeExprAstKind_CONSTANT) {
      if (DeePointerType_Check(rhs_ty)) rhs_kind = DeePointer_ADDR(DeeExprAst_OP_B(self)->ob_const) ? 1 : 0;
      else if (DeeObject_Cast(Dee_uint64_t,DeeExprAst_OP_B(self)->ob_const,&temp) != 0) DeeError_Handled(),rhs_kind = 2;
      else rhs_kind = temp ? 1 : 0;
     } else rhs_kind = 2;
     if (op == OP_CMP_EQ && lhs_kind == 1 && rhs_kind == 0) { // !lhs
      if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags) == -1 ||
          DeeCodeWriter_WriteOp(writer,OP_NOT) == -1) return -1;
     } else if (op == OP_CMP_EQ && lhs_kind == 1 && rhs_kind == 0) { // !rhs
      if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1 ||
          DeeCodeWriter_WriteOp(writer,OP_NOT) == -1) return -1;
     } else if (op == OP_CMP_NE && lhs_kind == 1 && rhs_kind == 0) { // (bool)lhs
      if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags) == -1) return -1;
      if (lhs_ty != (DeeTypeObject *)&DeeBool_Type &&
          DeeCodeWriter_WriteOp(writer,OP_BOOL) == -1) return -1;
     } else if (op == OP_CMP_NE && lhs_kind == 1 && rhs_kind == 0) { // (bool)rhs
      if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1) return -1;
      if (rhs_ty != (DeeTypeObject *)&DeeBool_Type &&
          DeeCodeWriter_WriteOp(writer,OP_BOOL) == -1) return -1;
     } else goto binary;
    } else goto binary;
   } else
#endif
    goto binary;
  } break;
  case DeeExprAstKind_CMP_LO: op = OP_CMP_LO; goto binary;
  case DeeExprAstKind_CMP_LE: op = OP_CMP_LE; goto binary;
  case DeeExprAstKind_CMP_GR: op = OP_CMP_GR; goto binary;
  case DeeExprAstKind_CMP_GE: op = OP_CMP_GE; goto binary;
  case DeeExprAstKind_ADD:    op = OP_ADD; goto binary;
  case DeeExprAstKind_SUB:    op = OP_SUB; goto binary;
  case DeeExprAstKind_MUL:    op = OP_MUL; goto binary;
  case DeeExprAstKind_DIV:    op = OP_DIV; goto binary;
  case DeeExprAstKind_MOD:    op = OP_MOD; goto binary;
  case DeeExprAstKind_SHL:    op = OP_SHL; goto binary;
  case DeeExprAstKind_SHR:    op = OP_SHR; goto binary;
  case DeeExprAstKind_AND:    op = OP_AND; goto binary;
  case DeeExprAstKind_OR:     op = OP_OR; goto binary;
  case DeeExprAstKind_XOR:    op = OP_XOR; goto binary;
  case DeeExprAstKind_POW:    op = OP_POW; binary:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_BinaryOp(writer,op) == -1) return -1;
   goto pop_unused;
#define INPLACE_OPS(name) \
 op=OP_INPLACE_##name,\
 op_loc=OP_LOC_INPLACE_##name,\
 op_cst=OP_CST_INPLACE_##name,\
 op_arg=OP_ARG_INPLACE_##name,\
 op_retval=OPEXT_RET_INPLACE_##name
  case DeeExprAstKind_IADD: INPLACE_OPS(ADD); goto inplace_binary;
  case DeeExprAstKind_ISUB: INPLACE_OPS(SUB); goto inplace_binary;
  case DeeExprAstKind_IMUL: INPLACE_OPS(MUL); goto inplace_binary;
  case DeeExprAstKind_IDIV: INPLACE_OPS(DIV); goto inplace_binary;
  case DeeExprAstKind_IMOD: INPLACE_OPS(MOD); goto inplace_binary;
  case DeeExprAstKind_ISHL: INPLACE_OPS(SHL); goto inplace_binary;
  case DeeExprAstKind_ISHR: INPLACE_OPS(SHR); goto inplace_binary;
  case DeeExprAstKind_IAND: INPLACE_OPS(AND); goto inplace_binary;
  case DeeExprAstKind_IOR:  INPLACE_OPS(OR);  goto inplace_binary;
  case DeeExprAstKind_IXOR: INPLACE_OPS(XOR); goto inplace_binary;
  case DeeExprAstKind_IPOW: INPLACE_OPS(POW); inplace_binary: {
#undef INPLACE_OPS
   if (DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_VARIABLE ||
       DeeExprAst_OP_A(self)->ob_kind == DeeExprAstKind_DECLVARIABLE) {
    var = DeeExprAst_OP_A(self)->ob_var;
    if ((var->ob_flags&DEE_LOCALVAR_FLAG_INITIALIZED) == 0 &&
        DeeCodeWriter_InitLocal(writer,(DeeObject *)var,
       have_local_names?lexer:NULL) == -1) return -1;
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),
     scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
    if ((var->ob_flags&DEE_LOCAL_VAR_FLAG_PARAMENTER)!=0) {
     if (DeeCodeWriter_WriteOpWithArg(writer,op_arg,var->ob_loc_id) == -1) return -1;
    } else if ((var->ob_flags&DEE_LOCAL_VAR_FLAG_RETVAL)!=0) {
     if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,op_retval) == -1) return -1;
    } else if ((var->ob_flags&DEE_LOCAL_VAR_FLAG_STATIC)!=0) {
     if (DeeCodeWriter_WriteOpWithArg(writer,op_cst,var->ob_loc_id) == -1) return -1;
    } else {
     if (DeeCodeWriter_WriteOpWithArg(writer,op_loc,var->ob_loc_id) == -1) return -1;
    }
   } else {
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
        DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
        DeeCodeWriter_BinaryOp(writer,op) == -1) return -1;
   }
   goto pop_unused;
  } break;

  case DeeExprAstKind_LAND:
  case DeeExprAstKind_LOR:
   // Compile the lhs AST
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),
    scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1) return -1;
   if (ret_used && DeeCodeWriter_UnaryOp(writer,OP_BOOL) == -1) return -1; // cast lhs to bool
   pos1 = DeeCodeWriter_ADDR(writer); // Jump over the rhs AST if we know it'll result in false
   if (DeeCodeWriter_WriteOpWithFutureArg(writer,(Dee_uint8_t)(
    DeeExprAst_KIND(self) == DeeExprAstKind_LAND
    ? (ret_used ? OP_JUMP_IF_FF : OP_JUMP_IF_FF_POP) // a && b (skip b if a is false)
    : (ret_used ? OP_JUMP_IF_TT : OP_JUMP_IF_TT_POP) // a || b (skip b if a is true)
    ),&future_arg_1) == -1) return -1;
   if (!ret_used) {
    // 'OP_JUMP_IF_FF_POP' / 'OP_JUMP_IF_TT_POP' popped this one
    DeeCodeWriter_DECSTACK(writer);
   } else {
    // pop lhs if we generated a value
    if (DeeCodeWriter_Pop(writer) == -1) return -1;
   }
   // compile the rhs AST
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1) return -1;
   if (ret_used && DeeCodeWriter_UnaryOp(writer,OP_BOOL) == -1) return -1; // cast rhs to bool if it's used
   // This is where we jump if rhs was skipped
   DeeCodeWriter_SetFutureArg(writer,future_arg_1,(Dee_uint16_t)(DeeCodeWriter_ADDR(writer)-pos1));
   break;
  case DeeExprAstKind_LXOR:
   if (ret_used) {
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags) == -1 ||
        DeeCodeWriter_UnaryOp(writer,OP_BOOL) == -1 ||
        DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1 ||
        DeeCodeWriter_UnaryOp(writer,OP_BOOL) == -1 ||
        DeeCodeWriter_BinaryOp(writer,OP_XOR) == -1
        ) return -1;
   } else {
    // NOTE: Technically we'd have to call the bool operator here... But what the hell!
    if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags) == -1 ||
        DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags) == -1
        ) return -1;
   }
   break;
  case DeeExprAstKind_LIST:
   op = OP_LIST;
   goto seq;
  case DeeExprAstKind_TUPLE: op = OP_TUPLE; seq: {
#define is_tuple (op==OP_TUPLE)
   int is_expand_construct = 0; Dee_size_t active_seq_size = 0;
   expr_list = DeeExprAst_ARGS_A(self);
   // Compile all sequence elements
   if (!DeeTuple_SIZE(expr_list)) {
    if (ret_used && _DeeCodeWriter_MakeSequence(writer,op,0) == -1) return -1;
   } else {
    DeeObject **begin,**end;
    end = (begin = DeeTuple_ELEM(expr_list))+DeeTuple_SIZE(expr_list);
    while (begin != end) {
     DeeExprAstObject *elem_ast = (DeeExprAstObject *)*begin++;
     if (elem_ast->ob_kind == DeeExprAstKind_EXPAND) {
      DeeObject *expand_ob = (DeeObject *)DeeExprAst_OP_A(elem_ast);
      if (ret_used) {
       // Pack everything we've got so far
       if (active_seq_size && _DeeCodeWriter_MakeSequence(writer,op,(Dee_uint16_t)active_seq_size) == -1) return -1;
       if (is_expand_construct && DeeCodeWriter_BinaryOp(writer,(Dee_uint8_t)(
        is_tuple ? OP_CONCAT_TUPLE : OP_CONCAT_LIST)) == -1) return -1;
       // Compile the new portion that should be expanded
       if (DeeExprAst_Compile(expand_ob,scope,writer,lexer,flags) == -1) return -1;
       if (active_seq_size) {
        // Concat the two portions
        if (DeeCodeWriter_BinaryOp(writer,(Dee_uint8_t)(
         is_tuple ? OP_CONCAT_TUPLE : OP_CONCAT_LIST)
         ) == -1)return -1;
       } else {
        DeeTypeObject *seq_type;
        // This is the first portion (make sure it's of the correct typing)
        // NOTE: We only have to do this if we can't predict its type.
        seq_type = DeeExprAst_PredictType(expand_ob,(DeeObject *)scope);
        if ((!seq_type || (is_tuple ? (seq_type != &DeeTuple_Type) : (seq_type != &DeeList_Type))) &&
            DeeCodeWriter_UnaryOp(writer,(Dee_uint8_t)(is_tuple ? OP_CAST_TUPLE : OP_CAST_LIST)) == -1
            ) return -1;
       }
       is_expand_construct = 1;
       active_seq_size = 0;
      } else {
       if (DeeExprAst_Compile(expand_ob,scope,writer,lexer,flags) == -1) return -1;
      }
     } else {
      if (DeeExprAst_Compile((DeeObject *)elem_ast,scope,writer,lexer,flags) == -1) return -1;
      ++active_seq_size;
     }
    }
    if (ret_used && active_seq_size) {
     if (_DeeCodeWriter_MakeSequence(
       writer,op,(Dee_uint16_t)active_seq_size) == -1) return -1;
     // Must still cat the last section with the previous expand portion
     if (is_expand_construct && DeeCodeWriter_BinaryOp(writer,(Dee_uint8_t)(
      is_tuple ? OP_CONCAT_TUPLE : OP_CONCAT_LIST)) == -1) return -1;
    }
   }
#undef is_tuple
  } break;

  case DeeExprAstKind_CALL: // Call expression
   return _DeeExprAst_CompileCall((DeeExprAstObject *)self,(DeeScopeObject *)scope,writer,(DeeLexerObject *)lexer,flags);

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DeeExprAstKind_BUILTIN_ALLOCA:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_ALLOCA) == -1) return -1;
   goto pop_unused;
#endif

  case DeeExprAstKind_SEQ_SIZE: op = OP_SEQ_SIZE; goto unary;
  case DeeExprAstKind_SEQ_GET: op = OP_SEQ_GET; goto binary;
  case DeeExprAstKind_SEQ_SET:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_TrinaryOp(writer,OP_SEQ_SET) == -1) return -1;
   goto pop_unused;
  case DeeExprAstKind_SEQ_RANGE_GET:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       (DeeExprAst_OP_B(self) ? DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) : DeeCodeWriter_PushNone(writer)) == -1 ||
       (DeeExprAst_OP_C(self) ? DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) : DeeCodeWriter_PushNone(writer)) == -1 ||
       DeeCodeWriter_TrinaryOp(writer,OP_SEQ_RANGE_GET) == -1) return -1;
   goto pop_unused;
  case DeeExprAstKind_SEQ_RANGE_SET:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       (DeeExprAst_OP_B(self) ? DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_B(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) : DeeCodeWriter_PushNone(writer)) == -1 ||
       (DeeExprAst_OP_C(self) ? DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) : DeeCodeWriter_PushNone(writer)) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_D(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeCodeWriter_QuadaryOp(writer,OP_SEQ_RANGE_SET) == -1) return -1;
   goto pop_unused;
  case DeeExprAstKind_ATTR_GET:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1
       ) return -1;
   slot = DeeCodeWriter_AllocConst(writer,(DeeObject *)DeeExprAst_MEMBER(self));
   if (slot == (Dee_uint16_t)-1) return -1;
   // Get the given attribute
   if (DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_GET_C,slot) == -1) return -1;
   goto pop_unused;
  case DeeExprAstKind_ATTR_SET:
   if (DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_A(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       DeeExprAst_Compile((DeeObject *)DeeExprAst_OP_C(self),scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED) == -1 ||
       (slot = DeeCodeWriter_AllocConst(writer,(DeeObject *)DeeExprAst_MEMBER(self))) == (Dee_uint16_t)-1 ||
       // Set the given attribute
       DeeCodeWriter_WriteOpWithArg(writer,OP_ATTR_SET_C,slot) == -1
       ) return -1;
   DeeCodeWriter_DECSTACK(writer); // Consumed by 'OP_ATTR_SET_C' (pop(2),push(1))
   goto pop_unused;
  case DeeExprAstKind_IS: op = OP_IS; goto binary;
  case DeeExprAstKind_IN: op = OP_IN; goto binary;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_COMPILER_MSVC_WARNING_PUSH(4061)
  case DeeExprAstKind_CLASS: {
   struct DeeClassDeclObject *class_decl;
   class_decl = ((DeeExprAstObject *)self)->ob_classdecl;
   if ((class_decl->ob_base
    ? DeeExprAst_Compile((DeeObject *)class_decl->ob_base,scope,writer,lexer,flags|DEE_COMPILER_FLAG_USED)
    : DeeCodeWriter_PushConstNoCopy(writer,(DeeObject *)&DeeObject_Type)) == -1 ||
    DeeCodeWriter_PushConstNoCopy(writer,class_decl->ob_name ? (DeeObject *)class_decl->ob_name : Dee_EmptyString) == -1 ||
    DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_CLASS_NEW) == -1
    ) return -1;
   DeeCodeWriter_DECSTACK_N(writer,1); // -2 +1
   if (_DeeExprAst_CompileClassElements(class_decl,scope,writer,lexer,flags) == -1) return -1;
   if (!ret_used && DeeCodeWriter_Pop(writer) == -1) return -1;
  } break;
DEE_COMPILER_MSVC_WARNING_POP
#endif

  case DeeExprAstKind_FUNCTION:
  case DeeExprAstKind_FUNCTION_EXPR:
  case DeeExprAstKind_FUNCTION_NONE:
  case DeeExprAstKind_VFUNCTION:
  case DeeExprAstKind_VFUNCTION_EXPR:
  case DeeExprAstKind_VFUNCTION_NONE:
  case DeeExprAstKind_YFUNCTION:
  case DeeExprAstKind_YVFUNCTION: if (ret_used) {
   DeeObject *code;
   DeeScopeObject *fun_scope = DeeExprAst_FUNCTION_SCOPE(self);
   Dee_size_t argc = _DeeTokenIDList_SIZE(DeeExprAst_FUNCTION_ARGS(self));
   struct DeeCodeWriter fun_writer = DeeCodeWriter_INIT();
   if (fun_scope && (fun_scope->ob_flags&DEE_SCOPE_FLAG_CODE_COPYABLE)!=0)
    fun_writer.ob_flags |= DEE_CODE_FLAG_COPY; // Enable the copyable bit
   if (DeeExprAst_KIND(self) == DeeExprAstKind_FUNCTION_EXPR ||
       DeeExprAst_KIND(self) == DeeExprAstKind_VFUNCTION_EXPR) {
    // Since only statements generate debug code,
    // we have to improvise to get at least one line of debug information in!
    if ((flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) {
     // Put some debug information
     if (DeeCodeWriter_Debug(&fun_writer,
      (flags&DEE_COMPILER_FLAG_DEBUG_FILE)!=0 ? DeeToken_FILE(DeeExprAst_TOKEN(self)) : NULL,
      (flags&DEE_COMPILER_FLAG_DEBUG_LINE)!=0 ? DeeToken_LINE(DeeExprAst_TOKEN(self)) : 0
      ) == -1) goto err_fun1;
    }
    if (DeeExprAst_Compile(
     (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
     &fun_writer,lexer,flags|DEE_COMPILER_FLAG_USED|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP) == -1 ||
     DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)fun_scope,lexer) == -1 ||
     DeeCodeWriter_Ret(&fun_writer) == -1) goto err_fun1;
   } else if (DeeExprAst_KIND(self) == DeeExprAstKind_FUNCTION ||
              DeeExprAst_KIND(self) == DeeExprAstKind_VFUNCTION ||
              DeeExprAst_KIND(self) == DeeExprAstKind_YFUNCTION ||
              DeeExprAst_KIND(self) == DeeExprAstKind_YVFUNCTION) {
    if (DeeStmtAst_Compile(
     (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
     &fun_writer,lexer,(flags|DEE_COMPILE_FLAG_NO_LOCAL_CLEANUP)&~(DEE_COMPILER_FLAG_USED)) == -1 ||
     DeeCodeWriter_FinalizeScope(&fun_writer,(DeeObject *)fun_scope,lexer) == -1
     ) goto err_fun1;
    if (!DeeStmtAst_IsNoreturn(
     (DeeObject *)DeeExprAst_FUNCTION_EXPR(self),(DeeObject *)fun_scope,
     DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS)) {
     DEE_ASSERT(fun_writer.ob_last != OP_YIELDSTOP &&
                fun_writer.ob_last != OP_RET &&
                fun_writer.ob_last != OP_RETNONE &&
                fun_writer.ob_last != OP_RETVAREXIT &&
                "Returning statement generated noreturn opcode");
     if (DeeExprAst_KIND(self) == DeeExprAstKind_YFUNCTION ||
         DeeExprAst_KIND(self) == DeeExprAstKind_YVFUNCTION) {
      // Put a missing yield exit statement, to stop iteration
      if (DeeCodeWriter_YieldExit(&fun_writer) == -1) goto err_fun1;
     } else if ((fun_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_RETVAR) != 0) {
      // Put a missing return statement, to return the stored result variable
      if (DeeCodeWriter_WriteOp(&fun_writer,OP_RETVAREXIT) == -1) goto err_fun1;
     } else {
      // Put a missing return statement, to return none by default
      if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_fun1;
     }
    } else if ((flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0) {
     if (DeeCodeWriter_Unreachable(&fun_writer) == -1) goto err_fun1;
    }
   } else {
    // No scope cleanup as we can't put our own code here...
    if (argc == 0) {
     Dee_INCREF(code = Dee_EmptyCode);
    } else {
     // Needed for correct allocation of argument locals
     // TODO: If we add opcodes to load/store from/to arguments we can optimize this quite a bit
     if (DeeCodeWriter_RetNone(&fun_writer) == -1) goto err_fun1;
     if ((code = DeeCodeWriter_Pack(&fun_writer,NULL)) == NULL) goto err_fun1;
    }
    goto emp_refs; // Continue by putting no references
   }
   if ((code = DeeCodeWriter_Pack(&fun_writer,(DeeObject *)(
    (flags&DEE_COMPILER_FLAG_DEBUG_FUNC)!=0
    ?DeeExprAst_FUNCTION_NAME(self):NULL))
    ) == NULL) goto err_fun1; // Pack the code
   DEE_ASSERT(!fun_scope || DeeObject_Check(fun_scope) && DeeScope_Check(fun_scope));
   if (fun_scope && fun_scope->ob_refc) {
    // Compile the list of function variable references
    if (DeeCodeWriter_PushVars(writer,scope,fun_scope->ob_refc,
     (DeeObject **)(fun_scope->ob_refv)) == -1) goto err_fun2;
    // Pack all references variable references into a tuple
    if (_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,(Dee_uint16_t)fun_scope->ob_refc) == -1) goto err_fun2;
   } else {
emp_refs: // empty refs
    if (DeeCodeWriter_PushConstNoCopy(writer,Dee_EmptyTuple) == -1) goto err_fun2;
   }
   if (DeeCodeWriter_PushConstNoCopy(writer,code) == -1)  goto err_fun2; // Push the code
   Dee_DECREF(code);
   {
    Dee_uint32_t func_flags;
    switch (DeeExprAst_KIND(self)) {
     case DeeExprAstKind_YFUNCTION:
      func_flags = DEE_FUNCTION_FLAG_YILD; break;
     case DeeExprAstKind_YVFUNCTION:
      func_flags = DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD; break;
     case DeeExprAstKind_VFUNCTION:
     case DeeExprAstKind_VFUNCTION_EXPR:
     case DeeExprAstKind_VFUNCTION_NONE:
      func_flags = DEE_FUNCTION_FLAG_VARG; break;
     default: func_flags = DEE_FUNCTION_FLAG_NONE; break;
    }
    if (fun_scope && (fun_scope->ob_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0)
     func_flags |= DEE_FUNCTION_FLAG_THIS;
    // Write the function flag prefix
    if (func_flags && DeeCodeWriter_WriteOpWithArg(writer,
     OP_PREFIX,(Dee_uint16_t)func_flags) == -1) goto err_fun1;
    // Write the actual function opcode
    if (DeeCodeWriter_WriteOpWithArg(writer,
     OP_FUNCTION,(Dee_uint16_t)argc) == -1) goto err_fun1;
   }
   DeeCodeWriter_DECSTACK(writer); // consumed by the function generator
   DeeCodeWriter_Quit(&fun_writer);
   break;
err_fun2: Dee_DECREF(code);
err_fun1: DeeCodeWriter_Quit(&fun_writer);
   return -1;
  } break;

  case DeeExprAstKind_SET: {
   DeeExprAstObject **iter,**end; Dee_size_t n_items;
   n_items = DeeTuple_SIZE(DeeExprAst_DICT(self));
   if (n_items) {
    end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(DeeExprAst_DICT(self)))+n_items;
    // Simply compile all keys
    while (iter != end) if (DeeExprAst_Compile(
     (DeeObject *)*iter++,scope,writer,lexer,flags) == -1) return -1;
    if (ret_used) {
     // If the result is used, write the set opcode
     if (DeeCodeWriter_WriteOpWithArg(writer,OP_DICT,(Dee_uint16_t)n_items) == -1) return -1;
     DeeCodeWriter_DECSTACK_N(writer,n_items); // Consumed by OP_SET
     DeeCodeWriter_INCSTACK(writer); // Created by OP_SET
    }
   } else if (ret_used) {
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_SET) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer); // Created by OPEXT_EMPTY_SET
   }
  } break;

  case DeeExprAstKind_DICT: {
   DeeExprAstObject **iter,**end; Dee_size_t n_items_x2;
   n_items_x2 = DeeTuple_SIZE(DeeExprAst_DICT(self));
   DEE_ASSERT(n_items_x2 % 2 == 0);
   if (n_items_x2) {
    end = (iter = (DeeExprAstObject **)DeeTuple_ELEM(DeeExprAst_DICT(self)))+n_items_x2;
    // Simply compile all key/item pairs
    while (iter != end) if (DeeExprAst_Compile(
     (DeeObject *)*iter++,scope,writer,lexer,flags) == -1) return -1;
    if (ret_used) {
     // If the result is used, write the dict opcode
     if (DeeCodeWriter_WriteOpWithArg(writer,OP_DICT,(Dee_uint16_t)(n_items_x2/2)) == -1) return -1;
     DeeCodeWriter_DECSTACK_N(writer,n_items_x2); // Consumed by OP_DICT
     DeeCodeWriter_INCSTACK(writer); // Created by OP_DICT
    }
   } else if (ret_used) {
    if (DeeCodeWriter_WriteOpWithArg(writer,OP_EXTENDED,OPEXT_EMPTY_DICT) == -1) return -1;
    DeeCodeWriter_INCSTACK(writer); // Created by OPEXT_EMPTY_DICT
   }
  } break;


  default: DEE_BUILTIN_UNREACHABLE();
 }
 return 0;
}
#undef have_local_names
#undef ret_used

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_EXPR_AST_COMPILE_C */
