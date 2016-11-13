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
#ifndef GUARD_DEEMON_CODEWRITER_H
#define GUARD_DEEMON_CODEWRITER_H 1

#ifndef DEE_LIMITED_API
#error "Limited Header"
#endif

// include/*
#include <deemon/__conf.inl>
#include <deemon/compiler/code.h>
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#include <deemon/runtime/execute.h>

DEE_DECL_BEGIN

#define DeeExceptionHandlerEntry_InitEx(ob,writer,kind,inherited_mask_ref)\
((ob)->e_store = 0,(ob)->e_type = (inherited_mask_ref),\
 (ob)->e_kind = (kind),\
 (ob)->e_stack = (writer)->cw_stack_size,\
 (ob)->e_handler = DeeCodeWriter_ADDR(writer))
#define DeeExceptionHandlerEntry_InitFinally(ob,writer)\
 DeeExceptionHandlerEntry_InitEx(ob,writer,DeeExceptionHandleKind_FINALLY,NULL)
#define DeeExceptionHandlerEntry_InitCatchAll(ob,writer)\
 DeeExceptionHandlerEntry_InitEx(ob,writer,DeeExceptionHandleKind_ALL,NULL)



struct DeeScopeObject;
struct DeeLocalVarObject;
struct DeeLexerObject;
struct DeeTokenObject;
struct DeeParserLabelList;

struct DeeForwardCodeLabel {
 DEE_A_REF struct DeeTokenObject *fcl_token;       /*< [1..1] Label used for error/debug output. */
 Dee_size_t                       fcl_jmp_addr;    /*< Address of the uninitialized OP_JUMP opcode argument. */
 Dee_size_t                       fcl_stack_size;  /*< Alignment of the stack at the goto instruction. */
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t                       fcl_except_size; /*< Alignment of exception handlers at the goto instruction. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
};
#define _DeeForwardCodeLabel_Quit(ob)\
do{ Dee_DECREF((ob)->fcl_token); }while(0)
#define _DeeForwardCodeLabel_Visit(ob)\
do{ Dee_VISIT((ob)->fcl_token); }while(0)

struct DeeCodeLabel {
 DEE_A_REF struct DeeTokenObject *cl_endptok;     /*< [0..1] Destination token. */
 Dee_size_t                       cl_addr;        /*< Destination address of the label (absolute). */
 Dee_size_t                       cl_fwd_c;       /*< Amount of forward uses of this label. */
 struct DeeForwardCodeLabel      *cl_fwd_v;       /*< [0..l_fwd_c][owned] List of uninitialized forward jumps to 'l_addr'. */
 Dee_size_t                       cl_stack_size;  /*< Alignment of the stack where this label leads to. */
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t                       cl_except_size; /*< Alignment of exception handlers where this label leads to. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
};
#define DeeCodeLabel_HAS_ENDP(ob)    ((ob)->cl_endptok!=NULL)
#define DeeCodeLabel_HAS_FORWARD(ob) ((ob)->cl_fwd_c!=0)
#define _DeeCodeLabel_Init(ob) memset((ob),0,sizeof(struct DeeCodeLabel))
#define _DeeCodeLabel_Quit(ob) \
do{\
 struct DeeForwardCodeLabel *lblfwd_iter,*lblfwd_end;\
 lblfwd_end = (lblfwd_iter = (ob)->cl_fwd_v)+(ob)->cl_fwd_c;\
 while (lblfwd_iter != lblfwd_end) { _DeeForwardCodeLabel_Quit(lblfwd_iter); ++lblfwd_iter; }\
 free((ob)->cl_fwd_v);\
 Dee_XDECREF((ob)->cl_endptok);\
}while(0)
#define _DeeCodeLabel_Visit(ob) \
do{\
 struct DeeForwardCodeLabel *lblfwd_iter,*lblfwd_end;\
 lblfwd_end = (lblfwd_iter = (ob)->cl_fwd_v)+(ob)->cl_fwd_c;\
 while (lblfwd_iter != lblfwd_end) { _DeeForwardCodeLabel_Visit(lblfwd_iter); ++lblfwd_iter; }\
 Dee_XVISIT((ob)->cl_endptok);\
}while(0)


struct DeeCodeWriterFreeLocalList {
 Dee_size_t  cwfl_c; /*< Actual size of the free local vector. */
 Dee_size_t  cwfl_a; /*< Allocated size of the free local vector. */
 Dee_size_t *cwfl_v; /*< [0..fl_c][owned] Vector of free local ids. */
};
#define DeeCodeWriterFreeLocalList_INIT() {0,0,NULL}
#define _DeeCodeWriterFreeLocalList_Init(ob) \
do{ (ob)->cwfl_c = 0; (ob)->cwfl_a = 0; (ob)->cwfl_v = NULL; }while(0)
#define _DeeCodeWriterFreeLocalList_Quit(ob) \
do{ free((ob)->cwfl_v); }while(0)

//////////////////////////////////////////////////////////////////////////
// Pops one free local id, that may then be reused
#define _DeeCodeWriterFreeLocalList_Pop(ob) \
 (DEE_ASSERT((ob)->cwfl_c),(ob)->cwfl_v[--(ob)->cwfl_c])

//////////////////////////////////////////////////////////////////////////
// Appends a new free local id (it is asserted that the id is not yet contained)
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterFreeLocalList_Append(
 DEE_A_INOUT struct DeeCodeWriterFreeLocalList *self, DEE_A_IN Dee_size_t id);


