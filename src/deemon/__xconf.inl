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
#ifndef GUARD_DEEMON___XCONF_INL
#define GUARD_DEEMON___XCONF_INL 1

// Extended configuration options
// -> If you really want to tweak deemon, do it here!
// ... Though you mostly can only configure buffersizes... :(

#include <deemon/__conf.inl>


//////////////////////////////////////////////////////////////////////////
// Force write-access to member variables,
// even when they're declared as read-only.
//  - v100 implemented this with a special dev-only member declaration,
//    that explicitly enabled write-access only in developer builds.
//  - v101 removed that declaration and overwrote the regular
//    read-only declaration to always include write-access in dev-builds.
//  - v102+ uses this, as with the rise of doc-generation,
//    we need some kind of hidden way that enables write-access
//    without interfering with the actual declarations.
// >> Though the same rules still apply: with this enabled,
//    you can really break a lot of stuff.
// >> This will not affect attribute enumeration, or _docgen, technically
//    leading them to lie about some attributes, and possibly hide others.
#ifndef DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
#define DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS DEE_DEVEL_BUILD
#endif

//////////////////////////////////////////////////////////////////////////
// Enabled access to some hidden type-members
//  - Hidden members will not show up in documentations (because they should always start with "__")
//  - Hidden members can still be enumerated using 'util::attr::enumattr'
#ifndef DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define DEE_XCONFIG_HAVE_HIDDEN_MEMBERS DEE_DEVEL_BUILD
#endif
#ifndef DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS
#define DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#endif
#ifndef DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS
#define DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS
#endif
#ifndef DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS
#define DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS
#endif


//////////////////////////////////////////////////////////////////////////
// Allows for recursive calls to 'Dee_Initialize' / 'Dee_Finalize'
// NOTE: Enabling this also makes those functions thread-safe
#ifndef DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
#define DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION 1
#endif


//////////////////////////////////////////////////////////////////////////
// Always add ids to the representations of ASTs
#ifndef DEE_XCONFIG_PARSER_XAST_REPR_ALWAYS_ADD_IDS
#define DEE_XCONFIG_PARSER_XAST_REPR_ALWAYS_ADD_IDS 0
#endif

//////////////////////////////////////////////////////////////////////////
// If enabled, use intrinsic function names in ast representations
// INPUT:  >> attr = "upper";
//         >> data = "foobar".operator . (attr)();
// OUTPUT: >> data = ("foobar").operator . (attr)();
// OUTPUT: >> data = __getattr__("foobar",attr)();
#ifndef DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
#define DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS 1
#endif

//////////////////////////////////////////////////////////////////////////
// Use computed gotos when executing generated code.
// NOTE: If the host compiler does not support this feature, this option is ignored.
#ifndef DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS
#define DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS 1
#endif


//////////////////////////////////////////////////////////////////////////
// Size of the generic object attribute lookup cache
// Using this cache, repeated lookup of object attributes can be performed faster
//  - Set to 0, or undef to disable.
//  - Don't define as something too low, as that wouldn't make any sense
// NOTES:
//   - To work properly, the cache uses the address of the type
//     implementing any given attribute, and alongside the address
//     of the user-provided attribute name, an ID is generated and
//     modulated by DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE.
//     >> So two strings containing the same value, but originating
//        from two different memory addresses will don't get a bonus
//        from the attribute lookup cache.
//     >> For usercode to take advantage of this cache, code must
//        be compiled with 'DEE_COMPILER_FLAG_OPTIMIZE_REUSE_CONSTS'.
//   - To actually use a hash of the attribute string,
//     DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_USEHASH must be defined.
//     WARNING: This will slow down the cache considerably, though.
//   - When 'DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYREAD' or
//     'DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYWRITE' are defined
//     as non-zero, deemon will not block until the cache is available
//     for the calling thread, but simply skip cache access.
//   - By default, both of these are enabled, because even though
//     there isn't any form of loop construct inside the cache
//     lookup, its better to make sure we will never spend idle
//     time because of something that's meant to make us faster.
#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE 128
#endif
#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYREAD
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYREAD  1
#endif
#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYWRITE
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYWRITE 1
#endif
// Each cache entry has its own lock
// NOTE: Despite sounding like a bad idea, it really isn't
//       as an atomic mutex only takes 1 byte, of which
//       cache entries have 3 unused ones due to memory alignment.
#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH       1
#endif
// v NOTE: We can't disable able, because of the slim chance
//         of someone allocating a string, using it to look up
//         an attribute, the free it, only to allocate a new one,
//         that then (by chance) gets allocated at the same location,
//         Thus resulting in a false match with a string that isn't
//         actually the real deal!
//   TODO: When this is disabled, we need to call strcmp to
//         compare the member/getset/method name from the cache
//         against the attribute given to us by the caller.
#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_USEHASH
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_USEHASH        1
#endif


