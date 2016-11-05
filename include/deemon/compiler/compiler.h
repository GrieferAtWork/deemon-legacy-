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
#ifndef GUARD_DEEMON_COMPILER_H
#define GUARD_DEEMON_COMPILER_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

DEE_STRUCT_DEF(DeeCompilerConfig);

//////////////////////////////////////////////////////////////////////////
// Parse flags (used by various DeeXAst_Parse* / DeeSAst_Parse* / ... function)
#define DEE_PARSER_FLAG_NONE  0x00000000
 
//////////////////////////////////////////////////////////////////////////
// Optimization flags while parsing ASTs
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE  0x00000001 /*< Allow automatic generation of constant lists (e.h.: '[10,20]'). */
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS 0x00000002 /*< Allow automatic generation of constant operators (e.g.: '10+20'). */
#define DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS 0x00000004 /*< Merge some operators to optimize code. */
#define DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH     0x00000008 /*< Optimize away dead branches in conditional code (e.g.: 'true?10:20'). */
#define DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT    0x00000010 /*< Optimize print calls to stdout. */
#define DEE_PARSER_FLAG_OPTIMIZE_LOOPS           0x00000020 /*< Optimize loop constructs. */
#define DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS      0x00000040 /*< Optimize throw and try statements. */
#define DEE_PARSER_FLAG_OPTIMIZE_UNWIND_LOOPS    0x00000080 /*< Unwind loops. */
#define DEE_PARSER_FLAG_NO_ASSERTIONS            0x00000100 /*< Disable assert statements. */
#define DEE_PARSER_FLAG_CONST_FUNCTION           0x00000200 /*< Allow automatic generation of constant functions/classes (aka. functions not having any references). NOTE: Enabling this prevents certain other optimizations from being performed! */
#define DEE_PARSER_FLAG_OPTIMIZE_PREDICATES      0x00000400 /*< Optimize known predicates ("[](x,y) -> x+y" -> "__add__"). */
#define DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS 0x00000800 /*< [v102+] Optimize calls to intrinsic functions into operator calls. */
#define DEE_PARSER_FLAG_OPTIMIZE_MULTIVARDECL    0x00001000 /*< [v102+] Optimize multivardecl expressions. */
#ifdef DEE_LIMITED_API
#define DEE_PARSER_FLAG_NO_RANGE_INITIALIZER     0x10000000 /*  [Internal] Don't parse range initializers in post-unary expressions. */
#define DEE_PARSER_FLAG_NO_OPTIMIZE_SUBSCRIPT    0x20000000 /*< [Internal] Don't optimize subscript operators (used inside of '__builtin_help'). */
#define DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE     0x40000000 /*< [Internal] Set when break/continue is allowed. */
#endif

// Internal set of flags used to generate constants
#define DEE_PARSER_FLAG_GENERATE_CONSTANT \
(DEE_PARSER_FLAG_CONST_FUNCTION\
|DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE\
|DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS\
|DEE_PARSER_FLAG_OPTIMIZE_PREDICATES\
|DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS)


#define DEE_PARSER_FLAG_DEBUG(debug) \
(Dee_uint32_t)((debug)?0:DEE_PARSER_FLAG_NO_ASSERTIONS)

// Parser flags used for fast compilation
#define DEE_PARSER_FLAG_FASTEST(debug)\
(DEE_PARSER_FLAG_DEBUG(debug)\
|DEE_PARSER_FLAG_NONE)

// Default parser flags
#define DEE_PARSER_FLAG_DEFAULT(debug)\
(DEE_PARSER_FLAG_DEBUG(debug)\
|DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE\
|DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)

#define DEE_PARSER_FLAG_OPTIMIZE(debug)\
(DEE_PARSER_FLAG_DEFAULT(debug)\
|DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS\
|DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH\
|DEE_PARSER_FLAG_OPTIMIZE_UNWIND_LOOPS\
|DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS\
|DEE_PARSER_FLAG_OPTIMIZE_PREDICATES\
|DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS)

