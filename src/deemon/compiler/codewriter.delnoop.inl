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
#include "codewriter.c"
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Delete and relocate code
// NOTE: Will not reallocate the internal code buffer
static void DeeCodeWriter_DelAndRelocateCode(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN Dee_size_t start, DEE_A_IN Dee_size_t size) {
 Dee_uint8_t *begin,*iter,*del_start,*del_end,*end,op;
 Dee_size_t jmp_start,jmp_end;
 DEE_ASSERT(self);
 DEE_ASSERT(start < (Dee_size_t)(self->cw_code_pos-self->cw_code_begin));
 DEE_ASSERT(size != 0);
 DEE_LVERBOSE1R("%s(%d) : +%.4Ix : Deleting %Iu bytes of code\n",
                DeeCodeWriter_Addr2File(self,start),
                DeeCodeWriter_Addr2Line(self,start)+1,
                start,size);
 del_start = (iter = begin = self->cw_code_begin)+start;
 del_end = del_start+size;
 end = self->cw_code_pos;

 // Relocate jumps 
 while (iter != end) {
  if DEE_UNLIKELY(iter == del_start) iter = del_end;
  op = *iter++;
  switch (op) {
   DEE_OPCODE_CASE_HAS_JMPARG {
    jmp_start = (Dee_size_t)((iter-1)-begin);
    jmp_end = (Dee_size_t)((Dee_ssize_t)jmp_start+
                           (Dee_ssize_t)*(Dee_int16_t *)iter);
    DEE_ASSERTF(jmp_end <= start || jmp_end >= start+size,
                "Specified code marks the destination of a jump");
    // Relocate a jump from 'jmp_start' to 'jmp_end'
    if (jmp_start <= start && jmp_end >= start+size) {
     // Relocate from below to above (aka. subtract 'size')
     *(Dee_int16_t *)iter -= (Dee_int16_t)size;
    } else if (jmp_start >= start+size && jmp_end <= start) {
     // Relocate from above to below (aka. add 'size')
     *(Dee_int16_t *)iter += (Dee_int16_t)size;
    }
    iter += 2;
   } break;
   
   case OP_PREFIX: {
    Dee_uint16_t *significant_word_p = (*(Dee_uint16_t **)&iter)++;
    DEE_ASSERT(iter < end);
    switch (*iter++) {
     DEE_OPCODE_CASE_HAS_JMPARG {
      Dee_int32_t jmp_offset;
      jmp_offset = ((Dee_int32_t)*significant_word_p << 16) |
                    (Dee_int32_t)*(Dee_int16_t *)iter;
      jmp_start = (Dee_size_t)((iter-1)-begin);
      jmp_end = (Dee_size_t)((Dee_ssize_t)jmp_start+
                             (Dee_ssize_t)jmp_offset);
      DEE_ASSERTF(jmp_end <= start || jmp_end >= start+size,
                  "Specified code marks the destination of a jump");
      // Relocate a jump from 'jmp_start' to 'jmp_end'
      if (jmp_start <= start && jmp_end >= start+size) {
       // Relocate from below to above (aka. subtract 'size')
       DEE_ASSERT(jmp_offset >= (Dee_ssize_t)size);
       jmp_offset -= (Dee_int32_t)size;
update_32x_jmparg:
       *significant_word_p  = (Dee_uint16_t)((jmp_offset & DEE_INT32_C(0xFFFF0000)) >> 16);
       *(Dee_uint16_t*)iter = (Dee_uint16_t)(jmp_offset & DEE_INT32_C(0xFFFF));
      } else if (jmp_start >= start+size && jmp_end <= start) {
       // Relocate from above to below (aka. add 'size')
       DEE_ASSERT(jmp_offset <= -(Dee_ssize_t)size);
       jmp_offset += (Dee_int32_t)size;
       goto update_32x_jmparg;
      }
      iter += 2;
     } break;
     default:
      if ((op&OPFLAG_ARG)!=0) { iter += 2; DEE_ASSERT(iter <= end); }
      break;
    }
   } break;

   default:
    if ((op&OPFLAG_ARG)!=0) { iter += 2; DEE_ASSERT(iter <= end); }
    break;
  }
 }
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 { // Relocate exception handlers
  struct DeeExceptionHandlerEntry *xiter,*xend;
  xend = (xiter = self->cw_exceptv)+self->cw_exceptc;
  while (xiter != xend) {
   DEE_ASSERT(xiter->e_handler <= start || xiter->e_handler >= start+size);
   if (xiter->e_handler >= start+size) xiter->e_handler -= size;
   ++xiter;
  }
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

 // Relocate debug line and file information
 DeeCodeWriterDebugLnoTab_Relocate(&self->cw_debug.cwd_lno,start,size);
 DeeCodeWriterDebugLnoTab_Relocate(&self->cw_debug.cwd_fno.cwdft_lno,start,size);

 DEE_ASSERT(del_start == self->cw_code_begin+start);
 DEE_ASSERT(del_end == self->cw_code_begin+start+size);

 // Move all the code above into the specified area
 memmove(del_start,del_end,(Dee_size_t)(end-del_end)*sizeof(Dee_uint8_t));

 // Update the symbolic end of the code
 self->cw_code_pos -= size;

}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RemoveNoopOpcodes(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_uint8_t *begin,*iter,*end,op;
 DEE_ASSERT(self);
 iter = begin = self->cw_code_begin;
 end = self->cw_code_pos;
 while (iter != end) {
  op = *iter++;
  if (op == OP_NOOP) {
   Dee_uint8_t *noop_start = iter-1;
   Dee_size_t n_noops = 1;
   while (iter != end && *iter == OP_NOOP) ++iter,++n_noops;
   // Remove 'n_noops' bytes of code starting at 'noop_start'
   DEE_ASSERT(noop_start > begin);
   DeeCodeWriter_DelAndRelocateCode(self,(Dee_size_t)(noop_start-begin),n_noops);
   // Continue searching at the first non-noop opcode afterwards
   iter = noop_start;
   end -= n_noops;
  } else {
   if ((op&OPFLAG_ARG)!=0) {
    iter += 2;
    DEE_ASSERT(iter <= end);
   }
  }
 }
 return 0;
}


DEE_DECL_END
