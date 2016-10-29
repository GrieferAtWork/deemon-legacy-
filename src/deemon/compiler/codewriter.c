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
#ifndef GUARD_DEEMON_CODEWRITER_C
#define GUARD_DEEMON_CODEWRITER_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// include/*
#include <deemon/cell.h>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/deemonrun.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/set.h>
#include <deemon/tuple.h>
#include <deemon/value.h>

// src/*
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/codewriter.h>
#include <deemon/compiler/scope.h>
#include DEE_INCLUDE_MEMORY_API()


// lib/*
#include <deemon/optional/std/string.h> // memcpy

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stdint.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H */



DEE_DECL_BEGIN

#define DEE_CODE_SIZE(code_size) (Dee_OFFSETOF(DeeCodeObject,co_code)+(code_size))

DEE_A_RET_EXCEPT(-1) int DeeCodeWriterFreeLocalList_Append(
 DEE_A_INOUT struct DeeCodeWriterFreeLocalList *self, DEE_A_IN Dee_size_t id) {
 Dee_size_t *new_list; Dee_size_t new_size;
 DEE_ASSERT(self);
 DEE_ASSERT(!self->cwfl_c || self->cwfl_v);
#ifdef DEE_DEBUG
 new_list = self->cwfl_v+self->cwfl_c;
 while (new_list != self->cwfl_v) {
  --new_list;
  DEE_ASSERTF(*new_list != id,"Id %I16u was already marked as free",id);
 }
#endif
 if DEE_UNLIKELY(self->cwfl_c == self->cwfl_a) {
  if DEE_UNLIKELY((new_size = self->cwfl_a) == 0) new_size = 2; else new_size *= 2;
  while DEE_UNLIKELY((new_list = (Dee_size_t *)realloc_nz(
   self->cwfl_v,new_size*sizeof(Dee_size_t))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->cwfl_v = new_list;
  self->cwfl_a = new_size;
 }
 self->cwfl_v[self->cwfl_c++] = id;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriterStackMemList_IncBuf(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self) {
 Dee_size_t new_bufsize;
 struct DeeCodeWriterStackMem *new_buf;
 DEE_ASSERT(self);
 if (!self->cwsml_mema) new_bufsize = 2;
 else new_bufsize = self->cwsml_mema*2;
 while DEE_UNLIKELY((new_buf = (struct DeeCodeWriterStackMem *)realloc_nz(
  self->cwsml_memv,new_bufsize*sizeof(struct DeeCodeWriterStackMem))) == NULL) {
  if (Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->cwsml_memv = new_buf;
 self->cwsml_mema = new_bufsize;
 return 0;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriterStackMemList_AllocBlock(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self, DEE_A_IN Dee_size_t s) {
 struct DeeCodeWriterStackMem *iter,*end; Dee_size_t insfree,curpos;
 DEE_ASSERT(self);
 if (!s) return 0; // Nothing to do here...
 // Search for free blocks big enough to fit 's'
 end = (iter = self->cwsml_memv)+self->cwsml_memc;
 curpos = 0;
 while (iter != end) {
  DEE_ASSERTF(iter->cwsm_start >= curpos,
              "Order of stackdata blocks is broken");
  insfree = iter->cwsm_start-curpos;
  if (insfree >= s) {
   // Insert the new block here!
   if (self->cwsml_memc == self->cwsml_mema &&
       DeeCodeWriterStackMemList_IncBuf(self) != 0) return (Dee_size_t)-1;
   memmove(iter+1,iter,(Dee_size_t)(end-iter)*sizeof(DeeCodeWriterStackMem));
   iter->cwsm_start = curpos;
   iter->cwsm_size = s;
   ++self->cwsml_memc;
   return curpos;
  }
  curpos = iter->cwsm_start+iter->cwsm_size;
  ++iter;
 }
 // Fallback: Allocate a new block at the end
 if (self->cwsml_memc == self->cwsml_mema &&
     DeeCodeWriterStackMemList_IncBuf(self) != 0) return (Dee_size_t)-1;
 iter = self->cwsml_memv+self->cwsml_memc++;
 iter->cwsm_start = self->cwsml_cursize;
 iter->cwsm_size = s;
 // Patch the current and maximum size
 if ((self->cwsml_cursize += s) > self->cwsml_maxsize)
  self->cwsml_maxsize = self->cwsml_cursize;
 return iter->cwsm_start;
}

void DeeCodeWriterStackMemList_FreeBlock(
 DEE_A_INOUT struct DeeCodeWriterStackMemList *self,
 DEE_A_IN Dee_size_t addr) {
 struct DeeCodeWriterStackMem *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERTF(self->cwsml_memc != 0,"No allocated blocks");
 end = self->cwsml_memv+self->cwsml_memc;
 iter = self->cwsml_memv+(Dee_size_t)(
  (double)self->cwsml_memc*(
  (double)addr/(double)self->cwsml_cursize));
      if (iter->cwsm_start > addr) { do --iter,DEE_ASSERT(iter >= self->cwsml_memv); while (iter->cwsm_start > addr); }
 else if (iter->cwsm_start < addr) { do ++iter,DEE_ASSERT(iter < end); while (iter->cwsm_start < addr); }
 else DEE_ASSERT(iter->cwsm_start == addr);
 --self->cwsml_memc;
 if (iter == end-1) {
  // Highest block was deleted --> update the current size
  if (!self->cwsml_memc) self->cwsml_cursize = 0;
  else self->cwsml_cursize = iter[-1].cwsm_start+iter[-1].cwsm_size;
 } else {
  // Block in the middle deleted --> relocate the list of declarations
  memmove(iter,iter+1,(Dee_size_t)((end-iter)-1)*
          sizeof(struct DeeCodeWriterStackMem));
 }
}




DEE_A_RET_WUNUSED Dee_int64_t DeeCodeWriterDebugLnoTab_Addr2Line(
 DEE_A_IN struct DeeCodeWriterDebugLnoTab const *self, DEE_A_IN Dee_size_t addr) {
 struct _DeeCodeLnOffList *lno;
 Dee_int64_t result; Dee_size_t coff;
 DEE_ASSERT(self);
 if (!self->cwdlt_lnoc) return -1;
 result = 0,coff = 0;
 lno = self->cwdlt_lnov;
 while (!_DeeCodeLnOffList_EMPTY(lno)) {
  coff += lno->lno_addroff;
  if (coff >= addr) break;
  result += lno->lno_lineoff;
  ++lno;
 }
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugLnoTab_IncBuf(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self) {
 struct _DeeCodeLnOffList *lno; Dee_size_t new_lno_size;
 DEE_ASSERT(self);
 if DEE_UNLIKELY((new_lno_size = self->cwdlt_lnoa*2) == 0) new_lno_size = 2;
 // NOTE: We overallocate by 1 to already make space for the eventual end-marker
 if DEE_UNLIKELY((lno = (struct _DeeCodeLnOffList *)realloc_nz(
  self->cwdlt_lnov,(new_lno_size+1)*sizeof(struct _DeeCodeLnOffList))) == NULL) {
  DeeError_NoMemory();
  return -1;
 }
 self->cwdlt_lnov = lno;
 self->cwdlt_lnoa = new_lno_size;
 return 0;
}


void DeeCodeWriterDebugLnoTab_Relocate(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self,
 DEE_A_IN Dee_size_t start, DEE_A_IN Dee_size_t size) {
 // Relocate debug line-number information
 Dee_size_t new_lno_addr,lno_addr,lno_offset_fix;
 struct _DeeCodeLnOffList *lno_iter,*lno_end;
 DEE_ASSERT(self);
 lno_offset_fix = size; lno_addr = 0;
 lno_end = (lno_iter = self->cwdlt_lnov)+self->cwdlt_lnoc;
 while (lno_iter != lno_end) {
  new_lno_addr = lno_addr+lno_iter->lno_addroff;
  if (lno_addr <= start && new_lno_addr > start) {
   Dee_uint8_t overflow = (Dee_uint8_t)(new_lno_addr-start);
   if ((Dee_size_t)overflow > size) overflow = (Dee_uint8_t)size;
   // Lno entry ends in this block (truncate it to its start)
   lno_iter->lno_addroff -= overflow;
   lno_offset_fix -= overflow;
   if (!lno_offset_fix) break;
  } else if (new_lno_addr > start+size) {
   // Lno entry describes code after the deleted block
   if (lno_offset_fix < lno_iter->lno_addroff) {
    lno_iter->lno_addroff -= (Dee_uint8_t)size;
    break; // Everything is now re-aligned
   }
   lno_offset_fix -= lno_iter->lno_addroff;
   lno_iter->lno_addroff = 0;
  }
  lno_addr = new_lno_addr;
  ++lno_iter;
 }
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugLnoTab_PutAddrLnoff(
 DEE_A_INOUT struct DeeCodeWriterDebugLnoTab *self,
 DEE_A_IN Dee_size_t addr, DEE_A_IN Dee_int64_t lnoff) {
 struct _DeeCodeLnOffList *lno; Dee_size_t addroff;
 DEE_ASSERT(self);
 addroff = addr-self->cwdlt_addr;
 if (!addroff && self->cwdlt_lnoc)
  return 0; // Don't put debug information if nothing changed and this isn't the first info
 self->cwdlt_addr = addr;
 // Add a new lno entry
 while (1) {
  if (self->cwdlt_lnoc == self->cwdlt_lnoa &&
      DeeCodeWriterDebugLnoTab_IncBuf(self) != 0) return -1;
  lno = self->cwdlt_lnov+self->cwdlt_lnoc++;
  // Fill in the new lno tab
  if (lnoff > INT8_MAX) {
   lno->lno_lineoff = INT8_MAX;
   lnoff -= INT8_MAX;
  } else if (lnoff < INT8_MIN) {
   lno->lno_lineoff = INT8_MIN;
   lnoff += -(INT8_MIN);
  } else {
   lno->lno_lineoff = (Dee_int8_t)lnoff;
   lnoff = 0;
  }
  if (addroff > DEE_UINT8_C(0xFF)) {
   lno->lno_addroff = DEE_UINT8_C(0xFF);
   addroff -= 0xFF;
  } else {
   lno->lno_addroff = (Dee_uint8_t)addroff;
   addroff = 0;
  }
  if (!lnoff && !addroff) break;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugFnoTab_PutAddrFile(
 DEE_A_INOUT struct DeeCodeWriterDebugFnoTab *self,
 DEE_A_IN Dee_size_t addr, DEE_A_INOUT DeeStringObject *file) {
 Dee_size_t fileid; DeeStringObject **iter,**end;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(file) && DeeString_Check(file));
 DEE_ASSERT(!self->cwdft_files || (DeeObject_Check(
  self->cwdft_files) && DeeTuple_Check(self->cwdft_files)));
 if (self->cwdft_files) {
  end = (iter = (DeeStringObject **)DeeTuple_ELEM(self->cwdft_files))+DeeTuple_SIZE(self->cwdft_files);
  // Check if this file was used before
  while (iter != end) {
   DEE_ASSERT(DeeObject_Check(*iter) && DeeString_Check(*iter));
   if (DeeString_SIZE(*iter) == DeeString_SIZE(file) &&
       memcmp(DeeString_STR(*iter),DeeString_STR(file),
       DeeString_SIZE(file)*sizeof(char)) == 0) { // Found it!
    fileid = (Dee_size_t)(iter-(DeeStringObject **)DeeTuple_ELEM(self->cwdft_files));
    goto put_fileid;
   }
   ++iter;
  }
  // Add a new file
  fileid = DeeTuple_SIZE(self->cwdft_files);
  if (DeeTuple_Append((DeeObject **)&self->cwdft_files,(DeeObject *)file) != 0) return -1;
 } else {
  fileid = 0;
  if ((self->cwdft_files = (DeeTupleObject *)DeeTuple_Pack(1,file)) == NULL) return -1;
 }
put_fileid:
 if (DeeCodeWriterDebugLnoTab_PutAddrLnoff(&self->cwdft_lno,addr,
  ((Dee_int64_t)fileid-(Dee_int64_t)(Dee_ssize_t)self->cwdft_fileid)) != 0) return -1;
 self->cwdft_fileid = fileid;
 return 0;
}

DEE_A_RET_MAYBE_NULL char const *DeeCodeWriterDebugFnoTab_Addr2File(
 DEE_A_IN struct DeeCodeWriterDebugFnoTab const *self, DEE_A_IN Dee_size_t addr) {
 DeeObject *result_str; Dee_int64_t fileid; DEE_ASSERT(self);
 if (!self->cwdft_files) return NULL;
 fileid = DeeCodeWriterDebugLnoTab_Addr2Line(&self->cwdft_lno,addr)-1;
 if (fileid < 0 || fileid > DeeTuple_SIZE(self->cwdft_files)) return NULL;
 result_str = DeeTuple_GET(self->cwdft_files,(Dee_size_t)fileid);
 DEE_ASSERT(DeeObject_Check(result_str) && DeeString_Check(result_str));
 return DeeString_STR(result_str);
}




DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebugVarNames_PutVarname(
 DEE_A_INOUT struct DeeCodeWriterDebugVarNames *self,
 DEE_A_IN Dee_size_t id, DEE_A_IN_Z char const *name) {
 DeeObject **iter,**end,*name_ob; Dee_size_t old_size;
 DEE_ASSERT(self);
 DEE_ASSERT(!self->cwdvn_names || (DeeObject_Check(
  self->cwdvn_names) && DeeTuple_Check(self->cwdvn_names)));
 old_size = self->cwdvn_names ? DeeTuple_SIZE(self->cwdvn_names) : 0;
 if (id < old_size) {
  iter = DeeTuple_ELEM(self->cwdvn_names)+id;
  DEE_ASSERT(DeeObject_Check(*iter));
  if (*iter != Dee_EmptyString) {
   DEE_ASSERT(DeeString_Check(*iter));
   if DEE_UNLIKELY(DeeString_AppendWithLength(iter,1,"|") != 0) return -1;
   return DeeString_Append(iter,name);
  }
 } else {
  if DEE_UNLIKELY(_DeeTuple_ResizeUnsafe((DeeObject **)&self->cwdvn_names,id+1) != 0) return -1;
  iter = DeeTuple_ELEM(self->cwdvn_names)+old_size;
  end = DeeTuple_ELEM(self->cwdvn_names)+(id+1);
  while (iter != end) { Dee_INCREF(*iter = Dee_EmptyString); ++iter; }
  --iter;
 }
 if DEE_UNLIKELY((name_ob = DeeString_New(name)) == NULL) return -1;
 Dee_DECREF(*iter);
 Dee_INHERIT_REF(*iter,name_ob);
 return 0;
}




DEE_A_RET_EXCEPT(-1) int DeeCodeWriterDebug_PutFileAndLine(
 DEE_A_INOUT struct DeeCodeWriterDebug *self, DEE_A_IN Dee_size_t addr,
 DEE_A_IN_Z_OPT char const *filename, DEE_A_IN Dee_int64_t line) {
 DeeObject *filename_ob; int error;
 DEE_ASSERT(self);
 if (line != self->cwd_line) {
  // Put line information
  if (DeeCodeWriterDebugLnoTab_PutAddrLnoff(
   &self->cwd_lno,addr,line-self->cwd_line) != 0) return -1;
  self->cwd_line = line;
 }
 if (filename && filename != self->cwd_file) {
  // Put file information
  self->cwd_file = filename;
  if ((filename_ob = DeeString_New(filename)) == NULL) return -1;
  error = DeeCodeWriterDebugFnoTab_PutAddrFile(
   &self->cwd_fno,addr,(DeeStringObject *)filename_ob);
  Dee_DECREF(filename_ob);
  if (error != 0) return error;
 }
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeCodeDebugInfoObject *DeeCodeWriterDebug_Pack(
 DEE_A_INOUT struct DeeCodeWriterDebug *self,
 DEE_A_IN_OBJECT_OPT(DeeStringObject) const *name) {
 DeeCodeDebugInfoObject *result;
 DEE_ASSERT(!name || (DeeObject_Check(name) && DeeString_Check(name)));
 if ((result = (DeeCodeDebugInfoObject *)_DeeCodeDebugInfo_New()) == NULL) return NULL;
 Dee_XINCREF(result->di_name = (DeeStringObject *)name);
 result->di_local_names = self->cwd_locals.cwdvn_names; // Transfer reference
 self->cwd_locals.cwdvn_names = NULL;
 result->di_const_names = self->cwd_consts.cwdvn_names; // Transfer reference
 self->cwd_consts.cwdvn_names = NULL;
 result->di_ref_names = self->cwd_refs.cwdvn_names; // Transfer reference
 self->cwd_refs.cwdvn_names = NULL;
 result->di_arg_names = self->cwd_args.cwdvn_names; // Transfer reference
 self->cwd_args.cwdvn_names = NULL;
 if (self->cwd_lno.cwdlt_lnoc) { // Mark the Lno end entry
  ((Dee_uint16_t *)(result->di_lno = self->cwd_lno.cwdlt_lnov))[self->cwd_lno.cwdlt_lnoc] = 0;
  self->cwd_lno.cwdlt_lnoc = 0;
  self->cwd_lno.cwdlt_lnoa = 0;
  self->cwd_lno.cwdlt_lnov = NULL;
 } else result->di_lno = NULL;
 if (!DeeCodeWriterDebugFnoTab_Empty(&self->cwd_fno)) { // Mark the Fno end entry
  ((Dee_uint16_t *)(result->di_fno = self->cwd_fno.cwdft_lno.cwdlt_lnov))[self->cwd_fno.cwdft_lno.cwdlt_lnoc] = 0;
  self->cwd_fno.cwdft_lno.cwdlt_lnoc = 0;
  self->cwd_fno.cwdft_lno.cwdlt_lnoa = 0;
  self->cwd_fno.cwdft_lno.cwdlt_lnov = NULL;
  result->di_file_names = self->cwd_fno.cwdft_files; // Transfer reference
  self->cwd_fno.cwdft_files = NULL;
 } else {
  result->di_fno = NULL;
  result->di_file_names = NULL;
 }
 return result;
}










//////////////////////////////////////////////////////////////////////////
// Code writer
static int _dee_illegal_jmp_stack(
 struct DeeLexerObject *lexer, struct DeeTokenObject *from_token,
 struct DeeTokenObject *to_token, Dee_size_t missing_slots) {
 return DeeError_CompilerErrorf(DEE_WARNING_ILLEGAL_STACK_JUMP,
  (DeeObject *)lexer,(DeeObject *)from_token,
  "Illegal jump: Destination for label has greater stack alignment requirements (+%Iu slots)\n"
  "%s(%d) : %k : See reference to label destination",
  missing_slots,DeeToken_FILE(to_token),DeeToken_LINE(to_token)+1,to_token);
}
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
static int _dee_illegal_jmp_except(
 struct DeeLexerObject *lexer, struct DeeTokenObject *from_token,
 struct DeeTokenObject *to_token, Dee_size_t missing_slots) {
 return DeeError_CompilerErrorf(DEE_WARNING_ILLEGAL_EXCEPT_JUMP,
  (DeeObject *)lexer,(DeeObject *)from_token,
  "Illegal jump: Destination for label has greater except alignment requirements (+%Iu slots)"
  "%s(%d) : %k : See reference to label destination",
  missing_slots,DeeToken_FILE(to_token),DeeToken_LINE(to_token)+1,to_token);
}
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_PutLabel(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_INOUT struct DeeLexerObject *lexer, DEE_A_INOUT struct DeeTokenObject *name_token) {
 struct DeeForwardCodeLabel *fwd_iter,*fwd_end;
 int has_alignment_code;
 Dee_size_t alignment_skip_opcode;
 DEE_ASSERT(self && lbl);
 DEE_ASSERT(DeeObject_Check(name_token) && DeeToken_Check(name_token));
 DEE_ASSERTF(!DeeCodeLabel_HAS_ENDP(lbl),"Label already has an endpoint");

 // Initialize label data
 lbl->cl_addr = DeeCodeWriter_ADDR(self);
 lbl->cl_stack_size = self->cw_stack_size;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 lbl->cl_except_size = self->cw_except_size;
#endif
 Dee_INCREF(lbl->cl_endptok = name_token);

 if (!lbl->cl_fwd_c) return 0;

 // Setup forward jumps
 // NOTE: There is the possibility that we have to hack in some
 //       additional code, so we can manually adjust alignments.
 has_alignment_code = 0;
 fwd_end = (fwd_iter = lbl->cl_fwd_v)+lbl->cl_fwd_c;
 do {
  if (fwd_iter->fcl_stack_size != lbl->cl_stack_size
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
      || fwd_iter->fcl_except_size != lbl->cl_except_size
#endif
      ) {
   // Illegal jump or requires alignment code
   if DEE_UNLIKELY(fwd_iter->fcl_stack_size < lbl->cl_stack_size) {
    // Illegal jump: dst has a greater stack alignment than src
    if DEE_UNLIKELY(_dee_illegal_jmp_stack(lexer,fwd_iter->fcl_token,name_token,(
     Dee_size_t)(lbl->cl_stack_size-fwd_iter->fcl_stack_size)) != 0) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
illegal_jmp:
#endif
    // Make the jump lead to the next opcode as fallback
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,fwd_iter->fcl_jmp_addr,
     sizeof(Dee_uint8_t)+sizeof(Dee_uint16_t)) != 0) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
   } else if DEE_UNLIKELY(fwd_iter->fcl_except_size < lbl->cl_except_size) {
    // Illegal jump: dst has a greater except alignment than src
    if DEE_UNLIKELY(_dee_illegal_jmp_except(lexer,fwd_iter->fcl_token,name_token,(
     Dee_size_t)(lbl->cl_except_size-fwd_iter->fcl_except_size)) != 0) return -1;
    goto illegal_jmp;
#endif
   } else {
    Dee_size_t stack_offset;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
    Dee_size_t except_offset;
#endif
    // Need custom alignment code
    if (!has_alignment_code) {
     // Put in the code to skip alignment code when we just pass the label
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
      self,OP_JUMP,&alignment_skip_opcode) != 0) return -1;
     has_alignment_code = 1;
    }
    // Jump here to adjust the stack / except handlers
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,fwd_iter->fcl_jmp_addr,
     (Dee_size_t)(DeeCodeWriter_ADDR(self)-(fwd_iter->fcl_jmp_addr-1))) != 0) return -1;
    // Align stack & Except handlers
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
    except_offset = (Dee_size_t)(fwd_iter->fcl_except_size-lbl->cl_except_size);
    if (except_offset) {
     while (except_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_TRY_END) != 0) return -1;
     // Must use an expensive stack align, as we don't actually know how big it will be after the last try_end
     if DEE_UNLIKELY(DeeCodeWriter_WriteStackAlign(self,lbl->cl_stack_size) != 0) return -1;
    } else
#endif
    {
     stack_offset = fwd_iter->fcl_stack_size-lbl->cl_stack_size;
     while (stack_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
    }
    // Perform the jump to the label destination
    if DEE_UNLIKELY(DeeCodeWriter_JmpAbs(self,lbl->cl_addr) != 0) return -1;
   }
  } else {
   // Simple case: no alignment required (we can simply fill in the label dst)
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,
    fwd_iter->fcl_jmp_addr,lbl->cl_addr-(fwd_iter->fcl_jmp_addr-1)) != 0) return -1;
  }
 } while (++fwd_iter != fwd_end);
 fwd_iter = lbl->cl_fwd_v;
 do _DeeForwardCodeLabel_Quit(fwd_iter);
 while (++fwd_iter != fwd_end);
 free_nn(lbl->cl_fwd_v);
 lbl->cl_fwd_v = NULL;
 lbl->cl_fwd_c = 0;
 if (has_alignment_code) {
  // Fill in the jump dst when skipping alignment code
  if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,alignment_skip_opcode,
   DeeCodeWriter_ADDR(self)-(alignment_skip_opcode-1)) != 0) return -1;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_GotoForwardLabelEx(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_IN Dee_uint8_t jmp_op, DEE_A_INOUT struct DeeTokenObject *name_token) {
 struct DeeForwardCodeLabel *fwd_list;
 Dee_size_t jmp_dst;
 DEE_ASSERT(self);
 DEE_ASSERT(lbl);
 DEE_ASSERT(DeeObject_Check(name_token) && DeeToken_Check(name_token));
 DEE_ASSERTF(!DeeCodeLabel_HAS_ENDP(lbl),"Label must not already have an endpoint when setting up forward jumps");
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,jmp_op,&jmp_dst) != 0) return -1;
 while DEE_UNLIKELY((fwd_list = (struct DeeForwardCodeLabel *)realloc_nz(
  lbl->cl_fwd_v,(lbl->cl_fwd_c+1)*sizeof(struct DeeForwardCodeLabel))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 lbl->cl_fwd_v = fwd_list;
 fwd_list += (lbl->cl_fwd_c++);
 // Put a forward declaration
 Dee_INCREF(fwd_list->fcl_token = name_token);
 fwd_list->fcl_jmp_addr = jmp_dst;
 fwd_list->fcl_stack_size = self->cw_stack_size;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 fwd_list->fcl_except_size = self->cw_except_size;
#endif
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_GotoLabel(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT struct DeeCodeLabel *lbl,
 DEE_A_INOUT struct DeeLexerObject *lexer, DEE_A_INOUT struct DeeTokenObject *name_token) {
 DEE_ASSERT(self);
 DEE_ASSERT(lbl);
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(name_token) && DeeToken_Check(name_token));
 // Check if we need to put a forward jump
 if (!DeeCodeLabel_HAS_ENDP(lbl)) {
  return _DeeCodeWriter_GotoForwardLabelEx(self,lbl,OP_JUMP,name_token);
 }
 // Put a direct jump
 if DEE_UNLIKELY(self->cw_stack_size < lbl->cl_stack_size) {
  // Illegal jump: dst has a greater stack alignment than src
  if DEE_UNLIKELY(_dee_illegal_jmp_stack(lexer,lbl->cl_endptok,name_token,
   (Dee_size_t)(lbl->cl_stack_size-self->cw_stack_size)) != 0) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 } else if DEE_UNLIKELY(self->cw_except_size < lbl->cl_except_size) {
  // Illegal jump: dst has a greater except alignment than src
  if DEE_UNLIKELY(_dee_illegal_jmp_except(lexer,lbl->cl_endptok,name_token,
   (Dee_size_t)(lbl->cl_except_size-self->cw_except_size)) != 0) return -1;
#endif
 } else {
  Dee_size_t stack_offset;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  Dee_size_t except_offset;
#endif
  // Align stack & Except handlers
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  except_offset = (Dee_size_t)(self->cw_except_size-lbl->cl_except_size);
  if (except_offset) {
   while (except_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_TRY_END) != 0) return -1;
   // Must use an expensive stack align, as we don't actually know how big it will be after the last try_end
   if DEE_UNLIKELY(DeeCodeWriter_WriteStackAlign(self,lbl->cl_stack_size) != 0) return -1;
  } else
#endif
  {
   stack_offset = self->cw_stack_size-lbl->cl_stack_size;
   while (stack_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
  }
  // Perform the jump to the label destination
  if DEE_UNLIKELY(DeeCodeWriter_JmpAbs(self,lbl->cl_addr) != 0) return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_FinalizeLabels(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeParserLabelList const *labels,
 DEE_A_INOUT struct DeeLexerObject *lexer) {
 struct DeeParserLabel *iter;
 struct DeeForwardCodeLabel *fwd_iter,*fwd_end;
 DEE_ASSERT(self && labels);
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 iter = labels->ll_front;
 while (iter) {
  fwd_end = (fwd_iter = iter->pl_code_label.cl_fwd_v)+iter->pl_code_label.cl_fwd_c;
  while (fwd_iter != fwd_end) {
   switch (iter->pl_kind) {
    case DEE_PARSERLABEL_KIND_LABEL:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNDEFINED_LABEL,
      (DeeObject *)lexer,(DeeObject *)fwd_iter->fcl_token,
      "Invalid jump: Label destination for %q never defined",
      TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),iter->pl_name)) != 0) return -1;
     break;
    case DEE_PARSERLABEL_KIND_C_DEFAULT:
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_UNDEFINED_LABEL,
      (DeeObject *)lexer,(DeeObject *)fwd_iter->fcl_token,
      "Invalid jump: Label destination for 'default' never defined") != 0) return -1;
     break;
    case DEE_PARSERLABEL_KIND_C_CASE:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNDEFINED_LABEL,
      (DeeObject *)lexer,(DeeObject *)fwd_iter->fcl_token,
      "Invalid jump: Label destination for 'case %r' never defined",
      iter->pl_c_case.clc_case) != 0) return -1;
     break;
    case DEE_PARSERLABEL_KIND_C_RANGE:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_UNDEFINED_LABEL,
      (DeeObject *)lexer,(DeeObject *)fwd_iter->fcl_token,
      "Invalid jump: Label destination for 'case %r ... %r' never defined",
      iter->pl_c_range.clr_begin,iter->pl_c_range.clr_end) != 0) return -1;
     break;
    default: break;
   }
   // Make the goto simply jump to the next opcode
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,fwd_iter->fcl_jmp_addr,
    sizeof(Dee_uint8_t)+sizeof(Dee_uint16_t)) != 0) return -1;
   ++fwd_iter;
  }
  iter = iter->pl_next;
 }
 return 0;
}

