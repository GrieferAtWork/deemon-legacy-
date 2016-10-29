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
#ifndef GUARD_DEEMON_EXPR_AST_H
#define GUARD_DEEMON_EXPR_AST_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#ifndef DEE_LIMITED_API
#error "Limited-only header"
#endif

#include <deemon/__conf.inl>
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeExprAstObject);

//////////////////////////////////////////////////////////////////////////
// Notes about the assignment operator '='
// 
// The meaning of the '=' has a very wide range:
// 
// >> function get;
// >> a = 21;      // store_or_assign (store)
// >> a = 21;      // store_or_assign (assign)
// >> get() = 21;  // assign
// >> a.b = 21;    // set_attr
// >> a[1] = 21;   // set_item
// 
// If you need to call the assign operator, use ':='
// 
// >> function get;
// >> a := 21;      // assign (with )
// >> a := 21;      // assign
// >> get() := 21;  // assign
// >> a.b := 21;    // set_attr
// >> a[1] := 21;   // set_item


typedef int DeeExprAstKind; enum{
   DeeExprAstKind_MASK_OPCOUNT       = 0xF0000000, // Mask for operator count
   DeeExprAstKind_MODE_CONST         = 0x00000000, // 10, 'foo', none
   DeeExprAstKind_MODE_UNARY         = 0x10000000, // !a, ~a, ...
   DeeExprAstKind_MODE_BINARY        = 0x20000000, // a+b, a-b, ...
   DeeExprAstKind_MODE_TRINARY       = 0x40000000, // a[b:c], a[b]=c, ...
   DeeExprAstKind_MODE_QUADARY       = 0x80000000, // a[b:c]=d
   DeeExprAstKind_MODE_N_NARY        = 0xF0000000, // a(b,c,d,e,...)
   DeeExprAstKind_MODULE             = DeeExprAstKind_MODE_CONST  |0x00000000, // reference to module
   DeeExprAstKind_VARIABLE           = DeeExprAstKind_MODE_CONST  |0x00000001, // <keyword>
   DeeExprAstKind_CONSTANT           = DeeExprAstKind_MODE_CONST  |0x00000002, // <number> / <string> / <none>
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
   DeeExprAstKind_CODE               = DeeExprAstKind_MODE_UNARY  |0x00000003, // '(' '{' <statements> '}' ')'
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
   DeeExprAstKind_VARDECL            = DeeExprAstKind_MODE_BINARY |0x00000004, // <keyword> '=' <expr>
   DeeExprAstKind_COND               = DeeExprAstKind_MODE_TRINARY|0x00000005, // <expr> '?' <expr> ':' <expr>
   DeeExprAstKind_ASSIGN             = DeeExprAstKind_MODE_BINARY |0x00000006, // <expr> ':=' <expr>
   DeeExprAstKind_NOT                = DeeExprAstKind_MODE_UNARY  |0x00000007, // '!' <expr>
   DeeExprAstKind_NEG                = DeeExprAstKind_MODE_UNARY  |0x00000008, // '-' <expr>
   DeeExprAstKind_POS                = DeeExprAstKind_MODE_UNARY  |0x00000009, // '+' <expr>
   DeeExprAstKind_INV                = DeeExprAstKind_MODE_UNARY  |0x0000000A, // '~' <expr>
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
   DeeExprAstKind_REF                = DeeExprAstKind_MODE_UNARY  |0x0000000B, // '&' <expr>
   DeeExprAstKind_DEREF              = DeeExprAstKind_MODE_UNARY  |0x0000000C, // '*' <expr>
#endif
   DeeExprAstKind_INC                = DeeExprAstKind_MODE_UNARY  |0x0000000D, // '++' <expr>
   DeeExprAstKind_DEC                = DeeExprAstKind_MODE_UNARY  |0x0000000E, // '--' <expr>
   DeeExprAstKind_INC_POST           = DeeExprAstKind_MODE_UNARY  |0x0000000F, // <expr> '++'
   DeeExprAstKind_DEC_POST           = DeeExprAstKind_MODE_UNARY  |0x00000010, // <expr> '--'
   DeeExprAstKind_CMP_LO             = DeeExprAstKind_MODE_BINARY |0x00000011, // <expr> '<' <expr>
   DeeExprAstKind_CMP_LE             = DeeExprAstKind_MODE_BINARY |0x00000012, // <expr> '<=' <expr>
   DeeExprAstKind_CMP_EQ             = DeeExprAstKind_MODE_BINARY |0x00000013, // <expr> '==' <expr>
   DeeExprAstKind_CMP_NE             = DeeExprAstKind_MODE_BINARY |0x00000014, // <expr> '!=' <expr>
   DeeExprAstKind_CMP_GR             = DeeExprAstKind_MODE_BINARY |0x00000015, // <expr> '>' <expr>
   DeeExprAstKind_CMP_GE             = DeeExprAstKind_MODE_BINARY |0x00000016, // <expr> '>=' <expr>
   DeeExprAstKind_ADD                = DeeExprAstKind_MODE_BINARY |0x00000017, // <expr> '+' <expr>
   DeeExprAstKind_IADD               = DeeExprAstKind_MODE_BINARY |0x00000018, // <expr> '+=' <expr>
   DeeExprAstKind_SUB                = DeeExprAstKind_MODE_BINARY |0x00000019, // <expr> '-' <expr>
   DeeExprAstKind_ISUB               = DeeExprAstKind_MODE_BINARY |0x0000001A, // <expr> '-=' <expr>
   DeeExprAstKind_MUL                = DeeExprAstKind_MODE_BINARY |0x0000001B, // <expr> '*' <expr>
   DeeExprAstKind_IMUL               = DeeExprAstKind_MODE_BINARY |0x0000001C, // <expr> '*=' <expr>
   DeeExprAstKind_DIV                = DeeExprAstKind_MODE_BINARY |0x0000001D, // <expr> '/' <expr>
   DeeExprAstKind_IDIV               = DeeExprAstKind_MODE_BINARY |0x0000001E, // <expr> '/=' <expr>
   DeeExprAstKind_MOD                = DeeExprAstKind_MODE_BINARY |0x0000001F, // <expr> '%' <expr>
   DeeExprAstKind_IMOD               = DeeExprAstKind_MODE_BINARY |0x00000020, // <expr> '%=' <expr>
   DeeExprAstKind_SHL                = DeeExprAstKind_MODE_BINARY |0x00000021, // <expr> '<<' <expr>
   DeeExprAstKind_ISHL               = DeeExprAstKind_MODE_BINARY |0x00000022, // <expr> '<<=' <expr>
   DeeExprAstKind_SHR                = DeeExprAstKind_MODE_BINARY |0x00000023, // <expr> '>>' <expr>
   DeeExprAstKind_ISHR               = DeeExprAstKind_MODE_BINARY |0x00000024, // <expr> '>>=' <expr>
   DeeExprAstKind_AND                = DeeExprAstKind_MODE_BINARY |0x00000025, // <expr> '&' <expr>
   DeeExprAstKind_IAND               = DeeExprAstKind_MODE_BINARY |0x00000026, // <expr> '&=' <expr>
   DeeExprAstKind_OR                 = DeeExprAstKind_MODE_BINARY |0x00000027, // <expr> '|' <expr>
   DeeExprAstKind_IOR                = DeeExprAstKind_MODE_BINARY |0x00000028, // <expr> '|=' <expr>
   DeeExprAstKind_XOR                = DeeExprAstKind_MODE_BINARY |0x00000029, // <expr> '^' <expr>
   DeeExprAstKind_IXOR               = DeeExprAstKind_MODE_BINARY |0x0000002B, // <expr> '^=' <expr>
   DeeExprAstKind_POW                = DeeExprAstKind_MODE_BINARY |0x0000002A, // <expr> '**' <expr>
   DeeExprAstKind_IPOW               = DeeExprAstKind_MODE_BINARY |0x0000002C, // <expr> '**=' <expr>
   DeeExprAstKind_LAND               = DeeExprAstKind_MODE_BINARY |0x0000002D, // <expr> '&&' <expr>
   DeeExprAstKind_LOR                = DeeExprAstKind_MODE_BINARY |0x0000002E, // <expr> '||' <expr>
   DeeExprAstKind_LXOR               = DeeExprAstKind_MODE_BINARY |0x0000002F, // <expr> '^^' <expr>
   DeeExprAstKind_LIST               = DeeExprAstKind_MODE_N_NARY |0x00000030, // '[' [<list_expression_elem>] ']'
   DeeExprAstKind_TUPLE              = DeeExprAstKind_MODE_N_NARY |0x00000031, // '(' <expr> ',' <list_expression_elem> ')'
   DeeExprAstKind_CALL               = DeeExprAstKind_MODE_BINARY |0x00000032, // <expr> '(' <list_expression_elem> ')'
   DeeExprAstKind_SEQ_SIZE           = DeeExprAstKind_MODE_UNARY  |0x00000033, // '#' <expr>
   DeeExprAstKind_SEQ_GET            = DeeExprAstKind_MODE_BINARY |0x00000034, // <expr> '[' <expr> ']'
   DeeExprAstKind_SEQ_SET            = DeeExprAstKind_MODE_TRINARY|0x00000035, // <expr> '[' <expr> ']' = <expr>
   DeeExprAstKind_SEQ_RANGE_GET      = DeeExprAstKind_MODE_TRINARY|0x00000036, // <expr> '[' [<expr>] ':' [<expr>] ']'
   DeeExprAstKind_SEQ_RANGE_SET      = DeeExprAstKind_MODE_QUADARY|0x00000037, // <expr> '[' [<expr>] ':' [<expr>] ']' = <expr>
   DeeExprAstKind_ATTR_GET           = DeeExprAstKind_MODE_BINARY |0x00000038, // <expr> '.' <keyword>
   DeeExprAstKind_ATTR_SET           = DeeExprAstKind_MODE_TRINARY|0x00000039, // <expr> '.' <keyword> = <expr>
   DeeExprAstKind_EXPAND             = DeeExprAstKind_MODE_UNARY  |0x0000003A, // <expr> '...'
   DeeExprAstKind_IS                 = DeeExprAstKind_MODE_BINARY |0x0000003B, // <expr> 'is' <expr>
   DeeExprAstKind_IN                 = DeeExprAstKind_MODE_BINARY |0x0000003C, // <expr> 'in' <expr>
   DeeExprAstKind_FUNCTION           = DeeExprAstKind_MODE_CONST  |0x0000003D, // 'function' ['('] [<keyword> [',' <keyword>]...] ')'] '{' <statements> '}'
   DeeExprAstKind_FUNCTION_EXPR      = DeeExprAstKind_MODE_CONST  |0x0000003E, // 'function' ['('] [<keyword> [',' <keyword>]...] ')'] '->' <expr>
   DeeExprAstKind_FUNCTION_NONE      = DeeExprAstKind_MODE_CONST  |0x0000003F, // 'function' ['('] [<keyword> [',' <keyword>]...] ')']
   DeeExprAstKind_VFUNCTION          = DeeExprAstKind_MODE_CONST  |0x00000040, // 'function' ['('] [<keyword> [',' <keyword>]...] '...' ')'] '{' <statements> '}'
   DeeExprAstKind_VFUNCTION_EXPR     = DeeExprAstKind_MODE_CONST  |0x00000041, // 'function' ['('] [<keyword> [',' <keyword>]...] '...' ')'] '->' <expr>
   DeeExprAstKind_VFUNCTION_NONE     = DeeExprAstKind_MODE_CONST  |0x00000042, // 'function' ['('] [<keyword> [',' <keyword>]...] '...' ')']
   DeeExprAstKind_YFUNCTION          = DeeExprAstKind_MODE_CONST  |0x00000043, // 'function' ['('] [<keyword> [',' <keyword>]...] ')'] '{' <statements> '}'       // NOTE: Contains yield statements
   DeeExprAstKind_YVFUNCTION         = DeeExprAstKind_MODE_CONST  |0x00000044, // 'function' ['('] [<keyword> [',' <keyword>]...] '...' ')'] '{' <statements> '}' // NOTE: Contains yield statements
   DeeExprAstKind_STR                = DeeExprAstKind_MODE_UNARY  |0x00000045, // 'str'  <expr>
   DeeExprAstKind_REPR               = DeeExprAstKind_MODE_UNARY  |0x00000046, // 'repr' <expr>
   DeeExprAstKind_COPY               = DeeExprAstKind_MODE_UNARY  |0x00000047, // 'copy' <expr>
   DeeExprAstKind_MOVE               = DeeExprAstKind_MODE_UNARY  |0x00000048, // 'move' <expr>
   DeeExprAstKind_TYPE               = DeeExprAstKind_MODE_UNARY  |0x00000049, // 'type' <expr>
   DeeExprAstKind_WEAK               = DeeExprAstKind_MODE_UNARY  |0x0000004A, // 'weak' <expr>
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
   DeeExprAstKind_BUILTIN_ALLOCA     = DeeExprAstKind_MODE_UNARY  |0x0000004B, // '__builtin_alloca' '(' <expr> ')'
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
   DeeExprAstKind_BUILTIN_UNREACHABLE= DeeExprAstKind_MODE_CONST  |0x0000004C, // '__builtin_unreachable' '(' ')'
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
   DeeExprAstKind_BUILTIN_BREAKPOINT = DeeExprAstKind_MODE_CONST  |0x0000004D, // '__builtin_breakpoint' '(' ')'
#endif
   DeeExprAstKind_BUILTIN_HELP       = DeeExprAstKind_MODE_UNARY  |0x0000004E, // '__builtin_help' '(' <expr> ')'
   DeeExprAstKind_DICT               = DeeExprAstKind_MODE_N_NARY |0x0000004F, // 'dict' '{' [(<expr> ':' <expr>)...] '}'
#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_NEW_DELETE
   DeeExprAstKind_NEW                = DeeExprAstKind_MODE_UNARY  |0x00000050, // 'new' <expr>
   DeeExprAstKind_DELETE             = DeeExprAstKind_MODE_UNARY  |0x00000051, // 'delete' <expr>
#endif
   DeeExprAstKind_BUILTIN_EXPECT     = DeeExprAstKind_MODE_BINARY |0x00000052, // '__builtin_expect' '(' <expr> ',' <expr> ')'
   DeeExprAstKind_SET                = DeeExprAstKind_MODE_N_NARY |0x00000053, // 'set' '{' ',' ~~ <expr>... '}'
   DeeExprAstKind_BOOL               = DeeExprAstKind_MODE_UNARY  |0x00000054, // '!' '!' <expr>
   DeeExprAstKind_SUPER              = DeeExprAstKind_MODE_UNARY  |0x00000055, // <expr> '.' 'super'
   DeeExprAstKind___SUPER            = DeeExprAstKind_MODE_BINARY |0x00000056, // '__super' '(' <expr> [',' <expr>] ')' | 'super' (in thiscall-function)
   DeeExprAstKind_CLASSOF            = DeeExprAstKind_MODE_UNARY  |0x00000057, // <expr> '.' 'class'
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
   DeeExprAstKind_CLASS              = DeeExprAstKind_MODE_UNARY  |0x00000058, // 'class' ...
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_CELL_SYNTAX
   DeeExprAstKind_CELL               = DeeExprAstKind_MODE_UNARY  |0x00000059, // '<' [<unary_expr>] '>'
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
   DeeExprAstKind_BUILTIN_BOUND      = DeeExprAstKind_MODE_UNARY  |0x00000060, // '__builtin_bound' '(' <unary_expr> ')'
#endif
   DeeExprAstKind_DEX                = DeeExprAstKind_MODE_CONST  |0x00000061, // '__dex' <keyword>
   DeeExprAstKind_DECLVARIABLE       = DeeExprAstKind_MODE_CONST  |0x00000062, // <keyword> (in decl expression)
};

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
enum DeeClassDeclEntryKind {
 DeeClassDeclEntryKind_NONE = 0,
 DeeClassDeclEntryKind_SLOT = 1,
 DeeClassDeclEntryKind_METHOD = 2,
 DeeClassDeclEntryKind_GETSET = 3,
 DeeClassDeclEntryKind_CLASS_METHOD = 4,
 DeeClassDeclEntryKind_CLASS_GETSET = 5,
};
enum DeeClassDeclEntryVisibility {
 DeeClassDeclEntryKind_DEFAULT   = 0,
 DeeClassDeclEntryKind_PRIVATE   = 1,
 DeeClassDeclEntryKind_PUBLIC    = 2,
 DeeClassDeclEntryKind_PROTECTED = 3,
};
#define _DeeClassDeclEntry_VISIT(ob,object,xobject)\
do{DEE_COMPILER_MSVC_WARNING_PUSH(4061) \
switch((ob)->ob_kind){\
 case DeeClassDeclEntryKind_SLOT:\
  object((ob)->ob_slot.ob_callback);\
  break;\
 case DeeClassDeclEntryKind_METHOD:\
 case DeeClassDeclEntryKind_CLASS_METHOD:\
  object((ob)->ob_method.ob_name);\
  object((ob)->ob_method.ob_callback);\
  break;\
 case DeeClassDeclEntryKind_GETSET:\
 case DeeClassDeclEntryKind_CLASS_GETSET:\
  object((ob)->ob_getset.ob_name);\
  xobject((ob)->ob_getset.ob_get);\
  xobject((ob)->ob_getset.ob_del);\
  xobject((ob)->ob_getset.ob_set);\
  break;\
 default: break;\
}DEE_COMPILER_MSVC_WARNING_POP}while(0)

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeClassDeclEntry {
 enum DeeClassDeclEntryKind       ob_kind;
 enum DeeClassDeclEntryVisibility ob_visibility;
 union{
  struct {
   int                                ob_slotid;   /*< Type Slot ID. */
   DEE_A_REF struct DeeExprAstObject *ob_callback; /*< [1..1] Callback set for this slot. */
  } ob_slot;
  struct {
   DEE_A_REF struct DeeStringObject  *ob_name;     /*< [1..1] Name of this method. */
   DEE_A_REF struct DeeExprAstObject *ob_callback; /*< [1..1] Callback set for this method. */
  } ob_method;
  struct {
   DEE_A_REF struct DeeStringObject  *ob_name; /*< [1..1] Name of this method. */
   DEE_A_REF struct DeeExprAstObject *ob_get;  /*< [0..1] Getter. */
   DEE_A_REF struct DeeExprAstObject *ob_del;  /*< [0..1] Delete. */
   DEE_A_REF struct DeeExprAstObject *ob_set;  /*< [0..1] Setter. */
  } ob_getset;
 };
};
DEE_COMPILER_MSVC_WARNING_POP
struct DeeClassDeclObject {
 DEE_OBJECT_HEAD
 DEE_A_REF struct DeeStringObject *ob_name;   /*< [0..1] Name of the class. */
 DEE_A_REF DeeExprAstObject       *ob_base;   /*< [0..1] Base of the class. */
 Dee_size_t                        ob_entryc; /*< Actual amount of entries. */
 Dee_size_t                        ob_entrya; /*< Allocated amount of entries. */
 struct DeeClassDeclEntry         *ob_entryv; /*< [0..ob_entryc][owned] Vector of class decl entires. */
};
#define DeeClassDecl_ELEM(ob) ((struct DeeClassDeclObject *)Dee_REQUIRES_POINTER(ob))->ob_entryv
#define DeeClassDecl_SIZE(ob) ((struct DeeClassDeclObject *)Dee_REQUIRES_POINTER(ob))->ob_entryc
#define DeeClassDecl_BASE(ob) ((struct DeeClassDeclObject *)Dee_REQUIRES_POINTER(ob))->ob_base
#define DeeClassDecl_NAME(ob) ((DeeObject *)((struct DeeClassDeclObject *)Dee_REQUIRES_POINTER(ob))->ob_name)
extern DeeTypeObject DeeClassDecl_Type;
#define DeeClassDecl_Check(ob)      DeeObject_InstanceOf(ob,&DeeClassDecl_Type)
#define DeeClassDecl_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeClassDecl_Type)
extern DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeClassDeclObject) *DeeClassDecl_New(
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *base) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *DeeClassDecl_MakeConstantClass(
 DEE_A_INOUT_OBJECT_OPT(struct DeeClassDeclObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeClassDecl_IncreaseCapacity(
 DEE_A_INOUT struct DeeClassDeclObject *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(NULL) struct DeeClassDeclEntry *DeeClassDecl_AllocSlot(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddSlot(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN int slot, DEE_A_INOUT_OBJECT(DeeExprAstObject) *decl)
 DEE_ATTRIBUTE_NONNULL((1,4));
extern DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddMethod(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN enum DeeClassDeclEntryKind kind,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT(DeeExprAstObject) *decl)
 DEE_ATTRIBUTE_NONNULL((1,4,5));
// Stored any token associated with the declaration in '*decl_token',
// or NULL if no token was associated with the declaration
extern DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasSlot(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN int slot, DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token)
 DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasFunctionOrGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_NOEXCEPT(0) int DeeClassDecl_HasClassFunctionOrGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OBJECT(DeeTokenObject) **decl_token) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeClassDecl_AddGetSet(
 DEE_A_INOUT_OBJECT(struct DeeClassDeclObject) *self,
 DEE_A_IN enum DeeClassDeclEntryVisibility visibility,
 DEE_A_IN enum DeeClassDeclEntryKind kind,
 DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *get_decl,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *del_decl,
 DEE_A_INOUT_OBJECT_OPT(DeeExprAstObject) *set_decl)
 DEE_ATTRIBUTE_NONNULL((1,4));
#endif

//////////////////////////////////////////////////////////////////////////
// Ast Object
struct DeeScopeObject;
struct DeeLocalVarObject;
struct DeeTupleObject;
struct DeeTokenObject;
struct DeeStmtAstObject;
struct DeeModuleObject;
struct DeeLexerObject;
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeExprAstObject {
 DEE_OBJECT_HEAD
 DeeExprAstKind                       ob_kind;
 DEE_A_REF struct DeeTokenObject     *ob_token;     /*< [1..1] Token associated with this ast (For output only). */
 union{
  DEE_A_REF        DeeExprAstObject  *ob_op_a;      /*< [0..1] Ast object a. */
  DEE_A_REF        DeeObject         *ob_const;     /*< [1..1] Constant (DeeAstOp_MODE_CONST only). */
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
  DEE_A_REF struct DeeStmtAstObject  *ob_stmt;      /*< [1..1] Used by DeeExprAstKind_CODE. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
  DEE_A_REF struct DeeLocalVarObject *ob_var;       /*< [1..1] Used by DeeExprAstKind_VARIABLE. (owns a use to the variable) */
  DEE_A_REF struct DeeModuleObject   *ob_mod_module;/*< [1..1] Used by DeeExprAstKind_MODULE. */
  DEE_A_REF struct DeeTupleObject    *ob_args_a;    /*< [1..1] Tuple of ast objects (DeeExprAstKind_LIST+DeeExprAstKind_TUPLE+DeeExprAstKind_SET only). */
  DEE_A_REF struct DeeTupleObject    *ob_dict;      /*< [1..1] Tuple of ast objects (DeeExprAstKind_DICT only) Length is always even, as every two items describe a key/item pair. */
  DEE_A_REF struct _DeeTokenIDListObject *ob_fun_args; /*< [1..1] Tuple of argument name ids (DeeExprAstKind_FUNCTION+DeeExprAstKind_FUNCTION_EXPR only). */
  DEE_A_REF struct DeeClassDeclObject *ob_classdecl; /*< [1..1] Class decl. */
  DEE_A_REF struct DeeStringObject   *ob_dexname;   /*< [1..1] __dex Variable name (DeeExprAstKind_DEX only). */
 };
 union{
  DEE_A_REF        DeeExprAstObject  *ob_op_b;      /*< [0..1] Ast object b. */
  DEE_A_REF struct DeeScopeObject    *ob_scope;     /*< [1..1] Used by DeeExprAstKind_CODE + DeeExprAstKind_FUNCTION + DeeExprAstKind_FUNCTION_EXPR. */
  DEE_A_REF struct DeeStringObject   *ob_member;    /*< [1..1] Used by DeeExprAstKind_ATTR_GET / DeeExprAstKind_ATTR_SET. */
 };
 union{
  DEE_A_REF DeeExprAstObject         *ob_op_c;      /*< [0..1] Ast object c. */
  DEE_A_REF DeeExprAstObject         *ob_fun_expr;  /*< [1..1] Used by DeeExprAstKind_FUNCTION_EXPR (function expression). */
  DEE_A_REF struct DeeStmtAstObject  *ob_fun_stmt;  /*< [1..1] Used by DeeExprAstKind_FUNCTION (function statements). */
 };
 union{
  DEE_A_REF DeeExprAstObject         *ob_op_d;     /*< [0..1] Ast object d. */
  DEE_A_REF struct DeeStringObject   *ob_fun_name; /*< [0..1] Name of a function (if it has one). */
 };
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeeExprAst_KIND(ob)   ((DeeExprAstObject *)(ob))->ob_kind
#define DeeExprAst_TOKEN(ob)  ((DeeExprAstObject *)(ob))->ob_token
#define DeeExprAst_CONST(ob)  ((DeeExprAstObject *)(ob))->ob_const
#define DeeExprAst_DICT(ob)   ((DeeExprAstObject *)(ob))->ob_dict
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
#define DeeExprAst_STMT(ob)   ((DeeExprAstObject *)(ob))->ob_stmt
#endif
#define DeeExprAst_SCOPE(ob)  ((DeeExprAstObject *)(ob))->ob_scope
#define DeeExprAst_MODULE(ob) ((DeeExprAstObject *)(ob))->ob_mod_module
#define DeeExprAst_VAR(ob)    ((DeeExprAstObject *)(ob))->ob_var
#define DeeExprAst_ARGS_A(ob) ((DeeExprAstObject *)(ob))->ob_args_a
#define DeeExprAst_MEMBER(ob) ((DeeExprAstObject *)(ob))->ob_member
#define DeeExprAst_OP_A(ob)   ((DeeExprAstObject *)(ob))->ob_op_a
#define DeeExprAst_OP_B(ob)   ((DeeExprAstObject *)(ob))->ob_op_b
#define DeeExprAst_OP_C(ob)   ((DeeExprAstObject *)(ob))->ob_op_c
#define DeeExprAst_OP_D(ob)   ((DeeExprAstObject *)(ob))->ob_op_d
#define DeeExprAst_DEXNAME(ob) ((DeeExprAstObject *)(ob))->ob_dexname
#define DeeExprAst_FUNCTION_SCOPE      DeeExprAst_SCOPE
#define DeeExprAst_FUNCTION_ARGS(ob)   ((DeeExprAstObject *)(ob))->ob_fun_args // DeeExprAstKind_FUNCTION + DeeExprAstKind_FUNCTION_EXPR
#define DeeExprAst_FUNCTION_STMTS(ob)  ((DeeExprAstObject *)(ob))->ob_fun_stmt // DeeExprAstKind_FUNCTION
#define DeeExprAst_FUNCTION_EXPR(ob)   ((DeeExprAstObject *)(ob))->ob_fun_expr // DeeExprAstKind_FUNCTION_EXPR
#define DeeExprAst_FUNCTION_NAME(ob)   ((DeeExprAstObject *)(ob))->ob_fun_name // DeeExprAstKind_FUNCTION + DeeExprAstKind_FUNCTION_EXPR

#define DeeExprAst_Check(ob) (Dee_TYPE(ob)==&DeeExprAst_Type)
extern DeeTypeObject DeeExprAst_Type;

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeExprAst_New(
 DEE_A_IN DeeExprAstKind kind, DEE_A_IN_OBJECT(DeeTokenObject) *token) DEE_ATTRIBUTE_NONNULL((2));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeExprAst_CopyWithScope(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self, DEE_A_INOUT_OBJECT(DeeScopeObject) *scope) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Clears the pool of cached expression ast objects
extern Dee_size_t DeeExprAst_ClearPool(void);

//////////////////////////////////////////////////////////////////////////
// Returns 1 if 'self' requires a semicolon to follow after; 0 otherwise
extern DEE_A_RET_WUNUSED int DeeExprAst_NeedSemicollon(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Walks down the tree, to find the branch with the left-most token
// - noexcept: Always returns non-null
// >> Used during error output, to get to the token display
// NOTE: 'DeeExprAst_End' does the same for the right-most token
// >> 'DeeExprAst_End' is used to check if a ';' is required to end an expression
extern DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeExprAstObject) *DeeExprAst_Begin(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeExprAstObject) *DeeExprAst_End(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Compiles an expression ast
//  - Leaves the writer with +1 stack entries (if DEE_COMPILER_FLAG_USED is set)
//  - scope should be the scope set in the statement AST where this expression is executed from
//  - Returns -1 on error
struct DeeCodeWriter;
extern DEE_A_RET_EXCEPT(-1) int DeeExprAst_Compile(
 DEE_A_IN_OBJECT(DeeExprAstObject) *self,
 DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCodeWriter *writer,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2,3));


//////////////////////////////////////////////////////////////////////////
// Predict the type of an expression ast
// >> Returns NULL if the type is unpredictable
extern DEE_A_RET_WUNUSED DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DeeTypeObject *DeeExprAst_PredictType(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeExprAst_IsNoexcept(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeExprAst_IsNoreturn(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeExprAst_IsNoeffect(
 DEE_A_IN_OBJECT(DeeExprAstObject) const *self, DEE_A_IN_OBJECT(DeeScopeObject) const *scope) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Optimize an expression AST and return the new optimized AST
extern DEE_A_RET_EXCEPT(-1) int DeeExprAst_Optimize(
 DEE_A_INOUT_OBJECT(DeeExprAstObject) *self,
 DEE_A_INOUT_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));



//////////////////////////////////////////////////////////////////////////
// Lexer: Parse expression
//  - Returns the freshly parsed expression AST
//  - Returns NULL on error
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpression(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionLv2(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionDecl(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeExprAstObject) *DeeLexer_ParseExpressionTuple(
 DEE_A_IN_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));




struct DeeArrayTypeObject;
struct DeeAttributeDecl;
struct DeeFunctionObject;
struct DeeLexerObject;
struct DeeScopeObject;
struct DeeStructTypeObject;
struct DeeTupleObject;

//////////////////////////////////////////////////////////////////////////
// Expression portion parsers
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
extern int _DeeLexer_ParseClassGetSet(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags,
 DEE_A_OUT DeeExprAstObject **getter_, DEE_A_OUT DeeExprAstObject **delete_,
 DEE_A_OUT DeeExprAstObject **setter_) DEE_ATTRIBUTE_NONNULL((1,2,4,5,6));
extern DeeExprAstObject *_DeeLexer_ParseClass(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DeeTypeObject *_DeeLexer_ParseConstClass(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern int _DeeLexer_ParseClassDecl(
 struct DeeClassDeclObject *decl, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 // v set to '0' for unnamed
 DEE_A_IN unsigned int class_name_id,
 DEE_A_OUT int *class_is_const_candidate,
 DEE_A_INOUT struct DeeAttributeDecl *attr);
extern int _DeeLexer_ParseConstClassDecl(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *decl, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 // v set to '0' for unnamed
 DEE_A_IN unsigned int class_name_id,
 DEE_A_INOUT struct DeeAttributeDecl *attr);
#endif

extern DeeExprAstObject *_DeeLexer_ParseList(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS
extern DeeExprAstObject *_DeeLexer_ParseTupleOrParenOrCode(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, int *is_double_paren);
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
extern DeeExprAstObject *_DeeLexer_ParseTupleOrParen(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, int *is_double_paren);
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_EXPR_STATEMENTS */
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXPRESSION
extern DeeExprAstObject *_DeeLexer_ParseIfExpr(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
extern DeeExprAstObject *_DeeLexer_ParseType(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CHOOSE_EXPR
extern DeeExprAstObject *_DeeLexer_ParseBuiltinChooseExpr(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_CONSTANT_P
extern DeeExprAstObject *_DeeLexer_ParseBuiltinConstantP(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_NOOP
extern DeeExprAstObject *_DeeLexer_ParseBuiltinNoop(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
extern DeeExprAstObject *_DeeLexer_ParseBuiltinAlloca(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
// NOTE: Returns tp_p_* slot ids if the real id depends on an associated argument count
extern int _DeeLexer_ParseTypeSlotID(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern unsigned int _Dee_TypeSlot_ToTokenID(int id);
extern struct DeeTupleObject *_DeeLexer_ParseSetInit(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern struct DeeTupleObject *_DeeLexer_ParseListInit(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern struct DeeTupleObject *_DeeLexer_ParseDictInit(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DeeExprAstObject *_DeeLexer_ParseGeneric(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DeeExprAstObject *_DeeLexer_ParseFunction(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern struct DeeFunctionObject *_DeeLexer_ParseConstFunction(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern int _DeeLexer_ParseFunctionSuffix(DeeExprAstObject *func_ast, struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, struct DeeAttributeDecl *attr, unsigned int func_name);
extern int _DeeLexer_ParseConstFunctionSuffix(struct DeeFunctionObject *func, struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, struct DeeAttributeDecl *attr, unsigned int func_name);
extern DeeExprAstObject *_DeeLexer_ParseFunCall(DeeExprAstObject *func, struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
// NOTE: returns '1' if the object does exist; return '2' if the object may exist
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int _DeeLexer_ParseExistsExpression(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DeeTypeObject *_DeeLexer_ParseConstantType(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedType(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args);
extern DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, struct DeeAttributeDecl *attr);
extern DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeWithBase(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, DeeTypeObject *base);
extern DEE_A_RET_EXCEPT_REF DeeTypeObject *_DeeLexer_ParseNamedTypeWithBaseAttr(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DEE_A_OUT_OPT int *unsized, DEE_A_OUT_OPT struct DeeTokenObject **name_token,
 DEE_A_INOUT_OPT struct _DeeTokenIDListObject **func_args, DeeTypeObject *base, struct DeeAttributeDecl *attr);
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
extern int _DeeLexer_ParseEnumDecl(
 struct DeeLexerObject *self, struct DeeScopeObject *scope,
 Dee_uint32_t flags, DeeTypeObject *constant_type);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
extern int _DeeLexer_ParseStructDecl(
 struct DeeStructTypeObject *struct_type, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 int union_mode, struct DeeAttributeDecl *decl_attr);
#endif
extern DeeTypeObject *_DeeLexer_ParseTypeBaseDecl(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
extern DeeExprAstObject *_DeeLexer_ParseBuiltinExpect(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXISTS
extern DeeExprAstObject *_DeeLexer_ParseBuiltinExists(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_OFFSETOF
extern DeeExprAstObject *_DeeLexer_ParseBuiltinOffsetof(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
extern DeeExprAstObject *_DeeLexer_ParseBuiltinBound(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
extern DeeExprAstObject *_DeeLexer_ParseArrayInitializer(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags, struct DeeArrayTypeObject *array_type);
extern DeeExprAstObject *_DeeLexer_ParseConditionalExpressionLv2(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
extern DeeExprAstObject *_DeeLexer_ParseForGenerator(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DeeExprAstObject *_DeeLexer_ParseWhileGenerator(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
extern DeeExprAstObject *_DeeLexer_ParseDoGenerator(struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags);
#endif
extern int _DeeExprAst_CompileCall(DeeExprAstObject *self, struct DeeScopeObject *scope, struct DeeCodeWriter *writer, struct DeeLexerObject *lexer, Dee_uint32_t flags);
extern int _DeeExprAst_OptimizeTuple(DeeExprAstObject *self);
extern int _DeeExprAst_OptimizeSet(DeeExprAstObject *self);
extern int _DeeExprAst_OptimizeList(DeeExprAstObject *self);
extern int _DeeExprAst_OptimizeDict(DeeExprAstObject *self);
extern int _DeeExprAst_OptimizeFunction(DeeExprAstObject *self, struct DeeLexerObject *lexer);
extern int _DeeExprAst_OptimizeFunctionKnownPredicates(DeeExprAstObject *self, struct DeeLexerObject *lexer);

#if DEE_CONFIG_LANGUAGE_HAVE_FEATURE_POINTERS
// v adds the offsets of tp_attr_get branches together
extern DEE_A_RET_NOEXCEPT(0) int _DeeExprAst_CalculateOffsetof(
 DeeExprAstObject *self, DEE_A_OUT Dee_size_t *offset, DEE_A_OUT DeeTypeObject **last_type);
#endif

#define DEE_HAVE_CONSTOP_OPTIMIZATION(ob)         1
#define DEE_HAVE_CONSTOP_OPTIMIZATION_SHIFT(ob)  (!DeeFile_Check(ob))
#define DEE_HAVE_CONSTOP_OPTIMIZATION_OR(ob)      1 // This is allowed with files, though...
#define DEE_HAVE_CONSTOP_OPTIMIZATION_BOOL(ob)    1
#define DEE_HAVE_CONSTOP_OPTIMIZATION_SUPER(ob)   1
#define DEE_HAVE_CONSTOP_OPTIMIZATION_CLASSOF(ob) 1

// Returns false if executing 'kind' with 'tp()' as this-instance might modify that instance
// TODO: Implement this (DEE_WARNING_MODIFYING_CONSTANT) for all the operators
extern int _DeeType_IsSlotConstant(DeeTypeObject const *tp, DeeExprAstKind kind);

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_EXPR_AST_H */
