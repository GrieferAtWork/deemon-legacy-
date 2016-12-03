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

typedef Dee_uint8_t  Dee_opcode_t;
typedef Dee_uint16_t Dee_oparg_t;

#define OPFLAG_ARG                           DEE_UINT8_C(0x80) /*< 16 bit argument associated with opcode follows. */
#define OPREG(id)                            DEE_UINT8_C(id)   /*< Regular Opcode. */
#define OPARG(id)  (Dee_opcode_t)(OPFLAG_ARG|DEE_UINT8_C(id))  /*< Opcode with an argument. */
#define OPEXT(id)                            DEE_UINT16_C(id)   /*< Extended opcode. */



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
#define OP_RETNONE                OPREG(0x00) // return none;
#define OP_NOOP                   OPREG(0x01) // // no-op (just skip it)
#define OP_POP                    OPREG(0x02) // pop(0)
#define OP_RET                    OPREG(0x03) // return pop(0)
#define OP_STOP                   OPREG(0x04) // [internal only] Simply stop execution (don't override return value)
#define OP_RETVAREXIT             OPREG(0x05) // return; // In a result variable function (set return value to none if it wasn't set before)
#define OP_YIELD                  OPREG(0x06) // yield pop(0);
#define OP_YIELDALL               OPREG(0x07) // try { x = top; while (1) yield x.__iternext__(); } catch (Signal.StopIteration) { } pop(1);
#define OP_YIELDSTOP              OPREG(0x08) // return; // In a yield function
#define OP_LOAD_NONE              OPREG(0x09) // push(none)
#define OP_COPY                   OPREG(0x0A) // push(copy pop(0))
#define OP_MOVE                   OPREG(0x0B) // push(move pop(0))
#define OP_TYPE                   OPREG(0x0C) // push(type pop(0))
#define OP_BOOL                   OPREG(0x0D) // push(!!pop(0)) // NOTE: Internal bool operator (string is true if it's non-empty)
#define OP_WEAK                   OPREG(0x0E) // push(weak pop(0))
#define OP_IS                     OPREG(0x0F) // push(pop(1) is pop(0))
#define OP_IN                     OPREG(0x10) // push(pop(1) in pop(0))
#define OP_CALL                   OPREG(0x11) // push(pop(1)(pop(0)...))
#define OP_COPY_ASSIGN            OPREG(0x12) // push(pop(1) = pop(0))
#define OP_STR                    OPREG(0x13) // push(str(pop(0)))
#define OP_REPR                   OPREG(0x14) // push(repr(pop(0)))
#define OP_NOT                    OPREG(0x15) // push(!pop(0))
#define OP_POS                    OPREG(0x16) // push(+pop(0))
#define OP_NEG                    OPREG(0x17) // push(-pop(0))
#define OP_REF                    OPREG(0x18) // push(&pop(0))
#define OP_DEREF                  OPREG(0x19) // push(*pop(0))
#define OP_INV                    OPREG(0x1A) // push(-pop(0))
#define OP_INC                    OPREG(0x1B) // push(++pop(0))
#define OP_DEC                    OPREG(0x1C) // push(--pop(0))
#define OP_INC_POST               OPREG(0x1D) // push(pop(0)++)
#define OP_DEC_POST               OPREG(0x1E) // push(pop(0)--)
#define OP_ADD                    OPREG(0x1F) // push(pop(1)+pop(0))
#define OP_INPLACE_ADD            OPREG(0x20) // push(pop(1)+=pop(0))
#define OP_SUB                    OPREG(0x21) // push(pop(1)-pop(0))
#define OP_INPLACE_SUB            OPREG(0x22) // push(pop(1)-=pop(0))
#define OP_MUL                    OPREG(0x23) // push(pop(1)*pop(0))
#define OP_INPLACE_MUL            OPREG(0x24) // push(pop(1)*=pop(0))
#define OP_DIV                    OPREG(0x25) // push(pop(1)/pop(0))
#define OP_INPLACE_DIV            OPREG(0x26) // push(pop(1)/=pop(0))
#define OP_MOD                    OPREG(0x27) // push(pop(1)%pop(0))
#define OP_INPLACE_MOD            OPREG(0x28) // push(pop(1)%=pop(0))
#define OP_SHL                    OPREG(0x29) // push(pop(1)<<pop(0))
#define OP_INPLACE_SHL            OPREG(0x2A) // push(pop(1)<<=pop(0))
#define OP_SHR                    OPREG(0x2B) // push(pop(1)>>pop(0))
#define OP_INPLACE_SHR            OPREG(0x2C) // push(pop(1)>>=pop(0))
#define OP_AND                    OPREG(0x2D) // push(pop(1)&pop(0))
#define OP_INPLACE_AND            OPREG(0x2E) // push(pop(1)&=pop(0))
#define OP_OR                     OPREG(0x2F) // push(pop(1)|pop(0))
#define OP_INPLACE_OR             OPREG(0x30) // push(pop(1)|=pop(0))
#define OP_XOR                    OPREG(0x31) // push(pop(1)^pop(0))
#define OP_INPLACE_XOR            OPREG(0x32) // push(pop(1)^=pop(0))
#define OP_POW                    OPREG(0x33) // push(pop(1)**pop(0))
#define OP_INPLACE_POW            OPREG(0x34) // push(pop(1)**=pop(0))
#define OP_CMP_LO                 OPREG(0x35) // push(pop(1)<pop(0))
#define OP_CMP_LE                 OPREG(0x36) // push(pop(1)<=pop(0))
#define OP_CMP_EQ                 OPREG(0x37) // push(pop(1)==pop(0))
#define OP_CMP_NE                 OPREG(0x38) // push(pop(1)!=pop(0))
#define OP_CMP_GR                 OPREG(0x39) // push(pop(1)>pop(0))
#define OP_CMP_GE                 OPREG(0x3A) // push(pop(1)>=pop(0))
#define OP_SEQ_GET                OPREG(0x3B) // push(pop(1)[pop(0)])
#define OP_SEQ_DEL                OPREG(0x3C) // del pop(1)[pop(0)]
#define OP_SEQ_SET                OPREG(0x3D) // push(pop(2)[pop(1)] = pop(0))
#define OP_SEQ_SIZE               OPREG(0x3E) // push(len(pop(0)))
#define OP_SEQ_RANGE_GET          OPREG(0x3F) // push(pop(2)[pop(1):pop(0)])
#define OP_SEQ_RANGE_DEL          OPREG(0x40) // del pop(2)[pop(1):pop(0)]
#define OP_SEQ_RANGE_SET          OPREG(0x41) // push(pop(3)[pop(2):pop(1)] = pop(0))
#define OP_SEQ_ITER_SELF          OPREG(0x42) // push(__iterself__(pop(0)))
#define OP_ATTR_GET               OPREG(0x43) // push(__getattr__(pop(1),pop(0)))
#define OP_ATTR_DEL               OPREG(0x44) // __delattr__(pop(1),pop(0))
#define OP_ATTR_SET               OPREG(0x45) // push(__setattr__(pop(2),pop(0),pop(1)))
#define OP_ROT_2                  OPREG(0x46) // rotate top 2 stack entries: 'ab' -> 'ba'
#define OP_LROT_3                 OPREG(0x47) // rotate top 3 stack entries: 'abc' -> 'bca'
#define OP_TRY_END                OPREG(0x48) // Pop the last exception hander from the stack of active handlers (NOTE: If it's a finally handler, it is executed before returning to the opcode following this one, unless an error occurred in the finally-block)
#define OP_EXCEPT_END             OPREG(0x49) // Written at the end of exception handling code (cleans up the exception chain by dropping the exception currently being handled)
#define OP_FINALLY_END            OPREG(0x4A) // Written at the end of finally handling code (rethrows finally exceptions and returns to the address this finally block was executed from)
#define OP_THROW                  OPREG(0x4B) // throw pop(0)
#define OP_RETHROW                OPREG(0x4C) // throw
#define OP_CONCAT_TUPLE           OPREG(0x4D) // push(pop(1)+pop(0))
#define OP_CONCAT_LIST            OPREG(0x4E) // push(pop(1)+pop(0))
#define OP_CAST_LIST              OPREG(0x4F) // push(list(pop(0)))
#define OP_CAST_TUPLE             OPREG(0x50) // push(tuple(pop(0)))
//UNUSED:                         OPREG(0x51)
#define OP_LOAD_RET               OPREG(0x52) // function: result { push(result); }
#define OP_STORE_RET              OPREG(0x53) // function: result { result = top; }
//UNUSED:                         OPREG(0x54) // Formerly: OP_STORE_RET_POP
// Throws an error if reached (this op-code is only generated in debug mode by default)
// NOTE: Finally blocks from the stack frame that encountered this opcode will not be executed
//      (It's barely possible to recover from this without having to execute user code...)
#define OP_UNREACHABLE            OPREG(0x55) // __builtin_unreachable();
#define OP_LOAD_THIS              OPREG(0x56) // function [[__thiscall__]] { push(this); }
#define OP_DUP                    OPREG(0x57) // push(top)
#define OP_JUMP_DYN_REL           OPREG(0x58) // jump ssize_t(pop(0)) bytes, starting at the address of this opcode
#define OP_JUMP_DYN_ABS           OPREG(0x59) // jump to size_t(pop(0)), starting at the co_code address of the associated code
//UNUSED:                         OPREG(0x5A)
//UNUSED:                         OPREG(0x5B)
#define OP_PRINT_ONE              OPREG(0x5C) // print pop(0),;
#define OP_PRINT_ONE_SEP          OPREG(0x5D) // print pop(0),"",;
#define OP_PRINT_ONE_END          OPREG(0x5E) // print pop(0);
#define OP_PRINT_ONE_F            OPREG(0x5F) // print pop(1): pop(0),;
#define OP_PRINT_ONE_F_SEP        OPREG(0x60) // print second: pop(0),"",;
#define OP_PRINT_ONE_F_END        OPREG(0x61) // print pop(1): pop(0);
#define OP_PRINT_ALL              OPREG(0x62) // print pop(0)...,;
#define OP_PRINT_ALL_SEP          OPREG(0x63) // print pop(0)...,"",;
#define OP_PRINT_ALL_END          OPREG(0x64) // print pop(0)...;
#define OP_PRINT_ALL_F            OPREG(0x65) // print pop(1): pop(0)...,;
#define OP_PRINT_ALL_F_SEP        OPREG(0x66) // print second: pop(0)...,"",;
#define OP_PRINT_ALL_F_END        OPREG(0x67) // print pop(1): pop(0)...;
#define OP_PRINT_END              OPREG(0x68) // print;
#define OP_PRINT_END_F            OPREG(0x69) // print pop(0):;
#define OP_NEW_INSTANCE           OPREG(0x6A) // push(pop(1)(pop(0)...)) // same as OP_CALL, but only for types (NOTE: pop(1) MUST be a tuple; this is an assertion)
#define OP_CELL                   OPREG(0x6B) // push(<pop(0)>)
#define OP_LROT_4                 OPREG(0x6C) // rotate top 4 stack entries: 'abcd' -> 'bcda'
#define OP_ATTR_HAS               OPREG(0x6D) // push(__builtin_hasattr(pop(1),pop(0)))
#define OP_MOVE_ASSIGN            OPREG(0x6E) // push(pop(1).operator move = (pop(0)))
#define OP_RROT_3                 OPREG(0x6F) // rotate top 3 stack entries: 'abc' -> 'cab'
#define OP_RROT_4                 OPREG(0x70) // rotate top 4 stack entries: 'abcd' -> 'dabc'
#define OP_CAST_SET               OPREG(0x71) // push(set(pop(0)))
#define OP_LOAD_EXTERN            OPREG(0x72) // push(__builtin_extern(pop(2),pop(1),pop(0)))
#define OP_LOAD_AUTOEXTERN        OPREG(0x73) // push(__builtin_extern(pop(1),pop(0)))
#define OP_FF_CLOSURE             OPREG(0x74) // push(__builtin_ff_closure(pop(1),pop(0)))
//UNUSED:                         OPREG(0x75)
//UNUSED:                         OPREG(0x76)
//UNUSED:                         OPREG(0x77)
//UNUSED:                         OPREG(0x78)
//UNUSED:                         OPREG(0x79)
//UNUSED:                         OPREG(0x7A)
//UNUSED:                         OPREG(0x7B)
//UNUSED:                         OPREG(0x7C)
//UNUSED:                         OPREG(0x7D)
//UNUSED:                         OPREG(0x7E)
//UNUSED:                         OPREG(0x7F)

