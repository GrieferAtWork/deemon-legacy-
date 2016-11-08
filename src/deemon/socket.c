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
#ifndef GUARD_DEEMON_SOCKET_C
#define GUARD_DEEMON_SOCKET_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/fs_api.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/string_forward.h>
#include <deemon/runtime_flags.h>
#include <deemon/generic_vtable.h>
#include <deemon/socket.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/time.h>
#include <deemon/tuple.h>
#include <deemon/value.h>

// src/*
#include <deemon/__alloca.inl>
#include <deemon/__xconf.inl>
#include <deemon/posix_features.inl>
#include <deemon/unicode/char_traits.inl>


#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
DEE_COMPILER_MSVC_WARNING_PUSH(4820 4255 4668 4365 4574)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#pragma comment(lib,"Ws2_32.lib")
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_NETDB_H
#include <netdb.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H
#include <signal.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SO_H
#include <unistd.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H
#include <sys/time.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H
#include <sys/types.h>
#endif
#endif
DEE_COMPILER_MSVC_WARNING_PUSH(4820)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>
#endif
#include <deemon/optional/std/string.h>
#if DEE_ENVIRONMENT_HAVE_INCLUDE_LIMITS_H
#include <limits.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_BLUETOOTH_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sco.h>
#include <bluetooth/hci.h>
#else
#undef AF_BLUETOOTH
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_BLUETOOTH_H
#include <bluetooth.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_LINUX_NETLINK_H
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ASM_TYPES_H
#include <asm/types.h>
#endif
#include <linux/netlink.h>
#else
#undef AF_NETLINK
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_UN_H
#include <sys/un.h>
#else
#ifndef DEE_PLATFORM_WINDOWS
#undef AF_UNIX
#endif
#endif
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()




#if defined(__INTELLISENSE__)
// Syntax highlighting
//#define AF_NETLINK   43
//#define AF_BLUETOOTH 44
#define SO_BINDTODEVICE 12345
#define IF_NAMESIZE     16
#endif




#ifndef DEE_HAVE_AF_INET
# if defined(AF_INET)
#  define DEE_HAVE_AF_INET  1
# else
#  define DEE_HAVE_AF_INET  0
# endif
#endif

#ifndef DEE_HAVE_AF_INET6
# if defined(AF_INET6)
#  define DEE_HAVE_AF_INET6  1
# else
#  define DEE_HAVE_AF_INET6  0
# endif
#endif

#ifndef DEE_HAVE_AF_UNIX
# if defined(AF_UNIX)
#  define DEE_HAVE_AF_UNIX   1
# else
#  define DEE_HAVE_AF_UNIX   0
# endif
#endif

#ifndef DEE_HAVE_AF_NETLINK
# if defined(AF_NETLINK)
#  define DEE_HAVE_AF_NETLINK 1
# else
#  define DEE_HAVE_AF_NETLINK 0
# endif
#endif

#ifndef DEE_HAVE_AF_BLUETOOTH
# if defined(AF_BLUETOOTH)/* && \
    !defined(__NetBSD__) && \
    !defined(__DragonFly__)*/
#  define DEE_HAVE_AF_BLUETOOTH	1
# else
#  define DEE_HAVE_AF_BLUETOOTH	0
# endif
#endif

#define DEE_HAVE_AF_PACKET     0 // TODO
#define DEE_HAVE_AF_TIPC       0 // TODO
#define DEE_HAVE_AF_CAN        0 // TODO
#define DEE_HAVE_PF_SYSTEM     0 // TODO


#define DEE_HAVE_AF_IMPLINK    0 // TODO ?
#define DEE_HAVE_AF_PUP        0 // TODO ?
#define DEE_HAVE_AF_CHAOS      0 // TODO ?
#define DEE_HAVE_AF_NS         0 // TODO ?
#define DEE_HAVE_AF_IPX        0 // TODO ?
#define DEE_HAVE_AF_ISO        0 // TODO ?
#define DEE_HAVE_AF_OSI        0 // TODO ?
#define DEE_HAVE_AF_ECMA       0 // TODO ?
#define DEE_HAVE_AF_DATAKIT    0 // TODO ?
#define DEE_HAVE_AF_CCITT      0 // TODO ?
#define DEE_HAVE_AF_SNA        0 // TODO ?
#define DEE_HAVE_AF_DECnet     0 // TODO ?
#define DEE_HAVE_AF_DLI        0 // TODO ?
#define DEE_HAVE_AF_LAT        0 // TODO ?
#define DEE_HAVE_AF_HYLINK     0 // TODO ?
#define DEE_HAVE_AF_APPLETALK  0 // TODO ?
#define DEE_HAVE_AF_NETBIOS    0 // TODO ?
#define DEE_HAVE_AF_VOICEVIEW  0 // TODO ?
#define DEE_HAVE_AF_FIREFOX    0 // TODO ?
#define DEE_HAVE_AF_BAN        0 // TODO ?
#define DEE_HAVE_AF_ATM        0 // TODO ?
#define DEE_HAVE_AF_CLUSTER    0 // TODO ?
#define DEE_HAVE_AF_12844      0 // TODO ?
#define DEE_HAVE_AF_IRDA       0 // TODO ?
#define DEE_HAVE_AF_NETDES     0 // TODO ?
#define DEE_HAVE_AF_MAX        0 // TODO ?
#define DEE_HAVE_AF_TCNPROCESS 0 // TODO ?
#define DEE_HAVE_AF_TCNMESSAGE 0 // TODO ?
#define DEE_HAVE_AF_ICLFXBM    0 // TODO ?
#define DEE_HAVE_AF_BTH        0 // TODO ?
#define DEE_HAVE_AF_LINK       0 // TODO ?
 


#ifdef DEE_PLATFORM_WINDOWS


#if (_WIN32_WINNT >= 0x0501)
#define IPPROTO_HOPOPTS IPPROTO_HOPOPTS
#endif
#define IPPROTO_ICMP IPPROTO_ICMP
#define IPPROTO_IGMP IPPROTO_IGMP
#define IPPROTO_GGP IPPROTO_GGP
#if (_WIN32_WINNT >= 0x0501)
#define IPPROTO_IPV4 IPPROTO_IPV4
#endif
#if (_WIN32_WINNT >= 0x0600)
#define IPPROTO_ST IPPROTO_ST
#endif
#define IPPROTO_TCP IPPROTO_TCP
#if (_WIN32_WINNT >= 0x0600)
#define IPPROTO_CBT IPPROTO_CBT
#define IPPROTO_EGP IPPROTO_EGP
#define IPPROTO_IGP IPPROTO_IGP
#endif
#define IPPROTO_PUP IPPROTO_PUP
#define IPPROTO_UDP IPPROTO_UDP
#define IPPROTO_IDP IPPROTO_IDP
#if (_WIN32_WINNT >= 0x0600)
#define IPPROTO_RDP IPPROTO_RDP
#endif
#if (_WIN32_WINNT >= 0x0501)
#define IPPROTO_IPV6 IPPROTO_IPV6
#define IPPROTO_ROUTING IPPROTO_ROUTING
#define IPPROTO_FRAGMENT IPPROTO_FRAGMENT
#define IPPROTO_ESP IPPROTO_ESP
#define IPPROTO_AH IPPROTO_AH
#define IPPROTO_ICMPV6 IPPROTO_ICMPV6
#define IPPROTO_NONE IPPROTO_NONE
#define IPPROTO_DSTOPTS IPPROTO_DSTOPTS
#endif
#define IPPROTO_ND IPPROTO_ND
#if (_WIN32_WINNT >= 0x0501)
#define IPPROTO_ICLFXBM IPPROTO_ICLFXBM
#endif
#if (_WIN32_WINNT >= 0x0600)
#define IPPROTO_PIM IPPROTO_PIM
#define IPPROTO_PGM IPPROTO_PGM
#define IPPROTO_L2TP IPPROTO_L2TP
#define IPPROTO_SCTP IPPROTO_SCTP
#endif
#define IPPROTO_RAW IPPROTO_RAW


#ifndef IPPROTO_HOPOPTS
#define IPPROTO_HOPOPTS       0
#endif
#ifndef IPPROTO_ICMP
#define IPPROTO_ICMP          1
#endif
#ifndef IPPROTO_IGMP
#define IPPROTO_IGMP          2
#endif
#ifndef IPPROTO_GGP
#define IPPROTO_GGP           3
#endif
#ifndef IPPROTO_IPV4
#define IPPROTO_IPV4          4
#endif
#ifndef IPPROTO_ST
#define IPPROTO_ST            5
#endif
#ifndef IPPROTO_TCP
#define IPPROTO_TCP           6
#endif
#ifndef IPPROTO_CBT
#define IPPROTO_CBT           7
#endif
#ifndef IPPROTO_EGP
#define IPPROTO_EGP           8
#endif
#ifndef IPPROTO_IGP
#define IPPROTO_IGP           9
#endif
#ifndef IPPROTO_PUP
#define IPPROTO_PUP           12
#endif
#ifndef IPPROTO_UDP
#define IPPROTO_UDP           17
#endif
#ifndef IPPROTO_IDP
#define IPPROTO_IDP           22
#endif
#ifndef IPPROTO_RDP
#define IPPROTO_RDP           27
#endif
#ifndef IPPROTO_IPV6
#define IPPROTO_IPV6          41
#endif
#ifndef IPPROTO_ROUTING
#define IPPROTO_ROUTING       43
#endif
#ifndef IPPROTO_FRAGMENT
#define IPPROTO_FRAGMENT      44
#endif
#ifndef IPPROTO_ESP
#define IPPROTO_ESP           50
#endif
#ifndef IPPROTO_AH
#define IPPROTO_AH            51
#endif
#ifndef IPPROTO_ICMPV6
#define IPPROTO_ICMPV6        58
#endif
#ifndef IPPROTO_NONE
#define IPPROTO_NONE          59
#endif
#ifndef IPPROTO_DSTOPTS
#define IPPROTO_DSTOPTS       60
#endif
#ifndef IPPROTO_ND
#define IPPROTO_ND            77
#endif
#ifndef IPPROTO_ICLFXBM
#define IPPROTO_ICLFXBM       78
#endif
#ifndef IPPROTO_PIM
#define IPPROTO_PIM           103
#endif
#ifndef IPPROTO_PGM
#define IPPROTO_PGM           113
#endif
#ifndef IPPROTO_L2TP
#define IPPROTO_L2TP          115
#endif
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP          132
#endif
#ifndef IPPROTO_RAW
#define IPPROTO_RAW           255
#endif

#ifndef MSG_OOB
#define MSG_OOB  0x1
#endif
#ifndef MSG_PEEK
#define MSG_PEEK 0x2
#endif
#ifndef MSG_DONTROUTE
#define MSG_DONTROUTE 0x4
#endif
#ifndef MSG_WAITALL
#define MSG_WAITALL 0x8
#endif
#ifndef MSG_PUSH_IMMEDIATE
#define MSG_PUSH_IMMEDIATE 0x20
#endif
#ifndef MSG_PARTIAL
#define MSG_PARTIAL 0x8000
#endif
#ifndef MSG_INTERRUPT
#define MSG_INTERRUPT 0x10
#endif
#endif



#ifndef INADDR_ANY
#define INADDR_ANY 0
#endif
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK DEE_BUILTIN_NTOH32_M(DEE_HOSTNAME_LOCALHOST)
#endif
#ifndef INADDR_BROADCAST
#define INADDR_BROADCAST 0xFFFFFFFF
#endif
#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFF
#endif
#ifndef INADDR_UNSPEC_GROUP
#define INADDR_UNSPEC_GROUP 0xE0000000
#endif
#ifndef INADDR_ALLHOSTS_GROUP
#define INADDR_ALLHOSTS_GROUP 0xe0000001
#endif
#ifndef INADDR_MAX_LOCAL_GROUP
#define INADDR_MAX_LOCAL_GROUP 0xe00000ff
#endif
#ifndef IPPORT_RESERVED
#define IPPORT_RESERVED 1024
#endif
#ifndef IPPORT_USERRESERVED
#define IPPORT_USERRESERVED 5000
#endif
#ifndef IPPROTO_IP
#define IPPROTO_IP 0
#endif
#ifndef IPPROTO_ICMP
#define IPPROTO_ICMP 1
#endif
#ifndef IPPROTO_TCP
#define IPPROTO_TCP 6
#endif
#ifndef IPPROTO_UDP
#define IPPROTO_UDP 17
#endif
#ifndef IPPROTO_RAW
#define IPPROTO_RAW 255
#endif


DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
#define DeeSocket_PRINTF "%Iu"
typedef Dee_uintptr_t DeeSocketHandle;
#else
#define DeeSocket_PRINTF "%d"
typedef int DeeSocketHandle;
#endif


// If this fails, then math has failed us...
// ... or someone messed with the endian
DEE_STATIC_ASSERT(
    DEE_HOSTNAME_LOCALHOST ==
    DEE_SOCKADDRINET_HOST(127,0,0,1)
);

#ifdef DEE_PLATFORM_UNIX
#define closesocket    close
#define INVALID_SOCKET -1
#endif

#ifdef __INTELLISENSE__
typedef DeeSocketHandle sock_t;
#else
#undef sock_t
#define sock_t  DeeSocketHandle
#endif
#ifdef DEE_PLATFORM_WINDOWS
typedef u_short sa_family_t;
typedef Dee_uint16_t in_port_t;
#endif

#ifdef DEE_PLATFORM_WINDOWS
// Dunno why the windows headers didn't already do this...
#define SHUT_RD   SD_RECEIVE
#define SHUT_WR   SD_SEND
#define SHUT_RDWR SD_BOTH
#if DEE_HAVE_AF_UNIX
struct sockaddr_un {
 sa_family_t sun_family;    /* AF_UNIX */
 char        sun_path[108]; /* pathname */
};
#endif
#if DEE_HAVE_AF_NETLINK
struct sockaddr_nl {
 sa_family_t     nl_family;  /* AF_NETLINK */
 unsigned short  nl_pad;     /* Zero */
 pid_t           nl_pid;     /* Port ID */
#if 1
 Dee_uint32_t    nl_groups;  /* Multicast groups mask */
#else
 __u32           nl_groups;  /* Multicast groups mask */
#endif
};
#endif

#if DEE_HAVE_AF_BLUETOOTH && 0
DEE_COMPILER_STRUCTURE_PACKED_BEGIN
//typedef struct { Dee_uint8_t b[6]; } bdaddr_t;
DEE_COMPILER_STRUCTURE_PACKED_END
#define sockaddr_l2 sockaddr_l2_
struct sockaddr_l2_ {
 sa_family_t	   l2_family;
 unsigned short	l2_psm;
 bdaddr_t	      l2_bdaddr;
 unsigned short	l2_cid;
 Dee_uint8_t		  l2_bdaddr_type;
};
#define sockaddr_rc sockaddr_rc_
struct sockaddr_rc_ {
 sa_family_t	   rc_family;
 bdaddr_t	      rc_bdaddr;
 Dee_uint8_t		  rc_channel;
};
#define sockaddr_sco sockaddr_sco_
struct sockaddr_sco_ {
 sa_family_t	   sco_family;
 bdaddr_t	      sco_bdaddr;
};
#define sockaddr_hci sockaddr_hci_
struct sockaddr_hci_ {
 sa_family_t	   hci_family;
 unsigned short	hci_dev;
 unsigned short hci_channel;
};
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Enables runtime-checks for IPv6 dualstack support
// >> These determine whether or not the socket will prefer AF_INET6 over AF_INET
#ifndef DEE_HAVE_IPv6_DUALSTACK
#if DEE_HAVE_AF_INET6 && defined(IPPROTO_IPV6) && defined(IPV6_V6ONLY)
#define DEE_HAVE_IPv6_DUALSTACK 1
#else
#define DEE_HAVE_IPv6_DUALSTACK 0
#endif
#endif



#ifndef SHUT_RD
#define SHUT_RD   0
#endif
#ifndef SHUT_WR
#define SHUT_WR   1
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

#ifdef DEE_PLATFORM_WINDOWS
typedef DWORD Dee_neterrno_t;
#define DeeNetError_GET    (DWORD)WSAGetLastError
#define DeeNetError_SET(x) WSASetLastError((int)(x))
#elif 0
typedef int Dee_neterrno_t;
#define DeeNetError_GET()      h_errno
#define DeeNetError_SET(x)     (void)(h_errno=(x))
#define DEESOCKET_PRINTFERR    hstrerror
#define DEESOCKET_PRINTFERROF  "%s"
#endif

#ifndef DeeNetError_GET
typedef int Dee_neterrno_t;
#define DeeNetError_GET()        errno
#define DeeNetError_SET(x)       (void)(errno=(x))
#endif

#ifndef DEESOCKET_PRINTFERR
#ifdef DEE_PLATFORM_WINDOWS
#define DEESOCKET_PRINTFERR    DeeSystemError_Win32ToString
#else
#define DEESOCKET_PRINTFERR    DeeSystemError_ToString
#endif
#define DEESOCKET_PRINTFERROF "%K"
#endif

#if 0
static DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject *
DeeErrorType_NETERROR(DEE_A_IN Dee_neterrno_t code) {
 switch (code) {
  default: return &DeeErrorType_NetError;
 }
}
#else
#define DeeErrorType_NETERROR(code) &DeeErrorType_NetError
#endif

#define DeeNetError_CONSUME DeeNetError_CONSUME
DEE_STATIC_INLINE(Dee_neterrno_t) DeeNetError_CONSUME(void) {
 Dee_neterrno_t result = DeeNetError_GET();
 DeeNetError_SET(0);
 return result;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeSockAddr {
 union{
  struct sockaddr     sa;
#if DEE_HAVE_AF_INET
  struct sockaddr_in  sa_inet;
#endif
#if DEE_HAVE_AF_INET6
  struct sockaddr_in6 sa_inet6;
  struct sockaddr_storage storage;
#endif
#if DEE_HAVE_AF_UNIX
  struct sockaddr_un  sa_un;
#endif
#if DEE_HAVE_AF_NETLINK
  struct sockaddr_nl  sa_nl;
#endif
#if DEE_HAVE_AF_BLUETOOTH
 struct sockaddr_l2 bt_l2;
 struct sockaddr_rc bt_rc;
 struct sockaddr_sco bt_sco;
 struct sockaddr_hci bt_hci;
#endif
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define sa      _sa_data.sa
#if DEE_HAVE_AF_INET
#define sa_inet _sa_data.sa_inet
#endif
#if DEE_HAVE_AF_INET6
#define sa_inet6 _sa_data.sa_inet6
#define storage _sa_data.storage
#endif
#if DEE_HAVE_AF_UNIX
#define sa_un   _sa_data.sa_un
#endif
#if DEE_HAVE_AF_NETLINK
#define sa_nl   _sa_data.sa_nl
#endif
#if DEE_HAVE_AF_BLUETOOTH
#define bt_l2   _sa_data.bt_l2
#define bt_rc   _sa_data.bt_rc
#define bt_sco  _sa_data.bt_sco
#define bt_hci  _sa_data.bt_hci
#endif
 _sa_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP

#define DeeSockAddr_FAMILY(ob)     ((ob)->sa.sa_family)
#if DEE_HAVE_AF_INET
#define DeeSockAddr_INET_HOST(ob)  ((ob)->sa_inet.sin_addr.s_addr)
#define DeeSockAddr_INET_PORT(ob)  ((ob)->sa_inet.sin_port)
#endif
#if DEE_HAVE_AF_INET6
#define DeeSockAddr_INET6_HOST(ob) ((DeeINet6Host *)(ob)->sa_inet6.sin6_addr.s6_addr)
#define DeeSockAddr_INET6_PORT(ob) ((ob)->sa_inet6.sin6_port)
#endif
#if DEE_HAVE_AF_UNIX
#define DeeSockAddr_UNIX_PATH(ob)  ((ob)->sa_un.sun_path)
#endif
#if DEE_HAVE_AF_NETLINK
#define DeeSockAddr_NETLINK_PID(ob)    ((ob)->sa_nl.nl_pid)
#define DeeSockAddr_NETLINK_GROUPS(ob) ((ob)->sa_nl.nl_groups)
#endif

#if DEE_HAVE_AF_BLUETOOTH
#if defined(__FreeBSD__)
#define BTPROTO_HCI                   BLUETOOTH_PROTO_HCI
#define BTPROTO_L2CAP                 BLUETOOTH_PROTO_L2CAP
#define BTPROTO_RFCOMM                BLUETOOTH_PROTO_RFCOMM
#define HCI_FILTER                    SO_HCI_RAW_FILTER
#define SOL_HCI                       SOL_HCI_RAW
#define hci_dev                       hci_node
// -- sockaddr_rc --
#define sockaddr_rc                   sockaddr_rfcomm
#define rc_family                     rfcomm_family
#define rc_bdaddr                     rfcomm_bdaddr
#define rc_channel                    rfcomm_channel
// -- sockaddr_l2 --
#define sockaddr_l2                   sockaddr_l2cap
#define l2_family                     l2cap_family
#define l2_psm                        l2cap_psm
#define l2_bdaddr                     l2cap_bdaddr
#define l2_cid                        l2cap_cid
#define l2_bdaddr_type                l2cap_bdaddr_type
#elif defined(__NetBSD__) || defined(__DragonFly__)
#define HCI_DATA_DIR                  SO_HCI_DIRECTION
#define SOL_HCI                       BTPROTO_HCI
// -- sockaddr_hci --
#define sockaddr_hci                  sockaddr_bt
#define hci_family                    bt_family
#define hci_dev                       bt_dev
#define hci_channel                   bt_channel
// -- sockaddr_sco --
#define sco_family                    bt_family
#define sco_bdaddr                    bt_bdaddr
// -- sockaddr_rc --
#define sockaddr_rc                   sockaddr_bt
#define rc_family                     bt_family
#define rc_bdaddr                     bt_bdaddr
#define rc_channel                    bt_channel
// -- sockaddr_l2 --
#define sockaddr_l2                   sockaddr_bt
#define l2_family                     bt_family
#define l2_psm                        bt_psm
#define l2_bdaddr                     bt_bdaddr
#define l2_cid                        bt_cid
#define l2_bdaddr_type                bt_bdaddr_type
#endif
#endif



#if DEE_HAVE_AF_BLUETOOTH
#define DEE_SOCKET_SIZEOF_SOCKADDR_NEEDS_PROTOCOL 1
#define Dee_SizeofSockAddr _Dee_SizeofSockAddr
DEE_STATIC_INLINE(Dee_size_t) _Dee_SizeofSockAddr(
 struct DeeSockAddr const *saddr, int protocol)
#else
#define Dee_SizeofSockAddr(saddr,protocol) _Dee_SizeofSockAddr(saddr)
DEE_STATIC_INLINE(Dee_size_t) _Dee_SizeofSockAddr(struct DeeSockAddr const *saddr)
#endif
{
 switch (DeeSockAddr_FAMILY(saddr)) {
#if DEE_HAVE_AF_INET
  case AF_INET: return sizeof(struct sockaddr_in);
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6: return sizeof(struct sockaddr_in6);
#endif
#if DEE_HAVE_AF_UNIX
  case AF_UNIX: return sizeof(struct sockaddr_un);
#endif
#if DEE_HAVE_AF_NETLINK
  case AF_NETLINK: return sizeof(struct sockaddr_nl);
#endif
#if DEE_HAVE_AF_BLUETOOTH
  case AF_BLUETOOTH:
   switch (protocol) {
    case BTPROTO_L2CAP: return sizeof(struct sockaddr_l2);
    case BTPROTO_RFCOMM: return sizeof(struct sockaddr_rc);
    case BTPROTO_HCI: return sizeof(struct sockaddr_hci);
#if !defined(__FreeBSD__)
    case BTPROTO_SCO: return sizeof(struct sockaddr_sco);
#endif
    default: return sizeof(struct DeeSockAddr);
   }
#endif
  default: return sizeof(struct DeeSockAddr); // Try our luck with the actual size...
 }
}



#define _DeeSocket_InitCommonNoFamily(ob,flags,type,protocol)\
 (DeeAtomicMutex_Init(&(ob)->s_lock),\
 (ob)->s_uses=0,(ob)->s_flags=(flags),\
 (ob)->s_type=(type),(ob)->s_protocol=(protocol),\
 (ob)->s_file=NULL)
#define _DeeSocket_InitCommon(ob,flags,af,type,protocol)\
 (_DeeSocket_InitCommonNoFamily(ob,flags,type,protocol),\
 DeeSockAddr_FAMILY(&(ob)->s_sock_addr)=(sa_family_t)(af))
struct DeeSocketObject {
 DEE_OBJECT_HEAD
 struct DeeAtomicMutex               s_lock;      /*< Lock of the socket. */
 DeeSocketHandle                     s_socket;    /*< System-specific socket handle. */
 Dee_uint16_t                        s_uses;      /*< Reference counter for threads currently using this socket. */
 Dee_uint16_t                        s_flags;     /*< Flags describing the state of the socket. */
 struct DeeSockAddr                  s_sock_addr; /*< Locally bound address (Also contains the af-constructor-argument). */
 int                                 s_type;      /*< Socket type (constructor-argument). */
 int                                 s_protocol;  /*< Socket protocol (constructor-argument). */
 DEE_A_WEAK_REF DeeSocketFileObject *s_file;      /*< [0..1] io file of the socket (accessible with 'socket.file'). */
};

// Mask of publicly visible socket flags
#define DEE_SOCKET_FLAG_PUBLIC_MASK \
(DEE_SOCKET_FLAG_BINDING|DEE_SOCKET_FLAG_BOUND|\
 DEE_SOCKET_FLAG_CONNECTING|DEE_SOCKET_FLAG_CONNECTED|\
 DEE_SOCKET_FLAG_STARTLISTENING|DEE_SOCKET_FLAG_LISTENING|\
 DEE_SOCKET_FLAG_SHUTDOWN|DEE_SOCKET_FLAG_SHUTTINGDOWN|\
 DEE_SOCKET_FLAG_CLOSED)

#define DeeSocket_GET_ADDRESS_FAMILY(ob) DeeSockAddr_FAMILY(&((DeeSocketObject *)Dee_REQUIRES_POINTER(ob))->s_sock_addr)
#define DeeSocket_GET_TYPE(ob)           ((DeeSocketObject *)Dee_REQUIRES_POINTER(ob))->s_type
#define DeeSocket_GET_PROTOCOL(ob)       ((DeeSocketObject *)Dee_REQUIRES_POINTER(ob))->s_protocol

#define DeeSocket_LOCK(ob)    (&((DeeSocketObject *)Dee_REQUIRES_POINTER(ob))->s_lock)
#define DeeSocket_ACQUIRE(ob) DeeAtomicMutex_Acquire(DeeSocket_LOCK(ob))
#define DeeSocket_RELEASE(ob) DeeAtomicMutex_Release(DeeSocket_LOCK(ob))
#define DeeSocket_HANDLE(ob)  ((DeeSocketObject *)(ob))->s_socket
#define DeeSocket_USEBEGIN_LOCKED(ob,sock) \
do{\
 (sock) = ((DeeSocketObject *)(ob))->s_socket;\
 DeeAtomic16_IncFetch(((DeeSocketObject *)(ob))->s_uses,memory_order_seq_cst);\
}while(0)
#define DeeSocket_USEBEGIN(ob,sock) \
do{\
 DeeSocket_ACQUIRE(ob);\
 (sock) = ((DeeSocketObject *)(ob))->s_socket;\
 DeeAtomic16_IncFetch(((DeeSocketObject *)(ob))->s_uses,memory_order_seq_cst);\
 DeeSocket_RELEASE(ob);\
}while(0)
#define DeeSocket_USEEND(ob) DeeAtomic16_DecFetch(((DeeSocketObject *)Dee_REQUIRES_POINTER(ob))->s_uses,memory_order_seq_cst)
#define DeeSocket_FILE(ob)   ((DeeObject *)((DeeSocketObject *)(ob))->s_file)

struct DeeSocketFileObject {
 DEE_FILE_OBJECT_HEAD
 DEE_A_REF DeeSocketObject *sf_socket; /*< [1..1] Keep-alive reference to the associated socket. */
};
struct DeeSocketFileToObject {
 DEE_FILE_OBJECT_HEAD
 DEE_A_REF DeeSocketObject *sft_socket; /*< [1..1] Keep-alive reference to the associated socket. */
 struct DeeSockAddr         sft_peer_addr;
};
struct DeeSockAddrObject {
 DEE_OBJECT_HEAD
 struct DeeSockAddr sa_addr;
};




int DeeSocket_DefaultMaxBacklog = DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG;
extern void _DeeSocket_SetDefaultMaxBacklogString(char const *value);
void _DeeSocket_SetDefaultMaxBacklogString(char const *value) {
 int new_size;
 if (!*value) new_size = DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG;
 else if DEE_UNLIKELY(DeeString_ToNumber(int,value,&new_size) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG
                 "' (defaulting to '" DEE_PP_STR(DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG) "')\n",1);
  new_size = DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG;
 }
 DEE_LVERBOSE1("Setting: 'DeeSocket_DefaultMaxBacklog = %d;'\n",new_size);
 DeeAtomicInt_Store(DeeSocket_DefaultMaxBacklog,new_size,memory_order_seq_cst);
}

DEE_A_RET_WUNUSED int DeeSocket_GetDefaultMaxBacklog(void) {
 DEE_ATOMIC_ONCE({
  DeeObject *envvar;
  if DEE_UNLIKELY((envvar = DeeFS_TryGetEnv(DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG)) != NULL) {
   _DeeSocket_SetDefaultMaxBacklogString(DeeString_STR(envvar));
   Dee_DECREF(envvar);
  } else DeeAtomicInt_Store(DeeSocket_DefaultMaxBacklog,DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG,memory_order_seq_cst);
 });
 return DeeAtomicInt_Load(DeeSocket_DefaultMaxBacklog,memory_order_seq_cst);
}




static int _DeeSocket_InitApi(void) {
 DEE_LVERBOSE1("Initializing socket API\n");
 {
#ifdef DEE_PLATFORM_WINDOWS
  Dee_neterrno_t error; WSADATA wsaData; // Start up the windows networking
  // Use 1.1, for maximum backwards compatibility
  error = (Dee_neterrno_t)WSAStartup(MAKEWORD(1,1),&wsaData);
  if DEE_UNLIKELY(error != 0) {
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "WSAStartup() : " DEESOCKET_PRINTFERROF,
                       DEESOCKET_PRINTFERR(error));
   return -1;
  }
#else
  // SIGPIPE is generated when a remote socket is closed.
  void (*handler)(int);
  // Try to ignore this signal.
  handler = signal(SIGPIPE,SIG_IGN);
  // Make sure we don't override a custom handler
  if (handler != SIG_DFL) signal(SIGPIPE,handler);
#endif
  return 0;
 }
}
static void _DeeSocket_QuitApi(void) {
 DEE_LVERBOSE1("Finalizing socket API\n");
 {
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY(WSACleanup() != 0) WSASetLastError(0);
#endif
 }
}

static struct DeeAtomicMutex _deesocket_instance_counter_lock = DeeAtomicMutex_INIT();
static Dee_uint32_t _deesocket_instance_counter = 0;
#define DEESOCKET_API_BEGIN_NONFIRST()\
do{\
 DeeAtomicMutex_AcquireRelaxed(&_deesocket_instance_counter_lock);\
 DEE_ASSERT(_deesocket_instance_counter != 0);\
 ++_deesocket_instance_counter;\
 DeeAtomicMutex_Release(&_deesocket_instance_counter_lock);\
}while(0)
#define DEESOCKET_API_BEGIN(on_err)\
do{\
 DeeAtomicMutex_AcquireRelaxed(&_deesocket_instance_counter_lock);\
 if DEE_UNLIKELY(_deesocket_instance_counter++ == 0) {\
  /* TODO: Possible deadlock if '_DeeSocket_InitApi' calls DEE_A_EXEC code. */\
  if DEE_UNLIKELY(_DeeSocket_InitApi() != 0) {\
   --_deesocket_instance_counter;\
   DeeAtomicMutex_Release(&_deesocket_instance_counter_lock);\
   on_err;\
   DEE_BUILTIN_UNREACHABLE_();\
  }\
 }\
 DeeAtomicMutex_Release(&_deesocket_instance_counter_lock);\
}while(0)

#define DEESOCKET_API_END_NONLAST()\
do{\
 DeeAtomicMutex_AcquireRelaxed(&_deesocket_instance_counter_lock);\
 --_deesocket_instance_counter;\
 DeeAtomicMutex_Release(&_deesocket_instance_counter_lock);\
}while(0)
#define DEESOCKET_API_END()\
do{\
 DeeAtomicMutex_AcquireRelaxed(&_deesocket_instance_counter_lock);\
 /* TODO: Possible deadlock if '_DeeSocket_QuitApi' calls DEE_A_EXEC code. */\
 if (!--_deesocket_instance_counter) _DeeSocket_QuitApi();\
 DeeAtomicMutex_Release(&_deesocket_instance_counter_lock);\
}while(0)


DEE_A_RET_OBJECT_EXCEPT_REF(DeeSockAddrObject) *
_DeeSockAddrObject_New(void) {
 DeeSockAddrObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSockAddrObject)) != NULL)
  DeeObject_INIT(result,&DeeSockAddr_Type);
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSockAddrObject) *
DeeSockAddrObject_New(DEE_A_IN struct DeeSockAddr const *addr) {
 DeeSockAddrObject *result;
 DEE_ASSERT(addr);
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSockAddrObject)) != NULL) {
  DeeObject_INIT(result,&DeeSockAddr_Type);
  result->sa_addr = *addr;
 }
 return (DeeObject *)result;
}
DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED struct DeeSockAddr *
DeeSockAddrObject_Addr(DEE_A_IN_OBJECT(DeeSockAddrObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSockAddr_Check(self));
 return &((DeeSockAddrObject *)self)->sa_addr;
}