struct DeeCodeWriterStackMem {
 Dee_size_t cwsm_start; /*< Start offset of the stackdata. */
 Dee_size_t cwsm_size;  /*< Size of the stackdata. */
};
struct DeeCodeWriterStackMemList {
 Dee_size_t                    cwsml_cursize; /*< Current total size. */
 Dee_size_t                    cwsml_maxsize; /*< Greatest total size. */
 Dee_size_t                    cwsml_memc;    /*< Used amount of stackdata blocks. */
 Dee_size_t                    cwsml_mema;    /*< Allocates amount of stackdata blocks. */
 struct DeeCodeWriterStackMem *cwsml_memv;    /*< [0..cwsml_memc][owned] List of stackdata blocks (Sorted ascendingly). */
};
#define DeeCodeWriterStackMemList_INIT() {0,0,0,0,NULL}
#define _DeeCodeWriterStackMemList_Init(ob) \
do{\
 (ob)->cwsml_cursize = 0;\
 (ob)->cwsml_maxsize = 0;\
 (ob)->cwsml_memc = 0;\
 (ob)->cwsml_mema = 0;\
 (ob)->cwsml_memv = NULL;\
}while(0)
#define _DeeCodeWriterStackMemList_Quit(ob) \
do{ free((ob)->cwsml_memv); }while(0)

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterStackMemList_IncBuf(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriterStackMemList_AllocBlock(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
extern void DeeCodeWriterStackMemList_FreeBlock(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self,
 DEE_A_IN Dee_size_t addr) DEE_ATTRIBUTE_NONNULL((1));


struct DeeCodeWriterDebugLnoTab {
 Dee_size_t                cwdlt_addr; /*< Last address at which debug information was written. */
 Dee_size_t                cwdlt_lnoa; /*< Allocated lno-vector size. */
 Dee_size_t                cwdlt_lnoc; /*< Used lno-vector size. */
 struct _DeeCodeLnOffList *cwdlt_lnov; /*< [0..cwdlt_lnoc] Lno-vector. */
};
#define DeeCodeWriterDebugLnoTab_Empty(ob) ((ob)->cwdlt_lnoc == 0)
extern DEE_A_RET_WUNUSED Dee_int64_t DeeCodeWriterDebugLnoTab_Addr2Line(
 DEE_A_IN struct DeeCodeWriterDebugLnoTab const *self,
 DEE_A_IN Dee_size_t addr) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugLnoTab_IncBuf(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeCodeWriterDebugLnoTab_PutAddrLnoff(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self,
 DEE_A_IN Dee_size_t addr, DEE_A_IN Dee_int64_t lnoff) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Relocate the given address range to no longer be covered
extern void DeeCodeWriterDebugLnoTab_Relocate(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self,
 DEE_A_IN Dee_size_t start, DEE_A_IN Dee_size_t size) DEE_ATTRIBUTE_NONNULL((1));

#define DeeCodeWriterDebugLnoTab_INIT() {0,0,0,NULL}
#define _DeeCodeWriterDebugLnoTab_Init(ob)\
do{\
 (ob)->cwdlt_addr = 0;\
 (ob)->cwdlt_lnoa = 0;\
 (ob)->cwdlt_lnoc = 0;\
 (ob)->cwdlt_lnov = NULL;\
}while(0)
#define _DeeCodeWriterDebugLnoTab_Quit(ob)\
do{ free((ob)->cwdlt_lnov); }while(0)

struct DeeCodeWriterDebugFnoTab {
 struct DeeCodeWriterDebugLnoTab  cwdft_lno;    /*< Underlying lno-tag to convert an address to a file-id. */
 Dee_size_t                       cwdft_fileid; /*< Last debug fileid ((Dee_size_t)-1 if no filenames loaded yet). */
 DEE_A_REF struct DeeTupleObject *cwdft_files;  /*< [0..1] Tuple of known filenames. */
};
#define DeeCodeWriterDebugFnoTab_Empty(ob) DeeCodeWriterDebugLnoTab_Empty(&(ob)->cwdft_lno)
#define DeeCodeWriterDebugFnoTab_INIT() {DeeCodeWriterDebugLnoTab_INIT(),(Dee_size_t)-1,NULL}
#define _DeeCodeWriterDebugFnoTab_Init(ob) \
do{\
 _DeeCodeWriterDebugLnoTab_Init(&(ob)->cwdft_lno);\
 (ob)->cwdft_fileid = (Dee_size_t)-1;\
 (ob)->cwdft_files = NULL;\
}while(0)
#define _DeeCodeWriterDebugFnoTab_Quit(ob) \
do{\
 _DeeCodeWriterDebugLnoTab_Quit(&(ob)->cwdft_lno);\
 Dee_XDECREF((ob)->cwdft_files);\
}while(0)

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugFnoTab_PutAddrFile(
 DEE_A_INOUT struct DeeCodeWriterDebugFnoTab *self,
 DEE_A_IN Dee_size_t addr, DEE_A_INOUT DeeStringObject *file) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_MAYBE_NULL char const *DeeCodeWriterDebugFnoTab_Addr2File(
 DEE_A_IN struct DeeCodeWriterDebugFnoTab const *self,
 DEE_A_IN Dee_size_t addr) DEE_ATTRIBUTE_NONNULL((1));


struct DeeCodeWriterDebugVarNames {
 DEE_A_REF struct DeeTupleObject *cwdvn_names; /*< [0..1] Tuple of known variable names. */
};
#define DeeCodeWriterDebugVarNames_Empty(ob) ((ob)->cwdvn_names == NULL)
#define DeeCodeWriterDebugVarNames_INIT() {NULL}
#define _DeeCodeWriterDebugVarNames_Init(ob)\
do{ (ob)->cwdvn_names = NULL; }while(0)
#define _DeeCodeWriterDebugVarNames_Quit(ob)\
do{ Dee_XDECREF((ob)->cwdvn_names); }while(0)


//////////////////////////////////////////////////////////////////////////
// Writes the given variable name to a given var id
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugVarNames_PutVarname(
 DEE_A_INOUT struct DeeCodeWriterDebugVarNames *self,
 DEE_A_IN Dee_size_t id, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_Z_OPT char const *DeeCodeWriterDebugVarNames_GetVarname(
 DEE_A_INOUT struct DeeCodeWriterDebugVarNames *self, DEE_A_IN Dee_size_t id) DEE_ATTRIBUTE_NONNULL((1));

struct DeeCodeWriterDebug {
 char const                       *cwd_file;   /*< [0..1] Last written filename. */
 Dee_int64_t                       cwd_line;   /*< Last debug line (sum of all ob_debug_lnov->e_lnoff entries). */
 struct DeeCodeWriterDebugLnoTab   cwd_lno;    /*< addr2line tab. */
 struct DeeCodeWriterDebugFnoTab   cwd_fno;    /*< addr2file tab. */
 struct DeeCodeWriterDebugVarNames cwd_locals; /*< Local variable names. */
 struct DeeCodeWriterDebugVarNames cwd_consts; /*< Static variable names. */
 struct DeeCodeWriterDebugVarNames cwd_refs;   /*< Reference variable names. */
 struct DeeCodeWriterDebugVarNames cwd_args;   /*< Argument variable names. */
};
#define DeeCodeWriterDebug_Empty(ob)\
(DeeCodeWriterDebugLnoTab_Empty(&(ob)->cwd_lno) &&\
 DeeCodeWriterDebugFnoTab_Empty(&(ob)->cwd_fno) &&\
 DeeCodeWriterDebugVarNames_Empty(&(ob)->cwd_locals) &&\
 DeeCodeWriterDebugVarNames_Empty(&(ob)->cwd_consts) &&\
 DeeCodeWriterDebugVarNames_Empty(&(ob)->cwd_refs) &&\
 DeeCodeWriterDebugVarNames_Empty(&(ob)->cwd_args))
#define DeeCodeWriterDebug_Addr2Line(ob,addr) DeeCodeWriterDebugLnoTab_Addr2Line(&(ob)->cwd_lno,addr)
#define DeeCodeWriterDebug_Addr2File(ob,addr) DeeCodeWriterDebugFnoTab_Addr2File(&(ob)->cwd_fno,addr)

extern DEE_A_RET_EXCEPT_REF DeeCodeDebugInfoObject *DeeCodeWriterDebug_Pack(
 DEE_A_INOUT struct DeeCodeWriterDebug *self,
 DEE_A_IN_OBJECT_OPT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebug_PutFileAndLine(
 DEE_A_INOUT struct DeeCodeWriterDebug *self, DEE_A_IN Dee_size_t addr,
 DEE_A_IN_Z_OPT char const *filename, DEE_A_IN Dee_int64_t line) DEE_ATTRIBUTE_NONNULL((1));

#define DeeCodeWriterDebug_INIT() {NULL,0,\
 DeeCodeWriterDebugLnoTab_INIT(),DeeCodeWriterDebugFnoTab_INIT(),\
 DeeCodeWriterDebugVarNames_INIT(),DeeCodeWriterDebugVarNames_INIT(),\
 DeeCodeWriterDebugVarNames_INIT(),DeeCodeWriterDebugVarNames_INIT()}
#define _DeeCodeWriterDebug_Init(ob)\
do{\
 (ob)->cwd_file = NULL; (ob)->cwd_line = 0;\
 _DeeCodeWriterDebugLnoTab_Init(&(ob)->cwd_lno);\
 _DeeCodeWriterDebugFnoTab_Init(&(ob)->cwd_fno);\
 _DeeCodeWriterDebugVarNames_Init(&(ob)->cwd_locals);\
 _DeeCodeWriterDebugVarNames_Init(&(ob)->cwd_consts);\
 _DeeCodeWriterDebugVarNames_Init(&(ob)->cwd_refs);\
 _DeeCodeWriterDebugVarNames_Init(&(ob)->cwd_args);\
}while(0)
#define _DeeCodeWriterDebug_Quit(ob)\
do{\
 _DeeCodeWriterDebugLnoTab_Quit(&(ob)->cwd_lno);\
 _DeeCodeWriterDebugFnoTab_Quit(&(ob)->cwd_fno);\
 _DeeCodeWriterDebugVarNames_Quit(&(ob)->cwd_locals);\
 _DeeCodeWriterDebugVarNames_Quit(&(ob)->cwd_consts);\
 _DeeCodeWriterDebugVarNames_Quit(&(ob)->cwd_refs);\
 _DeeCodeWriterDebugVarNames_Quit(&(ob)->cwd_args);\
}while(0)




struct DeeCodeWriter {
 struct DeeScopeObject               *cw_root_scope;          /*< [1..1] Root scope of the current code. */
 Dee_uint8_t                         *cw_code_pos;            /*< [0..1] Current write position in the code. */
 Dee_uint8_t                         *cw_code_begin;          /*< [0..1] Begin of the code. */
 Dee_uint8_t                         *cw_code_end;            /*< [0..1] End of the allocated code. */
 Dee_uint16_t                         cw_flags;               /*< Code flags (s.a.: DeeCodeObject::ob_flags). */
 Dee_size_t                           cw_constc;              /*< Used size of the constant vector. */
 Dee_size_t                           cw_consta;              /*< Allocated size of the constant vector. */
 DEE_A_REF DeeObject                **cw_constv;              /*< [1..1][0..ob_constc][owned] Vector of constants. */
 Dee_size_t                           cw_refc;                /*< Used size of the reference list. */
 Dee_size_t                           cw_refa;                /*< Allocated size of the reference list. */
 DEE_A_REF struct DeeLocalVarObject **cw_refv;                /*< [1..1][0..ob_refc][owned] List of references to external variables (index == runtime ref_id). */
 struct DeeCodeWriterFreeLocalList    cw_freelocals;
 Dee_size_t                           cw_stack_size;          /*< Current stack size. */
 Dee_size_t                           cw_stack_size_min;      /*< Min. stack size required to run the code. */
 struct DeeCodeWriterStackMemList     cw_stackmem;            /*< Used stackmemory. */
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t                           cw_except_size;         /*< Current amount of exception handlers. */
 Dee_size_t                           cw_except_size_min;     /*< Min. amount of exception handlers required to run the code. */
 struct DeeExceptionHandlerEntry     *cw_exceptv;             /*< [0..ob_exceptc][owned] List of known exception handlers templates. */
 Dee_size_t                           cw_exceptc;             /*< Amount of exceptions handlers. */
 Dee_size_t                           cw_finally_size;        /*< Current amount of recursive finally blocks. */
 Dee_size_t                           cw_finally_size_min;    /*< Min. amount of recursive finally blocks required to run the code. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 Dee_size_t                           cw_localc;              /*< Min. amount of locals required to run the code. */
 Dee_uint8_t                          cw_last_op;             /*< Last opcode that was written. */
 struct DeeCodeWriterDebug            cw_debug;               /*< Codewriter debug information. */
 //////////////////////////////////////////////////////////////////////////
 // Stuff for break + continue in loops
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t                           cw_loop_except_size;    /*< Amount of exception handlers when the loop starts/ends. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 Dee_size_t                           cw_loop_stack_size;     /*< Size of the stack then the loop starts/ends. */
 Dee_size_t                           cw_continue_addr;       /*< Address to jump to when continue is encountered (Dee_size_t)-1 if continue isn't allowed. */
 Dee_size_t                          *cw_break_forward_v;     /*< [0..ob_break_forward_c][owned] Positions in the code where we need to write the offset to jump to for break. */
 Dee_size_t                           cw_break_forward_c;     /*< Amount of forward uses of break. */
};

#define DeeCodeWriter_IsVarLocal(ob,var)\
 ((ob)->cw_root_scope==DeeScope_WEAK_ROOT(\
 ((DeeLocalVarObject *)Dee_REQUIRES_POINTER(var))->lv_scope))

// NOTE: Locals 0..n_args-1 are the slots for the function arguments
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define DeeCodeWriter_INIT() {NULL,NULL,NULL,NULL,0,0,0,NULL,0,0,NULL,DeeCodeWriterFreeLocalList_INIT(),0,0,DeeCodeWriterStackMemList_INIT(),0,0,NULL,0,0,0,0,(Dee_uint8_t)-1,DeeCodeWriterDebug_INIT(),0,0,(Dee_size_t)-1,NULL,0}
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
#define DeeCodeWriter_INIT() {NULL,NULL,NULL,NULL,0,0,0,NULL,0,0,NULL,DeeCodeWriterFreeLocalList_INIT(),0,0,DeeCodeWriterStackMemList_INIT(),0,(Dee_uint8_t)-1,DeeCodeWriterDebug_INIT(),0,(Dee_size_t)-1,NULL,0}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

#define DeeCodeWriter_ADDR(ob)         (Dee_size_t)((ob)->cw_code_pos-(ob)->cw_code_begin)
#define DeeCodeWriter_SETADDR(ob,addr)\
(DEE_ASSERTF(((ob)->cw_code_begin+(Dee_size_t)(addr))<\
 (ob)->cw_code_end,"Address %Iu is out of bounds",(Dee_size_t)(addr)),\
 (ob)->cw_code_pos=(ob)->cw_code_begin+(addr))
#define DeeCodeWriter_DECSTACK(ob)     (void)(DEE_ASSERT((ob)->cw_stack_size),--(ob)->cw_stack_size)
#define DeeCodeWriter_DECSTACK_N(ob,n) (void)(DEE_ASSERT((ob)->cw_stack_size>=(n)),(ob)->cw_stack_size-=(n))
#define DeeCodeWriter_INCSTACK(ob) (\
 (++(ob)->cw_stack_size > (ob)->cw_stack_size_min)\
 ?(void)((ob)->cw_stack_size_min = (ob)->cw_stack_size)\
 :(void)0)
#define DeeCodeWriter_INCSTACK_N(ob,n) (\
 (((ob)->cw_stack_size+=(n)) > (ob)->cw_stack_size_min)\
 ?(void)((ob)->cw_stack_size_min = (ob)->cw_stack_size)\
 :(void)0)
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define DeeCodeWriter_DECEXCEPTHANDLERS(ob) (void)(\
 DEE_ASSERT((ob)->cw_except_size),--(ob)->cw_except_size)
#define DeeCodeWriter_INCEXCEPTHANDLERS(ob) (\
 (++(ob)->cw_except_size > (ob)->cw_except_size_min)\
 ?(void)((ob)->cw_except_size_min = (ob)->cw_except_size)\
 :(void)0)
#endif

extern void DeeCodeWriter_Init(
 DEE_A_OUT struct DeeCodeWriter *self,
 DEE_A_IN struct DeeScopeObject *root_scope) DEE_ATTRIBUTE_NONNULL((1,2));
extern void DeeCodeWriter_Quit(DEE_A_IN struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));


