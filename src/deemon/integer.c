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
#ifndef GUARD_DEEMON_INTEGER_C
#define GUARD_DEEMON_INTEGER_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/__atomic_intrin.inl>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/string.h>
#include <deemon/file.h>
#include <deemon/marshal.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/optional/object_pool.h>
#if DEE_XCONFIG_INTEGER_DIVIDE_BY_ZERO_USE_SIGNALHANDLER || \
    DEE_XCONFIG_INTEGER_MODULO_BY_ZERO_USE_SIGNALHANDLER
#include <deemon/signalhandler.h>
#endif

DEE_DECL_BEGIN


// We spread the static integer cache thin, while concentrating on
// the more often used integer types, such as 32-bit, 64-bit and then 8-bit
DEE_OBJECTPOOL_DEFINE(int8_pool,DeeInt8Object)
DEE_OBJECTPOOL_DEFINE(int16_pool,DeeInt16Object)
DEE_OBJECTPOOL_DEFINE(int32_pool,DeeInt32Object)
DEE_OBJECTPOOL_DEFINE(int64_pool,DeeInt64Object)

Dee_size_t DeeInt_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(int8_pool)+DEE_OBJECTPOOL_CLEAR(int16_pool)+
        DEE_OBJECTPOOL_CLEAR(int32_pool)+DEE_OBJECTPOOL_CLEAR(int64_pool);
}

DEE_DECL_END

#ifndef __INTELLISENSE__
// Intellisense syntax screws up if it sees us
// including this multiple times with different settings
#define N 8
#include "interger.impl.inl"
#define N 16
#include "interger.impl.inl"
#define N 32
#include "interger.impl.inl"
#define N 64
#include "interger.impl.inl"
#include "bool.inl"
#endif

#endif /* !GUARD_DEEMON_INTEGER_C */
