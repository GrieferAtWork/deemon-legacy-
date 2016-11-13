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
#ifndef GUARD_DEEMON_BUILTIN_FUNCTIONS_H
#define GUARD_DEEMON_BUILTIN_FUNCTIONS_H 1
#ifdef __DEEMON__
#pragma warning(disable: 16)
#define DEE_LIMITED_DEX
#endif

#include <deemon/__conf.inl>
#include <deemon/cfunction.h>

#ifdef DEE_LIMITED_API
DEE_DECL_BEGIN

#define DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
extern DeeCFunctionObject DeeBuiltinFunction_##name;\
extern DEE_A_RET_EXCEPT_REF DeeObject *DEE_CALL \
 DeeBuiltinFunction_##name##_F(DEE_A_INOUT_OBJECT(DeeTupleObject) *args)

//////////////////////////////////////////////////////////////////////////
// Builtin functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__bool__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__repr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__type__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__copy__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__move__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__copyassign__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__moveassign__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__not__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__neg__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__pos__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__inv__);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ref__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__deref__);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__inc__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dec__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__incpost__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__decpost__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__lo__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__le__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__eq__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ne__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__gr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ge__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__add__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iadd__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__sub__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__isub__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__mul__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__imul__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__div__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__idiv__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__mod__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__imod__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__shl__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ishl__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__shr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ishr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__and__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iand__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__or__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ior__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__xor__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ixor__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__pow__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ipow__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__call__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__callv__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__size__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getitem__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setitem__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delitem__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getrange__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setrange__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delrange__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__is__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__contains__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getattr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__hasattr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delattr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setattr__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iterself__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iternext__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__hash__);

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__read__); // __readp__ or __readnp__
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__write__); // __writep__ or __writenp__
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__readp__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__writep__);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#define DeeBuiltinFunction___read__  DeeBuiltinFunction___readnp__
#define DeeBuiltinFunction___write__ DeeBuiltinFunction___writenp__
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__readnp__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__writenp__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__seek__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__flush__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__trunc__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__close__);

//////////////////////////////////////////////////////////////////////////
// Functions used by the compiler
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_dex);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_object);
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_extern);
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ff_closure);
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_assertion_failed);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_pointer_add);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_pointer_rem);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_lvalue_add);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_lvalue_rem);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_varray_add);
#endif


//////////////////////////////////////////////////////////////////////////
// Special intrinsic function used to substitute dual-meaning operators
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_posoradd__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_negorsub__);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_dereformul__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_reforand__);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_getitemorgetrange__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_delitemordelrange__);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_setitemorsetrange__);


//////////////////////////////////////////////////////////////////////////
// Exceptions related
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_last_exception);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_last_traceback);
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS */

//////////////////////////////////////////////////////////////////////////
// Misc utility functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_min); // varargs; return lowest argument (none, if no arguments)
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_max); // varargs; return greatest argument (none, if no arguments)
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_avg); // varargs; return averate of all argument (as double) Throws a ValueError if no arguments are given
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_all); // varargs; return true if all arguments are true
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_any); // varargs; return true if any argument is true
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sum); // varargs; return the sum of all arguments
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ord); // Returns the ordinal value of a character (throws a ValueError if argument isn't a 1-character string)
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_abs); // Returns the absolute value of the first argument
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ceil);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_floor);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_round);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_range);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_id);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_argv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_swap);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sqr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_enumattr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_enumerate);

//////////////////////////////////////////////////////////////////////////
// Sequence utility functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_find);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rfind);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_index);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rindex);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_contains);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_count);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_locate);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rlocate);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_locate_all);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_transform);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_empty);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_non_empty);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_front);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_back);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_get);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_at);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_reversed);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_sorted);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_segments);



//////////////////////////////////////////////////////////////////////////
// GC related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_gc_collect_now);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
//////////////////////////////////////////////////////////////////////////
// Class related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_class);
#endif

#if DEE_CONFIG_RUNTIME_HAVE_EXIT
//////////////////////////////////////////////////////////////////////////
// Exit related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_exit);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin__exit);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fatal);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_atexit);
#endif

//////////////////////////////////////////////////////////////////////////
// Stuff from the fs api
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_user);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wuser);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_machine);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wmachine);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getcwd);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgetcwd);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chdir);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgettmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettmpname);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgettmpname);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gethome);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgethome);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_listenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wlistenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_hasenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_delenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setenv);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_head);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_tail);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_file);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_ext);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_drive);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_inctrail);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_exctrail);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_abs);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_rel);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_join);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_expanduser);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_expandvars);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getatime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getctime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getmtime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettimes);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setatime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setctime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setmtime);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_settimes);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isabs);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isfile);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isdir);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_islink);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isdrive);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ismount);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ishidden);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isxfile); // Remember to watch the skies...
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ischrdev);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isblkdev);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isfifo);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_issocket);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_exists);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_remove);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_rmfile);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_mkdir);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_rmdir);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_copy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_move);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_link);
#ifdef DEE_PLATFORM_WINDOWS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32hardlink);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32modulename);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wmodulename);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32dlldirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wdlldirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32systemdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsystemdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32windowsdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wwindowsdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32systemwindowsdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsystemwindowsdirectory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32syswow64directory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsyswow64directory);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32drives);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wdrives);
#endif
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_readlink);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chmod);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getmod);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chown);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getown);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_uidof);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gidof);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_uidname);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gidname);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wuidname);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgidname);

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Raw memory / Pointer related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memcpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_mempcpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memccpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_rawmemchr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memmove);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memcmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcoll);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strxfrm);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_stpcpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_stpncpy);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcat);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncat);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memchr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memrchr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strchr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strchrnul);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcasestr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcasecmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncasecmp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memmem);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcspn);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strpbrk);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strrchr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strspn);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strstr);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strtok);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strerror);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strlen);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strnlen);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strdup);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strndup);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memset);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_malloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_calloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_realloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_free);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_malloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_calloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_realloc);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_free);
#endif

