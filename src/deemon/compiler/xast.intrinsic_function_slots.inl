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
#include "xast.c"
#endif

DEE_DECL_BEGIN

DEE_A_RET_MAYBE_NULL DeeObject *
DeeBuiltin_GetIntrinsicFunctionOfTypeSlot(DEE_A_IN int typeslot) {
 switch (typeslot) {
  case DeeType_SLOT_ID(tp_copy_new):
  case DeeType_SLOT_ID(tp_copy_ctor):       return (DeeObject *)&DeeBuiltinFunction___copy__;
  case DeeType_SLOT_ID(tp_move_new):
  case DeeType_SLOT_ID(tp_move_ctor):       return (DeeObject *)&DeeBuiltinFunction___move__;
  case DeeType_SLOT_ID(tp_copy_assign):
  case DeeType_SLOT_ID(tp_any_assign):      return (DeeObject *)&DeeBuiltinFunction___copyassign__;
  case DeeType_SLOT_ID(tp_move_assign):     return (DeeObject *)&DeeBuiltinFunction___moveassign__;
  case DeeType_SLOT_ID(tp_str):             return (DeeObject *)&DeeString_Type;
  case DeeType_SLOT_ID(tp_repr):            return (DeeObject *)&DeeBuiltinFunction___repr__;
  case DeeType_SLOT_ID(tp_int32):           return (DeeObject *)DeeObject_TYPE(Dee_int32_t);
  case DeeType_SLOT_ID(tp_int64):           return (DeeObject *)DeeObject_TYPE(Dee_int64_t);
  case DeeType_SLOT_ID(tp_double):          return (DeeObject *)DeeObject_TYPE(double);
  case DeeType_SLOT_ID(tp_call):            return (DeeObject *)&DeeBuiltinFunction___callv__;
  case DeeType_SLOT_ID(tp_not):             return (DeeObject *)&DeeBuiltinFunction___not__;
  case DeeType_SLOT_ID(tp_bool):            return (DeeObject *)DeeObject_TYPE(bool);
  case DeeType_SLOT_ID(tp_inv):             return (DeeObject *)&DeeBuiltinFunction___inv__;
  case DeeType_SLOT_ID(tp_pos):             return (DeeObject *)&DeeBuiltinFunction___pos__;
  case DeeType_SLOT_ID(tp_neg):             return (DeeObject *)&DeeBuiltinFunction___neg__;
  case DeeType_SLOT_ID(tp_inc):             return (DeeObject *)&DeeBuiltinFunction___inc__;
  case DeeType_SLOT_ID(tp_incpost):         return (DeeObject *)&DeeBuiltinFunction___incpost__;
  case DeeType_SLOT_ID(tp_dec):             return (DeeObject *)&DeeBuiltinFunction___dec__;
  case DeeType_SLOT_ID(tp_decpost):         return (DeeObject *)&DeeBuiltinFunction___decpost__;
  case DeeType_SLOT_ID(tp_add):             return (DeeObject *)&DeeBuiltinFunction___add__;
  case DeeType_SLOT_ID(tp_iadd):            return (DeeObject *)&DeeBuiltinFunction___iadd__;
  case DeeType_SLOT_ID(tp_sub):             return (DeeObject *)&DeeBuiltinFunction___sub__;
  case DeeType_SLOT_ID(tp_isub):            return (DeeObject *)&DeeBuiltinFunction___isub__;
#if !DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_p_mul):
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DeeType_SLOT_ID(tp_mul):             return (DeeObject *)&DeeBuiltinFunction___mul__;
  case DeeType_SLOT_ID(tp_imul):            return (DeeObject *)&DeeBuiltinFunction___imul__;
  case DeeType_SLOT_ID(tp_div):             return (DeeObject *)&DeeBuiltinFunction___div__;
  case DeeType_SLOT_ID(tp_idiv):            return (DeeObject *)&DeeBuiltinFunction___idiv__;
  case DeeType_SLOT_ID(tp_mod):             return (DeeObject *)&DeeBuiltinFunction___mod__;
  case DeeType_SLOT_ID(tp_imod):            return (DeeObject *)&DeeBuiltinFunction___imod__;
  case DeeType_SLOT_ID(tp_shl):             return (DeeObject *)&DeeBuiltinFunction___shl__;
  case DeeType_SLOT_ID(tp_ishl):            return (DeeObject *)&DeeBuiltinFunction___ishl__;
  case DeeType_SLOT_ID(tp_shr):             return (DeeObject *)&DeeBuiltinFunction___shr__;
  case DeeType_SLOT_ID(tp_ishr):            return (DeeObject *)&DeeBuiltinFunction___ishr__;
#if !DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_p_and):
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DeeType_SLOT_ID(tp_and):             return (DeeObject *)&DeeBuiltinFunction___and__;
  case DeeType_SLOT_ID(tp_iand):            return (DeeObject *)&DeeBuiltinFunction___iand__;
  case DeeType_SLOT_ID(tp_or):              return (DeeObject *)&DeeBuiltinFunction___or__;
  case DeeType_SLOT_ID(tp_ior):             return (DeeObject *)&DeeBuiltinFunction___ior__;
  case DeeType_SLOT_ID(tp_xor):             return (DeeObject *)&DeeBuiltinFunction___xor__;
  case DeeType_SLOT_ID(tp_ixor):            return (DeeObject *)&DeeBuiltinFunction___ixor__;
  case DeeType_SLOT_ID(tp_pow):             return (DeeObject *)&DeeBuiltinFunction___pow__;
  case DeeType_SLOT_ID(tp_ipow):            return (DeeObject *)&DeeBuiltinFunction___ipow__;
  case DeeType_SLOT_ID(tp_hash):            return (DeeObject *)&DeeBuiltinFunction___hash__;
  case DeeType_SLOT_ID(tp_cmp_lo):          return (DeeObject *)&DeeBuiltinFunction___lo__;
  case DeeType_SLOT_ID(tp_cmp_le):          return (DeeObject *)&DeeBuiltinFunction___le__;
  case DeeType_SLOT_ID(tp_cmp_eq):          return (DeeObject *)&DeeBuiltinFunction___eq__;
  case DeeType_SLOT_ID(tp_cmp_ne):          return (DeeObject *)&DeeBuiltinFunction___ne__;
  case DeeType_SLOT_ID(tp_cmp_gr):          return (DeeObject *)&DeeBuiltinFunction___gr__;
  case DeeType_SLOT_ID(tp_cmp_ge):          return (DeeObject *)&DeeBuiltinFunction___ge__;
  case DeeType_SLOT_ID(tp_seq_get):         return (DeeObject *)&DeeBuiltinFunction___getitem__;
  case DeeType_SLOT_ID(tp_seq_del):         return (DeeObject *)&DeeBuiltinFunction___delitem__;
  case DeeType_SLOT_ID(tp_seq_set):         return (DeeObject *)&DeeBuiltinFunction___setitem__;
  case DeeType_SLOT_ID(tp_seq_size):        return (DeeObject *)&DeeBuiltinFunction___size__;
  case DeeType_SLOT_ID(tp_seq_contains):    return (DeeObject *)&DeeBuiltinFunction___contains__;
  case DeeType_SLOT_ID(tp_seq_range_get):   return (DeeObject *)&DeeBuiltinFunction___getrange__;
  case DeeType_SLOT_ID(tp_seq_range_del):   return (DeeObject *)&DeeBuiltinFunction___delrange__;
  case DeeType_SLOT_ID(tp_seq_range_set):   return (DeeObject *)&DeeBuiltinFunction___setrange__;
  case DeeType_SLOT_ID(tp_seq_iter_self):   return (DeeObject *)&DeeBuiltinFunction___iterself__;
//case DEE_CLASS_SLOTID_OPERATOR_FOR:       return ...; // TOOD: special 'operator for { ... }'
  case DeeType_SLOT_ID(tp_seq_iter_next):   return (DeeObject *)&DeeBuiltinFunction___iternext__;
  case DeeType_SLOT_ID(tp_attr_get):        return (DeeObject *)&DeeBuiltinFunction___getattr__;
  case DeeType_SLOT_ID(tp_attr_del):        return (DeeObject *)&DeeBuiltinFunction___delattr__;
  case DeeType_SLOT_ID(tp_attr_set):        return (DeeObject *)&DeeBuiltinFunction___setattr__;
  case DeeType_SLOT_ID(tp_p_add):           return (DeeObject *)&DeeBuiltinFunction___dual_posoradd__;
  case DeeType_SLOT_ID(tp_p_sub):           return (DeeObject *)&DeeBuiltinFunction___dual_negorsub__;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_p_mul):           return (DeeObject *)&DeeBuiltinFunction___dual_dereformul__;
  case DeeType_SLOT_ID(tp_p_and):           return (DeeObject *)&DeeBuiltinFunction___dual_reforand__;
#endif
  case DeeType_SLOT_ID(tp_p_seq_get):       return (DeeObject *)&DeeBuiltinFunction___dual_getitemorgetrange__;
  case DeeType_SLOT_ID(tp_p_seq_del):       return (DeeObject *)&DeeBuiltinFunction___dual_delitemordelrange__;
  case DeeType_SLOT_ID(tp_p_seq_set):       return (DeeObject *)&DeeBuiltinFunction___dual_setitemorsetrange__;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_REF:               return (DeeObject *)&DeeBuiltinFunction___ref__;
  case DEE_XAST_TYPESLOT_DEREF:             return (DeeObject *)&DeeBuiltinFunction___deref__;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_XAST_TYPESLOT_ATTR_HAS:          return (DeeObject *)&DeeBuiltinFunction___hasattr__;
  case DEE_XAST_TYPESLOT_NONE:              return Dee_None;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_IO_READ:           return (DeeObject *)&DeeBuiltinFunction___read__;
  case DEE_XAST_TYPESLOT_IO_WRITE:          return (DeeObject *)&DeeBuiltinFunction___write__;
  case DeeType_SLOT_ID(tp_io_read):         return (DeeObject *)&DeeBuiltinFunction___readp__;
  case DeeType_SLOT_ID(tp_io_write):        return (DeeObject *)&DeeBuiltinFunction___writep__;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_CLASS_SLOTID_OPERATOR_READNP:    return (DeeObject *)&DeeBuiltinFunction___readnp__;
  case DEE_CLASS_SLOTID_OPERATOR_WRITENP:   return (DeeObject *)&DeeBuiltinFunction___writenp__;
  default: break;
 }
 return NULL;
}

DEE_DECL_END
