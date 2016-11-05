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
#ifndef GUARD_DEEMON_XAST_H
#define GUARD_DEEMON_XAST_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#ifndef DEE_LIMITED_API
#error "Limited-only header"
#endif

#include <deemon/__conf.inl>
#include <deemon/compiler/codewriter.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/scope.h>
#include <deemon/optional/uuid.h>
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#include <deemon/type.h>

//////////////////////////////////////////////////////////////////////////
// XAst -- v102+ Expression Ast replacement
//
//   - This time, let's do things right (Griefer@Work, 2016)
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Branch prediction when it comes to detection of recursive c-types
// >> This assumption is probably true for most usercode, but
//    when this is enabled, parsing a c-header could be quite slow...
#if 1
#define DEE_RECURSIVE_CTYPE_UNLIKELY  DEE_UNLIKELY
#else
#define DEE_RECURSIVE_CTYPE_UNLIKELY  /* nothing */
#endif

//////////////////////////////////////////////////////////////////////////
// Deemon v101:1 didn't follow this rule, yet it must
// be enabled for correct parsing of c declarations.
#ifndef DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_LOCAL
#define DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_LOCAL 1
#endif
#ifndef DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND
#define DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND 1
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

DEE_OBJECT_DEF(DeeXAstObject);
DEE_OBJECT_DEF(DeeSAstObject);

#define DEE_XASTFLAG_ZEROOP              DEE_UINT32_C(0x80000000)
#define DEE_XASTFLAG_UNARY               DEE_UINT32_C(0x10000000)
#define DEE_XASTFLAG_BINARY              DEE_UINT32_C(0x20000000)
#define DEE_XASTFLAG_TRINARY             DEE_UINT32_C(0x30000000)
#define DEE_XASTKIND_ISZEROOP(x)   (((x)&DEE_UINT32_C(0x80000000))!=0)
#define DEE_XASTKIND_OPCOUNT(x)    (((x)&DEE_UINT32_C(0x70000000))>>28)
#define DEE_XASTKIND_ISOPERATOR(x) (((x)&DEE_UINT32_C(0xF0000000))!=0)
#define DEE_XASTKIND_ISUNARY(x)    (DEE_XASTKIND_OPCOUNT(x)==1)
#define DEE_XASTKIND_ISBINARY(x)   (DEE_XASTKIND_OPCOUNT(x)==2)
#define DEE_XASTKIND_ISTRINARY(x)  (DEE_XASTKIND_OPCOUNT(x)==3)

#define DEE_XASTKIND_NONE                       0
#define DEE_XASTKIND_CONST                      1 /*< 42 */
#define DEE_XASTKIND_VAR                        2 /*< x */
#define DEE_XASTKIND_VARDECL                    3 /*< x = y */
#define DEE_XASTKIND_MULTIVARDECL               4 /*< x,y,z = v... */
#define DEE_XASTKIND_STATEMENT                  5 /*< ({ ... }) */
#define DEE_XASTKIND_IF                         6 /*< a ? b : c */
#define DEE_XASTKIND_FUNCTION                   7 /*< function foo() -> 42 */
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_XASTKIND_CLASS                      8 /*< class foo { this() {} } */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#define DEE_XASTKIND_TUPLE                      9 /*< (a,b,c) */
#define DEE_XASTKIND_LIST                      10 /*< [a,b,c] */
#define DEE_XASTKIND_SET                       11 /*< set { a,b,c } */
#define DEE_XASTKIND_DICT                      12 /*< dict { a: b, c: 42 } */
#define DEE_XASTKIND_SEQ_RANGE_GET             20 /*< x[y:z] */
#define DEE_XASTKIND_SEQ_RANGE_DEL             21 /*< del x[y:z] */
#define DEE_XASTKIND_SEQ_RANGE_SET             22 /*< x[y:z] = w */
#define DEE_XASTKIND_ATTR_GET_C                23 /*< x.foobar */
#define DEE_XASTKIND_ATTR_HAS_C                24 /*< __builtin_hasattr(x,"foobar") */
#define DEE_XASTKIND_ATTR_DEL_C                25 /*< del x.foobar */
#define DEE_XASTKIND_ATTR_SET_C                26 /*< x.foobar = y */
#define DEE_XASTKIND_MODULE                    27 /*< module x */
#define DEE_XASTKIND_DEL_VAR                   28 /*< del x; */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_XASTKIND_FOREIGN_FUNCTION          29 /*< none(int,long,float) */
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DEE_XASTKIND_SWITCH                    30 /*< switch(x) {10: 42,"bar": "foo",default: 19} */
#define DEE_XASTKIND_IFTRUE                    31 /*< true ? a : b */
#define DEE_XASTKIND_IFFALSE                   32 /*< false ? a : b */

// Common operator expressions (unary)
#define DEE_XASTKIND_STR                     (100|DEE_XASTFLAG_UNARY) /*< str x */
#define DEE_XASTKIND_REPR                    (101|DEE_XASTFLAG_UNARY) /*< repr x */
#define DEE_XASTKIND_COPY                    (102|DEE_XASTFLAG_UNARY) /*< copy x */
#define DEE_XASTKIND_MOVE                    (103|DEE_XASTFLAG_UNARY) /*< move x */
#define DEE_XASTKIND_TYPE                    (104|DEE_XASTFLAG_UNARY) /*< type x */
#define DEE_XASTKIND_WEAK                    (105|DEE_XASTFLAG_UNARY) /*< weak x */
#define DEE_XASTKIND_BOOL                    (106|DEE_XASTFLAG_UNARY) /*< !!x */
#define DEE_XASTKIND_SUPEROF                 (107|DEE_XASTFLAG_UNARY) /*< x.super */
#define DEE_XASTKIND_CLASSOF                 (108|DEE_XASTFLAG_UNARY) /*< x.class */
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_XASTKIND_REF                     (109|DEE_XASTFLAG_UNARY) /*< &x */
#define DEE_XASTKIND_DEREF                   (110|DEE_XASTFLAG_UNARY) /*< *x */
#define DEE_XASTKIND_PTROF                   (111|DEE_XASTFLAG_UNARY) /*< x * */
#define DEE_XASTKIND_LVALOF                  (112|DEE_XASTFLAG_UNARY) /*< x & */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#define DEE_XASTKIND_NOT                     (113|DEE_XASTFLAG_UNARY) /*< !x */
#define DEE_XASTKIND_NEG                     (114|DEE_XASTFLAG_UNARY) /*< -x */
#define DEE_XASTKIND_POS                     (115|DEE_XASTFLAG_UNARY) /*< +x */
#define DEE_XASTKIND_INV                     (116|DEE_XASTFLAG_UNARY) /*< ~x */
#define DEE_XASTKIND_INC                     (117|DEE_XASTFLAG_UNARY) /*< ++x */
#define DEE_XASTKIND_DEC                     (118|DEE_XASTFLAG_UNARY) /*< --x */
#define DEE_XASTKIND_INCPOST                 (119|DEE_XASTFLAG_UNARY) /*< x++ */
#define DEE_XASTKIND_DECPOST                 (120|DEE_XASTFLAG_UNARY) /*< x-- */
#define DEE_XASTKIND_EXPAND                  (121|DEE_XASTFLAG_UNARY) /*< x... */
#define DEE_XASTKIND_SEQ_SIZE                (122|DEE_XASTFLAG_UNARY) /*< #x */
#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
#define DEE_XASTKIND_NEW                     (123|DEE_XASTFLAG_UNARY) /*< new x */
#define DEE_XASTKIND_DELETE                  (124|DEE_XASTFLAG_UNARY) /*< delete x */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE */
#define DEE_XASTKIND_ITERSELF                (125|DEE_XASTFLAG_UNARY) /*< x.operator __iterself__() */
#define DEE_XASTKIND_ITERNEXT                (126|DEE_XASTFLAG_UNARY) /*< x.operator __iternext__() */
#define DEE_XASTKIND_CELL                    (127|DEE_XASTFLAG_UNARY) /*< <x> */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#define DEE_XASTKIND_VARRAYOF                (128|DEE_XASTFLAG_UNARY) /*< x[] */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */

// Common operator expressions (binary)
// TODO: Binary operators should have a special extension for expand expressions:
// >> a = [10,20,30];
// >> b = [false,false,false];
// >> c = [false,true,false];
// >> d = [true,true,true];
// >> print 0 + a...;      // 60    -- __builtin_sum
// >> print true && b...;  // false -- __builtin_all
// >> print true && c...;  // false -- __builtin_all
// >> print true && d...;  // true  -- __builtin_all
// >> print false || b...; // false -- __builtin_any
// >> print false || c...; // true  -- __builtin_any
// >> print false || d...; // true  -- __builtin_any
// Basically, '0 + x...' expands to something similar to this '0 + x[0] + x[1] + ... + x[n-1]',
// where '+' can be replaced with any binary operator.
// (Though obviously it would iterators instead of index-based lookup)
#define DEE_XASTKIND_CALL                    (300|DEE_XASTFLAG_BINARY) /*< x(a,b,c) */
#define DEE_XASTKIND_COMPARE_LO              (301|DEE_XASTFLAG_BINARY) /*< x < y */
#define DEE_XASTKIND_COMPARE_LE              (302|DEE_XASTFLAG_BINARY) /*< x <= y */
#define DEE_XASTKIND_COMPARE_EQ              (303|DEE_XASTFLAG_BINARY) /*< x == y */
#define DEE_XASTKIND_COMPARE_NE              (304|DEE_XASTFLAG_BINARY) /*< x != y */
#define DEE_XASTKIND_COMPARE_GR              (305|DEE_XASTFLAG_BINARY) /*< x > y */
#define DEE_XASTKIND_COMPARE_GE              (306|DEE_XASTFLAG_BINARY) /*< x >= y */
#define DEE_XASTKIND_ADD                     (307|DEE_XASTFLAG_BINARY) /*< x + y */
#define DEE_XASTKIND_SUB                     (308|DEE_XASTFLAG_BINARY) /*< x - y */
#define DEE_XASTKIND_MUL                     (309|DEE_XASTFLAG_BINARY) /*< x * y */
#define DEE_XASTKIND_DIV                     (310|DEE_XASTFLAG_BINARY) /*< x / y */
#define DEE_XASTKIND_MOD                     (311|DEE_XASTFLAG_BINARY) /*< x % y */
#define DEE_XASTKIND_SHL                     (312|DEE_XASTFLAG_BINARY) /*< x << y */
#define DEE_XASTKIND_SHR                     (313|DEE_XASTFLAG_BINARY) /*< x >> y */
#define DEE_XASTKIND_AND                     (314|DEE_XASTFLAG_BINARY) /*< x & y */
#define DEE_XASTKIND_OR                      (315|DEE_XASTFLAG_BINARY) /*< x | y */
#define DEE_XASTKIND_XOR                     (316|DEE_XASTFLAG_BINARY) /*< x ^ y */
#define DEE_XASTKIND_POW                     (317|DEE_XASTFLAG_BINARY) /*< x ** y */
#define DEE_XASTKIND_IADD                    (318|DEE_XASTFLAG_BINARY) /*< x += y */
#define DEE_XASTKIND_ISUB                    (319|DEE_XASTFLAG_BINARY) /*< x -= y */
#define DEE_XASTKIND_IMUL                    (320|DEE_XASTFLAG_BINARY) /*< x *= y */
#define DEE_XASTKIND_IDIV                    (321|DEE_XASTFLAG_BINARY) /*< x /= y */
#define DEE_XASTKIND_IMOD                    (322|DEE_XASTFLAG_BINARY) /*< x %= y */
#define DEE_XASTKIND_ISHL                    (323|DEE_XASTFLAG_BINARY) /*< x <<= y */
#define DEE_XASTKIND_ISHR                    (324|DEE_XASTFLAG_BINARY) /*< x >>= y */
#define DEE_XASTKIND_IAND                    (325|DEE_XASTFLAG_BINARY) /*< x &= y */
#define DEE_XASTKIND_IOR                     (326|DEE_XASTFLAG_BINARY) /*< x |= y */
#define DEE_XASTKIND_IXOR                    (327|DEE_XASTFLAG_BINARY) /*< x ^= y */
#define DEE_XASTKIND_IPOW                    (328|DEE_XASTFLAG_BINARY) /*< x **= y */
#define DEE_XASTKIND_SEQ_GET                 (329|DEE_XASTFLAG_BINARY) /*< x[y] */
#define DEE_XASTKIND_SEQ_DEL                 (330|DEE_XASTFLAG_BINARY) /*< del x[y] */
#define DEE_XASTKIND_ATTR_GET                (331|DEE_XASTFLAG_BINARY) /*< x.operator . (y) */
#define DEE_XASTKIND_ATTR_HAS                (332|DEE_XASTFLAG_BINARY) /*< __builtin_hasattr(x,y) */
#define DEE_XASTKIND_ATTR_DEL                (333|DEE_XASTFLAG_BINARY) /*< x.operator del. (y) */
#define DEE_XASTKIND_IS                      (334|DEE_XASTFLAG_BINARY) /*< x is y */
#define DEE_XASTKIND_IN                      (335|DEE_XASTFLAG_BINARY) /*< x in y */
#define DEE_XASTKIND_NOT_IS                  (336|DEE_XASTFLAG_BINARY) /*< x !is y */
#define DEE_XASTKIND_NOT_IN                  (337|DEE_XASTFLAG_BINARY) /*< x !in y */
#define DEE_XASTKIND_LAND                    (338|DEE_XASTFLAG_BINARY) /*< x && y */
#define DEE_XASTKIND_LOR                     (339|DEE_XASTFLAG_BINARY) /*< x || y */
#define DEE_XASTKIND_LXOR                    (340|DEE_XASTFLAG_BINARY) /*< x ^^ y */
#define DEE_XASTKIND_MOVE_ASSIGN             (341|DEE_XASTFLAG_BINARY) /*< x.operator move := (y) */
#define DEE_XASTKIND_COPY_ASSIGN             (342|DEE_XASTFLAG_BINARY) /*< x := y */

// Common operator expressions (trinary)
#define DEE_XASTKIND_SEQ_SET                 (500|DEE_XASTFLAG_TRINARY) /*< x[y] = z */
#define DEE_XASTKIND_ATTR_SET                (501|DEE_XASTFLAG_TRINARY) /*< x.operator .= (y,z) */

// File operator expressions
#define DEE_XASTKIND_IO_READNP               (600|DEE_XASTFLAG_BINARY)  /*< x.operator __read__(y) */
#define DEE_XASTKIND_IO_WRITENP              (601|DEE_XASTFLAG_BINARY)  /*< x.operator __write__(y) */
#define DEE_XASTKIND_IO_SEEK                 (602|DEE_XASTFLAG_TRINARY) /*< x.operator __seek__(y,z) */
#define DEE_XASTKIND_IO_FLUSH                (603|DEE_XASTFLAG_UNARY)   /*< x.operator __flush__() */
#define DEE_XASTKIND_IO_TRUNC                (604|DEE_XASTFLAG_UNARY)   /*< x.operator __trunc__() */
#define DEE_XASTKIND_IO_CLOSE                (605|DEE_XASTFLAG_UNARY)   /*< x.operator __close__() */
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_XASTKIND_IO_READP                (606|DEE_XASTFLAG_TRINARY) /*< x.operator __read__(y,z) */
#define DEE_XASTKIND_IO_WRITEP               (607|DEE_XASTFLAG_TRINARY) /*< x.operator __write__(y,z) */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

#define DEE_XASTKIND_VAR_INC                  800 /*< ++x */
#define DEE_XASTKIND_VAR_DEC                  801 /*< --x */
#define DEE_XASTKIND_VAR_INCPOST              802 /*< x++ */
#define DEE_XASTKIND_VAR_DECPOST              803 /*< x-- */
#define DEE_XASTKIND_VAR_IADD                 804 /*< x += y */
#define DEE_XASTKIND_VAR_ISUB                 805 /*< x -= y */
#define DEE_XASTKIND_VAR_IMUL                 806 /*< x *= y */
#define DEE_XASTKIND_VAR_IDIV                 807 /*< x /= y */
#define DEE_XASTKIND_VAR_IMOD                 808 /*< x %= y */
#define DEE_XASTKIND_VAR_ISHL                 809 /*< x <<= y */
#define DEE_XASTKIND_VAR_ISHR                 810 /*< x >>= y */
#define DEE_XASTKIND_VAR_IAND                 811 /*< x &= y */
#define DEE_XASTKIND_VAR_IOR                  812 /*< x |= y */
#define DEE_XASTKIND_VAR_IXOR                 813 /*< x ^= y */
#define DEE_XASTKIND_VAR_IPOW                 814 /*< x **= y */
#define DEE_XASTKIND_ISUNARYVAR(x)    ((x)>=800&&(x)<=803)
#define DEE_XASTKIND_ISBINARYVAR(x)   ((x)>=804&&(x)<=814)
#define DEE_XASTKIND_CASE_INPLACE_VAR_UNARY \
 case DEE_XASTKIND_VAR_INC: case DEE_XASTKIND_VAR_INCPOST:\
 case DEE_XASTKIND_VAR_DEC: case DEE_XASTKIND_VAR_DECPOST:
#define DEE_XASTKIND_CASE_INPLACE_VAR_BINARY \
 case DEE_XASTKIND_VAR_IADD: case DEE_XASTKIND_VAR_ISUB:\
 case DEE_XASTKIND_VAR_IMUL: case DEE_XASTKIND_VAR_IDIV:\
 case DEE_XASTKIND_VAR_IMOD: case DEE_XASTKIND_VAR_ISHL:\
 case DEE_XASTKIND_VAR_ISHR: case DEE_XASTKIND_VAR_IAND:\
 case DEE_XASTKIND_VAR_IOR: case DEE_XASTKIND_VAR_IXOR:\
 case DEE_XASTKIND_VAR_IPOW:


