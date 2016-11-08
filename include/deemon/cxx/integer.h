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
#ifndef GUARD_DEEMON_CXX_INTEGER_H
#define GUARD_DEEMON_CXX_INTEGER_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/integer.h>
#include <deemon/cxx/object.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

template<class T> struct integer;
template<class T> struct atomic_integer;

#define DEE_CXX_MAKE_INTEGER_TYPE(T,getval,makenew,check)\
template<> struct integer<T>: object {\
 public: /* public operators */\
  inline operator T &() const DEE_CXX11_NOEXCEPT { return getval(this->ob_ptr); }\
 public: /* public constructor / destructor */\
  inline integer(DEE_A_IN T v): object(makenew(v),detail::tag_ref_or_err()) {}\
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(integer,object,check)\
}
#define DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(T,getval,makenew,check)\
template<> struct atomic_integer<T>: object {\
 public: /* public constructor / destructor */\
  inline atomic_integer(DEE_A_IN T v): object(makenew(v),detail::tag_ref_or_err()) {}\
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(atomic_integer,object,check)\
}
DEE_CXX_MAKE_INTEGER_TYPE(Dee_int8_t,DeeInt8_VALUE,::DeeInt8_New,DeeInt8_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_int16_t,DeeInt16_VALUE,::DeeInt16_New,DeeInt16_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_int32_t,DeeInt32_VALUE,::DeeInt32_New,DeeInt32_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_int64_t,DeeInt64_VALUE,::DeeInt64_New,DeeInt64_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_uint8_t,DeeUInt8_VALUE,::DeeUInt8_New,DeeUInt8_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_uint16_t,DeeUInt16_VALUE,::DeeUInt16_New,DeeUInt16_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_uint32_t,DeeUInt32_VALUE,::DeeUInt32_New,DeeUInt32_Check);
DEE_CXX_MAKE_INTEGER_TYPE(Dee_uint64_t,DeeUInt64_VALUE,::DeeUInt64_New,DeeUInt64_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_int8_t,DeeInt8_VALUE,::DeeAtomicInt8_New,DeeAtomicInt8_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_int16_t,DeeInt16_VALUE,::DeeAtomicInt16_New,DeeAtomicInt16_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_int32_t,DeeInt32_VALUE,::DeeAtomicInt32_New,DeeAtomicInt32_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_int64_t,DeeInt64_VALUE,::DeeAtomicInt64_New,DeeAtomicInt64_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_uint8_t,DeeUInt8_VALUE,::DeeAtomicUInt8_New,DeeAtomicUInt8_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_uint16_t,DeeUInt16_VALUE,::DeeAtomicUInt16_New,DeeAtomicUInt16_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_uint32_t,DeeUInt32_VALUE,::DeeAtomicUInt32_New,DeeAtomicUInt32_Check);
DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE(Dee_uint64_t,DeeUInt64_VALUE,::DeeAtomicUInt64_New,DeeAtomicUInt64_Check);
#undef DEE_CXX_MAKE_ATOMIC_INTEGER_TYPE
#undef DEE_CXX_MAKE_INTEGER_TYPE

typedef integer<Dee_int8_t>   int8;
typedef integer<Dee_int16_t>  int16;
typedef integer<Dee_int32_t>  int32;
typedef integer<Dee_int64_t>  int64;
typedef integer<Dee_uint8_t>  uint8;
typedef integer<Dee_uint16_t> uint16;
typedef integer<Dee_uint32_t> uint32;
typedef integer<Dee_uint64_t> uint64;
typedef atomic_integer<Dee_int8_t>   atomic_int8;
typedef atomic_integer<Dee_int16_t>  atomic_int16;
typedef atomic_integer<Dee_int32_t>  atomic_int32;
typedef atomic_integer<Dee_int64_t>  atomic_int64;
typedef atomic_integer<Dee_uint8_t>  atomic_uint8;
typedef atomic_integer<Dee_uint16_t> atomic_uint16;
typedef atomic_integer<Dee_uint32_t> atomic_uint32;
typedef atomic_integer<Dee_uint64_t> atomic_uint64;


namespace detail {
template<> struct _typeof_impl<char> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(char); }
 static inline DeeObject *new_(char v) DEE_CXX11_NOEXCEPT { return DeeObject_New(char,v); }
};
template<> struct _typeof_impl<signed char> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(schar); }
 static inline DeeObject *new_(signed char v) DEE_CXX11_NOEXCEPT { return DeeObject_New(schar,v); }
};
template<> struct _typeof_impl<unsigned char> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(uchar); }
 static inline DeeObject *new_(unsigned char v) DEE_CXX11_NOEXCEPT { return DeeObject_New(uchar,v); }
};
template<> struct _typeof_impl<short> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(short); }
 static inline DeeObject *new_(short v) DEE_CXX11_NOEXCEPT { return DeeObject_New(short,v); }
};
template<> struct _typeof_impl<unsigned short> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(ushort); }
 static inline DeeObject *new_(unsigned short v) DEE_CXX11_NOEXCEPT { return DeeObject_New(ushort,v); }
};
template<> struct _typeof_impl<int> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(int); }
 static inline DeeObject *new_(int v) DEE_CXX11_NOEXCEPT { return DeeObject_New(int,v); }
};
template<> struct _typeof_impl<unsigned int> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(uint); }
 static inline DeeObject *new_(unsigned int v) DEE_CXX11_NOEXCEPT { return DeeObject_New(uint,v); }
};
template<> struct _typeof_impl<long> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(long); }
 static inline DeeObject *new_(long v) DEE_CXX11_NOEXCEPT { return DeeObject_New(long,v); }
};
template<> struct _typeof_impl<unsigned long> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(ulong); }
 static inline DeeObject *new_(unsigned long v) DEE_CXX11_NOEXCEPT { return DeeObject_New(ulong,v); }
};
#ifdef DEE_TYPES_SIZEOF_LLONG
template<> struct _typeof_impl<long long> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(llong); }
 static inline DeeObject *new_(long long v) DEE_CXX11_NOEXCEPT { return DeeObject_New(llong,v); }
};
template<> struct _typeof_impl<unsigned long long> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(ullong); }
 static inline DeeObject *new_(unsigned long long v) DEE_CXX11_NOEXCEPT { return DeeObject_New(ullong,v); }
};
#endif
}

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_INTEGER_H */
