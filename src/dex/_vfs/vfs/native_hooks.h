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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_HOOKS_H
#define GUARD_DEEMON_VFS_VFS_NATIVE_HOOKS_H 1

#include <deemon/__conf.inl>
#include <deemon/fs/dynamic_fs.h>
#include <deemon/optional/fs_api.fileproperty.h>

DEE_DECL_BEGIN

#define DEE_VFS_HFSVERSION 102


#define DeeVFS_NativeHooks DeeVFS_NativeHooks_v102
extern struct DeeFSApi_v102 DeeVFS_NativeHooks_v102;

#ifndef DEE_WITHOUT_THREADS
#define DEE_HFS_CHECKINTERRUPT(...) if(DeeThread_CheckInterrupt()!=0){__VA_ARGS__;}
#else
#define DEE_HFS_CHECKINTERRUPT(...) /* nothing */
#endif

#define DeeHFS_Utf8GetCwd            (*DeeVFS_NativeHooks_v102._fs102_Utf8GetCwd)
#define DeeHFS_WideGetCwd            (*DeeVFS_NativeHooks_v102._fs102_WideGetCwd)
#define DeeHFS_Utf8Chdir             (*DeeVFS_NativeHooks_v102._fs102_Utf8Chdir)
#define DeeHFS_WideChdir             (*DeeVFS_NativeHooks_v102._fs102_WideChdir)
#define DeeHFS_Utf8ChdirObject       (*DeeVFS_NativeHooks_v102._fs102_Utf8ChdirObject)
#define DeeHFS_WideChdirObject       (*DeeVFS_NativeHooks_v102._fs102_WideChdirObject)
#define DeeHFS_Utf8GetHome           (*DeeVFS_NativeHooks_v102.fs102_Utf8GetHome)
#define DeeHFS_WideGetHome           (*DeeVFS_NativeHooks_v102.fs102_WideGetHome)
#define DeeHFS_Utf8GetUserHome       (*DeeVFS_NativeHooks_v102.fs102_Utf8GetUserHome)
#define DeeHFS_WideGetUserHome       (*DeeVFS_NativeHooks_v102.fs102_WideGetUserHome)
#define DeeHFS_Utf8GetUserHomeObject (*DeeVFS_NativeHooks_v102.fs102_Utf8GetUserHomeObject)
#define DeeHFS_WideGetUserHomeObject (*DeeVFS_NativeHooks_v102.fs102_WideGetUserHomeObject)
#define DeeHFS_Utf8GetTmp            (*DeeVFS_NativeHooks_v102.fs102_Utf8GetTmp)
#define DeeHFS_WideGetTmp            (*DeeVFS_NativeHooks_v102.fs102_WideGetTmp)
#define DeeHFS_Utf8EnumEnv           (*DeeVFS_NativeHooks_v102.fs102_Utf8EnumEnv)
#define DeeHFS_WideEnumEnv           (*DeeVFS_NativeHooks_v102.fs102_WideEnumEnv)
#define DeeHFS_Utf8HasEnv            (*DeeVFS_NativeHooks_v102.fs102_Utf8HasEnv)
#define DeeHFS_WideHasEnv            (*DeeVFS_NativeHooks_v102.fs102_WideHasEnv)
#define DeeHFS_Utf8HasEnvObject      (*DeeVFS_NativeHooks_v102.fs102_Utf8HasEnvObject)
#define DeeHFS_WideHasEnvObject      (*DeeVFS_NativeHooks_v102.fs102_WideHasEnvObject)
#define DeeHFS_Utf8GetEnv            (*DeeVFS_NativeHooks_v102.fs102_Utf8GetEnv)
#define DeeHFS_WideGetEnv            (*DeeVFS_NativeHooks_v102.fs102_WideGetEnv)
#define DeeHFS_Utf8GetEnvObject      (*DeeVFS_NativeHooks_v102.fs102_Utf8GetEnvObject)
#define DeeHFS_WideGetEnvObject      (*DeeVFS_NativeHooks_v102.fs102_WideGetEnvObject)
#define DeeHFS_Utf8TryGetEnv         (*DeeVFS_NativeHooks_v102.fs102_Utf8TryGetEnv)
#define DeeHFS_WideTryGetEnv         (*DeeVFS_NativeHooks_v102.fs102_WideTryGetEnv)
#define DeeHFS_Utf8TryGetEnvObject   (*DeeVFS_NativeHooks_v102.fs102_Utf8TryGetEnvObject)
#define DeeHFS_WideTryGetEnvObject   (*DeeVFS_NativeHooks_v102.fs102_WideTryGetEnvObject)
#define DeeHFS_Utf8DelEnv            (*DeeVFS_NativeHooks_v102.fs102_Utf8DelEnv)
#define DeeHFS_WideDelEnv            (*DeeVFS_NativeHooks_v102.fs102_WideDelEnv)
#define DeeHFS_Utf8DelEnvObject      (*DeeVFS_NativeHooks_v102.fs102_Utf8DelEnvObject)
#define DeeHFS_WideDelEnvObject      (*DeeVFS_NativeHooks_v102.fs102_WideDelEnvObject)
#define DeeHFS_Utf8SetEnv            (*DeeVFS_NativeHooks_v102.fs102_Utf8SetEnv)
#define DeeHFS_WideSetEnv            (*DeeVFS_NativeHooks_v102.fs102_WideSetEnv)
#define DeeHFS_Utf8SetEnvObject      (*DeeVFS_NativeHooks_v102.fs102_Utf8SetEnvObject)
#define DeeHFS_WideSetEnvObject      (*DeeVFS_NativeHooks_v102.fs102_WideSetEnvObject)
#define DeeHFS_Utf8GetTimes          (*DeeVFS_NativeHooks_v102._fs102_Utf8GetTimes)
#define DeeHFS_WideGetTimes          (*DeeVFS_NativeHooks_v102._fs102_WideGetTimes)
#define DeeHFS_Utf8GetTimesObject    (*DeeVFS_NativeHooks_v102._fs102_Utf8GetTimesObject)
#define DeeHFS_WideGetTimesObject    (*DeeVFS_NativeHooks_v102._fs102_WideGetTimesObject)
#define DeeHFS_Utf8SetTimes          (*DeeVFS_NativeHooks_v102._fs102_Utf8SetTimes)
#define DeeHFS_WideSetTimes          (*DeeVFS_NativeHooks_v102._fs102_WideSetTimes)
#define DeeHFS_Utf8SetTimesObject    (*DeeVFS_NativeHooks_v102._fs102_Utf8SetTimesObject)
#define DeeHFS_WideSetTimesObject    (*DeeVFS_NativeHooks_v102._fs102_WideSetTimesObject)
#define DeeHFS_Utf8GetMod            (*DeeVFS_NativeHooks_v102._fs102_Utf8GetMod)
#define DeeHFS_WideGetMod            (*DeeVFS_NativeHooks_v102._fs102_WideGetMod)
#define DeeHFS_Utf8GetModObject      (*DeeVFS_NativeHooks_v102._fs102_Utf8GetModObject)
#define DeeHFS_WideGetModObject      (*DeeVFS_NativeHooks_v102._fs102_WideGetModObject)
#define DeeHFS_Utf8Chmod             (*DeeVFS_NativeHooks_v102._fs102_Utf8Chmod)
#define DeeHFS_WideChmod             (*DeeVFS_NativeHooks_v102._fs102_WideChmod)
#define DeeHFS_Utf8ChmodObject       (*DeeVFS_NativeHooks_v102._fs102_Utf8ChmodObject)
#define DeeHFS_WideChmodObject       (*DeeVFS_NativeHooks_v102._fs102_WideChmodObject)
#define DeeHFS_Utf8GetOwn            (*DeeVFS_NativeHooks_v102._fs102_Utf8GetOwn)
#define DeeHFS_WideGetOwn            (*DeeVFS_NativeHooks_v102._fs102_WideGetOwn)
#define DeeHFS_Utf8GetOwnObject      (*DeeVFS_NativeHooks_v102._fs102_Utf8GetOwnObject)
#define DeeHFS_WideGetOwnObject      (*DeeVFS_NativeHooks_v102._fs102_WideGetOwnObject)
#define DeeHFS_Utf8Chown             (*DeeVFS_NativeHooks_v102._fs102_Utf8Chown)
#define DeeHFS_WideChown             (*DeeVFS_NativeHooks_v102._fs102_WideChown)
#define DeeHFS_Utf8ChownObject       (*DeeVFS_NativeHooks_v102._fs102_Utf8ChownObject)
#define DeeHFS_WideChownObject       (*DeeVFS_NativeHooks_v102._fs102_WideChownObject)
#define DeeHFS_Utf8HasProperty       (*DeeVFS_NativeHooks_v102._fs102_Utf8HasProperty)
#define DeeHFS_WideHasProperty       (*DeeVFS_NativeHooks_v102._fs102_WideHasProperty)
#define DeeHFS_Utf8HasPropertyObject (*DeeVFS_NativeHooks_v102._fs102_Utf8HasPropertyObject)
#define DeeHFS_WideHasPropertyObject (*DeeVFS_NativeHooks_v102._fs102_WideHasPropertyObject)
#define DeeHFS_Utf8Remove            (*DeeVFS_NativeHooks_v102._fs102_Utf8Remove)
#define DeeHFS_WideRemove            (*DeeVFS_NativeHooks_v102._fs102_WideRemove)
#define DeeHFS_Utf8RemoveObject      (*DeeVFS_NativeHooks_v102._fs102_Utf8RemoveObject)
#define DeeHFS_WideRemoveObject      (*DeeVFS_NativeHooks_v102._fs102_WideRemoveObject)
#define DeeHFS_Utf8Unlink            (*DeeVFS_NativeHooks_v102._fs102_Utf8Unlink)
#define DeeHFS_WideUnlink            (*DeeVFS_NativeHooks_v102._fs102_WideUnlink)
#define DeeHFS_Utf8UnlinkObject      (*DeeVFS_NativeHooks_v102._fs102_Utf8UnlinkObject)
#define DeeHFS_WideUnlinkObject      (*DeeVFS_NativeHooks_v102._fs102_WideUnlinkObject)
#define DeeHFS_Utf8RmDir             (*DeeVFS_NativeHooks_v102._fs102_Utf8RmDir)
#define DeeHFS_WideRmDir             (*DeeVFS_NativeHooks_v102._fs102_WideRmDir)
#define DeeHFS_Utf8RmDirObject       (*DeeVFS_NativeHooks_v102._fs102_Utf8RmDirObject)
#define DeeHFS_WideRmDirObject       (*DeeVFS_NativeHooks_v102._fs102_WideRmDirObject)
#define DeeHFS_Utf8MkDir             (*DeeVFS_NativeHooks_v102._fs102_Utf8MkDir)
#define DeeHFS_WideMkDir             (*DeeVFS_NativeHooks_v102._fs102_WideMkDir)
#define DeeHFS_Utf8MkDirObject       (*DeeVFS_NativeHooks_v102._fs102_Utf8MkDirObject)
#define DeeHFS_WideMkDirObject       (*DeeVFS_NativeHooks_v102._fs102_WideMkDirObject)
#define DeeHFS_Utf8Copy              (*DeeVFS_NativeHooks_v102._fs102_Utf8Copy)
#define DeeHFS_WideCopy              (*DeeVFS_NativeHooks_v102._fs102_WideCopy)
#define DeeHFS_Utf8CopyObject        (*DeeVFS_NativeHooks_v102._fs102_Utf8CopyObject)
#define DeeHFS_WideCopyObject        (*DeeVFS_NativeHooks_v102._fs102_WideCopyObject)
#define DeeHFS_Utf8Move              (*DeeVFS_NativeHooks_v102._fs102_Utf8Move)
#define DeeHFS_WideMove              (*DeeVFS_NativeHooks_v102._fs102_WideMove)
#define DeeHFS_Utf8MoveObject        (*DeeVFS_NativeHooks_v102._fs102_Utf8MoveObject)
#define DeeHFS_WideMoveObject        (*DeeVFS_NativeHooks_v102._fs102_WideMoveObject)
#define DeeHFS_Utf8Link              (*DeeVFS_NativeHooks_v102._fs102_Utf8Link)
#define DeeHFS_WideLink              (*DeeVFS_NativeHooks_v102._fs102_WideLink)
#define DeeHFS_Utf8LinkObject        (*DeeVFS_NativeHooks_v102._fs102_Utf8LinkObject)
#define DeeHFS_WideLinkObject        (*DeeVFS_NativeHooks_v102._fs102_WideLinkObject)
#define DeeHFS_Utf8Opendir           (*DeeVFS_NativeHooks_v102._fs102_Utf8Opendir)
#define DeeHFS_WideOpendir           (*DeeVFS_NativeHooks_v102._fs102_WideOpendir)
#define DeeHFS_Utf8OpendirObject     (*DeeVFS_NativeHooks_v102._fs102_Utf8OpendirObject)
#define DeeHFS_WideOpendirObject     (*DeeVFS_NativeHooks_v102._fs102_WideOpendirObject)
#define DeeHFS_Utf8Open              (*DeeVFS_NativeHooks_v102._fs102_Utf8Open)
#define DeeHFS_WideOpen              (*DeeVFS_NativeHooks_v102._fs102_WideOpen)
#define DeeHFS_Utf8OpenObject        (*DeeVFS_NativeHooks_v102._fs102_Utf8OpenObject)
#define DeeHFS_WideOpenObject        (*DeeVFS_NativeHooks_v102._fs102_WideOpenObject)
#define DeeHFS_Utf8Readlink          (*DeeVFS_NativeHooks_v102._fs102_Utf8Readlink)
#define DeeHFS_WideReadlink          (*DeeVFS_NativeHooks_v102._fs102_WideReadlink)
#define DeeHFS_Utf8ReadlinkObject    (*DeeVFS_NativeHooks_v102._fs102_Utf8ReadlinkObject)
#define DeeHFS_WideReadlinkObject    (*DeeVFS_NativeHooks_v102._fs102_WideReadlinkObject)