// Builtin functions mapped to operators
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
#define DEE_XASTKIND_BUILTIN_ALLOCA         (1000|DEE_XASTFLAG_UNARY)  /*< __builtin_alloca(42)      - Allocate memory on the stack. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
#define DEE_XASTKIND_BUILTIN_HELP           (1001|DEE_XASTFLAG_UNARY)  /*< __builtin_help(foo.bar)   - Return a help string for a given expression. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
#define DEE_XASTKIND_BUILTIN_BREAKPOINT     (1002|DEE_XASTFLAG_ZEROOP) /*< __builtin_breakpoint()    - Enter interactive debugger. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
#define DEE_XASTKIND_BUILTIN_UNREACHABLE    (1003|DEE_XASTFLAG_ZEROOP) /*< __builtin_unreachable()   - Unreachable/Noreturn expression. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define DEE_XASTKIND_BUILTIN_BOUND           1004                      /*< __builtin_bound(x)        - Check if a given variable is bound. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
#define DEE_XASTKIND_BUILTIN_DEX            (1006|DEE_XASTFLAG_UNARY)  /*< __builtin_dex("foo")      - Load an extension module at runtime (same as '__dex foo'). */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define DEE_XASTKIND_BUILTIN_EXTERN          1007                      /*< __builtin_extern(...)     - Load an external object. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_XASTKIND_BUILTIN_FF_CLOSURE     (1008|DEE_XASTFLAG_BINARY) /*< __builtin_ff_closure(...) - Create a foreign-function closure. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */
#define DEE_XASTKIND_SUPER_AT               (1020|DEE_XASTFLAG_BINARY) /*< __super(T,x)              - Super-data for 'T' of 'x' or 'this'. */
#define DEE_XASTKIND_BUILTIN_UNUSED         (1021|DEE_XASTFLAG_BINARY) /*< __builtin_unused(x,y)     - Evaluate 'x', but always return 'y'. */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#define DEE_XASTKIND_BUILTIN_EXPECT          1022                      /*< __builtin_expect(x,true)  - Hint about the result of a branch condition (ignored elsewhere). */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
#define DEE_XASTKIND_SEQ_ANY                (1023|DEE_XASTFLAG_UNARY)  /*< seq||...                  - Returns true if any element of 'seq' is true. */
#define DEE_XASTKIND_SEQ_ALL                (1024|DEE_XASTFLAG_UNARY)  /*< seq&&...                  - Returns true if all elements of 'seq' are true. */
#define DEE_XASTKIND_SEQ_SUM                (1025|DEE_XASTFLAG_UNARY)  /*< seq+...                   - Returns the sum of all elements. */

typedef Dee_uint32_t DeeXAstKind;
typedef Dee_uint32_t DeeSAstKind;



struct DeeXAstFunctionAstArgument {
 DEE_A_REF DeeTokenObject    *a_token;   /*< [1..1] Argument token (used in Error/Debug output) */
 DEE_A_REF DeeLocalVarObject *a_var;     /*< [1..1] Argument variable (in function scope) */
 DEE_A_REF DeeXAstObject     *a_default; /*< [0..1] Default value of the variable. */
};
#define _DeeXAstFunctionAstArgument_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->a_token = (right)->a_token);\
 Dee_INCREF((ob)->a_var = (right)->a_var);\
 Dee_XINCREF((ob)->a_default = (right)->a_default);\
}while(0)

#define _DeeXAstFunctionAstArgument_Quit(ob)\
do{\
 Dee_DECREF((ob)->a_token);\
 Dee_DECREF((ob)->a_var);\
 Dee_XDECREF((ob)->a_default);\
}while(0)
#define _DeeXAstFunctionAstArgument_Visit(ob)\
do{\
 Dee_VISIT((ob)->a_token);\
 Dee_VISIT((ob)->a_var);\
 Dee_XVISIT((ob)->a_default);\
}while(0)

struct DeeXAstDictEntry {
 DEE_A_REF DeeXAstObject *de_key;  /*< [1..1] Expression used for the dict key. */
 DEE_A_REF DeeXAstObject *de_item; /*< [1..1] Expression used for the dict item. */
};
#define _DeeXAstDictEntry_Quit(ob)\
do{\
 Dee_DECREF((ob)->de_key);\
 Dee_DECREF((ob)->de_item);\
}while(0)
#define _DeeXAstDictEntry_Visit(ob)\
do{\
 Dee_VISIT((ob)->de_key);\
 Dee_VISIT((ob)->de_item);\
}while(0)

struct DeeXAstSwitchEntry {
 DEE_A_REF DeeXAstObject *se_key;    /*< [1..1] The switch key. */
 DEE_A_REF DeeXAstObject *se_value;  /*< [1..1] The value that the key will map to. */
 Dee_size_t               se_jmparg; /*< Used during compilation to store the address of the jmp-to-end argument */
};
#define _DeeXAstSwitchEntry_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->se_key = (right)->se_key);\
 Dee_INCREF((ob)->se_value = (right)->se_value);\
}while(0)

#define _DeeXAstSwitchEntry_Quit(ob)\
do{\
 Dee_DECREF((ob)->se_key);\
 Dee_DECREF((ob)->se_value);\
}while(0)




#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeXAstClassAstEntry {
#define DEE_XAST_CLASSENTRY_KIND_NONE         DEE_UINT32_C(0)
#define DEE_XAST_CLASSENTRY_KIND_SLOT         DEE_UINT32_C(1)
#define DEE_XAST_CLASSENTRY_KIND_METHOD       DEE_UINT32_C(2)
#define DEE_XAST_CLASSENTRY_KIND_GETSET       DEE_UINT32_C(3)
#define DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD DEE_UINT32_C(4)
#define DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET DEE_UINT32_C(5)
 Dee_uint32_t ce_kind;
 union{
  struct {
   int                        cs_slotid;   /*< Type Slot ID. */
   DEE_A_REF DeeXAstObject   *cs_callback; /*< [1..1] Callback set for this slot. */
  } ce_slot;
  struct {
   DEE_A_REF DeeStringObject *cm_name;     /*< [1..1] Name of this method. */
   DEE_A_REF DeeXAstObject   *cm_callback; /*< [1..1] Callback set for this method. */
  } ce_method;
  struct {
   DEE_A_REF DeeStringObject *cg_name; /*< [1..1] Name of this method. */
   DEE_A_REF DeeXAstObject   *cg_get;  /*< [0..1] Getter value. */
   DEE_A_REF DeeXAstObject   *cg_del;  /*< [0..1] Delete value. */
   DEE_A_REF DeeXAstObject   *cg_set;  /*< [0..1] Setter value. */
  } ce_getset;
 };
};
DEE_COMPILER_MSVC_WARNING_POP


// Returns true if 'a' and 'b' describe the same slot/member
// Note, that getsets and methods share the same namespace here!
extern DEE_A_RET_WUNUSED int DeeXAstClassAstEntry_SameDeclaration(
 DEE_A_IN struct DeeXAstClassAstEntry const *a,
 DEE_A_IN struct DeeXAstClassAstEntry const *b);

// Returns '1' if the entry was initialized as a reference-copy
extern DEE_A_RET_EXCEPT(-1) int DeeXAstClassAstEntry_InitCopyWithScope(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN struct DeeXAstClassAstEntry const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT DeeLexerObject *lexer);

extern DEE_A_RET_EXCEPT(-1) int DeeXAstClassAstEntry_CollectTopScopeChildren(
 DEE_A_IN struct DeeXAstClassAstEntry const *self, DEE_A_INOUT struct DeeScopeList *scopes);

#define _DeeXAstClassAstEntry_InitCopy(ob,right,...)\
do{\
 switch (((ob)->ce_kind=(right)->ce_kind)) {\
  case DEE_XAST_CLASSENTRY_KIND_SLOT:\
   (ob)->ce_slot.cs_slotid=(right)->ce_slot.cs_slotid;\
   Dee_INCREF((ob)->ce_slot.cs_callback=(right)->ce_slot.cs_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_METHOD:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:\
   Dee_INCREF((ob)->ce_method.cm_name=(right)->ce_method.cm_name);\
   Dee_INCREF((ob)->ce_method.cm_callback=(right)->ce_method.cm_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_GETSET:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:\
   Dee_INCREF((ob)->ce_getset.cg_name=(right)->ce_getset.cg_name);\
   Dee_XINCREF((ob)->ce_getset.cg_get=(right)->ce_getset.cg_get);\
   Dee_XINCREF((ob)->ce_getset.cg_del=(right)->ce_getset.cg_del);\
   Dee_XINCREF((ob)->ce_getset.cg_set=(right)->ce_getset.cg_set);\
   break;\
  default: break;\
 }\
}while(0)
#define _DeeXAstClassAstEntry_Quit(ob)\
do{\
 switch ((ob)->ce_kind) {\
  case DEE_XAST_CLASSENTRY_KIND_SLOT:\
   Dee_DECREF((ob)->ce_slot.cs_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_METHOD:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:\
   Dee_DECREF((ob)->ce_method.cm_name);\
   Dee_DECREF((ob)->ce_method.cm_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_GETSET:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:\
   Dee_DECREF((ob)->ce_getset.cg_name);\
   Dee_XDECREF((ob)->ce_getset.cg_get);\
   Dee_XDECREF((ob)->ce_getset.cg_del);\
   Dee_XDECREF((ob)->ce_getset.cg_set);\
   break;\
  default: break;\
 }\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */




#define DEE_XAST_AST_HEAD \
                  DeeXAstKind     ast_kind;  /*< The kind of AST. */\
 DEE_A_REF struct DeeTokenObject *ast_token; /*< [1..1] token of this AST (used in debug/error output) */

struct DeeXAstCommonAst { DEE_XAST_AST_HEAD };

#ifdef __INTELLISENSE__
// So I don't have to scroll all the time...
#undef DEE_XAST_AST_HEAD
#define DEE_XAST_AST_HEAD  /* nothing */
#endif

struct DeeXAstConstAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeObject *c_const; /*< [1..1] Constant value of this expression. */
};

struct DeeXAstVarAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeLocalVarObject *vs_var;   /*< [1..1] Variable referenced (NOTE: This ast also owns +1 in 'ob_var->lv_uses'). */
#define DEE_XAST_VARAST_FLAG_NONE DEE_UINT32_C(0x00000000)
#define DEE_XAST_VARAST_FLAG_REF  DEE_UINT32_C(0x00000001) /*< Variable is only accessible through a reference. */
 Dee_uint32_t                 vs_flags; /*< Flags describing the access to this variable (Set of DEE_XAST_VARAST_FLAG_*). */
};

struct DeeXAstVarDeclAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeTokenObject    *vd_token; /*< [1..1] Token of the variable (Used for error/debug output). */
 DEE_A_REF DeeLocalVarObject *vd_var;   /*< [1..1] Variable being declared (NOTE: This ast also owns +1 in 'ob_var->ob_init'). */
 DEE_A_REF DeeXAstObject     *vd_init;  /*< [1..1] Initialization branch. */
};

struct DeeXAstMultiVarDeclAst {
 // NOTE: This is the AST for 'a,b,c = foo()...;'
 //       'a', 'b' and 'c' are stored in 'mvd_varv'
 //       and 'foo()' is stored in 'mvd_init'.
 DEE_XAST_AST_HEAD
 DeeXAstKind               mvd_type; /*< The kind of sequence generated by this AST (Must be compatible with 'struct DeeXAstSequenceAst'). */
 Dee_size_t                mvd_varc; /*< Amount of variables being declared. */
 struct DeeVarDeclStorage *mvd_varv; /*< [1..ob_varc][owned] Vector of variables being declared. */
 DEE_A_REF DeeXAstObject  *mvd_init; /*< [1..1] Initialization branch (NOTE: Not an expand expression, but 'ast_operator.ob_a' of the expand expression instead). */
};

struct DeeXAstStatementAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeSAstObject *s_stmt;  /*< [1..1] Statement ast. */
};
struct DeeXAstIfAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *if_cond;   /*< [1..1] Condition value (never a 'DEE_XASTKIND_BOOL'). */
 DEE_A_REF DeeXAstObject *if_succ;   /*< [0..1] Result ast on success (or NULL if not given). */
 DEE_A_REF DeeXAstObject *if_fail;   /*< [0..1] Result ast on failure (or NULL if not given). */
};
struct DeeXAstFunctionAst {
 DEE_XAST_AST_HEAD
 DeeStringObject                   *f_name;  /*< [0..1] Name of the function (either through attribute, or declaration). */
 Dee_uint32_t                       f_flags; /*< Function flags (same as the final 'DeeFunctionObject.ob_flags') */
 Dee_size_t                         f_argc;  /*< Amount of argument that the function takes. */
 struct DeeXAstFunctionAstArgument *f_argv;  /*< [0..ob_argc][owned] Amount of argument that the function takes. */
 DEE_A_REF DeeSAstObject           *f_code;  /*< [0..1] Function code (NULL if none-function). */
 DEE_A_REF DeeScopeObject          *f_scope; /*< [0..1] Scope of the function (NULL if none-function). */
};
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
struct DeeXAstClassAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF struct DeeStringObject *c_name;    /*< [0..1] Name of the class. */
 DEE_A_REF DeeXAstObject          *c_base;    /*< [0..1] Base of the class. */
 Dee_size_t                        c_entryc;  /*< Actual amount of entries. */
 struct DeeXAstClassAstEntry      *c_entryv;  /*< [0..ob_entryc][owned] Vector of class decl entires. */
#define DEE_XAST_CLASSAST_FLAG_HAS_UUID   DEE_TYPE_FLAG_REGISTERED_MARSHAL
 Dee_uint32_t                      c_tpflags; /*< Additional type flags that will be or'ed onto the class. */
 struct DeeUUID                    c_uuid;    /*< The uuid associated with this class. */
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
struct DeeXAstSequenceAst {
 DEE_XAST_AST_HEAD
 Dee_size_t                s_elemc; /*< Amount of sequence elements. */
 DEE_A_REF DeeXAstObject **s_elemv; /*< [1..1][1..s_elemc][owned] List of sequence elements. */
};
struct DeeXAstDictAst {
 DEE_XAST_AST_HEAD
 Dee_size_t               d_elemc; /*< Amount of dict elements. */
 struct DeeXAstDictEntry *d_elemv; /*< [1..d_elemc][owned] List of dict elements. */
};
struct DeeXAstSeqRangeAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *sr_seq;   /*< [1..1] Sequence of query a range on. */
 DEE_A_REF DeeXAstObject *sr_begin; /*< [0..1] Lower bound (or NULL if not given or 'none'). */
 DEE_A_REF DeeXAstObject *sr_end;   /*< [0..1] Upper bound (or NULL if not given or 'none'). */
 DEE_A_REF DeeXAstObject *sr_value; /*< [1..1][if(ast_kind == DEE_XASTKIND_SEQ_RANGE_SET)] Value to assign. */
};
struct DeeXAstAttrCAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject   *ac_object; /*< [1..1] Object to query an attribute on. */
 DEE_A_REF DeeStringObject *ac_name;   /*< [1..1] Name of the attribute/member. */
 DEE_A_REF DeeXAstObject   *ac_value;  /*< [1..1][if(ast_kind == DEE_XASTKIND_ATTR_SET_C)] Value to assign. */
};
struct DeeXAstModuleAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeScopeObject *m_module; /*< [1..1] Referenced module. */
};
struct DeeXAstDelVarAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeTokenObject    *d_token; /*< [1..1] The variable name token. */
 DEE_A_REF DeeLocalVarObject *d_var;   /*< [1..1] Variable that should supposed to be deleted. */
};
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#ifdef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#undef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#endif
struct DeeXAstForeignFunctionAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject  *ff_base;  /*< [1..1] Base/Return type of the foreign function. */
 DeeFunctionFlags          ff_flags; /*< Foreign function flags. */
 Dee_size_t                ff_argc;  /*< Argument count. */
 DEE_A_REF DeeXAstObject **ff_argv;  /*< [1..1][owned][0..ff_argc][owned] Vector of argument types. */
};
#endif
struct DeeXAstSwitchAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject   *s_expr;    /*< [1..1] The switch expression. */
 Dee_size_t                 s_entryc;  /*< The amount of switch entries. */
 struct DeeXAstSwitchEntry *s_entryv;  /*< [1..s_entryc][owned] The vector of switch entries. */
 DEE_A_REF DeeXAstObject   *s_default; /*< [0..1] The default switch value (or NULL if no default is given) */
};
struct DeeXAstIfConstAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *ic_succ; /*< [0..1] The success branch. */
 DEE_A_REF DeeXAstObject *ic_fail; /*< [0..1] The fail branch. */
};

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
struct DeeXAstBuiltinBoundAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeTokenObject    *b_token; /*< [1..1] Token of the variable. */
 DEE_A_REF DeeLocalVarObject *b_var;   /*< [1..1] Variable that should be checked for binding. */
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
struct DeeXAstBuiltinExternAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *ex_file; /*< [0..1] Name of the shared library (or NULL for automatic linkage). */
 DEE_A_REF DeeXAstObject *ex_name; /*< [1..1] Name of the shared object. */
 DEE_A_REF DeeXAstObject *ex_type; /*< [1..1] Type of the shared object (raw type; to load a 'int &', simply set this to 'int'). */
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
struct DeeXAstBuiltinExpectAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *e_value;  /*< [1..1] AST whose value is being expected. */
 DEE_A_REF DeeObject     *e_expect; /*< [1..1] The expected value. */
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
struct DeeXAstOperatorAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeXAstObject *op_a; /*< [1..1][if(DEE_XASTKIND_OPCOUNT(ast_kind) >= 1)] First operand. */
 DEE_A_REF DeeXAstObject *op_b; /*< [1..1][if(DEE_XASTKIND_OPCOUNT(ast_kind) >= 2)] Second operand. */
 DEE_A_REF DeeXAstObject *op_c; /*< [1..1][if(DEE_XASTKIND_OPCOUNT(ast_kind) >= 3)] Third operand. */
};
struct DeeXAstUnaryInplaceVarAst {
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeLocalVarObject *uiv_var; /*< [1..1] Scope-local variable to operate on (NOTE: This ast also owns +1 in 'uiv_var->lv_uses' and 'uiv_var->lv_init'). */
 DEE_A_REF DeeTokenObject    *uiv_tok; /*< [1..1] Token for the variable name. */
};
struct DeeXAstBinaryInplaceVarAst {
 // NOTE: This structure must have binary compatibility
 //       with 'DeeXAstUnaryInplaceVarAst'.
 DEE_XAST_AST_HEAD
 DEE_A_REF DeeLocalVarObject *biv_var; /*< [1..1] Scope-local variable to operate on (NOTE: This ast also owns +1 in 'biv_var->lv_uses' and 'biv_var->lv_init'). */
 DEE_A_REF DeeTokenObject    *biv_tok; /*< [1..1] Token for the variable name. */
 DEE_A_REF DeeXAstObject     *biv_arg; /*< [1..1] Right-hand-side argument ast. */
};
#undef DEE_XAST_AST_HEAD