//////////////////////////////////////////////////////////////////////////
// Socket Address API
DEE_A_RET_EXCEPT(NULL) struct DeeSockAddr *DeeSockAddr_Alloc(void) {
 struct DeeSockAddr *result;
 while DEE_UNLIKELY((result = (struct DeeSockAddr *)malloc_nz(
  sizeof(struct DeeSockAddr))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
void DeeSockAddr_Free(DEE_A_IN struct DeeSockAddr *self) {
 DEE_ASSERT(self);
 free_nn(self);
}

#if DEE_HAVE_AF_INET
#define _DeeSockAddr_InitINET(self,host,port)\
do{\
 memset((self),0,sizeof(struct DeeSockAddr));\
 DeeSockAddr_FAMILY(self) = AF_INET;\
 DeeSockAddr_INET_HOST(self) = (host);\
 DeeSockAddr_INET_PORT(self) = (port);\
}while(0)
#endif

#if DEE_HAVE_AF_INET6
#define _DeeSockAddr_InitINET6(self,host,port)\
do{\
 memset((self),0,sizeof(struct DeeSockAddr));\
 DeeSockAddr_FAMILY(self) = AF_INET6;\
 *DeeSockAddr_INET6_HOST(self) = *(host);\
 DeeSockAddr_INET6_PORT(self) = (port);\
}while(0)
#endif


DEE_A_RET_EXCEPT(-1) int DeeSockAddr_InitINET(
 DEE_A_OUT struct DeeSockAddr *self,
 DEE_A_IN Dee_uint32_t host, DEE_A_IN Dee_uint16_t port) {
 DEE_ASSERT(self);
#if DEE_HAVE_AF_INET
 _DeeSockAddr_InitINET(self,host,port);
 return 0;
#else
 (void)self,host,port;
 DeeError_NotImplemented_str("AF_INET");
 return -1;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeSockAddr_InitINET6(
 DEE_A_OUT struct DeeSockAddr *self,
 DEE_A_IN DeeINet6Host const *host,
 DEE_A_IN Dee_uint16_t port) {
 DEE_ASSERT(self && host);
#if DEE_HAVE_AF_INET6
 _DeeSockAddr_InitINET6(self,host,port);
 return 0;
#else
 (void)self,host,port;
 DeeError_NotImplemented_str("AF_INET6");
 return -1;
#endif
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6255 6263)
DEE_A_RET_EXCEPT(-1) int DeeSockAddr_InitFromString(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af, DEE_A_IN_Z char const *addr) {
 char *buffer,chr; char const *port_begin,*port_end,*used_port_begin;
 Dee_size_t hostlen; int result; Dee_uint16_t port; int bracket_recursion;
 port_end = (port_begin = addr)+strlen(addr);
 switch (af) {
  case DEE_AF_AUTO:
#ifdef AF_RDS
  case AF_RDS:
#endif
#if DEE_HAVE_AF_INET
  case AF_INET:
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6:
#endif
   bracket_recursion = 0;
   while DEE_LIKELY(port_begin != port_end) {
    chr = *port_begin++;
    if (chr == '[') ++bracket_recursion;
    else if (chr == ']') --bracket_recursion;
    else if (chr == ':' && bracket_recursion == 0) {
     used_port_begin = port_begin;
     while (used_port_begin < port_end && DEE_CH_IS_SPACE(*used_port_begin)) ++used_port_begin;
     if DEE_UNLIKELY(DeeString_ToUInt16WithLength((Dee_size_t)(
      port_end-used_port_begin),used_port_begin,&port) != 0) return -1;
     hostlen = ((Dee_size_t)(port_begin-addr))-1;
#if DEE_HAVE_ALLOCA
     buffer = (char *)alloca((hostlen+1)*sizeof(char));
#else
     while DEE_UNLIKELY((buffer = (char *)malloc_nz((hostlen+1)*sizeof(char))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      DeeError_NoMemory();
      return -1;
     }
#endif
     memcpy(buffer,addr,hostlen);
     buffer[hostlen] = '\0';
     result = DeeSockAddr_InitFromStringAndPort(self,af,buffer,port);
#if !DEE_HAVE_ALLOCA
     free_nn(buffer);
#endif
     return result;
    }
   }
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Address string does not contain a port: %q",addr);
   return -1;
  default:
   DeeError_SetStringf(&DeeErrorType_NotImplemented,
                       "Unsupported Address Family: %K",
                       DeeSocket_AddressFamilyToString(af));
   return -1;
 }
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_EXCEPT(-1) int DeeSockAddr_InitFromStringAndPort(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af,
 DEE_A_IN_Z char const *host, DEE_A_IN Dee_uint16_t port) {
 int error;
 switch (af) {
  case DEE_AF_AUTO:
#if DEE_HAVE_AF_INET6
  case AF_INET6: {
   char port_buf[32];
   struct addrinfo *info;
   DEESOCKET_API_BEGIN(return -1);
   sprintf(port_buf,"%u",(unsigned)port);
   if DEE_UNLIKELY((error = getaddrinfo(host,port_buf,NULL,&info)) != 0) {
#ifdef EAI_SYSTEM
    if (error == EAI_SYSTEM) {
#ifdef DEE_PLATFORM_WINDOWS
     error = DeeSystemError_Win32Consume();
#else
     error = DeeSystemError_Consume();
#endif
     DeeError_SetStringf(DeeErrorType_NETERROR(error),
                         "getaddrinfo(%q) : %K",host,
#ifdef DEE_PLATFORM_WINDOWS
                         DeeSystemError_Win32ToString(error)
#else
                         DeeSystemError_ToString(error)
#endif
                         );
    } else
#endif
    {
#ifdef DEE_PLATFORM_WINDOWS
     // Windows has 'gai_strerror', but if you want to know the
     // problem with it and why this works better, just look at
     // its implementation in <WS2tcpip.h> ...
     DeeError_SetStringf(DeeErrorType_NETERROR(error),
                         "getaddrinfo(%q) : %K",
                         host,DeeSystemError_Win32ToString((DWORD)error));
#else
     DeeError_SetStringf(DeeErrorType_NETERROR(error),
                         "getaddrinfo(%q) : %#.8x : %s",
                         host,error,gai_strerror(error));
#endif
    }
    DEESOCKET_API_END();
    return -1;
   }
#if 1 // Is this correct, or even necessary?
   while (!info->ai_addr && info->ai_next) info = info->ai_next;
#endif
   if DEE_UNLIKELY(!info->ai_addr) {
    DeeError_SetStringf(&DeeErrorType_NetError,
                        "Didn't find an address at: %q (port: %I16u)",
                        host,port);
    error = -1;
   } else if DEE_UNLIKELY(info->ai_addrlen > sizeof(struct DeeSockAddr)) {
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
                        "Address family %K for %q (port: %I16u) is too big (%Iu exceeds %Iu)",
                        DeeSocket_AddressFamilyToString(info->ai_addr->sa_family),host,port,
                        (Dee_size_t)info->ai_addrlen,sizeof(struct DeeSockAddr));
    error = -1;
   } else {
    memset(self,0,sizeof(struct DeeSockAddr));
    memcpy(self,info->ai_addr,info->ai_addrlen);
    error = 0;
   }
   freeaddrinfo(info);
   DEESOCKET_API_END();
   return error;
  }
#endif
#if DEE_HAVE_AF_INET
#ifdef AF_RDS
  case AF_RDS:
#endif
  case AF_INET: {
   struct hostent *hp;
   DEESOCKET_API_BEGIN(return -1);
   if DEE_UNLIKELY((hp = (struct hostent *)gethostbyname(host)) == NULL) {
    Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
    DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                        "gethostbyname(%q) : " DEESOCKET_PRINTFERROF,
                        host,DEESOCKET_PRINTFERR(neterror));
    DEESOCKET_API_END();
    return -1;
   }
   DeeSockAddr_FAMILY(self) = (sa_family_t)af;
   DeeSockAddr_INET_HOST(self) = *(Dee_uint32_t *)hp->h_addr;
   DeeSockAddr_INET_PORT(self) = DEE_BUILTIN_HTON16(port);
   DEESOCKET_API_END();
   return 0;
  }
#endif

  default:
   DeeError_SetStringf(&DeeErrorType_NotImplemented,
                       "Unsupported address family: %K",
                       DeeSocket_AddressFamilyToString(af));
   return -1;
 }
}
DEE_A_RET_WUNUSED int DeeSockAddr_Compare(
 DEE_A_IN struct DeeSockAddr const *lhs,
 DEE_A_IN struct DeeSockAddr const *rhs) {
 if DEE_UNLIKELY(DeeSockAddr_FAMILY(lhs) != DeeSockAddr_FAMILY(rhs)) return 0;
 switch (DeeSockAddr_FAMILY(lhs)) {
#if DEE_HAVE_AF_INET
#ifdef AF_RDS
  case AF_RDS:
#endif
  case AF_INET:
   if (DeeSockAddr_INET_HOST(lhs) == DeeSockAddr_INET_HOST(rhs)) 
    return (int)DeeSockAddr_INET_PORT(lhs)-(int)DeeSockAddr_INET_PORT(rhs);
   else if (DeeSockAddr_INET_HOST(lhs) < DeeSockAddr_INET_HOST(rhs)) 
    return -1;
   else return 1;
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6: {
   int temp = memcmp(DeeSockAddr_INET6_HOST(lhs),DeeSockAddr_INET6_HOST(rhs),16);
   if (temp != 0) return temp;
   return (int)DeeSockAddr_INET6_PORT(lhs)-(int)DeeSockAddr_INET6_PORT(rhs);
  }
#endif
#if DEE_HAVE_AF_UNIX
  case AF_UNIX:
   return strncmp(
    DeeSockAddr_UNIX_PATH(lhs),
    DeeSockAddr_UNIX_PATH(rhs),
    sizeof(lhs->sa_un.sun_path));
#endif
#if DEE_HAVE_AF_NETLINK
  case AF_NETLINK:
   if (DeeSockAddr_NETLINK_PID(lhs) == DeeSockAddr_NETLINK_PID(rhs)) 
    return (int)DeeSockAddr_NETLINK_GROUPS(lhs)-(int)DeeSockAddr_NETLINK_GROUPS(rhs);
   else if (DeeSockAddr_NETLINK_PID(lhs) < DeeSockAddr_NETLINK_PID(rhs)) 
    return -1;
   else return 1;
#endif

  default: break;
 }
 return lhs < rhs ? -1 : lhs == rhs ? 0 : 1;
}


#if DEE_HAVE_AF_BLUETOOTH
static DEE_A_RET_EXCEPT(-1) int _dee_stobdaddr(char *name, bdaddr_t *bdaddr) {
 unsigned int b0,b1,b2,b3,b4,b5; char ch; int n;
 n = sscanf(name,"%X:%X:%X:%X:%X:%X%c",&b5,&b4,&b3,&b2,&b1,&b0,&ch);
 if (n == 6 && (b0|b1|b2|b3|b4|b5) < 256) {
  bdaddr->b[0] = b0;
  bdaddr->b[1] = b1;
  bdaddr->b[2] = b2;
  bdaddr->b[3] = b3;
  bdaddr->b[4] = b4;
  bdaddr->b[5] = b5;
  return 0;
 } else {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid bluetooth address: %q",name);
  return -1;
 }
}
#endif


DEE_A_RET_EXCEPT(-1) int DeeSockAddr_InitFromArgv(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af, DEE_A_IN int protocol,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject *const *argv) {
 DeeObject *arg0;
 DEE_ASSERT(self);
 DEE_ASSERT(!argc || argv);
 (void)protocol; // Most of the time unused
#ifdef DEE_DEBUG
 {DeeObject *const*iter,*const *end;
  end = (iter = argv)+argc;
  while (iter != end) { DEE_ASSERT(DeeObject_Check(*iter)); ++iter; }}
#endif
 if (argc == 1 && DeeSockAddr_Check(argv[0])) {
  arg0 = argv[0]; // Single argument is a sockaddr
  if DEE_LIKELY(af == DEE_AF_AUTO || af == DeeSockAddr_FAMILY(
   &((DeeSockAddrObject *)arg0)->sa_addr)) {
   *self = ((DeeSockAddrObject *)arg0)->sa_addr;
   return 0;
  } else {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Unexpected Address Family: %K (wanted: %K; got: %k)",
                       DeeSocket_AddressFamilyToString(DeeSockAddr_FAMILY(&((DeeSockAddrObject *)arg0)->sa_addr)),
                       DeeSocket_AddressFamilyToString(af),arg0);
   return -1;
  }
 }
 DeeSockAddr_FAMILY(self) = (sa_family_t)af;
 switch (af) {
  case DEE_AF_AUTO:
#ifdef AF_RDS
  case AF_RDS:
#endif
#if DEE_HAVE_AF_INET
  case AF_INET:
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6:
#endif
  {
   Dee_uint16_t port;
#if DEE_HAVE_AF_INET
   Dee_uint32_t host;
   Dee_uint8_t a,b,c,d;
#endif
   switch (argc) {
    case 1:
     arg0 = argv[0];
     // (string address)
     // ((Dee_uint32_t,Dee_uint16_t) address)
#if DEE_HAVE_AF_INET
     if (DeeTuple_Check(arg0) && DeeTuple_SIZE(arg0) == 2 &&
        (af == DEE_AF_AUTO || af == AF_INET)) {
      if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,DeeTuple_GET(arg0,0),&host) != 0 ||
                      DeeObject_Cast(Dee_uint16_t,DeeTuple_GET(arg0,1),&port) != 0) return -1;
      _DeeSockAddr_InitINET(self,DEE_BUILTIN_HTON32(host),DEE_BUILTIN_HTON16(port));
      return 0;
     } else
#endif
     {
      if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(arg0,&DeeString_Type) != 0) return -1;
      return DeeSockAddr_InitFromString(self,af,DeeString_STR(arg0));
     }
     break;
    case 2:
     // (string address, Dee_uint16_t port)
     // (Dee_uint32_t host, Dee_uint16_t port)
     if DEE_UNLIKELY(DeeObject_Cast(Dee_uint16_t,argv[1],&port) != 0) return -1;
     arg0 = argv[0];
#ifdef _DeeSockAddr_InitINET
     if ((af == DEE_AF_AUTO || af == AF_INET) && !DeeString_Check(arg0)) {
      if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,arg0,&host) != 0) return -1;
      _DeeSockAddr_InitINET(self,DEE_BUILTIN_HTON32(host),DEE_BUILTIN_HTON16(port));
      return 0;
     } else
#endif
     {
      if (DeeError_TypeError_CheckTypeExact(arg0,&DeeString_Type) != 0) return -1;
      return DeeSockAddr_InitFromStringAndPort(self,af,DeeString_STR(arg0),port);
     }
#if DEE_HAVE_AF_INET
    case 5:
     // (Dee_uint8_t a, Dee_uint8_t b, Dee_uint8_t c, Dee_uint8_t d, Dee_uint16_t port)
     if (af == DEE_AF_AUTO || af == AF_INET) {
      if DEE_UNLIKELY(DeeObject_Cast(Dee_uint8_t,argv[0],&a) != 0 ||
                      DeeObject_Cast(Dee_uint8_t,argv[1],&b) != 0 ||
                      DeeObject_Cast(Dee_uint8_t,argv[2],&c) != 0 ||
                      DeeObject_Cast(Dee_uint8_t,argv[3],&d) != 0 ||
                      DeeObject_Cast(Dee_uint16_t,argv[4],&port) != 0) return -1;
      _DeeSockAddr_InitINET(self,DEE_SOCKADDRINET_HOST(a,b,c,d),DEE_BUILTIN_HTON16(port));
      return 0;
     }
     DEE_ATTRIBUTE_FALLTHROUGH
#endif
    default:
     DeeError_SetStringf(&DeeErrorType_TypeError,
#if DEE_HAVE_AF_INET
                         "Address Family '%K' requires 1,2 or 5 arguments (%Iu given)",
#elif DEE_HAVE_AF_INET6
                         "Address Family '%K' requires 1 or 2 arguments (%Iu given)",
#else
                         "Address Family '%K' requires 1 or 2 arguments (%Iu given)",
#endif
                         DeeSocket_AddressFamilyToString(af),argc);
     break;
   }
  } break;

#if DEE_HAVE_AF_UNIX
  case AF_UNIX:
   if DEE_LIKELY(argc == 1) {
    arg0 = argv[0];
    if DEE_UNLIKELY((arg0 = DeeString_Cast(arg0)) == NULL) return -1;
    // v Note the '>='. That is on purpose as we need 'DeeString_SIZE(arg0)+1'
    //   bytes for the string and its terminating \0 character
    if DEE_UNLIKELY(DeeString_SIZE(arg0) >= sizeof(((struct sockaddr_un *)NULL)->sun_path)/sizeof(char)) {
     DeeError_SetStringf(&DeeErrorType_TypeError,
                         "Given path for 'AF_UNIX' is too long: %r",arg0);
     Dee_DECREF(arg0);
     return -1;
    }
    memset(&self->sa_un,0,sizeof(struct sockaddr_un));
    self->sa_un.sun_family = AF_UNIX;
    memcpy(self->sa_un.sun_path,DeeString_STR(arg0),DeeString_SIZE(arg0));
    Dee_DECREF(arg0);
   } else {
    DeeError_SetStringf(&DeeErrorType_TypeError,
                        "Address Family 'AF_UNIX' requires 1 argument (%Iu given)",
                        argc);
    return -1;
   }
   break;
#endif

#if DEE_HAVE_AF_NETLINK
  case AF_NETLINK: {
   switch (argc) {
    // TODO: String version
    case 2:
     memset(&self->sa_nl,0,sizeof(struct sockaddr_nl));
     self->sa_nl.nl_family = AF_NETLINK;
     if DEE_UNLIKELY(DeeObject_Cast(pid_t,argv[0],&self->sa_nl.nl_pid) != 0 ||
                     DeeObject_Cast(__u32,argv[1],&self->sa_nl.nl_groups) != 0) return -1;
     break;
    default:
     DeeError_SetStringf(&DeeErrorType_TypeError,
                         "Address Family 'AF_NETLINK' requires 2 argument (%Iu given)",
                         argc);
     return -1;
   }
  } break;
#endif

