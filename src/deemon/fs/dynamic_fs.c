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
#ifndef GUARD_DEEMON_FS_DYNAMIC_FS_C
#define GUARD_DEEMON_FS_DYNAMIC_FS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/file.h>
#include <deemon/fs_api.h>
#include <deemon/fs/expand_fs.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>

DEE_DECL_BEGIN

#ifdef DEE_FSAPIMODE_ENABLEVFS
#define DEE_FSAPIMODE_MASK (DEE_FSAPIMODE_EXPANDVARS|DEE_FSAPIMODE_ENABLEVFS)
#else
#define DEE_FSAPIMODE_MASK  DEE_FSAPIMODE_EXPANDVARS
#endif


//////////////////////////////////////////////////////////////////////////
// Dynamic FS API

DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeFSApi {
 PDEEFS_UTF8GETCWD                   fa_utf8getcwd;
 PDEEFS_WIDEGETCWD                   fa_widegetcwd;
 PDEEFS_UTF8CHDIR                    fa_utf8chdir;
 PDEEFS_WIDECHDIR                    fa_widechdir;
 PDEEFS_UTF8CHDIROBJECT              fa_utf8chdirobject;
 PDEEFS_WIDECHDIROBJECT              fa_widechdirobject;
 PDEEFS_UTF8CHDIR                   _fa_utf8chdir;
 PDEEFS_WIDECHDIR                   _fa_widechdir;
 PDEEFS_UTF8CHDIROBJECT             _fa_utf8chdirobject;
 PDEEFS_WIDECHDIROBJECT             _fa_widechdirobject;
 PDEEFS_UTF8GETHOME                  fa_utf8gethome;
 PDEEFS_WIDEGETHOME                  fa_widegethome;
 PDEEFS_UTF8GETUSERHOME              fa_utf8getuserhome;
 PDEEFS_WIDEGETUSERHOME              fa_widegetuserhome;
 PDEEFS_UTF8GETUSERHOMEOBJECT        fa_utf8getuserhomeobject;
 PDEEFS_WIDEGETUSERHOMEOBJECT        fa_widegetuserhomeobject;
 PDEEFS_UTF8GETTMP                   fa_utf8gettmp;
 PDEEFS_WIDEGETTMP                   fa_widegettmp;
 PDEEFS_UTF8ENUMENV                  fa_utf8enumenv;
 PDEEFS_WIDEENUMENV                  fa_wideenumenv;
 PDEEFS_UTF8GETENV                   fa_utf8getenv;
 PDEEFS_WIDEGETENV                   fa_widegetenv;
 PDEEFS_UTF8GETENVOBJECT             fa_utf8getenvobject;
 PDEEFS_WIDEGETENVOBJECT             fa_widegetenvobject;
 PDEEFS_UTF8HASENV                   fa_utf8hasenv;
 PDEEFS_WIDEHASENV                   fa_widehasenv;
 PDEEFS_UTF8HASENVOBJECT             fa_utf8hasenvobject;
 PDEEFS_WIDEHASENVOBJECT             fa_widehasenvobject;
 PDEEFS_UTF8DELENV                   fa_utf8delenv;
 PDEEFS_WIDEDELENV                   fa_widedelenv;
 PDEEFS_UTF8DELENVOBJECT             fa_utf8delenvobject;
 PDEEFS_WIDEDELENVOBJECT             fa_widedelenvobject;
 PDEEFS_UTF8SETENV                   fa_utf8setenv;
 PDEEFS_WIDESETENV                   fa_widesetenv;
 PDEEFS_UTF8SETENVOBJECT             fa_utf8setenvobject;
 PDEEFS_WIDESETENVOBJECT             fa_widesetenvobject;
 PDEEFS_UTF8TRYGETENV                fa_utf8trygetenv;
 PDEEFS_WIDETRYGETENV                fa_widetrygetenv;
 PDEEFS_UTF8TRYGETENVOBJECT          fa_utf8trygetenvobject;
 PDEEFS_WIDETRYGETENVOBJECT          fa_widetrygetenvobject;
 PDEEFS_UTF8GETTIMES                 fa_utf8gettimes;
 PDEEFS_WIDEGETTIMES                 fa_widegettimes;
 PDEEFS_UTF8GETTIMESOBJECT           fa_utf8gettimesobject;
 PDEEFS_WIDEGETTIMESOBJECT           fa_widegettimesobject;
 PDEEFS_UTF8GETTIMES                _fa_utf8gettimes;
 PDEEFS_WIDEGETTIMES                _fa_widegettimes;
 PDEEFS_UTF8GETTIMESOBJECT          _fa_utf8gettimesobject;
 PDEEFS_WIDEGETTIMESOBJECT          _fa_widegettimesobject;
 PDEEFS_UTF8SETTIMES                 fa_utf8settimes;
 PDEEFS_WIDESETTIMES                 fa_widesettimes;
 PDEEFS_UTF8SETTIMESOBJECT           fa_utf8settimesobject;
 PDEEFS_WIDESETTIMESOBJECT           fa_widesettimesobject;
 PDEEFS_UTF8SETTIMES                _fa_utf8settimes;
 PDEEFS_WIDESETTIMES                _fa_widesettimes;
 PDEEFS_UTF8SETTIMESOBJECT          _fa_utf8settimesobject;
 PDEEFS_WIDESETTIMESOBJECT          _fa_widesettimesobject;
 PDEEFS_UTF8HASPROPERTY              fa_utf8hasproperty;
 PDEEFS_WIDEHASPROPERTY              fa_widehasproperty;
 PDEEFS_UTF8HASPROPERTYOBJECT        fa_utf8haspropertyobject;
 PDEEFS_WIDEHASPROPERTYOBJECT        fa_widehaspropertyobject;
 PDEEFS_UTF8HASPROPERTY             _fa_utf8hasproperty;
 PDEEFS_WIDEHASPROPERTY             _fa_widehasproperty;
 PDEEFS_UTF8HASPROPERTYOBJECT       _fa_utf8haspropertyobject;
 PDEEFS_WIDEHASPROPERTYOBJECT       _fa_widehaspropertyobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8remove;
 PDEEFS_WIDEPATHFUN                  fa_wideremove;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8removeobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_wideremoveobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8remove;
 PDEEFS_WIDEPATHFUN                 _fa_wideremove;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8removeobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_wideremoveobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8unlink;
 PDEEFS_WIDEPATHFUN                  fa_wideunlink;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8unlinkobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_wideunlinkobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8unlink;
 PDEEFS_WIDEPATHFUN                 _fa_wideunlink;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8unlinkobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_wideunlinkobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8rmdir;
 PDEEFS_WIDEPATHFUN                  fa_widermdir;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8rmdirobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_widermdirobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8rmdir;
 PDEEFS_WIDEPATHFUN                 _fa_widermdir;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8rmdirobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_widermdirobject;
 PDEEFS_UTF8MKDIR                    fa_utf8mkdir;
 PDEEFS_WIDEMKDIR                    fa_widemkdir;
 PDEEFS_UTF8MKDIROBJECT              fa_utf8mkdirobject;
 PDEEFS_WIDEMKDIROBJECT              fa_widemkdirobject;
 PDEEFS_UTF8MKDIR                   _fa_utf8mkdir;
 PDEEFS_WIDEMKDIR                   _fa_widemkdir;
 PDEEFS_UTF8MKDIROBJECT             _fa_utf8mkdirobject;
 PDEEFS_WIDEMKDIROBJECT             _fa_widemkdirobject;
 PDEEFS_UTF8GETMOD                   fa_utf8getmod;
 PDEEFS_WIDEGETMOD                   fa_widegetmod;
 PDEEFS_UTF8GETMODOBJECT             fa_utf8getmodobject;
 PDEEFS_WIDEGETMODOBJECT             fa_widegetmodobject;
 PDEEFS_UTF8GETMOD                  _fa_utf8getmod;
 PDEEFS_WIDEGETMOD                  _fa_widegetmod;
 PDEEFS_UTF8GETMODOBJECT            _fa_utf8getmodobject;
 PDEEFS_WIDEGETMODOBJECT            _fa_widegetmodobject;
 PDEEFS_UTF8CHMOD                    fa_utf8chmod;
 PDEEFS_WIDECHMOD                    fa_widechmod;
 PDEEFS_UTF8CHMODOBJECT              fa_utf8chmodobject;
 PDEEFS_WIDECHMODOBJECT              fa_widechmodobject;
 PDEEFS_UTF8CHMOD                   _fa_utf8chmod;
 PDEEFS_WIDECHMOD                   _fa_widechmod;
 PDEEFS_UTF8CHMODOBJECT             _fa_utf8chmodobject;
 PDEEFS_WIDECHMODOBJECT             _fa_widechmodobject;
 PDEEFS_UTF8GETOWN                   fa_utf8getown;
 PDEEFS_WIDEGETOWN                   fa_widegetown;
 PDEEFS_UTF8GETOWNOBJECT             fa_utf8getownobject;
 PDEEFS_WIDEGETOWNOBJECT             fa_widegetownobject;
 PDEEFS_UTF8GETOWN                  _fa_utf8getown;
 PDEEFS_WIDEGETOWN                  _fa_widegetown;
 PDEEFS_UTF8GETOWNOBJECT            _fa_utf8getownobject;
 PDEEFS_WIDEGETOWNOBJECT            _fa_widegetownobject;
 PDEEFS_UTF8CHOWN                    fa_utf8chown;
 PDEEFS_WIDECHOWN                    fa_widechown;
 PDEEFS_UTF8CHOWNOBJECT              fa_utf8chownobject;
 PDEEFS_WIDECHOWNOBJECT              fa_widechownobject;
 PDEEFS_UTF8CHOWN                   _fa_utf8chown;
 PDEEFS_WIDECHOWN                   _fa_widechown;
 PDEEFS_UTF8CHOWNOBJECT             _fa_utf8chownobject;
 PDEEFS_WIDECHOWNOBJECT             _fa_widechownobject;
 PDEEFS_UTF8COPY                     fa_utf8copy;
 PDEEFS_WIDECOPY                     fa_widecopy;
 PDEEFS_UTF8COPYOBJECT               fa_utf8copyobject;
 PDEEFS_WIDECOPYOBJECT               fa_widecopyobject;
 PDEEFS_UTF8COPY                    _fa_utf8copy;
 PDEEFS_WIDECOPY                    _fa_widecopy;
 PDEEFS_UTF8COPYOBJECT              _fa_utf8copyobject;
 PDEEFS_WIDECOPYOBJECT              _fa_widecopyobject;
 PDEEFS_UTF8COPY                     fa_utf8move;
 PDEEFS_WIDECOPY                     fa_widemove;
 PDEEFS_UTF8COPYOBJECT               fa_utf8moveobject;
 PDEEFS_WIDECOPYOBJECT               fa_widemoveobject;
 PDEEFS_UTF8COPY                    _fa_utf8move;
 PDEEFS_WIDECOPY                    _fa_widemove;
 PDEEFS_UTF8COPYOBJECT              _fa_utf8moveobject;
 PDEEFS_WIDECOPYOBJECT              _fa_widemoveobject;
 PDEEFS_UTF8LINK                     fa_utf8link;
 PDEEFS_WIDELINK                     fa_widelink;
 PDEEFS_UTF8LINKOBJECT               fa_utf8linkobject;
 PDEEFS_WIDELINKOBJECT               fa_widelinkobject;
 PDEEFS_UTF8LINK                    _fa_utf8link;
 PDEEFS_WIDELINK                    _fa_widelink;
 PDEEFS_UTF8LINKOBJECT              _fa_utf8linkobject;
 PDEEFS_WIDELINKOBJECT              _fa_widelinkobject;
 PDEEFS_UTF8OPENDIR                  fa_utf8opendir;
 PDEEFS_WIDEOPENDIR                  fa_wideopendir;
 PDEEFS_UTF8OPENDIROBJECT            fa_utf8opendirobject;
 PDEEFS_WIDEOPENDIROBJECT            fa_wideopendirobject;
 PDEEFS_UTF8OPENDIR                 _fa_utf8opendir;
 PDEEFS_WIDEOPENDIR                 _fa_wideopendir;
 PDEEFS_UTF8OPENDIROBJECT           _fa_utf8opendirobject;
 PDEEFS_WIDEOPENDIROBJECT           _fa_wideopendirobject;
 PDEEFILE_UTF8OPENEX                 fa_utf8open;
 PDEEFILE_WIDEOPENEX                 fa_wideopen;
 PDEEFILE_UTF8OPENEXOBJECT           fa_utf8openobject;
 PDEEFILE_WIDEOPENEXOBJECT           fa_wideopenobject;
 PDEEFILE_UTF8OPENEX                _fa_utf8open;
 PDEEFILE_WIDEOPENEX                _fa_wideopen;
 PDEEFILE_UTF8OPENEXOBJECT          _fa_utf8openobject;
 PDEEFILE_WIDEOPENEXOBJECT          _fa_wideopenobject;
 PDEEFS_UTF8READLINK                 fa_utf8readlink;
 PDEEFS_WIDEREADLINK                 fa_widereadlink;
 PDEEFS_UTF8READLINKOBJECT           fa_utf8readlinkobject;
 PDEEFS_WIDEREADLINKOBJECT           fa_widereadlinkobject;
 PDEEFS_UTF8READLINK                _fa_utf8readlink;
 PDEEFS_WIDEREADLINK                _fa_widereadlink;
 PDEEFS_UTF8READLINKOBJECT          _fa_utf8readlinkobject;
 PDEEFS_WIDEREADLINKOBJECT          _fa_widereadlinkobject;
};