#define DeeCodeWriter_Addr2Line(ob,addr)  DeeCodeWriterDebug_Addr2Line(&(ob)->cw_debug,addr)
#define DeeCodeWriter_Addr2File(ob,addr)  DeeCodeWriterDebug_Addr2File(&(ob)->cw_debug,addr)
#define DeeCodeWriter_Debug(ob,file,line) DeeCodeWriterDebug_PutFileAndLine(&(ob)->cw_debug,DeeCodeWriter_ADDR(ob),file,line)

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Optimize(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1));

struct DeeCodeReachableAddrList;
//////////////////////////////////////////////////////////////////////////
// Perform peephole optimizations
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DoPeepholeOptimization(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT Dee_uint32_t *performed_optimizations) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DoPeepholeOptimizationWithProtectedAddrList(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT Dee_uint32_t *performed_optimizations,
 DEE_A_IN struct DeeCodeReachableAddrList const *protected_addr) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Remove all no-op opcodes and fix addresses that were affected
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RemoveNoopOpcodes(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Packs a code writer, generating and returning a new code object.
// Note, that every code writer may only be packed once if correct behavior
// is wanted. Packing a writer twice or even more often may yield unwanted side-effects
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeCodeObject) *DeeCodeWriter_Pack(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN_OBJECT_OPT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Label functionality
// NOTE: These two methods return -1 and set a CompilerError if a jump is illegal
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_PutLabel(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_INOUT struct DeeLexerObject *lexer, DEE_A_INOUT struct DeeTokenObject *name_token) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_GotoLabel(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_INOUT struct DeeLexerObject *lexer, DEE_A_INOUT struct DeeTokenObject *name_token) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_GotoForwardLabelEx(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_IN Dee_uint8_t jmp_op, DEE_A_INOUT struct DeeTokenObject *name_token) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Finalizes all given labels, fixing undefined jumps and emitting warnings for them
// NOTE: Called from 
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_FinalizeLabels(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeParserLabelList const *labels,
 DEE_A_INOUT struct DeeLexerObject *lexer) DEE_ATTRIBUTE_NONNULL((1,2,3));

extern DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_WriteData(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOp(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithArg(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_uint16_t arg) DEE_ATTRIBUTE_NONNULL((1));

#if 0
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithFutureArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op,
 DEE_A_OUT Dee_size_t *arg_addr) DEE_ATTRIBUTE_NONNULL((1,3));
#define DeeCodeWriter_SetFutureArg(self,addr,v) \
 (void)(*((Dee_uint16_t*)((self)->cw_code_begin+(addr))) = (v))
#endif

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithFutureSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op,
 DEE_A_OUT Dee_size_t *arg_addr) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_SetFutureSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t arg_addr, DEE_A_IN Dee_size_t arg) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_SetFutureSSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t arg_addr, DEE_A_IN Dee_ssize_t arg) DEE_ATTRIBUTE_NONNULL((1));

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithSizeArg(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_size_t arg) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithSSizeArg(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_ssize_t arg) DEE_ATTRIBUTE_NONNULL((1));
#define DeeCodeWriter_WriteStackAlign(ob,stack_size) DeeCodeWriter_WriteOpWithSizeArg(ob,OP_STACKALIGN,stack_size)

extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocConst(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *v, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocStatic(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *init, DEE_A_IN_Z_OPT char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocLocal(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN_Z_OPT char const *name) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocOrGetReference(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeLocalVarObject *referenced_var, DEE_A_IN_Z_OPT char const *name) DEE_ATTRIBUTE_NONNULL((1,2));

#define DeeCodeWriter_AddLocalName(ob,id,name) DeeCodeWriterDebugVarNames_PutVarname(&(ob)->cw_debug.cwd_locals,id,name)
#define DeeCodeWriter_AddConstName(ob,id,name) DeeCodeWriterDebugVarNames_PutVarname(&(ob)->cw_debug.cwd_consts,id,name)
#define DeeCodeWriter_AddRefName(ob,id,name)   DeeCodeWriterDebugVarNames_PutVarname(&(ob)->cw_debug.cwd_refs,id,name)
#define DeeCodeWriter_AddArgName(ob,id,name)   DeeCodeWriterDebugVarNames_PutVarname(&(ob)->cw_debug.cwd_args,id,name)
#define DeeCodeWriter_GetLocalName(ob,id)      DeeCodeWriterDebugVarNames_GetVarname(&(ob)->cw_debug.cwd_locals,id)
#define DeeCodeWriter_GetConstName(ob,id)      DeeCodeWriterDebugVarNames_GetVarname(&(ob)->cw_debug.cwd_consts,id)
#define DeeCodeWriter_GetRefName(ob,id)        DeeCodeWriterDebugVarNames_GetVarname(&(ob)->cw_debug.cwd_refs,id)
#define DeeCodeWriter_GetArgName(ob,id)        DeeCodeWriterDebugVarNames_GetVarname(&(ob)->cw_debug.cwd_args,id)

// Generate variable class operations (NOTE: operations illegal in certain classes are asserted)
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DelVar(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LoadVar(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var, DEE_A_IN_OPT struct DeeLexerObject const *lexer) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreVar(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var, DEE_A_IN_OPT struct DeeLexerObject const *lexer) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreVarPop(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var, DEE_A_IN_OPT struct DeeLexerObject const *lexer) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeCodeWriter_LoadThis(ob)         (DeeCodeWriter_INCSTACK(ob),DeeCodeWriter_WriteOp(ob,OP_LOAD_THIS))
#define DeeCodeWriter_LoadRet(ob)          (DeeCodeWriter_INCSTACK(ob),DeeCodeWriter_WriteOp(ob,OP_LOAD_RET))
#define DeeCodeWriter_LoadArgID(ob,id)     (DeeCodeWriter_INCSTACK(ob),DeeCodeWriter_WriteOpWithSizeArg(ob,OP_LOAD_ARG,id))
#define DeeCodeWriter_LoadRefID(ob,id)     (DeeCodeWriter_INCSTACK(ob),DeeCodeWriter_WriteOpWithSizeArg(ob,OP_LOAD_REF,id))
#define DeeCodeWriter_LoadLocID(ob,id)     (DeeCodeWriter_INCSTACK(ob),DeeCodeWriter_WriteOpWithSizeArg(ob,OP_LOAD_LOC,id))
#define DeeCodeWriter_StoreRet(ob)          DeeCodeWriter_WriteOp(ob,OP_STORE_RET)
#define DeeCodeWriter_StoreLocID(ob,id)     DeeCodeWriter_WriteOpWithSizeArg(ob,OP_STORE_LOC,id)
#define DeeCodeWriter_StoreRetPop(ob)      (DeeCodeWriter_DECSTACK(ob),DeeCodeWriter_WriteOp(ob,OP_STORE_RET_POP))
#define DeeCodeWriter_StoreLocPopID(ob,id) (DeeCodeWriter_DECSTACK(ob),DeeCodeWriter_WriteOpWithSizeArg(ob,OP_STORE_LOC_POP,id))

//////////////////////////////////////////////////////////////////////////
// 1. Pop stack(stack_id) from the tack
// 2. Pushes stack(stack_id) ontop of the tack
// 3. Stores TOP in stack(stack_id)
// 4. Replaces stack(stack_id) with TOP and pop 1
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DelStack(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LoadStack(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreStack(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreStackPop(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) DEE_ATTRIBUTE_NONNULL((1));


#define DeeCodeWriter_UnaryOp             DeeCodeWriter_WriteOp
#define DeeCodeWriter_BinaryOp(ob,op)    (DeeCodeWriter_WriteOp(ob,op)!=0 ? -1 : (DeeCodeWriter_DECSTACK(ob),0))
#define DeeCodeWriter_TrinaryOp(ob,op)   (DeeCodeWriter_WriteOp(ob,op)!=0 ? -1 : (DeeCodeWriter_DECSTACK_N(ob,2),0))
#define DeeCodeWriter_QuadaryOp(ob,op)   (DeeCodeWriter_WriteOp(ob,op)!=0 ? -1 : (DeeCodeWriter_DECSTACK_N(ob,3),0))
#define DeeCodeWriter_PushNone(ob)       (DeeCodeWriter_WriteOp(ob,OP_LOAD_NONE)!=0 ? -1 : (DeeCodeWriter_INCSTACK(ob),0))
#define DeeCodeWriter_Pop(ob)            (DeeCodeWriter_WriteOp(ob,OP_POP)!=0 ? -1 : (DeeCodeWriter_DECSTACK(ob),0))
#define DeeCodeWriter_Dup(ob)            (DeeCodeWriter_WriteOp(ob,OP_DUP)!=0 ? -1 : (DeeCodeWriter_INCSTACK(ob),0))
#define DeeCodeWriter_JmpAbs(ob,dst_addr) DeeCodeWriter_WriteOpWithSSizeArg(ob,OP_JUMP,(Dee_ssize_t)((dst_addr)-DeeCodeWriter_ADDR(ob)))
#define DeeCodeWriter_Unreachable(ob)   ((ob)->cw_last_op != OP_UNREACHABLE ? DeeCodeWriter_WriteOp(ob,OP_UNREACHABLE) : 0)
#define DeeCodeWriter_Yield(ob)          (DeeCodeWriter_WriteOp(ob,OP_YIELD)!=0 ? -1 : (DeeCodeWriter_DECSTACK(ob),0))
#define DeeCodeWriter_YieldAll(ob)       (DeeCodeWriter_WriteOp(ob,OP_YIELDALL)!=0 ? -1 : (DeeCodeWriter_DECSTACK(ob),0))


extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_PushConst(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *v, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LRot(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RRot(DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));

extern DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_PushConstEx(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t mode,
 DEE_A_INOUT DeeObject *v, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,3));
#define DeeCodeWriter_PushConstNoCopy(ob,v,compiler_flags) \
 _DeeCodeWriter_PushConstEx(ob,OP_LOAD_CST,v,compiler_flags)


#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_TryBegin(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_OUT Dee_size_t *entry) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_TryEnd(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeCodeWriter_TryBeginFinally(ob,entry) \
 (DeeCodeWriter_INC_FINALLYSIZE(ob),DeeCodeWriter_TryBegin(ob,entry))
#define DeeCodeWriter_INC_FINALLYSIZE(ob)\
 ((ob)->cw_finally_size++ == (ob)->cw_finally_size_min\
  ? (void)((ob)->cw_finally_size_min = (ob)->cw_finally_size) : (void)0)
#define DeeCodeWriter_DEC_FINALLYSIZE(ob)     (void)(DEE_ASSERT((ob)->cw_finally_size),--(ob)->cw_finally_size)
#define DeeCodeWriter_INC_FINALLYSIZE_N(ob,n)\
 (((ob)->cw_finally_size += (n)) >= (ob)->cw_finally_size_min\
  ? (void)((ob)->cw_finally_size_min = (ob)->cw_finally_size) : (void)0)
#define DeeCodeWriter_DEC_FINALLYSIZE_N(ob,n) (void)(DEE_ASSERT((ob)->cw_finally_size >= (n)),(ob)->cw_finally_size -= (n))

#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

//////////////////////////////////////////////////////////////////////////
// Pop the top slot and return it as the code return value (--)
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Ret(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Write code to cleanup local variables from "scope"
#define DeeCodeWriter_EnterStrongScope(self,scope,lexer,compiler_flags) \
 (/*(void)(lexer,compiler_flags),*/_DeeCodeWriter_EnterStrongScope_impl(self,scope))
extern DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_EnterStrongScope_impl(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeScopeObject *scope) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LeaveStrongScope(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeScopeObject *scope,
 DEE_A_IN struct DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LeaveWeakScope(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeScopeObject *scope,
 DEE_A_IN struct DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t compiler_flags) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Break / Continue integration
#define DeeCodeWriter_HasBreak(ob) ((ob)->cw_continue_addr!=(Dee_size_t)-1)
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Break(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Continue(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Return none
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RetNone(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_YieldExit(
 DEE_A_INOUT struct DeeCodeWriter *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Pop "s" items and pack them into a tuple / list (-=s;++)
extern DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_MakeSequence(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
#define DeeCodeWriter_MakeTuple(ob,s) _DeeCodeWriter_MakeSequence(ob,OP_TUPLE,s)
#define DeeCodeWriter_MakeList(ob,s)  _DeeCodeWriter_MakeSequence(ob,OP_LIST,s)
#define DeeCodeWriter_MakeSet(ob,s)   _DeeCodeWriter_MakeSequence(ob,OP_SET,s)


//////////////////////////////////////////////////////////////////////////
// Implementation of 'continue;' and 'break;' in loops
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define DeeCodeWriter_PUSH_LOOP(writer,continue_addr)\
do{Dee_size_t \
 _old_continue_addr = (writer)->cw_continue_addr,\
*_old_break_forward_v = (writer)->cw_break_forward_v,\
 _old_break_forward_c = (writer)->cw_break_forward_c,\
 _old_loop_stack_size = (writer)->cw_loop_stack_size;\
 Dee_size_t _old_loop_except_size = (writer)->cw_loop_except_size;\
 (writer)->cw_break_forward_v = NULL;\
 (writer)->cw_break_forward_c = 0;\
 (writer)->cw_continue_addr = (continue_addr);\
 (writer)->cw_loop_except_size = (writer)->cw_except_size;\
 (writer)->cw_loop_stack_size = (writer)->cw_stack_size
#define DeeCodeWriter_FAIL_LOOP(writer) (\
 free((writer)->cw_break_forward_v),\
 (writer)->cw_loop_except_size = _old_loop_except_size,\
 (writer)->cw_loop_stack_size = _old_loop_stack_size,\
 (writer)->cw_continue_addr = _old_continue_addr,\
 (writer)->cw_break_forward_v = _old_break_forward_v,\
 (writer)->cw_break_forward_c = _old_break_forward_c)
#define DeeCodeWriter_POP_LOOP(writer,break_addr,...)\
{Dee_size_t *_forward_begin,*_forward_end,_break_addr = (break_addr);\
 _forward_end = (_forward_begin = (writer)->cw_break_forward_v)+(writer)->cw_break_forward_c;\
 while (_forward_begin != _forward_end) {\
  if (DeeCodeWriter_SetFutureSizeArg(writer,*_forward_begin,(\
   Dee_size_t)(_break_addr-*_forward_begin)+1) != 0) {\
   free((writer)->cw_break_forward_v);\
   {__VA_ARGS__;}\
  }\
  ++_forward_begin;\
 }\
 free((writer)->cw_break_forward_v);\
 (writer)->cw_loop_except_size = _old_loop_except_size;\
 (writer)->cw_loop_stack_size = _old_loop_stack_size;\
 (writer)->cw_continue_addr = _old_continue_addr;\
 (writer)->cw_break_forward_v = _old_break_forward_v;\
 (writer)->cw_break_forward_c = _old_break_forward_c;\
}}while(0)
#else /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
#define DeeCodeWriter_PUSH_LOOP(writer,continue_addr)\
do{Dee_size_t \
 _old_continue_addr = (writer)->cw_continue_addr,\
*_old_break_forward_v = (writer)->cw_break_forward_v,\
 _old_break_forward_c = (writer)->cw_break_forward_c,\
 _old_loop_stack_size = (writer)->cw_loop_stack_size;\
 (writer)->cw_break_forward_v = NULL;\
 (writer)->cw_break_forward_c = 0;\
 (writer)->cw_continue_addr = (continue_addr);\
 (writer)->cw_loop_stack_size = (writer)->cw_stack_size
#define DeeCodeWriter_FAIL_LOOP(writer) (\
 free((writer)->cw_break_forward_v),\
 (writer)->cw_loop_stack_size = _old_loop_stack_size,\
 (writer)->cw_continue_addr = _old_continue_addr,\
 (writer)->cw_break_forward_v = _old_break_forward_v,\
 (writer)->cw_break_forward_c = _old_break_forward_c)
#define DeeCodeWriter_POP_LOOP(writer,break_addr,...)\
{Dee_size_t *_forward_begin,*_forward_end,_break_addr = (break_addr);\
 _forward_end = (_forward_begin = (writer)->cw_break_forward_v)+(writer)->cw_break_forward_c;\
 while (_forward_begin != _forward_end) {\
  if (DeeCodeWriter_SetFutureSizeArg(writer,*_forward_begin,(\
   Dee_size_t)(_break_addr-*_forward_begin)+1) != 0) {\
   free((writer)->cw_break_forward_v);\
   {__VA_ARGS__;}\
  }\
  ++_forward_begin;\
 }\
 (writer)->cw_loop_stack_size = _old_loop_stack_size;\
 (writer)->cw_continue_addr = _old_continue_addr;\
 (writer)->cw_break_forward_v = _old_break_forward_v;\
 (writer)->cw_break_forward_c = _old_break_forward_c;\
}}while(0)
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */


//////////////////////////////////////////////////////////////////////////
// Begin(+)/end(-) an __atomic_once block
// [+] >> {
// [+] >>   static __block_executed = atomic uint8_t(0);
// [+] >>#if (flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0
// [+] >>__begin:
// [+] >>#endif
// [+] >>   if (__block_executed.compare_exchange_bool(0,1)) {
// [+] >>#if (flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0
// [+] >>     try {
// [+] >>#endif
// [ ] >>       ...
// [-] >>#if (flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0
// [-] >>     } catch (...) {
// [-] >>       __block_executed.store(0);
// [-] >>       throw;
// [-] >>     }
// [-] >>#endif
// [-] >>     __block_executed.store(2);
// [-] >>   } else {
// [-] >>     while (true) {
// [-] >>       __stack local __state = __block_executed.load();
// [-] >>       if (state == 2) break;
// [-] >>#if (flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0
// [-] >>       if (state == 0) goto __begin;
// [-] >>#endif
// [-] >>       thread.yield();
// [-] >>     }
// [-] >>   }
// [-] >> }
#define DEE_CODEWRITER_ATOMICONCE_FLAG_NONE     DEE_UINT32_C(0x00000000)
#define DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT DEE_UINT32_C(0x00000001)
struct DeeCodeWriterAtomicOnceBlock {
 Dee_uint32_t ao_flags;            /*< Atomic once flags. */
 Dee_size_t   ao_executed;         /*< ID for '__block_executed' */
 Dee_size_t   ao_catchid;          /*< ID for the catch-all handler (unused if noexcept) */
 Dee_size_t   ao_begin_addr;       /*< address for 'goto __begin;' (unused if noexcept) */
 Dee_size_t   ao_shoutinit_jmparg; /*< address of the jump argument for the initialization check. */
};

// Runtime states of the '__block_executed' variable
#define DEE_CODEWRITER_ATOMICONCE_STATE_NEVER 0 /*< Block was never executed. */
#define DEE_CODEWRITER_ATOMICONCE_STATE_NOW   1 /*< Block is currently executing. */
#define DEE_CODEWRITER_ATOMICONCE_STATE_DONE  2 /*< Block was successfully executed. */

extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_AtomicOnceBegin(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_OUT struct DeeCodeWriterAtomicOnceBlock *block,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN Dee_uint32_t compiler_flags);
extern DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_AtomicOnceEnd(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN struct DeeCodeWriterAtomicOnceBlock const *block,
 DEE_A_IN Dee_uint32_t compiler_flags);




//////////////////////////////////////////////////////////////////////////
// List of reachable addresses (used during and required for peephole optimizations)
// in "codewriter.peephole.inl"
struct DeeCodeReachableAddrList {
 Dee_size_t  ra_begin; /*< Lowest known address. */
 Dee_size_t  ra_end;   /*< Greatest known address. */
 Dee_size_t  ra_listc; /*< Amount of reachable address entries. */
 Dee_size_t  ra_lista; /*< Allocated list size. */
 Dee_size_t *ra_listv; /*< [0..ra_listc] Ascendingly sorted list of reachable addresses */
};
#define DeeCodeReachableAddrList_INIT()   {(Dee_size_t)-1,0,0,0,NULL}
#define _DeeCodeReachableAddrList_Init(ob) \
do{\
 (ob)->ra_begin = (Dee_size_t)-1;\
 (ob)->ra_end = 0;\
 (ob)->ra_listc = 0;\
 (ob)->ra_lista = 0;\
 (ob)->ra_listv = NULL;\
}while(0)
#define DeeCodeReachableAddrList_Quit(ob) free((ob)->ra_listv)

// Returns true if 'addr' is contained
extern DEE_A_RET_WUNUSED int DeeCodeReachableAddrList_Contains(
 DEE_A_IN struct DeeCodeReachableAddrList const *self, DEE_A_IN Dee_size_t addr);

// Returns true if any address '>= addr_begin' and '< addr_end' is contained
extern DEE_A_RET_WUNUSED int DeeCodeReachableAddrList_ContainsAny(
 DEE_A_IN struct DeeCodeReachableAddrList const *self,
 DEE_A_IN Dee_size_t addr_begin, DEE_A_IN Dee_size_t addr_end);

// Inserts a new address in to the sorted list
extern DEE_A_RET_EXCEPT(-1) int DeeCodeReachableAddrList_Insert(
 DEE_A_INOUT struct DeeCodeReachableAddrList *self, DEE_A_IN Dee_size_t addr);

//////////////////////////////////////////////////////////////////////////
// Collect all addresses reachable through jumps from the given writer
extern DEE_A_RET_EXCEPT(-1) int DeeCodeReachableAddrList_CollectFromCodewriter(
 DEE_A_INOUT struct DeeCodeReachableAddrList *self,
 DEE_A_IN struct DeeCodeWriter const *writer);


//////////////////////////////////////////////////////////////////////////
// Generate code for wrapping a try-finally block, where
// the try-block if noreturn, and the finally block is noexcept:
// >> DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_TRY(writer,goto err) {
// >>   compile_try_block();
// >> } DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_FLN(writer,goto err) {
// >>   compile_finally_block();
// >> } DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_END(writer,goto err);
#define DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_TRY(writer,...)\
do{ Dee_size_t _cw_block_id;\
 if (DeeCodeWriter_TryBeginFinally(writer,&_cw_block_id) != 0) {__VA_ARGS__;}\
 do
#define DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_FLN(writer,...)\
while(0);\
 if (DeeCodeWriter_TryEnd(writer) != 0) return -1;\
do{\
 struct DeeExceptionHandlerEntry *_entry = (writer)->cw_exceptv+_cw_block_id;\
 DeeExceptionHandlerEntry_InitFinally(_entry,writer);\
}while(0);\
 do
#define DeeCodeWriter_FINALLY_NORETURNTRY_NOEXCEPTFLN_END(writer,...)\
while(0); if (DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) {__VA_ARGS__;} }while(0)


//////////////////////////////////////////////////////////////////////////
// Generate code for wrapping a try-finally block, where the try-block if noreturn:
// >> DeeCodeWriter_FINALLY_NORETURNTRY_TRY(writer,goto err) {
// >>   compile_try_block();
// >> } DeeCodeWriter_FINALLY_NORETURNTRY_FLN(writer,goto err) {
// >>   compile_finally_block();
// >> } DeeCodeWriter_FINALLY_NORETURNTRY_END(writer,goto err);
#define DeeCodeWriter_FINALLY_NORETURNTRY_TRY(writer,...)\
do{ Dee_size_t _cw_block_id,_cw_finally_id;\
 if (DeeCodeWriter_TryBeginFinally(writer,&_cw_block_id) != 0) {__VA_ARGS__;}\
 do
#define DeeCodeWriter_FINALLY_NORETURNTRY_FLN(writer,...)\
while(0);\
 if (DeeCodeWriter_TryEnd(writer) != 0) {__VA_ARGS__;}\
do{\
 struct DeeExceptionHandlerEntry *_entry = (writer)->cw_exceptv+_cw_block_id;\
 DeeExceptionHandlerEntry_InitFinally(_entry,writer);\
}while(0);\
 if (DeeCodeWriter_TryBeginFinally(writer,&_cw_finally_id) != 0) {__VA_ARGS__;}\
 do
#define DeeCodeWriter_FINALLY_NORETURNTRY_END(writer,...)\
while(0); do{\
 struct DeeExceptionHandlerEntry *_entry = (writer)->cw_exceptv+_cw_finally_id;\
 DeeExceptionHandlerEntry_InitFinally(_entry,writer);\
}while(0);\
 if (DeeCodeWriter_TryEnd(writer) != 0 ||\
     DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0 ||\
     DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) {__VA_ARGS__;}\
 DeeCodeWriter_DEC_FINALLYSIZE_N(writer,2);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Generate code for wrapping a try-finally block:
// >> DeeCodeWriter_FINALLY_TRY(writer,goto err) {
// >>   compile_try_block();
// >> } DeeCodeWriter_FINALLY_FLN(writer,goto err) {
// >>   compile_finally_block();
// >> } DeeCodeWriter_DEC_FINALLYSIZE(writer,goto err);
#define DeeCodeWriter_FINALLY_TRY(writer,...)\
do{ Dee_size_t _cw_block_id,_cw_finally_id,_cw_finally_jmp;\
 if (DeeCodeWriter_TryBeginFinally(writer,&_cw_block_id) != 0) {__VA_ARGS__;}\
 do
#define DeeCodeWriter_FINALLY_FLN(writer,...)\
while(0);\
 if (DeeCodeWriter_TryEnd(writer) != 0) {__VA_ARGS__;}\
do{\
 struct DeeExceptionHandlerEntry *_entry = (writer)->cw_exceptv+_cw_block_id;\
 DeeExceptionHandlerEntry_InitFinally(_entry,writer);\
}while(0);\
 if (DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&_cw_finally_jmp) != 0) {__VA_ARGS__;}\
 if (DeeCodeWriter_TryBeginFinally(writer,&_cw_finally_id) != 0) {__VA_ARGS__;}\
 do
#define DeeCodeWriter_FINALLY_END(writer,...)\
while(0); do{\
 struct DeeExceptionHandlerEntry *_entry = (writer)->cw_exceptv+_cw_finally_id;\
 DeeExceptionHandlerEntry_InitFinally(_entry,writer);\
}while(0);\
 if (DeeCodeWriter_TryEnd(writer) != 0 ||\
     DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0 ||\
     DeeCodeWriter_SetFutureSizeArg(writer,_cw_finally_jmp,\
      DeeCodeWriter_ADDR(writer)-(_cw_finally_jmp-sizeof(Dee_uint8_t))) != 0 || \
     DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) {__VA_ARGS__;}\
 DeeCodeWriter_DEC_FINALLYSIZE_N(writer,2);\
}while(0)


DEE_DECL_END

#endif /* !GUARD_DEEMON_CODEWRITER_H */
