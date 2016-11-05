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

#define LOGF(...) \
 (DEE_LVERBOSE1R("%s(%d) : PEEPHOLE : +%.4Ix : ",\
  DeeCodeWriter_Addr2File(self,PTR2ADDR(opcode)),\
  DeeCodeWriter_Addr2Line(self,PTR2ADDR(opcode))+1,\
  PTR2ADDR(opcode)),DEE_LVERBOSE1R(__VA_ARGS__))
#define SKIP_NOOP(ptr) \
 while (*(Dee_uint8_t *)(ptr) == OP_NOOP) \
 if((*(Dee_uint8_t **)&(ptr))++ == code_end) return 0;else;

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