enum{s=sizeof(struct DeeFSApi)/sizeof(void *)};
DEE_COMPILER_STRUCTURE_PACKED_END

#if DEE_CONFIG_RUNTIME_HAVE_VFS2 && ((\
    DEE_FSAPIMODE_DEFAULT&DEE_FSAPIMODE_ENABLEVFS)!=0)
#define _DeeDFS(x) &DeeVFS_##x
#else
#define _DeeDFS(x) &DeeNFS_##x
#endif

#if (DEE_FSAPIMODE_DEFAULT&DEE_FSAPIMODE_EXPANDVARS)!=0
#define DeeDFS(x)  &DeeXFS_##x
#else
#define DeeDFS     _DeeDFS
#endif


static struct DeeAtomicMutex DeeFSApi_ActiveLock = DeeAtomicMutex_INIT();
static struct DeeFSApi DeeFSApi_Active = {
 _DeeDFS(Utf8GetCwd),                   /*<  fa_utf8getcwd. */
 _DeeDFS(WideGetCwd),                   /*<  fa_widegetcwd. */
  DeeDFS(Utf8Chdir),                    /*<  fa_utf8chdir. */
  DeeDFS(WideChdir),                    /*<  fa_widechdir. */
  DeeDFS(Utf8ChdirObject),              /*<  fa_utf8chdirobject. */
  DeeDFS(WideChdirObject),              /*<  fa_widechdirobject. */
 _DeeDFS(Utf8Chdir),                    /*< _fa_utf8chdir. */
 _DeeDFS(WideChdir),                    /*< _fa_widechdir. */
 _DeeDFS(Utf8ChdirObject),              /*< _fa_utf8chdirobject. */
 _DeeDFS(WideChdirObject),              /*< _fa_widechdirobject. */
 &DeeNFS_Utf8GetHome,                   /*<  fa_utf8gethome. */
 &DeeNFS_WideGetHome,                   /*<  fa_widegethome. */
 &DeeNFS_Utf8GetUserHome,               /*<  fa_utf8getuserhome. */
 &DeeNFS_WideGetUserHome,               /*<  fa_widegetuserhome. */
 &DeeNFS_Utf8GetUserHomeObject,         /*<  fa_utf8getuserhomeobject. */
 &DeeNFS_WideGetUserHomeObject,         /*<  fa_widegetuserhomeobject. */
 &DeeNFS_Utf8GetTmp,                    /*<  fa_utf8gettmp. */
 &DeeNFS_WideGetTmp,                    /*<  fa_widegettmp. */
 &DeeNFS_Utf8EnumEnv,                   /*<  fa_utf8enumenv. */
 &DeeNFS_WideEnumEnv,                   /*<  fa_wideenumenv. */
 &DeeNFS_Utf8GetEnv,                    /*<  fa_utf8getenv. */
 &DeeNFS_WideGetEnv,                    /*<  fa_widegetenv. */
 &DeeNFS_Utf8GetEnvObject,              /*<  fa_utf8getenvobject. */
 &DeeNFS_WideGetEnvObject,              /*<  fa_widegetenvobject. */
 &DeeNFS_Utf8HasEnv,                    /*<  fa_utf8hasenv. */
 &DeeNFS_WideHasEnv,                    /*<  fa_widehasenv. */
 &DeeNFS_Utf8HasEnvObject,              /*<  fa_utf8hasenvobject. */
 &DeeNFS_WideHasEnvObject,              /*<  fa_widehasenvobject. */
 &DeeNFS_Utf8DelEnv,                    /*<  fa_utf8delenv. */
 &DeeNFS_WideDelEnv,                    /*<  fa_widedelenv. */
 &DeeNFS_Utf8DelEnvObject,              /*<  fa_utf8delenvobject. */
 &DeeNFS_WideDelEnvObject,              /*<  fa_widedelenvobject. */
 &DeeNFS_Utf8SetEnv,                    /*<  fa_utf8setenv. */
 &DeeNFS_WideSetEnv,                    /*<  fa_widesetenv. */
 &DeeNFS_Utf8SetEnvObject,              /*<  fa_utf8setenvobject. */
 &DeeNFS_WideSetEnvObject,              /*<  fa_widesetenvobject. */
 &DeeNFS_Utf8TryGetEnv,                 /*<  fa_utf8trygetenv. */
 &DeeNFS_WideTryGetEnv,                 /*<  fa_widetrygetenv. */
 &DeeNFS_Utf8TryGetEnvObject,           /*<  fa_utf8trygetenvobject. */
 &DeeNFS_WideTryGetEnvObject,           /*<  fa_widetrygetenvobject. */
  DeeDFS(Utf8GetTimes),                 /*<  fa_utf8gettimes. */
  DeeDFS(WideGetTimes),                 /*<  fa_widegettimes. */
  DeeDFS(Utf8GetTimesObject),           /*<  fa_utf8gettimesobject. */
  DeeDFS(WideGetTimesObject),           /*<  fa_widegettimesobject. */
 _DeeDFS(Utf8GetTimes),                 /*< _fa_utf8gettimes. */
 _DeeDFS(WideGetTimes),                 /*< _fa_widegettimes. */
 _DeeDFS(Utf8GetTimesObject),           /*< _fa_utf8gettimesobject. */
 _DeeDFS(WideGetTimesObject),           /*< _fa_widegettimesobject. */
  DeeDFS(Utf8SetTimes),                 /*<  fa_utf8settimes. */
  DeeDFS(WideSetTimes),                 /*<  fa_widesettimes. */
  DeeDFS(Utf8SetTimesObject),           /*<  fa_utf8settimesobject. */
  DeeDFS(WideSetTimesObject),           /*<  fa_widesettimesobject. */
 _DeeDFS(Utf8SetTimes),                 /*< _fa_utf8settimes. */
 _DeeDFS(WideSetTimes),                 /*< _fa_widesettimes. */
 _DeeDFS(Utf8SetTimesObject),           /*< _fa_utf8settimesobject. */
 _DeeDFS(WideSetTimesObject),           /*< _fa_widesettimesobject. */
  DeeDFS(Utf8HasProperty),              /*<  fa_utf8hasproperty. */
  DeeDFS(WideHasProperty),              /*<  fa_widehasproperty. */
  DeeDFS(Utf8HasPropertyObject),        /*<  fa_utf8haspropertyobject. */
  DeeDFS(WideHasPropertyObject),        /*<  fa_widehaspropertyobject. */
 _DeeDFS(Utf8HasProperty),              /*< _fa_utf8hasproperty. */
 _DeeDFS(WideHasProperty),              /*< _fa_widehasproperty. */
 _DeeDFS(Utf8HasPropertyObject),        /*< _fa_utf8haspropertyobject. */
 _DeeDFS(WideHasPropertyObject),        /*< _fa_widehaspropertyobject. */
  DeeDFS(Utf8Remove),                   /*<  fa_utf8remove. */
  DeeDFS(WideRemove),                   /*<  fa_wideremove. */
  DeeDFS(Utf8RemoveObject),             /*<  fa_utf8removeobject. */
  DeeDFS(WideRemoveObject),             /*<  fa_wideremoveobject. */
 _DeeDFS(Utf8Remove),                   /*< _fa_utf8remove. */
 _DeeDFS(WideRemove),                   /*< _fa_wideremove. */
 _DeeDFS(Utf8RemoveObject),             /*< _fa_utf8removeobject. */
 _DeeDFS(WideRemoveObject),             /*< _fa_wideremoveobject. */
  DeeDFS(Utf8Unlink),                   /*<  fa_utf8unlink. */
  DeeDFS(WideUnlink),                   /*<  fa_wideunlink. */
  DeeDFS(Utf8UnlinkObject),             /*<  fa_utf8unlinkobject. */
  DeeDFS(WideUnlinkObject),             /*<  fa_wideunlinkobject. */
 _DeeDFS(Utf8Unlink),                   /*< _fa_utf8unlink. */
 _DeeDFS(WideUnlink),                   /*< _fa_wideunlink. */
 _DeeDFS(Utf8UnlinkObject),             /*< _fa_utf8unlinkobject. */
 _DeeDFS(WideUnlinkObject),             /*< _fa_wideunlinkobject. */
  DeeDFS(Utf8RmDir),                    /*<  fa_utf8rmdir. */
  DeeDFS(WideRmDir),                    /*<  fa_widermdir. */
  DeeDFS(Utf8RmDirObject),              /*<  fa_utf8rmdirobject. */
  DeeDFS(WideRmDirObject),              /*<  fa_widermdirobject. */
 _DeeDFS(Utf8RmDir),                    /*< _fa_utf8rmdir. */
 _DeeDFS(WideRmDir),                    /*< _fa_widermdir. */
 _DeeDFS(Utf8RmDirObject),              /*< _fa_utf8rmdirobject. */
 _DeeDFS(WideRmDirObject),              /*< _fa_widermdirobject. */
  DeeDFS(Utf8MkDir),                    /*<  fa_utf8mkdir. */
  DeeDFS(WideMkDir),                    /*<  fa_widemkdir. */
  DeeDFS(Utf8MkDirObject),              /*<  fa_utf8mkdirobject. */
  DeeDFS(WideMkDirObject),              /*<  fa_widemkdirobject. */
 _DeeDFS(Utf8MkDir),                    /*< _fa_utf8mkdir. */
 _DeeDFS(WideMkDir),                    /*< _fa_widemkdir. */
 _DeeDFS(Utf8MkDirObject),              /*< _fa_utf8mkdirobject. */
 _DeeDFS(WideMkDirObject),              /*< _fa_widemkdirobject. */
  DeeDFS(Utf8GetMod),                   /*<  fa_utf8getmod. */
  DeeDFS(WideGetMod),                   /*<  fa_widegetmod. */
  DeeDFS(Utf8GetModObject),             /*<  fa_utf8getmodobject. */
  DeeDFS(WideGetModObject),             /*<  fa_widegetmodobject. */
 _DeeDFS(Utf8GetMod),                   /*< _fa_utf8getmod. */
 _DeeDFS(WideGetMod),                   /*< _fa_widegetmod. */
 _DeeDFS(Utf8GetModObject),             /*< _fa_utf8getmodobject. */
 _DeeDFS(WideGetModObject),             /*< _fa_widegetmodobject. */
  DeeDFS(Utf8Chmod),                    /*<  fa_utf8chmod. */
  DeeDFS(WideChmod),                    /*<  fa_widechmod. */
  DeeDFS(Utf8ChmodObject),              /*<  fa_utf8chmodobject. */
  DeeDFS(WideChmodObject),              /*<  fa_widechmodobject. */
 _DeeDFS(Utf8Chmod),                    /*< _fa_utf8chmod. */
 _DeeDFS(WideChmod),                    /*< _fa_widechmod. */
 _DeeDFS(Utf8ChmodObject),              /*< _fa_utf8chmodobject. */
 _DeeDFS(WideChmodObject),              /*< _fa_widechmodobject. */
  DeeDFS(Utf8GetOwn),                   /*<  fa_utf8getown. */
  DeeDFS(WideGetOwn),                   /*<  fa_widegetown. */
  DeeDFS(Utf8GetOwnObject),             /*<  fa_utf8getownobject. */
  DeeDFS(WideGetOwnObject),             /*<  fa_widegetownobject. */
 _DeeDFS(Utf8GetOwn),                   /*< _fa_utf8getown. */
 _DeeDFS(WideGetOwn),                   /*< _fa_widegetown. */
 _DeeDFS(Utf8GetOwnObject),             /*< _fa_utf8getownobject. */
 _DeeDFS(WideGetOwnObject),             /*< _fa_widegetownobject. */
  DeeDFS(Utf8Chown),                    /*<  fa_utf8chown. */
  DeeDFS(WideChown),                    /*<  fa_widechown. */
  DeeDFS(Utf8ChownObject),              /*<  fa_utf8chownobject. */
  DeeDFS(WideChownObject),              /*<  fa_widechownobject. */
 _DeeDFS(Utf8Chown),                    /*< _fa_utf8chown. */
 _DeeDFS(WideChown),                    /*< _fa_widechown. */
 _DeeDFS(Utf8ChownObject),              /*< _fa_utf8chownobject. */
 _DeeDFS(WideChownObject),              /*< _fa_widechownobject. */
  DeeDFS(Utf8Copy),                     /*<  fa_utf8copy. */
  DeeDFS(WideCopy),                     /*<  fa_widecopy. */
  DeeDFS(Utf8CopyObject),               /*<  fa_utf8copyobject. */
  DeeDFS(WideCopyObject),               /*<  fa_widecopyobject. */
 _DeeDFS(Utf8Copy),                     /*< _fa_utf8copy. */
 _DeeDFS(WideCopy),                     /*< _fa_widecopy. */
 _DeeDFS(Utf8CopyObject),               /*< _fa_utf8copyobject. */
 _DeeDFS(WideCopyObject),               /*< _fa_widecopyobject. */
  DeeDFS(Utf8Move),                     /*<  fa_utf8move. */
  DeeDFS(WideMove),                     /*<  fa_widemove. */
  DeeDFS(Utf8MoveObject),               /*<  fa_utf8moveobject. */
  DeeDFS(WideMoveObject),               /*<  fa_widemoveobject. */
 _DeeDFS(Utf8Move),                     /*< _fa_utf8move. */
 _DeeDFS(WideMove),                     /*< _fa_widemove. */
 _DeeDFS(Utf8MoveObject),               /*< _fa_utf8moveobject. */
 _DeeDFS(WideMoveObject),               /*< _fa_widemoveobject. */
  DeeDFS(Utf8Link),                     /*<  fa_utf8link. */
  DeeDFS(WideLink),                     /*<  fa_widelink. */
  DeeDFS(Utf8LinkObject),               /*<  fa_utf8linkobject. */
  DeeDFS(WideLinkObject),               /*<  fa_widelinkobject. */
 _DeeDFS(Utf8Link),                     /*< _fa_utf8link. */
 _DeeDFS(WideLink),                     /*< _fa_widelink. */
 _DeeDFS(Utf8LinkObject),               /*< _fa_utf8linkobject. */
 _DeeDFS(WideLinkObject),               /*< _fa_widelinkobject. */
  DeeDFS(Utf8Opendir),                  /*<  fa_utf8opendir. */
  DeeDFS(WideOpendir),                  /*<  fa_wideopendir. */
  DeeDFS(Utf8OpendirObject),            /*<  fa_utf8opendirobject. */
  DeeDFS(WideOpendirObject),            /*<  fa_wideopendirobject. */
 _DeeDFS(Utf8Opendir),                  /*< _fa_utf8opendir. */
 _DeeDFS(WideOpendir),                  /*< _fa_wideopendir. */
 _DeeDFS(Utf8OpendirObject),            /*< _fa_utf8opendirobject. */
 _DeeDFS(WideOpendirObject),            /*< _fa_wideopendirobject. */
  DeeDFS(Utf8Open),                     /*<  fa_utf8open. */
  DeeDFS(WideOpen),                     /*<  fa_wideopen. */
  DeeDFS(Utf8OpenObject),               /*<  fa_utf8openobject. */
  DeeDFS(WideOpenObject),               /*<  fa_wideopenobject. */
 _DeeDFS(Utf8Open),                     /*< _fa_utf8open. */
 _DeeDFS(WideOpen),                     /*< _fa_wideopen. */
 _DeeDFS(Utf8OpenObject),               /*< _fa_utf8openobject. */
 _DeeDFS(WideOpenObject),               /*< _fa_wideopenobject. */
  DeeDFS(Utf8Readlink),                 /*<  fa_utf8readlink. */
  DeeDFS(WideReadlink),                 /*<  fa_widereadlink. */
  DeeDFS(Utf8ReadlinkObject),           /*<  fa_utf8readlinkobject. */
  DeeDFS(WideReadlinkObject),           /*<  fa_widereadlinkobject. */
 _DeeDFS(Utf8Readlink),                 /*< _fa_utf8readlink. */
 _DeeDFS(WideReadlink),                 /*< _fa_widereadlink. */
 _DeeDFS(Utf8ReadlinkObject),           /*< _fa_utf8readlinkobject. */
 _DeeDFS(WideReadlinkObject),           /*< _fa_widereadlinkobject. */
};

