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
#ifndef GUARD_DEEMON_DEEMONRUN_H
#define GUARD_DEEMON_DEEMONRUN_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/std/stdarg.h>

// NOTE: Keep this file independent from other deemon headers

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

#if DEE_CONFIG_RUNTIME_HAVE_EXIT
//////////////////////////////////////////////////////////////////////////
// Register a callback that executes 'func(args...)' when Dee_Exit() is called
// or when the library finalized with 'Dee_Finalize'.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) Dee_AtExit(
 DEE_A_INOUT DeeObject *func, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_NORETURN void) Dee_Exit(DEE_A_IN int exitcode);
DEE_FUNC_DECL(           DEE_ATTRIBUTE_NORETURN void) Dee__Exit(DEE_A_IN int exitcode); // Doesn't execute 'Dee_AtExit' callbacks
#define Dee_Fatal() Dee__Exit(1)
#endif


#define DEE_INITIALIZE_FLAGS_NONE     DEE_UINT32_C(0x00000000)
// Don't initialize signal handlers if they need initialization.
// >> Always set 'DeeFlag_NoSignalHandlers'
//    to non-zero, thereby disabling signal handlers al-together.
// Same as defining '$DEEMON_NOSIGHANDLERS' as non-zero
// before calling 'Dee_Initialize' / 'Dee_InitializeEx'.
#define DEE_INITIALIZE_FLAGS_NOSIGNAL DEE_UINT32_C(0x00000001)

#define DEE_FINALIZE_FLAGS_NONE       DEE_UINT32_C(0x00000000)
#if DEE_CONFIG_RUNTIME_HAVE_EXIT
#define DEE_FINALIZE_FLAGS_NOCALLBACK DEE_UINT32_C(0x00000001) /*< Don't execute callbacks registered with 'Dee_AtExit' */
#else /* DEE_CONFIG_RUNTIME_HAVE_EXIT */
#define DEE_FINALIZE_FLAGS_NOCALLBACK DEE_UINT32_C(0x00000000)
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXIT */

//////////////////////////////////////////////////////////////////////////
// Lib initialization / finalization
// >> These methods must be called at the start + end of you main method
// NOTES:
//   - Unless 'DEE_FINALIZE_FLAGS_NOCALLBACK' is set,
//     'Dee_Finalize' will invoke callbacks registered by 'Dee_AtExit'
//   - These functions are not recursive (recursion should be implemented by the user)
//     (unless 'DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION' is enabled, which it is by default)
//   - Functions marked with 'DEE_A_EXEC' / 'DEE_A_REQ_INIT' may only be called after
//     this function returned zero (obviously excluding the ones here...)
//   - After 'Dee_Finalize' has been called, anything marked with
//     'DEE_A_EXEC' / 'DEE_A_REQ_INIT' can once again no longer be called.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_NOEXCEPT(-1) int) Dee_InitializeEx(DEE_A_IN Dee_uint32_t flags);
DEE_FUNC_DECL(DEE_A_EXEC void) Dee_FinalizeEx(DEE_A_IN Dee_uint32_t flags);

#define Dee_Initialize() Dee_InitializeEx(DEE_INITIALIZE_FLAGS_NONE)
#define Dee_Finalize()   Dee_FinalizeEx(DEE_FINALIZE_FLAGS_NONE)

//////////////////////////////////////////////////////////////////////////
// Can be called to try and free some memory used by object pools
// >> Returns non-zero if something got cleaned
DEE_FUNC_DECL(Dee_size_t) Dee_ClearInstancePools(void);

//////////////////////////////////////////////////////////////////////////
// Invoke various callbacks to free unused memory (called when a malloc fails)
// >> Returns non-zero if something got cleaned, with a higher return value indicate more.
//    Note though, that the return value does NOT indicate amount of bytes freed!
DEE_FUNC_DECL(DEE_A_EXEC Dee_size_t) Dee_CollectMemory(void);

//////////////////////////////////////////////////////////////////////////
// The main function that does everything
// NOTE: Even when calling this, you still have to call
//       'Dee_Initialize' and 'Dee_Finalize' from your main function
// NOTE: You will also have to handle any errors if -1 is returned.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) Dee_Main(
 DEE_A_IN int argc, DEE_A_IN_R(argc) char **argv, DEE_A_OUT_OPT int *retval);

