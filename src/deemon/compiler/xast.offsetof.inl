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

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
/*noexcept*/int DeeXAst_GetOffsetof(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_OUT Dee_size_t *final_offset,
 DEE_A_OUT DeeTypeObject **final_type) {
 DEE_ASSERT(final_offset && final_type);
 switch (self->ast_kind) {

  case DEE_XASTKIND_CONST: {
   DeeObject *base_constant;
   // Make sure that this is an lvalue constant
   base_constant = self->ast_const.c_const;
   if (!DeeLValue_Check(base_constant)) return 0;
   *final_offset = (Dee_size_t)DeeLValue_ADDR(base_constant);
   *final_type = DeeLValueType_BASE(Dee_TYPE(base_constant));
   return 1;
  } break;

  case DEE_XASTKIND_DEREF: {
   DeeObject *base_constant;
   // Make sure that the final dereference is of a pointer type
   if (self->ast_operator.op_a->ast_kind != DEE_XASTKIND_CONST) return 0;
   base_constant = self->ast_operator.op_a->ast_const.c_const;
   if (!DeePointer_Check(base_constant)) return 0;
   *final_offset = (Dee_size_t)DeePointer_ADDR(base_constant);
   *final_type = DeePointerType_BASE(Dee_TYPE(base_constant));
   return 1;
  } break;

  case DEE_XASTKIND_ATTR_GET_C: {
   Dee_size_t additional_offset;
   // Recursively check for offset through attribute lookup
   if (!DeeXAst_GetOffsetof(self->ast_attr_get_c.ac_object,final_offset,final_type)) return 0;
   DEE_ASSERT(*final_type);
   if ((*final_type = DeeType_OffsetofAttr(*final_type,(DeeObject *)
    self->ast_attr_get_c.ac_name,&additional_offset)) == NULL) {
    // Missing attribute (todo: should we emit a warning for this?)
    DeeError_Handled();
    return 0;
   }
   *final_offset += additional_offset;
   return 1;
  } break;

  case DEE_XASTKIND_SEQ_GET: {
   Dee_size_t additional_offset;
   if (self->ast_operator.op_b->ast_kind != DEE_XASTKIND_CONST) return 0;
   // Recursively check for offset through attribute lookup
   if (!DeeXAst_GetOffsetof(self->ast_attr_get_c.ac_object,final_offset,final_type)) return 0;
   DEE_ASSERT(*final_type);
   if ((*final_type = DeeType_OffsetofItem(*final_type,(DeeObject *)
    self->ast_operator.op_b->ast_const.c_const,&additional_offset)) == NULL) {
    // Missing attribute (todo: should we emit a warning for this?)
    DeeError_Handled();
    return 0;
   }
   *final_offset += additional_offset;
   return 1;
  } break;

  default: break;
 }
 // Not an offsetof
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

DEE_DECL_END