void DeeCodeWriter_Init(
 DEE_A_OUT struct DeeCodeWriter *self,
 DEE_A_IN struct DeeScopeObject *root_scope) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(root_scope) && DeeScope_Check(root_scope));
 self->cw_root_scope = root_scope;
 self->cw_constc = 0;
 self->cw_consta = 0;
 self->cw_constv = NULL;
 self->cw_refc = 0;
 self->cw_refa = 0;
 self->cw_refv = NULL;
 self->cw_flags = DEE_CODE_FLAG_NONE;
 self->cw_stack_size = 0;
 self->cw_stack_size_min = 0;
 _DeeCodeWriterStackMemList_Init(&self->cw_stackmem);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 self->cw_except_size = 0;
 self->cw_except_size_min = 0;
 self->cw_exceptv = NULL;
 self->cw_exceptc = 0;
 self->cw_finally_size = 0;
 self->cw_finally_size_min = 0;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 self->cw_localc = 0;
 self->cw_last_op = (Dee_uint8_t)-1;
 self->cw_code_pos = NULL;
 self->cw_code_begin = NULL;
 self->cw_code_end = NULL;
 _DeeCodeWriterDebug_Init(&self->cw_debug);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 self->cw_loop_except_size = 0;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 self->cw_loop_stack_size = 0;
 self->cw_continue_addr = (Dee_size_t)-1;
 self->cw_break_forward_v = NULL;
 self->cw_break_forward_c = 0;
}
void DeeCodeWriter_Quit(DEE_A_IN struct DeeCodeWriter *self) {
 DeeObject **begin,**end;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry *exc_handlers,*exc_handlers_end;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 DEE_ASSERT(self);
 // Make sure the writer is in a consistent state
 DEE_ASSERTF(self->cw_continue_addr == (Dee_size_t)-1 &&
             self->cw_break_forward_v == NULL &&
             self->cw_break_forward_c == 0,
             "loop not cleaned up correctly; "
             "forgot to call 'DeeCodeWriter_FAIL_LOOP'");
 _DeeCodeWriterFreeLocalList_Quit(&self->cw_freelocals);
 _DeeCodeWriterStackMemList_Quit(&self->cw_stackmem);
 // Free debug information
 _DeeCodeWriterDebug_Quit(&self->cw_debug);
 // Free constants
 end = (begin = self->cw_constv)+self->cw_constc;
 while (end != begin) Dee_DECREF(*--end);
 free(begin);
 end = (begin = (DeeObject **)self->cw_refv)+self->cw_refc;
 while (end != begin) Dee_DECREF(*--end);
 free(begin);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 // Free exception handlers
 exc_handlers_end = (exc_handlers = self->cw_exceptv)+self->cw_exceptc;
 while (exc_handlers != exc_handlers_end) {
  _DeeExceptionHandlerEntry_Quit(exc_handlers);
  ++exc_handlers;
 }
 free(self->cw_exceptv);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 // Free the code
 free(self->cw_code_begin);
}



DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Optimize(
 DEE_A_IN struct DeeCodeWriter *self,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 if ((compiler_flags&DEE_COMPILER_FLAG_OPTIMIZE_PEEPHOLE)!=0) {
  Dee_uint32_t performed_optimizations;
  do { performed_optimizations = 0;
   if DEE_UNLIKELY(DeeCodeWriter_DoPeepholeOptimization(
     self,&performed_optimizations) != 0) return -1;
  } while (performed_optimizations);
 }
 if DEE_UNLIKELY((compiler_flags&DEE_COMPILER_FLAG_OPTIMIZE_DELNOOP)!=0 &&
                 DEE_UNLIKELY(DeeCodeWriter_RemoveNoopOpcodes(self) != 0)) return -1;
 return 0;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeCodeObject) *DeeCodeWriter_Pack(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN_OBJECT_OPT(DeeStringObject) const *name) {
 DeeCodeObject *result; Dee_size_t code_size;
 DeeTupleObject *consts;
 DEE_ASSERT(self);
 DEE_ASSERTF(self->cw_stack_size == 0,
             "Can't create code object with inconsistent stack (+%Iu too many)",
             self->cw_stack_size);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DEE_ASSERTF(self->cw_except_size == 0,
             "Can't create code object with inconsistent exception handlers (+%Iu too many)",
             self->cw_except_size);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 code_size = (Dee_size_t)(self->cw_code_pos-self->cw_code_begin);
 if DEE_UNLIKELY((consts = (DeeTupleObject *)DeeTuple_NewFromVector(
  self->cw_constc,self->cw_constv)) == NULL) return NULL;
 if DEE_LIKELY((result = (DeeCodeObject *)DeeObject_Mallocf(DEE_CODE_SIZE(code_size),
  "Code object (%lu code bytes)",(unsigned long)code_size)) != NULL) {
  DeeObject_INIT(result,&DeeCode_Type);
  if (!DeeCodeWriterDebug_Empty(&self->cw_debug)) {
   if DEE_UNLIKELY((result->co_debug = DeeCodeWriterDebug_Pack(&self->cw_debug,name)) == NULL) {
    Dee_DECREF(consts);
    DeeObject_Free(result);
    return NULL;
   }
  } else result->co_debug = NULL;

  _DeeCode_InitCommon(result);
  Dee_INHERIT_REF(result->co_consts,consts);
  result->co_flags = self->cw_flags;
  result->co_nlocals = self->cw_localc;
  result->co_stacksize = self->cw_stack_size_min;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  result->co_exceptsize = self->cw_except_size_min;
  result->co_finallyc = self->cw_finally_size_min;
  // We transfer the all exception handlers instead of copying them
  // >> That way this function is faster and there is no situation
  //    where you would want to continue writing code after packing the writer!
  result->co_exceptc = self->cw_exceptc;
  result->co_exceptv = self->cw_exceptv;
  self->cw_exceptc = 0;
  self->cw_exceptv = NULL;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

  // Calculate execution buffer constants
  DeeCode_CALCULATE_WBUFFERSIZE(result,self->cw_stackmem.cwsml_maxsize);

  // Copy the actual bytecode data into the result code object
  result->co_size = code_size;
  memcpy(result->co_code,self->cw_code_begin,code_size);
 } else Dee_DECREF(consts);
 return (DeeObject *)result;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
_DeeCodeWriter_IncCodeBuf(DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_uint8_t *new_code;
 Dee_size_t new_size = (Dee_size_t)(self->cw_code_end-self->cw_code_begin);
 if DEE_UNLIKELY(!new_size) new_size = 8; else new_size *= 2;
 while DEE_UNLIKELY((new_code = (Dee_uint8_t *)realloc_nz(
  self->cw_code_begin,new_size*sizeof(Dee_uint8_t))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->cw_code_pos = new_code+(self->cw_code_pos-self->cw_code_begin);
 self->cw_code_end = (self->cw_code_begin = new_code)+new_size;
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
_DeeCodeWriter_IncConstBuf(DEE_A_INOUT struct DeeCodeWriter *self) {
 DeeObject **new_consts;
 Dee_size_t new_size = self->cw_constc;
 if DEE_UNLIKELY(!new_size) new_size = 2; else new_size *= 2;
 while DEE_UNLIKELY((new_consts = (DeeObject **)realloc_nz(
  self->cw_constv,new_size*sizeof(DeeObject *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->cw_constv = new_consts;
 self->cw_consta = new_size;
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
_DeeCodeWriter_IncRefBuf(DEE_A_INOUT struct DeeCodeWriter *self) {
 DeeLocalVarObject **new_refs;
 Dee_size_t new_size = self->cw_refc;
 if DEE_UNLIKELY(!new_size) new_size = 2; else new_size *= 2;
 while DEE_UNLIKELY((new_refs = (DeeLocalVarObject **)realloc_nz(
  self->cw_refv,new_size*sizeof(DeeLocalVarObject *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->cw_refv = new_refs;
 self->cw_refa = new_size;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_WriteData(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 DEE_ASSERT(self);
 // v not necessary, because we never write more than 8 bytes
 /*while*/if DEE_UNLIKELY((Dee_size_t)(self->cw_code_end-self->cw_code_pos) < s) {
  if DEE_UNLIKELY(_DeeCodeWriter_IncCodeBuf(self) != 0) return -1;
 }
 memcpy(self->cw_code_pos,p,s);
 self->cw_code_pos += s;
 return 0;
}



DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocConst(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *v,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 Dee_size_t result; DeeObject **iter,**end; int temp;
 DEE_ASSERT(self);
 if ((compiler_flags&DEE_COMPILER_FLAG_OPTIMIZE_REUSE_CONSTS)!=0) {
  end = (iter = self->cw_constv)+self->cw_constc;
  while (iter != end) {
   if DEE_UNLIKELY((temp = DeeObject_DeepEquals(v,*iter)) != 0) {
    if DEE_UNLIKELY(temp < 0) DeeError_Handled();
    if (temp == DEE_OBJECT_DEEPEQUALS_TRUE)
     return (Dee_size_t)(iter-self->cw_constv); // Found it!
   }
   ++iter;
  }
 }
 if DEE_UNLIKELY(self->cw_constc == self->cw_consta &&
                 _DeeCodeWriter_IncConstBuf(self) != 0
                 ) return (Dee_size_t)-1;
 Dee_INCREF(self->cw_constv[
  result = self->cw_constc++] = v);
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocStatic(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *init,
 DEE_A_IN_Z_OPT char const *name) {
 Dee_size_t result_id;
 DEE_ASSERT(self);
 if DEE_UNLIKELY(self->cw_constc == self->cw_consta &&
                 _DeeCodeWriter_IncConstBuf(self) != 0) return (Dee_size_t)-1;
 result_id = self->cw_constc++;
 Dee_INCREF(self->cw_constv[result_id] = init);
 if DEE_UNLIKELY(name && DEE_UNLIKELY(DeeCodeWriter_AddConstName(
  self,result_id,name) != 0)) return (Dee_size_t)-1;
 return result_id;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocLocal(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN_Z_OPT char const *name) {
 Dee_size_t result_id;
 DEE_ASSERT(self);
 if (self->cw_freelocals.cwfl_c) {
  result_id = _DeeCodeWriterFreeLocalList_Pop(&self->cw_freelocals);
  DEE_LVERBOSE1("Reusing free local id %I16u\n",result_id);
 } else result_id = self->cw_localc++;
 if DEE_UNLIKELY(name && DEE_UNLIKELY(DeeCodeWriter_AddLocalName(
  self,result_id,name) != 0)) return (Dee_size_t)-1;
 return result_id;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeCodeWriter_AllocOrGetReference(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_INOUT struct DeeLocalVarObject *referenced_var,
 DEE_A_IN_Z_OPT char const *name) {
 DeeLocalVarObject **iter,**end; Dee_size_t result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(referenced_var) && DeeLocalVar_Check(referenced_var));
 DEE_ASSERTF(!DeeCodeWriter_IsVarLocal(self,referenced_var),
             "Variable originates in the current root scope, meaning it isn't a reference");
 end = (iter = self->cw_refv)+self->cw_refc;
 while (iter != end) {
  if (*iter == referenced_var)
   return (Dee_size_t)(iter-self->cw_refv);
  ++iter;
 }
 if DEE_UNLIKELY(self->cw_refc == self->cw_refa &&
                 DEE_UNLIKELY(_DeeCodeWriter_IncRefBuf(self) != 0)) return (Dee_size_t)-1;
 DEE_ASSERT(self->cw_refc != self->cw_refa);
 result = self->cw_refc++;
 Dee_INCREF(self->cw_refv[result] = referenced_var);
 if DEE_UNLIKELY(name && DEE_UNLIKELY(DeeCodeWriter_AddRefName(
  self,result,name) != 0)) return (Dee_size_t)-1;
 return result;
}



DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOp(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op) {
 self->cw_last_op = op;
 return _DeeCodeWriter_WriteData(self,&op,1);
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_uint16_t arg) {
 Dee_uint8_t buf[3];
 buf[0] = (self->cw_last_op = op);
 buf[1] = ((Dee_uint8_t *)&arg)[0];
 buf[2] = ((Dee_uint8_t *)&arg)[1];
 return _DeeCodeWriter_WriteData(self,buf,3);
}
#if 0
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithFutureArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_OUT Dee_size_t *arg_addr) {
 Dee_uint8_t buf[3];
 DEE_ASSERT(arg_addr);
 buf[0] = (self->cw_last_op = op);
 *arg_addr = DeeCodeWriter_ADDR(self)+1;
 return _DeeCodeWriter_WriteData(self,buf,3);
}
#endif

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithFutureSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op,
 DEE_A_OUT Dee_size_t *arg_addr) {
 Dee_uint8_t buf[6];
 buf[0] = op; // PREFIX+16-arg + op+16-arg
 *arg_addr = DeeCodeWriter_ADDR(self)+1; // Point to the first argument
 if DEE_UNLIKELY(_DeeCodeWriter_WriteData(self,buf,6) != 0) return -1;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_SetFutureSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t arg_addr, DEE_A_IN Dee_size_t arg) {
 DEE_ASSERT(self);
 if DEE_LIKELY(arg <= 0xFFFF) {
  // Most likely case: Argument fits into 16-bit
//self->cw_code_begin[arg_addr-1] = ...; // Prefixed opcode ('op' argument of call to 'DeeCodeWriter_WriteOpWithFutureSizeArg')
  // Set the non-prefixed argument
  *(Dee_uint16_t *)&self->cw_code_begin[arg_addr+0] = (Dee_uint16_t)arg;
  // -> Fill the memory of the unused prefix with no-ops (will be optimized away later)
  self->cw_code_begin[arg_addr+2] = OP_NOOP;
  self->cw_code_begin[arg_addr+3] = OP_NOOP;
  self->cw_code_begin[arg_addr+4] = OP_NOOP;
  return 0;
 }
#ifdef DEE_PLATFORM_64_BIT
 // On 64-bit, we must make sure extending to 32-bit is enough
 // todo: Maybe we should make it possible to extend an argument to 64-bit? (at least on x64?)
 if (arg > 0xFFFFFFFF) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Opcode %I8u(0x%.2IX) argument %Iu(0x%.16IX) "
                      "is too big (cannot extend beyond 32-bit)",
                      self->cw_code_begin[arg_addr+2],
                      self->cw_code_begin[arg_addr+2],
                      arg,arg);
  return -1;
 }
#endif
 // Extend to 32-bit argument
 self->cw_code_begin[arg_addr+2] = self->cw_code_begin[arg_addr-1];
 self->cw_code_begin[arg_addr-1] = OP_PREFIX;
 *(Dee_uint16_t *)&self->cw_code_begin[arg_addr+0] = (Dee_uint16_t)((arg & DEE_INT32_C(0xFFFF0000)) >> 16);
 *(Dee_uint16_t *)&self->cw_code_begin[arg_addr+3] = (Dee_uint16_t)(arg & DEE_INT32_C(0xFFFF));
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_SetFutureSSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t arg_addr, DEE_A_IN Dee_ssize_t arg) {
 Dee_int32_t i32_arg;
 DEE_ASSERT(self);
 if DEE_LIKELY(arg >= INT16_MIN && arg <= INT16_MAX) {
  // Most likely case: Argument fits into 16-bit
//self->cw_code_begin[arg_addr-1] = ...; // Prefixed opcode ('op' argument of call to 'DeeCodeWriter_WriteOpWithFutureSizeArg')
  // Set the non-prefixed argument
  *(Dee_int16_t *)&self->cw_code_begin[arg_addr+0] = (Dee_int16_t)arg;
  // -> Fill the memory of the unused prefix with no-ops (will be optimized away later)
  self->cw_code_begin[arg_addr+2] = OP_NOOP;
  self->cw_code_begin[arg_addr+3] = OP_NOOP;
  self->cw_code_begin[arg_addr+4] = OP_NOOP;
 }
#ifdef DEE_PLATFORM_64_BIT
 // On 64-bit, we must make sure extending to 32-bit is enough
 // todo: Maybe we should make it possible to extend an argument to 64-bit? (at least on x64?)
 if (arg < INT32_MIN || arg > INT32_MAX) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Opcode %I8u(0x%.2IX) argument %Id(0x%.16IX) "
                      "is too big (cannot extend beyond 32-bit)",
                      self->cw_code_begin[arg_addr+2],
                      self->cw_code_begin[arg_addr+2],
                      arg,arg);
  return -1;
 }
#endif
 // Extend to 32-bit argument
 i32_arg = (Dee_int32_t)arg;
 self->cw_code_begin[arg_addr+2] = self->cw_code_begin[arg_addr-1];
 self->cw_code_begin[arg_addr-1] = OP_PREFIX;
 *(Dee_uint16_t *)&self->cw_code_begin[arg_addr+0] = (Dee_uint16_t)((i32_arg & DEE_INT32_C(0xFFFF0000)) >> 16);
 *(Dee_uint16_t *)&self->cw_code_begin[arg_addr+3] = (Dee_uint16_t)(i32_arg & DEE_INT32_C(0xFFFF));
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithSSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_ssize_t arg) {
 Dee_int32_t i32_arg;
 DEE_ASSERT(self);
 if DEE_LIKELY(arg >= INT16_MIN && arg <= INT16_MAX) // Most likely case: Argument fits into 16-bit
  return DeeCodeWriter_WriteOpWithArg(self,op,(Dee_uint16_t)(Dee_int16_t)arg);
#ifdef DEE_PLATFORM_64_BIT
 // On 64-bit, we must make sure extending to 32-bit is enough
 // todo: Maybe we should make it possible to extend an argument to 64-bit? (at least on x64?)
 if (arg < INT32_MIN || arg > INT32_MAX) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Opcode %I8u(0x%.2IX) argument %Id(0x%.16IX) "
                      "is too big (cannot extend beyond 32-bit)",
                      op,op,arg,arg);
  return -1;
 }
#endif
 // Extend to 32-bit argument
 i32_arg = (Dee_int32_t)arg;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_PREFIX,
  (Dee_uint16_t)((i32_arg & DEE_INT32_C(0xFFFF0000)) >> 16)) != 0) return -1;
 return DeeCodeWriter_WriteOpWithArg(self,op,(Dee_uint16_t)(i32_arg & DEE_INT32_C(0xFFFF)));
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_WriteOpWithSizeArg(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_size_t arg) {
 DEE_ASSERT(self);
 if DEE_LIKELY(arg <= 0xFFFF) // Most likely case: Argument fits into 16-bit
  return DeeCodeWriter_WriteOpWithArg(self,op,(Dee_uint16_t)arg);
#ifdef DEE_PLATFORM_64_BIT
 // On 64-bit, we must make sure extending to 32-bit is enough
 // todo: Maybe we should make it possible to extend an argument to 64-bit? (at least on x64?)
 if (arg > 0xFFFFFFFF) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Opcode %I8u(0x%.2IX) argument %Iu(0x%.16IX) "
                      "is too big (cannot extend beyond 32-bit)",
                      op,op,arg,arg);
  return -1;
 }
#endif
 // Extend to 32-bit argument
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_PREFIX,
  (Dee_uint16_t)((arg & DEE_INT32_C(0xFFFF0000)) >> 16)) != 0) return -1;
 return DeeCodeWriter_WriteOpWithArg(self,op,(Dee_uint16_t)(arg & DEE_INT32_C(0xFFFF)));
}



DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DelVar(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN struct DeeLocalVarObject *var,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(var) && DeeLocalVar_Check(var));
 DEE_ASSERTF(DeeCodeWriter_IsVarLocal(self,var),"Variable not local to this codewriter");
 if (--var->lv_module_refs != 0) return 0; // Too soon...
 if (!DeeLocalVar_IS_COMPILERINIT(var)) return 0; // Variable wasn't used / was optimized away
 // After this, the variable is no longer initialized
 // NOTE: This will allow a new instance to be bound to an instance-bound variable
 var->lv_flags &= ~DEE_LOCALVAR_FLAG_INITIALIZED;
 switch (DeeLocalVar_KIND(var)) {
 
  case DEE_LOCALVAR_KIND_LOCAL:
   if ((compiler_flags&DEE_COMPILER_FLAG_OPTIMIZE_REUSE_LOCALS)!=0) {
    // This id has become free and can be re-used!
    if DEE_UNLIKELY(DeeCodeWriterFreeLocalList_Append(
     &self->cw_freelocals,var->lv_loc_id) != 0) return -1;
   }
   return DeeCodeWriter_WriteOpWithSizeArg(self,OP_DEL_LOCAL,var->lv_loc_id);

  case DEE_LOCALVAR_KIND_RETVAL:
   return DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_DEL_RESULT);

  case DEE_LOCALVAR_KIND_STACK: {
   Dee_size_t minid; DeeLocalVarObject *locv;
   struct _DeeScopeEntry *iter,*end;
   minid = var->lv_loc_id;
   if DEE_UNLIKELY(DeeCodeWriter_DelStack(self,var->lv_loc_id) != 0) return -1;
   // Adjust ids of initialized __stack variables with greater ids.
   // >> local __stack x = 10; // slot 0
   // >> local __stack y = 20; // slot 1
   // >> del x; // slot 0
   // >> // Now 'y' is actually in slot '0', which needs to be adjusted
   // NOTE: Only need to check those in the same scope, as
   //       stack variables from different scopes may not be deleted,
   //       meaning that 'var->ob_scope' is the current scope
   end = (iter = var->lv_scope->sc_namesv)+var->lv_scope->sc_namesc;
   while (iter != end) {
    locv = iter->e_local;
    if (DeeLocalVar_Check(locv) &&
        DeeLocalVar_IS_COMPILERINIT(locv) &&
        DeeLocalVar_KIND(locv) == DEE_LOCALVAR_KIND_STACK &&
        locv->lv_loc_id > minid) --locv->lv_loc_id; // Shift by 1
    ++iter;
   }
  } return 0;

  default: DEE_BUILTIN_UNREACHABLE();
 }
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LoadVar(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var,
 DEE_A_IN_OPT struct DeeLexerObject const *lexer) {
 DEE_ASSERT(DeeObject_Check(var) && DeeLocalVar_Check(var));
 DEE_ASSERTF(DeeLocalVar_IS_COMPILERINIT(var),"Variable not initialized");
 if (DeeCodeWriter_IsVarLocal(self,var)) {
  switch (DeeLocalVar_KIND(var)) {
   case DEE_LOCALVAR_KIND_LOCAL:  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_LOAD_LOC,var->lv_loc_id) != 0) return -1; break;
   case DEE_LOCALVAR_KIND_PARAM:  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_LOAD_ARG,var->lv_loc_id) != 0) return -1; break;
   case DEE_LOCALVAR_KIND_RETVAL: if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_LOAD_RET) != 0) return -1; break;
   case DEE_LOCALVAR_KIND_STATIC: if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_LOAD_CST_LOCKED,var->lv_loc_id) != 0) return -1; break;
   case DEE_LOCALVAR_KIND_THIS:   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_LOAD_THIS) != 0) return -1; break;
   case DEE_LOCALVAR_KIND_STACK:  return DeeCodeWriter_LoadStack(self,var->lv_loc_id);
   default: DEE_BUILTIN_UNREACHABLE();
  }
 } else {
  Dee_size_t ref_id;
  if DEE_UNLIKELY((ref_id = DeeCodeWriter_AllocOrGetReference(
   self,var,lexer ? TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : NULL)
   ) == (Dee_size_t)-1) return -1;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_LOAD_REF,ref_id) != 0) return -1;
 }
 DeeCodeWriter_INCSTACK(self);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreVar(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var,
 DEE_A_IN_OPT struct DeeLexerObject const *lexer) {
 DEE_ASSERT(DeeObject_Check(var) && DeeLocalVar_Check(var));
 DEE_ASSERTF(DeeCodeWriter_IsVarLocal(self,var),
             "Can't store in variable %q from different strong scope",
             lexer ? TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : "?");
 DEE_ASSERTF(self->cw_stack_size != 0,"No value given that could be stored");
 DEE_ASSERTF(DeeLocalVar_KIND(var) != DEE_LOCALVAR_KIND_THIS,"Can't store in 'this'");
 if ((var->lv_flags&(DEE_LOCALVAR_FLAG_INITIALIZED))==(DEE_LOCALVAR_FLAG_INITIALIZED) &&
     (var->lv_flags&(DEE_LOCALVAR_FLAG_INSTBOUNDC|DEE_LOCALVAR_FLAG_INSTBOUNDM))!=0) {
  // Initialized, instance-bound variable (compile as move-assign)
  // Load the variable...
  if DEE_UNLIKELY(DeeCodeWriter_LoadVar(self,var,lexer) != 0) return -1;
  // Must swap the variable and its value
  if DEE_UNLIKELY(DeeCodeWriter_RRot(self,2) != 0) return -1;
  // Copy/Move-assign the value to the variable
  return DeeCodeWriter_BinaryOp(self,(Dee_uint8_t)(
   (var->lv_flags&DEE_LOCALVAR_FLAG_INSTBOUNDC)!=0
   ? OP_COPY_ASSIGN : OP_MOVE_ASSIGN));
 }
 switch (DeeLocalVar_KIND(var)) {

  case DEE_LOCALVAR_KIND_LOCAL: { // Local variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    if DEE_UNLIKELY((var->lv_loc_id = DeeCodeWriter_AllocLocal(self,lexer ?
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : NULL)
     ) == (Dee_size_t)-1) return -1;
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
   }
   return DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_LOC,var->lv_loc_id);
  }

  case DEE_LOCALVAR_KIND_PARAM: { // Argument variable
   DEE_ASSERTF(DeeLocalVar_IS_COMPILERINIT(var),"Argument variable not initialized");
   return DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_ARG,var->lv_loc_id);
  }

  case DEE_LOCALVAR_KIND_RETVAL: { // Return value variable
   DEE_ASSERTF(DeeLocalVar_IS_COMPILERINIT(var),"Return value variable not initialized");
   return DeeCodeWriter_WriteOp(self,OP_STORE_RET);
  }

  case DEE_LOCALVAR_KIND_STATIC: { // Static variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    // Static variables are initialized to 'none' by default
    if DEE_UNLIKELY((var->lv_loc_id = DeeCodeWriter_AllocStatic(self,Dee_None,lexer ?
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : NULL)) == (Dee_size_t)-1) return -1;
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
   }
   return DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_CST,var->lv_loc_id);
  }

  case DEE_LOCALVAR_KIND_STACK: { // Stack variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    var->lv_loc_id = (Dee_size_t)(self->cw_stack_size-1);
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
    return DeeCodeWriter_Dup(self);
   }
   return DeeCodeWriter_StoreStack(self,var->lv_loc_id);
  }

  default: DEE_BUILTIN_UNREACHABLE();
 }
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreVarPop(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeLocalVarObject *var,
 DEE_A_IN_OPT struct DeeLexerObject const *lexer) {
 DEE_ASSERT(DeeObject_Check(var) && DeeLocalVar_Check(var));
 DEE_ASSERTF(DeeLocalVar_KIND(var) != DEE_LOCALVAR_KIND_THIS,"Can't store in 'this'");
 DEE_ASSERTF(DeeCodeWriter_IsVarLocal(self,var),
             "Can't store in variable %q from different strong scope",
             lexer ? TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : "?");
 if ((var->lv_flags&(DEE_LOCALVAR_FLAG_INITIALIZED))==(DEE_LOCALVAR_FLAG_INITIALIZED) &&
     (var->lv_flags&(DEE_LOCALVAR_FLAG_INSTBOUNDC|DEE_LOCALVAR_FLAG_INSTBOUNDM))!=0) {
  // Initialized, instance-bound variable (compile as move-assign)
  // Load the variable...
  if DEE_UNLIKELY(DeeCodeWriter_LoadVar(self,var,lexer) != 0) return -1;
  // Must swap the variable and its value
  if DEE_UNLIKELY(DeeCodeWriter_RRot(self,2) != 0) return -1;
  // Copy/Move-assign the value to the variable
  if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(self,(Dee_uint8_t)(
   (var->lv_flags&DEE_LOCALVAR_FLAG_INSTBOUNDC)!=0
   ? OP_COPY_ASSIGN : OP_MOVE_ASSIGN)) != 0) return -1;
  // Pop the variable (isn't used)
  return DeeCodeWriter_Pop(self);
 }
 switch (DeeLocalVar_KIND(var)) {

  case DEE_LOCALVAR_KIND_LOCAL: // Local variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    if DEE_UNLIKELY((var->lv_loc_id = DeeCodeWriter_AllocLocal(self,lexer ?
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : NULL)
     ) == (Dee_size_t)-1) return -1;
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
   }
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_LOC_POP,var->lv_loc_id) != 0) return -1;
   break;

  case DEE_LOCALVAR_KIND_PARAM: // Argument variable
   DEE_ASSERTF(DeeLocalVar_IS_COMPILERINIT(var),"Argument variable not initialized");
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_ARG_POP,var->lv_loc_id) != 0) return -1;
   break;

  case DEE_LOCALVAR_KIND_RETVAL: // Return value variable
   DEE_ASSERTF(DeeLocalVar_IS_COMPILERINIT(var),"Return value variable not initialized");
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_STORE_RET_POP) != 0) return -1;
   break;

  case DEE_LOCALVAR_KIND_STATIC: // Static variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    // Static variables are initialized to 'none' by default
    if DEE_UNLIKELY((var->lv_loc_id = DeeCodeWriter_AllocStatic(self,Dee_None,lexer ?
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),var->lv_name) : NULL)) == (Dee_size_t)-1) return -1;
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
   }
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_STORE_CST_POP,var->lv_loc_id) != 0) return -1;
   break;

  case DEE_LOCALVAR_KIND_STACK: // Stack variable
   if (!DeeLocalVar_IS_COMPILERINIT(var)) {
    var->lv_loc_id = (Dee_size_t)(self->cw_stack_size-1);
    var->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
    // Don't even need to generate code!
    // >> The variable is already stored where it belongs
    return 0;
   }
   return DeeCodeWriter_StoreStackPop(self,var->lv_loc_id);

  default: DEE_BUILTIN_UNREACHABLE();
 }
 DeeCodeWriter_DECSTACK(self);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_DelStack(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) {
 DEE_ASSERT(self);
 DEE_ASSERTF(stack_id < self->cw_stack_size,"Invalid stack id");
 /* ...axb...t */ if DEE_UNLIKELY(DeeCodeWriter_LRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* ...ab...tx */ if DEE_UNLIKELY(DeeCodeWriter_Pop(self) != 0) return -1;
 /* ...ab...t  */ return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LoadStack(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) {
 DEE_ASSERT(self);
 DEE_ASSERTF(stack_id < self->cw_stack_size,"Invalid stack id");
 /* ...axb...t  */ if DEE_UNLIKELY(DeeCodeWriter_LRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* ...ab...tx  */ if DEE_UNLIKELY(DeeCodeWriter_Dup(self) != 0) return -1;
 /* ...ab...txx */ if DEE_UNLIKELY(DeeCodeWriter_RRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* ...axb...tx */ return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreStack(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) {
 DEE_ASSERT(self);
 DEE_ASSERTF(stack_id < (self->cw_stack_size-1),"Invalid stack id");
 /* ...axb...y */ if DEE_UNLIKELY(DeeCodeWriter_LRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* ...ab...yx */ if DEE_UNLIKELY(DeeCodeWriter_Pop(self) != 0) return -1;
 /* ...ab...y  */ if DEE_UNLIKELY(DeeCodeWriter_Dup(self) != 0) return -1;
 /* ...ab...yy */ if DEE_UNLIKELY(DeeCodeWriter_RRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* ...ayb...y */ return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_StoreStackPop(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t stack_id) {
 DEE_ASSERT(self);
 DEE_ASSERTF(stack_id < (self->cw_stack_size-1),"Invalid stack id");
 /* stack: ...axb...y */ if DEE_UNLIKELY(DeeCodeWriter_LRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* stack: ...ab...yx */ if DEE_UNLIKELY(DeeCodeWriter_Pop(self) != 0) return -1;
 /* stack: ...ab...y  */ if DEE_UNLIKELY(DeeCodeWriter_RRot(self,(Dee_size_t)(self->cw_stack_size-stack_id)) != 0) return -1;
 /* stack: ...ayb...  */ return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_LRot(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t n) {
 DEE_ASSERT(n);
 DEE_ASSERT(self && self->cw_stack_size >= n);
 switch (n) {
  case 1: break;
#ifdef OP_ROT_2
  case 2: return DeeCodeWriter_WriteOp(self,OP_ROT_2);
#endif
#ifdef OP_LROT_3
  case 3: return DeeCodeWriter_WriteOp(self,OP_LROT_3);
#endif
#ifdef OP_LROT_4
  case 4: return DeeCodeWriter_WriteOp(self,OP_LROT_4);
#endif
#ifdef OP_LROT_5
  case 5: return DeeCodeWriter_WriteOp(self,OP_LROT_5);
#endif
  default: return DeeCodeWriter_WriteOpWithSizeArg(self,OP_LROT_N,n-1);
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RRot(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN Dee_size_t n) {
 DEE_ASSERT(n);
 DEE_ASSERT(self && self->cw_stack_size >= n);
 switch (n) {
  case 1: break;
#ifdef OP_ROT_2
  case 2: return DeeCodeWriter_WriteOp(self,OP_ROT_2);
#endif
#ifdef OP_RROT_3
  case 3: return DeeCodeWriter_WriteOp(self,OP_RROT_3);
#endif
#ifdef OP_RROT_4
  case 4: return DeeCodeWriter_WriteOp(self,OP_RROT_4);
#endif
#ifdef OP_RROT_5
  case 5: return DeeCodeWriter_WriteOp(self,OP_RROT_5);
#endif
  default: return DeeCodeWriter_WriteOpWithSizeArg(self,OP_RROT_N,n-1);
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_PushConstEx(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN Dee_uint8_t mode, DEE_A_INOUT DeeObject *v,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 Dee_size_t cst; Dee_uint16_t builtin_id;
 builtin_id = DeeBuiltin_Id(v);
 if (builtin_id != (Dee_uint16_t)-1) {
  if (builtin_id == 0) { // push(none)
   DEE_ASSERT(DeeNone_Check(v));
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_LOAD_NONE) != 0) return -1;
  } else { // push(<any_builtin_except_none>)
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_LOAD_BLTIN,builtin_id) != 0) return -1;
  }
#if 1 // Optimizations for special instances of intrinsic types
 } else if (DeeTuple_CheckExact(v) && DeeTuple_SIZE(v) == 0) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_EMPTY_TUPLE) != 0) return -1;
 } else if (DeeList_CheckExact(v) && DeeList_SIZE(v) == 0) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_EMPTY_LIST) != 0) return -1;
 } else if (DeeDict_CheckExact(v) && DeeDict_SIZE(v) == 0) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_EMPTY_DICT) != 0) return -1;
 } else if (DeeSet_CheckExact(v) && DeeSet_SIZE(v) == 0) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_EMPTY_SET) != 0) return -1;
 } else if (DeeCell_CheckExact(v) && DeeCell_VALUE(v) == NULL) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,OPEXT_EMPTY_CELL) != 0) return -1;
