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


DEE_A_RET_WUNUSED int DeeCodeReachableAddrList_Contains(
 DEE_A_IN struct DeeCodeReachableAddrList const *self, DEE_A_IN Dee_size_t addr) {
 Dee_size_t *posv; double likely_offset;
 DEE_ASSERT(self);
 if (addr < self->ra_begin || addr > self->ra_end) return 0;
 if (addr == self->ra_begin || addr == self->ra_end) return 1;
 likely_offset = (double)(addr-self->ra_begin)/(self->ra_end-self->ra_begin);
 posv = self->ra_listv+(Dee_size_t)(likely_offset*self->ra_listc);
 DEE_ASSERT(posv >= self->ra_listv && posv < self->ra_listv+self->ra_listc);
 if (*posv < addr) { do ++posv; while (*posv < addr); return *posv == addr; }
 if (*posv > addr) { do --posv; while (*posv > addr); return *posv == addr; }
 DEE_ASSERT(*posv == addr);
 return 1;
}
DEE_A_RET_WUNUSED int DeeCodeReachableAddrList_ContainsAny(
 DEE_A_IN struct DeeCodeReachableAddrList const *self,
 DEE_A_IN Dee_size_t addr_begin, DEE_A_IN Dee_size_t addr_end) {
 Dee_size_t *posv,addr_avg; double likely_offset;
 DEE_ASSERT(self); DEE_ASSERT(addr_begin <= addr_end);
 if (addr_end < self->ra_begin || addr_begin >= self->ra_end) return 0;
 addr_avg = addr_begin+(addr_end-addr_begin)/2;
 likely_offset = (double)(addr_avg-self->ra_begin)/(self->ra_end-self->ra_begin);
 posv = self->ra_listv+(Dee_size_t)(likely_offset*self->ra_listc);
 DEE_ASSERT(posv >= self->ra_listv && posv < self->ra_listv+self->ra_listc);
 if (*posv < addr_begin) { do ++posv; while (*posv < addr_begin); return *posv >= addr_begin && *posv < addr_end; }
 if (*posv > addr_end)   { do --posv; while (*posv > addr_end);   return *posv >= addr_begin && *posv < addr_end; }
 DEE_ASSERT(*posv >= addr_begin && *posv < addr_end);
 return 1;
}

#ifdef DEE_DEBUG
#define DEBUG_CHECK_INTEGRITY(ob)\
do{\
 if ((ob)->ra_listc >= 2) {\
  Dee_size_t *check_pos;\
  check_pos = (ob)->ra_listv+((ob)->ra_listc-1);\
  while (check_pos > (ob)->ra_listv) {\
   DEE_ASSERT(check_pos[0] > check_pos[-1]);\
   --check_pos;\
  }\
 }\
}while(0)
#else
#define DEBUG_CHECK_INTEGRITY(ob) (void)0
#endif