#define DeeHFS_Utf8IsFile(path)                   DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define DeeHFS_WideIsFile(path)                   DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFILE)
#define DeeHFS_Utf8IsFileObject(path)       DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISFILE)
#define DeeHFS_WideIsFileObject(path)       DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISFILE)
#define DeeHFS_Utf8IsDir(path)                    DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define DeeHFS_WideIsDir(path)                    DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDIR)
#define DeeHFS_Utf8IsDirObject(path)        DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISDIR)
#define DeeHFS_WideIsDirObject(path)        DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISDIR)
#define DeeHFS_Utf8IsLink(path)                   DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define DeeHFS_WideIsLink(path)                   DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISLINK)
#define DeeHFS_Utf8IsLinkObject(path)       DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISLINK)
#define DeeHFS_WideIsLinkObject(path)       DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISLINK)
#define DeeHFS_Utf8IsDrive(path)                  DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define DeeHFS_WideIsDrive(path)                  DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISDRIVE)
#define DeeHFS_Utf8IsDriveObject(path)      DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISDRIVE)
#define DeeHFS_WideIsDriveObject(path)      DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISDRIVE)
#define DeeHFS_Utf8IsMount(path)                  DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define DeeHFS_WideIsMount(path)                  DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISMOUNT)
#define DeeHFS_Utf8IsMountObject(path)      DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISMOUNT)
#define DeeHFS_WideIsMountObject(path)      DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISMOUNT)
#define DeeHFS_Utf8IsHidden(path)                 DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define DeeHFS_WideIsHidden(path)                 DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISHIDDEN)
#define DeeHFS_Utf8IsHiddenObject(path)     DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISHIDDEN)
#define DeeHFS_WideIsHiddenObject(path)     DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISHIDDEN)
#define DeeHFS_Utf8IsExecutable(path)             DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define DeeHFS_WideIsExecutable(path)             DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define DeeHFS_Utf8IsExecutableObject(path) DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define DeeHFS_WideIsExecutableObject(path) DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISEXECUTABLE)
#define DeeHFS_Utf8IsCharDev(path)                DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define DeeHFS_WideIsCharDev(path)                DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISCHARDEV)
#define DeeHFS_Utf8IsCharDevObject(path)    DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISCHARDEV)
#define DeeHFS_WideIsCharDevObject(path)    DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISCHARDEV)
#define DeeHFS_Utf8IsBlockDev(path)               DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define DeeHFS_WideIsBlockDev(path)               DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define DeeHFS_Utf8IsBlockDevObject(path)   DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define DeeHFS_WideIsBlockDevObject(path)   DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISBLOCKDEV)
#define DeeHFS_Utf8IsFiFo(path)                   DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define DeeHFS_WideIsFiFo(path)                   DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISFIFO)
#define DeeHFS_Utf8IsFiFoObject(path)       DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISFIFO)
#define DeeHFS_WideIsFiFoObject(path)       DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISFIFO)
#define DeeHFS_Utf8IsSocket(path)                 DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define DeeHFS_WideIsSocket(path)                 DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISSOCKET)
#define DeeHFS_Utf8IsSocketObject(path)     DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISSOCKET)
#define DeeHFS_WideIsSocketObject(path)     DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISSOCKET)
#define DeeHFS_Utf8IsAbs(path)                    DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_ISABS)
#define DeeHFS_WideIsAbs(path)                    DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_ISABS)
#define DeeHFS_Utf8IsAbsObject(path)        DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_ISABS)
#define DeeHFS_WideIsAbsObject(path)        DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_ISABS)
#define DeeHFS_Utf8Exists(path)                   DeeHFS_Utf8HasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define DeeHFS_WideExists(path)                   DeeHFS_WideHasProperty(path,DEE_FILEPROPERTY_EXISTS)
#define DeeHFS_Utf8ExistsObject(path)       DeeHFS_Utf8HasPropertyObject(path,DEE_FILEPROPERTY_EXISTS)
#define DeeHFS_WideExistsObject(path)       DeeHFS_WideHasPropertyObject(path,DEE_FILEPROPERTY_EXISTS)