#endif
 } else {
  cst = DeeCodeWriter_AllocConst(self,v,compiler_flags);
  if DEE_UNLIKELY(cst == (Dee_size_t)-1 || DEE_UNLIKELY(
   DeeCodeWriter_WriteOpWithSizeArg(self,mode,cst) != 0)) return -1;
 }
 DeeCodeWriter_INCSTACK(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_PushConst(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_INOUT DeeObject *v,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 Dee_uint8_t opcode; DeeTypeObject *ctp; int temp;
 ctp = Dee_TYPE(v);
 // Constants that are copied the normal way
 if (DeeType_MustDeepCopy(ctp)) {
  // Constants that must be inserted with deep copies
  if DEE_UNLIKELY((temp = DeeObject_TIsImmutable(ctp,v)) < 0) return temp;
  opcode = (Dee_uint8_t)(temp ? OP_LOAD_CST : OP_LOAD_CST_DCOPY);
 } else if (DeeType_MustCopy(ctp)) {
  opcode = OP_LOAD_CST_COPY;
 } else opcode = OP_LOAD_CST; // Constants we can just insert like they are...
 return _DeeCodeWriter_PushConstEx(self,opcode,v,compiler_flags);
}


#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_TryBegin(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_OUT Dee_size_t *entry) {
 struct DeeExceptionHandlerEntry *new_handlers;
 Dee_size_t id = (*entry = self->cw_exceptc++);
 while DEE_UNLIKELY((new_handlers = (struct DeeExceptionHandlerEntry *)realloc_nz(
  self->cw_exceptv,(id+1)*sizeof(struct DeeExceptionHandlerEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 new_handlers[id].e_type = NULL; // Required for consistent state
 self->cw_exceptv = new_handlers;
 DeeCodeWriter_INCEXCEPTHANDLERS(self);
 return DeeCodeWriter_WriteOpWithSizeArg(self,OP_TRY_BEGIN,id);
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_TryEnd(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 DeeCodeWriter_DECEXCEPTHANDLERS(self);
 return DeeCodeWriter_WriteOp(self,OP_TRY_END);
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Ret(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_size_t pop_size = self->cw_stack_size;
 DEE_ASSERT(pop_size && "Can't return value (empty stack)");
 // Pop everything that stands between us
 // and returning from a clean stack
 while (--pop_size) {
  // Hide the return value from the pop opcode
  if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_ROT_2) != 0 ||
                  DeeCodeWriter_WriteOp(self,OP_POP) != 0
                  ) return -1;
 }
 // Finally return the actual value
 if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_RET) != 0) return -1;
 DeeCodeWriter_DECSTACK(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_MakeSequence(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN Dee_uint8_t op, DEE_A_IN Dee_size_t s) {
 if (s) {
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,op,s) != 0) return -1;
  DEE_ASSERT(self->cw_stack_size >= (s-1u));
  self->cw_stack_size -= (s-1u); // One element is added again (the tuple / list itself)
 } else {
  Dee_uint16_t op_ext;
  switch (op) {
   case OP_LIST: op_ext = OPEXT_EMPTY_LIST; break;
   case OP_TUPLE: op_ext = OPEXT_EMPTY_TUPLE; break;
   default: DEE_ASSERT(0 && "Invalid sequence op"); DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithArg(self,OP_EXTENDED,op_ext) != 0) return -1;
  DeeCodeWriter_INCSTACK(self);
 }
 return 0;
}

// Align the stack + exception handlers for executing a break/continue
DEE_A_RET_EXCEPT(-1) int _DeeCodeWriter_AlignBreakContinue(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_size_t stack_offset;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t except_offset;
 DEE_ASSERT(self->cw_except_size >= self->cw_loop_except_size &&
            "External exception handlers were lost while inside a loop");
 except_offset = (Dee_size_t)(self->cw_except_size-self->cw_loop_except_size);
 if (except_offset) {
  while (except_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_TRY_END) != 0) return -1;
  // Must use an expensive stack align, as we don't actually know how big it will be after the last try_end
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_STACKALIGN,self->cw_loop_stack_size) != 0) return -1;
 } else
