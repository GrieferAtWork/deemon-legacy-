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
#define DEE_LIMITED_API
#include "__opcode.inl"
#define RULE(...)
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// === NOTICE ===
//   - After making changes to this file, you must
//     run '$ deemon codewriter.peephole.make.dee'
//
// === HINTS ===
//   - Be careful with what you do in here, as thoughtless, or nonsensical
//     rules will lead to incorrect code being generated.
//   - Checks for protected opcodes are done automatically
//   - OP_NOOP is skipped automatically
//
// === SYNTAX ===
//   - The general syntax of a rule definition is:
//     >> opcode_names = '|' ~~ keyword...;
//     >> opname ::= ('*' | opcode_names | keyword '[' * ']' | '$' ## keyword | opcode_names '...');
//     >> oplist ::= '{' [',' ~~ opname...] '}';
//     >> @rule_def ::= 'RULE' '(' oplist  '-->' oplist ')' ';';
//   - The pattern described by the left oplist (from now on referred to as 'input')
//     is searched for, and replaced with that of the right one (from now on referred to as 'output')
//   - Using '*' as 'opname' is only allowed in the output and instructs
//     the peephole optimizer to leave the opcode located at that position untouched.
//     Note, that while the input is matching the output, code will never be modified.
//   - Adding a '...' suffix to a keyword will instruct the peephole optimizer
//     to skip any amount of any listed opcode before matching against what codes after.
//     NOTE: Using this opcode in the input, also requires use in the output
//   - Using a 'keyword' in an 'opname' refers to a named
//     opcode, using the same names as those listed in "__opcode.inl".
//   - Using a 'keyword' prefixed with a '$' in 'input' refers to a runtime-value,
//     that is initialized with the argument of its first occurrence.
//     If re-used at a later point, a check is performed to ensure that the
//     argument found at the new position matches that of the initial position.
//     The argument name can then be re-used in the output, where it will be filled in
//     with the initial value read during parsing of the input. e.g.:
//     >> // Optimize 2x load local --> load once, then dup.
//     >> RULE({OP_LOAD_LOC,$locid,OP_LOAD_LOC,$locid} --> {OP_LOAD_LOC,$locid,OP_DUP});
//   - Using a '[' suffix after a keyword instructs the runtime to
//     match the argument following the opcode against the given value.
//   - More than one runtime value may be used, but different names must be chosen.
//   - The total byte count described by the output can not exceed that of the input.
//     If it does, the rule table cannot be generated and 'codewriter.peephole.make.dee'
//     will return with an error.
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define OPGROUP_LOAD          (OP_LOAD_THIS|OP_LOAD_RET)
#define OPGROUP_LOAD_CST      (OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY|OP_LOAD_CST_LOCKED)
#define OPGROUP_LOAD_ARG      (OPGROUP_LOAD_CST|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN)
#define OPGROUP_STORE         (OP_STORE_RET)
#define OPGROUP_STORE_ARG     (OP_STORE_LOC|OP_STORE_CST)

//////////////////////////////////////////////////////////////////////////
// Stack-effect opgroups
// NOTE: 'ns' means no stack, meaning that the opcode doesn't require any existing stack entries
#define /* +1   */OPGROUP_SE_P1        (OPGROUP_LOAD)
#define /* +1   */OPGROUP_SE_P1_ARG    (OPGROUP_LOAD_ARG)
#define /* -1   */OPGROUP_SE_M1       \
 (OP_POP|OP_YIELD|OP_PRINT_ONE|OP_PRINT_ONE_SEP\
 |OP_PRINT_ONE_END|OP_PRINT_ALL|OP_PRINT_ALL_SEP\
 |OP_PRINT_ALL_END|OP_PRINT_END_F)
#define /* -1   */OPGROUP_SE_M1_ARG    ()
#define /* -1+1 */OPGROUP_SE_M1_P1     (OPGROUP_STORE)
#define /* -1+1 */OPGROUP_SE_M1_P1_ARG (OPGROUP_STORE_ARG)
#define /* -1+2 */OPGROUP_SE_M1_P2     (OP_DUP)
#define /* -1+2 */OPGROUP_SE_M1_P2_ARG ()


RULE({OP_LOAD_NONE,OP_RET} --> {OP_RETNONE});
RULE({OP_LOAD_NONE,OP_POP} --> {});

//////////////////////////////////////////////////////////////////////////
// Drop unnecessary tostring op-calls
// >> OP_STR and OP_REPR must always return a string,
//    meaning that calling OP_STR on their result has no effect
RULE({OP_STR,OP_STR} --> {OP_STR});
RULE({OP_REPR,OP_STR} --> {OP_REPR});

//////////////////////////////////////////////////////////////////////////
// Drop unnecessary OP_BOOL op-calls
// >> OP_BOOL always returns a boolean, meaning that calling OP_BOOL
//    again has no effect
RULE({OP_BOOL,OP_BOOL} --> {OP_BOOL});
RULE({OP_BOOL,OP_NOT,OP_NOT} --> {OP_BOOL});

