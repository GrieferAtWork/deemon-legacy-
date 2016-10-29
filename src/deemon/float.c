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
#ifndef GUARD_DEEMON_FLOAT_C
#define GUARD_DEEMON_FLOAT_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/integer.h>
#include <deemon/file.h>
#include <deemon/math.h>
#include <deemon/memberdef.h>
#include <deemon/structured.h>
#include <deemon/marshal_data.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/hash.h>
#include DEE_INCLUDE_MEMORY_API()

#ifndef __INTELLISENSE__
#define KIND 0
#include "float_impl.inl"
#define KIND 1
#include "float_impl.inl"
#define KIND 2
#include "float_impl.inl"
#endif

DEE_DECL_BEGIN
Dee_size_t DeeFloat_ClearPool(void) {
#ifndef __INTELLISENSE__
 return DEE_OBJECTPOOL_CLEAR(float_pool)
      + DEE_OBJECTPOOL_CLEAR(double_pool)
      + DEE_OBJECTPOOL_CLEAR(ldouble_pool);
#endif
}
DEE_DECL_END


#endif /* !GUARD_DEEMON_FLOAT_H */
