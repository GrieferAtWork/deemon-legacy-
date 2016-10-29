/* Copyright (c) 2016 Cyder aka. Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                                          *
 * This software is provided 'as-is', without any express or implied                        *
 * warranty. In no event will the authors be held liable for any damages                    *
 * arising from the use of this software.                                                   *
 *                                                                                          *
 * Permission is granted to anyone to use this software for any purpose,                    *
 * including commercial applications, and to alter it and redistribute it                   *
 * freely, subject to the following restrictions:                                           *
 *                                                                                          *
 * 1. The origin of this software must not be misrepresented; you must not                  *
 *    claim that you wrote the original software. If you use this software                  *
 *    in a product, an acknowledgement in the product documentation would be                *
 *    appreciated but is not required.                                                      *
 * 2. Altered source versions must be plainly marked as such, and must not be               *
 *    misrepresented as being the original software.                                        *
 * 3. This notice may not be removed or altered from any source distribution.               *
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