#if DEE_HAVE_AF_BLUETOOTH
  case AF_BLUETOOTH: {
   switch (protocol) {
    case BTPROTO_L2CAP: {
     if DEE_UNLIKELY(argc != 2) {
      DeeError_SetStringf(&DeeErrorType_TypeError,
                          "Address Family 'AF_BLUETOOTH/BTPROTO_L2CAP' requires 2 argument (%Iu given)",
                          argc);
      return -1;
     }
     memset(&self->bt_l2,0,sizeof(struct sockaddr_l2));
     self->bt_l2.l2_family = AF_BLUETOOTH;
     if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
     if DEE_UNLIKELY(DeeObject_Cast(ushort,argv[1],&self->bt_l2.l2_psm) != 0) return -1;
     if DEE_UNLIKELY(_dee_stobdaddr(DeeString_STR(argv[0]),&self->bt_l2.l2_bdaddr) != 0) return -1;
    } break;
    case BTPROTO_RFCOMM: {
     if DEE_UNLIKELY(argc != 2) {
      DeeError_SetStringf(&DeeErrorType_TypeError,
                          "Address Family 'AF_BLUETOOTH/BTPROTO_RFCOMM' requires 2 argument (%Iu given)",
                          argc);
      return -1;
     }
     memset(&self->bt_rc,0,sizeof(struct sockaddr_rc));
     self->bt_rc.rc_family = AF_BLUETOOTH;
     if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
     if DEE_UNLIKELY(DeeObject_Cast(ushort,argv[1],&self->bt_rc.rc_channel) != 0) return -1;
     if DEE_UNLIKELY(_dee_stobdaddr(DeeString_STR(argv[0]),&self->bt_rc.rc_bdaddr) != 0) return -1;
    } break;
    case BTPROTO_HCI: {
     if DEE_UNLIKELY(argc != 1) {
      DeeError_SetStringf(&DeeErrorType_TypeError,
                          "Address Family 'AF_BLUETOOTH/BTPROTO_HCI' requires 1 argument (%Iu given)",
                          argc);
      return -1;
     }
     memset(&self->bt_hci,0,sizeof(struct sockaddr_hci));
     self->bt_hci.hci_family = AF_BLUETOOTH;
#if defined(__NetBSD__) || defined(__DragonFly__)
     if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
     if DEE_UNLIKELY(setbdaddr(DeeString_StR(argv[0]),&self->bt_hci.hci_bdaddr) != 0) return -1;
#else
     if DEE_UNLIKELY(DeeObject_Cast(ushort,argv[0],&self->bt_hci.hci_dev) != 0) return -1;
#endif
    } break;

#if !defined(__FreeBSD__)
    case BTPROTO_SCO: {
     if DEE_UNLIKELY(argc != 1) {
      DeeError_SetStringf(&DeeErrorType_TypeError,
                          "Address Family 'AF_BLUETOOTH/BTPROTO_SCO' requires 1 argument (%Iu given)",
                          argc);
      return -1;
     }
     memset(&self->bt_sco,0,sizeof(struct sockaddr_sco));
     self->bt_sco.sco_family = AF_BLUETOOTH;
     if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(argv[0],&DeeString_Type) != 0) return -1;
     if DEE_UNLIKELY(_dee_stobdaddr(DeeString_STR(argv[0]),&self->bt_sco.sco_bdaddr) != 0) return -1;
    } break;
#endif

    default:
     DeeError_SetStringf(&DeeErrorType_TypeError,
                         "Invalid Protocol for 'AF_BLUETOOTH': %K",
                         DeeSocket_ProtocolToString(protocol));
     return -1;
   }
  } break;
#endif


  default:
   DeeError_SetStringf(&DeeErrorType_NotImplemented,
                       "Unsupported Address Family: %K",
                       DeeSocket_AddressFamilyToString(af));
   return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSocket_INETHostname(
 DEE_A_IN Dee_uint32_t host, DEE_A_IN Dee_uint32_t flags) {
#if !DEE_HAVE_AF_INET
 if DEE_UNLIKELY((flags&(DEE_SOCKADDR_STR_FLAG_NOFAIL|DEE_SOCKADDR_STR_FLAG_NODNS)) == 0) {
  DeeError_NotImplemented_str("AF_INET");
  return NULL;
 }
#else
 struct hostent *hp; DeeObject *result; Dee_neterrno_t error;
 if ((flags&DEE_SOCKADDR_STR_FLAG_NODNS)!=0) goto nodns;
 DEESOCKET_API_BEGIN(return NULL);
 if DEE_UNLIKELY((hp = (struct hostent *)gethostbyaddr((
  char const *)&host,sizeof(Dee_uint32_t),AF_INET)) == NULL) {
  if DEE_LIKELY((flags&DEE_SOCKADDR_STR_FLAG_NOFAIL)!=0) {
   DeeNetError_SET(0);
   goto raw_ip;
  }
  error = DeeNetError_GET();
  DeeNetError_SET(0);
  DeeError_SetStringf(DeeErrorType_NETERROR(error),
                      "gethostbyaddr(\"%I8u.%I8u.%I8u.%I8u\") : "
                      DEESOCKET_PRINTFERROF,
                      DEE_SOCKET_INET_HOSTNAME_A(host),
                      DEE_SOCKET_INET_HOSTNAME_B(host),
                      DEE_SOCKET_INET_HOSTNAME_C(host),
                      DEE_SOCKET_INET_HOSTNAME_D(host),
                      DEESOCKET_PRINTFERR(error));
  DEESOCKET_API_END();
  return NULL;
 }
 if DEE_UNLIKELY(!hp->h_name) {
raw_ip:
  DEESOCKET_API_END();
nodns:
#endif
  return DeeString_Newf("%I8u.%I8u.%I8u.%I8u",
                        DEE_SOCKET_INET_HOSTNAME_A(host),
                        DEE_SOCKET_INET_HOSTNAME_B(host),
                        DEE_SOCKET_INET_HOSTNAME_C(host),
                        DEE_SOCKET_INET_HOSTNAME_D(host));
#if DEE_HAVE_AF_INET
 }
 result = DeeString_New(hp->h_name);
 DEESOCKET_API_END();
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSocket_INET6Hostname(
 DEE_A_IN DeeINet6Host const *host, DEE_A_IN Dee_uint32_t flags) {
#if !DEE_HAVE_AF_INET6
 if DEE_UNLIKELY((flags&(DEE_SOCKADDR_STR_FLAG_NOFAIL|DEE_SOCKADDR_STR_FLAG_NODNS)) == 0) {
  DeeError_NotImplemented_str("AF_INET6");
  return NULL;
 }
#else
 struct hostent *hp; DeeObject *result;
 DEE_ASSERT(host);
 if ((flags&DEE_SOCKADDR_STR_FLAG_NODNS)!=0) goto nodns;
 DEESOCKET_API_BEGIN(return NULL);
 if DEE_UNLIKELY((hp = (struct hostent *)gethostbyaddr((
  char const *)&host,sizeof(Dee_uint32_t),AF_INET6)) == NULL) {
  Dee_neterrno_t error;
  if DEE_LIKELY((flags&DEE_SOCKADDR_STR_FLAG_NOFAIL)!=0) {
   DeeNetError_SET(0);
   goto raw_ip;
  }
  error = DeeNetError_GET();
  DeeNetError_SET(0);
  DeeError_SetStringf(DeeErrorType_NETERROR(error),
                      "gethostbyaddr(\"%I16u:%I16u:%I16u:%I16u:%I16u:%I16u:%I16u:%I16u\") : " DEESOCKET_PRINTFERROF,
                      DEE_BUILTIN_NTOH16(host->in6_words[0]),DEE_BUILTIN_NTOH16(host->in6_words[1]),
                      DEE_BUILTIN_NTOH16(host->in6_words[2]),DEE_BUILTIN_NTOH16(host->in6_words[3]),
                      DEE_BUILTIN_NTOH16(host->in6_words[4]),DEE_BUILTIN_NTOH16(host->in6_words[5]),
                      DEE_BUILTIN_NTOH16(host->in6_words[6]),DEE_BUILTIN_NTOH16(host->in6_words[7]),
                      DEESOCKET_PRINTFERR(error));
  DEESOCKET_API_END();
  return NULL;
 }
 if DEE_UNLIKELY(!hp->h_name) {
raw_ip: DEESOCKET_API_END();
nodns:
#endif
  return DeeString_Newf("%I16u:%I16u:%I16u:%I16u:%I16u:%I16u:%I16u:%I16u",
                        DEE_BUILTIN_NTOH16(host->in6_words[0]),DEE_BUILTIN_NTOH16(host->in6_words[1]),
                        DEE_BUILTIN_NTOH16(host->in6_words[2]),DEE_BUILTIN_NTOH16(host->in6_words[3]),
                        DEE_BUILTIN_NTOH16(host->in6_words[4]),DEE_BUILTIN_NTOH16(host->in6_words[5]),
                        DEE_BUILTIN_NTOH16(host->in6_words[6]),DEE_BUILTIN_NTOH16(host->in6_words[7]));
#if DEE_HAVE_AF_INET6
 }
 result = DeeString_New(hp->h_name);
 DEESOCKET_API_END();
 return result;
#endif
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSockAddr_Str(
 DEE_A_IN struct DeeSockAddr const *self, DEE_A_IN Dee_uint32_t flags) {
 switch (DeeSockAddr_FAMILY(self)) {
#if DEE_HAVE_AF_INET
#ifdef AF_RDS
  case AF_RDS:
#endif
  case AF_INET:
   if DEE_LIKELY((flags&DEE_SOCKADDR_STR_FLAG_NOPORT)!=0) {
    return DeeSocket_INETHostname(DeeSockAddr_INET_HOST(self),flags);
   }
   return DeeString_Newf("%K:%I16u",
                         DeeSocket_INETHostname(DeeSockAddr_INET_HOST(self),flags),
                         DEE_BUILTIN_NTOH16(DeeSockAddr_INET_PORT(self)));
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6:
   if ((flags&DEE_SOCKADDR_STR_FLAG_NOPORT)!=0)
    return DeeSocket_INET6Hostname(DeeSockAddr_INET6_HOST(self),flags);
   return DeeString_Newf("[%K]:%I16u",
                         DeeSocket_INET6Hostname(DeeSockAddr_INET6_HOST(self),flags),
                         DEE_BUILTIN_NTOH16(DeeSockAddr_INET6_PORT(self)));
#endif
#if DEE_HAVE_AF_UNIX
  case AF_UNIX:
   return DeeString_Newf("AF_UNIX:%.*q",
                         (unsigned int)(sizeof(DeeSockAddr_UNIX_PATH(self))/sizeof(char)),
                         DeeSockAddr_UNIX_PATH(self));
#endif
#if DEE_HAVE_AF_NETLINK
  case AF_NETLINK:
   return DeeString_Newf("AF_NETLINK:" DEE_TYPES_IPRINTF(DEE_TYPES_SIZEOF_PID_T) ":I32u",
                         DeeSockAddr_NETLINK_PID(self),DeeSockAddr_NETLINK_GROUPS(self));
#endif
  default:
   return DeeString_Newf("[%K]",DeeSocket_AddressFamilyToString(DeeSockAddr_FAMILY(self)));
 }
}





#define _self ((DeeSocketObject *)self)

//////////////////////////////////////////////////////////////////////////
// Low-level socket Functions
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetPeerAddr(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_OUT struct DeeSockAddr *sock_addr) {
 sock_t sock; Dee_neterrno_t error; socklen_t socklen;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 socklen = (socklen_t)Dee_SizeofSockAddr(sock_addr,DeeSocket_GET_PROTOCOL(self));
 DeeSocket_USEBEGIN(self,sock);
 if DEE_UNLIKELY(getpeername(sock,(struct sockaddr *)sock_addr,&socklen) < 0) {
  DeeSocket_USEEND(self);
  error = DeeNetError_CONSUME();
  DeeError_SetStringf(DeeErrorType_NETERROR(error),
                      "getpeername(%k) : " DEESOCKET_PRINTFERROF,
                      self,DEESOCKET_PRINTFERR(error));
  return -1;
 }
 DeeSocket_USEEND(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetSockAddr(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_OUT struct DeeSockAddr *sock_addr) {
 Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
again:
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_ACQUIRE(self);
 flags = _self->s_flags;
 if DEE_UNLIKELY((flags&(DEE_SOCKET_FLAG_BOUND|DEE_SOCKET_FLAG_CONNECTED)) == 0) {
  DeeSocket_RELEASE(self);
  if DEE_LIKELY((flags&(DEE_SOCKET_FLAG_BINDING|DEE_SOCKET_FLAG_CONNECTING))!=0) {
   // Socket is currently binding/connecting (wait a bit more)
   DeeThread_SleepNoInterrupt(1);
   goto again;
  }
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Socket not bound or connected");
  return -1;
 }
 DeeSocket_RELEASE(self);
 *sock_addr = _self->s_sock_addr;
 return 0;
}
DEE_A_RET_NOEXCEPT(1) int DeeSocket_TryGetPeerAddr(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_OUT struct DeeSockAddr *sock_addr) {
 sock_t sock; socklen_t socklen;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 socklen = (socklen_t)Dee_SizeofSockAddr(sock_addr,DeeSocket_GET_PROTOCOL(self));
 DeeSocket_USEBEGIN(self,sock);
 if DEE_UNLIKELY(getpeername(sock,(struct sockaddr *)sock_addr,&socklen) < 0) {
  DeeSocket_USEEND(self);
  DeeNetError_SET(0);
  return 1;
 }
 DeeSocket_USEEND(self);
 return 0;
}
DEE_A_RET_NOEXCEPT(1) int DeeSocket_TryGetSockAddr(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_OUT struct DeeSockAddr *sock_addr) {
 Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
again:
 DeeSocket_ACQUIRE(self);
 flags = _self->s_flags;
 if DEE_UNLIKELY((flags&(DEE_SOCKET_FLAG_BOUND|DEE_SOCKET_FLAG_CONNECTED)) == 0) {
  DeeSocket_RELEASE(self);
  if DEE_LIKELY((flags&(DEE_SOCKET_FLAG_BINDING|DEE_SOCKET_FLAG_CONNECTING))!=0) {
   // Socket is currently binding/connecting (wait a bit more)
   DeeThread_SleepNoInterrupt(1);
   goto again;
  }
  return 1;
 }
 DeeSocket_RELEASE(self);
 *sock_addr = _self->s_sock_addr;
 return 0;
}

void DeeSocket_Close(DEE_A_IN_OBJECT(DeeSocketObject) *self) {
 sock_t old_handle; Dee_uint16_t old_flags;
 DEE_ASSERT(DeeSocket_Check(self));
 DeeSocket_ACQUIRE(self);
 old_handle = _self->s_socket;
 if (old_handle == INVALID_SOCKET) {
  DeeSocket_RELEASE(self);
  return;
 }
 while (1) {
  old_flags = _self->s_flags;
  if DEE_UNLIKELY((old_flags&DEE_SOCKET_FLAG_SHUTTINGDOWN)!=0) {
   // Wait for somebody else to shut down the socket
   DeeSocket_RELEASE(self);
   DeeThread_SleepNoInterrupt(1);
   DeeSocket_ACQUIRE(self);
   continue;
  }
  break;
 }
 _self->s_flags = DEE_SOCKET_FLAG_CLOSED|DEE_SOCKET_FLAG_SHUTDOWN;
 _self->s_socket = INVALID_SOCKET;
 DeeSocket_RELEASE(self);
 if DEE_LIKELY((old_flags&DEE_SOCKET_FLAG_SHUTDOWN)!=0) {
  // Shutdown the old socket (ignore error here)
  if DEE_UNLIKELY(shutdown(old_handle,SHUT_RDWR) != 0) DeeNetError_SET(0);
 }
 // Wait for everybody to stop using the socket
 while (DeeAtomic16_Load(_self->s_uses,memory_order_seq_cst))
  DeeThread_SleepNoInterrupt(1);
 // Close the old handle
 if DEE_UNLIKELY(closesocket(old_handle) != 0) DeeNetError_SET(0);
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_Shutdown(
 DEE_A_IN_OBJECT(DeeSocketObject) *self, DEE_A_IN int how) {
 sock_t sock; int error;
again:
 DeeSocket_ACQUIRE(self);
 if ((_self->s_flags&(
  DEE_SOCKET_FLAG_SHUTDOWN|DEE_SOCKET_FLAG_CLOSED)) != 0) {
  DeeSocket_RELEASE(self);
  return 1; // Already shut down/closed
 }
 if DEE_UNLIKELY((_self->s_flags&DEE_SOCKET_FLAG_SHUTTINGDOWN) != 0) {
  DeeSocket_RELEASE(self);
  if (DeeThread_Sleep(1) != 0) return -1;
  goto again; // Retry
 }
 _self->s_flags |= DEE_SOCKET_FLAG_SHUTTINGDOWN;
 DeeSocket_USEBEGIN_LOCKED(self,sock);
 DeeSocket_RELEASE(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
  DeeSocket_USEEND(self);
  DeeSocket_ACQUIRE(self);
  _self->s_flags &= ~DEE_SOCKET_FLAG_SHUTTINGDOWN;
  DeeSocket_RELEASE(self);
  return -1;
 }
 error = shutdown(sock,SHUT_RDWR);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error != 0) {
  Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
  DeeSocket_ACQUIRE(self);
  _self->s_flags &= ~DEE_SOCKET_FLAG_SHUTTINGDOWN;
  DeeSocket_RELEASE(self);
  DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                      "shutdown(%k,%s:%d) : " DEESOCKET_PRINTFERROF,self,
                      how == SHUT_RD ? "SHUT_RD" : how == SHUT_WR ? "SHUT_WR" :
                      how == SHUT_RDWR ? "SHUT_RDWR" : "??" "?",how,
                      DEESOCKET_PRINTFERR(neterror));
  return -1;
 }
 DeeSocket_ACQUIRE(self);
 _self->s_flags &= ~DEE_SOCKET_FLAG_SHUTTINGDOWN;
 _self->s_flags |= DEE_SOCKET_FLAG_SHUTDOWN;
 DeeSocket_RELEASE(self);
 return 0;
}
#undef _self

static int _DeeListenSocket_Configure(DEE_A_IN sock_t sock) {
 // Enable non-blocking IO (required for accept w/ timeout)
 // >> Required for closing a socket currently being blocked by an 'accept()' call
 // TODO: Throw an error if these fail
#if defined(__BEOS__) && defined(SO_NONBLOCK)
 { long yes = 1; setsockopt(sock,SOL_SOCKET,SO_NONBLOCK,&yes,sizeof(yes)); }
#elif defined(O_NONBLOCK)
 { fcntl(sock,F_SETFL,O_NONBLOCK); }
#elif defined(DEE_PLATFORM_WINDOWS)
 { unsigned long yes = 1; ioctlsocket(sock,FIONBIO,&yes); }
#elif defined(__OS2__)
 { int yes = 1; ioctl(sock,FIONBIO,&yes); }
#endif
 return 0;
}

static int _DeeSysSocket_Configure(DEE_A_INOUT DeeSocketObject *self) {
#ifdef SO_RCVTIMEO
 // Setup a receive timeout, so we don't stay in the
 // kernel while someone might be trying to close the socket...
 // >> Using this, we temporarily unlock the socket once in a while, seemingly
 //    allowing 's.close()' while another thread is in a blocking 's.file.read()'
 // Note: We do the same for the send timeout, but that one isn't really as important...
 if DEE_UNLIKELY(DeeSocket_SetRecvTimeout((DeeObject *)self,
  DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT) != 0) return -1;
#endif
#ifdef SO_SNDTIMEO
 if DEE_UNLIKELY(DeeSocket_SetSendTimeout((DeeObject *)self,
  DEE_XCONFIG_SOCKET_DEFAULT_SEND_TIMEOUT) != 0) return -1;
#endif
#if DEE_HAVE_IPv6_DUALSTACK
 if (DeeSocket_GET_ADDRESS_FAMILY(self) == AF_INET6) {
  int value = 0; socklen_t optlen = sizeof(value);
  if (setsockopt(self->s_socket,IPPROTO_IPV6,IPV6_V6ONLY,(char const *)&value,sizeof(value)) != 0
   || getsockopt(self->s_socket,IPPROTO_IPV6,IPV6_V6ONLY,(char *)&value,&optlen) != 0
   || value) DeeNetError_SET(0); // Failed to enable dual-stack
 }
#endif
 return 0;
}

#ifndef __INTELLISENSE__
#define VALUE
#include "socket.impl.sockopt.inl"
#include "socket.impl.sockopt.inl"
#endif

DEE_A_RET_EXCEPT(-1) int DeeSocket_SetRecvTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN unsigned int msecs) {
#if defined(SO_RCVTIMEO) && defined(SOL_SOCKET)
 sock_t sock;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 if DEE_UNLIKELY(!msecs) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid recv timeout: 0");
  return -1;
 }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 {
#ifdef DEE_PLATFORM_WINDOWS
  DEE_STATIC_ASSERT(sizeof(DWORD) == sizeof(msecs));
  if DEE_UNLIKELY(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char const *)&msecs,sizeof(DWORD)) != 0)
#else
  struct timeval tv;
  tv.tv_sec = (long)(DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT/1000ul);
  tv.tv_usec = (long)((DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT%1000ul)*1000ul);
  if DEE_UNLIKELY(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char const *)&tv,sizeof(tv)) != 0)
#endif
  {
   Dee_neterrno_t error = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "setsockopt(%k,SOL_SOCKET,SO_RCVTIMEO) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR(error));
   return -1;
  }
 }
 DeeSocket_USEEND(self);
 return 0;
#else
 (void)self,msecs;
 DeeError_NotImplemented_str("SO_RCVTIMEO");
 return -1;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetRecvTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT unsigned int *msecs) {
#if defined(SO_RCVTIMEO) && defined(SOL_SOCKET)
 sock_t sock;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEE_ASSERT(msecs);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 {
#ifdef DEE_PLATFORM_WINDOWS
  socklen_t socklen = sizeof(DWORD);
  if DEE_UNLIKELY(getsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)msecs,&socklen) != 0)
#else
  struct timeval tv;
  socklen_t socklen = sizeof(struct timeval);
  if DEE_UNLIKELY(getsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,&socklen) != 0)
#endif
  {
   Dee_neterrno_t error = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "getsockopt(%k,SOL_SOCKET,SO_RCVTIMEO) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR(error));
   return -1;
  }
#ifndef DEE_PLATFORM_WINDOWS
  *msecs = (unsigned int)((tv.tv_sec*1000ul)+(tv.tv_usec/1000ul));
#endif
 }
 DeeSocket_USEEND(self);
 return 0;
#else
 (void)self,msecs;
 DeeError_NotImplemented_str("SO_RCVTIMEO");
 return -1;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_SetSendTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN unsigned int msecs) {
#if defined(SO_SNDTIMEO) && defined(SOL_SOCKET)
 sock_t sock;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 if DEE_UNLIKELY(!msecs) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid send timeout: 0");
  return -1;
 }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 {
#ifdef DEE_PLATFORM_WINDOWS
  DEE_STATIC_ASSERT(sizeof(DWORD) == sizeof(msecs));
  if DEE_UNLIKELY(setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char const *)&msecs,sizeof(DWORD)) != 0)
#else
  struct timeval tv;
  tv.tv_sec = (long)(DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT/1000ul);
  tv.tv_usec = (long)((DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT%1000ul)*1000ul);
  if DEE_UNLIKELY(setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char const *)&tv,sizeof(tv)) != 0)
#endif
  {
   Dee_neterrno_t error = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "setsockopt(%k,SOL_SOCKET,SO_SNDTIMEO) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR(error));
   return -1;
  }
 }
 DeeSocket_USEEND(self);
 return 0;
#else
 (void)self,msecs;
 DeeError_NotImplemented_str("SO_SNDTIMEO");
 return -1;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetSendTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT unsigned int *msecs) {
#if defined(SO_SNDTIMEO) && defined(SOL_SOCKET)
 sock_t sock;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEE_ASSERT(msecs);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 {
#ifdef DEE_PLATFORM_WINDOWS
  socklen_t socklen = sizeof(DWORD);
  if DEE_UNLIKELY(getsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)msecs,&socklen) != 0)
#else
  struct timeval tv;
  socklen_t socklen = sizeof(struct timeval);
  if DEE_UNLIKELY(getsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,&socklen) != 0)
#endif
  {
   Dee_neterrno_t error = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "getsockopt(%k,SOL_SOCKET,SO_SNDTIMEO) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR(error));
   return -1;
  }
#ifndef DEE_PLATFORM_WINDOWS
  *msecs = (unsigned int)((tv.tv_sec*1000ul)+(tv.tv_usec/1000ul));
#endif
 }
 DeeSocket_USEEND(self);
 return 0;
#else
 (void)self,msecs;
 DeeError_NotImplemented_str("SO_SNDTIMEO");
 return -1;
#endif
}




DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeSocket_Init(
 DEE_A_INOUT DeeSocketObject *self, DEE_A_IN int af,
 DEE_A_IN int type, DEE_A_IN int protocol) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEESOCKET_API_BEGIN(return -1);
 if DEE_UNLIKELY((self->s_socket = (DeeSocketHandle)socket(
  af,type,protocol)) == (DeeSocketHandle)INVALID_SOCKET) {
  Dee_neterrno_t error = DeeNetError_GET();
  DeeNetError_SET(0);
  DeeError_SetStringf(DeeErrorType_NETERROR(error),
                      "socket(%K,%K,%K) : " DEESOCKET_PRINTFERROF,
                      DeeSocket_AddressFamilyToString(af),
                      DeeSocket_TypeToString(type),
                      DeeSocket_ProtocolToString(protocol),
                      DEESOCKET_PRINTFERR(error));
  DEESOCKET_API_END();
  return -1;
 }
 _DeeSocket_InitCommon(self,DEE_SOCKET_FLAG_NONE,af,type,protocol);
 if DEE_UNLIKELY(_DeeSysSocket_Configure(self) != 0) {
  if DEE_UNLIKELY(closesocket(self->s_socket) != 0) DeeNetError_SET(0);
  return -1;
 }
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *DeeSocket_New(
 DEE_A_IN int af, DEE_A_IN int type, DEE_A_IN int protocol) {
 DeeSocketObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSocketObject)) != NULL) {
  DeeObject_INIT(result,&DeeSocket_Type);
  if DEE_UNLIKELY(DeeSocket_Init(result,af,type,protocol) != 0) {
   _DeeObject_DELETE(&DeeSocket_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}



DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeSocket_AcceptImpl(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_IN unsigned int msecs, sock_t *result,
 DEE_A_OUT struct DeeSockAddr *peer_addr) {
 sock_t server_sock,client_sock;
 socklen_t socklen; int error; 
#if !DEE_HAVE_SELECT
 Dee_uint64_t start_time;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DeeSocket_USEBEGIN(self,server_sock);
#if DEE_HAVE_SELECT
 if (msecs) again: {
  fd_set rfds;
  struct timeval tv;
  tv.tv_sec = (long)(DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT/1000ul);
  tv.tv_usec = (long)((DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT%1000ul)*1000ul);
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
   DeeSocket_USEEND(self);
   return -1;
  }
DEE_COMPILER_MSVC_WARNING_PUSH(4548)
  FD_ZERO(&rfds);
  FD_SET(server_sock,&rfds);
DEE_COMPILER_MSVC_WARNING_POP
#ifdef DEE_PLATFORM_WINDOWS
  error = select(0,&rfds,NULL,NULL,&tv);
#else
  error = select(server_sock+1,&rfds,NULL,NULL,&tv);
#endif
  if DEE_LIKELY(error == 0) { // timed out
   DeeSocket_USEEND(self);
timeout:
   if DEE_LIKELY(msecs != (unsigned int)-1) {
    if (msecs <= DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT) return 1;
    msecs -= DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT;
   }
   DeeSocket_USEBEGIN(self,server_sock);
   goto again;
  }
  if DEE_UNLIKELY(error < 0) {
   Dee_neterrno_t neterror;
   // Error
   DeeSocket_USEEND(self);
   neterror = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                       "select(%k) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR(neterror));
   return -1;
  }
 }
 socklen = (socklen_t)Dee_SizeofSockAddr(&((DeeSocketObject *)self)->s_sock_addr,
                                         DeeSocket_GET_PROTOCOL(self));
 client_sock = accept(server_sock,(struct sockaddr *)peer_addr,&socklen);
#else
 // rudimentary implementation of a non-blocking wait (without select...)
 if (msecs != (unsigned int)-1) {
  end_time = DeeTime_GetClock1000()+(Dee_uint64_t)msecs;
 }
 if (0) {
timeout:
  if DEE_LIKELY(msecs != (unsigned int)-1) {
   if (DeeTime_GetClock1000() >= end_time) return 1;
  }
  DeeThread_SleepNoInterrupt(10); // Sleep a bit while we wait for clients
  DeeSocket_USEBEGIN(self,server_sock);
 }
 socklen = (socklen_t)Dee_SizeofSockAddr(&((DeeSocketObject *)self)->s_sock_addr,
                                         DeeSocket_GET_PROTOCOL(self));
 client_sock = accept(server_sock,(struct sockaddr *)peer_addr,&socklen);
#endif
 DeeSocket_USEEND(self);
 // Got a client socket.
 // >> Assume timeout has passed
 if DEE_UNLIKELY(client_sock == INVALID_SOCKET) {
  Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
#ifdef WSAEWOULDBLOCK
  if (neterror == WSAEWOULDBLOCK) goto timeout;
#endif
#ifdef EWOULDBLOCK
  if (neterror == EWOULDBLOCK) goto timeout;
#endif
#ifdef EAGAIN
  if (neterror == EAGAIN) goto timeout;
#endif
#ifdef WSAETIMEDOUT
  if (neterror == WSAETIMEDOUT) goto timeout;
#endif
#ifdef ETIMEDOUT
  if (neterror == ETIMEDOUT) goto timeout;
#endif
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                      "accept(%k) : " DEESOCKET_PRINTFERROF,
                      self,DEESOCKET_PRINTFERR(neterror));
  return -1;
 }
 *result = client_sock;
 return 0;
}


DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_AcceptTimedEx(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_IN unsigned int msecs,
 DEE_A_OUT_OBJECT(DeeSocketObject) **result) {
 sock_t client_sock; int error; DeeSocketObject *result_ob;
 struct DeeSockAddr peer_address; socklen_t socklen;
 if DEE_UNLIKELY((error = _DeeSocket_AcceptImpl(self,msecs,&client_sock,&peer_address)) != 0) return error;
 if DEE_UNLIKELY((result_ob = DeeObject_MALLOCF(DeeSocketObject,"socket.accept()")) == NULL) {
err: if DEE_UNLIKELY(closesocket(client_sock) != 0) DeeNetError_SET(0);
  return -1;
 }
 socklen = (socklen_t)Dee_SizeofSockAddr(&peer_address,DeeSocket_GET_PROTOCOL(self));
 if DEE_UNLIKELY(getsockname(client_sock,(struct sockaddr *)&result_ob->s_sock_addr,&socklen) < 0) {
  Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
  DeeObject_Free(result);
  DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                      "getsockname(" DeeSocket_PRINTF ") : " DEESOCKET_PRINTFERROF,
                      client_sock,DEESOCKET_PRINTFERR(neterror));
  goto err;
 }
 DeeObject_INIT(result_ob,&DeeSocket_Type);
 // Get 'type' and 'protocol' from the real socket (through SO_* constants)
 _DeeSocket_InitCommonNoFamily(result_ob,DEE_SOCKET_FLAG_CONNECTED,
                               DeeSocket_GET_TYPE(self),
                               DeeSocket_GET_PROTOCOL(self));
 result_ob->s_socket = client_sock;
 DEESOCKET_API_BEGIN_NONFIRST();
 if DEE_UNLIKELY(_DeeSysSocket_Configure(result_ob) != 0) {
  Dee_DECREF(result_ob);
  return -1;
 }
 *result = (DeeObject *)result_ob;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *DeeSocket_Accept(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeSocket_AcceptTimedEx(self,(unsigned int)-1,&result) != 0) result = NULL;
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_TryAcceptEx(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self,
 DEE_A_OUT_OBJECT(DeeSocketObject) **result) {
 return DeeSocket_AcceptTimedEx(self,0,result);
}
DEE_A_RET_WUNUSED Dee_uint16_t DeeSocket_Flags(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self) {
 Dee_uint16_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DeeSocket_ACQUIRE(self);
 result = ((DeeSocketObject *)self)->s_flags;
 DeeSocket_RELEASE(self);
 return (Dee_uint16_t)(result & DEE_SOCKET_FLAG_PUBLIC_MASK);
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetAddressFamily(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 // I know this never thrown an error, but it might in the future
 return DeeSocket_GET_ADDRESS_FAMILY(self);
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetType(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 // I know this never thrown an error, but it might in the future
 return DeeSocket_GET_TYPE(self);
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_GetProtocol(
 DEE_A_IN_OBJECT(DeeSocketObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 // I know this never thrown an error, but it might in the future
 return DeeSocket_GET_PROTOCOL(self);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_Bind(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN struct DeeSockAddr const *sock_addr) {
 sock_t sock; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEE_ASSERT(sock_addr);
again:
 DeeSocket_ACQUIRE(self);
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_BOUND) != 0) {
  DeeSocket_RELEASE(self);
  return 1; // Already bound
 }
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_BINDING) != 0) {
  DeeSocket_RELEASE(self);
  if DEE_UNLIKELY(DeeThread_Sleep(1) != 0) return -1;
  goto again; // Retry
 }
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_BINDING;
 DeeSocket_USEBEGIN_LOCKED(self,sock);
 DeeSocket_RELEASE(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
  DeeSocket_USEEND(self);
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_BINDING;
  DeeSocket_RELEASE(self);
  return -1;
 }
#if defined(SOL_SOCKET) && defined(SO_REUSEADDR)
 // Allow local address reuse
 { int yes = 1; setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(yes)); }
#endif
 error = bind(sock,(struct sockaddr *)sock_addr,(socklen_t)
              Dee_SizeofSockAddr(sock_addr,DeeSocket_GET_PROTOCOL(self)));
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error != 0) {
  Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_BINDING;
  DeeSocket_RELEASE(self);
  DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                      "bind(%k,%K) : " DEESOCKET_PRINTFERROF,
                      self,DeeSockAddr_Str(sock_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                      DEESOCKET_PRINTFERR(neterror));
  return -1;
 }
 DeeSocket_ACQUIRE(self);
 ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_BINDING;
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_BOUND;
 ((DeeSocketObject *)self)->s_sock_addr = *sock_addr;
 DeeSocket_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_Connect(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN struct DeeSockAddr const *peer_addr) {
 sock_t sock; int error; socklen_t addrlen;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEE_ASSERT(peer_addr);
again:
 DeeSocket_ACQUIRE(self);
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_CONNECTED) != 0) {
  DeeSocket_RELEASE(self);
  return 1; // Already connected
 }
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_CONNECTING) != 0) {
  DeeSocket_RELEASE(self);
  if DEE_UNLIKELY(DeeThread_Sleep(1) != 0) return -1;
  goto again; // Retry
 }
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_CONNECTING;
 DeeSocket_USEBEGIN_LOCKED(self,sock);
 DeeSocket_RELEASE(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
  DeeSocket_USEEND(self);
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
  DeeSocket_RELEASE(self);
  return -1;
 }
 addrlen = (socklen_t)Dee_SizeofSockAddr(peer_addr,DeeSocket_GET_PROTOCOL(self));
 error = connect(sock,(struct sockaddr *)peer_addr,addrlen);
 if DEE_UNLIKELY(error != 0) {
  error = (int)DeeNetError_GET(); DeeNetError_SET(0);
#if defined(WSAEINPROGRESS) || defined(EINPROGRESS)
#if defined(WSAEINPROGRESS) && defined(EINPROGRESS)
  if (error == WSAEINPROGRESS || error == EINPROGRESS)
#elif defined(WSAEINPROGRESS)
  if (error == WSAEINPROGRESS)
#elif defined(EINPROGRESS)
  if (error == EINPROGRESS)
#endif
  {
   // Non-blocking connect still going on
   // >> Use 'select' to wait for the socket to finish
   fd_set wfds; socklen_t errlen;
DEE_COMPILER_MSVC_WARNING_PUSH(4548)
   FD_ZERO(&wfds);
   FD_SET(sock,&wfds);
DEE_COMPILER_MSVC_WARNING_POP
#ifdef DEE_PLATFORM_WINDOWS
   error = select(0,NULL,&wfds,NULL,NULL);
#else
   error = select(sock+1,NULL,&wfds,NULL,NULL);
#endif
   // Handled errors in 'select'
   if DEE_UNLIKELY(error == 0) {
    Dee_neterrno_t neterror;
    DeeSocket_USEEND(self);
    DeeSocket_ACQUIRE(self);
    ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
    DeeSocket_RELEASE(self);
    neterror = DeeNetError_GET(); DeeNetError_SET(0);
    DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                        "connect(%k,%K) : EINPROGRESS : select() : " DEESOCKET_PRINTFERROF,
                        self,DeeSockAddr_Str(peer_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                        DEESOCKET_PRINTFERR(neterror));
    return -1;
   }
   errlen = sizeof(error);
   // Check for an error in the connect operation
   if DEE_UNLIKELY(getsockopt(sock,SOL_SOCKET,SO_ERROR,(char *)&error,&errlen) != 0) {
    Dee_neterrno_t neterror;
    DeeSocket_USEEND(self);
    DeeSocket_ACQUIRE(self);
    ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
    DeeSocket_RELEASE(self);
    neterror = DeeNetError_GET(); DeeNetError_SET(0);
    DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                        "connect(%k,%K) : EINPROGRESS : getsockopt(SOL_SOCKET,SO_ERROR) : " DEESOCKET_PRINTFERROF,
                        self,DeeSockAddr_Str(peer_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                        DEESOCKET_PRINTFERR(neterror));
    return -1;
   }
   // If the connect failed, throw its error
   if DEE_UNLIKELY(error != 0) {
    DeeSocket_USEEND(self);
    DeeSocket_ACQUIRE(self);
    ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
    DeeSocket_RELEASE(self);
    goto connect_err;
   }
  } else
#endif /* WSAEINPROGRESS || EINPROGRESS */
  if DEE_UNLIKELY(error != 0) {
   DeeSocket_USEEND(self);
   DeeSocket_ACQUIRE(self);
   ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
   DeeSocket_RELEASE(self);
   //error = DeeNetError_GET(); DeeNetError_SET(0);
#if defined(WSAEINPROGRESS) || defined(EINPROGRESS)
connect_err:
#endif
   DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                       "connect(%k,%K) : " DEESOCKET_PRINTFERROF,
                       self,DeeSockAddr_Str(peer_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                       DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
   return -1;
  }
 }
 // The socket is now connected --> Time to figure out our address
 error = getsockname(sock,(struct sockaddr *)&((DeeSocketObject *)self)->s_sock_addr,&addrlen);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error < 0) {
  Dee_neterrno_t neterror = DeeNetError_GET(); DeeNetError_SET(0);
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
  DeeSocket_RELEASE(self);
  DeeError_SetStringf(DeeErrorType_NETERROR(neterror),
                      "getsockname(%k) : " DEESOCKET_PRINTFERROF,
                      self,DEESOCKET_PRINTFERR(neterror));
  return -1;
 }
 DeeSocket_ACQUIRE(self);
 ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_CONNECTING;
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_CONNECTED;
 DeeSocket_RELEASE(self);
 return 0;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSocket_ListenEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN int max_backlog) {
 int error; sock_t sock;
 if DEE_LIKELY(max_backlog == DEE_SOCKET_MAXBACKLOG_DEFAULT)
  max_backlog = DeeSocket_GetDefaultMaxBacklog();
again:
 DeeSocket_ACQUIRE(self);
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_LISTENING) != 0) {
  DeeSocket_RELEASE(self);
  return 1; // Already listening
 }
 if DEE_UNLIKELY((((DeeSocketObject *)self)->s_flags&DEE_SOCKET_FLAG_STARTLISTENING) != 0) {
  DeeSocket_RELEASE(self);
  if DEE_UNLIKELY(DeeThread_Sleep(1) != 0) return -1;
  goto again; // Retry
 }
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_STARTLISTENING;
 DeeSocket_USEBEGIN_LOCKED(self,sock);
 DeeSocket_RELEASE(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0 ||
                 _DeeListenSocket_Configure(sock) != 0) {
  DeeSocket_USEEND(self);
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_STARTLISTENING;
  DeeSocket_RELEASE(self);
  return -1;
 }
 error = listen(sock,max_backlog);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error != 0) {
  DeeSocket_ACQUIRE(self);
  ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_STARTLISTENING;
  DeeSocket_RELEASE(self);
  error = (int)DeeNetError_GET(); DeeNetError_SET(0);
  DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                      "listen(%k,%d) : " DEESOCKET_PRINTFERROF,
                      self,max_backlog,DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
  return -1;
 }
 DeeSocket_ACQUIRE(self);
 ((DeeSocketObject *)self)->s_flags &= ~DEE_SOCKET_FLAG_STARTLISTENING;
 ((DeeSocketObject *)self)->s_flags |= DEE_SOCKET_FLAG_LISTENING;
 DeeSocket_RELEASE(self);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeSocket_SendEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s,
 DEE_A_OUT Dee_size_t *ws, DEE_A_IN int flags) {
 int error; sock_t sock;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(s >= INT_MAX) s = INT_MAX;
 DeeSocket_USEBEGIN(self,sock);
 error = send(sock,(char const *)p,(int)s,flags);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error == -1) {
  error = (int)DeeNetError_GET();
  if DEE_UNLIKELY(error != 0) {
   DeeNetError_SET(0);
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                       "send(%k,%p,%Iu,%d) : " DEESOCKET_PRINTFERROF,
                       self,p,s,flags,DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
   return -1;
  } else error = -1;
 }
 *ws = (Dee_size_t)error;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_RecvEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s,
 DEE_A_OUT Dee_size_t *rs, DEE_A_IN int flags) {
 int error; sock_t sock;
 if DEE_UNLIKELY(s >= INT_MAX) s = INT_MAX;
again:
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 error = recv(sock,(char *)p,(int)s,flags);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error == -1) {
  if DEE_UNLIKELY((error = (int)DeeNetError_GET()) != 0) {
   DeeNetError_SET(0);
#ifdef WSAEWOULDBLOCK
   if (error == WSAEWOULDBLOCK) goto again;
#endif
#ifdef EWOULDBLOCK
   if (error == EWOULDBLOCK) goto again;
#endif
#ifdef EAGAIN
   if (error == EAGAIN) goto again;
#endif
#ifdef WSAETIMEDOUT
   if (error == WSAETIMEDOUT) goto again;
#endif
#ifdef ETIMEDOUT
   if (error == ETIMEDOUT) goto again;
#endif
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                       "recv(%k,%p,%Iu,%d) : " DEESOCKET_PRINTFERROF,
                       self,p,s,flags,DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
   return -1;
  } else error = -1;
 }
 *rs = (Dee_size_t)error;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeSocket_SendAllEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN int flags) {
 int result; Dee_size_t temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 if DEE_UNLIKELY(!s) return 0;
 while (1) {
  result = DeeSocket_SendEx(self,p,s,&temp,flags);
  if DEE_UNLIKELY(result != 0) break;
  if DEE_LIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was written
  if (!temp) { // Second likely case: Can't write at all
   DeeError_Throw(DeeErrorInstance_FileCantWrite);
   result = -1;
   break;
  }
  (*(char **)&p) += temp; // rare case: only wrote a bit (remainder must be written next)
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_RecvAllEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_IN int flags) {
 int result; Dee_size_t temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 if DEE_UNLIKELY(!s) return 0;
 while (1) {
  result = DeeSocket_RecvEx(self,p,s,&temp,flags);
  if DEE_UNLIKELY(result != 0) break;
  if DEE_LIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was read
  if (!temp) { // Second likely case: Can't read at all
   DeeError_Throw((DeeObject *)&_DeeErrorInstance_FileCantRead);
   result = -1;
   break;
  }
  (*(char **)&p) += temp; // rare case: only read a bit (remainder must be read next)
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeSocket_SendToEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws,
 DEE_A_IN struct DeeSockAddr const *peer_addr, DEE_A_IN int flags) {
 int error; sock_t sock;
 DEE_ASSERT(!s || p);
 DEE_ASSERT(peer_addr && ws);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeSocket_USEBEGIN(self,sock);
 error = sendto(sock,(char const *)p,(int)s,flags,(struct sockaddr *)peer_addr,
                (socklen_t)Dee_SizeofSockAddr(peer_addr,DeeSocket_GET_PROTOCOL(self)));
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error == -1) {
  error = (int)DeeNetError_GET();
  if DEE_LIKELY(error == 0) error = -1;
  else {
   DeeNetError_SET(0);
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                       "sendto(%k) : " DEESOCKET_PRINTFERROF,
                       self,DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
   return -1;
  }
 }
 *ws = (Dee_size_t)error;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSocket_RecvFromEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs,
 DEE_A_OUT struct DeeSockAddr *peer_addr, DEE_A_IN int flags) {
 int error; sock_t sock;
 socklen_t sock_addr_len;
 DEE_ASSERT(!s || p != NULL);
 DEE_ASSERT(peer_addr && rs);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 sock_addr_len = sizeof(struct DeeSockAddr);
 DeeSocket_USEBEGIN(self,sock);
 error = recvfrom(sock,(char *)p,(int)s,flags,
                  (struct sockaddr *)peer_addr,
                  &sock_addr_len);
 DeeSocket_USEEND(self);
 if DEE_UNLIKELY(error == -1) {
  error = (int)DeeNetError_GET();
  if DEE_UNLIKELY(error == 0) error = -1;
  else {
   DeeNetError_SET(0);
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
   DeeError_SetStringf(DeeErrorType_NETERROR((Dee_neterrno_t)error),
                       "recvfrom() : " DEESOCKET_PRINTFERROF,
                       DEESOCKET_PRINTFERR((Dee_neterrno_t)error));
   return -1;
  }
 }
 *rs = (Dee_size_t)error;
 return 0;
}

Dee_size_t DeeSocket_MaxUDPPacketSize = DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE;
extern void _DeeSocket_SetMaxUDPPacketSizeString(char const *value);
void _DeeSocket_SetMaxUDPPacketSizeString(char const *value) {
 Dee_size_t new_default_size;
 if (!*value) new_default_size = DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE;
 else if DEE_UNLIKELY(DeeString_ToNumber(Dee_size_t,value,&new_default_size) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE
                 "' (defaulting to '" DEE_PP_STR(DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE) "')\n",1);
  new_default_size = DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE;
 }
 DEE_LVERBOSE1("Setting: 'DeeSocket_MaxUDPPacketSize = %Iu;'\n",new_default_size);
 DeeAtomicN_Store(DEE_TYPES_SIZEOF_SIZE_T,DeeSocket_MaxUDPPacketSize,
                  new_default_size,memory_order_seq_cst);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSocket_RecvDataFromEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN Dee_size_t max_bytes,
 DEE_A_OUT struct DeeSockAddr *peer_addr, DEE_A_IN int flags) {
 DeeObject *result_buffer; Dee_size_t used_size,maxpacksize;
 DEE_ATOMIC_ONCE({
  DeeObject *env_packetsize;
  if ((env_packetsize = DeeFS_TryGetEnv(
   DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE)) != NULL) {
   _DeeSocket_SetMaxUDPPacketSizeString(DeeString_STR(env_packetsize));
   Dee_DECREF(env_packetsize);
  } else DeeAtomicN_Store(DEE_TYPES_SIZEOF_SIZE_T,DeeSocket_MaxUDPPacketSize,
                          DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE,memory_order_seq_cst);
 });
 maxpacksize = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,DeeSocket_MaxUDPPacketSize,memory_order_seq_cst);
 if DEE_UNLIKELY(max_bytes >= maxpacksize) max_bytes = maxpacksize;
 if DEE_UNLIKELY((result_buffer = DeeString_NewSized(max_bytes/sizeof(char))) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSocket_RecvFromEx(self,DeeString_STR(result_buffer),max_bytes,&used_size,peer_addr,flags) != 0 ||
                (used_size != max_bytes && DeeString_Resize(&result_buffer,used_size/sizeof(char)) != 0)) Dee_CLEAR(result_buffer);
 return result_buffer;
}

#ifndef __INTELLISENSE__
#define ALL
#include "socket.impl.recvdata.inl"
#include "socket.impl.recvdata.inl"
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketFileToObject) *DeeSocket_FileTo(
 DEE_A_INOUT_OBJECT(DeeUDPSocketObject) *self, DEE_A_IN struct DeeSockAddr const *peer_addr) {
 DeeSocketFileToObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DEE_ASSERT(peer_addr);
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSocketFileToObject)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeSocketFileTo_Type);
  _DeeFile_Init(result);
  Dee_INCREF(result->sft_socket = (DeeSocketObject *)self);
  result->sft_peer_addr = *peer_addr;
 }
 return (DeeObject *)result;
}















DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *
DeeSocket_NewTCPServerEx(DEE_A_IN Dee_uint16_t port, DEE_A_IN int max_backlog) {
#if DEE_HAVE_AF_INET || DEE_HAVE_AF_INET6
 DeeSocketObject *result; struct DeeSockAddr sockaddr;
#if DEE_HAVE_IPv6_DUALSTACK
 static struct in6_addr const i6_any = IN6ADDR_ANY_INIT; sock_t sock;
 // If dual stacking is supported, prefer IPv6
 DEESOCKET_API_BEGIN(return NULL);
 if DEE_LIKELY((sock = socket(AF_INET6,DEE_TCP_TYPE,DEE_TCP_PROTOCOL)) != INVALID_SOCKET) {
  int value = 0; socklen_t optlen = sizeof(value);
  _DeeSockAddr_InitINET(&sockaddr,INADDR_ANY,DEE_BUILTIN_HTON16(port));
  if (setsockopt(sock,IPPROTO_IPV6,IPV6_V6ONLY,(char const *)&value,sizeof(value)) != 0
   || getsockopt(sock,IPPROTO_IPV6,IPV6_V6ONLY,(char *)&value,&optlen) != 0
   || !optlen || value) { // Failed to enable dual-stack
   closesocket(sock);
   DeeNetError_SET(0);
   goto ipv4_sock;
  }
  if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeSocketObject)) == NULL) {
   if DEE_UNLIKELY(closesocket(sock) != 0) DeeNetError_SET(0);
   DEESOCKET_API_END();
   return NULL;
  }
  DeeObject_INIT(result,&DeeSocket_Type); // Inherit reference from 'DEESOCKET_API_BEGIN'
  _DeeSocket_InitCommon(result,DEE_SOCKET_FLAG_NONE,AF_INET6,DEE_TCP_TYPE,DEE_TCP_PROTOCOL);
  result->s_socket = sock;
  if DEE_UNLIKELY(_DeeSysSocket_Configure(result) != 0) { Dee_DECREF(result); return NULL; }
  DEE_LVERBOSE1("Created dualstack TCP server socket: %k\n",result);
  _DeeSockAddr_InitINET6(&sockaddr,(DeeINet6Host *)&i6_any,DEE_BUILTIN_HTON16(port));
 } else ipv4_sock:
#endif
 {
  result = (DeeSocketObject *)DeeSocket_New(AF_INET,DEE_TCP_TYPE,DEE_TCP_PROTOCOL);
#if DEE_HAVE_IPv6_DUALSTACK
  DEESOCKET_API_END();
#endif
  if DEE_UNLIKELY(!result) return NULL;
  _DeeSockAddr_InitINET(&sockaddr,INADDR_ANY,DEE_BUILTIN_HTON16(port));
 }
 if DEE_UNLIKELY(DeeSocket_Bind((DeeObject *)result,&sockaddr) != 0 ||
                 DeeSocket_ListenEx((DeeObject *)result,max_backlog) != 0
                 ) Dee_CLEAR(result);
 return (DeeObject *)result;
#else
 DeeError_NotImplemented_str("AF_INET|AF_INET6");
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *
DeeSocket_NewTCPClient(DEE_A_IN struct DeeSockAddr const *addr) {
 DeeObject *result; DEE_ASSERT(addr);
 if DEE_UNLIKELY((result = DeeSocket_New(DeeSockAddr_FAMILY(
  addr),DEE_TCP_TYPE,DEE_TCP_PROTOCOL)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSocket_Connect(result,addr) != 0) Dee_CLEAR(result);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketFileObject) *
DeeSocket_File(DEE_A_INOUT_OBJECT(DeeSocketObject) *self) {
 DeeSocketFileObject *result,*old_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 DeeSocket_ACQUIRE(self);
 if DEE_LIKELY(DeeSocket_FILE(self) && (result = (DeeSocketFileObject *)
  DeeObject_LockWeakref(DeeSocket_FILE(self))) != NULL) {
  DeeSocket_RELEASE(self);
  return (DeeObject *)result;
 }
 DeeSocket_RELEASE(self);
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSocketFileObject)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeSocketFile_Type);
  _DeeFile_Init(result);
  Dee_INCREF(result->sf_socket = (DeeSocketObject *)self);
  Dee_WEAKINCREF(result);
  DeeSocket_ACQUIRE(self);
  old_file = ((DeeSocketObject *)self)->s_file;
  ((DeeSocketObject *)self)->s_file = result;
  DeeSocket_RELEASE(self);
  Dee_WEAKXDECREF(old_file);
 }
 return (DeeObject *)result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *
DeeSocket_NewUDP(DEE_A_IN Dee_uint16_t port) {
#if DEE_HAVE_AF_INET || DEE_HAVE_AF_INET6
 sock_t sock;
 DeeSocketObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeSocketObject)) != NULL) {
  DeeObject_INIT(result,&DeeSocket_Type);
  DEESOCKET_API_BEGIN(goto err_r);
  if DEE_UNLIKELY((sock = socket(DEE_UDP_AF,DEE_UDP_TYPE,DEE_UDP_PROTOCOL)) == INVALID_SOCKET) {
   Dee_neterrno_t error = DeeNetError_GET();
   DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "socket() : " DEESOCKET_PRINTFERROF,
                       DEESOCKET_PRINTFERR(error));
err: DEESOCKET_API_END();
err_r: _DeeObject_DELETE(&DeeSocket_Type,result);
   return NULL;
  }
  /* Bind locally, if appropriate */
#if DEE_HAVE_AF_INET
  _DeeSockAddr_InitINET(&result->s_sock_addr,INADDR_ANY,DEE_BUILTIN_HTON16(port));
#else
  static struct in6_addr const i6_any = IN6ADDR_ANY_INIT;
  _DeeSockAddr_InitINET6(&result->s_sock_addr,(DeeINet6Host *)&i6_any,DEE_BUILTIN_HTON16(port));
#endif
  /* Bind the socket for listening */
  if DEE_UNLIKELY(bind(sock,(struct sockaddr *)&result->s_sock_addr,(
   socklen_t)Dee_SizeofSockAddr(&result->s_sock_addr,DEE_UDP_PROTOCOL)) != 0) {
   Dee_neterrno_t error = DeeNetError_GET(); DeeNetError_SET(0);
   DeeError_SetStringf(DeeErrorType_NETERROR(error),
                       "bind(<udp_socket: %I16u>) : " DEESOCKET_PRINTFERROF,
                       port,DEESOCKET_PRINTFERR(error));
   if DEE_UNLIKELY(closesocket(sock) != 0) DeeNetError_SET(0);
   goto err;
  }