DEE_A_RET_NEVER_NULL DeeFunPointer DeeFS_GetFunction_v102(DEE_A_IN Dee_size_t id) {
 DeeFunPointer result;
 DEE_ASSERTF(id < sizeof(struct DeeFSApi),"Invalid id: out of bounds");
 DEE_ASSERTF((id%sizeof(DeeFunPointer))==0,"Invalid id: Not pointer aligned");
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 result = *(DeeFunPointer *)(((Dee_uintptr_t)&DeeFSApi_Active)+id);
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}

DEE_A_RET_WUNUSED Dee_fsapimode_t DeeFS_GetAPIMode(void) {
 Dee_fsapimode_t result = DEE_FSAPIMODE_NONE;
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 if (DeeFSApi_Active._fa_utf8chdir != DeeFSApi_Active.fa_utf8chdir
     ) result |= DEE_FSAPIMODE_EXPANDVARS;
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
 if (DeeFSApi_Active.fa_utf8getcwd == &DeeVFS_Utf8GetCwd)
  result |= DEE_FSAPIMODE_ENABLEVFS;
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}

// Private exports:
//  - Never-expand versions of regular callbacks
//  - Callbacks not taking a path (aka. no-expand)
#define MAP_PRIVATE_FSAPI_EXPORTS(X)\
  X(fa_utf8getcwd,               Utf8GetCwd)\
  X(fa_widegetcwd,               WideGetCwd)\
  X(_fa_utf8chdir,               Utf8Chdir)\
  X(_fa_widechdir,               WideChdir)\
  X(_fa_utf8chdirobject,         Utf8ChdirObject)\
  X(_fa_widechdirobject,         WideChdirObject)\
  X(_fa_utf8gettimes,            Utf8GetTimes)\
  X(_fa_widegettimes,            WideGetTimes)\
  X(_fa_utf8gettimesobject,      Utf8GetTimesObject)\
  X(_fa_widegettimesobject,      WideGetTimesObject)\
  X(_fa_utf8settimes,            Utf8SetTimes)\
  X(_fa_widesettimes,            WideSetTimes)\
  X(_fa_utf8settimesobject,      Utf8SetTimesObject)\
  X(_fa_widesettimesobject,      WideSetTimesObject)\
  X(_fa_utf8hasproperty,         Utf8HasProperty)\
  X(_fa_widehasproperty,         WideHasProperty)\
  X(_fa_utf8haspropertyobject,   Utf8HasPropertyObject)\
  X(_fa_widehaspropertyobject,   WideHasPropertyObject)\
  X(_fa_utf8remove,              Utf8Remove)\
  X(_fa_wideremove,              WideRemove)\
  X(_fa_utf8removeobject,        Utf8RemoveObject)\
  X(_fa_wideremoveobject,        WideRemoveObject)\
  X(_fa_utf8unlink,              Utf8Unlink)\
  X(_fa_wideunlink,              WideUnlink)\
  X(_fa_utf8unlinkobject,        Utf8UnlinkObject)\
  X(_fa_wideunlinkobject,        WideUnlinkObject)\
  X(_fa_utf8rmdir,               Utf8RmDir)\
  X(_fa_widermdir,               WideRmDir)\
  X(_fa_utf8rmdirobject,         Utf8RmDirObject)\
  X(_fa_widermdirobject,         WideRmDirObject)\
  X(_fa_utf8mkdir,               Utf8MkDir)\
  X(_fa_widemkdir,               WideMkDir)\
  X(_fa_utf8mkdirobject,         Utf8MkDirObject)\
  X(_fa_widemkdirobject,         WideMkDirObject)\
  X(_fa_utf8getmod,              Utf8GetMod)\
  X(_fa_widegetmod,              WideGetMod)\
  X(_fa_utf8getmodobject,        Utf8GetModObject)\
  X(_fa_widegetmodobject,        WideGetModObject)\
  X(_fa_utf8chmod,               Utf8Chmod)\
  X(_fa_widechmod,               WideChmod)\
  X(_fa_utf8chmodobject,         Utf8ChmodObject)\
  X(_fa_widechmodobject,         WideChmodObject)\
  X(_fa_utf8getown,              Utf8GetOwn)\
  X(_fa_widegetown,              WideGetOwn)\
  X(_fa_utf8getownobject,        Utf8GetOwnObject)\
  X(_fa_widegetownobject,        WideGetOwnObject)\
  X(_fa_utf8chown,               Utf8Chown)\
  X(_fa_widechown,               WideChown)\
  X(_fa_utf8chownobject,         Utf8ChownObject)\
  X(_fa_widechownobject,         WideChownObject)\
  X(_fa_utf8copy,                Utf8Copy)\
  X(_fa_widecopy,                WideCopy)\
  X(_fa_utf8copyobject,          Utf8CopyObject)\
  X(_fa_widecopyobject,          WideCopyObject)\
  X(_fa_utf8move,                Utf8Move)\
  X(_fa_widemove,                WideMove)\
  X(_fa_utf8moveobject,          Utf8MoveObject)\
  X(_fa_widemoveobject,          WideMoveObject)\
  X(_fa_utf8link,                Utf8Link)\
  X(_fa_widelink,                WideLink)\
  X(_fa_utf8linkobject,          Utf8LinkObject)\
  X(_fa_widelinkobject,          WideLinkObject)\
  X(_fa_utf8opendir,             Utf8Opendir)\
  X(_fa_wideopendir,             WideOpendir)\
  X(_fa_utf8opendirobject,       Utf8OpendirObject)\
  X(_fa_wideopendirobject,       WideOpendirObject)\
  X(_fa_utf8open,                Utf8Open)\
  X(_fa_wideopen,                WideOpen)\
  X(_fa_utf8openobject,          Utf8OpenObject)\
  X(_fa_wideopenobject,          WideOpenObject)\
  X(_fa_utf8readlink,            Utf8Readlink)\
  X(_fa_widereadlink,            WideReadlink)\
  X(_fa_utf8readlinkobject,      Utf8ReadlinkObject)\
  X(_fa_widereadlinkobject,      WideReadlinkObject)\