#define DeeHFS_GetCwd                DeeHFS_Utf8GetCwd
#define DeeHFS_Chdir                 DeeHFS_Utf8Chdir
#define DeeHFS_ChdirObject           DeeHFS_Utf8ChdirObject
#define DeeHFS_GetHome               DeeHFS_Utf8GetHome
#define DeeHFS_GetUserHome           DeeHFS_Utf8GetUserHome
#define DeeHFS_GetUserHomeObject     DeeHFS_Utf8GetUserHomeObject
#define DeeHFS_GetTmp                DeeHFS_Utf8GetTmp
#define DeeHFS_EnumEnv               DeeHFS_Utf8EnumEnv
#define DeeHFS_HasEnv                DeeHFS_Utf8HasEnv
#define DeeHFS_HasEnvObject          DeeHFS_Utf8HasEnvObject
#define DeeHFS_GetEnv                DeeHFS_Utf8GetEnv
#define DeeHFS_GetEnvObject          DeeHFS_Utf8GetEnvObject
#define DeeHFS_TryGetEnv             DeeHFS_Utf8TryGetEnv
#define DeeHFS_TryGetEnvObject       DeeHFS_Utf8TryGetEnvObject
#define DeeHFS_DelEnv                DeeHFS_Utf8DelEnv
#define DeeHFS_DelEnvObject          DeeHFS_Utf8DelEnvObject
#define DeeHFS_SetEnv                DeeHFS_Utf8SetEnv
#define DeeHFS_SetEnvObject          DeeHFS_Utf8SetEnvObject
#define DeeHFS_GetTimes              DeeHFS_Utf8GetTimes
#define DeeHFS_GetTimesObject        DeeHFS_Utf8GetTimesObject
#define DeeHFS_SetTimes              DeeHFS_Utf8SetTimes
#define DeeHFS_SetTimesObject        DeeHFS_Utf8SetTimesObject
#define DeeHFS_GetMod                DeeHFS_Utf8GetMod
#define DeeHFS_GetModObject          DeeHFS_Utf8GetModObject
#define DeeHFS_Chmod                 DeeHFS_Utf8Chmod
#define DeeHFS_ChmodObject           DeeHFS_Utf8ChmodObject
#define DeeHFS_GetOwn                DeeHFS_Utf8GetOwn
#define DeeHFS_GetOwnObject          DeeHFS_Utf8GetOwnObject
#define DeeHFS_Chown                 DeeHFS_Utf8Chown
#define DeeHFS_ChownObject           DeeHFS_Utf8ChownObject
#define DeeHFS_IsFile                DeeHFS_Utf8IsFile
#define DeeHFS_IsFileObject          DeeHFS_Utf8IsFileObject
#define DeeHFS_IsDir                 DeeHFS_Utf8IsDir
#define DeeHFS_IsDirObject           DeeHFS_Utf8IsDirObject
#define DeeHFS_IsLink                DeeHFS_Utf8IsLink
#define DeeHFS_IsLinkObject          DeeHFS_Utf8IsLinkObject
#define DeeHFS_IsDrive               DeeHFS_Utf8IsDrive
#define DeeHFS_IsDriveObject         DeeHFS_Utf8IsDriveObject
#define DeeHFS_IsMount               DeeHFS_Utf8IsMount
#define DeeHFS_IsMountObject         DeeHFS_Utf8IsMountObject
#define DeeHFS_IsHidden              DeeHFS_Utf8IsHidden
#define DeeHFS_IsHiddenObject        DeeHFS_Utf8IsHiddenObject
#define DeeHFS_IsExecutable          DeeHFS_Utf8IsExecutable
#define DeeHFS_IsExecutableObject    DeeHFS_Utf8IsExecutableObject
#define DeeHFS_IsCharDev             DeeHFS_Utf8IsCharDev
#define DeeHFS_IsCharDevObject       DeeHFS_Utf8IsCharDevObject
#define DeeHFS_IsBlockDev            DeeHFS_Utf8IsBlockDev
#define DeeHFS_IsBlockDevObject      DeeHFS_Utf8IsBlockDevObject
#define DeeHFS_IsFiFo                DeeHFS_Utf8IsFiFo
#define DeeHFS_IsFiFoObject          DeeHFS_Utf8IsFiFoObject
#define DeeHFS_IsSocket              DeeHFS_Utf8IsSocket
#define DeeHFS_IsSocketObject        DeeHFS_Utf8IsSocketObject
#define DeeHFS_IsAbs                 DeeHFS_Utf8IsAbs
#define DeeHFS_IsAbsObject           DeeHFS_Utf8IsAbsObject
#define DeeHFS_Exists                DeeHFS_Utf8Exists
#define DeeHFS_ExistsObject          DeeHFS_Utf8ExistsObject
#define DeeHFS_Remove                DeeHFS_Utf8Remove
#define DeeHFS_RemoveObject          DeeHFS_Utf8RemoveObject
#define DeeHFS_Unlink                DeeHFS_Utf8Unlink
#define DeeHFS_UnlinkObject          DeeHFS_Utf8UnlinkObject
#define DeeHFS_RmDir                 DeeHFS_Utf8RmDir
#define DeeHFS_RmDirObject           DeeHFS_Utf8RmDirObject
#define DeeHFS_MkDir                 DeeHFS_Utf8MkDir
#define DeeHFS_MkDirObject           DeeHFS_Utf8MkDirObject
#define DeeHFS_Copy                  DeeHFS_Utf8Copy
#define DeeHFS_CopyObject            DeeHFS_Utf8CopyObject
#define DeeHFS_Move                  DeeHFS_Utf8Move
#define DeeHFS_MoveObject            DeeHFS_Utf8MoveObject
#define DeeHFS_Link                  DeeHFS_Utf8Link
#define DeeHFS_LinkObject            DeeHFS_Utf8LinkObject
#define DeeHFS_Opendir               DeeHFS_Utf8Opendir
#define DeeHFS_OpendirObject         DeeHFS_Utf8OpendirObject
#define DeeHFS_Readlink              DeeHFS_Utf8Readlink
#define DeeHFS_ReadlinkObject        DeeHFS_Utf8ReadlinkObject


DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_HOOKS_H */
