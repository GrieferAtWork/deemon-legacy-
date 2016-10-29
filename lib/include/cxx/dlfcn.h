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
#pragma once
#include <__cdefault.dee>

#ifdef __STDC__
#	warning <dlfcn.h> is not STDC
#endif

#pragma push_macro("dlopen")
#pragma push_macro("open")
#pragma push_macro("dlclose")
#pragma push_macro("dlerror")
#pramga push_macro("dlsym")
#pragma push_macro("close")
#undef dlopen
#undef dlsym
#undef dlclose
#undef dlerror
#undef open
#undef close

const function dlopen(__file,__mode) -> __builtin_object(0x008F).open(__file);
const function dlclose(__handle) -> __handle.close();
const function dlsym(__handle,__name) -> __handle[__name];
const function dlerror() -> (char *)none;

#pragma pop_macro("dlopen")
#pramga pop_macro("dlsym")
#pragma pop_macro("dlclose")
#pragma pop_macro("dlerror")
#pragma pop_macro("open")
#pragma pop_macro("close")