#endif
 {
  DEE_ASSERT(self->cw_stack_size >= self->cw_loop_stack_size &&
             "Stack entries were lost while inside a loop");
  stack_offset = self->cw_stack_size-self->cw_loop_stack_size;
  while (stack_offset--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Break(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_size_t *new_list,id,arg;
 DEE_ASSERT(self && DeeCodeWriter_HasBreak(self));
 id = self->cw_break_forward_c++;
 while DEE_UNLIKELY((new_list = (Dee_size_t *)realloc_nz(
  self->cw_break_forward_v,(id+1)*sizeof(Dee_size_t))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->cw_break_forward_v = new_list;
 if DEE_UNLIKELY(_DeeCodeWriter_AlignBreakContinue(self) != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,OP_JUMP,&arg) != 0)
  return -1; // Already having reallocated the list doesn't matter
 new_list[id] = arg; // Store the future arg
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_Continue(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 DEE_ASSERT(self && DeeCodeWriter_HasBreak(self));
 if DEE_UNLIKELY(_DeeCodeWriter_AlignBreakContinue(self) != 0) return -1;
 return DeeCodeWriter_WriteOpWithSSizeArg(self,OP_JUMP,
  -(Dee_ssize_t)(DeeCodeWriter_ADDR(self)-self->cw_continue_addr));
}
#undef DeeCodeWriter_RetNone
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_RetNone(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_size_t pop_size = self->cw_stack_size;
 // Pop everything that stands between us and returning from a clean stack
 while (pop_size--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
 // TODO: Shouldn't we also pop exception handlers?
 return DeeCodeWriter_WriteOp(self,OP_RETNONE);
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_YieldExit(
 DEE_A_INOUT struct DeeCodeWriter *self) {
 Dee_size_t pop_size = self->cw_stack_size;
 // Pop everything that stands between us and returning from a clean stack
 while (pop_size--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
 // TODO: Shouldn't we also pop exception handlers?
 return DeeCodeWriter_WriteOp(self,OP_YIELDSTOP);
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_FinalizeStrongScope(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeScopeObject *scope,
 DEE_A_IN struct DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t compiler_flags) {
 struct _DeeScopeEntry *iter,*begin;
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeScope_IS_STRONG(scope));
 iter = (begin = scope->sc_namesv)+scope->sc_namesc;
 while (iter != begin) {
  --iter;
  DEE_ASSERT(DeeObject_Check(iter->e_local));
  if (DeeLocalVar_Check(iter->e_local) &&
      DEE_LOCALVARFLAGS_KIND(iter->e_local->lv_flags) == DEE_LOCALVAR_KIND_STACK &&
     (iter->e_local->lv_flags&DEE_LOCALVAR_FLAG_INITIALIZED)!=0) {
   // Only delete initialized stack variables
   if DEE_UNLIKELY(DeeCodeWriter_DelVar(self,iter->e_local,compiler_flags) != 0) return -1;
  }
 }
 return DeeCodeWriter_FinalizeLabels(self,&scope->sc_labels,lexer);
}
DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_FinalizeWeakScope(
 DEE_A_INOUT struct DeeCodeWriter *self, DEE_A_IN struct DeeScopeObject *scope,
 DEE_A_IN struct DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t compiler_flags) {
 struct _DeeScopeEntry *iter,*begin;
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!DeeScope_IS_STRONG(scope));
 iter = (begin = scope->sc_namesv)+scope->sc_namesc;
 while (iter != begin) {
  --iter;
  DEE_ASSERT(DeeObject_Check(iter->e_local));
  if (DeeLocalVar_Check(iter->e_local) &&
     (iter->e_local->lv_flags&DEE_LOCALVAR_FLAG_INITIALIZED)!=0 &&
      DEE_LOCALVARFLAGS_KIND(iter->e_local->lv_flags) == DEE_LOCALVAR_KIND_LOCAL ||
      DEE_LOCALVARFLAGS_KIND(iter->e_local->lv_flags) == DEE_LOCALVAR_KIND_STACK) {
   // Only delete initialized locals
   if DEE_UNLIKELY(DeeCodeWriter_DelVar(self,iter->e_local,compiler_flags) != 0) return -1;
  }
 }
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_AtomicOnceBegin(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_OUT struct DeeCodeWriterAtomicOnceBlock *block,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN Dee_uint32_t compiler_flags) {
 DeeString_NEW_STATIC(member_cmpxch,"compare_exchange_strong");
 DeeObject *temp; int error;
 Dee_size_t cmpxch_id;
 DEE_ASSERT(self);
 DEE_ASSERT(block);
 block->ao_flags = flags;
 if DEE_UNLIKELY((cmpxch_id = DeeCodeWriter_AllocConst(self,(
  DeeObject *)&member_cmpxch,compiler_flags)) == (Dee_size_t)-1) return -1;
 if DEE_UNLIKELY((temp = DeeAtomicUInt8_New(DEE_CODEWRITER_ATOMICONCE_STATE_NEVER)) == NULL) return -1;
 block->ao_executed = DeeCodeWriter_AllocStatic(self,temp,NULL);
 Dee_DECREF(temp);
 if DEE_UNLIKELY(block->ao_executed == (Dee_size_t)-1) return -1;
 if ((flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0)
  block->ao_begin_addr = DeeCodeWriter_ADDR(self);

 // Load the block-executed static variable
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
  self,OP_LOAD_CST_LOCKED,block->ao_executed) != 0) return -1;
 DeeCodeWriter_INCSTACK(self);

 // Call '.compare_exchange_bool(0,1)'
 if DEE_UNLIKELY((temp = Dee_BuildValue("(I8uI8u)",
  DEE_CODEWRITER_ATOMICONCE_STATE_NEVER,
  DEE_CODEWRITER_ATOMICONCE_STATE_NOW)) == NULL) return -1;
 error = DeeCodeWriter_PushConst(self,temp,compiler_flags);
 Dee_DECREF(temp);
 if DEE_UNLIKELY(error != 0) return error;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_CALL_MEMBER_C,cmpxch_id) != 0) return -1;
 DeeCodeWriter_DECSTACK(self); // -2 +1

 // Setup the jump over the initializer block
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
  self,OP_JUMP_IF_FF_POP,&block->ao_shoutinit_jmparg) != 0) return -1;
 DeeCodeWriter_DECSTACK(self);
 // Begin the try block for the atomic_once exec block
 if ((flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0 &&
     DEE_UNLIKELY(DeeCodeWriter_TryBegin(self,&block->ao_catchid) != 0)) return -1;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeCodeWriter_AtomicOnceEnd(
 DEE_A_INOUT struct DeeCodeWriter *self,
 DEE_A_IN struct DeeCodeWriterAtomicOnceBlock const *block,
 DEE_A_IN Dee_uint32_t compiler_flags) {
 DeeString_NEW_STATIC(member_load,"load");
 DeeString_NEW_STATIC(member_store,"store");
 DeeObject *temp; int error; Dee_size_t load_id,store_id;
 Dee_size_t wait_branch_jmparg,end_wait_jmparg,wait_branch_begin;
 DEE_ASSERT(self);
 DEE_ASSERT(block);
 if DEE_UNLIKELY((load_id = DeeCodeWriter_AllocConst(self,(
  DeeObject *)&member_load,compiler_flags)) == (Dee_size_t)-1) return -1;
 if DEE_UNLIKELY((store_id = DeeCodeWriter_AllocConst(self,(
  DeeObject *)&member_store,compiler_flags)) == (Dee_size_t)-1) return -1;
 if DEE_UNLIKELY((block->ao_flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0) {
  struct DeeExceptionHandlerEntry *handler;
  Dee_size_t handler_block_jmparg;
  Dee_size_t handler_finally_id;
  if DEE_UNLIKELY(DeeCodeWriter_TryEnd(self) != 0) return -1;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,OP_JUMP,&handler_block_jmparg) != 0) return -1;

  // Configure the exception handler
  handler = self->cw_exceptv+block->ao_catchid;
  handler->e_handler = DeeCodeWriter_ADDR(self);
  handler->e_kind = DeeExceptionHandleKind_ALL;
  handler->e_type = NULL;
  handler->e_store = 0;
  handler->e_stack = (Dee_size_t)self->cw_stack_size;

  if (self->cw_finally_size++ == self->cw_finally_size_min)
   self->cw_finally_size_min = self->cw_finally_size;
  if DEE_UNLIKELY(DeeCodeWriter_TryBegin(self,&handler_finally_id) != 0) return -1;;

  // Load the block-executed static variable
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
   self,OP_LOAD_CST_LOCKED,block->ao_executed) != 0) return -1;
  DeeCodeWriter_INCSTACK(self);

  // Call '.store(0)' to allow another thread to retry executing the block
  if DEE_UNLIKELY((temp = Dee_BuildValue("(I8u)",DEE_CODEWRITER_ATOMICONCE_STATE_NEVER)) == NULL) return -1;
  error = DeeCodeWriter_PushConst(self,temp,compiler_flags);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(error != 0) return error;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_CALL_MEMBER_C,store_id) != 0) return -1;
  DeeCodeWriter_DECSTACK(self); // -2 +1
  if DEE_UNLIKELY(DeeCodeWriter_Pop(self) != 0) return -1;

  // Rethrow the exception from the try-block
  if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_RETHROW) != 0) return -1;
