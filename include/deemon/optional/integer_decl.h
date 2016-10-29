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
#ifndef GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H
#define GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H 1

#include <deemon/__conf.inl>
#ifndef GUARD_DEEMON_OBJECT_H
#include <deemon/object.h>
#endif
#ifndef GUARD_DEEMON_INTEGER_H
#include <deemon/integer.h>
#endif
#ifndef GUARD_DEEMON_STRUCTURED_H
#include <deemon/structured.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif

DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeInt8Object  { DEE_STRUCTURED_OBJECT_HEAD union {  Dee_rt_int8_t s;  Dee_rt_uint8_t u; } i_value; };
struct DeeInt16Object { DEE_STRUCTURED_OBJECT_HEAD union { Dee_rt_int16_t s; Dee_rt_uint16_t u; } i_value; };
struct DeeInt32Object { DEE_STRUCTURED_OBJECT_HEAD union { Dee_rt_int32_t s; Dee_rt_uint32_t u; } i_value; };
struct DeeInt64Object { DEE_STRUCTURED_OBJECT_HEAD union { Dee_rt_int64_t s; Dee_rt_uint64_t u; } i_value; };
DEE_COMPILER_STRUCTURE_PACKED_END

#define DeeInt8_VALUE(ob)   ((DeeInt8Object *)Dee_REQUIRES_POINTER(ob))->i_value.s
#define DeeInt16_VALUE(ob)  ((DeeInt16Object *)Dee_REQUIRES_POINTER(ob))->i_value.s
#define DeeInt32_VALUE(ob)  ((DeeInt32Object *)Dee_REQUIRES_POINTER(ob))->i_value.s
#define DeeInt64_VALUE(ob)  ((DeeInt64Object *)Dee_REQUIRES_POINTER(ob))->i_value.s
#define DeeUInt8_VALUE(ob)  ((DeeInt8Object *)Dee_REQUIRES_POINTER(ob))->i_value.u
#define DeeUInt16_VALUE(ob) ((DeeInt16Object *)Dee_REQUIRES_POINTER(ob))->i_value.u
#define DeeUInt32_VALUE(ob) ((DeeInt32Object *)Dee_REQUIRES_POINTER(ob))->i_value.u
#define DeeUInt64_VALUE(ob) ((DeeInt64Object *)Dee_REQUIRES_POINTER(ob))->i_value.u

struct DeeRangeInt8IteratorObject { DEE_OBJECT_HEAD Dee_rt_int8_t rii_begin,rii_end,rii_step; };
struct DeeRangeInt16IteratorObject { DEE_OBJECT_HEAD Dee_rt_int16_t rii_begin,rii_end,rii_step; };
struct DeeRangeInt32IteratorObject { DEE_OBJECT_HEAD Dee_rt_int32_t rii_begin,rii_end,rii_step; };
struct DeeRangeInt64IteratorObject { DEE_OBJECT_HEAD Dee_rt_int64_t rii_begin,rii_end,rii_step; };
struct DeeRangeUInt8IteratorObject { DEE_OBJECT_HEAD Dee_rt_uint8_t rii_begin,rii_end,rii_step; };
struct DeeRangeUInt16IteratorObject { DEE_OBJECT_HEAD Dee_rt_uint16_t rii_begin,rii_end,rii_step; };
struct DeeRangeUInt32IteratorObject { DEE_OBJECT_HEAD Dee_rt_uint32_t rii_begin,rii_end,rii_step; };
struct DeeRangeUInt64IteratorObject { DEE_OBJECT_HEAD Dee_rt_uint64_t rii_begin,rii_end,rii_step; };

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H */