// Public exports:
//  - Functions capable of expanding a given path
//    before passing it the associated with version.
// NOTE: These functions must feature private versions,
//       that will be alias-linked in case the
//       EXPANDPATH flag is disabled.
#define MAP_PUBLIC_FSAPI_EXPORTS(X)\
  X(fa_utf8chdir,               Utf8Chdir)\
  X(fa_widechdir,               WideChdir)\
  X(fa_utf8chdirobject,         Utf8ChdirObject)\
  X(fa_widechdirobject,         WideChdirObject)\
  X(fa_utf8gettimes,            Utf8GetTimes)\
  X(fa_widegettimes,            WideGetTimes)\
  X(fa_utf8gettimesobject,      Utf8GetTimesObject)\
  X(fa_widegettimesobject,      WideGetTimesObject)\
  X(fa_utf8settimes,            Utf8SetTimes)\
  X(fa_widesettimes,            WideSetTimes)\
  X(fa_utf8settimesobject,      Utf8SetTimesObject)\
  X(fa_widesettimesobject,      WideSetTimesObject)\
  X(fa_utf8hasproperty,         Utf8HasProperty)\
  X(fa_widehasproperty,         WideHasProperty)\
  X(fa_utf8haspropertyobject,   Utf8HasPropertyObject)\
  X(fa_widehaspropertyobject,   WideHasPropertyObject)\
  X(fa_utf8remove,              Utf8Remove)\
  X(fa_wideremove,              WideRemove)\
  X(fa_utf8removeobject,        Utf8RemoveObject)\
  X(fa_wideremoveobject,        WideRemoveObject)\
  X(fa_utf8unlink,              Utf8Unlink)\
  X(fa_wideunlink,              WideUnlink)\
  X(fa_utf8unlinkobject,        Utf8UnlinkObject)\
  X(fa_wideunlinkobject,        WideUnlinkObject)\
  X(fa_utf8rmdir,               Utf8RmDir)\
  X(fa_widermdir,               WideRmDir)\
  X(fa_utf8rmdirobject,         Utf8RmDirObject)\
  X(fa_widermdirobject,         WideRmDirObject)\
  X(fa_utf8mkdir,               Utf8MkDir)\
  X(fa_widemkdir,               WideMkDir)\
  X(fa_utf8mkdirobject,         Utf8MkDirObject)\
  X(fa_widemkdirobject,         WideMkDirObject)\
  X(fa_utf8getmod,              Utf8GetMod)\
  X(fa_widegetmod,              WideGetMod)\
  X(fa_utf8getmodobject,        Utf8GetModObject)\
  X(fa_widegetmodobject,        WideGetModObject)\
  X(fa_utf8chmod,               Utf8Chmod)\
  X(fa_widechmod,               WideChmod)\
  X(fa_utf8chmodobject,         Utf8ChmodObject)\
  X(fa_widechmodobject,         WideChmodObject)\
  X(fa_utf8getown,              Utf8GetOwn)\
  X(fa_widegetown,              WideGetOwn)\
  X(fa_utf8getownobject,        Utf8GetOwnObject)\
  X(fa_widegetownobject,        WideGetOwnObject)\
  X(fa_utf8chown,               Utf8Chown)\
  X(fa_widechown,               WideChown)\
  X(fa_utf8chownobject,         Utf8ChownObject)\
  X(fa_widechownobject,         WideChownObject)\
  X(fa_utf8copy,                Utf8Copy)\
  X(fa_widecopy,                WideCopy)\
  X(fa_utf8copyobject,          Utf8CopyObject)\
  X(fa_widecopyobject,          WideCopyObject)\
  X(fa_utf8move,                Utf8Move)\
  X(fa_widemove,                WideMove)\
  X(fa_utf8moveobject,          Utf8MoveObject)\
  X(fa_widemoveobject,          WideMoveObject)\
  X(fa_utf8link,                Utf8Link)\
  X(fa_widelink,                WideLink)\
  X(fa_utf8linkobject,          Utf8LinkObject)\
  X(fa_widelinkobject,          WideLinkObject)\
  X(fa_utf8opendir,             Utf8Opendir)\
  X(fa_wideopendir,             WideOpendir)\
  X(fa_utf8opendirobject,       Utf8OpendirObject)\
  X(fa_wideopendirobject,       WideOpendirObject)\
  X(fa_utf8open,                Utf8Open)\
  X(fa_wideopen,                WideOpen)\
  X(fa_utf8openobject,          Utf8OpenObject)\
  X(fa_wideopenobject,          WideOpenObject)\
  X(fa_utf8readlink,            Utf8Readlink)\
  X(fa_widereadlink,            WideReadlink)\
  X(fa_utf8readlinkobject,      Utf8ReadlinkObject)\
  X(fa_widereadlinkobject,      WideReadlinkObject)\