//////////////////////////////////////////////////////////////////////////
// Returns the 'DEE_VERSION_FULL' value that deemon was compiled with
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint32_t) Dee_GetVersion(void);

//////////////////////////////////////////////////////////////////////////
// Returns the string that is displayed by the '--version' commandline switch
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_Z char const *) Dee_GetVersionString(void);

//////////////////////////////////////////////////////////////////////////
// Returns a '\0' separated list of enabled features, ending with '\0\0'
// Names are equivalent to the macro names in <deemon/version.h> without the
// leading 'DEE_CONFIG_*' and the contained '*HAVE_*', as well as being lowercase
// e.g.: If docstrings are enabled ('DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS == 1') then
//       the string "runtime_docstrings" is one of the strings in the feature list
//       and 'Dee_HasFeatures("runtime_docstrings")' will return non-zero
// HINTS:
//   - For a fast and easy way of viewing all available features, execute: 'deemon --features'
//   - The returned list of strings is always alphabetically sorted
// NOTES:
//   - If you simply want to check if a given feature exists, call 'Dee_HasFeatures'
//   - 'Dee_HasWildFeatures' works the same as 'Dee_HasFeatures', but accepts wildcards
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_Z char const *) Dee_GetFeatures(void);
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) Dee_HasFeatures(DEE_A_IN_Z char const *name);
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) Dee_HasWildFeatures(DEE_A_IN_Z char const *wildname);


//////////////////////////////////////////////////////////////////////////
// Get / Set the '__builtin_argv' (aka. 'sys.argv') object
// NOTES:
//   - Returns NULL if no argv variable was set
//   - Must be a list
//   - These functions are thread-safe.
//   - This value is automatically set by 'Dee_Main'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeListObject) *) Dee_GetArgv(void);
DEE_FUNC_DECL(DEE_A_EXEC void) Dee_SetArgv(DEE_A_INOUT_OBJECT_OPT(DeeListObject) *ob);

//////////////////////////////////////////////////////////////////////////
// Get/Set the globally used deemon home folder, that is the root
// directory used to search for dexes, as well as the standard library.
// >> Calling 'Dee_GetHome' will lazily look for an enviroment variable '$DEEMON_HOME',
//    before defaulting to '/usr/lib/deemon' (on linux), or the hosting exe's directory+'/lib' (on windows)
// NOTE: 'Dee_SetHome' can be called to change the home folder at any time,
//       though note that the paths of some objects (such as lexers) will not be updated.
// NOTE: The home path is used by the "S" and "V" dex search modes.
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) Dee_GetHome(void);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) Dee_SetHome(DEE_A_IN_OBJECT(DeeAnyStringObject) const *home);



struct DeeCompilerConfig;
struct DeeFunctionObject;

