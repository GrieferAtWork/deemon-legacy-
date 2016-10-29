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
#ifndef GUARD_DEEMON_RUNTIME_SANDBOX_H
#define GUARD_DEEMON_RUNTIME_SANDBOX_H 1

#if 0
#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#ifdef DEE_LIMITED_API
#include <deemon/runtime/execute.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif


//////////////////////////////////////////////////////////////////////////
// Code execution in a sand-boxed environment
struct DeeSandbox;
struct DeeSandboxRule;

#define DEE_SANDBOX_RULEMODE_ALLOW 0 /*< The rule defines an allowed action. */
#define DEE_SANDBOX_RULEMODE_DENY  1 /*< The rule defines an illegal action. */
// NOTE: Rule modes greater than '1' will define reasons for denial

#ifdef DEE_LIMITED_DEX
#define DEE_SANDBOXRULE_KIND_NONE             DEE_UINT32_C(0x00000000)
#define DEE_SANDBOXRULE_KIND_OBJECT_BYTYPE    DEE_UINT32_C(0x00000001)
#define DEE_SANDBOXRULE_KIND_OBJECT_BYOBJECT  DEE_UINT32_C(0x00000002)
#define DEE_SANDBOXRULE_KIND_OBJECT_BYCUSTOM  DEE_UINT32_C(0x00000003)
#define DEE_SANDBOXRULE_KIND_OPCODE_BYID      DEE_UINT32_C(0x00000004)
#define DEE_SANDBOXRULE_MASK_KIND             DEE_UINT32_C(0x000000FF)
#define DEE_SANDBOXRULE_MASK_MODE             DEE_UINT32_C(0x00000F00)
#define DEE_SANDBOXRULE_MASK_FLAGS            DEE_UINT32_C(0xFFFFF000)
#define DEE_SANDBOXRULE_KIND(x)             ((x)&DEE_SANDBOXRULE_MASK_KIND)     /* 'DEE_SANDBOXRULE_KIND_*' */
#define DEE_SANDBOXRULE_MODE(x)            (((x)&DEE_SANDBOXRULE_MASK_MODE)>>8) /* 'DEE_SANDBOX_RULEMODE_*' */
#define DEE_SANDBOXRULE_FLAGS(x)            ((x)&DEE_SANDBOXRULE_MASK_FLAGS)    /* 'DEE_SANDBOXRULE_FLAG_*' */

struct DeeSandboxByTypeRule {
 DEE_A_REF DeeTypeObject *sbbtr_type;   /*< [1..1] The type that this rule affects. */
};
struct DeeSandboxByObjectRule {
 DEE_A_REF DeeObject     *sbbor_object; /*< [1..1] The object that this rule affects. */
};
struct DeeSandboxByOpcodeRule {
 DEE_A_REF DeeObject     *sbbor_object; /*< [1..1] The object that this rule affects. */
};
struct DeeSandboxRule {
 struct DeeSandboxRule *sbr_prev; /*< [0..1] Prev rule. */
 struct DeeSandboxRule *sbr_next; /*< [0..1] Next rule. */
 Dee_uint32_t           sbr_kind; /*< Rule kind & basic flags. */
 union{
  struct DeeSandboxByTypeRule   sbr_bytype;
  struct DeeSandboxByObjectRule sbr_byobject;
 };
};
struct DeeSandbox {
 Dee_uint32_t           sb_flags;  /*< Basic sandbox environment flags. */
 struct DeeSandboxRule *sb_rbytyp; /*< [0..1][owned][this->sbr_prev == NULL] By-type rules. */
 struct DeeSandboxRule *sb_rbyobj; /*< [0..1][owned][this->sbr_prev == NULL] By-object rules. */
 struct DeeSandboxRule *sb_rbyop;  /*< [0..1][owned][this->sbr_prev == NULL] By-opcode rules. */
 struct DeeSandboxRule *sb_rbyxop; /*< [0..1][owned][this->sbr_prev == NULL] By-ext-opcode rules. */
 struct DeeSandboxRule *sb_rfree;  /*< [0..1][owned][this->sbr_prev == NULL] Free rules. */
};
#define DeeSandbox_INIT() {DEE_SANDBOX_FLAG_NONE,NULL,NULL,NULL,NULL,NULL}
#define DeeSandbox_GET_FLAGS(ob)   ((ob)->sb_flags)
#define DeeSandbox_SET_FLAGS(ob,v) ((ob)->sb_flags = (v))
#else
#define DeeSandbox_GET_FLAGS DeeSandbox_GetFlags
#define DeeSandbox_SET_FLAGS DeeSandbox_SetFlags
#endif