#if 1
  DeeCodeWriter_DECEXCEPTHANDLERS(self);
#else
  // Never reached
  if DEE_UNLIKELY(DeeCodeWriter_TryEnd(self) != 0) return -1;
#endif

  // Configure the second exception handler
  handler = self->cw_exceptv+handler_finally_id;
  handler->e_store = 0;
  handler->e_type = NULL;
  handler->e_kind = DeeExceptionHandleKind_FINALLY;
  handler->e_stack = (Dee_size_t)self->cw_stack_size;
  handler->e_handler = DeeCodeWriter_ADDR(self);
  // Pop the exception handler in the finally block
  if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_EXCEPT_END) != 0) return -1;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOp(self,OP_FINALLY_END) != 0) return -1;
  --self->cw_finally_size;

  // Setup the jump across the catch handler
  if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,handler_block_jmparg,(Dee_size_t)(
   DeeCodeWriter_ADDR(self)-(handler_block_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
 }

 // Load the block-executed static variable
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
  self,OP_LOAD_CST_LOCKED,block->ao_executed) != 0) return -1;
 DeeCodeWriter_INCSTACK(self);

 // Call '.store(2)' to signal that the __atomic_once block has been successfully executed
 if DEE_UNLIKELY((temp = Dee_BuildValue("(I8u)",DEE_CODEWRITER_ATOMICONCE_STATE_DONE)) == NULL) return -1;
 error = DeeCodeWriter_PushConst(self,temp,compiler_flags);
 Dee_DECREF(temp);
 if DEE_UNLIKELY(error != 0) return error;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_CALL_MEMBER_C,store_id) != 0) return -1;
 DeeCodeWriter_DECSTACK(self); // -2 +1
 if DEE_UNLIKELY(DeeCodeWriter_Pop(self) != 0) return -1;

 // Skip the wait branch
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,OP_JUMP,&wait_branch_jmparg) != 0) return -1;

 // This is where we jump to enter the wait branch
 if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,block->ao_shoutinit_jmparg,(Dee_size_t)(
  DeeCodeWriter_ADDR(self)-(block->ao_shoutinit_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;

 // Now for the wait branch
 wait_branch_begin = DeeCodeWriter_ADDR(self); // This is where the while (1) jumps back to

 // Load the state of the block-executed static variable
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_LOAD_CST_LOCKED,block->ao_executed) != 0) return -1;
 DeeCodeWriter_INCSTACK(self);
 if DEE_UNLIKELY(DeeCodeWriter_PushConst(self,Dee_EmptyTuple,compiler_flags) != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(self,OP_CALL_MEMBER_C,load_id) != 0) return -1;
 DeeCodeWriter_DECSTACK(self); // -2 +1

 // Duplicate the state to check for retry if an exception occurred inside the other thread
 if ((block->ao_flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0 &&
     DeeCodeWriter_Dup(self) != 0) return -1;

 // Stop waiting once the state is '2'
 if DEE_UNLIKELY((temp = DeeUInt8_New(DEE_CODEWRITER_ATOMICONCE_STATE_DONE)) == NULL) return -1;
 error = DeeCodeWriter_PushConst(self,temp,compiler_flags);
 Dee_DECREF(temp);
 if DEE_UNLIKELY(error != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(self,OP_CMP_EQ) != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,OP_JUMP_IF_TT_POP,&end_wait_jmparg) != 0) return -1;
 DeeCodeWriter_DECSTACK(self); // Popped by 'OP_JUMP_IF_TT_POP'

 if ((block->ao_flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0) {
  Dee_size_t end_retry_jmparg;
#if !DEE_CODEWRITER_ATOMICONCE_STATE_NEVER
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,
   OP_JUMP_IF_TT_POP,&end_retry_jmparg) != 0) return -1;
