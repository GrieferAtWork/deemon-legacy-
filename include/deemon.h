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
#ifndef GUARD_DEEMON_H
#define GUARD_DEEMON_H 1

//////////////////////////////////////////////////////////////////////////
//
// Used identifier namespaces:
// Deemon: Dee*, DEE_*, _Dee*, _DEE_*
// TPP:    TPP*, _TPP*, KWD_* (only used if <deemon/compiler/lexer.h> is included)
//
// The C API strictly follows these namespaces and unless you define
// 'DEE_LIMITED_API' (which you really never should), nothing but
// identifiers in these namespaces will be used.
//
//////////////////////////////////////////////////////////////////////////
//
// General error return value consensus:
//  - T  *xxx(); // Returns NULL & throws an error on failure
//  - T  *xxx(); // Returns NULL on failure (explicitly stated)
//  - int xxx(); // Returns <0 & throws an error on failure
//  - int xxx(); // Returns  0 on success (indicate true)
//  - int xxx(); // Returns >0 on failure (indicate false)
// Exception: noexcept bool functions (this is just an example...)
//  - int Dee*_Check(); // Returns 0 for false (dosn't set an error)
//  - int Dee*_Check(); // Returns 1 for true
// But you can always look at the exception return annotation:
//  - DEE_A_RET_EXCEPT(x)              // Returns 'x' and throws an error on failure
//  - DEE_A_RET_EXCEPT_FAIL(x,y)       // Returns 'x' and throws an error on failure; Returns 'y' on failure without an error
//  - DEE_A_RET_NOEXCEPT(x)            // Returns 'x' on failure (never sets an error)
//  - DEE_A_RET_OBJECT_NOEXCEPT_REF(T) // Returns a new reference to an object of T; Returns NULL on failure (never sets an error)
//  - DEE_A_RET_NOEXCEPT_REF           // Just like 'DEE_A_RET_OBJECT_NOEXCEPT_REF', but the object typing is unspecified
//  - DEE_A_RET_OBJECT_EXCEPT_REF(T)   // Returns a new reference to an object of T; Returns NULL on failure and throws an error
//  - DEE_A_RET_EXCEPT_REF             // Just like 'DEE_A_RET_OBJECT_EXCEPT_REF', but the object typing is unspecified
//  - DEE_A_RET_OBJECT_EXCEPT(T)       // Returns an object of T; Returns NULL on failure and throws an error
// 
//////////////////////////////////////////////////////////////////////////
//
// Naming conventions in API functions:
// Note, that the existence of any of these names for a given object
// is always optional. But if they exist, these are their functions
//  - Object struct names:      Dee<Name>Object
//  - Non-Object struct names:  Dee<Name>
//  - Object type:              Dee<Name>_Type
//  - Object instance check:    Dee<Name>_Check, Dee<Name>_CheckExact
//  - Initializer:              Dee<Name>_Init
//  - Finalizer:                Dee<Name>_Quit
//  - Allocator:                Dee<Name>_Alloc
//  - Free function:            Dee<Name>_Free
//  - Allocate+Init:            Dee<Name>_New
//  - Finalize+Free:            Dee<Name>_Delete
//  - Utility/Member functions: Dee<Name>_<FunctionName>
//
//////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////
// -- Version History (latest release) --
//////////////////////////////////////////////////////////////////////////
// -- v102/102:0 --
//   Comments:
//     - This update fixes those pesky bugs that always seem to pop
//       up the second I finally decide to roll out a new update.
//     - Removed API exports will continue to be provided until API version 104
//     - Fixed deemon constantly crashing on any x86-64 linux
//     - Added new optimizations allowing for the prediction
//       of the type and value of local variables.
//     - Began work on python-style string formatting
//     - '__builtin_help' will be removed in v103
//
//   New:
//     - Added a way for dex extensions being prevented from loading
//       if they were compiled under a different version of deemon.
//     - Improved the way the dex searches for modules, allowing it to search
//       much more places such as $PATH or the current directory.
//       By default, deemon behaves similar to its host OS, meaning that
//       on windows it will search $PATH by default, but won't do so on linux.
//     - Added some functions to the dex type that can be used to modify
//       the dex search path and search order from usercode.
//       These functions can easily be accessed through a new header lib:<dexmodule>
//     - Dex modules can now be compared against each other and hashed
//       In other words: they can now be used as keys in dicts.
//     - Added a new hidden commandline switch: '--debug-new-hush',
//       that can be used to silence debug new output.
//       Useful, when wanting to prevent log-spam after CTRL+C-ing the app on windows.
//       NOTE: Since I didn't mention it in the previous changelog, there
//             is another switch '--debug-new-breakpoint <id>', that can
//             be used to raise a SIGTRAP on a given allocation id.
//     - When opening a file with 'file.io' or 'file.io.reopen', in addition
//       to the mode, linux-style open permissions can now be specified:
//       >> fp = file.io("foo.txt","w",0644);
//       When not specified, they still default to 0644 (as was the default in 100-101)
//      (set with DEE_FILEIO_DEFAULT_PERMISSIONS)
//       NOTE: On windows, only the 0444 mask has an effect,
//             and can be used to set the read-only flag.
//     - Implemented the optimization for 'print file.io.stdout: ...;' --> 'print ...;'
//     - Added a warning for a missing [[[end]]] marker when formatting a file with -F
//     - Added a new function 'dict.insert_all' that can be used to
//       insert a sequence of key-item pairs into an existing dict.
//     - Added an optimization for turning 'for (none: seq);' --> 'seq...;'
//     - Added a new optimization that prevents usercode from having
//       to create deep copies of tuples only filled with immutable objects.
//     - Compile-time type prediction is now much smarter, knowing about
//       a variety of builtin types and how they behave, this might lead
//       to faster code being generated.
//       NOTE: But we still can't predict the types of local variables.
//     - Added a new function 'set.insert_all' that can be used to
//       insert all elements from a given sequence into a set.
//     - Implemented string.format (aka. python-style string formatting)
//       NOTE: Just like in python, you can implement a function
//             '__format__' to implement special string formating.
//       NOTE: Currently, only 'time' is implementing this, but more types will follow.
//       NOTE: In addition, deemon allows you to not only call __getattr__ and __getitem__
//             on format items (like you also can in python), but even allows calling
//             element, allowing for something like this:
//             >> print "Hello, I'm {me.upper()}. - Nice to meat you.".format({ .me = "g@w" });
//       NOTE: The syntax is not yet finalized, and I won't document its entirety
//             until it is, with features like escaping '{' and '}' still missing,
//             but documentation and tests will be added once its done.
//     - The noreturn attribute of goto statements is now calculated correctly.
//     - Added a new way for the optimizer to predict not only the type, but
//       under certain circumstances even the value of a local variable.
//       NOTE: This optimization only happens during optimization, meaning
//             that you won't be able to detect the type of a local variable
//             with '__builtin_typeof'.
//             >> local x = "foobar";
//             >> print x; // x gets inlined.
//             >> print type x; // The optimizer will be able to deduce the type of x
//       By default, this optimization will only be enabled under -O3 and can
//       manually be enabled/disabled with '-Co-ASSUMPTIONS' / '-Cno-o-ASSUMPTIONS'
//       To prevent the optimizer from being allowed to optimize any given
//       variable in such a way, a new variable modifier 'volatile' was added.
//     - Added a warning for using an inplace operator with a constant on its left-hand-side
//     - Added a new iterator that can be used to enumerate sequences:
//       >> #include <util>
//       >> seq = [10,20,30];
//       >> for (local i,x: util::enumerate(seq)) {
//       >>   print i,x; // prints '0 10' '1 20' and '2 30'
//       >> }
//     - Added two new predefined macros '__DEEMON_API__' and '__DEEMON_REVISION__'
//       that can be used to detect the api and revision number of the deemon compiler.
//       NOTE: If the code is compiled with '-o' or marshaled, the deemon executing
//             it might be different than indicated by these macros.
//     - Added more opcodes that can be prefixed with OP_PREFIX to extend them,
//       essentially allowing for 32-bit longjumps, 32-bit variable/const/ref/arg ids
//       and much more, resulting in the (theoretical) usercode that's 2**16 times
//       larger than the previous maximum.
//     - Implemented an optimization for removing no-op opcodes from generated code.
//       NOTE: no-ops are generated in various places as placeholder bytes.
//     - Added a constant 'MARSHAL_VERSION' to 'file'
//       that evaluates to the current marshal version.
//     - Began work on a script that can generate the html
//       documentation for deemon (using the '_docgen' dex)
//       Now if only I wasn't such a noob when it comes to html...
//     - Added a new attribute '[[instance_bound]]', '[[instance_bound("copy")]]', '[[instance_bound("move")]]',
//       that can be used to force variables to be bound to an explicit instance.
//       When a variable is declared with this attribute, all store operations
//       except for the first one immediately after declaration are compiled
//       as copy/move-assign operations.
//       NOTE: '[[instance_bound]]' behaves the same as '[[instance_bound("copy")]]'
//       Named function declarations are implicitly declared with '[[instance_bound("move")]]'
//       NOTE: When an instance-bound variable is deleted, a new instance can be assigned
//     - Added two new function 'dexmodule.nameof' and 'dexmodule.fileof', that can
//       be used to query the name and filename of a given dexmodule from usercode.
//     - Added a new dex '_marshal' that can be used to access some of
//       the more in-depth features of the marshal from deemon's core.
//     - Added a new dex '_system' that replaces the now
//       deprecated core functionality from '<deemon/system.h>'
//     - Added a new command-line switch '--keep-comments' that can be used
//       to instruct deemon to keep comment tokens during preprocessing.
//       Use as 'deemon --keep-comment -E <file>' to preprocess <file> and keep all comments intact.
//     - Various user-code attributes, such as [[name(...)]] now accept 'none' as
//       an argument, which can be used to delete the associated attribute.
//     - Added a predicate overload to 'list.remove'
//     - Added more documentation for the members of 'list'
//
//   Changes:
//     - On linux, deemon will now look for dex extensions
//       in '/usr/lib/deemon/dex.%d' before checking '/usr/lib/deemon/dex'.
//       When existing, 'dex.%d' describe folders containing version-specific
//       dex extensions. (for this release that is 'dex.102')
//       s.a.: 'DeeDex_SetSearchOrder', lib:<dexmodule>
//     - Redesigned optimization flags for the default compilation mode.
//       -Omax/-O3 still provides all optimizations, but the default mode -Odef
//       now includes much less, making compiling code with it much faster
//       by basically only enabling constant optimizations to emit warnings
//       for runtime-errors caused by constants.
//     - Proxy functions that map to their type-explicit versions are now always
//       implemented as macros and no longer guaranty single-evaluation of their arguments.
//       This change will make it much easier for the compiler, and few
//       previously existing c-code actually depended on single-evaluation.
//       e.g.: 'DeeObject_Add(a,b)' 'a' and 'b' are no longer guarantied to be evaluated
//             once, so store them in a local variable if you must ensure single-evaluation.
//       NOTE: This only affects the C-Api
//     - Renamed 'DeeList_SetRangeIteartor' to 'DeeList_SetRangeIterator' (typo...)
//     - Renamed the c-functions for adding class properties to be more fitting.
//       (DeeClass_GetGetSet??? How did I ever allow this in the first place?)
//       Instead of being called GetSet, they're now called Properties:
//       'DeeClass_GetGetSet' --> 'DeeClass_GetProperty'.
//       Deprecated support is provided until v102 if you
//       '#define DEE_DEPRECATED_API 101' before including the <deemon/class.h>
//     - Support for T-style API functions is now more wide-spread
//     - Operator callbacks needing a specific type on their right-hand-side
//       should now call 'DeeObject_GetInstance' instead of DeeError_TypeError_CheckType.
//       NOTE: Those calling 'DeeError_TypeError_CheckTypeExact' should NOT be changed.
//       >> Using this function, you can add better support for super objects
//     - 'DeeFileWriter_New' is now implemented by a macro calling 'DeeFileWriter_NewWithSizeHint',
//       which can be used to specify an optional size hint for the writer.
//     - Renamed 'DeeDict_NewFromSeq' to 'DeeDict_NewFromSequence'
//     - Changed the way 'DeeObject_GetLValue' works, making it return an int
//       and taking a 'void **' for the result. (This is required because of the
//       fact that as of 101/101:1 lvalues are no longer required to be non-NULL)
//       NOTE: Backwards compatibility isn't provided in the headers, as
//             'DeeObject_GetLValue' isn't really something that was called a
//             lot, but internally, binary backwards compatibility is provided.
//     - Renamed 'DeeSet_InsertAllFromSeq' to 'DeeSet_InsertSequence'
//     - Renamed 'DeeSet_InsertAllFromIterator' to 'DeeSet_InsertIterator'
//     - Rewritten the way tests are executed, allowing for more precise
//       execution of test by executing each in its own process.
//     - Some of the c headers have been marked as pending for removal/wiping.
//       Function/objects implemented by those headers will eventually be
//       moved into individual dex extensions, thus eventually making the
//       runtime smaller and more efficient.
//     - Re-written the docstring parser inside of a new dex module '_docgen'
//       All docstring queries will now leave the runtime and be executed by a dex.
//       As a consequence, '__builtin_help' is now deprecated, and the functions
//       from '<deemon/optional/object_doc.h>' have been replaced with macros.
//       The function is now provided through a header 'lib:<__builtin_help.dee>',
//       that will implement it as a macro and disable the warning about using it.
//     - Changed the way write-permission is granted to all members in developer builds.
//     - Refactored all hidden members of all objects, updating them and changing
//       their naming conventions to not show up in the documentation.
//     - The functions from <deemon/system.h> have been replaced with macros.
//     - When deemon fails to find a dex module, a new error type 'Error.RuntimeError.DexError'
//       will be thrown, where deemon v102:1 used to throw an Error.AttributeError.
//     - Changed the return values of 'DeeError_Handled' to mirror 'DeeError_Handle'
//     - Line numbers are now universally signed 64-bit integers.
//     - Changed the way filenames are generated in debug output.
//       Now using the same technique as line-numbers, filenames 
//
//   Fixes:
//     - Fixed a reference/memory leak when using the '!!' version of 'operator bool'.
//     - Fixed util::range for generic structured objects
//     - Fixed unsigned integer check macros in <deemon/integer.h>
//     - Fixed a bug in 'DeeTuple_UnpackInfo' that caused an incorrect invalid format error
//     - Fixed the this-argument being ignored when compiling an attribute-style operator
//       call where the operator doesn't have a dedicated AST, or can only be deduced at runtime.
//     - Fixed a crash under certain circumstances, when optimizing away an expand
//       expression on an empty sequence at the end of another sequence (e.g.: '[10,20,[]...]')
//     - Fixed a crash during peephole optimization (caused by incorrect protected address order)
//       - Assertions were added to detect such problems in the future
//     - Fixed a bug that caused incorrect compiler warnings related to the loop variable,
//       when a foreach loop over an empty sequences got optimized into a dead statement.
//     - Fixed a bug where labels where sometimes deleted when inlining statements during optimization
//     - Went through some TODOs and actually did them
//     - Fixed some typos here and there
//     - Added utility functions to 'code.debuginfo'
//     - Added access to the 'code.debuginfo' through 'code.debuginfo' (forgot that last time)
//     - Fixed a minor design problem that might have caused data to be lost when
//       formatting a file. (NOTE: This did NOT affect 'deemon -F', only making it a minor bug)
//     - Fixed a bug that caused a non-existing error to be
//       detected when not providing a predicate to 'list.unique'
//     - Fixed a rare scenario where list.contains() may have incorrectly
//       assumed an error when an explicit predicate returned false.
//     - Fixed a reference leak when using 'DeeObject_TGetIndex' on a list
//     - Fixed the file.joined constructor incorrectly assuming an error if less than two files are given
//     - Fixed a reference leak in file.joined.tp_io_seek
//     - Fixed missing references in 'DeeSequence_TLocate' and
//       'DeeSequence_TRLocate' when the given sequence is a tuple
//     - Fixed a potential SEGFAULT in DeeTuple_RFindPred
//     - Fixed a potential SEGFAULT in DeeTuple_RFind
//     - Fixed a potential SEGFAULT in DeeTuple_FindPred
//     - Fixed a potential SEGFAULT in DeeTuple_Find
//     - Fixed DeeHash_Memory and DeeHash_String not being exported
//     - Fixed 'DeeProcess_SetCwd' / 'process.cwd' internally
//       expecting 'string' instead of any kind of string
//     - The 'thread.name' set-property now accepts any kind of string (casting it to 'string')
//     - Finished implementing 'DeeXAst_InitCopy' (aka. 'xast.tp_copy_ctor')
//     - Finished implementing 'DeeSAst_Equals' (used by alias declarations)
//     - Finished implementing 'DeeXAst_Equals' (used by alias declarations)
//     - Fixed tp_pos and tp_neg for negative integral types smaller than int and bool
//     - Fixed incorrect argument requirements in '__builtin_i32round',
//       '__builtin_i64round', '__builtin_i32rint' and '__builtin_i64rint'
//     - Fixed '__builtin_strcasecmp' and '__builtin_strcasestr' being swapped around
//     - Fixed 'DeeFSWin32SID_Type' missing in the list of builtin objects.
//     - Fixed deemon constantly crashing on any x86-64 linux
//     - Fixed a missing reference when explicitly calling a
//       [[thiscall]] function with an incorrect amount of arguments.
//       (This caused a dead-object assertion later down the line)
//     - Fixed the annotation for 'Dee_InitializeEx' to 'DEE_A_RET_NOEXCEPT'
//       (obviously it can't throw an exception if the library couldn't be initialized)
//     - Fixed the potential of invalid error detection when an lvalue was NULL
//     - Fixed string.(wide|utf16|utf32).index and string.(wide|utf16|utf32).rindex calling
//       the wrong function (NOTE: string.utf8 wasn't broken though; he's the one that was always called...)
//     - Fixed a bug that caused invalid bytecode to be generated when returning
//       from an exception handler that is handling more than one exception.
//     - Fixed various bugs within the implementation of statement attributes,
//       something leading to incorrect attributes being deduces, most notably
//       when combining exceptions with loop break/continue.
//       Test cases were added and the bugs have been fixed.
//     - Fixed 'DeeFunction_Type' missing in the list of builtin objects.
//     - Fixed 'DEE_TYPE_FLAG_MUST_COPY' missing from builtin sequence types.
//       Instead of leaving it away, 'DeeType_MustDeepCopy' should be called
//       before checking for the 'DEE_TYPE_FLAG_MUST_COPY' (which is now present in list, tuple, dict, ...)
//       NOTE: 'cell' already had the flag set, potentially allowing usercode
//             to modify a constant that would have been contained within.
//     - Fixed missing documentation of the binary operators / and % in LANGUAGE.txt
//     - Fixed another bug that lead to an incorrect address order during peephole optimization
//     - Fixed an incorrect assertion when using __builtin_expect
//     - Fixed deemon -in=data performing a seek operations on the input
//       stream if a bash-execution header is present.
//       With this out of the way, you can now stream a raw compiled deemon file via stdin:
//       >>$ deemon -c -o compiled_sourcefile.out sourcefile.dee
//       >>$ deemon -in=data /dev/stdin < compiled_sourcefile.out
//     - Fixed the list of class methods in 'socket' not being properly terminated
//     - Fixed a design-flaw with yield-function iterators.
//       Were neither thread-safe, nor gc-objects, even though they can reference themselves.
//       s.a.: 'tests/test_yield_iter_selfref.dee'
//     - Fixed a bug that caused peephole optimizations to break code in some rare
//       situations, where protection of the last protected opcode was sometimes ignored.
//     - Fixed 'util::seq::contains' being mapped to 'util::seq::count'
//     - Fixed access to floating-point constants not being constexpr
//     - Fixed some of the special iterators from <deemon/iterator.h> not implementing
//       the standard sequence functions, even though they're meant to double as iterables.
//     - Fixed a crash when trying to access a member constant with write-access
//       (The standard doesn't mention this, meaning it's supposed to be weak undefined behavior)
//     - Fixed '"%s" % ((char *)x)' not working in usercode
//     - Fixed '__has_feature(__expr_statement__)' in lib:<cxx/assert.h> to '__has_feature(__expr_statements__)'
//     - Fixed the demonstration header lib:<cxx/gl/gl.h>
//     - Fixed a crash when enumerating the attributes of a structured object
//     - Fixed a crash when enumerating the attributes of an lvalue
//     - Fixed a bug that caused problems with pointer-to-character types in tp_members
//     - Fixed a bug that caused a crash when counting variable usage on an unimplemented function
//     - Fixed a bug that caused an invalid value to be returned by 'socket.address_family' and 'sockaddr.address_family'
//     - Fixed forward-declared functions not working, as well as
//       somewhat hardening the rules when it comes to declaring them:
//       Forward-declared functions may not feature an argument list, as doing so
//       implicitly requires the function to feature a scope (to support argument defaults),
//       thereby also requiring the function to receive an implicitly generated, empty body.
//       >> function foo;   //< Forward declaration ('foo' should be implemented later, but calling it now returns 'none')
//       >> function foo(); //< Not a forward declaration. This will generate a warning, through
//       A test case has been added to ensure forward-declared functions always working in the future
//     - Fixed an incorrect warning when referencing a module with 'forward'
//     - Fixed a minor bug that prevent a relaxed-comma from being skipped:
//       >> list({ 10,20,30, },);
//                            ^ here
//       NOTE: A test case was added for this
//     - Fixed a possible memory leak when reading code debuginfo from unexpected/corrupted data
//     - Fixed Error.ThreadCrash not being a builtin object
//     - Fixed an assertion that failed whenever a type without marshal
//       support was being marshaled. Instead, an error is is thrown now.
//     - Fixed a design-flaw with 'DeeError_Handle' by defining the corner-case
//       where more errors were available after the first one was caught.
//       This is something that's really tricky to handle. So since it's something
//       not really done often, 'DeeError_Handle()' and 'DeeError_Handled()' will
//       now look at the first exception, and if they decide to handle it discard
//       all other exceptions.
//       >> To extend on this behavior, two new function were added:
//         - DeeError_PopOccurredOne() - Behaves similar to how 'DeeError_PopOccurred' used to, but only removes one exception
//         - DeeError_HandledOne() - Behaves like 'DeeError_Handled' used to
//         - DeeError_HandleOne() - Behaves similar to how 'DeeError_Handle' used to,
//           but has a special return value indicating that more exceptions are unhandled.
//       NOTE: The old functions are not deprecated and will not be removed.
//             Though the new ones allow more control and keep additional exceptions intact.
//     
//   Optimized:
//     - Generating an iterator with util::range is now faster
//     - Added prediction optimization to list member functions and everything else in 'list.c'
//     - Added prediction optimization to tuple member functions and everything else in 'tuple.c'
//     - Added prediction optimization to the functions in 'generic_vtable.c'
//     - Compile-time type prediction is now much smarter
//     - Compile-time type prediction is now much smarter
//     - Removed some static variables that were never used
//     - Added new optimizations for assumptions of variable values/types
//     - '__builtin_abs' is now faster, using a switch to enumerate most likely
//       argument types, instead of calling DeeObject_InstanceOf a million times.
//
//   Removed:
//     - Removed write access to the callbacks of a property
//      (It's supposed to be a singleton; writing isn't thread-safe)
//     - Removed write access to the members of a code debug object
//      (It's supposed to be a singleton; writing isn't thread-safe)
//     - Removed DEE_WITHOUT_DEX (didn't really in the first place)
//       The dex is too important for deemon and its future, making
//       it something you shouldn't be able to deactivate.
//     - Removed tp_copy_assign from list.iterator (can't be made thread-safe)
//     - Removed 'DeeTuple_SetElem' (being a singleton, you
//       should only be allowed to do this via the unsafe API)
//     - Removed 'DeeTuple_GetIndex' (Either use 'DeeTuple_Get' or 'DeeTuple_At')
//     - Removed tuple.tp_copy_assign
//      (No idea how drunk I was when I implemented that.
//       It's a f*ing singleton! And I don't even drink...)
//     - Removed assigning dicts/copies to dict proxies
//     - Removed 'DeeStructured_Copy' (Meant as a fast-pass
//       for DeeObject_Copy, but was never used)
//     - Various API functions have been replaced with macro versions
//     - Public access to all those undocumented members of objects.
//      (You know: stuff like 'tp_name' and the likes)
//       Access to everything you might need is still possible,
//       but the internal member fields are now hidden
//      (that is: only available in debug builds, not showing up in docs and staring with "__")
//
//   Known bugs:
//     - Very, very few, minor parts of the runtime are still not threadsafe (process)
//     - 'DeeMarshal_IsCompiledFile' still contains a seek that can
//       cause problems if /dev/stdin is used as the input file
//       without specifying '-in=code' to skip that check
//     - No way to specify custom arguments to super constructors in user classes
//       Currently they just get called with the same arguments as your own constructor.
//       The problem is that deemon works similar to c++, where the super constructor
//       absolutely __must__ be invoked before any other usercode can be allowed to run.
//
//////////////////////////////////////////////////////////////////////////


