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


DEE_COMPILER_STRUCTURE_PACKED_BEGIN

#ifndef DEE_CODE_FILEHEADERS_DEFINED
#define DEE_CODE_FILEHEADERS_DEFINED 1
struct _DeeExceptionHandlerEntryFileHeader_ {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
};
struct _DeeExceptionHandlerEntryFileHeader {
 Dee_uint8_t                        ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 /*small*/Dee_uint64_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
 /*small*/Dee_uint64_t              ehefh_handler; /*< s.a.: 'DeeExceptionHandlerEntry::e_handler'. */
 //[optional] /*small*/Dee_uint64_t ehefh_store;   /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_VAR)] s.a.: 'DeeExceptionHandlerEntry::e_store'. */
 //[optional] object                ehefh_type;    /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_TYPED)] s.a.: 'DeeExceptionHandlerEntry::e_type'. */
};
struct _DeeCodeFileHeader_ {
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
};
struct _DeeCodeFileHeader {
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
 /*small*/Dee_uint64_t cfh_size;           /*< Size of the bytecode (== DeeCode_SIZE(...)). */
 /*small*/Dee_uint64_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
 /*small*/Dee_uint64_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
 /*small*/Dee_uint64_t cfh_exceptc;        /*< s.a.: DeeCodeObject::co_exceptc. */
 /*small*/Dee_uint64_t cfh_finallyc;       /*< s.a.: DeeCodeObject::co_finallyc. */
 /*small*/Dee_uint64_t cfh_stacksize;      /*< s.a.: DeeCodeObject::co_stacksize. */
 /*small*/Dee_uint64_t cfh_stackdata;      /*< s.a.: DeeCodeObject::co_woff_sdata (size of the stack data in bytes). */
 //       object       cfh_debug;          /*< s.a.: DeeCodeObject::co_debug. (NOTE: 'none' is an alias for NULL/empty) */
 //       object       cfh_consts;         /*< s.a.: DeeCodeObject::co_consts. (NOTE: 'none' is an alias for empty) */
 //       struct _DeeExceptionHandlerEntryFileHeader cfh_exceptv[cfh_exceptc]; /*< s.a.: DeeCodeObject::co_exceptv. */
 //       Dee_uint8_t  cfh_code[cfh_size]; /*< s.a.: DeeCodeObject::co_code. (NOTE: opargs are written in lil-endian) */
};
#endif

#if VERSION == 101
//////////////////////////////////////////////////////////////////////////
// Old headers
struct _DeeExceptionHandlerEntryFileHeader101_ {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 Dee_uint16_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
};
struct _DeeExceptionHandlerEntryFileHeader101 {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 Dee_uint16_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
 /*small*/Dee_uint64_t     ehefh_handler; /*< s.a.: 'DeeExceptionHandlerEntry::e_handler'. */
 //[optional] Dee_uint16_t ehefh_store;   /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_VAR)] s.a.: 'DeeExceptionHandlerEntry::e_store'. */
 //[optional] object       ehefh_type;    /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_TYPED)] s.a.: 'DeeExceptionHandlerEntry::e_type'. */
};
struct _DeeCodeFileHeader101_ {
          Dee_uint16_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
          Dee_uint16_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
};
struct _DeeCodeFileHeader101 {
          Dee_uint16_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
          Dee_uint16_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
 /*small*/Dee_uint64_t cfh_size;           /*< Size of the bytecode (== DeeCode_SIZE(...)). */
 /*small*/Dee_uint64_t cfh_exceptc;        /*< s.a.: DeeCodeObject::co_exceptc. */
 /*small*/Dee_uint64_t cfh_finallyc;       /*< s.a.: DeeCodeObject::co_finallyc. */
 /*small*/Dee_uint64_t cfh_stacksize;      /*< s.a.: DeeCodeObject::co_stacksize. */
 //       object       cfh_debug;          /*< s.a.: DeeCodeObject::co_debug. (NOTE: 'none' is an alias for NULL/empty) */
 //       object       cfh_consts;         /*< s.a.: DeeCodeObject::co_consts. (NOTE: 'none' is an alias for empty) */
 //       struct _DeeExceptionHandlerEntryFileHeader cfh_exceptv[cfh_exceptc]; /*< s.a.: DeeCodeObject::co_exceptv. */
 //       Dee_uint8_t  cfh_code[cfh_size]; /*< s.a.: DeeCodeObject::co_code. (NOTE: opargs are written in lil-endian) */
};
#else
#define LATEST_VERSION
#endif
DEE_COMPILER_STRUCTURE_PACKED_END

