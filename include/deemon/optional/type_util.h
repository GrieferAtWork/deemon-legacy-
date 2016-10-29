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
#ifndef GUARD_DEEMON_OPTIONAL_TYPE_UTIL_H
#define GUARD_DEEMON_OPTIONAL_TYPE_UTIL_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_DEX
#define DEE_PRIVATE_CLAMP_INDEX(i,size)\
do{\
 if (((i)%=(Dee_ssize_t)(size))<0)(i)+=(Dee_ssize_t)(size);\
}while(0)
#define DEE_PRIVATE_CLAMP_UINDEX(i,size)\
do{\
 (i)%=(Dee_size_t)(size);\
}while(0)
#define DEE_PRIVATE_CLAMP_RANGE(lo,hi,size)\
do{\
 if ((lo)<0)(lo)=0;else if((lo)>(Dee_ssize_t)(size))(lo)=((Dee_ssize_t)(size));\
 if ((hi)<0)(hi)=0;else if((hi)>(Dee_ssize_t)(size))(hi)=((Dee_ssize_t)(size));\
}while(0)
#define DEE_PRIVATE_CLAMP_URANGE(lo,hi,size)\
do{\
 if((lo)>(Dee_size_t)(size))(lo)=((Dee_size_t)(size));\
 if((hi)>(Dee_size_t)(size))(hi)=((Dee_size_t)(size));\
}while(0)
#endif /* DEE_LIMITED_DEX */

#endif /* !GUARD_DEEMON_OPTIONAL_TYPE_UTIL_H */