//////////////////////////////////////////////////////////////////////////
// Builtin math API
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sin);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_cos);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_tan);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sincos);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_asin);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_acos);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_atan);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_atan2);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_asincos);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sinh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_cosh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_tanh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sincosh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_asinh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_acosh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_atanh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_asincosh);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_exp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_frexp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_log);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_log10);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_modf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_exp2);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_expm1);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ilogb);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_log1p);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_log2);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_logb);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_scalbn);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_scalbi32n);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sqrt);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_cbrt);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_hypot);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_erf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_tgamma);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_lgamma);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ceilf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_floorf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_truncf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_roundf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i32round);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i64round);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fmod);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_rint);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i32rint);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i64rint);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_nearbyint);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_remainder);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_remquo);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_copysign);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_nextafter);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_nexttoward);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fdim);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isfinite);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isinf);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isnan);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isnormal);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_signbit);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isgreater);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isgreaterequal);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isless);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_islessequal);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_islessgreater);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_isunordered);

//////////////////////////////////////////////////////////////////////////
// Low-level stack access functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_code);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_get_stacklimit);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_set_stacklimit);

//////////////////////////////////////////////////////////////////////////
// Misc builtin functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_FILE);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_LINE);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_FUNCTION);

//////////////////////////////////////////////////////////////////////////
// Byte-swap builtin function
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap16);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap32);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap64);

#undef DEE_PRIVATE_DEF_BUILTIN_FUNCTION

