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

#include <deemon/__conf.inl>
#include <deemon/cfunction.h>

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

#endif /* !GUARD_DEEMON_BUILTIN_FUNCTIONS_H */
