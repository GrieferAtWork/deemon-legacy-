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

RULE({OP_LOAD_NONE,OP_RET} --> {OP_RETNONE});
RULE({OP_LOAD_THIS,OP_POP} --> {});

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
RULE({OP_BOOL,OP_JUMP_IF_TT,$off} --> {OP_JUMP_IF_TT,$off});
RULE({OP_BOOL,OP_JUMP_IF_FF,$off} --> {OP_JUMP_IF_FF,$off});
RULE({OP_BOOL,OP_JUMP_IF_TT_POP,$off} --> {OP_JUMP_IF_TT_POP,$off});
RULE({OP_BOOL,OP_JUMP_IF_FF_POP,$off} --> {OP_JUMP_IF_FF_POP,$off});

//////////////////////////////////////////////////////////////////////////
// Invert the condition of the jump opcode (and remove the OP_BOOL)
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_TT,$off} --> {OP_JUMP_IF_FF,$off});
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_FF,$off} --> {OP_JUMP_IF_TT,$off});
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_TT_POP,$off} --> {OP_JUMP_IF_FF_POP,$off});
RULE({OP_BOOL,OP_NOT,OP_JUMP_IF_FF_POP,$off} --> {OP_JUMP_IF_TT_POP,$off});

#define DEFINE_LOAD_RULES(op) \
 RULE({op,$arg,OP_POP} --> {});\
 RULE({op,$arg,op,$arg} --> {*,$arg,OP_DUP})

// Optimization rules for loading variables
DEFINE_LOAD_RULES(OP_LOAD_LOC);
DEFINE_LOAD_RULES(OP_LOAD_REF);
DEFINE_LOAD_RULES(OP_LOAD_CST);
DEFINE_LOAD_RULES(OP_LOAD_CST_COPY);
DEFINE_LOAD_RULES(OP_LOAD_CST_DCOPY);
DEFINE_LOAD_RULES(OP_LOAD_BLTIN);
DEFINE_LOAD_RULES(OP_LOAD_ARG);

//////////////////////////////////////////////////////////////////////////
// Unary opcodes with a stack-effect of '-1' (basic)
// e.g.: 'yield pop(0)' applies for this (-1), but 'push(pop(1)+pop(0))' (-2+1) doesn't
#define OPGROUP_BASIC_STACKEFFECT_M1\
 (OP_YIELD|OP_PRINT_ONE\
 |OP_PRINT_ONE_SEP|OP_PRINT_ONE_END\
 |OP_PRINT_ALL|OP_PRINT_ALL_SEP\
 |OP_PRINT_ALL_END|OP_PRINT_END_F)


//////////////////////////////////////////////////////////////////////////
// Optimization rules for OP_DUP
RULE({OP_DUP,OP_POP} --> {});
RULE({OP_DUP,OP_ROT_2} --> {*});
RULE({OP_DUP,OPGROUP_BASIC_STACKEFFECT_M1,OP_POP} --> {OP_NOOP,*,OP_NOOP});

//////////////////////////////////////////////////////////////////////////
// Optimize away rotations that cancle each other out
RULE({OP_ROT_2,OP_ROT_2} --> {});
RULE({OP_LROT_3,OP_RROT_3} --> {});
RULE({OP_RROT_3,OP_LROT_3} --> {});
RULE({OP_LROT_4,OP_RROT_4} --> {});
RULE({OP_RROT_4,OP_LROT_4} --> {});
RULE({OP_LROT_N,$shift,OP_RROT_N,$shift} --> {});
RULE({OP_RROT_N,$shift,OP_LROT_N,$shift} --> {});


//////////////////////////////////////////////////////////////////////////
// Optimize store + pop --> store_pop
RULE({OP_STORE_LOC,$locid,OP_POP} --> {OP_STORE_LOC_POP,$locid});
RULE({OP_STORE_ARG,$argid,OP_POP} --> {OP_STORE_ARG_POP,$argid});
RULE({OP_STORE_CST,$cstid,OP_POP} --> {OP_STORE_CST_POP,$cstid});
RULE({OP_STORE_RET,OP_POP} --> {OP_STORE_RET_POP});

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

#define OPGROUP_LOAD_CST  \
 (OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY)
#define OPGROUP_LOAD  \
 (OPGROUP_LOAD_CST|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN)

//////////////////////////////////////////////////////////////////////////
// Optimize store_pop + load --> store
RULE({OP_STORE_LOC_POP,$locid,OP_LOAD_LOC,$locid} --> {OP_STORE_LOC,$locid});
RULE({OP_STORE_ARG_POP,$argid,OP_LOAD_ARG,$argid} --> {OP_STORE_ARG,$argid});
RULE({OP_STORE_CST_POP,$cstid,OPGROUP_LOAD_CST,$cstid} --> {OP_STORE_CST,$cstid});
RULE({OP_STORE_RET_POP,OP_LOAD_RET} --> {OP_STORE_RET});


//////////////////////////////////////////////////////////////////////////
// Optimize pop_store x + load y + load x  --> store x + load y + rot 2
RULE({OP_STORE_LOC_POP,$locid,OPGROUP_LOAD,$locid2,OP_LOAD_LOC,$locid} --> {OP_STORE_LOC,$locid,*,$locid2,OP_ROT_2});
RULE({OP_STORE_ARG_POP,$locid,OPGROUP_LOAD,$locid2,OP_LOAD_ARG,$locid} --> {OP_STORE_ARG,$locid,*,$locid2,OP_ROT_2});
RULE({OP_STORE_CST_POP,$locid,OPGROUP_LOAD,$locid2,OPGROUP_LOAD_CST,$locid} --> {OP_STORE_CST,$locid,*,$locid2,OP_ROT_2});
RULE({OP_STORE_RET_POP,OPGROUP_LOAD,$locid2,OP_LOAD_RET} --> {OP_STORE_RET,*,$locid2,OP_ROT_2});

