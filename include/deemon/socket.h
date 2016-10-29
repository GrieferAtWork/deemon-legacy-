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
#ifndef GUARD_DEEMON_SOCKET_H
#define GUARD_DEEMON_SOCKET_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_SOCKET_API

#include <deemon/__bswap_intrin.inl>

//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The socket API might be moved into a Dex at one point or another --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

struct DeeSockAddr;

DEE_OBJECT_DEF(DeeSockAddrObject);
DEE_OBJECT_DEF(DeeSocketObject);
DEE_OBJECT_DEF(DeeTCPSocketObject);
DEE_OBJECT_DEF(DeeSocketFileObject);
DEE_OBJECT_DEF(DeeSocketFileToObject);

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif


#ifndef DEE_PLATFORM_NET_ENDIAN
#define DEE_PLATFORM_NET_ENDIAN 4321 // Big endian
#endif

#if DEE_PLATFORM_ENDIAN == DEE_PLATFORM_NET_ENDIAN
#define DEE_SOCKADDRINET_HOST(a,b,c,d)\
(((Dee_uint32_t)((Dee_uint8_t)(a))<<24)|\
 ((Dee_uint32_t)((Dee_uint8_t)(b))<<16)|\
 ((Dee_uint32_t)((Dee_uint8_t)(c))<<8)|\
 ((Dee_uint32_t)((Dee_uint8_t)(d))))
#define DEE_SOCKET_INET_HOSTNAME_A(x) ((Dee_uint8_t)(((x)&DEE_UINT32_C(0xFF000000))>>24))
#define DEE_SOCKET_INET_HOSTNAME_B(x) ((Dee_uint8_t)(((x)&DEE_UINT32_C(0x00FF0000))>>16))
#define DEE_SOCKET_INET_HOSTNAME_C(x) ((Dee_uint8_t)(((x)&DEE_UINT32_C(0x0000FF00))>>8))
#define DEE_SOCKET_INET_HOSTNAME_D(x) ((Dee_uint8_t)((x)&DEE_UINT32_C(0x000000FF)))
#define DEE_HOSTNAME_LOCALHOST DEE_UINT32_C(0x7F000001) // 127.0.0.1 (in network endian)
#define DEE_BUILTIN_HTON16   /* nothing */
#define DEE_BUILTIN_HTON16_M /* nothing */
#define DEE_BUILTIN_HTON32   /* nothing */
#define DEE_BUILTIN_HTON32_M /* nothing */
#define DEE_BUILTIN_HTON64   /* nothing */
#define DEE_BUILTIN_HTON64_M /* nothing */
#else
#define DEE_SOCKADDRINET_HOST(a,b,c,d)\
(((Dee_uint32_t)((Dee_uint8_t)(a)))|\
 ((Dee_uint32_t)((Dee_uint8_t)(b))<<8)|\
 ((Dee_uint32_t)((Dee_uint8_t)(c))<<16)|\
 ((Dee_uint32_t)((Dee_uint8_t)(d))<<24))
#define DEE_SOCKET_INET_HOSTNAME_A(x) ((x)&DEE_UINT32_C(0x000000FF))
#define DEE_SOCKET_INET_HOSTNAME_B(x) (((x)&DEE_UINT32_C(0x0000FF00))>>8)
#define DEE_SOCKET_INET_HOSTNAME_C(x) (((x)&DEE_UINT32_C(0x00FF0000))>>16)
#define DEE_SOCKET_INET_HOSTNAME_D(x) (((x)&DEE_UINT32_C(0xFF000000))>>24)
#define DEE_HOSTNAME_LOCALHOST DEE_UINT32_C(0x0100007F) // 127.0.0.1 (in network endian)
#define DEE_BUILTIN_HTON16   DEE_BUILTIN_BSWAP16
#define DEE_BUILTIN_HTON16_M DEE_BUILTIN_BSWAP16_M
#define DEE_BUILTIN_HTON32   DEE_BUILTIN_BSWAP32
#define DEE_BUILTIN_HTON32_M DEE_BUILTIN_BSWAP32_M
#define DEE_BUILTIN_HTON64   DEE_BUILTIN_BSWAP64
#define DEE_BUILTIN_HTON64_M DEE_BUILTIN_BSWAP64_M
#endif
#define DEE_BUILTIN_NTOH16   DEE_BUILTIN_HTON16
#define DEE_BUILTIN_NTOH16_M DEE_BUILTIN_HTON16_M
#define DEE_BUILTIN_NTOH32   DEE_BUILTIN_HTON32
#define DEE_BUILTIN_NTOH32_M DEE_BUILTIN_HTON32_M
#define DEE_BUILTIN_NTOH64   DEE_BUILTIN_HTON64
#define DEE_BUILTIN_NTOH64_M DEE_BUILTIN_HTON64_M