//////////////////////////////////////////////////////////////////////////
// Buffersizes used in various calls to system functions
// -> All of these are mearly hints and only regulate the allocation behavior
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETMODULENAME
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETMODULENAME             256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES
#define DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES                    256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETTMP                    256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETDLLDIRECTORY           256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMDIRECTORY        256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETWINDOWSDIRECTORY       256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSTEMWINDOWSDIRECTORY 256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETSYSWOW64DIRECTORY      256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETCWD                    256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD
#define DEE_XCONFIG_FSBUFSIZE_POSIXGETCWD                    256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETENV
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETENV                    256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32READLINK
#define DEE_XCONFIG_FSBUFSIZE_WIN32READLINK                  256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK
#define DEE_XCONFIG_FSBUFSIZE_POSIXREADLINK                  256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME
#define DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME                   256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID
#define DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID                 256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32PGETFILEINFORMATIONBYHANDLEEX
#define DEE_XCONFIG_FSBUFSIZE_WIN32PGETFILEINFORMATIONBYHANDLEEX 256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETFINALPATHBYHANDLE
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETFINALPATHBYHANDLE      256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETMAPPEDFILENAME
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETMAPPEDFILENAME         256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32NTQUERYOBJECT
#define DEE_XCONFIG_FSBUFSIZE_WIN32NTQUERYOBJECT             256
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETCOMPUTERNAME           MAX_COMPUTERNAME_LENGTH
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME
#define DEE_XCONFIG_FSBUFSIZE_WIN32GETUSERNAME               UNLEN
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_POSIXGETHOSTNAME
#define DEE_XCONFIG_FSBUFSIZE_POSIXGETHOSTNAME               16
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_POSIXGETLOGIN
#define DEE_XCONFIG_FSBUFSIZE_POSIXGETLOGIN                  16
#endif
#ifndef DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY /*< copy-through fd buffersize */
#define DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY                   8096
#define DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY_ALLOCHEAP 1 /*< Define to allocate the buffer on the heap. */
#endif

//////////////////////////////////////////////////////////////////////////
// Allow the use of win32 SEH (aka. '__try')
// for catching system-signals on windows
// NOTE: If SEH isn't available, this config is ignored
#ifndef DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH
#define DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH 1
#endif

//////////////////////////////////////////////////////////////////////////
// Allow the use of posix process signals (aka. 'signal(SIGSEGV,...)')
// for catching system-signals on posix
// WARNING: The implementation does kind-of break the rules
//          by calling 'pthread_getspecific' (which it kind-of shouldn't)
//          -> So keeping that in mind, you should declare any
//             function that this could possibly interfere
//             with as '[[nothrow]]'
//          >> // Marked as nothrow, as to not handle the raised exception when called.
//          >> extern "...": [[nothrow]] none raise(int signal);
#ifndef DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS
#define DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS 1
#endif

//////////////////////////////////////////////////////////////////////////
// Use signal handlers to check for divide-by-zero
// NOTE: Ignored if no signal handlers are available for the platform
// -> It isn't recommended to turn these on, as the cost
//    of setting up and tearing down a signal handler is
//    much greater than simply checking for '0' beforehand.
#ifndef DEE_XCONFIG_INTEGER_DIVIDE_BY_ZERO_USE_SIGNALHANDLER
#define DEE_XCONFIG_INTEGER_DIVIDE_BY_ZERO_USE_SIGNALHANDLER 0
#endif
#ifndef DEE_XCONFIG_INTEGER_MODULO_BY_ZERO_USE_SIGNALHANDLER
#define DEE_XCONFIG_INTEGER_MODULO_BY_ZERO_USE_SIGNALHANDLER 0
#endif