#else
  if DEE_UNLIKELY((temp = DeeUInt8_New(DEE_CODEWRITER_ATOMICONCE_STATE_NEVER)) == NULL) return -1;
  error = DeeCodeWriter_PushConst(self,temp,compiler_flags);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(error != 0) return -1;
  if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(self,OP_CMP_EQ) != 0) return -1;
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(self,
   OP_JUMP_IF_FF_POP,&end_retry_jmparg) != 0) return -1;
#endif
  DeeCodeWriter_DECSTACK(self); // Popped by 'OP_JUMP_IF_TT_POP'

  // Jump back to the beginning if execution caused an error before
  if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(self,OP_JUMP,-(Dee_ssize_t)(
   DeeCodeWriter_ADDR(self)-block->ao_begin_addr)) != 0) return -1;

  // Skip the the alignment code for jumping back
  if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,end_retry_jmparg,(Dee_size_t)(
   DeeCodeWriter_ADDR(self)-(end_retry_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
 }

 // TODO: thread.yield();

 if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(self,OP_JUMP,-(Dee_ssize_t)(
  DeeCodeWriter_ADDR(self)-wait_branch_begin)) != 0) return -1;
 if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,end_wait_jmparg,(Dee_size_t)(
  DeeCodeWriter_ADDR(self)-(end_wait_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
 // Pop the copy of the state that was still on the stack for the 'state == 2' check
 // NOTE: This must only be done for the 'end_wait_jmparg' jump
 if DEE_UNLIKELY((block->ao_flags&DEE_CODEWRITER_ATOMICONCE_FLAG_NOEXCEPT)==0 &&
                 DeeCodeWriter_WriteOp(self,OP_POP) != 0) return -1;
 // End of wait branch. - This is where the initializer jumps once he is done
 if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(self,wait_branch_jmparg,(Dee_size_t)(
  DeeCodeWriter_ADDR(self)-(wait_branch_jmparg-sizeof(Dee_uint8_t)))) != 0) return -1;
 return 0;
}




#undef DEE_CODE_SIZE

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "codewriter.delnoop.inl"
#include "codewriter.peephole.inl"
#endif

#endif /* !GUARD_DEEMON_CODEWRITER_C */