#ifdef SOL_SOCKET
#ifdef SO_BROADCAST /* Allow LAN broadcasts with the socket */
  { int yes = 1; setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&yes,sizeof(yes)); }
#endif
#ifdef SO_RCVBUF
  { int buffsize = 50000; setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char*)&buffsize,sizeof(buffsize)); }
#endif
#endif
#ifdef IP_ADD_MEMBERSHIP
  { struct ip_mreq g;
#if 1
   g.imr_multiaddr.s_addr = DEE_SOCKADDRINET_HOST(224,0,0,1);
#else
   g.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
#endif
   g.imr_interface.s_addr = INADDR_ANY;
   setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&g,sizeof(g));
 }
#endif
  _DeeSocket_InitCommon(result,DEE_SOCKET_FLAG_BOUND,DEE_UDP_AF,
                        DEE_UDP_TYPE,DEE_UDP_PROTOCOL);
  result->s_socket = (DeeSocketHandle)sock;
 }
 return (DeeObject *)result;
#else
 DeeError_NotImplemented_str("AF_INET|AF_INET6");
 return NULL;
#endif
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//  -- DeeSocket_AddressFamilyToString
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSocket_AddressFamilyToString(DEE_A_IN int af) {
 switch (af) {
  case DEE_AF_AUTO: DeeReturn_STATIC_STRING("AF_AUTO");
DEE_COMPILER_MSVC_WARNING_PUSH(4574)
#ifdef AF_UNSPEC
  case AF_UNSPEC: DeeReturn_STATIC_STRING("AF_UNSPEC");
#endif
DEE_COMPILER_MSVC_WARNING_POP
#if DEE_HAVE_AF_UNIX
  case AF_UNIX: DeeReturn_STATIC_STRING("AF_UNIX");
#endif
#if DEE_HAVE_AF_INET
  case AF_INET: DeeReturn_STATIC_STRING("AF_INET");
#endif
#if DEE_HAVE_AF_PACKET
  case AF_PACKET: DeeReturn_STATIC_STRING("AF_PACKET");
#endif
#if DEE_HAVE_AF_TIPC
  case AF_TIPC: DeeReturn_STATIC_STRING("AF_TIPC");
#endif
#if DEE_HAVE_AF_CAN
  case AF_CAN: DeeReturn_STATIC_STRING("AF_CAN");
#endif
#if DEE_HAVE_PF_SYSTEM
  case PF_SYSTEM: DeeReturn_STATIC_STRING("PF_SYSTEM");
#endif
#if DEE_HAVE_AF_IMPLINK
  case AF_IMPLINK: DeeReturn_STATIC_STRING("AF_IMPLINK");
#endif
#if DEE_HAVE_AF_PUP
  case AF_PUP: DeeReturn_STATIC_STRING("AF_PUP");
#endif
#if DEE_HAVE_AF_CHAOS
  case AF_CHAOS: DeeReturn_STATIC_STRING("AF_CHAOS");
#endif
#if DEE_HAVE_AF_NS
  case AF_NS: DeeReturn_STATIC_STRING("AF_NS");
#endif
#if DEE_HAVE_AF_IPX && (!DEE_HAVE_AF_NS || AF_IPX != AF_NS)
  case AF_IPX: DeeReturn_STATIC_STRING("AF_IPX");
#endif
#if DEE_HAVE_AF_ISO
  case AF_ISO: DeeReturn_STATIC_STRING("AF_ISO");
#endif
#if DEE_HAVE_AF_OSI && (!DEE_HAVE_AF_ISO || AF_OSI != AF_ISO)
  case AF_OSI: DeeReturn_STATIC_STRING("AF_OSI");
#endif
#if DEE_HAVE_AF_ECMA
  case AF_ECMA: DeeReturn_STATIC_STRING("AF_ECMA");
#endif
#if DEE_HAVE_AF_DATAKIT
  case AF_DATAKIT: DeeReturn_STATIC_STRING("AF_DATAKIT");
#endif
#if DEE_HAVE_AF_CCITT
  case AF_CCITT: DeeReturn_STATIC_STRING("AF_CCITT");
#endif
#if DEE_HAVE_AF_SNA
  case AF_SNA: DeeReturn_STATIC_STRING("AF_SNA");
#endif
#if DEE_HAVE_AF_DECnet
  case AF_DECnet: DeeReturn_STATIC_STRING("AF_DECnet");
#endif
#if DEE_HAVE_AF_DLI
  case AF_DLI: DeeReturn_STATIC_STRING("AF_DLI");
#endif
#if DEE_HAVE_AF_LAT
  case AF_LAT: DeeReturn_STATIC_STRING("AF_LAT");
#endif
#if DEE_HAVE_AF_HYLINK
  case AF_HYLINK: DeeReturn_STATIC_STRING("AF_HYLINK");
#endif
#if DEE_HAVE_AF_APPLETALK
  case AF_APPLETALK: DeeReturn_STATIC_STRING("AF_APPLETALK");
#endif
#if DEE_HAVE_AF_NETBIOS
  case AF_NETBIOS: DeeReturn_STATIC_STRING("AF_NETBIOS");
#endif
#if DEE_HAVE_AF_VOICEVIEW
  case AF_VOICEVIEW: DeeReturn_STATIC_STRING("AF_VOICEVIEW");
#endif
#if DEE_HAVE_AF_FIREFOX
  case AF_FIREFOX: DeeReturn_STATIC_STRING("AF_FIREFOX");
#endif
#if DEE_HAVE_AF_BAN
  case AF_BAN: DeeReturn_STATIC_STRING("AF_BAN");
#endif
#if DEE_HAVE_AF_ATM
  case AF_ATM: DeeReturn_STATIC_STRING("AF_ATM");
#endif
#if DEE_HAVE_AF_INET6
  case AF_INET6: DeeReturn_STATIC_STRING("AF_INET6");
#endif
#if DEE_HAVE_AF_CLUSTER
  case AF_CLUSTER: DeeReturn_STATIC_STRING("AF_CLUSTER");
#endif
#if DEE_HAVE_AF_12844
  case AF_12844: DeeReturn_STATIC_STRING("AF_12844");
#endif
#if DEE_HAVE_AF_IRDA
  case AF_IRDA: DeeReturn_STATIC_STRING("AF_IRDA");
#endif
#if DEE_HAVE_AF_NETDES
  case AF_NETDES: DeeReturn_STATIC_STRING("AF_NETDES");
#endif
#if DEE_HAVE_AF_TCNPROCESS
  case AF_TCNPROCESS: DeeReturn_STATIC_STRING("AF_TCNPROCESS");
#endif
#if DEE_HAVE_AF_TCNMESSAGE
  case AF_TCNMESSAGE: DeeReturn_STATIC_STRING("AF_TCNMESSAGE");
#endif
#if DEE_HAVE_AF_ICLFXBM
  case AF_ICLFXBM: DeeReturn_STATIC_STRING("AF_ICLFXBM");
#endif
#if DEE_HAVE_AF_BTH
  case AF_BTH: DeeReturn_STATIC_STRING("AF_BTH");
#endif
#if DEE_HAVE_AF_LINK
  case AF_LINK: DeeReturn_STATIC_STRING("AF_LINK");
#endif
#if DEE_HAVE_AF_NETLINK
  case AF_NETLINK: DeeReturn_STATIC_STRING("AF_NETLINK");
#endif
#if DEE_HAVE_AF_BLUETOOTH
  case AF_BLUETOOTH: DeeReturn_STATIC_STRING("AF_BLUETOOTH");
#endif
  default: return DEE_PP_CAT_2(DeeString_FromInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))(af);
 }
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//  -- DeeSocket_TypeToString
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSocket_TypeToString(DEE_A_IN int type) {
 switch (type) {
#ifdef SOCK_STREAM
  case SOCK_STREAM: DeeReturn_STATIC_STRING("SOCK_STREAM");
#endif
#ifdef SOCK_DGRAM
  case SOCK_DGRAM: DeeReturn_STATIC_STRING("SOCK_DGRAM");
#endif
#ifdef SOCK_RAW
  case SOCK_RAW: DeeReturn_STATIC_STRING("SOCK_RAW");
#endif
#ifdef SOCK_RDM
  case SOCK_RDM: DeeReturn_STATIC_STRING("SOCK_RDM");
#endif
#ifdef SOCK_SEQPACKET
  case SOCK_SEQPACKET: DeeReturn_STATIC_STRING("SOCK_SEQPACKET");
#endif
  default: break;
 }
 return DEE_PP_CAT_2(DeeString_FromInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))(type);
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//  -- DeeSocket_StringToProtocol
//  -- DeeSocket_ProtocolToString
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_EXCEPT(-1) int DeeSocket_StringToProtocol(
 DEE_A_IN_Z char const *name, DEE_A_OUT int *protocol) {
 struct protoent *ent;
 if DEE_LIKELY((ent = getprotobyname(name)) != NULL) {
  *protocol = ent->p_proto;
  return 0;
 }
 return DeeString_ToNumber(int,name,protocol);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSocket_ProtocolToString(DEE_A_IN int protocol) {
 struct protoent *ent;
 if DEE_LIKELY((ent = getprotobynumber(protocol)) != NULL) return DeeString_New(ent->p_name);
 return DEE_PP_CAT_2(DeeString_FromInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))(protocol);
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//  -- DeeSocket_OptLevelToString
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSocket_OptLevelToString(DEE_A_IN int level) {
 switch (level) {
#ifdef SOL_SOCKET
  case SOL_SOCKET:
#endif
  case DEE_INTERNAL_SOL_SOCKET:
   DeeReturn_STATIC_STRING("SOL_SOCKET");
#ifdef IPPROTO_HOPOPTS
  case IPPROTO_HOPOPTS: DeeReturn_STATIC_STRING("IPPROTO_HOPOPTS");
#endif
#ifdef DEE_PLATFORM_WINDOWS  // Same id as 'SOL_SOCKET' on non-windows
DEE_COMPILER_MSVC_WARNING_PUSH(4668)
#ifdef IPPROTO_ICMP
  case IPPROTO_ICMP: DeeReturn_STATIC_STRING("IPPROTO_ICMP");
#endif
#endif
DEE_COMPILER_MSVC_WARNING_POP
#ifdef IPPROTO_IGMP
  case IPPROTO_IGMP: DeeReturn_STATIC_STRING("IPPROTO_IGMP");
#endif
#ifdef IPPROTO_GGP
  case IPPROTO_GGP: DeeReturn_STATIC_STRING("IPPROTO_GGP");
#endif
#ifdef IPPROTO_IPV4
  case IPPROTO_IPV4: DeeReturn_STATIC_STRING("IPPROTO_IPV4");
#endif
#ifdef IPPROTO_ST
  case IPPROTO_ST: DeeReturn_STATIC_STRING("IPPROTO_ST");
#endif
#ifdef IPPROTO_TCP
  case IPPROTO_TCP: DeeReturn_STATIC_STRING("IPPROTO_TCP");
#endif
#ifdef IPPROTO_CBT
  case IPPROTO_CBT: DeeReturn_STATIC_STRING("IPPROTO_CBT");
#endif
#ifdef IPPROTO_EGP
  case IPPROTO_EGP: DeeReturn_STATIC_STRING("IPPROTO_EGP");
#endif
#ifdef IPPROTO_IGP
  case IPPROTO_IGP: DeeReturn_STATIC_STRING("IPPROTO_IGP");
#endif
#ifdef IPPROTO_PUP
  case IPPROTO_PUP: DeeReturn_STATIC_STRING("IPPROTO_PUP");
#endif
#ifdef IPPROTO_UDP
  case IPPROTO_UDP: DeeReturn_STATIC_STRING("IPPROTO_UDP");
#endif
#ifdef IPPROTO_IDP
  case IPPROTO_IDP: DeeReturn_STATIC_STRING("IPPROTO_IDP");
#endif
#ifdef IPPROTO_RDP
  case IPPROTO_RDP: DeeReturn_STATIC_STRING("IPPROTO_RDP");
#endif
#ifdef IPPROTO_IPV6
  case IPPROTO_IPV6: DeeReturn_STATIC_STRING("IPPROTO_IPV6");
#endif
#ifdef IPPROTO_ROUTING
  case IPPROTO_ROUTING: DeeReturn_STATIC_STRING("IPPROTO_ROUTING");
#endif
#ifdef IPPROTO_FRAGMENT
  case IPPROTO_FRAGMENT: DeeReturn_STATIC_STRING("IPPROTO_FRAGMENT");
#endif
#ifdef IPPROTO_ESP
  case IPPROTO_ESP: DeeReturn_STATIC_STRING("IPPROTO_ESP");
#endif
#ifdef IPPROTO_AH
  case IPPROTO_AH: DeeReturn_STATIC_STRING("IPPROTO_AH");
#endif
#ifdef IPPROTO_ICMPV6
  case IPPROTO_ICMPV6: DeeReturn_STATIC_STRING("IPPROTO_ICMPV6");
#endif
#ifdef IPPROTO_NONE
  case IPPROTO_NONE: DeeReturn_STATIC_STRING("IPPROTO_NONE");
#endif
#ifdef IPPROTO_DSTOPTS
  case IPPROTO_DSTOPTS: DeeReturn_STATIC_STRING("IPPROTO_DSTOPTS");
#endif
#ifdef IPPROTO_ND
  case IPPROTO_ND: DeeReturn_STATIC_STRING("IPPROTO_ND");
#endif
#ifdef IPPROTO_ICLFXBM
  case IPPROTO_ICLFXBM: DeeReturn_STATIC_STRING("IPPROTO_ICLFXBM");
#endif
#ifdef IPPROTO_PIM
  case IPPROTO_PIM: DeeReturn_STATIC_STRING("IPPROTO_PIM");
#endif
#ifdef IPPROTO_PGM
  case IPPROTO_PGM: DeeReturn_STATIC_STRING("IPPROTO_PGM");
#endif
#ifdef IPPROTO_L2TP
  case IPPROTO_L2TP: DeeReturn_STATIC_STRING("IPPROTO_L2TP");
#endif
#ifdef IPPROTO_SCTP
  case IPPROTO_SCTP: DeeReturn_STATIC_STRING("IPPROTO_SCTP");
#endif
#ifdef IPPROTO_RAW
  case IPPROTO_RAW: DeeReturn_STATIC_STRING("IPPROTO_RAW");
#endif
  default: break;
 }
 return DEE_PP_CAT_2(DeeString_FromInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))(level);
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//  -- DeeSocket_OptNameToString
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSocket_OptNameToString(DEE_A_IN int optname) {
 switch (optname) {
#ifdef SO_DEBUG
  case SO_DEBUG: DeeReturn_STATIC_STRING("SO_DEBUG");
#endif
#ifdef SO_ACCEPTCONN
  case SO_ACCEPTCONN:
#endif
  case DEE_INTERNAL_SO_ACCEPTCONN:
   DeeReturn_STATIC_STRING("SO_ACCEPTCONN");
#ifdef SO_REUSEADDR
  case SO_REUSEADDR: DeeReturn_STATIC_STRING("SO_REUSEADDR");
#endif
#ifdef SO_KEEPALIVE
  case SO_KEEPALIVE: DeeReturn_STATIC_STRING("SO_KEEPALIVE");
#endif
#ifdef SO_DONTROUTE
  case SO_DONTROUTE: DeeReturn_STATIC_STRING("SO_DONTROUTE");
#endif
#ifdef SO_BROADCAST
  case SO_BROADCAST: DeeReturn_STATIC_STRING("SO_BROADCAST");
#endif
#ifdef SO_USELOOPBACK
  case SO_USELOOPBACK: DeeReturn_STATIC_STRING("SO_USELOOPBACK");
#endif
#ifdef SO_LINGER
  case SO_LINGER: DeeReturn_STATIC_STRING("SO_LINGER");
#endif
#ifdef SO_OOBINLINE
  case SO_OOBINLINE: DeeReturn_STATIC_STRING("SO_OOBINLINE");
#endif
#ifdef SO_DONTLINGER
  case SO_DONTLINGER: DeeReturn_STATIC_STRING("SO_DONTLINGER");
#endif
#ifdef SO_EXCLUSIVEADDRUSE
  case SO_EXCLUSIVEADDRUSE: DeeReturn_STATIC_STRING("SO_EXCLUSIVEADDRUSE");
#endif
#ifdef SO_SNDBUF
  case SO_SNDBUF: DeeReturn_STATIC_STRING("SO_SNDBUF");
#endif
#ifdef SO_RCVBUF
  case SO_RCVBUF: DeeReturn_STATIC_STRING("SO_RCVBUF");
#endif
#ifdef SO_SNDLOWAT
  case SO_SNDLOWAT: DeeReturn_STATIC_STRING("SO_SNDLOWAT");
#endif
#ifdef SO_RCVLOWAT
  case SO_RCVLOWAT: DeeReturn_STATIC_STRING("SO_RCVLOWAT");
#endif
#ifdef SO_ERROR
  case SO_ERROR: DeeReturn_STATIC_STRING("SO_ERROR");
#endif
#ifdef SO_TYPE
  case SO_TYPE:
#endif
#if defined(SO_STYLE) && (!defined(SO_TYPE) || SO_STYLE != SO_TYPE)
  case SO_STYLE:
#endif
  case DEE_INTERNAL_SO_TYPE:
   DeeReturn_STATIC_STRING("SO_TYPE");
#ifdef SO_DOMAIN
  case SO_DOMAIN:
#endif
  case DEE_INTERNAL_SO_DOMAIN:
   DeeReturn_STATIC_STRING("SO_DOMAIN");
#ifdef SO_PROTOCOL
  case SO_PROTOCOL:
#endif
  case DEE_INTERNAL_SO_PROTOCOL:
   DeeReturn_STATIC_STRING("SO_PROTOCOL");
#ifdef SO_GROUP_ID
  case SO_GROUP_ID: DeeReturn_STATIC_STRING("SO_GROUP_ID");
#endif
#ifdef SO_GROUP_PRIORITY
  case SO_GROUP_PRIORITY: DeeReturn_STATIC_STRING("SO_GROUP_PRIORITY");
#endif
#ifdef SO_MAX_MSG_SIZE
  case SO_MAX_MSG_SIZE: DeeReturn_STATIC_STRING("SO_MAX_MSG_SIZE");
#endif
#ifdef SO_CONDITIONAL_ACCEPT
  case SO_CONDITIONAL_ACCEPT: DeeReturn_STATIC_STRING("SO_CONDITIONAL_ACCEPT");
#endif
#ifdef SO_RCVTIMEO
  case SO_RCVTIMEO: DeeReturn_STATIC_STRING("SO_RCVTIMEO");
#endif
#ifdef SO_SNDTIMEO
  case SO_SNDTIMEO: DeeReturn_STATIC_STRING("SO_SNDTIMEO");
#endif
#ifdef SO_BINDTODEVICE
  case SO_BINDTODEVICE: DeeReturn_STATIC_STRING("SO_BINDTODEVICE");
#endif
  default: break;
 }
 return DEE_PP_CAT_2(DeeString_FromInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))(optname);
}