#define DEE_POINTER_LIST_INIT(T)        {{0,0,NULL}}
#define _DEE_POINTER_LIST_Init(T,ob)    ((ob)->pl_data.pl_c=0,(ob)->pl_data.pl_a=0,(ob)->pl_data.pl_a=NULL)
#define _DEE_POINTER_LIST_Quit(T,ob)    free((ob)->pl_data.pl_v)
#define DEE_POINTER_LIST_ELEM(T,ob)     ((T *)(ob)->pl_data.pl_v)
#define DEE_POINTER_LIST_SIZE(T,ob)     ((ob)->pl_data.pl_c)
#define DEE_POINTER_LIST_ALLOC(T,ob)    ((ob)->pl_data.pl_a)
#ifdef __INTELLISENSE__
#define DEE_POINTER_LIST_Append(T,ob,p) _DeePointerList_AppendImpl((struct _DeePointerList *)(&(ob)->pl_data),(void *)(p))
#else
#define DEE_POINTER_LIST_Append(T,ob,p) _DeePointerList_AppendImpl((struct _DeePointerList *)(ob),(void *)(p))
#endif
#define DEE_POINTER_LIST(T)\
{ struct {\
 Dee_size_t pl_c; /*< Actual vector size. */\
 Dee_size_t pl_a; /*< Allocated vector size. */\
 T         *pl_v; /*< [0..pl_c] Vector of pointers. */\
} pl_data; }
struct _DeePointerList DEE_POINTER_LIST(void *);
extern DEE_A_RET_EXCEPT(-1) int _DeePointerList_AppendImpl(
 DEE_A_INOUT struct _DeePointerList *self, DEE_A_IN void *elem);


struct DeeScopeList DEE_POINTER_LIST(DeeScopeObject *);
#define DeeScopeList_INIT()           DEE_POINTER_LIST_INIT(DeeScopeObject *)
#define _DeeScopeList_Init(ob)        _DEE_POINTER_LIST_Init(DeeScopeObject *,ob)
#define _DeeScopeList_Quit(ob)        _DEE_POINTER_LIST_Quit(DeeScopeObject *,ob)
#define DeeScopeList_ELEM(ob)         DEE_POINTER_LIST_ELEM(DeeScopeObject *,ob)
#define DeeScopeList_SIZE(ob)         DEE_POINTER_LIST_SIZE(DeeScopeObject *,ob)
#define DeeScopeList_Append(ob,scope) DEE_POINTER_LIST_Append(DeeScopeObject *,ob,scope)

struct DeeXAstList DEE_POINTER_LIST(DeeXAstObject *);
#define DeeXAstList_INIT()           DEE_POINTER_LIST_INIT(DeeXAstObject *)
#define _DeeXAstList_Init(ob)        _DEE_POINTER_LIST_Init(DeeXAstObject *,ob)
#define _DeeXAstList_Quit(ob)        _DEE_POINTER_LIST_Quit(DeeXAstObject *,ob)
#define DeeXAstList_ELEM(ob)         DEE_POINTER_LIST_ELEM(DeeXAstObject *,ob)
#define DeeXAstList_SIZE(ob)         DEE_POINTER_LIST_SIZE(DeeXAstObject *,ob)
#define DeeXAstList_Append(ob,scope) DEE_POINTER_LIST_Append(DeeXAstObject *,ob,scope)

struct DeeSAstList DEE_POINTER_LIST(DeeSAstObject *);
#define DeeSAstList_INIT()           DEE_POINTER_LIST_INIT(DeeSAstObject *)
#define _DeeSAstList_Init(ob)        _DEE_POINTER_LIST_Init(DeeSAstObject *,ob)
#define _DeeSAstList_Quit(ob)        _DEE_POINTER_LIST_Quit(DeeSAstObject *,ob)
#define DeeSAstList_ELEM(ob)         DEE_POINTER_LIST_ELEM(DeeSAstObject *,ob)
#define DeeSAstList_SIZE(ob)         DEE_POINTER_LIST_SIZE(DeeSAstObject *,ob)
#define DeeSAstList_Append(ob,scope) DEE_POINTER_LIST_Append(DeeSAstObject *,ob,scope)


// Expression AST (v102+)
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeXAstObject {
 DEE_OBJECT_HEAD
#define DeeXAst_UNCOMMON_SIZE \
 (sizeof(DeeXAstObject)-(sizeof(DeeObject)+sizeof(struct DeeXAstCommonAst)))
#define DeeXAst_UNCOMMON_DATA(ob)\
 (void *)(((uintptr_t)(ob))+(sizeof(DeeObject)+sizeof(struct DeeXAstCommonAst)))

 union{
  DeeXAstKind                       ast_kind;            /*< The kind of AST. */
  struct DeeXAstCommonAst           ast_common;          /*< DEE_XASTKIND_*. */
  struct DeeXAstConstAst            ast_const;           /*< DEE_XASTKIND_CONST. */
  struct DeeXAstVarAst              ast_var;             /*< DEE_XASTKIND_VAR. */
  struct DeeXAstVarDeclAst          ast_vardecl;         /*< DEE_XASTKIND_VARDECL. */
  struct DeeXAstMultiVarDeclAst     ast_multivardecl;    /*< DEE_XASTKIND_MULTIVARDECL. */
  struct DeeXAstStatementAst        ast_statement;       /*< DEE_XASTKIND_STATEMENT. */
  struct DeeXAstIfAst               ast_if;              /*< DEE_XASTKIND_IF. */
  struct DeeXAstFunctionAst         ast_function;        /*< DEE_XASTKIND_FUNCTION. */
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  struct DeeXAstClassAst            ast_class;           /*< DEE_XASTKIND_CLASS. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  struct DeeXAstSequenceAst         ast_sequence;        /*< DEE_XASTKIND_TUPLE, DEE_XASTKIND_LIST, DEE_XASTKIND_SET. */
  struct DeeXAstSequenceAst         ast_tuple;           /*< DEE_XASTKIND_TUPLE. */
  struct DeeXAstSequenceAst         ast_list;            /*< DEE_XASTKIND_LIST. */
  struct DeeXAstSequenceAst         ast_set;             /*< DEE_XASTKIND_SET. */
  struct DeeXAstDictAst             ast_dict;            /*< DEE_XASTKIND_DICT. */
  struct DeeXAstSeqRangeAst         ast_seq_range;       /*< DEE_XASTKIND_SEQ_RANGE_GET, DEE_XASTKIND_SEQ_RANGE_DEL, DEE_XASTKIND_SEQ_RANGE_SET. */
  struct DeeXAstSeqRangeAst         ast_seq_range_get;   /*< DEE_XASTKIND_SEQ_RANGE_GET. */
  struct DeeXAstSeqRangeAst         ast_seq_range_del;   /*< DEE_XASTKIND_SEQ_RANGE_DEL. */
  struct DeeXAstSeqRangeAst         ast_seq_range_set;   /*< DEE_XASTKIND_SEQ_RANGE_SET. */
  struct DeeXAstAttrCAst            ast_attr_c;          /*< DEE_XASTKIND_ATTR_GET_C, DEE_XASTKIND_ATTR_HAS_C, DEE_XASTKIND_ATTR_DEL_C, DEE_XASTKIND_ATTR_SET_C. */
  struct DeeXAstAttrCAst            ast_attr_get_c;      /*< DEE_XASTKIND_ATTR_GET_C. */
  struct DeeXAstAttrCAst            ast_attr_has_c;      /*< DEE_XASTKIND_ATTR_HAS_C. */
  struct DeeXAstAttrCAst            ast_attr_del_c;      /*< DEE_XASTKIND_ATTR_DEL_C. */
  struct DeeXAstAttrCAst            ast_attr_set_c;      /*< DEE_XASTKIND_ATTR_SET_C. */
  struct DeeXAstModuleAst           ast_module;          /*< DEE_XASTKIND_MODULE. */
  struct DeeXAstDelVarAst           ast_delvar;          /*< DEE_XASTKIND_DEL_VAR. */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  struct DeeXAstForeignFunctionAst  ast_foreignfunction; /*< DEE_XASTKIND_FOREIGN_FUNCTION. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
  struct DeeXAstSwitchAst           ast_switch;          /*< DEE_XASTKIND_SWITCH. */
  struct DeeXAstIfConstAst          ast_ifconst;         /*< DEE_XASTKIND_IFTRUE, DEE_XASTKIND_IFFALSE. */
  struct DeeXAstIfConstAst          ast_iftrue;          /*< DEE_XASTKIND_IFTRUE. */
  struct DeeXAstIfConstAst          ast_iffalse;         /*< DEE_XASTKIND_IFFALSE. */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  struct DeeXAstBuiltinBoundAst     ast_builtin_bound;   /*< DEE_XASTKIND_BUILTIN_BOUND. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  struct DeeXAstBuiltinExternAst    ast_builtin_extern;  /*< DEE_XASTKIND_BUILTIN_EXTERN. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  struct DeeXAstBuiltinExpectAst    ast_builtin_expect;  /*< DEE_XASTKIND_BUILTIN_EXPECT. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  struct DeeXAstCommonAst           ast_zeroop;          /*< DEE_XASTKIND_ISZEROOP(ast_kind). */
  struct DeeXAstOperatorAst         ast_operator;        /*< DEE_XASTKIND_ISOPERATOR(ast_kind). */
  struct DeeXAstUnaryInplaceVarAst  ast_unary_var;       /*< DEE_XASTKIND_ISUNARYVAR(ast_kind). */
  struct DeeXAstBinaryInplaceVarAst ast_binary_var;      /*< DEE_XASTKIND_ISBINARYVAR(ast_kind). */
 };
};
DEE_COMPILER_MSVC_WARNING_POP

extern DeeTypeObject DeeXAst_Type;
#define DeeXAst_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeXAst_Type)
#define DeeXAst_Check          DeeXAst_CheckExact


extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_ClearPool(void);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewUnsafe(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk);
#define _DeeXAst_FreeUnsafe(ob)\
do{\
 Dee_DECREF((ob)->ast_common.ast_token);\
 _DeeObject_DELETE(&DeeXAst_Type,(DeeObject *)(ob));\
}while(0)


//////////////////////////////////////////////////////////////////////////
// Collects the top scopes used by 'self'
// e.g.: 'true ? ({ ... }) : ({ ... })' Returns two scopes
//                ^ this one  ^ and this one
// e.g.: '({ if (x) { ... } })' Returns one scope
//         ^ this one, because only the first (aka. top) scopes are collected
// in "xast.collect_top_scope_children.inl"
extern int DeeXAst_CollectTopScopeChildren(
 DEE_A_IN DeeXAstObject const *self, DEE_A_INOUT struct DeeScopeList *scopes);

typedef DEE_A_RET_EXCEPT_FAIL(-1,1) int (DEE_CALL *DeeXAstVisitProc)(DEE_A_IN DeeXAstObject *ast, void *closure);
typedef DEE_A_RET_EXCEPT_FAIL(-1,1) int (DEE_CALL *DeeSAstVisitProc)(DEE_A_IN DeeSAstObject *ast, void *closure);

//////////////////////////////////////////////////////////////////////////
// Visit all reachable asts
// Stop visiting a branch if the callback returned '!= 0'
// in "xast.visit_all.inl"
extern int DeeXAst_VisitAll(
 DEE_A_IN DeeXAstObject *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure);
extern int DeeVarDeclStorage_VisitAll(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure);


//////////////////////////////////////////////////////////////////////////
// Warn about the use of an inplace operator 'kind'
// on a constant instance of 'lhs_operator_type'.
//  - Returns <0 if the error was considered terminal.
// in "xast.c"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_WarnInplaceOnConstant(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_INOUT DeeTokenObject *tok, DEE_A_IN DeeTypeObject const *lhs_operator_type)
 DEE_ATTRIBUTE_NONNULL((2,3,4));

//////////////////////////////////////////////////////////////////////////
// Copies any given AST into the context of a new scope
// in "xast.copy_with_scope.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_CopyWithScope(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeScopeObject *new_scope,
 DEE_A_INOUT DeeLexerObject *lexer);

//////////////////////////////////////////////////////////////////////////
// Returns true/false if 'self' is equal to 'right'
// in "xast.equals.inl"
// NOTE: Tokens will be ignored during comparison
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_Equals(
 DEE_A_IN DeeXAstObject const *self,
 DEE_A_IN DeeXAstObject const *right);

