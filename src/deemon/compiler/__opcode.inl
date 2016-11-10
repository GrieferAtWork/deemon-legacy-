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
#ifndef GUARD_DEEMON___OPCODE_INL
#define GUARD_DEEMON___OPCODE_INL 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API

//////////////////////////////////////////////////////////////////////////
// Note about comments in this file:
// Comments are usually written in deemon-style code, provided with the following env:
//  >> stack = [];
//  >> function push(ob) -> stack.append(ob);
//  >> // NOTE: Please not that pop ids always referr to index offsets when the comment started
//  >> //       e.g.: 'xxx(pop(0),pop(1))' 
//  >> //             - assuming 'stack == [10,20,30]'
//  >> //               'pop(0) == 30' and 'pop(1) == 20',
//  >> function pop(id) -> stack.pop(#stack-(id+1));
// - unsigned(ARG) - Only available for 'OPFLAG_ARG' operators - the 16-bit argument interpreted as unsigned
// - signed(ARG)   - Only available for 'OPFLAG_ARG' operators - the 16-bit argument interpreted as signed
#define OP_RETNONE                0x00 // return none;
#define OP_NOOP                   0x01 // // no-op (just skip it)
#define OP_POP                    0x02 // pop(0)
#define OP_RET                    0x03 // return pop(0)
#define OP_STOP                   0x04 // [internal only] Simply stop execution (don't override return value)
#define OP_RETVAREXIT             0x05 // return; // In a result variable function (set return value to none if it wasn't set before)
#define OP_YIELD                  0x06 // yield pop(0);
#define OP_YIELDALL               0x07 // try { x = top; while (1) yield x.__iternext__(); } catch (Signal.StopIteration) { } pop(1);
#define OP_YIELDSTOP              0x08 // return; // In a yield function
#define OP_LOAD_NONE              0x09 // push(none)
#define OP_COPY                   0x0A // push(copy pop(0))
#define OP_MOVE                   0x0B // push(move pop(0))
#define OP_TYPE                   0x0C // push(type pop(0))
#define OP_BOOL                   0x0D // push(!!pop(0)) // NOTE: Internal bool operator (string is true if it's non-empty)
#define OP_WEAK                   0x0E // push(weak pop(0))
#define OP_IS                     0x0F // push(pop(1) is pop(0))
#define OP_IN                     0x10 // push(pop(1) in pop(0))
#define OP_CALL                   0x11 // push(pop(1)(pop(0)...))
#define OP_COPY_ASSIGN            0x12 // push(pop(1) = pop(0))
#define OP_STR                    0x13 // push(str(pop(0)))
#define OP_REPR                   0x14 // push(repr(pop(0)))
#define OP_NOT                    0x15 // push(!pop(0))
#define OP_POS                    0x16 // push(+pop(0))
#define OP_NEG                    0x17 // push(-pop(0))
#define OP_REF                    0x18 // push(&pop(0))
#define OP_DEREF                  0x19 // push(*pop(0))
#define OP_INV                    0x1A // push(-pop(0))
#define OP_INC                    0x1B // push(++pop(0))
#define OP_DEC                    0x1C // push(--pop(0))
#define OP_INC_POST               0x1D // push(pop(0)++)
#define OP_DEC_POST               0x1E // push(pop(0)--)
#define OP_ADD                    0x1F // push(pop(1)+pop(0))
#define OP_INPLACE_ADD            0x20 // push(pop(1)+=pop(0))
#define OP_SUB                    0x21 // push(pop(1)-pop(0))
#define OP_INPLACE_SUB            0x22 // push(pop(1)-=pop(0))
#define OP_MUL                    0x23 // push(pop(1)*pop(0))
#define OP_INPLACE_MUL            0x24 // push(pop(1)*=pop(0))
#define OP_DIV                    0x25 // push(pop(1)/pop(0))
#define OP_INPLACE_DIV            0x26 // push(pop(1)/=pop(0))
#define OP_MOD                    0x27 // push(pop(1)%pop(0))
#define OP_INPLACE_MOD            0x28 // push(pop(1)%=pop(0))
#define OP_SHL                    0x29 // push(pop(1)<<pop(0))
#define OP_INPLACE_SHL            0x2A // push(pop(1)<<=pop(0))
#define OP_SHR                    0x2B // push(pop(1)>>pop(0))
#define OP_INPLACE_SHR            0x2C // push(pop(1)>>=pop(0))
#define OP_AND                    0x2D // push(pop(1)&pop(0))
#define OP_INPLACE_AND            0x2E // push(pop(1)&=pop(0))
#define OP_OR                     0x2F // push(pop(1)|pop(0))
#define OP_INPLACE_OR             0x30 // push(pop(1)|=pop(0))
#define OP_XOR                    0x31 // push(pop(1)^pop(0))
#define OP_INPLACE_XOR            0x32 // push(pop(1)^=pop(0))
#define OP_POW                    0x33 // push(pop(1)**pop(0))
#define OP_INPLACE_POW            0x34 // push(pop(1)**=pop(0))
#define OP_CMP_LO                 0x35 // push(pop(1)<pop(0))
#define OP_CMP_LE                 0x36 // push(pop(1)<=pop(0))
#define OP_CMP_EQ                 0x37 // push(pop(1)==pop(0))
#define OP_CMP_NE                 0x38 // push(pop(1)!=pop(0))
#define OP_CMP_GR                 0x39 // push(pop(1)>pop(0))
#define OP_CMP_GE                 0x3A // push(pop(1)>=pop(0))
#define OP_SEQ_GET                0x3B // push(pop(1)[pop(0)])
#define OP_SEQ_DEL                0x3C // del pop(1)[pop(0)]
#define OP_SEQ_SET                0x3D // push(pop(2)[pop(1)] = pop(0))
#define OP_SEQ_SIZE               0x3E // push(len(pop(0)))
#define OP_SEQ_RANGE_GET          0x3F // push(pop(2)[pop(1):pop(0)])
#define OP_SEQ_RANGE_DEL          0x40 // del pop(2)[pop(1):pop(0)]
#define OP_SEQ_RANGE_SET          0x41 // push(pop(3)[pop(2):pop(1)] = pop(0))
#define OP_SEQ_ITER_SELF          0x42 // push(__iterself__(pop(0)))
#define OP_ATTR_GET               0x43 // push(__getattr__(pop(1),pop(0)))
#define OP_ATTR_DEL               0x44 // __delattr__(pop(1),pop(0))
#define OP_ATTR_SET               0x45 // push(__setattr__(pop(2),pop(0),pop(1)))
#define OP_ROT_2                  0x46 // rotate top 2 stack entries: 'ab' -> 'ba'
#define OP_LROT_3                 0x47 // rotate top 3 stack entries: 'abc' -> 'bca'
#define OP_TRY_END                0x48 // Pop the last exception hander from the stack of active handlers (NOTE: If it's a finally handler, it is executed before returning to the opcode following this one, unless an error occurred in the finally-block)
#define OP_EXCEPT_END             0x49 // Written at the end of exception handling code (cleans up the exception chain by dropping the exception currently being handled)
#define OP_FINALLY_END            0x4A // Written at the end of finally handling code (rethrows finally exceptions and returns to the address this finally block was executed from)
#define OP_THROW                  0x4B // throw pop(0)
#define OP_RETHROW                0x4C // throw
#define OP_CONCAT_TUPLE           0x4D // push(pop(1)+pop(0))
#define OP_CONCAT_LIST            0x4E // push(pop(1)+pop(0))
#define OP_CAST_LIST              0x4F // push(list(pop(0)))
#define OP_CAST_TUPLE             0x50 // push(tuple(pop(0)))
//UNUSED:                         0x51
#define OP_LOAD_RET               0x52 // function: result { push(result); }
#define OP_STORE_RET              0x53 // function: result { result = top; }
#define OP_STORE_RET_POP          0x54 // function: result { result = pop(0); }
// Throws an error if reached (this op-code is only generated in debug mode by default)
// NOTE: Finally blocks from the stack frame that encountered this opcode will not be executed
//      (It's barely possible to recover from this without having to execute user code...)
#define OP_UNREACHABLE            0x55 // __builtin_unreachable();
#define OP_LOAD_THIS              0x56 // function [[__thiscall__]] { push(this); }
#define OP_DUP                    0x57 // push(top)
#define OP_JUMP_DYN_REL           0x58 // jump ssize_t(pop(0)) bytes, starting at the address of this opcode
#define OP_JUMP_DYN_ABS           0x59 // jump to size_t(pop(0)), starting at the co_code address of the associated code
//UNUSED:                         0x5A
//UNUSED:                         0x5B
#define OP_PRINT_ONE              0x5C // print pop(0),;
#define OP_PRINT_ONE_SEP          0x5D // print pop(0),"",;
#define OP_PRINT_ONE_END          0x5E // print pop(0);
#define OP_PRINT_ONE_F            0x5F // print pop(1): pop(0),;
#define OP_PRINT_ONE_F_SEP        0x60 // print second: pop(0),"",;
#define OP_PRINT_ONE_F_END        0x61 // print pop(1): pop(0);
#define OP_PRINT_ALL              0x62 // print pop(0)...,;
#define OP_PRINT_ALL_SEP          0x63 // print pop(0)...,"",;
#define OP_PRINT_ALL_END          0x64 // print pop(0)...;
#define OP_PRINT_ALL_F            0x65 // print pop(1): pop(0)...,;
#define OP_PRINT_ALL_F_SEP        0x66 // print second: pop(0)...,"",;
#define OP_PRINT_ALL_F_END        0x67 // print pop(1): pop(0)...;
#define OP_PRINT_END              0x68 // print;
#define OP_PRINT_END_F            0x69 // print pop(0):;
#define OP_NEW_INSTANCE           0x6A // push(pop(1)(pop(0)...)) // same as OP_CALL, but only for types (NOTE: pop(1) MUST be a tuple; this is an assertion)
#define OP_CELL                   0x6B // push(<pop(0)>)
#define OP_LROT_4                 0x6C // rotate top 4 stack entries: 'abcd' -> 'bcda'
#define OP_ATTR_HAS               0x6D // push(__builtin_hasattr(pop(1),pop(0)))
#define OP_MOVE_ASSIGN            0x6E // push(pop(1).operator move = (pop(0)))
#define OP_RROT_3                 0x6F // rotate top 3 stack entries: 'abc' -> 'cab'
#define OP_RROT_4                 0x70 // rotate top 4 stack entries: 'abcd' -> 'dabc'
#define OP_CAST_SET               0x71 // push(set(pop(0)))
#define OP_LOAD_EXTERN            0x72 // push(__builtin_extern(pop(2),pop(1),pop(0)))
#define OP_LOAD_AUTOEXTERN        0x73 // push(__builtin_extern(pop(1),pop(0)))
#define OP_FF_CLOSURE             0x74 // push(__builtin_ff_closure(pop(1),pop(0)))
//UNUSED:                         0x75
//UNUSED:                         0x76
//UNUSED:                         0x77
//UNUSED:                         0x78
//UNUSED:                         0x79
//UNUSED:                         0x7A
//UNUSED:                         0x7B
//UNUSED:                         0x7C
//UNUSED:                         0x7D
//UNUSED:                         0x7E
//UNUSED:                         0x7F

