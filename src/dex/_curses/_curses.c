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
#ifndef GUARD_DEEMON_DEX_CURSES_C
#define GUARD_DEEMON_DEX_CURSES_C 1
#define DEE_EXTENSION 1
#ifdef _MSC_VER
#pragma warning(disable: 4365)
#endif

//@ dex.name = _curses
//@ dex.require = <ncursesw/curses.h> ($ sudo apt-get install libncurses5-dev && sudo apt-get install libncursesw5-dev)
//@ linker.lib = ncurses

#include "_curses.h"
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/memberdef.h>
#include <deemon/mp/mutex.h>
#include <deemon/none.h>
#include <deemon/structured.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#ifdef CHTYPE_LONG
#if(CHTYPE_LONG >= 2)
#define SIZEOF_CHTYPE 8
#else
#define SIZEOF_CHTYPE 4
#endif
#else
#define SIZEOF_CHTYPE 2
#endif

#define SIZEOF_ATTR_T SIZEOF_CHTYPE

#if SIZEOF_CHTYPE == 8
#define F_chtype "%I64u"
#elif SIZEOF_CHTYPE == 4
#define F_chtype "%I32u"
#elif SIZEOF_CHTYPE == 2
#define F_chtype "%I16u"
#else
#define F_chtype "%I8u"
#endif

#if SIZEOF_ATTR_T == 8
#define F_attr_t "%I64u"
#elif SIZEOF_ATTR_T == 4
#define F_attr_t "%I32u"
#elif SIZEOF_ATTR_T == 2
#define F_attr_t "%I16u"
#else
#define F_attr_t "%I8u"
#endif

#if SIZEOF_CHTYPE == 8
#define T_chtype  Dee_uint64_t
#elif SIZEOF_CHTYPE == 4
#define T_chtype  Dee_uint32_t
#elif SIZEOF_CHTYPE == 2
#define T_chtype  Dee_uint16_t
#else
#define T_chtype  Dee_uint8_t
#endif



// v Global lock for thread-exclusive access to curses.
static struct DeeNativeMutex _DeeCurses_GlobalLock;
static struct DeeAtomicOnceFlag _deecurses_initflag = DeeAtomicOnceFlag_INIT();
#define DeeCurses_ACQUIRE()         DeeNativeMutex_Acquire(&_DeeCurses_GlobalLock)
#define DeeCurses_ACQUIRENOEXCEPT() DeeNativeMutex_AcquireNoexcept(&_DeeCurses_GlobalLock)
#define DeeCurses_RELEASE()         DeeNativeMutex_ReleaseNoexcept(&_DeeCurses_GlobalLock)

static DEE_A_RET_EXCEPT(-1) int DeeCurses_EnsureInitialized(void) {
 int failed = 0;
 DeeAtomicOnceFlag_RUN(&_deecurses_initflag,{ failed = initscr() == NULL; });
 if DEE_UNLIKELY(failed) {
  DeeError_SET_STRING(&DeeCursesError_Type,
                      "initscr() : Failed to initialize curses");
  return -1;
 }
 return 0;
}
 