DEE_COMPILER_MSVC_WARNING_PUSH(4366)
#ifndef LATEST_VERSION
static DeeCodeObject *DEE_CALL DEE_PP_CAT_2(_deecode_tp_marshal_new,VERSION)
#else
static DeeCodeObject *DEE_CALL _deecode_tp_marshal_new
#endif
(DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *map,
 PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeCodeObject *result; struct _DeeCodeFileHeader header;
 struct DeeExceptionHandlerEntry *iter,*end;
#ifdef LATEST_VERSION
 // Check for an older version
 if (map->mrm_version <= DEE_MARSHAL_V101) {
  DEE_LVERBOSE3("[Backward] Reading code object from v101\n");
  return _deecode_tp_marshal_new101(NULL,infile,map,after_alloc,after_alloc_data);
 }
#endif

#if VERSION <= 101
 {
  struct _DeeCodeFileHeader101_ header101; // Backwards compatibility with 101:1
  if (DeeFile_ReadAll(infile,&header101,sizeof(header101)) != 0) return NULL;
  if (DeeFile_GetLeSmall64(infile,&header.cfh_size) != 0) return NULL;
  header.cfh_flags = header101.cfh_flags;
  header.cfh_nlocals = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(header101.cfh_nlocals);
  header.cfh_exceptsize = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(header101.cfh_exceptsize);
 }
#else
 // The new way (nlocals is now a small64)
 if (DeeFile_ReadAll(infile,&header,Dee_OFFSETOF(struct _DeeCodeFileHeader,cfh_size)) != 0
  || DeeFile_GetLeSmall64(infile,&header.cfh_size) != 0
  || DeeFile_GetLeSmall64(infile,&header.cfh_nlocals) != 0
  || DeeFile_GetLeSmall64(infile,&header.cfh_exceptsize) != 0
  ) return NULL;
#endif

#ifndef DEE_PLATFORM_LIL_ENDIAN
 header.cfh_flags = DEE_BUILTIN_LESWAP16(header.cfh_flags);
#endif
 if (DeeFile_GetLeSmall64(infile,&header.cfh_exceptc) != 0 ||
     DeeFile_GetLeSmall64(infile,&header.cfh_finallyc) != 0 ||
     DeeFile_GetLeSmall64(infile,&header.cfh_stacksize) != 0
     ) return NULL;
#if VERSION <= 101
 header.cfh_stackdata = 0;
#else
 if (DeeFile_GetLeSmall64(infile,&header.cfh_stackdata) != 0) return NULL;
#endif
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(header.cfh_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code byte-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_size,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_nlocals > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code locals-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_nlocals,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_exceptc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code exception-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_exceptc,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_finallyc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code finally-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_finallyc,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_stacksize > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code stack-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_stacksize,(Dee_size_t)-1);
  return NULL;
 }
#if VERSION >= 101
 if DEE_UNLIKELY(header.cfh_stackdata > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code stack-data is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_stackdata,(Dee_size_t)-1);
  return NULL;
 }
