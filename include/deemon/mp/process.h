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
#ifndef GUARD_DEEMON_MP_PROCECSS_H
#define GUARD_DEEMON_MP_PROCECSS_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
#include <deemon/optional/string_forward.h>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <unistd.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The process API will be moved into a Dex sooner or later --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeProcessObject);

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

#ifdef DEE_TYPES_SIZEOF_PID_T
DEE_STATIC_ASSERT(sizeof(pid_t) == DEE_TYPES_SIZEOF_PID_T);
#endif

typedef DEE_TYPES_UINT(DEE_TYPES_SIZEOF_DEE_PROCESS_RETURN) DeeProcessReturn;

#ifdef DEE_PLATFORM_WINDOWS
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
typedef Dee_uint32_t DeeProcessID;
typedef void *DeeProcessHandle;
#define DEE_PROCESS_INVALID_ID      0
#define DEE_PROCESS_INVALID_HANDLE  NULL
#elif defined(DEE_PLATFORM_UNIX)
typedef pid_t DeeProcessID;
typedef pid_t DeeProcessHandle;
#define DEE_PROCESS_INVALID_ID      ((pid_t)-1)
#define DEE_PROCESS_INVALID_HANDLE  ((pid_t)-1)
#endif

#ifdef DEE_LIMITED_API
#define DeeProcess_SIZEOF_REF \
 Dee_OFFSETOF(DeeProcessObject,p_exe)
struct DeeProcessObject {
 DEE_OBJECT_HEAD
 DeeProcessHandle              p_handle;   /*< Handle of the process (system-specific); must be closed. */
 struct DeeAtomicMutex         p_lock;     /*< Lock for this process. */
#define DEE_PROCESS_FLAG_NONE  DEE_UINT8_C(0x00)
#define DEE_PROCESS_FLAG_NOREF DEE_UINT8_C(0x01) /*< Non-reference process */
 Dee_uint8_t                   p_flags;    /*< Process flags. */
 // All of the following are only available for non-reference processes (with the 'DEE_PROCESS_FLAG_NOREF' flag set)
 DEE_A_REF DeeAnyStringObject *p_exe;      /*< [1..1] Process executable. */
 DEE_A_REF DeeAnyStringObject *p_args;     /*< [1..1] Process arguments. */
 DEE_A_REF DeeListObject      *p_environ;  /*< [0..1] Execution environment - list<tuple<string,string>>. */
 DEE_A_REF DeeFileFDObject    *p_stdin;    /*< [0..1] File object overriding stdin. */
 DEE_A_REF DeeFileFDObject    *p_stdout;   /*< [0..1] File object overriding stdout. */
 DEE_A_REF DeeFileFDObject    *p_stderr;   /*< [0..1] File object overriding stderr. */
 DEE_A_REF DeeAnyStringObject *p_cwd;      /*< [0..1] Current working directory of the child process. */
 double                        p_priority; /*< 0.0 - 1.0 normalized process priority. */
};
#define DeeProcess_FLAGS(ob)   ((DeeProcessObject *)Dee_REQUIRES_POINTER(ob))->p_flags
#define DeeProcess_IS_REF(ob)  ((DeeProcess_FLAGS(ob)&DEE_PROCESS_FLAG_NOREF)==0)
#define DeeProcess_INIT_COMMON(ob)\
 DeeAtomicMutex_Init(&(ob)->p_lock)
#define DeeProcess_INIT_REF(ob,handle)\
 (DeeProcess_INIT_COMMON(ob),\
 (ob)->p_handle=(handle))
#define DeeProcess_INIT_NONREF(ob)\
 (DeeProcess_INIT_COMMON(ob),\
 (ob)->p_handle=DEE_PROCESS_INVALID_HANDLE,\
 (ob)->p_environ=NULL,\
 (ob)->p_stdin=NULL,\
 (ob)->p_stdout=NULL,\
 (ob)->p_stderr=NULL,\
 (ob)->p_cwd=NULL,\
 (ob)->p_priority=0.5)
