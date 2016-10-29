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




#define LOGF(...) \
 (DEE_LVERBOSE1R("%s(%d) : PEEPHOLE : +%.4Ix : ",\
  DeeCodeWriter_Addr2File(self,PTR2ADDR(opcode)),\
  DeeCodeWriter_Addr2Line(self,PTR2ADDR(opcode))+1,\
  PTR2ADDR(opcode)),DEE_LVERBOSE1R(__VA_ARGS__))
#define SKIP_NOOP(ptr) \
 while (*(Dee_uint8_t *)(ptr) == OP_NOOP) \
 if((*(Dee_uint8_t **)&(ptr))++ == code_end) return 0;else;






// TODO: pop_store x + load y + load x  --> store x + load y + rot 2
// TODO: if *1: jmp x; *2 ... x: *2 ... --> *2; if *1: jmp x; ... x: ...
//       (Move duplicate code out from inside of a jump)

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DoPeepholeOptimization(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_INOUT Dee_uint32_t *performed_optimizations) {
 int result;
 struct DeeCodeReachableAddrList pro_addr = DeeCodeReachableAddrList_INIT();
 if (DeeCodeReachableAddrList_CollectFromCodewriter(&pro_addr,self) == 0)
  result = DeeCodeWriter_DoPeepholeOptimizationWithProtectedAddrList(self,performed_optimizations,&pro_addr);
 else result = -1;
 DeeCodeReachableAddrList_Quit(&pro_addr);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DoPeepholeOptimizationWithProtectedAddrList(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT Dee_uint32_t *performed_optimizations,
 DEE_A_IN struct DeeCodeReachableAddrList const *protected_addr) {
 Dee_uint8_t *code,*code_begin,*code_end;
 Dee_uint8_t *opcode;
 Dee_uint16_t *oparg;
 DEE_ASSERT(self);
 DEE_ASSERT(performed_optimizations);
 DEE_ASSERT(protected_addr);
 code_begin = self->cw_code_begin;
 code_end = self->cw_code_pos;
 code = code_begin;
 if (code != code_end) while (1) {
  DEE_ASSERT(code < code_end);
  opcode = code++;
  if ((*opcode&OPFLAG_ARG)==0) oparg = NULL;
  else oparg = (*(Dee_uint16_t **)&code)++;
  if (code == code_end) break;
  SKIP_NOOP(code);
#define IS_PROTECTED(addr)            DeeCodeReachableAddrList_Contains(protected_addr,addr)
#define IS_PROTECTED_RANGE(begin,end) DeeCodeReachableAddrList_ContainsAny(protected_addr,begin,end)
#define ASSERT_ADDRESS(addr)\
 DEE_ASSERTF((Dee_uint8_t *)(addr) >= code_begin && \
             (Dee_uint8_t *)(addr) < code_end,\
             "Address %+Id is out of bounds",\
             (Dee_uint8_t *)(addr)-code_begin)
#define PTR2ADDR(p)           (Dee_size_t)(((Dee_uint8_t *)(p))-code_begin)
#define ADDR2PTR(a)           (code_begin+(Dee_size_t)(a))
#define ARG_OF_OPADDR(opaddr) (*(Dee_uint16_t*)((Dee_uint8_t*)(opaddr)+1))
#define BYTE(i)               opcode[i]
#define NEXT_BYTE(i)          next_opcode[i]
#define oparg_0               ((Dee_uint8_t *)oparg+0)
#define oparg_1               ((Dee_uint8_t *)oparg+1)
#define ADDR                  (Dee_size_t)(opcode-code_begin)
#define next_opcode           code
#define next_oparg            ((Dee_uint16_t *)(code+1))
// Dee_uint8_t *JMP_ADDRPTR; --> The absolute code address of a relative jump
#define JMP_ADDRPTR           (opcode+(Dee_ssize_t)*(Dee_int16_t *)oparg)
  switch (*opcode) {

   case OP_LOAD_NONE:
    if (*next_opcode == OP_RET &&
        !IS_PROTECTED(PTR2ADDR(next_opcode))) {
     // OP_LOAD_NONE, OP_RET --> OP_RETNONE
     *opcode = OP_RETNONE;
     *next_opcode = OP_NOOP;
add_opt:
     ++*performed_optimizations;
    }
    break;

   case OP_LOAD_LOC:
   case OP_LOAD_REF:
   case OP_LOAD_CST:
   case OP_LOAD_CST_COPY:
   case OP_LOAD_CST_DCOPY:
   case OP_LOAD_BLTIN:
   case OP_LOAD_ARG:
    if (*next_opcode == OP_POP &&
        !IS_PROTECTED(PTR2ADDR(next_opcode))) {
     // Load something, immediately pop it --> remove the load+pop
     LOGF("Removing load followed by OP_POP\n");
     BYTE(0) = OP_NOOP;
     BYTE(1) = OP_NOOP;
     BYTE(2) = OP_NOOP;
     NEXT_BYTE(0) = OP_NOOP;
     goto add_opt;
    } else if (*next_oparg == *opcode && *next_oparg == *oparg &&
               !IS_PROTECTED(PTR2ADDR(next_opcode))) {
     // TODO: This optimization should be recursive
     // Load something twice --> load once, then dup
     LOGF("Replaced repeated load with OP_DUP\n");
     NEXT_BYTE(0) = OP_DUP;
     NEXT_BYTE(1) = OP_NOOP;
     NEXT_BYTE(2) = OP_NOOP;
     goto add_opt;
    }
    break;

   case OP_JUMP: {
    Dee_uint8_t *jmp_dest;
    if (*oparg == sizeof(Dee_uint8_t)+sizeof(Dee_uint16_t)) {
     // Jump points to the next opcode (can safely be removed)
     LOGF("Removing jmp to next address\n");
     BYTE(0) = OP_NOOP;
     BYTE(1) = OP_NOOP;
     BYTE(2) = OP_NOOP;
     goto add_opt;
    }
   case OP_JUMP_IF_TT:
   case OP_JUMP_IF_FF:
   case OP_JUMP_IF_TT_POP:
   case OP_JUMP_IF_FF_POP:
    jmp_dest = JMP_ADDRPTR;
    ASSERT_ADDRESS(jmp_dest);
    if (*jmp_dest == OP_JUMP) {
     Dee_ssize_t jmp_offset;
     // Jump points to another jump (forward to the destination of that jump)
     jmp_dest += ARG_OF_OPADDR(jmp_dest);
     jmp_offset = jmp_dest-opcode;
     // Make sure this jump wasn't just split because it would be too large
     if (jmp_offset >= -32768 && jmp_offset <= 32767) {
      LOGF("Forwarding jmp %Ix to %Ix\n",
           PTR2ADDR(JMP_ADDRPTR),(Dee_size_t)((Dee_ssize_t)ADDR+jmp_offset));
      *(Dee_int16_t *)oparg = (Dee_int16_t)jmp_offset;
      goto add_opt;
     }
    }
   } break;

   case OP_DUP: {
    switch (*next_opcode) {
     // Unary opcodes with a stack-effect of '-1' (basic)
     // e.g.: 'yield pop(0)' applies for this (-1), but not 'push(pop(1)+pop(0))' (-2+1)
     case OP_YIELD:
     case OP_PRINT_ONE:
     case OP_PRINT_ONE_SEP:
     case OP_PRINT_ONE_END:
     case OP_PRINT_ALL:
     case OP_PRINT_ALL_SEP:
     case OP_PRINT_ALL_END:
     case OP_PRINT_END_F: {
      Dee_uint8_t *after_next_opcode;
      after_next_opcode = next_opcode+1;
      SKIP_NOOP(after_next_opcode);
      if (*after_next_opcode == OP_POP) {
       if (IS_PROTECTED_RANGE(PTR2ADDR(opcode),PTR2ADDR(after_next_opcode)+1)) break;
       // Optimize:
       // +0 dup top     // +0 noop
       // +1 yield pop 1 // +1 yield pop 1
       // +2 pop 1       // +2 noop
       *opcode = OP_NOOP;
       *after_next_opcode = OP_NOOP;
       LOGF("OP_DUP + unary|basic(-1) + OP_POP --> unary|basic(-1)\n");
       goto add_opt;
      }
     } break;

     case OP_POP:
      if (IS_PROTECTED(PTR2ADDR(next_opcode))) break;
      // dup, pop --> cancel out each other
      LOGF("OP_DUP + OP_POP --> OP_NOOP\n");
      *opcode = OP_NOOP;
      *next_opcode = OP_NOOP;
      goto add_opt;

     case OP_ROT_2:
      if (IS_PROTECTED(PTR2ADDR(next_opcode))) break;
      // dup, rot 2 --> rotation is unnecessary
      LOGF("Removing OP_ROT_2 after OP_DUP\n");
      *next_opcode = OP_NOOP;
      goto add_opt;

     default: break;
    }
   } break;

   case OP_ROT_2:
    if (*next_opcode == OP_ROT_2 &&
        !IS_PROTECTED(PTR2ADDR(next_opcode))) {
     LOGF("Removing OP_ROT_2 after OP_ROT_2\n");
     *opcode = OP_NOOP;
     *next_opcode = OP_NOOP;
     goto add_opt;
    }
    break;

   // store + pop --> store_pop
   case OP_STORE_LOC: if (*next_opcode == OP_POP && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_LOC + OP_POP --> OP_STORE_LOC_POP\n"); *opcode = OP_STORE_LOC_POP; *next_opcode = OP_NOOP; goto add_opt; } break;
   case OP_STORE_ARG: if (*next_opcode == OP_POP && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_ARG + OP_POP --> OP_STORE_ARG_POP\n"); *opcode = OP_STORE_ARG_POP; *next_opcode = OP_NOOP; goto add_opt; } break;
   case OP_STORE_CST: if (*next_opcode == OP_POP && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_CST + OP_POP --> OP_STORE_CST_POP\n"); *opcode = OP_STORE_CST_POP; *next_opcode = OP_NOOP; goto add_opt; } break;
   case OP_STORE_RET: if (*next_opcode == OP_POP && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_RET + OP_POP --> OP_STORE_RET_POP\n"); *opcode = OP_STORE_RET_POP; *next_opcode = OP_NOOP; goto add_opt; } break;

    // Generate sequence T1+ cast T2 --> generate sequence T2
   case OP_TUPLE:
    if (!IS_PROTECTED(PTR2ADDR(next_opcode))) switch (*next_opcode) {
     case OP_CAST_TUPLE: *next_opcode = OP_NOOP; LOGF("Removing OP_CAST_TUPLE after \n"); goto add_opt;
     case OP_CAST_LIST: *opcode = OP_LIST; *next_opcode = OP_NOOP; LOGF("OP_TUPLE + OP_CAST_LIST --> OP_LIST\n"); goto add_opt;
     case OP_CAST_SET: *opcode = OP_SET; *next_opcode = OP_NOOP; LOGF("OP_TUPLE + OP_CAST_SET --> OP_SET\n"); goto add_opt;
     default: break;
    }
    break;
   case OP_LIST:
    if (!IS_PROTECTED(PTR2ADDR(next_opcode))) switch (*next_opcode) {
     case OP_CAST_TUPLE: *opcode = OP_TUPLE; *next_opcode = OP_NOOP; LOGF("OP_LIST + OP_CAST_TUPLE --> OP_TUPLE\n"); goto add_opt;
     case OP_CAST_LIST: *next_opcode = OP_NOOP; LOGF("OP_LIST + OP_CAST_LIST --> OP_LIST\n"); goto add_opt;
     case OP_CAST_SET: *opcode = OP_SET; *next_opcode = OP_NOOP; LOGF("OP_LIST + OP_CAST_SET --> OP_SET\n"); goto add_opt;
     default: break;
    }
    break;
   case OP_SET:
    if (!IS_PROTECTED(PTR2ADDR(next_opcode))) switch (*next_opcode) {
     case OP_CAST_TUPLE: *opcode = OP_TUPLE; *next_opcode = OP_NOOP; LOGF("OP_SET + OP_CAST_TUPLE --> OP_TUPLE\n"); goto add_opt;
     case OP_CAST_LIST: *opcode = OP_LIST; *next_opcode = OP_NOOP; LOGF("OP_SET + OP_CAST_LIST --> OP_LIST\n"); goto add_opt;
     case OP_CAST_SET: *next_opcode = OP_NOOP; LOGF("OP_SET + OP_CAST_SET --> OP_SET\n"); goto add_opt;
     default: break;
    }
    break;

   // store_pop x, load x --> store x (keep the variable on the stack)
   case OP_STORE_LOC_POP: if (*next_opcode == OP_LOAD_LOC && *next_oparg == *oparg && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_LOC_POP + OP_LOAD_LOC --> OP_STORE_LOC\n"); *opcode = OP_STORE_LOC; NEXT_BYTE(0) = OP_NOOP; NEXT_BYTE(1) = OP_NOOP; NEXT_BYTE(2) = OP_NOOP; goto add_opt; } break;
   case OP_STORE_ARG_POP: if (*next_opcode == OP_LOAD_ARG && *next_oparg == *oparg && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_ARG_POP + OP_LOAD_ARG --> OP_STORE_ARG\n"); *opcode = OP_STORE_ARG; NEXT_BYTE(0) = OP_NOOP; NEXT_BYTE(1) = OP_NOOP; NEXT_BYTE(2) = OP_NOOP; goto add_opt; } break;
   case OP_STORE_CST_POP: if (*next_opcode == OP_LOAD_CST && *next_oparg == *oparg && !IS_PROTECTED(PTR2ADDR(next_opcode))) { LOGF("OP_STORE_CST_POP + OP_LOAD_CST --> OP_STORE_CST\n"); *opcode = OP_STORE_CST; NEXT_BYTE(0) = OP_NOOP; NEXT_BYTE(1) = OP_NOOP; NEXT_BYTE(2) = OP_NOOP; goto add_opt; } break;

   default: break;
  }
#undef JMP_ADDRPTR
#undef next_opcode
#undef oparg_1
#undef oparg_0
#undef NEXT_BYTE
#undef BYTE
#undef PTR2ADDR
#undef ADDR2PTR
#undef ASSERT_ADDRESS
#undef IS_PROTECTED_RANGE
#undef IS_PROTECTED
#undef ADDR
 }
 return 0;
}
#undef LOGF


DEE_DECL_END