// Parser flags used for highest safe level of optimization
#define DEE_PARSER_FLAG_OPTIMIZE_ALL(debug)\
(DEE_PARSER_FLAG_OPTIMIZE(debug)\
|DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT\
|DEE_PARSER_FLAG_OPTIMIZE_LOOPS\
|DEE_PARSER_FLAG_OPTIMIZE_MULTIVARDECL)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// Optimization flags - Used by:
//  - DeeXAst_Optimize
//  - DeeSAst_Optimize
#define DEE_OPTIMIZE_FLAG_NONE            0x00000000
#define DEE_OPTIMIZE_FLAG_UNUSED_VAR      0x00000001 /*< Optimize away unused variables. */
#define DEE_OPTIMIZE_FLAG_DEL_NOEFFECT    0x00000002 /*< Delete ASTs and AST parts that don't have any side-effects. */
#define DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE 0x00000004 /*< Delete ASTs that are unreachable. */
#define DEE_OPTIMIZE_FLAG_UNINIT_BOUND    0x00000008 /*< Optimize away __builtin_bound calls on uninitialized variables. */
#define DEE_OPTIMIZE_FLAG_MERGE_OPERATORS 0x00000010 /*< Merge/move operator calls to reduce code size. */
#define DEE_OPTIMIZE_FLAG_MERGE_CONSTS    0x00000020 /*< Merge constant ASTs (again). */
#define DEE_OPTIMIZE_FLAG_ASSUMPTIONS     0x00000040 /*< Generate assumptions about variable and use them to optimize code. */
#define DEE_OPTIMIZE_FLAG_CONST_FUNCTION  0x00000080 /*< Optimize functions into constant (required for inline optimizations). */
#ifdef DEE_LIMITED_API
#define DEE_OPTIMIZE_FLAG_USED            0x10000000 /*< [internal] Set if the optimization result is used. */
#endif


#define DEE_OPTIMIZE_FLAG_DEBUG(debug)\
(Dee_uint32_t)((debug)?0:0)

// Optimizer flags used for fast compilation
// NOTE: If this set of flags is set, optimization is skipped
#define DEE_OPTIMIZE_FLAG_FASTEST(debug) \
(DEE_OPTIMIZE_FLAG_DEBUG(debug)\
|DEE_OPTIMIZE_FLAG_NONE)

// Default optimizer flags
#define DEE_OPTIMIZE_FLAG_DEFAULT(debug) \
(DEE_OPTIMIZE_FLAG_DEBUG(debug)\
|DEE_OPTIMIZE_FLAG_UNUSED_VAR\
|DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)

#define DEE_OPTIMIZE_FLAG_OPTIMIZE(debug)\
(DEE_OPTIMIZE_FLAG_DEFAULT(debug)\
|DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE\
|DEE_OPTIMIZE_FLAG_CONST_FUNCTION)

// Optimizer flags used for highest safe level of optimization
#define DEE_OPTIMIZE_FLAG_OPTIMIZE_ALL(debug)\
(DEE_OPTIMIZE_FLAG_OPTIMIZE(debug)\
|DEE_OPTIMIZE_FLAG_UNINIT_BOUND\
|DEE_OPTIMIZE_FLAG_MERGE_OPERATORS\
|DEE_OPTIMIZE_FLAG_MERGE_CONSTS\
|DEE_OPTIMIZE_FLAG_ASSUMPTIONS)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// Compile flags - Used by:
//  - DeeXAst_Compile
//  - DeeSAst_Compile

// Default flag (no effect)
#define DEE_COMPILER_FLAG_NONE 0x00000000

#ifdef DEE_LIMITED_API
// Set, if the return value of an expression is used
// - If not set, DeeExprAst_Compile will leave the code with
//   the same amount of stack entries as before
// - When set while compiling a statement, the statement
//   will leave a value on the stack as well
#define DEE_COMPILER_FLAG_USED                 0x10000000
#define DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE 0x20000000 /*< [Internal] Set when break/continue is allowed. */
#endif