//////////////////////////////////////////////////////////////////////////
// Returns true/false if 'self' uses 'variable' in some way, shape or form
// in "xast.variable_usage.inl"
extern DEE_A_RET_WUNUSED int DeeXAst_UsesVariable(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED int DeeXAst_LoadsVariable(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED int DeeXAst_StoresVariable(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableUses(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableLoads(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableStores(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN DeeLocalVarObject const *variable);

//////////////////////////////////////////////////////////////////////////
// in "xast.assign.inl"
extern void DeeXAst_InitMove(DEE_A_OUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right);
extern void DeeXAst_AssignMove(DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_InitCopy(DEE_A_OUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_AssignCopy(DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right);
extern void DeeXAst_AssignConst(DEE_A_INOUT DeeXAstObject *self, DEE_A_IN DeeObject *const_value);
#define DeeXAst_AssignEmpty(ob) DeeXAst_AssignConst(ob,Dee_None)

//////////////////////////////////////////////////////////////////////////
// Calculates the total offset and final type of an expression tree like the following:
// >> ((a *)x)->b.c.d; (added offsets of b,c,d + integral value of 'x')
// in "xast.offsetof.inl"
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
extern /*noexcept*/int DeeXAst_GetOffsetof(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_OUT Dee_size_t *final_offset,
 DEE_A_OUT DeeTypeObject **final_type);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

//////////////////////////////////////////////////////////////////////////
// Create a new constant/variable AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewConst(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeObject *ast_const);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *ast_var,
 DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewReferenceVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *ast_var,
 DEE_A_INOUT DeeScopeObject *curr_scope);
#ifdef DEE_DEBUG
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewLocalVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *ast_var,
 DEE_A_INOUT DeeScopeObject *curr_scope);
#else
#define DeeXAst_NewLocalVar(tk,ast_var,curr_scope) _DeeXAst_NewLocalVar(tk,ast_var)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewLocalVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *ast_var);
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the size of a tuple-style XAst
// - Returns '(Dee_size_t)-1' if not a tuple or size couldn't be
//   determined, as in a tuple containing an expand expression.
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_TupleSize(DEE_A_IN DeeXAstObject const *self);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_ListSize(DEE_A_IN DeeXAstObject const *self);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_SetSize(DEE_A_IN DeeXAstObject const *self);
extern DEE_A_RET_WUNUSED Dee_size_t DeeXAst_SequenceSize(DEE_A_IN DeeXAstObject const *self);

#define DeeXAst_IsTuple(ob)\
 ((ob)->ast_kind == DEE_XASTKIND_TUPLE ||\
 ((ob)->ast_kind == DEE_XASTKIND_CONST && DeeTuple_Check((ob)->ast_const.c_const)) ||\
 ((ob)->ast_kind == DEE_XASTKIND_MULTIVARDECL && (ob)->ast_multivardecl.mvd_type == DEE_XASTKIND_TUPLE))
#define DeeXAst_IsList(ob)\
 ((ob)->ast_kind == DEE_XASTKIND_LIST ||\
 ((ob)->ast_kind == DEE_XASTKIND_CONST && DeeList_Check((ob)->ast_const.c_const)) ||\
 ((ob)->ast_kind == DEE_XASTKIND_MULTIVARDECL && (ob)->ast_multivardecl.mvd_type == DEE_XASTKIND_LIST))
#define DeeXAst_IsSet(ob)\
 ((ob)->ast_kind == DEE_XASTKIND_SET ||\
 ((ob)->ast_kind == DEE_XASTKIND_CONST && DeeSet_Check((ob)->ast_const.c_const)) ||\
 ((ob)->ast_kind == DEE_XASTKIND_MULTIVARDECL && (ob)->ast_multivardecl.mvd_type == DEE_XASTKIND_SET))
#define DeeXAst_IsDict(ob)\
 ((ob)->ast_kind == DEE_XASTKIND_DICT ||\
 ((ob)->ast_kind == DEE_XASTKIND_CONST && DeeDict_Check((ob)->ast_const.c_const)))


//////////////////////////////////////////////////////////////////////////
// NOTE: The following functions will already perform optimizations as
//       specified and possible by the given arguments and parser_flags
#ifdef DEE_DEBUG
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeXAstObject *) DeeXAst_NewZeroOp(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk) {
 DEE_ASSERT(DEE_XASTKIND_ISZEROOP(kind));
 return _DeeXAst_NewUnsafe(kind,tk);
}
#else
#define DeeXAst_NewZeroOp   _DeeXAst_NewUnsafe
#endif
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewUnary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBinary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewTrinary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b,
 DEE_A_INOUT DeeXAstObject *ast_c);

extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewCast(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *cast_ob,
 DEE_A_INOUT DeeXAstObject *result_type);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewCastC(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *cast_ob,
 DEE_A_INOUT DeeTypeObject const *result_type);

//////////////////////////////////////////////////////////////////////////
// Private functions for creating inplace-var asts (don't use these!)
// >> They are automatically invoked when an appropriate AST is being generated
// NOTE: If 'local_var' isn't a scope-local variable in the
//       context of this ast, undefined behavior is invoked
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewUnaryInplaceVar(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeTokenObject *var_tk, DEE_A_INOUT DeeLocalVarObject *local_var);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewBinaryInplaceVar(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeTokenObject *var_tk, DEE_A_INOUT DeeLocalVarObject *local_var,
 DEE_A_INOUT DeeXAstObject *ast_arg);

#define DeeXAst_NewStr(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_STR,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewRepr(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_REPR,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewCopy(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_COPY,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewMove(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_MOVE,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewType(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_TYPE,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewWeak(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_WEAK,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewBool(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewSuperOf(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_SUPEROF,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewClassOf(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_CLASSOF,tk,lexer,parser_flags,ast_ob)
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DeeXAst_NewRef(tk,lexer,parser_flags,ast_lval) DeeXAst_NewUnary(DEE_XASTKIND_REF,tk,lexer,parser_flags,ast_lval)
#define DeeXAst_NewDeref(tk,lexer,parser_flags,ast_ptr) DeeXAst_NewUnary(DEE_XASTKIND_DEREF,tk,lexer,parser_flags,ast_ptr)
#define DeeXAst_NewPtrOf(tk,lexer,parser_flags,ast_type) DeeXAst_NewUnary(DEE_XASTKIND_PTROF,tk,lexer,parser_flags,ast_type)
#define DeeXAst_NewLValOf(tk,lexer,parser_flags,ast_type) DeeXAst_NewUnary(DEE_XASTKIND_LVALOF,tk,lexer,parser_flags,ast_type)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#define DeeXAst_NewNot(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_NOT,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewNeg(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_NEG,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewPos(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_POS,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewInv(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_INV,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewInc(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_INC,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewDec(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_DEC,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewIncPost(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_INCPOST,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewDecPost(tk,lexer,parser_flags,ast_ob) DeeXAst_NewUnary(DEE_XASTKIND_DECPOST,tk,lexer,parser_flags,ast_ob)
#define DeeXAst_NewExpand(tk,lexer,parser_flags,ast_seq) DeeXAst_NewUnary(DEE_XASTKIND_EXPAND,tk,lexer,parser_flags,ast_seq)
#define DeeXAst_NewSeqSize(tk,lexer,parser_flags,ast_seq) DeeXAst_NewUnary(DEE_XASTKIND_SEQ_SIZE,tk,lexer,parser_flags,ast_seq)
#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
#define DeeXAst_NewNew(tk,lexer,parser_flags,ast_type) DeeXAst_NewUnary(DEE_XASTKIND_NEW,tk,lexer,parser_flags,ast_type)
#define DeeXAst_NewDelete(tk,lexer,parser_flags,ast_ptr) DeeXAst_NewUnary(DEE_XASTKIND_DELETE,tk,lexer,parser_flags,ast_ptr)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE */
#define DeeXAst_NewIterSelf(tk,lexer,parser_flags,ast_seq) DeeXAst_NewUnary(DEE_XASTKIND_ITERSELF,tk,lexer,parser_flags,ast_seq)
#define DeeXAst_NewIterNext(tk,lexer,parser_flags,ast_seq) DeeXAst_NewUnary(DEE_XASTKIND_ITERNEXT,tk,lexer,parser_flags,ast_seq)
#define DeeXAst_NewCell(tk,lexer,parser_flags,elem) DeeXAst_NewUnary(DEE_XASTKIND_CELL,tk,lexer,parser_flags,elem)
#define DeeXAst_NewBuiltinAlloca(tk,lexer,parser_flags,alloc_size) DeeXAst_NewUnary(DEE_XASTKIND_BUILTIN_ALLOCA,tk,lexer,parser_flags,alloc_size)
#define DeeXAst_NewBuiltinHelp(tk,lexer,parser_flags,object_) DeeXAst_NewUnary(DEE_XASTKIND_BUILTIN_HELP,tk,lexer,parser_flags,object_)
#define DeeXAst_NewBuiltinDex(tk,lexer,parser_flags,dex_name) DeeXAst_NewUnary(DEE_XASTKIND_BUILTIN_DEX,tk,lexer,parser_flags,dex_name)

#define DeeXAst_NewCall(tk,lexer,parser_flags,fun,args) DeeXAst_NewBinary(DEE_XASTKIND_CALL,tk,lexer,parser_flags,fun,args)
#define DeeXAst_NewCompareLo(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_LO,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCompareLe(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_LE,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCompareEq(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_EQ,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCompareNe(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_NE,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCompareGr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_GR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCompareGe(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COMPARE_GE,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewAdd(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ADD,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewSub(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SUB,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewMul(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_MUL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewDiv(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_DIV,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewMod(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_MOD,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewShl(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SHL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewShr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SHR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewAnd(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_AND,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewOr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_OR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewXor(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_XOR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewPow(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_POW,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceAdd(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IADD,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceSub(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ISUB,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceMul(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IMUL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceDiv(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IDIV,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceMod(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IMOD,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceShl(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ISHL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceShr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ISHR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceAnd(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IAND,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceOr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IOR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplaceXor(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IXOR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewInplacePow(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IPOW,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewSeqGet(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SEQ_GET,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewSeqDel(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SEQ_DEL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewAttrGet(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ATTR_GET,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewAttrHas(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ATTR_HAS,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewAttrDel(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_ATTR_DEL,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewIs(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IS,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewIn(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_IN,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewNotIs(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_NOT_IS,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewNotIn(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_NOT_IN,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewLAnd(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_LAND,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewLOr(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_LOR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewLXor(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_LXOR,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewMoveAssign(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_MOVE_ASSIGN,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewCopyAssign(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_COPY_ASSIGN,tk,lexer,parser_flags,lhs,rhs)
#define DeeXAst_NewSuperAt(tk,lexer,parser_flags,lhs,rhs) DeeXAst_NewBinary(DEE_XASTKIND_SUPER_AT,tk,lexer,parser_flags,lhs,rhs)
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
#define DeeXAst_NewFFClosure(tk,lexer,parser_flags,ff_type,ff_function) DeeXAst_NewBinary(DEE_XASTKIND_BUILTIN_FF_CLOSURE,tk,lexer,parser_flags,ff_type,ff_function)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */

#define DeeXAst_NewSeqSet(tk,lexer,parser_flags,seq,key,value) DeeXAst_NewTrinary(DEE_XASTKIND_SEQ_SET,tk,lexer,parser_flags,seq,key,value)
#define DeeXAst_NewAttrSet(tk,lexer,parser_flags,object_,attr,value) DeeXAst_NewTrinary(DEE_XASTKIND_ATTR_SET,tk,lexer,parser_flags,object_,attr,value)

#define DeeXAst_NewSeqAny(tk,lexer,parser_flags,seq) DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ANY,tk,lexer,parser_flags,seq)
#define DeeXAst_NewSeqAll(tk,lexer,parser_flags,seq) DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ALL,tk,lexer,parser_flags,seq)
#define DeeXAst_NewSeqSum(tk,lexer,parser_flags,seq) DeeXAst_NewUnary(DEE_XASTKIND_SEQ_SUM,tk,lexer,parser_flags,seq)

#define DeeXAst_NewIOReadnp(tk,lexer,parser_flags,file,size)     DeeXAst_NewBinary(DEE_XASTKIND_IO_READNP,tk,lexer,parser_flags,file,size)
#define DeeXAst_NewIOWritenp(tk,lexer,parser_flags,file,data)    DeeXAst_NewBinary(DEE_XASTKIND_IO_WRITENP,tk,lexer,parser_flags,file,data)
#define DeeXAst_NewIOSeek(tk,lexer,parser_flags,file,off,whence) DeeXAst_NewTrinary(DEE_XASTKIND_IO_SEEK,tk,lexer,parser_flags,file,off,whence)
#define DeeXAst_NewIOFlush(tk,lexer,parser_flags,file)           DeeXAst_NewUnary(DEE_XASTKIND_IO_FLUSH,tk,lexer,parser_flags,file)
#define DeeXAst_NewIOTrunc(tk,lexer,parser_flags,file)           DeeXAst_NewUnary(DEE_XASTKIND_IO_TRUNC,tk,lexer,parser_flags,file)
#define DeeXAst_NewIOClose(tk,lexer,parser_flags,file)           DeeXAst_NewUnary(DEE_XASTKIND_IO_CLOSE,tk,lexer,parser_flags,file)
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DeeXAst_NewIOReadp(tk,lexer,parser_flags,file,p,s)       DeeXAst_NewTrinary(DEE_XASTKIND_IO_READP,tk,lexer,parser_flags,file,p,s)
#define DeeXAst_NewIOWritep(tk,lexer,parser_flags,file,p,s)      DeeXAst_NewTrinary(DEE_XASTKIND_IO_WRITEP,tk,lexer,parser_flags,file,p,s)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */


//////////////////////////////////////////////////////////////////////////
// Create a new __builtin_unused - AST
#define DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,ast_eval,ast_result) \
 DeeXAst_NewBinary(DEE_XASTKIND_BUILTIN_UNUSED,tk,lexer,parser_flags,ast_eval,ast_result)

//////////////////////////////////////////////////////////////////////////
// Create a new sequence-style AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSequenceFromInheritedAstv(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_IN size_t astc, DEE_A_IN_R(astc) DeeXAstObject **astv);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSequence(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_IN size_t astc, DEE_A_IN_R(astc) DeeXAstObject *const *astv);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewEmptySequence(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_OptimizeSequence(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags);
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewEmptyTuple(tk) DeeXAst_NewEmptySequence(DEE_XASTKIND_TUPLE,tk)
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewEmptyList(tk)  DeeXAst_NewEmptySequence(DEE_XASTKIND_LIST,tk)
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewEmptySet(tk)   DeeXAst_NewEmptySequence(DEE_XASTKIND_SET,tk)
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewTuple(tk,lexer,parser_flags,astc,astv) DeeXAst_NewSequence(DEE_XASTKIND_TUPLE,tk,lexer,parser_flags,astc,astv)
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewList(tk,lexer,parser_flags,astc,astv)  DeeXAst_NewSequence(DEE_XASTKIND_LIST,tk,lexer,parser_flags,astc,astv)
#define /* DEE_A_RET_EXCEPT_REF DeeXAstObject * */DeeXAst_NewSet(tk,lexer,parser_flags,astc,astv)   DeeXAst_NewSequence(DEE_XASTKIND_SET,tk,lexer,parser_flags,astc,astv)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDictFromInheritedEntryv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN size_t entryc,
 DEE_A_IN_R(entryc) struct DeeXAstDictEntry *entryv);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDict(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN size_t entryc,
 DEE_A_IN_R(entryc) struct DeeXAstDictEntry const *entryv);

extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_OptimizeMultiVarDecl(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeScopeObject *curr_scope);

//////////////////////////////////////////////////////////////////////////
// Create a new statement-AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStatement(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT struct DeeSAstObject *ast_statement);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfFromIfSAst(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeSAstObject *if_sast,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfConstFromIfConstSAst(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT_OPT DeeSAstObject *succ_block, DEE_A_INOUT_OPT DeeScopeObject *succ_scope,
 DEE_A_INOUT_OPT DeeSAstObject *fail_block, DEE_A_INOUT_OPT DeeScopeObject *fail_scope,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags);

//////////////////////////////////////////////////////////////////////////
// Create a new module-AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewModule(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeScopeObject *module_);

//////////////////////////////////////////////////////////////////////////
// Create a new __builtin_bound-AST
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinBound(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeTokenObject *var_token,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeLocalVarObject *var_object, DEE_A_INOUT DeeScopeObject *curr_scope);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

//////////////////////////////////////////////////////////////////////////
// Create a new __builtin_expect-AST
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinExpect(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeXAstObject *ast_value,
 DEE_A_INOUT DeeObject *expected);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

//////////////////////////////////////////////////////////////////////////
// Create a new del-AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDelVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeTokenObject *var_token,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeLocalVarObject *del_var, DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDel(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *del_ast,
 DEE_A_INOUT DeeScopeObject *curr_scope);

//////////////////////////////////////////////////////////////////////////
// Create a new if-style AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIf(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_cond,
 DEE_A_INOUT_OPT DeeXAstObject *ast_succ, DEE_A_INOUT_OPT DeeXAstObject *ast_fail);
#define DeeXAst_NewBuiltinDead(tk,lexer,parser_flags,dead_branch)   DeeXAst_NewIfConst(DEE_XASTKIND_IFFALSE,tk,lexer,parser_flags,dead_branch,NULL)
#define DeeXAst_NewIfTrue(tk,lexer,parser_flags,ast_succ,ast_fail)  DeeXAst_NewIfConst(DEE_XASTKIND_IFTRUE,tk,lexer,parser_flags,ast_succ,ast_fail)
#define DeeXAst_NewIfFalse(tk,lexer,parser_flags,ast_succ,ast_fail) DeeXAst_NewIfConst(DEE_XASTKIND_IFFALSE,tk,lexer,parser_flags,ast_succ,ast_fail)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfConst(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OPT DeeXAstObject *ast_succ, DEE_A_INOUT_OPT DeeXAstObject *ast_fail);

//////////////////////////////////////////////////////////////////////////
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSwitchFromInheritedSwitches(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_IN Dee_size_t switch_c, DEE_A_IN_R(switch_c) struct DeeXAstSwitchEntry *switch_v,
 DEE_A_INOUT_OPT DeeXAstObject *switch_default);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSwitch(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_IN Dee_size_t switch_c, DEE_A_IN_R(switch_c) struct DeeXAstSwitchEntry const *switch_v,
 DEE_A_INOUT_OPT DeeXAstObject *switch_default);

//////////////////////////////////////////////////////////////////////////
// Create a new operator call AST
// >> $ast_this.operator $typeslot ($ast_args);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewOperatorCall(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_A_INOUT DeeXAstObject *ast_this,
 DEE_A_INOUT DeeXAstObject *ast_args, DEE_A_IN int typeslot,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags);

//////////////////////////////////////////////////////////////////////////
// Create a new attribute-lookup AST
#define DeeXAst_NewAttrGetC(tk,lexer,parser_flags,ast_object,attr_name) _DeeXAst_NewAttrGetHasOrDelC(DEE_XASTKIND_ATTR_GET_C,tk,lexer,parser_flags,ast_object,attr_name)
#define DeeXAst_NewAttrHasC(tk,lexer,parser_flags,ast_object,attr_name) _DeeXAst_NewAttrGetHasOrDelC(DEE_XASTKIND_ATTR_HAS_C,tk,lexer,parser_flags,ast_object,attr_name)
#define DeeXAst_NewAttrDelC(tk,lexer,parser_flags,ast_object,attr_name) _DeeXAst_NewAttrGetHasOrDelC(DEE_XASTKIND_ATTR_DEL_C,tk,lexer,parser_flags,ast_object,attr_name)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewAttrGetHasOrDelC(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_object, DEE_A_INOUT_OBJECT(DeeStringObject) *attr_name);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewAttrSetC(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_object,
 DEE_A_INOUT_OBJECT(DeeStringObject) *attr_name, DEE_A_INOUT DeeXAstObject *ast_value);

//////////////////////////////////////////////////////////////////////////
// Create a new sequence-range AST
#define DeeXAst_NewSeqRangeGet(tk,lexer,parser_flags,ast_object,ast_begin,ast_end) _DeeXAst_NewSeqGetOrDelRange(DEE_XASTKIND_SEQ_RANGE_GET,tk,lexer,parser_flags,ast_object,ast_begin,ast_end)
#define DeeXAst_NewSeqRangeDel(tk,lexer,parser_flags,ast_object,ast_begin,ast_end) _DeeXAst_NewSeqGetOrDelRange(DEE_XASTKIND_SEQ_RANGE_DEL,tk,lexer,parser_flags,ast_object,ast_begin,ast_end)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewSeqGetOrDelRange(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_object, DEE_A_INOUT_OPT DeeXAstObject *ast_begin,
 DEE_A_INOUT_OPT DeeXAstObject *ast_end);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSeqRangeSet(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_object,
 DEE_A_INOUT_OPT DeeXAstObject *ast_begin, DEE_A_INOUT_OPT DeeXAstObject *ast_end,
 DEE_A_INOUT DeeXAstObject *ast_value);

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
//////////////////////////////////////////////////////////////////////////
// Creates a new foreign_function type AST
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewForeignFunctionFromInheritedArgv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeXAstObject **argv);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewForeignFunction(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeXAstObject *const *argv);
#endif


//////////////////////////////////////////////////////////////////////////
// Creates a new AST describing the operation '$ast_dest = $ast_init'
//  - This creates __setattr__, __setitem__, __setrange__, VARDECL expressions
//  - Also creates MULTIVARDECL if 'ast_dest' is a tuple and 'ast_init' is an expand expression
//  - If 'ast_init' is NULL, create a vardecl without an initializer
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStore(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_dest,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStoreVardeclStorage(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeVarDeclStorage const *storage,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStoreVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeLocalVarObject *dest_var,
 DEE_A_INOUT DeeTokenObject *dest_var_token, DEE_A_INOUT DeeXAstObject *ast_init,
 DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewMultiStoreFromInheritedStorage(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN Dee_size_t storage_c,
 DEE_A_IN_R(storage_c) struct DeeVarDeclStorage *storage_v,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_IN DeeXAstKind return_seq_kind,
 DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewMultiStore(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN Dee_size_t storage_c,
 DEE_A_IN_R(storage_c) struct DeeVarDeclStorage const *storage_v,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_IN DeeXAstKind return_seq_kind,
 DEE_A_INOUT DeeScopeObject *curr_scope);

//////////////////////////////////////////////////////////////////////////
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewFunctionFromInheritedArgv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeCompilerConfig const *config,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) struct DeeXAstFunctionAstArgument *argv,
 DEE_A_INOUT_OPT struct DeeSAstObject *code, DEE_A_INOUT_OPT DeeScopeObject *scope);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewFunction(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeCompilerConfig const *config,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) struct DeeXAstFunctionAstArgument const *argv,
 DEE_A_INOUT_OPT struct DeeSAstObject *code, DEE_A_INOUT_OPT DeeScopeObject *scope);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
//////////////////////////////////////////////////////////////////////////
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewClassFromInheritedEntries(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_INOUT_OPT DeeXAstObject *base, DEE_A_IN Dee_uint32_t more_tpflags,
 DEE_A_IN Dee_size_t entryc, DEE_A_IN_R(entryc) struct DeeXAstClassAstEntry *entryv, DEE_A_IN_OPT struct DeeUUID const *uuid);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewClass(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_INOUT_OPT DeeXAstObject *base, DEE_A_IN Dee_uint32_t more_tpflags,
 DEE_A_IN Dee_size_t entryc, DEE_A_IN_R(entryc) struct DeeXAstClassAstEntry const *entryv, DEE_A_IN_OPT struct DeeUUID const *uuid);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

//////////////////////////////////////////////////////////////////////////
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinExtern(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT_OPT DeeXAstObject *object_file,
 DEE_A_INOUT DeeXAstObject *object_name, DEE_A_INOUT DeeXAstObject *object_type);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

//////////////////////////////////////////////////////////////////////////
// Initialize a vardecl storage from a given Expression AST
// >> Produces a warning and an assign-vardecl if 'ast_decl' isn't appropriate
// in "xast.vardeclstorage_from_xast.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitFromXAst(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_dest,
 DEE_A_INOUT DeeScopeObject *curr_scope);
extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitFromVar(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeLocalVarObject *dest_var,
 DEE_A_INOUT DeeTokenObject *dest_var_token, DEE_A_INOUT DeeScopeObject *curr_scope);



extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidUnaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_IN DeeTypeObject const *rt_error);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidBinaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_IN DeeTypeObject const *rt_error);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidTrinaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_INOUT DeeXAstObject *ast_c,
 DEE_A_IN DeeTypeObject const *rt_error);


//////////////////////////////////////////////////////////////////////////
// Descriptive operator strings
// >> Returns a string describing the use of an operator with given asts
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_UnaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_BinaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b);
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_TrinaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_INOUT DeeXAstObject *ast_c);



