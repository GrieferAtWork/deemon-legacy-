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
 PDEEFS_UTF8TRYCHDIR                 fa_utf8trychdir;
 PDEEFS_WIDETRYCHDIR                 fa_widetrychdir;
 PDEEFS_UTF8TRYCHDIROBJECT           fa_utf8trychdirobject;
 PDEEFS_WIDETRYCHDIROBJECT           fa_widetrychdirobject;
 PDEEFS_UTF8TRYCHDIR                _fa_utf8trychdir;
 PDEEFS_WIDETRYCHDIR                _fa_widetrychdir;
 PDEEFS_UTF8TRYCHDIROBJECT          _fa_utf8trychdirobject;
 PDEEFS_WIDETRYCHDIROBJECT          _fa_widetrychdirobject;
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
 PDEEFS_UTF8TRYHASENV                fa_utf8tryhasenv;
 PDEEFS_WIDETRYHASENV                fa_widetryhasenv;
 PDEEFS_UTF8TRYHASENVOBJECT          fa_utf8tryhasenvobject;
 PDEEFS_WIDETRYHASENVOBJECT          fa_widetryhasenvobject;
 PDEEFS_UTF8TRYDELENV                fa_utf8trydelenv;
 PDEEFS_WIDETRYDELENV                fa_widetrydelenv;
 PDEEFS_UTF8TRYDELENVOBJECT          fa_utf8trydelenvobject;
 PDEEFS_WIDETRYDELENVOBJECT          fa_widetrydelenvobject;
 PDEEFS_UTF8TRYSETENV                fa_utf8trysetenv;
 PDEEFS_WIDETRYSETENV                fa_widetrysetenv;
 PDEEFS_UTF8TRYSETENVOBJECT          fa_utf8trysetenvobject;
 PDEEFS_WIDETRYSETENVOBJECT          fa_widetrysetenvobject;
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
 PDEEFS_UTF8TRYGETTIMES              fa_utf8trygettimes;
 PDEEFS_WIDETRYGETTIMES              fa_widetrygettimes;
 PDEEFS_UTF8TRYGETTIMESOBJECT        fa_utf8trygettimesobject;
 PDEEFS_WIDETRYGETTIMESOBJECT        fa_widetrygettimesobject;
 PDEEFS_UTF8TRYGETTIMES             _fa_utf8trygettimes;
 PDEEFS_WIDETRYGETTIMES             _fa_widetrygettimes;
 PDEEFS_UTF8TRYGETTIMESOBJECT       _fa_utf8trygettimesobject;
 PDEEFS_WIDETRYGETTIMESOBJECT       _fa_widetrygettimesobject;
 PDEEFS_UTF8TRYSETTIMES              fa_utf8trysettimes;
 PDEEFS_WIDETRYSETTIMES              fa_widetrysettimes;
 PDEEFS_UTF8TRYSETTIMESOBJECT        fa_utf8trysettimesobject;
 PDEEFS_WIDETRYSETTIMESOBJECT        fa_widetrysettimesobject;
 PDEEFS_UTF8TRYSETTIMES             _fa_utf8trysettimes;
 PDEEFS_WIDETRYSETTIMES             _fa_widetrysettimes;
 PDEEFS_UTF8TRYSETTIMESOBJECT       _fa_utf8trysettimesobject;
 PDEEFS_WIDETRYSETTIMESOBJECT       _fa_widetrysettimesobject;
 PDEEFS_UTF8HASPROPERTY              fa_utf8hasproperty;
 PDEEFS_WIDEHASPROPERTY              fa_widehasproperty;
 PDEEFS_UTF8HASPROPERTYOBJECT        fa_utf8haspropertyobject;
 PDEEFS_WIDEHASPROPERTYOBJECT        fa_widehaspropertyobject;
 PDEEFS_UTF8HASPROPERTY             _fa_utf8hasproperty;
 PDEEFS_WIDEHASPROPERTY             _fa_widehasproperty;
 PDEEFS_UTF8HASPROPERTYOBJECT       _fa_utf8haspropertyobject;
 PDEEFS_WIDEHASPROPERTYOBJECT       _fa_widehaspropertyobject;
 PDEEFS_UTF8TRYHASPROPERTY           fa_utf8tryhasproperty;
 PDEEFS_WIDETRYHASPROPERTY           fa_widetryhasproperty;
 PDEEFS_UTF8TRYHASPROPERTYOBJECT     fa_utf8tryhaspropertyobject;
 PDEEFS_WIDETRYHASPROPERTYOBJECT     fa_widetryhaspropertyobject;
 PDEEFS_UTF8TRYHASPROPERTY          _fa_utf8tryhasproperty;
 PDEEFS_WIDETRYHASPROPERTY          _fa_widetryhasproperty;
 PDEEFS_UTF8TRYHASPROPERTYOBJECT    _fa_utf8tryhaspropertyobject;
 PDEEFS_WIDETRYHASPROPERTYOBJECT    _fa_widetryhaspropertyobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8remove;
 PDEEFS_WIDEPATHFUN                  fa_wideremove;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8removeobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_wideremoveobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8remove;
 PDEEFS_WIDEPATHFUN                 _fa_wideremove;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8removeobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_wideremoveobject;
 PDEEFS_UTF8TRYPATHFUN               fa_utf8tryremove;
 PDEEFS_WIDETRYPATHFUN               fa_widetryremove;
 PDEEFS_UTF8TRYPATHFUNOBJECT         fa_utf8tryremoveobject;
 PDEEFS_WIDETRYPATHFUNOBJECT         fa_widetryremoveobject;
 PDEEFS_UTF8TRYPATHFUN              _fa_utf8tryremove;
 PDEEFS_WIDETRYPATHFUN              _fa_widetryremove;
 PDEEFS_UTF8TRYPATHFUNOBJECT        _fa_utf8tryremoveobject;
 PDEEFS_WIDETRYPATHFUNOBJECT        _fa_widetryremoveobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8unlink;
 PDEEFS_WIDEPATHFUN                  fa_wideunlink;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8unlinkobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_wideunlinkobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8unlink;
 PDEEFS_WIDEPATHFUN                 _fa_wideunlink;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8unlinkobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_wideunlinkobject;
 PDEEFS_UTF8TRYPATHFUN               fa_utf8tryunlink;
 PDEEFS_WIDETRYPATHFUN               fa_widetryunlink;
 PDEEFS_UTF8TRYPATHFUNOBJECT         fa_utf8tryunlinkobject;
 PDEEFS_WIDETRYPATHFUNOBJECT         fa_widetryunlinkobject;
 PDEEFS_UTF8TRYPATHFUN              _fa_utf8tryunlink;
 PDEEFS_WIDETRYPATHFUN              _fa_widetryunlink;
 PDEEFS_UTF8TRYPATHFUNOBJECT        _fa_utf8tryunlinkobject;
 PDEEFS_WIDETRYPATHFUNOBJECT        _fa_widetryunlinkobject;
 PDEEFS_UTF8PATHFUN                  fa_utf8rmdir;
 PDEEFS_WIDEPATHFUN                  fa_widermdir;
 PDEEFS_UTF8PATHFUNOBJECT            fa_utf8rmdirobject;
 PDEEFS_WIDEPATHFUNOBJECT            fa_widermdirobject;
 PDEEFS_UTF8PATHFUN                 _fa_utf8rmdir;
 PDEEFS_WIDEPATHFUN                 _fa_widermdir;
 PDEEFS_UTF8PATHFUNOBJECT           _fa_utf8rmdirobject;
 PDEEFS_WIDEPATHFUNOBJECT           _fa_widermdirobject;
 PDEEFS_UTF8TRYPATHFUN               fa_utf8tryrmdir;
 PDEEFS_WIDETRYPATHFUN               fa_widetryrmdir;
 PDEEFS_UTF8TRYPATHFUNOBJECT         fa_utf8tryrmdirobject;
 PDEEFS_WIDETRYPATHFUNOBJECT         fa_widetryrmdirobject;
 PDEEFS_UTF8TRYPATHFUN              _fa_utf8tryrmdir;
 PDEEFS_WIDETRYPATHFUN              _fa_widetryrmdir;
 PDEEFS_UTF8TRYPATHFUNOBJECT        _fa_utf8tryrmdirobject;
 PDEEFS_WIDETRYPATHFUNOBJECT        _fa_widetryrmdirobject;
 PDEEFS_UTF8MKDIR                    fa_utf8mkdir;
 PDEEFS_WIDEMKDIR                    fa_widemkdir;
 PDEEFS_UTF8MKDIROBJECT              fa_utf8mkdirobject;
 PDEEFS_WIDEMKDIROBJECT              fa_widemkdirobject;
 PDEEFS_UTF8MKDIR                   _fa_utf8mkdir;
 PDEEFS_WIDEMKDIR                   _fa_widemkdir;
 PDEEFS_UTF8MKDIROBJECT             _fa_utf8mkdirobject;
 PDEEFS_WIDEMKDIROBJECT             _fa_widemkdirobject;
 PDEEFS_UTF8TRYMKDIR                 fa_utf8trymkdir;
 PDEEFS_WIDETRYMKDIR                 fa_widetrymkdir;
 PDEEFS_UTF8TRYMKDIROBJECT           fa_utf8trymkdirobject;
 PDEEFS_WIDETRYMKDIROBJECT           fa_widetrymkdirobject;
 PDEEFS_UTF8TRYMKDIR                _fa_utf8trymkdir;
 PDEEFS_WIDETRYMKDIR                _fa_widetrymkdir;
 PDEEFS_UTF8TRYMKDIROBJECT          _fa_utf8trymkdirobject;
 PDEEFS_WIDETRYMKDIROBJECT          _fa_widetrymkdirobject;
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
  DeeDFS(Utf8TryChdir),                 /*<  fa_utf8trychdir. */
  DeeDFS(WideTryChdir),                 /*<  fa_widetrychdir. */
  DeeDFS(Utf8TryChdirObject),           /*<  fa_utf8trychdirobject. */
  DeeDFS(WideTryChdirObject),           /*<  fa_widetrychdirobject. */
 _DeeDFS(Utf8TryChdir),                 /*< _fa_utf8trychdir. */
 _DeeDFS(WideTryChdir),                 /*< _fa_widetrychdir. */
 _DeeDFS(Utf8TryChdirObject),           /*< _fa_utf8trychdirobject. */
 _DeeDFS(WideTryChdirObject),           /*< _fa_widetrychdirobject. */
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
 &DeeNFS_Utf8TryHasEnv,                 /*<  fa_utf8tryhasenv. */
 &DeeNFS_WideTryHasEnv,                 /*<  fa_widetryhasenv. */
 &DeeNFS_Utf8TryHasEnvObject,           /*<  fa_utf8tryhasenvobject. */
 &DeeNFS_WideTryHasEnvObject,           /*<  fa_widetryhasenvobject. */
 &DeeNFS_Utf8TryDelEnv,                 /*<  fa_utf8trydelenv. */
 &DeeNFS_WideTryDelEnv,                 /*<  fa_widetrydelenv. */
 &DeeNFS_Utf8TryDelEnvObject,           /*<  fa_utf8trydelenvobject. */
 &DeeNFS_WideTryDelEnvObject,           /*<  fa_widetrydelenvobject. */
 &DeeNFS_Utf8TrySetEnv,                 /*<  fa_utf8trysetenv. */
 &DeeNFS_WideTrySetEnv,                 /*<  fa_widetrysetenv. */
 &DeeNFS_Utf8TrySetEnvObject,           /*<  fa_utf8trysetenvobject. */
 &DeeNFS_WideTrySetEnvObject,           /*<  fa_widetrysetenvobject. */
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
  DeeDFS(Utf8TryGetTimes),              /*<  fa_utf8trygettimes. */
  DeeDFS(WideTryGetTimes),              /*<  fa_widetrygettimes. */
  DeeDFS(Utf8TryGetTimesObject),        /*<  fa_utf8trygettimesobject. */
  DeeDFS(WideTryGetTimesObject),        /*<  fa_widetrygettimesobject. */
 _DeeDFS(Utf8TryGetTimes),              /*< _fa_utf8trygettimes. */
 _DeeDFS(WideTryGetTimes),              /*< _fa_widetrygettimes. */
 _DeeDFS(Utf8TryGetTimesObject),        /*< _fa_utf8trygettimesobject. */
 _DeeDFS(WideTryGetTimesObject),        /*< _fa_widetrygettimesobject. */
  DeeDFS(Utf8TrySetTimes),              /*<  fa_utf8trysettimes. */
  DeeDFS(WideTrySetTimes),              /*<  fa_widetrysettimes. */
  DeeDFS(Utf8TrySetTimesObject),        /*<  fa_utf8trysettimesobject. */
  DeeDFS(WideTrySetTimesObject),        /*<  fa_widetrysettimesobject. */
 _DeeDFS(Utf8TrySetTimes),              /*< _fa_utf8trysettimes. */
 _DeeDFS(WideTrySetTimes),              /*< _fa_widetrysettimes. */
 _DeeDFS(Utf8TrySetTimesObject),        /*< _fa_utf8trysettimesobject. */
 _DeeDFS(WideTrySetTimesObject),        /*< _fa_widetrysettimesobject. */
  DeeDFS(Utf8HasProperty),              /*<  fa_utf8hasproperty. */
  DeeDFS(WideHasProperty),              /*<  fa_widehasproperty. */
  DeeDFS(Utf8HasPropertyObject),        /*<  fa_utf8haspropertyobject. */
  DeeDFS(WideHasPropertyObject),        /*<  fa_widehaspropertyobject. */
 _DeeDFS(Utf8HasProperty),              /*< _fa_utf8hasproperty. */
 _DeeDFS(WideHasProperty),              /*< _fa_widehasproperty. */
 _DeeDFS(Utf8HasPropertyObject),        /*< _fa_utf8haspropertyobject. */
 _DeeDFS(WideHasPropertyObject),        /*< _fa_widehaspropertyobject. */
  DeeDFS(Utf8TryHasProperty),           /*<  fa_utf8tryhasproperty. */
  DeeDFS(WideTryHasProperty),           /*<  fa_widetryhasproperty. */
  DeeDFS(Utf8TryHasPropertyObject),     /*<  fa_utf8tryhaspropertyobject. */
  DeeDFS(WideTryHasPropertyObject),     /*<  fa_widetryhaspropertyobject. */
 _DeeDFS(Utf8TryHasProperty),           /*< _fa_utf8tryhasproperty. */
 _DeeDFS(WideTryHasProperty),           /*< _fa_widetryhasproperty. */
 _DeeDFS(Utf8TryHasPropertyObject),     /*< _fa_utf8tryhaspropertyobject. */
 _DeeDFS(WideTryHasPropertyObject),     /*< _fa_widetryhaspropertyobject. */
  DeeDFS(Utf8Remove),                   /*<  fa_utf8remove. */
  DeeDFS(WideRemove),                   /*<  fa_wideremove. */
  DeeDFS(Utf8RemoveObject),             /*<  fa_utf8removeobject. */
  DeeDFS(WideRemoveObject),             /*<  fa_wideremoveobject. */
 _DeeDFS(Utf8Remove),                   /*< _fa_utf8remove. */
 _DeeDFS(WideRemove),                   /*< _fa_wideremove. */
 _DeeDFS(Utf8RemoveObject),             /*< _fa_utf8removeobject. */
 _DeeDFS(WideRemoveObject),             /*< _fa_wideremoveobject. */
  DeeDFS(Utf8TryRemove),                /*<  fa_utf8tryremove. */
  DeeDFS(WideTryRemove),                /*<  fa_widetryremove. */
  DeeDFS(Utf8TryRemoveObject),          /*<  fa_utf8tryremoveobject. */
  DeeDFS(WideTryRemoveObject),          /*<  fa_widetryremoveobject. */
 _DeeDFS(Utf8TryRemove),                /*< _fa_utf8tryremove. */
 _DeeDFS(WideTryRemove),                /*< _fa_widetryremove. */
 _DeeDFS(Utf8TryRemoveObject),          /*< _fa_utf8tryremoveobject. */
 _DeeDFS(WideTryRemoveObject),          /*< _fa_widetryremoveobject. */
  DeeDFS(Utf8Unlink),                   /*<  fa_utf8unlink. */
  DeeDFS(WideUnlink),                   /*<  fa_wideunlink. */
  DeeDFS(Utf8UnlinkObject),             /*<  fa_utf8unlinkobject. */
  DeeDFS(WideUnlinkObject),             /*<  fa_wideunlinkobject. */
 _DeeDFS(Utf8Unlink),                   /*< _fa_utf8unlink. */
 _DeeDFS(WideUnlink),                   /*< _fa_wideunlink. */
 _DeeDFS(Utf8UnlinkObject),             /*< _fa_utf8unlinkobject. */
 _DeeDFS(WideUnlinkObject),             /*< _fa_wideunlinkobject. */
  DeeDFS(Utf8TryUnlink),                /*<  fa_utf8tryunlink. */
  DeeDFS(WideTryUnlink),                /*<  fa_widetryunlink. */
  DeeDFS(Utf8TryUnlinkObject),          /*<  fa_utf8tryunlinkobject. */
  DeeDFS(WideTryUnlinkObject),          /*<  fa_widetryunlinkobject. */
 _DeeDFS(Utf8TryUnlink),                /*< _fa_utf8tryunlink. */
 _DeeDFS(WideTryUnlink),                /*< _fa_widetryunlink. */
 _DeeDFS(Utf8TryUnlinkObject),          /*< _fa_utf8tryunlinkobject. */
 _DeeDFS(WideTryUnlinkObject),          /*< _fa_widetryunlinkobject. */
  DeeDFS(Utf8RmDir),                    /*<  fa_utf8rmdir. */
  DeeDFS(WideRmDir),                    /*<  fa_widermdir. */
  DeeDFS(Utf8RmDirObject),              /*<  fa_utf8rmdirobject. */
  DeeDFS(WideRmDirObject),              /*<  fa_widermdirobject. */
 _DeeDFS(Utf8RmDir),                    /*< _fa_utf8rmdir. */
 _DeeDFS(WideRmDir),                    /*< _fa_widermdir. */
 _DeeDFS(Utf8RmDirObject),              /*< _fa_utf8rmdirobject. */
 _DeeDFS(WideRmDirObject),              /*< _fa_widermdirobject. */
  DeeDFS(Utf8TryRmDir),                 /*<  fa_utf8tryrmdir. */
  DeeDFS(WideTryRmDir),                 /*<  fa_widetryrmdir. */
  DeeDFS(Utf8TryRmDirObject),           /*<  fa_utf8tryrmdirobject. */
  DeeDFS(WideTryRmDirObject),           /*<  fa_widetryrmdirobject. */
 _DeeDFS(Utf8TryRmDir),                 /*< _fa_utf8tryrmdir. */
 _DeeDFS(WideTryRmDir),                 /*< _fa_widetryrmdir. */
 _DeeDFS(Utf8TryRmDirObject),           /*< _fa_utf8tryrmdirobject. */
 _DeeDFS(WideTryRmDirObject),           /*< _fa_widetryrmdirobject. */
  DeeDFS(Utf8MkDir),                    /*<  fa_utf8mkdir. */
  DeeDFS(WideMkDir),                    /*<  fa_widemkdir. */
  DeeDFS(Utf8MkDirObject),              /*<  fa_utf8mkdirobject. */
  DeeDFS(WideMkDirObject),              /*<  fa_widemkdirobject. */
 _DeeDFS(Utf8MkDir),                    /*< _fa_utf8mkdir. */
 _DeeDFS(WideMkDir),                    /*< _fa_widemkdir. */
 _DeeDFS(Utf8MkDirObject),              /*< _fa_utf8mkdirobject. */
 _DeeDFS(WideMkDirObject),              /*< _fa_widemkdirobject. */
  DeeDFS(Utf8TryMkDir),                 /*<  fa_utf8trymkdir. */
  DeeDFS(WideTryMkDir),                 /*<  fa_widetrymkdir. */
  DeeDFS(Utf8TryMkDirObject),           /*<  fa_utf8trymkdirobject. */
  DeeDFS(WideTryMkDirObject),           /*<  fa_widetrymkdirobject. */
 _DeeDFS(Utf8TryMkDir),                 /*< _fa_utf8trymkdir. */
 _DeeDFS(WideTryMkDir),                 /*< _fa_widetrymkdir. */
 _DeeDFS(Utf8TryMkDirObject),           /*< _fa_utf8trymkdirobject. */
 _DeeDFS(WideTryMkDirObject),           /*< _fa_widetrymkdirobject. */
};