#define OPFLAG_ARG                0x80 // 16 bit argument associated with opcode follows
#define OP_LOAD_LOC               (Dee_uint8_t)(OPFLAG_ARG|0x00) // push local slot unsigned(ARG) onto the stack
#define OP_LOAD_REF               (Dee_uint8_t)(OPFLAG_ARG|0x01) // push ref   slot unsigned(ARG) onto the stack
#define OP_LOAD_CST               (Dee_uint8_t)(OPFLAG_ARG|0x02) // push const slot unsigned(ARG) onto the stack
#define OP_LOAD_CST_COPY          (Dee_uint8_t)(OPFLAG_ARG|0x03) // push const slot unsigned(ARG) onto the stack (copy the const before pushing)
#define OP_LOAD_CST_DCOPY         (Dee_uint8_t)(OPFLAG_ARG|0x04) // push const slot unsigned(ARG) onto the stack (create a deep copy of the const before pushing)
#define OP_LOAD_BLTIN             (Dee_uint8_t)(OPFLAG_ARG|0x05) // push bltin slot unsigned(ARG) onto the stack (see table below for builtin objects) 
#define OP_LOAD_ARG               (Dee_uint8_t)(OPFLAG_ARG|0x08) // push args slot unsigned(ARG) onto the stack
#define OP_STORE_LOC              (Dee_uint8_t)(OPFLAG_ARG|0x09) // store top in local slot unsigned(ARG) (don't pop it though)
#define OP_STORE_LOC_POP          (Dee_uint8_t)(OPFLAG_ARG|0x0A) // store top in local slot unsigned(ARG) (and pop it)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x0B) // Formerly: OP_STORE_ARG
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x0C) // Formerly: OP_STORE_ARG_POP
#define OP_STORE_CST              (Dee_uint8_t)(OPFLAG_ARG|0x0D) // store top in const slot unsigned(ARG) (don't pop it though)
#define OP_STORE_CST_POP          (Dee_uint8_t)(OPFLAG_ARG|0x0E) // store top in const slot unsigned(ARG) (and pop it)
#define OP_DEL_LOCAL              (Dee_uint8_t)(OPFLAG_ARG|0x0F) // delete local slot unsigned(ARG)
#define OP_TUPLE                  (Dee_uint8_t)(OPFLAG_ARG|0x10) // pop unsigned(ARG) values and pack them into a tuple (NOTE: reverse order; top is last element; undefined behaviour if ARG == 0)
#define OP_LIST                   (Dee_uint8_t)(OPFLAG_ARG|0x11) // pop unsigned(ARG) values and pack them into a list (NOTE: reverse order; top is last element; undefined behaviour if ARG == 0)
#define OP_DICT                   (Dee_uint8_t)(OPFLAG_ARG|0x12) // pop unsigned(ARG)*2 values and pack them into a dict (undefined behaviour if ARG == 0)
#define OP_JUMP                   (Dee_uint8_t)(OPFLAG_ARG|0x13) // jump signed(ARG) bytes (based on address of this opcode)
#define OP_JUMP_IF_TT             (Dee_uint8_t)(OPFLAG_ARG|0x14) // jump signed(ARG) bytes if stack top is true (based on address of this opcode)
#define OP_JUMP_IF_FF             (Dee_uint8_t)(OPFLAG_ARG|0x15) // jump signed(ARG) bytes if stack top is false (based on address of this opcode)
#define OP_JUMP_IF_TT_POP         (Dee_uint8_t)(OPFLAG_ARG|0x16) // jump signed(ARG) bytes if stack top is true (based on address of this opcode)
#define OP_JUMP_IF_FF_POP         (Dee_uint8_t)(OPFLAG_ARG|0x17) // jump signed(ARG) bytes if stack top is false (based on address of this opcode)
#define OP_ATTR_GET_C             (Dee_uint8_t)(OPFLAG_ARG|0x18) // same as OP_ATTR_GET, but the name argument is const slot unsigned(ARG)
#define OP_ATTR_DEL_C             (Dee_uint8_t)(OPFLAG_ARG|0x19) // same as OP_ATTR_DEL, but the name argument is const slot unsigned(ARG)
#define OP_ATTR_SET_C             (Dee_uint8_t)(OPFLAG_ARG|0x1A) // same as OP_ATTR_SET, but the name argument is const slot unsigned(ARG)
// Yield one from an iterator and store its value on the stack
// If the iterator is empty, jump +signed(ARG) from the code address
// a = __iter_next__(get(0));
// if (a) { push(a); } else { jump signed(ARG); }
#define OP_SEQ_ITER_WALK          (Dee_uint8_t)(OPFLAG_ARG|0x1B)
#define OP_LOAD_CST_LOCKED        (Dee_uint8_t)(OPFLAG_ARG|0x1C) // push const(unsigned(ARG)) onto the stack (while locking the consts mutex) (Formerly: OP_PRINT)
#define OP_ATTR_HAS_C             (Dee_uint8_t)(OPFLAG_ARG|0x1D) // same as OP_ATTR_HAS, but the name argument is const slot unsigned(ARG) (Formerly: OP_PRINT_PUSH_TPL)
#define OP_STACKALIGN             (Dee_uint8_t)(OPFLAG_ARG|0x1E) // Align the stack to unsigned(ARG) (Formerly: OP_PRINT_NOLF)
#define OP_LROT_N                 (Dee_uint8_t)(OPFLAG_ARG|0x1F) // Rotate unsigned(ARG)+1 stack entries to the left (putting stack_env[-unsigned(ARG)] to the top) (Formerly: OP_PRINT_PUSH_TPL_NOLF)
#define OP_RROT_N                 (Dee_uint8_t)(OPFLAG_ARG|0x20) // Rotate unsigned(ARG)+1 stack entries to the right (putting stack_env[-1] to the top) (Formerly: OP_PRINT_F)
#define OP_FOREIGN_FUNCTION       (Dee_uint8_t)(OPFLAG_ARG|0x21) // Create a foreign function type (return type is at the base of the stack; unsigned(ARG) is the argument count; both are popped; use OP_PREFIX for custom flags) (Formerly: OP_PRINT_F_PUSH_TPL)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x22) // Formerly: OP_PRINT_F_NOLF
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x23) // Formerly: OP_PRINT_F_PUSH_TPL_NOLF
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x24) // Formerly: OP_DEBUG_FILE
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x25) // Formerly: OP_DEBUG_LINE
#define OP_FUNCTION               (Dee_uint8_t)(OPFLAG_ARG|0x26) // generate a function using pop(0) as code, pop(1) as refs and unsigned(ARG) as argc
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x27)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x28)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x29)
#define OP_TRY_BEGIN              (Dee_uint8_t)(OPFLAG_ARG|0x2A) // Add the exception handler in unsigned(ARG) ontop of the stack of active handlers
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x2B) // Formerly: OP_EXTERN_DECL
// generate a module using pop(0) as code, pop(1) as refs and const slot unsigned(ARG) as local names
#define OP_CALL_MEMBER_C          (Dee_uint8_t)(OPFLAG_ARG|0x2C) // push(__getattr__(pop(1),const(unsigned(ARG)))(pop(0)...))
// v TODO: Extended size support
#define OP_UNPACK_TUPLE           (Dee_uint8_t)(OPFLAG_ARG|0x2D) // same as 'OP_UNPACK_SEQUENCE', but asserts a tuple
// v TODO: Extended size support
#define OP_UNPACK_LIST            (Dee_uint8_t)(OPFLAG_ARG|0x2E) // same as 'OP_UNPACK_SEQUENCE', but asserts a list
// v TODO: Extended size support
#define OP_UNPACK_SEQUENCE        (Dee_uint8_t)(OPFLAG_ARG|0x2F) // pop a sequence and unpack its contents into unsigned(ARG)!=0 new stack slots
// if (pop(0) == second) jmp +signed(ARG);
// - Pop the first stack slot and compare it with the second.
//   If they are true, jump by +signed(ARG).
#define OP_JUMP_IF_CASE_COMPARE   (Dee_uint8_t)(OPFLAG_ARG|0x30)
// if (pop(1) <= third && pop(0) >= third) jmp +signed(ARG);
// - Pop the first stack slot and compare it with the second.
//   If they are true, jump by +signed(ARG).
#define OP_JUMP_IF_RCASE_COMPARE  (Dee_uint8_t)(OPFLAG_ARG|0x31)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x32) // Formerly: OP_LOC_INPLACE_ADD
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x33) // Formerly: OP_LOC_INPLACE_SUB
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x34) // Formerly: OP_LOC_INPLACE_MUL
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x35) // Formerly: OP_LOC_INPLACE_DIV
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x36) // Formerly: OP_LOC_INPLACE_MOD
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x37) // Formerly: OP_LOC_INPLACE_SHL
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x38) // Formerly: OP_LOC_INPLACE_SHR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x39) // Formerly: OP_LOC_INPLACE_AND
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3A) // Formerly: OP_LOC_INPLACE_OR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3B) // Formerly: OP_LOC_INPLACE_XOR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3C) // Formerly: OP_LOC_INPLACE_POW
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3D) // Formerly: OP_ARG_INPLACE_ADD
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3E) // Formerly: OP_ARG_INPLACE_SUB
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x3F) // Formerly: OP_ARG_INPLACE_MUL
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x40) // Formerly: OP_ARG_INPLACE_DIV
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x41) // Formerly: OP_ARG_INPLACE_MOD
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x42) // Formerly: OP_ARG_INPLACE_SHL
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x43) // Formerly: OP_ARG_INPLACE_SHR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x44) // Formerly: OP_ARG_INPLACE_AND
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x45) // Formerly: OP_ARG_INPLACE_OR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x46) // Formerly: OP_ARG_INPLACE_XOR
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x47) // Formerly: OP_ARG_INPLACE_POW
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x48) // Formerly: OP_LOC_INPLACE_INC
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x49) // Formerly: OP_LOC_INPLACE_DEC
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4A) // Formerly: OP_LOC_INPLACE_INC_POST
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4B) // Formerly: OP_LOC_INPLACE_DEC_POST
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4C) // Formerly: OP_ARG_INPLACE_INC
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4D) // Formerly: OP_ARG_INPLACE_DEC
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4E) // Formerly: OP_ARG_INPLACE_INC_POST
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x4F) // Formerly: OP_ARG_INPLACE_DEC_POST
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x50)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x51)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x52)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x53)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x54)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x55)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x56)
// NOTE: Since I didn't have a better idea on how to do this,
//       you have to push(""), aka. empty_string for get/del/set to indicate
//       that you're not implementing that specific operator.
//       I didn't want to use 'none' for this, because 'none', as awesome as it is,
//       can be used for any operator to indicate a no-op.
//       But since the empty string is a builtin object as well, and since strings
//       aren't callable, this should really be a problem and should go unnoticed.
#define OP_CLASS_SET_SLOT         (Dee_uint8_t)(OPFLAG_ARG|0x57) // second.__set_slot(unsigned(ARG),pop(0))
#define OP_CLASS_ADD_METHOD       (Dee_uint8_t)(OPFLAG_ARG|0x58) // second.__add_member(const(unsigned(ARG)),pop(0))
#define OP_CLASS_ADD_GETSET       (Dee_uint8_t)(OPFLAG_ARG|0x59) // fourth.__add_getset(const(unsigned(ARG)),pop(2),pop(1),pop(0))
#define OP_CLASS_ADD_CLASS_METHOD (Dee_uint8_t)(OPFLAG_ARG|0x5A) // second.__add_class_member(const(unsigned(ARG)),pop(0))
#define OP_CLASS_ADD_CLASS_GETSET (Dee_uint8_t)(OPFLAG_ARG|0x5B) // fourth.__add_class_getset(const(unsigned(ARG)),pop(2),pop(1),pop(0))
#define OP_SET                    (Dee_uint8_t)(OPFLAG_ARG|0x5C) // pop unsigned(ARG) values and pack them into a set (undefined behaviour if ARG == 0)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x5D)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x5E)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x5F)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x60)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x61)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x62)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x63)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x64)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x65)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x66)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x67)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x68)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x69)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x70)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x7A)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x7B)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x7C)
//UNUSED:                         (Dee_uint8_t)(OPFLAG_ARG|0x7D)
#define OP_PREFIX                 (Dee_uint8_t)(OPFLAG_ARG|0x7E) // Special prefix including additional data for the next opcode (no-op if next opcode doesn't implement special handling)
#define OP_EXTENDED               (Dee_uint8_t)(OPFLAG_ARG|0x7F) // extended opcode (use op argument for any OPEXT_xxx opcode)