//////////////////////////////////////////////////////////////////////////
// Socket VTable
static int DEE_CALL _deesocket_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSocketObject *self, DeeObject *args) {
 DeeObject *protocol = NULL;
 int sock_af,sock_type,sock_protocol;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"d|do:socket",&sock_af,&sock_type,&protocol) != 0) return -1;
 if (protocol) { if (DeeString_Check(protocol)
  ? DeeSocket_StringToProtocol(DeeString_STR(protocol),&sock_protocol) != 0
  : DeeObject_Cast(int,protocol,&sock_protocol) != 0) return -1;
 } else sock_protocol = 0;
 return DeeSocket_Init(self,sock_af,sock_type,sock_protocol);
}
static void DEE_CALL _deesocket_tp_dtor(DeeSocketObject *self) {
 DeeSocket_Close((DeeObject *)self);
 Dee_WEAKXDECREF(self->s_file);
}
static int DEE_CALL _deesocket_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSocketObject *self, DeeSocketObject *right) {
 self->s_type = right->s_type;
 self->s_protocol = right->s_protocol;
 DeeAtomicMutex_Init(&self->s_lock);
 self->s_uses = 0;
 DeeSocket_ACQUIRE(right);
 self->s_socket = right->s_socket;
 self->s_flags = right->s_flags;
 self->s_sock_addr = right->s_sock_addr;
 right->s_socket = INVALID_SOCKET;
 DeeSocket_RELEASE(right);
 // Still need to wait for the right socket to finish
 // >> Otherwise the left one might close the handle
 //    while a function on the right one is still using it...
 while (DeeAtomic16_Load(right->s_uses,memory_order_seq_cst))
  DeeThread_SleepNoInterrupt(1);
 return 0;
}
static int DEE_CALL _deesocket_tp_bool(DeeSocketObject *self) {
 return !DeeSocket_IsClosed((DeeObject *)self);
}
static DeeObject *DEE_CALL _deesocket_tp_not(DeeSocketObject *self) {
 DeeReturn_Bool(DeeSocket_IsClosed((DeeObject *)self));
}
static int DEE_CALL _deesocket_tp_hash(
 DeeSocketObject *self, Dee_hash_t start, Dee_hash_t *result) {
 sock_t hself;
 DeeSocket_ACQUIRE(self);
 hself = self->s_socket;
 DeeSocket_RELEASE(self);
 *result = start ^ (Dee_hash_t)hself;
 return 0;
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_lo(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs < hrhs);
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_le(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs <= hrhs);
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_eq(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs == hrhs);
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_ne(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs != hrhs);
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_gr(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs > hrhs);
}
static DeeObject *DEE_CALL _deesocket_tp_cmp_ge(
 DeeSocketObject *lhs, DeeSocketObject *rhs) {
 sock_t hlhs,hrhs;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&rhs,&DeeSocket_Type) != 0) return NULL;
 DeeSocket_ACQUIRE(lhs); hlhs = lhs->s_socket; DeeSocket_RELEASE(lhs);
 DeeSocket_ACQUIRE(rhs); hrhs = rhs->s_socket; DeeSocket_RELEASE(rhs);
 DeeReturn_Bool(hlhs >= hrhs);
}
DeeString_NEW_STATIC(_closed_socket_str,"<socket [CLOSED]>");
static DeeObject *DEE_CALL _deesocket_tp_str(DeeSocketObject *self) {
 struct DeeSockAddr sockaddr,peeraddr;
 int hassock,haspeer; sock_t sock; Dee_uint16_t flags;
 hassock = !DeeSocket_TryGetSockAddr((DeeObject *)self,&sockaddr);
 haspeer = !DeeSocket_TryGetPeerAddr((DeeObject *)self,&peeraddr);
 flags = DeeSocket_Flags((DeeObject *)self);
 if (hassock && haspeer) {
  return DeeString_Newf("<socket %K -> %K>",
                        DeeSockAddr_Str(&sockaddr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                        DeeSockAddr_Str(&peeraddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 } else if (hassock) {
  return DeeString_Newf("<socket %K -> void>",
                        DeeSockAddr_Str(&sockaddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 } else if (haspeer) {
  return DeeString_Newf("<socket void -> %K>",
                        DeeSockAddr_Str(&peeraddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 }
 DeeSocket_ACQUIRE(self);
 sock = self->s_socket;
 DeeSocket_RELEASE(self);
 if (sock == INVALID_SOCKET)
  DeeReturn_NEWREF((DeeObject *)&_closed_socket_str);
 return DeeString_Newf("<socket " DeeSocket_PRINTF ">",sock);
}
static DeeObject *DEE_CALL _deesocket_tp_repr(DeeSocketObject *self) {
 static char const _text_closed[] = "|CLOSED";
 static char const _text_bound[] = "|BOUND";
 static char const _text_binding[] = "|BINDING";
 static char const _text_connected[] = "|CONNECTED";
 static char const _text_connecting[] = "|CONNECTING";
 static char const _text_listening[] = "|LISTENING";
 static char const _text_startlistening[] = "|STARTLISTENING";
 static char const _text_shutdown[] = "|SHUTDOWN";
 static char const _text_shuttingdown[] = "|SHUTTINGDOWN";
 char buffer[Dee_MAX(2,Dee_MAX(sizeof(_text_closed),(
  Dee_MAX(sizeof(_text_bound),sizeof(_text_binding))+
  Dee_MAX(sizeof(_text_connected),sizeof(_text_connecting))+
  Dee_MAX(sizeof(_text_listening),sizeof(_text_startlistening))
  ))+Dee_MAX(sizeof(_text_shutdown),sizeof(_text_shuttingdown)))];
 struct DeeSockAddr sockaddr,peeraddr;
 int hassock,haspeer; sock_t sock; Dee_uint16_t flags;
 hassock = !DeeSocket_TryGetSockAddr((DeeObject *)self,&sockaddr);
 haspeer = !DeeSocket_TryGetPeerAddr((DeeObject *)self,&peeraddr);
 flags = DeeSocket_Flags((DeeObject *)self);
 buffer[0] = buffer[1] = 0;
 if ((flags&DEE_SOCKET_FLAG_CLOSED)!=0) strcat(buffer,_text_closed);
 else {
  if ((flags&DEE_SOCKET_FLAG_BOUND)!=0) strcat(buffer,_text_bound);
  else if ((flags&DEE_SOCKET_FLAG_BINDING)!=0) strcat(buffer,_text_binding);
  if ((flags&DEE_SOCKET_FLAG_CONNECTED)!=0) strcat(buffer,_text_connected);
  else if ((flags&DEE_SOCKET_FLAG_CONNECTING)!=0) strcat(buffer,_text_connecting);
  if ((flags&DEE_SOCKET_FLAG_LISTENING)!=0) strcat(buffer,_text_listening);
  else if ((flags&DEE_SOCKET_FLAG_STARTLISTENING)!=0) strcat(buffer,_text_startlistening);
 }
 if ((flags&DEE_SOCKET_FLAG_SHUTDOWN)!=0) strcat(buffer,_text_shutdown);
 else if ((flags&DEE_SOCKET_FLAG_SHUTTINGDOWN)!=0) strcat(buffer,_text_shuttingdown);
 if (hassock && haspeer) {
  return DeeString_Newf("<socket[%s] %K -> %K>",buffer+1,
                        DeeSockAddr_Str(&sockaddr,DEE_SOCKADDR_STR_FLAG_NOFAIL),
                        DeeSockAddr_Str(&peeraddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 } else if (hassock) {
  return DeeString_Newf("<socket[%s] %K -> void>",buffer+1,
                        DeeSockAddr_Str(&sockaddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 } else if (haspeer) {
  return DeeString_Newf("<socket[%s] void -> %K>",buffer+1,
                        DeeSockAddr_Str(&peeraddr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
 }
 DeeSocket_ACQUIRE(self);
 sock = self->s_socket;
 DeeSocket_RELEASE(self);
 if DEE_UNLIKELY(sock == INVALID_SOCKET)
  DeeReturn_NEWREF((DeeObject *)&_closed_socket_str);
 return DeeString_Newf("<socket[%s] " DeeSocket_PRINTF ">",buffer+1,sock);
}

static DeeObject *DEE_CALL _deesocket_is_bound(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_bound") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsBound((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_binding(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_binding") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsBinding((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_connected(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_connected") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsConnected((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_connecting(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_connecting") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsConnecting((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_listening(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_listening") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsListening((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_listenstarting(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_listenstarting") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsListenStarting((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_shutdown(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_shutdown") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsShutDown((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_shuttingdown(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_shuttingdown") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsShuttingDown((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_is_closed(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_closed") != 0) return NULL;
 DeeReturn_Bool(DeeSocket_IsClosed((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_shutdown(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result,how = SHUT_RDWR;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|d:shutdown") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSocket_Shutdown((DeeObject *)self,how)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deesocket_close(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":close") != 0) return NULL;
 DeeSocket_Close((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesocket_flags(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":flags") != 0) return NULL;
 return DeeObject_New(Dee_uint16_t,DeeSocket_Flags((DeeObject *)self));
}
static DeeObject *DEE_CALL _deesocket_getsockopt(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int sock_level,sock_optname;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"dd:getsockopt",&sock_level,&sock_optname) != 0) return NULL;
 return DeeSocket_GetSockOpt((DeeObject *)self,sock_level,sock_optname);
}
static DeeObject *DEE_CALL _deesocket_setsockopt(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int sock_level,sock_optname;
 if DEE_UNLIKELY(DeeTuple_SIZE(args) < 2) {
  DeeError_TypeError_ArgCountExpectedNamedMin("setsockopt",2,DeeTuple_SIZE(args));
  return NULL;
 }
 if DEE_UNLIKELY(DeeObject_Cast(int,DeeTuple_GET(args,0),&sock_level) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,DeeTuple_GET(args,1),&sock_optname) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SetSockOpt((DeeObject *)self,sock_level,sock_optname,
  DeeTuple_SIZE(args)-2,DeeTuple_ELEM(args)+2) != 0) return NULL;
 DeeReturn_None;
}

static DeeObject *DEE_CALL _deesocket_bind(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeSockAddr addr; int result;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
  DeeSocket_GET_ADDRESS_FAMILY(self),DeeSocket_GET_PROTOCOL(self),
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSocket_Bind((DeeObject *)self,&addr)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deesocket_connect(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeSockAddr addr; int result;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
  DeeSocket_GET_ADDRESS_FAMILY(self),DeeSocket_GET_PROTOCOL(self),
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSocket_Connect((DeeObject *)self,&addr)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deesocket_listen(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result,max_backlog = DEE_SOCKET_MAXBACKLOG_DEFAULT;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|d:listen",&max_backlog) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSocket_ListenEx((DeeObject *)self,max_backlog)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deesocket_accept(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":accept") != 0) return NULL;
 return DeeSocket_Accept((DeeObject *)self);
}
static DeeObject *DEE_CALL _deesocket_try_accept(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *socket; int success;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":try_accept") != 0) return NULL;
 if DEE_UNLIKELY((success = DeeSocket_TryAcceptEx((DeeObject *)self,&socket)) < 0) return NULL;
 return success == 0 ? socket : DeeNone_New();
}
static DeeObject *DEE_CALL _deesocket_accept_timed(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *socket; int success; unsigned int n_msecs;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"u:accept_timed",&n_msecs) != 0) return NULL;
 if DEE_UNLIKELY((success = DeeSocket_AcceptTimedEx((DeeObject *)self,n_msecs,&socket)) < 0) return NULL;
 return success == 0 ? socket : DeeNone_New();
}
static DeeObject *DEE_CALL _deesocket_recv(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; DeeObject *data = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:recv",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:recv",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (data == NULL) return DeeSocket_RecvData((DeeObject *)self,(Dee_size_t)-1);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvAll((DeeObject *)self,p,s) != 0) return NULL;
  DeeReturn_None;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
 return DeeSocket_RecvData((DeeObject *)self,max_read);
}
static DeeObject *DEE_CALL _deesocket_recv_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; int flags;
 DeeObject *a,*b = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:recv_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:recv_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (b == NULL) {
  if DEE_UNLIKELY(DeeObject_Cast(int,a,&flags) != 0) return NULL;
  return DeeSocket_RecvDataEx((DeeObject *)self,(Dee_size_t)-1,flags);
 }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c != NULL) {
  void *p;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvAllEx((DeeObject *)self,p,max_read,flags) != 0) return NULL;
  DeeReturn_None;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,a,&max_read) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
 return DeeSocket_RecvDataEx((DeeObject *)self,max_read,flags);
}
static DeeObject *DEE_CALL _deesocket_recvall(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; DeeObject *data = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:recvall",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:recvall",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (data == NULL) return DeeSocket_RecvAllData((DeeObject *)self,(Dee_size_t)-1);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvAll((DeeObject *)self,p,max_read) != 0) return NULL;
  DeeReturn_None;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
 return DeeSocket_RecvAllData((DeeObject *)self,max_read);
}
static DeeObject *DEE_CALL _deesocket_recvall_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; int flags;
 DeeObject *a,*b = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:recvall_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:recvall_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (b == NULL) {
  if DEE_UNLIKELY(DeeObject_Cast(int,a,&flags) != 0) return NULL;
  return DeeSocket_RecvAllDataEx((DeeObject *)self,(Dee_size_t)-1,flags);
 }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c == NULL) {
  void *p;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvAllEx((DeeObject *)self,p,max_read,flags) != 0) return NULL;
  DeeReturn_None;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,a,&max_read) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
 return DeeSocket_RecvAllDataEx((DeeObject *)self,max_read,flags);
}
static DeeObject *DEE_CALL _deesocket_maybe_recv(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; DeeObject *data = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:maybe_recv",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:maybe_recv",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (data == NULL) return DeeSocket_RecvData((DeeObject *)self,(Dee_size_t)-1);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p; Dee_size_t result;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_Recv((DeeObject *)self,p,max_read,&result) != 0) return NULL;
  return DeeObject_New(Dee_size_t,result);
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
 return DeeSocket_RecvData((DeeObject *)self,max_read);
}
static DeeObject *DEE_CALL _deesocket_maybe_recv_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read; int flags;
 DeeObject *a,*b = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:maybe_recv_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:maybe_recv_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (b == NULL) {
  if DEE_UNLIKELY(DeeObject_Cast(int,a,&flags) != 0) return NULL;
  return DeeSocket_RecvDataEx((DeeObject *)self,(Dee_size_t)-1,flags);
 }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c != NULL) {
  void *p; Dee_size_t result;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvEx((DeeObject *)self,p,max_read,&result,flags) != 0) return NULL;
  return DeeObject_New(Dee_size_t,result);
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,a,&max_read) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
 return DeeSocket_RecvDataEx((DeeObject *)self,max_read,flags);
}
static DeeObject *DEE_CALL _deesocket_send(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:send",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:send",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_SendAll((DeeObject *)self,p,s) != 0) return NULL;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SendAll((DeeObject *)self,DeeString_STR(data),DeeString_SIZE(data)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesocket_send_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int flags; DeeObject *a,*b;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:send_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:send_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c != NULL) {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_SendAllEx((DeeObject *)self,p,s,flags) != 0) return NULL;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(a,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SendAllEx((DeeObject *)self,DeeString_STR(a),DeeString_SIZE(a),flags) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesocket_maybe_send(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t result;
 DeeObject *data;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:maybe_send",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:maybe_send",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_Send((DeeObject *)self,p,s,&result) != 0) return NULL;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_Send((DeeObject *)self,DeeString_STR(data),DeeString_SIZE(data),&result) != 0) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *DEE_CALL _deesocket_maybe_send_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int flags; Dee_size_t result;
 DeeObject *a,*b;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:maybe_send_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:maybe_send_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c != NULL) {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeSocket_SendEx((DeeObject *)self,p,s,&result,flags) != 0) return NULL;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(a,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SendEx((DeeObject *)self,DeeString_STR(a),DeeString_SIZE(a),&result,flags) != 0) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *DEE_CALL _deesocket_sendto(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 void *p; Dee_size_t argc,s,ws; DeeObject **argv,*arg;
 struct DeeSockAddr addr;
 argc = DeeTuple_SIZE(args);
 argv = DeeTuple_ELEM(args);
 if (argc >= 1 && DeeString_Check(argv[argc-1])) {
  arg = argv[argc-1]; p = DeeString_STR(arg); s = DeeString_SIZE(arg);
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-1,argv) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 } else if (argc >= 2) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(argv[argc-2],&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,argv[argc-1],&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-2,argv) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 } else {
  DeeError_TypeError_ArgCountExpectedNamedMin("sendto",2,argc);
  return NULL;
 }
 if DEE_UNLIKELY(DeeSocket_SendTo((DeeObject *)self,p,s,&ws,&addr) != 0) return NULL;
 if DEE_UNLIKELY(ws != s) {
  DeeError_Throw((DeeObject *)&_DeeErrorInstance_FileCantWrite);
  return NULL;
 }
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesocket_sendto_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 void *p; Dee_size_t argc,s,ws; DeeObject **argv,*arg;
 struct DeeSockAddr addr; int flags;
 argc = DeeTuple_SIZE(args);
 argv = DeeTuple_ELEM(args);
 if (argc >= 2 && DeeString_Check(argv[argc-2])) {
  arg = argv[argc-2]; p = DeeString_STR(arg); s = DeeString_SIZE(arg);
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-2,argv) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 } else if (argc >= 3) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(argv[argc-3],&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,argv[argc-2],&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-3,argv) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 } else {
  DeeError_TypeError_ArgCountExpectedNamedMin("sendto_ex",3,argc);
  return NULL;
 }
 if DEE_UNLIKELY(DeeObject_Cast(int,argv[argc-1],&flags) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SendToEx((DeeObject *)self,p,s,&ws,&addr,flags) != 0) return NULL;
 if DEE_UNLIKELY(ws != s) {
  DeeError_Throw((DeeObject *)&_DeeErrorInstance_FileCantWrite);
  return NULL;
 }
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deesocket_maybe_sendto(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 void *p; Dee_size_t argc,s,ws; DeeObject **argv,*arg;
 struct DeeSockAddr addr;
 argc = DeeTuple_SIZE(args);
 argv = DeeTuple_ELEM(args);
 if (argc >= 1 && DeeString_Check(argv[argc-1])) {
  arg = argv[argc-1],p = DeeString_STR(arg),s = DeeString_SIZE(arg);
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-1,argv) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 } else if (argc >= 2) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(argv[argc-2],&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,argv[argc-1],&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-2,argv) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 } else {
  DeeError_TypeError_ArgCountExpectedNamedMin("maybe_sendto",2,argc);
  return NULL;
 }
 if DEE_UNLIKELY(DeeSocket_SendTo((DeeObject *)self,p,s,&ws,&addr) != 0) return NULL;
 return DeeObject_New(Dee_size_t,ws);
}
static DeeObject *DEE_CALL _deesocket_maybe_sendto_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 void *p; Dee_size_t argc,s,ws; DeeObject **argv,*arg;
 struct DeeSockAddr addr; int flags;
 argc = DeeTuple_SIZE(args);
 argv = DeeTuple_ELEM(args);
 if (argc >= 2 && DeeString_Check(argv[argc-2])) {
  arg = argv[argc-2],p = DeeString_STR(arg),s = DeeString_SIZE(arg);
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-2,argv) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 } else if (argc >= 3) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(argv[argc-3],&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,argv[argc-2],&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
   DeeSocket_GET_ADDRESS_FAMILY(self),
   DeeSocket_GET_PROTOCOL(self),argc-3,argv) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 } else {
  DeeError_TypeError_ArgCountExpectedNamedMin("sendto_ex",3,argc);
  return NULL;
 }
 if DEE_UNLIKELY(DeeObject_Cast(int,argv[argc-1],&flags) != 0) return NULL;
 if DEE_UNLIKELY(DeeSocket_SendToEx((DeeObject *)self,p,s,&ws,&addr,flags) != 0) return NULL;
 return DeeObject_New(Dee_size_t,ws);
}