#ifdef DEE_NO_API
#undef DEE_NO_API
#ifndef DEE_INCLUDE_NO_API
#define DEE_INCLUDE_NO_API
#endif
#endif

#ifdef DEE_NO_IO
#undef DEE_NO_IO
#ifndef DEE_INCLUDE_NO_IO
#define DEE_INCLUDE_NO_IO
#endif
#endif

#ifdef DEE_NO_MP
#undef DEE_NO_MP
#ifndef DEE_INCLUDE_NO_MP
#define DEE_INCLUDE_NO_MP
#endif
#endif

#ifndef DEE_WITH_COMPILER
#ifndef DEE_INCLUDE_NO_COMPILER
#define DEE_INCLUDE_NO_COMPILER
#endif
#else
#undef DEE_WITH_COMPILER
#endif

#ifndef DEE_WITH_RUNTIME
#ifndef DEE_INCLUDE_NO_RUNTIME
#define DEE_INCLUDE_NO_RUNTIME
#endif
#else
#undef DEE_WITH_RUNTIME
#endif

#if defined(DEE_NO_CXX) || !defined(__cplusplus)
#undef DEE_NO_CXX
#ifndef DEE_INCLUDE_NO_CXX
#define DEE_INCLUDE_NO_CXX
#endif
#endif