//////////////////////////////////////////////////////////////////////////
// Returns the default backlog size
// -> This is equivalent to 'DeeSocket_DefaultMaxBacklog',
//    on the first call 'DeeSocket_DefaultMaxBacklog' is initialized with its env value
// -> This function is called internally if '-1' is used for 'max_backlog'
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeSocket_GetDefaultMaxBacklog(void);


// -- Socket flags --
#define DEE_SOCKET_FLAG_NONE           DEE_UINT16_C(0x0000)
#define DEE_SOCKET_FLAG_BINDING        DEE_UINT16_C(0x0001) // Socket is current being bound
#define DEE_SOCKET_FLAG_BOUND          DEE_UINT16_C(0x0002) // Socket was bound
#define DEE_SOCKET_FLAG_CONNECTING     DEE_UINT16_C(0x0004) // Internally set while a socket is connecting
#define DEE_SOCKET_FLAG_CONNECTED      DEE_UINT16_C(0x0008) // Socket was connected
#define DEE_SOCKET_FLAG_STARTLISTENING DEE_UINT16_C(0x0010) // Socket is starting to listen
#define DEE_SOCKET_FLAG_LISTENING      DEE_UINT16_C(0x0020) // Socket is listening (SO_ACCEPTCONN)
#define DEE_SOCKET_FLAG_SHUTDOWN       DEE_UINT16_C(0x0040) // Socket was shut down
#define DEE_SOCKET_FLAG_SHUTTINGDOWN   DEE_UINT16_C(0x0080) // Socket is being shut down
#define DEE_SOCKET_FLAG_CLOSED         DEE_UINT16_C(0x1000) // Socket was closed


DEE_DATA_DECL(DeeTypeObject) DeeSockAddr_Type;
DEE_DATA_DECL(DeeTypeObject) DeeSocket_Type;
DEE_DATA_DECL(DeeFileTypeObject) DeeSocketFile_Type;
DEE_DATA_DECL(DeeFileTypeObject) DeeSocketFileTo_Type;
#define /* int */DeeSockAddr_Check(ob)       DeeObject_InstanceOf(ob,&DeeSockAddr_Type)
#define /* int */DeeSockAddr_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeSockAddr_Type)
#define /* int */DeeSocket_Check(ob)         DeeObject_InstanceOf(ob,&DeeSocket_Type)
#define /* int */DeeSocket_CheckExact(ob)    DeeObject_InstanceOfExact(ob,&DeeSocket_Type)
#define /* int */DeeTCPFile_Check(ob)        DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeSocketFile_Type)
#define /* int */DeeTCPFile_CheckExact(ob)   DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeSocketFile_Type)
#define /* int */DeeUDPFile_Check(ob)        DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeSocketFileTo_Type)
#define /* int */DeeUDPFile_CheckExact(ob)   DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeSocketFileTo_Type)


DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSockAddrObject) *)
 _DeeSockAddrObject_New(void);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSockAddrObject) *)
 DeeSockAddrObject_New(DEE_A_IN struct DeeSockAddr const *addr) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED struct DeeSockAddr *)
 DeeSockAddrObject_Addr(DEE_A_IN_OBJECT(DeeSockAddrObject) *self) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Socket Address API
typedef struct { Dee_uint16_t in6_words[8]; } DeeINet6Host;
#define DEE_AF_AUTO  (-1) // Automatic address family deduction (usually detects 'AF_INET' + 'AF_INET6')
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeSockAddr *) DeeSockAddr_Alloc(void);
DEE_FUNC_DECL(void) DeeSockAddr_Free(DEE_A_IN struct DeeSockAddr *self) DEE_ATTRIBUTE_NONNULL((1));
// NOTE: 'host' and 'port' must be network endian in the following two functions
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSockAddr_InitINET(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN Dee_uint32_t host,
 DEE_A_IN Dee_uint16_t port) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSockAddr_InitINET6(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN DeeINet6Host const *host,
 DEE_A_IN Dee_uint16_t port) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Init sockaddr from given data. NOTE: All decimals in this are in host endian.