#define OP_LOAD_LOC               OPARG(0x00) // push local slot unsigned(ARG) onto the stack
#define OP_LOAD_REF               OPARG(0x01) // push ref   slot unsigned(ARG) onto the stack
#define OP_LOAD_CST               OPARG(0x02) // push const slot unsigned(ARG) onto the stack
#define OP_LOAD_CST_COPY          OPARG(0x03) // push const slot unsigned(ARG) onto the stack (copy the const before pushing)
#define OP_LOAD_CST_DCOPY         OPARG(0x04) // push const slot unsigned(ARG) onto the stack (create a deep copy of the const before pushing)
#define OP_LOAD_BLTIN             OPARG(0x05) // push bltin slot unsigned(ARG) onto the stack (see table below for builtin objects) 
#define OP_LOAD_ARG               OPARG(0x08) // push args slot unsigned(ARG) onto the stack
#define OP_STORE_LOC              OPARG(0x09) // store top in local slot unsigned(ARG) (don't pop it though)
//UNUSED:                         OPARG(0x0A) // Formerly: OP_STORE_LOC_POP
//UNUSED:                         OPARG(0x0B) // Formerly: OP_STORE_ARG
//UNUSED:                         OPARG(0x0C) // Formerly: OP_STORE_ARG_POP
#define OP_STORE_CST              OPARG(0x0D) // store top in const slot unsigned(ARG) (don't pop it though)
//UNUSED:                         OPARG(0x0E) // Formerly: OP_STORE_CST_POP
#define OP_DEL_LOCAL              OPARG(0x0F) // delete local slot unsigned(ARG)
#define OP_TUPLE                  OPARG(0x10) // pop unsigned(ARG) values and pack them into a tuple (NOTE: reverse order; top is last element; undefined behaviour if ARG == 0)
#define OP_LIST                   OPARG(0x11) // pop unsigned(ARG) values and pack them into a list (NOTE: reverse order; top is last element; undefined behaviour if ARG == 0)
#define OP_DICT                   OPARG(0x12) // pop unsigned(ARG)*2 values and pack them into a dict (undefined behaviour if ARG == 0)
#define OP_JUMP                   OPARG(0x13) // jump signed(ARG) bytes (based on address of this opcode)
//UNUSED:                         OPARG(0x14) // Formerly: OP_JUMP_IF_TT
//UNUSED:                         OPARG(0x15) // Formerly: OP_JUMP_IF_FF
#define OP_JUMP_IF_TT_POP         OPARG(0x16) // jump signed(ARG) bytes if stack top is true (based on address of this opcode)
#define OP_JUMP_IF_FF_POP         OPARG(0x17) // jump signed(ARG) bytes if stack top is false (based on address of this opcode)
#define OP_ATTR_GET_C             OPARG(0x18) // same as OP_ATTR_GET, but the name argument is const slot unsigned(ARG)
#define OP_ATTR_DEL_C             OPARG(0x19) // same as OP_ATTR_DEL, but the name argument is const slot unsigned(ARG)
#define OP_ATTR_SET_C             OPARG(0x1A) // same as OP_ATTR_SET, but the name argument is const slot unsigned(ARG)
// Yield one from an iterator and store its value on the stack
// If the iterator is empty, jump +signed(ARG) from the code address
// a = __iter_next__(get(0));
// if (a) { push(a); } else { jump signed(ARG); }
#define OP_SEQ_ITER_WALK          OPARG(0x1B)
#define OP_LOAD_CST_LOCKED        OPARG(0x1C) // push const(unsigned(ARG)) onto the stack (while locking the consts mutex) (Formerly: OP_PRINT)
#define OP_ATTR_HAS_C             OPARG(0x1D) // same as OP_ATTR_HAS, but the name argument is const slot unsigned(ARG) (Formerly: OP_PRINT_PUSH_TPL)
#define OP_STACKALIGN             OPARG(0x1E) // Align the stack to unsigned(ARG) (Formerly: OP_PRINT_NOLF)
#define OP_LROT_N                 OPARG(0x1F) // Rotate unsigned(ARG)+1 stack entries to the left (putting stack_env[-unsigned(ARG)] to the top) (Formerly: OP_PRINT_PUSH_TPL_NOLF)
#define OP_RROT_N                 OPARG(0x20) // Rotate unsigned(ARG)+1 stack entries to the right (putting stack_env[-1] to the top) (Formerly: OP_PRINT_F)
#define OP_FOREIGN_FUNCTION       OPARG(0x21) // Create a foreign function type (return type is at the base of the stack; unsigned(ARG) is the argument count; both are popped; use OP_PREFIX for custom flags) (Formerly: OP_PRINT_F_PUSH_TPL)
//UNUSED:                         OPARG(0x22) // Formerly: OP_PRINT_F_NOLF
//UNUSED:                         OPARG(0x23) // Formerly: OP_PRINT_F_PUSH_TPL_NOLF
//UNUSED:                         OPARG(0x24) // Formerly: OP_DEBUG_FILE
//UNUSED:                         OPARG(0x25) // Formerly: OP_DEBUG_LINE
#define OP_FUNCTION               OPARG(0x26) // generate a function using pop(0) as code, pop(1) as refs and unsigned(ARG) as argc
//UNUSED:                         OPARG(0x27)
//UNUSED:                         OPARG(0x28)
//UNUSED:                         OPARG(0x29)
#define OP_TRY_BEGIN              OPARG(0x2A) // Add the exception handler in unsigned(ARG) ontop of the stack of active handlers
//UNUSED:                         OPARG(0x2B) // Formerly: OP_EXTERN_DECL
// generate a module using pop(0) as code, pop(1) as refs and const slot unsigned(ARG) as local names
#define OP_CALL_MEMBER_C          OPARG(0x2C) // push(__getattr__(pop(1),const(unsigned(ARG)))(pop(0)...))
// v TODO: Extended size support
#define OP_UNPACK_TUPLE           OPARG(0x2D) // same as 'OP_UNPACK_SEQUENCE', but asserts a tuple
// v TODO: Extended size support
#define OP_UNPACK_LIST            OPARG(0x2E) // same as 'OP_UNPACK_SEQUENCE', but asserts a list
// v TODO: Extended size support
#define OP_UNPACK_SEQUENCE        OPARG(0x2F) // pop a sequence and unpack its contents into unsigned(ARG)!=0 new stack slots
// if (pop(0) == second) jmp +signed(ARG);
// - Pop the first stack slot and compare it with the second.
//   If they are true, jump by +signed(ARG).
#define OP_JUMP_IF_CASE_COMPARE   OPARG(0x30)
// if (pop(1) <= third && pop(0) >= third) jmp +signed(ARG);
// - Pop the first stack slot and compare it with the second.
//   If they are true, jump by +signed(ARG).
#define OP_JUMP_IF_RCASE_COMPARE  OPARG(0x31)
//UNUSED:                         OPARG(0x32) // Formerly: OP_LOC_INPLACE_ADD
//UNUSED:                         OPARG(0x33) // Formerly: OP_LOC_INPLACE_SUB
//UNUSED:                         OPARG(0x34) // Formerly: OP_LOC_INPLACE_MUL
//UNUSED:                         OPARG(0x35) // Formerly: OP_LOC_INPLACE_DIV
//UNUSED:                         OPARG(0x36) // Formerly: OP_LOC_INPLACE_MOD
//UNUSED:                         OPARG(0x37) // Formerly: OP_LOC_INPLACE_SHL
//UNUSED:                         OPARG(0x38) // Formerly: OP_LOC_INPLACE_SHR
//UNUSED:                         OPARG(0x39) // Formerly: OP_LOC_INPLACE_AND
//UNUSED:                         OPARG(0x3A) // Formerly: OP_LOC_INPLACE_OR
//UNUSED:                         OPARG(0x3B) // Formerly: OP_LOC_INPLACE_XOR
//UNUSED:                         OPARG(0x3C) // Formerly: OP_LOC_INPLACE_POW
//UNUSED:                         OPARG(0x3D) // Formerly: OP_ARG_INPLACE_ADD
//UNUSED:                         OPARG(0x3E) // Formerly: OP_ARG_INPLACE_SUB
//UNUSED:                         OPARG(0x3F) // Formerly: OP_ARG_INPLACE_MUL
//UNUSED:                         OPARG(0x40) // Formerly: OP_ARG_INPLACE_DIV
//UNUSED:                         OPARG(0x41) // Formerly: OP_ARG_INPLACE_MOD
//UNUSED:                         OPARG(0x42) // Formerly: OP_ARG_INPLACE_SHL
//UNUSED:                         OPARG(0x43) // Formerly: OP_ARG_INPLACE_SHR
//UNUSED:                         OPARG(0x44) // Formerly: OP_ARG_INPLACE_AND
//UNUSED:                         OPARG(0x45) // Formerly: OP_ARG_INPLACE_OR
//UNUSED:                         OPARG(0x46) // Formerly: OP_ARG_INPLACE_XOR
//UNUSED:                         OPARG(0x47) // Formerly: OP_ARG_INPLACE_POW
//UNUSED:                         OPARG(0x48) // Formerly: OP_LOC_INPLACE_INC
//UNUSED:                         OPARG(0x49) // Formerly: OP_LOC_INPLACE_DEC
//UNUSED:                         OPARG(0x4A) // Formerly: OP_LOC_INPLACE_INC_POST
//UNUSED:                         OPARG(0x4B) // Formerly: OP_LOC_INPLACE_DEC_POST
//UNUSED:                         OPARG(0x4C) // Formerly: OP_ARG_INPLACE_INC
//UNUSED:                         OPARG(0x4D) // Formerly: OP_ARG_INPLACE_DEC
//UNUSED:                         OPARG(0x4E) // Formerly: OP_ARG_INPLACE_INC_POST
//UNUSED:                         OPARG(0x4F) // Formerly: OP_ARG_INPLACE_DEC_POST
//UNUSED:                         OPARG(0x50)
//UNUSED:                         OPARG(0x51)
//UNUSED:                         OPARG(0x52)
//UNUSED:                         OPARG(0x53)
//UNUSED:                         OPARG(0x54)
//UNUSED:                         OPARG(0x55)
//UNUSED:                         OPARG(0x56)
// NOTE: Since I didn't have a better idea on how to do this,
//       you have to push(""), aka. empty_string for get/del/set to indicate
//       that you're not implementing that specific operator.
//       I didn't want to use 'none' for this, because 'none', as awesome as it is,
//       can be used for any operator to indicate a no-op.
//       But since the empty string is a builtin object as well, and since strings
//       aren't callable, this should really be a problem and should go unnoticed.
#define OP_CLASS_SET_SLOT         OPARG(0x57) // second.__set_slot(unsigned(ARG),pop(0))
#define OP_CLASS_ADD_METHOD       OPARG(0x58) // second.__add_member(const(unsigned(ARG)),pop(0))
#define OP_CLASS_ADD_GETSET       OPARG(0x59) // fourth.__add_getset(const(unsigned(ARG)),pop(2),pop(1),pop(0))
#define OP_CLASS_ADD_CLASS_METHOD OPARG(0x5A) // second.__add_class_member(const(unsigned(ARG)),pop(0))
#define OP_CLASS_ADD_CLASS_GETSET OPARG(0x5B) // fourth.__add_class_getset(const(unsigned(ARG)),pop(2),pop(1),pop(0))
#define OP_SET                    OPARG(0x5C) // pop unsigned(ARG) values and pack them into a set (undefined behaviour if ARG == 0)
//UNUSED:                         OPARG(0x5D)
//UNUSED:                         OPARG(0x5E)
//UNUSED:                         OPARG(0x5F)
//UNUSED:                         OPARG(0x60)
//UNUSED:                         OPARG(0x61)
//UNUSED:                         OPARG(0x62)
//UNUSED:                         OPARG(0x63)
//UNUSED:                         OPARG(0x64)
//UNUSED:                         OPARG(0x65)
//UNUSED:                         OPARG(0x66)
//UNUSED:                         OPARG(0x67)
//UNUSED:                         OPARG(0x68)
//UNUSED:                         OPARG(0x69)
//UNUSED:                         OPARG(0x70)
//UNUSED:                         OPARG(0x7A)
//UNUSED:                         OPARG(0x7B)
//UNUSED:                         OPARG(0x7C)
//UNUSED:                         OPARG(0x7D)
#define OP_PREFIX                 OPARG(0x7E) // Special prefix including additional data for the next opcode (no-op if next opcode doesn't implement special handling)
#define OP_EXTENDED               OPARG(0x7F) // extended opcode (use op argument for any OPEXT_xxx opcode)