#define DEE_SANDBOX_FLAG_NONE        DEE_UINT32_C(0x00000000)
#define DEE_SANDBOX_FLAG_SAFEOPS     DEE_UINT32_C(0x00000001) /*< Enable safety constraints on bytecode, such as jumps not being out-of-bounds, or constants or locals actually existing. */
#define DEE_SANDBOX_FLAG_NOUNCHECKED DEE_UINT32_C(0x00000002) /*< Don't allow execution of operators on unchecked (aka. non-fundamental) types (NOTE: fundamental refers to integers, floats, bool, list, tuple, set, dict, strings, as well as constexpr functions). */
#define DEE_SANDBOX_FLAG_NOOUTPUT    DEE_UINT32_C(0x00000004) /*< Don't allow the program to output anything using the various print opcodes. */

DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeSandbox *) DeeSandbox_New(void);
DEE_FUNC_DECL(void) DeeSandbox_Delete(DEE_A_IN struct DeeSandbox *self);
#ifdef DEE_LIMITED_DEX
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeSandbox_Init(DEE_A_OUT struct DeeSandbox *self);
DEE_FUNC_DECL(void) DeeSandbox_Quit(DEE_A_IN struct DeeSandbox *self);
#endif
#ifdef DEE_LIMITED_API
DEE_FUNC_DECL(void) DeeSandbox_Violation(DEE_A_IN struct DeeSandbox *self);
#endif

//////////////////////////////////////////////////////////////////////////
// Get/set the current sandbox flag configuration (set of 'DEE_SANDBOX_FLAG_*')
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint32_t) DeeSandbox_GetFlags(DEE_A_IN struct DeeSandbox const *sbx);
DEE_FUNC_DECL(void) DeeSandbox_SetFlags(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint32_t flags);

//////////////////////////////////////////////////////////////////////////
// Delete a given sandbox rule
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeSandbox_DelRule(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN struct DeeSandboxRule *rule);

//////////////////////////////////////////////////////////////////////////
// Set the mode of a given rule
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeSandbox_SetRuleMode(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN struct DeeSandboxRule *rule, DEE_A_IN Dee_uint8_t mode);

//////////////////////////////////////////////////////////////////////////
// Add new rules for illegal types, objects and opcodes
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeSandboxRule *) DeeSandbox_AddIllegalTypeEx(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint8_t mode, DEE_A_INOUT DeeTypeObject *ob);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeSandboxRule *) DeeSandbox_AddIllegalObject(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint8_t mode, DEE_A_INOUT DeeObject *ob);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeSandboxRule *) DeeSandbox_AddIllegalOpcode(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint8_t mode, DEE_A_IN Dee_uint8_t opcode);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeSandboxRule *) DeeSandbox_AddIllegalExtOpcode(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint8_t mode, DEE_A_IN Dee_uint8_t opcode);

//////////////////////////////////////////////////////////////////////////
// Returns any 'DEE_SANDBOX_RULEMODE_*' value, where
// 'DEE_SANDBOX_RULEMODE_DENY' indicates that the action is allowed.
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint8_t) DeeSandbox_OpcodeAllowed(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint8_t opcode);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint8_t) DeeSandbox_ExtOpcodeAllowed(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN Dee_uint16_t opext_code);
#define DeeSandbox_ObjectAllowed(sbx,ob) DeeSandbox_TObjectAllowed(sbx,Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint8_t) DeeSandbox_TObjectAllowed(DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN DeeTypeObject const *tp_ob, DEE_A_IN DeeObject *ob);

//////////////////////////////////////////////////////////////////////////
// Run given code under restrictions set by the sandbox
// >> Returns the regular code return value or NULL if
//    an error occurred, including the violation of any active sandbox rule
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeSandbox_RunCode(
 DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_R(refc) DeeObject **refv,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject **argv,
 DEE_A_IN_OPT DeeObject *this_arg);

#ifdef DEE_LIMITED_API
struct DeeSandboxStackFrame {
 struct DeeStackFrame    sbf_frame; /*< Regular, underlying stackframe. */
#ifndef DEE_DEBUG
 /*readonly*/Dee_size_t  sbf_argc;  /*< Amount of arguments. */
 /*readonly*/Dee_size_t  sbf_refc;  /*< Amount of references. */
#endif
};
#ifdef DEE_DEBUG
#define DeeSandboxStackFrame_ARGC(ob) (((struct DeeSandboxStackFrame *)(ob))->sbf_frame.f_argc)
#define DeeSandboxStackFrame_REFC(ob) (((struct DeeSandboxStackFrame *)(ob))->sbf_frame.f_refc)
#else
#define DeeSandboxStackFrame_ARGC(ob) ((ob)->sbf_argc)
#define DeeSandboxStackFrame_REFC(ob) ((ob)->sbf_refc)
#endif
#endif

DEE_DECL_END
#endif

#endif /* !GUARD_DEEMON_RUNTIME_SANDBOX_H */