#include <deemon/__conf.inl>
#include <deemon/deemonrun.h>

#ifndef DEE_INCLUDE_NO_API
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/cfunction.h>
#include <deemon/class.h>
#include <deemon/dex.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#ifndef DEE_INCLUDE_NO_IO
#include <deemon/file.h>
#endif
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/fs_api.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/uuid.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/math.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/random.h>
#include <deemon/runtime_flags.h>
#include <deemon/sequence.h>
#include <deemon/set.h>
#ifndef DEE_INCLUDE_NO_IO
#include <deemon/socket.h>
#endif
#include <deemon/string.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/time.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/value.h>
#include <deemon/version.h>
#include <deemon/weakref.h>
#endif
#ifndef DEE_INCLUDE_NO_COMPILER
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#endif
#ifndef DEE_INCLUDE_NO_MP
#include <deemon/mp/mutex.h>
#ifndef DEE_INCLUDE_NO_IO
#include <deemon/mp/pipe.h>
#endif
#include <deemon/mp/process.h>
#include <deemon/mp/thread.h>
#include <deemon/mp/tls.h>
#endif
#ifndef DEE_INCLUDE_NO_RUNTIME
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime/function.h>
#endif
#ifndef DEE_INCLUDE_NO_CXX
#include <deemon/cxx/dict.h>
#include <deemon/cxx/file.h>
#include <deemon/cxx/float.h>
#include <deemon/cxx/function.h>
#include <deemon/cxx/integer.h>
#include <deemon/cxx/list.h>
#include <deemon/cxx/none.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/string.h>
#include <deemon/cxx/structured.h>
#include <deemon/cxx/time.h>
#include <deemon/cxx/tuple.h>
#include <deemon/cxx/type.h>
#endif

#undef DEE_INCLUDE_NO_API
#undef DEE_INCLUDE_NO_COMPILER
#undef DEE_INCLUDE_NO_CXX
#undef DEE_INCLUDE_NO_IO
#undef DEE_INCLUDE_NO_MP
#undef DEE_INCLUDE_NO_RUNTIME

#endif /* !GUARD_DEEMON_H */
