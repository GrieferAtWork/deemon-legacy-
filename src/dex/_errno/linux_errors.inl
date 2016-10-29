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

#ifdef E2BIG
LINUX_ERROR(E2BIG)
#endif
#ifdef EACCES
LINUX_ERROR(EACCES)
#endif
#ifdef EADDRINUSE
LINUX_ERROR(EADDRINUSE)
#endif
#ifdef EADDRNOTAVAIL
LINUX_ERROR(EADDRNOTAVAIL)
#endif
#ifdef EADV
LINUX_ERROR(EADV)
#endif
#ifdef EAFNOSUPPORT
LINUX_ERROR(EAFNOSUPPORT)
#endif
#ifdef EAGAIN
LINUX_ERROR(EAGAIN)
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || (EAGAIN != EWOULDBLOCK))
LINUX_ERROR(EWOULDBLOCK)
#endif
#ifdef EALREADY
LINUX_ERROR(EALREADY)
#endif
#ifdef EBADE
LINUX_ERROR(EBADE)
#endif
#ifdef EBADF
LINUX_ERROR(EBADF)
#endif
#ifdef EBADFD
LINUX_ERROR(EBADFD)
#endif
#ifdef EBADMSG
LINUX_ERROR(EBADMSG)
#endif
#ifdef EBADR
LINUX_ERROR(EBADR)
#endif
#ifdef EBADRQC
LINUX_ERROR(EBADRQC)
#endif
#ifdef EBADSLT
LINUX_ERROR(EBADSLT)
#endif
#ifdef EBFONT
LINUX_ERROR(EBFONT)
#endif
#ifdef EBUSY
LINUX_ERROR(EBUSY)
#endif
#ifdef ECANCELED
LINUX_ERROR(ECANCELED)
#endif
#ifdef ECHILD
LINUX_ERROR(ECHILD)
#endif
#ifdef ECHRNG
LINUX_ERROR(ECHRNG)
#endif
#ifdef ECOMM
LINUX_ERROR(ECOMM)
#endif
#ifdef ECONNABORTED
LINUX_ERROR(ECONNABORTED)
#endif
#ifdef ECONNREFUSED
LINUX_ERROR(ECONNREFUSED)
#endif
#ifdef ECONNRESET
LINUX_ERROR(ECONNRESET)
#endif
#ifdef EDEADLK
LINUX_ERROR(EDEADLK)
#endif
#if defined(EDEADLOCK) && (!defined(EDEADLK) || (EDEADLK != EDEADLOCK))
LINUX_ERROR(EDEADLOCK)
#endif
#ifdef EDESTADDRREQ
LINUX_ERROR(EDESTADDRREQ)
#endif
#ifdef EDOM
LINUX_ERROR(EDOM)
#endif
#ifdef EDOTDOT
LINUX_ERROR(EDOTDOT)
#endif
#ifdef EDQUOT
LINUX_ERROR(EDQUOT)
#endif
#ifdef EEXIST
LINUX_ERROR(EEXIST)
#endif
#ifdef EFAULT
LINUX_ERROR(EFAULT)
#endif
#ifdef EFBIG
LINUX_ERROR(EFBIG)
#endif
#ifdef EHOSTDOWN
LINUX_ERROR(EHOSTDOWN)
#endif
#ifdef EHOSTUNREACH
LINUX_ERROR(EHOSTUNREACH)
#endif
#ifdef EIDRM
LINUX_ERROR(EIDRM)
#endif
#ifdef EILSEQ
LINUX_ERROR(EILSEQ)
#endif
#ifdef EINPROGRESS
LINUX_ERROR(EINPROGRESS)
#endif
#ifdef EINTR
LINUX_ERROR(EINTR)
#endif
#ifdef EINVAL
LINUX_ERROR(EINVAL)
#endif
#ifdef EIO
LINUX_ERROR(EIO)
#endif
#ifdef EISCONN
LINUX_ERROR(EISCONN)
#endif
#ifdef EISDIR
LINUX_ERROR(EISDIR)
#endif
#ifdef EISNAM
LINUX_ERROR(EISNAM)
#endif
#ifdef EKEYEXPIRED
LINUX_ERROR(EKEYEXPIRED)
#endif
#ifdef EKEYREJECTED
LINUX_ERROR(EKEYREJECTED)
#endif
#ifdef EKEYREVOKED
LINUX_ERROR(EKEYREVOKED)
#endif
#ifdef EL2HLT
LINUX_ERROR(EL2HLT)
#endif
#ifdef EL2NSYNC
LINUX_ERROR(EL2NSYNC)
#endif
#ifdef EL3HLT
LINUX_ERROR(EL3HLT)
#endif
#ifdef EL3RST
LINUX_ERROR(EL3RST)
#endif
#ifdef ELIBACC
LINUX_ERROR(ELIBACC)
#endif
#ifdef ELIBBAD
LINUX_ERROR(ELIBBAD)
#endif
#ifdef ELIBEXEC
LINUX_ERROR(ELIBEXEC)
#endif
#ifdef ELIBMAX
LINUX_ERROR(ELIBMAX)
#endif
#ifdef ELIBSCN
LINUX_ERROR(ELIBSCN)
#endif
#ifdef ELNRNG
LINUX_ERROR(ELNRNG)
#endif
#ifdef ELOOP
LINUX_ERROR(ELOOP)
#endif
#ifdef EMEDIUMTYPE
LINUX_ERROR(EMEDIUMTYPE)
#endif
#ifdef EMFILE
LINUX_ERROR(EMFILE)
#endif
#ifdef EMLINK
LINUX_ERROR(EMLINK)
#endif
#ifdef EMSGSIZE
LINUX_ERROR(EMSGSIZE)
#endif
#ifdef EMULTIHOP
LINUX_ERROR(EMULTIHOP)
#endif
#ifdef ENAMETOOLONG
LINUX_ERROR(ENAMETOOLONG)
#endif
#ifdef ENAVAIL
LINUX_ERROR(ENAVAIL)
#endif
#ifdef ENETDOWN
LINUX_ERROR(ENETDOWN)
#endif
#ifdef ENETRESET
LINUX_ERROR(ENETRESET)
#endif
#ifdef ENETUNREACH
LINUX_ERROR(ENETUNREACH)
#endif
#ifdef ENFILE
LINUX_ERROR(ENFILE)
#endif
#ifdef ENOANO
LINUX_ERROR(ENOANO)
#endif
#ifdef ENOBUFS
LINUX_ERROR(ENOBUFS)
#endif
#ifdef ENOCSI
LINUX_ERROR(ENOCSI)
#endif
#ifdef ENODATA
LINUX_ERROR(ENODATA)
#endif
#ifdef ENODEV
LINUX_ERROR(ENODEV)
#endif
#ifdef ENOENT
LINUX_ERROR(ENOENT)
#endif
#ifdef ENOEXEC
LINUX_ERROR(ENOEXEC)
#endif
#ifdef ENOKEY
LINUX_ERROR(ENOKEY)
#endif
#ifdef ENOLCK
LINUX_ERROR(ENOLCK)
#endif
#ifdef ENOLINK
LINUX_ERROR(ENOLINK)
#endif
#ifdef ENOMEDIUM
LINUX_ERROR(ENOMEDIUM)
#endif
#ifdef ENOMEM
LINUX_ERROR(ENOMEM)
#endif
#ifdef ENOMSG
LINUX_ERROR(ENOMSG)
#endif
#ifdef ENONET
LINUX_ERROR(ENONET)
#endif
#ifdef ENOPKG
LINUX_ERROR(ENOPKG)
#endif
#ifdef ENOPROTOOPT
LINUX_ERROR(ENOPROTOOPT)
#endif
#ifdef ENOSPC
LINUX_ERROR(ENOSPC)
#endif
#ifdef ENOSR
LINUX_ERROR(ENOSR)
#endif
#ifdef ENOSTR
LINUX_ERROR(ENOSTR)
#endif
#ifdef ENOSYS
LINUX_ERROR(ENOSYS)
#endif
#ifdef ENOTBLK
LINUX_ERROR(ENOTBLK)
#endif
#ifdef ENOTCONN
LINUX_ERROR(ENOTCONN)
#endif
#ifdef ENOTDIR
LINUX_ERROR(ENOTDIR)
#endif
#ifdef ENOTEMPTY
LINUX_ERROR(ENOTEMPTY)
#endif
#ifdef ENOTNAM
LINUX_ERROR(ENOTNAM)
#endif
#ifdef ENOTRECOVERABLE
LINUX_ERROR(ENOTRECOVERABLE)
#endif
#ifdef ENOTSOCK
LINUX_ERROR(ENOTSOCK)
#endif
#ifdef ENOTSUP
LINUX_ERROR(ENOTSUP)
#endif
#if defined(EOPNOTSUPP) && (!defined(ENOTSUP) || (ENOTSUP != EOPNOTSUPP))
LINUX_ERROR(EOPNOTSUPP)
#endif
#ifdef ENOTTY
LINUX_ERROR(ENOTTY)
#endif
#ifdef ENOTUNIQ
LINUX_ERROR(ENOTUNIQ)
#endif
#ifdef ENXIO
LINUX_ERROR(ENXIO)
#endif
#ifdef EOTHER
LINUX_ERROR(EOTHER)
#endif
#ifdef EOVERFLOW
LINUX_ERROR(EOVERFLOW)
#endif
#ifdef EOWNERDEAD
LINUX_ERROR(EOWNERDEAD)
#endif
#ifdef EPERM
LINUX_ERROR(EPERM)
#endif
#ifdef EPFNOSUPPORT
LINUX_ERROR(EPFNOSUPPORT)
#endif
#ifdef EPIPE
LINUX_ERROR(EPIPE)
#endif
#ifdef EPROTO
LINUX_ERROR(EPROTO)
#endif
#ifdef EPROTONOSUPPORT
LINUX_ERROR(EPROTONOSUPPORT)
#endif
#ifdef EPROTOTYPE
LINUX_ERROR(EPROTOTYPE)
#endif
#ifdef ERANGE
LINUX_ERROR(ERANGE)
#endif
#ifdef EREMCHG
LINUX_ERROR(EREMCHG)
#endif
#ifdef EREMOTE
LINUX_ERROR(EREMOTE)
#endif
#ifdef EREMOTEIO
LINUX_ERROR(EREMOTEIO)
#endif
#ifdef ERESTART
LINUX_ERROR(ERESTART)
#endif
#ifdef EROFS
LINUX_ERROR(EROFS)
#endif
#ifdef ESHUTDOWN
LINUX_ERROR(ESHUTDOWN)
#endif
#ifdef ESOCKTNOSUPPORT
LINUX_ERROR(ESOCKTNOSUPPORT)
#endif
#ifdef ESPIPE
LINUX_ERROR(ESPIPE)
#endif
#ifdef ESRCH
LINUX_ERROR(ESRCH)
#endif
#ifdef ESRMNT
LINUX_ERROR(ESRMNT)
#endif
#ifdef ESTALE
LINUX_ERROR(ESTALE)
#endif
#ifdef ESTRPIPE
LINUX_ERROR(ESTRPIPE)
#endif
#ifdef ETIME
LINUX_ERROR(ETIME)
#endif
#ifdef ETIMEDOUT
LINUX_ERROR(ETIMEDOUT)
#endif
#ifdef ETOOMANYREFS
LINUX_ERROR(ETOOMANYREFS)
#endif
#ifdef ETXTBSY
LINUX_ERROR(ETXTBSY)
#endif
#ifdef EUCLEAN
LINUX_ERROR(EUCLEAN)
#endif
#ifdef EUNATCH
LINUX_ERROR(EUNATCH)
#endif
#ifdef EUSERS
LINUX_ERROR(EUSERS)
#endif
#ifdef EXDEV
LINUX_ERROR(EXDEV)
#endif
#ifdef EXFULL
LINUX_ERROR(EXFULL)
#endif
#ifdef STRUNCATE
LINUX_ERROR(STRUNCATE)
#endif