DEE_A_RET_EXCEPT(-1) int DeeCurses_AcquireContext(
 DEE_A_OUT struct DeeCursesContext *context, DEE_A_IN Dee_uint32_t flags) {
 int failed;
 if (DeeCurses_ACQUIRE() != 0) return -1;
 failed = 0;
 context->cc_flags = DEE_CURSESCONTEXT_FLAG_NONE;
 DeeAtomicOnceFlag_RUN(&_deecurses_initflag,{
  failed = initscr() == NULL;
  context->cc_flags |= DEE_CURSESCONTEXT_FLAG_WASENDWIN;
 });
 if DEE_UNLIKELY(failed) {
  DeeError_SET_STRING(&DeeCursesError_Type,
                      "initscr() : Failed to initialize curses");
  return -1;
 }
 if (DeeCurses_EnsureInitialized() != 0) {
err_release:
  DeeCurses_RELEASE();
  return -1;
 }
 if ((flags&DEE_CURSES_ACQUIRECONTEXT_FLAG_PUSHSTATE)!=0) {
  context->cc_flags |= (DEE_CURSESCONTEXT_FLAG_HASPROGMODE/*|DEE_CURSESCONTEXT_FLAG_HASSHELLMODE*/);
  if ((context->cc_flags&DEE_CURSESCONTEXT_FLAG_WASENDWIN) == 0 &&
      isendwin()) context->cc_flags |= DEE_CURSESCONTEXT_FLAG_WASENDWIN;
  if (def_prog_mode() != 0) {
   DeeError_SET_STRING(&DeeCursesError_Type,
                       "def_prog_mode() : Failed to acquire curses context");
   goto err_release;
  }
 }
 return 0;
}
void DeeCurses_ReleaseContext(DEE_A_IN struct DeeCursesContext *context) {
 clear(); // Always Clear the screen (for security and as not to leave articacts)
 refresh(); // Also clear the actual screen buffer (may cause a short moment of flicker...)
 if ((context->cc_flags&DEE_CURSESCONTEXT_FLAG_HASSHELLMODE)!=0) reset_shell_mode();
 if ((context->cc_flags&DEE_CURSESCONTEXT_FLAG_HASPROGMODE)!=0) reset_prog_mode();
 if ((context->cc_flags&DEE_CURSESCONTEXT_FLAG_WASENDWIN)!=0) {
  while (!isendwin() && endwin() == OK);
 }
 DeeCurses_RELEASE();
}


#if 1
#define _deecurses_error(funcname) DeeNone_New()
#else
#define _deecurses_error(funcname) (_deecurses_do_error(funcname),NULL)
#endif
static void _deecurses_do_error(char const *funcname) {
 DeeError_SetStringf(&DeeCursesError_Type,
                     "Function %q returned ERR",
                     funcname);
}

#ifndef __INTELLISENSE__
#include "_curses.generated.inl"
#endif



DEE_A_RET_EXCEPT_REF DeeCursesWindowObject *
DeeCursesWindow_New(DEE_A_IN WINDOW *wnd) {
 DeeCursesWindowObject *result;
 DEE_ASSERT(wnd);
 if ((result = DeeObject_MALLOC(DeeCursesWindowObject)) != NULL) {
  DeeObject_INIT(result,&DeeCursesWindow_Type);
  result->w_wnd = wnd;
 }
 return result;
}

//////////////////////////////////////////////////////////////////////////
// DeeCursesWindow VTable
static int _deecurseswindow_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCursesWindowObject *self, DeeObject *args) {
 int nlines = -1,ncols = -1,begin_y = -1,begin_x = -1;
 if (DeeTuple_Unpack(args,"|dddd:newwin",&nlines,&ncols,&begin_y,&begin_x) != 0) return -1;
 if (DeeCurses_ACQUIRE() != 0) return -1;
 if (DeeCurses_EnsureInitialized() != 0) { DeeCurses_RELEASE(); return -1; }
 if (nlines < 0 || nlines > LINES) nlines = LINES;
 if (ncols < 0 || ncols > COLS) ncols = COLS;
 if (begin_y < 0) begin_y = (LINES-nlines)/2;
 if (begin_x < 0) begin_x = (COLS-ncols)/2;
 self->w_wnd = newwin(nlines,ncols,begin_y,begin_x);
 DeeCurses_RELEASE();
 if (!self->w_wnd) {
  DeeError_SetStringf(&DeeCursesError_Type,
                      "newwin(%d,%d,%d,%d) : Failed",
                      nlines,ncols,begin_y,begin_x);
  return -1;
 }
 return 0;
}
static int _deecurseswindow_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCursesWindowObject *self, DeeCursesWindowObject *right) {
 if (DeeCurses_ACQUIRE() != 0) return -1;
 self->w_wnd = dupwin(right->w_wnd);
 DeeCurses_RELEASE();
 if (!self->w_wnd) {
  DeeError_SetStringf(&DeeCursesError_Type,
                      "dupwin(%p) : Failed",
                      right->w_wnd);
  return -1;
 }
 return 0;
}
static int _deecurseswindow_tp_dtor(DeeCursesWindowObject *self) {
 if (self->w_wnd) {
  DeeCurses_ACQUIRENOEXCEPT();
  delwin(self->w_wnd);
  DeeCurses_RELEASE();
 }
 return 0;
}