DEE_A_RET_EXCEPT(-1) int DeeCodeReachableAddrList_Insert(
 DEE_A_INOUT struct DeeCodeReachableAddrList *self, DEE_A_IN Dee_size_t addr) {
 Dee_size_t *posv; double likely_offset;
 DEE_ASSERT(self);
 if (self->ra_listc == self->ra_lista) {
  if (!self->ra_lista) self->ra_lista = 2; else self->ra_lista *= 2;
  while ((posv = (Dee_size_t *)realloc_nz(
   self->ra_listv,self->ra_lista*sizeof(Dee_size_t))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->ra_listv = posv;
 }
 DEBUG_CHECK_INTEGRITY(self);
 if (addr <= self->ra_begin) {
  if (addr == self->ra_begin) return 0;
  if DEE_UNLIKELY(addr > self->ra_end)
   self->ra_end = addr; // This is the case for the first added address
  // Insert at the start
  self->ra_begin = addr;
  memmove(self->ra_listv+1,self->ra_listv,
          self->ra_listc*sizeof(Dee_size_t));
  self->ra_listv[0] = addr;
  ++self->ra_listc;
  DEBUG_CHECK_INTEGRITY(self);
  return 0;
 }
 if (addr >= self->ra_end) {
  if (addr == self->ra_end) return 0;
  // Insert at the end
  self->ra_end = addr;
  self->ra_listv[self->ra_listc++] = addr;
  DEBUG_CHECK_INTEGRITY(self);
  return 0;
 }
 DEE_ASSERT(self->ra_listc >= 2);
 DEBUG_CHECK_INTEGRITY(self);
 likely_offset = (double)(addr-self->ra_begin)/(self->ra_end-self->ra_begin);
 posv = self->ra_listv+(Dee_size_t)(likely_offset*self->ra_listc);
 DEE_ASSERT(posv >= self->ra_listv && posv < self->ra_listv+self->ra_listc);
 if (*posv < addr) { do ++posv; while (*posv < addr); }
 else if (posv[-1] > addr) { do --posv; while (posv[-1] > addr); }
 DEE_ASSERT(posv >= self->ra_listv && posv < self->ra_listv+self->ra_listc);
 if (*posv == addr || posv[-1] == addr) return 0; // Address already in use
 memmove(posv+1,posv,(self->ra_listc-(posv-self->ra_listv))*sizeof(Dee_size_t));
 *posv = addr;
 ++self->ra_listc;
 DEBUG_CHECK_INTEGRITY(self);
 return 0;
}
#undef DEBUG_CHECK_INTEGRITY

DEE_A_RET_EXCEPT(-1) int DeeCodeReachableAddrList_CollectFromCodewriter(
 DEE_A_INOUT struct DeeCodeReachableAddrList *self,
 DEE_A_IN struct DeeCodeWriter const *writer) {
 Dee_uint8_t *iter,*end;
 Dee_size_t jmp_dest;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 iter = writer->cw_code_begin;
 end = writer->cw_code_pos;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 { // Protect exception handlers
  struct DeeExceptionHandlerEntry *except_iter,*except_end;
  except_end = (except_iter = writer->cw_exceptv)+writer->cw_exceptc;
  while (except_iter != except_end) {
   if (DeeCodeReachableAddrList_Insert(self,except_iter->e_handler) != 0) return -1;
   ++except_iter;
  }
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 while (iter != end) {
  DEE_ASSERT(iter < end);
  switch (*iter++) {
   DEE_OPCODE_CASE_HAS_JMPARG
    // v ignore jumps to the next address
    if (*(Dee_int16_t *)iter != sizeof(Dee_uint8_t)+sizeof(Dee_uint16_t)) {
     jmp_dest = (Dee_size_t)(((iter-1)-writer->cw_code_begin)+(Dee_ssize_t)*(Dee_int16_t *)iter);
     DEE_ASSERTF(jmp_dest < (Dee_size_t)(writer->cw_code_pos-writer->cw_code_begin),
                 "Jump destination is out-of-bounds (%Iu exceeds %Iu)",
                 jmp_dest,(Dee_size_t)(writer->cw_code_pos-writer->cw_code_begin));
     if (DeeCodeReachableAddrList_Insert(self,jmp_dest) != 0) return -1;
     iter += sizeof(Dee_int16_t);
    }
    break;
   case OP_PREFIX: {
    Dee_int32_t jump_offset; // Long jumps
    Dee_uint16_t significant_word = *(*(Dee_uint16_t **)&iter)++;
    DEE_ASSERT(iter < end);
    switch (*iter++) {
     DEE_OPCODE_CASE_HAS_JMPARG
      jump_offset = (Dee_int32_t)((Dee_uint32_t)significant_word << 16 | *(Dee_int16_t *)iter);
      jmp_dest = (Dee_size_t)(((iter-4)-writer->cw_code_begin)+(Dee_ssize_t)jump_offset);
      DEE_ASSERTF(jmp_dest < (Dee_size_t)(writer->cw_code_pos-writer->cw_code_begin),
                  "Jump destination is out-of-bounds (%Iu exceeds %Iu)",
                  jmp_dest,(Dee_size_t)(writer->cw_code_pos-writer->cw_code_begin));
      if (DeeCodeReachableAddrList_Insert(self,jmp_dest) != 0) return -1;
      iter += sizeof(Dee_int16_t);
      break;
     default:
      if ((iter[-1]&OPFLAG_ARG)!=0) iter += sizeof(Dee_int16_t);
      break;
    }
   } break;
   default:
    if ((iter[-1]&OPFLAG_ARG)!=0) iter += sizeof(Dee_int16_t);
    break;
  }
 }
 return 0;
}

DEE_DECL_END
