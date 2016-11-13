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
#ifndef GUARD_DEEMON_BUILTINS_C
#define GUARD_DEEMON_BUILTINS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// include/*
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/cfunction.h>
#include <deemon/class.h>
#include <deemon/compiler/code.h>
#include <deemon/compiler/lexer.h>
#include <deemon/dex.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/fs_api.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/memberfunction.h>
#include <deemon/mp/mutex.h>
#include <deemon/mp/pipe.h>
#include <deemon/mp/process.h>
#include <deemon/mp/thread.h>
#include <deemon/mp/tls.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/random.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/socket.h>
#include <deemon/string.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/time.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/xast.h>
#include <deemon/runtime/builtin_functions.h>

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// NOTE: Changing indices in this list breaks already compiled code
// Feature-specific object should be replaced with 'Dee_None' if not available
// NOTE: The ids in here can be used with the '__builtin_object'
//       builtin function to retrieve the listed objects
DeeObject *const _DeeBuiltin_ID2Type[] = {
 //////////////////////////////////////////////////////////////////////////
 // New in v100
 /*0x0000*/Dee_None, // Must be 0
 /*0x0001*/Dee_EmptyString,
 /*0x0002*/Dee_EmptyTraceback,
 /*0x0003*/Dee_True,
 /*0x0004*/Dee_False,
 /*0x0005*/Dee_EmptyTuple,
 /*0x0006*/(DeeObject *)&DeeList_Type, // Used-to-be: Dee_EmptyFunction
 /*0x0007*/(DeeObject *)&DeeObject_Type,
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 /*0x0008*/(DeeObject *)&DeeUtf8String_Type, // Used-to-be: DeeString_Type
#else
 /*0x0008*/(DeeObject *)Dee_None,
#endif
 /*0x0009*/(DeeObject *)&DeeWeakref_Type,
 /*0x000A*/(DeeObject *)&DeeTuple_Type,
 /*0x000B*/(DeeObject *)&DeeType_Type,
 /*0x000C*/(DeeObject *)&DeeNone_Type,
 /*0x000D*/(DeeObject *)&DeeBuiltinFunction___builtin_object,
 /*0x000E*/(DeeObject *)&DeeTupleIterator_Type,
 /*0x000F*/(DeeObject *)&DeeDict_Type,
 /*0x0010*/(DeeObject *)&DeeDictKeys_Type,
 /*0x0011*/(DeeObject *)&DeeDictItems_Type,
 /*0x0012*/(DeeObject *)&DeeDictValues_Type,
 /*0x0013*/(DeeObject *)&DeeDictIterator_Type,
 /*0x0014*/(DeeObject *)&DeeDictKeysIterator_Type,
 /*0x0015*/(DeeObject *)&DeeDictItemsIterator_Type,
 /*0x0016*/(DeeObject *)&DeeFloat_Type,
 /*0x0017*/(DeeObject *)&DeeDouble_Type,
 /*0x0018*/(DeeObject *)&DeeLDouble_Type,
 /*0x0019*/(DeeObject *)&DeeBool_Type,
 /*0x001A*/(DeeObject *)&DeeInt8_Type,
 /*0x001B*/(DeeObject *)&DeeInt16_Type,
 /*0x001C*/(DeeObject *)&DeeInt32_Type,
 /*0x001D*/(DeeObject *)&DeeInt64_Type,
 /*0x001E*/(DeeObject *)&DeeUInt8_Type,
 /*0x001F*/(DeeObject *)&DeeUInt16_Type,
 /*0x0020*/(DeeObject *)&DeeUInt32_Type,
 /*0x0021*/(DeeObject *)&DeeUInt64_Type,
 /*0x0022*/(DeeObject *)&DeeAtomicInt8_Type,
 /*0x0023*/(DeeObject *)&DeeAtomicInt16_Type,
 /*0x0024*/(DeeObject *)&DeeAtomicInt32_Type,
 /*0x0025*/(DeeObject *)&DeeAtomicInt64_Type,
 /*0x0026*/(DeeObject *)&DeeAtomicUInt8_Type,
 /*0x0027*/(DeeObject *)&DeeAtomicUInt16_Type,
 /*0x0028*/(DeeObject *)&DeeAtomicUInt32_Type,
 /*0x0029*/(DeeObject *)&DeeAtomicUInt64_Type,
 /*0x002A*/(DeeObject *)&DeeBuiltinFunction___repr__,
 /*0x002B*/(DeeObject *)&DeeBuiltinFunction___type__,
 /*0x002C*/(DeeObject *)&DeeBuiltinFunction___copy__,
 /*0x002D*/(DeeObject *)&DeeBuiltinFunction___move__,
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 /*0x002E*/(DeeObject *)&DeeBuiltinFunction___builtin_last_exception,
 /*0x002F*/(DeeObject *)&DeeBuiltinFunction___builtin_last_traceback,
#else /* DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS */
 /*0x002E*/Dee_None,
 /*0x002F*/Dee_None,
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS */
 /*0x0030*/(DeeObject *)&DeeBuiltinFunction___copyassign__,
 /*0x0031*/(DeeObject *)&DeeBuiltinFunction___not__,
 /*0x0032*/(DeeObject *)&DeeBuiltinFunction___neg__,
 /*0x0033*/(DeeObject *)&DeeBuiltinFunction___pos__,
 /*0x0034*/(DeeObject *)&DeeBuiltinFunction___inv__,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x0035*/(DeeObject *)&DeeBuiltinFunction___ref__,
 /*0x0036*/(DeeObject *)&DeeBuiltinFunction___deref__,
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x0035*/(DeeObject *)Dee_None,
 /*0x0036*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x0037*/(DeeObject *)&DeeBuiltinFunction___inc__,
 /*0x0038*/(DeeObject *)&DeeBuiltinFunction___dec__,
 /*0x0039*/(DeeObject *)&DeeBuiltinFunction___incpost__,
 /*0x003A*/(DeeObject *)&DeeBuiltinFunction___decpost__,
 /*0x003B*/(DeeObject *)&DeeBuiltinFunction___lo__,
 /*0x003C*/(DeeObject *)&DeeBuiltinFunction___le__,
 /*0x003D*/(DeeObject *)&DeeBuiltinFunction___eq__,
 /*0x003E*/(DeeObject *)&DeeBuiltinFunction___ne__,
 /*0x003F*/(DeeObject *)&DeeBuiltinFunction___gr__,
 /*0x0040*/(DeeObject *)&DeeBuiltinFunction___ge__,
 /*0x0041*/(DeeObject *)&DeeBuiltinFunction___add__,
 /*0x0042*/(DeeObject *)&DeeBuiltinFunction___iadd__,
 /*0x0043*/(DeeObject *)&DeeBuiltinFunction___sub__,
 /*0x0044*/(DeeObject *)&DeeBuiltinFunction___isub__,
 /*0x0045*/(DeeObject *)&DeeBuiltinFunction___mul__,
 /*0x0046*/(DeeObject *)&DeeBuiltinFunction___imul__,
 /*0x0047*/(DeeObject *)&DeeBuiltinFunction___div__,
 /*0x0048*/(DeeObject *)&DeeBuiltinFunction___idiv__,
 /*0x0049*/(DeeObject *)&DeeBuiltinFunction___mod__,
 /*0x004A*/(DeeObject *)&DeeBuiltinFunction___imod__,
 /*0x004B*/(DeeObject *)&DeeBuiltinFunction___shl__,
 /*0x004C*/(DeeObject *)&DeeBuiltinFunction___ishl__,
 /*0x004D*/(DeeObject *)&DeeBuiltinFunction___shr__,
 /*0x004E*/(DeeObject *)&DeeBuiltinFunction___ishr__,
 /*0x004F*/(DeeObject *)&DeeBuiltinFunction___and__,
 /*0x0050*/(DeeObject *)&DeeBuiltinFunction___iand__,
 /*0x0051*/(DeeObject *)&DeeBuiltinFunction___or__,
 /*0x0052*/(DeeObject *)&DeeBuiltinFunction___ior__,
 /*0x0053*/(DeeObject *)&DeeBuiltinFunction___xor__,
 /*0x0054*/(DeeObject *)&DeeBuiltinFunction___ixor__,
 /*0x0055*/(DeeObject *)&DeeBuiltinFunction___pow__,
 /*0x0056*/(DeeObject *)&DeeBuiltinFunction___ipow__,
 /*0x0057*/(DeeObject *)&DeeBuiltinFunction___call__,
 /*0x0058*/(DeeObject *)&DeeBuiltinFunction___size__,
 /*0x0059*/(DeeObject *)&DeeBuiltinFunction___getitem__,
 /*0x005A*/(DeeObject *)&DeeBuiltinFunction___setitem__,
 /*0x005B*/(DeeObject *)&DeeBuiltinFunction___delitem__,
 /*0x005C*/(DeeObject *)&DeeBuiltinFunction___getrange__,
 /*0x005D*/(DeeObject *)&DeeBuiltinFunction___setrange__,
 /*0x005E*/(DeeObject *)&DeeBuiltinFunction___delrange__,
 /*0x005F*/(DeeObject *)&DeeBuiltinFunction___is__,
 /*0x0060*/(DeeObject *)&DeeBuiltinFunction___contains__,
 /*0x0061*/(DeeObject *)&DeeBuiltinFunction___getattr__,
 /*0x0062*/(DeeObject *)&DeeBuiltinFunction___delattr__,
 /*0x0063*/(DeeObject *)&DeeBuiltinFunction___setattr__,
 /*0x0064*/(DeeObject *)&DeeBuiltinFunction___iterself__,
 /*0x0065*/(DeeObject *)&DeeBuiltinFunction___iternext__,
 /*0x0066*/(DeeObject *)&DeeFileType_Type,
 /*0x0067*/(DeeObject *)&DeeFile_Type,
 /*0x0068*/(DeeObject *)&DeeFileIO_Type,
 /*0x0069*/(DeeObject *)&DeeFileReader_Type,
 /*0x006A*/(DeeObject *)&DeeFileWriter_Type,
 /*0x006B*/(DeeObject *)&DeeFileIterator_Type,
 /*0x006C*/(DeeObject *)&DeeSignalType_Signal,
 /*0x006D*/(DeeObject *)&DeeSignalType_Interrupt,
 /*0x006E*/(DeeObject *)&DeeSignalType_StopIteration,
 /*0x006F*/(DeeObject *)&DeeErrorType_Error,
 /*0x0070*/(DeeObject *)&DeeErrorType_AttributeError,
 /*0x0071*/(DeeObject *)&DeeErrorType_CompilerError,
 /*0x0072*/(DeeObject *)&DeeErrorType_KeyError,
 /*0x0073*/(DeeObject *)&DeeErrorType_NoMemory,
 /*0x0074*/(DeeObject *)&DeeErrorType_NotImplemented,
 /*0x0075*/(DeeObject *)&DeeErrorType_RuntimeError,
 /*0x0076*/(DeeObject *)&DeeErrorType_AssertionError,
 /*0x0077*/(DeeObject *)&DeeErrorType_TypeError,
 /*0x0078*/(DeeObject *)&DeeErrorType_ValueError,
 /*0x0079*/(DeeObject *)&DeeErrorType_SystemError,
 /*0x007A*/(DeeObject *)&DeeErrorType_IOError,
#ifndef DEE_WITHOUT_THREADS
 /*0x007B*/(DeeObject *)&DeeSemaphore_Type,
 /*0x007C*/(DeeObject *)&DeeMutex_Type,
 /*0x007D*/(DeeObject *)&DeeTls_Type,
 /*0x007E*/(DeeObject *)&DeeThread_Type,
#else
 /*0x007B*/(DeeObject *)Dee_None,
 /*0x007C*/(DeeObject *)Dee_None,
 /*0x007D*/(DeeObject *)&DeeTls_Type,
 /*0x007E*/(DeeObject *)Dee_None,
#endif
 /*0x007F*/(DeeObject *)&DeeTime_Type,
 /*0x0080*/(DeeObject *)&DeeStructuredType_Type,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x0081*/(DeeObject *)&DeePointerType_Type,
 /*0x0082*/(DeeObject *)&DeeLValueType_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x0081*/(DeeObject *)Dee_None,
 /*0x0082*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x0083*/(DeeObject *)&DeeStructured_Type,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x0084*/(DeeObject *)&DeePointer_Type,
 /*0x0085*/(DeeObject *)&DeeLValue_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x0084*/(DeeObject *)Dee_None,
 /*0x0085*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x0086*/(DeeObject *)&DeeBuiltinFunction___builtin_pointer_add,
 /*0x0087*/(DeeObject *)&DeeBuiltinFunction___builtin_pointer_rem,
 /*0x0088*/(DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add,
 /*0x0089*/(DeeObject *)&DeeBuiltinFunction___builtin_lvalue_rem,
#else
 /*0x0086*/(DeeObject *)Dee_None,
 /*0x0087*/(DeeObject *)Dee_None,
 /*0x0088*/(DeeObject *)Dee_None,
 /*0x0089*/(DeeObject *)Dee_None,
#endif
 /*0x008A*/(DeeObject *)&DeeTraceback_Type,
 /*0x008B*/(DeeObject *)&DeeTracebackIterator_Type,
 /*0x008C*/(DeeObject *)&DeeTracebackEntry_Type,
 /*0x008D*/(DeeObject *)&DeeMemberFunction_Type,
 /*0x008E*/(DeeObject *)&DeeCFunction_Type,
 /*0x008F*/(DeeObject *)&DeeSharedLib_Type,
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 /*0x0090*/(DeeObject *)&DeeForeignFunctionType_Type, // Used-to-be: DeeExternFunction_Type
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
 /*0x0090*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
 /*0x0091*/(DeeObject *)&DeeCode_Type,
 /*0x0092*/(DeeObject *)&_Dee_EmptyCode,
 /*0x0093*/(DeeObject *)&DeeXAst_Type,
 /*0x0094*/(DeeObject *)&DeeDexModule_Type, // Used-to-bo: _DeeTokenIDList_Type
 /*0x0095*/(DeeObject *)&DeeErrorType_DexError, // Used-to-bo: __DeeTokenIDList_EmptyList
 /*0x0096*/(DeeObject *)&DeeToken_Type,
 /*0x0097*/(DeeObject *)&DeeLocalVar_Type,
 /*0x0098*/(DeeObject *)&DeeLexer_Type,
 /*0x0099*/(DeeObject *)&DeeSAst_Type,
 /*0x009A*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getcwd,
 /*0x009B*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_chdir,
 /*0x009C*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gethome,
 /*0x009D*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_listenv,
 /*0x009E*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getenv,
 /*0x009F*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_hasenv,
 /*0x00A0*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_delenv,
 /*0x00A1*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_setenv,
 /*0x00A2*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_head,
 /*0x00A3*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_tail,
 /*0x00A4*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_file,
 /*0x00A5*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_ext,
 /*0x00A6*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_drive,
 /*0x00A7*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_inctrail,
 /*0x00A8*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_exctrail,
 /*0x00A9*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_abs,
 /*0x00AA*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_rel,
 /*0x00AB*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_join,
 /*0x00AC*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_expanduser,
 /*0x00AD*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_path_expandvars,
 /*0x00AE*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getatime,
 /*0x00AF*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getctime,
 /*0x00B0*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getmtime,
 /*0x00B1*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gettimes,
 /*0x00B2*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isabs,
 /*0x00B3*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isfile,
 /*0x00B4*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isdir,
 /*0x00B5*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_islink,
 /*0x00B6*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isdrive,
 /*0x00B7*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_ismount,
 /*0x00B8*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_ishidden,
 /*0x00B9*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_exists,
 /*0x00BA*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_remove,
 /*0x00BB*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_rmfile,
 /*0x00BC*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_mkdir,
 /*0x00BD*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_rmdir,
 /*0x00BE*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_copy,
 /*0x00BF*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_move,
 /*0x00C0*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_link,
 /*0x00C1*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_readlink,
 /*0x00C2*/(DeeObject *)&DeeBuiltinFunction___builtin_assertion_failed,
 /*0x00C3*/(DeeObject *)&DeeFSDir_Type,
 /*0x00C4*/(DeeObject *)&DeeFSDirIterator_Type,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x00C5*/(DeeObject *)&DeeBuiltinFunction___builtin_memcpy,
 /*0x00C6*/(DeeObject *)&DeeBuiltinFunction___builtin_memmove,
 /*0x00C7*/(DeeObject *)&DeeBuiltinFunction___builtin_memcmp,
 /*0x00C8*/(DeeObject *)&DeeBuiltinFunction___builtin_strcoll,
 /*0x00C9*/(DeeObject *)&DeeBuiltinFunction___builtin_strcmp,
 /*0x00CA*/(DeeObject *)&DeeBuiltinFunction___builtin_strncmp,
 /*0x00CB*/(DeeObject *)&DeeBuiltinFunction___builtin_strxfrm,
 /*0x00CC*/(DeeObject *)&DeeBuiltinFunction___builtin_strcpy,
 /*0x00CD*/(DeeObject *)&DeeBuiltinFunction___builtin_strncpy,
 /*0x00CE*/(DeeObject *)&DeeBuiltinFunction___builtin_strcat,
 /*0x00CF*/(DeeObject *)&DeeBuiltinFunction___builtin_memchr,
 /*0x00D0*/(DeeObject *)&DeeBuiltinFunction___builtin_strchr,
 /*0x00D1*/(DeeObject *)&DeeBuiltinFunction___builtin_strcspn,
 /*0x00D2*/(DeeObject *)&DeeBuiltinFunction___builtin_strpbrk,
 /*0x00D3*/(DeeObject *)&DeeBuiltinFunction___builtin_strrchr,
 /*0x00D4*/(DeeObject *)&DeeBuiltinFunction___builtin_strspn,
 /*0x00D5*/(DeeObject *)&DeeBuiltinFunction___builtin_strstr,
 /*0x00D6*/(DeeObject *)&DeeBuiltinFunction___builtin_strtok,
 /*0x00D7*/(DeeObject *)&DeeBuiltinFunction___builtin_strerror,
 /*0x00D8*/(DeeObject *)&DeeBuiltinFunction___builtin_strlen,
 /*0x00D9*/(DeeObject *)&DeeBuiltinFunction___builtin_strnlen,
 /*0x00DA*/(DeeObject *)&DeeBuiltinFunction___builtin_malloc,
 /*0x00DB*/(DeeObject *)&DeeBuiltinFunction___builtin_calloc,
 /*0x00DC*/(DeeObject *)&DeeBuiltinFunction___builtin_realloc,
 /*0x00DD*/(DeeObject *)&DeeBuiltinFunction___builtin_free,
 /*0x00DE*/(DeeObject *)&DeeBuiltinFunction___builtin_aligned_malloc,
 /*0x00DF*/(DeeObject *)&DeeBuiltinFunction___builtin_aligned_calloc,
 /*0x00E0*/(DeeObject *)&DeeBuiltinFunction___builtin_aligned_realloc,
 /*0x00E1*/(DeeObject *)&DeeBuiltinFunction___builtin_aligned_free,
#else
 /*0x00C5*/(DeeObject *)Dee_None,
 /*0x00C6*/(DeeObject *)Dee_None,
 /*0x00C7*/(DeeObject *)Dee_None,
 /*0x00C8*/(DeeObject *)Dee_None,
 /*0x00C9*/(DeeObject *)Dee_None,
 /*0x00CA*/(DeeObject *)Dee_None,
 /*0x00CB*/(DeeObject *)Dee_None,
 /*0x00CC*/(DeeObject *)Dee_None,
 /*0x00CD*/(DeeObject *)Dee_None,
 /*0x00CE*/(DeeObject *)Dee_None,
 /*0x00CF*/(DeeObject *)Dee_None,
 /*0x00D0*/(DeeObject *)Dee_None,
 /*0x00D1*/(DeeObject *)Dee_None,
 /*0x00D2*/(DeeObject *)Dee_None,
 /*0x00D3*/(DeeObject *)Dee_None,
 /*0x00D4*/(DeeObject *)Dee_None,
 /*0x00D5*/(DeeObject *)Dee_None,
 /*0x00D6*/(DeeObject *)Dee_None,
 /*0x00D7*/(DeeObject *)Dee_None,
 /*0x00D8*/(DeeObject *)Dee_None,
 /*0x00D9*/(DeeObject *)Dee_None,
 /*0x00DA*/(DeeObject *)Dee_None,
 /*0x00DB*/(DeeObject *)Dee_None,
 /*0x00DC*/(DeeObject *)Dee_None,
 /*0x00DD*/(DeeObject *)Dee_None,
 /*0x00DE*/(DeeObject *)Dee_None,
 /*0x00DF*/(DeeObject *)Dee_None,
 /*0x00E0*/(DeeObject *)Dee_None,
 /*0x00E1*/(DeeObject *)Dee_None,
#endif
#if DEE_PLATFORM_HAVE_PIPES
 /*0x00E2*/(DeeObject *)&DeePipe_Type,
 /*0x00E3*/(DeeObject *)&DeePipeReader_Type,
 /*0x00E4*/(DeeObject *)&DeePipeWriter_Type,
#else /* DEE_PLATFORM_HAVE_PIPES */
 /*0x00E2*/(DeeObject *)Dee_None,
 /*0x00E3*/(DeeObject *)Dee_None,
 /*0x00E4*/(DeeObject *)Dee_None,
#endif /* !DEE_PLATFORM_HAVE_PIPES */
 /*0x00E5*/(DeeObject *)&DeeErrorType_SegFault, // Used-to-be: &DeeBuiltinFunction___builtin_gc_collect,
 /*0x00E6*/(DeeObject *)&DeeBuiltinFunction___builtin_gc_collect_now,
 /*0x00E7*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isxfile,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x00E8*/(DeeObject *)&DeeBuiltinFunction___builtin_strdup,
 /*0x00E9*/(DeeObject *)&DeeBuiltinFunction___builtin_strndup,
 /*0x00EA*/(DeeObject *)&DeeBuiltinFunction___builtin_strncat,
#else
 /*0x00E8*/(DeeObject *)Dee_None,
 /*0x00E9*/(DeeObject *)Dee_None,
 /*0x00EA*/(DeeObject *)Dee_None,
#endif
 /*0x00EB*/(DeeObject *)&DeeFSQuery_Type,
 /*0x00EC*/(DeeObject *)&DeeFSQueryIterator_Type,
#ifdef DEE_PLATFORM_WINDOWS
 /*0x00ED*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32hardlink,
 /*0x00EE*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32modulename,
#else
 /*0x00ED*/(DeeObject *)Dee_None,
 /*0x00EE*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_RUNTIME_HAVE_EXIT
 /*0x00EF*/(DeeObject *)&DeeBuiltinFunction___builtin_exit,
 /*0x00F0*/(DeeObject *)&DeeBuiltinFunction___builtin__exit,
 /*0x00F1*/(DeeObject *)&DeeBuiltinFunction___builtin_fatal,
 /*0x00F2*/(DeeObject *)&DeeBuiltinFunction___builtin_atexit,
#else
 /*0x00EF*/(DeeObject *)Dee_None,
 /*0x00F0*/(DeeObject *)Dee_None,
 /*0x00F1*/(DeeObject *)Dee_None,
 /*0x00F2*/(DeeObject *)Dee_None,
#endif
 /*0x00F3*/(DeeObject *)&DeeYieldFunction_Type,
 /*0x00F4*/(DeeObject *)&DeeYieldFunctionIterator_Type,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x00F5*/(DeeObject *)&DeeBuiltinFunction___builtin_memset,
 /*0x00F6*/(DeeObject *)&DeeBuiltinFunction___builtin_mempcpy,
 /*0x00F7*/(DeeObject *)&DeeBuiltinFunction___builtin_memccpy,
 /*0x00F8*/(DeeObject *)&DeeBuiltinFunction___builtin_rawmemchr,
 /*0x00F9*/(DeeObject *)&DeeBuiltinFunction___builtin_memrchr,
 /*0x00FA*/(DeeObject *)&DeeBuiltinFunction___builtin_strchrnul,
 /*0x00FB*/(DeeObject *)&DeeBuiltinFunction___builtin_strcasestr,
 /*0x00FC*/(DeeObject *)&DeeBuiltinFunction___builtin_strcasecmp,
 /*0x00FD*/(DeeObject *)&DeeBuiltinFunction___builtin_memmem,
 /*0x00FE*/(DeeObject *)&DeeBuiltinFunction___builtin_strncasecmp,
 /*0x00FF*/(DeeObject *)&DeeBuiltinFunction___builtin_stpcpy,
 /*0x0100*/(DeeObject *)&DeeBuiltinFunction___builtin_stpncpy,
#else
 /*0x00F5*/(DeeObject *)Dee_None,
 /*0x00F6*/(DeeObject *)Dee_None,
 /*0x00F7*/(DeeObject *)Dee_None,
 /*0x00F8*/(DeeObject *)Dee_None,
 /*0x00F9*/(DeeObject *)Dee_None,
 /*0x00FA*/(DeeObject *)Dee_None,
 /*0x00FB*/(DeeObject *)Dee_None,
 /*0x00FC*/(DeeObject *)Dee_None,
 /*0x00FD*/(DeeObject *)Dee_None,
 /*0x00FE*/(DeeObject *)Dee_None,
 /*0x00FF*/(DeeObject *)Dee_None,
 /*0x0100*/(DeeObject *)Dee_None,
#endif
 /*0x0101*/(DeeObject *)&DeeRandom_Type,
 /*0x0102*/(DeeObject *)&DeeBuiltinFunction___builtin_min,
 /*0x0103*/(DeeObject *)&DeeBuiltinFunction___builtin_max,
 /*0x0104*/(DeeObject *)&DeeBuiltinFunction___builtin_avg,
 /*0x0105*/(DeeObject *)&DeeBuiltinFunction___builtin_all,
 /*0x0106*/(DeeObject *)&DeeBuiltinFunction___builtin_any,
 /*0x0107*/(DeeObject *)&DeeBuiltinFunction___builtin_sum,
 /*0x0108*/(DeeObject *)&DeeBuiltinFunction___builtin_sqr, // Used-to-be: DeeBuiltinFunction___builtin_chr,
 /*0x0109*/(DeeObject *)&DeeBuiltinFunction___builtin_ord,
 /*0x010A*/(DeeObject *)&DeeBuiltinFunction___builtin_abs,
 /*0x010B*/(DeeObject *)&DeeBuiltinFunction___builtin_ceil,
 /*0x010C*/(DeeObject *)&DeeBuiltinFunction___builtin_floor,
 /*0x010D*/(DeeObject *)&DeeBuiltinFunction___builtin_round,
 /*0x010E*/(DeeObject *)&DeeRangeInt8Iterator_Type,
 /*0x010F*/(DeeObject *)&DeeRangeInt16Iterator_Type,
 /*0x0110*/(DeeObject *)&DeeRangeInt32Iterator_Type,
 /*0x0111*/(DeeObject *)&DeeRangeInt64Iterator_Type,
 /*0x0112*/(DeeObject *)&DeeRangeUInt8Iterator_Type,
 /*0x0113*/(DeeObject *)&DeeRangeUInt16Iterator_Type,
 /*0x0114*/(DeeObject *)&DeeRangeUInt32Iterator_Type,
 /*0x0115*/(DeeObject *)&DeeRangeUInt64Iterator_Type,
 /*0x0116*/(DeeObject *)&DeeGenericRangeIterator_Type,
 /*0x0117*/(DeeObject *)&DeeBuiltinFunction___builtin_range,
 /*0x0118*/(DeeObject *)&DeeBuiltinFunction___builtin_id,
 /*0x0119*/(DeeObject *)&DeeBuiltinFunction___builtin_sin,
 /*0x011A*/(DeeObject *)&DeeBuiltinFunction___builtin_cos,
 /*0x011B*/(DeeObject *)&DeeBuiltinFunction___builtin_tan,
 /*0x011C*/(DeeObject *)&DeeBuiltinFunction___builtin_sincos,
 /*0x011D*/(DeeObject *)&DeeBuiltinFunction___builtin_asin,
 /*0x011E*/(DeeObject *)&DeeBuiltinFunction___builtin_acos,
 /*0x011F*/(DeeObject *)&DeeBuiltinFunction___builtin_atan,
 /*0x0120*/(DeeObject *)&DeeBuiltinFunction___builtin_atan2,
 /*0x0121*/(DeeObject *)&DeeBuiltinFunction___builtin_asincos,
 /*0x0122*/(DeeObject *)&DeeBuiltinFunction___builtin_sinh,
 /*0x0123*/(DeeObject *)&DeeBuiltinFunction___builtin_cosh,
 /*0x0124*/(DeeObject *)&DeeBuiltinFunction___builtin_tanh,
 /*0x0125*/(DeeObject *)&DeeBuiltinFunction___builtin_sincosh,
 /*0x0126*/(DeeObject *)&DeeBuiltinFunction___builtin_asinh,
 /*0x0127*/(DeeObject *)&DeeBuiltinFunction___builtin_acosh,
 /*0x0128*/(DeeObject *)&DeeBuiltinFunction___builtin_atanh,
 /*0x0129*/(DeeObject *)&DeeBuiltinFunction___builtin_asincosh,
 /*0x012A*/(DeeObject *)&DeeBuiltinFunction___builtin_exp,
 /*0x012B*/(DeeObject *)&DeeBuiltinFunction___builtin_frexp,
 /*0x012C*/(DeeObject *)&DeeBuiltinFunction___builtin_log,
 /*0x012D*/(DeeObject *)&DeeBuiltinFunction___builtin_log10,
 /*0x012E*/(DeeObject *)&DeeBuiltinFunction___builtin_modf,
 /*0x012F*/(DeeObject *)&DeeBuiltinFunction___builtin_exp2,
 /*0x0130*/(DeeObject *)&DeeBuiltinFunction___builtin_expm1,
 /*0x0131*/(DeeObject *)&DeeBuiltinFunction___builtin_ilogb,
 /*0x0132*/(DeeObject *)&DeeBuiltinFunction___builtin_log1p,
 /*0x0133*/(DeeObject *)&DeeBuiltinFunction___builtin_log2,
 /*0x0134*/(DeeObject *)&DeeBuiltinFunction___builtin_logb,
 /*0x0135*/(DeeObject *)&DeeBuiltinFunction___builtin_scalbn,
 /*0x0136*/(DeeObject *)&DeeBuiltinFunction___builtin_scalbi32n,
 /*0x0137*/(DeeObject *)&DeeBuiltinFunction___builtin_sqrt,
 /*0x0138*/(DeeObject *)&DeeBuiltinFunction___builtin_cbrt,
 /*0x0139*/(DeeObject *)&DeeBuiltinFunction___builtin_hypot,
 /*0x013A*/(DeeObject *)&DeeBuiltinFunction___builtin_erf,
 /*0x013B*/(DeeObject *)&DeeBuiltinFunction___builtin_tgamma,
 /*0x013C*/(DeeObject *)&DeeBuiltinFunction___builtin_lgamma,
 /*0x013D*/(DeeObject *)&DeeBuiltinFunction___builtin_ceilf,
 /*0x013E*/(DeeObject *)&DeeBuiltinFunction___builtin_floorf,
 /*0x013F*/(DeeObject *)&DeeBuiltinFunction___builtin_truncf,
 /*0x0140*/(DeeObject *)&DeeBuiltinFunction___builtin_roundf,
 /*0x0141*/(DeeObject *)&DeeBuiltinFunction___builtin_i32round,
 /*0x0142*/(DeeObject *)&DeeBuiltinFunction___builtin_i64round,
 /*0x0143*/(DeeObject *)&DeeBuiltinFunction___builtin_fmod,
 /*0x0144*/(DeeObject *)&DeeBuiltinFunction___builtin_rint,
 /*0x0145*/(DeeObject *)&DeeBuiltinFunction___builtin_i32rint,
 /*0x0146*/(DeeObject *)&DeeBuiltinFunction___builtin_i64rint,
 /*0x0147*/(DeeObject *)&DeeBuiltinFunction___builtin_nearbyint,
 /*0x0148*/(DeeObject *)&DeeBuiltinFunction___builtin_remainder,
 /*0x0149*/(DeeObject *)&DeeBuiltinFunction___builtin_remquo,
 /*0x014A*/(DeeObject *)&DeeBuiltinFunction___builtin_copysign,
 /*0x014B*/(DeeObject *)&DeeBuiltinFunction___builtin_nextafter,
 /*0x014C*/(DeeObject *)&DeeBuiltinFunction___builtin_nexttoward,
 /*0x014D*/(DeeObject *)&DeeBuiltinFunction___builtin_fdim,
 /*0x014E*/(DeeObject *)&DeeBuiltinFunction___builtin_isfinite,
 /*0x014F*/(DeeObject *)&DeeBuiltinFunction___builtin_isinf,
 /*0x0150*/(DeeObject *)&DeeBuiltinFunction___builtin_isnan,
 /*0x0151*/(DeeObject *)&DeeBuiltinFunction___builtin_isnormal,
 /*0x0152*/(DeeObject *)&DeeBuiltinFunction___builtin_signbit,
 /*0x0153*/(DeeObject *)&DeeBuiltinFunction___builtin_isgreater,
 /*0x0154*/(DeeObject *)&DeeBuiltinFunction___builtin_isgreaterequal,
 /*0x0155*/(DeeObject *)&DeeBuiltinFunction___builtin_isless,
 /*0x0156*/(DeeObject *)&DeeBuiltinFunction___builtin_islessequal,
 /*0x0157*/(DeeObject *)&DeeBuiltinFunction___builtin_islessgreater,
 /*0x0158*/(DeeObject *)&DeeBuiltinFunction___builtin_isunordered,
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 /*0x0159*/(DeeObject *)&DeeWideString_Type, // Used-to-be: DeeBuiltinFunction___builtin_stackframe_get_next_local
#else
 /*0x0159*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 /*0x015A*/(DeeObject *)&DeeUtf16String_Type, // Used-to-be: DeeBuiltinFunction___builtin_stackframe_get_prev_local
#else
 /*0x015A*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 /*0x015B*/(DeeObject *)&DeeUtf32String_Type, // Used-to-be: DeeBuiltinFunction___builtin_stackframe_get_next_arg
#else
 /*0x015B*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
 /*0x015C*/(DeeObject *)&DeeSockAddr_Type, // Used-to-be: DeeBuiltinFunction___builtin_stackframe_get_prev_arg
#else /* DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
 /*0x015C*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
 /*0x015D*/(DeeObject *)&DeeZipIterator_Type, // Used-to-be: DeeBuiltinFunction___builtin_zip
 /*0x015E*/(DeeObject *)&DeeBuiltinFunction___builtin_FILE,
 /*0x015F*/(DeeObject *)&DeeBuiltinFunction___builtin_LINE,
 /*0x0160*/(DeeObject *)&DeeBuiltinFunction___builtin_FUNCTION,
 /*0x0161*/(DeeObject *)&DeeBuiltinFunction___builtin_argv,
#if defined(DEE_PLATFORM_WINDOWS)
 /*0x0162*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32dlldirectory,
 /*0x0163*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemdirectory,
#else
 /*0x0162*/(DeeObject *)Dee_None,
 /*0x0163*/(DeeObject *)Dee_None,
#endif
 /*0x0164*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gettmp,
#ifdef DEE_PLATFORM_WINDOWS
 /*0x0165*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32windowsdirectory,
 /*0x0166*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemwindowsdirectory,
 /*0x0167*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32syswow64directory,
#else
 /*0x0165*/(DeeObject *)Dee_None,
 /*0x0166*/(DeeObject *)Dee_None,
 /*0x0167*/(DeeObject *)Dee_None,
#endif
 /*0x0168*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gettmpname,
 /*0x0169*/(DeeObject *)&DeeBuiltinFunction___builtin_code,
 /*0x016A*/(DeeObject *)&DeeErrorType_UnboundLocal,
 /*0x016B*/(DeeObject *)&DeeErrorType_DivideByZero,
 /*0x016C*/(DeeObject *)&DeeGenericSequenceIterator_Type, // Used-to-be: 'DeeErrorType_RangeError'
 /*0x016D*/(DeeObject *)&DeeErrorType_FileNotFound,
 /*0x016E*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_ischrdev,
 /*0x016F*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isblkdev,
 /*0x0170*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_isfifo,
 /*0x0171*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_issocket,
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 /*0x0172*/(DeeObject *)&DeeInstanceMethod_Type,
 /*0x0173*/(DeeObject *)&DeeBuiltinFunction___builtin_class, // Used-to-be: 'DeeClass_Type'
#else
 /*0x0172*/(DeeObject *)Dee_None,
 /*0x0173*/(DeeObject *)Dee_None,
#endif
 /*0x0174*/(DeeObject *)&DeeErrorType_ThreadCrash, // Used-to-be: 'DeeModule_Type'
 /*0x0175*/(DeeObject *)&DeeBuiltinFunction___builtin_swap,
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 /*0x0176*/(DeeObject *)&DeeUtf8StringIterator_Type, // Used-to-be: DeeStringIterator_Type
#else
 /*0x0176*/(DeeObject *)Dee_None,
#endif
 /*0x0177*/(DeeObject *)&DeeBuiltinFunction___builtin_bswap16,
 /*0x0178*/(DeeObject *)&DeeBuiltinFunction___builtin_bswap32,
 /*0x0179*/(DeeObject *)&DeeBuiltinFunction___builtin_bswap64,
 /*0x017A*/(DeeObject *)&DeeStructType_Type,
 /*0x017B*/(DeeObject *)&DeeScope_Type,
 /*0x017C*/(DeeObject *)&DeeLexerIterator_Type,
 /*0x017D*/(DeeObject *)&DeeErrorType_IndexError,
 /*0x017E*/(DeeObject *)&DeeSet_Type,
 /*0x017F*/(DeeObject *)&DeeSetIterator_Type,
 /*0x0180*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_setatime,
 /*0x0181*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_setctime,
 /*0x0182*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_setmtime,
 /*0x0183*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_settimes,
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 /*0x0184*/(DeeObject *)&DeeArrayType_Type,
 /*0x0185*/(DeeObject *)&DeeArrayIterator_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 /*0x0184*/(DeeObject *)Dee_None,
 /*0x0185*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 /*0x0186*/(DeeObject *)&DeeFileJoined_Type,
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
 /*0x0187*/(DeeObject *)&DeeProperty_Type,
#else
 /*0x0187*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
 /*0x0188*/(DeeObject *)&DeeProcess_Type,
#else
 /*0x0188*/(DeeObject *)Dee_None,
#endif
 /*0x0189*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_contains, // Used-to-be: DeeProcessRef_Type
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
 /*0x018A*/(DeeObject *)&DeeSocket_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
 /*0x018A*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
 /*0x018B*/(DeeObject *)&DeeBuiltinFunction___hasattr__, // Used-to-be: &DeeTCPSocket_Type,
 /*0x018C*/(DeeObject *)&DeeSubRangeIterator_Type, // Used-to-be: DeeUDPSocket_Type
#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
 /*0x018D*/(DeeObject *)&DeeSocketFile_Type,
 /*0x018E*/(DeeObject *)&DeeSocketFileTo_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
 /*0x018D*/(DeeObject *)Dee_None,
 /*0x018E*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 /*0x018F*/(DeeObject *)&DeeWideStringIterator_Type,
 /*0x0190*/(DeeObject *)&_Dee_EmptyWideString,
#else
 /*0x018F*/(DeeObject *)Dee_None,
 /*0x0190*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 /*0x0191*/(DeeObject *)&DeeUtf16StringIterator_Type,
 /*0x0192*/(DeeObject *)&_Dee_EmptyUtf16String,
#else
 /*0x0191*/(DeeObject *)Dee_None,
 /*0x0192*/(DeeObject *)Dee_None,
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 /*0x0193*/(DeeObject *)&DeeUtf32StringIterator_Type,
 /*0x0194*/(DeeObject *)&_Dee_EmptyUtf32String,
#else
 /*0x0193*/(DeeObject *)Dee_None,
 /*0x0194*/(DeeObject *)Dee_None,
#endif
 /*0x0195*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_user,
 /*0x0196*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_machine,
 /*0x0197*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wuser,
 /*0x0198*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wmachine,
 /*0x0199*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wgetcwd,
 /*0x019A*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wgettmp,
 /*0x019B*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wgethome,
 /*0x019C*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wlistenv,
#ifdef DEE_PLATFORM_WINDOWS
 /*0x019D*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wmodulename,
 /*0x019E*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wdlldirectory,
 /*0x019F*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wsystemdirectory,
 /*0x01A0*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wwindowsdirectory,
 /*0x01A1*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wsystemwindowsdirectory,
 /*0x01A2*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wsyswow64directory,
#else
 /*0x019D*/(DeeObject *)Dee_None,
 /*0x019E*/(DeeObject *)Dee_None,
 /*0x019F*/(DeeObject *)Dee_None,
 /*0x01A0*/(DeeObject *)Dee_None,
 /*0x01A1*/(DeeObject *)Dee_None,
 /*0x01A2*/(DeeObject *)Dee_None,
#endif
 /*0x01A3*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wgettmpname,
 /*0x01A4*/(DeeObject *)&DeeListIterator_Type,
 /*0x01A5*/(DeeObject *)&DeeSuper_Type,
 /*0x01A6*/(DeeObject *)&DeeErrorType_UnicodeError,
 /*0x01A7*/(DeeObject *)&DeeErrorType_ReferenceError,
 /*0x01A8*/(DeeObject *)&DeeErrorType_NetError,
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 /*0x01A9*/(DeeObject *)&DeeBuiltinFunction___builtin_varray_add, // Used-to-bo: DeeClassDecl_Type
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 /*0x01AA*/(DeeObject *)&_DeeErrorInstance_IterDone,
 /*0x01AB*/(DeeObject *)&_DeeErrorInstance_OutOfMemory,
 /*0x01AC*/(DeeObject *)&_DeeErrorInstance_NotImplemented,
 /*0x01AD*/(DeeObject *)&_DeeErrorInstance_DivByZero,
 /*0x01AE*/(DeeObject *)&_DeeErrorInstance_StackOverflow,
 /*0x01AF*/(DeeObject *)&DeeErrorType_StackOverflow,
 /*0x01B0*/(DeeObject *)&DeeBuiltinFunction___builtin_get_stacklimit,
 /*0x01B1*/(DeeObject *)&DeeBuiltinFunction___builtin_set_stacklimit,
 /*0x01B2*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_find,
 /*0x01B3*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_rfind,
 /*0x01B4*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_index,
 /*0x01B5*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_rindex,
 /*0x01B6*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_count,
 /*0x01B7*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_locate,
 /*0x01B8*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_rlocate,
 /*0x01B9*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_locate_all,
 /*0x01BA*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_transform,
 /*0x01BB*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_empty,
 /*0x01BC*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_non_empty,
 /*0x01BD*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_front,
 /*0x01BE*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_back,
 /*0x01BF*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_get,
 /*0x01C0*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_at,
 /*0x01C1*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_reversed,
 /*0x01C2*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_sorted,
 /*0x01C3*/(DeeObject *)&DeeBuiltinFunction___builtin_seq_segments,
 /*0x01C4*/(DeeObject *)&DeeCell_Type,
 /*0x01C5*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_chmod,
 /*0x01C6*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getmod,
 /*0x01C7*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_chown,
 /*0x01C8*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_getown,
 /*0x01C9*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_uidof,
 /*0x01CA*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gidof,
 /*0x01CB*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_uidname,
 /*0x01CC*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wuidname,
 /*0x01CD*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_gidname,
 /*0x01CE*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_wgidname,
#ifdef DEE_PLATFORM_WINDOWS
 /*0x01CF*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32drives,
 /*0x01D0*/(DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wdrives,
#else
 /*0x01CF*/(DeeObject *)Dee_None,
 /*0x01D0*/(DeeObject *)Dee_None,
#endif
 /*0x01D1*/(DeeObject *)Dee_None, // [UNUSED] Used-to-be: &DeeBuiltinFunction___builtin_crypt_base64encode,
 /*0x01D2*/(DeeObject *)Dee_None, // [UNUSED] Used-to-be: &DeeBuiltinFunction___builtin_crypt_base64decode,
 /*0x01D3*/(DeeObject *)&_Dee_EmptyZipIterator,
 /*0x01D4*/(DeeObject *)&DeeBuiltinFunction___moveassign__,
 /*0x01D5*/(DeeObject *)&DeeBuiltinFunction___callv__,
 /*0x01D6*/(DeeObject *)&DeeBuiltinFunction___hash__,
 /*0x01D7*/(DeeObject *)&DeeBuiltinFunction___dual_posoradd__,
 /*0x01D8*/(DeeObject *)&DeeBuiltinFunction___dual_negorsub__,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x01D9*/(DeeObject *)&DeeBuiltinFunction___dual_dereformul__,
 /*0x01DA*/(DeeObject *)&DeeBuiltinFunction___dual_reforand__,
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x01D9*/(DeeObject *)Dee_None,
 /*0x01DA*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x01DB*/(DeeObject *)&DeeBuiltinFunction___dual_getitemorgetrange__,
 /*0x01DC*/(DeeObject *)&DeeBuiltinFunction___dual_delitemordelrange__,
 /*0x01DD*/(DeeObject *)&DeeBuiltinFunction___dual_setitemorsetrange__,
 /*0x01DE*/(DeeObject *)&DeeBuiltinFunction___builtin_dex,
 /*0x01DF*/(DeeObject *)&DeeBuiltinFunction___bool__,
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
 /*0x01E0*/(DeeObject *)&DeeBuiltinFunction___builtin_extern,
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 /*0x01E0*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
 /*0x01E1*/(DeeObject *)&DeeBuiltinFunction___builtin_ff_closure,
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
 /*0x01E1*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
 /*0x01E2*/(DeeObject *)&DeeBuiltinFunction___builtin_enumattr,
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 /*0x01E3*/(DeeObject *)&DeeVarArrayType_Type,
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 /*0x01E3*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 // Builtin file operators
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*0x01E3*/(DeeObject *)&DeeBuiltinFunction___read__,
 /*0x01E4*/(DeeObject *)&DeeBuiltinFunction___write__,
 /*0x01E5*/(DeeObject *)&DeeBuiltinFunction___readp__,
 /*0x01E6*/(DeeObject *)&DeeBuiltinFunction___writep__,
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 // By still linking against the np-versions,
 // __read__ and __write__ can still kind-of work
 /*0x01E3*/(DeeObject *)&DeeBuiltinFunction___readnp__,
 /*0x01E4*/(DeeObject *)&DeeBuiltinFunction___writenp__,
 /*0x01E5*/(DeeObject *)Dee_None,
 /*0x01E6*/(DeeObject *)Dee_None,
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 /*0x01E7*/(DeeObject *)&DeeBuiltinFunction___readnp__,
 /*0x01E8*/(DeeObject *)&DeeBuiltinFunction___writenp__,
 /*0x01E9*/(DeeObject *)&DeeBuiltinFunction___seek__,
 /*0x01EA*/(DeeObject *)&DeeBuiltinFunction___flush__,
 /*0x01EB*/(DeeObject *)&DeeBuiltinFunction___trunc__,
 /*0x01EC*/(DeeObject *)&DeeBuiltinFunction___close__,
#ifdef DEE_PLATFORM_WINDOWS
 /*0x01ED*/(DeeObject *)&DeeFSWin32SID_Type,
#else
 /*0x01ED*/(DeeObject *)Dee_None,
#endif
 /*0x01EF*/(DeeObject *)&DeeFunction_Type,
 /*0x01F0*/(DeeObject *)&DeeEnumIterator_Type,
 /*0x01F1*/(DeeObject *)&DeeBuiltinFunction___builtin_enumerate,

// New builtin constants must be added here...
};
//enum{n_builtins=sizeof(_DeeBuiltin_ID2Type)/sizeof(*_DeeBuiltin_ID2Type)};

 

DEE_A_RET_WUNUSED Dee_uint16_t DeeBuiltin_Id(DEE_A_IN DeeObject const *ob) {
 DeeObject *const *iter,*const *end;
 end = (iter = _DeeBuiltin_ID2Type)+(sizeof(_DeeBuiltin_ID2Type)/sizeof(*_DeeBuiltin_ID2Type));
 do if (*iter == ob) return (Dee_uint16_t)(iter-_DeeBuiltin_ID2Type); while (++iter != end);
 return (Dee_uint16_t)-1;
}
DEE_A_RET_WUNUSED Dee_uint16_t _DeeBuiltin_Count(void) {
 return (Dee_uint16_t)(sizeof(_DeeBuiltin_ID2Type)/sizeof(*_DeeBuiltin_ID2Type));
}
DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeObject *DeeBuiltin_Get(DEE_A_IN Dee_uint16_t id) {
 return id < (sizeof(_DeeBuiltin_ID2Type)/sizeof(*_DeeBuiltin_ID2Type))
  ? _DeeBuiltin_ID2Type[id] : NULL;
}





DeeObject *const _DeeBuiltin_ConstantFunctions[] = {
 // Whitelist of builtin functions that are considered constexpr (order doesn't matter)
 (DeeObject *)&DeeBuiltinFunction___builtin_object,
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
 (DeeObject *)&DeeBuiltinFunction___builtin_ff_closure,
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
 (DeeObject *)&DeeBuiltinFunction___repr__,
 (DeeObject *)&DeeBuiltinFunction___type__,
 (DeeObject *)&DeeBuiltinFunction___copy__,
 (DeeObject *)&DeeBuiltinFunction___move__,
 (DeeObject *)&DeeBuiltinFunction___copyassign__,
 (DeeObject *)&DeeBuiltinFunction___moveassign__,
 (DeeObject *)&DeeBuiltinFunction___not__,
 (DeeObject *)&DeeBuiltinFunction___neg__,
 (DeeObject *)&DeeBuiltinFunction___pos__,
 (DeeObject *)&DeeBuiltinFunction___inv__,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 (DeeObject *)&DeeBuiltinFunction___ref__,
 (DeeObject *)&DeeBuiltinFunction___deref__,
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 (DeeObject *)&DeeBuiltinFunction___inc__,
 (DeeObject *)&DeeBuiltinFunction___dec__,
 (DeeObject *)&DeeBuiltinFunction___incpost__,
 (DeeObject *)&DeeBuiltinFunction___decpost__,
 (DeeObject *)&DeeBuiltinFunction___lo__,
 (DeeObject *)&DeeBuiltinFunction___le__,
 (DeeObject *)&DeeBuiltinFunction___eq__,
 (DeeObject *)&DeeBuiltinFunction___ne__,
 (DeeObject *)&DeeBuiltinFunction___gr__,
 (DeeObject *)&DeeBuiltinFunction___ge__,
 (DeeObject *)&DeeBuiltinFunction___add__,
 (DeeObject *)&DeeBuiltinFunction___iadd__,
 (DeeObject *)&DeeBuiltinFunction___sub__,
 (DeeObject *)&DeeBuiltinFunction___isub__,
 (DeeObject *)&DeeBuiltinFunction___mul__,
 (DeeObject *)&DeeBuiltinFunction___imul__,
 (DeeObject *)&DeeBuiltinFunction___div__,
 (DeeObject *)&DeeBuiltinFunction___idiv__,
 (DeeObject *)&DeeBuiltinFunction___mod__,
 (DeeObject *)&DeeBuiltinFunction___imod__,
 (DeeObject *)&DeeBuiltinFunction___shl__,
 (DeeObject *)&DeeBuiltinFunction___ishl__,
 (DeeObject *)&DeeBuiltinFunction___shr__,
 (DeeObject *)&DeeBuiltinFunction___ishr__,
 (DeeObject *)&DeeBuiltinFunction___and__,
 (DeeObject *)&DeeBuiltinFunction___iand__,
 (DeeObject *)&DeeBuiltinFunction___or__,
 (DeeObject *)&DeeBuiltinFunction___ior__,
 (DeeObject *)&DeeBuiltinFunction___xor__,
 (DeeObject *)&DeeBuiltinFunction___ixor__,
 (DeeObject *)&DeeBuiltinFunction___pow__,
 (DeeObject *)&DeeBuiltinFunction___ipow__,
 (DeeObject *)&DeeBuiltinFunction___call__,
 (DeeObject *)&DeeBuiltinFunction___size__,
 (DeeObject *)&DeeBuiltinFunction___getitem__,
 (DeeObject *)&DeeBuiltinFunction___setitem__,
 (DeeObject *)&DeeBuiltinFunction___delitem__,
 (DeeObject *)&DeeBuiltinFunction___getrange__,
 (DeeObject *)&DeeBuiltinFunction___setrange__,
 (DeeObject *)&DeeBuiltinFunction___delrange__,
 (DeeObject *)&DeeBuiltinFunction___is__,
 (DeeObject *)&DeeBuiltinFunction___contains__,
 (DeeObject *)&DeeBuiltinFunction___getattr__,
 (DeeObject *)&DeeBuiltinFunction___delattr__,
 (DeeObject *)&DeeBuiltinFunction___setattr__,
 (DeeObject *)&DeeBuiltinFunction___iterself__,
 (DeeObject *)&DeeBuiltinFunction___iternext__,
 (DeeObject *)&DeeBuiltinFunction___callv__,
 (DeeObject *)&DeeBuiltinFunction___hash__,
 (DeeObject *)&DeeBuiltinFunction___bool__,
 // File Operators
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 (DeeObject *)&DeeBuiltinFunction___read__,
 (DeeObject *)&DeeBuiltinFunction___write__,
 (DeeObject *)&DeeBuiltinFunction___readp__,
 (DeeObject *)&DeeBuiltinFunction___writep__,
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 (DeeObject *)&DeeBuiltinFunction___readnp__,
 (DeeObject *)&DeeBuiltinFunction___writenp__,
 (DeeObject *)&DeeBuiltinFunction___seek__,
 (DeeObject *)&DeeBuiltinFunction___flush__,
 (DeeObject *)&DeeBuiltinFunction___trunc__,
 (DeeObject *)&DeeBuiltinFunction___close__,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 (DeeObject *)&DeeBuiltinFunction___builtin_pointer_add,
 (DeeObject *)&DeeBuiltinFunction___builtin_pointer_rem,
 (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add,
 (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_rem,
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 (DeeObject *)&DeeBuiltinFunction___builtin_varray_add,
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 (DeeObject *)&DeeBuiltinFunction___dual_posoradd__,
 (DeeObject *)&DeeBuiltinFunction___dual_negorsub__,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 (DeeObject *)&DeeBuiltinFunction___dual_dereformul__,
 (DeeObject *)&DeeBuiltinFunction___dual_reforand__,
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 (DeeObject *)&DeeBuiltinFunction___dual_getitemorgetrange__,
 (DeeObject *)&DeeBuiltinFunction___dual_delitemordelrange__,
 (DeeObject *)&DeeBuiltinFunction___dual_setitemorsetrange__,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_head,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_tail,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_file,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_ext,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_inctrail,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_exctrail,
#if DEE_XCONFIG_HAVE_CONSTEXPR_PLATFORM_FUNCTIONS
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_drive,
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_join,
#endif
#if DEE_XCONFIG_HAVE_CONSTEXPR_PLATFORM_FUNCTIONS
#ifdef DEE_PLATFORM_UNIX
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_ishidden, // fs.path.tail(x).startswith(".")
#endif
#ifdef DEE_PLATFORM_WINDOWS
 (DeeObject *)&DeeBuiltinFunction___builtin_fs_isxfile, // fs.path.tail(x).endswith(".exe")
#endif
#endif
 (DeeObject *)&DeeBuiltinFunction___builtin_min,
 (DeeObject *)&DeeBuiltinFunction___builtin_max,
 (DeeObject *)&DeeBuiltinFunction___builtin_avg,
 (DeeObject *)&DeeBuiltinFunction___builtin_all,
 (DeeObject *)&DeeBuiltinFunction___builtin_any,
 (DeeObject *)&DeeBuiltinFunction___builtin_sum,
 (DeeObject *)&DeeBuiltinFunction___builtin_sqr,
 (DeeObject *)&DeeBuiltinFunction___builtin_ord,
 (DeeObject *)&DeeBuiltinFunction___builtin_abs,
 (DeeObject *)&DeeBuiltinFunction___builtin_ceil,
 (DeeObject *)&DeeBuiltinFunction___builtin_floor,
 (DeeObject *)&DeeBuiltinFunction___builtin_round,
 (DeeObject *)&DeeBuiltinFunction___builtin_range,
 (DeeObject *)&DeeBuiltinFunction___builtin_sin,
 (DeeObject *)&DeeBuiltinFunction___builtin_cos,
 (DeeObject *)&DeeBuiltinFunction___builtin_tan,
 (DeeObject *)&DeeBuiltinFunction___builtin_sincos,
 (DeeObject *)&DeeBuiltinFunction___builtin_asin,
 (DeeObject *)&DeeBuiltinFunction___builtin_acos,
 (DeeObject *)&DeeBuiltinFunction___builtin_atan,
 (DeeObject *)&DeeBuiltinFunction___builtin_atan2,
 (DeeObject *)&DeeBuiltinFunction___builtin_asincos,
 (DeeObject *)&DeeBuiltinFunction___builtin_sinh,
 (DeeObject *)&DeeBuiltinFunction___builtin_cosh,
 (DeeObject *)&DeeBuiltinFunction___builtin_tanh,
 (DeeObject *)&DeeBuiltinFunction___builtin_sincosh,
 (DeeObject *)&DeeBuiltinFunction___builtin_asinh,
 (DeeObject *)&DeeBuiltinFunction___builtin_acosh,
 (DeeObject *)&DeeBuiltinFunction___builtin_atanh,
 (DeeObject *)&DeeBuiltinFunction___builtin_asincosh,
 (DeeObject *)&DeeBuiltinFunction___builtin_exp,
 (DeeObject *)&DeeBuiltinFunction___builtin_frexp,
 (DeeObject *)&DeeBuiltinFunction___builtin_log,
 (DeeObject *)&DeeBuiltinFunction___builtin_log10,
 (DeeObject *)&DeeBuiltinFunction___builtin_modf,
 (DeeObject *)&DeeBuiltinFunction___builtin_exp2,
 (DeeObject *)&DeeBuiltinFunction___builtin_expm1,
 (DeeObject *)&DeeBuiltinFunction___builtin_ilogb,
 (DeeObject *)&DeeBuiltinFunction___builtin_log1p,
 (DeeObject *)&DeeBuiltinFunction___builtin_log2,
 (DeeObject *)&DeeBuiltinFunction___builtin_logb,
 (DeeObject *)&DeeBuiltinFunction___builtin_scalbn,
 (DeeObject *)&DeeBuiltinFunction___builtin_scalbi32n,
 (DeeObject *)&DeeBuiltinFunction___builtin_sqrt,
 (DeeObject *)&DeeBuiltinFunction___builtin_cbrt,
 (DeeObject *)&DeeBuiltinFunction___builtin_hypot,
 (DeeObject *)&DeeBuiltinFunction___builtin_erf,
 (DeeObject *)&DeeBuiltinFunction___builtin_tgamma,
 (DeeObject *)&DeeBuiltinFunction___builtin_lgamma,
 (DeeObject *)&DeeBuiltinFunction___builtin_ceilf,
 (DeeObject *)&DeeBuiltinFunction___builtin_floorf,
 (DeeObject *)&DeeBuiltinFunction___builtin_truncf,
 (DeeObject *)&DeeBuiltinFunction___builtin_roundf,
 (DeeObject *)&DeeBuiltinFunction___builtin_i32round,
 (DeeObject *)&DeeBuiltinFunction___builtin_i64round,
 (DeeObject *)&DeeBuiltinFunction___builtin_fmod,
 (DeeObject *)&DeeBuiltinFunction___builtin_rint,
 (DeeObject *)&DeeBuiltinFunction___builtin_i32rint,
 (DeeObject *)&DeeBuiltinFunction___builtin_i64rint,
 (DeeObject *)&DeeBuiltinFunction___builtin_nearbyint,
 (DeeObject *)&DeeBuiltinFunction___builtin_remainder,
 (DeeObject *)&DeeBuiltinFunction___builtin_remquo,
 (DeeObject *)&DeeBuiltinFunction___builtin_copysign,
 (DeeObject *)&DeeBuiltinFunction___builtin_nextafter,
 (DeeObject *)&DeeBuiltinFunction___builtin_nexttoward,
 (DeeObject *)&DeeBuiltinFunction___builtin_fdim,
 (DeeObject *)&DeeBuiltinFunction___builtin_isfinite,
 (DeeObject *)&DeeBuiltinFunction___builtin_isinf,
 (DeeObject *)&DeeBuiltinFunction___builtin_isnan,
 (DeeObject *)&DeeBuiltinFunction___builtin_isnormal,
 (DeeObject *)&DeeBuiltinFunction___builtin_signbit,
 (DeeObject *)&DeeBuiltinFunction___builtin_isgreater,
 (DeeObject *)&DeeBuiltinFunction___builtin_isgreaterequal,
 (DeeObject *)&DeeBuiltinFunction___builtin_isless,
 (DeeObject *)&DeeBuiltinFunction___builtin_islessequal,
 (DeeObject *)&DeeBuiltinFunction___builtin_islessgreater,
 (DeeObject *)&DeeBuiltinFunction___builtin_isunordered,
 //(DeeObject *)&DeeBuiltinFunction___builtin_swap,
 (DeeObject *)&DeeBuiltinFunction___builtin_bswap16,
 (DeeObject *)&DeeBuiltinFunction___builtin_bswap32,
 (DeeObject *)&DeeBuiltinFunction___builtin_bswap64,
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
 (DeeObject *)&DeeBuiltinFunction___builtin_class,
#endif
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_find,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_rfind,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_index,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_rindex,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_contains,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_count,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_locate,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_rlocate,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_locate_all,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_transform,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_empty,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_non_empty,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_front,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_back,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_get,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_at,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_reversed,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_sorted,
 (DeeObject *)&DeeBuiltinFunction___builtin_seq_segments,
};
DEE_A_RET_WUNUSED int _DeeBuiltin_AllowConstantCall(DEE_A_IN DeeObject const *ob) {
 DeeObject *const*iter,*const*end;
 end = (iter = _DeeBuiltin_ConstantFunctions)+(
  sizeof(_DeeBuiltin_ConstantFunctions)/sizeof(*_DeeBuiltin_ConstantFunctions));
 while (iter != end) if (*iter++ == ob) return 1;
 return 0;
}

DEE_DECL_END


#endif /* !GUARD_DEEMON___BUILTINS_INL */
