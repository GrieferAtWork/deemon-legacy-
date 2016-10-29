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
#ifdef __INTELLISENSE__
#include "socket.c"
DEE_DECL_BEGIN
//#define VALUE
#endif


#ifdef VALUE
DEE_A_RET_EXCEPT(-1) int DeeSocket_SetSockOpt(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN int level, DEE_A_IN int optname,
 DEE_A_IN size_t argc, DEE_A_IN_R(argc) DeeObject *const *argv)
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeSocket_GetSockOpt(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN int level, DEE_A_IN int optname)
#endif
{
#ifdef VALUE
 int intval;
#else
 DeeObject *result;
 socklen_t result_len;
#endif
 int error; sock_t sock;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
#ifdef VALUE
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (argc == 2 && Dee_TYPE(argv[1]) == DeeObject_TYPE(Dee_size_t)) {
  DeeTypeObject *temp;
  if ((temp = DeeType_Pointer((DeeTypeObject *)&DeeNone_Type)) == NULL) return -1;
  if (Dee_TYPE(argv[0]) == temp) goto fallback_opt; // setsockopt(?,?,none *,size_t);
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif
 switch (level) {
#if defined(SOL_SOCKET) || !defined(VALUE)
#ifdef SOL_SOCKET
  case SOL_SOCKET:
#endif
  case DEE_INTERNAL_SOL_SOCKET:
   switch (optname) {
#ifndef VALUE
    case DEE_INTERNAL_SO_ACCEPTCONN:
     DeeReturn_Bool(DeeSocket_IsListening(self));
    case DEE_INTERNAL_SO_TYPE:
     DeeReturn_Bool(DeeSocket_GET_TYPE(self));
    case DEE_INTERNAL_SO_DOMAIN:
     DeeReturn_Bool(DeeSocket_GET_ADDRESS_FAMILY(self));
    case DEE_INTERNAL_SO_PROTOCOL:
     DeeReturn_Bool(DeeSocket_GET_PROTOCOL(self));
#endif


#ifdef SO_DEBUG
    case SO_DEBUG:
#endif
#ifdef SO_ACCEPTCONN
    case SO_ACCEPTCONN:
#endif
#ifdef SO_REUSEADDR
    case SO_REUSEADDR:
#endif
#ifdef SO_KEEPALIVE
    case SO_KEEPALIVE:
#endif
#ifdef SO_DONTROUTE
    case SO_DONTROUTE:
#endif
#ifdef SO_BROADCAST
    case SO_BROADCAST:
#endif
#ifdef SO_USELOOPBACK
    case SO_USELOOPBACK:
#endif
#ifdef SO_OOBINLINE
    case SO_OOBINLINE:
#endif
#ifdef SO_DONTLINGER
    case SO_DONTLINGER:
#endif
#ifdef SO_EXCLUSIVEADDRUSE
    case SO_EXCLUSIVEADDRUSE:
#endif
#ifdef SO_CONDITIONAL_ACCEPT
    case SO_CONDITIONAL_ACCEPT:
#endif
val_bool:
    {
#ifdef VALUE
     if (argc != 1) goto fallback_opt;
     if (DeeString_Check(argv[0])) goto fallback_opt;
     if ((intval = DeeObject_Bool(argv[0])) == -1) return -1;
     DeeSocket_USEBEGIN(self,sock);
     error = setsockopt(sock,level,optname,(char *)&intval,(socklen_t)sizeof(intval));
     DeeSocket_USEEND(self);
     if (error != 0) {
syserr_intval:
      error = (int)DeeNetError_GET(); DeeNetError_SET(0);
      DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                          "setsockopt(%k,%k,%k,%d) : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),intval,
                          DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
      return -1;
     }
#else
     int retval; result_len = sizeof(retval);
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,(char *)&retval,&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) {
syserr:
      error = (int)DeeNetError_GET(); DeeNetError_SET(0);
      DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                          "getsockopt(%k,%k,%k) : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),
                          DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
      return NULL;
     }
     DeeReturn_Bool(retval);
#endif
    } break;

#ifdef SO_SNDTIMEO
    case SO_SNDTIMEO: {
     unsigned int msecs;
#ifdef VALUE
     if (argc != 1) goto fallback_opt;
     if (DeeString_Check(argv[0])) goto fallback_opt;
     if (DeeObject_Cast(uint,argv[0],&msecs) != 0) return -1;
     return DeeSocket_SetSendTimeout(self,msecs);
#else
     if (DeeSocket_GetSendTimeout(self,&msecs) != 0) return NULL;
     return DeeObject_New(uint,msecs);
#endif
    }
#endif
#ifdef SO_RCVTIMEO
    case SO_RCVTIMEO: {
     unsigned int msecs;
#ifdef VALUE
     if (argc != 1) goto fallback_opt;
     if (DeeString_Check(argv[0])) goto fallback_opt;
     if (DeeObject_Cast(uint,argv[0],&msecs) != 0) return -1;
     return DeeSocket_SetRecvTimeout(self,msecs);
#else
     if (DeeSocket_GetRecvTimeout(self,&msecs) != 0) return NULL;
     return DeeObject_New(uint,msecs);
#endif
    }
#endif

#ifdef SO_SNDBUF
    case SO_SNDBUF:
#endif
#ifdef SO_RCVBUF
    case SO_RCVBUF:
#endif
#ifdef SO_SNDLOWAT
    case SO_SNDLOWAT:
#endif
#ifdef SO_RCVLOWAT
    case SO_RCVLOWAT:
#endif
#ifdef DEE_PLATFORM_WINDOWS
#define val_dword_or_size_t val_int
     goto val_dword_or_size_t;
#else
val_dword_or_size_t:
    {
#ifdef VALUE
     size_t value;
     if (argc != 1) goto fallback_opt;
     if (DeeString_Check(argv[0])) goto fallback_opt;
     if (DeeObject_Cast(size_t,argv[0],&value) != 0) return -1;
     DeeSocket_USEBEGIN(self,sock);
     error = setsockopt(sock,level,optname,(char const *)&value,sizeof(value));
     DeeSocket_USEEND(self);
     if (error != 0) {
      error = DeeNetError_GET(); DeeNetError_SET(0);
      DeeError_SetStringf(DeeErrorType_NETERROR(error),
                          "setsockopt(%k,%k,%k,%Iu) : %#.8x : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),value,error,
                          DEESOCKET_PRINTFERR(error));
      return -1;
     }
#else
     size_t retval;
     result_len = sizeof(retval);
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,(char *)&retval,&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr;
     return DeeObject_New(size_t,(size_t)retval);
#endif
    } break;
#endif

#ifdef SO_ERROR
    case SO_ERROR:
#endif
#ifdef SO_TYPE
    case SO_TYPE:
#endif
#ifdef SO_DOMAIN
    case SO_DOMAIN:
#endif
#ifdef SO_PROTOCOL
    case SO_PROTOCOL:
#endif
#ifdef SO_GROUP_PRIORITY
    case SO_GROUP_PRIORITY:
#endif
val_int:
    {
#ifdef DEE_PLATFORM_WINDOWS
     DEE_STATIC_ASSERT(sizeof(DWORD) == sizeof(int));
     DEE_STATIC_ASSERT(sizeof(ULONG) == sizeof(int));
#endif
#ifdef VALUE
     if (argc != 1) goto fallback_opt;
     if (DeeString_Check(argv[0])) goto fallback_opt;
     if (DeeObject_Cast(int,argv[0],&intval) != 0) return -1;
     DeeSocket_USEBEGIN(self,sock);
     error = setsockopt(sock,level,optname,(char const *)&intval,sizeof(intval));
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr_intval;
#else
     int retval;
     result_len = sizeof(retval);
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,(char *)&retval,&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr;
     return DeeObject_New(int,retval);
#endif
    } break;

#if defined(SO_GROUP_ID) || defined(SO_MAX_MSG_SIZE)
#ifdef SO_GROUP_ID
    case SO_GROUP_ID:
#endif
#ifdef SO_MAX_MSG_SIZE
    case SO_MAX_MSG_SIZE: // Actually, this is ULONG
#endif
#ifdef VALUE
#define val_uint  val_int
#define val_ulong val_int
     goto val_uint;
#else
    {
     unsigned int retval;
     result_len = sizeof(retval);
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,(char *)&retval,&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr;
     return DeeObject_New(uint,retval);
    }
#endif
#endif

#ifdef SO_LINGER
    case SO_LINGER: {
     struct linger value;
#ifdef VALUE
     int temp;
     if (argc == 1 && DeeTuple_Check(argv[0]) && DeeTuple_SIZE(argv[0]) == 2
         ) argv = DeeTuple_ELEM(argv[0]),argc = 2;
     if (argc == 1 && DeeString_Check(argv[0])) goto fallback_opt;
     if (argc != 2) { DeeError_TypeError_ArgCountExpected(2,argc); return -1; }
     if ((temp = DeeObject_Bool(argv[0])) == -1) return -1;
     value.l_onoff = (u_short)temp;
     if (DeeObject_Cast(int,argv[1],&temp) != 0) return -1;
     value.l_linger = (u_short)temp;
     DeeSocket_USEBEGIN(self,sock);
     error = setsockopt(sock,level,optname,(char const *)&intval,sizeof(intval));
     DeeSocket_USEEND(self);
     if (error != 0) {
      error = (int)DeeNetError_GET(); DeeNetError_SET(0);
      DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                          "setsockopt(%k,%k,%k,{%hu,%hu}) : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),
                          value.l_onoff,value.l_linger,error,
                          DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
      return -1;
     }
#else
     result_len = sizeof(value);
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,(char *)&value,&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr;
     return Dee_BuildValue("(bd)",(int)value.l_onoff,(int)value.l_linger);
#endif
    } break;
#endif

#ifdef SO_BINDTODEVICE
    case SO_BINDTODEVICE: {
#ifdef VALUE
     if (argc != 1) goto fallback_opt;
     if (DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
     DeeSocket_USEBEGIN(self,sock);
     error = setsockopt(sock,level,optname,DeeString_STR(argv[0]),DeeString_SIZE(argv[0]));
     DeeSocket_USEEND(self);
     if (error != 0) {
      error = DeeNetError_GET(); DeeNetError_SET(0);
      DeeError_SetStringf(DeeErrorType_NETERROR(error),
                          "setsockopt(%k,%k,%k,%r) : %#.8x : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),
                          argv[0],error,DEESOCKET_PRINTFERR(error));
      return -1;
     }
#else
#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16 // Should always be defined, but better be safe...
#endif
     result_len = IF_NAMESIZE;
     if ((result = DeeString_NewSized(IF_NAMESIZE)) == NULL) return NULL;
     DeeSocket_USEBEGIN(self,sock);
     error = getsockopt(sock,level,optname,DeeString_STR(result),&result_len);
     DeeSocket_USEEND(self);
     if (error != 0) goto syserr;
     if (DeeString_Resize(&result,(size_t)result_len) != 0) Dee_CLEAR(result);
     return result;
#endif
    } break;
#endif

    default: goto fallback_opt;
   }
   break;
#endif /* SOL_SOCKET */



#ifdef IPPROTO_TCP
  case IPPROTO_TCP:
   switch (optname) {
#ifdef TCP_NODELAY
    case TCP_NODELAY: goto val_bool;
#endif
#ifdef TCP_EXPEDITED_1122
    case TCP_EXPEDITED_1122: goto val_bool;
#endif
#ifdef TCP_KEEPALIVE
    case TCP_KEEPALIVE: goto val_bool;
#endif
#ifdef TCP_MAXSEG
    case TCP_MAXSEG: goto val_int;
#endif
#ifdef TCP_MAXRT
    case TCP_MAXRT: goto val_int;
#endif
#ifdef TCP_STDURG
    case TCP_STDURG: goto val_bool;
#endif
#ifdef TCP_NOURG
    case TCP_NOURG: goto val_bool;
#endif
#ifdef TCP_ATMARK
    case TCP_ATMARK: goto val_int;
#endif
#ifdef TCP_NOSYNRETRIES
    case TCP_NOSYNRETRIES: goto val_bool;
#endif
#ifdef TCP_TIMESTAMPS
    case TCP_TIMESTAMPS: goto val_bool;
#endif
#ifdef TCP_OFFLOAD_PREFERENCE
    case TCP_OFFLOAD_PREFERENCE: goto val_int;
#endif
#ifdef TCP_CONGESTION_ALGORITHM
    case TCP_CONGESTION_ALGORITHM: goto val_int;
#endif
#ifdef TCP_DELAY_FIN_ACK
    case TCP_DELAY_FIN_ACK: goto val_int;
#endif
    default: goto fallback_opt;
   }
   break;
#endif /* IPPROTO_TCP */



  default:fallback_opt: {
#ifdef VALUE
   void *p; size_t s;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (argc == 2) {
    if (DeeObject_GetVoidPointerEx(argv[0],&p) != 0 ||
        DeeObject_Cast(size_t,argv[1],&s) != 0) return -1;
   } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (argc == 1) {
    if (DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
    p = DeeString_STR(argv[0]);
    s = DeeString_SIZE(argv[0]);
   } else {
    DeeError_SetStringf(&DeeErrorType_TypeError,
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
                        "setsockopt with unknown arguments (%k,%k) requires 1 or 2 arguments",
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
                        "setsockopt with unknown arguments (%k,%k) requires 1 argument",
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
                        DeeSocket_OptLevelToString(optname),
                        DeeSocket_OptNameToString(optname));
    return -1;
   }
   DeeSocket_USEBEGIN(self,sock);
   error = setsockopt(sock,level,optname,(char const *)p,(socklen_t)s);
   DeeSocket_USEEND(self);
   if (error != 0) {
    error = (int)DeeNetError_GET(); DeeNetError_SET(0);
    switch (error) {
#ifdef EPROTOTYPE
     case EPROTOTYPE:
#endif
#ifdef WSAEPROTOTYPE
     case WSAEPROTOTYPE:
#endif
#ifdef ENOPROTOOPT
     case ENOPROTOOPT:
#endif
#ifdef WSAENOPROTOOPT
     case WSAENOPROTOOPT:
#endif
#ifdef EPROTONOSUPPORT
     case EPROTONOSUPPORT:
#endif
#ifdef WSAEPROTONOSUPPORT
     case WSAEPROTONOSUPPORT:
#endif
#ifdef ESOCKTNOSUPPORT
     case ESOCKTNOSUPPORT:
#endif
#ifdef WSAESOCKTNOSUPPORT
     case WSAESOCKTNOSUPPORT:
#endif
#ifdef EOPNOTSUPP
     case EOPNOTSUPP:
#endif
#ifdef WSAEOPNOTSUPP
     case WSAEOPNOTSUPP:
#endif
#ifdef EPFNOSUPPORT
     case EPFNOSUPPORT:
#endif
#ifdef WSAEPFNOSUPPORT
     case WSAEPFNOSUPPORT:
#endif
      DeeError_SetStringf(&DeeErrorType_NotImplemented,
                          "setsockopt(%k,%K,%K,%p,%Iu) : Unsupported option",
                          self,DeeSocket_OptLevelToString(level),p,s,
                          DeeSocket_OptNameToString(optname));
      break;
     default:
      DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                          "setsockopt(%k,%k,%k,%p,%Iu) : " DEESOCKET_PRINTFERROF,
                          self,DeeSocket_OptLevelToString(optname),
                          DeeSocket_OptNameToString(optname),p,s,
                          DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
      break;
    }
    return -1;
   }
#endif
  } break;
 }
#ifdef VALUE
 return 0;
#else
#define DEE_SOCKET_OPT_GENERIC_GETTER_MAX_SIZE 1024
 result_len = sizeof(int);
 if ((result = DeeString_NewSized(DEE_SOCKET_OPT_GENERIC_GETTER_MAX_SIZE)) == NULL) return NULL;
def_again:
 DeeSocket_USEBEGIN(self,sock);
 error = getsockopt(sock,level,optname,DeeString_STR(result),&result_len);
 DeeSocket_USEEND(self);
 if (error != 0) {
  error = (int)DeeNetError_GET(); DeeNetError_SET(0);
  switch (error) {
   case 0: break;

#ifdef EINVAL
   case EINVAL:
#endif
#ifdef WSAEINVAL
   case WSAEINVAL:
#endif
   {
    if DEE_LIKELY(result_len == 0) {
#ifdef EPROTOTYPE
   case EPROTOTYPE:
#endif
#ifdef WSAEPROTOTYPE
   case WSAEPROTOTYPE:
#endif
#ifdef ENOPROTOOPT
   case ENOPROTOOPT:
#endif
#ifdef WSAENOPROTOOPT
   case WSAENOPROTOOPT:
#endif
#ifdef EPROTONOSUPPORT
   case EPROTONOSUPPORT:
#endif
#ifdef WSAEPROTONOSUPPORT
   case WSAEPROTONOSUPPORT:
#endif
#ifdef ESOCKTNOSUPPORT
   case ESOCKTNOSUPPORT:
#endif
#ifdef WSAESOCKTNOSUPPORT
   case WSAESOCKTNOSUPPORT:
#endif
#ifdef EOPNOTSUPP
   case EOPNOTSUPP:
#endif
#ifdef WSAEOPNOTSUPP
   case WSAEOPNOTSUPP:
#endif
#ifdef EPFNOSUPPORT
   case EPFNOSUPPORT:
#endif
#ifdef WSAEPFNOSUPPORT
   case WSAEPFNOSUPPORT:
#endif
     Dee_DECREF(result);
     DeeError_SetStringf(&DeeErrorType_NotImplemented,
                         "getsockopt(%k,%K,%K): Unsupported options",
                         self,DeeSocket_OptLevelToString(level),
                         DeeSocket_OptNameToString(optname));
     return NULL;
    }
    if DEE_LIKELY(++result_len != sizeof(int)) {
     if DEE_UNLIKELY(result_len == 1024) result_len = 1;
     goto def_again;
    } else goto syserr;
   } break;

   default: goto syserr;
  }
 }
 if (result_len < DEE_SOCKET_OPT_GENERIC_GETTER_MAX_SIZE &&
     DeeString_Resize(&result,(size_t)result_len) != 0) Dee_CLEAR(result);
 return result;
#undef DEE_SOCKET_OPT_GENERIC_GETTER_MAX_SIZE
#endif
}

#ifdef val_dword_or_size_t
#undef val_dword_or_size_t
#endif
#ifdef val_ulong
#undef val_ulong
#endif
#ifdef val_uint
#undef val_uint
#endif
#ifdef VALUE
#undef VALUE
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

