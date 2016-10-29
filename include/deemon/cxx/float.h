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
#ifndef GUARD_DEEMON_CXX_FLOAT_H
#define GUARD_DEEMON_CXX_FLOAT_H 1

#include <deemon/__conf.inl>
#include <deemon/float.h>
#include <deemon/cxx/object.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {
namespace detail {
template<> struct _typeof_impl<Dee_rt_float> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(Dee_rt_float); }
 static inline DeeObject *new_(Dee_rt_float v) DEE_CXX11_NOEXCEPT { return DeeObject_New(Dee_rt_float,v); }
};
#if DEE_CONFIG_RT_FLOATID_DOUBLE != DEE_CONFIG_RT_FLOATID_FLOAT
template<> struct _typeof_impl<Dee_rt_double> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(Dee_rt_double); }
 static inline DeeObject *new_(Dee_rt_double v) DEE_CXX11_NOEXCEPT { return DeeObject_New(Dee_rt_double,v); }
};
#endif
#if DEE_CONFIG_RT_FLOATID_LDOUBLE != DEE_CONFIG_RT_FLOATID_FLOAT && \
    DEE_CONFIG_RT_FLOATID_LDOUBLE != DEE_CONFIG_RT_FLOATID_DOUBLE
template<> struct _typeof_impl<Dee_rt_ldouble> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(Dee_rt_ldouble); }
 static inline DeeObject *new_(Dee_rt_ldouble v) DEE_CXX11_NOEXCEPT { return DeeObject_New(Dee_rt_ldouble,v); }
};
#endif
}
}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_FLOAT_H */
