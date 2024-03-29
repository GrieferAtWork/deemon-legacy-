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

1. If you want to learn how deemon works, check out
   'LANGUAGE.txt' for the syntax specification and
   'tut/*' for some code examples.

2. The remainder of this file contains some things
   concerning the language, which I didn't know where else to put.

- RESERVED IDENTIFERS:
  Keywords beginning with 2 underscores (e.g.: '__foobar') are
  reserved for internal usage by the language. But note, that
  unlike c/c++, deemon does not reserve underscope-uppercase. We
  only reserve double underscore at the beginning of a keyword.

  Other, new keywords not beginning with two underscores will always
  require a specific header file which, using a #define assigns a
  regular name to it. For example: I might add 'static_assert'. But 'static_assert'
  will never be a reserved identifer. Instead the compiler will look for
  '__static_assert' and there will be a header in the standard library
  containing a macro '#define static_assert __static_assert'.
  So you won't run into name collision until you include that specific header.
  NOTE: This is a bad example, since 'static_assert' is actually a reserved identifer...

  Note, that there is a list of keywords reserved for future use.
  It can be found in '<deemon/compiler/__keywords.inl>', but no further
  keywords will ever get added onto that list. The keywords contain
  some more things we might need to implement user classes and
  struct/union/enum type declarations.