//////////////////////////////////////////////////////////////////////////
// The conditional jump opcodes already perform their own to-bool cast
// NOTE: The prefixed OP_NOOP are required to keep the jump offsets aligned
//       Though they are optimized away later, when all other no-ops are removed
//RULE({OP_BOOL,OP_JUMP_IF_TT,$off} --> {OP_NOOP,OP_JUMP_IF_TT,$off});
//RULE({OP_BOOL,OP_JUMP_IF_FF,$off} --> {OP_NOOP,OP_JUMP_IF_FF,$off});
RULE({OP_BOOL,OP_JUMP_IF_TT_POP,$off} --> {OP_NOOP,OP_JUMP_IF_TT_POP,$off});
RULE({OP_BOOL,OP_JUMP_IF_FF_POP,$off} --> {OP_NOOP,OP_JUMP_IF_FF_POP,$off});

//////////////////////////////////////////////////////////////////////////
// Invert the condition of the jump opcode (and remove the OP_BOOL)
// NOTE: The prefixed OP_NOOP are required to keep the jump offsets aligned
//       Though they are optimized away later, when all other no-ops are removed
//RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_TT,$off} --> {OP_NOOP,OP_NOOP,OP_JUMP_IF_FF,$off});
//RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_FF,$off} --> {OP_NOOP,OP_NOOP,OP_JUMP_IF_TT,$off});
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_TT_POP,$off} --> {OP_NOOP,OP_NOOP,OP_JUMP_IF_FF_POP,$off});
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_FF_POP,$off} --> {OP_NOOP,OP_NOOP,OP_JUMP_IF_TT_POP,$off});

#define DEFINE_LOAD_RULES(op) \
 RULE({op,$arg,OP_POP} --> {});\
 RULE({op,$arg,OP_DUP...,op,$arg} --> {op,$arg,OP_DUP...,OP_DUP})

// Optimization rules for loading variables
DEFINE_LOAD_RULES(OP_LOAD_LOC);
DEFINE_LOAD_RULES(OP_LOAD_REF);
DEFINE_LOAD_RULES(OP_LOAD_CST);
DEFINE_LOAD_RULES(OP_LOAD_CST_COPY);
DEFINE_LOAD_RULES(OP_LOAD_CST_DCOPY);
DEFINE_LOAD_RULES(OP_LOAD_CST_LOCKED);
DEFINE_LOAD_RULES(OP_LOAD_BLTIN);
DEFINE_LOAD_RULES(OP_LOAD_ARG);

//////////////////////////////////////////////////////////////////////////
// Optimizations for code like:
// >> local x = 42;
// >> x = x; // totally unnecessary
RULE({OP_LOAD_RET,OP_STORE_RET} --> {OP_LOAD_RET});
RULE({OP_LOAD_LOC,$loc,OP_STORE_LOC,$loc} --> {OP_LOAD_LOC,$loc});
RULE({OP_LOAD_CST,$loc,OP_STORE_CST,$loc} --> {OP_LOAD_LOC,$loc});


RULE({OP_LOAD_THIS,OP_POP} --> {});
RULE({OP_LOAD_THIS,OP_DUP...,OP_LOAD_THIS} --> {OP_LOAD_THIS,OP_DUP...,OP_DUP});
RULE({OP_LOAD_RET,OP_POP} --> {});
RULE({OP_LOAD_RET,OP_DUP...,OP_LOAD_RET} --> {OP_LOAD_RET,OP_DUP...,OP_DUP});

//////////////////////////////////////////////////////////////////////////
// Unary opcodes with a stack-effect of '-1' (basic)
// e.g.: 'yield pop(0)' applies for this (-1), but 'push(pop(1)+pop(0))' (-2+1) doesn't


//////////////////////////////////////////////////////////////////////////
// Optimization rules for OP_DUP
RULE({OP_DUP,OP_POP} --> {});
RULE({OP_DUP,OP_ROT_2} --> {OP_DUP});
RULE({OP_DUP,OPGROUP_SE_M1,OP_POP} --> {OP_NOOP,*,OP_NOOP});
//RULE({OP_DUP,OPGROUP_SE_M1_ARG,$searg1,OP_POP} --> {OP_NOOP,*,$searg1,OP_NOOP});