DEE_A_RET_NEVER_NULL DeeFunPointer DeeFS_GetFunction(DEE_A_IN Dee_size_t id) {
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
  X(_fa_utf8trychdir,            Utf8TryChdir)\
  X(_fa_widetrychdir,            WideTryChdir)\
  X(_fa_utf8trychdirobject,      Utf8TryChdirObject)\
  X(_fa_widetrychdirobject,      WideTryChdirObject)\
  X(_fa_utf8gettimes,            Utf8GetTimes)\
  X(_fa_widegettimes,            WideGetTimes)\
  X(_fa_utf8gettimesobject,      Utf8GetTimesObject)\
  X(_fa_widegettimesobject,      WideGetTimesObject)\
  X(_fa_utf8settimes,            Utf8SetTimes)\
  X(_fa_widesettimes,            WideSetTimes)\
  X(_fa_utf8settimesobject,      Utf8SetTimesObject)\
  X(_fa_widesettimesobject,      WideSetTimesObject)\
  X(_fa_utf8trygettimes,         Utf8TryGetTimes)\
  X(_fa_widetrygettimes,         WideTryGetTimes)\
  X(_fa_utf8trygettimesobject,   Utf8TryGetTimesObject)\
  X(_fa_widetrygettimesobject,   WideTryGetTimesObject)\
  X(_fa_utf8trysettimes,         Utf8TrySetTimes)\
  X(_fa_widetrysettimes,         WideTrySetTimes)\
  X(_fa_utf8trysettimesobject,   Utf8TrySetTimesObject)\
  X(_fa_widetrysettimesobject,   WideTrySetTimesObject)\
  X(_fa_utf8hasproperty,         Utf8HasProperty)\
  X(_fa_widehasproperty,         WideHasProperty)\
  X(_fa_utf8haspropertyobject,   Utf8HasPropertyObject)\
  X(_fa_widehaspropertyobject,   WideHasPropertyObject)\
  X(_fa_utf8tryhasproperty,      Utf8TryHasProperty)\
  X(_fa_widetryhasproperty,      WideTryHasProperty)\
  X(_fa_utf8tryhaspropertyobject,Utf8TryHasPropertyObject)\
  X(_fa_widetryhaspropertyobject,WideTryHasPropertyObject)\
  X(_fa_utf8remove,              Utf8Remove)\
  X(_fa_wideremove,              WideRemove)\
  X(_fa_utf8removeobject,        Utf8RemoveObject)\
  X(_fa_wideremoveobject,        WideRemoveObject)\
  X(_fa_utf8tryremove,           Utf8TryRemove)\
  X(_fa_widetryremove,           WideTryRemove)\
  X(_fa_utf8tryremoveobject,     Utf8TryRemoveObject)\
  X(_fa_widetryremoveobject,     WideTryRemoveObject)\
  X(_fa_utf8unlink,              Utf8Unlink)\
  X(_fa_wideunlink,              WideUnlink)\
  X(_fa_utf8unlinkobject,        Utf8UnlinkObject)\
  X(_fa_wideunlinkobject,        WideUnlinkObject)\
  X(_fa_utf8tryunlink,           Utf8TryUnlink)\
  X(_fa_widetryunlink,           WideTryUnlink)\
  X(_fa_utf8tryunlinkobject,     Utf8TryUnlinkObject)\
  X(_fa_widetryunlinkobject,     WideTryUnlinkObject)\
  X(_fa_utf8rmdir,               Utf8RmDir)\
  X(_fa_widermdir,               WideRmDir)\
  X(_fa_utf8rmdirobject,         Utf8RmDirObject)\
  X(_fa_widermdirobject,         WideRmDirObject)\
  X(_fa_utf8tryrmdir,            Utf8TryRmDir)\
  X(_fa_widetryrmdir,            WideTryRmDir)\
  X(_fa_utf8tryrmdirobject,      Utf8TryRmDirObject)\
  X(_fa_widetryrmdirobject,      WideTryRmDirObject)\
  X(_fa_utf8mkdir,               Utf8MkDir)\
  X(_fa_widemkdir,               WideMkDir)\
  X(_fa_utf8mkdirobject,         Utf8MkDirObject)\
  X(_fa_widemkdirobject,         WideMkDirObject)\
  X(_fa_utf8trymkdir,            Utf8TryMkDir)\
  X(_fa_widetrymkdir,            WideTryMkDir)\
  X(_fa_utf8trymkdirobject,      Utf8TryMkDirObject)\
  X(_fa_widetrymkdirobject,      WideTryMkDirObject)\

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
  X(fa_utf8trychdir,            Utf8TryChdir)\
  X(fa_widetrychdir,            WideTryChdir)\
  X(fa_utf8trychdirobject,      Utf8TryChdirObject)\
  X(fa_widetrychdirobject,      WideTryChdirObject)\
  X(fa_utf8gettimes,            Utf8GetTimes)\
  X(fa_widegettimes,            WideGetTimes)\
  X(fa_utf8gettimesobject,      Utf8GetTimesObject)\
  X(fa_widegettimesobject,      WideGetTimesObject)\
  X(fa_utf8settimes,            Utf8SetTimes)\
  X(fa_widesettimes,            WideSetTimes)\
  X(fa_utf8settimesobject,      Utf8SetTimesObject)\
  X(fa_widesettimesobject,      WideSetTimesObject)\
  X(fa_utf8trygettimes,         Utf8TryGetTimes)\
  X(fa_widetrygettimes,         WideTryGetTimes)\
  X(fa_utf8trygettimesobject,   Utf8TryGetTimesObject)\
  X(fa_widetrygettimesobject,   WideTryGetTimesObject)\
  X(fa_utf8trysettimes,         Utf8TrySetTimes)\
  X(fa_widetrysettimes,         WideTrySetTimes)\
  X(fa_utf8trysettimesobject,   Utf8TrySetTimesObject)\
  X(fa_widetrysettimesobject,   WideTrySetTimesObject)\
  X(fa_utf8hasproperty,         Utf8HasProperty)\
  X(fa_widehasproperty,         WideHasProperty)\
  X(fa_utf8haspropertyobject,   Utf8HasPropertyObject)\
  X(fa_widehaspropertyobject,   WideHasPropertyObject)\
  X(fa_utf8tryhasproperty,      Utf8TryHasProperty)\
  X(fa_widetryhasproperty,      WideTryHasProperty)\
  X(fa_utf8tryhaspropertyobject,Utf8TryHasPropertyObject)\
  X(fa_widetryhaspropertyobject,WideTryHasPropertyObject)\
  X(fa_utf8remove,              Utf8Remove)\
  X(fa_wideremove,              WideRemove)\
  X(fa_utf8removeobject,        Utf8RemoveObject)\
  X(fa_wideremoveobject,        WideRemoveObject)\
  X(fa_utf8tryremove,           Utf8TryRemove)\
  X(fa_widetryremove,           WideTryRemove)\
  X(fa_utf8tryremoveobject,     Utf8TryRemoveObject)\
  X(fa_widetryremoveobject,     WideTryRemoveObject)\
  X(fa_utf8unlink,              Utf8Unlink)\
  X(fa_wideunlink,              WideUnlink)\
  X(fa_utf8unlinkobject,        Utf8UnlinkObject)\
  X(fa_wideunlinkobject,        WideUnlinkObject)\
  X(fa_utf8tryunlink,           Utf8TryUnlink)\
  X(fa_widetryunlink,           WideTryUnlink)\
  X(fa_utf8tryunlinkobject,     Utf8TryUnlinkObject)\
  X(fa_widetryunlinkobject,     WideTryUnlinkObject)\
  X(fa_utf8rmdir,               Utf8RmDir)\
  X(fa_widermdir,               WideRmDir)\
  X(fa_utf8rmdirobject,         Utf8RmDirObject)\
  X(fa_widermdirobject,         WideRmDirObject)\
  X(fa_utf8tryrmdir,            Utf8TryRmDir)\
  X(fa_widetryrmdir,            WideTryRmDir)\
  X(fa_utf8tryrmdirobject,      Utf8TryRmDirObject)\
  X(fa_widetryrmdirobject,      WideTryRmDirObject)\
  X(fa_utf8mkdir,               Utf8MkDir)\
  X(fa_widemkdir,               WideMkDir)\
  X(fa_utf8mkdirobject,         Utf8MkDirObject)\
  X(fa_widemkdirobject,         WideMkDirObject)\
  X(fa_utf8trymkdir,            Utf8TryMkDir)\
  X(fa_widetrymkdir,            WideTryMkDir)\
  X(fa_utf8trymkdirobject,      Utf8TryMkDirObject)\
  X(fa_widetrymkdirobject,      WideTryMkDirObject)\


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