static DeeCursesWindowObject *_deecurseswindow_subwin(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeCursesWindowObject *result;
 WINDOW *rwin; int nlines = -1,ncols = -1,begin_y = -1,begin_x = -1;
 if (DeeTuple_Unpack(args,"|dddd:subwin",&nlines,&ncols,&begin_y,&begin_x) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 if (nlines < 0 || nlines > LINES) nlines = LINES;
 if (ncols < 0 || ncols > COLS) ncols = COLS;
 if (begin_y < 0) begin_y = (LINES-nlines)/2;
 if (begin_x < 0) begin_x = (COLS-ncols)/2;
 rwin = subwin(self->w_wnd,nlines,ncols,begin_y,begin_x);
 DeeCurses_RELEASE();
 if (!rwin) {
  DeeError_SetStringf(&DeeCursesError_Type,
                      "subwin(%p,%d,%d,%d,%d) : Failed",
                      self->w_wnd,nlines,ncols,begin_y,begin_x);
  return NULL;
 }
 result = DeeCursesWindow_New(rwin);
 if (!result) {
  DeeCurses_ACQUIRENOEXCEPT();
  delwin(rwin);
  DeeCurses_RELEASE();
 }
 return result;
}
static DeeCursesWindowObject *_deecurseswindow_derwin(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeCursesWindowObject *result;
 WINDOW *rwin; int nlines = -1,ncols = -1,begin_y = -1,begin_x = -1;
 if (DeeTuple_Unpack(args,"|dddd:derwin",&nlines,&ncols,&begin_y,&begin_x) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 if (nlines < 0 || nlines > LINES) nlines = LINES;
 if (ncols < 0 || ncols > COLS) ncols = COLS;
 if (begin_y < 0) begin_y = (LINES-nlines)/2;
 if (begin_x < 0) begin_x = (COLS-ncols)/2;
 rwin = derwin(self->w_wnd,nlines,ncols,begin_y,begin_x);
 DeeCurses_RELEASE();
 if (!rwin) {
  DeeError_SetStringf(&DeeCursesError_Type,
                      "derwin(%p,%d,%d,%d,%d) : Failed",
                      self->w_wnd,nlines,ncols,begin_y,begin_x);
  return NULL;
 }
 result = DeeCursesWindow_New(rwin);
 if (!result) {
  DeeCurses_ACQUIRENOEXCEPT();
  delwin(rwin);
  DeeCurses_RELEASE();
 }
 return result;
}
static DeeObject *_deecurseswindow_mvwin(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int y = 0,x = 0;
 if (DeeTuple_Unpack(args,"|dd:mvwin",&y,&x) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 if (mvwin(self->w_wnd,y,x) == ERR) {
  DeeCurses_RELEASE();
  DeeError_SetStringf(&DeeCursesError_Type,
                      "mvwin(%p,%d,%d) : Failed",
                      self->w_wnd,y,x);
  return NULL;
 }
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_mvderwin(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int par_y = 0,par_x = 0;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 if (DeeTuple_Unpack(args,"|dd:mvderwin",&par_y,&par_x) != 0) return NULL;
 if (mvderwin(self->w_wnd,par_y,par_x) == ERR) {
  DeeCurses_RELEASE();
  DeeError_SetStringf(&DeeCursesError_Type,
                      "mvderwin(%p,%d,%d) : Failed",
                      self->w_wnd,par_y,par_x);
  return NULL;
 }
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_syncup(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":syncup") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 wsyncup(self->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_syncdown(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":syncdown") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 wsyncdown(self->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_cursyncup(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":cursyncup") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 wcursyncup(self->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_syncok(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int is_ok,err;
 if (DeeTuple_Unpack(args,"b:syncok",&is_ok) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 err = syncok(self->w_wnd,!!is_ok);
 DeeCurses_RELEASE();
 if (err == ERR) return _deecurses_error("syncok");
 DeeReturn_None;
}
static DeeObject *_deecurseswindow_refresh(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int err;
 if (DeeTuple_Unpack(args,":refresh") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 err = wrefresh(self->w_wnd);
 DeeCurses_RELEASE();
 if (err == ERR) {
  _deecurses_error("wrefresh");
  return NULL;
 }
 DeeReturn_None;
}

static DeeObject *_deecurseswindow_box(
 DeeCursesWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int err; chtype a = '|',b = '-';
 if (DeeTuple_Unpack(args,"|" F_chtype F_chtype ":box",&a,&b) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 err = box(self->w_wnd,a,b);
 DeeCurses_RELEASE();
 if (err == ERR) {
  _deecurses_error("box");
  return NULL;
 }
 DeeReturn_None;
}
DEE_A_RET_EXCEPT_REF DeeObject *
DeeCurses_KeyToStr(DEE_A_IN int key) {
 char const *name;
 if (key >= KEY_MIN && key <= KEY_MAX) {
  name = keyname(key);
  if (name && strcmp(name,"UNKNOWN KEY") == 0) DeeReturn_None;
  if (name) return DeeString_New(name);
 }
 if (key < 0) DeeReturn_None;
 if (key <= 0xFF) DeeReturn_Character(key);
 return DeeObject_New(int,key);
}
DEE_A_RET_WUNUSED int DeeCurses_StrToKey(DEE_A_IN_Z char const *key) {
 int i;
 if (!key[0]) return -1;
 if (!key[1]) return (int)*key; // Character
 for (i = KEY_MIN; i <= KEY_MAX; ++i) {
  char const *name = keyname(i);
  if (name && strcmp(name,key) == 0) return i;
 }
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeCurses_StrToKeyOb(
 DEE_A_INOUT DeeObject *ob, DEE_A_OUT int *result) {
 if (DeeString_Check(ob)) { *result = DeeCurses_StrToKey(DeeString_STR(ob)); return 0; }
 return DeeObject_Cast(int,ob,result);
}


static struct DeeMethodDef _deecurseswindow_tp_methods[] = {
 DEE_METHODDEF_v100("refresh",member(&_deecurseswindow_refresh),"refresh() -> none"),
 DEE_METHODDEF_v100("subwin",member(&_deecurseswindow_subwin),"subwin(int nlines = 0, int ncols = 0, int begin_y = 0, int begin_x = 0) -> WINDOW"),
 DEE_METHODDEF_v100("derwin",member(&_deecurseswindow_derwin),"derwin(int nlines = 0, int ncols = 0, int begin_y = 0, int begin_x = 0) -> WINDOW"),
 DEE_METHODDEF_v100("mvwin",member(&_deecurseswindow_mvwin),"mvwin(int y = 0, int x = 0) -> none"),
 DEE_METHODDEF_v100("mvderwin",member(&_deecurseswindow_mvderwin),"mvderwin(int par_y = 0, int par_x = 0) -> none"),
 DEE_METHODDEF_v100("box",member(&_deecurseswindow_box),"box(chtype v = '|', chtype h = '-') -> none"),
 DEE_METHODDEF_v100("syncup",member(&_deecurseswindow_syncup),"syncup() -> none"),
 DEE_METHODDEF_v100("syncdown",member(&_deecurseswindow_syncdown),"syncdown() -> none"),
 DEE_METHODDEF_v100("cursyncup",member(&_deecurseswindow_cursyncup),"cursyncup() -> none"),
 DEE_METHODDEF_v100("syncok",member(&_deecurseswindow_syncok),"syncok(bf = true) -> none"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeCursesWindow_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("WINDOW"),null,
  member(DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCursesWindowObject),null,
  null,member(&_deecurseswindow_tp_copy_ctor),
  null,member(&_deecurseswindow_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deecurseswindow_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deecurseswindow_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeTypeObject DeeCursesScreen_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("SCREEN"),null,
  member(DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCursesScreenObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeTypeObject DeeCursesError_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("CursesError"),null,null,member(&DeeErrorType_SystemError)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeErrorObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


static DeeObject *_deecurses_LINES_get(void) { return DeeObject_New(int,LINES); }
static DeeObject *_deecurses_COLS_get(void) { return DeeObject_New(int,COLS); }

static DeeCursesWindowObject _deecurses_stdscr = {
 DEE_OBJECT_HEAD_INIT(&DeeCursesWindow_Type),NULL};
static struct DeeAtomicOnceFlag _deecurses_stdscr_initialized = DeeAtomicOnceFlag_INIT();
static DeeCursesWindowObject *_deecurses_stdscr_ctor(void) {
 if (!DeeAtomicOnceFlag_DID_RUN(&_deecurses_initflag)) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Accessing 'stdscr' without calling 'initscr()'");
  return NULL;
 }
 DeeAtomicOnceFlag_RUN(&_deecurses_stdscr_initialized,{
  DeeCurses_ACQUIRENOEXCEPT();
  _deecurses_stdscr.w_wnd = stdscr;
  DeeCurses_RELEASE();
 });
 Dee_INCREF(&_deecurses_stdscr);
 return &_deecurses_stdscr;
}
static void _deecurses_stdscr_dtor(DeeCursesWindowObject *DEE_UNUSED(ob)) { }
static DeeCursesWindowObject *_deecurses_initscr(DeeObject *args) {
 if (DeeTuple_Unpack(args,":initscr") != 0) return NULL;
 if (DeeCurses_EnsureInitialized() != 0) return NULL;
 return _deecurses_stdscr_ctor();
}
static DeeObject *_deecurses_endwin(DeeObject *args) {
 if (DeeTuple_Unpack(args,":endwin") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 if (isendwin()) {
  DeeCurses_RELEASE();
  DeeError_SET_STRING(&DeeCursesError_Type,
                      "Can't endwin(): No open windows");
  return NULL;
 }
 if (endwin() == ERR) {
  DeeCurses_RELEASE();
  return _deecurses_error("endwin");
 }
 DeeCurses_RELEASE();
 DeeReturn_None;
}


//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("CursesError",&DeeCursesError_Type),
 DeeDex_EXPORT_OBJECT("WINDOW",&DeeCursesWindow_Type),
 DeeDex_EXPORT_OBJECT("SCREEN",&DeeCursesScreen_Type),
 DeeDex_EXPORT_GENERATOR("stdscr",&_deecurses_stdscr_ctor,&_deecurses_stdscr_dtor),
 DeeDex_EXPORT_GETSET("LINES",&_deecurses_LINES_get,NULL,NULL),
 DeeDex_EXPORT_GETSET("COLS",&_deecurses_COLS_get,NULL,NULL),
 DeeDex_EXPORT_OBJECT("chtype",DeeObject_TYPE(DEE_PP_CAT_2(uint,DEE_PP_MUL8(SIZEOF_CHTYPE)))),
 DeeDex_EXPORT_OBJECT("attr_t",DeeObject_TYPE(DEE_PP_CAT_2(uint,DEE_PP_MUL8(SIZEOF_ATTR_T)))),
 DeeDex_EXPORT_FUNCTION("initscr",&_deecurses_initscr,"initscr() -> WINDOW"),
 DeeDex_EXPORT_FUNCTION("endwin",&_deecurses_endwin,"endwin() -> none"),
#ifndef __INTELLISENSE__
#include "_curses.generated.exports.inl"
#endif
 DeeDex_EXPORT_END
};

int DeeDex_Main(DEE_A_INOUT struct DeeDexContext *context) {
 switch (context->dc_kind) {
  case DEE_DEXCONTEXTKIND_INITIALIZE:
   if (DeeNativeMutex_Init(&_DeeCurses_GlobalLock) != 0) return -1;
   break;
  case DEE_DEXCONTEXTKIND_FINALIZE:
   if (DeeAtomicOnceFlag_DID_RUN(&_deecurses_stdscr_initialized)) {
    DeeAtomicOnceFlag_RESET(&_deecurses_stdscr_initialized);
    _deecurses_stdscr.w_wnd = NULL;
   }
   if (DeeAtomicOnceFlag_DID_RUN(&_deecurses_initflag)) {
    DeeAtomicOnceFlag_RESET(&_deecurses_initflag);
    DEE_LVERBOSE2("Closing all curses windows on shutdown...\n");
    while (!isendwin() && endwin() == OK) // Force close all windows
     DEE_LVERBOSE2("Terminated one more curses window\n");
   }
   DeeNativeMutex_Quit(&_DeeCurses_GlobalLock);
   break;
  default: break;
 }
 return 0;
}

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "_curses.api.inl"
#include "_curses.keycode.inl"
#endif

#endif /* !GUARD_DEEMON_DEX_CURSES_C */