//////////////////////////////////////////////////////////////////////////
// A few functions further simplifying the compilation process
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_FileStream(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_FileStreamEx(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_Filename(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_FilenameEx(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_FilenameObject(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_FilenameObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_String(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_StringObject(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_StringWithLength(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_StringEx(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_StringObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *) DeeCompile_StringWithLengthEx(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,3));

//////////////////////////////////////////////////////////////////////////
// A few functions that can be used to directly compile and execute code
// WARNING: If the code compiles into a yield function, it will only be executed once you start iterating it.
// In the end, these function are just slightly optimized aliases for 'DeeObject_Call(DeeCompile_*(...),Dee_EmptyTuple)'
//  -> It really doesn't get much simpler than this!
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStream(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStreamEx(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_Filename(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameEx(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObject(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_String(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObject(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLength(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringEx(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLengthEx(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,3));

//////////////////////////////////////////////////////////////////////////
// A few functions that can be used to directly compile and execute code
// WARNING: If the code compiles into a yield function, it will only be executed once you start iterating it.
// In the end, these function are just slightly optimized aliases for 'DeeObject_Call(DeeCompile_*(...),args)'
//  -> It really doesn't get much simpler than this!
// NOTE: The given arguments are available as varargs (aka. '...') in the root scope
// >> DeeRun_StringArgsf("print ...;","ddd",10,20,30); // Prints '(10,20,30)'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStreamArgs(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStreamExArgs(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameArgs(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameExArgs(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObjectArgs(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObjectExArgs(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringArgs(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObjectArgs(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLengthArgs(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringExArgs(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObjectExArgs(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLengthExArgs(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3,5));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStreamArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FileStreamExArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameExArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_FilenameObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringArgsf(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLengthArgsf(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringExArgsf(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_StringWithLengthExArgsf(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3,5));

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFileStreamArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFileStreamExArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFilenameArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFilenameExArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFilenameObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VFilenameObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringArgsf(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringWithLengthArgsf(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringExArgsf(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRun_VStringWithLengthExArgsf(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3,5));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Load compiled deemon files
// >> Throws a ValueError is the given file is not a compiled deemon file
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoadCompiled_FileStream(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoadCompiled_Filename(DEE_A_IN_Z char const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoadCompiled_FilenameObject(DEE_A_IN_OBJECT(DeeStringObject) const *filename) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Saves compiled deemon files
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSaveCompiled_FileStream(DEE_A_INOUT DeeObject *compiled_object, DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSaveCompiled_Filename(DEE_A_INOUT DeeObject *compiled_object, DEE_A_IN_Z char const *filename) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSaveCompiled_FilenameObject(DEE_A_INOUT DeeObject *compiled_object, DEE_A_IN_OBJECT(DeeStringObject) const *filename) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Load a file (can be a compile deemon file, or a deemon source file)
// Returned object can then be executed with 'DeeObject_Call(return,Dee_EmptyTuple)'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_FileStream(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_FileStreamEx(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_Filename(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_FilenameEx(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_FilenameObject(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeLoad_FilenameObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Execute a file (can be a compile deemon file, or a deemon source file)
// >> This is the very tip of the pyramid. Nothing can be simpler than this!
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStream(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStreamEx(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_Filename(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameEx(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObject(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObjectEx(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStreamArgs(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStreamExArgs(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameArgs(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameExArgs(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObjectArgs(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObjectExArgs(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,4));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStreamArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FileStreamExArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameExArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_FilenameObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4));

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFileStreamArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFileStreamExArgsf(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFilenameArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFilenameExArgsf(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFilenameObjectArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeExec_VFilenameObjectExArgsf(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4));
#endif

#if defined(GUARD_DEEMON_TUPLE_H) && !defined(__INTELLISENSE__)
#define /*DEE_A_EXEC*/ DeeRun_FileStream(filestream,config)                             DeeRun_FileStreamArgs(filestream,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_FileStreamEx(filestream,disp_filename,config)             DeeRun_FileStreamExArgs(filestream,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_Filename(filename,config)                                 DeeRun_FilenameArgs(filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_FilenameEx(filename,disp_filename,config)                 DeeRun_FilenameExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_FilenameObject(filename,config)                           DeeRun_FilenameObjectArgs(filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_FilenameObjectEx(filename,disp_filename,config)           DeeRun_FilenameObjectExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_String(code,config)                                       DeeRun_StringArgs(code,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_StringObject(code,config)                                 DeeRun_StringObjectArgs(code,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_StringWithLength(code,code_length,config)                 DeeRun_StringWithLengthArgs(code,code_length,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_StringEx(code,disp_filename,config)                       DeeRun_StringExArgs(code,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_StringObjectEx(code,disp_filename,config)                 DeeRun_StringObjectExArgs(code,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeRun_StringWithLengthEx(code,code_length,disp_filename,config) DeeRun_StringWithLengthExArgs(code,code_length,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_FileStream(filestream,config)                            DeeExec_FileStreamArgs(filestream,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_FileStreamEx(filestream,disp_filename,config)            DeeExec_FileStreamExArgs(filestream,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_Filename(filename,config)                                DeeExec_FilenameArgs(filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_FilenameEx(filename,disp_filename,config)                DeeExec_FilenameExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_FilenameObject(filename,config)                          DeeExec_FilenameObjectArgs(filename,config,Dee_EmptyTuple)
#define /*DEE_A_EXEC*/ DeeExec_FilenameObjectEx(filename,disp_filename,config)          DeeExec_FilenameObjectExArgs(filename,disp_filename,config,Dee_EmptyTuple)
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEEMONRUN_H */
