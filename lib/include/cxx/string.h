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

__if_not_exists(size_t) {
	const size_t = __SIZE_TYPE__;
}

#ifndef NULL
#define NULL none
#endif

const memcpy  = __builtin_object(0x00C5); // void *memcpy(void *dst, void const *src, size_t n);
const memmove = __builtin_object(0x00C6); // void *memmove(void *dst, void const *src, size_t n);
const strcpy  = __builtin_object(0x00CC); // char *strcpy(char *dst, char const *src);
const strncpy = __builtin_object(0x00CD); // char *strncpy(char *dst, char const *src, size_t n);
const strcat  = __builtin_object(0x00CE); // char *strcat(char *dst, char const *src);
const strncat = __builtin_object(0x00EA); // char *strncat(char *dst, char const *src, size_t n);

const memcmp  = __builtin_object(0x00C7); // int memcmp(void const *a, void const *b, size_t n);
const strcmp  = __builtin_object(0x00C9); // int strcmp(char const *a, char const *b);
const strcoll = __builtin_object(0x00C8); // int strcoll(char const *a, char const *b);
const strncmp = __builtin_object(0x00CA); // int strncmp(char const *a, char const *b, size_t n);
const strxfrm = __builtin_object(0x00CB); // size_t strxfrm(char *a, char const *b, size_t n)

const memchr  = __builtin_object(0x00CF); // void *memchr(void const *p, int c, size_t n);
const strchr  = __builtin_object(0x00D0); // char *strchr(char const *s, int c);
const strcspn = __builtin_object(0x00D1); // size_t strcspn(char const *a, char const *b);
const strpbrk = __builtin_object(0x00D2); // char *strpbrk(char const *a, char const *b);
const strrchr = __builtin_object(0x00D3); // char *strrchr(char const *a, int c);
const strspn  = __builtin_object(0x00D4); // size_t strspn(char const *a, char const *b);
const strstr  = __builtin_object(0x00D5); // char *strstr(char const *a, char const *b);
const strtok  = __builtin_object(0x00D6); // char *strtok(char *a, char const *b);

const memset   = __builtin_object(0x00F5); // void *memset(void *p, int c, size_t s);
const strerror = __builtin_object(0x00D7); // string strerror(int error);
const strlen   = __builtin_object(0x00D8); // size_t strlen(char const *s);

#ifndef __STDC__
// Extensions

const strnlen   = __builtin_object(0x00D9); // size_t strlen(char const *s, size_t max);
const memccpy   = __builtin_object(0x00F7); // void *memccpy(void *dst, void const *src, int c, size_t n);
const rawmemchr = __builtin_object(0x00F8); // void *rawmemchr(void const *s, int c);
const memrchr   = __builtin_object(0x00F9); // void *memrchr(const void *s, int c, size_t n);
// TODO?  int strcoll_l(const char *s1, const char *s2, locale_t l);
const function strcoll_l(__s1,__s2,__l) -> strcoll(__s1,__s2);
// TODO?  size_t strxfrm_l(char *dest, const char *src, size_t n, locale_t l);
const function strxfrm_l(__dest,__src,__n,__l) -> strxfrm(__dest,__src,__n);
const strdup    = __builtin_object(0x00E8); // char *strdup(const char *s);
const strndup   = __builtin_object(0x00E9); // char *strndup(const char *s, size_t n);
const strchrnul = __builtin_object(0x00FA); // char *strchrnul(const char *s, int c);
// TODO?  char *strtok_r(char *s, const char *delim, char **save_ptr);
const function strtok_r(__s,__delim,__save_ptr)  -> strtok(__s,__delim);
const strcasestr = __builtin_object(0x00FB); // char *strcasestr(const char *haystack, const char *needle);
const memmem    = __builtin_object(0x00FD); // void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
const __mempcpy = __builtin_object(0x00F6); // void *__mempcpy(void *dest, void const *src, size_t n);
const mempcpy   = __builtin_object(0x00F6); // void *mempcpy(void *dest, void const *src, size_t n);
const function strerror_r(__errnum,__buf,__buflen) -> strerror(__errnum);
// TODO?  char *strerror_l(int errnum, locale_t l);
const function strerror_l(__errnum,__l) -> strerror(__errnum);

const function __bzero(__s,__n) { memset(__s,0,__n); }
const function bcopy(__src,__dst,__n) { memmove(__dst,__src,__n); }
const function bzero(__s,__n) { memset(__s,0,__n); }
const bcmp      = __builtin_object(0x00C7);
const index     = __builtin_object(0x00D0);
const rindex    = __builtin_object(0x00D3);

#include <__ffs.dee>
#define ffs  __ffs
#define ffsl __ffsl

const strcasecmp = __builtin_object(0x00FC);  // int strcasecmp(const char *s1, const char *s2);
const strncasecmp = __builtin_object(0x00FE); // int strncasecmp(const char *s1, const char *s2, size_t n);

// TODO?  int strcasecmp_l(const char *s1, const char *s2, locale_t loc);
const function strcasecmp_l(__s1,__s2,__loc) -> strcasecmp(__s1,__s2);
// TODO?  int strncasecmp_l(const char *s1, const char *s2, size_t n, locale_t loc);
const function strncasecmp_l(__s1,__s2,__n,__loc) -> strncasecmp(__s1,__s2,__n);

// TODO: char *strsep(char **stringp, const char *delim);

// TODO: char *strsignal(int sig);
#define strsignal(sig) ""

// char *__stpcpy(char *dest, const char *src);
const __stpcpy = __builtin_object(0x00FF);

// char *stpcpy(char *dest, const char *src);
const stpcpy = __builtin_object(0x00FF);

// char *__stpncpy(char *dest, const char *src, size_t n);
const __stpncpy = __builtin_object(0x0100);

// char *stpncpy(char *dest, const char *src, size_t n);
const stpncpy = __builtin_object(0x0100);

const function basename(__fname): __f {
	__f = (char *)__fname;
	if (__f) while (!(*__f++ in pack('/','\\')));
}

// TODO: int strverscmp(const char *s1, const char *s2);
// TODO: char *strfry(char *string);
// TODO: void *memfrob(void *s, size_t n);

#endif
