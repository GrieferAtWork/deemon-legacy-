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
#ifndef GUARD_DEEMON_VFS___VFSCONF_INL
#define GUARD_DEEMON_VFS___VFSCONF_INL 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// Control Availability of virtual files.
//////////////////////////////////////////////////////////////////////////

#define DEE_VFSCONFIG_HAVEFILE_DEV                        DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/dev"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_NULL                   DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/null"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_RANDOM                 DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/random"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_STDERR                 DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/stderr"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_STDIN                  DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/stdin"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_STDOUT                 DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/stdout"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_URANDOM                DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/urandom"). */
#define DEE_VFSCONFIG_HAVEFILE_DEV_ZERO                   DEE_VFSCONFIG_HAVEFILE_DEV          /*< fs::exists("/dev/zero"). */
#define DEE_VFSCONFIG_HAVEFILE_MOUNT                      DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/mount"). */
#define DEE_VFSCONFIG_HAVE_MOUNT_SCANDRIVES               DEE_VFSCONFIG_HAVEFILE_MOUNT        /*< fs::dir("/mount"). */
#define DEE_VFSCONFIG_HAVEFILE_HOME                       DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/home"). */
#define DEE_VFSCONFIG_HAVEFILE_NET                        DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/net"). */
#define DEE_VFSCONFIG_HAVE_NET_SCANSERVER                 DEE_VFSCONFIG_HAVEFILE_NET          /*< fs::dir("/dev"). */
#define DEE_VFSCONFIG_HAVE_NET_SCANSHARE                  DEE_VFSCONFIG_HAVEFILE_NET          /*< fs::dir("/dev/[SERVER]"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC                       DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/proc"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID                   DEE_VFSCONFIG_HAVEFILE_PROC         /*< fs::exists("/proc/[PID]"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE           DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/cmdline"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD               DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/cwd"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE               DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/exe"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD                DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/fd"). */
#define DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS            DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD  /*< fs::dir("/proc/[PID]/fd"). */
#define DEE_VFSCONFIG_HAVE_PROC_PID_FD_NOSELF             DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD  /*< "/proc/[PID]/fd" is available for any process and not just "/proc/self" */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT              DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/root"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32               DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/[PID]/w32"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD       DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 /*< fs::exists("/proc/[PID]/w32/closefd"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE      DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 /*< fs::exists("/proc/[PID]/w32/exitcode"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 /*< fs::exists("/proc/[PID]/w32/priority_boost"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 /*< fs::exists("/proc/[PID]/w32/priority_class"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE     DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 /*< fs::exists("/proc/[PID]/w32/terminate"). */
#define DEE_VFSCONFIG_HAVEFILE_PROC_SELF                  DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::exists("/proc/self"). */
#define DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS                  DEE_VFSCONFIG_HAVEFILE_PROC_PID     /*< fs::dir("/proc"). (NOTE: still yields 'self', but doesn't include running pids) */
#define DEE_VFSCONFIG_HAVEFILE_TMP                        DEE_CONFIG_RUNTIME_HAVE_VFS         /*< fs::exists("/tmp"). */


#endif /* !GUARD_DEEMON_VFS___VFSCONF_INL */