// Extended opcodes
#define OPEXT_TRY_INPLACE_ADD     0x0000 // push(({ res = pop(1) +=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) +  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_ADD)
#define OPEXT_TRY_INPLACE_SUB     0x0001 // push(({ res = pop(1) -=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) -  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SUB)
#define OPEXT_TRY_INPLACE_MUL     0x0002 // push(({ res = pop(1) *=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) *  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_MUL)
#define OPEXT_TRY_INPLACE_DIV     0x0003 // push(({ res = pop(1) /=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) /  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_DIV)
#define OPEXT_TRY_INPLACE_MOD     0x0004 // push(({ res = pop(1) %=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) %  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_MOD)
#define OPEXT_TRY_INPLACE_SHL     0x0005 // push(({ res = pop(1) <<= pop(0); pack res,res; })...); <or> push(({ res = pop(1) << pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SHL)
#define OPEXT_TRY_INPLACE_SHR     0x0006 // push(({ res = pop(1) >>= pop(0); pack res,res; })...); <or> push(({ res = pop(1) >> pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SHR)
#define OPEXT_TRY_INPLACE_AND     0x0007 // push(({ res = pop(1) &=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) &  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_AND)
#define OPEXT_TRY_INPLACE_OR      0x0008 // push(({ res = pop(1) |=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) |  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_OR)
#define OPEXT_TRY_INPLACE_XOR     0x0009 // push(({ res = pop(1) ^=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) ^  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_XOR)
#define OPEXT_TRY_INPLACE_POW     0x000A // push(({ res = pop(1) **= pop(0); pack res,res; })...); <or> push(({ res = pop(1) ** pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_POW)
//UNUSED:                         0x000B // Formerly: OPEXT_LOAD_LAST_EXCEPTION
//UNUSED:                         0x000C // Formerly: OPEXT_LOAD_LAST_TRACEBACK
// Same a the C function - allocates memory on the stack that will be
// freed once the function is left (allocates into a void* pointer)
// NOTE: Deemon will simulate the behavior of alloca if the native library doesn't support it.
//       So this is something you can always enable if you dare.
#define OPEXT_ALLOCA              0x000D // push(__builtin_alloc(pop(0)))
#define OPEXT_BREAKPOINT          0x000E // __builtin_breakpoint();
#define OPEXT_DOC                 0x000F // push(__builtin_help(pop(0)))
#define OPEXT_DOC_ATTR            0x0010 // push(__builtin_help(pop(1).operator . (pop(0))))
#define OPEXT_EMPTY_TUPLE         0x0011 // push(pack())
#define OPEXT_EMPTY_LIST          0x0012 // push([])
#define OPEXT_EMPTY_DICT          0x0013 // push(dict {})
#define OPEXT_TRY_INPLACE_INC     0x0014 // push(({ val = pop(0); res = ++val; pack res,val; })...); <or> push(({ val = pop(0); res = val += 1; pack res,val; })...); <or> push(({ val = pop(0); res = val = val + 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_DEC     0x0015 // push(({ val = pop(0); res = ++val; pack res,val; })...); <or> push(({ val = pop(0); res = val -= 1; pack res,val; })...); <or> push(({ val = pop(0); res = val = val - 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_INC_POST 0x0016 // push(({ val = pop(0); res = val++; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val += 1; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val = val + 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_DEC_POST 0x0017 // push(({ val = pop(0); res = val--; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val -= 1; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val = val - 1; pack res,val; })...);
#define OPEXT_CLASS_NEW           0x0018 // push(__builtin_object(0x0173)(pop(1),pop(0)))
#define OPEXT_CLASS_NEW_UUID      0x0019 // push(__builtin_object(0x0173)(pop(2),pop(1),pop(0)))
#define OPEXT_EMPTY_SET           0x001A // push(set {})
#define OPEXT_SUPER_AT            0x001B // push(__super(pop(1),pop(0)))
#define OPEXT_EMPTY_CELL          0x001C // push(<>)
#define OPEXT_BOUND_LOCAL         0x001D // push(__builtin_bound(...)) (w/ OP_PREFIX)
#define OPEXT_BOUND_RESULT        0x001D // push(__builtin_bound(...)) (w/o OP_PREFIX)
//UNUSED:                         0x001E
#define OPEXT_DEL_RESULT          0x001F // function foo: result { del result; }
#define OPEXT_SUPEROF             0x0020 // push(pop(0).super)
#define OPEXT_CLASSOF             0x0021 // push(pop(0).class)
// Iterate the sequence in top, discarding all elements it may produce
// - no-op if top is a builtin tuple/list/dict
// - This is used when expanding an expression without using its result
#define OPEXT_SEQ_ITERALL         0x0022
#define OPEXT_SEQ_ANY             0x0023 // push(pop(0)||...)
#define OPEXT_SEQ_ALL             0x0024 // push(pop(0)&&...)
#define OPEXT_SEQ_SUM             0x0025 // push(pop(0)&&...)
#define OPEXT_TYPEFLAGS_LO        0x0026 // w/prefix or the argument to the type flags in top (low word: 0x0000FFFF)
#define OPEXT_TYPEFLAGS_HI        0x0027 // w/prefix or the argument to the type flags in top (high word: 0xFFFF0000)
#define OPEXT_SEQ_ITER_NEXT       0x0028 // Simple operator __iternext__ w/o a conditional jump (throws a Signal.StopIteration if the iterator is done)
#define OPEXT_TRY_CMP_EQ          0x0029 // try { push(pop(1) == pop(0)); } catch (Error.RuntimeError.NotImplemented) { push(false); }
#define OPEXT_VARRAYOF            0x002A // push(pop(0)[])
// File Operators
#define OPEXT_IO_WRITEP           0x002B // push(pop(2).operator __write__(pop(1),pop(0)))
#define OPEXT_IO_WRITENP          0x002C // push(pop(1).operator __write__(pop(0)))
#define OPEXT_IO_READP            0x002D // push(pop(2).operator __read__(pop(1),pop(0)))
#define OPEXT_IO_READNP           0x002E // push(pop(1).operator __read__(pop(0)))
#define OPEXT_IO_SEEK             0x002F //      pop(2).operator __seek__(pop(1),pop(0))
#define OPEXT_IO_SEEK_USED        0x0030 // push(pop(1).operator __seek__(pop(1),pop(0)))
#define OPEXT_IO_TRUNC            0x0031 //      pop(0).operator __trunc__()
#define OPEXT_IO_FLUSH            0x0032 //      pop(0).operator __flush__()
#define OPEXT_IO_CLOSE            0x0033 //      pop(0).operator __close__()
#define OPEXT_SEQ_MIN             0x0034 // push(pop(0)<...)
#define OPEXT_SEQ_MAX             0x0035 // push(pop(0)>...)