#endif
#endif
 result = (DeeCodeObject *)DeeObject_Malloc(Dee_OFFSETOF(DeeCodeObject,co_code)+
                                            ((Dee_size_t)header.cfh_size)*sizeof(Dee_uint8_t));
 if DEE_UNLIKELY(!result) return NULL;
 DeeObject_INIT(result,&DeeCode_Type);
 (*after_alloc)((DeeObject *)result,after_alloc_data);
 // Not for initializing the code object
 result->co_size = (Dee_size_t)header.cfh_size;
 result->co_flags = header.cfh_flags;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 result->co_exceptsize = (Dee_size_t)header.cfh_exceptsize;
 result->co_exceptc = (Dee_size_t)header.cfh_exceptc;
 result->co_finallyc = (Dee_size_t)header.cfh_finallyc;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 result->co_nlocals = (Dee_size_t)header.cfh_nlocals;
 result->co_stacksize = (Dee_size_t)header.cfh_stacksize;
 if ((result->co_debug = (DeeCodeDebugInfoObject *)
  DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_r: _DeeObject_DELETE(&DeeCode_Type,result); return NULL;
 }
 if (DeeNone_Check(result->co_debug)) Dee_CLEAR(result->co_debug);
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)result->co_debug,&DeeCodeDebugInfo_Type) != 0) {
err_r_debug: Dee_XDECREF(result->co_debug); goto err_r;
 } else {
  if (!result->co_debug->di_name && !result->co_debug->di_local_names &&
      !result->co_debug->di_const_names && !result->co_debug->di_ref_names &&
      !result->co_debug->di_arg_names && !result->co_debug->di_lno &&
      !result->co_debug->di_fno && !result->co_debug->di_file_names) Dee_CLEAR(result->co_debug);
 }
 if ((result->co_consts = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_r_debug;
 if (DeeNone_Check(result->co_consts)) { Dee_DECREF(result->co_consts); Dee_INCREF(result->co_consts = (DeeTupleObject *)Dee_EmptyTuple); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)result->co_consts,&DeeTuple_Type) != 0) {
err_r_const: Dee_DECREF(result->co_consts); goto err_r_debug;
 }
 if (result->co_exceptc) {
  while ((result->co_exceptv = (struct DeeExceptionHandlerEntry *)malloc_nz(
   result->co_exceptc*sizeof(struct DeeExceptionHandlerEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   goto err_r_const;
  }
  end = (iter = result->co_exceptv)+result->co_exceptc;
  do {
   int error;
   struct _DeeExceptionHandlerEntryFileHeader hheader;
#if VERSION <= 101
   Dee_uint16_t ehefh_stack; // Backwards compatibility
   error = DeeFile_ReadAll(infile,&hheader,Dee_OFFSETOF(
    struct _DeeExceptionHandlerEntryFileHeader,ehefh_stack));
   if (error == 0) {
    error = DeeFile_ReadAll(infile,&ehefh_stack,2);
    hheader.ehefh_stack = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(ehefh_stack);
   }
#else
   error = DeeFile_ReadAll(infile,&hheader,Dee_OFFSETOF(
    struct _DeeExceptionHandlerEntryFileHeader,ehefh_stack));
   if (error == 0) error = DeeFile_GetLeSmall64(infile,&hheader.ehefh_stack);
#endif
   if DEE_UNLIKELY(error != 0) {
err_r_except_iter:
    while (iter != result->co_exceptv) { --iter; _DeeExceptionHandlerEntry_Quit(iter); }
    free_nn(result->co_exceptv);
    goto err_r_const;
   }
   if (DeeFile_GetLeSmall64(infile,&hheader.ehefh_handler) != 0) goto err_r_except_iter;
   if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
    hheader.ehefh_stack >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
    (Dee_size_t)hheader.ehefh_stack >= (Dee_size_t)header.cfh_stacksize) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Exception handler stack alignment is too large: (%I64u exceeds maximum of %I64u)",
                        hheader.ehefh_stack,header.cfh_stacksize);
    goto err_r_except_iter;
   }
   if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
    hheader.ehefh_handler >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
    (Dee_size_t)hheader.ehefh_handler >= (Dee_size_t)header.cfh_size) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Exception handler address is out of bounds: (%I64u exceeds maximum of %I64u)",
                        hheader.ehefh_handler,header.cfh_size);
    goto err_r_except_iter;
   }
   iter->e_kind = (DeeExceptionHandlerKind)hheader.ehefh_kind;
   iter->e_stack = (Dee_size_t)hheader.ehefh_stack;
   iter->e_handler = (Dee_size_t)hheader.ehefh_handler;
   if ((iter->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0) {
    Dee_uint64_t local_id;
#if VERSION <= 101
    Dee_uint16_t local_id16;
    if DEE_UNLIKELY(DeeFile_GetLe(infile,local_id16) != 0) goto err_r_except_iter;
    local_id = (Dee_uint64_t)local_id16;
#else
    if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&local_id) != 0) goto err_r_except_iter;
    if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
     local_id >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
     (Dee_size_t)local_id > result->co_nlocals) {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Exception variable storage slot is out of bounds: (%I64u exceeds maximum of %Iu)",
                         local_id,result->co_nlocals);
     goto err_r_except_iter;
    }
#endif
    iter->e_store = (Dee_size_t)local_id;
   } else iter->e_store = 0;
   if ((iter->e_kind&DeeExceptionHandleKind_FLAG_TYPED)!=0) {
    if DEE_UNLIKELY((iter->e_type = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_r_except_iter;
    if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)iter->e_type,&DeeType_Type) != 0) { Dee_DECREF(iter->e_type); goto err_r_except_iter; }
   } else iter->e_type = NULL;
  } while (++iter != end);
 } else result->co_exceptv = NULL;


 if DEE_UNLIKELY(DeeFile_ReadAll(infile,result->co_code,(Dee_size_t)header.cfh_size) != 0) {
  if (!result->co_exceptc) goto err_r_const;
  iter = result->co_exceptv+result->co_exceptc;
  goto err_r_except_iter;
 }
#if DEE_PLATFORM_ENDIAN != DEE_MARSHAL_ENDIAN_BYTECODE
 { // Swap endian on opargs
  Dee_uint8_t *iter,*end;
  end = (iter = DeeCode_CODE(result))+DeeCode_SIZE(result);
  while (iter != end) {
   if ((*iter++&OPFLAG_ARG)!=0) {
    *(Dee_uint16_t *)iter = DEE_BUILTIN_BSWAP16(*(Dee_uint16_t *)iter);
    iter += 2;
   }
  }
 }
#endif
#if VERSION <= 101
 {
  Dee_uint8_t *iter,*end;
  // Delete OP_DEBUG_FILE opcodes (no longer used)
  // NOTE: By doing this, we will loose filename debug information,
  //       but the user simply has to recompile to get them back.
  end = (iter = DeeCode_CODE(result))+DeeCode_SIZE(result);
  while (iter != end) {
   if (*iter == v101_OP_DEBUG_FILE) {
    // Just delete it, and its argument
    iter[0] = OP_NOOP;
    iter[1] = OP_NOOP;
    iter[2] = OP_NOOP;
   } else {
    if ((*iter++&OPFLAG_ARG)!=0) iter += 2;
   }
  }
 }
#endif

 // Calculate the w-buffer sizes
 DeeCode_CALCULATE_WBUFFERSIZE(result,(Dee_size_t)header.cfh_stackdata);
 DeeAtomicMutex_Init(&result->co_consts_lock);
 return result;
}

#ifdef LATEST_VERSION
#undef LATEST_VERSION
#endif
#undef VERSION