#define DeeProcess_LOCK(ob)     (&((DeeProcessObject *)(ob))->p_lock)
#define DeeProcess_ACQUIRE(ob)  DeeAtomicMutex_Acquire(DeeProcess_LOCK(ob))
#define DeeProcess_RELEASE(ob)  DeeAtomicMutex_Release(DeeProcess_LOCK(ob))
#define DeeProcess_HANDLE(ob)   ((DeeProcessObject *)(ob))->p_handle
#define DeeProcess_STARTED(ob)  (DeeProcess_HANDLE(ob)!=DEE_PROCESS_INVALID_HANDLE)
#define DeeProcess_DETACHED(ob) (DeeProcess_HANDLE(ob)==DEE_PROCESS_INVALID_HANDLE)
#else
#define DeeProcess_STARTED(ob)  DeeProcess_Started((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeProcess_DETACHED(ob) DeeProcess_Detached((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

DEE_DATA_DECL(DeeTypeObject) DeeProcess_Type;
#define DeeProcess_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeProcess_Type)
#define DeeProcess_Check          DeeProcess_CheckExact


#ifdef DEE_PLATFORM_WINDOWS
//////////////////////////////////////////////////////////////////////////
// Returns a process reference to a given win32 Process ID
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_Win32NewFromID(DEE_A_IN DeeProcessID id);
#endif
#ifdef DEE_PLATFORM_UNIX
//////////////////////////////////////////////////////////////////////////
// Returns a process reference to a given posix pid_t
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_PosixNewFromPid(DEE_A_IN DeeProcessHandle handle);
#endif

//////////////////////////////////////////////////////////////////////////
// Returns a process generated from a given commandline
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_Utf8New(DEE_A_IN_Z Dee_Utf8Char const *cmd);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_WideNew(DEE_A_IN_Z Dee_WideChar const *cmd);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_NewObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *cmd);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_Utf8NewEx(DEE_A_IN_Z Dee_Utf8Char const *exe, DEE_A_IN_Z Dee_Utf8Char const *args);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_WideNewEx(DEE_A_IN_Z Dee_WideChar const *exe, DEE_A_IN_Z Dee_WideChar const *args);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *) DeeProcess_NewExObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *exe, DEE_A_IN_OBJECT(DeeAnyStringObject) const *args);

//////////////////////////////////////////////////////////////////////////
// Starts a non-reference process
// >> If 'detached' is true, the process will be started detached from the current process
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeProcess_Start(DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN int detached);

//////////////////////////////////////////////////////////////////////////
// Detach a running process (implicitly called from destructor; may be called on reference processes)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeProcess_Detach(DEE_A_IN_OBJECT(DeeProcessObject) *self);

//////////////////////////////////////////////////////////////////////////
// Detach a running process (implicitly called from destructor; may be called on reference processes; no-op on posix)
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeProcess_Terminate(DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN DeeProcessReturn exit_code);