//////////////////////////////////////////////////////////////////////////
// Default stacklimit
#ifndef DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT
#if defined(DEE_DEBUG)
// Less inlined functions + bigger stack to keep all
// available variables -> don't push it in debug builds
#define DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT 360
#else
#define DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT 512
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Makes functions such as 'fs::path::join' constexpr,
// that always return the same when compiling on a single platform,
// but would generate different results on different platforms.
// >> We leave this on to discourage use of 'deemon -o', as
//    well as having more parts of the API being evaluated at compile-time
#ifndef DEE_XCONFIG_HAVE_CONSTEXPR_PLATFORM_FUNCTIONS
#define DEE_XCONFIG_HAVE_CONSTEXPR_PLATFORM_FUNCTIONS 1
#endif


//////////////////////////////////////////////////////////////////////////
// Enable deprecated compiler flag names on the command line
// >> The value this is defined as specifies which version should still be supported
// >> Define as '0' to disable all deprecated flags
#ifndef DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS
#define DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS (DEE_API_VERSION-2)
#endif





















//////////////////////////////////////////////////////////////////////////
// Dynamic buffersize when receiving data in sockets
// -> Used by 'socket.read()' and friends
// NOTE: The value of this doesn't really matter
#ifndef DEE_XCONFIG_SOCKET_RECVDATA_BUFSIZE
#define DEE_XCONFIG_SOCKET_RECVDATA_BUFSIZE 256
#endif


//////////////////////////////////////////////////////////////////////////
// Default buffer size when calling recvfrom(s)
// -> If the given max_size 's' is larger than the active
//    max-size, it will be truncated.
// NOTE: This is merely the default value of that max-size, but
//       the actual value can be set at runtime using an environment
//       variable DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE ('DEEMON_UDPPACKETSIZE' by default)
// e.g.:
// >> #!/bin/bash
// >> DEEMON_UDPPACKETSIZE=4096
// >> # The maximum size of a udp packet is now '4096' instead of '576'
// >> deemon my_script.dee
// NOTE: The used size is initialized the first time 'DeeSocket_RecvDataFrom' is called
#ifndef DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE
#define DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE 576
#endif

//////////////////////////////////////////////////////////////////////////
// Default-default max backlog size for listen-style sockets
// Can be overwritten by setting '$DEEMON_MAXBACKLOG'
// which can be overwritten again by a call to the explicit call to 
// NOTE: The used size is initialized the first time 'DeeSocket_RecvDataFrom' is called
#ifndef DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG
#define DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG 5 // reasonable value for most situations
#endif

//////////////////////////////////////////////////////////////////////////
// Maximum time that a socket may idly spend inside the kernel at a time,
// before having to switch back to user-space and check it is being closed.
// -> These timeouts are required to take the headache of having to
//    to either having to do crazy $h1t in order to unblock sockets,
//    before realizing that the easiest way is to simply check for
//    termination from time to time.
// -> With this, you can simply call 'close()' to safely stop a socket at any point
#ifndef DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT
#define DEE_XCONFIG_SOCKET_DEFAULT_RECV_TIMEOUT   100   // 100 milliseconds
#endif
#ifndef DEE_XCONFIG_SOCKET_DEFAULT_SEND_TIMEOUT
#define DEE_XCONFIG_SOCKET_DEFAULT_SEND_TIMEOUT   100   // 100 milliseconds
#endif
#ifndef DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT
#define DEE_XCONFIG_SOCKET_DEFAULT_ACCEPT_TIMEOUT 100   // 100 milliseconds
#endif

//////////////////////////////////////////////////////////////////////////
// Socket configuration used by the intrinsic TCP/UDP socket types
// NOTE: Don't tinker with these. - Otherwise sockets might break!
#ifndef DEE_TCP_AF
#define DEE_TCP_TYPE     SOCK_STREAM
#define DEE_TCP_PROTOCOL IPPROTO_TCP
#endif
#ifndef DEE_UDP_AF
#define DEE_UDP_AF       AF_INET
#define DEE_UDP_TYPE     SOCK_DGRAM
#define DEE_UDP_PROTOCOL IPPROTO_UDP
#endif



#endif /* !GUARD_DEEMON___XCONF_INL */
