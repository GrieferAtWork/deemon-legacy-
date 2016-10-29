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
#ifndef GUARD_DEEMON_DEX_ZLIB_C
#define GUARD_DEEMON_DEX_ZLIB_C 1
#define DEE_EXTENSION 1

//@ dex.name = _system

#include "_system.h"
#include <deemon/error.h>
#include <deemon/time.h>
#include <deemon/unicode/char_traits.inl>
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#ifdef DEE_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#include <time.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H
#include <sys/sysinfo.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include <deemon/posix_features.inl>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeSystem_GetUptimeImpl(DEE_A_OUT Dee_uint64_t *msecs) {
 // Partially based on this:
 // http://stackoverflow.com/questions/30095439/how-do-i-get-system-up-time-in-milliseconds-in-c
#if defined(DEE_PLATFORM_WINDOWS)
 // On windows, this is actually the same thing
 // >> We use the time function though because
 //    it already does the dynamic loading part
 //    required to use 'GetTickCount64()'.
 *msecs = DeeTime_GetClock1000();
 return 0;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSINFO_H
 struct sysinfo sinf;
 if (sysinfo(&sinf) != 0) {
  DeeError_SystemError("sysinfo");
  return -1;
 }
 *msecs = ((Dee_uint64_t)sinf.uptime)*DEE_UINT64_C(1000);
 return 0;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SYSCTL_H
 struct timeval ts; Dee_size_t len = sizeof(ts);
 int mib[2] = {CTL_KERN,KERN_BOOTTIME};
 if (sysctl(mib,2,&ts,&len,NULL,0) != 0) {
  DeeError_SystemError("sysctl({CTL_KERN,KERN_BOOTTIME})");
  return -1;
 }
 *msecs = (Dee_uint64_t)ts.tv_sec*DEE_UINT64_C(1000)+
          (Dee_uint64_t)ts.tv_usec/DEE_UINT64_C(1000));
 return 0;
#elif DEE_HAVE_CLOCK_GETTIME
 struct timespec ts;
 if (clock_gettime(CLOCK_UPTIME_PRECISE,&ts) != 0) {
  DeeError_SystemError("clock_gettime(CLOCK_UPTIME_PRECISE)");
  return -1;
 }
 *msecs = (Dee_uint64_t)ts.tv_sec*DEE_UINT64_C(1000)+
          (Dee_uint64_t)ts.tv_nsec/DEE_UINT64_C(1000000));
 return 0;
#else
 // Implementation using '/proc/uptime'
 int fd,error; char buffer[64];
 long double result; char *iter,ch;
 if ((fd = open("/proc/uptime",O_RDONLY)) < 0) {
  DeeError_SystemError("open(\"/proc/uptime\")");
  return -1;
 }
 error = read(fd,buffer,63);
 if (close(fd) < 0) errno = 0;
 if (error < 0) {
  DeeError_SystemError("read(open(\"/proc/uptime\"))");
  return -1;
 }
 if (error > 63) error = 63; // shouldn't happen
 buffer[error] = 0;
 iter = buffer;
 while (DEE_CH_IS_DIGIT(*iter) || *iter == '.') ++iter;
 *iter = 0;
 if (DeeString_ToLDouble(buffer,&result) != 0) return -1;
 *msecs = (Dee_uint64_t)(result*1000.0L);
 return 0;
#endif
}

DEE_A_RET_EXCEPT(-1) int DeeSystem_GetCPUCountImpl(
 DEE_A_OUT Dee_uint64_t *ncpus) {
#if defined(DEE_PLATFORM_WINDOWS)
 SYSTEM_INFO sinfo;
 GetSystemInfo(&sinfo);
 *ncpus = (Dee_uint64_t)sinfo.dwNumberOfProcessors;
 return 0;
#elif DEE_HAVE_SYSCONF && defined(_SC_NPROCESSORS_ONLN)
 long result;
 if ((result = sysconf(_SC_NPROCESSORS_ONLN)) < 0) {
  DeeError_SystemError("sysconf(_SC_NPROCESSORS_ONLN)");
  return -1;
 }
 *ncpus = (Dee_uint64_t)result;
 return 0;
#else
 //int fd,error;
 //if ((fd = open("/proc/cpuinfo",O_RDONLY)) < 0) {
 // DeeError_SystemError("open(\"/proc/cpuinfo\")");
 // return -1;
 //}
 // TODO: parse '/proc/cpuinfo'
 DeeError_TODONotImplemented(); // TODO
 return -1;
#endif
}





static DeeObject *DEE_CALL _deesystem_uptime(DeeObject *args) {
 Dee_uint64_t result;
 if (DeeTuple_Unpack(args,":uptime",&result) != 0) return NULL;
 if (DeeSystem_GetUptimeImpl(&result) != 0) return NULL;
 return DeeObject_New(Dee_uint64_t,result);
}
static DeeObject *DEE_CALL _deesystem_cpucount(DeeObject *args) {
 Dee_uint64_t result;
 if (DeeTuple_Unpack(args,":cpucount",&result) != 0) return NULL;
 if (DeeSystem_GetCPUCountImpl(&result) != 0) return NULL;
 return DeeObject_New(Dee_uint64_t,result);
}



//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_FUNCTION("uptime",&_deesystem_uptime,"() -> uint64_t\n"
  "@throws Error.SystemError: Failed to retrive the current uptime\n"
  "@return: The current uptime of the system in milliseconds"),
 DeeDex_EXPORT_FUNCTION("cpucount",&_deesystem_cpucount,"() -> uint64_t\n"
  "@throws Error.SystemError: Failed to retrive the amount of cores\n"
  "@return: The amount of available CPU cores"),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ZLIB_C */
