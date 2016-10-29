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
#include "marshal.c"

DEE_DECL_BEGIN
#endif


#ifdef NEW_OBJECT
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_TWriteNewObjectWithMap
#else
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_TWriteObjectWithMap
#endif
(DEE_A_INOUT_OBJECT(DeeFileObject) *file,
 DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 Dee_uint32_t ref_id;
 DEE_ASSERT(DeeObject_Check(file) && DeeFile_Check(file));
 DEE_ASSERT(file_map);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == (DeeTypeObject const *)&DeeNone_Type) {
  // Fast-pass for the none singleton
  return DeeFile_Putc(file,DEE_MARSHALID_NONE);
 }

#ifndef NEW_OBJECT
 // Check if an existing object can be referenced
 ref_id = _DeeMarshalWriteMap_FindObject(file_map,tp_self,self);

 // If a reference can be created, do so!
 if DEE_UNLIKELY(ref_id != (Dee_uint32_t)-1)
  return _DeeMashal_WriteGetRef(file,ref_id);
#endif

 // Cast references to immutable objects (such as strings), types (such as classes)
 // and recursive objects (list containing itself, etc.)
 if (((file_map->mwm_flags&DEE_MARSHAL_WRITEFLAG_REFERENCE_ALL)!=0 && DeeType_IsImmutable(tp_self)) ||
     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) || DeeMarshal_IsRecursiveObject(tp_self,self)) {
  struct DeeMarshalHeaderPutRef block;
  // Declare a new object reference
  if ((ref_id = _DeeMarshalWriteMap_RegisterID(
   file_map,tp_self,self)) == (Dee_uint32_t)-1) return -1;
  if (ref_id <= 0xFF) {
   block.mhpr_marker = DEE_MARSHALID_PUTREF8;
   block.mhpr_block.mhpr_ref8 = (Dee_uint8_t)ref_id;
  } else if (ref_id <= 0xFFFF) {
   block.mhpr_marker = DEE_MARSHALID_PUTREF16;
   block.mhpr_block.mhpr_ref16 = DEE_BUILTIN_LESWAP16((Dee_uint16_t)ref_id);
  } else {
   block.mhpr_marker = DEE_MARSHALID_PUTREF32;
   block.mhpr_block.mhpr_ref32 = DEE_BUILTIN_LESWAP32((Dee_uint32_t)ref_id);
  }
  if (DeeFile_WriteAll(file,&block,DeeMarshalHeaderPutRef_SIZE(&block)) != 0) return -1;
  // Continue writing the object normally
 }
 return DeeMarshal_TDoWriteObjectWithMap(file,file_map,tp_self,self);
}


#ifdef NEW_OBJECT
#undef NEW_OBJECT
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