// Include file+line debug information when compiling
#define DEE_COMPILER_FLAG_DEBUG_FILE   0x00000001 /*< Include filename information for debugging. */
#define DEE_COMPILER_FLAG_DEBUG_LINE   0x00000002 /*< Include line information for debugging. */
#define DEE_COMPILER_FLAG_DEBUG_FUNC   0x00000004 /*< Include function name information for debugging. */
#define DEE_COMPILER_FLAG_DEBUG_LOCALS 0x00000008 /*< Include local variable name information for debugging. */
#define DEE_COMPILER_FLAG_DEBUG_EXPR   0x00000010 /*< Include debug information on every expression (quite excessive). */
#define DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE \
 (DEE_COMPILER_FLAG_DEBUG_FILE|DEE_COMPILER_FLAG_DEBUG_LINE)


// Generate unreachable opcode where-ever code that shouldn't be reachable is located
// >> This causes a runtime-error to be thrown, instead of actual undefined behavior
// Note though, that unreachable should still be considered to be undefined behavior
#define DEE_COMPILER_FLAG_GEN_UNREACHABLE 0x00000100

// Generate new_instance opcodes instead of call opcodes when calling a type
#define DEE_COMPILER_FLAG_OPTIMIZE_CONSTRUCTOR_CALLS 0x00000200

#define DEE_COMPILER_FLAG_OPTIMIZE_PEEPHOLE     0x00001000 /*< Perform peephole optimizations on the generated code. */
#define DEE_COMPILER_FLAG_OPTIMIZE_DELNOOP      0x00002000 /*< Delete noop opcodes (should be used w/ 'DEE_COMPILER_FLAG_OPTIMIZE_PEEPHOLE'). */
#define DEE_COMPILER_FLAG_OPTIMIZE_REUSE_LOCALS 0x00004000 /*< Re-use local variable ids (breaks local variable names) */
#define DEE_COMPILER_FLAG_OPTIMIZE_REUSE_CONSTS 0x00008000 /*< Re-use constants that are deep-equal */


#define DEE_COMPILER_FLAG_DEBUG(debug) \
(Dee_uint32_t)((debug)?(\
 DEE_COMPILER_FLAG_GEN_UNREACHABLE\
|DEE_COMPILER_FLAG_DEBUG_FILE\
|DEE_COMPILER_FLAG_DEBUG_LINE\
|DEE_COMPILER_FLAG_DEBUG_FUNC\
|DEE_COMPILER_FLAG_DEBUG_LOCALS):0)


#define DEE_COMPILER_FLAG_FASTEST(debug)\
(DEE_COMPILER_FLAG_DEBUG(debug)\
|DEE_COMPILER_FLAG_NONE)
#define DEE_COMPILER_FLAG_DEFAULT(debug)\
(DEE_COMPILER_FLAG_DEBUG(debug)\
|DEE_COMPILER_FLAG_OPTIMIZE_DELNOOP)

#define DEE_COMPILER_FLAG_OPTIMIZE(debug)\
(DEE_COMPILER_FLAG_DEFAULT(debug)\
|DEE_COMPILER_FLAG_OPTIMIZE_CONSTRUCTOR_CALLS\
|DEE_COMPILER_FLAG_OPTIMIZE_REUSE_CONSTS)

// Compiler flags used for highest safe level of optimization
#define DEE_COMPILER_FLAG_OPTIMIZE_ALL(debug)\
(DEE_COMPILER_FLAG_OPTIMIZE(debug)\
|DEE_COMPILER_FLAG_OPTIMIZE_REUSE_LOCALS\
|DEE_COMPILER_FLAG_OPTIMIZE_PEEPHOLE)




struct DeeCompilerConfig {
 Dee_uint32_t parser_flags;   /*< Any combination of 'DEE_PARSER_FLAG_*'. */
 Dee_uint32_t optimize_flags; /*< Any combination of 'DEE_OPTIMIZE_FLAG_*'. */
 Dee_uint32_t compiler_flags; /*< Any combination of 'DEE_COMPILER_FLAG_*'. */
 DeeObject   *main_name;      /*< [0..1] Name of the root scope (Must be a DeeStringObject). */
 Dee_uint32_t padding[16];    /*< Padding data (initialize to ZERO) */
};