//////////////////////////////////////////////////////////////////////////
// Returns true if a given process contains a valid handle
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeProcess_Started(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
#define DeeProcess_Detached         !DeeProcess_Started

//////////////////////////////////////////////////////////////////////////
// Returns true if the process of a given handle was terminated (aka. zombie process)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeProcess_Terminated(DEE_A_IN_OBJECT(DeeProcessObject) const *self);

//////////////////////////////////////////////////////////////////////////
// Joins a given process, waiting for it to finish
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeProcess_Join(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_OUT_OPT DeeProcessReturn *retval);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeProcess_TryJoin(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_OUT_OPT DeeProcessReturn *retval);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeProcess_JoinTimed(
 DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_IN unsigned int msecs,
 DEE_A_OUT_OPT DeeProcessReturn *retval);

//////////////////////////////////////////////////////////////////////////
// Get/set the stdin/stdout/stderr file objects associated with a process.
// NOTE: These allow for any file-mapping, where pipe objects are automatically
//       created when setting something that doesn't support handles
// NOTE: Calling get will create a pipe file 
// NOTE: Setting an object that isn't a file.io or pipe, causes a TypeError (Meaning you don't have to check the typing yourself)
// NOTE: May only be called on non-running, non-ref processes
// NOTE: The getters may also be called on 'DeeProcess_Self()'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *) DeeProcess_GetStdin(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *) DeeProcess_GetStdout(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *) DeeProcess_GetStderr(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetStdin(DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *fp);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetStdout(DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *fp);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetStderr(DEE_A_INOUT_OBJECT(DeeProcessObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *fp);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_DelStdin(DEE_A_INOUT_OBJECT(DeeProcessObject) *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_DelStdout(DEE_A_INOUT_OBJECT(DeeProcessObject) *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_DelStderr(DEE_A_INOUT_OBJECT(DeeProcessObject) *self);

//////////////////////////////////////////////////////////////////////////
// Get/Set the priority of a given process as a floating point clamped between 0.0 and 1.0
// NOTE: Can be called on any kind of process at any time
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) double) DeeProcess_GetPriority(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetPriority(DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_IN double priority);

//////////////////////////////////////////////////////////////////////////
// Get/Del/Set the environment block of a given process
// NOTE: May only be called on non-running, non-ref processes or 'DeeProcess_Self()'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeProcess_GetEnviron(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetEnviron(DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_INOUT_OBJECT(DeeListObject) *env);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_DelEnviron(DEE_A_IN_OBJECT(DeeProcessObject) *self);

//////////////////////////////////////////////////////////////////////////
// Get/Set the current working directory of a process
// NOTE: The getter may be called on any kind of process
// NOTE: The setting may only be called on non-running, non-ref processes or 'DeeProcess_Self()'
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_SetCwd(DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *cwd);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_DelCwd(DEE_A_IN_OBJECT(DeeProcessObject) *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeProcess_Utf8Cwd(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeProcess_WideCwd(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
#define DeeProcess_Cwd  DeeProcess_Utf8Cwd

//////////////////////////////////////////////////////////////////////////
// Get/Set the commandline, executable or arguments of a process
// NOTE: Can be called on any kind of process at any time
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeProcess_Utf8Cmd(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeProcess_WideCmd(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeProcess_Utf8Exe(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeProcess_WideExe(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeProcess_Utf8Args(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeProcess_WideArgs(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeProcess_Utf8Argv(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeProcess_WideArgv(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
#define DeeProcess_Cmd  DeeProcess_Utf8Cmd
#define DeeProcess_Exe  DeeProcess_Utf8Exe
#define DeeProcess_Args DeeProcess_Utf8Args
#define DeeProcess_Argv DeeProcess_Utf8Argv


//////////////////////////////////////////////////////////////////////////
// Returns a special process-reference object that describes the current process
// NOTE: It is possible to create alias objects of this by using platform-specific functionality.
//       >> DeeProcess_Win32NewFromID(GetCurrentProcessId())
//       >> DeeProcess_PosixNewFromPid(getpid())
DEE_FUNC_DECL(DEE_A_RET_OBJECT_NOEXCEPT(DeeProcessObject) *) DeeProcess_Self(void);

//////////////////////////////////////////////////////////////////////////
// Returns the platform-specific process id of a given or the current process
// NOTE: Can be called on any running process; causes non-critical undefined behavior for terminated processes
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeProcessID) DeeProcess_ID(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeProcessID) DeeProcess_SelfID(void);

//////////////////////////////////////////////////////////////////////////
// Returns a list of all running processes (elements are process.ref objects)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeProcess_Enumerate(void);

//////////////////////////////////////////////////////////////////////////
// Runtime Initialization / Finalization
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeProcess_Initialize(void);
DEE_FUNC_DECL(void) DeeProcess_Finalize(void);


#ifdef DEE_PLATFORM_WINDOWS
// Windows-specific process functions; may be called on any running process
struct DeeTimeObject;
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeProcess_Win32Times(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **creation_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **exit_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **kernel_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **user_time);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeProcess_Win32IsCritical(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeProcess_Win32GetPriorityBoost(DEE_A_IN_OBJECT(DeeProcessObject) const *self);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeProcess_Win32SetPriorityBoost(DEE_A_IN_OBJECT(DeeProcessObject) *self, DEE_A_IN int enabled);
#endif

DEE_DECL_END

#if !defined(__INTELLISENSE__)
#if defined(DEE_PLATFORM_WINDOWS)
#ifdef _INC_WINDOWS
#define DeeProcess_SelfID     (DeeProcessID)GetCurrentProcessId
#endif
#elif defined(DEE_PLATFORM_UNIX)
#ifdef DeeProcess_HANDLE
#define DeeProcess_ID         DeeProcess_HANDLE
#endif
#define DeeProcess_SelfID     getpid
#else
#define DeeProcess_SelfID     DEE_PROCESS_INVALID_ID
#endif
#endif

#endif /* DEE_CONFIG_RUNTIME_HAVE_PROCESS */

#endif /* !GUARD_DEEMON_MP_PROCECSS_H */