#ifdef DEE_OUTDATED_OPCODES
#if DEE_OUTDATED_OPCODES >= 101
#define v101_OP_STORE_ARG             (Dee_uint8_t)(OPFLAG_ARG|0x0B) /*< Converted to locals now. */
#define v101_OP_STORE_ARG_POP         (Dee_uint8_t)(OPFLAG_ARG|0x0C) /*< Converted to locals now. */
#define v101_OP_DEBUG_FILE            (Dee_uint8_t)(OPFLAG_ARG|0x24) /*< Uses a different system now. */
#endif
#if DEE_OUTDATED_OPCODES >= 100
// Outdated/unsupported opcodes
#define v100_OP_ASSIGN                OP_COPY_ASSIGN   /*< Renamed to indicate the kind of assignment. */
#define v100_OP_ROT_3                 OP_LROT_3        /*< Renamed to indicate rotation direction. */
#define v100_OP_ITERATE_SEQUENCE      0x51             /*< Use 'OP_EXTENDED+OPEXT_SEQ_ITERALL' instead. */
#define v100_OP_SEQ_ITER_NEXT         OP_SEQ_ITER_WALK /*< Renamed, as this isn't an operator opcode. */
#define v100_OP_PRINT_v100            (Dee_uint8_t)(OPFLAG_ARG|0x1C) /*< Uses a different system now. */
#define v100_OP_PRINT_PUSH_TPL        (Dee_uint8_t)(OPFLAG_ARG|0x1D) /*< *ditto*. */
#define v100_OP_PRINT_NOLF            (Dee_uint8_t)(OPFLAG_ARG|0x1E) /*< *ditto*. */
#define v100_OP_PRINT_PUSH_TPL_NOLF   (Dee_uint8_t)(OPFLAG_ARG|0x1F) /*< *ditto*. */
#define v100_OP_PRINT_F               (Dee_uint8_t)(OPFLAG_ARG|0x20) /*< *ditto*. */
#define v100_OP_PRINT_F_PUSH_TPL      (Dee_uint8_t)(OPFLAG_ARG|0x21) /*< *ditto*. */
#define v100_OP_PRINT_F_NOLF          (Dee_uint8_t)(OPFLAG_ARG|0x22) /*< *ditto*. */
#define v100_OP_PRINT_F_PUSH_TPL_NOLF (Dee_uint8_t)(OPFLAG_ARG|0x23) /*< *ditto*. */
#define v100_OP_DEBUG_LINE            (Dee_uint8_t)(OPFLAG_ARG|0x25) /*< Uses a different system now. */
#define v100_OP_FUNCTION_V            (Dee_uint8_t)(OPFLAG_ARG|0x27) /*< Uses a different system now. */
#define v100_OP_FUNCTION_Y            (Dee_uint8_t)(OPFLAG_ARG|0x28) /*< *ditto*. */
#define v100_OP_FUNCTION_YV           (Dee_uint8_t)(OPFLAG_ARG|0x29) /*< *ditto*. */
#define v100_OP_EXTERN_DECL           (Dee_uint8_t)(OPFLAG_ARG|0x2B) /*< Uses a different system now. */
#define v100_OP_CALL_MEMBER           OP_CALL_MEMBER_C           /*< Renamed to indicate attribute lookup by constant. */
#define v100_OP_EXTENDED_ARG          OP_PREFIX                  /*< Renamed as this opcode is not just used to OP_EXTENDED */
#define v100_OP_LOC_INPLACE_ADD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x32) /*< These are now using a different system. */
#define v100_OP_LOC_INPLACE_SUB_v100  (Dee_uint8_t)(OPFLAG_ARG|0x33) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_MUL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x34) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_DIV_v100  (Dee_uint8_t)(OPFLAG_ARG|0x35) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_MOD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x36) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_SHL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x37) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_SHR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x38) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_AND_v100  (Dee_uint8_t)(OPFLAG_ARG|0x39) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_OR_v100   (Dee_uint8_t)(OPFLAG_ARG|0x3A) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_XOR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x3B) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_POW_v100  (Dee_uint8_t)(OPFLAG_ARG|0x3C) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_ADD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x3D) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SUB_v100  (Dee_uint8_t)(OPFLAG_ARG|0x3E) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_MUL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x3F) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_DIV_v100  (Dee_uint8_t)(OPFLAG_ARG|0x40) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_MOD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x41) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SHL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x42) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SHR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x43) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_AND_v100  (Dee_uint8_t)(OPFLAG_ARG|0x44) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_OR_v100   (Dee_uint8_t)(OPFLAG_ARG|0x45) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_XOR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x46) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_POW_v100  (Dee_uint8_t)(OPFLAG_ARG|0x47) /*< *ditto*. */
#define v100_OP_CST_INPLACE_ADD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x48) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SUB_v100  (Dee_uint8_t)(OPFLAG_ARG|0x49) /*< *ditto*. */
#define v100_OP_CST_INPLACE_MUL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4A) /*< *ditto*. */
#define v100_OP_CST_INPLACE_DIV_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4B) /*< *ditto*. */
#define v100_OP_CST_INPLACE_MOD_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4C) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SHL_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4D) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SHR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4E) /*< *ditto*. */
#define v100_OP_CST_INPLACE_AND_v100  (Dee_uint8_t)(OPFLAG_ARG|0x4F) /*< *ditto*. */
#define v100_OP_CST_INPLACE_OR_v100   (Dee_uint8_t)(OPFLAG_ARG|0x50) /*< *ditto*. */
#define v100_OP_CST_INPLACE_XOR_v100  (Dee_uint8_t)(OPFLAG_ARG|0x51) /*< *ditto*. */
#define v100_OP_CST_INPLACE_POW_v100  (Dee_uint8_t)(OPFLAG_ARG|0x52) /*< *ditto*. */
#define v100_OPEXT_BOOL_USER          0x0014 /*< Call the bool constructor instead. */
#define v100_OPEXT_CODE_SELF          0x0015 /*< Call builtin object #0x0169 instead. */
#endif
#endif /* DEE_OUTDATED_OPCODES */


// Case-list of all opcodes with a jmp-argument
#define DEE_OPCODE_CASE_HAS_JMPARG\
  case OP_JUMP:\
  case OP_JUMP_IF_TT:\
  case OP_JUMP_IF_FF:\
  case OP_JUMP_IF_TT_POP:\
  case OP_JUMP_IF_FF_POP:\
  case OP_SEQ_ITER_WALK:\
  case OP_JUMP_IF_CASE_COMPARE:\
  case OP_JUMP_IF_RCASE_COMPARE:


#endif /* DEE_LIMITED_API */

#endif /* !GUARD_DEEMON___OPCODE_INL */
