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
// This doesn't really have to descrive no-op,
// nor is this macro used to determine which code should be deleted.
// A more appropriate name would be IS_M0P0,
// implying -0+0 stackeffect opcodes that don't perform any jumps
#define IS_NOOP(x) ((x)==OP_NOOP||(x)==OP_PRINT_END)
DEE_STATIC_INLINE(Dee_uint8_t *) _dee_after_noop(Dee_uint8_t *start) {
 while (IS_NOOP(*start)) ++start;
 return start;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DoPeepholeOptimizationWithProtectedAddrList(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT Dee_uint32_t *performed_optimizations,
 DEE_A_IN struct DeeCodeReachableAddrList const *protected_addr) {
 Dee_uint8_t *code,*code_begin,*code_end,
        *readpos,*writepos,before_opcode;
#ifdef DEE_DEBUG
 char const *cur_input,*cur_output;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(performed_optimizations);
 DEE_ASSERT(protected_addr);
 code_begin = self->cw_code_begin;
 code_end = self->cw_code_pos;
 code = code_begin;
 while (code != code_end) {
  DEE_ASSERT(code < code_end);
  if (!DeeCodeReachableAddrList_Contains(protected_addr,(Dee_size_t)(code-code_begin))) {
   before_opcode = *code;
   switch (before_opcode) {
#define TARGET(x) case x:
#define RT_DO_READ_BEGIN()  (readpos=code+1)
#define RT_DO_WRITE_BEGIN() (writepos=code)
#ifdef DEE_DEBUG
#define RT_WRITE_BEGIN()     (\
 DEE_LVERBOSE1R("%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : %q --> %q\n",\
                DeeCodeWriter_Addr2File(self,(Dee_size_t)(code-code_begin)),\
                DeeCodeWriter_Addr2Line(self,(Dee_size_t)(code-code_begin))+1,\
                (Dee_size_t)(code-code_begin),*code,cur_input,cur_output),RT_DO_WRITE_BEGIN())
#else
#define RT_WRITE_BEGIN       RT_DO_WRITE_BEGIN
#endif
#define RT_PROTECTED()       (DEE_UNLIKELY(readpos == code_end) || DeeCodeReachableAddrList_Contains(protected_addr,(Dee_size_t)(readpos-code_begin)))
#define RT_WRITE_OP(x)       do{ while(IS_NOOP(*writepos)) ++writepos; *writepos++ = (x); }while(0)
#define RT_WRITE_GETOP()     (*_dee_after_noop(writepos))
#define RT_WRITE_SKIPOP()    do{ while(IS_NOOP(*writepos)) ++writepos; ++writepos; }while(0)
//#define RT_WRITE_SKIPOPARG() do{ while(IS_NOOP(*writepos)) ++writepos; writepos += 3; }while(0)
#define RT_WRITE_ARG(x)      do{ *(Dee_uint16_t *)writepos=(x); writepos += 2; }while(0)
#define RT_WRITE_SKIPARG()   do{ writepos += 2; }while(0)
#define RT_SKIP_ARG()        (void)(readpos += 2)
#define RT_READ_ARG()        (*(*(Dee_uint16_t **)&readpos)++)
#define RT_READ_SARG()       (*(*(Dee_int16_t **)&readpos)++)
#define RT_SEEK_FIRST_OP()   do{ while (IS_NOOP(*readpos) && !RT_PROTECTED()) ++readpos; }while(0)
#define RT_NEXT_OP()         do{ ++readpos; while(IS_NOOP(*readpos) && !RT_PROTECTED()) ++readpos; }while(0)
#define RT_NEXT_OP_ARG()     do{ readpos += 3; }while(0)
#define RT_GET_OP()          (*readpos)
#define RT_GET_OPARG()       (*(Dee_uint16_t *)(readpos+1))
#ifdef DEE_DEBUG
#define RULE(input,output)   for (RT_DO_READ_BEGIN(),cur_input=(input),cur_output=(output);;)
#else
#define RULE(input,output)   for (RT_DO_READ_BEGIN();;)
#endif
#define RULE_BREAK()         break
#define DISPATCH()           goto next
#define DISPATCH_OPTIMIZED() goto next_after_optimize
#ifndef __INTELLISENSE__
#include "codewriter.peephole.generated.inl"
#endif
    {
     Dee_int16_t jmp_off;
     Dee_uint8_t *jmp_dst;
    case OP_JUMP_IF_TT: case OP_JUMP_IF_TT_POP:
    case OP_JUMP_IF_FF: case OP_JUMP_IF_FF_POP:
    case OP_JUMP: case OP_SEQ_ITER_WALK:
    case OP_JUMP_IF_CASE_COMPARE:
    case OP_JUMP_IF_RCASE_COMPARE:
     RT_DO_READ_BEGIN();
     jmp_off = RT_READ_SARG();
     if (before_opcode == OP_JUMP && jmp_off == sizeof(Dee_int16_t)) {
      // Jump points to the next opcode (can safely be removed)
      DEE_LVERBOSE1R("%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : Removing OP_JUMP to next address\n",
                     DeeCodeWriter_Addr2File(self,(Dee_size_t)(code-code_begin)),
                     DeeCodeWriter_Addr2Line(self,(Dee_size_t)(code-code_begin))+1,
                     (Dee_size_t)(code-code_begin),*code);
      RT_DO_WRITE_BEGIN();
      RT_WRITE_OP(OP_NOOP);
      RT_WRITE_OP(OP_NOOP);
      RT_WRITE_OP(OP_NOOP);
      DISPATCH_OPTIMIZED();
     }
     jmp_dst = code+jmp_off;
     DEE_ASSERT(jmp_dst >= code_begin && jmp_dst < code_end);
     if (before_opcode == OP_JUMP && (*jmp_dst == OP_RET || *jmp_dst == OP_RETNONE)) {
      // Jump to return --> Replace with return immediatly
      // -> Replace a jump to an OP_RET/OP_RETNONE with the return opcode
      DEE_LVERBOSE1R("%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : Forwarding jump to %s at +%.4Ix\n"
                     "%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : See reference to return opcode destination\n",
                     DeeCodeWriter_Addr2File(self,(Dee_size_t)(code-code_begin)),
                     DeeCodeWriter_Addr2Line(self,(Dee_size_t)(code-code_begin))+1,
                     (Dee_size_t)(code-code_begin),*code,(Dee_size_t)((code-code_begin)+(jmp_dst-code)),
                     *jmp_dst == OP_RET ? "OP_RET" : "OP_RETNONE",
                     DeeCodeWriter_Addr2File(self,(Dee_size_t)((code-code_begin)+(jmp_dst-code))),
                     DeeCodeWriter_Addr2Line(self,(Dee_size_t)((code-code_begin)+(jmp_dst-code)))+1,
                     (Dee_size_t)((code-code_begin)+(jmp_dst-code)),*jmp_dst);
      code[0] = *jmp_dst;
      code[1] = OP_NOOP;
      code[2] = OP_NOOP;
      DISPATCH_OPTIMIZED();
     }
     if (*jmp_dst == OP_JUMP) {
      Dee_ssize_t jmp_offset;
      // Jump points to another jump (forward to the destination of that jump)
      jmp_dst += *(Dee_int16_t *)(jmp_dst+1);
      DEE_ASSERT(jmp_dst >= code_begin && jmp_dst < code_end);
      jmp_offset = jmp_dst-code;
      // Make sure this jump wasn't just split because it would be too large
      if (jmp_offset >= -32768 && jmp_offset <= 32767) {
       DEE_LVERBOSE1R("%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : Forwarding jump to +%.4Ix\n"
                      "%s(%I64d) : PEEPHOLE : +%.4Ix : %#.2I8x : See reference to forward destination\n",
                      DeeCodeWriter_Addr2File(self,(Dee_size_t)(code-code_begin)),
                      DeeCodeWriter_Addr2Line(self,(Dee_size_t)(code-code_begin))+1,
                      (Dee_size_t)(code-code_begin),*code,(Dee_size_t)((code-code_begin)+jmp_offset),
                      DeeCodeWriter_Addr2File(self,(Dee_size_t)((code-code_begin)+jmp_offset)),
                      DeeCodeWriter_Addr2Line(self,(Dee_size_t)((code-code_begin)+jmp_offset))+1,
                      (Dee_size_t)((code-code_begin)+jmp_offset),code[jmp_offset]);
       *(Dee_int16_t *)(code+1) = (Dee_int16_t)jmp_offset;
       DISPATCH_OPTIMIZED();
      }
     }
     DISPATCH();
    }
#undef TARGET
#undef RT_DO_READ_BEGIN
#undef RT_DO_WRITE_BEGIN
#undef RT_WRITE_BEGIN
#undef RT_WRITE_OP
#undef RT_WRITE_GETOP
#undef RT_WRITE_SKIPOP
#undef RT_WRITE_ARG
#undef RT_WRITE_SKIPARG
#undef RT_SKIP_ARG
#undef RT_READ_ARG
#undef RT_READ_SARG
#undef RT_NEXT_OP
#undef RT_NEXT_OP_ARG
#undef RT_PROTECTED
#undef RT_GET_OP
#undef RULE
#undef RULE_BREAK
#undef DISPATCH
#undef DISPATCH_OPTIMIZED
    default: goto next;
   }
next_after_optimize:
   ++*performed_optimizations;
   if ((*++code&OPFLAG_ARG)!=0) code += 2;
  } else {
   before_opcode = *code;
next:
   DEE_ASSERTF(before_opcode == *code,
               "Code modified without calling DISPATCH_OPTIMIZED()");
   if ((before_opcode&OPFLAG_ARG)!=0) code += 3; else ++code;
  }
 }
 return 0;
}


DEE_DECL_END