// NOTE: These functions accept DEE_AF_AUTO as argument for 'af'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSockAddr_InitFromString(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af, DEE_A_IN_Z char const *addr) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSockAddr_InitFromStringAndPort(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af, DEE_A_IN_Z char const *host,
 DEE_A_IN Dee_uint16_t port) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSockAddr_InitFromArgv(
 DEE_A_OUT struct DeeSockAddr *self, DEE_A_IN int af, DEE_A_IN int protocol,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject *const *argv) DEE_ATTRIBUTE_NONNULL((1));
// Returns '< 0' if 'lhs < rhs'; '== 0' if 'lhs == rhs' and '> 0' if 'lhs > rhs'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_WUNUSED int) DeeSockAddr_Compare(
 DEE_A_IN struct DeeSockAddr const *lhs,
 DEE_A_IN struct DeeSockAddr const *rhs);

#define DEE_SOCKADDR_STR_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_SOCKADDR_STR_FLAG_NOFAIL DEE_UINT32_C(0x00000001) /*< Don't return with an error if DNS lookup failed. */
#define DEE_SOCKADDR_STR_FLAG_NODNS  DEE_UINT32_C(0x00000002) /*< Don't use DNS names in the string (use raw IPs instead). */
#define DEE_SOCKADDR_STR_FLAG_NOPORT DEE_UINT32_C(0x00000004) /*< Don't include port numbers in address strings. */
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_INETHostname(DEE_A_IN Dee_uint32_t host, DEE_A_IN Dee_uint32_t flags);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_INET6Hostname(DEE_A_IN DeeINet6Host const *host, DEE_A_IN Dee_uint32_t flags);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSockAddr_Str(
 DEE_A_IN struct DeeSockAddr const *self, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Socket common API
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *)
 DeeSocket_New(DEE_A_IN int af, DEE_A_IN int type, DEE_A_IN int protocol);
DEE_FUNC_DECL(void) DeeSocket_Close(
 DEE_A_IN_OBJECT(DeeSocketObject) *self) DEE_ATTRIBUTE_NONNULL((1)); // NOTE: also calls shutdown(SHUT_RDWR)