// Use this macro for default initialization
#define DeeCompilerConfig_INIT_EX(parser_flags,optimize_flags,compiler_flags) \
 {parser_flags,optimize_flags,compiler_flags,NULL,\
 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
// Generic initializers for commonly used compiler configurations
#define DeeCompilerConfig_INIT_FASTEST(debug)      DeeCompilerConfig_INIT_EX(DEE_PARSER_FLAG_FASTEST(debug),DEE_OPTIMIZE_FLAG_FASTEST(debug),DEE_COMPILER_FLAG_DEFAULT(debug))
#define DeeCompilerConfig_INIT_DEFAULT(debug)      DeeCompilerConfig_INIT_EX(DEE_PARSER_FLAG_DEFAULT(debug),DEE_OPTIMIZE_FLAG_DEFAULT(debug),DEE_COMPILER_FLAG_DEFAULT(debug))
#define DeeCompilerConfig_INIT_OPTIMIZE(debug)     DeeCompilerConfig_INIT_EX(DEE_PARSER_FLAG_OPTIMIZE(debug),DEE_OPTIMIZE_FLAG_OPTIMIZE(debug),DEE_COMPILER_FLAG_DEFAULT(debug))
#define DeeCompilerConfig_INIT_OPTIMIZE_ALL(debug) DeeCompilerConfig_INIT_EX(DEE_PARSER_FLAG_OPTIMIZE_ALL(debug),DEE_OPTIMIZE_FLAG_OPTIMIZE_ALL(debug),DEE_COMPILER_FLAG_DEFAULT(debug))
#define DeeCompilerConfig_INIT()  DeeCompilerConfig_INIT_DEFAULT(1)

//////////////////////////////////////////////////////////////////////////
// Default compiler configuration, if the config argument is NULL
// >> You can modify this global however you like, but know that
//    its original default value is restored when deemon is reloaded
//    (when Dee_Initialize() is called after Dee_Finalize())
DEE_DATA_DECL(DeeCompilerConfig) DeeCompilerConfig_Default;


#if DEE_DEPRECATED_API_VERSION(100,101,102)
//////////////////////////////////////////////////////////////////////////
// Deprecated compiler flags
// >> Don't use anymore
#define DEE_COMPILE_FLAG_INCLUDE_DEBUGINFO_FILE_AND_LINE        DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE
#define DEE_COMPILE_FLAG_INCLUDE_DEBUGINFO_FUNCNAME             DEE_COMPILER_FLAG_DEBUG_FUNC
#define DEE_COMPILE_FLAG_OPTIMIZE_UNNECESSARY_INTRINSIC_CALLS   0x00000000
#define DEE_COMPILE_FLAG_OPTIMIZE_PARTIAL_CONST_INTRINSIC_CALLS 0x00000000
#define DEE_COMPILE_FLAG_PUT_UNREACHABLE_OPCODES                DEE_COMPILER_FLAG_GEN_UNREACHABLE
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_LIST                     DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_TUPLE                    DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_DICT                     DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE
#define DEE_PARSER_FLAG_OPTIMIZE_CONST_SET                      DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE
#define DEE_PARSER_FLAG_OPTIMIZE_FOR                            DEE_PARSER_FLAG_OPTIMIZE_LOOPS
#define DEE_PARSER_FLAG_OPTIMIZE_FOR_IN                         DEE_PARSER_FLAG_OPTIMIZE_LOOPS
#define DEE_PARSER_FLAG_OPTIMIZE_IF                             DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH
#define DEE_PARSER_FLAG_OPTIMIZE_WHILE                          DEE_PARSER_FLAG_OPTIMIZE_LOOPS
#define DEE_PARSER_FLAG_MAYBE_DICT                              0x00000000
#endif


DEE_DECL_END

#endif /* !GUARD_DEEMON_COMPILER_H */