static DeeObject *DEE_CALL _deesocket_recvfrom(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *addr; Dee_size_t max_read;
 DeeObject *result_data,*data = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:recvfrom",&data,&size) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:recvfrom",&data) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (size != NULL) {
  void *p; Dee_size_t rs;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  if DEE_UNLIKELY((addr = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvFrom((DeeObject *)self,p,max_read,&rs,&addr->sa_addr) != 0) goto err_addr;
  return Dee_BuildValue("(OIu)",addr,rs);
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (data) {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
 } else max_read = (Dee_size_t)-1;
 if DEE_UNLIKELY((addr = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY((result_data = DeeSocket_RecvDataFrom((DeeObject *)self,max_read,&addr->sa_addr)) == NULL) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
err_addr:
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  Dee_DECREF(addr);
  return NULL;
 }
 return Dee_BuildValue("(OO)",addr,result_data);
}
static DeeObject *DEE_CALL _deesocket_recvfrom_ex(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *addr; Dee_size_t max_read;
 int flags; DeeObject *result_data,*a,*b = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:recvfrom_ex",&a,&b,&c) != 0) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:recvfrom_ex",&a,&b) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (c != NULL) {
  void *p; Dee_size_t rs;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(a,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,b,&max_read) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(int,c,&flags) != 0) return NULL;
  if DEE_UNLIKELY((addr = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
  if DEE_UNLIKELY(DeeSocket_RecvFromEx((DeeObject *)self,p,max_read,&rs,&addr->sa_addr,flags) != 0) goto err_addr;
  return Dee_BuildValue("(OIu)",addr,rs);
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (b) {
  if DEE_UNLIKELY(DeeObject_Cast(int,b,&flags) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,a,&max_read) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeObject_Cast(int,a,&flags) != 0) return NULL;
  max_read = (Dee_size_t)-1;
 }
 if DEE_UNLIKELY((addr = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY((result_data = DeeSocket_RecvDataFromEx((DeeObject *)self,max_read,&addr->sa_addr,flags)) == NULL) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
err_addr:
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  Dee_DECREF(addr);
  return NULL;
 }
 return Dee_BuildValue("(OO)",addr,result_data);
}
static DeeSockAddrObject *DEE_CALL _deesocket_sockaddr_get(DeeSocketObject *self, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *result;
 if DEE_UNLIKELY((result = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSocket_GetSockAddr((DeeObject *)self,&result->sa_addr) != 0) Dee_CLEAR(result);
 return result;
}
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static int DEE_CALL _deesocket_sockaddr_set(
 DeeSocketObject *self, DeeSockAddrObject *value, void *DEE_UNUSED(closure)) {
 if (DeeObject_InplaceGetInstance(&value,&DeeSockAddr_Type) != 0) return -1;
 memcpy(&self->s_sock_addr,&value->sa_addr,sizeof(struct DeeSockAddr));
 return 0;
}
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static DeeObject *DEE_CALL _deesocket_peeraddr_get(DeeSocketObject *self, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *result;
 if DEE_UNLIKELY((result = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSocket_GetPeerAddr((DeeObject *)self,&result->sa_addr) != 0) Dee_CLEAR(result);
 return (DeeObject *)result;
}
static DeeObject *DEE_CALL _deesocket_file_get(
 DeeSocketObject *self, void *DEE_UNUSED(closure)) {
 return DeeSocket_File((DeeObject *)self);
}
static DeeObject *DEE_CALL _deesocketclass_tcp_server(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t port; int max_backlog = -1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u|d:tcp_server",&port,&max_backlog) != 0) return NULL;
 return DeeSocket_NewTCPServerEx(port,max_backlog);
}
static DeeObject *DEE_CALL _deesocketclass_tcp_client(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeSockAddr addr;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,DEE_AF_AUTO,DEE_TCP_PROTOCOL,
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 return DeeSocket_NewTCPClient(&addr);
}
static DeeObject *DEE_CALL _deesocketclass_tcp(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t port; struct DeeSockAddr addr;
 if (DeeTuple_SIZE(args) == 1 && !DeeString_Check(DeeTuple_GET(args,0))) {
  // Server
  if DEE_UNLIKELY(DeeObject_Cast(Dee_uint16_t,DeeTuple_GET(args,0),&port) != 0) return NULL;
  return DeeSocket_NewTCPServer(port);
 }
 // Client
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,DEE_AF_AUTO,
  DEE_TCP_PROTOCOL,DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 return DeeSocket_NewTCPClient(&addr);
}
static DeeObject *DEE_CALL _deesocketclass_udp(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t port;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:udp",&port) != 0) return NULL;
 return DeeSocket_NewUDP(port);
}
static DeeObject *DEE_CALL _deesocket_fileto(
 DeeSocketObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeSockAddr addr;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&addr,
  DeeSocket_GET_ADDRESS_FAMILY(self),
  DeeSocket_GET_PROTOCOL(self),
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 return DeeSocket_FileTo((DeeObject *)self,&addr);
}








//////////////////////////////////////////////////////////////////////////
// Socket File VTable
static void DEE_CALL _deesocketfile_tp_dtor(DeeSocketFileObject *self) {
 Dee_DECREF(self->sf_socket);
}
static int DEE_CALL _deesocketfile_tp_io_read(
 DeeSocketFileObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 return DeeSocket_Recv((DeeObject *)self->sf_socket,p,s,rs);
}
static int DEE_CALL _deesocketfile_tp_io_write(
 DeeSocketFileObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 return DeeSocket_Send((DeeObject *)self->sf_socket,p,s,ws);
}
static int DEE_CALL _deesocketfile_tp_io_seek(
 DeeSocketFileObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *DEE_UNUSED(pos)) {
 DeeError_NotImplemented_str("seek");
 return -1;
}
static int DEE_CALL _deesocketfile_tp_io_flush(DeeSocketFileObject *DEE_UNUSED(self)) {
 return 0;
}
static int DEE_CALL _deesocketfile_tp_io_trunc(DeeSocketFileObject *DEE_UNUSED(self)) {
 DeeError_NotImplemented_str("trunc");
 return -1;
}
static void DEE_CALL _deesocketfile_tp_io_close(DeeSocketFileObject *self) {
 DeeSocket_Close((DeeObject *)self->sf_socket);
}
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesocketfile_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef _deesocketfile_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__sf_socket",DeeSocketFileObject,sf_socket,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */








//////////////////////////////////////////////////////////////////////////
// UDP File VTable
static void DEE_CALL _deesocketfileto_tp_dtor(DeeSocketFileToObject *self) {
 Dee_DECREF(self->sft_socket);
}
static int DEE_CALL _deesocketfileto_tp_io_read(
 DeeSocketFileToObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 struct DeeSockAddr sender; int result;
 while (1) {
  result = DeeSocket_RecvFrom((DeeObject *)self->sft_socket,p,s,rs,&sender);
  if DEE_UNLIKELY(result != 0) return result;
  if (DeeSockAddr_Compare(&self->sft_peer_addr,&sender) == 0) break;
 }
 return 0;
}
static int DEE_CALL _deesocketfileto_tp_io_write(
 DeeSocketFileToObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 return DeeSocket_SendTo((DeeObject *)self->sft_socket,p,s,ws,&self->sft_peer_addr);
}
static int DEE_CALL _deesocketfileto_tp_io_seek(
 DeeSocketFileToObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *DEE_UNUSED(pos)) {
 DeeError_NotImplemented_str("seek");
 return -1;
}
static int DEE_CALL _deesocketfileto_tp_io_flush(DeeSocketFileToObject *DEE_UNUSED(self)) {
 return 0;
}
static int DEE_CALL _deesocketfileto_tp_io_trunc(DeeSocketFileToObject *DEE_UNUSED(self)) {
 DeeError_NotImplemented_str("trunc");
 return -1;
}
static void DEE_CALL _deesocketfileto_tp_io_close(DeeSocketFileToObject *self) {
 DeeSocket_Close((DeeObject *)self->sft_socket);
}

static DeeSockAddrObject *DEE_CALL _deesocketfileto_peer_get(
 DeeSocketFileToObject *self, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *result;
 if ((result = (DeeSockAddrObject *)DeeSockAddr_Alloc()) == NULL) return NULL;
 memcpy(&result->sa_addr,&self->sft_peer_addr,sizeof(struct DeeSockAddr));
 return result;
}

#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static int DEE_CALL _deesocketfileto_peer_set(
 DeeSocketFileToObject *self, DeeSockAddrObject *value, void *DEE_UNUSED(closure)) {
 if (DeeObject_InplaceGetInstance(&value,&DeeSockAddr_Type) != 0) return -1;
 memcpy(&self->sft_peer_addr,&value->sa_addr,sizeof(struct DeeSockAddr));
 return 0;
}
#endif

static struct DeeGetSetDef _deesocketfileto_tp_getsets[] = {
 DEE_GETSETDEF_v100("peer",member(&_deesocketfileto_peer_get),null,null,"-> sockaddr"),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_GETSETDEF_v100("__sft_peer_addr",
  member(&_deesocketfileto_peer_get),null,
  member(&_deesocketfileto_peer_set),"-> sockaddr"),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_GETSETDEF_END_v100
};

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesocketfileto_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef _deesocketfileto_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__sft_socket",DeeSocketFileToObject,sft_socket,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */










static struct DeeGetSetDef _deesocket_tp_getsets[] = {
 DEE_GETSETDEF_v100("sockaddr",member(&_deesocket_sockaddr_get),null,null,"sockaddr -> sockaddr"),
 DEE_GETSETDEF_v100("peeraddr",member(&_deesocket_peeraddr_get),null,null,"peeraddr -> sockaddr"),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_GETSETDEF_v100("__s_sock_addr",
  member(&_deesocket_sockaddr_get),null,
  member(&_deesocket_sockaddr_set),"sockaddr -> sockaddr"),
#endif
 DEE_GETSETDEF_v100("file",member(&_deesocket_file_get),null,null,"file -> socket.tcp.file"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMemberDef _deesocket_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeSocketObject,s_socket,DeeSocketHandle),
 DEE_MEMBERDEF_NAMED_RO_v100("address_family",DeeSocketObject,s_sock_addr.sa.sa_family,sa_family_t),
 DEE_MEMBERDEF_NAMED_RO_v100("type",DeeSocketObject,s_type,int),
 DEE_MEMBERDEF_NAMED_RO_v100("protocol",DeeSocketObject,s_protocol,int),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__s_lock",DeeSocketObject,s_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_socket",DeeSocketObject,s_socket,DeeSocketHandle),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_uses",DeeSocketObject,s_uses,Dee_uint16_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_flags",DeeSocketObject,s_flags,Dee_uint16_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_type",DeeSocketObject,s_type,int),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_protocol",DeeSocketObject,s_protocol,int),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_file",DeeSocketObject,s_file,weak_object_null),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef _deesocket_tp_class_methods[] = {
 DEE_METHODDEF_v100("tcp",member(&_deesocketclass_tcp),"tcp(uint16_t port) -> socket\ntcp(...) -> socket"),
 DEE_METHODDEF_v100("tcp_server",member(&_deesocketclass_tcp_server),"tcp_server(uint16_t port) -> socket"),
 DEE_METHODDEF_v100("tcp_client",member(&_deesocketclass_tcp_client),"tcp_client(...) -> socket"),
 DEE_METHODDEF_v100("udp",member(&_deesocketclass_udp),"udp(uint16_t port) -> socket"),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef _deesocket_tp_methods[] = {
 DEE_METHODDEF_v100("flags",member(&_deesocket_flags),"() -> uint16_t\n"
  "\tReturns a set of 'socket.FLAG_*' flags"),
 DEE_METHODDEF_v100("close",member(&_deesocket_close),"() -> none"),
 DEE_METHODDEF_v100("bind",member(&_deesocket_bind),
  "(...) -> bool\n"
  "\tBind the socket to a given address. Returns false if the socket was already bound"),
 DEE_METHODDEF_v100("connect",member(&_deesocket_connect),
  "(...) -> bool\n"
  "\tConnect the socket to a given address. Returns false if the socket was already connected"),
 DEE_METHODDEF_v100("listen",member(&_deesocket_listen),
  "(int max_backlog = -1) -> bool"),
 DEE_METHODDEF_v100("accept",member(&_deesocket_accept),
  "() -> socket"),
 DEE_METHODDEF_v100("try_accept",member(&_deesocket_try_accept),
  "() -> socket\ntry_accept() -> none"),
 DEE_METHODDEF_v100("accept_timed",member(&_deesocket_accept_timed),
  "(unsigned int msecs) -> socket\n"
  "(unsigned int msecs) -> none"),
 DEE_METHODDEF_v100("recv",member(&_deesocket_recv),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> none"),
 DEE_METHODDEF_v100("recv_ex",member(&_deesocket_recv_ex),
  "(int flags) -> string\n"
  "(size_t max, int flags) -> string\n"
  "(none *p, size_t size, int flags) -> none"),
 DEE_METHODDEF_v100("recvall",member(&_deesocket_recvall),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> none"),
 DEE_METHODDEF_v100("recvall_ex",member(&_deesocket_recvall_ex),
  "(int flags) -> string\n"
  "(size_t max, int flags) -> string\n"
  "(none *p, size_t size, int flags) -> none"),
 DEE_METHODDEF_v100("maybe_recv",member(&_deesocket_maybe_recv),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> size_t"),
 DEE_METHODDEF_v100("maybe_recv_ex",member(&_deesocket_maybe_recv_ex),
  "(int flags) -> string\n"
  "(size_t max, int flags) -> string\n"
  "(none *p, size_t size, int flags) -> size_t"),
 DEE_METHODDEF_v100("send",member(&_deesocket_send),
  "(string data) -> none\n"
  "(none *p, size_t size) -> none"),
 DEE_METHODDEF_v100("send_ex",member(&_deesocket_send_ex),
  "(string data, int flags) -> none\n"
  "(none *p, size_t size, int flags) -> none"),
 DEE_METHODDEF_v100("maybe_send",member(&_deesocket_maybe_send),
  "(string data) -> size_t\n"
  "(none *p, size_t size) -> size_t"),
 DEE_METHODDEF_v100("maybe_send_ex",member(&_deesocket_maybe_send_ex),
  "(string data, int flags) -> size_t\n"
  "(none *p, size_t size, int flags) -> size_t"),
 DEE_METHODDEF_v100("sendto",member(&_deesocket_sendto),
  "(..., string data) -> none\n"
  "(..., none *p, size_t size) -> none\n"),
 DEE_METHODDEF_v100("sendto_ex",member(&_deesocket_sendto_ex),
  "(..., string data, int flags) -> none\n"
  "(..., none *p, size_t size, int flags) -> none\n"),
 DEE_METHODDEF_v100("maybe_sendto",member(&_deesocket_maybe_sendto),
  "(..., string data) -> size_t\n"
  "(..., none *p, size_t size) -> size_t\n"),
 DEE_METHODDEF_v100("maybe_sendto_ex",member(&_deesocket_maybe_sendto_ex),
  "(..., string data, int flags) -> size_t\n"
  "(..., none *p, size_t size, int flags) -> size_t\n"),
 DEE_METHODDEF_v100("recvfrom",member(&_deesocket_recvfrom),
  "(size_t max_size = size_t(-1)) -> (sockaddr,string)\n"
  "(none *p, size_t size) -> (sockaddr,size_t)\n"),
 DEE_METHODDEF_v100("recvfrom_ex",member(&_deesocket_recvfrom_ex),
  "(int flags) -> (sockaddr,string)\n"
  "(size_t max_size, int flags) -> (sockaddr,string)\n"
  "(none *p, size_t size, int flags) -> (sockaddr,size_t)\n"),
 DEE_METHODDEF_v100("shutdown",member(&_deesocket_shutdown),
  "(int how = socket.SHUT_RDWR) -> bool\n"
  "\tReturns false if the socket was already shut down/closed"),
 DEE_METHODDEF_v100("getsockopt",member(&_deesocket_getsockopt),
  "(int level, int optname) -> object\n"
  "(int level, string optname) -> object\n"
  "(string level, int optname) -> object\n"
  "(string level, string optname) -> object"),
 DEE_METHODDEF_v100("setsockopt",member(&_deesocket_setsockopt),
  "(int level, int optname, ...) -> none\n"
  "(int level, string optname, ...) -> none\n"
  "(string level, int optname, ...) -> none\n"
  "(string level, string optname, ...) -> none"),
 DEE_METHODDEF_v100("is_bound",member(&_deesocket_is_bound),
  "() -> bool\n@return: true if 'bind' was called on the socket"),
 DEE_METHODDEF_v100("is_binding",member(&_deesocket_is_binding),
  "() -> bool\n@return: true during a 'bind' called with the socket"),
 DEE_METHODDEF_v100("is_connected",member(&_deesocket_is_connected),
  "() -> bool\n@return: true if 'connect' was called on the socket"),
 DEE_METHODDEF_v100("is_connecting",member(&_deesocket_is_connecting),
  "() -> bool\n@return: true during a 'connect' called with the socket"),
 DEE_METHODDEF_v100("is_listening",member(&_deesocket_is_listening),
  "() -> bool\n@return: true if 'listen' was called on the socket"),
 DEE_METHODDEF_v100("is_listenstarting",member(&_deesocket_is_listenstarting),
  "() -> bool\n@return: true during a 'listen' called with the socket"),
 DEE_METHODDEF_v100("is_shutdown",member(&_deesocket_is_shutdown),
  "() -> bool\n@return: true if 'shutdown' or 'close' was called on the socket"),
 DEE_METHODDEF_v100("is_shuttingdown",member(&_deesocket_is_shuttingdown),
  "() -> bool\n@return: true during a 'shutdown' called with the socket"),
 DEE_METHODDEF_v100("is_closed",member(&_deesocket_is_closed),
  "() -> bool\n@return: true if 'close' was called on the socket"),
 DEE_METHODDEF_v100("fileto",member(&_deesocket_fileto),
  "(...) -> socket.fileto\n@return: A file object bound to a given address"),
 DEE_METHODDEF_END_v100
};

#if DEE_HAVE_AF_BLUETOOTH
DeeString_NEW_STATIC(_deestr_BDADDR_ANY,"00:00:00:00:00:00");
DeeString_NEW_STATIC(_deestr_BDADDR_ALL,"FF:FF:FF:FF:FF:FF");
DeeString_NEW_STATIC(_deestr_BDADDR_LOCAL,"00:00:00:FF:FF:FF");
#endif
#if DEE_HAVE_AF_INET6
DeeString_NEW_STATIC(_deestr_IN6ADDR_ANY,"::0");
DeeString_NEW_STATIC(_deestr_IN6ADDR_LOOPBACK,"::1");

#if 0 // TODO...
#define IN6ADDR_ALLNODESONNODE_INIT { \
    0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 \
}

#define IN6ADDR_ALLNODESONLINK_INIT { \
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 \
}

#define IN6ADDR_ALLROUTERSONLINK_INIT { \
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 \
}

#define IN6ADDR_ALLMLDV2ROUTERSONLINK_INIT { \
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16 \
}
#endif
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4574)
static struct DeeMemberDef _deesocket_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("sockaddr",object,&DeeSockAddr_Type),
 DEE_MEMBERDEF_CONST_v100("file",object,&DeeSocketFile_Type),
 DEE_MEMBERDEF_CONST_v100("fileto",object,&DeeSocketFileTo_Type),
 DEE_MEMBERDEF_CONST_v100("SHUT_RD",int,SHUT_RD),
 DEE_MEMBERDEF_CONST_v100("SHUT_WR",int,SHUT_WR),
 DEE_MEMBERDEF_CONST_v100("SHUT_RDWR",int,SHUT_RDWR),

 // Socket flags
 DEE_MEMBERDEF_CONST_v100("FLAG_BINDING",Dee_uint16_t,DEE_SOCKET_FLAG_BINDING),
 DEE_MEMBERDEF_CONST_v100("FLAG_BOUND",Dee_uint16_t,DEE_SOCKET_FLAG_BOUND),
 DEE_MEMBERDEF_CONST_v100("FLAG_CONNECTING",Dee_uint16_t,DEE_SOCKET_FLAG_CONNECTING),
 DEE_MEMBERDEF_CONST_v100("FLAG_CONNECTED",Dee_uint16_t,DEE_SOCKET_FLAG_CONNECTED),
 DEE_MEMBERDEF_CONST_v100("FLAG_STARTLISTENING",Dee_uint16_t,DEE_SOCKET_FLAG_STARTLISTENING),
 DEE_MEMBERDEF_CONST_v100("FLAG_LISTENING",Dee_uint16_t,DEE_SOCKET_FLAG_LISTENING),
 DEE_MEMBERDEF_CONST_v100("FLAG_SHUTDOWN",Dee_uint16_t,DEE_SOCKET_FLAG_SHUTDOWN),
 DEE_MEMBERDEF_CONST_v100("FLAG_SHUTTINGDOWN",Dee_uint16_t,DEE_SOCKET_FLAG_SHUTTINGDOWN),
 DEE_MEMBERDEF_CONST_v100("FLAG_CLOSED",Dee_uint16_t,DEE_SOCKET_FLAG_CLOSED),

 // Socket send/recv/sendto/recvfrom flags
#ifdef MSG_CONFIRM
 DEE_MEMBERDEF_CONST_v100("MSG_CONFIRM",int,MSG_CONFIRM),
#endif
#ifdef MSG_DONTROUTE
 DEE_MEMBERDEF_CONST_v100("MSG_DONTROUTE",int,MSG_DONTROUTE),
#endif
#ifdef MSG_DONTWAIT
 DEE_MEMBERDEF_CONST_v100("MSG_DONTWAIT",int,MSG_DONTWAIT),
#endif
#ifdef MSG_EOR
 DEE_MEMBERDEF_CONST_v100("MSG_EOR",int,MSG_EOR),
#endif
#ifdef MSG_MORE
 DEE_MEMBERDEF_CONST_v100("MSG_MORE",int,MSG_MORE),
#endif
#ifdef MSG_NOSIGNAL
 DEE_MEMBERDEF_CONST_v100("MSG_NOSIGNAL",int,MSG_NOSIGNAL),
#endif
#ifdef MSG_OOB
 DEE_MEMBERDEF_CONST_v100("MSG_OOB",int,MSG_OOB),
#endif
#ifdef MSG_WAITALL
 DEE_MEMBERDEF_CONST_v100("MSG_WAITALL",int,MSG_WAITALL),
#endif
#ifdef MSG_PARTIAL
 DEE_MEMBERDEF_CONST_v100("MSG_PARTIAL",int,MSG_PARTIAL),
#endif
#ifdef MSG_INTERRUPT
 DEE_MEMBERDEF_CONST_v100("MSG_INTERRUPT",int,MSG_INTERRUPT),
#endif
#ifdef MSG_PUSH_IMMEDIATE
 DEE_MEMBERDEF_CONST_v100("MSG_PUSH_IMMEDIATE",int,MSG_PUSH_IMMEDIATE),
#endif

 DEE_MEMBERDEF_CONST_v100("IPPORT_RESERVED",Dee_uint16_t,IPPORT_RESERVED),
 DEE_MEMBERDEF_CONST_v100("IPPORT_USERRESERVED",Dee_uint16_t,IPPORT_USERRESERVED),
 DEE_MEMBERDEF_CONST_v100("INADDR_ANY",Dee_uint32_t,INADDR_ANY),
 DEE_MEMBERDEF_CONST_v100("INADDR_LOOPBACK",Dee_uint32_t,INADDR_LOOPBACK),
 DEE_MEMBERDEF_CONST_v100("INADDR_BROADCAST",Dee_uint32_t,INADDR_BROADCAST),
 DEE_MEMBERDEF_CONST_v100("INADDR_NONE",Dee_uint32_t,INADDR_NONE),
 DEE_MEMBERDEF_CONST_v100("INADDR_UNSPEC_GROUP",Dee_uint32_t,INADDR_UNSPEC_GROUP),
 DEE_MEMBERDEF_CONST_v100("INADDR_ALLHOSTS_GROUP",Dee_uint32_t,INADDR_ALLHOSTS_GROUP),
 DEE_MEMBERDEF_CONST_v100("INADDR_MAX_LOCAL_GROUP",Dee_uint32_t,INADDR_MAX_LOCAL_GROUP),
#if DEE_HAVE_AF_INET6
 DEE_MEMBERDEF_CONST_v100("IN6ADDR_ANY",object,&_deestr_IN6ADDR_ANY),
 DEE_MEMBERDEF_CONST_v100("IN6ADDR_LOOPBACK",object,&_deestr_IN6ADDR_LOOPBACK),
#endif

 // Intrinsically supported protocols
 DEE_MEMBERDEF_CONST_v100("AF_AUTO",int,DEE_AF_AUTO),
#if DEE_HAVE_AF_UNIX
 DEE_MEMBERDEF_CONST_v100("AF_UNIX",int,AF_UNIX),
#endif
#if DEE_HAVE_AF_INET
 DEE_MEMBERDEF_CONST_v100("AF_INET",int,AF_INET),
#endif
#ifdef AF_RDS
 DEE_MEMBERDEF_CONST_v100("AF_RDS",int,AF_RDS),
#endif
#if DEE_HAVE_AF_PACKET
 DEE_MEMBERDEF_CONST_v100("AF_PACKET",int,AF_PACKET),
#endif
#if DEE_HAVE_AF_TIPC
 DEE_MEMBERDEF_CONST_v100("AF_TIPC",int,AF_TIPC),
#endif
#if DEE_HAVE_AF_CAN
 DEE_MEMBERDEF_CONST_v100("AF_CAN",int,AF_CAN),
#endif
#if DEE_HAVE_PF_SYSTEM
 DEE_MEMBERDEF_CONST_v100("PF_SYSTEM",int,PF_SYSTEM),
#endif
#if DEE_HAVE_AF_IMPLINK
 DEE_MEMBERDEF_CONST_v100("AF_IMPLINK",int,AF_IMPLINK),
#endif
#if DEE_HAVE_AF_PUP
 DEE_MEMBERDEF_CONST_v100("AF_PUP",int,AF_PUP),
#endif
#if DEE_HAVE_AF_CHAOS
 DEE_MEMBERDEF_CONST_v100("AF_CHAOS",int,AF_CHAOS),
#endif
#if DEE_HAVE_AF_NS
 DEE_MEMBERDEF_CONST_v100("AF_NS",int,AF_NS),
#endif
#if DEE_HAVE_AF_IPX
 DEE_MEMBERDEF_CONST_v100("AF_IPX",int,AF_IPX),
#endif
#if DEE_HAVE_AF_ISO
 DEE_MEMBERDEF_CONST_v100("AF_ISO",int,AF_ISO),
#endif
#if DEE_HAVE_AF_OSI
 DEE_MEMBERDEF_CONST_v100("AF_OSI",int,AF_OSI),
#endif
#if DEE_HAVE_AF_ECMA
 DEE_MEMBERDEF_CONST_v100("AF_ECMA",int,AF_ECMA),
#endif
#if DEE_HAVE_AF_DATAKIT
 DEE_MEMBERDEF_CONST_v100("AF_DATAKIT",int,AF_DATAKIT),
#endif
#if DEE_HAVE_AF_CCITT
 DEE_MEMBERDEF_CONST_v100("AF_CCITT",int,AF_CCITT),
#endif
#if DEE_HAVE_AF_SNA
 DEE_MEMBERDEF_CONST_v100("AF_SNA",int,AF_SNA),
#endif
#if DEE_HAVE_AF_DECnet
 DEE_MEMBERDEF_CONST_v100("AF_DECnet",int,AF_DECnet),
#endif
#if DEE_HAVE_AF_DLI
 DEE_MEMBERDEF_CONST_v100("AF_DLI",int,AF_DLI),
#endif
#if DEE_HAVE_AF_LAT
 DEE_MEMBERDEF_CONST_v100("AF_LAT",int,AF_LAT),
#endif
#if DEE_HAVE_AF_HYLINK
 DEE_MEMBERDEF_CONST_v100("AF_HYLINK",int,AF_HYLINK),
#endif
#if DEE_HAVE_AF_APPLETALK
 DEE_MEMBERDEF_CONST_v100("AF_APPLETALK",int,AF_APPLETALK),
#endif
#if DEE_HAVE_AF_NETBIOS
 DEE_MEMBERDEF_CONST_v100("AF_NETBIOS",int,AF_NETBIOS),
#endif
#if DEE_HAVE_AF_VOICEVIEW
 DEE_MEMBERDEF_CONST_v100("AF_VOICEVIEW",int,AF_VOICEVIEW),
#endif
#if DEE_HAVE_AF_FIREFOX
 DEE_MEMBERDEF_CONST_v100("AF_FIREFOX",int,AF_FIREFOX),
#endif
#if DEE_HAVE_AF_BAN
 DEE_MEMBERDEF_CONST_v100("AF_BAN",int,AF_BAN),
#endif
#if DEE_HAVE_AF_ATM
 DEE_MEMBERDEF_CONST_v100("AF_ATM",int,AF_ATM),
#endif
#if DEE_HAVE_AF_INET6
 DEE_MEMBERDEF_CONST_v100("AF_INET6",int,AF_INET6),
#endif
#if DEE_HAVE_AF_CLUSTER
 DEE_MEMBERDEF_CONST_v100("AF_CLUSTER",int,AF_CLUSTER),
#endif
#if DEE_HAVE_AF_12844
 DEE_MEMBERDEF_CONST_v100("AF_12844",int,AF_12844),
#endif
#if DEE_HAVE_AF_IRDA
 DEE_MEMBERDEF_CONST_v100("AF_IRDA",int,AF_IRDA),
#endif
#if DEE_HAVE_AF_NETDES
 DEE_MEMBERDEF_CONST_v100("AF_NETDES",int,AF_NETDES),
#endif
#if DEE_HAVE_AF_MAX
 DEE_MEMBERDEF_CONST_v100("AF_MAX",int,AF_MAX),
#endif
#if DEE_HAVE_AF_TCNPROCESS
 DEE_MEMBERDEF_CONST_v100("AF_TCNPROCESS",int,AF_TCNPROCESS),
#endif
#if DEE_HAVE_AF_TCNMESSAGE
 DEE_MEMBERDEF_CONST_v100("AF_TCNMESSAGE",int,AF_TCNMESSAGE),
#endif
#if DEE_HAVE_AF_ICLFXBM
 DEE_MEMBERDEF_CONST_v100("AF_ICLFXBM",int,AF_ICLFXBM),
#endif
#if DEE_HAVE_AF_BTH
 DEE_MEMBERDEF_CONST_v100("AF_BTH",int,AF_BTH),
#endif
#if DEE_HAVE_AF_LINK
 DEE_MEMBERDEF_CONST_v100("AF_LINK",int,AF_LINK),
#endif
#ifdef SOCK_STREAM
 DEE_MEMBERDEF_CONST_v100("SOCK_STREAM",int,SOCK_STREAM),
#endif
#ifdef SOCK_DGRAM
 DEE_MEMBERDEF_CONST_v100("SOCK_DGRAM",int,SOCK_DGRAM),
#endif
#ifdef SOCK_RAW
 DEE_MEMBERDEF_CONST_v100("SOCK_RAW",int,SOCK_RAW),
#endif
#ifdef SOCK_RDM
 DEE_MEMBERDEF_CONST_v100("SOCK_RDM",int,SOCK_RDM),
#endif
#ifdef SOCK_SEQPACKET
 DEE_MEMBERDEF_CONST_v100("SOCK_SEQPACKET",int,SOCK_SEQPACKET),
#endif

 // Bluetooth support
#if DEE_HAVE_AF_BLUETOOTH
 DEE_MEMBERDEF_CONST_v100("AF_BLUETOOTH",int,AF_BLUETOOTH),
 DEE_MEMBERDEF_CONST_v100("BTPROTO_L2CAP",int,BTPROTO_L2CAP),
 DEE_MEMBERDEF_CONST_v100("BTPROTO_HCI",int,BTPROTO_HCI),
#ifdef SOL_HCI
 DEE_MEMBERDEF_CONST_v100("SOL_HCI",int,SOL_HCI),
#endif
#ifdef SOL_L2CAP
 DEE_MEMBERDEF_CONST_v100("SOL_L2CAP",int,SOL_L2CAP),
#endif
#ifdef SOL_SCO
 DEE_MEMBERDEF_CONST_v100("SOL_SCO",int,SOL_SCO),
#endif
#ifdef SOL_RFCOMM
 DEE_MEMBERDEF_CONST_v100("SOL_RFCOMM",int,SOL_RFCOMM),
#endif
#if !defined(__NetBSD__) && !defined(__DragonFly__)
 DEE_MEMBERDEF_CONST_v100("HCI_FILTER",int,HCI_FILTER),
#endif
#if !defined(__FreeBSD__)
#if !defined(__NetBSD__) && !defined(__DragonFly__)
 DEE_MEMBERDEF_CONST_v100("HCI_TIME_STAMP",int,HCI_TIME_STAMP),
#endif
 DEE_MEMBERDEF_CONST_v100("HCI_DATA_DIR",int,HCI_DATA_DIR),
 DEE_MEMBERDEF_CONST_v100("BTPROTO_SCO",int,BTPROTO_SCO),
#endif
 DEE_MEMBERDEF_CONST_v100("BTPROTO_RFCOMM",int,BTPROTO_RFCOMM),
 DEE_MEMBERDEF_CONST_v100("BDADDR_ANY",object,&_deestr_BDADDR_ANY),
 DEE_MEMBERDEF_CONST_v100("BDADDR_ALL",object,&_deestr_BDADDR_ALL),
 DEE_MEMBERDEF_CONST_v100("BDADDR_LOCAL",object,&_deestr_BDADDR_LOCAL),
#endif

 // IP Protocols
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IP",int,IPPROTO_IP),
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ICMP",int,IPPROTO_ICMP),
 DEE_MEMBERDEF_CONST_v100("IPPROTO_TCP",int,IPPROTO_TCP),
 DEE_MEMBERDEF_CONST_v100("IPPROTO_UDP",int,IPPROTO_UDP),
 DEE_MEMBERDEF_CONST_v100("IPPROTO_RAW",int,IPPROTO_RAW),
#ifdef IPPROTO_HOPOPTS
 DEE_MEMBERDEF_CONST_v100("IPPROTO_HOPOPTS",int,IPPROTO_HOPOPTS),
#endif
#ifdef IPPROTO_IGMP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IGMP",int,IPPROTO_IGMP),
#endif
#ifdef IPPROTO_GGP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_GGP",int,IPPROTO_GGP),
#endif
#ifdef IPPROTO_IPV4
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IPV4",int,IPPROTO_IPV4),
#endif
#ifdef IPPROTO_ST
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ST",int,IPPROTO_ST),
#endif
#ifdef IPPROTO_CBT
 DEE_MEMBERDEF_CONST_v100("IPPROTO_CBT",int,IPPROTO_CBT),
#endif
#ifdef IPPROTO_EGP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_EGP",int,IPPROTO_EGP),
#endif
#ifdef IPPROTO_IGP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IGP",int,IPPROTO_IGP),
#endif
#ifdef IPPROTO_PUP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_PUP",int,IPPROTO_PUP),
#endif
#ifdef IPPROTO_IDP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IDP",int,IPPROTO_IDP),
#endif
#ifdef IPPROTO_RDP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_RDP",int,IPPROTO_RDP),
#endif
#ifdef IPPROTO_IPV6
 DEE_MEMBERDEF_CONST_v100("IPPROTO_IPV6",int,IPPROTO_IPV6),
#endif
#ifdef IPPROTO_ROUTING
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ROUTING",int,IPPROTO_ROUTING),
#endif
#ifdef IPPROTO_FRAGMENT
 DEE_MEMBERDEF_CONST_v100("IPPROTO_FRAGMENT",int,IPPROTO_FRAGMENT),
#endif
#ifdef IPPROTO_ESP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ESP",int,IPPROTO_ESP),
#endif
#ifdef IPPROTO_AH
 DEE_MEMBERDEF_CONST_v100("IPPROTO_AH",int,IPPROTO_AH),
#endif
#ifdef IPPROTO_ICMPV6
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ICMPV6",int,IPPROTO_ICMPV6),
#endif
#ifdef IPPROTO_NONE
 DEE_MEMBERDEF_CONST_v100("IPPROTO_NONE",int,IPPROTO_NONE),
#endif
#ifdef IPPROTO_DSTOPTS
 DEE_MEMBERDEF_CONST_v100("IPPROTO_DSTOPTS",int,IPPROTO_DSTOPTS),
#endif
#ifdef IPPROTO_ND
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ND",int,IPPROTO_ND),
#endif
#ifdef IPPROTO_ICLFXBM
 DEE_MEMBERDEF_CONST_v100("IPPROTO_ICLFXBM",int,IPPROTO_ICLFXBM),
#endif
#ifdef IPPROTO_PIM
 DEE_MEMBERDEF_CONST_v100("IPPROTO_PIM",int,IPPROTO_PIM),
#endif
#ifdef IPPROTO_PGM
 DEE_MEMBERDEF_CONST_v100("IPPROTO_PGM",int,IPPROTO_PGM),
#endif
#ifdef IPPROTO_L2TP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_L2TP",int,IPPROTO_L2TP),
#endif
#ifdef IPPROTO_SCTP
 DEE_MEMBERDEF_CONST_v100("IPPROTO_SCTP",int,IPPROTO_SCTP),
#endif

 // Socket Option levels
#ifdef SOL_SOCKET
 DEE_MEMBERDEF_CONST_v100("SOL_SOCKET",int,SOL_SOCKET),
#else
 DEE_MEMBERDEF_CONST_v100("SOL_SOCKET",int,DEE_INTERNAL_SOL_SOCKET),
#endif

 // Socket Option names
#ifdef SO_DEBUG
 DEE_MEMBERDEF_CONST_v100("SO_DEBUG",int,SO_DEBUG),
#endif
#ifdef SO_ACCEPTCONN
 DEE_MEMBERDEF_CONST_v100("SO_ACCEPTCONN",int,SO_ACCEPTCONN),
#else
 DEE_MEMBERDEF_CONST_v100("SO_ACCEPTCONN",int,DEE_INTERNAL_SO_ACCEPTCONN),
#endif
#ifdef SO_REUSEADDR
 DEE_MEMBERDEF_CONST_v100("SO_REUSEADDR",int,SO_REUSEADDR),
#endif
#ifdef SO_KEEPALIVE
 DEE_MEMBERDEF_CONST_v100("SO_KEEPALIVE",int,SO_KEEPALIVE),
#endif
#ifdef SO_DONTROUTE
 DEE_MEMBERDEF_CONST_v100("SO_DONTROUTE",int,SO_DONTROUTE),
#endif
#ifdef SO_BROADCAST
 DEE_MEMBERDEF_CONST_v100("SO_BROADCAST",int,SO_BROADCAST),
#endif
#ifdef SO_USELOOPBACK
 DEE_MEMBERDEF_CONST_v100("SO_USELOOPBACK",int,SO_USELOOPBACK),
#endif
#ifdef SO_LINGER
 DEE_MEMBERDEF_CONST_v100("SO_LINGER",int,SO_LINGER),
#endif
#ifdef SO_OOBINLINE
 DEE_MEMBERDEF_CONST_v100("SO_OOBINLINE",int,SO_OOBINLINE),
#endif
#ifdef SO_DONTLINGER
 DEE_MEMBERDEF_CONST_v100("SO_DONTLINGER",int,SO_DONTLINGER),
#endif
#ifdef SO_EXCLUSIVEADDRUSE
 DEE_MEMBERDEF_CONST_v100("SO_EXCLUSIVEADDRUSE",int,SO_EXCLUSIVEADDRUSE),
#endif
#ifdef SO_SNDBUF
 DEE_MEMBERDEF_CONST_v100("SO_SNDBUF",int,SO_SNDBUF),
#endif
#ifdef SO_RCVBUF
 DEE_MEMBERDEF_CONST_v100("SO_RCVBUF",int,SO_RCVBUF),
#endif
#ifdef SO_SNDLOWAT
 DEE_MEMBERDEF_CONST_v100("SO_SNDLOWAT",int,SO_SNDLOWAT),
#endif
#ifdef SO_RCVLOWAT
 DEE_MEMBERDEF_CONST_v100("SO_RCVLOWAT",int,SO_RCVLOWAT),
#endif
#ifdef SO_ERROR
 DEE_MEMBERDEF_CONST_v100("SO_ERROR",int,SO_ERROR),
#endif
#ifdef SO_TYPE
 DEE_MEMBERDEF_CONST_v100("SO_TYPE",int,SO_TYPE),
#elif defined(SO_STYLE)
 DEE_MEMBERDEF_CONST_v100("SO_TYPE",int,SO_STYLE),
#else
 DEE_MEMBERDEF_CONST_v100("SO_TYPE",int,DEE_INTERNAL_SO_TYPE),
#endif
#ifdef SO_DOMAIN
 DEE_MEMBERDEF_CONST_v100("SO_DOMAIN",int,SO_DOMAIN),
#else
 DEE_MEMBERDEF_CONST_v100("SO_DOMAIN",int,DEE_INTERNAL_SO_DOMAIN),
#endif
#ifdef SO_PROTOCOL
 DEE_MEMBERDEF_CONST_v100("SO_PROTOCOL",int,SO_PROTOCOL),
#else
 DEE_MEMBERDEF_CONST_v100("SO_PROTOCOL",int,DEE_INTERNAL_SO_PROTOCOL),
#endif
#ifdef SO_GROUP_ID
 DEE_MEMBERDEF_CONST_v100("SO_GROUP_ID",int,SO_GROUP_ID),
#endif
#ifdef SO_GROUP_PRIORITY
 DEE_MEMBERDEF_CONST_v100("SO_GROUP_PRIORITY",int,SO_GROUP_PRIORITY),
#endif
#ifdef SO_MAX_MSG_SIZE
 DEE_MEMBERDEF_CONST_v100("SO_MAX_MSG_SIZE",int,SO_MAX_MSG_SIZE),
#endif
#ifdef SO_CONDITIONAL_ACCEPT
 DEE_MEMBERDEF_CONST_v100("SO_CONDITIONAL_ACCEPT",int,SO_CONDITIONAL_ACCEPT),
#endif
#ifdef SO_RCVTIMEO
 DEE_MEMBERDEF_CONST_v100("SO_RCVTIMEO",int,SO_RCVTIMEO),
#endif
#ifdef SO_SNDTIMEO
 DEE_MEMBERDEF_CONST_v100("SO_SNDTIMEO",int,SO_SNDTIMEO),
#endif
#ifdef SO_BINDTODEVICE
 DEE_MEMBERDEF_CONST_v100("SO_BINDTODEVICE",int,SO_BINDTODEVICE),
#endif
#ifdef TCP_NODELAY
 DEE_MEMBERDEF_CONST_v100("TCP_NODELAY",int,TCP_NODELAY),
#endif
#ifdef TCP_EXPEDITED_1122
 DEE_MEMBERDEF_CONST_v100("TCP_EXPEDITED_1122",int,TCP_EXPEDITED_1122),
#endif
#ifdef TCP_KEEPALIVE
 DEE_MEMBERDEF_CONST_v100("TCP_KEEPALIVE",int,TCP_KEEPALIVE),
#endif
#ifdef TCP_MAXSEG
 DEE_MEMBERDEF_CONST_v100("TCP_MAXSEG",int,TCP_MAXSEG),
#endif
#ifdef TCP_MAXRT
 DEE_MEMBERDEF_CONST_v100("TCP_MAXRT",int,TCP_MAXRT),
#endif
#ifdef TCP_STDURG
 DEE_MEMBERDEF_CONST_v100("TCP_STDURG",int,TCP_STDURG),
#endif
#ifdef TCP_NOURG
 DEE_MEMBERDEF_CONST_v100("TCP_NOURG",int,TCP_NOURG),
#endif
#ifdef TCP_ATMARK
 DEE_MEMBERDEF_CONST_v100("TCP_ATMARK",int,TCP_ATMARK),
#endif
#ifdef TCP_NOSYNRETRIES
 DEE_MEMBERDEF_CONST_v100("TCP_NOSYNRETRIES",int,TCP_NOSYNRETRIES),
#endif
#ifdef TCP_TIMESTAMPS
 DEE_MEMBERDEF_CONST_v100("TCP_TIMESTAMPS",int,TCP_TIMESTAMPS),
#endif
#ifdef TCP_OFFLOAD_PREFERENCE
 DEE_MEMBERDEF_CONST_v100("TCP_OFFLOAD_PREFERENCE",int,TCP_OFFLOAD_PREFERENCE),
#endif
#ifdef TCP_OFFLOAD_NO_PREFERENCE
 DEE_MEMBERDEF_CONST_v100("TCP_OFFLOAD_NO_PREFERENCE",int,TCP_OFFLOAD_NO_PREFERENCE),
#endif
#ifdef TCP_OFFLOAD_NOT_PREFERRED
 DEE_MEMBERDEF_CONST_v100("TCP_OFFLOAD_NOT_PREFERRED",int,TCP_OFFLOAD_NOT_PREFERRED),
#endif
#ifdef TCP_OFFLOAD_PREFERRED
 DEE_MEMBERDEF_CONST_v100("TCP_OFFLOAD_PREFERRED",int,TCP_OFFLOAD_PREFERRED),
#endif
#ifdef TCP_CONGESTION_ALGORITHM
 DEE_MEMBERDEF_CONST_v100("TCP_CONGESTION_ALGORITHM",int,TCP_CONGESTION_ALGORITHM),
#endif
#ifdef TCP_DELAY_FIN_ACK
 DEE_MEMBERDEF_CONST_v100("TCP_DELAY_FIN_ACK",int,TCP_DELAY_FIN_ACK),
#endif
 DEE_MEMBERDEF_END_v100
};
DEE_COMPILER_MSVC_WARNING_POP