//////////////////////////////////////////////////////////////////////////
// @param how: {SHUT_RD|SHUT_WR|SHUT_RDWR}; aka. {SD_RECEIVE|SD_SEND|SD_BOTH};
// Return false if the socket was already shut down; no-op on closed sockets
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_Shutdown(DEE_A_IN_OBJECT(DeeSocketObject) *self, DEE_A_IN int how) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetSockAddr(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_OUT struct DeeSockAddr *sock_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetPeerAddr(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_OUT struct DeeSockAddr *peer_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT(1) int) DeeSocket_TryGetSockAddr(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_OUT struct DeeSockAddr *sock_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT(1) int) DeeSocket_TryGetPeerAddr(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_OUT struct DeeSockAddr *peer_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *) DeeSocket_Accept(DEE_A_IN_OBJECT(DeeSocketObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_TryAcceptEx(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_OUT_OBJECT(DeeSocketObject) **result) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_AcceptTimedEx(DEE_A_IN_OBJECT(DeeSocketObject) const *self, DEE_A_IN unsigned int msecs, DEE_A_OUT_OBJECT(DeeSocketObject) **result) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(           DEE_A_RET_WUNUSED Dee_uint16_t) DeeSocket_Flags(DEE_A_IN_OBJECT(DeeSocketObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /* int */DeeSocket_IsBound(ob)          ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_BOUND)!=0)
#define /* int */DeeSocket_IsBinding(ob)        ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_BINDING)!=0)
#define /* int */DeeSocket_IsConnected(ob)      ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_CONNECTED)!=0)
#define /* int */DeeSocket_IsConnecting(ob)     ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_CONNECTING)!=0)
#define /* int */DeeSocket_IsListening(ob)      ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_LISTENING)!=0)
#define /* int */DeeSocket_IsListenStarting(ob) ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_STARTLISTENING)!=0)
#define /* int */DeeSocket_IsShutDown(ob)       ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_SHUTDOWN)!=0)
#define /* int */DeeSocket_IsShuttingDown(ob)   ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_SHUTTINGDOWN)!=0)
#define /* int */DeeSocket_IsClosed(ob)         ((DeeSocket_Flags(ob)&DEE_SOCKET_FLAG_CLOSED)!=0)

//////////////////////////////////////////////////////////////////////////
// Returns the address-family, type and protocol of a given socket, or -1 on error
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetAddressFamily(DEE_A_IN_OBJECT(DeeSocketObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetType(DEE_A_IN_OBJECT(DeeSocketObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetProtocol(DEE_A_IN_OBJECT(DeeSocketObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Connect/Bind the socket to a given address.
//  - Returns 0 on success
//  - Returns <0 on error
//  - Returns >0 if the socket was already bound/connected
#define DEE_SOCKET_MAXBACKLOG_DEFAULT   (-1)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_Bind(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN struct DeeSockAddr const *sock_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_Connect(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN struct DeeSockAddr const *peer_addr) DEE_ATTRIBUTE_NONNULL((1,2));
#define /*DEE_A_EXEC*/ DeeSocket_Listen(ob) DeeSocket_ListenEx(ob,DEE_SOCKET_MAXBACKLOG_DEFAULT)

//////////////////////////////////////////////////////////////////////////
// Begin listening on a given, bound socket.
// Set 'max_backlog' to '-1' to use 'DeeSocket_GetDefaultMaxBacklog()' instead.
//  - Returns 0 on success
//  - Returns <0 on error
//  - Returns >0 if the socket was already listening
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSocket_ListenEx(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN int max_backlog) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Send/Receive data from a given peer-address, or the address the given socket is bound to
#define /* int */DeeSocket_Send(ob,p,s,ws)                      DeeSocket_SendEx(ob,p,s,ws,0)
#define /* int */DeeSocket_Recv(ob,p,s,rs)                      DeeSocket_RecvEx(ob,p,s,rs,0)
#define /* int */DeeSocket_SendAll(ob,p,s)                      DeeSocket_SendAllEx(ob,p,s,0)
#define /* int */DeeSocket_RecvAll(ob,p,s)                      DeeSocket_RecvAllEx(ob,p,s,0)
#define /* int */DeeSocket_SendTo(ob,p,s,ws,peer_addr)          DeeSocket_SendToEx(ob,p,s,ws,peer_addr,0)
#define /* int */DeeSocket_RecvFrom(ob,p,s,rs,peer_addr)        DeeSocket_RecvFromEx(ob,p,s,rs,peer_addr,0)
#define /* int */DeeSocket_RecvDataFrom(ob,max_bytes,peer_addr) DeeSocket_RecvDataFromEx(ob,max_bytes,peer_addr,0)
#define /* int */DeeSocket_RecvData(ob,max_bytes)               DeeSocket_RecvDataEx(ob,max_bytes,0)
#define /* int */DeeSocket_RecvAllData(ob,max_bytes)            DeeSocket_RecvAllDataEx(ob,max_bytes,0)
// The following use the same flags as in the send/recv/sendto/recvfrom system calls
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SendEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_RecvEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SendAllEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_RecvAllEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SendToEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws, DEE_A_IN struct DeeSockAddr const *peer_addr, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1,4,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_RecvFromEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs, DEE_A_OUT struct DeeSockAddr *peer_addr, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1,4,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_RecvDataFromEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN Dee_size_t max_bytes, DEE_A_OUT struct DeeSockAddr *peer_addr, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_RecvDataEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN Dee_size_t max_bytes, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_RecvAllDataEx(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN Dee_size_t max_bytes, DEE_A_IN int flags) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns an object that can be used as a regular file, but
// is associated with a given socket, or a specific peer address.
// NOTE: If 'DeeSocket_FileTo' is used to create a bound file,
//       trying to read data from it will cause the socket to
//       discard any packages not originating from the specified address.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketFileToObject) *) DeeSocket_FileTo(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN struct DeeSockAddr const *peer_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketFileObject) *)
 DeeSocket_File(DEE_A_INOUT_OBJECT(DeeSocketObject) *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Creates a new UDP-style socket on a given port
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *)
 DeeSocket_NewUDP(DEE_A_IN Dee_uint16_t port);

//////////////////////////////////////////////////////////////////////////
// Creates a new server-style TCP socket
//  - Tries to create a dualstack IPv6 socket, but will create a regular IPv4 socket if that failed.
//  - Set 'max_backlog' to '-1' to use 'DeeSocket_GetDefaultMaxBacklog()' instead
//  >> sock = net::socket::tcp_server(12345,-1);
// Except for the dualstack bonus feature, this is identical to:
//  >> sock = net::socket(net::socket::AF_INET,net::socket::SOCK_STREAM);
//  >> sock.bind("0.0.0.0",12345);
//  >> sock.listen(-1);
#define /*DEE_A_EXEC*/ DeeSocket_NewTCPServer(ob) \
 DeeSocket_NewTCPServerEx(ob,DEE_SOCKET_MAXBACKLOG_DEFAULT)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *)
 DeeSocket_NewTCPServerEx(DEE_A_IN Dee_uint16_t port, DEE_A_IN int max_backlog);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSocketObject) *)
 DeeSocket_NewTCPClient(DEE_A_IN struct DeeSockAddr const *addr) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Get/Set the socket send/recv timeout
// Note, that a value of '0' isn't allowed and causes an Error.ValueError to be thrown
// Also note, that a thread stuck in a read() call will take up to 'msecs' milliseconds before responding to a close() call
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SetRecvTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN unsigned int msecs) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetRecvTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT unsigned int *msecs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SetSendTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN unsigned int msecs) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_GetSendTimeout(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_OUT unsigned int *msecs) DEE_ATTRIBUTE_NONNULL((1,2));

#define DEE_INTERNAL_SOL_ID(id)    (0x0F000000+(id))
#define DEE_INTERNAL_SO_ID(id)     (0xF0000000+(id))
// Deemon's special sockopts. (Use these if your platform doesn't implement them normally)
// Note though, that these only work with 'DeeSocket_SetSockOpt' / 'DeeSocket_GetSockOpt'
#define DEE_INTERNAL_SOL_SOCKET    DEE_INTERNAL_SOL_ID(0)
#define DEE_INTERNAL_SO_ACCEPTCONN DEE_INTERNAL_SO_ID(0)
#define DEE_INTERNAL_SO_TYPE       DEE_INTERNAL_SO_ID(1)
#define DEE_INTERNAL_SO_DOMAIN     DEE_INTERNAL_SO_ID(2)
#define DEE_INTERNAL_SO_PROTOCOL   DEE_INTERNAL_SO_ID(3)

//////////////////////////////////////////////////////////////////////////
// Get/Set a socket option
//  - The setter usually accepts at least one argument that would be returned by the getter
//  - Socket options without special support are handled like this:
//    >> setsockopt(?,?,string data) -> none;
//    >> setsockopt(?,?,none *p, size_t s) -> none;
//    >> getsockopt(?,?) -> string;
// In addition, the following are supported:
//  -> property SOL_SOCKET|DEE_INTERNAL_SOL_SOCKET::DEE_INTERNAL_SO_ACCEPTCONN -> bool
//  -> property SOL_SOCKET|DEE_INTERNAL_SOL_SOCKET::DEE_INTERNAL_SO_TYPE -> int
//  -> property SOL_SOCKET|DEE_INTERNAL_SOL_SOCKET::DEE_INTERNAL_SO_DOMAIN -> int
//  -> property SOL_SOCKET|DEE_INTERNAL_SOL_SOCKET::DEE_INTERNAL_SO_PROTOCOL -> int
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_SetSockOpt(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN int level, DEE_A_IN int optname,
 DEE_A_IN size_t argc, DEE_A_IN_R(argc) DeeObject *const *argv);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSocket_GetSockOpt(
 DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN int level, DEE_A_IN int optname);

//////////////////////////////////////////////////////////////////////////
// Socket name utility functions
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSocket_StringToProtocol(DEE_A_IN_Z char const *name, DEE_A_OUT int *protocol) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_AddressFamilyToString(DEE_A_IN int af);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_TypeToString(DEE_A_IN int type);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_ProtocolToString(DEE_A_IN int protocol);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_OptLevelToString(DEE_A_IN int level);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeSocket_OptNameToString(DEE_A_IN int optname);



//////////////////////////////////////////////////////////////////////////
// Maximum size of a udp packet.
// -> s.a.: 'DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE'
// -> s.a.: 'DeeSocket_RecvDataFrom'
// -> s.a.: 'DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE'
DEE_DATA_DECL(Dee_size_t) DeeSocket_MaxUDPPacketSize;

//////////////////////////////////////////////////////////////////////////
// Default maximum size for socket backlogs
// Unless overwritten with 'DEEMON_MAXBACKLOG', this is '5'
// -> s.a.: 'DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG'
DEE_DATA_DECL(int) DeeSocket_DefaultMaxBacklog;


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_SOCKET_API */

#endif /* !GUARD_DEEMON_SOCKET_H */