DEE_A_RET_EXCEPT(-1) int DeeFS_SetAPIMode(DEE_A_IN Dee_fsapimode_t mode) {
 if ((mode&~(DEE_FSAPIMODE_MASK))!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid FS-ApiMode: %#.8I32x",mode);
  return -1;
 }
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
 if ((mode&DEE_FSAPIMODE_ENABLEVFS)!=0) {
  // Link against the virtual file system
#define MAP(name,api) DeeFSApi_Active.name = &DeeVFS_##api;
  MAP_PRIVATE_FSAPI_EXPORTS(MAP)
#undef MAP
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */
 {
  // Directly link against the native filesystem
#define MAP(name,api) DeeFSApi_Active.name = &DeeNFS_##api;
  MAP_PRIVATE_FSAPI_EXPORTS(MAP)
#undef MAP
 }
 if ((mode&DEE_FSAPIMODE_EXPANDVARS)!=0) {
  // Link against the expand-fs functions.
#define MAP(name,api) DeeFSApi_Active.name = DeeXFS_##api;
  MAP_PUBLIC_FSAPI_EXPORTS(MAP)
#undef MAP
 } else {
  // Configure public functions as aliases of private functions.
#define MAP(name,api) DeeFSApi_Active.name = DeeFSApi_Active._##name;
  MAP_PUBLIC_FSAPI_EXPORTS(MAP)
#undef MAP
 }
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return 0;
}

#undef MAP_PUBLIC_FSAPI_EXPORTS
#undef MAP_PRIVATE_FSAPI_EXPORTS

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_DYNAMIC_FS_C */