DEE_DECL_END
#elif defined(DEE_LIMITED_DEX)
#include <deemon/runtime/builtins.h>
/*[[[deemon
#include <file>
#include <fs>

fs::chdir(fs::path::head(__FILE__));
local mappings = list();

local longest_name = 0;
for (local line: file.io("builtins.c","r")) {
  local id,name;
  try id,name = line.scanf(" /" "* %[xX0-9a-fA-F] *" "/ ( DeeObject * ) & DeeBuiltinFunction_%[^, ]")...;
  catch (...) continue;
  if (#name > longest_name) longest_name = #name;
  if (name !in pack "__readnp__","__writenp__")
    mappings.append(pack name,id);
}

// Export builtins
for (local name,id: mappings) {
  print "#define DeeBuiltinFunction_"+name+" "*(longest_name-#name)+" (*DeeBuiltin_GET("+id+"))";
}

]]]*/
#define DeeBuiltinFunction___builtin_object                          (*DeeBuiltin_GET(0x000D))
#define DeeBuiltinFunction___repr__                                  (*DeeBuiltin_GET(0x002A))
#define DeeBuiltinFunction___type__                                  (*DeeBuiltin_GET(0x002B))
#define DeeBuiltinFunction___copy__                                  (*DeeBuiltin_GET(0x002C))
#define DeeBuiltinFunction___move__                                  (*DeeBuiltin_GET(0x002D))
#define DeeBuiltinFunction___builtin_last_exception                  (*DeeBuiltin_GET(0x002E))
#define DeeBuiltinFunction___builtin_last_traceback                  (*DeeBuiltin_GET(0x002F))
#define DeeBuiltinFunction___copyassign__                            (*DeeBuiltin_GET(0x0030))
#define DeeBuiltinFunction___not__                                   (*DeeBuiltin_GET(0x0031))
#define DeeBuiltinFunction___neg__                                   (*DeeBuiltin_GET(0x0032))
#define DeeBuiltinFunction___pos__                                   (*DeeBuiltin_GET(0x0033))
#define DeeBuiltinFunction___inv__                                   (*DeeBuiltin_GET(0x0034))
#define DeeBuiltinFunction___ref__                                   (*DeeBuiltin_GET(0x0035))
#define DeeBuiltinFunction___deref__                                 (*DeeBuiltin_GET(0x0036))
#define DeeBuiltinFunction___inc__                                   (*DeeBuiltin_GET(0x0037))
#define DeeBuiltinFunction___dec__                                   (*DeeBuiltin_GET(0x0038))
#define DeeBuiltinFunction___incpost__                               (*DeeBuiltin_GET(0x0039))
#define DeeBuiltinFunction___decpost__                               (*DeeBuiltin_GET(0x003A))
#define DeeBuiltinFunction___lo__                                    (*DeeBuiltin_GET(0x003B))
#define DeeBuiltinFunction___le__                                    (*DeeBuiltin_GET(0x003C))
#define DeeBuiltinFunction___eq__                                    (*DeeBuiltin_GET(0x003D))
#define DeeBuiltinFunction___ne__                                    (*DeeBuiltin_GET(0x003E))
#define DeeBuiltinFunction___gr__                                    (*DeeBuiltin_GET(0x003F))
#define DeeBuiltinFunction___ge__                                    (*DeeBuiltin_GET(0x0040))
#define DeeBuiltinFunction___add__                                   (*DeeBuiltin_GET(0x0041))
#define DeeBuiltinFunction___iadd__                                  (*DeeBuiltin_GET(0x0042))
#define DeeBuiltinFunction___sub__                                   (*DeeBuiltin_GET(0x0043))
#define DeeBuiltinFunction___isub__                                  (*DeeBuiltin_GET(0x0044))
#define DeeBuiltinFunction___mul__                                   (*DeeBuiltin_GET(0x0045))
#define DeeBuiltinFunction___imul__                                  (*DeeBuiltin_GET(0x0046))
#define DeeBuiltinFunction___div__                                   (*DeeBuiltin_GET(0x0047))
#define DeeBuiltinFunction___idiv__                                  (*DeeBuiltin_GET(0x0048))
#define DeeBuiltinFunction___mod__                                   (*DeeBuiltin_GET(0x0049))
#define DeeBuiltinFunction___imod__                                  (*DeeBuiltin_GET(0x004A))
#define DeeBuiltinFunction___shl__                                   (*DeeBuiltin_GET(0x004B))
#define DeeBuiltinFunction___ishl__                                  (*DeeBuiltin_GET(0x004C))
#define DeeBuiltinFunction___shr__                                   (*DeeBuiltin_GET(0x004D))
#define DeeBuiltinFunction___ishr__                                  (*DeeBuiltin_GET(0x004E))
#define DeeBuiltinFunction___and__                                   (*DeeBuiltin_GET(0x004F))
#define DeeBuiltinFunction___iand__                                  (*DeeBuiltin_GET(0x0050))
#define DeeBuiltinFunction___or__                                    (*DeeBuiltin_GET(0x0051))
#define DeeBuiltinFunction___ior__                                   (*DeeBuiltin_GET(0x0052))
#define DeeBuiltinFunction___xor__                                   (*DeeBuiltin_GET(0x0053))
#define DeeBuiltinFunction___ixor__                                  (*DeeBuiltin_GET(0x0054))
#define DeeBuiltinFunction___pow__                                   (*DeeBuiltin_GET(0x0055))
#define DeeBuiltinFunction___ipow__                                  (*DeeBuiltin_GET(0x0056))
#define DeeBuiltinFunction___call__                                  (*DeeBuiltin_GET(0x0057))
#define DeeBuiltinFunction___size__                                  (*DeeBuiltin_GET(0x0058))
#define DeeBuiltinFunction___getitem__                               (*DeeBuiltin_GET(0x0059))
#define DeeBuiltinFunction___setitem__                               (*DeeBuiltin_GET(0x005A))
#define DeeBuiltinFunction___delitem__                               (*DeeBuiltin_GET(0x005B))
#define DeeBuiltinFunction___getrange__                              (*DeeBuiltin_GET(0x005C))
#define DeeBuiltinFunction___setrange__                              (*DeeBuiltin_GET(0x005D))
#define DeeBuiltinFunction___delrange__                              (*DeeBuiltin_GET(0x005E))
#define DeeBuiltinFunction___is__                                    (*DeeBuiltin_GET(0x005F))
#define DeeBuiltinFunction___contains__                              (*DeeBuiltin_GET(0x0060))
#define DeeBuiltinFunction___getattr__                               (*DeeBuiltin_GET(0x0061))
#define DeeBuiltinFunction___delattr__                               (*DeeBuiltin_GET(0x0062))
#define DeeBuiltinFunction___setattr__                               (*DeeBuiltin_GET(0x0063))
#define DeeBuiltinFunction___iterself__                              (*DeeBuiltin_GET(0x0064))
#define DeeBuiltinFunction___iternext__                              (*DeeBuiltin_GET(0x0065))
#define DeeBuiltinFunction___builtin_pointer_add                     (*DeeBuiltin_GET(0x0086))
#define DeeBuiltinFunction___builtin_pointer_rem                     (*DeeBuiltin_GET(0x0087))
#define DeeBuiltinFunction___builtin_lvalue_add                      (*DeeBuiltin_GET(0x0088))
#define DeeBuiltinFunction___builtin_lvalue_rem                      (*DeeBuiltin_GET(0x0089))
#define DeeBuiltinFunction___builtin_fs_getcwd                       (*DeeBuiltin_GET(0x009A))
#define DeeBuiltinFunction___builtin_fs_chdir                        (*DeeBuiltin_GET(0x009B))
#define DeeBuiltinFunction___builtin_fs_gethome                      (*DeeBuiltin_GET(0x009C))
#define DeeBuiltinFunction___builtin_fs_listenv                      (*DeeBuiltin_GET(0x009D))
#define DeeBuiltinFunction___builtin_fs_getenv                       (*DeeBuiltin_GET(0x009E))
#define DeeBuiltinFunction___builtin_fs_hasenv                       (*DeeBuiltin_GET(0x009F))
#define DeeBuiltinFunction___builtin_fs_delenv                       (*DeeBuiltin_GET(0x00A0))
#define DeeBuiltinFunction___builtin_fs_setenv                       (*DeeBuiltin_GET(0x00A1))
#define DeeBuiltinFunction___builtin_fs_path_head                    (*DeeBuiltin_GET(0x00A2))
#define DeeBuiltinFunction___builtin_fs_path_tail                    (*DeeBuiltin_GET(0x00A3))
#define DeeBuiltinFunction___builtin_fs_path_file                    (*DeeBuiltin_GET(0x00A4))
#define DeeBuiltinFunction___builtin_fs_path_ext                     (*DeeBuiltin_GET(0x00A5))
#define DeeBuiltinFunction___builtin_fs_path_drive                   (*DeeBuiltin_GET(0x00A6))
#define DeeBuiltinFunction___builtin_fs_path_inctrail                (*DeeBuiltin_GET(0x00A7))
#define DeeBuiltinFunction___builtin_fs_path_exctrail                (*DeeBuiltin_GET(0x00A8))
#define DeeBuiltinFunction___builtin_fs_path_abs                     (*DeeBuiltin_GET(0x00A9))
#define DeeBuiltinFunction___builtin_fs_path_rel                     (*DeeBuiltin_GET(0x00AA))
#define DeeBuiltinFunction___builtin_fs_path_join                    (*DeeBuiltin_GET(0x00AB))
#define DeeBuiltinFunction___builtin_fs_path_expanduser              (*DeeBuiltin_GET(0x00AC))
#define DeeBuiltinFunction___builtin_fs_path_expandvars              (*DeeBuiltin_GET(0x00AD))
#define DeeBuiltinFunction___builtin_fs_getatime                     (*DeeBuiltin_GET(0x00AE))
#define DeeBuiltinFunction___builtin_fs_getctime                     (*DeeBuiltin_GET(0x00AF))
#define DeeBuiltinFunction___builtin_fs_getmtime                     (*DeeBuiltin_GET(0x00B0))
#define DeeBuiltinFunction___builtin_fs_gettimes                     (*DeeBuiltin_GET(0x00B1))
#define DeeBuiltinFunction___builtin_fs_isabs                        (*DeeBuiltin_GET(0x00B2))
#define DeeBuiltinFunction___builtin_fs_isfile                       (*DeeBuiltin_GET(0x00B3))
#define DeeBuiltinFunction___builtin_fs_isdir                        (*DeeBuiltin_GET(0x00B4))
#define DeeBuiltinFunction___builtin_fs_islink                       (*DeeBuiltin_GET(0x00B5))
#define DeeBuiltinFunction___builtin_fs_isdrive                      (*DeeBuiltin_GET(0x00B6))
#define DeeBuiltinFunction___builtin_fs_ismount                      (*DeeBuiltin_GET(0x00B7))
#define DeeBuiltinFunction___builtin_fs_ishidden                     (*DeeBuiltin_GET(0x00B8))
#define DeeBuiltinFunction___builtin_fs_exists                       (*DeeBuiltin_GET(0x00B9))
#define DeeBuiltinFunction___builtin_fs_remove                       (*DeeBuiltin_GET(0x00BA))
#define DeeBuiltinFunction___builtin_fs_rmfile                       (*DeeBuiltin_GET(0x00BB))
#define DeeBuiltinFunction___builtin_fs_mkdir                        (*DeeBuiltin_GET(0x00BC))
#define DeeBuiltinFunction___builtin_fs_rmdir                        (*DeeBuiltin_GET(0x00BD))
#define DeeBuiltinFunction___builtin_fs_copy                         (*DeeBuiltin_GET(0x00BE))
#define DeeBuiltinFunction___builtin_fs_move                         (*DeeBuiltin_GET(0x00BF))
#define DeeBuiltinFunction___builtin_fs_link                         (*DeeBuiltin_GET(0x00C0))
#define DeeBuiltinFunction___builtin_fs_readlink                     (*DeeBuiltin_GET(0x00C1))
#define DeeBuiltinFunction___builtin_assertion_failed                (*DeeBuiltin_GET(0x00C2))
#define DeeBuiltinFunction___builtin_memcpy                          (*DeeBuiltin_GET(0x00C5))
#define DeeBuiltinFunction___builtin_memmove                         (*DeeBuiltin_GET(0x00C6))
#define DeeBuiltinFunction___builtin_memcmp                          (*DeeBuiltin_GET(0x00C7))
#define DeeBuiltinFunction___builtin_strcoll                         (*DeeBuiltin_GET(0x00C8))
#define DeeBuiltinFunction___builtin_strcmp                          (*DeeBuiltin_GET(0x00C9))
#define DeeBuiltinFunction___builtin_strncmp                         (*DeeBuiltin_GET(0x00CA))
#define DeeBuiltinFunction___builtin_strxfrm                         (*DeeBuiltin_GET(0x00CB))
#define DeeBuiltinFunction___builtin_strcpy                          (*DeeBuiltin_GET(0x00CC))
#define DeeBuiltinFunction___builtin_strncpy                         (*DeeBuiltin_GET(0x00CD))
#define DeeBuiltinFunction___builtin_strcat                          (*DeeBuiltin_GET(0x00CE))
#define DeeBuiltinFunction___builtin_memchr                          (*DeeBuiltin_GET(0x00CF))
#define DeeBuiltinFunction___builtin_strchr                          (*DeeBuiltin_GET(0x00D0))
#define DeeBuiltinFunction___builtin_strcspn                         (*DeeBuiltin_GET(0x00D1))
#define DeeBuiltinFunction___builtin_strpbrk                         (*DeeBuiltin_GET(0x00D2))
#define DeeBuiltinFunction___builtin_strrchr                         (*DeeBuiltin_GET(0x00D3))
#define DeeBuiltinFunction___builtin_strspn                          (*DeeBuiltin_GET(0x00D4))
#define DeeBuiltinFunction___builtin_strstr                          (*DeeBuiltin_GET(0x00D5))
#define DeeBuiltinFunction___builtin_strtok                          (*DeeBuiltin_GET(0x00D6))
#define DeeBuiltinFunction___builtin_strerror                        (*DeeBuiltin_GET(0x00D7))
#define DeeBuiltinFunction___builtin_strlen                          (*DeeBuiltin_GET(0x00D8))
#define DeeBuiltinFunction___builtin_strnlen                         (*DeeBuiltin_GET(0x00D9))
#define DeeBuiltinFunction___builtin_malloc                          (*DeeBuiltin_GET(0x00DA))
#define DeeBuiltinFunction___builtin_calloc                          (*DeeBuiltin_GET(0x00DB))
#define DeeBuiltinFunction___builtin_realloc                         (*DeeBuiltin_GET(0x00DC))
#define DeeBuiltinFunction___builtin_free                            (*DeeBuiltin_GET(0x00DD))
#define DeeBuiltinFunction___builtin_aligned_malloc                  (*DeeBuiltin_GET(0x00DE))
#define DeeBuiltinFunction___builtin_aligned_calloc                  (*DeeBuiltin_GET(0x00DF))
#define DeeBuiltinFunction___builtin_aligned_realloc                 (*DeeBuiltin_GET(0x00E0))
#define DeeBuiltinFunction___builtin_aligned_free                    (*DeeBuiltin_GET(0x00E1))
#define DeeBuiltinFunction___builtin_gc_collect_now                  (*DeeBuiltin_GET(0x00E6))
#define DeeBuiltinFunction___builtin_fs_isxfile                      (*DeeBuiltin_GET(0x00E7))
#define DeeBuiltinFunction___builtin_strdup                          (*DeeBuiltin_GET(0x00E8))
#define DeeBuiltinFunction___builtin_strndup                         (*DeeBuiltin_GET(0x00E9))
#define DeeBuiltinFunction___builtin_strncat                         (*DeeBuiltin_GET(0x00EA))
#define DeeBuiltinFunction___builtin_fs_win32hardlink                (*DeeBuiltin_GET(0x00ED))
#define DeeBuiltinFunction___builtin_fs_win32modulename              (*DeeBuiltin_GET(0x00EE))
#define DeeBuiltinFunction___builtin_exit                            (*DeeBuiltin_GET(0x00EF))
#define DeeBuiltinFunction___builtin__exit                           (*DeeBuiltin_GET(0x00F0))
#define DeeBuiltinFunction___builtin_fatal                           (*DeeBuiltin_GET(0x00F1))
#define DeeBuiltinFunction___builtin_atexit                          (*DeeBuiltin_GET(0x00F2))
#define DeeBuiltinFunction___builtin_memset                          (*DeeBuiltin_GET(0x00F5))
#define DeeBuiltinFunction___builtin_mempcpy                         (*DeeBuiltin_GET(0x00F6))
#define DeeBuiltinFunction___builtin_memccpy                         (*DeeBuiltin_GET(0x00F7))
#define DeeBuiltinFunction___builtin_rawmemchr                       (*DeeBuiltin_GET(0x00F8))
#define DeeBuiltinFunction___builtin_memrchr                         (*DeeBuiltin_GET(0x00F9))
#define DeeBuiltinFunction___builtin_strchrnul                       (*DeeBuiltin_GET(0x00FA))
#define DeeBuiltinFunction___builtin_strcasestr                      (*DeeBuiltin_GET(0x00FB))
#define DeeBuiltinFunction___builtin_strcasecmp                      (*DeeBuiltin_GET(0x00FC))
#define DeeBuiltinFunction___builtin_memmem                          (*DeeBuiltin_GET(0x00FD))
#define DeeBuiltinFunction___builtin_strncasecmp                     (*DeeBuiltin_GET(0x00FE))
#define DeeBuiltinFunction___builtin_stpcpy                          (*DeeBuiltin_GET(0x00FF))
#define DeeBuiltinFunction___builtin_stpncpy                         (*DeeBuiltin_GET(0x0100))
#define DeeBuiltinFunction___builtin_min                             (*DeeBuiltin_GET(0x0102))
#define DeeBuiltinFunction___builtin_max                             (*DeeBuiltin_GET(0x0103))
#define DeeBuiltinFunction___builtin_avg                             (*DeeBuiltin_GET(0x0104))
#define DeeBuiltinFunction___builtin_all                             (*DeeBuiltin_GET(0x0105))
#define DeeBuiltinFunction___builtin_any                             (*DeeBuiltin_GET(0x0106))
#define DeeBuiltinFunction___builtin_sum                             (*DeeBuiltin_GET(0x0107))
#define DeeBuiltinFunction___builtin_sqr                             (*DeeBuiltin_GET(0x0108))
#define DeeBuiltinFunction___builtin_ord                             (*DeeBuiltin_GET(0x0109))
#define DeeBuiltinFunction___builtin_abs                             (*DeeBuiltin_GET(0x010A))
#define DeeBuiltinFunction___builtin_ceil                            (*DeeBuiltin_GET(0x010B))
#define DeeBuiltinFunction___builtin_floor                           (*DeeBuiltin_GET(0x010C))
#define DeeBuiltinFunction___builtin_round                           (*DeeBuiltin_GET(0x010D))
#define DeeBuiltinFunction___builtin_range                           (*DeeBuiltin_GET(0x0117))
#define DeeBuiltinFunction___builtin_id                              (*DeeBuiltin_GET(0x0118))
#define DeeBuiltinFunction___builtin_sin                             (*DeeBuiltin_GET(0x0119))
#define DeeBuiltinFunction___builtin_cos                             (*DeeBuiltin_GET(0x011A))
#define DeeBuiltinFunction___builtin_tan                             (*DeeBuiltin_GET(0x011B))
#define DeeBuiltinFunction___builtin_sincos                          (*DeeBuiltin_GET(0x011C))
#define DeeBuiltinFunction___builtin_asin                            (*DeeBuiltin_GET(0x011D))
#define DeeBuiltinFunction___builtin_acos                            (*DeeBuiltin_GET(0x011E))
#define DeeBuiltinFunction___builtin_atan                            (*DeeBuiltin_GET(0x011F))
#define DeeBuiltinFunction___builtin_atan2                           (*DeeBuiltin_GET(0x0120))
#define DeeBuiltinFunction___builtin_asincos                         (*DeeBuiltin_GET(0x0121))
#define DeeBuiltinFunction___builtin_sinh                            (*DeeBuiltin_GET(0x0122))
#define DeeBuiltinFunction___builtin_cosh                            (*DeeBuiltin_GET(0x0123))
#define DeeBuiltinFunction___builtin_tanh                            (*DeeBuiltin_GET(0x0124))
#define DeeBuiltinFunction___builtin_sincosh                         (*DeeBuiltin_GET(0x0125))
#define DeeBuiltinFunction___builtin_asinh                           (*DeeBuiltin_GET(0x0126))
#define DeeBuiltinFunction___builtin_acosh                           (*DeeBuiltin_GET(0x0127))
#define DeeBuiltinFunction___builtin_atanh                           (*DeeBuiltin_GET(0x0128))
#define DeeBuiltinFunction___builtin_asincosh                        (*DeeBuiltin_GET(0x0129))
#define DeeBuiltinFunction___builtin_exp                             (*DeeBuiltin_GET(0x012A))
#define DeeBuiltinFunction___builtin_frexp                           (*DeeBuiltin_GET(0x012B))
#define DeeBuiltinFunction___builtin_log                             (*DeeBuiltin_GET(0x012C))
#define DeeBuiltinFunction___builtin_log10                           (*DeeBuiltin_GET(0x012D))
#define DeeBuiltinFunction___builtin_modf                            (*DeeBuiltin_GET(0x012E))
#define DeeBuiltinFunction___builtin_exp2                            (*DeeBuiltin_GET(0x012F))
#define DeeBuiltinFunction___builtin_expm1                           (*DeeBuiltin_GET(0x0130))
#define DeeBuiltinFunction___builtin_ilogb                           (*DeeBuiltin_GET(0x0131))
#define DeeBuiltinFunction___builtin_log1p                           (*DeeBuiltin_GET(0x0132))
#define DeeBuiltinFunction___builtin_log2                            (*DeeBuiltin_GET(0x0133))
#define DeeBuiltinFunction___builtin_logb                            (*DeeBuiltin_GET(0x0134))
#define DeeBuiltinFunction___builtin_scalbn                          (*DeeBuiltin_GET(0x0135))
#define DeeBuiltinFunction___builtin_scalbi32n                       (*DeeBuiltin_GET(0x0136))
#define DeeBuiltinFunction___builtin_sqrt                            (*DeeBuiltin_GET(0x0137))
#define DeeBuiltinFunction___builtin_cbrt                            (*DeeBuiltin_GET(0x0138))
#define DeeBuiltinFunction___builtin_hypot                           (*DeeBuiltin_GET(0x0139))
#define DeeBuiltinFunction___builtin_erf                             (*DeeBuiltin_GET(0x013A))
#define DeeBuiltinFunction___builtin_tgamma                          (*DeeBuiltin_GET(0x013B))
#define DeeBuiltinFunction___builtin_lgamma                          (*DeeBuiltin_GET(0x013C))
#define DeeBuiltinFunction___builtin_ceilf                           (*DeeBuiltin_GET(0x013D))
#define DeeBuiltinFunction___builtin_floorf                          (*DeeBuiltin_GET(0x013E))
#define DeeBuiltinFunction___builtin_truncf                          (*DeeBuiltin_GET(0x013F))
#define DeeBuiltinFunction___builtin_roundf                          (*DeeBuiltin_GET(0x0140))
#define DeeBuiltinFunction___builtin_i32round                        (*DeeBuiltin_GET(0x0141))
#define DeeBuiltinFunction___builtin_i64round                        (*DeeBuiltin_GET(0x0142))
#define DeeBuiltinFunction___builtin_fmod                            (*DeeBuiltin_GET(0x0143))
#define DeeBuiltinFunction___builtin_rint                            (*DeeBuiltin_GET(0x0144))
#define DeeBuiltinFunction___builtin_i32rint                         (*DeeBuiltin_GET(0x0145))
#define DeeBuiltinFunction___builtin_i64rint                         (*DeeBuiltin_GET(0x0146))
#define DeeBuiltinFunction___builtin_nearbyint                       (*DeeBuiltin_GET(0x0147))
#define DeeBuiltinFunction___builtin_remainder                       (*DeeBuiltin_GET(0x0148))
#define DeeBuiltinFunction___builtin_remquo                          (*DeeBuiltin_GET(0x0149))
#define DeeBuiltinFunction___builtin_copysign                        (*DeeBuiltin_GET(0x014A))
#define DeeBuiltinFunction___builtin_nextafter                       (*DeeBuiltin_GET(0x014B))
#define DeeBuiltinFunction___builtin_nexttoward                      (*DeeBuiltin_GET(0x014C))
#define DeeBuiltinFunction___builtin_fdim                            (*DeeBuiltin_GET(0x014D))
#define DeeBuiltinFunction___builtin_isfinite                        (*DeeBuiltin_GET(0x014E))
#define DeeBuiltinFunction___builtin_isinf                           (*DeeBuiltin_GET(0x014F))
#define DeeBuiltinFunction___builtin_isnan                           (*DeeBuiltin_GET(0x0150))
#define DeeBuiltinFunction___builtin_isnormal                        (*DeeBuiltin_GET(0x0151))
#define DeeBuiltinFunction___builtin_signbit                         (*DeeBuiltin_GET(0x0152))
#define DeeBuiltinFunction___builtin_isgreater                       (*DeeBuiltin_GET(0x0153))
#define DeeBuiltinFunction___builtin_isgreaterequal                  (*DeeBuiltin_GET(0x0154))
#define DeeBuiltinFunction___builtin_isless                          (*DeeBuiltin_GET(0x0155))
#define DeeBuiltinFunction___builtin_islessequal                     (*DeeBuiltin_GET(0x0156))
#define DeeBuiltinFunction___builtin_islessgreater                   (*DeeBuiltin_GET(0x0157))
#define DeeBuiltinFunction___builtin_isunordered                     (*DeeBuiltin_GET(0x0158))
#define DeeBuiltinFunction___builtin_FILE                            (*DeeBuiltin_GET(0x015E))
#define DeeBuiltinFunction___builtin_LINE                            (*DeeBuiltin_GET(0x015F))
#define DeeBuiltinFunction___builtin_FUNCTION                        (*DeeBuiltin_GET(0x0160))
#define DeeBuiltinFunction___builtin_argv                            (*DeeBuiltin_GET(0x0161))
#define DeeBuiltinFunction___builtin_fs_win32dlldirectory            (*DeeBuiltin_GET(0x0162))
#define DeeBuiltinFunction___builtin_fs_win32systemdirectory         (*DeeBuiltin_GET(0x0163))
#define DeeBuiltinFunction___builtin_fs_gettmp                       (*DeeBuiltin_GET(0x0164))
#define DeeBuiltinFunction___builtin_fs_win32windowsdirectory        (*DeeBuiltin_GET(0x0165))
#define DeeBuiltinFunction___builtin_fs_win32systemwindowsdirectory  (*DeeBuiltin_GET(0x0166))
#define DeeBuiltinFunction___builtin_fs_win32syswow64directory       (*DeeBuiltin_GET(0x0167))
#define DeeBuiltinFunction___builtin_fs_gettmpname                   (*DeeBuiltin_GET(0x0168))
#define DeeBuiltinFunction___builtin_code                            (*DeeBuiltin_GET(0x0169))
#define DeeBuiltinFunction___builtin_fs_ischrdev                     (*DeeBuiltin_GET(0x016E))
#define DeeBuiltinFunction___builtin_fs_isblkdev                     (*DeeBuiltin_GET(0x016F))
#define DeeBuiltinFunction___builtin_fs_isfifo                       (*DeeBuiltin_GET(0x0170))
#define DeeBuiltinFunction___builtin_fs_issocket                     (*DeeBuiltin_GET(0x0171))
#define DeeBuiltinFunction___builtin_class                           (*DeeBuiltin_GET(0x0173))
#define DeeBuiltinFunction___builtin_swap                            (*DeeBuiltin_GET(0x0175))
#define DeeBuiltinFunction___builtin_bswap16                         (*DeeBuiltin_GET(0x0177))
#define DeeBuiltinFunction___builtin_bswap32                         (*DeeBuiltin_GET(0x0178))
#define DeeBuiltinFunction___builtin_bswap64                         (*DeeBuiltin_GET(0x0179))
#define DeeBuiltinFunction___builtin_fs_setatime                     (*DeeBuiltin_GET(0x0180))
#define DeeBuiltinFunction___builtin_fs_setctime                     (*DeeBuiltin_GET(0x0181))
#define DeeBuiltinFunction___builtin_fs_setmtime                     (*DeeBuiltin_GET(0x0182))
#define DeeBuiltinFunction___builtin_fs_settimes                     (*DeeBuiltin_GET(0x0183))
#define DeeBuiltinFunction___builtin_seq_contains                    (*DeeBuiltin_GET(0x0189))
#define DeeBuiltinFunction___hasattr__                               (*DeeBuiltin_GET(0x018B))
#define DeeBuiltinFunction___builtin_fs_user                         (*DeeBuiltin_GET(0x0195))
#define DeeBuiltinFunction___builtin_fs_machine                      (*DeeBuiltin_GET(0x0196))
#define DeeBuiltinFunction___builtin_fs_wuser                        (*DeeBuiltin_GET(0x0197))
#define DeeBuiltinFunction___builtin_fs_wmachine                     (*DeeBuiltin_GET(0x0198))
#define DeeBuiltinFunction___builtin_fs_wgetcwd                      (*DeeBuiltin_GET(0x0199))
#define DeeBuiltinFunction___builtin_fs_wgettmp                      (*DeeBuiltin_GET(0x019A))
#define DeeBuiltinFunction___builtin_fs_wgethome                     (*DeeBuiltin_GET(0x019B))
#define DeeBuiltinFunction___builtin_fs_wlistenv                     (*DeeBuiltin_GET(0x019C))
#define DeeBuiltinFunction___builtin_fs_win32wmodulename             (*DeeBuiltin_GET(0x019D))
#define DeeBuiltinFunction___builtin_fs_win32wdlldirectory           (*DeeBuiltin_GET(0x019E))
#define DeeBuiltinFunction___builtin_fs_win32wsystemdirectory        (*DeeBuiltin_GET(0x019F))
#define DeeBuiltinFunction___builtin_fs_win32wwindowsdirectory       (*DeeBuiltin_GET(0x01A0))
#define DeeBuiltinFunction___builtin_fs_win32wsystemwindowsdirectory (*DeeBuiltin_GET(0x01A1))
#define DeeBuiltinFunction___builtin_fs_win32wsyswow64directory      (*DeeBuiltin_GET(0x01A2))
#define DeeBuiltinFunction___builtin_fs_wgettmpname                  (*DeeBuiltin_GET(0x01A3))
#define DeeBuiltinFunction___builtin_get_stacklimit                  (*DeeBuiltin_GET(0x01B0))
#define DeeBuiltinFunction___builtin_set_stacklimit                  (*DeeBuiltin_GET(0x01B1))
#define DeeBuiltinFunction___builtin_seq_find                        (*DeeBuiltin_GET(0x01B2))
#define DeeBuiltinFunction___builtin_seq_rfind                       (*DeeBuiltin_GET(0x01B3))
#define DeeBuiltinFunction___builtin_seq_index                       (*DeeBuiltin_GET(0x01B4))
#define DeeBuiltinFunction___builtin_seq_rindex                      (*DeeBuiltin_GET(0x01B5))
#define DeeBuiltinFunction___builtin_seq_count                       (*DeeBuiltin_GET(0x01B6))
#define DeeBuiltinFunction___builtin_seq_locate                      (*DeeBuiltin_GET(0x01B7))
#define DeeBuiltinFunction___builtin_seq_rlocate                     (*DeeBuiltin_GET(0x01B8))
#define DeeBuiltinFunction___builtin_seq_locate_all                  (*DeeBuiltin_GET(0x01B9))
#define DeeBuiltinFunction___builtin_seq_transform                   (*DeeBuiltin_GET(0x01BA))
#define DeeBuiltinFunction___builtin_seq_empty                       (*DeeBuiltin_GET(0x01BB))
#define DeeBuiltinFunction___builtin_seq_non_empty                   (*DeeBuiltin_GET(0x01BC))
#define DeeBuiltinFunction___builtin_seq_front                       (*DeeBuiltin_GET(0x01BD))
#define DeeBuiltinFunction___builtin_seq_back                        (*DeeBuiltin_GET(0x01BE))
#define DeeBuiltinFunction___builtin_seq_get                         (*DeeBuiltin_GET(0x01BF))
#define DeeBuiltinFunction___builtin_seq_at                          (*DeeBuiltin_GET(0x01C0))
#define DeeBuiltinFunction___builtin_seq_reversed                    (*DeeBuiltin_GET(0x01C1))
#define DeeBuiltinFunction___builtin_seq_sorted                      (*DeeBuiltin_GET(0x01C2))
#define DeeBuiltinFunction___builtin_seq_segments                    (*DeeBuiltin_GET(0x01C3))
#define DeeBuiltinFunction___builtin_fs_chmod                        (*DeeBuiltin_GET(0x01C5))
#define DeeBuiltinFunction___builtin_fs_getmod                       (*DeeBuiltin_GET(0x01C6))
#define DeeBuiltinFunction___builtin_fs_chown                        (*DeeBuiltin_GET(0x01C7))
#define DeeBuiltinFunction___builtin_fs_getown                       (*DeeBuiltin_GET(0x01C8))
#define DeeBuiltinFunction___builtin_fs_uidof                        (*DeeBuiltin_GET(0x01C9))
#define DeeBuiltinFunction___builtin_fs_gidof                        (*DeeBuiltin_GET(0x01CA))
#define DeeBuiltinFunction___builtin_fs_uidname                      (*DeeBuiltin_GET(0x01CB))
#define DeeBuiltinFunction___builtin_fs_wuidname                     (*DeeBuiltin_GET(0x01CC))
#define DeeBuiltinFunction___builtin_fs_gidname                      (*DeeBuiltin_GET(0x01CD))
#define DeeBuiltinFunction___builtin_fs_wgidname                     (*DeeBuiltin_GET(0x01CE))
#define DeeBuiltinFunction___builtin_fs_win32drives                  (*DeeBuiltin_GET(0x01CF))
#define DeeBuiltinFunction___builtin_fs_win32wdrives                 (*DeeBuiltin_GET(0x01D0))
#define DeeBuiltinFunction___moveassign__                            (*DeeBuiltin_GET(0x01D4))
#define DeeBuiltinFunction___callv__                                 (*DeeBuiltin_GET(0x01D5))
#define DeeBuiltinFunction___hash__                                  (*DeeBuiltin_GET(0x01D6))
#define DeeBuiltinFunction___dual_posoradd__                         (*DeeBuiltin_GET(0x01D7))
#define DeeBuiltinFunction___dual_negorsub__                         (*DeeBuiltin_GET(0x01D8))
#define DeeBuiltinFunction___dual_dereformul__                       (*DeeBuiltin_GET(0x01D9))
#define DeeBuiltinFunction___dual_reforand__                         (*DeeBuiltin_GET(0x01DA))
#define DeeBuiltinFunction___dual_getitemorgetrange__                (*DeeBuiltin_GET(0x01DB))
#define DeeBuiltinFunction___dual_delitemordelrange__                (*DeeBuiltin_GET(0x01DC))
#define DeeBuiltinFunction___dual_setitemorsetrange__                (*DeeBuiltin_GET(0x01DD))
#define DeeBuiltinFunction___builtin_dex                             (*DeeBuiltin_GET(0x01DE))
#define DeeBuiltinFunction___bool__                                  (*DeeBuiltin_GET(0x01DF))
#define DeeBuiltinFunction___builtin_extern                          (*DeeBuiltin_GET(0x01E0))
#define DeeBuiltinFunction___builtin_ff_closure                      (*DeeBuiltin_GET(0x01E1))
#define DeeBuiltinFunction___builtin_enumattr                        (*DeeBuiltin_GET(0x01E2))
#define DeeBuiltinFunction___read__                                  (*DeeBuiltin_GET(0x01E3))
#define DeeBuiltinFunction___write__                                 (*DeeBuiltin_GET(0x01E4))
#define DeeBuiltinFunction___readp__                                 (*DeeBuiltin_GET(0x01E5))
#define DeeBuiltinFunction___writep__                                (*DeeBuiltin_GET(0x01E6))
#define DeeBuiltinFunction___seek__                                  (*DeeBuiltin_GET(0x01E9))
#define DeeBuiltinFunction___flush__                                 (*DeeBuiltin_GET(0x01EA))
#define DeeBuiltinFunction___trunc__                                 (*DeeBuiltin_GET(0x01EB))
#define DeeBuiltinFunction___close__                                 (*DeeBuiltin_GET(0x01EC))
#define DeeBuiltinFunction___builtin_enumerate                       (*DeeBuiltin_GET(0x01F1))
//[[[end]]]
#define DeeBuiltinFunction___readnp__                                (*DeeBuiltin_GET(0x01E7))
#define DeeBuiltinFunction___writenp__                               (*DeeBuiltin_GET(0x01E8))
#endif

#endif /* !GUARD_DEEMON_BUILTIN_FUNCTIONS_H */