// Extended opcodes
#define OPEXT_TRY_INPLACE_ADD     OPEXT(0x0000) // push(({ res = pop(1) +=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) +  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_ADD)
#define OPEXT_TRY_INPLACE_SUB     OPEXT(0x0001) // push(({ res = pop(1) -=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) -  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SUB)
#define OPEXT_TRY_INPLACE_MUL     OPEXT(0x0002) // push(({ res = pop(1) *=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) *  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_MUL)
#define OPEXT_TRY_INPLACE_DIV     OPEXT(0x0003) // push(({ res = pop(1) /=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) /  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_DIV)
#define OPEXT_TRY_INPLACE_MOD     OPEXT(0x0004) // push(({ res = pop(1) %=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) %  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_MOD)
#define OPEXT_TRY_INPLACE_SHL     OPEXT(0x0005) // push(({ res = pop(1) <<= pop(0); pack res,res; })...); <or> push(({ res = pop(1) << pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SHL)
#define OPEXT_TRY_INPLACE_SHR     OPEXT(0x0006) // push(({ res = pop(1) >>= pop(0); pack res,res; })...); <or> push(({ res = pop(1) >> pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_SHR)
#define OPEXT_TRY_INPLACE_AND     OPEXT(0x0007) // push(({ res = pop(1) &=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) &  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_AND)
#define OPEXT_TRY_INPLACE_OR      OPEXT(0x0008) // push(({ res = pop(1) |=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) |  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_OR)
#define OPEXT_TRY_INPLACE_XOR     OPEXT(0x0009) // push(({ res = pop(1) ^=  pop(0); pack res,res; })...); <or> push(({ res = pop(1) ^  pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_XOR)
#define OPEXT_TRY_INPLACE_POW     OPEXT(0x000A) // push(({ res = pop(1) **= pop(0); pack res,res; })...); <or> push(({ res = pop(1) ** pop(0); pack res,res; })...); (Formerly: OPEXT_RET_INPLACE_POW)
//UNUSED:                         OPEXT(0x000B) // Formerly: OPEXT_LOAD_LAST_EXCEPTION
//UNUSED:                         OPEXT(0x000C) // Formerly: OPEXT_LOAD_LAST_TRACEBACK
// Same a the C function - allocates memory on the stack that will be
// freed once the function is left (allocates into a void* pointer)
// NOTE: Deemon will simulate the behavior of alloca if the native library doesn't support it.
//       So this is something you can always enable if you dare.
#define OPEXT_ALLOCA              OPEXT(0x000D) // push(__builtin_alloc(pop(0)))
#define OPEXT_BREAKPOINT          OPEXT(0x000E) // __builtin_breakpoint();
#define OPEXT_DOC                 OPEXT(0x000F) // push(__builtin_help(pop(0)))
#define OPEXT_DOC_ATTR            OPEXT(0x0010) // push(__builtin_help(pop(1).operator . (pop(0))))
#define OPEXT_EMPTY_TUPLE         OPEXT(0x0011) // push(pack())
#define OPEXT_EMPTY_LIST          OPEXT(0x0012) // push([])
#define OPEXT_EMPTY_DICT          OPEXT(0x0013) // push(dict {})
#define OPEXT_TRY_INPLACE_INC     OPEXT(0x0014) // push(({ val = pop(0); res = ++val; pack res,val; })...); <or> push(({ val = pop(0); res = val += 1; pack res,val; })...); <or> push(({ val = pop(0); res = val = val + 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_DEC     OPEXT(0x0015) // push(({ val = pop(0); res = ++val; pack res,val; })...); <or> push(({ val = pop(0); res = val -= 1; pack res,val; })...); <or> push(({ val = pop(0); res = val = val - 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_INC_POST OPEXT(0x0016) // push(({ val = pop(0); res = val++; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val += 1; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val = val + 1; pack res,val; })...);
#define OPEXT_TRY_INPLACE_DEC_POST OPEXT(0x0017) // push(({ val = pop(0); res = val--; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val -= 1; pack res,val; })...); <or> push(({ val = pop(0); res = copy val; val = val - 1; pack res,val; })...);
#define OPEXT_CLASS_NEW           OPEXT(0x0018) // push(__builtin_object(0x0173)(pop(1),pop(0)))
#define OPEXT_CLASS_NEW_UUID      OPEXT(0x0019) // push(__builtin_object(0x0173)(pop(2),pop(1),pop(0)))
#define OPEXT_EMPTY_SET           OPEXT(0x001A) // push(set {})
#define OPEXT_SUPER_AT            OPEXT(0x001B) // push(__super(pop(1),pop(0)))
#define OPEXT_EMPTY_CELL          OPEXT(0x001C) // push(<>)
#define OPEXT_BOUND_LOCAL         OPEXT(0x001D) // push(__builtin_bound(...)) (w/ OP_PREFIX)
#define OPEXT_BOUND_RESULT        OPEXT(0x001D) // push(__builtin_bound(...)) (w/o OP_PREFIX)
//UNUSED:                         OPEXT(0x001E)
#define OPEXT_DEL_RESULT          OPEXT(0x001F) // function foo: result { del result; }
#define OPEXT_SUPEROF             OPEXT(0x0020) // push(pop(0).super)
#define OPEXT_CLASSOF             OPEXT(0x0021) // push(pop(0).class)
// Iterate the sequence in top, discarding all elements it may produce
// - no-op if top is a builtin tuple/list/dict
// - This is used when expanding an expression without using its result
#define OPEXT_SEQ_ITERALL         OPEXT(0x0022)
#define OPEXT_SEQ_ANY             OPEXT(0x0023) // push(pop(0)||...)
#define OPEXT_SEQ_ALL             OPEXT(0x0024) // push(pop(0)&&...)
#define OPEXT_SEQ_SUM             OPEXT(0x0025) // push(pop(0)&&...)
#define OPEXT_TYPEFLAGS_LO        OPEXT(0x0026) // w/prefix or the argument to the type flags in top (low word: 0x0000FFFF)
#define OPEXT_TYPEFLAGS_HI        OPEXT(0x0027) // w/prefix or the argument to the type flags in top (high word: 0xFFFF0000)
#define OPEXT_SEQ_ITER_NEXT       OPEXT(0x0028) // Simple operator __iternext__ w/o a conditional jump (throws a Signal.StopIteration if the iterator is done)
#define OPEXT_TRY_CMP_EQ          OPEXT(0x0029) // try { push(pop(1) == pop(0)); } catch (Error.RuntimeError.NotImplemented) { push(false); }
//UNUSED:                         OPEXT(0x002A) // Formerly: OPEXT_VARRAYOF
// File Operators
#define OPEXT_IO_WRITEP           OPEXT(0x002B) // push(pop(2).operator __write__(pop(1),pop(0)))
#define OPEXT_IO_WRITENP          OPEXT(0x002C) // push(pop(1).operator __write__(pop(0)))
#define OPEXT_IO_READP            OPEXT(0x002D) // push(pop(2).operator __read__(pop(1),pop(0)))
#define OPEXT_IO_READNP           OPEXT(0x002E) // push(pop(1).operator __read__(pop(0)))
#define OPEXT_IO_SEEK             OPEXT(0x002F) //      pop(2).operator __seek__(pop(1),pop(0))
#define OPEXT_IO_SEEK_USED        OPEXT(0x0030) // push(pop(1).operator __seek__(pop(1),pop(0)))
#define OPEXT_IO_TRUNC            OPEXT(0x0031) //      pop(0).operator __trunc__()
#define OPEXT_IO_FLUSH            OPEXT(0x0032) //      pop(0).operator __flush__()
#define OPEXT_IO_CLOSE            OPEXT(0x0033) //      pop(0).operator __close__()
#define OPEXT_SEQ_MIN             OPEXT(0x0034) // push(pop(0)<...)
#define OPEXT_SEQ_MAX             OPEXT(0x0035) // push(pop(0)>...)