DeeTypeObject DeeSocket_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("socket"),member(
  "(int af, int type = socket.SOCK_STREAM, int protocol = 0) -> socket\n"
  "(int af, int type = socket.SOCK_STREAM, string protocol) -> socket\n"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSocketObject),null,null,null,
  member(&_deesocket_tp_move_ctor),
  member(&_deesocket_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deesocket_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesocket_tp_str),
  member(&_deesocket_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deesocket_tp_not),
  member(&_deesocket_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deesocket_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesocket_tp_cmp_lo),
  member(&_deesocket_tp_cmp_le),
  member(&_deesocket_tp_cmp_eq),
  member(&_deesocket_tp_cmp_ne),
  member(&_deesocket_tp_cmp_gr),
  member(&_deesocket_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesocket_tp_members),member(_deesocket_tp_getsets),member(_deesocket_tp_methods),
  member(_deesocket_tp_class_members),null,member(_deesocket_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeFileTypeObject DeeSocketFile_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(member(&DeeFileType_Type),
   member("socket.file"),null,null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSocketFileObject),null,null,null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deesocketfile_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deesocketfile_tp_members),null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deesocketfile_tp_io_read),
  member(&_deesocketfile_tp_io_write),
  member(&_deesocketfile_tp_io_seek),
  member(&_deesocketfile_tp_io_flush),
  member(&_deesocketfile_tp_io_trunc),
  member(&_deesocketfile_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeeSocketFileTo_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(member(&DeeFileType_Type),
   member("socket.fileto"),null,null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSocketFileObject),null,null,null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deesocketfileto_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deesocketfileto_tp_members),
   member(_deesocketfileto_tp_getsets),null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deesocketfileto_tp_io_read),
  member(&_deesocketfileto_tp_io_write),
  member(&_deesocketfileto_tp_io_seek),
  member(&_deesocketfileto_tp_io_flush),
  member(&_deesocketfileto_tp_io_trunc),
  member(&_deesocketfileto_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};






//////////////////////////////////////////////////////////////////////////
// SockAddr VTable
static int DEE_CALL _deesockaddr_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSockAddrObject *self, DeeObject *args) {
 int af; Dee_size_t argc;
 if DEE_UNLIKELY((argc = DeeTuple_SIZE(args)) == 0) {
  DeeError_TypeError_ArgCountExpectedNamedMin("sockaddr",1,argc);
  return -1;
 }
 if DEE_UNLIKELY(DeeObject_Cast(int,DeeTuple_GET(args,0),&af) != 0) return -1;
 return DeeSockAddr_InitFromArgv(&self->sa_addr,af,0,argc-1,DeeTuple_ELEM(args)+1);
}
static int DEE_CALL _deesockaddr_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSockAddrObject *self, DeeSockAddrObject *right) {
 self->sa_addr = right->sa_addr;
 return 0;
}
static DeeObject *DEE_CALL _deesockaddr_tp_str(DeeSockAddrObject *self) {
 return DeeSockAddr_Str(&self->sa_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL);
}
static DeeObject *DEE_CALL _deesockaddr_tp_repr(DeeSockAddrObject *self) {
 return DeeString_Newf("<sockaddr(%R)>",DeeSockAddr_Str(
  &self->sa_addr,DEE_SOCKADDR_STR_FLAG_NOFAIL));
}
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_lo(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) < 0); }
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_le(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) <= 0); }
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_eq(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) == 0); }
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_ne(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) != 0); }
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_gr(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) > 0); }
static DeeObject *DEE_CALL _deesockaddr_tp_cmd_ge(DeeSockAddrObject *self, DeeSockAddrObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeSockAddr_Type) != 0) return NULL; DeeReturn_Bool(DeeSockAddr_Compare(&self->sa_addr,&right->sa_addr) >= 0); }

static DeeObject *DEE_CALL _deesockaddrclass_resolve_ex(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *result; int af,protocol; Dee_size_t argc;
 if DEE_UNLIKELY((argc = DeeTuple_SIZE(args)) < 2) {
  DeeError_TypeError_ArgCountExpectedNamedMin("resolve_ex",2,argc);
  return NULL;
 }
 if DEE_UNLIKELY(DeeObject_Cast(int,DeeTuple_GET(args,0),&af) != 0) return NULL;
 if (DeeString_Check(DeeTuple_GET(args,1))) {
  if DEE_UNLIKELY(DeeSocket_StringToProtocol(DeeString_STR(DeeTuple_GET(args,1)),&protocol) != 0) return NULL;
 } else if DEE_UNLIKELY(DeeObject_Cast(int,DeeTuple_GET(args,1),&protocol) != 0) return NULL;
 if DEE_UNLIKELY((result = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&result->sa_addr,af,protocol,
  argc-2,DeeTuple_ELEM(args)+2) != 0) Dee_CLEAR(result);
 return (DeeObject *)result;
}
static DeeObject *DEE_CALL _deesockaddrclass_resolve(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeSockAddrObject *result;
 if DEE_UNLIKELY((result = (DeeSockAddrObject *)_DeeSockAddrObject_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeSockAddr_InitFromArgv(&result->sa_addr,DEE_AF_AUTO,0,
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) Dee_CLEAR(result);
 return (DeeObject *)result;
}
static DeeObject *DEE_CALL _deesockaddrclass_inet_hostname(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_HAVE_AF_INET
 struct DeeSockAddr addr;
#endif
 Dee_uint8_t a,b,c,d; Dee_uint32_t host;
 switch (DeeTuple_SIZE(args)) {
  case 1:
#if DEE_HAVE_AF_INET
   if (DeeString_Check(DeeTuple_GET(args,0))) {
    if DEE_UNLIKELY(DeeSockAddr_InitFromStringAndPort(
     &addr,AF_INET,DeeString_STR(DeeTuple_GET(args,0)),0) != 0) return NULL;
    host = DeeSockAddr_INET_HOST(&addr);
   } else
#endif
   {
    if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,DeeTuple_GET(args,0),&host) != 0) return NULL;
    host = DEE_BUILTIN_HTON32(host);
   }
   break;
  case 4:
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,0),&a) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,1),&b) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,2),&c) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,3),&d) != 0
                   ) return NULL;
   host = DEE_SOCKADDRINET_HOST(a,b,c,d);
   break;
  default:
   DeeError_TypeError_ArgCountExpectedNamedMinMax("inet_hostname",1,4,DeeTuple_SIZE(args));
   return NULL;
 }
 return Dee_BuildValue("(I8uI8uI8uI8u)",
  DEE_SOCKET_INET_HOSTNAME_A(host),DEE_SOCKET_INET_HOSTNAME_B(host),
  DEE_SOCKET_INET_HOSTNAME_C(host),DEE_SOCKET_INET_HOSTNAME_D(host));
}
static DeeObject *DEE_CALL _deesockaddrclass_inet_aton(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_HAVE_AF_INET
 struct DeeSockAddr addr;
#endif
 Dee_uint32_t host; Dee_uint8_t a,b,c,d;
 switch (DeeTuple_SIZE(args)) {
  case 1:
#if DEE_HAVE_AF_INET
   if (DeeString_Check(DeeTuple_GET(args,0))) {
    if DEE_UNLIKELY(DeeSockAddr_InitFromStringAndPort(&addr,AF_INET,
     DeeString_STR(DeeTuple_GET(args,0)),0) != 0) return NULL;
    return DeeObject_New(Dee_uint32_t,DEE_BUILTIN_NTOH32(DeeSockAddr_INET_HOST(&addr)));
   }
#endif
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,DeeTuple_GET(args,0),&host) != 0) return NULL;
   return DeeObject_New(Dee_uint32_t,host);
  case 4:
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,0),&a) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,1),&b) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,2),&c) != 0 ||
                   DeeObject_Cast(Dee_uint8_t,DeeTuple_GET(args,3),&d) != 0
                   ) return NULL;
   return DeeObject_New(Dee_uint32_t,DEE_BUILTIN_NTOH32(DEE_SOCKADDRINET_HOST(a,b,c,d)));
  default:
   DeeError_TypeError_ArgCountExpectedNamedMinMax("inet_aton",1,4,DeeTuple_SIZE(args));
   return NULL;
 }
}
static DeeObject *DEE_CALL _deesockaddrclass_inet_ntoa(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *arg0,*arg1 = NULL,*arg2 = NULL,*arg3 = NULL,*arg4 = NULL;
 struct DeeSockAddr addr; Dee_uint32_t host; Dee_uint16_t port;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oooo:inet_ntoa",&arg0,&arg1,&arg2,&arg3,&arg4) != 0) return NULL;
 if (!arg1) {
  if (DeeString_Check(arg0)) {
   char const *iter,*end;
   end = (iter = DeeString_STR(arg0))+DeeString_SIZE(arg0);
   while (iter != end) if (*iter++ == ':') {
    if DEE_UNLIKELY(DeeSockAddr_InitFromString(&addr,DEE_AF_AUTO,DeeString_STR(arg0)) != 0) return NULL;
    goto ipstr;
   }
   if DEE_UNLIKELY(DeeSockAddr_InitFromStringAndPort(
    &addr,DEE_AF_AUTO,DeeString_STR(arg0),0) != 0) return NULL;
   return DeeSockAddr_Str(&addr,DEE_SOCKADDR_STR_FLAG_NOPORT|DEE_SOCKADDR_STR_FLAG_NOFAIL);
  } else {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,arg0,&host) != 0) return NULL;
   host = DEE_BUILTIN_HTON32(host);
  }
#if DEE_HAVE_AF_INET
hostname:
#endif
  return DeeSocket_INETHostname(host,DEE_SOCKADDR_STR_FLAG_NOFAIL);
 } else if (!arg2) {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_uint16_t,arg1,&port) != 0) return NULL;
#if DEE_HAVE_AF_INET
  if (!DeeString_Check(arg0)) {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,arg0,&host) != 0) return NULL;
   _DeeSockAddr_InitINET(&addr,DEE_BUILTIN_HTON32(host),DEE_BUILTIN_HTON16(port));
  } else
#else
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(arg0,&DeeString_Type) != 0) return NULL;
#endif
  {
   if DEE_UNLIKELY(DeeSockAddr_InitFromStringAndPort(
    &addr,DEE_AF_AUTO,DeeString_STR(arg0),port) != 0) return NULL;
  }
 } else {
#if DEE_HAVE_AF_INET
  Dee_uint8_t a,b,c,d;
  if DEE_UNLIKELY(!arg3) {
   DeeError_SET_STRING(&DeeErrorType_TypeError,
                       "inet_ntoa() takes 1,2,4 or 5 arguments (not 3)");
   return NULL;
  }
  if DEE_UNLIKELY(DeeObject_Cast(Dee_uint8_t,arg0,&a) != 0
               || DeeObject_Cast(Dee_uint8_t,arg1,&b) != 0
               || DeeObject_Cast(Dee_uint8_t,arg2,&c) != 0
               || DeeObject_Cast(Dee_uint8_t,arg3,&d) != 0) return NULL;
  host = DEE_SOCKADDRINET_HOST(a,b,c,d);
  if (!arg4) goto hostname;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_uint16_t,arg4,&port) != 0) return NULL;
  _DeeSockAddr_InitINET(&addr,host,DEE_BUILTIN_HTON16(port));
#else
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "inet_ntoa() takes 1,2,4 arguments (not %Iu)",
                      DeeTuple_SIZE(args));
  return NULL;
#endif
 }
ipstr:
 return DeeSockAddr_Str(&addr,DEE_SOCKADDR_STR_FLAG_NOFAIL);
}

static DeeObject *DEE_CALL _deesockaddr_inet_host_get(
 DeeSockAddrObject *self, void *DEE_UNUSED(closure)) {
#ifdef AF_INET
 if DEE_UNLIKELY(DeeSockAddr_FAMILY(&self->sa_addr) != AF_INET) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Expected AF_INET address_family (got: %K)",
                      DeeSocket_AddressFamilyToString(DeeSockAddr_FAMILY(&self->sa_addr)));
  return NULL;
 }
 return DeeObject_New(Dee_uint32_t,DEE_BUILTIN_NTOH32(DeeSockAddr_INET_HOST(&self->sa_addr)));
#else
 DeeError_NotImplemented_str("AF_INET");
 return NULL;
#endif
}
static DeeObject *DEE_CALL _deesockaddr_inet_port_get(
 DeeSockAddrObject *self, void *DEE_UNUSED(closure)) {
#if defined(AF_INET) || defined(AF_INET6)
 switch (DeeSockAddr_FAMILY(&self->sa_addr)) {
#ifdef AF_INET
  case AF_INET:
   return DeeObject_New(Dee_uint16_t,DEE_BUILTIN_NTOH16(DeeSockAddr_INET_PORT(&self->sa_addr)));
#endif
#ifdef AF_INET6
  case AF_INET6:
   return DeeObject_New(Dee_uint16_t,DEE_BUILTIN_NTOH16(DeeSockAddr_INET6_PORT(&self->sa_addr)));
#endif
  default:
   DeeError_SetStringf(&DeeErrorType_ValueError,
#if defined(AF_INET) && defined(AF_INET6)
                       "Expected AF_INET|AF_INET6 address_family (got: %K)",
#elif defined(AF_INET6)
                       "Expected AF_INET6 address_family (got: %K)",
#else
                       "Expected AF_INET address_family (got: %K)",
#endif
                       DeeSocket_AddressFamilyToString(DeeSockAddr_FAMILY(&self->sa_addr)));
   return NULL;
 }
#else
 DeeError_NotImplemented_str("AF_INET|AF_INET6");
 return NULL;
#endif
}
static DeeObject *DEE_CALL _deesockaddr_inet6_host_get(
 DeeSockAddrObject *self, void *DEE_UNUSED(closure)) {
#ifdef AF_INET6
 if DEE_UNLIKELY(DeeSockAddr_FAMILY(&self->sa_addr) != AF_INET6) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Expected AF_INET6 address_family (got: %K)",
                      DeeSocket_AddressFamilyToString(DeeSockAddr_FAMILY(&self->sa_addr)));
  return NULL;
 }
 return DeeSocket_INET6Hostname(DeeSockAddr_INET6_HOST(&self->sa_addr),
                                DEE_SOCKADDR_STR_FLAG_NOFAIL|DEE_SOCKADDR_STR_FLAG_NODNS);
#else
 DeeError_NotImplemented_str("AF_INET6");
 return NULL;
#endif
}
static DeeObject *DEE_CALL _deesockaddr___size(
 DeeSockAddrObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *protocol = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o",&protocol) != 0) return NULL;
#ifdef DEE_SOCKET_SIZEOF_SOCKADDR_NEEDS_PROTOCOL
 { int sock_protocol;
   if DEE_UNLIKELY(DeeString_Check(protocol)
                   ? DeeSocket_StringToProtocol(DeeString_STR(protocol),&sock_protocol) != 0
                   : DeeObject_Cast(int,protocol,&sock_protocol) != 0) return NULL;
   return DeeObject_New(Dee_size_t,Dee_SizeofSockAddr(&self->sa_addr,sock_protocol));
 }
#else
 return DeeObject_New(Dee_size_t,_Dee_SizeofSockAddr(&self->sa_addr));
#endif
}

static struct DeeMemberDef _deesockaddr_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("address_family",DeeSockAddrObject,sa_addr.sa.sa_family,sa_family_t),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_sa_family",DeeSockAddrObject,sa_addr.sa.sa_family,sa_family_t),
#if DEE_HAVE_AF_INET
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_family",DeeSockAddrObject,sa_addr.sa_inet.sin_family,sa_family_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_port",DeeSockAddrObject,sa_addr.sa_inet.sin_port,in_port_t),
#ifdef s_addr
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_addr",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_addr,Dee_uint32_t),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_addr",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint32_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+0),
#endif
#ifdef s_host
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_host",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_host,Dee_uint8_t),
#elif DEE_PLATFORM_ENDIAN == 1234
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_host",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+1),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_host",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+2),
#endif
#ifdef s_net
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_net",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_net,Dee_uint8_t),
#elif DEE_PLATFORM_ENDIAN == 1234
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_net",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+0),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_net",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+3),
#endif
#ifdef s_imp
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_imp",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_imp,Dee_uint16_t),
#elif DEE_PLATFORM_ENDIAN == 1234
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_imp",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+2),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_imp",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+0),
#endif
#ifdef s_impno
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_impno",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_impno,Dee_uint8_t),
#elif DEE_PLATFORM_ENDIAN == 1234
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_impno",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+3),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_impno",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+0),
#endif
#ifdef s_lh
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet_sin_addr_s_lh",DeeSockAddrObject,sa_addr.sa_inet.sin_addr.s_lh,Dee_uint8_t),
#elif DEE_PLATFORM_ENDIAN == 1234
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_lh",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+2),
#else
 DEE_MEMBERDEF_FULL_v100    ("__sa_addr_sa_inet_sin_addr_s_lh",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet.sin_addr)+1),
#endif
#endif
#if DEE_HAVE_AF_INET6
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet6_sin6_family",DeeSockAddrObject,sa_addr.sa_inet6.sin6_family,sa_family_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet6_sin6_port",DeeSockAddrObject,sa_addr.sa_inet6.sin6_port,in_port_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet6_sin6_flowinfo",DeeSockAddrObject,sa_addr.sa_inet6.sin6_flowinfo,Dee_uint32_t),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b0",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+0),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b1",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+1),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b2",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+2),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b3",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+3),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b4",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+4),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b5",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+5),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b6",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+6),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b7",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+7),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b8",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+8),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b9",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+9),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b10",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+10),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b11",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+11),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b12",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+12),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b13",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+13),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b14",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+14),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_b15",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint8_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+15),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w0",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+0),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w1",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+2),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w2",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+4),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w3",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+6),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w4",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+8),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w5",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+10),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w6",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+12),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_w7",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint16_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+14),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_l0",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint32_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+0),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_l1",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint32_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+4),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_l2",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint32_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+8),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_l3",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint32_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+12),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_ll0",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint64_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+0),
 DEE_MEMBERDEF_FULL_v100("__sa_addr_sa_inet6_sin6_sin6_addr_ll1",NULL,DEE_PRIVATE_MEMBERTYPE(Dee_uint64_t),DeeMemberAccess_MAKE(1,0),Dee_OFFSETOF(DeeSockAddrObject,sa_addr.sa_inet6.sin6_addr)+8),
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_inet6_sin6_scope_id",DeeSockAddrObject,sa_addr.sa_inet6.sin6_scope_id,Dee_uint32_t),
#endif
#if DEE_HAVE_AF_UNIX
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_un_sun_family",DeeSockAddrObject,sa_addr.sa_un.sun_family,sa_family_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__sa_addr_sa_un_sun_path",DeeSockAddrObject,sa_addr.sa_un.sun_path,inplace_string),
#endif
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef _deesockaddr_tp_memthods[] = {
 DEE_METHODDEF_v100("__size",member(&_deesockaddr___size),
  "(int protocol = 0) -> size_t"
  "(string protocol) -> size_t"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef _deesockaddr_tp_getsets[] = {
 DEE_GETSETDEF_v100("inet_host",member(&_deesockaddr_inet_host_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_v100("inet_port",member(&_deesockaddr_inet_port_get),null,null,"-> uint16_t"),
 DEE_GETSETDEF_v100("inet6_host",member(&_deesockaddr_inet6_host_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("inet6_port",member(&_deesockaddr_inet_port_get),null,null,"-> uint16_t"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef _deesockaddr_tp_class_methods[] = {
 DEE_METHODDEF_v100("resolve",member(&_deesockaddrclass_resolve),
  "(...) -> sockaddr\n"
  "\tResolves an address from the given arguments.\n"
  "\tSame as calling 'sockaddr(-1,...)'\n"),
 DEE_METHODDEF_v100("resolve",member(&_deesockaddrclass_resolve_ex),
  "(int af, int protocol, ...) -> sockaddr\n"
  "(int af, string protocol, ...) -> sockaddr\n"
  "(string af, int protocol, ...) -> sockaddr\n"
  "(string af, string protocol, ...) -> sockaddr\n"),
 DEE_METHODDEF_v100("inet_hostname",member(&_deesockaddrclass_inet_hostname),
  "(string host) -> (uint8_t,uint8_t,uint8_t,uint8_t)\n"
  "(uint32_t host) -> (uint8_t,uint8_t,uint8_t,uint8_t)\n"
  "(uint8_t a, uint8_t b, uint8_t c, uint8_t d) -> (uint8_t,uint8_t,uint8_t,uint8_t)\n"
  "\tReturns 4 portions of a given AF_INET address"),
 DEE_METHODDEF_v100("inet_aton",member(&_deesockaddrclass_inet_aton),
  "(string host) -> uint32_t\n"
  "(uint32_t host) -> uint32_t\n"
  "(uint8_t a, uint8_t b, uint8_t c, uint8_t d) -> uint32_t\n"
  "\tReturns the IP of a given AF_INET address"),
 DEE_METHODDEF_v100("inet_ntoa",member(&_deesockaddrclass_inet_ntoa),
  "(string address) -> string\n"
  "(string address, uint16_t port) -> string\n"
  "(uint32_t host) -> string\n"
  "(uint32_t host, uint16_t port) -> string\n"
  "((uint32_t,uint16_t) address) -> string\n"
  "(uint8_t a, uint8_t b, uint8_t c, uint8_t d) -> string\n"
  "(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) -> string\n"
  "\tReturns the hostname (and possibly port) of a given ip or address\n"
  "\tIf the argument is a string, the call is identical to 'inet_ntoa(inet_aton(address),...)'"),
 DEE_METHODDEF_END_v100
};
DEE_COMPILER_MSVC_WARNING_PUSH(4574)

DeeTypeObject DeeSockAddr_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("sockaddr"),member(
  "(int af, ...) -> sockaddr\n"
  "(string af, ...) -> sockaddr\n"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSocketObject),null,null,
  member(&_deesockaddr_tp_copy_ctor),null,
  member(&_deesockaddr_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesockaddr_tp_str),
  member(&_deesockaddr_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericmemcmp_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesockaddr_tp_cmd_lo),
  member(&_deesockaddr_tp_cmd_le),
  member(&_deesockaddr_tp_cmd_eq),
  member(&_deesockaddr_tp_cmd_ne),
  member(&_deesockaddr_tp_cmd_gr),
  member(&_deesockaddr_tp_cmd_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesockaddr_tp_members),member(_deesockaddr_tp_getsets),
  member(_deesockaddr_tp_memthods),null,null,member(_deesockaddr_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */

#endif /* !GUARD_DEEMON_SOCKET_C */