//////////////////////////////////////////////////////////////////////////
// Query semantic attribute of a given AST
typedef Dee_uint32_t DeeAstAttribute;
#define DEE_AST_ATTRIBUTE_NONE                           0x00000000 /*< Attributes that don't say anything special about the AST */
#define DEE_AST_ATTRIBUTE_NORETURN                       0x00000001 /*< Given certain restrictions, the AST will not return at runtime. (making following code possibly unreachable) */
#define DEE_AST_ATTRIBUTE_NOEXCEPT                       0x00000002 /*< The given AST will never throw an exception. */
#define DEE_AST_ATTRIBUTE_NOEFFECT                       0x00000004 /*< Executing the given AST does not have any side-effects. */
#define DEE_AST_ATTRIBUTE_NOJUMP                         0x00000008 /*< The AST will always return somehow (because it doesn't jump outside the visible scope). */
#define DEE_AST_ATTRIBUTE_FLAG_NONE                      0x00000000
#define DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK      0x00000001 /*< Allow 'break' and 'continue' to affect 'DEE_XAST_ATTRIBUTE_NORETURN' */
#define DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT 0x00000002 /*< noreturn requires noexcept (set inside of try-blocks) */
#define DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS            0x00000004 /*< Consider all exceptions (including low-probability out-of-memory, or unbound local errors; Only set when querying for behavior) */

//////////////////////////////////////////////////////////////////////////
// Returns a set of attributes that should be used for expressions that always throw an error
#define DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags)\
 (DeeAstAttribute)(((flags)&DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT)!=0\
                 ? (DEE_AST_ATTRIBUTE_NONE|DEE_AST_ATTRIBUTE_NOJUMP) : DEE_AST_ATTRIBUTE_NORETURN)

// Attributes of a no-op AST
// NOTE: Merging with these never has any effect
#define DEE_AST_ATTRIBUTE_NOOP \
 (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT|\
  DEE_AST_ATTRIBUTE_NOJUMP)

// Once these attributes are reached, calling 'DeeAstAttribute_MergeSequential'
// will no longer have any effect
#define DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX \
 (DEE_AST_ATTRIBUTE_NORETURN)

//////////////////////////////////////////////////////////////////////////
// Merge attributes of concurrent asts
// {
//   a; // Ast for the 'a' attributes
//   b; // Ast for the 'b' attributes
// }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED DeeAstAttribute) DeeAstAttribute_MergeSequential(
 DEE_A_IN DeeAstAttribute a, DEE_A_IN DeeAstAttribute b, DEE_A_IN Dee_uint32_t flags) {
 DeeAstAttribute result = a&b; // Attributes shared
 if ((flags&DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT)!=0) {
  if ((a&(DEE_AST_ATTRIBUTE_NORETURN)) == (DEE_AST_ATTRIBUTE_NORETURN)) {
   // Once noreturn, always noreturn
   result |= DEE_AST_ATTRIBUTE_NORETURN;
  } else if ((b&DEE_AST_ATTRIBUTE_NORETURN)!=0) {
   // Only if 'a' always returns and 'a' and 'b' are noexcept
   if ((a&(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP)
       )==(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP) &&
       (b&(DEE_AST_ATTRIBUTE_NOEXCEPT))==(DEE_AST_ATTRIBUTE_NOEXCEPT)
       ) result |= DEE_AST_ATTRIBUTE_NORETURN;
  }
 } else {
  if ((a&(DEE_AST_ATTRIBUTE_NORETURN)) == (DEE_AST_ATTRIBUTE_NORETURN))
   result |= DEE_AST_ATTRIBUTE_NORETURN; // If a is noreturn, the result is too
  else if ((a&(DEE_AST_ATTRIBUTE_NOJUMP)) == (DEE_AST_ATTRIBUTE_NOJUMP) &&
           (b&(DEE_AST_ATTRIBUTE_NORETURN)) == (DEE_AST_ATTRIBUTE_NORETURN))
   result |= DEE_AST_ATTRIBUTE_NORETURN; // If b is noreturn and a always returns, the result is noreturn
 }
 return result;
}

//////////////////////////////////////////////////////////////////////////
// Merge attributes of parallel asts
// if (x) {
//   a; // Ast for the 'a' attributes
// } else {
//   b; // Ast for the 'b' attributes
// }
#ifdef __INTELLISENSE__
#define DeeAstAttribute_MergeParallel(a,b,flags) (__noop(flags),(a)&(b))
#else
#define DeeAstAttribute_MergeParallel(a,b,flags) ((a)&(b))
#endif

//////////////////////////////////////////////////////////////////////////
// Returns true if the given set of flags indicates code that will always,
// under all circumstances return. Implying that it isn't noreturn, is noexcept,
// and doesn't contain any jumps that would point outside of its block.
#define DEE_AST_ATTRIBUTE_IS_ALWAYSRETURN(flags)\
 (((flags)&(DEE_AST_ATTRIBUTE_NORETURN|DEE_AST_ATTRIBUTE_NOJUMP|DEE_AST_ATTRIBUTE_NOEXCEPT))==\
                                      (DEE_AST_ATTRIBUTE_NOJUMP|DEE_AST_ATTRIBUTE_NOEXCEPT))

//////////////////////////////////////////////////////////////////////////
// Returns true if the given set of flags indicates code
// that will not, under any circumstances ever return.
// Note though, that this behavior can be archived by two
// different means, the first of which is always causing an exception,
// and the second of which is to jump somewhere out of the code.
// WARNING: Remember, that this jump destination may just be the next
//          statement, if that statement has any labels.
//          So only use this function to enable unreachable during optimization,
//          but make sure that you re-enable reachable as soon as any labels are encountered.
#define DEE_AST_ATTRIBUTE_IS_NEVERRETURN(flags)\
 (((flags)&(DEE_AST_ATTRIBUTE_NORETURN))==(DEE_AST_ATTRIBUTE_NORETURN))


// In "xast.typeattributes.inl"
extern DEE_A_RET_WUNUSED int DeeType_SlotIdFromXAstKind(DEE_A_IN DeeXAstKind kind);
extern DeeAstAttribute DeeType_UnaryAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN_OPT Dee_uint32_t flags, DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DeeAstAttribute DeeType_BinaryAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OPT DeeTypeObject const *tb,
 DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DeeAstAttribute DeeType_TrinaryAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OPT DeeTypeObject const *tb,
 DEE_A_IN_OPT DeeTypeObject const *tc, DEE_A_OUT_OPT DeeTypeObject const **result_type);
#define DeeType_GetRangeAstAttr(self,flags,tbegin,tend,result_type) DeeType_TrinaryAstAttr(self,DeeType_SLOT_ID(tp_seq_range_get),flags,tbegin,tend,result_type)
#define DeeType_DelRangeAstAttr(self,flags,tbegin,tend)             DeeType_TrinaryAstAttr(self,DeeType_SLOT_ID(tp_seq_range_del),flags,tbegin,tend,NULL)
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeType_SetRangeAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OPT DeeTypeObject const *tbegin, DEE_A_IN_OPT DeeTypeObject const *tend,
 DEE_A_IN_OPT DeeTypeObject const *tvalue);
extern DeeAstAttribute DeeType_CallAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DeeAstAttribute DeeObject_CallAstAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DeeAstAttribute DeeType_GetAttrAstAttr(DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeType_HasAttrAstAttr(DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member);
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeType_DelAttrAstAttr(DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member);
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeType_SetAttrAstAttr(DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_IN_OPT DeeTypeObject const *tvalue);
extern DeeAstAttribute DeeType_CallAttrAstAttr(DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv, DEE_A_OUT_OPT DeeTypeObject const **result_type);
extern DeeAstAttribute DeeObject_CallAttrAstAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self, DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv, DEE_A_OUT_OPT DeeTypeObject const **result_type);


//////////////////////////////////////////////////////////////////////////
// The root_ast argument is used to determine the greatest label:
// >> assert !__builtin_noreturn(({ local x; if (x) goto local_label;  return 42; local_label: print "HERE"; }));
// >> assert  __builtin_noreturn(({ local x; if (x) goto global_label; return 42;              print "HERE"; }));
// >> global_label:;
// In "xast.attributes.inl"
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeXAst_AttrEx(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast);
extern DEE_A_RET_WUNUSED DeeAstAttribute DeeVarDeclStorage_AttrEx(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast);
#define DeeXAst_Attr(ob,flags)  DeeXAst_AttrEx(ob,flags,(DeeObject *)(ob))
#define DeeXAst_IsNoReturn(ob,flags) ((DeeXAst_Attr(ob,flags)&DEE_AST_ATTRIBUTE_NORETURN)!=0)
#define DeeXAst_IsNoExcept(ob)       ((DeeXAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_NONE)&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0)
#define DeeXAst_IsNoEffect(ob)       ((DeeXAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_NONE)&DEE_AST_ATTRIBUTE_NOEFFECT)!=0)
#define DeeXAst_IsReallyNoExcept(ob) ((DeeXAst_Attr(ob,DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0)


//////////////////////////////////////////////////////////////////////////
// Predict the runtime typing of an XAst.
//  - Returns NULL, but doesn't throw an error if the AST is unpredictable
// In: "xast.predict_type.inl"
extern DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
 DeeXAst_PredictType(DEE_A_IN DeeXAstObject const *self);

//////////////////////////////////////////////////////////////////////////
// Returns the name of an AST object, as generated for use in class/function names
// In: "xast.repr.inl"
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeXAst_Name(DEE_A_IN DeeXAstObject const *self);
extern DEE_A_RET_WUNUSED DEE_A_RET_Z char const *_DeeType_ClassOperatorName(DEE_A_IN int slot);

//////////////////////////////////////////////////////////////////////////
// Returns the iterator type of known sequence types
//  - Returns NULL if 'ty' was NULL or unknown
// >> _DeeType_IteratorType(&DeeList_Type) == &DeeListIterator_Type
// In: "xast.predict_type.inl"
extern DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
 _DeeType_IteratorType(DEE_A_IN DeeTypeObject const *seq_type);

//////////////////////////////////////////////////////////////////////////
// Returns true if 'ob' can be called at compile-time
// In: "xast.new_binary.inl"
extern DEE_A_RET_WUNUSED int _DeeObject_IsConstexprCallable(
 DEE_A_IN DeeObject const *ob, DEE_A_IN_OBJECT(DeeTypeObject) const *args);

//////////////////////////////////////////////////////////////////////////
// Returns the intrinsic function of a given typeslot operator.
//  - Returns NULL if the typeslot doesn't have an intrinsic function
// NOTE: Special intrinsics are returned for dual-meaning typeslots,
//       as listed in the comment of 'DeeXAstOperatorName_Parse'.
//       e.g.: DeeBuiltin_GetIntrinsicFunctionOfTypeSlot(tp_p_add)
//             returns a function can either invoke 'tp_add' or 'tp_pos'.
// In: "xast.intrinsic_function_slots.inl"
extern DEE_A_RET_MAYBE_NULL DeeObject *DeeBuiltin_GetIntrinsicFunctionOfTypeSlot(DEE_A_IN int typeslot);

//////////////////////////////////////////////////////////////////////////
// Returns true (1) if 'id' is a token commonly found after an expression.
// This function might be a little wonky, but is required for proper implementation
// of creation of pointer types vs. operator __mul__, alongside some other places.
// -> Ids that return true include: ']', ')', ',', ';', '.' as well as a bunch of others.
//    Note though, that this is a whitelist of token ids that should include any token
//    that can't appear at the start of an expression.
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionBeginDecl(DEE_A_IN TPPTokenID id);
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionBegin(
 DEE_A_IN TPPTokenID id, DEE_A_IN DeeScopeObject const *scope);
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionEnd(DEE_A_IN TPPTokenID id);
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsAttribute(DEE_A_IN TPPTokenID id);
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsCTypeSuffixBegin(DEE_A_IN TPPTokenID id);

#define DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS 0

#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
extern DEE_A_RET_WUNUSED int DeeXAst_TokenIsReservedIdentifier(DEE_A_IN DeeTokenObject *tok);
extern DEE_A_RET_WUNUSED int DeeXAst_CheckReservedToken(
 DEE_A_IN DeeLexerObject *lexer, DEE_A_IN DeeTokenObject *tok);
#endif





#ifndef DEE_COMPILER_PARAMS
struct DeeCodeWriter;
#define DEE_COMPILER_PARAMS \
 DEE_A_INOUT struct DeeCodeWriter *writer, DEE_A_INOUT struct DeeLexerObject *lexer, \
 DEE_A_INOUT struct DeeScopeObject *scope, DEE_A_IN struct DeeCompilerConfig const *config, DEE_A_IN Dee_uint32_t compiler_flags
#define DEE_COMPILER_ARGS                                writer,lexer,scope,config,compiler_flags
#define DEE_COMPILER_ARGS_SCOPE(scope)                   writer,lexer,scope,config,compiler_flags
#define DEE_COMPILER_ARGS_EX(compiler_flags)             writer,lexer,scope,config,compiler_flags
#define DEE_COMPILER_ARGS_SCOPE_EX(scope,compiler_flags) writer,lexer,scope,config,compiler_flags
#endif

//////////////////////////////////////////////////////////////////////////
// Compiles the given Expression AST
// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+1'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_Compile(
 DEE_A_INOUT DeeXAstObject *self, DEE_COMPILER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int _DeeXAst_CompileFunctionGenerateReturnCode(
 DEE_A_INOUT struct DeeSAstObject *code, DEE_A_INOUT DeeScopeObject *scope,
 DEE_A_IN Dee_uint32_t function_flags, DEE_A_IN Dee_uint32_t scope_flags,
 DEE_A_IN Dee_uint32_t compiler_flags, DEE_A_INOUT struct DeeCodeWriter *writer);

//////////////////////////////////////////////////////////////////////////
// Compiles the given Expression AST
// NOTE: 'initializer' will be expanded into 'storagec' elements
// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+storagec'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileMultiVarDecl(
 DEE_A_IN Dee_size_t storagec, DEE_A_IN_R(storagec) struct DeeVarDeclStorage *storagev,
 DEE_A_IN DeeXAstObject *initializer, DEE_COMPILER_PARAMS);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
// Stackeffect: '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileClassEntry(
 DEE_A_IN struct DeeXAstClassAstEntry const *entry, DEE_COMPILER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileNewClass(
 DEE_A_IN_OPT DeeXAstObject *class_base, DEE_A_IN_OPT DeeStringObject *class_name,
 DEE_A_IN Dee_uint32_t more_type_flags, DEE_A_IN_OPT struct DeeUUID const *uuid, DEE_COMPILER_PARAMS);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+1'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileSequence(
 DEE_A_IN uint8_t seq_op, DEE_A_IN uint8_t seq_concat_op,
 DEE_A_IN uint8_t seq_cast_op, DEE_A_IN DeeTypeObject *seq_type,
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeXAstObject *const *elemv,
 DEE_COMPILER_PARAMS);

// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+1'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '+0'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileDict(
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) struct DeeXAstDictEntry const *elemv,
 DEE_COMPILER_PARAMS);

// Stackeffect: w/  DEE_COMPILER_FLAG_USED '+0'
// Stackeffect: w/o DEE_COMPILER_FLAG_USED '-1'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileStore(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_COMPILER_PARAMS);















//////////////////////////////////////////////////////////////////////////
// -==================================================================- //
// |  XAst Parser functions                                           | //
// -==================================================================- //
//////////////////////////////////////////////////////////////////////////


#ifndef DEE_PARSER_PARAMS
#define DEE_PARSER_PARAMS \
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeScopeObject *scope,\
 DEE_A_IN DeeCompilerConfig const *config, DEE_A_IN Dee_uint32_t parser_flags
#define DEE_PARSER_ARGS                              lexer,scope,config,parser_flags
#define DEE_PARSER_ARGS_SCOPE(scope)                 lexer,scope,config,parser_flags
#define DEE_PARSER_ARGS_EX(parser_flags)             lexer,scope,config,parser_flags
#define DEE_PARSER_ARGS_SCOPE_EX(scope,parser_flags) lexer,scope,config,parser_flags
#endif



#ifdef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#undef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#endif
struct DeeAttributeDecl {
 DEE_A_REF struct DeeStringObject  *a_name;      /*< [0..1] Name override. */
 DEE_A_REF struct DeeStringObject  *a_depr;      /*< [0..1] Deprecation reason. */
 DEE_A_REF struct DeeXAstObject    *a_super;     /*< [0..1] Super class type. */
 DeeFunctionFlags                   a_fun_flags; /*< Function flags (used in extern functions). */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DeeAttributeDecl_GET_CALLING_CONVENTION(ob) ((ob)->a_fun_flags&DeeFunctionFlags_CC_MASK)
#else
#define DeeAttributeDecl_GET_CALLING_CONVENTION(ob) ((DeeFunctionFlags)(ob)->a_fun_flags)
#endif
#define DEE_ATTRIBUTE_DECL_FLAG_NONE       DEE_UINT32_C(0x00000000)
#define DEE_ATTRIBUTE_DECL_FLAG_UUID       DEE_UINT32_C(0x00000001) /*< UUID has been set. */
#define DEE_ATTRIBUTE_DECL_FLAG_COPYABLE   DEE_UINT32_C(0x00000002) /*< __attribute__((__copyable__)). */
#define DEE_ATTRIBUTE_DECL_FLAG_USED       DEE_UINT32_C(0x00000004) /*< __attribute__((__used__)). */
#define DEE_ATTRIBUTE_DECL_FLAG_UNUSED     DEE_UINT32_C(0x00000008) /*< __attribute__((__unused__)). */
#define DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC DEE_UINT32_C(0x00000010) /*< __attribute__((__instance_bound__("copy"))). */
#define DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM DEE_UINT32_C(0x00000020) /*< __attribute__((__instance_bound__("move"))). */
 Dee_uint32_t                       a_flags;     /*< Function flags (used in extern functions). */
 Dee_size_t                         a_alignment; /*< Manual data alignment. */
 struct DeeUUID                     a_uuid;      /*< UUID attribute. */
};
#define DeeAttributeDecl_GET_FUNCTION_FLAGS(ob) \
(DeeAttributeDecl_GET_CALLING_CONVENTION(ob)==DeeFunctionFlags_THISCALL \
 ? DEE_FUNCTION_FLAG_THIS : DEE_FUNCTION_FLAG_NONE)

#define DeeAttributeDecl_INIT() \
 {NULL,NULL,NULL,DeeFunctionFlags_DEFAULT,\
  DEE_ATTRIBUTE_DECL_FLAG_NONE,0,DeeUUID_INIT()}