//////////////////////////////////////////////////////////////////////////
// Optimize away rotations that cancel each other out
RULE({OP_ROT_2,OP_ROT_2} --> {});
RULE({OP_LROT_3,OP_RROT_3} --> {});
RULE({OP_RROT_3,OP_LROT_3} --> {});
RULE({OP_LROT_4,OP_RROT_4} --> {});
RULE({OP_RROT_4,OP_LROT_4} --> {});
RULE({OP_LROT_N,$shift,OP_RROT_N,$shift} --> {});
RULE({OP_RROT_N,$shift,OP_LROT_N,$shift} --> {});
RULE({OP_ROT_2,OP_POP,OP_POP} --> {OP_POP,OP_POP});
RULE({OP_LROT_3,OP_POP,OP_POP,OP_POP} --> {OP_POP,OP_POP,OP_POP});
RULE({OP_RROT_3,OP_POP,OP_POP,OP_POP} --> {OP_POP,OP_POP,OP_POP});
RULE({OP_LROT_4,OP_POP,OP_POP,OP_POP,OP_POP} --> {OP_POP,OP_POP,OP_POP,OP_POP});
RULE({OP_RROT_4,OP_POP,OP_POP,OP_POP,OP_POP} --> {OP_POP,OP_POP,OP_POP,OP_POP});

//////////////////////////////////////////////////////////////////////////
// Optimize sequence T1+cast T2 --> sequence T2
RULE({OP_TUPLE,$size,OP_CAST_TUPLE} --> {OP_TUPLE,$size});
RULE({OP_TUPLE,$size,OP_CAST_LIST} --> {OP_LIST,$size});
RULE({OP_TUPLE,$size,OP_CAST_SET} --> {OP_SET,$size});
RULE({OP_LIST,$size,OP_CAST_TUPLE} --> {OP_TUPLE,$size});
RULE({OP_LIST,$size,OP_CAST_LIST} --> {OP_LIST,$size});
RULE({OP_LIST,$size,OP_CAST_SET} --> {OP_SET,$size});
RULE({OP_SET,$size,OP_CAST_TUPLE} --> {OP_TUPLE,$size});
RULE({OP_SET,$size,OP_CAST_LIST} --> {OP_LIST,$size});
RULE({OP_SET,$size,OP_CAST_SET} --> {OP_SET,$size});

//////////////////////////////////////////////////////////////////////////
// Optimize store + pop + load --> store
RULE({OP_STORE_LOC,$locid,OP_POP,OP_LOAD_LOC,$locid} --> {OP_STORE_LOC,$locid});
RULE({OP_STORE_CST,$cstid,OP_POP,OPGROUP_LOAD_CST,$cstid} --> {OP_STORE_CST,$cstid});
RULE({OP_STORE_RET,OP_POP,OP_LOAD_RET} --> {OP_STORE_RET});

//////////////////////////////////////////////////////////////////////////
// Optimize store x + pop + s+1 + load x  --> store x + s+1 + rot 2
RULE({OP_STORE_LOC,$locid,OP_POP,OPGROUP_SE_P1,OP_LOAD_LOC,$locid} --> {OP_STORE_LOC,$locid,*,OP_ROT_2});
RULE({OP_STORE_CST,$locid,OP_POP,OPGROUP_SE_P1,OPGROUP_LOAD_CST,$locid} --> {OP_STORE_CST,$locid,*,OP_ROT_2});
RULE({OP_STORE_RET,OP_POP,OPGROUP_SE_P1,OP_LOAD_RET} --> {OP_STORE_RET,*,OP_ROT_2});
RULE({OP_STORE_LOC,$locid,OP_POP,OPGROUP_SE_P1_ARG,$searg1,OP_LOAD_LOC,$locid} --> {OP_STORE_LOC,$locid,*,$searg1,OP_ROT_2});
RULE({OP_STORE_CST,$locid,OP_POP,OPGROUP_SE_P1_ARG,$searg1,OPGROUP_LOAD_CST,$locid} --> {OP_STORE_CST,$locid,*,$searg1,OP_ROT_2});
RULE({OP_STORE_RET,OP_POP,OPGROUP_SE_P1_ARG,$searg1,OP_LOAD_RET} --> {OP_STORE_RET,*,$searg1,OP_ROT_2});


//////////////////////////////////////////////////////////////////////////
// Optimize return form a retvar function
RULE({OP_STORE_RET,OP_POP,OP_RETVAREXIT} --> {OP_RET});


//////////////////////////////////////////////////////////////////////////
// Optimize away empty sequences / Unnecessary / no-op opcodes
RULE({OP_EXTENDED[OPEXT_EMPTY_TUPLE],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_EMPTY_LIST],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_EMPTY_DICT],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_EMPTY_SET],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_EMPTY_CELL],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_BOUND_LOCAL],OP_POP} --> {});
RULE({OP_EXTENDED[OPEXT_SUPEROF],OP_POP} --> {OP_POP});
RULE({OP_EXTENDED[OPEXT_CLASSOF],OP_POP} --> {OP_POP});
RULE({OP_EXTENDED[OPEXT_SUPER_AT],OP_POP} --> {OP_POP,OP_POP});
RULE({OP_EXTENDED[OPEXT_CLASS_NEW],OP_POP} --> {OP_POP,OP_POP});
RULE({OP_EXTENDED[OPEXT_CLASS_NEW_UUID],OP_POP} --> {OP_POP,OP_POP,OP_POP});
RULE({OP_EXTENDED[OPEXT_ALLOCA],OP_POP} --> {OP_POP,OP_POP});