#ifdef DEE_OUTDATED_OPCODES
#if DEE_OUTDATED_OPCODES >= 102
#define v102_OP_STORE_RET_POP                                  0x54  /*< Replace with OP_STORE_RET + OP_POP. */
#define v102_OP_STORE_LOC_POP         OPARG(0x0A) /*< Replace with OP_STORE_LOC + OP_POP. */
#define v102_OP_STORE_CST_POP         OPARG(0x0E) /*< Replace with OP_STORE_CST + OP_POP. */
#define v102_OP_JUMP_IF_TT            OPARG(0x14) /*< Replace with OP_DUP + OP_JUMP_IF_TT_POP. */
#define v102_OP_JUMP_IF_FF            OPARG(0x15) /*< Replace with OP_DUP + OP_JUMP_IF_FF_POP. */
#endif
#if DEE_OUTDATED_OPCODES >= 101
#define v101_OP_STORE_ARG             OPARG(0x0B) /*< Converted to locals now. */
#define v101_OP_STORE_ARG_POP         OPARG(0x0C) /*< Converted to locals now. */
#define v101_OP_DEBUG_FILE            OPARG(0x24) /*< Uses a different system now. */
#endif
#if DEE_OUTDATED_OPCODES >= 100
// Outdated/unsupported opcodes
#define v100_OP_ASSIGN                OP_COPY_ASSIGN   /*< Renamed to indicate the kind of assignment. */
#define v100_OP_ROT_3                 OP_LROT_3        /*< Renamed to indicate rotation direction. */
#define v100_OP_ITERATE_SEQUENCE      OPREG(0x51)      /*< Use 'OP_EXTENDED+OPEXT_SEQ_ITERALL' instead. */
#define v100_OP_SEQ_ITER_NEXT         OP_SEQ_ITER_WALK /*< Renamed, as this isn't an operator opcode. */
#define v100_OP_PRINT_v100            OPARG(0x1C) /*< Uses a different system now. */
#define v100_OP_PRINT_PUSH_TPL        OPARG(0x1D) /*< *ditto*. */
#define v100_OP_PRINT_NOLF            OPARG(0x1E) /*< *ditto*. */
#define v100_OP_PRINT_PUSH_TPL_NOLF   OPARG(0x1F) /*< *ditto*. */
#define v100_OP_PRINT_F               OPARG(0x20) /*< *ditto*. */
#define v100_OP_PRINT_F_PUSH_TPL      OPARG(0x21) /*< *ditto*. */
#define v100_OP_PRINT_F_NOLF          OPARG(0x22) /*< *ditto*. */
#define v100_OP_PRINT_F_PUSH_TPL_NOLF OPARG(0x23) /*< *ditto*. */
#define v100_OP_DEBUG_LINE            OPARG(0x25) /*< Uses a different system now. */
#define v100_OP_FUNCTION_V            OPARG(0x27) /*< Uses a different system now. */
#define v100_OP_FUNCTION_Y            OPARG(0x28) /*< *ditto*. */
#define v100_OP_FUNCTION_YV           OPARG(0x29) /*< *ditto*. */
#define v100_OP_EXTERN_DECL           OPARG(0x2B) /*< Uses a different system now. */
#define v100_OP_CALL_MEMBER           OP_CALL_MEMBER_C           /*< Renamed to indicate attribute lookup by constant. */
#define v100_OP_EXTENDED_ARG          OP_PREFIX                  /*< Renamed as this opcode is not just used to OP_EXTENDED */
#define v100_OP_LOC_INPLACE_ADD_v100  OPARG(0x32) /*< These are now using a different system. */
#define v100_OP_LOC_INPLACE_SUB_v100  OPARG(0x33) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_MUL_v100  OPARG(0x34) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_DIV_v100  OPARG(0x35) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_MOD_v100  OPARG(0x36) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_SHL_v100  OPARG(0x37) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_SHR_v100  OPARG(0x38) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_AND_v100  OPARG(0x39) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_OR_v100   OPARG(0x3A) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_XOR_v100  OPARG(0x3B) /*< *ditto*. */
#define v100_OP_LOC_INPLACE_POW_v100  OPARG(0x3C) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_ADD_v100  OPARG(0x3D) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SUB_v100  OPARG(0x3E) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_MUL_v100  OPARG(0x3F) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_DIV_v100  OPARG(0x40) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_MOD_v100  OPARG(0x41) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SHL_v100  OPARG(0x42) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_SHR_v100  OPARG(0x43) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_AND_v100  OPARG(0x44) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_OR_v100   OPARG(0x45) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_XOR_v100  OPARG(0x46) /*< *ditto*. */
#define v100_OP_ARG_INPLACE_POW_v100  OPARG(0x47) /*< *ditto*. */
#define v100_OP_CST_INPLACE_ADD_v100  OPARG(0x48) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SUB_v100  OPARG(0x49) /*< *ditto*. */
#define v100_OP_CST_INPLACE_MUL_v100  OPARG(0x4A) /*< *ditto*. */
#define v100_OP_CST_INPLACE_DIV_v100  OPARG(0x4B) /*< *ditto*. */
#define v100_OP_CST_INPLACE_MOD_v100  OPARG(0x4C) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SHL_v100  OPARG(0x4D) /*< *ditto*. */
#define v100_OP_CST_INPLACE_SHR_v100  OPARG(0x4E) /*< *ditto*. */
#define v100_OP_CST_INPLACE_AND_v100  OPARG(0x4F) /*< *ditto*. */
#define v100_OP_CST_INPLACE_OR_v100   OPARG(0x50) /*< *ditto*. */
#define v100_OP_CST_INPLACE_XOR_v100  OPARG(0x51) /*< *ditto*. */
#define v100_OP_CST_INPLACE_POW_v100  OPARG(0x52) /*< *ditto*. */
#define v100_OPEXT_BOOL_USER          OPEXT(0x0014) /*< Call the bool constructor instead. */
#define v100_OPEXT_CODE_SELF          OPEXT(0x0015) /*< Call builtin object #0x0169 instead. */
#endif
#endif /* DEE_OUTDATED_OPCODES */


// Case-list of all opcodes with a jmp-argument
#define DEE_OPCODE_CASE_HAS_JMPARG\
  case OP_JUMP:\
/*case OP_JUMP_IF_TT:\
  case OP_JUMP_IF_FF:\
*/case OP_JUMP_IF_TT_POP:\
  case OP_JUMP_IF_FF_POP:\
  case OP_SEQ_ITER_WALK:\
  case OP_JUMP_IF_CASE_COMPARE:\
  case OP_JUMP_IF_RCASE_COMPARE:


#endif /* DEE_LIMITED_API */

#endif /* !GUARD_DEEMON___OPCODE_INL */
