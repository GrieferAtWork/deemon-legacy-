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

// TODO: lib:<langinfo.h>

// TODO: User operators for '&' and '*' (ref/deref)

// TODO: tpp: Support for files encoded in something other than utf-8

// TODO: Linux: use fstat to figure out the perfect buffersize for reading a device file
//              >> Currently we are simply trying to read everything with a fix-length buffer of 4096,
//                 which should be enough for every case, but still is just that: a static number.
//              >> Using fstat, we could figure out a size-hint that would be given by the device file itself.
//              NOTE: That hint will usually be 4096, tough...

// TODO: Fix the multi-evaluation problem when declaring typed variables:
// >> class my_class: object {
// >>    this() { print "Hello"; }
// >> } insta,instb;
// >> // Asts are generated like this:
// >> // And yes: this will generate two different classes
// >> insta = (my_class = class [[name("my_class")]] {
// >>   this { print "Hello"; }
// >> })(),instb = (my_class = class [[name("my_class")]] {
// >>   this { print "Hello"; }
// >> })();

// TODO: Optimize the api by marking more types with 'DEE_TYPE_FLAG_NO_SUBCLASS',
//       thus allowing check callbacks to be turned into CheckExact.


// TODO: Deemon is the perfect candidate for 'Escape analysis'
//       >> https://en.wikipedia.org/wiki/Escape_analysis
//       Using this, alongside the current implementation of variable assumptions,
//       we could really optimize code like this:
//       >> function cycle(n) {
//       >>   // v 'i' will never leave this function:
//       >>   //   >> We can predict its type, and thereby its tp_str operator
//       >>   //      That was we know that it will never leave this function.
//       >>   for (local i = 0; i < n; ++i) {
//       >>     print i;
//       >>   }
//       >> }
//       NOTE: Though firstly, this will require the possibility of
//             declaring variables that will be allocated on the stack.
//       >> function foo() {
//       >>   // At runtime, 'i' is allocated on the stack.
//       >>   [[alloc("stack")]] local i;
//       >>   for (i = 0; i < 10; ++i) print i;
//       >>   // v This is where the troubles lie...
//       >>   //   - At compile-time, we would have to detect that here
//       >>   //     we need to create a heap-copy of 'i' to be returned.
//       >>   //   - But what to do when we call some other function with 'i'?
//       >>   //     Obviously we should create a copy them!
//       >>   //     We should just pass an lvalue to 'i'. OK. Great!
//       >>   //     But what if that other function throws something somehow referencing that lvalue?
//       >>   //     >> function trw(x) { throw list { 10,x,20 }; }
//       >>   //     >> function foo() {
//       >>   //     >>   [[alloc("stack")]] local i = 42;
//       >>   //     >>   trw(i);
//       >>   //     >> }
//       >>   //     By the time 'foo' returns with the exception,
//       >>   //     it will be pointing to a dangling object.
//       >>   //     -> OK: In that case, LValues are supposed to handle any
//       >>   //            kind of invalid error at runtime, something they
//       >>   //            ~mostly~ are capable of.
//       >>   //            But this still feels like something really dangrous...
//       >>   return i;
//       >> }

// TODO: Optimizations:
// Remove default parameter from explicit function call:
// 'file.io(fname,"r")' --> 'file.io(fname)' (Because "r" is the default mode)
// --> Reduce binary size
// --> Can only be added for intrinsic / predictable function calls

// -O4 optimizations:
//   >> local x = file.io(f).read();
//   >> local pat = get_pattern();
//   >> parse(for (local line: x.splitlines()) if (line.lower().wmatch(pat.lower())) line);
// Optimized:
//   >> local x = file.io(f).read();
//   >> local pat = get_pattern();
//   >> {
//   >> 	__stack local __hidden_x = x.splitlines();
//   >> 	__stack local __hidden_pat = pat.lower();
//   >> 	parse(for (local line: __hidden_x) if (line.lower().wmatch(__hidden_pat)) line);
//   >> }


// TODO: Optimization:
//   >> 'a ? true : false'  --> '!!a'
//   >> 'a ? false : false' --> '!(!!a)'
//   >> 'a ? : false'       --> '!!a'
//   >> 'a ? b : false'     --> 'a && b'
//   >> 'a ? true : b'      --> 'a || b'
//   >> 'a ? a : false'     --> '!!a' // Only if 'a' has no side-effects

#ifdef _MSC_VER
#pragma warning(disable: 4514) // Unreferenced inline function removed
#endif

#include <deemon/error.h>
#include <deemon/deemonrun.h>

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDLIB_H
#include <stdlib.h> /*< EXIT_FAILURE. */
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

int main(int argc, char *argv[]) {

 int result;
 if DEE_UNLIKELY(Dee_Initialize() != 0) return EXIT_FAILURE;

 if DEE_UNLIKELY(Dee_Main(argc,argv,&result) != 0) {
  while (!DeeError_Print((char const *)0,1));
  result = EXIT_FAILURE;
 }

 Dee_Finalize();
 return result;
}