#define _DeeAttributeDecl_InitCopy(ob,right)\
do{\
 *(ob) = *(right);\
 Dee_XINCREF((ob)->a_depr);\
 Dee_XINCREF((ob)->a_super);\
 Dee_XINCREF((ob)->a_name);\
}while(0)
#define _DeeAttributeDecl_Quit(ob)\
do{\
 Dee_XDECREF((ob)->a_depr);\
 Dee_XDECREF((ob)->a_super);\
 Dee_XDECREF((ob)->a_name);\
}while(0)
// v in "xast_parse.attribute.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_Parse(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS);
//      vvvvvvvvvvvvvvvv
// >> [[name("foobar")]] 42
//                       ^^
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseAfterSecondBracket(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseContent(
 DEE_A_INOUT struct DeeAttributeDecl *self,
 DEE_A_IN TPPTokenID end_token, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseNoReturn(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseNoExcept(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeAttributeDecl_ParseAlignas(
 DEE_A_INOUT struct DeeAttributeDecl *self, DEE_PARSER_PARAMS);
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_ATTRIBUTEDECL_TOKENS_NOUNARY \
 case KWD__declspec: case KWD___declspec:\
 case KWD___attribute: case KWD___attribute__:\
 case KWD__syssv: case KWD___syssv:\
 case KWD__stdcall: case KWD___stdcall:\
 case KWD__thiscall: case KWD___thiscall:\
 case KWD__fastcall: case KWD___fastcall:\
 case KWD__cdecl: case KWD___cdecl:\
 case KWD__pascal: case KWD___pascal:\
 case KWD__register: case KWD___register:\
 case KWD__win64: case KWD___win64:\
 case KWD__unix64: case KWD___unix64:\
 case KWD__Alignas:
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DEE_ATTRIBUTEDECL_TOKENS_NOUNARY \
 case KWD__declspec: case KWD___declspec:\
 case KWD___attribute: case KWD___attribute__:\
 case KWD__Alignas:
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DEE_ATTRIBUTEDECL_TOKENS \
 DEE_ATTRIBUTEDECL_TOKENS_NOUNARY case '[':\
 case KWD__Noexcept: case KWD__Noreturn:




#define DEE_XAST_UNARAYSUFFIX_FLAG_NONE     0x00000000
#define DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND 0x00000001
// in "xast_parse.unary_with_suffix.inl": x[y]; x[y:z]; x.y; x...; x++; x--;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_ParseUnarySuffixWith(
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_IN Dee_uint32_t flags, DEE_PARSER_PARAMS);
// in "xast_parse.c"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnaryEx(
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);

// in "xast_parse.cast_suffix.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCastSuffix(
 DEE_A_INOUT DeeXAstObject *cast_type, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Var declaration modes for 'DeeXAst_ParseVariableLookup'
// NOTE: These flags can be overwritten by with keywords such as 'local'
// Exception: 'DEE_XAST_VARDECL_MODE_DISABLED' cannot be overwritten (if it's set, no variables may be created)
#define DEE_XAST_VARDECL_MODE_DEFAULT   0x00000000 /*< Lookup underlying variables; create+init new in current scope. */
#define DEE_XAST_VARDECL_MODE_LOCAL     0x00000001 /*< Create+init new in current scope. */
#define DEE_XAST_VARDECL_MODE_GLOBAL    0x00000002 /*< Only search global (don't allow creation unless inside the global scope). */
#define DEE_XAST_VARDECL_FLAG_ENABLED   0x00000100 /*< Allow declaration of new variables. */
#define DEE_XAST_VARDECL_FLAG_FORWARD   0x00000200 /*< Warn is the referenced variable was already initialized. */
#define DEE_XAST_VARDECL_FLAG_STATIC    0x00000400 /*< Create static variables / Warn is a variable isn't static. */
#define DEE_XAST_VARDECL_FLAG_MODULE    0x00000800 /*< Only lookup modules. */
#define DEE_XAST_VARDECL_FLAG_CONST     0x00001000 /*< Create constants / Warn is a variable isn't constant. */
#define DEE_XAST_VARDECL_FLAG_STACK     0x00002000 /*< Create stack variables / Warn if a variable isn't a stack variable. */
#define DEE_XAST_VARDECL_FLAG_VOLATILE  0x00004000 /*< Create volatile variables / Warn if a variable isn't volatile. */
#define DEE_XAST_VARDECL_MODE(x)   ((x)&0x000000FF)
#define DEE_XAST_VARDECL_FLAGS(x)  ((x)&0xFFFFFF00)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseVariableLookup(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.var_lookup.inl"

//////////////////////////////////////////////////////////////////////////
//    vvvvvvvv
// >> __nth(2) x = 10;
//             ^
// NOTE: Stores the value of __nth-1 in '*nth' (e.g.: '__nth(2)' --> '*nth' == 1)
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupNTh(
 DEE_A_OUT Dee_size_t *nth, DEE_PARSER_PARAMS); // in "xast_parse.var_lookup.inl"
//////////////////////////////////////////////////////////////////////////
//    vvvvvvvvvvv
// >> __scope(-2) x = 10;
//                ^
// NOTE: Stores the value of '-__scope' in '*scope_uprefs' (e.g.: '__scope(-2)' --> '*scope_uprefs' == 2)
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupScope(
 DEE_A_OUT Dee_size_t *scope_uprefs, DEE_PARSER_PARAMS); // in "xast_parse.var_lookup.inl"

#ifdef DEE_PARSE_TOKENID_CASE_STACK
#define DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_STACK_VARIABLE DEE_PARSE_TOKENID_CASE_STACK
#else
#define DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_STACK_VARIABLE /* nothing */
#endif
#define DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_NOSTMT \
 case KWD_forward: case KWD_static: case KWD_local:\
 DEE_PARSE_TOKENID_CASE_CONST DEE_PARSE_TOKENID_CASE_VOLATILE\
 DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_STACK_VARIABLE\
 DEE_PARSE_TOKENID_CASE_GLOBAL

#define DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS \
 case KWD_module: DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_NOSTMT

#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_FINAL case KWD_final:
#else /* ... */
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_FINAL /* nothing */
#endif /* !... */
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_STRUCT_TYPES case KWD_union:case KWD_struct:
#else /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_STRUCT_TYPES /* nothing */
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_CLASS_TYPES case KWD_class:
#else /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_CLASS_TYPES /* nothing */
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ENUM_TYPES case KWD_enum:
#else /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ENUM_TYPES /* nothing */
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_ALIAS
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ALIAS case KWD_alias:
#else /* DEE_CONFIG_LANGUAGE_HAVE_ALIAS */
#define DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ALIAS /* nothing */
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_ALIAS */
#define DEE_XAST_VARIABLE_DECL_TOKENS case KWD_function:\
 DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_FINAL\
 DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_STRUCT_TYPES\
 DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_CLASS_TYPES\
 DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ENUM_TYPES\
 DEE_PRIVATE_XAST_VARIABLE_LOOKUP_TOKENS_ALIAS

#define DEE_XAST_VARIBLE_LOOKUP_TOKENS_NOSTMT \
 DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS_NOSTMT \
 case KWD___nth: case KWD___scope: DEE_XAST_VARIABLE_DECL_TOKENS
#define DEE_XAST_VARIBLE_LOOKUP_TOKENS \
 DEE_XAST_VARIBLE_LOOKUP_TOKENS_NOSTMT case KWD_module:



extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseVariableLookupModifiers(
 DEE_A_INOUT Dee_uint32_t *vardecl_mode, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.var_lookup.inl"

extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRawUnaryEx(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, 
 DEE_PARSER_PARAMS); // in "xast_parse.unary.inl": (x); x; 42; function; class; struct; [...]; ++x; --x; [...];


// Values for the out-parameter 'syntax_mode' in some of the functions below
#define DEE_XAST_FUNCTION_SYNTAX_MODE_NONE  0x00000000 // Empty function
#define DEE_XAST_FUNCTION_SYNTAX_MODE_EXPR  0x00000001 // Function block is an expression
#define DEE_XAST_FUNCTION_SYNTAX_MODE_STMT  0x00000002 // Function block is a statement
#define DEE_XAST_FUNCTION_SYNTAX_FLAGS_NONE 0x00000000
#define DEE_XAST_FUNCTION_SYNTAX_MODE(x)   ((x)&0x0000FFFF)
#define DEE_XAST_FUNCTION_SYNTAX_FLAGS(x)  ((x)&0xFFFF0000)

//////////////////////////////////////////////////////////////////////////
// Parses a function (starts on the first token after 'function' or '[]', ... which should be a keyword, '(', '{' or '->')
//  - 'vardecl_mode' behaves the same as in 'DeeXAst_ParseVariableLookup'
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS); // in "xast_parse.function.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseFunctionWithAttribute(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.function.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS); // in "xast_parse.function.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantFunctionWithAttribute(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.function.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnnamedFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS); // in "xast_parse.function.inl"
// Parses the arguments of a function
//  - Adds the 'DEE_FUNCTION_FLAG_VARG' flag to '*flags' if the arguments are variadic
//  - Stores a vector and size in '*out_argc' and '*out_argv', then owned by the caller
// NOTE: Call this in the context of the function's scope
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseFunctionArgs(
 DEE_A_OUT Dee_size_t *out_argc, DEE_A_OUT struct DeeXAstFunctionAstArgument **out_argv,
 DEE_A_INOUT Dee_uint32_t *flags, DEE_PARSER_PARAMS); // in "xast_parse.function.inl"

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define DEE_XAST_PARSECLASS_FLAGS_NONE  0x00000000
#define DEE_XAST_PARSECLASS_FLAGS_FINAL 0x00000001
#define DEE_XAST_PARSECLASS_FLAGS_GETTYPEFLAGS(flags)\
 (((flags)&DEE_XAST_PARSECLASS_FLAGS_FINAL)!=0?DEE_TYPE_FLAG_NO_SUBCLASS:0)

extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseClass(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnnamedClass(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN TPPTokenID real_class_name,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantClassWithAttribute(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseClassWithAttribute(
 DEE_A_INOUT DeeTokenObject *class_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES ||\
    DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES ||\
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
// Parses the base of a class and stores it in '*base'
// NOTE: If no base is found, store NULL in '*base'
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassBase(
 DEE_A_REF DEE_A_OUT DeeXAstObject **base, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
#endif /* ... */

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassEntry(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT DeeXAstObject *class_base, DEE_PARSER_PARAMS); // in "xast_parse.class_entry.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassEntryWithAttribute(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class_entry.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseConstantClassEntry(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *class_object, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT DeeXAstObject *class_base, DEE_PARSER_PARAMS); // in "xast_parse.class_entry.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseConstantClassEntryWithAttribute(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *class_object, DEE_A_IN TPPTokenID class_name,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *class_name_str,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class_entry.inl"

// v initializes the 'cg_get', 'cg_del' and 'cg_set' members
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlock(
 DEE_A_OUT struct DeeXAstClassAstEntry *self,
 DEE_A_IN DeeXAstObject *class_base, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlockEntry(
 DEE_A_INOUT struct DeeXAstClassAstEntry *self,
 DEE_A_IN DeeXAstObject *class_base, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseClassPropertyBlockEntryWithAttribute(
 DEE_A_INOUT struct DeeXAstClassAstEntry *self,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.class.inl"
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
#define DEE_XAST_PARSESTRUCT_FLAG_NONE  0x00000000
#define DEE_XAST_PARSESTRUCT_FLAG_UNION 0x00000001 /*< Parse as a union. */
#define DEE_XAST_PARSESTRUCT_FLAG_FINAL 0x00000002 /*< Parse as a final struct/union. */
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseStruct(
 DEE_A_INOUT DeeTokenObject *struct_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseStructWithAttribute(
 DEE_A_INOUT DeeTokenObject *struct_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlock(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *struct_object,
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"

struct DeeXAstStructDecl {
 DeeTypeObject *s_object;         /*< [1..1] The struct being declared. */
 Dee_size_t     s_current_offset; /*< Current offset for members. */
 Dee_uint32_t   s_flags;          /*< Struct flags: 'DEE_XAST_STRUCT_FLAG_*'. */
};
#define _DeeXAstStructDecl_AlignOffset(offset,align)\
 (((offset)+(align)-1)&((Dee_size_t)(-(Dee_ssize_t)(align))))

extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlockEntryWithAttribute(
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_IN Dee_uint32_t vardecl_mode, // in "xast_parse.struct.inl"
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseStructBlockEntryWithAttributeCopy(
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_IN Dee_uint32_t vardecl_mode, // in "xast_parse.struct.inl"
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeXAstStructDecl_AddMember( // in "xast_parse.struct.inl"
 DEE_A_INOUT struct DeeXAstStructDecl *decl, DEE_A_INOUT struct DeeAttributeDecl const *attr,
 DEE_A_INOUT DeeObject *name, DEE_A_INOUT DeeTypeObject *type_,
 DEE_A_INOUT DeeTokenObject *name_token, DEE_A_INOUT DeeLexerObject *lexer);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */

#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
//////////////////////////////////////////////////////////////////////////
// Parses an enum type
// in "xast_parse.enum.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseEnum(
 DEE_A_INOUT DeeTokenObject *enum_token,
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseEnumWithAttribute(
 DEE_A_INOUT DeeTokenObject *enum_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseEnumBlock(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT DeeTypeObject *elem_type,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
struct DeeXAstEnumDecl {
           DeeTypeObject *e_type; /*< [1..1] Enum element type. */
 DEE_A_REF DeeObject     *e_last; /*< [0..1] Last enum constant. */
};
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseEnumBlockEntryWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT struct DeeXAstEnumDecl *decl,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.struct.inl"
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */


//////////////////////////////////////////////////////////////////////////
// Ast parsing (low-level to high-level)
// in "xast_parse.c"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseType(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.type.inl":  int; float; double; [...];
#define DeeXAst_ParseUnary(...) DeeXAst_ParseUnaryEx(\
 DEE_XAST_UNARAYSUFFIX_FLAG_NONE,DEE_XAST_VARDECL_MODE_DEFAULT,NULL,__VA_ARGS__)

extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseProdEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);   // x * y; x / y; x % y; x ** y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSumEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);    // x + y; x - y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseShiftEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);  // x << y; x >> y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCmpEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);    // x < y; x <= y; x > y; x >= y; x in y; x is y; x !in y; x !is y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseCmpEqEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);  // x == y; x != y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAndEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);    // x & y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseXorEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);    // x ^ y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOrEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);     // x | y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLandEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);   // x && y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLxorEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);   // x ^^ y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseLorEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);    // x || y;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseIfEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);     // x ? y : z;
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAssignEx(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS); // x := y; x += y; x -= y; [...]; x **= y;
#define DeeXAst_ParseProd(...)    DeeXAst_ParseProdEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseSum(...)     DeeXAst_ParseSumEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseShift(...)   DeeXAst_ParseShiftEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseCmp(...)     DeeXAst_ParseCmpEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseCmpEq(...)   DeeXAst_ParseCmpEqEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseAnd(...)     DeeXAst_ParseAndEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseXor(...)     DeeXAst_ParseXorEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseOr(...)      DeeXAst_ParseOrEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseLand(...)    DeeXAst_ParseLandEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseLxor(...)    DeeXAst_ParseLxorEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseLor(...)     DeeXAst_ParseLorEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseIf(...)      DeeXAst_ParseIfEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)
#define DeeXAst_ParseAssign(...)  DeeXAst_ParseAssignEx(DEE_XAST_VARDECL_MODE_DEFAULT,__VA_ARGS__)

#define DEE_XAST_INITORDECL_FLAGS_NONE             DEE_UINT32_C(0x00000000)
#define DEE_XAST_INITORDECL_FLAGS_ALLOWINIT        DEE_UINT32_C(0x00000001) /*< Allow initialization of variables. */
#define DEE_XAST_INITORDECL_FLAGS_SINGLEVAL        DEE_UINT32_C(0x00000002) /*< Stop at the first ',', not parsing tuples at the top level. */
#define DEE_XAST_INITORDECL_FLAGS_RETURNSEQ        DEE_UINT32_C(0x00000004) /*< Always return a sequence (even if no ',' was ever found). (NOTE: Also prevents expansion of top-level tuples) */
#define DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA      DEE_UINT32_C(0x00000008) /*< Don't parse an ignored ',' in the post expression (must be set if an ending ',' has special meaning, as in print statments). */
#define DEE_XAST_INITORDECL_FLAGS_FORCECTYPES      DEE_UINT32_C(0x00000010) /*< Force enable recursive c-types. */
#define DEE_XAST_INITORDECL_FLAGS_ALLOWRANGE       DEE_UINT32_C(0x00000020) /*< Allow range initializers. */
#define DEE_XAST_INITORDECL_FLAGS_NOSEQONCOLLON    DEE_UINT32_C(0x00000040) /*< Don't return 1-element sequence if a ':' terminated the expression (use together w/ 'DEE_XAST_INITORDECL_FLAGS_RETURNSEQ'). */
#define DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX   DEE_UINT32_C(0x00000080) /*< Don't parse the c-types suffix. */
#define DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON    DEE_UINT32_C(0x00000100) /*< Require the expression to end with a ';'. (never required in ff-closures) (incompatible w/ 'DEE_XAST_INITORDECL_FLAGS_NOSEQONCOLLON'). */
#define DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA   DEE_UINT32_C(0x00000200) /*< Allow ',' to suppress warnings about missing ';'. */
// 'seq_kind' must be one of the types allowed in 'DeeXAst_NewSequence'
// NOTE: 'syntax_mode' must be set to 0 by the caller (special syntactical event will then enable bits)
#define DEE_XAST_INITORDECL_SYNTAXMODE_ISSEQUENCE  DEE_UINT32_C(0x00000001) /*< Sequence (never set with 'DEE_XAST_INITORDECL_FLAGS_SINGLEVAL') */
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_ParseInitOrDecl(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstKind seq_kind, DEE_A_INOUT_OPT struct DeeAttributeDecl *attr,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS);

// Parse comma-separated expressions and Assign values to existing variables
#define DeeXAst_ParseInit(...)       _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)
#define DeeXAst_ParseInitSingle(...) _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|DEE_XAST_INITORDECL_FLAGS_SINGLEVAL,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)
#define DeeXAst_ParseInitTuple(...)  _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|DEE_XAST_INITORDECL_FLAGS_RETURNSEQ,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)
// Parse comma-separated expressions and Assign values to existing variables, or create new ones
#define DeeXAst_ParseDecl(...)       _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT|DEE_XAST_VARDECL_FLAG_ENABLED,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)
#define DeeXAst_ParseDeclSingle(...) _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT|DEE_XAST_VARDECL_FLAG_ENABLED,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|DEE_XAST_INITORDECL_FLAGS_SINGLEVAL,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)
#define DeeXAst_ParseDeclTuple(...)  _DeeXAst_ParseInitOrDecl(DEE_XAST_VARDECL_MODE_DEFAULT|DEE_XAST_VARDECL_FLAG_ENABLED,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|DEE_XAST_INITORDECL_FLAGS_RETURNSEQ,DEE_XASTKIND_TUPLE,NULL,NULL,__VA_ARGS__)

// Use these for parsing function arguments
#define DeeXAst_Parse       DeeXAst_ParseInit
#define DeeXAst_ParseSingle DeeXAst_ParseInitSingle
#define DeeXAst_ParseTuple  DeeXAst_ParseInitTuple


// Behavior of 'flags' is identical to those in '_DeeXAst_ParseInitOrDecl'
// in "xast_parse.named_type_suffix.inl": float x; int (*foo)(float);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffix(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeXAstObject *base, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffixWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeXAstObject *base,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantNamedTypeSuffixWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeXAstObject *base,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffixWithCopyAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeXAstObject *base,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllNamedTypeSuffix(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_IN DeeXAstKind seq_kind, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllNamedTypeSuffixWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_IN DeeXAstKind seq_kind,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);

// Same as the Suffix versions below, but parses the prefix as well
// e.g.: 'none (*foo)(float x, float y)'
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedType(
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeWithAttribute(
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"

// Returns the type branch and stores the name token or NULL for unnamed types in 'name_token'
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffix(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffixWithAttribute(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffixWithAttributeCopy(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_A_INOUT struct DeeAttributeDecl const *attr,
 DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"


//////////////////////////////////////////////////////////////////////////
// Internal named type suffix parser
// Since the order of post modifiers has to be reversed,
// we need to store them temporarily to apply them later
#define DEE_XASTTYPEOPERATION_KIND_NONE             0
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_XASTTYPEOPERATION_KIND_LVALUE           1 // x &
#define DEE_XASTTYPEOPERATION_KIND_POINTER          2 // x *
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#define DEE_XASTTYPEOPERATION_KIND_ARRAY            3 // x[42]
#define DEE_XASTTYPEOPERATION_KIND_VARARRAY         4 // x[]
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION 5 // x(int,long,float)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
struct DeeXAstTypeOperationsArrayEntry {
 DEE_A_REF DeeXAstObject *a_size; /*< [1..1] The size of the array. */
};
#define _DeeXAstTypeOperationsArrayEntry_Quit(ob)\
do{ Dee_DECREF((ob)->a_size); }while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
struct DeeXAstTypeOperationsForeignFunctionArgument {
 Dee_uint32_t              ffa_varflags; /*< Variable flags (set of 'DEE_LOCALVAR_FLAG_*'). */
 DEE_A_REF DeeXAstObject  *ffa_type;     /*< [1..1] Argument type. */
 DEE_A_REF DeeTokenObject *ffa_name;     /*< [0..1] Argument name (if given). */
};
#define _DeeXAstTypeOperationsForeignFunctionArgument_Quit(ob)\
do{ Dee_DECREF((ob)->ffa_type); Dee_XDECREF((ob)->ffa_name); }while(0)
struct DeeXAstTypeOperationsForeignFunctionEntry {
 DeeFunctionFlags                                     ff_flags; /*< Special function flags (set through attributes). */
 Dee_size_t                                           ff_argc;  /*< Argument count. */
 struct DeeXAstTypeOperationsForeignFunctionArgument *ff_argv;  /*< [0..ff_argc] Argument vector. */
};
#define _DeeXAstTypeOperationsForeignFunctionEntry_Quit(ob)\
do{\
 struct DeeXAstTypeOperationsForeignFunctionArgument *arg_iter,*arg_end;\
 arg_end = (arg_iter = (ob)->ff_argv)+(ob)->ff_argc;\
 while (arg_iter != arg_end) { _DeeXAstTypeOperationsForeignFunctionArgument_Quit(arg_iter); ++arg_iter; }\
 free((ob)->ff_argv);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS || \
    DEE_CONFIG_LANGUAGE_HAVE_ARRAYS || \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeXAstTypeOperationsEntry {
 DEE_A_REF DeeTokenObject *oe_token; /*< [1..1] Token describing this operation. */
 Dee_uint16_t              oe_kind;  /*< The kind of entry, as listed by 'DEE_XASTTYPEOPERATION_KIND_*' */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS || \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
 union {
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  struct DeeXAstTypeOperationsArrayEntry           oe_array;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  struct DeeXAstTypeOperationsForeignFunctionEntry oe_foreign_function;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
 };
#endif /* ... */
};
DEE_COMPILER_MSVC_WARNING_POP
#endif /* ... */

#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS && \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define _DeeXAstTypeOperationsEntry_Quit(ob)\
do{\
 Dee_DECREF((ob)->oe_token);\
 switch ((ob)->oe_kind) {\
  case DEE_XASTTYPEOPERATION_KIND_ARRAY:\
   _DeeXAstTypeOperationsArrayEntry_Quit(&(ob)->oe_array);\
   break;\
  case DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION:\
   _DeeXAstTypeOperationsForeignFunctionEntry_Quit(&(ob)->oe_foreign_function);\
   break;\
  default: break;\
 }\
}while(0)
#elif DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
#define _DeeXAstTypeOperationsEntry_Quit(ob)\
do{\
 Dee_DECREF((ob)->oe_token);\
 if ((ob)->oe_kind == DEE_XASTTYPEOPERATION_KIND_ARRAY) {\
  _DeeXAstTypeOperationsArrayEntry_Quit(&(ob)->oe_array);\
 }\
}while(0)
#elif DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define _DeeXAstTypeOperationsEntry_Quit(ob)\
do{\
 Dee_DECREF((ob)->oe_token);\
 if ((ob)->oe_kind == DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION) {\
  _DeeXAstTypeOperationsForeignFunctionEntry_Quit(&(ob)->oe_foreign_function);\
 }\
}while(0)
#elif 0
#define _DeeXAstTypeOperationsEntry_Quit(ob)\
do{ Dee_DECREF((ob)->oe_token); }while(0)
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS || \
    DEE_CONFIG_LANGUAGE_HAVE_ARRAYS || \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS 1
struct DeeXAstTypeOperations {
 Dee_size_t                         o_opsa;
 Dee_size_t                         o_opsc;
 struct DeeXAstTypeOperationsEntry *o_opsv;
};
#define DeeXAstTypeOperations_INIT() {0,0,NULL}
#define DeeXAstTypeOperations_SIZE(ob) (ob)->o_opsc
#define _DeeXAstTypeOperations_Quit(ob)\
do{\
 struct DeeXAstTypeOperationsEntry *ops_iter,*ops_end;\
 ops_end = (ops_iter = (ob)->o_opsv)+(ob)->o_opsc;\
 while (ops_iter != ops_end) { _DeeXAstTypeOperationsEntry_Quit(ops_iter); ++ops_iter; }\
 free((ob)->o_opsv);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// execute the given type operations, generating the required AST transformations
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAstTypeOperations_Exec(
 DEE_A_IN struct DeeXAstTypeOperations const *self, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags);

// Appends an uninitialized entry and returns it
extern DEE_A_RET_EXCEPT(NULL) struct DeeXAstTypeOperationsEntry *
 DeeXAstTypeOperations_AppendNew(DEE_A_INOUT struct DeeXAstTypeOperations *self);
// Removes an appended entry returned by 'DeeXAstTypeOperations_AppendNew',
// after its initialization failed
#define DeeXAstTypeOperations_AppendFree(ob) ((void)--(ob)->o_opsc)

// Inserts an uninitialized entry at position 'pos' and returns it
extern DEE_A_RET_EXCEPT(NULL) struct DeeXAstTypeOperationsEntry *
 DeeXAstTypeOperations_InsertNew(DEE_A_INOUT struct DeeXAstTypeOperations *self, DEE_A_IN Dee_size_t pos);
// Removes an inserted entry returned by 'DeeXAstTypeOperations_InsertNew',
// after its initialization failed
extern void DeeXAstTypeOperations_InsertFree(
 DEE_A_INOUT struct DeeXAstTypeOperations *self, DEE_A_IN Dee_size_t pos);
#endif /* ... */

#ifndef DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
#define DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS 0
#endif


#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
//////////////////////////////////////////////////////////////////////////
// Parses a foreign function closure block:
//   base   post_operations
//     |       |
//    vvv     vvvvvvvvvvvv  vvvvvvvvvvvvvvv
// >> int add(int x, int y) { return x+y; }
//                                        ^
// Returns the ff-closure declaration AST that will be assign to 'add' in the above example
// Note, that the caller is responsible for setting 'lexer->ob_func_name' to 'add'
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseForeignFunctionClosureCode(
 DEE_A_INOUT DeeXAstObject *base, DEE_A_INOUT struct DeeXAstTypeOperations *post_operations,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_A_INOUT_OPT DeeObject *function_name,
 DEE_PARSER_PARAMS); // in "xast_parse.foreign_function_closure.inl"
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */


#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS || \
    DEE_CONFIG_LANGUAGE_HAVE_ARRAYS || \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
extern DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalDoParseNamedTypeSuffix( // in "xast_parse.named_type_suffix.inl"
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr,  DEE_A_INOUT struct DeeXAstTypeOperations *post_operations, DEE_PARSER_PARAMS);
#else /* ... */
extern DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalDoParseNamedTypeSuffix( // in "xast_parse.named_type_suffix.inl"
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr,  DEE_PARSER_PARAMS);
#endif /* ... */

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
// v initializes the 'ff_argc' and 'ff_argv' members of 'ff'
extern DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalParseForeignFunctionArguments(
 DEE_A_INOUT struct DeeXAstTypeOperationsForeignFunctionEntry *ff, DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"
extern DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalParseForeignFunctionArgument(
 DEE_A_INOUT struct DeeXAstTypeOperationsForeignFunctionArgument *arg, DEE_PARSER_PARAMS); // in "xast_parse.named_type_suffix.inl"
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */


//////////////////////////////////////////////////////////////////////////
// Parses the contents of a __builtin_exists expression
// >>                  vvvvvv
// >> __builtin_exists(foobar);
// >>                        v
// in "xast_parse.exists.inl"
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionUnary(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionUnaryWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLand(DEE_PARSER_PARAMS); // x && y;
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLxor(DEE_PARSER_PARAMS); // x ^^ y;
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLor(DEE_PARSER_PARAMS); // x || y;
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionIf(DEE_PARSER_PARAMS); // x ? y : z;
#define DeeXAst_ParseExistsExpression DeeXAst_ParseExistsExpressionIf

//////////////////////////////////////////////////////////////////////////
// Parses the arguments of a __builtin_types_compatible_p expression
// >>                              vvvvvvvv
// >> __builtin_types_compatible_p(int,long);
// >>                                      v
// in "xast_parse.types_compatible_p.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseTypesCompatibleP(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_PARSER_PARAMS);


//////////////////////////////////////////////////////////////////////////
// Parses the second argument of a __builtin_offsetof expression
// >>                               vvvvvvvvvvvvvvvv
// >> __builtin_offsetof(struct foo,bar.foobar[10].x);
// >>                                               v
// in "xast_parse.offsetof.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOffsetofSuffix(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_A_IN DeeTypeObject const *base, DEE_PARSER_PARAMS);


//////////////////////////////////////////////////////////////////////////
// Parses the contents of a __builtin_bound expression
// >>                 vvvvvvvvvvvv
// >> __builtin_bound(local foobar);
// >>                             v
// in "xast_parse.c"
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseBuiltinBoundExpression(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_PARSER_PARAMS);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */


//////////////////////////////////////////////////////////////////////////
// Looks ahead, staring after an lparen, checking for a recursive C-Type:
// >>       vvvvvvv
// >> none (*foobar)(void)
// >>       v
//  - Always returns to the token it started at
//  - Returns -1 on Error
//  - Returns 0 if the lexer is sitting on a recursive c-type
//  - Returns 1 if the lexer isn't sitting on a recursive c-type
// NOTE: In the above example, the indication to return '0' is the fact that 'foobar' doesn't exist / isn't initialized
//////////////////////////////////////////////////////////////////////////
// Recursive c types require the var declaration privilege
// >> local x;
// >> none (*x)(int,float,long); // Not a C-type (variable already existed)
// >> none (*y)(int,float,long); // C-type
// >> none (*y)(int,float,long); // Not a C-type (variable already existed)
// >> print none (*x)(int,float,long); // Not a C-type (variable already existed)
// >> print none (*a)(int,float,long); // Not a C-type (missing right to declare variables)
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeXAst_ParseLookAheadForRecursiveCType(DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses an import statement
// >>        vvvvvvvv
// >> import foo::bar,foo::baz;
// >>                ^
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseImport(
 DEE_PARSER_PARAMS); // in xast_parse.import.inl
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseImportWithAttribute(
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in xast_parse.import.inl
//////////////////////////////////////////////////////////////////////////
// Parses an import statement
// >>        vvvvvvvvvvvvvvvvv
// >> import foo::bar,foo::baz;
// >>                         ^
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllImports(
 DEE_A_IN DeeTokenObject *import_token, DEE_PARSER_PARAMS); // in xast_parse.import.inl


//////////////////////////////////////////////////////////////////////////
// Parses an del expression
// >>     vvv
// >> del foo,bar;
// >>        ^
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseDel(
 DEE_PARSER_PARAMS); // in xast_parse.del.inl
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseDelWithAttribute(
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS); // in xast_parse.del.inl
//////////////////////////////////////////////////////////////////////////
// Parses a tuple of del expression
// >>     vvvvvvv
// >> del foo,bar;
// >>            ^
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllDel(
 DEE_A_IN DeeTokenObject *del_token, DEE_PARSER_PARAMS); // in xast_parse.del.inl

extern DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_ParseConst(
 DEE_A_REF DEE_A_OUT_OPT DeeTokenObject **const_token, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Custom component parsing
extern DEE_A_RET_EXCEPT(-1) int DeeXAstFunctionAstArgument_Parse(
 struct DeeXAstFunctionAstArgument *self, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT(-1) int DeeXAstDictEntry_Parse(
 struct DeeXAstDictEntry *self, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses the argument portion of an operator call:
//                                 vvvv
// >> $ast_this.operator $typeslot (42)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseOperatorCall(
 DEE_A_INOUT DeeTokenObject *operator_token,
 DEE_A_INOUT DeeXAstObject *ast_this, DEE_A_IN int typeslot, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses an expression-style switch block:
//                            vvvvvvvvvvvvvvvvvvv
// >> switch (&switch_expr) { 42: 10, default: 20 }
// in "xast_parse.switch_block.inl"
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSwitchBlock(
 DEE_A_INOUT DeeTokenObject *switch_token, DEE_A_IN DeeXAstObject *switch_expr, DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Parses the operator name portion of an operator call:
//                 v
// >> foo.operator + (42)
//////////////////////////////////////////////////////////////////////////
// Parses a operator name, returning its typeslot name
//   - Returns -1 on error
//   - Returned value can be turned into a string using '_DeeType_ClassOperatorName'
//   - Returns '0' for unknown operators
// e.g.: 'DeeXAstOperatorName_Parse("__add__") >> tp_add'
// NOTE: Some tokens return special operator names, where the caller
//       is responsible for determining the correct operator based
//       on the arguments provided:
//   INPUT            | RESULT                        | MEANING
// +------------------+-------------------------------+-----------
//  "+"               | tp_p_add                      | tp_add, tp_pos
//  "-"               | tp_p_sub                      | tp_sub, tp_neg
//  "*"               | tp_p_mul                      | tp_mul, /* deref */
//  "&"               | tp_p_and                      | tp_and, /* ref */
//  "for"             | DEE_CLASS_SLOTID_OPERATOR_FOR | calling it twice returns an iterator
//  "__ref__"         | DEE_XAST_TYPESLOT_REF         | /* ref */
//  "__deref__"       | DEE_XAST_TYPESLOT_DEREF       | /* deref */
//  "[]"              | tp_p_seq_get                  | tp_seq_get, tp_seq_range_get
//  "del []"          | tp_p_seq_del                  | tp_seq_del, tp_seq_range_del
//  "[] ="            | tp_p_seq_set                  | tp_seq_set, tp_seq_range_set
//  "__constructor__" | tp_p_any_ctor                 | tp_ctor, tp_any_ctor
//  "none"            | DEE_XAST_TYPESLOT_NONE        | /* nothing */
//  "__hasattr__"     | DEE_XAST_TYPESLOT_ATTR_HAS    | DeeObject_HasAttr
//  "__read__"        | DEE_XAST_TYPESLOT_IO_READ     | tp_io_read, DEE_CLASS_SLOTID_OPERATOR_READNP
//  "__write__"       | DEE_XAST_TYPESLOT_IO_WRITE    | tp_io_write, DEE_CLASS_SLOTID_OPERATOR_WRITENP
// +------------------+-------------------------------+-----------
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_XAST_TYPESLOT_REF      0x70000001
#define DEE_XAST_TYPESLOT_DEREF    0x70000002
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#define DEE_XAST_TYPESLOT_NONE     0x70000003
#define DEE_XAST_TYPESLOT_ATTR_HAS 0x70000004
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_XAST_TYPESLOT_IO_READ  0x70000005
#define DEE_XAST_TYPESLOT_IO_WRITE 0x70000006
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
extern DEE_A_RET_EXCEPT(-1) int DeeXAstOperatorName_Parse(DEE_PARSER_PARAMS);

//////////////////////////////////////////////////////////////////////////
// Fix operator names with a given argument count
// -> Returns 1 and emits a warning if no operator overload can handle 'argc'
// NOTE: Also checks 'argc' for correctness of non-overloaded
//       operators, returning 1 if it isn't correct.
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeXAst_FixOperatorWithArgcount(
 DEE_A_IN DeeLexerObject *lexer, DEE_A_IN DeeTokenObject *error_token,
 DEE_A_INOUT int *operator_name, DEE_A_IN Dee_size_t argc);


//////////////////////////////////////////////////////////////////////////
// Parses a generic range initializer
// NOTE: 'seq_kind' is only meant as a hint, and if the range is actually
//       a dict, the returned AST will not adhere to 'seq_kind'.
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRangeInitializer(
 DEE_A_INOUT DeeTokenObject *range_token, DEE_A_IN DeeXAstKind seq_kind, DEE_PARSER_PARAMS);
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseRangeInitializerWithType(
 DEE_A_INOUT DeeXAstObject *range_type, DEE_PARSER_PARAMS);

#define DEE_PRIVATE_XAST_TOKENS_IF_0(...)
#define DEE_PRIVATE_XAST_TOKENS_IF_1(...) __VA_ARGS__
#define DEE_PRIVATE_XAST_TOKENS_IF(x) DEE_PRIVATE_XAST_TOKENS_IF_##x
#define DEE_XAST_TOKENS_IF(cond,...) DEE_PRIVATE_XAST_TOKENS_IF(cond)(__VA_ARGS__)


// v List of tokens that can be found in a unary suffix expression
//   (blacklist of keywords that will prevent a cast operation)
//   NOTE: Don't add tokens that can also appear at the start of a unary expression
#define DEE_XAST_UNARY_SUFFIX_TOKENS \
 case KWD_in: case KWD_is:


// v this list of tokens is not just for optimization, but
//   is required for detecting recursive c types.
//   Every builtin keyword that can be written at the start
//   of any unary expression must be present in here (excluding variable modifiers)
//   NOTE: (some) non-keyword tokens are allowed, but not necessary
#define DEE_XAST_UNARY_TOKENS_NOSTMT_NOCLASSOPS \
 case KWD___builtin_object: case KWD___builtin_dex:\
 case KWD_object: case KWD_dict: case KWD_list: case KWD_set:\
 case KWD_tuple: case KWD_true: case KWD_false: DEE_PARSE_TOKENID_CASE_NONE\
 case TPP_TOK_INT: case TPP_TOK_CHR: case TPP_TOK_FLOAT: case TPP_TOK_STR:\
 case KWD___FUNCTION__: case KWD___func__: case KWD_pack: case KWD_str: case KWD_repr:\
 case KWD_type: case KWD_weak: case KWD_super: case KWD___super:\
 case KWD_long: case KWD_char: case KWD_short: case KWD_int:\
 DEE_PARSE_TOKENID_CASE_WCHAR_T DEE_PARSE_TOKENID_CASE_CHAR16_T\
 DEE_PARSE_TOKENID_CASE_CHAR32_T DEE_PARSE_TOKENID_CASE_ATOMIC DEE_PARSE_TOKENID_CASE_SIGNED\
 DEE_PARSE_TOKENID_CASE_UNSIGNED DEE_PARSE_TOKENID_CASE_UINT8 DEE_PARSE_TOKENID_CASE_UINT16\
 DEE_PARSE_TOKENID_CASE_UINT32 DEE_PARSE_TOKENID_CASE_UINT64 DEE_PARSE_TOKENID_CASE_INT8\
 DEE_PARSE_TOKENID_CASE_INT16 DEE_PARSE_TOKENID_CASE_INT32 DEE_PARSE_TOKENID_CASE_INT64\
 DEE_PARSE_TOKENID_CASE_BOOL case KWD_float: case KWD_double:case KWD_string:\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP,case KWD___noop:case KWD___builtin_noop:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNUSED,case KWD___builtin_unused:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEAD,case KWD___builtin_dead:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P,case KWD___builtin_constant_p:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEXCEPT,case KWD___builtin_noexcept:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NORETURN,case KWD___builtin_noreturn:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOEFFECT,case KWD___builtin_noeffect:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR,case KWD___builtin_choose_expr:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE,case KWD___builtin_unreachable:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPES_COMPATIBLE_P,case KWD___builtin_types_compatible_p:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT,case KWD___builtin_breakpoint:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_TYPEOF,case KWD___builtin_typeof:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP,case KWD___builtin_help:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF,case KWD___builtin_offsetof:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS,case KWD___builtin_exists:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT,case KWD___builtin_expect:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_PREDICTABLE,case KWD___builtin_predictable:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPRAST,case KWD___builtin_exprast:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND,case KWD___builtin_bound:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA,case KWD___builtin_alloca:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE,case KWD_new:case KWD_delete:)\

#define DEE_XAST_UNARY_TOKENS_NOSTMT \
 case KWD_copy: case KWD_move: case KWD_operator: \
 DEE_XAST_UNARY_TOKENS_NOSTMT_NOCLASSOPS

#define DEE_XAST_UNARY_TOKENS \
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION,case KWD_if:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_SWITCH_EXPRESSION,case KWD_switch:)\
 DEE_XAST_TOKENS_IF(DEE_CONFIG_LANGUAGE_HAVE_GENERATORS,\
  case KWD_do: case KWD_while: case KWD_for:\
  case KWD___foreach: case KWD___looponce:\
  case KWD___loopever: case KWD___loopnone:)\
 DEE_XAST_UNARY_TOKENS_NOSTMT

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_PRIVATE_XAST_UNARY_POINTER_TOKENS \
 case '*': case '&': case TPP_TOK_LAND:\
 case TPP_TOK_BIN_AND_ASSIGN: case TPP_TOK_POW:\
 case TPP_TOK_POW_ASSIGN:
#else
#define DEE_PRIVATE_XAST_UNARY_POINTER_TOKENS 
#endif
#define DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS_NOATTR \
 DEE_XAST_UNARY_TOKENS DEE_PRIVATE_XAST_UNARY_POINTER_TOKENS \
 case '(': case '!': case '#': case '~': case '<': case TPP_TOK_SHL: \
 case TPP_TOK_INC: case TPP_TOK_DEC: case '+': case '-':

#define DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS \
 DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS_NOATTR case '[':


struct DeeOptimizerVarInitAssumption {
 DEE_A_REF DeeLocalVarObject *ovia_var;  /*< [1..1] The local_var object that we know the initialization of. */
 DEE_A_REF DeeObject         *ovia_init; /*< [1..1] The constant value used to initialize the variable. */
};
struct DeeOptimizerVarTypeAssumption {
 DEE_A_REF DeeLocalVarObject *ovta_var;  /*< [1..1] The local_var object that we know the initialization of. */
 DEE_A_REF DeeTypeObject     *ovta_type; /*< [1..1] The type used to initialize the variable. */
};
struct DeeOptimizerExprAssumption {
 DEE_A_REF DeeXAstObject     *oea_expr;  /*< [1..1] The expression assumed to be true. */
};
struct DeeOptimizerAssumption {
#define DEE_OPTIMIZER_ASSUMPTION_KIND_NONE    DEE_UINT32_C(0x00000000)
#define DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT DEE_UINT32_C(0x00000001) /*< Assume a given variable to be initialized with some constant. */
#define DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE DEE_UINT32_C(0x00000002) /*< Assume a given variable to be initialized from the instance of some type. */
#define DEE_OPTIMIZER_ASSUMPTION_KIND_EXPR    DEE_UINT32_C(0x00000003) /*< Assume some expression to be true. */
 Dee_uint32_t    oa_kind;  /*< The kind of assumption. */
 union{
  struct DeeOptimizerVarInitAssumption oa_varinit; /*< DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT. */
  struct DeeOptimizerVarTypeAssumption oa_vartype; /*< DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE. */
  struct DeeOptimizerExprAssumption    oa_expr;    /*< DEE_OPTIMIZER_ASSUMPTION_KIND_EXPR. */
 };
};
#if 1
#define _DeeOptimizerAssumption_Quit(ob)\
do{\
 switch ((ob)->oa_kind) {\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT:\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE:\
   Dee_DECREF((ob)->oa_varinit.ovia_var);\
   Dee_DECREF((ob)->oa_varinit.ovia_init);\
   break;\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_EXPR:\
   Dee_DECREF((ob)->oa_expr.oea_expr);\
   break;\
  default: break;\
 }\
}while(0)
#else
#define _DeeOptimizerAssumption_Quit(ob)\
do{\
 switch ((ob)->oa_kind) {\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT:\
   Dee_DECREF((ob)->oa_varinit.ovia_var);\
   Dee_DECREF((ob)->oa_varinit.ovia_init);\
   break;\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE:\
   Dee_DECREF((ob)->oa_vartype.ovta_var);\
   Dee_DECREF((ob)->oa_vartype.ovta_type);\
   break;\
  case DEE_OPTIMIZER_ASSUMPTION_KIND_EXPR:\
   Dee_DECREF((ob)->oa_expr.oea_expr);\
   break;\
  default: break;\
 }\
}while(0)
#endif

struct DeeOptimizerAssumptions {
 // v [0..1] List of additional assumptions that still apply,
 //          originating from an underlying (strong or weak) scope.
 struct DeeOptimizerAssumptions *oas_prev;
#define DEE_OPTIMIZER_ASSUMPTIONS_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG DEE_UINT32_C(0x00000001) /*< Assumptions originate from the the root of a strong scope. */
#define DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL  DEE_UINT32_C(0x00000002) /*< A label was encountered. */
 Dee_uint32_t                    oas_flags;       /*< Flags for this list of assumptions. */
 DeeScopeObject                 *oas_scope;       /*< [1..1] The top-most scope in which this assumption should apply (NOTE: This assumes that unnecessary scopes have been removed). */
 Dee_size_t                      oas_assumptionc; /*< The amount of assumptions made. */
 struct DeeOptimizerAssumption  *oas_assumptionv; /*< [0..oas_assumptionc] Vector of assumptions. */
};

//////////////////////////////////////////////////////////////////////////
// Add/Override the assumption of a variable being initialized to a given value
// in "xast.optimizer_assumptions.inl"
// NOTES:
//   - The variable must belong to the same strong scope as the list of assumptions
//   - If an assumption is contradicting a previous assumption, then
//     the previous assumption will either be overwritten (if it was made in the same scope)
//     or the previous assumption will be deleted and no new one will be added (if it was made is a previous scope).
//   - Calling this function after a label was encountered within the same strong scope results in a no-op
//   - if 'local_var' is marked as volatile, calling this function is a no-op
//   - Assuming the value of a variable requires that value's type to be immutable
#define DeeOptimizerAssumptions_AddAssumeInit(ob,local_var,init_val)\
 DeeOptimizerAssumptions_AddVarAssumption(ob,local_var,\
  DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT,init_val)
#define DeeOptimizerAssumptions_AddAssumeType(ob,local_var,init_tp)\
 DeeOptimizerAssumptions_AddVarAssumption(ob,local_var,\
  DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE,(DeeObject *)Dee_REQUIRES_POINTER(init_tp))
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ERROR    (-1) /*< Error occurred during addition of assumption. */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP       0  /*< The assumption could not be made and the call was a no-op. */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_MATCHOLD   1  /*< The assumption matched an existing assumption. */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_OVERRIDE   2  /*< An existing assumption was overwritten with the new value. */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_DEGRADE    3  /*< An existing assumption was degraded (init --> type). */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_UPGRADE    4  /*< An existing assumption was upgraded (type --> init). */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_REPLACE    5  /*< An existing assumption was replaced. */
#define DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED      6  /*< Assumption was added normally. */
extern DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_AddVarAssumption(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self, DEE_A_INOUT DeeLocalVarObject *local_var,
 DEE_A_IN Dee_uint32_t kind, DEE_A_INOUT DeeObject *arg) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Returns the initialization value/type currently assumed for a given variable.
// in "xast.optimizer_assumptions.inl"
// NOTES:
//   - Returns NULL if no explicit value/type is being assumed
extern DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeOptimizerAssumptions_GetAssumedVarInit(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *DeeOptimizerAssumptions_GetAssumedVarType(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeOptimizerAssumptions_IsVarBound(ob,local_var) \
 (DeeOptimizerAssumptions_GetAssumedVarType(ob,local_var)!=NULL)

//////////////////////////////////////////////////////////////////////////
// Delete the assumption of what a given variable is initialized as.
// in "xast.optimizer_assumptions.inl"
// NOTES:
//   - Assumptions can be deleted anywhere inside the current string scope
//   - This function can still be used after a label was encountered.
//   - This function is called when a local variable is explicitly deleted.
//   - Returns >0 if no assumption was made about the variable.
extern DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_DelVarAssumptions(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Assume a given expression to be true.
// in "xast.optimizer_assumptions.inl"
// NOTES:
//   - If the expression evaluates to a constant false, undefined behavior is invoked.
//   - If the expression contradicts a previous assumptions, undefined behavior is invoked.
//   - Calling this function after a label/goto was encountered results in a no-op
extern DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_AssumeExpr(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeXAstObject *expr) DEE_ATTRIBUTE_NONNULL((1,2));

#define _DeeOptimizerAssumptions_EncounterSAst(ob,sast,on_err)\
do{\
 if (!DeeParserLabelRefList_EMPTY(&(sast)->ast_common.ast_labels) || \
     (sast)->ast_kind == DEE_SASTKIND_GOTO) {\
  /* Disable assumptions as soon as a label/goto is encountered. */\
  /* NOTE: In the future, we might be able to diffuse what exactly a \
           label/goto is doing, allowing us to keep assumptions enabled, \
           but for now, we stick to what we know and try to leave labels alone... */\
  struct DeeOptimizerAssumptions *weak_root;\
  weak_root = (ob);\
  while ((weak_root->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG)==0) {\
   if ((weak_root->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)!=0) break;\
   weak_root->oas_flags |= DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL;\
   DEE_ASSERTF(weak_root->oas_prev,"Failed to find weak root");\
   weak_root = weak_root->oas_prev;\
  }\
 }\
}while(0)
#define _DeeOptimizerAssumptions_EncounterXAst(ob,xast,on_err)\
do{}while(0)

#define _DeeOptimizerAssumptions_InitRoot(ob,scope)\
do{\
 (ob)->oas_prev = NULL;\
 (ob)->oas_flags = DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG;\
 (ob)->oas_scope = (scope);\
 (ob)->oas_assumptionc = 0;\
 (ob)->oas_assumptionv = NULL;\
}while(0)

#define _DeeOptimizerAssumptions_Quit(ob)\
do{\
 struct DeeOptimizerAssumption *_o_iter,*_o_end;\
 _o_end = (_o_iter = (ob)->oas_assumptionv)+(ob)->oas_assumptionc;\
 while (_o_iter != _o_end) { _DeeOptimizerAssumption_Quit(_o_iter); ++_o_iter; }\
 free((ob)->oas_assumptionv);\
}while(0)

#define DEE_OPTIMIZE_SCOPE_ENTER(name,scope,assumptions)\
 DEE_OPTIMIZE_SCOPE_ENTER_EX(name,scope,assumptions,DEE_OPTIMIZER_ASSUMPTIONS_FLAG_NONE)

#define DEE_OPTIMIZE_SCOPE_ENTER_EX(name,scope,assumptions,flags)\
do{\
 struct DeeOptimizerAssumptions name;\
 DEE_ASSERT(assumptions);\
 DEE_ASSERTF(DeeScope_SameWeakScope(\
  (DeeObject *)(scope),(DeeObject *)(assumptions)->oas_scope) ||\
  (scope)->sc_prev == (assumptions)->oas_scope,"Invalid scope");\
 name.oas_prev = (assumptions);\
 name.oas_scope = (scope);\
 name.oas_flags = (flags);\
 name.oas_assumptionc = 0;\
 name.oas_assumptionv = NULL;\
 if(0);else
#define DEE_OPTIMIZE_SCOPE_BREAK(name)\
do{ _DeeOptimizerAssumptions_Quit(&name); }while(0)
#define DEE_OPTIMIZE_SCOPE_LEAVE(name)\
  _DeeOptimizerAssumptions_Quit(&name);\
}while(0)


#ifndef DEE_OPTIMIZE_PARAMS
#define DEE_OPTIMIZE_PARAMS \
 DEE_A_INOUT Dee_size_t *performed_optimizations,\
 DEE_A_INOUT struct DeeLexerObject *lexer, DEE_A_INOUT struct DeeScopeObject *scope,\
 DEE_A_IN struct DeeCompilerConfig const *config, DEE_A_IN Dee_uint32_t optimize_flags,\
 DEE_A_IN struct DeeOptimizerAssumptions *assumptions
#define DEE_OPTIMIZE_ARGS                                            performed_optimizations,lexer,scope,config,optimize_flags,assumptions
#define DEE_OPTIMIZE_ARGS_SCOPE(scope,assumptions)                   performed_optimizations,lexer,scope,config,optimize_flags,assumptions
#define DEE_OPTIMIZE_ARGS_EX(optimize_flags)                         performed_optimizations,lexer,scope,config,optimize_flags,assumptions
#define DEE_OPTIMIZE_ARGS_SCOPE_EX(scope,assumptions,optimize_flags) performed_optimizations,lexer,scope,config,optimize_flags,assumptions
#endif

//////////////////////////////////////////////////////////////////////////
// Optimizer
// WARNING: For performance, this function is allowed to modify 'self'
// NOTE: Some optimizations will not work if specific parser flags weren't set during parsing
// in: "xast_optimize.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeXAst_Optimize(
 DEE_A_INOUT DeeXAstObject *self, DEE_OPTIMIZE_PARAMS);

#define DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_NONE     0x00000000
#define DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_OPTIONAL 0x00000001 /*< Set of the variable storage can always be removed if it is unused. */
extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_Optimize(
 DEE_A_INOUT struct DeeVarDeclStorage *self,
 DEE_A_IN Dee_uint32_t flags, DEE_OPTIMIZE_PARAMS);


#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_PRIVATE_XAST_RESERVED_KEYWORD_FOREIGNFUNCTION_TOKENS
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DEE_PRIVATE_XAST_RESERVED_KEYWORD_FOREIGNFUNCTION_TOKENS \
 case KWD__syssv: case KWD___syssv: case KWD__stdcall: case KWD___stdcall:\
 case KWD__thiscall: case KWD___thiscall: case KWD__fastcall: case KWD___fastcall:\
 case KWD__cdecl: case KWD___cdecl: case KWD__pascal: case KWD___pascal:\
 case KWD__register: case KWD___register: case KWD__win64: case KWD___win64:\
 case KWD__unix64: case KWD___unix64:
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

#define DEE_XAST_RESERVED_KEYWORD_TOKENS \
 DEE_PRIVATE_XAST_RESERVED_KEYWORD_FOREIGNFUNCTION_TOKENS \
 case KWD__Alignof: case KWD__Thread_local: case KWD__Complex:\
 case KWD__Imaginary: case KWD_auto: case KWD_const_cast:\
 case KWD_decltype: case KWD_dynamic_cast: case KWD_explicit:\
 case KWD_inline: case KWD_mutable: case KWD_override:\
 case KWD_reinterpret_cast: case KWD_sizeof: case KWD_static_cast:\
 case KWD_throws: case KWD_protected: case KWD_private: case KWD_public:

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#define DEE_PRIVATE_AST_RESERVED_OPERATOR_POINTER_TOKENS
#else
#define DEE_PRIVATE_AST_RESERVED_OPERATOR_POINTER_TOKENS case KWD___ref__: case KWD___deref__:
#endif
#define DEE_PRIVATE_AST_RESERVED_OPERATOR_TOKENS \
 DEE_PRIVATE_AST_RESERVED_OPERATOR_POINTER_TOKENS

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
#undef DEE_PRIVATE_AST_RESERVED_OPERATOR_TOKENS
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_XAST_H */
